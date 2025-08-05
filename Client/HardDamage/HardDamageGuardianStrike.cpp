#include "stdafx.h"
#include ".\harddamageguardianstrike.h"
#include "DamageManager.h"

#include "Spark.h"
#include "Stage.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"

CHardDamageGuardianStrike::CHardDamageGuardianStrike(void)
{
}

CHardDamageGuardianStrike::~CHardDamageGuardianStrike(void)
{
}


void CHardDamageGuardianStrike::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	//발동후 당분간 움직이지 마라..
	if (SiKGCRoomManager()->IsMonsterGameMode())
	{
		if (pDamageInstance->m_Life > 245)
			return;
	}
	else
	{
		if (pDamageInstance->m_Life > 200)
			return;
	}

	CDamage::NotAngleFrameMove(pDamageInstance);  

	if (SiKGCRoomManager()->IsMonsterGameMode())
	{
		if (pDamageInstance->m_Life > 240)
			return;
	}
	else
	{
		if (pDamageInstance->m_Life > 195)
			return;
	}

	// 바닥 충돌체크
	float fPosY;
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        D3DXVECTOR2 dxvCrashPos;
        SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
        fPosY = dxvCrashPos.y;
    }
    else
    {
        fPosY= g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0 ).y;
    }
	if( fPosY != -1.0f )
	{
		pDamageInstance->m_Life = 1;
		g_KDSound.Play( "38" );

		g_MyD3D->m_pDamageManager->AddWithLocate( DT_GUARDIAN_FOG, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y );

		CDamageInstance* fire;
		float fAngle = 0.0f;
		for( int i=0 ; i<8 ; ++i, fAngle += 45.0f )
		{
			if (SiKGCRoomManager()->IsMonsterGameMode())
			{
				fire = g_MyD3D->m_pDamageManager->AddWithLocate(DT_GUARDIAN_BALL_DUN, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
				fire->m_Angle = D3DXToRadian(fAngle);
			}
			else
			{
				fire = g_MyD3D->m_pDamageManager->AddWithLocate(DT_GUARDIAN_BALL, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
				fire->m_Angle = D3DXToRadian(fAngle);
			}
		}

		pDamageInstance->Add( DARK_SKULL, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 1.5f );
	}

}
