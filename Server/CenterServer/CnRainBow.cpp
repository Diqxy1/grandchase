#include "CenterSimLayer.h" // winsock2
#include "CnRainBow.h"
#include <dbg/dbg.hpp >
#include "CommonPacket.h"
#include "CenterDBThread.h"
#include "CenterPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnRainBow );
ImplOstmOperatorW2A( KCnRainBow );
NiImplementRootRTTI( KCnRainBow );

KCnRainBow::KCnRainBow(void)
:m_nRainbowID(-1)
,m_tmEventBegin(1)
,m_tmEventEnd(1)
,m_bChanged(false)
,m_bEventTerm(false)
,m_nJackPotID(-1)
,m_dwTickGap(1000*60)
{
    m_vecDailyEventList.clear();
    m_vecJackPotEventList.clear();
    m_vecJacPotTimeList.clear();
    m_dwLastTick = ::GetTickCount();
}

KCnRainBow::~KCnRainBow(void)
{
}

ImplToStringW( KCnRainBow )
{
    KLocker lock(m_csRainbow);
    START_TOSTRINGW
        << TOSTRINGW( m_nRainbowID )
        << TOSTRINGWt( CTime(m_tmEventBegin) )
        << TOSTRINGWt( CTime(m_tmEventEnd) )
        << TOSTRINGWb( m_bChanged )
        << TOSTRINGWb( m_bEventTerm )
        << TOSTRINGWb( m_nJackPotID )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_vecDailyEventList.size() )
        << TOSTRINGW( m_vecJackPotEventList.size() )
        << TOSTRINGW( m_vecJacPotTimeList.size() );
    DumpCurrentEvent( stm_ );
    return stm_;

}

void KCnRainBow::DBRainbowEventUpdate( IN const int nRainbowID_, IN const time_t tmBegin_, IN const time_t tmEnd_ )
{
    KLocker lock( m_csRainbow );
    m_nRainbowID        = nRainbowID_;
    m_tmEventBegin      = tmBegin_;
    m_tmEventEnd        = tmEnd_;
}

void KCnRainBow::DBDailyEventUpdate( IN std::vector<KDailyEventData>& vecDailyEventList_ )
{
    KLocker lock( m_csRainbow );
    m_vecDailyEventList.swap( vecDailyEventList_ );
}

void KCnRainBow::DBJacPotEventListUpdate( IN std::vector<KRainBowData>& vecJackPotEventList_ )
{
    KLocker lock( m_csRainbow );
    m_vecJackPotEventList.swap( vecJackPotEventList_ );
}

void KCnRainBow::DBJacPotTimeListUpdate( IN std::vector<KJackpotTimeList>& vecJackPotTimeList_ )
{
    KLocker lock( m_csRainbow );
    m_vecJacPotTimeList.swap( vecJackPotTimeList_ );
}

void KCnRainBow::UpdateState()
{
    KLocker lock( m_csRainbow );

    if( m_bChanged ) return; // DB나 외부에서 이미 변경이 일어 났다.

    bool bEventTerm     = CheckEventTerm();
    int nJackPotID      = CheckJacPotTerm();
    KDailyEventData kCurrentDailyEvent;
    GetDailyEvent( kCurrentDailyEvent );

    // 이벤트 기간이 아니고 이전에도 이벤트 기간이 아니었다면. 더이상 체크 할 필요없다.
    if( !bEventTerm && !m_bEventTerm ) return;
    // 이벤트 기간이다.

    // 이벤트 진행 기간 변경 여부
    if( bEventTerm != m_bEventTerm ) m_bChanged = true;

    // 잭팟 기간이 변경이 있었나?
    if( m_nJackPotID != nJackPotID ) m_bChanged = true;

    // 일간 이벤트가 변경 되었나?
    if( kCurrentDailyEvent.m_nDailyID != m_kCurrentDailyEvent.m_nDailyID ) m_bChanged = true;

    if( !m_bChanged ) return; // 변경 사항이 없다면.. 빠이 빠이..

    m_bEventTerm            = bEventTerm;
    m_nJackPotID            = nJackPotID;
    m_kCurrentDailyEvent    = kCurrentDailyEvent;
}

bool KCnRainBow::CheckEventTerm()
{
    CTime tmCurrent = CTime::GetCurrentTime();
    time_t tmTodaty = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 ) );
    return ( m_tmEventBegin <= tmTodaty && m_tmEventEnd >= tmTodaty );
}

