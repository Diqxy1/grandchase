#include "stdafx.h"
#include ".\HardDamageRyanTotem.h"
#include "KGCPlayerCommon.h"
#include "KGCRoomManager.h"
#include "DamageManager.h"

CHardDamageRyanTotem::CHardDamageRyanTotem(void)
{
}

CHardDamageRyanTotem::~CHardDamageRyanTotem(void)
{
}


bool CHardDamageRyanTotem::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	// 플레이어 아니면 안녕~
	if ( false == g_MyD3D->IsPlayerIndex( hitWho ) )
		return false;

	// 우리팀 아니면 안녕~
	if( false == g_MyD3D->IsSameTeam( hitWho, pDamageInstance->m_Who ) )
		return false;

	PLAYER* pPlayer = g_MyD3D->MyPlayer[hitWho];

	// 알맞은 버프를 걸쟈~
	switch ( pDamageInstance->m_What )
	{
	case DT_SKILL_RYAN_HP_TOTEM_AREA:
		{
			if ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
			{

				float rate = 0.00034f;
                if( GC_GLOBAL_DEFINE::bMatchBalance2014 ) {
                    rate = 0.000238f;
                }

				if ( pPlayer->GetHP() != 0.0f )
				{
					pPlayer->SetHP( pPlayer->GetHP() + pPlayer->m_fAbility[ABILITY_HP]*rate );
				}
			}
		}
		break;
	case DT_SKILL_RYAN_MP_TOTEM_AREA:
		if ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
		{
			if ( SiKGCRoomManager()->IsMonsterGameMode() )
			{
				g_MyD3D->ChargeLocalPlayerMana( 0.0016f );
			}
			else
			{
				g_MyD3D->ChargeLocalPlayerMana( 0.0006f );
			}
		}
		break;
	case DT_SKILL_RYAN_DP_TOTEM_AREA:
		g_MyD3D->MyPlayer[hitWho]->SetMagicEffect( EGC_EFFECT_DP_TOTEM, 3.0f );
		break;
	}

	return true;
}

void CHardDamageRyanTotem::End( CDamageInstance* pDamageInstance )
{
	if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
		switch (pDamageInstance->m_What)
		{
		case DT_SKILL_RYAN_HP_TOTEM:
			pDamageInstance->CreateSequence("Ryan1_Totem_HP_spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			pDamageInstance->CreateSequence("Ryan1_Totem_HP_spark_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			pDamageInstance->CreateSequence("Ryan1_Totem_spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			break;
		case DT_SKILL_RYAN_MP_TOTEM:
			pDamageInstance->CreateSequence("Ryan1_Totem_MP_spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			pDamageInstance->CreateSequence("Ryan1_Totem_MP_spark_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			pDamageInstance->CreateSequence("Ryan1_Totem_spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			break;
		case DT_SKILL_RYAN_DP_TOTEM:
			pDamageInstance->CreateSequence("Ryan1_Totem_DP_spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			pDamageInstance->CreateSequence("Ryan1_Totem_DP_spark_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			pDamageInstance->CreateSequence("Ryan1_Totem_spark_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			break;
		}
	}
	CDamage::End( pDamageInstance );
}

void CHardDamageRyanTotem::FrameMove( CDamageInstance* pDamageInstance )
{
	switch ( pDamageInstance->m_What )
	{
	case DT_SKILL_RYAN_HP_TOTEM:
		g_MyD3D->m_pDamageManager->AddWithLocate( DT_SKILL_RYAN_HP_TOTEM_AREA, pDamageInstance->m_Who ,pDamageInstance->m_X , pDamageInstance->m_Y);
		break;
	case DT_SKILL_RYAN_MP_TOTEM:
		g_MyD3D->m_pDamageManager->AddWithLocate( DT_SKILL_RYAN_MP_TOTEM_AREA, pDamageInstance->m_Who ,pDamageInstance->m_X , pDamageInstance->m_Y);
		break;
	case DT_SKILL_RYAN_DP_TOTEM:
		g_MyD3D->m_pDamageManager->AddWithLocate( DT_SKILL_RYAN_DP_TOTEM_AREA, pDamageInstance->m_Who ,pDamageInstance->m_X , pDamageInstance->m_Y);
		break;
	}

	CDamage::FrameMove( pDamageInstance );
}

