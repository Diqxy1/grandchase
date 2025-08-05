#include "ItemManager.h"
#include <boost/bind/bind.hpp>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KItemManager );
NiImplementRootRTTI( KItemManager );

ImplOstmOperatorW2A( KItemManager );
ImplToStringW( KItemManager )
{

    KLocker lock( m_csSellItem );
    KLocker lock2( m_csItems );
    KLocker lock3( m_csBuyLimitItem );
    KLocker lock4( m_csPackageItem );
    KLocker lock5( m_csInvenBuffItem );

    return START_TOSTRINGW
        << TOSTRINGW( m_dwUpdateGap )
        << TOSTRINGW( m_dwLastUpdateTick )
        << TOSTRINGW( m_mapSellItemInfo.size() )
        << TOSTRINGW( m_mapPackageItemList.size() )
        << TOSTRINGW( m_setBuyLimitItem.size() )
        << TOSTRINGW( m_vecInvenBuffItem.size() );
}

KItemManager::KItemManager(void)
:m_dwUpdateGap( 1000 * 60 * 30 )
{
    m_dwLastUpdateTick = ::GetTickCount();
}

KItemManager::~KItemManager(void)
{
}

void KItemManager::SetUpdateGap( DWORD dwUpdateGap_ )
{
    if( dwUpdateGap_ < 60 * 1000 ) return;

    KLocker lock( m_csSellItem );
    m_dwUpdateGap = dwUpdateGap_;
}

bool KItemManager::CheckUpdateList()
{
    KLocker lock( m_csSellItem );
    if( ::GetTickCount() - m_dwLastUpdateTick >= m_dwUpdateGap )
    {
        m_dwLastUpdateTick = ::GetTickCount();
        return true;
    }
    return false;
}

bool KItemManager::GetSellInfo( IN GCITEMID dwItmeID_, OUT KSellInfo& kSellInfo_ )
{
    KLocker lock( m_csSellItem );
    std::map<GCITEMID, sSellItemInfo>::iterator mit;
    mit = m_mapSellItemInfo.find( dwItmeID_ );
    if( mit == m_mapSellItemInfo.end() ) return false;

    kSellInfo_.m_cSellMoneyType = mit->second.m_cSellMoneyType;
    kSellInfo_.m_nBuyPrice      = mit->second.m_nBuyPrice;
    kSellInfo_.m_nBuyCount      = mit->second.m_nBuyCount;
    kSellInfo_.m_nDivideFactor  = mit->second.m_nDivideFactor;

    return true;
}

void KItemManager::UpdateSellItemInfo( IN std::map<GCITEMID, sSellItemInfo>& mapSellItemInfo_ )
{
    JIF( !mapSellItemInfo_.empty() ); // 비어 있으면 않됨..
    KLocker lock( m_csSellItem );
    m_mapSellItemInfo = mapSellItemInfo_;
}

int KItemManager::_GetTableID( GCITEMID dwItemID, std::map<GCITEMID,int>& mapTable )
{
    KLocker lock( m_csItemTables );
    std::map<GCITEMID,int>::iterator mit = mapTable.find( dwItemID );

    if ( mit == mapTable.end() ) {
        START_LOG( cerr, L"ItemID : " << dwItemID ) << END_LOG;
        return -1;
    }

    return mit->second;
}

int KItemManager::GetGradeTableID( GCITEMID dwItemID )
{
    return _GetTableID( dwItemID, m_mapItemGradeTable );
}

int KItemManager::GetAttributeTableID( GCITEMID dwItemID )
{
    return _GetTableID( dwItemID, m_mapItemAttributeTable );
}

int KItemManager::GetSocketTableID( GCITEMID dwItemID )
{
    return _GetTableID( dwItemID, m_mapItemSocketTable );
}

