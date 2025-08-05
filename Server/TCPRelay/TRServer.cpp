#include "TRServer.h"
#include <KncUtil.h>
#include <dbg/dbg_log4.hpp>
//#include "TRUser.h"

#ifdef _DEBUG
#pragma comment( lib, "log4cxxD" ) // log4cxx
#else
#pragma comment( lib, "log4cxx" ) // log4cxx
#endif


#ifdef NDEBUG
#   include "CrashRpt/KncCrashRpt.h"
#   pragma comment ( lib, "../Common/CrashRpt/CrashRpt.lib" )

BOOL CALLBACK CrashCallbackTR( LPVOID lpvState )
{
    std::ostringstream stm;

    stm << "Local IP : " << NetCommon::GetLocalIP() << std::endl
        << TOSTRINGA( std::locale( "" ).name() )
        << TOSTRINGA( "TCP Relay Server" )
        << TOSTRINGA( ::GetTickCount() );

    AppendToMailContent( lpvState, stm.str().c_str() );

    // get post-crash execute file's full path.
    char szBuff[MAX_PATH] = {0};
    ::GetCurrentDirectoryA( MAX_PATH, szBuff );
    ::strcat( szBuff, "\\DumpUploader.exe" );

    stm.str( "" );  // 초기화 - 버퍼의 내용을 인자로 받은 문자열로 대치.

    // build command line parameter
    stm << "/s " << NetCommon::GetLocalIP()             // 서버 이름 (아이피)
        //<< " /f " << dbg::logfile::GetLogFileName()     // 로그파일 이름
        << " /f " << GetFullDumpFileName( lpvState );   // 덤프파일 이름

    ShellExecuteA( NULL, "open", szBuff, stm.str().c_str(), NULL, SW_SHOWNORMAL );

    return true;
}
#endif

NiImplementRTTI( KTRServer, KNetLayer );
ImplementSingleton( KTRServer );

ImplToStringW( KTRServer )
{
    std::wstring strTagVersion = KncUtil::toWideString( m_strTagVersion );
    START_TOSTRING_PARENTW( KNetLayer )
        << TOSTRINGW( m_kTRUserManager.GetCountTot() )
        << TOSTRINGW( m_kTRUserManager.GetCountReg() )
        << TOSTRINGW( strTagVersion );

    size_t nPopQueueSize = 0;
    size_t nPushQueueSize = 0;
    {
        KLocker lock( m_csPushQueue );
        nPushQueueSize = m_pkPushQueue->size();
    }
    {
        KLocker lock( m_csPopQueue );
        nPopQueueSize = m_pkPopQueue->size();
    }

    return stm_ << TOSTRINGW( nPushQueueSize )
        << TOSTRINGW( nPopQueueSize );
}

KTRServer::KTRServer(void)
{
    m_pkPushQueue   = &m_queue1;
    m_pkPopQueue    = &m_queue2;
}

KTRServer::~KTRServer(void)
{
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"
bool KTRServer::Init( const char* szFileName_ )
{
    KncUtil::GetTimeStampByNameA( m_strTagVersion ); // TagVersion 저장
#ifdef NDEBUG
    LPVOID lpvState = Install( CrashCallbackTR, 
                                "woosh@kog.co.kr, tslayer@kog.co.kr, tgkwon@kog.co.kr, nodefeat@kog.co.kr, lucidust@kog.co.kr", m_strTagVersion.c_str() ); 
#endif

    std::locale loc("");
    std::cout.imbue(loc);
    std::wcout.imbue( loc );
    //로그 파일설정
    //dbg::logfile::SetFileNameAtTime( true );
    //dbg::logfile::SetLogFileTitle( KncUtil::toWideString(m_strTagVersion).c_str() ); 

    START_LOG( Info, L"Start Server" ) << END_LOG;
    // Related Lua
    m_sptlua = boost::shared_ptr<lua_State>( lua_open(), lua_close );
    _JIF( m_sptlua != NULL, false );
    luaopen_base( m_sptlua.get() );
    luabind::open( m_sptlua.get() );
    RegToLua( m_sptlua );
    
    char szFullPath[MAX_PATH];
    GetCurrentDirectoryA( MAX_PATH, szFullPath );

    std::string strFileName( szFullPath );
    strFileName += "\\";
    strFileName += szFileName_;
    strFileName += ".lua";

    _JIF( 0 == lua_dofile( m_sptlua.get(), strFileName.c_str() ), return false );

    _JIF( NetCommon::InitWinSock(), return false );
    m_vecAccepter.push_back( IAccepterPtr( new KAccepter<KTRUser,false>( m_usPortMaster ) ) );
    _JIF( KNetLayer::Init(), return false );

    m_spThreadick = boost::shared_ptr< KTThread<KTRServer> >( new KTThread<KTRServer>( *this, &KTRServer::Tick, 1 ) );
    //m_spThreadick.reset( new KTThread<KTRServer>( *this, &KTRServer::Tick, 1 ) );
    return true;
}

ImplOstmOperatorW2A( KTRServer );
void KTRServer::RegToLua( boost::shared_ptr<lua_State> L_ )
{
    KNetLayer::RegToLua( L_ );// NetLayer
    luabind::module( L_.get() )
    [
       
        luabind::class_<KTRServer, KNetLayer>( "KTRServer" )
        .def( luabind::tostring(luabind::self) ),
        luabind::def( "GetTRServer", &KTRServer::GetInstance )
        
        //luabind::def( "log", dbg::logfile::SetLogLevel ),
        //luabind::def( "changelog", dbg::logfile::CloseFile )
    ];
    
}

void KTRServer::Run()
{
    std::cout << "TCP_Relay Server Start." << std::endl;
    std::cout << *this << std::endl;
    m_spThreadick->Begin();
    lua_interpreter( m_sptlua.get() );
}
void KTRServer::ShutDown()
{
    m_kTRUserManager.SetCreateEnable( false );
    m_spThreadick->End();
    KNetLayer::ShutDown();
    
}

void KTRServer::Tick()
{
    m_kTRUserManager.Tick();

    KLocker lock( m_csPopQueue );

    {// Locking Scope

        KLocker lock( m_csPushQueue );
        std::swap( m_pkPushQueue, m_pkPopQueue );

    }// Locking Scope

    // HB, UID Reg 등은 모두 처리되었고, Relay 해야할 데이터들만 queue에 들어 있다.
    KRelayDataPtr spData;

    while( !m_pkPopQueue->empty() )
    {
        spData = m_pkPopQueue->front();
        m_pkPopQueue->pop();
        
        m_kTRUserManager.Relay( spData );
    }
}

void KTRServer::QueueingRelayData( KRelayDataPtr& spData )
{
    KLocker lock( m_csPushQueue );
    m_pkPushQueue->push( spData );
}