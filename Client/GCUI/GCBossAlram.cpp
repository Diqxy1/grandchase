#include "stdafx.h"
#include "GCBossAlram.h"

IMPLEMENT_CLASSNAME( KGCBossAlram );
IMPLEMENT_WND_FACTORY( KGCBossAlram );
IMPLEMENT_WND_FACTORY_NAME( KGCBossAlram, "gc_boss_alram" );

KGCBossAlram::KGCBossAlram( void )
: m_iBlinkFrame( 0 )
, m_iBlinkCount( 0 )
, m_iCurrentFrame( 0 )
, m_iDivAlpha( 0 )
{
    for( int i=0 ; i< ALRAM_UI_CNT ; ++i ) { 
        m_pkAlram[i] = NULL;
    }

    LINK_CONTROL( "alram_left",     m_pkAlram[0] );
    LINK_CONTROL( "alram_right",    m_pkAlram[1] ); 
}

KGCBossAlram::~KGCBossAlram( void )
{
}

void KGCBossAlram::OnCreate( void )
{
    for( int i=0 ; i<ALRAM_UI_CNT ; ++i ) { 
        m_pkAlram[i]->InitState( false, false, this );
    }
}

void KGCBossAlram::ActionPerformed( const KActionEvent& event ) {}

void KGCBossAlram::FrameMoveInEnabledState( void )
{
    if( m_iBlinkCount <= 0 ) { 
        return;
    }

    if( m_iCurrentFrame < m_iBlinkFrame ) { 
        SetAlramAlpha( ALRAM_ALPHA_MAX - ( m_iDivAlpha * m_iCurrentFrame ) );
        ++m_iCurrentFrame;
    }

    if( m_iCurrentFrame >= m_iBlinkFrame ) { 
        m_iCurrentFrame = 0;
        --m_iBlinkCount;
    }

    if( m_iBlinkCount == 0 ) { 
        ToggleRenderAlram( false );
    }
}

void KGCBossAlram::StartAlram( const int iBlinkFrame_, const int iBlinkCount_ )
{
    if( iBlinkFrame_ <= 0 || iBlinkCount_ <= 0 ) { 
        return;
    }

    m_iBlinkFrame = iBlinkFrame_;
    m_iBlinkCount = iBlinkCount_;
    m_iCurrentFrame = 0;
    m_iDivAlpha = ALRAM_ALPHA_MAX / m_iBlinkFrame;

    ToggleRenderAlram( true );
    SetAlramAlpha( ALRAM_ALPHA_MAX );
}

void KGCBossAlram::SetAlramAlpha( int iAlpha_ )
{
    if( iAlpha_ < ALRAM_ALPHA_MIN ) { 
        iAlpha_ = ALRAM_ALPHA_MIN;
    }

    if( iAlpha_ > ALRAM_ALPHA_MAX ) { 
        iAlpha_ = ALRAM_ALPHA_MAX;
    }

    for( int i=0 ; i<ALRAM_UI_CNT ; ++i ) { 
        m_pkAlram[i]->SetAllWindowAlpha( iAlpha_ );
    }
}

void KGCBossAlram::ToggleRenderAlram( const bool bToggle_ )
{
    this->ToggleRender( bToggle_ );

    for( int i=0 ; i<ALRAM_UI_CNT ; ++i ) { 
        m_pkAlram[i]->ToggleRender( bToggle_ );
    }
}