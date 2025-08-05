#include "stdafx.h"
#include "GCWorldMapScene.h"


//

#include "gcui/GCOnGameDlg.h"
#include "GCInventoryFullMsg.h"
#include "../BuddySystem/KFriendShip.h"
#include "KGCBillboardChatBoard.h"

IMPLEMENT_CLASSNAME( KGCWorldMapScene );
IMPLEMENT_WND_FACTORY( KGCWorldMapScene );
IMPLEMENT_WND_FACTORY_NAME( KGCWorldMapScene, "gc_worldmap_scene" );

KGCWorldMapScene::KGCWorldMapScene( void )
{
    LINK_CONTROL("worldbillboardchat", m_pkBillboardChatBoard);
}

KGCWorldMapScene::~KGCWorldMapScene(void)
{
}

void KGCWorldMapScene::OnCreate()
{
    KGCScene::OnCreate();
    m_pkBillboardChatBoard->InitState(true, true, this);
}

void KGCWorldMapScene::OnInitialize()
{
}

void KGCWorldMapScene::ActionPerformed( const KActionEvent& event )
{
    KGCScene::ActionPerformed( event );
}

void KGCWorldMapScene::FrameMoveInEnabledState( void )
{
    if( g_pkInput->IsDown( DIK_ESCAPE ) )
    {
        if ( false == g_pkUIScene->IsMsgBoxModal() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ESC_MENU_DLG );
            g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        }
    }

	SiKFriendShip()->CheckAlramAddFriend();
	
	KGCScene::FrameMoveInEnabledState();
}

void KGCWorldMapScene::OnEnterScene()
{
    g_pkUIScene->m_pkOnGameDlg->ToggleRender( false );

    g_pkUIScene->m_pkInventoryFullMsg->UpdateMsg();
#if !defined( USE_COORDI_SHOP )
    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect( D3DXVECTOR2( 296.0f, 0.0f ) );
#else
    g_pkUIScene->m_pkInventoryFullMsg->SetWindowPosDirect( D3DXVECTOR2( 296, 0.0f ) );
#endif
    g_pkUIScene->m_pkInventoryFullMsg->ToggleRender( true );

    
    if ( g_kGlobalValue.m_cEventAlarmManager.IsOpenEventAlarm() && GC_GLOBAL_DEFINE::bEventAlarm && g_MyD3D->m_kGuideMgr.IsGuideOngoing() == false ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_ALARM_DLG, L"", L"", 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        g_kGlobalValue.m_cEventAlarmManager.SetEventAlarm( false );
    }

    // 활력정보 출력
    g_pkUIScene->m_pkWorldMapContainer->SetVitalityInfo();
}