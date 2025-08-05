#include "ManufactureS3.h"
#include "UserPacket.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "ItemManager.h"
#include "GCHelper.h"
#include "UserEvent.h"
#include "Log4.h"

KManufactureS3::KManufactureS3(void)
{
}

KManufactureS3::~KManufactureS3(void)
{
}

bool KManufactureS3::LoadScript( IN const std::string& strScript_ )
{
    KLuaManager kLuaMng;
    _JIF( kLuaMng.DoFile( strScript_.c_str() ) == S_OK, return false );

    std::map< PAIR_USHORT_DWORD, KManufactureItem > mapItemList;
    std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >mapMaterialList;
    std::map< int, std::vector<KManufactureItem> > mapTabItemList;

    _JIF( kLuaMng.BeginTable( "Manufacture" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;

        std::vector<KDropItemInfo> vecMaterial;
        KManufactureItem kCatalog;
        kCatalog.m_Index = static_cast<USHORT>(i-1);

        _JIF( LoadCatalog( kLuaMng, kCatalog), return false );
        PAIR_USHORT_DWORD prKey = std::make_pair(kCatalog.m_Index, kCatalog.m_ItemID );
        LoadLookItem( kLuaMng, kCatalog );
        LoadCategory( kLuaMng, kCatalog );
        LoadContinents( kLuaMng, kCatalog );
        _JIF( LoadMaterials( kLuaMng, vecMaterial ), return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        if ( mapItemList.insert( std::make_pair(prKey, kCatalog) ).second == false ) {
            START_LOG( cerr, L" 중복된 아이템 목록이 있음. Idx : " << prKey.first << L",ItemID : " << prKey.second ) << END_LOG;
            return false;
        }

        if ( mapMaterialList.insert( std::make_pair(prKey, vecMaterial) ).second == false ) {
            START_LOG( cerr, L" 중복된 아이템 목록이 있음.(Material) Idx : " << prKey.first << L",ItemID : " << prKey.second ) << END_LOG;
            return false;
        }

        SetTabItemList( kCatalog, mapTabItemList );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 아이템 제작 스크립트 로드 완료 : " << strScript_.c_str() )
        << BUILD_LOG( mapItemList.size() )
        << BUILD_LOG( mapMaterialList.size() )
        << BUILD_LOG( mapTabItemList.size() ) << END_LOG;

    KLocker lock( m_csManufacture );
    m_mapItemList.swap( mapItemList );
    m_mapMaterialList.swap( mapMaterialList );
    m_mapTabItemList.swap( mapTabItemList );
    m_buffClientCatalog.Clear();
    return true;
}

bool KManufactureS3::LoadCatalog( IN OUT KLuaManager& kLuaMng_, OUT KManufactureItem& kCatalog_ )
{
    _JIF( kLuaMng_.GetValue( "ItemID", kCatalog_.m_ItemID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Count", kCatalog_.m_nDuration ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Period", kCatalog_.m_nPeriod ) == S_OK, return false );
    if ( kLuaMng_.GetValue( "Grade", kCatalog_.m_nGrade ) != S_OK ) {
        kCatalog_.m_nGrade = -1;
    }
    _JIF( kLuaMng_.GetValue( "Price", kCatalog_.m_nPrice ) == S_OK, return false );
    if ( kLuaMng_.GetValue( "Level", kCatalog_.m_nLevel ) != S_OK ) {
        kCatalog_.m_nLevel = -1;
    }
    if ( kLuaMng_.GetValue( "IsOnDiscount", kCatalog_.m_bIsOnDiscount ) != S_OK ) {
        kCatalog_.m_bIsOnDiscount = true;
    }
    return true;
}

bool KManufactureS3::LoadMaterials( IN OUT KLuaManager& kLuaMng_, OUT std::vector<KDropItemInfo>& vecMaterial_ )
{
    vecMaterial_.clear();
    _JIF( kLuaMng_.BeginTable( "Material" ) == S_OK, return false );

    int i = 1;
    while( true ) {
        KDropItemInfo kMaterial;
        if ( kLuaMng_.GetValue( i++, kMaterial.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kMaterial.m_nDuration ) == S_OK, return false ); // 영구 아이템인 경우 -1 이라 그냥 두면 된다.
        vecMaterial_.push_back( kMaterial );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KManufactureS3::LoadLookItem( IN OUT KLuaManager& kLuaMng_, IN OUT KManufactureItem& kItem_ )
{
    bool bLookItem = false;
    if ( kLuaMng_.GetValue( "IsLook", bLookItem ) != S_OK ) {
        return;
    }

    if ( bLookItem ) {
        kItem_.SetItemFlag( KManufactureItem::IF_LOOK );
    }
}

void KManufactureS3::LoadCategory( IN OUT KLuaManager& kLuaMng_, IN OUT KManufactureItem& kItem_ )
{
    bool bEventItem = false;
    kLuaMng_.GetValue( "IsEvent", bEventItem );
    if ( bEventItem ) {
        kItem_.SetItemFlag( KManufactureItem::IF_EVENT );
    }

    bool bVIPItem = false;
    kLuaMng_.GetValue( "IsVIP", bVIPItem );
    if ( bVIPItem ) {
        kItem_.SetItemFlag( KManufactureItem::IF_VIP );
    }

    int nTabID = -1;
    kLuaMng_.GetValue( "GuildTab", nTabID );
    switch ( nTabID )
    {
    case 0: kItem_.SetItemFlag( KManufactureItem::IF_GUILD_1 ); break;
    case 1: kItem_.SetItemFlag( KManufactureItem::IF_GUILD_2 ); break;
    case 2: kItem_.SetItemFlag( KManufactureItem::IF_GUILD_3 ); break;
    default: break;
    }

    bool bAgitObj = false;
    kLuaMng_.GetValue( "ISAgitObj", bAgitObj );
    if ( bAgitObj ) {
        kItem_.SetItemFlag( KManufactureItem::IF_AGIT_OBJ );
    }

    kItem_.m_nInvelLimit = -1;
    kLuaMng_.GetValue( "InvenLimit", kItem_.m_nInvelLimit );
}

DWORD KManufactureS3::GetContinentsFlag( IN const int nContinents_ )
{
    DWORD dwContinentsFlag = 0;

    switch ( nContinents_ ) {
        case -1:
        case 0:
            dwContinentsFlag = KManufactureItem::ECF_ALL;
            break;

        case 1:
            dwContinentsFlag = KManufactureItem::ECF_BERMESIA_SERDIN;
            break;

        case 2:
            dwContinentsFlag = KManufactureItem::ECF_BERMESIA_CANAVAN;
            break;

        case 3:
            dwContinentsFlag = KManufactureItem::ECF_SILVERLAND;
            break;

        case 4:
            dwContinentsFlag = KManufactureItem::ECF_ELIA;
            break;

        case 5:
            dwContinentsFlag = KManufactureItem::ECF_XENIA;
            break;

        case 6:
            dwContinentsFlag = KManufactureItem::ECF_ATUM;
            break;

        case 7:
            dwContinentsFlag = KManufactureItem::ECF_ACHAEMEDIA;
            break;

        case 8:
            dwContinentsFlag = KManufactureItem::ECF_FLOSLAND;
            break;

        case 9:
            dwContinentsFlag = KManufactureItem::ECF_DEVILDOM;
            break;

        case 10:
            dwContinentsFlag = KManufactureItem::ECF_ELYOS;
            break;

        default:
            dwContinentsFlag = KManufactureItem::ECF_ALL;
            break;
    }

    return dwContinentsFlag;
}

bool KManufactureS3::LoadContinents( IN OUT KLuaManager& kLuaMng_, OUT KManufactureItem& kItem_ )
{
    if ( S_OK != kLuaMng_.BeginTable( "Continents" ) ) {
        return false;
    }

    bool bIsRead = false; // 혹시모를 스크립트 오류에 대비하여 추가함
    DWORD dwContinentsFlag = 0;

    int i = 1;
    while( true ) {
        int nContinents = -1;

        if ( kLuaMng_.GetValue( i++, nContinents ) != S_OK ) break;

        bIsRead = true;
        dwContinentsFlag |= GetContinentsFlag( nContinents );
    }

    kItem_.SetContinentFlag( dwContinentsFlag );

    if ( false == bIsRead ) {
        kItem_.SetContinentFlag( KManufactureItem::ECF_ALL );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KManufactureS3::BuildPrice()
{
    KLocker lock( m_csManufacture );

    std::vector<KManufactureItem> vecClientCatalog;
    vecClientCatalog.reserve( m_mapItemList.size() );

    std::map< PAIR_USHORT_DWORD, KManufactureItem >::iterator mit;
    for ( mit = m_mapItemList.begin() ; mit != m_mapItemList.end() ; ++mit ) {
        if ( mit->second.m_nPrice < 0 ) {
            mit->second.m_nPrice = GetCostFromSocket( mit->second.m_ItemID );
        }

        if ( mit->second.m_nPrice < 0 ) {
            continue;
        }

        vecClientCatalog.push_back( mit->second );
    }

    KSerBuffer buffClientCatalog;
    SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_MANUFACTURES3_CATALOG_ACK, vecClientCatalog, buffClientCatalog, 100 );
    m_buffClientCatalog.Swap( buffClientCatalog );
    return true;
}

bool KManufactureS3::GetCatalog( IN const PAIR_USHORT_DWORD& prKey_, OUT KManufactureItem& kCatalogs_ )
{
    std::map< PAIR_USHORT_DWORD, KManufactureItem >::iterator mit;
    KLocker lock( m_csManufacture );
    mit = m_mapItemList.find( prKey_ );
    if ( mit == m_mapItemList.end() ) return false;
    kCatalogs_ = mit->second;
    return true;
}

bool KManufactureS3::GetCatalog( IN const PAIR_USHORT_PAIR_DWORD_INT& prKey_, OUT KManufactureItem& kCatalogs_ )
{
    PAIR_USHORT_DWORD kTemp;
    kTemp.first = prKey_.first;
    kTemp.second = prKey_.second.first;

    std::map< PAIR_USHORT_DWORD, KManufactureItem >::iterator mit;
    KLocker lock( m_csManufacture );
    mit = m_mapItemList.find( kTemp );
    if ( mit == m_mapItemList.end() ) return false;
    kCatalogs_ = mit->second;
    if( prKey_.second.second > 0 )
        kCatalogs_.m_nDuration *= prKey_.second.second;
    return true;
}

bool KManufactureS3::GetCatalogs( OUT KSerBuffer& buffEvent_ )
{
    buffEvent_.Clear();
    KLocker lock( m_csManufacture );
    if ( m_buffClientCatalog.GetLength() == 0 )
        BuildPrice();

    if ( m_buffClientCatalog.GetLength() == 0 ) return false;

    buffEvent_ = m_buffClientCatalog;
    return true;
}

bool KManufactureS3::GetCatalogs( OUT std::map< PAIR_USHORT_DWORD, KManufactureItem >& mapList_ )
{
    KLocker lock( m_csManufacture );
    if ( m_mapItemList.empty() ) return false;
    mapList_ = m_mapItemList;
    return true;
}

bool KManufactureS3::GetCatalogs( OUT std::map<int, std::vector<KManufactureItem> >& mapList_ )
{
    KLocker lock( m_csManufacture );
    if ( m_mapTabItemList.empty() ) return false;
    mapList_ = m_mapTabItemList;
    return true;
}

bool KManufactureS3::GetCatalogByItemID( IN const GCITEMID& ItemID_, OUT KManufactureItem& kCatalogs_ )
{
    std::map< PAIR_USHORT_DWORD, KManufactureItem >::iterator mit;
    KLocker lock( m_csManufacture );
    for ( mit = m_mapItemList.begin() ; mit != m_mapItemList.end() ; ++mit ) {
        if ( mit->first.second == ItemID_ ) {
            break;
        }
    }

    if ( mit == m_mapItemList.end() ) {
        return false;
    }

    kCatalogs_ = mit->second;
    return true;
}

bool KManufactureS3::GetMaterials( IN const PAIR_USHORT_DWORD& prKey_, OUT std::vector<KDropItemInfo>& vecMaterials_ )
{
    vecMaterials_.clear();
    std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >::iterator mit;
    KLocker lock( m_csManufacture );
    mit = m_mapMaterialList.find( prKey_ );
    if ( mit == m_mapMaterialList.end() ) return false;

    _JIF( !mit->second.empty(), return false );
    vecMaterials_ = mit->second;
    return true;
}

bool KManufactureS3::GetMaterials(IN const PAIR_USHORT_PAIR_DWORD_INT& prKey_, OUT std::vector<KDropItemInfo>& vecMaterials_)
{
    PAIR_USHORT_DWORD kTemp;
    kTemp.first = prKey_.first;
    kTemp.second = prKey_.second.first;

    vecMaterials_.clear();
    std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >::iterator mit;

    KLocker lock( m_csManufacture );
    mit = m_mapMaterialList.find( kTemp );
    if ( mit == m_mapMaterialList.end() ) return false;
    _JIF( !mit->second.empty(), return false );
    vecMaterials_ = mit->second;
    for (int i = 0; i < vecMaterials_.size(); i++)
    {
        if( prKey_.second.second > 0 )
            vecMaterials_[i].m_nDuration *= prKey_.second.second;
    }
    return true;
}

int KManufactureS3::GetCostFromSocket( IN const GCITEMID ItemID_ )
{
    DWORD dwEquipLevel = 0;
    int nCost = -1;
    SiKItemManager()->GetItemEquipLevel( ItemID_, dwEquipLevel );
    if ( !SiKItemManager()->m_kSocketCashItem.GetConsumeGP( dwEquipLevel, 0, nCost ) )
        return -1;
    return nCost;
}

void KManufactureS3::GetMaterialList( IN const std::set< PAIR_USHORT_DWORD >& setRequest_, OUT std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterialList_ )
{
    std::set< PAIR_USHORT_DWORD >::const_iterator sit;
    for ( sit = setRequest_.begin() ; sit != setRequest_.end() ; ++sit ) {
        std::vector<KDropItemInfo> vecMaterials;
        if ( !GetMaterials( *sit, vecMaterials ) ) {
            START_LOG( cwarn, L"존재하지 않는 제작 리스트 입니다. Idx : " << (*sit).first << L", ItemID : " << (*sit).second ) << END_LOG;
            continue;
        }
        LIF( mapMaterialList_.insert( std::make_pair( *sit, vecMaterials ) ).second );
    }
}

void KManufactureS3::GetMaterialList( OUT std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterialList_ )
{
    KLocker lock( m_csManufacture );
    mapMaterialList_ = m_mapMaterialList;
}

void KManufactureS3::SetTabItemList( IN const KManufactureItem& kCatalog_, IN OUT std::map<int, std::vector<KManufactureItem> >& mapList_ )
{
    int nTabID = -1;

    if ( kCatalog_.CheckItemFlag( KManufactureItem::IF_GUILD_1 ) ) {
        nTabID = 0;
    }
    else if ( kCatalog_.CheckItemFlag( KManufactureItem::IF_GUILD_2 ) ) {
        nTabID = 1;
    }
    else if ( kCatalog_.CheckItemFlag( KManufactureItem::IF_GUILD_3 ) ) {
        nTabID = 2;
    }

    if ( nTabID >= 0 ) {
        mapList_[ nTabID ].push_back( kCatalog_ );
    }
}

bool KManufactureS3::GetTabIDs( OUT std::vector<int>& vecList_ )
{
    KLocker lock( m_csManufacture );

    vecList_.clear();
    vecList_.reserve( m_mapTabItemList.size() );

    std::map< int, std::vector<KManufactureItem> >::const_iterator cmit;
    for ( cmit = m_mapTabItemList.begin() ; cmit != m_mapTabItemList.end() ; ++cmit ) {
        vecList_.push_back( cmit->first );
    }

    return true;
}