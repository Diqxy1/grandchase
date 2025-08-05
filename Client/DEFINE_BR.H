#pragma once

//#include <boost/mpl/int.hpp>
//#include <boost/mpl/plus.hpp>
//#include <boost/mpl/minus.hpp>
//#include <boost/mpl/multiplies.hpp>

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
#endif // SERVICE && __PATH__ && !_OPEN_TEST_

#if ( !defined ( NO_GAMEGUARD ) || !defined ( NO_HACKSHIELD ) || defined( USE_XTRAP ) ) && !defined ( _DISABLE_HACK_CHECK_ )
#   define _DISABLE_HACK_CHECK_
#endif

// 해킹 검사 해제 매크로가 정의되어 있는지 확인한다.

// -----------------------------------------------------------------------------
// 국가코드
// -----------------------------------------------------------------------------
#   define NATION_CODE L"BR"

//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#   define _PORTUGUESE          // 포르투갈어
#   define NATION_NAME          "Brazil"

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

#ifdef _PORTUGUESE          // 포르투갈어
#   define GC_CODE_PAGE_IDENTIFIER  (860)
#endif // _PORTUGUESE

//------------------------------------------------------------------------------------------------------------------------
// 레이턴시(한번에 보내는 버퍼량)
#define LATENCY                 (5)
#define SHOWLATENCY                 (2)

#define MAX_OBJECT                  (30)
#define MAX_MONSTER                 (80)
#define BEGIN_NPC					(80)
#define MAX_PET_NUM                 (50)
#define LIMIT_INVENTORY_SIZE		(5000)
#define DEFAULT_INVENTORY_NUM       (3000)
#define MAX_INVENTORY_SIZE          (4000)
#define GC_COORDI_MAX_INVEN_SIZE	(1000)
#define EXTEND_INVENTORY_SIZE_500   (500)      ///< +500 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_200   (200)     ///< +200 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_100   (100)    ///< +100 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_60    (60)    ///< +60 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_30    (30)   ///< +30 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_10    (10)  ///< +30 인벤토리 확장 아이템을 사용 시 늘어나는 크기
#define EXTEND_INVENTORY_SIZE_5     (5)  ///< +5 인벤토리 확장 아이템 사용 시 늘어나는 크기
#define MAX_EQUIP_NUM               (36)    ///< 최대 장착 개수
#define MAX_NUM_MON_SLOT            (5)     ///< 최대 몬스터 카드 슬롯 개수
#define NUM_TAKE_ITEM               (3)     ///< 한 플레이어가 가질 수 있는 게임 아이템 최대 개수
#define MAX_NUM_ITEM_IN_MAP         (20)
#define DEFAULT_BAN_COUNT           (3)
#define MAX_GP_VALUE             (100000000)
#define DBOX_SIZE_X 420
#define DBOX_SIZE_Y 420
#define GIFT_DBOX_SIZE_X 480          ///< 선물하기 웹 브라우져 크기 X
#define GIFT_DBOX_SIZE_Y 705          ///< 선물하기 웹 브라우져 크기 Y

#define GEM_PER_CASH_RATE       (20)

#define GC_NICKNAME_SIZE        (20)
#define GC_ACCOUNT_NICKNAME_SIZE        (8)
#define MAX_LOGIN_ID_SIZE       (24)    // DB에 저장되는 로그인 아이디의 최대 길이 : 20
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
#define REPORT_MAIL_FROM                   "denuncias@chaseorigin.com.br"
#define REPORT_MAIL_TO                     "denuncias@chaseorigin.com.br"
#define REPORT_MAIL_SMTP                   "14.45.79.45"

const __int64 nMailSHAKey = 189745675553;
const int nMailPassWordLen = 32;
const unsigned char szMailPassword[nMailPassWordLen + 1] =
{
    0x10, 0x03, 0x07, 0xb3, 0x05, 0xac, 0xc1, 0xc7,
    0x5f, 0x00, 0x10, 0xbe, 0xeb, 0xbd, 0xfa, 0x27,
    0xea, 0xf2, 0x12, 0xc3, 0x03, 0x9f, 0xcb, 0x9b,
    0xf2, 0x54, 0xab, 0x1a, 0xbf, 0x71, 0x7b, 0x4b
}; //rmcp123QAZ

