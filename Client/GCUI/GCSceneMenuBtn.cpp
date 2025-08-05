#include "stdafx.h"
#include "GCSceneMenuBtn.h"
#include <KNC/ClientErr.h>
#include "./KGCDepotManager.h"
#include "./GCOlympicManager.h"

IMPLEMENT_CLASSNAME( KGCSceneMenuBtn );
IMPLEMENT_WND_FACTORY( KGCSceneMenuBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCSceneMenuBtn, "gc_scenemenubtn" );

KGCSceneMenuBtn::KGCSceneMenuBtn( void )
{
    m_pkSubBox = NULL;
    m_pkMenuOpenBtn = NULL;
    m_pkMenuCloseBtn = NULL;
    m_pkFrame = NULL;
    m_pSubBoxFunc = NULL;

	m_iCntMenuOpenTime = 0;
}

KGCSceneMenuBtn::~KGCSceneMenuBtn( void )
{
}

void KGCSceneMenuBtn::OnCreate( void )
{
    m_pkSubBox = (KD3DSizingBox*)g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "scene_sub_box", "boxes\\scene_menu_sub_box" );
    
#if defined USE_COORDI_SHOP
    m_pkSubBox->SetWindowPosDirect( D3DXVECTOR2( 662.f * GC_SCREEN_DIV_WIDTH , 30.f * GC_SCREEN_DIV_WIDTH ) );
#else
    m_pkSubBox->SetWindowPosDirect( D3DXVECTOR2( 575.f * GC_SCREEN_DIV_WIDTH, 30.f * GC_SCREEN_DIV_WIDTH ) );
#endif
    
    m_pkSubBox->ToggleRender( true );
    m_pkSubBox->SetSelfInputCheck( true );
    m_pkSubBox->SetWidth( GC_SCREEN_DIV_SIZE_INT(50));

    KD3DWnd* pWnd = NULL;
#ifndef DISABLE_HERO_SHOP_ICON
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_heroshop_button",           "buttons\\scene_heroshop_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickHeroShop );
    m_vecMiniBtns.push_back( pWnd );
#endif
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_skilltree_btn",                  "buttons\\scene_skilltree_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickSkillTree );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_couple_button",                  "buttons\\scene_couple_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCouple );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_square_button",                  "buttons\\scene_square_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickSquare );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_attendance_calendar_btn",        "buttons\\scene_calendar_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCalendar );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_mission_button",                 "buttons\\scene_mission_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMission );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_guild_button",              "buttons\\scene_guild_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickGuild );
    m_vecMiniBtns.push_back( pWnd );
#if !defined (PARTY_SYSTEM)
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_party_button",              "buttons\\scene_party_btn" );
    m_vecMiniBtns.push_back( pWnd );
#endif
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_msn_button",                     "buttons\\scene_messenger_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMSN );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_option_button",                  "buttons\\scene_option_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickOption );
    m_vecMiniBtns.push_back( pWnd );
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_createandbreakup_button",   "buttons\\scene_createandbreakup_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCreateBreadkup );
    m_vecMiniBtns.push_back( pWnd );
#if defined(USE_MIGRATION_BTN)
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_user_migration_button",   "buttons\\scene_user_migration_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickUserMigration );
    m_vecMiniBtns.push_back( pWnd );
#endif
#if defined( USE_EVENT_BANNER )
	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_event_banner_button",   "buttons\\scene_event_banner_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickEventBanner );
	m_vecMiniBtns.push_back( pWnd );
#endif


    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_event_minigame_banner_button",   "buttons\\scene_event_minigame_banner_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickEventMiniGameBanner );
    m_vecMiniBtns.push_back( pWnd );


#if defined( REPLAY_BOX_NEW )
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_replay_record_button",           "buttons\\scene_replay_record_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickReplayRecord );
    m_vecMiniBtns.push_back( pWnd );
#endif   


    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_gacha_button",   "buttons\\scene_gacha_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickGacha );
    m_vecMiniBtns.push_back( pWnd );


#if defined( USE_VIP_MENU_BTN )
    if ( !g_kGlobalValue.IsHideSceneMenuBtn( EM_HIDE_TYPE_VIP ) )
    {
        pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_vip_button",   "buttons\\scene_vip_btn" );
        pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickVipReward );
        m_vecMiniBtns.push_back( pWnd );
    }
#endif

#if defined( USE_SURVEY_SYSTEM )
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_survey_button",           "buttons\\scene_survey_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickSurvey );
    m_vecMiniBtns.push_back( pWnd );
#endif

	if(g_pkUIScene->m_pkDepot){
		pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_menu_depot_button",           "buttons\\scene_depot_btn" );
		pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickDepot );
		m_vecMiniBtns.push_back( pWnd );
	}

 
    std::vector<KD3DWnd*>::iterator vit = m_vecMiniBtns.begin();
    for( ; vit != m_vecMiniBtns.end() ; ++vit ) {
        (*vit)->SetWidth( GC_SCREEN_DIV_SIZE_INT(20) );
        (*vit)->SetHeight( GC_SCREEN_DIV_SIZE_INT(20) );
        (*vit)->ToggleRender( false );
        (*vit)->SetSelfInputCheck( false );
    }

    m_pkMenuOpenBtn = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "scene_menu_open_btn", "buttons\\scene_menu_open_btn" );
#if defined USE_COORDI_SHOP
    m_pkMenuOpenBtn->SetWindowPosDirect( D3DXVECTOR2(631.f * GC_SCREEN_DIV_WIDTH, 27.f * GC_SCREEN_DIV_WIDTH) );
#else
    m_pkMenuOpenBtn->SetWindowPosDirect( D3DXVECTOR2(544.f * GC_SCREEN_DIV_WIDTH, 27.f * GC_SCREEN_DIV_WIDTH) );
#endif
    m_pkMenuOpenBtn->SetSelfInputCheck( true );
    m_pkMenuOpenBtn->ToggleRender( true );
    m_pkMenuOpenBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMenuOpen );

    m_pkMenuCloseBtn = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "scene_menu_close_btn", "buttons\\scene_menu_close_btn" );
