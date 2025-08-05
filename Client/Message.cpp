#include "stdafx.h"
//
//
#include "KncP2PLib/KncP2P.h"
////
//


#include "GCUIHeader.h"
//
#include "LatencyTransfer.h"
#include "QuestGameManager.h"




#include "Message.h"
#include "Procedure.h"
#include "KSingleton.h"
#include "Headup Display.h"
#include "Item.h"

#include "Stage.h"

#include "Buff/KGCBuffManager.h"
#include "zlib.h"
#include "GCStateGame.h"
#include "KGCGameBoard.h"



#include "ClientErr.h"
#include "GCStateLoading.h"
#include "Replay.h"



#include "DamageManager.h"
//
#include "EveryTriggerManager.h"

#include "GCDropItemManager.h"
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "GCIronDoor.h"

#include "TGDialogue.h"
#include "KGCAngelsEgg.h"

#include "./Square/GCSquare.h"

#include "./Square/GCSquarePeopleActivitySensor.h"



#include "KGCDeathMatch.h"
#include "KGCMatch.h"

#include "GCRenderManager.h"
#include "GCCoupleSystem.h"

#include "TGMonsterGenerater.h"
#include "TGNextStage.h"
#include "KGCSyncObject.h"
#include "GCClientErrorManager.h"
#include "KGCEmbarkManager.h"
#include "KGCNetworkStatManager.h"

bool g_bGameModeChange = false;
#ifdef _PACKET_PROFILE_
    KPACKET_TEST g_akRecv[GC_PID_END] = {0,};
    KPACKET_TEST g_akSend[GC_PID_END] = {0,};
#endif


KncCriticalSection m_csPingLock;
std::map<DWORD,DWORD> g_mapSendedPing;
int		iPingInterval = 3;
DWORD	g_dwPingLimit = 2000;
int		g_iPingOutCountLimit = 1;
int     g_iPingOutCount = 0;
KncStopWatch m_kStopWatch;
FILE*   pTcpLogFile = NULL;
bool g_bCheckGuardianWeapon = false;
bool g_bCheckTuxedoWeapon = false;

#if defined( USE_ONLY_TCP_RELAY )
EP2PCONNECTION	g_eP2PConMode = P2P_TCP_RELAY_ONLY;
#else
EP2PCONNECTION	g_eP2PConMode = P2P_CON_DEFAULT;
#endif

void	AddPing( DWORD dwUID_, DWORD dwTimeStamp_ );
bool	RemovePing( DWORD dwUID_ );

void SetP2PMode( EP2PCONNECTION eCon_ )
{

}
int GetRelayPingInterval()
{
    return iPingInterval;
}

DWORD GetRelayPingLimit()
{
    return g_dwPingLimit;
}

void SetRelayPingInterval( int iMilliSecond_ )
{
	//iPingInterval = static_cast<int>( iMilliSecond_ / 1000 );

	//if( iPingInterval <= 0 ) {
	//	iPingInterval = 1;
	//}

    iPingInterval = iMilliSecond_;
    m_kStopWatch.SetStopWatch(iPingInterval);
}

void SetRelayPingLimit( DWORD dwLimit_ )
{
	g_dwPingLimit = dwLimit_;
}

void SetRelayPingOutCount( int iCount_ )
{
	g_iPingOutCountLimit = iCount_;
	if( g_iPingOutCountLimit <= 0 ) {
		g_iPingOutCountLimit = 1;
	}
}

bool SetAutoBlueTeam()
{
    int Num_Blue=0;
    int Num_Red=0;
    for(int i=0;i<6;i++)
    {
        if(g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
        {
            if(g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam())
                Num_Blue++;
            else Num_Red++;
        }
    }
    if(Num_Blue>Num_Red) return false;
    else return true;
}

bool WINAPI KncP2PMessageHandler( DWORD dwMessageID, PVOID pMsgBuffer )
{
#if defined( BUG_TRAP )
	BT_SetTerminate();
#endif

    switch ( dwMessageID )
    {
        case KNC_MSGID_CONNECT_COMPLETE:
        {
			((KGCStateLoading*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME_LOADING ))->m_bConnectComplete = true;
            break;
        }
        case KNC_MSGID_RECEIVE:
        {
            KNCMSG_RECEIVE* pReceiveMsg = (KNCMSG_RECEIVE*)pMsgBuffer;
            KGC_PACKET_HEADER* pPacketHeader = (KGC_PACKET_HEADER*)pReceiveMsg->pReceiveData;
			KGCPC_VALUE( "P2P Rcv", pReceiveMsg->dwReceiveDataSize );
            P2PPacketProc( pPacketHeader->Type, pReceiveMsg->pReceiveData, pReceiveMsg->dwUID, pReceiveMsg->dwReceiveDataSize );
            return S_OK;
            break;
        }
    }
    return true;
}

void Set_HighRanker( void )
{
    // 전체 능력치를 현재 플레이어들의 능력치 중 가장 높은 것으로 사용한다.
    for(int i = 0 ; i < MAX_PLAYER_NUM ; i++)
    {
        //초고수 플레이어나 운영자임...
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && 
            g_MyD3D->MyPlayer[i]->IsHighRanker() )
        {
            float fAtkPoint=.1f;
            float fDefPoint=.1f;
            float fHPPoint=.1f;
            for(int j = 0 ; j < MAX_PLAYER_NUM ; j++)
            {
                if( i != j && g_MyD3D->MyPlayer[j]->m_kUserInfo.bLive )
                {
                    if( fAtkPoint < g_MyD3D->MyPlayer[j]->m_fAbility[ABILITY_ATK] ) 
                        fAtkPoint = g_MyD3D->MyPlayer[j]->m_fAbility[ABILITY_ATK];
                    if( fDefPoint < g_MyD3D->MyPlayer[j]->m_fAbility[ABILITY_DEF] ) 
                        fDefPoint=g_MyD3D->MyPlayer[j]->m_fAbility[ABILITY_DEF];
                    if( fHPPoint < g_MyD3D->MyPlayer[j]->m_fAbility[ABILITY_HP] ) 
                        fHPPoint=g_MyD3D->MyPlayer[j]->m_fAbility[ABILITY_HP];
                }
            }
            g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATK] = fAtkPoint * 1.1f;
            g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_DEF] = fDefPoint * 1.1f;
            g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP] = fHPPoint * 1.1f;
        }
    }
}

bool Is_Must_Reach_Basic_Packet()
{
    if ( g_kTransNPC->TransNPC.Action == -1 )
    {
        // Item을 사용 했으면 반드시 보내야 하는 Packet을 보내야 한다.
        for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if(g_kEffectItem->Eff_Item[i].cWhat > -1)
            {
                return true;
            }
        }
        //다 죽었고, 나는 더이상 데이터를 보내지 않겠다라고 선언 하지 않았으면
        //Reliable한 Packet으로 다 죽었다는 것을 알리고 더이상 Packet을 보내지 않는다.
        if(!g_kFrameInfoPacket->Life && !g_MyD3D->m_bLastFrameSend)
        {
            g_MyD3D->m_bLastFrameSend=true;
            return true;
        }
        return false;
    }
    return true;
}

HRESULT SendNetworkData( char msg_Type, int Option, const void* Param_Text )
{
    switch ( msg_Type )
    {
        case GC_PID_PLAYERLATENCY_FRAME_INFO:    // 각 플레이어는 자신의 LATENCY 만큼의 프레임 동작을 보내준다.
        {    
			//if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && g_kGlobalValue.m_kUserInfo.bHost )
			//	SendDungeonFrameLatencyInfo();
			//else
			//	SendPlayerFrameLatencyInfo();
			//break;
			if( (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ||
                SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
                && g_kGlobalValue.m_kUserInfo.bHost )
			{
                // 기존 Send코드 : TG팩커에 트리거 정보 + 프레임정보를 쌓아서 보내고 있음..
                //KGC_PID_TCP_PACKED_PACKET kPacket;
                //SendFrameLatencyInfo( &g_kTGPacketPacker, &kPacket );

                // 트리거 패킷은 무조건 TCP로 보내도록 수정합니다.( 패킷 도착순서꼬이면서 오동작되는 부분이 있음 )
                UINT uiPacketSize = 0;
                KGC_PID_TCP_PACKED_PACKET* pkPacket = g_kTGPacketPacker.GetData( uiPacketSize );
                if ( pkPacket->ucPacketCount > 0 ) 
                {
                SendServerRelayPacket(pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME );
                }

                // 트리거 패킷은 TCP로 보냈으니깐, 
                // 프레임정보는 g_kPacketPacker여기에 쌓아서 보내도록 하겠다.
                KGC_PID_PACKED_PACKET kPacket;
                SendFrameLatencyInfo( &g_kPacketPacker, &kPacket );
			}
            else if(SiKGCRoomManager()->GetGameMode() == GC_GM_AGIT){
                KGC_PID_PACKED_PACKET kPacket;
                SendAgitFrameLatencyInfo( &g_kPacketPacker, &kPacket );
            }
			else
			{
				KGC_PID_PACKED_PACKET kPacket;
				SendFrameLatencyInfo( &g_kPacketPacker, &kPacket );
            }
			KGCPC_MEMO( "GC_PID_PLAYERLATENCY_FRAME_INFO" );
            break;
        }
        case GC_PID_TG_DEFAULT:       // 트리거 패킷을 TCP 릴레이 서버로 보내는 역할을 한다.
            if( g_kTGPacketPacker.GetPackCount()
                && (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory())
                && g_kGlobalValue.m_kUserInfo.bHost
                )
            {
                // BroadCast
                UINT uiPacketSize = 0;
                KGC_PID_TCP_PACKED_PACKET* pkPacket = g_kTGPacketPacker.GetData( uiPacketSize );
				SendServerRelayPacket(pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME );
				KGCPC_MEMO( "GC_PID_TG_DEFAULT(" << uiPacketSize << ")" );
            }
            break;
        case GC_PID_SHOOTING_ARROW_TOURNAMENT: // 궁수 활쏘기 대회에서 각도와 발사 시점을 보내줌
        {
            ShotPacket->Type=GC_PID_SHOOTING_ARROW_TOURNAMENT;
            ShotPacket->TimeOut=false;
            if(Option==1){
                ShotPacket->RequestStartMessage=true;
                ShotPacket->TimeOut=false;
                ShotPacket->Shot=false;
            }else if(Option==2){
                ShotPacket->RequestStartMessage=false;
                ShotPacket->Shot=false;
                ShotPacket->TimeOut=true;
            }else ShotPacket->RequestStartMessage=false;
            
            if(g_kGlobalValue.m_kUserInfo.bHost && ShotPacket->Shot)
            {              
                g_MyD3D->Story=200+730*g_MyD3D->Get_MyPlayer()+625;
            }

            if(ShotPacket->Shot || ShotPacket->TimeOut)
            {
                SendP2PPacket( ShotPacket, sizeof( KGC_PID_SHOOTING_ARROW_TOURNAMEN ), KNC_BROADCAST_NOT_ME, _RELIABLE );
            }
            else
            {
                SendP2PPacket( ShotPacket, sizeof( KGC_PID_SHOOTING_ARROW_TOURNAMEN ), KNC_BROADCAST_NOT_ME, _UNRELIABLE );
            }
            break;
        }
        case GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT: // 궁수 활쏘기 대회에서 전체 점수와 높이 난이도 등을 보내줌.
        {
            ShotScorePacket->Type=GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT;
            ShotScorePacket->Story=g_MyD3D->Story;
            ShotScorePacket->Height=g_MyD3D->ArrowHeight;
            ShotScorePacket->Level=g_MyD3D->DistanceLevel;
            for(int i=0;i<MAX_PLAYER_NUM;i++)
            {
                for(int ii=0;ii<3;ii++)
                {
                    ShotScorePacket->Score[i][ii]=g_aiArrowScore[i][ii];
                }
            }
            SendP2PPacket( ShotScorePacket, sizeof( KGC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT ), KNC_BROADCAST_NOT_ME, _RELIABLE );
            break;
        }
        case GC_PID_ACTION_LEAGUE_RESULT: // 액션 리그 팀점수를 알린다.
        {
            if( SiKGCRoomManager()->GetUserCount() < 2 )
                break;
            // if ( Num_Net < 2 )
            //    break;
            KGC_PID_ACTION_LEAGUE_RESULT Point[1];
            Point->Type=GC_PID_ACTION_LEAGUE_RESULT;
            Point->Team_Point=Option;
            SendP2PPacket( Point, sizeof( KGC_PID_ACTION_LEAGUE_RESULT ), KNC_BROADCAST, _RELIABLE );
			KGCPC_MEMO( "GC_PID_ACTION_LEAGUE_RESULT" );
            break;
        }
        case GC_PID_LATENCY_TRANSFER: // 레이턴시 트랜스퍼
        {
            DWORD dwSendType = 0;
            switch( Option )
            {
            case 0:     dwSendType = KNC_BROADCAST_NOT_ME;         break;
            case 1:     dwSendType = KNC_BROADCAST;                break;
            case 2:     dwSendType = KNC_RELAY_BROADCAST_NOT_ME;   break;
            case 3:     dwSendType = KNC_RELAY_TO_ME;              break;
            case 4:     dwSendType = KNC_TCP_RELAY_TO_ME;          break;
            default:    dwSendType = KNC_BROADCAST_NOT_ME;         break;
            }

            // 릴레이서버에 접속되어 있지 않다면 무쪼건 NOT_ME로 쏜다.
            if ( !SiKP2P()->m_spTRUserProxy->IsConnected() ) 
            {
                dwSendType = KNC_BROADCAST_NOT_ME;
            }

            //  한명만 있다는건 자기만 보낸다는 얘기 이니깐 NotMe로 보낸다.
            if ( dwSendType == KNC_BROADCAST && GetRelayPacketUser() == 1 )
            {
                dwSendType = KNC_BROADCAST_NOT_ME;
            }

            if ( dwSendType == KNC_BROADCAST_NOT_ME ) 
            {
                g_LatencyTransfer.PacketHandler((LTP_BASIC*)Param_Text);
            }

            SendServerRelayPacket(const_cast<void*>( Param_Text ), reinterpret_cast<const LTP_BASIC*>( Param_Text )->size, dwSendType );
            break;
        }
        case GC_PID_SUMMON_MONSTER:
        {
            if( g_kGlobalValue.m_kUserInfo.bHost && g_kSummonMonster->m_usMonIndex != -1 && g_kSummonMonster->m_usMonType != -1 )
            {
                SendP2PPacket( g_kSummonMonster, sizeof( KGC_PID_SUMMON_MONSTER ), Option, _RELIABLE );
                g_kSummonMonster->Init();
            }
            KGCPC_MEMO( "GC_PID_SUMMON_MONSTER" );
            break;
        }
        case GC_PID_QUEST_REAULT_ITEM_DICE:
            {
                // BroadCast
                UINT uiPacketSize;
                KGC_PID_TCP_PACKED_PACKET* pkPacket = g_kTGPacketPacker.GetData( uiPacketSize );
                SendServerRelayPacket(pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME );
                if( 0 == g_MyD3D->m_pkQuestGameMgr->m_vecRouletteWaitList.size() )
                    g_MyD3D->m_pkQuestGameMgr->m_bRouletteList = true;
                KGCPC_MEMO( "GC_PID_QUEST_REAULT_ITEM_DICE" );
                break;
            }
        case GC_PID_QUEST_STAGE_RESULT_SCORE:
            {
                // BroadCast
                SQuestPoint temp;
				
				const int iMyPlayerIndex = g_MyD3D->Get_MyPlayer();
				const int iCurStage = g_MyD3D->m_pkQuestGameMgr->m_iCurStage;
				if( g_MyD3D->IsPlayerIndex( iMyPlayerIndex ) )
				{
					if( g_MyD3D->m_pkQuestGameMgr->m_alQuestPoint[iMyPlayerIndex].count( iCurStage ) )
						temp = g_MyD3D->m_pkQuestGameMgr->m_alQuestPoint[iMyPlayerIndex][iCurStage];
				}
				
                g_kSendQuestStageResultScore->usAerialAttack       = temp.AerialAttack;
                g_kSendQuestStageResultScore->usAttack             = temp.Attack;
                g_kSendQuestStageResultScore->usAttacked           = temp.Attacked;
                g_kSendQuestStageResultScore->usBackAttack         = temp.BackAttack;
                g_kSendQuestStageResultScore->usCounterAttack      = temp.CounterAttack;
                g_kSendQuestStageResultScore->usCriticalAttack     = temp.CriticalAttack;
                g_kSendQuestStageResultScore->usDoubleAttack       = temp.DoubleAttack;
                g_kSendQuestStageResultScore->usSpecial1           = temp.iSpecial1;
                g_kSendQuestStageResultScore->usSpecial2           = temp.iSpecial2;
                g_kSendQuestStageResultScore->usSpecial3           = temp.iSpecial3;
                g_kSendQuestStageResultScore->ucStage              = iCurStage;
                g_kSendQuestStageResultScore->ucPlayerIndex        = iMyPlayerIndex;
                g_kTGPacketPacker.Pack(g_kSendQuestStageResultScore, sizeof(KGC_PID_QUEST_STAGE_RESULT_SCORE));

                UINT uiPacketSize = 0;
                KGC_PID_TCP_PACKED_PACKET* pkPacket = g_kTGPacketPacker.GetData( uiPacketSize );
                SendServerRelayPacket(pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME );

                if( 0 == g_MyD3D->m_pkQuestGameMgr->m_vecEndGameInfo.size() )
                    g_MyD3D->m_pkQuestGameMgr->m_bGetEndGameInfo = true;
                KGCPC_MEMO( "GC_PID_QUEST_STAGE_RESULT_SCORE" );
                break;
            }
        case GC_PID_QUEST_STAGE_POINT:
            {
				if( false == g_MyD3D->IsPlayerIndex( g_MyD3D->Get_MyPlayer() ) )
					break;

                // Stage == Option
                if( (int)g_MyD3D->m_pkQuestGameMgr->m_alQuestPoint[g_MyD3D->Get_MyPlayer()].count( Option ) <= 0  )
                    break;

                SQuestPoint temp = g_MyD3D->m_pkQuestGameMgr->m_alQuestPoint[g_MyD3D->Get_MyPlayer()][Option];
                g_kQuestStageScore->usAerialAttack       = temp.AerialAttack;
                g_kQuestStageScore->usAttack             = temp.Attack;
                g_kQuestStageScore->usAttacked           = temp.Attacked;
                g_kQuestStageScore->usBackAttack         = temp.BackAttack;
                g_kQuestStageScore->usCounterAttack      = temp.CounterAttack;
                g_kQuestStageScore->usCriticalAttack     = temp.CriticalAttack;
                g_kQuestStageScore->usDoubleAttack       = temp.DoubleAttack;
                g_kQuestStageScore->usSpecial1           = temp.iSpecial1;
                g_kQuestStageScore->usSpecial2           = temp.iSpecial2;
                g_kQuestStageScore->usSpecial3           = temp.iSpecial3;
                g_kQuestStageScore->ucStage              = g_MyD3D->m_pkQuestGameMgr->m_iCurStage;
                g_kQuestStageScore->ucPlayerIndex        = g_MyD3D->Get_MyPlayer();

                g_kTGPacketPacker.Pack(g_kQuestStageScore, sizeof(KGC_PID_QUEST_STAGE_POINT));

                UINT uiPacketSize = 0;
                KGC_PID_TCP_PACKED_PACKET* pkPacket = g_kTGPacketPacker.GetData( uiPacketSize );
                SendServerRelayPacket(pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME );

                KGCPC_MEMO( "GC_PID_QUEST_STAGE_POINT" );
                break;
            }
        case GC_PID_UDP_RELAYSERVER_HEART_BEAT:
            {
                // UDP 릴레이 서버를 안쓰므로 일단 이 부분도 안씁니다.
                //// 방원이 UID 릴레이 서버에서 제명되었을 경우 날려주는 패킷
                //KGC_PID_UDP_RELAYSERVER_HEART_BEAT packet;
                //SendP2PPacket( &packet, sizeof( KGC_PID_UDP_RELAYSERVER_HEART_BEAT ), KNC_RELAY_BROADCAST_NOT_ME, _RELIABLE );
                break;
            }
        case GC_PID_SQUARE_ACTION:
            {
                if( g_kSquarePacketPacker.GetPackCount() > 0 )
                {
                    UINT uiPacketSize = 0;
                    KGC_PID_TCP_PACKED_PACKET* pkPacket = g_kSquarePacketPacker.GetData( uiPacketSize );
                    std::vector< DWORD >& vecUID = SiGCSquare()->GetNearPeopleUIDVector();
                    std::vector< DWORD >::iterator vit = std::find( vecUID.begin(), vecUID.end(), g_kGlobalValue.m_kUserInfo.dwUID );
                    if( vit != vecUID.end() )
                    {
                        vecUID.erase( vit );

                        if( vecUID.empty() )
                            return S_OK;
                    }
                    SendServerRelayPacketInSquare( pkPacket, uiPacketSize, vecUID );
                }
                break;
            }

		case GC_PID_ANGELS_EGG_INFO:
			{
				SendP2PPacket( g_kAngelsEggInfo, sizeof( KGC_PID_ANGELS_EGG_INFO ), KNC_BROADCAST_NOT_ME, _RELIABLE );
				break;
			}
		case GC_PID_COUPLE_EVENT_CREDIT:
			{
				break;
			}
		case GC_PID_MAGIC_MISSILE:
			{
				SendP2PPacket( g_KMagicMissile, sizeof( KGC_PID_MAGIC_MISSILE ), KNC_BROADCAST, _RELIABLE );
				KGCPC_MEMO( "GC_PID_MAGIC_MISSILE" );
			}
			break;
        case GC_PID_CHANGE_MONSTER:
            {
                SendP2PPacket( g_kChangeMonster, sizeof( KGC_PID_CHANGE_MONSTER ), KNC_BROADCAST, _RELIABLE );
                KGCPC_MEMO( "GC_PID_CHANGE_MONSTER" );
            }
            break;
        case GC_PID_BROAD_CONTROL_MISSION_DATA:
            {
                const int iMyPlayerIndex = g_MyD3D->Get_MyPlayer();

                int iControlInfoComboCount = 0;             
                int iControlInfoBackAttackCount = 0;        
                int iControlInfoBeAttackedCount = 0;        
                std::map<int, int> mapControlInfoSkill; 
                std::map<int, int> mapControlInfoPotion;

                mapControlInfoSkill.clear();
                mapControlInfoPotion.clear();

                iControlInfoBeAttackedCount = g_MyD3D->MyPlayer[iMyPlayerIndex]->GetControlInfoBeAttackedCount();
                iControlInfoComboCount = g_MyD3D->MyPlayer[iMyPlayerIndex]->GetControlInfoComboCount();
                iControlInfoBackAttackCount = g_MyD3D->MyPlayer[iMyPlayerIndex]->GetControlInfoBackAttackCount();
                mapControlInfoSkill = g_MyD3D->MyPlayer[iMyPlayerIndex]->GetControlInfoSkill();
                mapControlInfoPotion = g_MyD3D->MyPlayer[iMyPlayerIndex]->GetControlInfoPotion();

                KGC_PID_BROAD_CONTROL_MISSION_DATA kPacket( GC_PID_BROAD_CONTROL_MISSION_DATA, g_kGlobalValue.m_kUserInfo.dwUID,
                                                            iControlInfoComboCount, iControlInfoBackAttackCount, iControlInfoBeAttackedCount, 
                                                            mapControlInfoSkill, mapControlInfoPotion );
                SERIALIZE_P2P_PACKET( kPacket );
                SendServerRelayPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST_NOT_ME );
            }
            break;
        case GC_PID_SUMMON_INFINITY_MONSTER:
            {
                if( g_kGlobalValue.m_kUserInfo.bHost && g_kSummonInfinityMonster->m_usMonIndex != -1 && g_kSummonInfinityMonster->m_usMonType != -1 )
                {
                    SendP2PPacket( g_kSummonInfinityMonster, sizeof( KGC_PID_SUMMON_INFINITY_MONSTER ), Option, _RELIABLE );
                    g_kSummonInfinityMonster->Init();
                }
                KGCPC_MEMO( "GC_PID_SUMMON_INFNITY_MONSTER" );
                break;
            }
    }
    return S_OK;
}

bool SendServerRelayPacket( void* pkPacket_, ULONG ulPacketSize_, DWORD dwUID_ )
{
    //if ( g_kGlobalValue.m_kUserInfo.bLive != true ||
    //    !( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM || 
    //    g_MyD3D->m_pStateMachine->GetState() == GS_GAME ||
    //    g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING ||
    //    g_MyD3D->m_pStateMachine->GetState() == GS_GAME_END ) )
    //    return;

    //if ( ulPacketSize_ > MAX_COMPRESS_BUFFER - sizeof( DWORD ) )
    //{
    //    return false;
    //}

    static KRelayData s_kPacket;
    static BYTE pCompressBuffer[MAX_COMPRESS_BUFFER] = {0,};
    static BYTE pUncompressBuffer[MAX_COMPRESS_BUFFER] = {0,};
    static ULONG ulCompressLength = 0;

    s_kPacket.m_cRecvCnt = 0;
    s_kPacket.m_vecUID.clear();
    s_kPacket.m_buff.clear();

	bool bRet = true;

    //s_kPacket.m_vecUID.clear();
    //s_kPacket.m_dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    static std::vector<DWORD> vecUID;
    vecUID.clear();
    switch ( dwUID_ )
    {
        case KNC_BROADCAST:
        {
            for ( int  i = 0; i < MAX_PLAYER_NUM + NUM_TEMP_PLAYERINFO; i++ )
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
                {
                    vecUID.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
                }
            }
            if ( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() )
            {
                std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;
                for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
                {
                    vecUID.push_back(vit->dwUID);
                }
            }
            
            break;
        }
        case KNC_BROADCAST_NOT_ME:
        {            
            for ( int  i = 0; i <  MAX_PLAYER_NUM + NUM_TEMP_PLAYERINFO; i++ )
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true && g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID != g_kGlobalValue.m_kUserInfo.dwUID )
                {
                    vecUID.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
                }
            }
            if ( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() )
            {
                std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;
                for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
                {
                    vecUID.push_back(vit->dwUID);
                }
            }
            break;
        }
        case KNC_TCP_RELAY_TO_ME:
        {
            vecUID.push_back( g_kGlobalValue.m_kUserInfo.dwUID );
            break;
        }
        case KNC_RELAY_TO_ME:
        {
            vecUID.push_back( g_kGlobalValue.m_kUserInfo.dwUID );
        }
        default:
        {
            for ( int i = 0; i < MAX_PLAYER_NUM + NUM_TEMP_PLAYERINFO; i++)
            {
                if ( dwUID_ == g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
                {
                    vecUID.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
                    break;
                }                
            }
            break;
        }
    }   

    if ( vecUID.empty() )
    {
        return false;
    }

    
    memset( pCompressBuffer, 0, sizeof(BYTE)*MAX_COMPRESS_BUFFER );
    ulCompressLength = MAX_COMPRESS_BUFFER;

    memcpy( pCompressBuffer, &g_kGlobalValue.m_kUserInfo.dwUID, sizeof(g_kGlobalValue.m_kUserInfo.dwUID) );
    
    if ( Z_OK != compress( pCompressBuffer + sizeof(g_kGlobalValue.m_kUserInfo.dwUID), &ulCompressLength, (BYTE*)pkPacket_, ulPacketSize_) )
        return false;    
    
    KGCPC_VALUE( "tcp packet", ulCompressLength );

    int nErrorCode = 0;
	//CE_TCP_RELAY_SERVER_ERROR
	bRet = SiKP2P()->m_spTRUserProxy->Send( vecUID, pCompressBuffer, ulCompressLength + sizeof(DWORD), nErrorCode );

    // TR통계
    SiKGCNetworkStatManager()->AddStatCount( KGCNetworkStatManager::EM_TICK_TR_PACKET_COUNT, 1 );
    SiKGCNetworkStatManager()->AddStatCount( KGCNetworkStatManager::EM_TICK_TR_PACKET_SIZE, ulCompressLength + sizeof(DWORD) );
    SiKGCNetworkStatManager()->AddStatModeCount( KGCNetworkStatManager::EM_MODE_TR_PACKET_COUNT, 1 );


#if !defined ( __PATH__ )
    if ( !pTcpLogFile )
    {
        pTcpLogFile = fopen( "TcpPacketLog.txt", "wb" );
    }

    if ( pTcpLogFile ) 
    {
        std::wstring strTime = (LPCTSTR)CTime::GetCurrentTime().Format( _T("%Y-%m-%d %H_%M_%S") );
        KGC_PACKET_HEADER* pPacketHeader = (KGC_PACKET_HEADER*)pkPacket_;
        GCPID kiPacketID_ = pPacketHeader->Type;
        LTP_BASIC* pBasic = (LTP_BASIC*)pkPacket_;
        
        fwprintf( pTcpLogFile, L"%s\t%d\t%d\t%d\n", strTime.c_str(), (int)kiPacketID_, pBasic->packetType, ulCompressLength + sizeof(DWORD));
        fflush( pTcpLogFile );
    }
#endif

	if (bRet==false &&  
		!(SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME))
		//혼자 하는 게임인 경우에 TR서버 오류 안뜨도록 예외 처리.
	{
        ErrorCheckRelayPacket( pkPacket_, nErrorCode );   
	}

    return bRet;
}

void ErrorCheckRelayPacket( void* pkPacket_, int nErrorCode )
{
    bool bDungeon = (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON);

    if ( nErrorCode != KSocketObject::EM_SEND_SUCCESS )
    {
        START_LOG( cerr, L"TR Send Error " << nErrorCode );

        if ( pkPacket_ )
        {
            LTP_BASIC* pPacket = ( LTP_BASIC* ) pkPacket_;

            START_LOG( cerr, L"TR Send Error Type " << nErrorCode )
                << BUILD_LOG( pPacket->packetType )
                << BUILD_LOG( pPacket->Type );

        }
    }

    switch( nErrorCode )
    {
    case KSocketObject::EM_SEND_SUCCESS:
        break;
    case KSocketObject::EM_SEND_FAIL_CONNECT:
        {
            if ( bDungeon )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_DUNGEON_TCP_ERROR_CONNECT );
            }
            else
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_MATCH_TCP_ERROR_CONNECT );
            }
        }
        break;
    case KSocketObject::EM_SEND_FAIL_TARGET:
        {
            if ( bDungeon )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_DUNGEON_TCP_ERROR_TARGET );
            }
            else
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_MATCH_TCP_ERROR_TARGET );
            }
        }
        break;
    case KSocketObject::EM_SEND_FAIL_SIZE:
        {
            if ( bDungeon )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_DUNGEON_TCP_ERROR_SIZE );
            }
            else
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_MATCH_TCP_ERROR_SIZE );
            }
        }
        break;
    case KSocketObject::EM_SEND_FAIL_NODATA:
        {
            if ( bDungeon )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_DUNGEON_TCP_ERROR_NODATA );
            }
            else
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_MATCH_TCP_ERROR_NODATA );
            }
        }
        break;
    case KSocketObject::EM_SEND_FAIL_BUFFSIZE:
        {
            if ( bDungeon )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_DUNGEON_TCP_ERROR_BUFFSIZE );
            }
            else
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_MATCH_TCP_ERROR_BUFFSIZE );
            }
        }
        break;
    case KSocketObject::EM_SEND_FAIL_SENDERROR:
        {
            if ( bDungeon )
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_DUNGEON_TCP_ERROR_SENDERROR );
            }
            else
            {
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_MATCH_TCP_ERROR_SENDERROR );
            }
        }
        break;
    default:
        break;
    }


}
	
