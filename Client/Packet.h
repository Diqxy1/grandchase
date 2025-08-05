#ifndef _PACKET_H_
#define _PACKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "UserPacket.h"
//#include <windows.h>    // for data type 'BYTE'
//#include "GCEnum.h"
#include "GCSkillEnum.h"
#include "define.h"
#include "ProtectedType.h"

#include "Serializer/SerPair.h"
#include "Serializer/SerList.h"
#include "Serializer/SerVector.h"
#include "Serializer/SerDeque.h"
#include "Serializer/SerMap.h"
#include "Serializer/SerSet.h"
#include "Serializer/SerBuffer.h"

//#include <boost/utility.hpp>    // noncopyable
#include "TypeDef.h"
#include "GCUTIL/KJohnGen.h"
#include "GCUTIL/GCUtil.h"
#include "GCChatManager.h"
//#include <string>

#pragma pack( push, 1 )

#define MAX_COMPRESS_BUFFER     30000

enum GC_P2P_PACKET_ENUM
{
    // 이 이후로는 게임 시작한 이후에 쓰는 Packet이다.
    GC_PID_PLAYERLATENCY_FRAME_INFO = 2,
    GC_PID_MONSTER_LATENCY_FRAME_INFO,
//     GC_PID_MONSTER_LIVE_INFO,
    GC_PID_SHOOTING_ARROW_TOURNAMENT,            
    GC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT,     
    GC_PID_ACTION_LEAGUE_RESULT,                 
    GC_PID_LATENCY_TRANSFER,                     
    GC_PID_QUEST_REBIRTH,                        
    GC_PID_CHAT_PACKET,                          

    // 게임의 진행에 관한 Packet들을 넣도록 하자.
    GC_PID_BROAD_PLAYER_DEATH_INFO,              
    GC_PID_BROAD_SANTA_EVENT,                    
    GC_PID_COMPRESSED_PACKET,                    
	GC_PID_EFFECT_ITEM,							
	GC_PID_USEPETSKILL,
	GC_PID_TRANSNPC,							
	GC_PID_PACKED_PACKET,						
    GC_PID_TCP_PACKED_PACKET,
    GC_PID_GAME_BOARD_UPDATE,				     
	GC_PID_BROAD_SANTAMON_EVENT,
    GC_PID_TRANSFORM,
    GC_PID_TAG_CHARACTER,
    GC_PID_SUMMON_MONSTER,
    GC_PID_QUEST_REAULT_ITEM_DICE,
    GC_PID_QUEST_STAGE_RESULT_SCORE,
    GC_PID_QUEST_STAGE_POINT,


    // 퀘스트 진행에 쓰이는 Packet 입니다
    // 퀘스트 정보를 날립니다.
    GC_PID_DIALOGUE_PROGRESS,
    // 퀘스트 트리거 정보를 날립니다.
    GC_PID_TG_DEFAULT,


    GC_PID_POISON_BREATH,

    // 아이템 드랍에 쓰이는 Packet 입니다.
    GC_PID_DROPITEM,
    GC_PID_PLAYERGETITEM,

    // 던전 이동 문 상태 정보를 보내는 패킷입니다.
    GC_PID_GATE_STATE,
    GC_PID_UDP_RELAYSERVER_HEART_BEAT,

    GC_PID_DAMAGE_PACKET,

	// [1/8/2008] breadceo. 광장 행동
    GC_PID_SQUARE_ACTION,

    GC_PID_SWAPWEAPON,

	GC_PID_ANGELS_EGG_INFO,

    // 게임 입장
    GC_PID_ENTER_GAME,

	GC_PID_COUPLE_EVENT_CREDIT,

	GC_PID_MAGIC_MISSILE,

    GC_PID_MONSTER_TARGET_REFRESH,  // 몬스터가 논타겟 상태에서~ 타겟을 잡을때 보냄.
    GC_PID_QUEST_RANK_COMBO_STATE, // 방장에게 콤보상태를 보낸다.

    GC_PID_SELECT_BONUSBOX,
    GC_PID_OPEN_BONUSBOX,

    GC_PID_CHANGE_HP_INFO,      // 누구한테 어떤 공격을 받았냐는 정보입니다.. 아놔.. 될까?

    GC_PID_READY_BONUSBOX_SELECT,
    GC_PID_START_BONUSBOX_SELECT,   //

    GC_PID_BURNNING_POINT, // 공격한 캐릭터의 버닝포인트를 플레이어들에게 전달(특정 기술에서 버닝포인트를 알 필요가 있어서...)
	GC_PID_RELAY_PING,	// For Ping test

    GC_PID_CHANGE_MONSTER, // 몬스터 바꾸기!

    GC_PID_SHIELD,
    GC_PID_BROAD_EGOITEM_STATE,
    GC_PID_WEAPONCHANGE, // 무기체인지
    GC_PID_BROAD_SUMMON_MONSTER,
    GC_PID_BROAD_SUMMON_MONSTER_HP,
    GC_PID_BROAD_SUMMON_MONSTER_STATE,
    GC_PID_BROAD_ANGELS_EGG_DAMAGE,
    GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER,
    GC_PID_BROAD_DOTA_SUMMON_MONSTER,
    GC_PID_BROAD_DOTA_POINT,
    GC_PID_BUY_DOTA_GAMEITEM_REQ,
    GC_PID_BUY_DOTA_GAMEITEM_ACK,
    GC_PID_BROAD_DOTA_ZONE_POSITION,
    GC_PID_BROAD_DOTA_ITEM_CHEAT,
    GC_PID_BROAD_DOTA_MONSTERKILL_MSG,
    GC_PID_BROAD_GAME_BURNNING,
    GC_PID_BROAD_AGIT_BREAK,
    GC_PID_BROAD_DUNGEON_PLAYSTATE,
    GC_PID_BROAD_ESCORT_MONSTER_DESC,
    GC_PID_BROAD_SUMMON_DAMAGEMONSTER,
    GC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER,
    GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END,
    GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END,
    GC_PID_BROAD_ADD_DAMAGE,
    GC_PID_BROAD_CAMERAZOOM_STATE,
    GC_PID_BROAD_SYNCOBJECT,
    GC_PID_BROAD_RENDER_HIT_NUMBER,
    GC_PID_BROAD_CHANGE_MONSTER_STATE,
    GC_PID_BROAD_MACRO_COMMUNITY,
    GC_PID_BROAD_EMBARK_FRAME_INFO,
    GC_PID_BROAD_EMBARKATION_REQ,
    GC_PID_BROAD_EMBARKATION_ACK,
    GC_PID_BROAD_EMBARK_SUMMON_REQ,
    GC_PID_BROAD_EMBARK_SUMMON_ACK,
    GC_PID_BROAD_EMBARK_DELETE,
    GC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST,
    GC_PID_BROAD_EMBARK_IN_PORTAL,
    GC_PID_BROAD_DELETE_DAMAGE,
    GC_PID_BROAD_EMBARK_SYNC_CALLBACK,
    GC_PID_BROAD_EMBARK_SYNC_UID,
    GC_PID_BROAD_CONTROL_MISSION_DATA,

    GC_PID_SUMMON_INFINITY_MONSTER, // 전장 몬스터 소환용 

	GC_PID_END
};


enum GCADMINCOMMAND
{
	ADMINCOMMAND_DECMAXPLAYER	= 0,
	ADMINCOMMAND_INCMAXPLAYER	= 1,
	ADMINCOMMAND_KICKPLAYER		= 2,
};

// Copy Construct선언 + CopyOperator정의 + Equal Operator정의.
#define DECL_COPY_EQUAL_OPERATOR( kStruct ) kStruct( void );                                   \
                                            kStruct( const kStruct& kSrc_ );                   \
                                            const kStruct& operator = ( const kStruct& kSrc_ );\
                                            bool operator == ( const kStruct& kSrc_ ) const
