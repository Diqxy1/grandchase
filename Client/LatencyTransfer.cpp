#include "stdafx.h"
// LatencyTransfer.cpp: implementation of the CLatencyTransfer class.
//
//////////////////////////////////////////////////////////////////////


//
//


#include "LatencyTransfer.h"

#include "./gcui/GCGameOverlayUI.h"
#include "DamageManager.h"
#include "Spark.h"

#include "Message.h"
#include "Buff/KGCBuffManager.h"

#include "PetAI.h"
#include "Stage.h"
#include "GCUI/GCHeadupDisplay.h"

#include "GCStateMachine.h"

#include "Replay.h"
#include "KGCRoomManager.h"
#include "KGCEmoticon.h"
#include "gcui/GCMyHeadup.h"
#include "KGCJungSu.h"
#include "KGCMonsterBuff.h"
#include "KGCMatch.h"

CLatencyTransfer g_LatencyTransfer;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _PACKET_PROFILE_
struct LTPacketProfile
{
    UINT uiCount;
    UINT uiSize;
    char strID[256];
};
KSafeArray<LTPacketProfile,ELTP_END> g_kSendPacket;
KSafeArray<LTPacketProfile,ELTP_END> g_kRecvPacket;
#endif
CLatencyTransfer::CLatencyTransfer()
{	
#ifdef _PACKET_PROFILE_
    memset( g_kSendPacket, 0, sizeof(LTPacketProfile)*ELTP_END );
    memset( g_kRecvPacket, 0, sizeof(LTPacketProfile)*ELTP_END );
#endif
}

CLatencyTransfer::~CLatencyTransfer()
{
	for( int i = 0; i < (int)m_LatencyPacketList.size(); i++ ) 
	{
		SAFE_DELETE( m_LatencyPacketList[i] );
	}
	m_LatencyPacketList.clear();
#ifdef _PACKET_PROFILE_
    FILE* pFile = fopen( "LTPacketProfile.txt", "w" );
    char strProfile[1024] = {0,};
    for ( int i = 0; i < ELTP_END; i++ )
    {
        sprintf( strProfile, "%s : Send( %u, %uk ), Recv( %u, %uk )\n", g_kSendPacket[i].strID, g_kSendPacket[i].uiCount,
                 g_kSendPacket[i].uiSize/1024, g_kRecvPacket[i].uiCount, g_kRecvPacket[i].uiSize/1024 );
        fwrite( strProfile, sizeof(char), strlen( strProfile ), pFile );
    }
    fclose( pFile );
#endif
}

