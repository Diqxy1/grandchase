#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../GCGlobal.h"
#include "KNC/RelayData.h"
#include "TypeDef.h"
bool	SendServerRelayPacket( void* pkPacket_, ULONG ulPacketSize_, DWORD dwUID_ );
void    SendServerRelayPacketInSquare( void* pkPacket_, ULONG ulPacketSize_, const std::vector< DWORD >& vecSend_ );
void    ErrorCheckRelayPacket( void* pkPacket_, int nErrorCode );
int     GetRelayPacketUser();   // TR에 패킷 보낼 유저가 몇명인지 확인한다.

//void   NetworkProc( float fElapsedTime );

enum EP2PCONNECTION {
	P2P_CON_DEFAULT = 0,		// 필요하면 Peer to Peer, 필요하면 Udp Relay
	P2P_UDP_RELAY_ONLY,		// UDP 릴레이만 사용
	P2P_TCP_RELAY_ONLY,		// TCP 릴레이만 사용
};

void	SetP2PMode( EP2PCONNECTION eCon_ );
void    SendP2PPacket( UINT uiPacketID_ );
void	SendP2PPacket( void* pkPacket_, UINT uiPacketSize_, DWORD dwUID_, UINT uiSendOption_ );
void    RealSendP2PPacket( void* pkPacket_, UINT uiPacketSize_, DWORD dwUID_, UINT uiSendOption_ );

