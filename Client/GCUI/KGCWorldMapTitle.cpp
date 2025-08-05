#include "stdafx.h"
#include "KGCWorldMapTitle.h"

IMPLEMENT_CLASSNAME( KGCWorldMapTitle );
IMPLEMENT_WND_FACTORY( KGCWorldMapTitle );
IMPLEMENT_WND_FACTORY_NAME( KGCWorldMapTitle, "gc_worldmap_title" );

KGCWorldMapTitle::KGCWorldMapTitle( void )
{
    m_iCurrentCharacterLv   = 0;
    LINK_CONTROL( "icon",m_pkIcon);
    LINK_CONTROL( "name",m_pkName);
    m_eContentsType = (CONTINENT_CONTENTS)-1;     
}

KGCWorldMapTitle::~KGCWorldMapTitle(void) 
{
}

void KGCWorldMapTitle::OnCreate()
{
    if( m_pkIcon )
        m_pkIcon->InitState( true, true, this );
 
    if( m_pkName )
        m_pkName->InitState( true, true, this );
}
void KGCWorldMapTitle::SetControlWndMode( ED3DWndMode eMode )
{
    m_pkName->SetWndMode(eMode );
    m_pkIcon->SetWndMode(eMode );
}
void KGCWorldMapTitle::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        if( event.m_pWnd == m_pkIcon )
        {
            //SetControlWndMode(D3DWM_HOVER);              
            m_pkName->SetWndMode(D3DWM_HOVER );
            m_pkIcon->SetWndMode(D3DWM_HOVER );
        }

    }
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP ) { 
            return;
        }

        if( event.m_pWnd == m_pkIcon )             //대륙 상점으로 이동
        {
            if( GetContentsType() == STATE_GP_SHOP )
                g_MyD3D->m_pStateMachine->GoState( GS_SHOP_GP );
            
            else if( GetContentsType() == STATE_MANUFACTURE )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FORGE_DLG );
            
            else if( GetContentsType() == STATE_ENCHANT )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_S6_DLG );
            }
            
            else if( GetContentsType() == STATE_INVALID )
                return;
        }
    }
}
void KGCWorldMapTitle::SetContentsType(CONTINENT_CONTENTS eContentsType)
{
    m_eContentsType = eContentsType;
}
CONTINENT_CONTENTS KGCWorldMapTitle::GetContentsType()
{
    return m_eContentsType;
}
void KGCWorldMapTitle::OnCreateComplete( void ){}
void KGCWorldMapTitle::FrameMoveInEnabledState( void )
{
    if( GetContentsType() == STATE_INVALID )
        return;
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
 
    if( m_pkName->CheckPosInWindowBound( vMousePos ) )
    {
        m_pkIcon->SetWndMode(D3DWM_HOVER);
        m_pkName->SetWndMode(D3DWM_HOVER);
    }
    else if( m_pkIcon->CheckPosInWindowBound( vMousePos ) )
    {
        m_pkName->SetWndMode(D3DWM_HOVER);
        m_pkIcon->SetWndMode(D3DWM_HOVER);
    }
}



IMPLEMENT_CLASSNAME( KGCWorldMapUpperTitle );
IMPLEMENT_WND_FACTORY( KGCWorldMapUpperTitle );
IMPLEMENT_WND_FACTORY_NAME( KGCWorldMapUpperTitle, "gc_worldmap_upper_title" );

KGCWorldMapUpperTitle::KGCWorldMapUpperTitle( void )
{
    m_bUp = false;
    m_pkIcon = NULL;
    m_pkName = NULL;
    m_pkCharIcon = NULL;
    m_pkQuestIcon = NULL;

    LINK_CONTROL( "base_icon",m_pkIcon );
    LINK_CONTROL( "base_name",m_pkName );
    LINK_CONTROL( "char_icon",m_pkCharIcon );
    LINK_CONTROL( "quest_icon",m_pkQuestIcon );

    
    m_d3dVecCreateCharIconPos = D3DXVECTOR2(0.f,0.f);
    m_d3dVecCharIconPos = D3DXVECTOR2(0.f,0.f);

}

KGCWorldMapUpperTitle::~KGCWorldMapUpperTitle(void) 
{
}
void KGCWorldMapUpperTitle::OnCreateComplete( void ){}
void KGCWorldMapUpperTitle::OnCreate()
{
    if( m_pkIcon )
        m_pkIcon->InitState( false, false, NULL);
    if( m_pkName )
        m_pkName->InitState( true, false, NULL);
    if( m_pkCharIcon  )
        m_pkCharIcon->InitState( true, false, NULL );
    if( m_pkQuestIcon  )
        m_pkQuestIcon->InitState( false, false, NULL );


    m_d3dVecCreateCharIconPos = m_pkCharIcon->GetFixedWindowLocalPos();
    m_d3dVecCharIconPos = m_d3dVecCreateCharIconPos;
}
void KGCWorldMapUpperTitle::SetRenderIcon( bool bRender)
{
    if( m_pkIcon )
        m_pkIcon->ToggleRender( bRender );
}
void KGCWorldMapUpperTitle::SetRenderName( bool bRender)
{
    if( m_pkName )
        m_pkName->ToggleRender( bRender );
}
void KGCWorldMapUpperTitle::SetRenderCharIcon( bool bRender)
{
    if( m_pkCharIcon )
        m_pkCharIcon->ToggleRender( bRender );
}
void KGCWorldMapUpperTitle::SetRenderQuestIcon( bool bRender)
{
    if( m_pkQuestIcon )
        m_pkQuestIcon->ToggleRender( bRender );
}


void KGCWorldMapUpperTitle::SetCharIconTexNum(int iTexNum)
{
    if( m_pkCharIcon )
        m_pkCharIcon->SetWndTex(D3DWM_DEFAULT, iTexNum);
}
void KGCWorldMapUpperTitle::SetCharIconTexCoord(D3DXVECTOR4 vecTexCoord)
{
    if( m_pkCharIcon )
        m_pkCharIcon->SetWndTexCoords(D3DWM_DEFAULT,vecTexCoord);
}

void KGCWorldMapUpperTitle::FrameMoveInEnabledState( void )
{
    if( SiKGCWorldMapManager()->GetCurrentWorldMap() == WORLDMAP_UPPERMAP )
        MoveCharIconUpAndDown();
}
void KGCWorldMapUpperTitle::ActionPerformed( const KActionEvent& event )
{

}
//아이콘을 위 아래로 움직인다.
void KGCWorldMapUpperTitle::MoveCharIconUpAndDown()
{
    if( !m_pkCharIcon->IsRenderOn() )
        return;

    if( m_d3dVecCharIconPos.y > m_d3dVecCreateCharIconPos.y - 7.f && !m_bUp )
    {
        m_d3dVecCharIconPos.y -= 0.5f;
    }
    else
    {
        m_bUp = true;
    }

    if( m_d3dVecCharIconPos.y < m_d3dVecCreateCharIconPos.y + 8.f && m_bUp )
    {
        m_d3dVecCharIconPos.y += 0.5f;
    }
    else
    {
        m_bUp = false;
    }

    m_pkCharIcon->SetWindowPosYDirect( m_d3dVecCharIconPos.y );

}
