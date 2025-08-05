#pragma once

#include "CommonPacket.h"

#pragma pack( push, 1 )

DECL_PACKET(ECN_VERIFY_ACCOUNT_REQ)
{
	int                 m_nProtocolVer;
	int                 m_nUserProtocolVer;
	std::wstring        m_strName;
	std::wstring        m_strIP;
	unsigned short      m_usPort;
	DWORD               m_dwUID;
};

DECL_PACKET(ECN_VERIFY_ACCOUNT_ACK)
{
	int             m_nOK;
	KServerInfo     m_kServerInfo;
	std::vector<KSquareInfo> m_vecSquares;
};

DECL_PACKET(ECN_KICK_USER_NOT)
{
	DWORD   m_dwDestUserUID;
	int     m_nReason;
	std::wstring m_wstrLogin;
};

// 한시간 이벤트 정보.
DECL_PACKET(ECN_REPETITION_PROVIDE_REQ) // center -> centerPrx
{
	DWORD       m_dwEventUID;
	int         m_nCommentID;
};

DECL_DATA(KMiniGameRankList)
{
	std::map< char, std::vector<KMiniGameRankInfo> > m_mapRank;
};

typedef std::map<int, int>   MAP_INT_INT;

DECL_DATA(KConnectedUser)
{
	DWORD       m_dwUID;
};

DECL_PACKET(ECN_RAINBOW_UPDATE_NOT) // Center -> Center Proxy
{
	int                         m_nRainBowID;
	int                         m_nDailyEventID;
	int                         m_nJackPotEventID;
	std::vector<KRainBowData>   m_vecEventList;
};

// Mission Event
DECL_PACKET(ECN_MISSION_EVENT_UPDATE_NOT) // Center -> Center Proxy
{
	std::vector<KMissionDailyData>    m_vecMissions;
	std::set<int>                   m_setItemList;
	bool                            m_bDateChanged;
};
typedef std::map< int, KSubscription >  KECN_SUBSCRIPTION_INFO_UPDATE_NOT;

DECL_PACKET(ECN_MISSION_NOTIFY_NOT) // Center -> Center Proxy
{
	DWORD                       m_dwTime;        // 몇분전의 알림 공지인지 보내주자~!
};

typedef std::map<int, std::vector< KDropItemInfo > >   KECN_NPC_GIFT_UPDATE_NOT;
DECL_PACKET(ECN_ITEM_DONATION_UPDATE_NOT) // Center -> Center Proxy
{
	std::vector<KItemDonationInfo>   m_vecGradeInfo;        // 전체 기부 등급 정보.
};

DECL_PACKET(ECN_ENTER_SQUARE_ACK) // Center -> Center Proxy
{
	int                             m_nOK;
	DWORD                           m_dwUserUID;
	DWORD                           m_dwSquareUID;
	std::vector< KSquareUserInfo >  m_vecUserInfo;
	DWORD                           m_dwRelayServerIP;
	USHORT                          m_usRelayServerPort;
	KSquarePos                      m_kMyPos;
};

DECL_PACKET(ECN_ENTER_SQUARE_BROAD) // Center -> Center Proxy
{
};

DECL_PACKET(ECN_LEAVE_SQUARE_ACK) // Center -> Center Proxy
{
};

DECL_PACKET(ECN_LEAVE_SQUARE_BROAD) // Center -> Center Proxy
{
	DWORD               m_dwUID;
	DWORD               m_dwSquareUID;
};

DECL_PACKET(ECN_UPDATE_NEIGHBORHOOD_SQUARE_POS_NOT)
{
	DWORD m_dwUID;
	std::vector<DWORD> m_vecNeighborhood;
};

DECL_PACKET(ECN_ENTER_GUILD_SQUARE_REQ)
{
	KSquareUserInfo m_kSquareUserInfo;
	int             m_iGuildSquareIndex;
};

DECL_PACKET(ECN_ENTER_GUILD_SQUARE_ACK)
{
	KECN_ENTER_SQUARE_ACK   m_kEnterSquare;
	KSquareInfo             m_kSquareInfo;
};

DECL_PACKET(ECN_ADD_GUILD_USER_REQ)
{
	DWORD            m_dwGuildUID;
	KNGuildUserInfo  m_kUserInfo;
};

