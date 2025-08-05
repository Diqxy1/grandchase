#pragma once

#if defined( _TRUNK )
#define NATION_KOREA
#endif
//#include <boost/mpl/int.hpp>
//#include <boost/mpl/plus.hpp>
//#include <boost/mpl/minus.hpp>
//#include <boost/mpl/multiplies.hpp>


//------------------------------------------------------------------------------------------------------------------------
// 매크로 정의 확인
// 서비스 매크로가 정의되어 있는지 확인한다.
#ifdef SERVICE
#   pragma message( "Macro SERVICE is defined!" )
#endif // SERVICE

// 설치 경로 확인 매크로가 정의되어 있는지 확인한다.
#ifdef __PATH__
#   pragma message( "Macro __PATH__ is defined!" )
#endif // __PATH__

// 사내 테스트 버전(알파)인지 확인한다.
#if defined( SERVICE ) && !defined( __PATH__ ) && !defined( _OPEN_TEST_ )
#   undef _DISABLE_HACK_CHECK_
#   define _DISABLE_ONLY_ONE_EXECUTE_
#   pragma message( "This is alpha test version." )
//#define BINARY_LOAD // UI Binary Load for Test
#define _LOADING_TIMECHECK_
#endif // SERVICE && !__PATH__ && !_OPEN_TEST_

// 오픈 테스트 버전(베타)인지 확인한다.
#if defined( SERVICE ) && defined( __PATH__ ) && defined( _OPEN_TEST_ )
#   undef _DISABLE_HACK_CHECK_
#   undef _DISABLE_ONLY_ONE_EXECUTE_
#   pragma message( "This is beta test version." )
//#define BINARY_LOAD // UI Binary Load for Test
#endif // SERVICE && __PATH__ && _OPEN_TEST_

// 서비스 버전인지 확인한다.
#if defined( SERVICE ) && defined( __PATH__ ) && !defined( _OPEN_TEST_ )
#   undef _DISABLE_HACK_CHECK_
#   undef _DISABLE_ONLY_ONE_EXECUTE_
#   pragma message( "This is original service version." )
#endif // SERVICE && __PATH__ && !_OPEN_TEST_

#if ( !defined ( NO_GAMEGUARD ) || !defined ( NO_HACKSHIELD ) || defined( USE_XTRAP ) ) && !defined ( _DISABLE_HACK_CHECK_ )
#   define _DISABLE_HACK_CHECK_
#endif

// 해킹 검사 해제 매크로가 정의되어 있는지 확인한다.
#ifdef _DISABLE_HACK_CHECK_
#   pragma message( "Macro _DISABLE_HACK_CHECK_ is defined!" )
#endif // _DISABLE_HACK_CHECK_

// 중복 실행 해제 매크로가 정의되어 있는지 확인한다.
#ifdef _DISABLE_ONLY_ONE_EXECUTE_
#   pragma message( "Macro _DISABLE_ONLY_ONE_EXECUTE_ is defined!" )
#endif // _DISABLE_ONLY_ONE_EXECUTE_

// -----------------------------------------------------------------------------
// 국가코드
// -----------------------------------------------------------------------------
#   define NATION_CODE L"PH"

//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#   define _ENGLISH             // 영어
#   pragma message( "This is PHILIPPINE version!" )
#   define NATION_NAME          "Philippine"

//------------------------------------------------------------------------------------------------------------------------
// 언어별 코드 페이지 정의
// ----------------------------
//  ANSI Code-Page Identifiers
// ----------------------------
// Korean                 - 949
// Japanese               - 932
// Chinese(Simplified)    - 936
// Chinese(Traditional)   - 950
// Portuguese             - 860
// English(United States) - 437
// ----------------------------

// --------------------------------------------------------------------------------------
// Examples
// --------------------------------------------------------------------------------------
// Korean                 - IsDBCSLeadByteEx( 949, ... ), WideCharToMultiByte( 949, ... )
// Japanese               - IsDBCSLeadByteEx( 932, ... ), WideCharToMultiByte( 932, ... )
// Chinese(Simplified)    - IsDBCSLeadByteEx( 936, ... ), WideCharToMultiByte( 936, ... )
// Chinese(Traditional)   - IsDBCSLeadByteEx( 950, ... ), WideCharToMultiByte( 950, ... )
// Portuguese             - IsDBCSLeadByteEx( 860, ... ), WideCharToMultiByte( 860, ... )
// English(United States) - IsDBCSLeadByteEx( 437, ... ), WideCharToMultiByte( 437, ... )
// --------------------------------------------------------------------------------------
#ifdef _ENGLISH             // 영어
#   pragma message( "Macro _ENGLISH is defined!" )
#   define GC_CODE_PAGE_IDENTIFIER  (437)
#endif // _ENGLISH

