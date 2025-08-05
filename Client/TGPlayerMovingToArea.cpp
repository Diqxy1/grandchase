#include "StdAfx.h"
#include ".\tgplayermovingtoarea.h"


#include "Monster.h"
#include "MapSystem/GCFantasticMap.h"

CTGPlayerMovingToArea::CTGPlayerMovingToArea(void)
{
    enType          = TG_MOVINGTOAREA;
    m_bPlayer       = false;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_iLeft         = 0;
    m_iRight        = 0;
    m_iTop          = 0;
    m_iBottom       = 0;
}

CTGPlayerMovingToArea::~CTGPlayerMovingToArea(void)
{
}

bool CTGPlayerMovingToArea::CheckExistNextBlock(float fPosX, float fPosY, bool bRight, float fWidth)
{
    KNextBlockCheck kNextCheck;
    kNextCheck.fX           = fPosX;
    kNextCheck.fY           = fPosY;
    kNextCheck.fWidth       = fWidth;
    kNextCheck.bIsRight     = bRight;

    return SiKGCFantasticMap()->IsExistNextBlock(kNextCheck);
}

void CTGPlayerMovingToArea::PlayerMovingToArea(void)
{
    float fLeft  =  m_iLeft / 400.0f;
    float fRight =  m_iRight / 400.0f;
    float fTop   =  m_iTop / 400.0f;
    float fBottom=  m_iBottom / 400.0f;

    if( m_bPlayer )
    {
        const float fCharWidth = g_MyD3D->GetMyPlayer()->GetCollisionRect().m_Right - g_MyD3D->GetMyPlayer()->GetCollisionRect().m_Left;
        const float fWalkSpeed = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fWalkSpeed;
        const float fJumpXSpeed = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fRunJumpXSpeed;
        const float fJumpYSpeed = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fRunJumpYSpeed;
        const float fPlayerXSpeed = g_MyD3D->GetMyPlayer()->x_Speed;
        const float fPlayerYSpeed = g_MyD3D->GetMyPlayer()->y_Speed;
        const float fPlayerXPos = g_MyD3D->GetMyPlayer()->vPos.x;
        const float fPlayerYPos = g_MyD3D->GetMyPlayer()->vPos.y;

        g_MyD3D->SetEnableKeyInput(false);
        if( g_MyD3D->Get_MyPlayer() == m_iSlot )
        {
            if( g_MyD3D->GetMyPlayer()->uiMotion != g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP
                && g_MyD3D->GetMyPlayer()->uiMotion != g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP_READY
                )
            {
                KNextBlockCheck kNextCheck;
                if( g_MyD3D->GetMyPlayer()->vPos.x > fRight )
                {
                    g_MyD3D->GetMyPlayer()->bIsRight = false;
                    if( CheckExistNextBlock(fPlayerXPos-fWalkSpeed, fPlayerYPos+fPlayerYSpeed, false, fCharWidth) )
                    {
                        g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().WALK);
                        g_MyD3D->GetMyPlayer()->x_Speed     = -fWalkSpeed;
                    }
                    else
                    {
                        g_MyD3D->GetMyPlayer()->cFrame  = 0;
                        g_MyD3D->GetMyPlayer()->x_Speed = -fWalkSpeed;
                        g_MyD3D->GetMyPlayer()->y_Speed = fJumpYSpeed;
                        g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP);
                    }
                }
                else if( g_MyD3D->GetMyPlayer()->vPos.x < fLeft )
                {
                    g_MyD3D->GetMyPlayer()->bIsRight = true;
                    if( CheckExistNextBlock(fPlayerXPos+fWalkSpeed, fPlayerYPos+fPlayerYSpeed, true, fCharWidth) )
                    {
                        g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().WALK);
                        g_MyD3D->GetMyPlayer()->x_Speed     = fWalkSpeed;
                    }
                    else
                    {
                        g_MyD3D->GetMyPlayer()->cFrame  = 0;
                        g_MyD3D->GetMyPlayer()->x_Speed = fWalkSpeed;
                        g_MyD3D->GetMyPlayer()->y_Speed = fJumpYSpeed;
                        g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP);
                    }
                }
                else if( g_MyD3D->GetMyPlayer()->vPos.y < fTop )
                {
                    g_MyD3D->GetMyPlayer()->cFrame = 0;
                    g_MyD3D->GetMyPlayer()->y_Speed = fJumpYSpeed;
                    g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP);
                }
                else if( g_MyD3D->GetMyPlayer()->vPos.y > fBottom )
                {
                    g_MyD3D->GetMyPlayer()->cFrame = 0;
                    g_MyD3D->GetMyPlayer()->y_Speed = -fJumpYSpeed;
                    g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP);
                }
                else
                {
                    g_MyD3D->SetEnableKeyInput(true);
                }
            }
            else
            {
                if( g_MyD3D->GetMyPlayer()->vPos.x > fRight )
                {
                    g_MyD3D->GetMyPlayer()->x_Speed = -fWalkSpeed;
                }
                else if( g_MyD3D->GetMyPlayer()->vPos.x < fLeft )
                {
                    g_MyD3D->GetMyPlayer()->x_Speed = fWalkSpeed;
                }
            }
        }
    }
    else
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iSlot);
		if( pMonster != NULL ){
            const float fCharWidth = pMonster->m_fWidth;
            const float fWalkSpeed = pMonster->m_fWalkSpeed;
            const float fMonXSpeed = pMonster->m_fSpeedX;
            const float fMonYSpeed = pMonster->m_fSpeedY;
            const float fMonXPos = pMonster->m_afX[ LATENCY_INDEX ];
            const float fMonYPos = pMonster->m_afY[ LATENCY_INDEX ];

            pMonster->SetMovement(FALSE);
            pMonster->SetFrameLock(FALSE);

            KNextBlockCheck kNextCheck;
            std::string strState = g_kMonsterManager.GetMonsterStateToIndex( pMonster->m_iMonsterType, pMonster->m_aiMotion[ LATENCY_INDEX ] );
            if( strState != "JUMP" )
            {
                if( fMonXPos > fRight )
                {
                    pMonster->m_abIsRight[LATENCY_INDEX] = false;
                    if( CheckExistNextBlock(fMonXPos-(fMonXSpeed-fWalkSpeed), fMonYPos+fMonYSpeed, false, fCharWidth) )
                    {
                        if( strState != "WALK" )
                            pMonster->DirectMotionInput("WALK");
                    }
                    else
                    {
                        pMonster->SetState("JUMP");
                        pMonster->m_aiFrame[LATENCY_INDEX] = 0;
                        pMonster->m_fSpeedY = 0.04f;
                    }
                }
                else if( fMonXPos < fLeft )
                {
                    pMonster->m_abIsRight[LATENCY_INDEX] = true;
                    if( CheckExistNextBlock(fMonXPos-(fMonXSpeed+fWalkSpeed), fMonYPos+fMonYSpeed, true, fCharWidth) )
                    {
                        if( strState != "WALK" )
                            pMonster->DirectMotionInput("WALK");
                    }
                    else
                    {
                        pMonster->SetState("JUMP");
                        pMonster->m_aiFrame[LATENCY_INDEX] = 0;
                        pMonster->m_fSpeedY = 0.04f;
                    }
                }
                else if( fMonYPos < fTop )
                {
                    pMonster->SetState("JUMP");
                    pMonster->m_aiFrame[LATENCY_INDEX] = 0;
                    pMonster->m_fSpeedY = 0.04f;
                }
                else if( fMonYPos > fBottom )
                {
                    pMonster->SetState("JUMP");
                    pMonster->m_aiFrame[LATENCY_INDEX] = 0;
                    pMonster->m_fSpeedY = 0.04f;
                }
            }
        }
    }
    
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( m_bPlayer )
        {
            if( g_MyD3D->Get_MyPlayer() == m_iSlot )
            {
                // 죽은 녀석이면 트리거 종료...
                if( g_MyD3D->GetMyPlayer()->IsDie() )
                {
                    g_MyD3D->SetEnableKeyInput(true);
                    SetComplete(true);
                    return;
                }

                if( g_MyD3D->GetMyPlayer()->vPos.x < fRight
                    && g_MyD3D->GetMyPlayer()->vPos.x > fLeft
                    && g_MyD3D->GetMyPlayer()->vPos.y > fTop
                    && g_MyD3D->GetMyPlayer()->vPos.y < fBottom
                    )
                {
                    SetComplete(true);
                }
            }
        }
        else
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iSlot);
			if( pMonster != NULL )
            {
                // 죽은녀석이면 트리거 종료...
                if( pMonster->IsDie() )
                {
                    pMonster->SetMovement(true);
                    pMonster->OnDie();
                    SetComplete(true);
                    return;
                }

                if( pMonster->m_afX[ LATENCY_INDEX ] < fRight
                    && pMonster->m_afX[ LATENCY_INDEX ] > fLeft
                    && pMonster->m_afY[ LATENCY_INDEX ] > fTop
                    && pMonster->m_afY[ LATENCY_INDEX ] < fBottom
                    )
                {
                    SetComplete(true);
                }
            }
        }
    }
}

void CTGPlayerMovingToArea::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        PlayerMovingToArea();

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
        PlayerMovingToArea();
    }
}

HRESULT CTGPlayerMovingToArea::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("Left", m_iLeft, return E_FAIL);
    LUA_GET_VALUE("Right", m_iRight, return E_FAIL);
    LUA_GET_VALUE("Top", m_iTop, return E_FAIL);
    LUA_GET_VALUE("Bottom", m_iBottom, return E_FAIL);

    return S_OK;
}
