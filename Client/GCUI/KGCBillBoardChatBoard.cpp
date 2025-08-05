#include "StdAfx.h"
#include "KGCRoomManager.h"
#include "KGCBillBoardChatBoard.h"
#include "KGCBillboardChatManager.h"

IMPLEMENT_CLASSNAME( KGCBillBoardChatBoard );
IMPLEMENT_WND_FACTORY( KGCBillBoardChatBoard );
IMPLEMENT_WND_FACTORY_NAME( KGCBillBoardChatBoard, "gc_billboardchatboard" );

//00634E18  /$  6A 08         PUSH 8
KGCBillBoardChatBoard::KGCBillBoardChatBoard(void)
{
	m_pkBackNameLeft = NULL;
	m_pkBackNameCenter = NULL;
	m_pkBackNameRight = NULL;
	m_pkWndBillboard = NULL;
	m_pkStaticMsg = NULL;
	m_pkStaticNickname = NULL;
	m_pkWndBillboardNum = NULL;
	m_pkStaticBillboardNum = NULL;

	LINK_CONTROL( "back_name_left", m_pkBackNameLeft );
	LINK_CONTROL( "back_name_center", m_pkBackNameCenter );
	LINK_CONTROL( "back_name_right", m_pkBackNameRight );
	LINK_CONTROL( "wnd_billboard", m_pkWndBillboard );
	LINK_CONTROL( "static_msg", m_pkStaticMsg );
	LINK_CONTROL( "static_nickname", m_pkStaticNickname );
	LINK_CONTROL( "wnd_billboard_num", m_pkWndBillboardNum );
	LINK_CONTROL( "static_billboard_num", m_pkStaticBillboardNum );

	m_fWindowScaleX = 1.0f;
	m_bBlink = true;
	m_bToggle = true;
	this->m_iItemCount = 0;

}

KGCBillBoardChatBoard::~KGCBillBoardChatBoard(void)
{
}

void KGCBillBoardChatBoard::OnCreate()
{
	if( m_pkWndBillboard )
	{
		m_pkWndBillboard->InitState(true, true, this);
		m_pkWndBillboard->Lock( true );
	}
	
	if( m_pkStaticMsg )
	{
		m_pkStaticMsg->InitState(true, true, this); 
		m_pkStaticMsg->SetAutoScroll(false);
		m_pkStaticMsg->SetText("");
		m_pkStaticMsg->SetAlign( DT_LEFT );
	}

	if( m_pkStaticNickname )
	{
		m_pkStaticNickname->InitState(true, true, this); 
		m_pkStaticNickname->SetText("");
		m_pkStaticNickname->SetAlign( DT_LEFT );
	}
	
	if( m_pkWndBillboardNum )
	{
		m_pkWndBillboardNum->InitState(false, true, this);
	}
	
	if( m_pkStaticBillboardNum )
	{
		m_pkStaticBillboardNum->InitState(false, true, this);
		m_pkStaticBillboardNum->SetAlign( DT_CENTER );
	}
}

void KGCBillBoardChatBoard::OnCreateComplete()
{
}

void KGCBillBoardChatBoard::SetIngamePosition() {

	D3DXVECTOR2 dxv2Scale(1.0f, 1.0f);
	if (g_pGCDeviceManager2->GetWindowScaleX() >= 1024.0f)
		dxv2Scale.x = g_pGCDeviceManager2->GetWindowScaleX() / (1024.0f);

	if (g_pGCDeviceManager2->GetWindowScaleY() >= 768.0f)
		dxv2Scale.y = g_pGCDeviceManager2->GetWindowScaleY() / (768.0f);

	D3DXVECTOR2 vPos = D3DXVECTOR2(20, 180);

	if (GC_GM_QUEST64 == SiKGCRoomManager()->GetGameMode())
		vPos = D3DXVECTOR2(15.0f, 165.0f);
	this->SetAlignType(WNDUIAT_POSZEROBASE);
	this->SetWindowScale(dxv2Scale);
	this->SetFixedWindowLocalPos(vPos);
}

void KGCBillBoardChatBoard::FrameMoveInEnabledState()
{
	std::wstring strNickname;
	std::wstring strMessage;
	D3DCOLOR dwColor;
	SiKGCBillboardChatManager()->PopMessage(strNickname, strMessage, dwColor);

	if( strNickname.empty() )
	{
		m_pkBackNameLeft->ToggleRender( false );
		m_pkBackNameCenter->ToggleRender( false );
		m_pkBackNameRight->ToggleRender( false );
	}
	else
	{
		int iWidth = g_pkFontMgr->GetWidth( strNickname.c_str() );
		m_pkBackNameLeft->ToggleRender( true );
		m_pkBackNameCenter->ToggleRender( true );
		m_pkBackNameRight->ToggleRender( true );

		m_pkBackNameCenter->SetWidth(iWidth);
		D3DXVECTOR2 vPos = m_pkBackNameCenter->GetFixedWindowLocalPos();

		vPos.x += iWidth + m_pkBackNameRight->GetWidth() - 5;
		m_pkBackNameRight->SetFixedWindowLocalPos(vPos);

		vPos.x += static_cast<float>(m_pkBackNameRight->GetWidth() + 2);
		m_pkStaticMsg->SetFixedWindowLocalPos( vPos );
	}

	m_pkStaticMsg->SetFontColor( dwColor );
	m_pkStaticMsg->SetText( strMessage );
	m_pkStaticNickname->SetText( strNickname );
	m_pkWndBillboard->Lock( false );

	int nItemCount = g_pItemMgr->GetInventoryItemDuration(ITEM_BILLBOARD);
	if( nItemCount > 0 &&  m_bBlink )
	{
		if (m_pkWndBillboardNum)
		{
			m_pkWndBillboardNum->ToggleRender(true);
			m_pkStaticBillboardNum->ToggleRender(true);
			m_pkStaticBillboardNum->SetText(nItemCount);
		}
	}
	else
	{
		if (m_pkWndBillboardNum)
		{
			m_pkWndBillboardNum->ToggleRender(false);
			m_pkStaticBillboardNum->ToggleRender(false);
		}
	}

	if( nItemCount <= 0 )
	{
		m_pkWndBillboard->Lock( true );
	}
	
	m_pkWndBillboard->ToggleRender( m_bToggle );
}

void KGCBillBoardChatBoard::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkWndBillboard, KD3DWnd::D3DWE_BUTTON_CLICK, OnBillboardClick );
	GCWND_MSG_MAP( m_pkWndBillboard, KD3DWnd::D3DWE_BUTTON_UP, OnBillboardClick );
}

void KGCBillBoardChatBoard::OnBillboardClick()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_BILLBOARD_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
}