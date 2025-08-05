#include "GSSimLayer.h"
#include "UserEvent.h"
#include "GachaManager.h"
#include "Lua/KLuaManager.h"
#include <KncUtilLua.h> // boost::bind
#include <boost/foreach.hpp>
#include <dbg/dbg.hpp>
#include "NetError.h"
#include "Log4.h"

ImplementSingleton( KGachaManager );
ImplOstmOperatorW2A( KGachaManager );
NiImplementRootRTTI( KGachaManager );

KGachaManager::KGachaManager(void)
{
    m_mapGachaInfos.clear();
}

KGachaManager::~KGachaManager(void)
{
}

ImplToStringW( KGachaManager )
{
    KLocker lock( m_csGacha );
    return START_TOSTRINGW
        << TOSTRINGW( m_vecUseVersions.size() )
        << TOSTRINGW( m_prDefaultReelPointInfo.first )
        << TOSTRINGW( m_prDefaultReelPointInfo.second )
        << TOSTRINGW( m_mapGachaInfos.size() );
}

bool KGachaManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGachaManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    std::vector< KGachaNewList > vecUseVersions;
    std::pair< float, float > prDefaultReelPointInfo; // 기본 릴 포인트 정보
    std::map< int, KGachaInfo > mapGachaInfos; // < 버전, < 가챠구슬ID, 가차정보 > >

    mapGachaInfos.clear();

    std::string strScriptName;
    if ( "" == strScript_ ) {
        strScriptName = "InitGacha.lua";
    }
    else {
        strScriptName = strScript_;
    }

    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadUseVersions( kLuaMng, vecUseVersions ), return false );

    _JIF( LoadDefaultReelPointInfo( kLuaMng, prDefaultReelPointInfo ), return false );

    _JIF( kLuaMng.BeginTable( "InitGacha" ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;

        KGachaInfo kInfo;

        _JIF( kLuaMng.GetValue( "GachaVersion", kInfo.m_nGachaVer ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "GachaMaxReelPoint", kInfo.m_nGachaReelMaxPoint ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "GachaDecreaseEnable", kInfo.m_bGachaDecreaseEnable ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "DecreaseCount", kInfo.m_nGachaDecreaseCount ) == S_OK, return false );

        _JIF( kLuaMng.GetValue( "ReelPointEnable", kInfo.m_bGachaReelPointEnable ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "EquipmentEnable", kInfo.m_bEquipmentEnable ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "AccessoriesEnable", kInfo.m_bAccessoriesEnable ) == S_OK, return false );

        // 가챠아이템 등급 구하기(릴에서 사용)
        _JIF( LoadGachaItemGrade( kLuaMng, kInfo.m_mapGachaItems ), return false );

        _JIF( kLuaMng.BeginTable( "CharacterItemInfo" ) == S_OK, return false );

        for( int j = 1 ; ; ++j )
        {
            if ( kLuaMng.BeginTable( j ) != S_OK ) break;
            KCharacterItemInfo kCharacterItemInfo;

            _JIF( kLuaMng.GetValue( "GachaItem", kCharacterItemInfo.m_dwGachaItemID ) == S_OK, return false );
            _JIF( kLuaMng.GetValue( "KeyItemID", kCharacterItemInfo.m_dwKeyItemID ) == S_OK, return false );
            _JIF( LoadNeedKeyInfo( kLuaMng, kCharacterItemInfo.m_mapNeedKeyInfo ), return false );

            // 가차 세트 보상 아이템.
            _JIF( kLuaMng.BeginTable( "GachaSetRewardList" ) == S_OK, return false );
            _JIF( LoadGachaTotalSetReward( kLuaMng, kCharacterItemInfo.m_mapGachSetReward, kCharacterItemInfo.m_mapSetRewardList ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            // 가차 일반 보상 정보 로드.
            // 가챠 일반 보상 아이템 리스트 로드.
            _JIF( kLuaMng.BeginTable( "CollectItemList" ) == S_OK, return false );
            _JIF( LoadCollectItemList( kLuaMng, kCharacterItemInfo.m_mapKairoNormal, kCharacterItemInfo.m_mapCollectionList ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            // 가차 꽝 보상  로드..
            _JIF( kLuaMng.BeginTable( "FailItemList" ) == S_OK, return false );
            _JIF( LoadGachaRatio( kLuaMng, kCharacterItemInfo.m_mapKairoFail ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            _JIF( kLuaMng.EndTable() == S_OK, return false ); // j

            if ( false == kInfo.m_mapCharacterItemInfo.insert( std::make_pair( kCharacterItemInfo.m_dwGachaItemID, kCharacterItemInfo ) ).second ) {
                START_LOG( cerr, L" 중복된 가차 정보 있음. GachaItemID : " << kCharacterItemInfo.m_dwGachaItemID ) << END_LOG;
                return false;
            }
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false ); // CharacterItemInfo

        if ( false == mapGachaInfos.insert( std::make_pair( kInfo.m_nGachaVer, kInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 가차 정보 있음. GachaVersionID : " << kInfo.m_nGachaVer ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false ); // i
    }

    _JIF( kLuaMng.EndTable() == S_OK, return false ); // InitGacha

    // 현재 사용중인 버전의 정보만 유지한다
    std::map< int, KGachaInfo > mapGachaInfosFinal;
    typedef std::vector< KGachaNewList > vecUseVersionsType;
    BOOST_FOREACH(vecUseVersionsType::value_type & rUseVersion, vecUseVersions)
    {
        std::map< int, KGachaInfo >::iterator mitGachaInfos;
        mitGachaInfos = mapGachaInfos.find(rUseVersion.m_nID);

        if (mitGachaInfos != mapGachaInfos.end())
        {
            mapGachaInfosFinal[mitGachaInfos->first] = mitGachaInfos->second;
        }
    }

    START_LOG( cerr, L" 가차 정보 읽기 성공")
        << BUILD_LOG( vecUseVersions.size() )
        << BUILD_LOG( mapGachaInfos.size() )
        << BUILD_LOG( mapGachaInfosFinal.size() )
        << END_LOG;

    KLocker lock( m_csGacha );
    m_vecUseVersions.swap( vecUseVersions );
    m_prDefaultReelPointInfo = prDefaultReelPointInfo;
    m_mapGachaInfos.swap( mapGachaInfosFinal );

    return true;
}

bool KGachaManager::LoadCollectItemList( IN KLuaManager& kLuaMng_, OUT std::map< int, Kairo >& mapKairoNormal_, OUT std::map< int, std::set<GCITEMID> >& mapCollectionList_ )
{
    mapKairoNormal_.clear();
    mapCollectionList_.clear();

    for( int i = 1; ; ++i ) {
        DWORD dwCollectType;
        Kairo kKairo;
        std::set<GCITEMID> setCollectionList;
        setCollectionList.clear();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "CollectItemType", dwCollectType ) == S_OK, return false );
        _JIF( LoadGachaRatio( kLuaMng_, std::string("CollectItemList"), kKairo ), return false );

        // 일반 보상 아이템 정보 Load.
        if( !mapKairoNormal_.insert( std::make_pair( dwCollectType, kKairo ) ).second ) {
            START_LOG( cerr, L" 중복된 CollectType정보가 일반 보상 아이템 정보 있음. CollectType : " << dwCollectType ) << END_LOG;
        }

        // 일본 보상 아이템 리스트 정보 Load.
        kKairo.GetItemList( setCollectionList );
        if( !mapCollectionList_.insert( std::make_pair( dwCollectType, setCollectionList ) ).second ) {
            START_LOG( cerr, L" 중복된 CollectType정보가 일반 보상 아이템 리스트에 있음. CollectType : " << dwCollectType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    return true;
}

bool KGachaManager::LoadGachaRatio( IN KLuaManager& kLuaMng_, OUT std::map< int, Kairo >& mapKairoFail_ )
{
    mapKairoFail_.clear();

    for( int i = 1; ; ++i ) {
        int nItemLevel;
        Kairo kKairo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "ItemLevel", nItemLevel ) == S_OK, return false );
        _JIF( LoadGachaRatio( kLuaMng_, std::string("LevelRatioReward"), kKairo ), return false );

        // 꽝 아이템 아이템 정보 Load.
        if( !mapKairoFail_.insert( std::make_pair( nItemLevel, kKairo ) ).second ) {
            START_LOG( cerr, L" 중복된 아이템 정보가 꽝 아이템 보상 정보 있음. ItemLevel : " << nItemLevel ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    return true;
}

bool KGachaManager::LoadGachaRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        GCITEMID itemID = 0;
        int nDuration = 0;
        int nPeriod = 0;
        float fProb = 0.f;
        bool bDisplay = false;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostItem( itemID, nDuration, nPeriod, fProb ), return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGachaManager::LoadGachaTotalSetReward( IN KLuaManager& kLuaMng_, OUT std::map< int, std::vector<KDropItemInfo> >& mapGachSetReward_, OUT std::map< int, std::vector<GCITEMID> >& mapSetRewardList_ )
{
    mapGachSetReward_.clear();
    mapSetRewardList_.clear();

    for( int i = 1; ; ++i ) {
        DWORD dwRewardType;
        std::vector<KDropItemInfo> vecGachSetReward;
        std::vector<GCITEMID> vecRewardList;
        std::vector<KDropItemInfo>::iterator vit;
        vecGachSetReward.clear();
        vecRewardList.clear();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "SetRewardType", dwRewardType ) == S_OK, return false );
        _JIF( LoadGachSetRewardList( kLuaMng_, std::string("RewardList"), vecGachSetReward ), return false );

        if( !mapGachSetReward_.insert( std::make_pair( dwRewardType, vecGachSetReward ) ).second ) {
            START_LOG( cerr, L" 중복된 RewardType정보가 있음. RewardType : " << dwRewardType ) << END_LOG;
        }

        for( vit = vecGachSetReward.begin(); vit != vecGachSetReward.end(); ++vit ) {
            vecRewardList.push_back( vit->m_ItemID );
        }
        if( !mapSetRewardList_.insert( std::make_pair( dwRewardType, vecRewardList ) ).second ) {
            START_LOG( cerr, L" 중복된 RewardType정보가 SetList에 있음. RewardType : " << dwRewardType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    return true;
}

bool KGachaManager::LoadGachSetRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KDropItemInfo>& vecGachSetReward_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KDropItemInfo kItem;
        _JIF( kLuaMng_.GetValue( 1, kItem.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kItem.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kItem.m_nPeriod ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecGachSetReward_.push_back( kItem );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGachaManager::LoadUserReelPoint( IN KLuaManager& kLuaMng_, OUT std::map<USHORT,bool>& mapConfig_ )
{
    _JIF( kLuaMng_.BeginTable( "UseReelPoint" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        int nGrade = GG_FREE;
        bool bReelEnable = false;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, bReelEnable ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapConfig_[nGrade] = bReelEnable;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGachaManager::LoadGachaItemGrade( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::pair< float, float > >& mapItems_ )
{
    mapItems_.clear();

    _JIF( kLuaMng_.BeginTable( "RealPointInfo" ) == S_OK, return false );
    _JIF( LoadGachaItems( kLuaMng_, mapItems_ ), return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGachaManager::LoadGachaItems( IN KLuaManager& kLuaMng_, IN OUT std::map< DWORD, std::pair< float, float > >& mapItems_ )
{
    mapItems_.clear();

    for( int i = 1 ; ; ++i )
    {
        GCITEMID nItemID = 0;
        float fGrade = 0.f;
        float fDiv = 0.f;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( 1, nItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, fGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, fDiv) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapItems_[ static_cast< DWORD >( nItemID ) ] = std::make_pair( fGrade, fDiv );
    }

    return true;
}

void KGachaManager::GetRewardItemList( IN const int nVersion_, IN const GCITEMID dwItemID_, OUT std::map< int, std::set< GCITEMID > >& mapCollectionList_ )
{
    mapCollectionList_.clear();
    
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos != m_mapGachaInfos.end() ) {
        std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

        mitCharacterItemInfo = mapCharacterItemInfo.find( dwItemID_ );
        if ( mitCharacterItemInfo != mapCharacterItemInfo.end() ) {
            mapCollectionList_ = mitCharacterItemInfo->second.m_mapCollectionList; // 가챠 Collect List.
        }
    }
}

bool KGachaManager::DoGamble( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_, OUT KDropItemInfo& kItem_, OUT bool& bRewardType_ )
{
    bool bSuccess = false;

    {
        std::map< int, KGachaInfo >::iterator mitGachaInfos;
        std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;
        std::map< int, Kairo >::iterator mitKairo;
        bRewardType_ = false;

        KLocker lock( m_csGacha );

        mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
        _JIF( mitGachaInfos != m_mapGachaInfos.end(), SET_ERROR( ERR_GACHA_00 ); return false );

        std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

        mitCharacterItemInfo = mapCharacterItemInfo.find( dwItemID_ );
        _JIF( mitCharacterItemInfo != mapCharacterItemInfo.end(), SET_ERROR( ERR_GACHA_00 ); return false );

        mitKairo = mitCharacterItemInfo->second.m_mapKairoNormal.find( dwType_ );
        _JIF( mitKairo != mitCharacterItemInfo->second.m_mapKairoNormal.end(), SET_ERROR( ERR_GACHA_00 ); return false );

        bSuccess = mitKairo->second.Do( kItem_ );
    }

    if ( !bSuccess ) {
        SET_ERROR( ERR_GACHA_02 );
        return false;
    }

    if ( kItem_.m_ItemID == 0 ) {
        bSuccess = DoGambleFail( nVersion_, dwItemID_, nLevel_, kItem_ );
        if ( bSuccess ) return true;

        SET_ERROR( ERR_GACHA_01 );
        return false;
    }

    bRewardType_ = true;

    return true;
}

bool KGachaManager::DoGambleFail( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const int nLevel_, OUT KDropItemInfo& kItem_ )
{
    bool bSuccess = false;

    {
        std::map< int, KGachaInfo >::iterator mitGachaInfos;
        std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;
        std::map< int, Kairo >::iterator mitKairo;

        KLocker lock( m_csGacha );

        mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
        _JIF( mitGachaInfos != m_mapGachaInfos.end(), SET_ERROR( ERR_GACHA_00 ); return false );

        std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

        mitCharacterItemInfo = mapCharacterItemInfo.find( dwItemID_ );
        _JIF( mitCharacterItemInfo != mapCharacterItemInfo.end(), SET_ERROR( ERR_GACHA_00 ); return false );

        mitKairo = mitCharacterItemInfo->second.m_mapKairoFail.find( nLevel_ );
        _JIF( mitKairo != mitCharacterItemInfo->second.m_mapKairoFail.end(), SET_ERROR( ERR_GACHA_00 ); return false );

        bSuccess = mitKairo->second.Do( kItem_ );
    }

    if ( !bSuccess ) {
        SET_ERROR( ERR_GACHA_02 );
        return false;
    }

    if ( kItem_.m_ItemID == 0 ) {
        SET_ERROR( ERR_GACHA_01 );
        return false;
    }

    return true;
}

GCITEMID KGachaManager::GetKeyItemID( IN const int nVersion_, IN const GCITEMID dwGachaItemID_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos != m_mapGachaInfos.end() ) {
        std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

        mitCharacterItemInfo = mapCharacterItemInfo.find( dwGachaItemID_ );
        if ( mitCharacterItemInfo == mapCharacterItemInfo.end() ) {
            return 0;
        }

        return mitCharacterItemInfo->second.m_dwKeyItemID;    
    }

    return 0;
}

int KGachaManager::GetNeedKeyCount( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwLevel_, IN const bool bClear_ )
{
    int nNeedKeyCount = INT_MAX;
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        return nNeedKeyCount;
    }

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    mitCharacterItemInfo = mapCharacterItemInfo.find( dwGachaItemID_ );
    if ( mitCharacterItemInfo == mapCharacterItemInfo.end() ) {
        return nNeedKeyCount;
    }

    // std::map< ItemLV, NeedKeyCount >
    std::map< int, int >::iterator mitNeedCount;
    mitNeedCount = mitCharacterItemInfo->second.m_mapNeedKeyInfo.find( dwLevel_ );
    if ( mitNeedCount == mitCharacterItemInfo->second.m_mapNeedKeyInfo.end() ) {
        return nNeedKeyCount;
    }

    nNeedKeyCount = mitNeedCount->second;

    if ( bClear_ && mitGachaInfos->second.m_bGachaDecreaseEnable ) {
        int nCount = mitGachaInfos->second.m_nGachaDecreaseCount;
        nNeedKeyCount = std::max< int >( 1, nNeedKeyCount - nCount );
    }

    return nNeedKeyCount;
}

bool KGachaManager::IsGachaItem( IN const int nVersion_, IN const GCITEMID itemID_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos != m_mapGachaInfos.end() ) {
        std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

        return mapCharacterItemInfo.find( itemID_ ) != mapCharacterItemInfo.end();
    }

    return false;
}

void KGachaManager::DumpNeedKey()
{
    //std::wstringstream stm;
    //KLocker lock( m_csGacha );
    //stm << L"-- Need Key List --" << dbg::endl;
    //std::map<GCITEMID,KGachaInfo>::iterator mit;
    //for( mit = m_mapGachaInfos.begin() ; mit != m_mapGachaInfos.end() ; ++mit )
    //{
    //    stm << L"["<< mit->first << L"," << mit->second.m_dwKeyItemID << L"], ";
    //}
    //std::cout2 << stm.str();
    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KGachaManager::DumpGacha()
{
    //std::wstringstream stm;
    //KLocker lock( m_csGacha );
    //stm << L"-- Gacha List --" << std::endl;
    //std::map<GCITEMID,KGachaInfo>::iterator mit;
    //std::map< int, Kairo >::iterator mitKairo;
    //for( mit = m_mapGachaInfos.begin() ; mit != m_mapGachaInfos.end() ; ++mit ) {
    //    for( mitKairo = mit->second.m_mapKairoNormal.begin(); mitKairo != mit->second.m_mapKairoNormal.end(); ++mitKairo ) {
    //        stm << L"["<< mit->first << L"," << mitKairo->second.GetItemNum() << L"], ";
    //    }
    //}
    //stm << std::endl;
    //std::cout2 << stm.str();
    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KGachaManager::Print( IN const GCITEMID dwGachaItemID_ )
{
    //std::wstringstream stm;
    //KLocker lock( m_csGacha );
    //std::map<GCITEMID,KGachaInfo>::iterator mit;
    //std::map< int, Kairo >::iterator mitKairo;
    //mit = m_mapGachaInfos.find( dwGachaItemID_ );
    //if( mit == m_mapGachaInfos.end() ) {
    //    stm << L"Can't Find : " << dwGachaItemID_;
    //    return;
    //}

    //stm << L" Item ID : " << mit->first << std::endl;
    //for( mitKairo = mit->second.m_mapKairoNormal.begin(); mitKairo != mit->second.m_mapKairoNormal.end(); ++mitKairo ) {
    //    mitKairo->second.ToString(stm) ;
    //}
    //std::cout2 << stm.str();
    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

bool KGachaManager::GetGachaSetReward( IN const int nVersion_, IN const GCITEMID gachaItemID_, OUT std::vector< KDropItemInfo >& vecRewardItems_, IN const DWORD dwType_, IN const GCITEMID ItemID_ )
{
    vecRewardItems_.clear();

    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;
    std::map< int, std::vector< KDropItemInfo > >::iterator mitReward;
    std::vector< KDropItemInfo >::iterator vit;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    _JIF( mitGachaInfos != m_mapGachaInfos.end(), return false );

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    mitCharacterItemInfo = mapCharacterItemInfo.find( gachaItemID_ );
    _JIF( mitCharacterItemInfo != mapCharacterItemInfo.end(), return false );

    mitReward = mitCharacterItemInfo->second.m_mapGachSetReward.find( dwType_ );
    if ( mitReward == mitCharacterItemInfo->second.m_mapGachSetReward.end() ) {
        return false;
    }

    vit = std::find_if( mitReward->second.begin(), mitReward->second.end(), boost::bind( &KDropItemInfo::m_ItemID, _1 ) == ItemID_ );
    if ( vit == mitReward->second.end() ) {
        return false;
    }

    vecRewardItems_.push_back( *vit );

    return true;
}

bool KGachaManager::GetGachaSetRewardList( IN const int nVersion_, IN const GCITEMID gachaItemID_, OUT std::map< int, std::vector< GCITEMID > >& mapGachSetReward_ )
{
    mapGachSetReward_.clear();

    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    _JIF( mitGachaInfos != m_mapGachaInfos.end(), return false );

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    std::map< GCITEMID, KGachaInfo >::iterator mit;
    mitCharacterItemInfo = mapCharacterItemInfo.find( gachaItemID_ );
    _JIF( mitCharacterItemInfo != mapCharacterItemInfo.end(), return false );
    mapGachSetReward_ = mitCharacterItemInfo->second.m_mapSetRewardList;

    return true;
}

bool KGachaManager::IsCollectionItem( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const GCITEMID nItemID_, IN const DWORD dwType_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;
    std::map< int, std::set< GCITEMID > >::iterator mitList;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    _JIF( mitGachaInfos != m_mapGachaInfos.end(), return false );

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    mitCharacterItemInfo = mapCharacterItemInfo.find( dwGachaItemID_ );
    _JIF( mitCharacterItemInfo != mapCharacterItemInfo.end(), return false );
    mitList = mitCharacterItemInfo->second.m_mapCollectionList.find( dwType_ );
    if ( mitList == mitCharacterItemInfo->second.m_mapCollectionList.end() ) {
        return false;
    }

    return ( mitList->second.find( nItemID_ ) != mitList->second.end() );
}

void KGachaManager::GetGachaInfo( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, OUT float& fGrade_, OUT float& fDiv_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< DWORD, std::pair< float, float > >::iterator mitGachaItems;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        START_LOG( cerr, L"해당 가챠아이템의 버전 정보가 없음. Version : " << nVersion_ ) << END_LOG;
        fGrade_ = std::max< float >( 1, m_prDefaultReelPointInfo.first );
        fDiv_ = std::max< float >( 1, m_prDefaultReelPointInfo.second );
        return;
    }

    std::map< DWORD, std::pair< float, float > >& mapGachaItems = mitGachaInfos->second.m_mapGachaItems;

    mitGachaItems = mapGachaItems.find( dwGachaItemID_ );
    if ( mitGachaItems == mapGachaItems.end() )
    {
        START_LOG( cerr, L"해당 가챠아이템의 등급 정보가 없음. ItemID : " << dwGachaItemID_ ) << END_LOG;
        fGrade_ = std::max< float >( 1, m_prDefaultReelPointInfo.first );
        fDiv_ = std::max< float >( 1, m_prDefaultReelPointInfo.second );
        return;
    }

    fGrade_ = std::max< float >( 1, mitGachaItems->second.first );
    fDiv_ = std::max< float >( 1, mitGachaItems->second.second );
}

bool KGachaManager::GetGachaCollectItem( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const GCITEMID dwItemID_, OUT KDropItemInfo& kReward_, IN const DWORD dwType_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;
    std::vector< KDropItemInfo >::iterator vit;
    std::vector< KDropItemInfo > vecCollectList;
    std::map< int, Kairo >::iterator mitKairo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        START_LOG( cerr, L"해당 가챠 버전정보가 없음. Version : " << nVersion_) << END_LOG;
        return false;
    }

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    // 가챠정보 찾기
    mitCharacterItemInfo = mapCharacterItemInfo.find( dwGachaItemID_ );
    if ( mitCharacterItemInfo == mapCharacterItemInfo.end() ) {
        START_LOG( cerr, L"해당 가챠정보가 없음. Version : " << nVersion_ << ", 가챠ItemID : " << dwGachaItemID_ ) << END_LOG;
        return false;
    }

    // 리스트에서 아이템 정보 찾기
    mitKairo = mitCharacterItemInfo->second.m_mapKairoNormal.find( dwType_ );
    if ( mitKairo == mitCharacterItemInfo->second.m_mapKairoNormal.end() ) {
        return false;
    }

    mitKairo->second.GetItemList( vecCollectList );
    vit = std::find_if( vecCollectList.begin(), vecCollectList.end(),
        boost::bind( &KDropItemInfo::m_ItemID, _1 ) == dwItemID_ );

    if ( vit == vecCollectList.end() ) {
        START_LOG( cerr, L"가챠보상정보에 해당 아이템이 없음. Version : " << nVersion_ << ", 가챠ItemID : " << dwGachaItemID_ << L", ItemID : " <<  dwItemID_ ) << END_LOG;
        return false;
    }

    kReward_ = *vit;
    START_LOG( clog, L"Version : " << nVersion_ << ", GachaID : " << dwGachaItemID_ << L", ItemID : " <<  dwItemID_ ) << END_LOG;

    return true;
}

bool KGachaManager::IsUseReelPoint( IN const int nVersion_, IN const GCITEMID dwGachaItemID_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< DWORD, std::pair< float, float > >::iterator mit;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        return false;
    }

    std::map< DWORD, std::pair< float, float > >& mapGachaItems = mitGachaInfos->second.m_mapGachaItems;

    mit = mapGachaItems.find( dwGachaItemID_ );
    if ( mit == mapGachaItems.end() ) {
        return false;
    }

    return true;
}

bool KGachaManager::LoadNeedKeyInfo( IN KLuaManager& kLuaMng_, OUT std::map<int, int>& mapNeedKeyInfo_ )
{
    mapNeedKeyInfo_.clear();

    _JIF( kLuaMng_.BeginTable( "NeedKeyItemCount" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        int nLevel = 0;
        int nCount = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nCount ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapNeedKeyInfo_[nLevel] = nCount;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KGachaManager::IsGachaTypeEnable( IN const int nVersion_, IN const DWORD dwType_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        return false;
    }

    bool& bEquipmentEnable = mitGachaInfos->second.m_bEquipmentEnable;
    bool& bAccessoriesEnable = mitGachaInfos->second.m_bAccessoriesEnable;

    bool bReturn = false;
    switch ( dwType_ ) {
        case KGachaManager::GC_EQUIPMENT:
            bReturn = bEquipmentEnable;

            break;
        case KGachaManager::GC_ACCESSORISE:
            bReturn = bAccessoriesEnable;

            break;
        default:
            bReturn = false;

            break;
    }

    return bReturn;
}

bool KGachaManager::CheckGachaRewardLevel( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwLevel_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;
    std::map< int, int >::iterator mitNeedCount;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        return false;
    }

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    mitCharacterItemInfo = mapCharacterItemInfo.find( dwGachaItemID_ );
    if ( mitCharacterItemInfo == mapCharacterItemInfo.end() ) {
        return false;
    }

    // std::map< ItemLV, NeedKeyCount >
    mitNeedCount = mitCharacterItemInfo->second.m_mapNeedKeyInfo.find( dwLevel_ );
    if ( mitNeedCount == mitCharacterItemInfo->second.m_mapNeedKeyInfo.end() ) {
        return false;
    }

    return true;
}

bool KGachaManager::GetGachaNeedKeyInfo( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const bool bClear_, OUT std::map< int,int >& mapNeedKeyInfo_ )
{
    mapNeedKeyInfo_.clear();

    std::map< int, KGachaInfo >::iterator mitGachaInfos;
    std::map< GCITEMID, KCharacterItemInfo >::iterator mitCharacterItemInfo;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        return false;
    }

    std::map< GCITEMID, KCharacterItemInfo >& mapCharacterItemInfo = mitGachaInfos->second.m_mapCharacterItemInfo;

    mitCharacterItemInfo = mapCharacterItemInfo.find( dwGachaItemID_ );
    if ( mitCharacterItemInfo == mapCharacterItemInfo.end() ) {
        return false;
    }

    mapNeedKeyInfo_ = mitCharacterItemInfo->second.m_mapNeedKeyInfo;
    DecreaseNeedKeyByClear( bClear_, mitGachaInfos->second.m_bGachaDecreaseEnable, mitGachaInfos->second.m_nGachaDecreaseCount, mapNeedKeyInfo_ );

    return true;
}

void KGachaManager::DecreaseNeedKeyByClear( IN const bool bClear_, IN const bool bGachaDecreaseEnable_, IN const int nGachaDecreaseCount_, IN OUT std::map< int, int >& mapNeedKeyInfo_ )
{
    if ( false == bClear_ || false == bGachaDecreaseEnable_ ) {
        return;
    }

    // map[Level, Count]
    std::map< int, int >::iterator mit;
    for ( mit = mapNeedKeyInfo_.begin() ; mit != mapNeedKeyInfo_.end() ; ++mit ) {
        // 최종 보상을 받았을때 필요한 키 개수를 1개씩 감소 (키 최소개수 1개 보장)
        if ( mit->second > 1 ) {
            mit->second = std::max< int >( 1, mit->second - nGachaDecreaseCount_ );
        }
    }
}

bool KGachaManager::LoadUseVersions(IN KLuaManager& kLuaMng_, OUT std::vector< KGachaNewList >& vecUseVersions_)
{
    vecUseVersions_.clear();

    _JIF(kLuaMng_.BeginTable("UseVersions") == S_OK, return false);

    int i = 1;
    while (true) {
        int nVersion = 0;
        int nStrNum = 0;
        int nTabID = 0;

        if (kLuaMng_.GetValue(i++, nVersion) != S_OK) break;
        _JIF(kLuaMng_.GetValue(i++, nStrNum) == S_OK, return false);
        _JIF(kLuaMng_.GetValue(i++, nTabID) == S_OK, return false);

        KGachaNewList gachaList;
        gachaList.m_iItemID = static_cast<GCITEMID>(nStrNum);
        gachaList.m_nID = nVersion;
        gachaList.m_nTabID = nTabID;

        vecUseVersions_.push_back(gachaList);
    }
    _JIF(kLuaMng_.EndTable() == S_OK, return false); // UseVersions

    return true;
}

void KGachaManager::GetGachaVersions( OUT std::vector< int >& vecVersions_ )
{
    vecVersions_.clear();

    std::map< int, KGachaInfo >::iterator mitGachaInfos;

    KLocker lock( m_csGacha );

    typedef std::map< int, KGachaInfo > mapGachaInfosType;
    BOOST_FOREACH( mapGachaInfosType::value_type& rGachaInfo , m_mapGachaInfos ) {
        vecVersions_.push_back( rGachaInfo.first );
    }
}

bool KGachaManager::GetGachaDecreaseEnable( IN const int nVersion_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        START_LOG( cerr, L"잘못된 가차 버전 정보를 요청하였습니다 : " << nVersion_ ) << END_LOG;
        return false;
    }

    return mitGachaInfos->second.m_bGachaDecreaseEnable;
}

bool KGachaManager::GetAccessoriesEnable( IN const int nVersion_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        START_LOG( cerr, L"잘못된 가차 버전 정보를 요청하였습니다 : " << nVersion_ ) << END_LOG;
        return false;
    }

    return mitGachaInfos->second.m_bAccessoriesEnable;
}

bool KGachaManager::IsReelPointEnable( IN const int nVersion_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        START_LOG( cerr, L"잘못된 가차 버전 정보를 요청하였습니다 : " << nVersion_ ) << END_LOG;
        return false;
    }

    return mitGachaInfos->second.m_bGachaReelPointEnable;
}

bool KGachaManager::IsReelPointEnable()
{
    KLocker lock( m_csGacha );

    bool bReturn = false;

    typedef std::map< int, KGachaInfo > mapGachaInfosType;
    BOOST_FOREACH( mapGachaInfosType::value_type& rGachaInfo, m_mapGachaInfos ) {
        if ( true == rGachaInfo.second.m_bGachaReelPointEnable ) {
            bReturn = true;
        }
    }

    return bReturn;
}

int KGachaManager::GetGachaMaxReelPoint( IN const int nVersion_ )
{
    std::map< int, KGachaInfo >::iterator mitGachaInfos;

    KLocker lock( m_csGacha );

    mitGachaInfos = m_mapGachaInfos.find( nVersion_ );
    if ( mitGachaInfos == m_mapGachaInfos.end() ) {
        START_LOG( cerr, L"잘못된 가차 버전 정보를 요청하였습니다 : " << nVersion_ ) << END_LOG;
        return false;
    }

    return mitGachaInfos->second.m_nGachaReelMaxPoint;
}

bool KGachaManager::GetGachaUseVersions( OUT std::vector< KGachaNewList >& vecUseVersions_ )
{
    vecUseVersions_.clear();

    KLocker lock(m_csGacha);

    vecUseVersions_ = m_vecUseVersions;

    return true;
}

bool KGachaManager::LoadDefaultReelPointInfo( IN KLuaManager& kLuaMng_, OUT std::pair< float, float >& prDefaultReelPointInfo_ )
{

    _JIF( kLuaMng_.BeginTable( "DefaultRealPointInfo" ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, prDefaultReelPointInfo_.first ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, prDefaultReelPointInfo_.second ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // DefaultRealPointInfo

    return true;
}

bool KGachaManager::SendUserGachaDataGetFromDBNot( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_UPDATEPLAN_GET_GACHA_USER_DB_DATA_NOT, dwTemp );

    return true;
}

bool KGachaManager::SendGachaDataChangeNot( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_GACHA_DATA_CHANGE_NOT, dwTemp );

    return true;
}