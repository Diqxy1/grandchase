#include "StdAfx.h"
#include ".\tgreward.h"
//



CTGReward::CTGReward(void)
{
    SetComplete( false );
    enType       = TG_REWARD;

    m_iEXP       = 0;
    m_iGP        = 0;
}

CTGReward::~CTGReward(void)
{
}

void CTGReward::Reward(void)
{
    // 보상에 관한 것을 적어준다.
    // 현재 사용안함
    //g_pkQuestManager->m_iExp        = m_iEXP;
    //g_pkQuestManager->m_iGp         = m_iGP;

    SetComplete( true );
}

void CTGReward::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        Reward();

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
        Reward();
    }
}

HRESULT CTGReward::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("EXP", m_iEXP, return E_FAIL);
    LUA_GET_VALUE("GP", m_iGP, return E_FAIL);

    return S_OK;
}