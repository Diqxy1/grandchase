#include "StdAfx.h"
#include "TGObjHpChange.h"

#include "Monster.h"

CTGObjHpChange::CTGObjHpChange(void)
: m_iObjectIndex(0)
, m_fPercent(1.0f)
, m_iObjectType(1)
{
}

CTGObjHpChange::~CTGObjHpChange(void)
{
}

void CTGObjHpChange::FrameMove( void )
{
	if( IsComplete() == true ) return;

	if( g_kGlobalValue.m_kUserInfo.bHost )
	{
		switch( m_iObjectType )
		{
		case HC_PLAYER:
			CheckPlayerHP( m_iObjectIndex, m_fPercent );
			break;
		case HC_MONSTER:
			CheckMonsterHP( m_iObjectIndex, m_fPercent );
			break;
		}
	}

	if( g_kGlobalValue.m_kUserInfo.bHost )
	{
		if( IsComplete() == true && m_bSendPacket == false )
		{
			// 패킷을 보낸다.
			g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
			g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
			m_bSendPacket = true;
		}
	}
	else if( m_bCheckByHost )
	{
		SetComplete( true );
	}
}

bool CTGObjHpChange::CheckPlayerHP( int iIndex, float fPercent )
{
	PLAYER* player = g_MyD3D->MyPlayer[iIndex];

	float HP = player->GetHP();
	float HPFull = player->m_fAbility[ABILITY_HP];

	if( HP <= HPFull * fPercent )
	{
		SetComplete( true );
		return true;
	}
	return false;
}

bool CTGObjHpChange::CheckMonsterHP( int iIndex, float fPercent )
{
	MONSTER *pMonster = g_kMonsterManager.GetMonster(iIndex);

	float HP = pMonster->m_fHP;
	float HPFull = pMonster->m_fHPPoint;

	if( HP <= HPFull * fPercent )
	{
		SetComplete( true );
		return true;
	}
	return false;
}

HRESULT CTGObjHpChange::LoadItemFromLua( KLuaManager &luaMgr )
{
	LUA_GET_VALUE( "ObjectIndex", m_iObjectIndex, return false );
	LUA_GET_VALUE( "HpPercent", m_fPercent, return false );
	LUA_GET_VALUE( "ObjectType", m_iObjectType, return false );

	return S_OK;
}