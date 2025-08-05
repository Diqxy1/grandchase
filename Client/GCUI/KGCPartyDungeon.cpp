#include "StdAfx.h"
#include "gcui/KGCPartyDungeon.h"
#include "KGCRoomManager.h"

IMPLEMENT_CLASSNAME( KGCPartyDungeon );
IMPLEMENT_WND_FACTORY( KGCPartyDungeon );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyDungeon, "gc_party_dungeon_dlg" );

KGCPartyDungeon::KGCPartyDungeon(void)
: m_pkRegionSelStr ( NULL)
, m_pkDungeonSelStr ( NULL)
, m_pkRegionStr ( NULL)
, m_pkDungeonStr ( NULL)
, m_pkRegionSelect ( NULL)
, m_pkDungeonSelect ( NULL)
, m_pkChampionStr ( NULL)
, m_pkChampionCheckbox ( NULL)
, m_pkConfirm ( NULL)
, m_pkCancel ( NULL)
, m_pkClose (NULL)
, m_pkRegionMenu ( NULL )
, m_pkDungeonMenu ( NULL )
, m_pkLevelBox ( NULL )
, m_iSelectedModeIndex (0)
, m_iSelectedRegionIndex (0)
, m_iSelectedLevel(0)
, m_iDungeonMaxDifficulty (0)
, m_iCheckOverlap(0)
{
    //던전 선택, 해당 지역 선택 String 
    LINK_CONTROL("region_select_static", m_pkRegionSelStr);
    LINK_CONTROL("dungeon_select_static", m_pkDungeonSelStr );

    //선택된 지역을 표시 하기 위한 String 표시
    LINK_CONTROL("region_static ", m_pkRegionStr );
    LINK_CONTROL("dungeon_static", m_pkDungeonStr );

    //해당 던전이나 지역을 선택시 누르는 버튼 풀다운박스를 호출 시키는 윈도우
    LINK_CONTROL("region_select", m_pkRegionSelect );
    LINK_CONTROL("dungeon_select", m_pkDungeonSelect );

    //챔피언 스트링 표시 & Check box 
    LINK_CONTROL("champion_static", m_pkChampionStr );
    LINK_CONTROL("champion_check_box", m_pkChampionCheckbox );

    //멀티 메뉴들
    LINK_CONTROL("popupbox_region" , m_pkRegionMenu );
    LINK_CONTROL("popupbox_dungeon" , m_pkDungeonMenu );

    //확인 및 취소
    LINK_CONTROL("confirm", m_pkConfirm );
    LINK_CONTROL("cancel", m_pkCancel );
    LINK_CONTROL("close", m_pkClose);
    LINK_CONTROL("level_change_box", m_pkLevelBox );

}

KGCPartyDungeon::~KGCPartyDungeon(void)
{

}

