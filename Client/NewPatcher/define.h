#if defined( _DEFINED_COLORING_asdfqwerdsfasdfzs )  //이건 오로지 디파인들의 컬러링을 위한 것
#define _KOREAN
#define _LEVELUP_BRAZIL
#define _TAIWAN
#define _HONGKONG
#define _USA
#define _PHILIPPINE
#define __OPEN_TEST__
#define __BUY_TEST__
#define _TEST
#define _TAIWAN_TEST
#define _HONGKONG_TEST
#define _BRAZIL_TEST
#define _PHILIPPINE_TEST
#define _USA_TEST
#define _THAILAND
#define _THAILAND_TEST
#define NO_PATCHER_PATCH
#define _KOREA_INTERNAL_
#define _BEAN_FUN_
#define _INDONESIA
#define _LATINAMERICA_TEST
#define _LATINAMERICA
#define GRID_DOWN
#define GRID_DOWN_LOG
#define CHECK_DIRECTX_VERSION
#define CUSTOM_DOWNLOAD_PATH
#define PRE_CLIP_BOARD_READ
#define USE_SPARE_PARAMETER_FORM_COOKIE
#define _EU
#define _EU_TEST
#define _NETMARBLE_QA1_
#define _NETMARBLE_QA2_
#define _CHINA
#define _CHINA_TEST
#endif

#define WM_PATCH_START WM_USER+1
#define WM_DISABLE_BTN WM_USER+100

#define BUILD_VERSION L"NewPatcher Update 2009.05.28 BugReport Version" 
#define LOG_FILE_NAME			"PatchLog.dat"
#define ENCRYP_XOR_NUM	567 
#define ENCRYP_ENT_NUM	123
#define OPEN_PASSWORD "rmfosemcpdltmdbslr100akszlddhkdWkd"

#ifdef OPEN_PATCH_PATH_FILENAME
#undef OPEN_PATCH_PATH_FILENAME
#endif
#define OPEN_PATCH_PATH_FILENAME "PatchPathOpen.dat"

#if defined _KOREAN || _KOREA_INTERNAL_
#ifdef OPEN_PATCH_PATH_FILENAME
#undef OPEN_PATCH_PATH_FILENAME
#endif
#define OPEN_PATCH_PATH_FILENAME "PatchPathKoreaOpen.dat"
#endif

//여기에 각 국가별 OPEN_PATCH_PATH_FILENAME을 정의해 주세요.


#if !defined( _KOREAN ) && !defined( _LEVELUP_BRAZIL ) && !defined( _TAIWAN ) && !defined( _HONGKONG ) && !defined( _TEST ) && !defined( _USA ) && !defined( _THAILAND ) && !defined( _PHILIPPINE ) && !defined( _LATINAMERICA ) && !defined(_EU ) && !defined(_CHINA)
#define _TEST
#endif

#if defined( _KOREAN ) || defined( _LEVELUP_BRAZIL ) || defined( _TAIWAN ) || defined( _HONGKONG ) ||  defined( _USA ) || defined( _THAILAND ) || defined( _PHILIPPINE ) || defined( _INDONESIA ) || defined( _LATINAMERICA ) || defined( _EU ) && !defined(_CHINA)
#undef _TEST
#endif

#if defined( _TEST ) || defined( __BUY_TEST__ ) || defined( _DEBUG )
#define __PATCHER_LOG__
#define NO_PATCHER_PATCH
#endif

//FTP 다운로드 일단 태국만 사용
#if defined(_THAILAND) || defined(_THAILAND_TEST)
#define USE_FTP_DOWNLOAD
#endif


