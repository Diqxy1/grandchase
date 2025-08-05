#include "stdafx.h"
#include "KGCHeroDungeonRewardDlg.h"
#include "GCUI\KGCHeroDungeonRewardItemS6.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonRewardDlg);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonRewardDlg);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonRewardDlg, "gc_herodungeonrewards6dlg");

KGCHeroDungeonRewardDlg::KGCHeroDungeonRewardDlg(void)
: IsRewardAvaliable(false)
{
    for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i) {
        m_apkItemBox[i] = NULL;
        char szBuf[MAX_PATH];
        sprintf_s(szBuf, MAX_PATH - 1, "itemslot%02d", i);
        LINK_CONTROL(szBuf, m_apkItemBox[i]);
    }

    m_pkClose = NULL;
    LINK_CONTROL("btn_close", m_pkClose);
    m_pkBtnOk = NULL;
    LINK_CONTROL("btn_ok", m_pkBtnOk);
    m_pkTitle = NULL;
    LINK_CONTROL("title", m_pkTitle);
    m_pkTitleInfo = NULL;
    LINK_CONTROL("title_info", m_pkTitleInfo);
    m_pkStaticDesc = NULL;
    LINK_CONTROL("static_desc", m_pkStaticDesc);
}

KGCHeroDungeonRewardDlg::~KGCHeroDungeonRewardDlg() {
}

void KGCHeroDungeonRewardDlg::OnDestroy(void) {
}

void KGCHeroDungeonRewardDlg::OnCreate(void) {

    for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i) {
        if (m_apkItemBox[i])
            m_apkItemBox[i]->InitState(false, true, this);
    }

    if (m_pkClose)
    {
        m_pkClose->InitState(true, true, this);
        m_pkClose->SetHotKey(DIK_ESCAPE);
    }

    if(m_pkBtnOk)
        m_pkBtnOk->InitState(false, true, this);
    if(m_pkTitle)
        m_pkTitle->InitState(false, false);
    if(m_pkTitleInfo)
        m_pkTitleInfo->InitState(true, false);

    if (m_pkStaticDesc) 
    {
        m_pkStaticDesc->SetFontColor(0xffffffff);
        m_pkStaticDesc->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
        m_pkStaticDesc->SetAlign(DT_LEFT);
        m_pkStaticDesc->InitState(true, false, this);
        m_pkStaticDesc->SetMultiLine(true);
        m_pkStaticDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_HERO_REWARD_DESC));
    }

    InitializeReawrdData();
    SetHeroItemBox();

    IsRewardAvaliable = false;
}

void KGCHeroDungeonRewardDlg::ResetSlots(void) 
{
    for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i) 
    {
        if(m_apkItemBox[i])
            m_apkItemBox[i]->ClearInfo();
    }
    m_pkSelectedSlot = -1;
}

void KGCHeroDungeonRewardDlg::OnCreateComplete(void)
{
    int nSelect = SiKGCWorldMapManager()->GetDungeonRewardNum(SiKGCWorldMapManager()->GetGameMode(), g_kGlobalValue.m_kUserInfo.mapStagePlayable);
    bool bOpen = nSelect > 0 ? true : false;

    IsRewardAvaliable = bOpen;

    UpdateRewardButton();
    SetSelectedItem();
}

void KGCHeroDungeonRewardDlg::FrameMoveInEnabledState(void) 
{
}

void KGCHeroDungeonRewardDlg::InitializeReawrdData(void) 
{
    IsRewardAvaliable = false;

    if (!m_mapItemInfo.empty())
        return;

    m_mapItemInfo.clear();

    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator ItemIter;
    std::map<INDEX_GCITEMID, KManufactureItem> _mapClearReward;
    g_kGlobalValue.m_kHeroDungeonInfo.GetClearReward(GC_GM_QUEST52, _mapClearReward);
    std::map<INDEX_GCITEMID, KManufactureItem>::iterator iterItem = _mapClearReward.begin();
    std::map<INDEX_GCITEMID, KManufactureItem>::iterator iterItem_end = _mapClearReward.end();
    for ( ; iterItem != iterItem_end; ++iterItem ) 
    {
        KHeroItemInfo tempInfo;
        tempInfo.m_pairIdIndex = iterItem->first;
        tempInfo.m_ItemInfo = iterItem->second;
        DWORD dwCharType = g_pItemMgr->GetItemData( static_cast<DWORD>( iterItem->first.second / 10 ) )->dwCharType;
        ItemIter = m_mapItemInfo.find( dwCharType );
        if ( m_mapItemInfo.end() == ItemIter ) 
        {
            std::vector<KHeroItemInfo> vecInfo;
            vecInfo.push_back( tempInfo );
            m_mapItemInfo.insert( std::make_pair( dwCharType, vecInfo ) );
        }
        else 
        {
            ItemIter->second.push_back( tempInfo );
        }
    }
}

