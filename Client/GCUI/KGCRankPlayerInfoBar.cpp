#include "stdafx.h"
#include "KGCRankPlayerInfoBar.h"

IMPLEMENT_CLASSNAME( KGCRankPlayerInfoBar );
IMPLEMENT_WND_FACTORY( KGCRankPlayerInfoBar );
IMPLEMENT_WND_FACTORY_NAME( KGCRankPlayerInfoBar, "gc_party_player_info_bar" );

KGCRankPlayerInfoBar::KGCRankPlayerInfoBar( void )
: m_pkNickName(NULL)
, m_pkRankGrade(NULL)
{
    LINK_CONTROL("static_nickname", m_pkNickName);
    LINK_CONTROL("static_rank_grade", m_pkRankGrade);
}

KGCRankPlayerInfoBar::~KGCRankPlayerInfoBar( void )
{
}

void KGCRankPlayerInfoBar::OnCreate()
{
    m_pkNickName->InitState(true);
    m_pkNickName->SetText(L"");
    m_pkRankGrade->InitState(true);
    m_pkRankGrade->SetAlign( DT_CENTER );
    m_pkRankGrade->SetText(L"");

}
void KGCRankPlayerInfoBar::ActionPerformed(const KActionEvent& event )
{

}

void KGCRankPlayerInfoBar::FrameMoveInEnabledState()
{

}


void KGCRankPlayerInfoBar::SetPlayerRankInfo(IN int iIndex , OUT bool& bRender)
{
    std::wstring wstrNickName;
    int iRankGrade;

    if ( g_pkQuestManager->GetPlayerRank(iIndex, wstrNickName, iRankGrade) ) {
        
        if ( wstrNickName.size() <= 0 ) {
            bRender = false;
            return;
        }

        std::wstring wstrRankGrade;

        m_pkNickName->SetText(wstrNickName);
        m_pkNickName->SetAlign( DT_LEFT );
        GCFUNC::GetRankGradeText( iRankGrade, wstrRankGrade);
        m_pkRankGrade->SetText( wstrRankGrade );
        bRender = true;
    }
    else {
        bRender = false;
    }
}

