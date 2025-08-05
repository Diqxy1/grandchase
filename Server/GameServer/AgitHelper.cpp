#include "AgitHelper.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "Log4.h"
#include "boost/bind.hpp"

ImplementSingleton( KAgitHelper );
NiImplementRootRTTI( KAgitHelper );
ImplOstmOperatorW2A( KAgitHelper );

KAgitHelper::KAgitHelper(void)
{
}

KAgitHelper::~KAgitHelper(void)
{
}

ImplToStringW( KAgitHelper )
{
    KLocker lock1(m_csConfig);
    KLocker lock2(m_csMap);
    KLocker lock3(m_csFlowerPot);
    KLocker lock4(m_csAgitSID);

    START_TOSTRINGW
        << TOSTRINGW( m_kAgitConfig.m_nMaxInvenSize )
        << TOSTRINGW( m_kAgitConfig.m_nMaxFavoriteCount )
        << TOSTRINGW( m_kAgitConfig.m_usMaxAgitNameLength )
        << TOSTRINGW( m_kAgitConfig.m_usMaxPasswordLength )
        << TOSTRINGW( m_kAgitConfig.m_usMaxProfileLength )
        << TOSTRINGW( m_kAgitConfig.m_usSellRewardFlowerCount )
        << TOSTRINGW( m_kAgitConfig.m_usMaxGuestbookMsgLenth )
        << TOSTRINGW( m_kAgitConfig.m_dwGuestbookWriteTerm )
        << TOSTRINGW( m_setAgitObjectItemID.size() )
        << TOSTRINGW( m_setBrownieItemID.size() )
        << TOSTRINGW( m_setBrownieID.size() )
        << TOSTRINGW( m_mapMapInfo.size() )
        << TOSTRINGW( m_kFlowerPotConfig.m_FlowerItemID )
        << TOSTRINGW( m_kFlowerPotConfig.m_mapFlowerPotOption.size() )
        << TOSTRINGW( m_kFlowerPotConfig.m_nDailyInitSeedCount )
        << TOSTRINGW( m_kFlowerPotConfig.m_nDailyMaxSeedCount )
        << TOSTRINGW( m_kFlowerPotConfig.m_nInvenLimitFlowerCount )
        << TOSTRINGW( m_kFlowerPotConfig.m_nInvenLimitSeedCount )
        << TOSTRINGW( m_kFlowerPotConfig.m_nSeedDropCount )
        << TOSTRINGW( m_kFlowerPotConfig.m_nSeedDropTime )
        << TOSTRINGW( m_kFlowerPotConfig.m_SeedItemID )
        << TOSTRINGW( m_kFlowerPotConfig.m_setCashPotItemID.size() )
        << TOSTRINGW( m_kFlowerPotConfig.m_setNormalPotItemID.size() )
        << TOSTRINGW( m_kFlowerPotConfig.m_usMaxAgitCashPotCount )
        << TOSTRINGW( m_mapAgitSID.size() );
    return stm_;
}

bool KAgitHelper::LoadScript()
{
    return _LoadScript(std::string());
}

bool KAgitHelper::_LoadScript( OUT std::string& strScript_ )
{
    const std::string strAgitStoreScriptName = "InitAgitStore.lua";
    strScript_ = strAgitStoreScriptName;
    _JIF( m_kAgitStoreItems.LoadScript( strAgitStoreScriptName ), return false );

    const std::string strBrownieStoreScriptName = "InitBrownieStore.lua";
    strScript_ = strBrownieStoreScriptName;
    _JIF( m_kBrownieStoreItems.LoadScript( strBrownieStoreScriptName ), return false );

    UpdateAgitObjectItemID();
    UpdateBrownieItemID();
    return true;
}

