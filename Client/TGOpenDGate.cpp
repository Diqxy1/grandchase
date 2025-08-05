#include "StdAfx.h"
#include ".\tgopendgate.h"

#include "QuestGameManager.h"


CTGOpenDGate::CTGOpenDGate(void)
{
    enType          = TG_OPEN_DGATE;
}

CTGOpenDGate::~CTGOpenDGate(void)
{
}

void CTGOpenDGate::OpenGate(void)
{
    g_pkQuestManager->MoveGate(m_iDirection);

    SetComplete( true );
}

void CTGOpenDGate::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        OpenGate();

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
        OpenGate();
    }
}

HRESULT CTGOpenDGate::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Direction", m_iDirection, return E_FAIL);

    return S_OK;
}