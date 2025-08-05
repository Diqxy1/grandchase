#include "PlantTree.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KPlantTree );
ImplOstmOperatorW2A( KPlantTree );
NiImplementRootRTTI( KPlantTree );


KPlantTree::KPlantTree(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwPlantTreeVersion(0)
,m_dwPlantTreeBoard(0)
,m_dwGoldTreeItemID(0)
,m_dwWaterItemID(0)
,m_dwManureItemID(0)
,m_dwMaxContinent(0)
{
    m_mapPlantTreeInfo.clear();
    m_setTreeStatus.clear();
}

KPlantTree::~KPlantTree(void)
{
}

ImplToStringW( KPlantTree )
{
    KLocker lock( m_csPlantTree );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwPlantTreeVersion )
        << TOSTRINGW( m_dwPlantTreeBoard )
        << TOSTRINGW( m_dwGoldTreeItemID )
        << TOSTRINGW( m_dwWaterItemID )
        << TOSTRINGW( m_dwManureItemID )
        << TOSTRINGW( m_dwManureItemID )
        << TOSTRINGW( m_mapPlantTreeInfo.size() );
}

bool KPlantTree::LoadScript()
{
    return _LoadScript(std::string());
}

bool KPlantTree::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    KLuaManager kLuaMng;
    DWORD       dwPlantTreeVersion; // 나무나무 이벤트 버전.
    DWORD       dwPlantTreeBoard;   // 나무나무 이벤트 말판 ItemID
    GCITEMID       dwGoldTreeItemID;   // 황금나무 ItemID
    GCITEMID       dwWaterItemID;      // 물주기 ItemID
    GCITEMID       dwManureItemID;     // 거름주기 ItemID
    DWORD       dwMaxContinent;     // Max 대륙Num.
    std::map< DWORD, KPlantTreeInfo> mapPlantTreeInfo;
    Kairo                        kKairo;
    std::set< DWORD >            setTreeStatus;

    const std::string strScriptName = "InitPlantTree.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PLANTTREE_VERSION", dwPlantTreeVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PLANTTREE_ITEM", dwPlantTreeBoard ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GOLDTREE_ITEM", dwGoldTreeItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "WATER_ITEM", dwWaterItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MANURE_ITEM", dwManureItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MAX_CONTINENT", dwMaxContinent ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    // 대륙별 나무 말판 정보 로드.
    _JIF( LoadPlantTreeInfo( kLuaMng, std::string("PLANTTREE_INFO"), mapPlantTreeInfo ), return false );

    // 거름 확률 설정.
    _JIF( LoadManureRatio( kLuaMng, std::string("MANURE_RATIO"), kKairo ), return false );

    // 나무상태값 Load.
    _JIF( LoadTreeStatus( kLuaMng, std::string("PLANTTREE_STATUS"), setTreeStatus ), return false );

    START_LOG( cerr, L"나무나무 이벤트 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csPlantTree );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwPlantTreeVersion = dwPlantTreeVersion;
        m_dwPlantTreeBoard = dwPlantTreeBoard;
        m_dwGoldTreeItemID = dwGoldTreeItemID;
        m_dwWaterItemID = dwWaterItemID;
        m_dwManureItemID = dwManureItemID;
        m_dwMaxContinent = dwMaxContinent;
        m_mapPlantTreeInfo.swap( mapPlantTreeInfo );
        m_kairoInfo = kKairo;
        m_setTreeStatus.swap( setTreeStatus );

        return true;
    }
}

