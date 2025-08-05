#include "stdafx.h"
#include "GCLevelResetEventManager.h"

ImplementSingleton( KGCLevelResetEventManager )

KGCLevelResetEventManager::KGCLevelResetEventManager()
: m_bInitData( false )
, m_nLevel( 0 )
{
    m_mapCashResetItemList.clear();
    m_mapGPResetItemList.clear();

    m_mapCashRewardItemList.clear();
    m_mapGPRewardItemList.clear();

    m_mapResetCharInfo.clear();
}

KGCLevelResetEventManager::~KGCLevelResetEventManager()
{
}

void KGCLevelResetEventManager::SetLevelResetEventInfo( const KEVENT_LEVEL_RESET_EVENT_INFO_ACK& kPacket )
{
    m_bInitData = true;
    m_nLevel = kPacket.m_nLevel;
    m_mapResetCharInfo = kPacket.m_mapResetCharInfo;
    
    m_mapCashResetItemList = kPacket.m_mapCashResetItemList;
    m_mapGPResetItemList = kPacket.m_mapGPResetItemList;
    
    m_mapCashRewardItemList = kPacket.m_mapCashRewardItemList;
    m_mapGPRewardItemList = kPacket.m_mapGPRewardItemList;
}

bool KGCLevelResetEventManager::IsResetItem( GCITEMID itemID )
{
    if ( !m_bInitData ) 
        return false;

    itemID *= 10;

    std::map<int,GCITEMID>::iterator iter = m_mapCashResetItemList.begin();
    std::map<int,GCITEMID>::iterator iter_end = m_mapCashResetItemList.end();

    for ( ; iter != iter_end; ++iter )
    {   
        if ( iter->second == itemID )
            return true;
    }

    iter = m_mapGPResetItemList.begin();
    iter_end = m_mapGPResetItemList.end();

    for ( ; iter != iter_end; ++iter )
    {   
        if ( iter->second == itemID )
            return true;
    }

    return false;
}

bool KGCLevelResetEventManager::IsResetItem( char cCharType, GCITEMID itemID )
{
    if ( !m_bInitData ) 
        return false;

    itemID *= 10;

    std::map<int,GCITEMID>::iterator iter = m_mapCashResetItemList.find( cCharType );    
    if ( iter != m_mapCashResetItemList.end() )
    {
        if ( iter->second == itemID ) 
            return true;
    }

    iter = m_mapGPResetItemList.find( cCharType );    
    if ( iter != m_mapCashResetItemList.end() )
    {
        if ( iter->second == itemID ) 
            return true;
    }

        return false;
}