#if defined USE_COORDI_SHOP
    m_pkMenuCloseBtn->SetWindowPosDirect( D3DXVECTOR2(631.f * GC_SCREEN_DIV_WIDTH, 27.f * GC_SCREEN_DIV_WIDTH) );
#else
    m_pkMenuCloseBtn->SetWindowPosDirect( D3DXVECTOR2(544.f * GC_SCREEN_DIV_WIDTH, 27.f * GC_SCREEN_DIV_WIDTH) );
#endif
    m_pkMenuCloseBtn->SetSelfInputCheck( true );
    m_pkMenuCloseBtn->ToggleRender( false );
    m_pkMenuCloseBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMenuClose );

    m_pkFrame = (KD3DSizingBox*)g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "scene_menu_frame", "boxes\\scene_menu_box" );
    m_pkFrame->SetWidth( GC_SCREEN_DIV_SIZE_INT(265) );
    m_pkFrame->SetHeight( GC_SCREEN_DIV_SIZE_INT(48) );
    
#if defined USE_COORDI_SHOP
    m_pkFrame->SetWindowPosDirect( D3DXVECTOR2(447.f * GC_SCREEN_DIV_WIDTH, 60.f * GC_SCREEN_DIV_WIDTH) );
#else
   m_pkFrame->SetWindowPosDirect( D3DXVECTOR2(534.f * GC_SCREEN_DIV_WIDTH, 60.f * GC_SCREEN_DIV_WIDTH) );
#endif
    m_pkFrame->SetSelfInputCheck( true );
    m_pkFrame->ToggleRender( false );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mission_button", "buttons\\scene_mission_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMission );
	pWnd->SetToolTip( g_pkStrLoader->GetString( 1500 ) );
	m_vecBtns.push_back( pWnd );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "skilltree_btn", "buttons\\scene_skilltree_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickSkillTree );
	pWnd->SetToolTip( g_pkStrLoader->GetString( 3927 ) );
	m_vecBtns.push_back( pWnd );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "attendance_calendar_btn", "buttons\\scene_calendar_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCalendar );
	pWnd->SetToolTip( g_pkStrLoader->GetString(1860), 0.f, 5.f );
	m_vecBtns.push_back( pWnd );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_createandbreakup_button", "buttons\\scene_createandbreakup_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCreateBreadkup );
	pWnd->SetToolTip( g_pkStrLoader->GetString( 6455 ) );
	m_vecBtns.push_back( pWnd );

#if !defined(CLOSED_BETA)
	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_enchant_button", "buttons\\scene_enchant_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickEnchant );
	pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_ENCHANT_MENU_TOOL_TIP ) );
	m_vecBtns.push_back( pWnd );
#endif

#ifndef DISABLE_HERO_SHOP_ICON
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_heroshop_button", "buttons\\scene_heroshop_btn" );
    pWnd->SetToolTip( g_pkStrLoader->GetString(6525) );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickHeroShop );
    m_vecBtns.push_back( pWnd );
#endif

	if(g_pkUIScene->m_pkDepot){
		pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_depot_button",           "buttons\\scene_depot_btn" );
		pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickDepot );
		pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_DEPOT_TOOLTIP ) );
		m_vecBtns.push_back( pWnd );
	}


	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_event_minigame_banner_button",   "buttons\\scene_event_minigame_banner_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickEventMiniGameBanner );
	pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_MINIGAMES ) );
	m_vecBtns.push_back( pWnd );


	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_gacha_button",   "buttons\\scene_gacha_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickGacha );
	pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_BEAD_SEAL_CLEAR ) );
	m_vecBtns.push_back( pWnd );
    
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "couple_button", "buttons\\scene_couple_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCouple );
    pWnd->SetToolTip( g_pkStrLoader->GetString( 3673 ) );
    m_vecBtns.push_back( pWnd );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "msn_button", "buttons\\scene_messenger_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMSN );
	pWnd->SetToolTip( g_pkStrLoader->GetString( 1491 ) );
	m_vecBtns.push_back( pWnd );

#if defined( USE_SURVEY_SYSTEM )
	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_survey_button",           "buttons\\scene_survey_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickSurvey );
	m_vecBtns.push_back( pWnd );
#endif

#if defined( REPLAY_BOX_NEW )
	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "mini_replay_record_button",           "buttons\\scene_replay_record_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickReplayRecord );

	// 브라질 말이 기네...
#if defined(NATION_BRAZIL)
	float fOffsetX = 25.0f;
	float fOffsetY = 0.0f;
	fOffsetX = fOffsetX * g_pGCDeviceManager->GetWindowScaleX();
	fOffsetY = fOffsetY * g_pGCDeviceManager->GetWindowScaleY();

	pWnd->SetToolTip( g_pkStrLoader->GetString( 6607 ), fOffsetX, fOffsetY);
#else
	pWnd->SetToolTip( g_pkStrLoader->GetString( 6607 ) );
#endif

	m_vecBtns.push_back( pWnd );
#endif

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "option_button", "buttons\\scene_option_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickOption );
	pWnd->SetToolTip( g_pkStrLoader->GetString( 1501 ) );
	m_vecBtns.push_back( pWnd );

    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "square_button", "buttons\\scene_square_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickSquare );
    pWnd->SetToolTip( g_pkStrLoader->GetString( 2761 ) );
    m_vecBtns.push_back( pWnd );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "option_button", "buttons\\scene_charselect_scene_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickCharSelectScene );
	//pWnd->SetToolTip( g_pkStrLoader->GetString( 1501 ) );
	m_vecBtns.push_back( pWnd );

	pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "option_button", "buttons\\scene_server_scene_btn" );
	pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickServerScene );
	//pWnd->SetToolTip( g_pkStrLoader->GetString( 1501 ) );
	m_vecBtns.push_back( pWnd );
    


    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_guild_button", "buttons\\scene_guild_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickGuild );
    pWnd->SetToolTip( g_pkStrLoader->GetString( 5328 ) );
    m_vecBtns.push_back( pWnd );
