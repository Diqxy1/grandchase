#include "stdafx.h"
#include ".\KGCBuffLasHide.h"

KGCBuffLasHide::KGCBuffLasHide(void)
{
}

KGCBuffLasHide::~KGCBuffLasHide(void)
{
}

bool KGCBuffLasHide::Init( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID )
{
	if ( KGCBuff::Init( iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID ) )
	{
        g_pMagicEffect->SetMagicEffect( iPlayerIndex_, EGC_EFFECT_INVISIBLE, fBuffTime );
		return true;
	}
	
	return false;
}

bool KGCBuffLasHide::FrameMove( int iPlayerIndex_, KGCBuffInstance* pBuffInst_ )
{
	if ( KGCBuff::FrameMove( iPlayerIndex_, pBuffInst_ ) )
	{
		if ( false == pBuffInst_->m_bPostCoolTime )
		{
			g_pMagicEffect->SetMagicEffect( iPlayerIndex_, EGC_EFFECT_INVISIBLE, pBuffInst_->m_fRemainTime );
		}
		else
		{
			g_pMagicEffect->ClearMagicEffect( iPlayerIndex_, EGC_EFFECT_INVISIBLE );
		}
	}

	return true;
}

bool KGCBuffLasHide::Destroy( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_ )
{
	if ( KGCBuff::Destroy( iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_ ) )
	{
		g_pMagicEffect->ClearMagicEffect( iPlayerIndex_, EGC_EFFECT_INVISIBLE );

        if( g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle )
        {
            g_ParticleManager->DeleteSequence( g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle );
            g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle = NULL;
            
        }

		return true;
	}

	return false;
}

void KGCBuffLasHide::DestroyBuffEffect( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_ )
{
    KGCBuff::DestroyBuffEffect( iPlayerIndex_, pBuffInst_, bOnDie_ );
    if( g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle )
    {
        g_ParticleManager->DeleteSequence( g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle );
        g_MyD3D->MyPlayer[iPlayerIndex_]->m_pHideParticle = NULL;

    }
}