#include "stdafx.h"
#include "MyD3D.h"
#include <shellapi.h>
#include "VersionDef.h"
#include "ClientErr.h"
#include "KGCWorldMapManager.h"
#include "KGCRoomManager.h"
#include "EveryTriggerManager.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../gcui/GCOptionMusic.h"
#include "./curl/curl.h"
#include "DamageManager.h"
#include "GCClientErrorManager.h"
#include <winsock.h>
#include <Ipexport.h>
#include <Icmpapi.h>
#include "shellapi.h"
#include "gcui/GCRoomUserSlotS6.h"
#include <KncP2PLib/KncP2P.h>
#include <GCUtil/GCRandomObj.h>
#include "KSingleton.h"

KGCGlobal g_kGlobalValue;
_Mail KGCGlobal::ms_kMail;
KGCGlobal::KGCGlobal()
: m_bRenderSquarePeopleData(false)
, m_bObserverMode( false )
, m_bHellMode( false )
, m_bChatEventMessageTemp( false )
, m_iChatEventCount( 0 )
, m_cSavedCharType(0)
, m_bTutorialReselect ( false )
, m_bSendServerInfoNot ( false )
, m_bIsGetWebCashOnStateMyInfoFirstInit( false )
{
    m_kUserInfo.Init();
    m_kUserInfo.bLive = true;
    m_kUserInfo.cCharIndex = -1;

	m_kLoginInfo.bNewUser = false;
    m_kLoginInfo.bGuideAzit = true;

    m_fTime = 0.0f;

    MEMORYSTATUS memState;
    memState.dwLength = sizeof (memState);
    GlobalMemoryStatus(&memState);
    m_bHighMem = true;
    m_iTestValue = 1;
    m_strDetectedHackTool = L"";
    m_bRenderPosition = false;
    m_bRenderBoundBox = false;
    m_bFrameLock = false;
    m_bShowControls = false;
    m_kLoginInfo.bExpAccount = false;
	m_kLoginInfo.nFunBoxBonus = 0;

    ServerInfo.dwServerType = ST_FORCE_DWORD;

    m_bCharInfoIsNull = false;
    m_bSkillMotionIDErrorOccur = false;
    m_bMonsterSuicide = false;

    m_iInvenCapacity = DEFAULT_INVENTORY_NUM;

    //일단 수행한 것으로 간주하고 안했으면 패킷을 받아서 업데이트 해 줄꺼임.
    m_bNewUser_Tutorial = true;
    m_bNewUser_Quest = true;

    m_vecBlockUserName.clear();
    m_vecCurrentStateUserInfo.clear();

    m_iCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_NORMAL;
    m_bSendErrReport = false;

    m_dwCash = 0;
    m_dwVirtualCash = 0;
	m_dwGachaCoin = 0;
    m_MonsterMake = 0;
    m_kCashbackRatioInfo.m_mapCashbackDefaultRatio.clear();
    m_kCashbackRatioInfo.m_mapCashbackEventRatio.clear();
    m_nCashbackRatioFirstBuying = 0;
    m_dwChinaWebCash = -1;

    m_stmHackComment.str(L"");

    m_iGCPoint = 0;
    m_iPlayTime = 0;
    m_bExistCalendar = false;
    m_iReportAvailableCount = 0;
    m_iReportedCount = 0;
    m_bReportInGame = false;

    m_bRisingSunOrMoonEvent = false;

    m_bLoadingTimeSend = false;
    m_iLoadingTime = 0;

    m_bLoginServerConTimeSend = false;
    m_dwElapsedSec = 0;

    m_bAIEnable = true;
    m_bRenderMonster = true;
    m_bRenderFootHold = false;
    m_bRenderMonsterStatus = false;

    m_bIsOnGameClose = false;

	m_bActiveApp = true;
	m_bUseCrack = FALSE;

    m_mapMonsterHuntInfo.clear();
    m_mapMonsterCrusaderInfo.clear();
    m_vecChampionInfo.clear();
    m_mapErrorMotion.clear();
    m_mapGuildLevelTable.clear();
	m_bExpAccount = false; //exp_id
	m_tmServerTime = CTime::GetCurrentTime();
	m_tmServerSyncTime = 0;
	m_iUseHellModeBtn = 0;

	m_iMaxLevel = MAX_LEVEL; // 최대 레벨 초기화

	m_bLoginFirst = true;
    m_bShowItemInfoBox = false;

    m_bBoxAlphaToggle = false;
    m_bCriticalToggle = false;
    m_bCriticalOffToggle = false;

    m_bComboEffectShow = true;

//  사내는 OFF가 Default
#if !defined( __PATH__ )    
    m_bSpecialFaceShow = false;
#else 
    m_bSpecialFaceShow = true;
#endif 

#if defined ( __PATH__ )
#    define    MD5_PASSWORD_LOGIN
#endif

#if !defined( __PATH__) && defined( NATION_KOREA )
	m_bLoginFirst = true;
	//#define LOGIN_FIRST
#endif

#if defined(_TAIWAN_GASH_TEST_)
	m_bLoginFirst = true;
#elif defined(_HONGKONG_GASH_TEST_)
	m_bLoginFirst = true;
#endif

    m_bIsRecommendEvent = false;
	m_bIsRainbowEvent = false;
	m_bJackPot = false;
	m_iRainbowEventID = 0;

    m_bUnlock = false;
    m_bDungeonFreePass = false;

	m_bIncompletePatch = false;

	m_setGiftsEvent.clear();

	m_mapModifiedFile.clear();
	m_vecDeleteFileLIst.clear();

	m_mapSubscriptionEvent.clear();

	m_ivurcashper = 0;
	m_mapNGuildUserInfo.clear();
	m_mapNGuildChangeMemberGrade.clear();
	m_setAddGuildMemberData.clear();
	m_setDelGuildMemberData.clear();

    m_bMUHANBonusPoint = false;
	m_bMUHANLife = false;

    m_vecEventMonster.clear();
    m_bMonsterAction = false;
    m_iMonsterActionIndex = 0;

	m_bUseLoadCheckInServer = false;

    m_dwJoinRoomLimitPing = -1;

    m_fFakeCCU = -1.f;

    m_bUseOldReplay = false;

    m_mapMonLevelTable.clear();
    m_mapMonWeightTable.clear();
    m_vecBossHPRate.clear();
    m_setBossHPDisable.clear();

    m_vecNormalMonHPRate.clear();
    m_setNormalMonHPDisable.clear();

    m_iAdjustMaxLevel = 11;
    m_iAdjustMinLevel = -5;
    m_fDefaultReward = 1.0f;
    m_mapAdjustRatio.clear();

    m_bEventCreateFirst = true;
    m_bEventCreateEnable = false;

    m_iSelectFontIndex = -1;

    m_dwConnetionTime = 0;

    m_bServerReconnection = false;
    m_bUIInfo = false;

    m_iCurDotaMapID = 1;

    m_mapChampionAbility.clear();

    //DllBlackList 관련
    m_bHackCheck = false;
    m_bFindMainFile = false;
	m_ulGanadara = rand()%1000;
    
    m_setBlackDllList.clear();
	m_strCheckingBlackDll.clear();

    m_vecExtendSHAList.clear();


	m_ulRandomCount = m_ulGanadara;
	//파티
    m_vecPartyPeople.clear();

    m_nPlayerCount = 1;
	m_biUniqueKey = 0; // Init.
    m_nUniqueKey = 0; // Init.

    // 호위 정보 초기화
	m_kEscortInfo.m_dwMonsterID = -1; // -1이면 소환해야 할 몹 없음
    m_kEscortInfo.m_nMonsterHealth = 0;
    m_kEscortInfo.m_nTakeDamage = 0;
    m_kEscortInfo.m_nInvincibleTime = 0;
    m_kEscortInfo.m_PotionID = -1;
    m_kEscortInfo.m_vecStrNum.clear();
    m_kEscortInfo.m_bIsActiveEscort = false; // 게임 시작시 이벤트 기간인지
    m_kEscortInfo.m_bIsSummoned = false;
    
	
    m_bShutDown = false;
    m_mapEventMonsterAbility.clear();

	m_bDamageHack = false;
	m_fBefore = 0.0f;
	m_fAter = 0.0f;

#if defined (USE_JOYSTICK)
    InitMatchingKey();
#endif 

#if !defined( __PATH__ )
    m_fDamageRaito = 1.0f;
#endif

    m_mapEnchantEffect.clear();

    SetSelectMotionID();

    m_prHybridSpecialUseCount = std::make_pair( 0, 0 );

    m_iGetCharType = -1;

    m_bJPGScreenShot = true;
    m_mapSealedCharInfo.clear();
    m_iShutDownType = 0;

    bItemCrafting = false;

	m_mapGachaSuccessList.clear();
	m_mapGachaFailedList.clear();
    m_vecDisableUIBtnList.clear();

    m_iSendTimeForPacket = -1;
    m_vecSendPacket.clear();

    m_iSendTimeForPacket = -1;
    m_vecSendPacket.clear();
    m_bNoCameraZoom = false;

    m_bToggleRecoverHP = false;
    m_bToggleRecoverMP = false;

    m_iLocalPlayerIndex = -1;

    m_pTempTex = NULL;
    bRenderTempTexture = false;
    vTempTexRenderPos = D3DXVECTOR2(0.0f, 0.0f);

    m_bPrintTG = false;
    m_dwCurrentServerUID = -1;
    m_strPatcherSelIP = LOGIN_SERVER_IP;
    m_strPatcherSelPort = LOGIN_SERVER_PORT;
    m_bPatcherSelectServer = false;

	m_strUser = "";
	m_strPass = "";

    m_bSmartPanelMailListEnable = false;
    m_vecSmartPanelMailList.clear();

    m_iPrevRemainVitality = 0;
    m_mapVitalityDungeonInfo.clear();
	
	m_mapMoveModelData.clear();
    m_vecOpenedEventDungeonList.clear();
    m_vecCashQuantityList.clear();
    m_vecVIPItemList.clear();
    m_iPreContinent = -1;
}

KGCGlobal::~KGCGlobal()
{
	m_mapSubscriptionEvent.clear();
    m_mapMonsterHuntInfo.clear();
    m_mapMonsterCrusaderInfo.clear();
    m_vecChampionInfo.clear();
    m_mapMonLevelTable.clear();
    m_mapMonWeightTable.clear();
    m_vecBossHPRate.clear();
    m_setBossHPDisable.clear();
    m_mapPetTemplate.clear();
    m_mapEventMonsterAbility.clear();
    m_vecCashQuantityList.clear();
    m_vecVIPItemList.clear();
}

int KGCGlobal::TreatCharTypeID(int ExtraCharNum) {
    if (ExtraCharNum > 59)
        return ExtraCharNum - 60;
    if (ExtraCharNum > 39)
        return ExtraCharNum - 40;
    if (ExtraCharNum > 19)
        return ExtraCharNum - 20;
    return ExtraCharNum;
}

void KGCGlobal::SetEscortInfo( KEVENT_ESCORT_INFO_NOT& kEscortInfo_ )
{

    m_mapEscortInfo.clear();
    KEVENT_ESCORT_INFO_NOT::iterator mit = kEscortInfo_.begin();

    for ( ; mit!=kEscortInfo_.end() ; ++mit )
    {
        KGlobalEscortInfo tmpInfo;
        tmpInfo.m_dwMonsterID = mit->second.m_dwMonsterID;
        tmpInfo.m_nMonsterHealth = mit->second.m_nMonsterHealth;
        tmpInfo.m_nTakeDamage = mit->second.m_nTakeDamage;
        tmpInfo.m_nInvincibleTime = mit->second.m_nInvincibleTime;
        tmpInfo.m_PotionID = mit->second.m_PotionID;
        tmpInfo.m_vecStrNum.insert( tmpInfo.m_vecStrNum.end(), mit->second.m_vecStrNum.begin(), mit->second.m_vecStrNum.end() );
        g_kGlobalValue.m_mapEscortInfo.insert( std::map<DWORD,KGlobalEscortInfo>::value_type(mit->first , tmpInfo) );
    }
}

void KGCGlobal::InitPetTable()
{
    m_mapPetTemplate.clear();
}

int KGCGlobal::GetMaxCharLevel()
{
    int iMaxLevel = 0;
    int iCharLevel = 0;
    for( int i = 0; i < (int)m_kUserInfo.vecCharInfo.size(); i++ )
    {
        iCharLevel = (int)Exp_2_Level( m_kUserInfo.vecCharInfo[i].biExp );
        if ( iMaxLevel < iCharLevel )
            iMaxLevel = iCharLevel;
    }
    return iMaxLevel;
}

void KGCGlobal::SetCashQuantityList() {
    KLuaManager luaMgr;
    if ( GCFUNC::LoadLuaScript( luaMgr, "InitCashQuantity.stg" ) )
    {
        luaMgr.BeginTable( "CashQuantityList" );
        {
            for (int i = 1; ; ++i) {
                GCITEMID nItemID;
                if ( !SUCCEEDED( luaMgr.GetValue( i, nItemID ) ) ) 
                    break;
                m_vecCashQuantityList.push_back( nItemID );
            }
        }
        luaMgr.EndTable();
    }
}

void KGCGlobal::SetVIPItemList() {
    KLuaManager luaMgr;
    if ( GCFUNC::LoadLuaScript( luaMgr, "InitVIPItemList.lua" ) )
    {
        luaMgr.BeginTable( "VIPItemList" );
        {
            for ( int i = 1; ; ++i ) {
                GCITEMID nItemID;
                if ( !SUCCEEDED( luaMgr.GetValue( i, nItemID ) ) )
                    break;
                m_vecVIPItemList.push_back( nItemID );
            }
        }
        luaMgr.EndTable();
    }
}

bool KGCGlobal::IsItemInVIPList(GCITEMID iItemID) 
{
    for ( auto element : m_vecVIPItemList )
        if( element == iItemID * 10 )
            return true;
    return false;
}

// 출석부 버튼안에 누를 수 있는게 있으면 버튼위에 ( ! ) 띄워준다!
bool KGCGlobal::CheckCalendarAlert()
{
    bool bAlert = false;

    // 오늘 이전의 데이터를 보고 받은 도장을 표시해 준다
    std::map<char, std::vector<KDailyInfo> >::iterator mit;

    // 한주 한주 살펴본다
    for( mit = m_mapCalendar.begin(); mit != m_mapCalendar.end(); ++mit )
    {
        int iWeekStampCount = 0;

        std::vector<KDailyInfo> vecWeekInfo = mit->second;

        for( int i = 0; i < (int)vecWeekInfo.size(); ++i )
        {
            KDailyInfo kDaylyInfo = vecWeekInfo[i];

            // 네모라서 출석체크할 수 있는가?
            if( kDaylyInfo.m_cSealType == ST_SQU )
                bAlert = true;

            if( kDaylyInfo.m_kDate == m_kToday &&   // 오늘이고.. 
                kDaylyInfo.m_cSealType == ST_SQU )  // 네모이면..
                bAlert = true;

            // 세모나 체크를 했으면 카운트를 해서 7개이면..
            // 주 개근을 찍을 수 있도록 한다
            if( ( !(kDaylyInfo.m_kDate == g_kGlobalValue.m_kToday ) &&
                ( ( kDaylyInfo.m_cSealType & ST_TRI ) && ( ( kDaylyInfo.m_cSealType & ST_STA ) == 0 ) ) ||
                ( ( kDaylyInfo.m_cSealType & ST_CIR ) && ( ( kDaylyInfo.m_cSealType & ST_STA ) == 0 ) ) ) )
            {
                ++iWeekStampCount;
            }
        }

        // 도장을 빠짐없이 일곱개 모았다면!!
        // 주개근 버튼
        if( iWeekStampCount == 7 )
        {
            bAlert = true;
        }
    }

    if( m_vecReservedReward.size() > 0 )
        bAlert = true;

    return bAlert;
}

void KGCGlobal::SetCalendarInfo( KCalendarInfo kCalendarInfo )
{
    m_iPlayTime = kCalendarInfo.m_nAccTime;
    m_kToday = kCalendarInfo.m_kToday;
    m_mapCalendar = kCalendarInfo.m_mapCalendar;
    m_iGCPoint = kCalendarInfo.m_nPoint;

    m_kLastDayOfLastMonth = kCalendarInfo.m_kLastDayOfLastMonth;
    m_kFirstDay = kCalendarInfo.m_kFirstDay;
    m_kLastDay = kCalendarInfo.m_kLastDay;

    m_dwAccTimeNot = timeGetTime();

    m_iAttendTime = kCalendarInfo.m_nAttendTime;
}

void CashRechargeWebClose()
{
	g_kGlobalValue.UpdateCashData();
}
void GachaWebClose()
{
	g_MyD3D->m_KGCOption.SetMusic();
	g_pkUIScene->Stop( false );
	int iTemp;
	KP2P::GetInstance()->Send_InventoryInfoReq( "Server" );
	g_MyD3D->WaitForServerAck( iTemp, 0, 3000, 2 );
}

void CashRechargeWebOpen()
{
#if defined ( NATION_USA )
    // 북미는 웹 캐시 충전 페이지를 외부 브라우저에서 띄운다. 2012. 07. 11 lucidust
    // KBrowserWrapper에 Navigate()에 타겟 프레임 인자가 추가되었다
    RECT rc =
    {
        0, 0, 0, 0
    };

    if ( false == g_pBrowser->Create( rc, g_MyD3D->m_hWnd ) ) return;

    std::ostringstream strmWebPage;
    std::ostringstream strIGSParam;

    CHARGE_WEB_PAGE( strmWebPage );

    strIGSParam<<"id="<<g_kGlobalValue.m_kLoginInfo.strID<<"&"
        <<"key="<<g_kGlobalValue.m_kLoginInfo.strPass<<"&"
        <<"level="<<g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel<<"\0";

    g_pBrowser->Navigate( strmWebPage.str().c_str(), strIGSParam.str().c_str(), L"_blank" );

    g_pBrowser->DestroyWin();
    g_pkUIScene->SetAllStop(false);

    // 외부 브라우저가 닫히는 시점을 알 수 없으므로 캐시 잔액정보 조회를 하지 않음
    //g_pBrowser->SetEndFunction( CashRechargeWebClose );

    // 외부 브라우저를 실행한 뒤 GC 창을 최소화 시킨다
    ::SendMessage( g_MyD3D->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
#else
	static const LONG lBoxSizeX = 660;
	static const LONG lBoxSizeY = 550;
	RECT rc =
	{
		W_Width / 2 - lBoxSizeX / 2,
			W_Height / 2 - lBoxSizeY / 2,
			W_Width / 2 - lBoxSizeX / 2 + lBoxSizeX,
			W_Height / 2 - lBoxSizeY / 2 + lBoxSizeY,
	};

	if ( false == g_pBrowser->Create( rc, g_MyD3D->m_hWnd ) ) return;

	std::ostringstream strmWebPage;
	std::ostringstream strIGSParam;

	CHARGE_WEB_PAGE( strmWebPage );

    strIGSParam<<"id="<<g_kGlobalValue.m_kLoginInfo.strID<<"&"
        <<"key="<<g_kGlobalValue.m_kLoginInfo.strPass<<"&"
        <<"level="<<g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel<<"\0";

    g_pBrowser->Navigate( strmWebPage.str().c_str(), strIGSParam.str().c_str(), L"" );
    g_pBrowser->SetEndFunction( CashRechargeWebClose );
#endif
}

void PopUpWebPage( const WCHAR* strURL_ )
{
    ::PostMessage( g_MyD3D->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );	
    ShellExecute(NULL, L"open", strURL_, NULL, NULL, SW_SHOWNORMAL);
}

#ifdef _DEBUG
void KGCGlobal::LoadDevLua()
{
    KLuaManager	luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" ) == false )
        return;

    LUA_BEGIN_TABLE("DevTable", return )
    {
        for( int i = 1; ; ++i )
        {
            if( i == 11 )
                break;

            LUA_GET_VALUE_DEF( i, m_fDevValue[i-1], 0.0f );
        }
    }
    LUA_END_TABLE( return )
}
#endif

void KGCGlobal::LoadUseHellModeBtn()
{
    m_iUseHellModeBtn = 0;

	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "UseHellModeBtn.lua") == false)
		return;

	LUA_BEGIN_TABLE("UseTable", return )
	{
		LUA_GET_VALUE_DEF( 1, m_iUseHellModeBtn, 0 );
	}
	LUA_END_TABLE( return )
}

bool SServerInfo::CheckServerType( ServerType eServerType_ )
{
    if( SiKP2P()->IsConnectedToGameServer() && (dwServerType & eServerType_) != 0 )
    {
        return true;
    }
    return false;
}

std::wstring KGCGlobal::GetGameVersionW()
{
    std::wstring strBuildTime;
    KncUtil::GetTimeStampByNameW( strBuildTime );
    TCHAR strGameVersion[MAX_PATH] ={0,};

    wsprintfW( strGameVersion, L"Origin v%s", strBuildTime.c_str() );

    std::wstring strReturn( strGameVersion );

    return strReturn;
}

std::string KGCGlobal::GetGameVersionA()
{
    return GCUTIL_STR::GCStrWideToChar( GetGameVersionW().c_str() );
}

void KGCGlobal::SendLoadingTime()
{
    if( KP2P::GetInstance()->IsConnectedToGameServer() == false )
        return;

    if( m_bLoadingTimeSend == false )
    {
        m_iLoadingTime /= 1000;        
        if( m_iLoadingTime < 30 )
        {
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_LOADINGTIME_30 );
        }
        else if( m_iLoadingTime >= 30 && m_iLoadingTime < 40 )
        {

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_LOADINGTIME_30_40 );
        }
        else if( m_iLoadingTime >= 40 && m_iLoadingTime < 50 )
        {

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_LOADINGTIME_40_50 );
        }
        else if( m_iLoadingTime >= 50 && m_iLoadingTime < 60 )
        {

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_LOADINGTIME_50_60 );
        }
        else if( m_iLoadingTime >= 60 && m_iLoadingTime < 120 )
        {

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_LOADINGTIME_60_120 );
        }
        else
        {

            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_LOADINGTIME_120 );
        }        
        
        m_bLoadingTimeSend = true;
    }    
}

void KGCGlobal::SendLoginServerConTime()
{
    if( !KP2P::GetInstance()->IsConnectedToGameServer() ) return;

    if( m_bLoginServerConTimeSend ) return;

    m_bLoginServerConTimeSend = true;

    if( m_dwElapsedSec < 3 )
    {
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_CON_TO_LOGIN_3 );
    }
    else if( m_dwElapsedSec >= 3 && m_dwElapsedSec < 6 )
    {
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_CON_TO_LOGIN_3_6 );
    }
    else if( m_dwElapsedSec >= 6 && m_dwElapsedSec < 10 )
    {
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_CON_TO_LOGIN_6_10 );
    }
    else
    {
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_CON_TO_LOGIN_10 );
    }
}

void KGCGlobal::SetIndigoRank( std::vector<KIndigoRankInfo>& vecRank )
{
	m_vecIndigoRank = vecRank;
}

