#include "stdafx.h"
#include "GCChatBox.h"
#include "GCGuildMemo.h"
//
//
//
#include "KDInput.h"
#include "../MyD3D.h"
#include "../SlangFilter.h"
#include "KGCRoomManager.h"
#include "../GCUI/GCColorDlg.h"
#include "SlangFilter.h"
#include "../GCUI/KGCWorldMapContainer.h"
#include "GCStateMachine.h"
#include "../GCUI/KGCCheckButton.h"
#include "../GCUI/GCAgitOverlayUI.h"
#include "KGCSocialMotionManager.h"
#include "KGCAgitGameManager.h"
#include "GCCheckBox.h"

#include "GCTextColorManager.h"



IMPLEMENT_CLASSNAME( KGCChatBox );
IMPLEMENT_WND_FACTORY( KGCChatBox );
IMPLEMENT_WND_FACTORY_NAME( KGCChatBox, "gc_chat_box" );

KGCChatBox* g_pkGuildChatBox = NULL;
KGCChatBox* g_pkRoomChatBox  = NULL;
KGCChatBox* g_pkSquareChatBox  = NULL;
KGCChatBox* g_pkLobbyChatBox   = NULL;
KGCChatBox* g_pkWorldmapChatBox= NULL;
KGCChatBox* g_pkGuildLobbyChatBox = NULL;

#if defined( USE_AGIT_SYSTEM )
KGCChatBox* g_pkAgitChatBox = NULL; 
#endif

KGCChatBox::KGCChatBox( void )
: m_bFirstCrerate( true )
, m_iCurChatType( CT_ALL )
, m_pkOptionBtn( NULL )
, m_pkOptionList(NULL)
, m_pkKindList(NULL)
, m_pkSizeUpBtn(NULL)
, m_pkSizeDownBtn(NULL)
, m_pkChatListBack(NULL)
, m_pkScrollBack(NULL)
, m_iCurSize(CTSIZE_NOMAL)
, m_pkEditBack(NULL)
, m_pkPaint_Btn(NULL)
, m_pkColorBtn(NULL)
{
	m_eChatBoxType  = CBT_NONE;
	m_pkEdit        = NULL;

	m_pkList        = NULL;
	m_pkScroll      = NULL;
	for( int i = 0 ; i < CT_NUM; ++i )
	{
		m_pkChatType[i] = NULL;
	}

	LINK_CONTROL( "edit",   m_pkEdit );
	LINK_CONTROL( "list",   m_pkList );
	LINK_CONTROL( "scroll", m_pkScroll );
	LINK_CONTROL( "menu_btn", m_pkOptionBtn );
	LINK_CONTROL( "chat_option", m_pkOptionList );
	LINK_CONTROL( "chat_btn_list_s6", m_pkKindList );

	LINK_CONTROL( "up_size_btn", m_pkSizeUpBtn);
	LINK_CONTROL( "down_size_btn", m_pkSizeDownBtn);

	LINK_CONTROL( "chat_list_back", m_pkChatListBack);
	LINK_CONTROL( "back", m_pkScrollBack);
	LINK_CONTROL( "chat_edit_back", m_pkEditBack );

	LINK_CONTROL( "color_palette", m_pkPaint_Btn );
	LINK_CONTROL( "colordrop_btn", m_pkColorBtn );

	m_mapEvent["chat_btn_all"] = &KGCChatBox::OnChatBtn;
	m_mapEvent["chat_btn_general"] = &KGCChatBox::OnChatBtn;
	m_mapEvent["chat_btn_whisper"] = &KGCChatBox::OnChatBtn;
	m_mapEvent["chat_btn_guild"] = &KGCChatBox::OnChatBtn;

	m_mapEvent["edit"]   = &KGCChatBox::OnEdit;
	m_mapEvent["list"]   = &KGCChatBox::OnList;
	m_mapEvent["scroll"] = &KGCChatBox::OnScroll;

	m_Color = 0xFFFFFFFF;
	m_bChatBtnFold = true;
	m_bPrevChatBtnFold = true;
	m_strWhisperNickname = L"";
}

KGCChatBox::~KGCChatBox( void )
{
	m_mapEvent.clear();

	if ( NULL != g_pkGuildChatBox && this == g_pkGuildChatBox )
	{
		g_pkGuildChatBox = NULL;
	}
	else if ( NULL != g_pkRoomChatBox && this == g_pkRoomChatBox )
	{
		g_pkRoomChatBox = NULL;
	}
	else if ( NULL != g_pkSquareChatBox && this == g_pkSquareChatBox )
	{
		g_pkSquareChatBox = NULL;
	}
	else if( NULL != g_pkLobbyChatBox && this == g_pkLobbyChatBox )
	{
		g_pkLobbyChatBox = NULL;
	}
	else if( NULL != g_pkWorldmapChatBox && this == g_pkWorldmapChatBox )
	{
		g_pkWorldmapChatBox = NULL;
	}
	else if( NULL != g_pkGuildLobbyChatBox && this == g_pkGuildLobbyChatBox )
	{
		g_pkGuildLobbyChatBox = NULL;
	}
}