//------------------------------------------------------------------------------------------------------------------------
// 레이턴시(한번에 보내는 버퍼량)
#define LATENCY						(8)
#define SHOWLATENCY                 (2)
#define BEGIN_NPC					(80)

#define MAX_OBJECT                  (30)
#define MAX_MONSTER                 (80)
#define SUMMON_MONSTER_INDEX        (15)
#define DOTA_MONSTER_INDEX          (30)
#define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
#define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
#define END_DOTA_MONSTER            (MAX_MONSTER)

#define MAX_LEVEL                   (85)    ///< 그랜드체이스 캐릭터의 최고 레벨
#define NUM_TEMP_PLAYERINFO         (2)     ///< 임시 플레이어 정보 개수
#define MAX_PET_NUM                 (50)

#define LIMIT_INVENTORY_SIZE		(1500) // 아이템 제한, 이 수치 넘으면 미션 보상도 못받는다.

#define DEFAULT_INVENTORY_NUM       (90)   ///< 인벤토리 기본 최대 개수 (숫자)
#define MAX_INVENTORY_SIZE			(450)	 ///< 인벤토리 최대 사이즈

#define EXTEND_INVENTORY_SIZE_60    (60)    ///< +60 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_30    (30)    ///< +30 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_10    (10)    ///< +30 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_5     (5)     ///< +5 인벤토리 확장 아이템 사용 시 늘어나는 크기

#define MAX_EQUIP_NUM               (36)    ///< 최대 장착 개수
#define MAX_NUM_MON_SLOT            (5)     ///< 최대 몬스터 카드 슬롯 개수

#define NUM_TAKE_ITEM               (3)     ///< 한 플레이어가 가질 수 있는 게임 아이템 최대 개수
#define MAX_NUM_ITEM_IN_MAP         (20)    ///< 한 화면에 나올 수 있는 게임 아이템 최대 개수

#define DEFAULT_BAN_COUNT           (3)

#define MAX_GP_VALUE             (10000000) ///< GP 최대치

#define DBOX_SIZE_X 420          ///< 구매 웹 브라우져 크기 X
#define DBOX_SIZE_Y 420          ///< 구매 웹 브라우져 크기 Y

#define GEM_PER_CASH_RATE       (80)     ///< 필리핀 금액 당 GEM 비율

#define GC_NICKNAME_SIZE        (24)
#define GC_ACCOUNT_NICKNAME_SIZE        (8)
#define MAX_LOGIN_ID_SIZE       (20)    // DB에 저장되는 로그인 아이디의 최대 길이 : 20
#define MAX_LOGIN_PASSWORD_SIZE (20)

#define MAX_LOGIN_ID_BUF_SIZE       (boost::mpl::int_<MAX_LOGIN_ID_SIZE>::next::value)
#define MAX_NICKNAME_SIZE           (24)    // DB에 저장되는 닉네임의 최대 길이 : 24
#define MAX_NICKNAME_BUF_SIZE       (boost::mpl::int_<MAX_NICKNAME_SIZE>::next::value)
#define MAX_GUILDNAME_SIZE          (12)    // DB에 저장되는 길드명의 최대 길이 : 12
#define MAX_GUILDNAME_BUF_SIZE      (boost::mpl::int_<MAX_GUILDNAME_SIZE>::next::value)
#define MAX_CHARNAME_SIZE           (20)    // DB에 저장되는 캐릭터 이름의 최대 길이 : 20 (임시-확장고려)
#define MAX_CHARNAME_BUF_SIZE       (boost::mpl::int_<MAX_CHARNAME_SIZE>::next::value)

#if defined ( __PATH__ )
#	if defined( NATION_BRAZIL ) || defined( NATION_USA ) || defined( NATION_IDN )
//#    define    LOGIN_FIRST
#		define    MD5_PASSWORD_LOGIN
#	endif
#endif

