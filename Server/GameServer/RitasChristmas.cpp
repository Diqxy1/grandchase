#include "GSSimLayer.h"
#include "RitasChristmas.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "UserEvent.h"

struct KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ;

ImplementSingleton( KRitasChristmas );
ImplOstmOperatorW2A( KRitasChristmas );
NiImplementRootRTTI( KRitasChristmas );

KRitasChristmas::KRitasChristmas(void)
: m_nVersion( 1 )
, m_tmBegin( 0 )
, m_tmEnd( 0 )
, m_tmRankUpdateTime( 0 )
, m_dwTickGap( 60 * 1000 )
, m_nCoinPerGame( 1 )
, m_nCoinSupplyInterval( 4 * 60 )
, m_nCoinSupplyCount( 1 )
, m_nCoinMaxQuantity( 5 )
, m_dwCoinDoubleGainItemID( -1 )
, m_dwContinueCoinExchangeItemID( -1 )
, m_nContinueCoinExchangeConsumeCount( 1 )
, m_nContinueCoinExchangeResultCount( 1 )
, m_nBasketEmptyPointLimit( 300 * 10 )
, m_nFeverPointLimit( 10000 )
, m_dwPremiumKeyItemID( -1 )
, m_nPremiumPerFeverObjectCount( 1 )
{
    m_dwLastTick = ::GetTickCount();

    m_mapObjectInfo.clear();
    m_mapStageInfo.clear();

    m_vecSpecialRewardInfo.clear();
    m_vecPremiumRewardInfo.clear();

    m_vecRewardViewListInfo.clear();
}

KRitasChristmas::~KRitasChristmas(void)
{
}

ImplToStringW( KRitasChristmas )
{
    KLocker lock( m_csRitasChristmas );

    return START_TOSTRINGW
        << TOSTRINGW( m_nVersion )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGWt( CTime( m_tmRankUpdateTime ) )
        << TOSTRINGW( m_nCoinPerGame )
        << TOSTRINGW( m_nCoinSupplyInterval )
        << TOSTRINGW( m_nCoinSupplyCount )
        << TOSTRINGW( m_nCoinMaxQuantity )
        << TOSTRINGW( m_dwCoinDoubleGainItemID )
        << TOSTRINGW( m_dwContinueCoinExchangeItemID )
        << TOSTRINGW( m_nContinueCoinExchangeConsumeCount )
        << TOSTRINGW( m_nContinueCoinExchangeResultCount )
        << TOSTRINGW( m_nBasketEmptyPointLimit )
        << TOSTRINGW( m_nFeverPointLimit )
        << TOSTRINGW( m_dwPremiumKeyItemID )
        << TOSTRINGW( m_nPremiumPerFeverObjectCount )
        << TOSTRINGW( m_mapObjectInfo.size() )
        << TOSTRINGW( m_mapStageInfo.size() )
        << TOSTRINGW( m_vecSpecialRewardInfo.size() )
        << TOSTRINGW( m_vecPremiumRewardInfo.size() )
        << TOSTRINGW( m_vecRewardViewListInfo.size() );
}

bool KRitasChristmas::LoadScript()
{
    return _LoadScript(std::string());
}

