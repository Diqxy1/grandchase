#include "stdafx.h"
#include "KGCTutorialJob.h"

IMPLEMENT_CLASSNAME( KGCTutorialJob );
IMPLEMENT_WND_FACTORY( KGCTutorialJob );
IMPLEMENT_WND_FACTORY_NAME( KGCTutorialJob, "gc_tutorial_job" );

KGCTutorialJob::KGCTutorialJob( void )
{
    m_bFirst = true;
    char szTemp[128];
    for(int iLoop=0; iLoop<=NUM_JOB_LEVEL; ++iLoop)
    {
        if( iLoop< NUM_JOB_LEVEL ) { 
            sprintf(szTemp, "job_button%d", iLoop);
            LINK_CONTROL( szTemp, m_pkWndJobButton[iLoop] );
            sprintf(szTemp, "job_button_com%d", iLoop);
            LINK_CONTROL( szTemp, m_pkWndComJobButton[iLoop] );
        }
        else { 
            LINK_CONTROL( "job_button1_2",     m_pkWndJobButton[iLoop] );
            LINK_CONTROL( "job_button_com1_2", m_pkWndComJobButton[iLoop] );
        }
    }
}

KGCTutorialJob::~KGCTutorialJob( void ){}

void KGCTutorialJob::OnCreateComplete( void )
{
    if( m_bFirst ) { 
        m_bFirst = false;
        for(int iLoop=0; iLoop<=NUM_JOB_LEVEL; ++iLoop)
        {
            m_pkWndJobButton[iLoop]->InitState( true, true, this );
            m_pkWndComJobButton[iLoop]->InitState( true, true, this );
        }
    }
}

void KGCTutorialJob::ActionPerformed( const KActionEvent& event )
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
        for(int iLoop=0; iLoop<=NUM_JOB_LEVEL; ++iLoop)
        {
            if( event.m_pWnd == m_pkWndJobButton[iLoop] || event.m_pWnd == m_pkWndComJobButton[iLoop] )
            {
                if( iLoop == NUM_JOB_LEVEL ) { 
                    SpeakToActionListener( KActionEvent( this, 1 ) );
                }
                else { 
                    SpeakToActionListener( KActionEvent( this, iLoop ) );
                }
                return;
            }
        }
    }
}

void KGCTutorialJob::SelectJop(int iMaxLevel, int iJobLevel, bool bPlayerTap, bool bUseSpecialClass /*= false*/ )
{
    int iLoop;
    if( bPlayerTap ) {    
        if( bUseSpecialClass ) { 
            for( iLoop=0 ; iLoop<= iMaxLevel ; ++iLoop ) { 
                if( iLoop == iJobLevel && iJobLevel != 0  ) { 
                    m_pkWndJobButton[NUM_JOB_LEVEL]->SetWndMode( D3DWM_ACTIVE );
                    m_pkWndJobButton[NUM_JOB_LEVEL]->SetSelfInputCheck( false );
                }
                else if( iLoop == iJobLevel ) { 
                    m_pkWndJobButton[iLoop]->SetWndMode(D3DWM_ACTIVE);
                    m_pkWndJobButton[iLoop]->SetSelfInputCheck( false );

                    m_pkWndJobButton[NUM_JOB_LEVEL]->SetWndMode( D3DWM_DEFAULT );
                    m_pkWndJobButton[NUM_JOB_LEVEL]->SetSelfInputCheck( true );
                }
                else { 
                    m_pkWndJobButton[iLoop]->SetWndMode( D3DWM_DEFAULT );
                    m_pkWndJobButton[iLoop]->SetSelfInputCheck( true );
                }
            }
            for( ; iLoop<NUM_JOB_LEVEL ; ++iLoop ) { 
                m_pkWndJobButton[iLoop]->SetWndMode( D3DWM_LOCK );
                m_pkWndJobButton[iLoop]->SetSelfInputCheck( false );
            }

            return;
        }
        else { 
            m_pkWndJobButton[NUM_JOB_LEVEL]->ToggleRender( false );
        }

        for(iLoop=0; iLoop<=iMaxLevel; ++iLoop)
        {
            if( iLoop == iJobLevel )
            {
                m_pkWndJobButton[iLoop]->SetWndMode(D3DWM_ACTIVE);
                m_pkWndJobButton[iLoop]->SetSelfInputCheck( false );
            }
            else
            {
                m_pkWndJobButton[iLoop]->SetWndMode( D3DWM_DEFAULT );
                m_pkWndJobButton[iLoop]->SetSelfInputCheck( true );
            }
        }
        for(; iLoop<NUM_JOB_LEVEL; ++iLoop)
        {
            m_pkWndJobButton[iLoop]->SetWndMode( D3DWM_LOCK );
            m_pkWndJobButton[iLoop]->SetSelfInputCheck( false );
        }
    } 
    else {
        if( bUseSpecialClass ) { 
            for( iLoop=0 ; iLoop<= iMaxLevel ; ++iLoop ) { 
                if( iLoop == iJobLevel && iJobLevel != 0  ) { 
                    m_pkWndComJobButton[NUM_JOB_LEVEL]->SetWndMode( D3DWM_ACTIVE );
                    m_pkWndComJobButton[NUM_JOB_LEVEL]->SetSelfInputCheck( false );
                }
                else if( iLoop == iJobLevel ) { 
                    m_pkWndComJobButton[iLoop]->SetWndMode(D3DWM_ACTIVE);
                    m_pkWndComJobButton[iLoop]->SetSelfInputCheck( false );

                    m_pkWndComJobButton[NUM_JOB_LEVEL]->SetWndMode( D3DWM_DEFAULT );
                    m_pkWndComJobButton[NUM_JOB_LEVEL]->SetSelfInputCheck( true );
                }
                else { 
                    m_pkWndComJobButton[iLoop]->SetWndMode( D3DWM_DEFAULT );
                    m_pkWndComJobButton[iLoop]->SetSelfInputCheck( true );
                }
            }
            for( ; iLoop<NUM_JOB_LEVEL ; ++iLoop ) { 
                m_pkWndComJobButton[iLoop]->SetWndMode( D3DWM_LOCK );
                m_pkWndComJobButton[iLoop]->SetSelfInputCheck( false );
            }

            return;
        }
        else { 
            m_pkWndComJobButton[NUM_JOB_LEVEL]->ToggleRender( false );
        }

        for(iLoop=0; iLoop<=iMaxLevel; ++iLoop)
        {
            if( iLoop == iJobLevel )
            {
                m_pkWndComJobButton[iLoop]->SetWndMode(D3DWM_ACTIVE);
                m_pkWndComJobButton[iLoop]->SetSelfInputCheck( false );                
            }
            else
            {
                m_pkWndComJobButton[iLoop]->SetWndMode( D3DWM_DEFAULT );
                m_pkWndComJobButton[iLoop]->SetSelfInputCheck( true );
            }
        }
        for(; iLoop<NUM_JOB_LEVEL; ++iLoop)
        {
            m_pkWndComJobButton[iLoop]->SetWndMode( D3DWM_LOCK );
            m_pkWndComJobButton[iLoop]->SetSelfInputCheck( false );
        }
    }
}

