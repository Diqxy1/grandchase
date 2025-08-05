#include "stdafx.h"
#include ".\harddamagelightninggenerate1.h"
#include "Monster.h"
#include "DamageManager.h"
#include "KGCRoomManager.h"


CHardDamageLightningGenerate1::CHardDamageLightningGenerate1(void)
{
}

CHardDamageLightningGenerate1::~CHardDamageLightningGenerate1(void)
{
}

void CHardDamageLightningGenerate1::Begin(CDamageInstance* pDamageInstance )
{
	CDamage::Begin(pDamageInstance);
	// 라이팅 볼트의 제너레이트 원... 재조정한다.
	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_Angle = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle) * 0.09f - 0.03f;
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle) * 0.15f - 0.01f;		
	}
	else
	{
		pDamageInstance->m_Angle = -g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
		pDamageInstance->m_Y += sinf(-pDamageInstance->m_Angle) * 0.09f - 0.03f;
		pDamageInstance->m_X += -cosf(pDamageInstance->m_Angle) * 0.15f + 0.01f;			
	}
}

bool CHardDamageLightningGenerate1::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if( pDamageInstance->m_Life < 140 )
	{
		float temp_Angle = pDamageInstance->m_Angle;
		if( pDamageInstance->m_IsRight )
			pDamageInstance->m_Angle = 0;
		else 
			pDamageInstance->m_Angle = 3.1416f; 

        if( pDamageInstance->m_IsMonsterDamage )
		{	
			MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
			if( pMonster != NULL ) {
				pDamageInstance->m_pOriginalDamage->GetDamageRate( fDamageRate, pMonster, g_MyD3D->MyPlayer[hitWho], pDamageInstance );
			}

        }

		if( g_MyD3D->m_pDamageManager->Add_LighteningBolt( hitWho, pDamageInstance ) )
        {
            PLAYER::DamageInfo damage;
            damage.SetDamage( m_fDamageToPlayer * fDamageRate );
            damage.SetDamageInstanceAttacker( pDamageInstance );
			g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

			if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
			{
				if( g_MyD3D->MyPlayer[hitWho]->GetHP() > 0 )
				{
					g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( MID_COMMON_SMALLDAMAGE, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
				}
				else 
					g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}
		}                        
		pDamageInstance->m_Angle = temp_Angle;
	}

	return true;

}

void CHardDamageLightningGenerate1::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
	if( pDamageInstance->m_Life > 140 )
	{
		float temp_Angle = pDamageInstance->m_Angle;
		if( pDamageInstance->m_IsRight ) 
			pDamageInstance->m_Angle = 0;
		else 
			pDamageInstance->m_Angle = 3.1416f; 

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

		pDamageInstance->m_Angle = temp_Angle;
		
	}
}
