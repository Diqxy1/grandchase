#pragma once
//#include <string>
//#include <vector>
//#include <map>
#include "KNCSingleton.h"
#include "GCItemStruct.h"

struct GCMissionItem{
    DWORD m_dwGoodsID;
};

class KGCMissionInSort
{
    DeclareSingleton(KGCMissionInSort);
private:
    DWORD m_dwNumItem;
    std::map< DWORD, GCItemData > m_mapItemData;
    std::vector< DWORD >    m_vecMissionItem;
public:
    KGCMissionInSort(void);
    KGCMissionInSort( std::string strFileName_ )    {
        LoadSortFile( strFileName_ );
    };
    ~KGCMissionInSort(void);

    bool LoadSortFile( std::string strFileName_ );
    int GetTotalNum()   { return (int)m_mapItemData.size(); } //아이템의 갯수를 리턴해 줍니다.
    int GetMissionNum() { return (int)m_vecMissionItem.size(); } //미션의 갯수를 리턴해 줍니다.
    void Clear();
    std::vector< DWORD > GetMissionList()   { return m_vecMissionItem; }
    const GCItemData* GetMissionItem( int iIndex );
    const GCItemData* GetItemData( DWORD dwID );

    std::map< DWORD, GCItemData >::iterator Begin() { return m_mapItemData.begin(); }
    std::map< DWORD, GCItemData >::iterator End() { return m_mapItemData.end(); }
    GCItemData* operator[] ( DWORD dwGoodsID )
    {
        if( m_mapItemData.count( dwGoodsID ) == 0 )
            return NULL;
        return &m_mapItemData[dwGoodsID];
    }
};
DefSingletonInline(KGCMissionInSort);