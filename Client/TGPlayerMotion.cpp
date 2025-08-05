#include "StdAfx.h"
#include ".\tgplayermotion.h"
#include "MyD3D.h"
//



CTGPlayerMotion::CTGPlayerMotion(void)
{
    enType      = TG_PLAYERMOTION;
    m_iSlot     = 0;
    m_iMotion   = 0;
}

CTGPlayerMotion::~CTGPlayerMotion(void)
{
}

void CTGPlayerMotion::PlayerMotion(void)
{
    if( g_MyD3D->Get_MyPlayer() == m_iSlot )
    {
        // 하드코딩 --
        // 어떤 방향으로 제작될지 아직 완전히 결정이 안되어있습니다.
        // 지금은 일단 이상태로만 구현해두기로 합니다.
        vector<int> vtMotion;
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ATK1);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ATKEND);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetWaitMotion());
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetFatalWaitMotion());
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().WALK);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SPECIAL1_W);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SPECIAL2_W);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SPECIAL3_W);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SPECIAL1_B);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SPECIAL2_B);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SPECIAL3_B);        
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ORIGINAL1_W);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ORIGINAL2_W);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ORIGINAL3_W);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ORIGINAL1_B);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ORIGINAL2_B);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ORIGINAL3_B);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().DASH);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().DASH_ATK);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().WIN);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().START);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SELECT);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().RESULT_WIN);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().RESULT_LOSE);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetJokeMotion());
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().RUN);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP_DASH_ATK);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().ATK1_FASTINPUT);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP_ATK);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP_DASH);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().JUMP_READY);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().DOUBLE_JUMP);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().DOUBLE_JUMP_ATK);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().DOUBLE_JUMP_LAND);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().CRITICAL_ATK);
		vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().BURNNING_ACTIVE);
		vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().BURNNING_CHARGE);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().DOWN_STAND_UP_EVASION);
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SID_EXPANSION_MANA );
        vtMotion.push_back(g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SID_SPECIAL4);

        g_MyD3D->GetMyPlayer()->cFrame = 0;
        g_MyD3D->GetMyPlayer()->SetPlayerState( g_MyD3D->GetMyPlayer()->GetWaitMotion());
        g_MyD3D->GetMyPlayer()->SetPlayerState( vtMotion[m_iMotion] );
    }

    SetComplete( true );
}

void CTGPlayerMotion::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        PlayerMotion();

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
        PlayerMotion();
    }
}

HRESULT CTGPlayerMotion::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Motion", m_iMotion, return E_FAIL);

    return S_OK;
}