void KAgitHelper::UpdateAgitObjectItemID()
{
    std::set<GCITEMID> setAgitObjectItemID;
    std::map<PAIR_USHORT_DWORD,KManufactureItem> mapList;
    m_kAgitStoreItems.GetCatalogs( mapList );

    std::map<PAIR_USHORT_DWORD,KManufactureItem>::iterator mit;
    for ( mit = mapList.begin() ; mit != mapList.end() ; ++mit ) {
        if ( false == mit->second.CheckItemFlag(KManufactureItem::IF_AGIT_OBJ) ) {
            continue;
        }

        setAgitObjectItemID.insert( mit->second.m_ItemID );
    }

    std::vector<GCITEMID> vecList;
    std::vector<GCITEMID>::iterator vit;
    GetObjectItemID( vecList );

    for ( vit = vecList.begin() ; vit != vecList.end() ; ++vit ) {
        setAgitObjectItemID.insert( *vit );
    }

    KLocker lock(m_csConfig);
    m_setAgitObjectItemID.swap( setAgitObjectItemID );
}

void KAgitHelper::UpdateBrownieItemID()
{
    std::set<GCITEMID> setBrownieItemID;
    std::map<PAIR_USHORT_DWORD,KManufactureItem> mapList;
    m_kBrownieStoreItems.GetCatalogs( mapList );

    std::map<PAIR_USHORT_DWORD,KManufactureItem>::iterator mit;
    for ( mit = mapList.begin() ; mit != mapList.end() ; ++mit ) {
        setBrownieItemID.insert( mit->second.m_ItemID );
    }

    KLocker lock(m_csConfig);
    m_setBrownieItemID.swap( setBrownieItemID );
}

void KAgitHelper::UpdateAgitConfig( IN const KAgitConfig& kInfo_ )
{
    {
        KLocker lock(m_csConfig);
        m_kAgitConfig = kInfo_;
    }

    std::vector<GCITEMID>::const_iterator cvit;
    for ( cvit = kInfo_.m_vecObjectItemID.begin() ; cvit != kInfo_.m_vecObjectItemID.end() ; ++cvit ) {
        AddAgitObjectItemID( *cvit );
    }
}

void KAgitHelper::UpdateMapInfo( IN const KAgitMapInfo& kInfo_ )
{
    KLocker lock(m_csConfig);
    m_mapMapInfo = kInfo_;
}

void KAgitHelper::UpdateFlowerPotConfig( IN const KFlowerPotConfig& kInfo_ )
{
    {
        KLocker lock(m_csConfig);
        m_kFlowerPotConfig = kInfo_;
    }

    std::set<GCITEMID>::const_iterator sit;
    for ( sit = kInfo_.m_setCashPotItemID.begin() ; sit != kInfo_.m_setCashPotItemID.end() ; ++sit ) {
        AddAgitObjectItemID( *sit );
    }

    for ( sit = kInfo_.m_setNormalPotItemID.begin() ; sit != kInfo_.m_setNormalPotItemID.end() ; ++sit ) {
        AddAgitObjectItemID( *sit );
    }
}

void KAgitHelper::DumpInfo()
{
    // 세부정보 출력
}

void KAgitHelper::GetMapCatalogue( OUT GCITEMID& PayItemID_, OUT std::map<DWORD,KAgitMap>& mapList_ )
{
    mapList_.clear();
    PayItemID_ = GetFlowerItemID();

    KLocker lock( m_csMap );
    mapList_ = m_mapMapInfo;
}

bool KAgitHelper::GetCurrentMapInfo( IN const DWORD& dwMapID_, OUT KAgitMap& kInfo_ )
{
    KLocker lock( m_csMap );

    std::map<DWORD,KAgitMap>::const_iterator cmit;
    cmit = m_mapMapInfo.find( dwMapID_ );
    if ( cmit == m_mapMapInfo.end() ) {
        return false;
    }

    kInfo_ = cmit->second;
    return true;
}

bool KAgitHelper::GetFlowerPotOption( IN const int& nSeedType_, OUT KFlowerPotOption& kOption_ )
{
    KLocker lock(m_csFlowerPot);

    std::map<int,KFlowerPotOption>::const_iterator cmit;
    cmit = m_kFlowerPotConfig.m_mapFlowerPotOption.find( nSeedType_ );
    if ( cmit == m_kFlowerPotConfig.m_mapFlowerPotOption.end() ) {
        return false;
    }

    kOption_ = cmit->second;
    return true;
}

