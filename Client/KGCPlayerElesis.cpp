#include "stdafx.h"
// KGCPlayerElesis.cpp: implementation of the KGCPlayerElesis class.
//
//////////////////////////////////////////////////////////////////////
//





#include "KGCPlayerElesis.h"
#include "DamageManager.h"
#include "Controls.h"
#include "Stage.h"

#include "KGCPlayerCommon.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCPlayerElesis::KGCPlayerElesis()
{

}

KGCPlayerElesis::~KGCPlayerElesis()
{

}

int KGCPlayerElesis::OnComboAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {            
            if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 )
            {
				if( pPlayer->GetCurFormTemplate().ATK1_FASTINPUT != -1 && 
					g_Hot_Count > pPlayer->GetCurFormTemplate().iFastInputNeedCount )
				{
					if( pPlayer->Next_Attack_Count != 255 )
					{
						pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().ATK1_FASTINPUT);
						pPlayer->cFrame = 0;
						return 0;
					}
				}
            }

            if( pPlayer->Next_Attack_Count > 0 
                || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_2 
                || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_3
                || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_4)
            {
                pPlayer->Next_Attack_Count = 255;
            }
        }

        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 )
        {
            if( g_MyD3D->MyCtrl->k_Fresh_Left_Right == true )
            {
                g_MyD3D->MyCtrl->k_Fresh_Left_Right = false;

                if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_SPECIAL_ATK1 ) )
                {
                    pPlayer->bIsRight = true;
                    pPlayer->SetPlayerState( MID_SKILL_ELESIS1_SPECIAL_ATK1 );
                    pPlayer->cFrame = 0;
                    return 0;
                }
            }
            else if( g_MyD3D->MyCtrl->k_Fresh_Right_Left == true )
            {
                g_MyD3D->MyCtrl->k_Fresh_Right_Left = false;

                if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_SPECIAL_ATK1 ) )
                {
                    pPlayer->bIsRight = false;
                    pPlayer->SetPlayerState( MID_SKILL_ELESIS1_SPECIAL_ATK1 );
                    pPlayer->cFrame = 0;
                    return 0;
                }
            }
        }

        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().ATK1 )
        {
            if( (pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right) ||
                (!pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left) )
            {
                if( pPlayer->cFrame < 20 )
                {
                    if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_SPECIAL_ATK2 ) )
                    {
                        pPlayer->Next_Motion = MID_SKILL_ELESIS1_SPECIAL_ATK2_0_1_DASH;
                        return 0;
                    }
                }
            }
        }

        if(pPlayer->Next_Attack_Count == 0)
            return 0;

        if( g_MyD3D->MyCtrl->k_Up)
        {		
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
            {
                if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_COMBO_A_UP ))
                {
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_NEWCOMBO_A_UP);                                    
					pPlayer->cFrame = 0;                    
                }
                else if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_COMBO_A_UP2 ) )
                {                        
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2);
					pPlayer->cFrame = 0;                    
                }    
                else if(g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_BASE_COMBO_UP2 ) )
                {
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_COMBO_A_UP_2);
					pPlayer->cFrame = 0;                    
                }
                else 
                {
                    pPlayer->SetPlayerState(MID_ELESIS_FLYATK);
					pPlayer->cFrame = 0;                    
                }                
            }
            g_MyD3D->MyCtrl->k_Up = false;                                
            return 0;
        }
#ifndef MATCH_BALLENCE_121217
        else if( g_MyD3D->MyCtrl->k_Down ) 
        {	
            g_MyD3D->MyCtrl->k_Down = false;                    
            if( pPlayer->uiMotion != MID_SKILL_ELESIS1_COMBO_A_1 || pPlayer->uiMotion != MID_ELESIS_ATK1_1 )
            {
                if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_COMBO_A_DOWN ) )
                {
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_NEWCOMBO_A_DOWN);
                    pPlayer->cFrame = 0;                    
                }
                else if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_COMBO_A_DOWN2 ) )
                {                        
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_NEWCOMBO_A_DOWN_2);
                    pPlayer->cFrame = 0;                    
                }    
                else if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_BASE_COMBO_DOWN2 ) )
                {
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_COMBO_A_DOWN_2);    
                    pPlayer->cFrame = 0;                    
                }
                else if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_BASE_COMBO_DOWN ) )
                {
                    pPlayer->SetPlayerState(MID_SKILL_ELESIS1_COMBO_A_DOWN);
                    pPlayer->cFrame = 0;                    
                }                            
                }    
            return 0;
        }

