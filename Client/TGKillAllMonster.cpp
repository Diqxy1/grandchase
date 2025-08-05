#include "StdAfx.h"
#include "TGKillAllMonster.h"
#include "Monster.h"

CTGKillAllMonster::CTGKillAllMonster()
{
    m_bLive = false;
}

CTGKillAllMonster::~CTGKillAllMonster()
{

}

//************************************************************************
// 모든 몬스터가 죽었나 체크 
//************************************************************************
void CTGKillAllMonster::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( g_kMonsterManager.GetLiveMonsterNumExceptSummonMonster() == 0 ) 
        {
            SetComplete( true );
            if( !m_bSendPacket )
            {
                // 패킷을 보낸다.
                g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
                g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
                m_bSendPacket = true;
            }
        }
    }
    else if( m_bCheckByHost )
    {
        if( g_kMonsterManager.GetLiveMonsterNumExceptSummonMonster() == 0 ) { 
            SetComplete( true );
        }
    }
}

HRESULT CTGKillAllMonster::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE_DEF("MonKill", m_bLive, false);
    return S_OK;
}
