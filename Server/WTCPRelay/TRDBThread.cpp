#include <winsock2.h>
#include <iomanip>
#include <sstream>
#include <dbg/dbg.hpp>
#include "TRDBThread.h"
#include "TRServer.h"
#include "TRSimLayer.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementDBThread( KTRDBThread );
ImplementDBThreadName( KTRDBThread, Stat );

#define CLASS_TYPE KTRDBThread

KTRDBThread::KTRDBThread()
{
    m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() );
    m_kODBCStat.DriverConnect( ms_wstrFileDSNStat.c_str() );
}

KTRDBThread::~KTRDBThread(void)
{
    End();
}

void KTRDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
         INT_CASE( DB_EVENT_FIRST_SERVER_INFO_REQ );
         INT_CASE( DB_ETR_TYPE_COUNT_STAT );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " )
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID )
            << END_LOG;
    }
}

INT_IMPL_ON_FUNC( DB_EVENT_FIRST_SERVER_INFO_REQ )
{
    // 서버 초기정보 받아오기
    // 어떤 정보를 쓰고 어떤 정보를 받아와야 할까?
    int nUID;
    if( kPacket_.m_bInit ) {
        nUID = GetServerUID( kPacket_.m_strIP, kPacket_.m_usPort, kPacket_.m_nType );
        SiKTRServer()->SetServerID( nUID );
        LIF( UpdateCCU( nUID, 1, 0 ) );
        SiKTRServer()->SetInitEvent(); // 서버 켜짐.
    } else {
        nUID = SiKTRServer()->GetServerID();
        LIF( UpdateCCU( nUID, 0, 0 ) ); // 서버 꺼짐.
    }
}

INT_IMPL_ON_FUNC( DB_ETR_TYPE_COUNT_STAT )
{
    std::map<int,int>::iterator mit;
    for( mit = kPacket_.m_mapTypeStat.begin(); mit != kPacket_.m_mapTypeStat.end(); ++mit ) {
        LIF( UpdateStatType( mit->first, kPacket_.m_nSID, mit->second ) );
        if( mit->first == KDB_ETR_TYPE_COUNT_STAT::STAT_CURRENT_USER ) {
            LIF( UpdateCCU( kPacket_.m_nSID, 1, mit->second ) );
    }
    }

    START_LOG( clog, L"유저 통계기록" )
        << BUILD_LOG( kPacket_.m_nSID )
        << BUILD_LOG( kPacket_.m_mapTypeStat.size() )
        << END_LOG;
}
