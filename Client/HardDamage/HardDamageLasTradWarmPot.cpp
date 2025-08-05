#include "stdafx.h"
#include ".\HardDamageLasTradWarmPot.h"





//


#include "DamageManager.h"

CHardDamageLasTradWarmPot::CHardDamageLasTradWarmPot(void)
{	
}

CHardDamageLasTradWarmPot::~CHardDamageLasTradWarmPot(void)
{
}

void CHardDamageLasTradWarmPot::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );

	pDamageInstance->m_IsRight = true;
}

void CHardDamageLasTradWarmPot::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );

	if ( 0 != o3 )
		pDamageInstance->m_pAnim->SetShow( pDamageInstance->m_IsShow );
	else
		pDamageInstance->m_pAnim->SetShow( false );


	if(pDamageInstance->m_IsRight)
		pDamageInstance->m_pAnim->GetMatrix()->Scale(  pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );
	else
		pDamageInstance->m_pAnim->GetMatrix()->Scale( -pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );				
	
	if( pDamageInstance->m_pOriginalDamage->m_iDamageHP <= 0 )
	{
		g_KDSound.Play( "1447" );
		g_KDSound.Play( "1448" );
		g_MyD3D->m_pDamageManager->AddWithLocate( DT_SKILL_LAS_TRADWARM_BOMB, pDamageInstance->m_Who , pDamageInstance->m_X, pDamageInstance->m_Y );
	}
}