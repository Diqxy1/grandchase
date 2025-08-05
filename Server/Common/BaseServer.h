#pragma once

#include <KncLua.h>
#include <boost/scoped_ptr.hpp>
#include "Thread/Thread.h"
#include "StringTable.h"
#include <RTTI.h>
#include <ToString.h>
#include <KNCSingleton.h>

class KODBC;
class KDBLayer;
class KNetLayer;
class KSimLayer;
class KBaseServer
{
	NiDeclareRootRTTI(KBaseServer);
	DeclToStringW;

public:
	KBaseServer(void);
	virtual ~KBaseServer(void);

	virtual bool Init(const wchar_t* szFileName);
	virtual void RegToLua();
	virtual void Run(bool bRunInterpreter);   // 콘솔일땐 자체 interpreter 구동, 윈도우일땐 무시.
	virtual void ShutDown();

	bool SetServerName(IN const std::wstring& strServerName);
	void GetServerName(OUT std::wstring& strServerName);
	std::wstring GetServerName();

	void SetProtocolVersion(int nProtocolVer);
	void SetProtocolVersion2(int nProtocolVer);

	void SetServerList(IN const std::vector< KServerInfo >& vecServerList); // DB T. 에서 호출 해야 한다.
	void GetServerList(OUT std::vector< KServerInfo >& vecServerList); // User에서 호출한다.(Main T.)
	void DumpServerList();

	KDBLayer* GetDBLayer() { return m_pkDBLayer; }
	lua_State* GetLuaState() { return m_sptlua.get(); }

	void ReLoadStringTable();

	void SetServerID(IN int& nServerID);
	int GetServerID();

	void GetServerPort(OUT std::wstring& strServerPort);
	void InitLog4cxx();
	void SetLogLevel(IN const int nLogLevel_);

	void SetTCPServerList(IN const std::vector< KSimpleServerInfo >& vecUseTCPServerList, IN const std::vector< KSimpleServerInfo >& vecNoUseTCPServerList);
	void GetTCPServerList(OUT std::vector< KSimpleServerInfo >& vecUseTCPServerList, OUT std::vector< KSimpleServerInfo >& vecNoUseTCPServerList);

protected:
	inline void SetTagVersion();
	inline void LoadINIFile(const wchar_t* szFileName);
	inline void SetCrashMail();

    std::shared_ptr<lua_State>    m_sptlua;
	std::locale                     m_locale;               ///< 060103. 국가별 OS마다 다르게 적용.
	KNetLayer*                      m_pkNetLayer;
	KSimLayer*                      m_pkSimLayer;
	KDBLayer*                       m_pkDBLayer;
	std::wstring                    m_strTagVersion;        ///< 050716. florist. 태그빌드 버전 문자열

	KncCriticalSection  m_csServerName;         // 서버 이름 보호. DB T.에서 쓰고 Main T.에서 읽는다.
	std::wstring        m_strServerName;        // 051005. florist. 서버 이름. DB에서 매분 읽어온다.
	std::wstring        m_strIniFilePath;
	int                 m_nServerID;

	// 060207. kkurrung. Server Info List
	DWORD                       m_dwServerInfoUpdateTick; // 다중스레드를 고려할 필요 없다.
	mutable KncCriticalSection  m_csServerInfo;
	std::vector< KServerInfo >  m_vecServerInfoList; // 게임서버 리스트.
	std::vector< KSimpleServerInfo > m_vecNoUseTCPServerInfoList; // 사용불가 TCP 서버 리스트.
	std::vector< KSimpleServerInfo > m_vecUseTCPServerInfoList; // 사용불가 TCP 서버 리스트.

public:
	int                 m_nProtocolVer;
	int                 m_nProtocolVer2;
	KStringTable        m_stringTable;

	// 060206. kkurrung. Server Info List Refresh Gap
	static DWORD        ms_dwSendServerListGap;
};