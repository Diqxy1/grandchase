#include "stdafx.h"
#include ".\HardDamageLas4DownAtk.h"



//

CHardDamageLas4DownAtk::CHardDamageLas4DownAtk(void)
{
}

CHardDamageLas4DownAtk::~CHardDamageLas4DownAtk(void)
{
}

void CHardDamageLas4DownAtk::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.bLive == false ||
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != MID_LAS4_JUMP_ATK )
    {
        pDamageInstance->m_Life = 0;
    }
    else
    {
        if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame > 40 )
        {
            pDamageInstance->m_Life = 0;
            return;
        }

        pDamageInstance->m_Life = 255;
        pDamageInstance->m_pOriginalDamage->m_iCrashStart = 255;
        pDamageInstance->m_pOriginalDamage->m_iCrashEnd = 0;
    }

    if( pDamageInstance->m_pOriginalDamage->m_Y_per_x > 0.3f )
    {
        pDamageInstance->m_pOriginalDamage->m_Y_per_x -= 0.2f;
        pDamageInstance->m_pOriginalDamage->m_Start_Locate_y -= 0.06f;
    }
}
