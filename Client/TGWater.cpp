#include "StdAfx.h"
#include ".\tgwater.h"

#include "./MapSystem/GCFantasticMap.h"


CTGWater::CTGWater(void)
{
    enType              = TG_WATER;
    m_bOnOff            = false;
    m_iWaterHeight      = 0;
}

CTGWater::~CTGWater(void)
{
}

void CTGWater::Water(void)
{
    SiKGCFantasticMap()->SetWaterHeight(m_iWaterHeight*0.0025f);
    if( m_bOnOff )
        SiKGCFantasticMap()->CreateWater();
    else
        SiKGCFantasticMap()->ClearWater();

    SetComplete(true);
}

void CTGWater::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        Water();

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
        Water();
    }
}

HRESULT CTGWater::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("WaterHeight", m_iWaterHeight, return E_FAIL);

    return S_OK;
}