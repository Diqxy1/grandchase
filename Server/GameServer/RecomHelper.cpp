#include "RecomHelper.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "GameServer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KRecomHelper );
ImplOstmOperatorW2A( KRecomHelper );
NiImplementRootRTTI( KRecomHelper );

KRecomHelper::KRecomHelper(void)
:m_bEventTerm(false)
,m_bLimitEventTerm(false)
,m_kUpdateTimer(60*1000,false)
,m_tmBegin(1)
,m_tmEnd(1)
,m_tmReturn(1)
,m_tmLimitDate(1)
,m_nAdjustPeriod(45)
,m_dwMaxAcceptNum(12)
,m_dwMaxRecommReqNum(50)
,m_nEventID(1)
,m_fExpRatio(0.3f)
,m_fGpRatio(0.3f)
,m_nRecomBonusGap(3)
,m_tmNewBeginDate(1)
,m_nVersion(0)
{
    m_kUpdateTimer.Reset();
}

KRecomHelper::~KRecomHelper(void)
{
}

ImplToStringW( KRecomHelper )
{
    KLocker lock( m_csRecommend );
    START_TOSTRINGW
        << TOSTRINGWb( m_bEventTerm )
        << TOSTRINGWb( m_bLimitEventTerm )
        << TOSTRINGW( m_kUpdateTimer.GetInterval() )
        << TOSTRINGW( m_kUpdateTimer.GetLastTick() )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmReturn) )
        << TOSTRINGWt( CTime(m_tmLimitDate) )
        << TOSTRINGW( m_dwMaxRecommReqNum )
        << TOSTRINGW( m_dwMaxAcceptNum)
        << TOSTRINGW( m_nEventID )
        << TOSTRINGW( m_fExpRatio )
        << TOSTRINGW( m_fGpRatio )
        << TOSTRINGW( m_strConnectMsg );
    DumpRewardList( stm_, std::wstring(L"First"),m_mapFirstAttendance );
    DumpRewardList( stm_, std::wstring(L"Daily"),m_mapDailyAttendance );
    DumpRewardList( stm_, std::wstring(L"Bonus"),m_mapRecommendeeReward );
    DumpRewardList( stm_, std::wstring(L"LevelUp"),m_mapLevelUpReward );
    DumpRewardList( stm_, std::wstring(L"RecommendLevelUp"),m_mapRecommendLevelUpReward );
    return stm_;
}

bool KRecomHelper::LoadScript()
{
    return _LoadScript(std::string());
}