#if defined(_TAIWAN)
#   define PATCH_ORG_SRC       "http://patch.grandchase.com.tw/GrandChase/"        // 대만 패치 경로
#   define PATCH_TEST_SRC      "http://210.208.87.93/cp/GrandChase/"    // 대만사내 테스트 패치 경로
#   define DEF_TEST_SVRIP      "210.208.87.93"                          // 대만사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT    "9400"                                   // 대만사내 테스트 서버 PORT
#   define BACKGROUND_URL           "http://tw.beanfun.com/Grandchase/ingame/patch.htm"
#   define BACKGROUND_PATCHTIME_URL "https://ssl.grandchase.com.tw/maintaining.htm"
#   define NATION_CODE L"[TW]"
#   pragma message( "_TAIWAN macro is defined." )
#	define _BEAN_FUN_
#elif defined(_HONGKONG)
#   define PATCH_ORG_SRC       "http://hk.update.grandchase.gamania.com/cp/GrandChase/"
#   define PATCH_TEST_SRC      "http://210.176.30.187/cp/GrandChase/HongKongTest/"
#   define DEF_TEST_SVRIP      "125.215.234.117"   
#   define DEF_TEST_SVRPORT    "9400"            
#   define BACKGROUND_URL "http://www.gamania.com.hk/grandchase/new_patcher/new_patcher.html"
#   define BACKGROUND_PATCHTIME_URL "http://www.gamania.com.hk/game/grandchase/new_patcher/maintain.html"
#   define NATION_CODE L"[HK]"
#   pragma message( "_HONGKONG macro is defined." )
#	define _BEAN_FUN_
#elif defined(_LEVELUP_BRAZIL)
#   define PATCH_ORG_SRC       "http://fpatch.grandchase.com.br/cp/GrandChase"     // 브라질본섭 패치 경로
//#   define PATCH_TEST_SRC      "http://200.229.51.4/cp/GrandChase/"     // 브라질사내 테스트 패치 경로
#   define PATCH_TEST_SRC      "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로dks
#   define DEF_TEST_SVRIP      "200.229.51.4"                           // 브라질사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT    "9400"                                   // 대만사내 테스트 서버 PORT
#   define BACKGROUND_URL "http://launcher.levelupgames.com.br:80/Launchers/GrandChase/index.html"
#   define BACKGROUND_PATCHTIME_URL "http://launcher.levelupgames.com.br/Launchers/GrandChase/launcher_manutencao.php"
#   define NATION_CODE L"[BR]"
#   pragma message( "_BRAZIL macro is defined." )
#elif defined( _USA )
#   define PATCH_ORG_SRC	"http://gc.cdn.patchgamerage.com/grandchase/"   // 싱가폴 패치 경로
#   define PATCH_TEST_SRC	"http://gc.cdn.patchgamerage.com/grandchase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP	"216.39.108.201"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT	"9400"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://grandchase.gamerage.com:58888/game/launcher.aspx"
#   define BACKGROUND_PATCHTIME_URL "http://grandchase.gamerage.com:58888/game/launcher.aspx"
#   define NATION_CODE L"[USA]"
#elif defined( _PHILIPPINE )
#   define PATCH_ORG_SRC	"http://gcpatch.levelupgames.ph/"   // 필리핀 패치 경로 //"ftp://ftpchase:$(782489*(q289@202.57.118.69/"
#   define PATCH_TEST_SRC	"http://gcpatch.levelupgames.ph/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP	"202.57.118.104"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT	"9511"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://launcher.levelupgames.ph/gc/launcher.html"
#   define BACKGROUND_PATCHTIME_URL "http://launcher.levelupgames.ph/gc/launcher.html"
#   define CRASH_MAIL_TO	"milennium9@kogstudios.com"
#   define CRASH_MAIL_TITLE	"Patcher Crash Report Philippine!!"
#   define NATION_CODE L"[PH]"
#elif defined(_THAILAND)
#   define PATCH_ORG_SRC			"ftp://auto-gc.asiasoft.co.th/GC/"        // 태국 패치 경로
#   define PATCH_TEST_SRC			"ftp://122.155.2.170/"         // 태국 테스트 패치 경로
#   define DEF_TEST_SVRIP	"122.155.2.170"                                  // 태국 테스트 서버 IP
#   define DEF_TEST_SVRPORT	"9400"                                            // 사내 테스트 서버 PORT
#   define BACKGROUND_URL           "http://gc.funbox.co.th/webgc/WebGCBackOffice/patcher/patcher.aspx"
#   define BACKGROUND_PATCHTIME_URL "http://gc.funbox.co.th/webgc/WebGCBackOffice/patcher/patcherMA.aspx"
#   define NATION_CODE L"[TH]"
#	define GCFTP_DEFAULT_DIRECTORY	"/GC/"
#	define WRITE_REGISTERY_VERSION
#   define PATCHREVFILE	"patchrev"
#   pragma message( "_THAILAND macro is defined." )
#   define USE_MANUAL_PATCHSYSTEM
#	define PATCHER_LOG_LEVEL
#elif defined(_THAILAND_TEST)
#   define PATCH_ORG_SRC    "ftp://122.155.2.170/"         // 태국 테스트 패치 경로
#   define PATCH_TEST_SRC   "ftp://122.155.2.170/"         // 태국 테스트 패치 경로
#   define DEF_TEST_SVRIP   "122.155.2.170"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://game3.netmarble.net/chase/popup/for_game/index.asp"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[TH]"
#	define GCFTP_DEFAULT_DIRECTORY	"/GC/"
#	define WRITE_REGISTERY_VERSION
#   define PATCHREVFILE	"patchrev"
#   define USE_MANUAL_PATCHSYSTEM
#	define NO_PATCHER_PATCH
#	define PATCHER_LOG_LEVEL
#   define PATCHER_CURL_LOG
#elif defined(_PHILIPPINE_TEST) //어차피 사내는 Testpatchserver.ini파일을 참조한다. 국가코드만 사용함.
#   define PATCH_ORG_SRC    "http://patch.grandchase.cdn.netmarble.net/grandchase/"  
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            
#   define DEF_TEST_SVRIP   "116.120.238.39"                                 
#   define DEF_TEST_SVRPORT "9400"                                           
#   define BACKGROUND_URL	"http://chase.netmarble.net/popup/for_game/index.asp"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[PH]"
#elif defined(_TAIWAN_TEST)
#   define PATCH_ORG_SRC       "http://patch.grandchase.com.tw/GrandChase/"        // 대만 패치 경로
#   define PATCH_TEST_SRC      "http://210.208.83.94/cp/GrandChase/"    // 대만사내 테스트 패치 경로
#   define DEF_TEST_SVRIP      "210.208.87.94"                          // 대만사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL           "http://tw.beanfun.com/Grandchase/ingame/patch.htm"
#   define BACKGROUND_PATCHTIME_URL "https://ssl.grandchase.com.tw/maintaining.htm"
#   define NATION_CODE L"[TW]"
#elif defined(_HONGKONG_TEST)
#   define PATCH_ORG_SRC    "http://patch.grandchase.cdn.netmarble.net/grandchase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://chase.netmarble.net/popup/for_game/index.asp"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[HK]"
#	define _BEAN_FUN_
#elif defined(_BRAZIL_TEST)
#   define PATCH_ORG_SRC    "http://patch.grandchase.cdn.netmarble.net/grandchase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://chase.netmarble.net/popup/for_game/index.asp"
#   define BACKGROUND_PATCHTIME_URL "http://launcher.levelupgames.com.br/Launchers/GrandChase/launcher_manutencao.php"
#   define NATION_CODE L"[BR]"
#elif defined(_USA_TEST)
#   define PATCH_ORG_SRC    "http://patch.grandchase.cdn.netmarble.net/grandchase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://chase.netmarble.net/popup/for_game/index.asp"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[USA]"
#elif defined(_INDONESIA)
#   if defined(INTERNAL_TEST)
#       define PATCH_ORG_SRC    "http://116.120.238.38/cp/GrandChase/"   // 넷마블 패치 경로
#       define PATCH_TEST_SRC   "http://122.102.49.221/patch/cp/GrandChase/"            // 사내 테스트 패치 경로
#       define DEF_TEST_SVRIP   "116.120.238.38"                                  // 사내 테스트 서버 IP
#       define DEF_TEST_SVRPORT "9501"                                           // 사내 테스트 서버 PORT
#   else
#       define PATCH_ORG_SRC    "http://122.102.49.202/cp/GrandChase/"   // 넷마블 패치 경로
#       define PATCH_TEST_SRC   "http://122.102.49.221/patch/cp/GrandChase/"            // 사내 테스트 패치 경로
#       define DEF_TEST_SVRIP   "122.102.49.221"                                  // 사내 테스트 서버 IP
#       define DEF_TEST_SVRPORT "9401"
#   endif// 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://grandchase.megaxus.com/patcher/"
#   define BACKGROUND_PATCHTIME_URL "http://grandchase.megaxus.com/patcher/"
#   define NATION_CODE L"[ID]"
#elif defined(_LATINAMERICA)
#   define PATCH_ORG_SRC       "http://gcdl.axeso5.com/update/"     // 남미 본섭 패치 경로
#   define PATCH_TEST_SRC      "http://gcdl.axeso5.com/update/"     // 사내 테스트 패치 경로dks
#   define DEF_TEST_SVRIP      "208.39.112.60"                     // 남미 센터 서버 IP
#   define DEF_TEST_SVRPORT    "9501"                              // 남미 센터 서버 PORT
#   define BACKGROUND_URL "http://www.axeso5.com/games/grandchase/launcher"
#   define BACKGROUND_PATCHTIME_URL "http://www.axeso5.com"
#   define NATION_CODE L"[LA]"
#   pragma message( "_LATINAMERICA macro is defined." )
#elif defined(_LATINAMERICA_TEST)
#   define PATCH_ORG_SRC    "http://208.39.112.61/GrandChase"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://208.39.112.61/GrandChase"   // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "208.39.112.61"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9501"                                           // 사내 테스트 서버 PORT
#   define BACKGROUND_URL	"http://www.axeso5.com/games/granchase/launcher.aspx"
#   define BACKGROUND_PATCHTIME_URL "http://www.axeso5.com"
#   define NATION_CODE L"[LA]"
#elif defined( _EU_TEST )
#   define PATCH_ORG_SRC    "http://patch.grandchase.cdn.netmarble.net/grandchase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"
#   define BACKGROUND_URL	"http://grandchase-eu.com:58888/game/launcher.aspx"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[EU]"
#elif defined( _EU )
#   define PATCH_ORG_SRC    "http://eu.gc.cdnetworks.net/GrandChase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"
#   define BACKGROUND_URL	"http://grandchase-eu.com:58888/game/launcher.aspx"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[EU]"
#elif defined( _CHINA ) || defined(_CHINA_TEST)
#   define PATCH_ORG_SRC    "http://eu.gc.cdnetworks.net/GrandChase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"
#   define BACKGROUND_URL	"http://gc.changyou.com/content/launcher/launcher2013.shtml"
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"
#   define NATION_CODE L"[EU]"
#define PATCH_SERVER_INFO_SRC "http://update.changyou.com/yhmx/"
#define PATCH_TEST_SERVER_INFO_SRC "http://update.changyou.com/yhmx/"
#define PATCH_SERVER_FILE_NAME "serverlist.txt"
#define PATCH_SERVER_FILE_NAME_W L"serverlist.txt"
#else // _KOREAN
#   define PATCH_ORG_SRC    "http://patch.grandchase.cdn.netmarble.net/grandchase/"   // 넷마블 패치 경로
#   define PATCH_TEST_SRC   "http://59.25.186.169/cp/GrandChase/"            // 사내 테스트 패치 경로
#   define DEF_TEST_SVRIP   "116.120.238.39"                                  // 사내 테스트 서버 IP
#   define DEF_TEST_SVRPORT "9400"                                           // 사내 테스트 서버 PORT
#ifdef __BUY_TEST__
	#   define BACKGROUND_URL	"http://chase.netmarble.net/popup/for_game/index.asp"
	#   define BACKGROUND_URL2	"http://chase-dev.tooniland.com/popup/for_game/index.asp"
    #   define BACKGROUND_URL3	"http://chase.nate.netmarble.net/popup/for_game/index.asp"
    #   define BACKGROUND_URL4	"http://chase.playnetwork.co.kr/popup/for_game/index.asp"
