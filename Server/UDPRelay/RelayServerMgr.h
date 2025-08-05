// 2007.1.19
// Written by Woo Sang Hyuk
// UDPRelayServerManager
// 
#pragma once
#include <KNCSingleton.h>
#include <KncException.h>
#include <KncLua.h>
#include <ToString.h>
#include "KncSmartPtr.h"
#include "RelayServer.h"
#include "StringTable.h"


class KRelayServerMgr// : KRelayServer  // 상속해도 되는걸까 -_-?
{
    DeclToStringW;
    DeclareSingleton( KRelayServerMgr );
public:
    KRelayServerMgr();
    virtual ~KRelayServerMgr(void);
    bool Init( const wchar_t* szFileName );
    void Run( bool bRunInterpreter );
    void ShutDown();

    void SetSendTick( DWORD dwSendTick );
    void Add(USHORT port);
    KRelayServer* Get(USHORT port);
    void Del(USHORT port);
    void Help();
    void ToggleLog();
    void SetLogTick( DWORD dwLogTick );
    void Dump( USHORT port );

    lua_State* GetLuaState() { return m_sptlua.get(); }
    void SetCrashMail();
    void LoadINIFile( const wchar_t* szFileName );

    static DWORD           g_dwHBTick;
    DWORD                  m_dwLogTick; // 로그 기록 시간 간격 
    KStringTable           m_stringTable;

protected:
    boost::shared_ptr<lua_State>	m_sptlua;
    SOCKET                          m_sock;
    DWORD                           m_dwSendTick;
    std::wstring                    m_strTagVersion;
    std::vector< boost::shared_ptr<KRelayServer> > m_vecRelay; // sp     생성한 객체를 저장할 컨테이너 
        
    mutable KncCriticalSection      m_csVecRelay;
    std::wstring        m_strIniFilePath;

};
DefSingletonInline( KRelayServerMgr );