#include "stdafx.h"
#include ".\KGCBuffHeroEarring01.h"

#include "Controls.h"
#include "Damage.h"
#include "KGCRoomManager.h"
#include "KGCBuffManager.h"

GCUTIL::GCRand_Int KGCBuffHeroEarring01::ms_kRandom;

KGCBuffHeroEarring01::KGCBuffHeroEarring01(void)
:m_iPeriodTime(0)
,m_iShieldTime(0)
,m_iDungeonShieldHP(0)
,m_iOtherShieldHP(0)
,m_iProbability(0)
,m_iShieldType(EGC_EFFECT_HERO_EARRING_SHIELD)
,m_bSetShildType( false )
{
}

KGCBuffHeroEarring01::~KGCBuffHeroEarring01(void)
{
}

bool KGCBuffHeroEarring01::Init( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID )
{
	if ( KGCBuff::Init( iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID ) && pBuffInst_ )
    {
        m_mapStopWatch.insert(std::pair<int,KncStopWatch>(iPlayerIndex_,KncStopWatch()));
        m_mapStopWatch[iPlayerIndex_].SetStopWatch(5000);        
        
        MAP_HEROITEM_ITER mit = g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.find(pBuffInst_->m_iBuffEnum);

        if( mit != g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.end() )
        {
            int iShieldType = EGC_EFFECT_HERO_EARRING_SHIELD;

            switch( pBuffInst_->m_iBuffEnum ) 
            {
            case EGC_EFFECT_HERO_EARRING_01:
                iShieldType = EGC_EFFECT_HERO_EARRING_SHIELD;
                break;
            case EGC_EFFECT_HERO_EARRING_02:
                iShieldType = EGC_EFFECT_HERO_EARRING_SHIELD_2;
                break;
            case EGC_EFFECT_HERO_EARRING_03:
                iShieldType = EGC_EFFECT_HERO_EARRING_SHIELD_3;
                break;
            case EGC_EFFECT_APOSTOLO_NECKLACE:
                iShieldType = EGC_EFFECT_APOSTOLO_NECKLACE_SHIELD;
                break;
            case EGC_ETERNAL_SAGE_NOBLITAS_EFF:
                iShieldType = EGC_ETERNAL_SAGE_NOBLITAS_EFF_SHIELD;
                break;
            default:
                iShieldType = EGC_EFFECT_HERO_EARRING_SHIELD;
                break;
            }

            std::vector<float> vecCondition;
            if ( !g_pItemMgr->GetHeroItemCondition( mit->second, &vecCondition )  ) 
                return false;

            if( vecCondition.size() > 4 )
            {
                m_iShieldType = iShieldType;
                m_iPeriodTime = static_cast<int>(vecCondition[0]);
                m_iShieldTime = static_cast<int>(vecCondition[1]);
                m_iDungeonShieldHP = static_cast<int>(vecCondition[2]);
                m_iOtherShieldHP = static_cast<int>(vecCondition[3]);
                m_iProbability = static_cast<int>(vecCondition[4]);
            }
        }

		return true;
    }
	
	return false;
}

