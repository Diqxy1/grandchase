//////////////////////////////////////////////////////////////////////////////
/// 070430. woosh.
///
/// 비속어 필터링
//////////////////////////////////////////////////////////////////////////////
#include <map>
#include <assert.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include "utf16_cvt.h"
#include <boost/algorithm/string.hpp>
#include "dbg/dbg.hpp"
#include "SlangFilter.h"
#include <algorithm>
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }

namespace
{
	class KComp
	{
	public:
		bool operator() (const std::pair<int, int> pr1, const std::pair<int, int> pr2)
		{
			if (pr1.first < pr2.first)
				return true;
			return false;
		}

		bool operator() (const int nVal, const std::pair<int, int> pr)
		{
			if (nVal < pr.first && nVal < pr.second)
				return true;
			return false;
		}

		bool operator() (const std::pair<int, int> pr, const int nVal)
		{
			if (nVal > pr.first && nVal > pr.second)
				return true;
			return false;
		}
	};
};

ImplementSingleton(KSlangFilter);
//////////////////////////////////////////////////////////////////////////////
/// \struct SlangFilter::IMPL
/// \brief SlangFilter 클래스 내부 데이터 구조체
//////////////////////////////////////////////////////////////////////////////
struct KSlangFilter::IMPL
{
	KSlangNode* pRoot; ///< 최상위 노드

	static const std::wstring s_Punctuations; ///< 문장 부호들
	IMPL() : pRoot(new KSlangNode) { } //AssertPtr(pRoot);
	~IMPL() { SAFE_DELETE(pRoot); }
};

/// 문장 부호들
const std::wstring KSlangFilter::IMPL::s_Punctuations = L" `~!@#$%^&*()-_=+\\|[{]};:'\",<.>/?";

KSlangFilter::KSlangFilter() /// 생성자
	:m_pImpl(new IMPL),
	m_bEnableUnicode(false)
{}

KSlangFilter::~KSlangFilter()
{
	if (m_pImpl != NULL) // JIF( m_pImpl ) //뭘 써야 하는걸까  071120. woosh.
		m_pImpl->pRoot->DeleteChild();

	SAFE_DELETE(m_pImpl);
} /// 소멸자

void KSlangFilter::AddSlang(const std::wstring& slang_)
{
	KLocker lock(m_csFilter);

	KSlangNode* pCurrent = m_pImpl->pRoot; //최상위 노드를 정해줘야 한다

	for (unsigned int i = 0; i < slang_.length(); ++i)
	{
		if (pCurrent->isLeafNode())
		{
			return;
		}
		pCurrent = pCurrent->AddChild(slang_[i]);
	}
	pCurrent->setLeafNode(true);
}

size_t KSlangFilter::Match(const std::wstring& text_, const bool bIsNickName_) const
{
	KSlangNode* pCurrent = m_pImpl->pRoot;
	size_t i = 0;

	while (i < text_.size())
	{
		if (isPunctuation(text_[i]) && !bIsNickName_) //
		{
			++i;
			continue;
		}
		pCurrent = pCurrent->FindChild(text_[i]);

		if (pCurrent == NULL) return 0;

		if (pCurrent->isLeafNode()) return i + 1;

		++i;
	}
	return 0;
}

std::wstring KSlangFilter::Filter(const std::wstring& original_) const
{
	std::wstring text(original_);

	for (size_t i = 0; i < original_.size(); )
	{
		size_t size = Match(original_.substr(i, original_.size() - i));
		if (size > 0)
		{
			text.replace(i, size, std::wstring(size, '*')); //한글은 글자당 *를 하나만 할 수 있도록 수정하기(unicode, done)
			i += size;
		}
		else
		{
			i += 1; // for unicode , if not, use next  "i += original[i] & 0x80 ? 2 : 1;"
		}
	}
	return text;
}

bool KSlangFilter::HasSlang(const std::wstring& original_, const bool bIsNickName_) const// Does this Nickname include vulgarity word(s)?
{
	for (size_t i = 0; i < original_.size(); i++)
	{
		if (Match(original_.substr(i, original_.size() - i), bIsNickName_) > 0)
			return true;
	}
	return false;
}

bool KSlangFilter::isPunctuation(wchar_t c) const
{
	if (IMPL::s_Punctuations.find(c) != std::wstring::npos)
		return true;
	return false;
}

bool KSlangFilter::LoadFilter(const char* szFileName)
{
	std::wifstream  m_wFile;
	IMBUE_NULL_CODECVT(m_wFile);
	m_wFile.open(szFileName, std::ios::binary | std::ios::in);

	std::wstring strOut;

	if (!m_wFile.is_open())
	{
		std::cout << "can't file open" << std::endl;
		return false;
	}
	wchar_t chBom[1];
	m_wFile.read(chBom, 1);
	//std::cout <<"Bom: " << *chBom << std::endl; // 봄 내용 읽고, little endian? 아니면 return
	if (*chBom != 0xFEFF) // FEFF = 65279 (little endian), FFFE = 65534 (Big endian)
	{
		return false;
	}

	while (std::getline(m_wFile, strOut, L'\n'))
	{
		boost::trim(strOut);
		if (strOut.empty())
			continue;

		strOut = boost::to_lower_copy(strOut);
		if (strOut[0] != '/') // '/'  == 주석
		{
			AddSlang(strOut);
		}
	}

	if (m_wFile.is_open())
	{
		m_wFile.close();
	}
	return true;
}

bool KSlangFilter::IsInQueryInvalidChar(const std::wstring& strString_)
{
	static wchar_t strProhibition[] = { L' ', L'\t', L'\'', L'\"', L',', L'　', L'\n', L'\r' };

	return (std::find_first_of(strString_.begin(), strString_.end(),
		strProhibition, strProhibition + (sizeof(strProhibition) / sizeof(wchar_t))) != strString_.end());
}

