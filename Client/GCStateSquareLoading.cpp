#include "stdafx.h"
//
#include "GCStateSquareLoading.h"

#include "dxutil.h"
#include "MapSystem/KGCLayerManager.h"


#include "KMci.h"
//
#include "./GCUIHeader.h"
#include "./Square/GCSquare.h"



#include "Procedure.h"
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"


#define FORCE_LOADING_TIME (3000)

KGCStateSquareLoading::KGCStateSquareLoading(void)
: m_dwLoadingTime(0)
, m_bSendLoadingComplete( false )
{
}

KGCStateSquareLoading::~KGCStateSquareLoading(void)
{
}

bool KGCStateSquareLoading::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    g_AnimManager.StartGame();
    return true;
}

bool KGCStateSquareLoading::FrameMove(float fElapsedTime)
{
    g_MyD3D->m_bKeepFPS60 = FALSE;
    // [1/7/2008] breadceo. square rough
    g_pkUIMgr->FrameMove( fElapsedTime );
    g_MyD3D->m_pUIAnim->FrameMove( g_MyD3D->m_fTime );

    g_pkUIScene->m_pkSquareLoadingScene->SetGage( SiGCSquare()->GetLoadPercent() );

    FRAME_TIMER_RESET;
    if( SiGCSquare()->IsLoadingComplete() )
    {
        if( m_bSendLoadingComplete == false )
        {
            m_bSendLoadingComplete = true;
            SiKP2P()->Send_SquareLodingComplete();
            SiKP2P()->Send_SyncNeighborhood();
        }

        if( ::timeGetTime() - m_dwLoadingTime < (DWORD)FORCE_LOADING_TIME )
        {
            return true;
        }

        if( false == g_pkUIScene->m_pkSquareLoadingScene->IsEventEnd() )
        {
            return true;
        }

        if( Result_Enter_Square == 0 )
        {
            g_pkUIScene->m_pkSquareLoadingScene->Init();
            g_MyD3D->m_pStateMachine->GoSquare();
            g_MyD3D->m_bKeepFPS60 = TRUE;
        }
    }
    else
    {
        SiGCSquare()->SquareLoading();
    }
    return true;
}

bool KGCStateSquareLoading::Render()
{
    // [1/7/2008] breadceo. square rough
    if ( BeginScene() )
    {
        g_MyD3D->RenderUI();
        g_MyD3D->RenderFPS();
        EndScene();
    }
    return true;
}

bool KGCStateSquareLoading::OnInit()
{
	
    g_MyD3D->m_pStateMachine->Clear_SavedState();

    g_KMci.Play( GC_BGM_SQUARE, true );

    g_pkUIScene->m_pkSquareLoadingScene->EnterScene();

	// 광장 발판 스크립트를 로드 합니다.
#if defined(LOADING_IMAGE_FROM_SERVER_SCRIPT)
	SiKGCLayerManager()->LoadScript( KncUtil::toNarrowString(g_kGlobalValue.GetScriptNameSquare()) );
#else
	SiKGCLayerManager()->LoadScript( "Square.lua" );
#endif

	// 광장 오브젝트 스크립트를 로드 합니다.
	SiGCSquare()->LoadObjectScript();
	
	SiGCSquare()->Load3DObjcetScript();
#if defined( ENABLE_MINIGAME )
    KP2P::GetInstance()->Send_MiniGameTopRank( 0 );
    KP2P::GetInstance()->Send_MiniGameTopRank( 1 );
    SiKGCChannelManager()->SetCurrentChannelType( CT_SQUARE );
    SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
    SiKGCRoomManager()->InitGameMode();
#endif

    m_dwLoadingTime = ::timeGetTime();
    Result_Enter_Square = INT_MAX;
    m_bSendLoadingComplete = false;
    return true;
}

bool KGCStateSquareLoading::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateSquareLoading::OnDestroy()
{
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->ClearPlayerData();
    }

	g_pkUIMgr->ClearUnusedTexture(0);
}

void KGCStateSquareLoading::OnSetupFSM()
{
    ON_SETUP_FSM( GS_SQUARE,    GS_SERVER );
    ON_SETUP_FSM( GS_SQUARE_END,GS_SERVER );
    ON_SETUP_FSM( GS_SERVER,    FAIL_N_SHUTDOWN);

    m_eStateForFail = GS_SERVER;
}