void KGCChatBox::OnCreate( void )
{
	SetSelfInputCheck( true );

	ASSERT( m_pkEdit != NULL );
	ASSERT( m_pkList != NULL );
	ASSERT( m_pkScroll != NULL );

	m_pkEdit->AddActionListener( this );
	m_pkEdit->SetSelfInputCheck( false );
	m_pkList->AddActionListener( this );
	m_pkList->SetSelfInputCheck( false );
	m_pkList->SetLineSpace( 1.2f );


	if ( GetWindowName() == "guild_chat_box" )
	{
		g_pkGuildChatBox = this;
		m_eChatBoxType   = CBT_GUILD;
	}
    else if ( GetWindowName() == "room_chat_box_s6" )
    {
        g_pkRoomChatBox  = this;
        m_eChatBoxType   = CBT_ROOM;
    }
	else if ( GetWindowName() == "square_chat_box" )
	{
		g_pkSquareChatBox  = this;
		m_eChatBoxType   = CBT_SQUARE;
	}
	else if( GetWindowName() == "lobby_chat_box" )
	{
		g_pkLobbyChatBox = this;
		m_eChatBoxType = CBT_LOBBY;
	}
	else if( GetWindowName() == "match_chat_box_s6" )
	{
		g_pkLobbyChatBox = this;
		m_eChatBoxType = CBT_LOBBY;
	}
	else if( GetWindowName() == "worldmap_chat_box" )
	{
		g_pkWorldmapChatBox = this;
		m_eChatBoxType = CBT_WORLDMAP;
		g_pkWorldmapChatBox->AddActionListener( g_pkUIScene->m_pkWorldMapContainer );
		g_pkWorldmapChatBox->SetSelfInputCheck( false );
	}
	else if( GetWindowName() == "guildlobby_chat_box" )
	{
		g_pkGuildLobbyChatBox = this;
		m_eChatBoxType = CBT_LOBBY;
		//g_pkGuildLobbyChatBox->AddActionListener( g_pkUIScene->m_pkGuildLobbyDlg );
		// g_pkWorldmapChatBox->SetSelfInputCheck( false );
	}
#if defined (USE_AGIT_SYSTEM )
    else if( GetWindowName() == "agit_chat_box" )
    {
        g_pkAgitChatBox = this;
        m_eChatBoxType = CBT_AGIT;
    }
#endif

	if ( !IsNewChatBox() ) {
		m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
		m_pkScroll->SetScrollRangeMax( 0 );
	}
	else {
		m_pkScroll->InitScroll( 0, 1, 0, m_pkList->GetDisplayLimitLine(), 0, 0, 1, true );

	}



	m_pkScroll->InitState(true,false,this);

	// 채팅 박스!
	InitChatTypeValues();

	m_bChatBtnFold = true;

	// 챗버튼 월드맵은 다른거 쓸거에요~!
	if ( !IsNewChatBox() ) { 

		FoldingChatBox( m_bChatBtnFold );
		m_pkChatType[m_iCurChatType]->ToggleRender( true );
		m_pkChatType[CT_GENERAL]->SetToolTip( g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_10 ),-100.0f );
		m_pkChatType[CT_WHISPER]->SetToolTip( g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_11 ),-100.0f );
		m_pkChatType[CT_GUILD]->SetToolTip( g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_12 ), -100.0f );
	}
	else {
		m_pkOptionBtn->InitState(true,true,this);	
		m_pkOptionBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChatBox::OnClickOptionBtn );
		m_pkOptionList->InitState(false, true, this);
		m_pkKindList->InitState(true,true,this);

		g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_CHAT_OPTION_BTN_CLICK);
		g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_CHAT_OPTION_BTN_CLICK, this, &KGCChatBox::OnClickOption );

		g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_CHAT_KIND_BTN_CLICK);
		g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_CHAT_KIND_BTN_CLICK, this, &KGCChatBox::ChangeChatType );

        if ( m_pkSizeUpBtn ) 
        {
		    m_pkSizeUpBtn->InitState(true,true,this);
		    m_pkSizeUpBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCChatBox::OnClickSizeUpBtn);
        }
        
        if ( m_pkSizeDownBtn ) 
        {
		    m_pkSizeDownBtn->InitState(true,true,this);
		    m_pkSizeDownBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCChatBox::OnClickSizeDownBtn);
        }

        if ( m_pkChatListBack ) 
        {
    		m_pkChatListBack->InitState(true,false,NULL);
        }

        if ( m_pkScrollBack ) 
        {
    		m_pkScrollBack->InitState(true,false,NULL);
        }

        if ( m_pkEditBack ) 
        {
		    m_pkEditBack->InitState(true,false,NULL);
        }

		m_iCurSize = CTSIZE_NOMAL;
	}

	m_bFirstCrerate = false;

	if (m_pkPaint_Btn)
		m_pkPaint_Btn->InitState(false,true,this);
	
	if (m_pkColorBtn)
	{
		m_pkColorBtn->InitState(false, true, this);
		m_pkColorBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChatBox::OnClickColorBtn);
	}

	m_bIsChatColorEnabled = false;
	InitColorButton();
}

void KGCChatBox::OnClickColorBtn()
{
	m_pkPaint_Btn->ToggleRender(!m_pkPaint_Btn->IsRenderOn());
}

void KGCChatBox::ChangeChatSize ( int iSize )
{
	const int ciChatBtnListPosY = 81;
	const int ciEditPosY = 136;
	const int ciEditBackPosY = 131;
	const int ciMenuBtnPosY = 134;
	const int ciOptionListBtnPosY = 5;

	const int ciTotalHeghit = 160;

	const int ciChatListBackHeghit = 130;
	const int ciChatListHeghit = 116;
	const int ciScrollBackHeghit = 96;
	const int ciScrollHeghit = 96;

	const int ciColorBtnPosY = 134;
	const int ciPaintBtnPosY = 132;

	int iPosY = 0;
	int iHeghit = 0;

	switch (iSize) {
		case CTSIZE_MINIMUM :
			iHeghit = -130;
			iPosY = 130;
			break;
		case CTSIZE_NOMAL :
			break;
		case CTSIZE_MAXIMUM :
			iHeghit = 130;
			iPosY = -130;
			break;
		default:
			break;
	}

	int iTotalResultHeghit = ciTotalHeghit + iHeghit;

	int iChatListBackHeghit = ciChatListBackHeghit + iHeghit;
	int iChatListHeghit = ciChatListHeghit + iHeghit;
	int iScrollBackHeghit = ciScrollBackHeghit + iHeghit;
	int iScrollHeghit = ciScrollHeghit + iHeghit;

	SetHeightDirect(iTotalResultHeghit);

    if ( m_pkChatListBack ) 
    {
    	m_pkChatListBack->SetSize(m_pkChatListBack->GetWidth(),iChatListBackHeghit);
    }

	m_pkList->SetHeightDirect(iChatListHeghit);
	m_pkScroll->SetHeightDirect(iScrollHeghit);

    if ( m_pkScrollBack ) 
    {
    	m_pkScrollBack->SetSize(m_pkScrollBack->GetWidth(),iScrollBackHeghit);
    }


	int iChatBtnListPosY = ciChatBtnListPosY + iHeghit;
	int iEditPosY = ciEditPosY + iHeghit;
	int iEditBackPosY = ciEditBackPosY + iHeghit;
	int iMenuBtnPosY = ciMenuBtnPosY + iHeghit;
	int iOptionListBtnPosY = ciOptionListBtnPosY + iHeghit;

	int iColorBtnPosY = ciColorBtnPosY + iHeghit;
	int iPaintBtnPosY = ciPaintBtnPosY + iHeghit;

	m_pkKindList->SetWindowPosYDirect(static_cast<float>(iChatBtnListPosY));
	m_pkEdit->SetWindowPosYDirect(static_cast<float>(iEditPosY));
	m_pkEditBack->SetWindowPosYDirect(static_cast<float>(iEditBackPosY));
	m_pkOptionBtn->SetWindowPosYDirect(static_cast<float>(iMenuBtnPosY));
	m_pkOptionList->SetWindowPosYDirect(static_cast<float>(iOptionListBtnPosY));
	m_pkPaint_Btn->SetWindowPosYDirect(static_cast<float>(iPaintBtnPosY));
	m_pkColorBtn->SetWindowPosYDirect(static_cast<float>(iColorBtnPosY));

	if ( iSize == CTSIZE_MINIMUM ) {
		m_pkEdit->SetFocus(false);
		ToggleChatList(false);
	}
	else {
		m_pkScroll->InitScroll( 
			m_pkList->GetTopIndex(), 
			1, 
			0, 
			m_pkList->GetViewContentSize(), 
			0, 
			0, 
			m_pkList->GetDisplayLimitLine(), 
			true );
		m_pkList->SetTopIndex( m_pkScroll->GetScrollPos());
		m_pkEdit->SetFocus(true);

		ToggleChatList(true);

		if (m_pkPaint_Btn)
		{
			if (g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN)
			{
				SiGCTextColorManager()->SetColorNum( GCTextColorManager::CASH_ITEM_COLOR_NUM + 1 );
				m_pkPaint_Btn->CheckPaletteButtons(true);
				m_pkPaint_Btn->EnablePaletteButtons(true);
			}

		}
	}

	InitColorButton();

	m_iCurSize = iSize;

	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_CHAT_CHANGE_SIZE_BTN_CLICK, iSize) );

}


void KGCChatBox::ToggleChatList ( bool bShow )
{
    if ( m_pkChatListBack ) 
    {
        m_pkChatListBack->ToggleRender(bShow);
    }

    if ( m_pkScrollBack ) 
    {
	    m_pkScrollBack->ToggleRender(bShow);
    }


	m_pkList->ToggleRender(bShow);
	m_pkScroll->ToggleRender(bShow);

    if ( m_pkSizeUpBtn && m_pkSizeDownBtn ) 
    {
	    m_pkSizeUpBtn->ToggleRender(bShow);
	    m_pkSizeDownBtn->ToggleRender(bShow);
    }
}


