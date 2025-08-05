#include "stdafx.h"
#include "KGCShortMenuDlg.h"
#include "GCSceneMenuS6.h"
#include "GCStateDef.h"

IMPLEMENT_CLASSNAME( KGCShortMenuDlg );
IMPLEMENT_WND_FACTORY( KGCShortMenuDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCShortMenuDlg, "gc_shortmenu_dlg" );

KGCShortMenuDlg::KGCShortMenuDlg(void)
: m_pkBackGround1( NULL )
, m_pkBackGround2( NULL )
, m_pkBackGroundClose1( NULL )
, m_pkBackGroundClose2( NULL )
, m_pkBackGroundMenu1( NULL )
, m_pkBackGroundMenu2( NULL )
, m_pkBackGroundBar1( NULL )
, m_pkBackGroundBar2( NULL )
, m_pkCloseBtn( NULL )
, m_pkCloseBtn2( NULL )
, m_pkShortMenuCharInfo( NULL )
, m_pkShortMenuCharView( NULL )
{
	for( int i = 0; i < MAX_BUTTONS; i++ )
	{
		m_saButtons[i] = NULL;
		LINK_CONTROL_STM( "shortmenu_btn" << i, m_saButtons[i] );
	}

	LINK_CONTROL( "background1", m_pkBackGround1 );
	LINK_CONTROL( "background2", m_pkBackGround2 );

	LINK_CONTROL( "background_close1", m_pkBackGroundClose1 );
	LINK_CONTROL( "background_close2", m_pkBackGroundClose2 );

	LINK_CONTROL( "background_menu1", m_pkBackGroundMenu1 );
	LINK_CONTROL( "background_menu2", m_pkBackGroundMenu2 );

	LINK_CONTROL( "background_bar1", m_pkBackGroundBar1 );
	LINK_CONTROL( "background_bar2", m_pkBackGroundBar2 );

	LINK_CONTROL( "close_btn1", m_pkCloseBtn );
	LINK_CONTROL( "close_btn2", m_pkCloseBtn2 );

	LINK_CONTROL( "shortmenu_char_view",  m_pkShortMenuCharView );
	LINK_CONTROL( "shortmenu_char_info",  m_pkShortMenuCharInfo );

	m_saButtonsServer[BTN_DEPOT] = SMB_DEPOT;
	m_saButtonsServer[BTN_MISSION] = SMB_MISSION;
	m_saButtonsServer[BTN_MAIL] = SMB_POST;
	m_saButtonsServer[BTN_GACHA] = SMB_GACHA;
	m_saButtonsServer[BTN_MSN] = SMB_MSN;
	m_saButtonsServer[BTN_CALENDAR] = SMB_CALENDAR;
	m_saButtonsServer[BTN_MONSTERCARD] = SMB_CARD;
	m_saButtonsServer[BTN_EVENT] = SMB_EVENT;
	m_saButtonsServer[BTN_GUILD] = SMB_ENTER_GUILD;
	m_saButtonsServer[BTN_AGIT] = SMB_AGIT_ENTER;
	m_saButtonsServer[BTN_SQUARE] = SMB_SQUARE;
	m_saButtonsServer[BTN_COUPLE] = SMB_COUPLE;
	m_saButtonsServer[BTN_REC] = SMB_REC;
	m_saButtonsServer[BTN_CLOSET] = SMB_CLOSET;
	m_saButtonsServer[BTN_VIPSTORE] = SMB_VIPSTORE;
#ifndef DISABLE_GACHA_PON
	m_saButtonsServer[BTN_GACHAPONG] = SMB_GACHA_PONG;
#endif
	m_saButtonsServer[BTN_CHANGECHARACTER] = SMB_CHARACTER_SCENE;
	m_saButtonsServer[BTN_OPTION] = SMB_OPTION;
	m_saButtonsServer[BTN_GUIDE] = SMB_GUIDE;
	m_saButtonsServer[BTN_END] = SMB_END;
}

KGCShortMenuDlg::~KGCShortMenuDlg(void)
{
}

void KGCShortMenuDlg::ActionPerformed( const KActionEvent& event )
{
}