bool IsSendP2PPacketToTcpRelay() {


	if( g_eP2PConMode != P2P_TCP_RELAY_ONLY ) 
		return false;

// 	if( g_pGameBoard != NULL ) {
// 		if( g_pGameBoard->GetStartingMemberCount() == 1 ){
// 			return false;
// 		}
// 	}

	return true;
}

void SendP2PPacket( void* pkPacket_, UINT uiPacketSize_, DWORD dwUID_, UINT uiSendOption_ )
{
#if !defined( __PATH__ )
    if( -1 < g_kGlobalValue.m_iSendTimeForPacket )
    {
        if( false == g_kGlobalValue.m_kSendTimeForPacket.IsStop() )
        {
            g_kGlobalValue.m_kSendPacket.AddPacketData( static_cast<BYTE*>(pkPacket_), uiPacketSize_, dwUID_, uiSendOption_ );
            g_kGlobalValue.m_vecSendPacket.push_back( g_kGlobalValue.m_kSendPacket );

            std::vector<KAddSendPacket>::iterator vecIter = g_kGlobalValue.m_vecSendPacket.begin();
            for(; vecIter != g_kGlobalValue.m_vecSendPacket.end(); )
            {
                if( rand() % 10 == 0 )
                {
                    RealSendP2PPacket( vecIter->pData, vecIter->uiPacketSize, vecIter->dwUID, vecIter->uiSendOption );
                    vecIter = g_kGlobalValue.m_vecSendPacket.erase( vecIter );
                }
                else    break;
            }
            return;
        }

        std::vector<KAddSendPacket>::iterator vecIter = g_kGlobalValue.m_vecSendPacket.begin();
        for(; vecIter != g_kGlobalValue.m_vecSendPacket.end(); )
        {
            RealSendP2PPacket( vecIter->pData, vecIter->uiPacketSize, vecIter->dwUID, vecIter->uiSendOption );
            vecIter = g_kGlobalValue.m_vecSendPacket.erase( vecIter );
        }
        g_kGlobalValue.m_vecSendPacket.clear();
        g_kGlobalValue.m_kSendTimeForPacket.SetStopWatch( g_kGlobalValue.m_iSendTimeForPacket );
    }
#endif

    RealSendP2PPacket( pkPacket_, uiPacketSize_, dwUID_, uiSendOption_ );
}

void RealSendP2PPacket( void* pkPacket_, UINT uiPacketSize_, DWORD dwUID_, UINT uiSendOption_ )
{
    if( IsSendP2PPacketToTcpRelay() || dwUID_ == KNC_TCP_RELAY_TO_ME ) {
        switch( dwUID_ ) {
        case KNC_RELAY_BROADCAST_NOT_ME:
            dwUID_ = KNC_BROADCAST_NOT_ME;
            break;
        default:
            break;
        }
#if defined(__CHECK_PACKET__)
        //g_vecpairPacketCheckList.push_back(std::make_pair(((KGC_PACKET_HEADER*)pkPacket_)->Type,uiPacketSize_));
#endif
        KGCPC_VALUE( "tcp packet", uiPacketSize_ );
        SendServerRelayPacket( pkPacket_, uiPacketSize_, dwUID_ );
        return;
    }


    //#if !defined( _OPEN_TEST_ ) && defined( __PATH__ )
    //    if ( uiSendOption_ == _RELIABLE )
    //    {
    //        if( SendServerRelayPacket( pkPacket_, uiPacketSize_, dwUID_ ) )
    //        {
    //            return;
    //        }
    //    }
    //#endif
    //====================================================================================
    //          Packet 압축

    static void* s_pkPacket = NULL;
    static ULONG s_ulPacketSize = 0;
    memset( &g_kCompressPacket, 0, sizeof( KGC_PID_COMPRESSED_PACKET ) );    
    g_kCompressPacket.Type = GC_PID_COMPRESSED_PACKET;
    s_ulPacketSize = MAX_COMPRESS_BUFFER;
    if ( Z_OK == compress( g_kCompressPacket.pCompressBuffer, &s_ulPacketSize, (BYTE*)pkPacket_, uiPacketSize_ ) )
    {
        s_pkPacket = (void*)&g_kCompressPacket;
        s_ulPacketSize += sizeof(GCPID);
    }
    else
    {
        s_pkPacket = pkPacket_;
        s_ulPacketSize = (ULONG)uiPacketSize_;
    }
    KSingleton<KncP2P>::GetInstance()->Send( s_pkPacket, (int)s_ulPacketSize,
        dwUID_, static_cast<SendReliability>(uiSendOption_));


    SiKGCNetworkStatManager()->AddStatCount( KGCNetworkStatManager::EM_TICK_UR_PACKET_COUNT, 1 );
    SiKGCNetworkStatManager()->AddStatCount( KGCNetworkStatManager::EM_TICK_UR_PACKET_SIZE, s_ulPacketSize );
    SiKGCNetworkStatManager()->AddStatModeCount( KGCNetworkStatManager::EM_MODE_UR_PACKET_COUNT, 1 );

    //====================================================================================
    //                          Packet Profile Module
#ifdef _PACKET_PROFILE_
    static DWORD s_dwTime = timeGetTime();
    g_akSend[((KGC_PACKET_HEADER*)pkPacket_)->Type].dwCount++;
    g_akSend[((KGC_PACKET_HEADER*)pkPacket_)->Type].dwSize += (DWORD)s_ulPacketSize;
    if ( 1000 < timeGetTime() - s_dwTime )
    {
        if ( g_akSend[GC_PID_PLAYERLATENCY_FRAME_INFO].dwCount != 0 )
            g_kGlobalValue.m_iTestValue = (int)(g_akSend[GC_PID_PLAYERLATENCY_FRAME_INFO].dwSize/g_akSend[GC_PID_PLAYERLATENCY_FRAME_INFO].dwCount);
        s_dwTime = timeGetTime();
    }
#endif
    //====================================================================================
}

void SendP2PPacket( UINT uiPacketID_ )
{
    switch ( uiPacketID_ )
    {
        case GC_PID_GAME_BOARD_UPDATE:
        {
            if( g_kGlobalValue.m_kUserInfo.bHost == true )
            {
                KGC_PID_GAME_BOARD_UPDATE kPacket( GC_PID_GAME_BOARD_UPDATE, g_kGlobalValue.m_kUserInfo.dwUID, 
                                                   g_pGameBoard->GetMonsterNum(), g_pGameBoard->GetMonKillNum(), 
                                                   g_pGameBoard->GetGameScore() );

                for ( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
                {
                    kPacket.aKillNum[i] = g_pGameBoard->GetHowKill( i );
                    kPacket.aDeathNum[i] = g_pGameBoard->GetHowDie( i );
                }

                for ( int i = 0 ; i < TEAM_NUM ; ++i )
                {
                    kPacket.aTeamScore[i] = g_pGameBoard->GetTeamScore( i );
                }
                    
                g_MyD3D->MyReplay->Set_GameBoard( &kPacket );
                
                SERIALIZE_P2P_PACKET( kPacket );
                g_kPacketPacker.Pack( (void*)ksBuff.GetData(), (PACKED_ELEMENT_SIZE)ksBuff.GetLength() );
            }
            KGCPC_MEMO( "GC_PID_GAME_BOARD_UPDATE" );
            break;
        }
        case GC_PID_BROAD_SANTA_EVENT:
        {
            // 산타 날아 다니는 이벤트를 위한 동기화 패킷.
            #if defined(EVENT_SANTA)
            {
				static int iClearStage = 0;
				static int iAppearCount = 0;
//#ifndef _DEBUG
				// 한 스테이지에서 3번 초과 떨어뜨리지 않겠다.
				if ( g_kGlobalValue.m_kUserInfo.bHost == false ||
					( iClearStage == g_pkQuestManager->GetNumOfClearStage() && iAppearCount >= 2 ) )
					break;
//#endif
				float fFactorY = (g_MyD3D->MyStg->GetYLimit() / 7.0f * ( ((float)(rand()%100)) * 0.01f ));
                float fStartPosX = 0.0f;
                float fEndPosX = 0.0f;
                if ( rand()%2 == 0 )
                {
					fStartPosX = 0.1f;
					fEndPosX = g_MyD3D->MyStg->GetXLimit();
					fFactorY = fFactorY*-1.0f;
                }
                else
                {
					fStartPosX = g_MyD3D->MyStg->GetXLimit();
					fEndPosX = 0.1f;
                }
				D3DXVECTOR2 vStartPos = D3DXVECTOR2( fStartPosX, g_MyD3D->MyStg->GetYLimit() * 0.35f + fFactorY );
				D3DXVECTOR2 vEndPos = D3DXVECTOR2( fEndPosX, g_MyD3D->MyStg->GetYLimit() * 0.35f - fFactorY );

                KGC_PID_BROAD_SANTA_EVENT kPacket( GC_PID_BROAD_SANTA_EVENT, g_kGlobalValue.m_kUserInfo.dwUID, vStartPos.x, vStartPos.y, vEndPos.x, vEndPos.y );
                SERIALIZE_P2P_PACKET( kPacket );
                SendP2PPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(),KNC_BROADCAST, _RELIABLE);
//#ifndef _DEBUG
				if ( iClearStage != g_pkQuestManager->GetNumOfClearStage() )
				{
					iClearStage = g_pkQuestManager->GetNumOfClearStage();
					iAppearCount = 0;
				}
				else
				{
					++iAppearCount;
				}
//#endif
			}
            #endif //#if defined(EVENT_SANTA)
            break;
        }
        case GC_PID_BROAD_SANTAMON_EVENT:
        {
            //산타몬스터가 나타나기 위한 동기화 패킷
            #if defined( EVENT_SANTAMON )
            if( g_kGlobalValue.m_kUserInfo.bHost == false )
                break;

            D3DXVECTOR2 pos = g_MyD3D->MyStg->Get_Random_XY();
            KGC_PID_BROAD_SANTAMON_EVENT kPacket( GC_PID_BROAD_SANTAMON_EVENT, g_kGlobalValue.m_kUserInfo.dwUID, 3, (float)pos.x, (float)pos.y );
            SERIALIZE_P2P_PACKET( kPacket );
            SendP2PPacket( (void*)ksBuff.GetData(), ksBuff.GetLength(), KNC_BROADCAST, _RELIABLE );
            #endif
            break;
        }
		case GC_PID_RELAY_PING:
		{		
#if defined(CHECK_SEND_PING)
			if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME &&
				g_pGameBoard->GetStartingMemberCount() < 0 )
				break;                       

            if( m_kStopWatch.IsStop() ) {
                DWORD dwPing = SiKP2P()->m_spTRUserProxy->GetPing();

                if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
                {
                    if( dwPing > 0 )
                        KP2P::GetInstance()->Send_PingInfoNot(dwPing);
                }

                if( dwPing > 0 )
                    g_kGlobalValue.SetTRAvgPing(static_cast<int>(dwPing));

                m_kStopWatch.SetStopWatch(10000);

            }
#endif
			break;
		}
        default:
        {

            START_LOG( cerr, "Invalid P2P Packet ID" );

			break;
        }
    }
}

#if defined(_DEBUG) && !defined(_PACKET_PROFILE_)
    #define             P2P_PACKET_CASE( ID )                       case( ID ):                         \
                                                                    START_LOG( cout, #ID);              \
                                                                    return ON_##ID( pRecvData_, dwUID_, dwRecvSize_ )
    #define             _P2P_PACKET_CASE( ID )                      case( ID ):                         \
                                                                    return ON_##ID( pRecvData_, dwUID_, dwRecvSize_ )
#elif defined(_DEBUG) && defined(_PACKET_PROFILE_)
    #define             P2P_PACKET_CASE( ID )                       case( ID ):                         \
                                                                    START_LOG( cout, #ID);              \
                                                                    g_akRecv[ID].dwSize += dwRecvSize_; \
                                                                    g_akRecv[ID].dwCount++;             \
                                                                    return ON_##ID( pRecvData_, dwUID_, dwRecvSize_ )
    #define             _P2P_PACKET_CASE( ID )                      case( ID ):                         \
                                                                    g_akRecv[ID].dwSize += dwRecvSize_; \
                                                                    g_akRecv[ID].dwCount++;             \
                                                                    return ON_##ID( pRecvData_, dwUID_, dwRecvSize_ )
#elif !defined(_DEBUG)
    #define             P2P_PACKET_CASE( ID )                       case( ID ):                         \
                                                                    return ON_##ID( pRecvData_, dwUID_, dwRecvSize_ )
    #define             _P2P_PACKET_CASE( ID )                      case( ID ):                         \
                                                                    return ON_##ID( pRecvData_, dwUID_, dwRecvSize_ )
#endif

#define             FPRINTF_CASE( ID )                      case( ID ):                         \
														    strText << #ID <<"\n";              \
                                                            break;

void P2PPacketProc( GCPID kiPacketID_, PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{

#if !defined(__PATH__)
    std::wstringstream strText;
	switch( kiPacketID_ ) 
	{
		// 이 이후로는 게임 시작한 이후에 쓰는 Packet이다.
		FPRINTF_CASE( GC_PID_GAME_BOARD_UPDATE );        
		FPRINTF_CASE( GC_PID_PLAYERLATENCY_FRAME_INFO );               // 2
		FPRINTF_CASE( GC_PID_MONSTER_LATENCY_FRAME_INFO );
//        FPRINTF_CASE( GC_PID_MONSTER_LIVE_INFO );
//		FPRINTF_CASE( GC_PID_DATA_FOR_CALC_GAME_RESULT_FROM_HOST );
//		FPRINTF_CASE( GC_PID_GAME_RESULT_FROM_HOST );
		FPRINTF_CASE( GC_PID_SHOOTING_ARROW_TOURNAMENT );
		FPRINTF_CASE( GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT );
		FPRINTF_CASE( GC_PID_ACTION_LEAGUE_RESULT );
		FPRINTF_CASE( GC_PID_LATENCY_TRANSFER );
		FPRINTF_CASE( GC_PID_QUEST_REBIRTH );
		FPRINTF_CASE( GC_PID_BROAD_PLAYER_DEATH_INFO );
		FPRINTF_CASE( GC_PID_BROAD_SANTA_EVENT );
		FPRINTF_CASE( GC_PID_COMPRESSED_PACKET );
		FPRINTF_CASE( GC_PID_EFFECT_ITEM );
		FPRINTF_CASE( GC_PID_USEPETSKILL );
		FPRINTF_CASE( GC_PID_TRANSFORM );
		FPRINTF_CASE( GC_PID_TAG_CHARACTER );
		FPRINTF_CASE( GC_PID_TRANSNPC );
		FPRINTF_CASE( GC_PID_PACKED_PACKET );
		FPRINTF_CASE( GC_PID_TCP_PACKED_PACKET );
		FPRINTF_CASE( GC_PID_CHAT_PACKET );
		FPRINTF_CASE( GC_PID_DIALOGUE_PROGRESS );
		FPRINTF_CASE( GC_PID_SUMMON_MONSTER );
		FPRINTF_CASE( GC_PID_DROPITEM );
		FPRINTF_CASE( GC_PID_PLAYERGETITEM );
		FPRINTF_CASE( GC_PID_QUEST_REAULT_ITEM_DICE );
		FPRINTF_CASE( GC_PID_QUEST_STAGE_RESULT_SCORE );
		FPRINTF_CASE( GC_PID_QUEST_STAGE_POINT );        
		FPRINTF_CASE( GC_PID_GATE_STATE );
		FPRINTF_CASE( GC_PID_UDP_RELAYSERVER_HEART_BEAT );        
		FPRINTF_CASE( GC_PID_DAMAGE_PACKET );                
		FPRINTF_CASE( GC_PID_TG_DEFAULT );
		FPRINTF_CASE( GC_PID_SQUARE_ACTION );
		FPRINTF_CASE( GC_PID_SWAPWEAPON );
		FPRINTF_CASE( GC_PID_ENTER_GAME );
		FPRINTF_CASE( GC_PID_ANGELS_EGG_INFO );
		FPRINTF_CASE( GC_PID_COUPLE_EVENT_CREDIT );
		FPRINTF_CASE( GC_PID_MAGIC_MISSILE );
		//FPRINTF_CASE( GC_PID_QUEST_COMBO_SKILL_DAMAGE );
		FPRINTF_CASE( GC_PID_MONSTER_TARGET_REFRESH );
		FPRINTF_CASE( GC_PID_QUEST_RANK_COMBO_STATE );
		FPRINTF_CASE( GC_PID_SELECT_BONUSBOX);
		//FPRINTF_CASE( GC_PID_SELECT_BONUSBOX_FROM_HOST);
		FPRINTF_CASE( GC_PID_OPEN_BONUSBOX);
		FPRINTF_CASE( GC_PID_READY_BONUSBOX_SELECT);
		FPRINTF_CASE( GC_PID_START_BONUSBOX_SELECT);
		FPRINTF_CASE( GC_PID_BURNNING_POINT);
		FPRINTF_CASE( GC_PID_RELAY_PING );
		FPRINTF_CASE( GC_PID_CHANGE_MONSTER );
        FPRINTF_CASE( GC_PID_BROAD_EGOITEM_STATE );
        FPRINTF_CASE( GC_PID_WEAPONCHANGE );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_MONSTER );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_MONSTER_HP );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_MONSTER_STATE );
        FPRINTF_CASE( GC_PID_BROAD_ANGELS_EGG_DAMAGE );
        FPRINTF_CASE( GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER );
        FPRINTF_CASE( GC_PID_BROAD_DOTA_SUMMON_MONSTER );
        FPRINTF_CASE( GC_PID_BROAD_DOTA_POINT );
        FPRINTF_CASE( GC_PID_BUY_DOTA_GAMEITEM_REQ );
        FPRINTF_CASE( GC_PID_BUY_DOTA_GAMEITEM_ACK );
        FPRINTF_CASE( GC_PID_BROAD_DOTA_ZONE_POSITION );
        FPRINTF_CASE( GC_PID_BROAD_DOTA_ITEM_CHEAT );
        FPRINTF_CASE( GC_PID_BROAD_DOTA_MONSTERKILL_MSG );
        FPRINTF_CASE( GC_PID_BROAD_GAME_BURNNING );
        FPRINTF_CASE( GC_PID_BROAD_AGIT_BREAK );
        FPRINTF_CASE( GC_PID_BROAD_DUNGEON_PLAYSTATE );
        FPRINTF_CASE( GC_PID_BROAD_ESCORT_MONSTER_DESC );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END );
        FPRINTF_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END );
        FPRINTF_CASE( GC_PID_BROAD_ADD_DAMAGE );
        FPRINTF_CASE( GC_PID_BROAD_CAMERAZOOM_STATE );
        FPRINTF_CASE( GC_PID_BROAD_SYNCOBJECT );
        FPRINTF_CASE( GC_PID_BROAD_RENDER_HIT_NUMBER );
        FPRINTF_CASE( GC_PID_BROAD_CHANGE_MONSTER_STATE );
        FPRINTF_CASE( GC_PID_BROAD_MACRO_COMMUNITY );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_FRAME_INFO );
        FPRINTF_CASE( GC_PID_BROAD_EMBARKATION_REQ );
        FPRINTF_CASE( GC_PID_BROAD_EMBARKATION_ACK );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_SUMMON_REQ );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_SUMMON_ACK );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_DELETE );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_IN_PORTAL );
        FPRINTF_CASE( GC_PID_BROAD_DELETE_DAMAGE );
        FPRINTF_CASE( GC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_SYNC_CALLBACK );
        FPRINTF_CASE( GC_PID_BROAD_EMBARK_SYNC_UID );
        FPRINTF_CASE( GC_PID_BROAD_CONTROL_MISSION_DATA );
        

        FPRINTF_CASE( GC_PID_SUMMON_INFINITY_MONSTER );
	default:
		{

#if defined( COLLECT_RECEIVED_P2P_PACKET )
            FILE *fo = NULL;            
			strText << "ErrorPacket = " << kiPacketID_ <<"\n";
            fo = fopen("ReceivedP2PPacket.txt","a");
            fwprintf(fo,L"%s",strText.str().c_str());
            fclose(fo);
#else
            START_LOG( cerr, "P2PPacketProc Error!!" );
#endif
		}
	}	

#endif
    switch( kiPacketID_ ) 
    {
        // 이 이후로는 게임 시작한 이후에 쓰는 Packet이다.
        _P2P_PACKET_CASE( GC_PID_GAME_BOARD_UPDATE );        
        _P2P_PACKET_CASE( GC_PID_PLAYERLATENCY_FRAME_INFO );               // 2
        _P2P_PACKET_CASE( GC_PID_MONSTER_LATENCY_FRAME_INFO );
//         _P2P_PACKET_CASE( GC_PID_MONSTER_LIVE_INFO );
        P2P_PACKET_CASE( GC_PID_SHOOTING_ARROW_TOURNAMENT );
        P2P_PACKET_CASE( GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT );
        P2P_PACKET_CASE( GC_PID_ACTION_LEAGUE_RESULT );
        _P2P_PACKET_CASE( GC_PID_LATENCY_TRANSFER );
        P2P_PACKET_CASE( GC_PID_QUEST_REBIRTH );
        P2P_PACKET_CASE( GC_PID_BROAD_PLAYER_DEATH_INFO );
        P2P_PACKET_CASE( GC_PID_BROAD_SANTA_EVENT );
        _P2P_PACKET_CASE( GC_PID_COMPRESSED_PACKET );
        _P2P_PACKET_CASE( GC_PID_EFFECT_ITEM );
        _P2P_PACKET_CASE( GC_PID_USEPETSKILL );
        _P2P_PACKET_CASE( GC_PID_TRANSFORM );
        _P2P_PACKET_CASE( GC_PID_TAG_CHARACTER );
        _P2P_PACKET_CASE( GC_PID_TRANSNPC );
        _P2P_PACKET_CASE( GC_PID_PACKED_PACKET );
        _P2P_PACKET_CASE( GC_PID_TCP_PACKED_PACKET );
        _P2P_PACKET_CASE( GC_PID_CHAT_PACKET );
        P2P_PACKET_CASE( GC_PID_DIALOGUE_PROGRESS );
        P2P_PACKET_CASE( GC_PID_SUMMON_MONSTER );
        P2P_PACKET_CASE( GC_PID_DROPITEM );
        _P2P_PACKET_CASE( GC_PID_PLAYERGETITEM );
        P2P_PACKET_CASE( GC_PID_QUEST_REAULT_ITEM_DICE );
        P2P_PACKET_CASE( GC_PID_QUEST_STAGE_RESULT_SCORE );
        P2P_PACKET_CASE( GC_PID_QUEST_STAGE_POINT );        
        P2P_PACKET_CASE( GC_PID_GATE_STATE );
        P2P_PACKET_CASE( GC_PID_UDP_RELAYSERVER_HEART_BEAT );        
        P2P_PACKET_CASE( GC_PID_DAMAGE_PACKET );                
        P2P_PACKET_CASE( GC_PID_TG_DEFAULT );
        _P2P_PACKET_CASE( GC_PID_SQUARE_ACTION );
        _P2P_PACKET_CASE( GC_PID_SWAPWEAPON );
        P2P_PACKET_CASE( GC_PID_ENTER_GAME );
		P2P_PACKET_CASE( GC_PID_ANGELS_EGG_INFO );
		P2P_PACKET_CASE( GC_PID_COUPLE_EVENT_CREDIT );
		P2P_PACKET_CASE( GC_PID_MAGIC_MISSILE );
        _P2P_PACKET_CASE( GC_PID_MONSTER_TARGET_REFRESH );
        P2P_PACKET_CASE( GC_PID_QUEST_RANK_COMBO_STATE );
        P2P_PACKET_CASE( GC_PID_SELECT_BONUSBOX);        
        P2P_PACKET_CASE( GC_PID_OPEN_BONUSBOX);
        P2P_PACKET_CASE( GC_PID_READY_BONUSBOX_SELECT);
        P2P_PACKET_CASE( GC_PID_START_BONUSBOX_SELECT);
        P2P_PACKET_CASE( GC_PID_BURNNING_POINT);
        P2P_PACKET_CASE( GC_PID_RELAY_PING );
        P2P_PACKET_CASE( GC_PID_CHANGE_MONSTER );
        _P2P_PACKET_CASE( GC_PID_SHIELD );
        P2P_PACKET_CASE( GC_PID_BROAD_EGOITEM_STATE );
        P2P_PACKET_CASE( GC_PID_WEAPONCHANGE );
        P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_MONSTER );
        P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_MONSTER_HP );
        P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_MONSTER_STATE );
        _P2P_PACKET_CASE( GC_PID_BROAD_ANGELS_EGG_DAMAGE );
        _P2P_PACKET_CASE( GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER );
        P2P_PACKET_CASE( GC_PID_BROAD_DOTA_SUMMON_MONSTER );
        P2P_PACKET_CASE( GC_PID_BROAD_DOTA_POINT );
        P2P_PACKET_CASE( GC_PID_BUY_DOTA_GAMEITEM_REQ );
        P2P_PACKET_CASE( GC_PID_BUY_DOTA_GAMEITEM_ACK );
        _P2P_PACKET_CASE( GC_PID_BROAD_DOTA_ZONE_POSITION );
        _P2P_PACKET_CASE( GC_PID_BROAD_DOTA_ITEM_CHEAT );
        _P2P_PACKET_CASE( GC_PID_BROAD_DOTA_MONSTERKILL_MSG );
        _P2P_PACKET_CASE( GC_PID_BROAD_GAME_BURNNING );
        _P2P_PACKET_CASE( GC_PID_BROAD_AGIT_BREAK );
        _P2P_PACKET_CASE( GC_PID_BROAD_DUNGEON_PLAYSTATE );
        _P2P_PACKET_CASE( GC_PID_BROAD_ESCORT_MONSTER_DESC );
        _P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER );
        _P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER );
        _P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END );
        _P2P_PACKET_CASE( GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END );
        _P2P_PACKET_CASE( GC_PID_BROAD_ADD_DAMAGE );
        _P2P_PACKET_CASE( GC_PID_BROAD_CAMERAZOOM_STATE );
        _P2P_PACKET_CASE( GC_PID_BROAD_SYNCOBJECT );
        _P2P_PACKET_CASE( GC_PID_BROAD_RENDER_HIT_NUMBER );
        _P2P_PACKET_CASE( GC_PID_BROAD_CHANGE_MONSTER_STATE );
        _P2P_PACKET_CASE( GC_PID_BROAD_MACRO_COMMUNITY );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_FRAME_INFO );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARKATION_REQ );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARKATION_ACK );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_SUMMON_REQ );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_SUMMON_ACK );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_DELETE );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_IN_PORTAL );
        _P2P_PACKET_CASE( GC_PID_BROAD_DELETE_DAMAGE );
        _P2P_PACKET_CASE( GC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_SYNC_CALLBACK );
        _P2P_PACKET_CASE( GC_PID_BROAD_EMBARK_SYNC_UID );
        _P2P_PACKET_CASE( GC_PID_BROAD_CONTROL_MISSION_DATA );
        P2P_PACKET_CASE( GC_PID_SUMMON_INFINITY_MONSTER );
        

        default:
        {
            START_LOG( cerr, "P2PPacketProc Error!!" );
            return;
        }
    }
}

void ON_GC_PID_ANGELS_EGG_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
	KGC_PID_ANGELS_EGG_INFO* Custom = (KGC_PID_ANGELS_EGG_INFO*)pRecvData_;

	((KGCAngelsEgg*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetGameModePtr( GC_GMC_ANGELS_EGG ))->SetEggState( Custom->m_ucTeam, Custom->m_ucState, Custom->m_fHP, Custom->m_fMaxHP, Custom->m_bBreak );
}

void ON_GC_PID_MAGIC_MISSILE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
	KGC_PID_MAGIC_MISSILE* Custom = (KGC_PID_MAGIC_MISSILE*)pRecvData_;
	
	Result_Pos_MagicMissile.x = Custom->fPosX;
	Result_Pos_MagicMissile.y = Custom->fPosY;
    Result_Damage_Life = Custom->iLife;
}

void ON_GC_PID_PLAYERLATENCY_FRAME_INFO(PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_)           // 2
{
    KGC_PID_PLAYERLATENCY_FRAME_INFO* Custom = (KGC_PID_PLAYERLATENCY_FRAME_INFO*)pRecvData_;
    int Index;
    // 플레이어의 인덱스가 유효한지 확인한다.
    if ((Index = g_MyD3D->Get_Player(dwUID_)) >= MAX_PLAYER_NUM)
    {
        return;
    }

    g_MyD3D->MyPlayer[Index]->m_iCatchPlayerIndex = static_cast<int>(1000 * Custom->Body_Angle);
    g_MyD3D->MyPlayer[Index]->Body_Angle = Custom->Body_Angle;
    g_MyD3D->MyPlayer[Index]->First_Receive = true;
    for (int i = LATENCY - 1; i > -1; --i)
    {
#if defined(LEVEL_DESIGN_STAT_CALC)
        g_MyD3D->MyPlayer[Index]->L_HP[i] = static_cast<float>(Custom->iHP[i]);
#else
        g_MyD3D->MyPlayer[Index]->L_HP[i] = static_cast<float>((Custom->iHP[i]) / 10000.0f);
#endif
        g_MyD3D->MyPlayer[Index]->L_IsRight[i] = Custom->IsRight[i];
        g_MyD3D->MyPlayer[Index]->L_Frame[i] = Custom->Frame[i];
        g_MyD3D->MyPlayer[Index]->L_Now_Motion[i] = Custom->Now_Motion[i];
        g_MyD3D->MyPlayer[Index]->L_x[i] = GCUTIL_MATH::halfToFloat(Custom->usPosX[i]);
        g_MyD3D->MyPlayer[Index]->L_y[i] = GCUTIL_MATH::halfToFloat(Custom->usPosY[i]);
    }
    g_MyD3D->MyPlayer[Index]->m_cLife = Custom->Life;

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
        g_MyD3D->MyPlayer[Index]->UpdateTagCharInfo();

    if (g_MyD3D->MyPlayer[Index]->m_cLife == 0)
        g_pMagicEffect->ClearMagicEffect(Index, true);

    g_MyD3D->MyPlayer[Index]->pLatency = 0;

    if (g_MyD3D->MyPlayer[Index]->m_kUserInfo.bLive &&
        g_MyD3D->m_pStateMachine->GetState() == GS_GAME)
    {
        g_MyD3D->MyPlayer[Index]->m_kUserInfo.nUserState = GC_RUS_PLAYING;
    }
}

