#include "stdafx.h"
#include ".\kgcformnephilim.h"





#include "KGCPlayerCommon.h"
#include "DamageManager.h"
#include "Controls.h"

//
#include "Stage.h"



#define ISPUSHED(p)	(g_MyD3D->MyCtrl->p)

KGCFormNephilim::KGCFormNephilim(void)
{
}

KGCFormNephilim::~KGCFormNephilim(void)
{
}

int KGCFormNephilim::OnNephilimFormEnd(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame == 36 )
    {
        pPlayer->SetPlayerState( MID_NEPHILLIM_WAIT );
        pPlayer->cFrame = 0;
        //pPlayer->All_Latency_Equal();
    }

    return 0;
}

int KGCFormNephilim::OnWait(PLAYER* pPlayer, bool bKeyProc)
{
    if (bKeyProc)
    {
        if (g_MyD3D->MyCtrl->k_Select_Item)
        {
            pPlayer->ProcessMetamorphosisNormalForm();
            pPlayer->Direct_Motion_Input(pPlayer->GetCurFormTemplate().WAIT);
            pPlayer->cFrame = 0;
            pPlayer->GetMetaForm(FORM_NORMAL)->SetWorldMat(pPlayer->GetMetaForm(FORM_NEPHILIM)->GetWorldMat());
            pPlayer->ClearMagicEffect(EGC_EFFECT_METAMORPHOSIS_NEPHILLIM_FORM);
            pPlayer->ClearMagicEffect(EGC_EFFECT_NEPHILLIM_ARMOR_ME);
            g_MyD3D->MyCtrl->k_Select_Item = false;
            return 0;
        }
        return KGCPlayerCommon::OnWait(pPlayer, true);
    }

    return KGCPlayerCommon::OnWait(pPlayer, false);
}

int KGCFormNephilim::OnCombo(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( pPlayer->uiMotion != MID_NEPHILLIM_ATK1 )
        {		
            if( ISPUSHED(k_Fresh_Right_Dash) || ISPUSHED(k_Fresh_Left_Dash))
            {
                pPlayer->bIsRight = ISPUSHED(k_Fresh_Right_Dash);
                pPlayer->cFrame = 0;

                if( pPlayer->uiMotion == MID_NEPHILLIM_ATK5 )
					pPlayer->SetPlayerState(MID_NEPHILLIM_CRITICAL_ATK);                   
                else 
					pPlayer->SetPlayerState(MID_NEPHILLIM_DOUBLE_ATK);
                    

                return 0;
            }
        }

        if( ISPUSHED(k_Fresh_Punch) )
        {
          //  if( pPlayer->uiMotion != MID_NEPHILLIM_ATK5)
           // {
			//	if( pPlayer->Next_Attack_Count > 0 )
			//	{
					pPlayer->Next_Attack_Count = 255;
			//	}
           // }
        }
        return 0;
    }	
    return 0;
}

int KGCFormNephilim::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            ISPUSHED(k_Fresh_Punch) = false;
            pPlayer->SetPlayerState( MID_NEPHILLIM_DASH_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }

        if( ISPUSHED(k_Up) )
        {
            if( pPlayer->IsContact )
            {
                if( pPlayer->bIsRight )
                    pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunJumpXSpeed;
                else
                    pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunJumpXSpeed;

                pPlayer->SetPlayerState( MID_NEPHILLIM_JUMP );
                pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fRunJumpYSpeed;
                pPlayer->cFrame = 0;
                ISPUSHED(k_Up) = false;
                return 0;
            }
            pPlayer->NoCheckContact = 0;
        }

        if( ISPUSHED(k_Down) )
        {
            ISPUSHED(k_Down) = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;
            
            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

            pPlayer->y_Speed = -0.01f;
            pPlayer->NoCheckContact = 20;
            return 0;
        }

        if(pPlayer->IsContact)
        {		
            if( pPlayer->bIsRight && ISPUSHED(k_Left) )
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(MID_NEPHILLIM_WALK);
                pPlayer->cFrame = 0;
                return 0;
            }

            if( !pPlayer->bIsRight && ISPUSHED(k_Right) )
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(MID_NEPHILLIM_WALK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        //정지
        if( ( ( pPlayer->bIsRight &&  !ISPUSHED(k_Right) ) || 
            ( !pPlayer->bIsRight &&  !ISPUSHED(k_Left) ) )&& pPlayer->cFrame < 36 )
        {
            pPlayer->cFrame = 36;
            return 0;
        }

        return 0;
    }

    //무한 반복하는 프레임..
    if( pPlayer->cFrame == 40 )
        pPlayer->cFrame = 8;

    if( pPlayer->cFrame < 40)
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }

    return 0;
}

