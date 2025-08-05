#include "stdafx.h"
#include "GCScene.h"
//
#include "MyD3D.h"
//

#include "GCStateMachine.h"
#include "KDlgPrivateInfo.h"

//

#include "GCSKT.h"
#include "GCJustInTime.h"
#include "GCShopAdvertising.h"

std::set<KGCScene*>  KGCScene::ms_setScenes;

KGCScene::KGCScene(void)
{
    m_pkMainFrame = NULL;

    for( int i = 0; i < NUM_BUTTON; ++i )
        m_apkButton[i] = NULL;

    m_pkJustInTime_btn = NULL;

    LINK_CONTROL( "main_frame",                 m_pkMainFrame );

    LINK_CONTROL( "worldmap_button",            m_apkButton[BTN_WORLDMAP] );
    LINK_CONTROL( "match_button",               m_apkButton[BTN_MATCH] );
    LINK_CONTROL( "shop_button",                m_apkButton[BTN_SHOP] );
	LINK_CONTROL( "guild_match_button",         m_apkButton[BTN_GUILD_MATCH] );

#if defined USE_COORDI_SHOP
    LINK_CONTROL( "coordi_shop_btn",            m_apkButton[BTN_COORDI_SHOP] );
#endif
    LINK_CONTROL( "my_info_button",             m_apkButton[BTN_MY_INFO] );
    LINK_CONTROL( "skilltree_button",           m_apkButton[BTN_SKILL_TREE] );
    
    LINK_CONTROL( "just_in_time_btn",            m_pkJustInTime_btn );

    ms_setScenes.insert( this );
}

void KGCScene::OnCreate( void )
{

    for( int i = 0; i < NUM_BUTTON; ++i ) {
		m_apkButton[i]->InitState( true, true, this );
    }

#if defined ( NATION_PHILIPPINE )
    m_apkButton[BTN_GUILD_MATCH]->ToggleRender(false);
#endif

    g_kGlobalValue.m_kUserMission.RefreshUserMission();
    g_kGlobalValue.m_kUserCollectionMission.RefreshUserCollectionMission();
	g_kGlobalValue.m_kUserMission.LoadGuideExceptionMssion("GuideExceptionMissionList.lua");

    // Just in Time Event
    if( m_pkJustInTime_btn ){
        m_pkJustInTime_btn->InitState( false, true, this );
    }


#ifdef CALENDAR_RECOMMAND
    g_pkUIScene->m_pkAttendanceInfoMsg->SetWindowPosDirect( D3DXVECTOR2( 550.0f * GC_SCREEN_DIV_WIDTH, 70.0f * GC_SCREEN_DIV_WIDTH ) );
    g_pkUIScene->m_pkAttendanceInfoMsg->SetMsgType( KGCInventoryFullMsg::CALENDAR_INFO );
#endif

}	

void KGCScene::InitScene(void)
{

    m_apkButton[BTN_MATCH]->Lock(false);
    m_apkButton[BTN_WORLDMAP]->Lock(false);

    m_apkButton[BTN_SHOP]->Lock(false);

#if defined USE_COORDI_SHOP
    m_apkButton[BTN_COORDI_SHOP]->Lock(false);
#endif

    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_GUILD)) {
        m_apkButton[BTN_WORLDMAP]->Lock(true);
    }

    // just in time
