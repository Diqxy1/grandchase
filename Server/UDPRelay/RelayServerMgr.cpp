
#include "RelayServerMgr.h"
#include <dbg/dbg.hpp>
#include "Socket/Netcommon.h"
#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#ifdef NDEBUG
#   include "CrashRpt/KncCrashRpt.h"
#   pragma comment(lib, "../Common/Crashrpt/CrashRpt.lib")
BOOL CALLBACK CrashCallback( LPVOID lpvState )
{
    AppendToMailContent( lpvState, NetCommon::GetLocalIP() );
    return true;
}
#endif

#ifdef _DEBUG
#pragma comment( lib, "log4cxxD" ) // log4cxx
#else
#pragma comment( lib, "log4cxx" ) // log4cxx
#endif

namespace ConfigINI 
{
    /*//// ...in ini file... ////////////////////////////////////////////////////

    [Version]
    Protocol Version 1 = XXX
    Protocol Version 2 = YYY
    
    [String]
    aaa = AAA
    bbb = BBB
    ...

    /////////////////////////////////////////////////////////////////////////*/

    const wchar_t* szSectionVersion = L"Version";
    const wchar_t* szSectionString  = L"String";
    const wchar_t* szSectionMail    = L"Mail";
    const wchar_t* szKeyVersion1    = L"Protocol Version 1";
    const wchar_t* szKeyVersion2    = L"Protocol Version 2";
}


ImplementSingleton( KRelayServerMgr );
ImplOstmOperatorW2A( KRelayServerMgr );
ImplToStringW( KRelayServerMgr )
{
    size_t VectorSize = 0;
    {
        KLocker lock( m_csVecRelay );
        VectorSize = m_vecRelay.size();
    }

    stm_ << L"Tag Version          : " << m_strTagVersion << std::endl
         << L"SendTick             : " << m_dwSendTick << std::endl
         << L"m_dwHBTick           : " << g_dwHBTick << std::endl
         << L"Number of Open Ports : " << VectorSize << std::endl;

    std::for_each( m_vecRelay.begin(), m_vecRelay.end(),
        boost::bind( &KRelayServer::GetSimpleInfo, _1, boost::ref( stm_ ) ) );

    return stm_;
}
KRelayServerMgr::KRelayServerMgr()
:
m_strTagVersion( L"Tag version undefined." ),
m_sock( INVALID_SOCKET ),
m_dwSendTick( 1 ),
m_dwLogTick( 60*60*1000 )
{
}
KRelayServerMgr:: ~KRelayServerMgr(void)
{
}
void KRelayServerMgr::Add(USHORT port_)
{
    std::vector< boost::shared_ptr<KRelayServer> >::iterator vit;
    for(vit = m_vecRelay.begin(); vit != m_vecRelay.end(); ++vit)
    {
        if( (*vit)->m_usPort == port_ )
        {
            std::cout << "Can't add " << port_ << "port!! Choose others." << std::endl;
            return;
        } // 벡터에는 존재하지만 ShutDown() 호출했던 포트라면 다시 열 수 있도록 수정해야 한다.
    } 
    std::cout << "Open port: " << port_ << std::endl;
    boost::shared_ptr<KRelayServer> m_spObject( new KRelayServer );
    
    m_spObject->m_usPort = port_;
    m_spObject->Init();
    m_spObject->Run();
    m_spObject->SetSendTick(m_dwSendTick);
    m_spObject->SetLogTick(m_dwLogTick);
    m_vecRelay.push_back( m_spObject );

    size_t vecSize;
    {
        KLocker lock( m_csVecRelay );
        vecSize = m_vecRelay.size();
    }
    std::cout << "vector size: " << vecSize << std::endl; // vector에 객체가 정상적으로 추가된다면 크기가 증가할 것..
}
KRelayServer* KRelayServerMgr::Get(USHORT port_) // 열린 port 의 shared_ptr을 찾는 부분
{
    std::vector< boost::shared_ptr<KRelayServer> >::iterator vit;
    for(vit = m_vecRelay.begin(); vit != m_vecRelay.end(); ++vit)
    {
        if( (*vit)->m_usPort == port_ )
        {
            std::cout << "Get Port : " << (*vit)->m_usPort << std::endl;

            KRelayServer* pkRelayServer = (*vit).get();
            return pkRelayServer;
        }     
    }  
    std::cout << "Can't find [port] information" << std::endl;
    return NULL;
}
void KRelayServerMgr::Del(USHORT port_) // 벡터에서 삭제가 아닌 shutdown 만 지원
{
    std::vector< boost::shared_ptr<KRelayServer> >::iterator vit;
    for(vit = m_vecRelay.begin(); vit != m_vecRelay.end(); ++vit)
    {
        if( (*vit)->m_usPort == port_ )
        {
            std::cout << "Delete Port : " << (*vit)->m_usPort << std::endl;
            (*vit)->ShutDown();            
        }
    }   
}
void KRelayServerMgr::Run( bool bRunInterpreter_ )
{
    START_LOG( cerr, L"Relay Server Manager Start." ) << END_LOG;
    std::cout << *this << std::endl;

    if( bRunInterpreter_ )  lua_interpreter( m_sptlua.get() );

    //lua_interpreter( m_sptlua.get() );// 070824. woosh. BaseServer에서 처럼 처리되어 있지 않음	
}

