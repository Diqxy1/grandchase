#include "stdafx.h"
#include "GCStateOption.h"

#include "MyD3D.h"

KGCStateOption::KGCStateOption(void)
{
}

KGCStateOption::~KGCStateOption(void)
{
}

bool KGCStateOption::OnInitBeforeStateChange(GAME_STATE eNowState_ )
{
	return true;
}

bool KGCStateOption::FrameMove(float fElapsedTime)
{
    SendP2PPacket( GC_PID_RELAY_PING );
    g_pkUIMgr->FrameMove( g_MyD3D->m_fTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );
	return true;
}

bool KGCStateOption::Render()
{
    if ( BeginScene() ) 
    {
	    g_MyD3D->RenderUI();
        EndScene();
    }

	return true;
}

bool KGCStateOption::OnInit()
{
	return true;
}

bool KGCStateOption::OnDestroyBeforeStateChange()
{
	return true;
}

void KGCStateOption::OnSetupFSM()
{
	ON_SETUP_FSM( GS_INIT_LOADING , FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_LOGIN ,         GS_SERVER );
	m_eStateForFail = FAIL_N_SHUTDOWN;	//이동 실패시 클라이언트를 종료합니다.
}

void KGCStateOption::OnDestroy()
{

}