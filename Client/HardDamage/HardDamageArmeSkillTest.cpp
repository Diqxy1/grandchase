#include "stdafx.h"
#include ".\HardDamageArmeSkillTest.h"
#include "Headup Display.h"
#include "KGC3DObject.h"

#include "KGCRoomManager.h"

#include "Monster.h"

CHardDamageArmeSkillTest::CHardDamageArmeSkillTest(void)
{
	m_iTime = 0;
	m_xRate = 1;
	m_yRate = 1;	
	m_iTargetNum = 0;
}

CHardDamageArmeSkillTest::~CHardDamageArmeSkillTest(void)
{
}


void CHardDamageArmeSkillTest::NotAngleFrameMove(CDamageInstance* pDamageInstance )
{
	float fDistanceX = 0.0f, fDistanceY = 0.0f;
	float fSpeedX = 0.0f, fSpeedY = 0.0f;
	bool bPlayerLive = false;

	if( pDamageInstance->m_What == DT_ARME_SKILL_BLOODPEST_TRACE_OBJECT ||
		pDamageInstance->m_What == DT_STATUE_HOMMING_MISSILE ||
		pDamageInstance->m_What == DT_PERIOT_HOMMING_MISSILE_HP ||
        pDamageInstance->m_What == DT_SUPERMON_PERIOT_HOMMING_MISSILE_HP || 
		pDamageInstance->m_What == DT_DKMARK_II_TOYHOMMING ||
		pDamageInstance->m_What == DT_DARKTEMPLAR_SOUL_DRAIN ||
        pDamageInstance->m_What == DT_THANATOS2_GHOSTOFDEATH ||
        pDamageInstance->m_What == DT_TALIN_SMART_HOMMING ||
        pDamageInstance->m_What == DT_SANDRULER_ATK4 ||
        pDamageInstance->m_What == DT_SUPERMON_SANDRULER_ATK4 || 
        pDamageInstance->m_What == DT_THUNDER_HAMMER_DUEL_ATK2 ||
        pDamageInstance->m_What == DT_HERO_GADOSEN_LIGHTOFHELL ||
        pDamageInstance->m_What == DT_HERO_KAMIKI_ATK1 ||
        pDamageInstance->m_What == DT_HERO_NEMOPHILLA_MIND_STARTER ||
        pDamageInstance->m_What == DT_HELL_HOWLING_GHOST ||
        pDamageInstance->m_What == DT_HELL_HOWLING_GHOST_VULCANUS ||
        pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK1 ||
        pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK1 ||
        pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE1_LIGHTOFHELL ||
        pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE2_LIGHTOFHELL
        )
	{
		if( m_iTime == 0 )
		{	
			if ( pDamageInstance->m_IsMonsterDamage )
			{
                if ( pDamageInstance->m_What == DT_SANDRULER_ATK4 ||
                    pDamageInstance->m_What == DT_SUPERMON_SANDRULER_ATK4 ||
                    pDamageInstance->m_What == DT_THUNDER_HAMMER_DUEL_ATK2 ||
                    pDamageInstance->m_What == DT_HERO_KAMIKI_ATK1 ||
                    pDamageInstance->m_What == DT_HERO_NEMOPHILLA_MIND_STARTER ||
                    pDamageInstance->m_What == DT_HELL_HOWLING_GHOST_VULCANUS ||
                    pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK1 ||
                    pDamageInstance->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK1
                    )
                    m_iTargetNum = pDamageInstance->m_HitWho;
                else
				    m_iTargetNum = MostClosedPlayerIndex(pDamageInstance);
			}
			else
			{
				if( SiKGCRoomManager()->IsMonsterGameMode() )
				{
					m_iTargetNum = MostClosedMonsterIndex(pDamageInstance);
				}
				else
				{
					m_iTargetNum = MostClosedPlayerIndex(pDamageInstance);
				}
			}
		}
		
		if ( pDamageInstance->m_IsMonsterDamage )
		{
			for( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
			{
				if( g_MyD3D->MyPlayer[i]->m_cLife > 0 && g_MyD3D->MyPlayer[i]->GetPlayerState() != PS_DEAD )
				{
					bPlayerLive = true;
					break;
				}
			}

			if( !bPlayerLive )
			{
				pDamageInstance->m_Life = 0;
				return;
			}

			if( g_MyD3D->IsPlayerIndex( m_iTargetNum ) )
			{
				m_vPos.x = g_MyD3D->MyPlayer[m_iTargetNum]->GetPosition().x;				
				m_vPos.y = g_MyD3D->MyPlayer[m_iTargetNum]->GetPosition().y + 0.1f; 
			}
			else
			{
				pDamageInstance->m_Life = 0;
			}
		}
		else
		{
			if( SiKGCRoomManager()->IsMonsterGameMode() )
			{
				for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
					MONSTER* pMonster = mit->second;
					if( (pMonster->m_fHP > 0.0f && pMonster->m_bLive && pMonster->IsInvincible() == false ) )
					{
						bPlayerLive = true;
						break;
					}

				}

				if( !bPlayerLive )
				{
					pDamageInstance->m_Life = 0;
					return;
				}

				if( m_iTargetNum != -1 )
				{
					MONSTER *pMonster = g_kMonsterManager.GetMonster(m_iTargetNum);
					if(pMonster != NULL){
						m_vPos.x = pMonster->GetX();				
						m_vPos.y = pMonster->GetY() + 0.1f; 
					}
				}			
				else
				{
					pDamageInstance->m_Life = 0;
				}
			}
			else
			{
				for( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
				{
					if( g_MyD3D->MyPlayer[i]->GetHP() > 0.0f && !g_MyD3D->IsSameTeam( pDamageInstance->m_Who, i) )
					{
						bPlayerLive = true;
						break;
					}
				}

				if( !bPlayerLive )
				{
					pDamageInstance->m_Life = 0;
					return;
				}

				if( g_MyD3D->IsPlayerIndex( m_iTargetNum ) )
				{
					m_vPos.x = g_MyD3D->MyPlayer[m_iTargetNum]->GetPosition().x;				
					m_vPos.y = g_MyD3D->MyPlayer[m_iTargetNum]->GetPosition().y + 0.1f; 
				}
				else
				{
					pDamageInstance->m_Life = 0;
				}
			}
		}
	}
	else if(pDamageInstance->m_What == DT_ARME_SKILL_BLOODPEST_CATCHED_MP )
	{
        if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
        {
		    m_vPos.x = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().x;
		    m_vPos.y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().y + 0.1f;
        }
	}	

	fDistanceX = ASMfabs(m_vPos.x - pDamageInstance->m_X);
	fDistanceY = ASMfabs(m_vPos.y - pDamageInstance->m_Y);

	if( fDistanceX > fDistanceY )
	{
		m_xRate =1;
		m_yRate = (int)(fDistanceX/fDistanceY+0.5f);
	}
	else if(fDistanceX < fDistanceY )
	{
		m_xRate = (int)(fDistanceY/fDistanceX+0.5f);
		m_yRate = 1;
	}
	else
	{
		m_xRate = 1;
		m_yRate = 1;
	}


	fSpeedX = m_x_Speed / m_xRate;
	fSpeedY = m_y_Speed / m_yRate;

    bool bRight = true;
	if( pDamageInstance->m_X < m_vPos.x )
	{
		pDamageInstance->m_X += fSpeedX;
	}
	else
	{
		pDamageInstance->m_X -= fSpeedX;
        bRight = false;
	}

	if( pDamageInstance->m_Y < m_vPos.y )
	{
		pDamageInstance->m_Y += fSpeedY;
		//pDamageInstance->m_Y += 0.1f;
	}
	else
	{
		pDamageInstance->m_Y -= fSpeedY;
		//pDamageInstance->m_Y += 0.1f;
	}

    if( pDamageInstance->m_What == DT_SANDRULER_ATK4 || pDamageInstance->m_What == DT_SUPERMON_SANDRULER_ATK4 )
    {
        pDamageInstance->m_IsRight = bRight;
        pDamageInstance->m_p3DObject->IsRight( bRight );
    }

	if(pDamageInstance->m_What == DT_ARME_SKILL_BLOODPEST_CATCHED_MP )
	{
		if( (m_vPos.x - 0.05f <= pDamageInstance->m_X && m_vPos.x + 0.05f >= pDamageInstance->m_X) &&
			(m_vPos.y - 0.05f <= pDamageInstance->m_Y && m_vPos.y + 0.05f >= pDamageInstance->m_Y))
		{
			pDamageInstance->m_Life = 0;
			if( SiKGCRoomManager()->IsMonsterGameMode() )
			{
				MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_HitWho);
				if( pMonster ){
					if( pMonster->m_fHP > 0 )
						g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.9f);
				}
			}
			//	맞은넘이 소환수다.
			else if ( pDamageInstance->m_emHitWhoType == OBJ_TYPE_MONSTER )
			{
				if ( g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_HitWho ) &&
					g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
				{
					MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_HitWho);
					if( pMonster ){
						if( pMonster->m_fHP > 0 )
							g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.9f);
					}

				}
			}
			else
			{
				if ( g_MyD3D->IsPlayerIndex( pDamageInstance->m_HitWho ) &&
					 g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
				{
					if( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetHP() > 0 )
						g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.9f);
				}
			}			
		}
	}
	
    m_iTime++;               //1프레임을 1초로 계산하자.
}

