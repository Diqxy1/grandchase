#include "stdafx.h"
#include "KGCMissionListBar_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionListBar_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionListBar_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionListBar_S4, "gc_mission_list_bar" );

KGCMissionListBar_S4::KGCMissionListBar_S4( void )
: m_pkMark_New( NULL )
, m_pkMark_Complate( NULL )
, m_pkStatic_Level( NULL )
, m_pkStatic_Title( NULL )
, m_pkStatic_Chain( NULL )
, m_pkBackground( NULL )
, m_FuncCallBack( NULL )
, m_iBarIdx( 0 )
, m_iAlphaCnt( 0 )
, m_iSwitch( 8 )
, m_bPrograssMode( false )
{
    LINK_CONTROL( "new_mark", m_pkMark_New );
    LINK_CONTROL( "complate_mark", m_pkMark_Complate );
    LINK_CONTROL( "level", m_pkStatic_Level );
    LINK_CONTROL( "title", m_pkStatic_Title );
    LINK_CONTROL( "chain", m_pkStatic_Chain );
    LINK_CONTROL( "background", m_pkBackground );

    m_pkPrograssBar.SetBackTexture("ui_game22.dds");                      // 텍스쳐 셋팅
    m_pkPrograssBar.SetBGFrontUV( 384.0f, 435.0f, 387.0f, 445.0f );       // Back Front UV
    m_pkPrograssBar.SetBGCenterUV( 387.0f, 435.0f, 437.0f, 445.0f );      // Back Center UV
    m_pkPrograssBar.SetBGBackUV( 437.0f, 435.0f, 440.0f, 445.0f );        // Back Back UV
    m_pkPrograssBar.SetBarTexture("ui_game22.dds");                       // 텍스쳐 셋팅
    m_pkPrograssBar.SetBarFrontUV( 387.0f, 428.0f, 388.0f, 434.0f );      // Bar Front UV
    m_pkPrograssBar.SetBarCenterUV( 389.0f, 428.0f, 392.0f, 434.0f );     // Bar Center UV
    m_pkPrograssBar.SetBarBackUV( 393.0f, 428.0f, 394.0f, 434.0f );       // Bar Back UV

    char szName[MAX_PATH];
    for (int i = 0 ; i < MAX_MISSION_TYPE ; ++i)
    {
        m_pkMissionTypeIcon[i] = NULL;
        sprintf( szName, "type_%d", i );
        LINK_CONTROL( szName, m_pkMissionTypeIcon[i] );
    }
}

KGCMissionListBar_S4::~KGCMissionListBar_S4( void ){}

void KGCMissionListBar_S4::OnCreateComplete( void )
{
    if( m_pkStatic_Level )
    {
        m_pkStatic_Level->InitState(!m_bPrograssMode, true, this);
        m_pkStatic_Level->DisconnectAllEventProcedure();
        m_pkStatic_Level->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_PRESS, this, &KGCMissionListBar_S4::OnPressBar );
        m_pkStatic_Level->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionListBar_S4::OnClickBar );
        m_pkStatic_Level->SetAlign( DT_CENTER );
        m_pkStatic_Level->SetFontSpace( 1 );
    }
    if( m_pkStatic_Title )
    {
        m_pkStatic_Title->InitState(true, true, this);
        m_pkStatic_Title->DisconnectAllEventProcedure();
        m_pkStatic_Title->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_PRESS, this, &KGCMissionListBar_S4::OnPressBar );
        m_pkStatic_Title->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionListBar_S4::OnClickBar );
        m_pkStatic_Title->SetAlign( DT_LEFT );
        m_pkStatic_Title->SetFontSpace( 1 );
    }
    if( m_pkStatic_Chain )
    {
        m_pkStatic_Chain->InitState(!m_bPrograssMode, true, this);
        m_pkStatic_Chain->DisconnectAllEventProcedure();
        m_pkStatic_Chain->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_PRESS, this, &KGCMissionListBar_S4::OnPressBar );
        m_pkStatic_Chain->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionListBar_S4::OnClickBar );
        m_pkStatic_Chain->SetAlign( DT_CENTER );
        m_pkStatic_Chain->SetFontSpace( 1 );
    }
    if( m_pkBackground )
    {
        m_pkBackground->InitState(true, true, this);
        m_pkBackground->DisconnectAllEventProcedure();
        m_pkBackground->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionListBar_S4::OnClickBar );
        m_pkBackground->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionListBar_S4::OnEnterCursor );
    }

    D3DXVECTOR2 vecPos = this->GetFixedWindowLocalPos();
    vecPos.x /= GC_SCREEN_DIV_WIDTH;
    vecPos.y /= GC_SCREEN_DIV_HEIGHT;
    vecPos.x += 315.0f; 
    vecPos.y += 45.0f;
    m_pkPrograssBar.SetRenderPos( vecPos.x, vecPos.y );
    m_pkPrograssBar.SetWidth( 70.0f );
    m_pkPrograssBar.SetHeight( 8.0f );
}

void KGCMissionListBar_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionListBar_S4::FrameMoveInEnabledState( void )
{
    m_iAlphaCnt += m_iSwitch;
    if( 255 <= m_iAlphaCnt || m_iAlphaCnt <= 0 )
    {
        m_iAlphaCnt = (255<=m_iAlphaCnt) ? 255:0;
        m_iSwitch *= -1;
    }

    if( m_pkMark_New->IsRenderOn() )
        m_pkMark_New->SetAllWindowAlpha( m_iAlphaCnt );
    if( m_pkMark_Complate->IsRenderOn() )
        m_pkMark_Complate->SetAllWindowAlpha( m_iAlphaCnt );
}

