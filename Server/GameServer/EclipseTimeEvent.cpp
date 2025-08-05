#include "GSSimLayer.h"
#include "EclipseTimeEvent.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "UserPacket.h"
#include "EventMonsterManager.h"
#include "SignBoard.h"
#include "LoudMessage.h"
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KEclipseTimeEvent );
ImplOstmOperatorW2A( KEclipseTimeEvent );
NiImplementRootRTTI( KEclipseTimeEvent );

KEclipseTimeEvent::KEclipseTimeEvent( void )
: m_tmCurrentPeriod( 0 )
, m_tmNextPeriod( 0 )
, m_dwTickGap( 60 * 1000 )
, m_dwLastTick( 0 )
, m_bEclipseInProgress( false )
{
    m_kEclipseTimeInfo.Clear();
}

KEclipseTimeEvent::~KEclipseTimeEvent( void )
{
}

ImplToStringW( KEclipseTimeEvent )
{
    KLocker lock( m_csETE );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_kEclipseTimeInfo.m_tmBegin ) )
        << TOSTRINGWt( CTime( m_kEclipseTimeInfo.m_tmEnd ) )
        << TOSTRINGW( m_kEclipseTimeInfo.m_dwPeriod )
        << TOSTRINGW( m_kEclipseTimeInfo.m_dwDuration )
        << TOSTRINGW( m_kEclipseTimeInfo.m_vecMonsterID.size() )
        << TOSTRINGW( m_bEclipseInProgress )
        << TOSTRINGWt( CTime( m_tmCurrentPeriod ) )
        << TOSTRINGWt( CTime( m_tmNextPeriod ) );
}

bool KEclipseTimeEvent::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEclipseTimeEvent::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitEclipseTimeEvent.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    KEclipseTimeInfo kEclipseTimeInfo;
    _JIF( LoadEclipseTimeInfo( kLuaMng, std::string("EclipseTimeInfo"), kEclipseTimeInfo ), return false );

    if ( kEclipseTimeInfo.m_tmBegin > kEclipseTimeInfo.m_tmEnd ) {
        START_LOG( cerr, L"InitEclipseTimeEvent.lua 스크립트 오류. 기간을 확인하세요." ) << END_LOG;
        return false;
    }
    START_LOG( cerr, L"InitEclipseTimeEvent.lua 스크립트 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csETE );

        m_kEclipseTimeInfo.m_tmBegin = kEclipseTimeInfo.m_tmBegin;
        m_kEclipseTimeInfo.m_tmEnd = kEclipseTimeInfo.m_tmEnd;
        m_kEclipseTimeInfo.m_dwPeriod = kEclipseTimeInfo.m_dwPeriod;
        m_kEclipseTimeInfo.m_dwDuration = kEclipseTimeInfo.m_dwDuration;
        m_kEclipseTimeInfo.m_vecMonsterID.swap( kEclipseTimeInfo.m_vecMonsterID );
    }
    UpdateStatus();
    SiKEventMonsterManager()->LoadScript();

    return true;
}

bool KEclipseTimeEvent::LoadEclipseTimeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KEclipseTimeInfo& kEclipseTimeInfo_ )
{
    kEclipseTimeInfo_.Clear();
    DWORD dwPeriod = 0;
    DWORD dwDuration = 0;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng_, std::string( "Begin" ), kEclipseTimeInfo_.m_tmBegin ), return false );
    _JIF( LoadTime( kLuaMng_, std::string( "End" ), kEclipseTimeInfo_.m_tmEnd ), return false );
    _JIF( kLuaMng_.GetValue( "Period", dwPeriod ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Duration", dwDuration ) == S_OK, return false );
    _JIF( LoadMonsterID( kLuaMng_, std::string( "MonsterID" ), kEclipseTimeInfo_.m_vecMonsterID ), return false );

    kEclipseTimeInfo_.m_dwPeriod = dwPeriod * 60;     // 분으로 읽어와 초로 저장
    kEclipseTimeInfo_.m_dwDuration = dwDuration * 60;

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KEclipseTimeEvent::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 5, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime( nYear, nMonth, nDay, nHour, nMin, 0 ) );

    return true;
}

bool KEclipseTimeEvent::LoadMonsterID( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< int >& vecMonsterID_ )
{
    vecMonsterID_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        int nMonsterID = 0;
        if ( kLuaMng_.GetValue( i++, nMonsterID ) != S_OK ) break;
        vecMonsterID_.push_back( nMonsterID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KEclipseTimeEvent::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csETE );
    return ( m_kEclipseTimeInfo.m_tmBegin <= tmCurrent && m_kEclipseTimeInfo.m_tmEnd >= tmCurrent );
}