void ON_GC_PID_MONSTER_LATENCY_FRAME_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_MONSTER_LATENCY_FRAME_INFO* pPacket = (KGC_PID_MONSTER_LATENCY_FRAME_INFO*)pRecvData_;

    // 파티원이 먼저 다음스테이지로 이동했을때 이전 스테이지 레이턴시로 덮어쓰임을 방지하기 위함.
    if( g_kEveryTriggerManager.GetActiveStage() != pPacket->iStageID )
        return;

    int iMonsterIndex = (int)pPacket->cMonsterIndex;

    if( pPacket->iMonsterKind == MONSTER::EMON_AGIT_CHAR ) {
		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER* pMonster = mit->second;
			if( pMonster->IsLive() == false )
				continue;

			if( pPacket->iMonsterType != pMonster->GetMonsterType() )
				continue;

			iMonsterIndex = mit->first;
			break;

		}
    }

    // 몬스터의 인덱스가 유효한지 확인한다.
	MONSTER* pMonster = g_kMonsterManager.GetMonster( iMonsterIndex );
	if(pMonster == NULL )
        return;

    if( SiKGCAgitGameManager()->GetExitGame() == false ) {
        if( pMonster->IsLive() == false && pPacket->iMonsterKind == MONSTER::EMON_AGIT_CHAR ) {
            int iCharType = SiKGCAgitGameManager()->GetCharTypeFromAgitMon( pPacket->iMonsterType );

            if( iCharType != GC_CHAR_INVALID ) {
                iMonsterIndex = SiKGCAgitGameManager()->CreatePlaceCharacter( iCharType, 0.0f, 0.0f );
                pMonster = g_kMonsterManager.GetMonster( iMonsterIndex );
            }
        }
    }

    // 방장과 파티원의 슬롯정보가 다르면 방장이 보내준 정보로 몬스터를 다시 생성한다.
    if( pMonster->GetMonsterType() != pPacket->iMonsterType )
    {
        pMonster->EndMonster( true, true, true );
        int iMonsterIdex = g_MyD3D->AddMonster( pPacket->iMonsterType, pPacket->ausX[0], pPacket->ausY[0], pPacket->abIsRight[0], false, pPacket->iBoss, pPacket->iLevel, 0x00, false, false, pMonster->m_iMonsterIndex);
        pMonster->m_bLive = true;
        pMonster->m_bRender = true;
        pMonster->ToggleMonsterInfo( true );
        pMonster->ClearTraceIcon();
    }

    pMonster->m_fHP = (float)pPacket->uiHP;
    pMonster->Body_Angle = (float)( pPacket->usBodyAngle / 10000.0f );
    if ( pPacket->cTargetIndex > MAX_PLAYER_NUM )
    {
        pMonster->SetMonsterTargetManual( (int)(pPacket->cTargetIndex - MAX_PLAYER_NUM ) );
    }
    else
    {
        pMonster->SetPlayerTargetManual( (int)pPacket->cTargetIndex );
    }
    pMonster->m_iLastAttackedMe = (int)pPacket->cLastAttackedMe;

    pMonster->m_iRemainFrame = LATENCY;

    for( int i = 0; i < LATENCY; ++i )
    {
        pMonster->m_aiMotion[i] = (int)pPacket->aucMotion[i];
        pMonster->m_aiFrame[i] = (int)pPacket->aucFrame[i];

        pMonster->SetLatencyX( i, GCUTIL_MATH::halfToFloat( pPacket->ausX[i] ) );
        pMonster->SetLatencyY( i, GCUTIL_MATH::halfToFloat( pPacket->ausY[i] ) );

        pMonster->m_abIsRight[i] = pPacket->abIsRight[i];
    }

    for( int i = LATENCY; i < LATENCY + SHOWLATENCY; ++i )
    {
        pMonster->m_aiMotion[i] = (int)pPacket->aucMotion[ LATENCY - 1 ];
        pMonster->m_aiFrame[i] = (int)pPacket->aucFrame[ LATENCY - 1 ];

        pMonster->SetLatencyX( i, GCUTIL_MATH::halfToFloat( pPacket->ausX[ LATENCY - 1 ] ) );
        pMonster->SetLatencyY( i, GCUTIL_MATH::halfToFloat( pPacket->ausY[ LATENCY - 1 ] ) );

        pMonster->m_abIsRight[i] = pPacket->abIsRight[ LATENCY - 1 ];
    }
}

void ON_GC_PID_MONSTER_LIVE_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_MONSTER_LIVE_INFO* pPacket = (KGC_PID_MONSTER_LIVE_INFO*)pRecvData_;

//     for( int i = 0 ; i < MAX_MONSTER ; ++i ) {
// 
//         if( g_kMonsterManager.m_mapMonster[i]->IsLive() == true &&
//             pPacket->LiveInfo[i] == false ) {
//                 g_kMonsterManager.m_mapMonster[i]->EndMonster( false, true );
//         }
//     }
}

bool SetPlayerCharacterExp( IN int iPlayerIndex, IN std::map< int, std::map< char, GCEXPTYPE > >& mapData, 
                                              OUT std::map< char, GCEXPTYPE >& mapCharacterExp )
{
    std::map< int, std::map< char, GCEXPTYPE > >::iterator mit = mapData.find( iPlayerIndex );
    if( mit == mapData.end() )
    {
        // 이런 경우는 있으면 안됨
        //  ASSERT( !"IsLive인 녀석의 캐릭터 경험치 정보가 없음" );
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_CHARACTER_EXP_NOT_FOUND );
        return false;
    }
    mapCharacterExp = mit->second;
    return true;
}

void WinLoseProcess( SUserInfo* pUserInfo, std::vector< std::pair<char,int> >& vecWin, std::vector< std::pair<char,int> >& vecLose )
{
    std::vector< std::pair<char,int> >::iterator vitPairWin;
    for( vitPairWin = vecWin.begin(); vitPairWin != vecWin.end(); ++vitPairWin )
    {
        KCharInfoVector::iterator vitChar = std::find_if( pUserInfo->vecCharInfo.begin(), pUserInfo->vecCharInfo.end(),
            boost::bind( &SCharInfo::iCharType, _1 ) == vitPairWin->first );

        if( vitChar != pUserInfo->vecCharInfo.end() )
        {
            vitChar->iWin = vitPairWin->second;
        }
    }

    std::vector< std::pair<char,int> >::iterator vitPairLose;
    for( vitPairLose = vecLose.begin(); vitPairLose != vecLose.end(); ++vitPairLose )
    {
        KCharInfoVector::iterator vitChar = std::find_if( pUserInfo->vecCharInfo.begin(), pUserInfo->vecCharInfo.end(),
            boost::bind( &SCharInfo::iCharType, _1 ) == vitPairLose->first );

        if( vitChar != pUserInfo->vecCharInfo.end() )
        {
            vitChar->iLose = vitPairLose->second;
        }
    }
}

void ON_GC_PID_SHOOTING_ARROW_TOURNAMENT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )          // 8
{
    int Index=g_MyD3D->Get_Player(dwUID_);
    KGC_PID_SHOOTING_ARROW_TOURNAMEN *Custom = (KGC_PID_SHOOTING_ARROW_TOURNAMEN*)pRecvData_;
    g_MyD3D->MyPlayer[Index]->First_Receive=true;
    g_MyD3D->MyPlayer[Index]->Body_Angle=Custom->Body_Angle;
    if(Custom->Shot) 
    {
        g_MyD3D->bShot=true;        
        if(g_kGlobalValue.m_kUserInfo.bHost)
        { // 호스트인 경우 경기를 빨리 끝나게 해준다.
           g_MyD3D->Story=200+730*Index+625;
        }        
    }
    if(Custom->TimeOut) 
    {
        g_MyD3D->Story=200+730*Index+629;
        if(g_kGlobalValue.m_kUserInfo.bHost)
        {
            if(g_MyD3D->DistanceLevel<3)
                g_aiArrowScore[Index][g_MyD3D->DistanceLevel]=0;
        }
    }
  
    if(Custom->RequestStartMessage){
        ShotPacket->Shot = false;
        SendNetworkData( GC_PID_SHOOTING_ARROW_TOURNAMENT, 0);
    }
}
void ON_GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )   // 9
{
    KGC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT *Custom = (KGC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT*)pRecvData_;
    g_MyD3D->Story=Custom->Story;
    g_MyD3D->ArrowHeight=Custom->Height;
    g_MyD3D->DistanceLevel=Custom->Level;
    //g_MyD3D->Process = -1;

    for(int i=0;i<MAX_PLAYER_NUM;i++)
    {
        for(int ii=0;ii<3;ii++)
        {
            g_aiArrowScore[i][ii]=Custom->Score[i][ii];
        }
    }                          
}
void ON_GC_PID_ACTION_LEAGUE_RESULT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )               // 10
{
    KGC_PID_ACTION_LEAGUE_RESULT *Custom = (KGC_PID_ACTION_LEAGUE_RESULT*)pRecvData_;
    int Index=g_MyD3D->Get_MyPlayer();
    if(Index<6)
    {
        if(g_pGameBoard->m_pStartingMember[Index].IsWin)
        {
            g_TeamPoint=Custom->Team_Point;
        }                            
    }    
}
void ON_GC_PID_LATENCY_TRANSFER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )                   // 12
{
    g_LatencyTransfer.PacketHandler((LTP_BASIC*)pRecvData_);
}

void ON_GC_PID_QUEST_REBIRTH( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )                      // 13
{
    KGC_PID_QUEST_REBIRTH* packet = (KGC_PID_QUEST_REBIRTH*)pRecvData_;
    KGCGameModeInterface* pMode = g_pStateGame->GetCurGameModePtr();
    if( packet->bRequest )
    {
        //if( pMode->VerifyRebirth() )
        //{
            packet->dwUserDPNID = dwUID_;
            packet->bRequest = false;
            packet->bOK = true;

            SendP2PPacket( (void*)packet, sizeof( KGC_PID_QUEST_REBIRTH ), KNC_BROADCAST, _RELIABLE );
            KGCPC_MEMO( "KGC_PID_QUEST_REBIRTH" );
        //}
    }
    else if(packet->bOK)        //살아나면됨
    {
        pMode->Rebirth( packet->dwUserDPNID );
    }
    g_pStateGame->GetCurGameModePtr()->SetRequestRebirth( false );
}

void ON_GC_PID_BROAD_EGOITEM_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EGOITEM_STATE *kPacket = (KGC_PID_BROAD_EGOITEM_STATE*)pRecvData_;

    int iPlayerIdx = g_MyD3D->Get_Player(kPacket->dwUID);
    if( 0 <= iPlayerIdx && iPlayerIdx <= MAX_PLAYER_NUM )
        g_MyD3D->MyPlayer[ iPlayerIdx ]->SetEgoItemOrderPlayer( kPacket->iPlayerID, kPacket->dwItemID, kPacket->iModelID, kPacket->iConditionID );
}

void ON_GC_PID_BROAD_DOTA_SUMMON_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DOTA_SUMMON_MONSTER *kPacket = (KGC_PID_BROAD_DOTA_SUMMON_MONSTER*)pRecvData_;

	int iSlot = g_MyD3D->AddMonster( kPacket->MonsterID, kPacket->vecSummonPos.x, kPacket->vecSummonPos.y, kPacket->bIsRight, false, 0, -1, 0, true, false, kPacket->iSlot);
	
	MONSTER *pMonster = g_kMonsterManager.GetMonster(iSlot);
	
	if ( pMonster == NULL )
		return;

	pMonster->m_iTeam = kPacket->iTeam;
	pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
	pMonster->m_dwOwnerUID = kPacket->iGateID + MAX_PLAYER_NUM;
	pMonster->m_bLive = true;
	pMonster->m_bRender = true;
	pMonster->SetImitSummon( false );
	pMonster->SetDotaGateIsRight( kPacket->bIsRight );
	pMonster->ToggleMonsterInfo( true );

	// 도타템플릿 얻기
	SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();

	if( pDotaTemplate )
	{
		// 플레이어의 Item_Remain_Time을 보고 같은 팀 몬스터라면 능력치 적용
		bool bDef, bSpeed;
		bDef = bSpeed = false;
		for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
		{       
			if( kPacket->iTeam == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
			{
				// NPC몬스터 방어력 증가
				if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_NPC_DEFANCEUP] > 0 )
					bDef = true;

				// NPC몬스터 속도 증가
				if( g_MyD3D->MyPlayer[iLoop]->Item_Remain_Time[GC_GAME_ITEM_DOTA_NPC_SPEEDUP] > 0 )
					bSpeed = true;
			}
		}

		// 소환 몬스터 기본능력치 처리
		pMonster->m_fHPPoint = float(kPacket->iHP);

		// 아이템으로 처리된 능력치 적용
		if( TEAM_SERDIN == kPacket->iTeam )
		{
			pMonster->m_fAttackPoint += pDotaTemplate->sDPointItemInfo.fAccSerdinNPCAtk;
			pMonster->m_fHPPoint += pDotaTemplate->sDPointItemInfo.fAccSerdinNPCHp;
		}
		else
		{
			pMonster->m_fAttackPoint += pDotaTemplate->sDPointItemInfo.fAccCanabanNPCAtk;
			pMonster->m_fHPPoint += pDotaTemplate->sDPointItemInfo.fAccCanabanNPCHp;
		}
		pMonster->m_fHP = pMonster->m_fHPPoint;

		if( bDef )
		{
			pMonster->m_fDefRatio += pDotaTemplate->sDPointItemInfo.fNPCDef;
		}
		if( bSpeed )
		{
			pMonster->m_fWalkSpeed += pDotaTemplate->sDPointItemInfo.fNPCSpeed;
			pMonster->m_fRunSpeed += pDotaTemplate->sDPointItemInfo.fNPCSpeed;
		}
	}


}

void ON_GC_PID_BROAD_SUMMON_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_MONSTER *kPacket = (KGC_PID_BROAD_SUMMON_MONSTER*)pRecvData_;

    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
    //UINT GetCurrentMotion() { return uiMotion; }
    UINT uiPlayerMotion = pPlayer->GetCurrentMotion();
    
    if( pPlayer )
    {
        // 소환 마릿수 제한
        pPlayer->SummonMonsterLimit( kPacket->iSummonLimit, kPacket->MonsterID );
        pPlayer->m_bCheckDieSummons = true;
        // 몬스터 소환
        int iMonsterIdex = g_MyD3D->AddMonster( kPacket->MonsterID, kPacket->vecSummonPos.x, kPacket->vecSummonPos.y, kPacket->IsRight,false,0,-1,0x00,false,false, kPacket->iSlotID);
        
		MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIdex);

		if ( pMonster == NULL ) return;

        

		pMonster->m_iTeam = kPacket->iTeam;
        pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;
        pMonster->m_dwOwnerUID = kPacket->dwUID;//pPlayer->m_kUserInfo.dwUID
        pMonster->m_bLive = true;
        pMonster->m_bRender = true;
        pMonster->ToggleMonsterInfo( true );
        pMonster->ClearTraceIcon();        

        if( g_kGlobalValue.GetEscortMonsterID() == kPacket->MonsterID )
            pMonster->SetMonsterKind(MONSTER::EMON_ESCORT);

        // 몬스터 능력치 설정 : 플레이어가 소환한 몬스터는 플레이어 능력치에 따라 몬스터도 따라간다.
        SCharInfo sCharInfo = pPlayer->GetCurrentChar();

        // 호위 몬스터일 경우 서버에서 받은 HP 그대로 유지
        if( pMonster->GetMonsterKind() == MONSTER::EMON_ESCORT || kPacket->fStaticMaxHP != -1.f )
        {
            pMonster->m_iLevel = sCharInfo.iLevel;
            pMonster->m_fHP = kPacket->fStaticMaxHP;
            pMonster->m_fHPPoint = kPacket->fStaticMaxHP;
            pMonster->m_fOldHP = pMonster->m_fHP;
            pMonster->m_bUnlimitDef = kPacket->bUnlimitDef;
        }
        else if ( -1 != sCharInfo.iCharType )
        {
            // 레벨
            pMonster->m_iLevel = sCharInfo.iLevel;

            // 생명력
            if( kPacket->fMotherHpRatio != -1.f ) {
                pMonster->m_fHPPoint = pPlayer->GetPlayerMaxHP() + ( pPlayer->GetPlayerMaxHP() * kPacket->fMotherHpRatio );                
            } else {
                pMonster->m_fHPPoint = ( ( sCharInfo.iLevel + 1 ) * kPacket->fMaxHP );
            }
            pMonster->m_fHP = pMonster->m_fHPPoint;
            pMonster->m_fOldHP = pMonster->m_fHP;
        }

        if( kPacket->fSummonsAtkRatio != -1.f ) {
            pMonster->m_fSummonsAtkRatio = kPacket->fSummonsAtkRatio;
            pMonster->m_fOwnerAtkRatio = kPacket->fOwnerAtkRatio;
        }

        // 플레이어와 몬스터 간 제한거리 설정
        pPlayer->SetSummonMonsterLimitDist( kPacket->fLimitDistance );

        // 소환 버프 아이콘 출력
        if( pPlayer->IsLocalPlayer() && kPacket->iBuffIcon )
        {
            pMonster->m_iSummonBuffIcon = kPacket->iBuffIcon;
            pPlayer->SetMagicEffect( kPacket->iBuffIcon, 999999.0f );
        }

        // 내 소환수 색깔은 틀리게...

        if( pMonster->m_dwOwnerUID == g_kGlobalValue.m_kUserInfo.dwUID && SiKGCRoomManager()->IsMonsterGameMode() == false ) {
            pMonster->SetUserEdgeColor( 0.0f, 0.7f, 0.0f, 0.0f );
        }
    }
}

void ON_GC_PID_BROAD_SUMMON_MONSTER_HP( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_MONSTER_HP *kPacket = (KGC_PID_BROAD_SUMMON_MONSTER_HP*)pRecvData_;

    //if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
        if( pPlayer )
            pPlayer->SetSummonMonsterHP( kPacket->fHpRadio, kPacket->cIndex, kPacket->bDieType, static_cast< EGCMonster >( kPacket->iMonID ) );
    }
}

void ON_GC_PID_BROAD_SUMMON_MONSTER_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_MONSTER_STATE *kPacket = (KGC_PID_BROAD_SUMMON_MONSTER_STATE*)pRecvData_;

    //if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster( kPacket->iMonsterSlotIndex );
		if(pMonster == NULL )
			return;
        pMonster->m_iRecvStateFromPlayer = kPacket->iMonsterState;
    }
}

void ON_GC_PID_BROAD_CHANGE_MONSTER_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_CHANGE_MONSTER_STATE *kPacket = (KGC_PID_BROAD_CHANGE_MONSTER_STATE*)pRecvData_;

    MONSTER* pMonster = g_kMonsterManager.GetMonster( kPacket->iSlotID );
    if( NULL == pMonster )
        return;

    if( kPacket->iMonsterID != pMonster->m_iMonsterType )
        return;
    
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( pMonster->m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mapIter = mapAction.begin();
    for(; mapIter != mapAction.end(); ++mapIter)
    {
        if( kPacket->iStateID == mapIter->second.m_iMotionIndex )
            break;
    }

    if( mapIter != mapAction.end() )
    {
        pMonster->SetState( mapIter->first.c_str() );
        pMonster->SetRequestUserUID( kPacket->dwUID );
    }
}

void ON_GC_PID_BROAD_MACRO_COMMUNITY( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_MACRO_COMMUNITY *kPacket = (KGC_PID_BROAD_MACRO_COMMUNITY*)pRecvData_;
    
    std::map<int, SMacroCommunity> mapMacroCommunity = SiKGCWorldMapManager()->GetMacroCommunity( SiKGCRoomManager()->GetGameMode() );
    if( false == mapMacroCommunity.empty() )
    {
        std::map<int, SMacroCommunity>::iterator mapIter = mapMacroCommunity.find( (kPacket->iCommunitySlot+1) );
        if( mapIter != mapMacroCommunity.end() )
        {
            DWORD dwColor = D3DCOLOR_ARGB(255, mapIter->second.m_iStringColorR, mapIter->second.m_iStringColorG, mapIter->second.m_iStringColorB);
            g_pkChatManager->AddMacroCommunityMsg( kPacket->dwUID, g_pkStrLoader->GetString( mapIter->second.m_iStringID ), mapIter->second.m_iSoundID, dwColor );
        }
    }
}

void ON_GC_PID_BROAD_AGIT_BREAK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_AGIT_BREAK *kPacket = (KGC_PID_BROAD_AGIT_BREAK*)pRecvData_;

    if( kPacket )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
        if( pPlayer )
        {
            int iLoop = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_SUMMON );

            g_MyD3D->AddMonster( kPacket->iMonID, (kPacket->vPos.x / SiKGCFantasticMap()->GetPixelWidth()), kPacket->vPos.y, true, false, 0, -1, 0, false, false, iLoop );
			
			MONSTER *pMonster = g_kMonsterManager.GetMonster(iLoop);

			if ( pMonster == NULL )
				return;

            pMonster->m_iTeam = pPlayer->m_kUserInfo.iTeam;
            pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;
            pMonster->m_dwOwnerUID = pPlayer->m_kUserInfo.dwUID;
            pMonster->m_bLive = true;
            pMonster->m_bRender = true;
            pMonster->ToggleMonsterInfo( true );
            pMonster->m_iRecvStateFromPlayer = kPacket->iDiscCnt;
            
        }
    }
}

void ON_GC_PID_BROAD_ANGELS_EGG_DAMAGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_ANGELS_EGG_DAMAGE *kPacket = (KGC_PID_BROAD_ANGELS_EGG_DAMAGE*)pRecvData_;

    //if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( g_MyD3D->IsPlayerIndex( kPacket->iKiller ) && g_MyD3D->IsPlayerIndex( kPacket->iGhost ) )
        {
            g_MyD3D->MyPlayer[kPacket->iKiller]->AddDamageWithNoDirectionLocateAbsolutePos( 
                kPacket->iDamageEnum,
                g_MyD3D->MyPlayer[kPacket->iGhost]->vPos.x, 
                g_MyD3D->MyPlayer[kPacket->iGhost]->vPos.y );
        }
    }
}

void ON_GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER *kPacket = (KGC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER*)pRecvData_;

	g_MyD3D->AddMonster( kPacket->iMonType, kPacket->vPos.x, kPacket->vPos.y, kPacket->bIsRight, false, 0, -1, 0, false, false, kPacket->iSlot);

	MONSTER *pMonster = g_kMonsterManager.GetMonster(kPacket->iSlot);

    if( pMonster != NULL )
    {
       
        pMonster->m_iTeam = kPacket->iTeam;
        pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
        pMonster->m_dwOwnerUID = -1;
        pMonster->m_bLive = true;
        pMonster->m_bRender = true;
        pMonster->ToggleMonsterInfo( true );

        // 몬스터 스탯 설정
        pMonster->m_fHPPoint = kPacket->fHP;
        pMonster->m_fHP = pMonster->m_fHPPoint;
        //pMonster->m_fDefRatio = 300.0f;
        pMonster->m_fDefRatio = 30.0f;

        

        // 로딩완료셋팅
        ((KGCAngelsEgg*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetGameModePtr( GC_GMC_ANGELS_EGG ))->m_iMonSlotNum[kPacket->iTeam] = kPacket->iSlot;
        ((KGCAngelsEgg*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetGameModePtr( GC_GMC_ANGELS_EGG ))->SetAngelsEggBeforeHp(kPacket->iTeam, pMonster->GetHP());
        ((KGCAngelsEgg*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetGameModePtr( GC_GMC_ANGELS_EGG ))->InitAngelsEggHP( kPacket->iTeam );
        ((KGCAngelsEgg*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetGameModePtr( GC_GMC_ANGELS_EGG ))->InitAngelsEggTexture( kPacket->iTeam );
        ((KGCAngelsEgg*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME ))->GetGameModePtr( GC_GMC_ANGELS_EGG ))->SetLoadingComplate(kPacket->iTeam, true);
    }
}

void ON_GC_PID_BROAD_DOTA_ITEM_CHEAT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DOTA_ITEM_CHEAT *kPacket = (KGC_PID_BROAD_DOTA_ITEM_CHEAT*)pRecvData_;

    SDotaTemplate* pSDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
    if( pSDotaTemplate )
    {
        pSDotaTemplate->sDPointItemInfo.fAtk=           kPacket->fAtk;
        pSDotaTemplate->sDPointItemInfo.fHp=            kPacket->fHp;
        pSDotaTemplate->sDPointItemInfo.fFullHp=        kPacket->fFullHp;
        pSDotaTemplate->sDPointItemInfo.fCritical=      kPacket->fCritical;
        pSDotaTemplate->sDPointItemInfo.fNPCAtk=        kPacket->fNPCAtk;
        pSDotaTemplate->sDPointItemInfo.fNPCDef=        kPacket->fNPCDef;
        pSDotaTemplate->sDPointItemInfo.fNPCHp=         kPacket->fNPCHp;
        pSDotaTemplate->sDPointItemInfo.fNPCSpeed=      kPacket->fNPCSpeed;
    }
}

void ON_GC_PID_BROAD_DOTA_MONSTERKILL_MSG( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DOTA_MONSTERKILL_MSG *kPacket = (KGC_PID_BROAD_DOTA_MONSTERKILL_MSG*)pRecvData_;

    SDotaTemplate* pSDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
    if( pSDotaTemplate )
    {
        int iKillerTeam;
        if(0 <= kPacket->iMsgID && kPacket->iMsgID < SDotaPlayingDesc::MAX_DOTA_MSG )
        {
        if( kPacket->bKillerMonster )
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster( kPacket->iKillerIdx );
			if(pMonster != NULL ) {
                KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                KInGameMessageAttribute kMessage;
                kMessage.iStringID = pSDotaTemplate->sDotaMsgInfo.mapDotaMsg[kPacket->iMsgID].first;
                kMessage.iShowFrame = pSDotaTemplate->sDotaMsgInfo.mapDotaMsg[kPacket->iMsgID].second;
                kMessage.strValue1 = const_cast<WCHAR*>(pMonster->GetMonsterName());
                kMessage.strValue2 = const_cast<WCHAR*>(g_pkStrLoader->GetString( kPacket->iTeamStringID ).c_str());
                pMessage->SetMessage( &kMessage );

                iKillerTeam = pMonster->m_iTeam;
			}
        }
        else
        {
                KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                KInGameMessageAttribute kMessage;
                kMessage.iStringID = pSDotaTemplate->sDotaMsgInfo.mapDotaMsg[kPacket->iMsgID].first;
                kMessage.iShowFrame = pSDotaTemplate->sDotaMsgInfo.mapDotaMsg[kPacket->iMsgID].second;
                kMessage.strValue1 = const_cast<WCHAR*>(g_MyD3D->MyPlayer[kPacket->iKillerIdx]->GetStrUserName().c_str());
                kMessage.strValue2 = const_cast<WCHAR*>(g_pkStrLoader->GetString( kPacket->iTeamStringID ).c_str());
                pMessage->SetMessage( &kMessage );

                // 가드타워나 가디언타워가 깨졌다면 다음목표를 알려주자!
                int iPlayerIdx = g_MyD3D->Get_MyPlayer();
                if( g_MyD3D->IsPlayerIndex(iPlayerIdx) && 
                    (g_MyD3D->MyPlayer[kPacket->iKillerIdx]->m_kUserInfo.iTeam == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam) )
                {
                    switch( kPacket->iMsgID )
                    {
                    case SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDIANTOWER:
                        g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_BREAK_GUARDIANTOWER );
                        break;
                    case SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDTOWER:
                        g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_BREAK_GUARDTOWER );
                        break;
        }
    }
                iKillerTeam = g_MyD3D->MyPlayer[kPacket->iKillerIdx]->m_kUserInfo.iTeam;
}
        }

        // 내가 죽였다면 APOINT에 대한 가이드를 출력하자!
        if( g_MyD3D->Get_MyPlayer() == kPacket->iKillerIdx )
        {
            g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_FIRST_KILL );
            g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_APOINT );
        }

        // 킬러와 다른팀일때 위험한 상황에 대한 파티클 처리를 해주자!
        int iMyPlayer = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex(iMyPlayer) && (iKillerTeam != g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam) )
        {
            switch( kPacket->iMsgID )
            {
            case SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDIANTOWER:
            case SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDTOWER:
                D3DXVECTOR3 vPos = g_MyD3D->MyPlayer[iMyPlayer]->GetPosition();
                g_ParticleManager->CreateSequence( "Dota_Warning", vPos.x, vPos.y, 1.0f );
                break;
            }
        }

        // 가드타워가 깨졌을때 그 위치에 파티클을 띄우주도록 하자.
        if( kPacket->iMsgID == SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDTOWER )
        {
            g_ParticleManager->CreateSequence( "Dota_Open_01", kPacket->vDeathPos.x, kPacket->vDeathPos.y, 1.0f );
            g_ParticleManager->CreateSequence( "Dota_Open_02", kPacket->vDeathPos.x, kPacket->vDeathPos.y, 1.0f );
            g_ParticleManager->CreateSequence( "Dota_Open_03", kPacket->vDeathPos.x, kPacket->vDeathPos.y, 1.0f );
        }
    }
}

void ON_GC_PID_BROAD_GAME_BURNNING( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_GAME_BURNNING *kPacket = (KGC_PID_BROAD_GAME_BURNNING*)pRecvData_;

    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
    if( pPlayer && false == pPlayer->IsLocalPlayer() )
    {
        // 버닝모드 발동자와 네트워크 싱크가 맞지 않다면 보정해주어야한다.
        if( pPlayer->m_bIsBurnningMode == kPacket->bBurnning )
        {
            pPlayer->m_bIsBurnningMode = !kPacket->bBurnning;
            swap( pPlayer->GetCurFormTemplate(), pPlayer->GetCurBurnningTemplate() );
        }
        pPlayer->ChangeBurnningTemplate();
    }
}

