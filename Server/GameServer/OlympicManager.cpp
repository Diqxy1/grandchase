#include "CnConnector.h" // winsock
#include "GSSimLayer.h"
#include "OlympicManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include <kncutil.h>
#include "Lottery.h"
#include "UserEvent.h"
#include "Log4.h"
#include <boost/bind.hpp>

ImplementSingleton( KOlympicManager );
ImplOstmOperatorW2A( KOlympicManager );
NiImplementRootRTTI( KOlympicManager );

KOlympicManager::KOlympicManager(void)
:m_tmBegin(0)
,m_tmEnd(0)
,m_tmTodayDate(0)
,m_nGameType(0)
,m_nTeamType(0)
,m_tmUpdateTime(0)
,m_dwTickGap(1000*60)
,m_nTimeIndex(0)
,m_dwCountryID(0)
,m_biTotalData(0)
,m_nDeltaData(0)
,m_nTopPlayerData(0)
,m_tmStatDate(0)
,m_bChanged(false)
{
    m_dwLastTick = ::GetTickCount();
    m_vecOlympicSchedule.clear();
    m_mapOlympicGameInfo.clear();
    m_mapGradeRewardList.clear();
    m_mapGradeDesc.clear();
    m_vecTimeInfo.clear();
    m_setGameAlarmNotice.clear();
}

KOlympicManager::~KOlympicManager(void)
{
}

ImplToStringW( KOlympicManager )
{
    KLocker lock( m_csOlympic );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_vecOlympicSchedule.size() )
        << TOSTRINGW( m_mapOlympicGameInfo.size() )
        << TOSTRINGW( m_mapGradeRewardList.size() )
        << TOSTRINGW( m_mapGradeDesc.size() )
        << TOSTRINGW( m_vecRegDateRank.size() )
        << TOSTRINGW( m_vecTotalRank.size() )
        << TOSTRINGWt( CTime(m_tmTodayDate) )
        << TOSTRINGW( m_nGameType )
        << TOSTRINGW( m_nTeamType )
        << TOSTRINGWt( CTime(m_tmUpdateTime) )
        << TOSTRINGW( m_nTimeIndex )
        << TOSTRINGW( m_dwCountryID )
        << TOSTRINGWt( CTime(m_tmStatDate) )
        << TOSTRINGW( m_nStatGameType )
        << TOSTRINGW( m_nStatTeamType )
        << TOSTRINGW( m_biTotalData )
        << TOSTRINGW( m_nDeltaData )
        << TOSTRINGW( m_nTopPlayerData )
        << TOSTRINGW( m_strNickName );
}

bool KOlympicManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KOlympicManager::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwMaxGrade;       // 최대 등급.
    DWORD       dwMaxPoint;       // 최대 Point.
    DWORD       dwCountryID = 0;
    std::vector<KOlympicSchedule>                   vecOlympicSchedule;
    std::map<int, KOlympicGameInfo>                 mapOlympicGameInfo;
    std::map<DWORD,VEC_REWARD>                      mapGradeRewardList;
    std::map<DWORD,std::pair<DWORD,DWORD>>          mapGradeDesc;
    std::map< int, PAIR_INT >                       mapOlympicGawibawiboWinLoseRatio;
    std::vector<KTimeIndexInfo>                     vecTimeInfo;
    std::set<int>                                   setGameAlarmNotice;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitOlympic.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 국가 정보 읽어오기.
    _JIF( kLuaMng.GetValue( "CountryCode", dwCountryID ) == S_OK, return false );
    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    // 올림픽 스케줄 정보 가져오기.
    _JIF( LoadOlympicScheduleInfo( kLuaMng, std::string("OlympicSchedule"), vecOlympicSchedule ), return false );
    // 올림픽 게임 세부정보 가져오기.
    _JIF( LoadOlympicGameInfo( kLuaMng, std::string("OlympicGameInfo"), mapOlympicGameInfo ), return false );
    // 등급별 보상정보 가져오기.
    _JIF( LoadGradeRewardInfo( kLuaMng, std::string("GradeRewardInfo"), mapGradeRewardList ), return false );
    // 등급별 포인트 범위 정보 가져오기.
    _JIF( LoadGradeDescInfo( kLuaMng, std::string( "GradeRewardDesc" ), mapGradeDesc, dwMaxGrade, dwMaxPoint ), return false );
    // 가위바위보 승률 정보 가져오기
    _JIF( LoadOlympicGawibawiboWinLoseRatio( kLuaMng, std::string( "GawibawiboInfo" ), mapOlympicGawibawiboWinLoseRatio ), return false );
    // 타임 인덱스 정보 가져오기.
    _JIF( LoadTimeIndexDescInfo( kLuaMng, std::string( "TimeIndexDesc" ), vecTimeInfo, setGameAlarmNotice ), return false );

    START_LOG( cerr, L"올림픽 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csOlympic );
        m_dwCountryID = dwCountryID;
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwMaxGrade = dwMaxGrade;
        m_dwMaxPoint = dwMaxPoint;
        m_vecOlympicSchedule.swap( vecOlympicSchedule );
        m_mapOlympicGameInfo.swap( mapOlympicGameInfo );
        m_mapGradeRewardList.swap( mapGradeRewardList );
        m_mapGradeDesc.swap( mapGradeDesc );
        m_mapOlympicGawibawiboWinLoseRatio.swap( mapOlympicGawibawiboWinLoseRatio );
        m_vecTimeInfo.swap( vecTimeInfo );
        m_setGameAlarmNotice.swap( setGameAlarmNotice );
    }

    return true;
}