void KItemManager::_SetTableID( GCITEMID dwItemID, int iTableID, std::map<GCITEMID,int>& mapTable )
{
    KLocker lock( m_csItemTables );
    mapTable[dwItemID] = iTableID;
    m_setItemList.insert( dwItemID );
}

void KItemManager::SetGradeTableID( GCITEMID dwItemID, int iTableID )
{
    _SetTableID( dwItemID, iTableID, m_mapItemGradeTable );
}

void KItemManager::SetAttributeTableID( GCITEMID dwItemID, int iTableID )
{
    _SetTableID( dwItemID, iTableID, m_mapItemAttributeTable );
}

void KItemManager::SetSocketTableID( GCITEMID dwItemID, int iTableID )
{
    _SetTableID( dwItemID, iTableID, m_mapItemSocketTable );
}

bool KItemManager::LoadTableScript(OUT std::string& strScript_)
{
    KLuaManager luaMgr;
    KLuaManager luaMgr2;

    const std::string strScriptName = "ItemTable\\ItemTable.lua";
    strScript_ = strScriptName;
    _JIF(luaMgr.DoFile(strScriptName.c_str()) == S_OK, return false);

    _JIF(luaMgr.BeginTable("LoadTableList") == S_OK, return false);

    std::string strFileName;
    for (int i = 1; SUCCEEDED(luaMgr.GetValue(i, strFileName)); i++)
    {
        strFileName.insert(0, "ItemTable\\");
        _JIF(luaMgr2.DoFile(strFileName.c_str()) == S_OK, continue);
        _JIF(luaMgr2.BeginTable("ItemTableList") == S_OK, continue);

        int iIndex = 0;
        while (++iIndex)
        {
            if (FAILED(luaMgr2.BeginTable(iIndex))) {
                break;
            }

            GCITEMID dwItemID;
            _JIF(luaMgr2.GetValue(1, dwItemID) == S_OK, return false);

            int iGradeTableID = -1;
            int iAttributeTableID = -1;
            int iSocketTableID = -1;
            int nLevelTableID = -1;
            int nEquipLvDownLimit = 0;
            int nContinentTypeTableID = -1;

            _JIF(luaMgr2.GetValue(2, iGradeTableID) == S_OK, return false);
            _JIF(luaMgr2.GetValue(3, iAttributeTableID) == S_OK, return false);
            _JIF(luaMgr2.GetValue(4, iSocketTableID) == S_OK, return false);
            _JIF(luaMgr2.GetValue(5, nLevelTableID) == S_OK, return false);
            _JIF(luaMgr2.GetValue(6, nEquipLvDownLimit) == S_OK, return false);
            luaMgr2.GetValue(7, nContinentTypeTableID);

            SetGradeTableID(dwItemID, iGradeTableID);
            SetAttributeTableID(dwItemID, iAttributeTableID);
            SetSocketTableID(dwItemID, iSocketTableID);
            SetLevelTableID(dwItemID, nLevelTableID);
            SetEquipLvDownLimit(dwItemID, nEquipLvDownLimit);

            luaMgr2.EndTable();
        }

        luaMgr2.EndTable();
    }
    luaMgr.EndTable();
    return true;
}

bool KItemManager::SaveTableScript( std::string strFilePath )
{
    std::ofstream file;

    file.open( strFilePath.c_str() );

    if( file.is_open() == false )
        return false;

    file<<"ItemTableList = {\n";

    KLocker lock( m_csItemTables );
    std::set<GCITEMID>::iterator sit;
    for( sit = m_setItemList.begin() ; sit != m_setItemList.end() ; ++sit )
    {
        file<<"\t{ "
            << (*sit)<< ", "
            <<GetGradeTableID( *sit )<<", "
            <<GetAttributeTableID( *sit )<<", "
            <<GetSocketTableID( *sit )<<" },\n";
    }
    file<<"}\n";

    file.close();

    return true;
}

