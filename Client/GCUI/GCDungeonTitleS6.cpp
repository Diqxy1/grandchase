#include "stdafx.h"
#include "GCDungeonTitleS6.h"

IMPLEMENT_CLASSNAME( KGCDungeonTitleS6 );
IMPLEMENT_WND_FACTORY( KGCDungeonTitleS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonTitleS6, "gc_dungeon_title_s6" );

KGCDungeonTitleS6::KGCDungeonTitleS6( void )
{
	//UPPERWORLDMAP_BERMESIA  = 0,
	//UPPERWORLDMAP_ELIA      = 1,
	//UPPERWORLDMAP_XENEA     = 2,
	//UPPERWORLDMAP_SILVERLAND= 3,
	//UPPERWORLDMAP_ATUM	    = 4,
	//UPPERWORLDMAP_ACHAEMEDIA= 5,

	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for(int i = 0; i < NUM_WORLDMAP ; i++){
		sprintf(szTemp, "title_%d", i);
		m_arTitleList[i] = NULL;
		LINK_CONTROL(szTemp,m_arTitleList[i]);
	}
}

KGCDungeonTitleS6::~KGCDungeonTitleS6( void )
{
}

void KGCDungeonTitleS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCDungeonTitleS6::OnCreate( void )
{
	SetWorldTitle();
}

void KGCDungeonTitleS6::SetWorldTitle( void )
{
	// 대륙정보를 가져와서 켜줄 것 켜준다
	int iSelWorld = SiKGCWorldMapManager()->GetCurrentWorldMap();

	for(int i = 0; i < NUM_WORLDMAP ; i++){
		if(m_arTitleList[i])
			m_arTitleList[i]->ToggleRender(i == iSelWorld);
	}
	
}