void    P2PPacketProc( GCPID kiPacketID_, PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void    SquarePacketProc( GCPID kiPacketID_, PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void	SendPingToTCPRelay();
void	SendPingToUDPRelay();

void	SetRelayPingInterval( int iMilliSecond_ );
int	    GetRelayPingInterval();
void	SetRelayPingLimit( DWORD dwLimit_ );
DWORD	GetRelayPingLimit();
void	SetRelayPingOutCount( int iCount_ );
void	CheckPing();

#define SERIALIZE_P2P_PACKET( kPacket )     KSerBuffer ksBuff;                                  \
                                            KSerializer ks;                                     \
                                            ks.BeginWriting( &ksBuff );                         \
                                            ks.Put( kPacket );                                  \
                                            ks.EndWriting()
// P2PPacketProc에서만 해당되는 매크로이다.
#define DESERIALIZE_P2P_PACKET( kPacket )   KSerBuffer ksBuff;                                  \
                                            KSerializer ks;                                     \
                                            ksBuff.Write( pRecvData_, dwRecvSize_ );            \
                                            ks.BeginReading( &ksBuff );                         \
                                            ks.Get( kPacket );                                  \
                                            ks.EndReading();                                    \


void   ON_GC_PID_PLAYERLATENCY_FRAME_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );           
void   ON_GC_PID_MONSTER_LATENCY_FRAME_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_MONSTER_LIVE_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_SHOOTING_ARROW_TOURNAMENT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );          
void   ON_GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );   
void   ON_GC_PID_ACTION_LEAGUE_RESULT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );               
void   ON_GC_PID_LATENCY_TRANSFER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );                   
void   ON_GC_PID_QUEST_REBIRTH( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );                      
void   ON_GC_PID_BROAD_PLAYER_DEATH_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SANTA_EVENT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_COMPRESSED_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_EFFECT_ITEM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_USEPETSKILL( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_POISON_BREATH( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_TRANSFORM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_TAG_CHARACTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_TRANSNPC( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_PACKED_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_TCP_PACKED_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_GAME_BOARD_UPDATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_CHANGE_TAGMATCH_SLOT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_CHAT_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_DIALOGUE_PROGRESS( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_TG_DEFAULT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_SUMMON_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_DROPITEM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_PLAYERGETITEM( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_QUEST_REAULT_ITEM_DICE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_QUEST_STAGE_RESULT_SCORE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_QUEST_STAGE_POINT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_GATE_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_UDP_RELAYSERVER_HEART_BEAT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_DAMAGE_PACKET( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_SQUARE_ACTION( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_SWAPWEAPON( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_ANGELS_EGG_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_ENTER_GAME( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_COUPLE_EVENT_CREDIT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_MAGIC_MISSILE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_MONSTER_TARGET_REFRESH( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_QUEST_RANK_COMBO_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BURNNING_POINT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_RELAY_PING( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_SELECT_BONUSBOX( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_OPEN_BONUSBOX( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_READY_BONUSBOX_SELECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_START_BONUSBOX_SELECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_CHANGE_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );


void   On_CHANGE_INDIGO_SLOT( const KChangeRoomUserInfo& kUserInfo_);
void   On_INSERT_USER( const KInDoorUserInfo& kUserInfo_ );
void   On_DESTROY_USER( const KEVENT_INFORM_USER_LEAVE_ROOM_NOT& kPacket_ );
void   On_TERMINATE_USER( const KEVENT_INFORM_USER_LEAVE_ROOM_NOT& kPacket_ );
void   On_HOSTMIGRATE( const KEVENT_HOST_MIGRATED_NOT kPacket_ );
void   On_HOSTMIGRATE_WithoutLeave( const DWORD dwNewHostUID );
void   On_CHANGE_CHARACTER( const KChangeRoomUserInfo& kPacket_ );
void   On_CHANGE_SLOT( const KChangeRoomUserInfo& kPacket_ );
void   On_CHANGE_TEAM_READY( const KChangeRoomUserInfo& kPacket_ );
void   On_CHANGE_ROOM_INFO( const KChangeRoomInfo& kPacket_ );
void   On_CHANGE_EQUIP_ITEM( const KEquipUser& kPacket_ );
void   On_CHANGE_INDOORUSER_INFO( const KInDoorUserInfo& kPacket_ );
void   On_CHANGE_INTO_INFO( const KChangeRoomUserInfo& kUserInfo_);
void   On_EQUIP_ITEM( const KEVENT_EQUIP_ITEM_ACK& kEquipItem_ );
bool   On_EQUIP_ITEM_TO_USERINFO( SUserInfo& kUserInfo, const KEVENT_EQUIP_ITEM_ACK& kEquipItem_ );
void   On_LOAD_TIMEOUT( const KEVENT_INFORM_USER_LEAVE_ROOM_NOT& kPacket_ );
void   ON_GC_PID_SHIELD( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EGOITEM_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_WEAPONCHANGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SUMMON_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SUMMON_MONSTER_HP( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void ON_GC_PID_BROAD_SUMMON_MONSTER_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_ANGELS_EGG_DAMAGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_DOTA_ITEM_CHEAT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_DOTA_MONSTERKILL_MSG( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_GAME_BURNNING( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void ON_GC_PID_BROAD_DOTA_SUMMON_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void ON_GC_PID_BROAD_DOTA_POINT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BUY_DOTA_GAMEITEM_REQ( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BUY_DOTA_GAMEITEM_ACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_DOTA_ZONE_POSITION( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_BROAD_AGIT_BREAK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_DUNGEON_PLAYSTATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_BROAD_ESCORT_MONSTER_DESC( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_ADD_DAMAGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_CAMERAZOOM_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_SYNCOBJECT( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_RENDER_HIT_NUMBER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_CHANGE_MONSTER_STATE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_MACRO_COMMUNITY( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARK_FRAME_INFO( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARKATION_REQ( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARKATION_ACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_BROAD_EMBARK_SUMMON_REQ( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARK_SUMMON_ACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_BROAD_EMBARK_DELETE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARK_IN_PORTAL( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_DELETE_DAMAGE( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARK_SYNC_CALLBACK( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_EMBARK_SYNC_UID( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );
void   ON_GC_PID_BROAD_CONTROL_MISSION_DATA( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   ON_GC_PID_SUMMON_INFINITY_MONSTER( PBYTE pRecvData_, DWORD dwUID_, DWORD dwRecvSize_ );

void   AddConnectQ( const SUserInfo& kUserInfo_, bool bConnectNow_ = false );

template< typename Packer, typename Packet >
void	SendFrameLatencyInfo( Packer* pkPacker , Packet* pkPacket );
template< typename Packer, typename Packet > 
void SendAgitFrameLatencyInfo( Packer* pkPacker , Packet* pkPacket );


struct SUserInfo;
void   WinLoseProcess( SUserInfo* pUserInfo, std::vector< std::pair<char,int> >& vecWin, std::vector< std::pair<char,int> >& vecLose );

inline bool CheckCorrectIndex( int& iIndex_ ){ return !(iIndex_ >= MAX_PLAYER_NUM + 1 || iIndex_ < 0); }

extern void         Set_HighRanker( void );
extern HRESULT      SendNetworkData( char msg_Type, int Option, const void* Param_Text = NULL );
extern bool WINAPI  KncP2PMessageHandler( DWORD dwMessageID, PVOID pMsgBuffer );
extern bool		g_bGameModeChange;
extern EP2PCONNECTION	g_eP2PConMode;
extern bool g_bCheckGuardianWeapon;
extern bool g_bCheckTuxedoWeapon;

#ifdef _PACKET_PROFILE_
    struct KPACKET_TEST
    {
        DWORD dwSize;
        DWORD dwCount;
    };
    extern KPACKET_TEST   g_akRecv[GC_PID_END];
    extern KPACKET_TEST   g_akSend[GC_PID_END];
#endif

#endif // _MESSAGE_H_
