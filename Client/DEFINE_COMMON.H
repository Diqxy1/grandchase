#pragma once

//------------------------------------------------------------------------------------------------------------------------
// 서비스 국가 정의
// 다음 매크로는 여기가 아닌 프로젝트 속성에서 명시해주도록 하자.
#if defined( FOR_COLORING_SDFSDOIFUSDFKJWERSMDBFKCXV123412341 )
#define NATION_KOREA        // 한국
#define NATION_TAIWAN       // 대만 (오픈베타/정식)
#define NATION_PHILIPPINE   // 필리핀
#define NATION_BRAZIL       // 브라질
#define NATION_USA          // 미국
#define NATION_IDN		    // 인도네시아
#define NATION_THAILAND		// 태국
#define NATION_LATINAMERICA // 남미
#define NATION_EU           // 유럽
#define NETMARBLE_BUY_TEST	// 한국 구매 테섭
#define __PATH__	        // 어디든 본섭이면 Define 되는것
#define NETMARBLE_QA1       // 넷마블 QA 서버 1번
#define NETMARBLE_QA2       // 넷마블 QA 서버 2번
#define NATION_CHINA
#define NETMARBLE_QA1       // 넷마블 QA 서버 1번
#define NETMARBLE_QA2       // 넷마블 QA 서버 2번
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

#define LOGIN_SERVER_IP         "127.0.0.1" //104.152.186.61
#define LOGIN_SERVER_PORT       "1231"

#define MY_MAIN_PARAM           "__firstchase_2023__"

#define HOMEPAGE_EVENT_LINK     (L"https://firstchase.com.br")
#define HOMEPAGE_LINK           (L"https://firstchase.com.br")
#define HOMAPAGE_SHOP_LINK      (L"https://firstchase.com.br/cash")
#define HOMEPAGE_CHARGE_CASH    (L"https://firstchase.com.br/cash")
#define HOMEPAGE_JOIN			(L"https://firstchase.com.br/register")
#define HOMEPAGE_FINDID			(L"https://firstchase.com.br")
#define HOMEPAGE_FINDPASS		(L"https://firstchase.com.br/userarea")
#define	COUPON_EVENT_LINK		(L"https://firstchase.com.br/userarea") 
#define BDMPEG1SETUP_DOWNLOAD   (L"https://dl.bandicam.com/codec/BDMPEG1SETUP.EXE")

#define HOMEPAGE_NEWS_LINK      (L"https://discord.com/channels/932624926639263834/1158861372021555210")
#define HOMEPAGE_GUIDE_LINK     (L"https://discord.com/channels/932624926639263834/1158861372021555210")

#define MAX_PLAYER_NUM              (6)     ///< 최대 플레이 인원
#define NUM_TEMP_PLAYERINFO         (2)     ///< 임시 플레이어 정보 개수
#define MAX_DUNGEON_PLAYER_NUM      (6)     ///< 최대 던전 플레이 인원
#define OLD_MAX_DUNGEON_PLAYER_NUM  (4)     ///< 예전 최대 던전 플레이 인원

#define ARENA_SUPER_TIME            (boost::mpl::multiplies<\
    boost::mpl::int_<GC_FPS_LIMIT>,\
    boost::mpl::int_<5>\
>::next::value)	// 아레나시 5초간 무적

#define ARENA_SUPER_TIME_FATAL      (boost::mpl::multiplies<\
    boost::mpl::int_<GC_FPS_LIMIT>,\
    boost::mpl::int_<5>\
>::next::value)	// 아레나시 2초간 무적

#define ARENA_COOL_TIME_DEATH           GC_FPS_LIMIT * 3
#define ARENA_COOL_TIME_FATAL_DEATH     1
#define ARENA_COOL_TIME_EX              GC_FPS_LIMIT * 5

#define GUILD_MARK_COMBINE_NUM      (3)     // 길드 마크 조합수 : 3 (방패색상+방패종류+문양종류)
#define GUILD_MARK_COLOR_NUM        (11)    // 길드 마크 색상 종류 가지 수
#define GUILD_MARK_SHIELD_NUM       (10)    // 길드 마크 방패 종류 가지 수
#define GUILD_MARK_CREST_NUM        (10)    // 길드 마크 문양 종류 가지 수

