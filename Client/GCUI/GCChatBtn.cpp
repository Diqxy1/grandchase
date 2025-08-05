#include "stdafx.h"
#include "GCChatBtn.h"
//

IMPLEMENT_CLASSNAME( KGCChatBtn );
IMPLEMENT_WND_FACTORY( KGCChatBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCChatBtn, "gc_chat_btn" );

KGCChatBtn::KGCChatBtn(void)
: m_dwUID( (DWORD)ULONG_MAX )
{
	m_pkNickName = NULL;
	m_pkAlert = NULL;

	LINK_CONTROL( "nickname_static", m_pkNickName );
	LINK_CONTROL( "alert", m_pkAlert );
}

KGCChatBtn::~KGCChatBtn(void)
{
}

void KGCChatBtn::ActionPerformed( const KActionEvent& event )
{
}

void KGCChatBtn::OnCreate( void )
{
	m_pkNickName->SetFontColor( 0xff000000 );
	m_pkNickName->SetText( L"" );

	m_pkAlert->InitState( false, true, NULL );
}

void KGCChatBtn::SetNickName( std::wstring& strNickName )
{
	m_pkNickName->SetText( strNickName );
}
