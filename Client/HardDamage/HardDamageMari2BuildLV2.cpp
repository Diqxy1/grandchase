#include "stdafx.h"
#include ".\HardDamageMari2BuildLV2.h"

CHardDamageMari2BuildLV2::CHardDamageMari2BuildLV2(void)
{
}

CHardDamageMari2BuildLV2::~CHardDamageMari2BuildLV2(void)
{
}

void CHardDamageMari2BuildLV2::AngleFrameMove(CDamageInstance* pDamageInstance )
{
	//CDamage::FrameMove(pDamageInstance);
    float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
    float Angle = acosf( m_x_Speed / Length );

    if( pDamageInstance->m_What != DT_ARROW 
        || pDamageInstance->m_Life > 100 )
    {
        if( pDamageInstance->m_IsRight )
        {
            pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
            pDamageInstance->m_Angle += m_Angle_Speed;
            pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
        }
        else
        {
            pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
            pDamageInstance->m_Angle -= m_Angle_Speed;
            pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
        }
    }	
}



CHardDamageMari2MagicBeadLv2::CHardDamageMari2MagicBeadLv2(void)
{
}

CHardDamageMari2MagicBeadLv2::~CHardDamageMari2MagicBeadLv2(void)
{
}

void CHardDamageMari2MagicBeadLv2::FrameMove(CDamageInstance* pDamageInstance)
{
    CDamage::FrameMove( pDamageInstance );

    if( pDamageInstance->m_Life == 1 )
        g_MyD3D->GetMyPlayer()->SetMariSummonState(false);

    if( g_MyD3D->GetMyPlayer()->GetMariSummonState() == false )
        pDamageInstance->m_Life = 0;
}