void KGCShortMenuDlg::OnCreate()
{
	for( int i = 0; i < MAX_BUTTONS; i++ )
	{
		if( m_saButtons[i] )
		{
			m_saButtons[i]->InitState(true, true, this);
		}
	}

	if( m_pkCloseBtn )
	{
		m_pkCloseBtn->InitState( true, true, this);
		m_pkCloseBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClose);
		m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkCloseBtn2 )
	{
		m_pkCloseBtn2->InitState( false, true, this);
		m_pkCloseBtn2->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClose);
		m_pkCloseBtn2->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkBackGround1 )
	{
		m_pkBackGround1->InitState( true, true, this);
	}

	if( m_pkBackGround2 )
	{
		m_pkBackGround2->InitState( false, true, this);
	}

	if( m_pkBackGroundClose1 )
	{
		m_pkBackGroundClose1->InitState( true, true, this);
	}

	if( m_pkBackGroundClose2 )
	{
		m_pkBackGroundClose2->InitState( false, true, this);
	}

	if( m_pkBackGroundMenu1 )
	{
		m_pkBackGroundMenu1->InitState( true, true, this);
	}

	if( m_pkBackGroundMenu2 )
	{
		m_pkBackGroundMenu2->InitState( false, true, this);
	}

	if( m_pkBackGroundBar1 )
	{
		m_pkBackGroundBar1->InitState( true, true, this);
	}

	if( m_pkBackGroundBar2 )
	{
		m_pkBackGroundBar2->InitState( false, true, this);
	}

	if( m_pkShortMenuCharInfo )
	{
		m_pkShortMenuCharInfo->InitState( true, true);
	}

	if( m_pkShortMenuCharView )
	{
		m_pkShortMenuCharView->InitState( true, true);
	}

	m_saButtons[BTN_DEPOT]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickDepot);
	m_saButtons[BTN_MISSION]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickMission);
	m_saButtons[BTN_MAIL]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickPost);
	m_saButtons[BTN_GACHA]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickGacha);
	m_saButtons[BTN_MSN]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickMSN);
	m_saButtons[BTN_CALENDAR]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickCalendar);
	m_saButtons[BTN_MONSTERCARD]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickMonster);
	m_saButtons[BTN_EVENT]->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickEventMiniGameBanner);
	m_saButtons[BTN_GUILD]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickGuild);
	m_saButtons[BTN_AGIT]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickAgit);
	m_saButtons[BTN_SQUARE]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickSquare);
	m_saButtons[BTN_COUPLE]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickCouple);
	m_saButtons[BTN_REC]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickReplayRecord);
	m_saButtons[BTN_CLOSET]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickCloset);
	m_saButtons[BTN_VIPSTORE]->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickVipStore);
	m_saButtons[BTN_GACHAPONG]->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickGachaPong);
	m_saButtons[BTN_CHANGECHARACTER]->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickCharSelectScene);
	m_saButtons[BTN_OPTION]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickOption);
	m_saButtons[BTN_GUIDE]->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickGuide);
	m_saButtons[BTN_END]->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCShortMenuDlg::OnClickEnd);
}

bool KGCShortMenuDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
	if( IsCharViewDisable() == true )
	{
		DisableCharView();
	}
	else
	{
		m_pkShortMenuCharView->ToggleRender( true );
		m_pkShortMenuCharInfo->ToggleRender( true );
		m_pkShortMenuCharView->SetPreviewWindow( &g_MyD3D->m_TempPlayer );

		float posX = 0.f;
		for( int i = 0; i < MAX_BUTTONS; i++ )
		{
			posX = (i % 4) * 97.f;
			m_saButtons[i]->SetWindowPosX( posX + 565.f);
			m_saButtons[i]->Lock( false );
 		}
	}

	UpdateButtonState();
	return true;
}

void KGCShortMenuDlg::OnInitialize()
{
	
}

void KGCShortMenuDlg::DisableCharView()
{
	m_pkShortMenuCharView->ToggleRender( false );
	m_pkShortMenuCharInfo->ToggleRender( false );

	m_pkBackGround1->ToggleRender( false );
	m_pkBackGroundBar1->ToggleRender( false );
	m_pkBackGroundClose1->ToggleRender( false );
	m_pkBackGroundMenu1->ToggleRender( false );
	m_pkCloseBtn->ToggleRender( false );

	m_pkBackGround2->ToggleRender( true );
	m_pkBackGroundBar2->ToggleRender( true );
	m_pkBackGroundClose2->ToggleRender( true );
	m_pkBackGroundMenu2->ToggleRender( true );
	m_pkCloseBtn2->ToggleRender( true );

	float posX = 0.f;
	for( int i = 0; i < MAX_BUTTONS; i++ )
	{
		// Kawan>
		if( i == 18 )
		{
			posX = 3 * 95.f;
		}
		else
		{
			posX = (i % 4) * 95.f;
		}

		m_saButtons[i]->SetWindowPosX( posX + 325.f ); // Kawan> 325.f
		m_saButtons[i]->Lock( false );
	}
}

