#include "stdafx.h"
#include "GCReplayBox.h"
//
//
//
#include "../MyD3D.h"


#include "../Replay.h"
//

IMPLEMENT_CLASSNAME( KGCReplayBox );
IMPLEMENT_WND_FACTORY( KGCReplayBox );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayBox, "gc_replay_box" );

KGCReplayBox::KGCReplayBox( void )
{
	m_iTopIndex     = 0;
	m_pkScrollbar   = NULL;
	m_pkExit		= NULL; 
	::ZeroMemory( m_apkBar, sizeof( m_apkBar ) );

	LINK_CONTROL( "replay_box_scroll",   m_pkScrollbar );
	LINK_CONTROL( "replay_exit",   m_pkExit );

	LINK_CONTROL( "replay_bar0", m_apkBar[0] );
	LINK_CONTROL( "replay_bar1", m_apkBar[1] );
	LINK_CONTROL( "replay_bar2", m_apkBar[2] );
	LINK_CONTROL( "replay_bar3", m_apkBar[3] );
	LINK_CONTROL( "replay_bar4", m_apkBar[4] );
	LINK_CONTROL( "replay_bar5", m_apkBar[5] );
	LINK_CONTROL( "replay_bar6", m_apkBar[6] );
	LINK_CONTROL( "replay_bar7", m_apkBar[7] );
	LINK_CONTROL( "replay_bar8", m_apkBar[8] );
}

KGCReplayBox::~KGCReplayBox( void )
{
	// empty
}

void KGCReplayBox::OnCreate( void )
{
	m_pkScrollbar->SetSelfInputCheck( false );
	m_pkScrollbar->AddActionListener( this );
	m_pkScrollbar->SetScrollPageSize( NUM_REPLAY_BAR );
	m_pkScrollbar->SetScrollPos( 0 );

	m_pkExit->SetSelfInputCheck( true );
	m_pkExit->AddActionListener( this );
    m_pkExit->SetHotKey( DIK_ESCAPE );
    m_pkExit->SetHotKey( DIK_RETURN );

	m_iTopIndex = 0;

	for ( int i = 0; i < NUM_REPLAY_BAR; ++i )
	{
		m_apkBar[i]->SetSelfInputCheck( true );
		m_apkBar[i]->AddActionListener( this );
	}
	
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_REPLAY_ON);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_REPLAY_ON, this, &KGCReplayBox::GoReplay );

	RequestList();
}

void KGCReplayBox::OnCreateComplete( void )
{
	SpeakToActionListener( KActionEvent( this, IRR_CREATED ) );
}

void KGCReplayBox::OnDestroyComplete( void )
{
	SpeakToActionListener( KActionEvent( this, IRR_DESTROYED ) );
}

void KGCReplayBox::ActionPerformed( const KActionEvent& event )
{
	// 스크롤바를 조작했다면 리스트를 갱신한다.
	if ( event.m_pWnd == m_pkScrollbar )
	{
		int iTop = m_pkScrollbar->GetScrollPos();
		if ( m_iTopIndex != iTop )
		{
			g_KDSound.Play( "73" );
			m_iTopIndex = iTop;
			UpdateReplayList();
		}
		return;
	}
	else if ( event.m_pWnd == m_pkExit )  //닫기 버튼에서 이벤트가 발생 했다면
	{
		switch ( event.m_dwCode )
		{
		case KD3DWnd::D3DWE_BUTTON_DOWN:
			{
				g_KDSound.Play( "31" );
				return;
			}
		case KD3DWnd::D3DWE_BUTTON_CLICK:
			{
				SpeakToActionListener( KActionEvent( this, IRR_CLOSE ) );
				return;
			}
		}
	}
	
	for ( int i = 0; i < NUM_REPLAY_BAR; ++i )
	{		
		if ( m_apkBar[i] == event.m_pWnd )
		{
			m_apkBar[i]->SetFontColor(0xff00ff00); //마우스 커서가 위치하면 녹색으로 표시.
			if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
			{
				g_MyD3D->m_iRPFileNum = m_iTopIndex + i;
				g_KDSound.Play( "31" );
				g_pkUIMgr->SendEvent( KActionEvent( NULL,  KGCUIScene::D3DWE_REPLAY_ON ) );
		
				break;
			}		
		}
		else
			m_apkBar[i]->SetFontColor(0xffffffff);  //마우스 커서가 위치하지 않으면 다시 흰색.
	}
}

void KGCReplayBox::GoReplay( void )
{
	Destroy();
	g_MyD3D->m_pStateMachine->GoReplay();
}


void KGCReplayBox::RequestList( void )
{
	g_MyD3D->MyReplay->GetList();

	UpdateReplayList();	
}

void KGCReplayBox::UpdateReplayList( void )
{
	static WCHAR s_strFileName[MAX_PATH] = {0,};
	// 스크롤바의 최대치를 조정한다.
	m_pkScrollbar->SetScrollRangeMax( g_MyD3D->MyReplay->m_iFileNum /*이 숫자는 반드시 MyReplay의 GetList 함수를 부른 다음에야 제대로 값을 가진다.*/); 

	// 리플레이 파일 리스트 정보를 입력한다.
	WCHAR strSep[] = {L"\\"};
	for ( int i = 0; i < NUM_REPLAY_BAR; i++ )
	{
		if ( m_iTopIndex + i < g_MyD3D->MyReplay->m_iFileNum )
		{
			m_apkBar[i]->ToggleRender( true );
			wcscpy( s_strFileName, g_MyD3D->MyReplay->m_vecFileList[m_iTopIndex+i].c_str() );
			WCHAR* strFileName = wcstok(s_strFileName, strSep );
			strFileName = wcstok( NULL, strSep );
			m_apkBar[i]->SetText( strFileName );
		}
		else
		{
			m_apkBar[i]->ToggleRender( false );
		}
	
	}
}