void KGCChatBox::OnClickSizeUpBtn()
{
	if ( ++m_iCurSize > CTSIZE_MAXIMUM )
		m_iCurSize = CTSIZE_MAXIMUM;

	ChangeChatSize(m_iCurSize);

}

void KGCChatBox::OnClickSizeDownBtn()
{
	if ( --m_iCurSize < CTSIZE_MINIMUM )
		m_iCurSize = CTSIZE_MINIMUM;

	ChangeChatSize(m_iCurSize);
}


void KGCChatBox::OnClickOptionBtn()
{
	m_pkOptionList->InitOptionText();

	m_pkOptionList->ToggleRender( !m_pkOptionList->IsRenderOn() );
	m_pkScroll->Lock(m_pkOptionList->IsRenderOn());

	
}

void KGCChatBox::OnClickOption( const KActionEvent& event )
{
	std::map<int, bool> mapInfo = m_pkOptionList->GetCheck();

	for ( std::map<int, bool>::iterator mit = mapInfo.begin() ; mit != mapInfo.end() ; ++mit ) {

		if (mit->first == CT_GENERAL)
		{
			m_pkList->SetFilterInfo(KGCChatManager::CHAT_TYPE_COLORCHAT,mit->second);
		}
		SetFilteringListInfo ( mit->first, mit->second );

		if ( event.m_dwlParam == mit->first )
			FilterCheckMessageBox(mit->first,!mit->second);

	}

	SetScrollHeight(GetScrollHeight()); //그냥 높이 그대로 재 할당 해줘도 리플래쉬한다.

}

void KGCChatBox::ChangeChatType( const KActionEvent& event )
{
	m_iCurChatType = static_cast<int>(event.m_dwlParam);

	KGCChatManager::EChatCommand  eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
	EChatType eType = static_cast<EChatType>(m_iCurChatType);
	switch (eType)
	{
	case CT_GENERAL:             
		eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
		break;
	case CT_GUILD:
		eChatCmd = KGCChatManager::CHAT_CMD_GUILD;
		break;                
	case CT_WHISPER:                            
		eChatCmd = KGCChatManager::CHAT_CMD_WHISPER;
		break;                
	default:
		eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
		break;

	}
	g_pkChatManager->SetCommend(eChatCmd);
	if ( m_iCurChatType == CT_WHISPER )
		SetText(GetCurrentPreMark());				
}

void KGCChatBox::InitColorButton()
{
	if (m_pkPaint_Btn)
	{
		KItem* chatPaintItem = g_pItemMgr->GetInventoryItemFromID(ITEM_VIP_TEMPORARY_ITEM);
		if (chatPaintItem != NULL)
		{
			if (m_pkColorBtn)
				m_pkColorBtn->ToggleRender(true);
			m_bIsChatColorEnabled = true;
			SiGCTextColorManager()->SetColorNum(GCTextColorManager::CASH_ITEM_COLOR_NUM + 1);
			m_pkPaint_Btn->CheckPaletteButtons(true);
			m_pkPaint_Btn->EnablePaletteButtons(true);
		}
		else
		{
			if (m_pkColorBtn)
				m_pkColorBtn->ToggleRender(false);
			m_bIsChatColorEnabled = false;
		}
	}
}

void KGCChatBox::FrameMoveInEnabledState(void)
{
	if (g_pkWorldmapChatBox == this && (g_pkInput->IsDown(DIK_RETURN) || g_pkInput->IsDown(DIK_NUMPADENTER))) {
		if (m_iCurSize == CTSIZE_MINIMUM) {
			ChangeChatSize(CTSIZE_NOMAL);
		}
		m_pkEdit->SetFocus(true);
	}

	if (m_bIsChatColorEnabled == false)
	{
		m_pkPaint_Btn->ToggleRender(false);
		m_Color = 0xFFFFFFFF;
		m_pkEdit->SetFontColor(m_Color);
	}

	if (m_pkEdit->HasFocus())
	{
		if (m_pkPaint_Btn->GetColor() != m_pkEdit->GetFontColor() && m_bIsChatColorEnabled)
		{
			m_Color = m_pkPaint_Btn->GetColor();
			m_pkEdit->SetFontColor(m_Color);
		}

		if (g_pkWorldmapChatBox == this && m_iCurSize == CTSIZE_MINIMUM)
			ChangeChatSize(CTSIZE_NOMAL);

		std::wstring strTemp = m_pkEdit->GetText();

		if (strTemp.size() == 3 && strTemp[0] == L'/') {
			KGCChatManager::EChatCommand  eChatCmd = g_pkChatManager->FindChatType(m_pkEdit->GetText());

			if (eChatCmd != KGCChatManager::CHAT_CMD_ERROR) {
				EChatType eType = CT_GENERAL;
				switch (eChatCmd)
				{
				case KGCChatManager::CHAT_CMD_NORMAL:
					eType = CT_GENERAL;
					m_pkEdit->SetText(L"");
					break;
				case KGCChatManager::CHAT_CMD_GUILD:
					eType = CT_GUILD;
					m_pkEdit->SetText(L"");
					break;
				case KGCChatManager::CHAT_CMD_WHISPER:
					eType = CT_WHISPER;
					m_pkEdit->SetText(L">", false);
					break;
				default:
					eType = CT_GENERAL;
					break;

				}

				if (eType != m_iCurChatType) {
					if (!IsNewChatBox()) {
						ChangeChatTypePos(eType, true);
					}
					else {
						m_pkKindList->SetBtnNum(static_cast<int>(eType) - 1);
						m_iCurChatType = eType;
						g_pkChatManager->SetCommend(eChatCmd);
					}
				}

			}
		}

		for (int i = 0; i < 256; i++)
		{
			if (i == DIK_F1 || i == DIK_F2 || i == DIK_F3 || i == DIK_F4 || i == DIK_F5 || i == DIK_F6 ||
				i == DIK_F7 || i == DIK_F8 || i == DIK_F9 || i == DIK_F10 || i == DIK_F11 || i == DIK_F12)
				continue;

			if (g_pkInput->IsDown(i))
			{
				g_KDSound.Play("71");
			}
		}

		if (1 == ::wcslen(m_pkEdit->GetText()) &&
			((g_pkInput->IsPressed(DIK_LSHIFT) && g_pkInput->IsDown(DIK_PERIOD)) ||
				(g_pkInput->IsPressed(DIK_RSHIFT) && g_pkInput->IsDown(DIK_PERIOD))))
		{
			ASSERT(g_pkChatManager != NULL);
			std::wstring strCmd = g_pkChatManager->OnWhisperInputStart();

			if (strCmd.size() > 0)
			{
				m_pkEdit->SetText(strCmd.c_str());
				m_pkEdit->SetSelection((int)strCmd.size() + 1, (int)strCmd.size() + 1);
			}
		}
		else if (g_pkChatManager->IsWhisperMode() && g_pkInput->IsDown(DIK_UP))
		{
			ASSERT(g_pkChatManager != NULL);
			std::wstring strCmd = g_pkChatManager->OnWhisperScrollUp();
			if (strCmd.size() > 0)
			{
				m_pkEdit->SetText(strCmd.c_str(), false);
			}
		}
		else if (g_pkChatManager->IsWhisperMode() && g_pkInput->IsDown(DIK_DOWN))
		{
			ASSERT(g_pkChatManager != NULL);
			std::wstring strCmd = g_pkChatManager->OnWhisperScrollDown();
			if (strCmd.size() > 0)
			{
				m_pkEdit->SetText(strCmd.c_str(), false);
			}
		}
	}
}

