#include "StdAfx.h"
#include "TGClearOfStage.h"
#include "QuestGameManager.h"



//////////////////////////////////////////////////////////////////////////
CTGClearOfStage::CTGClearOfStage()
{
    enType              = TG_CLEAR_STAGE;
}

CTGClearOfStage::~CTGClearOfStage()
{

}

//************************************************************************
// 스테이지 클리어 실행
//************************************************************************
void CTGClearOfStage::ClearStage(void)
{
    g_pkQuestManager->DoStageClearProcess();

    g_pkQuestManager->m_bClearStage = true;
    SetComplete( true );
}

//************************************************************************
// 스테이지 클리어 처리
//************************************************************************
void CTGClearOfStage::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        ClearStage();
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
        ClearStage();
    }

    KGCPC_MEMO( "Stage cleared" );
}

HRESULT CTGClearOfStage::LoadItemFromLua(KLuaManager &luaMgr)
{
    return S_OK;
}