void ON_GC_PID_BROAD_PLAYER_DEATH_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_PLAYER_DEATH_INFO kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );

    if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
    {
        return;
    }  

    int iPlayerIdx = g_MyD3D->Get_Player(kPacket.dwUID);
    g_MyD3D->MyPlayer[iPlayerIdx]->SetEPlayerState(PS_DEAD);

    if( iPlayerIdx < MAX_PLAYER_NUM )
    {
        if ( g_kGlobalValue.m_kUserInfo.bHost )
        {
            g_pGameBoard->m_pStartingMember[iPlayerIdx].How_Die++;

            // 죽은 캐릭터가 소환한 몬스터 다 죽이자~
            g_MyD3D->MyPlayer[iPlayerIdx]->SetSummonMonsterHP( 0.0f );
        }
    }

    // 아 하드하지만.... 일단 ㅈㅅ
    if( kPacket.dwKillMePlayerID == g_kGlobalValue.m_kUserInfo.dwUID ) {
        if( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_LIME &&
            SiGCSKT()->CheckTrainedSkill( g_MyD3D->GetMyPlayer(), SID_LIME_NORMAL_KILL_BUFF ) ) {
            PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
            if( pPlayer ) {
                pPlayer->SetHP( pPlayer->GetHP() + pPlayer->GetPlayerMaxHP() * 0.05f );
            }
        } else if( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_RIN && g_MyD3D->GetMyPlayer()->MetamorphosisForm != FORM_NORMAL &&
            g_MyD3D->GetMyPlayer()->CheckSkill( SID_RIN4_FORM_PASSIVE_SKILL_1_TYPE_B ) ) {
            PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
            if( pPlayer ) {
                pPlayer->SetHP( pPlayer->GetHP() + pPlayer->GetPlayerMaxHP() * 0.05f );
            }
        }
    }

    if ( kPacket.bSelfKilling == true )
    {
        int iIndex = g_MyD3D->Get_Player( kPacket.dwUID );
        if ( iIndex < 0 || MAX_PLAYER_NUM <= iIndex )
            return;

        float fPaneltyExpGpRatio = GetPaneltyExpGpRatio( iIndex, iIndex );
        g_MyD3D->MyPlayer[iIndex]->m_fGetExpGpRatio -= fPaneltyExpGpRatio;

        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
             SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
        {            
            g_pkChatManager->AddSystemMsg( -1,
                g_pkStrLoader->GetReplacedString( STR_ID_DEATHMATCH_KILL_SELF, "S", g_MyD3D->MyPlayer[iIndex]->GetStrUserName().c_str() ),
                KGCChatManager::CHAT_TYPE_ALERT);

            if ( g_kGlobalValue.m_kUserInfo.bHost )
            {
                if ( g_pGameBoard->m_pStartingMember[iPlayerIdx].How_Kill > 0)
                {
                    g_pGameBoard->m_pStartingMember[iPlayerIdx].How_Kill--;
                }
            }
            
            
            if ( g_kGlobalValue.m_kUserInfo.bHost )
            {
                if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM || 
                    SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_TEAM )
                {
                    int iTeamScore = g_pGameBoard->GetTeamScore( g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam );
                    if ( iTeamScore > 0 )
                    {
                        g_pGameBoard->SetTeamScore( g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam, iTeamScore - 1 );
                    }
                }

            }
            

        }
    }
    else
    {
        // - MAX_PLAYER_NUM
        bool bKillerIsMonster = false;
        int iKiller = g_MyD3D->Get_Player( kPacket.dwKillMePlayerID );
        int iGhost = g_MyD3D->Get_Player( kPacket.dwUID );

        // 킬러가 플레이어인지 몬스터인지 확인
        if( iKiller > MAX_PLAYER_NUM )
            bKillerIsMonster = true;

        if( bKillerIsMonster )
        {
            // 킬러가 소환된 몬스터일때는 엄마의 인덱스를 셋팅해주도록하자
			if( g_kMonsterManager.IsSummonMonsterByPlayer(kPacket.dwKillMePlayerID) )
            {
				MONSTER* pMonster = g_kMonsterManager.GetMonster( kPacket.dwKillMePlayerID );
				if(pMonster != NULL ) {
					PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
					if( pPlayer )
					{
						iKiller = pPlayer->m_iPlayerIndex;
						bKillerIsMonster = false;
					}
				}
            }
        }

#if defined( ANGELS_EGG_REFORM )
        // 천사의 알깨기일때는 죽은 플레이어 자리에 데미지를 생성하자
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG )
        {
            if( g_kGlobalValue.m_kUserInfo.bHost )
            {
                int iDamage=0;
                
				if ( g_MyD3D->MyPlayer[iGhost]->m_kUserInfo.iTeam == TEAM_SERDIN )
				{
					switch( rand()%4 )
					{
					case 0: iDamage = DT_ANGELS_EGG_SENTRY_GUN;     break;
					case 1: iDamage = DT_ANGELS_EGG_SENTRY_GUN2;    break;
					case 2: iDamage = DT_ANGELS_EGG_TURRET;         break;
					case 3: iDamage = DT_ANGELS_EGG_TURRET2;        break;
					}

				}
				else
				{
					switch( rand()%4 )
					{
					case 0: iDamage = DT_ANGELS_EGG_SENTRY_GUN_R;     break;
					case 1: iDamage = DT_ANGELS_EGG_SENTRY_GUN2_R;    break;
					case 2: iDamage = DT_ANGELS_EGG_TURRET_R;         break;
					case 3: iDamage = DT_ANGELS_EGG_TURRET2_R;        break;
					}
				}
				
				
                KGC_PID_BROAD_ANGELS_EGG_DAMAGE kPacket;
                kPacket.iDamageEnum = iDamage;
                kPacket.iKiller = iKiller;
                kPacket.iGhost = iGhost;

                SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
                KGCPC_MEMO( "KGC_PID_BROAD_ANGELS_EGG_DAMAGE" );
            }
        }
#endif
        g_MyD3D->MyPlayer[iPlayerIdx]->SetLastKillmePlayer( iKiller );

		//	마지막 결과창에 Kill 수치 체크
		if ( g_MyD3D->IsPlayerIndex(iKiller) && 
			( g_MyD3D->MyPlayer[iKiller]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) ) &&
			( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG ) )
        {
			if ( g_kGlobalValue.m_kUserInfo.bHost )
				g_pGameBoard->m_pStartingMember[iKiller].How_Kill++;
		}

        if ( g_MyD3D->IsPlayerIndex(iKiller) && 
             ( g_MyD3D->MyPlayer[iKiller]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) ) &&
			 ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_ANGELS_EGG ) &&
             ( GC_GMC_DOTA != SiKGCRoomManager()->GetGameModeCategory() ) )
        {
            float fExp = 0.0f;
            float fAdjustRatio = 1.0f;
            // 경험치 획득 이펙투!!!
            for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                // 킬러랑 같은 팀이면 경험치 먹는거 표현!
                if ( g_MyD3D->Get_MyPlayer() == i && g_MyD3D->IsSameTeam( iKiller, i ) )
                {
                    PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
                    int iLevel = pPlayer->m_kUserInfo.GetCurrentChar().iLevel;
                    int iGhostLevel = g_MyD3D->MyPlayer[iGhost]->m_kUserInfo.GetCurrentChar().iLevel;
                    std::map< int, LevelTable >::iterator mitL = g_kGlobalValue.m_mapMonLevelTable.find( iLevel );
                    if ( mitL != g_kGlobalValue.m_mapMonLevelTable.end() )
                    {
                        // 레벨차 보정
                        if ( iLevel > iGhostLevel )
                        {
                            fAdjustRatio = 1.0f - ( 0.01f * ( iLevel - iGhostLevel ));
                        }

                        fExp = (*mitL).second.Exp * 1.0f * fAdjustRatio;
                    }
					if ( SiKGCFatigueManager()->IsFatigueSystemApplyUser() == true ) {
						float fRatio = static_cast<float>(SiKGCFatigueManager()->GetGameRatio()) / 100.f;
						fExp *= fRatio;
					}
	                   
					g_MyD3D->MyPlayer[i]->ShowExpEffect( iGhost, fExp );
                }
            }

            // 죽은놈이 나면 서버한테 날 누가 죽였는지 이르자!!!! 경험치는 저놈이 받겠지만...
            if ( iGhost == g_MyD3D->Get_MyPlayer() )
            {
                SUserInfo killerInfo = g_MyD3D->MyPlayer[iKiller]->m_kUserInfo;
                SUserInfo ghostInfo = g_MyD3D->MyPlayer[iGhost]->m_kUserInfo;
                if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
                {
                    std::vector< std::pair<DWORD,char> > vecKillTeamInfo;
                    PLAYER* pPlayer;
                    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
                    {
                        pPlayer = g_MyD3D->MyPlayer[i];
                        if ( g_MyD3D->IsSameTeam(iKiller, i) )
                        {
                            vecKillTeamInfo.push_back( std::make_pair(pPlayer->m_kUserInfo.dwUID, pPlayer->m_kUserInfo.GetCurrentChar().iCharType ) );
                        }
                    }

                    if ( killerInfo.iTeam != ghostInfo.iTeam )
                    {
                        SiKP2P()->GetInstance()->Send_PvpTagRewardExpGpReq( killerInfo.dwUID, vecKillTeamInfo, std::make_pair(ghostInfo.dwUID, ghostInfo.GetCurrentChar().iCharType) );
                    }
                }
                else
                {
                    if ( killerInfo.iTeam != ghostInfo.iTeam || SiKGCRoomManager()->GetGameMode() == GC_GM_SURVIVAL_MODE )
                    {
						
                        SiKP2P()->GetInstance()->Send_PvpRewardExpGpReq( std::make_pair(killerInfo.dwUID, killerInfo.GetCurrentChar().iCharType),
                            std::make_pair(ghostInfo.dwUID, ghostInfo.GetCurrentChar().iCharType) );
                    }
                }
            }

            float fDeathFromFallValue = 0.0f;

            if ( g_kGlobalValue.m_kUserInfo.bHost )
                g_pGameBoard->m_pStartingMember[iKiller].How_Kill++;

            if ( kPacket.bDeathFromFall == true )
            {
                fDeathFromFallValue = 0.5f;
            }
            else
            {
                fDeathFromFallValue = 1.0f;
            }
            g_MyD3D->MyPlayer[iKiller]->m_fGetExpGpRatio += kPacket.fGetExpGpRatio * fDeathFromFallValue;

            float fPaneltyExpGpRatio = GetPaneltyExpGpRatio( iKiller, iGhost);

            g_MyD3D->MyPlayer[iGhost]->m_fGetExpGpRatio -= fPaneltyExpGpRatio * fDeathFromFallValue;

            if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
                SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
            {
				if( g_kGlobalValue.m_kUserInfo.bHost )
				{
					int iTeamScore = g_pGameBoard->GetTeamScore( g_MyD3D->MyPlayer[iKiller]->m_kUserInfo.iTeam );
					g_pGameBoard->SetTeamScore( g_MyD3D->MyPlayer[iKiller]->m_kUserInfo.iTeam, iTeamScore + 1 );
				}                

                DWORD dwStr = STR_ID_DEATHMATCH_KILL_INFORM;
                if ( g_MyD3D->IsMyTeam( iKiller ) )
                {
                    dwStr = STR_ID_DEATHMATCH_KILL_INFORM2;
                }                                

                g_pkChatManager->AddSystemMsg( -1,
                    g_pkStrLoader->GetReplacedString( dwStr, "SS", g_MyD3D->MyPlayer[iKiller]->GetPlayerName(), g_MyD3D->MyPlayer[iGhost]->GetPlayerName() ) );
            }
        }
    }
    g_pkArenaBattleDlg->UpdateDlg();
    g_pkGameOverlayUI->m_pkDeathMatchScoreBoard->UpdateGameData();

#if defined( DUNGEONPLAYSTATE )
    // 던전플레이통계( Death )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
    {
        g_kGlobalValue.AddDungeonPlayState( KGCGlobal::DUNPLAY_STATE_DEATH );
    }
#endif

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
    {
        // 킬러가 몬스터이면 몬스터와 같은 팀원들에게 APoint를 지급해주는 코드를 집어넣자!!
        int iMyPlayerID = g_MyD3D->Get_MyPlayer();
        int iKiller = g_MyD3D->Get_Player( kPacket.dwKillMePlayerID );
        if( kPacket.bDeathFromMonster && 
            false == g_MyD3D->IsPlayerIndex( iKiller ) && g_kMonsterManager.IsMonsterIndex( kPacket.dwKillMePlayerID ) )
        {
            iKiller = kPacket.dwKillMePlayerID;
            if( g_kGlobalValue.m_kUserInfo.bHost )
            {
                for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
                {
                    if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
                        continue;
					MONSTER* pMonster = g_kMonsterManager.GetMonster( iKiller );
					if(pMonster == NULL ) continue;

                    if( pMonster->m_iTeam == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
                    {
                        g_MyD3D->MyPlayer[ iLoop ]->Set_D_Point( g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point() + g_kGlobalValue.GetDotaKillerMonster_D_Point() );
                    }
                }
            }

            // Killer와 같은팀원이면 킬러의 획득한 DPoint를 표현해주자!!
            if( g_MyD3D->IsPlayerIndex(iMyPlayerID) && g_MyD3D->IsSameTeam_PlayerAndMonster(iMyPlayerID, iKiller) )
            {
                g_MyD3D->MyPlayer[ iMyPlayerID ]->AddParticleUINumber( g_kGlobalValue.GetDotaKillerMonster_D_Point(), D3DXVECTOR2(-1.05f, -0.05f), true );
            }
        }
        else if( g_MyD3D->IsPlayerIndex( iKiller ) && 
                 g_MyD3D->MyPlayer[ iKiller ]->Get_D_Point() < g_MyD3D->MyPlayer[ iKiller ]->Get_MAX_D_Point() )
        {
            // 방장이 D-Point처리하자( 처리하고 브로드캐스팅으로 쏴주자 )
            if ( g_kGlobalValue.m_kUserInfo.bHost && false == kPacket.bDeathFromMonster )
            {        
                g_MyD3D->MyPlayer[ iKiller ]->Set_D_Point( g_MyD3D->MyPlayer[ iKiller ]->Get_D_Point() + g_kGlobalValue.GetDotaPlayer_D_Point() );
                g_pGameBoard->m_pStartingMember[iKiller].How_Kill++;
            }

            // Killer와 같은팀원이면 킬러의 획득한 DPoint를 표현해주자!!
            int iMyPlayerID = g_MyD3D->Get_MyPlayer();
            if( g_MyD3D->IsPlayerIndex(iMyPlayerID) && g_MyD3D->IsSameTeam(iMyPlayerID, iKiller) )
            {
                g_MyD3D->MyPlayer[ iKiller ]->AddParticleUINumber( g_kGlobalValue.GetDotaPlayer_D_Point(), D3DXVECTOR2(-1.05f, -0.05f), true );
            }
        }

        // 방장은 A포인터 정보를 전체에게 알려주자!!
        if( g_kGlobalValue.m_kUserInfo.bHost )
        {
            int iDPoint[MAX_PLAYER_NUM]={0,};
            int iKill[MAX_PLAYER_NUM]={0,};
            int iDeath[MAX_PLAYER_NUM]={0,};
            for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
            {
                if( g_MyD3D->MyPlayer[ iLoop ]->IsLive() )
                {
                    iDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
                    iKill[iLoop] = g_pGameBoard->GetHowKill( iLoop );
                    iDeath[iLoop] = g_pGameBoard->GetHowDie( iLoop );
            }
            }
            KGC_PID_BROAD_DOTA_POINT kCustom( GC_PID_BROAD_DOTA_POINT, g_MyD3D->MyPlayer[ iPlayerIdx ]->m_kUserInfo.dwUID, iDPoint, iKill, iDeath, true );
            SendP2PPacket( &kCustom, sizeof(kCustom), KNC_BROADCAST_NOT_ME, _RELIABLE );
            KGCPC_MEMO( "GC_PID_BROAD_DOTA_POINT" );
        }

        // 내가 죽었다면 아레나 쿨타임을 셋팅해주자.
        if( g_MyD3D->Get_MyPlayer() == iPlayerIdx )
        {
            SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
            KGCGameModeInterface* pMode = g_pStateGame->GetCurGameModePtr();
            if( pMode && pDotaTemplate )
            {
                pMode->SetArenaCoolTime( pDotaTemplate->iArenaCoolTime );
            }
            g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_FIRST_DEATH );
        }

        // 내가 죽였다면 APOINT에 대한 가이드를 출력하자.
        if( g_MyD3D->Get_MyPlayer() == iKiller )
        {
            g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_FIRST_KILL );
            g_pkUIScene->m_pkGameOverlayUI->AddGuideMsg( SDotaGameGuide::DOTA_GUIDE_APOINT );
        }

        // 제압메시지를 출력한다. : @1님께서 @2님을 제압했습니다.
        if( g_MyD3D->IsPlayerIndex( iKiller ) && g_MyD3D->IsPlayerIndex(iPlayerIdx) &&
            g_MyD3D->IsPlayerIndex( iMyPlayerID ) && g_MyD3D->MyPlayer[iMyPlayerID]->IsLocalPlayer() )
        {
            SDotaTemplate* pSDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
            if( pSDotaTemplate )
            {
                KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                KInGameMessageAttribute kMessage;
                kMessage.iStringID = pSDotaTemplate->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_PLAYER_DEATH].first;
                kMessage.iShowFrame = pSDotaTemplate->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_PLAYER_DEATH].second;
                kMessage.strValue1 = const_cast<WCHAR*>(g_MyD3D->MyPlayer[iKiller]->GetStrUserName().c_str());
                kMessage.strValue2 = const_cast<WCHAR*>(g_MyD3D->MyPlayer[iPlayerIdx]->GetStrUserName().c_str());
                pMessage->SetMessage( &kMessage );
            }
        }

        // 점령전에서 죽은 이유를 통계정보로 남기자.
        if( g_MyD3D->IsPlayerIndex(iPlayerIdx) )
        {
            SDotaStats &sDotaStats = g_MyD3D->MyPlayer[iPlayerIdx]->GetDotaStats();

            // 사망한 존 정보
            std::map<DWORD,int>::iterator mapCntIter = sDotaStats.m_mapZoneDeathCount.find( g_MyD3D->MyPlayer[iPlayerIdx]->GetDotaStage() );
            if( mapCntIter != sDotaStats.m_mapZoneDeathCount.end() )
                ++mapCntIter->second;
            else
                sDotaStats.m_mapZoneDeathCount[ g_MyD3D->MyPlayer[iPlayerIdx]->GetDotaStage() ] = 1;

            // 사망의 원인
            DWORD dwKiller = static_cast<DWORD>(-1);
            DWORD dwType = kPacket.bDeathFromMonster ? 0 : 1;
			MONSTER* pMonster = g_kMonsterManager.GetMonster( iKiller );

            if( kPacket.bDeathFromMonster && pMonster != NULL )
                dwKiller = pMonster->GetMonsterType();
            else if( g_MyD3D->IsPlayerIndex(iKiller) )
                dwKiller = g_MyD3D->MyPlayer[iKiller]->GetCurrentChar().iCharType;

            std::map<std::pair<DWORD, DWORD>, int >::iterator mapTypeIter = sDotaStats.m_mapDeathType.find( std::make_pair(dwType, iKiller) );
            if( mapTypeIter != sDotaStats.m_mapDeathType.end() )
                ++mapTypeIter->second;
            else
                sDotaStats.m_mapDeathType[ std::make_pair(dwType, dwKiller) ] = 1;
        }
    }

    SendP2PPacket( GC_PID_GAME_BOARD_UPDATE );
}

void   ON_GC_PID_BROAD_SANTA_EVENT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SANTA_EVENT kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );
    KGCStateGame* pState = (KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME );    
    pState->GetCurGameModePtr()->m_kSanta.Create( D3DXVECTOR2( kPacket.fStartPosX, kPacket.fStartPosY) , D3DXVECTOR2( kPacket.fEndPosX, kPacket.fEndPosY ) );
}

void   ON_GC_PID_COMPRESSED_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    static BYTE s_pUnCompressBuffer[MAX_COMPRESS_BUFFER*10] = {0,};
    static UINT s_uiUnCompressBufferSize = sizeof(BYTE)*MAX_COMPRESS_BUFFER*10;
    static ULONG s_ulunCompressSize = 0;

    KGC_PID_COMPRESSED_PACKET* pPacket = (KGC_PID_COMPRESSED_PACKET*)pRecvData_;
    memset( s_pUnCompressBuffer, 0, s_uiUnCompressBufferSize );
    s_ulunCompressSize = MAX_COMPRESS_BUFFER*10;
    uncompress( s_pUnCompressBuffer, &s_ulunCompressSize, pPacket->pCompressBuffer, dwRecvSize_ - sizeof(GCPID) );
    KGC_PACKET_HEADER* pPacketHeader = (KGC_PACKET_HEADER*)s_pUnCompressBuffer;

	KGCPC_VALUE( "Cmp Rcv", dwRecvSize_ );
    P2PPacketProc( pPacketHeader->Type, s_pUnCompressBuffer, dwUID_, (DWORD)s_ulunCompressSize );
}

void   ON_GC_PID_USEPETSKILL( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_USEPETSKILL* pkPacket = (KGC_PID_USEPETSKILL*)pRecvData_;

    if( (int)pkPacket->cPlayerIndex < 0 || (int)pkPacket->cPlayerIndex > MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO - 1 )
        return;

    if( pkPacket->bUsePetSkill )
    {
        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->vPos.x = pkPacket->PetSkill.fX;
        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->vPos.y = pkPacket->PetSkill.fY;
        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->bIsRight = pkPacket->PetSkill.bIsRight;

        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->usMotion = (int)pkPacket->cPetMotion;//EGCPETMOTION::ATTACK;
        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->cFrame = 0;

        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->m_fCurrentMP = 0.0f;

        // 리플레이에 펫 스킬 사용하는것 남기기
        g_MyD3D->MyReplay->Set_UsePetSkill( pkPacket, pkPacket->cPlayerIndex );
        g_MyD3D->m_akPet[ (int)pkPacket->cPlayerIndex ]->EnableGlyphEffect();
    }
}

void   ON_GC_PID_POISON_BREATH( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
	KGC_PID_POISON_BREATH* pkPacket = (KGC_PID_POISON_BREATH*)pRecvData_;

	if ( pkPacket->iBreathIndex < 0 )
	{
		return;
	}

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;

		if( pMonster->m_iMonsterType == MON_LENASIEN || 
			pMonster->m_iMonsterType == MON_LENASIEN_BOSS || 
			pMonster->m_iMonsterType == MON_VS_LENASIEN )
		{
			for( int j = 0 ; j < 10 ; j++ )
				g_MyD3D->m_pDamageManager->MonsterAddWithLocate( DT_RENASIEN_GAS, mit->first, pkPacket->fPosX, pkPacket->fPosY );	
			break;
		}
	}

}
//====================================================================================
// 2007.02.22 : Asirion
// 변신 싱크 맞추는 모듈 추가.
// 현재는 버그 수정차원에서 간단하게 처리하고 추후에 수정.
void   ON_GC_PID_TRANSFORM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_TRANSFORM* pkPacket = (KGC_PID_TRANSFORM*)pRecvData_;

    if( (int)pkPacket->ucPlayerIndex < 0 || (int)pkPacket->ucPlayerIndex > MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO - 1 )
        return;

    if ( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_kUserInfo.bLive == false )
        return;

    if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
    {
        return;
    }


    switch( pkPacket->cTransformType )
    {
        case FORM_NORMAL:
        {
            g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->ProcessMetamorphosisNormalForm( true );
            break;
        }
        case FORM_WOLF:
        case FORM_NEPHILIM:
        case FORM_MDAN:
        case FORM_RIN_GODDESS:
        case FORM_RIN_EVIL:
        {
            g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->ProcessMetamorphosisFormChange( pkPacket->cTransformType );
            break;
        }
        default:
        {
            //====================================================================================
            // 2007.02.22 : Asirion
            // 추후에 이리로 들어오면 에러 리포팅 하도록 하자.
            break;
        }
    }
}

void   ON_GC_PID_SWAPWEAPON( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
    {
        return;
    }
    
    KGC_PID_SWAPWEAPON* pkPacket = (KGC_PID_SWAPWEAPON*)pRecvData_;

    if( (int)pkPacket->ucPlayerIndex < 0 || (int)pkPacket->ucPlayerIndex > MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO - 1 )
        return;

    if ( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_kUserInfo.bLive == false )
        return;

    if (g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_iWeaponType != (int)pkPacket->ucWeaponType)
    {
        g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->ProcessSwapWeapon( pkPacket->bNotChangeMotion );
    }    
}

void   ON_GC_PID_ENTER_GAME( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_ENTER_GAME* pkPacket = (KGC_PID_ENTER_GAME*)pRecvData_;

    if( (int)pkPacket->ucPlayerIndex < 0 || (int)pkPacket->ucPlayerIndex > MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO - 1 )
        return;

    if ( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_kUserInfo.bLive )
    {
        g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_kUserInfo.nUserState = GC_RUS_PLAYING;
        if ( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_pObject )
        {
            g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->m_pObject->SetRender( true );
        }
        
        g_pGameBoard->OnEnterPlayer( pkPacket->ucPlayerIndex );
        
        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
             SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
        {
            KGCDeathMatch* pGameMode = (KGCDeathMatch*)g_pStateGame->GetCurGameModePtr();
            pGameMode->UpdateStatBalace();
            g_pkGameOverlayUI->m_pkDeathMatchScoreBoard->UpdateGameData();            
            g_pkChatManager->AddSystemMsg( pkPacket->ucPlayerIndex, g_pkStrLoader->GetString( STR_ID_GAMEMSG_REPORT_JOIN ) );            
        }        

        g_kTransform->ucPlayerIndex = (UCHAR)g_MyD3D->Get_MyPlayer();
        g_kTransform->cTransformType = (CHAR)g_MyD3D->GetMyPlayer()->MetamorphosisForm;

        if( g_MyD3D->GetMyPlayer()->Extra_Char_Num == CID_RYAN4 )
        {
            g_kSwapWeapon->ucPlayerIndex = (UCHAR)g_MyD3D->Get_MyPlayer();
            g_kSwapWeapon->ucWeaponType = g_MyD3D->GetMyPlayer()->m_iWeaponType;
        }

		if( g_MyD3D->GetMyPlayer()->Extra_Char_Num == CID_AMY3 )
		{
			g_kSwapWeapon->ucPlayerIndex = (UCHAR)g_MyD3D->Get_MyPlayer();
			g_kSwapWeapon->ucWeaponType = g_MyD3D->GetMyPlayer()->m_iWeaponType;
		}


        if ( g_MyD3D->IsMyTeam(pkPacket->ucPlayerIndex) )
        {
            if( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->GetMetaForm(FORM_NORMAL) )
                g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->GetMetaForm(FORM_NORMAL)->SetOutlineColor( EDGE_COLOR_FRIEND );
        }
        else
        {
            if( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->GetMetaForm(FORM_NORMAL) )
                g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->GetMetaForm(FORM_NORMAL)->SetOutlineColor( EDGE_COLOR_ENEMY );
        }
        if( g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->GetMetaForm(FORM_NORMAL) )
            g_MyD3D->MyPlayer[pkPacket->ucPlayerIndex]->GetMetaForm(FORM_NORMAL)->SetShellMatrixScale( DEFAULT_PLAYER_SCALE_SIZE );

    }

    g_MyD3D->GetMyPlayer()->InitBonusBuff( true );
    if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        SendP2PPacket( GC_PID_GAME_BOARD_UPDATE );
    }
}
    
void ON_GC_PID_COUPLE_EVENT_CREDIT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
	KGC_PID_COUPLE_EVENT_CREDIT* pkPacket = (KGC_PID_COUPLE_EVENT_CREDIT*)pRecvData_;
}

// 2007/3/16. iridology. Tag Character
void   ON_GC_PID_TAG_CHARACTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_TAG_CHARACTER* pkPacket = (KGC_PID_TAG_CHARACTER*)pRecvData_;
    
    if( (int)pkPacket->cPlayerIndex < 0 || (int)pkPacket->cPlayerIndex > MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO - 1 )
        return;

    if( g_MyD3D->MyPlayer[pkPacket->cPlayerIndex]->m_kUserInfo.bLive == false )
        return;

    g_MyD3D->MyPlayer[pkPacket->cPlayerIndex]->TagCharacter( pkPacket->bArena );

    // Replay에 남기기
    g_MyD3D->MyReplay->Set_TagCharacter( pkPacket, pkPacket->cPlayerIndex );
}

void   ON_GC_PID_EFFECT_ITEM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_EFFECT_ITEM* pkPacket = (KGC_PID_EFFECT_ITEM*)pRecvData_;
    int ItemUse=-1;
    bool bShowChat = true;
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->Eff_Item( pkPacket->Eff_Item[i] );
        if ( pkPacket->Eff_Item[i].cWhat > -1 ) 
        {
            ItemUse = (int)pkPacket->Eff_Item[i].cWhat;
            bShowChat = pkPacket->Eff_Item[i].bShowChat;
        }
    }
    
    if ( ItemUse <= -1 )
        return;
    
    int Who = g_MyD3D->Get_Player( dwUID_ );
    if ( Who < MAX_PLAYER_NUM )
    {
        if( bShowChat && g_MyD3D->IsMyTeam(Who) && g_pkChatManager != NULL )
        {
			g_pkChatManager->AddSystemMsg( -1,
				g_pkStrLoader->GetReplacedString(STR_ID_GAMEMSG_ITEM_USE1, "ss", 
                L"#c" + g_MyD3D->MyPlayer[Who]->GetStrUserColor() + g_MyD3D->MyPlayer[Who]->GetStrUserName().c_str() + L"#cx",
				g_MyD3D->MyItms->GetItemDesc( ItemUse ) ) 
				);
        }
    }

    //리플레이 저장용.
    g_MyD3D->MyReplay->Set_ItemData(pkPacket,Who);

}