void KGCChatBox::ActionPerformed( const KActionEvent& event )
{	

	std::map<std::string, HANDLER>::iterator mit = m_mapEvent.find( event.m_pWnd->GetWindowName() );
	if ( mit == m_mapEvent.end() ) return;

	(this->*m_mapEvent[event.m_pWnd->GetWindowName()])( event );


	
}

void KGCChatBox::OnEdit( const KActionEvent& event_ )
{
	bool bUseTeamChat = FALSE;

	UNREFERENCED_PARAMETER( event_ );
	ASSERT( g_pkChatManager != NULL );

#if defined( NATION_USA ) || defined( NATION_LATINAMERICA ) || defined( NATION_EU )
	std::wstring strText = m_pkEdit->GetText();
	if( SiKSlangFilter()->CheckUnicode( strText ) == true )
	{
		g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString(STR_ID_NOT_AVAILABLE_LANGUAGE) , KGCChatManager::CHAT_TYPE_ALERT );
		m_pkEdit->SetText( L"" );
		g_KDSound.Play( "72" );
		return;
	}
#endif
    
#ifdef NICKNAME_CAMPAIGN

    // 특수문자나를 가진 사람이 채팅을 입려하면 채팅을 사용할 수 없다는 스트링을 뿌리자.
    if( g_pkUIScene->IsValidNickname() == false && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN ) {
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_CHATTING_RESTRICTION), KGCChatManager::CHAT_TYPE_ALERT );

        m_pkEdit->SetText( L"" );	
        // 소리 시끄럽다고 꺼달래요!
        g_KDSound.Play( "72" );
        m_pkEdit->ToggleRender(true);//원인은 모르지만 가끔 리드로우 안되는 부분이 있어 글짜 쓸때 강제로 한번 더 드로우 해줌
        
        return;
    }
#endif

	if ( 0 == ::wcslen( m_pkEdit->GetText() ) )
	{
#if defined( USE_AGIT_SYSTEM )
		if ( m_eChatBoxType == CBT_AGIT && event_.m_dwCode == D3DWE_EDIT_RETURN )
		{
			::SetFocus( g_hUIMainWnd );
			m_pkEdit->SetText( L"" );
			g_pkUIScene->m_pkAgitOverlayUI->SetChatEventHandling(KGCAgitOverlayUI::EM_CHAT_RETURN);
		}
#endif
		if( g_pkWorldmapChatBox == this && m_iCurSize != CTSIZE_MINIMUM) {
			ChangeChatSize(CTSIZE_MINIMUM);
			m_pkEdit->SetFocus(false);
		}

		return;
	}
	if ( event_.m_dwCode == D3DWE_EDIT_ESCAPE ) 
	{
		m_pkEdit->SetText( L"" );
		g_KDSound.Play( "72" );
		return;
	}

	if ( !wcscmp( m_pkEdit->GetText() ,
		g_pkStrLoader->GetString(STR_ID_WHISPER_FILTER_COMMAND).c_str() ) )
	{
		if( g_pkChatManager->GetWhisperFilter() )
			g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE_OFF), KGCChatManager::CHAT_TYPE_ALERT );
		else
			g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE_ON), KGCChatManager::CHAT_TYPE_ALERT );

		g_pkChatManager->SetWhisperFilter( !g_pkChatManager->GetWhisperFilter() );
	}

	else
	{
		KGCChatManager::EChatCommand  eChatCmd=g_pkChatManager->AnalyzeChatCommand( m_pkEdit->GetText() );
		
		switch(eChatCmd)
		{
		case KGCChatManager::CHAT_CMD_NORMAL:
			{
				static DWORD dwLastSendChat = 0;
				static DWORD dwLastSendShoutChat = 0;
				static CString strPrev = _T("");
				static int iNumSameInput = 0;

				// 마지막 입력시간에서 일정 시간이상 지나면?
				if( timeGetTime() - dwLastSendChat > 1000 * 3/*초*/ )
					iNumSameInput = 0; // 요걸 초기화 시켜준다
				// 긴 시간을 두고 ㅋㅋㅋ 만 여러번 쳤다고 도배는 아니니까..	

				//입력된 문자가 예전것과 같다면 기록한다.
				if ( strPrev == m_pkEdit->GetText() )
					iNumSameInput++;
				else
				{
					strPrev = m_pkEdit->GetText();
					iNumSameInput = 0;
				}

				//3번이나 기록되면 도배로 간주하여 치지 않게 한다. 
				if (iNumSameInput >= 3)
				{		
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ), KGCChatManager::CHAT_TYPE_ALERT );				
				}
				// 일반 다른 메시지는 이렇단다!
				else if( timeGetTime() - dwLastSendChat < 1100/*(DWORD)(eChatCmd == KGCChatManager::CHAT_CMD_SHOUT ? 3100 : 1100)*/ )
				{				
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ), KGCChatManager::CHAT_TYPE_ALERT );				
				}
				else
				{
					dwLastSendChat = timeGetTime();
					std::wostringstream strmChatMsg;
					std::wstring strChatMsg;

					switch ( m_eChatBoxType )
					{
					case CBT_WORLDMAP:
					case CBT_ROOM:
					case CBT_LOBBY:
						{
							strmChatMsg << GetEditStr();
							KP2P::GetInstance()->Send_ChatReq( L"Server", strmChatMsg.str().c_str(), KChatData::MSG_COMMON ,m_Color);
							break;
						}
					case CBT_GUILD:
						{
							if ( false == g_pkGuildMemo->FilterGuildMemoCommand( m_pkEdit->GetText() ) )
							{
								strChatMsg = GetEditStr();
								SiKD3DEmoticonString()->CheckForSend( strChatMsg );
								//KP2P::GetInstance()->Send_GuildChatReq( L"Server", strChatMsg.c_str() );
								KP2P::GetInstance()->Send_ChatReq( L"Server", strChatMsg.c_str(), KChatData::MSG_GUILD );
							}

							break;
						}
					case CBT_SQUARE: //광장 채팅 박스인 경우...
						{
							switch (eChatCmd)
							{
							case KGCChatManager::CHAT_CMD_NORMAL:
								strChatMsg = GetEditStr();
								SiKD3DEmoticonString()->CheckForSend( strChatMsg );

								if(USERLEVEL_ADMIN == g_kGlobalValue.m_kUserInfo.eGCUserLevel  )
								{
									KP2P::GetInstance()->Send_SquareChat(KChatData::MSG_ADMIN_COMMON,
										NULL, strChatMsg,m_Color);
								}
								else
								{
									if( m_Color != 0xFFFFFFFF )
									{
										KP2P::GetInstance()->Send_SquareChat(KChatData::MSG_COLOR_CHAT,
											NULL, strChatMsg,m_Color);
									}
									else
									{
										KP2P::GetInstance()->Send_SquareChat(KChatData::MSG_COMMON,
											NULL, strChatMsg,0xffffffff);
									}
								}
								break;
							}
						}
#if defined( USE_AGIT_SYSTEM )
                    case CBT_AGIT:
                        {
                            strmChatMsg << GetEditStr();
                            
                            if( !strmChatMsg.str().empty() )
                                KP2P::GetInstance()->Send_AgitChatReq( KChatData::MSG_COMMON, strmChatMsg.str(), m_Color );
                            break;
					}
#endif
				}
			}	
            }	
			break;
			//길드챗 코멘트가 입력된 경우
		case KGCChatManager::CHAT_CMD_GUILD:
			{
				std::wstring strChatMsg;
				if ( false == g_pkGuildMemo->FilterGuildMemoCommand( m_pkEdit->GetText() ) )
				{
					strChatMsg = m_pkEdit->GetText();
					SiKD3DEmoticonString()->CheckForSend( strChatMsg );
					//KP2P::GetInstance()->Send_GuildChatReq( L"Server", strChatMsg.c_str() );
					KP2P::GetInstance()->Send_ChatReq( L"Server", strChatMsg.c_str(), KChatData::MSG_GUILD );
				}
				break;
			}
		case KGCChatManager::CHAT_CMD_TEAM: //zstaiji - 팀채팅
			{
				if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT && g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
				{
					KChatData::ENUM_CHAT_TYPE eChatType = KChatData::MSG_TEAM_CHAT;
					std::wstring strChatMsg;
					std::wstring team;
					D3DCOLOR Color;
					strChatMsg = m_pkEdit->GetText();
					strChatMsg = strChatMsg.substr(1);
					if(m_Color == 0xffffffff) //글자가 흰색일 때
					{
						Color = 0xffff99ff; //보라색으로 지정
						eChatType = KChatData::MSG_TEAM_CHAT; //팀 채팅이면서 GC클럽 색깔을 지정하지 않았을 경우
					}else{
						eChatType = KChatData::MSG_TEAM_COLOR; //팀 채팅이면서 GC클럽 색깔을 지정했을 경우.
						Color = m_Color;
					}

					team = g_pkStrLoader->GetString(STR_ID_TEAM); //팀

                    std::wstring strNickName = L"[" + team + L"] " + L"#c" + g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.strNickColor + g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.strNickName + L"#cx";
					KP2P::GetInstance()->Send_ChatReq( strNickName.c_str(), strChatMsg.c_str(), eChatType ,Color);
					bUseTeamChat = TRUE;
				}
				else
				{
					g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOT_IN_TEAM), KGCChatManager::CHAT_TYPE_ALERT );
				}
                break;
			}
