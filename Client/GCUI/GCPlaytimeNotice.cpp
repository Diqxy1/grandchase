#include "stdafx.h"
#include "GCPlaytimeNotice.h"

IMPLEMENT_CLASSNAME( KGCPlaytimeNotice );
IMPLEMENT_WND_FACTORY( KGCPlaytimeNotice );
IMPLEMENT_WND_FACTORY_NAME( KGCPlaytimeNotice, "gc_playtime_notice" );

KGCPlaytimeNotice::KGCPlaytimeNotice( void )
: m_iPlayTime( 0 )
, m_iShowTimer( 0 )
, m_iShowIndex( -1 )
, m_iShowAlpha( 255 )
, m_bShow( false )
{
    for( int i=0 ; i<NUM_NOTICE ; ++i ) { 
        char szTemp[ 128 ];
        sprintf_s( szTemp, 127, "notice%d", i );

        m_apkNotice[ i ] = NULL;
        LINK_CONTROL( szTemp, m_apkNotice[ i ] );
    }
	m_pkTimeValue = NULL;
	LINK_CONTROL( "time_value", m_pkTimeValue );

}

KGCPlaytimeNotice::~KGCPlaytimeNotice( void )
{
}

void KGCPlaytimeNotice::OnCreate( void )
{
    for( int i=0 ; i<NUM_NOTICE ; ++i ) { 
        m_apkNotice[ i ]->InitState( false );
    }

    m_iShowTimer = 0;
    m_iShowIndex = 0;
    m_bShow  = false;

    m_pkTimeValue->ToggleRender( true );
	m_pkTimeValue->SetStringType("ui_small_num");
}

void KGCPlaytimeNotice::ActionPerformed( const KActionEvent& event )
{

}

void KGCPlaytimeNotice::FrameMoveInEnabledState( void )
{
    if( false == m_bShow ) { 
        return;
    }

    if( m_bShow ) { 
        if( m_apkNotice[ m_iShowIndex ]->IsRenderOn() == false ) { 
            m_apkNotice[ m_iShowIndex ]->ToggleRender( true );
            m_apkNotice[ m_iShowIndex ]->SetAllWindowAlpha( 255 );
        }

        if( m_iShowTimer > ALPHA_START_TIME ) { 
            m_iShowAlpha = 255 - static_cast<int>( (m_iShowTimer - ALPHA_START_TIME) * 2.318f  ); // 255/110 = 2.318
            m_apkNotice[ m_iShowIndex ]->SetAllWindowAlpha( m_iShowAlpha >= 0 ? m_iShowAlpha : 0 );  

            if( m_iShowIndex == 0 && m_pkTimeValue->IsRenderOn() )  {
                m_pkTimeValue->SetAllWindowAlpha( m_iShowAlpha );
            }
        }

        if( ++m_iShowTimer > SHOW_TIME ) { 
            NextNotice();
        }
    }
}

void KGCPlaytimeNotice::StartNotice( int iIndex_, int iPlayTime_ /*= 0*/ )
{
    if( 0 > iIndex_ || NUM_NOTICE <= iIndex_ ) { 
        return;
    }

    for( int i=0 ; i<NUM_NOTICE ; ++i ) { 
        m_apkNotice[ i ]->ToggleRender( false );
    }

    m_iPlayTime  = iPlayTime_;
    m_iShowTimer = 0;
    m_iShowIndex = iIndex_;
    m_iShowAlpha = 255;
    m_bShow = true;

    m_pkTimeValue->ToggleRender( true );
	m_pkTimeValue->SetNumber(m_iPlayTime);
    
    this->ToggleRender( true );
}

void KGCPlaytimeNotice::NextNotice( void )
{
    m_apkNotice[ m_iShowIndex++ ]->ToggleRender( false );

    if( 0 > m_iShowIndex || NUM_NOTICE <= m_iShowIndex ) { 
        StopNotice();
    }
    else { 
        m_iShowTimer = 0;
        m_iShowAlpha = 255;
        m_pkTimeValue->ToggleRender( false );
    }
}

void KGCPlaytimeNotice::StopNotice( void )
{
    for( int i=0 ; i<NUM_NOTICE ; ++i ) { 
        m_apkNotice[ i ]->ToggleRender( false );
    }

    m_iPlayTime  = 0;
    m_iShowTimer = 0;
    m_iShowIndex = -1;
    m_iShowAlpha = 255;
    m_bShow = false;
    m_pkTimeValue->ToggleRender( false );
    this->ToggleRender( false );
}

void KGCPlaytimeNotice::PostChildDraw( void )
{

}
