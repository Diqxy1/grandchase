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
#define NATION_CODE L"USA"

//------------------------------------------------------------------------------------------------------------------------
// 국가별 언어 정의
#define _ENGLISH             // 영어
#pragma message( "This is English version!" )
#if defined( NATION_USA )
#define NATION_NAME          "USA"
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

#    define LATENCY                 (8)
#define SHOWLATENCY                 (2)

#define MAX_OBJECT                  (30)
#define MAX_MONSTER                 (80)
#define BEGIN_NPC					(80)
#define SUMMON_MONSTER_INDEX        (15)
#define DOTA_MONSTER_INDEX          (30)
#define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
#define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
#define END_DOTA_MONSTER            (MAX_MONSTER)

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

#define MAX_GP_VALUE             (9999999) ///< GP 최대치

#define DBOX_SIZE_X 420          ///< 구매 웹 브라우져 크기 X
#define DBOX_SIZE_Y 420          ///< 구매 웹 브라우져 크기 Y

#define GIFT_DBOX_SIZE_X 480          ///< 선물하기 웹 브라우져 크기 X
#define GIFT_DBOX_SIZE_Y 705          ///< 선물하기 웹 브라우져 크기 Y

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

#define PARTICLEPOS_Z			0.2f

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

//  파티클 멀티쓰레드 로드 할려면 반드시 켜야 합니다.
#define DEVICE_MULTITHREADED
const __int64 nMailSHAKey = 189745675553;
const int nMailPassWordLen = 32;
const unsigned char szMailPassword[nMailPassWordLen+1] = 
{ 
    0xe4, 0xde, 0x4e, 0xb5, 0x01, 0x67, 0xb1, 0xc6, 
    0xd4, 0xd1, 0x6c, 0x18, 0xf7, 0x86, 0x03, 0xbd, 
    0xd8, 0xd2, 0xa2, 0xe7, 0xf4, 0xd5, 0x42, 0x11, 
    0x1f, 0xef, 0xd0, 0x0d, 0x3f, 0x0d, 0x49, 0xcc
}; //""