void CLatencyTransfer::FrameMove( float fElapsedTime )
{
    PROFILE_SET("CLatencyTransfer::FrameMove");

	for( int i = 0; i < (int)m_LatencyPacketList.size(); i++ ) 
	{
		m_LatencyPacketList[i]->latencyTime -= fElapsedTime;
		if( m_LatencyPacketList[i]->latencyTime <= 0.0f )
		{
			PacketHandler( m_LatencyPacketList[i] );

			SAFE_DELETE( m_LatencyPacketList[i] );
			m_LatencyPacketList.erase( m_LatencyPacketList.begin()+i );
			i--;
		}
	}
}
#ifdef _PACKET_PROFILE_
#define             LTPACKET_CASE( ID )                 case(ID):                                   \
                                                        g_kSendPacket[ID].uiCount++;                \
                                                        g_kSendPacket[ID].uiSize += packet->size;   \
                                                        if ( strlen( g_kSendPacket[ID].strID )== 0 )\
                                                        {                                           \
                                                            strcpy( g_kSendPacket[ID].strID, #ID);  \
                                                        }                                           \
                                                        break
#define             _LTPACKET_CASE( ID )                case(ID):                                   \
                                                        g_kRecvPacket[ID].uiCount++;                \
                                                        g_kRecvPacket[ID].uiSize += packet->size;   \
                                                        if ( strlen( g_kRecvPacket[ID].strID )== 0 )\
                                                        {                                           \
                                                            strcpy( g_kRecvPacket[ID].strID, #ID);  \
                                                        }                                           \
                                                        break
#endif
void CLatencyTransfer::PushPacket( LTP_BASIC* packet, int iSendType )
{
	packet->dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
	packet->Type = GC_PID_LATENCY_TRANSFER;

	SendNetworkData( packet->Type, iSendType, packet );
#ifdef _PACKET_PROFILE_
    switch( packet->packetType )
    {
        LTPACKET_CASE( ELTP_BASIC );
        LTPACKET_CASE( ELTP_QUEST_PROGRESS );
        LTPACKET_CASE( ELTP_QUEST_STAGE_FIREBALL );
        LTPACKET_CASE( ELTP_AI_SPACE_JUMP );
        LTPACKET_CASE( ELTP_AI_MOVE );
        LTPACKET_CASE( ELTP_AI_MOVE_PACK );
        LTPACKET_CASE( ELTP_PET_AI_MOVE );
        LTPACKET_CASE( ELTP_PET_AI_MOVE_PACK );
        LTPACKET_CASE( ELTP_KAIDO_FIREBALL );
        LTPACKET_CASE( ELTP_FASTPOTAL_MOVE );
        LTPACKET_CASE( ELTP_MONSTER_GENERATE );
        LTPACKET_CASE( ELTP_GAIKOZ_SLASH );
        LTPACKET_CASE( ELTP_KAMIKI_LIGHTNING );
        LTPACKET_CASE( ELTP_KAMIKI_FIRE );
        LTPACKET_CASE( ELTP_KAMIKI_ICE );
        LTPACKET_CASE( ELTP_ELLENA_MOVE );
        LTPACKET_CASE( ELTP_MAGICEFF );
        LTPACKET_CASE( ELTP_QUEST_TIME );
        LTPACKET_CASE( ELTP_GAME_ARENA );
        LTPACKET_CASE( ELTP_ENT_FRUIT );
        LTPACKET_CASE( ELTP_MAGIC_DAMAGE_SYNCH );		
		LTPACKET_CASE( ELTP_INSECTIVOROUS_BREEDING );
		LTPACKET_CASE( ELTP_INSECTIVOROUS_SPIT );
		LTPACKET_CASE( ELTP_GOLEM_TOKEN );
        LTPACKET_CASE( ELTP_EMOTICON );
        LTPACKET_CASE( ELTP_DAMAGE );
        LTPACKET_CASE( ELTP_JUNGSU_REQ );
        LTPACKET_CASE( ELTP_JUNGSU_ACK );
        LTPACKET_CASE( ELTP_TITLE_STRING );
        LTPACKET_CASE( ELTP_BUFF );
        LTPACKET_CASE( ELTP_BUILD_PORTAL );
        LTPACKET_CASE( ELTP_CHANGE_HP_INFO );
        LTPACKET_CASE( ELTP_CHANGE_OBJECT_HP_INFO );
        LTPACKET_CASE( ELTP_AGIT_OBJECT_POS );
        LTPACKET_CASE( ELTP_AGIT_DICE_OBJECT_STATE );
        LTPACKET_CASE( ELTP_AGIT_FLAG_OBJECT_STATE );
        LTPACKET_CASE( ELTP_CHANGE_PLAYER_HP );
        LTPACKET_CASE( ELTP_MATCH_QUICK_MODE );
    }
#endif
}

void CLatencyTransfer::SendMagicEffectSynchPacket(int iMagicEffect, float fTime, bool bIsDamageUnitPlayer, int iDamageUnit, int iMagicLevel, DWORD dwAttackUnit )
{
	LTP_MAGIC_DAMAGE_SYNCH packet;
	packet.iMagicEffect = iMagicEffect;
//	packet.bIsAttakeUnitPlayer = bIsAttackUnitPlayer;
	packet.dwAttakeUnit = dwAttackUnit;
	packet.bIsDamagedUnitPlayer = bIsDamageUnitPlayer;
	packet.iDamagedUnit = iDamageUnit;
	packet.fTime = fTime;
    packet.iMagicLevel = iMagicLevel;
    packet.latencyTime = GC_ELAPSED_TIME;
	PushPacket(&packet, 1);
}

int CLatencyTransfer::ClearGamePacket( )
{
	int iDeletePacketCount = 0;
	iDeletePacketCount += ClearPacket( ELTP_QUEST_STAGE_FIREBALL );
	iDeletePacketCount += ClearPacket( ELTP_AI_SPACE_JUMP );
	iDeletePacketCount += ClearPacket( ELTP_AI_MOVE );
	iDeletePacketCount += ClearPacket( ELTP_AI_MOVE_PACK );
	iDeletePacketCount += ClearPacket( ELTP_PET_AI_MOVE );
	iDeletePacketCount += ClearPacket( ELTP_PET_AI_MOVE_PACK );
	iDeletePacketCount += ClearPacket( ELTP_KAIDO_FIREBALL );
	iDeletePacketCount += ClearPacket( ELTP_FASTPOTAL_MOVE );
	iDeletePacketCount += ClearPacket( ELTP_MONSTER_GENERATE );
	iDeletePacketCount += ClearPacket( ELTP_GAIKOZ_SLASH );
	iDeletePacketCount += ClearPacket( ELTP_KAMIKI_LIGHTNING );
	iDeletePacketCount += ClearPacket( ELTP_KAMIKI_FIRE );
	iDeletePacketCount += ClearPacket( ELTP_KAMIKI_ICE );
	iDeletePacketCount += ClearPacket( ELTP_ELLENA_MOVE );
	iDeletePacketCount += ClearPacket( ELTP_MAGICEFF );
	iDeletePacketCount += ClearPacket( ELTP_QUEST_TIME );
	iDeletePacketCount += ClearPacket( ELTP_GAME_ARENA );
	iDeletePacketCount += ClearPacket( ELTP_ENT_FRUIT );
	iDeletePacketCount += ClearPacket( ELTP_MAGIC_DAMAGE_SYNCH );	
	iDeletePacketCount += ClearPacket( ELTP_INSECTIVOROUS_BREEDING );	
	iDeletePacketCount += ClearPacket( ELTP_INSECTIVOROUS_SPIT );	
	iDeletePacketCount += ClearPacket( ELTP_GOLEM_TOKEN );
    iDeletePacketCount += ClearPacket( ELTP_EMOTICON );
    iDeletePacketCount += ClearPacket( ELTP_DAMAGE );
    iDeletePacketCount += ClearPacket( ELTP_JUNGSU_REQ );
    iDeletePacketCount += ClearPacket( ELTP_JUNGSU_ACK );
    iDeletePacketCount += ClearPacket( ELTP_TITLE_STRING );
    iDeletePacketCount += ClearPacket( ELTP_BUFF  );
    iDeletePacketCount += ClearPacket( ELTP_BUILD_PORTAL  );
    iDeletePacketCount += ClearPacket( ELTP_CHANGE_HP_INFO  );
    iDeletePacketCount += ClearPacket( ELTP_CHANGE_OBJECT_HP_INFO  );
    iDeletePacketCount += ClearPacket( ELTP_AGIT_OBJECT_POS );
    iDeletePacketCount += ClearPacket( ELTP_AGIT_DICE_OBJECT_STATE );
    iDeletePacketCount += ClearPacket( ELTP_AGIT_FLAG_OBJECT_STATE );
    iDeletePacketCount += ClearPacket( ELTP_CHANGE_PLAYER_HP );
    iDeletePacketCount += ClearPacket( ELTP_MATCH_QUICK_MODE );    
    iDeletePacketCount += ClearPacket( ELTP_PLAYER_QUEST_RANK );

	return iDeletePacketCount;
}

int CLatencyTransfer::ClearPacket(LT_PACKET_LIST Type )
{
	int iDeletePacketCount = 0;
	for(int i = (int)m_LatencyPacketList.size()-1; i >=0; --i)
	{
		LTP_BASIC* pPacket = m_LatencyPacketList[i];
		if( pPacket->packetType == Type )
		{
            char* pPacket_ = (char*)pPacket;
            SAFE_DELETE_ARRAY( pPacket_ );
			m_LatencyPacketList.erase(m_LatencyPacketList.begin()+i);
			iDeletePacketCount++;
		}
	}
	return iDeletePacketCount;
}

void CLatencyTransfer::PacketHandler( LTP_BASIC* packet )
{
    if ( packet == NULL )
        return;
    
	// 게임에서만 패킷 처리 합니다.
    static int s_iGCState = 0;
    s_iGCState = (int)g_MyD3D->m_pStateMachine->GetState();
    switch( s_iGCState )
    {
        case GS_GAME:
        {
            if ( g_pStateGame->m_bOnExit )
            {
                return;
            }
            break;
        }
        case GS_GAME_LOADING:
        {
            if ( packet->packetType == ELTP_QUEST_PROGRESS )
            {
                packet->latencyTime = 0.1f;
                LTP_BASIC* latencyPacket = (LTP_BASIC*)( new char[packet->size] );
                memcpy( latencyPacket, packet, packet->size );
                m_LatencyPacketList.push_back( latencyPacket ); //리스트에 등록하고 나중에 처리한다
            }
            return;
        }
        default:
        {
            return;
        }
    }

	//자기가 보냈고 지연 시간이 있으면
	if( packet->dwUID == g_kGlobalValue.m_kUserInfo.dwUID && packet->latencyTime > 0.0f )
	{
        LTP_BASIC* latencyPacket = (LTP_BASIC*)( new char[packet->size] );
		memcpy( latencyPacket, packet, packet->size );
		m_LatencyPacketList.push_back( latencyPacket ); //리스트에 등록하고 나중에 처리한다
	}
	else
	{
#ifdef _PACKET_PROFILE_
        switch( packet->packetType )
        {
            _LTPACKET_CASE( ELTP_BASIC );
            _LTPACKET_CASE( ELTP_QUEST_PROGRESS );
            _LTPACKET_CASE( ELTP_QUEST_STAGE_FIREBALL );
            _LTPACKET_CASE( ELTP_QUEST_STAGE_TRAP );
            _LTPACKET_CASE( ELTP_AI_SPACE_JUMP );
            _LTPACKET_CASE( ELTP_AI_MOVE );
            _LTPACKET_CASE( ELTP_AI_MOVE_PACK );
            _LTPACKET_CASE( ELTP_PET_AI_MOVE );
            _LTPACKET_CASE( ELTP_PET_AI_MOVE_PACK );
            _LTPACKET_CASE( ELTP_KAIDO_FIREBALL );
            _LTPACKET_CASE( ELTP_FASTPOTAL_MOVE );
            _LTPACKET_CASE( ELTP_MONSTER_GENERATE );
            _LTPACKET_CASE( ELTP_GAIKOZ_SLASH );
            _LTPACKET_CASE( ELTP_KAMIKI_LIGHTNING );
            _LTPACKET_CASE( ELTP_KAMIKI_FIRE );
            _LTPACKET_CASE( ELTP_KAMIKI_ICE );
            _LTPACKET_CASE( ELTP_ELLENA_MOVE );
            _LTPACKET_CASE( ELTP_MAGICEFF );
            _LTPACKET_CASE( ELTP_QUEST_TIME );
            _LTPACKET_CASE( ELTP_GAME_ARENA );
            _LTPACKET_CASE( ELTP_ENT_FRUIT );
            _LTPACKET_CASE( ELTP_MAGIC_DAMAGE_SYNCH );			
			_LTPACKET_CASE( ELTP_INSECTIVOROUS_BREEDING );	
			_LTPACKET_CASE( ELTP_INSECTIVOROUS_SPIT );	
            _LTPACKET_CASE( ELTP_EMOTICON );	
            _LTPACKET_CASE( ELTP_DAMAGE );	
            _LTPACKET_CASE( ELTP_JUNGSU_REQ );
            _LTPACKET_CASE( ELTP_JUNGSU_ACK );
            
        }
#endif
		switch( packet->packetType )
		{
			case ELTP_MAGIC_DAMAGE_SYNCH:
			{
				LTP_MAGIC_DAMAGE_SYNCH* pPacket = (LTP_MAGIC_DAMAGE_SYNCH*)packet;

				//몬스터는 아직 해당사항 없음
				if( !pPacket->bIsDamagedUnitPlayer || false == g_MyD3D->IsPlayerIndex( pPacket->iDamagedUnit ) )
					return;

				g_pMagicEffect->SetMagicEffect( pPacket->iDamagedUnit, pPacket->iMagicEffect, pPacket->fTime, pPacket->iMagicLevel, -1, true, false, pPacket->dwAttakeUnit );

				break;
			}
			case ELTP_GAME_ARENA:
			{
				LTP_ARENA* pPacket = (LTP_ARENA*)packet;

				for(int i = 0; i < MAX_PLAYER_NUM; i++)
				{	
					if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == pPacket->dwUID )
					{
                        g_MyD3D->MyPlayer[i]->ArenaToInitPlayer( pPacket->m_iChar );
						break;
					}
				}
				break;
			}
			case ELTP_BASIC:
            {
                break;
            }            
			case ELTP_PET_AI_MOVE:
			{
				LTP_PET_AI_MOVE* pMovePacket = (LTP_PET_AI_MOVE*)packet;
				g_pkPetAI->PacketMove( pMovePacket );
				break;
			}
            case ELTP_PET_AI_MOVE_PACK:
			{
				LTP_PET_AI_MOVE_PACK* pPetMovePackPacket = (LTP_PET_AI_MOVE_PACK*)packet;
				for( int i = 0; i < pPetMovePackPacket->ucActiveNum; i++ )
				{
					LTP_PET_AI_MOVE movePacket = pPetMovePackPacket->movePacket[i];
					g_pkPetAI->PacketMove( &movePacket );
				}
				break;
			}
			case ELTP_KAIDO_FIREBALL:
			{
				CDamageInstance* fire = NULL;
				LTP_KAIDO_FIREBALL* kaidoFireballPacket = (LTP_KAIDO_FIREBALL*)packet;
				fire = g_MyD3D->m_pDamageManager->Add( DT_FIREBALL, MAX_PLAYER_NUM, false, 0 );
                if( fire != NULL )
                {
				    if( kaidoFireballPacket->bRight == true )
				    {
					    fire->m_X = kaidoFireballPacket->x + 1.1f;
					    fire->m_Y = kaidoFireballPacket->y + 0.3f;
					    fire->m_IsRight = false;
				    }
				    else
				    {
					    fire->m_X = kaidoFireballPacket->x + 0.9f;
					    fire->m_Y = kaidoFireballPacket->y + 0.3f;
					    fire->m_IsRight = true;
				    }
                }
				break;
            }
			case ELTP_MAGICEFF:
			{
				LTP_MAGICEFF_SYNCH* pMagicEff = (LTP_MAGICEFF_SYNCH*)packet;
				switch( pMagicEff->iMagicEffType )
				{
				case 0:		//Healing Effect연출합니다
					{
						for( int j = 0; j < 20; j++ ) 
						{
							float x = pMagicEff->vecPos.x - 1.15f + 0.3f * (float)rand()/RAND_MAX;
							float y = pMagicEff->vecPos.y + 0.3f * (float)rand() / RAND_MAX;
							int Delay = rand() % 10;
							g_MyD3D->MySparks->Add( WATER_PIECE, x, y, Delay, 0.1f + 0.1f * randf(), x, y - 0.4f * randf(), 12.6f * randf() );
						}
					}
					break;
				case 1:		//몬스터 Risen효과
					{
						g_MyD3D->MySparks->Add( MON_RISEN, pMagicEff->vecPos.x - 1.0f, 
							pMagicEff->vecPos.y - 0.1f, 0 );
					}
					break;
				}
				break;
			}
			case ELTP_QUEST_TIME:
			{
				LTP_QUEST_TIME* pQuestTimePacket = (LTP_QUEST_TIME*)packet;
                if ( pQuestTimePacket->bEnd )
                {
                    pQuestTimePacket->fTime = 0.0f;
                }
				g_pkGameOverlayUI->SetQuestTime( pQuestTimePacket->fTime );
                g_pkGameOverlayUI->CheckToToggleOfTimerUI();
				break;
			}
            case ELTP_EMOTICON:
            {
                LTP_EMOTICON* pPacket = (LTP_EMOTICON*)packet;
                KGCEmoticonInstance* pInstance = SiKGCEmoticonManager()->CreateEmoticon( pPacket->iItemID );                
                g_MyD3D->MyPlayer[pPacket->iCharIndex]->SetEmoticon( pInstance );
                SiKGCSocialMotionManager()->SetMotion( pPacket->iItemID, pPacket->iCharIndex );
                break;
            }
            case ELTP_DAMAGE:
            {
                LTP_DAMAGE* pPacket = (LTP_DAMAGE*) packet;
                if( pPacket->bMonsterDamage )
                {
					MONSTER* pMonster = g_kMonsterManager.GetMonster( pPacket->iCharIndex );
					if(pMonster != NULL ) {
						pMonster->AddDamageWithLocate( pPacket->iDamage, pPacket->fx,pPacket->fy,pPacket->bAbsoluePos );
					}
                }
                else
                {
                    if( pPacket->bAbsoluePos )
                    {
                        g_MyD3D->MyPlayer[pPacket->iCharIndex]->AddDamageWithLocateAbsolutePos( pPacket->iDamage, pPacket->fx,pPacket->fy, false);
                    }
                    else
                    {
                        if( pPacket->fx == 0.0f && pPacket->fy == 0.0f )
                        {
                            g_MyD3D->MyPlayer[pPacket->iCharIndex]->AddDamage( pPacket->iDamage,0 );
                        }
                        else
                        {
                            g_MyD3D->MyPlayer[pPacket->iCharIndex]->AddDamageWithLocate( pPacket->iDamage, pPacket->fx,pPacket->fy, false );
                        }
                    }
                    g_pMagicEffect->DamageBuffDestroy( pPacket->iCharIndex, pPacket->iDamage );
                    
                }
                break;
            }
            case ELTP_JUNGSU_REQ:
            {
                if( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
                {
                    LTP_JUNGSU_REQ* pPacket = (LTP_JUNGSU_REQ*)packet;
                    if( g_MyD3D->m_pObject[pPacket->iSlotIndex]->IsLive() )
                    {
                        LTP_JUNGSU_ACK Ack;
                        Ack.iJungSuType = pPacket->iJungSuType;
                        Ack.iSlotIndex = pPacket->iSlotIndex;
                        Ack.iCharIndex = pPacket->iCharIndex;
                        PushPacket( &Ack );

                        g_MyD3D->m_pObject[pPacket->iSlotIndex]->EnableLive( false );
                    }
                }
                break;
            }

            case ELTP_JUNGSU_ACK:
            {
                LTP_JUNGSU_ACK* pPacket = (LTP_JUNGSU_ACK*)packet;
                KGCJungSu* JungSu = (KGCJungSu*)g_MyD3D->m_pObject[pPacket->iSlotIndex];
                JungSu->Activate(pPacket->iCharIndex);
            }
            break;

            case ELTP_TITLE_STRING:
            {
                LTP_TITLE_STRING* pPacket = (LTP_TITLE_STRING*)packet;

                if( pPacket->bSameTeamOnly )
                {
                    if( g_MyD3D->IsMyTeam( pPacket->iCharIndex ) )
                        g_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( pPacket->iStringID ) );
                }
                else
                    g_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( pPacket->iStringID ) );
            }
            break;

            case ELTP_BUFF:
            {
                LTP_BUFF* pPacket = (LTP_BUFF*)packet;
                if(pPacket->bClearBuff == true)
                {                    
                    if( pPacket->iTargetIndex < MAX_PLAYER_NUM )
                    {
                        // 라임끼리 중복된 오오라 사용 시 발생하는 문제 때문에 일단은 하드하게 처리함.....
                        PLAYER* pPlayer = g_MyD3D->GetPlayer( pPacket->iTargetIndex );
                        if( pPlayer && pPlayer->m_kUserInfo.dwUID == g_kGlobalValue.m_kUserInfo.dwUID && pPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME ) {
                            SRangeCheckInfo sRangeCheck = pPlayer->GetRangeCheckInfo();
                            std::set< int >::iterator sit = sRangeCheck.m_setRangeBuff.find( pPacket->iBuffIndex );
                            if( sit == sRangeCheck.m_setRangeBuff.end() ) {
                                if(g_pMagicEffect->IsMagicEffect(pPacket->iTargetIndex,pPacket->iBuffIndex))
                                    g_pMagicEffect->ClearMagicEffect( pPacket->iTargetIndex, pPacket->iBuffIndex );
                            }
                        } else {
                            if(g_pMagicEffect->IsMagicEffect(pPacket->iTargetIndex,pPacket->iBuffIndex))
                                g_pMagicEffect->ClearMagicEffect( pPacket->iTargetIndex, pPacket->iBuffIndex );
                        }                        
                    }
                    else
                    {   
                        if(g_pMagicEffect->IsMagicEffect(pPacket->iTargetIndex,pPacket->iBuffIndex))
                            g_pMagicEffect->ClearMagicEffect( pPacket->iTargetIndex, pPacket->iBuffIndex);
                    }                

                }
                else
                {
                    if( pPacket->iTargetIndex < MAX_PLAYER_NUM )
                    {
                        g_pMagicEffect->SetMagicEffect( pPacket->iTargetIndex, pPacket->iBuffIndex, pPacket->fBuffTime, pPacket->iBuffLv, pPacket->iWho, true, false, pPacket->dwAttackPlayerUnit );
                    }
                    else
                    {   
                        g_pMagicEffect->SetMagicEffect( pPacket->iTargetIndex-MAX_PLAYER_NUM, pPacket->iBuffIndex, pPacket->fBuffTime, pPacket->iBuffLv, pPacket->iWho, false );
                    }                

                }
            }
            break;
            case ELTP_BUILD_PORTAL:
            {
                LTP_BUILD_PORTAL* pPacket = (LTP_BUILD_PORTAL*)packet;
                KGCTeleporter *teleporter = new KGCTeleporter;                                
                teleporter->SetPosX(pPacket->vPos.x + 0.15f );
				if( SiKGCRoomManager()->IsMonsterGameMode() )
					teleporter->SetPosY(pPacket->vPos.y + 0.08f);
				else
					teleporter->SetPosY(pPacket->vPos.y);
                teleporter->Create();
                teleporter->SetNowFrame(34);
                teleporter->SetShowMariTeleportParticle(true);
                teleporter->Enable();
                //teleporter->m_pDamage = g_MyD3D->m_pDamageManager->AddWithLocate( pPacket->iDamage, 0, pPacket->vPos.x, pPacket->vPos.y, true );
                teleporter->SetIndex( pPacket->iObjectIndex );

                if( g_MyD3D->m_pObject[pPacket->iObjectIndex] )
                {
                    g_MyD3D->m_pObject[pPacket->iObjectIndex]->Destroy();
                    SAFE_DELETE(g_MyD3D->m_pObject[pPacket->iObjectIndex]);
                }

                g_MyD3D->m_pObject[pPacket->iObjectIndex] = teleporter;

                teleporter->SetPair( pPacket->cPairID );			
            }
            break;
            case ELTP_CHANGE_HP_INFO:
                {
                    LTP_CHANGE_HP_INFO* Custom = (LTP_CHANGE_HP_INFO*)packet;

                    // 내가 때린 공격만 해당;
                    if ( g_MyD3D->Get_MyPlayer() == Custom->iAttackerIndex )
                    {
                        // 일반공격 일때만 피&엠 채워준다!
                        if ( Custom->dwAttackType & PAA_NORMAL )
                        {
                            // 피 빨수 있나 함 보쟈~
                            // 10프로의 확률이지만 밑에서 66프로의 확률로 HP or MP를 흡수함으로
                            // 14프로 정도로 올려줄테다... ㅋ
                            int iRand = rand()%7;
                            if ( iRand == 1 )
                            {
                                // 0 1 2 나오게 하자 ㅎ 
                                g_MyD3D->GetMyPlayer()->DrainHPMP( rand() % 3 );
                            }
                        }
                        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
                            int iPlayerStartingCnt = g_pGameBoard->GetStartingMemberCount();
                            float fCorrectionValue = 0.f; 
                            float fInfiniteValue = 0.f;
                            float fNormalValue = 0.f;

                            g_pkQuestManager->GetRankCorrectionValue( iPlayerStartingCnt , 2 , fInfiniteValue);
                            g_pkQuestManager->GetRankCorrectionValue( iPlayerStartingCnt , 0 , fNormalValue);

                            fCorrectionValue = SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ? fInfiniteValue : fNormalValue;
#ifdef LEVEL_DESIGN_STAT_CALC	
                            g_pkQuestManager->SetTotalDamagePoint_S6(Custom->fDamage * iPlayerStartingCnt * fCorrectionValue * (-1.0f), Custom->fMonsterMaxHP, g_MyD3D->MyPlayer[Custom->iAttackerIndex]->GetcomboState());
#else
                            g_pkQuestManager->SetTotalDamagePoint_S6(Custom->fDamage * iPlayerStartingCnt * fCorrectionValue * (-100.0f), Custom->fMonsterMaxHP, g_MyD3D->MyPlayer[Custom->iAttackerIndex]->GetcomboState());
#endif
                        }
                    }
                    g_kGlobalValue.SetSpearEffefct( Custom );   // 헬스피어 이펙트 처리 
#ifdef LEVEL_DESIGN_STAT_CALC					
                    g_MyD3D->MyHead->Add_HP_Changer(Custom->iTargetIndex, Custom->fDamage* (-1.0f), false, "", Custom->dwAttackType, Custom->fHitPosX, Custom->fHitPosY);
#else
                    g_MyD3D->MyHead->Add_HP_Changer(Custom->iAttackerIndex, Custom->iTargetIndex, Custom->fDamage* (-100.0f), false, "", Custom->dwAttackType, Custom->fHitPosX, Custom->fHitPosY);
#endif

                    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON &&
                        g_MyD3D->IsPlayerIndex( Custom->iAttackerIndex ) )
                    {
                        g_pGameBoard->m_pStartingMember[Custom->iAttackerIndex].fTotalDamage += ( Custom->fDamage * (-1.0f) );
                        if ( g_MyD3D->MyHead->ShowCombo )
                            g_pGameBoard->m_pStartingMember[Custom->iAttackerIndex].fComboDamage += ( Custom->fDamage * (-1.0f) );
                        else {
                            g_pGameBoard->m_pStartingMember[Custom->iAttackerIndex].fComboDamage = 0.f;
                            g_pGameBoard->SetComboBreak(false);
                        }
                    }
                    else if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && 
                        g_MyD3D->IsPlayerIndex( Custom->iAttackerIndex ) )
                    {
                        float fCalcTotalDamage = 0.f;

                        if ( Custom->fDamage  < -(Custom->fMonsterMaxHP) )
                            fCalcTotalDamage = Custom->fMonsterMaxHP;
                        else
                            fCalcTotalDamage = -(Custom->fDamage);

                        g_pkQuestManager->SetPlayerDamage(  Custom->iAttackerIndex, fCalcTotalDamage );

                    }
                }
                break;
            case ELTP_CHANGE_OBJECT_HP_INFO:
            {
                LTP_CHANGE_OBJECT_HP_INFO* Custom = (LTP_CHANGE_OBJECT_HP_INFO*)packet;
                KGCMonsterBuff* MonObject = (KGCMonsterBuff*)g_MyD3D->m_pObject[Custom->iSlotIndex];

                if ( MonObject != NULL )
                    MonObject->Activate();
            }
            break;
            case ELTP_AGIT_OBJECT_POS:
            {
                LTP_AGIT_OBJECT_POS* Custom = (LTP_AGIT_OBJECT_POS*)packet;

                SiKGCAgitGameManager()->UpdateLatencyInfo( *Custom );
            }
            break;
            case ELTP_AGIT_DICE_OBJECT_STATE:
            {
                LTP_AGIT_DICE_OBJECT_STATE* Custom = (LTP_AGIT_DICE_OBJECT_STATE*)packet;
                SiKGCAgitGameManager()->UpdateDiceObject( *Custom );
            }
            break;
            case ELTP_AGIT_FLAG_OBJECT_STATE:
                {
                    LTP_AGIT_FLAG_OBJECT_STATE* Custom = (LTP_AGIT_FLAG_OBJECT_STATE*)packet;
                    SiKGCAgitGameManager()->UpdateFlagObject( *Custom );
                }
            break;
            case ELTP_CHANGE_PLAYER_HP:
                {
                    LTP_CHANGE_PLAYER_HP* Custom = (LTP_CHANGE_PLAYER_HP*)packet;
                    PLAYER* pPlayer;
                    if( Custom->m_dwUserUID == g_kGlobalValue.m_kUserInfo.dwUID ) {
                        pPlayer = g_MyD3D->GetMyPlayer();                        
                    } else {
                        pPlayer = g_MyD3D->GetPlayerByUID( Custom->m_dwUserUID );
                    }

                    if( pPlayer ) {
                        float fDamage = pPlayer->GetHP() - Custom->m_fChangeHP;
                        if( Custom->m_iSkillID == -1 || pPlayer->CheckSkill( static_cast< EGCSkillTree >( Custom->m_iSkillID ) ) ) {
                            pPlayer->SetHP( Custom->m_fChangeHP );
                            g_MyD3D->MyHead->Add_HP_Changer(pPlayer->m_iPlayerIndex, pPlayer->m_iPlayerIndex, fDamage, false, "",0);
                        }                     
                    }
                }
            break;
            case ELTP_MATCH_QUICK_MODE:
                {
                    LTP_MATCH_QUICK_MODE* pPacket = (LTP_MATCH_QUICK_MODE*)packet;
                    if ( pPacket->cQuickType == LTP_MATCH_QUICK_MODE::FAST_MODE ) 
                    {
                        
                        g_MyD3D->MyHead->Start_Text("FAST MATCH");

                        for(int j=0 ; j < MAX_PLAYER_NUM ; ++j) 
                        {
                            if(g_MyD3D->MyPlayer[j]->m_cLife > 0)
                            {
                                g_MyD3D->MyPlayer[j]->m_cLife -= pPacket->nLife;

                                if(g_MyD3D->MyPlayer[j]->m_cLife < 1)	
                                    g_MyD3D->MyPlayer[j]->m_cLife = 1;	
                            }
                        }

                        g_KDSound.Play( "81" );

                        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH )
                        {
                            KGCMatch* pGameMode = (KGCMatch*)g_pStateGame->GetCurGameModePtr();
                            pGameMode->SetFastMode( true );       
                        }  

                    }
                    else if ( pPacket->cQuickType == LTP_MATCH_QUICK_MODE::FATAL_MODE ) 
                    {
                        g_MyD3D->MyHead->Start_Text("FATAL MATCH");

                        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH )
                        {
                            KGCMatch* pGameMode = (KGCMatch*)g_pStateGame->GetCurGameModePtr();
                            pGameMode->SetFatalMode( true );       
                        }
                    }
                }
                break;
            case ELTP_PLAYER_QUEST_RANK:
                {
                    LTP_PLAYER_QUEST_RANK* pPacket = (LTP_PLAYER_QUEST_RANK*)packet;
                    g_pkQuestManager->SetPlayerRank( pPacket );
                }
                break;
			default:
            {
                break;
            }
		}
	}
}