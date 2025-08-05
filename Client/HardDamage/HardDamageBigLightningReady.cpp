#include "stdafx.h"
#include "DamageManager.h"
#include ".\HardDamageBigLightningReady.h"

static int siIndex[MAX_PLAYER_NUM] = {0,0,0,0,0,0,}; 
static D3DXVECTOR2 svPlayerPos[MAX_PLAYER_NUM];

CHardDamageBigLightningReady::CHardDamageBigLightningReady(void)
{
    m_iFrameCount = 0;
    m_iTargetCount = 0;
}

CHardDamageBigLightningReady::~CHardDamageBigLightningReady(void)
{
}

#define LIGHTNING_NUM 5
void CHardDamageBigLightningReady::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );

    m_iFrameCount = 0;
    m_iTargetCount = 1;
    bRight = true;
    m_iWho = pDamageInstance->m_Who;

    float fPosX[LIGHTNING_NUM] = { -0.4f, 0.4f, -0.25f , 0.25f, 0.0f } ;
    float fPosY[LIGHTNING_NUM] = { 0.48f , 0.48f, 0.55f , 0.55f, 0.58f} ;
    float fDelta = 0.1f;

    if ( !pDamageInstance->m_IsRight )
    {
        fDelta *= -1;
    }
    
    //switch(siIndex)
    //{
    //case 0:case 4:
    //    m_iTargetCount = 10;
    //	break;
    //case 1:case 3:
    //    m_iTargetCount = 25;
    //    break;
    //case 2:
    //    m_iTargetCount = 35;
    //    break;
    //default:
    //    break;
    //}

    m_iTargetCount = 7;
    if ( siIndex[pDamageInstance->m_Who] == 0 )
    {        
        svPlayerPos[pDamageInstance->m_Who] = (D3DXVECTOR2)g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos;
    }
    pDamageInstance->m_X = (float)(svPlayerPos[pDamageInstance->m_Who].x + fPosX[siIndex[pDamageInstance->m_Who]] + fDelta);
    pDamageInstance->m_Y = (float)(svPlayerPos[pDamageInstance->m_Who].y + fPosY[siIndex[pDamageInstance->m_Who]]);
    
    
    siIndex[pDamageInstance->m_Who]++;
    if (siIndex[pDamageInstance->m_Who] >= LIGHTNING_NUM)
    {
        siIndex[pDamageInstance->m_Who] = 0;
    }
    
}

void CHardDamageBigLightningReady::End( CDamageInstance* pDamageInstance )
{
    siIndex[pDamageInstance->m_Who] = 0;
    CDamage::End( pDamageInstance );
}

void CHardDamageBigLightningReady::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    //법진 생성후 잠시 있다가 번개가 나갑니다. 그리고 번개 마다 시간이 다릅니다.
    ++m_iFrameCount;
    if( m_iFrameCount == m_iTargetCount )
    {    
        CDamageInstance* pkBigLightning = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL2_BLACK, pDamageInstance->m_Who );
        CDamageInstance* pkBigLightning2 = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL2_BLACK_PARTICLE1, pDamageInstance->m_Who );
        CDamageInstance* pkBigLightning3 = g_MyD3D->m_pDamageManager->Add( DT_ARME4_SPECIAL2_BLACK_PARTICLE2, pDamageInstance->m_Who );
        pkBigLightning->m_X =  pkBigLightning2->m_X = pkBigLightning3->m_X =  pDamageInstance->m_X;
        pkBigLightning->m_Y =  pkBigLightning2->m_Y = pkBigLightning3->m_Y = pDamageInstance->m_Y;
    }
}