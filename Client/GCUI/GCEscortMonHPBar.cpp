#include "stdafx.h"
#include "GCEscortMonHPBar.h"

IMPLEMENT_CLASSNAME( KGCEscortMonHPBar );
IMPLEMENT_WND_FACTORY( KGCEscortMonHPBar );
IMPLEMENT_WND_FACTORY_NAME( KGCEscortMonHPBar, "gc_escort_mon_hp_bar" );

KGCEscortMonHPBar::KGCEscortMonHPBar( void )
: m_fCurHP( .0f )
, m_fMaxHP( .0f )
, m_iTimer( 0 )
, m_iAlpha( 0 )
, m_fAlphaChange( .0f )
, m_iSelectImg( 0 )
, m_pkBarBack( NULL )
, m_pkHPBar( NULL )
{
    char szTemp[128];
    for(int iLoop=0; iLoop<MAX_IMG; ++iLoop)
    {
        m_apkMonIcon[iLoop] = NULL;
        sprintf(szTemp, "Img%d", iLoop);
        LINK_CONTROL( szTemp, m_apkMonIcon[iLoop] );
    }
    LINK_CONTROL( "barBack",  m_pkBarBack );
    LINK_CONTROL( "bar",      m_pkHPBar );
}

KGCEscortMonHPBar::~KGCEscortMonHPBar( void )
{
}

void KGCEscortMonHPBar::OnCreate( void )
{
    for(int iLoop=0; iLoop<MAX_IMG; ++iLoop)
    {
        if( m_apkMonIcon[iLoop] )
            m_apkMonIcon[iLoop]->InitState( false );
    }
    
    m_pkBarBack->InitState( true );
    m_pkHPBar->InitState( true );

    m_pkHPBar->SetWidth( MIN_BAR );

    m_iSelectImg = 0;
    if( m_apkMonIcon[m_iSelectImg] )
        m_apkMonIcon[m_iSelectImg]->InitState( true );
}

void KGCEscortMonHPBar::ActionPerformed( const KActionEvent& event )
{

}

void KGCEscortMonHPBar::FrameMoveInEnabledState( void )
{
    for(int iLoop=0; iLoop<MAX_IMG; ++iLoop)
    {
        if( m_apkMonIcon[iLoop] )
            m_apkMonIcon[iLoop]->ToggleRender( false );
    }
    if( m_apkMonIcon[m_iSelectImg] )
        m_apkMonIcon[m_iSelectImg]->ToggleRender( true );

    if( m_iTimer > 0 ) { 
        m_iAlpha = static_cast<int>( static_cast< float >( m_iAlpha ) - m_fAlphaChange );

        if( m_iAlpha < 0 ) {
            m_iAlpha = 255;
        }

        m_pkHPBar->SetAllWindowAlpha( m_iAlpha );
        --m_iTimer;
    }
    else { 
        m_iTimer = 0;
        m_iAlpha = 255;
        m_pkHPBar->SetAllWindowAlpha( m_iAlpha );
    }
}

void KGCEscortMonHPBar::ShowHPBar( bool bShow_ )
{
    if( m_fMaxHP == .0f ) { 
        this->ToggleRender( false );
        return;
    }

    this->ToggleRender( bShow_ );
}

void KGCEscortMonHPBar::SetHPBar( float fMaxHP_, float fCurHP_ )
{
    if( fMaxHP_ <= 0 ) {
        return;
    }

    if( fCurHP_ > fMaxHP_ ) { 
        fCurHP_ = fMaxHP_;
    }

    m_fCurHP = fCurHP_;
    m_fMaxHP = fMaxHP_;

    if( m_fMaxHP == m_fCurHP ) { 
        m_pkHPBar->SetWidth( MAX_BAR );
        m_pkHPBar->ToggleRender( true );
    }
    else if ( m_fCurHP == 0.0f ) { 
        m_pkHPBar->SetWidth( MIN_BAR );
        m_pkHPBar->ToggleRender( false );
    }
    else { 
        float fBarWidth = static_cast<float>( MAX_BAR ) * ( m_fCurHP / m_fMaxHP );
        m_pkHPBar->SetWidth( static_cast<DWORD>( fBarWidth ) );
        m_pkHPBar->ToggleRender( true );
    }
}

void KGCEscortMonHPBar::ClearHPBar( void )
{
    m_fCurHP = .0f;
    m_fMaxHP = .0f;
    this->ToggleRender( false );
}

void KGCEscortMonHPBar::SetWarning( void )
{
    m_iTimer = WARNING_TIME;
    m_iAlpha = 255;
    m_fAlphaChange = 255.0f / 40.0f;
    m_pkHPBar->SetAllWindowAlpha( m_iAlpha );
}

void KGCEscortMonHPBar::SelectImg( int iImgIndex ) 
{
    if( 0 > iImgIndex || iImgIndex >= MAX_IMG )
        iImgIndex = 0;

    m_iSelectImg = iImgIndex;

    for(int iLoop=0; iLoop<MAX_IMG; ++iLoop)
    {
        if( m_apkMonIcon[iLoop] )
            m_apkMonIcon[iLoop]->InitState( false );
    }

    if( m_apkMonIcon[m_iSelectImg] )
        m_apkMonIcon[m_iSelectImg]->InitState( true );
}