#else
	#   define BACKGROUND_URL	"http://chase.netmarble.net/popup/for_game/index.asp"
	#   define BACKGROUND_URL2	"http://chase.tooniland.com/popup/for_game/index.asp"
    #   define BACKGROUND_URL3	"http://chase.nate.netmarble.net/popup/for_game/index.asp"
    #   define BACKGROUND_URL4	"http://chase.playnetwork.co.kr/popup/for_game/index.asp"
#endif
#   define BACKGROUND_PATCHTIME_URL "http://www.kog.co.kr/kor/main/main.htm"

#   define NATION_CODE L"[KR]"
#endif

//추가 웹링크
#if defined( _CHINA ) || defined(_CHINA_TEST)
#define BUTTON_WEBLINK_JOIN L"http://member.changyou.com/register/regPhoneSmAction.do?gametype=PE-YHMX"
#define BUTTON_WEBLINK_HOME L"http://gc.changyou.com"
#define BUTTON_WEBLINK_BEGINNER L"http://gc.changyou.com/data/guidelines/sys.shtml"
#define BUTTON_WEBLINK_DOWNLOAD L"http://gc.changyou.com/download/"
#define BUTTON_WEBLINK_FORUM L"http://bbs.changyou.com/gc"
#define BUTTON_WEBLINK_SERVICE L"http://gm.changyou.com/html/category/category-101-262.jsp?101,num=0"
#define BUTTON_WEBLINK_PACKAGE L"http://gc.changyou.com/download/patch.shtml"
#define BUTTON_FLATFORM L"http://app.changyou.com"
#else
#define BUTTON_WEBLINK_JOIN L"http://member.changyou.com/register/regPhoneSmAction.do?gametype=PE-GC"
#define BUTTON_WEBLINK_HOME L"http://gc.changyou.com"
#define BUTTON_WEBLINK_BEGINNER L"http://gc.changyou.com/newbie/index.shtml"
#define BUTTON_WEBLINK_DOWNLOAD L"http://gc.changyou.com/downlaod"
#define BUTTON_WEBLINK_FORUM L"http://bbs.changyou.com/gc"
#define BUTTON_WEBLINK_SERVICE L"http://gm.changyou.com/"
#define BUTTON_WEBLINK_PACKAGE L"http://gc.changyou.com/download/patch.shtml"
#endif

