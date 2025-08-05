#include "StdAfx.h"
#include ".\tgunitmovement.h"

#include "QuestGameManager.h"
#include "Monster.h"


CTGUnitMovement::CTGUnitMovement(void)
{
    enType          = TG_UNITMOVEMENT;
    m_bPlayer       = false;
    m_bOnOff        = false;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
}

CTGUnitMovement::~CTGUnitMovement(void)
{

}

void CTGUnitMovement::UnitMovement(void)
{
    if( m_bPlayer )
    {
        if( g_MyD3D->Get_MyPlayer() == m_iSlot )
        {
            g_MyD3D->SetEnableKeyInput(m_bOnOff);
            if( m_bOnOff )
            {
                g_pkQuestManager->PlayerToReady(m_iSlot);
            }
        }
    }
    else
    {
        if( g_kGlobalValue.m_kUserInfo.bHost )
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iSlot);
			if( pMonster != NULL )
            {
                pMonster->SetMovement(m_bOnOff);
            }
        }
    }

    SetComplete(true);
}

void CTGUnitMovement::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        UnitMovement();

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
        UnitMovement();
    }
}

HRESULT CTGUnitMovement::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);

    return S_OK;
}