bool KGCGlobal::MakeErrorMotionToFile( std::string strFileName, bool bSendMail /*= false*/ )
{
    if( m_mapErrorMotion.size() == 0 )
        return false;

    std::fstream kOutFile;
    kOutFile.open( strFileName.c_str(), std::ios::out );
    for( std::map< UINT, UINT >::iterator itm = m_mapErrorMotion.begin(); itm != m_mapErrorMotion.end(); itm++ )
    {
        kOutFile << "MotionID : " << itm->first << " : " << itm->second << "\n";
    }
    kOutFile.close();

    m_mapErrorMotion.clear();

    if( bSendMail )
    {
        std::stringstream strTitle;
        strTitle << "<ErrorMotion : " << "[" << NATION_CODE << "]" << ">" << "UserID : " << GCUTIL_STR::GCStrWideToChar( m_kUserInfo.strLogin.c_str() );
        strTitle << ", Time : " << GCUTIL_STR::GCStrWideToChar( (LPCTSTR)m_tmServerTime.Format( KNC_TIME_FORMAT ) );

        std::vector<std::string> vecFileName;
        vecFileName.push_back( strFileName );

        std::string strBody = "";

        MIME_Free( ms_kMail.dataHead1 );
        MIME_Free( ms_kMail.dataHead2 );
        MIME_Free( ms_kMail.dataBody );
        MIME_Free( ms_kMail.dataAttach );

        //아래 리턴이 INADDR_NONE이면 잘못된거져..--;;
        unsigned long ulSMTPAddr=SMTP_TransAddr( "kogstudios.com" );

        if( ulSMTPAddr == INADDR_NONE )
            return false;

        ms_kMail.from		=   "gcdev@kogstudios.com";
        ms_kMail.to		    =   "gcdev@kogstudios.com";
        ms_kMail.cc		    =   NULL; //CC는 무조건 NULL
        ms_kMail.bcc        =   NULL; //BCC는 무조건 NULL
        ms_kMail.dataHead1	=   MIME_MakeHead1( ms_kMail.to, ms_kMail.cc );
        ms_kMail.dataHead2	=   MIME_MakeHead2( "ErrorMotion", ms_kMail.from, (char*)strTitle.str().c_str(), 0 );
        ms_kMail.dataBody	=   MIME_MakeBody( (char*)strBody.c_str(), 0);

        // Attatch안에 어떻게 되어있는지 몰라서 일단 아래 코드 살려둔다. 썅
        char **pFiles=NULL;
        int cFiles = (int)vecFileName.size();
        for( int i = 0 ; i < cFiles ; ++i )
        {
            if( i == 0 )
            {
                pFiles=(char**)malloc(sizeof(char*));
            }
            else
            {
                pFiles=(char**)realloc( pFiles, sizeof( char* ) * (i + 1 ));
            }
            pFiles[i]= (char*)vecFileName[i].c_str();
        }
        ms_kMail.dataAttach	= MIME_MakeAttach( pFiles, cFiles );
        ms_kMail.dataEnd	= MIME_GetEnd();

        if (!SMTP_SendMail( g_MyD3D->Get_hWnd(), ulSMTPAddr, &ms_kMail, GlobalMailSendEnd, NULL ) )
        {
            //실패, 실패하면 콜백으로 들어가는지 안들어 가는지 모르겠다. 일단 놔둔다.
            MIME_Free( ms_kMail.dataHead1 );
            MIME_Free( ms_kMail.dataHead2 );
            MIME_Free( ms_kMail.dataBody );
            MIME_Free( ms_kMail.dataAttach );
            return false;
        }
    }
    return true;
}

void KGCGlobal::GlobalMailSendEnd( BOOL bSuccess_ )
{
    MIME_Free( ms_kMail.dataHead1 );
    MIME_Free( ms_kMail.dataHead2 );
    MIME_Free( ms_kMail.dataBody );
    MIME_Free( ms_kMail.dataAttach );
}

void KGCGlobal::ADD_BLOCK_USER( std::wstring strName )
{
    std::vector<std::wstring>::iterator vCurrentItr;
    vCurrentItr = find( m_vecCurrentStateUserInfo.begin(), m_vecCurrentStateUserInfo.end(), strName );
    if ( vCurrentItr != m_vecCurrentStateUserInfo.end() )
    {
        // 내가 속한 장소(대전, 던전, 광장)에 차단할 아이디가 있다!!!!!!
        std::wstringstream strTemp;
        std::vector<std::wstring>::iterator vit;
        vit = find( m_vecBlockUserName.begin(), m_vecBlockUserName.end(), strName );
        // 내가 찍은놈은 근처에 있을수 밖에 없군하!  이 유저가 있는놈인지 없는 놈인지는 검사하지 않겠다!
        if ( vit == m_vecBlockUserName.end() )
        {
            // 차단 유저리스트에 없다면 차단하쟈!!
            m_vecBlockUserName.push_back( strName );
            strTemp << g_pkStrLoader->GetReplacedString( STR_ID_BLOCK_BADGUY_REPORT, "l", strName );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strTemp.str(), L""
                , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        }
        else
        {
            // 이미 등록되어 있네 -_-;;
            strTemp << g_pkStrLoader->GetReplacedString( STR_ID_ALREADY_BLOCK_BADGUY, "l", strName );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strTemp.str(), L""
                , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        }
    }
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ERROR_BADGUY ), L""
            , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
}

void KGCGlobal::DEL_BLOCK_USER( std::wstring strName )
{
    std::vector<std::wstring>::iterator vCurrentItr;
    vCurrentItr = find( m_vecCurrentStateUserInfo.begin(), m_vecCurrentStateUserInfo.end(), strName );
    if ( vCurrentItr != m_vecCurrentStateUserInfo.end() )
    {
        std::vector<std::wstring>::iterator vit;
        vit = find( m_vecBlockUserName.begin(), m_vecBlockUserName.end(), strName );
        if ( vit != m_vecBlockUserName.end() )
        {
            std::wstringstream strTemp;
            m_vecBlockUserName.erase( vit );
            strTemp << g_pkStrLoader->GetReplacedString( STR_ID_RELEASE_BADGUY_REPORT, "l", strName );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, strTemp.str(), L""
                , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        }
    }
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ERROR_BADGUY ), L""
            , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
}

void KGCGlobal::AddCurrentStateUser( std::wstring strName )
{
    std::vector<std::wstring>::iterator vit;
    vit = find( m_vecCurrentStateUserInfo.begin(), m_vecCurrentStateUserInfo.end(), strName );
    if ( vit == m_vecCurrentStateUserInfo.end() )
        m_vecCurrentStateUserInfo.push_back( strName );
}

void KGCGlobal::DelCurrentStateUser( std::wstring strName )
{
    std::vector<std::wstring>::iterator vit;
    vit = find( m_vecCurrentStateUserInfo.begin(), m_vecCurrentStateUserInfo.end(), strName );
    if ( vit != m_vecCurrentStateUserInfo.end() )
        m_vecCurrentStateUserInfo.erase( vit );
}

void KGCGlobal::ReleaseCurrentStateUser( void )
{
    m_vecCurrentStateUserInfo.clear();
}

bool KGCGlobal::IsBlockUser( std::wstring strName )
{
    //오후 4:44 2008-02-28 jemitgge - 차단당한 아뒤는 띄워주지 않습니다!
    std::vector<std::wstring>::iterator vit;
    vit = find( g_kGlobalValue.m_vecBlockUserName.begin(), g_kGlobalValue.m_vecBlockUserName.end(), strName );
    if ( vit != g_kGlobalValue.m_vecBlockUserName.end() )
        return true;
    else
        return false;
}

void KGCGlobal::UpdateCashData()
{
#if defined( WEB_CASH )
#if defined( NATION_USA ) || defined(NATION_EU)
	std::stringstream stm;
    stm.str("");    // 가끔 이상하게 초기화되지 않는 문제가 있어서 직접 초기화 한다
	GET_CASH_WEB_PAGE( stm );
	stm<<"\0";
	int iCash = 0;

	typedef std::pair< std::string, std::string > PostArg;
	std::vector< PostArg > vecPostArg;

	vecPostArg.push_back( PostArg( "id", m_kLoginInfo.strID ) );
	vecPostArg.push_back( PostArg( "pw", m_kLoginInfo.strPass ) );
#if defined ( __PATH__ ) || defined ( USE_NA_INTERNAL_BILLING )
	if( false == GCFUNC::GetValueFromURL( stm.str().c_str(), iCash, vecPostArg)){
		iCash = 0;
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_IE9_ERR ), L""
			, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
    }
#else
    // 현재 북미 사내에 빌링 웹페이지가 연결되지 않아서 응답없음이 리턴되기까지 오래 기다리는 문제가 있음
    // 임시로 사내에 한하여 캐시데이터 가져오지 않게 처리함.
    // 웹캐시 사용하는 타 국가가 이쪽 코드로 넘어올 경우 사내 캐시데이터를 가져오지 않는다면 여기 확인하면 됨
    iCash = 0;
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, L"The internal server failed to call up the web cache data.\nThis is not an error message, it is for testing purposes only.", L""
        , KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );

#endif
	m_dwCash = (DWORD)iCash;
#endif
#endif
}

bool KGCGlobal::IsAdmin()
{
	return m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN;
}

void KGCGlobal::SetObserverMode( bool bObserver )
{
	m_bObserverMode = bObserver;
}

void KGCGlobal::SetHellMode( bool bHellMode )
{
    m_bHellMode = bHellMode;
}

void KGCGlobal::SetBreakMode(bool bBreakMode)
{
    m_bBreakMode = bBreakMode;
}


bool KGCGlobal::IsEquipSkill(const int iSkill_ID, int iPetID)
{
	if( -1 == iPetID )
		return false;

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK( L );
    lua_getglobal( L, "IsAvailablePetSkill" );
    if( lua_isfunction( L, -1 ) == false ) {
        lua_pop( L, 1 );
        return false;
    }

    lua_pushnumber( L, iPetID );
    lua_pushnumber( L, iSkill_ID );
    if( lua_pcall( L, 2, 1, 0 ) != 0 ) {
        lua_pop( L, 1 );
    }
    else {
        if( lua_isboolean( L, -1 ) ) {
            bool bRet = (lua_toboolean( L, -1 ) != 0);
            lua_pop( L, 1 );
            return bRet;
        }
        else {
            lua_pop( L, 1 );
            return false;
        }
    }

    return false;
}


void KGCGlobal::ToggleVideoCapture()
{
	IDirect3DDevice9* pd3d9Device = g_pd3dDevice;

	// 캡처를 시작합니다.
	if(m_bandiCaptureLibrary.IsCapturing() == FALSE && g_pkUIScene->IsReplayDlgRenderOn() ==  false )
	{
		// 최초 실행시 DLL 을 로드하고 내부 인스턴스를 생성합니다. 
		if(m_bandiCaptureLibrary.IsCreated()==FALSE)
		{
			// 실패시 DLL과 .H파일의 버전이 동일한지 여부등을 
			// 확인해 보시기 바랍니다.
			if(FAILED(m_bandiCaptureLibrary.Create(BANDICAP_RELEASE_DLL_FILE_NAME)))
			{
				ASSERT(0);
			}
			// 인증을 받지 않으면 인코딩된 동영상 상단에 로고가 표시됩니다. 
			// 정식구매후 발급받은 키를 사용하거나, 홈페이지에서 데모용으로 공개된 
			// 키를 사용해서 인증을 받습니다. 
			//if(FAILED(m_bandiCaptureLibrary.Verify("BANDISOFT-TRIAL-200809", "e1b03d86")))    데모 인증키
            if(FAILED(m_bandiCaptureLibrary.Verify("KOG_GRANDCHASE_20100712", "fdf40de1")))    //  진짜 인증키
			{
				ASSERT(0);
			}
		}

		if(m_bandiCaptureLibrary.IsCreated())
		{
            BCAP_CONFIG cfg;
            // 프리셋을 통해서 손쉽게 설정이 가능하며, 
            // 직접 파라미터 설정을 바꿀 수도 있습니다.
            BCapConfigPreset(&cfg, BCAP_PRESET_DEFAULT);
            SetBancapCfg( cfg );
            m_bandiCaptureLibrary.CheckConfig(&cfg);	// 잘못된 설정 바로 잡기
            m_bandiCaptureLibrary.SetConfig(&cfg);		// 설정 적용
            m_bandiCaptureLibrary.SetMinMaxFPS(25, GC_FPS_LIMIT);	// 최소, 최대 프레임 설정

            // 저장 경로 만들기 
            TCHAR strPath[MAX_PATH] = {0, };
            GetPrivateProfileString( L"COMMON", L"Path", L"\\Replay", strPath, MAX_PATH - 1, L"./BancapConfig.ini" );
            CreateDirectory( strPath, NULL );
            StrCatW(strPath, L"\\");

            TCHAR pathName[MAX_PATH] = {0, };
            m_bandiCaptureLibrary.MakePathnameByDate(strPath, _T("GrandChase"), _T("avi"), pathName, MAX_PATH );

            // 캡처를 시작합니다.
            HRESULT hr = m_bandiCaptureLibrary.Start(pathName, NULL, 
                BCAP_MODE_D3D9_SCALE, (LONG_PTR)pd3d9Device);
            if(FAILED(hr))
            {
                ASSERT(0);
            }

            g_pkUIScene->RECPlay();
		}
	}
	// 캡처를 중지합니다.
	else
	{
		m_bandiCaptureLibrary.Stop();
        g_pkUIScene->RECStop();
	}

}

void KGCGlobal::CapTureWork( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_bandiCaptureLibrary.Work((LONG_PTR)pd3dDevice);
    // 여기 넣자 ~ 표시 
}

bool KGCGlobal::IsCapturing( void )
{
	if ( m_bandiCaptureLibrary.IsCapturing())
		return true;
	else
		return false;
}

bool KGCGlobal::IsEditMode()
{
    return m_bEditMode;
}

void KGCGlobal::SetEditMode( bool bEdit_ )
{   

    m_bEditMode = bEdit_;

}

SPetTemplate& KGCGlobal::GetPetTemplate_( int iPetID_, int iPromotion_ )
{
    static SPetTemplate tempTemplate = SPetTemplate();
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK( L );
    lua_getglobal( L, "GetPetTemplate" );
    if( lua_isfunction( L, -1 ) == false ) {
        lua_pop( L, 1 );
        return tempTemplate;
    }

    lua_pushnumber( L, iPetID_ );
    lua_pushnumber( L, iPromotion_ );
    if( lua_pcall( L, 2, 1, 0 ) != 0 ) {
        lua_pop( L, 1 );
    }
    else {
        if( lua_isuserdata( L, -1 ) == 1 ) {
            SPetTemplate* pData = lua_tinker::lua2object<SPetTemplate*>::invoke( L, -1 );
            lua_pop( L, 1 );
            return *pData;
        }
        else {
            lua_pop( L, 1 );
            return tempTemplate;
        }
    }

    return tempTemplate;

    std::map< int, KPetTemplateContainer >::iterator mitPet = m_mapPetTemplate.find( iPetID_ );
    if( mitPet == m_mapPetTemplate.end() )
        return tempTemplate;

    KPetTemplateContainer::iterator mitPromotion = mitPet->second.find( iPromotion_ );
    if( mitPromotion == mitPet->second.end() ) {
        if( iPromotion_ != 0 ) {
            return GetPetTemplate_( iPetID_, 0 );
        }
        else {
            return tempTemplate;
        }
    }

    return mitPromotion->second;
}

const SPetTemplate& KGCGlobal::GetPetTemplate( int iPetID_, int iPromotion_ )
{
    return GetPetTemplate_( iPetID_, iPromotion_ );
}

const SPetTemplate& KGCGlobal::GetPetTemplateFromItemID( GCITEMID ItemID_, int iPromotion_ )
{
	int iPetID = g_kGlobalValue.GetPetIDFromItemID( ItemID_ );
	return GetPetTemplate( iPetID, iPromotion_ );
}

void KGCGlobal::UpdatePetCostume( int iPetID_, int iPetPromotion_, const std::set<DWORD>& setCostumes )
{
    SPetTemplate& sPetTemplate = GetPetTemplate_( iPetID_, iPetPromotion_ );
    sPetTemplate.vecCostumeList.clear();
    sPetTemplate.vecCostumeList.insert( sPetTemplate.vecCostumeList.end(), setCostumes.begin(), setCostumes.end() );
}

int SPetTemplate::GetDescID( int iState_ ) const
{
    std::map<int, int>::const_iterator mit = mapDescID.find( iState_ );
    if( mit == mapDescID.end() )
        return -1;

    return mit->second;
}

std::vector<PetMeshInfo> KGCGlobal::GetPetCostumeMeshList( DWORD dwItemID )
{
	if( m_mapPetCostumeInfo.find( dwItemID ) == m_mapPetCostumeInfo.end() )
		return std::vector<PetMeshInfo>();

	return m_mapPetCostumeInfo[dwItemID];
}

void KGCGlobal::AddPetCostumeMeshInfo( DWORD dwItemID, PetMeshInfo sInfo )
{
	typedef std::map<DWORD,std::vector<PetMeshInfo>> PetCosInfo;
	m_mapPetCostumeInfo[dwItemID].push_back( sInfo );
}

bool KGCGlobal::IsPetCostumeItem( DWORD dwItemID )
{
	return m_mapPetCostumeInfo.find( dwItemID ) != m_mapPetCostumeInfo.end();
}

void KGCGlobal::AddPetTemplate( int iPetID_, int iPromotion_, const SPetTemplate& sPetTemplate_ )
{
    SPetTemplate& sTemplate = m_mapPetTemplate[iPetID_][iPromotion_];
    sTemplate = sPetTemplate_;
    sTemplate.iPetID = iPetID_;
    sTemplate.iPromotion = iPromotion_;

    if( sTemplate.iPromotion == 1 ) {
        sTemplate.fEvolutionConst = 2.f;
    }
    else if( sTemplate.iPromotion == 2 ) {
        sTemplate.fEvolutionConst = 4.f;
    }
}

int KGCGlobal::GetPetIDFromItemID( GCITEMID ItemID_ )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK( L );
    lua_getglobal( L, "GetPetTemplateFromItemID" );
    if( lua_isfunction( L, -1 ) == false ) {
        lua_pop( L, 1 );
        return -1;
    }

    lua_pushnumber( L, ItemID_ );
    if( lua_pcall( L, 1, 1, 0 ) != 0 ) {
        lua_pop( L, 1 );
    }
    else {
        if( lua_isuserdata( L, -1 ) == 1 ) {
            SPetTemplate* pData = lua_tinker::lua2object<SPetTemplate*>::invoke( L, -1 );
            int iPetID = pData->iPetID;
            lua_pop( L, 1 );
            return iPetID;
        }
        else {
            lua_pop( L, 1 );
            return -1;
        }
    }
    
    return -1;

    std::map< int, KPetTemplateContainer >::const_iterator mit;
    for( mit = m_mapPetTemplate.begin() ; mit != m_mapPetTemplate.end() ; ++mit ) {
        KPetTemplateContainer::const_iterator mitPromotion = mit->second.begin();

        for( ; mitPromotion != mit->second.end() ; ++mitPromotion ) {
            const SPetTemplate& sTemplate = mitPromotion->second;

            if( sTemplate.dwGoodsID != ItemID_ )
                continue;

            return mit->first;
        }
    }
    return -1;
}

const SPetTemplate& KGCGlobal::GetBasePetTemplateFromPetCostume( DWORD dwItemID )
{
    static SPetTemplate tempTemplate = SPetTemplate();
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK( L );
    lua_getglobal( L, "GetBasePetTemplateFromPetCostume" );
    if( lua_isfunction( L, -1 ) == false ) {
        lua_pop( L, 1 );
        return tempTemplate;
    }

    lua_pushnumber( L, dwItemID );
    if( lua_pcall( L, 1, 1, 0 ) != 0 ) {
        lua_pop( L, 1 );
    }
    else {
        if( lua_isuserdata( L, -1 ) == 1 ) {
            SPetTemplate* pData = lua_tinker::lua2object<SPetTemplate*>::invoke( L, -1 );
            lua_pop( L, 1 );
            return *pData;
        }
        else {
            lua_pop( L, 1 );
            return tempTemplate;
        }
    }

    return tempTemplate;


//    static SPetTemplate tempTemplate = SPetTemplate();
    std::map< int, KPetTemplateContainer >::const_iterator mit;
    for( mit = m_mapPetTemplate.begin() ; mit != m_mapPetTemplate.end() ; ++mit ) {
        KPetTemplateContainer::const_iterator mitPromotion = mit->second.begin();

        for( ; mitPromotion != mit->second.end() ; ++mitPromotion ) {
            const SPetTemplate& sTemplate = mitPromotion->second;

            std::vector<DWORD>::const_iterator vit = std::find( sTemplate.vecCostumeList.begin(), sTemplate.vecCostumeList.end(), dwItemID );
            if( vit == sTemplate.vecCostumeList.end() )
                continue;

            return mitPromotion->second;
        }
    }
    return tempTemplate;
}

DWORD KGCGlobal::GetBasePetItemIDFromPetCostume( DWORD dwItemID )
{
    return GetBasePetTemplateFromPetCostume(dwItemID).dwGoodsID;
}

int KGCGlobal::GetPromotionFromPetCostume( DWORD dwItemID )
{
	return GetBasePetTemplateFromPetCostume(dwItemID).iPromotion;
}

std::wstring KGCGlobal::GetServerbarEventTitle()
{
	return m_strServerbarEventTitle;
}

std::wstring KGCGlobal::GetServerbarEventSubtitle()
{
	return m_strServerbarEventSubtitle;
}

std::wstring KGCGlobal::GetServerbarEventDesc()
{
	return m_strServerbarEventDesc;
}

void KGCGlobal::SetServerbarEventTitle( IN const std::wstring& str_ )
{
	m_strServerbarEventTitle = str_;
}

void KGCGlobal::SetServerbarEventSubtitle( IN const std::wstring& str_ )
{
	m_strServerbarEventSubtitle = str_;
}

void KGCGlobal::SetServerbarEventDesc( IN const std::wstring& str_ )
{
	m_strServerbarEventDesc = str_;
}

void KGCGlobal::ServerbarEventStringClear()
{
	m_strServerbarEventTitle.clear();
	m_strServerbarEventSubtitle.clear();
	m_strServerbarEventDesc.clear();
}

bool KGCGlobal::CheckPremium( KPremiumInfo::PREMIUM_TYPE eType )
{
	return (m_kUserInfo.dwPremium & eType) > 0;
}

void KGCGlobal::AddFreeWorldMapInHellMode()
{
    m_setFreeWorldMapInHellMode.clear();

    std::set<int>::iterator sit = m_setFreeDungeonInHellMode.begin();

    for(; sit != m_setFreeDungeonInHellMode.end(); ++sit)
    {
        int iWorldMap = NUM_WORLDMAP;

        if( SiKGCWorldMapManager()->GetWorldMapOfGameMode( static_cast< EGCGameMode >(*sit) , iWorldMap ) && 
            m_setFreeWorldMapInHellMode.find( iWorldMap ) == m_setFreeWorldMapInHellMode.end() )
        {
            m_setFreeWorldMapInHellMode.insert( iWorldMap );
        }
    }
}

bool KGCGlobal::IsOpenedChar( int iCharID )
{
	return m_mapCharactes.find( iCharID ) != m_mapCharactes.end();
}

bool KGCGlobal::IsOpenedPromotion( int iCharID, char iPromotion )
{
    std::map< int, std::set< char > >::iterator mit = m_mapCharactes.find( iCharID );
    if( mit != m_mapCharactes.end() ) {
        return mit->second.find( iPromotion ) != mit->second.end();
    }
    return false;
}

bool KGCGlobal::IsOpenedPromotion( int iCharID, std::set< char > setPromotion )
{
    if( setPromotion.empty() ) { 
        return false;
    }

    for( std::set< char >::iterator sit = setPromotion.begin(); sit != setPromotion.end(); ++sit ) {
        if( IsOpenedPromotion( iCharID, *sit ) == false ) {
            return false;
        }
    }

    return true;
}

void KGCGlobal::GetOpenedPromotion( int iCharID, std::set< char >& setPromotion )
{
    std::map<int, std::set< char > >::iterator mit = m_mapCharactes.find( iCharID );

    if( mit != m_mapCharactes.end() ) {
        setPromotion = mit->second;
    }
}