bool KRitasChristmas::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitRitasChristmas.lua";
    strScript_ = strScriptName;

    int                                         nVersion;
    time_t                                      tmBegin;
    time_t                                      tmEnd;
    time_t                                      tmRankUpdateTime;
    int                                         nCoinPerGame;
    int                                         nCoinSupplyInterval;
    int                                         nCoinSupplyCount;
    int                                         nCoinMaxQuantity;
    GCITEMID                                    dwCoinDoubleGainItemID;
    GCITEMID                                    dwContinueCoinExchangeItemID;
    int                                         nContinueCoinExchangeConsumeCount;
    int                                         nContinueCoinExchangeResultCount;
    KRitasChristmasPlayInfo                     kPlayInfo;
    std::map< int, KRitasChristmasObjectInfo >  mapObjectInfo;
    int                                         nBasketEmptyPointLimit;
    int                                         nFeverPointLimit;
    std::map< int, KRitasChristmasStageInfo >   mapStageInfo;
    std::vector< KRewardInfo >                  vecSpecialRewardInfo;
    std::vector< KRewardInfo >                  vecPremiumRewardInfo;
    GCITEMID                                    dwPremiumKeyItemID;
    int                                         nPremiumPerFeverObjectCount;
    std::vector< GCITEMID >                     vecRewardViewListInfo;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Version", nVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string( "BeginTime" ), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string( "EndTime" ), tmEnd ), return false );
    _JIF( LoadUpdateTime( kLuaMng, std::string( "RankUpdateTime" ), tmRankUpdateTime ), return false );
    _JIF( kLuaMng.GetValue( "COIN_PER_GAME", nCoinPerGame ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "COIN_SUPPLY_INTERVAL", nCoinSupplyInterval ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "COIN_SUPPLY_COUNT", nCoinSupplyCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "COIN_MAX_QUANTITY", nCoinMaxQuantity ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "COIN_DOUBLE_GAIN_ITEMID", dwCoinDoubleGainItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CONTINUE_COIN_EXCHANGE_ITEMID", dwContinueCoinExchangeItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CONTINUE_COIN_EXCHANGE_CONSUME_COUNT", nContinueCoinExchangeConsumeCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CONTINUE_COIN_EXCHANGE_RESULT_COUNT", nContinueCoinExchangeResultCount ) == S_OK, return false );
    _JIF( LoadPlayInfo( kLuaMng, std::string("PlayInfo"), kPlayInfo ), return false );
    _JIF( LoadObjectInfo( kLuaMng, std::string("ObjectInfo"), mapObjectInfo ), return false );
    _JIF( kLuaMng.GetValue( "BASKET_EMPTY_POINT_LIMIT", nBasketEmptyPointLimit ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "FEVER_POINT_LIMIT", nFeverPointLimit ) == S_OK, return false );
    _JIF( LoadStageInfo( kLuaMng, std::string("StageInfo"), mapStageInfo ), return false );
    _JIF( LoadRewardInfo( kLuaMng, std::string("SpecialRewardInfo"), vecSpecialRewardInfo ), return false );
    _JIF( kLuaMng.GetValue( "PREMIUM_KEY_ITEMID", dwPremiumKeyItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PREMIUM_PER_FEVER_OBJECT_COUNT", nPremiumPerFeverObjectCount ) == S_OK, return false );
    _JIF( LoadRewardInfo( kLuaMng, std::string("PremiumRewardInfo"), vecPremiumRewardInfo ), return false );
    _JIF( LoadRewardViewListInfo( kLuaMng, std::string("RewardViewListInfo"), vecRewardViewListInfo ), return false );

    {
        KLocker lock( m_csRitasChristmas );

        m_nVersion = nVersion;
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_tmRankUpdateTime = tmRankUpdateTime;
        m_nCoinPerGame = nCoinPerGame;
        m_nCoinSupplyInterval = nCoinSupplyInterval;
        m_nCoinSupplyCount = nCoinSupplyCount;
        m_nCoinMaxQuantity = nCoinMaxQuantity;
        m_dwCoinDoubleGainItemID = dwCoinDoubleGainItemID;
        m_dwContinueCoinExchangeItemID = dwContinueCoinExchangeItemID;
        m_nContinueCoinExchangeConsumeCount = nContinueCoinExchangeConsumeCount;
        m_nContinueCoinExchangeResultCount = nContinueCoinExchangeResultCount;
        m_kPlayInfo = kPlayInfo;
        m_mapObjectInfo.swap( mapObjectInfo );
        m_nBasketEmptyPointLimit = nBasketEmptyPointLimit;
        m_nFeverPointLimit = nFeverPointLimit;
        m_mapStageInfo.swap( mapStageInfo );
        m_vecSpecialRewardInfo.swap( vecSpecialRewardInfo );
        m_vecPremiumRewardInfo.swap( vecPremiumRewardInfo );
        m_vecRewardViewListInfo.swap( vecRewardViewListInfo );
        _JIF( SetRewardInfos(), return false );
        m_dwPremiumKeyItemID = dwPremiumKeyItemID;
        m_nPremiumPerFeverObjectCount = nPremiumPerFeverObjectCount;
    }
    START_LOG( cerr, L"Rita's Christmas Event script loaded." ) << END_LOG;

    return true;
}

bool KRitasChristmas::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"리타의 크리스마스 이벤트 스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KRitasChristmas::LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    CTimeSpan ts( 1, 0, 0, 0 ); // 내일

    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmUpdate( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), nHour, nMin, 0 );

    if( tmCurrent > tmUpdate )
        tmUpdate += ts;

    tmTime_ = KncUtil::TimeToInt( tmUpdate );

    return true;
}

bool KRitasChristmas::LoadPlayInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KRitasChristmasPlayInfo& kPlayInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "SlotNum", kPlayInfo_.m_nSlotNum ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "BasketSize", kPlayInfo_.m_nBasketSize ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "BasketWidth", kPlayInfo_.m_fBasketWidth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "PlayerSpeed", kPlayInfo_.m_fPlayerSpeed ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CatchZoneTop", kPlayInfo_.m_fCatchZoneTop ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "CatchZoneBottom", kPlayInfo_.m_fCatchZoneBottom ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KRitasChristmas::LoadObjectInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KRitasChristmasObjectInfo >& mapObjectInfo_ )
{
    mapObjectInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KRitasChristmasObjectInfo kObjectInfo;
        _JIF( kLuaMng_.GetValue( "ObjectID", kObjectInfo.m_nObjectID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ObjectType", kObjectInfo.m_nObjectType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DownSpeed", kObjectInfo.m_fDownSpeed ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Point", kObjectInfo.m_nPoint ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DecreaseTime", kObjectInfo.m_fDecreaseTime ) == S_OK, return false );

        if ( false == mapObjectInfo_.insert( std::make_pair( kObjectInfo.m_nObjectID, kObjectInfo ) ).second ) {
            START_LOG( cerr, L"리타의 크리스마스 이벤트 스크립트에 중복된 오브젝트 정보가 있습니다." ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KRitasChristmas::LoadStageInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KRitasChristmasStageInfo >& mapStageInfo_ )
{
    mapStageInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KRitasChristmasStageInfo kStageInfo;
        _JIF( kLuaMng_.GetValue( "StageNum", kStageInfo.m_nStageNum ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "TimeLimit", kStageInfo.m_fTimeLimit ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GoalCount", kStageInfo.m_nGoalCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DownSpeedIncRatio", kStageInfo.m_fDownSpeedIncreaseRatio ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ClearRewardCount", kStageInfo.m_nClearRewardCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ContinueCoinCount", kStageInfo.m_nContinueCoinCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "RemainTimeBonusPointPerSec", kStageInfo.m_nRemainTimeBonusPointPerSec ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GenerateSec", kStageInfo.m_fGenerateSec ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "GenerateNum", kStageInfo.m_nGenerateNum ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "FeverGenerateSec", kStageInfo.m_fFeverGenerateSec ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "FeverGenerateNum", kStageInfo.m_nFeverGenerateNum ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "FeverTimeDuration", kStageInfo.m_fFeverTimeDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ComboLimit", kStageInfo.m_nComboLimit ) == S_OK, return false );

        if ( false == mapStageInfo_.insert( std::make_pair( kStageInfo.m_nStageNum, kStageInfo ) ).second ) {
            START_LOG( cerr, L"리타의 크리스마스 이벤트 스크립트에 중복된 스테이지 정보가 있습니다." ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KRitasChristmas::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KRewardInfo >& vecRewardInfo_ )
{
    vecRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KRewardInfo kRewardInfo;
        _JIF( kLuaMng_.GetValue( 1, kRewardInfo.m_dwItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kRewardInfo.m_nCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kRewardInfo.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kRewardInfo.m_fProb ) == S_OK, return false );

        vecRewardInfo_.push_back( kRewardInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KRitasChristmas::LoadRewardViewListInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN std::vector< GCITEMID >& vecRewardViewListInfo_ )
{
    vecRewardViewListInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while ( true ) {
        GCITEMID dwItemID = 0;
        if ( kLuaMng_.GetValue( i++, dwItemID ) != S_OK ) break;

        vecRewardViewListInfo_.push_back( dwItemID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KRitasChristmas::SetRewardInfos()
{
    // todo: 100%확률 안채워졌을 경우 오류 출력 및 실패하도록 추가하자

    if ( m_vecSpecialRewardInfo.empty() || m_vecPremiumRewardInfo.empty() ) {
        START_LOG( cerr, L"리타의 크리스마스 이벤트 스크립트의 보상 정보가 없습니다." ) << END_LOG;
        return false;
    }

    Kairo kKairoSpecialReward;
    std::vector< KRewardInfo >::iterator vitSpecial;
    for ( vitSpecial = m_vecSpecialRewardInfo.begin(); vitSpecial != m_vecSpecialRewardInfo.end(); ++vitSpecial ) {
        if ( false == kKairoSpecialReward.SetPostItem( vitSpecial->m_dwItemID, vitSpecial->m_nCount, vitSpecial->m_nPeriod, vitSpecial->m_fProb ) ) {
            START_LOG( cerr, L"리타의 크리스마스 이벤트 스크립트의 보상 정보에 오류가 있습니다(SpecialRewardInfo)." ) << END_LOG;
            return false;
        }
    }
    m_kKairoSpecialReward = kKairoSpecialReward;

    Kairo kKairoPremiumReward;
    std::vector< KRewardInfo >::iterator vitPremium;
    for ( vitPremium = m_vecPremiumRewardInfo.begin(); vitPremium != m_vecPremiumRewardInfo.end(); ++vitPremium ) {
        if ( false == kKairoPremiumReward.SetPostItem( vitPremium->m_dwItemID, vitPremium->m_nCount, vitPremium->m_nPeriod, vitPremium->m_fProb ) ) {
            START_LOG( cerr, L"리타의 크리스마스 이벤트 스크립트의 보상 정보에 오류가 있습니다(PremiumRewardInfo)." ) << END_LOG;
            return false;
        }
    }
    m_kKairoPremiumReward = kKairoPremiumReward;

    return true;
}

bool KRitasChristmas::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csRitasChristmas );

    return ( m_tmBegin <= tmCurrent && tmCurrent <= m_tmEnd );
}

bool KRitasChristmas::IsRegisteredRewardInfo()
{
    KLocker lock( m_csRitasChristmas );

    if ( m_kKairoSpecialReward.GetCaseNum() != 0 && m_kKairoPremiumReward.GetCaseNum() != 0 ) {
        return true;
    }

    START_LOG( cerr, L"상자 구성품이 등록되지 않았음" )
        << BUILD_LOG( m_kKairoSpecialReward.GetCaseNum() )
        << BUILD_LOG( m_kKairoPremiumReward.GetCaseNum() )
        << END_LOG;

    return false;
}

GCITEMID KRitasChristmas::GetPremiumKeyItemID()
{
    KLocker lock( m_csRitasChristmas );

    return m_dwPremiumKeyItemID;
}

bool KRitasChristmas::GetRewardKairo( IN int nOpenBoxType_, OUT Kairo& kReward_ )
{
    bool bReturn = false;

    switch ( nOpenBoxType_ ) {
        case KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_SPECIAL:
            kReward_ = m_kKairoSpecialReward;
            bReturn = true;

            break;

        case KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_PREMIUM:
            kReward_ = m_kKairoPremiumReward;
            bReturn = true;

            break;

        default:
            START_LOG( cerr, L"유효하지 않은 상자 타입 요청" ) << END_LOG;
            break;
    }

    return bReturn;
}

bool KRitasChristmas::GetPlayInfo( OUT KRitasChristmasPlayInfo& kPlayInfo_ )
{
    KLocker lock( m_csRitasChristmas );

    kPlayInfo_.m_nSlotNum           = m_kPlayInfo.m_nSlotNum;
    kPlayInfo_.m_nBasketSize        = m_kPlayInfo.m_nBasketSize;
    kPlayInfo_.m_fBasketWidth       = m_kPlayInfo.m_fBasketWidth;
    kPlayInfo_.m_fPlayerSpeed       = m_kPlayInfo.m_fPlayerSpeed;
    kPlayInfo_.m_fCatchZoneTop      = m_kPlayInfo.m_fCatchZoneTop;
    kPlayInfo_.m_fCatchZoneBottom   = m_kPlayInfo.m_fCatchZoneBottom;

    return true;
}

bool KRitasChristmas::GetObjectInfo( OUT std::map< int, KRitasChristmasObjectInfo >& mapObjectInfo_ )
{
    mapObjectInfo_.clear();

    KLocker lock( m_csRitasChristmas );

    mapObjectInfo_ = m_mapObjectInfo;

    return true;
}

bool KRitasChristmas::GetStageInfo( IN int nStageNum_, OUT KRitasChristmasStageInfo& kStageInfo_ )
{
    KLocker lock( m_csRitasChristmas );

    if ( true == m_mapStageInfo.empty() ) {
        START_LOG( cerr, L"스테이지 정보 가져오기 중 스테이지 정보가 비어있음" ) << END_LOG;
        return false;
    }

    std::map< int, KRitasChristmasStageInfo >::iterator mitStageInfo = m_mapStageInfo.find( nStageNum_ );
    if ( mitStageInfo == m_mapStageInfo.end() ) {
        START_LOG( cerr, L"스테이지 정보 가져오기 중 스테이지 정보가 없음" ) << END_LOG;
        return false;
    }

    kStageInfo_ = mitStageInfo->second;

    return true;
}

void KRitasChristmas::GetAllStageInfo( OUT std::map< int, KRitasChristmasStageInfo >& mapStageInfo_ )
{
    mapStageInfo_.clear();

    KLocker lock( m_csRitasChristmas );

    mapStageInfo_ = m_mapStageInfo;
}

bool KRitasChristmas::GetFirstStageNum( OUT int& nFirstStageNum_ )
{
    nFirstStageNum_ = 0;

    KLocker lock( m_csRitasChristmas );

    if ( true == m_mapStageInfo.empty() ) {
        START_LOG( cerr, L"첫 스테이지 번호 가져오기 중 스테이지 정보가 없음" ) << END_LOG;
        return false;
    }

    std::map< int, KRitasChristmasStageInfo >::iterator mitStageInfo = m_mapStageInfo.begin();
    if ( mitStageInfo == m_mapStageInfo.end() ) {
        START_LOG( cerr, L"위에서 empty 검사를 했으니 여기에 들어오지는 않겠지" ) << END_LOG;
        return false;
    }

    nFirstStageNum_ = mitStageInfo->first;

    return true;
}

bool KRitasChristmas::GetNextStageNum( IN int nCurrentStageNum_, OUT int& nNextStageNum_ )
{
    nNextStageNum_ = 0;

    KLocker lock( m_csRitasChristmas );

    if ( true == m_mapStageInfo.empty() ) {
        START_LOG( cerr, L"다음 스테이지 번호 가져오기 중 스테이지 정보가 없음" ) << END_LOG;
        return false;
    }

    std::map< int, KRitasChristmasStageInfo >::iterator mitStageInfo = m_mapStageInfo.find( nCurrentStageNum_ );
    if ( mitStageInfo == m_mapStageInfo.end() ) {
        START_LOG( cerr, L"다음 스테이지 번호 가져오기 중 현재 스테이지 정보가 없음" )
            << BUILD_LOG( nCurrentStageNum_ )
            << END_LOG;
        return false;
    }

    mitStageInfo++;

    if ( mitStageInfo == m_mapStageInfo.end() ) {
        START_LOG( clog, L"현재 스테이지가 마지막 스테이지라서 다음 스테이지 정보가 없음" )
            << BUILD_LOG( nCurrentStageNum_ )
            << END_LOG;
        return false;
    }

    nNextStageNum_ = mitStageInfo->first;

    return true;
}

bool KRitasChristmas::GetContinueCoinConsumeCount( IN int nStageNum_, OUT int& nContinueCoinConsumeCount_ )
{
    nContinueCoinConsumeCount_ = 0;

    KLocker lock( m_csRitasChristmas );

    if ( true == m_mapStageInfo.empty() ) {
        START_LOG( cerr, L"스테이지별 재도전 코인 수 가져오기 중 스테이지 정보가 비어있음" ) << END_LOG;
        return false;
    }

    std::map< int, KRitasChristmasStageInfo >::iterator mitStageInfo = m_mapStageInfo.find( nStageNum_ );
    if ( mitStageInfo == m_mapStageInfo.end() ) {
        START_LOG( cerr, L"스테이지별 재도전 코인 수 가져오기 중 스테이지 정보가 없음" ) << END_LOG;
        return false;
    }

    nContinueCoinConsumeCount_ = mitStageInfo->second.m_nContinueCoinCount;

    return true;
}

int KRitasChristmas::GetPremiumPerFeverObjectCount()
{
    KLocker lock( m_csRitasChristmas );

    // 혹시나 0나누기 크래시 발생할지도 모르니 추가함
    if ( m_nPremiumPerFeverObjectCount <= 0 ) {
        return 1;
    }

    return m_nPremiumPerFeverObjectCount;
}

int KRitasChristmas::GetBasketEmptyPointLimit()
{
    KLocker lock( m_csRitasChristmas );

    return m_nBasketEmptyPointLimit;
}

int KRitasChristmas::GetFeverPointLimit()
{
    KLocker lock( m_csRitasChristmas );

    return m_nFeverPointLimit;
}

int KRitasChristmas::GetVersion()
{
    KLocker lock( m_csRitasChristmas );

    return m_nVersion;
}

GCITEMID KRitasChristmas::GetContinueCoinExchangeItemID()
{
    KLocker lock( m_csRitasChristmas );

    return m_dwContinueCoinExchangeItemID;
}

int KRitasChristmas::GetContinueCoinExchangeConsumeCount()
{
    KLocker lock( m_csRitasChristmas );

    return m_nContinueCoinExchangeConsumeCount;
}

int KRitasChristmas::GetContinueCoinExchangeResultCount()
{
    KLocker lock( m_csRitasChristmas );

    return m_nContinueCoinExchangeResultCount;
}

int KRitasChristmas::GetCoinSupplyInterval()
{
    KLocker lock( m_csRitasChristmas );

    return m_nCoinSupplyInterval;
}

int KRitasChristmas::GetCoinSupplyCount()
{
    KLocker lock( m_csRitasChristmas );

    return m_nCoinSupplyCount;
}

int KRitasChristmas::GetCoinMaxQuantity()
{
    KLocker lock( m_csRitasChristmas );

    return m_nCoinMaxQuantity;
}

GCITEMID KRitasChristmas::GetCoinDoubleGainItemID()
{
    KLocker lock( m_csRitasChristmas );

    return m_dwCoinDoubleGainItemID;
}

int KRitasChristmas::GetCoinPerGame()
{
    KLocker lock( m_csRitasChristmas );

    return m_nCoinPerGame;
}

bool KRitasChristmas::CheckOneMinTick()
{
    // m_dwTickGap은 1분이다
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();

    return true;
}

bool KRitasChristmas::CheckRankUpdateTime( IN time_t& tmDate_ )
{
    KLocker lock( m_csRitasChristmas );

    CTime tmCurrent( tmDate_ );
    CTime tmUpdate( m_tmRankUpdateTime );

    // 업데이트 시간이 되었다면 m_tmRankUpdateTime을 내일로 변경한다
    if ( tmUpdate < tmCurrent ) {
        CTimeSpan ts( 1, 0, 0, 0 );
        tmUpdate += ts;

        m_tmRankUpdateTime = KncUtil::TimeToInt( tmUpdate );

        return true;
    }

    return false;
}

void KRitasChristmas::Tick()
{
    if ( false == CheckOneMinTick() || false == IsEventTerm() ) {
        return;
    }

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    if ( CheckRankUpdateTime( tmCurrent ) ) { // 하루가 지났다면 랭킹 정보 새로 받을 수 있도록 세팅
        // 접속중인 모든 유저의 스레드에 이벤트 큐잉
        SendRankUpdateTimeChangedNot( false );
    }
}

void KRitasChristmas::SendRankUpdateTimeChangedNot( IN bool bReceivedLastRankInfo_ )
{
    // false로 전달하면 랭킹을 새로 받을 수 있게 된다
    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_RITAS_CHRISTMAS_RANK_UPDATE_TIME_CHANGED_NOT, bReceivedLastRankInfo_ );

    START_LOG( cerr, L"리타의 크리스마스 이벤트 랭킹 업데이트 시간 변경 알림 메시지 전달." )
        << BUILD_LOG( bReceivedLastRankInfo_ )
        << END_LOG;
}

bool KRitasChristmas::GetRewardViewListInfo( OUT std::vector< GCITEMID >& vecRewardViewListInfo_ )
{
    KLocker lock( m_csRitasChristmas );

    vecRewardViewListInfo_ = m_vecRewardViewListInfo;

    return true;
}

//===========================================================================================

KRitasChristmasUserData::KRitasChristmasUserData()
: m_nCurrentStageNum( 0 )
, m_nCurrentPoint( 0 )
, m_nStartCoinCount( 0 )
, m_nInitialStartCoinCount( 0 )
, m_tmStartCoinReceived( 0 )
, m_nContinueCoinCount( 0 )
, m_bContinuable( false )
, m_nSpecialRewardCount( 0 )
, m_nInitialPremiumRewardCount( 0 )
, m_nPremiumRewardCount( 0 )
, m_nMaxPoint( 0 )
, m_bReceivedLastRankInfo( false )
{
    m_vecRankInfo.clear();
}

KRitasChristmasUserData::~KRitasChristmasUserData()
{
}

void KRitasChristmasUserData::GetRitasChristmasEventUserData( OUT KRitasChristmasEventUserData& kRitasChristmasEventUserData_ )
{
    kRitasChristmasEventUserData_.m_nInitialStartCoinCount = m_nInitialStartCoinCount;
    kRitasChristmasEventUserData_.m_nStartCoinCount = m_nStartCoinCount;
    kRitasChristmasEventUserData_.m_tmStartCoinReceived = m_tmStartCoinReceived;
    kRitasChristmasEventUserData_.m_nInitialPremiumRewardCount = m_nInitialPremiumRewardCount;
    kRitasChristmasEventUserData_.m_nPremiumRewardCount = m_nPremiumRewardCount;
}