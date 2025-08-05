#include "stdafx.h"
#include ".\HardDamageAmy1SkillTree.h"
#include "Stage.h"
#include "LTPacket.h"
#include "DamageManager.h"
#include "LatencyTransfer.h"



void CHardDamageAmy1DivineMsg::BeginParticle( CDamageInstance* pDamageInstance )
{
    if( !g_MyD3D->IsMyTeam( pDamageInstance->m_Who ) )
    {
        pDamageInstance->m_pOriginalDamage->m_vecChildParticle = g_MyD3D->m_pDamageManager->GetOriginalDamage(DT_AMY_DIVINE_MESSAGE_HEAL)->m_vecChildParticle;
        pDamageInstance->m_pOriginalDamage->m_vecCrashParticle = g_MyD3D->m_pDamageManager->GetOriginalDamage(DT_AMY_DIVINE_MESSAGE_HEAL)->m_vecCrashParticle;
    }
    CDamage::BeginParticle( pDamageInstance );
}

bool CHardDamageAmy1DivineMsg::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /*= 1.0f */ )
{
    if( g_MyD3D->IsSameTeam(pDamageInstance->m_Who, hitWho ) )
    {
        pDamageInstance->m_Life = 1;
        return false;
    }

    return CDamage::CrashPlayer( pDamageInstance, hitWho , fDamageRate );
}




void CHardDamageAmy1Abracadabra::End( CDamageInstance* pDamageInstance )
{
    if (pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() )
        return;

    int r = rand() %100;

    //1. ÀüÃ¼ Ã¼·Â È¸º¹	13
    //2. ÀüÃ¼ ¸¶³ª È¸º¹	12
    //3. ¹öÇÁ ½Ã°£ µ¿¾È Ä«¿îÅÍ ½Ã ¸¶³ª ¼Ò¸ð ¾øÀ½	11
    //4. °ø°Ý·Â Áõ°¡	11
    //5. ¹æ¾î·Â Áõ°¡	11
    //6. »ý¸í È¸º¹·Â Áõ°¡	11
    //7. ±×¸®°í ¾Æ¹«ÀÏµµ ÀÏ¾î³ªÁö ¾Ê¾Ò´Ù.	11
    //1. ¾Æ±ºÀÇ ÀüÃ¼ Ã¼·Â °¨¼Ò	3
    //2. ¾Æ±ºÀÇ ÀüÃ¼ ¸¶³ª °¨¼Ò	3
    //3. ÀÏÁ¤ ½Ã°£ µ¿¾È ¸¶³ª°¡ ÀÚµ¿ °¨¼ÒÇÑ´Ù.	3
    //4. ¾Æ±ºÀÇ °ø°Ý·Â °¨¼Ò	3
    //5. ¾Æ±ºÀÇ ¹æ¾î·Â °¨¼Ò	3
    //6. ÀÏÁ¤ ½Ã°£ µ¿¾È Ã¼·ÂÀÌ È¸º¹ µÇÁö ¾Ê´Â´Ù.	3

    int randTable[] = { 
        12,
        12,
        13,
        13,
        13,
        12,
        3,
        3   ,
        3   ,
        3   ,
        3   ,
        3   ,
        7   ,
    };

    int timeTable[] = { 
        3,
        3,
        15,
        20,
        20,
        20,
        3,
        3,
        10,
        10,
        10,
        10,
        0,
    };

    int iIndex = 0;
    for( int i = 0; i < sizeof( randTable ); i++ )
    {
        if( r < randTable[i] )
        {
            iIndex += i;
            break;
        }
        r -= randTable[i];
    }
    int iBuffStart = EGC_AMY_ABRACADABRA_1; 
    for( int i = 0 ; i <MAX_PLAYER_NUM ; i++ ) 
    {

        if( g_MyD3D->IsMyTeam( i ) && g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) ) 
            g_LatencyTransfer.SendMagicEffectSynchPacket( iBuffStart + iIndex, (float)timeTable[iIndex], true,i,1);
    }
    
    int iSound = DT_AMY_ABRACADABRA_GOOD_SOUND;
    if( iIndex > 6 )
        iSound = DT_AMY_ABRACADABRA_BAD_SOUND;

    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->AddDamageOnP2P( iSound );

    LTP_TITLE_STRING packet;
    packet.iStringID = 5298 + iIndex;
    packet.iCharIndex = pDamageInstance->m_Who;
    packet.bSameTeamOnly = false;
    g_LatencyTransfer.PushPacket( &packet );

    


    
}
void CHardDamageAmy1AbracadabraDun::End(CDamageInstance* pDamageInstance)
{
    if (pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer())
        return;

    int r = rand() % 100;

    //1. 전체 체력 회복	13
    //2. 전체 마나 회복	12
    //3. 버프 시간 동안 카운터 시 마나 소모 없음	11
    //4. 공격력 증가	11
    //5. 방어력 증가	11
    //6. 생명 회복력 증가	11
    //7. 그리고 아무일도 일어나지 않았다.	11
    //1. 아군의 전체 체력 감소	3
    //2. 아군의 전체 마나 감소	3
    //3. 일정 시간 동안 마나가 자동 감소한다.	3
    //4. 아군의 공격력 감소	3
    //5. 아군의 방어력 감소	3
    //6. 일정 시간 동안 체력이 회복 되지 않는다.	3

    int randTable[] = {
        15,
        15,
        20,
        10,
        5,
        5,
        30,
        0,
        0,
        0,
        0,
        0,
        0,
    };

    int timeTable[] = {
        3,
        5,
        20,
        7,
        10,
        30,
        3,
        3,
        10,
        10,
        10,
        10,
        0,
    };

    int iIndex = 0;
    for (int i = 0; i < sizeof(randTable); i++)
    {
        if (r < randTable[i])
        {
            iIndex += i;
            break;
        }
        r -= randTable[i];
    }
    int iBuffStart = EGC_AMY_ABRACADABRA_DUN_1;
    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {

        if (g_MyD3D->IsMyTeam(i) && g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING))
            g_LatencyTransfer.SendMagicEffectSynchPacket(iBuffStart + iIndex, (float)timeTable[iIndex], true, i, 1);
    }

    int iSound = DT_AMY_ABRACADABRA_GOOD_SOUND;
    if (iIndex > 5)
        iSound = DT_AMY_ABRACADABRA_BAD_SOUND;

    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->AddDamageOnP2P(iSound);

    LTP_TITLE_STRING packet;
    packet.iStringID = 14201 + iIndex;
    packet.iCharIndex = pDamageInstance->m_Who;
    packet.bSameTeamOnly = true;
    g_LatencyTransfer.PushPacket(&packet);
}