#include "StdAfx.h"
#include ".\tgmp.h"
#include "MyD3D.h"

#include "Headup Display.h"


CTGMp::CTGMp(void)
{
    enType              = TG_MP;
    m_bPlayer           = false;
    m_iSlot             = 0;
    m_iMonsterID        = 0;
    m_fMp               = 0;
}

CTGMp::~CTGMp(void)
{
}

void CTGMp::MP(void)
{
    if( m_bPlayer )
    {
        if( g_MyD3D->Get_MyPlayer() == m_iSlot
            && PS_DEAD != g_MyD3D->GetMyPlayer()->m_ePlayerState
            )
        {
            g_MyD3D->MyHead->Level = m_fMp;
            if ( g_MyD3D->MyHead->Level > g_kGlobalValue.GetMaxMP() )
                g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
            else if( g_MyD3D->MyHead->Level < 0.0f )
                g_MyD3D->MyHead->Level = 0.0f;
        }
    }
    else
    {
        // 몬스터는 MP를 안쓴다... 혹시 쓸지 모르니까 인터페이스만...
    }

    SetComplete(true);
}

void CTGMp::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        MP();

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
        MP();
    }
}

HRESULT CTGMp::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("MP", m_fMp, return E_FAIL);

    return S_OK;
}