DECL_DATA( SUserItem ) // 게임 정보 패킷 
{
	GCITEMUID	    dwItemUID;			//ItemUID를 싫었음2005.11.14
	int             iItemID;
	CHAR			cEnchantLevel;
    CHAR            cGradeID;
    int             nEquipLevel;
    char            m_cEnchantEquipGradeID;
    GCITEMID        m_DesignCoordiID;

    std::vector< KAttributeInfo >   vecItemAttribute;
    std::vector< KSocketInfo >      vecItemSocket;

    DECL_COPY_EQUAL_OPERATOR( SUserItem );
    const SUserItem& operator = ( const KEquipItemInfo& kSrc_ );
	const SUserItem& operator = ( const KCoupleEquip& kSrc_ );
    const SUserItem& operator = ( const KItem& kSrc_ );
    bool operator == (const KEquipItemInfo & kSrc_);

    void Copy( KEquipItemInfo& kItemInfo );
    void Copy( KItem& kItem );
};
typedef         std::vector<SUserItem>      KUserItemVector;
typedef         std::vector<GCITEMID>       KItemIDVector;
typedef			std::vector<KSimpleItem>	KSimpleItemVector;
//====================================================================================
// 2006.07.10 : Asirion
// 케릭터 정보
DECL_DATA( SCharInfo ) // 캐릭터 정보 패킷
{
    char                iCharType;   
    char                iPromotionLevel;
    std::set< char >    setPromotion;
    GCEXPTYPE           biExp;
	UCHAR               iLevel;
	USHORT              iWin;
	USHORT              iLose;
    int                 iGP;
    int                 m_nInvenCapacity;
    int                 m_nCoordiCapacity;

    DWORD               dwSlotNum;

    std::wstring        strCharName;

    KUserItemVector     vecItemInfo;
    KUserItemVector     vecLookInfo;
    KSimpleItemVector	vecPetGlyphInfo;//0x67
    DWORD				dwPetGlyphSize;//0x77

	// 2007.01.08 wony
	// 캐릭터의 펫 정보를 담는 구조체
	KPetInfo            kPetInfo;

    // [7/30/2008] breadceo. 케릭터의 전직별 장착스킬정보
    KSkillInfo kSkillInfo;
	std::map<char, std::vector<KSkillSlot> > m_mapEquipSkill;

    std::vector<KSPInfo> m_vecSPInfo; // 배운스킬 정보.

	KELOUserData kELOUserData;

    KBonusPointInfo kBonusPoint;

    DECL_COPY_EQUAL_OPERATOR( SCharInfo );
    bool operator == ( const int iCharType_ ) const;
    const SCharInfo& operator = ( const KInDoorCharInfo& kSrc_ );

    KItemIDVector GetEquipmentForLoadModel( int iPlayerID = -1 ) const;
    int GetEquipWeaponItemID() const;
    char GetCurrentPromotion(); // 현재 전직상태는? 착용한 무기의 차수가 뭔지
    bool IsAcquiredPromotion( char cPromotion );
    int GetMaxPromotionNum();
    void SetPetGlyphInfo( const KSimpleItemVector vecPetGlyph );
    char GetHighestPromotion();
    void SetCharName(std::wstring strName_ )    { strCharName = strName_; }
    int GetCurrentGP( void ) { return iGP; }
    void SetCurrentGP( const int iGP_ ) { iGP = iGP_; }
    KBonusPointInfo GetBonusPoint( void ) { return kBonusPoint; }
    void SetBonusPoint( const KBonusPointInfo& Info_ ) { kBonusPoint = Info_; }
};
typedef         std::vector<SCharInfo>      KCharInfoVector;
//====================================================================================
// 2006.07.10 : Asirion
// 유저정보 구조체 선언
DECL_DATA( SUserInfo )
{
    // 유저의 신상정보.
    DWORD			dwUID;
    std::wstring    strLogin;
    std::wstring    strNickName;
    std::wstring    strNickColor;
	std::wstring	strNation;
    EGCUserLevel    eGCUserLevel;
    UCHAR           iAge;
    bool            bMan;
    // 케릭터 정보
    char            cCharIndex;
    KCharInfoVector vecCharInfo;
    char          	aiTagSlot[3];
    // 길드정보
    int             iGuildID;
    int             iGuildScore;
    int             aiGuildMark[GUILD_MARK_COMBINE_NUM];
    std::wstring    strMarkName;
    std::wstring    strGuildName;
    // 접속 피씨방 관련
    KProtectedType<char>            cIsNetmarblePCRoom;
    //유저 혜택 (태국은 3BB)
    KProtectedType<char>            clsUserBenefitType; // 태국은 3BB, 타국가에서 유저 혜택을 주기 위해서는 이 변수를 쓰면 된다.
    // P2P접속 이후 활용되는 정보.
    KProtectedType<int>             iTeam;
    bool            bHost;
    //대쓰매치 만들면서 제거 할테다!! blive!!i
    bool            bLive;
    
    // 각종 포인트 관련.
    int             iGP;
	int				iRP;
	int				iGetRP;
	int				iRPRank;

    // 용사의 섬 승패
    USHORT          iIndigoWin;
    USHORT          iIndigoLose;

    // 장비냐, 미션보고있냐
	KProtectedType<short>	    nUserState;

	// 단축슬롯 플레이어별 정보
	KQuickSlot		kQuickSlot;

    std::map<int,KDungeonUserInfo>  mapStagePlayable;

    DWORD           dwPremium;
    
    KIPVector       vecIP;
    KPortVector     vecPort;

    KProtectedType<int> m_nSlotCount;     //  캐릭터 생성 가능한 슬롯 수
    KProtectedType<int> iInvenCapacity;
    KProtectedType<int> iUserItemCount;

	std::set<GCITEMID> m_setInvenBuffItemID; // 인벤토리 버프 ItemID
    std::vector< GCITEMID > m_vecFontVector;

    KProtectedType< int > m_iPvExp;

    DECL_COPY_EQUAL_OPERATOR(SUserInfo);

    SCharInfo& GetCurrentChar(void);
    void UpdatePetGlyph(const KSimpleItemVector vecPetGlyph);
    SCharInfo& GetCurrentChar(int iCharType_);

    int GetCurrentCharType() { return GetCurrentChar().iCharType; }
    void SetCurrentChar(int iCharType_);
    void ClearChar(void);
    void Init(void);
    BYTE GetCharacterGrade(void);
    void AddChar(const SCharInfo & kCharInfo_, bool bSort = false);
    int  GetCharSize(void) { return static_cast<int>(vecCharInfo.size()); }
    bool IsBlueTeam(void) const { return iTeam == 1; }
    bool IsCharExist(int iCharType);
    const SUserInfo& operator = (const KInDoorUserInfo & kSrc_);
    const SUserInfo& operator = (const KChangeRoomUserInfo kSrc_);
    void SetTeam(int iTeam_) { iTeam = iTeam_; }
    void SetIndoorUserInfo(KInDoorUserInfo * pkUserInfo_);
    void SetChangeRoomUserInfo(KChangeRoomUserInfo * pkUserInfo_, KChangeRoomUserInfo::ROOM_USER_INFO eRoomUserInfo_);

    // skill
    void GetSkillSet( IN char Extra_Char_Num, OUT std::vector< EGCSkillTree >& setSkill );
    void GetSkillEquipInfo( IN char Extra_Char_Num, OUT std::vector< EGCSkillTree >& setSkill );
    void UpdateSkillSet( std::map< std::pair<char,char>, int >& mapSetting, std::map< std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSet );
    void UpdateSkillEquipInfo( std::map< std::pair<char,char>, std::vector<KSkillSlot> >& mapSkillEquipInfo_ );

	BYTE GetUserGrade( void );
	int GetUserWin();
	int GetUserLose();

    int GetPromotion( int iCharType = -1 );    

    int GetPvExp();

    bool CheckState( EGCRoomUserState eState_ )
    {
        return eState_ == nUserState;
    }
};
BYTE GetCharGrade( DWORD dwWin );
DECL_DATA( SRoomInfo )
{
    KProtectedType<EGCGameModeCategory> eGameModeCategory;
    KProtectedType<EGCGameMode>         eGameMode;
    KProtectedType<int>                 nSubGameMode;
    KProtectedType<unsigned char>       ucStage;
    bool                                bRandomMapMode;
    char                                cMonsterHuntLevel;
    int                                 nDungeonLevel;
    unsigned int                        uiRandSeed;
    bool                                bPublicRoom;
    bool                                bOpenSlot[MAX_PLAYER_NUM];
    std::wstring                        strRoomName;
    USHORT                              usRoomID;
    USHORT                              usMaxUsers;
    USHORT                              usUsers;
    KProtectedType<int>                 iRestBanCount;
    DWORD                               dwHostUID;
    std::vector<SUserInfo>              vecObserverUserInfo;
    char                                m_cRoutingMethod; // 아이템 루팅 방식... 스펠링이 맞는지 모름..
    short                               iMonsterID;
    short                               iMonsterCount;
    bool                                bPlaying;

    DWORD                               dwRelayServerIP;
    USHORT                              usRelayServerPort;

    DWORD                               dwTRelayServerIP;
    USHORT                              usTRelayServerPort;

    bool                                bDeathMatchIntrudable;
    bool                                bDeathMatchBalancing;
    int                                 nDeathMatchTime;
    short                               nDeathKillCount;
    std::pair<std::wstring,std::wstring> m_pairGuildMarkName;
    std::pair<std::wstring,std::wstring> m_pairGuildName;
    std::pair< DWORD, DWORD >            m_pairBattlePoint; 

    bool								m_bDisablePetAttackPvP;
    bool								m_bEnablePvPBallance;
    bool								m_bDisableSP4SkillsPvP;

    KSafeArray< bool, GC_CHAR_NUM >		m_saAllowedChars;
    KSafeArray< bool, GC_BG_GRADE_MAX >	m_saAllowedRankings;

    DECL_COPY_EQUAL_OPERATOR( SRoomInfo );
    const SRoomInfo& operator = ( const KRoomInfo& kSrc_ );
    const SRoomInfo& operator = ( const KChangeRoomInfo kSrc_ );
    const SRoomInfo& operator = ( const KRoomOptions kSrc_ );
    void SetChangeRoomInfo( KChangeRoomInfo* pkRoomInfo_ );
    inline bool IsExistObserver() { return !vecObserverUserInfo.empty(); }
};
class PLAYER;
struct KPlayerRenderData
{
    bool                            bIsRight;
    D3DXVECTOR3                     vPos;

	KProtectedType<short>            usMotion;  
    KProtectedType<USHORT>           cFrame;
    KPlayerRenderData(){ Init(); }
    KPlayerRenderData( const KPlayerRenderData& src )
    {
        bIsRight    = src.bIsRight;
        vPos        = src.vPos;
        usMotion    = src.usMotion;
        cFrame      = src.cFrame;
    }
    const KPlayerRenderData& operator=( const KPlayerRenderData& src )
    {
        bIsRight    = src.bIsRight;
        vPos        = src.vPos;
        usMotion    = src.usMotion;
        cFrame      = src.cFrame;
        return *this;
    }