void KGCPartyDungeon::OnCreate()
{
    m_pkRegionSelStr->InitState( true , false , this );
    m_pkRegionSelStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_SELECT_REGION )  , true );
    m_pkRegionSelStr->SetToolTip( g_pkStrLoader->GetString( STR_ID_SELECT_REGION ) );
    m_pkRegionSelStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
    

    m_pkDungeonSelStr->InitState( true , false , this );
    m_pkDungeonSelStr->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_SELECT_DUNGEON ), true );
    m_pkDungeonSelStr->SetToolTip( g_pkStrLoader->GetString( STR_ID_SELECT_DUNGEON ) );
    m_pkDungeonSelStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkRegionStr->InitState( true , false , this );
    m_pkRegionStr->SetText(L"");

    m_pkDungeonStr->InitState( true , false , this );
    m_pkDungeonStr->SetText(L"");

    m_pkRegionSelect->InitState( true , true , this );
    m_pkRegionSelect->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCPartyDungeon::OnClickRegion);

    m_pkDungeonSelect->InitState( true , true , this );
    m_pkDungeonSelect->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCPartyDungeon::OnClickDungeon);

    m_pkChampionStr->InitState( true , true , this );
    m_pkChampionStr->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION ) );
    m_pkChampionStr->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );

    m_pkChampionCheckbox->InitState( true , true , this );
    //m_pkChampionCheckbox->ToggleCheckBox( false );
    m_pkChampionCheckbox->SetCheck( false );
    m_pkChampionCheckbox->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK , this ,&KGCPartyDungeon::CheckChampionMode);

    m_pkConfirm->InitState( true , true , this );
    m_pkConfirm->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK , this ,&KGCPartyDungeon::OnConfirm);

    m_pkCancel->InitState( true , true , this );
    m_pkClose->InitState( true , true , this );

    m_pkRegionMenu->InitState( false, true, this );
    m_pkRegionMenu->ConnectEventProcedure(KGCMultiMenu::K3DWE_MENU_CLICKED,this,&KGCPartyDungeon::OnClickRegionMenu);

    m_pkDungeonMenu->InitState( false, true, this );
    m_pkDungeonMenu->ConnectEventProcedure(KGCMultiMenu::K3DWE_MENU_CLICKED,this,&KGCPartyDungeon::OnClickDungeonMenu);

    m_pkLevelBox->SetSelfInputCheck( false );
    m_pkLevelBox->AddActionListener( this );
    m_pkLevelBox->SetText( g_pkStrLoader->GetString( SiKGCWorldMapManager()->GetBasicLevelString(0) ));
    m_pkLevelBox->ConnectEventProcedure(KGCChangeBox::LEFT_BUTTON,this,&KGCPartyDungeon::ChangeLevelCategoryLeft);
    m_pkLevelBox->ConnectEventProcedure(KGCChangeBox::RIGHT_BUTTON,this,&KGCPartyDungeon::ChangeLevelCategoryRight);
}
void KGCPartyDungeon::CheckChampionMode ( void )
{
    if ( !m_pkChampionCheckbox->GetCheck() && SiKGCRoomManager()->CanPlayHellmode() )
    {
        if (SiKGCWorldMapManager()->IsOpenHellMode( static_cast<EGCGameMode> (m_iSelectedModeIndex), g_kGlobalValue.m_kUserInfo.mapStagePlayable )) {
            m_pkChampionCheckbox->ToggleCheck();
            m_iSelectedLevel = GC_DUNGEON_LEVEL_HELL;
            m_pkLevelBox->SetText( g_pkStrLoader->GetString( STR_ID_CHAMPION ) );
            m_pkLevelBox->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
            m_pkLevelBox->EnableChangeBox( false );
            g_kGlobalValue.SetHellMode(true);
        }
        else
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PARTY_NO_CHAMPION_DUNGEON ) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
        }
    }
    else if ( m_pkChampionCheckbox->GetCheck() )
    {
        m_pkChampionCheckbox->ToggleCheck();
    
        int nStringIndex = SiKGCWorldMapManager()->GetDungeonLevelString( static_cast<EGCGameMode>(m_iSelectedModeIndex), 0 ) ;

        m_pkLevelBox->SetText( g_pkStrLoader->GetString ( nStringIndex ) );
        m_pkLevelBox->EnableChangeBox( true );
        m_iSelectedLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( static_cast<EGCGameMode>(m_iSelectedModeIndex), g_kGlobalValue.m_kUserInfo.mapStagePlayable );
        g_kGlobalValue.SetHellMode(false);
    }
    else if ( !SiKGCRoomManager()->CanPlayHellmode())
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_HAVE_CHAMPION_ENTER_ITEM ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
    }
    
}

void KGCPartyDungeon::SetRegionString( void )
{
    m_pkRegionMenu->ClearMenu();
    std::map<int,int> mapEnableRegion = SiKGCWorldMapManager()->GetEnableRegionNumberAndStringNumber();
    
    for(std::map<int,int>::iterator mIt = mapEnableRegion.begin() ; mIt != mapEnableRegion.end();mIt++)
    {
        if(mIt == mapEnableRegion.begin())
        {
            SetViewRegionString(mIt->first, mIt->second);
        }

        m_pkRegionMenu->AddMenu(mIt->first, g_pkStrLoader->GetString( mIt->second ));
    }
}

