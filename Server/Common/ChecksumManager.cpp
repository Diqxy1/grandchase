#include "ChecksumManager.h"
#include "odbc/Odbc.h"
#include <dbg/dbg.hpp >
#include "Serializer/SerPair.h"
#include "Serializer/SerTuple.h"
#include <boost/algorithm/string.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplOstmOperatorW2A(KChecksumManager);
NiImplementRootRTTI(KChecksumManager);

KChecksumManager::KChecksumManager(void)
	:m_nProtocol(0)
	, m_dwTickGap(1000 * 60)
	, m_bForce(true)
	, m_bChecksumCheckEnable(true)
{
	m_dwLastUpdateTick = ::GetTickCount();
}

KChecksumManager::~KChecksumManager(void)
{
}

ImplToStringW(KChecksumManager)
{
	KLocker lock(m_csChecksum);
	START_TOSTRINGW
		<< TOSTRINGW(m_nProtocol)
		<< TOSTRINGW(m_dwLastUpdateTick)
		<< TOSTRINGWb(m_bForce)
		<< TOSTRINGWb(m_bChecksumCheckEnable)
		<< TOSTRINGW(m_mapInvalidHash.size())
		<< TOSTRINGW(m_mapHash.size());

	return stm_;
}

void KChecksumManager::DumpChecksum()
{
	std::wstringstream stm;
	KLocker lock1(m_csChecksum);

	if (!m_mapHash.empty()) {
		std::wstring strCurrentKey;
		std::multimap< std::wstring, SHashInfo >::const_iterator mit;

		stm << L"---- Dump Checksum ----" << std::endl;
		for (mit = m_mapHash.begin(); mit != m_mapHash.end(); ++mit) {
			if (mit->first != strCurrentKey) {
				strCurrentKey = mit->first;
				stm << strCurrentKey << std::endl;
			}
			stm << L" - [" << mit->second.m_strHash << L"] : " << mit->second.m_dwFileSize << std::endl;
		}
	}

	if (!m_mapInvalidHash.empty()) {
		std::wstring strCurrentKey;
		std::multimap< std::wstring, SInvalidHashInfo >::const_iterator mit;

		stm << L"---- Dump Invalid Checksum ----" << std::endl;
		for (mit = m_mapInvalidHash.begin(); mit != m_mapInvalidHash.end(); ++mit) {
			if (mit->first != strCurrentKey) {
				strCurrentKey = mit->first;
				stm << strCurrentKey << std::endl;
			}
			stm << L" - [" << mit->second.m_sInvalidHashInfo.m_strHash << L"] : " << mit->second.m_sInvalidHashInfo.m_dwFileSize << L" : " << mit->second.m_dwCount << std::endl;
		}
	}

	if (!m_mapHash.empty() || !m_mapInvalidHash.empty()) {
		START_LOG(cerr, stm.str()) << END_LOG;
	}
}

void KChecksumManager::UpdateFromDB(KODBC& kOdbc_, const int nVersion_, bool bClearInvalid_)
{
	std::multimap< std::wstring, SHashInfo > mapHash;

	SetVersion(nVersion_);
	{
		// mk8253. 20091109.
		// { call dbo.CCGAClientCheck_select }
		// n return ( @1, @2, @3, @4, @5, @6)
		// @1 ; FileUID  int
		// @2 ; FileName nvarchar(30)
		// @3 ; Protocol int
		// @4 ; FileSize int
		// @5 ; SHA      varchar(100)
		KLocker lock(kOdbc_.GetCS());
		_DBJIF(kOdbc_.Query(L"{ call dbo.CCGAClientCheck_select }"), return, (&kOdbc_));
		while (kOdbc_.Fetch() != SQL_NO_DATA)
		{
			int nFileUID = -1;
			int nVersion = -1;
			SHashInfo sInfo;

			VERIFY_QUERY(kOdbc_ >> nFileUID
				>> sInfo.m_strFileName
				>> nVersion
				>> sInfo.m_dwFileSize
				>> sInfo.m_strHash);

			if (nVersion_ == nVersion)
			{
				boost::to_lower(sInfo.m_strFileName);
				boost::to_lower(sInfo.m_strHash);

				mapHash.insert(std::make_pair(sInfo.m_strFileName, sInfo));
				START_LOG(clog, L"Update " << sInfo.m_strFileName
					<< L" Info..Script FileSize : " << sInfo.m_dwFileSize
					<< L", strHash : " << sInfo.m_strHash) << END_LOG;
			}
		}
	}

	KLocker lock1(m_csChecksum);

	if (bClearInvalid_)
		m_mapInvalidHash.clear();
	m_mapHash.swap(mapHash);
	m_dwLastUpdateTick = ::GetTickCount();
}

bool KChecksumManager::CheckUpdate()
{
	if (!m_bForce && ::GetTickCount() - m_dwLastUpdateTick < m_dwTickGap) // 1분
		return false;

	m_bForce = false;
	m_dwLastUpdateTick = ::GetTickCount();
	return true;
}