int KGCGlobal::GetOpenedPromotionNum( int iCharID )
{
    std::map<int, std::set< char > >::iterator mit = m_mapCharactes.find( iCharID );

    if( mit != m_mapCharactes.end() ) {
        return static_cast< int >( mit->second.size() ) - 1;
    }

    return 0;
}

int KGCGlobal::GetGuildMemberTotalCount()
{
	int iMemberTotalCount = 0;

	std::map< DWORD, KNGuildUserInfo >::iterator mit;
	for( mit = m_mapNGuildUserInfo.begin(); mit != m_mapNGuildUserInfo.end(); ++mit )
	{
		if( mit->second.m_cMemberLevel == KNGuildUserInfo::GL_MASTER || 
			mit->second.m_cMemberLevel == KNGuildUserInfo::GL_GENERAL||
			mit->second.m_cMemberLevel == KNGuildUserInfo::GL_NORMAL )
			iMemberTotalCount++;
	}

	return iMemberTotalCount;
}

char KGCGlobal::GetSavedCharTypeToIndex()
{
    for ( int i = 0; i < (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size(); ++i ) 
    {
        if ( m_cSavedCharType == g_kGlobalValue.m_kUserInfo.vecCharInfo[i].iCharType )
        {
            return i;
        }
    }

    // 맞는 아이가 없다면...
    if ( !g_kGlobalValue.m_kUserInfo.vecCharInfo.empty() )
    {
        return 0;
    }

    return -1;
}

void KGCGlobal::SendSavedCharType()
{
    KP2P::GetInstance()->Send_GuideBoolCheckPointNot( 255, g_kGlobalValue.GetSavedCharType() );
    KP2P::GetInstance()->Send_SetCurrentCharacter( g_kGlobalValue.GetSavedCharType() );
}

void KGCGlobal::SetTRAvgPing( int iPing )
{
    if( iPing > 0 )
    {
        if( m_deqTRAvgPing.empty() )
        {
            for( int i = 0; i < 20; i++ )
                m_deqTRAvgPing.push_back(iPing);
        }
        else
        {
            m_deqTRAvgPing.pop_front();
            m_deqTRAvgPing.push_back(iPing);
        }
    }               
}

int KGCGlobal::GetTRAvgPing()
{
    if( m_deqTRAvgPing.empty() )
    {
        for( int i = 0; i < 20; i++ )
            m_deqTRAvgPing.push_back(SiKP2P()->m_spTRUserProxy->GetPing());
    }

    std::deque<int>::iterator dit = m_deqTRAvgPing.begin();

    int iTempAvgPing = 0;
    for( ; dit != m_deqTRAvgPing.end(); ++dit )
    {
        iTempAvgPing += (*dit);
    }

    if( iTempAvgPing > 0 )
        return iTempAvgPing / static_cast<int>(m_deqTRAvgPing.size());
    else
        return 0;
}

void KGCGlobal::SetBancapCfg(BCAP_CONFIG& pCfg)
{
    int iUseDefault = GetPrivateProfileInt( L"COMMON", L"UseDefault", 1, L"./BancapConfig.ini" );

    if( 1 == iUseDefault )
    {
        int iLevel = -1;

#if defined( NATION_KOREA )
        iLevel = GetPrivateProfileInt(L"KOREA", L"DefaultLevel", 2, L"./BancapConfig.ini");
#else
        iLevel = GetPrivateProfileInt(L"OTHER", L"DefaultLevel", 1, L"./BancapConfig.ini");
#endif

        switch (iLevel)
        {
        case 1:
            pCfg.VideoSizeW = GetPrivateProfileInt(L"DEFAULT_L1", L"SizeWidth", 320, L"./BancapConfig.ini");
            pCfg.VideoSizeH = GetPrivateProfileInt(L"DEFAULT_L1", L"SizeHeight", 240, L"./BancapConfig.ini");
            pCfg.VideoFPS = static_cast<float>(GetPrivateProfileInt(L"DEFAULT_L1", L"FPS", 25, L"./BancapConfig.ini"));
            break;

        case 2:
            pCfg.VideoSizeW = GetPrivateProfileInt(L"DEFAULT_L2", L"SizeWidth", 400, L"./BancapConfig.ini");
            pCfg.VideoSizeH = GetPrivateProfileInt(L"DEFAULT_L2", L"SizeHeight", 300, L"./BancapConfig.ini");
            pCfg.VideoFPS = static_cast<float>(GetPrivateProfileInt(L"DEFAULT_L2", L"FPS", 40, L"./BancapConfig.ini"));
            break;

        case 3:
            pCfg.VideoSizeW = GetPrivateProfileInt(L"KOREA", L"SizeWidth", 800, L"./BancapConfig.ini");
            pCfg.VideoSizeH = GetPrivateProfileInt(L"KOREA", L"SizeHeight", 600, L"./BancapConfig.ini");
            pCfg.VideoFPS = static_cast<float>(GetPrivateProfileInt(L"KOREA", L"FPS", 55, L"./BancapConfig.ini"));
            break;
        }

    }
    else
    {
#if defined( NATION_KOREA )   
        pCfg.VideoSizeW = GetPrivateProfileInt(L"KOREA", L"SizeWidth", 400, L"./BancapConfig.ini");
        pCfg.VideoSizeH = GetPrivateProfileInt(L"KOREA", L"SizeHeight", 300, L"./BancapConfig.ini");
        pCfg.VideoFPS = static_cast<float>(GetPrivateProfileInt(L"KOREA", L"FPS", 40, L"./BancapConfig.ini"));
#else
        pCfg.VideoSizeW = GetPrivateProfileInt(L"OTHER", L"SizeWidth", 400, L"./BancapConfig.ini");
        pCfg.VideoSizeH = GetPrivateProfileInt(L"OTHER", L"SizeHeight", 300, L"./BancapConfig.ini");
        pCfg.VideoFPS = static_cast<float>(GetPrivateProfileInt(L"OTHER", L"FPS", 40, L"./BancapConfig.ini"));
#endif
    }
}

void KGCGlobal::SetMonsterStat( int monId_, int level_, SMonsterTemplate& monTemplate_, bool bBoss )
{
    if ( level_ < EM_MONSTER_MIN_LV || level_ > EM_MONSTER_MAX_LV )
    {
        level_ = 0;
        ASSERT( "몬스터 레벨이 이상해요~ 0으로 만들어 버릴꺼야!! " );
    }

    if ( monId_ < 0 || monId_ > MONSTER_MODEL_NUM )
    {
        monId_ = 0;
        ASSERT( "몬스터 번호가 이상해요~ 슬라임로 만들어 버릴꺼야!! " );
    }

    std::map< int, LevelTable >::iterator mitL = m_mapMonLevelTable.find( level_ );
    if ( mitL != m_mapMonLevelTable.end() )
    {
        monTemplate_.fExp = (*mitL).second.Exp;
        monTemplate_.fSkExp = (*mitL).second.SkExp;
        monTemplate_.fGP = (*mitL).second.GP;
        monTemplate_.fAttackPoint = (*mitL).second.Atk;
        monTemplate_.fHPPoint = (*mitL).second.Vit;
    }

    std::map< int, MonsterWeightTable >::iterator mitW = m_mapMonWeightTable.find( monId_ );
    if ( mitW != m_mapMonWeightTable.end() )
    {
        monTemplate_.fExp *= (*mitW).second.Exp;
        monTemplate_.fSkExp *= (*mitW).second.SkExp;
        monTemplate_.fGP *= (*mitW).second.GP;
        monTemplate_.fAttackPoint *= (*mitW).second.Atk;
        monTemplate_.fHPPoint *= (*mitW).second.Vit;
        monTemplate_.fDefence = (*mitW).second.Def;
        monTemplate_.fSDefence = (*mitW).second.SDef;
    }

    // Não aplica na torre da extinção e nem na torre das ilusões.
    if (SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST81 && SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST52) {
        if (bBoss) {
            monTemplate_.fAttackPoint += (monTemplate_.fAttackPoint * 0.5); // 50%
            monTemplate_.fHPPoint += (monTemplate_.fHPPoint * 2.0); // 200%.
        }
        else {
            monTemplate_.fAttackPoint += (monTemplate_.fAttackPoint * 0.25); // 25%
            monTemplate_.fHPPoint += (monTemplate_.fHPPoint * 1.5); // 150%.
        }
    }

    // Portão dimensional
    {
        if (SiKGCWorldMapManager()->IsInfinityDungeon(SiKGCRoomManager()->GetGameMode())) {
            // A Cada minuto aumenta a vida dos monstros.
            DWORD minute = g_pGameBoard->GetPlayTimeInFrame() / GC_FPS_LIMIT / 60;

            // Percentage
            float mPercentage = 7.5 * minute;
            mPercentage /= 100;

            // Altera a vida
            monTemplate_.fHPPoint += (monTemplate_.fHPPoint * mPercentage);
        }
    }

    // Torre
    {
        // -95% de exp.
        if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST52) {
            monTemplate_.fExp -= (monTemplate_.fExp * 0.95);
            monTemplate_.fSkExp -= (monTemplate_.fSkExp * 0.95);
        }
    }

    if ( bBoss ) 
    {
        //  HP 증가 제외할 보스 몬스터
        if ( m_setBossHPDisable.find( monId_ ) == m_setBossHPDisable.end() )
        {
            if (m_nPlayerCount > 0 && m_nPlayerCount <= static_cast<int>(m_vecBossHPRate.size())) {
                monTemplate_.fHPPoint *= m_vecBossHPRate[m_nPlayerCount - 1];
                monTemplate_.fDefence *= m_vecBossHPRate[m_nPlayerCount - 1];
                monTemplate_.fAttackPoint *= m_vecBossHPRate[m_nPlayerCount - 1];
            }
        }
    }
    else { 
        // 일반 몬스터도 파티원 수에 따라 HP 가중 추가 
        if ( m_setNormalMonHPDisable.find( monId_ ) == m_setNormalMonHPDisable.end() )
        {
            if (m_nPlayerCount > 0 && m_nPlayerCount <= static_cast<int>(m_vecNormalMonHPRate.size())) {
                monTemplate_.fHPPoint *= m_vecNormalMonHPRate[m_nPlayerCount - 1];
                monTemplate_.fDefence *= m_vecNormalMonHPRate[m_nPlayerCount - 1];
                monTemplate_.fAttackPoint *= m_vecNormalMonHPRate[m_nPlayerCount - 1];
            }
        }
    }


    KMonsterAbility kMonsterAbility;
    if(g_kGlobalValue.GetEventMonsterList(kMonsterAbility, monId_)){
        monTemplate_.fAttackPoint = (monTemplate_.fAttackPoint * static_cast<float>(kMonsterAbility.m_nAttackRatio)) / 100.f ;
        monTemplate_.fDefence = (monTemplate_.fDefence * static_cast<float>(kMonsterAbility.m_nDefenceRatio))/ 100.f;
        monTemplate_.fHPPoint = (monTemplate_.fHPPoint * static_cast<float>(kMonsterAbility.m_nHPRatio))/ 100.f;
    }
}

void KGCGlobal::LoadMonsterStatInfo()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "MonsterStat.lua" ) == false )
    {
        MessageBoxA( NULL, "MonsterStat.lua Load Failed!", "", MB_OK );
        return;
    }

    luaMgr.BeginTable( "Stat" );
    {
        int level = 0;
        while( true )
        {
            char strtmp[MAX_PATH];
            sprintf(strtmp, "Lv%d", level);
            if( FAILED( luaMgr.BeginTable( strtmp ) ) ) break;
            {
                LevelTable levelTb;
                LUA_GET_VALUE_DEF( 1, levelTb.Exp, 1.0f );
                LUA_GET_VALUE_DEF( 2, levelTb.SkExp, 1.0f );
                LUA_GET_VALUE_DEF( 3, levelTb.GP, 1.0f );
                LUA_GET_VALUE_DEF( 4, levelTb.Atk, 1.0f );
                LUA_GET_VALUE_DEF( 5, levelTb.Vit, 1.0f );

                g_kGlobalValue.m_mapMonLevelTable[level] = levelTb;
            }
            luaMgr.EndTable();

            ++level;
        }
    }
    luaMgr.EndTable();

    luaMgr.BeginTable( "WeightTable" );
    {
        
        for ( int Id = 0; Id < MONSTER_MODEL_NUM; ++Id )
        {
            char strtmp[MAX_PATH];
            sprintf(strtmp, "MonID_%d", Id);
            if( FAILED( luaMgr.BeginTable( strtmp ) ) ) continue;
            {
                MonsterWeightTable weightTb;
                LUA_GET_VALUE_DEF( 1, weightTb.Exp, 1.0f );
                LUA_GET_VALUE_DEF( 2, weightTb.SkExp, 1.0f );
                LUA_GET_VALUE_DEF( 3, weightTb.GP, 1.0f );
                LUA_GET_VALUE_DEF( 4, weightTb.Atk, 1.0f );
                LUA_GET_VALUE_DEF( 5, weightTb.Vit, 1.0f );
                LUA_GET_VALUE_DEF( 6, weightTb.Def, 0.0f );
                LUA_GET_VALUE_DEF( 7, weightTb.SDef, 0.0f );

                g_kGlobalValue.m_mapMonWeightTable[Id] = weightTb;
            }
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

    g_kGlobalValue.m_vecBossHPRate.clear();

    if ( luaMgr.BeginTable( "BossHPRate" ) == S_OK )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
        {
            float fHPRate = 0.0f;
            LUA_GET_VALUE_DEF( i+1, fHPRate, 1.0f );

            g_kGlobalValue.m_vecBossHPRate.push_back( fHPRate );
        }
        
        luaMgr.EndTable();
    }

    g_kGlobalValue.m_vecNormalMonHPRate.clear();

    if ( luaMgr.BeginTable( "NormalMonHPRate" ) == S_OK )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
        {
            float fHPRate = 0.0f;
            LUA_GET_VALUE_DEF( i+1, fHPRate, 1.0f );

            g_kGlobalValue.m_vecNormalMonHPRate.push_back( fHPRate );
        }

        luaMgr.EndTable();
    }

    g_kGlobalValue.m_setBossHPDisable.clear();

    if ( luaMgr.BeginTable( "DisableBossHP") == S_OK )
    {
        for( int j = 1; ; ++j )
        {
            int nModID = 0;
            if( luaMgr.GetValue( j, nModID ) == E_FAIL )
                break;
            
            g_kGlobalValue.m_setBossHPDisable.insert( nModID );
        }
        luaMgr.EndTable();
    }

    g_kGlobalValue.m_setNormalMonHPDisable.clear();

    if ( luaMgr.BeginTable( "DisableNormalMonHP") == S_OK )
    {
        for( int j = 1; ; ++j )
        {
            int nModID = 0;
            if( luaMgr.GetValue( j, nModID ) == E_FAIL )
                break;

            g_kGlobalValue.m_setNormalMonHPDisable.insert( nModID );
        }
        luaMgr.EndTable();
    }
}

void KGCGlobal::LoadAdjustLevelInfo()
{
    m_mapAdjustRatio.clear();

    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "AdjustReward.lua" ) == false )
    {
        MessageBoxA( NULL, "AdjustReward.lua Load Failed!", "", MB_OK );
        return;
    }

    luaMgr.BeginTable( "MonModifiedInfo" );
    {
        LUA_GET_VALUE_DEF( "ModifiedMaxLevel", m_iAdjustMaxLevel, 11 );
        LUA_GET_VALUE_DEF( "ModifiedMinLevel", m_iAdjustMinLevel, -5 );
        LUA_GET_VALUE_DEF( "DefaultReward", m_fDefaultReward, 1.0f );

        luaMgr.BeginTable( "MonModifiedRatio" );
        {
            for( int i = 1 ; ; ++i )
            {
                LUA_BEGIN_TABLE(i, break )
                {
                    int iLevel = 0;
                    float fRatio = 1.0;
                    luaMgr.GetValue( 1, iLevel );
                    luaMgr.GetValue( 2, fRatio );

                    m_mapAdjustRatio[iLevel] = fRatio;
                }
                LUA_END_TABLE( break );
            }
        }
        luaMgr.EndTable();
    }
    luaMgr.EndTable();
}

void KGCGlobal::AdjustExp( IN const int iPlayerLevel_, IN const int iMonsterLevel_, IN const int monId_, OUT float& fMonReward_ )
{
    // 레벨 차이값 연산.
    int nLvDiff = iMonsterLevel_ - iPlayerLevel_;  

    // 캐릭터Lv - 몬스터Lv가 보정 최대 레벨보다 큰지 체크.
    if( nLvDiff < m_iAdjustMinLevel || nLvDiff > m_iAdjustMaxLevel ) {
        fMonReward_ = m_fDefaultReward; // 보정이 안될경우 기본 지급 경험치.
        return;
    }

    std::map< int, float >::iterator mit;
    mit = m_mapAdjustRatio.find( nLvDiff );
    if( mit == m_mapAdjustRatio.end() )
    {
        fMonReward_ = m_fDefaultReward; // 레벨 차이값이 보정Table에 없을 경우 기본 경험치 지급.
    }
    else
    {
        fMonReward_ = fMonReward_ * mit->second; // 몬스터 획득 경험치 * 보정 Ratio
    }
}

bool KGCGlobal::IsQuickSkillPlayer( int iCharType_ )
{
    switch( iCharType_ % GC_CHAR_NUM )
    {
    case GC_CHAR_DIO:
    case GC_CHAR_ZERO:
    case GC_CHAR_LEY:
    case GC_CHAR_LUPUS:    
    case GC_CHAR_BEIGAS:
#ifdef PERYTON
    case GC_CHAR_PERYTON:
#endif
        return true;
    default:
        return false;
    }
}

bool KGCGlobal::IsHybridPlayer( int iCharType_ )
{
    switch( iCharType_ % GC_CHAR_NUM )
    {
    case GC_CHAR_ASIN:
    case GC_CHAR_LIME:
    case GC_CHAR_RIN:
    case GC_CHAR_EDEL:
#ifdef DECANEE
    case GC_CHAR_DECANEE:
#endif
        return true;
    default:
        return false;
    }
}

bool KGCGlobal::IsCharUseSpecial4( int iCharType_ )
{
    switch( iCharType_ % GC_CHAR_NUM )
    {
    case GC_CHAR_ELESIS:
    case GC_CHAR_ARME:
    case GC_CHAR_LIRE:
    case GC_CHAR_LAS:
    case GC_CHAR_RYAN:
    case GC_CHAR_RONAN:
    case GC_CHAR_AMY:
    case GC_CHAR_JIN:
    case GC_CHAR_SIEG:
    case GC_CHAR_MARI:
    case GC_CHAR_UNO:
#ifdef ALICE // Ken>
    case GC_CHAR_ALICE:
#endif
        return true;
    default:
        return false;
    }
}

// 4단필 다 풀리기 전까지만 쓰자 - comming soon 표시용
bool KGCGlobal::IsTempUseSpecial4Char( int iCharType_ )
{
    if ( std::find(m_vecCharSP4OpenInfo.begin(), m_vecCharSP4OpenInfo.end(), iCharType_) != m_vecCharSP4OpenInfo.end() )
        return true;
    else
        return false;
}

void KGCGlobal::LoadFontList()
{
    DWORD dwSize = 1024;
    char szCurPath[1024];
    m_vecFontList.clear();
    GetCurrentDirectoryA(dwSize, szCurPath);
    strcat(szCurPath, "\\Fonts");
    FindDirectoryAllFontFiles( szCurPath );
}

void KGCGlobal::FindDirectoryAllFontFiles( char *strDirectory)
{
    char        strTempPath[MAX_PATH];
    char        strTempDrive[_MAX_DRIVE];
    char        strTempDir[_MAX_DIR];

    HANDLE      fileHandle;
    WIN32_FIND_DATAA  fileData;

    fileHandle = FindFirstFileA(strDirectory, &fileData);

    if( INVALID_HANDLE_VALUE != fileHandle )
    {
        do
        {
            if( _A_SUBDIR == ( fileData.dwFileAttributes & _A_SUBDIR ) )
            {
                if( strcmp(".", fileData.cFileName) && strcmp("..", fileData.cFileName) )
                {
                    strTempPath[0]  = 0;
                    strTempDrive[0] = 0;
                    strTempDir[0]   = 0;

                    _splitpath(strDirectory, strTempDrive, strTempDir, NULL, NULL);

                    strcpy(strTempPath, strTempDrive);
                    strcat(strTempPath, strTempDir);
                    strcat(strTempPath, fileData.cFileName);
                    strcat(strTempPath, "\\*.*");

                    FindDirectoryAllFontFiles(strTempPath);
                }
            }
            else
            {
                if( strstr(fileData.cFileName, ".ttf") ||
                    strstr(fileData.cFileName, ".ttc") ||
                    strstr(fileData.cFileName, ".fon") ||
                    strstr(fileData.cFileName, ".fot") )
                    m_vecFontList.push_back( fileData.cFileName );
            }
        }while( FindNextFileA(fileHandle, &fileData) );

        FindClose(fileHandle);
    }
}

void KGCGlobal::LoadStatsMIDList( char* strFilename_ )
{
    if( strFilename_ )
    {
        KLuaManager kLuaMgr;
        if ( !GCFUNC::LoadLuaScript( kLuaMgr, "MotionEnum.stg" ) )
        {
            return;
        }
        if ( !GCFUNC::LoadLuaScript( kLuaMgr, strFilename_ ) )
        {
            return;
        }

        int iData;
        kLuaMgr.BeginTable("StatsMotionList");
        {
            g_kGlobalValue.m_setMIDList.clear();
            for ( int iLoop = 1; kLuaMgr.GetValue( iLoop++, iData ) == S_OK; g_kGlobalValue.m_setMIDList.insert( iData ) );
        }
        kLuaMgr.EndTable();
    }
}
int KGCGlobal::GetGuildLevel()
{
    int iGlevel = -1;
    int iGpoint = (int)g_kGlobalValue.m_kGuildInfo.m_dwPoint;
    int iContribution = (int)g_kGlobalValue.m_kGuildUserInfo.m_dwContributePoint;

    std::map< USHORT, int >::iterator mit;
    for (mit = m_mapGuildLevelTable.begin() ; mit != m_mapGuildLevelTable.end(); ++mit )
    {
        if (mit->second > iGpoint)
            break;
        else
            iGlevel = mit->first;
    }
    return iGlevel;
}

float KGCGlobal::GetChampionAbility( int iAbilityType_ )
{
    if( m_mapChampionAbility.empty() )
        return 1.5f;

    std::map<int, float>::iterator mit = m_mapChampionAbility.find(iAbilityType_);

    if( mit != m_mapChampionAbility.end() )
    {
        return mit->second;
    }

    return 1.5f;
}

void KGCGlobal::SetChampionAbility( std::map<int, float> mapAbility_ )
{
    m_mapChampionAbility = mapAbility_;
}

KEquipItemInfo KGCGlobal::ConvertKItemtoKEquipItemInfo( KItem *pItem )
{
    KEquipItemInfo equipItemInfo;
    equipItemInfo.m_dwID = pItem->m_ItemID * 10;
    equipItemInfo.m_dwUID = pItem->m_ItemUID;
    equipItemInfo.m_cEnchantLevel = pItem->m_cEnchantLevel;
    equipItemInfo.m_nEquipLevel = pItem->m_sEquipLevel;
    equipItemInfo.m_cGradeID = pItem->m_cGradeID;
    equipItemInfo.m_vecSocket= pItem->m_vecSocket;
    equipItemInfo.m_vecAttribute = pItem->m_vecAttribute;    
    equipItemInfo.m_cEnchantEquipGradeID    = pItem->m_cEnchantEquipGradeID;
    return equipItemInfo;
}