#define PATCHER_EXE_NAME    "grandchase.exe"
#define PATCHER_TEMP_NAME   "grandchase.exe_"
#define VERSION_FILE_NAME	"newver.xml"
#define VERSION_TEMP_NAME	"ver.xml_"

#define GAME_NAME_NORMAL    "NetmarbleGrandChase"       // 보통 서버용
#define GAME_NAME_OPEN_TEST "NetmarbleGrandChaseTest"   // 오픈 테스트 서버용
#define GAME_NAME_BUY_TEST  "NetmarbleGrandChaseBuyTest"// 구매 테스트 서버용

#define PATCH_TIME_CHECK             "PatchTimeCheck.dat"     // 보통 서버용
#define PATCH_IPBASE_CHECK           "PatchIPBaseCheck.dat"     // 보통 서버용
#define PATCH_CRCSTRING_CHECK		 "PatchCRCString.dat"

#if defined( _USA )
#define PATCH_PATH_FILE             "PatchPath2.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPathTest.dat" // 오픈 테스트 서버용
#elif defined( _INDONESIA )
#   if defined(INTERNAL_TEST) 
#       define PATCH_PATH_FILE             "PatchPathIdn.dat"     // 보통 서버용
#       define PATCH_PATH_FILE_OPEN_TEST   "PatchPathTest.dat" // 오픈 테스트 서버용
#   else
#       define PATCH_PATH_FILE             "PatchPath.dat"     // 보통 서버용
#       define PATCH_PATH_FILE_OPEN_TEST   "PatchPathTest.dat" // 오픈 테스트 서버용
#   endif
#elif defined( _TAIWAN )
#define PATCH_PATH_FILE             "patchpathseason3.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "patchpathseason3.dat" // 오픈 테스트 서버용
#elif defined( _PHILIPPINE )
#define PATCH_PATH_FILE             "PatchPathServiceS3.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPathTestNew.dat" // 오픈 테스트 서버용
#elif defined( _LEVELUP_BRAZIL )
#define PATCH_PATH_FILE             "PatchPathNewS3.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPathTestNew.dat" // 오픈 테스트 서버용
#elif defined( _KOREAN )
#if defined( __OPEN_TEST__ )
#define PATCH_PATH_FILE             "PatchPathTest.dat" //한국 오픈 테섭용
#elif defined( __BUY_TEST__ )
#define PATCH_PATH_FILE             "PatchPathBuyTest.dat" // 한국 구매 테섭용
#elif defined( _NETMARBLE_QA1_ )
#define PATCH_PATH_FILE             "PatchPath_QA1.dat"    // 한국 넷마블 QA1번 
#elif defined( _NETMARBLE_QA2_ )
#define PATCH_PATH_FILE             "PatchPath_QA2.dat"    // 한국 넷마블 QA2번 
#else
#define PATCH_PATH_FILE             "PatchPath.dat" // 한국 본섭용
#endif
#elif defined( _LATINAMERICA )
#define PATCH_PATH_FILE             "PatchPathLA.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPathTestLA.dat" // 오픈 테스트 서버용
#elif defined( _THAILAND ) || defined ( _THAILAND_TEST )
#define PATCH_PATH_FILE             "PatchPathNewS3.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPathNewS3.dat" // 오픈 테스트 서버용
#elif defined( _EU )
#define PATCH_PATH_FILE             "PatchPath.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPath.dat" // 오픈 테스트 서버용
#else
#define PATCH_PATH_FILE             "PatchPathNew.dat"     // 보통 서버용
#define PATCH_PATH_FILE_OPEN_TEST   "PatchPathNew.dat" // 오픈 테스트 서버용
#endif