void KRelayServerMgr::SetCrashMail()
{
    // 050716. crash report 설정 부분을 main.cpp에서 simlayer로 옮김 - 태그 제목을 스크립트로 수정하기 위함.
#ifdef NDEBUG 
    std::string strTagVersion = KncUtil::toNarrowString( m_strTagVersion );
    std::string strMailSubject = "[CRASH]" + strTagVersion;

    std::string strMail;
    std::vector<std::wstring> vecKeys;
    size_t keySize = m_stringTable.GetAllKeys( ConfigINI::szSectionMail, vecKeys );
    int nLeftSize = vecKeys.size();
    std::vector<std::wstring>::iterator vit;
    for ( vit = vecKeys.begin() ; vit != vecKeys.end() ; ++vit ) {
        std::string strTmp = KncUtil::toNarrowString( *vit );
        strMail.insert( strMail.size(), strTmp );

        --nLeftSize;
        if ( nLeftSize > 0 ) {
            strMail.insert( strMail.size(), ", " );
        }
    }

    LPVOID lpvState = Install( CrashCallback, strMail.c_str(), strMailSubject.c_str() );
#endif
}


bool KRelayServerMgr::Init(const wchar_t* szFileName_ )
{
    //////////////////////////////////////////////////////////////////////////
    // 실행파일의 timestamp로 로그 타이틀 설정, crash report 태그 설정.
    KncUtil::GetTimeStampByNameW( m_strTagVersion );

    // 050716. crash report 설정 부분을 main.cpp에서 simlayer로 옮김 - 태그 제목을 스크립트로 수정하기 위함.
    LoadINIFile( szFileName_ );
    SetCrashMail();
    // init log file
    //dbg::logfile::SetFileNameAtTime( true );
    // 제목은 처음 초기화때 출력하므로, 아직 아무 로그도 출력하지 않은 상태여야 한다. 
    //dbg::logfile::SetLogFileTitle( m_strTagVersion.c_str() ); 
    //////////////////////////////////////////////////////////////////////////
    // lua setting.
    m_sptlua = boost::shared_ptr<lua_State>( lua_open(), lua_close );
/////////
    _JIF( m_sptlua != NULL, return false ); // 070824. woosh. add
    luaopen_base( m_sptlua.get() );  // 루아함수 tonumber를 사용해야 한다.
/////////
    luabind::open( m_sptlua.get() );
    luabind::module( m_sptlua.get() )
        [
            //luabind::def( "log", dbg::logfile::SetLogLevel ),
            //luabind::def( "changelog", dbg::logfile::CloseFile ),

            luabind::def( "GetRelayMgr", KRelayServerMgr::GetInstance ),
            luabind::class_<KRelayServerMgr>( "KRelayServerMgr" )
                .def( luabind::tostring(luabind::self) )
                .def( "SetSendTick", &KRelayServerMgr::SetSendTick )
                .def( "Add", &KRelayServerMgr::Add )
                .def( "Get", &KRelayServerMgr::Get )
                .def( "Del", &KRelayServerMgr::Del )
                .def( "Help", &KRelayServerMgr::Help )
                .def( "ToggleLog", &KRelayServerMgr::ToggleLog )
                .def( "SetLogTick", &KRelayServerMgr::SetLogTick )
                .def( "Dump", &KRelayServerMgr::Dump ),

            luabind::class_<KRelayServer>( "KRelayServer" )
                .def( luabind::tostring(luabind::self) )
                .def( "ToggleLog", &KRelayServer::ToggleLog )
        ];

    std::string strLuaFileName = KncUtil::toNarrowString( szFileName_ );
    strLuaFileName += ".lua";

    _JIF( 0 == lua_dofile( m_sptlua.get(), strLuaFileName.c_str() ), lua_pop( m_sptlua.get(), -1 ); return false );
    return true;
}