void KGCHeroDungeonRewardDlg::SetHeroItemBox(void) {
    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
    DWORD m_dwCurrentCharacter = static_cast<DWORD>(pow(2.0f, iCharType));

    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator itemIter = m_mapItemInfo.find(m_dwCurrentCharacter);
    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator commonIter = m_mapItemInfo.find(ECT_ALL);

    if (m_mapItemInfo.end() == itemIter && m_mapItemInfo.end() == commonIter)
        return;

    int iCharItem = 0;
    int iCommonItem = 0;

    int iStartItemIndex = 1;
    --iStartItemIndex;
    iStartItemIndex *= NUM_HERO_REWARD_BOX;

    std::vector<KHeroItemInfo> tempVector;

    if (itemIter != m_mapItemInfo.end())
    {
        for (int i = 0; i < static_cast<int>(itemIter->second.size()); ++i)
            tempVector.push_back(itemIter->second[i]);

        iCharItem = static_cast<int>(itemIter->second.size());
    }

    if (commonIter != m_mapItemInfo.end())
    {
        for (int i = 0; i < static_cast<int>(commonIter->second.size()); ++i)
            tempVector.push_back(commonIter->second[i]);

        iCommonItem = static_cast<int>(commonIter->second.size());
    }

    int iTotalItem = iCharItem + iCommonItem;

    list<DWORD> ItemSize;

    for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i)
    {
        if (iStartItemIndex < iTotalItem)
        {
            if ((int)tempVector.size() <= iStartItemIndex)
                break;

            KHeroItemInfo tempHero = tempVector[iStartItemIndex++];

            GCItem* tempInfo = g_pItemMgr->GetItemData(tempHero.m_ItemInfo.m_ItemID / 10);

            if (tempInfo == NULL) {
                return;
            }

            ItemSize.push_back(tempInfo->dwGoodsID);

            for (int j = 0; j < (int)ItemSize.size(); j++) {
                if(m_apkItemBox[i])
                    m_apkItemBox[j]->ToggleRender(true);
            }

            if (m_apkItemBox[i])
            {
                m_apkItemBox[i]->SetGameMode(GC_GM_QUEST52);
                m_apkItemBox[i]->SetItemInfo(tempInfo->dwGoodsID);
                m_apkItemBox[i]->SetHeroItemInfo(tempHero);
            }
        }
    }
}

void KGCHeroDungeonRewardDlg::UpdateRewardButton()
{
    for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i) {
        if(m_apkItemBox[i])
            m_apkItemBox[i]->SelectSlot(false);
    }

    if(m_pkBtnOk)
        m_pkBtnOk->ToggleRender(IsRewardAvaliable);
    if(m_pkTitle)
        m_pkTitle->ToggleRender(IsRewardAvaliable);
    if(m_pkTitleInfo)
        m_pkTitleInfo->ToggleRender(!IsRewardAvaliable);
}

void KGCHeroDungeonRewardDlg::ActionPerformed(const KActionEvent& event) {

    for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i) {
        if (event.m_pWnd == m_apkItemBox[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
        {
            if (IsRewardAvaliable)
            {
                UpdateRewardButton();
                m_pkSelectedSlot = i;
                m_pkSelectedItem = m_apkItemBox[i]->m_kCurrentItem;
                m_IndexSelected = m_apkItemBox[i]->m_kCurrenHeroItem;
                if(m_apkItemBox[i])
                    m_apkItemBox[i]->SelectSlot(true);
            }
        }
    }

    if (event.m_pWnd == m_pkBtnOk && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        std::vector<INDEX_GCITEMID> vecIndexItemID;
        vecIndexItemID.push_back(m_IndexSelected.m_pairIdIndex);
        KP2P::GetInstance()->Send_DungeonClearRewardReq(GC_GM_QUEST52, vecIndexItemID);

        for (int i = 0; i < NUM_HERO_REWARD_BOX; ++i) {
            if (m_apkItemBox[i])
                m_apkItemBox[i]->ClearInfo();
        }

        IsRewardAvaliable = false;
    }

    GCWND_MSG_MAP(m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, [&]() {
        g_KDSound.Play("31");
        SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
        });
}

void KGCHeroDungeonRewardDlg::GetItemSelected(DWORD iItemID) 
{
}

void KGCHeroDungeonRewardDlg::SetSelectedItem() 
{
    if (IsRewardAvaliable != false) 
    {
        if(m_pkBtnOk)
            m_pkBtnOk->ToggleRender(true);
    }
    else 
    {
        if (m_pkBtnOk)
            m_pkBtnOk->ToggleRender(false);
    }
}