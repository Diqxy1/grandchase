#include "stdafx.h"
#include ".\harddamagebasiliskfire.h"
#include "Spark.h"
#include "Stage.h"
#include "DamageManager.h"
#include "KGCRoomManager.h"

CHardDamageBasiliskFire::CHardDamageBasiliskFire(void)
{
}

CHardDamageBasiliskFire::~CHardDamageBasiliskFire(void)
{
}

void CHardDamageBasiliskFire::Begin(CDamageInstance* pDamageInstance )
{
	pDamageInstance->m_X		= g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - 0.8f;
	pDamageInstance->m_Y		= g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + sinf(g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle)*.4f;
	pDamageInstance->m_Angle	= m_Start_Angle + g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
	pDamageInstance->m_Scale_x	= m_Start_Scale_x + RANDOM_NUMBER*.2f;
}

bool CHardDamageBasiliskFire::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	if( g_MyD3D->MyStg->GetStage() != GC_GS_QUEST6_C )
    {
        PLAYER::DamageInfo damage;
        damage.SetDamage( -m_fDamageToPlayer*0.6f );
        damage.SetDamageInstanceAttacker( pDamageInstance );
		g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
		g_MyD3D->MySparks->Fires( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y ) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 0.3f, 5 );

		if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) ) 
        {
            PLAYER::DamageInfo damage;
            damage.SetDamage( m_fDamageToPlayer );
            damage.SetDamageInstanceAttacker( pDamageInstance );
			g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

			if( g_MyD3D->MyPlayer[hitWho]->GetHP() > 0 )
            {
                PLAYER::DamageInfo damage;
                damage.SetDamage( -m_fDamageToPlayer );
                damage.SetDamageInstanceAttacker( pDamageInstance );
				g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
				g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_BIGDAMAGE, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}
			else
            {
                PLAYER::DamageInfo damage;
                damage.SetDamage( -m_fDamageToPlayer*0.9f );
                damage.SetDamageInstanceAttacker( pDamageInstance );
				g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
				g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}
			g_MyD3D->MyPlayer[hitWho]->bIsRight = !pDamageInstance->m_IsRight;
		}						
	}
	else
    {
        PLAYER::DamageInfo damage;
        damage.SetDamage( m_fDamageToPlayer*0.039f );
        damage.SetDamageInstanceAttacker( pDamageInstance );
		g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
		g_MyD3D->MySparks->Fires( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 0.3f, 5 );

		if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
		{						
			if( g_MyD3D->MyPlayer[hitWho]->GetHP() > 0 )
			{
				g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_BIGDAMAGE, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}
			else
			{
				g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}							
			g_MyD3D->MyPlayer[hitWho]->bIsRight = !pDamageInstance->m_IsRight;
		}

		g_MyD3D->MyPlayer[hitWho]->bIsRight = true;
		g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.01f;
		if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0 )		//점프 상태 해제
			g_MyD3D->MyPlayer[hitWho]->y_Speed = 0;

		return true;
	}

    PLAYER::DamageInfo damage;
#if defined( LEVEL_DESIGN_STAT_CALC )
    damage.SetDamage( -10.f );
#else
    damage.SetDamage( -0.1f );
#endif
    damage.SetDamageInstanceAttacker( pDamageInstance );
	g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

	if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
	{
		if( pDamageInstance->m_IsRight ) 
			g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.01f;
		else 
			g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.01f;
		if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0 )		//점프 상태 해제
			g_MyD3D->MyPlayer[hitWho]->y_Speed = 0;
	}

	return true;
}
