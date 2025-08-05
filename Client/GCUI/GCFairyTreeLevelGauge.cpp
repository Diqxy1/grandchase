#include "stdafx.h" 
#include "GCFairyTreeLevelGauge.h"

IMPLEMENT_CLASSNAME( KGCFairyTreeLevelGauge );
IMPLEMENT_WND_FACTORY( KGCFairyTreeLevelGauge );
IMPLEMENT_WND_FACTORY_NAME( KGCFairyTreeLevelGauge, "gc_fairy_tree_gauge" );

const float fGAUGE_MIN = 4 * GC_SCREEN_DIV_WIDTH;
const float fGAUGE_MAX = 112 * GC_SCREEN_DIV_WIDTH;

KGCFairyTreeLevelGauge::KGCFairyTreeLevelGauge( void )
: m_pkFrame( NULL )
, m_pkGauge( NULL )
, m_pkStaticLevel( NULL )
, m_iLevel( 0 )
, m_dwCurExp( 0 )
, m_dwNextLevelExp( 0 )
, m_dxvPos( 0.0f, 0.0f )
, m_dxvOffset( 0.0f, 0.0f )
, m_dxvRenderPos( 0.0f, 0.0f )
{
    LINK_CONTROL( "frame", m_pkFrame );
    LINK_CONTROL( "gauge", m_pkGauge );
    LINK_CONTROL( "staticLevel", m_pkStaticLevel );
}

KGCFairyTreeLevelGauge::~KGCFairyTreeLevelGauge( void )
{

}

void KGCFairyTreeLevelGauge::OnCreate( void )
{
    m_pkFrame->InitState( true );
    m_pkGauge->InitState( true );
    m_pkStaticLevel->InitState( true );
    m_pkStaticLevel->SetAlign( DT_CENTER );
    m_pkStaticLevel->SetFontOutline( true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
    m_pkStaticLevel->SetFontSpace( 0.0f );

    m_iLevel = 0;
    m_dwCurExp = 0;
    m_dwNextLevelExp = 0;
    
    m_dxvPos = D3DXVECTOR2( 0.0f, 0.0f );
    m_dxvOffset = D3DXVECTOR2( 0.0f, 0.0f );
}

void KGCFairyTreeLevelGauge::ActionPerformed( const KActionEvent& event )
{

}

void KGCFairyTreeLevelGauge::SetFairyTreeLevelGauge( int iLevel_, DWORD dwCurExp_, DWORD dwNextLevelExp_, D3DXVECTOR2& dxvPos_, D3DXVECTOR2& dxvOffset_ )
{
    m_iLevel = iLevel_;
    m_dwCurExp = dwCurExp_;
    m_dwNextLevelExp = dwNextLevelExp_;
    m_dxvPos = dxvPos_;
    m_dxvOffset = dxvOffset_;

    SetLevelStatic();
    SetGauge();
}

void KGCFairyTreeLevelGauge::UpdateFairyTreeLevelGaugePos( void )
{
    D3DXMATRIX matBasic, mat1, mat2;
    D3DXMatrixIdentity( &matBasic );
    D3DXMatrixIdentity( &mat1 );
    D3DXMatrixIdentity( &mat2 );
    D3DXMatrixTranslation( &mat1, m_dxvPos.x - 1.0f , m_dxvPos.y - 0.48f, 1.f );
    D3DXMatrixMultiply( &mat2, &matBasic, &mat1 );
    D3DXMatrixMultiply( &matBasic, &mat2, &g_kCamera.m_matCamera );

    D3DXVECTOR3 dxvTransPos;
    D3DXVec3TransformCoord( &dxvTransPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &matBasic );
    
    dxvTransPos.x += m_dxvOffset.x;
    dxvTransPos.y += m_dxvOffset.y;

    //float fScreenX = ( dxvTransPos.x + 1.0f ) * 400.0f * GC_SCREEN_DIV_WIDTH + 0.5f;
    //float fScreenY = ( -dxvTransPos.y + 0.75f ) * 400.0f * GC_SCREEN_DIV_WIDTH + 0.5f;

    float fScreenX = ( dxvTransPos.x + 1.0f ) * (400.f * GC_SCREEN_DIV_WIDTH )  + 0.5f * GC_SCREEN_DIV_WIDTH;
    float fScreenY = ( -dxvTransPos.y + 0.75f ) * ( 400.f * GC_SCREEN_DIV_WIDTH) + 0.5f  * GC_SCREEN_DIV_WIDTH;


    m_dxvRenderPos = D3DXVECTOR2( fScreenX, fScreenY );
    this->SetFixedWindowLocalPos( m_dxvRenderPos );
}

void KGCFairyTreeLevelGauge::SetLevelStatic( void )
{
    if( m_iLevel <= 0 ) { 
        m_pkStaticLevel->SetText( L"" );
    }
    else {
        m_pkStaticLevel->SetText( g_pkStrLoader->GetReplacedString( STR_ID_FAIRY_TREE_LEVEL, "i", m_iLevel ) );
    }
}

void KGCFairyTreeLevelGauge::SetGauge( void )
{
    if( 7 == m_iLevel ) { 
        m_pkFrame->ToggleRender( false );
        m_pkGauge->ToggleRender( false );
    }
    else {
        float   fGaugeLength = fGAUGE_MAX - fGAUGE_MIN;
        float   fDivLength = fGaugeLength / static_cast<float>( m_dwNextLevelExp );
        DWORD   dwResultLength = static_cast<DWORD>( fDivLength * m_dwCurExp );

        if( dwResultLength < fGAUGE_MIN ) { 
            dwResultLength = static_cast<DWORD>(fGAUGE_MIN);
        }

        if( dwResultLength > fGAUGE_MAX ) { 
            dwResultLength = static_cast<DWORD>(fGAUGE_MAX);
        }

        m_pkGauge->SetWidth( dwResultLength );

        m_pkFrame->ToggleRender( true );
        m_pkGauge->ToggleRender( true );
    }
}