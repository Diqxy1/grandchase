#include "stdafx.h"
#include "GCRoomUserSlotPanelS6.h"

IMPLEMENT_CLASSNAME(KGCRoomUserSlotPanelS6);
IMPLEMENT_WND_FACTORY(KGCRoomUserSlotPanelS6);
IMPLEMENT_WND_FACTORY_NAME(KGCRoomUserSlotPanelS6, "gc_room_user_slot_panel_s6");

KGCRoomUserSlotPanelS6::KGCRoomUserSlotPanelS6(void)
    : m_nMaxUserSlot(MAX_PLAYER_NUM)
{
    char str[MAX_PATH] = { 0, };

    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        m_apkUserSlot[i] = NULL;

        sprintf(str, "user_slot%d", i);
        LINK_CONTROL(str, m_apkUserSlot[i]);

        m_apkUserMyBack[i] = NULL;
        sprintf(str, "myback%d", i);
        LINK_CONTROL(str, m_apkUserMyBack[i]);

    }
}

KGCRoomUserSlotPanelS6::~KGCRoomUserSlotPanelS6(void)
{
}

void KGCRoomUserSlotPanelS6::OnCreate(void)
{
    for (int i = 0; i < m_nMaxUserSlot; ++i)
    {
        if (m_apkUserSlot[i])
            m_apkUserSlot[i]->InitState(true, true, this);

        if (m_apkUserMyBack[i])
            m_apkUserMyBack[i]->InitState(false);
    }
}

void KGCRoomUserSlotPanelS6::Destroy()
{
    for (int i = 0; i < m_nMaxUserSlot - 1; ++i)
    {
        if (m_apkUserSlot[i])
            m_apkUserSlot[i]->Destroy();
    }

}

void KGCRoomUserSlotPanelS6::ActionPerformed(const KActionEvent& event)
{
    if (SiKGCChannelManager()->CheckChannel(CT_AUTOMATCH))
        return;

    for (int i = 0; i < m_nMaxUserSlot; ++i)
    {
        if (event.m_pWnd == m_apkUserSlot[i] && event.m_dwCode == D3DWE_RBUTTON_CLICK)
        {
            OnClickUserSlot(i);
        }
    }
}

void KGCRoomUserSlotPanelS6::OnClickUserSlot(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_nMaxUserSlot)
        return;

    if (g_MyD3D->MyPlayer[nIndex]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[nIndex]->m_kUserInfo.cCharIndex != -1)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_USER_INFO_POPUP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,
            nIndex, 0, true, true, true);
    }
}

void KGCRoomUserSlotPanelS6::UpdateGameData( bool bUpdateExtraMotion )
{
    for (int i = 0; i < m_nMaxUserSlot; ++i)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_kUserInfo.cCharIndex != -1)
        {
            m_apkUserSlot[i]->UpdatePlayerData(i, bUpdateExtraMotion);
            m_apkUserSlot[i]->SetExtraRoomObject(bUpdateExtraMotion);
        }
        else
        {
            m_apkUserSlot[i]->ClearPlayerData();
        }

        bool bMyPlayer = (g_MyD3D->Get_MyPlayer() == i);
        m_apkUserMyBack[i]->ToggleRender(bMyPlayer);
        m_apkUserSlot[i]->SetIndex(i);
        m_apkUserSlot[i]->UpdateSlot(SiKGCRoomManager()->GetRoomInfo().bOpenSlot[i]);
    }
}

void KGCRoomUserSlotPanelS6::SetRoomMemberPingInfo(std::map<DWORD, int>& mapPlayerInfo,
    std::vector<std::pair<DWORD, DWORD>>& vecRoomMemberPingInfo_)
{
    for (int i = 0; i < (int)vecRoomMemberPingInfo_.size(); i++)
    {
        std::map<DWORD, int>::iterator mit = mapPlayerInfo.find(vecRoomMemberPingInfo_[i].first);

        if (mit != mapPlayerInfo.end())
        {
            if (mit->second >= m_nMaxUserSlot)
                continue;

            m_apkUserSlot[mit->second]->SetPingState(vecRoomMemberPingInfo_[i].second);
        }
    }
}

void KGCRoomUserSlotPanelS6::LevelUpEffect(int levelUpPlayerIndex)
{
    if (levelUpPlayerIndex < 0 || levelUpPlayerIndex >= m_nMaxUserSlot)
        return;

    float fx1 = 320.0f * GC_SCREEN_DIV_WIDTH;
    float fy1 = 150.0f * GC_SCREEN_DIV_HEIGHT;

    if (!g_pkUIScene->m_pkRoomSceneS6->IsMaxPlayerNumSix())
    {
        fx1 = 275.0f * GC_SCREEN_DIV_WIDTH;
    }

    D3DXVECTOR2 vPos = m_apkUserSlot[levelUpPlayerIndex]->GetFixedWindowPos();
    g_ParticleManager->LevelUpEffect((vPos.x - fx1) / (400.0f * GC_SCREEN_DIV_WIDTH), (-vPos.y + fy1) / (405.0f * GC_SCREEN_DIV_WIDTH));
}


void KGCRoomUserSlotPanelS6::ClearUserEnchant()
{
    for (int i = 0; i < m_nMaxUserSlot; ++i)
    {
#ifdef ROOM_EFFECT
        m_apkUserSlot[i]->ClearRoomEffect();
#endif

#ifdef ENCHANT_EFFECT
        m_apkUserSlot[i]->ClearEnchantEffect();
#endif
    }
}