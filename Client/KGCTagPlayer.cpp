#include "stdafx.h"
#include "KGCTagPlayer.h"
//





#include "KGCPlayerCommon.h"
#include "DamageManager.h"
#include "Controls.h"

//
#include "GCCameraOrtho.h"
#include "Stage.h"


#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCTagPlayer::KGCTagPlayer()
{

}

KGCTagPlayer::~KGCTagPlayer()
{

}

int KGCTagPlayer::OnTagEnd(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
        {
            pPlayer->bIsRight = true;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            return 0;
        }       
        else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            pPlayer->bIsRight = false;
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH);
            g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            pPlayer->Jump();
            g_kCamera.SetZoom(0.75f,g_MyD3D->Get_MyPlayer());
            pPlayer->cFrame = 0;
            g_MyD3D->MyCtrl->k_Up = false;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Down )
        {
            g_MyD3D->MyCtrl->k_Down = false;
			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 40;
            return 0;
        }
        return 0;
    }
    return 0;
}

int KGCTagPlayer::OnTagStart(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame > 7 && ISPUSHED(k_Fresh_Punch) )
        {
            pPlayer->Next_Attack_Count = 255;
            return 0;
        }
    }

    if( pPlayer->cFrame == 14 )
    {
		InitBeforeTag( pPlayer );

        TagCharacterInfo *pTagPlayer = pPlayer->GetNextTagCharInfo();
        pPlayer->SetPlayerState( pTagPlayer->m_FormTemplate[pTagPlayer->MetamorphosisForm].sTagTemplate.m_uiEndMotion );
        pPlayer->cFrame = 0;

        pPlayer->TagCharacter();
        pPlayer->NoCheckContact = 20;
    }

    return 0;
}

void KGCTagPlayer::InitBeforeTag( PLAYER* pPlayer_ )
{
	if ( pPlayer_->m_bIsBurnningMode )
	{
		pPlayer_->ChangeBurnningTemplate();
		pPlayer_->SetSkillType( pPlayer_->m_iSkillType );
	}
	// 순서 바꾸면 안되요 ㅎ
	pPlayer_->InitBurnningGauge();

	g_pMagicEffect->ClearMagicEffect( pPlayer_->m_iPlayerIndex, EGC_SKILL_LAS_MOVE_SPEEDUP );
	g_pMagicEffect->ClearMagicEffect( pPlayer_->m_iPlayerIndex, EGC_SKILLEFFECT_MOVE_SPEEDUP );
	g_pMagicEffect->ClearMagicEffect( pPlayer_->m_iPlayerIndex, EGC_EFFECT_RYAN_MOVE_SPEEDUP );
}