#include "stdafx.h"
#include "GCEscortMerryDesc.h"

IMPLEMENT_CLASSNAME( KGCEscortMerryDesc );
IMPLEMENT_WND_FACTORY( KGCEscortMerryDesc );
IMPLEMENT_WND_FACTORY_NAME( KGCEscortMerryDesc, "gc_escort_merry_desc" );

KGCEscortMerryDesc::KGCEscortMerryDesc( void )
: m_iAlpha( 0 )
, m_iShowFrame( 0 )
, m_iFadeInFrame( 0 )
, m_iFadeOutFrame( 0 )
, m_fAlphaChange( 0.0f ) 
, m_pkBack( NULL )
, m_pkCharacter( NULL )
, m_pkStaticDesc( NULL )
{
    LINK_CONTROL( "back",   m_pkBack );
    LINK_CONTROL( "character", m_pkCharacter );
    LINK_CONTROL( "static_desc", m_pkStaticDesc );
}

KGCEscortMerryDesc::~KGCEscortMerryDesc( void )
{
}

void KGCEscortMerryDesc::OnCreate( void )
{
    m_pkBack->InitState( false );
    m_pkCharacter->InitState( false );
    m_pkStaticDesc->InitState( false );
}

void KGCEscortMerryDesc::ActionPerformed( const KActionEvent& event )
{

}

void KGCEscortMerryDesc::FrameMoveInEnabledState( void )
{
    if( 0 < m_iShowFrame )
    {
        if( m_iFadeInFrame < m_iShowFrame ) {
            m_iAlpha += static_cast<int>( m_fAlphaChange + 0.5f );
        }
        else if( m_iFadeOutFrame > m_iShowFrame ) {
            m_iAlpha -= static_cast<int>( m_fAlphaChange + 0.5f );
        }

        if( 0 > m_iAlpha ) { 
            m_iAlpha = 0;
        }
        else if( 255 < m_iAlpha ) {
            m_iAlpha = 255;
        }

        m_pkBack->SetAllWindowAlpha( m_iAlpha );
        m_pkCharacter->SetAllWindowAlpha( m_iAlpha );
        m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(m_iAlpha, 255, 249, 217) );
        m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0) );

        --m_iShowFrame;
    }
    else
    {
        OnHide();
    }
}

void KGCEscortMerryDesc::ShowEscortMerryDesc( int iSrtingID_, int iShowFrame_ )
{
    if( iShowFrame_ <= 0 ) { 
        OnHide();
        return;
    }

    m_iAlpha = 0;
    m_iShowFrame = iShowFrame_;
    m_iFadeInFrame = m_iShowFrame - static_cast<int>( m_iShowFrame / 5.0f );
    m_iFadeOutFrame = static_cast<int>( m_iShowFrame / 3.0f );
    m_fAlphaChange = 255.0f / static_cast<float>( m_iShowFrame / 5.0f );
    
    m_pkBack->ToggleRender( true );
    m_pkCharacter->ToggleRender( true );
    m_pkStaticDesc->ToggleRender( true );

    m_pkStaticDesc->SetMultiLine( true );
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( iSrtingID_ ) );

    this->SetAllWindowAlpha( 0 );
    m_pkBack->SetAllWindowAlpha( m_iAlpha );
    m_pkCharacter->SetAllWindowAlpha( m_iAlpha );
    m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(m_iAlpha, 255, 249, 217) );
    m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0) );

    this->ToggleRender( true );
}

void KGCEscortMerryDesc::OnHide( void )
{
    m_iAlpha = 0;
    m_iShowFrame = 0;
    m_iFadeInFrame = 0;
    m_iFadeOutFrame = 0;
    m_fAlphaChange = 0.0f;

    this->ToggleRender( false );
}