void KGCShortMenuDlg::UpdateButtonState()
{
	int iState = g_MyD3D->m_pStateMachine->GetState();

	switch(iState)
	{
	case GS_ROOM:
		{
			m_saButtons[BTN_GACHA]->Lock(true);
			m_saButtons[BTN_VIPSTORE]->Lock(true);
			m_saButtons[BTN_CLOSET]->Lock(true);
			m_saButtons[BTN_GUIDE]->Lock(true);
		}
	case GS_MY_INFO_FROM_ROOM:
		{
			m_saButtons[BTN_DEPOT]->Lock(true);
			//Friends
			m_saButtons[BTN_GUILD]->Lock(true);
			m_saButtons[BTN_AGIT]->Lock(true);
			m_saButtons[BTN_SQUARE]->Lock(true);
			m_saButtons[BTN_CHANGECHARACTER]->Lock(true);
			m_saButtons[BTN_EVENT]->Lock(true);
#ifndef DISABLE_GACHA_PON
			m_saButtons[BTN_GACHAPONG]->Lock( true );
#endif
			m_saButtons[BTN_VIPSTORE]->Lock(true);
			m_saButtons[BTN_CLOSET]->Lock(true);
			m_saButtons[BTN_GUIDE]->Lock(true);
		}
		break;
	case GS_SQUARE:
		{
			//contents
			m_saButtons[BTN_DEPOT]->Lock(true);
			m_saButtons[BTN_MISSION]->Lock(true);
			m_saButtons[BTN_MAIL]->Lock(true);

			//Events
			m_saButtons[BTN_GACHA]->Lock(true);

			//Friends
			m_saButtons[BTN_GUILD]->Lock(true);
			m_saButtons[BTN_AGIT]->Lock(true);
			m_saButtons[BTN_SQUARE]->Lock(true);
			
			//System
			m_saButtons[BTN_REC]->Lock(true);
			m_saButtons[BTN_CHANGECHARACTER]->Lock(true);
			m_saButtons[BTN_EVENT]->Lock(true);
			m_saButtons[BTN_VIPSTORE]->Lock(true);
			m_saButtons[BTN_CLOSET]->Lock(true);
			m_saButtons[BTN_GUIDE]->Lock(true);
		}
		break;

	case GS_GUILD:
	{
		m_saButtons[BTN_DEPOT]->Lock(true);
		m_saButtons[BTN_MISSION]->Lock(true);
		m_saButtons[BTN_EVENT]->Lock(true);
		m_saButtons[BTN_GUIDE]->Lock(true);
	}
	break;
	case GS_CHAR_SELECT_FROM_ROOM:
	case GS_CHAR_SELECT:
		OnClose();
		break;
	}

	std::vector<int> vecDisableList = g_kGlobalValue.GetDisableMenuList();
	for( int i = 0; i < MAX_BUTTONS; i++ )
	{
		if(std::find(vecDisableList.begin(), vecDisableList.end(), m_saButtonsServer[i]) != vecDisableList.end())
		{
			m_saButtons[i]->ToggleRender( false );
		}
		else if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
		{
			if ( IsPrivateCharDisableMenu( i ) )
			{
				m_saButtons[i]->Lock( true );
			}
		}
	}
}

void KGCShortMenuDlg::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCShortMenuDlg::OnClickDepot()
{
	g_pkUIScene->m_pkSceneMenuBtnS6->m_pkBtnList->OnClickDepot();
	OnClose();
}

void KGCShortMenuDlg::OnClickMission()
{
	g_MyD3D->OnMissionButton();
	OnClose();
}

void KGCShortMenuDlg::OnClickMSN()
{
	g_MyD3D->OnMSNButton();
	OnClose();
}

void KGCShortMenuDlg::OnClickOption()
{
	g_MyD3D->OnOptionButton();
	OnClose();
}

