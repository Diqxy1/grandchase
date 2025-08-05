#include "stdafx.h"
#include ".\HardDamagePet.h"
#include "..\LatencyTransfer.h"

void CHardDamagePetSeamusBuff::End( CDamageInstance* pDamageInstance )
{
    if (pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() )
        return;

    int iBuffLevel = ( rand() %4 ) + 1;

    // 0: ���ݷ� 2% ����
    // 1: ���� 2% ����
    // 2: ���ݷ� 2% ����, ���� 2% �϶�
    // 3: ���ݷ� 2% �϶�, ���� 2% ����

    // �Ʒ� MAX_PLAYER ������ �ȵ��� �Ϸ��� �߰�����.
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
