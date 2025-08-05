#include "StdAfx.h"
#include "HardDamageChargedBolt.h"
#include "KGCRoomManager.h"
#include "mapsystem/GCFantasticMap.h"
#include "Stage.h"

HardDamageChargedBolt::HardDamageChargedBolt(void)
{
}

HardDamageChargedBolt::~HardDamageChargedBolt(void)
{
}

void HardDamageChargedBolt::FrameMove( CDamageInstance* pDamageInstance )
{

    float fPosY;
    if( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        D3DXVECTOR2 dxvCrashPos;
        SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y );
        fPosY = dxvCrashPos.y;
    }
    else
    {
        fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0 ).y;
    }

    KGCContectInfo kInfo( pDamageInstance->m_X, pDamageInstance->m_Y, pDamageInstance->m_pOriginalDamage->m_x_Speed, pDamageInstance->m_pOriginalDamage->m_x_Speed, false, false );
    if( true == g_MyD3D->MyStg->Check_Contact( kInfo ) )
    {
        pDamageInstance->m_Y = kInfo.m_y;
    }
    else
    {
        pDamageInstance->m_Life = 1;

        g_ParticleManager->CreateSequence( "Phantom_Lightning_End_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
        g_ParticleManager->CreateSequence( "Phantom_Lightning_End_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );

        //  가져와서 하는게 없음
//         DIRECTION_INFO dir;
//         dir.m_bDirVelocity = true;
//         dir.m_bIsRight = pDamageInstance->m_IsRight;
//         dir.m_bReverseTexture = true;
//         dir.m_pbIsRight = NULL;
//         pParticle1->GetDirectInfo( dir );
//         pParticle2->GetDirectInfo( dir );
    }

    return CDamage::FrameMove( pDamageInstance );
}