int KGCFormNephilim::OnLand(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= 5 )
    {
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );
    }

    return 0;
}

int KGCFormNephilim::OnJump(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
		if( ISPUSHED(k_Fresh_Right_Dash) || ISPUSHED(k_Fresh_Left_Dash))
		{
			pPlayer->bIsRight = ISPUSHED(k_Fresh_Right_Dash);
			pPlayer->cFrame = 0;

			pPlayer->SetPlayerState(MID_NEPHILLIM_JUMP_DASH);
			return 0;
		}

        if( ISPUSHED(k_Fresh_Punch) )
        {
            ISPUSHED(k_Fresh_Punch) = false;

            if( ISPUSHED(k_Down) )
            {
                pPlayer->SetPlayerState( MID_NEPHILLIM_JUMP_ATK3 );
                pPlayer->cFrame = 0;
                ISPUSHED(k_Down) = false;
                return 0;
            }
            else if( ISPUSHED(k_Up) )
            {
                pPlayer->SetPlayerState( MID_NEPHILLIM_JUMP_ATK4 );
                pPlayer->cFrame = 0;
                return 0;
            }
            else
            {
                pPlayer->SetPlayerState( MID_NEPHILLIM_JUMP_ATK1 );
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        return KGCPlayerCommon::OnJump( pPlayer, true );
    }

    return 0;
}

int KGCFormNephilim::OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
            pPlayer->Next_Motion = MID_NEPHILLIM_JUMP_ATK2;

        return 0;
    }

    if( pPlayer->cFrame >= 10 && pPlayer->Next_Motion == MID_NEPHILLIM_JUMP_ATK2 )
    {
        pPlayer->SetPlayerState( MID_NEPHILLIM_JUMP_ATK2 );
        pPlayer->cFrame = 0;
        return 0;
    }

    return 0;
}

int KGCFormNephilim::OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= 31 )
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCFormNephilim::OnJumpAtk3(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame == 23 )
        pPlayer->cFrame = 22;

    if( pPlayer->cFrame >= 40 )
        return KGCPlayerCommon::OnWait( pPlayer, bKeyProc );

    return 0;
}

int KGCFormNephilim::OnJumpAtk4(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= 30 )
        pPlayer->m_bFrameLock = true;

    return 0;
}

int KGCFormNephilim::OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if( pPlayer->cFrame >= 31 )
        pPlayer->m_bFrameLock = true;

    if( pPlayer->IsContact )
    {
        switch( pPlayer->MetamorphosisForm )
        {
        case FORM_NORMAL:
            pPlayer->SetPlayerState( MID_RYAN2_WAIT );
            break;
        case FORM_NEPHILIM:
            pPlayer->SetPlayerState( MID_NEPHILLIM_WAIT );
            break;
        }
        pPlayer->cFrame = 0;
    }

    return 0;
}

int KGCFormNephilim::OnJumpDash(PLAYER* pPlayer, bool bKeyProc)
{
    if( bKeyProc )
    {
        if( ISPUSHED(k_Fresh_Punch) )
        {
            ISPUSHED(k_Fresh_Punch) = false;
            pPlayer->SetPlayerState( MID_NEPHILLIM_JUMP_DASH_ATK );
            pPlayer->cFrame = 0;
            return 0;
        }

		// 2007/2/7. iridology. 일명 스텝이 되게 하는 코드임.
		if( ISPUSHED(k_Down) )
		{
			g_MyD3D->MyCtrl->k_Down = false;

			if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
				return 0;

            if( pPlayer->IsContact && !g_MyD3D->MyStg->IsEnableNoCheck(pPlayer->vPos.x,pPlayer->vPos.y) || pPlayer->vPos.y <= -0.5f)
            {
                return 0;
            }

			pPlayer->y_Speed = -0.01f;
			pPlayer->NoCheckContact = 20;
			return 0;
		}
    }	

    return 0;
}

int KGCFormNephilim::OnWalk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        KGCPlayerCommon::OnWait(pPlayer,true);
        return 0;
    }

    if( pPlayer->cFrame == 103 )
    {
        if( (ISPUSHED(k_Left) && !pPlayer->bIsRight ) ||
            (ISPUSHED(k_Right) && pPlayer->bIsRight ) )
        {
            pPlayer->cFrame = 24;
            return 0;
        }
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCFormNephilim::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
	// 2007/2/6. iridology. 혹 대쉬가 무디면 여기다가 처리를 해줘야함..

	return KGCPlayerCommon::OnDash( pPlayer, bKeyProc );
}
