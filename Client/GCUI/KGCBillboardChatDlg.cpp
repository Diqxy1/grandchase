#include "stdafx.h"
#include "KGCBillboardChatDlg.h"
#include "GCDungeonTitleColorS6.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME( KGCBillboardChatDlg );
IMPLEMENT_WND_FACTORY( KGCBillboardChatDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCBillboardChatDlg, "gc_billboardchatdlg" );

KGCBillboardChatDlg::KGCBillboardChatDlg(void) :
	m_iMessageMaxLenght(50),
	m_pkBtnConfirm(nullptr),
	m_pkBtnClose(nullptr),
	m_pkStaticLenght(nullptr),
	m_pkStaticAvailableCnt(nullptr),
	m_pkEditMsg(nullptr),
	m_pkPaintBtnSet(nullptr),
	m_bInit( false )
{
	LINK_CONTROL( "btn_confirm", m_pkBtnConfirm );
	LINK_CONTROL( "btn_close", m_pkBtnClose );
	LINK_CONTROL( "static_length", m_pkStaticLenght );
	LINK_CONTROL( "static_available_cnt", m_pkStaticAvailableCnt );
	LINK_CONTROL( "edit_msg", m_pkEditMsg );
	LINK_CONTROL( "paint_btn_set", m_pkPaintBtnSet );
	
	m_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
}

KGCBillboardChatDlg::~KGCBillboardChatDlg(void)
{
}

void KGCBillboardChatDlg::OnCreate(void)
{
	g_pkUIMgr->DisconnectAllEventProcedure(GetUID());

	if( m_pkBtnClose)
	{
		m_pkBtnClose->InitState(true, true, this);
		m_pkBtnClose->SetHotKey(DIK_ESCAPE);
	}
	
	if( m_pkBtnConfirm )
	{
		m_pkBtnConfirm->InitState(true, true, this);
	}

	if( m_pkStaticLenght )
	{
		m_pkStaticLenght->InitState(true, true);
		m_pkStaticLenght->SetAlign( DT_RIGHT );
	}

	if( m_pkStaticAvailableCnt )
	{
		m_pkStaticAvailableCnt->InitState(true, true);
		m_pkStaticAvailableCnt->SetAlign( DT_RIGHT );
		m_pkStaticAvailableCnt->SetText("");
	}

	if( m_pkEditMsg )
	{
		m_pkEditMsg->InitState(true, true, this);
		m_pkEditMsg->SetMultiLine( true );
		m_pkEditMsg->SetNewLine( true );
		m_pkEditMsg->SetLimitLine( 10 );
		m_pkEditMsg->SetLimitText(m_iMessageMaxLenght);
		m_pkEditMsg->SetFontColor( D3DCOLOR_RGBA(90, 90, 90, 255) );
		m_pkEditMsg->SetText(L"");
	}

	if(m_pkPaintBtnSet)
	{
		m_pkPaintBtnSet->InitState(true, true, this);
	}

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN, this, &KGCBillboardChatDlg::OnClickColorBtn);

	//CheckPaint();
}

void KGCBillboardChatDlg::CheckPaint()
{
	if (g_pItemMgr->IsCashPaintItem() || g_pItemMgr->IsPaintItem())
	{
		m_pkPaintBtnSet->ToggleRender(true);
	}
	else
	{
		m_pkPaintBtnSet->ToggleRender(false);
	}
}

void KGCBillboardChatDlg::OnCreateComplete( void )
{
	InitCreate();

	int nItemCount = g_pItemMgr->GetInventoryItemDuration(ITEM_BILLBOARD);
	m_pkStaticAvailableCnt->SetText(nItemCount);
	m_pkEditMsg->SetText(g_pkStrLoader->GetString(STR_ID_BILLBOARD_CHAT_TEMP_TEXT1).c_str());
}

void KGCBillboardChatDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkBtnConfirm, KD3DWnd::D3DWE_BUTTON_CLICK, OnConfirm );

	if( event.m_pWnd == m_pkEditMsg && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		m_bInit = true;
		m_pkEditMsg->SetText(L"");
		m_pkEditMsg->SetFontColor( D3DCOLOR_RGBA(255, 255, 255, 255) );
		m_pkEditMsg->SetLimitText(m_iMessageMaxLenght);
	}
}

void KGCBillboardChatDlg::FrameMoveInEnabledState()
{
	if( m_bInit )
	{
		InitCreate();
	}
}

void KGCBillboardChatDlg::InitCreate()
{
	std::wstring text = m_pkEditMsg->GetText();
	WCHAR tempCount[10];
	swprintf(tempCount, sizeof(tempCount), L"%d / %d", text.length(), m_iMessageMaxLenght);
	m_pkStaticLenght->SetText(tempCount);

	m_pkBtnConfirm->Lock(text.empty());
	m_pkPaintBtnSet->ToggleRender( true );
}

void KGCBillboardChatDlg::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCBillboardChatDlg::OnClickColorBtn()
{
	m_Color = SiKGCRoomListManager()->GetCreateRoomTitleColor();
}

void KGCBillboardChatDlg::OnConfirm()
{
	if(g_kGlobalValue.m_kUserInfo.eGCUserLevel >= USERLEVEL_NOMAL)
	{
		SiKP2P()->Send_BillboardChatRegistrationReq(m_pkEditMsg->GetText(), m_Color);
	}

	OnClose();
}