#define REG_KEY             "SOFTWARE\\Netmarble\\"     // 레지스트리 키 경로
#define MAX_RETRY           32                           // 최대 재시도 횟수
#define MAX_DELAY           800                         // 재시도 지연 간격

//#define __OPEN_TEST__ // 오픈 테스트 서버용으로 쓸 때 Enable해야 한다.
#ifdef __OPEN_TEST__
#define GAME_NAME       GAME_NAME_OPEN_TEST
#elif defined( __BUY_TEST__ )
#define GAME_NAME       GAME_NAME_BUY_TEST
#else // !__OPEN_TEST__
#define GAME_NAME       GAME_NAME_NORMAL
//#define PATCH_PATH_FILE PATCH_PATH_FILE_NORM
#endif // __OPEN_TEST__


#if defined( _INDONESIA ) || defined( _THAILAND ) || defined( _THAILAND_TEST ) || defined( _INDONESIA_INTERNAL_TEST ) || defined( _INDONESIA_TEST )
#   define RUN_WITHOUT_PATCH
#endif


//==============================================================================
// 2009.04.21 : tcpl85
// Comment : XTrap 관련 디파인
#if defined( _TEST)
    #if defined(_INDONESIA_TEST) || defined(_INDONESIA_INTERNAL_TEST)
        #define  XTRAP_HTTP_URL_KEY_PATCHER "660970B4C959DBD5B43C6D9844CFE56299599CBEB1E7ADC86BCB5DF016C3D544AFE24C4AD9AD86137BF16D56D51006DC9DE76DC3C133B30D3D263B1DCFDE372A0F7D04245246E456F423430F8900395F8910DE6784C1109110FBCBBE4D4CCC"
    #elif defined(_TAIWAN_TEST) 
        #define XTRAP_HTTP_URL_KEY_PATCHER "660970B42859DBF0C5246D9844CFE86285EDCCD8F8443B4DBBF4F4D2B46AA97FFB9385A26A31977EE542D088ACC8FA5BA52470FBD4AD5E9619E3ED7A0F016C440F7D04245246E455F425430C8B083954880CB5032B840A09A65E9A"
    #elif defined(_TAIWAN) 
        #define XTRAP_HTTP_URL_KEY_PATCHER "660970B42859DBF0C5246D984FCFE8626D0EF941AF039CD1E5EB7110CFC759193231D480581C2AAE4DC67C7032B001CE2F6E3D44643685EDC0A5D947353E73250F7D04245246E417A4610E569557650CE0023EE9C09D90894EEF32A8A18AFA7189C59279B4D6A9BC266FAF6CABF421"
    #elif defined(_LATINAMERICA_TEST)
        #define XTRAP_HTTP_URL_KEY_PATCHER "660970B4965BDB22C5246D9849CFEF62146A0F6CC151254C897F62835E22592D5E70F307717E954D55D1613B10E92216A9916422967E108D4F6A2F733022CBDA0F7D04245246E455F52D430D821E265C0BF881D2E633A426C25177D2FD1CBF5F41B2CC69C7"
    #else
        #define	XTRAP_HTTP_URL_KEY_PATCHER "660970B47859DB1FC5326D9844CFEF629566F60F1D66D8ED22CB9C6A09F6DD3098D8E5C2BAE8E428772F4708B63DA46B90C1D93FE997AAEEA632EF63C391432C0F7D04245246E456F13B590B95072E43B903BDE728DFBFA5"
    #endif