DECL_PACKET(ECN_CHAT_REQ)
{
	KChatData               m_kChatData;
	DWORD                   m_dwServerUID;
};

DECL_PACKET(ECN_CHAT_ACK)
{
	int                     m_nOK;
	KChatData               m_kChatData;
	DWORD                   m_dwServerUID;
};

DECL_DATA(KECNBillboardChat)
{
	std::wstring	Nickname;
	DWORD			MessageColor;
	std::wstring	Message;
};

DECL_PACKET(ECN_LEAVE_ROOM_ACK)
{
	int                     m_nOK;
	DWORD                   m_dwUserUID;
};

DECL_PACKET(ECN_JOIN_ROOM_BROAD)
{
	USHORT              m_usRoomID;
	KInDoorUserInfo     m_kIndoorUserInfo;
};

DECL_PACKET(ECN_LEAVE_ROOM_BROAD)
{
	USHORT          m_usRoomID;
	DWORD           m_dwUserUID;
};

DECL_PACKET(ECN_CHANGE_ROOMUSER_INFO_REQ)
{
	KInDoorUserInfo     m_kInDoorUserInfo;
	KChangeRoomUserInfo m_kChangeRoomUserInfo;
};

DECL_PACKET(ECN_CHANGE_ROOMUSER_INFO_BROAD)
{
	int             m_nOK;
	USHORT          m_usRoomID;
	unsigned char   m_ucChangeType; //변경되는 목록의 타입
	KInDoorUserInfo m_kInDoorUserInfo;
};

DECL_PACKET(ECN_HOST_MIGRATED_NOT)
{
	USHORT          m_usRoomID;
	DWORD           m_dwHostUID;
};

DECL_PACKET(ECN_START_GAME_BROAD)
{
	int                         m_nOK;
	USHORT                      m_usRoomID;
	DWORD                       m_dwRandomSeed; // Time
	std::vector<DWORD>			m_vecStartingUsers;
	DWORD                       m_dwUniqueNumber;
	DWORD                       m_dwUserUID;
};

DECL_PACKET(ECN_LEAVE_GAME_REQ)
{
	DWORD           m_dwUserUID;
	USHORT          m_usRoomID;
	KGameResultIn   m_kGameResult;
};

typedef std::vector<std::pair<DWORD, DWORD> > VEC_PAIR_DWORD; // 맵, 페어는 패킷 없이 보낼수 없다.. 패킷 만들기 싫다..

DECL_PACKET(ECN_PREMIUM_NOT)
{
	USHORT m_usRoomID;
	USER_PREMIUM_INFO m_mapPremium;
};

DECL_PACKET(ECN_ACCEPT_GUILD_JOINER_NOT)
{
	int             m_iGuildID;         // 길드ID
	SET_DWORD       m_setNewcomerList;  // 신입 길드원 리스트
};

DECL_PACKET(ECN_REJECT_GUILD_JOINER_NOT)
{
	int             m_iGuildID;         // 길드ID
	SET_DWORD       m_setRejectList;    // 가입 거절 유저 리스트
};

DECL_PACKET(ECN_GUILD_MEMEBER_LIST_NOT)
{
	DWORD                               m_dwGuildUID;       // 길드UID
	std::map<DWORD, KNGuildUserInfo>    m_mapGuildUserList; // 맴버리스트 map<UserUID,UserInfo>
};

DECL_DATA(KNOneGuildData)
{
	KNGuildInfo                     m_kGuildInfo;       // 길드정보
	std::vector<KNGuildNotice>      m_vecGuildNotice;   // 길드공지
	std::map<DWORD, KNGuildUserInfo> m_mapGuildUserList; // 길드맴버정보
};

DECL_PACKET(ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT)
{
	DWORD           m_dwGuildUID;       // 길드UID
	DWORD           m_dwUserUID;        // 유저UID
	std::wstring    m_strUserComment;   // 해당 길드유저의 변경될 자기 소갯글
};

DECL_PACKET(ECN_GUILD_LIST_NOT)
{
	std::map< DWORD, KNGuildInfo >                m_mapGuildList;       // 전체 길드 정보
	std::map< DWORD, std::vector<KNGuildNotice> > m_mapGuildNotice;     // 전체 길드 공지
	std::map< DWORD, std::map<DWORD, KNGuildUserInfo> >  m_mapGuildMember;     // 온라인 상태의 길드맴버 리스트
};

