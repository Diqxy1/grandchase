//#include "stdafx.h"
//#include "KGCValentineDayDlg.h"
//
//#include "../MyD3D.h"
//#include "GCUI/GCItemImgWnd.h" 
//
//#include "Procedure.h"
//#include "../KGCValentineDayManager.h"
//
//
//IMPLEMENT_CLASSNAME( KGCValentineDayDlg );
//IMPLEMENT_WND_FACTORY( KGCValentineDayDlg );
//IMPLEMENT_WND_FACTORY_NAME( KGCValentineDayDlg, "gc_valentineday_dlg" );
//
//KGCValentineDayDlg::KGCValentineDayDlg(void)
//: m_pkOkBtn( NULL )
//, m_pkCancelBtn( NULL )
//, m_pkUpArrowBtn( NULL )
//, m_pkDownArrowBtn( NULL )
//, m_pkPollCountView( NULL )
//, m_pkStaticTitle( NULL )
//, m_pkPollItemImgView( NULL )
//, m_iPollCount( 0 )
//, m_iBtnPressCount( 0 )
//{
//	LINK_CONTROL( "btn_ok", m_pkOkBtn);
//	LINK_CONTROL( "btn_cancel", m_pkCancelBtn);
//	LINK_CONTROL( "up_arrow", m_pkUpArrowBtn);
//	LINK_CONTROL( "down_arrow", m_pkDownArrowBtn);
//	LINK_CONTROL( "item_count_view", m_pkPollCountView);
//	LINK_CONTROL( "static_title", m_pkStaticTitle);
//	LINK_CONTROL( "poll_item_img_wnd", m_pkPollItemImgView);
//}
//
//KGCValentineDayDlg::~KGCValentineDayDlg(void)
//{
//
//}
//
//void KGCValentineDayDlg::OnCreate()
//{
//	m_pkOkBtn->InitState( true, true, this);
//	m_pkCancelBtn->InitState( true, true, this);
//    m_pkCancelBtn->SetHotKey(DIK_ESCAPE);
//	m_pkUpArrowBtn->InitState( true, true, this );
//	m_pkDownArrowBtn->InitState( true, true, this );
//	m_pkStaticTitle->InitState( true, true, this);
//	m_pkPollCountView->InitState( true );
//	m_pkPollItemImgView->InitState( true );
//}
//
//void KGCValentineDayDlg::OnCreateComplete( void )
//{
//	OnInitPressCount();
//	m_iPollCount = 0;
//	m_pkOkBtn->Lock(false);
//#if !defined(NATION_USA) && !defined( NATION_EU )
//	m_pkStaticTitle->SetText( g_pkStrLoader->GetReplacedString(STR_ID_VALENTINE_DAY_DLG_TITLE, "L", SiKGCValentineDayManager()->GetSelectedCharString() ) );
//#endif
//	DWORD dwPollItemID = SiKGCValentineDayManager()->GetPollItemID();
//
//	if( 0 == dwPollItemID)
//	{
//		m_pkPollItemImgView->ToggleRender(false);
//	}else
//	{
//		m_pkPollItemImgView->ToggleRender(true);
//		m_pkPollItemImgView->SetItemInfo(dwPollItemID);
//	}
//}
//
//void KGCValentineDayDlg::FrameMoveInEnabledState( void )
//{
//}
//
//void KGCValentineDayDlg::ActionPerformed(const KActionEvent& event )
//{
//	GCWND_MSG_MAP( m_pkCancelBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
//	GCWND_MSG_MAP( m_pkOkBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnOk );
//
//	GCWND_MSG_MAP( m_pkUpArrowBtn,		KD3DWnd::D3DWE_BUTTON_DOWN, OnClickUpArrowBtn );
//	GCWND_MSG_MAP( m_pkDownArrowBtn,		KD3DWnd::D3DWE_BUTTON_DOWN, OnClickDownArrowBtn );
//
//	GCWND_MSG_MAP( m_pkUpArrowBtn,		KD3DWnd::D3DWE_BUTTON_PRESS, OnPressUpArrowBtn );
//	GCWND_MSG_MAP( m_pkDownArrowBtn,		KD3DWnd::D3DWE_BUTTON_PRESS, OnPressDownArrowBtn );
//
//// 	GCWND_MSG_MAP( m_pkUpArrowBtn,		KD3DWnd::D3DWE_BUTTON_UP, OnInitPressCount );
//// 	GCWND_MSG_MAP( m_pkDownArrowBtn,		KD3DWnd::D3DWE_BUTTON_UP, OnInitPressCount );
//	//SpeakToActionListener( event );
//}
//
//void KGCValentineDayDlg::OnExit()
//{
//	SiKGCValentineDayManager()->SetPollingNow(FALSE);
//	g_KDSound.Play( "31" );
//	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
//}
//
//void KGCValentineDayDlg::OnOk()
//{
//	if(0 == m_iPollCount )
//	{
//		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VALENTINE_DAY_ERROR4), L"",
//			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
//
//		return;
//	}
//		
//
//	Result_Char_Poll = INT_MAX;
//
//	KP2P::GetInstance()->Send_CharPollReq( static_cast< const char >(SiKGCValentineDayManager()->GetSelectedCharType() ) , SiKGCValentineDayManager()->GetPollItemID(), m_iPollCount );
//	
//	g_MyD3D->WaitForServerAck( Result_Char_Poll, INT_MAX, 5000, TIME_OUT_VALUE );
//}
//
//void KGCValentineDayDlg::OnPressUpArrowBtn()
//{
//	if(++m_iBtnPressCount > 5)
//	{
//		PollCountUp();
//		m_iBtnPressCount = 0;
//	}
//}
//
//void KGCValentineDayDlg::OnPressDownArrowBtn()
//{
//	if(++m_iBtnPressCount > 5)
//	{
//		PollCountDown();
//		m_iBtnPressCount = 0;
//	}
//}
//
//void KGCValentineDayDlg::OnDestroyComplete( void )
//{
//	m_iPollCount = 0;
//	m_pkOkBtn->Lock(false);
//}
//
//void KGCValentineDayDlg::PostChildDraw()
//{
//	D3DXVECTOR2 vPos = this->GetFixedWindowPos();
//
//    // 투표수
//    float fX = 125.0f;
//    if( 0 < (m_iPollCount/10) ) fX = 108.0f;
//    D3DXVECTOR2 vPos1( (vPos.x * m_fWindowScaleX) + (fX * m_fWindowScaleX), (vPos.y * m_fWindowScaleY) + (60.0f * m_fWindowScaleY) );
//    g_pkUIMgr->RenderNumber("orange_gradation_num3", vPos1, m_iPollCount, D3DXCOLOR(0xffffffff), true, false, -1);
//
//    // 용지갯수
//    D3DXVECTOR2 vPos2( (vPos.x * m_fWindowScaleX) + (25.0f * m_fWindowScaleX), (vPos.y * m_fWindowScaleY) + (92.0f * m_fWindowScaleY) );
//    g_pkUIMgr->RenderNumber("blue_gradation_num", vPos2, SiKGCValentineDayManager()->GetPollItemCurrentCount(), D3DXCOLOR(0xffffffff), true);
//}
//
//void KGCValentineDayDlg::SetItemImg( DWORD dwItemID)
//{
//	m_pkPollItemImgView->SetItemInfo(dwItemID);
//}
//
//void KGCValentineDayDlg::PollCountUp()
//{
//	if(0 == SiKGCValentineDayManager()->GetPollItemCurrentCount() )
//		return;
//
//	++m_iPollCount;
//	if( m_iPollCount > static_cast<int>(SiKGCValentineDayManager()->GetPollItemCurrentCount()) || m_iPollCount > 99 )
//		m_iPollCount = 0;
//}
//
//void KGCValentineDayDlg::PollCountDown()
//{
//	if(0 == SiKGCValentineDayManager()->GetPollItemCurrentCount() )
//		return;
//
//	if(--m_iPollCount < 0)
//		m_iPollCount = SiKGCValentineDayManager()->GetPollItemCurrentCount() < 99 ? static_cast<int>(SiKGCValentineDayManager()->GetPollItemCurrentCount() ) : 99 ;
//}
//
//void KGCValentineDayDlg::OnClickUpArrowBtn()
//{
//	PollCountUp();
//	OnInitPressCount();
//}
//
//void KGCValentineDayDlg::OnClickDownArrowBtn()
//{
//	PollCountDown();
//	OnInitPressCount();
//}