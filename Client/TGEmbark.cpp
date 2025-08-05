#include "StdAfx.h"
#include "TGEmbark.h"

#include "MyD3D.h"

CTGEmbark::CTGEmbark(void)
{
    enType              = TG_EMBARK;

    InitData();
}

CTGEmbark::~CTGEmbark(void)
{
}

void CTGEmbark::InitData(void)
{
    m_bBeginTG          = false;

    m_bIsRight          = false;
    m_iEmbarkType       = -1;
    m_iEmbarkSlot       = -1;
    m_iEmbarkationCharSlot = -1;
    m_vPos = D3DXVECTOR2(0.0f, 0.0f);
}

void CTGEmbark::FrameMove(void)
{
    if( IsComplete() == true ) return;
	
    if( false == m_bBeginTG )
    {
        m_bBeginTG = true;
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        LoadEmbark();
        if( !m_bSendPacket )
        {
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        LoadEmbark();
    }
}

void CTGEmbark::LoadEmbark()
{
    bool bEmbakation = false;
    DWORD dwPlayerUID = 0;
    PLAYER *pPlayer = NULL;
    
    if( g_MyD3D->IsPlayerIndex( m_iEmbarkationCharSlot ) )
    {
         pPlayer = g_MyD3D->MyPlayer[m_iEmbarkationCharSlot];

        // 탑승할 캐릭터가 셋팅되어 있는데 캐릭터가 살아있지 않으면 탑승물 소환하지 마라.
        if( false == pPlayer->IsLive() )
        {
            SetComplete( true );
            return;
        }
        
        // 소환과 동시에 탑승시켜주자!!
        bEmbakation = true;
        dwPlayerUID = pPlayer->GetPlayerUID();
        pPlayer->SetIsRight( m_bIsRight );
        pPlayer->vPos.x = (m_vPos.x/400.0f);
        pPlayer->vPos.y = (m_vPos.y/400.0f);
    }
    
    KGC_PID_BROAD_EMBARK_SUMMON_ACK kPacket;
    kPacket.dwPlayerUID = dwPlayerUID;    
    kPacket.iEmbarkType = m_iEmbarkType;
    kPacket.dwEmbarkUID = -1 == m_iEmbarkSlot ? SiKGCEmbarkManager()->GetCountUIDWithAutoCount() : m_iEmbarkSlot;
    kPacket.fPosX = (m_vPos.x/400.0f);
    kPacket.fPosY = (m_vPos.y/400.0f);
    kPacket.bIsRight = m_bIsRight;
    kPacket.bEmbakation = bEmbakation;
    SiKGCEmbarkManager()->SummonEmbark( &kPacket );
    
    if( SiKGCEmbarkManager()->GetCountUID() < kPacket.dwEmbarkUID )
    {
        SiKGCEmbarkManager()->SetCountUID( kPacket.dwEmbarkUID, true );
    }
    
    SetComplete( true );
}

HRESULT CTGEmbark::LoadItemFromLua(KLuaManager &luaMgr)
{
    InitData();
    LUA_GET_VALUE_DEF( "EmbarkType", m_iEmbarkType, -1 );
    LUA_GET_VALUE_DEF( "EmbarkSlot", m_iEmbarkSlot, -1 );
    LUA_GET_VALUE_DEF( "EntranceX", m_vPos.x, 0.0f );
    LUA_GET_VALUE_DEF( "EntranceY", m_vPos.y, 0.0f );
    LUA_GET_VALUE_DEF( "Right", m_bIsRight, 0 );
    LUA_GET_VALUE_DEF( "EmbarkationCharSlot", m_iEmbarkationCharSlot, -1 );

    return S_OK;
}
