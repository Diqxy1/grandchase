#include "stdafx.h"
#include ".\HardDamageFire.h"
#include "DamageManager.h"


CHardDamageFire::CHardDamageFire(void)
{
}

CHardDamageFire::~CHardDamageFire(void)
{
}


void CHardDamageFire::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
    CDamage::NotAngleFrameMove(pDamageInstance);

    //물이 뿜어져 나온다면 매우 빨리 꺼진다.
    if( g_MyD3D->m_pDamageManager->IsExistDamage(DT_WATER_POLE) == TRUE )
    {
        pDamageInstance->m_Life += (pDamageInstance->m_pOriginalDamage->m_Life_Change * 15);
    }
}

bool CHardDamageFire::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    //항상 맞는게 아니라 버벅대야 자연스럽다.
    if ( (pDamageInstance->m_Life < 400 && pDamageInstance->m_Life > 300) || (pDamageInstance->m_Life < 200 && pDamageInstance->m_Life > 100) )
    {
        if( g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP )
            && g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP )
            && g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER ) )
            g_MyD3D->MyPlayer[hitWho]->Ah_Sound( 0 );

        // 2007/2/9. iridology. 네피림 슈퍼아머 테스트
        //bool bSuperAromor = true;
        //if( !bSuperAromor )
        //{
            m_iDamageMotion = MID_COMMON_DOWN_AND_STANDUP;
            g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(m_iDamageMotion, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
            g_MyD3D->MyPlayer[hitWho]->y_Speed = 0.03f;
        //}
        return true;
    }

    CDamage::CrashPlayer(pDamageInstance,hitWho,fDamageRate);

    //Damage에서 밀려나도록 - 플레이어를 밀어내는 역할.
    if(g_MyD3D->MyPlayer[hitWho]->vPos.x > pDamageInstance->m_X)
        g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.03f;
    else
        g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.03f;

    //매우 빨리 좀 꺼뜨려 주자.
    pDamageInstance->m_Life += (pDamageInstance->m_pOriginalDamage->m_Life_Change * 5);
    return true;
}
