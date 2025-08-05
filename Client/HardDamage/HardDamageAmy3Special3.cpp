#include "stdafx.h"
#include ".\HardDamageAmy3Special3.h"
#include "Stage.h"



CHardDamageAmy3Special3::CHardDamageAmy3Special3(void)
{
}

CHardDamageAmy3Special3::~CHardDamageAmy3Special3(void)
{
}


void CHardDamageAmy3Special3::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
    CDamage::NotAngleFrameMove( pDamageInstance );

	switch ( pDamageInstance->m_What )
	{
	case DT_JURIOR_BACKCOLOR1:
		g_MyD3D->MyStg->Color_r = 0;
		g_MyD3D->MyStg->Color_g = 150;
		g_MyD3D->MyStg->Color_b = 255;
		break;
	default:
		g_MyD3D->MyStg->Color_r = 0;
		g_MyD3D->MyStg->Color_g = 0;
		g_MyD3D->MyStg->Color_b = 0;
		break;
	}
}
