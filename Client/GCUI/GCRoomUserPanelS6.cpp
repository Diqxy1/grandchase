#include "stdafx.h"
#include "KGCChannelManager.h"
#include "GCRoomUserPanelS6.h"
#include "KGCRoomSceneS6.h"
#include "GCRoomUserSlotPanelS6.h"

IMPLEMENT_CLASSNAME(KGCRoomUserPanelS6);
IMPLEMENT_WND_FACTORY(KGCRoomUserPanelS6);
IMPLEMENT_WND_FACTORY_NAME(KGCRoomUserPanelS6, "gc_roomuserpanel_s6");

KGCRoomUserPanelS6::KGCRoomUserPanelS6(void)
    : m_pkUserSlotPanel_4(NULL)
    , m_pkUserSlotPanel_6(NULL)
{
    LINK_CONTROL("user_slot_panel_4", m_pkUserSlotPanel_4);
    LINK_CONTROL("user_slot_panel_6", m_pkUserSlotPanel_6);
}

KGCRoomUserPanelS6::~KGCRoomUserPanelS6(void)
{
}

void KGCRoomUserPanelS6::ActionPerformed(const KActionEvent& event)
{


}

void KGCRoomUserPanelS6::OnCreate(void)
{
    m_pkUserSlotPanel_4->SetMaxUserSlot(OLD_MAX_DUNGEON_PLAYER_NUM);
    m_pkUserSlotPanel_6->SetMaxUserSlot(MAX_PLAYER_NUM);

    m_pkUserSlotPanel_4->InitState(false, true, this);
    m_pkUserSlotPanel_6->InitState(false, true, this);

    InitPrevValue();
}

void KGCRoomUserPanelS6::Destroy()
{
    m_pkUserSlotPanel_4->Destroy();
    m_pkUserSlotPanel_6->Destroy();

    InitPrevValue();
}

void KGCRoomUserPanelS6::FrameMoveInEnabledState(void)
{
    if (SiKGCChannelManager()->CheckChannel(CT_AUTOMATCH))
        return;


    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        SUserInfo& userInfo = g_MyD3D->MyPlayer[i]->m_kUserInfo;
        SCharInfo& charInfo = userInfo.GetCurrentChar();
        if (charInfo.iCharType != -1) {
            if (m_apkPrevCharType[i] != -1 && charInfo.iCharType == m_apkPrevCharType[i]) {
                if (charInfo.iLevel > m_apkPrevLevel[i])
                {
                    std::map<DWORD, int>::iterator mit = m_mapPlayerInfo.find(userInfo.dwUID);
                    if (mit != m_mapPlayerInfo.end())
                    {
                        m_vecLevelUpPlayer.push_back(mit->second);
                    }
                }
            }
            m_apkPrevCharType[i] = charInfo.iCharType;
            m_apkPrevLevel[i] = charInfo.iLevel;
        }
    }

    if (g_pkUIScene->IsEmptyWaitMessageBox() && false == g_pkUIScene->IsMsgBoxModal())
    {
        LevelUpEffectOnEnableFrameMove();
    }
}

void KGCRoomUserPanelS6::SetUserSlot()
{
    bool bMaxPlayerNum = true;

    if (!g_pkUIScene->m_pkRoomSceneS6->IsMaxPlayerNumSix())
        bMaxPlayerNum = false;

    m_pkUserSlotPanel_6->ToggleRender(bMaxPlayerNum);
    m_pkUserSlotPanel_4->ToggleRender(!bMaxPlayerNum);
}

KGCRoomUserSlotPanelS6* KGCRoomUserPanelS6::GetUserSlotPanel()
{
    if (g_pkUIScene->m_pkRoomSceneS6->IsMaxPlayerNumSix())
        return m_pkUserSlotPanel_6;
    else
        return m_pkUserSlotPanel_4;
}

void KGCRoomUserPanelS6::UpdateGameData( bool bUpdateExtraModel )
{
    m_mapPlayerInfo.clear();

    int m_nMyPlayerNum = 0;

    for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    {
        if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_kUserInfo.cCharIndex != -1)
        {
            m_mapPlayerInfo.insert(std::make_pair(g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID, i));
        }
    }
    GetUserSlotPanel()->UpdateGameData(bUpdateExtraModel);
}

void KGCRoomUserPanelS6::SetRoomMemberPingInfo(std::vector<std::pair<DWORD, DWORD>>& vecRoomMemberPingInfo_)
{
    GetUserSlotPanel()->SetRoomMemberPingInfo(m_mapPlayerInfo, vecRoomMemberPingInfo_);
}

void KGCRoomUserPanelS6::SetLevelUpPlayer(const int i_)
{
    std::map<DWORD, int>::iterator mit = m_mapPlayerInfo.find(g_MyD3D->MyPlayer[i_]->m_kUserInfo.dwUID);
    if (mit != m_mapPlayerInfo.end())
    {
        m_vecLevelUpPlayer.push_back(mit->second);
    }
}

void KGCRoomUserPanelS6::InitLevelupEffect()
{
    m_vecLevelUpPlayer.clear();
    InitPrevValue();
}



void KGCRoomUserPanelS6::LevelUpEffectOnEnableFrameMove()
{
#if defined( NEW_LEVELUP_EFFECT )

#if !defined( __PATH__ )
    if (g_pkInput->BtnUp(KInput::MBRIGHT)) {
        if (m_vecLevelUpPlayer.empty()) {

            m_vecLevelUpPlayer.push_back(0);
            m_vecLevelUpPlayer.push_back(1);
            m_vecLevelUpPlayer.push_back(2);
            m_vecLevelUpPlayer.push_back(3);
            m_vecLevelUpPlayer.push_back(4);
            m_vecLevelUpPlayer.push_back(5);
        }
    }
#endif

    for (size_t i = 0; i < m_vecLevelUpPlayer.size(); ++i) {
        const int levelUpPlayerIndex = m_vecLevelUpPlayer[i];
        if (!g_MyD3D->IsPlayerIndex(levelUpPlayerIndex))
            return;

        GetUserSlotPanel()->LevelUpEffect(levelUpPlayerIndex);
    }

    m_vecLevelUpPlayer.clear();
#endif
}

void KGCRoomUserPanelS6::InitPrevValue()
{
    for (size_t i = 0; i < m_apkPrevCharType.size(); ++i) {
        m_apkPrevCharType[i] = -1;
    }
    for (size_t i = 0; i < m_apkPrevLevel.size(); ++i) {
        m_apkPrevLevel[i] = -1;
    }
}

void KGCRoomUserPanelS6::ClearUserEnchant()
{
    m_pkUserSlotPanel_4->ClearUserEnchant();
    m_pkUserSlotPanel_6->ClearUserEnchant();
}