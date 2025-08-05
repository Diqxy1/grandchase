#include "stdafx.h"
//
#include "KGCContestHelp.h"
//
//
//
//

IMPLEMENT_CLASSNAME( KGCContestHelp );
IMPLEMENT_WND_FACTORY( KGCContestHelp );
IMPLEMENT_WND_FACTORY_NAME( KGCContestHelp, "gc_contest_help" );

KGCContestHelp::KGCContestHelp(void)
{
	LINK_CONTROL( "str_help", m_pkHelpStr );
	LINK_CONTROL( "str_title", m_pkTitleStr );
	LINK_CONTROL( "help_change_left", m_pkLeft );
	LINK_CONTROL( "help_change_right", m_pkRight );
}

KGCContestHelp::~KGCContestHelp(void)
{
}

void KGCContestHelp::OnCreate( void )
{	
	m_pkHelpStr->SetMultiLine( true );
	m_pkHelpStr->SetLineSpace( 1.2f );
	m_pkTitleStr->SetAlign( DT_CENTER );
	m_pkLeft->InitState( true, true, this );
	m_pkRight->InitState( true, true, this );

	lua_tinker::class_add<KGCContestHelp>( KGCLuabinder::getInstance().GetLuaState(), "KGCContestHelp" );
		lua_tinker::class_def<KGCContestHelp>( KGCLuabinder::getInstance().GetLuaState(), "AddPage",  &KGCContestHelp::AddPage );;

	lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "ContestHelp", this );

	// 스크립트로부터 페이지 세팅
    if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "Help.lua" ) == false )
	{
		MessageBoxA( NULL, "Help.lua Load Failed!", NULL, MB_OK );
		exit(0);
    }

    InitPage();
}

void KGCContestHelp::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkLeft )
		{
			--m_iCurrPage;
			if( m_iCurrPage < 0 )
				m_iCurrPage = (int)m_vecPage.size() - 1;

			SetPage( m_iCurrPage );
		}
		else if( event.m_pWnd == m_pkRight )
		{
			++m_iCurrPage;
			if( m_iCurrPage > (int)m_vecPage.size() - 1 )
				m_iCurrPage = 0;

			SetPage( m_iCurrPage );
		}
	}
}

void KGCContestHelp::OnInitialize()
{
	InitPage();
}

void KGCContestHelp::SetPage( int iPage )
{
	if( iPage > (int)m_vecPage.size()-1 || (int)m_vecPage.size() == 0 )
		return;

	PAGE& stPage = m_vecPage[iPage];

	m_pkTitleStr->SetText( g_pkStrLoader->GetString( stPage.m_iStrTitleID ) );
	m_pkHelpStr->SetTextAutoMultiline( g_pkStrLoader->GetString( stPage.m_iStrID ) );
}

void KGCContestHelp::AddPage( int iTitle, int iStr )
{
	PAGE stPage;
	stPage.m_iStrTitleID = iTitle;
	stPage.m_iStrID = iStr;

	m_vecPage.push_back( stPage );
}

void KGCContestHelp::InitPage()
{
    m_iCurrPage = 0;
    SetPage( m_iCurrPage );
}