#if defined ( USE_JUST_IN_TIME_EVENT )
    switch (g_kJustInTime.GetJustInTimeState())
    {
    case JIT_NO_MORE_EVENT:
        m_pkJustInTime_btn->ToggleRender(false);
        break;
    case JIT_CLICK_ON_TO_GET_REWARD:
    case JIT_WAIT_REWARD:
    case JIT_WAIT_NEXT_TIME:
        m_pkJustInTime_btn->ToggleRender(true);
#if defined ( NATION_USA )
        m_pkJustInTime_btn->SetToolTip(g_kJustInTime.GetJustInTimeToolTip(), (float)(m_pkJustInTime_btn->GetWidth() / 2), (float)(m_pkJustInTime_btn->GetHeight()), WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_RIGHT);
#elif defined( NATION_KOREA ) 
        m_pkJustInTime_btn->SetToolTip(g_kJustInTime.GetJustInTimeToolTip(), 0.0f, (float)(m_pkJustInTime_btn->GetHeight() * 1.5f), WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_NORMAL);
#elif defined( NATION_PHILIPPINE )
        m_pkJustInTime_btn->SetToolTip(g_kJustInTime.GetJustInTimeToolTip());
#else 
        m_pkJustInTime_btn->SetToolTip(g_kJustInTime.GetJustInTimeToolTip(), (float)(m_pkJustInTime_btn->GetWidth() / 2), (float)(m_pkJustInTime_btn->GetHeight()), WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_LEFT);
#endif
        break;
    }
#endif

    if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM) {
        m_pkJustInTime_btn->ToggleRender(false);
    }
}

KGCScene::~KGCScene(void)
{
    ms_setScenes.erase( this );
}

void KGCScene::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
        case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); return;
    }

	if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;

    for( int i = 0; i < NUM_BUTTON; ++i )
    {
        if( event.m_pWnd == m_apkButton[i] )
        {
            switch( i )
            {
                case BTN_WORLDMAP:
                    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_FREE) || g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO)) {
                        g_MyD3D->OnMatchButton();
						return;
					}

					g_MyD3D->m_pStateMachine->GoWorldMap();
                    return;
                case BTN_MATCH:
				case BTN_GUILD_MATCH:
                    g_MyD3D->OnMatchButton();
                    return;
                case BTN_SHOP :

                    if( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_CLEAR_PRIVATE_DUNGEON ));
                        return;
                    }

                    g_MyD3D->m_pStateMachine->GoCashShop();
                    return;

#if defined USE_COORDI_SHOP
                case BTN_COORDI_SHOP:

                    if( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_CLEAR_PRIVATE_DUNGEON ));
                        return;
                    }

                    g_MyD3D->m_pStateMachine->GoCoordiShop();
                    return;
#endif

                case BTN_MY_INFO:
                    g_MyD3D->m_pStateMachine->GoMyInfo();
                    return;
                case BTN_SKILL_TREE:
                    g_MyD3D->OnSkillTreeButton();
                    return;
                default:
                    break;
            }
        }
    }

    if ( event.m_pWnd == m_pkJustInTime_btn )
    {
        ActiveJustInTime();        
        return;
    }
}

void KGCScene::SetButtonLock( ButtonType eButton, bool bLock, std::wstring strToolTip )
{
    m_apkButton[eButton]->Lock( bLock );

    if( strToolTip.empty() == false )
        m_apkButton[eButton]->SetToolTip( strToolTip, m_apkButton[eButton]->GetToolTipOffset().x, m_apkButton[eButton]->GetToolTipOffset().y );
}

void KGCScene::SetButtonRender( ButtonType eButton, bool bRender )
{
    m_apkButton[eButton]->ToggleRender( bRender );
}


void KGCScene::SetAlertButtonRender( AlertButtonType eButton, bool bRender )
{
    m_apkAlertButton[eButton]->ToggleRender( bRender );
}