typedef std::pair<DWORD, std::vector<KSquareInfo> > KECN_SQUARE_LIST_ACK;

DECL_PACKET(ECN_DRUMOUT_GUILD_USER_NOT)
{
	DWORD           m_dwGuildUID;    // 길드UID
	std::set<DWORD> m_setUserUID;   // 탈퇴된 유저UID리스트
	char            m_cOutType;     // 탈퇴 경위
};

DECL_PACKET(ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT)
{
	DWORD                           m_dwGuildUID;            // 길드UID
	std::map<char, std::set<DWORD> > m_mapChangeMemberLevel; // 길드맴버레벨 변경정보 map< MemberLevel, set<UserUID> >
};

DECL_PACKET(ECN_UPDATE_GUILD_NOTICE_NOT)
{
	DWORD                           m_dwGuildUID;   // 길드UID
	std::vector<KNGuildNotice>      m_vecNotice;    // 길드공지
};

DECL_PACKET(ECN_UPDATE_GUILD_NAME_NOT)
{
	DWORD                           m_dwGuildUID;   // 길드UID
	std::wstring                    m_strGuildName; // 길드이름
};

DECL_PACKET(ECN_JOIN_GUILD_REQ)
{
	DWORD           m_dwUserUID;        // 가입신청자 유저UID
	DWORD           m_dwGuildUID;       // 가입할 길드의 ID
	std::wstring    m_strMyComment;     // 자기소개
	KNGuildInfo     m_kGuildInfo;       // 가입할 길드의 정보
};

DECL_PACKET(ECN_JOIN_GUILD_ACK)
{
	int             m_nOK;              // 길드가입 결과
	DWORD           m_dwUserUID;        // 가입신청자 유저UID
	DWORD           m_dwGuildUID;       // 가입할 길드의 ID
	KNGuildUserInfo m_kGuildUserInfo;   // 내 길드원 정보(가입성공시 전달)
	KNGuildInfo     m_kGuildInfo;       // 가입한 길드의 정보
};

DECL_PACKET(ECN_ADD_GUILD_USER_BROAD_NOT)
{
	DWORD           m_dwGuildUID;       // 길드UID
	KNGuildUserInfo m_kGuildUserInfo;   // 추가된 길드원 정보
};

DECL_PACKET(ECN_CANCEL_JOIN_GUILD_ACK)
{
	int             m_nOK;              // 길드가입취소 결과
	DWORD           m_dwUserUID;        // 유저UID
	DWORD           m_dwGuildUID;       // 길드UID
};

DECL_PACKET(ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ)
{
	DWORD                               m_dwGuildUID;           // 길드UID
	DWORD                               m_dwGuildMasterUID;     // 길드마스터UID
	std::map< char, std::set<DWORD> >   m_mapChangeMemberLevel; // map< 변경할 맴버레벨, set<UserUID> >
};

DECL_PACKET(ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK)
{
	int                                 m_nOK;                  // 결과
	DWORD                               m_dwGuildUID;           // 길드UID
	DWORD                               m_dwGuildMasterUID;     // 길드마스터UID
	std::map< char, std::set<DWORD> >   m_mapChangeMemberLevel; // map< 변경할 맴버레벨, set<UserUID> >
};

DECL_PACKET(ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT)
{
	DWORD                               m_dwGuildUID;           // 길드UID
	std::map< char, std::set<DWORD> >   m_mapChangeMemberLevel; // map< 변경할 맴버레벨, set<UserUID> >
	char                                m_cOutType;             // 탈퇴시,탈퇴경위.
};

typedef KECN_CANCEL_JOIN_GUILD_ACK  KECN_SELF_DRUMOUT_GUILD_ACK; // 탈퇴신청결과

DECL_PACKET(ECN_ACCEPT_GUILD_JOINER_REQ)
{
	DWORD           m_dwUserUID;        // UserUID
	DWORD           m_dwGuildUID;       // GuildUID
	std::set<DWORD> m_setUserUID;       // 대상 유저UID
};

