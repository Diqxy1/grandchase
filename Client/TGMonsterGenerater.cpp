#include "StdAfx.h"
#include "TGMonsterGenerater.h"



#include "monster.h"
#include "GCDropItemManager.h"
#include "gcui/GCGameOverlayUI.h"

CTGMonsterGenerater::CTGMonsterGenerater()
{
    enType          = TG_MON_GEN;
}

CTGMonsterGenerater::~CTGMonsterGenerater()
{

}
/*
//************************************************************************
// 몬스터 정보 설정
//************************************************************************
void CTGMonsterGenerater::SetMonsterList(bool bRight, int iBoss, int iMonIndex, int iMonType, int iMonLevel, float fEntranceX, float fEntranceY)
{
    m_bRight = bRight;
    m_iBoss = iBoss;
    m_iMonIndex = iMonIndex;
    m_iMonType = iMonType;
    m_iMonLevel = iMonLevel;
    m_fEntranceX = fEntranceX;
    m_fEntranceY = fEntranceY;
}*/

//************************************************************************
// 몬스터 생성 처리
//************************************************************************
void CTGMonsterGenerater::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        bool bRelease = false;
        g_kTGPacket->SetExtraValue( -1 );
        if ( m_iPercent * 0.01f < randf() )
        {
            // 0이면 없어져라 몬스터..
            g_kTGPacket->SetExtraValue( 0 );
            bRelease = true;
        }
        GenMonster( bRelease );

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
        GenMonster( m_bRelease );
    }
}

//************************************************************************
// 몬스터 생성
//************************************************************************
void CTGMonsterGenerater::GenMonster( bool bRelease /*= false*/ )
{
	//g_MyD3D->CreateMonster(m_iMonIndex);
	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonIndex);
	if( pMonster == NULL ){
		g_kMonsterManager.CreateMonster(m_iMonIndex);
		pMonster = g_kMonsterManager.GetMonster(m_iMonIndex);
	}

    if ( bRelease )
    {
        pMonster->Release();
    }
    else
    {
        DWORD dwChampionProperty = 0x00000000;

        for( int j = 0; j < (int)g_kGlobalValue.m_vecChampionInfo.size(); ++j )
        {
            if( g_kGlobalValue.m_vecChampionInfo[j].m_nTriggerID == m_iTG_ID )
            {
                dwChampionProperty = g_kGlobalValue.m_vecChampionInfo[j].m_dwProperty;
                break;
            }
        }

		if( m_iMonType == g_kGlobalValue.m_kSpecialMonster.m_nMonsterID ){
			m_iMonType = g_kGlobalValue.m_kSpecialMonster.m_nMonsterID;
			m_iMonLevel = g_kGlobalValue.m_kSpecialMonster.m_nMonLv;
			dwChampionProperty = g_kGlobalValue.m_kSpecialMonster.m_dwProperty;
		//	m_iTG_ID = g_kGlobalValue.m_kSpecialMonster.m_nTriggerID;
		}

        float x = m_fEntranceX*0.0025f / SiKGCFantasticMap()->GetPixelWidth();
        float y = m_fEntranceY*0.0025f;

        D3DXVECTOR2 pos;
        if ( m_bRandom )
            pos = g_MyD3D->MyStg->Get_Random_XY();
        else
            pos = g_MyD3D->MyStg->Check_SafePostionX( SiKGCFantasticMap()->GetPixelWidth() * x, y );

        bool bSuccess;
        pMonster->m_bLive = false;
        bSuccess = pMonster->Wait_Room( m_iMonType, pos.x, pos.y, m_bRight, true, m_iMonLevel, dwChampionProperty, m_iBoss );
        pMonster->SetTriggerID(m_iTG_ID);
        SiKGCDropItemManager()->SetObserverToMonster( m_iTG_ID, m_iMonIndex, m_iMonType );
        pMonster->ToggleRender( true );
        //pMonster->SetTargetingDelayTime( GC_FPS_LIMIT * 6 );     // 5초간 몬스터들이 타게팅을 하지 않는다.
        pMonster->SetTargetingDelayTime( 30 );     // 30Frame간 몬스터들이 타게팅 하지 않는다. 

        //====================================================================================
        // 2009.02.03 : Stone
        // Comment : 몬스터 모션파일이 지워 졌을 시 때의 처리를 위한 코드
        //           Wait_Room함수에서 m_bIsDeletedMonsterMotion의 값이 결정된다.
#ifdef __PATH__
        if( !bSuccess )
        {
           return;
        }
#endif

        if ( !g_pkGameOverlayUI->IsShowDialogue() )
        {
            g_pkGameOverlayUI->UpdateBossList();
        }
    }

    SetComplete( true );
}

HRESULT CTGMonsterGenerater::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Right", m_bRight, return E_FAIL);
    LUA_GET_VALUE("Boss", m_iBoss, return E_FAIL);
    LUA_GET_VALUE("Index", m_iMonIndex, return E_FAIL);
    LUA_GET_VALUE("Type", m_iMonType, return E_FAIL);
    LUA_GET_VALUE_DEF("Level", m_iMonLevel, -1);
    LUA_GET_VALUE_DEF("PROBABILITY", m_iPercent, 100);
    LUA_GET_VALUE_DEF("RANDOM", m_bRandom, false);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);

    return S_OK;
}