int CHardDamageArmeSkillTest::MostClosedPlayerIndex(CDamageInstance* pDamageInstance)
{
	int iIndex = 0, iMinIndex = 0;
	KSafeArray<int,MAX_PLAYER_NUM> iTargetNum;
	KSafeArray<float,MAX_PLAYER_NUM> fDistance;
	bool bLive = false;
	KSafeArray<D3DXVECTOR3,MAX_PLAYER_NUM> vPos;

	for( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife > 0 &&
			(pDamageInstance->m_IsMonsterDamage ? true : !g_MyD3D->IsSameTeam( pDamageInstance->m_Who, i)) )
		{
			vPos[iIndex] = g_MyD3D->MyPlayer[i]->GetPosition();
			iTargetNum[iIndex++] = i;
			bLive = true;
		}
	}
	
	for( int i = 0 ; i < iIndex ; i++ )
	{
		fDistance[i] = ASMsqrt( pow(vPos[i].x - pDamageInstance->m_X, 2) + pow(vPos[i].y - pDamageInstance->m_Y, 2));
	}

	for( int i = 0 ; i < iIndex ; i++ ) 
	{
		if( fDistance[iMinIndex] > fDistance[i] )
			iMinIndex = i;
		else 
			iMinIndex = iMinIndex;
	}

	if( bLive )
		return iTargetNum[iMinIndex];

	return -1;
}

