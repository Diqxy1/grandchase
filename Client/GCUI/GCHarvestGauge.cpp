#include "stdafx.h"
#include "GCHarvestGauge.h"

IMPLEMENT_CLASSNAME( KGCHarvestGauge );
IMPLEMENT_WND_FACTORY( KGCHarvestGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCHarvestGauge, "gc_harvest_gauge" );

KGCHarvestGauge::KGCHarvestGauge( void )
{
    m_pkGauge = NULL;
    LINK_CONTROL( "gauge", m_pkGauge );

    m_iLevel = LEVEL0;
    m_iFrameCount = 0;
    m_iLengthCount = 0;
    m_fGaugeStep = 0.0f;
}

KGCHarvestGauge::~KGCHarvestGauge( void )
{
}

void KGCHarvestGauge::OnCreate( void )
{
    m_pkGauge->InitState( true, false, NULL );
    m_iLevel = LEVEL0;
    m_iFrameCount = 0;
    m_iLengthCount = 0;
    m_fGaugeStep = 0.0f;
}

void KGCHarvestGauge::ActionPerformed( const KActionEvent& event )
{

}

void KGCHarvestGauge::FrameMoveInEnabledState( void )
{
    float fWidth = static_cast<float>( GAUGE_MIN );
    fWidth += ( m_fGaugeStep * m_iLengthCount );
    DWORD dwWidth = static_cast<DWORD>( fWidth );

    if( dwWidth < GAUGE_MAX )
    {
        m_pkGauge->SetWidth( dwWidth );
        ++m_iLengthCount;
        g_KDSound.Play( "984033" );
    }
    else
    {
        m_iLengthCount = 0;
        m_pkGauge->SetWidth( GAUGE_MIN );
        KP2P::GetInstance()->Send_HarvestFlowerPotReq( SiKGCAgitGameManager()->GetNowPlantPotUID().first, false );
        this->ToggleRender( false );
        g_MyD3D->SetEnableKeyInput( true );
    }
}

void KGCHarvestGauge::Init( D3DXVECTOR2& dxvPos_ )
{
    // 초기화 해주고 
    m_iLevel = LEVEL0;
    m_iFrameCount = 0;
    m_iLengthCount = 0;
    m_fGaugeStep = 0.0f;
    m_pkGauge->SetWidth( GAUGE_MIN );

    // 위치 잡고 
    this->SetWindowPos( dxvPos_ );

    // 어떤 속도로 돌릴꺼냐??
    srand( time(NULL ) );
    int iRate = rand() % 100;
    m_iLevel = GetSpeedLevel( iRate );

    int iGaugeLenth = GAUGE_MAX - GAUGE_MIN;
    m_fGaugeStep = static_cast<float>( iGaugeLenth / ( 55.0f * m_iLevel ) );  // 한번에 얼마만큼??

    g_MyD3D->SetEnableKeyInput( false );
}

bool KGCHarvestGauge::Stop( void )
{
    this->ToggleRender( false );
    g_MyD3D->SetEnableKeyInput( true );

    DWORD dwWidth = m_pkGauge->GetWidth();
    if( GAUGE_HIT_START <= dwWidth && dwWidth <= GAUGE_HIT_END )
    {
        //float fX = 0.0f;
        //float fY = 0.0f;
        //KLuaManager luaMgr;
        //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
        //LUA_GET_VALUE_DEF( "fX", fX, 0.0f );
        //LUA_GET_VALUE_DEF( "fY", fY, 0.0f );

        //D3DXVECTOR2 dxvPos = this->GetCurrentWindowPos();
        //float fWorldX = ( dxvPos.x - 0.5f ) / 400.0f - 1.0f;
        //float fWroldY = ( 0.5f - dxvPos.y ) / 400.0f + 0.75f;

        //fWorldX += fX;
        //fWroldY += fY;

        //g_ParticleManager->CreateSequence( "House_StarGet_06", fWorldX, fWroldY, 0.5f );
        //g_ParticleManager->CreateSequence( "House_StarGet_08", fWorldX, fWroldY, 0.5f );
        return true;
    }

    return false;
}

int KGCHarvestGauge::GetSpeedLevel( int iRate_ )
{
    if( 0 <= iRate_ && iRate_ < 40 )            // 40%
        return LEVEL0;
    else if( 40 <= iRate_ && iRate_ < 70 )      // 30%
        return LEVEL1;
    else if( 70 <= iRate_ && iRate_ < 90 )      // 20%
        return LEVEL2;
    else                                        // 10%
        return LEVEL3;
}

void KGCHarvestGauge::SetHarvestParticle( int iNumStar, bool bBonus )
{
    if( iNumStar < 1 )
        return;

    //if( 1 == iNumStar )

}
