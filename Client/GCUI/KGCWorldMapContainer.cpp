#include "stdafx.h"
#include "../mission/GCMissionManager.h"
#include "KGCWorldMapContainer.h"
#include "GCDungeonMissionList.h"
#include "KGCWorldMapPanel.h"
#include "MyD3D.h"

#include "GCStateMachine.h"
//

#include "GCChatBox.h"
#include "KDInput.h"
#include "KMci.h"
//
//
#include "KGCRoomManager.h"

#include "KGCChatScrollBackground.h"
#include "KGCRoomListManager.h"
#include "GCSongkranWaterEffect.h"
#include "GCSongkranEventManager.h"
#include "GCUI/GCChatBoxDlgS6.h"
#include "GCVitalityWorldmap.h"


IMPLEMENT_CLASSNAME( KGCWorldMapContainer );
IMPLEMENT_WND_FACTORY( KGCWorldMapContainer );
IMPLEMENT_WND_FACTORY_NAME( KGCWorldMapContainer, "gc_worldmap_container" );

KGCWorldMapContainer::KGCWorldMapContainer( void )
{
    m_pkUpperMapButton  = NULL;

#ifdef DIMENSIONAL_BREAK
    m_pkNewMapButton = NULL;
#endif

    //m_pkHellModeButton  = NULL;
    //m_pkUpperMapArrow   = NULL;

	//m_pkChangeViewBtn = NULL;
    m_pkDungeonMissionList = NULL;

	m_bDragWindowCheck            = false;
	m_pkWaterEffect = NULL;
	m_pkBackGroundDeco = NULL;
    m_pkBackgoundBlack = NULL;
    m_pkNewContinentNotice = NULL;
    m_pkHeroDungeon = NULL;
    m_pkEventDungeon = NULL;
    m_pkHeroShop = NULL;
    m_pkHeroShopName = NULL;

    m_pkCollection = NULL;
    m_pkCollectionName = NULL;

    for( int i = 0; i < NUM_WORLDMAP; ++i )
        m_apkWorldMapPanel[i] = NULL;

    LINK_CONTROL( "upper_map_button",       m_pkUpperMapButton );

#ifdef DIMENSIONAL_BREAK
    LINK_CONTROL("new_map_button", m_pkNewMapButton);
#endif

    //LINK_CONTROL( "hellmode_btn",           m_pkHellModeButton );
    //LINK_CONTROL( "hellmode_arrow",         m_pkHellModeArrow );
    LINK_CONTROL( "dungeon_mission_list",   m_pkDungeonMissionList );
    LINK_CONTROL( "background3",            m_pkBackGroundDeco );

	//LINK_CONTROL("chatbox_downBtn" ,        m_pkChangeViewBtn );  
	LINK_CONTROL("water_effect",			m_pkWaterEffect );
    LINK_CONTROL("background_black" , m_pkBackgoundBlack );
    LINK_CONTROL("new_continent" , m_pkNewContinentNotice );


    LINK_CONTROL("hero_dungeon", m_pkHeroDungeon );
    LINK_CONTROL("event_dungeon", m_pkEventDungeon );
    LINK_CONTROL("hero_name", m_pkHeroName);
    LINK_CONTROL("event_name", m_pkEventName );
    LINK_CONTROL("hero_shop", m_pkHeroShop );
    LINK_CONTROL("hero_shop_name", m_pkHeroShopName);

    LINK_CONTROL("collection", m_pkCollection );
    LINK_CONTROL("collection_name", m_pkCollectionName );

    char strWndName[MAX_PATH] = {0,};
    for( int i = 0; i < NUM_WORLDMAP; ++i )
    {
        sprintf( strWndName, "map%02d", i );
        LINK_CONTROL( strWndName, m_apkWorldMapPanel[i] );
    }

    m_iEffectRenderDelay = 0;

    m_pkVitalityExpInfo = NULL;
    LINK_CONTROL("worldmap_vitality" , m_pkVitalityExpInfo );
}

KGCWorldMapContainer::~KGCWorldMapContainer( void )
{
    // g_ParticleManager 에서 알아서 게임 종료시 다 지우므로..
    // 여기서는 그냥 vector만 클리어 하자.. ( 정말?? )
    m_vecMapEffect.clear();
}

