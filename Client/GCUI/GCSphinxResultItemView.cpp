#include "stdafx.h"
#include "GCSphinxResultItemView.h"

#include "GCSphinxResultItemBox.h"


IMPLEMENT_CLASSNAME( KGCSphinxResultItemView );
IMPLEMENT_WND_FACTORY( KGCSphinxResultItemView );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxResultItemView, "gc_sphinx_view" );

KGCSphinxResultItemView::KGCSphinxResultItemView( void )
: m_pkStatic(NULL)
, m_pkScrollbar(NULL)
, m_pkOKBtn(NULL)
, m_pkItemBox(NULL)
, m_pkClipRange(NULL)
, m_iScrollPos(0)
, m_iTopIndex(0)
, m_iViewFirstIndex(-1)
{
    LINK_CONTROL("static_reward",m_pkStatic);
    LINK_CONTROL("scroll",m_pkScrollbar);
    LINK_CONTROL("ok_btn",m_pkOKBtn);
    LINK_CONTROL("item_box",m_pkItemBox);
    LINK_CONTROL("cliprange",m_pkClipRange);

    char buf[256];
    for(int i= 0;i < 8;i++)
    {
        memset(buf,0,256);
        m_arrBackBox[i] = NULL;
        itoa(i,buf,10);
        std::string wndname = "back";
        wndname += buf;
        LINK_CONTROL(wndname.c_str(),m_arrBackBox[i]);
    }

}

KGCSphinxResultItemView::~KGCSphinxResultItemView( void )
{
}

void KGCSphinxResultItemView::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkScrollbar )
    {
        int iTop = m_iTopIndex;
        switch( event.m_dwCode )
        {
        case 0:
        case D3DWE_SCROLLBAR_DOWNBTN:
        case D3DWE_SCROLLBAR_UPBTN:
        case D3DWE_SCROLLBAR_DRAG:
            m_iTopIndex = m_pkScrollbar->GetScrollPos();
            break;
        }

        if( iTop != m_iTopIndex )
        {
            UpdateItemList();
            g_KDSound.Play( "73" );
        }
        return;
    }
}

void KGCSphinxResultItemView::OnCreate( void )
{
    m_pkOKBtn->InitState(true,true,this);
    m_pkItemBox->InitState(false,false,NULL);

    m_pkScrollbar->InitState(true,true,this);
    m_pkScrollbar->SetScrollPageSize( 2 );
    m_pkScrollbar->SetScrollPos( m_iTopIndex );

    m_pkScrollbar->SetScrollGap( 1 );

    m_pkStatic->InitState(true,false,NULL);
	m_pkStatic->SetMultiLine( true );
    m_pkStatic->SetAlign(DT_CENTER);
    m_pkStatic->SetTextAutoMultiline(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_9 ));


    m_pkOKBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSphinxResultItemView::OnChangeStateChallenge);

}

void KGCSphinxResultItemView::SetRewardItem( KEVENT_SPHINX_ITEM_LIST_NOT mapRewardItem )
{
    m_vecRewardItemList.clear();

    for(KEVENT_SPHINX_ITEM_LIST_NOT::iterator mIter = mapRewardItem.begin() ; mIter != mapRewardItem.end() ; mIter++)
    {
        for(std::set<int>::iterator sIter = mIter->second.begin(); sIter != mIter->second.end(); sIter++)
        {
            m_vecRewardItemList.push_back(std::pair<int,int>(mIter->first, *sIter));
        }
    }
}

void KGCSphinxResultItemView::SetRewardItem()
{
    ClearItemList();
    std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRange, m_pkItemBox->GetFileName().c_str(), "",m_pkItemBox->GetPath().c_str(),true,m_vecRewardItemList.size() );

    std::vector<KD3DWnd*>::iterator vIterWnd = vecTemp.begin();

    for(std::vector<std::pair<int,int>>::iterator vIter = m_vecRewardItemList.begin() ; vIter != m_vecRewardItemList.end() ; vIter++, vIterWnd++)
    {
        if(vIterWnd == vecTemp.end()) break;
        KGCSphinxResultItemBox* pWnd = static_cast<KGCSphinxResultItemBox*>(*vIterWnd);
        pWnd->SetItem(vIter->second, vIter->first);
        m_vecResultItemImageList.push_back(pWnd);
    }

    m_pkScrollbar->SetScrollRangeMax( static_cast<int>(m_vecResultItemImageList.size()/4)+1);
    if(static_cast<int>(m_vecResultItemImageList.size()/4) > 2)
        m_pkScrollbar->SetScrollPageSize( 2 );
    else
        m_pkScrollbar->SetScrollPageSize( static_cast<int>(m_vecResultItemImageList.size()/4) );

    m_iTopIndex = 0;
    m_pkScrollbar->SetScrollPos( m_iTopIndex );

    SetViewItem();


}



