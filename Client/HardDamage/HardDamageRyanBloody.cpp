#include "stdafx.h"
#include ".\HardDamageRyanBloody.h"
#include "KGCPlayerCommon.h"
#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"

CHardDamageRyanBloody::CHardDamageRyanBloody(void)
{
}

CHardDamageRyanBloody::~CHardDamageRyanBloody(void)
{
}

void CHardDamageRyanBloody::End( CDamageInstance* pDamageInstance )
{
	CDamage::End( pDamageInstance );
}

bool CHardDamageRyanBloody::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f */ )
{
	PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
	if ( pPlayer->IsLocalPlayer() )
	{
#if defined (NATION_USA )
		pPlayer->SetHP( pPlayer->GetHP() + pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer * fDamageRate * -0.5f );
#else
		pPlayer->SetHP( pPlayer->GetHP() + pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer * fDamageRate * -1.0f );
#endif
	}

	return CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );
}

void CHardDamageRyanBloody::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{

	PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
	if ( pPlayer->IsLocalPlayer() )
	{
#if defined (NATION_USA )
		pPlayer->SetHP( pPlayer->GetHP() + pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * 10.0f * -0.5f );
#else
		pPlayer->SetHP( pPlayer->GetHP() + pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * 10.0f * -1.0f );
#endif
	}

	CDamage::CrashMonster( pDamageInstance, Monsteri );
	return;
}

void CHardDamageRyanBloody::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );
}