void KGCGlobal::CheckUseTcpRelay( std::vector<int>& vecPing_ )
{   
    g_eP2PConMode = P2P_CON_DEFAULT;
    KSingleton<KncP2P>::GetInstance()->SetRelayOnly( false );
    int iTRPing = g_kGlobalValue.GetTRAvgPing();
    for( int i =  0; i < static_cast<int>(vecPing_.size()); i++ )
    {
        if( vecPing_[i] <= 0 || iTRPing < vecPing_[i] || vecPing_[i] > 300 )
        {
            g_eP2PConMode = P2P_TCP_RELAY_ONLY;
            KSingleton<KncP2P>::GetInstance()->SetRelayOnly( true );
            return;
        }
    }
}

void KGCGlobal::UseTCPRelay( bool bOnlyTR )
{
    if( bOnlyTR )
    {
        g_eP2PConMode = P2P_TCP_RELAY_ONLY;
        KSingleton<KncP2P>::GetInstance()->SetRelayOnly( true );
    }
    else
    {
        g_eP2PConMode = P2P_CON_DEFAULT;
        KSingleton<KncP2P>::GetInstance()->SetRelayOnly( false );
    }
}

void KGCGlobal::ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                    int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
        cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}

int KGCGlobal::GetCiphers( int iNum_ )
{
    int iCiphers = 0;
    while( true )
    {
        iNum_ = iNum_ / 10;

        if( iNum_ == 0 )
            break;

        ++iCiphers;
    }

    return iCiphers;
}

int KGCGlobal::IsSexCharacter( int iCharType )
{
    switch( iCharType )
    {
    case GC_CHAR_ELESIS:
    case GC_CHAR_LIRE:
    case GC_CHAR_ARME:
    case GC_CHAR_AMY:
    case GC_CHAR_MARI:
    case GC_CHAR_LEY:
    case GC_CHAR_RIN:
    case GC_CHAR_LIME:
    case GC_CHAR_EDEL:
#ifdef ALICE
    case GC_CHAR_ALICE:
#endif
#ifdef DECANEE
    case GC_CHAR_DECANEE:
#endif
        return SEX_GRIL;
    case GC_CHAR_LAS:
    case GC_CHAR_RYAN:
    case GC_CHAR_RONAN:
    case GC_CHAR_JIN:
    case GC_CHAR_SIEG:
    case GC_CHAR_DIO:
    case GC_CHAR_ZERO:
    case GC_CHAR_LUPUS:
    case GC_CHAR_ASIN:
    case GC_CHAR_BEIGAS:
    case GC_CHAR_UNO:
#ifdef PERYTON // Kawan>
    case GC_CHAR_PERYTON:
#endif
        return SEX_MAN;
    }

    return SEX_NONE;
}

bool KGCGlobal::IsHost()
{
    return m_kUserInfo.bHost;
}

#if defined( DUNGEONPLAYSTATE )
void KGCGlobal::AddDungeonPlayState( int iState, int iValue/* = 0*/ )
{
    // State 인덱스 확인
    if( DUNPLAY_STATE_CLEARTIME > iState || iState >= NUM_DUNPLAY_STATE )
        return;

    // 던전과 스테이지를 얻자!
    int iDungeonID = SiKGCRoomManager()->GetGameMode();
    int iStageID = g_kEveryTriggerManager.GetActiveStage();
    int iDifficult = SiKGCRoomManager()->GetDungeonLevel();

    // 키를 찾자!! => X던전에 X스테이지
    KDungeonDetail kDungeonDetail;
    kDungeonDetail.m_dwDungeonID = iDungeonID;
    kDungeonDetail.m_dwStageID = iStageID;
    kDungeonDetail.m_dwDifficult = iDifficult;
    std::map< KDungeonDetail, KDungeonPlayState >::iterator mapIter = m_mapDungeonPlayState.find( kDungeonDetail );

    // 키가 없으면 생성하자!
    if( mapIter == m_mapDungeonPlayState.end() )
    {
        m_mapDungeonPlayState.insert( std::make_pair( kDungeonDetail, KDungeonPlayState() ) );
        mapIter = m_mapDungeonPlayState.find( kDungeonDetail );
    }

    // 생성이 안瑛만?나가자!
    KDungeonPlayState* pDungeonPlayState = NULL;
    if( mapIter == m_mapDungeonPlayState.end() )
        return;
    else
    {
        pDungeonPlayState = &mapIter->second;
    }

    // 구조체가 없으면 나가자!
    if( NULL == pDungeonPlayState )
        return;

    // 통계를 누적하자!!
    switch( iState )
    {
    case DUNPLAY_STATE_CLEARTIME:
        if( 0 == pDungeonPlayState->m_dwClearTime )
            pDungeonPlayState->m_dwClearTime = (timeGetTime() - m_dwStageStartTime) / 1000;
        break;
    case DUNPLAY_STATE_DEATH:
        pDungeonPlayState->m_usDeath_Cnt++;
        break;
    case DUNPLAY_STATE_LEAVE:
        m_kDunPerforState.iNormalityLaveCnt++;
        pDungeonPlayState->m_usLeave_Cnt++;
        break;
    case DUNPLAY_STATE_SPECIAL:
        pDungeonPlayState->m_usSpecial_Cnt++;
        break;
    case DUNPLAY_STATE_PET:
        pDungeonPlayState->m_usPetSpecial_Cnt++;
        break;
    case DUNPLAY_STATE_GIVE_DAMAGE:
        pDungeonPlayState->m_dwSendDamage += iValue;
        break;
    case DUNPLAY_STATE_RECEIVE_DAMAGE:
        pDungeonPlayState->m_dwReceiveDamage += iValue;
        break;
    }
}
#endif

void KGCGlobal::applyPromotionBonuses(int statusBonus, float criticalBonus, int& iAtk_, int& iDef_, int& iHP_, float& fCritical_)
{
    iAtk_ += statusBonus;
    iDef_ += statusBonus;
    iHP_ += statusBonus;
    fCritical_ += criticalBonus;
}

void KGCGlobal::GetPromotionBonusStatus(const int& iPromotionLevel_, int& iAtk_, int& iDef_, int& iHP_, float& fCritical_, PLAYER* pPlayer_)
{
    if (SiKGCChannelManager()->IsPVPBallanceServer() || SiKGCRoomManager()->IsAngelEggModeAbility() || SiKGCRoomManager()->IsDotaModeAbility())
        return;

    int charType = pPlayer_->GetCurrentChar().iCharType;
    int charLevel = pPlayer_->GetCurrentChar().iLevel;
    bool isLocalPlayer = pPlayer_->IsLocalPlayer();

    int statusBonus = 0;
    float criticalBonus = 0.0f;

    for (int i = 0; i <= iPromotionLevel_; ++i)
    {
        if (charType == GC_CHAR_ASIN || charType == GC_CHAR_LIME || charType == GC_CHAR_EDEL || charType == GC_CHAR_BEIGAS || charType == GC_CHAR_UNO)
        {
            applyPromotionBonuses(statusBonus, criticalBonus, iAtk_, iDef_, iHP_, fCritical_);
            break;
        }

        switch (i)
        {
        case 1:
            statusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
            break;
        case 2:
            statusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
            criticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
            break;
        case 3:
            statusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
            criticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
            break;
        }
    }

    if (charType == GC_CHAR_ASIN)
    {
        bool bAbilityUpLv1 = isLocalPlayer ? SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_1) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL1)
            : pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL1);
        bool bAbilityUpLv2 = isLocalPlayer ? SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_2) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL2)
            : pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL2);
        bool bAbilityUpLv3 = isLocalPlayer ? SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_3) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL3)
            : pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL3);

        if (bAbilityUpLv3)
            applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical, iAtk_, iDef_, iHP_, fCritical_);

        if (bAbilityUpLv2)
            applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical, iAtk_, iDef_, iHP_, fCritical_);

        if (bAbilityUpLv1)
            applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status, 0.0f, iAtk_, iDef_, iHP_, fCritical_);
    }
    else if (charType == GC_CHAR_LIME)
    {
        if (isLocalPlayer && SiGCSKT()->IsLearnedSkill(SID_LIME_NORMAL_ABILITY_UP))
        {
            if (charLevel >= 60) applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical, iAtk_, iDef_, iHP_, fCritical_);
            if (charLevel >= 40) applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical, iAtk_, iDef_, iHP_, fCritical_);
            if (charLevel >= 20) applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status, 0.0f, iAtk_, iDef_, iHP_, fCritical_);
        }
    }
    else if (charType == GC_CHAR_UNO)
    {
        if (isLocalPlayer && SiGCSKT()->IsLearnedSkill(SID_UNO_UP_STATS))
        {
            if (charLevel >= 60) applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical, iAtk_, iDef_, iHP_, fCritical_);
            if (charLevel >= 40) applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical, iAtk_, iDef_, iHP_, fCritical_);
            if (charLevel >= 20) applyPromotionBonuses(g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status, 0.0f, iAtk_, iDef_, iHP_, fCritical_);
        }
    }
}

/*void KGCGlobal::GetPromotionBonusStatus(IN const int& iPromotionLevel_, OUT int& iAtk_, OUT int& iDef_, OUT int& iHP_, OUT float& fCritical_, PLAYER* pPlayer_)
{
    if (SiKGCChannelManager()->IsPVPBallanceServer())
        return;

    if ( SiKGCRoomManager()->IsAngelEggModeAbility() ) 
        return;

    if ( SiKGCRoomManager()->IsDotaModeAbility() )
        return;

    // 전직 레벨에 따른 보너스 능력치 표시 
    for(int i = 0 ; i <= iPromotionLevel_ ; ++i )
    {
        if( pPlayer_ != NULL ) {
#ifdef DECANEE
            if( pPlayer_->GetCurrentChar().iCharType == GC_CHAR_ASIN || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_LIME || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_DECANEE || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_EDEL || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_BEIGAS || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_UNO) {
#else
            if (pPlayer_->GetCurrentChar().iCharType == GC_CHAR_ASIN || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_LIME || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_EDEL || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_BEIGAS || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_UNO) {
#endif
                iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
                iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
                iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
                fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
                break;
            }
        }
        
        switch( i )
        {
        case 1:
            iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
            iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
            iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
            fCritical_ += 0.0f;
            break;

        case 2:
            iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
            iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;    
            iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status; 
            fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
            break;

        case 3:
            iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
            iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
            iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status; 
            fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
            break;
        }
    }

    // 여기도 정리하도록 하겠음..........
    if( pPlayer_ != NULL ) {
        if( pPlayer_->GetCurrentChar().iCharType == GC_CHAR_ASIN ) {
            bool bAbilityUpLv1 = false;
            bool bAbilityUpLv2 = false;
            bool bAbilityUpLv3 = false;
            if( pPlayer_->IsLocalPlayer() ) {
                if( SiGCSKT()->IsLearnedSkill( SID_ASIN_HIDDEN_ABILITY_UP ) ) {
                    if( SiGCSKT()->IsLearnedSkill( SID_ASIN_HIDDEN_SKILL_3 ) || SiGCSKT()->IsLearnedSkill( SID_ASIN2_SPECIAL3 ) ) {
                        bAbilityUpLv3 = true;
                    } 

                    if( SiGCSKT()->IsLearnedSkill( SID_ASIN_HIDDEN_SKILL_2 ) || SiGCSKT()->IsLearnedSkill( SID_ASIN2_SPECIAL2 ) ) {
                        bAbilityUpLv2 = true;
                    } 

                    if( SiGCSKT()->IsLearnedSkill( SID_ASIN_HIDDEN_SKILL_1 ) || SiGCSKT()->IsLearnedSkill( SID_ASIN2_SPECIAL1 ) ) {
                        bAbilityUpLv1 = true;
                    }
                } else {
                    if( SiGCSKT()->IsLearnedSkill( SID_ASIN2_SPECIAL3 ) ) {
                        bAbilityUpLv3 = true;
                    } 

                    if( SiGCSKT()->IsLearnedSkill( SID_ASIN2_SPECIAL2 ) ) {
                        bAbilityUpLv2 = true;
                    } 

                    if( SiGCSKT()->IsLearnedSkill( SID_ASIN2_SPECIAL1 ) ) {
                        bAbilityUpLv1 = true;
                    }
                }
            } else {
                if( pPlayer_->IsTrainedSkill( SID_ASIN_HIDDEN_ABILITY_UP ) ) {
                    if( pPlayer_->IsTrainedSkill( SID_ASIN_HIDDEN_SKILL_3 ) || pPlayer_->IsTrainedSkill( SID_ASIN2_SPECIAL3 ) ) {
                        bAbilityUpLv3 = true;
                    } 

                    if( pPlayer_->IsTrainedSkill( SID_ASIN_HIDDEN_SKILL_2 ) || pPlayer_->IsTrainedSkill( SID_ASIN2_SPECIAL2 ) ) {
                        bAbilityUpLv2 = true;
                    } 

                    if( pPlayer_->IsTrainedSkill( SID_ASIN_HIDDEN_SKILL_1 ) || pPlayer_->IsTrainedSkill( SID_ASIN2_SPECIAL1 ) ) {
                        bAbilityUpLv1 = true;
                    }
                } else {
                    if( pPlayer_->IsTrainedSkill( SID_ASIN2_SPECIAL3 ) ) {
                        bAbilityUpLv3 = true;
                    } 

                    if( pPlayer_->IsTrainedSkill( SID_ASIN2_SPECIAL2 ) ) {
                        bAbilityUpLv2 = true;
                    } 

                    if( pPlayer_->IsTrainedSkill( SID_ASIN2_SPECIAL1 ) ) {
                        bAbilityUpLv1 = true;
                    }
                }
            }

            if( bAbilityUpLv3 ) {
                iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
                iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
                iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status; 
                fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
            } 

            if( bAbilityUpLv2 ) {
                iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
                iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;    
                iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status; 
                fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
            } 

            if( bAbilityUpLv1 ) {
                iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
                iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
                iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
                fCritical_ += 0.0f;
            }
        } else if( pPlayer_->GetCurrentChar().iCharType == GC_CHAR_LIME ) {
            if( pPlayer_->IsLocalPlayer() ) {
                if( SiGCSKT()->IsLearnedSkill( SID_LIME_NORMAL_ABILITY_UP ) ) {
                    if( pPlayer_->GetCurrentChar().iLevel >= 60 ) {
                        iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
                        iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
                        iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status; 
                        fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
                    } 

                    if( pPlayer_->GetCurrentChar().iLevel >= 40 ) {
                        iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
                        iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;    
                        iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status; 
                        fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
                    } 

                    if( pPlayer_->GetCurrentChar().iLevel >= 20 ) {
                        iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
                        iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
                        iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
                        fCritical_ += 0.0f;
                    }
                }
            } else {
                if( pPlayer_->IsTrainedSkill( SID_LIME_NORMAL_ABILITY_UP ) ) {
                    if( pPlayer_->GetCurrentChar().iLevel >= 60 ) {
                        iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
                        iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;    
                        iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status; 
                        fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
                    } 

                    if( pPlayer_->GetCurrentChar().iLevel >= 40 ) {
                        iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
                        iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;    
                        iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status; 
                        fCritical_ += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
                    } 

                    if( pPlayer_->GetCurrentChar().iLevel >= 20 ) {
                        iAtk_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
                        iDef_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;    
                        iHP_ += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
                        fCritical_ += 0.0f;
                    }
                }
            }
        }
    }
}
*/
std::wstring KGCGlobal::GetStatusGrade(IN const int iTotalAtk_)
{
    if (SiKGCChannelManager()->IsPVPBallanceServer())
    {
        return L"B";
    }

    if (SiKGCRoomManager()->IsAngelEggModeAbility() ||
        SiKGCRoomManager()->IsDotaModeAbility())
    {
        return L"B";
    }

    std::wstring strGrade;

         if (iTotalAtk_ >= 800000)    strGrade = L"SSS";
    else if (iTotalAtk_ >= 300000)    strGrade = L"SS+";
    else if (iTotalAtk_ >=  45000)    strGrade =  L"SS";
    else if (iTotalAtk_ >=  32600)    strGrade =  L"S+";
    else if (iTotalAtk_ >=  26700)    strGrade =   L"S";
    else if (iTotalAtk_ >=  23700)    strGrade =  L"S-";
    else if (iTotalAtk_ >=  20700)    strGrade =  L"A+";
    else if (iTotalAtk_ >=  17800)    strGrade =   L"A";
    else if (iTotalAtk_ >=  16300)    strGrade =  L"A-";
    else if (iTotalAtk_ >=  14800)    strGrade =  L"B+";
    else if (iTotalAtk_ >=  13300)    strGrade =   L"B";
    else if (iTotalAtk_ >=  11900)    strGrade =  L"B-";
    else if (iTotalAtk_ >=  10400)    strGrade =  L"C+";
    else if (iTotalAtk_ >=   8900)    strGrade =   L"C";
    else if (iTotalAtk_ >=   7400)    strGrade =  L"C-";
    else if (iTotalAtk_ >=   5900)    strGrade =  L"D+";
    else if (iTotalAtk_ >=   4400)    strGrade =   L"D";
    else if (iTotalAtk_ >=   3000)    strGrade =  L"D-";
    else if (iTotalAtk_ >=   1500)    strGrade =   L"E";
    else                              strGrade =   L"F";

    return strGrade;
}

void KGCGlobal::LoadMantisProjectList()
{
    m_mapMantisProjectList.clear();

    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "MantisProjectList.stg" ) == false )
        return;


    std::vector<std::string> vecNation;
    vecNation.push_back("KR");
    vecNation.push_back("BR");
    vecNation.push_back("USA");
    vecNation.push_back("IDN");
    vecNation.push_back("LA");
    vecNation.push_back("TW");
    vecNation.push_back("TH");
    vecNation.push_back("EU");
    vecNation.push_back("COMMON");

    for( int i = 0; i < static_cast<int>( vecNation.size() ); i++ ) {
        std::string strTableName = vecNation[i] + "_PROJECT_LIST";

        LUA_BEGIN_TABLE( strTableName.c_str(), return )
        {
            std::string str="";
            std::vector<std::wstring> vecStr;
            for(int j = 1; ; j++) {
                LUA_GET_VALUE_NOASSERT( j, str , break );
                vecStr.push_back( GCUTIL_STR::GCStrCharToWide( str.c_str() ) );
            }

            m_mapMantisProjectList.insert( 
                std::map<std::wstring, std::vector<std::wstring>>::value_type( GCUTIL_STR::GCStrCharToWide( vecNation[i].c_str() ), vecStr ) );
        }
        LUA_END_TABLE(return);
    }    
}

void SPetTemplate::BindToLua( lua_State* L )
{
    CHECK_STACK(L);
    lua_tinker::class_add<SPetTemplate>( L, "SPetTemplate" );
    lua_tinker::class_mem<SPetTemplate>( L, "iPetID",                   &SPetTemplate::iPetID );
    lua_tinker::class_mem<SPetTemplate>( L, "iPromotion",               &SPetTemplate::iPromotion );
    lua_tinker::class_mem<SPetTemplate>( L, "dwGoodsID",                &SPetTemplate::dwGoodsID );
    lua_tinker::class_mem<SPetTemplate>( L, "fScale",                   &SPetTemplate::fScale );
    lua_tinker::class_mem<SPetTemplate>( L, "fDistX",                   &SPetTemplate::fDistX );
    lua_tinker::class_mem<SPetTemplate>( L, "fDistY",                   &SPetTemplate::fDistY );
    lua_tinker::class_mem<SPetTemplate>( L, "fNameHeight",              &SPetTemplate::fNameHeight );
    lua_tinker::class_mem<SPetTemplate>( L, "iNameID",                  &SPetTemplate::iNameID );
    lua_tinker::class_mem<SPetTemplate>( L, "fResultScale",             &SPetTemplate::fResultScale );
    lua_tinker::class_mem<SPetTemplate>( L, "iMaxEvolutionLevel",       &SPetTemplate::iMaxEvolutionLevel );
    lua_tinker::class_mem<SPetTemplate>( L, "iItemDescID",              &SPetTemplate::iItemDescID );
    lua_tinker::class_mem<SPetTemplate>( L, "fAtkConstant",             &SPetTemplate::fAtkConstant );
    lua_tinker::class_mem<SPetTemplate>( L, "fDefConstant",             &SPetTemplate::fDefConstant );
    lua_tinker::class_mem<SPetTemplate>( L, "fHPConstant",              &SPetTemplate::fHPConstant );
    lua_tinker::class_mem<SPetTemplate>( L, "fEvolutionConst",          &SPetTemplate::fEvolutionConst );
    lua_tinker::class_mem<SPetTemplate>( L, "fAtkDefault",              &SPetTemplate::fAtkDefault );
    lua_tinker::class_mem<SPetTemplate>( L, "fDefDefault",              &SPetTemplate::fDefDefault );
    lua_tinker::class_mem<SPetTemplate>( L, "fHPDefault",               &SPetTemplate::fHPDefault );
    lua_tinker::class_mem<SPetTemplate>( L, "bExistMirrorMotion",       &SPetTemplate::bExistMirrorMotion );
    lua_tinker::class_mem<SPetTemplate>( L, "iTechnique",               &SPetTemplate::iTechnique );
    lua_tinker::class_mem<SPetTemplate>( L, "vAxisCorrection",          &SPetTemplate::vAxisCorrection );
    lua_tinker::class_mem<SPetTemplate>( L, "iTransformEndSound",       &SPetTemplate::iTransformEndSound );

    lua_tinker::class_mem<SPetTemplate>( L, "SkillGP",          &SPetTemplate::SkillGP );
    lua_tinker::class_mem<SPetTemplate>( L, "SkillCash",          &SPetTemplate::SkillCash );
	lua_tinker::class_mem<SPetTemplate>( L, "SkillAwakeningGP",			&SPetTemplate::SkillAwakeningGP );
	lua_tinker::class_mem<SPetTemplate>( L, "SkillAwakeningCash",		&SPetTemplate::SkillAwakeningCash );

    lua_tinker::class_def<SPetTemplate>( L, "PushMeshInfo",             &SPetTemplate::PushMeshInfo  );
    lua_tinker::class_def<SPetTemplate>( L, "PushSoundInfo",            &SPetTemplate::PushSoundInfo  );
    lua_tinker::class_def<SPetTemplate>( L, "PushCostume",              &SPetTemplate::PushCostume  );
    lua_tinker::class_def<SPetTemplate>( L, "PushEffect",               &SPetTemplate::PushEffect  );
    lua_tinker::class_def<SPetTemplate>( L, "PushActionEffect",         &SPetTemplate::PushActionEffect  );
    lua_tinker::class_def<SPetTemplate>( L, "PushRenderOffEffect",      &SPetTemplate::PushRenderOffEffect  );
    lua_tinker::class_def<SPetTemplate>( L, "PushTransformEndEffect",   &SPetTemplate::PushTransformEndEffect  );
    lua_tinker::class_def<SPetTemplate>( L, "PushBoneEffect",           &SPetTemplate::PushBoneEffect  );
    lua_tinker::class_def<SPetTemplate>( L, "AddDesc",                  &SPetTemplate::AddDesc  );
    lua_tinker::class_def<SPetTemplate>( L, "AddMent",                  &SPetTemplate::AddMent  );
    lua_tinker::class_def<SPetTemplate>( L, "SetAxisCorrection",        &SPetTemplate::SetAxisCorrection  );
    lua_tinker::class_def<SPetTemplate>( L, "SetAxisCorrectionX",       &SPetTemplate::SetAxisCorrectionX  );
    lua_tinker::class_def<SPetTemplate>( L, "SetAxisCorrectionY",       &SPetTemplate::SetAxisCorrectionY  );
    lua_tinker::class_def<SPetTemplate>( L, "SetAxisCorrectionZ",       &SPetTemplate::SetAxisCorrectionZ  );
    lua_tinker::class_def<SPetTemplate>( L, "IsAvailableCostume",       &SPetTemplate::IsAvailableCostume  );
    lua_tinker::class_con<SPetTemplate>( L, lua_tinker::constructor<>() );
}