void KGCWorldMapContainer::OnCreate()
{
    for( int i = 0; i < NUM_WORLDMAP; ++i )
        m_apkWorldMapPanel[i]->InitState( false );

    m_pkUpperMapButton->InitState( true, true, this );

#ifdef DIMENSIONAL_BREAK
    m_pkNewMapButton->InitState(true, true, this);
#endif

	if( m_pkDungeonMissionList != NULL )
	    m_pkDungeonMissionList->InitState( false, true );


	if ( m_pkWaterEffect ) 
	{
		m_pkWaterEffect->InitState( false );
	}

	if ( m_pkBackgoundBlack )
	    m_pkBackgoundBlack->InitState(false);
    
	if ( m_pkNewContinentNotice )
		m_pkNewContinentNotice->InitState(false);

    if( m_pkVitalityExpInfo )
        m_pkVitalityExpInfo->InitState( false, true, this );
    if ( m_pkHeroDungeon )
        m_pkHeroDungeon->InitState( true, true, this );
    if ( m_pkEventDungeon )
        m_pkEventDungeon->InitState( true, true, this );
    if ( m_pkHeroName )
        m_pkHeroName->InitState( true, true, this );
    if ( m_pkEventName )
        m_pkEventName->InitState( true, true, this );

    if( m_pkHeroShop )
        m_pkHeroShop->InitState( true, true, this );
    if( m_pkHeroShopName )
        m_pkHeroShopName->InitState( true, true, this );

    if ( m_pkCollection )
        m_pkCollection->InitState( true, true, this );
    if ( m_pkCollectionName )
        m_pkCollectionName->InitState( true, true, this );

}
void KGCWorldMapContainer::RenderEventDungeonIcon( bool bRender )
{
    m_pkEventDungeon->ToggleRender( bRender );
    m_pkEventName->ToggleRender( bRender );
}
void KGCWorldMapContainer::ReLocateChatBox()
{
	D3DXVECTOR2 vPos;

//#if !defined( NATION_IDN )
//	vPos.x = m_pkChangeViewBtn->GetFixedWindowPos().x;
//#endif
    vPos.y = static_cast<FLOAT>(g_pkWorldmapChatBox->GetScrollHeight() + g_pkWorldmapChatBox->GetScrollBtnGap()*3);
}

void KGCWorldMapContainer::OnPostChildInitialize()
{
	m_bDragWindowCheck       = false;
}

void KGCWorldMapContainer::ActionPerformed( const KActionEvent& event )
{
    
    switch( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); /*return;*/
        case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); /*return;*/
    }

    if( event.m_pWnd == m_pkHeroShop )
    {
        PLAYER* pPlayer = NULL;
        pPlayer = &g_MyD3D->m_TempPlayer;
        
        if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            g_kGlobalValue.isCurrentShopVIP = false;
            g_pkUIScene->UpdateAllHeroShop();
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_HERODUNGEON_SHOP_DLG_S6);
        }
    }
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
#ifdef DIMENSIONAL_BREAK
        if (event.m_pWnd == m_pkNewMapButton)
        {
            SiKGCWorldMapManager()->SetCurrentWorldMap( WORLDMAP_NEWMAP );
            UpdateWorldMap();
            EventNewContinent(false);
        }
#endif
        if( event.m_pWnd == m_pkUpperMapButton )
        {
            SiKGCWorldMapManager()->SetCurrentWorldMap( WORLDMAP_UPPERMAP );
            UpdateWorldMap();
            EventNewContinent(false);
        }
        if( event.m_pWnd == m_pkHeroDungeon )
        {
            if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_HERO_DUNGEON_SEASON5 );
        }
        if( event.m_pWnd == m_pkEventDungeon )
        {
            if( SiKGCWorldMapManager()->IsExistEventDungeon() && ( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP ))
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_DUNGEON_SEASON5 );
            
        }
        if ( event.m_pWnd == m_pkCollection )
        {
            if ( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ALL_COLLECTION_DLG );
            }
        }
    }
}

