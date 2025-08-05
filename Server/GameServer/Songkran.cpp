#include "User.h"
#include "Songkran.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <KncUtilLua.h> // boost::bind
#include <kncutil.h>
#include "NetError.h"
#include "UserPacket.h"
#include "ItemContainer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSongkran );
ImplOstmOperatorW2A( KSongkran );
NiImplementRootRTTI( KSongkran );

KSongkran::KSongkran(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwVersion(1)
,m_dwMaxGrade(1)
,m_dwMaxScore(0)
,m_fPlayerMoveSpeed(0.0f)
,m_fWaterBombFylingSpeed(0.0f)
,m_fWaterBombRange(0.0f)
,m_dwDailyMaxSupplyCount( 1 )
,m_WaterBombItem(0)
,m_nWaterBombSupplyInterval(5000)
,m_dwWaterBombSupplyDuration(1)
,m_dwOneTimeMaxScore(1)
{
    m_mapTargetInfoList.clear();
    m_mapGradeRewardList.clear();
}

KSongkran::~KSongkran(void)
{
}

ImplToStringW( KSongkran )
{
    KLocker lock( m_csSongkran );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwVersion )
        << TOSTRINGW( m_WaterBombItem )
        << TOSTRINGW( m_dwMaxScore )
        << TOSTRINGW( m_dwMaxGrade )
        << TOSTRINGW( m_fPlayerMoveSpeed )
        << TOSTRINGW( m_fWaterBombFylingSpeed )
        << TOSTRINGW( m_fWaterBombRange )
        << TOSTRINGW( m_dwDailyMaxSupplyCount )
        << TOSTRINGW( m_nWaterBombSupplyInterval )
        << TOSTRINGW( m_dwWaterBombSupplyDuration )
        << TOSTRINGW( m_dwOneTimeMaxScore )
        << TOSTRINGW( m_mapTargetInfoList.size() )
        << TOSTRINGW( m_mapGradeRewardList.size() );
}

bool KSongkran::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csSongkran );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KSongkran::GetGradeRewardList( IN const DWORD dwGrade_, OUT VEC_REWARD& vecReward_ )
{
    std::map<DWORD, KSongkranEventRewardInfo>::iterator mit;

    KLocker lock( m_csSongkran );
    mit = m_mapGradeRewardList.find( dwGrade_ );
    if( mit == m_mapGradeRewardList.end() ) {
        return false;
    }
    vecReward_ = mit->second.m_vecReward;

    return true;
}

// 해당 등급의 스코어 End point를 얻는다
void KSongkran::GetCurGradeEndScore( IN DWORD& dwGrade_, OUT DWORD& dwEndScore_ )
{
    std::map<DWORD, KSongkranEventRewardInfo>::iterator mit;

    KLocker lock( m_csSongkran );
    mit = m_mapGradeRewardList.find( dwGrade_ );
    if ( mit != m_mapGradeRewardList.end() ) {
        dwEndScore_ = mit->second.m_dwGradeEndScore;
    }
}

void KSongkran::GetSongkranInfo( OUT KEVENT_SONGKRAN_SCRIPT_INFO_ACK& kPacket_ ) // 클라에서 필요 한 정보들
{
    KLocker lock( m_csSongkran );

    kPacket_.m_fPlayerCharMoveSpeed = m_fPlayerMoveSpeed;       // 유저 캐릭터 이동 속도
    kPacket_.m_fWaterBombFlyingSpeed = m_fWaterBombFylingSpeed; // 물풍선 비행 속도
    kPacket_.m_fWaterBombRange = m_fWaterBombRange;             // 물풍선 폭발 범위
    kPacket_.m_nSupplyInterval = m_nWaterBombSupplyInterval;    // 물풍선 충전 주기
    kPacket_.m_dwSupplyDuration = m_dwWaterBombSupplyDuration;
    kPacket_.m_dwDailySupplyDuration = m_dwDailySupplyDuration;   // 첫 접속 시 충전 개수

    kPacket_.m_mapTargetInfoList = m_mapTargetInfoList;         // 타겟 정보 컨테이너
    kPacket_.m_mapGradeRewardList = m_mapGradeRewardList;       // 등급 보상 정보
}