bool KRecomHelper::_LoadScript( OUT std::string& strScript_ )
{
    DWORD   dwUpdateGap = 60000;
    time_t  tmBegin;
    time_t  tmEnd;
    time_t  tmReturn;
    time_t  tmLimitDate;
    time_t  tmNewBeginDate;
    int     nAdjustPeriod;
    std::map< int, VEC_REWARD > mapFirstAttendance;
    std::map< int, VEC_REWARD > mapDailyAttendance;
    std::map< int, VEC_REWARD > mapRecommendeeReward;
    std::map< int, VEC_REWARD > mapLevelUpReward;
    std::map< int, VEC_REWARD > mapRecommendLevelUpReward;

    int nRecomBonusGap = 3;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitRecom.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
 
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadTime( kLuaMng, std::string("ReturnTime"), tmReturn ), return false );
    _JIF( LoadTime( kLuaMng, std::string("NewBeginTime"), tmNewBeginDate ), return false );
    _JIF( kLuaMng.GetValue( "AdjustPeriod", nAdjustPeriod ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "AttendanceUserGap", nRecomBonusGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PartyPlayEventID", m_nEventID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PartyPlayBonusExp", m_fExpRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PartyPlayBonusGp", m_fGpRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Version", m_nVersion ) == S_OK, return false );

    CTime tmTmp(tmEnd);
    tmTmp += CTimeSpan( nAdjustPeriod, 0, 0, 0 );
    tmLimitDate = KncUtil::TimeToInt( tmTmp );

    // 첫 출석 관련..
    _JIF( kLuaMng.BeginTable( "FirstAttendance" ) == S_OK, return false );
    _JIF( LoadRewardList( kLuaMng, std::string("New"), mapFirstAttendance[UT_NEW] ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string("Old"), mapFirstAttendance[UT_OLD] ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string("Recommendee"), mapFirstAttendance[UT_RECOMMENDEE] ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );
    // 매일 출석 관련

    _JIF( kLuaMng.BeginTable( "DailyAttendance" ) == S_OK, return false );
    _JIF( LoadRewardList( kLuaMng, std::string("New"), mapDailyAttendance[UT_NEW] ), return false );
    _JIF( LoadRewardList( kLuaMng, std::string("Old"), mapDailyAttendance[UT_OLD] ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    // 추천자 첫 출석 횟수..
    
    _JIF( kLuaMng.BeginTable( "RecommendReward" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        VEC_REWARD vecRewardList;
        if( !LoadRewardList( kLuaMng, i , vecRewardList ) ) break;
        LIF( mapRecommendeeReward.insert( std::make_pair( nRecomBonusGap * i, vecRewardList ) ).second );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    // 레벨업 했을 경우의 아이템 지급 여부.
    _JIF( LoadLevelUpReward( kLuaMng, std::string("LevelUpReward"), mapLevelUpReward ), return false );

    // 추천 받은 유저 레벨 달성시 지급 아이템.
    _JIF( LoadLevelUpReward( kLuaMng, std::string("RecommendLevelUp"), mapRecommendLevelUpReward ), return false );

    std::string ConnectMsg;
    _JIF( kLuaMng.GetValue( "ConnectMsg", ConnectMsg ) == S_OK, return false );

    std::wstring strMsg = KncUtil::toWideString( ConnectMsg );


    START_LOG( cerr, L"추천 관련 스크립트 로드 완료.." )
        << BUILD_LOG( mapFirstAttendance.size() )
        << BUILD_LOG( mapDailyAttendance.size() )
        << BUILD_LOG( mapRecommendeeReward.size() )
        << BUILD_LOG( mapLevelUpReward.size() )
        << BUILD_LOG( mapRecommendLevelUpReward.size() )
        << BUILD_LOG( KncUtil::toWideString( ConnectMsg ) ) << END_LOG;

    {
        KLocker lock( m_csRecommend );
        m_kUpdateTimer.SetInterval( dwUpdateGap );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_tmReturn = tmReturn;
        m_tmLimitDate = tmLimitDate;
        m_tmNewBeginDate = tmNewBeginDate;
        m_nAdjustPeriod = nAdjustPeriod;
        m_mapFirstAttendance.swap( mapFirstAttendance );
        m_mapDailyAttendance.swap( mapDailyAttendance );
        m_mapRecommendeeReward.swap( mapRecommendeeReward );
        m_nRecomBonusGap = nRecomBonusGap;
        m_mapLevelUpReward.swap( mapLevelUpReward );
        m_mapRecommendLevelUpReward.swap( mapRecommendLevelUpReward );
        m_strConnectMsg = strMsg;
    }

    UpdateEventState();
    return true;
}

bool KRecomHelper::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    nHour = std::max<int>( nHour, 23 );
    nHour = std::min<int>( nHour, 0 );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KRecomHelper::LoadRewardList( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT VEC_REWARD& vecReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( LoadRewardList( kLuaMng_, vecReward_ ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KRecomHelper::LoadRewardList( IN KLuaManager& kLuaMng_, IN const int nIndex_, OUT VEC_REWARD& vecReward_ )
{
    if ( kLuaMng_.BeginTable( nIndex_ ) != S_OK ) return false;
    _JIF( LoadRewardList( kLuaMng_, vecReward_ ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KRecomHelper::LoadRewardList( IN KLuaManager& kLuaMng_, OUT VEC_REWARD& vecReward_ )
{
    vecReward_.clear();
    int nIndex = 0;
    while( true )
    {
        KDropItemInfo prReward;
        if( kLuaMng_.GetValue( ++nIndex, prReward.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, prReward.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, prReward.m_nPeriod ) == S_OK, return false );
        vecReward_.push_back( prReward );
    }

    return true;
}

bool KRecomHelper::IsReturnUser( time_t tmLastLogin_ )
{
    KLocker lock( m_csRecommend );
    bool bResult = (tmLastLogin_ < m_tmReturn);
    START_LOG( clog, L"IsReturnUser : " << bResult )
        << BUILD_LOGtm( CTime(tmLastLogin_) )
        << BUILD_LOGtm( CTime(m_tmBegin) ) << END_LOG;
    return bResult;
}

void KRecomHelper::Tick()
{
    if( IsUpdateTime() )
        UpdateEventState();
}

bool KRecomHelper::IsUpdateTime()
{
    KLocker lock( m_csRecommend );
    return m_kUpdateTimer.CheckTime();
}

void KRecomHelper::UpdateEventState()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock( m_csRecommend );
    m_bEventTerm = ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
    m_bLimitEventTerm = ( m_tmBegin <= tmCurrent && m_tmLimitDate > tmCurrent );

    m_kUpdateTimer.Reset();
}

void KRecomHelper::GetRecommenders( IN const std::vector<KRecommenderInfo>& vecAll_, IN const int nState_, OUT std::vector<DWORD>& vecWaiting_ )
{
    vecWaiting_.clear();
    std::vector<KRecommenderInfo>::const_iterator vit;
    for( vit = vecAll_.begin() ; vit != vecAll_.end() ; ++vit )
    {
        if( vit->m_nState == nState_ )
            vecWaiting_.push_back( vit->m_dwLoginUID );
    }
}

DWORD KRecomHelper::GetRecommendersCount( IN const std::vector<KRecommenderInfo>& vecAll_, IN const int nState_ )
{
    DWORD dwRet = 0;
    std::vector<KRecommenderInfo>::const_iterator vit;
    for( vit = vecAll_.begin() ; vit != vecAll_.end() ; ++vit )
    {
        if( vit->m_nState == nState_ )
            ++dwRet;
    }
    return dwRet;
}

DWORD KRecomHelper::GetMaxRecommReqNum()
{
    KLocker lock( m_csRecommend );
    return m_dwMaxRecommReqNum;
}

DWORD KRecomHelper::GetMaxAcceptNum()
{
    KLocker lock( m_csRecommend );
    return m_dwMaxAcceptNum;
}

int KRecomHelper::GetEventID()
{
    KLocker lock( m_csRecommend );
    return m_nEventID;
}

float KRecomHelper::GetPartyPlayExpBonus()
{
    KLocker lock( m_csRecommend );
    return m_fExpRatio;
}

float KRecomHelper::GetPartyPlayGpBonus()
{
    KLocker lock( m_csRecommend );
    return m_fGpRatio;
}

bool KRecomHelper::GetFirstAttenReward( IN const int nUserType_, OUT std::vector<KDropItemInfo>& vecReward_ )
{
    vecReward_.clear();
    std::map< int, VEC_REWARD >::const_iterator mit;
    KLocker lock( m_csRecommend );
    mit = m_mapFirstAttendance.find( nUserType_ );
    if( mit == m_mapFirstAttendance.end() ) return false;
    vecReward_ = mit->second;
    return true;
}

bool KRecomHelper::GetDailyAttenReward( IN const int nUserType_, OUT std::vector<KDropItemInfo>& vecReward_ )
{

    vecReward_.clear();
    std::map< int, VEC_REWARD >::const_iterator mit;
    KLocker lock( m_csRecommend );
    mit = m_mapDailyAttendance.find( nUserType_ );
    if( mit == m_mapDailyAttendance.end() ) return false;
    vecReward_ = mit->second;
    return true;

}

bool KRecomHelper::CheckRecomBonus( IN const int nBonusDay )
{

    KLocker lock( m_csRecommend );
    return (m_mapRecommendeeReward.find( nBonusDay ) != m_mapRecommendeeReward.end());
}

bool KRecomHelper::GetRecomBonusReward( IN const int nBonusDay_, OUT std::vector<KDropItemInfo>& vecReward_ )
{
    vecReward_.clear();
    std::map< int, VEC_REWARD >::iterator mit;
    KLocker lock( m_csRecommend );
    mit = m_mapRecommendeeReward.find( nBonusDay_ );
    if( mit == m_mapRecommendeeReward.end() ) return false;

    vecReward_ = mit->second;
    return true;
}

void KRecomHelper::GetRecommendeeReward( OUT std::map< int, VEC_REWARD >& mapRewardList_ )
{
    mapRewardList_.clear();
    KLocker lock( m_csRecommend );
    mapRewardList_ = m_mapRecommendeeReward;
}

bool KRecomHelper::IsEventTerm( IN const time_t tmDate_ )
{
    KLocker lock( m_csRecommend );
    return ( m_tmBegin <= tmDate_ && m_tmEnd >= tmDate_ );
}

bool KRecomHelper::IsLimitEventTerm( IN const time_t tmDate_ )
{
    KLocker lock( m_csRecommend );
    return ( m_tmBegin <= tmDate_ && m_tmLimitDate > tmDate_ );
}

bool KRecomHelper::IsNewBeginTerm( IN const time_t tmDate_ )
{
    KLocker lock( m_csRecommend );
    bool bResult = ( m_tmBegin <= tmDate_ );
    START_LOG( clog, L"IsNewBeginTerm : " << bResult )
        << BUILD_LOGtm( CTime(tmDate_) )
        << BUILD_LOGtm( CTime(m_tmBegin) ) << END_LOG;
    return bResult;
}

bool KRecomHelper::IsAdjustTerm( IN const KRecommenderInfo& kMyData_ )
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    if( kMyData_.m_tmBeginDate > tmCurrent || kMyData_.m_tmEndDate <= tmCurrent )
        return false;
    return true;
}

void KRecomHelper::DumpRewardList( IN OUT std::wostream& stm_, IN const std::wstring& strName_,
                                   IN const std::map< int, VEC_REWARD >& mapReward_ ) const
{
    std::map< int, VEC_REWARD >::const_iterator mit;
    VEC_REWARD::const_iterator vit;
    stm_ << L"---- " << strName_ << L" ----" << std::endl;
    for( mit = mapReward_.begin() ; mit != mapReward_.end() ; ++mit )
    {
        stm_ << L"[" << mit->first << L"] : ";
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            stm_ << L"(" << vit->m_ItemID << L"," << vit->m_nDuration << L"," << vit->m_nPeriod << L"), ";
        }
        stm_ << std::endl;
    }    
}

bool KRecomHelper::LoadLevelUpReward(IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<int,VEC_REWARD>& mapReward_ )
{
    mapReward_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nLevel = 0;
        VEC_REWARD vecItems;
        if ( kLuaMng_.GetValue( "Level", nLevel ) != S_OK ) break;
        _JIF( LoadRewardList( kLuaMng_, std::string("RewardItem"), vecItems ), return false );
        _JIF( mapReward_.insert( std::make_pair( nLevel, vecItems ) ).second, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

int KRecomHelper::AddLevelUpReward( IN const int nOldLv_, IN const int nCurrentLv_, IN const int nRewardLv_, IN OUT std::vector<KDropItemInfo>& vecItem_ )
{
    // nRewardLv_ : 내가 마지막으로 받은 보상의 레벨
    int nCount = 0;
    if ( nOldLv_ >= nCurrentLv_ ) return nCount;
    if ( nRewardLv_ >= nCurrentLv_ ) return nCount;

    for ( int i = nCurrentLv_ ; i > nRewardLv_ ; --i )
    {
        std::map<int, VEC_REWARD >::iterator mit;
        mit = m_mapLevelUpReward.find( i );
        if( mit == m_mapLevelUpReward.end() ) continue;

        if( mit->second.empty() ) continue;

        VEC_REWARD::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            KDropItemInfo kItem;
            kItem.m_ItemID = vit->m_ItemID;
            kItem.m_nDuration = vit->m_nDuration;
            kItem.m_nPeriod = vit->m_nPeriod;
            vecItem_.push_back( kItem );
            ++nCount;
        }
    }
    return nCount;
}

void KRecomHelper::GetRecommendLevelUpReward( IN const std::pair<int,int> prRecomInfo_, OUT VEC_REWARD& vecRewardList_ )
{
    // prRecomInfo_.first : 달성레벨
    // prRecomInfo_.second: 받아간 보상레벨
    vecRewardList_.clear();

    if ( prRecomInfo_.second >= prRecomInfo_.first ) {
        START_LOG( clog, L"받아갈 레벨 보상이 없음." )
            << BUILD_LOG( prRecomInfo_.first )
            << BUILD_LOG( prRecomInfo_.second ) << END_LOG;
        return;
    }

    for ( int nLv = prRecomInfo_.first ; nLv > prRecomInfo_.second ; --nLv ) {
        KLocker lock( m_csRecommend );

        std::map< int, VEC_REWARD >::iterator mit;
        mit = m_mapRecommendLevelUpReward.find( nLv );
        if ( mit == m_mapRecommendLevelUpReward.end() ) {
            START_LOG( clog, L"해당 레벨의 보상이 없음. Level : " << nLv ) << END_LOG;
            continue;
        }

        std::copy( mit->second.begin(), mit->second.end(), std::back_inserter( vecRewardList_ ) );

        START_LOG( clog, L"Recom Reward. Level : " << nLv )
            << BUILD_LOG( mit->second.size() )
            << BUILD_LOG( vecRewardList_.size() ) << END_LOG;
    }

    START_LOG( clog, L"Recom Reward size : " << vecRewardList_.size() )
        << BUILD_LOG( prRecomInfo_.first )
        << BUILD_LOG( prRecomInfo_.second ) << END_LOG;
}

bool KRecomHelper::IsRecomLevel( IN const int nOldLv_, IN const int nCurrentLv_ )
{
    if( nOldLv_ >= nCurrentLv_ ) return false;
    // 유저 레벨이 일치하면..,
    return ( m_mapRecommendLevelUpReward.find( nCurrentLv_ ) != m_mapRecommendLevelUpReward.end() );
}

std::wstring KRecomHelper::GetMsg()
{
    KLocker lock( m_csRecommend );
    return m_strConnectMsg;
}