int KItemManager::GetAttributeCountFromGrade( IN int nGrade )
{
    switch( nGrade )
    {
        case KItem::GRADE_NORMAL:   return 0;
        case KItem::GRADE_RARE:     return 2;
        case KItem::GRADE_EPIC:     return 3;
        case KItem::GRADE_LEGEND:   return 4;
        case KItem::GRADE_MYSTIC:   return 5;
    }
    return 0;
}

void KItemManager::UpdateItemEquipLevelList( std::map< DWORD, DWORD >& mapEquipItemLevel_ )
{
    JIF( !mapEquipItemLevel_.empty() );

    KLocker lock( m_csItems );
    m_mapItemEquipLevel = mapEquipItemLevel_;
}

bool KItemManager::GetItemEquipLevel( IN const GCITEMID ItemID_, OUT DWORD& EquipLevel_ )
{
    EquipLevel_ = 0;

    std::map<DWORD,DWORD>::iterator mit;
    KLocker lock( m_csItems );

    mit = m_mapItemEquipLevel.find( ItemID_ );
    if ( mit == m_mapItemEquipLevel.end() ) {
        return false;
    }

    EquipLevel_ = mit->second;
    return true;
}

bool KItemManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KItemManager::_LoadScript( OUT std::string& strScript_ )
{
    _JIF( LoadTableScript(strScript_), return false );
    _JIF( m_kGradeDecider._LoadScript(strScript_), return false );
    _JIF( m_kAttributeDecider._LoadScript(strScript_), return false );
    _JIF( m_kSocketDecider._LoadScript(strScript_), return false );
    _JIF( LoadScript_MonsterCard(strScript_), return false );
    _JIF( m_kItemBreakup._LoadScript(strScript_), return false );

    strScript_ = "InitManufactureS3.lua";
    _JIF( m_kItemManufacture.LoadScript( "InitManufactureS3.lua" ), return false );
    _JIF( m_kItemConfig._LoadScript(strScript_), return false );
    _JIF( m_kItemLevelDecider._LoadScript(strScript_), return false );
    _JIF( LoadInvenBuffItem(strScript_), return false );

    return true;
}

