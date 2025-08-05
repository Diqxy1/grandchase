#include "stdafx.h"
#include ".\HardDamageStarDustRain.h"





//====================================================================================
// 2009.02.11 : Jemitgge
// Comment : ���� �������� ����ó�� ������ ��� �ϴ� ���̸� ������� �ߴ�.
//			 ���� �Ƹ��ٿ� ����� �������� ������ ���⺤��(������ �������� ������ ���ؾ� �Ѵ�)�� ��� ���Ͽ���
//			 ������ ��ü�� ������ ��ȭ���Ѹ� ��������... �ð��� ���ٴ� �ΰ踦 �ϰڴ�..
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
	// Comment : SPEED_X = 0.04, SPEED_Y = 0.05 - ���� ������
	//			 SPEED_X = 0.05, SPEED_Y = 0.08 - �߰� ������
	//			 SPEED_X = 0.06, SPEED_Y = 0.11 - �� ������

	// SPEED_X �� 0���� ����~
	if ( pDamageInstance->m_pOriginalDamage->m_x_Speed >= 0.01f )
	{
		pDamageInstance->m_pOriginalDamage->m_x_Speed -= 0.001f;
	}

	// SPEED_Y �� ������ �ϰ� ���ǵ�� ����~
	if ( pDamageInstance->m_pOriginalDamage->m_y_Speed > -0.06f )
	{
		// 0.005�� �߷°� �䳻������...
		pDamageInstance->m_pOriginalDamage->m_y_Speed -= 0.005f;
	}
}

