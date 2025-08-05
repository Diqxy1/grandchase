#include "CenterSimLayer.h" // winsock2
#include "CnMissionEvent.h"
#include <dbg/dbg.hpp >
#include "CommonPacket.h"
#include "CenterDBThread.h"
#include "CenterPacket.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnMissionEvent );
ImplOstmOperatorW2A( KCnMissionEvent );
NiImplementRootRTTI( KCnMissionEvent );

KCnMissionEvent::KCnMissionEvent(void)
:m_bEventTerm(false)
,m_bChanged(false)
,m_bTodayChanged(false)
,m_tmEventBegin(1)
,m_tmEventEnd(1)
,m_dwTickGap(1000*60)
,m_dwNotifyTime(5)
,m_bFirstTimeCheck(false)
,m_bSecondTimeCheck(false)
,m_bThirdTimeCheck(false)
,m_bFirst(true)
,m_nMissionEventID(0)
,m_nMissionCheckHour(0)
{
    m_vecMissionDaily.clear();
    m_dwLastTick = ::GetTickCount();
    // 서버 시작시간을 기억하고 있자..,!
    // 하루가 지났는지에 대한 여부를 확인하기 위해서..,!
    CTime tmCurrent = CTime::GetCurrentTime();
    m_tmStartBegin = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
                                                    tmCurrent.GetMonth(), 
                                                        tmCurrent.GetDay(), 
                                                                    0, 0, 0 ) );
}

KCnMissionEvent::~KCnMissionEvent(void)
{
}


ImplToStringW( KCnMissionEvent )
{
    KLocker lock(m_csMissionEvent);
    START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmEventBegin) )
        << TOSTRINGWt( CTime(m_tmEventEnd) )
        << TOSTRINGWb( m_bEventTerm )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_vecMissionDaily.size() )
        << TOSTRINGWt( CTime(m_tmFirstNofity) )
        << TOSTRINGWt( CTime(m_tmSecondNofity) )
        << TOSTRINGWt( CTime(m_tmStartBegin) )
        << TOSTRINGW( m_nMissionCheckHour );
    DumpCurrentEvent( stm_ );
    return stm_;
}

void KCnMissionEvent::DBMissionEventUpdate( IN const int nMissionEventID_, IN const time_t tmBegin_, IN const time_t tmEnd_, IN int nMissionCheckHour_ )
{
    KLocker lock( m_csMissionEvent );
    m_nMissionEventID      = nMissionEventID_;
    m_tmEventBegin         = tmBegin_;
    m_tmEventEnd           = tmEnd_;

    // 0보다 더 작은 값이면 0으로 맞춘다.
    // CTime의 인자로 넣어줄 때 음수값이 들어가면 서버의 덤프가 발생한다.
    nMissionCheckHour_     = std::max<int>( nMissionCheckHour_, 0 ); 
    m_nMissionCheckHour    = nMissionCheckHour_;

    if( m_bFirst )
    {
        CTime tmCurrent = CTime::GetCurrentTime();
        m_tmStartBegin = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
                                                    tmCurrent.GetMonth(), 
                                                    tmCurrent.GetDay(), 
                                                    nMissionCheckHour_, 0, 0 ) );

        int nHour = tmCurrent.GetHour();

        // 오늘의 이벤트 기간이 지나지 않았을 경우..,
        if( nHour < m_nMissionCheckHour )
        {
            CTime tmTodayEventTime( m_tmStartBegin );
            CTimeSpan ts(0,0,m_dwNotifyTime,0);
            // 하루변경을 이 변수로 알려주자.
            m_tmthirdCheck = tmTodayEventTime;
            tmTodayEventTime -= ts;
            // 오늘 공지 시간의 5분전을 저장해 두기.
            m_tmSecondNofity = tmTodayEventTime;
            tmTodayEventTime -= ts;
            // 오늘 공지 시간의 10분전을 저장해 두기.
            m_tmFirstNofity = tmTodayEventTime;
        }
        else
        {
            // 오늘 이벤트 기간이 지났을 경우..,
            CTime tmTodayEventTime1( m_tmStartBegin );
            CTimeSpan ts1(1,0,0,0);
            CTimeSpan ts2(0,0,m_dwNotifyTime,0);
            tmTodayEventTime1 += ts1;
            // 하루변경을 이 변수로 알려주자.
            m_tmthirdCheck = tmTodayEventTime1;
            tmTodayEventTime1 -= ts2;
            // 오늘 공지 시간의 5분전을 저장해 두기.
            m_tmSecondNofity = tmTodayEventTime1;
            tmTodayEventTime1 -= ts2;
            // 오늘 공지 시간의 10분전을 저장해 두기.
            m_tmFirstNofity = tmTodayEventTime1;
        }
        m_bFirst = false;

        START_LOG( clog, L"서버시작하고 전체 공지시간을 세팅." )
            << BUILD_LOG( nHour )
            << BUILD_LOGtm( CTime(m_tmFirstNofity) )
            << BUILD_LOGtm( CTime(m_tmSecondNofity) )
            << BUILD_LOGtm( CTime(m_tmStartBegin) )
            << BUILD_LOG( m_nMissionCheckHour ) << END_LOG;
    }
}

