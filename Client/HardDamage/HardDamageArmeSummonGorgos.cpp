#include "stdafx.h"
#include ".\HardDamageArmeSummonGorgos.h"


#include "KGCRoomManager.h"
#include "KGC3DObject.h"

//


#include "DamageManager.h"

CHardDamageArmeSummonGorgos::CHardDamageArmeSummonGorgos(void)
{	
}

CHardDamageArmeSummonGorgos::~CHardDamageArmeSummonGorgos(void)
{
}

void CHardDamageArmeSummonGorgos::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );
}

void CHardDamageArmeSummonGorgos::FrameMove(CDamageInstance* pDamageInstance )
{
	if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) && pDamageInstance->m_p3DObject != NULL )
		pDamageInstance->m_p3DObject->SetRender(false);

	CDamage::FrameMove( pDamageInstance );	

	unsigned short OriginalFrameNumber = pDamageInstance->m_p3DObject->GetFrame();

	if( pDamageInstance->m_What == DT_ARME_SKILL_W3_GORGOSFLAME_LV1 )
	{	
		if ( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) && 
            MID_SKILL_ARME1_W3_GORGOSFLAME_LV1 != g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion && 
			SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )//&&
			//g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame < 125)
		{
			pDamageInstance->m_Life = 0;
		}

		if( OriginalFrameNumber == 1)
		{
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			{
				pDamageInstance->CreateSequence("Arme01_Summons_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			}
		}
		else if( OriginalFrameNumber == 48 )
			g_KDSound.Play( "976" );		
		else if( OriginalFrameNumber == 80 )
			g_KDSound.Play( "977" );
		else if( OriginalFrameNumber == 180 )
			g_KDSound.Play( "978" );
		else if( OriginalFrameNumber == 187 )
		{
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			{
				pDamageInstance->CreateSequence("Arme01_Summons_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_04_Re", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_05_Re", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			}
		}

		if( OriginalFrameNumber < 80 || OriginalFrameNumber > 160 )
			return;			
	}
	else
	{
		if ( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) && 
            MID_SKILL_ARME1_W3_GORGOSFLAME_LV2 != g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion && 
			SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )//&&
			//g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame < 125)
		{
			pDamageInstance->m_Life = 0;
		}

		if( OriginalFrameNumber == 1 )
		{
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			{
				pDamageInstance->CreateSequence("Arme01_Summons_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_04", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_05", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			}
		}
		if( OriginalFrameNumber == 50 )	
			g_KDSound.Play( "977" );			
		else if( OriginalFrameNumber == 133 )
		{
			g_KDSound.Play( "978" );
			if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
			{
				pDamageInstance->CreateSequence("Arme01_Summons_01", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_02", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_03", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_04_Re", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
				pDamageInstance->CreateSequence("Arme01_Summons_05_Re", pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f);
			}
		}

		if( OriginalFrameNumber < 50 || OriginalFrameNumber > 105 )
			return;
	}
	

	D3DXVECTOR3 vTarget;


	D3DXMATRIX matNeck;
	D3DXMATRIX matJaw;

	// 몬스터가 GCObject기 때문에 ㅡ 컬링될 가능성이 있다. 컬링 되면 모션 매트릭스를 계산 안하니까 ㅡ 강제로 한번 계산해준다.
	pDamageInstance->m_p3DObject->GetGCObject()->SetMotionMatrix( true, NULL , false , true );


	//불이 나와야 하는 위치를 구한다.
	pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 17, &matJaw );
	pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 16, &matNeck );


	D3DXVECTOR2 vBreathDir(matNeck._41 - matJaw._41 ,matNeck._42 - matJaw._42);
	D3DXVECTOR2 vHorizonDir(1.0f,0.0f);
	float fXOffset;
	if ( pDamageInstance->m_IsRight )
	{
		vHorizonDir = D3DXVECTOR2(1.0f,0.0f);
		fXOffset = -0.19f;
	}
	else
	{
		vHorizonDir = D3DXVECTOR2(-1.0f,0.0f);
		fXOffset = 0.19f;

	}
	D3DXVec2Normalize(&vBreathDir,&vBreathDir);

	float fcosTheta = vBreathDir.x * vHorizonDir.x + vBreathDir.y * vHorizonDir.y;

	float fAngle = acos( fcosTheta );

	matJaw *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
	vTarget.x = matJaw._41;
	vTarget.y = matJaw._42;
	vTarget.z = matJaw._43;

	CDamageInstance* pkMeteo;

	float fEfffPosX = 0.0f;

	if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight() )
		fEfffPosX = 0.88f;
	else
		fEfffPosX = 1.1f;

	if( pDamageInstance->m_What == DT_ARME_SKILL_W3_GORGOSFLAME_LV1 )
	{		
		pkMeteo = g_MyD3D->m_pDamageManager->AddWithLocate(DT_ARME_SKILL_GORGOSFLAME_LV1_BREATH, pDamageInstance->m_Who, vTarget.x + fEfffPosX + ( vBreathDir.x * fXOffset ),
			vTarget.y + 0.52f + ( vBreathDir.y * -0.19f ), true );
	}	
	else
	{
		pkMeteo = g_MyD3D->m_pDamageManager->AddWithLocate(DT_ARME_SKILL_GORGOSFLAME_BREATH, pDamageInstance->m_Who, vTarget.x + fEfffPosX + ( vBreathDir.x * fXOffset ),
			vTarget.y + 0.52f + ( vBreathDir.y * -0.19f ), true );
	}

	CDamageInstance* pkDeco = g_MyD3D->m_pDamageManager->AddWithLocate(DT_DRAGON_BREATH_DECO, pDamageInstance->m_Who, vTarget.x + fEfffPosX + ( vBreathDir.x * fXOffset ),
		vTarget.y + 0.52f + ( vBreathDir.y * -0.19f ), true );

	//	다단히트라서 Damage 갯수가 512가 넘을때가 가끔있다..
	//	일단 한두대 정도는 없어도 되겠지...
	if ( !pkMeteo || !pkDeco )
		return;

	if( pDamageInstance->m_What == DT_ARME_SKILL_W3_GORGOSFLAME_LV1 )
	{
		if ( pDamageInstance->m_IsRight )
		{
			pkMeteo->m_Angle = fAngle + D3DX_PI + 0.55f;        
		}
		else
		{
			pkMeteo->m_Angle = fAngle - 0.55f;        
		}

		pkDeco->m_Angle = pkMeteo->m_Angle + (0.5f - (float)rand() / (float)RAND_MAX);
		pkMeteo->m_Angle = pkMeteo->m_Angle + (0.5f - (float)rand() / (float)RAND_MAX) * 0.09f;
	}	
	
	if( pDamageInstance->m_What == DT_ARME_SKILL_W3_GORGOSFLAME_LV1 )
	{
		pkMeteo->m_IsRight = pDamageInstance->m_IsRight;
	}
	else
	{
		pkMeteo->m_IsRight = !pDamageInstance->m_IsRight;
	}
	
	pkDeco->m_IsRight = pDamageInstance->m_IsRight;
	pkMeteo->m_Scale_x	= pkMeteo->m_pOriginalDamage->m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.06f;	
	pkDeco->m_Scale_x	= pkDeco->m_pOriginalDamage->m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.18f;	
}