void KGCShortMenuDlg::OnClickCalendar()
{
	g_MyD3D->OnCalendarButton();
	OnClose();
}

void KGCShortMenuDlg::OnClickCouple()
{
	g_MyD3D->OnCoupleButton();
	OnClose();
}

void KGCShortMenuDlg::OnClickEventMiniGameBanner()
{
	if(g_kGlobalValue.m_vecEventOpenInfo.empty())
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_NOTHING_EVENT_LIST ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
	else
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_MINIGAME_LIST_DLG );
	}

	OnClose();
}

void KGCShortMenuDlg::OnClickAgit()
{
	SiKGCAgitGameManager()->EnterMyAgit();
	OnClose();
}

void KGCShortMenuDlg::OnClickReplayRecord()
{
	
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_RECORD );
	OnClose();
}

void KGCShortMenuDlg::OnClickPost()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_POST_LETTER );
	OnClose();
}

void KGCShortMenuDlg::OnClickMonster()
{
	g_pkUIScene->m_pkSceneMenuBtnS6->m_pkBtnList->OnClickMonster();
	OnClose();
}

void KGCShortMenuDlg::OnClickGuild()
{
	g_pkUIScene->m_pkSceneMenuBtnS6->m_pkBtnList->OnClickEnterGuild();
	OnClose();
}

void KGCShortMenuDlg::OnClickGacha()
{
	if( g_kGlobalValue.m_vecGachaUseVersion.empty() || 
		(g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP &&
		g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO) ) 
	{ 
		return;
	}

	SiKGCDepotManager()->Send_DepotInfoByCharTabReqForGacha();

	if( g_kGlobalValue.IsSingleGacha() ) 
	{  
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0, g_kGlobalValue.GetSingleGachaVer() );  //m_iItemID, GachaVer
	}
	else 
	{  
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUPER_GACHA, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType,g_MyD3D->m_TempPlayer.GetCurrentChar().GetCurrentPromotion() );
	}
	OnClose();
}

void KGCShortMenuDlg::OnClickSquare()
{
	g_MyD3D->OnSquareButton();
	OnClose();
}

void KGCShortMenuDlg::OnClickCharSelectScene()
{
	g_pkUIScene->m_pkSceneMenuBtnS6->m_pkBtnList->OnClickCharSelectScene();
	OnClose();
}

void KGCShortMenuDlg::OnClickServerScene()
{
	g_pkUIScene->m_pkSceneMenuBtnS6->m_pkBtnList->OnClickServerScene();
	OnClose();
}

void KGCShortMenuDlg::OnClickEnd()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L""
		, KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, false );
	OnClose();
}

#ifndef DISABLE_GACHA_PON
void KGCShortMenuDlg::OnClickGachaPong()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GACHA_PONG_DLG );
	OnClose();
}
#endif

void KGCShortMenuDlg::OnClickVipStore()
{
	g_kGlobalValue.isCurrentShopVIP = true;
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_HERODUNGEON_SHOP_DLG_S6);
	OnClose();
}

void KGCShortMenuDlg::OnClickCloset()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COORDI_VIEW_DLG );
	OnClose();
}

bool KGCShortMenuDlg::IsCharViewDisable()
{
	int iCurState = g_MyD3D->m_pStateMachine->GetState();
	switch(iCurState)
	{
		case GS_SQUARE:
		case GS_MY_INFO_FROM_ROOM:
		case GS_GUILD:
		case GS_ROOM:
		case GS_MY_INFO:
		case GS_SHOP_CASH:
#ifndef DISABLE_MAP_SHOP // Kawan>
		case GS_SHOP_GP:
#endif
#ifdef USE_COORDI_SHOP
		case GS_COORDI_SHOP:
#endif
			return true;
	}

	return false;
}

bool KGCShortMenuDlg::IsPrivateCharDisableMenu( int nMenu )
{
	switch ( nMenu ) 
	{
	case BTN_GUIDE:
	case BTN_GACHA:
	case BTN_GUILD:
	case BTN_AGIT:
	case BTN_COUPLE:
	case BTN_DEPOT:
	case BTN_SQUARE:
#ifndef DISABLE_GACHA_PON
	case BTN_GACHAPONG:
#endif
		return true;
	default:
		return false;
	}

	return true;
}

void KGCShortMenuDlg::OnClickGuide()
{
	PopUpWebPage(HOMEPAGE_GUIDE_LINK);
	OnClose();
}