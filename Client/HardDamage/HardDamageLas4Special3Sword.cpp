#include "stdafx.h"
#include ".\HardDamageLas4Special3Sword.h"



//

#define DA_NOT(p) (0xFFFFFFFF - p)

CHardDamageLas4Special3Sword::CHardDamageLas4Special3Sword(void)
{
}

CHardDamageLas4Special3Sword::~CHardDamageLas4Special3Sword(void)
{
}

void CHardDamageLas4Special3Sword::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.bLive == false )
        return;

    if( !(pDamageInstance->m_Life >= pDamageInstance->m_pOriginalDamage->m_Start_Life - 3 || pDamageInstance->m_Life == 1) )
    {
        D3DXVECTOR3* pvSwordPos = ComputePosDesc( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_posDesc, EPT_SWORD_CENTER );
        if( pDamageInstance->m_What == DT_LAS4_SPECIAL3_SWORD_L ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_FIRE_SWORD_L ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_DARK_SWORD_L ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_LIGHT_SWORD_L ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_ICE_SWORD_L ||
			 pDamageInstance->m_What == DT_LAS4_SPECIAL3_LOVE_SWORD_L)
            pDamageInstance->m_X = pvSwordPos->x + 1.0f - ( (0.225f - pDamageInstance->m_Scale_x * 0.5f) * -1.0f);
        else if( pDamageInstance->m_What == DT_LAS4_SPECIAL3_SWORD_R ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_FIRE_SWORD_R ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_DARK_SWORD_R ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_ICE_SWORD_R ||
            pDamageInstance->m_What == DT_LAS4_SPECIAL3_LIGHT_SWORD_R ||
			 pDamageInstance->m_What == DT_LAS4_SPECIAL3_LOVE_SWORD_R)
            pDamageInstance->m_X = pvSwordPos->x + 1.0f - ( (0.225f - pDamageInstance->m_Scale_x * 0.5f) * 1.0f);

        pDamageInstance->m_Y = pvSwordPos->y + 0.5f;

        pDamageInstance->m_pOriginalDamage->m_dwDMAttribute &= DA_NOT( DA_TRACE_CREATOR );
        pDamageInstance->m_pOriginalDamage->m_iTracePosType = -1;
    }
    else
    {
        pDamageInstance->m_pOriginalDamage->m_dwDMAttribute |= DA_TRACE_CREATOR;
        pDamageInstance->m_pOriginalDamage->m_iTracePosType = EPT_SWORD_BODY;
    }

    if( pDamageInstance->m_Life >= 23 )
        return;

    pDamageInstance->m_Scale_x += 0.0147f;
    if( pDamageInstance->m_Scale_x >= 0.45f )
        pDamageInstance->m_Scale_x = 0.45f;
}

void CHardDamageLas4Special3Sword::UVOperation( CDamageInstance* pDamageInstance, int iVertexOffset, SLVERTEX*& v )
{
    v[iVertexOffset].tu = v[iVertexOffset+2].tu = 0.1f;
    v[iVertexOffset+1].tu = v[iVertexOffset+3].tu = 0.41f;

    if( pDamageInstance->m_Life >= 23 )
        return;

    float fGrow = ( 23 - pDamageInstance->m_Life ) * 0.0252f;

    v[iVertexOffset].tu = v[iVertexOffset+2].tu = 0.1f;
    v[iVertexOffset+1].tu = v[iVertexOffset+3].tu = 0.41f + fGrow;
}
