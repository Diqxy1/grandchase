#include "stdafx.h"
#include "GCPlayerO2Bar.h"
//


IMPLEMENT_CLASSNAME( KGCPlayerO2Bar );
IMPLEMENT_WND_FACTORY( KGCPlayerO2Bar );
IMPLEMENT_WND_FACTORY_NAME( KGCPlayerO2Bar, "gc_player_o2_bar" );

KGCPlayerO2Bar::KGCPlayerO2Bar( void )
{
    m_fNowO2        = 0.0f;
    m_fMaxO2        = 0.0f;

    m_bEnable       = false;
    m_fTimeParam    = 0.0f;

	for(int i = 0; i < 10; i++)
	{
		m_pkGaugeBar[i] = NULL;
	}

    LINK_CONTROL( "playero2_bar1",     m_pkGaugeBar[0] );
    LINK_CONTROL( "playero2_bar2",     m_pkGaugeBar[1] );
    LINK_CONTROL( "playero2_bar3",     m_pkGaugeBar[2] );
	LINK_CONTROL( "playero2_bar4",     m_pkGaugeBar[3] );
    LINK_CONTROL( "playero2_bar5",     m_pkGaugeBar[4] );
    LINK_CONTROL( "playero2_bar6",     m_pkGaugeBar[5] );
	LINK_CONTROL( "playero2_bar7",     m_pkGaugeBar[6] );
    LINK_CONTROL( "playero2_bar8",     m_pkGaugeBar[7] );
    LINK_CONTROL( "playero2_bar9",     m_pkGaugeBar[8] );
	LINK_CONTROL( "playero2_bar10",     m_pkGaugeBar[9]);

	m_iBlinkGauge = 0;
	m_iBlinkGap = 0;
}

KGCPlayerO2Bar::~KGCPlayerO2Bar( void )
{
    // empty
}

void KGCPlayerO2Bar::OnCreate( void )
{
	for(int i = 0; i < 10; i++)
	{
		m_pkGaugeBar[i]->SetSelfInputCheck( false );
	}
}

void KGCPlayerO2Bar::FrameMoveInEnabledState( void )
{
	/*
    m_fTimeParam += GC_ELAPSED_TIME;
    if ( m_fTimeParam > 1.0f ) m_fTimeParam = 1.0f;
    D3DXVECTOR2 vCurOffset;
    if ( m_bEnable )
    {
        ::D3DXVec2Lerp( &vCurOffset, &D3DXVECTOR2( 0.0f, 0.0f ),
                                     &D3DXVECTOR2( 0.0f, -40.0f ), m_fTimeParam );
    }
    else
    {
        ::D3DXVec2Lerp( &vCurOffset, &D3DXVECTOR2( 0.0f, -40.0f ),
                                     &D3DXVECTOR2( 0.0f, 0.0f ), m_fTimeParam );
    }
    vCurOffset.x = static_cast<int>( vCurOffset.x );
    vCurOffset.y = static_cast<int>( vCurOffset.y );
    SetCurrentWindowLocalPos( GetFixedWindowLocalPos() + vCurOffset );
	*/
}

void KGCPlayerO2Bar::EnableO2Bar( bool bEnable )
{
    m_bEnable = bEnable;
    m_fTimeParam = 0.0f;
	
	return;
}

void KGCPlayerO2Bar::SetNowO2( float fNowO2 )
{
    if ( ( m_fNowO2 = fNowO2 ) > m_fMaxO2 ) m_fNowO2 = m_fMaxO2;
    ModulateGauge();
}

void KGCPlayerO2Bar::SetMaxO2( float fMaxO2 )
{
    if ( ( m_fMaxO2 = fMaxO2 ) < m_fNowO2 ) m_fNowO2 = m_fMaxO2;
    ModulateGauge();
}

void KGCPlayerO2Bar::ModulateGauge( void )
{
	int Percent = (int)(m_fNowO2 / m_fMaxO2 * 100.0f);

	m_iBlinkGauge = Percent / 10;
	int Remain = Percent % 10;

	for(int i = 0; i < 10; i++)
	{
		if( Percent == 0 )
		{
			m_pkGaugeBar[i]->ToggleRender(false);
		}
		else
		{		
			if( i > m_iBlinkGauge )		
				m_pkGaugeBar[i]->ToggleRender(false);
			else if( i == m_iBlinkGauge )
			{
				if( Remain < 5)
					m_pkGaugeBar[i]->ToggleRender( m_iBlinkGap++ % 30 > 9 );
				else m_pkGaugeBar[i]->ToggleRender(true);
			}
			else m_pkGaugeBar[i]->ToggleRender(true);
		}
	}
	
    //m_pkGaugeBar1->SetWidth( m_fMaxO2 == 0.0f ? 0 : static_cast<int>( MAX_GAUGE_LENGTH * ( m_fNowO2 / m_fMaxO2 ) ) );
}