	const KPlayerRenderData& operator=( const PLAYER& src );
	void Init() 
    {
        bIsRight = false;
        SETVECTOR3( vPos, 0.0f, 0.0f, 0.5f );
        usMotion = 0;
        cFrame = 0;
    }
    bool Verify() const;
};
//프로텍티드 타입을 사용하지 않는 렌더데이타 ㅡ ㅋ 왠지 플레이어가 아니면 프로텍티드를 안써도 될꺼 같지 않나?!!
struct KRenderData
{
    bool                            bIsRight;
    D3DXVECTOR3                     vPos;
    short                          usMotion;
    USHORT                           cFrame;
    KRenderData(){ Init(); }
    KRenderData( const KRenderData& src )
    {
        bIsRight    = src.bIsRight;
        vPos        = src.vPos;
        usMotion    = src.usMotion;
        cFrame      = src.cFrame;
    }
    const KRenderData& operator=( const KRenderData& src )
    {
        bIsRight    = src.bIsRight;
        vPos        = src.vPos;
        usMotion    = src.usMotion;
        cFrame      = src.cFrame;
        return *this;
    }
    const KRenderData& operator=( const KPlayerRenderData& src )
    {
        bIsRight    = src.bIsRight;
        vPos        = src.vPos;
        usMotion    = src.usMotion;
        cFrame      = src.cFrame;
        return *this;
    }
	const KRenderData& operator=( const PLAYER& src );
    void Init() 
    {
        bIsRight = false;
        SETVECTOR3( vPos, 0.0f, 0.0f, 0.5f );
        usMotion = 0;
        cFrame = 0;
    }
};

struct KPlayerLatency
{
    KPlayerRenderData     kRenderData[LATENCY*2];
    D3DXVECTOR2           vSpeed[LATENCY*2];
    KProtectedType<float> fHP[LATENCY*2];
    KProtectedType<USHORT>usShield[LATENCY*2];

    KPlayerLatency() { Init(); }
    void Init()
    {
        for( int i = 0; i < LATENCY*2; ++i )
        {
            kRenderData[i].Init();
            SETVECTOR2( vSpeed[i], 0.0f, 0.0f );
            fHP[i] = 0;
            usShield[i] = 0;
        }
    }
};


struct NPC
{
    NPC()
    {
        Init();
    }

    unsigned char What;
    char          Action;
    bool          IsRight;
    float         x;
    float         y;
    float         HP;

    void Init()
    {
        IsRight = 0;
        x = 0;
        y = 0;
        What = 0;
        Action = -1;
        HP = 0;
    }
};

struct EFFITEM
{
    bool bShowChat;
	char  cWhat;
	short sTime;
};

struct PETSKILL
{
	float		fX;
	float		fY;
	bool		bIsRight;
};

struct KGC_PID_PLAYERLATENCY_FRAME_INFO
{
    GCPID                   Type;
	UCHAR			        ucStage;
    char                    Life;
	bool                    IsRight[LATENCY];
    float                   Body_Angle;
    USHORT                  usPosX[LATENCY];				//float를 SHORT로 바꾸고 1000을 곱해서 사용한다.
    USHORT                  usPosY[LATENCY];
    INT                     iHP[LATENCY];              //float를 INT로 바꾸고 10000을 곱해서 사용한다.    
    USHORT                  Now_Motion[LATENCY];
    unsigned short           Frame[LATENCY];
};

// 몬스터 프레임 정보를 담는 구조체
struct KGC_PID_MONSTER_LATENCY_FRAME_INFO
{
    GCPID           Type;

    UCHAR           cMonsterIndex;
    UCHAR           aucMotion[LATENCY];
    USHORT          aucFrame[LATENCY];
    USHORT          ausX[LATENCY];
    USHORT          ausY[LATENCY];
    bool            abIsRight[LATENCY];

    UINT            uiHP;
    USHORT          usBodyAngle;
    char            cTargetIndex;
    char            cLastAttackedMe;
    int             iMonsterKind;
    int             iMonsterType;
    int             iBoss;
    int             iLevel;
    int             iStageID;
};

struct KGC_PID_MONSTER_LIVE_INFO
{
	const KGC_PID_MONSTER_LIVE_INFO& operator = (const KGC_PID_MONSTER_LIVE_INFO &src){
		Type = src.Type;
		LiveInfo = src.LiveInfo;
	}
    GCPID           Type;
	std::map<int, bool> LiveInfo;
};

struct KGC_PID_SHIELD
{
    KGC_PID_SHIELD()
    {
        Type = GC_PID_SHIELD;
        for( int i = 0; i < LATENCY; i++ )
            ucShield[i] = 0;
    }
    GCPID           Type;
    UCHAR           ucShield[LATENCY];
};

struct KGC_PID_TRANSNPC
{
	KGC_PID_TRANSNPC()
	{
		Type = GC_PID_TRANSNPC;
	}
	GCPID			Type;
	NPC             TransNPC;
};
struct KGC_PID_EFFECT_ITEM
{
	KGC_PID_EFFECT_ITEM()
	{
		Type = GC_PID_EFFECT_ITEM;
	}
	GCPID           Type;
	EFFITEM         Eff_Item[MAX_PLAYER_NUM];
};

struct KGC_PID_TRANSFORM
{
    KGC_PID_TRANSFORM()
    {
        Type = GC_PID_TRANSFORM;
    }
    GCPID			Type;
    CHAR            cTransformType;
    UCHAR		    ucPlayerIndex;
};

struct KGC_PID_TAG_CHARACTER
{
    KGC_PID_TAG_CHARACTER()
    {
        Type = GC_PID_TAG_CHARACTER;
    }
    GCPID			Type;
    bool            bArena;
    CHAR		    cPlayerIndex;
};

struct KGC_PID_USEPETSKILL
{
	KGC_PID_USEPETSKILL()
	{
		Type = GC_PID_USEPETSKILL;
	}
	GCPID			Type;
	char			cPlayerIndex;
	char			cPetMotion;
	bool			bUsePetSkill;
	PETSKILL		PetSkill;
};

// 궁수 활쏘기 대회에서...
struct KGC_PID_SHOOTING_ARROW_TOURNAMEN
{
    GCPID Type;
    bool  TimeOut;
    bool  RequestStartMessage;
    bool  Shot;
    float Body_Angle;
};

struct KGC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT
{
    GCPID Type;
    int   Story;
    float Height;
    int   Level;
    int   Score[6][3];
};

struct KGC_PID_ANGELS_EGG_INFO
{
	GCPID Type;
	UCHAR   m_ucTeam;
	UCHAR   m_ucState;
	float	m_fHP;
	float	m_fMaxHP;
	bool	m_bBreak;
};

// 액션 리그
struct KGC_PID_ACTION_LEAGUE_RESULT
{
    GCPID   Type;
    int     Team_Point;
};

struct KGC_PID_QUEST_REBIRTH
{
	KGC_PID_QUEST_REBIRTH()
	{
		Type = GC_PID_QUEST_REBIRTH;
	}
	GCPID   Type;
	bool    bRequest;			//Request인지 Ack인지를 나타냄..  패킷 한개만 만들기위해
	bool    bOK;				//Ack일 경우에 사용유무를 나타냅니다
	DWORD   dwUserDPNID;		//사용한사람의 DPNID

};

// 방장 권한 컨트롤 패킷
struct KGC_PID_ADMIN_COMMAND
{
	GCPID         Type;
	unsigned char Command;
	unsigned char Option;
};

struct KGC_PID_COMPRESSED_PACKET
{
    GCPID   Type;
    BYTE    pCompressBuffer[MAX_COMPRESS_BUFFER];
};

struct KGC_PID_PACKED_PACKET
{
	KGC_PID_PACKED_PACKET()
	{
		Type = GC_PID_PACKED_PACKET;
	}
	GCPID           Type;
	UCHAR			ucPacketCount;
	BYTE			pPackedPacket[MAX_COMPRESS_BUFFER];
};


struct KGC_PACKET_HEADER
{
    KGC_PACKET_HEADER(){}
    KGC_PACKET_HEADER( GCPID kPacketID_, DWORD dwUID_ )
    {
        dwUID      = dwUID_;
        Type  = kPacketID_;
    }
    GCPID                           Type;
    DWORD                           dwUID;
};

struct LTP_BASIC : public KGC_PACKET_HEADER
{
    UCHAR       	packetType;
    USHORT			size;	
    float			latencyTime;

	LTP_BASIC()
	{
		latencyTime	=	0.0f;
	}
};


// TCP/IP 릴레이서버를 사용하기 위한 패킷 패커 구조체
struct KGC_PID_TCP_PACKED_PACKET : public LTP_BASIC
{
    KGC_PID_TCP_PACKED_PACKET()
    {
        Type = GC_PID_TCP_PACKED_PACKET;
    }
    UCHAR			ucPacketCount;
    BYTE			pPackedPacket[MAX_COMPRESS_BUFFER];
};

struct KGC_PID_POISON_BREATH
{
	KGC_PID_POISON_BREATH()
	{
		Type = GC_PID_POISON_BREATH;
		iBreathIndex = -1;
	}
	GCPID			Type;
	int				iBreathIndex;
	float			fPosX;
	float			fPosY;
	
};

struct KGC_PID_SWAPWEAPON
{
    KGC_PID_SWAPWEAPON()
    {
        Type = GC_PID_SWAPWEAPON;
    }
    GCPID			Type;
    UCHAR		    ucPlayerIndex;
    UCHAR           ucWeaponType;
    bool            bNotChangeMotion;
};