#if !defined (PARTY_SYSTEM)
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_party_button", "buttons\\scene_party_btn" );
    pWnd->SetToolTip( g_pkStrLoader->GetString( 2766 ) );
    pWnd->Lock( true );
    m_vecBtns.push_back( pWnd );
#endif
    
#if defined(USE_MIGRATION_BTN)
    pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_user_migration_button", "buttons\\scene_user_migration_btn" );
    pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickUserMigration );
    pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_USER_MIGRATION_TITLE ), 0.f, 0.f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_LEFT );
    m_vecBtns.push_back( pWnd );
#endif

#if defined( USE_VIP_MENU_BTN )
    if ( !g_kGlobalValue.IsHideSceneMenuBtn( EM_HIDE_TYPE_VIP ) )
    {
        pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_vip_button",   "buttons\\scene_vip_btn" );
        pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickVipReward );
        pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_VIP_REWARD ) );
        m_vecBtns.push_back( pWnd );
    }
#endif 


	if (!g_kGlobalValue.IsHideSceneMenuBtn( EM_HIDE_TYPE_MONSTER_CARD ))
	{
		if ( GC_GLOBAL_DEFINE::bMonsterCardMix ) 
		{
			pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_boxes.stg", "menu_monster_button",           "buttons\\scene_monster_btn" );
			pWnd->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuBtn::OnClickMonster );
			pWnd->SetToolTip( g_pkStrLoader->GetString( STR_ID_MONSTER_MENU_TOOL_TIP ) );
			m_vecBtns.push_back( pWnd );	
		}
	}

    vit = m_vecBtns.begin();
    for( ; vit != m_vecBtns.end() ; ++vit ) {
        (*vit)->ToggleRender( false );
        (*vit)->SetSelfInputCheck( false );
    }

    //g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCSceneMenuBtn::HideSceneButtons );
    //g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCSceneMenuBtn::CheckSubBoxOpen );

    //g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_MESSAGEBOX_OPEN, this, &KGCSceneMenuBtn::UpdateMarks );
    //g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_MESSAGEBOX_CLOSE, this, &KGCSceneMenuBtn::UpdateMarks );
    //g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_ALERT_EVENT_UPDATE, this, &KGCSceneMenuBtn::UpdateMarks ); 
    //g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF, this, &KGCSceneMenuBtn::AllRenderOff ); 
    //g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_ON, this, &KGCSceneMenuBtn::AllRenderOn ); 

    EnableEventCreateInfo();
}

#ifdef USE_VIP_MENU_BTN // Kawan>
void KGCSceneMenuBtn::OnClickVipReward()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_VIP_REWARD); 
}
#endif

void KGCSceneMenuBtn::OnClickGacha()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY ) {
#if defined( USE_COORDI_CLEARSEAL )
    if( g_pkUIScene->GetClearSealDlg()->IsOpenUI() )
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0, g_kGlobalValue.GetSingleGachaVer() );  //m_iItemID, GachaVer
#else
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0);  //m_iItemID
#endif
    }
}

#ifndef DISABLE_HERO_SHOP_ICON
void KGCSceneMenuBtn::OnClickHeroShop()
{
    g_MyD3D->OnHeroShopButton();
}
#endif

void KGCSceneMenuBtn::OnClickSkillTree()
{
    g_MyD3D->OnSkillTreeButton();
}

void KGCSceneMenuBtn::OnClickCouple()
{
    g_MyD3D->OnCoupleButton();
}

void KGCSceneMenuBtn::OnClickSquare()
{
    g_MyD3D->OnSquareButton();
}

void KGCSceneMenuBtn::OnClickCalendar()
{
    g_MyD3D->OnCalendarButton();
}

void KGCSceneMenuBtn::OnClickMission()
{
    g_MyD3D->OnMissionButton();
}

void KGCSceneMenuBtn::OnClickMSN()
{
    g_MyD3D->OnMSNButton();
}

void KGCSceneMenuBtn::OnClickOption()
{
    g_MyD3D->OnOptionButton();
}

void KGCSceneMenuBtn::OnClickCreateBreadkup()
{
    g_MyD3D->OnItemCreateAndBreakUpButton();
}

void KGCSceneMenuBtn::OnClickEventBanner()
{
}

void KGCSceneMenuBtn::OnClickEventMiniGameBanner()
{
#if !defined( LOADING_IMAGE_FROM_SERVER_SCRIPT )

    if(g_kGlobalValue.m_vecEventOpenInfo.empty())
    {
        KLuaManager luaMgr;
        if( !GCFUNC::LoadLuaScript( luaMgr, "EventMinigameInfo.stg", 0 ) )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_NOTHING_EVENT_LIST ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        }

        LUA_BEGIN_TABLE("EventOpenInfo", return)
        {
            for ( int i = 1; ; ++i )
            {
                std::string strTmp;
                KEventOpenInfo kEventOpenInfo;

                LUA_BEGIN_TABLE( i, break );

                luaMgr.GetValue( "EventID", kEventOpenInfo.m_dwEventID );
                luaMgr.GetValue( "EventMBoxID", kEventOpenInfo.m_dwMBoxID );
                luaMgr.GetValue( "EventTextureFileName", strTmp );
                kEventOpenInfo.m_wstrFileName = KncUtil::toWideString( strTmp );
                LUA_BEGIN_TABLE( "EventItemList", return );
                for ( int j = 1; ; ++j )
                {
                    GCITEMID dwItemID;
                    if( luaMgr.GetValue( j, dwItemID ) != S_OK ) break;
                    kEventOpenInfo.m_vecItemList.push_back( dwItemID );
                }
                LUA_END_TABLE(return);

                LUA_END_TABLE(return);

                g_kGlobalValue.m_vecEventOpenInfo.push_back( kEventOpenInfo );
            }        
        }
        LUA_END_TABLE(return);
    }