void KGCWorldMapContainer::FrameMoveInEnabledState( void )
{
	
    static int iAlpha = 0;
    static bool bIncrease = true;

    if ( bIncrease )
    {
        iAlpha += 4;
        if ( iAlpha >= 255 )
        {
            bIncrease = false;
            iAlpha = 255;
        }
    }
    else
    {
        iAlpha -= 2;
        if ( iAlpha <= 0 )
        {
            bIncrease = true;
            iAlpha = 0;
        }
    } 


//zstaiji - 사내섭일 경우 A키를 누르면 레벨 제한을 푼다.
#if !defined( __PATH__ )
    if( g_pkInput->IsUp( DIK_A ) )
    { 
		g_kGlobalValue.SetUnlock(!g_kGlobalValue.IsUnlock());
		g_kGlobalValue.m_iUseHellModeBtn = g_kGlobalValue.IsUnlock();
		SiKGCWorldMapManager()->UpdateWorldMap();
		UpdateWorldMap();
    }
#endif;
        //SiKGCWorldMapManager()->UpdateRoomCount();
}

void KGCWorldMapContainer::RenderMapEffect( bool bRender )
{
    m_iEffectRenderDelay = 0;

    for( int i = 0; i < (int)m_vecMapEffect.size(); ++i )
    {
        if( NULL != m_vecMapEffect[i] ) { 
            m_vecMapEffect[i]->SetShow( bRender ) ;
        }
    }
}

void KGCWorldMapContainer::GoTo(Mapas ID) {
    switch (ID) {
    case Mapas::Ernas:
        SiKGCWorldMapManager()->SetCurrentWorldMap(WORLDMAP_UPPERMAP);
        break;
    case Mapas::Elyos:
        SiKGCWorldMapManager()->SetCurrentWorldMap(WORLDMAP_ELYOS);
        break;
    }
    UpdateWorldMap();
    EventNewContinent(false);
}

