#include "stdafx.h"
#include ".\HardDamageHommingMissle.h"







CHardDamageHommingMissle::CHardDamageHommingMissle(void)
{
}

CHardDamageHommingMissle::~CHardDamageHommingMissle(void)
{
}

void CHardDamageHommingMissle::AngleFrameMove( CDamageInstance* pDamageInstance )
{
    D3DXVECTOR2 vDamage( pDamageInstance->m_X, pDamageInstance->m_Y );
    D3DXVECTOR2 vPlayer( g_MyD3D->MyPlayer[1]->vPos.x, g_MyD3D->MyPlayer[1]->vPos.y + 0.15f );

    if( abs(pDamageInstance->m_X - g_MyD3D->MyPlayer[1]->vPos.x) >= 0.175f )
    {
        if( vDamage.x <= vPlayer.x )
        {
            pDamageInstance->m_IsRight = true;
            D3DXVec2Subtract( &vDamage, &vPlayer, &vDamage );
        }
        else
        {
            pDamageInstance->m_IsRight = false;
            D3DXVec2Subtract( &vDamage, &vDamage, &vPlayer );
        }
        D3DXVec2Normalize( &vDamage, &vDamage );
        vPlayer.x = 1.0f;
        vPlayer.y = 0.0f;

        float cosAng = D3DXVec2Dot( &vDamage, &vPlayer );
        float fPrevAng = pDamageInstance->m_Angle;
        float fTargetAng = (vDamage.y*vPlayer.x + vDamage.x*vPlayer.y > 0.0f) ? acosf(cosAng) : -acosf(cosAng);
        m_Angle_Speed = ( fTargetAng - fPrevAng ) * 0.1f;
    }

    float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
    float Angle = acosf( m_x_Speed / Length );

    if( pDamageInstance->m_IsRight )
    {
        pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
        pDamageInstance->m_Angle += m_Angle_Speed;
        pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
    }
    else
    {
        pDamageInstance->m_X -= cosf(pDamageInstance->m_Angle + Angle) * Length;
        pDamageInstance->m_Angle += m_Angle_Speed;
        pDamageInstance->m_Y -= sinf(pDamageInstance->m_Angle + Angle) * Length;
    }

    if( pDamageInstance->m_Angle < -(D3DX_PI * 2.0f) )
        pDamageInstance->m_Angle = 0.0f;
    if( pDamageInstance->m_Angle > (D3DX_PI * 2.0f) )
        pDamageInstance->m_Angle = 0.0f;
}

void CHardDamageHommingMissle::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );

    D3DXVECTOR2 vDamage( pDamageInstance->m_X, pDamageInstance->m_Y );
    D3DXVECTOR2 vPlayer( g_MyD3D->MyPlayer[1]->vPos.x, g_MyD3D->MyPlayer[1]->vPos.y + 0.15f );

    if( vDamage.x <= vPlayer.x )
    {
        pDamageInstance->m_IsRight = true;
        D3DXVec2Subtract( &vDamage, &vPlayer, &vDamage );
    }
    else
    {
        pDamageInstance->m_IsRight = false;
        D3DXVec2Subtract( &vDamage, &vDamage, &vPlayer );
    }
    D3DXVec2Normalize( &vDamage, &vDamage );
    vPlayer.x = 1.0f;
    vPlayer.y = 0.0f;

    float cosAng = D3DXVec2Dot( &vDamage, &vPlayer );
    pDamageInstance->m_Angle = (vDamage.y*vPlayer.x + vDamage.x*vPlayer.y > 0.0f) ? acosf(cosAng) : -acosf(cosAng);
}