void KGCSphinxResultItemView::ClearItemList()
{
    if(m_vecResultItemImageList.empty()) return;

    for(std::vector<KGCSphinxResultItemBox*>::iterator vIter = m_vecResultItemImageList.begin(); vIter != m_vecResultItemImageList.end();vIter++)
    {
        (*vIter)->ClearImage();
    }
    m_vecResultItemImageList.clear();
    m_pkClipRange->RemoveAllChild();

}

void KGCSphinxResultItemView::FrameMoveInEnabledState()
{
    // 마우스 휠 돌릴때 스크롤 처리
    if ( 0 != g_pkInput->GetZMov() )
    {
        if( g_pkInput->GetZMov() > 0 )
        {
            if( --m_iTopIndex < 0 )
                ++m_iTopIndex;
            else
                g_KDSound.Play( "73" );

            UpdateItemList();
        }
        else 
        {
            if( ++m_iTopIndex > m_pkScrollbar->GetScrollRangeMax() )
                --m_iTopIndex;
            else
                g_KDSound.Play( "73" );

            UpdateItemList();
        }
    }
}

void KGCSphinxResultItemView::UpdateItemList()
{
    if(m_iTopIndex < 0)
    {
        m_iTopIndex = 0;
        m_pkScrollbar->SetScrollPos( m_iTopIndex );
        return;
    }
    if(static_cast<int>(m_vecResultItemImageList.size()/4) < 2)
    {
        m_iTopIndex = 0;
        m_pkScrollbar->SetScrollPos( m_iTopIndex );
        return;
    }
    if(static_cast<int>(m_vecResultItemImageList.size()/4) - m_iTopIndex < 1)
    {
        m_iTopIndex = static_cast<int>(m_vecResultItemImageList.size()/4) - 1;
        if(m_iTopIndex <0)
            m_iTopIndex = 0;
        UpdateItemList();
        return;
    }

    SetViewItem();

    m_pkScrollbar->SetScrollPos( m_iTopIndex );

}

void KGCSphinxResultItemView::SetViewItem()
{
    m_iViewFirstIndex = -1;
    for(int i = 0; i < static_cast<int>(m_vecResultItemImageList.size()) ; i++)
    {
        if( i < m_iTopIndex * 4 || i >= m_iTopIndex * 4 + 8)
        {
            m_vecResultItemImageList[i]->ToggleRender(false);
            continue;
        }
        if(m_iViewFirstIndex == -1) m_iViewFirstIndex = i;
        float posX = static_cast<float>(i % 4 * 59);
        float posY = static_cast<float>((i / 4 - m_iTopIndex) * 59);
        m_vecResultItemImageList[i]->ToggleRender(true);
        m_vecResultItemImageList[i]->SetWindowPos(D3DXVECTOR2(posX,posY));
        m_vecResultItemImageList[i]->SetItemImagePos();
    }
    if(m_iViewFirstIndex == -1) m_iViewFirstIndex =0;
    
    int backOnSize = static_cast<int>(m_vecResultItemImageList.size()) - (m_iTopIndex * 4);

    if(backOnSize >=8)
    {
        backOnSize = 8;
    }

    for(int i = 0 ; i< 8 ; i++)
    {
        if( i < backOnSize )
        {
            m_arrBackBox[i]->ToggleRender(true);
        }
        else
        {
            m_arrBackBox[i]->ToggleRender(false);
        }
    }

}

void KGCSphinxResultItemView::OnChangeStateChallenge()
{
    ClearItemList();
    g_pkUIScene->m_pkSphinxDlg->SetSphinxState(0);

}

void KGCSphinxResultItemView::ItemInfoDraw()
{
    D3DXVECTOR2	pos;
    POINT pt = g_pkInput->GetMousePos();
    GCItem* pItem = NULL;

    for(int i = 0; i < 8; i++)
    {
        pos = m_arrBackBox[i]->GetRelocatedWindowPos();
        if(pos.x <pt.x && pos.x +57.f >pt.x && pos.y < pt.y && pos.y+57.f>pt.y)
        {
            if(static_cast<int>(m_vecResultItemImageList.size()) <= m_iViewFirstIndex+i )
                break;

            pItem = g_pItemMgr->GetItemData(m_vecResultItemImageList[m_iViewFirstIndex+i]->GetItemID());
            pos.x += 70.f;
            break;
        }
   }

    if( pItem != NULL )
    {
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( pos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}


void KGCSphinxResultItemView::PostChildDraw()
{
    ItemInfoDraw();
}