bool KAgitHelper::IsNormalPotItemID( IN const GCITEMID& PotItemID_ )
{
    KLocker lock(m_csFlowerPot);
    return ( m_kFlowerPotConfig.m_setNormalPotItemID.find(PotItemID_) != m_kFlowerPotConfig.m_setNormalPotItemID.end() );
}

bool KAgitHelper::IsCashPotItemID( IN const GCITEMID& PotItemID_ )
{
    KLocker lock(m_csFlowerPot);
    return ( m_kFlowerPotConfig.m_setCashPotItemID.find(PotItemID_) != m_kFlowerPotConfig.m_setCashPotItemID.end() );
}

int KAgitHelper::GetAgitSID( IN const DWORD& dwAgitUID )
{
    KLocker lock(m_csAgitSID);

    std::map<DWORD,int>::iterator mit;
    mit = m_mapAgitSID.find( dwAgitUID );
    if ( mit == m_mapAgitSID.end() ) {
        return -1;
    }

    return mit->second;
}

float KAgitHelper::GetFruitBuffRate( IN const int& nFairyTreeLv_, IN const int& nFruitType_ )
{
    KFairyTreeConfig kConfig;
    GetFairyTreeConfig(kConfig);

    if ( nFruitType_ == KFairyTreeConfig::FBT_BUFF_ALL ) {
        return kConfig.m_fBuffAllRate;
    }

    if ( nFruitType_ == KFairyTreeConfig::FBT_EXP_GP ) {
        return kConfig.m_fExpGpRate;
    }

    std::map<int,float>::iterator mit;
    mit = kConfig.m_mapFruitBuffByLv.find( nFairyTreeLv_ );
    if ( mit == kConfig.m_mapFruitBuffByLv.end() ) {
        START_LOG( clog, L"해당하는 버프 증가량이 없음.. Lv : " << nFairyTreeLv_ ) << END_LOG;
        return 0.f;
    }

    START_LOG( clog, L"나무열매 버프 증가량.. Lv : " << nFairyTreeLv_ << L", BuffRate : " << mit->second ) << END_LOG;
    return mit->second;
}

bool KAgitHelper::GetObjectItemInvenLimitCount( IN const GCITEMID& ItemID_, OUT int& nLimitCount_ )
{
    nLimitCount_ = 0;
    std::map<GCITEMID,int> mapLimitList;
    mapLimitList.clear();

    GetObjectItemInvenLimitList( mapLimitList );
    std::map<GCITEMID,int>::iterator mit;
    mit = mapLimitList.find( ItemID_ );
    if ( mit != mapLimitList.end() ) {
        nLimitCount_ = mit->second;
        return true;
    }

    KManufactureItem kInfo;
    if ( m_kAgitStoreItems.GetCatalogByItemID( ItemID_, kInfo ) ) {
        nLimitCount_ = kInfo.m_nInvelLimit;
        return true;
    }

    return false;
}

bool KAgitHelper::IsTutorialItem( IN const GCITEMID dwItemID_ )
{
    KLocker lock( m_csConfig );

    std::vector< KDropItemInfo >::iterator vit;
    vit = std::find_if( m_kAgitConfig.m_vecTutorialItem.begin(), m_kAgitConfig.m_vecTutorialItem.end(),
        boost::bind( &KDropItemInfo::m_ItemID, _1 ) == dwItemID_ );

    if ( m_kAgitConfig.m_vecTutorialItem.end() == vit ) {
        return false;
    }

    return true;
}

bool KAgitHelper::IsAgitItem( IN const GCITEMID dwItemID_ )
{
    return ( IsObjectItem( dwItemID_ ) || IsTutorialItem( dwItemID_ ) || IsNotResellObjectItemID( dwItemID_ ) );
}
