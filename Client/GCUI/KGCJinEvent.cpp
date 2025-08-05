#include "stdafx.h"
#include "KGCJinEvent.h"
#include "KDSound.h"
#include "GCUI/GCItemImgWnd.h" 
#include "MyD3D.h"
#include "Procedure.h"
#include "P2P.H"

IMPLEMENT_CLASSNAME( KGCJinEvent );
IMPLEMENT_WND_FACTORY( KGCJinEvent );
IMPLEMENT_WND_FACTORY_NAME( KGCJinEvent, "gc_jin_event" );

KGCJinEvent::KGCJinEvent(void)
{
	m_pkitem_Img_View	= NULL;
	m_pkCount_View		= NULL;
	m_pkOk				= NULL;
	m_pkCancle			= NULL;
	m_pkUp				= NULL;
	m_pkDown			= NULL;

    LINK_CONTROL( "item_img_view",			m_pkitem_Img_View );
	LINK_CONTROL( "poll_item_img_wnd",		m_pkCount_View );
	LINK_CONTROL( "btn_ok",					m_pkOk );
	LINK_CONTROL( "btn_cancel",				m_pkCancle );
	LINK_CONTROL( "up_arrow",				m_pkUp );
	LINK_CONTROL( "down_arrow",				m_pkDown );

	char str[MAX_PATH];
	for(int i=0;i<NUM_OF_JE_STAR;++i)
	{
		m_pkStar[i]		= NULL;
		m_pkStatic[i]	= NULL;
		sprintf( str, "star%d", i+1 );
		LINK_CONTROL( str,       m_pkStar[i] );
		sprintf( str, "static_text%d", i+1 );
		LINK_CONTROL( str,       m_pkStatic[i] );
	}

	m_iPollCount	= 0;
	m_iTotalCnt		= 0;
} 

KGCJinEvent::~KGCJinEvent(void)
{
}

void KGCJinEvent::OnCreate()
{
	m_iPollCount	= 0;

	m_pkitem_Img_View->InitState( true, true, this );
	m_pkCount_View->InitState( true, true, this );
	m_pkOk->InitState( true, true, this );
	m_pkCancle->InitState( true, true, this );
	m_pkUp->InitState( true, true, this );
	m_pkDown->InitState( true, true, this );

	m_pkUp->SetHotKey( DIK_UPARROW );
	m_pkDown->SetHotKey( DIK_DOWN );
	m_pkOk->SetHotKey( DIK_RETURN );
	m_pkCancle->SetHotKey( DIK_ESCAPE );

	RefreshMyItemCnt();

	for(int i=0;i<NUM_OF_JE_STAR;++i)
	{
		m_pkStar[i]->InitState( true );
		m_pkStatic[i]->InitState( true, true, this );

		m_pkStatic[i]->SetText( g_pkStrLoader->GetString(STR_ID_JIN_EVENT_STRING1 + i) );
		m_pkStatic[i]->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0) );
		m_pkStatic[i]->SetToolTip( g_pkStrLoader->GetString(STR_ID_JIN_EVENT_TOOLTIP1 + i) );
	}

	SetItemImg(JIN_EVENT_ITEMID);

	KLuaManager luaMgr;
	if( GCFUNC::LoadLuaScript( luaMgr, "jin_event.lua" ) == false )
		return;

	if( SUCCEEDED( luaMgr.BeginTable( "Goal" ) ) )
	{
		if( SUCCEEDED( luaMgr.BeginTable( "Count" ) ) )
		{
			for(int i=0;i<NUM_OF_JE_STAR;++i)
			{
				int iNumOfCnt = 0;
				if( FAILED( luaMgr.GetValue( i+1, iNumOfCnt ) ) )
					break;

				m_iGoalCount[i] = iNumOfCnt;
			}
			luaMgr.EndTable();
		}

		luaMgr.EndTable();
	}
}

void KGCJinEvent::FrameMoveInEnabledState()
{
}

void KGCJinEvent::Initialize()
{
}

void KGCJinEvent::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCancle,		KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_pkOk,			KD3DWnd::D3DWE_BUTTON_CLICK, OnOk );

	GCWND_MSG_MAP( m_pkUp,			KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUpArrowBtn );
	GCWND_MSG_MAP( m_pkDown,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClickDownArrowBtn );
}

void KGCJinEvent::PostChildDraw()
{
	D3DXVECTOR2 vPos = this->GetFixedWindowPos();

	D3DXVECTOR2 vPos1( 295.0f, 160.0f );
	g_pkUIMgr->RenderNumber("orange_gradation_num3", vPos + vPos1, m_iPollCount, D3DXCOLOR(0xffffffff), false, false, -1);

	D3DXVECTOR2 vPos2( 338.0f, 112.0f );
	g_pkUIMgr->RenderNumber("red_gradation_num", vPos + vPos2, m_iMyItemCnt, D3DXCOLOR(0xffffffff), false);

}

void KGCJinEvent::SetItemImg( DWORD dwItemID)
{
	m_pkCount_View->SetItemInfo(dwItemID);
}

void KGCJinEvent::OnClickUpArrowBtn()
{
	m_iPollCount++;

	if( 0 > m_iPollCount )
		m_iPollCount = 0;

	if( 999 < m_iPollCount )
		m_iPollCount = 999;

	if( m_iMyItemCnt <= m_iPollCount )
		m_iPollCount = m_iMyItemCnt;
}

void KGCJinEvent::OnClickDownArrowBtn()
{
	--m_iPollCount;

	if( 0 > m_iPollCount )
		m_iPollCount = 0;

	if( 999 < m_iPollCount )
		m_iPollCount = 999;

	if( m_iMyItemCnt <= m_iPollCount )
		m_iPollCount = m_iMyItemCnt;
}

void KGCJinEvent::OnExit()
{
//	SiKGCValentineDayManager()->SetPollingNow(FALSE);
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCJinEvent::OnOk()
{
	if( 0 == m_iPollCount )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VALENTINE_DAY_ERROR4), L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );

		return;
	}

	Result_JinEvent_Poll = INT_MAX;
	KP2P::GetInstance()->Send_GymDonateReq( 100, JIN_EVENT_ITEMID, m_iPollCount );
	g_MyD3D->WaitForServerAck( Result_JinEvent_Poll, INT_MAX, 5000, TIME_OUT_VALUE );

	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_JIN_EVENT_MSG );
}

void KGCJinEvent::SetTotalCount( int iTotalCnt )
{ 
	m_iTotalCnt = iTotalCnt;

	for(int i=0;i<NUM_OF_JE_STAR;++i)
	{
		if( m_iGoalCount[i] <= m_iTotalCnt )
			m_pkStar[i]->SetWndMode(D3DWM_ACTIVE);
		else
			m_pkStar[i]->SetWndMode(D3DWM_DEFAULT);
	}
}

void KGCJinEvent::RefreshMyItemCnt()
{
	KInventoryItem* pItem = NULL;
	if( g_pItemMgr->FindInventoryForItemID( JIN_EVENT_ITEMID, &pItem ) )
	{
		m_iMyItemCnt = pItem->m_iDuration;
	}
	else
	{
		m_iMyItemCnt = 0;
	}
}