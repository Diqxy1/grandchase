#include "StdAfx.h"
#include ".\tgchangeobjstate.h"
#include "GCIronDoor.h"


#include "MyD3D.h"
#include "../gcui/GCGameOverlayUI.h"
#include "monster.h"

//
//
//
//

#define MINIMAP_SHOWTIME	(boost::mpl::multiplies<\
								boost::mpl::int_<GC_FPS_LIMIT>,\
								boost::mpl::int_<4>\
							>::next::value)



CTGChangeObjState::CTGChangeObjState(void)
{
    m_iSlot         = 0;
    m_iType         = 0;
    m_iState        = 0;
    enType          = TG_OBJ_CHANGE_STATE;
	m_iOBJType		= 0;
}

CTGChangeObjState::~CTGChangeObjState(void)
{
}


void CTGChangeObjState::SetObjStateInfo(int iSlot, int iType, int iState)
{
    m_iSlot     = iSlot;
    m_iType     = iType;
    m_iState    = iState;
}

void CTGChangeObjState::ChangeObjState(void)
{
    if( 0 == m_iOBJType  && m_iType == 0)
    {
		if( m_iSlot >= MAX_OBJECT || m_iSlot < 0 ) return;
		KGCGameObject *pObject = g_MyD3D->m_pObject[m_iSlot];
		if( !pObject ) return;

        KGCDoor *pIronDoor = (KGCDoor*)pObject;

        switch(m_iState)
        {
            case 0:             // 파괴불가상태
                pIronDoor->EnableLive(false);
                break;
            case 1:             // 파괴가능상태
                pIronDoor->EnableLive(true);

                // 문이 파괴가능 상태 인경우 미니맵을 잠깐 띄운다.
                if( GC_GM_QUEST51 != SiKGCRoomManager()->GetGameMode() )
                    g_pkGameOverlayUI->SetTimerMinimap( MINIMAP_SHOWTIME );

                // 모든 몹을 다 죽였다는 이야기 임으로 콤보 타이머를 초기화 시킨다.
                g_MyD3D->MyHead->SetComboTimer( true, 2000 );
                g_pkQuestManager->RankPointCalcState(true);
                break;
            case 2:             // 기본 상태
                pIronDoor->SetCurHit(0);
                pIronDoor->SetDoorState(KGCDoor::IDS_NORMAL);
                break;
            case 3:             // 데미지 상태
                pIronDoor->SetCurHit(pIronDoor->GetDamageHit());
                pIronDoor->SetDoorState(KGCDoor::IDS_DAMAGE);
                break;
            case 4:             // 파괴된 상태
                pIronDoor->SetCurHit(pIronDoor->GetCrashHit());
                pIronDoor->SetDoorState(KGCDoor::IDS_OPEN);
                break;
        }    
    }
	else if( 1 == m_iOBJType)
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iSlot);
		if( !pMonster ) return;

		switch(m_iState)
		{
		case 0:             // 파괴불가상태
			pMonster->SetInvincibleNoFrame(true);
			break;
		}    
	}

    SetComplete(true);
}

void CTGChangeObjState::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        ChangeObjState();

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
        ChangeObjState();
    }
}

HRESULT CTGChangeObjState::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
	LUA_GET_VALUE("State", m_iState, return E_FAIL);
	LUA_GET_VALUE_DEF("OBJType", m_iOBJType, 0);

    return S_OK;
}
