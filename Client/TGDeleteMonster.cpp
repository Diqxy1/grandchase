#include "StdAfx.h"
#include ".\tgdeletemonster.h"

#include "Monster.h"


CTGDeleteMonster::CTGDeleteMonster(void)
{
    enType          = TG_DELETEMONSTER;
    m_bAllSlot      = false;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
}

CTGDeleteMonster::~CTGDeleteMonster(void)
{
}

void CTGDeleteMonster::DeleteMonster(void)
{
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( m_bAllSlot )
        {
			for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				MONSTER *pMonster = mit->second;
				if(pMonster == NULL) continue;

				if( pMonster->GetMonsterType() == g_kGlobalValue.GetEscortMonsterID() )
					continue;
				pMonster->SetHP( 0.0f );
			}
        }
        else
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iSlot);
			if( pMonster ){
                 pMonster->SetHP( 0.0f );
            }
        }
    }

    SetComplete(true);
}

void CTGDeleteMonster::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        DeleteMonster();

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
        DeleteMonster();
    }
}

HRESULT CTGDeleteMonster::LoadItemFromLua(KLuaManager &luaMgr)
{    
    LUA_GET_VALUE_DEF("AllSlot", m_bAllSlot, false);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);

    return S_OK;
}
