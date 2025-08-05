#include "StdAfx.h"
#include ".\tgnextstage.h"
//

#include "EveryTriggerManager.h"
#include "MyD3D.h"
#include "QuestGameManager.h"

CTGNextStage::CTGNextStage(void)
{
    m_iLevel            = 0;
    m_iDecisionStage    = 0;
    enType              = TG_NEXT_STAGE;
}

CTGNextStage::~CTGNextStage(void)
{
}

void CTGNextStage::NextStage(void)
{
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        //************************************************************************
        // 스테이지 경로를 확률에 따라서 랜덤으로 선택해주는 부분
        vector<int> vtRandTable;
        for(int j=0;j<(int)m_vtStage.size();j++)
        {
            for(int k=0;k<m_vtPercent[j];k++)
                vtRandTable.push_back(m_vtStage[j]);
        }
        //************************************************************************

        if( vtRandTable.size() )
        {
            // 섞어주쟈 ㅎㅎ 
            std::random_shuffle(vtRandTable.begin(), vtRandTable.end());
            int iRand           = ::rand()%vtRandTable.size();
            m_iDecisionStage    = vtRandTable[iRand];
            TileMap tilemap = g_pkQuestManager->m_Quest.FindStage(m_iDecisionStage);
            g_kEveryTriggerManager.SetNextStage(tilemap);
        }
    }
    else
    {
        TileMap tilemap = g_pkQuestManager->m_Quest.FindStage(m_iDecisionStage);
        g_kEveryTriggerManager.SetNextStage(tilemap);
    }
    KGCPC_MEMO( "Go to next stage" );
    g_MyD3D->GetMyPlayer()->SetPlayerState(g_MyD3D->GetMyPlayer()->GetWaitMotion());
    g_pkQuestManager->m_bClearStage = false;
    SetComplete( true );
}

void CTGNextStage::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        NextStage();

        if( !m_bSendPacket )
        {
            // 패킷을 보낸다.
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacket->SetExtraValue( m_iDecisionStage );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        NextStage();
    }
}

HRESULT CTGNextStage::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_BEGIN_TABLE("Stage", return E_FAIL);
    m_vtStage.clear();
    for(int i=1;;i++)
    {
        int iValue = 0;
        if( FAILED(luaMgr.GetValue(i, iValue)) ) 
            break;
        m_vtStage.push_back(iValue);
    }
    LUA_END_TABLE(return E_FAIL);

    LUA_BEGIN_TABLE("Percent", return E_FAIL);
    m_vtPercent.clear();
    for(int i=1;;i++)
    {
        int iValue = 0;
        if( FAILED(luaMgr.GetValue(i, iValue)) ) 
            break;
        m_vtPercent.push_back(iValue);
    }
    LUA_END_TABLE(return E_FAIL);

    return S_OK;

}

bool CTGNextStage::CheckEqualCondition(CTriggerCondition *pCondition)
{
    if( CTriggerCondition::CheckEqualCondition(pCondition) )
    {
        m_iDecisionStage = ((CTGNextStage*)pCondition)->m_iDecisionStage;
        return true;
    }

    return false;
}
