#include "stdafx.h"
#include "HardDamageMonsterMine.h"


#include "DamageManager.h"
#include "Spark.h"

CHardDamageMonsterMine::CHardDamageMonsterMine( void )
{

}

CHardDamageMonsterMine::~CHardDamageMonsterMine( void )
{

}


void CHardDamageMonsterMine::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::NotAngleFrameMove(pDamageInstance);

	if( pDamageInstance->m_pAnim != NULL )
	{		

		int iLife = pDamageInstance->m_Life;

		float fX = pDamageInstance->m_X + pDamageInstance->m_fAnimScaleX;
		float fY = pDamageInstance->m_Y + pDamageInstance->m_fAnimScaleY;
		float fTransX = ( pDamageInstance->m_IsRight ? 0.01942f : -0.01942f );
		float fTransY = 0.00426f;
		float fScaleX = ( pDamageInstance->m_IsRight ? 0.000109f : -0.000109f )*iLife;
		float fScaleY = 0.000175f*iLife;

		pDamageInstance->m_pAnim->GetMatrix()->Move( fX+fScaleX+fTransX-1.0f, fY+fScaleY+fTransY-0.5f, 0.5f );

		if ( 0 != o3 )
			pDamageInstance->m_pAnim->SetShow( pDamageInstance->m_IsShow );
		else
			pDamageInstance->m_pAnim->SetShow( false );


		if(pDamageInstance->m_IsRight)
			pDamageInstance->m_pAnim->GetMatrix()->Scale(  pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );
		else
			pDamageInstance->m_pAnim->GetMatrix()->Scale( -pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );				
	}

	if ( pDamageInstance->m_Life < 5 )
	{
		// 폭발 이펙트를 발생시킨다.
		g_KDSound.Play( "43" );
		CDamageInstance* pDamage = NULL;
		pDamage = g_MyD3D->m_pDamageManager->AddWithLocate( DT_MONSTER_BOMB_FIRE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true, pDamageInstance->m_ExtraMonsterNum );
		pDamage->m_IsMonsterDamage = pDamageInstance->m_IsMonsterDamage;

		pDamage = g_MyD3D->m_pDamageManager->AddWithLocate( DT_MONSTER_BOMB_WAVE, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y, false, pDamageInstance->m_ExtraMonsterNum );
		pDamage->m_IsMonsterDamage = pDamageInstance->m_IsMonsterDamage;
		g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
	}
}
