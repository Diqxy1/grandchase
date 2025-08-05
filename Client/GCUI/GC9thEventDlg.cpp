#include "stdafx.h"
#include "GC9thEventDlg.h"

IMPLEMENT_CLASSNAME( KGC9thEventDlg );
IMPLEMENT_WND_FACTORY( KGC9thEventDlg );
IMPLEMENT_WND_FACTORY_NAME( KGC9thEventDlg, "gc_gc9th_event_dlg" );

const GCITEMID TickID = 100069;
const char* strLinkPage = "http://chase.netmarble.net/news/notice/BbsContentView.asp?seq=8119021&searchstring=&searchstringoptionval=1&searchmonth=0&searchrange=0&cateId=0&choice=0&usememosp=0&SearchYear=2012&pageno=1";

KGC9thEventDlg::KGC9thEventDlg( void )
: m_pkBtnClose( NULL )
, m_pkBtnLink( NULL )
, m_pkStaticCount( NULL )
, m_pkStaticDesc0( NULL )
, m_pkStaticDesc1( NULL )
, m_pkStaticDesc2( NULL )
, m_pkStaticLink0( NULL )
, m_pkStaticLink1( NULL )
, m_pkCountUI( NULL )
, m_iTicketCount( 0 )
{
    LINK_CONTROL( "btnColse", m_pkBtnClose );       
    LINK_CONTROL( "btnLink", m_pkBtnLink );

    LINK_CONTROL( "staticCount", m_pkStaticCount );
    LINK_CONTROL( "staticDesc0", m_pkStaticDesc0 );
    LINK_CONTROL( "staticDesc1", m_pkStaticDesc1 );
    LINK_CONTROL( "staticDesc2", m_pkStaticDesc2 );
    
    LINK_CONTROL( "staticLink0", m_pkStaticLink0 );
    LINK_CONTROL( "staticLink1", m_pkStaticLink1 );

    LINK_CONTROL( "count", m_pkCountUI );
}

KGC9thEventDlg::~KGC9thEventDlg( void )
{
}

void KGC9thEventDlg::OnCreate( void )
{
    m_iTicketCount = 0;

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGC9thEventDlg::OnClickClose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnLink->InitState( true, true, this );
    m_pkBtnLink->DisconnectAllEventProcedure();
    m_pkBtnLink->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGC9thEventDlg::OnClickLink );

    m_pkStaticCount->InitState( false );
    m_pkStaticCount->SetFontSize( 14 );
    m_pkStaticCount->SetAlign( DT_LEFT );

    m_pkStaticLink0->SetFontSize( 9 );
    m_pkStaticLink1->SetFontSize( 9 );
    m_pkStaticLink0->SetFontColor( D3DCOLOR_ARGB( 255, 76, 118, 160 ) );
    m_pkStaticLink1->SetFontColor( D3DCOLOR_ARGB( 255, 76, 118, 160 ) );

    m_pkStaticCount->SetText( g_pkStrLoader->GetString( STR_ID_9TH_EVENT_COUNT ) );
    m_pkStaticDesc0->SetText( g_pkStrLoader->GetString( STR_ID_9TH_EVENT_DESC0 ) );
    m_pkStaticDesc1->SetText( g_pkStrLoader->GetString( STR_ID_9TH_EVENT_DESC1 ) );
    m_pkStaticDesc2->SetText( g_pkStrLoader->GetString( STR_ID_9TH_EVENT_DESC2 ) );
    m_pkStaticLink0->SetText( g_pkStrLoader->GetString( STR_ID_9TH_EVENT_LINK0 ) );
    m_pkStaticLink1->SetText( g_pkStrLoader->GetString( STR_ID_9TH_EVENT_LINK1 ) );

    m_pkCountUI->InitState( false );
    m_pkCountUI->SetStringType( "9th_event_num", false );
}

bool KGC9thEventDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    SetTicketCount();
    SetCountUI();

    return true;
}

void KGC9thEventDlg::ActionPerformed( const KActionEvent& event ) { }

void KGC9thEventDlg::SetTicketCount( void )
{
    KItem* pItem = g_pItemMgr->GetInventoryItemFromID( TickID );
    if( pItem == NULL ) { 
        m_iTicketCount = 0;
    }
    else { 
        m_iTicketCount = pItem->m_nCount;
    }
}

void KGC9thEventDlg::SetCountUI( void )
{
    m_pkCountUI->SetNumber( m_iTicketCount );
    m_pkCountUI->ToggleRender( true );
}

void KGC9thEventDlg::OnClickClose( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void CloseLinkPage( void )
{
    if(g_pkUIScene->IsMsgBoxModal() == false)
        g_pkUIScene->Stop( false );

    ::SetFocus( g_hUIMainWnd );
}

void KGC9thEventDlg::OnClickLink( void )
{
    if( 0 == strcmp( strLinkPage, "" ) ) { 
        return;
    }

    // 웹브라우저를 띄우기 전에 버퍼 카운트를 맞춰준다.
    int cnt;
    for ( cnt = 0; !g_MyD3D->IsPrimaryBuffer && cnt < 100; ++cnt )
    {
        if ( D3D_OK == g_MyD3D->m_pd3dDevice->Present( NULL, NULL, NULL, NULL ) )
        {
            g_MyD3D->IsPrimaryBuffer = !g_MyD3D->IsPrimaryBuffer;
        }
    }
    if ( 100 == cnt ) {
        return;
    }

    RECT rc =
    {
        W_Width / 42, 
        W_Height / 32,
        W_Width - ( W_Width / 42 ),
        W_Height - ( W_Height / 32 ),
    };

    if ( false == g_pBrowser->Create( rc, g_MyD3D->m_hWnd ) ) {
        return;
    }

    g_pBrowser->Navigate( strLinkPage, "" );
    g_pBrowser->SetEndFunction( CloseLinkPage );
    g_pkUIScene->Stop( true );
}