int  KCnRainBow::CheckJacPotTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    std::vector<KJackpotTimeList>::const_iterator vit;
    for( vit = m_vecJacPotTimeList.begin() ; vit != m_vecJacPotTimeList.end() ; ++vit )
    {
        if( vit->m_tmBegin <= tmCurrent && vit->m_tmEnd >= tmCurrent )
            return vit->m_nJackpotID;
    }
    return -1;
}

bool KCnRainBow::GetDailyEvent( KDailyEventData& kCurrentDailyEvent_ )
{
    CTime tmCurrent = CTime::GetCurrentTime();
    time_t tmToDay = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 ) );

    std::vector<KDailyEventData>::const_iterator vit;
    for( vit = m_vecDailyEventList.begin() ; vit != m_vecDailyEventList.end() ; ++vit )
    {
        if( vit->m_tmEventDate == tmToDay )
        {
            kCurrentDailyEvent_ = *vit;
            return true;
        }
    }
    return false;
}

bool KCnRainBow::IsChanged()
{
    KLocker lock( m_csRainbow );
    return m_bChanged;
}

void KCnRainBow::SetChangeTag( IN const bool bChanged_ )
{
    KLocker lock( m_csRainbow );
    m_bChanged = bChanged_;
}

void KCnRainBow::Tick()
{
    // 시간 체크
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return;
    m_dwLastTick = ::GetTickCount();

    // 업데이트
    UpdateState();

    if( IsChanged() )
    {
        // Center 큐잉
        SendToCenter();
        // Change 변경
        SetChangeTag( false );
    }
    // DB 문의
    QueuingToDB( false );
}

void KCnRainBow::QueuingToDB( IN const bool bFirst_ )
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_RAINBOW_UPDATE_REQ, L"", (bFirst_?1:0) );
}

void KCnRainBow::SendToCenter()
{
    KECN_RAINBOW_UPDATE_NOT kPacket;
    GetRainbowPacket( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_RAINBOW_UPDATE_NOT, kPacket );
}

void KCnRainBow::GetRainbowPacket( OUT KECN_RAINBOW_UPDATE_NOT& kPacket_ )
{

    // 초기화
    kPacket_.m_nRainBowID       = -1;
    kPacket_.m_nDailyEventID    = -1;
    kPacket_.m_nJackPotEventID  = -1;
    kPacket_.m_vecEventList.clear();

    kPacket_.m_nRainBowID       = GetRainbowID();
    kPacket_.m_nDailyEventID    = GetDailyEventID();
    kPacket_.m_nJackPotEventID  = GetJackPotID();
    GetDailyEventForServer( kPacket_.m_vecEventList );

}

int KCnRainBow::GetRainbowID()
{
    KLocker lock( m_csRainbow );
    return m_nRainbowID;
}

int KCnRainBow::GetDailyEventID()
{
    KLocker lock( m_csRainbow );
    return m_kCurrentDailyEvent.m_nDailyID;
}

int KCnRainBow::GetJackPotID()
{
    KLocker lock( m_csRainbow );
    return m_nJackPotID;
}

void KCnRainBow::GetDailyEventForServer( OUT std::vector<KRainBowData>& vecEventList_ )
{
    vecEventList_.clear();
    int nJackPotID = GetJackPotID();
    if( nJackPotID == -1 )
    {
        KRainBowData kData;
        {
            KLocker lock( m_csRainbow );
            kData.m_nEventID        = m_kCurrentDailyEvent.m_nEventID;
            kData.m_fPrimaryRatio   = m_kCurrentDailyEvent.m_fPrimaryRatio;
            kData.m_fSecondaryRatio = m_kCurrentDailyEvent.m_fSecondaryRatio;
        }
        vecEventList_.push_back( kData );
    }
    else
    {
        vecEventList_ = m_vecJackPotEventList;
    }
}

void KCnRainBow::DumpCurrentEvent( std::wostream& stm_ ) const 
{
    stm_ << L" --- Current Event ---- " << std::endl
        << L" JackPot : " << (m_nJackPotID > 0 ? L"TRUE":L"FALSE") << std::endl
        << L"[" << m_kCurrentDailyEvent.m_nDailyID
        << L", " << m_kCurrentDailyEvent.m_nEventID
        << L", " << (LPCTSTR)CTime( m_kCurrentDailyEvent.m_tmEventDate ).Format( KNC_TIME_FORMAT )
        << L", " << m_kCurrentDailyEvent.m_fPrimaryRatio
        << L", " << m_kCurrentDailyEvent.m_fSecondaryRatio
        << L"]" << std::endl;
}

void KCnRainBow::ReloadAll()
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_RAINBOW_UPDATE_REQ, L"", 1 );
}