void KGCPartyDungeon::SetViewRegionString( int iRegion, int iStringNum)
{
    m_iSelectedRegionIndex = iRegion;
    m_pkRegionStr->SetForceWordWrapText( g_pkStrLoader->GetString( iStringNum ) , true );
    m_pkRegionStr->SetToolTip( g_pkStrLoader->GetString( iStringNum ) );
}


void KGCPartyDungeon::SetDungeonString( int iRegion )
{
    m_pkDungeonMenu->ClearMenu();

    std::map<int,int> mapEnableDungeon = SiKGCWorldMapManager()->GetEnableDungeonNumberAndStringNumber(iRegion);
    if(mapEnableDungeon.empty()) return;

    for(std::map<int,int>::iterator mIt = mapEnableDungeon.begin() ; mIt != mapEnableDungeon.end();mIt++)
    {
        if(mIt == mapEnableDungeon.begin())
        {
            SetViewDungeonString(mIt->first, mIt->second);
        }

        if ( !SiKGCWorldMapManager()->IsEnablePartyDungeon(static_cast<EGCGameMode>(mIt->first) ) ) 
            continue;
        else
            m_pkDungeonMenu->AddMenu(mIt->first, g_pkStrLoader->GetString( mIt->second ));
    }
}

void KGCPartyDungeon::SetViewDungeonString( int iDungeon, int iStringNum)
{
    m_iSelectedModeIndex = iDungeon;
    m_pkDungeonStr->SetForceWordWrapText( g_pkStrLoader->GetString( iStringNum ) , true );
    m_pkDungeonStr->SetToolTip( g_pkStrLoader->GetString( iStringNum ) );
}


void KGCPartyDungeon::OnClickDungeonMenu( const KActionEvent& event )
{
    if ( !event.m_dwlParam == 0) {
        SetViewDungeonString(static_cast<int>(event.m_dwlParam), SiKGCWorldMapManager()->GetDungeonStringNumber(static_cast<EGCGameMode>(event.m_dwlParam)));
        m_iDungeonMaxDifficulty = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( static_cast<EGCGameMode>(m_iSelectedModeIndex), g_kGlobalValue.m_kUserInfo.mapStagePlayable );
        if ( m_pkChampionCheckbox->GetCheck() )
        {
            m_pkChampionCheckbox->ToggleCheck();
            m_pkLevelBox->EnableChangeBox( true );
            m_iSelectedLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( static_cast<EGCGameMode>(m_iSelectedModeIndex), g_kGlobalValue.m_kUserInfo.mapStagePlayable );
            g_kGlobalValue.SetHellMode(false);

            int nStringIndex = SiKGCWorldMapManager()->GetDungeonLevelString( static_cast<EGCGameMode>(m_iSelectedModeIndex), m_iSelectedLevel ) ;
            m_pkLevelBox->SetText( g_pkStrLoader->GetString ( nStringIndex ) );
        }
        else
        {
            m_iSelectedLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( static_cast<EGCGameMode>(m_iSelectedModeIndex), g_kGlobalValue.m_kUserInfo.mapStagePlayable );
            
            int nStringIndex = SiKGCWorldMapManager()->GetDungeonLevelString( static_cast<EGCGameMode>(m_iSelectedModeIndex), m_iSelectedLevel ) ;
            m_pkLevelBox->SetText( g_pkStrLoader->GetString ( nStringIndex ) );
        }
    }
}