void SPetTemplate::AddDesc( int iState_, int iStringID_ )
{
    mapDescID[iState_] = iStringID_;
}

void SPetTemplate::AddMent( int iState_, int iTime_, int iStringID_ )
{
    mapMentList[iState_].push_back( SPetMent( iTime_, iStringID_ ) );
}

bool SPetTemplate::IsAvailableCostume( GCITEMID ItemID_ )
{
    return std::find( vecCostumeList.begin(), vecCostumeList.end(), ItemID_ ) != vecCostumeList.end();
}

bool KGCGlobal::GetEventMonsterList( KMonsterAbility &kMonster, int iMonsterID )
{
    std::map<int, KMonsterAbility>::iterator mIt = m_mapEventMonsterAbility.find(iMonsterID);
    std::map<int, KMonsterAbility>::iterator mItAll = m_mapEventMonsterAbility.find( -1 );
    if(mIt != m_mapEventMonsterAbility.end()){
        kMonster = mIt->second;
        return true;
    }
    else if(mItAll != m_mapEventMonsterAbility.end() ){
        kMonster = mItAll->second;
        return true;
    }
    return false;
}

bool KGCGlobal::GetMatchingKey( OUT int &iJoystickKey_, int ikeyboardKey_ )
{
    std::map<int,int>::iterator mIt = m_mapMatchingKey.find(ikeyboardKey_);
    if(mIt == m_mapMatchingKey.end())
        return false;    
    iJoystickKey_ = mIt->second;
    return true;
}

void KGCGlobal::InitMatchingKey()
{
    m_mapMatchingKey.clear();

    m_mapMatchingKey.insert(std::pair<int,int>(DIK_ESCAPE,EJK_KEY_SELECT));

    m_mapMatchingKey.insert(std::pair<int,int>(DIK_1,EJK_KEY_1));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_2,EJK_KEY_2));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_3,EJK_KEY_3));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_4,EJK_KEY_4));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_5,EJK_KEY_5));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_6,EJK_KEY_6));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_7,EJK_KEY_7));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_8,EJK_KEY_8));

    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD1,EJK_KEY_1));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD2,EJK_KEY_2));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD3,EJK_KEY_3));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD4,EJK_KEY_4));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD5,EJK_KEY_5));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD6,EJK_KEY_6));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD7,EJK_KEY_7));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPAD8,EJK_KEY_8));

    m_mapMatchingKey.insert(std::pair<int,int>(DIK_BACK,EJK_KEY_SELECT));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_RETURN,EJK_KEY_START));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_NUMPADENTER,EJK_KEY_START));

    m_mapMatchingKey.insert(std::pair<int,int>(DIK_UP,EJK_UP));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_DOWN,EJK_DOWN));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_LEFT,EJK_LEFT));
    m_mapMatchingKey.insert(std::pair<int,int>(DIK_RIGHT,EJK_RIGHT));

    m_mapMatchingKey.insert(std::pair<int,int>(DIK_F9,EJK_KEY_START));
}

bool KGCGlobal::SearchProcess( const std::wstring& strURL )
{
    HINSTANCE hInst = ::ShellExecute( NULL, L"open", L"iexplore", strURL.c_str(), NULL, SW_SHOWNORMAL );
    if ( reinterpret_cast<int>(hInst) <= 32 )
    {
        return false;
    }

    return true;
}

void KGCGlobal::SetSSkillHotKeyUsedInfo( bool bUseHotKey_, char cSkillLevel_ )
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ) {
        return;
    }

    if( bUseHotKey_ ) {
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) {
            CountSSkillHotKeyUsedInfo( m_sSSkillHotKeyStatistics.m_mapDungeonUsedHotkey, cSkillLevel_ );
        } else {
            CountSSkillHotKeyUsedInfo( m_sSSkillHotKeyStatistics.m_mapPVPUsedHotkey, cSkillLevel_ );
        }
    } else {
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) {
            CountSSkillHotKeyUsedInfo( m_sSSkillHotKeyStatistics.m_mapDungeonUsedCharge, cSkillLevel_ );
        } else {
            CountSSkillHotKeyUsedInfo( m_sSSkillHotKeyStatistics.m_mapPVPUsedCharge, cSkillLevel_ );
        }
    }        
}

void KGCGlobal::CountSSkillHotKeyUsedInfo( std::map< char, std::map< char, int > >& mapInfo_, char cSkillLevel_ )
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();

    if( pPlayer && 
        pPlayer->IsLocalPlayer() &&
        !IsQuickSkillPlayer( pPlayer->m_kUserInfo.cCharIndex ) ) {

        std::map< char, std::map< char, int > >::iterator mit = mapInfo_.find( pPlayer->m_kUserInfo.cCharIndex );

        if( mit == mapInfo_.end() ) {
            std::map< char, int > mapTemp;
            mapTemp.insert( std::make_pair( cSkillLevel_, 1 ) );
            mapInfo_.insert( std::make_pair( pPlayer->m_kUserInfo.cCharIndex, mapTemp ) );                    
        } else {
            std::map< char, int >::iterator mitUseCnt = mit->second.find( cSkillLevel_ );
            if( mitUseCnt == mit->second.end() ) {
                mit->second.insert( std::make_pair( cSkillLevel_, 1 ) );
            } else {
                mitUseCnt->second++;
            }
        }
    }    
}

bool KGCGlobal::IsEmptySSkillHotKeyUsedInfo()
{
    if( m_sSSkillHotKeyStatistics.m_mapDungeonUsedCharge.empty() &&
        m_sSSkillHotKeyStatistics.m_mapDungeonUsedHotkey.empty() &&
        m_sSSkillHotKeyStatistics.m_mapPVPUsedCharge.empty() &&
        m_sSSkillHotKeyStatistics.m_mapPVPUsedHotkey.empty() ) {
            return false;
    } else {
        return true;
    }
}

void KGCGlobal::ClearSSkillHotKeyUsedInfo()
{
    m_sSSkillHotKeyStatistics.m_mapDungeonUsedCharge.clear();
    m_sSSkillHotKeyStatistics.m_mapDungeonUsedHotkey.clear();
    m_sSSkillHotKeyStatistics.m_mapPVPUsedCharge.clear();
    m_sSSkillHotKeyStatistics.m_mapPVPUsedHotkey.clear();    
}


std::wstring KGCGlobal::ConvertAtoW( std::string str )
{
	USES_CONVERSION;
	return std::wstring(A2W(str.c_str()));
}

std::string KGCGlobal::ConvertWtoA( std::wstring str )
{
	USES_CONVERSION;
	return std::string(W2A(str.c_str()));
}

#ifdef ENCHANT_EFFECT
bool KGCGlobal::LoadEnchantEffectInfo(const char* strFileName_)
{
    m_mapEnchantEffect.clear();

    KLuaManager luaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, strFileName_) == false) {
        return false;
    }

    if (SUCCEEDED(luaMgr.BeginTable("ENCHANT_EFFECT_GROUP"))) {
        int iEffectGroup = 1;

        while (true) {

            if (FAILED(luaMgr.BeginTable(iEffectGroup))) {
                break;
            }

            EGCEnchantColorGroup  eGroup = static_cast<EGCEnchantColorGroup>(iEffectGroup - 1);
            std::map< EGCEnchantEffectLevel, std::vector< EnchantEffectInfo > > mapInfo;

            int iEffectIndex = 1;
            while (true)
            {

                if (FAILED(luaMgr.BeginTable(iEffectIndex++)))
                {
                    break;
                }

                int iLevel;
                EnchantEffectInfo sInfo;

                LUA_GET_VALUE_DEF(1, iLevel, 9);
                LUA_GET_VALUE_DEF(2, sInfo.strSquence, "");
                LUA_GET_VALUE_DEF(3, sInfo.bRenderGame, true);
                LUA_GET_VALUE_DEF(4, sInfo.dxvGamePos.x, 0.0f);
                LUA_GET_VALUE_DEF(5, sInfo.dxvGamePos.y, 0.0f);
                LUA_GET_VALUE_DEF(6, sInfo.bRenderRoomMy, false);
                LUA_GET_VALUE_DEF(7, sInfo.dxvRoomMyPos.x, 0.0f);
                LUA_GET_VALUE_DEF(8, sInfo.dxvRoomMyPos.y, 0.0f);
                LUA_GET_VALUE_DEF(9, sInfo.bRenderRoomOther, false);
                LUA_GET_VALUE_DEF(10, sInfo.dxvRoomOtherPos.x, 0.0f);
                LUA_GET_VALUE_DEF(11, sInfo.dxvRoomOtherPos.y, 0.0f);
                LUA_GET_VALUE_DEF(12, sInfo.iParticleNum, 0);
                LUA_GET_VALUE_DEF(13, sInfo.fTraceTime, 0.0f);

                EGCEnchantEffectLevel eLevel = static_cast<EGCEnchantEffectLevel>(iLevel);
                std::map< EGCEnchantEffectLevel, std::vector< EnchantEffectInfo > >::iterator mit = mapInfo.find(eLevel);

                if (mit == mapInfo.end())
                {
                    std::vector< EnchantEffectInfo > vecInfo;
                    vecInfo.push_back(sInfo);

                    mapInfo.insert(std::make_pair(eLevel, vecInfo));
                }
                else
                {
                    mit->second.push_back(sInfo);
                }
                luaMgr.EndTable();
            }

            m_mapEnchantEffect.insert(std::make_pair(eGroup, mapInfo));
            luaMgr.EndTable();
            ++iEffectGroup;
        }
    }
    luaMgr.EndTable();

    return true;
}

void KGCGlobal::AddEnchantEffect(const EGCEnchantColorGroup eEffectGruop_, const EGCEnchantEffectLevel eEffectLevel_, PLAYER* pPlayer_, float fX_ /*= 0.0f*/, float fY_ /*= 0.0f*/, int iLayer /*= 0*/, std::vector<CParticleEventSeqPTR>* pVecParticle_ /*= NULL*/, EGCEnchantEffectPos ePos /*= ENCHANT_EFFECT_ROOM_MY*/)
{
    if (NULL == pPlayer_)
    {
        return;
    }

    std::map< EGCEnchantColorGroup, std::map< EGCEnchantEffectLevel, std::vector< EnchantEffectInfo > > >::iterator GroupIter;
    GroupIter = m_mapEnchantEffect.find(eEffectGruop_);
    if (GroupIter == m_mapEnchantEffect.end())
    {
        return;
    }

    std::map< EGCEnchantEffectLevel, std::vector< EnchantEffectInfo > >::iterator LevelIter;
    LevelIter = GroupIter->second.find(eEffectLevel_);
    if (LevelIter == GroupIter->second.end())
    {
        return;
    }

    std::vector< EnchantEffectInfo >::iterator InfoIter = LevelIter->second.begin();

    if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME)
    {
        for (; InfoIter != LevelIter->second.end(); ++InfoIter)
        {
            if (InfoIter->bRenderGame)
            {
                pPlayer_->AddParticleNoDirectionPosWithTrace(const_cast<char*>(InfoIter->strSquence.c_str()), InfoIter->iParticleNum, InfoIter->dxvGamePos.x, InfoIter->dxvGamePos.y, InfoIter->fTraceTime);
            }
        }
    }
    else
    {
        if (g_MyD3D->m_pStateMachine->GetState() != GS_CHAR_SELECT_FROM_ROOM)
        {
            if (ePos == ENCHANT_EFFECT_ROOM_MY)
            {
                for (; InfoIter != LevelIter->second.end(); ++InfoIter)
                {
                    if (InfoIter->bRenderRoomMy)
                    {
                        if (pVecParticle_ != NULL)
                        {
                            pVecParticle_->push_back(g_ParticleManager->EnchantEffect(InfoIter->strSquence, fX_ + InfoIter->dxvRoomMyPos.x, fY_ + InfoIter->dxvRoomMyPos.y, iLayer));
                        }
                    }
                }
            }
            else
            {
                for (; InfoIter != LevelIter->second.end(); ++InfoIter)
                {
                    if (InfoIter->bRenderRoomOther)
                    {
                        if (pVecParticle_ != NULL)
                        {
                            pVecParticle_->push_back(g_ParticleManager->EnchantEffect(InfoIter->strSquence, fX_ + InfoIter->dxvRoomOtherPos.x, fY_ + InfoIter->dxvRoomOtherPos.y, iLayer));
                        }
                    }
                }
            }
        }
    }
}
#endif

void KGCGlobal::ReportforGCLOG(std::string strType )
{
#ifndef __PATH__
	return;
#endif 
#ifndef USE_GC_LOG_SYSTEM
	return;
#endif
    return;
    m_strLogType = strType;

    boost::thread t(boost::bind(&KGCGlobal::threadReportforGCLOG,this));

	return ;
}

void KGCGlobal::threadReportforGCLOG( )
{
    
    CURL *ctx;
    ctx = curl_easy_init();

    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;	

    std::wstring wservice = NATION_CODE;

    //	char url[200] = "http://gc_loggetter.kog.co.kr:7070/report/client.asmx/report"; 
    std::string url = "http://gc_loggetter.kog.co.kr:7070/report/client.asmx/report";


    std::string service = ConvertWtoA(wservice);
    std::string version = m_strLogType;

    std::string time =	GCUTIL_STR::GCStrWideToChar( (LPCTSTR)m_tmServerTime.Format( KNC_TIME_FORMAT ));

    char name[100] = "";
    char ip[100] = "";
    PHOSTENT hostinfo;
    gethostname(name, sizeof(name));
    hostinfo = gethostbyname(name);
    strcpy(ip, inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));



    //서비스 네임
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "sv", CURLFORM_COPYCONTENTS, service.c_str(), CURLFORM_END);
    //버전
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "vi", CURLFORM_COPYCONTENTS, version.c_str(), CURLFORM_END); //새버젼
    //시간
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "tm", CURLFORM_COPYCONTENTS, time.c_str(), CURLFORM_END); //새버젼
    //아이피
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "ip", CURLFORM_COPYCONTENTS, ip, CURLFORM_END); //새버젼


    /* Set the form info */
    curl_easy_setopt(ctx, CURLOPT_URL, (char*)url.c_str());
    curl_easy_setopt(ctx, CURLOPT_HTTPPOST, post);

    CURLcode re =curl_easy_perform(ctx); /* post away! */

    /* free the post data again */
    curl_formfree(post);
    curl_easy_cleanup(ctx);
   
}
namespace GC_GLOBAL_DEFINE
{
    void InitGlobalDefine()
    {
        bReformCoordi = FALSE;
        bWeaponCoordi = FALSE;
        bCoordiCompose = FALSE;
        bCoordiUpgrade = FALSE;
        bRelayHeroDungeon = FALSE;
        bJustInTimeSearch = FALSE;
        strSearchURL = L"about:Tabs";
		strBingoHintURL = L"about:Tabs";

		bGuildLevel7 = FALSE;
		bMonsterCardMix = FALSE;
        bEventAlarm = FALSE;		
        nShopBeginShowTab = 0;			
		bLoadGameModeScript = FALSE;	
        bCheckStageClearTime = FALSE;				
        iLevelItemAlram = 0;
        bAutoMatch4Special = FALSE;	
        bAutoMatchUsePetSpecial = FALSE;
        bUseAutoMatchBalance = FALSE;
        
        bThreadLoadTexture = FALSE;
        bThreadLoadAnimTex = FALSE;
        bThreadLoadBuffTex = FALSE;
        bRealTimeLoadEtcTex = FALSE;
        bRealTimeLoadMotion = FALSE;
        
        nCoordiInven = 0;	
		
        bAutoMatchRanking = FALSE;		
        bAutoMatch = FALSE;

        bMatchBalance2014 = FALSE;  
		
    }

    void LoadGlobalDefine()
    {
        KLuaManager luaMgr;

        if( GCFUNC::LoadLuaScript( luaMgr , "GlobalDefine.stg" ) == false )
            return;
    
        BOOL bTemp = FALSE;
        int nTemp = 0;		
		
        LUA_GET_VALUE_DEF("ReformCoordi", bTemp, FALSE);
        bReformCoordi = bTemp;

        LUA_GET_VALUE_DEF("WeaponCoordi", bTemp, FALSE);
        bWeaponCoordi = bTemp;

        LUA_GET_VALUE_DEF("CoordiCompose", bTemp, FALSE);
        bCoordiCompose = bTemp;

        LUA_GET_VALUE_DEF("CoordiUpgrade", bTemp, FALSE);
        bCoordiUpgrade = bTemp;

        LUA_GET_VALUE_DEF("RelayHeroDungeon", bTemp, FALSE);
        bRelayHeroDungeon = bTemp;

        LUA_GET_VALUE_DEF("JustInTimeSearch", bTemp, FALSE);
        bJustInTimeSearch = bTemp;

        std::string strURL = "";
        LUA_GET_VALUE_DEF("SearchURL", strURL, "about:Tabs" );

        strSearchURL = GCUTIL_STR::GCStrCharToWide( strURL.c_str() );		

		LUA_GET_VALUE_DEF("GuildLevel7", bTemp, FALSE);
		bGuildLevel7 = bTemp;

		strURL = "";
		LUA_GET_VALUE_DEF("BingoHintURL", strURL, "about:Tabs" );

		strBingoHintURL = GCUTIL_STR::GCStrCharToWide( strURL.c_str() );
		
		LUA_GET_VALUE_DEF("MonsterCardMix", bTemp, FALSE);
		bMonsterCardMix = bTemp;

		LUA_GET_VALUE_DEF("LoadGameModeScript", bTemp, FALSE);
		bLoadGameModeScript = bTemp;		

        LUA_GET_VALUE_DEF("bEventAlarm", bTemp, FALSE);
        bEventAlarm = bTemp;	
		
        LUA_GET_VALUE_DEF("ShopBeginShowTab", nTemp, 0);
        nShopBeginShowTab = nTemp;		

        LUA_GET_VALUE_DEF("AD_Shop", nTemp, 0);
        g_kShopAdvertising.SetShopAdType( nTemp );

        LUA_GET_VALUE_DEF("AD_AvatarShop", nTemp, 0);
        g_kShopAdvertising.SetAvatarShopAdType( nTemp );
	    
		LUA_GET_VALUE_DEF("CheckEvent", bTemp, FALSE);
		bCheckStageClearTime = bTemp;

        LUA_GET_VALUE_DEF("LevelItemAlram", nTemp, 0 );
        iLevelItemAlram = nTemp;

        LUA_GET_VALUE_DEF("AutoMatch4Special", bTemp, FALSE );
        bAutoMatch4Special = bTemp;

        LUA_GET_VALUE_DEF("AutoMatchUsePetSpecial", bTemp, FALSE );
        bAutoMatchUsePetSpecial = bTemp;        

        LUA_GET_VALUE_DEF("UseAutoMatchBalance", bTemp, FALSE );
        bUseAutoMatchBalance = bTemp;        
		
        LUA_GET_VALUE_DEF("CoordiInven", nTemp, 0);
        nCoordiInven = nTemp;		

        LUA_GET_VALUE_DEF("bThreadLoadTexture", bTemp, FALSE );
        bThreadLoadTexture = bTemp;

        LUA_GET_VALUE_DEF("bThreadLoadAnimTex", bTemp, FALSE );
        bThreadLoadAnimTex = bTemp;

        LUA_GET_VALUE_DEF("bThreadLoadBuffTex", bTemp, FALSE );
        bThreadLoadBuffTex = bTemp;        

        LUA_GET_VALUE_DEF("bRealTimeLoadEtcTex", bTemp, FALSE );
        bRealTimeLoadEtcTex = bTemp;      

        LUA_GET_VALUE_DEF("bRealTimeLoadMotion", bTemp, FALSE );
        bRealTimeLoadMotion = bTemp;       

        LUA_GET_VALUE_DEF("DangerHPPercent", nTemp, 20);
        nDangerHPPercent = nTemp;


//  해당 디파인 설정 안되어 있으면 무조껀 끈다.        
#if !defined( DEVICE_MULTITHREADED )
        bThreadLoadTexture = FALSE;
        bThreadLoadAnimTex = FALSE;
        bThreadLoadBuffTex = FALSE;
#endif        

        LUA_GET_VALUE_DEF("AutoMatchRanking", bTemp, FALSE );
        bAutoMatchRanking = bTemp;

        LUA_GET_VALUE_DEF("AutoMatch", bTemp, FALSE );
        bAutoMatch = bTemp;

        LUA_GET_VALUE_DEF("MatchBalance2014", nTemp, FALSE);
        bMatchBalance2014 = nTemp;

        return;
    }

    KProtectedType<BOOL> bReformCoordi = FALSE;         //  코디 개편 여부
    KProtectedType<BOOL> bWeaponCoordi = FALSE;         //  무기코디 사용
    KProtectedType<BOOL> bCoordiCompose = FALSE;         // 코디 합성 사용 여부   
    KProtectedType<BOOL> bCoordiUpgrade = FALSE;         // 코디 등급 업그레이드 사용 여부
    KProtectedType<BOOL> bRelayHeroDungeon = FALSE;     //  릴레이 영웅던전
    KProtectedType<BOOL> bJustInTimeSearch = FALSE;     //  JustInTime 이벤트 -> 검색용
    std::wstring         strSearchURL =  L"about:Tabs";
	KProtectedType<BOOL> bGuildLevel7 = FALSE;			//	길드레벨 7단계 수정
	KProtectedType<BOOL> bMonsterCardMix = FALSE;   //  몬스터 카드 조합
    std::wstring         strBingoHintURL =  L"about:Tabs";		//	빙고 힌트 검색 URL
    KProtectedType<BOOL> bEventAlarm = FALSE;  //  이벤트 알람
    KProtectedType<int>  nShopBeginShowTab = 0; 	
	KProtectedType<BOOL> bLoadGameModeScript = FALSE;		//  게임모드 스크립트에서 로드 ( 사내만 동작 )		
    KProtectedType<BOOL> bCheckStageClearTime = FALSE;
    KProtectedType<int>  iLevelItemAlram = 0;       //레벨 달성 아이템 지급 기준 레벨 

    KProtectedType<BOOL> bAutoMatch4Special = FALSE;
    KProtectedType<BOOL> bAutoMatchUsePetSpecial = FALSE;    
    KProtectedType<BOOL> bUseAutoMatchBalance = FALSE;       // 대전매칭 밸런스 대전에서 사용여부
	
    KProtectedType<int> nCoordiInven = 0; //코디 인벤 사이즈
    KProtectedType<BOOL> bThreadLoadTexture = FALSE;	
    KProtectedType<BOOL> bThreadLoadAnimTex = FALSE;
    KProtectedType<BOOL> bThreadLoadBuffTex = FALSE;   
    KProtectedType<BOOL> bRealTimeLoadEtcTex = FALSE;   
    KProtectedType<BOOL> bRealTimeLoadMotion = FALSE;   
    int nDangerHPPercent = 0; //HPPercentage For DangerAlarm

    
	
    KProtectedType<BOOL> bAutoMatchRanking = FALSE;	
    KProtectedType<BOOL> bAutoMatch = FALSE;	

    KProtectedType<BOOL> bMatchBalance2014 = FALSE;
}

namespace GC_GLOBAL_CONST
{
    void InitGlobalConst()
    {
#if defined ( NATION_KOREA )
        fPetMPRecovery = 1.0f;
#else
        fPetMPRecovery = 1.113f;
#endif 
    }

    KProtectedType<float> fPetMPRecovery = 1.0f;
}


