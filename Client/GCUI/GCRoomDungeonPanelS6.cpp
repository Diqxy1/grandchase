#include "stdafx.h"
#include "GCRoomDungeonPanelS6.h"

IMPLEMENT_CLASSNAME( KGCRoomDungeonPanelS6 );
IMPLEMENT_WND_FACTORY( KGCRoomDungeonPanelS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomDungeonPanelS6, "gc_roomdungeonpanel_s6" );

KGCRoomDungeonPanelS6::KGCRoomDungeonPanelS6( void )
: m_pkDungeonBossInfo( NULL ) 
, m_pkDungeonSubInfo( NULL ) 
, m_pkDungeonMissionInfo( NULL ) 
, m_pkDungeonSelButton( NULL ) 
, m_pkAddMissionButton( NULL )
, m_pkMissionAlarm( NULL )
, m_dwNormalMissionID(-1)
{
    LINK_CONTROL( "dungeon_boss_info_s6", m_pkDungeonBossInfo );
    LINK_CONTROL( "dungeon_sub_info_s6", m_pkDungeonSubInfo );
    LINK_CONTROL( "dungeon_mission_info", m_pkDungeonMissionInfo );
    LINK_CONTROL( "sel_dungeon_btn", m_pkDungeonSelButton );
    LINK_CONTROL( "add_mission_btn", m_pkAddMissionButton );
    LINK_CONTROL( "mission_alarm", m_pkMissionAlarm );
}

KGCRoomDungeonPanelS6::~KGCRoomDungeonPanelS6( void )
{
}


void KGCRoomDungeonPanelS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkDungeonSelButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeDungeon );
    GCWND_MSG_MAP( m_pkAddMissionButton, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickAddMission );
}


void KGCRoomDungeonPanelS6::OnCreate( void )
{
    m_pkDungeonBossInfo->InitState( true, true, this );
    m_pkDungeonBossInfo->SetBossRoom( true );
    m_pkDungeonSubInfo->InitState( true, true, this );
    m_pkDungeonMissionInfo->InitState( true, true, this );
    m_pkDungeonSelButton->InitState( true, true, this );
    m_pkAddMissionButton->InitState( true, true, this );
    m_pkMissionAlarm->InitState( false );

    m_dwNormalMissionID = 0;
    m_pkAddMissionButton->Lock(true);
}

void KGCRoomDungeonPanelS6::OnChangeDungeon()
{
	if ( g_kGlobalValue.m_kUserInfo.bHost == false ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DUNGEION_ERR2), L"" ,KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
		return;
	}
	else if ( SiKGCRoomManager()->IsHellMode() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DUNGEION_ERR1), L"" ,KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
		return;
	}
	
    g_pkUIScene->m_pkRoomSceneS6->ShowWorldMap( true );
}

void KGCRoomDungeonPanelS6::UpdateGameData()
{
    m_pkDungeonBossInfo->SetBoss();
    m_pkDungeonSubInfo->UpdateData();
    m_pkDungeonMissionInfo->UpdateData();

    UpdateDunSelBtn();
}

void KGCRoomDungeonPanelS6::UpdateDunSelBtn()
{
    m_pkDungeonSelButton->ToggleRender( true );

    if( SiKGCWorldMapManager()->IsHeroDungeonMode(SiKGCRoomManager()->GetGameMode()) )
        m_pkDungeonSelButton->ToggleRender( false );

    if( SiKGCWorldMapManager()->IsEventDungeon(SiKGCRoomManager()->GetGameMode()) )
        m_pkDungeonSelButton->ToggleRender( false );

    if( SiKGCWorldMapManager()->IsInfinityDungeon(SiKGCRoomManager()->GetGameMode()) )
        m_pkDungeonSelButton->ToggleRender( false );

    m_pkDungeonSelButton->Lock( !g_kGlobalValue.m_kUserInfo.bHost || SiKGCRoomManager()->IsHellMode() || SiKGCRoomManager()->IsBreakMode());
}

void KGCRoomDungeonPanelS6::UpdateMissionData()
{
    m_pkDungeonMissionInfo->UpdateData();
}

void KGCRoomDungeonPanelS6::OnClickAddMission()
{
    if ( m_dwNormalMissionID != 0 )
    {
        KP2P::GetInstance()->Send_DungeonMissionRegistReq(m_dwNormalMissionID);
    }
}

void KGCRoomDungeonPanelS6::SetNormalMission( DWORD dwMissionID )
{
    m_dwNormalMissionID = dwMissionID;

    if ( m_dwNormalMissionID != 0 )
    {
        m_pkAddMissionButton->Lock(false);
        m_pkMissionAlarm->ToggleRender( true );
    }
    else
    {
        m_pkAddMissionButton->Lock(true);
        m_pkMissionAlarm->ToggleRender( false );
    }
}