//모든 시스템에 대한 디파인 소개
#if defined( _ALL_SYSTEM_DEFINE_INTRODUCTION_ )
#	define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#	define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
#	define TAIWAN_USER_AGREEMENT	// 대만 유저 동의서
#	define WEB_CASH					// 웹캐쉬
#	define GAME_CASH				// 게임내캐쉬
#	define USING_HELL_MODE			// 헬모드
#	define WELCOMEBACK_FRIEND		// 반갑다친구야
#   define FORCE_KICK_SLEEP_USER    // 5분간 방에서 액션없는 유저 조내기
#   define ENABLE_MINIGAME          // 미니게임 할래?
#   define SKILL_TREE               // 스킬트리
#	define USE_JOYSTICK
#	define RING_OF_FRIENDSHIP_UNLIMIT_REBIRTH	// 프랜드쉽 반지로 던전에서 3회 무료부활
#   define EVENT_SANTA              // 산타이벤트
#	define BUG_TRAP					// 크래쉬리포트방법
#	define COUPLE_SYSTEM
#	define GUILD_SYSTEM
#	define PET_COSTUME
#	define EMOTICON_SYSTEM
#	define GP_CAPSULE				// GP 캡슐
#	define ENABLE_TEAM_CHAT			// 팀 채팅기능 사용
#	define PING_STATISTICS			// 핑 통계 측정
#	define RUN_PATCHER_WHEN_NO_PARAMETER
#	define ENABLE_DUGEON_HACK_CHECKING	//던전에서 몬스터의 올킬을 체크한다.	
#	define ONLY_COLLECT_HACKING_DATA// 해킹체크는 하되 데이터만 수집할려고 할 때 사용
#	define STATIC_THAI_WORDWRAP		// 태국어 워드랩 해주는거
#   define SEASON3_COMBO_SYSTEM
#   define CHECK_DUPLICATED_FILE //중복된 파일 검출시 클라이언트 종료.
#   define CALENDAR_RECOMMAND       // 출석 찍으라고 추천하는 시스템?
#   define CALENDAR_RECOMMAND       // 출석 찍으라고 추천하는 시스템?
#   define DISABLE_ITEM_SOCKET
#   define USE_PING_STATE_UI
#   define ITEM_BREAKUP_INFO_BOX    // 아이템 해체 정보박스
#   define WORLD_MAP_MISSION_ICON   // 월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#   define ROOM_MISSION_ICON        // 던전 대기방에서 미션 아이콘 표시하기 
#   define EXIT_DUNGEON_MESSAGE     // 던전 플에이 중 나갈 때 잃을 수 있는 것 명확히 표시
#   define MISSION_COMPLETE_TOOLTIP // 미션 완료 버튼에 툴팁 사용 여부 
#   define MATCH_BALANCE            // 대전 벨런싱
//#   define USE_HOST_MIGRATION
#	define USE_GAMBLE_SYSTEM
#   define FONT_INSTALL_FUNC
#   define MISSION_COMPLETE_ITEMLIST_BOX
#   define LOADING_CHAR_MOTION      // 대전로딩 캐릭터 모션
#   define ITEMBOX_ANI_TOGGLE       // 아이템정보박스 애니메이션 토글 사용
#   define ENABLE_PVP_SHOP          // PVP서버에서 상점 탭 풀기
#   define USE_EVENT_BANNER         // 이벤트 베너
#   define USE_HERO_DUNGEON
#   define DIRECTX_VERSIONUP_2009_AUG // DirectX 2009 Aug로 버전업
#   define USE_EVENT_BANNER_NEW // 이벤트 베너 새버전 
#   define USE_QUIZ_BINGO               //VIP보상 시스템 버튼 메뉴 UI에 출력
#   define USE_VIP_MENU_BTN 
#   define USE_MIGRATION_BTN        //MIGRATION
#   define BUY_CASH_ITEM_WITH_POINT        //캐시 아이템을 출석포인트로 구매 가능함
#   define NEW_GPITEM_BUY_PROCESS                      //GP아이템 구입할때 포인트가 부족하면 구입 확인창을 띠우지 않는다.
#   define SERVER_ONLY_NON_COUNT_ITEM                  //NonCount Item을 서버에서만 받는다.
#   define ANGELS_EGG_REFORM                  //NonCount Item을 서버에서만 받는다.
#	define PARTY_SYSTEM						// 파티 시스템
#	define USE_AGIT_SYSTEM					// 아지트 코드
#   define USE_SOCIAL_MOTION                // 소셜 모션 사용 
#   define USE_MAGIC_BOX //코우나트 마법상자
#   define USE_POST_LETTER_SYSTEM // 우편함 사용 
#   define LOAD_ENCHANT_EFFECT_FROM_SCRIPT
#   define USE_GOLD_SPEAR // 헬 스피어를 골드 스피어로 
#   define USE_GP_ITEM_ATTRIBUTE_SELECT // GP 아이템 속성변환 사용 
#   define USE_BALANCE_SERVER_MARK  // 서버바에 벨런스 서버 마크 띄워즘 

#endif

#if defined (_ALL_SMALL_FUNCTION_DEFINE_INTRODUCTION_)  //아주 작은 온오프 기능들의 Define
#	define CHAT_EMOTICON_EMPTY_ROLLOVER
#   define COUPON_TWO_INPUT
#	define SAVE_CHATCOLOR_INFO
#   define COOPORATION_EVENT
#   define LOADING_IMAGE_FROM_SERVER_SCRIPT
#	define LINK_HOMEPAGE_WHEN_ENDGAME
#   define TOOLTIP_FOR_SUPPORT_NEW_CHARACTER                       //신규캐릭터 지원 툴팁 사용여부
#   define MINIMIZE_ALL_WINDOW
#endif

#if defined( _TRUNK )
#	define GP_CAPSULE				// GP 캡슐
#	define WELCOMEBACK_FRIEND		// 반갑다친구야
#endif

#if defined( NATION_USA )

# if !defined( __PATH__ )
#           define VIRTUAL_CASH
#else
#           define ONLY_SINGLE_SEAL_CLEAR
# endif

// gash? probably? idk
#define VIRTUAL_CASH
#define BUY_CASHITEM_WITH_VIRTUAL_CASH


#define SQUARE_OPEN				// 광장 사용 광장 입장 버튼을 막음
#define ENABLE_POINT_SYSTEM		// 포인트 시스템 사용여부..
//#define WEB_CASH
#define USING_HELL_MODE
//#define WELCOMEBACK_FRIEND		// 반갑다친구야
#define FORCE_KICK_SLEEP_USER
#define ENABLE_MINIGAME
#define SKILL_TREE               // 스킬트리 (길드전 시 빼야할 항목)
#define COUPLE_SYSTEM
#define GUILD_SYSTEM
#define PET_COSTUME
#define ENABLE_DUGEON_HACK_CHECKING

//#   define USING_HELL_MODE
#   define EVENT_SANTA              // 산타이벤트
#	define BUG_TRAP					// 크래쉬리포트방법
#	define EMOTICON_SYSTEM
#	define PING_STATISTICS			// 핑 통계
//#   define COOPORATION_EVENT
#	define GUILD_SYSTEM

