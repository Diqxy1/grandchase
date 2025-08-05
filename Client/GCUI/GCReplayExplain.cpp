#include "stdafx.h"
#include "GCReplayExplain.h"
#include "GCUI/KGCFileFindDialog.h"

IMPLEMENT_CLASSNAME( KGCReplayExplain );
IMPLEMENT_WND_FACTORY( KGCReplayExplain );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayExplain, "gc_replay_record_explain" );

KGCReplayExplain::KGCReplayExplain( void )
{
    m_pkPrevBtn = NULL;
    m_pkNextBtn = NULL;

    LINK_CONTROL( "prev_btn", m_pkPrevBtn );
    LINK_CONTROL( "next_btn", m_pkNextBtn );

    for( int i=0 ; i<MAX_EXPLAIN ; ++i )
    {
        m_apkExplain[i] = NULL;
        
        char strExplain[100] = {0, };
        sprintf_s(strExplain, 99, "static_explain%d", i);

        LINK_CONTROL( strExplain, m_apkExplain[i] );
    }

    m_iPage = 0;
}

KGCReplayExplain::~KGCReplayExplain( void )
{

}

void KGCReplayExplain::OnCreate( void )
{
    m_pkPrevBtn->InitState(true, true, this);
    m_pkNextBtn->InitState(true, true, this);

    for( int i=0 ; i<MAX_EXPLAIN ; ++i )
    {
        m_apkExplain[i]->InitState(true, false, this);
        m_apkExplain[i]->SetMultiLine(true);
    }

    // 설명 글 세팅
    SetExplain();

    // 페이지 세팅
    m_iPage = 0;
    SetPage();
}

void KGCReplayExplain::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkPrevBtn,          KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevPage );
    GCWND_MSG_MAP( m_pkNextBtn,          KD3DWnd::D3DWE_BUTTON_CLICK, OnNextPage );
}

void KGCReplayExplain::SetExplain( void )
{
    std::wostringstream strmExplain;

#if defined( NATION_KOREA )
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_TITLE1);
#else 
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_TITLE1) << L"\n" << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_EXPLAIN1);
#endif
    m_apkExplain[0]->SetTextAutoMultiline( strmExplain.str() );

    strmExplain.str(L"");
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_TITLE2); 
    m_apkExplain[1]->SetTextAutoMultiline( strmExplain.str() );

    strmExplain.str(L"");
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_TITLE3); 
    m_apkExplain[2]->SetTextAutoMultiline( strmExplain.str() );
    
    strmExplain.str(L"");
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_EXPLAIN3); 
    m_apkExplain[3]->SetTextAutoMultiline( strmExplain.str() );

    strmExplain.str(L"");
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_EXPLAIN5); 
    m_apkExplain[4]->SetTextAutoMultiline( strmExplain.str() );

    strmExplain.str(L"");
    strmExplain << g_pkStrLoader->GetString(STR_ID_RECORD_EXPLAIN_EXPLAIN6); 
    m_apkExplain[5]->SetTextAutoMultiline( strmExplain.str() );
}

void KGCReplayExplain::OnPrevPage( void )
{
    --m_iPage;
    if( m_iPage < 0 )   m_iPage = 0;

    SetPage();
}

void KGCReplayExplain::OnNextPage( void )
{
    ++m_iPage;
    if( m_iPage > MAX_PAGE )    m_iPage = MAX_PAGE;

    SetPage();
}

void KGCReplayExplain::SetPage( void )
{
    if( 0 == m_iPage )
    {
        m_apkExplain[0]->ToggleRender(true);
        m_apkExplain[1]->ToggleRender(true);
        m_apkExplain[2]->ToggleRender(true);
        m_apkExplain[3]->ToggleRender(false);
        m_apkExplain[4]->ToggleRender(false);
        m_apkExplain[5]->ToggleRender(false);
    }
    else if ( MAX_PAGE == m_iPage )
    {
        m_apkExplain[0]->ToggleRender(false);
        m_apkExplain[1]->ToggleRender(false);
        m_apkExplain[2]->ToggleRender(false);
        m_apkExplain[3]->ToggleRender(true);
        m_apkExplain[4]->ToggleRender(true);
        m_apkExplain[5]->ToggleRender(true);
    }
}