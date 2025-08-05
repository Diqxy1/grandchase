#include "StdAfx.h"
#include ".\tgtriggeronoff.h"
//

#include "EveryTriggerManager.h"

CTGTriggerOnOff::CTGTriggerOnOff(void)
{
    enType              = TG_TRIGGER_ONOFF;
    m_bHasChar          = true;
    m_vecCharType.clear();
}

CTGTriggerOnOff::~CTGTriggerOnOff(void)
{
}

void CTGTriggerOnOff::TriggerOnOff(void)
{
    // 한명이라도 타입조건이 맞지않으면 트리거를 실행하지 않는다.
    if( false == CheckCharType() )
    {
        SetComplete( true );
        return;
    }

    // 트리거 매니져 돌면서 트리거 이름이 같은놈들 실행.
    std::string strTGName = m_szTriggerName;
    CTrigger *pFindTrigger = g_kEveryTriggerManager.FindTrigger(strTGName);
    if( pFindTrigger )
    {
        pFindTrigger->SetActive(m_bOnOff);
		if ( m_bOnOff == false ){
			pFindTrigger->EndTrigger();
		}

        SetComplete( true );
    }
}

bool CTGTriggerOnOff::CheckCharType()
{
    if( false == m_vecCharType.empty() )
    {
        int iLoop;
        for(iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            PLAYER *pPlayer = g_MyD3D->MyPlayer[iLoop];
            if( false == pPlayer->IsLive() )
                continue;
            
            std::vector<int>::iterator vecIter = std::find( m_vecCharType.begin(), m_vecCharType.end(), pPlayer->GetCurrentChar().iCharType );
            if( m_bHasChar )
            {
                if( vecIter == m_vecCharType.end() )
                    return false;
            }
            else
            {
                if( vecIter != m_vecCharType.end() )
                    return false;
            }
        }
    }

    return true;
}

void CTGTriggerOnOff::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        TriggerOnOff();

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
        TriggerOnOff();
    }
}

HRESULT CTGTriggerOnOff::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    if( FAILED(luaMgr.GetValue("TriggerName", m_szTriggerName, MAX_TG_STR_LENTH)) ) return E_FAIL;

    luaMgr.GetValue( "HasChar", m_bHasChar );
    if( SUCCEEDED(luaMgr.BeginTable( "Con_CharType" )) )
    {
        int iCharType=-1;
        m_vecCharType.clear();
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iCharType )); ++iLoop)
        {
            m_vecCharType.push_back( iCharType );
        }
        luaMgr.EndTable();
    }

    return S_OK;
}