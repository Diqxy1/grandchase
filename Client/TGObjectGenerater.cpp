#include "StdAfx.h"
#include ".\tgobjectgenerater.h"



#include "GCIronDoor.h"
#include "GCJumpZone.h"
#include "GCO2Manager.h"
#include "MapSystem/GCFantasticMap.h"
#include "KGCJungSu.h"
#include "KGCMonsterBuff.h"

CTGObjectGenerater::CTGObjectGenerater(void)
{
    enType          = TG_OBJECTGENERATER;
}

CTGObjectGenerater::~CTGObjectGenerater(void)
{
}

void CTGObjectGenerater::DestroyObj(int iSlot)
{
    if( g_MyD3D->m_pObject[iSlot] )
    {
        g_MyD3D->m_pObject[iSlot]->Destroy();
        SAFE_DELETE(g_MyD3D->m_pObject[iSlot]);
    }
}

void CTGObjectGenerater::GenerateObj(int iSlot, int iType)
{
    D3DXVECTOR2 pos = SiKGCFantasticMap()->GetPositionNear( m_fEntranceX*0.0025f, m_fEntranceY*0.0025f );

    switch(iType)
    {
    case 0:         // 0번은 OBJ_DOOR_0 입니다.
        {
            KGCDoor *ironDoor = new KGCDoor;
            ironDoor->SetCrashCheck(true);            
            ironDoor->SetPosX(pos.x);
            ironDoor->SetPosY(pos.y);
            ironDoor->Create();
            ironDoor->Enable();
            ironDoor->SetIndex(iSlot);
            g_MyD3D->m_pObject[iSlot] = ironDoor;

            const int iDefaultNumOfAttack = 10;
            const int iPlayerNumPerAttack = 5;
            int iNumOfPlayer = 0;
            for(int i=0;i<MAX_PLAYER_NUM;++i)
            {
                if( (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
                    && (g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID != g_MyD3D->GetMyPlayer()->m_kUserInfo.dwUID)
                    )
                    ++iNumOfPlayer;
            }
            ironDoor->SetMaxHit( iDefaultNumOfAttack + iNumOfPlayer*iPlayerNumPerAttack );
            ironDoor->SetCrashHit( iDefaultNumOfAttack + iNumOfPlayer*iPlayerNumPerAttack );
            ironDoor->SetDamageHit( (iDefaultNumOfAttack + iNumOfPlayer*iPlayerNumPerAttack)/2 );
            ironDoor->SetDoorState(KGCDoor::IDS_NORMAL);
            ironDoor->SetCurHit(0);

            if( m_bCrashAble )
            {
                ironDoor->EnableLive(true);
            }
            else
            {
                ironDoor->EnableLive(false);
            }
        }
        break;
    case 1:
        {
            KGCO2Region *pO2region = new KGCO2Region;
            pO2region->SetCrashCheck(true);
            D3DXVECTOR2 pos = SiKGCFantasticMap()->GetPositionNear( m_fEntranceX*0.0025f, m_fEntranceY*0.0025f );
            pO2region->Init(pos.x, pos.y, 0.2f);
            pO2region->SetWidth(276.0f*0.0025f);
            pO2region->SetHeight(266.0f*0.0025f);
            pO2region->Enable();
            pO2region->SetIndex(iSlot);
            g_MyD3D->m_pObject[iSlot] = pO2region;

            if( m_bCrashAble )
            {
                pO2region->EnableLive(true);
            }
            else
            {
                pO2region->EnableLive(false);
            }
        }
        break;
    case 2:
        {			
            KGCJumpZone *jumpzone = new KGCJumpZone;
            jumpzone->SetIndex(iSlot);
            jumpzone->SetPosX(pos.x);
            jumpzone->SetPosY(pos.y);
            jumpzone->Create();			
            jumpzone->SetJump(m_fProperty);
            g_MyD3D->m_pObject[iSlot]=jumpzone;			
        }
        break;
    case 3:
        {            
            KGCJungSu *pJungSu = new KGCJungSuCirculation;
            pJungSu->SetIndex(iSlot);
            pJungSu->SetPosX(pos.x);
            pJungSu->SetPosY(pos.y);
            pJungSu->SetWidth(103.0f*0.0025f);
            pJungSu->SetHeight(103.0f*0.0025f);
            pJungSu->Create();			
            g_MyD3D->m_pObject[iSlot]=pJungSu;			
        }
        break;
    case 4:
        {            
            KGCJungSu *pJungSu = new KGCJungSuInferno;
            pJungSu->SetIndex(iSlot);
            pJungSu->SetPosX(pos.x);
            pJungSu->SetPosY(pos.y);
            pJungSu->SetWidth(103.0f*0.0025f);
            pJungSu->SetHeight(103.0f*0.0025f);
            pJungSu->Create();			
            g_MyD3D->m_pObject[iSlot]=pJungSu;			
        }
        break;

    case 5:
        {            
            KGCJungSu *pJungSu = new KGCJungSuControl;
            pJungSu->SetIndex(iSlot);
            pJungSu->SetPosX(pos.x);
            pJungSu->SetPosY(pos.y);
            pJungSu->SetWidth(103.0f*0.0025f);
            pJungSu->SetHeight(103.0f*0.0025f);
            pJungSu->Create();			
            g_MyD3D->m_pObject[iSlot]=pJungSu;			
        }
        break;

    case 6:
        {            
            KGCJungSu *pJungSu = new KGCJungSuDestruction;
            pJungSu->SetIndex(iSlot);
            pJungSu->SetPosX(pos.x);
            pJungSu->SetPosY(pos.y);
            pJungSu->SetWidth(103.0f*0.0025f);
            pJungSu->SetHeight(103.0f*0.0025f);
            pJungSu->Create();			
            g_MyD3D->m_pObject[iSlot]=pJungSu;			
        }
        break;

    case 7:
        {            
            KGCJungSu *pJungSu = new KGCJungSuResurrection;
            pJungSu->SetIndex(iSlot);
            pJungSu->SetPosX(pos.x);
            pJungSu->SetPosY(pos.y);
            pJungSu->SetWidth(103.0f*0.0025f);
            pJungSu->SetHeight(103.0f*0.0025f);
            pJungSu->Create();			
            g_MyD3D->m_pObject[iSlot]=pJungSu;			
        }
        break;

    case 8:
        {
            KGCMonsterBuff *pMonBuff = new KGCMonsterDefence;
            pMonBuff->SetIndex(iSlot);
            pMonBuff->SetPosX(pos.x);
            pMonBuff->SetPosY(pos.y);
            pMonBuff->SetWidth(103.0f*0.0025f);
            pMonBuff->SetHeight(103.0f*0.0025f);
            pMonBuff->SetMonsterType( static_cast<int>(m_fProperty) ); // 프로퍼티를 몬스터 타입으로 쓰쟈!
            pMonBuff->Create();
            g_MyD3D->m_pObject[iSlot] = pMonBuff;
        }
        break;
    case 9:
        {
            KGCMonsterBuff *pMonBuff = new KGCDemonCore;
            pMonBuff->SetIndex(iSlot);
            pMonBuff->SetPosX(pos.x);
            pMonBuff->SetPosY(pos.y);
            pMonBuff->SetWidth(103.0f*0.0025f);
            pMonBuff->SetHeight(103.0f*0.0025f);
            pMonBuff->SetMonsterType( static_cast<int>(m_fProperty) ); // 프로퍼티를 몬스터 타입으로 쓰쟈!
            pMonBuff->Create();
            g_MyD3D->m_pObject[iSlot] = pMonBuff;
        }
        break;
    case 10:
        {
            KGCMonsterBuff *pMonBuff = new KGCDotaMonsterSummonGate;
            pMonBuff->SetIndex(iSlot);
            pMonBuff->SetPosX(pos.x);
            pMonBuff->SetPosY(pos.y);
            pMonBuff->SetWidth(150.0f*0.0025f);
            pMonBuff->SetHeight(150.0f*0.0025f);
            pMonBuff->SetTgID( m_iTgID );
            pMonBuff->SetMonsterType( static_cast<int>(m_fProperty) );
            pMonBuff->Create();
            g_MyD3D->m_pObject[iSlot] = pMonBuff;
    }
        break;
    case 11:
        {
            KGCMonsterBuff *pMonBuff = new KGCDotaMonsterPortal;
            pMonBuff->SetIndex(iSlot);
            pMonBuff->SetPosX(m_fEntranceX*0.0025f);
            pMonBuff->SetPosY(m_fEntranceY*0.0025f);
            pMonBuff->SetTgID( m_iTgID );
            pMonBuff->SetMonsterType( static_cast<int>(m_fProperty) );
            pMonBuff->Create();
            g_MyD3D->m_pObject[iSlot] = pMonBuff;
        }
        break;
    case 12:
        {
            KGCMonsterBuff *pMonBuff = new KGCDotaDPointShop;
            pMonBuff->SetIndex(iSlot);
            pMonBuff->SetPosX(m_fEntranceX*0.0025f);
            pMonBuff->SetPosY(m_fEntranceY*0.0025f);
            pMonBuff->SetTgID( m_iTgID );
            pMonBuff->SetMonsterType( static_cast<int>(m_fProperty) );
            pMonBuff->Create();
            g_MyD3D->m_pObject[iSlot] = pMonBuff;
        }
        break;
    }
}

void CTGObjectGenerater::GenObject(void)
{
    if( m_iSlot >= 0 && m_iSlot < MAX_OBJECT )
    {
        DestroyObj(m_iSlot);

        if( m_bGenerate ) GenerateObj(m_iSlot, m_iType);
    }

    SetComplete( true );
}

void CTGObjectGenerater::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        GenObject();

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
        GenObject();
    }
}

HRESULT CTGObjectGenerater::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("TG_ID", m_iTgID, return E_FAIL);
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("Right", m_bRight, return E_FAIL);
    LUA_GET_VALUE("Generate", m_bGenerate, return E_FAIL);
    LUA_GET_VALUE_DEF("CrashAble", m_bCrashAble, false);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);
	LUA_GET_VALUE_DEF("Property", m_fProperty, 0.08f);

    return S_OK;
}