void   ON_GC_PID_TRANSNPC( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_TRANSNPC* pkPacket = (KGC_PID_TRANSNPC*)pRecvData_;
    // NPC에 대한 정보가 실려왔다면....
    int Index;
    // 플레이어의 인덱스가 유효한지 확인한다.
    if ( ( Index = g_MyD3D->Get_Player( dwUID_ ) ) >= MAX_PLAYER_NUM ) 
    {
        return;
    }
    switch ( pkPacket->TransNPC.What )
    {
        case 1: // 아이템
        {
            // 아이템에 대한 정보(Created_Itm>-1)이면...
            // 호스트가 보낸 정보가 아니라도 괜찮다.
            if ( pkPacket->TransNPC.Action < GC_GAME_ITEM_NUM )
            {
                if( pkPacket->TransNPC.Action == GC_GAME_ITEM_GP &&
                    g_MyD3D->MyPlayer[Index]->Item_Score > 0 ) 
                    g_MyD3D->MyPlayer[Index]->Item_Score--;

                if ( pkPacket->TransNPC.Action == GC_GAME_ITEM_MATCH_GP )
                {
                    //나오는 GP공식 : 드랍 GP 수식 : 1+(n+m)-(n-m) ( n:죽인 유저Lv / m:죽은 유저 Lv )
                    int iGP = 1 + 2*(int)Exp_2_Level(
                        GetCharacterExp( g_pGameBoard->m_pStartingMember[Index].m_mapStartExp, 
                                            (int)g_MyD3D->MyPlayer[Index]->GetCurrentChar().iCharType ) );
                    //아이템 튀어 나오는 방향 결정.죽인 사람이 죽은 사람의 오른쪽에 있었나?
                    unsigned char  ucKiller = g_MyD3D->MyPlayer[Index]->GetLastKillmePlayer();
                    
                    //printf( "cKiller is : %d\n" , (int)ucKiller );
                    //printf( "cDeadBody is : %d\n" , Index );
                    if (ucKiller >= 0 && ucKiller < MAX_PLAYER_NUM)
                    {
                        float xspeed =  (g_MyD3D->MyPlayer[Index]->vPos.x < g_MyD3D->MyPlayer[ucKiller]->vPos.x)? 0.015f:-0.015f;
                        int nIndex = (int)pkPacket->TransNPC.HP;
                        g_MyD3D->MyItms->AddMatchGP( iGP, D3DXVECTOR2(pkPacket->TransNPC.x,pkPacket->TransNPC.y), true, 550, xspeed, 0.04f, ucKiller, nIndex );
                    }
                }
                else
                {
                    g_MyD3D->MyItms->Add( pkPacket->TransNPC.Action, D3DXVECTOR2(pkPacket->TransNPC.x,pkPacket->TransNPC.y), true );
                }
            }
            break;
        }
        case BEGIN_NPC+2:
        {
            STAGE* pStg = g_MyD3D->MyStg;
            switch ( pStg->GetStage() )
            {
                case GC_GS_FOGOTTEN_CITY:
                {
                    pStg->Event = 0;
                    break;
                }
                case GC_GS_BABEL_OF_X_MAS:
                {
                    pStg->m_fScrollY  = pkPacket->TransNPC.y;
                    pStg->m_bUpScroll = pkPacket->TransNPC.IsRight;
                    break;
                }
                case GC_GS_TEMPLE_OF_FIRE:
                {
                    int iIdx = static_cast<int>( pkPacket->TransNPC.x );
                    if ( iIdx >= 0 && iIdx < (int)( pStg->m_vecPushHand.size() ) )
                    {
                        pStg->m_vecPushHand[iIdx].bPush = true;
                        pStg->m_vecPushHand[iIdx].iHole = pkPacket->TransNPC.Action;
                        pStg->m_vecPushHand[iIdx].iAlpha = static_cast<int>( pkPacket->TransNPC.HP );
                        pStg->m_vecPushHand[iIdx].bInc = pkPacket->TransNPC.IsRight;
                        pStg->RemoveDuplicateHole();
                    }
                    break;
                }
            }
        }
        default:
        {
            // Stream Sync Packet
            if( pkPacket->TransNPC.What >= 3+BEGIN_NPC )
            {
                STAGE* pStg = g_MyD3D->MyStg;
                pStg->InitializeStreamEvent( pkPacket->TransNPC.What - (3+BEGIN_NPC) );
            }
            else 
            {
                if( 1 < pkPacket->TransNPC.What && pkPacket->TransNPC.What < BEGIN_NPC+2 )
                {
                    int Index = pkPacket->TransNPC.What - 2;

                    // 보내준 패킷이 호스트가 보내준 것이라면...
                    if ( !g_kGlobalValue.m_kUserInfo.bHost && g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) // 이 경우 Extra Information을 받는다.
                    {
                        if ( pkPacket->TransNPC.Action > 6 && pkPacket->TransNPC.Action < 77 )
                        {
                            // 몬스터를 생성한다.
                            //g_kMonsterManager.m_mapMonster[Index]->Wait_Room( pkPacket->TransNPC.Action - 7, pkPacket->TransNPC.x, pkPacket->TransNPC.y, 
                            //                            pkPacket->TransNPC.IsRight, SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_MONSTER_HUNT );
							MONSTER* pMonster = g_kMonsterManager.GetMonster( Index );
							if ( pMonster ) {
								pMonster->m_fHP = 1.0f;
								pMonster->m_bLive = true;
							}


                            //if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
                            //    g_kMonsterManager.m_mapMonster[Index]->UpateMonsterPoint();
                            //else
                            //    g_kMonsterManager.m_mapMonster[Index]->Calculate_Point();
                        }
                        else
                        {
                            // 몬스터를 출연시키기 위한 카운트를 받아드린다.
                            if ( pkPacket->TransNPC.Action < -50 && pkPacket->TransNPC.Action > -56 )
                            {
                                g_MyD3D->Story = 20499 + (short)pkPacket->TransNPC.Action * 100;

                                break;
                            }

                            bool Bounce = false;
                            if ( pkPacket->TransNPC.Action == 1 ) // 1번째 동작인 경우
                            {
                                //if ( g_kMonsterManager.m_mapMonster[Index]->Now_Monster == 2 )
                                //{
                                //    g_kMonsterManager.m_mapMonster[Index]->Set_Correct_Bounce( pkPacket->TransNPC.x, pkPacket->TransNPC.y );
                                //    Bounce = true;
                                //}
                            }
                            else if ( pkPacket->TransNPC.Action == 3 ) // 3번째 동작인 경우
                            {
                                //g_kMonsterManager.m_mapMonster[Index]->Set_Correct_Bounce( pkPacket->TransNPC.x, pkPacket->TransNPC.y );
                                Bounce = true;
                            }
                            else if ( pkPacket->TransNPC.Action == 77 ) // 초필살기인 동작인 경우
                            {
                                pkPacket->TransNPC.Action = 8;
                            }
                            //else if ( pkPacket->TransNPC.Action == MON_ACTION_JUMP )
                            //{
                            //    g_kMonsterManager.m_mapMonster[Index]->Set_Correct_Bounce( pkPacket->TransNPC.x, pkPacket->TransNPC.y );
                            //    Bounce = true;
                            //    pkPacket->TransNPC.Action = MON_TROLL_MOTION_JUMP;
                            //}

                            //if ( ( g_kMonsterManager.m_mapMonster[Index]->Now_Monster == 10 && pkPacket->TransNPC.Action == 1 ) &&
                            //    ( pkPacket->TransNPC.IsRight == false ) )
                            //{
                            //    pkPacket->TransNPC.Action += 5;
                            //}

                            if ( Bounce )
                            {
                                //g_kMonsterManager.m_mapMonster[Index]->IsRight_Input_Direct( pkPacket->TransNPC.IsRight );
                            }
                            else
                            {
                                //g_kMonsterManager.m_mapMonster[Index]->IsRight_Input_Direct( pkPacket->TransNPC.IsRight, pkPacket->TransNPC.x, pkPacket->TransNPC.y );
                            }

                            //g_kMonsterManager.m_mapMonster[Index]->MonsterMotionInput( pkPacket->TransNPC.Action, true );

                            // 괴물 내구력에 대한 정보
							MONSTER* pMonster = g_kMonsterManager.GetMonster( Index );
							if ( pMonster ) {
							   pMonster->m_fHP = pkPacket->TransNPC.HP;
							}
						}
                    }
                }
            }
            break;
        }
    } // switch ( pkPacket->TransNPC.What )
}
void   ON_GC_PID_GAME_BOARD_UPDATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_GAME_BOARD_UPDATE kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );
    g_pGameBoard->SetMonsterNum( kPacket.m_mapMonNum );
    g_pGameBoard->SetMonKillNum( kPacket.m_mapMonKillNum );
    g_pGameBoard->SetGameScore( kPacket.iMonHuntPoint );

    for ( int i = 0 ; i< MAX_PLAYER_NUM ; i++ )
    {
        g_pGameBoard->m_pStartingMember[i].How_Kill = kPacket.aKillNum[i];
        g_pGameBoard->m_pStartingMember[i].How_Die = kPacket.aDeathNum[i];
    }
    
    for ( int i = 0 ; i< TEAM_NUM ; i++ )
    {
        g_pGameBoard->SetTeamScore( i, kPacket.aTeamScore[i] );        
    }

    g_pkGameOverlayUI->m_pkDeathMatchScoreBoard->UpdateGameData();

    g_MyD3D->MyReplay->Set_GameBoard( &kPacket );
}

void ON_GC_PID_CHAT_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_CHAT_PACKET kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );
    if ( g_pkChatManager != NULL )
    {
        /*g_pkChatManager->ChatMsgProc kPacket.strNickName, kPacket.strChat );*/
        if ( true == g_kGlobalValue.IsBlockUser( kPacket.strNickName ) || 
             true == SiKFriendShip()->IsBlockFriend( kPacket.strNickName ) || 
            kPacket.strChat[0] == L'/' )
            return;

		EGCUserLevel eGCUserLevel = USERLEVEL_NOMAL;

        std::wstring strNickColorTemp = kPacket.strNickColor;
        std::wstring tempMsgNick = kPacket.strNickName;

		int iIndex = g_MyD3D->Get_Player( kPacket.dwUID );
		if ( 0 <= iIndex && iIndex <= MAX_PLAYER_NUM - 1  )
		{
			eGCUserLevel = g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.eGCUserLevel;
		}

        if (strNickColorTemp.size() < 6)
            strNickColorTemp = L"FFFFFF";

		if( KGCChatManager::CHAT_CMD_TEAM == (KGCChatManager::EChatCommand)kPacket.ecmd  ) //zstaiji - 팀채팅일 경우
		{
			g_pkChatManager->ChatMsgProc(strNickColorTemp, tempMsgNick, kPacket.strChat, KChatData::MSG_TEAM_CHAT, kPacket.dwColor );
		}
		else if( USERLEVEL_ADMIN == eGCUserLevel)
		{
			g_pkChatManager->ChatMsgProc(strNickColorTemp, tempMsgNick, kPacket.strChat, KChatData::MSG_ADMIN_COMMON, kPacket.dwColor );
		}
		else
		{
			if( kPacket.dwColor != 0xFFFFFFFF )
				g_pkChatManager->ChatMsgProc(strNickColorTemp, tempMsgNick, kPacket.strChat, KChatData::MSG_COLOR_CHAT, kPacket.dwColor );
			else
				g_pkChatManager->ChatMsgProc(strNickColorTemp, tempMsgNick, kPacket.strChat, KChatData::MSG_COMMON, kPacket.dwColor );
		}
    }
}

void   ON_GC_PID_DIALOGUE_PROGRESS( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_DIALOGUE_PROGRESS *Custom = (KGC_PID_DIALOGUE_PROGRESS*)pRecvData_;
    CTriggerCondition *pTriggerCondition = g_kEveryTriggerManager.FindTriggerConditonByID( Custom->m_sTG_ID );
    if( pTriggerCondition )
    {
        if( CTriggerCondition::TG_DIALOGUE == pTriggerCondition->enType )
        {
            CTGDialogue *pDialogue = static_cast<CTGDialogue*>(pTriggerCondition);

            if( !g_pkQuestManager->IsEndDialogList( pDialogue->m_szDialgue ) )
            {
                if( -1 == Custom->m_sIndex )
                {
					
					g_pkQuestManager->SetShowDialogue( Custom->bShow );
					g_pkQuestManager->AddEndDialogList( pDialogue->m_szDialgue );					
                    return;
                }
                // 처음으로 드라마가 시작되었거나 현재 내 드라마와 방장의 드라마가 다르면 컷을 넘겨주자!!
                else if( 0 == Custom->m_sIndex || Custom->m_sIndex != g_pkQuestManager->GetCurDialogeIndex() )
                {
                    g_pkQuestManager->SetShowDialogue( Custom->bShow );
                    g_pkQuestManager->SetDialogueList( pDialogue->m_szDialgue );
                    g_pkQuestManager->SetDialogue( Custom->m_sIndex );
                    g_pkQuestManager->m_iDialogueTG_ID  = Custom->m_sTG_ID;
                }

                if(Custom->bSkipping==true)
                {
                    g_pkQuestManager->SkipDialogue();
                }
            }
        }
    }
}

void   ON_GC_PID_TG_DEFAULT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_TG_DEFAULT *Custom = (KGC_PID_TG_DEFAULT*)pRecvData_;

#if !defined(__PATH__)
	FILE *fo = NULL;
	std::wstringstream strText;
	strText << "TrigerInfo = " << Custom->m_sTG_ID << "\n";
	fo = fopen("ReceivedTrigerInfo.txt","a");
	fwprintf(fo,L"%s",strText.str().c_str());
	fclose(fo);
#endif
    CTriggerCondition *pFindCondition = g_kEveryTriggerManager.FindActiveTriggerConditonByID(Custom->m_sTG_ID);

    if( pFindCondition && g_sTG_MaxQuestID == Custom->m_sMaxTG_ID )
    {
        pFindCondition->m_bCheckByHost = true;
        // ㅅㅂ
        if ( CTriggerCondition::TG_MON_GEN == Custom->m_ucTG_Type )
        {
            CTGMonsterGenerater* pMonGen = (CTGMonsterGenerater*)pFindCondition;
            pMonGen->SetRelease( Custom->m_iValue == 0 ? true : false );
        }
        if ( CTriggerCondition::TG_NEXT_STAGE == Custom->m_ucTG_Type )
        {
            CTGNextStage* pNextStage = (CTGNextStage*)pFindCondition;
            pNextStage->SetDecisionStage( Custom->m_iValue );
        }

        pFindCondition->FrameMove();
    }
    else
    {
        START_LOG( clog, "Danger!!!!! Trigger not match");        

        if( !g_pkUIScene->IsMsgBoxModal() )
        {
            int iCurPlayerNum = 0;
            for( int i = 0; i < MAX_PLAYER_NUM; i++ )
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
                    iCurPlayerNum++;
            }

            int iAvgPing = 0;
            int iAvgPingUR = 0;
#if defined(USE_ONLY_TCP_RELAY) || defined(USE_P2P_OR_TCP_RELAY)
            iAvgPing = g_kGlobalValue.GetTRAvgPing();
#else
            iAvgPing = KSingleton<KncP2P>::GetInstance()->GetAvgPingTime( false );
            iAvgPingUR = KSingleton<KncP2P>::GetInstance()->GetAvgPingTime( true );
#endif

            bool bIsNull = false;

            if( pFindCondition == NULL )
                bIsNull = true;

            TileMap sActiveTileMap = g_kEveryTriggerManager.GetActiveTileMap();
            TileMap sNextTileMap = g_kEveryTriggerManager.GetActiveTileMap();

            std::wstringstream strText;
            strText << " DungeonID : "<< SiKGCRoomManager()->GetGameMode() << ", CurrentStage : " << g_pkQuestManager->m_iCurStage
                << ", Current Dungeon Level : " << SiKGCRoomManager()->GetDungeonLevel()
                << ", g_sTG_MaxQuestID : "      << g_sTG_MaxQuestID <<", Receive Value m_sMaxTG_ID : " << Custom->m_sMaxTG_ID
                << ", Receive Value m_iTG_ID : "<< Custom->m_sTG_ID 
                << ", Average Ping : " << iAvgPing << ", UR Average Ping : " << iAvgPingUR
                << ", pFindCondition is NULL : "<< bIsNull 
                << ", PlayerNum : "             << iCurPlayerNum    << ", IsHost : " << g_kGlobalValue.m_kUserInfo.bHost
                << ", ActiveTileMap [Name]" << sActiveTileMap.strName.c_str() << " [Stage]" << sActiveTileMap.iStage
                << " [Row]" << sActiveTileMap.iRow << " [Col]" << sActiveTileMap.iCol
                << ", NextTileMap [Name]" << sNextTileMap.strName.c_str() << " [Stage]" << sNextTileMap.iStage
                << " [Row]" << sNextTileMap.iRow << " [Col]" << sNextTileMap.iCol;
				
			//SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_NOT_EQUAL_QUEST_INFO, strText.str() );

            g_kEveryTriggerManager.SetNotMatchTrigger( true);
			

			if(pFindCondition == NULL)
				KP2P::GetInstance()->Send_ClientFailedGameStartNot(5, 0, iCurPlayerNum, g_pkQuestManager->m_iCurStage, 1,static_cast<DWORD>(Custom->m_sTG_ID));
			else if(g_sTG_MaxQuestID != Custom->m_sMaxTG_ID){
				DWORD Reason = g_sTG_MaxQuestID;
				Reason += 0x10000000;
				Reason += Custom->m_sMaxTG_ID;
				KP2P::GetInstance()->Send_ClientFailedGameStartNot(5, 0, iCurPlayerNum, g_pkQuestManager->m_iCurStage, 2,Reason);
			}

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME,
             g_pkStrLoader->GetString( STR_ID_NOT_EQUAL_QUEST_INFO_1 ),
             g_pkStrLoader->GetString( STR_ID_NOT_EQUAL_QUEST_INFO_2 ),
             KGCUIScene::GC_MBOX_USE_END_GAME, 0, 0, 0, 1 );

         return;
        }
    }
}

void   ON_GC_PID_DROPITEM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_DROPITEM kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );
    KGC_PID_DROPITEM* Custom = &kPacket;
    SiKGCDropItemManager()->DropItem( Custom->m_vecDropUID, D3DXVECTOR2( Custom->m_fX, Custom->m_fY ) );
}

void   ON_GC_PID_QUEST_REAULT_ITEM_DICE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_QUEST_REAULT_ITEM_DICE *Custom = (KGC_PID_QUEST_REAULT_ITEM_DICE*)pRecvData_;
    if( !Custom->bBlank )
        g_pkQuestManager->m_mapRouletteList[Custom->dwItemUID].push_back(Custom->dwPlayerUID);

    // 플레이어가 룰렛 정보를 다~ 보냈나 확인~!
    std::vector<DWORD>::iterator vit = std::find( g_MyD3D->m_pkQuestGameMgr->m_vecRouletteWaitList.begin()
                                                , g_MyD3D->m_pkQuestGameMgr->m_vecRouletteWaitList.end(), dwUID_ );
    if ( vit != g_MyD3D->m_pkQuestGameMgr->m_vecRouletteWaitList.end() )
        g_MyD3D->m_pkQuestGameMgr->m_vecRouletteWaitList.erase( vit );
}

void   ON_GC_PID_QUEST_STAGE_RESULT_SCORE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_QUEST_STAGE_RESULT_SCORE *Custom = (KGC_PID_QUEST_STAGE_RESULT_SCORE*)pRecvData_;

    if( 0 > Custom->ucPlayerIndex || Custom->ucPlayerIndex >= MAX_PLAYER_NUM ) return;

    SQuestPoint temp;
    temp.AerialAttack       = Custom->usAerialAttack;
    temp.Attack             = Custom->usAttack;
    temp.Attacked           = Custom->usAttacked;
    temp.BackAttack         = Custom->usBackAttack;
    temp.CounterAttack      = Custom->usCounterAttack;
    temp.CriticalAttack     = Custom->usCriticalAttack;
    temp.DoubleAttack       = Custom->usDoubleAttack;
    temp.iSpecial1          = Custom->usSpecial1;
    temp.iSpecial2          = Custom->usSpecial2;
    temp.iSpecial3          = Custom->usSpecial3;
    g_MyD3D->m_pkQuestGameMgr->SaveToQuestPointList(Custom->ucStage, Custom->ucPlayerIndex, temp);

    if( g_pkQuestManager->m_bEndGame )
    {
        std::vector<DWORD>::iterator vit = std::find( g_MyD3D->m_pkQuestGameMgr->m_vecEndGameInfo.begin()
            , g_MyD3D->m_pkQuestGameMgr->m_vecEndGameInfo.end(), dwUID_ );
        if ( vit != g_MyD3D->m_pkQuestGameMgr->m_vecEndGameInfo.end() )
        {
            g_MyD3D->m_pkQuestGameMgr->m_vecEndGameInfo.erase( vit );
        }
    }
}

void   ON_GC_PID_QUEST_STAGE_POINT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{

    KGC_PID_QUEST_STAGE_POINT *Custom = (KGC_PID_QUEST_STAGE_POINT*)pRecvData_;

    if( 0 > Custom->ucPlayerIndex || Custom->ucPlayerIndex >= MAX_PLAYER_NUM ) return;

    SQuestPoint temp;
    temp.AerialAttack       = Custom->usAerialAttack;
    temp.Attack             = Custom->usAttack;
    temp.Attacked           = Custom->usAttacked;
    temp.BackAttack         = Custom->usBackAttack;
    temp.CounterAttack      = Custom->usCounterAttack;
    temp.CriticalAttack     = Custom->usCriticalAttack;
    temp.DoubleAttack       = Custom->usDoubleAttack;
    temp.iSpecial1          = Custom->usSpecial1;
    temp.iSpecial2          = Custom->usSpecial2;
    temp.iSpecial3          = Custom->usSpecial3;
    g_MyD3D->m_pkQuestGameMgr->SaveToQuestPointList(Custom->ucStage, Custom->ucPlayerIndex, temp);
}

void   ON_GC_PID_GATE_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_GATE_STATE *Custom = (KGC_PID_GATE_STATE*)pRecvData_;
    if( Custom->GetIndex() < 0 || Custom->GetIndex() >= MAX_OBJECT ) return;
    if( !g_MyD3D->m_pObject[Custom->GetIndex()] ) return;

	    if( GC_PORTAL == g_MyD3D->m_pObject[Custom->GetIndex()]->GetObjType() )
    {
        KGCDoor *pDoorObject = (KGCDoor*)g_MyD3D->m_pObject[Custom->GetIndex()];
		pDoorObject->SetCurHit(Custom->GetHit() );
        pDoorObject->SetDoorState( (KGCDoor::IRON_DOOR_STATE)Custom->GetState() );
    }

}

void   ON_GC_PID_PLAYERGETITEM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_PLAYERGETITEM kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );
    KGC_PID_PLAYERGETITEM* Custom = &kPacket;    
    SiKGCDropItemManager()->PushGotItem( Custom->m_dwPlayerUID, Custom->m_dwDropUID, Custom->m_cDropTpye );
}

void   ON_GC_PID_SELECT_BONUSBOX( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_SELECT_BONUSBOX* Custom = (KGC_PID_SELECT_BONUSBOX*)pRecvData_;
    
    g_pkGameOverlayUI->GetQuestResultS3()->GetBonusBoxSelect()->SetUserSelectBox( Custom->m_dwUserUID, Custom->m_ucSelectIndex );
}

void   ON_GC_PID_OPEN_BONUSBOX( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_OPEN_BONUSBOX kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );
    KGC_PID_OPEN_BONUSBOX *Custom = &kPacket;

}

void   ON_GC_PID_START_BONUSBOX_SELECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_START_BONUSBOX_SELECT* Custom = (KGC_PID_START_BONUSBOX_SELECT*)pRecvData_;
}

void   ON_GC_PID_READY_BONUSBOX_SELECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_READY_BONUSBOX_SELECT* Custom = (KGC_PID_READY_BONUSBOX_SELECT*)pRecvData_;
}

#define        PACKED_PACKET_CASE( ID )    case(ID):\
                                        P2PPacketProc( ID, pkPacket_->pPackedPacket + iOffset, dwUID_, kElementSize );\
                                        iOffset += kElementSize;\
                                        break

void   ON_GC_PID_PACKED_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_PACKED_PACKET* pkPacket_ = (KGC_PID_PACKED_PACKET*)pRecvData_;
    int        iOffset = 0;
    PACKED_ELEMENT_SIZE kElementSize = 0;
    GCPID    cPacketID = GC_PID_END;
    for ( int i = 0; i < (int)pkPacket_->ucPacketCount; i++ )
    {
		memcpy( &kElementSize, &pkPacket_->pPackedPacket[iOffset], sizeof( PACKED_ELEMENT_SIZE ) );
        //kElementSize = (PACKED_ELEMENT_SIZE)pkPacket_->pPackedPacket[iOffset];
        iOffset += sizeof( PACKED_ELEMENT_SIZE );
        cPacketID = (GCPID)pkPacket_->pPackedPacket[iOffset];
        switch( cPacketID )
        {
            PACKED_PACKET_CASE( GC_PID_EFFECT_ITEM );
            PACKED_PACKET_CASE( GC_PID_USEPETSKILL );
            PACKED_PACKET_CASE( GC_PID_TRANSFORM );
            PACKED_PACKET_CASE( GC_PID_TAG_CHARACTER );
            PACKED_PACKET_CASE( GC_PID_PLAYERLATENCY_FRAME_INFO );
            PACKED_PACKET_CASE( GC_PID_MONSTER_LATENCY_FRAME_INFO );
//             PACKED_PACKET_CASE( GC_PID_MONSTER_LIVE_INFO );
            PACKED_PACKET_CASE( GC_PID_TRANSNPC );
            PACKED_PACKET_CASE( GC_PID_GAME_BOARD_UPDATE );
            PACKED_PACKET_CASE( GC_PID_DIALOGUE_PROGRESS );
            PACKED_PACKET_CASE( GC_PID_QUEST_REAULT_ITEM_DICE );
            PACKED_PACKET_CASE( GC_PID_QUEST_STAGE_RESULT_SCORE );
            PACKED_PACKET_CASE( GC_PID_QUEST_STAGE_POINT );            
            PACKED_PACKET_CASE( GC_PID_GATE_STATE );
            PACKED_PACKET_CASE( GC_PID_UDP_RELAYSERVER_HEART_BEAT );
            PACKED_PACKET_CASE( GC_PID_DAMAGE_PACKET );                
            PACKED_PACKET_CASE( GC_PID_TG_DEFAULT );
            PACKED_PACKET_CASE( GC_PID_SWAPWEAPON );
            PACKED_PACKET_CASE( GC_PID_ENTER_GAME );
			PACKED_PACKET_CASE( GC_PID_COUPLE_EVENT_CREDIT );
            PACKED_PACKET_CASE( GC_PID_PLAYERGETITEM );
            PACKED_PACKET_CASE( GC_PID_SHIELD );
            PACKED_PACKET_CASE( GC_PID_MONSTER_TARGET_REFRESH );
            default:
            {
                break;
            }
        }
    }
}

void   ON_GC_PID_TCP_PACKED_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_TCP_PACKED_PACKET* pkPacket_ = (KGC_PID_TCP_PACKED_PACKET*)pRecvData_;
    int        iOffset = 0;
    PACKED_ELEMENT_SIZE kElementSize = 0;
    GCPID    cPacketID = GC_PID_END;
    for ( int i = 0; i < (int)pkPacket_->ucPacketCount; i++ )
    {
		memcpy( &kElementSize, &pkPacket_->pPackedPacket[iOffset], sizeof(PACKED_ELEMENT_SIZE) );
        //kElementSize = (PACKED_ELEMENT_SIZE)pkPacket_->pPackedPacket[iOffset];
        iOffset += sizeof( PACKED_ELEMENT_SIZE );
        cPacketID = (GCPID)pkPacket_->pPackedPacket[iOffset];
        switch( cPacketID )
        {
            PACKED_PACKET_CASE( GC_PID_EFFECT_ITEM );
            PACKED_PACKET_CASE( GC_PID_USEPETSKILL );
            PACKED_PACKET_CASE( GC_PID_TRANSFORM );
            PACKED_PACKET_CASE( GC_PID_TAG_CHARACTER );
            PACKED_PACKET_CASE( GC_PID_PLAYERLATENCY_FRAME_INFO );
            PACKED_PACKET_CASE( GC_PID_MONSTER_LATENCY_FRAME_INFO );
//            PACKED_PACKET_CASE( GC_PID_MONSTER_LIVE_INFO );
            PACKED_PACKET_CASE( GC_PID_TRANSNPC );
            PACKED_PACKET_CASE( GC_PID_GAME_BOARD_UPDATE );
            PACKED_PACKET_CASE( GC_PID_DIALOGUE_PROGRESS );
            PACKED_PACKET_CASE( GC_PID_QUEST_REAULT_ITEM_DICE );
            PACKED_PACKET_CASE( GC_PID_QUEST_STAGE_RESULT_SCORE );
            PACKED_PACKET_CASE( GC_PID_QUEST_STAGE_POINT );            
            PACKED_PACKET_CASE( GC_PID_GATE_STATE );
            PACKED_PACKET_CASE( GC_PID_UDP_RELAYSERVER_HEART_BEAT );
            PACKED_PACKET_CASE( GC_PID_DAMAGE_PACKET );                
            PACKED_PACKET_CASE( GC_PID_TG_DEFAULT );
            PACKED_PACKET_CASE( GC_PID_SQUARE_ACTION );
            PACKED_PACKET_CASE( GC_PID_SWAPWEAPON );
            PACKED_PACKET_CASE( GC_PID_ENTER_GAME );
			PACKED_PACKET_CASE( GC_PID_COUPLE_EVENT_CREDIT );
            PACKED_PACKET_CASE( GC_PID_PLAYERGETITEM );
            PACKED_PACKET_CASE( GC_PID_SHIELD );
            PACKED_PACKET_CASE( GC_PID_MONSTER_TARGET_REFRESH );
            default:
            {
                break;
            }
        }
    }
}

void AddConnectQ( const SUserInfo& kUserInfo_, bool bConnectNow_ )
{
    // [6/3/2008] breadceo. ConnectQ에 추가하고 P2P Connection한다.
    if( kUserInfo_.dwUID == g_kGlobalValue.m_kUserInfo.dwUID )
        return;

    if( kUserInfo_.bLive == false )
		return;


    if( KSingleton<KncP2P>::GetInstance()->IsIn( kUserInfo_.dwUID )  )
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_ADDCONNECT_TO_CONNECTED );


	KConnectInfo kInfo;
	kInfo.m_dwUID   = kUserInfo_.dwUID;
	kInfo.m_vecIP   = kUserInfo_.vecIP;
	kInfo.m_vecPort = kUserInfo_.vecPort;

    // [6/3/2008] breadceo. ConnectQ에 추가하고 P2P Connection한다.
    KSingleton<KncP2P>::GetInstance()->AddConnectQ( kInfo );

	if( bConnectNow_ )
		KSingleton<KncP2P>::GetInstance()->ConnectP2P( kUserInfo_.dwUID );
    
}

void   On_INSERT_USER( const KInDoorUserInfo& kPacket_ )
{
    //====================================================================================
    // 2006.07.14 : Asirion
    // Comment : 운영자가 들어오는 것이라면 따로 처리한다.    
    if ( kPacket_.m_bIsObserver )
    {
        SUserInfo temp;
        temp = kPacket_;
        SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo.push_back(temp);
        std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;
        for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
        {
            AddConnectQ( *vit );
        }

    }
    else
    {
        g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->ClearPlayerData();
        g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo = kPacket_;
        SUserInfo kTempUserInfo = g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo;
        g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->ClearWeaponChengeItem();

        for ( unsigned int i = 0; i < kPacket_.m_vecCharInfo.size(); ++i )
        {
            KInDoorCharInfo charInfo = kPacket_.m_vecCharInfo[i];
            g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->SetWeaponChangeItem(charInfo.m_cCharType, false, charInfo.m_kChangeWeaponItem );
        }

        // 서버에서 받은 아이템 정보에는 기본복장이 제거 되어 있다. 기본복장을 입히자.
        for( int i = 0 ; i < (int)g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo.vecCharInfo.size() ; ++i )
        {
            g_pItemMgr->EquipBaseItem( &g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo.vecCharInfo[i] );
        }

		g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo.nUserState = kPacket_.m_nState;

		if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
        {
            g_pRoomAnimationMgr->GetRoomAnimation( kPacket_.m_iPlayerIndex );
            g_bCheckGuardianWeapon = true;
            g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->RemovePlayerObject();
            g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->SetSelectMotion( true );
            g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->UpdatePlayerResource();
        }
        
        g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->vPos.x=0;
        g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->cFrame=0;
        g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo.kQuickSlot = kPacket_.m_kQuickSlot;

		g_kGlobalValue.m_kRanking.AddUserRankingInfo( kPacket_.m_dwUserUID, kPacket_.m_mapMonthlyRank );

        // 오후 10:19 2008-02-28 jemitgge - 차단기능을 위해 방에 들락날락하는 놈들 다 저장 -_-;
        if ( false == g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->IsLocalPlayer() )
            g_kGlobalValue.AddCurrentStateUser( g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->GetStrUserName() );
        
		SiKGCCoupleSystem()->AddCoupleUserInfo( kPacket_ );
        SiKGCRoomManager()->GetRoomInfo().bOpenSlot[kPacket_.m_iPlayerIndex] = false;
#if defined(USE_P2P_OR_TCP_RELAY)
		AddConnectQ( g_MyD3D->MyPlayer[kPacket_.m_iPlayerIndex]->m_kUserInfo, true );
#endif
    }
    if ( kPacket_.m_bIsHost )
    {
        SiKGCRoomManager()->GetRoomInfo().dwHostUID = kPacket_.m_dwUserUID;
    }
    
    g_pkUIScene->RoomButtonSetStartDelay();    
    SiKGCRoomManager()->GetRoomInfo().usUsers++;

	if( g_kGlobalValue.m_bChatEventMessageTemp == true )
	{
		g_kGlobalValue.m_iChatEventCount = -1;
		g_kGlobalValue.m_bChatEventMessageTemp = false;
		g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString( STR_ID_CHAT_EVENT_DISABLED2 ) );
	}
}

