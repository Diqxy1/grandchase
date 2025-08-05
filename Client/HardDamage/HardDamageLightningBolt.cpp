#include "stdafx.h"
#include ".\harddamagelightningbolt.h"
#include "DamageManager.h"
#include "Monster.h"
#include "Spark.h"
#include "Stage.h"


CHardDamageLightningBolt::CHardDamageLightningBolt(void)
{
}

CHardDamageLightningBolt::~CHardDamageLightningBolt(void)
{
}


void CHardDamageLightningBolt::Begin(CDamageInstance* pDamageInstance )
{
	CDamage::Begin(pDamageInstance);   

	if( pDamageInstance->m_IsMonsterDamage )
		return;

	// 라이트닝 볼트의 무한 확대
	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_Angle = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
		pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle) * 0.57f;
		pDamageInstance->m_X += cosf(pDamageInstance->m_Angle) * 0.57f - 0.57f;


		if( pDamageInstance->m_X < g_MyD3D->MyStg->GetXLimit()
			&& pDamageInstance->m_Y > 0 
			&& pDamageInstance->m_Y < g_MyD3D->MyStg->GetYLimit() + 1.0f )
		{
			float fPosX = cosf(pDamageInstance->m_Angle);
			float fPosY = sinf(pDamageInstance->m_Angle);
			g_MyD3D->m_pDamageManager->AddWithLocate(pDamageInstance->m_What, pDamageInstance->m_Who, fPosX, fPosY, false, pDamageInstance->m_ExtraMonsterNum );
		}
	}
	else
	{
		pDamageInstance->m_Angle = -g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
		pDamageInstance->m_Y += sinf(-pDamageInstance->m_Angle) * 0.57f;
		pDamageInstance->m_X += -cosf(pDamageInstance->m_Angle) * 0.57f + 0.57f;
		if( pDamageInstance->m_X > 0 
			&& pDamageInstance->m_Y > 0 
			&& pDamageInstance->m_Y < g_MyD3D->MyStg->GetYLimit() + 1.0f )
		{
            float fPosX = -cosf(pDamageInstance->m_Angle);
            float fPosY = -sinf(pDamageInstance->m_Angle);
            g_MyD3D->m_pDamageManager->AddWithLocate(pDamageInstance->m_What, pDamageInstance->m_Who, fPosX, fPosY,false, pDamageInstance->m_ExtraMonsterNum );
		}
	}
	g_MyD3D->MyStg->Color_r = 0;
	g_MyD3D->MyStg->Color_g = 0;
	g_MyD3D->MyStg->Color_b = 0;

}

bool CHardDamageLightningBolt::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	
	CDamage::CrashPlayer(pDamageInstance,hitWho,fDamageRate);

	if( g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP )
	 && g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP )
     && g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER ) )
	 g_MyD3D->MyPlayer[hitWho]->Ah_Sound( 0 );

	if( pDamageInstance->m_What == DT_LIGHTNING_BOLT_BLUE )
		g_MyD3D->MySparks->BlueLight( g_MyD3D->MyPlayer[hitWho]->MyBody.x + 0.15f, g_MyD3D->MyPlayer[hitWho]->vPos.y - 0.05f, 0, 1.0f );
	else
		g_MyD3D->MySparks->Light( g_MyD3D->MyPlayer[hitWho]->MyBody.x + 0.15f, g_MyD3D->MyPlayer[hitWho]->vPos.y - 0.05f, 0, 1.0f );
	
	return true;

}

void CHardDamageLightningBolt::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if( pMonster == NULL ) return;

    pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;

    pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What, m_fDamageToMonster * -1000.0f,
                (EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
                m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack, 
                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );


	if( pDamageInstance->m_What == DT_LIGHTNING_BOLT_BLUE )
	{
		g_MyD3D->MySparks->BlueLight( pMonster->m_afX[0] - 1.25f + RANDOM_NUMBER * 0.5f,
			                          pMonster->m_afY[0] - 0.25f + RANDOM_NUMBER * 0.5f, 0, 2.0f );
	}
	else
	{
		g_MyD3D->MySparks->Light( pMonster->m_afX[0] - 1.25f + RANDOM_NUMBER * 0.5f,
			                      pMonster->m_afY[0] - 0.25f + RANDOM_NUMBER * 0.5f, 0, 2.0f );
	}

}

void CHardDamageLightningBolt::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );

	float fMonsterBody_Angle = 0.0f;
	if ( pMonster != NULL )
		fMonsterBody_Angle = pMonster->Body_Angle;

    if( pDamageInstance->m_IsMonsterDamage && 
        ( pDamageInstance->m_What == DT_LIGHTNING_BOLT ||
          pDamageInstance->m_What == DT_LIGHTNING_BOLT_BLUE ) )
    {
        pDamageInstance->m_pOriginalDamage->m_Start_Angle = fMonsterBody_Angle;
        CDamage::BeginWithLocate(pDamageInstance,fStartX, fStartY );
        return;
    }

    if ( false == pDamageInstance->m_IsMonsterDamage )
        CDamage::Begin(pDamageInstance);

	

    // 라이트닝 볼트의 무한 확대
    if( pDamageInstance->m_IsRight )
    {
        pDamageInstance->m_Angle = pDamageInstance->m_IsMonsterDamage ? fMonsterBody_Angle : g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
        pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle) * 0.57f + fStartY;
        pDamageInstance->m_X += cosf(pDamageInstance->m_Angle) * 0.57f - 0.57f + fStartX;

        if( pDamageInstance->m_X < g_MyD3D->MyStg->GetXLimit()
            && pDamageInstance->m_Y > 0 
            && pDamageInstance->m_Y < g_MyD3D->MyStg->GetYLimit() + 1.0f )
        {
            fStartX += cosf(pDamageInstance->m_Angle);
            fStartY += sinf(pDamageInstance->m_Angle);
            g_MyD3D->m_pDamageManager->AddWithLocate(pDamageInstance->m_What, pDamageInstance->m_Who, fStartX, fStartY, false, pDamageInstance->m_ExtraMonsterNum );
        }
    }
    else
    {
        pDamageInstance->m_Angle = -(pDamageInstance->m_IsMonsterDamage ? fMonsterBody_Angle : g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle);
        pDamageInstance->m_Y += sinf(-pDamageInstance->m_Angle) * 0.57f + fStartY;
        pDamageInstance->m_X += -cosf(pDamageInstance->m_Angle) * 0.57f + 0.57f + fStartX;
        if( pDamageInstance->m_X > 0 
            && pDamageInstance->m_Y > 0 
            && pDamageInstance->m_Y < g_MyD3D->MyStg->GetYLimit() + 1.0f )
        {
            fStartX -= cosf(pDamageInstance->m_Angle);
            fStartY -= sinf(pDamageInstance->m_Angle);
            g_MyD3D->m_pDamageManager->AddWithLocate(pDamageInstance->m_What, pDamageInstance->m_Who, fStartX, fStartY,false, pDamageInstance->m_ExtraMonsterNum );
        }
    }
}