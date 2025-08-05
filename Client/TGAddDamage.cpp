#include "StdAfx.h"
#include ".\tgadddamage.h"

#include "DamageManager.h"

#include "QuestGameManager.h"

CTGAddDamage::CTGAddDamage(void)
{
    enType              = TG_ADDDAMAGE;

    m_iDamageID         = 0;
    m_iEntranceX        = 0;
    m_iEntranceY        = 0;
    m_dwRecycleTime     = 6000;

    // Options - Use Only WaterPole.
    m_iLife             = 500;
    m_iStartTime		= 0;
    m_iContinueance     = 3000;

    m_dwTermTime        = 0;
	m_iDir				= 0;
    m_dwFirstTime       = 0;

	m_bIsStart=false;
}

CTGAddDamage::~CTGAddDamage(void)
{
}

void CTGAddDamage::AddDamage(void)
{
    if( m_dwTermTime < timeGetTime() )
    {
        StartDamage();

        KGC_PID_DAMAGE_PACKET packet;
        packet.m_sTG_ID             = m_iTG_ID;
        g_kTGPacketPacker.Pack(&packet, sizeof(KGC_PID_DAMAGE_PACKET));
        //SendNetworkData(GC_PID_DAMAGE_PACKET, 0);     // 트리거 정보 날릴때 보내기 때문에 여기서 궂이 않해줘도 됨.
    }

    if( 0 == m_dwRecycleTime )
        SetComplete( true );
}

void CTGAddDamage::StartDamage(void)
{
	if(true == IsDetermineWin)
		return;

    switch( m_iDamageID )
    {
		case DT_WATER_POLE:
		case DT_FIRE_POLE: // 불기둥 번호가 바뀌어서 일단 노가다 코딩 나중에 꼭 고치겠음 ㅠ_ㅠ <== 거짓말쟁이
            {
                g_pkQuestManager->StartWaterPole(*this);
            }
            break;
        default:
            g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x = m_iEntranceX*0.0025f;
            g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y = m_iEntranceY*0.0025f;
 			g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight = (0 != m_iDir);
			g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->m_kUserInfo.iTeam = ETRed;
            g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->m_fAbility[ABILITY_ATK] = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_ATK];
            g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->m_fAbility[ABILITY_ATTACK_PERCENT] = 0.0f;

			CDamageInstance* pDamage = g_MyD3D->m_pDamageManager->AddWithLocate(m_iDamageID, MAX_PLAYER_NUM, g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x, g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y);
            int iContinueance = 0;
            if( pDamage )
            {
				pDamage->m_IsRight = (0 != m_iDir);
                iContinueance = (int)( ((float)pDamage->m_pOriginalDamage->m_Start_Life) / GC_FPS_LIMIT ) * 1000;       // 프레임 단위를 ms단위로...
            }
            break;
    }

	m_dwTermTime = timeGetTime() + m_dwRecycleTime + m_iContinueance;
}

void CTGAddDamage::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if(!m_bIsStart)
        {
            m_dwFirstTime = timeGetTime();
            m_bIsStart=true;
        }

        if( timeGetTime() < (m_dwFirstTime + m_iStartTime) )
            return;

        AddDamage();

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
        // AddDamage();     // 방원에게는 TCP/IP 패킷으로 던질 것 이다.
        if( 0 == m_dwRecycleTime )
            SetComplete( true );
    }
}

HRESULT CTGAddDamage::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("DamageID", m_iDamageID, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_iEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_iEntranceY, return E_FAIL);
    LUA_GET_VALUE_DEF("RecycleTime", m_dwRecycleTime, 6000);
    LUA_GET_VALUE_DEF("Life", m_iLife, 500);
    LUA_GET_VALUE_DEF("StartTime", m_iStartTime, 0);    
    LUA_GET_VALUE_DEF("Continueance", m_iContinueance, 3000);	
	LUA_GET_VALUE_DEF("Dir", m_iDir,0);	

    if( 0 != m_iStartTime  )
	    m_iStartTime += m_dwRecycleTime;

    return S_OK;
}