//LEAVE_SELF_DECISION             = 0,    // 개인 의지로 방을 나감
//LEAVE_BANNED_ON_PLAYING         = 1,    // 강퇴 : 이미 게임 시작함
//LEAVE_BANNED_EXCEED_MAXUSER     = 2,    // 강퇴 : 유저수가 너무 많음
//LEAVE_BANNED_HOST_DECISION      = 3,    // 강퇴 : 방장 의지로 강퇴
//LEAVE_SERVER_FIND_ERR           = 4,    // 서버측 소켓오류
//LEAVE_CLIENT_FIND_ERR           = 5,    // 클라이언트측 소켓오류
//LEAVE_HOST_MIGRATION_FAILED     = 6,    // 접속중인 방의 방장이 빠져나가서 방이 없어짐.
//NUM_OF_LEAVE_ROOM_REASON
void   On_DESTROY_USER( const KEVENT_INFORM_USER_LEAVE_ROOM_NOT& kPacket_ )
{

    int iPlayerIndex = g_MyD3D->Get_Player( kPacket_.m_dwUserUID );

    int strMsgID = 0;
    switch( kPacket_.m_nReasonWhy ) 
    {
    case 3:
        {
            strMsgID = STR_ID_GAMEMSG_REPORT_KICK;
            break;
        }
    default:
        {
            strMsgID = STR_ID_GAMEMSG_REPORT_LEAVE;
            break;
        }
    }

    //====================================================================================
    // 2006.07.14 : Asirion
    // Comment : 운영자가 나가는 것이라면 따로 처리를 하도록 한다.
    if ( SiKGCRoomManager()->IsPlayerObserver(kPacket_.m_dwUserUID))
    {
        SiKGCRoomManager()->InitObserver(kPacket_.m_dwUserUID);
    }
    else
    {
        JIF( CheckCorrectIndex( iPlayerIndex ) );

        g_MyD3D->MyPlayer[iPlayerIndex]->m_LoadingState.Init();
        
        if ( ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME || 
               g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING || 
               g_MyD3D->m_pStateMachine->GetState() == GS_ROOM ||
               g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ||
               g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM ||
               g_MyD3D->m_pStateMachine->GetState() == GS_GAME_END ) )
        {

            if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME || g_MyD3D->m_pStateMachine->GetState() == GS_GAME_END )
            {
                if ( g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
                {
                    if(g_pkChatManager != NULL)
                    {
                        g_pkChatManager->AddSystemMsg( iPlayerIndex, g_pkStrLoader->GetString( strMsgID ), 
                            KGCChatManager::CHAT_TYPE_ALERT );
                    }
                }
            }
            else
            {
                if(g_pkChatManager != NULL)
                {
                    g_pkChatManager->AddSystemMsg( iPlayerIndex, g_pkStrLoader->GetString( strMsgID ), 
                        KGCChatManager::CHAT_TYPE_ALERT );
                }
            }


            // 한번 나갔던 플레이어가 게임 결과창이 나오는 동안 다시 들어왔을 때 문제가 생겨서 수정
            if( g_pGameBoard != NULL )
                g_pGameBoard->m_pStartingMember[iPlayerIndex].dwUID = 0;

            // 변신 상태가 초기화가 안되는듯...
            if( g_MyD3D->MyPlayer[iPlayerIndex]->MetamorphosisForm != FORM_NORMAL )
                g_MyD3D->MyPlayer[iPlayerIndex]->ProcessMetamorphosisNormalForm( true );

            // 게임중에 나갈경우 나간 Player의 MagicEffect를 지워준다.
            g_pMagicEffect->ClearMagicEffect( iPlayerIndex );
            g_MyD3D->MyPlayer[iPlayerIndex]->EffectClear();
            //g_MyD3D->MyPlayer[iPlayerIndex]->m_cLife = 0;

            // 오후 10:19 2008-02-28 jemitgge - 차단기능을 위해 방에 들락날락하는 놈들 다 저장 -_-;
            g_kGlobalValue.DelCurrentStateUser( g_MyD3D->MyPlayer[iPlayerIndex]->GetStrUserName() );

            
        }
        g_MyD3D->ClearSlot( iPlayerIndex, false );
	}
#if defined( COUPLE_SYSTEM )
	SiKGCCoupleSystem()->RemoveCoupleUserInfo( kPacket_.m_dwUserUID );
	if( SiKGCCoupleSystem()->Are_you_My_Honey( kPacket_.m_dwUserUID ) && g_MyD3D->GetMyPlayer() )
	{
		g_pMagicEffect->ClearMagicEffect( g_MyD3D->GetMyPlayer()->m_iPlayerIndex, EGC_EFFECT_COUPLE_BUFF );
		g_pMagicEffect->ClearMagicEffect( g_MyD3D->GetMyPlayer()->m_iPlayerIndex, EGC_EFFECT_COUPLE_DAY );
		g_pMagicEffect->ClearMagicEffect( g_MyD3D->GetMyPlayer()->m_iPlayerIndex, EGC_EFFECT_COUPLE_BUFF_GP );
	}
#endif

    KGCStateLoading* pLoadingState = (KGCStateLoading*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME_LOADING );
    pLoadingState->RemoveP2PConnectList( kPacket_.m_dwUserUID );
    pLoadingState->RemoveFromLoadList( kPacket_.m_dwUserUID );



    SiKGCRoomManager()->GetRoomInfo().usUsers--;
    KSingleton<KncP2P>::GetInstance()->DisConnectPeer( kPacket_.m_dwUserUID );


    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING )
    {
        if ( g_kGlobalValue.m_kUserInfo.dwUID == kPacket_.m_dwUserUID )
        {
            g_MyD3D->m_pStateMachine->ExitRoom( false );
            return;
        }
    }
    g_pkArenaBattleDlg->UpdateDlg();

    // 던전스테이지 로딩 시 나가버리는 유저가 생길 경우에는 m_vecWaitToLoad 이 안의 리스트도 제거되어야 한다.
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
        g_pkQuestManager->SetStageLoadComplete(kPacket_.m_dwUserUID);

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
         SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        KGCDeathMatch* pGameMode = (KGCDeathMatch*)g_pStateGame->GetCurGameModePtr();
        pGameMode->UpdateStatBalace();
        g_pkGameOverlayUI->m_pkDeathMatchScoreBoard->UpdateGameData();
    }

    if ( g_MyD3D->m_pStateMachine->IsStateRoom() )
    {        
        g_pkUIScene->RoomSceneUpdateData();
        g_pkUIScene->RoomButtonSetStartDelay();
    }

    g_pGameBoard->OnLeavePlayer( iPlayerIndex );

	if( g_kGlobalValue.m_kChattingEvent.GetSuccessEvent() )
   {
	//나가면 채팅 버프 없에줌.
		g_kGlobalValue.m_kChattingEvent.SetSuccessEvent(false);
        g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString( STR_ID_CHAT_EVENT_DISABLED1 ) );
		//  g_pMagicEffect->ClearMagicEffect( iPlayerIndex, EGC_EFFECT_CHATTING_BUFF );
    }


	if( g_kGlobalValue.m_bChatEventMessageTemp == true )
	{
		g_kGlobalValue.m_iChatEventCount = -1;
		g_kGlobalValue.m_bChatEventMessageTemp = false;
		g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString( STR_ID_CHAT_EVENT_DISABLED1 ) );
	}

    g_MyD3D->MyPlayer[iPlayerIndex]->ClearPlayerData();

    g_MyD3D->MyPlayer[iPlayerIndex]->RemovePlayerObject();

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
    {
        g_pkUIScene->m_pkGameOverlayUI->UpdateDotaMiniMap();
    }

    SAFE_REMOVE_DELETE(g_MyD3D->m_akPet[iPlayerIndex]->m_pObject);
}

void   On_TERMINATE_USER( const KEVENT_INFORM_USER_LEAVE_ROOM_NOT& kPacket_ )
{  
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                g_pkStrLoader->GetString( STR_ID_GAMEMSG_KICKOUT1 ),
                                g_pkStrLoader->GetString( STR_ID_GAMEMSG_KICKOUT2 ) );

	SiKGCCoupleSystem()->ClearCoupleUserInfo();
    g_MyD3D->m_pStateMachine->ExitRoom( false );
    
    SiKGCRoomManager()->AddBannedRoom( SiKGCRoomManager()->GetRoomInfo().usRoomID, timeGetTime() );    
}
void   On_LOAD_TIMEOUT( const KEVENT_INFORM_USER_LEAVE_ROOM_NOT& kPacket_ )
{
	//g_MyD3D->m_pStateMachine->OnExitGame();
	KP2P::GetInstance()->Send_ClientFailedGameStartNot(KFailRate::EXCEED_LOADING_TIME);
	g_MyD3D->m_pStateMachine->ExitRoom( false );
	
    std::wstring strTemp = boost::str(boost::wformat( L"%1% - (%2%)")
        %g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE2 )%KClientErr::EM_LOADING_TIME_OUT1);


    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_ROOM,
                g_pkStrLoader->GetString( STR_ID_CAN_NOT_LOADCOMPLETE1 ),
                strTemp );
	
    g_MyD3D->m_pStateMachine->ExitRoom( false );
}

void On_HOSTMIGRATE( const KEVENT_HOST_MIGRATED_NOT kPacket_ )
{
    int iNewHostIndex = g_MyD3D->Get_Player( kPacket_.m_dwNewHostUID );
    JIF( CheckCorrectIndex( iNewHostIndex ) );
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i ) {
		g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost = false;
	}
    g_MyD3D->MyPlayer[iNewHostIndex]->SetHostPlayer( true );

    // 호위몹 호스트 마이그래이션

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if ( g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) == false || !mit->second->IsEscortMonster())
			continue;
		mit->second->m_dwOwnerUID = kPacket_.m_dwNewHostUID;
	}

    // 누구누구님이 방장입니다 메세지 뜨는거..
    if( g_pkChatManager != NULL )
        g_pkChatManager->AddSystemMsg( iNewHostIndex, g_pkStrLoader->GetString( STR_ID_GAMEMSG_REPORT_HOST ) );

    SiKGCRoomManager()->GetRoomInfo().dwHostUID = kPacket_.m_dwNewHostUID;

    // 이 부분 필요없을듯.. 옛날에 LEVEL_LIMIT_SYSTEM 일때 사용했던거임..
    //if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
    //{
    //    SiKGCRoomManager()->ResetGameMode();
    //    SiKGCRoomManager()->ResetSubGameMode();
    //    g_pkUIScene->RoomSceneUpdateData();
    //}

    g_pkUIScene->RoomButtonSetStartDelay( 219 );
    SiKGCRoomManager()->GetPrevRoomInfo() = SiKGCRoomManager()->GetRoomInfo();

    if ( g_kGlobalValue.m_kUserInfo.bHost    //방장이고
        && g_MyD3D->m_pStateMachine->GetState() == GS_ROOM) //방에서의 host_migration
    {
        //바뀐 방정보를 보낸다.
        KP2P::GetInstance()->Send_ChangeRoomInfoReq( &SiKGCRoomManager()->GetRoomInfo() );
    }
}

void On_HOSTMIGRATE_WithoutLeave(const DWORD dwNewHostUID)
{
    int iNewHostIndex = g_MyD3D->Get_Player(dwNewHostUID);
    JIF(CheckCorrectIndex(iNewHostIndex));
    g_kGlobalValue.m_kUserInfo.bHost = false;
    for (int i = 0; i < MAX_PLAYER_NUM; ++i) {
        g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost = false;
    }
    g_MyD3D->MyPlayer[iNewHostIndex]->m_kUserInfo.bHost = true;

    if (g_kGlobalValue.m_kUserInfo.dwUID == dwNewHostUID)
        g_kGlobalValue.m_kUserInfo.bHost = true;

    SiKGCRoomManager()->GetRoomInfo().dwHostUID = dwNewHostUID;
    SiKGCRoomManager()->GetPrevRoomInfo() = SiKGCRoomManager()->GetRoomInfo();

    g_pkChatManager->AddChatMsgTo(GS_GAME, g_pkStrLoader->GetReplacedString(STR_ID_NETWORK_HOST_MIGRATED, "l", g_MyD3D->MyPlayer[iNewHostIndex]->GetStrUserName()), KGCChatManager::CHAT_TYPE_ADMIN);
}

void On_CHANGE_CHARACTER(const KChangeRoomUserInfo& kPacket_)
{
    int iUserIndex = g_MyD3D->Get_Player(kPacket_.m_dwUserUID);
    JIF(CheckCorrectIndex(iUserIndex) == true);
    g_MyD3D->MyPlayer[iUserIndex]->cFrame = 0;
    g_MyD3D->MyPlayer[iUserIndex]->m_kUserInfo = kPacket_;
    g_MyD3D->MyPlayer[iUserIndex]->RemovePlayerObject();
    g_MyD3D->MyPlayer[iUserIndex]->SetSelectMotion(true);
    g_MyD3D->MyPlayer[iUserIndex]->UpdatePlayerResource();
    g_MyD3D->MyPlayer[iUserIndex]->Calcualte_Point();
    if (g_MyD3D->MyPlayer[iUserIndex]->IsLocalPlayer() == true)
    {
        g_MyD3D->MyPlayer[iUserIndex]->UpdateGlobalUserInfo();
    }
    if (g_pkUIScene->RoomButtonIsAutoStart() == false)
        g_pkUIScene->RoomButtonSetStartDelay(219);
}

void   On_CHANGE_SLOT( const KChangeRoomUserInfo& kPacket_ )
{
    int iIndex = 0;
    for( iIndex = 0; iIndex < MAX_PLAYER_NUM; ++iIndex )
    {
        if( true == g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.strLogin == kPacket_.m_strLogin )
            break;
    }

    // 옵저버인 경우에는  change_slot 필요 없다 .
    if ( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() &&
        SiKGCRoomManager()->IsPlayerObserver(kPacket_.m_dwUserUID))
    {
        return;
    }

    if( iIndex < 0 || iIndex >= MAX_PLAYER_NUM )
        return;

    g_MyD3D->MovePlayer( kPacket_.m_nUserSlot, iIndex );
}

void   On_CHANGE_INDOORUSER_INFO(const KInDoorUserInfo& kPacket_)
{
    if (kPacket_.m_bIsObserver)
    {
        SUserInfo temp;
        temp = kPacket_;
        bool bFind = false;
        std::vector<SUserInfo>& observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;
        for (std::vector<SUserInfo>::iterator vit = observer.begin(); vit != observer.end(); vit++)
        {
            if (vit->dwUID == kPacket_.m_dwUserUID)
            {
                (*vit) = temp;
                bFind = true;
                break;
            }
        }
        if (!bFind)
        {
            SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo.push_back(temp);
        }
    }
    else
    {
        int iIndex = g_MyD3D->Get_Player(kPacket_.m_dwUserUID);
        if (iIndex > MAX_PLAYER_NUM || iIndex < 0)
            return;
        g_MyD3D->MyPlayer[iIndex]->ClearPlayerData();
        g_MyD3D->MyPlayer[iIndex]->m_kUserInfo = kPacket_;
        g_MyD3D->MyPlayer[iIndex]->RemovePlayerObject();
        g_MyD3D->MyPlayer[iIndex]->UpdatePlayerResource();
        g_MyD3D->MyPlayer[iIndex]->Calcualte_Point();
        g_MyD3D->MyPlayer[iIndex]->vPos.x = 0;
        g_MyD3D->MyPlayer[iIndex]->cFrame = 0;
    }
    g_pkUIScene->RoomSceneUpdateData();
}

void   On_CHANGE_TEAM_READY( const KChangeRoomUserInfo& kPacket_ )
{
	for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.strLogin == kPacket_.m_strLogin && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
		{
			g_MyD3D->MyPlayer[i]->m_kUserInfo = kPacket_;
			if ( g_MyD3D->Get_MyPlayer() == i )
			{
				g_kGlobalValue.m_kUserInfo = kPacket_;
			}
#ifdef CHAT_EVENT
			// 채팅 이벤트 체크 합니다.
			if( g_kGlobalValue.m_bChatEventMessageTemp == true &&
				g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) == false )
			{
				g_kGlobalValue.m_iChatEventCount = -1;
				g_kGlobalValue.m_bChatEventMessageTemp = false;
				g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString(STR_ID_CHAT_EVENT_DISABLED4) );
			}
#endif
			break;
		}
	}

#ifdef CHAT_EVENT
	int iReadyPlayerNum = 0;
	int iTotalPlayerNum = 0;
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
		{
			++iTotalPlayerNum;
			if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) == true )
			{
				++iReadyPlayerNum;
			}
		}
	}
	int iEventPlayerNum = 5;
	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
	{
		iEventPlayerNum = 3;
	}
	if( iTotalPlayerNum - 1 == iReadyPlayerNum && iReadyPlayerNum >= iEventPlayerNum )
	{
		g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString(STR_ID_CHAT_EVENT_INFORMATION) );
	}
#endif
}

void   On_CHANGE_ROOM_INFO( const KChangeRoomInfo& kPacket_ )
{
    SiKGCRoomManager()->UpdateRoomInfo( kPacket_ );
    g_pkUIScene->RoomSceneUpdateData();
	if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost == true )
        g_pkUIScene->RoomButtonSetStartDelay( 219 );
}

void On_CHANGE_EQUIP_ITEM( const KEquipUser& kPacket_ )
{
    int iPlayerIndex = 7;
    // 룸안에 있을때만 myplayer에서 정보 가져온다.
    if ( g_MyD3D->m_pStateMachine->IsStateRoom() || g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
    {
        iPlayerIndex = g_MyD3D->Get_Player( kPacket_.m_dwUID );
    }

    if( false == g_MyD3D->IsPlayerIndex(iPlayerIndex) )
        return;

    std::vector<SCharInfo>& vecChar = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.vecCharInfo;
    bool bWeaponChange = false;
    for( int i = 0 ; i < (int)vecChar.size() ; ++i )
    {
        SCharInfo& CharInfo = vecChar[i];

        //케릭터 맵에 대한 예외 처리
        if( kPacket_.m_mapEquipInfo.count( CharInfo.iCharType ) == 0 )
        {
            //이상하다. 유저가 가진 케릭터임에도 불구하고 아이템 정보가 없다.
            continue;
        }

        std::map<char, KChangeEquipment >::const_iterator mit;
        mit = kPacket_.m_mapEquipInfo.find(CharInfo.iCharType);
        if( mit == kPacket_.m_mapEquipInfo.end() )
            continue;

        // 기본 장비는 우선 다 벗기도록 하자..
        // 서버에서 준 정보를 적용시킨 뒤에 다시 기본 장비를 입힌다..
        g_pItemMgr->UnEquipBaseItem( &CharInfo );

        const KChangeEquipment& kChangedEquip = mit->second;    //레퍼런스로 접근해야지~

        //벗은 아이템을 벗겨 봅시다.
        for( int j = 0 ; j < (int) kChangedEquip.m_vecReleaseItems.size() ; ++j )
        {
            for( int k = 0 ; k < (int)CharInfo.vecItemInfo.size() ; ++k )
            {
                //그럴리 없지만 같은데 다르다고 할까봐 UID만 따로 다시 비교해 줍니다. ( UID가 다르면 다른아이템이다!! )
                if( CharInfo.vecItemInfo[k].dwItemUID != kChangedEquip.m_vecReleaseItems[j].m_dwUID )
                    continue;

                CharInfo.vecItemInfo.erase( CharInfo.vecItemInfo.begin() + k );
                break;
            }
        }

        //입은 아이템을 입혀 봅시다.
        for( int j = 0 ; j < (int) kChangedEquip.m_vecEquipItems.size() ; ++j )
        {
            //같은 아이템이 있으면 또 안넣어줘야 합니다.
            bool bExist = false;
			KUserItemVector::iterator it = CharInfo.vecItemInfo.begin();
			for( ; it != CharInfo.vecItemInfo.end() ; it++ )
			{
				if( static_cast<DWORD>(it->iItemID) * 10 == kChangedEquip.m_vecEquipItems[j].m_dwID )
				{
					bExist = true;
					break;
				}
			}
			//버그는 어떻게 찾아내는 거야 ㅠ,.ㅠ
			if(!bExist)
			{
                GCItem *kItem2 = g_pItemMgr->GetItemData(kChangedEquip.m_vecEquipItems[j].m_dwID/10);
                it = CharInfo.vecItemInfo.begin();
                for( ; it != CharInfo.vecItemInfo.end() ; it++ )
                {
                    GCItem *kItem1 = g_pItemMgr->GetItemData(it->iItemID);


                    if( (kItem1->dwSlotPosition & ESP_A_EARRING) && (kItem2->dwSlotPosition & ESP_A_EARRING) )
                    {
                        continue;
                    }
                    else if(kItem1->dwSlotPosition == kItem2->dwSlotPosition)
                    {
                        CharInfo.vecItemInfo.erase(it);
                        break;
                    }
                }
			}

            if( bExist == false )
            {
                SUserItem sUserItem;
                sUserItem = (KEquipItemInfo)kChangedEquip.m_vecEquipItems[j];
                CharInfo.vecItemInfo.push_back( sUserItem );
            }
        }
        g_pItemMgr->EquipBaseItem( &CharInfo );

        // 무기체인지 정보쓰기!
#ifdef WEAPONCHANGE
        bool bInGame = (g_MyD3D->m_pStateMachine->GetState() == GS_GAME);        

        bWeaponChange = g_MyD3D->MyPlayer[iPlayerIndex]->SetWeaponChangeItem(CharInfo.iCharType, false, kChangedEquip.m_kChangeWeaponItem, bInGame );
#endif

        //UID를 클라이언트용으로 바꾸자!!
        if( kChangedEquip.m_equipPetInfo.m_dwUID == 0 )
        {
            CharInfo.kPetInfo.Init();
            continue;
        }

        //케릭터가 데리고 있는 펫 정보를 갱신해 줍시다.
        CharInfo.kPetInfo = kChangedEquip.m_equipPetInfo;

        CharInfo.kPetInfo.m_dwUID =  CharInfo.kPetInfo.m_dwUID;
        CharInfo.kPetInfo.m_dwID = CharInfo.kPetInfo.m_dwID / 10;
		CharInfo.kPetInfo.m_kCostume.m_dwID = CharInfo.kPetInfo.m_kCostume.m_dwID / 10;
		CharInfo.kPetInfo.m_kCostume.m_dwUID =  CharInfo.kPetInfo.m_kCostume.m_dwUID;

        // 2007.12.04 wony
        // 펫이 끼고 있는 아이템의 정보도 클라이언트에서 쓰는 값으로 바꿔줘야한다
        for( int j = 0; j < (int)CharInfo.kPetInfo.m_vecEquipItem.size(); ++j )
        {
            bool bDurationItem = ( CharInfo.kPetInfo.m_vecEquipItem[j].m_cItemType != 0 );
            CharInfo.kPetInfo.m_vecEquipItem[j].m_dwUID =  CharInfo.kPetInfo.m_vecEquipItem[j].m_dwUID;
            CharInfo.kPetInfo.m_vecEquipItem[j].m_dwID = CharInfo.kPetInfo.m_vecEquipItem[j].m_dwID / 10;
        }

        // m_mapPetInfo에 Local User 펫정보만 넣도록 수정.
        if( g_MyD3D->MyPlayer[iPlayerIndex]->IsLocalPlayer() == true )
        {
            g_MyD3D->m_mapPetInfo[ CharInfo.kPetInfo.m_dwUID ].m_vecEquipItem = CharInfo.kPetInfo.m_vecEquipItem;
            std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( CharInfo.kPetInfo.m_dwUID );
            if( mit != g_MyD3D->m_mapPetInfo.end() )
            {
                mit->second.m_vecEquipItem = CharInfo.kPetInfo.m_vecEquipItem;
            }
        }
    }
    g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.SetCurrentChar( kPacket_.m_cCharType );
    g_MyD3D->MyPlayer[iPlayerIndex]->cFrame = 0;
    
    if( bWeaponChange == true ) {        
        g_MyD3D->MyPlayer[iPlayerIndex]->ForcedClearBuffList();
    }

    if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) { 
        g_MyD3D->MyPlayer[ iPlayerIndex ]->SetOnGameItemChange( true );
        g_MyD3D->MyPlayer[ iPlayerIndex ]->ReloadResource();
        g_MyD3D->MyPlayer[ iPlayerIndex ]->SetOnGameItemChange( false );        
    }
    else { 
        g_MyD3D->MyPlayer[iPlayerIndex]->SetSelectMotion( true );
        g_MyD3D->MyPlayer[iPlayerIndex]->UpdatePlayerResource();
        g_MyD3D->MyPlayer[iPlayerIndex]->Calcualte_Point();
    }

    if( bWeaponChange == true && g_kGlobalValue.IsCharUseSpecial4( kPacket_.m_cCharType ) ) {        
        g_MyD3D->MyPlayer[iPlayerIndex]->UpdateChangedSkillSlot();
    }

    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
    {
        if ( g_MyD3D->MyPlayer[iPlayerIndex]->IsLocalPlayer() )
            g_MyD3D->MyPlayer[iPlayerIndex]->UpdateGlobalUserInfo();
    }

    if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        g_pkUIScene->RoomSceneUpdateData();
	else if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO )
		g_pkUIScene->m_pkMyInfoScene->UpdateData();
	else if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )
	{
		g_pkUIScene->m_pkMyInfoScene->UpdateData();
		g_pkUIScene->RoomSceneUpdateData();
	}
    else if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM)
    {
        g_pkUIScene->RoomSceneUpdateData();
    }
}

void   On_CHANGE_INDIGO_SLOT( const KChangeRoomUserInfo& kUserInfo_)
{
    if ( kUserInfo_.m_nUserSlot >= MAX_PLAYER_NUM || kUserInfo_.m_nUserSlot < 0 )
        return;
    for ( int i = 0; i < 3; i++ )
    {
        g_MyD3D->MyPlayer[kUserInfo_.m_nUserSlot]->m_kUserInfo.aiTagSlot[i] = kUserInfo_.m_acTagModeInfo[i][0];
    }
}

void    On_CHANGE_INTO_INFO( const KChangeRoomUserInfo& kUserInfo_)
{
    int iPlayerIndex = g_MyD3D->Get_Player( kUserInfo_.m_dwUserUID );

    g_pRoomAnimationMgr->GetRoomAnimation( iPlayerIndex );
    g_bCheckGuardianWeapon = true;
    g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.nUserState = kUserInfo_.m_nState;
	g_MyD3D->MyPlayer[iPlayerIndex]->RemovePlayerObject();
	g_MyD3D->MyPlayer[iPlayerIndex]->UpdatePlayerResource();
}

bool On_EQUIP_ITEM_TO_USERINFO(SUserInfo& kUserInfo, const KEVENT_EQUIP_ITEM_ACK& kEquipItem_)
{
    std::vector<SCharInfo>& vecChar = kUserInfo.vecCharInfo;

    for (int i = 0; i < (int)vecChar.size(); ++i)
    {
        SCharInfo& CharInfo = vecChar[i];

        int iCharIndex = -1;
        if (CharInfo.iCharType == kEquipItem_.m_vecCharacterInfo[i].m_cCharType)
        {
            iCharIndex = i;
        }
        else
        {
            for (int j = 0; j < (int)kEquipItem_.m_vecCharacterInfo.size(); ++j)
            {
                if (CharInfo.iCharType == kEquipItem_.m_vecCharacterInfo[j].m_cCharType)
                {
                    iCharIndex = j;
                }
            }
        }

        if (iCharIndex == -1)
            continue;

        const KInDoorCharInfo& kIndoorCharInfo = kEquipItem_.m_vecCharacterInfo[iCharIndex];
        CharInfo = kIndoorCharInfo;

        g_pItemMgr->EquipBaseItem(&CharInfo);
    }
    kUserInfo.SetCurrentChar(kEquipItem_.m_cCharType);

    return true;
}

void On_EQUIP_ITEM(const KEVENT_EQUIP_ITEM_ACK& kEquipItem_)
{
    int iPlayerIndex = g_MyD3D->Get_MyPlayer();
    if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex))
        return;

    if (On_EQUIP_ITEM_TO_USERINFO(g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo, kEquipItem_) == false)
        return;

    g_MyD3D->MyPlayer[iPlayerIndex]->cFrame = 0;
    g_MyD3D->MyPlayer[iPlayerIndex]->RemovePlayerObject();
    g_MyD3D->MyPlayer[iPlayerIndex]->UpdatePlayerResource();
    g_MyD3D->MyPlayer[iPlayerIndex]->Calcualte_Point();
    g_MyD3D->MyPlayer[iPlayerIndex]->UpdateGlobalUserInfo();

    if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
        g_pkUIScene->RoomSceneUpdateData();
    else if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO)
        g_pkUIScene->m_pkMyInfoScene->UpdateData();
    else if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
    {
        g_pkUIScene->m_pkMyInfoScene->UpdateData();
        g_pkUIScene->RoomSceneUpdateData();
    }
    else if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM)
    {
        g_pkUIScene->RoomSceneUpdateData();
    }
}

void   ON_GC_PID_SUMMON_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_SUMMON_MONSTER *Custom = (KGC_PID_SUMMON_MONSTER*)pRecvData_;

    if( g_kMonsterManager.IsSummonMonsterByPlayer(Custom->m_usMonIndex) )
    {
        ASSERT( !"Custom->m_ucMonIndex >= SUMMON_MONSTER_INDEX" );
    }

    MONSTER *pMonster = g_kMonsterManager.GetMonster( Custom->m_usMonIndex );
    if( pMonster && pMonster->IsLive() )
    {
        pMonster->EndMonster( true, true, true );
    }

	float x = GCUTIL_MATH::halfToFloat( Custom->m_usEntranceX );
	float y = GCUTIL_MATH::halfToFloat( Custom->m_usEntranceY );

	g_MyD3D->AddMonster( Custom->m_usMonType, x, y, Custom->m_bIsRight, false, 0, Custom->m_iLevel, 0, Custom->m_bCheckSafePostion, false, Custom->m_usMonIndex );

    MONSTER* pMon = g_kMonsterManager.GetMonster( Custom->m_usMonIndex );

	if ( pMon == NULL ) return;

	pMon->m_bLive = true;
    pMon->m_pObject->SetRender( true );
    pMon->SetCountValue( Custom->m_iCountValue );
    pMon->SetState( Custom->m_szState );
    pMon->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
}

