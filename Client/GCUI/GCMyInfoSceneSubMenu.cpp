#include "stdafx.h"
#include "GCMyInfoSceneSubMenu.h"
#include "GCMyInfoSceneSubMenuCloseFrame.h"

IMPLEMENT_CLASSNAME( KGCMyInfoSceneSubMenu );
IMPLEMENT_WND_FACTORY( KGCMyInfoSceneSubMenu );
IMPLEMENT_WND_FACTORY_NAME( KGCMyInfoSceneSubMenu, "gc_my_info_scene_sub_menu" );

KGCMyInfoSceneSubMenu::KGCMyInfoSceneSubMenu( void )
:	m_pBtnOpen(NULL)
,	m_pBtnClose(NULL)
,	m_pCloseFrame(NULL)

{
	LINK_CONTROL("btn_open" ,		m_pBtnOpen );
	LINK_CONTROL("btn_close" ,		m_pBtnClose );
	LINK_CONTROL("close_frame" ,		m_pCloseFrame );

}

KGCMyInfoSceneSubMenu::~KGCMyInfoSceneSubMenu( void )
{
}

void KGCMyInfoSceneSubMenu::ActionPerformed( const KActionEvent& event )
{
}

void KGCMyInfoSceneSubMenu::OnCreate( void )
{
	if(m_pBtnOpen)
	{
		m_pBtnOpen->InitState(false,true,this);
		m_pBtnOpen->Lock(true);
		m_pBtnOpen->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoSceneSubMenu::ClickOpenBtn );

	}
	if(m_pBtnClose)
	{
		m_pBtnClose->InitState(false,true,this);
		m_pBtnClose->Lock(true);
		m_pBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMyInfoSceneSubMenu::ClickCloseBtn );
		g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SELL_DLG_OPEN_START, this, &KGCMyInfoSceneSubMenu::ClickCloseBtn );
	}
	if(m_pCloseFrame)
	{
		m_pCloseFrame->InitState(true,true,NULL);
	}


}

void KGCMyInfoSceneSubMenu::ClickOpenBtn()
{
	if(m_pCloseFrame)
		m_pCloseFrame->ToggleRender(true);
	if(m_pBtnOpen){
		m_pBtnOpen->Lock(true);
		m_pBtnOpen->ToggleRender(false);
	}
	if(m_pBtnClose){
		m_pBtnClose->Lock(false);
		m_pBtnClose->ToggleRender(true);
	}
}

void KGCMyInfoSceneSubMenu::ClickCloseBtn()
{
	return;
	//아래 현재 사용은 안하지만 UI개편 후 사용 예정.

	if(m_pCloseFrame)
		m_pCloseFrame->ToggleRender(false);
	if(m_pBtnOpen){
		m_pBtnOpen->Lock(false);
		m_pBtnOpen->ToggleRender(true);
	}
	if(m_pBtnClose){
		m_pBtnClose->Lock(true);
		m_pBtnClose->ToggleRender(false);
	}
}

void KGCMyInfoSceneSubMenu::InitSubMenu()
{
	m_pCloseFrame->InitSubMenu();

	return;
	//아래 현재 사용은 안하지만 UI개편 후 사용 예정.

	if(m_pCloseFrame)
		m_pCloseFrame->ToggleRender(false);
	if(m_pBtnOpen){
		m_pBtnOpen->Lock(false);
		m_pBtnOpen->ToggleRender(true);
	}
	if(m_pBtnClose){
		m_pBtnClose->Lock(true);
		m_pBtnClose->ToggleRender(false);
	}

}

void KGCMyInfoSceneSubMenu::ButtonRender( bool bRender, bool bPet, bool bSell /*=true*/)
{
    m_pCloseFrame->ButtonRender( bRender, bPet, bSell );

    if( false == bRender && bPet == false)
	{
		g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_MYINFOSCENE_SUB_BTN_MENU_STATE_UPDATE, KGCUserItemWnd::IBS_DEFAULT ) );
	}
}