#include "DepotManager.h"

#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "Log4.h"

ImplementSingleton( KDepotManager );
ImplOstmOperatorW2A( KDepotManager );
NiImplementRootRTTI( KDepotManager );

KDepotManager::KDepotManager(void):
m_nMaxTabSize(6)
,m_nMaxSlotRow(10)
,m_cInitTabID(0)
,m_nInitSlotSize(5)
,m_nMaxSlotSize(25)
,m_nExtendSlotSize(5)
,m_TabExtendItemID(0)
,m_SlotExtendItemID(0)
{
    m_setRestrictItem.clear();
}

KDepotManager::~KDepotManager(void)
{
}

ImplToStringW( KDepotManager )
{
    KLocker lock( m_csRestrictItem ); // lock 잘 보자. 이 싱글턴에는 두 개의 크리티컬 섹션이 존재한다

    return START_TOSTRINGW
        << TOSTRINGW( m_setRestrictItem.size() );
}

bool KDepotManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KDepotManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    int nMaxTabSize = 0;
    int nMaxSlotRow = 0;
    GCITEMID tabExtendItemID = 0;
    GCITEMID slotExtendItemID = 0;
    int extendSlotSize = 0;
    int nInitSlotSize = 0;
    int nMaxSlotSize = 0;
    std::set<GCITEMID> setRestrictItem;
    
    strScript_ = "InitDepotManager.lua";
    _JIF( kLuaMng.DoFile( strScript_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "MaxTabSize", nMaxTabSize ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxSlotRow", nMaxSlotRow ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "TAB_EXTEND_ITEM", tabExtendItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SLOT_EXTEND_ITEM", slotExtendItemID ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "EXTEND_SLOT_SIZE", extendSlotSize ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "INIT_SLOT_SIZE", nInitSlotSize ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MAX_SLOT_SIZE", nMaxSlotSize ) == S_OK, return false );

    _JIF( LoadRestrictItem( kLuaMng, setRestrictItem ), return false );

    KLocker lock( m_csDepotInfo );
    m_nMaxTabSize = nMaxTabSize;
    m_nMaxSlotRow = nMaxSlotRow;
    m_TabExtendItemID = tabExtendItemID;
    m_SlotExtendItemID = slotExtendItemID;
    m_nExtendSlotSize = extendSlotSize;
    m_nInitSlotSize = nInitSlotSize;
    m_nMaxSlotSize = nMaxSlotSize;

    KLocker lock2( m_csRestrictItem );
    m_setRestrictItem.swap( setRestrictItem );

    START_LOG( cerr, L"창고 정보 스크립트 읽기" )
        << BUILD_LOG( m_nMaxTabSize )
        << BUILD_LOG( m_nMaxSlotSize )
        << BUILD_LOG( m_nMaxSlotRow )
        << BUILD_LOG( m_TabExtendItemID )
        << BUILD_LOG( m_SlotExtendItemID )
        << BUILD_LOG( m_nExtendSlotSize )
        << BUILD_LOG( m_nInitSlotSize )
        << BUILD_LOG( m_setRestrictItem.size() )
        << END_LOG;

    return true;
}

void KDepotManager::GetDepotMaxSizeInfo( OUT KEVENT_DEPOT_INFO_ACK& kData_ )
{
    KLocker lock( m_csDepotInfo );
    kData_.m_nMaxTabSize = m_nMaxTabSize;
    kData_.m_nMaxSlotRow = m_nMaxSlotRow;
}

char KDepotManager::GetInitTabID()
{
    KLocker lock( m_csDepotInfo );
    return m_cInitTabID;
}

int KDepotManager::GetInitSlotSize()
{
    KLocker lock( m_csDepotInfo );
    return m_nInitSlotSize;
}

void KDepotManager::SetExtendInfo( IN OUT KDepotExtend& kData_ )
{
    // 탭, 슬롯 확장 아이템 
    kData_.m_kDepot.m_nSlotSize = GetSlotExtendSize();

    if ( kData_.m_nExtendType == KDepotExtend::SLOT_EXTEND ) {
        kData_.m_kItem.m_ItemID = m_SlotExtendItemID;
        kData_.m_kDepot.m_nSlotSize = GetSlotExtendSize();
        //kData_.m_kDepot.
    }
    else if ( kData_.m_nExtendType == KDepotExtend::TAB_EXTEND ) {
        kData_.m_kItem.m_ItemID = m_TabExtendItemID;
        kData_.m_kDepot.m_nSlotSize = GetInitSlotSize();
    }

}

int KDepotManager::GetSlotExtendSize()
{
    KLocker lock( m_csDepotInfo );
    return m_nExtendSlotSize;
}

int KDepotManager::GetMaxSlotSize()
{
    KLocker lock( m_csDepotInfo );
    return m_nMaxSlotSize;
}

int KDepotManager::GetMaxTabSize()
{
    KLocker lock( m_csDepotInfo );
    return m_nMaxTabSize;
}

bool KDepotManager::LoadRestrictItem( IN OUT KLuaManager& kLuaMng_, OUT std::set<DWORD>& setRestrictItem_ )
{
    setRestrictItem_.clear();
    _JIF( kLuaMng_.BeginTable( "RestrictItem" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        GCITEMID dwItemID;
        if( kLuaMng_.GetValue( i, dwItemID ) != S_OK ) break;
        setRestrictItem_.insert( dwItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KDepotManager::IsRestrictItem( IN GCITEMID ItemID_ )
{
    KLocker lock( m_csRestrictItem );
    return m_setRestrictItem.find( ItemID_ ) != m_setRestrictItem.end();
}

void KDepotManager::GetRestrictItem( OUT std::set<GCITEMID>& setItem_ )
{
    setItem_.clear();
    KLocker lock( m_csRestrictItem );
    setItem_ = m_setRestrictItem;
}
