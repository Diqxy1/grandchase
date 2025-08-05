#include "StdAfx.h"
#include ".\TGObjDestroy.h"
#include "MyD3D.h"

#include "GCIronDoor.h"



CTGObjDestroy::CTGObjDestroy(void)
{
    m_iObjIndex     = 0;
    m_iObjType      = 0;
    enType          = TG_OBJ_DESTROY;
}

CTGObjDestroy::~CTGObjDestroy(void)
{
}

//************************************************************************
// 오브젝트 정보를 설정합니다.
//************************************************************************
void CTGObjDestroy::SetObject( int iObjType, int iObjIndex )
{
    m_iObjType = iObjType;
    m_iObjIndex = iObjIndex;
}

//************************************************************************
// 몬스터 정보 처리
//************************************************************************
void CTGObjDestroy::FrameMove(void)
{
    if( IsComplete() == true ) return;
    if( m_iObjIndex >= MAX_OBJECT || m_iObjIndex < 0 ) return;

    KGCGameObject *pObject = g_MyD3D->m_pObject[m_iObjIndex];

    
	if( !pObject ) return;

	if( GC_PORTAL != pObject->GetObjType() ) return;     // 해당 오브젝트 문인지 확인

    
    KGCDoor *pIronDoor = (KGCDoor*)pObject;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( KGCDoor::IDS_OPEN == pIronDoor->GetDoorState() )
        {
            SetComplete( true );

            if( !m_bSendPacket )
            {
                // 패킷을 보낸다.
                g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
                g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
                m_bSendPacket = true;
            }
        }
    }
    else if( m_bCheckByHost )
    {
        pIronDoor->SetDoorState(KGCDoor::IDS_OPEN);
        SetComplete( true );
    }
}

HRESULT CTGObjDestroy::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("ObjIndex", m_iObjIndex, return E_FAIL);
    LUA_GET_VALUE("ObjType", m_iObjType, return E_FAIL);
    return S_OK;
}

void CTGObjDestroy::EndTrigger()
{
}
