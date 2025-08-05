#include "stdafx.h"
#include "GCMyInfoSceneSubMenuCloseFrame.h"
#include ".\gcmyinfoscene.h"
#include "GCUserItemWnd.h"

IMPLEMENT_CLASSNAME( KGCMyInfoSceneSubMenuFrame );
IMPLEMENT_WND_FACTORY( KGCMyInfoSceneSubMenuFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCMyInfoSceneSubMenuFrame, "gc_my_info_scene_sub_menu_close_frame" );

KGCMyInfoSceneSubMenuFrame::KGCMyInfoSceneSubMenuFrame( void )
:	m_pBtnInitAttribute(NULL)
,	m_pBtnSell(NULL)
,	m_pBtnSocket(NULL)
,	m_bSellMode(false)
{
	LINK_CONTROL("btn_sell" ,		m_pBtnSell );
	LINK_CONTROL("btn_init_attribute" ,		m_pBtnInitAttribute );
	LINK_CONTROL("btn_socket" ,		m_pBtnSocket );
}

KGCMyInfoSceneSubMenuFrame::~KGCMyInfoSceneSubMenuFrame( void )
{
}

void KGCMyInfoSceneSubMenuFrame::ActionPerformed( const KActionEvent& event )
{
}

void KGCMyInfoSceneSubMenuFrame::OnCreate( void )
{
	if(m_pBtnSell){
		m_pBtnSell->InitState(true,true,this);
		m_pBtnSell->Lock(false);
		m_pBtnSell->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoSceneSubMenuFrame::OnClickSellBtn );
		g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SELL_END, this, &KGCMyInfoSceneSubMenuFrame::OnClickSellCloseBtn );
	}

#ifndef NOT_USE_ITEM_ATTRIBUTE_SELECT
	if(m_pBtnInitAttribute){
		m_pBtnInitAttribute->InitState(true,true,this);
		m_pBtnInitAttribute->Lock(false);
		m_pBtnInitAttribute->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoSceneSubMenuFrame::OnClickInitAttributeBtn );
	}
	if(m_pBtnSocket){
		m_pBtnSocket->InitState(false,true,this);
		m_pBtnSocket->Lock(true);
		m_pBtnSocket->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoSceneSubMenuFrame::OnClickSocketBtn );
	}
#else
	if(m_pBtnSocket){
		m_pBtnSocket->InitState(true,true,this);
		m_pBtnSocket->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoSceneSubMenuFrame::OnClickSocketBtn );
	}
	if(m_pBtnInitAttribute){
		m_pBtnInitAttribute->InitState(false,true,this);
		m_pBtnInitAttribute->Lock(true);
	}
#endif
}

void KGCMyInfoSceneSubMenuFrame::OnClickSellBtn( const KActionEvent& event )
{
	//if(m_bSellMode){
	//	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SELL_EXIT) );
	//	m_bSellMode = false;
	//}
	//else{
	//	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SELL_DLG_OPEN_START) );
	//	m_bSellMode = true;
	//}
    if( false == m_bSellMode ) { 
        g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_SELL_DLG_OPEN_START) );
        m_bSellMode = true;
        
        ButtonRender( false, false, false );
    }
}

void KGCMyInfoSceneSubMenuFrame::OnClickSellCloseBtn( const KActionEvent& event )
{
	m_bSellMode = false;
}

void KGCMyInfoSceneSubMenuFrame::OnClickInitAttributeBtn( const KActionEvent& event )
{
	m_pBtnInitAttribute->Lock(true);
	m_pBtnInitAttribute->ToggleRender(false);

	m_pBtnSocket->Lock(false);
	m_pBtnSocket->ToggleRender(true);

	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE
		,KGCUserItemWnd::IBS_RANDOM_INIT_ATTRIBUTE ) );	
}

void KGCMyInfoSceneSubMenuFrame::OnClickSocketBtn( const KActionEvent& event )
{
#if defined( USE_ATTRIBUTE_SELECT_EVENT )
    // 타입 선택 UI
    if( g_pkUIScene->m_pkMyInfoScene->m_pkPopopAttributeSelect->IsRenderOn() ) { 
        g_pkUIScene->m_pkMyInfoScene->m_pkPopopAttributeSelect->ToggleRender( false );
    }
#endif

#ifndef NOT_USE_ITEM_ATTRIBUTE_SELECT
	m_pBtnSocket->Lock(true);
	m_pBtnSocket->ToggleRender(false);
	m_pBtnInitAttribute->Lock(false);
	m_pBtnInitAttribute->ToggleRender(true);
#endif
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE
		,KGCUserItemWnd::IBS_SOCKET ) );
}

void KGCMyInfoSceneSubMenuFrame::InitSubMenu()
{
	if(m_pBtnSell){
		m_pBtnSell->Lock(false);
	}

#ifndef NOT_USE_ITEM_ATTRIBUTE_SELECT
	if(m_pBtnInitAttribute){
		m_pBtnInitAttribute->ToggleRender(true);
		m_pBtnInitAttribute->Lock(false);
	}
	if(m_pBtnSocket){
		m_pBtnSocket->ToggleRender(false);
		m_pBtnSocket->Lock(true);
	}
#else
	if(m_pBtnInitAttribute){
		m_pBtnInitAttribute->ToggleRender(false);
		m_pBtnInitAttribute->Lock(true);
	}
	if(m_pBtnSocket){
		m_pBtnSocket->ToggleRender(true);
	}
#endif

	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE, KGCUserItemWnd::IBS_SOCKET ) );

}

void KGCMyInfoSceneSubMenuFrame::ButtonRender( bool bRender, bool bPet, bool bSell /*= true */ )
{
    if ( bRender ) 
    {
        if(m_pBtnInitAttribute && !m_pBtnInitAttribute->IsLocked() )
        {
            m_pBtnInitAttribute->ToggleRender(true);
        }
        if(m_pBtnSocket && !m_pBtnSocket->IsLocked() )
        {
            m_pBtnSocket->ToggleRender(true);
        }
    }
    else
    {
        if ( m_pBtnInitAttribute ) 
        {
            m_pBtnInitAttribute->ToggleRender( false );
        }

        if ( m_pBtnSocket ) 
        {
            m_pBtnSocket->ToggleRender( false );
        }
    }

	if( bPet )
	{
		if ( m_pBtnInitAttribute ) 
		{
			m_pBtnInitAttribute->ToggleRender( false );
		}

		if ( m_pBtnSocket ) 
		{
			m_pBtnSocket->ToggleRender( false );
		}
	}

    if( bSell ) { 
        if( m_pBtnSell && !m_pBtnSell->IsLocked() )
        {
            m_pBtnSell->ToggleRender( true );
        }
    }
    else if ( !bSell ) { 
        if( m_pBtnSell )
        {
            m_pBtnSell->ToggleRender( false );
        }
    }
}
