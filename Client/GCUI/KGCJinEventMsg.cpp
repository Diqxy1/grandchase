#include "stdafx.h"
#include "KGCJinEventMsg.h"
#include "KDSound.h"
//#include "uifx/D3DStatic.h"
#include "KGCCategoryBox.h"
#include "MyD3D.h"
#include "GCStateMachine.h"

IMPLEMENT_CLASSNAME( KGCJinEventMsg );
IMPLEMENT_WND_FACTORY( KGCJinEventMsg );
IMPLEMENT_WND_FACTORY_NAME( KGCJinEventMsg, "gc_jin_event_msg" );

KGCJinEventMsg::KGCJinEventMsg(void)
{
	m_pkOK			= NULL;
	m_pkStatic		= NULL;

	LINK_CONTROL( "btn_ok",				m_pkOK );
    LINK_CONTROL( "static_text1",       m_pkStatic );
} 

KGCJinEventMsg::~KGCJinEventMsg(void)
{
}

void KGCJinEventMsg::OnCreate()
{
	m_pkOK->InitState( true, true, this );
	m_pkStatic->InitState( true, true, this );
	
	m_pkStatic->SetMultiLine( true );
	m_pkStatic->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_JIN_EVENT_MSG, "l"
		, g_kGlobalValue.m_kUserInfo.strNickName ) );
	m_pkStatic->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 0) );

	m_pkOK->SetHotKey( DIK_ESCAPE );
	m_pkOK->SetHotKey( DIK_RETURN );
}

void KGCJinEventMsg::OnCreateComplete( void )
{
	m_pkStatic->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_JIN_EVENT_MSG, "l"
		, g_kGlobalValue.m_kUserInfo.strNickName ) );
}
void KGCJinEventMsg::FrameMoveInEnabledState()
{
}

void KGCJinEventMsg::Initialize()
{
}

void KGCJinEventMsg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkOK,			KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
}

void KGCJinEventMsg::OnExit()
{
	//SiKGCValentineDayManager()->SetPollingNow(FALSE);
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

