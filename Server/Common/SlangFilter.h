// 070504. woosh.    source code from serious-code.net
#ifndef __SLANGFILTER_H__
#define __SLANGFILTER_H__

//////////////////////////////////////////////////////////////////////////////
/// \class SlangFilter
/// \brief 비속어 필터링 클래스
///
/// 이 클래스는 입력된 비속어를 트리로 만들어 가지고 있는다. 비속어 문자열의
/// 한 바이트, 한 바이트가 노드를 이루게 된다. 예를 들어, "fuck"이란 단어와
/// "fucking"이란 단어가 입력되었다면 대강 다음과 같은 모양을 가지게 된다.
///
/// <pre>
/// f(f) - u(f) - c(f) - k(t) - i(f) - n(f) - g(t)
/// </pre>
///
/// 옆의 f, t는 SlangNode 클래스의 bLeafNode 멤버 변수와 매치되는 값인데,
/// t일 때, 이 노드가 한 단어의 끝임을 말한다. 즉 fuck이란 단어도 있고,
/// "fucking"이란 단어도 있음을 말한다. 위의 트리에 "fox"란 단어도 비속어로
/// 입력되었다면 다음과 같은 모양이 될 것이다.
///
/// <pre>
/// f(f) + u(f) - c(f) - k(t) - i(f) - n(f) - g(t)
///      |
///      + o(f) - x(t)
/// </pre>
///
/// 이런 식으로 트리를 구성해두면, 비속어 데이터베이스가 아무리 커도
/// 비속어 포함 여부를 문자열 길이의 O(N)으로 검사할 수 있다.
//////////////////////////////////////////////////////////////////////////////
#include "KNCSingleton.h"
#include "slangnode.h"
#include <vector>
#include <Thread/Locker.h>

class KSlangFilter
{
	DeclareSingleton(KSlangFilter);
private:
	struct IMPL;
	IMPL* m_pImpl; ///< 내부 데이터 PIMPL

public:
	KSlangFilter(); /// 생성자
	~KSlangFilter(); /// 소멸자

	std::wstring Filter(const std::wstring& original_) const;
	bool HasSlang(const std::wstring& original_, const bool bIsNickName_ = false) const;
	void AddSlang(const std::wstring& slang);
	bool LoadFilter(const char* szFileName);

	int IsValidateNickNameString(const std::wstring& strNickNameString, std::wstring& strName, const unsigned int iNickLength);
	bool IsInQueryInvalidChar(const std::wstring& strString);
	bool IsWrapEmptyChar(const std::wstring& strString);
	void AddLanguage(int iStart, int iEnd); // 071116. woosh. Add Unicode

	bool CheckUnicode(const std::wstring& strNickName);
	void SetEnableUnicode(bool bEnableUnicode);
	int GetMultiByteCount(std::wstring& strString);
	bool CheckExtUnicode(bool bAllow, std::vector< std::pair<DWORD, DWORD> >& vecUnicode, std::wstring& strName);

private:
	size_t Match(const std::wstring& text, const bool bIsNickName_ = false) const;
	bool isPunctuation(wchar_t c) const;

	std::vector< std::pair< int, int > > m_vecUnicode;

	bool m_bEnableUnicode;
	mutable KncCriticalSection          m_csFilter;
};
DefSingletonInline(KSlangFilter);
#endif //__SLANGFILTER_H__