void KGCScene::ShowHotItemType( DWORD _dwType )
{
#if defined( USE_NEW_ITEM_CHECK ) 
    if ( m_apkHotItem[EM_HOT_ITEM_WEAPON] ) {
        if ( _dwType & EM_HOT_ITEM_WEAPON_TYPE ) {
            m_apkHotItem[EM_HOT_ITEM_WEAPON]->ToggleRender( true );
        }   
        else {
            m_apkHotItem[EM_HOT_ITEM_WEAPON]->ToggleRender( false );
        }
    }

    if ( m_apkHotItem[EM_HOT_ITEM_ARMOR] ) {
        if ( _dwType&EM_HOT_ITEM_ARMOR_TYPE ) {
            m_apkHotItem[EM_HOT_ITEM_ARMOR]->ToggleRender( true );
        }
        else {
            m_apkHotItem[EM_HOT_ITEM_ARMOR]->ToggleRender( false );
        }
    }

    if ( m_apkHotItem[EM_HOT_ITEM_BOX] ) {
        if ( _dwType&EM_HOT_ITEM_BOX_TYPE ) {
            m_apkHotItem[EM_HOT_ITEM_BOX]->ToggleRender( true );
        }
        else {
            m_apkHotItem[EM_HOT_ITEM_BOX]->ToggleRender( false );
        }
    }

    for( int i=0 ; i<EM_HOT_ITEM_SIZE ; ++i ) { 
        SetHotItemTypePos( i );
    }
#endif
}

void KGCScene::SetHotItemTypePos( int iIndex_ )
{
    // 위치 조정 
    const static float fStartPosX = 533.0f;
    const static float fStartPosXAtRoom = 450.0f;
    const static float fPosY = 8.0f;
    const static float fXGap = 27.0f;

    //float fScaleX = g_pGCDeviceManager2->GetWindowScaleX();
    //float fScaleY = g_pGCDeviceManager2->GetWindowScaleY();

    int iCount = 0;
    for( int i=0 ; i<iIndex_ ; ++i ) { 
        if( m_apkHotItem[ i ] && !m_apkHotItem[ i ]->IsRenderOn() ) { 
            ++iCount;
        }
    }

    int iShowIndex = iIndex_ - iCount;
    if( m_apkHotItem[ iIndex_ ] ) { 
        D3DXVECTOR2 dxvPos;

        if( g_MyD3D->m_pStateMachine->GetState() != GS_ROOM ) { 
            dxvPos.x = fStartPosX + ( fXGap * static_cast< float >( iShowIndex ) );
        }
        else {
            dxvPos.x = fStartPosXAtRoom + ( fXGap * static_cast< float >( iShowIndex ) );
        }


        dxvPos.y = fPosY;
        m_apkHotItem[ iIndex_ ]->SetFixedWindowLocalPos( dxvPos );
    }
}

void KGCScene::FrameMoveInEnabledState()
{
#if defined (USE_JUST_IN_TIME_EVENT)
    JustInTimeFrameMove();
#endif
}


void KGCScene::BindToLua()
{
    lua_tinker::class_add<KGCScene>( KGCLuabinder::getInstance().GetLuaState(), "KGCScene" );
    lua_tinker::class_inh<KGCScene,KD3DWnd>( KGCLuabinder::getInstance().GetLuaState() );
}

void KGCScene::EnterScene()
{
    std::set<KGCScene*>::iterator vit;
    for( vit = ms_setScenes.begin() ; vit != ms_setScenes.end() ; ++vit )
    {
        KGCScene* pScene = (*vit);
        pScene->ToggleRender( pScene == this );
    }

    InitScene();
    Initialize();
    OnEnterScene();
    g_pkUIScene->SetReverse( false );
    KGCPC_MEMO( "Scene Chanaged to " << GetWindowName() );
    g_pkUIMgr->SendEvent( KActionEvent( this, KD3DWnd::D3DWE_SCENE_CHANGE ) );

    // 상점 알리미
//    g_kShopAdvertising.CreatePaticles();
	g_KDSound.Stop("alram");

#if defined( USE_GUIDE_SYSTEM )
    // 가이드 상태조건 갱신
    g_MyD3D->m_kGuideMgr.CheckEnableGuide();
    g_pkUIScene->m_pkGCGuide->ToggleRender( false );
#endif

    // 반갑다 친구야 추천
#if defined (NATION_BRAZIL) || defined ( NATION_KOREA ) || defined ( NATION_TAIWAN ) || defined ( NATION_IDN )
    if( ( g_pkUIScene->m_pkNickNameBox->GetRecommendRegisterComplete() == false && 
          g_MyD3D->m_pStateMachine->GetState() != GS_CHAR_SELECT &&
          g_MyD3D->m_pStateMachine->GetState() != GS_CHAR_GAIN ) && 
          g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_TUTORIAL_END &&
        ( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_CHAR_SELECT ||        
          g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_CHAR_GAIN ||
          g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ) ) { 
            g_pkUIScene->m_pkNickNameBox->SetRecommendRegisterComplete( false );
            g_pkUIScene->m_pkNickNameBox->SetChangeNickName(false);
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME );
    }
