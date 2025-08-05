#include "stdafx.h"
#include ".\HardDamagePet.h"
#include "..\LatencyTransfer.h"

void CHardDamagePetSeamusBuff::End( CDamageInstance* pDamageInstance )
{
    if (pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() )
        return;

    int iBuffLevel = ( rand() %4 ) + 1;

    // 0: 공격력 2% 증가
    // 1: 방어력 2% 증가
    // 2: 공격력 2% 증가, 방어력 2% 하락
    // 3: 공격력 2% 하락, 방어력 2% 증가

    // 아래 MAX_PLAYER 루프를 안돌게 하려고 추가했음.
    int iMyPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex(iMyPlayer) )
    {
        g_LatencyTransfer.SendMagicEffectSynchPacket( EGC_PET_SEAMUS_BUFF, 10, true,iMyPlayer,iBuffLevel);
    }
    //for( int i = 0 ; i <MAX_PLAYER_NUM ; i++ ) 
    //{
    //    if( g_MyD3D->MyPlayer[i]->IsLocalPlayer() == true ) 
    //        g_LatencyTransfer.SendMagicEffectSynchPacket( EGC_PET_SEAMUS_BUFF, 10, true,i,iBuffLevel);
    //}
}
