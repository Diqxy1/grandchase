#include "StdAfx.h"
#include "TGLoading.h"
#include "QuestGameManager.h"


#include "GCUI/GCGameOverlayUI.h"

CTGLoading::CTGLoading()
{
    m_bOnOff            = 0;
    m_iTexID            = 0;
    enType              = TG_LOADING;
}

CTGLoading::~CTGLoading()
{

}

//************************************************************************
// 로딩 배경 파일 저장
//************************************************************************
void CTGLoading::SetLoadingID(int iTexID)
{
    m_iTexID = iTexID;
}

//************************************************************************
// 로딩 OnOff 여부
//************************************************************************
void CTGLoading::SetLoading(bool bOnOff)
{
    m_bOnOff = bOnOff;
}

//************************************************************************
// 로딩 처리
//************************************************************************
void CTGLoading::LoadingStage(void)
{
    SetComplete( true );
    g_pkQuestManager->m_bLoading = m_bOnOff;
    if( m_bOnOff )
        g_pkGameOverlayUI->ShowQuestTitle( m_iTexID, g_pkQuestManager->m_iCurStage + 1 , m_bOnOff );
    //else
    //    g_pkGameOverlayUI->HideAllQuestTitle();

    g_pkGameOverlayUI->EndMinimap();
    KGCPC_MEMO( "Stage Loading" );
}

//************************************************************************
// 로딩 진행 처리
//************************************************************************
void CTGLoading::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        LoadingStage();

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
        LoadingStage();
    }
}

HRESULT CTGLoading::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("LoadingID", m_iTexID, return E_FAIL);

    return S_OK;
}