struct KGC_PID_ENTER_GAME
{
    KGC_PID_ENTER_GAME()
    {
        Type = GC_PID_ENTER_GAME;
    }
    GCPID			Type;
    UCHAR		    ucPlayerIndex;
};

struct KGC_PID_COUPLE_EVENT_CREDIT
{
	KGC_PID_COUPLE_EVENT_CREDIT()
	{
		Type = GC_PID_COUPLE_EVENT_CREDIT;
	}
	GCPID			Type;
	UCHAR		    m_iEventType;
};

struct KGC_PID_MAGIC_MISSILE
{
	KGC_PID_MAGIC_MISSILE()
	{
		Type = GC_PID_MAGIC_MISSILE;
	}
	GCPID			Type;
	float			fPosX;
	float			fPosY;
    int 			iLife;
};

struct KGC_PID_MONSTER_TARGET_REFRESH
{
    KGC_PID_MONSTER_TARGET_REFRESH()
    {
        Init();
        Type = GC_PID_MONSTER_TARGET_REFRESH;
    }

    GCPID Type;

    char iMonsterIndex;

    void Init()
    {
        iMonsterIndex = 0;
    }
};

struct KGC_PID_QUEST_RANK_COMBO_STATE
{
    KGC_PID_QUEST_RANK_COMBO_STATE()
    {
        Init();
        Type = GC_PID_QUEST_RANK_COMBO_STATE;
    }

    GCPID Type;
    DWORD dwUID;
    bool bIsComboState;
    bool bAttacked;

    void Init()
    {
        dwUID = -1;
        bIsComboState = false;
        bAttacked = false;
    }
};

struct KGC_PID_BURNNING_POINT
{
    KGC_PID_BURNNING_POINT()
    {
        Init();
        Type = GC_PID_BURNNING_POINT;
    }

    GCPID Type;
    DWORD dwUID;
    float fBP;

    void Init()
    {
        dwUID = -1;
        fBP = 0.0f;

    }
};

struct KGC_PID_RELAY_PING
{
	KGC_PID_RELAY_PING()
	{
		Init();
		Type = GC_PID_RELAY_PING;
	}

	enum {
		TCP_RELAY = 0,
		UDP_RELAY,
	};

	GCPID Type;
	DWORD dwUID;
	UCHAR ucServerType;
	DWORD dwTimeStamp;

	void Init()
	{
		dwUID = 0;
		dwTimeStamp = 0;
		ucServerType = UDP_RELAY;
	}
};

struct KGC_PID_CHANGE_MONSTER
{
    // 바꿔치기 몬스터 인포
    struct KChangeMonInfo
    {
        int      usMonType;
        int      ucLv;
        int      usTriggerID;
        int      usSpecial;
        float    fAttackPoint;
        float    fHPPoint;
        DWORD    dwChampionProperty;

        KChangeMonInfo()
        {
            usMonType        = -1;
            ucLv             = -1;
            usTriggerID      = -1;
            usSpecial        = -1;
            fAttackPoint    = 0.0f;
            fHPPoint        = 0.0f;
            dwChampionProperty = 0x00000000;

            Init();
        }

        void SetChangeMonInfo( int iMonType_, int iLv_, float fAttackPoint_, float fHPPoint_, int nSpecial_, int iTriggerID_ = -1, DWORD dwChampionProperty_ = 0x00000000 )
        {
            usMonType = iMonType_;
            ucLv = iLv_;
            usTriggerID = iTriggerID_;
            usSpecial = nSpecial_;
            fAttackPoint    = fAttackPoint_;
            fHPPoint        = fHPPoint_;
            dwChampionProperty = dwChampionProperty_;
        }

        void Init()
        {
            usMonType    = -1;
            ucLv         = -1;
            usTriggerID  = -1;
            usSpecial    = -1;
            fAttackPoint    = 0.0f;
            fHPPoint        = 0.0f;
            dwChampionProperty = 0x00000000;
        }
    };

    KGC_PID_CHANGE_MONSTER()
    {
        Type = GC_PID_CHANGE_MONSTER;
    }
    GCPID			Type;
    USHORT          usMonIndex;
    KChangeMonInfo  kChangeMon;
};

struct KGC_PID_WEAPONCHANGE
{
    KGC_PID_WEAPONCHANGE()
    {
        Type = GC_PID_WEAPONCHANGE;
        dwUID = 0;
    }

    GCPID Type;
    DWORD dwUID;
};


// 일괄 처리를 위해서 모두다 Serialize를 한다.( 방안, 승부에 관한 부분만 )
// 다른 패킷은 처리 속도와 전송량 감소를 위해 Serialize하지 않는다.
#define DECL_P2P_PACKET_DATA(name)      struct name; \
                                        SERIALIZE_DEFINE_TAG(name, eTAG_USERCLASS); \
                                        SERIALIZE_DECLARE_PUTGET(name); \
                                        struct name : public boost::noncopyable, public KGC_PACKET_HEADER

struct KGC_PID_BROAD_ESCORT_MONSTER_DESC 
{
    KGC_PID_BROAD_ESCORT_MONSTER_DESC() 
    {
        Init();
        Type = GC_PID_BROAD_ESCORT_MONSTER_DESC;
    }

    GCPID   Type;
    int     iStringID;
    int     iShowFrame;
    bool    bWarning;

    void Init() 
    {
        iStringID = 0;
        iShowFrame = 0;
        bWarning = false;
    }
};

struct KGC_PID_BROAD_AGIT_BREAK
{
    KGC_PID_BROAD_AGIT_BREAK()
    {
        Init();
        Type = GC_PID_BROAD_AGIT_BREAK;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iMonID;
    int         iDiscCnt;
    D3DXVECTOR2 vPos;
    void Init()
    {
        iMonID = 0;
        iDiscCnt = 0;
        vPos = D3DXVECTOR2(0.0f, 0.0f);
    }
};

struct KGC_PID_BROAD_SUMMON_MONSTER
{
    KGC_PID_BROAD_SUMMON_MONSTER()
    {
        Init();
        Type = GC_PID_BROAD_SUMMON_MONSTER;
    }

    GCPID       Type;
    DWORD       dwUID;

    bool        IsRight;
    int         iTeam;
    float       fMaxHP;
    float       fMotherHpRatio;
    float       fLimitDistance;
    float       fSummonsAtkRatio;
    float       fOwnerAtkRatio;
    int         iSummonLimit;
    int         iBuffIcon;
    EGCMonster  MonsterID;
    D3DXVECTOR2 vecSummonPos;
	int			iSlotID;
    float       fStaticMaxHP;
    bool        bUnlimitDef;

    void Init()
    {
        IsRight = false;
        iTeam = 0;
        fMaxHP = 0.f;
        fMotherHpRatio = -1.f;
        fLimitDistance = 0.0f;
        iSummonLimit = -1;
        iBuffIcon = 0;
        MonsterID = MON_INVALID;
        vecSummonPos = D3DXVECTOR2(0.0f, 0.0f);
		iSlotID =0;
        fStaticMaxHP = -1.f;
        bUnlimitDef = false;
        fSummonsAtkRatio = -1.f;
        fOwnerAtkRatio = -1.f;
    }
};

struct KGC_PID_BROAD_SUMMON_DAMAGEMONSTER
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER()
    {
        Init();
    }

    void Init()
    {
        Type           = GC_PID_BROAD_SUMMON_DAMAGEMONSTER;
        m_usMonType     = SHRT_MAX;
        m_usEntranceX   = 0;
        m_usEntranceY   = 0;
        m_iSlot         = -1;
        m_iLevel        = 0;
        m_iHitPlayerIdx = -1;
        m_bCharLock = false;
        ZeroMemory( m_szState, sizeof(m_szState) );
    }

    GCPID           Type;
    USHORT          m_usMonType;
    USHORT          m_usEntranceX;
    USHORT          m_usEntranceY;
    int             m_iSlot;
    int             m_iLevel;
    int             m_iHitPlayerIdx;
    char            m_szState[16];
    bool            m_bCharLock;
};

struct KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER()
    {
        Init();
    }

    void Init()
    {
        Type           = GC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER;
        IsRight = false;
        iTeam = 0;
        iLevel = 0;
        fLimitDistance = 0.0f;
        iSummonLimit = -1;
        iHitPlayerIdx = -1;
        MonsterID = MON_INVALID;
        vecSummonPos = D3DXVECTOR2(0.0f, 0.0f);
        ZeroMemory( szState, sizeof(szState) );
		iSlotID = 0;
    }

    GCPID       Type;
    DWORD       dwUID;

    bool        IsRight;
    int         iTeam;
    int         iLevel;
    int         iSummonLimit;
    int         iHitPlayerIdx;
    float       fLimitDistance;
    char        szState[16];

    EGCMonster  MonsterID;
    D3DXVECTOR2 vecSummonPos;
	int			iSlotID;
};

struct KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_END
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_END()
    {
        Init();
        Type = GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END;
    }

    GCPID       Type;
    int         iHitPlayerIdx;
    void Init()
    {
        Type           = GC_PID_BROAD_SUMMON_DAMAGEMONSTER_END;
        iHitPlayerIdx = -1;
    }
};

struct KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END
{
    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END()
    {
        Init();
        Type = GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END;
    }

    GCPID       Type;
    int         iHitPlayerIdx;
    void Init()
    {
        Type           = GC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END;
        iHitPlayerIdx = -1;
    }
};

struct KGC_PID_BROAD_SUMMON_MONSTER_HP
{
    KGC_PID_BROAD_SUMMON_MONSTER_HP()
    {
        Init();
        Type = GC_PID_BROAD_SUMMON_MONSTER_HP;
    }

