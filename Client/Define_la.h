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
#	   define NATION_CODE L"LA"

//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#   define _SPANISH              // 스페인어
#   pragma message( "This is Latin America version!" )
#   define NATION_NAME          "LatinAmerica"

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
// Spanish(Latin)         - 850
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

#ifdef _SPANISH              // 한국어
#   pragma message( "Macro _SPANISH is defined!" )
#   define GC_CODE_PAGE_IDENTIFIER  (850)
#endif // _KOREAN

//------------------------------------------------------------------------------------------------------------------------
// 레이턴시(한번에 보내는 버퍼량)
#define LATENCY                     (5)
#define SHOWLATENCY                 (2)
#define MAX_LEVEL                   (80)    ///< 그랜드체이스 캐릭터의 최고 레벨
#define BEGIN_NPC					(80)
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

#define MAX_GP_VALUE             (3000000) ///< GP 최대치

#define DBOX_SIZE_X 450          ///< 구매 웹 브라우져 크기 X
#define DBOX_SIZE_Y 340          ///< 구매 웹 브라우져 크기 Y

#define GIFT_DBOX_SIZE_X 480          ///< 선물하기 웹 브라우져 크기 X
#define GIFT_DBOX_SIZE_Y 705          ///< 선물하기 웹 브라우져 크기 Y

#define GEM_PER_CASH_RATE       (200)       ///< 캐쉬 금액 당 GEM 비율

#define GUILD_MARK_COMBINE_NUM      (3)     // 길드 마크 조합수 : 3 (방패색상+방패종류+문양종류)
#define GUILD_MARK_COLOR_NUM        (11)    // 길드 마크 색상 종류 가지 수
#define GUILD_MARK_SHIELD_NUM       (10)    // 길드 마크 방패 종류 가지 수
#define GUILD_MARK_CREST_NUM        (10)    // 길드 마크 문양 종류 가지 수

//길드개편
#define NEW_GUILD_NOTICE_LIMIT		(70)	// 길드 공지사항 제한 글 수
#define	NEW_GUILD_COMMENT_LIMIT		(70)	// 길드 소개 제한 글 수
#define NEW_GUILD_MY_COMMENT_LIMIT	(50)	// 길드 자기소개 제한 글 수

#define MAX_MONSTER                 (30)
#define SUMMON_MONSTER_INDEX        (15)
#define DOTA_MONSTER_INDEX          (30)
#define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
#define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
#define END_DOTA_MONSTER            (MAX_MONSTER)

#define MAX_ITEM_RECEIVE            (15)    // 서버로 부터 받는 최대 아이템 개수(5의배수로 맞출것)

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

#define MAX_CHAT_STR_SIZE       (256)

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
#if (defined( NATION_TAIWAN ) && !defined( BEAN_FUN )) || defined( NATION_HONGKONG ) || defined(NATION_BRAZIL) || defined( NATION_USA )
#define LOGIN_FIRST
#endif

#if !defined( __PATH__)
//#define LOGIN_FIRST
#endif

#if defined(__PATH__)
#   define BDMPEG1SETUP_DOWNLOAD   (L"http://gcdl.axeso5.com/kog-codec/BDMPEG1SETUP.EXE")
#else
#   define BDMPEG1SETUP_DOWNLOAD   (L"http://208.39.112.61/GrandChase/BDMPEG1SETUP.EXE")
#endif


//------------------------------------------------------------------------------------------------------------------------
//신고 관련 디파인
#define REPORT_MAIL_FROM                   "gc112@kog.co.kr"
#define REPORT_MAIL_TO                     "gc112@kog.co.kr"
#define REPORT_MAIL_SMTP                   "mail.kog.co.kr"

const __int64 nMailSHAKey = 189745675553;
const int nMailPassWordLen = 32;
const unsigned char szMailPassword[nMailPassWordLen+1] = 
{ 
    0x10, 0x03, 0x07, 0xb3, 0x05, 0xac, 0xc1, 0xc7,
    0x5f, 0x00, 0x10, 0xbe, 0xeb, 0xbd, 0xfa, 0x27, 
    0xea, 0xf2, 0x12, 0xc3, 0x03, 0x9f, 0xcb, 0x9b, 
    0xf2, 0x54, 0xab, 0x1a, 0xbf, 0x71, 0x7b, 0x4b
}; //rmcp123QAZ

//------------------------------------------------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------------------------------------------------

// 아이템 아이디들 정의된거..

#if defined (_ALL_SMALL_FUNCTION_DEFINE_INTRODUCTION_)  //아주 작은 온오프 기능들의 Define
#	define CHAT_EMOTICON_EMPTY_ROLLOVER
#   define COUPON_TWO_INPUT
#	define SAVE_CHATCOLOR_INFO
#   define COOPORATION_EVENT
#   define LOADING_IMAGE_FROM_SERVER_SCRIPT
#	define LINK_HOMEPAGE_WHEN_ENDGAME
#endif

#if defined( _TRUNK )
#	define GP_CAPSULE				// GP 캡슐
#	define WELCOMEBACK_FRIEND		// 반갑다친구야
#endif

# if defined( _DEBUG )
# endif