//------------------------------------------------------------------------------------------------------------------------
//신고 관련 디파인
#define REPORT_MAIL_FROM                   "gcreport@ntreevusa.com"
#define REPORT_MAIL_TO                     "gcreport@ntreevusa.com"
#define REPORT_MAIL_SMTP                   "ntreevusa.com"

const __int64 nMailSHAKey = 189745675553;
const int nMailPassWordLen = 32;
const unsigned char szMailPassword[nMailPassWordLen+1] = 
{ 
    0xe4, 0xde, 0x4e, 0xb5, 0x01, 0x67, 0xb1, 0xc6, 
    0xd4, 0xd1, 0x6c, 0x18, 0xf7, 0x86, 0x03, 0xbd, 
    0xd8, 0xd2, 0xa2, 0xe7, 0xf4, 0xd5, 0x42, 0x11, 
    0x1f, 0xef, 0xd0, 0x0d, 0x3f, 0x0d, 0x49, 0xcc
}; //""

#define OLD_GRADE_SYSTEM		// 대전 강화가 들어가기 전 예전 계급 시스템
#define ENABLE_ROOM_NAME_COLOR	// 방 제목 색깔 변경 아이템
#define ENABLE_CHANGE_SKILL		// 신필살기 사용 여부
#define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#define NEWBIE_GUIDE				// 신규유저 가이드( 기사단장 등장 )
#define RANKING_SYSTEM			// 랭킹 게시판 기능 ( 스크립트와 함께 컨트롤 해 줘야 합니다. SquareObject.lua)
#define USING_SEASON2_INVITE		// 시즌2 광장에서 초대하기 기능
#define HALF_ATTACK_POINT		// 공격력 2배 하지 않음
#define WEB_CASH					// 웹캐쉬
#define GAME_CASH
#define STATIC_WORDWRAP			// 영문 워드랩 해주는거
#define TAIWAN_USER_AGREEMENT
#define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
#define ENABLE_ROOM_NAME_COLOR	// 방 제목 색깔 변경 아이템
#define PET_SYSTEM				// 펫시스템 사용
#define USE_JOYSTICK
//#   define ENABLE_MINIGAME
#define BUG_TRAP					// 크래쉬리포트방법
#define FORCE_KICK_SLEEP_USER    // 5분간 방에서 액션없는 유저 조내기
#define SQUARE_DASH				// 광장대쉬
#define USING_GC_CLUB
#define MESSENGER_S2
#define ENABLE_CHANGE_SKILL		// 신필살기 사용 여부
#define GP_CAPSULE				// GP 캡슐
#define MD5_PASSWORD_LOGIN		//
#define COUPON_NPC				// 쿠폰 NPC
#define PET_COSTUME				// 펫 코스튬
#define USING_HELL_MODE			// 헬모드
#define NEW_COLLECTION			// 칭호
#define ENABLE_TEAM_CHAT			// 팀 채팅기능 사용
#define ENABLE_DUGEON_HACK_CHECKING
#define LOAD_SLANG_FROM_KOM		// script.kom파일에서 total.stg파일을 읽는다.
#define EVENT_SANTA				// 산타이벤트
#define SHOW_EXTRA_CHAR_ICON
#define SAME_IMAGE_INDEX // 같은 이미지나 모델파일 및 텍스쳐 사용
#define COUPON_NPC				// 이벤트 쿠폰 npc
#define USE_LOGOUT
#define EMOTICON_SYSTEM
#define COUPLE_SYSTEM			//	커플
#define DICE_PLAY
#define DRAG_ITEM_SORT
#define ENABLE_CONTINUE_MISSION  // 연계미션
#define SELL_SCROLL_IN_GACHA_UI
#define NEW_YEAR_DONATION
#define USE_SEASON3_QUEST_RESULT //시즌3 던전결과창 사용
#define USE_LUA_SCRIPT_IN_UI
#define USE_LUA_API
#define MISSION_GUIDE
#define NEW_BOSS_HP_BAR
#define GUILD_SYSTEM
#define LOAD_GAMEMODE_FROM_PACKET
#define SEASON3_COMBO_SYSTEM
#define CHECK_DUPLICATED_FILE //중복된 파일 검출시 클라이언트 종료.
//#define LOAD_NECESSITY_TEXTURE   // 정보가 충분히 수집된 후 패치되어야 합니다.
#define ENABLE_MONSTER_CARD_BTN
#define RECOMLEVEL_TO_LUA
//디오 들어가고 들어가는 것들~!
#define TUTORIAL_NEW             // 연습모드 UI
#define STATUS_WINDOW            // 기본/상세 정보 창 UI
#define STATUS_PANEL             // 능력치 정보 창 UI
#define NEW_CHARACTER_SELECT_BOX
#define CHECK_ROOM_READY
#define MISSION_COMPLETE_SOUND   // 미션 완료시 효과음
#define BONUS_BOX_SELECT_KEY     // 던전 클리어 보상 보물상자 키보드 숫자키로 선택 
#define MONSTER_CARD_COLOR       // 던전에서 드롭 몬서트카드를 획득할때 출력 메시지 색깔 다르게 
#define ITEM_BREAKUP_INFO_BOX   // 아이템 해체 정보박스
#define SKILL_TREE               // 스킬트리
#define LOADING_IMAGE_FROM_SERVER_SCRIPT
#define LOADING_CHAR_MOTION
#define INVENTORY_NEW
#define MENU_CREATEANDBREAKUP    // 아이템 제작/해체 메뉴
#define USE_LOADING_STATE_BAR
#define USE_PING_STATE_UI
#define CHECK_SEND_PING
#define USE_STAGE_LOAD_COMPLETE_SIGN
//#define RETRY_LOGIN_ON_CLOSE_CONNECTION
#define USE_HERO_DUNGEON
#define LIRE2_RENEW
#define ARME2_RENEW
//#define USE_GAMBLE_SYSTEM
//#define USE_EVENT_BANNER        //이벤트 배너
#define WEAPONCHANGE_LV             (45)//무기체인지
#define WEAPONCHANGE
#define USE_EVENT_MINIGAME_BANNER