bool KSlangFilter::IsWrapEmptyChar(const std::wstring& strString_)
{
	return (boost::trim_copy(strString_).length() != strString_.length());
}

int KSlangFilter::IsValidateNickNameString(const std::wstring& strNickNameString_, std::wstring& strName_, const unsigned int iNickLength_)
{
	if (strNickNameString_.size() > iNickLength_ || strNickNameString_.size() == 0)
	{
		START_LOG(cwarn, L"닉네임 중에 부적절한 것이 있습니다.(제한크기 초과 또는 크기가 0)")
			<< BUILD_LOG(strName_)
			<< BUILD_LOG(strNickNameString_)
			<< END_LOG;
		return -5;
	}

	if (IsInQueryInvalidChar(strNickNameString_))
	{
		START_LOG(cwarn, L"닉네임 중에 부적절한 것이 있습니다.(빈칸, 탭, \')")
			//<< BUILD_LOG( m_strName )
			<< BUILD_LOG(strNickNameString_)
			<< END_LOG;

		return -4;
	}

	// 닉네임이 사용 가능한것인가.(커스필터)
	// 소문자 변환
	std::wstring wstr = boost::to_lower_copy(strNickNameString_); // 070504. woosh.
	if (SiKSlangFilter()->HasSlang(wstr, true)) // 070626. woosh. 두번째 인자는 닉네임 검사일 때 true
	{
		START_LOG(cwarn, L"닉네임 중에 부적절한 것이 있습니다.(욕설..등)")
			<< BUILD_LOG(strName_)
			<< BUILD_LOG(strNickNameString_)
			<< END_LOG;

		return -4;
	}

	if (CheckUnicode(strNickNameString_))
	{
		// 뭔가 걸리는게 있다
		START_LOG(cwarn, L"닉네임이 적합하지 않음..유니코드 필터")
			<< BUILD_LOG(strName_)
			<< BUILD_LOG(strNickNameString_)
			<< END_LOG;

		return -4;
	}

	return 0;
}

void KSlangFilter::AddLanguage(int iStart_, int iEnd_) // 071116. woosh. Add Unicode
{
	KLocker lock(m_csFilter);

	m_vecUnicode.push_back(std::make_pair(iStart_, iEnd_));

	std::sort(m_vecUnicode.begin(), m_vecUnicode.end(), KComp());
}

void KSlangFilter::SetEnableUnicode(const bool bEnableUnicode_)
{
	// true 이면 벡터에 넣은 유니코드 값들을 허용, false이면 등록된 유니코드 필터
	KLocker lock(m_csFilter);

	m_bEnableUnicode = bEnableUnicode_;

	START_LOG(clog, L"언어셋 허용여부")
		<< BUILD_LOG(m_bEnableUnicode) << END_LOG;
}

bool KSlangFilter::CheckUnicode(const std::wstring& strNickName_) // 071119. woosh.
{
	/*
	KLocker lock(m_csFilter);

	DWORD dwMask;
	if (m_bEnableUnicode)
		dwMask = 2; // 10 , 허용 유니코드
	else
		dwMask = 1; // 01 , 차단 유니코드

	for (unsigned int i = 0; i < strNickName_.size(); ++i)
	{
		DWORD dwValue = 1; // 01

		if (true == std::binary_search(m_vecUnicode.begin(), m_vecUnicode.end(), (int)strNickName_[i], KComp())) // 080324 tslayer. 해당 문자가 설정한 유니코드 리스트 중에 있는가.
		{
			dwValue = 2; // 10
		}

		if ((dwValue & dwMask) <= 0)
		{
			// log
			START_LOG(cwarn, L"사용불가 유니코드가   검출된 경우")
				<< BUILD_LOG(strNickName_)
				<< BUILD_LOG(strNickName_[i])
				<< BUILD_LOG((int)strNickName_[i])
				<< BUILD_LOG(dwValue)
				<< BUILD_LOG(dwMask)
				<< BUILD_LOG(m_bEnableUnicode) << END_LOG;

			return true; // 걸렸네
		}
	}
	*/
	return false;
}

bool KSlangFilter::CheckExtUnicode(bool bAllow_, std::vector< std::pair<DWORD, DWORD> >& vecUnicode_, std::wstring& strName_)
{
	struct Finder
	{
		Finder(DWORD dwVal) : m_dwVal(dwVal) {}
		bool operator() (const std::pair<DWORD, DWORD> pr)
		{
			if (m_dwVal >= pr.first && m_dwVal <= pr.second)
				return true;
			return false;
		}
	private:
		DWORD m_dwVal;
	};

	/*
	Allow  | Unicode | Result
	-------------------------
	true   | true    | true
	true   | false   | false
	false  | true    | false
	false  | false   | true

	if ( Allow ^ Unicode )
		false
	else
		true
	*/

	std::wstring::iterator sit;
	for (sit = strName_.begin(); sit != strName_.end(); ++sit) {
		bool bFind = (std::find_if(vecUnicode_.begin(), vecUnicode_.end(), Finder((DWORD)(*sit))) != vecUnicode_.end());
		if (bAllow_ ^ bFind) return true;
	}
	return false;
}

int KSlangFilter::GetMultiByteCount(std::wstring& strString_)
{
	int nCount = 0;
	wchar_t chBegin = 0x0021;
	wchar_t chEnd = 0x007E;
	std::wstring::iterator sit;
	for (sit = strString_.begin(); sit != strString_.end(); ++sit)
	{
		if (*sit >= chBegin && *sit <= chEnd) ++nCount;
		else nCount += 2;
	}
	return nCount;
}