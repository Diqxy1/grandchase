#pragma once


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



#	define NATION_CODE L"TH"


//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#   define _THAI_LANG			// 태국어
#   pragma message( "This is Thailand version!" )
#   define NATION_NAME          "Thailand"

// 국가별 언어 코드 페이지
#   define GC_CODE_PAGE_IDENTIFIER  (874)


//------------------------------------------------------------------------------------------------------------------------
// 레이턴시(한번에 보내는 버퍼량)
#   define LATENCY                 (8)
#	define SHOWLATENCY              (2)
#	define MAX_LEVEL (80)

#define NUM_TEMP_PLAYERINFO         (2)     ///< 임시 플레이어 정보 개수
#define MAX_PET_NUM                 (50)

#define LIMIT_INVENTORY_SIZE		(1500) // 아이템 제한, 이 수치 넘으면 미션 보상도 못받는다.
#define MAX_INVENTORY_SIZE          (450)   ///< 인벤토리 최대 사이즈

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

#define GEM_PER_CASH_RATE       (200)   ///< 캐쉬 금액 당 GEM 비율
#define DEFAULT_INVENTORY_NUM       (90)   ///< 인벤토리 기본 최대 개수 (숫자)

#define MAX_MONSTER                 (30)
#define BEGIN_NPC					(80)

#define SUMMON_MONSTER_INDEX        (15)
#define DOTA_MONSTER_INDEX          (30)
#define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
#define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
#define END_DOTA_MONSTER            (MAX_MONSTER)

#define MAX_ITEM_RECEIVE            (15)    // 서버로 부터 받는 최대 아이템 개수(5의배수로 맞출것)

#	define GC_NICKNAME_SIZE        (24)
#define GC_ACCOUNT_NICKNAME_SIZE        (8)
#	define MAX_LOGIN_ID_SIZE       (40)    // DB에 저장되는 로그인 아이디의 최대 길이 : 20
#	define MAX_LOGIN_PASSWORD_SIZE (20)

#define MAX_LOGIN_ID_BUF_SIZE       (boost::mpl::int_<MAX_LOGIN_ID_SIZE>::next::value)
#define MAX_NICKNAME_SIZE           (24)    // DB에 저장되는 닉네임의 최대 길이 : 24
#define MAX_NICKNAME_BUF_SIZE       (boost::mpl::int_<MAX_NICKNAME_SIZE>::next::value)
#define MAX_GUILDNAME_SIZE          (12)    // DB에 저장되는 길드명의 최대 길이 : 12
#define MAX_GUILDNAME_BUF_SIZE      (boost::mpl::int_<MAX_GUILDNAME_SIZE>::next::value)
#define MAX_CHARNAME_SIZE           (20)    // DB에 저장되는 캐릭터 이름의 최대 길이 : 20 (임시-확장고려)
#define MAX_CHARNAME_BUF_SIZE       (boost::mpl::int_<MAX_CHARNAME_SIZE>::next::value)

// 무기 체인지 레벨제한
#define WEAPONCHANGE_LV             (45)

#define LOGIN_FIRST
#define    MD5_PASSWORD_LOGIN

#define REPORT_MAIL_FROM                   "gc112_send@smtp.kog.co.kr"
#define REPORT_MAIL_TO                     "gc112@kog.co.kr"
#define REPORT_MAIL_SMTP                   "14.45.79.45"

const __int64 nMailSHAKey = 189745675553;
const int nMailPassWordLen = 32;
const unsigned char szMailPassword[nMailPassWordLen+1] = 
{ 
    0x10, 0x03, 0x07, 0xb3, 0x05, 0xac, 0xc1, 0xc7,
    0x5f, 0x00, 0x10, 0xbe, 0xeb, 0xbd, 0xfa, 0x27, 
    0xea, 0xf2, 0x12, 0xc3, 0x03, 0x9f, 0xcb, 0x9b, 
    0xf2, 0x54, 0xab, 0x1a, 0xbf, 0x71, 0x7b, 0x4b
}; //rmcp123QAZ


