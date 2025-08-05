#include "stdafx.h"
#include ".\HardDamageStoneCurse.h"
#include "KGCPlayerCommon.h"
#include "Monster.h"
#include "KGCRoomManager.h"
#include "DamageManager.h"

CHardDamageStoneCurse::CHardDamageStoneCurse(void)
{
	m_iMonsterIndex = -1;
	m_bIsCrashMonster = false;
}

CHardDamageStoneCurse::~CHardDamageStoneCurse(void)
{
}

void CHardDamageStoneCurse::Begin( CDamageInstance* pDamageInstance )
{
	CDamage::Begin( pDamageInstance );
}

void CHardDamageStoneCurse::FrameMove(CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );	
	
	if( m_iMonsterIndex == -1 || !m_bIsCrashMonster)
		return;

	if( pDamageInstance->m_What == DT_ARME_SKILL_STONE_CURSE_LV2_DAMAGE )
	{
		MONSTER *pMonster = g_kMonsterManager.GetMonster( m_iMonsterIndex);
		if( pMonster == NULL ) return;

		pDamageInstance->m_X = pMonster->vRenderPos.x;
		pDamageInstance->m_Y = pMonster->vRenderPos.y;;
	}

}

bool CHardDamageStoneCurse::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
	{
        if( g_MyD3D->MyPlayer[hitWho]->MetamorphosisForm == FORM_RIN_GODDESS || g_MyD3D->MyPlayer[hitWho]->MetamorphosisForm == FORM_RIN_EVIL ) {
            return false;
        }

		if( pDamageInstance->m_Who != MAX_PLAYER_NUM )
		{
			g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( pDamageInstance->m_Who );
		}		
		else
        {
            // Attacker����( ��ȯ�� ���Ͱ� Attacker��� �÷��̾� Index�� ����������. )
            int iAttacker = pDamageInstance->m_ExtraMonsterNum + MAX_PLAYER_NUM;
			if( g_kMonsterManager.IsSummonMonsterByPlayer(pDamageInstance->m_ExtraMonsterNum) )
            {
                MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
				if ( pMonster ){
					PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
					if( pPlayer )
						iAttacker = pPlayer->m_iPlayerIndex;
				}
            }
            g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( iAttacker );
        }

        if (pDamageInstance->m_What == DT_PET_MEDUSA_ATTACT1) {            
            g_Push_Count = 0;
            g_MyD3D->MyPlayer[hitWho]->SetPlayerState(MID_COMMON_STONECURSE_BY_MEDUSA);
            g_MyD3D->MyPlayer[hitWho]->cFrame    = 0;
            g_MyD3D->MyPlayer[hitWho]->bIsRight  = true;
            g_MyD3D->MyPlayer[hitWho]->x_Speed   = 0;
            g_MyD3D->MyPlayer[hitWho]->m_bIsContinousDamage = pDamageInstance->m_pOriginalDamage->m_bIsContinousDamage;
            g_MyD3D->MyPlayer[hitWho]->m_fContinousDamage = pDamageInstance->m_pOriginalDamage->m_fContinousDamage;

        } else {
            if( g_MyD3D->MyPlayer[hitWho]->IsContact ) {	
                g_Push_Count = 0;

                g_MyD3D->MyPlayer[hitWho]->SetPlayerState(MID_COMMON_STONECURSE);
                g_MyD3D->MyPlayer[hitWho]->cFrame    = 0;
                g_MyD3D->MyPlayer[hitWho]->bIsRight  = true;
                g_MyD3D->MyPlayer[hitWho]->x_Speed   = 0;
                g_MyD3D->MyPlayer[hitWho]->m_bIsContinousDamage = pDamageInstance->m_pOriginalDamage->m_bIsContinousDamage;
                g_MyD3D->MyPlayer[hitWho]->m_fContinousDamage = pDamageInstance->m_pOriginalDamage->m_fContinousDamage;
            } else {
                g_MyD3D->MyPlayer[hitWho]->iContact_to_Stone = g_MyD3D->MyPlayer[hitWho]->GetLastKillmePlayer();
            }
        }		
	}

	g_MyD3D->MyPlayer[hitWho]->m_iStoneCurseType = pDamageInstance->m_What;
	return true;
}
void CHardDamageStoneCurse::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
	MONSTER *pMonster = g_kMonsterManager.GetMonster( Monsteri );
	if ( pMonster == NULL ) return;

    // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
    pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
    
    if( pDamageInstance->m_What == DT_ARME3_ICE_STONE_CURSE )
    {
        // ���� 3����.. �������� ���������� ���鼭.. ����� ����!!
#if defined( LEVEL_DESIGN_STAT_CALC )
        pMonster->OnDamage( pDamageInstance->m_Who,pDamageInstance->m_What,
			pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1.0f,
                                                ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, 
                                                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#else
        pMonster->OnDamage( pDamageInstance->m_Who,pDamageInstance->m_What,
			pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
                                                ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, 
                                                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#endif

    }
    else if ( pDamageInstance->m_What == DT_ARME3_ICE_STORM )
    {
        // ���� 3����.. �������� ���������� ���鼭.. ����� ����!!
#if defined( LEVEL_DESIGN_STAT_CALC )
        pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
            pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1.0f,
                                                ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true,
                                                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade);
#else
        pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
            pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
                                                ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true,
                                                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade);
#endif

    }
    else
    {
		if( pDamageInstance->m_What == DT_ARME_SKILL_STONE_CURSE_LV2_DAMAGE )
		{
			g_MyD3D->m_pDamageManager->AddWithLocate(DT_ARME_SKILL_STONECURSE_LV2_MONDAMAGE,pDamageInstance->m_Who,
				pMonster->vRenderPos.x + 1.0f,
				pMonster->vRenderPos.y + 0.6f, true );
			m_bIsCrashMonster = true;
			m_iMonsterIndex = Monsteri;
		}

		if ( pDamageInstance->InsertAlreadyDamaged( Monsteri + MAX_PLAYER_NUM ) == false ) {
			return;
		}

        // ���Ϳ��� ����Ŀ��.. 
#if defined( LEVEL_DESIGN_STAT_CALC )
        pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What,
			pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1.0f,
                                                ATTACKTYPE_STONECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, 
                                                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#else
        pMonster->OnDamage( pDamageInstance->m_Who, pDamageInstance->m_What,
			pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
                                                ATTACKTYPE_STONECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, true, 
                                                pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
#endif
    }
}


void CHardDamageStoneBody::FrameMove( CDamageInstance* pDamageInstance )
{
	CDamage::FrameMove( pDamageInstance );

	if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetOwnMotion( MID_COMMON_STONECURSE ) &&
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion != g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetOwnMotion( MID_COMMON_STONECURSE_BY_MEDUSA ) )
	{
		pDamageInstance->m_Life = 1;
	}
}
