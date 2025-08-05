#include "stdafx.h"
#include ".\gcindigolobby.h"
#include "MyD3D.h"
#include "GCStateMachine.h"

//
#include "KGCContestHelp.h"
#include "GCIndigoRanking.h"

IMPLEMENT_CLASSNAME( KGCIndigoLobby );
IMPLEMENT_WND_FACTORY( KGCIndigoLobby );
IMPLEMENT_WND_FACTORY_NAME( KGCIndigoLobby, "gc_indigo_lobby" );

KGCIndigoLobby::KGCIndigoLobby(void)
:m_pkEnter(NULL)
,m_pkHelp(NULL)
,m_pkRanking(NULL)
,m_pkReplay(NULL)
,m_pkPractice(NULL)
{
	LINK_CONTROL( "indigo_enter", m_pkEnter );
	LINK_CONTROL( "indigo_help", m_pkHelp );
	LINK_CONTROL( "indigo_ranking", m_pkRanking );
	LINK_CONTROL( "indigo_replay", m_pkReplay );
	LINK_CONTROL( "indigo_practice", m_pkPractice );
}

KGCIndigoLobby::~KGCIndigoLobby(void)
{
}

void KGCIndigoLobby::OnCreate()
{
	m_pkEnter->InitState( true, true, this );
	m_pkHelp->InitState( true );
	m_pkRanking->InitState( true );
	m_pkReplay->InitState( true, true, this );
	m_pkPractice->InitState( true, true, this );
}

void KGCIndigoLobby::ActionPerformed( const KActionEvent& event )
{
	GCWND_CODE_MAP( m_pkEnter, OnEnter );
	GCWND_CODE_MAP( m_pkReplay, OnReplay );
	GCWND_CODE_MAP( m_pkPractice, OnPractice );
}

void KGCIndigoLobby::OnEnter( DWORD dwCode)
{
	if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;

	if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
		return;

	if( FAILED( SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_QUICK ) ) )
	{
		KRoomOptions options;
		if (FAILED(SiKGCRoomManager()->CreateRoom(L"", L"", INDIGO_MAX_PLAYER, true, options)))
		{
			g_MyD3D->m_pStateMachine->GoMatch();
		}
	}
}

void KGCIndigoLobby::UpdateRanking()
{
	m_pkRanking->UpdateRankingData();
}

void KGCIndigoLobby::OnReplay( DWORD dwCode )
{
    if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
        return;

    //#if defined( SKILL_TREE )
    //    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_REPLAY_NOT_YET ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
    //    return ;
    //#endif
#if defined( REPLAY_BOX_NEW )
    if( g_kGlobalValue.m_bUseOldReplay )
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_S6 );
    else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_BOX_NEW );
#else
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_S6 );
#endif
}

void KGCIndigoLobby::OnPractice( DWORD dwCode )
{
	if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW );
}