DECL_PACKET(ECN_ACCEPT_GUILD_JOINER_ACK)
{
	int             m_nOK;              // 승인결과
	DWORD           m_dwUserUID;        // UserUID
	DWORD           m_dwGuildUID;       // GuildUID
	std::set<DWORD> m_setUserUID;       // 처리된 유저UID
};

typedef KECN_ACCEPT_GUILD_JOINER_REQ    KECN_REJECT_GUILD_JOINER_REQ; // 가입거절
typedef KECN_ACCEPT_GUILD_JOINER_ACK    KECN_REJECT_GUILD_JOINER_ACK;

DECL_PACKET(ECN_BREAKUP_GUILD_REQ)
{
	DWORD           m_dwUserUID;        // 유저UID
	DWORD           m_dwGuildUID;       // 해체될 자기 길드UID
};

DECL_PACKET(ECN_BREAKUP_GUILD_ACK)
{
	int             m_nOK;              // 결과
	DWORD           m_dwUserUID;        // 유저UID
	DWORD           m_dwGuildUID;       // 해체될 자기 길드UID
};

DECL_DATA(KCnNGuildJoinPolicy)
{
	DWORD m_dwUserUID;
	KNGuildJoinPolicy m_kPolicy;
};

DECL_PACKET(ECN_UPDATE_GUILD_MEMBER_LIST_REQ)
{
	DWORD                               m_dwGuildUID;       // 길드UID
	std::map<DWORD, KNGuildUserInfo>    m_mapMemberList;    // 맴버리스트
};

typedef KECN_REJECT_GUILD_JOINER_REQ    KECN_AUTO_REJECT_GUILD_JOINER_REQ; // 신청기간체크에따른 자동가입거절

DECL_PACKET(ECN_LOG_OUT_NOT) // Center Proxy -> Center
{
	std::wstring    m_strLogin;
	std::wstring    m_strPassword;
	DWORD           m_dwUID;
	DWORD           m_dwIPNum;
	DWORD           m_dwAuthType;
};

DECL_PACKET(ECN_QUIZ_BINGO_DATA_NOT)
{
	int                                 m_nEventID;
	time_t                              m_tmBegin;
	time_t                              m_tmEnd;
	std::map<int, KSerBuffer>           m_mapCompressedQuestionList;
};

DECL_PACKET(ECN_UPGRADE_GUILD_GRADE_GIFT_REQ)
{
	DWORD           m_dwGuildUID;       // 길드UID
	DWORD           m_dwGuildMasterUID; // 길마UID
	KDropItemInfo   m_kPostItem;        // 요청아이템(캐릭터 분리에서 사용하지 않음)
};

DECL_PACKET(ECN_UPGRADE_GUILD_GRADE_GIFT_ACK)
{
	int                 m_nOK;              // 결과
	DWORD               m_dwGuildUID;       // 길드UID
	DWORD               m_dwGuildMasterUID; // 길마UID
	__int64             m_WUID;           // Wait UID(캐릭터 분리에서 사용하지 않음)
};

DECL_PACKET(ECN_SYNC_INDOORUSERINFO_BROAD)
{
	USHORT    m_usRoomID;
	std::map< DWORD, KInDoorUserInfo> m_mapInDoorUserInfo;
};

DECL_PACKET(ECN_REMOVE_GUILD_USER_BROAD_NOT)
{
	DWORD       m_dwGuildUID;   // 길드UID
	DWORD       m_dwUserUID;    // 유저UID
	char        m_cOutType;     // 탈퇴경위
};

typedef KECN_REMOVE_GUILD_USER_BROAD_NOT    KECN_DRUMOUT_GUILD_USER_BROAD_NOT;

// map< GuildUID, map<UserUID,UserInfo> > : 게임서버 저장된 길드별 맴버리스트 정보
typedef std::map< DWORD, std::map<DWORD, KNGuildUserInfo> >  KECN_GUILD_MEMEBER_LIST_SYNC_NOT;

DECL_PACKET(ECN_INVITE_GUILD_USER_REQ)
{
	DWORD                   m_dwUserUID;
	DWORD                   m_dwGuildUID;
	std::vector<DWORD>      m_vecInviteList;
	KInviteInfo             m_kInviteInfo;
};

DECL_PACKET(ECN_INVITE_GUILD_USER_ACK)
{
	int         m_nOK;
	std::vector<DWORD>      m_vecUserUID;
	KInviteInfo             m_kInviteInfo;
};

