#pragma once

#if defined( _TRUNK )
#define NATION_KOREA
#endif

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
#   define NATION_CODE L"TW"

//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#   define _CHINESE_TRADITIONAL // 중국어(번체)
#   pragma message( "This is TAIWAN Open Beta version!" )
#   define NATION_NAME          "Taiwan"

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

#ifdef GC_CODE_PAGE_IDENTIFIER
#   undef GC_CODE_PAGE_IDENTIFIER
#endif // GC_CODE_PAGE_IDENTIFIER

#ifdef _CHINESE_TRADITIONAL // 중국어(번체)
#   pragma message( "Macro _CHINESE_TRADITIONAL is defined!" )
#   define GC_CODE_PAGE_IDENTIFIER  (950)
#endif // _CHINESE_TRADITIONAL


//------------------------------------------------------------------------------------------------------------------------
// 레이턴시(한번에 보내는 버퍼량)
#    define LATENCY                 (8)
#define SHOWLATENCY                 (2)

#define MAX_MONSTER                 (80)
#define BEGIN_NPC					(80)
// #define SUMMON_MONSTER_INDEX        (15)
// #define DOTA_MONSTER_INDEX          (30)
// #define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
// #define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
// #define END_DOTA_MONSTER            (MAX_MONSTER)

#define MAX_LEVEL                   (85)    ///< 그랜드체이스 캐릭터의 최고 레벨
#define NUM_TEMP_PLAYERINFO         (2)     ///< 임시 플레이어 정보 개수
#define MAX_PET_NUM                 (50)

#define LIMIT_INVENTORY_SIZE		  (1500) // 아이템 제한, 이 수치 넘으면 미션 보상도 못받는다.

#define DEFAULT_INVENTORY_NUM       (90)   ///< 인벤토리 기본 최대 개수 (숫자)
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

#   define MAX_GP_VALUE             (10000000) ///< GP 최대치

#   define DBOX_SIZE_X 420          ///< 구매 웹 브라우져 크기 X
#   define DBOX_SIZE_Y 420          ///< 구매 웹 브라우져 크기 Y

#define GIFT_DBOX_SIZE_X 480          ///< 선물하기 웹 브라우져 크기 X
#define GIFT_DBOX_SIZE_Y 705          ///< 선물하기 웹 브라우져 크기 Y

#define GEM_PER_CASH_RATE       (10)    ///< 대만 금액 당 GEM 비율

#define GUILD_MARK_COMBINE_NUM      (3)     // 길드 마크 조합수 : 3 (방패색상+방패종류+문양종류)
#define GUILD_MARK_COLOR_NUM        (11)    // 길드 마크 색상 종류 가지 수
#define GUILD_MARK_SHIELD_NUM       (10)    // 길드 마크 방패 종류 가지 수
#define GUILD_MARK_CREST_NUM        (10)    // 길드 마크 문양 종류 가지 수

//길드개편
#define NEW_GUILD_NOTICE_LIMIT		(70)	// 길드 공지사항 제한 글 수
#define	NEW_GUILD_COMMENT_LIMIT		(70)	// 길드 소개 제한 글 수
#define NEW_GUILD_MY_COMMENT_LIMIT	(50)	// 길드 자기소개 제한 글 수

#	define GC_NICKNAME_SIZE        (12)
#define GC_ACCOUNT_NICKNAME_SIZE        (8)
#	define MAX_LOGIN_ID_SIZE       (20)    // DB에 저장되는 로그인 아이디의 최대 길이 : 20
#	define MAX_LOGIN_PASSWORD_SIZE (20)

#define MAX_LOGIN_ID_BUF_SIZE       (boost::mpl::int_<MAX_LOGIN_ID_SIZE>::next::value)
#define MAX_NICKNAME_SIZE           (24)    // DB에 저장되는 닉네임의 최대 길이 : 24
#define MAX_NICKNAME_BUF_SIZE       (boost::mpl::int_<MAX_NICKNAME_SIZE>::next::value)
#define MAX_GUILDNAME_SIZE          (12)    // DB에 저장되는 길드명의 최대 길이 : 12
#define MAX_GUILDNAME_BUF_SIZE      (boost::mpl::int_<MAX_GUILDNAME_SIZE>::next::value)
#define MAX_CHARNAME_SIZE           (20)    // DB에 저장되는 캐릭터 이름의 최대 길이 : 20 (임시-확장고려)
#define MAX_CHARNAME_BUF_SIZE       (boost::mpl::int_<MAX_CHARNAME_SIZE>::next::value)

