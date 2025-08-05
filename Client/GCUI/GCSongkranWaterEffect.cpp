#include "stdafx.h"
#include "GCSongkranWaterEffect.h"

IMPLEMENT_CLASSNAME( KGCSongkranWaterEffect );
IMPLEMENT_WND_FACTORY( KGCSongkranWaterEffect );
IMPLEMENT_WND_FACTORY_NAME( KGCSongkranWaterEffect, "gc_songkranwatereffect" );

const DWORD fOFF_SET = 51;

KGCSongkranWaterEffect::KGCSongkranWaterEffect( void )
: m_pTexWater ( NULL )
, m_pWaterBack( NULL )
, m_pWaterFront( NULL )
{
	ZeroMemory( m_fBackVertex, sizeof(float) * 2 );
	ZeroMemory( m_fFrontVertex, sizeof(float) * 2 );
}

KGCSongkranWaterEffect::~KGCSongkranWaterEffect( void )
{
	SAFE_RELEASE( m_pTexWater );
	SAFE_RELEASE( m_pWaterBack );
	SAFE_RELEASE( m_pWaterFront );
}


void KGCSongkranWaterEffect::ActionPerformed( const KActionEvent& event )
{
}

void KGCSongkranWaterEffect::OnCreate( void )
{
	SAFE_RELEASE( m_pTexWater );
	SAFE_RELEASE( m_pWaterBack );
	SAFE_RELEASE( m_pWaterFront );

	m_pTexWater = g_pGCDeviceManager2->CreateTexture( "WATER.DDS" );
	m_pWaterBack = g_pGCDeviceManager2->CreateTexture( "WaveSurface2.dds" );
	m_pWaterFront = g_pGCDeviceManager2->CreateTexture( "WaveSurface1.dds" );

	m_fBackVertex[0] = 0.0f;
	m_fFrontVertex[0] = 0.0f;

	m_fBackVertex[1] = 3.0f;
	m_fFrontVertex[1] = 3.0f;

    ADD_WORLDMAP_PARTICLE( "UI_water_01" );
    ADD_WORLDMAP_PARTICLE( "UI_water_02" );
    RenderMapEffect( false );
}

void KGCSongkranWaterEffect::FrameMoveInEnabledState()
{
	for( int i = 0; i < 2; i++ )
	{
		m_fBackVertex[i] -= GC_ELAPSED_TIME / 3.0f;
		m_fFrontVertex[i] += GC_ELAPSED_TIME / 3.0f;
	}
}


void KGCSongkranWaterEffect::PostDraw( void )
{	
	D3DXVECTOR2 vPos( GetRelocatedWindowPos() );
	
	DWORD dwWidth = GetWidth();
	DWORD dwHeight = GetHeight();

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	{
		g_pGCDeviceManager2->DrawInScreen( m_pTexWater,
			vPos.x , vPos.y + ( 10 * m_fWindowScaleY ), vPos.x + dwWidth * m_fWindowScaleX, vPos.y + ( dwHeight * m_fWindowScaleY ),
			0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 0xffffffff);

	}
	g_pGCDeviceManager2->PopState();

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	{
		g_pGCDeviceManager2->DrawInScreen( m_pWaterBack,
 			vPos.x , vPos.y, vPos.x + dwWidth * m_fWindowScaleX, vPos.y + ( fOFF_SET * m_fWindowScaleY ),
 			m_fBackVertex[0], 0.45f, m_fBackVertex[1], 0.55f, false, 10.0f, 0xffffffff);

		g_pGCDeviceManager2->DrawInScreen( m_pWaterFront,
			vPos.x , vPos.y, vPos.x + dwWidth * m_fWindowScaleX, vPos.y + ( fOFF_SET * m_fWindowScaleY ),
			m_fFrontVertex[0], 0.45f, m_fFrontVertex[1], 0.55f, false, 10.0f, 0xffffffff);

	}
	g_pGCDeviceManager2->PopState();
}

void KGCSongkranWaterEffect::OnDestroy()
{
    ClearMapEffect();
}

void KGCSongkranWaterEffect::ClearMapEffect()
{
    for( int i = 0; i < (int)m_vecMapEffect.size(); ++i )
    {
        g_ParticleManager->DeleteSequence( m_vecMapEffect[i] );
    }
    m_vecMapEffect.clear();
}

void KGCSongkranWaterEffect::ADD_WORLDMAP_PARTICLE( std::string seq,float x/*=0.0f*/,float y/*=0.0f*/ )
{
    CParticleEventSeqPTR pParticle = NULL;
    pParticle = g_ParticleManager->CreateSequence( seq, x, y, 0.5f );
    if( pParticle )
    {
        g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
        m_vecMapEffect.push_back( pParticle );
    }
}

void KGCSongkranWaterEffect::RenderMapEffect( bool bRender )
{
    for( int i = 0; i < (int)m_vecMapEffect.size(); ++i )
    {
        if( NULL != m_vecMapEffect[i] ) { 
            m_vecMapEffect[i]->SetShow( bRender ) ;
        }
    }
}
