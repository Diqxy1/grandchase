#include "StdAfx.h"
#include ".\tghp.h"

#include "Monster.h"



CTGHp::CTGHp(void)
{
    enType              = TG_HP;
    m_bPlayer           = false;
    m_iSlot             = 0;
    m_iMonsterID        = 0;
    m_fHp               = 0;
}

CTGHp::~CTGHp(void)
{
}

void CTGHp::HP(void)
{
    if( m_bPlayer )
    {
        if( g_MyD3D->Get_MyPlayer() == m_iSlot
            && PS_DEAD != g_MyD3D->GetMyPlayer()->m_ePlayerState
            )
        {
            g_MyD3D->GetMyPlayer()->SetHP( m_fHp );
        }
    }
    else
    {
        if( g_kGlobalValue.m_kUserInfo.bHost )
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iSlot);
			if( pMonster ){
                pMonster->m_fHP = m_fHp;
                if( pMonster->m_fHP > pMonster->m_fHPPoint )
                    pMonster->m_fHP = pMonster->m_fHPPoint;
                else if( pMonster->m_fHP < 0.0f )
                    pMonster->m_fHP = 0.0f;
            }
        }
    }

    SetComplete(true);
}

void CTGHp::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        HP();

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
        HP();
    }
}

HRESULT CTGHp::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("HP", m_fHp, return E_FAIL);

    return S_OK;
}