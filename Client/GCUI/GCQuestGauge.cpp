#include "stdafx.h"
#include "GCQuestGauge.h"
//

//

IMPLEMENT_CLASSNAME( KGCQuestGauge );
IMPLEMENT_WND_FACTORY( KGCQuestGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCQuestGauge, "gc_quest_gauge" );

KGCQuestGauge::KGCQuestGauge( void )
{
    m_pkGuageBar    = NULL;
    m_pkType        = NULL;
    m_dwCur         = 0;
    m_dwMax         = 0;

    LINK_CONTROL( "quest_gauge_bar",    m_pkGuageBar );
    LINK_CONTROL( "quest_gauge_type",   m_pkType );
}

KGCQuestGauge::~KGCQuestGauge( void )
{
    // empty
}

void KGCQuestGauge::OnCreate( void )
{
    SetSelfInputCheck( true );

    m_pkGuageBar->SetSelfInputCheck( false );
    m_pkType->SetSelfInputCheck( false );

    SetChar( 0 );
}

void KGCQuestGauge::FrameMoveInEnabledState( void )
{
    float fGauge = static_cast<float>( m_dwCur )
                 / static_cast<float>( m_dwMax )
                 * ( GetHeight() - m_pkType->GetHeight() );

    m_pkGuageBar->SetHeight( (DWORD)fGauge );
    m_pkGuageBar->SetFixedWindowLocalPos(
        D3DXVECTOR2( m_pkGuageBar->GetFixedWindowLocalPos().x,
                     m_pkType->GetFixedWindowLocalPos().y - fGauge ) );
}

void KGCQuestGauge::PostDraw( void )
{
    /*g_pkUIMgr->RenderNumber( "small_num",
                             GetFixedWindowPos() + D3DXVECTOR2( 0.0f, 1.0f ),
                             m_dwCur,
                             false );*/
}

void KGCQuestGauge::SetChar( int iChar )
{
    switch ( iChar )
    {
        case 0: // 엘리시스
        {
            m_pkType->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
            break;
        }
        case 1: // 리르
        {
            m_pkType->SetWndMode( KD3DWnd::D3DWM_HOVER );
            break;
        }
        case 2: // 아르메
        {
            m_pkType->SetWndMode( KD3DWnd::D3DWM_SELECT );
            break;
        }
    }
}

void KGCQuestGauge::SetCurrentVal( DWORD dwCur )
{
    if ( ( m_dwCur = dwCur ) > m_dwMax ) m_dwCur = m_dwMax;
}

void KGCQuestGauge::SetMaximumVal( DWORD dwMax )
{
    if ( ( m_dwMax = dwMax ) < m_dwCur ) m_dwCur = m_dwMax;
}