    GCPID       Type;
    DWORD       dwUID;
    float       fHpRadio;
    char        cIndex;
    bool        bDieType;
    int         iMonID;

    void Init()
    {
        fHpRadio = 0.0f;
        cIndex = -1;
        bDieType = false;
        iMonID = -1;
    }
};

struct KGC_PID_BROAD_EMBARK_FRAME_INFO
{
    KGC_PID_BROAD_EMBARK_FRAME_INFO()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_FRAME_INFO;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUID;

    bool        bIsRight[LATENCY];
    bool        bIsContact[LATENCY];
    short       iStateIndex[LATENCY];
    UCHAR       cFrame[LATENCY];
    D3DXVECTOR3 vPos[LATENCY];

    float       fHP;

    int         iAngleBone;
    D3DXVECTOR3 vAngle;

    void Init()
    {
        for(int iLoop=0; iLoop<LATENCY; ++iLoop)
        {
            bIsRight[iLoop] = true;
            bIsContact[iLoop] = true;
            iStateIndex[iLoop] = 0;
            cFrame[iLoop] = 0;
            vPos[iLoop] = D3DXVECTOR3(0.0f, 0.0f, 2.5f);
        }
        fHP = 0.0f;
        iAngleBone = -1;
        vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }
};

struct KGC_PID_BROAD_EMBARKATION_REQ
{
    KGC_PID_BROAD_EMBARKATION_REQ()
    {
        Init();
        Type = GC_PID_BROAD_EMBARKATION_REQ;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUID;
    bool        bEmbarkation;
    int         iBoneID;

    void Init()
    {
        dwEmbarkUID = 0;
        bEmbarkation = false;
        iBoneID = -1;
    }
};

struct KGC_PID_BROAD_EMBARKATION_ACK
{
    KGC_PID_BROAD_EMBARKATION_ACK()
    {
        Init();
        Type = GC_PID_BROAD_EMBARKATION_ACK;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUID;
    bool        bEmbarkation;
    int         iBoneID;

    void Init()
    {
        dwEmbarkUID = 0;
        bEmbarkation = false;
        iBoneID = -1;
    }
};

struct KGC_PID_BROAD_EMBARK_SUMMON_REQ
{
    KGC_PID_BROAD_EMBARK_SUMMON_REQ()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_SUMMON_REQ;
    }

    GCPID       Type;
    DWORD       dwPlayerUID;
    DWORD       dwEmbarkUID;
    int         iEmbarkType;
    float       fPosX;
    float       fPosY;
    bool        bIsRight;
    bool        bEmbakation;

    void Init()
    {
        dwPlayerUID = 0;
        dwEmbarkUID = 0;
        iEmbarkType = 0;
        fPosX = 0.0f;
        fPosY = 0.0f;
        bIsRight = false;
        bEmbakation = false;
    }
};

struct KGC_PID_BROAD_EMBARK_SUMMON_ACK
{
    KGC_PID_BROAD_EMBARK_SUMMON_ACK()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_SUMMON_ACK;
    }

    GCPID       Type;
    DWORD       dwPlayerUID;
    DWORD       dwEmbarkUID;
    int         iEmbarkType;
    float       fPosX;
    float       fPosY;
    bool        bIsRight;
    bool        bEmbakation;

    void Init()
    {
        dwPlayerUID = 0;
        dwEmbarkUID = 0;
        iEmbarkType = 0;
        fPosX = 0.0f;
        fPosY = 0.0f;
        bIsRight = false;
        bEmbakation = false;
    }
};

struct KGC_PID_BROAD_EMBARK_DELETE
{
    KGC_PID_BROAD_EMBARK_DELETE()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_DELETE;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUID;
    
    void Init()
    {
        dwEmbarkUID = 0;
    }
};

struct KGC_PID_BROAD_EMBARK_IN_PORTAL
{
    KGC_PID_BROAD_EMBARK_IN_PORTAL()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_IN_PORTAL;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUID;
    float       fPosX;

    void Init()
    {
        dwEmbarkUID = 0;
        fPosX = 0.0f;
    }
};

struct KGC_PID_BROAD_EMBARK_SYNC_CALLBACK
{
    KGC_PID_BROAD_EMBARK_SYNC_CALLBACK()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_SYNC_CALLBACK;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUID;
    float       fValue;

    void Init()
    {
        dwEmbarkUID = 0;
        fValue = 0.0f;
    }
};

struct KGC_PID_BROAD_EMBARK_SYNC_UID
{
    KGC_PID_BROAD_EMBARK_SYNC_UID()
    {
        Init();
        Type = GC_PID_BROAD_EMBARK_SYNC_UID;
    }

    GCPID       Type;
    DWORD       dwUID;
    DWORD       dwEmbarkUIDCnt;

    void Init()
    {
        dwEmbarkUIDCnt = 0;
    }
};

struct KGC_PID_BROAD_DELETE_DAMAGE
{
    KGC_PID_BROAD_DELETE_DAMAGE()
    {
        Init();
        Type = GC_PID_BROAD_DELETE_DAMAGE;
    }

    GCPID       Type;
    DWORD       dwUID;

    int         iWhat;
    int         iWho;
    DWORD       dwIdentifie;
    bool        bIsMonsterDamage;

    void Init()
    {
       iWhat = -1;
       iWho =  -1;
       dwIdentifie = 0;
       bIsMonsterDamage = false;
    }
};

struct KGC_PID_BROAD_SUMMON_MONSTER_STATE
{
    KGC_PID_BROAD_SUMMON_MONSTER_STATE()
    {
        Init();
        Type = GC_PID_BROAD_SUMMON_MONSTER_STATE;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iMonsterSlotIndex;
    int         iMonsterState;

    void Init()
    {
        iMonsterSlotIndex = -1;
        iMonsterState = 0;
    }
};

struct KGC_PID_BROAD_CHANGE_MONSTER_STATE
{
    KGC_PID_BROAD_CHANGE_MONSTER_STATE()
    {
        Init();
        Type = GC_PID_BROAD_CHANGE_MONSTER_STATE;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iMonsterID;
    int         iSlotID;
    int         iStateID;

    void Init()
    {
        iMonsterID = -1;
        iSlotID = -1;
        iStateID = -1;
    }
};

struct KGC_PID_BROAD_MACRO_COMMUNITY
{
    KGC_PID_BROAD_MACRO_COMMUNITY()
    {
        Init();
        Type = GC_PID_BROAD_MACRO_COMMUNITY;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iCommunitySlot;

    void Init()
    {
        iCommunitySlot = -1;
    }
};

struct KGC_PID_BROAD_GAME_BURNNING
{
    KGC_PID_BROAD_GAME_BURNNING()
    {
        Type = GC_PID_BROAD_GAME_BURNNING;
        bBurnning = false;
    }

    GCPID       Type;
    DWORD       dwUID;
    bool        bBurnning;
};

struct KGC_PID_BROAD_ANGELS_EGG_DAMAGE
{
    KGC_PID_BROAD_ANGELS_EGG_DAMAGE()
    {
        Init();
        Type = GC_PID_BROAD_ANGELS_EGG_DAMAGE;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iDamageEnum;
    int         iKiller;
    int         iGhost;

    void Init()
    {
        iDamageEnum = 0;
        iKiller = 0;
        iGhost = 0;
    }
};

struct KGC_PID_BROAD_DOTA_ITEM_CHEAT
{
    KGC_PID_BROAD_DOTA_ITEM_CHEAT()
    {
        Init();
        Type = GC_PID_BROAD_DOTA_ITEM_CHEAT;
    }

    GCPID       Type;
    DWORD       dwUID;
    float fAtk;
    float fHp;
    float fFullHp;
    float fCritical;
    float fNPCAtk;
    float fNPCDef;
    float fNPCHp;
    float fNPCSpeed;

    void Init()
    {
        fAtk = 0.0f;
        fHp = 0.0f;
        fFullHp = 0.0f;
        fCritical = 0.0f;
        fNPCAtk = 0.0f;
        fNPCDef = 0.0f;
        fNPCHp = 0.0f;
        fNPCSpeed = 0.0f;
    }
};

struct KGC_PID_BROAD_DOTA_MONSTERKILL_MSG
{
    KGC_PID_BROAD_DOTA_MONSTERKILL_MSG()
    {
        Init();
        Type = GC_PID_BROAD_DOTA_MONSTERKILL_MSG;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iKillerIdx;
    int         iTeamStringID;
    int         iMsgID;
    int         iMonsterIdx;
    bool        bKillerMonster;
    D3DXVECTOR2 vDeathPos;

    void Init()
    {
        iMsgID = -1;
        iKillerIdx = -1;
        iTeamStringID = -1;
        iMonsterIdx = -1;
        bKillerMonster = false;
        vDeathPos.x = 0.0f;
        vDeathPos.y = 0.0f;
    }
};

struct KGC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER
{
    KGC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER()
    {
        Init();
        Type = GC_PID_BROAD_ANGELS_SUMMON_EGGMONSTER;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iSlot;
    int         iMonType;
    int         iTeam;
    float       fHP;
    bool        bIsRight;
    D3DXVECTOR2 vPos;

    void Init()
    {
        iSlot = -1;
        iMonType = -1;
        iTeam = -1;
        fHP = 0.0f;
        bIsRight = false;
        vPos = D3DXVECTOR2(0.0f, 0.0f);
    }
};

struct KGC_PID_BROAD_DOTA_SUMMON_MONSTER
{
    KGC_PID_BROAD_DOTA_SUMMON_MONSTER()
    {
        Init();
        Type = GC_PID_BROAD_DOTA_SUMMON_MONSTER;
    }

