#include "GSSimLayer.h"
#include "Gawibawibo.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Lottery.h"
#include "NetError.h"
#include "UserPacket.h"
#include "Log4.h"
#include "UserEvent.h"

ImplementSingleton( KGawibawibo );
ImplOstmOperatorW2A( KGawibawibo );
NiImplementRootRTTI( KGawibawibo );

KGawibawibo::KGawibawibo(void)
:m_dwPointTimeGap(5)
,m_dwUseTryPoint(1)
,m_dwUseRetryPoint(1)
,m_dwVersion(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwDefaultRewardStage(0)
,m_dwRetryPointPerPlays(0)
,m_dwTryPointLimitPerDay(0)
,m_dwRetryPositionValue(0)
,m_nRetryState(0)
{
    m_mapWinReward.clear();
    m_mapBossWinLoseRatio.clear();
}

KGawibawibo::~KGawibawibo(void)
{}

ImplToStringW( KGawibawibo )
{
    KLocker lock( m_csGawibawibo );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwVersion );
}

bool KGawibawibo::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGawibawibo::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwVersion;
    DWORD       dwDailyFree;
    DWORD       dwFreeTickGap;
    DWORD       dwRetryPointPerPlays;
    DWORD       dwRetryPosition;

    std::map< std::pair< DWORD, DWORD >, VEC_REWARD > mapWinReward;
    std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > > mapBossWinLoseRatio;
    std::pair< GCITEMID, DWORD > prTryPointItem;
    std::pair< GCITEMID, DWORD > prRetryPointItem;
    std::map< DWORD, DWORD > mapStageFinalBoss;
    std::map< std::pair< DWORD, DWORD >, Kairo > mapKairoReward;
    VEC_REWARD vecLoseReward;
    DWORD dwDefaultRewardStage;
    KLuaManager kLuaMng;
    std::string strScriptName;

    if ( "" == strScript_ ) {
        strScriptName = "InitGawibawibo.lua";
    }
    else {
        strScriptName = strScript_;
    }

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "VERSION", dwVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DAILY_FREE", dwDailyFree ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "FREE_TICK_GAP", dwFreeTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "RETRY_POINT_PER_PLAY", dwRetryPointPerPlays ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "DEFAULT_REWARD_STAGE", dwDefaultRewardStage ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "RETRY_POSITION_VALUE", dwRetryPosition ) == S_OK, return false );
    
    _JIF( LoadItem( kLuaMng, std::string("TryPointItemTrade"), prTryPointItem ), return false );
    _JIF( LoadItem( kLuaMng, std::string("RetryPointItemTrade"), prRetryPointItem ), return false );
    _JIF( LoadReward( kLuaMng, std::string("WinReward"), mapWinReward, mapBossWinLoseRatio, mapStageFinalBoss, mapKairoReward ), return false );
    _JIF( LoadLoseReward( kLuaMng, std::string("LoseReward"), vecLoseReward ), return false );

    KLocker lock( m_csGawibawibo );

    m_tmBegin = tmBegin;
    m_tmEnd = tmEnd;
    m_dwVersion = dwVersion;
    m_dwPointTimeGap = dwFreeTickGap;
    m_dwRetryPointPerPlays = dwRetryPointPerPlays;
    m_mapBossWinLoseRatio.swap( mapBossWinLoseRatio );
    m_mapWinReward.swap( mapWinReward );
    m_dwRetryPositionValue = dwRetryPosition;
    m_prTryPointItem = prTryPointItem;
    m_prRetryPointItem = prRetryPointItem;
    m_mapStageFinalBoss.swap( mapStageFinalBoss );
    m_dwTryPointLimitPerDay = dwDailyFree;
    m_mapKairoReward.swap( mapKairoReward );
    m_vecLoseReward.swap( vecLoseReward );
    m_dwDefaultRewardStage = dwDefaultRewardStage;

    START_LOG( cerr, L"가위바위보 스크립트 로드 완료" )
        << BUILD_LOG( m_mapBossWinLoseRatio.size() )
        << BUILD_LOG( m_mapWinReward.size() )
        << BUILD_LOG( m_vecLoseReward.size() )
        << BUILD_LOG( m_dwDefaultRewardStage ) << END_LOG;

    return true;
}