void KGCPartyDungeon::OnClickRegionMenu( const KActionEvent& event )
{
    SetViewRegionString(static_cast<int>(event.m_dwlParam), SiKGCWorldMapManager()->GetRegionStringNumber(static_cast<int>(event.m_dwlParam)));
    SetDungeonString(static_cast<int>(event.m_dwlParam));
    
    int iTemp = SiKGCWorldMapManager()->GetRegionStringNumber(static_cast<int>(event.m_dwlParam));
    int iLimitLevel = 0;

    if ( iTemp ==  STR_ID_CONTINENT_BERMESIA  )
        iLimitLevel = 0; 
    else if ( iTemp ==  STR_ID_CONTINENT_SILVERLAND  )
        iLimitLevel = 1;
    else
        iLimitLevel = 2;

    int iDungeonIdx = GetDungeonIdx( iTemp );
    m_iDungeonMaxDifficulty = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel( static_cast<EGCGameMode>(iDungeonIdx), g_kGlobalValue.m_kUserInfo.mapStagePlayable );

    if ( m_pkChampionCheckbox->GetCheck() )
    {
        m_pkChampionCheckbox->ToggleCheck();
        m_pkLevelBox->EnableChangeBox( true );
        g_kGlobalValue.SetHellMode(false);
    }
    
    m_iSelectedLevel = 0;

    int nStringIndex = SiKGCWorldMapManager()->GetDungeonLevelString( static_cast<EGCGameMode>(m_iSelectedModeIndex), m_iSelectedLevel ) ;
    m_pkLevelBox->SetText( g_pkStrLoader->GetString ( nStringIndex ) );
}

int KGCPartyDungeon::GetDungeonIdx (int iRegion)
{
    int iDungeonIdx = 0;
    
    switch ( iRegion )
    {
    case STR_ID_CONTINENT_BERMESIA :
        iDungeonIdx = GC_GM_QUEST0;
    	break;
    case STR_ID_CONTINENT_ELIA :
        iDungeonIdx = GC_GM_QUEST12;
        break;
    case STR_ID_CONTINENT_XENIA :
        iDungeonIdx = GC_GM_QUEST20;
        break;
    case STR_ID_CONTINENT_SILVERLAND :
        iDungeonIdx = GC_GM_QUEST23;
        break;
    case STR_ID_CONTINENT_ATUM :
        iDungeonIdx = GC_GM_QUEST39;
        break;
    case STR_ID_CONTINENT_ACHAEMEDIA :
        iDungeonIdx = GC_GM_QUEST42;
        break;
    }
    
    return iDungeonIdx;
}

void KGCPartyDungeon::OnCreateComplete( void )
{
    SetRegionString();
    SetDungeonString();
    m_pkLevelBox->SetText( g_pkStrLoader->GetString( SiKGCWorldMapManager()->GetBasicLevelString(0) ));
    m_iSelectedLevel = 0;
    m_iSelectedRegionIndex = 7;

    m_pkCancel->SetHotKey(DIK_ESCAPE);
    m_pkCancel->DisconnectAllEventProcedure();
    m_pkCancel->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyDungeon::OnCancel );
    m_pkClose->SetHotKey(DIK_ESCAPE);
    m_pkClose->DisconnectAllEventProcedure();
    m_pkClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyDungeon::OnCancel );
}

void KGCPartyDungeon::ActionPerformed( const KActionEvent& event )
{

}

void KGCPartyDungeon::FrameMoveInEnabledState( void )
{
 
}

void KGCPartyDungeon::PostChildDraw( void )
{

}

void KGCPartyDungeon::OnCancel( void )
{
    this->Destroy();
    g_pkUIScene->EndMessageBox();

    m_pkRegionStr->SetText(L"");
    m_pkDungeonStr->SetText(L"");
    g_KDSound.Play( "31" );
}

