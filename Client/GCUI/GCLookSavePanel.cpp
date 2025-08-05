#include "stdafx.h"
#include "GCLookSavePanel.h"

IMPLEMENT_CLASSNAME( KGCLookSavePanel );
IMPLEMENT_WND_FACTORY( KGCLookSavePanel );
IMPLEMENT_WND_FACTORY_NAME( KGCLookSavePanel, "gc_looksavepanel" );

KGCLookSavePanel::KGCLookSavePanel( void )
: m_pkBtnChange( NULL )
, m_pkBtnOpen( NULL )
, m_pkBtnSave( NULL )
{
    LINK_CONTROL( "btn_open",   m_pkBtnOpen );
    LINK_CONTROL( "btn_save",   m_pkBtnSave );
    LINK_CONTROL( "btn_change", m_pkBtnChange );
}

KGCLookSavePanel::~KGCLookSavePanel( void )
{
}

void KGCLookSavePanel::OnCreate( void )
{
    
    m_pkBtnSave->InitState( true );//, true, this );
    m_pkBtnSave->SetToolTip( g_pkStrLoader->GetString( STR_ID_NOUSE_EXPRANKING_TOOLTIP ) );
    m_pkBtnOpen->InitState( true );//, true, this );
    m_pkBtnOpen->SetToolTip( g_pkStrLoader->GetString( STR_ID_NOUSE_EXPRANKING_TOOLTIP ) );


#if defined(NATION_EU)
    m_pkBtnChange->InitState( true, false, NULL );
    m_pkBtnChange->SetToolTip( g_pkStrLoader->GetString( STR_ID_NOUSE_EXPRANKING_TOOLTIP ) );
#else

    m_pkBtnChange->InitState( true, true, this );
    m_pkBtnChange->DisconnectAllEventProcedure();
    m_pkBtnChange->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCLookSavePanel::OnClickBtnChange );

    if ( GC_GLOBAL_DEFINE::bReformCoordi ) 
    {
//         m_pkBtnSave->ToggleRender( false );
//         m_pkBtnOpen->ToggleRender( false );
    }

    
#endif
}

void KGCLookSavePanel::ActionPerformed( const KActionEvent& event )
{

}

void KGCLookSavePanel::OnClickBtnChange()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COORDI_CHANGE_DLG );
}