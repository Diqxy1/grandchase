#include "stdafx.h"
#include "GCOlympicRankSlot.h"

IMPLEMENT_CLASSNAME( KGCOlympicRankSlot );
IMPLEMENT_WND_FACTORY( KGCOlympicRankSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicRankSlot, "gc_olympic_rank_slot" );

KGCOlympicRankSlot::KGCOlympicRankSlot( void )
: m_iNation( -1 )
, m_iRank( -1 )
, m_iScore( -1 )
{
    for( int i=0 ; i<NUM_NATION ; ++i ) {
        char szTemp[ 128 ];

        // 순위 표시 
        m_apkRank[ i ] = NULL;                          
        sprintf_s( szTemp, 127, "wnd_rank%d", i );
        LINK_CONTROL( szTemp, m_apkRank[ i ] );

        // 국기 표시 
        m_apkFlag[ i ] = NULL;
        sprintf_s( szTemp, 127, "flag%d", i );
        LINK_CONTROL( szTemp, m_apkFlag[ i ] );

        // 국가 이름 표시 
        m_apkNatoion[ i ] = NULL;
        sprintf_s( szTemp, 127, "wnd_nation%d", i );
        LINK_CONTROL( szTemp, m_apkNatoion[ i ] );
    }

    m_pkScore = NULL;
    LINK_CONTROL( "score", m_pkScore );
}

KGCOlympicRankSlot::~KGCOlympicRankSlot( void )
{
}

void KGCOlympicRankSlot::OnCreate( void )
{
    for( int i=0 ; i<NUM_NATION ; ++i ) {
        m_apkRank[ i ]->InitState( false );
        m_apkFlag[ i ]->InitState( false );
        m_apkNatoion[ i ]->InitState( false );
    }

    m_pkScore->InitState( false );
    m_pkScore->SetStringType( "orange_gradation_num2", false );
    m_pkScore->SetWriteToRight( false );

    m_iNation = -1;
    m_iRank = -1;
    m_iScore = -1;
}

void KGCOlympicRankSlot::ActionPerformed( const KActionEvent& event ) {}

void KGCOlympicRankSlot::FrameMoveInEnabledState( void ) {}

void KGCOlympicRankSlot::RenderRankSlot( const bool bRender_ )
{
    for( int i=0 ; i<NUM_NATION ; ++i ) { 
        m_apkRank[ i ]->ToggleRender( bRender_ );
        m_apkFlag[ i ]->ToggleRender( bRender_ );
        m_apkNatoion[ i ]->ToggleRender( bRender_ );
    }

    m_pkScore->ToggleRender( bRender_ );
}

void KGCOlympicRankSlot::ClearRankSlot( void )
{
    m_iNation = -1;
    m_iRank = -1;
    m_iScore = -1;

    RenderRankSlot( false );
}

void KGCOlympicRankSlot::SetRankSlot( const int iNation_, const int iRank_, const int iScore_ )
{
    ClearRankSlot();

    if( iNation_ <= 0 || iNation_ > NUM_NATION ) { 
        return;
    }

    if( iRank_ <= 0 || iRank_ > NUM_NATION ) { 
        return;
    }

    m_iNation = iNation_ - 1;
    m_iRank = iRank_ - 1;
    m_iScore = iScore_;

    m_apkRank[ m_iRank]->ToggleRender( true );
    m_apkFlag[ m_iNation ]->ToggleRender( true );
    m_apkNatoion[ m_iNation ]->ToggleRender( true );

    m_pkScore->SetNumber( m_iScore );
    m_pkScore->ToggleRender( true );
}