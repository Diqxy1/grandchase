#include "stdafx.h"
#include ".\HardDamageBlitchen.h"
#include "Monster.h"
#include "DamageManager.h"
#include "Buff\KGCBuffManager.h"


CHardDamageBlitchen::CHardDamageBlitchen(void)
:dwTime(0)
{
}

CHardDamageBlitchen::~CHardDamageBlitchen(void)
{
}

void CHardDamageBlitchen::FrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::FrameMove(pDamageInstance);

    if( false == g_MyD3D->IsPlayerIndex( pDamageInstance->m_HitWho ) )
        return;
    if ( pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() || g_MyD3D->IsSameTeam(g_MyD3D->Get_MyPlayer(),pDamageInstance->m_Who) )
        return;

    if ( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetPlayerState() == PS_DEAD )
        return;

    if( pDamageInstance->m_What == DT_PET_BLITZCHEN_ATTACK_2 )
    {
        if ( pDamageInstance->m_Life == 1 )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
            g_MyD3D->m_pDamageManager->AddWithLocate(DT_PET_BLITZCHEN_ATTACK_3, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, -1, NULL, pDamageInstance->m_HitWho );	
        }
    }


    if( pDamageInstance->m_What == DT_PET_BLACK_BLITZCHEN_ATTACK_2 )
    {
        if ( pDamageInstance->m_Life == 1 )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
            g_MyD3D->m_pDamageManager->AddWithLocate(DT_PET_BLACK_BLITZCHEN_ATTACK_3, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, -1, NULL, pDamageInstance->m_HitWho );	
        }
    }
}

void CHardDamageBlitchen::CheckAndDebuff(CDamageInstance* pDamageInstance)
{
    //남미는 흑왕 및 블리첸 AP 회복
#if defined( NATION_LATINAMERICA )
    if ( pDamageInstance->m_What == DT_PET_BLITZCHEN_ATTACK_1 || pDamageInstance->m_What == DT_PET_BLACK_BLITZCHEN_ATTACK_1 ) { 
        if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLITCHEN_SP_UP)==false)
        {
            //마족 케릭터만 SP찹니당.
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.5f);
                g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLITCHEN_SP_UP, 5.0f);
            }						
        }
    }
    if ( pDamageInstance->m_What == DT_PET_BLACK_BLITZCHEN_ATTACK_1 ) { 
        if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLITCHEN_SP_UP)==false)
        {
            //마족 케릭터만 SP찹니당.
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.5f);
                g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLITCHEN_SP_UP, 5.0f);
            }						
        }
    }
#else 
    if ( pDamageInstance->m_What == DT_PET_BLACK_BLITZCHEN_ATTACK_1 ) { 
        if (g_pMagicEffect->IsMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLITCHEN_SP_UP)==false)
        {
            //마족 케릭터만 SP찹니당.
            if (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Is_SP_Character()==true)
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP(0.5f);
                g_pMagicEffect->SetMagicEffect(pDamageInstance->m_Who,EGC_EFFECT_BLITCHEN_SP_UP, 5.0f);
            }						
        }
    }
#endif
}

void CHardDamageBlitchen::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
    CheckAndDebuff(pDamageInstance);
    CDamage::CrashMonster(pDamageInstance,Monsteri);
    return;
}

bool CHardDamageBlitchen::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( pDamageInstance->m_What == DT_PET_BLITZCHEN_ATTACK_1 )
    {    
        pDamageInstance->m_HitWho = hitWho;
        std::map< int, std::vector<CDamageInstance*> >::iterator itor;

        for( itor= g_MyD3D->m_pDamageManager->GetDamageInstancePool().begin(); itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end() ; ++itor )
        {
            std::vector<CDamageInstance*>::iterator vit;
            for (vit=itor->second.begin() ;vit !=itor->second.end() ;vit++)
            {
                CDamageInstance* pDamageOld = *vit;

                // 저 데미지고.. 이미 한번 저 데미지를 맞았었다면.. 이전 데미지는 사라져!
                if ( pDamageOld->m_What == DT_PET_BLITZCHEN_ATTACK_2 && pDamageOld->m_HitWho == hitWho )
                {
                    pDamageOld->m_Life = 0;
                }
            }
        }

        PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
        if( pPlayer && pPlayer->IsSuperNotSpecial(false) == false ) { 
            g_MyD3D->m_pDamageManager->AddWithLocate(DT_PET_BLITZCHEN_ATTACK_2, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, -1, NULL, pDamageInstance->m_HitWho ); 
        }
    }

    if( pDamageInstance->m_What == DT_PET_BLACK_BLITZCHEN_ATTACK_1 )
    {    
        pDamageInstance->m_HitWho = hitWho;
        std::map< int, std::vector<CDamageInstance*> >::iterator itor;

        for( itor= g_MyD3D->m_pDamageManager->GetDamageInstancePool().begin(); itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end() ; ++itor )
        {
            std::vector<CDamageInstance*>::iterator vit;
            for (vit=itor->second.begin() ;vit !=itor->second.end() ;vit++)
            {
                CDamageInstance* pDamageOld = *vit;

                // 저 데미지고.. 이미 한번 저 데미지를 맞았었다면.. 이전 데미지는 사라져!
                if ( pDamageOld->m_What == DT_PET_BLACK_BLITZCHEN_ATTACK_2 && pDamageOld->m_HitWho == hitWho )
                {
                    pDamageOld->m_Life = 0;
                }
            }
        }

        PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
        if( pPlayer && pPlayer->IsSuperNotSpecial(false) == false ) { 
            g_MyD3D->m_pDamageManager->AddWithLocate(DT_PET_BLACK_BLITZCHEN_ATTACK_2, MAX_PLAYER_NUM, pPlayer->vPos.x, pPlayer->vPos.y, false, -1, NULL, pDamageInstance->m_HitWho );
        }
    }

    return CDamage::CrashPlayer( pDamageInstance, hitWho, fDamageRate );
}