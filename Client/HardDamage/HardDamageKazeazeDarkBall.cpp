#include "stdafx.h"
#include ".\harddamagekazeazedarkball.h"
#include "DamageManager.h"
#include "Spark.h"


CHardDamageKazeazeDarkBall::CHardDamageKazeazeDarkBall(void)
{
	m_fBrokenTime = 0.0f;
}

CHardDamageKazeazeDarkBall::~CHardDamageKazeazeDarkBall(void)
{
}

void CHardDamageKazeazeDarkBall::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove(pDamageInstance);

	m_fBrokenTime += GC_ELAPSED_TIME;

	//변신 카제아제 암흑볼일때 일정시간후 작은 암흑볼로 분기한다
	if( m_fBrokenTime >= 0.7f )
	{
		pDamageInstance->Add( HIT_LIGHT, pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.25f, LATENCY + SHOWLATENCY, 2.0f, pDamageInstance->m_X - 0.9f, pDamageInstance->m_Y - 0.25f, 0 );

		int iAngle = 45;
		for( int i = 0; i < 4; ++i, iAngle += 90 )
		{
			CDamageInstance* pTempDamageInstance = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( DT_KAZEAZE_DARKBALL_CHILD, pDamageInstance->m_ExtraMonsterNum, pDamageInstance->m_X ,pDamageInstance->m_Y - 0.3f );
			pTempDamageInstance->m_Angle = D3DXToRadian(iAngle);
		}

		pDamageInstance->m_Life = 0;
	}

}