void KGCWorldMapContainer::UpdateWorldMap(bool bReset)
{
	if(bReset)
	{
		SiKGCWorldMapManager()->SetCurrentSelectDungeon(-1);			
	}


#ifndef DIMENSIONAL_BREAK
    if (!SiKGCWorldMapManager()->CheckContinentEnable(SiKGCWorldMapManager()->GetCurrentWorldMap()))
    {
        SiKGCWorldMapManager()->SetCurrentWorldMap(SiKGCWorldMapManager()->GetPlayableWorldMap());
    }
#endif
    
    int iCurrentWorldMap = static_cast<int>(SiKGCWorldMapManager()->GetCurrentWorldMap());

    // 현재 월드맵이 보이도록 설정한다!
    for( int i = 0; i < NUM_WORLDMAP; ++i )
        m_apkWorldMapPanel[i]->ToggleRender( false );
    
    m_apkWorldMapPanel[ iCurrentWorldMap ]->ToggleRender( true );

    //현재 대륙이 프로슬란드/ 트리비아면 
    if( iCurrentWorldMap ==  WORLDMAP_FLOSLAND )
    {
        m_pkHeroDungeon->ToggleRender(false );
        m_pkEventDungeon->ToggleRender(false );
        m_pkHeroName->ToggleRender(false );
        m_pkEventName->ToggleRender(false );
        m_pkHeroShop->ToggleRender(false );
        m_pkHeroShopName->ToggleRender(false );
        m_pkCollection->ToggleRender(false );
        m_pkCollectionName->ToggleRender(false );
    }
    else { 
        m_pkHeroDungeon->ToggleRender(true );
        m_pkHeroName->ToggleRender(true );
        m_pkHeroShop->ToggleRender(true );
        m_pkHeroShopName->ToggleRender(true );
        m_pkCollection->ToggleRender(true );
        m_pkCollectionName->ToggleRender(true );
        
        //캐릭터 전용 대륙이 아닐때, 오픈중인 이벤트 던전이 있으면 이벤트 던전 아이콘을 그려준다.
        if( !g_kGlobalValue.GetOpenedEventDungeonInfo().empty() )
            RenderEventDungeonIcon(true);
        else
            RenderEventDungeonIcon(false);
    }

    // 영던 상점버튼 Lock 처리 
    if( g_MyD3D->m_TempPlayer.GetCurrentCharLevel() < HERO_SHOP_ENABLE_LV ) { 
        m_pkHeroShop->SetWndMode( KD3DWnd::D3DWM_LOCK );
        m_pkHeroShopName->SetWndMode( KD3DWnd::D3DWM_LOCK );
        m_pkHeroShop->SetToolTip(g_pkStrLoader->GetString( STR_ID_TOOLTIP_HEROSHOP_LIMIT_LEVEL ),90.f);
    }
    else { 
        m_pkHeroShop->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
        m_pkHeroShopName->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
        m_pkHeroShop->ClearToolTip();
    }

#ifdef DIMENSIONAL_BREAK
    bool bShowNewMapButton = (iCurrentWorldMap == WORLDMAP_UPPERMAP || iCurrentWorldMap == WORLDMAP_ELYOS);
    m_pkNewMapButton->ToggleRender(bShowNewMapButton);
#endif

    // 상위 월드맵 버튼
    bool bShowUpperWorldmapButton = ( iCurrentWorldMap != WORLDMAP_UPPERMAP );
	
    // 상위 월드맵에 갈수 없다면 해당 UI 표시 하지 말자.
    if( !SiKGCWorldMapManager()->CheckContinentEnable( WORLDMAP_UPPERMAP ) )
    {
        bShowUpperWorldmapButton = false;
    }

#ifdef DIMENSIONAL_BREAK
    if (iCurrentWorldMap == WORLDMAP_NEWMAP || iCurrentWorldMap == WORLDMAP_ELYOS) {
        bShowUpperWorldmapButton = false;
    }
#endif

    m_pkUpperMapButton->ToggleRender( bShowUpperWorldmapButton );
    if (m_pkBackGroundDeco != NULL)
        m_pkBackGroundDeco->ToggleRender( !bShowUpperWorldmapButton );

    //일반적인 경로로 월드맵에 진입하는 경우
    if ( iCurrentWorldMap != WORLDMAP_UPPERMAP &&  g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP) {
#if defined (USE_NEWBIE_GUIDE)
		KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ iCurrentWorldMap ];
        pkWorldMapPanel->SetDungeonName();
#endif
		g_pkUIScene->m_pkChatBoxDlgS6->RenderOn();
    }
    //그외의 모든경우
    else
    {
        //던전 룸에서 다른던전 가기로 이동할 경우 게임 상태를 GS_ROOM으로 인식한다. 
        if( iCurrentWorldMap != WORLDMAP_UPPERMAP && g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        {
#if defined (USE_NEWBIE_GUIDE)
        KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ iCurrentWorldMap ];
        pkWorldMapPanel->SetDungeonName(); 
#endif
        }
		g_pkUIScene->m_pkChatBoxDlgS6->RenderOff();
    }

    if ( iCurrentWorldMap == WORLDMAP_UPPERMAP )
    {
        KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ WORLDMAP_UPPERMAP ];
        
        if( NULL != pkWorldMapPanel)
        {
            pkWorldMapPanel->ShowAvailableContinentIcon();
            pkWorldMapPanel->ShowPreContinentCharIcon();
            pkWorldMapPanel->ShowContinentQuestIcon();
        }
    }
    else
    {
        g_kGlobalValue.SetPreContinent( (int)SiKGCWorldMapManager()->GetCurrentWorldMap());
    }
     // 아이콘의 트리구조나 등등을 업데이트 함
	RefreshOpenedContinent();
    SiKGCWorldMapManager()->UpdateWorldMap();

    //헬모드관련 세팅
    if ( SiKGCRoomListManager()->IsHellMode() )
    {
        SiKGCRoomManager()->SetSubGameMode( GC_SGM_DUNGEON_HELL );
    }
    else if (SiKGCRoomListManager()->IsBreakMode())
    {
        SiKGCRoomManager()->SetSubGameMode( GC_SGM_DUNGEON_BREAK );
    }
    else
    {
        SiKGCRoomManager()->SetSubGameMode(GC_SGM_DUNGEON_NORMAL);
    }


    if( SiKGCRoomListManager()->IsHellMode() )
    {
        if( g_KMci.GetTrack() != GC_BGM_WORLDMAP_HELL )
            g_KMci.Play( GC_BGM_WORLDMAP_HELL, true );
    }
    else if (SiKGCRoomListManager()->IsBreakMode())
    {
        if (g_KMci.GetTrack() != GC_BGM_WORLDMAP_BREAK)
            g_KMci.Play(GC_BGM_WORLDMAP_BREAK, true);
    }
    else
    {
        int iBGMIndex;
        EGCWorldMap eWorldMap = SiKGCWorldMapManager()->GetCurrentWorldMap();
        iBGMIndex = SiKGCWorldMapManager()->GetWorldMapBgm( eWorldMap );
		
		if(g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
			if( g_KMci.GetTrack() != iBGMIndex )
				g_KMci.Play( iBGMIndex, true );
    }      


    // 맵 파티클 : UI 개편되면서 새롭게 작업된 Map파티클 입니다.
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
    {
        CreateMapParticle();
    }

//     if ( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
//     {
//         CreateMapEffect();
//     }   

    // 메세지 박스가 떠 있으면 끈다!
//     if( g_pkUIScene->IsMsgBoxModal() )
//         RenderMapEffect( false );

    //헬 모드가 가능한지 확인하고 버튼 상태를 바꿔줌... 현재는 신형장인의 목검 이 잇으면 ㅡ 
    //m_pkHellModeButton->SetWndMode( g_MyD3D->m_kItemMgr.FindInventoryForItemID( 75 )?D3DWM_DEFAULT:D3DWM_LOCK );

}


