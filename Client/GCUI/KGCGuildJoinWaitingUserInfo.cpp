#include "StdAfx.h"
#include "KGCGuildJoinWaitingUserInfo.h"

IMPLEMENT_CLASSNAME( KGCGuildJoinWaitingUserInfo );
IMPLEMENT_WND_FACTORY( KGCGuildJoinWaitingUserInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildJoinWaitingUserInfo, "gc_guild_join_waiting_user_info" );

KGCGuildJoinWaitingUserInfo::KGCGuildJoinWaitingUserInfo(void)
:m_pkGradeMark(NULL)
,m_pkStaticNIckname(NULL)
,m_pkStatintroduce(NULL)
,m_pkStaticApplicationDate(NULL)
,m_pkSelectList(NULL)
{
	LINK_CONTROL("grade_mark",					m_pkGradeMark );    
	LINK_CONTROL("bg_list_select",				m_pkSelectList );
    LINK_CONTROL("static_nickname",				m_pkStaticNIckname );
	LINK_CONTROL("static_introduce",			m_pkStatintroduce );
	LINK_CONTROL("static_application_date",		m_pkStaticApplicationDate );
}

KGCGuildJoinWaitingUserInfo::~KGCGuildJoinWaitingUserInfo(void)
{
}


void KGCGuildJoinWaitingUserInfo::OnCreate( void )
{   
	//m_pkGradeMark->InitState(true);
	m_pkGradeMark->InitState(false);
	m_pkSelectList->InitState(false);
	m_pkStaticNIckname->InitState(true);
	m_pkStaticNIckname->SetAlign(DT_LEFT);
	m_pkStatintroduce->InitState(true);
	m_pkStatintroduce->SetAlign(DT_LEFT);
    m_pkStatintroduce->SetWordWrap(true);

	m_pkStaticApplicationDate->InitState(true);
	m_pkStaticApplicationDate->SetAlign(DT_LEFT);
}

void KGCGuildJoinWaitingUserInfo::OnCreateComplete( void )
{
}

void KGCGuildJoinWaitingUserInfo::ActionPerformed( const KActionEvent& event )
{
}

void KGCGuildJoinWaitingUserInfo::OnDestroyComplete( void )
{
}

void KGCGuildJoinWaitingUserInfo::FrameMoveInEnabledState( void )
{

}

void KGCGuildJoinWaitingUserInfo::PostChildDraw( void )
{

}

void KGCGuildJoinWaitingUserInfo::SetJoinWaitingUserInfo( KNGuildUserInfo& sinfo_ )
{
	WCHAR strText[20];
	swprintf(strText, 20, L"%d.%d.%d", (int)sinfo_.m_kLastLoginDate.m_sYear, sinfo_.m_kLastLoginDate.m_cMonth, sinfo_.m_kLastLoginDate.m_cDay);	
	m_pkStaticApplicationDate->SetText(strText);

	//m_pkGradeMark->SetGrade(sinfo_.m_ucCharacterGrade);
	std::wstring strNickName = sinfo_.m_strNickName;
	GCUTIL_STR::Trim( strNickName );
	m_pkStaticNIckname->SetText(strNickName);

	CString src = sinfo_.m_strMyComment.c_str();
	src.Replace(L"\n", L" ");
	std::wstring strConvertor(src);
	m_pkStatintroduce->SetForceWordWrapText(strConvertor, true);
	
}

void KGCGuildJoinWaitingUserInfo::SetSelectCursorBox( bool bRender_ )
{
	m_pkSelectList->ToggleRender(bRender_);
}

bool KGCGuildJoinWaitingUserInfo::IsSelectListBoxRender()
{
	return m_pkSelectList->IsRenderOn();
}
