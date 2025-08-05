#pragma once
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>

class KODBC;
class KChecksumManager
{
	NiDeclareRootRTTI(KChecksumManager);
	DeclToStringW;
public:
	struct SHashInfo
	{
		std::wstring    m_strFileName;
		std::wstring    m_strHash;
		DWORD           m_dwFileSize;

		SHashInfo() :m_dwFileSize(0) {}

		SHashInfo(std::wstring& strFileName, std::wstring& strHash, DWORD dwSize) :
			m_strFileName(strFileName), m_strHash(strHash), m_dwFileSize(dwSize) {}
	};

	struct SInvalidHashInfo
	{
		SHashInfo       m_sInvalidHashInfo;
		time_t          m_tmRegTime;
		std::wstring    m_strLogin;
		DWORD           m_dwCount;
	};

public:
	KChecksumManager(void);
	~KChecksumManager(void);

	bool CheckHash(KODBC& kOdbc_, const std::wstring& strLogin_, const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_);
	void UpdateFromDB(KODBC& kOdbc, const int nVersion, bool bClearInvalid_);
	int GetVersion() { KLocker lock(m_csChecksum); return m_nProtocol; }
	void SetVersion(int nVersion) { KLocker lock(m_csChecksum); m_nProtocol = nVersion; }
	bool CheckUpdate();
	void UpdateNow() { KLocker lock(m_csChecksum); m_bForce = true; };
	void RunTest();
	void Clear();
	void SetCheckEnable(bool bEnable);
	void DumpChecksum();
	void GetHashCheckList(OUT std::set< std::wstring >& setList);
	bool GetCheckEnable() { KLocker lock(m_csChecksum); return m_bChecksumCheckEnable; }

protected:
	bool FindHash(const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_);
	bool FindInvalidHash(const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_);
	void RegInvalidHash(const std::wstring& strLogin_, const std::wstring& strFileName_, const std::wstring& strHash_, const DWORD dwFileSize_);

	// for test
	void AddFakeChecksum(SHashInfo& sInfo_);

protected:
	mutable KncCriticalSection  m_csChecksum;
	int     m_nProtocol;
	DWORD   m_dwLastUpdateTick;
	DWORD   m_dwTickGap;
	bool    m_bForce;
	bool    m_bChecksumCheckEnable;

	// Checksum
	std::multimap< std::wstring, SHashInfo >            m_mapHash;
	std::multimap< std::wstring, SInvalidHashInfo >     m_mapInvalidHash;
};
DeclOstmOperatorA(KChecksumManager);