bool KSongkran::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSongkran::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    GCITEMID    WaterBombItem;
    DWORD       dwVersion;
    DWORD       dwMaxGrade;

    std::map<USHORT, KSongkranEventTargetInfo> mapTargetInfoList;
    std::map<DWORD, KSongkranEventRewardInfo> mapGradeRewardList;

    float fPlayerMoveSpeed;
    float fWaterBombFylingSpeed;
    float fWaterBombRange;
    DWORD dwDailyMaxSupplyCount;
    int nWaterBombSupplyInterval;
    DWORD dwWaterBombSupplyDuration;
    DWORD dwOneTimeMaxScore;
    DWORD dwExchangeDuration;
    DWORD dwOneadaySupplyDuration; 

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitSongkranEvent.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    // 이벤트 설정값 읽어오기.
    _JIF( kLuaMng.GetValue( "Version", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "WaterBombItem", WaterBombItem ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxGrade", dwMaxGrade ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PlayerMoveSpeed", fPlayerMoveSpeed ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "WaterBombFylingSpeed", fWaterBombFylingSpeed ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "WaterBombRange", fWaterBombRange ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DailyMaxSupplyCount", dwDailyMaxSupplyCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "WaterBombSupplyInterval", nWaterBombSupplyInterval ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "WaterBombSupplyDuration", dwWaterBombSupplyDuration ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "OneTimeMaxScore", dwOneTimeMaxScore ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ExchangeDuration", dwExchangeDuration ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "OneadaySupplyDuration", dwOneadaySupplyDuration ) == S_OK, return false );
    _JIF( LoadTargetInfoList( kLuaMng, std::string("TargetInfoList"), mapTargetInfoList ), return false );
    _JIF( LoadGradeRewardList( kLuaMng, std::string("GradeRewardList"), dwMaxGrade, mapGradeRewardList ), return false );

    START_LOG( cerr, L"송크란 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csSongkran );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwVersion = dwVersion;
        m_WaterBombItem = WaterBombItem;
        m_dwMaxGrade = dwMaxGrade;
        m_fPlayerMoveSpeed = fPlayerMoveSpeed;
        m_fWaterBombFylingSpeed = fWaterBombFylingSpeed;
        m_fWaterBombRange = fWaterBombRange;
        m_dwDailyMaxSupplyCount = dwDailyMaxSupplyCount;
        m_nWaterBombSupplyInterval = nWaterBombSupplyInterval;
        m_dwWaterBombSupplyDuration = dwWaterBombSupplyDuration;
        m_dwOneTimeMaxScore = dwOneTimeMaxScore;
        m_dwExchangeDuration = dwExchangeDuration;
        m_dwDailySupplyDuration = dwOneadaySupplyDuration;

        m_mapTargetInfoList.swap( mapTargetInfoList );
        m_mapGradeRewardList.swap( mapGradeRewardList );

        return true;
    }
}