bool KItemManager::LoadScript_MonsterCard( OUT std::string& strScript_ )
{
    KLuaManager luaMgr;
    std::map<DWORD, bool> mapMonsterCard;
    const std::string strScriptName = "ItemMonsterCard.lua";
    strScript_ = strScriptName;
    _JIF( luaMgr.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( luaMgr.BeginTable( "MONSTER_CARD_LIST" ) == S_OK, return false );

    int iIndex = 0;
    while ( ++iIndex )
    {
        if ( FAILED( luaMgr.BeginTable( iIndex ) ) ) {
            break;
        }

        GCITEMID dwItemID;
        bool bUse = false;
        _JIF( luaMgr.GetValue( "GOODSID", dwItemID ) == S_OK, return false );
        _JIF( luaMgr.GetValue( "SHOW", bUse ) == S_OK, return false );

        mapMonsterCard.insert( std::map<DWORD, bool>::value_type( dwItemID, bUse ) );

        luaMgr.EndTable();
    }

    luaMgr.EndTable();

    KLocker lock( m_csItems );
    m_mapMonsterCard.swap( mapMonsterCard );

    return true;
}

bool KItemManager::FindMonsterCardByItemID( GCITEMID ItemID_ )
{
    KLocker lock( m_csItems );
    std::map<DWORD, bool>::iterator mit;
    mit = m_mapMonsterCard.find( ItemID_ );

    if( mit == m_mapMonsterCard.end() ) {
        return false;
    }

    return true;
}

void KItemManager::DumpBreakupList()
{
    m_kItemBreakup.DumpMaterialList();
}

void KItemManager::DumpBreakupInfo( IN const int& nLv_, IN const int& nGrade_ )
{
    m_kItemBreakup.DumpMaterialInfo( nLv_, nGrade_ );
}

void KItemManager::DumpSetItemInfo( IN const GCITEMID& dwSetID_ )
{
    m_kAttributeDecider.DumpSetItemInfo( dwSetID_ );
}

int KItemManager::GetLevelTableID( IN GCITEMID dwItemID_ )
{
    return _GetTableID( dwItemID_, m_mapItemLevelTable );
}

int KItemManager::GetEquipLvDownLimit( IN GCITEMID dwItemID_ )
{
    KLocker lock( m_csItemTables );
    std::map<GCITEMID,int>::iterator mit;
    mit = m_mapEquipLvDownLimit.find( dwItemID_ );
    if ( mit == m_mapEquipLvDownLimit.end() ) {
        return 0;
    }
    return mit->second;
}

void KItemManager::SetLevelTableID( IN GCITEMID dwItemID_, IN int iTableID_ )
{
    _SetTableID( dwItemID_, iTableID_, m_mapItemLevelTable );
}

void KItemManager::SetEquipLvDownLimit( IN GCITEMID dwItemID_, IN int nLvLimit_ )
{
    KLocker lock( m_csItemTables );
    m_mapEquipLvDownLimit[dwItemID_] = nLvLimit_;
}

void KItemManager::UpdatePackageInfo( IN std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > >& vecList_ )
{
    KLocker lock( m_csPackageItem );
    m_vecPackageTotalList.swap( vecList_ );
}

void KItemManager::GetPackageInfo( OUT std::vector<std::map<GCITEMID,std::vector<KDropItemInfo > > >& vecList_ )
{
    vecList_.clear();

    KLocker lock( m_csPackageItem );
    vecList_ = m_vecPackageTotalList;
}

size_t KItemManager::GetPackageInfoSize()
{
    KLocker lock( m_csPackageItem );
    return m_vecPackageTotalList.size();
}

void KItemManager::SetBuyLimitItem( IN const std::set<GCITEMID>& setList_ )
{
    KLocker lock( m_csBuyLimitItem );
    m_setBuyLimitItem = setList_;
}

bool KItemManager::IsBuyLimitItem( IN const GCITEMID& ItemID_ )
{
    KLocker lock( m_csBuyLimitItem );
    return ( m_setBuyLimitItem.find( ItemID_ ) != m_setBuyLimitItem.end() );
}

int KItemManager::GetUseSocketItemCount( IN const KItem& kItem_ )
{
    int nUseCount = 0;
    DWORD dwEquipLevel = 0;
    SiKItemManager()->GetItemEquipLevel( kItem_.m_ItemID, dwEquipLevel );
    if ( dwEquipLevel == 0 ) {
        dwEquipLevel = static_cast<DWORD>( kItem_.m_sEquipLevel );
    }
    SiKItemManager()->m_kSocketCashItem.GetCashItemNum( dwEquipLevel, (int)kItem_.m_cGradeID, nUseCount );

    return nUseCount;
}

void KItemManager::DumpBuyLimitItem()
{
    //KLocker lock( m_csBuyLimitItem );

    //std::cout << L"--- Dump BuyLimitItem (size:" << m_setBuyLimitItem.size() << L") ---" << dbg::endl;

    //std::set<GCITEMID>::iterator sit;
    //for ( sit = m_setBuyLimitItem.begin() ; sit != m_setBuyLimitItem.end() ; ++sit ) {
    //    std::cout << *sit << L", ";
    //}
    //std::cout << dbg::endl;
    //std::cout << L"--- End Dump ---" << dbg::endl;
}

bool KItemManager::LoadInvenBuffItem( OUT std::string& strScript_ )
{
    KLuaManager luaMgr;
    std::vector<KInvenBuffItem> vecInvenBuffItem;
    std::map<int,std::map<USHORT,GCITEMID> > mapItemGroup;

    const std::string strScriptName = "InitInvenBuffItem.lua";
    strScript_ = strScriptName;
    _JIF( luaMgr.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( luaMgr.BeginTable( "ItemList" ) == S_OK, return false );

    int iIndex = 0;
    while ( ++iIndex )
    {
        if ( FAILED( luaMgr.BeginTable( iIndex ) ) ) {
            break;
        }

        KInvenBuffItem kInfo;
        _JIF( luaMgr.GetValue( "ItemID", kInfo.m_ItemID ) == S_OK, return false );
        _JIF( luaMgr.GetValue( "BuffID", kInfo.m_nBuffID ) == S_OK, return false );
        _JIF( luaMgr.GetValue( "GroupID", kInfo.m_nGroupID ) == S_OK, return false );
        _JIF( luaMgr.GetValue( "PriorityIndex", kInfo.m_usPriorityIndex ) == S_OK, return false );
        _JIF( luaMgr.GetValue( "All", kInfo.m_bAll ) == S_OK, return false );

        _JIF( luaMgr.BeginTable( "ModeCategory" ) == S_OK, return false );
        for ( int i = 1 ; ; ++i ) {
            int nCategory = -1;
            if ( luaMgr.GetValue( i, nCategory ) != S_OK ) break;
            kInfo.m_setModeCategory.insert( nCategory );
        }
        luaMgr.EndTable();

        vecInvenBuffItem.push_back( kInfo );
        mapItemGroup[kInfo.m_nGroupID][kInfo.m_usPriorityIndex] = kInfo.m_ItemID;

        luaMgr.EndTable();
    }

    luaMgr.EndTable();

    {
        KLocker lock( m_csInvenBuffItem );
        m_vecInvenBuffItem.swap( vecInvenBuffItem );
        m_mapItemGroup.swap( mapItemGroup );
    }

    return true;
}

void KItemManager::GetInvenBuffItemList( OUT std::vector<KInvenBuffItem>& vecList_ )
{
    KLocker lock( m_csInvenBuffItem );
    vecList_ = m_vecInvenBuffItem;
}

bool KItemManager::GetInvenBuffItem( IN const GCITEMID& ItemID_, OUT KInvenBuffItem& kInfo_ )
{
    KLocker lock( m_csInvenBuffItem );

    std::vector<KInvenBuffItem>::iterator vit;
    vit = std::find_if( m_vecInvenBuffItem.begin(), m_vecInvenBuffItem.end(),
        boost::bind(&KInvenBuffItem::m_ItemID, boost::placeholders::_1) == ItemID_ );

    if ( vit == m_vecInvenBuffItem.end() ) {
        return false;
    }

    kInfo_ = *vit;
    return true;
}

bool KItemManager::GetGroupItemID( IN const int& nGroupID_, OUT std::map<USHORT,GCITEMID>& mapItemID_ )
{
    mapItemID_.clear();

    KLocker lock( m_csInvenBuffItem );

    std::map<int,std::map<USHORT,GCITEMID> >::iterator mit;
    mit = m_mapItemGroup.find( nGroupID_ );
    if ( mit == m_mapItemGroup.end() ) {
        return false;
    }

    mapItemID_ = mit->second;
    return true;
}

void KItemManager::GetItemBreakupInfo( OUT KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK& kPacket_ )
{
    m_kItemBreakup.GetItemBreakupInfoAll( kPacket_ );
}

bool KItemManager::IsPackageItem( IN GCITEMID dwItemID_ )
{
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo> >>::iterator vitAllPackage;
    std::map<GCITEMID,std::vector<KDropItemInfo>>::iterator mitPackageInfo;

    for ( vitAllPackage = m_vecPackageTotalList.begin(); vitAllPackage != m_vecPackageTotalList.end(); vitAllPackage++ ) {
        mitPackageInfo = vitAllPackage->find( dwItemID_ );

        // 해당 아이템이 패키지 아이템인지 확인
        if ( vitAllPackage->end() != mitPackageInfo ) {
            return true;
        }
    }
    return false;
}