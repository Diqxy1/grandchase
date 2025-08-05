#include "StdAfx.h"
#include "TGWall.h"
#include "MyD3D.h"


CTGWall::CTGWall(void)
{
    enType              = TG_WALL;
    InitData();
}

CTGWall::~CTGWall(void)
{
}

void CTGWall::InitData(void)
{
    m_bBeginTG          = false;
    m_bDestroy          = false;
    m_bIsCheckInBound   = true;
    m_iWallType         = 0;
    m_iWallSlot         = 0;
    m_fPosX             = 0.0f;
}

void CTGWall::FrameMove(void)
{
    if( IsComplete() == true ) return;
	
    if( false == m_bBeginTG )
    {
        m_bBeginTG = true;
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        LoadWall();
        if( !m_bSendPacket )
        {
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        LoadWall();
    }
}

void CTGWall::LoadWall()
{
    KGCWall* pWall = NULL;
    if( m_bDestroy )
        SiKGCWallManager()->ClearInstance( m_iWallSlot );
    else
        pWall = SiKGCWallManager()->CreateWall( m_iWallType, m_iWallSlot, (m_fPosX/400.0f) );

    if( pWall )
        pWall->m_bCheckInBound = m_bIsCheckInBound;

    SetComplete( true );
}

HRESULT CTGWall::LoadItemFromLua(KLuaManager &luaMgr)
{
    InitData();
    LUA_GET_VALUE_DEF( "WallType", m_iWallType, 0 );
    LUA_GET_VALUE_DEF( "WallSlot", m_iWallSlot, 0 );
    LUA_GET_VALUE_DEF( "EntranceX", m_fPosX, 0.0f );
    LUA_GET_VALUE_DEF( "IsDestroy", m_bDestroy, false );
    LUA_GET_VALUE_DEF( "IsCheckInBound", m_bIsCheckInBound, true );

#if !defined(__PATH__)
    if( 0 == m_iWallSlot )
    {
        ::MessageBoxA( NULL, "벽 슬롯 디폴트가 0입니다.\nWallSlot으로 스크립트 검색 후 0이상으로 수정하세요.", "WallScriptLoadFail", MB_OK);
        m_iWallSlot = 1;
    }
#endif

    return S_OK;
}