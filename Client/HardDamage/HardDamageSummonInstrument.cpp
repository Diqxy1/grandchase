#include "stdafx.h"
#include ".\HardDamageSummonInstrument.h"
#include "DamageManager.h"


//

#include "Monster.h"

CHardDamageSummonInstrument::CHardDamageSummonInstrument(void)
{
    m_iTime = 0;
    m_fWeight = 0.2f;
}

CHardDamageSummonInstrument::~CHardDamageSummonInstrument(void)
{
}

bool CHardDamageSummonInstrument::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    CDamage::CrashPlayer(pDamageInstance,hitWho,fDamageRate);    

    if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)){
        pDamageInstance->CreateSequence("Amy02_SPECIAL2-3_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        pDamageInstance->CreateSequence("Amy02_SPECIAL2-3_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        pDamageInstance->CreateSequence("Amy02_SPECIAL2-3_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }

    //g_KDSound.Play( "43" );
    D3DXVECTOR3 vecWho = g_MyD3D->MyPlayer[hitWho]->GetPosition();
    float temp = 0.0f;
    if( pDamageInstance->m_X >= 0 )
        temp = 0.05f;
    else
        temp = -0.05f;

    if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT1_MODEL)
    {
        if( pDamageInstance->m_X+temp >= vecWho.x )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT1_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT1_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
            pDmg->m_IsRight = true;
        }
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT2_MODEL)
    {        
        //if( pDamageInstance->m_X+temp >= vecWho.x )
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT2_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
            pDmg->m_IsRight = true;
        //else
           // g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT2_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y );
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT3_MODEL)
    {
        if( pDamageInstance->m_X+temp >= vecWho.x )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT3_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT3_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
            pDmg->m_IsRight = true;
        }
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT4_MODEL)
    {
        if( pDamageInstance->m_X+temp >= vecWho.x )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT4_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT4_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
            pDmg->m_IsRight = true;
        }
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT5_MODEL)
    {
        if( pDamageInstance->m_X+temp >= vecWho.x )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT5_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT5_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y);
            pDmg->m_IsRight = true;
        }
    }    

    pDamageInstance->m_Life = 1;    
    return true;
}

void CHardDamageSummonInstrument::CrashMonster(CDamageInstance* pDamageInstance, int Monsteri )
{
    CDamage::CrashMonster(pDamageInstance, Monsteri);

    if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
        pDamageInstance->CreateSequence("Amy02_SPECIAL2-3_spark01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        pDamageInstance->CreateSequence("Amy02_SPECIAL2-3_spark02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
        pDamageInstance->CreateSequence("Amy02_SPECIAL2-3_spark03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }

    //g_KDSound.Play( "43" );
	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if ( pMonster == NULL ) return;

    if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT1_MODEL)
    {

        if( pDamageInstance->m_X >= pMonster->m_afX[LATENCY_INDEX] )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT1_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT1_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT2_MODEL)
    {
        
        //if( pDamageInstance->m_X >= pMonster->m_afX[LATENCY_INDEX] )
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT2_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        //else
       //     g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT2_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT3_MODEL)
    {
        if( pDamageInstance->m_X >= pMonster->m_afX[LATENCY_INDEX] )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT3_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT3_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT4_MODEL)
    {
        if( pDamageInstance->m_X >= pMonster->m_afX[LATENCY_INDEX] )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT4_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT4_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
    }
    else if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT5_MODEL)
    {
        if( pDamageInstance->m_X >= pMonster->m_afX[LATENCY_INDEX] )
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT5_MOTION, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
        else
        {
            CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate( DT_AMY2_PERFORMENCE_INSTRUMENT5_MOTION_L, pDamageInstance->m_Who , pDamageInstance->m_X , pDamageInstance->m_Y , true );
            pDmg->m_IsRight = true;
        }
    }

    

    pDamageInstance->m_Life = 1;    
}

void CHardDamageSummonInstrument::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
}

void CHardDamageSummonInstrument::FrameMove( CDamageInstance* pDamageInstance )
{       
    CDamage::FrameMove( pDamageInstance );

    if( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT1_MODEL )
        m_fWeight = 0.2f;
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT2_MODEL )
        m_fWeight = 0.15f;
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT3_MODEL )
        m_fWeight = 0.22f;
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT4_MODEL )
        m_fWeight = 0.12f;
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT5_MODEL )
        m_fWeight = 0.25f;
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT_FINISH || pDamageInstance->m_What == DT_AMY2_PERFORMENCE_INSTRUMENT_FINISH_L )
        m_fWeight = 0.75f;
    
    pDamageInstance->m_Y -= (m_y_Speed*(float)m_iTime*m_fWeight);
    m_iTime++;
}