#else
	#if defined( _KOREAN )
		#if defined( __OPEN_TEST__ )       //한국 오픈 테섭용
			#define XTRAP_HTTP_URL_KEY_PATCHER	"660970B47859DB1FC5326D9844CFE862EE9EED228D0C728C1B0DD1B06CC3961064F9BFB4E47E8B67025911D2542914F4A54106ABE1B0DF4D4831A4B12EE12AD20F7D04245246E417A4610E569557650CE0023EE9C09D90894EE431A8BB98A15B7C2A4C4D7B424439BAA8F55B3CBD571E2D718081C3BBC32AAFD2BFE9B1BF9B5EF796CA39CF"
		#elif defined( __BUY_TEST__ )      // 한국 구매 테섭용
			#define XTRAP_HTTP_URL_KEY_PATCHER	""
		#elif defined( _KOREA_INTERNAL_ )  //한국 사내용
			#define XTRAP_HTTP_URL_KEY_PATCHER	"660970B47859DB1FC5326D9844CFEF629566F60F1D66D8ED22CB9C6A09F6DD3098D8E5C2BAE8E428772F4708B63DA46B90C1D93FE997AAEEA632EF63C391432C0F7D04245246E456F13B590B95072E43B903BDE728DFBFA5" // 한국 사내용
		#else //한국 본섭용
            #define XTRAP_HTTP_URL_KEY_PATCHER	"660970B47859DB1FC5326D9844CFE862EE9EED228D0C728C1B0DD1B06CC3961064F9BFB4E47E8B67025911D2542914F4A54106ABE1B0DF4D4831A4B12EE12AD20F7D04245246E417A4610E569557650CE0023EE9C09D90894EE431A8BB98A15B7C2A4C4D7B424439BAA8F55B3CBD571E2D718081C3BBC32AAFD2BFE9B1BF9B5E"
		#endif
    #elif defined( _INDONESIA )
		#if defined(_INDONESIA_TEST) || defined(_INDONESIA_INTERNAL_TEST)
			#define  XTRAP_HTTP_URL_KEY_PATCHER "660970B4C959DBD5B43C6D9844CFE56299599CBEB1E7ADC86BCB5DF016C3D544AFE24C4AD9AD86137BF16D56D51006DC9DE76DC3C133B30D3D263B1DCFDE372A0F7D04245246E456F423430F8900395F8910DE6784C1109110FBCBBE4D4CCC"
		#else
			#define  XTRAP_HTTP_URL_KEY_PATCHER "660970B4C959DBD5B43C6D9849CFE5624FD2C50F803ADF935F8DD959DD09A2C81B8F30BC0D4ECE7C1D4B4E5C42969E21741EC4F69F304B198EE16240E98408180F7D04245246E456F727430F8B023959E43479E3E59E44C16B7608083119"
		#endif	
    #elif defined(_TAIWAN)
        #if defined(_TAIWAN_TEST) 
            #define XTRAP_HTTP_URL_KEY_PATCHER "660970B42859DBF0C5246D9844CFE86285EDCCD8F8443B4DBBF4F4D2B46AA97FFB9385A26A31977EE542D088ACC8FA5BA52470FBD4AD5E9619E3ED7A0F016C440F7D04245246E455F425430C8B083954880CB5032B840A09A65E9A"
        #else
            //#define XTRAP_HTTP_URL_KEY_PATCHER "660970B42859DBF0C5246D9844CFE86285EDCCD8F8443B4DBBF4F4D2B46AA97FFB9385A26A31977EE542D088ACC8FA5BA52470FBD4AD5E9619E3ED7A0F016C440F7D04245246E417A4610E569557650CE0023EE9C09D90894EEF32A8A18AFA7189C59279B4D6A9BC266FAF6CABF4214D"
            #define XTRAP_HTTP_URL_KEY_PATCHER "660970B42859DBF0C5246D984FCFE8626D0EF941AF039CD1E5EB7110CFC759193231D480581C2AAE4DC67C7032B001CE2F6E3D44643685EDC0A5D947353E73250F7D04245246E417A4610E569557650CE0023EE9C09D90894EEF32A8A18AFA7189C59279B4D6A9BC266FAF6CABF421" //22일 JoytoKey
        #endif
    #elif defined(_LATINAMERICA)
        #define XTRAP_HTTP_URL_KEY_PATCHER "660970B4965BDB22C5246D9849CFEF62146A0F6CC151254C897F62835E22592D5E70F307717E954D55D1613B10E92216A9916422967E108D4F6A2F733022CBDA0F7D04245246E400A6710110DA48721EF23805E53A8D401AA55983DA"
    #elif defined (_LEVELUP_BRAZIL)
        #define XTRAP_HTTP_URL_KEY_PATCHER "660970B4485BDB86C5556D9844CFEF62963E1A0B24BF43985542242973DCE2AAF242150F6F5D1B83A9E928767FE9691C38F44C63AAEDE6EF1C176F8AD3904EF70F7D04245246E41FB1670C4E9557650C03609EEBF61B756298FD5E7377DF"
    #else
		#define	XTRAP_HTTP_URL_KEY_PATCHER	""
	#endif