    GCPID       Type;
    DWORD       dwUID;

    bool        bIsRight;
    int         iHP;
    int         iTeam;
    int         iSlot;
    int         iGateID;
    EGCMonster  MonsterID;
    D3DXVECTOR2 vecSummonPos;

    void Init()
    {
        bIsRight = false;
        iHP = 0;
        iTeam = 0;
        iSlot = -1;
        iGateID = -1;
        MonsterID = MON_INVALID;
        vecSummonPos = D3DXVECTOR2(0.0f, 0.0f);
    }
};

struct KGC_PID_BROAD_DOTA_POINT
{
    KGC_PID_BROAD_DOTA_POINT( GCPID kPacketID_, DWORD dwUID_, int *piDPoint_, int *piKill_=NULL, int *piDeath_=NULL, bool bKillDeathInfo_ = false)
    {
        dwUID = dwUID_;
        iType = kPacketID_;
        memcpy(iDPoint, piDPoint_, sizeof(int) * MAX_PLAYER_NUM);
        if( piKill_ )
            memcpy(iKill, piKill_, sizeof(int) * MAX_PLAYER_NUM);
        if( piDeath_ )
            memcpy(iDeath, piDeath_, sizeof(int) * MAX_PLAYER_NUM);
        bKillDeathInfo = bKillDeathInfo_;
    }

    int iType;
    DWORD dwUID;
    int iDPoint[MAX_PLAYER_NUM];
    int iDeath[MAX_PLAYER_NUM];
    int iKill[MAX_PLAYER_NUM];
    bool bKillDeathInfo;
};

struct KGC_PID_BUY_DOTA_GAMEITEM
{
    KGC_PID_BUY_DOTA_GAMEITEM( GCPID kPacketID_, DWORD dwUID_, int iGameItemID_, int iOK_ )
    {
        iType = kPacketID_;
        dwUID = dwUID_;
        iOK = iOK_;
        iGameItemID = iGameItemID_;
    }

    GCPID iType;
    DWORD dwUID;
    int iOK;
    int iGameItemID;
};

struct KGC_PID_BROAD_DOTA_ZONE_POSITION
{
    KGC_PID_BROAD_DOTA_ZONE_POSITION( GCPID kPacketID_, DWORD dwUID_, int iPlayerIdx_, int iZonePos_, bool bSuper_, D3DXVECTOR2 vPlayerPos_ )
    {
        iType = kPacketID_;
        dwUID = dwUID_;
        iPlayerIdx = iPlayerIdx_;
        iZonePos = iZonePos_;
        bSuper = bSuper_;
        vPlayerPos = vPlayerPos_;
    }

    GCPID iType;
    DWORD dwUID;
    int iPlayerIdx;
    int iZonePos;
    D3DXVECTOR2 vPlayerPos;
    bool bSuper;
};

struct KGC_PID_BROAD_EGOITEM_STATE
{
    KGC_PID_BROAD_EGOITEM_STATE()
    {
        Init();
        Type = GC_PID_BROAD_EGOITEM_STATE;
    }

    GCPID       Type;
    int         iPlayerID;
    int         iModelID;
    int         iConditionID;
    DWORD       dwItemID;
    DWORD       dwUID;

    void Init()
    {
        iPlayerID = 0;
        iModelID = 0;
        dwItemID = 0;
        iConditionID = 0;
    }
};

struct KGC_PID_BROAD_DUNGEON_PLAYSTATE
{
    KGC_PID_BROAD_DUNGEON_PLAYSTATE()
    {
        Init();
        Type = GC_PID_BROAD_DUNGEON_PLAYSTATE;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iState;

    void Init()
    {
        iState = -1;
    }
};

struct KGC_PID_BROAD_CAMERAZOOM_STATE
{
    KGC_PID_BROAD_CAMERAZOOM_STATE()
    {
        Init();
        Type = GC_PID_BROAD_CAMERAZOOM_STATE;
    }

    GCPID       Type;
    DWORD       dwUID;
    float       fBefore;
    float       fAfter;

    void Init()
    {
        fBefore=0.0f;
        fAfter=0.0f;
    }
};

DECL_P2P_PACKET_DATA( KGC_PID_BROAD_PLAYER_DEATH_INFO )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_BROAD_PLAYER_DEATH_INFO, kSrc_ );
    KGC_PID_BROAD_PLAYER_DEATH_INFO( GCPID kPacketID_, DWORD dwUID_, DWORD dwKillMePlayerID_, float fGetExpGpRatio_, 
                                     bool bDeathFromFall_, bool bSelfKilling_ = false, bool bDeathFromMonster_ = false) : KGC_PACKET_HEADER( kPacketID_, dwUID_ )
    {
        dwKillMePlayerID    = dwKillMePlayerID_;
        bDeathFromFall      = bDeathFromFall_;
        bSelfKilling        = bSelfKilling_;
        fGetExpGpRatio      = fGetExpGpRatio_;
		bDeathFromMonster	= bDeathFromMonster_;
    }
    DWORD                           dwKillMePlayerID;
	bool							bDeathFromMonster;
    bool                            bDeathFromFall;
    bool                            bSelfKilling;
    float                           fGetExpGpRatio;
};

DECL_P2P_PACKET_DATA( KGC_PID_BROAD_SANTA_EVENT )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_BROAD_SANTA_EVENT, kSrc_ );
    KGC_PID_BROAD_SANTA_EVENT( GCPID kPacketID_, DWORD dwUID_, float fStartPosX_, float fStartPosY_, 
                               float fEndPosX_, float fEndPosY_ ) : KGC_PACKET_HEADER( kPacketID_, dwUID_ )
    {
        fStartPosX  = fStartPosX_;
        fStartPosY  = fStartPosY_;
        fEndPosX    = fEndPosX_;
        fEndPosY    = fEndPosY_;
    }
    float                           fStartPosX;
    float                           fStartPosY;
    float                           fEndPosX;
    float                           fEndPosY;
};

DECL_P2P_PACKET_DATA( KGC_PID_BROAD_SANTAMON_EVENT )
{
	CON_COPYCON_ASSIGNOP( KGC_PID_BROAD_SANTAMON_EVENT, kSrc_ );
	KGC_PID_BROAD_SANTAMON_EVENT( GCPID kPacketID_, DWORD dwUID_,  
							int		iSantaLv_, 
							float	fStartPosX_, 
							float	fStartPosY_) 
							: KGC_PACKET_HEADER( kPacketID_, dwUID_ )
	{
		iSantaLv	= iSantaLv_;
		fStartPosX	= fStartPosX_;
		fStartPosY	= fStartPosY_;
	}
	int								iSantaLv;
	float                           fStartPosX;
	float                           fStartPosY;	
};

DECL_P2P_PACKET_DATA( KGC_PID_GAME_BOARD_UPDATE )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_GAME_BOARD_UPDATE, kSrc_ );
	KGC_PID_GAME_BOARD_UPDATE(  GCPID kPacketID_, DWORD dwUID_, 
		std::map<int, unsigned short> mapMonNum,
		std::map<int, unsigned short> mapMonKillNum, 
		int iMonHuntPoint_ ) 
		: KGC_PACKET_HEADER( kPacketID_, dwUID_ )
    {
		m_mapMonNum = mapMonNum;	
		m_mapMonKillNum = mapMonKillNum;
        iMonHuntPoint = iMonHuntPoint_;
        memset( aDeathNum, 0, sizeof(aDeathNum) );
        memset( aKillNum, 0, sizeof(aKillNum) );
        memset( aTeamScore, 0, sizeof(aTeamScore) );
    }

    ~KGC_PID_GAME_BOARD_UPDATE()
    {
    }
    int iMonHuntPoint;
    UCHAR aKillNum[MAX_PLAYER_NUM];
    UCHAR aDeathNum[MAX_PLAYER_NUM];
    UCHAR aTeamScore[TEAM_NUM];
	std::map< int, unsigned short > m_mapMonNum;
	std::map< int, unsigned short > m_mapMonKillNum;
};

DECL_P2P_PACKET_DATA( KGC_PID_CHAT_PACKET )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_CHAT_PACKET, kSrc_ );
    KGC_PID_CHAT_PACKET( DWORD dwUID_, const std::wstring & strNickName_, const std::wstring & strNickColor_, const std::wstring& strChat_ , D3DCOLOR dwColor_,int ecmd_ )
    {
        dwUID     = dwUID_;
        Type = GC_PID_CHAT_PACKET;
        strChat   = strChat_;
        strNickName = strNickName_;
        strNickColor= strNickColor_;
        dwColor = dwColor_;
		ecmd = ecmd_;
    }
    std::wstring strNickName;
    std::wstring strNickColor;
    std::wstring  strChat;
    D3DCOLOR     dwColor;
	int ecmd;
};

// 호스트가 경기 결과를 보내준다.
DECL_P2P_PACKET_DATA( KGC_PID_DROPITEM )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_DROPITEM, kSrc_ )
    {
        Type            = kSrc_.Type;
        m_vecDropUID    = kSrc_.m_vecDropUID;
        m_fX            = kSrc_.m_fX;
        m_fY            = kSrc_.m_fY;

        return *this;
    }
    GCPID               Type;
    std::vector<DWORD>  m_vecDropUID;
    float               m_fX;
    float               m_fY;
};

