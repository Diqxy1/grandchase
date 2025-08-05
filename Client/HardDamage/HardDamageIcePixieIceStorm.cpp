#include "stdafx.h"
#include ".\HardDamageIcePixieIceStorm.h"

#include "KGCRoomManager.h"
#include "Monster.h"
#include "buff\KGCBuffManager.h"

KHardDamageIcePixieIceStorm::KHardDamageIcePixieIceStorm(void)
{
}

KHardDamageIcePixieIceStorm::~KHardDamageIcePixieIceStorm(void)
{
}


bool KHardDamageIcePixieIceStorm::CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) )
    {
        // �ѹ��� �´´� �÷��̾
		if ( pDamageInstance->InsertAlreadyDamaged( hitWho ) == false ) {
			return false;
		}

        if( pDamageInstance->m_IsMonsterDamage ) // ���Ϳ��� �¾��� ��
        {
            pDamageInstance->m_pOriginalDamage->GetDamageRate( fDamageRate, g_kMonsterManager.m_mapMonster[pDamageInstance->m_ExtraMonsterNum], g_MyD3D->MyPlayer[hitWho], pDamageInstance );

            // Attacker����( ��ȯ�� ���Ͱ� Attacker��� �÷��̾� Index�� ����������. )
            int iAttacker = pDamageInstance->m_ExtraMonsterNum + MAX_PLAYER_NUM;
            if( g_kMonsterManager.IsSummonMonsterByPlayer(pDamageInstance->m_ExtraMonsterNum) )
            {
                MONSTER *pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
				if( pMonster ) {
					PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
					if( pPlayer )
						iAttacker = pPlayer->m_iPlayerIndex;
				}
            }
            g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( iAttacker );

            g_MyD3D->MyPlayer[hitWho]->m_fFreezeDamage = m_fDamageToPlayer * fDamageRate;

            switch( pDamageInstance->m_What )
            {
                case DT_BLACKFAIRYQUEEN_BREATH:
                case DT_MONSTER_KAMIKI05:
                case DT_MONSTER_WENDY03:
                case DT_HERO_KAMIKI_ICEFIELD:
                case DT_EXTINCTION_MON_KAMIKI_PHASE1_ICEFIELD:
                case DT_EXTINCTION_MON_KAMIKI_PHASE2_ICEFIELD:
                case DT_ARME_SKILL_ICESTRIKE_ICEFIELD:
                    g_MyD3D->MyPlayer[hitWho]->m_iPlusFreezeCount = 4;
                    break;
				case DT_SABERTOOTH_HOWL_GAS:
				case DT_FREEZING_KENKRE_BREATH:
					g_MyD3D->MyPlayer[hitWho]->m_iPlusFreezeCount = 20;
					break;
                //case DT_ARME3_ICEBALL:
                    //g_MyD3D->MyPlayer[hitWho]->m_iPlusFreezeCount = 8;
                    //break;
                default:
                    if ( m_fDecreasePerPush )
                        g_MyD3D->MyPlayer[hitWho]->m_iPlusFreezeCount = (int)m_fDecreasePerPush;
                    else
                        g_MyD3D->MyPlayer[hitWho]->m_iPlusFreezeCount = 4;
                    break;
            }
        }
        else // �÷��̾��ѵ� �¾�����.. ( ���̽��Ƚ� )
        {
            g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer( pDamageInstance->m_Who );
            PLAYER::DamageInfo damage;
            damage.SetDamage( m_fDamageToPlayer );
            damage.SetDamageInstance( pDamageInstance );
			g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

			if ( m_fDecreasePerPush )
			{
				g_MyD3D->MyPlayer[hitWho]->m_iPlusFreezeCount = (int)m_fDecreasePerPush;
			}			
        }

        g_MyD3D->MyPlayer[hitWho]->SetPlayerState( MID_COMMON_FREEZED_BY_PIXIE );
        g_MyD3D->MyPlayer[hitWho]->cFrame = 0;        
    }

    // ONETIME�̸� ������ ���������!!
    if ( (m_dwDMAttribute & DA_ONETIME_DAMAGE) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

    return true;
}


void KHardDamageIcePixieIceStorm::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    //�÷��̾� ���״� �Ѵ븸. �������״� ������.
    //pDamageInstance->m_bAlreadyDamaged[ Monsteri + MAX_PLAYER_NUM ] = false;
    //CDamage::CrashMonster(pDamageInstance , Monsteri);
    bool bCheckSpeical = false;
    bool bPetDamage = false;
    switch( pDamageInstance->m_What )
    {
    case DT_ICEPIXIE_ICEBREATH:
    case DT_PET_GWEN_CHAIN_ATK:
    case DT_PET_RURU_ATTACK2:
    case DT_PET_ICEDRAGON_ATTACK1:
    case DT_PET_ICEDRAGON_ATTACK2:
    case DT_PET_ICEDRAGON_ATTACK3:
        bCheckSpeical = false;
        bPetDamage = true;
        break;
    default:
        bCheckSpeical = true;
        break;   
    }

    if( pDamageInstance->m_What == DT_EDEL1_UPGRADE_SPECIAL1_TYPE2_ATK4 ||
        pDamageInstance->m_What == DT_EDEL2_SPECIAL_A_1_ICE_ATK ||
        pDamageInstance->m_What == DT_EDEL2_SPECIAL_A_2_ICE_ATK ) {
            if ( (m_dwDMAttribute & DA_ONETIME_DAMAGE) && pDamageInstance->m_Life > 1) {
                pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
                pDamageInstance->m_Life = 1;
            } else if( pDamageInstance->InsertAlreadyDamaged( Monsteri ) == false ) {
                return;
            }
    }

    // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
    g_kMonsterManager.m_mapMonster[Monsteri]->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    g_kMonsterManager.m_mapMonster[Monsteri]->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;
#if defined( LEVEL_DESIGN_STAT_CALC )
    g_kMonsterManager.m_mapMonster[Monsteri]->OnDamage( pDamageInstance->m_Who,pDamageInstance->m_What,
        pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1.0f,
        ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, bCheckSpeical, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade, true, false, bPetDamage,false,0,false, pDamageInstance->m_ExtraMonsterNum);
#else
    g_kMonsterManager.m_mapMonster[Monsteri]->OnDamage( pDamageInstance->m_Who,pDamageInstance->m_What,
        pDamageInstance->m_pOriginalDamage->m_fDamageToMonster * -1000.0f,
        ATTACKTYPE_ICECURSE, ATTACKDIR_NORMAL, 0.0f, 0.0f, bCheckSpeical, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade, true, false, bPetDamage );
#endif

    // SP ȸ�� �� 
    if( pDamageInstance->m_What == DT_PET_ICEDRAGON_ATTACK1 || pDamageInstance->m_What == DT_PET_ICEDRAGON_ATTACK2 || pDamageInstance->m_What == DT_PET_ICEDRAGON_ATTACK3 ) { 
        if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLACK_DRAGON_SP_UP)==false)
        {
            //���� �ɸ��͸� SP���ϴ�.
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(1.25f);
                g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLACK_DRAGON_SP_UP, 5.0f);
            }						
        }	
    }    
}