#define GAME_CASH
#define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
#define WEB_CASH
#define USING_HELL_MODE
#define FORCE_KICK_SLEEP_USER
#define ENABLE_MINIGAME
#define ENABLE_DUGEON_HACK_CHECKING
#define EVENT_SANTA              // 산타이벤트
#define BUG_TRAP					// 크래쉬리포트방법
#define EMOTICON_SYSTEM
#define USE_LOGOUT
#define PING_STATISTICS			// 핑 통계
#define CHECK_SEND_PING
#define USE_PING_STATE_UI
#define ITEM_BREAKUP_INFO_BOX   // 아이템 해체 정보박스
#define USE_HOST_MIGRATION
#define WORLD_MAP_MISSION_ICON   // 월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#define ROOM_MISSION_ICON        // 던전 대기방에서 미션 아이콘 표시하기 
#define MISSION_COMPLETE_TOOLTIP // 미션 완료시 툴팁 사용 여부 
#define USE_GAMBLE_SYSTEM
#define MATCH_BALANCE			 // 대전 벨런스 적용
#define NEW_LEVELUP_EFFECT       //  새로 만든 레벨업 이펙트 사용
//#define MISSION_COMPLETE_ITEMLIST_BOX
#define LOADING_CHAR_MOTION      // 대전 로딩 캐릭터 모션 처리방식 변경
#define ITEMBOX_ANI_TOGGLE       // 아이템박스 애니메이션 옵션토글
#define ENABLE_PVP_SHOP          // 대회서버에서 상점탭버튼 활성화
#define LOADING_IMAGE_FROM_SERVER_SCRIPT
#define USE_STAGE_LOAD_COMPLETE_SIGN
#define MD5_PASSWORD_LOGIN
#define RETRY_LOGIN_ON_CLOSE_CONNECTION
#define CHECK_DUPLICATED_FILE
//#define NO_USE_WINDOW_MODE       // 윈도우모드 사용않겠다.
// #define VIRTUAL_CASH
// #define BUY_CASHITEM_WITH_VIRTUAL_CASH
#define USE_P2P_OR_TCP_RELAY
//#define USE_ONLY_TCP_RELAY
//#define USE_MIGRATION_BTN
#define ATTRIBUTE_SELECT_NEW
#define USE_PLAY_COUNTRY_GROUP
#define USE_HERO_DUNGEON
#define USE_PACKAGE_PREVIEW
#define USE_COORDI_SHOP //코디샵이다
#define USE_VIP_MENU_BTN
#define GUILD_SYSTEM
#define GUILD_RENEW_S4          //시즌 4 길드 개편 (길드전)
#define GUILD_STORE_S4          //길드상점
#define USE_QUIZ_BINGO
#define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM    //트레져박스 아이템이 장착 레벨을 제한 시키겠다.
#define USE_LEVEL_LIMIT_GACHA       // 레벨제한 가차사용 - 획득한 가차레벨의 동레벨 최종보상 받기
#define USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE // 전직미션 구매 가이드
#define DLL_INJECTION_CHECK         //dll 체크 코드 (서버 initclientcontents에 DLLBlackList 목록을 가지고 있음)
#define LOAD_ENCHANT_EFFECT_FROM_SCRIPT //강화 이펙트 정보 스크립트에서 읽기
#define MATCH_BALANCE_2011_8     // 2011년 8월 25일 국내에 업데이트된 캐릭터 벨런스
#define MATCH_BALANCE			 // 대전 벨런스 적용
#define USE_WEATHER_SYSTEM //날씨 시스템 사용 (아신 들어가면서 들어간것)
#define USE_HERO_ITEM_UPGRADE


#define USE_CHRISTMAS_GIFT_EVENT      // 크리스마스 선물 이벤트
#define USE_JUST_IN_TIME_EVENT      // JUST IN TIME 이벤트
#define COUPLE_SYSTEM               // 커플 시스템
#define USE_JOYSTICK                // 조이스틱 사용하도록 설정
#define USE_NEW_CHARACTER_SELECT    // 캐릭터 선택창
#define TOOLTIP_FOR_SUPPORT_NEW_CHARACTER
#define RIN_PROMOTION //린 전직 들어 가면 사용 (전직 후 캐릭터가 MP -> 하이브리드로 바꼈다)

//#define USE_EVENTDUN_CARNIVAL    // 이벤트 던전( 카니발 )

