#include "stdafx.h"
#include "GCPlantTreeMap.h"

IMPLEMENT_CLASSNAME( KGCPlantTreeMap );
IMPLEMENT_WND_FACTORY( KGCPlantTreeMap );
IMPLEMENT_WND_FACTORY_NAME( KGCPlantTreeMap, "gc_plant_tree_worldmap_list" );

KGCPlantTreeMap::KGCPlantTreeMap( void )
{
    m_mapWorldMapWnd.insert(std::pair<int,KD3DWnd*>(EPTW_BERMESIAH,NULL));
    m_mapWorldMapWnd.insert(std::pair<int,KD3DWnd*>(EPTW_SILVER_LAND,NULL));
    m_mapWorldMapWnd.insert(std::pair<int,KD3DWnd*>(EPTW_ELLIA,NULL));
    m_mapWorldMapWnd.insert(std::pair<int,KD3DWnd*>(EPTW_ARCHIMEDIA,NULL));

    LINK_CONTROL("worldmap_set1",m_mapWorldMapWnd[EPTW_BERMESIAH]);
    LINK_CONTROL("worldmap_set2",m_mapWorldMapWnd[EPTW_SILVER_LAND]);
    LINK_CONTROL("worldmap_set3",m_mapWorldMapWnd[EPTW_ELLIA]);
    LINK_CONTROL("worldmap_set4",m_mapWorldMapWnd[EPTW_ARCHIMEDIA]);
}

KGCPlantTreeMap::~KGCPlantTreeMap( void )
{
}

void KGCPlantTreeMap::ActionPerformed( const KActionEvent& event )
{

}

void KGCPlantTreeMap::OnCreate( void )
{

}

void KGCPlantTreeMap::SetWorldMapIndex( int iIndex )
{
    if(iIndex < EPTW_BERMESIAH || iIndex >EPTW_ARCHIMEDIA)
        iIndex = EPTW_BERMESIAH;

    std::map<int,KD3DWnd*>::iterator mIter = m_mapWorldMapWnd.begin();

    for(;mIter != m_mapWorldMapWnd.end();mIter++)
    {
        if(mIter->first == iIndex)
            mIter->second->ToggleRender(true);
        else
            mIter->second->ToggleRender(false);
    }
}
