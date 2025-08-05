#include "stdafx.h"
#include "GCSphinxChallenge.h"

IMPLEMENT_CLASSNAME( KGCSphinxChallenge );
IMPLEMENT_WND_FACTORY( KGCSphinxChallenge );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxChallenge, "gc_sphinx_challeange" );

KGCSphinxChallenge::KGCSphinxChallenge( void )
: m_pkStaticSphinxMent(NULL)
, m_pkBtnChallenge(NULL)
{
    LINK_CONTROL("static_sphinx_ment",m_pkStaticSphinxMent);
    LINK_CONTROL("challenge_btn",m_pkBtnChallenge);
}

KGCSphinxChallenge::~KGCSphinxChallenge( void )
{
}

void KGCSphinxChallenge::ActionPerformed( const KActionEvent& event )
{

}

void KGCSphinxChallenge::OnCreate( void )
{
    m_pkStaticSphinxMent->InitState(true);
	m_pkStaticSphinxMent->SetMultiLine( true );
    m_pkStaticSphinxMent->SetText(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_1 ));
    m_pkStaticSphinxMent->SetAlign(DT_CENTER|DT_VCENTER);

    m_pkBtnChallenge->InitState(true,true,this);
    m_pkBtnChallenge->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSphinxChallenge::OnClickBtnOXQuizStart);
//    m_pkBtnChallenge->SetHotKey(DIK_RETURN);

}

void KGCSphinxChallenge::OnClickBtnOXQuizStart()
{
    if(g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_SPHINX_COIN ) == false)
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_12 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
    Result_SphinxReq = INT_MAX;
    KP2P::GetInstance()->Send_SphinxQuestionReq( KGCItemManager::ITEM_SPHINX_COIN * 10 ); 
    g_MyD3D->WaitForServerAck( Result_SphinxReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

