#include "stdafx.h"
#include ".\HardDamageRonan4SummonValkyrie.h"
#include "KGCRoomManager.h"
#include "KGC3DObject.h"
#include "DamageManager.h"

CHardDamageRonan4SummonValkyrie::CHardDamageRonan4SummonValkyrie(void)
{	
	m_bIsLastMotion = false;
}

CHardDamageRonan4SummonValkyrie::~CHardDamageRonan4SummonValkyrie(void)
{
}

void CHardDamageRonan4SummonValkyrie::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );
}

void CHardDamageRonan4SummonValkyrie::FrameMove(CDamageInstance* pDamageInstance )
{
	if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) && pDamageInstance->m_p3DObject != NULL )
		pDamageInstance->m_p3DObject->SetRender(false);

	CDamage::FrameMove( pDamageInstance );

    if ( pDamageInstance->m_What == DT_RONAN4_WHITE_MAGIC_SPECIAL3_VALKYRIE && pDamageInstance->m_p3DObject != NULL )
    {    
        if ( MID_RONAN4_WHITE_MAGIC_SPECIAL3 != g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion && 
            SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )//&&
            //g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame < 125)
        {
            pDamageInstance->m_Life = 0;
        }
		if( pDamageInstance->m_p3DObject->GetFrame() == 40 )
		{
			g_KDSound.Play( "927" );
			if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_01", pDamageInstance->m_X - 0.85f, pDamageInstance->m_Y - 0.5f, 0.5f);
			else
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_01_R", pDamageInstance->m_X - 1.15f, pDamageInstance->m_Y - 0.5f, 0.5f);
		}
		else if( pDamageInstance->m_p3DObject->GetFrame() == 78 )
		{		
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			{
				if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
				{
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_01", pDamageInstance->m_X - 0.70f, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_02", pDamageInstance->m_X - 0.70f, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_03", pDamageInstance->m_X - 0.70f, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_04", pDamageInstance->m_X - 0.70f, pDamageInstance->m_Y - 0.5f, 0.5f);
				}
				else
				{
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_01", pDamageInstance->m_X - 1.30f, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_02", pDamageInstance->m_X - 1.30f, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_03", pDamageInstance->m_X - 1.30f, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_Shot_04_R", pDamageInstance->m_X - 1.30f, pDamageInstance->m_Y - 0.5f, 0.5f);
				}
			}
			g_KDSound.Play( "928" );
		}
		else if( pDamageInstance->m_p3DObject->GetFrame() == 80 )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_ARROW1, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);						
		}		
		else if( pDamageInstance->m_p3DObject->GetFrame() == 82 )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_ARROW2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
		}
		else if( pDamageInstance->m_p3DObject->GetFrame() == 84 )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_ARROW3, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);

		}
		else if( pDamageInstance->m_p3DObject->GetFrame() == 86 )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_ARROW4, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);

		}
		else if( pDamageInstance->m_p3DObject->GetFrame() == 88 )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_ARROW5, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);

		}
		else if( pDamageInstance->m_p3DObject->GetFrame() == 90 )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_ARROW6	, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);
		}
		else if(pDamageInstance->m_p3DObject->GetFrame() == 130 )
		{
			g_KDSound.Play( "929" );
			if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_01", pDamageInstance->m_X - 0.85f, pDamageInstance->m_Y - 0.5f, 0.5f);
			else
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_Arrow_01_R", pDamageInstance->m_X - 1.15f, pDamageInstance->m_Y - 0.5f, 0.5f);
		}
		else if(pDamageInstance->m_p3DObject->GetFrame() == 168 )
		{
			g_KDSound.Play( "931" );
			if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight() )
				g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_FINISH_R, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);			
			else
				g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN4_RONAN4_VALKYRIE_FINISH_L, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y);			

			float fXPos;
			if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight())
			{
				fXPos = -0.7f;
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_01", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_02", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
				}
			}
			else
			{
				fXPos = -1.3f;
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_01_R", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
					pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_02_R", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
				}
			}
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
				pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_03", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_04", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Ronan4_SPECIAL2-3_finisher_Start_05", pDamageInstance->m_X + fXPos, pDamageInstance->m_Y - 0.5f, 0.5f);
			}
		}
	}	

	if( pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW1 || 
		pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW2 ||
		pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW3 ||
		pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW4 ||
		pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW5 ||
		pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW6 )
	{
		if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			if( !o3 )
				pDamageInstance->CreateSequence( "Amy02_Attack_Note01", pDamageInstance->m_X - 1.0f,pDamageInstance->m_Y - 0.5f,0.5f );
	}

	if( (pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW1 && pDamageInstance->m_Life == 35) ||
		(pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW2 && pDamageInstance->m_Life == 37) ||
		(pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW3 && pDamageInstance->m_Life == 40) ||
		(pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW4 && pDamageInstance->m_Life == 40) ||
		(pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW5 && pDamageInstance->m_Life == 37) ||
		(pDamageInstance->m_What == DT_RONAN4_RONAN4_VALKYRIE_ARROW6 && pDamageInstance->m_Life == 35) )
	{
		pDamageInstance->m_Life = 0;
	}
}