#endif

    if(g_kGlobalValue.m_vecEventOpenInfo.empty())
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_NOTHING_EVENT_LIST ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_MINIGAME_LIST_DLG );
}

void KGCSceneMenuBtn::OnClickGuild()
{
    if( g_pkUIScene->m_pkSquareOverlayUI == NULL )
        return;

    if( g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu == NULL )
        return;

    if( g_pkUIScene->m_pkSquareOverlayUI->IsRenderOn() )
        g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->ToggleRender( g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->IsRenderOn() == false );
    else
    {
        g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->ToggleRender( g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->IsRenderOn() == false );
        g_pkUIScene->m_pkSquareOverlayUI->ToggleRender(true);
    }
}

void KGCSceneMenuBtn::OnClickReplayRecord()
{
    g_MyD3D->OnReplayRecordBtn();
}

void KGCSceneMenuBtn::OnClickMenuOpen()
{
	m_pkMenuOpenBtn->ToggleRender( false );
	m_pkMenuCloseBtn->ToggleRender( true );
	m_pkFrame->ToggleRender( true );
	ToggleButtons();
	m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_HideStartInit;

}

void KGCSceneMenuBtn::OnClickMenuClose()
{

	m_iCntMenuOpenTime = 0;
	m_pkMenuOpenBtn->ToggleRender( true );
	m_pkMenuCloseBtn->ToggleRender( false );
	m_pkFrame->ToggleRender( false );
	ToggleButtons();
	m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_OpenStart;

}

void KGCSceneMenuBtn::OnClickUserMigration()
{
	if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_USER_MIGRATION_DLG );
	}
}

void KGCSceneMenuBtn::OnClickEnchant()
{
	if ( SiKGCPartyManager()->IsParty() )
		SiKGCPartyManager()->LeaveParty();

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_S6_DLG );
}

void KGCSceneMenuBtn::OnClickSurvey()
{
	if ( g_pkUIScene->m_pkSurveyDlg->IsHaveQuestion() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SURVEY_DLG );

		g_pkUIScene->m_pkSurveyDlg->RenderRewardItem();
	}
}

void KGCSceneMenuBtn::OnClickMonster()
{
	if ( true == g_pkUIScene->m_pkMonsterCardCompose->GetSendReq() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MONSTER_CARD_COMPOSE_DLG );
	}
	else {
		// 서버에서 받은 스크립트 정보가 없다면 요청
		KP2P::GetInstance()->Send_MonsterCardMixInfoReq();
	}
}

void KGCSceneMenuBtn::OnClickDepot()
{
    bool bActive = SiKGCVirtualDepotManager()->IsEmptyItemList();

    if( bActive )
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_DEPOT );
    else // 스트링 변경 필요함
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MSG2) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);

}
void KGCSceneMenuBtn::OnClickCharSelectScene()
{
	g_MyD3D->OnChaSelectSceneBtn();

}

void KGCSceneMenuBtn::OnClickServerScene()
{
	g_MyD3D->OnServerSceneBtn();
}

void KGCSceneMenuBtn::OnDestroy()
{
	RemoveAllChild();
}

void KGCSceneMenuBtn::HideSceneButtons( const KActionEvent& event_ )
{
    if( event_.m_pWnd == NULL )
        return;

    DisableSceneBtns(event_);
    m_setExcept.clear();

#if defined USE_COORDI_SHOP
    D3DXVECTOR2 vPos( 631.f * GC_SCREEN_DIV_WIDTH, 27.f * GC_SCREEN_DIV_WIDTH );
#else
    D3DXVECTOR2 vPos( 544.f * GC_SCREEN_DIV_WIDTH, 27.f * GC_SCREEN_DIV_WIDTH );
#endif

    if( event_.m_pWnd == g_pkUIScene->m_pkServerScene ||
        event_.m_pWnd == g_pkUIScene->m_pkLoginScene ||
        event_.m_pWnd == g_pkUIScene->m_pkRegisterScene ||
        event_.m_pWnd == g_pkUIScene->m_pkLoadingScene ||
        event_.m_pWnd == g_pkUIScene->m_pkSquareLoadingScene ||
        event_.m_pWnd == g_pkUIScene->m_pkGameOverlayUI ) {
            this->ToggleRender( false );
    }
    else if( event_.m_pWnd == g_pkUIScene->m_pkSquareScene ) {
        g_pkUIScene->m_pkSquareOverlayUI->DisableMenuButtons();
        this->ToggleRender( true );
        if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_NORMAL ) {
            m_setExcept.insert( "menu_guild_button" );
        }
#if !defined (PARTY_SYSTEM)
        else {
            m_setExcept.insert( "menu_party_button" );
        }
#endif
        m_setExcept.insert( "skilltree_btn" );
        m_setExcept.insert( "square_button" );
        m_setExcept.insert( "menu_gacha_button" );
        m_setExcept.insert( "menu_vip_button" );
        m_setExcept.insert("menu_event_minigame_banner_button");

        vPos = D3DXVECTOR2( 525.f * GC_SCREEN_DIV_WIDTH, 13.f * GC_SCREEN_DIV_WIDTH );
    }
    else {
        this->ToggleRender( true );
        if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ) {
            m_setExcept.insert( "square_button" );
            m_setExcept.insert( "menu_createandbreakup_button" );
#if !defined(CLOSED_BETA)
            m_setExcept.insert( "menu_enchant_button" );
#endif
            m_setExcept.insert("menu_event_minigame_banner_button");
			m_setExcept.insert("menu_depot_button");

            if ( GC_GLOBAL_DEFINE::bMonsterCardMix ) 
            {
                m_setExcept.insert( "menu_monster_button" );
            }
            m_setExcept.insert( "menu_olympic_button" );

        }
        else if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM ) {
            m_setExcept.insert( "square_button" );
            m_setExcept.insert( "menu_createandbreakup_button" );
#if !defined(CLOSED_BETA)
            m_setExcept.insert( "menu_enchant_button" );
#endif
            m_setExcept.insert("menu_event_minigame_banner_button");

            if ( GC_GLOBAL_DEFINE::bMonsterCardMix ) 
            {
                m_setExcept.insert( "menu_monster_button" );
            }

			m_setExcept.insert( "menu_olympic_button" );

			m_setExcept.insert("menu_depot_button");
        }
        m_setExcept.insert( "menu_guild_button" );