void KGCWorldMapContainer::ADD_WORLDMAP_PARTICLE( std::string seq,float x/*=0.0f*/,float y/*=0.0f*/ )
{
    CParticleEventSeqPTR pParticle = NULL;
    pParticle = g_ParticleManager->CreateSequence( seq, x, y, 0.5f );
    if( pParticle )
    {
        g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
        m_vecMapEffect.push_back( pParticle );
    }
}

void KGCWorldMapContainer::CreateMapParticle()
{
    // 파티클 제거
    ClearMapEffect();

    // 대륙 파티클
    std::vector<SWorldMapParticle> vecMapParticle = SiKGCWorldMapManager()->GetMapParticleInfo( SiKGCWorldMapManager()->GetCurrentWorldMap() );
    std::vector<SWorldMapParticle>::iterator vecMapIter = vecMapParticle.begin();
    for(; vecMapIter != vecMapParticle.end(); ++vecMapIter )
    {
        ADD_WORLDMAP_PARTICLE( vecMapIter->m_strName.c_str(), vecMapIter->m_vPos.x, vecMapIter->m_vPos.y );
    }

    // 던전 파티클
    std::vector< EGCGameMode > vecInfo = SiKGCWorldMapManager()->GetCurWorldMapInfo();
    std::vector< EGCGameMode >::iterator vecIter = vecInfo.begin();
    for(; vecIter != vecInfo.end(); ++vecIter )
    {
        if ( !SiKGCRoomManager()->IsAvailableGameMode( GC_GMC_DUNGEON, *vecIter ) )
            continue;

        std::vector<SWorldMapParticle> vecDungeonParticle = SiKGCWorldMapManager()->GetDungeonParticle( *vecIter, SiKGCWorldMapManager()->GetCurrentWorldMap() );
        std::vector<SWorldMapParticle>::iterator vecDungeonIter = vecDungeonParticle.begin();
        for(; vecDungeonIter != vecDungeonParticle.end(); ++vecDungeonIter )
        {
            ADD_WORLDMAP_PARTICLE( vecDungeonIter->m_strName.c_str(), vecDungeonIter->m_vPos.x, vecDungeonIter->m_vPos.y );
        }
    }

#if !defined(NATION_IDN)
    //	송크란 이벤트 중이라면...
    if ( SiGCSongkranEventManager()->IsEventOpen() ) 
    {
        ADD_WORLDMAP_PARTICLE( "UI_water_01" );
        ADD_WORLDMAP_PARTICLE( "UI_water_02" );

        if ( m_pkWaterEffect )
            m_pkWaterEffect->ToggleRender( true );
    }
#endif
}

