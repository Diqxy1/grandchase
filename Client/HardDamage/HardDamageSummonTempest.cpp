#include "stdafx.h"
#include ".\HardDamageSummonTempest.h"
#include "DamageManager.h"
#include "KGC3DObject.h"


//



CHardDamageSummonTempest::CHardDamageSummonTempest(void)
{
    bRight           = true;
    bRenderParticle  = true;
}

CHardDamageSummonTempest::~CHardDamageSummonTempest(void)
{
}

void CHardDamageSummonTempest::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );
}

void CHardDamageSummonTempest::FrameMove( CDamageInstance* pDamageInstance )
{
	if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) && pDamageInstance->m_p3DObject != NULL )
		pDamageInstance->m_p3DObject->SetRender(false);

    CDamage::FrameMove( pDamageInstance );

    unsigned short OriginalFrameNumber = pDamageInstance->m_p3DObject->GetFrame();

    if ( pDamageInstance->m_IsRight )
    {
        bRight = true;
    }
    else
    {
        bRight = false;
    }

    D3DXMATRIX matTail;
    D3DXMATRIX matHead;
    D3DXMATRIX matWaist;
    D3DXMATRIX matArticulationL;
    D3DXMATRIX matArticulationR;
    D3DXMATRIX matLance;

    // 몬스터가 GCObject기 때문에 ㅡ 컬링될 가능성이 있다. 컬링 되면 모션 매트릭스를 계산 안하니까 ㅡ 강제로 한번 계산해준다.
    pDamageInstance->m_p3DObject->GetGCObject()->SetMotionMatrix( true, NULL , false , true );

    //불이 나와야 하는 위치를 구한다.
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 8, &matHead );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 4, &matTail );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 2, &matWaist );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 13, &matArticulationL );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 21, &matArticulationR );
    pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( 24, &matLance );

    matHead             *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
    matTail             *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
    matWaist            *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
    matArticulationL    *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
    matArticulationR    *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );
    matLance            *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );

    // 본의 위치 찾쟈! 업데이트 ㄱㄱㅆ
    m_vArticulationL.x = matArticulationL._41;  m_vArticulationL.y = matArticulationL._42 + 0.03f;  m_vArticulationL.z = matArticulationL._43;
    m_vArticulationR.x = matArticulationR._41;  m_vArticulationR.y = matArticulationR._42 + 0.02f;  m_vArticulationR.z = matArticulationR._43;

    if ( true == bRight )
    {
        m_vHead.x = matHead._41 - 0.03f;      m_vHead.y = matHead._42 + 0.02f;     m_vHead.z = matHead._43;
        m_vTail.x = matTail._41 - 0.05f;      m_vTail.y = matTail._42 - 0.05f;     m_vTail.z = matTail._43;
        m_vWaist.x = matWaist._41 + 0.0f;     m_vWaist.y = matWaist._42 + 0.02f;   m_vWaist.z = matWaist._43;
        m_vLance.x = matLance._41 - 0.4f;     m_vLance.y = matLance._42 -0.031f;    m_vLance.z = matLance._43;
    }
    else
    {
        m_vHead.x = matHead._41 + 0.03f;      m_vHead.y = matHead._42 + 0.0f;      m_vHead.z = matHead._43;
        m_vTail.x = matTail._41 + 0.1f;       m_vTail.y = matTail._42 - 0.05f;     m_vTail.z = matTail._43;
        m_vWaist.x = matWaist._41 + 0.05f;    m_vWaist.y = matWaist._42 + 0.04f;   m_vWaist.z = matWaist._43;
        m_vLance.x = matLance._41 + 0.4f;     m_vLance.y = matLance._42 -0.031f;    m_vLance.z = matLance._43;
    }

    if ( ( !vecTemp[0] || g_ParticleManager->IsLiveInstance( vecTemp[0] ) == false) && bRenderParticle == true && OriginalFrameNumber == 10 )
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
            vecTemp[4] = pDamageInstance->CreateSequence("Ronan3_special1-3_articulation01", 0, 0, 0.5f);
            vecTemp[5] = pDamageInstance->CreateSequence("Ronan3_special1-3_articulation02", 0, 0, 0.5f);
            vecTemp[6] = pDamageInstance->CreateSequence("Ronan3_special1-3_articulation01", 0, 0, 0.5f);
            vecTemp[7] = pDamageInstance->CreateSequence("Ronan3_special1-3_articulation02", 0, 0, 0.5f);
            vecTemp[8] = pDamageInstance->CreateSequence("Ronan3_special1-3_articulation01", 0, 0, 0.5f);
            vecTemp[9] = pDamageInstance->CreateSequence("Ronan3_special1-3_articulation02", 0, 0, 0.5f);

            if (pDamageInstance->m_IsRight)
            {
                vecTemp[0] = pDamageInstance->CreateSequence("Ronan3_special1-3_head01", 0, 0, 0.5f);
                vecTemp[1] = pDamageInstance->CreateSequence("Ronan3_special1-3_head02", 0, 0, 0.5f);
                vecTemp[2] = pDamageInstance->CreateSequence("Ronan3_special1-3_Leg01", 0, 0, 0.5f);
                vecTemp[3] = pDamageInstance->CreateSequence("Ronan3_special1-3_Leg02", 0, 0, 0.5f);
            }
            else
            {
                vecTemp[0] = pDamageInstance->CreateSequence("Ronan3_special1-3_head01_R", 0, 0, 0.5f);
                vecTemp[1] = pDamageInstance->CreateSequence("Ronan3_special1-3_head02_R", 0, 0, 0.5f);
                vecTemp[2] = pDamageInstance->CreateSequence("Ronan3_special1-3_Leg01_R", 0, 0, 0.5f);
                vecTemp[3] = pDamageInstance->CreateSequence("Ronan3_special1-3_Leg02_R", 0, 0, 0.5f);
            }
        }

        TRACE_INFO stTrace;
        stTrace.m_pvPos = &m_vHead;
        stTrace.m_fParticleTraceTime = 5.0f;
        vecTemp[0]->SetTrace( &stTrace );
        vecTemp[1]->SetTrace( &stTrace );

        stTrace.m_pvPos = &m_vTail;
        vecTemp[2]->SetTrace( &stTrace );
        vecTemp[3]->SetTrace( &stTrace );

        stTrace.m_pvPos = &m_vWaist;
        vecTemp[4]->SetTrace( &stTrace );
        vecTemp[5]->SetTrace( &stTrace );

        stTrace.m_pvPos = &m_vArticulationL;
        vecTemp[6]->SetTrace( &stTrace );
        vecTemp[7]->SetTrace( &stTrace );

        stTrace.m_pvPos = &m_vArticulationR;
        vecTemp[8]->SetTrace( &stTrace );
        vecTemp[9]->SetTrace( &stTrace );
    }

    int nForceNum = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetForceNum();
    if ( OriginalFrameNumber == 1 )
    {
        m_vPlayerPos = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition();
    }
    else if ( OriginalFrameNumber == 4 )
    {
        g_KDSound.Play( "759" );
    }
    else if ( OriginalFrameNumber == 54 )
    {
        if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who)) {
            if (pDamageInstance->m_IsRight)
            {
                vecTemp[10] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_Lance_01", 0, 0, 0.5f);
                vecTemp[11] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_Lance_02", 0, 0, 0.5f);
                vecTemp[12] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_10", 0, 0, 0.5f);
                vecTemp[13] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_11", 0, 0, 0.5f);
            }
            else
            {
                vecTemp[10] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_Lance_01_R", 0, 0, 0.5f);
                vecTemp[11] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_Lance_02_R", 0, 0, 0.5f);
                vecTemp[12] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_10_R", 0, 0, 0.5f);
                vecTemp[13] = pDamageInstance->CreateSequence("Ronan3_SPECIAL2-3_11_R", 0, 0, 0.5f);
            }
        }

        TRACE_INFO stTrace;
        stTrace.m_pvPos = &m_vLance;
        stTrace.m_fParticleTraceTime = 5.0f;
        vecTemp[10]->SetTrace( &stTrace );
        vecTemp[11]->SetTrace( &stTrace );
        vecTemp[12]->SetTrace( &stTrace );
        vecTemp[13]->SetTrace( &stTrace );

        g_KDSound.Play( "760" );
        g_KDSound.Play( "768" );

		CDamageInstance* pkDamage = 
				g_MyD3D->m_pDamageManager->AddWithLocate( 1025+nForceNum , pDamageInstance->m_Who, m_vPlayerPos.x, m_vPlayerPos.y, true );
		
		if( pkDamage )
        {
			pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
	    }
    }
    else if ( OriginalFrameNumber == 108 )
    {   
        g_KDSound.Play( "761" );
        g_KDSound.Play( "762" );
		
		CDamageInstance* pkDamage = 
			g_MyD3D->m_pDamageManager->AddWithLocate( 1034+nForceNum , pDamageInstance->m_Who, m_vPlayerPos.x, m_vPlayerPos.y, true );

		if( pkDamage )
        {
			pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
        }

        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->InitForceNum();
    }
    else if ( OriginalFrameNumber == 160 )
    {
        for ( int i = 0; i < 14; ++i ) 
        {
            g_ParticleManager->DeleteSequence( vecTemp[i] );
        }

        g_KDSound.Play( "763" );
        bRenderParticle = false;
    }
    else if ( OriginalFrameNumber == 165 )
    {
        g_KDSound.Play( "792" );
    }
}

void CHardDamageSummonTempest::End( CDamageInstance* pDamageInstance )
{
    for ( int i = 0; i < 14; ++i ) 
    {
        if ( vecTemp[i] ) 
            g_ParticleManager->DeleteSequence( vecTemp[i] );

        vecTemp[i] = NULL;
    }

    CDamage::End( pDamageInstance );
}