DECL_PACKET(ECN_UPDATE_GUILD_POINT_REQ)
{
	GCGuildUID          m_GuildUID;         // 길드 UID
	DWORD               m_dwUserUID;        // 보내는 유저의 UID
	DWORD               m_dwDiffPoint;      // 길드 포인트 변화량
	DWORD               m_dwGuildPoint;      // 길드 포인트
	DWORD               m_dwBattlePoint;    // 배틀 포인트
};

DECL_DATA(KGuildPageInfo)
{
	UCHAR                       m_ucSortType;   // 정렬타입
	int                         m_nPageNum;     // 페이지번호
	KSerBuffer                  m_kPageBuffer;  // 페이지버퍼
};

DECL_PACKET(ECN_SQUARE_USER_SYNC_ACK)
{
	KSquarePos m_kUserPos;
	std::vector<KPosUID> m_vecPosUID;
};

typedef std::map< DWORD, KNGuildInfo >                      KECN_GUILD_INFO_LIST_NOT;
typedef std::map< DWORD, std::vector<KNGuildNotice> >       KECN_GUILD_NOTICE_LIST_NOT;
typedef std::map< DWORD, std::map<DWORD, KNGuildUserInfo> >  KECN_GUILD_MEMBER_LIST_NOT;
typedef std::map<DWORD, int>   MAP_DWORD_INT;

DECL_PACKET(ECN_DEL_USER_REQ)
{
	enum DEL_REASON {
		DR_NORMAL,              // 정상종료
		DR_DUPLICATION_CENTER,  // 이중접속(Center)
		DR_DUPLICATION_GAME,    // 이중접속(Game)

		DR_MAX,
	};

	DWORD       m_dwUserUID;
	int         m_nReason;
	std::string m_strName;
	//std::string m_strServerIP;
};

DECL_PACKET(ECN_SOCKET_ITEM_GP_TABLE_NOT)
{
	std::map< DWORD, KSocketConsumeInfo > m_mapItemUseCount;
	std::map< DWORD, KSocketConsumeInfo > m_mapConsumeGP;
};

DECL_PACKET(ECN_GUILD_PAGE_NOT)
{
	UCHAR       m_ucSortType;   // 정렬타입
	KSerBuffer  m_kPageBuffer;  // 페이지버퍼 std::vector<DWORD>
};

DECL_PACKET(ECN_USER_SERVER_ROOM_ACK)
{
	int                 m_nOK;
	KSendUser           m_kSendUser;
	KUserServerRoomInfo m_kServerRoomInfo;
};

DECL_PACKET(ECN_USER_SERVER_ROOM_REQ)
{
	KSendUser           m_kSendUser;
	KUserServerRoomInfo m_kTargetUser;
};

typedef std::map< DWORD, std::set<GCITEMID> >   MAP_GCITEMID_SET;
typedef std::set<GCITEMID>  KECN_UPDATE_BUY_LIMIT_ITEM_LIST_NOT;

DECL_PACKET(ECN_SPHINX_DATA_NOT)
{
	int                                 m_nVersion;
	time_t                              m_tmBegin;
	time_t                              m_tmEnd;
	std::map<int, KSerBuffer>           m_mapCompressedSphinxQuestionInfo;
};

DECL_PACKET(ECN_USER_EXIT_GAME_NOT)
{
	std::wstring                            m_wstrLogin;
	std::wstring                            m_wstrIP;
	int                                     m_nGameCode;
	__int64                                 m_biUniqueKey;
	int                                     m_nUniqueKey;
};

DECL_PACKET(ECN_SURVEY_DATA_NOT)
{
	int                                 m_nVersion;
	time_t                              m_tmBegin;
	time_t                              m_tmEnd;
	time_t                              m_tmReturnTime;
	std::map< int, std::map< int, KSerBuffer > >  m_mapCompressedSurveyInfoPack;
	std::map< int, std::map< std::pair<int, int>, std::vector<int> > > m_mapSurveyTypeInfo; // 각 타입별 설문지 정보.
	std::map< int, std::vector< KDropItemInfo > > m_mapSurveyRewardInfo; // 각 타입별 보상 정보.
};