#if !defined (PARTY_SYSTEM)
        m_setExcept.insert( "menu_party_button" );
#endif
    }

#if !defined(SKILL_TREE)
    m_setExcept.insert("skilltree_btn");
#endif

#if !defined(COUPLE_SYSTEM)
    m_setExcept.insert("couple_button");
#endif

#if defined( REPLAY_BOX_NEW ) && !defined(AVI_RECODER)
    m_setExcept.insert("mini_replay_record_button");
#endif

#if !defined( USE_HERO_DUNGEON )
    m_setExcept.insert("menu_heroshop_button");
#endif

#if !defined(USE_EVENT_BANNER)
    m_setExcept.insert("menu_event_banner_button");
#endif


#if !defined(NATION_LATINAMERICA)
    m_setExcept.insert("menu_user_migration_button");
#endif

	if(g_pkUIScene->m_pkDepot == NULL)
	{
		m_setExcept.insert("menu_depot_button");
	}

    UpdateMarks();
    m_pkMenuOpenBtn->SetWindowPosDirect( vPos );
    m_pkMenuCloseBtn->SetWindowPosDirect( vPos );

#if defined USE_COORDI_SHOP
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE )
        m_pkFrame->SetWindowPosDirect( vPos + D3DXVECTOR2( -5.0f * GC_SCREEN_DIV_WIDTH, 33.0f * GC_SCREEN_DIV_WIDTH ) );
    else
        m_pkFrame->SetWindowPosDirect( vPos + D3DXVECTOR2( -97.f * GC_SCREEN_DIV_WIDTH, 33.f * GC_SCREEN_DIV_WIDTH ) );
#else
    m_pkFrame->SetWindowPosDirect( vPos + D3DXVECTOR2( -10.f * GC_SCREEN_DIV_WIDTH, 33.f * GC_SCREEN_DIV_WIDTH ) );
#endif
    
    ToggleButtons();

}

void KGCSceneMenuBtn::ToggleButtons()
{
    int CountX = 1;
    int CountY = 0;
    D3DXVECTOR2 vFramePos = m_pkFrame->GetFixedWindowLocalPos();
    std::vector<KD3DWnd*>::iterator vit = m_vecBtns.begin();
    for( ; vit != m_vecBtns.end() ; ++vit ) {
        if( m_setExcept.find( (*vit)->GetWindowName() ) != m_setExcept.end() ) {
            (*vit)->ToggleRender( false );
            continue;
        }

        (*vit)->SetOrder_Top();
        D3DXVECTOR2 vPos( vFramePos.x - 30.f * GC_SCREEN_DIV_WIDTH + static_cast<float>(CountX) * 36.f * GC_SCREEN_DIV_WIDTH, vFramePos.y + 5.f * GC_SCREEN_DIV_WIDTH + CountY * 36.f * GC_SCREEN_DIV_WIDTH );
        (*vit)->SetWindowPosDirect( vPos );
        (*vit)->ToggleRender( m_pkFrame->IsRenderOn() );
        (*vit)->SetSelfInputCheck( true );

        if( CountY < 1 ) {
            m_pkFrame->SetWidth( static_cast<DWORD>(vPos.x - (vFramePos.x - 30.f * GC_SCREEN_DIV_WIDTH) + 14.f * GC_SCREEN_DIV_WIDTH) );
        }

        if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ||
            g_MyD3D->m_pStateMachine->GetState() == GS_GUILD ) {
            m_pkFrame->SetHeight( static_cast<DWORD>(vPos.y) );
        }
        else {
            m_pkFrame->SetHeight( static_cast<DWORD>(vPos.y) - static_cast<DWORD>(14.f * GC_SCREEN_DIV_WIDTH) );
        }
        m_pkFrame->SetAllWindowAlpha( 200 );

        ++CountX;
        if( CountX > 7 ) {
            CountX = 1;
            ++CountY;
        }
    }
}

void KGCSceneMenuBtn::DisableSceneBtns( const KActionEvent& event_ )
{
    if( event_.m_pWnd == NULL )
        return;

    KD3DWnd* pSceneBtn = NULL;
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_heroshop_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("skilltree_btn")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("couple_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("square_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("attendance_calendar_btn")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("mission_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_guild_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_party_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("msn_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("option_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_createandbreakup_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_event_banner_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_event_minigame_banner_button")) != NULL )
        pSceneBtn->ToggleRender( false );
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_user_migration_button")) != NULL )
        pSceneBtn->ToggleRender( false );
#if !defined(CLOSED_BETA)
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_enchant_button")) != NULL )
        pSceneBtn->ToggleRender( false );
#endif
#if defined( USE_SURVEY_SYSTEM )
    if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_survey_button")) != NULL )
        pSceneBtn->ToggleRender( false );
#endif
	if ( GC_GLOBAL_DEFINE::bMonsterCardMix ) 
	{
		if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_monster_button")) != NULL )
			pSceneBtn->ToggleRender( false );
	}

	if( g_pkUIScene->m_pkDepot){
		if( (pSceneBtn=event_.m_pWnd->GetChildByName("menu_depot_button")) != NULL )
			pSceneBtn->ToggleRender( false );
	}
}



void KGCSceneMenuBtn::FrameMove_OpenStart()
{
    if( m_vecMiniBtns.empty() )
        return;

    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ||
        g_MyD3D->m_pStateMachine->GetState() == GS_GUILD ) {
        m_pSubBoxFunc = NULL;
        m_pkSubBox->ToggleRender(false);
        for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
            m_vecMiniBtns[i]->ToggleRender( false );
        }
        return;
    }

    m_pkSubBox->ToggleRender(true);

    int iCount = 0;
    std::vector<KD3DWnd*>::iterator vit = m_vecBtns.begin();
    for( ; vit != m_vecBtns.end() ; ++vit ) {
        KD3DWnd* pMark = (*vit)->GetChildByName( "mark" );
        if( pMark->IsRenderOn() ) {
            ++iCount;
        }
    }
    if( iCount == 0 ) {
        m_pSubBoxFunc = NULL;
        return;
    }

    m_pkSubBox->SetWidth( GC_SCREEN_DIV_SIZE_INT(20) );