struct KGC_PID_QUEST_REAULT_ITEM_DICE
{
    KGC_PID_QUEST_REAULT_ITEM_DICE()
    {
        Type            = GC_PID_QUEST_REAULT_ITEM_DICE;
        bBlank          = false;
        dwPlayerUID     = 0;
        dwItemUID       = 0;
    }

    GCPID			        Type;
    bool                    bBlank;
    DWORD                   dwPlayerUID;
    DWORD                   dwItemUID;
};

struct KGC_PID_QUEST_STAGE_RESULT_SCORE
{
    KGC_PID_QUEST_STAGE_RESULT_SCORE()
    {
        Type              = GC_PID_QUEST_STAGE_RESULT_SCORE;
        ucStage           = 0;
        ucPlayerIndex     = 0;
        usCriticalAttack  = 0;
        usDoubleAttack    = 0;
        usBackAttack      = 0;
        usCounterAttack   = 0;
        usAerialAttack    = 0;
        usAttacked        = 0;
        usAttack          = 0;
        usSpecial1        = 0;
        usSpecial2        = 0;
        usSpecial3        = 0;
    }

    GCPID			Type;
    UCHAR           ucStage;
    UCHAR           ucPlayerIndex;
    USHORT          usCriticalAttack;
    USHORT          usDoubleAttack;
    USHORT          usBackAttack;
    USHORT          usCounterAttack;
    USHORT          usAerialAttack;
    USHORT          usAttacked;
    USHORT          usAttack;
    USHORT          usSpecial1;
    USHORT          usSpecial2;
    USHORT          usSpecial3;
};

struct KGC_PID_QUEST_STAGE_POINT
{
    KGC_PID_QUEST_STAGE_POINT()
    {
        Type              = GC_PID_QUEST_STAGE_POINT;
        ucStage           = 0;
        ucPlayerIndex     = 0;
        usCriticalAttack  = 0;
        usDoubleAttack    = 0;
        usBackAttack      = 0;
        usCounterAttack   = 0;
        usAerialAttack    = 0;
        usAttacked        = 0;
        usAttack          = 0;
        usSpecial1        = 0;
        usSpecial2        = 0;
        usSpecial3        = 0;
    }

    GCPID			Type;
    UCHAR           ucStage;
    UCHAR           ucPlayerIndex;
    USHORT          usCriticalAttack;
    USHORT          usDoubleAttack;
    USHORT          usBackAttack;
    USHORT          usCounterAttack;
    USHORT          usAerialAttack;
    USHORT          usAttacked;
    USHORT          usAttack;
    USHORT          usSpecial1;
    USHORT          usSpecial2;
    USHORT          usSpecial3;
};

struct KGC_PID_DIALOGUE_PROGRESS
{
    KGC_PID_DIALOGUE_PROGRESS()
    {
        Init();
    }

    void Init(void)
    {
        bUse            = false;
        Type            = GC_PID_DIALOGUE_PROGRESS;
        m_sTG_ID        = -1;
        m_sIndex        = 0;
		bSkipping		= false;
        bShow           = false;
    }

    GCPID           Type;
    bool            bUse;
    short           m_sTG_ID;       // 다이얼로그 트리거 아이디
    short           m_sIndex;       // 다이얼로그 인덱스 번호
	bool			bSkipping;
    bool            bShow;          // 다이얼로그 출력유무
};

struct KGC_PID_TG_DEFAULT
{
    KGC_PID_TG_DEFAULT()
    {
        Init();
    }

    void Init( void )
    {
        m_Type              = GC_PID_TG_DEFAULT;
        m_bUse              = false;
        m_sTG_ID            = -1;
        m_sMaxTG_ID         = -1;
        m_iValue            = -1;
    }

    void SetValue(short sTG_ID, int iMaxTG_ID, int iTG_Type )
    {
        m_ucTG_Type      = iTG_Type;
        m_bUse          = true;
        m_sTG_ID        = sTG_ID;
        m_sMaxTG_ID     = iMaxTG_ID;
    }

    void SetExtraValue( int iValue_ )
    {
        m_iValue = iValue_;
    }

    GCPID           m_Type;
    UCHAR           m_ucTG_Type;
    bool            m_bUse;
    short           m_sTG_ID;
    short           m_sMaxTG_ID;
    int             m_iValue;
};

struct KGC_PID_SUMMON_MONSTER
{
    KGC_PID_SUMMON_MONSTER()
    {
        Init();
    }

    void Init()
    {
        Type           = GC_PID_SUMMON_MONSTER;
        m_usMonIndex    = SHRT_MAX;
        m_usMonType     = SHRT_MAX;
        m_iCountValue  = 0;
        m_usEntranceX   = 0;
        m_usEntranceY   = 0;
        m_iLevel        = 0;
        ZeroMemory( m_szState, sizeof(m_szState) );
        m_bIsRight      = false;
        m_bCheckSafePostion = true;
    }

    GCPID           Type;
    USHORT           m_usMonIndex;
    USHORT           m_usMonType;
    USHORT          m_usEntranceX;
    USHORT          m_usEntranceY;
    int             m_iCountValue;
    int             m_iLevel;
    char            m_szState[64];
    bool            m_bIsRight;
    bool            m_bCheckSafePostion;
};

struct KGC_PID_SUMMON_INFINITY_MONSTER
{
    KGC_PID_SUMMON_INFINITY_MONSTER()
    {
        Init();
    }

    void Init()
    {
        Type            = GC_PID_SUMMON_INFINITY_MONSTER;
        m_usMonIndex    = SHRT_MAX;
        m_usMonType     = SHRT_MAX;
        m_usTGid        = SHRT_MAX;
        m_iCountValue   = 0;
        m_usEntranceX   = 0;
        m_usEntranceY   = 0;
        m_iLevel        = 0;
        ZeroMemory( m_szState, sizeof(m_szState) );
        m_bIsRight      = false;
        m_bCheckSafePostion = true;
        m_bBoss         = false;
    }

    GCPID           Type;
    USHORT          m_usMonIndex;
    USHORT          m_usMonType;
    USHORT          m_usTGid;
    USHORT          m_usEntranceX;
    USHORT          m_usEntranceY;
    int             m_iCountValue;
    int             m_iLevel;
    char            m_szState[64];
    bool            m_bIsRight;
    bool            m_bCheckSafePostion;
    bool            m_bBoss;
};


DECL_P2P_PACKET_DATA( KGC_PID_PLAYERGETITEM )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_PLAYERGETITEM, kSrc_ )
    {
        Type                = kSrc_.Type;
        m_cDropTpye         = kSrc_.m_cDropTpye;
        m_dwDropUID         = kSrc_.m_dwDropUID;
        m_dwPlayerUID       = kSrc_.m_dwPlayerUID;
        return *this;
    }

    GCPID           Type;
    char            m_cDropTpye;
    DWORD           m_dwDropUID;
    DWORD           m_dwPlayerUID;
};

struct KGC_PID_GATE_STATE
{
public:
    KGC_PID_GATE_STATE()
    {
        Init();
    }

    void Init()
    {
        Type            = GC_PID_GATE_STATE;
        m_bUsed         = false;
        m_iIndex        = -1;
        m_iHit          = 0;
        m_iState        = 0;
    }

    void SetGateInfo(int iIndex, int iHit, int iStage)
    {
        m_iIndex    = iIndex;
        m_iHit      = iHit;
        m_iState    = iStage;
        m_bUsed     = true;
    }

    bool IsUsed(void) { return m_bUsed; }
    int GetIndex(void) { return m_iIndex; }
    int GetHit(void) { return m_iHit; }
    int GetState(void) { return m_iState; }

private:
    GCPID           Type;
    bool            m_bUsed;             // 이 값이 false인 경우만 패킷을 보낸다.
    short           m_iIndex;
    short           m_iHit;
    short           m_iState;
};

struct KGC_PID_UDP_RELAYSERVER_HEART_BEAT
{
    KGC_PID_UDP_RELAYSERVER_HEART_BEAT()
    {
        Type = GC_PID_UDP_RELAYSERVER_HEART_BEAT;
    }

    GCPID           Type;
};

struct KGC_PID_DAMAGE_PACKET
{
    KGC_PID_DAMAGE_PACKET()
    {
        Type = GC_PID_DAMAGE_PACKET;

        m_sTG_ID            = -1;
    }

    GCPID   Type;
    short     m_sTG_ID;
};

struct KGC_PID_SQUARE_ACTION
{
    KGC_PID_SQUARE_ACTION()
    {
        Init();
        Type = GC_PID_SQUARE_ACTION;
    }

    GCPID       Type;
    // ucState의 마지막 비트에 좌/우 정보를 묻혀서 보냄
    UCHAR ucState;          // 이 상태가
    UCHAR ucRemainFrame;    // 이 프레임 동안 지속될 것임
    USHORT usPosX;          // 이 상태가 기록되었을때의 x좌표
    USHORT usPosY;          // 이 상태가 기록되었을때의 y좌표

    void Init()
    {
        ucState = 0;
        ucRemainFrame = 0;
        usPosX = 0;
        usPosY = 0;
    }
};

struct KGC_PID_SELECT_BONUSBOX
{
    KGC_PID_SELECT_BONUSBOX()
    {
        Type = GC_PID_SELECT_BONUSBOX;
        m_ucSelectIndex = 0;
        m_dwUserUID = 0;
    }

    GCPID           Type;
    UCHAR           m_ucSelectIndex;
    DWORD           m_dwUserUID;
};