DECL_PACKET(ECN_GWC_EVENT_DATA_NOT)
{
	DWORD                                           m_dwCountryID;
	std::wstring                                    m_wstrCountryName;
	time_t                                          m_tmBegin;
	time_t                                          m_tmEnd;
	time_t                                          m_tmDBUpdateTime;
	std::map<DWORD, std::pair<float, float>>         m_mapGradeRewardInfo;
	std::map<DWORD, std::vector<KDropItemInfo >>    m_mapRankingRewardInfo;
	std::map<DWORD, KGWCCountryInfo>                m_mapTotalCountryInfo;
	std::map<DWORD, std::pair<DWORD, DWORD>>         m_mapGradeDesc;
};

DECL_PACKET(ECN_ADVENTURE_DATA_NOT)
{
	std::map< PAIR_DWORD, KAdventureMonsterInfo >   m_mapAdventureMonsterInfo;     // 몬스터 정보;     key: pair< m_dwDungeonID, m_dwMonsterID >
	std::map< int, KContinentRewardInfo >           m_mapContinentRewardInfo;      // 대륙 보상 정보;  key: m_nContinentID
	KDropItemInfo                                   m_kFinalRewardInfo;            // 최종 보상 정보;
	std::map< int, KAdventureEventDate >            m_mapAdventureEventDate;       // 모험 이벤트 기간 정보; key: EventID(발동 세부 기간; 게임서버는 요 데이터만 유지하면 된다)
};

DECL_PACKET(ECN_OLYMPIC_RANK_DATA_NOT)
{
	std::vector<KRegDateRankInfo>                   m_vecRegDateRank;
	std::vector<KTotalRankInfo>                     m_vecTotalRank;
};

DECL_PACKET(ECN_OLYMPIC_SYNC_SOLO_DATA_NOT)
{
	time_t                                          m_tmTodayDate;
	int                                             m_nGameType; // GameType
	int                                             m_nTeamType; // TeamType
	int                                             m_nTopPlayerData;
	std::wstring                                    m_strNickName; //NickName
};

DECL_PACKET(ECN_OLYMPIC_SYNC_TEAM_DATA_REQ)
{
	time_t                                          m_tmTodayDate;
	int                                             m_nGameType; // GameType
	int                                             m_nTeamType; // TeamType
	int                                             m_nDeltaData;
};

DECL_PACKET(ECN_OLYMPIC_SYNC_TEAM_DATA_ACK)
{
	time_t                                          m_tmTodayDate;
	int                                             m_nGameType; // GameType
	int                                             m_nTeamType; // TeamType
	__int64                                         m_biTotalData;
};

DECL_PACKET(ECN_OLYMPIC_EVENT_DATA_NOT)
{
	time_t                                          m_tmRegDate;
	int                                             m_nGameType;        // GameType
	int                                             m_nTeamType;        // TeamType
	std::wstring                                    m_strNickName;      // 개인전 1등 유저 닉네임.
	__int64                                         m_biTotalData;
	std::vector<KRegDateRankInfo>                   m_vecRegDateRank;
	std::vector<KTotalRankInfo>                     m_vecTotalRank;

	KECN_OLYMPIC_EVENT_DATA_NOT() :m_tmRegDate(0)
		, m_nGameType(0)
		, m_nTeamType(0)
		, m_biTotalData(0)
	{
		m_strNickName = L" ";
		m_vecRegDateRank.clear();
		m_vecTotalRank.clear();
	}
};

DECL_PACKET(ECN_BUFF_DONATION_TOTAL_DATA_NOT)
{
	__int64 m_biTotalDonationPoint;
	int     m_nBuffStateFlag;
};

DECL_PACKET(ECN_ACCUMULATE_ATTENDANCE_SETTING_DATA_NOT)
{
	int    m_nVersion;
	time_t m_tmBegin;
	time_t m_tmEnd;
};

typedef std::vector< PAIR_INT_STRING > KECN_PLAN_SCRIPT_UPDATE_NOT;

DECL_PACKET(ECN_ADD_USER_REQ)
{
	DWORD m_dwUID;
	std::string m_strName;
	std::string m_strServerIP;
	std::string m_strPassword;
};

DECL_PACKET(ECN_CYOU_USER_LOGIN_ACK)
{
	DWORD m_dwUID;
	int m_nOK;

	KECN_CYOU_USER_LOGIN_ACK() :
		m_dwUID(0)
		, m_nOK(0)
	{}
};

