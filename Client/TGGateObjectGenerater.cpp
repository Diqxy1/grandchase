#include "StdAfx.h"
#include ".\tggateobjectgenerater.h"
//
#include "MyD3D.h"
#include "GCIronDoor.h"
#include "GCPortal.h"
#include "GCTeleporter.h"
#include "./MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"


CTGGateObjectGenerater::CTGGateObjectGenerater(void)
{
    enType              = TG_GATE_OBJECT_GENERATER;

    m_bOnOff            = false;
    m_bRight            = true;
    m_bGenerate         = true;
    m_iDirection        = 0;
    m_iDelayTime        = 0;       // 대략 0.3초
    m_iSlot             = 0;
    m_iType             = 0;
    m_fEntranceX        = 0.0f;
    m_fEntranceY        = 0.0f;
}

CTGGateObjectGenerater::~CTGGateObjectGenerater(void)
{
}

void CTGGateObjectGenerater::DestroyObj(int iSlot)
{
    if( g_MyD3D->m_pObject[iSlot] )
    {
        g_MyD3D->m_pObject[iSlot]->Destroy();
        SAFE_DELETE(g_MyD3D->m_pObject[iSlot]);
    }
}

void CTGGateObjectGenerater::GenerateObj(int iSlot, int iType)
{
    switch(iType)
    {
	case 0:
		{
			KGCPortal *portal = new KGCPortal;

			switch(SiKGCRoomManager()->GetGameMode())
			{
			case GC_GM_QUEST0:
			case GC_GM_QUEST23:
			case GC_GM_QUEST28:
			case GC_GM_QUEST33:
            case GC_GM_QUEST54:
			    portal->SetType(2);
				break;
		    case GC_GM_QUEST1:
				portal->SetType(4);
				break;
			case GC_GM_QUEST2:
            case GC_GM_QUEST56:
				portal->SetType(3);
				break;
			case GC_GM_QUEST3:
				portal->SetType(5);
				break;
			case GC_GM_QUEST4:
            case GC_GM_QUEST43:
				portal->SetType(6);
				break;
			case GC_GM_QUEST5:
				portal->SetType(7);
				break;
			case GC_GM_QUEST6:
				portal->SetType(8);
				break;
			case GC_GM_QUEST7:
				portal->SetType(9);
				break;
			case GC_GM_QUEST8:
				portal->SetType(10);
				break;
			case GC_GM_QUEST9:
				portal->SetType(11);
				break;
			case GC_GM_QUEST10:
            case GC_GM_QUEST39:
				portal->SetType(12);
				break;
			case GC_GM_QUEST11:
				portal->SetType(13);
				break;
			case GC_GM_QUEST12:
            case GC_GM_QUEST62:
            case GC_GM_QUEST63:
            case GC_GM_QUEST65:
            case GC_GM_QUEST66:
            case GC_GM_QUEST67:
            case GC_GM_QUEST68:
            case GC_GM_QUEST69:
            case GC_GM_QUEST70:

				portal->SetType(14);
				break;
			case GC_GM_QUEST13:
				portal->SetType(15);
				break;
			case GC_GM_QUEST14:
            case GC_GM_QUEST26:
			case GC_GM_QUEST29:
            case GC_GM_QUEST47:
				portal->SetType(16);
				break;
			case GC_GM_QUEST15:
				portal->SetType(17);
				break;
			case GC_GM_QUEST16:
				portal->SetType(18);
				break;
			case GC_GM_QUEST17:
				portal->SetType(19);
				break;
			case GC_GM_QUEST18:
				portal->SetType(20);
				break;
			case GC_GM_QUEST19:			
				portal->SetType(21);
				break;
			case GC_GM_QUEST20:
				portal->SetType(24);
				break;
			case GC_GM_QUEST21:
            case GC_GM_QUEST40:
				portal->SetType(26);
				break;
			case GC_GM_QUEST22:
				portal->SetType(6);
				break;
			case GC_GM_QUEST24:
				portal->SetType(22);
				break;
            case GC_GM_QUEST25:
            case GC_GM_QUEST41:
                portal->SetType(27);
                break;
            case GC_GM_QUEST27:
                portal->SetType(28);
                break;
			case GC_GM_QUEST31:
				portal->SetType(29);
				break;
			case GC_GM_QUEST32:
				portal->SetType(30);
				break;
			case GC_GM_QUEST30:
				portal->SetType(31);
				break;
			case GC_GM_QUEST34:
            case GC_GM_QUEST44:
				portal->SetType(32);
				break;
            case GC_GM_QUEST42:
                portal->SetType(33);
                break;
            case GC_GM_QUEST45:
                portal->SetType(35);
                break;
            case GC_GM_QUEST46:
                portal->SetType(36);
                break;
            case GC_GM_QUEST51:
            case GC_GM_QUEST52:
                portal->SetType(38);
                break;
            case GC_GM_QUEST53:
                portal->SetType(39);
                break;
            case GC_GM_QUEST55:
                portal->SetType(40);
                break;
			default:
					assert(!"이 던전은 o미? 이거는 말이 안되");
					portal->SetType(2);
				break;
			}			

			portal->SetCrashCheck(false);
			D3DXVECTOR2 pos = SiKGCFantasticMap()->GetPositionNear( m_fEntranceX*0.0025f, m_fEntranceY*0.0025f );
			portal->SetPosX(pos.x);
			portal->SetPosY(pos.y);			
			portal->Create();
			portal->Enable();
			portal->SetIndex(iSlot);
			g_MyD3D->m_pObject[iSlot] = portal;

			portal->SetMaxHit( g_MyD3D->GetPlayerNum() );
			portal->SetCrashHit( g_MyD3D->GetPlayerNum() );
			portal->SetDamageHit(g_MyD3D->GetPlayerNum()/2 );
			portal->SetDoorState(KGCIronDoor::IDS_NORMAL);
			portal->SetCurHit(0);
			portal->SetGateDelay(0);
			portal->SetDirection(m_iDirection);
			portal->EnableLive(false);
		}		

		break;
    case 1:   
        {
			KGCTeleporter *teleporter = new KGCTeleporter;
			
			D3DXVECTOR2 pos = SiKGCFantasticMap()->GetPositionNear( m_fEntranceX*0.0025f, m_fEntranceY*0.0025f );
			teleporter->SetPosX(pos.x);
			teleporter->SetPosY(pos.y);
			teleporter->Create();
			teleporter->Enable();
			teleporter->SetIndex(iSlot);
			g_MyD3D->m_pObject[iSlot] = teleporter;
			teleporter->SetPair(m_iDirection);			
            teleporter->SetDotaStage( m_iDotaStage );
        }

		
        break;
	default:
		{

			KGCPortal *portal = new KGCPortal;
			portal->SetCrashCheck(false);
			D3DXVECTOR2 pos = SiKGCFantasticMap()->GetPositionNear( m_fEntranceX*0.0025f, m_fEntranceY*0.0025f );
			portal->SetPosX(pos.x);
			portal->SetPosY(pos.y);
			portal->SetType(iType);
			portal->Create();
			portal->Enable();
			portal->SetIndex(iSlot);
			g_MyD3D->m_pObject[iSlot] = portal;

			portal->SetMaxHit( g_MyD3D->GetPlayerNum() );
			portal->SetCrashHit( g_MyD3D->GetPlayerNum() );
			portal->SetDamageHit(g_MyD3D->GetPlayerNum()/2 );
			portal->SetDoorState(KGCIronDoor::IDS_NORMAL);
			portal->SetCurHit(0);
			portal->SetGateDelay(0);
			portal->SetDirection(m_iDirection);
			portal->EnableLive(false);
		}
		break;
    }
}

void CTGGateObjectGenerater::GenObject(void)
{
    if( m_iSlot >= 0 && m_iSlot < MAX_OBJECT )
    {
        DestroyObj(m_iSlot);

        if( m_bGenerate ) GenerateObj(m_iSlot, m_iType);
    }

    SetComplete( true );
}

void CTGGateObjectGenerater::FrameMove(void)
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

HRESULT CTGGateObjectGenerater::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("Right", m_bRight, return E_FAIL);
    LUA_GET_VALUE("Generate", m_bGenerate, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
    LUA_GET_VALUE("Direction", m_iDirection, return E_FAIL);
    LUA_GET_VALUE("DelayTime", m_iDelayTime, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);    
    LUA_GET_VALUE_DEF("DotaStage", m_iDotaStage, -1);

    return S_OK;
}
