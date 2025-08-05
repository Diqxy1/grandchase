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
#define NATION_CODE L"EU"

//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#define _ENGLISH             // 영어
#pragma message( "This is English version!" )
#if defined( NATION_EU )
#define NATION_NAME          "EU"
#endif

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

#    define LATENCY                 (5)
#define SHOWLATENCY                 (2)

#define MAX_MONSTER                 (80)
#define BEGIN_NPC					(80)

#define SUMMON_MONSTER_INDEX        (15)
#define DOTA_MONSTER_INDEX          (30)
#define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
#define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
#define END_DOTA_MONSTER            (MAX_MONSTER)

#define MAX_LEVEL                   (40)    ///< 그랜드체이스 캐릭터의 최고 레벨
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

#define MAX_GP_VALUE             (9999999) ///< GP 최대치

#define DBOX_SIZE_X 420          ///< 구매 웹 브라우져 크기 X
#define DBOX_SIZE_Y 420          ///< 구매 웹 브라우져 크기 Y

#define GEM_PER_CASH_RATE       (200)   ///< 캐쉬 금액 당 GEM 비율

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
    #define    MD5_PASSWORD_LOGIN
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
}; // ""


#define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#define HALF_ATTACK_POINT		// 공격력 2배 하지 않음
#define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
#define STATIC_WORDWRAP			// 영문 워드랩 해주는거
#define OLD_GRADE_SYSTEM			// 대전 강화가 들어가기 전 예전 계급 시스템

//#   define WELCOMEBACK_FRIEND
#define MISSION_COUNT_ITEM
#define DISABLE_AUTOSHOT         // 오토마커!
#define USING_HELL_MODE
#define EVENT_SANTA              // 산타이벵
//#   define CHAT_EVENT               // 채팅 이벤트~ 뭐.. 같이 외치는거... 
//#   define EVENT_MAP                // 할로윈 배경같이 맵 드간거~
//#   define RANDOM_LOADING_IMG       // 초기로딩이미지 ~ 랜덤으로 ~ 
#define WORLDMAP_CHARACTER_ICON_FOR_DRAMA
#define USING_ADMIN_CHATCOLOR
//#define SKILL_TREE               // 스킬트리
#define BUG_TRAP					// 크래쉬리포트방법
#define USE_JOYSTICK
#define USE_HTTPPOST
#define PET_COSTUM				//펫 코스튬
#define FORCE_KICK_SLEEP_USER    // 5분간 방에서 액션없는 유저 조내기
//#define COUPLE_SYSTEM
#define ENABLE_DUNGEON_OBSERVER //던전에서 옵저버모드로 관전가능
#define ONLY_COLLECT_HACKING_DATA
#define ENABLE_TEAM_CHAT			// 팀 채팅기능 사용
#define GP_CAPSULE				//GP캡슐 
#define CRYSTAL_CAPSULE			//크리스탈캡슐
#define EMOTICON_SYSTEM
#define CHECK_DUPLICATED_FILE
#define CALENDAR_RECOMMAND       // 출석 찍으라고 추천하는 시스템?
#define SEASON3_COMBO_SYSTEM
#define DICE_PLAY
//디오 들어가고 들어가는 것들~!
#define LOAD_NECESSITY_TEXTURE   // 정보가 충분히 수집된 후 패치되어야 합니다.

// 디오 다음에 들어가자!!
#define ITEM_BREAKUP_INFO_BOX   // 아이템 해체 정보박스
#define LOADING_IMAGE_FROM_SERVER_SCRIPT
#define GUILD_SYSTEM
//#define USE_HERO_DUNGEON
#define LOADING_CHAR_MOTION      // 대전로딩 캐릭터 모션
#define USE_PING_STATE_UI
#define CHECK_SEND_PING
#define USE_STAGE_LOAD_COMPLETE_SIGN
#define USE_EVENT_BANNER        //이벤트 배너
#define USE_GAMBLE_SYSTEM

#define BUTTON_LOCK_FRAME 20
#define MATCH_BALANCE_2011_8     // 2011년 8월 25일 국내에 업데이트된 캐릭터 벨런스
#   define MATCH_BALANCE            // 대전 벨런싱
#   define USE_HOST_MIGRATION        
///시즌 4 이후 들어간 디파인들
//#define REGION_MESSAGEBOX        // 리젼 메시지 박스
#define MISSION_COMPLETE_ITEMLIST_BOX
//#define USE_GAMBLE_SYSTEM
//#define WEAPONCHANGE
// 무기 체인지 레벨제한
//#define WEAPONCHANGE_LV             (45)
//#define USE_SELL_GACHA_BTN          //신가챠 버튼 메뉴UI출력

//#define DIRECTX_VERSIONUP_2009_AUG // DirectX 2009 Aug로 버전업
#define USE_NEW_ENCHANT_SYSTEM

#define USE_P2P_OR_TCP_RELAY
#define ACHEIVE_SYSTEM
#define PING_STATISTICS
#define USE_LUA_API
#define NEW_LEVELUP_EFFECT
#define ROOM_MISSION_ICON        // 던전 대기방에서 미션 아이콘 표시하기 
#define WORLD_MAP_MISSION_ICON
#define LOAD_SLANG_FROM_KOM

#if defined( __PATH__ )     //미국 본섭
#define BUY_WEB_PAGE( stm )         (stm<<"https://www.ntreev.net/Shop/gc/Game/BuyItem/")
#define CHARGE_WEB_PAGE( stm )      (stm<<"https://www.ntreev.net/recharge/game/goto/gc")
#define GET_CASH_WEB_PAGE( stm )    (stm<<"http://www.ntreev.net/Recharge/MyPoint/Goto/gc")
#define GACHA_WEB_PAGE( stm )       (stm<<"http://grandchase.ntreev.net/gacha/playgacha.aspx")
#else                       //미국 사내
#define BUY_WEB_PAGE( stm )         (stm<<"http://www.ntreev.net:8080/Shop/gc/Game/BuyItem/")
#define CHARGE_WEB_PAGE( stm )      (stm<<"https://www.ntreev.net/recharge/game/goto/gc")
#define GET_CASH_WEB_PAGE( stm )    (stm<<"http://www.ntreev.net:8080/Recharge/MyPoint/Goto/gc")
#define GACHA_WEB_PAGE( stm )       (stm<<"http://216.39.108.134:8080/gacha/playGacha.aspx")
#endif

#define SELL_DIV_VALUE 3
#define SELL_GP_DIV_VALUE 10

#define CLOSED_BETA

//#define WEB_CASH 

#define GCDeviceFont GCDeviceFontBase
#define USE_GC_LOG_SYSTEM