//========================================================================
/*[ 브라질 특화 국가 디파인 시작 ]
========================================================================*/
#define GAME_CASH
//#	define STATIC_WORDWRAP
#	define SQUARE_OPEN
#	define ENABLE_POINT_SYSTEM
#   define MISSION_COUNT_ITEM       // 미션 레시피 카운트 아이템으로
#	define USING_HELL_MODE
#	define USE_JOYSTICK
#   define ENABLE_MINIGAME
#	define BUG_TRAP					// 크래쉬리포트방법
#define SKILL_TREE               // 스킬트리
#define COUPLE_SYSTEM
//#   define EVENT_SANTA
#define GP_CAPSULE				// GP 캡슐
#define PING_STATISTICS
#define EMOTICON_SYSTEM
#define USE_LOGOUT
//#	define CHAT_EVENT               // 채팅 이벤트~ 뭐.. 같이 외치는거...
#define GUILD_SYSTEM
#if defined(__PATH__)
#define RUN_PATCHER_WHEN_NO_PARAMETER
#endif

//#define WELCOMEBACK_FRIEND
#define ENABLE_DUGEON_HACK_CHECKING
#define ONLY_COLLECT_HACKING_DATA// 해킹체크는 하되 데이터만 수집할려고 할 때 사용
//#   define COOPORATION_EVENT      //  예를 들면 버찌볼 이벤트 광장에 아이콘 표시가 되는 것.
#define SELL_SCROLL_IN_GACHA_UI
#define FORCE_KICK_SLEEP_USER    // 5분간 방에서 액션없는 유저 조내기
//#define SEASON3_COMBO_SYSTEM     //  시즌3 콤보 시스템 사용

//#   define DISABLE_ITEM_SOCKET          //  이걸넣으면

#define CHECK_DUPLICATED_FILE
#define NEW_LEVELUP_EFFECT       //  새로 만든 레벨업 이펙트 사용
#define ROOM_MISSION_ICON        //  던전 난이도선택 화면에서 미션 아이콘 출력
#define MISSION_COMPLETE_TOOLTIP // 미션 완료 버튼에 툴팁 사용 여부
#define EXIT_DUNGEON_MESSAGE     //  던전 퇴장 메세지 컬러풀하게
//#define USE_GAMBLE_SYSTEM
#define AVI_RECODER
#define REPLAY_BOX_NEW   // 새 리플레이 보기 사용   // 2010 10 06 이번에는 롤백
//#define RETRY_LOGIN_ON_CLOSE_CONNECTION  //  접속 불량시 로그인 창으로 돌리기
#define LOADING_CHAR_MOTION          //  새로운 대전 로딩 모션 적용 ->ResSet에 리소스 합쳐야 함.
#define USE_HERO_DUNGEON				//	영웅던전
#define MATCH_BALANCE			 // 대전 벨런스 적용
#define LIRE2_RENEW				//	리르 개편
#define ARME2_RENEW				//	아르메 개편
#define ITEM_BREAKUP_INFO_BOX
///시즌 4 이후 들어간 디파인들
//#define REGION_MESSAGEBOX        // 리젼 메시지 박스
#define MISSION_COMPLETE_ITEMLIST_BOX
#define USE_COORDI_SHOP //코디샵이다
#define USE_PACKAGE_PREVIEW //패키지 아이템 보기

