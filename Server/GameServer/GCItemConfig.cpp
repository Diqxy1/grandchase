#include "GCItemConfig.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"

KGCItemConfig::KGCItemConfig(void)
{
    m_setNonInvenSizeItem.clear();
    m_setHideInvenItem.clear();
    m_mapNickNameChangeItem.clear();
}

KGCItemConfig::~KGCItemConfig(void)
{
}

bool KGCItemConfig::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGCItemConfig::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "ItemConfigList.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    std::set<GCITEMID> setNonInvenSizeItem;
    std::set<GCITEMID> setHideInvenItem;
    std::map<int,GCITEMID> mapNickNameChangeItem;

    _JIF( LoadSetItemList( kLuaMng, std::string("NonInvenSizeItem"), setNonInvenSizeItem ), return false );
    _JIF( LoadSetItemList( kLuaMng, std::string("HideInvenItem"), setHideInvenItem ), return false );
    _JIF( LoadNickNameChangeItemInfo( kLuaMng, std::string("NickNameChangeCard"), mapNickNameChangeItem ), return false );

    {
        KLocker lock( m_csItemConfig );
        m_setNonInvenSizeItem.swap( setNonInvenSizeItem );
        m_setHideInvenItem.swap( setHideInvenItem );
        m_mapNickNameChangeItem.swap( mapNickNameChangeItem );
    }

    START_LOG( cerr, L"ItemConfig 정보 읽기 완료." )
        << BUILD_LOG( m_setNonInvenSizeItem.size() )
        << BUILD_LOG( m_setHideInvenItem.size() ) 
        << BUILD_LOG( m_mapNickNameChangeItem.size() ) << END_LOG;

    return true;
}

bool KGCItemConfig::LoadSetItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set<GCITEMID>& setItemList_ )
{
    setItemList_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        GCITEMID ItemID;
        if( kLuaMng_.GetValue( i, ItemID ) != S_OK ) break;
        setItemList_.insert( ItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // NonInvenSizeItem

    return true;
}

void KGCItemConfig::GetNonInvenSizeItemList( OUT std::set<GCITEMID>& setNonInvenSizeItemList_ )
{
    setNonInvenSizeItemList_.clear();

    KLocker lock(m_csItemConfig);
    setNonInvenSizeItemList_ = m_setNonInvenSizeItem;
}

void KGCItemConfig::GetHideInvenItemList( OUT std::set<GCITEMID>& setHideInvenItemList_ )
{
    setHideInvenItemList_.clear();

    KLocker lock(m_csItemConfig);
    setHideInvenItemList_ = m_setHideInvenItem;
}

bool KGCItemConfig::IsNoneSizeItem( IN const GCITEMID ItemID_ )
{
    KLocker lock(m_csItemConfig);
    return (m_setNonInvenSizeItem.find( ItemID_ ) != m_setNonInvenSizeItem.end());
}

bool KGCItemConfig::LoadNickNameChangeItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, GCITEMID>& mapNickNameChangeItem_ )
{
    mapNickNameChangeItem_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        int nType;
        GCITEMID itemID;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "Type", nType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ItemID", itemID ) == S_OK, return false );

        if( !mapNickNameChangeItem_.insert( std::make_pair( nType, itemID ) ).second ) {
            START_LOG( cerr, L" 중복된 Type정보가 있음. Type : " << nType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KGCItemConfig::GetNickNameChangeItemInfo( IN const int nType_, OUT GCITEMID& ItemID_ )
{
    std::map<int,GCITEMID>::iterator mit;
    KLocker lock(m_csItemConfig);
    mit = m_mapNickNameChangeItem.find( nType_ );
    if( mit == m_mapNickNameChangeItem.end() ) {
        return false;
    }
    ItemID_ = mit->second;
    return true;
}