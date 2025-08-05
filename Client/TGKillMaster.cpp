#include "StdAfx.h"
#include "TGKillMaster.h"

#include "Monster.h"




CTGKillMaster::CTGKillMaster()
{
    m_iMonIndex     = 0;
    m_iMonType      = 0;
    enType          = TG_KILL_MASTER;
	m_bAlive        = false;
}

CTGKillMaster::~CTGKillMaster()
{

}

//************************************************************************
// 몬스터 정보를 설정합니다.
//************************************************************************
void CTGKillMaster::SetMonster( int iMonType, int iMonIndex )
{
    m_iMonType = iMonType;
    m_iMonIndex = iMonIndex;
}

//************************************************************************
// 몬스터 정보 처리
//************************************************************************
void CTGKillMaster::FrameMove(void)
{
    if( IsComplete() == true ) return;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonIndex);
	if( pMonster == NULL ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( pMonster->m_bDie != m_bAlive)
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
        //pMonster->ActionCallBack( "Die" );
        pMonster->m_bDie = !m_bAlive;
        SetComplete( true );
    }
}

HRESULT CTGKillMaster::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("MonIndex", m_iMonIndex, return E_FAIL);
	LUA_GET_VALUE("MonType", m_iMonType, return E_FAIL);
	LUA_GET_VALUE_DEF("MonKill", m_bAlive, false);
    return S_OK;
}

void CTGKillMaster::EndTrigger()
{
}
