#include "StdAfx.h"
#include ".\tgplayerposition.h"

#include "MyD3D.h"
#include "QuestGameManager.h"

CTGPlayerPosition::CTGPlayerPosition(void)
{
    m_bRight            = false;
    m_iPlayerIndex      = -1;
    m_fEntranceX        = 0.0f;
    m_fEntranceY        = 0.0f;
    enType              = TG_PLAYERPOSITION;
}

CTGPlayerPosition::~CTGPlayerPosition(void)
{
}

void CTGPlayerPosition::SetPlayerPosition(int iPlayerIndex, float fEntranceX, float fEntranceY)
{
    m_iPlayerIndex      = iPlayerIndex;
    m_fEntranceX        = fEntranceX;
    m_fEntranceY        = fEntranceY;
}

void CTGPlayerPosition::PlayerPosition(void)
{
    if( m_iPlayerIndex >= 0 && m_iPlayerIndex < MAX_PLAYER_NUM )
    {
        // 속도가 조금이라도 있어야지 해당 좌표로 캐릭터가 이동하더라.. 그래서 일부러 스피드를 준것임.
        g_MyD3D->MyPlayer[m_iPlayerIndex]->bIsRight = m_bRight;
        g_MyD3D->MyPlayer[m_iPlayerIndex]->x_Speed = 0.0001f;
        g_MyD3D->MyPlayer[m_iPlayerIndex]->y_Speed = 0.0001f;
        g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.x = m_fEntranceX*0.0025f;
        g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y = m_fEntranceY*0.0025f;

        g_MyD3D->m_pkQuestGameMgr->SetArenaPos(m_fEntranceX*0.0025f,m_fEntranceY*0.0025f);

        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
        {
        g_MyD3D->MyPlayer[m_iPlayerIndex]->SetDotaArenaPos(m_fEntranceX*0.0025f,m_fEntranceY*0.0025f);
        g_MyD3D->MyPlayer[m_iPlayerIndex]->Calculate_Arena_Pos();
        }

        KGCEmbarkObject* pEmbark = g_MyD3D->MyPlayer[m_iPlayerIndex]->GetMyEmbarkInstance();
        if( pEmbark )
        {
            int iCopyLatency;
            if( pEmbark->IsMyOwnerOfEmbarkObject() )
                iCopyLatency = (LATENCY + SHOWLATENCY - 1);
            else
                iCopyLatency = pEmbark->GetLatency();

            pEmbark->SetIsRight( g_MyD3D->MyPlayer[m_iPlayerIndex]->bIsRight );
            pEmbark->SetPos( g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.x, g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y, pEmbark->GetPosZ() );
            pEmbark->LatencyAllEqual( iCopyLatency );
        }

        for(int i=0;i<LATENCY+SHOWLATENCY;++i)
        {
            g_MyD3D->MyPlayer[m_iPlayerIndex]->L_IsRight[i]  = g_MyD3D->MyPlayer[m_iPlayerIndex]->bIsRight;
            g_MyD3D->MyPlayer[m_iPlayerIndex]->L_x[i]        = g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.x;
            g_MyD3D->MyPlayer[m_iPlayerIndex]->L_y[i]        = g_MyD3D->MyPlayer[m_iPlayerIndex]->vPos.y;
        }
    }

    SetComplete( true );
}

void CTGPlayerPosition::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        PlayerPosition();

        if( !m_bSendPacket )
        {
            // 패킷을 보낸다.
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        PlayerPosition();
    }
}

HRESULT CTGPlayerPosition::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE_DEF("Right", m_bRight, false);
    LUA_GET_VALUE("PlayerIndex", m_iPlayerIndex, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);

    return S_OK;
}