void KChecksumManager::Clear()
{
	KLocker lock(m_csChecksum);
	m_mapInvalidHash.clear();
	m_mapHash.clear();
}

void KChecksumManager::SetCheckEnable(bool bEnable_)
{
	KLocker lock(m_csChecksum);
	m_bChecksumCheckEnable = bEnable_;
}

void KChecksumManager::RunTest()
{
	// 내용을 클리어 한다.
	Clear();
	// 체크를 활성화 한다.
	bool bOld = GetCheckEnable();
	SetCheckEnable(true);
	// 가상의 체크섬을 입력한다.
	SHashInfo sInfo(std::wstring(L"kkurrung.kom"), std::wstring(L"kkurrung"), 18);
	SHashInfo sInfo2(std::wstring(L"kkurrung1.kom"), std::wstring(L"kkurrung1"), 19);
	START_LOG(clog, L"정상 등록") << END_LOG;
	AddFakeChecksum(sInfo);

	// 정상적인 체크섬을 체크 한다.
	KODBC odbc;
	START_LOG(clog, L"정상 체크") << END_LOG;
	LIF(CheckHash(odbc, std::wstring(L"kkurrung"), sInfo.m_strFileName, sInfo.m_strHash, sInfo.m_dwFileSize));
	// 비정상적인 체크섬을 체크 한다.
	START_LOG(clog, L"비정상 체크 3종") << END_LOG;
	LIF(CheckHash(odbc, std::wstring(L"kkurrung1"), sInfo.m_strFileName, sInfo2.m_strHash, sInfo.m_dwFileSize));
	LIF(CheckHash(odbc, std::wstring(L"kkurrung1"), sInfo.m_strFileName, sInfo.m_strHash, sInfo2.m_dwFileSize));
	LIF(CheckHash(odbc, std::wstring(L"kkurrung1"), sInfo2.m_strFileName, sInfo.m_strHash, sInfo.m_dwFileSize));
	// 동일한 비정상적인 체크섬을 체크 해서 불량 체크섬의 카운트가 증가한지 확인 한다.
	START_LOG(clog, L"다시 비정상 체크 3종") << END_LOG;
	LIF(CheckHash(odbc, std::wstring(L"kkurrung2"), sInfo.m_strFileName, sInfo2.m_strHash, sInfo.m_dwFileSize));
	LIF(CheckHash(odbc, std::wstring(L"kkurrung2"), sInfo.m_strFileName, sInfo.m_strHash, sInfo2.m_dwFileSize));
	LIF(CheckHash(odbc, std::wstring(L"kkurrung2"), sInfo2.m_strFileName, sInfo.m_strHash, sInfo.m_dwFileSize));
	// 정상 체크섬을 체크 한다.( 통과 되는지 )
	START_LOG(clog, L"다시 정상 체크") << END_LOG;
	LIF(CheckHash(odbc, std::wstring(L"kkurrung3"), sInfo.m_strFileName, sInfo.m_strHash, sInfo.m_dwFileSize));

	START_LOG(clog, L"내용 덤프") << END_LOG;
	DumpChecksum();
	std::cout << *this;
	// 마무리
	Clear();
	UpdateNow();
	SetCheckEnable(bOld);
}

void KChecksumManager::GetHashCheckList(OUT std::set< std::wstring >& setList_)
{
	std::multimap< std::wstring, SHashInfo >::const_iterator mit;
	setList_.clear();

	{
		KLocker lock1(m_csChecksum);
		for (mit = m_mapHash.begin(); mit != m_mapHash.end(); ++mit) {
			setList_.insert(mit->first);
		}
	}

	START_LOG(clog, L"GetHashCheckList. vecList_.size() : " << setList_.size()) << END_LOG;
}

bool KChecksumManager::CheckHash(KODBC& kOdbc_, const std::wstring& strLogin_, const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_)
{
	if (!GetCheckEnable()) {
		START_LOG(clog, L"Hash 체크 안함.. strLogin_ : " << strLogin_ << L",strFileName_ : " << strFileName_ << L",strHash_ : " << strHash_) << END_LOG;
		return true;
	}

	if (FindHash(strFileName_, strHash_, dwFileSize_))
		return true;

	if (FindInvalidHash(strFileName_, strHash_, dwFileSize_))
		return false;

	// 등록된 값중에 없으면 DB에서 정보 얻어오기
	UpdateFromDB(kOdbc_, GetVersion(), false);

	// 다시 찾아서 없으면 불량 등록.
	if (!FindHash(strFileName_, strHash_, dwFileSize_)) {
		RegInvalidHash(strLogin_, strFileName_, strHash_, dwFileSize_);
		return false;
	}

	return true;
}

