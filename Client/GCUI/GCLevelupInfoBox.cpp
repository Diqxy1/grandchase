#include "stdafx.h"
#include "GCLevelupInfoBox.h"

IMPLEMENT_CLASSNAME( KGCLevelupInfoBox );
IMPLEMENT_WND_FACTORY( KGCLevelupInfoBox );
IMPLEMENT_WND_FACTORY_NAME( KGCLevelupInfoBox, "gc_levelup_info_box" );

KGCLevelupInfoBox::KGCLevelupInfoBox( void )
{
    m_pkBackground = NULL;
    LINK_CONTROL( "background", m_pkBackground );

    m_dxvLevelPos = D3DXVECTOR2( 0.0f, 0.0f);
    m_dwColor = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
    m_iLevel = 0;
    m_dwTimer = 0;
    m_bShow = false;
}

KGCLevelupInfoBox::~KGCLevelupInfoBox( void )
{
}

void KGCLevelupInfoBox::OnCreate( void )
{
    m_pkBackground->InitState( true, true, this );
    m_pkBackground->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    
    m_dxvLevelPos = D3DXVECTOR2( 0.0f, 0.0f);
    m_iLevel = 0;
    m_dwTimer = 0;
    m_bShow = false;
}

void KGCLevelupInfoBox::FrameMoveInEnabledState( void )
{
    FrameMoveLevelupInfo();
}

void KGCLevelupInfoBox::ActionPerformed( const KActionEvent& event )
{
    
}

void KGCLevelupInfoBox::PostChildDraw( void )
{
    if( m_bShow )
    {
        m_dxvLevelPos.y -= 0.05f * g_pGCDeviceManager->GetWindowScaleY();
        g_pkUIMgr->RenderNumber("orange_gradation_num", m_dxvLevelPos, m_iLevel, D3DXCOLOR(m_dwColor), false, false, -1.0f,true);
    }
}

void KGCLevelupInfoBox::InitInfo( void )
{
    m_pkBackground->SetWindowColor( D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) );
    m_dxvLevelPos = D3DXVECTOR2( 0.0f, 0.0f);
    m_dwColor = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
    m_iLevel = 0;
    m_dwTimer = 0;
    m_bShow = false;
}

void KGCLevelupInfoBox::OnLevelup( void )
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
    {
        PLAYER* pPlayer = g_MyD3D->GetMyPlayer();

        if( NULL == pPlayer )
            return;

        SCharInfo kCharInfo = pPlayer->GetCurrentChar();
        m_iLevel = kCharInfo.iLevel;

        if( 0 < m_iLevel && m_iLevel <= 80 )
        {
            m_dwTimer = ::timeGetTime();
            m_bShow = true;
        }
    }
    else
    {
        SCharInfo kCharInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
        m_iLevel = kCharInfo.iLevel + 1;

        if( 0 < m_iLevel && m_iLevel <= 80 )
        {
            m_dwTimer = ::timeGetTime();
            m_bShow = true;
        }
    }

    this->ToggleRender( m_bShow );
    m_pkBackground->ToggleRender( m_bShow );

    if( m_bShow )
    {
        float fX = X_POS;
        float fY = Y_POS;

        //KLuaManager luaMgr;
        //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
        //LUA_GET_VALUE_DEF( "x", fX, 0.0f );    
        //LUA_GET_VALUE_DEF( "y", fY, 0.0f );    

        fX *= g_pGCDeviceManager->GetWindowScaleX();
        fY *= g_pGCDeviceManager->GetWindowScaleY();

        m_dxvLevelPos = D3DXVECTOR2( fX, fY );
    }
}

void KGCLevelupInfoBox::FrameMoveLevelupInfo( void )
{
    if( false == m_bShow )
        return;

    D3DXCOLOR dxColor(m_dwColor);
    m_dwColor = D3DCOLOR_ARGB(static_cast<int>((dxColor.a-0.01f<0.01f?0.0f:dxColor.a-0.01f)*0xff), 0xff, 0xff, 0xff);
    m_pkBackground->SetWindowColor( m_dwColor );

    DWORD dwNowTime = ::timeGetTime();
    if( dwNowTime - m_dwTimer > ON_TIME )
    {
        m_bShow = false;
        m_dwTimer = dwNowTime;
        m_pkBackground->ToggleRender( m_bShow );
    }
}