bool KOlympicManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KOlympicManager::LoadGradeRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,VEC_REWARD>& mapGradeRewardList_ )
{
    mapGradeRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        DWORD dwGrade;
        VEC_REWARD vecReward;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Grade", dwGrade ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("RewardInfo"), vecReward ), return false );

        if( !mapGradeRewardList_.insert( std::make_pair( dwGrade, vecReward ) ).second ) {
            START_LOG( cerr, L" 중복된 Grade 정보가 있음. Grade : " << dwGrade ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
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

bool KOlympicManager::LoadGradeDescInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc_, OUT DWORD& dwMaxGrade_, OUT DWORD& dwMaxPoint_ )
{
    mapGradeDesc_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxGrade", dwMaxGrade_ ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MaxPoint", dwMaxPoint_ ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        DWORD dwGradeID;
        std::pair<DWORD,DWORD> prData;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "GradeID", dwGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "StratPoint", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndPoint", prData.second ) == S_OK, return false );

        if( !mapGradeDesc_.insert( std::make_pair( dwGradeID, prData ) ).second ) {
            START_LOG( cerr, L" 중복된 등급 Desc 정보가 있음. GraneID : " << dwGradeID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::LoadOlympicScheduleInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KOlympicSchedule>& vecOlympicSchedule_ )
{
    vecOlympicSchedule_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        KOlympicSchedule kOlympicSchedule;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( LoadTime( kLuaMng_, std::string("RegDate"), kOlympicSchedule.m_tmRegDate ), return false );
        _JIF( kLuaMng_.GetValue( "GameType", kOlympicSchedule.m_nGameType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "TeamType", kOlympicSchedule.m_nTeamType ) == S_OK, return false );

        vecOlympicSchedule_.push_back( kOlympicSchedule );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::LoadOlympicGameInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, KOlympicGameInfo>& mapOlympicGameInfo_ )
{
    mapOlympicGameInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        KOlympicGameInfo kOlympicGameInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "GameType", kOlympicGameInfo.m_nGameType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DefaultCount", kOlympicGameInfo.m_nDefaultCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "RewardCount", kOlympicGameInfo.m_nRewardCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "OnePointMaxPoint", kOlympicGameInfo.m_nOnePointMaxPoint ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "TotalMaxPoint", kOlympicGameInfo.m_nTotalMaxPoint ) == S_OK, return false );
        _JIF( LoadReward( kLuaMng_, std::string("RewardInfo"), kOlympicGameInfo.m_vecRewardInfo ), return false );

        if( !mapOlympicGameInfo_.insert( std::make_pair( kOlympicGameInfo.m_nGameType, kOlympicGameInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 GameType 정보가 있음. GameType : " << kOlympicGameInfo.m_nGameType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::LoadOlympicGawibawiboWinLoseRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, PAIR_INT >& mapOlympicGawibawiboWinLoseRatio_ )
{
    mapOlympicGawibawiboWinLoseRatio_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        int nRemainTryPoint = 0;
        PAIR_INT prWinLose;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "RemainTryPoint", nRemainTryPoint ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Win", prWinLose.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Lose", prWinLose.second ) == S_OK, return false );

        if ( prWinLose.first + prWinLose.second > 100 ) {
            START_LOG( cerr, L" 100%가 넘는 확률 설정 시도. RemainTryPoint : " << nRemainTryPoint ) << END_LOG;
            return false;
        }

        if ( false == mapOlympicGawibawiboWinLoseRatio_.insert( std::make_pair( nRemainTryPoint, prWinLose ) ).second ) {
            START_LOG( cerr, L" 중복된 RemainTryPoint 정보가 있음. RemainTryPoint : " << nRemainTryPoint ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KOlympicManager::LoadTimeIndexDescInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KTimeIndexInfo>& vecTimeInfo_, OUT std::set<int>& setGameAlarmNotice_ )
{
    vecTimeInfo_.clear();
    setGameAlarmNotice_.clear(); 

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( LoadSetInfo( kLuaMng_, std::string("GameAlarmNotice"), setGameAlarmNotice_ ), return false );
    for( int i = 1; ; ++i ) {
        KTimeIndexInfo kTimeIndexInfo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Index", kTimeIndexInfo.m_nTimeIndex ) == S_OK, return false );
        _JIF( LoadPairInfo( kLuaMng_, std::string("StartTime"), kTimeIndexInfo.m_prStartTime ), return false );
        _JIF( LoadPairInfo( kLuaMng_, std::string("EndTime"), kTimeIndexInfo.m_prEndTime ), return false );
        vecTimeInfo_.push_back( kTimeIndexInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::LoadSetInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<int>& setData_ )
{
    setData_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setData_.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::LoadPairInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair<DWORD,DWORD>& prData_ )
{
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, prData_.first ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, prData_.second ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KOlympicManager::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csOlympic );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KOlympicManager::GetGradeRewardList( IN const DWORD dwGrade_, OUT VEC_REWARD& vecReward_ )
{
    std::map<DWORD,VEC_REWARD>::iterator mit;

    KLocker lock( m_csOlympic );
    mit = m_mapGradeRewardList.find( dwGrade_ );
    if( mit == m_mapGradeRewardList.end() ) {
        return false;
    }
    vecReward_ = mit->second;
    return true;
}

bool KOlympicManager::GetPointGrade( IN const DWORD dwBonusPoint_, OUT DWORD& dwGrade_ )
{
    dwGrade_ = 0;
    std::map<DWORD,std::pair<DWORD,DWORD>>::iterator mit;

    KLocker lock( m_csOlympic );
    for( mit = m_mapGradeDesc.begin(); mit != m_mapGradeDesc.end(); ++mit ) {
        if( dwBonusPoint_ >= mit->second.second ) {
            dwGrade_ = mit->first;
        }
    }

    return (dwGrade_ != 0 );
}

void KOlympicManager::GetOlympicScheduleInfo( OUT std::vector<KOlympicSchedule>& vecOlympicSchedule_ )
{
    KLocker lock( m_csOlympic );
    vecOlympicSchedule_.clear();
    vecOlympicSchedule_ = m_vecOlympicSchedule;
}

void KOlympicManager::GetOlympicRegDateRank( OUT std::vector<KRegDateRankInfo>& vecRegDateRank_ )
{
    KLocker lock( m_csOlympic );
    vecRegDateRank_.clear();
    vecRegDateRank_ = m_vecRegDateRank;
}

void KOlympicManager::GetOlympicTotalRank( OUT std::vector<KTotalRankInfo>& vecTotalRank_ )
{
    KLocker lock( m_csOlympic );
    vecTotalRank_.clear();
    vecTotalRank_ = m_vecTotalRank;
}

void KOlympicManager::GetOlympicGradeRewardList( OUT std::map<DWORD, std::vector<KDropItemInfo>>& mapGradeRewardList_ )
{
    KLocker lock( m_csOlympic );
    mapGradeRewardList_.clear();
    mapGradeRewardList_ = m_mapGradeRewardList;
}

void KOlympicManager::GetOlympicGradeDesc( OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc_ )
{
    KLocker lock( m_csOlympic );
    mapGradeDesc_.clear();
    mapGradeDesc_ = m_mapGradeDesc;
}

bool KOlympicManager::GetGameInfo( IN const int& nGameType_, OUT int& nDefaultCount_, OUT int& nRewardCount_, OUT int& nOnePointMaxPoint_, OUT int& nTotalMaxPoint_ )
{
    std::map<int, KOlympicGameInfo>::iterator mit;
    KLocker lock( m_csOlympic );
    mit = m_mapOlympicGameInfo.find( nGameType_ );
    if( mit == m_mapOlympicGameInfo.end() ) {
        return false;
    }

    nDefaultCount_ = mit->second.m_nDefaultCount;
    nRewardCount_ = mit->second.m_nRewardCount;
    nOnePointMaxPoint_ = mit->second.m_nOnePointMaxPoint;
    nTotalMaxPoint_ = mit->second.m_nTotalMaxPoint;
    return true;
}

bool KOlympicManager::GetOlympicGameInfo( IN const int& nGameType_, OUT KOlympicGameInfo& kOlympicGameInfo_ )
{
    std::map< int, KOlympicGameInfo >::iterator mit;
    KLocker lock( m_csOlympic );
    mit = m_mapOlympicGameInfo.find( nGameType_ );
    if( mit == m_mapOlympicGameInfo.end() ) {
        return false;
    }

    kOlympicGameInfo_ = mit->second;

    return true;
}

void KOlympicManager::Tick()
{
    if( !ChkOneMinTick() || !IsRun() ) { // 1분마다 / 이벤트 기간 체크.
        return;
    }

    SendCenterSyncData();

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    if( ChkGameAlarm( tmCurrent ) && IsGamePlayEnable() ) { // 게임 알람 
        SendGameAlarmNotice( tmCurrent );
    }

    if( ChkUpdateDay( tmCurrent ) ) { // 하루가 지났는지에 대한 체크.
        UpdateGameInfo( tmCurrent );
    }
}

void KOlympicManager::UpdateOlympicRankData( IN std::vector<KRegDateRankInfo>& vecRegDateRank_, IN std::vector<KTotalRankInfo>& vecTotalRank_ )
{
    KLocker lock( m_csOlympic );
    m_vecRegDateRank = vecRegDateRank_;
    m_vecTotalRank = vecTotalRank_;
}

void KOlympicManager::InitProcess()
{
    CTime tmCurrent = CTime::GetCurrentTime();
    time_t tmTodayDate = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 ) );
    bool bChange = false;
    int nTimeIndex;

    std::vector<KOlympicSchedule>::iterator vit;
    KLocker lock( m_csOlympic );
    for( vit = m_vecOlympicSchedule.begin(); vit != m_vecOlympicSchedule.end(); ++vit ) {
        if( vit->m_tmRegDate == tmTodayDate ) {
            m_tmTodayDate = vit->m_tmRegDate;
            m_nGameType = vit->m_nGameType;
            m_nTeamType = vit->m_nTeamType;
            bChange = true;
            break;
        }
    }

    if( !bChange ) { // 금일 플레이 가능한 게임이 없으면 초기값으로 세팅.
        m_tmTodayDate = 0;
        m_nGameType = 0;
        m_nTeamType = 0;
    }

    CTimeSpan ts(1,0,0,0); // 내일.
    CTime tmUpdate( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 );
    tmUpdate += ts;
    m_tmUpdateTime = KncUtil::TimeToInt( tmUpdate );
    SetCurrentTimeIndex( nTimeIndex );
    m_nTimeIndex = nTimeIndex;
}

bool KOlympicManager::ChkOneMinTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();
    return true;
}

bool KOlympicManager::ChkGameAlarm( IN time_t& tmDate_ )
{
    KLocker lock( m_csOlympic );
    bool bRet = false;
    CTime tmCurrent( tmDate_ );
    int nHour = tmCurrent.GetHour();
    int nMinute = tmCurrent.GetMinute();
    int nTimeIndex = 0;

    std::set<int>::iterator sit;
    sit = m_setGameAlarmNotice.find( nMinute );
    if( sit != m_setGameAlarmNotice.end() ) { // 스크립트에 지정된 시간에 알람.
        nTimeIndex = GetTimeIndex( nHour, nMinute );
        bRet = true;
    }
    if( bRet ) { // TimeIndex 갱신.
        KLocker lock( m_csOlympic );
        m_nTimeIndex = nTimeIndex;
    }

    return bRet;
}

bool KOlympicManager::ChkUpdateDay( IN time_t& tmDate_ )
{
    KLocker lock( m_csOlympic );
    bool bRet = false;
    CTime tmCurrent( tmDate_ );
    CTime tmUpdate( m_tmUpdateTime );
    if( tmUpdate < tmCurrent ) {
        CTimeSpan ts(1,0,0,0); // 내일.
        tmUpdate += ts;
        bRet = true;
        m_tmUpdateTime = KncUtil::TimeToInt( tmUpdate );
    }

    return bRet;
}

void KOlympicManager::SendGameAlarmNotice( IN time_t& tmDate_ )
{
    CTime tmCurrent( tmDate_ );

    KEVENT_OLYMPIC_ALARM_NOT kPacket;
    kPacket.m_nTimeIndex = m_nTimeIndex;
    kPacket.m_nGameType = m_nGameType;
    kPacket.m_nTeamType = m_nTeamType;
    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_OLYMPIC_ALARM_NOT, kPacket );

    START_LOG( clog, L"올림픽 이벤트 게임참여 가능 알림 메세지 전달." )
        << BUILD_LOGtm( tmCurrent )
        << BUILD_LOG( kPacket.m_nGameType ) 
        << BUILD_LOG( kPacket.m_nTeamType ) 
        << BUILD_LOG( kPacket.m_nTimeIndex ) 
        << END_LOG;
}

int KOlympicManager::GetTimeIndex( IN int& nHour_, IN int& nMinute_ )
{
    int nTimeIndex = 0;
    std::vector<KTimeIndexInfo>::iterator vit;
    KLocker lock( m_csOlympic );
    for( vit = m_vecTimeInfo.begin(); vit != m_vecTimeInfo.end(); ++vit ) {
        if( vit->m_prStartTime.first == (DWORD)nHour_ && vit->m_prStartTime.second <= (DWORD)nMinute_ && vit->m_prEndTime.second >= (DWORD)nMinute_ ) {
            nTimeIndex = vit->m_nTimeIndex;
            break;
        }
    }
    return nTimeIndex;
}

void KOlympicManager::UpdateGameInfo( IN time_t& tmDate_ )
{
    CTime tmCurrent( tmDate_ );
    time_t tmTodayDate = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 ) );
    bool bChange = false;

    std::vector<KOlympicSchedule>::iterator vit;
    KLocker lock( m_csOlympic );
    for( vit = m_vecOlympicSchedule.begin(); vit != m_vecOlympicSchedule.end(); ++vit ) {
        if( vit->m_tmRegDate == tmTodayDate ) {
            m_tmTodayDate = vit->m_tmRegDate;
            m_nGameType = vit->m_nGameType;
            m_nTeamType = vit->m_nTeamType;
            bChange = true;
            break;
        }
    }

    if( !bChange ) { // 금일 플레이 가능한 게임이 없으면 초기값으로 세팅.
        m_tmTodayDate = 0;
        m_nGameType = 0;
        m_nTeamType = 0;
    }

    CTimeSpan ts(1,0,0,0); // 내일.
    CTime tmUpdate( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 );
    tmUpdate += ts;
    m_tmUpdateTime = KncUtil::TimeToInt( tmUpdate );
}

void KOlympicManager::GetCurrentOlympicGameInfo( OUT time_t& tmTodayDate_, OUT int& nGameType_, OUT int& nTeamType_ )
{
    KLocker lock( m_csOlympic );

    tmTodayDate_ = m_tmTodayDate;
    nGameType_ = m_nGameType;
    nTeamType_ = m_nTeamType;
}

// 물폭탄 함수들

// 가위바위보 함수들
bool KOlympicManager::DoOlympicGawibawibo( IN const int& nRemainTryPoint_, OUT int& nResult_ )
{
    nResult_ = 0;

    std::map< int, PAIR_INT >::iterator mitWinLoseRatio;
    mitWinLoseRatio = m_mapOlympicGawibawiboWinLoseRatio.find( nRemainTryPoint_ );
    if ( mitWinLoseRatio == m_mapOlympicGawibawiboWinLoseRatio.end() ) {
        START_LOG( cerr, L"지정된 도전 포인트 정보가 없음: " << nRemainTryPoint_ ) << END_LOG;
        return false;
    }

    KLottery kLottery;
    kLottery.AddCase( KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_WIN, static_cast< float >( mitWinLoseRatio->second.first ) );
    kLottery.AddCase( KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_LOSE, static_cast< float >( mitWinLoseRatio->second.second ) );
    kLottery.AddCase( KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_DRAW,  100.f - ( mitWinLoseRatio->second.first + mitWinLoseRatio->second.second ) );

    nResult_ = kLottery.Decision();

    return true;
}

// 주사위 함수들
void KOlympicManager::GetOlympicDiceNum( OUT int& nFirstDiceNum_, OUT int& nSecondDiceNum_ )
{
    nFirstDiceNum_ = 0;
    nFirstDiceNum_ = 0;

    srand( ::GetTickCount() );
    nFirstDiceNum_ = rand() % 6 + 1;
    nSecondDiceNum_ = rand() % 6 + 1;
}

bool KOlympicManager::CheckGameResultData( IN const int& nGameType_, IN const int& nAccumulateData_, IN OUT int& nResultData_ )
{
    std::map<int, KOlympicGameInfo>::iterator mit;
    int nAfterAccumulateData = 0;
    KLocker lock( m_csOlympic );

    mit = m_mapOlympicGameInfo.find( nGameType_ );
    if( mit == m_mapOlympicGameInfo.end() ) {
        return false;
    }

    nResultData_ = std::min<int>( nResultData_, mit->second.m_nOnePointMaxPoint );
    nAfterAccumulateData = nAccumulateData_;
    nAfterAccumulateData += nResultData_;
    if( nAfterAccumulateData > mit->second.m_nTotalMaxPoint ) { // 최대 MaxPoint 보정처리.
        nResultData_ = mit->second.m_nTotalMaxPoint - nAccumulateData_;
        nResultData_ = std::max<int>( nResultData_, 0 ); // 음수에 대한 보정체크.
    }
    return true;
}

bool KOlympicManager::IsGamePlayEnable()
{
    KLocker lock( m_csOlympic );
    if( m_nGameType == 0 ) {
        return false;
    }
    return true;
}

void KOlympicManager::GetGameRewardInfo( IN const int& nGameType_, OUT std::vector<KDropItemInfo>& vecRewardInfo_ )
{
    vecRewardInfo_.clear();
    std::map<int, KOlympicGameInfo>::iterator mit;

    KLocker lock( m_csOlympic );
    mit = m_mapOlympicGameInfo.find( nGameType_ );
    if( mit != m_mapOlympicGameInfo.end() ) {
        vecRewardInfo_ = mit->second.m_vecRewardInfo;
    }
}

bool KOlympicManager::CheckUserTimeIndex( IN const time_t& tmTodayDate_, IN const int& nTimeIndex_ )
{
    KLocker lock( m_csOlympic );
    if( m_tmTodayDate == tmTodayDate_ && m_nTimeIndex == nTimeIndex_ ) {
        return false;
    }
    return true;
}

void KOlympicManager::UpdateOlympicSoloPlayData( IN const std::wstring& strNickName_, IN const time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const int nTotalPlayerData_ )
{
    KLocker lock( m_csOlympic );
    if( tmDate_ != m_tmStatDate && tmDate_ == m_tmTodayDate ) {
        m_tmStatDate = tmDate_;
        m_nStatGameType = nGameType_;
        m_nStatTeamType = nTeamType_;
        m_strNickName = strNickName_;
        m_nTopPlayerData = nTotalPlayerData_;
        m_bChanged = true;
        return;
    }

    if( m_nTopPlayerData < nTotalPlayerData_ ) { // 개인전 데이터 갱신되었는지 체크.
        m_strNickName = strNickName_;
        m_nTopPlayerData = nTotalPlayerData_;
        m_bChanged = true;
    }
}

void KOlympicManager::UpdateOlympicTeamPlayData( IN const time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const int& nIncreasedData_ )
{
    KLocker lock( m_csOlympic );
    if( tmDate_ != m_tmStatDate && tmDate_ == m_tmTodayDate ) {
        m_tmStatDate = tmDate_;
        m_nStatGameType = nGameType_;
        m_nStatTeamType = nTeamType_;
        m_biTotalData = 0;
        m_nDeltaData = 0;
        m_nDeltaData += nIncreasedData_;
        return;
    }

    m_nDeltaData += nIncreasedData_; // 단체전 누적 데이터 갱신.
}

void KOlympicManager::SendCenterSyncData()
{
    KLocker lock( m_csOlympic );

    // CenterServer로 갱신해줄 데이터가 있을 경우에만 전송.
    if( m_nStatTeamType == KOlympicSchedule::OGT_SOLO && m_bChanged ) {
        KECN_OLYMPIC_SYNC_SOLO_DATA_NOT kPacket;
        kPacket.m_tmTodayDate = m_tmStatDate;
        kPacket.m_nGameType = m_nStatGameType;
        kPacket.m_nTeamType = m_nStatTeamType;
        kPacket.m_strNickName = m_strNickName;
        kPacket.m_nTopPlayerData = m_nTopPlayerData;
        LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_OLYMPIC_SYNC_SOLO_DATA_NOT, kPacket ) );
        m_bChanged = false;
    } else if( m_nStatTeamType == KOlympicSchedule::OGT_TEAM && m_nDeltaData > 0 ) {
        KECN_OLYMPIC_SYNC_TEAM_DATA_REQ kPacket;
        kPacket.m_tmTodayDate = m_tmStatDate;
        kPacket.m_nGameType = m_nStatGameType;
        kPacket.m_nTeamType = m_nStatTeamType;
        kPacket.m_nDeltaData = m_nDeltaData;
        m_nDeltaData = 0;
        LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_OLYMPIC_SYNC_TEAM_DATA_REQ, kPacket ) );
    }
}

