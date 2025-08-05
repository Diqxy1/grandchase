#include "StdAfx.h"
#include ".\TGSuperPlayer.h"
//

#include "MyD3D.h"


CTGSuperPlayer::CTGSuperPlayer(void)
{
    enType              = TG_SUPERPLAYER;
}

CTGSuperPlayer::~CTGSuperPlayer(void)
{
}

void CTGSuperPlayer::SuperPlayer(void)
{
    if( MAX_PLAYER_NUM <= m_iPlayerIndex || m_iPlayerIndex < 0 ) return;
    g_MyD3D->MyPlayer[m_iPlayerIndex]->m_bSuperMode = m_bOnOff;
    SetComplete( true );
}

void CTGSuperPlayer::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        SuperPlayer();

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
        SuperPlayer();
    }
}

HRESULT CTGSuperPlayer::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("PlayerIndex", m_iPlayerIndex, return E_FAIL);

    return S_OK;
}