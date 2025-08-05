#include <winsock2.h>
#include <iomanip>
#include <sstream>
#include <dbg/dbg.hpp>
#include "MatchDBThread.h"
#include "MatchServer.h"
#include "MatchSimLayer.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementDBThread( KMatchDBThread );
ImplementDBThreadName( KMatchDBThread, Stat );

#define CLASS_TYPE KMatchDBThread

KMatchDBThread::KMatchDBThread()
{
    m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() );
    m_kODBCStat.DriverConnect( ms_wstrFileDSNStat.c_str() );
}

KMatchDBThread::~KMatchDBThread(void)
{
    End();
}

void KMatchDBThread::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
        _INT_CASE( DB_EVENT_FIRST_INIT_REQ, int );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " )
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID )
            << END_LOG;
    }
}

_INT_IMPL_ON_FUNC( DB_EVENT_FIRST_INIT_REQ, int )
{
    SetServerListFromDB( kPacket_ );

    // 서버 초기정보 받아오기
    // 뭘 받아와야할까..

    SiKMatchServer()->SetInitEvent();
}