#  define CHECK_DUPLICATED_FILE //중복된 파일 검출시 클라이언트 종료.

#define CHECK_SEND_PING
//#define USE_PING_STATE_UI
#define ITEM_BREAKUP_INFO_BOX   // 아이템 해체 정보박스
#define USE_HOST_MIGRATION
#define WORLD_MAP_MISSION_ICON   // 월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#define ROOM_MISSION_ICON        // 던전 대기방에서 미션 아이콘 표시하기 
//#define EXIT_DUNGEON_MESSAGE     // 던전 플에이 중 나갈 때 잃을 수 있는 것 명확히 표시
#define MISSION_COMPLETE_TOOLTIP // 미션 완료시 툴팁 사용 여부 
//#define USE_GAMBLE_SYSTEM
#define MATCH_BALANCE			 // 대전 벨런스 적용
#define NEW_LEVELUP_EFFECT       //  새로 만든 레벨업 이펙트 사용
#define FONT_INSTALL_FUNC        // 폰트설치기능 사용
//#define MISSION_COMPLETE_ITEMLIST_BOX //시즌 4에서는 사용하지 않음
#define LOADING_CHAR_MOTION      // 대전 로딩 캐릭터 모션 처리방식 변경
#define ITEMBOX_ANI_TOGGLE       // 아이템박스 애니메이션 옵션토글
#define ENABLE_PVP_SHOP          // 대회서버에서 상점탭버튼 활성화
//#define USE_EVENT_BANNER
#define USE_HERO_DUNGEON
#define REGION_MESSAGEBOX        // 리젼 메시지 박스
#define LOADING_IMAGE_FROM_SERVER_SCRIPT
#define WEAPONCHANGE
#define NICKNAME_CAMPAIGN
#define CHARACTER_STATS
#define DIRECTX_VERSIONUP_2009_AUG// DirectX 2009 Aug로 버전업
#define USE_EVENT_BANNER_NEW


#define GUILD_RENEW_S4          //시즌 4 길드 개편 (길드전)
#define GUILD_STORE_S4          //길드상점
#define USE_COORDI_SHOP
#define USE_VIP_MENU_BTN            //VIP보상 시스템 버튼 메뉴 UI에 출력
//#define USE_VIP_NEW_USER_TAB        //신규 구매용 VIP도 사용 
//#define EVENT_VIEW_ITEM             //특정시간에만 샵에 아이템 보이게 하기
//#define EVENT_ITEM_CHANGE           //특정시간에만 샵에 있는 아이템을 바꾸게 하기
#define NEWS_BANNER_OPTIMIZESIZE    //채널뉴스에서 왼쪽에 보이는 이미지 싸이즈를 투명부분 뺀 싸이즈만 렌더링 하도록 함 //512에서 404.0f로 변경 하려면 ServerNews.lua 리소스에서 m_vTextureCoord_y = 0.789로 해야함
#define USE_PACKAGE_PREVIEW      //패키지 아이템 미리보기

#define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM    //트레져박스 아이템이 장착 레벨을 제한 시키겠다.
#define NICKNAME_CHECK_WITH_WSTRING_REAL_LENGTH         //wString의 실제 길이를 사용해서 닉네임 길이를 체크한다.
#define TOOLTIP_FOR_SUPPORT_NEW_CHARACTER                       //신규캐릭터 지원 툴팁 사용여부

#define BUY_CASH_ITEM_WITH_POINT        //캐시 아이템을 출석포인트로 구매 가능함

#define NEW_GPITEM_BUY_PROCESS                      //GP아이템 구입할때 포인트가 부족하면 구입 확인창을 띠우지 않는다.

#define SERVER_ONLY_NON_COUNT_ITEM                  //NonCount Item을 서버에서만 받는다.
#define ANGELS_EGG_REFORM                   // 천사의 알깨기 개편
#define USE_AGIT_SYSTEM                     // 아지트 
#define PARTY_SYSTEM                        // 파티시스템
#define USE_NEW_ENCHANT_SYSTEM

//#define MINIMIZE_ALL_WINDOW
#define USE_SOCIAL_MOTION
#define MATCH_BALANCE_2011_8     // 2011년 8월 25일 국내에 업데이트된 캐릭터 벨런스
#define DUNGEONPLAYSTATE         // 던전 플레이 통계
//#define USE_ONGAME_INVENTORY     // 게임내 인벤토리 
#define USE_NEW_ITEM_CHECK       // 아이템 획득시 아이템 체크
#define NEW_CHARACTER_SELECT_BOX_NEW // 신규 유저 캐릭터 선택박스 새 버전 