//아주 작은 온오프 기능들의 Define
#define LOADING_IMAGE_FROM_SERVER_SCRIPT
#define ATTRIBUTE_SELECT_NEW
#define GUILD_RENEW_S4          //시즌 4 길드 개편 (길드전)
#define GUILD_STORE_S4          //길드상점
#define WEAPONCHANGE
// 무기 체인지 레벨제한
#define WEAPONCHANGE_LV             (45)
#define PVP_SERVER_TEST //유저들도 PVP_Balance 서버에서 맵이랑 모드 선택이 되도록 하는 디파인
#define USE_SOCIAL_MOTION                // 소셜 모션 사용
#define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM    //트레져박스 아이템이 장착 레벨을 제한 시키겠다.(상자 까는데 레벨제한 두는 것)
#   define USE_NEW_CHARACTER_SELECT              //바뀐 캐릭터 선택창 사용할 것인가 다음주에 풀자
#   define TOOLTIP_FOR_SUPPORT_NEW_CHARACTER
//#   define USE_VIP_MENU_BTN
//#   define USE_VIP_NEW_USER_TAB //신규 유저 보상 탭 살릴건지?
#define USE_SURVEY_SYSTEM //설문조사 시스템
#define MATCH_BALANCE_2011_8     // 2011년 8월 25일 국내에 업데이트된 캐릭터 벨런스
//#define USE_EVENTDUN_CARNIVAL    // 이벤트 던전( 서커스 카니발 )
#define ANGELS_EGG_REFORM //알깨기 3D 형태 인식
#define DLL_INJECTION_CHECK //dll 체크코드(서버initclientcontents에DLLBlackList 목록을가지고있음)
#define GWC_EVENT_2011 //GWC 이벤트
#define USE_HACK_CHECK_MANAGER

//아지트 관련
#	define PARTY_SYSTEM						// 파티 시스템
#	define USE_AGIT_SYSTEM					// 아지트 코드
#   define USE_SOCIAL_MOTION                // 소셜 모션 사용

#define USE_CHRISTMAS_GIFT_EVENT // 크리스마스 선물 이벤트
#define USE_JUST_IN_TIME_EVENT
# define USE_POST_LETTER_SYSTEM // 우편함 사용
# define USE_PING_STATE_UI //브라질도 이제 핑 안테나 사용
//#define COLLECT_SKILL_HOTKEY_USED_INFO // 필살기 단축키 사용율 수집
#define USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE // 전직미션 구매 가이드
#define USE_TUTORIAL_LOADING    // 연습모드 로딩 사용
//# define USE_LEVEL_LIMIT_GACHA // 레벨제한 가차사용 - 획득한 가차레벨의 동레벨 최종보상 받기
#define NO_MSG_BOX_SKILL_LEARN_AND_REMOVE
//#define USE_CONTRIBUTE_WITH_NO_EVENT // 기부에 아무 이벤트 없이 UI 띄우기
#define HERO_DUNGEON_REWARD
#define COLLECT_SKILL_HOTKEY_USED_INFO //MP 캐릭터 대반격
#define LOAD_ENCHANT_EFFECT_FROM_SCRIPT //강화 이펙트 정보 스크립트에서 읽기
#define USE_WEATHER_SYSTEM //날씨 시스템 사용 (아신 들어가면서 들어간것)
#define USE_FACE_LIFT //얼굴표정 교체 기능
//#define LOCK_ASIN_HIDDEN_SKILL
//#define USE_NEW_RECOM  //새로운 상점 Recom 사용하기
#define SERVER_ONLY_NON_COUNT_ITEM
#define NEW_CHARACTER_SELECT_BOX
#define SKILL_TREE_OPEN_BY_SERVERSCRIPT  //서버에서 스킬 정보 받아오도록
#define PVP_SERVER_EVENT_NOT_CONVENTION
#define RIN_PROMOTION //린 전직 들어 가면 사용 (전직 후 캐릭터가 MP -> 하이브리드로 바꼈다)
#define USE_QA_LOG_SYSTEM //QA 로그 시스템 사용
#define USE_GP_ITEM_ATTRIBUTE_SELECT
#define USE_HERO_ITEM_UPADE //영웅던전 개편 디파인 (영웅상점 업그레이드)
#define USE_NEW_ENCHANT_SYSTEM //신강화
#define USE_VIP_POINT // VP(가상캐쉬) 사용
#define DEVICE_MULTITHREADED //파티클 멀티 스레드 로드