void ON_GC_PID_BROAD_DOTA_POINT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DOTA_POINT *pCustom = (KGC_PID_BROAD_DOTA_POINT*)pRecvData_;

    if( pCustom )
    {
        for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            if( g_MyD3D->MyPlayer[ iLoop ]->IsLive() )
            {
                g_MyD3D->MyPlayer[ iLoop ]->Set_D_Point( pCustom->iDPoint[iLoop] );

                if( pCustom->bKillDeathInfo )
                {
                    g_pGameBoard->m_pStartingMember[iLoop].How_Die = pCustom->iDeath[iLoop];
                    g_pGameBoard->m_pStartingMember[iLoop].How_Kill = pCustom->iKill[iLoop];
        }
            }
        }
        g_pkUIScene->m_pkDotaShop->RefreshMyPoint();
    }
}

void ON_GC_PID_BUY_DOTA_GAMEITEM_REQ( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BUY_DOTA_GAMEITEM *pCustom = (KGC_PID_BUY_DOTA_GAMEITEM*)pRecvData_;

    // 방장만 처리하도록
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( pCustom && g_MyD3D->IsPlayerIndex( iPlayerIdx ) && g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.bHost )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pCustom->dwUID );
        if( pPlayer )
        {
            SDotaTemplate *pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
            if( pDotaTemplate )
            {
                SDota_D_PointShopItemInfo m_sDSItemInfo = pDotaTemplate->sDPointShopItemList;
                std::vector<pair<int, int>>::iterator vecIter = m_sDSItemInfo.vecItemList.begin();
                for(; vecIter != m_sDSItemInfo.vecItemList.end(); ++vecIter)
                {
                    if( vecIter->first == pCustom->iGameItemID )
                        break;
                }
                if( vecIter != m_sDSItemInfo.vecItemList.end() )
                {
                    if( pPlayer->Get_D_Point() >= vecIter->second )
                    {
                        pCustom->iOK = 0;
                        pPlayer->Set_D_Point( (pPlayer->Get_D_Point() - vecIter->second) );
                    }
                    else
                        pCustom->iOK = 1;   // 아이템 가격오류
                }
                else
                    pCustom->iOK = 2;       // 아이템ID오류
            }
        }
        else
            pCustom->iOK = 3;           // UID오류

        if( 0 == pCustom->iOK )
        {
            // 처리 결과 전달
            int iDPoint[MAX_PLAYER_NUM]={0,};
            for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
            {
                if( g_MyD3D->MyPlayer[ iLoop ]->IsLive() )
                    iDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
            }

            // DPoint 보내기 ACK로 Send해주기
            KGC_PID_BROAD_DOTA_POINT kDPoint( GC_PID_BROAD_DOTA_POINT, g_MyD3D->MyPlayer[ iPlayerIdx ]->m_kUserInfo.dwUID, iDPoint );
            SendP2PPacket( &kDPoint, sizeof(kDPoint), KNC_BROADCAST_NOT_ME, _RELIABLE );
            KGCPC_MEMO( "GC_PID_BROAD_DOTA_POINT" );
        }

        // 구입아이템 그대로 보내기 ACK로 Send해주기
        KGC_PID_BUY_DOTA_GAMEITEM kGameItem( GC_PID_BUY_DOTA_GAMEITEM_ACK, pCustom->dwUID, pCustom->iGameItemID, pCustom->iOK );
        SendP2PPacket( &kGameItem, sizeof(kGameItem), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "GC_PID_BUY_DOTA_GAMEITEM_ACK" );
    }
}

void ON_GC_PID_BUY_DOTA_GAMEITEM_ACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BUY_DOTA_GAMEITEM *pCustom = (KGC_PID_BUY_DOTA_GAMEITEM*)pRecvData_;

    if( pCustom )
    {
        int iMyPlayerIdx = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex(iMyPlayerIdx) && pCustom->dwUID == g_MyD3D->MyPlayer[ iMyPlayerIdx ]->m_kUserInfo.dwUID )
        {
            switch( pCustom->iOK )
            {
            case 0:
                // 구매 직후 즉시 사용템
                switch( pCustom->iGameItemID )
                {
                case GC_GAME_ITEM_DOTA_FULLHPUP:
                case GC_GAME_ITEM_DOTA_CRITICAL:
                case GC_GAME_ITEM_DOTA_ATK:
                case GC_GAME_ITEM_DOTA_NPC_ATTACKUP:
                case GC_GAME_ITEM_DOTA_NPC_HPUP:
                    if( false == g_MyD3D->MyPlayer[ iMyPlayerIdx ]->IsDie() )
                    {
                        g_MyD3D->MyItms->Use_Item(iMyPlayerIdx, pCustom->iGameItemID);
                    }
                    return;
                }

                g_MyD3D->MyPlayer[ iMyPlayerIdx ]->Take_Item( pCustom->iGameItemID );
                break;                
            case 1:// 아이템 가격오류
            case 2:// 아이템ID오류
            case 3:// UID오류
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BUY_POINT_ITEM_FAIL ), L"", KGCUIScene::GC_MBOX_USE_DOTA_GAME, 0, 0, false, true);
            }
        }

        // 점령전에서 구입한 아이템 통계를 남기자.
        if( 0 == pCustom->iOK )
        {
            PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pCustom->dwUID );
            if( pPlayer && 
                g_MyD3D->IsPlayerIndex(iMyPlayerIdx) &&
                g_MyD3D->MyPlayer[iMyPlayerIdx]->IsLocalPlayer() )
            {
                SDotaStats &sDotaStats = pPlayer->GetDotaStats();
                SCharInfo &sCharInfo = pPlayer->GetCurrentChar();
                std::map<std::pair<DWORD, GCITEMID>, int>::iterator mapIter = sDotaStats.m_mapItemBuyCount.find( std::make_pair(sCharInfo.iLevel, pCustom->iGameItemID) );
                if( mapIter != sDotaStats.m_mapItemBuyCount.end() )
                    ++mapIter->second;
                else
                    sDotaStats.m_mapItemBuyCount[ std::make_pair(sCharInfo.iLevel, pCustom->iGameItemID) ] = 1;
            }
        }
    }
}

void ON_GC_PID_BROAD_DOTA_ZONE_POSITION( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DOTA_ZONE_POSITION *pCustom = (KGC_PID_BROAD_DOTA_ZONE_POSITION*)pRecvData_;

    if( pCustom )
    {
        if( g_MyD3D->IsPlayerIndex( pCustom->iPlayerIdx ) )
        {
            if( pCustom->bSuper )
                g_MyD3D->MyPlayer[pCustom->iPlayerIdx]->SetSuper( "", 0, 0, 55, true );
            g_MyD3D->MyPlayer[pCustom->iPlayerIdx]->SetDotaStage( pCustom->iZonePos );
            g_pkUIScene->m_pkGameOverlayUI->UpdateDotaMiniMap();

            // 소환된 몬스터의 위치를 부모위치와 같게 해준다.
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) == false )
					continue;

				DWORD dwUID = mit->second->GetSummonPlayerUID();
				PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwUID );
				if( pPlayer && dwUID == g_MyD3D->MyPlayer[pCustom->iPlayerIdx]->m_kUserInfo.dwUID )
				{
					mit->second->SetX( pCustom->vPlayerPos.x );
					mit->second->SetY( pCustom->vPlayerPos.y );
				}

			}

            // 코어존 침입 알림 처리
            int iMyPlayer = g_MyD3D->Get_MyPlayer();
            SDotaTemplate* pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();

            if( pCurDotaInfo &&
                g_MyD3D->IsPlayerIndex(iMyPlayer) && 
                g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam != g_MyD3D->MyPlayer[pCustom->iPlayerIdx]->m_kUserInfo.iTeam )
            {
                // 코어존인지 확인
                if( ((int(TEAM_SERDIN) == g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam) && (pCurDotaInfo->iCoreZone_Serdin == pCustom->iZonePos)) ||
                    ((int(TEAM_CANABAN) == g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam) && (pCurDotaInfo->iCoreZone_Canaban == pCustom->iZonePos)) )
                {
                    KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
                    KInGameMessageAttribute kMessage;
                    kMessage.iStringID = pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_INCOREZONE].first;
                    kMessage.iShowFrame = pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_INCOREZONE].second;
                    kMessage.strValue1 = const_cast<WCHAR*>(g_MyD3D->MyPlayer[ pCustom->iPlayerIdx ]->GetStrUserName().c_str());
                    pMessage->SetMessage( &kMessage );

                    // 위험한 상황에 대한 파티클 처리를 해주자!
                    D3DXVECTOR3 vPos = g_MyD3D->MyPlayer[iMyPlayer]->GetPosition();
                    g_ParticleManager->CreateSequence( "Dota_Warning", vPos.x, vPos.y, 1.9f );
                }
            }
        }
    }
}


void   ON_GC_PID_UDP_RELAYSERVER_HEART_BEAT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{

}

void   ON_GC_PID_DAMAGE_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_DAMAGE_PACKET *Custom = (KGC_PID_DAMAGE_PACKET*)pRecvData_;

    if( -1 != Custom->m_sTG_ID )
    {
        CTriggerCondition *pCondition = g_kEveryTriggerManager.FindTriggerConditonByID(Custom->m_sTG_ID);
        if( pCondition )
        {
            if( CTriggerCondition::TG_ADDDAMAGE == pCondition->enType )
            {
                (static_cast<CTGAddDamage*>(pCondition))->StartDamage();
            }
        }
    }
}

void ON_GC_PID_SQUARE_ACTION( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_SQUARE_ACTION* Custom = (KGC_PID_SQUARE_ACTION*)pRecvData_;

    GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople( dwUID_ );
    // 없으면 무시
    if( pPeople == NULL )
        return;

    pPeople->GetSensor()->AddRemoteData( *Custom );
}

void ON_GC_PID_MONSTER_TARGET_REFRESH( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_MONSTER_TARGET_REFRESH* Custom = (KGC_PID_MONSTER_TARGET_REFRESH*)pRecvData_;

	MONSTER* pMonster = g_kMonsterManager.GetMonster( Custom->iMonsterIndex );
	if ( pMonster ) {
	    pMonster->ShowAwakeMake();
	}
}

void ON_GC_PID_QUEST_RANK_COMBO_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_QUEST_RANK_COMBO_STATE* Custom = (KGC_PID_QUEST_RANK_COMBO_STATE*)pRecvData_;

    // 콤보스테이트가 풀리고 공격을 받은거라면...
    if( Custom->bIsComboState == false && Custom->bAttacked )
    {
        g_pkQuestManager->CountComboBreak(); //감점을 해주시겠다!
    }
    
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == Custom->dwUID )
        {
            g_MyD3D->MyPlayer[i]->SetComboState(Custom->bIsComboState);
            break;
        }
    }
}

void ON_GC_PID_BURNNING_POINT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BURNNING_POINT* Custom = (KGC_PID_BURNNING_POINT*)pRecvData_;

    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == Custom->dwUID )
        {
            g_MyD3D->MyPlayer[i]->SetBurnningPoint(Custom->fBP);
            break;
        }
    }
}

void ON_GC_PID_RELAY_PING( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
	KGC_PID_RELAY_PING* Custom = (KGC_PID_RELAY_PING*)pRecvData_;

	DWORD dwPing = ::timeGetTime() - Custom->dwTimeStamp;

	if( Custom->ucServerType == KGC_PID_RELAY_PING::TCP_RELAY ) {
		START_LOG( clog, "TCP Relay Ping Test" )
			<< BUILD_LOG( dwPing )
			<< END_LOG;
	}
	else {
		START_LOG( clog, "UDP Relay Ping Test" )
			<< BUILD_LOG( dwPing )
			<< END_LOG;
	}

    //테스트코드
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //int ping = 0;
    //LUA_GET_VALUE_DEF( "Ping", ping, 0 );

    //KP2P::GetInstance()->Send_PingInfoNot( ping );
    if( dwPing > 0 )
	    KP2P::GetInstance()->Send_PingInfoNot( dwPing );

	if( RemovePing( Custom->dwUID ) == false ) {
		START_LOG( clog, "Relay Ping Test( Request Timed Out! )");
		return;
	}
    else {
        g_iPingOutCount = 0;
    }
}

void   ON_GC_PID_SHIELD( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_SHIELD* Custom = (KGC_PID_SHIELD*)pRecvData_;
    int Index;
    // 플레이어의 인덱스가 유효한지 확인한다.
    if ( ( Index = g_MyD3D->Get_Player( dwUID_ ) ) >= MAX_PLAYER_NUM ) 
    {
        return;
    }

    for ( int i = LATENCY - 1; i > -1; --i )
    {  
        g_MyD3D->MyPlayer[Index]->L_Shield[i] = Custom->ucShield[i];
    }
}

void SendServerRelayPacketInSquare( void* pkPacket_, ULONG ulPacketSize_, const std::vector< DWORD >& vecSend_ )
{
    if ( g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE )
        return;

    static KRelayData s_kPacket;
    static BYTE pCompressBuffer[MAX_COMPRESS_BUFFER] = {0,};
    static ULONG ulCompressLength = 0;
    s_kPacket.m_vecUID.clear();
    s_kPacket.m_vecUID = vecSend_;
    s_kPacket.m_cRecvCnt = (char)s_kPacket.m_vecUID.size();
    memset( pCompressBuffer, 0, MAX_COMPRESS_BUFFER );
    ulCompressLength = MAX_COMPRESS_BUFFER;
    if ( Z_OK != compress( pCompressBuffer, &ulCompressLength, (BYTE*)pkPacket_, ulPacketSize_) )
        return;
    s_kPacket.m_buff.clear();
    s_kPacket.m_buff.resize( (UINT)ulCompressLength + sizeof(DWORD) );
	memcpy( &s_kPacket.m_buff[0] , &g_kGlobalValue.m_kUserInfo.dwUID , sizeof(g_kGlobalValue.m_kUserInfo.dwUID) );
    memcpy( &s_kPacket.m_buff[0] + sizeof(g_kGlobalValue.m_kUserInfo.dwUID), pCompressBuffer, (DWORD)ulCompressLength );

    KGCPC_VALUE( "tcp packet", ulCompressLength );

    int nErrorCode = 0;
    SiKP2P()->m_spTRUserProxy->Send( s_kPacket.m_vecUID, &s_kPacket.m_buff[0], s_kPacket.m_buff.size(), nErrorCode );

}

// 광장에서 주고받는 패킷 프로시저
void SquarePacketProc( GCPID kiPacketID_, PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    switch( kiPacketID_ ) 
    {
        // GC_PID_SQUARE_ACTION 압축되서 온다
        _P2P_PACKET_CASE( GC_PID_TCP_PACKED_PACKET );
    default:
        {
            START_LOG( cerr, "SquarePacketProc Error!!" );
            return;
        }
    }
}

template< typename Packer, typename Packet >
void	SendFrameLatencyInfo( Packer* pkPacker , Packet* pkPacket )
{
    if( Result_EndGame != 0 )
        return;

    if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        g_MyD3D->m_NPC.PopNPC_Action();
    }

    // 자신이 죽어서 쓰러지고 있다면 아이템을 날린다.
    if( g_kTransNPC->TransNPC.What == 0 &&
        GC_GMC_DOTA != SiKGCRoomManager()->GetGameModeCategory() )
    {
        if( ( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetOwnMotion( MID_COMMON_DOWN_AND_STANDUP ) ||
            g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP ) ||
            g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->uiMotion == g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetOwnMotion( MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER )             
            )                 
            && !g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetHP() )
        {
            g_MyD3D->MyItms->Through_Item();
        }
        //시체 상태.
        if( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetPlayerState() == PS_DEAD )
        {
            g_MyD3D->MyItms->Through_MatchGP();
        }
    }

    int iIndex = g_MyD3D->Get_MyPlayer();
    g_kFrameInfoPacket->Type = GC_PID_PLAYERLATENCY_FRAME_INFO;
    g_kFrameInfoPacket->Life=g_MyD3D->MyPlayer[iIndex]->m_cLife;
    g_kFrameInfoPacket->Body_Angle=g_MyD3D->MyPlayer[iIndex]->Body_Angle;
    //g_kFrameInfoPacket->ucStage = (UCHAR)g_MyD3D->m_pkQuestGameMgr->GetCurrentStageID();    
    

    if( g_MyD3D->MyPlayer[iIndex]->m_cLife == 0 )
        g_pMagicEffect->ClearMagicEffect( iIndex, true );

    if( g_kLocalPlayerLatency->usShield[0] > 0 )
        g_MyD3D->MyPlayer[iIndex]->m_bExistShield = true;

    for( int i = SHOWLATENCY; i < (LATENCY+SHOWLATENCY); i++ )
    {
        g_kShield->ucShield[i-SHOWLATENCY] = static_cast<UCHAR>(g_kLocalPlayerLatency->usShield[i]);

#if defined(LEVEL_DESIGN_STAT_CALC)
        g_kFrameInfoPacket->iHP[i - SHOWLATENCY] = static_cast<int>(g_kLocalPlayerLatency->fHP[i]);
#else
        g_kFrameInfoPacket->iHP[i - SHOWLATENCY] = static_cast<int>(g_kLocalPlayerLatency->fHP[i] * 10000.0f);
#endif
        g_kFrameInfoPacket->IsRight[i-SHOWLATENCY]=g_kLocalPlayerLatency->kRenderData[i].bIsRight;
        g_kFrameInfoPacket->Frame[i-SHOWLATENCY]=g_kLocalPlayerLatency->kRenderData[i].cFrame;
        g_kFrameInfoPacket->Now_Motion[i-SHOWLATENCY]=g_kLocalPlayerLatency->kRenderData[i].usMotion;
        g_kFrameInfoPacket->usPosX[i-SHOWLATENCY] = GCUTIL_MATH::floatToHalf( g_kLocalPlayerLatency->kRenderData[i].vPos.x );
        g_kFrameInfoPacket->usPosY[i-SHOWLATENCY] = GCUTIL_MATH::floatToHalf( g_kLocalPlayerLatency->kRenderData[i].vPos.y );
    }

    //실드 hp가 있을 때만 패킹해서 보내겠다!!
    if( g_kLocalPlayerLatency->usShield[0] > 0 && g_MyD3D->MyPlayer[iIndex]->m_bExistShield && !g_MyD3D->MyPlayer[iIndex]->m_bStartShield )
    {
        g_MyD3D->MyPlayer[iIndex]->m_bStartShield = true;
         pkPacker->Pack( g_kShield, sizeof(KGC_PID_SHIELD) );
    }
    else if( g_kLocalPlayerLatency->usShield[0] == 0 && g_MyD3D->MyPlayer[iIndex]->m_bExistShield )
    {
         pkPacker->Pack( g_kShield, sizeof(KGC_PID_SHIELD) );
         g_MyD3D->MyPlayer[iIndex]->m_bExistShield = false;
         g_MyD3D->MyPlayer[iIndex]->m_bStartShield = false;
    }

    if ( g_kTransNPC->TransNPC.Action != -1 )
    {
        pkPacker->Pack( g_kTransNPC, sizeof(KGC_PID_TRANSNPC) );
    }
    // 펫스킬 사용했을 때
    if( g_kUsePetSkill->bUsePetSkill )
    {
        pkPacker->Pack( g_kUsePetSkill, sizeof(KGC_PID_USEPETSKILL) );
        g_kUsePetSkill->bUsePetSkill = false;
    }
    if( g_kTransform->cTransformType != FORM_NUM )
    {
        pkPacker->Pack( g_kTransform, sizeof(KGC_PID_TRANSFORM) );
        g_kTransform->cTransformType = FORM_NUM;
    }
    if( g_kTagCharacter->cPlayerIndex != -1 )
    {
        pkPacker->Pack( g_kTagCharacter, sizeof(KGC_PID_TAG_CHARACTER) );
        g_kTagCharacter->cPlayerIndex = -1;
        g_kTagCharacter->bArena = false;
    }
    if ( g_kPoisonBreath->iBreathIndex != -1 )
    {
        pkPacker->Pack( g_kPoisonBreath, sizeof(KGC_PID_POISON_BREATH) );
        g_kPoisonBreath->iBreathIndex = -1;
    }
    if( g_kSwapWeapon->ucPlayerIndex != UCHAR_MAX )
    {
        pkPacker->Pack( g_kSwapWeapon, sizeof(KGC_PID_SWAPWEAPON) );
        g_kSwapWeapon->ucPlayerIndex = UCHAR_MAX;
    }
    if( g_kEnterGame->ucPlayerIndex != UCHAR_MAX )
    {
        pkPacker->Pack( g_kEnterGame, sizeof(KGC_PID_ENTER_GAME) );
        g_kEnterGame->ucPlayerIndex = UCHAR_MAX;
    }
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if(g_kEffectItem->Eff_Item[i].cWhat > -1)
        {
            pkPacker->Pack( g_kEffectItem, sizeof(KGC_PID_EFFECT_ITEM) );
            break;
        }
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( SiKGCDropItemManager()->IsGPListForSendEmpty() == false )
        {
            KGC_PID_PLAYERGETITEM kGetItem;
            if( SiKGCDropItemManager()->IsExistGetItem(kGetItem))
            {
                SERIALIZE_P2P_PACKET( kGetItem );
                pkPacker->Pack( (void*)ksBuff.GetData(), (PACKED_ELEMENT_SIZE)ksBuff.GetLength() );
            }            
        }

        if( g_MyD3D->m_pkQuestGameMgr->IsEmptyMonsterTagetRefresh() == false )
        {
            KGC_PID_MONSTER_TARGET_REFRESH kGetItem[1];
            if( g_MyD3D->m_pkQuestGameMgr->IsExistMonsterTagetRefresh(kGetItem))
            {
                pkPacker->Pack( kGetItem, sizeof(KGC_PID_MONSTER_TARGET_REFRESH) );
            }            
        }
    }    

    // 다 죽었으니 확실하게 다 죽었다는걸 알려 주고 담부터는 패킷 안보낸다.
    if ( g_kFrameInfoPacket->Life == 0 && g_MyD3D->m_bLastFrameSend == false )
    {
        pkPacker->Pack( g_kFrameInfoPacket, sizeof( KGC_PID_PLAYERLATENCY_FRAME_INFO ) );

        g_MyD3D->m_bLastFrameSend = true;
    }    

#if !defined(USE_ONLY_TCP_RELAY)
    if ( pkPacker->GetPackCount() != 0 )
    {
        UINT uiPacketSize = 0;
        pkPacket = pkPacker->GetData( uiPacketSize );

        // 임시로 UDP 릴레이 서버로 보내지 않고 그냥 P2P로 보낸다..
        // 방장이 나갔을 경우 나머지 얘들 중에서.. 방장이어받지 못한녀석의 싱크가 잘 안맞는 현상이 발생해서..

        // 2007.08.30 wony
        // 퀘스트 모드에서 방장이면 릴레이 서버로 패킷을 날리도록 하겠음..
        // 몬스터 매프레임 정보를 다 보내기 때문에 방장의 부하를 줄여주기 위함임..
        SendP2PPacket( pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME, _RELIABLE );
    }
#endif
    

    //////////////////////////////////////////////////////////////////////////
    // _UNRELIABLE 로 보낼 패킷들 ㅡ 
    if ( g_MyD3D->m_bLastFrameSend == false )
    {
        pkPacker->Pack( g_kFrameInfoPacket, sizeof( KGC_PID_PLAYERLATENCY_FRAME_INFO ) );
    }


    // 몬스터의 프레임 정보 - 방장일 경우에만 보낸다!
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
//		g_kMonsterLiveInfoPacket->LiveInfo.clear();
		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER* pMonster = mit->second;
			if( pMonster->m_bLive ) // 현재 살아있는가?
			{
				g_kMonsterFrameInfoPacket->Type = GC_PID_MONSTER_LATENCY_FRAME_INFO;
				g_kMonsterFrameInfoPacket->cMonsterIndex = (char)mit->first;
				g_kMonsterFrameInfoPacket->iMonsterKind = pMonster->GetMonsterKind();
				g_kMonsterFrameInfoPacket->iMonsterType = pMonster->GetMonsterType();
				g_kMonsterFrameInfoPacket->uiHP = (UINT)( pMonster->m_fHP );
				g_kMonsterFrameInfoPacket->usBodyAngle = (USHORT)( pMonster->Body_Angle * 10000.0f );
				g_kMonsterFrameInfoPacket->cTargetIndex = (char)(pMonster->IsTargetMonster() ? pMonster->GetMonsterTarget() + MAX_PLAYER_NUM : pMonster->GetPlayerTarget() );
				g_kMonsterFrameInfoPacket->cLastAttackedMe = (char)pMonster->m_iLastAttackedMe;
                g_kMonsterFrameInfoPacket->iLevel = pMonster->GetLevel();
                g_kMonsterFrameInfoPacket->iBoss = pMonster->m_iBossMonster;
                g_kMonsterFrameInfoPacket->iStageID = g_kEveryTriggerManager.GetActiveStage();

				for( int j = SHOWLATENCY; j < ( LATENCY + SHOWLATENCY ); ++j )
				{
					g_kMonsterFrameInfoPacket->aucMotion[j - SHOWLATENCY] = (UCHAR)pMonster->m_aiMotion[j];
					g_kMonsterFrameInfoPacket->aucFrame[j - SHOWLATENCY] = (USHORT)pMonster->m_aiFrame[j];

					g_kMonsterFrameInfoPacket->ausX[j - SHOWLATENCY] = GCUTIL_MATH::floatToHalf( pMonster->m_afX[j] );
					g_kMonsterFrameInfoPacket->ausY[j - SHOWLATENCY] = GCUTIL_MATH::floatToHalf( pMonster->m_afY[j] );

					g_kMonsterFrameInfoPacket->abIsRight[j - SHOWLATENCY] = pMonster->m_abIsRight[j];
				}

				pkPacker->Pack( g_kMonsterFrameInfoPacket, sizeof(g_kMonsterFrameInfoPacket) );
			}

//			g_kMonsterLiveInfoPacket->LiveInfo[mit->first] = pMonster->IsLive();

		}

//        g_kMonsterLiveInfoPacket->Type = GC_PID_MONSTER_LIVE_INFO;
//        pkPacker->Pack( g_kMonsterLiveInfoPacket, sizeof(g_kMonsterLiveInfoPacket) );
    }
    if ( pkPacker->GetPackCount() != 0 )
    {
        UINT uiPacketSize = 0;
        pkPacket = pkPacker->GetData( uiPacketSize );

        // TCP Relay서버만을 쓴다면 여기서 한번만 보내겠다. 그리고 reliable, unreliable은 관계가 없다. TCP니까!!
        SendP2PPacket( pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME, _UNRELIABLE );
    }



    g_kTransNPC->TransNPC.Init();
    for( int i=0;i<MAX_PLAYER_NUM;i++) 
    {
        g_kEffectItem->Eff_Item[i].cWhat = -1;
        g_kEffectItem->Eff_Item[i].sTime = 0;
    }
}


template< typename Packer, typename Packet >
void	SendAgitFrameLatencyInfo( Packer* pkPacker , Packet* pkPacket )
{
    if( Result_EndGame != 0 )
        return;

    if ( g_kGlobalValue.m_kUserInfo.bHost )
    {
        g_MyD3D->m_NPC.PopNPC_Action();
    }

    int iIndex = g_MyD3D->Get_MyPlayer();
    g_kFrameInfoPacket->Type = GC_PID_PLAYERLATENCY_FRAME_INFO;
    g_kFrameInfoPacket->Life=g_MyD3D->MyPlayer[iIndex]->m_cLife;
    g_kFrameInfoPacket->Body_Angle=g_MyD3D->MyPlayer[iIndex]->Body_Angle;
    //g_kFrameInfoPacket->ucStage = (UCHAR)g_MyD3D->m_pkQuestGameMgr->GetCurrentStageID();    


    if( g_MyD3D->MyPlayer[iIndex]->m_cLife == 0 )
        g_pMagicEffect->ClearMagicEffect( iIndex, true );

    if( g_kLocalPlayerLatency->usShield[0] > 0 )
        g_MyD3D->MyPlayer[iIndex]->m_bExistShield = true;

    for( int i = SHOWLATENCY; i < (LATENCY+SHOWLATENCY); i++ )
    {
        g_kShield->ucShield[i-SHOWLATENCY] = static_cast<UCHAR>(g_kLocalPlayerLatency->usShield[i]);

        g_kFrameInfoPacket->iHP[i - SHOWLATENCY] = static_cast<int>(g_kLocalPlayerLatency->fHP[i] * 100.0f);
        g_kFrameInfoPacket->IsRight[i-SHOWLATENCY]=g_kLocalPlayerLatency->kRenderData[i].bIsRight;
        g_kFrameInfoPacket->Frame[i-SHOWLATENCY]=g_kLocalPlayerLatency->kRenderData[i].cFrame;
        g_kFrameInfoPacket->Now_Motion[i-SHOWLATENCY]=g_kLocalPlayerLatency->kRenderData[i].usMotion;
        g_kFrameInfoPacket->usPosX[i-SHOWLATENCY] = GCUTIL_MATH::floatToHalf( g_kLocalPlayerLatency->kRenderData[i].vPos.x );
        g_kFrameInfoPacket->usPosY[i-SHOWLATENCY] = GCUTIL_MATH::floatToHalf( g_kLocalPlayerLatency->kRenderData[i].vPos.y );
    }

    if ( g_kTransNPC->TransNPC.Action != -1 )
    {
        pkPacker->Pack( g_kTransNPC, sizeof(KGC_PID_TRANSNPC) );
    }

    if( g_kTransform->cTransformType != FORM_NUM )
    {
        pkPacker->Pack( g_kTransform, sizeof(KGC_PID_TRANSFORM) );
        g_kTransform->cTransformType = FORM_NUM;
    }

    if ( g_kPoisonBreath->iBreathIndex != -1 )
    {
        pkPacker->Pack( g_kPoisonBreath, sizeof(KGC_PID_POISON_BREATH) );
        g_kPoisonBreath->iBreathIndex = -1;
    }
    if( g_kSwapWeapon->ucPlayerIndex != UCHAR_MAX )
    {
        pkPacker->Pack( g_kSwapWeapon, sizeof(KGC_PID_SWAPWEAPON) );
        g_kSwapWeapon->ucPlayerIndex = UCHAR_MAX;
    }
    if( g_kEnterGame->ucPlayerIndex != UCHAR_MAX )
    {
        pkPacker->Pack( g_kEnterGame, sizeof(KGC_PID_ENTER_GAME) );
        g_kEnterGame->ucPlayerIndex = UCHAR_MAX;
    }
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if(g_kEffectItem->Eff_Item[i].cWhat > -1)
        {
            pkPacker->Pack( g_kEffectItem, sizeof(KGC_PID_EFFECT_ITEM) );
            break;
        }
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( g_MyD3D->m_pkQuestGameMgr->IsEmptyMonsterTagetRefresh() == false )
        {
            KGC_PID_MONSTER_TARGET_REFRESH kGetItem[1];
            if( g_MyD3D->m_pkQuestGameMgr->IsExistMonsterTagetRefresh(kGetItem))
            {
                pkPacker->Pack( kGetItem, sizeof(KGC_PID_MONSTER_TARGET_REFRESH) );
            }            
        }
    }    

    // 다 죽었으니 확실하게 다 죽었다는걸 알려 주고 담부터는 패킷 안보낸다.
    if ( g_kFrameInfoPacket->Life == 0 && g_MyD3D->m_bLastFrameSend == false )
    {
        pkPacker->Pack( g_kFrameInfoPacket, sizeof( KGC_PID_PLAYERLATENCY_FRAME_INFO ) );

        g_MyD3D->m_bLastFrameSend = true;
    }    