#ifdef NATION_CHINA                        //중국같은 경우 채팅 수를 더 제한한다.
#	define MAX_CHAT_STR_SIZE       (128)   // P2P 채팅에 사용되는 스트링의 최대 길이
#else
#	define MAX_CHAT_STR_SIZE       (256)
#endif

#define SELL_DIV_VALUE 5
#define SELL_GP_DIV_VALUE 10

//  전 국가가 2.0에서 2.5로 상수값을 쓰기로 함.
#define ABILITY_CORRECT_PLAYER    2.5f

#define MAX_CHAT_STR_BUF_SIZE       (boost::mpl::int_<MAX_CHAT_STR_SIZE>::next::value)
//}} ---------------------------------------------------------------------------

//{{ ---------------------------------------------------------------------------
#define EMPTY_DEFAULT_CONSTRUCTOR(c)    c##( void ) {}
#define EMPTY_DEFAULT_DESTRUCTOR(c)     ~##c##( void ) {}
#define EMPTY_VIRTUAL_DESTRUCTOR(c)     virtual ~##c##( void ) {}

#define UNUSED_COPY_CONSTRUCTOR(c) \
    c##( const c##& ) \
        { ASSERT( false && "Do not use copy constructor!" ); }

#define UNUSED_ASSIGNMENT_OPERATOR(c) \
    const c##& operator=( const c##& ) \
        { ASSERT( false && "Do not use assignment operator!" ); return *this; }
//}} ---------------------------------------------------------------------------
#define LOGIN_FIRST

#if !defined( __PATH__) && defined( NATION_KOREA )
//#define LOGIN_FIRST
#endif

//#define _TAIWAN_GASH_TEST_
//#define _HONGKONG_GASH_TEST_


//=====================================================================
// 위의 LOGIN_SERVER_IP, LOGIN_SERVER_PORT 들 주석 처리 하고 아래 주석 풀면어서 디버그 모드로 컴파일 해서 테스트 하길
// 대만 사내에 띄운 서버랑 똑같은거 아래 주소에 띄우고
//#define LOGIN_SERVER_IP        "59.25.186.61"
//#define LOGIN_SERVER_PORT    (9510)
//#define STRSAFE_NO_DEPRECATE
//#define SERVICE
//#define NATION_TAIWAN
//#define _CHINESE_TRADITIONAL
//#define LOGIN_FIRST
//=====================================================================

//------------------------------------------------------------------------------------------------------------------------
//신고 관련 디파인
#define REPORT_MAIL_FROM                   "grandchase_bug@twindex.com.tw"
#define REPORT_MAIL_TO                     "grandchase_bug@twindex.com.tw"
#define REPORT_MAIL_SMTP                   "twindex.com.tw"
//------------------------------------------------------------------------------------------------------------------------

const __int64 nMailSHAKey = 189745675553;
const int nMailPassWordLen = 32;
const unsigned char szMailPassword[nMailPassWordLen+1] = 
{ 
    0xe4, 0xde, 0x4e, 0xb5, 0x01, 0x67, 0xb1, 0xc6, 
    0xd4, 0xd1, 0x6c, 0x18, 0xf7, 0x86, 0x03, 0xbd, 
    0xd8, 0xd2, 0xa2, 0xe7, 0xf4, 0xd5, 0x42, 0x11, 
    0x1f, 0xef, 0xd0, 0x0d, 0x3f, 0x0d, 0x49, 0xcc
}; //""


#define GC_FPS_LIMIT                (55)
#define GC_ELAPSED_TIME             (1.0f / GC_FPS_LIMIT)
#ifdef DIRECTINPUT_VERSION
#undef DIRECTINPUT_VERSION
#endif
#define DIRECTINPUT_VERSION 0x0800

#define TIME_OUT_VALUE              (INT_MAX - SHRT_MAX)


#define MAX_OBJECT                  (30)