bool KGawibawibo::LoadReward( IN KLuaManager& kLuaMng_, 
                             IN std::string& strTable_, 
                             OUT std::map< std::pair< DWORD, DWORD >,VEC_REWARD>& mapReward_,
                             OUT std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > >& mapBossWinLoseRatio_, 
                             OUT std::map< DWORD, DWORD >& mapStageFinalBoss_,
                             OUT std::map< std::pair< DWORD, DWORD >, Kairo >& mapKairoReward_ )
{
    
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        DWORD Stage;
        DWORD FinalBoss;
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "STAGE", Stage ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "FINAL_BOSS", FinalBoss ) == S_OK, return false );
        
        _JIF( LoadBoss( kLuaMng_, mapReward_, Stage, mapBossWinLoseRatio_, mapKairoReward_ ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapStageFinalBoss_.insert( std::make_pair( Stage, FinalBoss ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGawibawibo::LoadBoss( IN KLuaManager& kLuaMng_, 
                           OUT std::map< std::pair< DWORD, DWORD >,VEC_REWARD>& mapReward_, 
                           DWORD Stage_,
                           OUT std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > >& mapBossWinLoseRatio_,
                           OUT std::map< std::pair< DWORD, DWORD >, Kairo >& mapKairoReward_ )
{
    VEC_REWARD vecItemInfo;
    vecItemInfo.clear();
    std::pair< DWORD, DWORD > prStageBoss; // stage and boss
    std::pair< DWORD, DWORD > prWinLoseRatio;

    prStageBoss.first = Stage_;

    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break; 

        Kairo kairo;

        _JIF( kLuaMng_.GetValue( "BOSS", prStageBoss.second ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Win", prWinLoseRatio.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Lose", prWinLoseRatio.second ) == S_OK, return false );
        _JIF( kLuaMng_.BeginTable( "ItemList" ) == S_OK, return false );
        _JIF( LoadRewardList( kLuaMng_, vecItemInfo, kairo ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( !mapReward_.insert( std::make_pair( prStageBoss, vecItemInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 보상정보 있음. GOAL : " << prStageBoss.first << L", " << prStageBoss.second ) << END_LOG;
        }
        if ( !mapBossWinLoseRatio_.insert( std::make_pair( prStageBoss, prWinLoseRatio ) ).second ) {
            START_LOG( cerr, L" 중복된 보스 승패 정보 있음. BossID : " << prStageBoss.first << L", " << prStageBoss.second ) << END_LOG;
        }
        if ( !mapKairoReward_.insert( std::make_pair( prStageBoss, kairo ) ).second ) {
            START_LOG( cerr, L" 중복된 보상 정보 있음. BossID : " << prStageBoss.first << L", " << prStageBoss.second ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    

    return true;
}

// 보상 아이템 정보 읽어오기.
bool KGawibawibo::LoadRewardList( IN KLuaManager& kLuaMng_, 
                                 OUT VEC_REWARD& vecReward_,
                                 OUT Kairo& kairo_ )
{
    vecReward_.clear();
    int nOuterIndex = 0;

    while( true )
    {
        if ( kLuaMng_.BeginTable( ++nOuterIndex ) != S_OK ) {
            break;
        }
        KDropItemInfo kReward;
        float fProb = 0.f;
        int nIndex = 0;

        if( kLuaMng_.GetValue( ++nIndex, kReward.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, kReward.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, kReward.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, fProb ) == S_OK, return false ); // 확률

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecReward_.push_back( kReward );

        _JIF( kairo_.SetPostItem( kReward.m_ItemID, kReward.m_nDuration, kReward.m_nPeriod, fProb ), return false );        
    }

    return true;
}

bool KGawibawibo::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csGawibawibo );
    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

bool KGawibawibo::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 5, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, nMin, 0 ) );

    return true;
}

int KGawibawibo::DoGawibawibo( IN DWORD dwStage_, IN DWORD dwBossID_ )
{
    // 파라미터로 넘어온 정보로  승패 % 를 찾고, 
    std::pair< DWORD, DWORD > prBossStage = std::make_pair( dwStage_, dwBossID_ );

    std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > >::iterator mit; 
    mit = m_mapBossWinLoseRatio.find( prBossStage );

    int nResult = -99;

    if ( mit != m_mapBossWinLoseRatio.end() ) {
        KLottery kLottery;
        // 보스,스테이지 별 세팅되어 있는 '승패' 확률 '무승부' 확률을 사용한다.
        kLottery.AddCase( KEVENT_GAWIBAWIBO_ACK::WIN, static_cast<float>( mit->second.first ) );
        kLottery.AddCase( KEVENT_GAWIBAWIBO_ACK::LOSE, static_cast<float>( mit->second.second ) );
        kLottery.AddCase( KEVENT_GAWIBAWIBO_ACK::DRAW,  100.f - ( mit->second.first + mit->second.second ) );

        nResult = kLottery.Decision();

    }
    else {//if ( mit == m_mapBossWinLoseRatio.end() ) {

        std::map< std::pair< DWORD, DWORD >, std::pair< DWORD, DWORD > >::iterator mitDefault; 
        
        std::pair< DWORD, DWORD > prBossStageDefault = std::make_pair( m_dwDefaultRewardStage, dwBossID_ );
        mitDefault = m_mapBossWinLoseRatio.find( prBossStageDefault );

        if ( mitDefault != m_mapBossWinLoseRatio.end() ) {
            KLottery kLottery;
            // 보스,스테이지 별 세팅되어 있는 '승패' 확률 '무승부' 확률을 사용한다.
            kLottery.AddCase( KEVENT_GAWIBAWIBO_ACK::WIN, static_cast<float>( mitDefault->second.first ) );
            kLottery.AddCase( KEVENT_GAWIBAWIBO_ACK::LOSE, static_cast<float>( mitDefault->second.second ) );
            kLottery.AddCase( KEVENT_GAWIBAWIBO_ACK::DRAW,  100.f - ( mitDefault->second.first + mitDefault->second.second ) );

            nResult = kLottery.Decision();
        }
        else {
            START_LOG( cerr, L"디폴트 스테이지 설정이상, 스테이지, 보스 : " << prBossStage.first << L"," << prBossStage.second ) << END_LOG;
            nResult = NetError::ERR_GAWIBAWIBO_02;
        }
    }

    return nResult;
}

bool KGawibawibo::UseTryPoint( OUT DWORD& dwTryPoint_ )
{
    bool bResult = false;
    if ( dwTryPoint_ > 0 ) {
        dwTryPoint_ -= m_dwUseTryPoint;
        bResult = true;
    }
    return bResult;
}

bool KGawibawibo::UseRetryPoint( OUT DWORD& dwRetryPoint_ )
{
    bool bResult = false;
    if ( dwRetryPoint_ > 0 ) {
        dwRetryPoint_ -= m_dwUseRetryPoint;
        bResult = true;
    }
    return bResult;
}


void KGawibawibo::TestPlay( IN DWORD dwStage_, IN DWORD dwBossID_ )
{ // 스테이지, 보스별  가위바위보 결과를 테스트 하는 코드
    int nResult = DoGawibawibo( dwBossID_, dwStage_ );

    START_LOG( cerr, L"Test DoGawibawibo , Result : " << nResult << L", BossID :" << dwBossID_ << L", Stage: " << dwStage_ ) << END_LOG;
}

DWORD KGawibawibo::MarkRetryPosition()
{
    KLocker lock( m_csGawibawibo );
    return m_dwRetryPositionValue;
}

bool KGawibawibo::LoadItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair< GCITEMID, DWORD >& prItem_ )
{   
    GCITEMID ItemID;
    DWORD dwPoint;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, dwPoint ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    std::pair< GCITEMID, DWORD > prPointItem = std::make_pair( ItemID, dwPoint );
    prItem_.swap( prPointItem );
    return true;
}

void KGawibawibo::GetRewardList( OUT std::map< std::pair< DWORD, DWORD >, VEC_REWARD >& mapWinReward_ )
{
    mapWinReward_ = m_mapWinReward;
}

void KGawibawibo::GetBossReward( IN DWORD dwStage_, IN DWORD dwBossID_, OUT VEC_REWARD& vecReward_ )
{
    std::pair< DWORD, DWORD > prStageBoss = std::make_pair( dwStage_, dwBossID_ );
    
    std::map< std::pair< DWORD, DWORD >, VEC_REWARD >::iterator mit;
    mit = m_mapWinReward.find( prStageBoss );
    
    if ( mit != m_mapWinReward.end() ) {
        vecReward_ = mit->second;
    }
}

void KGawibawibo::GetBossKairoReward( IN DWORD dwStage_, IN DWORD dwBossID_, OUT VEC_REWARD& vecReward_ )
{
    vecReward_.clear();
    std::pair< DWORD, DWORD > prStageBoss = std::make_pair( dwStage_, dwBossID_ );

    std::map< std::pair< DWORD, DWORD >, Kairo >::iterator mit;
    mit = m_mapKairoReward.find( prStageBoss );

    if ( mit != m_mapKairoReward.end() ) {
        KDropItemInfo kItem;
        mit->second.Do( kItem );
        vecReward_.push_back( kItem );

        START_LOG( clog, L"가위바위보 보스 카이로 보상 선정" )
            << BUILD_LOG( kItem.m_ItemID )
            << BUILD_LOG( kItem.m_nDuration )
            << BUILD_LOG( kItem.m_nPeriod ) << END_LOG;
    }
    else { // mit == m_mapKairoReward.end()
        std::pair< DWORD, DWORD > prStageBossDefault = std::make_pair( m_dwDefaultRewardStage, dwBossID_ );
        std::map< std::pair< DWORD, DWORD >, Kairo >::iterator mitDefault;

        mitDefault = m_mapKairoReward.find( prStageBossDefault );
        
        if ( mitDefault != m_mapKairoReward.end() ) {
            KDropItemInfo kItem;
            mitDefault->second.Do( kItem );
            vecReward_.push_back( kItem );

            START_LOG( clog, L"가위바위보 보스 카이로 보상 선정[디폴트 스테이지]" )
                << BUILD_LOG( kItem.m_ItemID )
                << BUILD_LOG( kItem.m_nDuration )
                << BUILD_LOG( kItem.m_nPeriod ) << END_LOG;
        }
    }
}


bool KGawibawibo::CheckFinalBoss( IN DWORD dwStage_, IN DWORD dwBossID_ )
{
    std::map<DWORD, DWORD>::const_iterator mit;
    std::map<DWORD, DWORD>::const_iterator mitDefault;
    bool bReturn = false;

    mit = m_mapStageFinalBoss.find( dwStage_ );

    if ( mit != m_mapStageFinalBoss.end() ) {
        
        if ( mit->second == dwBossID_ ) { // 현재 이긴 위치가 파이널 보스이면  스테이지 이동
            bReturn = true;
        }
    }
    else {//if ( mit == m_mapStageFinalBoss.end() ) {
        // 디폴트 스테이지로 검색
        mitDefault = m_mapStageFinalBoss.find( m_dwDefaultRewardStage );
        if ( mitDefault != m_mapStageFinalBoss.end() ) {
            // 현재 이긴 위치가 파이널 보스이면  스테이지 이동...
            if ( mitDefault->second == dwBossID_ ) {
                bReturn = true;
            }
        }
    }
    
    return bReturn;
}

void KGawibawibo::GetNumberOfBoss( IN DWORD dwStage_, OUT USHORT& usNumberOfBoss_ )
{
    std::map<DWORD, DWORD>::const_iterator mit;
    mit = m_mapStageFinalBoss.find( dwStage_ );
    if ( mit != m_mapStageFinalBoss.end() ) {
        usNumberOfBoss_ = static_cast<USHORT>( mit->second + 1 ); // 보스 인덱스 + 1 = 보스의 수   
    }
    else { // 스테이지가 스크립트 설정치를 초과하면 디폴트 스테이지 사용
        std::map<DWORD, DWORD>::const_iterator mitDefault;
        mitDefault = m_mapStageFinalBoss.find( m_dwDefaultRewardStage );

        if ( mitDefault != m_mapStageFinalBoss.end() ) {
            usNumberOfBoss_ = static_cast<USHORT>( mitDefault->second + 1 ); // 보스 인덱스 + 1 = 보스의 수
        }
    }    
}


bool KGawibawibo::UsePoint( IN int nPointType_, OUT KUserGawibawiboInfo& kUserInfo_ )
{
    bool bReturn;
    switch( nPointType_ )
    {
    case KUserGawibawiboInfo::TRY_POINT:
        bReturn = UseTryPoint( kUserInfo_.m_dwTryPoint );
        break;
    case KUserGawibawiboInfo::RETRY_POINT:
        bReturn = UseRetryPoint( kUserInfo_.m_dwRetryPoint );
        break;
    default:
        bReturn = false;

    }
    return bReturn;
}

bool KGawibawibo::CheckTryPoint( IN const KUserGawibawiboInfo& kUserInfo_ )
{
    if ( kUserInfo_.m_dwTryPoint <= 0 ) {
        return false;
    }
    return true;
}

bool KGawibawibo::TradeItem( IN const GCITEMID ItemID_,
                                IN const int nItemNum_,
                                OUT KUserGawibawiboInfo& kUserInfo_ )
{
    KLocker lock( m_csGawibawibo );
    bool bReturn;
    if ( m_prTryPointItem.first == ItemID_ ) { // 도전 아이템인가

        kUserInfo_.m_dwTryPoint += m_prTryPointItem.second * nItemNum_;
        bReturn = true;
    }
    else if ( m_prRetryPointItem.first == ItemID_ ) { // 재도전 아이템인가

        kUserInfo_.m_dwRetryPoint += m_prRetryPointItem.second * nItemNum_;
        bReturn = true;
    }
    else {
        bReturn = false;
    }

    return bReturn;    
}

bool KGawibawibo::LoadLoseReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        KDropItemInfo dropItem;
        int nIndex = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, dropItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, dropItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, dropItem.m_nPeriod ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecReward_.push_back( dropItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;   
}

void KGawibawibo::GetLoseReward( OUT VEC_REWARD& vecReward_ )
{
    vecReward_ = m_vecLoseReward;
}

void KGawibawibo::SetInitGawibawiboPosition( IN OUT DWORD& dwCurrentPosition_ )
{
    dwCurrentPosition_ = 0; // 말 위치 초기화
}

void KGawibawibo::SetRetryPosition( IN OUT DWORD& dwCurrentPosition_ )
{
    dwCurrentPosition_ += MarkRetryPosition();
}

void KGawibawibo::DeleteRetryPosition( IN OUT DWORD& dwCurrentPosition_ )
{
    if ( dwCurrentPosition_ >= MarkRetryPosition() ) {
        dwCurrentPosition_ -= MarkRetryPosition();
    }
}

void KGawibawibo::AddGawibawiboTurnCount( IN OUT DWORD& dwTurnCount_ )
{
    ++dwTurnCount_;
}

void KGawibawibo::MoveGawibawiboCurrentPosition( IN OUT DWORD& dwCurrentPosition_ )
{
    ++dwCurrentPosition_;
}

void KGawibawibo::AddGawibawiboPlayCount( IN OUT DWORD& dwPlayCount_ )
{
    ++dwPlayCount_;
}

void KGawibawibo::AddGawibawiboRetryPoint( IN OUT KUserGawibawiboInfo& kGawibawiboInfo_ )
{
    if ( kGawibawiboInfo_.m_dwPlayCount > 0 && kGawibawiboInfo_.m_dwPlayCount % GetRetryPointPerPlays() == 0 ) {
        ++kGawibawiboInfo_.m_dwRetryPoint;
    }
}

bool KGawibawibo::Get_Gawibawibo_UserData_FromDB( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_GAWIBAWIBO_INIT_INFO_REQ, dwTemp );

    return true;
}
