#include "stdafx.h"
#include ".\harddamagemagicstar.h"

#include "Spark.h"

#include "KGCRoomManager.h"

CHardDamageMagicStar::CHardDamageMagicStar(void)
{
}

CHardDamageMagicStar::~CHardDamageMagicStar(void)
{
}


bool CHardDamageMagicStar::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
	g_MyD3D->MySparks->Particles( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 10 + pDamageInstance->m_Life / 60, 15, pDamageInstance->m_Who );
	pDamageInstance->Add( HIT_LIGHT, (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
	g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( pDamageInstance->m_Who );
	g_KDSound.Play( "34" );

	if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || ( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
		) 
	{
        if ( !g_MyD3D->MyPlayer[hitWho]->OnDamagePreProcess( pDamageInstance->m_IsMonsterDamage ? pDamageInstance->m_ExtraMonsterNum : pDamageInstance->m_Who, false, pDamageInstance->m_IsMonsterDamage, 
            pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_What,  hitWho) )
            return false;

		if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_ELESIS && pDamageInstance->m_What != DT_ARME3_TORNADO3 && pDamageInstance->m_What != DT_STONE_GOLEM_TORNADO3 )
		{
			if( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND2 != -1 )
			{
				g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND2, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}
		}
		else
        {
            PLAYER::DamageInfo damage;
            damage.SetDamage( m_fDamageToPlayer );
            damage.SetDamageInstanceAttacker( pDamageInstance );
			g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );
			g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_BIGDAMAGE, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);

		if( pDamageInstance->m_IsRight ) 
			g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.01f;
		else 
			g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.01f;
		if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0 )		//점프 상태 해제
			g_MyD3D->MyPlayer[hitWho]->y_Speed = 0;

		}
	}
	g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDamageEff( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.4f );
	
	return true;
}

void CHardDamageMagicStar::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);

	if( pDamageInstance->m_What == DT_ARME3_TORNADO3 || pDamageInstance->m_What == DT_STONE_GOLEM_TORNADO3 )
		return ;
	
	// 마법사의 별이라면...
	if( pDamageInstance->m_Scale_x > 0.4f ) 
		pDamageInstance->m_Scale_x = 0.4f;
	if( pDamageInstance->m_Life < 255 && o5 ) 
		pDamageInstance->m_Life++;	
}
