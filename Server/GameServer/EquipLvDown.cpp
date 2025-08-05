#include "EquipLvDown.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KEquipLvDown );
ImplOstmOperatorW2A( KEquipLvDown );
NiImplementRootRTTI( KEquipLvDown );

KEquipLvDown::KEquipLvDown(void)
:m_bEquipLvDownEnable(false)
{
}

KEquipLvDown::~KEquipLvDown(void)
{
}

ImplToStringW( KEquipLvDown )
{
    KLocker lock1( m_csEnable );
    KLocker lock2( m_csConfig );

    return START_TOSTRINGW
        << TOSTRINGWb( m_bEquipLvDownEnable )
        << TOSTRINGW( m_setMaterialItemID.size() );
}

bool KEquipLvDown::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEquipLvDown::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;

    bool bEnable = false;
    std::set<GCITEMID> setMaterialItemID;
    setMaterialItemID.clear();

    const std::string strScriptName = "InitEquipLvDown.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "EquipLvDownEnable", bEnable ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "MaterialItemID" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        GCITEMID MaterialItemID = 0;
        if ( kLuaMng.GetValue( i, MaterialItemID ) != S_OK ) break;
        setMaterialItemID.insert( MaterialItemID );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Load EquipLvDown Script.." )
        << BUILD_LOG( bEnable )
        << BUILD_LOG( setMaterialItemID.size() ) << END_LOG;

    {
        KLocker lock(m_csEnable);
        m_bEquipLvDownEnable = bEnable;
    }

    {
        KLocker lock(m_csConfig);
        m_setMaterialItemID.swap( setMaterialItemID );
    }

    return true;
}

bool KEquipLvDown::IsMarerialItemID( IN const GCITEMID& ItemID_ )
{
    KLocker lock(m_csConfig);
    return ( m_setMaterialItemID.find( ItemID_ ) != m_setMaterialItemID.end() );
}

int KEquipLvDown::CalcMaterialIemCount( IN const int& nItemLv_, IN const int& nLvDiff_ )
{
    KLocker lock(m_csConfig);
    int nMaterialCount = 0;

    // 계산공식
    nMaterialCount = ( nItemLv_ / 10 ) + ( nLvDiff_ * nLvDiff_ );

    return std::max<int>( nMaterialCount, 1 );
}