void KGCPartyDungeon::OnConfirm( void )
{
    std::wstring wsTemp = m_pkDungeonStr->GetText();

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE && wsTemp != L"")
    {
        this->Destroy();
        g_pkUIScene->m_pkPartyWnd->OffPartyDlg();
        g_pkSquareOverlayUI->ExitSquare();
        g_MyD3D->m_pStateMachine->GoWorldMap();
        //m_pkGuildSquareNotice->Destroy();

        g_KDSound.Play( "31" );
        std::wstring wTemp = SiKGCRoomManager()->m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str();
        
        HRESULT hr = SiKGCRoomManager()->CreateRoomParty( wTemp,L"",  m_iSelectedModeIndex, m_iSelectedLevel, GC_GMC_DUNGEON , m_iSelectedLevel , GC_GS_FOREST_OF_ELF ,  true );
    }
    else if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME && wsTemp != L"" )
    {
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
        {
            this->Destroy();
            g_pkUIScene->m_pkPartyWnd->OffPartyDlg();
            //	말풍선
            g_pkAgitOverlayUI->DeleteAllSayBalloon();

            // 모션제거
            g_pGraphicsHelper->UnLoadPlayerActionMotion();
            g_pGraphicsHelper->UnLoadPlayerModel();

            // 엑스트라 메시정보 제거
            g_kGlobalValue.m_ExtraMesh.ClearMeshInfo();

            SiKGCAgitGameManager()->SaveAgitObject();
            SiKGCAgitGameManager()->ClearObject();
            SiKGCAgitGameManager()->ClearPlaceCharUID();
            g_pkAgitOverlayUI->OnExitAgit();

            if( SiKGCAgitGameManager()->GetRegisterState() )
                SiKGCAgitGameManager()->UnRegisterAgitList();

            // 현재 아지트 나가고 
            g_pStateGame->OnExitGame(); 
            g_pkUIScene->EndMessageBox();

            g_MyD3D->m_pStateMachine->GoWorldMap();

            g_KDSound.Play( "31" );
            std::wstring wTemp = SiKGCRoomManager()->m_vecDungeonRoomNameList[rand() % KGCRoomManager::m_vecDungeonRoomNameList.size()].c_str();

            HRESULT hr = SiKGCRoomManager()->CreateRoomParty( wTemp,L"",  m_iSelectedModeIndex, m_iSelectedLevel, GC_GMC_DUNGEON , m_iSelectedLevel , GC_GS_FOREST_OF_ELF ,  true );
        }
    }
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,  g_pkStrLoader->GetString( STR_ID_PARTY_EMPTY_LIST) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true );
    }
}


void KGCPartyDungeon::OnClickRegion( void )
{
    m_pkDungeonMenu->ToggleRender(false);
    if( m_pkRegionMenu->IsRenderOn() )
        m_pkRegionMenu->ToggleRender(false);
    else
        m_pkRegionMenu->ToggleRender(true);

    m_pkDungeonStr->SetText(L"");
    g_KDSound.Play( "31" );
}

void KGCPartyDungeon::OnClickDungeon( void )
{
    if ( m_pkDungeonSelect->IsLocked() )
        return;

    m_pkRegionMenu->ToggleRender(false);
    if( m_pkDungeonMenu->IsRenderOn() )
        m_pkDungeonMenu->ToggleRender(false);
    else
        m_pkDungeonMenu->ToggleRender(true);

    g_KDSound.Play( "31" );
}

void KGCPartyDungeon::ChangeLevelCategoryLeft(  )
{
    if ( m_iSelectedLevel > 0 )
    {
		m_iSelectedLevel--;
    }

    int nStringIndex = SiKGCWorldMapManager()->GetDungeonLevelString( static_cast<EGCGameMode>(m_iSelectedModeIndex), m_iSelectedLevel ) ;

    m_pkLevelBox->SetText( g_pkStrLoader->GetString ( nStringIndex ) );
    
    g_KDSound.Play( "31" );
}
void KGCPartyDungeon::ChangeLevelCategoryRight(  )
{
    //초반 던전들은 던전 난이도가 0이기에 증가되면 안된다. 
    if ( (0 < m_iDungeonMaxDifficulty)  &&  (m_iSelectedLevel < m_iDungeonMaxDifficulty)  )
        m_iSelectedLevel++;

    int nStringIndex = SiKGCWorldMapManager()->GetDungeonLevelString( static_cast<EGCGameMode>(m_iSelectedModeIndex), m_iSelectedLevel ) ;
    m_pkLevelBox->SetText( g_pkStrLoader->GetString ( nStringIndex ) );

    g_KDSound.Play( "31" );
}

