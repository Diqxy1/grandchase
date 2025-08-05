#include "stdafx.h"
#include ".\HardDamageSummonDragon.h"
#include "DamageManager.h"
#include "KGC3DObject.h"


//


CHardDamageSummonDragon::CHardDamageSummonDragon(void)
{
    
}

CHardDamageSummonDragon::~CHardDamageSummonDragon(void)
{
}

void CHardDamageSummonDragon::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
}

void CHardDamageSummonDragon::FrameMove( CDamageInstance* pDamageInstance )
{
	if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) && pDamageInstance->m_p3DObject != NULL )
		pDamageInstance->m_p3DObject->SetRender(false);

    CDamage::FrameMove( pDamageInstance );

    unsigned short OriginalFrameNumber = pDamageInstance->m_p3DObject->GetFrame();

    if ( OriginalFrameNumber == 176 )
    {
        CDamageInstance* pDamage = g_MyD3D->m_pDamageManager->AddWithLocate( DT_RONAN2_MAGIC_SPECIAL3_PARTICLE2, pDamageInstance->m_Who, pDamageInstance->m_X, pDamageInstance->m_Y );
        pDamage->m_IsRight = pDamageInstance->m_IsRight;
        g_KDSound.Play( "575" );

    }

    if ( OriginalFrameNumber < 60 || OriginalFrameNumber > 138 || ( OriginalFrameNumber % 3 != 0 ) )
    {
        if ( OriginalFrameNumber == 47 )
        {
            g_KDSound.Play( "574" );
        }
        return;
    }

    if ( OriginalFrameNumber == 61 )
    {
        g_KDSound.Play( "573" );
    }
    D3DXVECTOR3 vTarget;
    
    
    D3DXMATRIX matNeck;
    D3DXMATRIX matJaw;

    // 몬스터가 GCObject기 때문에 ㅡ 컬링될 가능성이 있다. 컬링 되면 모션 매트릭스를 계산 안하니까 ㅡ 강제로 한번 계산해준다.
    pDamageInstance->m_p3DObject->GetGCObject()->SetMotionMatrix( true, NULL , false , true );

    
    //불이 나와야 하는 위치를 구한다.
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 16, &matJaw );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 15, &matNeck );


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


    CDamageInstance* pkMeteo = g_MyD3D->m_pDamageManager->AddWithLocate(DT_RONAN2_DRAGON_BREATH, pDamageInstance->m_Who, vTarget.x + 1.0f + ( vBreathDir.x * fXOffset ),
                                                                            vTarget.y + 0.54f + ( vBreathDir.y * -0.19f ), false, pDamageInstance->m_ExtraMonsterNum );
    CDamageInstance* pkDeco = g_MyD3D->m_pDamageManager->AddWithLocate(DT_DRAGON_BREATH_DECO, pDamageInstance->m_Who, vTarget.x + 1.0f + ( vBreathDir.x * fXOffset ),
                                                                            vTarget.y + 0.54f + ( vBreathDir.y * -0.19f ), false, pDamageInstance->m_ExtraMonsterNum );
    if( !pkMeteo || !pkDeco )
        return;
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

    pkMeteo->m_IsRight = pDamageInstance->m_IsRight;
    pkDeco->m_IsRight = pDamageInstance->m_IsRight;
    pkMeteo->m_Scale_x	= pkMeteo->m_pOriginalDamage->m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.06f;	
    pkDeco->m_Scale_x	= pkDeco->m_pOriginalDamage->m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.18f;	
    
}

CHardDamageSummonBlackDragon::CHardDamageSummonBlackDragon(void)
{

}

CHardDamageSummonBlackDragon::~CHardDamageSummonBlackDragon(void)
{
}

void CHardDamageSummonBlackDragon::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
}

void CHardDamageSummonBlackDragon::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    unsigned short OriginalFrameNumber = pDamageInstance->m_p3DObject->GetFrame();

    if ( OriginalFrameNumber < 161 )
    {
        return;
    }

    if ( OriginalFrameNumber > 191 && ( OriginalFrameNumber % 3 == 0 ) )
    {
        return;
    }

    if ( OriginalFrameNumber == 172 )
    {
        g_KDSound.Play( "574" );
    }

    if ( OriginalFrameNumber == 175 )
    {
        g_KDSound.Play( "573" );
    }
    D3DXVECTOR3 vTarget;


    D3DXMATRIX matNeck;
    D3DXMATRIX matJaw;

    // 몬스터가 GCObject기 때문에 ㅡ 컬링될 가능성이 있다. 컬링 되면 모션 매트릭스를 계산 안하니까 ㅡ 강제로 한번 계산해준다.
    pDamageInstance->m_p3DObject->GetGCObject()->SetMotionMatrix( true, NULL , false , true );


    //불이 나와야 하는 위치를 구한다.
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 16, &matJaw );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 15, &matNeck );


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


    CDamageInstance* pkMeteo = g_MyD3D->m_pDamageManager->MonsterAddWithLocate(DT_MONSTER_ZIG_DRAGON_BREATH, pDamageInstance->m_ExtraMonsterNum, vTarget.x + 1.00f + ( vBreathDir.x * fXOffset ),
        vTarget.y + 0.5f + ( vBreathDir.y * -0.25f ), false );
    CDamageInstance* pkDeco = g_MyD3D->m_pDamageManager->MonsterAddWithLocate(DT_DRAGON_BREATH_DECO, pDamageInstance->m_ExtraMonsterNum, vTarget.x + 1.00f + ( vBreathDir.x * fXOffset ),
        vTarget.y + 0.5f + ( vBreathDir.y * -0.25f ), false );

	if( !pkMeteo || !pkDeco )
		return;

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

    pkMeteo->m_IsRight = pDamageInstance->m_IsRight;
    pkDeco->m_IsRight = pDamageInstance->m_IsRight;
    pkMeteo->m_Scale_x	= pkMeteo->m_pOriginalDamage->m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.06f;	
    pkDeco->m_Scale_x	= pkDeco->m_pOriginalDamage->m_Start_Scale_x + (0.5f - (float)rand() / (float)RAND_MAX) * 0.18f;	

}
