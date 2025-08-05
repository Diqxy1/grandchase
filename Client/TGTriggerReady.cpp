#include "StdAfx.h"
#include ".\tgtriggerready.h"
//

#include "EveryTriggerManager.h"

CTGTriggerReady::CTGTriggerReady(void)
{
    enType              = TG_TRIGGER_READY;
}

CTGTriggerReady::~CTGTriggerReady(void)
{
}

void CTGTriggerReady::TriggerReady(void)
{
    // 트리거 매니져 돌면서 트리거 이름이 같은놈들 실행.
    std::string strTGName = m_szTriggerName;
    CTrigger *pFindTrigger = g_kEveryTriggerManager.FindTrigger(strTGName);
    if( pFindTrigger )
    {
        if( !g_kEveryTriggerManager.FindActiveTrigger(strTGName) ) g_kEveryTriggerManager.AddActiveTrigger(pFindTrigger);
        pFindTrigger->StartTrigger();
        pFindTrigger->SetActive(false);
        pFindTrigger->m_bCheckByHost      = false;
        pFindTrigger->m_bSendPacket       = false;
        pFindTrigger->SetComplete( false );
        SetComplete( true );
    }
}

void CTGTriggerReady::FrameMove(void)
{
    //if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        TriggerReady();

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
        TriggerReady();
    }
}

HRESULT CTGTriggerReady::LoadItemFromLua(KLuaManager &luaMgr)
{
    if( FAILED(luaMgr.GetValue("TriggerName", m_szTriggerName, MAX_TG_STR_LENTH)) ) return E_FAIL;

    return S_OK;
}