void KCnMissionEvent::DBDailyEventUpdate( IN std::vector<KMissionDailyData>& vecDailyEventData )
{
    std::sort( vecDailyEventData.begin(), vecDailyEventData.end(),
        boost::bind( &KMissionDailyData::m_tmEventDate, _1 ) < boost::bind( &KMissionDailyData::m_tmEventDate, _2 ));

    KLocker lock( m_csMissionEvent );
    m_vecMissionDaily.swap( vecDailyEventData );

    START_LOG( clog, L" DBDailyEventUpdate1" )
        << BUILD_LOG( m_vecMissionDaily.size() ) << END_LOG;
}

void KCnMissionEvent::UpdateState()
{
    KLocker lock( m_csMissionEvent );

    DWORD dwTime = 0;
    bool bEventTerm = CheckEventTerm();
    std::vector<KMissionDailyData> vecTodayMissions;
    CTime tmCurrent = CTime::GetCurrentTime();

    // 이벤트 기간 여부에 의한 체크 
    if( !bEventTerm && !m_bEventTerm ) return;

    // 이벤트 진행 기간 변경 여부.
    if( bEventTerm != m_bEventTerm ) SetChangeTag(true);

    GetTodayMissions( m_vecMissionDaily, vecTodayMissions );
    if( m_vecCurrentMissions != vecTodayMissions ) SetChangeTag(true);


    // 해당 하루 지남을 확인.
    if( m_bThirdTimeCheck == false && tmCurrent > m_tmthirdCheck )
    {
        SetChangeDate( true );
        m_bThirdTimeCheck = true;
        START_LOG( clog, L"하루가 지났다고 처리해줄때의 변수값들은..,")
            << BUILD_LOGtm( CTime( m_tmStartBegin ) )
            << BUILD_LOG( m_nMissionCheckHour ) << END_LOG;
    }

    // 공지시간 10분전을 확인.
    if( m_bFirstTimeCheck == false && tmCurrent > m_tmFirstNofity )
    {
        dwTime = m_dwNotifyTime * 2;
        SendToNotify( dwTime );
        m_bFirstTimeCheck = true;

        START_LOG( clog, L" 10분전 공지 GameServer로 전송." )
            << BUILD_LOG( dwTime )
            << BUILD_LOGtm( CTime(m_tmFirstNofity) )
            << BUILD_LOGtm( CTime(tmCurrent) ) << END_LOG;
    }

    // 공지시간 5분전을 확인.
    if( m_bSecondTimeCheck == false && tmCurrent > m_tmSecondNofity )
    {
        dwTime = m_dwNotifyTime;
        SendToNotify( dwTime );
        m_bSecondTimeCheck = true;

        START_LOG( clog, L" 5분전 공지 GameServer로 전송." )
            << BUILD_LOG( dwTime )
            << BUILD_LOGtm( CTime(m_tmSecondNofity) )
            << BUILD_LOGtm( CTime(tmCurrent) ) << END_LOG;
    }

    if( !m_bChanged ) return;

    m_bEventTerm = bEventTerm;
    m_vecCurrentMissions = vecTodayMissions;
}

