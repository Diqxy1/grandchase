#include "stdafx.h"
#include "KGCHPGauge.h"
#include "GCClientErrorManager.h"

//
//

IMPLEMENT_CLASSNAME( KGCHPGauge );
IMPLEMENT_WND_FACTORY( KGCHPGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCHPGauge, "gc_hp_gauge" );

KGCHPGauge::KGCHPGauge( void )
{
	char strTemp[MAX_PATH];
	for( int i=0 ; i < GAUGE_BAR_NUM ; ++i )
	{
		m_pkGaugeBar[i] = NULL;

		sprintf( strTemp, "gauag_bar%d", i+1 );
		LINK_CONTROL( strTemp,		m_pkGaugeBar[i] );
	}

	m_iBarNum = 0;
	m_dwGaugeHP = 1;
    m_fOldHp = 0.0f;
    m_iOldBarNum = 0;
    m_pkHpEffect = NULL;
    m_dwTime = 0;
	m_fMaxHP = 1.0f;
    LINK_CONTROL( "hpeffect", m_pkHpEffect );
}

KGCHPGauge::~KGCHPGauge( void )
{
	//empty
}

void KGCHPGauge::OnCreate( void )
{
	for( int i=0; i < GAUGE_BAR_NUM; ++i)
		m_pkGaugeBar[i]->SetSelfInputCheck( false );

    m_pkHpEffect->InitState( false, false, NULL );
}

void KGCHPGauge::SetMonID(int iMonID)
{
    m_iMonID = iMonID;
}

void KGCHPGauge::SetNowHP( float fNowHP )
{
    if ( m_fOldHp == m_fNowHP )
        m_fOldHp = m_fNowHP;

	if(m_dwGaugeHP <= 0)
		return;

    m_fNowHP = fNowHP;
    m_iOldBarNum = m_iBarNum;
    m_iBarNum = static_cast<int>( m_fNowHP/m_dwGaugeHP );


    ModulateGauge();

    HpEffect();
}

void KGCHPGauge::SetMaxHP( float fMaxHP )
{
	if ( ( m_fMaxHP = fMaxHP ) < m_fNowHP ) m_fNowHP = m_fMaxHP;

    m_fOldHp = m_fMaxHP;

	m_dwGaugeHP = static_cast<DWORD>( m_fMaxHP/( (int)(m_fMaxHP/MAX_GAUGE_HP) + 1 ) );

  
	ModulateGauge();
}

void KGCHPGauge::ModulateGauge( void )
{
	// 
	for( int i=0; i < GAUGE_BAR_NUM; ++i)
		m_pkGaugeBar[i]->SetWidth(0);

	if ( m_fMaxHP <= MAX_GAUGE_HP || m_fMaxHP == 0.0f )
	{
		m_pkGaugeBar[0]->SetWidth( m_fMaxHP == 0.0f ? 0 : static_cast<int>( MAX_GAUGE_LENGTH * ( m_fNowHP / m_fMaxHP ) ) );
	}
	else
	{
		for( int i=0; i <= m_iBarNum; ++i)
		{
			DWORD dwWidth = 0;

			if(m_dwGaugeHP <= 0)
				return;

			if( i < m_iBarNum) 
				dwWidth = MAX_GAUGE_LENGTH;
			else if( i == m_iBarNum )
				dwWidth = static_cast<int>( MAX_GAUGE_LENGTH * ( ( m_fNowHP - ( m_iBarNum * m_dwGaugeHP ) ) / m_dwGaugeHP ) );

			m_pkGaugeBar[i % GAUGE_BAR_NUM]->SetWidth( dwWidth );
			MakeTopChild( m_pkGaugeBar[i % GAUGE_BAR_NUM] );
		}
	}   

    MakeTopChild( m_pkHpEffect );

}

void KGCHPGauge::MakeTopChild( KD3DWnd* pWnd )
{
	std::vector<KD3DWnd*>::iterator vit = std::find( m_vecChild.begin(), m_vecChild.end(), pWnd );

	if ( vit == m_vecChild.end() )
		return;

	m_vecChild.erase( vit );
	m_vecChild.push_back( pWnd );
}

