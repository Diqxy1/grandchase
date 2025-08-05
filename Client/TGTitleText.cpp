#include "StdAfx.h"
#include ".\tgtitletext.h"


#include "GCUI/GCGameOverlayUI.h"
#include "gcui/GCAlertInDungeon.h"
//

CTGTitleText::CTGTitleText(void)
{
    enType              = TG_TITLETEXT;
    m_iTitleTextString	= -1;
}

CTGTitleText::~CTGTitleText(void)
{
}

void CTGTitleText::TitleText(void)
{
    SetComplete( true );
    // 스트링 넘버 여기 잘 안쓴다..
	
	if( /*MAX_NUM_STRING > m_iTitleTextString && */m_iTitleTextString >= 0 )
	{
		if (g_pkGameOverlayUI->m_pkAlerInDungeon)
			g_pkGameOverlayUI->m_pkAlerInDungeon->SetStaticDesc(m_iTitleTextString);
		else
			g_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( m_iTitleTextString ) );
	}
}

void CTGTitleText::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        TitleText();

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
        TitleText();
    }
}

HRESULT CTGTitleText::LoadItemFromLua(KLuaManager &luaMgr)
{
    if( FAILED(luaMgr.GetValue("TitleTextStringID", m_iTitleTextString)) ) return E_FAIL;

    return S_OK;
}