bool KCnMissionEvent::CheckEventTerm()
{
    // 현재 시간 구해와서..,
    CTime tmCurrent = CTime::GetCurrentTime();
    // 현재 시간 세팅을 해줄 때 DB에 저장되어 있는 Hour을 더해서 세팅한다.
    time_t tmToday  = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), m_nMissionCheckHour, 0, 0 ) );
    // 현재 시간이 전체 이벤트 기간에 포함되어 있는가?
    return ( m_tmEventBegin <= tmToday && m_tmEventEnd >= tmToday );
}

// 오늘 날짜의 해당 이벤트 구조체로 가져오기..,
bool KCnMissionEvent::GetCurrentDailyEvent( OUT KMissionDailyData& kCurrentDailyEvent_ )
{
    CTime tmCurrent = CTime::GetCurrentTime();
    int tmToDayHour = tmCurrent.GetHour();
    time_t tmToDay = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
        tmCurrent.GetMonth(), 
        tmCurrent.GetDay(), 
        m_nMissionCheckHour, 0, 0 ) );

    // 오늘의 시간값이 DB에 저장값보다 작을 때 이전날의 데이터를 세팅해주는 작업을
    // 진행해야 한다.
    if( tmToDayHour < m_nMissionCheckHour )
    {
        CTime tmTodayTemp( tmToDay );
        CTimeSpan ts(1,0,0,0);
        tmTodayTemp -= ts;
        tmToDay = KncUtil::TimeToInt( CTime( tmTodayTemp.GetYear(),
            tmTodayTemp.GetMonth(), 
            tmTodayTemp.GetDay(), 
            tmTodayTemp.GetHour(), 0, 0 ) );
    }

    std::vector<KMissionDailyData>::const_iterator vit;
    for( vit = m_vecMissionDaily.begin(); vit != m_vecMissionDaily.end(); ++vit )
    {
        if( vit->m_tmEventDate == tmToDay )
        {
            kCurrentDailyEvent_ = *vit;
            return true;
        }
    }

    return false;
}

bool KCnMissionEvent::IsChanged()
{
    KLocker lock( m_csMissionEvent );
    return m_bChanged;
}

void KCnMissionEvent::SetChangeTag( IN const bool bChanged_ )
{
    KLocker lock( m_csMissionEvent );
    m_bChanged = bChanged_;
}

bool KCnMissionEvent::IsDateUpdate()
{
    KLocker lock( m_csMissionEvent );
    return m_bTodayChanged;
}

void KCnMissionEvent::SetChangeDate( IN const bool bDateUpdated_ )
{
    KLocker lock( m_csMissionEvent );
    m_bTodayChanged = bDateUpdated_;
}

void KCnMissionEvent::GetTotalDailyEventItem( OUT std::set<int>& setItemList_ )
{
    // 전체 EventList를 GameServer에게 전부주는건 너무 무의미 하지 않나?
    // set으로 전달하는게 좋은듯..,
    KLocker lock( m_csMissionEvent );
    std::vector<KMissionDailyData>::iterator vit;
    for( vit = m_vecMissionDaily.begin(); vit != m_vecMissionDaily.end(); ++vit )
    {
        setItemList_.insert( vit->m_nItemID );
    }
}

void KCnMissionEvent::GetTodayMissions( IN std::vector<KMissionDailyData>& vecTotalMissions_, OUT std::vector<KMissionDailyData>& vecMissions_ )
{
    vecMissions_.clear();
    CTime tmCurrent = CTime::GetCurrentTime();

    KLocker lock( m_csMissionEvent );
    if( tmCurrent.GetHour() < m_nMissionCheckHour ) {
        tmCurrent = tmCurrent - CTimeSpan(1, 0, 0, 0 );
    }

    time_t tmToday = KncUtil::TimeToInt( CTime(tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), m_nMissionCheckHour, 0, 0 ) );

    
    std::copy_if( vecTotalMissions_.begin(), vecTotalMissions_.end(),
        std::insert_iterator<std::vector<KMissionDailyData> >( vecMissions_, vecMissions_.end() ),
        boost::bind(&KMissionDailyData::m_tmEventDate, _1) == tmToday );
}

void KCnMissionEvent::GetCurrentMissions( OUT std::vector<KMissionDailyData>& vecMissions_ )
{
    KLocker lock( m_csMissionEvent );
    vecMissions_ = m_vecCurrentMissions;
}