//길드개편
#define NEW_GUILD_NOTICE_LIMIT		(70)	// 길드 공지사항 제한 글 수
#define	NEW_GUILD_COMMENT_LIMIT		(70)	// 길드 소개 제한 글 수
#define NEW_GUILD_MY_COMMENT_LIMIT	(50)	// 길드 자기소개 제한 글 수

#	define MAX_CHAT_STR_SIZE       (512) // P2P 채팅에 사용되는 스트링의 최대 길이

//  전 국가가 2.0에서 2.5로 상수값을 쓰기로 함.
#define ABILITY_CORRECT_PLAYER    2.5f

#define MAX_CHAT_STR_BUF_SIZE       (boost::mpl::int_<MAX_CHAT_STR_SIZE>::next::value)

#define EMPTY_DEFAULT_CONSTRUCTOR(c)    c##( void ) {}
#define EMPTY_DEFAULT_DESTRUCTOR(c)     ~##c##( void ) {}
#define EMPTY_VIRTUAL_DESTRUCTOR(c)     virtual ~##c##( void ) {}

#define UNUSED_COPY_CONSTRUCTOR(c) \
    c##( const c##& ) \
        { ASSERT( false && "Do not use copy constructor!" ); }

#define UNUSED_ASSIGNMENT_OPERATOR(c) \
    const c##& operator=( const c##& ) \
        { ASSERT( false && "Do not use assignment operator!" ); return *this; }

#define GC_OLD_SCREEN_WIDTH             (800)
#define GC_OLD_SCREEN_HEIGHT            (600)
#define GC_OLD_SCREEN_FLOAT_WIDTH       static_cast<float>(GC_OLD_SCREEN_WIDTH)
#define GC_OLD_SCREEN_FLOAT_HEIGHT      static_cast<float>(GC_OLD_SCREEN_HEIGHT)

#define GC_SCREEN_WIDTH             (1024)
#define GC_SCREEN_HEIGHT            (768)
#define GC_SCREEN_FLOAT_WIDTH       static_cast<float>(GC_SCREEN_WIDTH)
#define GC_SCREEN_FLOAT_HEIGHT      static_cast<float>(GC_SCREEN_HEIGHT)
#define PARTICLEPOS_X(x)		( ((2*x)/GC_SCREEN_WIDTH) - 1 )
#define PARTICLEPOS_Y(y)		( (GC_SCREEN_HEIGHT -  (2*y))/GC_SCREEN_WIDTH )

#define GC_SCREEN_INTERPOLATION_WIDTH		GC_SCREEN_FLOAT_WIDTH/GC_OLD_SCREEN_WIDTH
#define GC_SCREEN_INTERPOLATION_HEIGHT		GC_SCREEN_FLOAT_HEIGHT/GC_OLD_SCREEN_FLOAT_HEIGHT

#define GC_SCREEN_DIV_WIDTH			(static_cast<float>(GC_SCREEN_WIDTH) / static_cast<float>(GC_OLD_SCREEN_WIDTH))
#define GC_SCREEN_DIV_HEIGHT			(static_cast<float>(GC_SCREEN_HEIGHT) / static_cast<float>(GC_OLD_SCREEN_HEIGHT))

#define GC_SCREEN_DIV_SIZE_INT(x_size) static_cast<int>(static_cast<float>(x_size) * (static_cast<float>(GC_SCREEN_WIDTH) / static_cast<float>(GC_OLD_SCREEN_WIDTH)))

#define GC_SCREEN_OLD_DIV_WIDTH			(static_cast<float>(GC_OLD_SCREEN_WIDTH) / static_cast<float>(GC_SCREEN_WIDTH))
#define GC_SCREEN_OLD_DIV_HEIGHT		(static_cast<float>(GC_OLD_SCREEN_HEIGHT) / static_cast<float>(GC_SCREEN_HEIGHT))

#define GC_SCREEN_DIV_WIDTH_HALF			( 400.f * GC_SCREEN_DIV_WIDTH )
#define GC_SCREEN_DIV_HEIGHT_HALF			( 300.f * GC_SCREEN_DIV_WIDTH )

//(55)
#define GC_FPS_LIMIT                (60)
#define GC_ELAPSED_TIME             (1.0f / GC_FPS_LIMIT)
#ifdef DIRECTINPUT_VERSION
#undef DIRECTINPUT_VERSION
#endif
#define DIRECTINPUT_VERSION 0x0800
#define TIME_OUT_VALUE              (INT_MAX - SHRT_MAX)