#define USE_SURVEY_SYSTEM       //설문조사 시스템
#define USE_EVENT_BANNER_SHOP   // 이벤트 베너에서 구매창 띄우기 
#define GWC_EVENT_2011          //GWC 이벤트
#define USE_POST_LETTER_SYSTEM // 우편함 사용 
#define USE_CHRISTMAS_GIFT_EVENT //선물 이벤트 
//#define WELCOMEBACK_FRIEND_NEW // 반갑다 친구야 추천인 4인 버전 -  WELCOMEBACK_FRIEND / WELCOMEBACK_FRIEND_NEW 둘다 걸어야 사용 가능 

#define USE_TUTORIAL_LOADING    // 연습모드 로딩 사용
#define USE_COORDI_CLEARSEAL    // 코디가챠
#define USE_JUST_IN_TIME_EVENT  //  JustInTime 이벤트
//#define USE_QUIZ_BINGO          // 퀴즈 빙고 
#define LOAD_ENCHANT_EFFECT_FROM_SCRIPT // 강화 이펙트 정보 스크립트에서 읽기 
//#define USE_GOLD_SPEAR                  // 헬 스피어를 골드 스피어로 
#define USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE
#define USE_CONTRIBUTE_WITH_NO_EVENT // 기부에 아무 이벤트 없이 UI 띄우기 
#define CHAR_ABILITY_STATE         // 캐릭터 능력치 통계
#define USE_DYNAMIC_VERTEX_BUFFER
#define USE_MISSION_COMPLETE_EFF // 미션 완료시 지정 이펙트 사용
//#define USE_MERRY_HP_BAR
#define HERO_DUNGEON_REWARD
//#define USE_LEVEL_DOWN
//#define USE_LEVEL_LIMIT_GACHA       // 획득 가챠 레벨에 맞게 최종 보상 
#define USE_OLYMPIC_EVENT           // 올림픽 이벤트
#define USE_CAMERA_ZOOM_STATE       // 카메라 줌 단계 통계
#define USE_MATCH_STATUS_RATE       // 대전 능력치 비율 조정 사용 
#define USE_GP_ITEM_ATTRIBUTE_SELECT // GP 아이템 속성변환 사용 
//#define USE_BALANCE_SERVER_MARK  // 서버바에 벨런스 서버 마크 띄워즘 
#define USE_MATCH_SERVER_EGDE    // 매칭 서버 서버 목록에 테두리 넣기 
//#define DISABLE_ITEM_SOCKET
//#define COLLECT_RECEIVED_P2P_PACKET // ReceivedP2PPacket.txt 파일에 받은 p2p패킷을 기록
//#define NO_USE_ADDITION_GUIDE       //임시 빤스 가이드 제키기 디파인
#define USE_MAGIC_BOX //코우나트 마법상자
#define USE_HERO_ITEM_UPGRADE
#define USE_BIND_PET            // 캐릭터 귀속 펫 사용 
#define USE_LOOK_INVEN_EXTEND   // 코디 인벤 확장 이용 
#define USE_18TH_CHAR           // 18차 캐릭터 사용하는지??? ui들에서 처리되어야 할 것들 있어서 디파인 추가함.
#define USE_19TH_CHAR
#define USE_GUIDE_SYSTEM        // 가이드 시스템 사용

#if !defined( __PATH__ )
#define USE_QA_LOG_SYSTEM          // QA용 로그 
#endif

#if !defined(__PATH__)
#define USE_LOADINGTIME_RECORD // 로딩시간기록
#endif

//#define COLLECT_SKILL_HOTKEY_USED_INFO

// 사내에서만 사용할 기능들
#if !defined( __PATH__)
#define USE_SHADER_LOAD_HLSL // Shader를 컴파일된것과 HLSL 둘다 로드할 수 있도록 할것인가??( 사용하지 않으면 컴파일된것으로만 로드함 )
#endif// #if !defined( __PATH__)

#endif// if defined( NATION_KOREA )

#define PET_LEVEL_IN_SHOP (30)

// 버튼잠금프레임
#define BUTTON_LOCK_FRAME 20

#define NO_MSG_BOX_SKILL_LEARN_AND_REMOVE

#define AVI_RECODER                         // 동영상 녹화 
#define REPLAY_BOX_NEW

//#define USE_AP_SLOT_CHANGE

#define COLLECT_FRAME_DATA
#define USE_WEATHER_SYSTEM