void KGCWorldMapContainer::CreateMapEffect()
{
    ClearMapEffect();

    float x=0.06f;
    float y=0.1f;


    // 던전이 오픈되던 말던 붙혀줘야할 파티클은 아래에
    switch( SiKGCWorldMapManager()->GetCurrentWorldMap() )
    {
    //case WORLDMAP_BERMESIA:
    //    {
    //        ADD_WORLDMAP_PARTICLE( "map01_elf01" );
    //        ADD_WORLDMAP_PARTICLE( "map01_smog01" );
    //        ADD_WORLDMAP_PARTICLE( "map01_smog02" );
    //        ADD_WORLDMAP_PARTICLE( "map01_smog03" );
    //        ADD_WORLDMAP_PARTICLE( "map01_snow");
    //        break;
    //    }
    case WORLDMAP_ELIA:
        {
            ADD_WORLDMAP_PARTICLE( "map02_fire01" );
            ADD_WORLDMAP_PARTICLE( "map02_lightning01" );
            ADD_WORLDMAP_PARTICLE( "map02_lightning02" );
            ADD_WORLDMAP_PARTICLE( "map02_smog01" );
            ADD_WORLDMAP_PARTICLE( "map02_spin01" );
            break;
        }
    case WORLDMAP_XENEA:
        {
            ADD_WORLDMAP_PARTICLE( "map03_lightning01" );
            ADD_WORLDMAP_PARTICLE( "map03_lightning02" );
            ADD_WORLDMAP_PARTICLE( "map03_fire01" );
            ADD_WORLDMAP_PARTICLE( "map03_fire02" );
            ADD_WORLDMAP_PARTICLE( "map03_smog01" );
            ADD_WORLDMAP_PARTICLE( "map03_snow01" );
            ADD_WORLDMAP_PARTICLE( "map03_snow02" );
            ADD_WORLDMAP_PARTICLE( "map03_waterfall01" );
            ADD_WORLDMAP_PARTICLE( "map03_waterfall02" );
            ADD_WORLDMAP_PARTICLE( "map03_waterfall03" );
            ADD_WORLDMAP_PARTICLE( "map03_waterfall_effect01" );
            ADD_WORLDMAP_PARTICLE( "map03_waterfall_effect02" );
            break;
        }
    case WORLDMAP_SILVERLAND:
        {
            ADD_WORLDMAP_PARTICLE( "SilverLand_Effect_01" );
            ADD_WORLDMAP_PARTICLE( "SilverLand_Effect_Fire_01" );
            ADD_WORLDMAP_PARTICLE( "SilverLand_Effect_Fire_02" );
            ADD_WORLDMAP_PARTICLE( "SilverLand_Effect_Fire_03" );
            if ( false == SiKGCRoomListManager()->IsHellMode() )
            {
                ADD_WORLDMAP_PARTICLE( "Hero02_spin" );
            }
            break;
        }
    case WORLDMAP_ACHAEMEDIA:
        {
            ADD_WORLDMAP_PARTICLE( "map06_fire01" );
            ADD_WORLDMAP_PARTICLE( "map06_fire02" );
            ADD_WORLDMAP_PARTICLE( "map06_smog" );
            ADD_WORLDMAP_PARTICLE( "map06_smog_01" );
            ADD_WORLDMAP_PARTICLE( "map06_smog_02" );
            ADD_WORLDMAP_PARTICLE( "map06_shine" );
            break;
        }
    case WORLDMAP_UPPERMAP:
        {
            break;
        }
    }

#if !defined(NATION_IDN)
	//	송크란 이벤트 중이라면...
	if ( SiGCSongkranEventManager()->IsEventOpen() ) 
	{
		if ( m_pkWaterEffect )
        {
            m_pkWaterEffect->RenderMapEffect( true );
			m_pkWaterEffect->ToggleRender( true );
        }
	}
#endif

    // 던전이 오픈되어야만 붙혀줄 파티클들은 아래에..
    std::vector< EGCGameMode > vecInfo = SiKGCWorldMapManager()->GetCurWorldMapInfo();
    std::vector< EGCGameMode >::iterator vecIter = vecInfo.begin();
    for(; vecIter != vecInfo.end(); ++vecIter)
    {
        if ( !SiKGCRoomManager()->IsAvailableGameMode( GC_GMC_DUNGEON, *vecIter ) )
            continue;

        switch( *vecIter )
        {
        case GC_GM_QUEST51:
        {
            if( false == SiKGCRoomListManager()->IsHellMode() )
            {                
				if (!SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON,GC_GM_QUEST51))
					break;

                switch( SiKGCWorldMapManager()->GetCurrentWorldMap() )
                {
                //case WORLDMAP_BERMESIA:
                //    ADD_WORLDMAP_PARTICLE( "Circus_Map_01_1");
                //    ADD_WORLDMAP_PARTICLE( "Circus_Map_01_2");
                //    ADD_WORLDMAP_PARTICLE( "Circus_Map_01_3");
                //    break;
                case WORLDMAP_ELIA:
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_02_1" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_02_2" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_02_3" );
                    break;
                case WORLDMAP_XENEA:
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_03_1" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_03_2" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_03_3" );
                    break;
                case WORLDMAP_SILVERLAND:
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_06_1" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_06_2" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_06_3" );
                    break;
                case WORLDMAP_ATUM:
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_05_1" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_05_2" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_05_3" );
                    break;
                case WORLDMAP_ACHAEMEDIA:
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_04_1" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_04_2" );
                    ADD_WORLDMAP_PARTICLE( "Circus_Map_04_3" );
                    break;
                default:
                    break;
                }
            }
            break;
        }
        case GC_GM_QUEST50:
            if( false == SiKGCRoomListManager()->IsHellMode() )
            {
                if (!SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON,GC_GM_QUEST50))
                    break;
                ADD_WORLDMAP_PARTICLE( "Map_80skull" );
            }
            break;
        case GC_GM_QUEST53:
            switch( SiKGCWorldMapManager()->GetCurrentWorldMap() )
            {
            //case WORLDMAP_BERMESIA:
            //    ADD_WORLDMAP_PARTICLE( "Map_Train_01");
            //    break;
            case WORLDMAP_ELIA:
                ADD_WORLDMAP_PARTICLE( "Map_Train_03");
                break;
            case WORLDMAP_XENEA:
                ADD_WORLDMAP_PARTICLE( "Map_Train_04");
                break;
            case WORLDMAP_SILVERLAND:
                ADD_WORLDMAP_PARTICLE( "Map_Train_02");
                break;
            case WORLDMAP_ATUM:
                ADD_WORLDMAP_PARTICLE( "Map_Train_06");
                break;
            case WORLDMAP_ACHAEMEDIA:
                ADD_WORLDMAP_PARTICLE( "Map_Train_05");
                break;
            }
            break;
        }
    }
}

