#include "stdafx.h"
#include ".\HardDamageElesis4Special2.h"

#include "../KGCPlayerElesis4.h"


#define DA_NOT(p) (0xFFFFFFFF - p)

CHardDamageElesis4Special2::CHardDamageElesis4Special2(void)
{
}

CHardDamageElesis4Special2::~CHardDamageElesis4Special2(void)
{
}

void CHardDamageElesis4Special2::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    // 칼 돌리고 있나?
    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.bLive == true &&
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion == MID_ELESIS4_SPECIAL2 )
    {
        if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == ELESIS4_SPECIAL2_SHOOT_FRAME + 20 )
        {
            pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.05f;
            pDamageInstance->m_pOriginalDamage->m_IsEnableDamage = false;
            pDamageInstance->m_pOriginalDamage->m_bAnimLoop = false;
            if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight == true )
                pDamageInstance->m_X += 0.0075f;
            else
                pDamageInstance->m_X -= 0.0075f;
            pDamageInstance->m_pOriginalDamage->m_dwDMAttribute &= DA_NOT( DA_TRACE_CREATOR );
            pDamageInstance->m_Life = 0;
        }
        else if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 90 )
        {
            // 변수 빌려쓰기 -_-
            if( pDamageInstance->m_pOriginalDamage->m_bAnimLoop == false )
            {
                pDamageInstance->m_pOriginalDamage->m_bAnimLoop = true;
                pDamageInstance->m_pOriginalDamage->m_Scale_Speed = 0.98f;
                pDamageInstance->m_pOriginalDamage->m_fAnimAngle = 25.0f;
            }

            if( pDamageInstance->m_pOriginalDamage->m_fAnimAngle > 0.0f )
            {
                pDamageInstance->m_pOriginalDamage->m_fAnimAngle -= 1.0f;
            }
            else
            {
                pDamageInstance->m_pOriginalDamage->m_Scale_Speed = 1.0f;
            }
        }

        // 발사 전에 죽으면 안되기 때문에
        if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame <= ELESIS4_SPECIAL2_SHOOT_FRAME + 20 && 
            pDamageInstance->m_ExtraMonsterNum == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame )
        {
            pDamageInstance->m_Life = pDamageInstance->m_Life++;
            if( pDamageInstance->m_pParticle != NULL )
                pDamageInstance->m_pParticle->SlowCount( true );
        }
        else pDamageInstance->m_pParticle->SlowCount( false );

        // 주인의 이전 프레임
        pDamageInstance->m_ExtraMonsterNum = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame;
    }
    else
    {
        if( pDamageInstance->m_pParticle != NULL )
            pDamageInstance->m_pParticle->SlowCount( false );
    }
}