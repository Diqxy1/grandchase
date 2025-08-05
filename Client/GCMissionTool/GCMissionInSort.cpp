#include "StdAfx.h"
#include ".\gcmissioninsort.h"

ImplementSingleton(KGCMissionInSort);

KGCMissionInSort::KGCMissionInSort(void)
{
    m_dwNumItem = 0;
}

KGCMissionInSort::~KGCMissionInSort(void)
{
    Clear();
}

bool KGCMissionInSort::LoadSortFile( std::string strFileName_ )
{
    Clear();
    m_dwNumItem = 0;

    FILE* pSortFile = fopen( strFileName_.c_str(), "rb" );

    DWORD dwTotalItem = NULL; //전체 아이템 수(중복 포함하지 않음)
    DWORD dwTotalItemInfo = NULL; //전체 아이템 수(중복 포함)
    int iIndex = 0;

    fread( &dwTotalItemInfo, sizeof( DWORD ), 1, pSortFile );

    if( dwTotalItemInfo == NULL )
        return false;

    fread( &dwTotalItem, sizeof( DWORD ), 1, pSortFile );

    if( dwTotalItem == NULL )
        return false;

    m_dwNumItem = dwTotalItem;

    GCItemData  _ItemData;
    ItemKey _ItemKey;

    for(DWORD i = 0; i < dwTotalItemInfo; ++i )
    {
        fread( & _ItemData, sizeof( GCItemData ), 1, pSortFile );
        m_mapItemData.insert( std::make_pair( _ItemData.dwGoodsID, _ItemData ) );
        if( _ItemData.eItemKind == GIK_MISSION_SCROLL )
            m_vecMissionItem.push_back( _ItemData.dwGoodsID );
    }

    return true;
}

void KGCMissionInSort::Clear()
{
    m_dwNumItem = 0;
    m_mapItemData.clear();
    m_vecMissionItem.clear();
}

const GCItemData* KGCMissionInSort::GetMissionItem( int iIndex )
{
    if( iIndex < 0 || iIndex > (int)m_vecMissionItem.size() )
        return NULL;

    GCItemData* kItem = (*this)[m_vecMissionItem[iIndex]];

    return kItem;
}

const GCItemData* KGCMissionInSort::GetItemData( DWORD dwID )
{
    std::map< DWORD, GCItemData >::iterator mit = m_mapItemData.find( dwID ) ;
    if( mit == m_mapItemData.end() )
        return NULL;

    return &mit->second;
}