#include "StdAfx.h"
#include ".\tgdialogue.h"

#include "QuestGameManager.h"

#include "KGCGameBoard.h"


CTGDialogue::CTGDialogue(void)
{
    enType              = TG_DIALOGUE;
    ZeroMemory(m_szDialgue, sizeof(m_szDialgue));
    m_bOnOff            = false;
    m_iShowChar         = -1;
}

CTGDialogue::~CTGDialogue(void)
{
}

void CTGDialogue::Dialogue(void)
{
    bool bOnOff = m_bOnOff;
    if( bOnOff )
    {
        if( -1 != m_iShowChar )
        {
            int iLoop;
            for(iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
            {
                if( false == g_pGameBoard->m_pStartingMember[iLoop].IsLive )
                    continue;

                if( m_iShowChar == g_pGameBoard->m_pStartingMember[iLoop].m_iChar )
                    break;
            }
            if( iLoop == MAX_PLAYER_NUM )
                bOnOff = false;
        }
        //}

        //// 이미 드라마가 켜져있다면..
        //if( g_pkGameOverlayUI->IsShowDialogue() )
        //{
        //    // 현재 드라마가 On이면 이전 드라마 죽이고, 틀자.
        //    // Off면 걍 이전 드라마 재생되게 놔두자.
        //    if( bOnOff )
        //    {
        //        g_pkQuestManager->SetShowDialogue( false );

        //        KGC_PID_DIALOGUE_PROGRESS packet;
        //        packet.m_sTG_ID     = g_pkQuestManager->m_iDialogueTG_ID;
        //        packet.m_sIndex     = 0;
        //        packet.bShow        = false;
        //        g_kTGPacketPacker.Pack(&packet, sizeof(KGC_PID_DIALOGUE_PROGRESS));
        //    }
        //    else
        //        return;
        //}

        //if( bOnOff )
        //{
        g_pkQuestManager->SetDialogueList(m_szDialgue);
    }

    g_pkQuestManager->SetShowDialogue( bOnOff );
    g_pkQuestManager->m_iDialogueTG_ID = m_iTG_ID;

    KGC_PID_DIALOGUE_PROGRESS packet;
    packet.m_sTG_ID     = m_iTG_ID;
    packet.m_sIndex     = 0;
    packet.bShow        = bOnOff;
    g_kTGPacketPacker.Pack(&packet, sizeof(KGC_PID_DIALOGUE_PROGRESS));

    SetComplete(true);
}

void CTGDialogue::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        Dialogue();

        if( !m_bSendPacket )
        {
            // 패킷을 보낸다.
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        // Dialogue();      // 방장이 틀라고 띄워줄 것임
        SetComplete(true);
    }
}

HRESULT CTGDialogue::LoadItemFromLua(KLuaManager &luaMgr)
{
    luaMgr.GetValue("Dialogue", m_szDialgue, sizeof(m_szDialgue));
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE_DEF("SHOW_CHAR", m_iShowChar, -1);

    return S_OK;
}