#else
    if( g_pkUIScene->m_pkNickNameBox->GetRecommendRegisterComplete() == false &&
        g_MyD3D->m_pStateMachine->GetState() != GS_CHAR_SELECT &&
        g_MyD3D->m_pStateMachine->GetState() != GS_CHAR_GAIN ) {
        g_pkUIScene->m_pkNickNameBox->SetRecommendRegisterComplete( true );
        g_pkUIScene->m_pkNickNameBox->SetChangeNickName(false);
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME );
    }
#endif
    if(g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO  ||
       g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO_FROM_ROOM) {
        if( g_pkUIScene->m_pkAddItemInClient ) { 
            g_pkUIScene->m_pkAddItemInClient->ToggleRender(false);
        }
    }
#ifndef __PATH__
    else { 
        if( g_pkUIScene->m_pkAddItemInClient ) { 
            g_pkUIScene->m_pkAddItemInClient->ToggleRender( true );
        }
    }
#endif
    // 상점 알리미
//    g_kShopAdvertising.CreatePaticles();
    // 알리미 (우편함 등)
    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP ||
        g_MyD3D->m_pStateMachine->GetState() == GS_MATCH ||
        g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH ||
        g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP ||
        g_MyD3D->m_pStateMachine->GetState() == GS_COORDI_SHOP ||
        g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO ||
        g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ||
        g_MyD3D->m_pStateMachine->GetState() == GS_ROOM ) {
        if( g_pkUIScene->m_pkSceneMenuBtnS6->IsUpPostBtn() && SiKGCPostLetterManager()->GetTotalNotReadLetterCount())
            g_pkUIScene->m_pkSceneMenuBtnS6->ShowNotice( true );
    } else {
        g_pkUIScene->m_pkSceneMenuBtnS6->ShowNotice( false );
    }

    if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
    {
        if( g_kGlobalValue.GetOpenedEventDungeonInfo().empty() )
            g_pkUIScene->m_pkWorldMapContainer->RenderEventDungeonIcon( false );
        else
            g_pkUIScene->m_pkWorldMapContainer->RenderEventDungeonIcon( true );
    }
    SiKGCPrivateDungeonManager()->CheckNextLevel();

}

void KGCScene::LeaveScene( void )
{
    OnLeaveScene();
}

void KGCScene::Destroy( void )
{
#ifdef CALENDAR_RECOMMAND
	g_pkUIScene->m_pkAttendanceInfoMsg->ToggleRender( false );
#endif
}

void KGCScene::PostChildDraw( void )
{
    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_FREE ) ){
        g_ParticleManager->Render( GC_LAYER_UI_SCENE );
        g_ParticleManager->Render( GC_LAYER_CUSTOM2 );
    }
    g_ParticleManager->Render( GC_LAYER_CUSTOM2 );
}