#undef ADD_WORLDMAP_PARTICLE

void KGCWorldMapContainer::ClearMapEffect()
{
    for( int i = 0; i < (int)m_vecMapEffect.size(); ++i )
    {
        g_ParticleManager->DeleteSequence( m_vecMapEffect[i] );
    }
    m_vecMapEffect.clear();
}

void KGCWorldMapContainer::UpperMapBtnRenderProc()
{
	
}

//void KGCWorldMapContainer::ToggleChatList( bool bShow )
//{
//    if( g_pkWorldmapChatBox != NULL )
//    {
//        g_pkWorldmapChatBox->ToggleShowList( bShow );
//        m_pkChatListBack->ToggleRender( bShow );
//        m_pkScrollBG->ToggleRender( bShow );
//        m_pkChatBoxClose->ToggleRender( bShow );
//#if !defined( NATION_IDN )
//		m_pkChangeViewBtn->ToggleRender( bShow );
//#endif
//        m_bShowList = bShow;
//
//        if( bShow == false )
//        {
//            ::SetFocus( g_hUIMainWnd );
//        }
//    }
//}

void KGCWorldMapContainer::OnCreateComplete()
{
    //m_pkMessage->SetText( g_pkStrLoader->GetString( STR_ID_CHAT_MESSAGE ) );
    //m_pkMessage->SetAlign( DT_RIGHT );
}

//void KGCWorldMapContainer::ShowChatBox( bool bShow )
//{
//    ToggleChatList( bShow );
//    g_pkWorldmapChatBox->ToggleRender( bShow );
//}

//void KGCWorldMapContainer::ChatColorBoxInit( void )
//{
//    g_pkWorldmapChatBox->InitColorButton();
//}