void KCnMissionEvent::Tick()
{
    // 시간체크
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return;
    m_dwLastTick = ::GetTickCount();

    // 업데이트
    UpdateState();

    // Mission Event 데이터의 수정이 일어났거나..,
    // 아니면 하루가 지나서 날짜가 변경되었거나..,
    if( IsChanged() || IsDateUpdate() )
    {
        SendToCenter();
        SetChangeTag( false );
        if( IsDateUpdate() )
        {
            // 하루가 지났음을 확인하면 서버의 시작 시간을 다시 현재 시간으로
            // 초기화 해주는 작업을 진행한다.
            CTime tmCurrent = CTime::GetCurrentTime();
            m_tmStartBegin = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
                tmCurrent.GetMonth(), 
                tmCurrent.GetDay(), 
                m_nMissionCheckHour, 0, 0 ) );

            CTime tmTodayEventTime( m_tmStartBegin );
            CTimeSpan ts(1,0,0,0);      // 하루를 더하고
            CTimeSpan ts1(0,0,m_dwNotifyTime,0);        // 해당 시간을 빼고..,
            tmTodayEventTime += ts;

            // 하루변경을 이 변수로 알려주자.
            m_tmthirdCheck = tmTodayEventTime;

            tmTodayEventTime -= ts1;
            // 오늘 공지 시간의 5분전을 저장해 두기.
            m_tmSecondNofity = tmTodayEventTime;
            tmTodayEventTime -= ts1;
            // 오늘 공지 시간의 10분전을 저장해 두기.
            m_tmFirstNofity = tmTodayEventTime;

            // 공지 변수 초기화.
            m_bFirstTimeCheck = false;
            m_bSecondTimeCheck = false;
            m_bThirdTimeCheck = false;

            SetChangeDate( false );

            START_LOG( clog, L" 하루가 지나서 초기화 했음." )
                << BUILD_LOGtm( CTime(m_tmStartBegin) )
                << BUILD_LOGtm( CTime(m_tmFirstNofity) )
                << BUILD_LOGtm( CTime(m_tmSecondNofity ) )
                << BUILD_LOG( m_nMissionCheckHour) << END_LOG;
        }
    }

    // DB 문의
    QueuingToDB( false );
}

void KCnMissionEvent::SendToCenter()
{
    KECN_MISSION_EVENT_UPDATE_NOT kPacket;
    GetMissionEventPacket( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_MISSION_EVENT_UPDATE_NOT, kPacket );
}

void KCnMissionEvent::SendToNotify( IN const DWORD dwTime_ )
{
    KECN_MISSION_NOTIFY_NOT kPacket;
    kPacket.m_dwTime = dwTime_;
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_MISSION_NOTIFY_NOT, kPacket );
}

void KCnMissionEvent::GetMissionEventPacket( OUT KECN_MISSION_EVENT_UPDATE_NOT& kPacket_ )
{
    kPacket_.m_vecMissions.clear();
    kPacket_.m_setItemList.clear();
    kPacket_.m_bDateChanged = false;

    KLocker lock( m_csMissionEvent );
    GetCurrentMissions( kPacket_.m_vecMissions );
    if( CheckEventTerm() )
    {
        GetTotalDailyEventItem( kPacket_.m_setItemList );
    }
    else
    {
        kPacket_.m_setItemList.clear();
    }
    kPacket_.m_bDateChanged = IsDateUpdate();
}

void KCnMissionEvent::QueuingToDB( IN const bool bFirst_ )
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_MISSION_EVENT_REQ, L"", (bFirst_?1:0) );
}

void KCnMissionEvent::DumpCurrentEvent( std::wostream& stm_ ) const 
{
    stm_ << L" --- Current Event ---- " << std::endl;
    std::vector<KMissionDailyData>::const_iterator vit;
    for( vit = m_vecCurrentMissions.begin() ; vit != m_vecCurrentMissions.end() ; ++ vit )
    {
        stm_ << L"[" << (LPCTSTR)CTime( vit->m_tmEventDate ).Format( KNC_TIME_FORMAT )
        << L", " << vit->m_nEventID
        << L", " << vit->m_nOrderID
        << L", " << vit->m_nItemID
        << L"]" << std::endl;
    }
}