void KOlympicManager::UpdateOlympicSoloData( IN const time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const int& nTopPlayerData_, IN const std::wstring& strNickName_ )
{
    KLocker lock( m_csOlympic );

    m_tmStatDate = tmDate_;
    m_nStatGameType = nGameType_;
    m_nStatTeamType = nTeamType_;
    m_nTopPlayerData = nTopPlayerData_;
    m_strNickName = strNickName_;
}

void KOlympicManager::UpdateOlympicTeamData( IN const time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const __int64& biTotalData_ )
{
    KLocker lock( m_csOlympic );

    m_tmStatDate = tmDate_;
    m_nStatGameType = nGameType_;
    m_nStatTeamType = nTeamType_;
    m_biTotalData = biTotalData_;
}

void KOlympicManager::UpdateOlympicEventData( IN KECN_OLYMPIC_EVENT_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csOlympic );
    m_tmStatDate = kPacket_.m_tmRegDate;
    m_nStatGameType = kPacket_.m_nGameType;
    m_nStatTeamType = kPacket_.m_nTeamType;
    if( kPacket_.m_nTeamType == KOlympicSchedule::OGT_SOLO ) {
        m_nTopPlayerData = static_cast< int >( kPacket_.m_biTotalData );
        m_strNickName = kPacket_.m_strNickName;
    } else if( kPacket_.m_nTeamType == KOlympicSchedule::OGT_SOLO ) {
        m_biTotalData = kPacket_.m_biTotalData;
    }
    m_vecRegDateRank = kPacket_.m_vecRegDateRank;
    m_vecTotalRank = kPacket_.m_vecTotalRank;
}