#	define PET_SYSTEM				// 펫시스템 사용
#	define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#	define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
#	define GAME_CASH
#	define OLD_GRADE_SYSTEM			// 대전 강화가 들어가기 전 예전 계급 시스템
#   define USING_HELL_MODE
//#   define WELCOMEBACK_FRIEND		// 반갑다 친구야.
#	define FORCE_KICK_SLEEP_USER
//#	define ENABLE_MINIGAME
#   define SKILL_TREE               // 스킬트리
#	define COUPLE_SYSTEM
#	define GUILD_SYSTEM				// 한국 아니면 켜도 버튼 Disable 되어 있어서 남겨둠..
#	define PET_COSTUME				// 펫 코스튬
#   define EVENT_SANTA              // 산타이벤트
#	define BUG_TRAP					// 크래쉬리포트방법
#	define EMOTICON_SYSTEM
#	define PING_STATISTICS			// 핑 통계
#	define MD5_PASSWORD_LOGIN		//
#   define COUPON_TWO_INPUT
#	define STATIC_THAI_WORDWRAP		// 태국어 워드랩 해주는거
#	define GP_CAPSULE				// GP 캡슐
#	define ENABLE_DUGEON_HACK_CHECKING
#   define SEASON3_COMBO_SYSTEM
#   define CHECK_DUPLICATED_FILE //중복된 파일 검출시 클라이언트 종료.
#   define DICE_PLAY
//여기서 부터 1주년 이후 갱신 내역
#   define USE_PING_STATE_UI
#   define CHECK_SEND_PING
#   define WORLD_MAP_MISSION_ICON   // 월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#   define ITEM_BREAKUP_INFO_BOX    // 아이템 해체 정보박스
#   define ROOM_MISSION_ICON        // 던전 대기방에서 미션 아이콘 표시하기 
//#   define EXIT_DUNGEON_MESSAGE     // 던전 플에이 중 나갈 때 잃을 수 있는 것 명확히 표시
#   define MISSION_COMPLETE_TOOLTIP // 미션 완료 버튼에 툴팁 사용 여부 
#   define MATCH_BALANCE            // 대전 벨런싱
//#   define USE_HOST_MIGRATION
#	define USE_GAMBLE_SYSTEM
#   define USE_EVENT_BANNER
#	define USE_STAGE_LOAD_COMPLETE_SIGN
#	define SKILL_TREE_OPEN_BY_SERVERSCRIPT
//시즌 4추가 디파인들
#define NEW_LEVELUP_EFFECT       //  새로 만든 레벨업 이펙트 사용
#define SEASON4
#define USE_SELL_GACHA_BTN          //신가챠 버튼 메뉴UI출력
#define WEAPONCHANGE
#define TOOLTIP_FOR_SUPPORT_NEW_CHARACTER                       //신규캐릭터 지원 툴팁 사용여부
#define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM    //트레져박스 아이템이 장착 레벨을 제한 시키겠다.
#define USE_PACKAGE_PREVIEW      //패키지 아이템 미리보기
#define USE_EVENT_MINIGAME_BANNER
#define USE_EVENTDUN_CARNIVAL    // 이벤트 던전( 카니발 )
#define GWC_EVENT_2011 //GWC 이벤트
#define USE_JUST_IN_TIME_EVENT      // JUST IN TIME 이벤트
#define USE_COORDI_SHOP             // 코디샵
#define USE_NEW_ENCHANT_SYSTEM      // 신강화
//#define USE_VIP_MENU_BTN            //VIP보상 시스템 버튼 메뉴 UI에 출력
//#define USE_CHRISTMAS_GIFT_EVENT      // 크리스마스 선물 이벤트

#define DLL_INJECTION_CHECK         //dll 체크 코드 (서버 initclientcontents에 DLLBlackList 목록을 가지고 있음)

#define GUILD_RENEW_S4          //시즌 4 길드 개편 (길드전)
#define GUILD_STORE_S4          //길드상점
#define USE_MAGIC_BOX			//코우나트 마법상자
#define USE_LEVEL_LIMIT_GACHA
#define USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE // 전직미션 구매 가이드
#define CHAR_ABILITY_STATE         // 캐릭터 능력치 통계

#if defined (__PATH__)
#define CHECK_LOCK_FILE                    //File Lock Check(던전핵 때문에 만듬)
#endif

#if !defined(__PATH__)
#define USE_LOADINGTIME_RECORD // 로딩시간기록
#endif

#define ENABLE_RESOLUTION

//--------------------여기서는 조낸 작은 온오프 기능들
#	define CHAT_EMOTICON_EMPTY_ROLLOVER
#	define SAVE_CHATCOLOR_INFO
#   define LOADING_IMAGE_FROM_SERVER_SCRIPT
#	define LINK_HOMEPAGE_WHEN_ENDGAME
#   define USE_HERO_DUNGEON

// 버튼잠금프레임
#define BUTTON_LOCK_FRAME 20

#define DISABLE_BADGUY_REPORT

//GP 되팔기 비율
#define SELL_DIV_VALUE 5
#define SELL_GP_DIV_VALUE 10

//웹캐쉬 쓰진 않지만 에러나지마라고 일단 넣음
#			define BUY_WEB_PAGE( stm )
#			define CHARGE_WEB_PAGE( stm )
#			define GET_CASH_WEB_PAGE( stm )

//태국어 폰트
#define	GCDeviceFont GCDeviceFontTH

#define PROTECTEDTYPE_NEW
#define USE_LUA_API		// 루아의 함수 콜을 Lua API를 이용해서 합니다. 안정화되면 본섭에서도 쓰면 됩니다. 2배 빨라짐...

#define DEVICE_MULTITHREADED // 파티클 실시간 로드 기능

#define USE_QA_LOG_SYSTEM //QA 로그 시스템 사용