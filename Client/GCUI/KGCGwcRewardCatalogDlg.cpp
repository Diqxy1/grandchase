#include "stdafx.h"
#include "KGCGwcRewardCatalogDlg.h"

IMPLEMENT_CLASSNAME( KGCGwcRewardCatalogDlg );
IMPLEMENT_WND_FACTORY( KGCGwcRewardCatalogDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGwcRewardCatalogDlg, "gc_gwc_reward_catalog_dlg" );

KGCGwcRewardCatalogDlg::KGCGwcRewardCatalogDlg( void )
: m_pkExit (NULL)
{
    m_pkItemImage = NULL;
    LINK_CONTROL("btn_exit" , m_pkExit);
}

KGCGwcRewardCatalogDlg::~KGCGwcRewardCatalogDlg( void )
{
    SAFE_RELEASE( m_pkItemImage );
}

void KGCGwcRewardCatalogDlg::OnCreate( void )
{
    m_pkExit->InitState( true, true, this);
    m_pkExit->DisconnectAllEventProcedure();
    m_pkExit->SetHotKey(DIK_ESCAPE);
    m_pkExit->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCGwcRewardCatalogDlg::OnClickClose );
    
    SetItemImage();
}

void KGCGwcRewardCatalogDlg::ActionPerformed( const KActionEvent& event )
{

}

void KGCGwcRewardCatalogDlg::PostChildDraw()
{
    if( m_pkItemImage == NULL )
        return;

    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

    //LUA_GET_VALUE_DEF( "x", x, 24.0f );
    //LUA_GET_VALUE_DEF( "y", y, 26.0f );
    //LUA_GET_VALUE_DEF( "fw", fw, 780.0f );
    //LUA_GET_VALUE_DEF( "fh", fh, 545.0f );

    g_pGCDeviceManager2->DrawInScreen( m_pkItemImage, 
        22.0f * g_pGCDeviceManager2->GetWindowScaleX(), 
        36.0f * g_pGCDeviceManager2->GetWindowScaleY(), 
        760.0f * g_pGCDeviceManager2->GetWindowScaleX(), 
        519.0f * g_pGCDeviceManager2->GetWindowScaleY(),
        0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 0xffffffff );

    m_pkExit->Render();
}

void KGCGwcRewardCatalogDlg::OnClickClose( void )
{
    this->Destroy();
    g_pkUIScene->EndMessageBox();
}

void KGCGwcRewardCatalogDlg::SetItemImage( )
{
    SAFE_RELEASE( m_pkItemImage );
    
    m_pkItemImage = g_pGCDeviceManager2->CreateTexture( "gwc_catalog.dds" );
}

