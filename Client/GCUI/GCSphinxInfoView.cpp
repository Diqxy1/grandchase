#include "stdafx.h"
#include "GCSphinxInfoView.h"

IMPLEMENT_CLASSNAME( KGCSphinxInfoView );
IMPLEMENT_WND_FACTORY( KGCSphinxInfoView );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxInfoView, "gc_sphinx_info_view" );


//KD3DWnd         *m_pkBuyBtn;
//KD3DWnd         *m_pkRewardItemView;
//KD3DStatic      *m_pkStaticCoin;
//KD3DStatic      *m_pkStaticCoinNum;
//KD3DStatic      *m_pkStaticDifficulty;
//KD3DStatic      *m_pkStaticDifficultyNum;
//KD3DStatic      *m_pkStaticTotalAnswer;
//KD3DStatic      *m_pkStaticTotalAnswerNum;

KGCSphinxInfoView::KGCSphinxInfoView( void )
: m_pkBuyBtn(NULL)
, m_pkRewardItemViewBtn(NULL)
{
    LINK_CONTROL("buy_btn",m_pkBuyBtn);
    LINK_CONTROL("item_view_btn",m_pkRewardItemViewBtn);

    for(int i = 0; i < SISE_MAX ; i ++)
    {
        m_vecStaticSphinxInfo.push_back(NULL);
    }
    LINK_CONTROL("static_coin",m_vecStaticSphinxInfo[SISE_COIN]);
    LINK_CONTROL("static_coin_num",m_vecStaticSphinxInfo[SISE_COIN_NUM]);
    LINK_CONTROL("static_difficulty",m_vecStaticSphinxInfo[SISE_DIFFICULTY]);
    LINK_CONTROL("static_difficulty_num",m_vecStaticSphinxInfo[SISE_DIFFICULTY_NUM]);
    LINK_CONTROL("static_total_answer",m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER]);
    LINK_CONTROL("static_total_answer_num",m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER_NUM]);
}

KGCSphinxInfoView::~KGCSphinxInfoView( void )
{
}

void KGCSphinxInfoView::ActionPerformed( const KActionEvent& event )
{
    if(event.m_pWnd == m_pkRewardItemViewBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnChangeStateResultItemView();
    }
    else if(event.m_pWnd == m_pkBuyBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        OnBuyBtn();
    }
}


void KGCSphinxInfoView::OnCreate( void )
{
    m_pkBuyBtn->InitState(true,true,this);
    m_pkRewardItemViewBtn->InitState(true,true,this);

    for(std::vector<KD3DStatic*>::iterator vit = m_vecStaticSphinxInfo.begin();vit != m_vecStaticSphinxInfo.end();vit++)
    {
        (*vit)->InitState(true,false,NULL);
        (*vit)->SetAlign(DT_CENTER);
    }
    
    m_vecStaticSphinxInfo[SISE_COIN]->SetText(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_5 ));
    m_vecStaticSphinxInfo[SISE_COIN]->SetFontColor( D3DCOLOR_ARGB(255,212,209,172) );
    m_vecStaticSphinxInfo[SISE_COIN]->SetFontOutline( true, D3DCOLOR_ARGB(255,24,34,48) );

    m_vecStaticSphinxInfo[SISE_DIFFICULTY]->SetText(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_6 ));
    m_vecStaticSphinxInfo[SISE_DIFFICULTY]->SetFontColor( D3DCOLOR_ARGB(255,175,211,228) );
    m_vecStaticSphinxInfo[SISE_DIFFICULTY]->SetFontOutline( true, D3DCOLOR_ARGB(255,14,24,33) );
    m_vecStaticSphinxInfo[SISE_DIFFICULTY]->SetFontSpace( 1.0f );

    m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER]->SetText(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_7 ));
    m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER]->SetFontColor( D3DCOLOR_ARGB(255,175,211,228) );
    m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER]->SetFontOutline( true, D3DCOLOR_ARGB(255,14,24,33) );
    m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER]->SetFontSpace( 1.0f );

}

void KGCSphinxInfoView::SetTotalAnswer( int iTotalAnswer )
{
    m_vecStaticSphinxInfo[SISE_TOTAL_ANSWER_NUM]->SetText(iTotalAnswer);
}

void KGCSphinxInfoView::SetDiffculty( int iDifficult )
{
    if(iDifficult == -1)
    {
        m_vecStaticSphinxInfo[SISE_DIFFICULTY_NUM]->SetText(L"");
        return;
    }

    std::wstring wstr = L"";
    for(int i = 0; i < iDifficult; i++)
    {
        wstr += L"★";
    }

    m_vecStaticSphinxInfo[SISE_DIFFICULTY_NUM]->SetText(wstr);
}

void KGCSphinxInfoView::UpdateCoin()
{
    int nCoin = g_pItemMgr->GetInventoryItemDuration(KGCItemManager::ITEM_SPHINX_COIN);
    if(nCoin == 0)
        m_vecStaticSphinxInfo[SISE_COIN_NUM]->SetText(L"0");
    else
        m_vecStaticSphinxInfo[SISE_COIN_NUM]->SetText(nCoin);

}

void KGCSphinxInfoView::OnChangeStateResultItemView()
{
    g_pkUIScene->m_pkSphinxDlg->SetResultItemList();
    g_pkUIScene->m_pkSphinxDlg->SetSphinxState(2);

}

void KGCSphinxInfoView::OnBuyBtn()
{
    g_pkUIScene->m_pkSphinxDlg->ClearPaticle();

    if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CANT_BUY_IN_ROOM), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    if( false == g_pItemMgr->CheckExtraInventoryExist() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    DWORD dwBuyItemID = KGCItemManager::ITEM_SPHINX_COIN;

    GCItem* pItem = g_pItemMgr->GetItemData( dwBuyItemID );

    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( dwBuyItemID );
#else
    #ifdef GAME_CASH
            g_pkUIScene->EnableBuyCashItemWnd( dwBuyItemID, false, true );
    #else
            g_MyD3D->m_kItemMgr.BuyItem( dwBuyItemID, pItem->iItemTypeValue  );
    #endif
#endif
    }
    else
    {

        std::wostringstream strmText1;
        std::wostringstream strmText2;

#if defined ( _PORTUGUESE ) || defined( _SPANISH )
        strmText1 << pItem->strItemName;
        strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", (int)pItem->dwPrice );
#elif defined( _ENGLISH ) || defined (NATION_THAILAND)
        strmText1<<g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", pItem->strItemName, (int)pItem->dwPrice );
#else
        strmText1 << pItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
        strmText2 << (int)pItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
            strmText1.str(), strmText2.str(),
            KGCUIScene::GC_MBOX_USE_BUY, dwBuyItemID, pItem->iItemTypeValue, false, true );
    }
}