#if defined USE_COORDI_SHOP
    D3DXVECTOR2 vPos(static_cast<float>(655.f * GC_SCREEN_DIV_WIDTH + 15.f * GC_SCREEN_DIV_WIDTH - static_cast<float>(m_pkSubBox->GetWidth())/2.f), 30.0f * GC_SCREEN_DIV_WIDTH );
#else
    D3DXVECTOR2 vPos( static_cast<float>(600 * GC_SCREEN_DIV_WIDTH - static_cast<float>(m_pkSubBox->GetWidth())/2.f) , 30.f * GC_SCREEN_DIV_WIDTH );
#endif
    
    m_pkSubBox->SetWindowPosDirect( vPos );

    vPos.x += 4.f * GC_SCREEN_DIV_WIDTH;
    for( int i = 0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
        m_vecMiniBtns[i]->ToggleRender( false );
        vPos.x += 22.f * GC_SCREEN_DIV_WIDTH;
    }

    m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_MoveUp;
}

void KGCSceneMenuBtn::FrameMove_MoveUp()
{
    D3DXVECTOR2 vPos = m_pkSubBox->GetFixedWindowLocalPos();
    vPos.y -= 3.f * GC_SCREEN_DIV_WIDTH;
    if( vPos.y < 2.f * GC_SCREEN_DIV_WIDTH ) {
        vPos.y = 2.f * GC_SCREEN_DIV_WIDTH;
    }
    m_pkSubBox->SetWindowPosDirect( vPos );

    if( vPos.y == 2.f * GC_SCREEN_DIV_WIDTH ) {
        m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_ChangeWidthInit;
    }
}

void KGCSceneMenuBtn::FrameMove_ChangeWidthInit()
{
    int iCount = 0;
    D3DXVECTOR2 vPos = m_pkSubBox->GetFixedWindowLocalPos();
    vPos.y += m_pkSubBox->GetHeight();
    for( int i = 0 ; i < (int)m_vecBtns.size() ; ++i ) {
        std::stringstream stm;
        stm<<"mini_"<<m_vecBtns[i]->GetWindowName();
        KD3DWnd* pWnd = this->GetChildByName( stm.str() );
        if( pWnd == NULL ) {
/*            ASSERT( !L"null" );*/
            continue;
        }

        if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM && false == IsInRoomEnableMini( stm.str() ) ) { 
            continue;
        }

        KD3DWnd* pMark = m_vecBtns[i]->GetChildByName( "mark" );
        if( pMark == NULL ) {
/*            ASSERT( !L"null" );*/
            continue;
        }

        D3DXVECTOR2 vMiniPos = pWnd->GetFixedWindowLocalPos();
        vMiniPos.y = vPos.y;
        pWnd->SetWindowPosDirect( vMiniPos );
        pWnd->ToggleRender( pMark->IsRenderOn() );
        pWnd->SetSelfInputCheck( false );
        if( pMark->IsRenderOn() )
            ++iCount;
    }

    if( iCount == 0 ) { 
        m_pkSubBox->ToggleRender( false );
        return;
    }

    DWORD dwTargetWidth = static_cast<DWORD>(iCount * 22.f * GC_SCREEN_DIV_WIDTH + 6.f * GC_SCREEN_DIV_WIDTH);
    m_pSubBoxFunc = boost::bind( &KGCSceneMenuBtn::FrameMove_ChangeWidth, this, dwTargetWidth );
}

void KGCSceneMenuBtn::FrameMove_ChangeWidth( DWORD dwTargetWidth )
{
    DWORD dwWidth = m_pkSubBox->GetWidth();
    dwWidth += GC_SCREEN_DIV_SIZE_INT(5);
    if( dwWidth > dwTargetWidth ) {
        dwWidth = dwTargetWidth;
        m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_MoveUpBtnsInit;
    }
    m_pkSubBox->SetWidth( dwWidth );

#if defined USE_COORDI_SHOP
    D3DXVECTOR2 vPos( static_cast<float>(655.f * GC_SCREEN_DIV_WIDTH + 15.f * GC_SCREEN_DIV_WIDTH - static_cast<float>(m_pkSubBox->GetWidth())/2) , 2.f * GC_SCREEN_DIV_WIDTH );
#else
    D3DXVECTOR2 vPos( static_cast<float>(600.f * GC_SCREEN_DIV_WIDTH - static_cast<float>(m_pkSubBox->GetWidth())/2) , 2.f * GC_SCREEN_DIV_WIDTH );
#endif
    
    m_pkSubBox->SetWindowPosDirect( vPos );
}

void KGCSceneMenuBtn::FrameMove_MoveUpBtnsInit()
{
    D3DXVECTOR2 vPos = m_pkSubBox->GetFixedWindowLocalPos();
    vPos.x += 4.f * GC_SCREEN_DIV_WIDTH;
    int iCount = 0;
    for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
        if( m_vecMiniBtns[i]->IsRenderOn() == false)
            continue;

        vPos.y = m_vecMiniBtns[i]->GetFixedWindowLocalPos().y;
        m_vecMiniBtns[i]->SetWindowPosDirect( vPos );
        vPos.x += 22.f * GC_SCREEN_DIV_WIDTH;
        ++iCount;
    }
    vPos = m_pkSubBox->GetFixedWindowLocalPos();
    vPos.y += 4.f * GC_SCREEN_DIV_WIDTH;

    m_pSubBoxFunc = boost::bind( &KGCSceneMenuBtn::FrameMove_MoveUpBtns, this, iCount, vPos );
}

