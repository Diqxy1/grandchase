#include "stdafx.h"
#include "KGCFadeoutWnd.h"

IMPLEMENT_CLASSNAME( KGCFadeoutWnd );
IMPLEMENT_WND_FACTORY( KGCFadeoutWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCFadeoutWnd, "gc_fadeout_window" );

KGCFadeoutWnd::KGCFadeoutWnd( void )
{
}

KGCFadeoutWnd::~KGCFadeoutWnd( void )
{
}

void KGCFadeoutWnd::OnCreate( void )
{
    for( int i = 0; i < 256; ++i )
    {
        // 키 입력 모두 누르는 효과!
        SetHotKey( i );
    }
}

void KGCFadeoutWnd::FrameMoveInEnabledState( void )
{
}

void KGCFadeoutWnd::ActionPerformed( const KActionEvent& event )
{
}

void KGCFadeoutWnd::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}