void KGCGlobal::SetSelectMotionID()
{
    m_aiSelectMotionID[CID_ELESIS1] = MID_ELESIS_SELECT;
    m_aiSelectMotionID[CID_LIRE1] = MID_LIRE_SELECT;
    m_aiSelectMotionID[CID_ARME1] = MID_ARME_SELECT;
    m_aiSelectMotionID[CID_LAS1] = MID_LAS_SELECT;
    m_aiSelectMotionID[CID_RYAN1] = MID_RYAN_SELECT;
    m_aiSelectMotionID[CID_RONAN1] = MID_RONAN_NOUSE_758;
    m_aiSelectMotionID[CID_AMY1] = MID_AMY1_SELECT;
    m_aiSelectMotionID[CID_JIN1] = MID_JIN_SELECT;
    m_aiSelectMotionID[CID_SIEG1] = MID_SIEG1_SELECT;
    m_aiSelectMotionID[CID_MARI1] = MID_MARI1_SELECT;
    m_aiSelectMotionID[CID_DIO1] = MID_DIO1_SELECT;
    m_aiSelectMotionID[CID_ZERO1] = MID_ZERO1_SELECT;
    m_aiSelectMotionID[CID_LEY1] = MID_LEY1_SELECT;
    m_aiSelectMotionID[CID_LUPUS1] = MID_LUPUS1_SELECT;
    m_aiSelectMotionID[CID_RIN1] = MID_RIN1_SELECT;
    m_aiSelectMotionID[CID_ASIN1] = MID_ASIN1_SELECT;
    m_aiSelectMotionID[CID_LIME1] = MID_LIME1_SELECT;
    m_aiSelectMotionID[CID_EDEL1] = MID_EDEL1_SELECT;
    m_aiSelectMotionID[CID_BEIGAS1] = MID_BEIGAS1_SELECT;
    m_aiSelectMotionID[CID_UNO1] = MID_UNO1_SELECT;
#ifdef PERYTON // Kawan>
    m_aiSelectMotionID[CID_PERYTON1] = MID_PERYTON1_SELECT;
#endif
#ifdef ALICE // KEN>
    m_aiSelectMotionID[CID_ALICE1] = MID_ALICE1_SELECT;
#endif
#ifdef DECANEE 
    m_aiSelectMotionID[CID_DECANEE1] = MID_DECANEE1_SELECT;
#endif
    m_aiSelectMotionID[CID_ELESIS2] = MID_ELESIS2_SELECT;
    m_aiSelectMotionID[CID_LIRE2] = MID_LIRE2_SELECT;
    m_aiSelectMotionID[CID_ARME2] = MID_ARME2_SELECT;
    m_aiSelectMotionID[CID_LAS2] = MID_LAS2_SELECT;
    m_aiSelectMotionID[CID_RYAN2] = MID_RYAN2_SELECT;
    m_aiSelectMotionID[CID_RONAN2] = MID_RONAN2_WAIT_ROOM;
    m_aiSelectMotionID[CID_AMY2] = MID_AMY2_SELECT;
    m_aiSelectMotionID[CID_JIN2] = MID_JIN1_SELECT;
    m_aiSelectMotionID[CID_SIEG2] = MID_SIEG2_SELECT;
    m_aiSelectMotionID[CID_MARI2] = MID_MARI2_SELECT;
    m_aiSelectMotionID[CID_DIO2] = MID_DIO2_SELECT;
    m_aiSelectMotionID[CID_ZERO2] = MID_ZERO2_SELECT;
    m_aiSelectMotionID[CID_LEY2] = MID_LEY2_SELECT;
    m_aiSelectMotionID[CID_LUPUS2] = MID_LUPUS1_SELECT;
    m_aiSelectMotionID[CID_RIN2] = MID_RIN2_SELECT;
    m_aiSelectMotionID[CID_ASIN2] = MID_ASIN2_SELECT;
    m_aiSelectMotionID[CID_LIME2] = MID_LIME2_SELECT;
    m_aiSelectMotionID[CID_EDEL2] = MID_EDEL2_SELECT;
    m_aiSelectMotionID[CID_BEIGAS2] = MID_BEIGAS1_SELECT;
	m_aiSelectMotionID[CID_UNO2] = MID_UNO1_SELECT;
#ifdef PERYTON // Kawan>
    m_aiSelectMotionID[CID_PERYTON2] = MID_PERYTON1_SELECT;
#endif
#ifdef ALICE // KEN>
    m_aiSelectMotionID[CID_ALICE2] = MID_ALICE1_SELECT;
#endif
#ifdef DECANEE 
    m_aiSelectMotionID[CID_DECANEE2] = MID_DECANEE2_SELECT;
#endif
    m_aiSelectMotionID[CID_ELESIS3] = MID_ELESIS3_SELECT;
    m_aiSelectMotionID[CID_LIRE3] = MID_LIRE3_SELECT;
    m_aiSelectMotionID[CID_ARME3] = MID_ARME3_SELECT;
    m_aiSelectMotionID[CID_LAS3] = MID_LAS3_SELECT;
    m_aiSelectMotionID[CID_RYAN3] = MID_RYAN3_SELECT;
    m_aiSelectMotionID[CID_RONAN3] = MID_RONAN3_WAIT_ROOM;
    m_aiSelectMotionID[CID_AMY3] = MID_AMY3_SELECT;
    m_aiSelectMotionID[CID_JIN3] = MID_JIN2_SELECT;
    m_aiSelectMotionID[CID_SIEG3] = MID_SIEG3_SELECT;
    m_aiSelectMotionID[CID_MARI3] = MID_MARI3_SELECT;
    m_aiSelectMotionID[CID_DIO3] = MID_DIO3_SELECT;
    m_aiSelectMotionID[CID_ZERO3] = MID_ZERO3_SELECT;
    m_aiSelectMotionID[CID_LEY3] = MID_LEY3_SELECT;
    m_aiSelectMotionID[CID_LUPUS3] = MID_LUPUS1_SELECT;
    m_aiSelectMotionID[CID_RIN3] = MID_RIN3_SELECT;
    m_aiSelectMotionID[CID_ASIN3] = MID_ASIN1_SELECT;
    m_aiSelectMotionID[CID_LIME3] = MID_LIME1_SELECT;
    m_aiSelectMotionID[CID_EDEL3] = MID_EDEL1_SELECT;
    m_aiSelectMotionID[CID_BEIGAS3] = MID_BEIGAS1_SELECT;
	m_aiSelectMotionID[CID_UNO3] = MID_UNO1_SELECT;
#ifdef PERYTON // Kawan>
    m_aiSelectMotionID[CID_PERYTON3] = MID_PERYTON1_SELECT;
#endif
#ifdef ALICE // KEN>
    m_aiSelectMotionID[CID_ALICE3] = MID_ALICE1_SELECT;
#endif
#ifdef DECANEE 
    m_aiSelectMotionID[CID_DECANEE3] = MID_DECANEE2_SELECT;
#endif
    m_aiSelectMotionID[CID_ELESIS4] = MID_ELESIS4_SELECT;
    m_aiSelectMotionID[CID_LIRE4] = MID_LIRE4_SELECT;
    m_aiSelectMotionID[CID_ARME4] = MID_ARME4_SELECT;
    m_aiSelectMotionID[CID_LAS4] = MID_LAS4_SELECT;
    m_aiSelectMotionID[CID_RYAN4] = MID_RYAN4_SELECT;
    m_aiSelectMotionID[CID_RONAN4] = MID_RONAN4_SELECT;
    m_aiSelectMotionID[CID_AMY4] = MID_AMY4_SELECT;
    m_aiSelectMotionID[CID_JIN4] = MID_JIN4_SELECT;
    m_aiSelectMotionID[CID_SIEG4] = MID_SIEG4_SELECT;
    m_aiSelectMotionID[CID_MARI4] = MID_MARI4_SELECT;
    m_aiSelectMotionID[CID_DIO4] = MID_DIO4_DEVIL_SELECT;
    m_aiSelectMotionID[CID_ZERO4] = MID_ZERO4_SELECT;
    m_aiSelectMotionID[CID_LEY4] = MID_LEY4_SELECT;
    m_aiSelectMotionID[CID_LUPUS4] = MID_LUPUS1_SELECT;
    m_aiSelectMotionID[CID_RIN4] = MID_RIN4_SELECT;
    m_aiSelectMotionID[CID_ASIN4] = MID_ASIN1_SELECT;
    m_aiSelectMotionID[CID_LIME4] = MID_LIME1_SELECT;
    m_aiSelectMotionID[CID_EDEL4] = MID_EDEL1_SELECT;
    m_aiSelectMotionID[CID_BEIGAS4] = MID_BEIGAS1_SELECT;
	m_aiSelectMotionID[CID_UNO4] = MID_UNO1_SELECT;
#ifdef PERYTON // Kawan>
    m_aiSelectMotionID[CID_PERYTON4] = MID_PERYTON1_SELECT;
#endif
#ifdef ALICE // KEN>
    m_aiSelectMotionID[CID_ALICE4] = MID_ALICE1_SELECT;
#endif
#ifdef DECANEE 
    m_aiSelectMotionID[CID_DECANEE4] = MID_DECANEE2_SELECT;
#endif
}

int KGCGlobal::GetCharacterSelectMotionID( int iExtra_Char_Num_ )
{
    if( iExtra_Char_Num_ >= CID_MAX ) {
        return MID_ELESIS_SELECT;
    }

    return m_aiSelectMotionID[iExtra_Char_Num_];
}

float KGCGlobal::GetMaxMP()
{
    if( IsExpantionManaCurChar() ) {
#if defined( LOCK_ASIN_HIDDEN_SKILL )
            return 3.0f;
#else
            return 4.0f;
#endif
        } else {
            return 3.0f;
        } 
}

bool KGCGlobal::IsExpantionManaCurChar()
{
    if( g_MyD3D->GetMyPlayer() ) {
        int iSID = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().SID_EXPANSION_MANA;
        if( SiGCSKT()->IsLearnedSkill( static_cast< EGCSkillTree >( iSID ) ) ) {
            return true;
        }
    }
    return false;    
}

bool KGCGlobal::RunPlayerAllMotion()
{
    if( g_MyD3D->GetMyPlayer() && g_MyD3D->GetMyPlayer()->IsLocalPlayer() == false ) {
        return false;
    }

    if( m_bStartCollectDamageList == false ) {
        return false;
    }

    if( m_iCurReserveIndex >= static_cast< int >( m_vecCurMotionList.size() ) &&
        g_MyD3D->GetMyPlayer()->GetFrameIndex() >= g_MyD3D->GetMyPlayer()->GetFrameNum() - 1) {
        m_iCurReserveIndex = 0;
        m_iPrevCheckMotion = 0;
        m_bStartCollectDamageList = false;

        std::stringstream strFileName;
        strFileName << "CharDamageList_" << GCUTIL_STR::GCStrWideToChar( GCFUNC::GetCharName( static_cast< int >( g_MyD3D->GetMyPlayer()->Extra_Char_Num % GC_CHAR_NUM ) ).c_str() ) 
            << "_" << static_cast< int >( g_MyD3D->GetMyPlayer()->Extra_Char_Num / GC_CHAR_NUM ) << ".txt";

        FILE* pFile = fopen( strFileName.str().c_str(), "wc" );

        std::map< char, std::set< int > >::iterator mit = m_mapPlayerDamageList.find( g_MyD3D->GetMyPlayer()->Extra_Char_Num );
        if( mit != m_mapPlayerDamageList.end() ) {
            for( std::set<int>::iterator sit = mit->second.begin(); sit!= mit->second.end(); ++sit ) {
             fprintf(pFile,"DamageID : %d\n", *sit );
            }        
        }       
        fclose(pFile);

        g_MyD3D->GetMyPlayer()->SetPlayerState( g_MyD3D->GetMyPlayer()->GetWaitMotion() );
        g_MyD3D->GetMyPlayer()->SetFrameIndex( 0 );

        return false;
    }


    if( m_iCurReserveIndex == 0 ) {
        m_bPrevIsRight = g_MyD3D->GetMyPlayer()->GetIsRight();
    }

    if( m_iCurReserveIndex == 0 || 
        g_MyD3D->GetMyPlayer()->GetFrameIndex() >= g_MyD3D->GetMyPlayer()->GetFrameNum() - 1 ||
        m_iPrevCheckMotion != g_MyD3D->GetMyPlayer()->GetCurrentMotion() ) {
        if( m_iCurReserveIndex < static_cast< int >( m_vecCurMotionList.size() ) ) {
            g_MyD3D->GetMyPlayer()->SetPlayerState( m_vecCurMotionList[m_iCurReserveIndex] );
            g_MyD3D->GetMyPlayer()->SetIsRight( m_bPrevIsRight );
            g_MyD3D->GetMyPlayer()->SetFrameIndex( 0 );
            m_iPrevCheckMotion = m_vecCurMotionList[m_iCurReserveIndex];            
            m_iCurReserveIndex++;
        }
        return true;
    }

    return false;
}

void KGCGlobal::SavePlayerMotionDamageList( int iDamageID )
{
    if( g_MyD3D->GetMyPlayer() && g_MyD3D->GetMyPlayer()->IsLocalPlayer() == false ) {
        return;
    }

    std::map< char, std::set< int > >::iterator mit = m_mapPlayerDamageList.find( g_MyD3D->GetMyPlayer()->Extra_Char_Num );

    if( mit != m_mapPlayerDamageList.end() ) {
        mit->second.insert( iDamageID );
    } else {
        std::set< int > setDamage;
        setDamage.insert( iDamageID );
        m_mapPlayerDamageList.insert( std::make_pair( g_MyD3D->GetMyPlayer()->Extra_Char_Num, setDamage ) );
    }    
}

void KGCGlobal::InitDamageCheckMotionInfo()
{
    m_iPrevCheckMotion = 0;
    m_iCurReserveIndex = 0;
    m_bStartCollectDamageList = false;
    m_bPrevIsRight = false;
    m_vecCurMotionList.clear();

    if( !g_MyD3D->GetMyPlayer() ) {
        return;
    }

    for( std::set< int >::iterator sit = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().setStandardCharMotion.begin(); sit != g_MyD3D->GetMyPlayer()->GetCurFormTemplate().setStandardCharMotion.end(); ++sit ) {
        if( *sit == g_MyD3D->GetMyPlayer()->GetCurFormTemplate().sTagTemplate.m_uiStartMotion || 
            *sit == g_MyD3D->GetMyPlayer()->GetFatalWaitMotion()) {
                continue;
        }

        m_vecCurMotionList.push_back( *sit );
    }

    for( std::set< int >::iterator sit = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().setExtendCharMotion.begin(); sit != g_MyD3D->GetMyPlayer()->GetCurFormTemplate().setExtendCharMotion.end(); ++sit ) {
        if( *sit == g_MyD3D->GetMyPlayer()->GetCurFormTemplate().sTagTemplate.m_uiStartMotion ||
            *sit == g_MyD3D->GetMyPlayer()->GetFatalWaitMotion()) {
            continue;
        }

        m_vecCurMotionList.push_back( *sit );
    }
}

void KGCGlobal::CheckParentDamage()
{
#if !defined( __PATH__ ) 
    if( g_MyD3D->GetMyPlayer() ) {        
        std::map< int , CDamage* > mapDamageInfo;
        for( int i = 0; i < static_cast< int >( g_MyD3D->m_pDamageManager->m_vecInfo.size() ); i++ ) {
            if( g_MyD3D->m_pDamageManager->m_vecInfo[i]->m_iReserveParent != -1 ) {
                mapDamageInfo.insert( std::make_pair( i, g_MyD3D->m_pDamageManager->m_vecInfo[i] ) );
            }
        }

        std::set< int > setDamageList = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().setDamageList;
        std::set< int > setParent;
    
        for( std::set< int >::iterator sit = setDamageList.begin(); sit != setDamageList.end(); sit++ ) {
            std::map< int , CDamage* >::iterator mit = mapDamageInfo.find( *sit );

            int iParent = -1;
            if( mit != mapDamageInfo.end() ) {
                if( mit->second->m_iReserveParent == -1 ) {
                    continue;
                } else {
                    iParent = mit->second->m_iReserveParent;
                }

                while(1) {
                    setParent.insert( iParent );

                    std::map< int , CDamage* >::iterator mitLoop = mapDamageInfo.find( iParent );
                    if( mitLoop != mapDamageInfo.end() ) {
                        iParent = mitLoop->second->m_iReserveParent;
                        if( iParent == -1 ) {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }        
        }   

        std::stringstream strFileName;
        strFileName << "CharParentDamageList_" << GCUTIL_STR::GCStrWideToChar( GCFUNC::GetCharName( static_cast< int >( g_MyD3D->GetMyPlayer()->Extra_Char_Num % GC_CHAR_NUM ) ).c_str() ) 
            << "_" << static_cast< int >( g_MyD3D->GetMyPlayer()->Extra_Char_Num / GC_CHAR_NUM ) << ".txt";

        FILE* pFile = fopen( strFileName.str().c_str(), "wc" );

        std::stringstream stm;
        for( std::set<int>::iterator sit = setParent.begin(); sit != setParent.end(); ++sit ) {
            stm << *sit << "\n";
        }
        fprintf(pFile, stm.str().c_str() );
        fclose(pFile);

        ::MessageBoxA( NULL, "부모데미지 수집완료", "", MB_OK );
    }
#endif
}

void KGCGlobal::WriteCollectedFrameData()
{
    if( wcscmp( m_wstmFrame.str().c_str(), L"" ) == 0 ||
        m_bStartFrameCheck == false ) {
        return;
    }

    CreateDirectoryA( "./FrameCheck", NULL );

    time_t curTime = GCUTIL_TIME::GetCurTime();

    char szFileName[1024];
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON ) {
        sprintf(szFileName, "FrameCheck\\FrameCheck_Dungeon_%04d%02d%02d_%02d%02d%02d.txt"
            , GCUTIL_TIME::GetYear(curTime), GCUTIL_TIME::GetMonth(curTime), GCUTIL_TIME::GetDay(curTime)
            , GCUTIL_TIME::GetHour(curTime), GCUTIL_TIME::GetMinute(curTime)
            , GCUTIL_TIME::GetSecond(curTime) );
    } else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH ) {
        sprintf(szFileName, "FrameCheck\\FrameCheck_Match_%04d%02d%02d_%02d%02d%02d.txt"
            , GCUTIL_TIME::GetYear(curTime), GCUTIL_TIME::GetMonth(curTime), GCUTIL_TIME::GetDay(curTime)
            , GCUTIL_TIME::GetHour(curTime), GCUTIL_TIME::GetMinute(curTime)
            , GCUTIL_TIME::GetSecond(curTime) );
    } else {
        sprintf(szFileName, "FrameCheck\\FrameCheck_Other_%04d%02d%02d_%02d%02d%02d.txt"
            , GCUTIL_TIME::GetYear(curTime), GCUTIL_TIME::GetMonth(curTime), GCUTIL_TIME::GetDay(curTime)
            , GCUTIL_TIME::GetHour(curTime), GCUTIL_TIME::GetMinute(curTime)
            , GCUTIL_TIME::GetSecond(curTime) );
    }

    int iPlayerCount = 0;
    for( int i = 0; i < MAX_PLAYER_NUM; i++ ) {
        if( g_MyD3D->MyPlayer[i] && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive ) {
            iPlayerCount++;    
        }
    }

    std::wstringstream strText;
    strText << L"[ Mode ID : " << SiKGCRoomManager()->GetGameMode() << L"  |  CharName : " 
        << GCFUNC::GetCharName( static_cast< int >( g_MyD3D->GetMyPlayer()->Extra_Char_Num % GC_CHAR_NUM ) )
        << L"  |  Promotion : " << static_cast< int >( g_MyD3D->GetMyPlayer()->Extra_Char_Num / GC_CHAR_NUM ) 
        << L"  |  PlayerCount : " << iPlayerCount << L" ]\n" << m_wstmFrame.str().c_str();

    FILE *fo = NULL;
    fo = fopen(szFileName,"a");    

    if ( fo ) 
    {
        fwprintf(fo,L"%s",strText.str().c_str());
        fclose(fo);
    }

    m_wstmFrame.str(L"");
    m_bStartFrameCheck = false;	
}

void KGCGlobal::StartCollectFrameData()
{
    m_wstmFrame.str(L"");
    m_bStartFrameCheck = true;
}

void KGCGlobal::LoadNecklaceScript()
{
    m_vecNecklace.clear();

    KLuaManager luaMgr; 
    
    if( GCFUNC::LoadLuaScript( luaMgr, "ShowRandomNecklace.stg") == false)
        return;

    LUA_BEGIN_TABLE( "RANDOM_NECKLACE", return )
    {
        int iTemp;
        LUA_BEGIN_TABLE( "SHOWRANDOM", return )
        {
            for (int k=1 ;;k++)
            {
                LUA_GET_VALUE_DEF(k, iTemp, -1);
                if (iTemp == -1)
                    break;
                m_vecNecklace.push_back(iTemp);
            }
        }
        LUA_END_TABLE( return )
    }
    LUA_END_TABLE( return )
}

bool KGCGlobal::CheckTrainedSkill( PLAYER* pPlayer, EGCSkillTree eSkill )
{
    if( pPlayer ) {
        return SiGCSKT()->CheckTrainedSkill( pPlayer, eSkill );
    } 

    return false;
}

bool KGCGlobal::IsLearnedSkill( char cCharType, EGCSkillTree eSkill )
{
    return SiGCSKT()->IsLearnedSkill( eSkill );
}

void KGCGlobal::SetPlayerSkillInfo( PLAYER* pPlayer, bool bUnlockAll, bool bSelectableChar, bool bSelectablePromotion )
{
    SiGCSKT()->SetOwnerPlayer( pPlayer );
    SiGCSKT()->SetUnlockAll( bUnlockAll );
    SiGCSKT()->SetCharacterSelectable( bSelectableChar );	
    SiGCSKT()->m_bPromotionSelectable = bSelectablePromotion;   
}

const GCSkill* KGCGlobal::GetSkill( char cCharType, EGCSkillTree eSkill )
{
    return SiGCSKT()->GetSkill( eSkill );
}

int KGCGlobal::GetOpenSkillSlotIndex( char cCharType )
{
    return SiGCSKT()->GetOpenSkillSlotIndex( cCharType );
}

bool KGCGlobal::IsTutorialMode()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ) {
        return true;
    } else if( SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL && g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) {
        return true;
    }
    return false;
}
std::wstring KGCGlobal::GetCharName(const int& iCharType_ )
{
	switch( iCharType_ )
	{
	case GC_CHAR_ELESIS:
		return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ELESIS );
	case GC_CHAR_LIRE:
		return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
	case GC_CHAR_ARME:
		return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
	case GC_CHAR_LAS:
		return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
	case GC_CHAR_RYAN:
		return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
	case GC_CHAR_RONAN:
		return g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
	case GC_CHAR_AMY:
		return g_pkStrLoader->GetString( STR_ID_AMY );
	case GC_CHAR_JIN:
		return g_pkStrLoader->GetString( STR_ID_JIN );
	case GC_CHAR_SIEG:
		return g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
	case GC_CHAR_MARI:
		return g_pkStrLoader->GetString( STR_ID_MARI_NAME );
	case GC_CHAR_DIO:
		return g_pkStrLoader->GetString( STR_ID_DIO_NAME );
	case GC_CHAR_ZERO:
		return g_pkStrLoader->GetString( STR_ID_ZERO_NAME );
	case GC_CHAR_LEY:
		return g_pkStrLoader->GetString( STR_ID_LEY_NAME );
	case GC_CHAR_LUPUS:
		return g_pkStrLoader->GetString( STR_ID_LUPUS_NAME );
	case GC_CHAR_RIN:
		return g_pkStrLoader->GetString( STR_ID_RIN_NAME );
	case GC_CHAR_ASIN:
		return g_pkStrLoader->GetString( STR_ID_ASIN_NAME );
	case GC_CHAR_LIME:
		return g_pkStrLoader->GetString( STR_ID_LIME_NAME );
    case GC_CHAR_EDEL:
        return g_pkStrLoader->GetString( STR_ID_EDEL_NAME );
    case GC_CHAR_BEIGAS:
        return g_pkStrLoader->GetString( STR_ID_BEIGAS_NAME );
    case GC_CHAR_UNO:
        return g_pkStrLoader->GetString( STR_ID_UNO_NAME );
#ifdef PERYTON
    case GC_CHAR_PERYTON:
        return g_pkStrLoader->GetString( STR_ID_PERYTON_NAME );
#endif
#ifdef ALICE
    case GC_CHAR_ALICE:
        return g_pkStrLoader->GetString( STR_ID_ALICE_NAME );
#endif
#ifdef DECANEE
    case GC_CHAR_DECANEE:
        return g_pkStrLoader->GetString(STR_ID_DECANEE_NAME);
#endif
	default:
		return std::wstring();
	}

}

