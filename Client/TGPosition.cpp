#include "StdAfx.h"
#include ".\tgposition.h"


#include "Monster.h"



#define TG_DOOR_REAMIN_TIME 55

CTGPosition::CTGPosition(void)
{
    m_bInOut        = true;
    m_pPlayer       = 0;
    m_pMonster      = 0;
    m_bPlayer       = true;
    m_bHalfMore     = false;
    enType          = TG_POSITION;
}

CTGPosition::~CTGPosition(void)
{
}

bool CTGPosition::InRect(float x, float y)
{
    if( x >= m_rtBound.left && m_rtBound.right >= x
        && y >= m_rtBound.top && m_rtBound.bottom >= y
        )
    {
        return true;
    }

    return false;
}

bool CTGPosition::CheckInOut(float x, float y)
{
    bool bInRect = InRect(x, y);
    
    if( !m_bInOut ) return !bInRect;        // 영역 밖인지 확인한다면 InRect의 역수를 리턴합니다.

    return bInRect;         // 영역 안인지 확인한다면 그대로 리턴합니다.
}

void CTGPosition::SetRect(RECT rtBound)
{
    m_rtBound = rtBound;
}

void CTGPosition::MonsterPosCheck(void)
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iIndex);
	if( pMonster != NULL )
        SetComplete( CheckInOut(pMonster->m_afX[0]*400, pMonster->m_afY[0]*400) );
}

void CTGPosition::PlayerPosCheck(void)
{
    if( m_bHalfMore )
    {
        int iNumOfLive          = 0;
        int iNumOfCheckInOut        = 0;
        for(int i=0;i<MAX_PLAYER_NUM;++i)
        {
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive 
                && (g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_ALIVE || g_MyD3D->MyPlayer[i]->m_ePlayerState == PS_FATAL)
                )
            {
                 if( CheckInOut(g_MyD3D->MyPlayer[i]->vPos.x*400, g_MyD3D->MyPlayer[i]->vPos.y*400) )
                    ++iNumOfCheckInOut;

                ++iNumOfLive;
            }
        }
        // 과반수.. 안쓴다 -_-;; 지금은 그냥 모두 모였을때로 하겠다!
        if( iNumOfLive == iNumOfCheckInOut )
        {
            --m_dwReaminTime;
            if( m_dwReaminTime <= 0 )
                SetComplete( true );
        }
        else
        {
            m_dwReaminTime = TG_DOOR_REAMIN_TIME;
        }

/*
        // 플레이 인원이 2명 이하인 경우 : 모든 캐릭터가 해당 문에 위치해야함...
        // 플레이 인원이 2명 이상인 경우 : 과반수가 해당 문에 위치해야함...
        if( iNumOfLive > 2 )
        {
            if( iNumOfLive/2 < iNumOfCheckInOut )
            {
                --m_dwReaminTime;
                if( m_dwReaminTime <= 0 )
                    SetComplete( true );
            }
            else
            {
                m_dwReaminTime = TG_DOOR_REAMIN_TIME;
            }
        }
        else
        {
            if( iNumOfLive == iNumOfCheckInOut )
            {
                --m_dwReaminTime;
                if( m_dwReaminTime <= 0 )
                    SetComplete( true );
            }
            else
            {
                m_dwReaminTime = TG_DOOR_REAMIN_TIME;
            }
        }
        */
    }
    else
    {
        if( m_iIndex >= 0 && m_iIndex < MAX_PLAYER_NUM )
            SetComplete( CheckInOut(g_MyD3D->MyPlayer[m_iIndex]->vPos.x*400, g_MyD3D->MyPlayer[m_iIndex]->vPos.y*400) );
    }
}

void CTGPosition::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( m_bPlayer )
            PlayerPosCheck();
        else
            MonsterPosCheck();

        // 위치 체크의 경우 방장을 기준으로 합니다.
        // 방장이 위치확인을 완료 하는 순간 패킷을 날리며,
        // 방원들은 바로 위치확인을 완료합니다.
        if( IsComplete() == true && m_bSendPacket == false )
        {
            // 패킷을 보낸다.
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        SetComplete( true );
    }
}

HRESULT CTGPosition::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("InOut", m_bInOut, return E_FAIL);
    LUA_GET_VALUE("Left", m_rtBound.left, return E_FAIL);
    LUA_GET_VALUE("Right", m_rtBound.right, return E_FAIL);
    LUA_GET_VALUE("Top", m_rtBound.top, return E_FAIL);
    LUA_GET_VALUE("Bottom", m_rtBound.bottom, return E_FAIL);
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("HalfMore", m_bHalfMore, return E_FAIL);
    LUA_GET_VALUE("Index", m_iIndex, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
    return S_OK;
}

void CTGPosition::StartTrigger(void)
{
    CTriggerCondition::StartTrigger();

    m_dwReaminTime = TG_DOOR_REAMIN_TIME;
}