void KEclipseTimeEvent::GetEclipseTimeInfo( OUT KEVENT_ECLIPSE_TIME_EVENT_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csETE );

    kPacket_.m_tmBegin = m_kEclipseTimeInfo.m_tmBegin;
    kPacket_.m_tmEnd = m_kEclipseTimeInfo.m_tmEnd;
    kPacket_.m_dwPeriod = m_kEclipseTimeInfo.m_dwPeriod;     // 클라이언트에 보낼 때 초 단위로 전송
    kPacket_.m_dwDuration = m_kEclipseTimeInfo.m_dwDuration;
}

void KEclipseTimeEvent::UpdateStatus()
{
    bool bBeforeStatus = false;
    // 전체 이벤트 시작 시간과 이벤트 발동 주기를 기준으로 현재 이벤트 발동 중인지 계산한다
    {
        KLocker lock( m_csETE );

        time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

        if ( m_kEclipseTimeInfo.m_tmBegin <= tmCurrent && tmCurrent < m_kEclipseTimeInfo.m_tmEnd ) {   // 이벤트 기간일 때만 계산
            time_t tmDiffer = tmCurrent - m_kEclipseTimeInfo.m_tmBegin; // 위 조건으로 항상 양수
            time_t tmPass = tmDiffer % m_kEclipseTimeInfo.m_dwPeriod;

            bBeforeStatus = m_bEclipseInProgress;
            m_bEclipseInProgress = ( tmPass < static_cast< time_t >( m_kEclipseTimeInfo.m_dwDuration ) );

            m_tmCurrentPeriod = tmCurrent - tmPass; // 현재 시간 - 
            m_tmNextPeriod = m_tmCurrentPeriod + m_kEclipseTimeInfo.m_dwPeriod;
        }
        else {  // 이벤트 기간 아닌 경우는 항상 false
            m_bEclipseInProgress = false;
            m_tmCurrentPeriod = 0;
            m_tmNextPeriod = 0;
        }
    }   // end lock
    // 이벤트가 발동/종료 되면 전광판에 알림 메시지를 보인다
    SetNoticeInfo( bBeforeStatus );
}

bool KEclipseTimeEvent::CheckTick()
{
    DWORD dwTickCount = ::GetTickCount();
    if (  dwTickCount - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = dwTickCount;
    return true;
}

void KEclipseTimeEvent::Tick()
{
    if ( CheckTick() ) {    // 1분틱
        UpdateStatus();
    }
}

bool KEclipseTimeEvent::IsEventOn()
{
    /*
    EclipseTimeEvent는 EventMonster의 설정을 사용하기 때문에 여기서 true를 반환 하더라도
    SikEventMonsterManager()->IsActiveEvent()에서 전체 off로 설정되어 있으면 이벤트 몬스터가 등장하지 않는다
    */
    KLocker lock( m_csETE );

    return m_bEclipseInProgress;
}

bool KEclipseTimeEvent::CheckEclipseEventMonster( IN const int& nMonID_ )
{
    KLocker lock( m_csETE );

    std::vector< int >::iterator vit;
    vit = find( m_kEclipseTimeInfo.m_vecMonsterID.begin(), m_kEclipseTimeInfo.m_vecMonsterID.end(), nMonID_ );
    if ( vit == m_kEclipseTimeInfo.m_vecMonsterID.end() ) {
        return false;
    }
    return true;
}

void KEclipseTimeEvent::SetNoticeInfo( IN const bool& bBeforeStatus_ )
{
    KLocker lock( m_csETE );

    int nFlag = -1;

    if ( bBeforeStatus_ == m_bEclipseInProgress ) {
        return;
    }

    if ( m_bEclipseInProgress ) {   // == true
        nFlag = KEclipseTimeEvent::AL_EVENT_START;
    }
    else { //   m_bEclipseInProgress == false
        nFlag = KEclipseTimeEvent::AL_EVENT_END;
    }

    if ( nFlag == -1 ) {
        return;
    }

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_SIGN_BOARD_ECLIPSE_ALARM_NOT, nFlag );
}

time_t KEclipseTimeEvent::GetCurrentPeriod()
{
    KLocker lock( m_csETE );
    return m_tmCurrentPeriod;
}

bool KEclipseTimeEvent::SendEclipseTimeEventInfoNot( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_ECLIPSE_TIME_EVENT_INFO_NOT, dwTemp );

    return true;
}