void KOlympicManager::SetCurrentTimeIndex( OUT int& nTimeIndex_ )
{
    CTime tmCurrent = CTime::GetCurrentTime();
    nTimeIndex_ = 0;
    int nHour = tmCurrent.GetHour();
    int nMinute = tmCurrent.GetMinute();
    std::vector<KTimeIndexInfo>::iterator vit;
    for( vit = m_vecTimeInfo.begin(); vit != m_vecTimeInfo.end(); ++vit ) {
        if( vit->m_prStartTime.first == (DWORD)nHour && vit->m_prStartTime.second <= (DWORD)nMinute && vit->m_prEndTime.second >= (DWORD)nMinute ) {
            nTimeIndex_ = vit->m_nTimeIndex;
            return;
        }
    }
}

bool KOlympicManager::CheckGradeEnable( IN DWORD& dwGrade_ )
{
    std::vector<KTotalRankInfo>::iterator vit;
    DWORD dwGrade = 0;
    KLocker lock( m_csOlympic );
    vit = std::find_if( m_vecTotalRank.begin(), m_vecTotalRank.end(), boost::bind( &KTotalRankInfo::m_nCountryCode, _1 ) == m_dwCountryID );
    if( vit == m_vecTotalRank.end() ) {
        return false;
    }

    if( !GetPointGrade( vit->m_nGradePoint, dwGrade ) ) {
        START_LOG( cerr, L"해당 포인트의 등급정보 없음" )
            << BUILD_LOG( vit->m_nGradePoint )
            << END_LOG;
        return false;
    }

    if( dwGrade >= dwGrade_ ) {
        return true;
    }
    return false;
}

