#include "UserEventChecker.h"
#include "UserEvent.h"
#include "Log4.h"

KUserEventChecker::KUserEventChecker(void)
{
    m_setProcessEvent.clear();
}

KUserEventChecker::~KUserEventChecker(void)
{
}

bool KUserEventChecker::EventIn( const USHORT& usEventID_ )
{
    if ( SiKGSSimLayer()->GetCheckInEnable() == false ) {
        return true;
    }

    if ( usEventID_ <= 0 ) {
        return true;
    }

    if ( m_setProcessEvent.insert( usEventID_ ).second ) {
        return true;
    }

    KCheckInStatPtr spStat = GET_STAT( KCheckInStat, SI_STAT_CHECK_IN );
    if ( spStat ) {
        spStat->AddStat( usEventID_ );
    }
    return false;
}

void KUserEventChecker::EventOut( const USHORT& usEventID_ )
{
    m_setProcessEvent.erase( usEventID_ );
}


KEventCheck::KEventCheck( const USHORT& usEventID_, const DWORD& dwUID_, const bool& bEnable_ )
:m_usEventID( 0 )
,m_dwUID( 0 )
{
    if ( !bEnable_ ) return;
    if ( usEventID_ <= 0 ) return;

    m_usEventID = usEventID_;
    m_dwUID = dwUID_;

    START_LOG( clog, L"CheckStart:" << m_usEventID << L"," << m_dwUID ) << END_LOG;
}

KEventCheck::~KEventCheck()
{
    if ( m_usEventID <= 0 ) return;

    unsigned short kPacket = m_usEventID;
    KGSSimLayer::GetInstance()->m_kActorManager.QueueingEventTo( m_dwUID, KUserEvent::EVENT_CHECK_OUT_RECORD_UPDATE, kPacket );
    START_LOG( clog, L"CheckEnd:" << m_usEventID << L"," << m_dwUID ) << END_LOG;
}
