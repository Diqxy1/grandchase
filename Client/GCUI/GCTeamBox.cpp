#include "stdafx.h"
#include "GCTeamBox.h"
//
#include "../MyD3D.h"


//
#include "KGCUIHelper.h"
#include "KGCChannelManager.h"
//
#include "KGCRoomManager.h"
#include "KGCRoomSceneS6.h"

IMPLEMENT_CLASSNAME( KGCTeamBox );
IMPLEMENT_WND_FACTORY( KGCTeamBox );
IMPLEMENT_WND_FACTORY_NAME( KGCTeamBox, "gc_team_box" );

KGCTeamBox::KGCTeamBox( void )
{
    m_bEnableTeamSel = true;
    m_pkSherdinBtn  = NULL;
    m_pkCanabanBtn  = NULL;

    LINK_CONTROL( "team_left_btn",   m_pkSherdinBtn );
    LINK_CONTROL( "team_right_btn",  m_pkCanabanBtn );
}

KGCTeamBox::~KGCTeamBox( void )
{
}

void KGCTeamBox::OnCreate( void )
{
    m_pkSherdinBtn->SetSelfInputCheck( true );
    m_pkCanabanBtn->SetSelfInputCheck( true );

	m_pkSherdinBtn->AddActionListener( this );
	m_pkCanabanBtn->AddActionListener( this );	
}

void KGCTeamBox::OnInitialize( void )
{
	UpdateGameData();
}

void KGCTeamBox::ActionPerformed( const KActionEvent& event )
{

	if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK)
		return;

	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_GUILD_BATTLE )		// 길드 채널에서는 선택할 수 없다
		return;

	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AUTOMATCH ) //자동 매칭에서는 팀선택은 자동으로 됨.
		return;

    //개인전에서도 팀이동 할 수 있도록 주석처리
    //if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT) //서바이벌에서는 팀 선택 할수 없음.
    //	return;


	int iIndex = g_MyD3D->Get_MyPlayer();
	
	g_KDSound.Play( "31" );

	if( g_kGlobalValue.m_kUserInfo.bHost ||
        (g_kGlobalValue.m_kUserInfo.bHost == false && g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY ) )
	{	
		// milennium9
		// 방에서 버튼을 누른 후 0.3초 딜레이 발생
		if( g_pkUIHelper->GetUILockTime( LOCK_ROOM ) == 0 )
		{
			g_kGlobalValue.m_kUserInfo.iTeam = (event.m_pWnd == m_pkCanabanBtn);
			KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( g_kGlobalValue.m_kUserInfo, 
				KChangeRoomUserInfo::RUI_TEAM, g_MyD3D->Get_MyPlayer() );

            KP2P::GetInstance()->Send_RoomMemberPingInfoReq();

			g_pkUIHelper->SetUILockTime( LOCK_ROOM , BUTTON_LOCK_FRAME );

            g_pkUIScene->m_pkRoomSceneS6->SetEnableStart( false );
		}		
	}
}

void KGCTeamBox::UpdateGameData()
{
	if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
	{
		EnableTeamBox( false );
		return;
	}
			EnableTeamBox( true );
    SetTeam( !g_kGlobalValue.m_kUserInfo.IsBlueTeam() );
    //개인전에서도 팀이동 할 수 있도록
    //if ( GC_GMT_ALONE_FIGHT == SiKGCRoomManager()->GetGameModeType() ||
    //	GC_GMT_UNION_FIGHT == SiKGCRoomManager()->GetGameModeType() )
    //{
    //	if ( true == m_bEnableTeamSel ) 
    //		EnableTeamBox( false );
    //	return;
    //}
    //else
    //{
    //	if ( false == m_bEnableTeamSel ) 
    //		EnableTeamBox( true );

    //	SetTeam( !g_kGlobalValue.m_kUserInfo.IsBlueTeam() );
    //}
	}

void KGCTeamBox::FrameMoveInEnabledState( void )
{

    }

void KGCTeamBox::EnableTeamBox( bool bEnable )
{
    if ( m_bEnableTeamSel == bEnable ) // NOT '=='
    {
        m_pkSherdinBtn->Lock( false );
        m_pkCanabanBtn->Lock( false );
    }
    else
    {
        m_pkSherdinBtn->Lock( true );
        m_pkCanabanBtn->Lock( true );
        m_pkSherdinBtn->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
        m_pkCanabanBtn->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
    }
}

void KGCTeamBox::SetTeam( bool bSherdinTeam )
{
	m_pkSherdinBtn->Activate( bSherdinTeam );
	m_pkCanabanBtn->Activate( !bSherdinTeam );
}