#define PROMOTION_STATUS_BONUS 
#define USE_SELL_GACHA_BTN          //신가챠 버튼 메뉴UI출력
#define MATCH_BALANCE

#define DLL_INJECTION_CHECK         //dll 체크 코드 (서버 initclientcontents에 DLLBlackList 목록을 가지고 있음)
#define USE_COORDI_SHOP
#define USE_PACKAGE_PREVIEW
#define GUILD_RENEW_S4
#define GUILD_STORE_S4          //길드상점


//#define USE_EVENTDUN_CARNIVAL       // 이벤트 던전( 카니발 )
//#define GWC_EVENT_2011              // GWC 이벤트
//#define USE_JUST_IN_TIME_EVENT      // JUST IN TIME 이벤트
#define USE_CHRISTMAS_GIFT_EVENT      // 크리스마스 선물 이벤트
#define USE_AGIT_SYSTEM               // 아지트 
#define USE_SOCIAL_MOTION
#define PARTY_SYSTEM                  //파티시스템
#define MATCH_BALANCE_2011_8          // 2011년 8월 25일 국내에 업데이트된 캐릭터 벨런스
#define MATCH_BALANCE                 // 대전 벨런스 적용
//#define USE_VIP_MENU_BTN
//#define COLLECT_SKILL_HOTKEY_USED_INFO// MP 스킬 단축키로 사용
#define USE_NEW_ENCHANT_SYSTEM        // 신강화 시스템
#define USE_EVENT_BANNER              // 이벤트 배너
//#define USE_GOLD_SPEAR                // 골든스피어
#define USE_LEVEL_LIMIT_GACHA           // 수정가차 사용
//#define USE_MAGIC_BOX                   // 코나우트 마법상자
#define USE_POST_LETTER_SYSTEM          // 우편함 사용 
#define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM //트레져박스 아이템이 장착 레벨을 제한 시키겠다.

#define USE_NEW_CHARACTER_SELECT              //바뀐 캐릭터 선택창 사용할 것인가 다음주에 풀자
#define TOOLTIP_FOR_SUPPORT_NEW_CHARACTER     //바뀐 캐릭터 선택창 툴팁표시
//#define WELCOMEBACK_FRIEND
//#define WELCOMEBACK_FRIEND_NEW // 반갑다 친구야 추천인 4인 버전 -  WELCOMEBACK_FRIEND / WELCOMEBACK_FRIEND_NEW 둘다 걸어야 사용 가능 
#define LOAD_ENCHANT_EFFECT_FROM_SCRIPT         // 17강 이펙트

