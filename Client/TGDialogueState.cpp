#include "StdAfx.h"
#include ".\tgdialoguestate.h"
#include "gcui/GCGameOverlayUI.h"

CTGDialogueState::CTGDialogueState(void)
{
    m_bOnOff          = true;
    enType            = TG_DIALOGUE_STATE;
}

CTGDialogueState::~CTGDialogueState(void)
{
}

void CTGDialogueState::SetOnOff( bool bOnOff )
{
    m_bOnOff = bOnOff;
}

void CTGDialogueState::OnOff(void)
{
    if( g_pkGameOverlayUI->IsShowDialogue() == m_bOnOff )
        SetComplete( true );
}

void CTGDialogueState::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        OnOff();

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
        OnOff();
    }
}

HRESULT CTGDialogueState::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE_DEF("OnOff", m_bOnOff, true);

    return S_OK;
}