#include "stdafx.h"
#include "KGCNewClearSealJobBtn.h"

IMPLEMENT_CLASSNAME( KGCNewClearSealJobBtn );
IMPLEMENT_WND_FACTORY( KGCNewClearSealJobBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCNewClearSealJobBtn, "gc_clear_seal_job_btn" );

KGCNewClearSealJobBtn::KGCNewClearSealJobBtn( void )
{
    char szTemp[128];
    for( int i = 0; i < NUM_JOB_LEVEL; ++i )
    {        
        sprintf(szTemp, "job_button%d", i);
        LINK_CONTROL( szTemp, m_pkWndJobButton[i] );
    }
}

KGCNewClearSealJobBtn::~KGCNewClearSealJobBtn( void ){}

void KGCNewClearSealJobBtn::OnCreateComplete( void )
{
    for( int i = 0; i < NUM_JOB_LEVEL; ++i)
    {
        m_pkWndJobButton[i]->InitState( true, true, this );		
    }
}

void KGCNewClearSealJobBtn::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == this )
    {  return; }

    // 롤오버소리
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {  g_KDSound.Play( "30" ); }

    // 클릭소리
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN ||
        event.m_dwCode == KD3DWnd::D3DWE_RBUTTON_DOWN )
    {  g_KDSound.Play( "31" ); }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        for(int i=0; i<NUM_JOB_LEVEL; ++i)
        {
            if( event.m_pWnd == m_pkWndJobButton[i] )
            {
               SpeakToActionListener( KActionEvent( this, i ) );
               return;
            }
        }
    }
}

void KGCNewClearSealJobBtn::SelectJop(int iMaxLevel, int iJobLevel, bool bPlayerTap)
{
    int i;
    for(i = 0; i <= iMaxLevel; ++i)
    {
        if( i == iJobLevel )
        {
            m_pkWndJobButton[i]->SetWndMode(D3DWM_ACTIVE);
            m_pkWndJobButton[i]->SetSelfInputCheck( false );
        }
        else
        {
            m_pkWndJobButton[i]->SetWndMode( D3DWM_DEFAULT );
            m_pkWndJobButton[i]->SetSelfInputCheck( true );
        }
    }
    for(; i<NUM_JOB_LEVEL; ++i)
    {
        m_pkWndJobButton[i]->SetWndMode( D3DWM_LOCK );
        m_pkWndJobButton[i]->SetSelfInputCheck( false );
    }
}

void KGCNewClearSealJobBtn::SetEnableButton(bool bSwitch)
{
    for(int i = 0; i < NUM_JOB_LEVEL; ++i)
    {
        m_pkWndJobButton[i]->ToggleRender( bSwitch );
    }
}

void KGCNewClearSealJobBtn::SetJopBnt( int iHavePromotion_ )
{
    for ( int i=0 ; i<NUM_JOB_LEVEL ; i++ )
    {
        if ( i<=iHavePromotion_ )
        {
            m_pkWndJobButton[i]->SetWndMode( D3DWM_DEFAULT );
            m_pkWndJobButton[i]->SetSelfInputCheck( true );
        }
        else
        {
            m_pkWndJobButton[i]->SetWndMode( D3DWM_DEFAULT );
            m_pkWndJobButton[i]->SetSelfInputCheck( false );
        }
    }

}