//#define MAX_MONSTER                 (30)
//#define SUMMON_MONSTER_INDEX        (15)
//#define DOTA_MONSTER_INDEX          (15)
//#define END_DUNGEON_MONSTER         (SUMMON_MONSTER_INDEX)
//#define END_SUMMON_MONSTER          (DOTA_MONSTER_INDEX)
//#define END_DOTA_MONSTER            (MAX_MONSTER)

#define MAX_ITEM_RECEIVE            (15)    // 서버로 부터 받는 최대 아이템 개수(5의배수로 맞출것)

//#define MAX_OBJECT                  (30)
#define JOB_LEVEL_0                 (0)
#define JOB_LEVEL_1                 (1)
#define JOB_LEVEL_2                 (2)
#define JOB_LEVEL_3                 (3)
#define NUM_JOB_LEVEL               (4)     ///< 최대 전직레벨

#define CASH_BUY_LIMIT_AGE          (7)     ///< 캐쉬 아이템 구입 제한 나이
#define CHAT_VIEW_TIME              (10)    ///< 채팅한거 보여주는 시간?
#define CHAR_SCALE_RATIO            (0.35f)
#define CHECK_NUM_AT_ONCE           (1)

// 반짝하는 프레임 길이
#define FLASH_EFFECT_FRAME          (4)     ///< 맞았을 때 반짝하는 프레임
#define CARTOON_FLASH_EFFECT_FRAME  (8)     ///< 카툰적용시에는 반짝하는게 너무짧아서 안보여서 좀더 길게..

#define GC_COORDI_INVEN_SIZE            (3000)
//------------------------------------------------------------------------------------------------------------------------