//=========================================================================================//
KOlympicUserData::KOlympicUserData()
:m_bLoaded(false)
,m_nMyAccumulateData(0)
,m_nRemainTryCount(0)
,m_nCurrentTimeIndex(0)
,m_bPlayEnable(false)
,m_dwGrade(0)
{
}

KOlympicUserData::~KOlympicUserData()
{
}

void KOlympicUserData::SetUserOlympicUserData( IN const time_t& tmTodayDate_, IN const int& nMyAccumulateData_ )
{
    m_bLoaded = true;
    m_tmTodayDate = tmTodayDate_;
    m_nMyAccumulateData = nMyAccumulateData_;
}

void KOlympicUserData::GetUserOlympicUserData( IN const time_t& tmTodayDate_, OUT int& nMyAccumulateData_ )
{
    if( m_tmTodayDate != tmTodayDate_ ) { // 해당 날짜 정보가 틀리면 하루가 지나서 게임변경됨.
        m_nMyAccumulateData = 0;
        m_tmTodayDate = tmTodayDate_;
    }
    nMyAccumulateData_ = m_nMyAccumulateData;
}

bool KOlympicUserData::CheckRemainTryPoint()
{
    if ( m_nRemainTryCount <= 0 ) {
        return false;
    }
    return true;
}

