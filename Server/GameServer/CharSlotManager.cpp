#include "CharSlotManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KCharSlotManager );
ImplOstmOperatorW2A( KCharSlotManager );
NiImplementRootRTTI( KCharSlotManager );

KCharSlotManager::KCharSlotManager(void)
:m_dwMaxSlotNum(0)
{
    m_mapCharSlotItemInfo.clear();
}

KCharSlotManager::~KCharSlotManager(void)
{
}

ImplToStringW( KCharSlotManager )
{
    KLocker lock( m_csCharSlot );
    return START_TOSTRINGW
        << TOSTRINGW( m_dwMaxSlotNum )
        << TOSTRINGW( m_mapCharSlotItemInfo.size() );
}

bool KCharSlotManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCharSlotManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    DWORD                               dwMaxSlotNum;
    std::map<GCITEMID, int>             mapCharSlotItemInfo;

    const std::string strScriptName = "InitCharSlotInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "MaxCharSlotNum", dwMaxSlotNum ) == S_OK, return false );
    _JIF( LoadCharSlotItemInfo( kLuaMng, std::string("AddCharSlotItemInfo"), mapCharSlotItemInfo ), return false );

    {
        KLocker lock( m_csCharSlot );
        m_dwMaxSlotNum = dwMaxSlotNum;
        m_mapCharSlotItemInfo.swap( mapCharSlotItemInfo );
    }

    START_LOG( cerr, L"KCharDefault 정보 읽기 완료." )
        << BUILD_LOG( m_dwMaxSlotNum ) 
        << BUILD_LOG( m_mapCharSlotItemInfo.size() ) 
        << END_LOG;

    return true;
}

bool KCharSlotManager::LoadCharSlotItemInfo( IN KLuaManager& kLuaMng_, 
                                             IN std::string& strTable_, 
                                             OUT std::map<GCITEMID, int>& mapCharSlotItemInfo_ )
{
    mapCharSlotItemInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID ItemID;
        int nAddSlotNum;

        _JIF( kLuaMng_.GetValue( "ItemID", ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "AddSlotNum", nAddSlotNum ) == S_OK, return false );

        if ( !mapCharSlotItemInfo_.insert( std::make_pair( ItemID, nAddSlotNum ) ).second ) {
            START_LOG( cerr, L"중복된 아이템 정보 있음. ItemID : " << ItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCharSlotManager::GetAddSlotNum( IN const GCITEMID& ItemID_, OUT int& nAddSlotNum_ )
{
    std::map<GCITEMID, int>::iterator mit;
    KLocker lock( m_csCharSlot );
    mit = m_mapCharSlotItemInfo.find( ItemID_ );
    if( mit == m_mapCharSlotItemInfo.end() ) {
        return false;
    }
    nAddSlotNum_ = mit->second;
    return true;
}

void KCharSlotManager::GetCharSlotItemList( OUT std::map<GCITEMID, int>& mapItem_ )
{
    mapItem_.clear();
    KLocker lock( m_csCharSlot );
    mapItem_ = m_mapCharSlotItemInfo;
}