#endif		
        else if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash || g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATK1 )
            {
                if( pPlayer->uiMotion != pPlayer->GetCurFormTemplate().ATKEND )	//Critical
                {
                    g_MyD3D->m_pDamageManager->Add(DT_CRITICAL,pPlayer->m_iPlayerIndex);
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().CRITICAL_ATK);
                }
                else //Double
                {
                    g_MyD3D->m_pDamageManager->Add(DT_DOUBLE,pPlayer->m_iPlayerIndex);
                    pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DOUBLE_ATK);
                }

                if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
                    pPlayer->bIsRight = true;
                else pPlayer->bIsRight = false;

                pPlayer->Next_Attack_Count=0;

                pPlayer->Ah_Sound(5);
                pPlayer->cFrame = 0;

                return 0;
            }
        }
        return 0;
    }

    if( pPlayer->cFrame == 20 && pPlayer->Next_Motion != -1 )
    {
        pPlayer->SetPlayerState( pPlayer->Next_Motion );
        pPlayer->cFrame = 0;
        return 0;
    }

    return 0;
}

int KGCPlayerElesis::OnRun(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            pPlayer->cFrame = 0;
            return 0;
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {			
                if(pPlayer->bIsRight)
                    pPlayer->x_Speed = 0.025f;
                else pPlayer->x_Speed = -0.025f;

                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);			
                pPlayer->y_Speed = 0.03f;
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }
            pPlayer->NoCheckContact = 0;			
        }

        if( g_MyD3D->MyCtrl->k_Down)
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
        if(pPlayer->IsContact)
        {		
            if( pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left)
            {
                pPlayer->bIsRight = false;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }

            if( !pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right)
            {
                pPlayer->bIsRight = true;
                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().WALK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }

        if( ( ( pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Right ) || 
            ( !pPlayer->bIsRight &&  !g_MyD3D->MyCtrl->k_Left ) )&& pPlayer->cFrame < 30)		//정지
        {
            pPlayer->cFrame = 30;
            return 0;
        }

        return 0;
    }

    if(pPlayer->cFrame == 29)		//무한 반복하는 프레임..
        pPlayer->cFrame = 0;

    if( pPlayer->cFrame < 30)
    {
        if( pPlayer->bIsRight )
            pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;
        else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
    }
    else
    {
        //정지하는 프레임
        /*
        if( pPlayer->IsRight && g_MyD3D->MyCtrl->k_Right)
        pPlayer->x_Speed = 0.007f;
        else pPlayer->x_Speed = -0.007f;
        */
    }

    pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerElesis::OnJumpAtk(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( pPlayer->cFrame > 0 && g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            if (g_kGlobalValue.CheckTrainedSkill(pPlayer, SID_ELESIS1_JUMP_ATK1))
            {
                pPlayer->SetPlayerState(MID_SKILL_ELESIS1_JUMP_ATK);
                pPlayer->cFrame = 0;
                return 0;
            }
        }
        return 0;
    }

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //무한 반복하는 프레임..
    {
        pPlayer->m_bFrameLock = true;
    }
    return 0;
}

int KGCPlayerElesis::OnSkillDashAtk1( PLAYER* pPlayer, bool bKeyProc )
{
    if(bKeyProc)
    {
        if( pPlayer->cFrame > 0 && g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->Next_Motion = MID_SKILL_ELESIS1_DASH_ATK1;
            return 0;
        }
        return 0;
    }

    if( pPlayer->Next_Attack_Count > 0 )
    {
        if( pPlayer->cFrame == 14 )
        {
            if( pPlayer->Next_Motion != -1 && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_DASH_ATK1 ) )
            {
            //    //if( g_MyD3D->MyCtrl->k_Right == true )
            //    //{
            //    //    pPlayer->bIsRight = true;
            //    //}
            //    //else if( g_MyD3D->MyCtrl->k_Left == true )
            //    //{
            //    //    pPlayer->bIsRight = false;
            //    //}
            //    //else
            //    //{
            //    //    pPlayer->Next_Motion = -1;
            //    //    return 0;
            //    //}

                pPlayer->SetPlayerState( pPlayer->Next_Motion );
                pPlayer->cFrame = 0;
                return 0;
            }
            else if( g_MyD3D->MyCtrl->k_Up == true && g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_DASH_ATK3 ) )
            {
                pPlayer->SetPlayerState( MID_SKILL_ELESIS1_DASH_ATK3 );
                pPlayer->cFrame = 0;
                return 0;
            }
            return 0;
        }
    }

    return 0;
}