#define NUM_JOB_LEVEL               (4)     ///< 최대 전직레벨

#define CASH_BUY_LIMIT_AGE          (7)     ///< 캐쉬 아이템 구입 제한 나이
#define CHAT_VIEW_TIME              (10)    ///< 채팅한거 보여주는 시간?
#define CHAR_SCALE_RATIO            (0.35f)
#define CHECK_NUM_AT_ONCE           (1)

// 반짝하는 프레임 길이
#define FLASH_EFFECT_FRAME          (4)     ///< 맞았을 때 반짝하는 프레임
#define CARTOON_FLASH_EFFECT_FRAME  (8)     ///< 카툰적용시에는 반짝하는게 너무짧아서 안보여서 좀더 길게..

// 무기 체인지 레벨제한
#define WEAPONCHANGE_LV             (45)

#if defined( _TRUNK )
#	define GP_CAPSULE				// GP 캡슐
#	define WELCOMEBACK_FRIEND		// 반갑다친구야
#endif

#   define USE_LUA_API
#   define PROTECTEDTYPE_NEW
#   define USE_STAGE_LOAD_COMPLETE_SIGN //던전 스테이지 이동시 유저별 로딩 완료 표시 기능
#	define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#	define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
#	define TAIWAN_USER_AGREEMENT
#	define GAME_CASH
#	define USING_HELL_MODE			// 헬모드
#   define ENABLE_MINIGAME          // 미니게임 할래?
#   define SKILL_TREE               // 스킬트리
#	define USE_JOYSTICK
#	define RING_OF_FRIENDSHIP_UNLIMIT_REBIRTH
#   define EVENT_SANTA
#	define COUPLE_SYSTEM
#	define VIRTUAL_CASH
#	define BUG_TRAP					// 크래쉬리포트방법
#	define GUILD_SYSTEM
#	define PET_COSTUME
#	define EMOTICON_SYSTEM
#	define BUY_CASHITEM_WITH_VIRTUAL_CASH
//#	define BUY_CASHITEM_MIX_VIRTUAL_CASH
#   define PING_STATISTICS
#	define GP_CAPSULE				// GP 캡슐
#   define COOPORATION_EVENT
#   define CHECK_DUPLICATED_FILE //중복된 파일 검출시 클라이언트 종료.
#   define ENABLE_DUGEON_HACK_CHECKING
#   define USE_HERO_DUNGEON
//#   define DISABLE_ITEM_SOCKET
#   define USE_PING_STATE_UI
#   define ITEM_BREAKUP_INFO_BOX    // 아이템 해체 정보박스
#   define WORLD_MAP_MISSION_ICON   // 월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#   define ROOM_MISSION_ICON        // 던전 대기방에서 미션 아이콘 표시하기 
//#   define EXIT_DUNGEON_MESSAGE     // 던전 플에이 중 나갈 때 잃을 수 있는 것 명확히 표시
#   define MISSION_COMPLETE_TOOLTIP // 미션 완료 버튼에 툴팁 사용 여부 
#   define NEW_CHARACTER_SELECT_BOX
#   define NEW_LEVELUP_EFFECT
//#	define USE_GAMBLE_SYSTEM
#   define CHECK_SEND_PING          //핑표시 해주는것
#   define USE_EVENT_BANNER        //이벤트 배너
#   define LOADING_CHAR_MOTION      // 대전로딩 캐릭터 모션
//#   define WELCOMEBACK_FRIEND
#   define CHAR_EXPINFO_PERCENT
//#   define MISSION_COMPLETE_ITEMLIST_BOX
//#   define REGION_MESSAGEBOX  //패쳐 끝나고 나오는 이미지 
#define WEAPONCHANGE
#define ITEMBOX_ANI_TOGGLE       // 아이템박스 애니메이션 옵션토글
#define ATTRIBUTE_SELECT_NEW
//#define DIRECTX_VERSIONUP_2009_AUG
#define GUILD_RENEW_S4          //시즌 4 길드 개편 (길드전)
#define GUILD_STORE_S4          //길드상점
#define USE_QUIZ_BINGO

#define PET_LEVEL_IN_SHOP (30)

