#include "StdAfx.h"
#include ".\tgpositionparticle.h"


CTGPositionParticle::CTGPositionParticle(void)
{
    m_iLeft         = 0;
    m_iRight        = 0;
    m_iTop          = 0;
    m_iBottom       = 0;
    m_iParticleID   = 0;
}

CTGPositionParticle::~CTGPositionParticle(void)
{
}

void CTGPositionParticle::PositionParticle(void)
{

}

void CTGPositionParticle::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        PositionParticle();

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
        PositionParticle();
    }
}

HRESULT CTGPositionParticle::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Left", m_iLeft, return E_FAIL);
    LUA_GET_VALUE("Right", m_iRight, return E_FAIL);
    LUA_GET_VALUE("Top", m_iTop, return E_FAIL);
    LUA_GET_VALUE("Bottom", m_iBottom, return E_FAIL);
    LUA_GET_VALUE("ParticleID", m_iParticleID, return E_FAIL);

    return S_OK;
}