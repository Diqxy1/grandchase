#include "stdafx.h"
#include "GCGuildStore.h"

IMPLEMENT_CLASSNAME( KGCGuildStore );
IMPLEMENT_WND_FACTORY( KGCGuildStore );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildStore, "gc_guildstore" );

KGCGuildStore::KGCGuildStore( void )
:m_pkBtnClose(NULL)
,m_pkStaticComment(NULL)
,m_iCurrentTabIndex(0)
{
    LINK_CONTROL("btn_close",m_pkBtnClose);
    LINK_CONTROL("comment",m_pkStaticComment);


    for (int i=0 ; i<EMAX_TAB_NUM ; i++)
    {
        m_pkBtnPage[i] = NULL;
        m_pkBtnPageover[i] = NULL;

        char szBuf[MAX_PATH];

        sprintf_s(szBuf, MAX_PATH - 1, "btn_page%d", i);
        LINK_CONTROL( szBuf, m_pkBtnPage[i] );

        sprintf_s(szBuf, MAX_PATH - 1, "btn_pageover%d", i);
        LINK_CONTROL( szBuf, m_pkBtnPageover[i] );

    }

    for (int i=0; i<EMAX_ITEM_BOX ; i++)
    {
        m_pkItemBox[i] = NULL;

        char szBuf[MAX_PATH];

        sprintf_s(szBuf, MAX_PATH - 1, "guild_item_box%d", i);
        LINK_CONTROL( szBuf, m_pkItemBox[i] );
    }

}

KGCGuildStore::~KGCGuildStore( void )
{
}


void KGCGuildStore::ActionPerformed(const KActionEvent &event)
{

    GCWND_MSG_MAP( m_pkBtnClose,KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

    
    if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        for (int i=0 ; i<EMAX_TAB_NUM ; i++)
        {
            if (event.m_pWnd == m_pkBtnPage[i])
            {
                if (i != m_iCurrentTabIndex){
                    SetTabToggle(i);
                    SetItemTab();
                    break;
                }               
            }
        }
    }
    
}
void KGCGuildStore::FrameMoveInEnabledState()
{

}

void KGCGuildStore::OnCreate()
{
    m_pkBtnClose->InitState(true,true,this);
    m_pkBtnClose->SetHotKey(DIK_ESCAPE);

    m_pkStaticComment->SetFontColor(0xffffffff);
    m_pkStaticComment->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticComment->SetAlign(DT_LEFT);
    m_pkStaticComment->InitState(true,true,this);
    m_pkStaticComment->SetText(L"");


 
    for (int i=0; i<EMAX_TAB_NUM ; i++)
    {
        m_pkBtnPage[i]->InitState(true,true,this);
        m_pkBtnPageover[i]->InitState(false,true,this);
    }

    for (int i=0; i<EMAX_ITEM_BOX ; i++)
    {
        m_pkItemBox[i]->InitState(true,true,this);
    }

}

void KGCGuildStore::OnCreateComplete()
{
    

    SendGuildStoreCatalogReq();

    SetItemTab();

    m_pkBtnPageover[m_iCurrentTabIndex]->ToggleRender(true);

    std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_GUILD_GUILDSTORE_COMMENT );
    if (strMsg.empty())
        strMsg = L" ";
    m_pkStaticComment->SetTextAutoMultiline(strMsg);

}

void KGCGuildStore::OnClose()
{
    g_KDSound.Play( "31" );
    m_iCurrentTabIndex = 0;

    for (int i=0 ; i<EMAX_TAB_NUM ; i++)
    {
        m_pkBtnPageover[i]->ToggleRender(false);
    }

    for (int i=0; i<EMAX_ITEM_BOX ; i++)
    {
        m_pkItemBox[i]->ToggleDisPlay(false);
    }

    m_mapItemList.clear();
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCGuildStore::SendGuildStoreCatalogReq()
{
    KP2P::GetInstance()->Send_GuildStoreCatalogReq();

    Result_ReqGuildStoreCatalog = INT_MAX;
    g_MyD3D->WaitForServerAck(Result_ReqGuildStoreCatalog, INT_MAX, 3000, TIME_OUT_VALUE);
}


void KGCGuildStore::SetItemTab()
{    

    for (int i=0 ; i<EMAX_ITEM_BOX ; i++)
    {
        m_pkItemBox[i]->ToggleDisPlay(false);
        m_pkItemBox[i]->ClearBox();
    }

    std::map<int, std::vector<KManufactureItem>>::iterator  iterItemList;

    for (iterItemList=m_mapItemList.begin() ; iterItemList != m_mapItemList.end() ; ++iterItemList)
    {
        //현재 설정되있는 탭이 에 해당되는아이템인지를 체크
        if (iterItemList->first == m_iCurrentTabIndex)
        {
            std::vector<KManufactureItem>::iterator iterKmanu;

            //진열 리스트보다 적게 왔을 경우를 대비 인덱스 값과 이터레이터를 따로 돌림
            int i=0;
            for (iterKmanu = iterItemList->second.begin() ; iterKmanu!= iterItemList->second.end() ; ++iterKmanu)
            {
                //진열리스트가 모두 차 있을경우 종료
                if (i>= EMAX_ITEM_BOX)
                    return;

                //m_pkItemBox[i]->m_iBuyIndex = iterKmanu->m_Index;
                //iterKmanu->m_Index = i;
                
                m_pkItemBox[i]->SetItemBox(&iterItemList->second[i],m_iCurrentTabIndex);
                i++;
            }

            for(int i=0 ; i<EMAX_ITEM_BOX ; i++)
            {
                if (!m_pkItemBox[i]->IsDisPlay())
                    continue;

                GCItem* pItem = g_MyD3D->m_kItemMgr.GetItemData( m_pkItemBox[i]->GetManufactureItem()->m_ItemID/10 );

                m_pkItemBox[i]->Lock(true);
                m_pkItemBox[i]->SetToolTip(pItem->strItemName);
                m_pkItemBox[i]->Lock(false);
            }    
            m_pkBtnPageover[m_iCurrentTabIndex]->ToggleRender(true);

            return;         
        }       
    }
}

void KGCGuildStore::SetItemList(std::map<int, std::vector<KManufactureItem>> &mapItemList_)
{
    if (!m_mapItemList.empty())
        return;

    m_mapItemList.clear();
    m_mapItemList = mapItemList_;
}

void KGCGuildStore::SetTabToggle(int iIndex_)
{
    for (int i=0 ; i<EMAX_TAB_NUM ; i++)
    {
        m_pkBtnPageover[i]->ToggleRender(false);
    }
    m_pkBtnPageover[iIndex_]->ToggleRender(true);

    m_iCurrentTabIndex = iIndex_;
}