void KGCSceneMenuBtn::FrameMove_MoveUpBtns( int iCount, D3DXVECTOR2 vPos )
{
    int iEndCount = 0;

    for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
        D3DXVECTOR2 vMiniPos = m_vecMiniBtns[i]->GetFixedWindowLocalPos();
        vMiniPos.y -= 3.f * GC_SCREEN_DIV_WIDTH;
        if( vMiniPos.y < vPos.y ) {
            vMiniPos.y = vPos.y;
            if( m_vecMiniBtns[i]->IsRenderOn() ) {
                m_vecMiniBtns[i]->SetSelfInputCheck( true );
                ++iEndCount;
            }
        }
        m_vecMiniBtns[i]->SetWindowPosDirect( vMiniPos );
    }

    if( iCount == iEndCount ) {
        m_pSubBoxFunc = NULL;
    }
}

void KGCSceneMenuBtn::FrameMove_HideStartInit()
{
    D3DXVECTOR2 vPos = m_pkSubBox->GetFixedWindowLocalPos();
    D3DXVECTOR2 vOrigin = vPos;

    for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
        m_vecMiniBtns[i]->SetSelfInputCheck( false );
    }    

    if( vPos.y >= 25.f * GC_SCREEN_DIV_WIDTH ) {
        m_pSubBoxFunc = NULL;
		m_pkSubBox->ToggleRender(false);
        for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
            m_vecMiniBtns[i]->ToggleRender( false );
        }
        return;
    }

    m_pSubBoxFunc = boost::bind( &KGCSceneMenuBtn::FrameMove_HideStart, this, vPos, vOrigin );
}

void KGCSceneMenuBtn::FrameMove_HideStart( D3DXVECTOR2 vPos, D3DXVECTOR2 vOrigin )
{
    vPos.y += 3.f * GC_SCREEN_DIV_WIDTH;
    m_pkSubBox->SetWindowPosDirect( vPos );

    for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
        D3DXVECTOR2 vMiniPos = m_vecMiniBtns[i]->GetFixedWindowLocalPos();
        vMiniPos.y += 3.f * GC_SCREEN_DIV_WIDTH;
        m_vecMiniBtns[i]->SetWindowPosDirect( vMiniPos );
    }

    if( ASMfabs( vOrigin.y - vPos.y ) >= 25.f * GC_SCREEN_DIV_WIDTH ) {
        m_pSubBoxFunc = NULL;
        for( int i =0 ; i < (int)m_vecMiniBtns.size() ; ++i ) {
            m_vecMiniBtns[i]->ToggleRender( false );
        }
        return;
    }
    m_pSubBoxFunc = boost::bind( &KGCSceneMenuBtn::FrameMove_HideStart, this, vPos, vOrigin );
}

void KGCSceneMenuBtn::FrameMoveInEnabledState()
{
    if( m_pSubBoxFunc != NULL ) {
        m_pSubBoxFunc(this);
    }

    for( int i = 0 ; i < (int)m_vecBtns.size() ; ++i ) {
        KD3DWnd* pMark = m_vecBtns[i]->GetChildByName( "mark" );
        if( pMark == NULL )
            continue;

        std::map<KD3DWnd::uid_type,std::pair<int,int>>::iterator mit;
        if( (mit = m_mapMarkAlpha.find(m_vecBtns[i]->GetUID() )) == m_mapMarkAlpha.end() )
            mit = m_mapMarkAlpha.insert( std::map<KD3DWnd::uid_type,std::pair<int,int>>::value_type( m_vecBtns[i]->GetUID(), std::pair<int,int>(0,8) ) ).first;

        mit->second.first += mit->second.second;
        if( mit->second.first >= 255 ) {
            mit->second.first = 255;
            mit->second.second *= -1;
        }
        else if( mit->second.first <= 0 ) {
            mit->second.first = 0;
            mit->second.second *= -1;
        }
        pMark->SetAllWindowAlpha( mit->second.first );
    }

	

	if (m_pkMenuCloseBtn->IsRenderOn())
	{
		if (m_pkFrame->CheckMousePosInWindowBound())
		{
			m_iCntMenuOpenTime = 0;
		}
		else
		{
			if (g_pkInput->BtnDown(KInput::MBLEFT))
				OnClickMenuClose();
			else
				m_iCntMenuOpenTime++;

		}

	}

	if (m_iCntMenuOpenTime > LIMIT_MENU_OPENTIME)
	{
		OnClickMenuClose();
	}
	
}

void KGCSceneMenuBtn::UpdateMarks()
{
    KD3DWnd* pWnd = GetChildByName( "skilltree_btn" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( g_MyD3D->AlertCheck_SkillTree() );
        }
    }
    pWnd = GetChildByName( "msn_button" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( g_MyD3D->AlertCheck_MSN() );
        }
    }
    pWnd = GetChildByName( "attendance_calendar_btn" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( g_MyD3D->AlertCheck_Calendar() );
        }
    }
    pWnd = GetChildByName( "couple_button" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( g_MyD3D->AlertCheck_CoupleWnd() );
        }
    }
    pWnd = GetChildByName( "mission_button" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( g_MyD3D->AlertCheck_Mission() );
        }
    }
    pWnd = GetChildByName( "menu_vip_button" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( true );
        }
    }

    pWnd = GetChildByName( "menu_event_minigame_banner_button" );
    if( pWnd != NULL ) {
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            // 이벤트 아이템 검사
            if( g_pItemMgr->HaveEventItem()
#if defined( USE_OLYMPIC_EVENT )
                || SiGCOlympicManager()->IsOlympicNotice()
#endif
                ){
                pWnd->ToggleRender( true );
            } else
                pWnd->ToggleRender( false );
        }
    }


#if defined( USE_SURVEY_SYSTEM )
    pWnd = GetChildByName( "menu_survey_button" );
    if( pWnd != NULL ) {
        pWnd->Lock( !g_MyD3D->AlertCheck_Survey() );
        pWnd = pWnd->GetChildByName( "mark" );
        if( pWnd ) {
            pWnd->ToggleRender( g_MyD3D->AlertCheck_Survey() );
        }
    }
#endif
}

