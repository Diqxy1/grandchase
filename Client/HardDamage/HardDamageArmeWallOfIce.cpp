#include "stdafx.h"
#include ".\HardDamageArmeWallOfIce.h"





//


#include "DamageManager.h"

CHardDamageArmeWallOfIce::CHardDamageArmeWallOfIce(void)
{	
}

CHardDamageArmeWallOfIce::~CHardDamageArmeWallOfIce(void)
{
}

void CHardDamageArmeWallOfIce::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );
}

void CHardDamageArmeWallOfIce::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );	
	
	if( pDamageInstance->m_pOriginalDamage->m_iDamageHP <= 0 )
	{
		KSafeArray<CDamage*,3> wall;
		
		wall[0] = g_MyD3D->m_pDamageManager->GetOriginalDamage(DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL);
		wall[1] = g_MyD3D->m_pDamageManager->GetOriginalDamage(DT_ARME_SKILL_WALLOFICE_VERTICAL_WALL);
		wall[2] = g_MyD3D->m_pDamageManager->GetOriginalDamage(DT_ARME_SKILL_WALLOFICE_HORIZON_WALL);

		for( int i = 0 ; i < 3 ; i++)
		{
			if( wall[i] )
			{
				wall[i]->m_iDamageHP = 0;
				wall[i]->End(pDamageInstance);
			}
		}
	}
}

