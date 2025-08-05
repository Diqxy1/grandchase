#include "stdafx.h"
#include ".\KGCBuffLasMoveUp.h"
#include "MyD3D.h"
#include "KGCBuffManager.h"


KGCBuffLasSpeedUp::KGCBuffLasSpeedUp(void)
{
}


KGCBuffLasSpeedUp::~KGCBuffLasSpeedUp(void)
{
}

bool KGCBuffLasSpeedUp::Init( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID )
{
    if ( KGCBuff::Init( iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID ) )
    {
		if ( g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL )
		{
			switch ( g_MyD3D->MyPlayer[iPlayerIndex_]->Extra_Char_Num )
			{
			case CID_LAS1:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_LAS1_DASH;
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN = MID_SKILL_LAS1_RUN;
				break;
			case CID_LAS2:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_SKILL_LAS2_DASH;
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().JUMP_DASH = MID_SKILL_LAS2_JUMPDASH;
				break;
			default:
				break;
			}
		}

        return true;
    }

    return false;
}

bool KGCBuffLasSpeedUp::Destroy( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_ )
{
    if ( KGCBuff::Destroy( iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_ ) )
    {
		if ( g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL )
		{
			switch ( g_MyD3D->MyPlayer[iPlayerIndex_]->Extra_Char_Num )
			{
			case CID_LAS1:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LAS_DASH;
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN = MID_LAS_RUN;
				break;
			case CID_LAS2:
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LAS2_DASH_1;
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().JUMP_DASH = MID_LAS2_JUMPDASH;
				break;
			default:
				break;
			}
		}
		
		return true;
    }
    return false;
}

bool KGCBuffLasSpeedUp::FrameMove( int iPlayerIndex_, KGCBuffInstance* pBuffInst_ )
{
	if ( KGCBuff::FrameMove( iPlayerIndex_, pBuffInst_ ) )
	{
		if ( true == pBuffInst_->m_bPostCoolTime )
		{
			if ( g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL )
			{
				switch ( g_MyD3D->MyPlayer[iPlayerIndex_]->Extra_Char_Num )
				{
				case CID_LAS1:
					g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LAS_DASH;
					g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().RUN = MID_LAS_RUN;
					break;
				case CID_LAS2:
					g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().DASH = MID_LAS2_DASH_1;
					g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().JUMP_DASH = MID_LAS2_JUMPDASH;
					break;
				default:
					break;
				}
			}
		}
	}

	return true;
}