KGCWorldMapPanel* KGCWorldMapContainer::GetWolrdMapPanel( int iIndex_ )
{
	if(iIndex_ < 0 || iIndex_ >= NUM_WORLDMAP)
		return NULL;

	return m_apkWorldMapPanel[iIndex_];
}

void KGCWorldMapContainer::RefreshOpenedContinent()
{
	EGCWorldMap eCurrentWorldmap = SiKGCWorldMapManager()->GetCurrentWorldMap();
	SiKGCWorldMapManager()->SetCurrentWorldMap( eCurrentWorldmap );
	SiKGCWorldMapManager()->UpdateWorldMap();
}

bool FunctorSortCharacter( const DWORD& left, const DWORD& right )
{
    return SiKGCMissionManager()->GetMissionInfo( left )->m_iCharacter == g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
}

bool FunctorSortUniqueMission( const DWORD& left, const DWORD& right )
{
    return SiKGCMissionManager()->GetMissionInfo( left )->m_bUnique;
}

void KGCWorldMapContainer::ShowDungeonMissionList( EGCGameMode eGameMode, D3DXVECTOR2 vPos )
{
    if( m_pkDungeonMissionList == NULL )
        return;

    std::vector<DWORD>& vecMissionList = g_kGlobalValue.m_kUserMission.GetDungeonMissionList( eGameMode );

    if( vecMissionList.empty() )
        return;

    if( vecMissionList.size() >= KGCDungeonMissionList::DML_LIST_SIZE )
    {
        vecMissionList.erase( vecMissionList.begin() + 5, vecMissionList.end() );
    }

    //std::sort( vecMissionList.begin(), vecMissionList.end(), FunctorSortUniqueMission );
    //std::sort( vecMissionList.begin(), vecMissionList.end(), FunctorSortCharacter );

    for( int i = 0 ; i < (int)vecMissionList.size() ; ++i )
    {
        m_pkDungeonMissionList->AddMissionInfo( vecMissionList[i] );
    }

    m_pkDungeonMissionList->ToggleRender( true );
    m_pkDungeonMissionList->SetWindowPosDirect( vPos );

    D3DXVECTOR2 vListPos = m_pkDungeonMissionList->GetFixedWindowPos();
    float fGap;
    if( (fGap = GC_SCREEN_WIDTH - ( vListPos.x + m_pkDungeonMissionList->GetWidth() ) ) < 0.0f )
    {
        D3DXVECTOR2 vReposition = m_pkDungeonMissionList->GetFixedWindowLocalPos();
        vReposition.x += fGap;
        m_pkDungeonMissionList->SetWindowPosDirect( vReposition );
    }
    
    if( (fGap = GC_SCREEN_HEIGHT - ( vListPos.y + m_pkDungeonMissionList->GetHeight() ) ) < 0.0f )
    {
        D3DXVECTOR2 vReposition = m_pkDungeonMissionList->GetFixedWindowLocalPos();
        vReposition.y += fGap;
        m_pkDungeonMissionList->SetWindowPosDirect( vReposition );
    }
}

void KGCWorldMapContainer::HideDungeonMissionList()
{
    if( m_pkDungeonMissionList == NULL )
        return;

    m_pkDungeonMissionList->ClearMissionInfo();
    m_pkDungeonMissionList->ToggleRender( false );
}

void KGCWorldMapContainer::EventNewContinent( bool bRender )
{
    if ( m_pkNewContinentNotice->IsRenderOn() )
        m_pkNewContinentNotice->ToggleRender( false );
    //else if ( true == bRender )
        //g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Button01_Worldmap", -0.638f, 0.306f, 0.5f), GC_LAYER_UI);
    
    m_pkNewContinentNotice->ToggleRender( bRender );
    m_pkBackgoundBlack->ToggleRender( bRender );
}

void KGCWorldMapContainer::SetVitalityInfo()
{
    if( g_kGlobalValue.UseVitalitySystem() == false ){
        m_pkVitalityExpInfo->ToggleRender( false );
    } else {
        m_pkVitalityExpInfo->SetCurrCharInfo();
        m_pkVitalityExpInfo->ToggleRender( true );
    }
}