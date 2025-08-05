#include "stdafx.h"
#include ".\HardDamageAmyNoteSinFly.h"




CHardDamageAmyNoteSinFly::CHardDamageAmyNoteSinFly(void)
{
	m_fStartY = 0.0f;
	m_iTime   = 0;
    m_fOffsetY = 0.0f;
}

CHardDamageAmyNoteSinFly::~CHardDamageAmyNoteSinFly(void)
{
}


void CHardDamageAmyNoteSinFly::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	if (m_iTime == 0)
	{       
		m_fStartY = pDamageInstance->m_Y;  
        m_fOffsetY = 0.0f;
	}	

	if( pDamageInstance->m_IsRight )
	{
		pDamageInstance->m_X += m_x_Speed;		
	}
	else
	{
		pDamageInstance->m_X -= m_x_Speed;        
	}


    if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK1_NOTE1 || 
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK1_NOTE2 ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK2_NOTE1 || 
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK2_NOTE2 ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK3_NOTE2 ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK4_NOTE1 || 
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK4_NOTE2 ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE1 || 
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE2 ||
         pDamageInstance->m_What == DT_AMY2_SPECIAL4_BIG_NOTE1 ||         
         pDamageInstance->m_What == DT_AMY2_SPECIAL4_SMALL_NOTE1 || 
         pDamageInstance->m_What == DT_AMY2_SPECIAL4_SMALL_NOTE1_L ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK1_NOTE1_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK1_NOTE2_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK2_NOTE1_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK2_NOTE2_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK3_NOTE2_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK4_NOTE1_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_ATK4_NOTE2_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE1_DUN ||
         pDamageInstance->m_What == DT_AMY2_PERFORMENCE_CRITICALDOUBLE_ATK_NOTE2_DUN )
    {
        float fDiff = sinf( (float)m_iTime / (D3DX_PI*2) );

        pDamageInstance->m_Y = m_fStartY + (fDiff / 10.0f);

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            if (!o3)
                pDamageInstance->CreateSequence("Amy02_Attack_Note01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_DASH_ATK_NOTE1 || 
              pDamageInstance->m_What == DT_AMY2_PERFORMENCE_DASH_ATK_NOTE2 ||
              pDamageInstance->m_What == DT_AMY2_PERFORMENCE_DASH_ATK_NOTE1_DUN || 
              pDamageInstance->m_What == DT_AMY2_PERFORMENCE_DASH_ATK_NOTE2_DUN )
    {
        float fDiff = sinf( (float)m_iTime / (D3DX_PI*2) );

        pDamageInstance->m_Y = m_fStartY + (fDiff / 10.0f);

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            if (!o3)
                pDamageInstance->CreateSequence("Amy02_Backattack_Note03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }
    else if ( pDamageInstance->m_What == DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE1 || 
              pDamageInstance->m_What == DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE2 ||
              pDamageInstance->m_What == DT_AMY2_SPECIAL4_BIG_NOTE2 || 
              pDamageInstance->m_What == DT_AMY2_SPECIAL4_SMALL_NOTE2 ||
              pDamageInstance->m_What == DT_AMY2_SPECIAL4_SMALL_NOTE2_L ||
              pDamageInstance->m_What == DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE1_DUN || 
              pDamageInstance->m_What == DT_AMY2_PERFORMENCE_JUMPDOWN_ATK_NOTE2_DUN )
    {
        float fTheta = 45.0f*D3DX_PI/180.f;
        float fDiff = sinf( (float)m_iTime / (D3DX_PI*2) );

        m_fOffsetY += 0.007f;

        pDamageInstance->m_Y = m_fStartY + (fDiff / 10.0f) - m_fOffsetY;

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            if (!o3)
                pDamageInstance->CreateSequence("Amy02_Attack_Note01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }
    else if ( pDamageInstance->m_What == DT_AMY2_SPECIAL4_BIG_NOTE3 || 
              pDamageInstance->m_What == DT_AMY2_SPECIAL4_SMALL_NOTE3 ||
              pDamageInstance->m_What == DT_AMY2_SPECIAL4_SMALL_NOTE3_L )
    {
        float fTheta = 45.0f*D3DX_PI/180.f;
        float fDiff = sinf( (float)m_iTime / (D3DX_PI*2) );

        m_fOffsetY += 0.007f;

        pDamageInstance->m_Y = m_fStartY + (fDiff / 10.0f) + m_fOffsetY;

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            if (!o3)
                pDamageInstance->CreateSequence("Amy02_Attack_Note01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }
    else if ( pDamageInstance->m_What == DT_AMY2_FIGHT_UNIQUE_EFFECT )
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            if (!o3)
                pDamageInstance->CreateSequence("Amy02_Move", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
    }
	else if( pDamageInstance->m_What == DT_AMY3_DANCE_SPECIAL1_ATK )
	{
		float fDiff = sinf( (float)m_iTime / (D3DX_PI*2) );
		pDamageInstance->m_Y = m_fStartY + (fDiff / 10.0f);

        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
            if (!o3)
            {
                pDamageInstance->CreateSequence("Amy03_special2-1_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
                pDamageInstance->CreateSequence("Amy03_special2-1_06", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
            }
	}
    

    m_iTime++;               //1프레임을 1초로 계산하자.
}