void KGCSceneMenuBtn::CheckSubBoxOpen()
{
    if( m_pkMenuOpenBtn->IsRenderOn() ) {
        m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_OpenStart;
    }
    else if( m_pkMenuCloseBtn->IsRenderOn() ) {
        m_pSubBoxFunc = &KGCSceneMenuBtn::FrameMove_HideStartInit;
    }
}

void KGCSceneMenuBtn::EnableEventCreateInfo()
{
    if (g_pkUIScene->m_pkEventCreateInfoMsg == NULL)
        return;

    if( !g_pkUIScene->m_pkEventCreateInfoMsg->IsRenderOn() )
    {
        g_pkUIScene->m_pkEventCreateInfoMsg->SetWindowPosDirect( D3DXVECTOR2(490 * GC_SCREEN_DIV_WIDTH, 60 * GC_SCREEN_DIV_WIDTH) );
        g_pkUIScene->m_pkEventCreateInfoMsg->SetMsgType( KGCInventoryFullMsg::EVNET_CREATE_INFO );

        if( g_kGlobalValue.m_bEventCreateFirst )
        {
            g_pkUIScene->m_pkEventCreateInfoMsg->SetExtraValue1( 1 );
            g_pkUIScene->m_pkEventCreateInfoMsg->ToggleRender( true );
            g_pkUIScene->m_pkEventCreateInfoMsg->UpdateMsg();
        }
        else if ( g_kGlobalValue.m_bEventCreateEnable )
        {
            g_pkUIScene->m_pkEventCreateInfoMsg->SetExtraValue1( 2 );
            g_pkUIScene->m_pkEventCreateInfoMsg->ToggleRender( true );
            g_pkUIScene->m_pkEventCreateInfoMsg->UpdateMsg();
        }
    }
#ifndef DISABLE_MAP_SHOP // Kawan>
    if (g_MyD3D->m_pStateMachine->GetState() != GS_SHOP_CASH && g_MyD3D->m_pStateMachine->GetState() != GS_SHOP_GP && g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO && g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP 
#else
    if (g_MyD3D->m_pStateMachine->GetState() != GS_SHOP_CASH && g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO && g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP
#endif
        && g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE )
        g_pkUIScene->m_pkEventCreateInfoMsg->ToggleRender( false );
}

void KGCSceneMenuBtn::DisableEventCreateInfo()
{
    g_pkUIScene->m_pkEventCreateInfoMsg->ToggleRender( false );
}

void KGCSceneMenuBtn::AllRenderOff( )
{
    m_pkSubBox->ToggleRender( false );
    m_pkMenuOpenBtn->ToggleRender( false );
    m_pkMenuCloseBtn->ToggleRender( false );
    m_pkFrame->ToggleRender( false );

    std::vector<KD3DWnd*>::iterator vit = m_vecMiniBtns.begin();
    for( ; vit != m_vecMiniBtns.end() ; ++vit ) {
        (*vit)->ToggleRender( false );
    }

    std::vector<KD3DWnd*>::iterator vit2 = m_vecBtns.begin();
    for( ; vit2 != m_vecBtns.end() ; ++vit2 ) {
        (*vit2)->ToggleRender( false );
    }
}

void KGCSceneMenuBtn::AllRenderOn()
{
    m_pkMenuOpenBtn->ToggleRender( true );
    m_pkMenuCloseBtn->ToggleRender( true );
    m_pkFrame->ToggleRender( true );

	std::string strHideMenuList[] =
	{
		"menu_guild_button", 
		"skilltree_btn", 
		"square_button", 
		"menu_gacha_button", 
		"menu_vip_button", 
		"menu_party_button",
		"menu_event_minigame_banner_button", 
	};

	int nSize = sizeof( strHideMenuList ) / sizeof(std::string);

	std::vector<string> vecHideControl;
	
	for ( int i = 0; i < nSize; ++i ) 
	{
		vecHideControl.push_back( strHideMenuList[i] );
	}		


    //버튼 켜주기
    std::vector<KD3DWnd*>::iterator vit2 = m_vecBtns.begin();
    for( ; vit2 != m_vecBtns.end() ; ++vit2 ) 
	{
		//	감추어야할 항목
		if ( std::find( vecHideControl.begin(), vecHideControl.end(), (*vit2)->GetWindowName() ) != vecHideControl.end() )
		{
			(*vit2)->ToggleRender( false );
		}
		else 
		{
			(*vit2)->ToggleRender( true );
		}

		//	길드 광장이라면 길드 버튼 다시 켜준다.
		if ( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD && (*vit2)->GetWindowName() == "menu_guild_button" ) 
		{
			(*vit2)->ToggleRender( true );
		}

		//길드 광장아니라면 파티 버튼은 켜준다.
		if ( SiGCSquare()->GetSquareType() != KSquareInfo::ST_GUILD && (*vit2)->GetWindowName() == "menu_party_button" )
		{
			(*vit2)->ToggleRender( true );
		}
    }
}





void KGCSceneMenuBtn::RenderMenuOpenBtn()
{
    m_pkMenuOpenBtn->ToggleRender( true );
}



bool KGCSceneMenuBtn::IsInRoomEnableMini( const std::string& strWndName_ )
{
    // 대기방 안에서 떠도 되는 미니버튼인가?
    if( strWndName_ == "mini_menu_heroshop_button" ||           // 영던 상점
        strWndName_ == "mini_skilltree_btn" ||                  // 스킬 트리
        strWndName_ == "mini_couple_button" ||                  // 커플
        strWndName_ == "mini_attendance_calendar_btn" ||        // 출석부
        strWndName_ == "mini_mission_button" ||                 // 미션
        strWndName_ == "mini_msn_button" ||                     // 메신저
        strWndName_ == "mini_option_button" ||                  // 옵션
        strWndName_ == "mini_menu_event_banner_button" ||       // 이벤트 베너 
        strWndName_ == "mini_menu_gacha_button" ||              // 가챠 
        strWndName_ == "mini_menu_survey_button"  ) {           // 설문
            return true;
    }

    return false;
}

