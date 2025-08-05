#include "stdafx.h"
#include "HardDamageBreeding.h"
#include "LatencyTransfer.h"
#include "Monster.h"
#include "Stage.h"

#include "MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"

CHardDamageBreeding::CHardDamageBreeding(void)
{
	m_bCrash = false;
}

CHardDamageBreeding::~CHardDamageBreeding(void)
{
}

void CHardDamageBreeding::AngleFrameMove(CDamageInstance* pDamageInstance)
{
	if( m_bCrash == false )
	{
		CDamage::AngleFrameMove(pDamageInstance);
		float fPosY;
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            D3DXVECTOR2 dxvCrashPos;
            SiKGCFantasticMap()->CrashCheck( pDamageInstance->m_X, pDamageInstance->m_Y, dxvCrashPos );
            fPosY = dxvCrashPos.y;
        }
        else
        {
            fPosY = g_MyD3D->MyStg->Check_Contact( pDamageInstance->m_X, pDamageInstance->m_Y, 0 ).y;
        }

		if( pDamageInstance->m_Angle < 0.0f )
			return;

		if( fPosY != -1.0f )
		{
			m_fCrashPosY = fPosY;
			m_bCrash = true;
		}
	}

	if( m_bCrash == true )
	{
		if( pDamageInstance->m_Life == 1 )
		{
			int iMonsterID = -1, iMonsterLv;
			std::string strAI;
			if ( pDamageInstance->m_What == DT_INSECTIVOROUS_BREEDING)
			{
				pDamageInstance->CreateSequence( "emerge_1", pDamageInstance->m_X-1.0f, m_fCrashPosY-0.5f, 0.5f );
				pDamageInstance->CreateSequence( "emerge_2", pDamageInstance->m_X-1.0f, m_fCrashPosY-0.5f, 0.5f );
				//iMonsterID = MON_MINI_INSECTIVOROUS;
				strAI = "AIMiniInsectivorous";
				iMonsterLv = 20;
			}
			else
			{
				pDamageInstance->CreateSequence( "Little_Stone_smog01", pDamageInstance->m_X-1.0f, m_fCrashPosY-0.5f, 0.5f );
				pDamageInstance->CreateSequence( "Stonetick01", pDamageInstance->m_X-1.0f, m_fCrashPosY-0.5f, 0.5f );
				//iMonsterID = MON_MINI_STONE_GOLEM;
				strAI = "AIMiniStoneGolem";

				MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);

				if ( pMonster == NULL ) return;

				iMonsterLv = pMonster->m_iLevel;
			}
			

			// 방장이라면 몬스터 생성 패킷을 날린다.
			if( g_kGlobalValue.m_kUserInfo.bHost == true )
			{
				int iIndex = -1;
				//for( int i=5 ; i<MAX_MONSTER ; ++i )
				//{
				//	if( g_kMonsterManager.m_mapMonster[i]->EnableCreate() == true && !g_kMonsterManager.m_mapMonster[i]->m_bBirthReserve )
				//	{
				//		iIndex = i;
				//		g_kMonsterManager.m_mapMonster[i]->m_bBirthReserve = true;						
				//		break;
				//	}
				//}

// 				if( iIndex != -1 )
// 				{
// 					LTP_MONSTER_GENERATE Packet;
// 					ZeroMemory( &Packet, sizeof(LTP_MONSTER_GENERATE) );
// 					Packet.packetType   = ELTP_MONSTER_GENERATE;
// 					Packet.size         = sizeof(LTP_MONSTER_GENERATE);
// 					Packet.latencyTime  = GC_ELAPSED_TIME * 10.0f;
// 
// 					Packet.iMonsterIndex	= iIndex;
// 					Packet.x				= pDamageInstance->m_X;
// 					Packet.y				= pDamageInstance->m_Y;
// 					
// 					Packet.iMonsterLv		= iMonsterLv;
// 					Packet.bAIEnable		= true;
// 					Packet.iNewMonsterID	= iMonsterID;
// 					strcpy( Packet.strAItemplate, strAI.c_str() );
// 					g_LatencyTransfer.PushPacket( &Packet );
// 				}
			}
		}
	}
}
