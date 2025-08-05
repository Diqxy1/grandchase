#include "stdafx.h"
#include ".\HardDamageBomb.h"





CHardDamageBomb::CHardDamageBomb(void)
{
}

CHardDamageBomb::~CHardDamageBomb(void)
{
}


void CHardDamageBomb::FrameMove(CDamageInstance* pDamageInstance )
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
}

