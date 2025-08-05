#include "stdafx.h"
#include "GCDungeonInfoS6.h"
#include "GCDungeonTitleS6.h"
#include "KGCRoomListManager.h"
#include "KGCWorldMapManager.h"
#include "GCHeroDungeonSubInfoS6.h"
#include "GCRaidDungeonInfoS6.h"

IMPLEMENT_CLASSNAME( KGCDungeonInfoS6 );
IMPLEMENT_WND_FACTORY( KGCDungeonInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonInfoS6, "gc_dungeon_info_s6" );

KGCDungeonInfoS6::KGCDungeonInfoS6( void )
: m_pkTitle(NULL)
, m_pkSubInfo(NULL)
, m_pkBossInfo(NULL)
, m_pkHeroSubInfo(NULL)
, m_pkRaidSubInfo(NULL)
{
	LINK_CONTROL("title",m_pkTitle);
	LINK_CONTROL("dungeon_sub_info_s6",m_pkSubInfo);
	LINK_CONTROL( "dungeon_boss_info_s6" ,m_pkBossInfo);
	LINK_CONTROL("hero_dungeon_sub_info_s6",m_pkHeroSubInfo);
	LINK_CONTROL("raid_dungeon_sub_info_s6",m_pkRaidSubInfo);
}

KGCDungeonInfoS6::~KGCDungeonInfoS6( void )
{
}

void KGCDungeonInfoS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCDungeonInfoS6::OnCreate( void )
{
	m_pkTitle->InitState(true);
	m_pkSubInfo->InitState(false, true, this);
	m_pkBossInfo->InitState(true);
	m_pkHeroSubInfo->InitState(false,true, this);
	m_pkRaidSubInfo->InitState(false,true,this);

	UpdateInfoDlg();    

    g_pkUIMgr->DisconnectGlobalEventProcedure(m_pkSubInfo->GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL, m_pkSubInfo,&KGCDungeonSubInfoS6::SetItemInfo );
    g_pkUIMgr->DisconnectGlobalEventProcedure(m_pkBossInfo->GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL,m_pkBossInfo, &KGCDungeonBossInfoS6::SetBoss);
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_SELECT);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_SELECT, this, &KGCDungeonInfoS6::UpdateInfoDlg);
}

void KGCDungeonInfoS6::UpdateInfoDlg( void )
{
	bool bHeroDungeon = SiKGCWorldMapManager()->IsHeroDungeonMode();
	bool bRaidDungeon = SiKGCWorldMapManager()->IsRaidHeroDungeon();
	bool bInfinityRaid = SiKGCWorldMapManager()->IsInfinityRaid();

	if ( bRaidDungeon ) {
        m_pkRaidSubInfo->ToggleRender(true);
		m_pkHeroSubInfo->ToggleRender(false);
		m_pkSubInfo->ToggleRender(false);	
	}
	else if (bInfinityRaid) {
		m_pkRaidSubInfo->ToggleRender(true);
		m_pkHeroSubInfo->ToggleRender(false);
		m_pkSubInfo->ToggleRender(false);
	}
	else if ( bHeroDungeon ) {
		m_pkHeroSubInfo->ToggleRender(true);
		m_pkSubInfo->ToggleRender(false);
		m_pkRaidSubInfo->ToggleRender(false);
	}
	else {
		m_pkSubInfo->UpdateChangeState();
		m_pkHeroSubInfo->ToggleRender(false);
		m_pkSubInfo->ToggleRender(true);
		m_pkRaidSubInfo->ToggleRender(false);
	}

	m_pkBossInfo->SetBoss();
}