DECL_P2P_PACKET_DATA( KGC_PID_OPEN_BONUSBOX )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_OPEN_BONUSBOX, kSrc_ )
    {
        Type                    = kSrc_.Type;
        m_vecBonusBoxSelectInfo   = kSrc_.m_vecBonusBoxSelectInfo;
        return *this;
    }

    GCPID           Type;
    std::vector< std::pair< int, DWORD> > m_vecBonusBoxSelectInfo;
};

struct KGC_PID_READY_BONUSBOX_SELECT
{
    KGC_PID_READY_BONUSBOX_SELECT()
    {
        Type = GC_PID_READY_BONUSBOX_SELECT;
    }
    GCPID           Type;
};

struct KGC_PID_START_BONUSBOX_SELECT
{
    KGC_PID_START_BONUSBOX_SELECT()
    {
        Type = GC_PID_START_BONUSBOX_SELECT;
    }
    GCPID           Type;
};

struct KGC_PID_BROAD_ADD_DAMAGE
{
    KGC_PID_BROAD_ADD_DAMAGE()
    {
        Init();
        Type = GC_PID_BROAD_ADD_DAMAGE;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iPlayerIndex;
    int         iDamageEnum;
    int         iAddDamageType;
    bool        bMonster;
    D3DXVECTOR2 vPos;
    D3DXVECTOR2 vSpeed;

    void Init()
    {
        iPlayerIndex = -1;
        iDamageEnum = -1;
        iAddDamageType = 0;
        bMonster = false;
        vPos = D3DXVECTOR2(0.0f, 0.0f);
        vSpeed = D3DXVECTOR2(0.0f, 0.0f);
    }
};

struct KGC_PID_BROAD_SYNCOBJECT
{
    KGC_PID_BROAD_SYNCOBJECT()
    {
        Init();
        Type = GC_PID_BROAD_SYNCOBJECT;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iObjectID;
    int         iSyncMode;
    float       fX;
    float       fY;

    void Init()
    {
        dwUID = 0;
        iObjectID = -1;
        iSyncMode = -1;
        fX = 0.0f;
        fY = 0.0f;
    }
};

struct KGC_PID_BROAD_RENDER_HIT_NUMBER
{
    KGC_PID_BROAD_RENDER_HIT_NUMBER()
    {
        Init();
        Type = GC_PID_BROAD_RENDER_HIT_NUMBER;
    }

    GCPID       Type;
    DWORD       dwUID;
    int         iUnitID;
    float       fNumber;
    bool        bMPDamage;
    float       fHitPosX;
    float       fHitPosY;

    void Init()
    {
        dwUID = 0;
        iUnitID = -1;
        fNumber = 0.0f;
        bMPDamage = false;
        fHitPosX = 0.0f;
        fHitPosY = 0.0f;
    }
};

struct KGC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST
{
    KGC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST()
    {
        Init();
        Type = GC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST;
    }

    GCPID       Type;
    DWORD       dwUID;
    float       fHP;
    int         iSlotID;

    void Init()
    {
        fHP = 0.0f;
        iSlotID = -1;
    }
};

DECL_P2P_PACKET_DATA( KGC_PID_BROAD_CONTROL_MISSION_DATA )
{
    CON_COPYCON_ASSIGNOP( KGC_PID_BROAD_CONTROL_MISSION_DATA, kSrc_ );
    KGC_PID_BROAD_CONTROL_MISSION_DATA(  GCPID kPacketID_, DWORD dwUID_, 
        USHORT iControlInfoComboCount,
        USHORT iControlInfoBackAttackCount,
        USHORT iControlInfoBeAttackedCount,
        std::map<int, int> mapControlInfoSkill, 
        std::map<int, int> mapControlInfoPotion ) 
        : KGC_PACKET_HEADER( kPacketID_, dwUID_ )
    {
        m_iControlInfoComboCount = iControlInfoComboCount;
        m_iControlInfoBackAttackCount = iControlInfoBackAttackCount;
        m_iControlInfoBeAttackedCount = iControlInfoBeAttackedCount;
        m_mapControlInfoSkill = mapControlInfoSkill;
        m_mapControlInfoPotion = mapControlInfoPotion;
    }

    ~KGC_PID_BROAD_CONTROL_MISSION_DATA()
    {
    }
    USHORT m_iControlInfoComboCount;               //최대콤보 횟수
    USHORT m_iControlInfoBackAttackCount;          //백어택 횟수
    USHORT m_iControlInfoBeAttackedCount;          //피격 횟수
    std::map<int, int> m_mapControlInfoSkill;   //체크할 스킬ID
    std::map<int, int> m_mapControlInfoPotion;  //체크할 포션그룹ID
};

#pragma pack( pop )

class KGCPacketPacker
{
private:
	//변수
	DWORD				  m_dwOffset;
	DWORD				  m_dwPackedSize;
	DWORD				  m_dwPacketInfoSize;
	UCHAR				  m_ucPacketCount;
	KGC_PID_PACKED_PACKET m_kPacket;
	CRITICAL_SECTION	  m_cs;
public:
	//함수
	KGCPacketPacker();
	virtual ~KGCPacketPacker();
	bool	Pack( void* pkPacket_,  PACKED_ELEMENT_SIZE kSize_ );
	int		GetPackCount();
	KGC_PID_PACKED_PACKET* GetData( UINT& uiPacketSize_ );
    UINT    GetPackSize() { return (UINT)(m_dwPacketInfoSize + m_dwPackedSize); }
};

class KGCTCPPacketPacker
{
private:
    //변수
    DWORD				        m_dwOffset;
    DWORD				        m_dwPackedSize;
    DWORD				        m_dwPacketInfoSize;
    UCHAR				        m_ucPacketCount;
    KGC_PID_TCP_PACKED_PACKET   m_kPacket;
    CRITICAL_SECTION	        m_cs;
public:
    //함수
    KGCTCPPacketPacker();
    virtual ~KGCTCPPacketPacker();
    bool	Pack( void* pkPacket_,  PACKED_ELEMENT_SIZE kSize_ );
    int		GetPackCount();
    KGC_PID_TCP_PACKED_PACKET* GetData( UINT& uiPacketSize_ );
    UINT    GetPackSize() { return (UINT)(m_dwPacketInfoSize + m_dwPackedSize); }
};

extern KGC_PID_ADMIN_COMMAND							MyAdminCommand[1];
extern KGC_PID_PLAYERLATENCY_FRAME_INFO					g_kFrameInfoPacket[1];
extern KGC_PID_MONSTER_LATENCY_FRAME_INFO               g_kMonsterFrameInfoPacket[1];
//extern KGC_PID_MONSTER_LIVE_INFO                        g_kMonsterLiveInfoPacket[1];
extern KGC_PID_SHOOTING_ARROW_TOURNAMEN                 ShotPacket[1];
extern KGC_PID_SHOOTING_ARROW_TOURNAMENT_RESULT         ShotScorePacket[1];
extern KGC_PID_COMPRESSED_PACKET						g_kCompressPacket;
extern KGC_PID_PACKED_PACKET							g_kPackedPacket[1];
extern KGC_PID_TCP_PACKED_PACKET                        g_kTCPPackedPacked[1];
extern KGC_PID_EFFECT_ITEM								g_kEffectItem[1];
extern KGC_PID_USEPETSKILL								g_kUsePetSkill[1];
extern KGC_PID_TRANSFORM								g_kTransform[1];
extern KGC_PID_TAG_CHARACTER                            g_kTagCharacter[1];
extern KGC_PID_TRANSNPC									g_kTransNPC[1];
extern KGC_PID_SUMMON_MONSTER                           g_kSummonMonster[1];
extern KGC_PID_DROPITEM                                 g_kDropItem[1];
extern KGC_PID_QUEST_REAULT_ITEM_DICE                   g_kQuestResultItemDice[1];
extern KGC_PID_PLAYERGETITEM                            g_kPlayerGetItem[1];
extern KGC_PID_QUEST_STAGE_RESULT_SCORE                 g_kSendQuestStageResultScore[1];
extern KGC_PID_QUEST_STAGE_POINT                        g_kQuestStageScore[1];
extern KGC_PID_GATE_STATE                               g_kGateStage[1];
extern KGC_PID_POISON_BREATH							g_kPoisonBreath[1];
extern KGC_PID_TG_DEFAULT                               g_kTGPacket[1];
extern KGC_PID_SWAPWEAPON								g_kSwapWeapon[1];
extern KGC_PID_ANGELS_EGG_INFO							g_kAngelsEggInfo[1];
extern KGC_PID_ENTER_GAME								g_kEnterGame[1];
extern KGC_PID_COUPLE_EVENT_CREDIT						g_KCoupleEventCredit[1];
extern KGC_PID_MAGIC_MISSILE							g_KMagicMissile[1];
extern KGC_PID_SELECT_BONUSBOX                          g_kSelectBonusBox[1];
extern KGC_PID_OPEN_BONUSBOX                            g_kOpenBonusBox[1];
extern KGC_PID_START_BONUSBOX_SELECT                    g_kStartBonusBoxSelect[1];
extern KGC_PID_READY_BONUSBOX_SELECT                    g_kReadyBonusBoxSelect[1];
extern KGC_PID_CHANGE_MONSTER                           g_kChangeMonster[1];
extern KGC_PID_SHIELD                                   g_kShield[1];
extern KGC_PID_SUMMON_INFINITY_MONSTER                  g_kSummonInfinityMonster[1];

extern KGCPacketPacker									g_kPacketPacker;
extern KPlayerLatency									g_kLocalPlayerLatency[1];
extern KGCTCPPacketPacker   						    g_kTGPacketPacker;
extern KGCTCPPacketPacker                               g_kSquarePacketPacker;
#endif // _PACKET_H_