//GachaPong
void KGCGlobal::SetGachaPonMachineInfo(std::map<DWORD, KGachaPongMachineInfo>& mapGachaPonMachineInfo)
{
    m_mapGachaPonMachineInfo.clear();
    m_mapGachaPonMachineInfo.swap(mapGachaPonMachineInfo);
}

void KGCGlobal::UpdateGachaPonMachine(std::pair<DWORD, DWORD>& prMachineInfo, DWORD dwLeftCapsule, DWORD dwRewardCnt)
{
    std::map<DWORD, KGachaPongMachineInfo>::iterator mit = m_mapGachaPonMachineInfo.find(prMachineInfo.first);

    if (mit == m_mapGachaPonMachineInfo.end())
        return;

    m_stGachaPonMachine.m_dwID = prMachineInfo.first;
    m_stGachaPonMachine.m_dwRewardID = prMachineInfo.second;
    m_stGachaPonMachine.m_dwLeftCapsule = dwLeftCapsule;
    m_stGachaPonMachine.m_dwRewardCnt = dwRewardCnt;
}

bool KGCGlobal::GetGachaPonMachineInfo(DWORD dwMachineNumber, KGachaPongMachineInfo& machineInfo)
{
    std::map<DWORD, KGachaPongMachineInfo>::iterator mit = m_mapGachaPonMachineInfo.find(dwMachineNumber);

    if (mit == m_mapGachaPonMachineInfo.end())
        return false;

    machineInfo = mit->second;
    return true;
}

std::wstring KGCGlobal::GetJopName(const int& iCharType_ , const int& iPromotion_ )
{
	//4시간 뒤져榮쨉?방법이 없네요.ㅡ,.ㅡ..String->Enum 변환...
// 	char temp[MAX_PATH];
// 	sprintf( temp, "STR_ID_JOPNAME_%d_%d", iCharType_, iPromotion_);

	switch( iCharType_ )
	{
	case GC_CHAR_ELESIS:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_0_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_0_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_0_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_0_3 );
		}
	case GC_CHAR_LIRE:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_1_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_1_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_1_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_1_3 );
		}
	case GC_CHAR_ARME:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_2_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_2_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_2_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_2_3 );
		}
	case GC_CHAR_LAS:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_3_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_3_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_3_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_3_3 );
		}
	case GC_CHAR_RYAN:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_4_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_4_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_4_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_4_3 );
		}
	case GC_CHAR_RONAN:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_5_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_5_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_5_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_5_3 );
		}
	case GC_CHAR_AMY:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_6_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_6_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_6_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_6_3 );
		}
	case GC_CHAR_JIN:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_7_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_7_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_7_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_7_3 );
		}
	case GC_CHAR_SIEG:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_8_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_8_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_8_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_8_3 );
		}
	case GC_CHAR_MARI:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_9_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_9_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_9_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_9_3 );
		}
	case GC_CHAR_DIO:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_10_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_10_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_10_2 );
		case 3:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_10_3 );
        default:
			return std::wstring();
		}
	case GC_CHAR_ZERO:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_11_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_11_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_11_2 );
		case 3:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_11_3 );
        default:
			return std::wstring();
		}
	case GC_CHAR_LEY:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_12_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_12_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_12_2 );
		case 3:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_12_3 );
			return std::wstring();
		}
	case GC_CHAR_LUPUS:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_13_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_13_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_13_2 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_13_3 );
		}
	case GC_CHAR_RIN:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_14_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_14_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_14_2 );
        case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_14_3 );
		}
	case GC_CHAR_ASIN:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_15_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_15_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_15_0 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_15_0 );
		}
	case GC_CHAR_LIME:
		switch(iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_16_0 );
		case 1:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_16_1 );
		case 2:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_16_0 );
		case 3:
			return g_pkStrLoader->GetString( STR_ID_JOPNAME_16_0 );
		}
    case GC_CHAR_EDEL:
        switch(iPromotion_)
        {
        case 0:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_17_0 );
        case 1:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_17_1 );
        case 2:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_17_0 );
        case 3:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_17_0 );
        }
    case GC_CHAR_BEIGAS:
        switch(iPromotion_)
        {
        case 0:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_18_0 );
        case 1:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_18_0 );
        case 2:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_18_0 );
        case 3:
            return g_pkStrLoader->GetString( STR_ID_JOPNAME_18_0 );
        }
	case GC_CHAR_UNO:
		switch (iPromotion_)
		{
		case 0:
			return g_pkStrLoader->GetString(STR_ID_JOPNAME_19_0);
		case 1:
			return g_pkStrLoader->GetString(STR_ID_JOPNAME_19_0);
		case 2:
			return g_pkStrLoader->GetString(STR_ID_JOPNAME_19_0);
		case 3:
			return g_pkStrLoader->GetString(STR_ID_JOPNAME_19_0);
		}
#ifdef PERYTON // Kawan>
    case GC_CHAR_PERYTON:
        switch (iPromotion_)
        {
        case 0:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_20_0);
        case 1:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_20_0);
        case 2:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_20_0);
        case 3:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_20_0);
        }
#endif
#ifdef ALICE // KEN>
    case GC_CHAR_ALICE:
        switch (iPromotion_)
        {
        case 0:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_21_0);
        case 1:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_21_0);
        case 2:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_21_0);
        case 3:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_21_0);
        }
#endif
#ifdef DECANEE // KEN>
    case GC_CHAR_DECANEE:
        switch (iPromotion_)
        {
        case 0:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_22_0);
        case 1:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_22_1);
        case 2:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_22_0);
        case 3:
            return g_pkStrLoader->GetString(STR_ID_JOPNAME_20_0);
        }
#endif
    }
	return std::wstring();
}

std::wstring KGCGlobal::GetRankScoreName(const int& iRank_)
{
    switch (iRank_)
    {
    case 1: return L"D";
    case 2: return L"D+";
    case 3: return L"C";
    case 4: return L"C+";
    case 5: return L"B";
    case 6: return L"B+";
    case 7: return L"A";
    case 8: return L"A+";
    case 9: return L"S";
    case 10: return L"S+";
    case 11: return L"SS";
    }
    return L"";
}

int KGCGlobal::GetCharDifficuleLevel(const int& iCharType_ )
{
	EGCCharType eChar = static_cast<EGCCharType>(iCharType_);

	switch( eChar ) { 
		case GC_CHAR_ELESIS:
			return 0;
		case GC_CHAR_LIRE:
			return 2;
		case GC_CHAR_ARME:
			return 1;
		case GC_CHAR_LAS:
			return 2;
		case GC_CHAR_RYAN:
			return 2;
		case GC_CHAR_RONAN:
			return 0;
		case GC_CHAR_AMY:
			return 1;
		case GC_CHAR_JIN:
			return 1;
		case GC_CHAR_SIEG:
			return 1;
		case GC_CHAR_MARI:
			return 1;
		case GC_CHAR_DIO:
			return 2;
		case GC_CHAR_ZERO:
			return 2;
		case GC_CHAR_LEY:
			return 1;
		case GC_CHAR_LUPUS:
			return 0;
		case GC_CHAR_RIN:
			return 1;
		case GC_CHAR_ASIN:
			return 0;
		case GC_CHAR_LIME:
			return 0;
        case GC_CHAR_EDEL:
            return 0;
        case GC_CHAR_BEIGAS:
            return 1;
		case GC_CHAR_UNO:
			return 1;
#ifdef PERYTON // Kawan>
        case GC_CHAR_PERYTON:
            return 1;
#endif
#ifdef ALICE // KEN>
        case GC_CHAR_ALICE:
            return 1;
#endif
#ifdef DECANEE // KEN>
        case GC_CHAR_DECANEE:
            return 0;
#endif
		default:
			return -1;
	}

}

std::wstring KGCGlobal::GetCharDesc( const int& iCharType_ )
{
	EGCCharType eChar = static_cast<EGCCharType>(iCharType_);

	switch( eChar ) { 
		case GC_CHAR_ELESIS:
			return g_pkStrLoader->GetString( STR_ID_ELESIS_INTRODUCE);
		case GC_CHAR_LIRE:
			return g_pkStrLoader->GetString( STR_ID_LIRE_INTRODUCE );
		case GC_CHAR_ARME:
			return g_pkStrLoader->GetString( STR_ID_ARME_INTRODUCE );
		case GC_CHAR_LAS:
			return std::wstring();
		case GC_CHAR_RYAN:
			return std::wstring();
		case GC_CHAR_RONAN:
			return std::wstring();
		case GC_CHAR_AMY:
			return std::wstring();
		case GC_CHAR_JIN:
			return std::wstring();
		case GC_CHAR_SIEG:
			return g_pkStrLoader->GetString( STR_ID_SIEG_INTRODUCE );
		case GC_CHAR_MARI:
			return std::wstring();
		case GC_CHAR_DIO:
			return std::wstring();
		case GC_CHAR_ZERO:
			return std::wstring();
		case GC_CHAR_LEY:
			return std::wstring();
		case GC_CHAR_LUPUS:
			return g_pkStrLoader->GetString( STR_ID_LUPUS_INTRODUCE );
		case GC_CHAR_RIN:
			return g_pkStrLoader->GetString( STR_ID_RIN_INTRODUCE );
		case GC_CHAR_ASIN:
			return std::wstring();
        case GC_CHAR_EDEL:
            return std::wstring();
        case GC_CHAR_BEIGAS:
            return std::wstring();
        case GC_CHAR_UNO:
            return std::wstring();
#ifdef PERYTON // Kawan>
        case GC_CHAR_PERYTON:
            return  std::wstring();
#endif
#ifdef ALICE // KEN>
        case GC_CHAR_ALICE:
            return  std::wstring();
#endif
#ifdef DECANEE // KEN>
        case GC_CHAR_DECANEE:
            return  std::wstring();
#endif
		default:
			return std::wstring();
	}
}

DWORD KGCGlobal::GetCharOpenType(const int& iCharType_ )
{
	std::map<int,DWORD>::iterator mit = m_mapCharOpenType.find(iCharType_);

	if (mit != m_mapCharOpenType.end())
		return mit->second;
	else
		return 0;	//테이블에 정의 되어 있지 않다면 무료 배포 케릭터로 체크 한다.
}

void KGCGlobal::AddShutDownMsg( const KShutdownInfo::EShutdownInfo eType_, const unsigned int iTime_ )
{
    switch( eType_ ) { 
        case KShutdownInfo::FORCED_SHUTDOWN_SYSTEM:
            switch( iTime_ ) { 
                case 60:    // 60분
                    if( g_pkChatManager ) { 
                        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_FORCE_SHUTDOWN_MSG_60MIN ), KGCChatManager::CHAT_TYPE_ALERT );
                    }
                break;
                case 30:    // 30분
                    if( g_pkChatManager ) { 
                    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_FORCE_SHUTDOWN_MSG_30MIN ), KGCChatManager::CHAT_TYPE_ALERT );
                    }
                break;
                default:
#if !defined( __PATH__ )
                    if( g_pkChatManager ) { 
                        std::wostringstream str;
                        str << L"셧다운 예고 메시지!!!  종료" << iTime_ << L"분전!!!!" ;
                        g_pkChatManager->AddChatMsg( str.str(), KGCChatManager::CHAT_TYPE_ALERT );
                    }
#endif
                break;
            }
        break;

        case KShutdownInfo::SELECTIVE_SHUTDOWN_SYSTEM:
            switch( iTime_ ) { 
                case 60:    // 60분
                    if( g_pkChatManager ) { 
                        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_SELECTIVE_SHUTDOWN_MSG_60MIN ), KGCChatManager::CHAT_TYPE_ALERT );
                    }
                break;
                case 30:    // 30분
                    if( g_pkChatManager ) { 
                        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_SELECTIVE_SHUTDOWN_MSG_30MIN ), KGCChatManager::CHAT_TYPE_ALERT );
                    }
                break;
                default:
#if !defined( __PATH__ )
                    if( g_pkChatManager ) { 
                        std::wostringstream str;
                        str << L"셧다운 예고 메시지!!!  종료" << iTime_ << L"분전!!!!" ;
                        g_pkChatManager->AddChatMsg( str.str(), KGCChatManager::CHAT_TYPE_ALERT );
                    }
#endif
                break;
            }
       break;

        default:
            break;
    }
}

int KGCGlobal::GetShutDownInfoType( const int iShutdownCheckResult_ )
{
    switch( iShutdownCheckResult_ ) { 
        case KShutdownInfo::FORCED_SHUTDOWN_TIME_OUT:
            return KShutdownInfo::FORCED_SHUTDOWN_SYSTEM;
        case KShutdownInfo::SELECTIVE_SHUTDOWN_NOT_ALLOW_TIME:
            return KShutdownInfo::SELECTIVE_SHUTDOWN_SYSTEM;
    }
    return -1;
}

void KGCGlobal::LoadCharGainParticleInfo( const char* strFileName_ )
{
	m_mapCharGainParticleInfo.clear();

	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, strFileName_ ) == false ) { 
		return ;
	}

	LUA_BEGIN_TABLE("ParticleInfo",return);
	{
		for(int i = 1; ; i++)
		{
			int iCharType = -1;
			std::vector<std::string> vecString;
			LUA_BEGIN_TABLE(i,break)
			{
				LUA_GET_VALUE("Char",iCharType,return);
				LUA_BEGIN_TABLE("ParticleName",return);
				for (int j=1 ; ; j++)
				{
					std::string strName;
					LUA_GET_VALUE_NOASSERT( j, strName, break );
					vecString.push_back(strName);
				}
				LUA_END_TABLE(return)
			}
			LUA_END_TABLE(return)
			if (iCharType != -1)
				m_mapCharGainParticleInfo.insert(std::make_pair(iCharType,vecString));
		}
	}
	LUA_END_TABLE(return)

}

bool KGCGlobal::IsSuccessRecvInventoryPacket( int iCurrentCnt )
{
	if ( m_iInventoryPacketMaxCnt < 0 )
		return false;

	if ( iCurrentCnt != m_iInventoryPacketMaxCnt )
		return false;

	return true;
}	


bool KGCGlobal::IsSuccessRecvInventoryPacket( )
{
	if ( m_iInventoryPacketMaxCnt != 0 )
		return false;

	return true;
}

void KGCGlobal::SetHideSceneMenuBtn( const std::set<int>& setHideSceneMenuBtn )
{
	m_setHideSceneMenuBtn = setHideSceneMenuBtn;
}

bool KGCGlobal::IsHideSceneMenuBtn( int nHideType )
{
	return ( m_setHideSceneMenuBtn.find( nHideType ) != m_setHideSceneMenuBtn.end() );
}	

int KGCGlobal::GetSingleGachaVer( void )
{
	if ( false == EnableGacha() ) {
		return 0;
	}

	std::vector< KGachaNewList >::const_iterator it = m_vecGachaUseVersion.begin();
	return it->m_nID;
}

bool KGCGlobal::GetPingData( std::string strIP, DWORD& dwTime )
{
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char  SendData[] = "Data Buffer";
    char  szReplyBuffer[1500];

    ipaddr = inet_addr(strIP.c_str());
    if( ipaddr == INADDR_NONE )
    {
//      printf("usage: %s IP address\n", szIP);
        return false;
    }

    hIcmpFile = IcmpCreateFile();
    if( hIcmpFile == INVALID_HANDLE_VALUE )
    {
//        printf("\tUnable to open handle.\n");
//        printf("IcmpCreatefile returned error: %ld\n", GetLastError() );
        return false;
    }    

    dwRetVal = IcmpSendEcho( hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, szReplyBuffer, sizeof(szReplyBuffer), 1000 );
    if( dwRetVal != 0 )
    {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)szReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;

//         printf("\t  Received from %s\n", inet_ntoa( ReplyAddr ) );
//         printf("\t  Status = %ld\n", pEchoReply->Status);
//         printf("\t  Roundtrip time = %ld milliseconds\n", pEchoReply->RoundTripTime);

        dwTime = pEchoReply->RoundTripTime;

        return true;
    }
    else
    {
        return false;
    }

    return true;
}

PLAYER_ATTACK_ATTRIBUTE KGCGlobal::GetSpearType( const int iPlayerIndex_ )
{
    if( g_MyD3D->MyPlayer[ iPlayerIndex_ ] == NULL ) { 
        return PAA_NORMAL;
    }

    if( g_MyD3D->MyPlayer[ iPlayerIndex_ ]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] <= 0.0f ) { 
        return PAA_NORMAL;
    }

#if defined( USE_GOLD_SPEAR ) 
    return PAA_GOLDSPEAR;
#else
    if( true == g_pItemMgr->IsEquipMonsterCard( g_MyD3D->MyPlayer[ iPlayerIndex_ ]->m_kUserInfo.GetCurrentChar(), KGCItemManager::ITEM_RED_SPEAR_MONSTER_CARD ) ) { 
        return PAA_REDSPEAR;
    }
    else {
        return PAA_DARKSPEAR;
    }
#endif
}