#define USE_FACE_LIFT //얼굴표정 교체 기능
//#define LOCK_ASIN_HIDDEN_SKILL

//#define USE_AWAKENING_SKILL_TREE

// 신강화시스템

#if defined( _OPEN_TEST_ ) || defined(NATION_THAILAND)
#define DISABLE_BADGUY_REPORT
#endif


//  레벨 디자인 관련 디파인 설정
#define LEVEL_DESIGN_PROMOTION_STAT     // 전직 능력치
#define LEVEL_DESIGN_STAT_CALC          // 공격력, 방어력 % 계산 방식 공식 수정, 데미지 관련 처리

#define LEVEL_DESIGN_AUTO_STAT_CALC     // 레벨에 상관없이 공,방,생이 자동으로 보정하는 연산
#define MATCH_BALLENCE_121217 //자동 매칭으로 인한 밸런스 개편

// 대전방에서 적용되는 f력치 비율 
#define COUNTER_DEF_PERCENT_MATCH   ( 0.5f )    // 대전에서는 착용 장비에 있는 카운터 저항 값에서 50%
#define COUNTER_DEF_MAX_VALUE_MATCH ( 0.5f )    // 대전에서 최대 적용되는 카운터 저항은 50%
#define MP_RECOVERY_PERCENT_MATCH   ( 0.25f )   // 대전에서는 착용 장비에 있는 MP회복력 값에서 25%
#define MP_RECOVERY_MAX_VALUE_MATCH ( 0.25f )   // 대전에서 최대 적용되는 MP회복력은 25%
//

#if defined(__PATH__)
#   define BDMPEG1SETUP_DOWNLOAD   (L"http://patch.grandchase.cdn.netmarble.net/grandchase/Patch/Codec/BDMPEG1SETUP.exe")
#else
#   define BDMPEG1SETUP_DOWNLOAD   (L"http://patch.grandchase.cdn.netmarble.net/grandchase/Patch/Codec/BDMPEG1SETUP.exe")
#endif

//  파티클 멀티쓰레드 로드 할려면 반드시 켜야 합니다.
#define DEVICE_MULTITHREADED

// VP(가상캐쉬) 사용
#define USE_VIP_POINT

//새로운 튜토리얼 가이드 디파인
#define USE_NEWBIE_GUIDE

// 속성 재설정 이벤트 
#define USE_ATTRIBUTE_SELECT_EVENT          // 속성 재설정 이벤트 
#define ATTRIBUTE_SELECT_MAX_COUNT ( 3 )    // 속성 재설정 이벤트 기간이 아닐때 속성 변경 가능한 최대 개수 

// 무기 체인지 레벨제한
#define WEAPONCHANGE_LV             (45)

// 펫 스킬 쉽게 사자
#define USE_SIMPLE_PET_BUY

// 속성 재설정 이벤트 
#define USE_ATTRIBUTE_SELECT_EVENT
#define WORLD_MAP_MISSION_ICON //월드맵에 플레이중인 케릭터 + 공용 미션 아이콘만 표시하기
#define ATTRIBUTE_SELECT_MAX_COUNT ( 3 )    // 속성 재설정 이벤트 기간이 아닐때 속성 변경 가능한 최대 개수 

#if defined( __PATH__ )     //미국 본섭
#define BUY_WEB_PAGE( stm )         (stm<<"http://gamerage.com/Shop/gc/Game/BuyItem/")
#define CHARGE_WEB_PAGE( stm )      (stm<<"http://gamerage.com/recharge/game/goto/gc")
#define GET_CASH_WEB_PAGE( stm )    (stm<<"http://gamerage.com/Recharge/MyPoint/Goto/gc")
#define GACHA_WEB_PAGE( stm )       (stm<<"http://grandchase.gamerage.com/gacha/playgacha.aspx")
#else                       //미국 사내
#define USE_NA_INTERNAL_BILLING
#define BUY_WEB_PAGE( stm )         (stm<<"http://gamerage.com:8000/Shop/gc/Game/BuyItem/")
#define CHARGE_WEB_PAGE( stm )      (stm<<"http://gamerage.com:8000/recharge/game/goto/gc")
#define GET_CASH_WEB_PAGE( stm )    (stm<<"http://gamerage.com:8000/Recharge/MyPoint/Goto/gc")
#define GACHA_WEB_PAGE( stm )       (stm<<"http://grandchase.gamerage.com:8000/gacha/playGacha.aspx")
#endif

#define SELL_DIV_VALUE 3
#define SELL_GP_DIV_VALUE 10

#define GCDeviceFont GCDeviceFontBase

