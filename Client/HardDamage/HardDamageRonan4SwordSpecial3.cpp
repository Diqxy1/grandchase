#include "stdafx.h"
#include ".\HardDamageRonan4SwordSpecial3.h"



#include "KGC3DObject.h"

//



CHardDamageRonan4SwordSpecial3::CHardDamageRonan4SwordSpecial3(void)
{	
	m_elapsedTime = 0;
	m_isOnParticle[0] = false; 
	m_isOnParticle[1] = false;
	m_isOnParticle[2] = false;
}

CHardDamageRonan4SwordSpecial3::~CHardDamageRonan4SwordSpecial3(void)
{
}

void CHardDamageRonan4SwordSpecial3::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );
}

void CHardDamageRonan4SwordSpecial3::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );	

	if( pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_1 || 
		pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_1_L )
	{		
		CountBonePosWithTrace(pDamageInstance, 0.0f, -0.05f);		
		if( m_elapsedTime == 1 )
		{
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			{
				pDamageInstance->CreateSequence("Ronan4_SPECIAL1-3_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Ronan4_SPECIAL1-3_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Ronan4_SPECIAL1-3_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Ronan4_SPECIAL1-3_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			}
		}

		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 12 )
		{
			if (m_isOnParticle[0] == false)
			{
				CreateParticle(pDamageInstance, "Ronan4_SPECIAL1-3_Sword01_01", pTemp1);
				CreateParticle(pDamageInstance, "Ronan4_SPECIAL1-3_Sword01_02", pTemp2);
				m_isOnParticle[0] = true;
			}
			
		}				
		else if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 13 )
		{
			DeleteParticle();
		}		
		else if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 14 )
		{		
			if (pDamageInstance->m_p3DObject == NULL)
				return;

			pDamageInstance->m_p3DObject->SetMotionFromIndex(1);

			if( m_isOnParticle[2] == false)
			{
				if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
				{
					TRACE_INFO stTrace;
					if (pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_1)
						pSwordCenter = pDamageInstance->CreateSequence("Ronan4_SPECIAL1-3_Sword03_01", 0.0f, 0.0f, 0.5f);
					else
						pSwordCenter = pDamageInstance->CreateSequence("Ronan4_SPECIAL1-3_Sword03_01_R", 0.0f, 0.0f, 0.5f);

					if (pSwordCenter != NULL) {
						stTrace.m_fParticleTraceTime = 5.0f;
						stTrace.m_pvPos = &m_vSwordPos[9];
						pSwordCenter->SetTrace(&stTrace);
						m_isOnParticle[2] = true;
					}
				}
				
			}			
		}
		else if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 42 )
		{
			DeleteParticle();
			pDamageInstance->m_Life = 0;
		}
	}
	else if( pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_2 ||
			 pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_2_L)
	{
		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight() )
			CountBonePosWithTrace(pDamageInstance, -0.03f, -0.05f);
		else
			CountBonePosWithTrace(pDamageInstance, 0.00, 0.00);
		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame < 44 )
		{
			if( m_isOnParticle[1] == false )
			{
				if (pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_2)
				{
					CreateParticle(pDamageInstance, "Ronan4_SPECIAL1-3_Sword02_01", pTemp1);
					CreateParticle(pDamageInstance, "Ronan4_SPECIAL1-3_Sword02_02", pTemp2);
					m_isOnParticle[1] = true;
				}
				else
				{
					CreateParticle(pDamageInstance, "Ronan4_SPECIAL1-3_Sword02_01_R", pTemp1);
					CreateParticle(pDamageInstance, "Ronan4_SPECIAL1-3_Sword02_02_R", pTemp2);
					m_isOnParticle[1] = true;
				}
			}			
		}
		else if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 44 )
			pDamageInstance->m_p3DObject->SetMotionFromIndex(1);
	}	


	if( pDamageInstance->m_What == DT_RONAN4_MAGIC_SWORD_SPECIAL3_1_DMG1 )
	{
		if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 30 )
		{
			pDamageInstance->m_Life = 0;
		}
	}	

	if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame == 105 )
	{
		pDamageInstance->m_Life = 0;
		DeleteParticle();
	}

	m_elapsedTime++;
}

void CHardDamageRonan4SwordSpecial3::End( CDamageInstance* pDamageInstance )
{
	DeleteParticle();
	CDamage::End( pDamageInstance );
}

void CHardDamageRonan4SwordSpecial3::CountBonePosWithTrace(CDamageInstance* pDamageInstance, float _x, float _y, float _z )
{
	KSafeArray<D3DXMATRIX,10> matSword;

	pDamageInstance->m_p3DObject->GetGCObject()->SetMotionMatrix( true, NULL , false , true );

	for( int i = 0; i < 9; i++ )
	{
		pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 3*(i+1), &matSword[i] );
		matSword[i] *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
		m_vSwordPos[i].x = matSword[i]._41 + _x; m_vSwordPos[i].y = matSword[i]._42 + _y; m_vSwordPos[i].z = matSword[i]._43 + _z;
	}			

	pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 26, &matSword[9] );
	matSword[9] *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
	m_vSwordPos[9].x = matSword[9]._41 + _x; m_vSwordPos[9].y = matSword[9]._42 + _y; m_vSwordPos[9].z = matSword[9]._43 + _z;
}

void CHardDamageRonan4SwordSpecial3::CreateParticle( CDamageInstance* pDamageInstance, std::string name, KSafeArray<CParticleEventSeqPTR,9>& pTemp )
{
	if (g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
		return;

	TRACE_INFO stTrace;

	if( pDamageInstance->m_p3DObject->GetFrame() == 0 )
	{
		for( int i = 0; i < 9 ; i++ )
		{
			pTemp[i] = pDamageInstance->CreateSequence( name, 0.0f, 0.0f,0.5f );	
            if (pTemp[i] != NULL){
                stTrace.m_fParticleTraceTime = 5.0f;
                stTrace.m_pvPos = &m_vSwordPos[i];		
                pTemp[i]->SetTrace( &stTrace );
            }
			
		}	
	}				
}

void CHardDamageRonan4SwordSpecial3::DeleteParticle()
{
	for( int i = 0 ; i < 9 ; i++)
	{
		if( pTemp1[i] )
			g_ParticleManager->DeleteSequence( pTemp1[i] );
		if( pTemp2[i] )
			g_ParticleManager->DeleteSequence( pTemp2[i] );
		pTemp1[i] = NULL;
		pTemp2[i] = NULL;
	}
	
	if( pSwordCenter )
		g_ParticleManager->DeleteSequence( pSwordCenter );
	
	pSwordCenter = NULL;
}