void KGCGlobal::SetSpearEffefct( LTP_BASIC* pInfo_ )
{
    if( pInfo_ == NULL ) {
        return;
    }
        
    LTP_CHANGE_HP_INFO* Custom = (LTP_CHANGE_HP_INFO*)pInfo_;
    if( Custom->dwAttackType & PAA_DARKSPEAR ) { 
        g_KDSound.Play( "81022" );
        if( Custom->iTargetIndex < 6 )
        {        
            g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticle("DarkSpear_01",0,0.0f);			
            g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticle("DarkSpear_02",0,0.0f);
            if(g_MyD3D->MyPlayer[Custom->iTargetIndex]->bIsRight)
            {
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_03",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_04",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_05",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_06",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_07",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_08",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_09",0,0.0f);
            }
            else
            {
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_03_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_04_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_05_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_06_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_07_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_08_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("DarkSpear_09_R",0,0.0f);
            }
        }
        else
        {     
            MONSTER* pMonster = g_kMonsterManager.GetMonster( Custom->iTargetIndex-6 );
            if(pMonster != NULL ) {
                pMonster->AddParticleNoDirection("DarkSpear_01",0.0f,0.0f);			
                pMonster->AddParticleNoDirection("DarkSpear_02",0.0f,0.0f);
                if(g_MyD3D->MyPlayer[Custom->iAttackerIndex]->bIsRight)
                {
                    pMonster->AddParticleNoDirection("DarkSpear_03",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_04",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_05",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_06",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_07",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_08",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_09",0.0f,0.0f);
                }
                else
                {
                    pMonster->AddParticleNoDirection("DarkSpear_03_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_04_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_05_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_06_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_07_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_08_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("DarkSpear_09_R",0.0f,0.0f);
                }
            }
        }
    }
    else if( Custom->dwAttackType & PAA_GOLDSPEAR ) { 
        g_KDSound.Play( "81022" );
        if( Custom->iTargetIndex < 6 )
        {        
            g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticle("HSpear_01",0,0.0f);			
            g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticle("HSpear_02",0,0.0f);
            if(g_MyD3D->MyPlayer[Custom->iTargetIndex]->bIsRight)
            {
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_03",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_04",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_05",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_06",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_07",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_08",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_09",0,0.0f);
            }
            else
            {
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_03_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_04_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_05_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_06_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_07_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_08_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("HSpear_09_R",0,0.0f);
            }
        }
        else
        {     
            MONSTER* pMonster = g_kMonsterManager.GetMonster( Custom->iTargetIndex-6 );
            if(pMonster != NULL ) {
                pMonster->AddParticleNoDirection("HSpear_01",0.0f,0.0f);			
                pMonster->AddParticleNoDirection("HSpear_02",0.0f,0.0f);
                if(g_MyD3D->MyPlayer[Custom->iAttackerIndex]->bIsRight)
                {
                    pMonster->AddParticleNoDirection("HSpear_03",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_04",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_05",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_06",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_07",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_08",0.0f,0.0f);                           
                    pMonster->AddParticleNoDirection("HSpear_09",0.0f,0.0f);                        }
                else
                {
                    pMonster->AddParticleNoDirection("HSpear_03_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_04_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_05_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_06_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_07_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_08_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("HSpear_09_R",0.0f,0.0f);
                }
            }
        }
    }
    else if( Custom->dwAttackType & PAA_REDSPEAR ) { 
        g_KDSound.Play( "81022" );
        if( Custom->iTargetIndex < 6 )
        {        
            g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticle("RedSpear_01",0,0.0f);			
            g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticle("RedSpear_02",0,0.0f);
            if(g_MyD3D->MyPlayer[Custom->iTargetIndex]->bIsRight)
            {
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_03",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_04",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_05",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_06",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_07",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_08",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_09",0,0.0f);
            }
            else
            {
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_03_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_04_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_05_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_06_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_07_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_08_R",0,0.0f);
                g_MyD3D->MyPlayer[Custom->iTargetIndex]->AddParticleNoDirection("RedSpear_09_R",0,0.0f);
            }
        }
        else
        {     
            MONSTER* pMonster = g_kMonsterManager.GetMonster( Custom->iTargetIndex-6 );
            if(pMonster != NULL ) {
                pMonster->AddParticleNoDirection("RedSpear_01",0.0f,0.0f);			
                pMonster->AddParticleNoDirection("RedSpear_02",0.0f,0.0f);
                if(g_MyD3D->MyPlayer[Custom->iAttackerIndex]->bIsRight)
                {
                    pMonster->AddParticleNoDirection("RedSpear_03",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_04",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_05",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_06",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_07",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_08",0.0f,0.0f);                           
                    pMonster->AddParticleNoDirection("RedSpear_09",0.0f,0.0f);                        }
                else
                {
                    pMonster->AddParticleNoDirection("RedSpear_03_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_04_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_05_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_06_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_07_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_08_R",0.0f,0.0f);
                    pMonster->AddParticleNoDirection("RedSpear_09_R",0.0f,0.0f);
                }
            }
        }
    }
}

bool KGCGlobal::IsVestedItem( const GCITEMID itemID_ )
{
    if( m_vecVestedItemList.empty() ) { 
        return false;
    }

    std::vector< GCITEMID >::iterator it = std::find( m_vecVestedItemList.begin(), m_vecVestedItemList.end(), itemID_ * 10 );
    return ( it != m_vecVestedItemList.end() );
}

bool KGCGlobal::IsInEscortMonsterInfo( DWORD dwMonserID_ )
{
    std::map<DWORD,KGlobalEscortInfo>::const_iterator cmit = m_mapEscortInfo.find( dwMonserID_ );
    if ( cmit == m_mapEscortInfo.end() )
    {
        m_kEscortInfo = KGlobalEscortInfo();
        return false;
    }
    else
    {
        m_kEscortInfo = cmit->second;
        return true;
    }
}

std::vector<std::wstring> KGCGlobal::GetLoadingFileName()
{
    if ( m_mapLoadingImageName.empty() )
    {
        return std::vector<std::wstring>();
    }
    else
    {
        GCUTIL::GCRand_Int gcRandInt;

        int iRandSeed = gcRandInt.rand()%m_mapLoadingImageName.size();

        std::map< int, std::vector< std::wstring > >::iterator mit = m_mapLoadingImageName.begin();

        for (int i=0 ; i<iRandSeed ; i++)
        {
            ++mit;

            //안전장치...
            if ( mit == m_mapLoadingImageName.end() )
            {
                --mit;
                break;
            }  
        }

        return mit->second;
    }
}

void KGCGlobal::SetAttributeMigrationInfo( const KEVENT_ATTRIBUTE_MIGRATION_INFO_ACK& Info_ )
{
    m_kAttributeMigrationInfo.m_bEventEnable = false;
    m_kAttributeMigrationInfo.m_dwChangePossbileCount = 0;
    m_kAttributeMigrationInfo.m_setChangeAttributeList.clear();
    m_kAttributeMigrationInfo.m_ItemID = 0;
    m_kAttributeMigrationInfo.m_vecAttributeList.clear();

    m_kAttributeMigrationInfo.m_bEventEnable = Info_.m_bEventEnable;
    m_kAttributeMigrationInfo.m_dwChangePossbileCount = Info_.m_dwChangePossbileCount;
    m_kAttributeMigrationInfo.m_setChangeAttributeList = Info_.m_setChangeAttributeList;
    m_kAttributeMigrationInfo.m_ItemID = Info_.m_ItemID;
    m_kAttributeMigrationInfo.m_vecAttributeList = Info_.m_vecAttributeList;
}

bool KGCGlobal::IsEnableAttributeMigration( void )
{
    if( m_kAttributeMigrationInfo.m_bEventEnable ) {    // 무료 이벤트 기간 
        return true;
    }

    if( static_cast<int>( m_kAttributeMigrationInfo.m_dwChangePossbileCount ) - ATTRIBUTE_SELECT_MAX_COUNT < 0 ) {  // 변환 가능 카운트 있음 
        return true;
    }

    return false;
}

bool KGCGlobal::GetMigrationChangeEnableAttributeList( OUT std::set< int >& List_ )
{
    List_.clear();

    if( IsEnableAttributeMigration() ) { 
        List_ = m_kAttributeMigrationInfo.m_setChangeAttributeList;
        return true;
    }
    else { 
        return false;
    }
}

bool KGCGlobal::GetMigrationItemCurrentAttribute( OUT std::vector< KAttributeInfo >& Info_ )
{
    Info_.clear();
    if( IsEnableAttributeMigration() ) { 
        Info_ = m_kAttributeMigrationInfo.m_vecAttributeList;
        return true;
    }
    else { 
        return false;
    }
}

bool KGCGlobal::IsOneWeaponCharacter( const int iCharType_ )
{
    if( iCharType_ < 0 || iCharType_ >= GC_CHAR_NUM ) { 
        return false;
    }

    if ( g_kGlobalValue.IsQuickSkillPlayer( iCharType_ ) || 
        g_kGlobalValue.IsHybridPlayer( iCharType_ ) )
    {
        return true;
    }

    return false;
}

bool KGCGlobal::RunExecute( std::string filename, std::string param )
{

    _SHELLEXECUTEINFOA execinfo;
    ZeroMemory(&execinfo,sizeof(_SHELLEXECUTEINFOA));
    execinfo.cbSize = sizeof(_SHELLEXECUTEINFOA);
    execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    execinfo.nShow = SW_SHOW;
    execinfo.lpVerb = "open";
    execinfo.lpFile = filename.c_str();
    execinfo.lpParameters = param.c_str();

    BOOL bExcueSuccess = ShellExecuteExA( &execinfo ); 
    if(bExcueSuccess == TRUE)
    {
        return true;
    }

    return false;
}

bool KGCGlobal::RunLogProgram( std::string arg, std::string NPerr /*= "0"*/ )
{
#if !defined(NATION_CHINA) || !defined(__PATH__)
    return true;
#endif
    std::string strCmdLine;

    std::string strFile("cu_log.exe ");

    USES_CONVERSION;
    std::string ver = std::string(W2A(m_wstrVersion.c_str()));

    strCmdLine.append( ver.empty() ? "0": ver );
    strCmdLine.append( " ");
    strCmdLine.append( arg );

    if ( !arg.compare( "19" ) )
    {
        strCmdLine.append( " ");
        strCmdLine.append( NPerr );
    }

    if(RunExecute(strFile,strCmdLine) == false)
    {
        START_LOG( cout,  L"Running GrandChase Failed." );
        return false;
    }
    START_LOG( cout,  L"GrandChase was successfully started." );
    return true;
}

void KGCGlobal::SetSmartPanelMailList( const bool bSmartPanelMailListEnable, const std::vector<std::wstring> vecSmartPanelMailList )
{
    m_bSmartPanelMailListEnable = bSmartPanelMailListEnable;
    m_vecSmartPanelMailList = vecSmartPanelMailList;
}

// 활력 시스템
int KGCGlobal::GetVitality( IN int nCharType )
{
    std::map< int, int >::iterator iter = m_kVitalityInfo.m_mapRemainVitality.find( nCharType );

    if( iter != m_kVitalityInfo.m_mapRemainVitality.end() ) {
        return (*iter).second;
    }

    return 0;
}

int KGCGlobal::GetMaxVitality()
{
    return m_kVitalityInfo.m_nMaxVitalityPerDay;
}

int KGCGlobal::GetVitalityRemainRechargePoint()
{
    return m_kVitalityInfo.m_nRechargePoint;
}

void KGCGlobal::SetUpdateVitalityInfo( IN KEVENT_VITALITY_INFO_NOT& kRedv_ )
{
    m_kVitalityInfo.m_mapRemainVitality = kRedv_.m_mapRemainVitality;
    m_kVitalityInfo.m_nMaxVitalityPerDay = kRedv_.m_nMaxVitalityPerDay;
    m_kVitalityInfo.m_nMaxVitalityForFirstCharacter = kRedv_.m_nMaxVitalityForFirstCharacter;
    m_kVitalityInfo.m_nRechargePoint = kRedv_.m_nRechargePoint;
}

bool KGCGlobal::UseVitalitySystem()
{
    return !m_kVitalityInfo.m_mapRemainVitality.empty();
}

int KGCGlobal::GetCurrentCharacterGP( void )
{
    return m_kUserInfo.GetCurrentChar().GetCurrentGP();
}

int KGCGlobal::GetEnableAttributeMigrationCount( void )
{
    return abs( static_cast<int>( m_kAttributeMigrationInfo.m_dwChangePossbileCount ) - ATTRIBUTE_SELECT_MAX_COUNT );
}

void KGCGlobal::LoadMoveModelData()
{
    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "MoveModelInfo.lua") == false)
        return;

    LUA_GET_VALUE_DEF("NUM_MOVE_MODEL", g_kGlobalValue.m_nMoveModelCount, 1);
    LUA_GET_VALUE_DEF("START_MODELID_SERVER_STATE", g_kGlobalValue.m_iStartModelIdServerState, 1);

    g_kGlobalValue.m_mapMoveModelData.clear();

    LUA_BEGIN_TABLE_RAII("MOVE_MODEL_DATA")
    {
        for (int i = 1; ; ++i)
        {
            MoveModelData ModelData;

            LUA_BEGIN_TABLE_RAII(i)
            {
                LUA_GET_VALUE_DEF("MODELID", ModelData.m_NPCID, 0);
                LUA_GET_VALUE_DEF("POS_X", ModelData.m_fPosX, 0.0f);
                LUA_GET_VALUE_DEF("POS_Y", ModelData.m_fPosY, 0.0f);
                LUA_GET_VALUE_DEF("SCALE", ModelData.m_fScale, 1.0f);
                LUA_GET_VALUE_DEF("USE_EVENT", ModelData.m_bUseEvent, 0);
                LUA_GET_VALUE_DEF("START_X", ModelData.m_fStartX, -1.0f);
                LUA_GET_VALUE_DEF("START_Y", ModelData.m_fStartY, -1.0f);
                LUA_GET_VALUE_DEF("WIDTH", ModelData.m_iWidth, 1);
                LUA_GET_VALUE_DEF("HEIGHT", ModelData.m_iHeight, 1);

                LUA_BEGIN_TABLE_RAII("MESH_DATA")
                {
                    LUA_GET_VALUE_DEF(1, ModelData.m_strMeshName, "NULL");
                    LUA_GET_VALUE_DEF(2, ModelData.m_strTexName, "NULL");
                }

                LUA_BEGIN_TABLE_RAII("MOTIONLIST")
                {
                    MoveModelMotion moveModelMotion;

                    for (int j = 1; ; ++j)
                    {
                        LUA_BEGIN_TABLE(j, break)
                        {
                            LUA_GET_VALUE_DEF(1, moveModelMotion.m_iMotionID, 1);
                            LUA_GET_VALUE_DEF(2, moveModelMotion.m_strMotionName, "NULL");
                            LUA_GET_VALUE_DEF(3, moveModelMotion.m_iReplayCount, 1);
                            LUA_GET_VALUE_DEF(4, moveModelMotion.m_iMouseEventType, 0);

                            ModelData.m_vecMotionList.emplace_back(moveModelMotion);
                        }
                        LUA_END_TABLE(break);
                    }
                }

                LUA_BEGIN_TABLE_RAII("PARTICLE")
                {
                    MoveModelParticle moveModelParticle;
                    for (int k = 1; ; ++k)
                    {
                        LUA_BEGIN_TABLE(k, break);
                        {
                            LUA_GET_VALUE_DEF(1, moveModelParticle.m_particleSeqName, ""); //Motion Data
                            LUA_GET_VALUE_DEF(2, moveModelParticle.m_fXOffset, 1.0f); //??
                            LUA_GET_VALUE_DEF(3, moveModelParticle.m_fYOffset, 1.0f); //??
                            LUA_GET_VALUE_DEF(4, moveModelParticle.m_fTraceTime, 1.0f); //??
                            LUA_GET_VALUE_DEF(5, moveModelParticle.m_iMouseEventType, 1); //Event
                            ModelData.m_vecParticle.emplace_back(moveModelParticle);
                        }
                        LUA_END_TABLE(break);
                    }
                }
            }
    else
    {
        break;
    }

    g_kGlobalValue.m_mapMoveModelData.insert(std::make_pair(ModelData.m_NPCID, ModelData));
    }
    }
}

bool KGCGlobal::GetMoveModelData(int iNPCID, MoveModelData& ModelData)
{
    std::map<int, MoveModelData>::iterator mit = m_mapMoveModelData.find(iNPCID);
    if (mit != m_mapMoveModelData.end())
    {
        ModelData = mit->second;
        return true;
    }

    return false;
}

void KGCGlobal::SetOpenedEventDungeonInfo( std::vector<std::pair<int, std::vector<int> > > CategoryModes )
{
    m_vecOpenedEventDungeonList.clear();

    std::vector<std::pair<int, std::vector<int> > >::const_iterator vitCM;

    for( vitCM =  CategoryModes.begin(); vitCM != CategoryModes.end(); ++ vitCM )
    {
        int iCategoryName = (int)GC_GMC_DUNGEON;
        if( (*vitCM).first == iCategoryName )
        {
            const std::vector<int>& vecModes = (*vitCM).second;
            std::vector<int>::const_iterator vit;
            for( vit = vecModes.begin(); vit != vecModes.end(); ++vit )
            {
                if( SiKGCWorldMapManager()->IsEventDungeon( (EGCGameMode)(*vit) ) )
                {
                    m_vecOpenedEventDungeonList.emplace_back((EGCGameMode)(*vit));
                }
            }

        }

    }

}

std::vector<EGCGameMode> KGCGlobal::GetOpenedEventDungeonInfo()
{
    return m_vecOpenedEventDungeonList;
}

void KGCGlobal::LoadShortMenuInfo()
{
    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "shortmenucharview.lua") == false)
        return;

    LUA_BEGIN_TABLE_RAII("CharView")
    {
        for (int i = 0; i < GC_CHAR_NUM; ++i)
        {
            ShortMenuCharInfo shortMenuCharInfoData;

            char strTemp[50];
            sprintf(strTemp, "Char%d", i);

            LUA_BEGIN_TABLE_RAII(strTemp)
            {
                LUA_GET_VALUE_DEF("FootholdModel", shortMenuCharInfoData.m_strFootholdModel, "");
                LUA_GET_VALUE_DEF("FootholdTexture", shortMenuCharInfoData.m_strFootholdTexture, "");
                LUA_GET_VALUE_DEF("FootholdMotion", shortMenuCharInfoData.m_strFootholdMotion, "");
                LUA_GET_VALUE_DEF("BackgroundTexture", shortMenuCharInfoData.m_strBackgroundTexture, "");

                LUA_BEGIN_TABLE_RAII("motion")
                {
                    int l = 1;
                    while (true)
                    {
                        std::string motion;
                        if (luaMgr.GetValue(l++, motion) != S_OK) break;
                        shortMenuCharInfoData.m_vecMotion.emplace_back(motion);
                    }
                }
            }
    else
    {
        break;
    }
    g_kGlobalValue.m_saShortMenuInfo[i] = shortMenuCharInfoData;
        }
    }
}

KGCGlobal::ShortMenuCharInfo KGCGlobal::GetShortMenuInfoForCharType(int iCharType)
{
    if (iCharType >= 0 && iCharType < GC_CHAR_NUM)
    {
        return m_saShortMenuInfo[iCharType];
    }

    return ShortMenuCharInfo();
}

bool KGCGlobal::GetIsZeroMask(int iItemID)
{
    for (auto element : m_vecZeroMaskID)
        if (iItemID == element)
            return true;
    return false;
}

int KGCGlobal::GetCurrEquipedZeroMaskID(SCharInfo* pkCharInfo)
{
    int iItemID = 0;
    for (auto& element : pkCharInfo->vecItemInfo)
    {
        if (GetIsZeroMask(element.iItemID * 10))
        {
            iItemID = element.iItemID;
            break;
        }
    }

    switch (iItemID)
    {
    case	  0: return 0;
    case 159522: return 1;
    case 159523: return 2;
    case 159524: return 3;
    case 159525: return 4;
    }
}

int KGCGlobal::GetZeroMaskItemID(int iOption)
{
    switch (iOption)
    {
    case 0: return 1595220;
    case 1: return 1595230;
    case 2: return 1595240;
    case 3: return 1595250;
    }
}

void KGCGlobal::SetCoordiViewItemInfo()
{
    if (!m_mapCoordiViewItemInfo.empty())
        return;
    m_mapCoordiViewItemInfo.clear();
    std::vector< std::pair< int, int > > vecInfo;
    for (int curChar = 0; curChar < GC_CHAR_NUM; curChar++) {
        for (auto& element : CoordiViewVector)
        {
            GCItem* ItemTemp = g_pItemMgr->GetItemData(element.first / 10);
            if (ItemTemp->GetCharType() == curChar)
                vecInfo.emplace_back(std::make_pair((int)element.first, element.second));
        }
        m_mapCoordiViewItemInfo.insert(std::make_pair(curChar, vecInfo));
        vecInfo.clear();
    }
}

bool KGCGlobal::IsSkillEffectDisabled(int iWho)
{
    if (g_MyD3D->m_KGCOption.IsSkillEffect())
        return false;
    if (!g_MyD3D->IsPlayerIndex(iWho))
        return false;
    if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
        return false;
    if (g_MyD3D->GetMyPlayer()->m_iPlayerIndex == iWho)
        return false;
    return true;
}

void KGCGlobal::GetEventScheduleList()
{
    m_vecEventDungeonList.clear();
    m_vecEventDungeonInfo.clear();
    KLuaManager luaMgr;
    if ( GCFUNC::LoadLuaScript( luaMgr, "InitEventDungeon.lua" ) == false )
        return;
    luaMgr.BeginTable( "EventDungeon" );
    {
        for ( int i = 1; ; ++i )
        {
            KEventDungeon kInfo;
            if ( FAILED( luaMgr.BeginTable( i ) ) ) break;
            {
                LUA_GET_VALUE_DEF( "ModeID", kInfo.m_nModeID, -1 );
                if ( FAILED( luaMgr.BeginTable( "OpenDays" ) ) ) break;
                {
                    for ( int j = 1; j < 8; ++j )
                    {
                        int tempDay = -1;
                        LUA_GET_VALUE_DEF( j, tempDay, -1 );
                        kInfo.m_vecOpenDays.emplace_back( tempDay );
                    }
                }
                luaMgr.EndTable();
            }
            m_vecEventDungeonInfo.emplace_back( kInfo );
            KDetailDungeonInfo kDungeonDetailInfo;
            kDungeonDetailInfo.iGameMode = kInfo.m_nModeID;
            kDungeonDetailInfo.wstrName = SiKGCWorldMapManager()->GetCurrentDungeonName( (EGCGameMode)kInfo.m_nModeID );
            kDungeonDetailInfo.wstrOpen = GetEventDungeonOpenDayStringInfo( (EGCGameMode)kInfo.m_nModeID );
            m_vecEventDungeonList.emplace_back( kDungeonDetailInfo );
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();
}

bool KGCGlobal::IsOpenDayEventDungeon( EGCGameMode _eGameMode )
{
    int iWeekDay = m_kToday.m_cWeekDay;
    for ( auto element : m_vecEventDungeonInfo )
    {
        if ( element.m_nModeID == _eGameMode )
        {
            for ( auto openingDay : element.m_vecOpenDays )
            {
                if ( iWeekDay == openingDay )
                    return true;
            }
            return false;
        }
    }
    return true;
}

std::wstring KGCGlobal::GetEventDungeonOpenDayString( EGCGameMode _eGameMode )
{
    std::wstring closeDay = L"#cf5762c";
    int iWeekDay = m_kToday.m_cWeekDay;
    std::wstringstream daysWeekOpening;
    daysWeekOpening.str(std::wstring());
    daysWeekOpening.clear();
    for ( auto element : m_vecEventDungeonInfo )
    {
        if ( element.m_nModeID == _eGameMode )
        {
            for ( auto openingDay : element.m_vecOpenDays )
            {
                if ( openingDay > -1 )
                {
                    if( element.m_vecOpenDays[0] != openingDay )
                        daysWeekOpening << L", ";
                    else
                        daysWeekOpening << g_pkStrLoader->GetString(STR_ID_EVENT_DUNGEON_DAILY_TEXT_01) << L" ";
                    daysWeekOpening << closeDay << g_pkStrLoader->GetString(STR_ID_EVENT_DUNGEON_DAILY_TEXT_02 + openingDay) << L"#cX";
                }
            }
        }
    }
    return daysWeekOpening.str();
}

std::wstring KGCGlobal::GetEventDungeonOpenDayStringInfo( EGCGameMode _eGameMode )
{
    std::wstring closeDay = L"#cf5762c";
    int iWeekDay = m_kToday.m_cWeekDay;
    std::wstringstream daysWeekOpening;
    daysWeekOpening.str( std::wstring() );
    daysWeekOpening.clear();
    for ( auto element : m_vecEventDungeonInfo )
    {
        if ( element.m_nModeID == _eGameMode )
        {
            for ( auto openingDay : element.m_vecOpenDays )
            {
                if ( openingDay > -1 )
                {
                    if ( element.m_vecOpenDays[0] != openingDay )
                        daysWeekOpening << L", ";
                    daysWeekOpening << closeDay << g_pkStrLoader->GetString( STR_ID_EVENT_DUNGEON_DAILY_TEXT_02 + openingDay ) << L"#cX";
                }
            }
        }
    }
    return daysWeekOpening.str();
}

void KGCGlobal::GetCharacterCollectionList()
{
    m_vecCharacterCollectionList.clear();
    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "InitHeroCollection.stg") == false)
        return;
    luaMgr.BeginTable("HeroCollection");
    {
        for (int i = 1; ; ++i)
        {
            CollectionCharacter kCharInfo;
            if (FAILED(luaMgr.BeginTable(i))) break;
            {
                for (int j = 1; ; ++j)
                {
                    CollectionPromotion kPromotionInfo;
                    if (FAILED(luaMgr.BeginTable(j))) break;
                    {
                        for (int k = 1; ; ++k)
                        {
                            CollectionAttribute kPromotionAttribute;
                            if (FAILED(luaMgr.BeginTable(k))) break;
                            {
                                LUA_GET_VALUE_NOASSERT(1, kPromotionAttribute.m_iAttribute, break);
                                LUA_GET_VALUE_NOASSERT(2, kPromotionAttribute.m_fAttributeValue, break);
                            }
                            kPromotionInfo.m_iPromotionLevel = j - 1;
                            kPromotionInfo.m_vecPromotionAttributes.push_back(kPromotionAttribute);
                            luaMgr.EndTable();
                        }
                    }
                    kCharInfo.m_iCharType = i - 1;
                    kCharInfo.m_pkPromotionArray[j - 1] = kPromotionInfo;
                    luaMgr.EndTable();
                }
            }
            m_vecCharacterCollectionList.push_back(kCharInfo);
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();
}

void KGCGlobal::GetVIPCollectionList()
{
    m_vecVIPCollectionList.clear();
    KLuaManager luaMgr;
    if (GCFUNC::LoadLuaScript(luaMgr, "InitVipCollection.stg") == false)
        return;
    luaMgr.BeginTable("VipCollection");
    {
        for (int i = 1; ; ++i)
        {
            CollectionVIP kVIPInfo;
            if (FAILED(luaMgr.BeginTable(i))) break;
            {
                CollectionAttribute kVIPAttribute;
                LUA_GET_VALUE_NOASSERT("FontID", kVIPInfo.m_iFontID, break);
                if (FAILED(luaMgr.BeginTable(1))) break;
                {
                    LUA_GET_VALUE_NOASSERT(1, kVIPAttribute.m_iAttribute, break);
                    LUA_GET_VALUE_NOASSERT(2, kVIPAttribute.m_fAttributeValue, break);
                }
                kVIPInfo.m_pkCollectionAttribute = kVIPAttribute;
                luaMgr.EndTable();
            }
            m_vecVIPCollectionList.push_back(kVIPInfo);
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();
}
void KGCGlobal::OtherWebOpen(const CHAR* strURL_)
{
    static const LONG lBoxSizeX = 800;
    static const LONG lBoxSizeY = 600;
    RECT rc =
    {
        W_Width / 2 - lBoxSizeX / 2,
            W_Height / 2 - lBoxSizeY / 2,
            W_Width / 2 - lBoxSizeX / 2 + lBoxSizeX,
            W_Height / 2 - lBoxSizeY / 2 + lBoxSizeY,
    };

    if (false == g_pBrowser->Create(rc, g_MyD3D->m_hWnd)) return;

    g_pBrowser->Navigate(strURL_, "", L"");
}

int KGCGlobal::GetGradeBonus(PLAYER* pPlayer)
{
    if (pPlayer->m_kUserInfo.m_iPvExp >= 1000)
        return 14;
    else if (pPlayer->m_kUserInfo.m_iPvExp >= 500)
        return 12;
    else if (pPlayer->m_kUserInfo.m_iPvExp >= 300)
        return 10;
    else if (pPlayer->m_kUserInfo.m_iPvExp >= 150)
        return 8;
    else if (pPlayer->m_kUserInfo.m_iPvExp >= 100)
        return 6;
    else if (pPlayer->m_kUserInfo.m_iPvExp >= 60)
        return 4;
    else if (pPlayer->m_kUserInfo.m_iPvExp >= 30)
        return 2;
    else
        return 0;
}