#include "StdAfx.h"
#include ".\tgfadeinout.h"

#include "gcui/GCGameOverlayUI.h"


CTGFadeInOut::CTGFadeInOut(void)
{
    enType          = TG_FADEINOUT;
    m_bOnOff        = false;
    m_dwAlpha       = 0;
}

CTGFadeInOut::~CTGFadeInOut(void)
{
}

void CTGFadeInOut::StartTrigger(void)
{
    CTriggerCondition::StartTrigger();
	m_dwAlpha = 0;
}

void CTGFadeInOut::FadeInOut(void)
{
    //g_pkQuestManager->m_bLoading = m_bOnOff;
		if( m_bOnOff )
        {
            //g_pkGameOverlayUI->SetWindowColor(D3DCOLOR_ARGB(255-m_dwAlpha, 0xff, 0xff, 0xff));
			g_pkGameOverlayUI->SetLoadingTextureColor(D3DCOLOR_ARGB(255-m_dwAlpha, m_Red, m_Green, m_Blue));
        }
        else
        {
            //g_pkGameOverlayUI->SetWindowColor(D3DCOLOR_ARGB(m_dwAlpha, 0xff, 0xff, 0xff));
			g_pkGameOverlayUI->SetLoadingTextureColor(D3DCOLOR_ARGB(m_dwAlpha, m_Red, m_Green, m_Blue));
        }
   

    m_dwAlpha += m_iSpeed;
    if( m_dwAlpha > 255 )
    {
		if( m_bOnOff )
			g_pkGameOverlayUI->SetLoadingTextureColor(D3DCOLOR_ARGB(0x00, m_Red, m_Green, m_Blue));
		else
			g_pkGameOverlayUI->SetLoadingTextureColor(D3DCOLOR_ARGB(0xff, m_Red, m_Green, m_Blue));

        SetComplete(true);
    }
}

void CTGFadeInOut::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        FadeInOut();

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
        FadeInOut();
    }
}

HRESULT CTGFadeInOut::LoadItemFromLua(KLuaManager &luaMgr)
{
	LUA_GET_VALUE( "Speed", m_iSpeed, return E_FAIL );
	LUA_GET_VALUE( "Red", m_Red, return E_FAIL );
	LUA_GET_VALUE( "Green", m_Green, return E_FAIL );
	LUA_GET_VALUE( "Blue", m_Blue, return E_FAIL );
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);

    return S_OK;
}