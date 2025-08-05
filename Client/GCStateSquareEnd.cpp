#include "stdafx.h"
#include "GCStateSquareEnd.h"
//
#include "./Square/GCSquare.h"
#include "KGCChannelManager.h"



#include "MyD3D.h"




#include "KMci.h"

KGCStateSquareEnd::KGCStateSquareEnd(void)
{
}

KGCStateSquareEnd::~KGCStateSquareEnd(void)
{
}

bool KGCStateSquareEnd::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    SiKP2P()->Send_LeaveSquareReq();
    return true;
}

bool KGCStateSquareEnd::FrameMove(float fElapsedTime)
{
    return true;
}

bool KGCStateSquareEnd::Render()
{
    return true;
}

bool KGCStateSquareEnd::OnInit()
{
    g_KMci.Stop();


    // ±¤ÀåÀ» ´Ý°í
    SiGCSquare()->CloseSquare();
    g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_ON ) );

    if( true == SiGCSquare()->GetJoinedChannel() )
    {
        if ( FAILED( g_MyD3D->EnterChannel( SiGCSquare()->GetChannelID() ) ) )
            return false;

        g_MyD3D->m_pStateMachine->SetJoinChannel( true );
        SiKGCChannelManager()->SetCurrentChannel( SiGCSquare()->GetChannelID() );
        SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
    }
    else if( true == SiGCSquare()->GetJoinedGuild() )
    {
        if( FAILED( g_MyD3D->EnterGuildRoom() ) )
        {
            return false;
        }
        g_MyD3D->m_pStateMachine->SetJoinGuild( true );
    }

    SiGCSquare()->RefreshRenderTech();

    g_MyD3D->m_pStateMachine->GoState( SiGCSquare()->GetPrevState() );
    g_MyD3D->m_pStateMachine->Clear_SavedState();
    if( false == SiGCSquare()->SavedStateEmpty() )
    {
        g_MyD3D->m_pStateMachine->Set_SavedState( SiGCSquare()->GetSavedState() );
        SiGCSquare()->ClearSavedState();
    }
    return true;
}

bool KGCStateSquareEnd::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateSquareEnd::OnDestroy()
{
}

void KGCStateSquareEnd::OnSetupFSM()
{
    ON_SETUP_FSM( GS_WORLDMAP,  GS_SERVER );
    ON_SETUP_FSM( GS_MATCH,     GS_SERVER );
    ON_SETUP_FSM( GS_GUILD,     GS_SERVER );
    ON_SETUP_FSM( GS_MY_INFO,   GS_SERVER );
    ON_SETUP_FSM( GS_SHOP_CASH,      GS_SERVER );
#ifndef DISABLE_MAP_SHOP // Kawan>
    ON_SETUP_FSM( GS_SHOP_GP,      GS_SERVER );
#endif
#if defined USE_COORDI_SHOP
    ON_SETUP_FSM( GS_COORDI_SHOP, GS_SERVER );
#endif
    
    ON_SETUP_FSM( GS_SERVER,    FAIL_N_SHUTDOWN);

    m_eStateForFail = GS_SERVER;
}