DECL_PACKET(ECN_CYOU_USER_HEART_BEAT_ACK)
{
	DWORD m_dwUID;
	int m_nOK;

	KECN_CYOU_USER_HEART_BEAT_ACK()
		:m_dwUID(0)
		, m_nOK(0)
	{}
};

DECL_PACKET(ECN_CONNECTION_GIFTBOX_INFO_NOT)
{
	bool                                        m_bUseGiftFirstCharacter;
	std::map< int, KCharGiftItem >              m_mapGiftFirstCharacter;
	bool                                        m_bUseGiftNewUser;
	time_t                                      m_tmDecideBeginTimeNewUser;
	int                                         m_nNewUserEffectDuration;
	std::map< int, KGiftGradeInfo >             m_mapGiftNewUser;
	int                                         m_nGiftEventUserVersion;
	time_t                                      m_tmGiftEventBegin;
	time_t                                      m_tmGiftEventEnd;
	std::map< int, KGiftGradeInfo >             m_mapGiftEventUser;
};

DECL_PACKET(ECN_EVENT_DB_UPDATE_NOT)
{
	// 추후 다른 이벤트에도 사용할 수 있도록 확장성 고려하여 EventType 추가.
	enum EVENT_TYPE {
		ET_VIP_EVENT = 12,
		ET_MAX,
	};
	int     m_nEventType;
};

DECL_PACKET(ECN_TONG_DONATION_INFO_NOT)
{
	time_t                                  m_tmVersion;                // 버전으로 사용할 시간
	time_t                                  m_tmBegin;                  // 기부 시작 날짜
	time_t                                  m_tmEnd;                    // 기부 종료 날짜
	DWORD                                   m_dwClientUIType;           // 클라이언트 UI 타입
	std::map< DWORD, KTongDonationNPCInfo > m_mapNPCs;                  // 통합 기부 NPC 정보( key: NPCType )
};

typedef std::map<int, KInformationDataOfSellType> KECN_SOCIAL_COMMERCE_INIT_DATA_NOT;

DECL_DATA(KItemProvideUserData)
{
	VEC_REWARD m_vecRewardItem; // 보상 아이템 목록
	DWORD      m_dwUserUID;     // 보상 아이템을 지급할 유저UID
	int        m_nGetType;      // 아이템 획득 경로
};

// ItemProvider 에 전달용으로 사용할 보상정보벡터
typedef std::vector< KItemProvideUserData > VEC_FOR_REWARD_PROVIDE;

DECL_DATA(KRewardUserListDataForSocialCommerce)
{
	KItemProvideUserData kProvideData;  // 보상 정보
	DWORD                m_dwRewardUID; // 공동구매의 동일 아이템 여러번 구매인지 DB에서 체크 확인 필요해서
	DWORD                m_dwEventUID;
};

// 소셜커머스 공동구매 보상 전용 정보 벡터
typedef std::vector< KRewardUserListDataForSocialCommerce > VEC_GROUP_PURCHASE_REWARD;

DECL_PACKET(DB_ECN_SOCIAL_COMMERCE_GROUPPURCHASE_USER_REQ)
{
	DWORD m_dwEventUID;
	int   m_nGetType;   // 아이템 획득 경로
};

// ItemProvider에서 DB thread 처리 요청 할 때 사용 하는 패킷
DECL_PACKET(DB_ECN_ITEM_PROVIDE_REQ)
{
	VEC_FOR_REWARD_PROVIDE m_vecProvideInfo;
	int                    m_nOneTimeProvisionUserCount;
};

// 던전 랭킹 시스템
DECL_PACKET(ECN_DUNGEON_RANK_RANKING_CONFIG_NOT)
{
	int                                     m_nRankEntryPerPage; // 한 페이지당 표시할 랭크 개수(클라 UI의 제약으로 고정하여 사용한다; 8개)
	std::set< int >                         m_setRankingModes; // 랭킹을 처리할 던전 리스트 < ModeID >
};

typedef std::map< int, std::list< KDungeonRankInfo > > KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT;

typedef std::vector< KDungeonRankInfo > KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ;

typedef KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT KECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT;

#pragma pack( pop )
