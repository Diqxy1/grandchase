#include <winsock2.h>
//#include <iomanip>
//#include <sstream>

#include <dbg/dbg.hpp>

#include "EvtDBThread.h"
#include "GSSimLayer.h"
#include "GameServer.h"

#define CLASS_TYPE KEvtDBThread

ImplementDBThread( KEvtDBThread );

KEvtDBThread::KEvtDBThread()
{
    m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() );
}

KEvtDBThread::~KEvtDBThread(void)
{
    End();
}

void KEvtDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    //switch( kIntEvent_.m_usEventID )
    //{    
    //    //INT_CASE( DB_EVENT_NM6TH_ANNIVERSARY );

    //default:
    //    START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
    //        << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) );
    //}

}

//INT_IMPL_ON_FUNC( DB_EVENT_NM6TH_ANNIVERSARY )
//{
//    START_LOG( clog, L"넷마블 6주년 기념 이벤트 응모권" )
//        << BUILD_LOG( strLogin_ )
//        << BUILD_LOG( kPacket_.m_strNMPasswd )
//        << BUILD_LOG( kPacket_.m_nItemNum ) << END_LOG;
//
//    for( int i = 0; i < kPacket_.m_nItemNum; i++ )  // 아이템 개수만큼 table record를 추가한다.
//    {
//        if( !m_kODBC.Query( L"INSERT INTO dbo.tblChaseEvent(cpKind,UniID,UserID) VALUES(\'Q\',\'%s\',\'%s\')",
//            kPacket_.m_strNMPasswd.c_str(),
//            strLogin_.c_str() ) )
//        {
//            START_LOG( cerr, L"넷마블 6주년 기념 이벤트 아이템 처리 실패" )
//                << BUILD_LOG( strLogin_ )
//                << BUILD_LOG( i )
//                << BUILD_LOG( kPacket_.m_strNMPasswd )
//                << BUILD_LOG( kPacket_.m_nItemNum ) << END_LOG;
//            return;
//        }
//    }
//}