#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "MRDBThread.h"
#include "ThreadManager.h"
#include "MsgRouter.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
ImplementDBThread( KMRDBThread );

#define CLASS_TYPE KMRDBThread

KMRDBThread::KMRDBThread()
{
    m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() );
}

KMRDBThread::~KMRDBThread(void)
{
    End();
}

void KMRDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {    
        INT_CASE( DB_EVENT_SERVER_INFO_REQ );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) << END_LOG;
    }
}

INT_IMPL_ON_FUNC( DB_EVENT_SERVER_INFO_REQ )
{
    if( kPacket_.m_bFirstDBWork ) // db에서 uid를 얻어와서 이름을 만든다.
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.MSG_MsgRouterID_Select ( N'%s', %d, %d ) }",
            kPacket_.m_strIP.c_str(),
            kPacket_.m_usPort,
            1 ) );

        if( m_kODBC.BeginFetch() )
        {
            int iUID = 0;
            VERIFY_QUERY( m_kODBC >> iUID );

            m_kODBC.EndFetch();

            boost::wformat fmt( L"MsgRouter_%02d" );
            fmt % iUID;
            SiKMsgRouter()->SetUID( iUID );
            SiKMsgRouter()->SetServerName( fmt.str() );
        }
    }

    /*  
    L"{ call dbo.MSG_MsgRouter_UserNum_Update_20120329 ( %d, %d, N'%s', %d, %d ) }"  

    { call dbo.MSG_MsgRouter_UserNum_Update_20120329 ( @1, @2, @3, @4, @5 ) }  
    @1 ; @iUserNum_          int  
    @2 ; @iProtocolVersion_  int  
    @3 ; @strIP_             nvarchar(16)  
    @4 ; @iPort_             int  
    @5 ; @iServerID_         int  

    1 select ( @1 )  
    @1  ; OK int  
    0        ; 성공  
    -1       ; 해당 서버ID/Port가 유효하지 않음  
    -101이하 ; 트랜잭션 에러  
    */

    DWORD dwUID = SiKMsgRouter()->GetUID();
    // 서버에 현재 동접수를 기록한다.
    DBJIF( m_kODBC.Query( L"{ call dbo.MSG_MsgRouter_UserNum_Update_20120329 ( %d, %d, N'%s', %d, %d ) }",
        kPacket_.m_mapChannellingUserNum[0],
        kPacket_.m_nProtocolVer,
        kPacket_.m_strIP.c_str(),
        kPacket_.m_usPort,
        dwUID ) );

    if( m_kODBC.BeginFetch() )
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if( kPacket_.m_hSyncEvent != NULL ) // 060404. florist. 메인 스레드에 작업의 종료를 알려야 하는 경우.
    {
        SetEvent( kPacket_.m_hSyncEvent );
    }

}