#endif

#if defined(_CHINA) || defined(_CHINA_TEST)
#define GRANDCHASE_TITLE_NAME L"永恒冒险"
#else
#define GRANDCHASE_TITLE_NAME L"GrandChase"
#endif

#define XTRAP_DOWNLOAD_TIME_OUT	 (60)
//=======================================================================================


//한국의 웹페이지가 들어갈 때 까지 웹페이지가 없다.
// #if defined( _KOREAN )
// #define _NO_WEB_
// #endif

#if defined( _KOREAN ) || defined( __OPEN_TEST__ ) || defined( _KOREA_INTERNAL_ )
#define GRID_DOWN
//#define GRID_DOWN_LOG
#define CHECK_DIRECTX_VERSION
#define CUSTOM_DOWNLOAD_PATH
#define USE_SPARE_PARAMETER_FORM_COOKIE     //쿠키에서 읽은 스페어 파라미터에 지정한 값이 있으면 테스트계로 접속 하도록 함
#define PRE_CLIP_BOARD_READ
#endif

#if defined( _CHINA ) || defined( _CHINA_TEST ) 
#define __VERSION_INFO_DOWNLOAD__
#endif

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!잘못하면 사고남!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define NO_PATCHER_PATCH
//#if defined( NO_PATCHER_PATCH )
// #   pragma message( "No Patcher Patch" )
// #endif
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!잘못하면 사고남!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define OKBOXA( msg, title ) \
{\
    std::stringstream strstm;\
    strstm<<msg;\
    MessageBoxA( NULL, strstm.str().c_str() , title , MB_OK );\
}

