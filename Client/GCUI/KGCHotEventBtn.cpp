#include "stdafx.h"
#include "KGCHotEventBtn.h"

IMPLEMENT_CLASSNAME( KGCHotEventBtn );
IMPLEMENT_WND_FACTORY( KGCHotEventBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCHotEventBtn, "gc_hot_event_btn" );

KGCHotEventBtn::KGCHotEventBtn( void )
: m_pkAniOn(0)
, m_pkAniOff(0)
{
	//LINK_CONTROL( "ani0", m_pkAniOn );
	//LINK_CONTROL( "ani1", m_pkAniOff );
}

KGCHotEventBtn::~KGCHotEventBtn( void )
{
}

void KGCHotEventBtn::OnCreate( void )
{
	ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHotEventBtn::OnClickBtn );
	ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCHotEventBtn::OnCursorEnter );

	m_pkAniOn = GetChildByName( "ani0" );
	m_pkAniOff = GetChildByName( "ani1" );

	m_pkAniOn->InitState( false );
	m_pkAniOff->InitState( false );
}

void KGCHotEventBtn::OnDestroy( void )
{
	DisconnectAllEventProcedure();
}

void KGCHotEventBtn::OnClickBtn()
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GAWIBAWIBO_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
//         Result_GawibawiboGameInfo = INT_MAX;
//         if( g_pkUIScene->m_pkGawibawiboGame )
//         {
//             KP2P::GetInstance()->Send_GetUserGwibawiboDataReq(g_pkUIScene->m_pkGawibawiboGame->GetVerInfo());
//             g_MyD3D->WaitForServerAck( Result_GawibawiboGameInfo, INT_MAX, 5000, TIME_OUT_VALUE );
//         }
        //if(g_pkUIScene->m_pkSphinxDlg == NULL)
            //return;
        if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
            return;

        //g_pkUIScene->m_pkSphinxDlg->InitState( KGCSphinxDlg::SEDS_CHALLENGE );
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_SPHINX_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
        if(g_pkUIScene->m_pkRickoonTornado)
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_RICKOON_TORNADO_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
        }
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_MINIGAME_LIST_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,false);
        return;	
    }
    
}

void KGCHotEventBtn::OnCursorEnter()
{
	m_pkAniOn->ToggleRender( false );
	m_pkAniOff->ToggleRender( true );
}

void KGCHotEventBtn::OnCreateComplete( void )
{
	SetOrder_Top();
}

void KGCHotEventBtn::FrameMoveInEnabledState()
{
	if( m_bHotState ){
		static bool	bBlit	=	false;
		static DWORD oNoffTimer = GetTickCount();
		if( GetTickCount() - oNoffTimer > 500 ){
			m_pkAniOn->ToggleRender( bBlit );
			m_pkAniOff->ToggleRender( !bBlit );
			bBlit = !bBlit;
			oNoffTimer = GetTickCount();
		}
	}
	else{
		m_pkAniOn->ToggleRender(false);
		m_pkAniOff->ToggleRender(false);
	}
}

void KGCHotEventBtn::BindToLua()
{
	
}

void KGCHotEventBtn::SetHotStateOn()
{
	m_bHotState = true;
}

void KGCHotEventBtn::SetHotStateOff()
{
	m_bHotState = false;

}

bool KGCHotEventBtn::m_bHotState	=	false;