#ifdef __NEVER_DEFINED_MACRO__
#define SI_ENUM(VAL, VAL2) VAL,
enum
{
#endif

	SI_ENUM(SI_CPL, CPL 통계 입니다.)
	SI_ENUM(SI_PLAY_TIME, Play Time 통계 입니다.)
	SI_ENUM(SI_MAP_COUNT, map Count 입니다.)
	SI_ENUM(SI_LOGINOUT_STAT, LoginOut Stat 입니다.) // tslayer
	SI_ENUM(SI_STAT_EMOTICON, 이모티콘 통계 입니다.)
	SI_ENUM(SI_STAT_ITEM_GET, 아이템 획득 통계용.)
	SI_ENUM(SI_STAT_GAMELOADING, 게임 로딩 통계.)
	SI_ENUM(SI_STAT_DEATH, DeathMatch.)
	SI_ENUM(SI_STAT_CHECK_IN, 패킷 중복처리 요청 통계)
	SI_ENUM(SI_STAT_P2P_CRITICAL, P2P통신중 패킷 읽기 오류)
	SI_ENUM(SI_STAT_MINIGAME, 미니게임 통계)
	SI_ENUM(SI_STAT_SPX, SPX 통계)
	SI_ENUM(SI_STAT_DISCONNECT, 종료 통계)
	SI_ENUM(SI_STAT_LEAVE_GAME, 게임 종료 정보)
	SI_ENUM(SI_STAT_COUPLE_ROOM, 커플 창 열기에 대한 통계.)
	SI_ENUM(SI_STAT_GAME_START_GAP, 게임 시작까지 걸리는 시간)
	SI_ENUM(SI_STAT_PING_STAT, 클라이언트 PING Stat 입니다.) // tgkwon
	SI_ENUM(SI_STAT_DUNGEON, 던전 통계)
	SI_ENUM(SI_STAT_LOADINGINFO, 게임로딩 전체 정보.) // 게임시작시, Stage이동시에 성공/실패에 대한 전체 로그를 남긴다.
	SI_ENUM(SI_STAT_ROOMJOIN, 방입장 종류별 카운트 통계.)
	SI_ENUM(SI_STAT_GP, GP 획득 소비 통계.)
	SI_ENUM(SI_STAT_CHARACTER, 캐릭터 통계.)
	SI_ENUM(SI_STAT_CHAR_EQUIP, 캐릭터 장착아이템 통계.)
	SI_ENUM(SI_STAT_NETWORK, 네트워크 관련 통계.)
	SI_ENUM(SI_STAT_FATAL_DEATH, FatalDeathMatch.)
	SI_ENUM(SI_STAT_PVP_PLAY_CHARACTER, 대전 플레이 캐릭터 타입 통계.)

	SI_ENUM(SI_SENTINEL, 이녀석은 마지막이라... 불려지면 안되는디..)
	//--------------------------------------------------------------

#ifdef __NEVER_DEFINED_MACRO__
};
#undef SI_ENUM
#endif