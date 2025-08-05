#include "stdafx.h"
#include "GCCharIconWnd.h"

IMPLEMENT_CLASSNAME( KGCCharIconWnd );
IMPLEMENT_WND_FACTORY( KGCCharIconWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCharIconWnd, "gc_char_icon_wnd" );

KGCCharIconWnd::KGCCharIconWnd( void )
{
    for( int i=0 ; i<GC_CHAR_NUM ; ++i )
    {
        m_apkIcon[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, 127, "char%d", i );
        LINK_CONTROL( szTemp, m_apkIcon[i] );
    }

    m_iCurIndex = -1;
}

KGCCharIconWnd::~KGCCharIconWnd( void )
{
}

void KGCCharIconWnd::OnCreate( void )
{
    for( int i=0 ; i<GC_CHAR_NUM ; ++i )
        m_apkIcon[i]->InitState( false );
}

void KGCCharIconWnd::ActionPerformed( const KActionEvent& event )
{

}

void KGCCharIconWnd::ShowCharIcon( int iIndex, bool bLock )
{
    if( iIndex < 0 || iIndex >= GC_CHAR_NUM )
        return;

    if( m_iCurIndex > 0 )
        m_apkIcon[ m_iCurIndex ]->ToggleRender( false );

    m_apkIcon[ iIndex ]->ToggleRender( true );
    m_apkIcon[ iIndex ]->SetShow( bLock );
    m_iCurIndex = iIndex;
}