#if defined( USE_SOCIAL_MOTION ) 
        case KGCChatManager::CHAT_CMD_SOCIAL_MOTION:
            {
                std::wstring strChatMsg = m_pkEdit->GetText();
                std::wstring strCommand;
                strCommand.assign( strChatMsg.begin() + 1, strChatMsg.end() );
                SiKGCSocialMotionManager()->SetMotion( strCommand );
        }
            break;
#endif
    }	
    }	

	m_pkEdit->SetText( L"" );

	if( bUseTeamChat )
	{
		m_pkEdit->SetText(L"%", false);
	}

	//if( g_pkChatManager->IsWhisperMode() )
	//{
	//	std::wstring strCmd = g_pkChatManager->OnWhisperInputStart();
	//	if ( strCmd.size() > 0 )
	//	{
	//		//m_pkEdit->SetText( strCmd.c_str() );
	//		m_pkEdit->SetText(strCmd.c_str(),false);
	//		//m_pkEdit->SetSelection( (int)strCmd.size() + 1, (int)strCmd.size() + 1 );
	//		//너 웨 들어가 있는거니?ㅡㅡ; 설마 셀렉트 된거 풀어줄려고 이런거니?ㅡㅡ;
	//	}
	//	else
	//	{
	//	    m_pkEdit->SetText( GetCurrentPreMark().c_str(), false );
	//	}
	//}
	//else
	//{
	//	m_pkEdit->SetText( GetCurrentPreMark().c_str(), false );
	//}

	// 소리 시끄럽다고 꺼달래요!
	g_KDSound.Play( "72" );
	m_pkEdit->ToggleRender(true);//원인은 모르지만 가끔 리드로우 안되는 부분이 있어 글짜 쓸때 강제로 한번 더 드로우 해줌

}

void KGCChatBox::OnList( const KActionEvent& event_ )
{
	if( m_bChatBtnFold == false )
	{
		return;
	}
	else
	{
		if( m_bPrevChatBtnFold == false )
		{
			m_bPrevChatBtnFold = true;
			return;
		}
	}

	int iIndex;

	// ScrollBar에서 MouseWheelMsg받고 있다
	//if (dwEventCode>KD3DListBox::DEFAULT_MAX_LINE)
	//{
	//    if (dwEventCode==D3DWE_CHAT_WHEEL_UP)
	//    {
	//        if (m_pkScroll->GetScrollPos()-CHAT_BOX_WHEEL_GAP>=m_pkScroll->GetScrollRangeMin())
	//        {
	//            m_pkScroll->SetScrollPos(m_pkScroll->GetScrollPos()-CHAT_BOX_WHEEL_GAP);
	//            m_pkList->SetTopIndex( m_pkScroll->GetScrollPos());

	//        }
	//    }
	//    else if (dwEventCode==D3DWE_CHAT_WHEEL_DOWN)
	//    {
	//        if (m_pkScroll->GetScrollPos()+CHAT_BOX_WHEEL_GAP<=m_pkScroll->GetScrollRangeMax())
	//        {
	//            m_pkScroll->SetScrollPos(m_pkScroll->GetScrollPos()+CHAT_BOX_WHEEL_GAP);
	//            m_pkList->SetTopIndex( m_pkScroll->GetScrollPos());
	//        }
	//    }
	//    else
	//    {
	//        ASSERT("이상한 메시지 받은거?");
	//    }
	//}

	iIndex = event_.m_dwCode;//m_pkList->GetFrontNIndex(int( dwEventCode ));
	std::wstring strClickedMsg = m_pkList->GetReturnMsg( iIndex );    
	if( !strClickedMsg.empty() && m_pkList->GetCheckList()==true )
	{
		//자동으로 귓속말 되도록 해준다.
		if ( CT_WHISPER != m_iCurChatType ) {
			if ( !IsNewChatBox() ) {
				ChangeChatTypePos( CT_WHISPER , true );
			}
			else {
				m_pkKindList->SetBtnNum( static_cast<int>(CT_WHISPER) - 1 );
				m_iCurChatType = CT_WHISPER;
				g_pkChatManager->SetCommend(KGCChatManager::CHAT_CMD_WHISPER);
			}
		}

		m_pkEdit->SetText((L">"+strClickedMsg+L" ").c_str(),false);
		m_strWhisperNickname = L">"+strClickedMsg+L" ";
		m_pkEdit->SetFocus();
	}
}

void KGCChatBox::OnScroll( const KActionEvent& event_ )
{
	UNREFERENCED_PARAMETER( event_ );

	int iTop = m_pkScroll->GetScrollPos();
	if ( m_pkList->GetTopIndex() != iTop )
	{
		g_KDSound.Play( "73" );
		m_pkList->SetTopIndex( iTop );
	}
}

void KGCChatBox::Clear()
{
	m_pkList->Clear();
	m_pkEdit->SetText( L"" );
	m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
	m_pkScroll->SetScrollRangeMax( m_pkList->GetViewContentSize() );
	m_pkScroll->SetScrollPos( m_pkList->GetTopIndex() );
}

//밑에 이 함수의 용도를 모르겠다-_-; 신고 기능과 연관되어 있는데.. 찝찝해서 일단 버림
void KGCChatBox::ClearChatBox( GCPairPlaySystem::PPS_CHAT_TYPE eType )
{
	m_pkList->Clear();
	m_pkEdit->SetText( L"" );
	m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
	m_pkScroll->SetScrollRangeMax( m_pkList->GetViewContentSize() );
	m_pkScroll->SetScrollPos( m_pkList->GetTopIndex() );
	SiGCPairPlaySystem()->ClearChatLog( eType );
}

