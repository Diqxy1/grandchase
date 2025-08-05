#include "stdafx.h"
#include "Controls.h"
#include ".\HardDamageChaos3.h"
#include "Controls.h"

CHardDamageBindWeb::CHardDamageBindWeb( void )
{

}

CHardDamageBindWeb::~CHardDamageBindWeb( void )
{

}

void CHardDamageBindWeb::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	return;
}

bool CHardDamageBindWeb::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    PLAYER* pPlayer = g_MyD3D->MyPlayer[hitWho];

    if ( pPlayer->IsLocalPlayer() && pPlayer->m_kUserInfo.bLive )
    {
        g_MyD3D->MyCtrl->k_Right = false;
        g_MyD3D->MyCtrl->k_Left = false;
        g_MyD3D->MyCtrl->k_Fresh_Right = false;
        g_MyD3D->MyCtrl->k_Fresh_Left = false;
        g_MyD3D->MyCtrl->Right_Dash_Count = 0;
        g_MyD3D->MyCtrl->Left_Dash_Count = 0;
        g_MyD3D->MyCtrl->k_Fresh_Right_Dash = false;
        g_MyD3D->MyCtrl->k_Fresh_Left_Dash = false;
    }

	return true;
}