void KGCHPGauge::HpEffect( void )
{
    // 
    if ( m_fNowHP < m_fOldHp )
    {
        size_t validIndex = m_iBarNum % GAUGE_BAR_NUM;
        if( validIndex >= GAUGE_BAR_NUM ){  //  여기로 들어오면 그냥 종료해야 한다.
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_NOT_FIND_CONNECTQ_USER );
            exit(0);
        }

        D3DXVECTOR2 tmp = m_pkGaugeBar[validIndex]->GetFixedWindowLocalPos();
        tmp.x += m_pkGaugeBar[validIndex]->GetWidth();
        m_pkHpEffect->SetWindowPosDirect( tmp );

		if(m_dwGaugeHP <= 0)
			return;

        if ( m_iOldBarNum != m_iBarNum )
        {
            m_pkHpEffect->SetWidth( MAX_GAUGE_LENGTH - m_pkGaugeBar[validIndex]->GetWidth() );
            m_fOldHp = static_cast<float>( (m_iBarNum + 1) * m_dwGaugeHP );
        }
        else
            m_pkHpEffect->SetWidthDirect( static_cast<int>( MAX_GAUGE_LENGTH * ( ( ( m_fOldHp - m_fNowHP) ) / m_dwGaugeHP ) ) );

        m_pkHpEffect->TransformWindow( m_pkHpEffect->GetFixedWindowLocalPos(), m_pkHpEffect->GetFixedWindowLocalPos() , 0.5f, true );
        m_pkHpEffect->ToggleRender( true );
    }
    else if ( m_fNowHP > m_fOldHp )
    {
        m_fOldHp = m_fNowHP;
    }
    else
    {
        m_fOldHp = m_fNowHP;
        m_pkHpEffect->SetWidthDirect( 0 );
        m_pkHpEffect->ToggleRender( false );
    }
}

void KGCHPGauge::FrameMoveInEnabledState( void )
{
    if ( m_dwTime + EFFECT_TIME_GAP < ::timeGetTime() && m_pkHpEffect->GetWidth() > 0 )
    {
        DWORD dwAdjustWidth = m_pkHpEffect->GetWidth() - DECREASE_VALUE;
        if ( dwAdjustWidth <= 1 )
        {
            m_fOldHp = m_fNowHP;
            m_pkHpEffect->SetWidthDirect( 0 );
            m_dwTime = ::timeGetTime();
        }
        else
        {
            m_fOldHp = ( dwAdjustWidth * m_dwGaugeHP / MAX_GAUGE_LENGTH ) + m_fNowHP;
        }
    }
}

void KGCHPGauge::PostChildDraw( void )
{
    if ( this->IsRenderOn() )
    {
        g_pGCDeviceManager2->PushState();
        {
            g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

            D3DXVECTOR2 vecPos = this->GetRelocatedWindowPos();
            vecPos.x += (this->GetParent()->GetWidth() - 57.0f) * m_fWindowScaleX; //10000이 넘어가면, 박스를 넘어서 조정
            vecPos.y += (9.0f/GC_SCREEN_DIV_HEIGHT / m_fWindowScaleY);

            float fScreenX = vecPos.x;
            float fScreenY = vecPos.y;

            std::wstringstream stm;
			if( m_iMonID == MON_HENIR_OBEZAAR ) //HP 물음표 나중에 던전타입 별로가 아닌, 몬스터별로 나눠주는 작업필요
			{
				stm.str(L"x???");
			}
			else
			{
				stm.str(L"");
				stm << L"x" << m_iBarNum;
			}

            D3DCOLOR Color = 0xFFFFFFFF;

            g_pkFontMgrOutline14->OutTextXY( (int)fScreenX, (int)fScreenY, stm.str().c_str(), Color, NULL, DT_RIGHT | DT_TOP );
        }
        g_pGCDeviceManager2->PopState();
    }
}