// 아이템 아이디들 정의된거.. 아이템 아이디는 공용으로 씁시다!!!!
#define GEM_ITEM_ID                         (888)  ///< GEM 아이템번호
#define CHOCOLATE_ID                        (28346)  ///< GEM 아이템번호
#define CRYSTAL_ITEM_ID                     (1249) ///< 크리스탈 아이템 번호
#define KAZEAZE_MAP1_ID                     (1253) ///< 카제아제 지도 조각1
#define KAZEAZE_MAP2_ID                     (1369) ///< 카제아제 지도 조각2
#define KAZEAZE_MAP3_ID                     (1413) ///< 카제아제 지도 조각3
#define KAZEAZE_MAP4_ID                     (1517) ///< 카제아제 지도 조각4
#define KAZEAZE_MAP_ORIGINAL_ID             (1749) ///< 카제아제 지도 조각 합쳐진것
#define ITEMID_PROTECT_RING                 (1271) ///< 수호의 반지(퀘스트부활) 아이템번호
#define ITEMID_PROTECT_RING_PC              (1751) ///< PC방전용 수호의 반지(퀘스트부활) 아이템번호
#define ITEMID_PROTECT_RING_10              (1936) ///< 10회짜리 수호의 반지
#define ITEMID_PROTECT_RING_QUESTPACKAGE    (5130) ///< 퀘스트 패키지
#define ITEM_ID_HEALBALL                    (1747) ///< 카제아제 힐링볼
#define ITEM_ID_HEALBALL2                   (11129) ///< 카제아제 힐링볼
#define ITEM_ID_HEALBALL_QUESTPACKAGE       (5131) ///< 퀘스트 패키지 카제아제 힐링볼
#define O2MASK_ITEM_ID                      (1412) ///< 산소마스크
#define O2MASK_ITEM_ID_10                   (7916) ///< 산소마스크 10개
#define O2MASK_ITEM_ID_DISCOUNT             (2884) ///< 산소마스크 할인하는거
#define O2MASK_ITEM_ID_SEASON4              (50600)///< 산소마스크
#define GASMASK_ITEM_ID                     (3751) ///< 방독면
#define GASMASK_ITEM_ID_SEASON4             (50601)///< 방독면
#define ITEM_ID_HEART_GP                    (2305) ///< 로미오 반지
#define ITEM_ID_HEART_CASH                  (2304) ///< 줄리엣 반지
#define ITEM_ID_FRIENDRING1                 (2992) ///< 우정의반지1
#define ITEM_ID_FRIENDRING2                 (2993) ///< 우정의반지2
#define ITEM_ID_GAIA_SHIELD                 (1518) ///< 가이아의방패
#define ITEM_ID_GAIA_SHIELD_ONCE            (1523) ///< 가이아의방패 수량
#define ITEM_ID_PET_GHOST                   (3289) ///< 할로윈 이벤트로 만든 유령 펫 ( 펌프킨헤드에게 데미지 30프로 감소 )
#define ITEMID_PROTECT_RING_DISCOUNT        (2885) ///< 수호의반지 할인하는것 (30회)
#define ITEMID_PROTECT_RING_10_DISCOUNT     (2886) ///< 수호의반지 할인하는것 (10회)
#define ITEMID_PROTECT_RING_SINGLE			(3503) ///< 수호의반지 한개짜리
#define ITEMID_EVENT_PEPERO                 (74547) // 이벤트 빼빼로 - 이벤트 끝나면 지우시오
#define ITEM_ID_KAZEAZE_SHIELD              (1748) ///< 카제아제의 방패
#define ITEM_ID_MOONLIGHT_RING              (3226) // 달빛 반지
#define ITEM_ID_OPEN_BLOCKSKILLGROUP        (9960) // 닫한스킬그룹을 열어주는 아이템
#define ITEM_ID_SKILLUNTRAIN_CARD           (9958) // SP를 환원하는 아이템
#define ITEM_ID_SKILLUNTRAIN_CARD2          (10047) // SP를 환원하는 아이템 두번째
#define ITEM_ID_USEFUL_WATCH                (14914) ///< 다기능 시계 액세서리
#define ITEM_ID_SKILLUNTRAIN_CARD3			(18294) // 한국 껌 쿠폰용 스킬sp환원카드
#define ITEM_ID_WC_UNLOCK                   (52549) // 무기 체인지 오픈 열쇠
#define ITEM_ID_TREE_WATER                  (55800) // 나무나무, 물 아이템
#define ITEM_ID_TREE_GOLDTREE               (54773) // 나무나무, 황금 나무 아이템
#define ITEM_ID_TREE_FERTILIZERS            (55801) // 나무나무, 비료 아이템
#define ITEM_ID_YELLOWCARNATION             (8577)
#define ITEM_ID_ANCIENT_STONE_USE           (56032) //고대의 마력석 사용
#define ITEM_ID_ANCIENT_STONE_BUY           (56034) //고대의 마력석 구입(패키지)
#define ITEM_ID_KONAUT_STONE_USE            (56033) //코나우트 마력석 사용
#define ITEM_ID_KONAUT_STONE_BUY            (56035) //코나우트 마력석 구입(패키지)
#define ITEM_ID_HERO_COIN                   (36208) //영웅주화
#define ITEM_ID_HERO_COIN_PIECE             (36711) //영웅주화 조각
#define ITEMID_ASIN_SHAADOW_SET             (89761) // 아신 그림자 복장 세트
#define ITEM_ID_OPEN_ULTIMATE_SKILL         (100057) // 마나4칸및 4단필살기 오픈 열쇠
#define ITEM_GACHA_LOTTERY					(100054) //가챠복권
#define ITEM_CHRISTMAS_PRIMIUM_KEY			(104674)//크리스마스 선물상자
#define ITEM_BLACKSMITH_PERMIT			    (149512)
#define ITEM_COORDI_PERMIT	    		    (165592)
#define ITEMID_LISNAR_PRAY     	    	    (148941)
#define ITEM_BILLBOARD						(154548)
#define ITEM_SLOT_SEAL_OPENER               (166725)
#define ITEM_VIP_BADGE                      (193625)