void KGCScene::JustInTimeFrameMove()
{
    static int STOP_RATE = 2;
    static int COOL_TIME  = 200;
    static int DELAY_TIME = 2;

    static int m_iTremorFrame = 0;
    static int m_iCoolTime = 0;

    // Just in time 이벤트 툴팁 갱신
#if defined ( NATION_USA )
    m_pkJustInTime_btn->SetToolTip( g_kJustInTime.GetJustInTimeToolTip(), (float)(m_pkJustInTime_btn->GetWidth() / 2), (float)(m_pkJustInTime_btn->GetHeight()), WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_RIGHT );
#elif defined( NATION_KOREA ) 
    m_pkJustInTime_btn->SetToolTip( g_kJustInTime.GetJustInTimeToolTip(), 0.0f, (float)(m_pkJustInTime_btn->GetHeight() * 1.5f ), WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_NORMAL );
#elif defined( NATION_PHILIPPINE )
    m_pkJustInTime_btn->SetToolTip( g_kJustInTime.GetJustInTimeToolTip() );	
#else 
    m_pkJustInTime_btn->SetToolTip( g_kJustInTime.GetJustInTimeToolTip(), (float)(m_pkJustInTime_btn->GetWidth() / 2), (float)(m_pkJustInTime_btn->GetHeight()), WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_LEFT );
#endif

    // 각 모드를 번갈아 가면서 출력하여 흔들기 표현
    // 0 : 중앙
    // 1 : 오른쪽
    // 2 : 중앙
    // 3 : 왼쪽
    // D3DWM_HOVER  <-> D3DWM_DEFAULT <-> D3DWM_SELECT

    if( m_iCoolTime == 0 && g_kJustInTime.GetJustInTimeState() == JIT_CLICK_ON_TO_GET_REWARD ){
        if ( m_iTremorFrame < DELAY_TIME ){
            g_KDSound.Play("alram", 1);
            m_pkJustInTime_btn->SetWndMode( D3DWM_HOVER );
        }
        else if ( m_iTremorFrame < DELAY_TIME * 2 )
            m_pkJustInTime_btn->SetWndMode( D3DWM_DEFAULT );
        else if ( m_iTremorFrame < DELAY_TIME * 3 )
            m_pkJustInTime_btn->SetWndMode( D3DWM_SELECT );
        else if ( m_iTremorFrame < DELAY_TIME * 4 ) {
            m_pkJustInTime_btn->SetWndMode( D3DWM_DEFAULT );
            m_iTremorFrame = 0;
            g_KDSound.Stop("alram");

            if ( rand()%100 < STOP_RATE )
                m_iCoolTime = COOL_TIME;
        }

        ++m_iTremorFrame;
    }
    else{
        m_pkJustInTime_btn->SetWndMode( D3DWM_DEFAULT );
        g_KDSound.Stop("alram");
    }

    if( m_iCoolTime > 0)
        --m_iCoolTime;
}

void KGCScene::ActiveJustInTime()
{
    // 보상 받을 수 있는 상태가 아니면 버튼 눌러봐야.. 소용없다.
	if ( g_kJustInTime.GetJustInTimeState() != JIT_CLICK_ON_TO_GET_REWARD )
    	return;

    if ( GC_GLOBAL_DEFINE::bJustInTimeSearch ) 
    {
        GCITEMID _itemID = 0;
		std::vector<KDropItemInfo>& vecRewardList = g_kJustInTime.GetRewardList();
		if ( !vecRewardList.empty() ) 
		{
			KDropItemInfo kItemInfo = *vecRewardList.begin();
			_itemID = kItemInfo.m_ItemID / 10;
		}

        time_t tEventTime = g_kJustInTime.GetJustInTimeNextEventTime();
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_HOT_TIME_SEARCH_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, _itemID, tEventTime );
    }
    else
    {
        ReqJustIntimeReward();
    }

    return;
}

void KGCScene::ReqJustIntimeReward()
{
    KEVENT_JUST_IN_TIME_REWARD_REQ  kPacket;

    kPacket = g_kJustInTime.GetJustInTimeNextEventTime();

    KP2P::GetInstance()->Send_EventJustInTimeRewardReq( kPacket );

    g_kJustInTime.SetJustInTimeState( JIT_WAIT_REWARD );
    g_kJustInTime.CreateJustInTimeToolTip();

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_JUST_IN_TIME_MESSAGE1 ) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, false, false, false);
}