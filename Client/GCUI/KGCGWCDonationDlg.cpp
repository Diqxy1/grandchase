//#include "stdafx.h"
//
//#include "KGCGWCDonationDlg.h"
//#include "gcui/GCItemImgWnd.h"
//#include "GCItemManager.h"
//
//IMPLEMENT_CLASSNAME( KGCGWCDonationDlg );
//IMPLEMENT_WND_FACTORY( KGCGWCDonationDlg );
//IMPLEMENT_WND_FACTORY_NAME( KGCGWCDonationDlg, "gc_gwc_donation_dlg" );
//
//#define MAX_DONATION_NUM 99
//
//KGCGWCDonationDlg::KGCGWCDonationDlg(void)
//{
//	m_iNum = 0;
//	m_dwEventItemID = 0;
//	m_nAccCount = 0;
//
//	m_pkBtnCancle = NULL;
//	m_pkBtnOK = NULL;
//	m_pkBtnUp = NULL;
//	m_pkBtnDown = NULL;
//	m_pkStaticDesc = NULL;
//	m_pkDonationItem = NULL;
//	m_pkNumber[0] = NULL;
//	m_pkNumber[1] = NULL;
//	m_pkBtnAll = NULL;
//
//	LINK_CONTROL( "btn_cancle", m_pkBtnCancle );
//	LINK_CONTROL( "btn_ok", m_pkBtnOK );
//	LINK_CONTROL( "btn_up", m_pkBtnUp );
//	LINK_CONTROL( "btn_down", m_pkBtnDown );
//	LINK_CONTROL( "static_desc", m_pkStaticDesc );
//	LINK_CONTROL( "item_img", m_pkDonationItem );
//	LINK_CONTROL( "num0", m_pkNumber[0] );
//	LINK_CONTROL( "num1", m_pkNumber[1] );
//	LINK_CONTROL( "btn_all", m_pkBtnAll );
//}
//
//KGCGWCDonationDlg::~KGCGWCDonationDlg(void)
//{
//
//}
//
//void KGCGWCDonationDlg::OnCreate(void)
//{
//	m_pkBtnCancle->InitState( true, true, this);
//	m_pkBtnOK->InitState( true, true, this);
//	m_pkBtnUp->InitState( true, true, this);
//	m_pkBtnDown->InitState( true, true, this);
//	m_pkBtnAll->InitState( true, true, this);
//
//	m_pkStaticDesc->InitState( true );
//	m_pkDonationItem->InitState( true );
//	m_pkNumber[0]->InitState( true );
//	m_pkNumber[1]->InitState( true );
//
//	InitNumberTextureCoordinate();
//
//	m_pkNumber[0]->SetNumber( 0 );
//	m_pkNumber[1]->SetNumber( 0 );
//
//	m_pkStaticDesc->SetFontColor( 0xff000000 );
//}
//
//void KGCGWCDonationDlg::InitNumberTextureCoordinate( void )
//{
//	D3DXVECTOR4 vStartCoord1( 216.0f, 157.0f, 243.0f, 189.0f );
//	int iOffsetArray1[] = { 0, 29, 30, 29, 28, 29, 29, 28, 30, 28 };
//
//	for ( int i = 0; i < 10; ++i )
//	{
//		vStartCoord1.x += iOffsetArray1[i];
//		vStartCoord1.z += iOffsetArray1[i];
//
//		m_pkNumber[0]->SetNumberCoordi( i, vStartCoord1 );
//	}
//
//	m_pkNumber[1]->SetTexCoordi( m_pkNumber[0]->GetTexCoordi() );
//}
//
//void KGCGWCDonationDlg::ActionPerformed( const KActionEvent& event )
//{
//	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
//		return;
//
//	GCWND_MSG_MAP( m_pkBtnCancle, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancle );
//	GCWND_MSG_MAP( m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
//
//	if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
//	{
//		int iMaxNum = g_pItemMgr->GetInventoryItemDuration(m_dwEventItemID);
//		if( event.m_pWnd == m_pkBtnUp)
//		{
//			// 아이템 가진 수량 체크하자
//			if( m_iNum < iMaxNum || m_iNum < MAX_DONATION_NUM )
//				m_iNum++;
//		}
//		else if( event.m_pWnd == m_pkBtnDown )
//		{
//			if( m_iNum > 0 )
//				m_iNum--;
//		}
//		else if( event.m_pWnd == m_pkBtnAll )
//		{
//			m_iNum = ( iMaxNum > MAX_DONATION_NUM ) ? MAX_DONATION_NUM: iMaxNum;
//		}
//
//// 		// 99개 넘어가면 99개로 표시되고 기부하는 수는 그대로.
//// 		if( m_iNum > 99 )
//// 		{
//// 			m_pkNumber[0]->SetNumber( 9 );
//// 			m_pkNumber[1]->SetNumber( 9 );
//// 		}
//// 		else
//// 		{
//			m_pkNumber[0]->SetNumber( m_iNum%10 );
//			m_pkNumber[1]->SetNumber( (m_iNum/10)%10 );
//// 		}
//	}
//}
//
//void KGCGWCDonationDlg::OnCancle()
//{
//	m_iNum = 0;
//	g_KDSound.Play( "31" );
//	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
//}
//
//void KGCGWCDonationDlg::OnOK()
//{
//	KP2P::GetInstance()->Send_GWCDonationReq( m_iNum );
//}
//
//void KGCGWCDonationDlg::SetItem( DWORD dwItemID, int nAccCount )
//{
//	m_dwEventItemID = dwItemID;
//	m_nAccCount = nAccCount;
//
//	m_pkDonationItem->ToggleRender( true );
//	m_pkDonationItem->SetItemInfo( m_dwEventItemID );
//	m_pkDonationItem->ShowItem( true );
//
//	const std::wstring& strDesc = g_pkStrLoader->GetReplacedString( STR_ID_GWC_DONATION_DESC, "i", m_nAccCount);
//
//	m_pkStaticDesc->SetText( strDesc );
//	m_pkStaticDesc->SetTextAutoMultiline( strDesc );
//}