int KGCPlayerElesis::OnDash(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( pPlayer->uiMotion != MID_SKILL_ELESIS1_SUPER_DASH )
        {
            if( pPlayer->cFrame > 0 && pPlayer->cFrame < 9 )
            {
                if( (pPlayer->bIsRight == true && g_MyD3D->MyCtrl->k_Fresh_Right == true) ||
                    (pPlayer->bIsRight == false && g_MyD3D->MyCtrl->k_Fresh_Left == true) )
                {
                    if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_SUPER_DASH ) )
                    {
						pPlayer->x_Speed = 0.0f;
                        pPlayer->SetPlayerState( MID_SKILL_ELESIS1_SUPER_DASH );
                        pPlayer->cFrame = 0;
                        return 0;
                    }
                }
            }
        }

        if( g_MyD3D->MyCtrl->k_Up )
        {
            if( pPlayer->IsContact )
            {
                if(pPlayer->bIsRight)
                    pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fDashJumpXSpeed;
                else pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fDashJumpXSpeed;

                pPlayer->y_Speed = pPlayer->GetCurFormTemplate().fDashJumpYSpeed;

                pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().JUMP);
                pPlayer->cFrame = 0;
                g_MyD3D->MyCtrl->k_Up = false;
                return 0;
            }
            pPlayer->NoCheckContact = 0;
        }
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            pPlayer->SetPlayerState(pPlayer->GetCurFormTemplate().DASH_ATK);
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            pPlayer->Next_Attack_Count = pPlayer->GetCurFormTemplate().iNextAttackCount;
            pPlayer->cFrame = 0;
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
            pPlayer->NoCheckContact = 20;
            return 0;
        }

        if( pPlayer->cFrame < 3 )
            pPlayer->SetWindSpeed();

        return 0;
    }

    if( pPlayer->cFrame < 3 )
        pPlayer->SetWindSpeed();

    return 0;
}

int KGCPlayerElesis::OnSkillFastInput( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            // 좀 어려운거 같아서..
            pPlayer->Next_Motion = pPlayer->GetCurFormTemplate().ATK1_FASTINPUT;
            g_MyD3D->MyCtrl->k_Fresh_Punch = false;
            return 0;
        }
    }

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //무한 반복하는 프레임..
    {
        if( pPlayer->Next_Motion != -1 )
        {
            pPlayer->SetPlayerState( pPlayer->Next_Motion );
            pPlayer->cFrame = 0;
            return 0;
        }
        else
        {
            pPlayer->SetPlayerState( MID_SKILL_ELESIS1_FAST_ATK_END_WAIT );
            pPlayer->cFrame = 0;
            return 0;
        }
    }
    return 0;
}

int KGCPlayerElesis::OnSkillSpecialAtk2Dash( PLAYER* pPlayer, bool bKeyProc )
{    
    if( bKeyProc )
    {
        if( pPlayer->cFrame == 0 )
            return 0;
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {
            if( pPlayer->Old_Motion == MID_SKILL_ELESIS1_SPECIAL_ATK2_0_1_DASH )
            {
                pPlayer->Next_Motion = MID_SKILL_ELESIS1_SPECIAL_ATK2_1;
                return 0;
            }
            if( pPlayer->Old_Motion == MID_SKILL_ELESIS1_SPECIAL_ATK2_1_2_DASH )
            {
                pPlayer->Next_Motion = MID_SKILL_ELESIS1_SPECIAL_ATK2_2;
                return 0;
            }
            else if( pPlayer->Old_Motion == MID_SKILL_ELESIS1_SPECIAL_ATK2_2_3_DASH )
            {
                pPlayer->Next_Motion = MID_SKILL_ELESIS1_SPECIAL_ATK2_3;
                return 0;
            }
        }
        if( g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left )
        {
            if( g_MyD3D->MyCtrl->k_Right )
            {
                pPlayer->Next_Direction = 1;
            }
            if( g_MyD3D->MyCtrl->k_Left )
            {
                pPlayer->Next_Direction = 0;
            }
            return 0;
        }
        return 0;
    }

    if( pPlayer->cFrame >= 6 && pPlayer->cFrame <= 15 && pPlayer->Next_Motion != -1 )
    {
        if( pPlayer->Next_Direction != -1 )
        {
            pPlayer->bIsRight = (pPlayer->Next_Direction == 1);
        }
        pPlayer->SetPlayerState( pPlayer->Next_Motion );
        pPlayer->cFrame = 0;
        pPlayer->Next_Attack_Count = 0;
        return 0;
    }

    if(pPlayer->cFrame >= (int)pPlayer->GetNowMotionFRM()->GetNum_Frame() -1)        //무한 반복하는 프레임..
    {
        pPlayer->SetPlayerState( MID_SKILL_ELESIS1_SPECIAL_ATK2_END_WAIT );
        pPlayer->cFrame = 0;
    }
    return 0;
}