//#define PET_COSTUME
//#define GUILD_SYSTEM
#define SKILL_TREE                          // 스킬트리
#define SKILL_TREE_OPEN_BY_SERVERSCRIPT
#define USE_NEW_RECOM                   // 레콤 
#define WEAPONCHANGE
#define NEW_CHARACTER_SELECT_BOX
#define AVI_RECODER
#define REPLAY_BOX_NEW   // 새 리플레이 보기 사용   // 2010 10 06 이번에는 롤백
//#define USE_MERRY_HP_BAR
//#define WELCOMEBACK_FRIEND          //반갑다 친구야
//#define LOADING_IMAGE_FROM_SERVER_SCRIPT
//#define KOREA_NEW_FONT              // 한국 새 폰트
//#define EXIT_DUNGEON_MESSAGE     // 던전 플에이 중 나갈 때 잃을 수 있는 것 명확히 표시
//#define CHECK_DUPLICATED_FILE //중복된 파일 검출시 클라이언트 종료.
//#define COOPORATION_EVENT
//#define USING_HELL_MODE
#define DUNGEONPLAYSTATE         // 던전 플레이 통계
#define CHAR_ABILITY_STATE         // 캐릭터 능력치 통계
#define USE_TUTORIAL_LOADING
#define HERO_DUNGEON_REWARD
#define USE_SIMPLE_PET_BUY
#define USE_WEATHER_SYSTEM //날씨 시스템 사용 (아신 들어가면서 들어간것)
#define USE_FACE_LIFT //얼굴표정 교체 기능
//#define LOCK_ASIN_HIDDEN_SKILL

#define USE_GP_ITEM_ATTRIBUTE_SELECT // GP 아이템 속성변환 사용
#define USE_POST_LETTER_SYSTEM
#define PET_COSTUME                 // 펫 코스튬
#define USE_VIP_POINT
#define USE_MAGIC_BOX

#define PET_LEVEL_IN_SHOP (30)

#define NO_MSG_BOX_SKILL_LEARN_AND_REMOVE



// 버튼잠금프레임
#define BUTTON_LOCK_FRAME 20

#if defined( _OPEN_TEST_ ) || defined(NATION_THAILAND)
#define DISABLE_BADGUY_REPORT
#endif



// 웹 캐시 구매 페이지 주소
#if defined( WEB_CASH )
#		if defined( __PATH__ )	//한국 본섭
#			define CHARGE_WEB_PAGE( stm )		
#			define GET_CASH_WEB_PAGE( stm )		
#		elif defined( NETMARBLE_BUY_TEST )	//한국 구매 테섭
#			define CHARGE_WEB_PAGE( stm )		
#			define GET_CASH_WEB_PAGE( stm )		
#		else	//한국 사내
#			define BUY_WEB_PAGE( stm, arg1 )
#			define CHARGE_WEB_PAGE( stm )
#			define GET_CASH_WEB_PAGE( stm )
#		endif
#else
#			define BUY_WEB_PAGE( stm )
#			define CHARGE_WEB_PAGE( stm )
#			define GET_CASH_WEB_PAGE( stm )
#endif

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
#define SELL_DIV_VALUE 5
#define SELL_GP_DIV_VALUE 10

#if defined(__PATH__)
#define USE_HTTPPOST
#endif
#if defined(USE_HTTPPOST)
#pragma comment(lib,"libcurl.lib")
#endif


//------------------------------------------------------------------------------------------------------------------------
//XTrap 관련 디파인
//XTrap HTTP URL 키값
#if defined(__PATH__) //본섭
#define XTRAP_HTTP_URL_KEY "660970B4965BDB22C5246D9849CFEF62146A0F6CC151254C897F62835E22592D5E70F307717E954D55D1613B10E92216A9916422967E108D4F6A2F733022CBDA0F7D04245246E400A6710110DA48721EF23805E53A8D401AA55983DA"
#else                 //사내
// 사내 키 재발급 필요.
#define XTRAP_HTTP_URL_KEY "660970B4965BDB22C5246D9849CFEF62146A0F6CC151254C897F62835E22592D5E70F307717E954D55D1613B10E92216A9916422967E108D4F6A2F733022CBDA0F7D04245246E455F52D430D821E265C0BF881D2E633A426C25177D2FD1CBF5F41B2CC69C7" 
#endif

#define XTRAP_CHECK_INTERVAL ( 20.0f )


#if !defined(__PATH__)
#define CHECK_ARRAY_INDEX
#define USE_LUA_API		// 루아의 함수 콜을 Lua API를 이용해서 합니다. 안정화되면 본섭에서도 쓰면 됩니다. 2배 빨라짐...
//#define PERFORMANCE_TEST 
#endif

#define ACHEIVE_SYSTEM //업적시스템을 쓰고 싶으면 이 부분을 주석처리 하세요.

#if defined(_THAI_LANG)
#define	GCDeviceFont GCDeviceFontTH
#else
#define GCDeviceFont GCDeviceFontBase
#endif

//#define USE_GC_LOG_SYSTEM

//대기방 관련 시간 Define
#define TIME_FOR_IDLE 60000 // 1분(60초) -- 잠수표시 되는데 걸리는 시간
#define TIME_FOR_PREESURE 15000 //15초 -- 다시 재촉할때까지 드는 시간
#define TIME_FOR_HURRYUPDLG 4500 //2.5초 -- 재촉하는 이미지가 떠 있는 시간
#define TIME_FOR_HURRYUPDLG_DISAPPEAR 1500 //1.5초 -- 재촉하는 이미지가 떠 있다 스르르 사라지는 시간
#define TIME_FOR_HURRYUPDLG_COMING 1500 //1.5초 -- 재촉하는 이미지가 떠 있다 스르르 사라지는 시간


//  파티클 멀티쓰레드 로드 할려면 반드시 켜야 합니다.
#define DEVICE_MULTITHREADED