#define USE_NEW_CHARACTER_SELECT    //바뀐 캐릭터 선택창 사용할 것인가
#define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM    //트레져박스 아이템이 장착 레벨을 제한 시키겠다.
#define USE_PACKAGE_PREVIEW      //패키지 아이템 미리보기
#define USE_COORDI_SHOP
#define GWC_EVENT_2011 //GWC 이벤트
#define USE_CHRISTMAS_GIFT_EVENT // 크리스마스 선물 이벤트
#define USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE // 전직미션 구매 가이드

//#   define USE_VIP_MENU_BTN 
//#   define USE_VIP_NEW_USER_TAB //신규 유저 보상 탭 살릴건지?

//아지트 관련
#	define PARTY_SYSTEM						// 파티 시스템
#	define USE_AGIT_SYSTEM					// 아지트 코드
#   define USE_SOCIAL_MOTION                // 소셜 모션 사용 

//Just in Time 디파인 
#define USE_JUST_IN_TIME_EVENT
#define LOADING_IMAGE_FROM_SERVER_SCRIPT
#define MATCH_BALANCE_2011_8 //매치 밸런스 디파인

#   define USE_MAGIC_BOX //코우나트 마법상자
//#define  USE_GOLD_SPEAR

#define LOAD_ENCHANT_EFFECT_FROM_SCRIPT //강화 이펙트 정보 스크립트에서 읽기

#define DLL_INJECTION_CHECK         //dll 체크 코드 (서버 initclientcontents에 DLLBlackList 목록을 가지고 있음)


//#define USE_SURVEY_SYSTEM //설문조사 시스템
# define USE_LEVEL_LIMIT_GACHA // 레벨제한 가차사용 - 획득한 가차레벨의 동레벨 최종보상 받기
#define HERO_DUNGEON_REWARD

#define USE_WEATHER_SYSTEM //날씨 시스템 사용 (아신 들어가면서 들어간것)
//#define LOCK_ASIN_HIDDEN_SKILL

#define USE_NEW_RECOM
#define USE_OLYMPIC_EVENT

#define SKILL_TREE_OPEN_BY_SERVERSCRIPT
#define USE_TUTORIAL_LOADING    // 연습모드 로딩 사용

#define SERVER_ONLY_NON_COUNT_ITEM  //NonCount Item을 서버에서만 받는다.
#define NO_MSG_BOX_SKILL_LEARN_AND_REMOVE

#define USE_FACE_LIFT //얼굴표정 교체 기능
#define USE_GP_ITEM_ATTRIBUTE_SELECT // GP 아이템 속성변환 사용
#define RIN_PROMOTION //린 전직 들어 가면 사용 (전직 후 캐릭터가 MP -> 하이브리드로 바꼈다)
#define USE_QA_LOG_SYSTEM //QA 로그 시스템 사용
#define USE_HERO_ITEM_UPGRADE //영웅던전 개편 디파인 (영웅상점 업그레이드)

#define DUNGEONPLAYSTATE         // 던전 플레이 통계
#define CHAR_ABILITY_STATE         // 캐릭터 능력치 통계

#define USE_DAMAGE_POSION_FREE      //DAMAGE_POTION 타입 포션 장착제한 풀기
#define USE_NEW_ENCHANT_SYSTEM


#define MATCH_BALANCE
#define USE_MATCH_STATUS_RATE       // 대전 능력치 비율 조정 사용 
#define MATCH_BALLENCE_121217 //자동 매칭으로 인한 밸런스 개편

// 대전방에서 적용되는 f력치 비율 
#define COUNTER_DEF_PERCENT_MATCH   ( 0.5f )    // 대전에서는 착용 장비에 있는 카운터 저항 값에서 50%
#define COUNTER_DEF_MAX_VALUE_MATCH ( 0.5f )    // 대전에서 최대 적용되는 카운터 저항은 50%
#define MP_RECOVERY_PERCENT_MATCH   ( 0.25f )   // 대전에서는 착용 장비에 있는 MP회복력 값에서 25%
#define MP_RECOVERY_MAX_VALUE_MATCH ( 0.25f )   // 대전에서 최대 적용되는 MP회복력은 25%

#define USE_GUIDE_SYSTEM
//새로운 튜토리얼 가이드 디파인
#define USE_NEWBIE_GUIDE
#define USE_BIND_PET            // 캐릭터 귀속 펫 사용 


