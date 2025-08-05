#include "stdafx.h"
#include "KGCMissionSubInfoBox_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionSubInfoBox_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionSubInfoBox_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionSubInfoBox_S4, "gc_mission_subinfobox_s4" );

KGCMissionSubInfoBox_S4::KGCMissionSubInfoBox_S4( void )
: m_pkStatic_Title( NULL )
, m_pkStatic_Desc( NULL )
, m_pkStatic_Exp( NULL )
, m_pkStatic_Gp( NULL )
, m_pkStatic_Complate_State( NULL )
{
    LINK_CONTROL( "static_title", m_pkStatic_Title );
    LINK_CONTROL( "static_desc", m_pkStatic_Desc );
    LINK_CONTROL( "static_exp", m_pkStatic_Exp );
    LINK_CONTROL( "static_gp", m_pkStatic_Gp );
    LINK_CONTROL( "static_complate_state", m_pkStatic_Complate_State );
}

KGCMissionSubInfoBox_S4::~KGCMissionSubInfoBox_S4( void ){}

void KGCMissionSubInfoBox_S4::OnCreate( void )
{
    if( m_pkStatic_Title )
    {
        m_pkStatic_Title->InitState( true, false );
        m_pkStatic_Title->SetAlign( DT_LEFT );
        m_pkStatic_Title->SetFontColor( D3DCOLOR_ARGB(255, 255, 234, 176) );
        m_pkStatic_Title->SetFontOutline( true, D3DCOLOR_ARGB(255, 27, 21, 7) );
        m_pkStatic_Title->SetFontSpace( 1 );
    }
    if( m_pkStatic_Complate_State )
    {
        m_pkStatic_Complate_State->InitState( true, false );
        m_pkStatic_Complate_State->SetAlign( DT_CENTER );
        m_pkStatic_Complate_State->SetFontColor( D3DCOLOR_ARGB(255, 255, 234, 176) );
        m_pkStatic_Complate_State->SetFontOutline( true, D3DCOLOR_ARGB(255, 27, 21, 7) );
    }
    if( m_pkStatic_Desc )
    {
        m_pkStatic_Desc->InitState( true, false );
        m_pkStatic_Desc->SetFontSpace( 0 );
    }
    if( m_pkStatic_Exp )
        m_pkStatic_Exp->InitState( true, false );
    if( m_pkStatic_Gp )
        m_pkStatic_Gp->InitState( true, false );
}

void KGCMissionSubInfoBox_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionSubInfoBox_S4::FrameMoveInEnabledState( void ){}

void KGCMissionSubInfoBox_S4::CleanUp()
{
    m_pkStatic_Complate_State->SetText(L"");
    m_pkStatic_Title->SetText(L"");
    m_pkStatic_Desc->SetText(L"");
    m_pkStatic_Exp->SetText(L"");
    m_pkStatic_Gp->SetText(L"");
}

void KGCMissionSubInfoBox_S4::SetTitleText( int iTitleID )
{
    if( m_pkStatic_Title )
        m_pkStatic_Title->SetForceWordWrapText( SiKGCMissionManager()->GetString( iTitleID ), true );
}

void KGCMissionSubInfoBox_S4::SetComplateStateText( int iComplateCnt, int iMaxCnt )
{
    if( m_pkStatic_Complate_State )
    {
        WCHAR wszTemp[128];
        wsprintf(wszTemp, L"(%d/%d)", iComplateCnt, iMaxCnt);
        m_pkStatic_Complate_State->SetText( std::wstring( wszTemp ) );
    }
}

void KGCMissionSubInfoBox_S4::SetDescriptionText( int iDescID )
{
    if( m_pkStatic_Desc )
        m_pkStatic_Desc->SetTextAutoMultiline( SiKGCMissionManager()->GetString( iDescID ) );
}

void KGCMissionSubInfoBox_S4::SetExpText( int iExp )
{
    if( m_pkStatic_Exp )
    {
        std::string strTemp;
        strTemp = GCUTIL_STR::InsertCommaNumber( iExp );
        m_pkStatic_Exp->SetText( GCUTIL_STR::GCStrCharToWide( strTemp.c_str() ) );
    }
}

void KGCMissionSubInfoBox_S4::SetGpText( int iGp )
{
    if( m_pkStatic_Gp )
    {
        std::string strTemp;
        strTemp = GCUTIL_STR::InsertCommaNumber( iGp );
        m_pkStatic_Gp->SetText( GCUTIL_STR::GCStrCharToWide( strTemp.c_str() ) );
    }
}

void KGCMissionSubInfoBox_S4::SetPosY( float fY )
{
    D3DXVECTOR2 vecPos = this->GetFixedWindowLocalPos();
    vecPos.y = fY;
    this->SetFixedWindowLocalPos( vecPos );
}