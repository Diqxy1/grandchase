#include "stdafx.h"
#include ".\gcgameresultui.h"

#include "../KGCRoomManager.h"
#include "MyD3D.h"
#include "Replay.h"
#include "GCMatchResultUI.h"

IMPLEMENT_CLASSNAME( KGCGameResultUI );
IMPLEMENT_WND_FACTORY( KGCGameResultUI );
IMPLEMENT_WND_FACTORY_NAME( KGCGameResultUI, "gc_game_result" );

KGCGameResultUI::KGCGameResultUI(void)
: m_pkMatchResultUI( NULL )
, m_pkReplay( NULL )
, m_iReplayBtnState( 0 )
{
	LINK_CONTROL( "match_result",	m_pkMatchResultUI );
	LINK_CONTROL( "btn_replay",		m_pkReplay );
}

KGCGameResultUI::~KGCGameResultUI(void)
{
}

void KGCGameResultUI::OnCreate()
{
	m_pkMatchResultUI->InitState( false, true, this );
	m_pkReplay->InitState( true , true, this );

    g_pkUIMgr->DisconnectGlobalEventProcedure( GetUID(), KGCUIScene::D3DWE_END_GAME );
    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_END_GAME, this, &KGCGameResultUI::ShowGuildResult );

}

void KGCGameResultUI::FrameMoveInEnabledState()
{
	FrameMoveReplayBtn();
	if( IsResultComplete == true &&
		m_pkMatchResultUI->IsRenderOn() == false &&
		( IsMatchMode() == true || 
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER || 
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA) )
	{
        g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_END_GAME) );

		if( g_MyD3D->MyReplay->m_iCurrState != RP_RECORD )
		{
			m_pkReplay->ToggleRender( true );
			m_iReplayBtnState = 1;
		}
	}

}

bool KGCGameResultUI::IsMatchMode()
{
	EGCGameModeCategory eCurrentGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
	switch( eCurrentGameModeCategory )
	{
	case GC_GMC_MATCH:
	case GC_GMC_TAG_MATCH:
	case GC_GMC_INDIGO:
	case GC_GMC_GUILD_BATTLE:
    case GC_GMC_DEATHMATCH:
    case GC_GMC_FATAL_DEATHMATCH:
    case GC_GMC_ANGELS_EGG:
	case GC_GMC_CAPTAIN:
	case GC_GMC_AUTOMATCH:
		return true;
	}
	return false;
}

//
void KGCGameResultUI::FrameMoveReplayBtn()
{
	static int iFrameCount = 0;
	static float fAccelation = 3.0f;
	D3DXVECTOR2 vPos( m_pkReplay->GetFixedWindowLocalPos() );

	// 스크립트의 역할을 코드가 하고 있으니 코드에 마법의 숫자들을 넣습니다.
	// 버튼이 이동해서 자리 자리를 찾아가는 애니메이션입니다.
	switch( m_iReplayBtnState )
	{
	case 0:
		m_pkReplay->SetWindowPosDirect( D3DXVECTOR2( vPos.x, (float)(-1.0f * m_pkReplay->GetHeight() ) ) );
		iFrameCount = 0;
		fAccelation = 1.0f;
		break;
	case 1:

		m_pkReplay->SetWindowPosDirect( vPos + D3DXVECTOR2( 0.0f, fAccelation ) );

		if( vPos.y + fAccelation > 14.8f && vPos.y + fAccelation < 15.2f )
		{
			m_iReplayBtnState = 2;
			break;
		}

		if( (vPos.y + fAccelation < 15.0f && fAccelation < 0.0f )|| (vPos.y + fAccelation > 15.0f && fAccelation > 0.0f ) )
		{
			fAccelation *= -0.7f;
			break;
		}
		break;
	case 2:
		fAccelation = 2.0f;
		break;
	case 3:
		m_pkReplay->SetWindowPosDirect( vPos + D3DXVECTOR2( 0.0f, fAccelation ) );
		fAccelation -= 0.3f;

		if( vPos.y + fAccelation < (float)(-1.0f * m_pkReplay->GetHeight() ) )
		{
			m_iReplayBtnState = 0;
			break;
		}
		break;
	};
}

void KGCGameResultUI::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkReplay )
		{
			g_MyD3D->MyReplay->SaveReplayStart();
			m_iReplayBtnState = 3;
		}
	}
}

void KGCGameResultUI::ShowGuildResult()
{
    m_pkMatchResultUI->ShowResultStart();
}