bool KGCBuffHeroEarring01::FrameMove( int iPlayerIndex_, KGCBuffInstance* pBuffInst_ )
{
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        return false;

    
    if ( !g_MyD3D->IsPlayerIndex( iPlayerIndex_ ) ) 
        return false;


    
    //  Shield Ÿ�� ���� ó��
    //  ��� �÷��̾�� ǥ���� ����� ��� ����� ó����.
    //  �ѹ��� �����ǰ� �Ѵ�.
    if( g_pMagicEffect->IsMagicEffect(iPlayerIndex_, m_iShieldType ) && 
        !g_pMagicEffect->IsCoolTime(iPlayerIndex_, m_iShieldType) )
    {
        if ( !m_bSetShildType ) 
        {
            g_MyD3D->MyPlayer[iPlayerIndex_]->ConfirmUseSheild( CID_MAX );
            m_bSetShildType = true;
        }
    }
    else
    {
        m_bSetShildType = false;
    }

    
    //  ������ ���� ����ؼ� �˷��ؤ���.
    if ( !g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer() ) 
        return false;

	if ( KGCBuff::FrameMove( iPlayerIndex_, pBuffInst_ ) )
	{
        if( g_pMagicEffect->IsMagicEffect(iPlayerIndex_, m_iShieldType ) && 
            !g_pMagicEffect->IsCoolTime(iPlayerIndex_, m_iShieldType) )
        {
            //  ���尡 �Ҹ�Ǿ��ų� �ٸ� ���尡 �ɸ��� ���� �����.
            //  �׳�ý��� �Ҵ��� �����. ������ ���� ��� ȿ�� ����
            if ( ( m_iShieldType == EGC_EFFECT_HERO_EARRING_SHIELD || m_iShieldType == EGC_EFFECT_HERO_EARRING_SHIELD_3 || m_iShieldType == EGC_EFFECT_APOSTOLO_NECKLACE_SHIELD || m_iShieldType == EGC_ETERNAL_SAGE_NOBLITAS_EFF_SHIELD)
                   && ( g_MyD3D->MyPlayer[iPlayerIndex_]->Shield <= 0 || g_MyD3D->MyPlayer[iPlayerIndex_]->m_cUseShield != CID_MAX ))
            {
                g_MyD3D->MyPlayer[iPlayerIndex_]->ClearMagicEffect( m_iShieldType );
            }
        }

        if( g_pMagicEffect->IsMagicEffect( iPlayerIndex_, m_iShieldType ) && g_pMagicEffect->IsCoolTime( iPlayerIndex_, m_iShieldType ) && m_iShieldType == EGC_EFFECT_APOSTOLO_NECKLACE_SHIELD )
        {
            g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleSecondBaseMeshAll( false );
            g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleBaseMesh( true);
        }

        if( m_mapStopWatch[iPlayerIndex_].IsStop() && !g_pMagicEffect->IsMagicEffect(iPlayerIndex_, m_iShieldType ))
        {
            srand((unsigned )time(NULL) );
            int randomNum = ms_kRandom.rand()%100;

            //std::ofstream file;
            //file.open( "hahaha.txt", std::ios::app );

            //file << "BuffIndex : " <<  m_iShieldType  << "\trandomNum : " << randomNum << "\tm_iProbability : " << m_iProbability << "\tResult : " << (randomNum < m_iProbability ? 
            //    "true" : "false ") << std::endl;
            //file.flush();
            //file.close();

            if( randomNum < m_iProbability )
            {
                if ( m_iShieldType == EGC_EFFECT_HERO_EARRING_SHIELD ||
                     m_iShieldType == EGC_EFFECT_HERO_EARRING_SHIELD_2 ||
                     m_iShieldType == EGC_EFFECT_HERO_EARRING_SHIELD_3 ||
                     m_iShieldType == EGC_EFFECT_APOSTOLO_NECKLACE_SHIELD ||
                     m_iShieldType == EGC_ETERNAL_SAGE_NOBLITAS_EFF_SHIELD)
                {
                    if( iPlayerIndex_ > -1 )
                    {
                        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
                        {                       
                            g_MyD3D->MyPlayer[iPlayerIndex_]->SetShield( m_iDungeonShieldHP, 0.f, m_iShieldTime, true);
                        }
                        else
                        {
                            g_MyD3D->MyPlayer[iPlayerIndex_]->SetShield( m_iOtherShieldHP, 0.f, m_iShieldTime, true);
                        }
                    }
                }

                g_MyD3D->MyPlayer[iPlayerIndex_]->SetMagicEffect( m_iShieldType, static_cast<float>(m_iShieldTime) );
            }

            m_mapStopWatch[iPlayerIndex_].SetStopWatch(m_iPeriodTime * 1000);
        }
	}

	return true;
}

bool KGCBuffHeroEarring01::Destroy( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_ )
{
    m_mapStopWatch.erase(iPlayerIndex_);
	if ( KGCBuff::Destroy( iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_ ) )
		return true;

	return false;
}