void KRelayServerMgr::ShutDown()
{
    START_LOG( cerr, L"Relay Server Manager End." ) << END_LOG;
    std::vector< boost::shared_ptr<KRelayServer> >::iterator vit;
  
    std::for_each( m_vecRelay.begin(), m_vecRelay.end(), boost::bind( &KRelayServer::ShutDown, _1 ) );
    m_vecRelay.clear();
    std::cout << "vector size at Shutdown: "<<m_vecRelay.size() << std::endl;
    std::cout << *this << std::endl;
}
void KRelayServerMgr::SetSendTick( DWORD dwSendTick_ )
{
    START_LOG( cerr, L"Change m_dwSendTick from " << m_dwSendTick << " to " << dwSendTick_ ) << END_LOG;
 
    m_dwSendTick = dwSendTick_;
 
}
void KRelayServerMgr::Help()
{
    std::cout << " RelayServer Manager " << std::endl	
    << " usage : Server:Add(port) <- Open port" << std::endl
    << "         Server:Get(port) <- Get Port Information " << std::endl
    << "         Server:Del(port) <- Shutdown "  << std::endl;

}

void KRelayServerMgr::ToggleLog()
{
    START_LOG( cerr, L" ---- Toggle Log ----" ) << END_LOG;
    std::vector< boost::shared_ptr<KRelayServer> >::iterator vit;
    KLocker lock( m_csVecRelay );
    for( vit = m_vecRelay.begin() ; vit != m_vecRelay.end() ; ++vit )
    {
        (*vit)->ToggleLog();
    }
}

void KRelayServerMgr::SetLogTick( DWORD dwLogTick_ )
{
    START_LOG( cerr, L"Change m_dwLogTick from " << m_dwLogTick << " to " << dwLogTick_ ) << END_LOG;

    m_dwLogTick = dwLogTick_;
}
void KRelayServerMgr::Dump( USHORT port_ )
{
    std::vector< boost::shared_ptr<KRelayServer> >::iterator vit;
    for(vit = m_vecRelay.begin(); vit != m_vecRelay.end(); ++vit)
    {
        if( (*vit)->m_usPort == port_ )
        {
            (*vit)->DumpAddressList();
            return;
        }     
    }  
    std::cout << "Can't find [port] information" << std::endl;
}

void KRelayServerMgr::LoadINIFile( const wchar_t* szFileName_ )
{
    // 050318. florist. ini 파일은 전체 경로(혹은 상대경로)를 적어서 사용해야 한다. 
    // 그렇지 않으면 무조건 윈도우 폴더 안에서만 찾는다. 이왕 해야 할거면 확실하게 절대경로를 쓰자.
    wchar_t buff[MAX_PATH] = {0};
    ::GetCurrentDirectoryW( MAX_PATH, buff );
    m_strIniFilePath = buff;
    m_strIniFilePath += L"\\";
    m_strIniFilePath += szFileName_;
    m_strIniFilePath += L".ini";

    // 050317. version 정보 얻어오기.
//    m_nProtocolVer  = ::GetPrivateProfileIntW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, m_strIniFilePath.c_str() );
//    m_nProtocolVer2 = ::GetPrivateProfileIntW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2, 0, m_strIniFilePath.c_str() );

    //if( m_nProtocolVer == 0 || m_nProtocolVer2 == 0 )
    //{
    //    START_LOG( cerr, L"버전 정보를 얻어오지 못했습니다." )
    //        << BUILD_LOG( m_strIniFilePath )
    //        << BUILD_LOG( m_nProtocolVer )
    //        << BUILD_LOG( m_nProtocolVer2 )
    //        << END_LOG;
    //}

    //{{ 060201. kkurrung. 문자열 테이블
    if( !m_stringTable.LoadIni( m_strIniFilePath.c_str() ) )
    {
        START_LOG( cerr, L"INI 파일을 읽어 오지 못했습니다." )
            << BUILD_LOG( m_strIniFilePath )
            << END_LOG;
    }

    m_stringTable.SetCurrentSection( ConfigINI::szSectionString );

    //}} kkurrung.
}