#if !defined(USE_ONLY_TCP_RELAY)
    if ( pkPacker->GetPackCount() != 0 )
    {
        UINT uiPacketSize = 0;
        pkPacket = pkPacker->GetData( uiPacketSize );

        // 임시로 UDP 릴레이 서버로 보내지 않고 그냥 P2P로 보낸다..
        // 방장이 나갔을 경우 나머지 얘들 중에서.. 방장이어받지 못한녀석의 싱크가 잘 안맞는 현상이 발생해서..

        // 2007.08.30 wony
        // 퀘스트 모드에서 방장이면 릴레이 서버로 패킷을 날리도록 하겠음..
        // 몬스터 매프레임 정보를 다 보내기 때문에 방장의 부하를 줄여주기 위함임..
        SendP2PPacket( pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME, _RELIABLE );
    }
#endif


    //////////////////////////////////////////////////////////////////////////
    // _UNRELIABLE 로 보낼 패킷들 ㅡ 
    if ( g_MyD3D->m_bLastFrameSend == false )
    {
        pkPacker->Pack( g_kFrameInfoPacket, sizeof( KGC_PID_PLAYERLATENCY_FRAME_INFO ) );
    }


    // 몬스터의 프레임 정보 - 방장일 경우에만 보낸다!
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
//		g_kMonsterLiveInfoPacket->LiveInfo.clear();
		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER* pMonster = mit->second;
			if( pMonster->m_bLive ) // 현재 살아있는가?
			{
				g_kMonsterFrameInfoPacket->Type = GC_PID_MONSTER_LATENCY_FRAME_INFO;
				g_kMonsterFrameInfoPacket->cMonsterIndex = (char)mit->first;
				g_kMonsterFrameInfoPacket->iMonsterKind = pMonster->GetMonsterKind();
				g_kMonsterFrameInfoPacket->iMonsterType = pMonster->GetMonsterType();
				g_kMonsterFrameInfoPacket->uiHP = (UINT)( pMonster->m_fHP );
				g_kMonsterFrameInfoPacket->usBodyAngle = (USHORT)( pMonster->Body_Angle * 10000.0f );
				g_kMonsterFrameInfoPacket->cTargetIndex = (char)(pMonster->IsTargetMonster() ? pMonster->GetMonsterTarget() + MAX_PLAYER_NUM : pMonster->GetPlayerTarget() );
				g_kMonsterFrameInfoPacket->cLastAttackedMe = (char)pMonster->m_iLastAttackedMe;
                g_kMonsterFrameInfoPacket->iStageID = g_kEveryTriggerManager.GetActiveStage();

				for( int j = SHOWLATENCY; j < ( LATENCY + SHOWLATENCY ); ++j )
				{
					g_kMonsterFrameInfoPacket->aucMotion[j - SHOWLATENCY] = (UCHAR)pMonster->m_aiMotion[j];
					g_kMonsterFrameInfoPacket->aucFrame[j - SHOWLATENCY] = (UCHAR)pMonster->m_aiFrame[j];

					g_kMonsterFrameInfoPacket->ausX[j - SHOWLATENCY] = GCUTIL_MATH::floatToHalf( pMonster->m_afX[j] );
					g_kMonsterFrameInfoPacket->ausY[j - SHOWLATENCY] = GCUTIL_MATH::floatToHalf( pMonster->m_afY[j] );

					g_kMonsterFrameInfoPacket->abIsRight[j - SHOWLATENCY] = pMonster->m_abIsRight[j];
				}

				pkPacker->Pack( g_kMonsterFrameInfoPacket, sizeof(g_kMonsterFrameInfoPacket) );
			}

//			g_kMonsterLiveInfoPacket->LiveInfo[mit->first] = pMonster->IsLive();
		}

        //g_kMonsterLiveInfoPacket->Type = GC_PID_MONSTER_LIVE_INFO;
        //pkPacker->Pack( g_kMonsterLiveInfoPacket, sizeof(g_kMonsterLiveInfoPacket) );
    }
    if ( pkPacker->GetPackCount() != 0 )
    {
        UINT uiPacketSize = 0;
        pkPacket = pkPacker->GetData( uiPacketSize );

        // TCP Relay서버만을 쓴다면 여기서 한번만 보내겠다. 그리고 reliable, unreliable은 관계가 없다. TCP니까!!
        SendP2PPacket( pkPacket, uiPacketSize, KNC_BROADCAST_NOT_ME, _UNRELIABLE );
    }



    g_kTransNPC->TransNPC.Init();
    for( int i=0;i<MAX_PLAYER_NUM;i++) 
    {
        g_kEffectItem->Eff_Item[i].cWhat = -1;
        g_kEffectItem->Eff_Item[i].sTime = 0;
    }
}


DWORD g_dwPingUID = 0;
DWORD MakePingUID() {
	return ++g_dwPingUID;
}

void AddPing( DWORD dwUID_, DWORD dwTimeStamp_ ) {
	KLocker lock( m_csPingLock );
	g_mapSendedPing.insert( std::map<DWORD,DWORD>::value_type( dwUID_, dwTimeStamp_ ) );
}

bool RemovePing( DWORD dwUID_ ) {
	KLocker lock( m_csPingLock );
	if( g_mapSendedPing.find( dwUID_ ) == g_mapSendedPing.end() )
		return false;

	g_mapSendedPing.erase( dwUID_ );
	return true;
}

void SendPingToTCPRelay()
{
	KGC_PID_RELAY_PING kPacket;
	kPacket.dwUID = MakePingUID();
	kPacket.dwTimeStamp = ::timeGetTime();
	kPacket.ucServerType = KGC_PID_RELAY_PING::TCP_RELAY;
	AddPing( kPacket.dwUID, kPacket.dwTimeStamp );

	SendP2PPacket( &kPacket, sizeof( kPacket), KNC_TCP_RELAY_TO_ME, _RELIABLE );
}

void SendPingToUDPRelay()
{
	KGC_PID_RELAY_PING kPacket;
	kPacket.dwUID = MakePingUID();
	kPacket.dwTimeStamp = ::timeGetTime();
	kPacket.ucServerType = KGC_PID_RELAY_PING::UDP_RELAY;
	AddPing( kPacket.dwUID, kPacket.dwTimeStamp );

	SendP2PPacket( &kPacket, sizeof( kPacket), KNC_RELAY_TO_ME, _RELIABLE );
}

void CheckPing()
{
    return;
#if defined(NATION_IDN)
    if( g_eP2PConMode == P2P_TCP_RELAY_ONLY ) {
        if( KP2P::GetInstance()->m_spTRUserProxy->PingContinuousRTOCount() >= (DWORD)g_iPingOutCountLimit && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN ) {

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_BAD_CONNECTION_USER );
            g_MyD3D->m_pStateMachine->OnExitGame();
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NETWORK_TOO_SLOW ) );
        }
    }
    else {
        std::map<DWORD,DWORD>::iterator mit;
        std::map<DWORD,DWORD> mapTemp;
        {
            KLocker lock( m_csPingLock );
            mapTemp = g_mapSendedPing;
        }

        for( mit = mapTemp.begin() ; mit != mapTemp.end() ; ++mit ) {
            DWORD dwPing = ::timeGetTime() - mit->second;

            if( dwPing > g_dwPingLimit ) {
                RemovePing( mit->first );

                if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) {
                    ++g_iPingOutCount;
                }
                else {
                    g_iPingOutCount = 0;
                }
            }
        }

        if( g_iPingOutCount >= g_iPingOutCountLimit && g_kGlobalValue.m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN ) {
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_BAD_CONNECTION_USER );
            g_MyD3D->m_pStateMachine->OnExitGame();
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NETWORK_TOO_SLOW ) );
            {
                KLocker lock( m_csPingLock );
                g_mapSendedPing.clear();
            }
            g_iPingOutCount = 0;
        }
    }
#endif
}


void ON_GC_PID_CHANGE_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_CHANGE_MONSTER* Custom = (KGC_PID_CHANGE_MONSTER*)pRecvData_;
	MONSTER* pMonster = g_kMonsterManager.GetMonster( Custom->usMonIndex );
	if ( pMonster ) {
		pMonster->ChangeMonster( Custom->kChangeMon );
	}

}

void ON_GC_PID_WEAPONCHANGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_WEAPONCHANGE* Custom = (KGC_PID_WEAPONCHANGE*)pRecvData_;

    //#define CHANGE_MASTER 888
    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( Custom->dwUID );
    if( pPlayer )
    {
        if( pPlayer->IsLocalPlayer() )
        {
            float fBuffTime = 10.0f;
            //if ( g_MyD3D->m_kItemMgr.FindInventoryForItemID( CHANGE_MASTER ) )
            {
                fBuffTime = 1.0f;
            }

            if( pPlayer->m_bIsBurnningMode )
                pPlayer->ChangeBurnningTemplate();

            g_pMagicEffect->SetMagicEffect( pPlayer->m_iPlayerIndex, EGC_EFFECT_WEAPONCHANGE, fBuffTime );
        }

        pPlayer->AddDamage( DT_WEAPONCHANGE_EFFECT, 0 );
        pPlayer->ChangePromotionTemplate();
    }
}

void ON_GC_PID_BROAD_DUNGEON_PLAYSTATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DUNGEON_PLAYSTATE *kPacket = (KGC_PID_BROAD_DUNGEON_PLAYSTATE*)pRecvData_;

    if ( kPacket )
    {
#if defined( DUNGEONPLAYSTATE )
        g_kGlobalValue.AddDungeonPlayState( kPacket->iState );
#endif
    }
}

void ON_GC_PID_BROAD_CAMERAZOOM_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_CAMERAZOOM_STATE *kPacket = (KGC_PID_BROAD_CAMERAZOOM_STATE*)pRecvData_;

    if ( kPacket )
    {
#if defined( USE_CAMERA_ZOOM_STATE )
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
        if( pPlayer )
        {
            pPlayer->AddCameraZoomState( kPacket->fBefore, kPacket->fAfter );
        }
#endif
    }
}

void ON_GC_PID_BROAD_ESCORT_MONSTER_DESC( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_ESCORT_MONSTER_DESC* kPacket = (KGC_PID_BROAD_ESCORT_MONSTER_DESC*)pRecvData_;

    if( kPacket ) { 
        g_pkGameOverlayUI->ShowEscortMerryDesc( kPacket->iStringID, kPacket->iShowFrame );

        if( kPacket->bWarning ) { 
            g_pkGameOverlayUI->SetEscortMonHPBarWarning();
        }
    }
}

void   ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER *Custom = (KGC_PID_BROAD_SUMMON_DAMAGEMONSTER*)pRecvData_;

    if( false == g_kMonsterManager.IsMonsterIndex(Custom->m_iSlot) )
    {
		g_kMonsterManager.CreateMonster(Custom->m_iSlot);
    }

    float x = GCUTIL_MATH::halfToFloat( Custom->m_usEntranceX );
    float y = GCUTIL_MATH::halfToFloat( Custom->m_usEntranceY );

    g_MyD3D->AddMonster( Custom->m_usMonType, x, y, false, false, 0, Custom->m_iLevel, 0, true, false, Custom->m_iSlot );

	MONSTER* pMon = g_kMonsterManager.GetMonster( Custom->m_iSlot );

	if(pMon == NULL)
		return;

    pMon->m_bLive = true;
    pMon->m_pObject->SetRender( true );
    pMon->SetState( Custom->m_szState );

	//추후 캐릭터 스킬에 몬스터로 변신시키는 스킬이 들어가게 되면 아래 몬스터 고정으로 박아놓은 부분 유동적으로 수정해주세요~!
	pMon->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;

    // Hit플레이어 셋팅
    if( Custom->m_bCharLock && g_MyD3D->IsPlayerIndex( Custom->m_iHitPlayerIdx ) )
    {
        // 카메라 셋팅
        PLAYER *pPlayer = g_MyD3D->MyPlayer[Custom->m_iHitPlayerIdx];
        pPlayer->SetDamageSummonMonSlot( Custom->m_iSlot );
        if( pPlayer->IsLocalPlayer() )
        {
            g_kCamera.SetTargetMonster( Custom->m_iSlot );
        }

        // 캐릭터 Lock처리
        g_MyD3D->MyPlayer[Custom->m_iHitPlayerIdx]->SetLock( true );

		for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			if( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
				continue;

			if( mit->second->m_bLive && 
				mit->second->m_fHP > 0.0f && 
				mit->second->GetPlayerTarget() == Custom->m_iHitPlayerIdx )
				mit->second->ReleaseTarget();
		}
    }
}

void   ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER *kPacket = (KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER*)pRecvData_;

    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
    if( pPlayer )
    {
        // 소환 마릿수 제한
        pPlayer->SummonMonsterLimit( kPacket->iSummonLimit, kPacket->MonsterID );


		int iSlotID = g_MyD3D->AddMonster( kPacket->MonsterID, kPacket->vecSummonPos.x, kPacket->vecSummonPos.y, kPacket->IsRight, false, 0, kPacket->iLevel, 0, false, false, kPacket->iSlotID);

		MONSTER *pMonster = g_kMonsterManager.GetMonster(iSlotID);
		if ( pMonster == NULL ) { 
			g_kMonsterManager.CreateMonster(iSlotID);
			pMonster = g_kMonsterManager.GetMonster(iSlotID);
		}

        // 몬스터 소환
        pMonster->m_iTeam = kPacket->iTeam;
		pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
        pMonster->m_dwOwnerUID = kPacket->dwUID;
        pMonster->m_bLive = true;
        pMonster->m_bRender = true;
        pMonster->ToggleMonsterInfo( true );

        // 플레이어와 몬스터 간 제한거리 설정
        pPlayer->SetSummonMonsterLimitDist( kPacket->fLimitDistance );
        
        // 몬스터 State셋팅
        pMonster->SetState( kPacket->szState );

        // Hit플레이어 셋팅
        if( g_MyD3D->IsPlayerIndex( kPacket->iHitPlayerIdx ) )
        {
            PLAYER *pPlayer = g_MyD3D->MyPlayer[kPacket->iHitPlayerIdx];
            pPlayer->SetDamageSummonMonSlot( pMonster->m_iMonsterIndex );
            if( pPlayer->IsLocalPlayer() )
            {
                g_kCamera.SetTargetMonster( pMonster->m_iMonsterIndex );
            }
        }
    }
}

void ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_END *kPacket = (KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_END*)pRecvData_;

    if( g_MyD3D->IsPlayerIndex(kPacket->iHitPlayerIdx) )
    {
        PLAYER *pPlayer = g_MyD3D->MyPlayer[kPacket->iHitPlayerIdx];
        if( pPlayer )
        {
            int iMonSlot = pPlayer->GetDamageSummonMonSlot();
			MONSTER* pMonster = g_kMonsterManager.GetMonster( iMonSlot);
			if ( pMonster ) {
				pMonster->SetHP( 0.0f );
                pMonster->SetLive( false );
				pMonster->EndMonster( true, true );
			}


            pPlayer->Change_Motion( pPlayer->GetCurFormTemplate().WAIT );
            pPlayer->SetLock( false );
            if( pPlayer->IsLocalPlayer() )
            {
                g_kCamera.SetTargetPlayer( pPlayer->m_iPlayerIndex );
                g_kCamera.SetTargetMonster( -1 );
            }

            pPlayer->SetDamageSummonMonSlot( -1 );
        }
    }
}

void ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END *kPacket = (KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END*)pRecvData_;

    if( g_MyD3D->IsPlayerIndex(kPacket->iHitPlayerIdx) )
    {
        PLAYER *pPlayer = g_MyD3D->MyPlayer[kPacket->iHitPlayerIdx];
        if( pPlayer )
        {
            pPlayer->SetLock( false );
            pPlayer->SetHP( 0.0f );
            pPlayer->Change_Motion( MID_COMMON_DOWN_AND_STANDUP );
            if( pPlayer->IsLocalPlayer() )
            {
                g_kCamera.SetTargetPlayer( pPlayer->m_iPlayerIndex );
                g_kCamera.SetTargetMonster( -1 );
            }

            pPlayer->SetDamageSummonMonSlot( -1 );
        }
    }
}

void ON_GC_PID_BROAD_ADD_DAMAGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_ADD_DAMAGE *kPacket = (KGC_PID_BROAD_ADD_DAMAGE*)pRecvData_;

    if( kPacket->bMonster )
    {
        MONSTER *pMonster = g_kMonsterManager.GetMonster( kPacket->iPlayerIndex );
        if( NULL == pMonster )
            return;

        switch( kPacket->iAddDamageType )
        {
        case 0:
            pMonster->AddDamage( kPacket->iDamageEnum );
            break;
        case 1:
            pMonster->AddDamageWithLocate( kPacket->iDamageEnum, kPacket->vPos.x, kPacket->vPos.y );
            break;
        case 2:
            pMonster->AddDamageWithSpeed( kPacket->iDamageEnum, kPacket->vPos.x, kPacket->vPos.y, kPacket->vSpeed.x, kPacket->vSpeed.y );
            break;
        case 3:
            pMonster->AddDamageWithStaticAndSpeed( kPacket->iDamageEnum, kPacket->vPos.x, kPacket->vPos.y, kPacket->vSpeed.x, kPacket->vSpeed.y );
            break;
        }
    }
    else
    {
        if( false == g_MyD3D->IsPlayerIndex( kPacket->iPlayerIndex ) )
            return;

        switch( kPacket->iAddDamageType )
        {
        case 0:
            g_MyD3D->MyPlayer[kPacket->iPlayerIndex]->AddDamage( kPacket->iDamageEnum, 0 );
            break;
        case 1:
            g_MyD3D->MyPlayer[kPacket->iPlayerIndex]->AddDamageWithStaticLocate( kPacket->iDamageEnum, kPacket->vPos.x, kPacket->vPos.y );
            break;
        }

    }
}

void ON_GC_PID_BROAD_SYNCOBJECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_SYNCOBJECT *kPacket = (KGC_PID_BROAD_SYNCOBJECT*)pRecvData_;

    switch( kPacket->iSyncMode )
    {
    case KGCSyncObjectManager::SYNC_MODE_ADD:
        SiKGCSyncObjectManager()->AddSyncObject_Packet_Private( kPacket->dwUID, kPacket->iObjectID, kPacket->fX, kPacket->fY );
        break;
    case KGCSyncObjectManager::SYNC_MODE_DEL:
        SiKGCSyncObjectManager()->DelSyncObject_Packet_Private( kPacket->dwUID );
        break;
    }
}

void ON_GC_PID_BROAD_RENDER_HIT_NUMBER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_RENDER_HIT_NUMBER *kPacket = (KGC_PID_BROAD_RENDER_HIT_NUMBER*)pRecvData_;

    g_MyD3D->MyHead->Add_HP_Changer( kPacket->iUnitID, kPacket->iUnitID, kPacket->fNumber, kPacket->bMPDamage, CString(), 1, kPacket->fHitPosX, kPacket->fHitPosY );
}

void ON_GC_PID_BROAD_EMBARK_FRAME_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARK_FRAME_INFO *kPacket = (KGC_PID_BROAD_EMBARK_FRAME_INFO*)pRecvData_;

    KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( kPacket->dwEmbarkUID );
    if( pEmbark )
    {
        pEmbark->SetFrameData( kPacket );
    }
}

void ON_GC_PID_BROAD_EMBARKATION_REQ( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARKATION_REQ *kRecvPacket = (KGC_PID_BROAD_EMBARKATION_REQ*)pRecvData_;

    if( false == g_kGlobalValue.m_kUserInfo.bHost )
        return;

    KGCEmbarkObject *pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( kRecvPacket->dwEmbarkUID );
    if( NULL == pEmbark )
        return;

    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( kRecvPacket->dwUID );
    if( NULL == pPlayer )
        return;

    if( kRecvPacket->bEmbarkation )
    {
        if( pEmbark->IsEmbarkation() )
            return;

        if( false == pEmbark->SetUpEmbarkation( kRecvPacket->dwUID, pEmbark->GetUpCharMotion(), pEmbark->GetUpCallback() ) )
            return;

        pPlayer->SetEmbarkAdaptBoneID( kRecvPacket->iBoneID );

        KGC_PID_BROAD_EMBARKATION_ACK kSendPacket;
        kSendPacket.dwUID = kRecvPacket->dwUID;
        kSendPacket.dwEmbarkUID = kRecvPacket->dwEmbarkUID;
        kSendPacket.bEmbarkation = true;
        kSendPacket.iBoneID = kRecvPacket->iBoneID;
        SendP2PPacket( &kSendPacket, sizeof(kSendPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_EMBARKATION_ACK" );
    }
    else
    {
        if( false == pEmbark->SetDownEmbarkation( kRecvPacket->dwUID, pEmbark->GetDownCharMotion(), pEmbark->GetDownCallback() ) )
            return;

        pPlayer->SetEmbarkAdaptBoneID( -1 );

        KGC_PID_BROAD_EMBARKATION_ACK kSendPacket;
        kSendPacket.dwUID = kRecvPacket->dwUID;
        kSendPacket.dwEmbarkUID = kRecvPacket->dwEmbarkUID;
        kSendPacket.bEmbarkation = false;
        kSendPacket.iBoneID = kRecvPacket->iBoneID;
        SendP2PPacket( &kSendPacket, sizeof(kSendPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_EMBARKATION_ACK" );
    }
}

void ON_GC_PID_BROAD_EMBARKATION_ACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARKATION_ACK *kPacket = (KGC_PID_BROAD_EMBARKATION_ACK*)pRecvData_;

    if( g_kGlobalValue.m_kUserInfo.bHost )
        return;

    KGCEmbarkObject *pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( kPacket->dwEmbarkUID );
    if( NULL == pEmbark )
        return;

    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( kPacket->dwUID );
    if( NULL == pPlayer )
        return;

    if( kPacket->bEmbarkation )
    {
        pEmbark->SetUpEmbarkation( kPacket->dwUID, pEmbark->GetUpCharMotion(), pEmbark->GetUpCallback() );
        pPlayer->SetEmbarkAdaptBoneID( kPacket->iBoneID );
    }
    else
    {
        pEmbark->SetDownEmbarkation( kPacket->dwUID, pEmbark->GetDownCharMotion(), pEmbark->GetDownCallback() );
        pPlayer->SetEmbarkAdaptBoneID( -1 );
    }
}

void ON_GC_PID_BROAD_EMBARK_SUMMON_REQ( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARK_SUMMON_REQ *kPacket = (KGC_PID_BROAD_EMBARK_SUMMON_REQ*)pRecvData_;

    if( false == g_kGlobalValue.m_kUserInfo.bHost )
        return;

    KGC_PID_BROAD_EMBARK_SUMMON_ACK kSendPacket;
    kSendPacket.dwPlayerUID = kPacket->dwPlayerUID;
    kSendPacket.dwEmbarkUID = SiKGCEmbarkManager()->GetCountUIDWithAutoCount();
    kSendPacket.iEmbarkType = kPacket->iEmbarkType;
    kSendPacket.fPosX = kPacket->fPosX;
    kSendPacket.fPosY = kPacket->fPosY;
    kSendPacket.bIsRight = kPacket->bIsRight;
    kSendPacket.bEmbakation = kPacket->bEmbakation;
    SendP2PPacket( &kSendPacket, sizeof(kSendPacket), KNC_BROADCAST, _RELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_EMBARK_SUMMON_ACK" );
}

void ON_GC_PID_BROAD_EMBARK_SUMMON_ACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
     KGC_PID_BROAD_EMBARK_SUMMON_ACK *kPacket = (KGC_PID_BROAD_EMBARK_SUMMON_ACK*)pRecvData_;

    SiKGCEmbarkManager()->SummonEmbark( kPacket );
}

void ON_GC_PID_BROAD_EMBARK_DELETE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARK_DELETE *kPacket = (KGC_PID_BROAD_EMBARK_DELETE*)pRecvData_;

    if( UINT_MAX == kPacket->dwEmbarkUID )
    {
        SiKGCEmbarkManager()->DestoryAllInstance();
    }
    else
    {
		KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( kPacket->dwEmbarkUID );
		if( pEmbark )
			pEmbark->EndEmbark( 0 );
    }
}

void ON_GC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST *kPacket = (KGC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST*)pRecvData_;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        MONSTER *pMonster = g_kMonsterManager.GetMonster( kPacket->iSlotID );
        if( pMonster )
        {
            pMonster->SetHP( kPacket->fHP );
        }
    }
}

void ON_GC_PID_BROAD_EMBARK_IN_PORTAL( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARK_IN_PORTAL *kPacket = (KGC_PID_BROAD_EMBARK_IN_PORTAL*)pRecvData_;

    /* 이 패킷은 탑승물을 탑승한 채로 이동포탈을 탈 수 있게 해준다!!!
    //
    // 1. 캐릭터에게 탑승물 타입을 셋팅해주어 포탈에서 나오거나 다음 스테이지로 이동했을때 탑승상태로 만들어줄 수 있도록 하자.
    // 2. 탑승물을 죽이자. 그러면 자동으로 하차처리가 될 것이다.
    // 3. 바로 캐릭터는 일반적인 포탈를 탄거처럼 처리해준다.
    // 4. ★★★단, 하차시 특별한 액션을 해야하는 탑승물이나 캐릭터가 있을 경우 문제가 될 수 있다..( 그 특별한 동작을 수행하지 못한다. )
    */

    KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( kPacket->dwEmbarkUID );
    if( pEmbark )
    {
        PLAYER *pPlayer = pEmbark->GetEmbarkationPlayer();
        if( NULL == pPlayer )
            return;

        pPlayer->SetEmbarkTypeOfInPortal( pEmbark->GetEmbarkType() );
        pPlayer->SetEmbarkUIDOfInPortal( pEmbark->GetEmbarkUID() );
        pEmbark->EndEmbark( 0 );

        pPlayer->ProcessMetamorphosisNormalForm( false );
        pPlayer->vPos.x = kPacket->fPosX;
        pPlayer->x_Speed=0.00001f;
        pPlayer->IsContact = true;
        pPlayer->cFrame=0;
        pPlayer->NoCheckContact = 0;
        pPlayer->SetPlayerState(MID_COMMON_WAIT_TELEPORT);
    }
}

void ON_GC_PID_BROAD_DELETE_DAMAGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_DELETE_DAMAGE *kPacket = (KGC_PID_BROAD_DELETE_DAMAGE*)pRecvData_;

    std::map< int, std::vector<CDamageInstance*> >& mapDamageInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapDamageInstance.begin();
    for(; mapIter != mapDamageInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter)
        {
            if( ((*vecIter)->m_What == kPacket->iWhat) &&
                ((*vecIter)->m_dwIdentifie == kPacket->dwIdentifie) &&
                ((*vecIter)->m_IsMonsterDamage == kPacket->bIsMonsterDamage) )
            {
                bool bDelete = false;
                if( kPacket->bIsMonsterDamage )
                {
                    if( (*vecIter)->m_ExtraMonsterNum == kPacket->iWho )
                        bDelete = true;
                }
                else
                {
                    if( (*vecIter)->m_Who == kPacket->iWho )
                        bDelete = true;
                }

                if( bDelete )
                {
                    (*vecIter)->m_Life = 1;
                    (*vecIter)->m_dwIdentifie = 0;
                    return;
                }
            }
        }
    }
}

void ON_GC_PID_BROAD_EMBARK_SYNC_CALLBACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARK_SYNC_CALLBACK *kPacket = (KGC_PID_BROAD_EMBARK_SYNC_CALLBACK*)pRecvData_;

    KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( kPacket->dwEmbarkUID );
    if( pEmbark )
    {
        pEmbark->SetTempValue( "SYNC", kPacket->fValue );
        pEmbark->CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_SYNC );
    }
}

void ON_GC_PID_BROAD_EMBARK_SYNC_UID( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_BROAD_EMBARK_SYNC_UID *kPacket = (KGC_PID_BROAD_EMBARK_SYNC_UID*)pRecvData_;

    SiKGCEmbarkManager()->SetCountUID( kPacket->dwEmbarkUIDCnt, true );
}

int GetRelayPacketUser()
{
    int nPlayer = 0;

    for ( int  i = 0; i < MAX_PLAYER_NUM + NUM_TEMP_PLAYERINFO; i++ )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
        {
            nPlayer++;
        }
    }
   
    if ( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() )
    {
        std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;
        for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
        {
            nPlayer++;
        }
    }

    return nPlayer;
}

void ON_GC_PID_BROAD_CONTROL_MISSION_DATA( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    int Index;
    // 플레이어의 인덱스가 유효한지 확인한다.
    if ( ( Index = g_MyD3D->Get_Player( dwUID_ ) ) >= MAX_PLAYER_NUM ) 
    {
        return;
    }

    KGC_PID_BROAD_CONTROL_MISSION_DATA kPacket;
    DESERIALIZE_P2P_PACKET( kPacket );

    g_MyD3D->MyPlayer[Index]->m_iControlInfoComboCount      = kPacket.m_iControlInfoComboCount;
    g_MyD3D->MyPlayer[Index]->m_iControlInfoBackAttackCount = kPacket.m_iControlInfoBackAttackCount;
    g_MyD3D->MyPlayer[Index]->m_iControlInfoBeAttackedCount = kPacket.m_iControlInfoBeAttackedCount;
    g_MyD3D->MyPlayer[Index]->m_mapControlInfoSkill         = kPacket.m_mapControlInfoSkill;
    g_MyD3D->MyPlayer[Index]->m_mapControlInfoPotion        = kPacket.m_mapControlInfoPotion;
}

void ON_GC_PID_SUMMON_INFINITY_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ )
{
    KGC_PID_SUMMON_INFINITY_MONSTER *Custom = (KGC_PID_SUMMON_INFINITY_MONSTER*)pRecvData_;

    MONSTER *pMonster = g_kMonsterManager.GetMonster( Custom->m_usMonIndex );
    if( pMonster && pMonster->IsLive() )
    {
        pMonster->EndMonster( true, true, true );
    }

    float x = GCUTIL_MATH::halfToFloat( Custom->m_usEntranceX );
    float y = GCUTIL_MATH::halfToFloat( Custom->m_usEntranceY );

    g_MyD3D->AddMonster( Custom->m_usMonType, x, y, Custom->m_bIsRight, false, Custom->m_bBoss, Custom->m_iLevel, 0, Custom->m_bCheckSafePostion, false, Custom->m_usMonIndex );

    MONSTER* pMon = g_kMonsterManager.GetMonster( Custom->m_usMonIndex );

    if ( pMon == NULL ) return;

    pMon->m_bLive = true;
    pMon->m_pObject->SetRender( true );
    pMon->SetCountValue( Custom->m_iCountValue );
    pMon->SetState( Custom->m_szState );
    pMon->SetTriggerID( Custom->m_usTGid );
    pMon->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_TRIGGER;
    pMon->m_iBossMonster = Custom->m_bBoss ? MONSTER::MON_BOSS : MONSTER::MON_JAKO ;
}
