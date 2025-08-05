#include "stdafx.h"
#include ".\HardDamageElesis4DownAtk.h"




CHardDamageElesis4DownAtk::CHardDamageElesis4DownAtk(void)
{
}

CHardDamageElesis4DownAtk::~CHardDamageElesis4DownAtk(void)
{
}

void CHardDamageElesis4DownAtk::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.bLive == false ||
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_ELESIS4_JUMP_ATK3 )
    {
        pDamageInstance->m_Life = 1;
    }
    else
    {
        pDamageInstance->m_Life = 255;
    }
}