bool KChecksumManager::FindHash(const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_)
{
	std::wstring strFileName = boost::to_lower_copy(strFileName_);
	std::wstring strHash = boost::to_lower_copy(strHash_);
	typedef std::multimap< std::wstring, SHashInfo >::const_iterator HASH_IT;

	std::pair<HASH_IT, HASH_IT> prIt;
	HASH_IT mit;

	KLocker lock(m_csChecksum);
	prIt = m_mapHash.equal_range(strFileName);
	if (prIt.first == prIt.second) {
		START_LOG(cerr, L"일치하는 Hash 항목 없음."
			<< L" FileName_ : " << strFileName
			<< L", Hash : " << strHash
			<< L", FileSize : " << dwFileSize_) << END_LOG;
		return false;
	}

	for (mit = prIt.first; mit != prIt.second; ++mit) {
		if (mit->second.m_strHash == strHash &&  mit->second.m_dwFileSize == dwFileSize_) {
			START_LOG(clog, L"일치하는 Hash 항목 찾음"
				<< L" FileName_ : " << strFileName
				<< L", Hash : " << strHash
				<< L", FileSize : " << dwFileSize_) << END_LOG;
			return true;
		}
	}

	START_LOG(cerr, L"일치하는 Hash 항목 없음."
		<< L" FileName_ : " << strFileName
		<< L", Hash : " << strHash
		<< L", FileSize : " << dwFileSize_) << END_LOG;
	return false;
}

bool KChecksumManager::FindInvalidHash(const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_)
{
	std::wstring strFileName = boost::to_lower_copy(strFileName_);
	std::wstring strHash = boost::to_lower_copy(strHash_);
	typedef std::multimap< std::wstring, SInvalidHashInfo >::iterator HASH_IT;

	std::pair<HASH_IT, HASH_IT> prIt;
	HASH_IT mit;

	KLocker lock(m_csChecksum);
	prIt = m_mapInvalidHash.equal_range(strFileName);
	if (prIt.first == prIt.second) {
		START_LOG(clog, L"일치하는 Invalid Hash 항목 없음."
			<< L" FileName_ : " << strFileName
			<< L", Hash : " << strHash
			<< L", FileSize : " << dwFileSize_) << END_LOG;
		return false;
	}

	for (mit = prIt.first; mit != prIt.second; ++mit) {
		if (mit->second.m_sInvalidHashInfo.m_strHash == strHash &&  mit->second.m_sInvalidHashInfo.m_dwFileSize == dwFileSize_) {
			START_LOG(cwarn, L"일치하는 Invalid Hash 항목 찾음"
				<< L" FileName_ : " << strFileName
				<< L", Hash : " << strHash
				<< L", FileSize : " << dwFileSize_) << END_LOG;
			++mit->second.m_dwCount;
			return true;
		}
	}

	START_LOG(clog, L"일치하는 Invalid Hash 항목 없음."
		<< L" FileName_ : " << strFileName
		<< L", Hash : " << strHash
		<< L", FileSize : " << dwFileSize_) << END_LOG;
	return false;
}

void KChecksumManager::RegInvalidHash(const std::wstring& strLogin_, const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_)
{
	std::wstring strFileName = boost::to_lower_copy(strFileName_);
	std::wstring strHash = boost::to_lower_copy(strHash_);
	typedef std::multimap< std::wstring, SInvalidHashInfo >::iterator HASH_IT;

	std::pair<HASH_IT, HASH_IT> prIt;
	HASH_IT mit;

	SInvalidHashInfo sInfo;
	sInfo.m_dwCount = 1;
	sInfo.m_strLogin = strLogin_;
	sInfo.m_tmRegTime = KncUtil::TimeToInt(CTime::GetCurrentTime());
	sInfo.m_sInvalidHashInfo.m_strFileName = strFileName_;
	sInfo.m_sInvalidHashInfo.m_strHash = strHash_;
	sInfo.m_sInvalidHashInfo.m_dwFileSize = dwFileSize_;

	KLocker lock(m_csChecksum);
	prIt = m_mapInvalidHash.equal_range(strFileName);
	if (prIt.first == prIt.second) {
		m_mapInvalidHash.insert(std::make_pair(strFileName_, sInfo));
		return;
	}

	for (mit = prIt.first; mit != prIt.second; ++mit) {
		if (mit->second.m_sInvalidHashInfo.m_strHash == strHash &&  mit->second.m_sInvalidHashInfo.m_dwFileSize == dwFileSize_) {
			START_LOG(clog, L"일치하는 Invalid Hash 항목 찾음"
				<< L" FileName_ : " << strFileName
				<< L", Hash : " << strHash
				<< L", FileSize : " << dwFileSize_) << END_LOG;
			++mit->second.m_dwCount;
			return;
		}
	}

	m_mapInvalidHash.insert(std::make_pair(strFileName_, sInfo));
}

void KChecksumManager::AddFakeChecksum(SHashInfo& sInfo_)
{
	KLocker lock1(m_csChecksum);
	m_mapHash.insert(std::make_pair(sInfo_.m_strFileName, sInfo_));
}