void KGCChatBox::SetFilteringListInfo(int itype, bool bSet)
{
	if ( !IsNewChatBox() ) {
		if( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID == 0 && itype == CT_GUILD )
			m_pkChatType[itype]->Check( true );
		else
		{
			m_pkChatType[itype]->Check( !bSet );
			m_pkList->SetFilterInfo(m_aChatType[itype],bSet);
		}
	}
	else {
		if( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID != 0 || itype != CT_GUILD ) {
			m_pkList->SetFilterInfo(m_aChatType[itype],bSet);
		}
	}


}
bool KGCChatBox::GetFilteringListInfo(int itype)
{
	return m_pkList->GetFilterInfo(itype);
}


void KGCChatBox::SetText( const std::wstring& strText )
{
	m_pkEdit->SetText( strText.c_str(), false );
}

void KGCChatBox::SetChatListClick(bool bClick)
{
	m_pkList->SetCheckList(bClick);
}

void KGCChatBox::SetFocus( void )
{
	m_pkEdit->SetFocus();
}

void KGCChatBox::AddChat( const std::wstring& strChat, int iChatType/* = KGCChatManager::CHAT_TYPE_NORMAL*/, 
						 std::wstring strReturnMsg/* = L""*/, DWORD dwColor/* = 0xFFFFFFFF*/ )
{
	bool bClick = false;
	bool bAutoScroll = false;

	dwColor = g_pkChatManager->GetColorFromTable(iChatType, dwColor);

	// 컬러채팅 아이템 구현은 이 밑에 하면 될듯

	//공백이 아니라면,(클릭시 나오는 글짜가 있는 경우)
	if (strReturnMsg.empty()==false)
	{
		//클릭 가능함
		bClick = true;
	}

	//최대 스크롤인 경우, 글짜 들어오면 자동 스크롤 된다.
	bAutoScroll = m_pkScroll->GetScrollRangeMax() == m_pkScroll->GetScrollPos() ? true : false;

	m_pkList->AddString( strChat, dwColor, bClick, 0, true, strReturnMsg, iChatType );

	m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
	m_pkScroll->SetScrollRangeMax( m_pkList->GetViewContentSize() );
	if (bAutoScroll==true)
	{
		m_pkScroll->SetScrollPos( m_pkList->GetTopIndex() );
	}

	m_pkList->SetTopIndex( m_pkScroll->GetScrollPos());
}

void KGCChatBox::SetListHeight(DWORD dwHeight)
{
	m_pkList->SetHeight(dwHeight);
	m_pkList->CalcTopIndex();
}

void KGCChatBox::SetScrollHeight(DWORD dwHeight)
{
	m_pkScroll->SetHeight(dwHeight);
	m_pkScroll->ResizeBtn();

	m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine() );
	m_pkScroll->SetScrollRangeMax( m_pkList->GetViewContentSize() );
	m_pkScroll->SetScrollPos( m_pkList->GetTopIndex() );
	m_pkList->SetTopIndex( m_pkScroll->GetScrollPos());

	//m_pkScroll->SetScrollPageSize( m_pkList->GetDisplayLimitLine());
	//m_pkScroll->SetScrollRangeMax( m_pkList->GetViewContentSize() );   
	//m_pkList->SetTopIndex( m_pkScroll->GetScrollRangeMax());
	//m_pkScroll->SetScrollPos( m_pkList->GetTopIndex()); //맨 아래쪽에 오도록 설정

}

void KGCChatBox::MoveListandScroolPosY(int iDeltaY)
{
	m_pkScroll->SetAllWindowPos(D3DXVECTOR2(m_pkScroll->GetFixedWindowLocalPos().x,m_pkScroll->GetFixedWindowLocalPos().y + iDeltaY));
	m_pkList->SetAllWindowPos(D3DXVECTOR2(m_pkList->GetFixedWindowLocalPos().x,m_pkList->GetFixedWindowLocalPos().y + iDeltaY));
}


void KGCChatBox::OnUserList( const std::wstring& strNickName )
{
	//자동으로 귓속말 되도록 해준다.
	if ( CT_WHISPER != m_iCurChatType ) {
		if ( !IsNewChatBox() ) {
			ChangeChatTypePos( CT_WHISPER , true );
		}
		else {
			m_pkKindList->SetBtnNum( static_cast<int>(CT_WHISPER) - 1 );
			m_iCurChatType = CT_WHISPER;
			g_pkChatManager->SetCommend(KGCChatManager::CHAT_CMD_WHISPER);
		}
	}


	m_pkEdit->SetText((L">"+strNickName+L" ").c_str(),false);	
	m_pkEdit->SetFocus();
}

void KGCChatBox::ToggleShowList( bool bShow )
{
	m_pkList->ToggleRender( bShow );
	m_pkScroll->ToggleRender( bShow );
}

void KGCChatBox::FilterCheckMessageBox(int state, bool bCheck )
{
    std::wstring wstr = L"";
    switch(state)
    {
    case CT_WHISPER:
        if(!bCheck)
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_6 );
        else
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_7 );
        break;
    case CT_GUILD:
        if(!bCheck)
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_8 );
        else
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_9 );
        break;
    case CT_GENERAL:
        if(!bCheck)
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_4 );
        else
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_5 );
        break;
    default:
        if(!bCheck)
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_2 );
        else
            wstr = g_pkStrLoader->GetString( STR_ID_CHAT_TOOLTIP_3 );
        break;
    }

    AddChat(wstr,KGCChatManager::CHAT_TYPE_ALERT);

//    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, wstr, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}

