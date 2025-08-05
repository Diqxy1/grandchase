#include "stdafx.h"
#include "KGCGuildJoinReqMbox.h"

IMPLEMENT_CLASSNAME( KGCGuildJoinReqMbox );
IMPLEMENT_WND_FACTORY( KGCGuildJoinReqMbox );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildJoinReqMbox, "gc_guild_join_req_mbox" );

KGCGuildJoinReqMbox::KGCGuildJoinReqMbox(void)
: m_pkConfirmBtn( NULL )
, m_pkCancelBtn( NULL )
, m_pkStaticJoinerNickname( NULL )
, m_pkStaticJoinerCareer( NULL )
, m_pkEditGuildJoinIntroduce( NULL )
, m_pkGradeMark( NULL )
, m_iSelectedGuildUID(0)
{
	LINK_CONTROL( "confirm_btn",				m_pkConfirmBtn );
	LINK_CONTROL( "cancel_btn",					m_pkCancelBtn );
	LINK_CONTROL( "guild_join_req_id_static",	m_pkStaticJoinerNickname );
	LINK_CONTROL( "guild_join_req_info_static",	m_pkStaticJoinerCareer );
	LINK_CONTROL( "guild_join_introduce_edit ",	m_pkEditGuildJoinIntroduce );
	LINK_CONTROL( "grade_mark",					m_pkGradeMark );
}

KGCGuildJoinReqMbox::~KGCGuildJoinReqMbox(void)
{
}

void KGCGuildJoinReqMbox::OnCreate()
{
	m_pkConfirmBtn->InitState( true, true, this );
	m_pkCancelBtn->InitState( true, true, this );

	m_pkStaticJoinerNickname->InitState( true, true, this );
	m_pkStaticJoinerNickname->SetAlign(DT_LEFT);
	m_pkStaticJoinerCareer->InitState( true, true, this );
	m_pkStaticJoinerCareer->SetAlign(DT_LEFT);
	m_pkStaticJoinerCareer->SetLineSpace(1.2f);
	m_pkEditGuildJoinIntroduce->InitState( true, false, this );
	m_pkEditGuildJoinIntroduce->SetText(L"");
	m_pkEditGuildJoinIntroduce->SetFocus();
	m_pkEditGuildJoinIntroduce->SetLimitText(MAX_EDIT_NUM);

	//m_pkGradeMark->InitState( true );
	m_pkGradeMark->InitState( false );
}

void KGCGuildJoinReqMbox::ActionPerformed( const KActionEvent& event )
{
	//	버튼 클릭 이벤트
	if( event.m_pWnd ==	m_pkConfirmBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		KP2P::GetInstance()->Send_JoinGuildReq(m_iSelectedGuildUID, m_pkEditGuildJoinIntroduce->GetText());
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}

	if( event.m_pWnd ==	m_pkCancelBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );

}

void KGCGuildJoinReqMbox::OnCreateComplete()
{
}

void KGCGuildJoinReqMbox::SetJoinerInfo(int iGuildUID_)
{
	// 캐릭터별이 아닌 유저의 통산전적을 계산한다.
	int iRate = 0;
	if( ( g_kGlobalValue.m_kUserInfo.GetUserWin() + g_kGlobalValue.m_kUserInfo.GetUserLose() ) > 0 )
	{
		iRate = ( g_kGlobalValue.m_kUserInfo.GetUserWin() * 100 )
			/ ( g_kGlobalValue.m_kUserInfo.GetUserWin() + g_kGlobalValue.m_kUserInfo.GetUserLose() );
	}

	m_pkStaticJoinerCareer->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_NEW_GUILD_JOINER_CAREER, "iii", g_kGlobalValue.m_kUserInfo.GetUserWin(),
		g_kGlobalValue.m_kUserInfo.GetUserLose(), iRate));
	m_pkStaticJoinerNickname->SetText( L"#c" + g_kGlobalValue.m_kUserInfo.strNickColor + g_kGlobalValue.m_kUserInfo.strNickName + L"#cX");
	//m_pkGradeMark->SetGrade((int)g_kGlobalValue.m_kUserInfo.GetUserGrade());
	m_iSelectedGuildUID = iGuildUID_;
}

