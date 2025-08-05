#include "StdAfx.h"
#include "TGCheckWall.h"
#include "./MapSystem/GCWallManager.h"


CTGCheckWall::CTGCheckWall(void)
: m_iWallSlot( 0 )
, m_bLive( false )
{
}

CTGCheckWall::~CTGCheckWall(void)
{
}

void CTGCheckWall::FrameMove( void )
{
	if( IsComplete() == true ) return;

	if( g_kGlobalValue.m_kUserInfo.bHost )
	{
        CheckWallLive( m_iWallSlot, m_bLive );
	}

	if( g_kGlobalValue.m_kUserInfo.bHost )
	{
		if( IsComplete() == true && m_bSendPacket == false )
		{
			// 패킷을 보낸다.
			g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
			g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
			m_bSendPacket = true;
		}
	}
	else if( m_bCheckByHost )
	{
		CheckWallLive( m_iWallSlot, m_bLive );
	}
}

bool CTGCheckWall::CheckWallLive( int iSlot, bool bLive )
{
    KGCWall* pWall = SiKGCWallManager()->GetWallInstance( iSlot );
    if( pWall && bLive )
    {
        SetComplete( true );
        return true;
    }
    else if( NULL == pWall && false == bLive )
    {
        SetComplete( true );
        return true;
    }

	return false;
}

HRESULT CTGCheckWall::LoadItemFromLua( KLuaManager &luaMgr )
{
	LUA_GET_VALUE( "WallSlot", m_iWallSlot, return false );
	LUA_GET_VALUE( "IsLive", m_bLive, return false );

#if !defined(__PATH__)
        if( 0 == m_iWallSlot )
        {
            ::MessageBoxA( NULL, "벽 슬롯 디폴트가 0입니다.\nWallSlot으로 스크립트 검색 후 0이상으로 수정하세요.", "WallScriptLoadFail", MB_OK);
            m_iWallSlot = 1;
        }
#endif

	return S_OK;
}