bool KPlantTree::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KPlantTree::LoadPlantTreeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KPlantTreeInfo >& mapPlantTreeInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KPlantTreeInfo kPlantTree;

        _JIF( kLuaMng_.GetValue( "CONTINENT_TYPE", kPlantTree.m_dwContinentType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "TOTAL_TREE_COUNT", kPlantTree.m_dwTotalTreeCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "NEED_WATER_COUNT", kPlantTree.m_dwWaterCount ) == S_OK, return false );
        _JIF( LoadTypeReward( kLuaMng_, std::string("REWARD_LIST"), kPlantTree.m_mapRewardList ), return false );

        if ( !mapPlantTreeInfo_.insert( std::make_pair( kPlantTree.m_dwContinentType, kPlantTree ) ).second ) {
            START_LOG( cerr, L"중복된 대륙 정보 있음. SocksID : " << kPlantTree.m_dwContinentType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KPlantTree::LoadTypeReward(IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::map<DWORD,VEC_REWARD>& mapRewardList_ )
{
    mapRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1 ; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwType = 0;
        VEC_REWARD vecItems;
        if ( kLuaMng_.GetValue( "TYPE", dwType ) != S_OK ) break;
        _JIF( LoadRewardList( kLuaMng_, std::string("REWARD"), vecItems ), return false );
        _JIF( mapRewardList_.insert( std::make_pair( dwType, vecItems ) ).second, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KPlantTree::LoadRewardList( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT VEC_REWARD& vecReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( LoadRewardList( kLuaMng_, vecReward_ ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KPlantTree::LoadRewardList( IN KLuaManager& kLuaMng_, OUT VEC_REWARD& vecReward_ )
{
    vecReward_.clear();
    int nIndex = 0;
    while( true ) {
        KDropItemInfo prReward;
        if( kLuaMng_.GetValue( ++nIndex, prReward.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, prReward.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, prReward.m_nPeriod ) == S_OK, return false );
        vecReward_.push_back( prReward );
    }

    return true;
}

bool KPlantTree::LoadManureRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;
        DWORD dwType = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, dwType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fProb ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostType( dwType, fProb ), return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KPlantTree::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csPlantTree );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}


void KPlantTree::GetPlantTreeInfo( OUT std::map< DWORD, KPlantTreeInfo >& mapPlantTreeInfo_ )
{
    mapPlantTreeInfo_.clear();

    KLocker lock( m_csPlantTree );
    mapPlantTreeInfo_ = m_mapPlantTreeInfo;
}

bool KPlantTree::DoGamble( OUT DWORD& dwType_ )
{
    bool bSuccess;
    {
        KLocker lock( m_csPlantTree );
        bSuccess = m_kairoInfo.Do( dwType_ );
    }
    if( bSuccess ) {
        return true;
    }
    return false;
}

bool KPlantTree::IsGoldTreeItem( IN const GCITEMID& dwItemID_ )
{
    KLocker lock( m_csPlantTree );
    return m_dwGoldTreeItemID == dwItemID_;
}

bool KPlantTree::IsWaterItem( IN const GCITEMID& dwItemID_ )
{
    KLocker lock( m_csPlantTree );
    return m_dwWaterItemID == dwItemID_;
}

bool KPlantTree::IsManureItem( IN const GCITEMID& dwItemID_ )
{
    KLocker lock( m_csPlantTree );
    return m_dwManureItemID == dwItemID_;
}

bool KPlantTree::LoadTreeStatus( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<DWORD>& setTreeStatusInfo_ )
{
    setTreeStatusInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    int nData = 0;
    while( true ) {
        if( kLuaMng_.GetValue( i++, nData ) != S_OK ) break;
        setTreeStatusInfo_.insert( nData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KPlantTree::CheckTreeStatus( IN OUT DWORD& dwTreeStatus_ )
{
    KLocker lock( m_csPlantTree );
    std::set< DWORD >::iterator sit;
    sit = m_setTreeStatus.find( dwTreeStatus_ );
    if( sit == m_setTreeStatus.end() ) {
        dwTreeStatus_ = 0;
    }
}

//=========================================================================================//
KPlantTreeUserData::KPlantTreeUserData()
:m_bLoaded(false)
,m_dwCurrentContinent(1)
,m_dwCurrentPosition(1)
,m_dwCurrentTreeStatus(0)
{
}

KPlantTreeUserData::~KPlantTreeUserData()
{

}

void KPlantTreeUserData::SetPlantTreeData( IN KEVENT_PLANT_TREE_INFO_ACK& kData_ )
{
    m_bLoaded = true;
    m_dwCurrentContinent = kData_.m_dwContinentType;
    m_dwCurrentPosition = kData_.m_dwCurrentPosition;
    m_dwCurrentTreeStatus = kData_.m_dwTreeStatus;
}

bool KPlantTreeUserData::ApplyUserAction( IN DWORD& dwActionType_, IN GCITEMID dwItemID_, OUT int& nReturn_ )
{
    // 일반 나무 / 황금나무 심기.
    if( KPlantTreeUserData::AT_PLANT_TREE == dwActionType_ || KPlantTreeUserData::AT_PLANT_GOLD_TREE == dwActionType_ ) {
        if( SetPlantTree( dwActionType_, dwItemID_, nReturn_ ) ) {
            return true;
        }
    }
    else if( KPlantTreeUserData::AT_WATER_THE_TREE == dwActionType_ ) { // 나무에 물주기.
        if( SetWaterThePlant( dwItemID_, nReturn_ ) ) {
            return true;
        }
    }
    else if( KPlantTreeUserData::AT_MANURE_THE_TREE == dwActionType_ ) { // 나무에 거름주기.
        if( SetManureThePlant( dwItemID_, nReturn_ ) ) {
            return true;
        }
    }
    else {
        nReturn_ = ET_UNDEFIND_ACTION_TYPE;
    }
    return false;
}

bool KPlantTreeUserData::SetPlantTree( IN DWORD& dwTreeType_, IN GCITEMID& dwItemID_, OUT int& nReturn_ )
{
    std::map< DWORD, KPlantTreeInfo > mapPlantTreeInfo;
    std::map< DWORD, KPlantTreeInfo >::iterator mit;
    SiKPlantTree()->GetPlantTreeInfo( mapPlantTreeInfo );

    if( KPlantTreeUserData::TS_EMPTY != m_dwCurrentTreeStatus ) { // 나무 심을수 있는 상태인지 체크.
        if( KPlantTreeUserData::TS_DEAD_TREE != m_dwCurrentTreeStatus ) {
            nReturn_ = KPlantTreeUserData::ET_ERROR_PLANT_TREE;
            return false;
        }
    }

    mit = mapPlantTreeInfo.find( m_dwCurrentContinent );
    if( mit == mapPlantTreeInfo.end() ) { // 유저의 대륙 정보가 스크립트에 없음.
        nReturn_ = KPlantTreeUserData::ET_ERROR_CONTINENT_TYPE;
        return false;
    }

    if( m_dwCurrentPosition > mit->second.m_dwTotalTreeCount ) { // 해당 대륙에 나무를 심을수 있는지 체크.
        nReturn_ = KPlantTreeUserData::ET_ERROR_PLANT_TREE;
        return false;
    }

    // 나무 최초 상태 저장.
    if( KPlantTreeUserData::AT_PLANT_GOLD_TREE == dwTreeType_) {
        if( !SiKPlantTree()->IsGoldTreeItem( dwItemID_ ) ) { // 황금나무 아이템인지 체크.
            nReturn_ = KPlantTreeUserData::ET_ERROR_ITEM;
            return false;
        }
        m_dwCurrentTreeStatus = KPlantTreeUserData::TS_GOLD_INIT_TREE;
    } else {
        m_dwCurrentTreeStatus = KPlantTreeUserData::TS_NORMAL_INIT_TREE;
    }

    nReturn_ = KPlantTreeUserData::ET_SUCCESS;
    return true;
}

bool KPlantTreeUserData::SetWaterThePlant( IN GCITEMID& dwItemID_, OUT int& nReturn_ )
{
    std::map< DWORD, KPlantTreeInfo > mapPlantTreeInfo;
    std::map< DWORD, KPlantTreeInfo >::iterator mit;
    SiKPlantTree()->GetPlantTreeInfo( mapPlantTreeInfo );

    if( !SiKPlantTree()->IsWaterItem( dwItemID_ ) ) { // 물주기 아이템인지 체크.
        nReturn_ = KPlantTreeUserData::ET_ERROR_ITEM;
        return false;
    }

    mit = mapPlantTreeInfo.find( m_dwCurrentContinent );
    if( mit == mapPlantTreeInfo.end() ) { // 유저의 대륙 정보가 스크립트에 없음.
        nReturn_ = KPlantTreeUserData::ET_ERROR_CONTINENT_TYPE;
        return false;
    }
    // 유저의 나무 상태가 물을 줄수 있는 상태인지 체크.
    if( m_dwCurrentTreeStatus >= KPlantTreeUserData::TS_NORMAL_FRUIT_WATER_TREE || m_dwCurrentTreeStatus == KPlantTreeUserData::TS_EMPTY ) {
        nReturn_ = KPlantTreeUserData::ET_ERROR_WATER_TREE;
        return false;
    }

    if( m_dwCurrentTreeStatus < KPlantTreeUserData::TS_GOLD_INIT_TREE ) { // 일반 나무 물주기.
        m_dwCurrentTreeStatus += 1; // 현재 나무 상태 변경.
        if( m_dwCurrentTreeStatus >= ( KPlantTreeUserData::TS_NORMAL_INIT_TREE + mit->second.m_dwWaterCount ) ) {
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_NORMAL_FRUIT_WATER_TREE;
        }
    }
    else { // 황금나무 물주기.
        m_dwCurrentTreeStatus += 1; // 현재 나무 상태 변경.
        if( m_dwCurrentTreeStatus >= ( KPlantTreeUserData::TS_GOLD_INIT_TREE + mit->second.m_dwWaterCount ) ) {
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_GOLD_FRUIT_WATER_TREE;
        }
    }
    nReturn_ = KPlantTreeUserData::ET_SUCCESS;
    return true;
}

bool KPlantTreeUserData::SetManureThePlant( IN GCITEMID& dwItemID_, OUT int& nReturn_ )
{
    std::map< DWORD, KPlantTreeInfo > mapPlantTreeInfo;
    std::map< DWORD, KPlantTreeInfo >::iterator mit;
    SiKPlantTree()->GetPlantTreeInfo( mapPlantTreeInfo );

    if( !SiKPlantTree()->IsManureItem( dwItemID_ ) ) { // 물주기 아이템인지 체크.
        nReturn_ = KPlantTreeUserData::ET_ERROR_ITEM;
        return false;
    }

    // 유저의 나무 상태가 거름을 줄 수 있는 상태인지 체크.
    if( m_dwCurrentTreeStatus == KPlantTreeUserData::TS_EMPTY || m_dwCurrentTreeStatus >= KPlantTreeUserData::TS_GOLD_INIT_TREE ) {
        nReturn_ = KPlantTreeUserData::ET_ERROR_MANURE_TREE;
        return false;
    }

    DWORD dwReturnType = 0;
    if( !SiKPlantTree()->DoGamble( dwReturnType ) ) { // 거름 설정된 확률에 의한 리턴값 가져오기.
        nReturn_ = KPlantTreeUserData::ET_ERROR_MANURE_TREE_KAIRO;
        return false;
    }

    // 거름주기 결과에 대한 처리.
    if( dwReturnType == KPlantTreeUserData::MT_TREE_GOLD ) {
        m_dwCurrentTreeStatus = KPlantTreeUserData::TS_GOLD_INIT_TREE;
    }
    else if( dwReturnType == KPlantTreeUserData::MT_TREE_NORMAL ) {
        mit = mapPlantTreeInfo.find( m_dwCurrentContinent );
        if( mit == mapPlantTreeInfo.end() ) { // 유저의 대륙 정보가 스크립트에 없음.
            nReturn_ = KPlantTreeUserData::ET_ERROR_CONTINENT_TYPE;
            return false;
        }
        m_dwCurrentTreeStatus += 1; // 현재 나무 상태 변경.
        if( m_dwCurrentTreeStatus == ( KPlantTreeUserData::TS_NORMAL_INIT_TREE + mit->second.m_dwWaterCount ) ) {
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_NORMAL_FRUIT_WATER_TREE;
        }
        return true;
    }
    else {
        m_dwCurrentTreeStatus = KPlantTreeUserData::TS_DEAD_TREE;
    }
    nReturn_ = KPlantTreeUserData::ET_SUCCESS;
    return true;
}

void KPlantTreeUserData::GetPlantTreeUserInfo( OUT DWORD& dwCurrentContinent_, OUT DWORD& dwCurrentPosition_, OUT DWORD& dwCurrentTreeStatus_ )
{
    dwCurrentContinent_ = m_dwCurrentContinent;
    dwCurrentPosition_ = m_dwCurrentPosition;
    dwCurrentTreeStatus_ = m_dwCurrentTreeStatus;
}

bool KPlantTreeUserData::CheckUserStatus( IN const DWORD& dwRewardType_ )
{
    if( dwRewardType_ == KPlantTreeUserData::RT_GOLD_FRUIT_TYPE ) { // 열매보상.
        if( m_dwCurrentTreeStatus != KPlantTreeUserData::TS_NORMAL_FRUIT_WATER_TREE ) { // 일반나무 열매 맺은 상태거나..,
            if( m_dwCurrentTreeStatus != KPlantTreeUserData::TS_GOLD_FRUIT_WATER_TREE ) {
                START_LOG( cerr, L"열매 보상을 받을수 없다. " << m_dwCurrentTreeStatus ) << END_LOG;
                return false;
            }
        }
    }
    else if( dwRewardType_ == KPlantTreeUserData::RT_CONTINENT_TYPE ) {// 대륙보상.
        if( m_dwCurrentTreeStatus != KPlantTreeUserData::TS_CONTINENT_REWARD_TREE ) {
            START_LOG( cerr, L"대륙 보상을 받을수 없다. " << m_dwCurrentTreeStatus ) << END_LOG;
            return false;
        }
    }
    else {
        START_LOG( cerr, L"잘못된 보상 Type : " << dwRewardType_ ) << END_LOG;
        return false;
    }
    return true;
}

bool KPlantTreeUserData::GetPlantTreeReward( IN const DWORD& dwRewardType_, OUT std::vector<KDropItemInfo>& vecReward_ )
{
    std::map< DWORD, KPlantTreeInfo > mapPlantTreeInfo;
    std::map< DWORD, KPlantTreeInfo >::iterator mit;
    std::map< DWORD, std::vector<KDropItemInfo> >::iterator mitReward;
    SiKPlantTree()->GetPlantTreeInfo( mapPlantTreeInfo );

    mit = mapPlantTreeInfo.find( m_dwCurrentContinent );
    if( mit == mapPlantTreeInfo.end() ) { // 유저의 대륙 정보가 스크립트에 없음.
        return false;
    }

    if( dwRewardType_ == KPlantTreeUserData::RT_GOLD_FRUIT_TYPE ) { // 열매보상.
        if( m_dwCurrentTreeStatus == KPlantTreeUserData::TS_NORMAL_FRUIT_WATER_TREE ) { // 일반 나무 보상.
            mitReward = mit->second.m_mapRewardList.find( KPlantTreeUserData::RT_NORMAL_FRUIT_TYPE );
            if( mitReward == mit->second.m_mapRewardList.end() ) {
                START_LOG( cerr, L"일반나무 열매보상이 없다. RewardSize " << mit->second.m_mapRewardList.size() ) << END_LOG;
                return false;
            }
        } else {
            mitReward = mit->second.m_mapRewardList.find( KPlantTreeUserData::RT_GOLD_FRUIT_TYPE );
            if( mitReward == mit->second.m_mapRewardList.end() ) {
                START_LOG( cerr, L"황금나무 열매보상이 없다. RewardSize " << mit->second.m_mapRewardList.size() ) << END_LOG;
                return false;
            }
        }
    }
    else if( dwRewardType_ == KPlantTreeUserData::RT_CONTINENT_TYPE ) { // 대륙보상.
        mitReward = mit->second.m_mapRewardList.find( KPlantTreeUserData::RT_CONTINENT_TYPE );
        if( mitReward == mit->second.m_mapRewardList.end() ) {
            START_LOG( cerr, L"대륙보상이 없다. RewardSize " << mit->second.m_mapRewardList.size() ) << END_LOG;
            return false;
        }
    }
    else {
        return false;
    }
    vecReward_ = mitReward->second;
    return true;
}

void KPlantTreeUserData::ApplyRewardPosition( IN const DWORD& dwRewardType_ )
{
    std::map< DWORD, KPlantTreeInfo > mapPlantTreeInfo;
    std::map< DWORD, KPlantTreeInfo >::iterator mit;
    SiKPlantTree()->GetPlantTreeInfo( mapPlantTreeInfo );

    if( dwRewardType_ == KPlantTreeUserData::RT_GOLD_FRUIT_TYPE ) { // 열매보상.
        mit = mapPlantTreeInfo.find( m_dwCurrentContinent );
        if( mit == mapPlantTreeInfo.end() ) {
            START_LOG( cerr, L"대륙정보가 없다. 대륙Num :  " << m_dwCurrentContinent ) << END_LOG;
            return;
        }
        if( m_dwCurrentPosition >= mit->second.m_dwTotalTreeCount ) { // 대륙 최종 열매보상일 경우.
            m_dwCurrentPosition += 1; // 클라이언트 프로그레스가 100% 되지 않아서 수정테스트 해보기
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_CONTINENT_REWARD_TREE;
        }
        else {
            m_dwCurrentPosition += 1;
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_EMPTY;
        }
    }
    else if( dwRewardType_ == KPlantTreeUserData::RT_CONTINENT_TYPE ) { // 대륙보상.
        // 최종 대륙 보상일 경우에 대한 처리.
        DWORD dwMaxContinent = SiKPlantTree()->GetMaxContinent();
        if( m_dwCurrentContinent == dwMaxContinent ) { // 초기값으로 세팅.
            m_dwCurrentContinent = 1;
            m_dwCurrentPosition = 1;
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_EMPTY;
        } else {
            m_dwCurrentContinent += 1;
            m_dwCurrentPosition = 1;
            m_dwCurrentTreeStatus = KPlantTreeUserData::TS_EMPTY;
        }
    }
    else {
        return;
    }
}