//Origin Migration
#define ITEMID_UNO_SHADOW_SET				 (155369)
#define ITEMID_VITTAR_POTION_OF_RESURRECTION (187695)
#define ITEMID_HEITAROS_PRAY			     (166876)
#define ITEM_VIP_TEMPORARY_ITEM              (184266)
#define ITEM_BLACKSMITH_DISCOUNT             (190112)
#define ITEM_HENIR_HP_POT                    (190899)
#define ITEM_HENIR_MP_POT                    (190900)
#define ITEM_HENIR_RESS_POT                  (190901)
#define ITEM_HENIR_INVINCIBILITY_POT         (190902)
#define ITEM_NEW_PVP_COIN                    (190903)
#define ITEM_NEW_EVENT_COIN                  (208320)
#define ITEM_NEW_SPECIAL_EVENT_COIN          (208321)

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
#   define REGION_MESSAGEBOX        // 리젼 메시지 박스
#   define CHARACTER_STATS          // 캐릭터 통계 수집
#   define DIRECTX_VERSIONUP_2009_AUG// DirectX 2009 Aug로 버전업
#   define USE_EVENT_BANNER_NEW     // 이벤트 베너 새버전
#   define USE_COORDI_SHOP          // 코디샵
#   define USE_VIP_MENU_BTN         //VIP보상 시스템 버튼 메뉴 UI에 출력
#   define GUILD_RENEW_S4           //길드 시즌4 개편 (길드전)
#   define GUILD_STORE_S4           //길드상점
#   define USE_MIGRATION_BTN        //MIGRATION
#   define EVENT_VIEW_ITEM          //특정시간에만 샵에 아이템 보이게 하기
#   define EVENT_ITEM_CHANGE        //특정시간에만 샵에 있는 아이템을 바꾸게 하기
#   define NEWS_BANNER_OPTIMIZESIZE    //채널뉴스에서 왼쪽에 보이는 이미지 싸이즈를 투명부분 뺀 싸이즈만 렌더링 하도록 함   //512에서 404.0f로 변경 하려면 ServerNews.lua 리소스에서 m_vTextureCoord_y = 0.789로 해야함
#   define USE_PACKAGE_PREVIEW      //패키지 아이템 미리보기
#   define RESTRICT_EQUIP_LEVEL_AT_TREASUREBOX_ITEM    //트레져박스 아이템이 장착 레벨을 제한 시키겠다.
#   define NICKNAME_CHECK_WITH_WSTRING_REAL_LENGTH         //wString의 실제 길이를 사용해서 닉네임 길이를 체크한다.

#   define PVP_SERVER_TEST          //유저들도 PVP_Balance 서버에서 맵이랑 모드 선택이 되도록 하는 디파인
#   define BUY_CASH_ITEM_WITH_POINT        //캐시 아이템을 출석포인트로 구매 가능함
#   define NEW_GPITEM_BUY_PROCESS                      //GP아이템 구입할때 포인트가 부족하면 구입 확인창을 띠우지 않는다.
#   define SERVER_ONLY_NON_COUNT_ITEM                  //NonCount Item을 서버에서만 받는다.
#   define ANGELS_EGG_REFORM                  //NonCount Item을 서버에서만 받는다.
#	define PARTY_SYSTEM						// 파티 시스템
#	define USE_AGIT_SYSTEM					// 아지트 코드
#   define MATCH_BALANCE_2011_8     // 2011년 8월 25일 국내에 업데이트된 캐릭터 벨런스 관련 코드
#   define DUNGEONPLAYSTATE         // 던전 플레이 통계
#   define USE_VIP_NEW_USER_TAB  // vip 보상 이벤트의 신규 유저 보상 메뉴 탭을 쓰고 싶을때
#   define USE_MAGIC_BOX //두근두근 마법 상자, 코우나트 마법 상자 디파인 (첫 접속시 REQ 쏘는 부분 체크를 위해)
#   define USE_ONGAME_INVENTORY     // 게임내 인벤토리 사용
#   define USE_NEW_ITEM_CHECK       // 아이템 획득시 아이템 체크
#   define NEW_CHARACTER_SELECT_BOX_NEW // 신규 유저 캐릭터 선택박스 새 버전
#   define USE_POST_LETTER_SYSTEM // 우편함 사용
#   define USE_LEVEL_LIMIT_GACHA // 레벨제한 가차사용 - 획득한 가차레벨의 동레벨 최종보상 받기
#   define WELCOMEBACK_FRIEND_NEW // 반갑다 친구야 추천인 4인 버전 -  WELCOMEBACK_FRIEND / WELCOMEBACK_FRIEND_NEW 둘다 걸어야 사용 가능
#   define LOAD_ENCHANT_EFFECT_FROM_SCRIPT // 강화 이펙트 정보 스크립트에서 읽기
#   define USE_GOLD_SPEAR // 헬 스피어를 골드 스피어로
#   define USE_LEVEL_LIMIT_GACHA // 레벨제한 가차사용 - 획득한 가차레벨의 동레벨 최종보상 받기

