#include "stdafx.h"
#include ".\HardDamageBermesiaFinalEarthQuake.h"

#include "DamageManager.h"

#include "GCCameraOrtho.h"

CHardDamageBermesiaFinalEarthQuake::CHardDamageBermesiaFinalEarthQuake(void)
: m_bParent(true), m_fStartX(0.0f), m_fStartY(0.0f), m_sDelay(0)
{
}

CHardDamageBermesiaFinalEarthQuake::~CHardDamageBermesiaFinalEarthQuake(void)
{
}

void CHardDamageBermesiaFinalEarthQuake::AddParticle( CDamageInstance* pDamageInstance , char* strSeqeunce, float fX , float fY )
{
    CParticleEventSeqPTR SeqPTR;
    SeqPTR = pDamageInstance->CreateSequence( strSeqeunce, fX - 1.0f, fY - 0.5f, 0.5f );
    if( SeqPTR )
        SeqPTR->SetShow( true );
}

void CHardDamageBermesiaFinalEarthQuake::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    m_fStartX = fStartX;
    m_fStartY = fStartY;
    CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

    AddParticle( pDamageInstance ,"EFFECT_STONEGOLEM_UPLIFT01", fStartX, fStartY );
    AddParticle( pDamageInstance ,"EFFECT_STONEGOLEM_UPLIFT02", fStartX, fStartY );
    g_KDSound.Play("283");
}

void CHardDamageBermesiaFinalEarthQuake::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove( pDamageInstance );

    if( m_sDelay % 6 == 0 )
        g_kCamera.SetEarthQuake(8);

    CHardDamageBermesiaFinalEarthQuake* pEarthQuake = static_cast<CHardDamageBermesiaFinalEarthQuake*>(pDamageInstance->m_pOriginalDamage);
    if( pEarthQuake->GetParent() == true )
    {
        m_sDelay++;

        // 나를 포함해 총 4개를 생성
        if( m_sDelay % 10 == 0 )
        {
            std::map< int, std::vector<CDamageInstance*> >& InstancePool = g_MyD3D->m_pDamageManager->GetDamageInstancePool();

            CDamageInstance* pChildInstance = new CDamageInstance();

			if(!g_MyD3D->m_pDamageManager->MakeDamage( pDamageInstance->m_What, pChildInstance ))
			{
				SAFE_DELETE(pDamageInstance);
				return;
			}

            pChildInstance->m_What              = pDamageInstance->m_What;
            pChildInstance->m_Who               = pDamageInstance->m_Who;
            pChildInstance->m_ExtraMonsterNum   = pDamageInstance->m_ExtraMonsterNum;
            pChildInstance->m_Life              = pDamageInstance->m_pOriginalDamage->m_Start_Life;
            pChildInstance->m_Scale_x           = pDamageInstance->m_pOriginalDamage->m_Start_Scale_x;
            pChildInstance->m_fAnimScaleX       = pDamageInstance->m_pOriginalDamage->m_fAnim_Start_ScaleX;
            pChildInstance->m_fAnimScaleY       = pDamageInstance->m_pOriginalDamage->m_fAnim_Start_ScaleY;
            pChildInstance->m_iTeam             = pDamageInstance->m_iTeam;
            pChildInstance->m_IsRight           = pDamageInstance->m_IsRight;
            pChildInstance->m_SlowCount         = pDamageInstance->m_SlowCount;
            pChildInstance->m_IsMonsterDamage   = pDamageInstance->m_IsMonsterDamage;
            CHardDamageBermesiaFinalEarthQuake* pChileEarthQuake = static_cast<CHardDamageBermesiaFinalEarthQuake*>(pChildInstance->m_pOriginalDamage);
            pChileEarthQuake->SetParent( false );
            pChildInstance->m_pOriginalDamage->BeginWithLocate( 
                pChildInstance, pEarthQuake->GetStartX() + (m_sDelay / 10 * 0.45f), pEarthQuake->GetStartY() );

            InstancePool[ pChildInstance->m_What ].push_back( pChildInstance );
        }
    }
}
