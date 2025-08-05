#include "StdAfx.h"
#include ".\tgunitparticle.h"


CTGUnitParticle::CTGUnitParticle(void)
{
    enType          = TG_UNITPARTICLE;
    m_bPlayer       = false;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_iParticleID   = 0;
}

CTGUnitParticle::~CTGUnitParticle(void)
{
}

void CTGUnitParticle::UnitParticle(void)
{

    SetComplete(true);
}

void CTGUnitParticle::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        UnitParticle();

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
        UnitParticle();
    }
}

HRESULT CTGUnitParticle::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("ParticleID", m_iParticleID, return E_FAIL);

    return S_OK;
}