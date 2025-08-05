#include "stdafx.h"
#include "KGCHappyNewYearNPCDlg.h"

IMPLEMENT_CLASSNAME( KGCHappyNewYearNPCDlg );
IMPLEMENT_WND_FACTORY( KGCHappyNewYearNPCDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCHappyNewYearNPCDlg, "gc_happynewyear_dlg" );

KGCHappyNewYearNPCDlg::KGCHappyNewYearNPCDlg(void)
{
	m_iItemID = 0;
	m_pTexItem = NULL;

	LINK_CONTROL( "btn_ok",                 m_pkBtnOk1 );
	LINK_CONTROL( "btn_ok2",                m_pkBtnOk2 );
	LINK_CONTROL( "bar",                    m_pkBar );
	LINK_CONTROL( "gp",                     m_pkGp );

	LINK_CONTROL( "static_title",           m_pkContent );
	LINK_CONTROL( "static_gp",              m_pkGpContent );

	LINK_CONTROL( "background",             m_pkBackGround );
	LINK_CONTROL( "gift_background2",       m_pkItemBack );
	LINK_CONTROL( "gp_background2",         m_pkGPBack );

}

KGCHappyNewYearNPCDlg::~KGCHappyNewYearNPCDlg(void)
{

}

void KGCHappyNewYearNPCDlg::OnCreate()
{
	m_pkBtnOk1->InitState( true, true, this );
	m_pkBtnOk1->Lock( false );
	m_pkBtnOk2->InitState( true, true, this );
	m_pkBar->InitState( true, true, this );
	m_pkGp->InitState( true, true, this );
	m_pkContent->InitState( true, true, this );
	m_pkGpContent->InitState( true, true, this );
	m_pkBackGround->InitState( true, true, this );
	m_pkItemBack->InitState( true, true, this );
	m_pkGPBack->InitState( true, true, this );
	m_pkContent->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0) );

	m_pkContent->SetMultiLine( true );
	SetExtent( false );
}

void KGCHappyNewYearNPCDlg::FrameMoveInEnabledState( void )
{
}

void KGCHappyNewYearNPCDlg::ActionPerformed(const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnOk1,               KD3DWnd::D3DWE_BUTTON_CLICK,    OnExtention );
	GCWND_MSG_MAP( m_pkBtnOk2,               KD3DWnd::D3DWE_BUTTON_CLICK,    OnConfirm );
}

void KGCHappyNewYearNPCDlg::OnExtention(void)
{
	SetExtent( true );
	m_pkBtnOk1->Lock( true );
}

void KGCHappyNewYearNPCDlg::OnConfirm(void)
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCHappyNewYearNPCDlg::OnDestroy()
{
	SAFE_RELEASE( m_pTexItem );
}

void KGCHappyNewYearNPCDlg::OnDestroyComplete( void )
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCHappyNewYearNPCDlg::SetItem(int iItemID)
{
	SAFE_RELEASE(m_pTexItem);
	m_iItemID   = 0;
	//m_pkContent->SetText( L"" );
	//m_pkContent->SetTextAutoMultiline( L"" );

	if( m_iItemID != -1 )
	{
		m_pTexItem  = g_pItemMgr->CreateItemTexture( iItemID, 0 );	
		m_iItemID   = iItemID;

		//GCItem *pkItem = g_pItemMgr->GetItemData( iItemID );
		//m_pkTitle->SetFontColor( g_pItemMgr->GetItemStringColor( pkItem->eItemKind ) );
		//m_pkTitle->SetText( pkItem->strItemName.c_str() );
		//m_pkContent->SetTextAutoMultiline( pkItem->strItemName.c_str() );
	}
}

void KGCHappyNewYearNPCDlg::SetGpContent(std::wstring wstrContent)
{
	m_pkGpContent->SetText( wstrContent );
}

void KGCHappyNewYearNPCDlg::SetTitleContent(std::wstring wstrContent)
{
	m_pkContent->SetTextAutoMultiline( wstrContent );
}

void KGCHappyNewYearNPCDlg::PostChildDraw()
{
	if( m_pkItemBack->IsRenderOn() )
	{
		if ( m_pTexItem && m_iItemID != -1 )
		{
			D3DXVECTOR2 vPos( m_pkItemBack->GetFixedWindowPos() );

			vPos.x += 8.0f;
			vPos.y += 8.0f;

			g_pGCDeviceManager2->PushState();
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			{
				g_pGCDeviceManager2->DrawInScreen(m_pTexItem,
					vPos.x
					, vPos.y
					, vPos.x + 75.0f
					, vPos.y + 75.0f
					, 0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f,0xffffffff);
			}
			g_pGCDeviceManager2->PopState();
		}
	}
}

void KGCHappyNewYearNPCDlg::SetExtent( bool bExtent )
{
	m_pkBtnOk2->ToggleRender( bExtent );
	m_pkBar->ToggleRender( bExtent );
	m_pkGp->ToggleRender( bExtent );
	m_pkGpContent->ToggleRender( bExtent );
	m_pkItemBack->ToggleRender( bExtent );
	m_pkGPBack->ToggleRender( bExtent );

	if( bExtent )
		m_pkBackGround->SetSize( m_pkBackGround->GetWidth(), 310 );
	else
		m_pkBackGround->SetSize( m_pkBackGround->GetWidth(), 206 );
}