// 속성 재설정 이벤트 
#define USE_ATTRIBUTE_SELECT_EVENT
#define ATTRIBUTE_SELECT_MAX_COUNT ( 3 )    // 속성 재설정 이벤트 기간이 아닐때 속성 변경 가능한 최대 개수 
#define USE_9999_POTION

#define FULLMOON_EVENT

// 버튼잠금프레임
#define BUTTON_LOCK_FRAME 25

#if defined( _OPEN_TEST_ ) || defined(NATION_THAILAND)
#define DISABLE_BADGUY_REPORT
#endif

#			define BUY_WEB_PAGE( stm )
#			define CHARGE_WEB_PAGE( stm )
#			define GET_CASH_WEB_PAGE( stm )

#define _NSERVER_LOG    //서버로그 보기싫을때 ㅋㅋ

//#define USING_VERTEX_SHADER

#define INDIGO_MAX_PLAYER	2

//#include <sstream>
#define OKBOXA( msg, title ) \
{\
    std::stringstream strstm;\
    strstm<<msg;\
    MessageBoxA( NULL, strstm.str().c_str() , title , MB_OK );\
}

#if defined( _DEBUG )
#define ASSERT_MBOX( msg ) OKBOXA( msg, __FUNCTION__ )\
    ASSERT( !"MBox Assert" );
#else
#define ASSERT_MBOX( msg ) 
#endif

#define FRAME_TIMER_RESET   g_MyD3D->m_fTime = 0.0f;\
    DXUtil_Timer( TIMER_RESET );\
    g_MyD3D->m_fElapsedTime = DXUtil_Timer( TIMER_GETAPPTIME )


//#define RESOURCE_GRAPH

//GP    되팔기 비율은 앞으로 20%로 할것임
#if defined(NATION_PHILIPPINE)
#define SELL_DIV_VALUE 10
#elif defined(NATION_USA)
#define SELL_DIV_VALUE 3
#else
#define SELL_DIV_VALUE 5
#endif

//------------------------------------------------------------------------------------------------------------------------
//XTrap 관련 디파인
#if defined(__PATH__)
    #define XTRAP_HTTP_URL_KEY "660970B42859DBF0C5246D984FCFE8626D0EF941AF039CD1E5EB7110CFC759193231D480581C2AAE4DC67C7032B001CE2F6E3D44643685EDC0A5D947353E73250F7D04245246E417A4610E569557650CE0023EE9C09D90894EEF32A8A18AFA7189C59279B4D6A9BC266FAF6CABF421" 
#else
    #define XTRAP_HTTP_URL_KEY "660970B42859DBF0C5246D9844CFE86285EDCCD8F8443B4DBBF4F4D2B46AA97FFB9385A26A31977EE542D088ACC8FA5BA52470FBD4AD5E9619E3ED7A0F016C440F7D04245246E455F425430C8B083954880CB5032B840A09A65E9A" 
#endif

#define XTRAP_CHECK_INTERVAL ( 20.0f )

#if !defined(__PATH__)
#define CHECK_ARRAY_INDEX
//#define PERFORMANCE_TEST 
#endif


#define ACHEIVE_SYSTEM //업적시스템을 쓰고 싶으면 이 부분을 주석처리 하세요.
#define GCDeviceFont GCDeviceFontBase

//대기방 관련 시간 Define
#define TIME_FOR_IDLE 60000 // 1분(60초) -- 잠수표시 되는데 걸리는 시간
#define TIME_FOR_PREESURE 15000 //15초 -- 다시 재촉할때까지 드는 시간
#define TIME_FOR_HURRYUPDLG 4500 //2.5초 -- 재촉하는 이미지가 떠 있는 시간
#define TIME_FOR_HURRYUPDLG_DISAPPEAR 1500 //1.5초 -- 재촉하는 이미지가 떠 있다 스르르 사라지는 시간
#define TIME_FOR_HURRYUPDLG_COMING 1500 //1.5초 -- 재촉하는 이미지가 떠 있다 스르르 사라지는 시간



//  파티클 멀티쓰레드 로드 할려면 반드시 켜야 합니다.
#define DEVICE_MULTITHREADED