void KGCChatBox::OnChatBtn( const KActionEvent& event_ )
{
	if ( IsNewChatBox() ) {
		return;
	}

	if ( event_.m_dwlParam == KGCCheckButton::GC_CBUTTON_CHK )
	{
		if ( event_.m_pWnd == m_pkChatType[CT_ALL] )
		{
			bool bCheck = m_pkChatType[CT_ALL]->IsChecked();

			SetFilteringListInfo( CT_WHISPER, !bCheck);
			SetFilteringListInfo( CT_GUILD,   !bCheck);
			SetFilteringListInfo( CT_GENERAL, !bCheck);
		}
		else
		{
			for( int i = 0 ; i < CT_NUM ; ++i )
			{
				if ( event_.m_pWnd != m_pkChatType[i] )
					continue;                                
                if (i == CT_GENERAL)
                {
                    m_pkList->SetFilterInfo(KGCChatManager::CHAT_TYPE_COLORCHAT,!m_pkChatType[i]->IsChecked());
                }
				SetFilteringListInfo( i, !m_pkChatType[i]->IsChecked());

                FilterCheckMessageBox(i,m_pkChatType[i]->IsChecked());
			}
		}

		SetScrollHeight(GetScrollHeight()); //그냥 높이 그대로 재 할당 해줘도 리플래쉬한다.
		//ReLocate();
	}
	else
	{        
        if ( g_kGlobalValue.m_kGuildInfo.m_dwUID ==0 || g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ) //길드 없는 경우..
        {
            m_pkChatType[CT_GUILD]->SetSelfInputCheck( false );
            m_pkChatType[CT_GUILD]->Lock( true );
        }
        
		if ( event_.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		{
			ChangeChatTypePos( event_.m_pWnd, !m_bChatBtnFold );
		}
	}
}

void KGCChatBox::FoldingChatBox( bool bFold_ )
{
    if( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID == 0 )
    {
        m_pkChatType[CT_GUILD]->Lock(true);
        m_pkChatType[CT_GUILD]->Check(true);
    }
    else
    {
        m_pkChatType[CT_GUILD]->Lock(false);
        m_pkChatType[CT_GUILD]->Check(false);
    }

	for( int i = 0 ; i < CT_NUM ; ++i )
	{
		m_pkChatType[i]->ToggleRender( !bFold_ );
	}
}

std::wstring KGCChatBox::GetCurrentPreMark()
{
	if( m_iCurChatType < 0 || m_iCurChatType >= CT_NUM )
		return L"";

	return m_aPreMark[m_iCurChatType];
}

void KGCChatBox::ChangeChatTypePos( EChatType eType_, bool bFoldOpen_ )
{
	ChangeChatTypePos( m_pkChatType[eType_], bFoldOpen_ );
}

void KGCChatBox::ChangeChatTypePos( KD3DWnd* pWnd_, bool bFoldOpen_ )
{
	if ( pWnd_ != m_pkChatType[m_iCurChatType] )
	{
		// 위치 바꾸쟈 ㅎ
		D3DXVECTOR2 vecTemp = pWnd_->GetFixedWindowLocalPos();
		pWnd_->SetWindowPosDirect( m_pkChatType[m_iCurChatType]->GetFixedWindowLocalPos() );
		m_pkChatType[m_iCurChatType]->SetWindowPosDirect( vecTemp );

		//std::wstring strEditString = m_pkEdit->GetText();
		////이전의 기호를 찾는다.
		std::wstring strBeforePreMark = GetCurrentPreMark();


		//if( 0 == strEditString.find( strBeforePreMark, 0 ) )
		//	strEditString = strEditString.substr( strBeforePreMark.size() );

		//if( 0 == strEditString.find( m_strWhisperNickname, 0 ) )
		//	strEditString = strEditString.substr( m_strWhisperNickname.size() );
		//	
	
		// 바꾸자 ㅎ순서가 중요함
		m_iCurChatType = GetChatTypeEnum( pWnd_ );
		if( m_iCurChatType == -1 )
			m_iCurChatType = CT_ALL;

		SetText(GetCurrentPreMark());				
		//if( pWnd_ == m_pkChatType[CT_WHISPER] )
		//{
  //          //현재의 기호를 넣어준다.
  //          SetText( strBeforePreMark + strEditString );
		//	m_pkEdit->SetSelection( strBeforePreMark.size() + strEditString.size() +1 , strBeforePreMark.size() + strEditString.size() +1 );
		//}
		//else
		//{
		//SetFocus();		    
		//}
	}

	// 열려라 참깨!		
	FoldingChatBox( bFoldOpen_ );
	// ㅠㅅㅠ
	m_pkChatType[m_iCurChatType]->ToggleRender( true );	
	m_bChatBtnFold = bFoldOpen_;
	if( bFoldOpen_ == false )
		m_bPrevChatBtnFold = false;

	KGCChatManager::EChatCommand  eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
	EChatType eType = static_cast<EChatType>(m_iCurChatType);
	switch (eType)
	{
	case CT_GENERAL:             
		eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
		break;
	case CT_GUILD:
		eChatCmd = KGCChatManager::CHAT_CMD_GUILD;
		break;                
	case CT_WHISPER:                            
		eChatCmd = KGCChatManager::CHAT_CMD_WHISPER;
		break;                
	default:
		eChatCmd = KGCChatManager::CHAT_CMD_NORMAL;
		break;

	}
	g_pkChatManager->SetCommend(eChatCmd);


}

void KGCChatBox::InitChatTypeValues()
{
	m_aChatUIName[CT_ALL]       = "chat_btn_all";
	m_aChatUIName[CT_WHISPER]   = "chat_btn_whisper";
	m_aChatUIName[CT_GUILD]     = "chat_btn_guild";
	m_aChatUIName[CT_GENERAL]   = "chat_btn_general";

	m_aPreMark[CT_ALL]        = L"";
	m_aPreMark[CT_GENERAL]    = L"";
	m_aPreMark[CT_WHISPER]    = L">";
	m_aPreMark[CT_GUILD]      = L"";

	m_aChatType[CT_ALL]      = KGCChatManager::CHAT_TYPE_NORMAL;
	m_aChatType[CT_GENERAL]  = KGCChatManager::CHAT_TYPE_NORMAL;
	m_aChatType[CT_WHISPER]  = KGCChatManager::CHAT_TYPE_WHISPER;
	m_aChatType[CT_GUILD]    = KGCChatManager::CHAT_TYPE_GUILD;

	m_iCurChatType  = CT_ALL;

	if ( IsNewChatBox() ) {
		m_pkOptionList->ClickCheckBox ( CT_ALL );
	}

	KD3DWnd* pChatBox = GetChildByName( "chat_btn_list" );
	if ( pChatBox == NULL ) return;
		

	for( int i = 0 ; i < CT_NUM ; ++i )
	{
		m_pkChatType[i] = static_cast<KGCCheckButton*>(pChatBox->GetChildByName( m_aChatUIName[i] ));
		ASSERT( m_pkChatType[i] != NULL );

		m_pkChatType[i]->InitState( true, true, this );

		if( m_bFirstCrerate )
		{
			m_vChatTypeOribinalPos[i] = m_pkChatType[i]->GetFixedWindowLocalPos();
		}
		else
		{
			m_pkChatType[i]->SetWindowPosDirect( m_vChatTypeOribinalPos[i] );
		}
	}
	

}

int KGCChatBox::GetChatTypeEnum( KD3DWnd* pWnd_ )
{

	for( int i = 0 ; i < CT_NUM;  ++i )
	{
		if( m_pkChatType[i] == pWnd_ )
			return i;
	}
	return -1;
}


bool KGCChatBox::IsNewChatBox()
{
    if( g_pkWorldmapChatBox == this ) 
        return true;

    if( g_pkRoomChatBox == this ) 
      return true;
	  
	if ( g_pkLobbyChatBox == this )
		return true;

    return false;
}

const std::wstring KGCChatBox::GetEditStr( void ) const
{
	if( m_pkPaint_Btn && m_pkPaint_Btn->GetColorString() != L"FFFFFF" )
	{
		std::wstring rt ;
		if (g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN)
		{
			rt = L"#c" + m_pkPaint_Btn->GetColorString() + m_pkEdit->GetText() + L"#cX";
		}
		else
		{
			rt = m_pkEdit->GetText();
		}
		return rt;
	}
	return m_pkEdit->GetText();
}



IMPLEMENT_CLASSNAME( KGCChatBoxOption );
IMPLEMENT_WND_FACTORY( KGCChatBoxOption );
IMPLEMENT_WND_FACTORY_NAME( KGCChatBoxOption, "gc_chat_option_s6" );

KGCChatBoxOption::KGCChatBoxOption( void )
{
	for ( int i = 0 ; i < KCB_CHAT_MAX_NUM ; ++i ) {
		m_saCheckList[i] = NULL;
	}

	LINK_CONTROL ( "checkbox_notice", m_saCheckList[KCB_CHAT_ALL] );
	LINK_CONTROL ( "checkbox_normal", m_saCheckList[KCB_CHAT_NOMAL] );
	LINK_CONTROL ( "checkbox_whisper", m_saCheckList[KCB_CHAT_WHISPER] );
	LINK_CONTROL ( "checkbox_guild", m_saCheckList[KCB_CHAT_GUILD] );
}

KGCChatBoxOption::~KGCChatBoxOption( void )
{

}

void KGCChatBoxOption::ActionPerformed( const KActionEvent& event )
{

}

void KGCChatBoxOption::OnCreate( void )
{
	for ( int i = 0 ; i < KCB_CHAT_MAX_NUM ; ++i ) {
		if ( m_saCheckList[i] == NULL ) continue; // 특정 국가에 옵션 추가 될 수 있어서 안전성 검사 해준다.
		
		m_saCheckList[i]->InitState(true, true, this);
		m_saCheckList[i]->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChatBoxOption::OnClickCheckBox );
		m_saCheckList[i]->SetCheck(true);
	}

	InitOptionText();
}

