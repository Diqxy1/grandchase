#include "stdafx.h"
#include ".\gcongameexpgauge.h"
//


#include "GCGameResult.h"



IMPLEMENT_CLASSNAME( GCOnGameExpGauge );
IMPLEMENT_WND_FACTORY( GCOnGameExpGauge );
IMPLEMENT_WND_FACTORY_NAME( GCOnGameExpGauge, "gc_on_game_exp_gauge" );

GCOnGameExpGauge::GCOnGameExpGauge(void)
{
    m_fGauge            = 0.0f;
    m_fCurExp           = 0.0f;
    m_fVirtualGauge     = 0.0f;
    m_fVirtualExp        = 0.0f;
    m_dwMaxGaugePixel   = 0;

    for( int i = 0; i < E_NUM; ++ i )
    {
        m_pkGauge[i]        = NULL;
        m_pkVirtualGauge[i] = NULL;
    }

    LINK_CONTROL( "exp_gauge_left",             m_pkGauge[E_LEFT] );
    LINK_CONTROL( "exp_gauge_body",             m_pkGauge[E_BODY] );
    LINK_CONTROL( "exp_gauge_right",            m_pkGauge[E_RIGHT] );
    LINK_CONTROL( "virtual_exp_gauge_left",     m_pkVirtualGauge[E_LEFT] );
    LINK_CONTROL( "virtual_exp_gauge_body",     m_pkVirtualGauge[E_BODY] );
    LINK_CONTROL( "virtual_exp_gauge_right",    m_pkVirtualGauge[E_RIGHT] );
    LINK_CONTROL( "exp_level_up",               m_pkLevelUp );
}

GCOnGameExpGauge::~GCOnGameExpGauge(void)
{
}

void GCOnGameExpGauge::OnCreate( void )
{

}

void GCOnGameExpGauge::OnDestroy( void )
{

}

void GCOnGameExpGauge::InitGauge()
{
    static bool s_bIsFirst = true;
    if ( s_bIsFirst == true )
    {
        m_dwMaxGaugePixel = m_pkGauge[E_BODY]->GetWidth();
        s_bIsFirst = false;
    }
    m_fGauge            = 0.0f;
    m_fCurExp           = 0.0f;
    m_fVirtualGauge     = 0.0f;
    m_fVirtualExp        = 0.0f;

    m_bLevelUp = false;
    m_pkLevelUp->ToggleRender( false );
}

DWORD GCOnGameExpGauge::GetBodyWidth( float fGauge_ )
{
    return (DWORD)((float)m_dwMaxGaugePixel*fGauge_*0.01f);
}

void GCOnGameExpGauge::AddExp( float fExp_ )
{
    float fAddedExp     = m_fCurExp + fExp_;
    UCHAR ucCurLevel    = Exp_2_Level( (GCEXPTYPE)m_fCurExp );
    UCHAR ucAddedLevel  = Exp_2_Level( (GCEXPTYPE)fAddedExp );
    // Level up
    if ( ucCurLevel != ucAddedLevel )
    {
        m_bLevelUp = true;
        m_pkLevelUp->ToggleRender( true );
    }
    m_fCurExp = fAddedExp;
    SetExp( m_fCurExp );
}

void GCOnGameExpGauge::SetExp( float fExp_ )
{
    m_fCurExp = fExp_;
    m_fGauge = GetCurrentExpPercent( (GCEXPTYPE)fExp_ );
    // 다음 레벨까지의 경험치가 0%이하.
    if ( m_fGauge <= 0.0f )
    {
        for ( int i = 0; i < (int)E_NUM; i++ )
        {
            m_pkGauge[i]->ToggleRender( false );
        }
    }
    else
    {
        for ( int i = 0; i < (int)E_NUM; i++ )
        {
            m_pkGauge[i]->ToggleRender( true );
        }
    }
    DWORD dwWidth = GetBodyWidth( m_fGauge );
    m_pkGauge[E_BODY]->SetWidth( GetBodyWidth( m_fGauge ) );
    D3DXVECTOR2 vPos = m_pkGauge[E_BODY]->GetFixedWindowLocalPos();
    vPos.x += (float)dwWidth;
    m_pkGauge[E_RIGHT]->SetFixedWindowLocalPos( vPos );
}

void GCOnGameExpGauge::AddVirtualExp( float fExp_ )
{
    float fAddedExp     = m_fVirtualExp + fExp_;
    UCHAR ucCurLevel    = Exp_2_Level( (GCEXPTYPE)m_fVirtualExp );
    UCHAR ucAddedLevel  = Exp_2_Level( (GCEXPTYPE)fAddedExp );
    // Level up
    if( ucCurLevel != ucAddedLevel )
    {
        SetExp( (float)Level_2_Exp( ucAddedLevel ) );
    }
    m_fVirtualExp = fAddedExp;
    SetVirtualExp( m_fVirtualExp );
}

void GCOnGameExpGauge::SetVirtualExp( float fExp_ )
{
    m_fVirtualExp = fExp_;
    m_fVirtualGauge = GetCurrentExpPercent( (GCEXPTYPE)fExp_ );
    // 다음 레벨까지의 경험치가 0%이하.
    if ( m_fVirtualGauge <= 0.0f )
    {
        for ( int i = 0; i < (int)E_NUM; i++ )
        {
            m_pkVirtualGauge[i]->ToggleRender( false );
        }
    }
    else
    {
        for ( int i = 0; i < (int)E_NUM; i++ )
        {
            m_pkVirtualGauge[i]->ToggleRender( true );
        }
    }
    DWORD dwWidth = GetBodyWidth( m_fVirtualGauge );
    m_pkVirtualGauge[E_BODY]->SetWidth( GetBodyWidth( m_fVirtualGauge ) );
    D3DXVECTOR2 vPos = m_pkVirtualGauge[E_BODY]->GetFixedWindowLocalPos();
    vPos.x += (float)dwWidth;
    m_pkVirtualGauge[E_RIGHT]->SetFixedWindowLocalPos( vPos );
}