void KGCMissionListBar_S4::PostChildDraw( void )
{
    if( m_bPrograssMode )
        m_pkPrograssBar.DrawPrograssBar( GC_LAYER_UI );
}

void KGCMissionListBar_S4::OnClickBar()
{
    if( m_FuncCallBack )
        m_FuncCallBack( m_iBarIdx );
}

void KGCMissionListBar_S4::OnPressBar()
{
    if( m_pkBackground )
    {
        m_pkBackground->SetWndMode( D3DWM_SELECT );
        m_pkBackground->OnWndModeChange();
    }
}

void KGCMissionListBar_S4::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCMissionListBar_S4::SetNewMark( bool bEnable )
{
    m_iAlphaCnt = 0;
    if( m_pkMark_New )
        m_pkMark_New->ToggleRender( bEnable );
}

void KGCMissionListBar_S4::SetComplateMark( bool bEnable )
{
    m_iAlphaCnt = 0;
    if( m_pkMark_Complate )
        m_pkMark_Complate->ToggleRender( bEnable );
}

void KGCMissionListBar_S4::SetLevelText( std::wstring szText )
{
    if( m_pkStatic_Level )
        m_pkStatic_Level->SetText( szText );
}

void KGCMissionListBar_S4::SetTitleText( std::wstring szText )
{
    if( m_pkStatic_Title )
        m_pkStatic_Title->SetForceWordWrapText( szText, true );
}

void KGCMissionListBar_S4::SetChainText( std::wstring szText )
{
    if( m_pkStatic_Chain )
        m_pkStatic_Chain->SetText( szText );
}

void KGCMissionListBar_S4::SetFontColor( D3DCOLOR dwColor )
{
    if( m_pkStatic_Level )
        m_pkStatic_Level->SetFontColor( dwColor );
    if( m_pkStatic_Title )
        m_pkStatic_Title->SetFontColor( dwColor );
    if( m_pkStatic_Chain )
        m_pkStatic_Chain->SetFontColor( dwColor );
}

void KGCMissionListBar_S4::SetOutlineColor( D3DCOLOR dwColor )
{
    if( m_pkStatic_Level )
        m_pkStatic_Level->SetFontOutline( true, dwColor );
    if( m_pkStatic_Title )
        m_pkStatic_Title->SetFontOutline( true, dwColor );
    if( m_pkStatic_Chain )
        m_pkStatic_Chain->SetFontOutline( true, dwColor );
}

void KGCMissionListBar_S4::ClearText()
{
    if( m_pkStatic_Level )
        m_pkStatic_Level->SetText(L""); 
    if( m_pkStatic_Title )
        m_pkStatic_Title->SetText(L"");
    if( m_pkStatic_Chain )
        m_pkStatic_Chain->SetText(L"");
    SetNewMark( false );
    SetComplateMark( false );
    m_iAlphaCnt = 0;
}

void KGCMissionListBar_S4::SwapBackGround( bool bChange )
{
    if( m_pkBackground )
        m_pkBackground->SwapDef( bChange );
}

void KGCMissionListBar_S4::SetProgressMode( bool bSwitch )
{
    // 타이틀 이동
    if( m_pkStatic_Title )
    {
        D3DXVECTOR2 vecPos = m_pkStatic_Title->GetFixedWindowLocalPos();
        vecPos.x += bSwitch ? -30 : 30;
        m_pkStatic_Title->SetFixedWindowLocalPos( vecPos );

        vecPos = m_pkMissionTypeIcon[0]->GetFixedWindowLocalPos();
        vecPos.x += bSwitch ? -30 : 30;
        for (int i = 0 ; i < MAX_MISSION_TYPE ; ++i)
        {
            m_pkMissionTypeIcon[i]->SetFixedWindowLocalPos( vecPos );
        }
    }

    // Toggle
    SetProgressBarToggle( bSwitch );
}

void KGCMissionListBar_S4::SetProgressBarToggle( bool bSwitch )
{
    if( m_pkStatic_Chain )
        m_pkStatic_Chain->ToggleRender( !bSwitch );
    if( m_pkStatic_Level )
        m_pkStatic_Level->ToggleRender( !bSwitch );
    m_bPrograssMode = bSwitch;
}

void KGCMissionListBar_S4::SetProgressBarValue( int iNowValue, int iMinValue, int iMaxValue )
{
    m_pkPrograssBar.SetMinValue( (iMinValue > -1) ? iMinValue : 0 );
    m_pkPrograssBar.SetMaxValue( (iMaxValue > -1) ? iMaxValue : 0 );
    m_pkPrograssBar.SetNowValue( (iNowValue > -1) ? iNowValue : 0 );
}

void KGCMissionListBar_S4::SetMissionTypeIcon( int iconType )
{
    if( m_pkMissionTypeIcon[iconType] == NULL )
        return;

    for (int i = 0 ; i < MAX_MISSION_TYPE ; ++i)
    {
        if ( i == iconType )
            m_pkMissionTypeIcon[i]->ToggleRender(true);
        else
            m_pkMissionTypeIcon[i]->ToggleRender(false);
    }
}