void KGCChatBoxOption::OnClickCheckBox ( const KActionEvent& event )
{
	if ( event.m_pWnd == NULL) return;

	for ( int i = 0 ; i < KCB_CHAT_MAX_NUM ; ++i ) {
		if ( m_saCheckList[i] != event.m_pWnd ) continue;
		
		m_saCheckList[i]->ToggleCheck();
		ClickCheckBox(i);
		return;
	}

}

void KGCChatBoxOption::ClickCheckBox ( int iType )
{
	if ( iType == KCB_CHAT_ALL ) {
		bool bCheck = m_saCheckList[KCB_CHAT_ALL]->GetCheck();		

		for ( int i = 1 ; i < KCB_CHAT_MAX_NUM ; ++i ) {
			if ( m_saCheckList[i] == NULL ) continue; // 특정 국가에 옵션 추가 될 수 있어서 안전성 검사 해준다.

			m_saCheckList[i]->SetCheck(bCheck);		
		}
	}
	else {
		bool bCheck = true;
		for ( int i = KCB_CHAT_WHISPER ; i < KCB_CHAT_MAX_NUM ; ++i ) {
			if ( m_saCheckList[i] == NULL ) continue; // 특정 국가에 옵션 추가 될 수 있어서 안전성 검사 해준다.

			if ( m_saCheckList[i]->GetCheck() == false ) {
				bCheck = false;
			}
		}
		m_saCheckList[KCB_CHAT_ALL]->SetCheck(bCheck);		
	}
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_CHAT_OPTION_BTN_CLICK, iType ) );

}

std::map<int,bool> KGCChatBoxOption::GetCheck ( )
{
	std::map<int,bool> mapInfo;
	for ( int i = 0 ; i < KCB_CHAT_MAX_NUM ; ++i ) {
		if ( m_saCheckList[i] == NULL ) continue; // 특정 국가에 옵션 추가 될 수 있어서 안전성 검사 해준다.

		mapInfo[i] = m_saCheckList[i]->GetCheck();
	}

	return mapInfo;
}

void KGCChatBoxOption::InitOptionText()
{
	m_saCheckList[KCB_CHAT_ALL]->SetText(g_pkStrLoader->GetString( STR_ID_CHAT_OPTION_ALL ));
	m_saCheckList[KCB_CHAT_NOMAL]->SetText(g_pkStrLoader->GetString( STR_ID_CHAT_OPTION_NORMAL ));
	m_saCheckList[KCB_CHAT_WHISPER]->SetText(g_pkStrLoader->GetString( STR_ID_CHAT_OPTION_WHISPER ));
	m_saCheckList[KCB_CHAT_GUILD]->SetText(g_pkStrLoader->GetString( STR_ID_CHAT_OPTION_GUILD ));

}



IMPLEMENT_CLASSNAME( KGCChatBoxChatKind );
IMPLEMENT_WND_FACTORY( KGCChatBoxChatKind );
IMPLEMENT_WND_FACTORY_NAME( KGCChatBoxChatKind, "gc_chat_btn_list_s6" );


KGCChatBoxChatKind::KGCChatBoxChatKind( void )
: m_pkBack(NULL)
, m_pkOptionBtn(NULL)
, m_iSelBtn(KCB_CHAT_BTN_NOMAL)
{
	for ( int i = 0 ; i < KCB_CHAT_BTN_MAX_NUM ; ++i ) {
		m_saViewCurrentList[i] = NULL;
		m_saBtnList[i] = NULL;
	}

	LINK_CONTROL ( "whisper", m_saViewCurrentList[KCB_CHAT_BTN_WHISPER] );
	LINK_CONTROL ( "guild", m_saViewCurrentList[KCB_CHAT_BTN_GUILD] );
	LINK_CONTROL ( "normal", m_saViewCurrentList[KCB_CHAT_BTN_NOMAL] );

	LINK_CONTROL ( "whisper_btn", m_saBtnList[KCB_CHAT_BTN_WHISPER] );
	LINK_CONTROL ( "guild_btn", m_saBtnList[KCB_CHAT_BTN_GUILD] );
	LINK_CONTROL ( "normal_btn", m_saBtnList[KCB_CHAT_BTN_NOMAL] );

	LINK_CONTROL ( "list_open_btn", m_pkOptionBtn );

	LINK_CONTROL ( "chat_edit_back", m_pkBack );

	
}

KGCChatBoxChatKind::~KGCChatBoxChatKind( void )
{

}

void KGCChatBoxChatKind::OnCreate( void )
{
	for ( int i = 0 ; i < KCB_CHAT_BTN_MAX_NUM ; ++i ) {
		m_saViewCurrentList[i]->InitState(false ,false,NULL);
		m_saBtnList[i]->InitState(false,true,this);
		m_saBtnList[i]->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChatBoxChatKind::OnClickMenuBtn);
	}

	m_pkOptionBtn->InitState(true,true,this);
	m_pkOptionBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCChatBoxChatKind::OnClickOpenMenuBtn);

	m_pkBack->InitState(false, false,NULL);
	SetBtnNum(KCB_CHAT_BTN_NOMAL);
}

void KGCChatBoxChatKind::OnClickOpenMenuBtn( )
{
	bool bOn = !m_pkBack->IsRenderOn();
	SetBtnPos();
	SetBtnOnOff( bOn );
}

void KGCChatBoxChatKind::OnClickMenuBtn( const KActionEvent& event )
{

	for ( int i = 0 ; i < KCB_CHAT_BTN_MAX_NUM ; ++i ) {
		if ( m_saBtnList[i] == event.m_pWnd ) {
			SetBtnNum(i);
			break;		
		}
	}

	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_CHAT_KIND_BTN_CLICK, m_iSelBtn + 1 ) );

}


void KGCChatBoxChatKind::ActionPerformed( const KActionEvent& event )
{
	
}

void KGCChatBoxChatKind::SetBtnOnOff( bool bOn )
{
	for ( int i = 0 ; i < KCB_CHAT_BTN_MAX_NUM ; ++i ) {
		m_saBtnList[i]->ToggleRender( (m_iSelBtn != i) && bOn );
	}

	for ( int i = 0 ; i < KCB_CHAT_BTN_MAX_NUM ; ++i ) {
		m_saViewCurrentList[i]->ToggleRender( i == m_iSelBtn );
	}

	m_pkBack->ToggleRender( bOn );

}


void KGCChatBoxChatKind::SetBtnPos( )
{
	int iPos = 0;
	for ( int i = 0 ; i < KCB_CHAT_BTN_MAX_NUM ; ++i ) {
		//m_saBtnList[i]->ToggleRender( false );
		if( i == m_iSelBtn ) continue;
		float posY = 11.0f + ( iPos * 21.f);
		m_saBtnList[i]->SetWindowPosYDirect(posY);	
		++iPos;
	}
}

void KGCChatBoxChatKind::SetBtnNum( int iSel )
{
	m_iSelBtn = iSel; 
	SetBtnPos(); 
	SetBtnOnOff(false);
}