#   define CHAR_ABILITY_STATE // 캐릭터 능력치 통계
#   define USE_MISSION_COMPLETE_EFF // 미션 완료시 지정 이펙트 사용
#   define USE_QA_LOG_SYSTEM          // QA용 로그
#   define LOAD_ENCHANT_EFFECT_FROM_SCRIPT // 강화 이펙트 정보 스크립트에서 읽기
#   define USE_SIMPLE_PET_BUY   // 펫 스킬 쉽게 사자

//  레벨 디자인 관련 디파인 설정
//#define LEVEL_DESIGN_PROMOTION_STAT     // 전직 능력치
//#define LEVEL_DESIGN_STAT_CALC          // 공격력, 방어력 % 계산 방식 공식 수정, 데미지 관련 처리
//#define LEVEL_DESIGN_AUTO_STAT_CALC     // 레벨에 상관없이 공,방,생이 자동으로 보정하는 연산

#   define USE_GP_ITEM_ATTRIBUTE_SELECT // GP 아이템 속성변환 사용
#   define USE_BALANCE_SERVER_MARK  // 서버바에 벨런스 서버 마크 띄워즘
#   define USE_MATCH_SERVER_EGDE    // 매칭 서버 서버 목록에 테두리 넣기
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

/////시즌 4 이후 변경되거나 들어간 디파인들
//#define ITEMBOX_ANI_TOGGLE       // 아이템박스 애니메이션 옵션토글
//#define MISSION_COMPLETE_ITEMLIST_BOX -- 무조건 디파인 해줘어야함
//#define REGION_MESSAGEBOX        // 리젼 메시지 박스

//--변경 Define_common.h 에 정의 되어있으므로 각 국가마다 정의 할필요 없음
//#define MAX_MONSTER                 (30)
//#define SUMMON_MONSTER_INDEX        (15)
//#define MAX_ITEM_RECEIVE            (15)    // 서버로 부터 받는 최대 아이템 개수(5의배수로 맞출것)

#define CRITICALDAMAGE_UP_RATE 1.2f

#define PET_LEVEL_IN_SHOP (30)

#define _NSERVER_LOG    //서버로그 보기싫을때 ㅋㅋ

#define INDIGO_MAX_PLAYER	2

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

#if defined(__PATH__)
#define USE_HTTPPOST
#endif
#if defined(NATION_PHILIPPINE) || defined(NATION_IDN)
#define HTTP_URL "http://Crash-getter.kog.co.kr/log/upload.aspx"
#else
#define HTTP_URL "http://116.120.238.62/log/upload.aspx"
#endif
#if defined(USE_HTTPPOST)
#pragma comment(lib,"libcurl.lib")
#endif

#if !defined(__PATH__)
#define CHECK_ARRAY_INDEX
//#define PERFORMANCE_TEST

#endif

#define ACHEIVE_SYSTEM //업적시스템을 쓰고 싶으면 이 부분을 주석처리 하세요.

//대기방 관련 시간 Define
#define TIME_FOR_IDLE 60000 // 1분(60초) -- 잠수표시 되는데 걸리는 시간
#define TIME_FOR_PREESURE 15000 //15초 -- 다시 재촉할때까지 드는 시간
#define TIME_FOR_HURRYUPDLG 4500 //2.5초 -- 재촉하는 이미지가 떠 있는 시간
#define TIME_FOR_HURRYUPDLG_DISAPPEAR 1500 //1.5초 -- 재촉하는 이미지가 떠 있다 스르르 사라지는 시간
#define TIME_FOR_HURRYUPDLG_COMING 1500 //1.5초 -- 재촉하는 이미지가 떠 있다 스르르 사라지는 시간

#define DEFAULT_PLAYER_SCALE_SIZE 0.35f	// 캐릭터 기본 스케일
#define PARTICLEPOS_Z 0.2f
#define CaseToString(p) case p: return #p
#define CaseToWideString(p) case p: return L#p

#define EnumToString(p) #p
#define EnumToWideString(p) L#p

//옵션 파일 버전
#define GC_OPTION_VERSION 10002

// 몬스터 반사로 줄수 있는 최대 데미지
#define MAX_MONSTER_REFLECTION_DAMAGE  900000.0f

#define MAX_LEVEL (90)

#define EXT ".first"