int CHardDamageArmeSkillTest::MostClosedMonsterIndex( CDamageInstance* pDamageInstance)
{
	std::set<int> setTargetNum;
	std::map<int,float> mapfDistance;
	std::map<int,D3DXVECTOR2> mapPos;

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pMonster->m_bLive && 
			!g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first) &&
			pMonster->m_fHP > 0 && 
			!pMonster->IsInvincible() )
		{
			mapPos.insert(std::pair<int,D3DXVECTOR2>(mit->first, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY() ) ));
			setTargetNum.insert(mit->first);
		}

	}

	if( setTargetNum.empty() == true )
		return -1;
		
	for ( std::set<int>::iterator sit = setTargetNum.begin() ; sit != setTargetNum.end() ; ++sit ) {
		mapfDistance.insert(
			std::pair<int,float>(
			*sit, ASMsqrt( pow(mapPos[*sit].x - pDamageInstance->m_X, 2) + pow(mapPos[*sit].y - pDamageInstance->m_Y, 2))));
	}

	int iTarget = *(setTargetNum.begin());

	for ( std::set<int>::iterator sit = setTargetNum.begin() ; sit != setTargetNum.end() ; ++sit ) {
		if( mapfDistance[iTarget] > mapfDistance[*sit] )
			iTarget = *sit;
	
	}

	return iTarget;
}

bool CHardDamageArmeSkillTest::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( g_MyD3D->MyPlayer[hitWho]->IsSpecialSkillMotion() )
        return false;

	if ( false == CDamage::CrashPlayer(pDamageInstance, hitWho, fDamageRate) )
		return false;
	
	if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
		return true;

	if ( pDamageInstance->m_ExtraMonsterNum < 0 )
		return true;

	MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

	if (pMonster == NULL)
		return false;

	if ( pDamageInstance->m_What == DT_PERIOT_HOMMING_MISSILE_HP ||
         pDamageInstance->m_What == DT_SUPERMON_PERIOT_HOMMING_MISSILE_HP ||
         pDamageInstance->m_What == DT_HERO_GADOSEN_LIGHTOFHELL ||
         pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE1_LIGHTOFHELL ||
         pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE2_LIGHTOFHELL )
	{
		pMonster->m_fHP += (pMonster->m_fHPPoint * 0.03f);

		if ( pMonster->m_fHP > pMonster->m_fHPPoint )
			pMonster->m_fHP = pMonster->m_fHPPoint;

		if( pDamageInstance->m_What == DT_HERO_GADOSEN_LIGHTOFHELL ||
            pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE1_LIGHTOFHELL ||
            pDamageInstance->m_What == DT_EXTINCTION_MON_GADOSEN_PHASE2_LIGHTOFHELL)
		{
			D3DXVECTOR3 m_vParticlePos = pMonster->GetBonePos( 4 );

			if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
			{
				g_ParticleManager->CreateSequence("Dark_Templar_Soul_Drain_Absorb_01", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f);
				g_ParticleManager->CreateSequence("Dark_Templar_Soul_Drain_Absorb_02", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f);
				g_ParticleManager->CreateSequence("Dark_Templar_Soul_Drain_Absorb_03", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f);
			}
		}
	}
	else if ( pDamageInstance->m_What == DT_DARKTEMPLAR_SOUL_DRAIN )
	{
		D3DXVECTOR3 m_vParticlePos = pMonster->GetBonePos( 4 );
		if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
		{
			g_ParticleManager->CreateSequence("Dark_Templar_Soul_Drain_Absorb_01", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f);
			g_ParticleManager->CreateSequence("Dark_Templar_Soul_Drain_Absorb_02", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f);
			g_ParticleManager->CreateSequence("Dark_Templar_Soul_Drain_Absorb_03", m_vParticlePos.x - 1.0f, m_vParticlePos.y - 0.48f, 0.5f);
		}

		if ( g_MyD3D->IsPlayerIndex( hitWho ) && g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
		{
			g_MyD3D->MyHead->Level = 0.0f;

			pMonster->m_fHP += (pMonster->m_fHPPoint * 0.25f);

			if ( pMonster->m_fHP > pMonster->m_fHPPoint )
				pMonster->m_fHP = pMonster->m_fHPPoint;
		}
	}

	return true;
}
