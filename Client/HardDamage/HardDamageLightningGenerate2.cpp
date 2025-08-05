#include "stdafx.h"
#include ".\harddamagelightninggenerate2.h"
#include "DamageManager.h"
#include "Monster.h"
#include "KGCRoomManager.h"

CHardDamageLightningGenerate2::CHardDamageLightningGenerate2(void)
{
}

CHardDamageLightningGenerate2::~CHardDamageLightningGenerate2(void)
{
}

bool CHardDamageLightningGenerate2::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if( pDamageInstance->m_Life < 140) 
	{
		if( g_MyD3D->m_pDamageManager->Add_LighteningBolt( hitWho, pDamageInstance ) )
		{
            if( pDamageInstance->m_IsMonsterDamage )
            {
				MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
				if( pMonster != NULL ) {
					pDamageInstance->m_pOriginalDamage->GetDamageRate( fDamageRate, pMonster, g_MyD3D->MyPlayer[hitWho], pDamageInstance );
				}

            }

            PLAYER::DamageInfo damage;
            damage.SetDamage( m_fDamageToPlayer * fDamageRate );
            damage.SetDamageInstanceAttacker( pDamageInstance );
			g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
			if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
			{
				if(g_MyD3D->MyPlayer[hitWho]->GetHP() > 0)
				{
					g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_SMALLDAMAGE, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
				}
				else 
					g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}
		}
	}

	return true;
}

void CHardDamageLightningGenerate2::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{


	if( pDamageInstance->m_Life > 140 )
	{
		if( g_MyD3D->m_pDamageManager->Add_LighteningBolt( 6, pDamageInstance ) )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
			if( pMonster != NULL ) {
                pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
                pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
				pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What, m_fDamageToMonster * -1000.0f,
					(EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
					m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack,
					pDamageInstance->m_pOriginalDamage->m_nSpecialGrade);
			}
        }
	}
	
}
