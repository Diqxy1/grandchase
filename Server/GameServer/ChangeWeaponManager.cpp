#include "ChangeWeaponManager.h"
#include "Lua/KLuaManager.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KChangeWeaponManager );
NiImplementRootRTTI( KChangeWeaponManager );
ImplOstmOperatorW2A( KChangeWeaponManager );

ImplToStringW( KChangeWeaponManager )
{
    KLocker lock( m_csChangeWeapon );

    START_TOSTRINGW
        << TOSTRINGW( m_mapMinLv.size() )
        << TOSTRINGW( m_mapUnlockItem.size() );
    return stm_;
}

KChangeWeaponManager::KChangeWeaponManager(void)
{
}

KChangeWeaponManager::~KChangeWeaponManager(void)
{
}

bool KChangeWeaponManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KChangeWeaponManager::_LoadScript( OUT std::string& strScript_ )
{
    std::map<int,int> mapMinLv; // CharacterType, Level
    std::map<GCITEMID,int> mapUnlockItem; // ItemID, Count

    KLuaManager kLuaMng;

    const std::string strScriptName = "InitChangeWeapon.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( LoadMinLv( kLuaMng, mapMinLv ), return false );
    _JIF( LoadUnLockItem( kLuaMng, mapUnlockItem ), return false );

    START_LOG( cerr, L"Load WeponChange script.. mapMinLv.size() : " << mapMinLv.size() << L", mapUnlockItem.size () : " << mapUnlockItem.size() ) << END_LOG;

    KLocker lock( m_csChangeWeapon );
    m_mapMinLv.swap( mapMinLv );
    m_mapUnlockItem.swap( mapUnlockItem );
    return true;
}

bool KChangeWeaponManager::LoadMinLv( IN KLuaManager& kLuaMng_, OUT std::map<int,int>& mapData_ )
{
    _JIF( kLuaMng_.BeginTable( "MinLv" ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {

        int nCharType = -1;
        int nLv = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nLv ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == mapData_.insert( std::make_pair( nCharType, nLv ) ).second ) {
            START_LOG( cerr, L"중복된 레벨 제한 정보.. CharType : " << nCharType ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KChangeWeaponManager::LoadUnLockItem( IN KLuaManager& kLuaMng_, OUT std::map<GCITEMID,int>& mapData_ )
{
    _JIF( kLuaMng_.BeginTable( "UnlockItemID" ) == S_OK, return false );

    for ( int i = 1 ; ; ++i ) {

        GCITEMID ItemID = 0;
        int nCount = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nCount ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( false == mapData_.insert( std::make_pair( ItemID, nCount ) ).second ) {
            START_LOG( cerr, L"중복된 ItemID.. : " << ItemID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KChangeWeaponManager::DumpInfo()
{
    //KLocker lock( m_csChangeWeapon );

    //std::cout << L"--- Dump ChangeWeapon Info ---" << dbg::endl;

    //std::cout << std::tab << L"Min Lv (size:" << m_mapMinLv.size() << L")" << dbg::endl;
    //std::map<int,int>::const_iterator cmitLv;
    //for ( cmitLv = m_mapMinLv.begin() ; cmitLv != m_mapMinLv.end() ; ++cmitLv ) {
    //    std::cout << std::tab << L"CharType : " << cmitLv->first << L", Lv : " << cmitLv->second << dbg::endl;
    //}
    //std::cout << dbg::endl;

    //std::cout << std::tab << L"Unlock Item (size:" << m_mapUnlockItem.size() << L")" << dbg::endl;
    //std::map<GCITEMID,int>::const_iterator cmitItem;
    //for ( cmitItem = m_mapUnlockItem.begin() ; cmitItem != m_mapUnlockItem.end() ; ++cmitItem ) {
    //    std::cout << std::tab << L"ItemID : " << cmitItem->first << L", count : " << cmitItem->second << dbg::endl;
    //}
    //std::cout << dbg::endl;

    //std::cout << L"--- End Dump ---" << dbg::endl;
}

bool KChangeWeaponManager::IsPossibleLv( IN const int& nCharType_, IN const int& nLv_ )
{
    KLocker lock( m_csChangeWeapon );

    std::map<int,int>::const_iterator cmitLv;

    // 캐릭터타입에 맞는 레벨 설정이 있는지.
    cmitLv = m_mapMinLv.find( nCharType_ );
    if ( cmitLv != m_mapMinLv.end() ) {
        return ( cmitLv->second <= nLv_ );
    }

    // 없으면 디폴드 레벨 설정으로 체크
    cmitLv = m_mapMinLv.find( -1 );
    if ( cmitLv != m_mapMinLv.end() ) {
        return ( cmitLv->second <= nLv_ );
    }

    START_LOG( cwarn, L"해당하는 제한 레벨값이 없음. CharType : " << nCharType_ ) << END_LOG;
    return false;
}

bool KChangeWeaponManager::GetUnlockItemList( OUT std::map<GCITEMID,int>& mapItem_ )
{
    KLocker lock( m_csChangeWeapon );
    _JIF( !m_mapUnlockItem.empty(), return false );
    mapItem_ = m_mapUnlockItem;
    return true;
}
