#include "StdAfx.h"
#include ".\tgtimer.h"

#include "gcui/GCGameOverlayUI.h"


CTGTimer::CTGTimer(void)
{
    m_iUI       = 0;
    m_bTimerOn  = false;
    m_fTime     = 0;
    m_fRemain   = 0;
    enType      = TG_TIMER;
	m_bStop		= false;
	m_iType		= 0;
}

CTGTimer::~CTGTimer(void)
{
}

void CTGTimer::StartTrigger(void)
{
    CTriggerCondition::StartTrigger();
    m_bTimerOn = 0;
    ResetTimer();
}

//void CTGTimer::TimerOn(bool bOn)
//{
//    m_bTimerOn = bOn;
//}

void CTGTimer::ResetTimer(void)
{
    m_fRemain = m_fTime;
}

void CTGTimer::SetTime(float fTime)
{
    m_fTime         = fTime;
    m_fRemain      = m_fTime;

}

void CTGTimer::Time(void)
{
    // UI 를 표시 하는 상태면 UI를 설정함.
    if( m_iUI == 1 )
    {
		m_iType = 1;
        m_iUI = 0;
        g_pkGameOverlayUI->SetTimer(true, m_fTime);
    }
	if ( m_iUI == 2 ){
		m_iUI = 0;
		m_iType = 2;
		m_bStop = true;
		g_pkGameOverlayUI->SetSpecialMonsterTimer( static_cast<int>( m_fTime ));
	}


	

    m_fRemain -= GC_ELAPSED_TIME;
    if( m_fRemain <= 0 )
    {
        SetComplete( true );
        return;
    }

    SetComplete( false );
}

void CTGTimer::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
		if ( m_bStop && g_pkInput->IsDown( DIK_ESCAPE ) ) {
			m_fRemain = 0.f;
		}        

		Time();

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
        Time();
    }
}

HRESULT CTGTimer::LoadItemFromLua(KLuaManager &luaMgr)
{
    float fTimer;
    luaMgr.GetValue( "IsUI", m_iUI );
    LUA_GET_VALUE("Timer", fTimer, return E_FAIL);
    fTimer /= 100;      // ms 타입으로 설정해두기 때문에 초단위로 변경해준다.
    SetTime(fTimer);
    return S_OK;
}

void CTGTimer::EndTrigger(void)
{
	if( m_iType == 2 )
		g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_TG_TIMER_END ) );

}
