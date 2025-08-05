#include "stdafx.h"
#include ".\HardDamageKindPet.h"
#include "Monster.h"
#include "DamageManager.h"


CHardDamageKindPet::CHardDamageKindPet( void )
:dwTime(0)
{

}

CHardDamageKindPet::~CHardDamageKindPet( void )
{

}

void CHardDamageKindPet::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove(pDamageInstance);

	if( false == g_MyD3D->IsPlayerIndex( pDamageInstance->m_HitWho ) )
		return;
	if ( pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() || g_MyD3D->IsSameTeam(g_MyD3D->Get_MyPlayer(),pDamageInstance->m_Who) )
		return;

	if ( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPlayerState() == PS_DEAD )
	{
		pDamageInstance->m_Life = 1;
	}

	if ( dwTime++ % (GC_FPS_LIMIT * 3) == 0 )
	{
		PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
#ifdef NATION_KOREA
		g_MyD3D->m_pDamageManager->AddWithLocate(DT_PET_ARON_ATTACK4, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, 0, NULL, pDamageInstance->m_HitWho );	
#else
		g_MyD3D->m_pDamageManager->AddWithLocate(DT_PET_ARON_ATTACK4, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, -1, NULL, pDamageInstance->m_HitWho );	
#endif	
	}
}
