#include "stdafx.h"
#include "GCCoordiCatalogDlg.h"

IMPLEMENT_CLASSNAME( KGCCoordiCatalogDlg );
IMPLEMENT_WND_FACTORY( KGCCoordiCatalogDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiCatalogDlg, "gc_coordi_catalog_dlg" );

#define LOCK_BUTTON_COLOR D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f )
#define ACTIVE_BUTTON_COLOR D3DXCOLOR( 0.97f, 0.75f, 0.03f, 1.0f )
#define NORMAL_BUTTON_COLOR D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f )

KGCCoordiCatalogDlg::KGCCoordiCatalogDlg( void )
{
    m_iCurrentPage      = -1;
    m_iTotalPage        = -1;

    m_pkBtnExit     = NULL;
    m_pkBtnExit     = NULL;
    m_pkBtnPrev     = NULL;
    m_pkBtnNext     = NULL;
    LINK_CONTROL( "btn_exit",   m_pkBtnExit );
    LINK_CONTROL( "btn_prev",   m_pkBtnPrev );
    LINK_CONTROL( "btn_next",   m_pkBtnNext );

    for( int i=0 ; i<MAX_NUM_BTN ; ++i )
    {
        char szBtnName[128];
        m_apkCatalogBtn[i] = NULL;
        
        sprintf_s( szBtnName, 127, "btn_page%d", i );
        LINK_CONTROL( szBtnName, m_apkCatalogBtn[i] );
    }

    m_pkItemImage = NULL;
}

KGCCoordiCatalogDlg::~KGCCoordiCatalogDlg( void )
{
    SAFE_RELEASE( m_pkItemImage );
}

void KGCCoordiCatalogDlg::OnCreate( void )
{
    m_pkBtnExit->InitState(true, true, this );
    m_pkBtnExit->SetHotKey( DIK_ESCAPE );
    m_pkBtnPrev->InitState( true, true, this );   
    m_pkBtnNext->InitState( true, true, this );   

    for(int i=0 ; i<MAX_NUM_BTN ; ++i )
        m_apkCatalogBtn[i]->InitState( false, true, this );
}

bool KGCCoordiCatalogDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    // 페이지 
    m_iCurrentPage = 0;
    m_iTotalPage = static_cast<int>( m_vecCatalog.size() );

    // 각 숫자 버튼 
    if( m_iTotalPage >= MAX_NUM_BTN )
    {
        for( int i=0 ; i<MAX_NUM_BTN ; ++i )
            m_apkCatalogBtn[i]->ToggleRender( true );
    }
    else
    {
        for( int i=0 ; i<m_iTotalPage ; ++i )
            m_apkCatalogBtn[i]->ToggleRender( true );
        
        for( int i=m_iTotalPage ; i<MAX_NUM_BTN ; ++i )
        {
            m_apkCatalogBtn[i]->ToggleRender( true );
            m_apkCatalogBtn[i]->SetSelfInputCheck( false );
            m_apkCatalogBtn[i]->SetWindowColor( LOCK_BUTTON_COLOR );
        }
    }

    OnNumBtn( m_iCurrentPage );
    return true;
}

void KGCCoordiCatalogDlg::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnPrev && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnPageBtn( PAGE_PREV );
    if( event.m_pWnd == m_pkBtnNext && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnPageBtn( PAGE_NEXT );

    for( int i=0 ; i<MAX_NUM_BTN ; ++i )
    {
        if( event.m_pWnd == m_apkCatalogBtn[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            m_iCurrentPage  = i;
            OnNumBtn( i );
            break;
        }

        if( event.m_pWnd == m_apkCatalogBtn[i] && event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        {
            if( 9 == i)
                m_apkCatalogBtn[i]->SetAllWindowColor( ACTIVE_BUTTON_COLOR );
            else
                m_apkCatalogBtn[i]->SetWindowColor( ACTIVE_BUTTON_COLOR );
        }
        else if( i != m_iCurrentPage )
        {
            if( 9 == i)
                m_apkCatalogBtn[i]->SetAllWindowColor( NORMAL_BUTTON_COLOR );
            else
                m_apkCatalogBtn[i]->SetWindowColor( NORMAL_BUTTON_COLOR );
        }
    }

    GCWND_MSG_MAP( m_pkBtnExit, KD3DWnd::D3DWE_BUTTON_CLICK,    OnExit );
}

void KGCCoordiCatalogDlg::PostChildDraw( void )
{
    if( m_pkItemImage == NULL )
        return;


	float x = 134.f;
	float y = 106.f;
	float fw = 756.0f;
	float fh = 519.0f;

	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
 //   LUA_GET_VALUE_DEF( "x", x, 24.0f );
 //   LUA_GET_VALUE_DEF( "y", y, 26.0f );
 //   LUA_GET_VALUE_DEF( "fw", fw, 780.0f );
 //   LUA_GET_VALUE_DEF( "fh", fh, 545.0f );
    g_pGCDeviceManager2->DrawInScreen( m_pkItemImage, 
        x * g_pGCDeviceManager2->GetWindowScaleX(), 
        y * g_pGCDeviceManager2->GetWindowScaleY(), 
        (x+fw) * g_pGCDeviceManager2->GetWindowScaleX(), 
        (y+fh) * g_pGCDeviceManager2->GetWindowScaleY(),
        0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 0xffffffff );

    m_pkBtnExit->Render();
}

void KGCCoordiCatalogDlg::OnExit( void )
{
    SpeakToActionListener( KActionEvent(this, EWNDMESSAGE_CLOSE) );
}

void KGCCoordiCatalogDlg::SetCatalogInfo( std::vector< std::string>& vecCatalog )
{
    m_vecCatalog = vecCatalog;
}

void KGCCoordiCatalogDlg::SetItemImage( int iIndex )
{
    SAFE_RELEASE( m_pkItemImage );

    if( iIndex < 0 || iIndex > m_iTotalPage-1 )
        return;

    m_pkItemImage = g_pGCDeviceManager2->CreateTexture( m_vecCatalog[iIndex].c_str() );
}

void KGCCoordiCatalogDlg::OnPageBtn( int iPageType )
{
    switch( iPageType )
    {
    case PAGE_PREV:
        if( m_iCurrentPage > 0 )
            --m_iCurrentPage;
        break;

    case PAGE_NEXT:
        if( m_iCurrentPage < m_iTotalPage-1 )
            ++m_iCurrentPage;
        break;
    }

    OnNumBtn( m_iCurrentPage );
}

void KGCCoordiCatalogDlg::OnNumBtn( int iSelectNum )
{
    for( int i=0 ; i<MAX_NUM_BTN ; ++i )
    {
        if( i == iSelectNum )
        {
            if( 9 == iSelectNum)
            {
                m_apkCatalogBtn[i]->SetAllWindowColor(ACTIVE_BUTTON_COLOR);
            }
            else
                m_apkCatalogBtn[i]->SetWindowColor( ACTIVE_BUTTON_COLOR );
        }
        else
        {
            if( 9 == iSelectNum)
                m_apkCatalogBtn[i]->SetAllWindowColor( NORMAL_BUTTON_COLOR );
            else
                m_apkCatalogBtn[i]->SetWindowColor( NORMAL_BUTTON_COLOR );
        }
    }
    
        
    SetItemImage( m_iCurrentPage );
}