int KGCPlayerElesis::OnSkillSpecialAtk2DashCombo( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( (pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Right) ||
            (!pPlayer->bIsRight && g_MyD3D->MyCtrl->k_Left) )
        {
            if( pPlayer->cFrame < 20 )
            {
                if( g_kGlobalValue.CheckTrainedSkill( pPlayer, SID_ELESIS1_SPECIAL_ATK2 ) )
                {
                    if( pPlayer->uiMotion == MID_SKILL_ELESIS1_SPECIAL_ATK2_1 )
                    {
                        pPlayer->Next_Motion = MID_SKILL_ELESIS1_SPECIAL_ATK2_1_2_DASH;
                        return 0;
                    }
                    else if( pPlayer->uiMotion == MID_SKILL_ELESIS1_SPECIAL_ATK2_2 )
                    {
                        pPlayer->Next_Motion = MID_SKILL_ELESIS1_SPECIAL_ATK2_2_3_DASH;
                        return 0;
                    }
                }
            }
        }
        return 0;
    }

    if( pPlayer->cFrame == 20 && pPlayer->Next_Motion != -1 )
    {
        pPlayer->SetPlayerState( pPlayer->Next_Motion );
        pPlayer->cFrame = 0;
        return 0;
    }
    return 0;
}

int KGCPlayerElesis::OnSkillSpecial2_Lv2( PLAYER* pPlayer, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( pPlayer->cFrame >= 8 && pPlayer->cFrame <= 38 )
        {
            if( g_MyD3D->MyCtrl->k_Right )
            {
                pPlayer->x_Speed = pPlayer->GetCurFormTemplate().fRunSpeed;		
                return 0;
            }
            else if( g_MyD3D->MyCtrl->k_Left )
            {
                pPlayer->x_Speed = -pPlayer->GetCurFormTemplate().fRunSpeed;
                return 0;
            }
            else if( g_MyD3D->MyCtrl->k_Down )
            {
                if(KGCPlayerCommon::CheckPortal(pPlayer,bKeyProc)==true)
                    return 0;

                pPlayer->y_Speed = -0.01f;
                pPlayer->NoCheckContact = 20;
                return 0;
            }
            else
            {
                pPlayer->x_Speed = 0.0f;
                return 0;
            }
        }
    }
    return 0;
}

int KGCPlayerElesis::OnSkillComboAttack(PLAYER* pPlayer, bool bKeyProc)
{
    if(bKeyProc)
    {
        if( g_MyD3D->MyCtrl->k_Fresh_Punch )
        {

            if( g_MyD3D->MyCtrl->k_Fresh_Punch )
            {
                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_ATK2
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_ATK3)
                {
                    pPlayer->Next_Attack_Count = 255;
                    return 0;
                }    

                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_DOWN 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_DOWN_ATK2
                    )
                {
                    pPlayer->Next_Attack_Count = 255;
                    return 0;
                }    

                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_UP_2 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_UP_2_ATK2
                    )
                {
                    pPlayer->Next_Attack_Count = 255;
                    return 0;
                }    

                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_DOWN_2 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_DOWN_2_ATK2
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_COMBO_A_DOWN_2_ATK3)
                {
                    pPlayer->Next_Attack_Count = 255;
                    return 0;
                }    

                if( pPlayer->Next_Attack_Count > 0 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2 
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK2
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK3
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK4
                    || pPlayer->uiMotion == MID_SKILL_ELESIS1_NEWCOMBO_A_UP_2_ATK5)
                {
                    pPlayer->Next_Attack_Count = 255;
                    return 0;
                }    
            }
        }
    }
    return 0;
}