bool KSongkran::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear = 0, nMonth = 0, nDay = 0, nHour = 0;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"송크란 이벤트 날짜 정보 스크립트 오류" ) << END_LOG;
            return false;
    }
    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KSongkran::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    vecRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSongkran::LoadGradeRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN DWORD& dwMaxGrade_, OUT std::map<DWORD, KSongkranEventRewardInfo>& mapGradeRewardList_ )
{
    mapGradeRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    DWORD dwPrevGradeEndScore = 0;
    for( int i = 1; ; ++i ) {
        DWORD dwGrade;
        KSongkranEventRewardInfo kRewardInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Grade", dwGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GradeEndScore", kRewardInfo.m_dwGradeEndScore ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "SpecialReward", kRewardInfo.m_bSpecialReward ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("Reward"), kRewardInfo.m_vecReward ), return false );

        if ( i != dwGrade ) { // index는 1부터 시작하니까 Grade1~n까지 다 채워졌는지 비교
            START_LOG( cerr, L"GradeRewardList의 Grade값이 잘못 설정 됨" ) << END_LOG;
            return false;
        }
        // kRewardInfo.m_nGradeEndScore 이전 읽은 값이랑 비교 해서 더 작으면 return false;
        if ( dwPrevGradeEndScore > kRewardInfo.m_dwGradeEndScore ) {
            START_LOG( cerr, L"GradeEndScore 값을 잘못 설정함" ) << END_LOG;
            return false;
        }
        dwPrevGradeEndScore = kRewardInfo.m_dwGradeEndScore;

        if( !mapGradeRewardList_.insert( std::make_pair( dwGrade, kRewardInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 Grade 정보가 있음. Grade : " << dwGrade ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( dwMaxGrade_ != mapGradeRewardList_.size() ) {
        START_LOG( cerr, L"MaxGrade값과 GradeRewardList의 갯수가 다름" ) << BUILD_LOG( m_dwMaxGrade ) << BUILD_LOG( mapGradeRewardList_.size() ) << END_LOG;
        return false;
    }

    return true;
}

bool KSongkran::LoadTargetInfoList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<USHORT, KSongkranEventTargetInfo>& mapTargetInfoList_ )
{
    mapTargetInfoList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        int nCharType;
        KSongkranEventTargetInfo kTargetInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MoveSpeed", kTargetInfo.m_fTargetMoveSpeed ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Score", kTargetInfo.m_dwTargetScore) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "TelpoPeriod", kTargetInfo.m_fTeleportPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ResetTime", kTargetInfo.m_fHitTargetAfterResetTime ) == S_OK, return false );

        if( !mapTargetInfoList_.insert( std::make_pair( nCharType, kTargetInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 nCharType 정보가 있음. nCharType : " << nCharType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KSongkran::GetUserScoreGrade( IN const DWORD& dwScore_, OUT DWORD& dwGrade_ )
{
    std::map<DWORD, KSongkranEventRewardInfo>::iterator mit;

    KLocker lock( m_csSongkran );
    for( mit = m_mapGradeRewardList.begin(); mit != m_mapGradeRewardList.end(); ++mit ) {
        if( dwScore_ < mit->second.m_dwGradeEndScore ) {
            return;
        } else {
            dwGrade_ = mit->first;
        }
    }
    dwGrade_ = GetMaxGrade();
    return;
}

bool KSongkran::Get_Songkran_UserData_FromDB( IN const std::string& strScript_ )
{
    bool bTemp = true;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::DB_EVENT_SONGKRAN_USER_INFO_REQ, bTemp );

    return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


KSongkranUserData::KSongkranUserData()
:m_bLoaded(false)
,m_bFinishToday(false)
,m_dwGrade(1)
,m_dwScore(0)
,m_nAccPlayTime(0)
,m_dwWaterBombDuration(0)
,m_dwSuppliedWaterBombDuration(0)
,m_dwUseCount(0)
{
}

KSongkranUserData::~KSongkranUserData()
{
}

// 보상 등급 증가
void KSongkranUserData::IncreaseGrade()
{
    if ( m_dwGrade > SiKSongkran()->GetMaxGrade() ) { // 최종 보상 획득 유무 판별을 위해 >로 검사. (획득 시 최종 보상 +1 만큼의 Grade가 된다)
        START_LOG( cerr, L"지급 가능 보상의 최대 수치를 넘었음" ) << END_LOG;
        return;
    }

    ++m_dwGrade;
    m_dwInitGrade = m_dwGrade;
}

////////////////////////////////////////////////////////////////
// 물풍선 충전 관련 부분 시작
bool KSongkranUserData::UpdateAccTime( IN const DWORD dwUID_, IN const std::wstring& strLogin_, IN const bool bChangeDate_, IN const KSimpleDate& kToday_, OUT KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT& kNotPacket_ )
{
    int dwWaterBombSupplyDuration = SiKSongkran()->GetWaterBombSupplyDuration(); // 한번에 지급 하는 물풍 갯수

    IncreaseAccTime( dwUID_, strLogin_, bChangeDate_ );

    int nAccTime = GetAccTime();
    // 물풍을 받을수 있나?
    if ( CheckTodayWaterBombRecvable( nAccTime ) ) {
        m_dwWaterBombDuration += dwWaterBombSupplyDuration; // 물풍 갯수 증가
        m_dwInitWaterBombDuration = m_dwWaterBombDuration;
        std::min<int> ( INT_MAX, m_dwWaterBombDuration ); // 최대값 보장
        m_dwSuppliedWaterBombDuration += dwWaterBombSupplyDuration; // 일일 지급 개수 증가

        UpdateFinishToday();
        ResetAccTime(); // 누적 시간 초기화

        if ( false == m_bFinishToday ) {
            kNotPacket_.m_nOK = KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT::PERIOD_SUPPLY;
        }
        else {
            kNotPacket_.m_nOK = KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT::PERIOD_SUPPLY_LAST;
        }
        kNotPacket_.m_dwWaterBombDuration = m_dwWaterBombDuration;

        return true;
    }

    return false;
}

void KSongkranUserData::IncreaseAccTime( IN const DWORD dwUID_, IN const std::wstring& strLogin_, IN const bool bChangeDate_ )
{
    if ( true == bChangeDate_ ) { // 하루가 넘어가면 초기화
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_SONGKRAN_USER_INFO_REQ, strLogin_.c_str(), dwUID_, 0, bChangeDate_ );
    }

    if ( true == IsFinishedToday() ) {
        return;
    }
    ++m_nAccPlayTime;
}

void KSongkranUserData::ResetAccTime()
{
    m_nAccPlayTime = 0;
    //m_bFinishToday = false;
}

void KSongkranUserData::UpdateFinishToday()
{
    if ( true != IsFinishedToday() ) {
        // 지급할 때 물풍 개수가 최대 개수에 도달하는지 체크
        if ( SiKSongkran()->GetDailyMaxSupplyDuration() <= m_dwSuppliedWaterBombDuration ) {
            m_bFinishToday = true;
        }
    }
}

bool KSongkranUserData::CheckTodayWaterBombRecvable( IN const int nAccPlayTime_ )
{
    // 설정 된 시간이 되고 오늘 지급 완료가 아니면
    if ( ( SiKSongkran()->GetWaterBombSupplyInterval() <= nAccPlayTime_ ) && false == IsFinishedToday() ) {   
        return true;
    }

    return false;
}

void KSongkranUserData::SetSongkranUserEventData( IN DWORD dwGrade_, IN DWORD dwScore_, IN int nAccPlayTime_, IN DWORD dwWaterBombDuration_, IN DWORD dwSuppliedWaterBombDuration_ )
{
    m_dwInitGrade = std::min<DWORD>( ULONG_MAX, dwGrade_ );
    m_dwGrade = m_dwInitGrade;
    m_dwInitScore = std::min<DWORD>( ULONG_MAX, dwScore_ );    
    m_dwScore = m_dwInitScore;

    m_nAccPlayTime = std::min<int>( INT_MAX, nAccPlayTime_ );
    m_nAccPlayTime = std::max<int>( 0, nAccPlayTime_ );
    m_dwWaterBombDuration = dwWaterBombDuration_;
    m_dwInitWaterBombDuration = dwWaterBombDuration_;
    m_dwSuppliedWaterBombDuration = dwSuppliedWaterBombDuration_;

    m_bLoaded = true;
}

void KSongkranUserData::GetSongkranUserEventDate( OUT KEVENT_SONGKRAN_USER_INFO_ACK& kPacket_ )
{
    kPacket_.m_dwGrade = m_dwGrade;
    kPacket_.m_dwScore = m_dwScore;
    kPacket_.m_dwWaterBombDuration = m_dwWaterBombDuration;
    kPacket_.m_nAccTime = m_nAccPlayTime;
    kPacket_.m_bFinish = m_bFinishToday;
}

void KSongkranUserData::UseWaterBomb( IN DWORD dwScoreDiff_ )
{
    // 물풍 개수 감소
    m_dwWaterBombDuration = std::max<DWORD>( 0, m_dwWaterBombDuration - 1 );
    m_dwInitWaterBombDuration = std::max<DWORD>( 0, m_dwInitWaterBombDuration - 1 );
    // 점수 증가
    m_dwScore = std::min<DWORD>( ULONG_MAX, m_dwScore + dwScoreDiff_ );
    m_dwInitScore = std::min<DWORD>( ULONG_MAX, m_dwInitScore + dwScoreDiff_ );
    // 통계용 데이터 증가
    ++m_dwUseCount;
}

void KSongkranUserData::GetSongkranCachingData( OUT KSongkranEventUserData& kData )
{
    kData.m_dwScoreDiff = m_dwScore - m_dwInitScore;
    kData.m_dwGradeDiff = m_dwGrade - m_dwInitGrade;
    kData.m_nWaterBombDurationDiff = static_cast<int>( m_dwWaterBombDuration - m_dwInitWaterBombDuration ); // 얘는 -도 돼야해서 int
    kData.m_dwSuppliedWaterBombDuration = m_dwSuppliedWaterBombDuration;
    kData.m_nAccTime = m_nAccPlayTime;
    kData.m_dwUseCount = m_dwUseCount;
    kData.m_bLoaded = m_bLoaded;
}
// 끝
//////////////////////////////////////////////////////////////////////////