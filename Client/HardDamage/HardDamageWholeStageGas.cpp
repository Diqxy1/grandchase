#include "stdafx.h"
#include ".\HardDamageWholeStageGas.h"
#include "Stage.h"
#include "DamageManager.h"

CHardDamageWholeStageGas::CHardDamageWholeStageGas(void)
{
}

CHardDamageWholeStageGas::~CHardDamageWholeStageGas(void)
{
}

void CHardDamageWholeStageGas::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

    // x축 전체에 가스를 뿌린다.
    // 1.5f = 512 1장 1.5f
    // 1.5 한장에 5개가 들어감
    int GasNum = (int)(g_MyD3D->MyStg->GetXLimit() * 0.033f);

    // 가스 한덩이
    for( int i = 0; i < GasNum; ++i )
        g_MyD3D->m_pDamageManager->AddWithLocate( DT_CASTUL_TRAP_GAS2, MAX_PLAYER_NUM, pDamageInstance->m_X + i*0.3f, pDamageInstance->m_Y );
}