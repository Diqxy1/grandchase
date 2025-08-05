#include "stdafx.h"
#include ".\HardDamageChaos2.h"
#include "Monster.h"

CHardDamageChaosMapRail::CHardDamageChaosMapRail( void )
{

}

CHardDamageChaosMapRail::~CHardDamageChaosMapRail( void )
{

}

void CHardDamageChaosMapRail::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    MONSTER* pMonster = g_kMonsterManager.GetMonster( Monsteri );
    if( pMonster )
    {
        // 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
        pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
        pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    }

	return;
}

bool CHardDamageChaosMapRail::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    PLAYER* pPlayer = g_MyD3D->MyPlayer[hitWho];
    if ( pPlayer->m_kUserInfo.bLive && pPlayer->IsContact )
    {
        pPlayer->vPos.x += pDamageInstance->m_pOriginalDamage->m_fDMXSpeed;
    }

	return true;
}