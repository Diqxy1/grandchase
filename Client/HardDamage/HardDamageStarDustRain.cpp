#include "stdafx.h"
#include ".\HardDamageStarDustRain.h"





//====================================================================================
// 2009.02.11 : Jemitgge
// Comment : 땅에 떨어지는 폭죽처럼 포물선 운동을 하는 아이를 만드려고 했다.
//			 가장 아름다운 방법은 데미지에 적절한 방향벡터(적당한 간격으로 각도가 변해야 한다)를 계속 더하여서
//			 데미지 자체의 방향을 변화시켜면 좋겠지만... 시간이 없다는 핑계를 하겠다..
CHardDamageStarDustRain::CHardDamageStarDustRain(void)
{

}

CHardDamageStarDustRain::~CHardDamageStarDustRain(void)
{
}

void CHardDamageStarDustRain::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
	CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

	switch ( pDamageInstance->m_What )
	{
	case DT_STRADUST2:
	case DT_STRADUST4:
	case DT_STRADUST6:
	case DT_STRADUST8:
		pDamageInstance->m_IsRight = !pDamageInstance->m_IsRight;
		break;
	default:
		break;
	}
}

void CHardDamageStarDustRain::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );

	//====================================================================================
    // 2009.02.11 : Jemitgge
	// Comment : SPEED_X = 0.04, SPEED_Y = 0.05 - 작은 포물선
	//			 SPEED_X = 0.05, SPEED_Y = 0.08 - 중간 포물선
	//			 SPEED_X = 0.06, SPEED_Y = 0.11 - 긴 포물선

	// SPEED_X 는 0으로 수렴~
	if ( pDamageInstance->m_pOriginalDamage->m_x_Speed >= 0.01f )
	{
		pDamageInstance->m_pOriginalDamage->m_x_Speed -= 0.001f;
	}

	// SPEED_Y 는 적당한 하강 스피드로 수렴~
	if ( pDamageInstance->m_pOriginalDamage->m_y_Speed > -0.06f )
	{
		// 0.005는 중력값 흉내낸것임...
		pDamageInstance->m_pOriginalDamage->m_y_Speed -= 0.005f;
	}
}