#define USE_ATTRIBUTE_SELECT_EVENT
#define WORLD_MAP_MISSION_ICON //월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#define ATTRIBUTE_SELECT_MAX_COUNT ( 3 )    // 속성 재설정 이벤트 기간이 아닐때 속성 변경 가능한 최대 개수

//------------------------------------------------------------------------------------------------------------------------
//XTrap 관련 디파인
#if defined(__PATH__)
#define XTRAP_HTTP_URL_KEY "660970B4485BDB86C5556D9844CFEF62963E1A0B24BF43985542242973DCE2AAF242150F6F5D1B83A9E928767FE9691C38F44C63AAEDE6EF1C176F8AD3904EF70F7D04245246E41FB1670C4E9557650C03609EEBF61B756298FD5E7377DF"
#else
#define XTRAP_HTTP_URL_KEY "660970B4485BDB86C5556D9844CFEF62963E1A0B24BF43985542242973DCE2AAF242150F6F5D1B83A9E928767FE9691C38F44C63AAEDE6EF1C176F8AD3904EF70F7D04245246E456F13B590B95072E43B903BDC708DFBFA551C165"
#endif

#define XTRAP_CHECK_INTERVAL ( 20.0f )

//========================================================================
/*[ 브라질 특화 국가 디파인 끝 ]
========================================================================*/
// 버튼잠금프레임
#define BUTTON_LOCK_FRAME 25

#define BUY_WEB_PAGE( stm )
#define CHARGE_WEB_PAGE( stm )
#define GET_CASH_WEB_PAGE( stm )

#define SELL_DIV_VALUE 5
#define SELL_GP_DIV_VALUE 10

#define GCDeviceFont GCDeviceFontBase
#define CHAR_ABILITY_STATE
#define DUNGEONPLAYSTATE

////시즌 이터널
//새로운 튜토리얼 가이드 디파인
#define USE_MATCH_STATUS_RATE       // 대전 능력치 비율 조정 사용
#define MATCH_BALLENCE_121217 //자동 매칭으로 인한 밸런스 개편
#define USE_NEWBIE_GUIDE
#define USE_GUIDE_SYSTEM        // 가이드 시스템 사용
#define USE_18TH_CHAR           // 18차 캐릭터 사용하는지??? ui들에서 처리되어야 할 것들 있어서 디파인 추가함.
#define USE_MATCH_SERVER_EGDE
#define USE_BIND_PET            // 캐릭터 귀속 펫 사용

// 대전방에서 적용되는 능력치 비율
#define COUNTER_DEF_PERCENT_MATCH   ( 0.5f )    // 대전에서는 착용 장비에 있는 카운터 저항 값에서 50%
#define COUNTER_DEF_MAX_VALUE_MATCH ( 0.5f )    // 대전에서 최대 적용되는 카운터 저항은 50%
#define MP_RECOVERY_PERCENT_MATCH   ( 0.25f )   // 대전에서는 착용 장비에 있는 MP회복력 값에서 25%
#define MP_RECOVERY_MAX_VALUE_MATCH ( 0.25f )   // 대전에서 최대 적용되는 MP회복력은 25%

//Additional Ep2 and Custom 
#define USE_19TH_CHAR
#define USE_ADMIN_COLOR_CHAT
#define PET_COSTUME

//Origin Migration
#define DSHOW
#define ALICE
#define ELYOS
#define PERYTON
#define DECANEE
#define NEW_MENU
#define BUG_TRAP
#define AGREEMENT
#define FEATURE_2021
#define ENCHANT_EFFECT
#define POTION_ROOM_SHOW
#define DIMENSIONAL_BREAK
//#define NEW_CHAR_SELECT_SCENE
#define _ENABLE_FIXED_PIPELINE
#define VIP_COLLECTION_SIZE (11)
//#define _DISABLE_ONLY_ONE_EXECUTE_
#define USE_LOOK_INVEN_EXTEND
//#define CONSOLEDEVV // Para utilizar o console dev