#include "stdafx.h"
#include "GCForgePageChange.h"
#include "GCForgeManager.h"

IMPLEMENT_CLASSNAME( KGCForgePageChange );
IMPLEMENT_WND_FACTORY( KGCForgePageChange );
IMPLEMENT_WND_FACTORY_NAME( KGCForgePageChange, "gc_forge_page" );

KGCForgePageChange::KGCForgePageChange( void )
: m_pkPrePageBtn(NULL)
, m_pkNextPageBtn(NULL)
, m_pkStaticPage(NULL)
{
    LINK_CONTROL("pre_page_btn",m_pkPrePageBtn);
    LINK_CONTROL("next_page_btn",m_pkNextPageBtn);
   // LINK_CONTROL("btn_left", m_pkPrePageBtn);
	//LINK_CONTROL("btn_right", m_pkNextPageBtn);
    LINK_CONTROL("static_current_page",m_pkStaticPage);
}

KGCForgePageChange::~KGCForgePageChange( void )
{
}

void KGCForgePageChange::ActionPerformed( const KActionEvent& event )
{

}

void KGCForgePageChange::OnCreate( void )
{
    m_pkPrePageBtn->InitState(true,true,this);
    m_pkNextPageBtn->InitState(true,true,this);
    m_pkStaticPage->InitState(true,false,NULL);
    m_pkStaticPage->SetAlign( DT_CENTER );

    m_pkPrePageBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgePageChange::OnClickPrePageBtn);
    m_pkNextPageBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCForgePageChange::OnClickNextPageBtn);

    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_FORGE_CHANGE_PAGE, this, &KGCForgePageChange::RefreshPageNum );

    SiGCForgeManager()->SetCurrentPage(0);
    SiGCForgeManager()->RefreshMaxPage();
    RefreshPageNum();
}

void KGCForgePageChange::OnClickPrePageBtn( const KActionEvent& event )
{
    int iCurrentPage = SiGCForgeManager()->GetCurrentPage();

    iCurrentPage--;
    if(iCurrentPage < 0){
        iCurrentPage = 0;
        return;
    }
    SetPage(iCurrentPage);
}
void KGCForgePageChange::OnClickNextPageBtn( const KActionEvent& event )
{
    int iCurrentPage = SiGCForgeManager()->GetCurrentPage();
    iCurrentPage++;
    if(iCurrentPage > SiGCForgeManager()->GetMaxPage()){
        iCurrentPage = SiGCForgeManager()->GetMaxPage();
        return;
    }
    SetPage(iCurrentPage);
}

void KGCForgePageChange::SetPage( int iIndex )
{
    SiGCForgeManager()->SetCurrentPage(iIndex);
    SpeakToActionListener( KActionEvent( NULL, KGCUIScene::D3DWE_FORGE_CHANGE_PAGE ));
}

void KGCForgePageChange::RefreshPageNum()
{
    m_pkStaticPage->SetText( g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_CARD_BOOK_CURRENT_PAGE, "ii", 
        SiGCForgeManager()->GetCurrentPage()+1, SiGCForgeManager()->GetMaxPage() + 1 ) );
}
