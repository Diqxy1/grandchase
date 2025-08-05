#include "stdafx.h"
#include ".\KGCBuffLasDropWeapon.h"

KGCBuffLasDropWeapon::KGCBuffLasDropWeapon(void)
:pkDamage(NULL)
{
}

KGCBuffLasDropWeapon::~KGCBuffLasDropWeapon(void)
{
}

bool KGCBuffLasDropWeapon::Init( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID )
{
	if ( KGCBuff::Init( iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID ) )
	{
		if( !g_MyD3D->IsPlayerIndex( iPlayerIndex_ ) )
			return false;

		g_MyD3D->MyPlayer[iPlayerIndex_]->RenderWeapon( false );
		g_MyD3D->MyPlayer[iPlayerIndex_]->RenderMergeWeapon( false );

        if( pBuffInst_->m_iBuffEnum == EGC_SKILL_DROP_WEAPON ) {
		    pkDamage = g_MyD3D->m_pDamageManager->Add( DT_SKILL_LAS_TWOHAND_QUICKEN_ICON, iPlayerIndex_ );
        }
		return true;
	}
	
	return false;
}

bool KGCBuffLasDropWeapon::FrameMove( int iPlayerIndex_, KGCBuffInstance* pBuffInst_ )
{
	if ( KGCBuff::FrameMove( iPlayerIndex_, pBuffInst_ ) )
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->RenderWeapon( false );
		g_MyD3D->MyPlayer[iPlayerIndex_]->RenderMergeWeapon( false );

        if( pBuffInst_->m_iBuffEnum == EGC_SKILL_DROP_WEAPON ) {
		    pkDamage->m_IsRight = g_MyD3D->MyPlayer[iPlayerIndex_]->bIsRight;
        }
	}

	return true;
}

bool KGCBuffLasDropWeapon::Destroy( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_ )
{
	if ( KGCBuff::Destroy( iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_ ) )
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->RenderWeapon( true );
		g_MyD3D->MyPlayer[iPlayerIndex_]->RenderWeapon( true );

        if( pBuffInst_->m_iBuffEnum == EGC_SKILL_DROP_WEAPON ) {
		    g_MyD3D->m_pDamageManager->DeleteInstance( pkDamage );
        }

		return true;
	}

	return false;
}