#if defined( __PATCHER_LOG__ )
#define ASSERT_MBOX( msg ) OKBOXA( msg, __FUNCTION__ )
#else
#define ASSERT_MBOX( msg ) 
#endif

#ifdef _DEBUG
#pragma comment( lib, "d3dx9d" )
#else
#pragma comment( lib, "d3dx9" )
#endif// _DEBUG
#pragma comment( lib, "d3d9" )
#if defined( CHECK_DIRECTX_VERSION )
#pragma comment( lib, "dxerr" )
#else
#pragma comment( lib, "dxerr9" )
#endif
#define D3D_H           <d3d9.h>
#define D3DX_H          <d3dx9.h>
#define D3DTYPES_H      <d3d9types.h>
#define D3DMATH_H       <d3dx9math.h>
#define LPDIRECT3DTEXTURE       LPDIRECT3DTEXTURE9
#define LPDIRECT3DDEVICE        LPDIRECT3DDEVICE9
#define LPDIRECT3DVERTEXBUFFER  LPDIRECT3DVERTEXBUFFER9
#define LPDIRECT3DINDEXBUFFER   LPDIRECT3DINDEXBUFFER9
#define D3DADAPTER_IDENTIFIER   D3DADAPTER_IDENTIFIER9
#define D3DCAPS                 D3DCAPS9
#define D3DCLIPSTATUS           D3DCLIPSTATUS9
#define D3DLIGHT                D3DLIGHT9
#define D3DMATERIAL             D3DMATERIAL9
#define D3DVIEWPORT             D3DVIEWPORT9
#define LPDIRECT3DDEVICE        LPDIRECT3DDEVICE9
#define LPDIRECT3D              LPDIRECT3D9
#define BUFFER_LOCK(buff,off,size,data,flag)  buff->Lock( off, size, (void**)data, flag )
#define _SetFVF                 SetFVF
#define IDirect3DDevice         IDirect3DDevice9
#define D3DCAPS                 D3DCAPS9
#define D3DADAPTER_IDENTIFIER   D3DADAPTER_IDENTIFIER9
#define LPDIRECT3DSURFACE       LPDIRECT3DSURFACE9
#define LPDIRECT3DTEXTURE       LPDIRECT3DTEXTURE9
#define Direct3DCreate          Direct3DCreate9
#define IDirect3D               IDirect3D9
#define IDirect3DSurface        IDirect3DSurface9

#ifdef _TEST
#   define PATCHER_CURL_LOG
#endif