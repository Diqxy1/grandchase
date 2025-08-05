#include "stdafx.h"
#include ".\harddamageblockbuster.h"



#include "Spark.h"

CHardDamageBlockBuster::CHardDamageBlockBuster(void)
{
}

CHardDamageBlockBuster::~CHardDamageBlockBuster(void)
{
}


void CHardDamageBlockBuster::AngleFrameMove(CDamageInstance* pDamageInstance )
{
   float Length = ASMsqrt( m_x_Speed * m_x_Speed + m_y_Speed * m_y_Speed );
   float Angle = acosf( m_x_Speed / Length );

   if( pDamageInstance->m_Life > 100 )
   {
       if( pDamageInstance->m_IsRight )
       {
           if ( pDamageInstance->m_Angle + Angle < -1.0f )
           {
               pDamageInstance->m_X += cosf(-1.0) * Length;
               pDamageInstance->m_Y += sinf(-1.0) * Length;
           }
           else
           {
               pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
               pDamageInstance->m_Angle += m_Angle_Speed;
               pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
           }
       }
       else
       {
           if ( pDamageInstance->m_Angle + Angle > 1.0 )
           {
               pDamageInstance->m_X -= cosf(1.0) * Length;
               pDamageInstance->m_Y -= sinf(1.0) * Length;
           }
           else
           {
               pDamageInstance->m_X -= cosf(pDamageInstance->m_Angle + Angle) * Length;
               pDamageInstance->m_Angle -= m_Angle_Speed;
               pDamageInstance->m_Y -= sinf(pDamageInstance->m_Angle + Angle) * Length;
           }
       }
   }

	pDamageInstance->Add( DUST, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.2f + randf() * 0.3f );
}