void KGCTutorialJob::SetEnableButton( bool bSwitch, bool bUseSpecialClass /*= false*/ )
{
    for(int iLoop=0; iLoop<=NUM_JOB_LEVEL; ++iLoop)
    {
        m_pkWndJobButton[iLoop]->ToggleRender( bSwitch ); 
    }
    m_pkWndJobButton[NUM_JOB_LEVEL]->ToggleRender( bUseSpecialClass );
}

void KGCTutorialJob::SetEnableComButton( bool bSwitch, bool bUseSpecialClass /*= false*/ )
{
    for(int iLoop=0; iLoop<=NUM_JOB_LEVEL; ++iLoop)
    {       
        m_pkWndComJobButton[iLoop]->ToggleRender( bSwitch );
    }
    m_pkWndComJobButton[NUM_JOB_LEVEL]->ToggleRender( bUseSpecialClass );
}

void KGCTutorialJob::SetAllEnable(bool bSwitch, bool bPlayerTab, bool bUseSpecialClass /*= false*/ )
{
    for(int iLoop=0; iLoop<=NUM_JOB_LEVEL; ++iLoop)
    {
        if( bPlayerTab ) {
            m_pkWndJobButton[iLoop]->ToggleRender( bSwitch );
        } else {
            m_pkWndComJobButton[iLoop]->ToggleRender( bSwitch );
        }                        
    }
    if( bPlayerTab ) { 
        m_pkWndJobButton[NUM_JOB_LEVEL]->ToggleRender( bUseSpecialClass );
    }
    else { 
        m_pkWndComJobButton[NUM_JOB_LEVEL]->ToggleRender( bUseSpecialClass );
    }
}

void KGCTutorialJob::SetJobBtnLock( bool bLock, int iOpendPromotion )
{

    if( iOpendPromotion >= NUM_JOB_LEVEL ) {
        iOpendPromotion = NUM_JOB_LEVEL - 1;
    }

    for( int i = 0; i <= iOpendPromotion; i++ ) {
        m_pkWndJobButton[ i ]->Lock( bLock );
    }
    m_pkWndJobButton[NUM_JOB_LEVEL]->Lock( bLock );
}

void KGCTutorialJob::SetComJobBtnLock( bool bLock, int iOpendPromotion )
{

    if( iOpendPromotion >= NUM_JOB_LEVEL ) {
        iOpendPromotion = NUM_JOB_LEVEL - 1;
    }

    for( int i = 0; i <= iOpendPromotion; i++ ) {
        m_pkWndComJobButton[ i ]->Lock( bLock );
    }

    m_pkWndComJobButton[NUM_JOB_LEVEL]->Lock( bLock );
}