void KOlympicUserData::DecreaseRemainTryCount( IN const int& nAmount_ )
{
    m_nRemainTryCount -= nAmount_;
    m_nRemainTryCount = std::max< int >( m_nRemainTryCount, 0 );
}

void KOlympicUserData::IncreaseMyAccumulateData( IN const time_t& tmTodayDate_, IN const int& nAmount_ )
{
    if( m_tmTodayDate != tmTodayDate_ ) { // 해당 날짜 정보가 틀리면 하루가 지나서 게임변경됨.
        m_nMyAccumulateData = 0;
        m_tmTodayDate = tmTodayDate_;
    }
    m_nMyAccumulateData += nAmount_;
}

void KOlympicUserData::SetTimeIndex( IN int nTimeIndex_, IN bool bPlayEnable_ )
{
    m_nCurrentTimeIndex = nTimeIndex_;
    m_bPlayEnable = bPlayEnable_;
}

bool KOlympicUserData::CheckUserGrade( IN const DWORD& dwGrade_, IN const DWORD& dwMaxGrade_  )
{
    if( m_dwGrade + 1 == dwGrade_ && dwGrade_ <= dwMaxGrade_ ) {
        return true;
    }

    START_LOG( cerr, L"유저의 등급에 받을수 없는 보상." )
        << BUILD_LOG( m_dwGrade ) 
        << BUILD_LOG( dwGrade_ ) 
        << BUILD_LOG( dwMaxGrade_ ) 
        << END_LOG;

    return false;
}

void KOlympicUserData::UpdateUserGrade()
{
    m_dwGrade = m_dwGrade + 1;
}