#define USE_WEATHER_SYSTEM
#define USE_FACE_LIFT //얼굴표정 교체 기능
//#define LOCK_ASIN_HIDDEN_SKILL
//#define USE_OLYMPIC_EVENT
#define NEW_CHARACTER_SELECT_BOX
#define SKILL_TREE_OPEN_BY_SERVERSCRIPT
#define USE_HOST_MIGRATION
#define USE_P2P_OR_TCP_RELAY
#define NO_MSG_BOX_SKILL_LEARN_AND_REMOVE
#define USE_GP_ITEM_ATTRIBUTE_SELECT
#define RIN_PROMOTION
#define CHAR_ABILITY_STATE
#define DUNGEONPLAYSTATE
#define HERO_DUNGEON_REWARD
#define USE_HERO_ITEM_UPGRADE

#define DEVICE_MULTITHREADED 
#define USE_VIP_POINT

#define WORLD_MAP_MISSION_ICON //월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기

//// 이터널
#define USE_NEWBIE_GUIDE //새로운 튜토리얼 가이드 디파인
#define USE_GUIDE_SYSTEM // 가이드 시스템

// 속성 재설정 이벤트 
#define USE_ATTRIBUTE_SELECT_EVENT          // 속성 재설정 이벤트 
#define ATTRIBUTE_SELECT_MAX_COUNT ( 3 )    // 속성 재설정 이벤트 기간이 아닐때 속성 변경 가능한 최대 개수 

#define USE_MATCH_STATUS_RATE // 대전 보정스텟 사용
#define MATCH_BALLENCE_121217 //자동 매칭으로 인한 밸런스 개편
//#define USE_18TH_CHAR           // 18차 캐릭터 사용하는지??? ui들에서 처리되어야 할 것들 있어서 디파인 추가함.
#define USE_BIND_PET            // 캐릭터 귀속 펫 사용 







// 대전방에서 적용되는 f력치 비율 
#define COUNTER_DEF_PERCENT_MATCH   ( 0.5f )    // 대전에서는 착용 장비에 있는 카운터 저항 값에서 50%
#define COUNTER_DEF_MAX_VALUE_MATCH ( 0.5f )    // 대전에서 최대 적용되는 카운터 저항은 50%
#define MP_RECOVERY_PERCENT_MATCH   ( 0.25f )   // 대전에서는 착용 장비에 있는 MP회복력 값에서 25%
#define MP_RECOVERY_MAX_VALUE_MATCH ( 0.25f )   // 대전에서 최대 적용되는 MP회복력은 25%



#if defined (__PATH__)
#define CHECK_LOCK_FILE                    //File Lock Check(던전핵 때문에 만듬)
#endif


//	원어데이 일단 사내 테스트 용으로만 넣음
#if !defined(__PATH__)
//#	define ONE_A_DAY	//	원어데이
#endif

///시즌 4 이후 들어간 디파인들
//#define REGION_MESSAGEBOX        // 리젼 메시지 박스
#define USE_CRITICAL_CHEAT       // 치명타 100% 치트키 사용
#define CHAR_EXPINFO_PERCENT     // 캐릭터 정보 중 남은 Exp량 Percent로 변경
#define MISSION_COMPLETE_ITEMLIST_BOX
#define LIRE2_RENEW				//	리르 개편
#define ARME2_RENEW				//	아르메 개편
#define GET_NEW_CHARACTER_INFO_DLG // 캐릭터 획득 및 전직 알림 표시  

#define ATTRIBUTE_SELECT_NEW    //랜덤속성

#if !defined( __PATH__ )
#define USE_QA_LOG_SYSTEM          // QA용 로그 
#endif

// 버튼잠금프레임
#define BUTTON_LOCK_FRAME 25

// 웹 캐시 구매 페이지 주소
#if defined( __PATH__ )
#define BUY_WEB_PAGE( stm )
#define CHARGE_WEB_PAGE( stm )	(stm<<"http://account.levelupgames.ph/gcingame/topup/Login.aspx?gc=16")
#define GET_CASH_WEB_PAGE( stm )
#else
#define BUY_WEB_PAGE( stm )
#define CHARGE_WEB_PAGE( stm )	(stm<<"http://account.levelupgames.ph/gcingame/topup/Login.aspx?gc=16")
#define GET_CASH_WEB_PAGE( stm )
#endif

#define SELL_DIV_VALUE 10
#define SELL_GP_DIV_VALUE 10

#define GCDeviceFont GCDeviceFontBase

