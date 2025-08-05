#include "stdafx.h"
#include <experimental/filesystem> 
#include "MyD3D.h"
#include "KDInput.h"
#include "KInfoBox.h"
#include "CheckHackTool.h"
#include "GCUIHeader.h"
#include "KStats.h"
#include "KMci.h"
#include "SlangFilter.h"
#include "VersionDef.h"
#include "DamageManager.h"
#include "Spark.h"
#include "GCNetwork.h"
#include "Controls.h"
#include "Headup Display.h"
#include "QuestGameManager.h"
#include "Item.h"
#include "Monster.h"
#include "Stage.h"
#include "KSingleton.h"
#include "Buff/KGCBuffManager.h"
#include "KGCGameBoard.h"
#include "PetAI.h"
#include "KTLib/KTDGFrustum.h"
#include "NProtect.h"
#include "HackShield.h"
#include "NPGameLib.h"
#include "GlobalVariables.h"
#include "OpenProcess.h"
#include "KGCRoute.h"
#include "Replay.h"
#include "SystemAnalysis/SystemAnalysis.h"
#include "KGCNewTerm.h"
#include "EnumString.h"
#include "ClientErr.h"
#include "GCRenderManager.h"
#include "MapSystem/KGCLayerManager.h"
#include "GCDropItemManager.h"
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "mission/GCMissionManager.h"
#include "mission/GCCTMissionManager.h"
#include "GCDropItemManager.h"
#include "KGCElvis.h"
#include "KGC3DObjectMgr.h"
#include "KGCMonCrusader.h"
#include "Registry.h"
#include "KMailSender.h"
#include "./Square/GCSquare.h"
#include "gcui/KGCEmoticonListBox.h"
#include "KGCAnnounceMgr.h"
#include "KGCMinigameManager.h"
#include "jpeg/header/ximage.h"
#include "BuddySystem/KFriendShip.h"
#include "Square/GCSquarePet.h"
#include "Buff/KGCBuffManager.h"
#include "KGCMath.h"
#include "EnchantStrong.h"
#include "GCSKT.h"
#include "GCCoupleSystem.h"
#include "KGCContributionManager.h"
#include "./gcui/KGCLogoutBox.h"
#include "../KGCValentineDayManager.h"
#include "./gcui/KGCGPCapsuleDlg.h"
#include "./gcui/KGCGPCapsuleItemList.h"
#include "./gcui/KGCGPCapsuleObtainWnd.h"
#include "./gcui/KGCCouponNPCDlg.h"
#include "./gcui/KGCCouponNPCEventWnd.h"
#include "./gcui/KGCSubscriptionBox.h"
#include "./gcui/KGCSubscriptionDlg.h"
#include "./gcui/GCInventoryFullMsg.h"
#include "./gcui/KGCFileFindDialog.h"
#include "gcui/GCChristmasGiftDlg.h"
#include "gcui/GCChristmasDonateDlg.h"
#include "gcui/GCChristmasSendGiftDlg.h"
#include "gcui/GCChristmasGiftItemBox.h"
#include "gcui/GCChristmasRewardBox.h"
#include "gcui/GCChristmasFriendList.h"
#include "gcui/GCPetItembox.h"
#include "gcui/GCEventEclipsePlotDlg.h"
#include "gcui/GCAddtionGuide.h"
#include "gcui/GCAlertInDungeon.h"
#include "gcui/KGCNewsFrame.h"
#include "GCUI/GCSongkranDlg.h"
#include "GCUI/GCEventAlarmDlg.h"
#include "GCUI/GCSongkranRewardDlg.h"
#include "Zip/Zipper.h"
#include "../GCUtil/KOTPMD5.h"
#include "psapi.h"
#include "KGCMultipleLanguages.h"
#include "KGCDota.h"
#include "KGCPartyManager.h"
#include "GCForgeManager.h"
#include "GCEclipseEventManager.h"
#include "KGCWeatherSystem.h"
#include "DragManager.h"
#include "GCSongkranEventManager.h"
#include "GCAdventureManager.h"
#include "GCUI/KGCAdventureReward.h"
#include "GCUI/KGCAdventureRewardItemWnd.h"
#include "KGCSyncObject.h"
#include "GCUI/KGCAdventureDialog.h"
#include "GCLevelResetEventManager.h"
#include "MonsterManager.h"
#include "GCOlympicManager.h"
#include "GCTextColorManager.h"
#include "gcui/GCNameTag.h"
#include "GCUI/KGCPrograssBar.h"
#include "GCItemCombinationManager.h"
#include "gcui/KGCItemCombinationDlg.h"
#include "gcui/KGCItemCombinationStuff.h"
#include "KGCMatchManager.h"
#include "GCClientErrorManager.h"
#include "TextureLoadManager.h"
#include "GCProfileManager.h"
#include "GCRitasChrismasManager.h"
#include "GCAutoMatchManager.h"
#include "KGCEmbarkManager.h"
#include "GCBuffDonationManager.h"
#include "GCAttendanceStampManager.h"
#include "GCUpdatePlanManager.h"
#include "GCChallengeErrandManager.h"
#include "./MapSystem/GCWallManager.h"
#include "KGCSubjectManager.h"
#include "GCGuideMgr.h"
#include "./GCUI/GCAccountNickNameFrame.h"
#include "KGCGiftBoxManager.h"
#include "KGCJumpingCharManager.h"
#include "GCGrowingSkillManager.h"
#include "KGCVirtualDepotManager.h"
#include "GCSocialCommerceManager.h"
#include "GCUI/KGCGachaPongDlg.h"
#include "GCUI/KGCRoomOptionCharDlg.h"
#include "GCUI/KGCRoomOptionRankDlg.h"
#include "GCUI/GCRoomUserPotionS6.h"
#include "GCUI/GCCustomizarionDlg.h"
#include "GCUI/GCFontSkinDlg.h"
#include "GCUI/GCRoomAnimationDlg.h"
#include "GCUI/GCCutInSkinDlg.h"
#include "GCUI/GCZeroMaskDlg.h"
#include "GCUI/KGCQuantityPurchaseBox.h"
#include "GCUI/GCQuantityUseBox.h"
#include "GCUI/KGCCalendarPerfectAttendanceControl.h"
#include "GCUI/GCHeroTicketUseWnd.h"
#include "GCUI/GCVipCollectionDlg.h"
#include "GCUI/GCCharCollectionDlg.h"
#include "GCUI/GCAllCollection.h"

#ifndef DEV_BUILD
#include "DiscordHelper.h"
#endif

HWND hDuplicateWnd = NULL;
CMyD3DApplication* g_MyD3D = NULL;
std::string g_strCurrentPath;
std::string g_strIDCookie;
std::string g_strNickNameCookie;
std::string g_strEtcCookie;
WCHAR g_Selected_Channel_Name[MAX_PATH];
DWORD StartGameTime = 0;
int o3 = 0;
int o5 = 0;
int o6 = 0;
int o10 = 0;
int o30 = 0;
int iGC_FPS = 0;
int	iSecondCount = 0;
long W_X = 0;
long W_Y = 0;
long W_Width = 0;
long W_Height = 0;
bool Windowable = false;
bool EnableMultiMap = false;
bool EnableModulate2X= false;
HINSTANCE g_hInstance;
char strRegValueName[MAX_PATH] = {0,};

D3DXMATRIX g_TempMatScale, g_TempMatRotate, g_TempMatTranslate, g_TempMatWorld;

HHOOK hKeyboardHook = NULL;

#if ( defined( _DEBUG ) && defined( UNICODE_DEBUG ) ) || !defined( _DEBUG)
/*HMODULE LoadUnicowsProc(void)
{
    HMODULE hModule = LoadLibraryA("unicows.dll");
    return hModule;
}*/

#ifdef _cplusplus
extern "C"{
#endif
    //extern FARPROC _PfnLoadUnicows = (FARPROC) &LoadUnicowsProc;
#ifdef _cplusplus
}
#endif
#endif

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(
    LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
    // TODO: MiniDumpWriteDump
    return NULL;
}

BOOL PreventSetUnhandledExceptionFilter()
{
    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
    
    if (hKernel32 == NULL) 
        return FALSE;
    void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
    
    if(pOrgEntry == NULL) 
        return FALSE;
    
    unsigned char newJump[ 100 ];
    DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
    dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
    void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
    DWORD dwNewEntryAddr = (DWORD) pNewFunc;
    DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

    newJump[ 0 ] = 0xE9;  // JMP absolute
    memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));
    SIZE_T bytesWritten;
    BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
        pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
    return bRet;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p;
    if(nCode == HC_ACTION)
    {
        p = (PKBDLLHOOKSTRUCT)lParam;
        if((p->vkCode == VK_ESCAPE) && (p->flags & LLKHF_ALTDOWN))
        {
            PostMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );
            return 1;
        }
        if((p->vkCode == VK_TAB) && (p->flags & LLKHF_ALTDOWN))
        {
            PostMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );
            return 1;
        }
        //if((p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN))
        //{
        //    return 1;
        //}
        return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
    }

    return S_OK;
}

HRESULT CorrectProgramPath( std::string& strOutProgramPath )
{
    // 현재 폴더 경로를 가져온다.
    char szBuf[MAX_PATH] = { 0, };
    ::GetCurrentDirectoryA( MAX_PATH, szBuf );
    strOutProgramPath = szBuf;

    // 끝부분에 '\'가 빠져 있다면 추가해준다.
    if ( '\\' != strOutProgramPath[strOutProgramPath.size() - 1] )
    {
        strOutProgramPath.append( "\\" );
    }

    //대만이면 __PATH__ 는 레지스트리와 연관 완전 없음
#if !defined( NATION_KOREA )
    return S_OK;
#endif //NATION_TAIWAN || NATION_HONGKONG || NATION_CHINA || NATION_BRAZIL


    // NOTE : __PATH__ 매크로가 정의되어 있다면 레지스트리의 경로로 수정한다.

#ifdef __PATH__
    KRegistry kRegOrg;
    KRegistry kRegTest;
    KRegistry kRegBuyTest;

    char szRegKeyOrg[MAX_PATH] = { 0, };
    char szRegKeyTest[MAX_PATH] ={ 0, };
    char szRegKeyBuyTest[MAX_PATH] ={ 0, };

    // 하드코딩 합니다 ㅠ.ㅠ;;;;;;;;; 하드코딩까지는 아닐수도 ㅡㅡ;
#if defined( NATION_JAPAN )
    {
        // NXJ
        if(__argc == 8 )
        {
            ::sprintf( szRegKeyOrg, "SOFTWARE\\Nexon\\Japan\\GrandChase\\" ); // 오리지널 서버용
            ::sprintf( szRegKeyTest, "SOFTWARE\\Nexon\\Japan\\GrandChaseTest\\" ); // 테스트 서버용
            ::sprintf( strRegValueName, "InstallPath" );
        }
        // NMJ
        else if( __argc == 7 )
        {
            ::sprintf( szRegKeyOrg, "SOFTWARE\\NetmarbleJP\\GrandChase\\" ); // 오리지널 서버용
            ::sprintf( szRegKeyTest, "SOFTWARE\\NetmarbleJP\\GrandChaseTest\\" ); // 테스트 서버용
            ::sprintf( strRegValueName, "DestDir" );
        }
    }
#else // NATION_KOREA
    {
        ::sprintf( szRegKeyOrg, "SOFTWARE\\Netmarble\\NetmarbleGrandChase\\" ); // 오리지널 서버용
        ::sprintf( szRegKeyTest, "SOFTWARE\\Netmarble\\NetmarbleGrandChaseTest\\" ); // 테스트 서버용
        ::sprintf( szRegKeyBuyTest, "SOFTWARE\\Netmarble\\NetmarbleGrandChaseBuyTest\\" ); // 구매테섭용
        ::sprintf( strRegValueName, "DestDir" );
    }
#endif

    std::string strDestDir;

    // 오픈테섭
#if defined( _OPEN_TEST_ )
    {
#define KREG kRegTest
#define REG_KEY szRegKeyTest
    }
#elif defined( NETMARBLE_BUY_TEST )
    {
#define KREG kRegBuyTest
#define REG_KEY szRegKeyBuyTest
    }
#else
    {
#define KREG kRegOrg
#define REG_KEY szRegKeyOrg
    }
#endif // _OPEN_TEST_

    if ( true == KREG.OpenKey( KRegistry::localMachine, REG_KEY ) )
    {
        if ( true == KREG.GetValue( strRegValueName, strDestDir ) )
        {
            ::SetCurrentDirectoryA( strDestDir.c_str() );
            strOutProgramPath = strDestDir;

            // 끝부분에 '\'가 빠져 있다면 추가해준다.
            if ( '\\' != strOutProgramPath[strOutProgramPath.size() - 1] )
            {
                strOutProgramPath.append( "\\" );
            }
        }
        else
        {
            KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_REGISTRY1 ).c_str(),
                g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_REGISTRY2 ).c_str(),
                KMsgBox::MBOX_OK );
            return E_FAIL;
        }
    }
    else
    {
        KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_REGISTRY1 ).c_str(),
            g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_REGISTRY2 ).c_str(),
            KMsgBox::MBOX_OK );
        return E_FAIL;
    }

#ifdef _OPEN_TEST_

    // NOTE : 테스트용 클라이언트일 경우 오리지널 서버용 클라이언트 경로와 비교한다. (해킹 방지)

    if ( true == kRegOrg.OpenKey( KRegistry::localMachine, szRegKeyOrg ) )
    {
        std::string strOrgDestDir;

        if ( true == kRegOrg.GetValue( strRegValueName, strOrgDestDir ) )
        {
            // 같은 폴더에 깔려 있는 경우에는 실행을 못하게 한다.
            if ( strDestDir == strOrgDestDir )
            {
                KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_TEST_CLIENT_RUN1 ).c_str(),
                    g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_TEST_CLIENT_RUN2 ).c_str(),
                    KMsgBox::MBOX_OK );
                return E_FAIL;
            }
        }
    }

    // NOTE : 테스트용 클라이언트일 경우 현재 경로와 비교한다. (해킹 방지)

    ::GetCurrentDirectoryA( MAX_PATH, szBuf );
    std::string strCurDir( szBuf );

    // 끝부분에 '\'가 빠져 있다면 추가해준다.
    if ( '\\' != strCurDir[strCurDir.size() - 1] ) strCurDir.append( "\\" );
    if ( '\\' != strDestDir[strDestDir.size() - 1] ) strDestDir.append( "\\" );

    // 대소문자 구분없이 비교해서 다르다면 실행을 막는다.
    if ( 0 != ::_stricmp( strCurDir.c_str(), strDestDir.c_str() ) )
    {
        KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_TEST_CLIENT_RUN1 ).c_str(),
            g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_TEST_CLIENT_RUN2 ).c_str(),
            KMsgBox::MBOX_OK );
        return E_FAIL;
    }

#endif // _OPEN_TEST_
#endif // __PATH__
    return S_OK;
}

void CMyD3DApplication::RestoreResoultionSetting()
{
	DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
	D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

	g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwCurrentMode = g_MyD3D->m_KGCOption.GetBasicMode();
	g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].MultiSampleTypeWindowed = D3DMULTISAMPLE_NONE;
	g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].MultiSampleTypeFullscreen = D3DMULTISAMPLE_NONE;
	m_d3dpp.MultiSampleType    = D3DMULTISAMPLE_NONE;

	g_MyD3D->m_KGCOption.SetResolution(g_MyD3D->m_KGCOption.GetBasicMode());
	g_MyD3D->m_KGCOption.SetSampleing(D3DMULTISAMPLE_NONE);
}

std::string CMyD3DApplication::ExecCommand(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    }
    catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

std::string CMyD3DApplication::GetSerialNumber() {
    return GCFUNC::Base64Encode(ExecCommand("wmic diskdrive get serialnumber"));
}


std::string CMyD3DApplication::GetVersionFromRegistery()
{
	KRegistry kReg;
	std::string strRet="Error";
	char szRegKey[256];
#if !defined(__PATH__)
	::sprintf( szRegKey, "SOFTWARE\\KOG\\Thailand\\GrandChaseTest\\" );
#else
	::sprintf( szRegKey, "SOFTWARE\\KOG\\Thailand\\GrandChase\\" );
#endif
	if (kReg.OpenKey(KRegistry::localMachine,szRegKey)== true)
	{
		kReg.GetValue("PatchVersion",strRet);		
	}
	return strRet;
}

void CMyD3DApplication::SetRunDirectoryToRegistery()
{
	KRegistry kReg;
	std::string strRet;
	CorrectProgramPath(strRet);
	char szRegKey[256];
#if !defined(__PATH__)
	::sprintf( szRegKey, "SOFTWARE\\KOG\\Thailand\\GrandChaseTest\\" );
#else
	::sprintf( szRegKey, "SOFTWARE\\KOG\\Thailand\\GrandChase\\" );
#endif
	if (kReg.OpenKey(KRegistry::localMachine,szRegKey)== true)
	{
		kReg.SetValue("InstallDir",strRet.c_str());		
	}
}


void CMyD3DApplication::SetResolution(BOOL bChange)
{
	float fWSizeX = 1.0f;
	float fWSizeY = 1.0f;
	if (bChange==TRUE)
	{
		fWSizeX = g_MyD3D->m_KGCOption.GetWindowScaleX();
		fWSizeY = g_MyD3D->m_KGCOption.GetWindowSacleY();
	}
    else
    {
        fWSizeX = g_MyD3D->m_KGCOption.GetBasicWindowScaleX();
        fWSizeY = g_MyD3D->m_KGCOption.GetBasicWindowSacleY();
    }

	g_pGCDeviceManager->SetWindowScale( fWSizeX, fWSizeY );
	g_pGCDeviceManager2->SetWindowScale( fWSizeX, fWSizeY );
	g_pkChatManager->ClearAllChatMsg();
	g_MyD3D->m_pkUIMgr->ResetFonts();
	g_MyD3D->m_pkUIMgr->ResetAllUIforResolution();

	::PostMessage(g_MyD3D->Get_hWnd(),WM_COMMAND,IDM_CHANGEDEVICE,TRUE);

	return;
}

bool CMyD3DApplication::ChangeSampleing(D3DMULTISAMPLE_TYPE eSample)
{
	DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
	D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];

	if( SUCCEEDED( g_MyD3D->m_pD3D->CheckDeviceMultiSampleType( g_MyD3D->m_dwAdapter,
		pDevice->DeviceType, pDevice->modes[g_MyD3D->m_KGCOption.GetCurrentMode()].Format, FALSE, eSample , NULL ) ) )
	{
		if( SUCCEEDED( g_MyD3D->m_pD3D->CheckDeviceMultiSampleType( g_MyD3D->m_dwAdapter,
			pDevice->DeviceType, pDevice->modes[g_MyD3D->m_KGCOption.GetCurrentMode()].Format, TRUE, eSample , NULL ) ) )
		{
			m_d3dpp.MultiSampleType    = eSample;
			pDevice->MultiSampleTypeWindowed = eSample;
			pDevice->MultiSampleTypeFullscreen = eSample;
			return true;
		}
	}
	return false;
}

bool CMyD3DApplication::ChangeResolution(BOOL bAlwaysResize)
{
	HRESULT hr;
	
	if( m_bActive && IsWindowMode() )
	{
        int iScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
        int iScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );

		g_MyD3D->m_dwCreationWidth = static_cast<DWORD>( GC_SCREEN_WIDTH * g_pGCDeviceManager->GetWindowScaleX() );
		g_MyD3D->m_dwCreationHeight = static_cast<DWORD>( GC_SCREEN_HEIGHT * g_pGCDeviceManager->GetWindowScaleY() );

		RECT rc;
		::SetRect( &rc, 0, 0, g_MyD3D->m_dwCreationWidth, g_MyD3D->m_dwCreationHeight );

		::SetWindowPos( g_MyD3D->m_hWnd, HWND_NOTOPMOST,
			( iScreenWidth - ( rc.right - rc.left ) ) / 2,
			( iScreenHeight - ( rc.bottom - rc.top ) ) / 2,
			( rc.right - rc.left ),
			( rc.bottom - rc.top ),
			SWP_SHOWWINDOW );

		RECT rcClientOld;
		rcClientOld = m_rcWindowClient;

		// Update window properties
		GetWindowRect( m_hWnd, &m_rcWindowBounds );
		GetClientRect( m_hWnd, &m_rcWindowClient );

		POINT pt = { m_rcWindowClient.left, m_rcWindowClient.top, };
		::ClientToScreen(m_hWnd, &pt);
		W_X = pt.x;
		W_Y = pt.y;


		if( rcClientOld.right - rcClientOld.left !=
			m_rcWindowClient.right - m_rcWindowClient.left ||
			rcClientOld.bottom - rcClientOld.top !=
			m_rcWindowClient.bottom - m_rcWindowClient.top || 
			bAlwaysResize == TRUE)
		{
			// A new window size will require a new backbuffer
			// size, so the 3D structures must be changed accordingly.
			m_bReady = FALSE;
			W_Width=m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
			W_Height=m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

			// Resize the 3D environment
			if( FAILED( hr = Resize3DEnvironment() ) )
			{
				KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ON_RESET_DEVICE1 ).c_str(),
					g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ON_RESET_DEVICE2 ).c_str(),
					KMsgBox::MBOX_OK );
				return false;
			}			
			m_bReady = TRUE;
		}
	}

	else if (m_bActive == TRUE && IsWindowMode()==FALSE )//&& g_MyD3D->m_KGCOption.GetBasicMode() != g_MyD3D->m_KGCOption.GetCurrentMode() )
	{   //전체모드
		D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
		D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
		D3DModeInfo*    pModeInfo;
		if (g_pGCDeviceManager->GetWindowScaleX()==1.0f && g_pGCDeviceManager->GetWindowScaleY()==1.0f)
		{
			pModeInfo    = &pDeviceInfo->modes[g_MyD3D->m_KGCOption.GetDefaultMode()];			
		}
		else
		{
			pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];
		}

		m_d3dpp.BackBufferWidth  = pModeInfo->Width;
		m_d3dpp.BackBufferHeight = pModeInfo->Height;
		m_d3dpp.BackBufferFormat = pModeInfo->Format;
		W_X=0;
		W_Y=0;
		W_Width=m_d3dpp.BackBufferWidth;
		W_Height=m_d3dpp.BackBufferHeight;

		if( FAILED( hr = Resize3DEnvironment() ) )
		{
			KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ON_RESET_DEVICE1 ).c_str(),
				g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ON_RESET_DEVICE2 ).c_str(),
				KMsgBox::MBOX_OK );
			return false;
		}
		m_bReady = TRUE;
	}
	return true;
}

void CMyD3DApplication::OnClickMonster()
{
    if (GC_GLOBAL_DEFINE::bMonsterCardMix) {

        if (true == g_pkUIScene->m_pkMonsterCardCompose->GetSendReq()) {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_MONSTER_CARD_COMPOSE_DLG);
        }
        else {
            // 서버에서 받은 스크립트 정보가 없다면 요청
            KP2P::GetInstance()->Send_MonsterCardMixInfoReq();
        }

    }
    else {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_EVENT_PERIOD), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
        return;
    }
}

INT WINAPI VirtualWinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
#if defined( CONSOLEDEVV )
	KSingleton<KncP2P>::GetInstance()->CreateConsoleWnd();
#endif

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#if ( defined( _DEBUG ) && defined( UNICODE_DEBUG ) ) || !defined( _DEBUG)
    //_PfnLoadUnicows();
#endif
    std::wcout.imbue( std::locale( "" ) );
    std::cout.imbue( std::locale( "" ) );
#if !defined( CONSOLEDEVV )
    START_LOG( cout, "GC_START" );
    dbg::logfile::SetLogLevel( -1 );
#else
    dbg::logfile::SetFileNameAtTime( true );
    START_LOG( cout, "GC_START" );
    dbg::logfile::SetLogLevel( 2 ); // error : 0, warn : 1, log : 2
#endif

    IsArtMoneyInstalled();
    if ( FAILED( CorrectProgramPath( g_strCurrentPath ) ) )
    {
        return 0;
    }

#if defined( REGION_MESSAGEBOX )
    KRgnMsgBox::Open( L"", L"", 0, NULL );
#elif !defined(NATION_CHINA)
    KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_INFO_INIT1 ).c_str(),
        g_pkStrLoader->GetString( STR_ID_APPMSG_INFO_INIT2 ).c_str() );
#endif
	
    KP2P::GetInstance();    

    if( !NPGameGuardInit() )
    {
		g_kGlobalValue.ReportforGCLOG("GAME_GUARD Err");
        return 0; // exit
    }

    if( !HS_Initialize() )
    {
		g_kGlobalValue.ReportforGCLOG("GAME_GUARD Err");
        return 0; // exit
    }

    if( !HS_StartService() )
    {
		g_kGlobalValue.ReportforGCLOG("GAME_GUARD Err");
        return 0; // exit
    }

    if( !HS_SaveFuncAddress() )
    {
        //return 0; // exit
    }

    HRESULT hr = S_OK;
    CMyD3DApplication* pApp = new CMyD3DApplication();
    CMyD3DApplication& FightApp = *pApp;
    FightApp.InstallCrashReport();
    g_kGlobalValue.m_dwLastMsgDisplayTime = timeGetTime();
    g_kGlobalValue.m_iPlayBeginTime = timeGetTime(); //ms 단위로 게임 시작 시간 구한다.

    BOOL bRet = PreventSetUnhandledExceptionFilter();

    if ( !bRet ) 
    {
        ASSERT( "Faild PreventSetUnhandledExceptionFilter" );
    }

#if defined( REGION_MESSAGEBOX )
	Sleep(1000);
    KRgnMsgBox::Close();
#else
    KMsgBox::Close();
#endif

	if ( FAILED( FightApp.Create( g_hInstance = hInst ) ) ){
		g_kGlobalValue.ReportforGCLOG("FightApp Create fail");
		goto HaveProblem;
	}

    NPGameGuardSetHWnd( FightApp.m_hWnd );

#if defined( LOADING_IMAGE_FROM_SERVER_SCRIPT)
	if( FightApp.ConnectLoginServer() == false )
    {
		MessageBoxW(NULL, L"Login Server Connect fail", L"GC-Debug", MB_OK);
		g_kGlobalValue.ReportforGCLOG("Login Server Connect fail");
        goto HaveProblem;
	}
#endif
             
#if defined ( __PATH__ ) && defined ( MINIMIZE_ALL_WINDOW )
    HWND hWndShell = FindWindow( L"Shell_TrayWnd", NULL );
    SendMessage( hWndShell, WM_COMMAND, 419, 0 );
#endif 

	MAKE_LOAD_PROFILE( "InitEtc" );
    if ( Windowable )
    {
        // if ( FightApp.Default_Fullscreen )
        {
            // ::SendMessage( FightApp.m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0 );
        }
        // else
        {
            FightApp.m_dwWindowStyle = WS_POPUP|WS_CLIPCHILDREN;//|WS_CAPTION;//|WS_THICKFRAME; // 크기 조절 불가능하게
            ::SetWindowLong( FightApp.m_hWnd, GWL_STYLE, FightApp.m_dwWindowStyle );
            ::GetWindowRect( FightApp.m_hWnd, &FightApp.m_rcWindowBounds );

            RECT rc;
            ::SetRect( &rc, 0, 0, FightApp.m_dwCreationWidth, FightApp.m_dwCreationHeight );
            ::AdjustWindowRect( &rc, FightApp.m_dwWindowStyle, FALSE );

            unsigned long f_Width = FightApp.m_d3dpp.BackBufferWidth;
            unsigned long f_Height = FightApp.m_d3dpp.BackBufferHeight;
            ::SetWindowPos( FightApp.m_hWnd, HWND_NOTOPMOST,
                ( ::GetSystemMetrics( SM_CXSCREEN ) - ( rc.right - rc.left ) ) / 2,
                ( ::GetSystemMetrics( SM_CYSCREEN ) - ( rc.bottom - rc.top ) ) / 2,
                ( rc.right - rc.left ),
                ( rc.bottom - rc.top ),
                SWP_SHOWWINDOW );
            ::SendMessage( FightApp.m_hWnd, WM_EXITSIZEMOVE, 0, 0 );
        }
    }
    ::ShowWindow( FightApp.Get_hWnd(), SW_SHOW );

	MAKE_LOAD_PROFILE( "InitializeWindow" );

    FightApp.InitBasicClasses();

	MAKE_LOAD_PROFILE( "InitBasicClass" );	

	FightApp.ShowExplorer(FALSE, g_pkStrLoader->GetString(STR_ID_MINIMIZED_EXPLORER_STRING) );


#if defined(DLL_INJECTION_CHECK)
    //게임루프 시작 전에 dll 체크
    try
    {
        KDLLChecker::GetInstance()->CheckDll();
        g_MyD3D->m_bCanUseDllchecker = true;
    }
    catch(...)
    {

    }
#endif
    //	g_kGlobalValue.ReportforGCLOG("Game Loop Start");


#if defined(SHA_CHECK)
    KSHAChecker::GetInstance()->StartThread();   
#endif

    hr = FightApp.Run(); // main loop

#if defined(AVI_RECODER)
    // 동영상 녹화중이라면 꺼줌 
    if( g_kGlobalValue.IsCapturing() ) { 
        g_kGlobalValue.ToggleVideoCapture();
    }
#endif

    // 서버와의 접속 마치기
    ShutDown(true);
    FightApp.Cleanup3DEnvironment();

    KGCLuabinder::release();
    KGCLuaManager::release();

    g_KDSound.Free();

HaveProblem:
    g_KMci.Clear();
    ::DestroyWindow( FightApp.m_hWnd );
    //CoUninitialize();

#if defined(LINK_HOMEPAGE_WHEN_ENDGAME) && defined(__PATH__)
	PopUpWebPage( HOMEPAGE_ENDGAME );
#endif

	FightApp.ShowExplorer( TRUE, g_pkStrLoader->GetString(STR_ID_MINIMIZED_EXPLORER_STRING) );
    //#ifdef _DEBUG
    // KSingleton<KLogManager>::ReleaseInstance();
    //#endif // _DEBUG
    KP2P::ReleaseInstance();
    KUserProxy::ReleaseForClient(); // 050610. florist.
    KSingleton<KGCNewTerm>::ReleaseInstance();
    KSingleton<KSystemAnalysis>::ReleaseInstance();
    KSingleton<EnumString>::ReleaseInstance();
#if !defined( __PATH__ )
    KSingleton<KGCFileMoniter>::ReleaseInstance();
#endif

    // NOTE : 사내 테스트에서는 패치 프로그램 파일에 대한 패치 기능을 활성화한다.
    // 본 서버나 오픈 테스트 서버에서는 넷마블 패치 방식을 이용하여 덮어쓰기를 한다.
#if (defined(SERVICE) && !defined(__PATH__)) || defined(NATION_TAIWAN) || defined(NATION_HONGKONG) || defined(NATION_CHINA) || defined(NATION_BRAZIL) || defined(NATION_JAPAN)
    // 패치프로그램을 패치한다.
    PatchPatcher();
#endif // (SERVICE && !__PATH__) || NATION_TAIWAN || NATION_HONGKONG || NATION_CHINA || NATION_BRAZIL

    if ( 0 != ::_tcsclen( FightApp.LastMessage1 ) || 0 != ::_tcsclen( FightApp.LastMessage2 ) )
    {
        std::wstring strMsg;
        if( 0 != ::_tcsclen( FightApp.LastMessageCode ) )
        {
            strMsg = FightApp.LastMessageCode;
            strMsg += L"\n";
        }
        else
        {
            strMsg = L"";
        }
        strMsg += FightApp.LastMessage1;
        strMsg += L"\n";
        strMsg += FightApp.LastMessage2;

        ::MessageBoxW( NULL, strMsg.c_str(), g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), MB_OK );
    }

    // 060406. jseop. HackShield stop service.
    if( !HS_StopService() )
    {
        return 0; // exit
    }

    // 060406. jseop. HackShield uninitialization.
    if( !HS_Uninitialize() )
    {
        return 0; // exit
    }

    SAFE_DELETE( pApp );

	CMemoryBlockBase::Clear();

    _ASSERTE( _CrtCheckMemory( ) );

#if defined(PERFORMANCE_TEST) || defined(PERFORMANCE_TEST_P2P)
    KGCPerformanceCounter::EndCounter();
#endif

	// 파일 변조하는 녀석들이 있어서 체크하는 내용과 틀린점이 발견되었다면 해당파일은 삭제 하겠다.
	for( int i = 0; i < (int)g_kGlobalValue.m_vecDeleteFileLIst.size(); i++ )
	{
		DeleteFileA(g_kGlobalValue.m_vecDeleteFileLIst[i].c_str());
	}

//------------------------------------------------------------------------
//	20091030 [crc구하는 로딩 시간 프로파일링] : 이동석
//#ifdef _LOADING_TIMECHECK_
//			fclose( g_pFile );
//			FileOutLevelUpStageCalc();	
//#endif
//========================================================================
	
    return hr;
}



void InvalidParameterHandler(const wchar_t* expression,
							 const wchar_t* function,
							 const wchar_t* file,
							 unsigned int line, 
							 uintptr_t pReserved)
{
#if defined(DEBUG)
	throw new std::exception("InvalidParameterHandler");
#else
	terminate();
#endif
}

void PurecallHandler(void) 
{
#if defined(DEBUG)
	throw new std::exception("PurecallHandler");
#else
	terminate();
#endif
}	

int NewHandler( size_t )
{
#if defined(DEBUG)
	throw new std::exception("NewHandler");
#else
	terminate();
#endif
}

HRESULT LaunchPatcher()
{
	wchar_t szCurrentPath[MAX_PATH] = {0,};
	wchar_t szDirPath[MAX_PATH] = {0,};
	wchar_t szFullPath[MAX_PATH] = {0,};
	::GetCurrentDirectoryW( MAX_PATH, szCurrentPath );
	swprintf_s( szDirPath, MAX_PATH, L"%s\\", szCurrentPath );
	swprintf_s( szFullPath, MAX_PATH, L"%s%s", szDirPath, L"GrandChase.exe" );

	PROCESS_INFORMATION pi;
	STARTUPINFOW si = { 0, };
	si.cb = sizeof( STARTUPINFO );

	if( CreateProcessW( NULL, szFullPath, NULL, NULL, FALSE, 0, NULL, szDirPath, &si, &pi ) == FALSE )
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CheckParameter()
{
	if( __argc <= 1 )
	{
		LaunchPatcher();
		return E_FAIL;
	}

	return S_OK;
}

void SelectServer() {
	for(int i = 1;__argv[i] != NULL;i++)
	{
		if( 0 == ::strcmp(__argv[i], "-ip") ) {
			g_kGlobalValue.m_strPatcherSelIP = __argv[i+1];
			g_kGlobalValue.m_strPatcherSelPort = __argv[i+2];
			g_kGlobalValue.m_bPatcherSelectServer = true;
			break;
		}
	}
}

void SetUser() {
	for (int i = 1; __argv[i] != NULL; i++)
	{
		if (0 == ::strcmp(__argv[i], "-up")) {
			g_kGlobalValue.m_strUser = __argv[i + 1];
			g_kGlobalValue.m_strPass = __argv[i + 2];
			g_kGlobalValue.m_bLoginFirst = false;
		}
	}
}

void SetVersion() {
	for(int i = 1;__argv[i] != NULL;i++)
	{
		if( 0 == ::strcmp(__argv[i], "-ver") ) {
			std::string strTemp = __argv[i+1];
			g_kGlobalValue.m_wstrVersion = GCUTIL_STR::GCStrCharToWide( strTemp.c_str() );
			g_kGlobalValue.m_bVersion = true;
			break;
		}
	}

}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hInstance, LPSTR str, INT nCmd )
{
	g_kGlobalValue.ReportforGCLOG("START");

#if defined( RUN_PATCHER_WHEN_NO_PARAMETER )
	if( CheckParameter() == E_FAIL )
	{
		return 0;
	}
#endif



	// 문자열 로딩
	KGCLuabinder::getInstance();
	KGCStringLoader kStrLoader( true );
#if defined(NATION_CHINA)
	SiKGCMultipleLanguages()->InitMultipleLanguages(LANGUAGE_CODE_CN_CN);
#endif // NATION_CHINA
	SiKGCMultipleLanguages()->InitMultipleLanguages();


    std::string strErrFileName = ".\\Stage\\err_str";
	if( SiKGCMultipleLanguages()->UseMultipleLanguages() )
	{
       strErrFileName += SiKGCMultipleLanguages()->GetLanguageTypeNUM();

	}
    strErrFileName += ".stg";

    kStrLoader.Load( strErrFileName.c_str() );

	//SelectServer();
	//SetVersion();

	SetUser();

	/*[ OTP TestCode ] : 20101114 exxa222
	========================================================================*/
#if defined( __PATH__ ) && defined(MD5_PATCHER_PARAMETER)
	KOTPMD5 kotp;
	if( !kotp.CheckOTP( std::string(str) ) ){
		::MessageBox( NULL, g_pkStrLoader->GetString(STR_ID_MD5_PATCHER_PARAMETER_INFO).c_str(), L"Update", MB_OK );
		return 1;
	}
#endif

	_set_invalid_parameter_handler(InvalidParameterHandler);
	_set_purecall_handler(PurecallHandler);
	_set_new_handler( NewHandler );

    return VirtualWinMain( hInst, hInstance, str, nCmd );
}

CMyD3DApplication::CMyD3DApplication( void )
{

	for( int i = 0; i < MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO; i++ )
	{
		MyPlayer[i].Get();
		m_akPet[i].Get();
	}

    g_MyD3D = this;

    MyReplay = NULL;
    m_pMapAnim = NULL;
    m_pPetAnim = NULL;
    m_pUIAnim = NULL;
    m_pFrustum = NULL;
    m_pkQuestGameMgr = NULL;
    g_pMagicEffect = NULL;
    g_pGameBoard = NULL;
    g_ParticleManager = NULL;
    //g_pMissionManager = NULL;
    //g_pMeshEffectManager = NULL;
    g_pRoute = NULL;
    m_pkUIMgr = NULL;

    MyStg = NULL;
    MyCtrl = NULL;
    MyHead = NULL;
    //MyBuff = NULL;
    MyItms = NULL;
    MyShortCutSlot = NULL;
    MySparks = NULL;

    m_pStateMachine = NULL;
    g_pGraphicsHelper = NULL;
    g_pGCDeviceManager = NULL;
    g_pkUIHelper = NULL;
    g_pRenderState = NULL;

    for(int i=0;i<MAX_OBJECT;i++)
    {
        m_pObject[i] = NULL;
    }

    ::ZeroMemory( LastMessageCode, sizeof( LastMessageCode ) );
    ::ZeroMemory( LastMessage1, sizeof( LastMessage1 ) );
    ::ZeroMemory( LastMessage2, sizeof( LastMessage2 ) );

    l_r=.25f;
    l_g=.21f;
    l_b=.16f;
    m_bLastFrameSend=false;
    AMBIENT=100;
    bSlotFrameRestart=true;

    m_IsCharSelect = false;
#ifdef NATION_CHINA
    m_strWindowTitle = (TCHAR*)g_pkStrLoader->GetString( STR_ID_GRANDCHASE_TITLE ).c_str();
#else
    m_strWindowTitle = GAME_VERSION;
#endif

    m_bUseDepthBuffer = TRUE;
    Reserve_InputFocus=false;

    // 빛 관련...
    l_r=.2f;
    l_g=.2f;
    l_b=.2f;
    AMBIENT=60;

    Show_FPS=false;

    m_iFPSCount_for_AutoGraphicQuality = 0;
    m_iFPS_for_AutoGraphicQuality = 0;
    m_bInitFPS_for_AutoGraphicQuality = true;
    m_iLowQualityCounter = 0;

    m_nLastFPSTick = 0;
    m_iAveFPS_in_PlayGame = 0;
    m_iAveFPSCount_in_PlayGame = 0;
    m_bInitAveFPS_in_PlayGame = true;

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
        ArrowScore[i] = 0;

    m_bEnableKeyInput = true;
#if !defined( __PATH__ )
    m_iGifRecordingFileCount = 0;
    m_iRecordFrame = 5;
    m_iRecordFrameCount = 0;
    for( int i = 0 ; i < 4 ; ++i )
    {
        m_aColorARGB[i] = 0xff;
    }
#endif
    m_iUserAgreement = -1;

    bEverSpecialCheck = false;

#if !defined( __PATH__ )
    m_bLuaInputMode = false;
#endif

    m_bUseVirtualCash = false;

    m_fIncMPRatio = 1.0f;

	m_bCreateToJoin = false;

    m_bCanUseDllchecker = false;
    m_vTempValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    m_bCheatOpenSkillUI = false;
}

std::string CMyD3DApplication::PassConstXor( std::string entryString )
{
    for ( int i = 0; i < entryString.size(); i++ )
        entryString[i] ^= GetConstXOR();
    return entryString;
}

void CMyD3DApplication::SetConstXOR()
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);
    int day = localTime->tm_mday;
    int month = localTime->tm_mon + 1;
    int year = localTime->tm_year + 1900;
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;
    m_iConstXOR = day ^ month ^ year ^ hour ^ minute ^ second;
    m_iConstXOR *= 7;
}

void CMyD3DApplication::InitBasicClasses()
{
    SetConstXOR();
    InitDataFiles();

	SiKGCRoomManager()->LoadStageSpecification();
   //StateMachine 생성
    m_pStateMachine = new KGCStateMachine();

#ifndef __PATH__
    g_kStatsManager.Init( 40, 150 );
#endif
    MyStg = new STAGE();
    MyCtrl = new Controls();
    UpdateKeySetting( !m_KGCOption.m_bEnableKeySetting );
}

void CMyD3DApplication::InitClasses()
{
    // Define 관련 함수 로드
    GC_GLOBAL_DEFINE::InitGlobalDefine();
    GC_GLOBAL_DEFINE::LoadGlobalDefine();

    GC_GLOBAL_CONST::InitGlobalConst();

#if !defined( __PATH__ ) 
	if ( GC_GLOBAL_DEFINE::bLoadGameModeScript )
		SiKGCRoomManager()->LoadGameMode();
#endif

    MyHead = new Headup();
    //MyBuff = new GCGameBuff();
    MyItms = new Items();


    MyShortCutSlot = new KGCShortCutSlot();
    MyShortCutSlot->initialization();

    //MyModels = new Models();
    //MyMotions = new Motions();
    MySparks = new KEffector();
    m_pDamageManager = new CDamageManager();
    g_pkUIHelper = new KGCUIHelper();

    // Singleton 객체들..
    SiKGCProfileManager()->GetInstance();
    SiGCPairPlaySystem()->GetInstance();
    SiKSlangFilter()->GetInstance();
    SiKGCWorldMapManager()->GetInstance()->Init();
    SiKGCPrivateDungeonManager()->GetInstance();
    SiKGCInfinityDungeonManager()->GetInstance();
    SiKGCChannelManager()->GetInstance();
    SiKGCRoomManager()->GetInstance();
    SiKGCMissionManager()->GetInstance();
    SiKGCCTMissionManager()->GetInstance();
    SiKGCDropItemManager()->GetInstance();
    SiKGCLayerManager()->GetInstance();
    SiKGC3DObjectMgr()->GetInstance();
    SiGCSquare()->GetInstance();
    SiKNeonSign()->GetInstance();
    SiKGCInviteManager()->GetInstance();
    SiKGCAnnounceMgr()->GetInstance();
    SiKD3DEmoticonString()->GetInstance();
	SiKGCMinigameManager()->GetInstance();

	SiKFriendShip()->GetInstance();

    SiGCSKT()->GetInstance();
    SiKGCPartyManager()->GetInstance();

    SiGCForgeManager()->GetInstance();
    SiKGCMultipleLanguages()->GetInstance();
    SiGCEclipseEventManager()->GetInstance();

	SiKGCDragManager()->GetInstance();
    SiGCAdventureManager()->GetInstance();
    SiKGCSyncObjectManager()->GetInstance();
	SiGCTextColorManager()->GetInstance();

    SiKGCWeatherSystem()->GetInstance();
	SiKGCMatchManager()->GetInstance();

	SiGCSongkranEventManager()->GetInstance();
    SiKGCLevelResetEventManager()->GetInstance();	
	SiGCOlympicManager()->GetInstance();
    SiGCItemCombinationManager()->GetInstance();

    SiGCFaceOffManager()->GetInstance();

    SiKGCInGamePrograssBar()->GetInstance();
	
    SiGCClientErrorManager()->GetInstance();
    SiKGCNetworkStatManager()->GetInstance();
    SiKGCEmbarkManager()->GetInstance();

    SiGCAutoMatchManager()->GetInstance();

    SiGCBuffDonationManager()->GetInstance();
    SiGCAttendanceStampManager()->GetInstance();
    SiGCChallengeErrandManager()->GetInstance();

    SiKGCWallManager()->GetInstance();
    SiKGCSubjectManager()->GetInstance();
	
	SiKGCGiftBoxManager()->GetInstance();
    SiKGCJumpingCharManager()->GetInstance();

    SiGCGrowingSkillManager()->GetInstance();
    SiKGCVirtualDepotManager()->GetInstance();
	
    GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "DevelopFunction.lua", false );
    g_kMonsterManager.LoadScript();

    SiKGCSocialCommerceManager()->GetInstance();

	//CreateAllMonster();

    D3DXMatrixIdentity(&g_kCamera.m_matCamera);

    // --;
    //이름 초기화
    for( int i = 0; i < MAX_PLAYER_NUM + NUM_TEMP_PLAYERINFO; i++ )
        MyPlayer[i]->m_iPlayerIndex = i;

    // 루아 바인딩
    RegisterLuabind();

	KGC2DObject::BindLua();

    m_fIncMP = 0.0015f;
    m_fIncMPShield = 0.001f;

    m_bRPLoadFinished = false;
    m_iRPFileNum = 0;

    m_pkQuestGameMgr = new KQuestGameManager();

    KPet::BindLua();
    KPet::LoadPetTemplate();
	KPet::LoadPetCostume();
	KPet::LoadPetSkillInfo();

    //몬스터 대전은 제작 보류됐음..
    //KGCMonsterHunt::LoadScript();
    // 몬스터 원정대 정보 로드
    KGCMonCrusader::LoadScript();

    // 점령전 정보로드
    KGCDota::LoadScript();

    // 싱크오브젝트 정보로드
    SiKGCSyncObjectManager()->LoadScript();

    // 몬스터 스탯 정보 로드
    KGCGlobal::LoadMonsterStatInfo();
	
	//	이벤트 알람 정보 로드
	g_kGlobalValue.m_cEventAlarmManager.LoadScript();

    //  자동매칭 클래스
    SiGCAutoMatchManager()->LoadScript();

    //  탑승물 정보 로드
    SiKGCEmbarkManager()->LoadScript();

    //  벽 정보 로드
    SiKGCWallManager()->LoadScript();

    // 달성과제 정보 로드
    SiKGCSubjectManager()->LoadScript();

    ClearMonster();

    //미션NPC 정보 로드
    KGCGlobal::LoadMoveModelData();

    KGCGlobal::LoadShortMenuInfo();
}

CMyD3DApplication::~CMyD3DApplication( void )
{
#ifndef __PATH__
    g_kStatsManager.CleanUp();
#endif
#ifdef _PACKET_PROFILE_
    g_MyD3D = NULL;
    FILE* pFile = fopen( "P2PPacketText.txt", "w" );
    char strTemp[256] = {0,};
    for( int i = 0; i < GC_PID_BROAD_SANTA_EVENT; i++ )
    {
        ZeroMemory( strTemp, sizeof(char)*256 );
        sprintf( strTemp, "%2d : SendCount(%5u), SendSize(%5uK), RecvCount(%5u), RecvSize(%5uk)\r\n",
            i, g_akSend[i].dwCount, g_akSend[i].dwSize/1024, g_akRecv[i].dwCount, g_akRecv[i].dwSize/1024 );
        fwrite( strTemp, sizeof( char ), strlen( strTemp ), pFile );
    }
    fclose( pFile );
#endif

    //g_BackGroundLoader.Shutdown();
	SAFE_DELETE (g_pBrowser);

//  최소화한 창 원래대로 복원
#if defined ( __PATH__ ) && defined ( MINIMIZE_ALL_WINDOW )
    HWND hWndShell = FindWindow( L"Shell_TrayWnd", NULL );
    SendMessage( hWndShell, WM_COMMAND, 416, 0 );
#endif 

}

void CMyD3DApplication::RegisterLuabind()
{
    // 바인딩
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<D3DXVECTOR2>( L, "D3DXVECTOR2" );
    lua_tinker::class_mem<D3DXVECTOR2>( L, "x",  &D3DXVECTOR2::x  );
    lua_tinker::class_mem<D3DXVECTOR2>( L, "y",  &D3DXVECTOR2::y  );
    lua_tinker::class_con<D3DXVECTOR2>( L, lua_tinker::constructor<>() );

    lua_tinker::class_add<D3DXVECTOR3>( L, "D3DXVECTOR3" );
    lua_tinker::class_mem<D3DXVECTOR3>( L, "x",  &D3DXVECTOR3::x );
    lua_tinker::class_mem<D3DXVECTOR3>( L, "y",  &D3DXVECTOR3::y );
    lua_tinker::class_mem<D3DXVECTOR3>( L, "z",  &D3DXVECTOR3::z );
    lua_tinker::class_con<D3DXVECTOR3>( L, lua_tinker::constructor<>() );

    lua_tinker::class_add<CMyD3DApplication>( L, "CMyD3DApplication" );

    lua_tinker::class_def<CMyD3DApplication>( L, "GetMaxPlayer",  &CMyD3DApplication::GetMaxPlayer );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetMaxMonster",  &CMyD3DApplication::GetMaxMonster );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetPlayer",  &CMyD3DApplication::GetPlayer );
    lua_tinker::class_def<CMyD3DApplication>( L, "IsMyTeam",  &CMyD3DApplication::IsMyTeam );
    lua_tinker::class_def<CMyD3DApplication>( L, "IsSameTeam",  &CMyD3DApplication::IsSameTeam );
    lua_tinker::class_def<CMyD3DApplication>( L, "AddBasiliskHP",  &CMyD3DApplication::AddBasiliskHP );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetPet",  &CMyD3DApplication::Get_Players_Pet );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetPlayerByUID",  &CMyD3DApplication::GetPlayerByUID );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetMyPlayerIdx",  &CMyD3DApplication::Get_MyPlayer );
    lua_tinker::class_def<CMyD3DApplication>( L, "SetShakeCameraFrame",  &CMyD3DApplication::SetShakeCameraFrame );
    lua_tinker::class_def<CMyD3DApplication>( L, "SetEnableKeyInput", 	&CMyD3DApplication::SetEnableKeyInput );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetMyPlayer",  &CMyD3DApplication::GetMyPlayer );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnCalendarButton",  &CMyD3DApplication::OnCalendarButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnCoupleButton",  &CMyD3DApplication::OnCoupleButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnMissionButton",  &CMyD3DApplication::OnMissionButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnMSNButton",  &CMyD3DApplication::OnMSNButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnOptionButton",  &CMyD3DApplication::OnOptionButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnSkillTreeButton",  &CMyD3DApplication::OnSkillTreeButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnSquareButton",  &CMyD3DApplication::OnSquareButton );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnItemCreateAndBreakUpButton",  &CMyD3DApplication::OnItemCreateAndBreakUpButton  );
    lua_tinker::class_def<CMyD3DApplication>( L, "OnHeroShopButton",  &CMyD3DApplication::OnHeroShopButton  );

    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_Mission",  &CMyD3DApplication::AlertCheck_Mission );
    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_MSN",  &CMyD3DApplication::AlertCheck_MSN );
    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_Calendar",  &CMyD3DApplication::AlertCheck_Calendar );
    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_MyInfoScene",  &CMyD3DApplication::AlertCheck_MyInfoScene );
    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_SkillTree",  &CMyD3DApplication::AlertCheck_SkillTree );
    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_CoupleWnd",  &CMyD3DApplication::AlertCheck_CoupleWnd );
    lua_tinker::class_def<CMyD3DApplication>( L, "AlertCheck_Recommend",  &CMyD3DApplication::AlertCheck_Recommend );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetDungeonLevel",  &CMyD3DApplication::GetDungeonLevel );
    lua_tinker::class_def<CMyD3DApplication>( L, "CreateMeshAnimationForPrevLoad",  &CMyD3DApplication::CreateMeshAnimationForPrevLoad );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetActiveStage",  &CMyD3DApplication::GetActiveStage );
    lua_tinker::class_def<CMyD3DApplication>( L, "CheckDunMonsterSummonSlot",  &CMyD3DApplication::CheckDunMonsterSummonSlot );
    lua_tinker::class_def<CMyD3DApplication>( L, "AddGameItem",  &CMyD3DApplication::AddGameItem );
    lua_tinker::class_def<CMyD3DApplication>( L, "Setv3TempValue",  &CMyD3DApplication::Setv3TempValue );
    lua_tinker::class_def<CMyD3DApplication>( L, "Getv3TempValue",  &CMyD3DApplication::Getv3TempValue );
    lua_tinker::class_def<CMyD3DApplication>( L, "WinMessageBox",  &CMyD3DApplication::WinMessageBox );
    lua_tinker::class_def<CMyD3DApplication>( L, "GetCurrentGameMode",  &CMyD3DApplication::GetCurrentGameMode );

	lua_tinker::class_mem<CMyD3DApplication>( L, "Story",      &CMyD3DApplication::Story );

    lua_tinker::decl( L, "g_MyD3D", this );

    lua_tinker::class_add< SServerInfo >( L, "SServerInfo"  );
    lua_tinker::class_mem<SServerInfo>( L, "dwServerType",      &SServerInfo::dwServerType  );
    lua_tinker::class_def<SServerInfo>( L, "CheckServerType",   &SServerInfo::CheckServerType  );;


    lua_tinker::class_add<KGCGlobal>( L, "KGCGlobal"  );
    lua_tinker::class_mem<KGCGlobal>( L, "bFrameLock",      &KGCGlobal::m_bFrameLock  );    
    lua_tinker::class_mem<KGCGlobal>( L, "bNoCameraZoom",  &KGCGlobal::m_bNoCameraZoom );
    lua_tinker::class_def<KGCGlobal>( L, "GetUserGP",       &KGCGlobal::GetUserGP );
    lua_tinker::class_def<KGCGlobal>( L, "GetCurrentCharacterGP",       &KGCGlobal::GetCurrentCharacterGP );
    lua_tinker::class_def<KGCGlobal>( L, "GetServerInfo",   &KGCGlobal::GetServerInfo  );
    lua_tinker::class_def<KGCGlobal>( L, "AddPetTemplate",  &KGCGlobal::AddPetTemplate );
    lua_tinker::decl( L, "g_kGlobalValue", &g_kGlobalValue );

    SPetTemplate::BindToLua( L );

    lua_tinker::def( L,"KGCPC_BEGIN",KGCPerformanceCounter::Begin);
	lua_tinker::def( L,"KGCPC_END",KGCPerformanceCounter::End);

    lua_tinker::class_add<KDropItemInfo>( L, "KDropItemInfo" );
    lua_tinker::class_mem<KDropItemInfo>( L, "itemid", &KDropItemInfo::m_ItemID );
    lua_tinker::class_mem<KDropItemInfo>( L, "duration", &KDropItemInfo::m_nDuration );
    lua_tinker::class_mem<KDropItemInfo>( L, "period", &KDropItemInfo::m_nPeriod );
    lua_tinker::class_con<KDropItemInfo>( L, lua_tinker::constructor<>() );

    lua_tinker::class_def<CMyD3DApplication>( L, "Num_TrainingObj",  &CMyD3DApplication::GetTrainingNum );
    lua_tinker::class_def<CMyD3DApplication>( L, "Clear_TrainingTile",  &CMyD3DApplication::ClearTrainingTile );


    lua_tinker::def( L, "GCStrCharToWide", &GCUTIL_STR::GCStrCharToWide );

    g_pItemMgr->BindToLua();
    m_pStateMachine->BindToLua();
    //m_kGuideMgr.BindToLua();
	g_kMonsterManager.RegisterLuabind();
}

void CMyD3DApplication::AddBasiliskHP( float fChange )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit !=				g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( !pMonster->m_bLive )
			continue;
		if( pMonster->m_fHP <= 0 )
			continue;
		//if( pMonster->Now_Monster == MON_DRAGON )
		//    pMonster->m_fHP += fChange;

		pMonster->m_fHP = min( pMonster->m_fHP, pMonster->m_fHPPoint );
		return;
	} 
}

HWND CMyD3DApplication::Get_hWnd(){ return m_hWnd; }


/** OneTimeSceneInit() : 애플리캐이션이 시작되기 전에 단 한번 호출된다.
처음 시작할 때 로드되어 끝날 때 해제되는 영구성 데이터를 로드한다.
*/
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
    // 윈도우 활성화
    SetActiveWindow(m_hWnd);

    // 서버 초기화
    //if( StartUpServer() != S_OK )
    // return E_FAIL;

    LoadBadWord();

	//국가별로 입력되어야 하는 언어를 제한합니다.
  //이제 이 처리는 GCStateInitLoading.cpp의 LoadResource 부분에서 합니다.
//#if defined( NATION_KOREA ) 
//    //SiKSlangFilter()->AddLanguage( 0x002F, 0x0039 );
//    //SiKSlangFilter()->AddLanguage( 0x0020, 0x005A );
//    //SiKSlangFilter()->AddLanguage( 0x0061, 0x007A );
//    SiKSlangFilter()->AddLanguage( 0x0020, 0x007F );
//    SiKSlangFilter()->AddLanguage( 0xAC00, 0xD7AF );
//    SiKSlangFilter()->SetEnableUnicode( true );   
//#elif defined( NATION_USA )
//    //    //SiKSlangFilter()->AddLanguage( 0x002F, 0x0039 );
//    //    //SiKSlangFilter()->AddLanguage( 0x0020, 0x005A );
//    //    //SiKSlangFilter()->AddLanguage( 0x0061, 0x007A );
//    //    SiKSlangFilter()->AddLanguage( 0x0020, 0x007F );
//    //    SiKSlangFilter()->SetEnableUnicode( true ); 
//#elif defined( NATION_BRAZIL )
//	SiKSlangFilter()->AddLanguage( 0x0020, 0x007F );
//	SiKSlangFilter()->AddLanguage( 0x0080, 0x00ff );
//	SiKSlangFilter()->AddLanguage( 0xAC00, 0xD7AF );
//    SiKSlangFilter()->SetEnableUnicode( true );    
//#endif


    D3DXMatrixIdentity(&Identity);

    // 쉐이딩 메트릭스 설정
    m_mat._11 = 0.5f/.35f; m_mat._12 = 0.0f; m_mat._13 = 0.0f; m_mat._14 = 0.0f;
    m_mat._21 = 0.0f; m_mat._22 =-0.5f/.35f; m_mat._23 = 0.0f; m_mat._24 = 0.0f;
    m_mat._31 = 0.0f; m_mat._32 = 0.0f; m_mat._33 = 1.0f; m_mat._34 = 0.0f;
    m_mat._41 = 0.5f; m_mat._42 = 0.5f; m_mat._43 = 0.0f; m_mat._44 = 1.0f;

    return S_OK;
}

void CMyD3DApplication::LoadBadWord()
{
	// 모든 스트링 파일을 집어넣겠다.
	return;

    if( SiKGCMultipleLanguages()->UseMultipleLanguages() == true )
        return;

#if defined(_JAPANESE) // 일본어 (일본)
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_j.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_j.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total_j.stg" ).c_str() );
#elif defined(_CHINESE_SIMPLIFIED) // 중국어 간체 (중국)
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_cn.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_cn.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total_cs.stg" ).c_str() );
#elif defined(_CHINESE_TRADITIONAL) // 중국어 번체 (대만)
#if defined( NATION_HONGKONG )
    {
        SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_c_ct.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );
        SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_n_ct.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    }
#else
    {
        SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_ct.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );
        SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_ct.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    }
#endif
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total_ct.stg" ).c_str() );
#elif defined(_PORTUGUESE) // 포르투갈어 (포르투갈,브라질)
#ifdef NATION_BRAZIL
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_br.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_br.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total_br.stg" ).c_str() );
#else
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_p.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_p.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total_p.stg" ).c_str() );
#endif
#elif defined(_THAI_LANG) // 태국어 (태국)
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_th.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_th.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
#elif defined(_ENGLISH) // 영어 (미국,영국,필리핀)
#if defined( NATION_PHILIPPINE )
    //SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_ph.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    //SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_ph.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
#elif defined( NATION_IDN )
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_id.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_id.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
#else
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_en.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_en.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total_en.stg" ).c_str() );
#endif
#elif defined(_SPANISH) // 스페인어
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_la.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total_la.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
#else // defined(_KOREAN) // 한국어 (한국)
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total.stg" ).c_str(), KSlangFilter::SLANG_FILTER_CHAT );    
    SiKSlangFilter()->LoadFilter( std::string( g_strCurrentPath + "Stage\\Total.stg" ).c_str(), KSlangFilter::SLANG_FILTER_NICKNAME );
    //g_kCurseFilter.LoadCurses( std::string( g_strCurrentPath + "Stage\\Total.stg" ).c_str() );
#endif
}

// 프레임당 한 번씩 호출된다.
HRESULT CMyD3DApplication::FrameMove()
{
    PROFILE_BLOCKSTART();
    PROFILE_SET("CMyD3DApplication::FrameMove");


#if (defined( _DEBUG ) || !defined( __PATH__ )) && defined(SKILL_TREE)
    if( bReloadSKT )
    {
        KLuaManager kSkillTreeLua;
        GCFUNC::LoadLuaScript(kSkillTreeLua, "Enum.stg" );
        GCFUNC::LoadLuaScript(kSkillTreeLua, "MotionEnum.stg" );

        for( int i = 0; i < GC_CHAR_NUM; i++ ) {
            for( int j = 0; j < NUM_JOB_LEVEL; j++ ) {
                std::stringstream strSkillScript;
                strSkillScript << "InitSkillTree" << i << "_" << j << ".lua";
                GCFUNC::LoadLuaScript( kSkillTreeLua, strSkillScript.str() );
            }
        }
#ifndef __PATH__
        SiGCSKT()->_D_ReloadUIinfo( kSkillTreeLua );
#endif


        g_pkUIScene->m_pkSkillTree->Update();
        g_pkUIScene->m_pkSkillTree->UpdateTab();
        bReloadSKT = false;
    }
#endif

	if(SiKGCDragManager()->GetDragState() == KGCDragManager::KGDM_DRAG_PROGRESS)
		SiKGCDragManager()->ProgressDrag();

#if defined( USE_SHADER_LOAD_HLSL )
    g_pRenderState->CheckFileMonitor();
#endif


    // 프레임 카운트 변수를 업데이트한다.
    o3 = ++o3 % 3;
    o5 = ++o5 % 5;
    o6 = ++o6 % 12;
    o10 = ++o10 % 20;
    o30 = ++o30 % 50;
    iGC_FPS = ++iGC_FPS % GC_FPS_LIMIT;
    if( iGC_FPS == 0 ) ++iSecondCount;

    // 게임 시간을 누적시킨다.
    g_kGlobalValue.m_fTime += GC_ELAPSED_TIME;

    // 060218. jseop. nProtect game guard timer.
    g_fNPCheckTime += GC_ELAPSED_TIME;

    // 060218. jseop. nProtect game guard check.
    if( g_fNPCheckTime >= NPGG_CHECK_INTERVAL )
    {
        g_fNPCheckTime = 0.0f;
        if( !NPGameGuardCheck() )
        {
            g_kGlobalValue.m_strDetectedHackTool = g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM );
            ExitWithHackAlert( L"NPGG" );
        }
    }
#ifndef __PATH__
	if( diks[DIK_NUMPAD9] && !Old_diks[DIK_NUMPAD9])
	{
		DWORD nFailedType_ =0;
		int nSecond_ =0;
		DWORD dwRoomTotalUser_ =0;
		DWORD dwMapID_ =0;
		int nFailType_ =0;
		DWORD dwFailReason_=0;

		KLuaManager luaMgr;
		GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

		LUA_GET_VALUE_DEF( "nFailedType_", nFailedType_, 0 );    
		LUA_GET_VALUE_DEF( "nSecond_", nSecond_, 0 );    
		LUA_GET_VALUE_DEF( "dwRoomTotalUser_", dwRoomTotalUser_, 0 );    
		LUA_GET_VALUE_DEF( "dwMapID_", dwMapID_, 0 );    
		LUA_GET_VALUE_DEF( "nFailType_", nFailType_, 0 );    
		LUA_GET_VALUE_DEF( "dwFailReason_", dwFailReason_, 0 );    


		KP2P::GetInstance()->Send_ClientFailedGameStartNot(nFailedType_, nSecond_, dwRoomTotalUser_, dwMapID_, nFailType_,dwFailReason_);
	}
#endif

//==============================================================================
// 2009.04.21 : tcpl85
// Comment : XTrap Check Time 

#if defined(USE_XTRAP) && !defined(_DEBUG)
	g_fXTrapCheckTiem += GC_ELAPSED_TIME;

	if( g_fXTrapCheckTiem >= XTRAP_CHECK_INTERVAL )
	{
		g_fXTrapCheckTiem = 0.0f;
		
		XTrap_C_CallbackAlive( static_cast<DWORD>( XTRAP_CHECK_INTERVAL * 1000) );
	}
#endif

	if (m_pStateMachine->GetState() == GS_INIT_LOADING)
	{
		m_pStateMachine->FrameMove( GC_ELAPSED_TIME );
		return S_OK;
	}

#ifdef NATION_KOREA
    {
        if( timeGetTime() - g_kGlobalValue.m_dwLastMsgDisplayTime > 3600000 )//1시간 마다 메세지 표시해줌
        {
            static int iHour = 0;
            iHour++;
            WCHAR strMsg[1000];
            if( iHour < 3 )
                swprintf( strMsg, 1000, L"%d시간이 경과 하였습니다.", iHour );
            else
                swprintf( strMsg, 1000, L"%d시간이 경과했습니다. 잠시 쉬었다 하시기 바랍니다.", iHour );
            if( g_pkChatManager )
                g_pkChatManager->AddChatMsg( strMsg, KGCChatManager::CHAT_TYPE_ALERT );

            if( g_pkChatManager )
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_YOUNGER_BOY_CONNECT1 ), KGCChatManager::CHAT_CMD_BROADCAST );

            g_pkUIScene->StartPlayTimeNotice( 0, iHour );

            g_kGlobalValue.m_dwLastMsgDisplayTime = timeGetTime();

            SiKGCAchieveManager()->OccurAction( SAchieve::PLAY_TIME, GC_CHAR_INVALID, iHour );
        }
    }
#endif //NATION_KOREA

	if( g_kGlobalValue.m_mapCommonMsg.size() > 0 )
	{
		// map<공지시간간격, pair<남은시간간격, vector<공지메시지> > >
		std::map< int, std::pair< DWORD, std::vector<std::wstring> > >::iterator itCommonMsg = g_kGlobalValue.m_mapCommonMsg.begin();

		for(;itCommonMsg != g_kGlobalValue.m_mapCommonMsg.end();itCommonMsg++)
		{
			if( timeGetTime() - itCommonMsg->second.first > 60000*(DWORD)itCommonMsg->first )
			{
				itCommonMsg->second.first = timeGetTime();

				for(int i=0;i< (int)itCommonMsg->second.second.size();i++)
				{
					if( g_pkChatManager )
						g_pkChatManager->AddChatMsg( itCommonMsg->second.second[i]
					, KGCChatManager::CHAT_CMD_BROADCAST );
				}
			}
		}
	}

    g_LatencyTransfer.FrameMove();
    m_KTDXTimer.FrameMove( GC_ELAPSED_TIME );
    //NetworkProc( GC_ELAPSED_TIME );
    SiKGCAnnounceMgr()->FrameMove();
    if( FAILED(m_pStateMachine->FrameMove( GC_ELAPSED_TIME ) ) )
        return S_OK;    

#if defined ( USE_GUIDE_SYSTEM )
        m_kGuideMgr.FrameMove();
#endif

#ifndef DEV_BUILD
    DiscordHelper::GetInstance()->Refresh();
    DiscordHelper::GetInstance()->UpdatePresence();
#endif

#if !defined( __PATH__ )
    GifFrameMove();
#endif

    PROFILE_BLOCKEND();
    PROFILE_DUMPOUTPUT_FILE();

    return S_OK;
}

PLAYER* CMyD3DApplication::GetMyPlayer()
{
	// 리플레이 모드에서는 옵져버로 설정되어있지만 
	// 실제 옵져버의 데이터에는 값이 없으므로 실제 플레이어 데이터를 가져가도록 하자!
	if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW || g_MyD3D->MyReplay->m_iCurrState == RP_PREPARE)
	{
		for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
		{
			if ( MyPlayer[i]->IsLocalPlayer() )
			{
				return MyPlayer[i];
			}
		}
		return &g_MyD3D->m_TempPlayer;
	}

	//옵저버면 자기 자신이 없기 때문에 적절한 녀석으로 리턴한다.
    if( g_MyD3D->IsLocalPlayerObserver() )
    {
        return &g_MyD3D->m_TempPlayer;
    }

    // 아래 MAX_PLAYER 루프를 안돌게 하려고 추가했음.
    if( IsPlayerIndex( g_kGlobalValue.m_iLocalPlayerIndex ) )
    {
        int iMyPlayer = g_kGlobalValue.m_iLocalPlayerIndex;
        if ( MyPlayer[iMyPlayer]->IsLocalPlayer() )
            return MyPlayer[iMyPlayer];
    }

	for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( MyPlayer[i]->IsLocalPlayer() ||
			( g_MyD3D->IsLocalPlayerObserver() && MyPlayer[i]->m_kUserInfo.bHost ) )
        {
            g_kGlobalValue.m_iLocalPlayerIndex = i;
            return MyPlayer[i];
        }
    }
    return NULL;
}

PLAYER* CMyD3DApplication::GetPlayer(int iPlayer)
{ 
    if ( iPlayer < 0 || iPlayer >= MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO ) 
        return MyPlayer[0];

    return MyPlayer[iPlayer]; 
}

int CMyD3DApplication::GetPlayerNum()
{
    int iNum = 0;
    for(int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( MyPlayer[i]->m_kUserInfo.bLive )
            iNum++;
    }
    return iNum;
}

int CMyD3DApplication::GetLivePlayerNum()
{
    int iNum = 0;
    for(int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( MyPlayer[i]->m_kUserInfo.bLive  && ((char)MyPlayer[i]->m_cLife) > 0)
            iNum++;
    }
    return iNum;
}


int CMyD3DApplication::Get_MyPlayer()
{
    // 아래 MAX_PLAYER 루프를 안돌게 하려고 추가했음.
    if (IsPlayerIndex(g_kGlobalValue.m_iLocalPlayerIndex))
    {
        int iMyPlayer = g_kGlobalValue.m_iLocalPlayerIndex;
        if (MyPlayer[iMyPlayer]->IsLocalPlayer())
            return iMyPlayer;
    }

    for (int i = 0; i < MAX_PLAYER_NUM; i++)
    {
        if (MyPlayer[i]->IsLocalPlayer())
        {
            g_kGlobalValue.m_iLocalPlayerIndex = i;
            return i;
        }
    }
    return 7;
}

bool CMyD3DApplication::IsMyTeam( int iPlayer )
{
	if(IsLocalPlayerObserver()) 
		return true;
	if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
		return true;

	if(iPlayer<0 || iPlayer>=MAX_PLAYER_NUM)
		return false; 
    if( Get_MyPlayer() == iPlayer )
        return true;

    // 홀로 싸우는 타입이면 자신만 해당된다.
    if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT )
        return false;

    if( MyPlayer[Get_MyPlayer()]->m_kUserInfo.iTeam != MyPlayer[iPlayer]->m_kUserInfo.iTeam )
        return false;

    return true;
}

bool CMyD3DApplication::IsSameTeam( int iPlayer1, int iPlayer2 )
{
	/*if(IsLocalPlayerObserver()) 
		return true;*/

	if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
		return true;

    if( iPlayer1 == iPlayer2 )
        return true;

    if( false == IsPlayerIndex(iPlayer1) || false == IsPlayerIndex(iPlayer2) )
        return false;

    // 홀로 싸우는 무조건 다른편
    if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT )
        return false;

    if( MyPlayer[iPlayer1]->m_kUserInfo.iTeam == TEAM_PUBLIC_ENEMY ||
        MyPlayer[iPlayer2]->m_kUserInfo.iTeam == TEAM_PUBLIC_ENEMY ) {
        return false;
    }

    if( MyPlayer[iPlayer1]->m_kUserInfo.iTeam != MyPlayer[iPlayer2]->m_kUserInfo.iTeam )
        return false;

    return true;
}

bool CMyD3DApplication::IsSameOriginalTeam( int iPlayer1, int iPlayer2 )
{
    if(IsLocalPlayerObserver()) 
        return true;
    
    if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
        return true;

    if( iPlayer1 == iPlayer2 )
        return true;

    if( false == IsPlayerIndex(iPlayer1) || false == IsPlayerIndex(iPlayer2) )
        return false;

    // 홀로 싸우는 무조건 다른편
    if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT )
        return false;

//  오리지널 팀은 해당 팀을 가질수 없다.
//     if( MyPlayer[iPlayer1]->m_iOriginalTeam == TEAM_PUBLIC_ENEMY ||
//         MyPlayer[iPlayer2]->m_iOriginalTeam == TEAM_PUBLIC_ENEMY ) {
//             return false;
//     }

    //  로딩 중일때는 아직 팀이 설정되어 있지 않다.
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING ) 
    {
        if( MyPlayer[iPlayer1]->m_kUserInfo.iTeam != MyPlayer[iPlayer2]->m_kUserInfo.iTeam )
            return false;
    }
    else
    {
        if( MyPlayer[iPlayer1]->m_iOriginalTeam != MyPlayer[iPlayer2]->m_iOriginalTeam )
            return false;
    }

    return true;


}

bool CMyD3DApplication::IsSameTeam_PlayerAndMonster( int iPlayer, int iMonster )
{
    if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
        return true;
	
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonster);
	if( pMonster  == NULL ) 
		return false;
    if( false == IsPlayerIndex(iPlayer) )
        return false;

    // 홀로 싸우는 모드일때는 엄마가 아니면 무조건 다른편
    if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT )
    {
        if( MyPlayer[iPlayer]->m_kUserInfo.dwUID != pMonster->GetSummonPlayerUID() )
            return false;
    }

    if( MyPlayer[iPlayer]->m_kUserInfo.iTeam == TEAM_PUBLIC_ENEMY ) {
            return false;
    }

    if( MyPlayer[iPlayer]->m_kUserInfo.iTeam != pMonster->m_iTeam )
        return false;

    return true;
}

bool CMyD3DApplication::IsSameTeam_MonsterAndMonster( int iMonster1, int iMonster2 )
{
    if( iMonster1 == iMonster2 )
        return true;

	MONSTER* pMonster1 = g_kMonsterManager.GetMonster(iMonster1);
	MONSTER* pMonster2 = g_kMonsterManager.GetMonster(iMonster2);
	if( pMonster1  == NULL || pMonster2 == NULL) 
		return false;

    // 홀로 싸우는 모드일때는 엄마가 다르면 무조건 다른편
    if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_ALONE_FIGHT )
    {
        if( pMonster1->GetSummonPlayerUID() != pMonster2->GetSummonPlayerUID() )
            return false;
    }

    if( pMonster1->m_iTeam != pMonster2->m_iTeam )
        return false;

    return true;
}

int CMyD3DApplication::Get_Player( DWORD ID )
{
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( MyPlayer[i]->m_kUserInfo.bLive && MyPlayer[i]->m_kUserInfo.dwUID == ID )
        {
            return i;
        }
    }
    return 7;
}
int CMyD3DApplication::Get_Player( std::wstring strLogin_ )
{
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( MyPlayer[i]->m_kUserInfo.bLive && MyPlayer[i]->m_kUserInfo.strLogin == strLogin_ )
        {
            return i;
        }
    }
    return 7;
}

KPet* CMyD3DApplication::Get_Players_Pet( DWORD dwPlayerID, DWORD dwPetID )
{
	if( true == KPet::IsPetAvailableState() )
	{
    return g_MyD3D->m_akPet[dwPlayerID];
	}
	else
	{
		GCSquarePet* pSquarePet = SiGCSquare()->GetSquarePetPtr( dwPlayerID );
		ASSERT( pSquarePet != NULL );
		return pSquarePet->GetPetPtr();
	}
}

int CMyD3DApplication::Get_HostPlayer()
{
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( MyPlayer[i]->m_kUserInfo.bLive && MyPlayer[i]->m_kUserInfo.bHost )
        {
            return i;
        }
    }
    return 7;
}

KGCShortCutSlot* CMyD3DApplication::GetMyShortCutSlot(void)
{
    ASSERT( MyShortCutSlot != NULL );
    return MyShortCutSlot;
}
// Render() : 프레임당 한번 씩 호출된다.
HRESULT CMyD3DApplication::Render()
{

    //핵 감지시 작동됨
	CheckBefor();

    RandomShutdown();

#if defined(SHA_CHECK)
        KSHAChecker::GetInstance()->CountingLimitTime();
#endif

    m_pStateMachine->Render();

    return S_OK;
}

/** InitDeviceObjects() : This creates all device-dependent managed objects, such as managed
textures and managed vertex buffers.
*/
HRESULT CMyD3DApplication::InitDeviceObjects()
{
    HRESULT hr = S_OK;
    TCHAR szProgram[MAX_PATH];
    STARTUPINFO si = {0, };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    BOOL bok = FALSE;

    ::_stprintf(szProgram, _T( "regsvr32 /s quartz.dll" ));
    bok = CreateProcess( NULL, szProgram, NULL, NULL, FALSE, 0, NULL, L".\\", &si, &pi );
    ::_stprintf(szProgram, _T( "regsvr32 /s l3codecx.ax" ));
    bok = CreateProcess( NULL, szProgram, NULL, NULL, FALSE, 0, NULL, L".\\", &si, &pi );


    // 멀티 텍스쳐링이 가능한지 검사한다.
    D3DCAPS9 pCaps;
    m_pd3dDevice->GetDeviceCaps( &pCaps );
    if( pCaps.MaxSimultaneousTextures > 1 && (pCaps.TextureOpCaps & D3DTEXOPCAPS_ADDSIGNED) )
    {
        EnableMultiMap = true;
    }
    else
    {
        //if( m_KGCOption.IsCartoon() )
        m_KGCOption.SetCartoon( false );

        EnableMultiMap = false;
    }

    if( pCaps.TextureOpCaps & D3DTEXOPCAPS_MODULATE2X )
        EnableModulate2X = true;
    else
    {
        EnableModulate2X = false;
        m_KGCOption.SetModulate2X( false );
    }

    m_KGCBlurMgr.InitDeviceObjects();
    m_kAfterImageRenderer.InitDeviceObjects();

    m_KGCOption.InitDevice( m_pd3dDevice );

    //디바이스 매니져
    GCDeviceManagerWrapper* pGCDeviceManager = new GCDeviceManagerWrapper( m_pd3dDevice );
    pGCDeviceManager->SetWindowMode( IsWindowMode() == TRUE );

    SiKTextureLoadManager()->Init();

#if !defined( __PATH__ )
    SiKTextureLoadManager()->InitLog();
#endif

    SiKTextureLoadManager()->BeginThread();

    g_pRenderState = pGCDeviceManager->CreateShader("GrandChase.kfx");
    g_RenderManager->SetDeviceShader( g_pRenderState );

    //g_pMagicEffect = new KGCMagicEffect;
    g_pMagicEffect = new KGCBuffManager();
    g_pGameBoard = new KGCGameBoard;
    g_ParticleManager = new KGCParticleManager;
    //g_pMeshEffectManager = new KGCMeshEffectManager;
    g_pRoute = new KGCRoute;
    MyReplay = new Replay;
        
    

#if defined( _DEBUG ) || !defined( __PATH__ )
    g_pGCDeviceManager->GetMassFileManager()->SetRealFileFirst(true);
#endif // !__PATH__ && NATION_KOREA

    g_pGraphicsHelper = new GCGraphicsHelper( m_pd3dDevice );

    std::string strUIFileName = "script.stg";

    m_pkUIMgr = new KD3DUIManager();

    RegisterUIFactoryClass();

    return S_OK;
}

/** RestoreDeviceObjects() : Restore device-memory objects and state after a device is created or
resized.
*/
HRESULT CMyD3DApplication::InitUIFiles(std::string strUIFileName)
{
    HRESULT hr = S_OK;

    if ( FAILED( hr = m_pkUIMgr->OneTimeSceneInit( m_hWnd, strUIFileName ) ) )
        //KLOG((0, "UI 스크립트 로딩에 실패했습니다."));
        KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_UI_SCRIPT_FAILED1 ).c_str(),
        g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_UI_SCRIPT_FAILED2 ).c_str(),
        KMsgBox::MBOX_OK );
    return hr;
}

HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
	//KD3DDeviceState::RestoreDeviceObjects( 0, D3DDEVTYPE_HAL );
    g_pGCDeviceManager2->OnResetDevice();

    SetLight();
    D3DMATERIAL9 Mtrl;
    D3DUtil_InitMaterial(Mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
    m_pd3dDevice->SetMaterial( &Mtrl);

    // Set the matrix for making view matrix
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR3 vEyePt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 1.0f);

    // Set the app view matrix
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec);
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // Set the transform matrices (view and world are updated per frame)

    D3DXMatrixOrthoLH( &matProj, 2.0f, 1.5f, .1f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	//일단 State는 손대지 않도록 수정-_-;
	g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
    g_pGCDeviceManager2->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

    // Set up the default texture states
    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

    g_pGCDeviceManager2->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
    g_pGCDeviceManager2->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
    g_pGCDeviceManager2->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );

    g_pGCDeviceManager2->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

    g_pGCDeviceManager2->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
    g_pGCDeviceManager2->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );

    if(m_KGCOption.IsLinearFilter())
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    }
    else
    {
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    }
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR );
    g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR );
    g_pGCDeviceManager2->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR );

    g_pGCDeviceManager2->SetRenderState( D3DRS_DITHERENABLE, FALSE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_LIGHTING, FALSE );

    g_pGCDeviceManager2->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );

    m_KGCBlurMgr.RestoreDeviceObjects();
    m_kAfterImageRenderer.RestoreDeviceObjects();
    return S_OK;
}


/** InvalidateDeviceObjects() : Called when the device-dependent objects are about to be lost.
*/
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
    //KD3DDeviceState::InvalidateDeviceObjects();
	if( g_pGCDeviceManager2 )
		g_pGCDeviceManager2->OnLostDevice();
    m_KGCBlurMgr.InvalidateDeviceObjects();
    m_kAfterImageRenderer.InvalidateDeviceObjects();

    return S_OK;
}




/** DeleteDeviceObjects() : Called when the app is exiting, or the device is being changed,
this function deletes any device dependent objects.
*/
// 릴리즈 순서 조심해야 할듯!!
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
    // Object 소멸시 m_pMapAnim, g_ParticleManager 객체를 사용하는 Object가 있다. (서브클래스에서...)
    // 그래서 그전에 Object를 모두 소멸해준다.
    for(int i=0;i<MAX_OBJECT;++i)
        SAFE_DELETE( m_pObject[i] );

	// 누구보다 빠르고 남들과는 다르게 싱글톤 주제에 맨 첨에 지워주자
	// 사실 다른 싱글톤들도 디바이스보다 먼저 지워야 할것같음
    SiKTextureLoadManager()->EndThread();
    SiKTextureLoadManager()->ReleaseInstance();
    SiKGCProfileManager()->ReleaseInstance();
    SiGCSquare()->ReleaseInstance();
	SiKGC3DObjectMgr()->ReleaseInstance();
	KGCEventManager::ReleaseInstance();
	SiKGCValentineDayManager()->ReleaseInstance();
	SiKGCCoupleSystem()->ReleaseInstance();
	// Sigleton 객체들 릴리즈..
	SiGCPairPlaySystem()->ReleaseInstance();
	SiKSlangFilter()->ReleaseInstance();
	SiKGCWorldMapManager()->ReleaseInstance();
    SiKGCPrivateDungeonManager()->ReleaseInstance();
    SiKGCInfinityDungeonManager()->ReleaseInstance();
    SiKGCChannelManager()->ReleaseInstance();
	SiKGCRoomManager()->ReleaseInstance();
	SiKGCMissionManager()->ReleaseInstance();
	SiKGCCTMissionManager()->ReleaseInstance();
	SiKGCDropItemManager()->ReleaseInstance();
	//KGCEventManager::ReleaseInstance();
	SiKGCFantasticMap()->ReleaseInstance();
	KSingleton<KEnchantStrong>::ReleaseInstance();
	SiKGCElvis()->ReleaseInstance();
	//SiKGC3DObjectMgr()->ReleaseInstance();
	SiKNeonSign()->ReleaseInstance();
	SiKGCInviteManager()->ReleaseInstance();

	SiKD3DEmoticonString()->ReleaseInstance();
	SiKGCAnnounceMgr()->ReleaseInstance();
#if !defined( __PATH__ )
	SiKGCFileMoniter()->ReleaseInstance();
#endif
	SiKGCEmoticonManager()->ReleaseInstance();
	SiKGCMinigameManager()->ReleaseInstance();

	SiKFriendShip()->ReleaseInstance();

	SiKGCMath()->ReleaseInstance();
	SiKGCContributionManager()->ReleaseInstance();
    SiGCForgeManager()->ReleaseInstance();
    SiKGCPartyManager()->ReleaseInstance();
    SiKGCDragManager()->ReleaseInstance();
	SiGCAdventureManager()->ReleaseInstance();
	SiKGCLevelResetEventManager()->ReleaseInstance();	
	SiGCTextColorManager()->ReleaseInstance();
    SiGCSongkranEventManager()->ReleaseInstance();
    SiGCOlympicManager()->ReleaseInstance();
	SiKGCInGamePrograssBar()->ReleaseInstance();

    SiKGCWeatherSystem()->ReleaseInstance();

	SiGCFaceOffManager()->ReleaseInstance();

	
	SiKGCMatchManager()->ReleaseInstance();
    SiGCItemCombinationManager()->ReleaseInstance();
    SiKGCEmbarkManager()->ReleaseInstance();

    SiGCAutoMatchManager()->ReleaseInstance();

    SiGCBuffDonationManager()->ReleaseInstance();
    SiGCAttendanceStampManager()->ReleaseInstance();
    SiGCChallengeErrandManager()->ReleaseInstance();

    SiKGCWallManager()->ReleaseInstance();
    SiKGCSubjectManager()->ReleaseInstance();
	
	SiKGCGiftBoxManager()->ReleaseInstance();
    SiKGCJumpingCharManager()->ReleaseInstance();
	

    SiGCClientErrorManager()->ReleaseInstance();
    SiKGCNetworkStatManager()->ReleaseInstance();
    SiKGCVirtualDepotManager()->ReleaseInstance();
    SiKGCSocialCommerceManager()->ReleaseInstance();
	//g_BackGroundLoader.Shutdown();


    m_KGCBlurMgr.DeleteDeviceObjects();
    m_kAfterImageRenderer.DeleteDeviceObjects();
    if (MySparks)
        MySparks->DeleteDevice();

    if (MyCtrl)
        MyCtrl->DeleteDevice();

    if (MyHead)
        MyHead->DeleteDevice();

    if (MyItms)
        MyItms->DeleteDevice();

    if (MyStg)
        MyStg->DeleteDevice();

    if (m_pDamageManager)
        m_pDamageManager->DeleteDevice();

    //MyModels->DeleteDevice();
    if (m_pkUIMgr)
        m_pkUIMgr->DeleteDeviceObjects();
    m_KGCGuildMark.DeleteDevice();

    g_AnimManager.DeleteDeviceObjects();
    if( m_pMapAnim != NULL )
        m_pMapAnim->DeleteDeviceObjects();
    if( m_pPetAnim != NULL )
        m_pPetAnim->DeleteDeviceObjects();

	g_kMonsterManager.DeleteAllMonster();
    SiKGCEmbarkManager()->DestoryAllInstance();
    SiKGCWallManager()->DestoryAllInstance();
    SiKGCSubjectManager()->DestoryAllInstance();

    SAFE_DELETE( MySparks );
    SAFE_DELETE( m_pDamageManager );
    SAFE_DELETE( g_pkPetAI );
    SAFE_DELETE( m_pMapAnim );
    SAFE_DELETE( m_pPetAnim );
    SAFE_DELETE( m_pUIAnim );
    SAFE_DELETE( MyReplay );
    SAFE_DELETE( m_pStateMachine );
    SAFE_DELETE( m_pFrustum );

    SAFE_DELETE( m_pkQuestGameMgr );
    SAFE_DELETE( g_pMagicEffect );
    SAFE_DELETE( g_pGameBoard );
    // particlemanager보다 먼저
    SAFE_DELETE( MyStg );
    SAFE_DELETE( g_ParticleManager );
    //SAFE_DELETE( g_pMissionManager );
    //SAFE_DELETE( g_pMeshEffectManager );
    SAFE_DELETE( g_pRoute );

    //아래부턴 FinalCleanup에서 처리하던 것들
    m_KGCOption.Save_Option();

    SAFE_DELETE( MyCtrl );
    SAFE_DELETE( MyHead );
    //SAFE_DELETE( MyBuff );
    SAFE_DELETE( MyItms );
    SAFE_DELETE( MyShortCutSlot );


    //SAFE_DELETE( MyModels );

    //MyMotions->CleanUp();
    //SAFE_DELETE( MyMotions );


    FreeDirectInput();
    if ( m_pkUIMgr != NULL )
        m_pkUIMgr->FinalCleanup();

    SAFE_DELETE( m_pkUIMgr );

    
    for( int i = 0; i < MAX_PLAYER_NUM+NUM_TEMP_PLAYERINFO ; ++i )
    {
        for(int j=0;j<FORM_NUM;++j)
            SAFE_REMOVE_DELETE( MyPlayer[i]->m_pObjMetaForm[j] );
        SAFE_REMOVE_DELETE( m_akPet[i]->m_pObject );

        MyPlayer[i]->m_pLire3Arrow = NULL;

    }


    SAFE_DELETE( g_pGraphicsHelper );
    SAFE_DELETE( g_pGCDeviceManager2 );
    SAFE_DELETE( g_pkUIHelper );

    g_RenderManager->ReleaseInstance();
    SiGCSKT()->ReleaseInstance();

    return S_OK;
}




/** FinalCleanup() : Called before the app exits, this function gives the app the chance
to cleanup after itself.
*/
HRESULT CMyD3DApplication::FinalCleanup()
{
    return S_OK;
}

/** ConfirmDevice() : Called during device intialization, this code checks the device
for some minimum set of capabilities
*/
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                         D3DFORMAT Format )
{
    if( dwBehavior & D3DCREATE_PUREDEVICE ) return E_FAIL; // GetTransform doesn't work on PUREDEVICE
    if( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) return E_FAIL;

    // This sample uses alpha textures and/or straight alpha. Make sure the
    // device supports them
    // if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
    // return S_OK;
    if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA )
        return S_OK;

    return E_FAIL;
}

bool CMyD3DApplication::CheckTeamCombination( bool bIsBlueTeam )
{
    std::set<int> setTeamComb;

    setTeamComb.clear();

    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if( MyPlayer[i]->m_kUserInfo.cCharIndex == -1 )
            continue;

        if( true == MyPlayer[i]->m_kUserInfo.bLive && bIsBlueTeam == MyPlayer[i]->m_kUserInfo.IsBlueTeam() )
        {
            if( setTeamComb.find( MyPlayer[i]->GetCurrentChar().iCharType ) != setTeamComb.end() )
                return false;

            setTeamComb.insert( MyPlayer[i]->GetCurrentChar().iCharType );
        }
    }

    return ( setTeamComb.size() > 1 ? true : false );
}

void CMyD3DApplication::RegisterUIFactoryClass( void )
{
    static KGCUISceneFactory kGCUISceneFactory;

    m_pkUIMgr->RegisterSceneFactory( "gc_scene", &kGCUISceneFactory );

#define REG_WND_FACTORY(cls) m_pkUIMgr->RegisterWindowFactory( GET_WND_FACTORY_NAME( ##cls ), GET_WND_FACTORY_PTR( ##cls ) )
    REG_WND_FACTORY( GCResultDamage );
    REG_WND_FACTORY( KGCDungeonRankSlot );
    REG_WND_FACTORY( KGCDungeonRankingPanel );
    REG_WND_FACTORY( KGCDungeonRankingClearData );
    REG_WND_FACTORY( KGCDungeonRankingDlg );
    REG_WND_FACTORY( KGCEnchantSubAlchemyPanel );
    REG_WND_FACTORY( KGCEnchantSubBreakupPanel );
    REG_WND_FACTORY( KGCEnchantSubEnchantPanel );
    REG_WND_FACTORY( KGCEnchantSubShopPanel );
    REG_WND_FACTORY( KGCEnchantSubS6Dlg );
    REG_WND_FACTORY( KGCEnchantS6Dlg );
    REG_WND_FACTORY( KGCBossAlram );
    REG_WND_FACTORY( KGCInfinityGuideDlg );
    REG_WND_FACTORY( KGCRankPlayerInfoBar );
    REG_WND_FACTORY( KGCPartyRankInfo );
    REG_WND_FACTORY( KGCComboUI_S6 );
    REG_WND_FACTORY( KGCDungeonDetailWnd );
    REG_WND_FACTORY( KGCPlayerRank );
    REG_WND_FACTORY( KGCGuildBattleInfo );
    REG_WND_FACTORY( KGCCoordiUpgradeDlg );
    REG_WND_FACTORY( KGCCommonItemWnd );
    REG_WND_FACTORY( KGCCoordiComposeDlg );
    REG_WND_FACTORY( KGCReplayBar );
    REG_WND_FACTORY( KGCReplayBoxS6 );
    REG_WND_FACTORY( KGCSquareEnterWndS6 );
    REG_WND_FACTORY( KGCDeathMatchOption );
    REG_WND_FACTORY( KGCChangeRoomTitleBoxS6 );
    REG_WND_FACTORY( KGCCreateRoomBoxS6);
    REG_WND_FACTORY( KGCSingleImage );
    REG_WND_FACTORY( KGCMatchMapBox );
    REG_WND_FACTORY( KGCMatchInfoBox );
    REG_WND_FACTORY( KGCGiftBoxState );
	REG_WND_FACTORY( KGCFriendListChild );
	REG_WND_FACTORY( KGCFriendListTree );
	REG_WND_FACTORY( KGCPresentFriendList );
	REG_WND_FACTORY( KGCPresentCashItemWnd );
    REG_WND_FACTORY( KGCGiftBox );
    REG_WND_FACTORY( KGCItemPopupMenu );
    REG_WND_FACTORY( KGCEscMenuDlg );
    REG_WND_FACTORY( KGCVirtualKeyManager );
    REG_WND_FACTORY( KGCVirtualKey );
    REG_WND_FACTORY( KGCSmartPanel );
	REG_WND_FACTORY( KGCFatigueInfoBox );
	REG_WND_FACTORY( KGCFatigueMessageBox2 );
	REG_WND_FACTORY( KGCFatigueMessageBox );
    REG_WND_FACTORY( KGCStampTitle );
    REG_WND_FACTORY( KGCJumpingCharDlg );
    REG_WND_FACTORY( KGCPopupAttributeSelect );
    REG_WND_FACTORY( KGCCharacterName );
    REG_WND_FACTORY( KGCMatchInfoPanelS6 );
    REG_WND_FACTORY( KGCDramaPhoto );
    REG_WND_FACTORY( KGCStampTitle );
    REG_WND_FACTORY( KGCItemBindDlg );
    REG_WND_FACTORY( KGCAutoMatchRankingReward );
    REG_WND_FACTORY( KGCGachaSelectSlot );
    REG_WND_FACTORY( KGCSuperGachaSelectWnd );
    REG_WND_FACTORY( KGCSuperGachaDlg );
	REG_WND_FACTORY( KGCRitasBoxOpenFrame );
	REG_WND_FACTORY( KGCRitasResultDlg );
	REG_WND_FACTORY( KGCRitasProgressBar );
	REG_WND_FACTORY( KGCRitasRankInfo );
	REG_WND_FACTORY( KGCRitasRankList );
	REG_WND_FACTORY( KGCRitasGoalBasket );
	REG_WND_FACTORY( KGCRitasPlayer );
	REG_WND_FACTORY( KGCRitasHelp );
	REG_WND_FACTORY( KGCRitasRankingDlg );
	REG_WND_FACTORY( KGCRitasObj );
	REG_WND_FACTORY( KGCRitasPanel );
	REG_WND_FACTORY( KGCRitasChristmas );
	REG_WND_FACTORY( KGCBonusItemS6 );
    REG_WND_FACTORY( KGCRoomUserSlotPanelS6 );
    REG_WND_FACTORY( KGCItemCreationSelectBox );
    REG_WND_FACTORY( KGCRoomMatchPanelS6 );
    REG_WND_FACTORY( KGCRoomCharWndS6 );
	REG_WND_FACTORY( KGCBonusBoxUserInfoS6 );
	REG_WND_FACTORY( KGCBonusBoxS6 );
	REG_WND_FACTORY( KGCRoomQuickJoinDlgS6 );
    REG_WND_FACTORY( KGCRoomUserInfoViewS6 );
    REG_WND_FACTORY( KGCRoomUserInfoPopupS6 );
	REG_WND_FACTORY( KGCAutoMatchLoadingBarS6 );
	REG_WND_FACTORY( KGCAutoMatchLoadingS6 );
	REG_WND_FACTORY( KGCMatchUserBtnS6 );
	REG_WND_FACTORY( KGCMatchInviteUserListS6 );
	REG_WND_FACTORY( KGCMatchUserIconS6 );
	REG_WND_FACTORY( KGCGaugeS6 );
	REG_WND_FACTORY( KGCMatchCompleteDlgS6 );
	REG_WND_FACTORY( KGCMatchMapSelectBtnS6 );
	REG_WND_FACTORY( KGCMatchMapSelectDlgS6 );
	REG_WND_FACTORY( KGCMatchPartyViewS6 );
	REG_WND_FACTORY( KGCMatchStartPartS6 );
	REG_WND_FACTORY( KGCMatchWaitPartS6 );
	REG_WND_FACTORY( KGCAutoMatchingS6 );
	REG_WND_FACTORY( KGCMatchWindowS6 );
	REG_WND_FACTORY( KGCLotteryRewardBox );
	REG_WND_FACTORY( KGCLottery );
    REG_WND_FACTORY( KGCOlympicGawibawiboGame );
    REG_WND_FACTORY( KGCItemCombinationStuff );
    REG_WND_FACTORY( KGCItemCombinationDlg );
    REG_WND_FACTORY( KGCLevelItemDlg );
    REG_WND_FACTORY( KGCRoomButtonPanelS6 );	
    REG_WND_FACTORY( KGCRoomDungeonMissionInfoS6 );
    REG_WND_FACTORY( KGCRoomUserCharImgS6 );
    REG_WND_FACTORY( KGCRoomUserSlotS6 );
    REG_WND_FACTORY( KGCRoomDungeonPanelS6 );
    REG_WND_FACTORY( KGCRoomUserPanelS6 );
    REG_WND_FACTORY( KGCRoomTitleS6 );
	REG_WND_FACTORY( KGCRaidDungeonInfoS6 );
    REG_WND_FACTORY( KGC9thEventDlg );
    REG_WND_FACTORY( KGCOlympicGawibawiboGameInfo );
    REG_WND_FACTORY( KGCOlympicGawibawiboGameLose );
    REG_WND_FACTORY( KGCOlympicGawibawiboGamePlay );
    REG_WND_FACTORY( KGCOlympicDicePlay_DiceView );
    REG_WND_FACTORY( KGCOlympicDicePlay );
    REG_WND_FACTORY( KGCOlympicSongkranDlg );
    REG_WND_FACTORY( KGCOlympicRankSlot );
    REG_WND_FACTORY( KGCOlympicRewardSlot );
    REG_WND_FACTORY( KGCOlympicScheduleSlot );
	REG_WND_FACTORY( KGCSceneMenuBtnS6 );
	REG_WND_FACTORY( KGCSceneMenuS6 );
	REG_WND_FACTORY( KGCChatBoxChatKind );
	REG_WND_FACTORY( KGCChatBoxOption );
	REG_WND_FACTORY( KGCHeroDungeonSubInfoS6 );
	REG_WND_FACTORY( KGCChatBoxDlgS6 );
	REG_WND_FACTORY( KGCSelectedCharFrame );
	REG_WND_FACTORY( KGCIntroNickNameFrame );
	REG_WND_FACTORY( KGCIntroMissionFrame );
	REG_WND_FACTORY( KGCIntroMainFrame );
#ifdef NEW_CHAR_SELECT_SCENE
    REG_WND_FACTORY( KGCCharInfoFrame_New );
#else
	REG_WND_FACTORY( KGCCharInfoFrame );
#endif
	REG_WND_FACTORY( KGCDungeonTitleColorS6 );
	REG_WND_FACTORY( KGCRoomDungeonBtnS6 );
	REG_WND_FACTORY( KGCRoomCreateDlgS6 );
	REG_WND_FACTORY( KGCRoomDungeonSelectDlgS6 );
	REG_WND_FACTORY( KGCRoomBtnBackS6 );
	REG_WND_FACTORY( KGCRoomBtnS6 );
	REG_WND_FACTORY( KGCRoomListBtnSetS6 );
	REG_WND_FACTORY( KGCDungeonBossInfoS6 );
	REG_WND_FACTORY( KGCDungeonSubInfoS6 );
	REG_WND_FACTORY( KGCDungeonTitleS6 );
	REG_WND_FACTORY( KGCDungeonInfoS6 );
	REG_WND_FACTORY( KGCRoomListS6 );
	REG_WND_FACTORY( KGCSongkranWaterEffect );
    REG_WND_FACTORY( KGCPlaytimeNotice );
    REG_WND_FACTORY( KGCOlympicEventDlg );
	REG_WND_FACTORY( KGCSongkranRewardDlg );
	REG_WND_FACTORY( KGCEventAlarmDlg );
	REG_WND_FACTORY( KGCSongkranDlg );
    REG_WND_FACTORY( KGCEclipseTime );
	REG_WND_FACTORY( KGCRoomListDlgS6 );
	REG_WND_FACTORY( KGCMoveModel );
	REG_WND_FACTORY( KGCSystemGateCount );
    REG_WND_FACTORY( KGCVipReward );
	REG_WND_FACTORY( KGCDepotChar );
	REG_WND_FACTORY( KGCDepotDlg );
    REG_WND_FACTORY( KGCBookForm );
    REG_WND_FACTORY( KGCEclipseOldBooks );
    REG_WND_FACTORY( KGCEventEclipsePlotDlg );
	REG_WND_FACTORY( KGCNewNecklaceEnchantDlg );	
	REG_WND_FACTORY( KGCMonsterCardInven );
	REG_WND_FACTORY( KGCMonsterCardRoulette );
	REG_WND_FACTORY( KGCMonsterCardCompose );
    REG_WND_FACTORY( KGCHotTimeSearchDlg );
    REG_WND_FACTORY( KGCItemLevelDownDlg );
    REG_WND_FACTORY( KGCEscortMonHPBar );
    REG_WND_FACTORY( KGCEscortMerryDesc );
    REG_WND_FACTORY( KGCHeroDungeonReward );
    REG_WND_FACTORY( KGCGwcRewardCatalogDlg );
    REG_WND_FACTORY( KGCNotifyMessage );
    REG_WND_FACTORY( KGCCoordiComposeBox );
    REG_WND_FACTORY( KGCCoordiItemInformationBox );	
    REG_WND_FACTORY( KGCGameInvenEquipSlot );
    REG_WND_FACTORY( KGCGameInvenItemSlot );
    REG_WND_FACTORY( KGCGameInventory );
    REG_WND_FACTORY( KGCRecvLetterUI );
    REG_WND_FACTORY( KGCRecvLetterSlot );
    REG_WND_FACTORY( KGCPostLetterBtn );
    REG_WND_FACTORY( KGCPostLetter );
    REG_WND_FACTORY( KGCMsgShutDown );
    REG_WND_FACTORY( KGCSurveyExample );
    REG_WND_FACTORY( KGCSurveyForm );
    REG_WND_FACTORY( KGCSurveyDlg );
    REG_WND_FACTORY( KGCNewForgeItemCraftMachine );
    REG_WND_FACTORY( KGCNewForgeItemBreakUp );
    REG_WND_FACTORY( KGCNewForgeItemCreation );
    REG_WND_FACTORY( KGCNewForgeDlg );
    REG_WND_FACTORY( KGCFairyTreeLevelGauge );
    REG_WND_FACTORY( KGCAgitCharActionSlot );
    REG_WND_FACTORY( KGCAgitCharSetting );
    REG_WND_FACTORY( KGCAgitCharItemSlot );
    REG_WND_FACTORY( KGCAgitCharEquipSlot );
    REG_WND_FACTORY( KGCCharIcon );
    REG_WND_FACTORY( KGCCharIconWnd );
    REG_WND_FACTORY( KGCAgitTrainingUI );
    REG_WND_FACTORY( KGCAgitGuestbook );
    REG_WND_FACTORY( KGCGuestbookSlot );
    REG_WND_FACTORY( KGCAgitMoneyInfo );
    REG_WND_FACTORY( KGCSphinxResultItemBox );
    REG_WND_FACTORY( KGCSphinxFace );
    REG_WND_FACTORY( KGCSphinxResultItemView );
    REG_WND_FACTORY( KGCSphinxQuiz );
    REG_WND_FACTORY( KGCSphinxChallenge );
    REG_WND_FACTORY( KGCSphinxInfoView );
    REG_WND_FACTORY( KGCSphinxDlg );
    REG_WND_FACTORY( KGCRewardItemListBox );
    REG_WND_FACTORY( KGCAgitMapSlot );
    REG_WND_FACTORY( KGCAgitMove );
    REG_WND_FACTORY( KGCAgitBrownie );
    REG_WND_FACTORY( KGCHarvestGauge );
    REG_WND_FACTORY( KGCSeedPlant );
    REG_WND_FACTORY( KGCSeedSlot );
    REG_WND_FACTORY( KGCAgitUserElement );
    REG_WND_FACTORY( KGCAgitListComboPopup );
    REG_WND_FACTORY( KGCAgitListComboMain );
    REG_WND_FACTORY( KGCAgitUserListCombo );
    REG_WND_FACTORY( KGCAgitList );
    REG_WND_FACTORY( KGCAgitSlot );
	REG_WND_FACTORY( KGCAgitEnterPass );
    REG_WND_FACTORY( KGCAgitInviteVisit );
    REG_WND_FACTORY( KGCAgitObjectSlot );
    REG_WND_FACTORY( KGCAgitObjectPlaceCollectUI );
    REG_WND_FACTORY( KGCAgitOption );
    REG_WND_FACTORY( KGCAgitPrePlace );
    REG_WND_FACTORY( KGCAgitShopItemBox );
    REG_WND_FACTORY( KGCAgitShop );
    REG_WND_FACTORY( KGCAgitInfoBar );
    REG_WND_FACTORY( KGCAgitOverlayUI );
    REG_WND_FACTORY( KGCChoiceDrop );
    REG_WND_FACTORY( KGCRickoonTornado );
    REG_WND_FACTORY( KGCMagicBoxItemWnd );
    REG_WND_FACTORY( KGCMagicBox );
    REG_WND_FACTORY( KGCCoordiShopScene );
    REG_WND_FACTORY( KGCCoordiCharView );
    REG_WND_FACTORY( KGCCoordiItemBox );
    REG_WND_FACTORY( KGCCoordiCatalogDlg );
    REG_WND_FACTORY( KGCCoordiShopScene );
    REG_WND_FACTORY( KGCCoordiCharView );
    REG_WND_FACTORY( KGCCoordiItemBox );
    REG_WND_FACTORY( KGCCoordiCatalogDlg );
	REG_WND_FACTORY( KGCAttributeSelectBox );
    REG_WND_FACTORY( KGCGuildItemBox );
    REG_WND_FACTORY( KGCGuildStore );
    REG_WND_FACTORY( KGCNicknameCampaignBox );
    REG_WND_FACTORY( KGCLevelupInfoBox );
    REG_WND_FACTORY( KGCPlantTreeRewardItemListView );
    REG_WND_FACTORY( KGCPlantTreeMap );
	REG_WND_FACTORY( KGCSocksTreeDlg );
    REG_WND_FACTORY( KGCPlantTree );
	REG_WND_FACTORY( KGCMyInfoSceneSubMenuFrame );
	REG_WND_FACTORY( KGCMyInfoSceneSubMenu );
	REG_WND_FACTORY( KGCGawibawiboItemView );
	REG_WND_FACTORY( KGCGawibawiboGameLose );
	REG_WND_FACTORY( KGCGawibawiboGamePlay );
	REG_WND_FACTORY( KGCGawibawiboGameBossInfo );
	REG_WND_FACTORY( KGCGawibawiboGameInfo );
	REG_WND_FACTORY( KGCGawibawiboGame );
    REG_WND_FACTORY( KGCReplayBoxNew );
    REG_WND_FACTORY( KGCReplayRecordREC );
    REG_WND_FACTORY( KGCReplayExplain );
    REG_WND_FACTORY( KGCRecordOptionBox );
    REG_WND_FACTORY( KGCReplayRecordAdvanceDlg );
    REG_WND_FACTORY( KGCReplayRecordCommonDlg );
    REG_WND_FACTORY( KGCReplayRecordDlg );
    REG_WND_FACTORY( KGCHeroItemPreview );
	REG_WND_FACTORY( KGCHotEventBtn );
    REG_WND_FACTORY( KGCSceneMenuBtn );
    REG_WND_FACTORY( KGCSellCountItemDlg );
    REG_WND_FACTORY( KGCSellInfo );
    REG_WND_FACTORY( KGCCharacterSelect );
    REG_WND_FACTORY( KGCCoordiChangeDlg );
    REG_WND_FACTORY( KGCUserItemWndInterface );
	REG_WND_FACTORY( KGCCharacterHoverBox );
	REG_WND_FACTORY( KGCCharacterLockBox );
	REG_WND_FACTORY( KGCCharacterSelectBox );
    REG_WND_FACTORY( KGCLookSavePanel );
    REG_WND_FACTORY( KD3DDialog );
    REG_WND_FACTORY( KGCParticleUI );
    REG_WND_FACTORY( KGCObjectUI );
    REG_WND_FACTORY( KGCDungeonMissionList );
    REG_WND_FACTORY( KGCBuyCashBox );
	REG_WND_FACTORY( KGCAgreement );
	REG_WND_FACTORY( KGCAgreement1301 );
	REG_WND_FACTORY( KGCPetCostumeWnd );
	REG_WND_FACTORY( KGCPetCostumeInven );
	REG_WND_FACTORY( KGCUINumber );
    REG_WND_FACTORY( KGCChatBox );
    REG_WND_FACTORY( KGCShopPanel );
    REG_WND_FACTORY( KGCInfoPanel );
    REG_WND_FACTORY( KGCShopTab );
    REG_WND_FACTORY( KGCItemBar );
    REG_WND_FACTORY( KGCQuestGauge );
    REG_WND_FACTORY( KGCRoomButton );
    REG_WND_FACTORY( KGCTeamBox );
    REG_WND_FACTORY( KGCChangeBox );
    REG_WND_FACTORY( KGCCharLoadWnd );
    REG_WND_FACTORY( KGCCharChangeBtn );
    REG_WND_FACTORY( KGCGuildInfo );
    REG_WND_FACTORY( KGCGuildMemo );
    REG_WND_FACTORY( KGCGraphicQuality );
    REG_WND_FACTORY( KGCInviteReplayBar );
    REG_WND_FACTORY( KGCMessageBox );
    REG_WND_FACTORY( KGCMainFrame );
    REG_WND_FACTORY( KGCGuildUserList );
    REG_WND_FACTORY( KGCCharWnd );
    REG_WND_FACTORY( KGCPetWnd );
    REG_WND_FACTORY( KGCRoomHover );
    REG_WND_FACTORY( KGCSetItemWnd );
    REG_WND_FACTORY( KGCOption );
    REG_WND_FACTORY( KGCPasswordBox );
    REG_WND_FACTORY( KGCGuildMemberBox );
    REG_WND_FACTORY( KGCGemBuyBox );
    REG_WND_FACTORY( KGCGameOverlayUI );
    REG_WND_FACTORY( KGCHeadupDisplay );
    REG_WND_FACTORY( KGCTutorialDlg );
    REG_WND_FACTORY( KGCArenaBattleDlg );
    REG_WND_FACTORY( KGCMyHeadup );
    REG_WND_FACTORY( KGCGameSkillBox );
    REG_WND_FACTORY( KGCPlayerStatusBar );
    REG_WND_FACTORY( KGCPlayerO2Bar );
    REG_WND_FACTORY( KGCUserItemBox );
    REG_WND_FACTORY( KGCShopItemBox );
    REG_WND_FACTORY( KGCUserEquipBox );
    REG_WND_FACTORY( KGCUserItemWnd );
    REG_WND_FACTORY( KGCShopItemWnd );
    REG_WND_FACTORY( KGCUserEquipWnd );
    REG_WND_FACTORY( KGCCashShopScene );
    REG_WND_FACTORY( KGCGPShopScene );
    REG_WND_FACTORY( KGCMyInfoScene );
    REG_WND_FACTORY( KGCGuildScene );
    REG_WND_FACTORY( KGCRoomSceneS6 );
    REG_WND_FACTORY( KGCLoginScene );
    REG_WND_FACTORY( KGCRegisterScene );
    REG_WND_FACTORY( KGCLoginDlg );
    REG_WND_FACTORY( KGCRegisterDlg );
    REG_WND_FACTORY( KGCLoginErrorDlg );
    REG_WND_FACTORY( KGCLoginEventDlg );
    REG_WND_FACTORY( KGCOnGameDlg );
    REG_WND_FACTORY( KGCSkillCheckBtn );
    REG_WND_FACTORY( KGCNickNameBox );
    REG_WND_FACTORY( KGCItemInfoBox );
    REG_WND_FACTORY( KGCLoadingScene );
    REG_WND_FACTORY( KGCCandidateWnd );
    REG_WND_FACTORY( KGCSpecialMark );
    REG_WND_FACTORY( KGCLoadingBox );
    REG_WND_FACTORY( KD3DSizingBox );
    REG_WND_FACTORY( KD3DBalloonBox );
    REG_WND_FACTORY( KGCSearchBar );
    REG_WND_FACTORY( KGCPetInfoBox );
    REG_WND_FACTORY( KGCPetMessage );
	REG_WND_FACTORY( KGCBuyCashItemWnd );
	REG_WND_FACTORY( KGCChangeCash );
    REG_WND_FACTORY( KGCItemImgWnd );
    REG_WND_FACTORY( KGCSetItemImgWnd );
    REG_WND_FACTORY( KGCColorDlg );
    REG_WND_FACTORY( KD3DColorButton );
    REG_WND_FACTORY( KGCIndigoSelect );
    REG_WND_FACTORY( KGCTagMatchSelect );
    REG_WND_FACTORY( KGCContestHelp );
    REG_WND_FACTORY( KGCOptionSound );
    REG_WND_FACTORY( KGCOptionMusic );
    REG_WND_FACTORY( KGCChatBtn );
    REG_WND_FACTORY( KGCToolTip );
    REG_WND_FACTORY( KGCAddFriendMenu );
    REG_WND_FACTORY( KGCPlayerSubMenu );
    REG_WND_FACTORY( KGCPetPreview );
    REG_WND_FACTORY( KGCPetMessage );
    REG_WND_FACTORY( KGCKeySettingWnd );
	REG_WND_FACTORY( KGCSkillKeySettingWnd );
    REG_WND_FACTORY( KGCEventBox );
    REG_WND_FACTORY( KGCInventoryCapacity );
    REG_WND_FACTORY( KGCRepetitionEventBox );
    REG_WND_FACTORY( KGCPetEvolutionBox );
    REG_WND_FACTORY( KGCPetCardSelect );
    REG_WND_FACTORY( KGCPetEvolutionWnd );
    REG_WND_FACTORY( KGCSelectPetNameBox );
    REG_WND_FACTORY( KGCRadioButtonBox );
    REG_WND_FACTORY( KGCAttendanceCalendar );
    REG_WND_FACTORY( KGCNumberControl );
    REG_WND_FACTORY( KGCCalendarDayControl );
    REG_WND_FACTORY( KGCPerfectAttendanceBox );
    REG_WND_FACTORY( KGCAttendanceItemWnd );
    REG_WND_FACTORY( GCOnGameExpBar);
    REG_WND_FACTORY( GCOnGameExpGauge );
    REG_WND_FACTORY( KGCNewTutorialOption );
    REG_WND_FACTORY( KGCLuckyDayReceiveBox );
    REG_WND_FACTORY( KGCReportBox );
    REG_WND_FACTORY( KGCSingleMenu );
    REG_WND_FACTORY( KGCMultiMenu );
    REG_WND_FACTORY( KGCComboEffect );
    REG_WND_FACTORY( KGCCharacterUI );   
    REG_WND_FACTORY( KGCSizingThreeBox );
    REG_WND_FACTORY( KGCMissionBtnInDlg );
    REG_WND_FACTORY( KGCQuestObtainGPEXP );
    REG_WND_FACTORY( KGCQuestRankScore );
    REG_WND_FACTORY( KGCQuestRankTotalScore );
    REG_WND_FACTORY( KGCServerScene );
    REG_WND_FACTORY( KGCWorldMapScene );
    REG_WND_FACTORY( KGCMatchScene );
    REG_WND_FACTORY( KGCServerBar );
    REG_WND_FACTORY( KGCServerList );
    REG_WND_FACTORY( KGCAlertWnd );
    REG_WND_FACTORY( KGCRoomList );
    REG_WND_FACTORY( KGCRoomBar );
    REG_WND_FACTORY( KGCUserBar );
    REG_WND_FACTORY( KGCChannelListBox );
    REG_WND_FACTORY( KGCChannelBar );
    REG_WND_FACTORY( KGCFadeoutWnd );
    REG_WND_FACTORY( KGCCategoryBox );
    REG_WND_FACTORY( KGCWorldMapContainer );
    REG_WND_FACTORY( KGCWorldMapPanel );
    REG_WND_FACTORY( KGCWorldMapIcon );
    REG_WND_FACTORY( KGCWorldMapPath );
    REG_WND_FACTORY( KGCSayBalloon );
    REG_WND_FACTORY( KGCSayBalloonSquare );    
    REG_WND_FACTORY( KGCSubMissionBar );
    REG_WND_FACTORY( KGCRoomFilterBox );
    REG_WND_FACTORY( KGCRoomFilterList );
    REG_WND_FACTORY( KGCQuestTileMap );
    REG_WND_FACTORY( KGCGradeMark );
    REG_WND_FACTORY( KD3DColorPalette );
    REG_WND_FACTORY( KGCGameResultUI );
    REG_WND_FACTORY( KGCMatchResultUI );
    REG_WND_FACTORY( KGCResultTower );
    REG_WND_FACTORY( KGCResultNick );
    REG_WND_FACTORY( KGCResultScore );
    REG_WND_FACTORY( KGCShortCutBox );
    REG_WND_FACTORY( KGCItemSlot );
    REG_WND_FACTORY( KGCEmoticonSlot );
    REG_WND_FACTORY( KGCUserCoreEquipWnd );
    REG_WND_FACTORY( KGCSocketWnd );
    REG_WND_FACTORY( KGCBonusChargeBox );
    REG_WND_FACTORY( KGCMatchSceneUserInfo );
    REG_WND_FACTORY( KGCMonsterCrusaderPanel );
    REG_WND_FACTORY( KGCMonsterHuntPanel );
    REG_WND_FACTORY( KGCIndigoLobby );
    REG_WND_FACTORY( KGCIndigoRanking );
    REG_WND_FACTORY( KGCIndigoRankingElement );
    REG_WND_FACTORY( KGCGuideNewbie );
    REG_WND_FACTORY( KGCHighLight );
    REG_WND_FACTORY( KGCSquareChannelSelect );
    REG_WND_FACTORY( KGCSquareChannelBar );
    REG_WND_FACTORY( KGCCheckBox );
    REG_WND_FACTORY( KGCSquareOverlayUI );
    REG_WND_FACTORY( KGCSquareScene );
    REG_WND_FACTORY( KGCChatScrollBackground );
    REG_WND_FACTORY( KGCChatBoxBackground );
    REG_WND_FACTORY( KGCCheckButton );
    REG_WND_FACTORY( KGCSquareLoadingScene );
    REG_WND_FACTORY( KGCEmoticonListBox );
    REG_WND_FACTORY( KD3DNumber );
    REG_WND_FACTORY( KGCSquareUserList );
    REG_WND_FACTORY( KGCSquareUserList_Element );
    REG_WND_FACTORY( KGCInviteRequestBox );
    REG_WND_FACTORY( KGCTreasureItemBox );
    REG_WND_FACTORY( KGCInviteSquareInfo );
    REG_WND_FACTORY( KGCInviteSquareList );
    REG_WND_FACTORY( KGCInviteUserInfo );
    REG_WND_FACTORY( KGCInviteUserList );
    REG_WND_FACTORY( KGCBarControl );
    REG_WND_FACTORY( KGCInviteMsgBox );
    REG_WND_FACTORY( KGCInviteInfo );
    REG_WND_FACTORY( KGCSquarePickUserList );
    REG_WND_FACTORY( KGCNeonsignSelectBox );
    REG_WND_FACTORY( KGCNeonsignItemDesc );
    REG_WND_FACTORY( KGCNeonsignItemList );
    REG_WND_FACTORY( KGCNeonsignMessage );
    REG_WND_FACTORY( KGCNeonsignPreview );
    REG_WND_FACTORY( KGCBossHPBar );
	REG_WND_FACTORY( KGCHPGauge );
    REG_WND_FACTORY( KGCEmoticonWindow );
    REG_WND_FACTORY( KGCEmoticonSlotRow );
    REG_WND_FACTORY( KGCEmoticonElement );
    REG_WND_FACTORY( KGCRankingDlg	);
    REG_WND_FACTORY( KGCRankingList );
    REG_WND_FACTORY( KGCRankingReward );
    REG_WND_FACTORY( KGCRankingElement );
    REG_WND_FACTORY( KGCConnectionMsg );
    REG_WND_FACTORY( KGCBadUserInfo );
    REG_WND_FACTORY( KGCWelcomeBack );
    REG_WND_FACTORY( KGCWelcomeBackRecom );
    REG_WND_FACTORY( KGCWelcomeBackReward );
    REG_WND_FACTORY( KGCWelcomeBackItemList );
    REG_WND_FACTORY( KGCAskUseVirtualCash );
    REG_WND_FACTORY( KGCTitleWindow );
    REG_WND_FACTORY( KGCTitleSlotRow );
    REG_WND_FACTORY( KGCPetWindow );
    REG_WND_FACTORY( KGCCTConditionWindow );    
	REG_WND_FACTORY( KGCDeathMatchScoreBoard );
    REG_WND_FACTORY( KGCDeathMatchCharInfoBar );
    REG_WND_FACTORY( KGCMinigameDlg );
    REG_WND_FACTORY( KGCMinigameCategory );
    REG_WND_FACTORY( KGCMinigameRanker );
    REG_WND_FACTORY( KGCMinigameResult );
    REG_WND_FACTORY( KGCMinigameScoreBoard );
    REG_WND_FACTORY( GCNateOn );
    REG_WND_FACTORY( GCBuddyTree );
    REG_WND_FACTORY( GCBuddyChild );
    REG_WND_FACTORY( GCNateOnAddGroupDlg );
    REG_WND_FACTORY( GCNateOnMultiMenu );
    REG_WND_FACTORY( GCNateOnGroupMultiMenu );
    REG_WND_FACTORY( GCEmoticonSelectDlg );
    REG_WND_FACTORY( GCNateOnAddFriend );
	REG_WND_FACTORY( GCNateOnChat );
	REG_WND_FACTORY( GCTalkList );
	REG_WND_FACTORY( GCBlockToAddFriend );
	REG_WND_FACTORY( KGCSkillTreeDlg );
	REG_WND_FACTORY( KGCSkillBoxSub );
	REG_WND_FACTORY( KGCSkillBoxMain );
	REG_WND_FACTORY( KGCSkilltreeField );
	REG_WND_FACTORY( KGCDescBox );
	REG_WND_FACTORY( KGCSkillBoxLock );
	REG_WND_FACTORY( GCNateOnInviteBuddy );
	REG_WND_FACTORY( GCInviteBuddyChild );
	REG_WND_FACTORY( GCInviteBuddyTree );
	REG_WND_FACTORY( GCNateOnMsg );
	REG_WND_FACTORY( KGCSkillBox );
	REG_WND_FACTORY( KGCSkillBoxGroup );
	REG_WND_FACTORY( KGCSkillSlotBox );
	REG_WND_FACTORY( KGCSkillSlot );
	REG_WND_FACTORY( KGCDummySkillBox );
	REG_WND_FACTORY( KD3DView );
	REG_WND_FACTORY( KGCCoupleViewer );
	REG_WND_FACTORY( KGCCoupleWnd );
	REG_WND_FACTORY( KGCCoupleInventory );
	REG_WND_FACTORY( KGCCoupleCharSelect );
	REG_WND_FACTORY( KGCCoupleItemBox );
	REG_WND_FACTORY( KGCCoupleAccept );
	REG_WND_FACTORY( KGCCouplePropose );
	REG_WND_FACTORY( KGCCoupleWait );
	REG_WND_FACTORY( KGCCoupleMessage );
	REG_WND_FACTORY( KGCCoupleGoodbye );
	REG_WND_FACTORY( KGCCoupleMark );
	REG_WND_FACTORY( KGCContributionDlg );
	REG_WND_FACTORY( KGCContributionBox );
	REG_WND_FACTORY( KGCHappyNewYearNPCDlg );
	REG_WND_FACTORY( KGCVipGachaBox );
	REG_WND_FACTORY( KGCVipGachaItemBox );
	REG_WND_FACTORY( KGCLogoutBox );
    REG_WND_FACTORY( KGCRecommendList );
    REG_WND_FACTORY( KGCCheckuser_ListElement );
    REG_WND_FACTORY( KGCCharDependentGauge ); 
	REG_WND_FACTORY( KGCGPCapsuleDlg );
	REG_WND_FACTORY( KGCGPCapsuleItemList );
	REG_WND_FACTORY( KGCGPCapsuleObtainWnd );
	REG_WND_FACTORY( KGCCouponNPCDlg );
	REG_WND_FACTORY( KGCCouponNPCEventWnd );
    REG_WND_FACTORY( GCSPGauge );    
	REG_WND_FACTORY( KGCSubscriptionDlg );
	REG_WND_FACTORY( KGCSubscriptionBox );
    REG_WND_FACTORY( KGCCooperationEventDlg );
	REG_WND_FACTORY( KGCDicePlay );
	REG_WND_FACTORY( KGCDicePlay_DiceView );
    REG_WND_FACTORY( KGCGuildMarkRegistDlg );
    REG_WND_FACTORY( KGCGuildDlg );
    REG_WND_FACTORY( KGCCommonEditBox );
    REG_WND_FACTORY( KGCGuildMatchRoomBar );    
	REG_WND_FACTORY( KGCGuildInfoDlg );
	REG_WND_FACTORY( KGCGuildMemberListDlg );
	REG_WND_FACTORY( KGCGuildManageDlg );
	REG_WND_FACTORY( KGCGuildMemberManageDlg );
	REG_WND_FACTORY( KGCGuildJoinWaitingUserInfo );
	REG_WND_FACTORY( KGCGuildJoinManageDlg );
    REG_WND_FACTORY( KGCGuildMemberInfo );  	
    REG_WND_FACTORY( KGCGuildEnterDlg );
    REG_WND_FACTORY( KGCGuildLobbyDlg );
	REG_WND_FACTORY( KGCGuildListDlg );
	REG_WND_FACTORY( KGCGuildListInfo );
	REG_WND_FACTORY( KGCGuildCreateDlg );
	REG_WND_FACTORY( KGCGuildSettingChangeDlg );
	REG_WND_FACTORY( KGCGuildJoinReqMbox );
	REG_WND_FACTORY( KGCGuildPublicMbox );
	REG_WND_FACTORY( KGCGuildSquareMenu );
	REG_WND_FACTORY( KGCGuildSquareNotice );
	REG_WND_FACTORY( KGCGuildLevelList );
    REG_WND_FACTORY( KGCGuildBenefitListBar );
    REG_WND_FACTORY( KGCFileFindDialog );
	REG_WND_FACTORY( KGCBingo );
    REG_WND_FACTORY( KGCQuiz );
    REG_WND_FACTORY( KGCItemInformationBox );
    REG_WND_FACTORY( KGCItemInformationBoxPair );
    REG_WND_FACTORY( KGCMonsterCardBook );
	REG_WND_FACTORY( KGCQuestResult_S3 );
    REG_WND_FACTORY( KGCScoreAnimUI );
    REG_WND_FACTORY( KGCSocketIcon );
    REG_WND_FACTORY( KGCItemSocketManagementDlg );
    REG_WND_FACTORY( KGCRealTimeRank );
    REG_WND_FACTORY( KGCBonusBoxSelect );
    REG_WND_FACTORY( KGCRadioButtonControl );
    REG_WND_FACTORY( KGCRecipeInfoBar );
	REG_WND_FACTORY( KGCMissionProgressBox );
    REG_WND_FACTORY( KGCInventoryFullMsg );
	REG_WND_FACTORY( KD3DButtonCombo );
	REG_WND_FACTORY( KGCPuzzleBox );
    REG_WND_FACTORY( KGCGiftItemBox );
	REG_WND_FACTORY( KGCProgressBtn );
	REG_WND_FACTORY( KGCHurryUPDlg );
    REG_WND_FACTORY( KGCStatusInfoPanel );
    REG_WND_FACTORY( KGCTutorialChar );
    REG_WND_FACTORY( KGCTutorialJob );
    REG_WND_FACTORY( KGCTutorialMap );
	REG_WND_FACTORY( KGCPingAntenna );
    REG_WND_FACTORY( KGCLoadingStateBar );
    REG_WND_FACTORY( KGCStageLoadCompleteSign );
	REG_WND_FACTORY( KGCItemInformationBreakupBox );
	REG_WND_FACTORY( KGCGambleBox );
    REG_WND_FACTORY( KGCCharGetInfoPromotion );
    REG_WND_FACTORY( KGCCharGetInfoPanel );
    REG_WND_FACTORY( KGCPromotionChangePanel );
    REG_WND_FACTORY( KGCCharacterGetInfoDlg );
    REG_WND_FACTORY( KGCHeroDungeonDlg );
    REG_WND_FACTORY( KGCHeroDungeonInfo );
    REG_WND_FACTORY( KGCHeroDungeonShop );
    REG_WND_FACTORY( KGCHeroDungeonInfoList );
    REG_WND_FACTORY( KGCHeroItemBox );
    REG_WND_FACTORY( KGCHeroDungeonOpentimeBox );
    REG_WND_FACTORY( KGCHeroDungeonLevelBox );
    REG_WND_FACTORY( KGCItemReceiveBoxNew );
    REG_WND_FACTORY( KGCBonusBoxSelect_S4_Row );
    REG_WND_FACTORY( KGCBonusBoxSelect_S4 );
    REG_WND_FACTORY( KGCMissionUI_S4 );
    REG_WND_FACTORY( KGCMissionMain_S4 );
    REG_WND_FACTORY( KGCMissionListBar_S4 );
    REG_WND_FACTORY( KGCMissionCharSelector_S4 );
    REG_WND_FACTORY( KGCMissionSub_S4 );
    REG_WND_FACTORY( KGCMissionSubInfoBox_S4 );
    REG_WND_FACTORY( KGCMissionReward_S4 );
    REG_WND_FACTORY( KGCMissionRewardBox_S4 );
    REG_WND_FACTORY( KGCMissionEquipPreview_S4 );
    REG_WND_FACTORY( KGCWeaponChangeBox );
    REG_WND_FACTORY( KGCUserMigrationDlg );
    REG_WND_FACTORY( KGCNewClearSealDlg );
    REG_WND_FACTORY( KGCNewClearSealJobBtn );
    REG_WND_FACTORY( KGCPackagePreview );
    REG_WND_FACTORY( KGCPackageItemBox );
    REG_WND_FACTORY( KGCDotaShop );
    REG_WND_FACTORY( KGCDPointItemBox );
    REG_WND_FACTORY( KGCCountryGroup );
    REG_WND_FACTORY( KGCDotaMiniMap );
    REG_WND_FACTORY( KGCDotaTileMap );
    REG_WND_FACTORY( KGCDotaPointOverLay );
    REG_WND_FACTORY( KGCDotaCoreHpBar );
    REG_WND_FACTORY( KGCDotaCount );
    REG_WND_FACTORY( KGCDotaItemImg );
    REG_WND_FACTORY( KGCDotaGuide );
	REG_WND_FACTORY( KGCPetItembox );
#if defined (PARTY_SYSTEM)
    REG_WND_FACTORY( KGCPartyWindow );
    REG_WND_FACTORY( KGCPartyMatch );
    REG_WND_FACTORY( KGCPartyDungeon );
    REG_WND_FACTORY( KGCPartyUserList );
    REG_WND_FACTORY( KGCPartyUserList_Element );
    REG_WND_FACTORY( KGCPartyInviteMsgBox );
#endif
    REG_WND_FACTORY( KGCEventMinigameList );
    REG_WND_FACTORY( KGCInviteAgitReqDlg );
    REG_WND_FACTORY( KGCInviteAgitSellBox );
    REG_WND_FACTORY( KGCEventMinigameList );

    REG_WND_FACTORY( KGCMantisReportDlg );
#if defined (GWC_EVENT_2011)
    REG_WND_FACTORY( KGCGwcEvent );
    REG_WND_FACTORY( KGCGwcCountry );
#endif
#if defined (USE_CHRISTMAS_GIFT_EVENT )
    REG_WND_FACTORY( KGCChristmasFriendList );
    REG_WND_FACTORY( KGCChristmasSendGiftDlg );
    REG_WND_FACTORY( KGCChristmasDonateDlg );
    REG_WND_FACTORY( KGCChristmasRewardBox );
    REG_WND_FACTORY( KGCChristmasGiftItemBox );
    REG_WND_FACTORY( KGCChristmasGiftDlg );
#endif
    REG_WND_FACTORY( KGCWelcomeBackNew );
    REG_WND_FACTORY( KGCCoordiClearSealDlg );
    REG_WND_FACTORY( KGCQPGauge ); 
#if defined (USE_TRANSFER_MISSION_SCROLL_BUYING_GUIDE )
    REG_WND_FACTORY( KGCTransferMissionScrollItemWnd );
    REG_WND_FACTORY( KGCTransferMissionScrollGuideGPCash );
    REG_WND_FACTORY( KGCTransferMissionScrollGuideCash );
    REG_WND_FACTORY( KGCTransferMissionScrollGuideDlg );
#endif
	REG_WND_FACTORY( KGCAddtionGuide );
	REG_WND_FACTORY( KGCAlertInDungeon );
    REG_WND_FACTORY( KGCLimitedTime );
    REG_WND_FACTORY( KGCFoodGauge );
    REG_WND_FACTORY( KGCRealMiniMap );    
	REG_WND_FACTORY( KGCNewsFrame );
#ifdef NEW_CHAR_SELECT_SCENE
    REG_WND_FACTORY(KGCCharSelectScene_New);
#else
    REG_WND_FACTORY(KGCCharSelectScene);
#endif
	REG_WND_FACTORY( KGCCharIntroFrame );
	REG_WND_FACTORY( KGCCharGainScene );
	REG_WND_FACTORY( KGCNameTag );
    REG_WND_FACTORY( KGCMacroCommunitySlot );
    REG_WND_FACTORY( KGCAdventureDialog );
    REG_WND_FACTORY( KGCAdventureReward );
    REG_WND_FACTORY( KGCAdventureRewardItemWnd );		
    REG_WND_FACTORY( KGCHeroDungeonUpgrade );
    REG_WND_FACTORY( KGCHeroItemUpgradeBox );
    REG_WND_FACTORY( KGCHeroItemUpgradeMaterial );
    REG_WND_FACTORY( KGCHeroItemUpgradeMaterialSelect );
    REG_WND_FACTORY( KGCAttendanceStampDlg );
    REG_WND_FACTORY( KGCAttendanceStampSlot );
    REG_WND_FACTORY( KGCBuffDonationDlg );
    REG_WND_FACTORY( KGCAddItemInClient );
    REG_WND_FACTORY( KGCAddItemInClientItemList );
    REG_WND_FACTORY( KGCAddItemInClientItemListElement );
    REG_WND_FACTORY( KGCBuyCashItemChooseCashTypeDlg );
    REG_WND_FACTORY( KGCBuyCashItemAccumulatedGCDlg );
    REG_WND_FACTORY( KGCBuyCashItemDlgS6 );
    REG_WND_FACTORY( KGCBuyCashItemBuyCompleteDlg ); 
    REG_WND_FACTORY( KGCChallengeErrandDlg );
    REG_WND_FACTORY( KGCChallengeErrandPanel );
    REG_WND_FACTORY( KGCChallengeErrandPlayer );
    REG_WND_FACTORY( KGCChallengeErrandModule );
    REG_WND_FACTORY( KGCChallengeErrandHelp );
    REG_WND_FACTORY( KGCChallengeErrandBuyList );
    REG_WND_FACTORY( KGCInGameMessage );
    REG_WND_FACTORY( KGCInGameMsgItemWnd );
    REG_WND_FACTORY( KGCSubjectList );
    REG_WND_FACTORY( KGCCommandGuid );
    REG_WND_FACTORY( KGCGuide );
    REG_WND_FACTORY( KGCGuideNPC );
    REG_WND_FACTORY( KGCGuideArrow );
	REG_WND_FACTORY( KGCItemImgUIWnd );
	REG_WND_FACTORY( KGCMonsterCardContents );
    REG_WND_FACTORY( KGCVitalityWorldmap );
    REG_WND_FACTORY( KGCAccountNickNameFrame );
    REG_WND_FACTORY( KGCVirtualDepotDlg );
    REG_WND_FACTORY( KGCVirtualDepotChar );
	REG_WND_FACTORY( KGCSocialCommerceDlg );
    REG_WND_FACTORY( KGCSocialCommerceRewardBox );
    REG_WND_FACTORY( KGCSavingGauge ); 
	REG_WND_FACTORY( KGCHeroDungeonSeason5 );
    REG_WND_FACTORY( KGCHeroDungeonSeason5Icon );
    REG_WND_FACTORY( KGCEventDungeonSeason5 );
    REG_WND_FACTORY( KGCEventDungeonSeason5Icon );
    REG_WND_FACTORY( KGCWorldMapTitle );
    REG_WND_FACTORY( KGCWorldMapUpperTitle );
    REG_WND_FACTORY( KGCBloodGauge );
    REG_WND_FACTORY( KGCShortMenuDlg );
    REG_WND_FACTORY( KGCShortMenuCharInfo );
    REG_WND_FACTORY( KGCShortMenuCharView );
    REG_WND_FACTORY( KGCBillBoardChatBoard );
    REG_WND_FACTORY( KGCBillboardChatDlg );
    REG_WND_FACTORY( KGCGachaBuyItemBox );
    REG_WND_FACTORY( KGCGachaBuyItemSlot );
    REG_WND_FACTORY( KGCQuantityPurchaseBox );
    REG_WND_FACTORY( KGCMaterialItemInfo );
    REG_WND_FACTORY( KGCHeroDungeonInfoS6 );
    REG_WND_FACTORY( KGCHeroDungeonInfoListS6 );
    REG_WND_FACTORY( KGCHeroDungeonOpentimeBoxS6 );
    REG_WND_FACTORY( KGCHeroDungeonShopS6Dlg );
    REG_WND_FACTORY( KGCHeroDungeonShopPanel );
    REG_WND_FACTORY( KGCHeroDungeonShopItemSlot );
    REG_WND_FACTORY( KGCHeroDungeonShopSidePanel );
    REG_WND_FACTORY( KGCHeroDungeonUpgradePanel );
    REG_WND_FACTORY( KGCHeroDungeonUpgradeSidePanel );
    REG_WND_FACTORY( KGCHeroDungeonUpgradeSlot );
    REG_WND_FACTORY( KGCHeroDungeonRewardDlg );
    REG_WND_FACTORY( KGCHeroDungeonRewardItemS6 );
    REG_WND_FACTORY( KGCRoomListFilterDlg );
    REG_WND_FACTORY( KGCRoomExtraOptionCharPanel );
    REG_WND_FACTORY( KGCRoomExtraOptionRankPanel );
    REG_WND_FACTORY( KGCRankIcon );
    REG_WND_FACTORY( KGCNicknameColorBox );
    REG_WND_FACTORY( KGCGachaPongDlg );
    REG_WND_FACTORY( KGCRoomOptionCharDlg );
    REG_WND_FACTORY( KGCRoomOptionRankDlg );
    REG_WND_FACTORY( KGCTowerLevelSelector );
    REG_WND_FACTORY( KGCRoomUserPotionS6 );
    REG_WND_FACTORY( KGCCoordiViewDlg );
    REG_WND_FACTORY( KGCCoordiViewItemWnd );
    REG_WND_FACTORY( KGCCoordiViewChar );
    REG_WND_FACTORY( KGCCustomizationDlg );
    REG_WND_FACTORY( KGCFontSkinDlg );
    REG_WND_FACTORY( KGCRoomAnimationDlg );
    REG_WND_FACTORY( KGCCutInSkinDlg );
    REG_WND_FACTORY( KGCQuantityUseBox );
    REG_WND_FACTORY( KGCZeroMaskDlg );
    REG_WND_FACTORY( KGCCalendarPerfectAttendanceControl );
    REG_WND_FACTORY( KGCPetMagicStoneDlg );
    REG_WND_FACTORY( KGCPetMagicStoneItemSlot );
    REG_WND_FACTORY( KGCHeroTicketSystem );
    REG_WND_FACTORY( KGCHeroTicketUseWnd );
    REG_WND_FACTORY( KGCCharCollection );
    REG_WND_FACTORY( KGCCharCollectionImage );
    REG_WND_FACTORY( KGCCharCollectionDlg );
    REG_WND_FACTORY( KGCVipCollection );
    REG_WND_FACTORY( KGCVipCollectionDlg );
    REG_WND_FACTORY( KGCChampionElement );
    REG_WND_FACTORY( KGCChampionPopup );
    REG_WND_FACTORY( KGCAllCollection );
}

void CMyD3DApplication::RenderUI( void )
{

    {
        m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0L );

        g_pGCDeviceManager2->PushState();
        {
            g_pGCDeviceManager2->SetRenderState(D3DRS_LIGHTING, FALSE);
            g_pGCDeviceManager2->SetRenderState(D3DRS_ZENABLE, FALSE);
            g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
            g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

            m_pkUIMgr->Render();
            g_MyD3D->m_pUIAnim->Render();

			if(SiKGCDragManager()->GetDragState() == KGCDragManager::KGDM_DRAG_PROGRESS){
				SiKGCDragManager()->Render();
			}

            //광장일 마우스 서서히 사라지게 하게 위해서, 255로 다시 채우는 부분 수정
            if (!(m_pStateMachine && m_pStateMachine->GetState() == GS_SQUARE))
            {
                MyCtrl->Mouse_Power = 255;
            }
            
            SiGCOlympicManager()->Render( );
            SiKGCAnnounceMgr()->Render();

        }
        g_pGCDeviceManager2->PopState();
		g_pGCDeviceManager2->Flush();
    }
}

HRESULT CMyD3DApplication::EnterChannel( DWORD dwChannelUID_ )
{
    Pause( TRUE );

    // 채널 입장을 요청한다.
    Result_Connect_GameServer = INT_MAX;
    KP2P::GetInstance()->Send_EnterChannelReq( L"Server", dwChannelUID_ );

    if( !WaitForServerAck( Result_Connect_GameServer, INT_MAX, 10000, TIME_OUT_VALUE ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL );
        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_GAMESERVER_CONNECT_TIMEOUT );
        KP2P::GetInstance()->Send_LeaveChannelNot( "Server" );
    }

    Pause( FALSE );

    if( Result_Connect_GameServer != 0 )
    {
        return E_FAIL;
    }

    // 다시 초기화 합시다~ ㅠ.ㅠ
    Result_Connect_GameServer = INT_MAX;

    return S_OK;
}

HRESULT CMyD3DApplication::LeaveChannel( void )
{
    KP2P::GetInstance()->Send_LeaveChannelNot( "Server" );
    return S_OK;
}

HRESULT CMyD3DApplication::EnterGuildRoom( void )
{
    Result_Connect_Gild = 0;
    KP2P::GetInstance()->Send_EnterGuildRoomReq( "Server" );
    WaitForServerAck( Result_Connect_Gild, 0, 3000, 3 );

    switch ( Result_Connect_Gild )
    {
    case 1:
        return S_OK;
    default:
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_GUILD1 ),
            g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_GUILD2 ) );
        break;
    }

    Result_Connect_Gild = 0;
    return E_FAIL;
}

HRESULT CMyD3DApplication::LeaveGuildRoom( void )
{
    KP2P::GetInstance()->Send_LeaveGuildRoomNot( "Server" );
    return S_OK;
}

void CMyD3DApplication::TagMatchHelpMessage()
{
    g_pkChatManager->AddChatMsg( 
        g_pkStrLoader->GetReplacedString( STR_ID_TAG_MATCH_HELP1, "l", 
        g_pkUIScene->m_pkOption->GetKeySettingWnd()->GetKeyStr( g_MyD3D->MyCtrl->m_keyUseItem ) ) 
        , KGCChatManager::CHAT_TYPE_ADMIN
        );
    g_pkChatManager->AddChatMsg( 
        g_pkStrLoader->GetReplacedString( STR_ID_TAG_MATCH_HELP2, "l", 
        g_pkUIScene->m_pkOption->GetKeySettingWnd()->GetKeyStr( g_MyD3D->MyCtrl->m_keyAttack ) )
        , KGCChatManager::CHAT_TYPE_ADMIN
        );
    //if( !g_pTagMatchSelect->IsReadyAvailable() )
    //{
    //    g_pkChatManager->AddChatMsg( L"- " + g_pkStrLoader->GetString( STR_ID_TAG_MATCH_READY_HELP ), 
    //        KGCChatManager::CHAT_TYPE_ADMIN );
    //}
}

void CMyD3DApplication::ExitWithInvalidFileAlert( void )
{
    StringCchCopyW( LastMessage1, MAX_PATH, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_FILE1 ).c_str() );
    StringCchCopyW( LastMessage2, MAX_PATH, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_FILE2 ).c_str() );
    PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

void CMyD3DApplication::ExitWithMsgDlg( const WCHAR* strMsgParam1_, const WCHAR* strMsgParam2_ )
{
    StringCchCopyW( LastMessage1, MAX_PATH, strMsgParam1_ );
    StringCchCopyW( LastMessage2, MAX_PATH, strMsgParam2_ );
    PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

void CMyD3DApplication::ExitWithMsgDlg( int iID1_, int iID2_ )
{
    ExitWithMsgDlg( g_pkStrLoader->GetString( iID1_ ).c_str(), g_pkStrLoader->GetString( iID2_ ).c_str() );
}

void ExitWithHackAlert( std::wstring strLog )
{
    //해킹 리플레이 보다가 튕기는 것 방지.
    if( g_MyD3D->MyReplay != NULL && g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
        return;

#if !defined(NO_GAMEGUARD) || defined(USE_XTRAP)
//    g_MyD3D->SendCurrentGameGuardInfo(GCUTIL_STR::GCStrWideToChar(strLog.c_str()));
#endif

    g_kGlobalValue.m_iCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_DETECT_HACK;

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];
    ::swprintf( szMsg1, MAX_PATH, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ALERT_HACK_TOOL1 ).c_str() );
    ::swprintf( szMsg2, MAX_PATH, g_kGlobalValue.m_strDetectedHackTool.c_str() );

    KMsgBox::Close();

    std::wstring strErrMsg;
    strErrMsg = szMsg1;
    strErrMsg += L"\n";
    strErrMsg += szMsg2;

    if( g_MyD3D->m_hWnd != NULL )
    {
        ::MessageBoxW( NULL, strErrMsg.c_str(), strLog.c_str(), MB_OK );
    }
    else
    {
        KMsgBox::Open( szMsg1, szMsg2, KMsgBox::MBOX_OK );
    }

	::PostQuitMessage( -1 );
    //exit(-1);// 그냥 종료 하는것이 옳다.
}

int CMyD3DApplication::AddMonster( int iMonID, float fRelativePosX, float fRelativePosY, bool bRight, bool bRandom,
                                  int iBoss, int iLevel /*= -1*/, DWORD dwChampionProperty /*= 0x00000000*/, bool bCheckSafePostion/* = true*/, bool bUseItemList /*= false*/, int iSlotIndex/* = -1*/ )
{
    D3DXVECTOR2 pos(0.0f, 0.0f);

    if ( bRandom )
    {
        pos = MyStg->Get_Random_XY();
    }
    else if( bCheckSafePostion )
    {
        pos = MyStg->Check_SafePostionX( SiKGCFantasticMap()->GetPixelWidth() * fRelativePosX, fRelativePosY );
    }

    if( (0.0f == pos.x && 0.0f == pos.y) || (0.0f == pos.x && -1.0f == pos.y) )
    {
        float fWidth = SiKGCFantasticMap()->GetPixelWidth();
        pos.x = fRelativePosX * fWidth;
        pos.y = fRelativePosY;
    }

    if( -1 == iSlotIndex )
    {
		for(MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			if ( mit->second->IsLive() == false ) {
				iSlotIndex = mit->first;
				break;
			}

		}

		if( -1 == iSlotIndex )
			m_iMonCursor = g_kMonsterManager.CreateMonster();
		else
			m_iMonCursor = iSlotIndex;
    }
    else
    {
		g_kMonsterManager.CreateMonster(iSlotIndex);
		m_iMonCursor = iSlotIndex;
    }

	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonCursor);

    if( false == pMonster->Wait_Room( iMonID, pos.x, pos.y, bRight, true, iLevel, dwChampionProperty, iBoss, bUseItemList ) )
    {
#if !defined(__PATH__)
        std::cout << "Add안되자나!!!버럭버럭!!!" <<std::endl;
#endif
    }
    pMonster->m_bLive = true;
    //g_kMonsterManager.m_mapMonster[m_iMonCursor]->Calculate_Point();

    return m_iMonCursor;
}

int CMyD3DApplication::AddMonster( std::string strMonID, int iMonType,int iLv, float fRelativePosX, float fRelativePosY, bool bRight,
                                  int iBoss, int iLevel /*= -1*/, DWORD dwChampionProperty /*= 0x00000000*/, bool bCheckSafePostion/* = true*/, bool bUseItemList /*= false*/ )
{
    D3DXVECTOR2 pos;
    MONSTER* pMon = g_kMonsterManager.GetMonster(m_iMonCursor);

	if ( pMon == NULL ) {
		m_iMonCursor = g_kMonsterManager.CreateMonster();
		pMon = g_kMonsterManager.GetMonster(m_iMonCursor);
	}

    if( bCheckSafePostion )
        pos = MyStg->Check_SafePostionX( SiKGCFantasticMap()->GetPixelWidth() * fRelativePosX, fRelativePosY );
    else
    {
        pos.x = fRelativePosX * SiKGCFantasticMap()->GetPixelWidth();
        pos.y = fRelativePosY;
    }

    pMon->Wait_Room( iMonType, pos.x, pos.y, bRight, true, iLevel, dwChampionProperty ,iBoss, bUseItemList );
    //pMon->m_strMonID = strMonID;
    //pMon->m_iLv = iLv;
    pMon->m_bLive = true;
    //pMon->UpateMonsterPoint();
    //pMon->m_bBossMonster = bBoss;

    return m_iMonCursor++;
}

int CMyD3DApplication::AddMonsterInQuest( std::string strMonID, int iMonType,int iLv, int iX, int iY, bool bRight, int iBoss, int iLevel /*= -1*/ )
{
    D3DXVECTOR2 pos;
	MONSTER* pMon = g_kMonsterManager.GetMonster(m_iMonCursor);

	if ( pMon == NULL ) {
		m_iMonCursor = g_kMonsterManager.CreateMonster();
		pMon = g_kMonsterManager.GetMonster(m_iMonCursor);
	}


    //pos = MyStg->Check_SafePostionX( MyStg->Num_Width * 1.5f * fRelativePosX, true, fRelativePosY );

    pos.x = (float)iX / 400.0f;
    pos.y = (float)iY / 400.0f;

    pMon->Wait_Room( iMonType, pos.x, pos.y, bRight, true, iLevel );
    //pMon->m_strMonID = strMonID;
    //pMon->m_iLv = iLv;
    pMon->m_bLive = true;
    //pMon->UpateMonsterPoint();
    //pMon->m_bBossMonster = bBoss;

    return m_iMonCursor++;
}

void CMyD3DApplication::ClearMonster( void )
{
    m_iMonCursor = 0;
}

void CMyD3DApplication::InitDataForTutorial()
{
    IsDetermineWin=false;
    Story = -1;
    g_kCamera.SetEarthQuake(0);// 지진 안일어 난다, FAST Match를 끈다.
    //SetSubGameMode( GC_SGM_NORMAL );
    SiKGCRoomManager()->SetSubGameMode( GC_SGM_NORMAL );

    g_pkUIScene->m_pkNewTutorialOption->LoadTutorial();

    g_kCamera.InitCamera(); // 카메라 초기화.
    g_kCamera.SetTargetPlayer(0);
    g_kCamera.InitCameraPosition();

    g_pkGameOverlayUI->OnStartGame();

    // [8/5/2007 breadceo] SOund
    g_KDSound.FullLoadfromLoadList();
}

bool CMyD3DApplication::WaitForServerAck( int& ioutVariable, int iInitValue, DWORD dwTimeOut, int iTimeOutReturnValue )
{
    MSG msg;
    DWORD dwStartTime = ::timeGetTime();

    //dwTimeOut = 10000;
    //여기에서 초기화 해주는 않는것은 타이밍상의 문제임
    //ioutVariable = iInitValue;


    //테스트용 코드입니다.
    bool bSendErrorOnce = false;

    while ( ioutVariable == iInitValue )
    {
        if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            ::DispatchMessage( &msg );
            if ( ioutVariable != iInitValue )
                return true;
        }
        if( dwTimeOut != 0 )
        {
            //테스트용 코드입니다.
            if( timeGetTime() - dwStartTime > 21000 && !bSendErrorOnce )
            {
                bSendErrorOnce = true;
                SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_CREATE_ROOM_FAIL );
            }

            if ( timeGetTime() - dwStartTime >= dwTimeOut )
            {
                ioutVariable = iTimeOutReturnValue;
                return false;
            }
        }
        // Server Thread를 아사시킬 가능성이 있어서 Sleep을 넣어 준다.
        Sleep( 0 );
    }
    return true;
}

void CMyD3DApplication::GetStatPromotionCount( std::vector< std::pair<char,char> >& vecPromotionCount_ )
{
    vecPromotionCount_.clear();
    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i ) // 모든 플레이어의 
    {
        if( !MyPlayer[i]->m_kUserInfo.bLive ) continue;

        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
        {
            for ( int j = 0; j < 3; j++ )
            {
                char cCharType = (char)g_MyD3D->MyPlayer[i]->m_kUserInfo.aiTagSlot[j];
                if( cCharType < 0 ) continue;

                char cCharPromotion = (char)MyPlayer[i]->GetPromotion( cCharType );
                vecPromotionCount_.push_back( std::make_pair(cCharType,cCharPromotion) );
            }
        }
        else
        {
            char cCharType = (char)MyPlayer[i]->GetCurrentChar().iCharType;
            char cCharPromotion = (char)MyPlayer[i]->GetPromotion( cCharType );
            vecPromotionCount_.push_back( std::make_pair(cCharType,cCharPromotion) );
        }
    }
}

void CMyD3DApplication::CreateMeshAnimationForPrevLoad( int iMonsterType )
{
    if( g_kMonsterManager.m_vecMonsterTable.size() <= (UINT)iMonsterType )
    {
#if defined( ERROR_MESSAGE_BOX )
        {
            std::wstringstream strText;
            std::wstringstream strCaption;
            strText << L"( " << iMonsterType << L" ) 잘못된 몬스터 타입입니다.";
            strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
            MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
        }
#endif

        return;
    }

    // 몬스터 템플릿의 정보를 가져옴
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ iMonsterType ];

    KGC3DObject* p3DObject = new KGC3DObject();
    p3DObject->Init(kMonsterTemplate.iTechnique);

    std::stringstream stm1, stm2;
    for( int i = 0; i < (int)kMonsterTemplate.vecMeshInfo.size(); ++i )
    {
        p3DObject->AddMesh( kMonsterTemplate.vecMeshInfo[i].strMeshName, kMonsterTemplate.vecMeshInfo[i].strTextureName, kMonsterTemplate.vecMeshInfo[i].strID, kMonsterTemplate.vecMeshInfo[i].dwTechnique );
    }

    // 모션 로드
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mapActionIter = mapAction.begin();
    for(; mapActionIter != mapAction.end(); ++mapActionIter)
    {
        KGCMonsterAction &kMonsterAction = mapActionIter->second;
        p3DObject->AddMotion( kMonsterAction.m_strMotion );
    }

    p3DObject->SetShellMatrixScale( 0.5f );

    SiKGC3DObjectMgr()->Add3DObjectForPrevLoad( p3DObject );

    // 사운드 로드
    for( int i = 0 ; i < (int)kMonsterTemplate.m_vecSounds.size() ; ++i )
    {
        g_KDSound.PreparetoLoad( kMonsterTemplate.m_vecSounds[i] );
        g_KDSound.LoadFromIndex( kMonsterTemplate.m_vecSounds[i] );
    }
}

int CMyD3DApplication::GetActiveStage()
{
    return g_kEveryTriggerManager.GetActiveStage();
}

void CMyD3DApplication::SendExitPacket()
{
    KP2P::GetInstance()->Send_ClientCrashedNot();
    KP2P::GetInstance()->DisConnectTCPRelay();

}

bool CMyD3DApplication::EnableKeyInput()
{
    if( m_bEnableKeyInput )
        return true;
    else
        return false;
}


/*[NewCrashInfo] : 20110104
	새로 크래시 데이터를 파싱하기 쉽게 만들려고 작성
========================================================================*/

class NewCrashInfo
{
public:
	void			PushData( const std::string& strKey_, const std::string& strData_ );
	std::string&	GetInfoString();

private:
	std::map< std::string, std::vector<std::string> >	m_mapStrCrashInfo;
	std::vector< std::string >							m_vecKeyOrder;
};

void NewCrashInfo::PushData( const std::string& strKey_, const std::string& strData_ )
{
	std::map< std::string, std::vector<std::string> >::const_iterator itrFind = m_mapStrCrashInfo.find(strKey_);
	if( m_mapStrCrashInfo.end() == itrFind ){
		m_vecKeyOrder.push_back( strKey_ );
	}
	m_mapStrCrashInfo[strKey_].push_back( strData_ );	
}

std::string& NewCrashInfo::GetInfoString()
{
	static std::string	infoString;
	infoString.clear();

	for( size_t i=0; i<m_vecKeyOrder.size(); ++i ){
		std::vector<std::string>&	vecString = m_mapStrCrashInfo[m_vecKeyOrder[i]];

		infoString += std::string("[");
		infoString += m_vecKeyOrder[i];
		infoString += std::string("]\n");

		for( size_t i=0; i<vecString.size(); ++i ){
			infoString += vecString[i];
			infoString += std::string("\n");
		}
	}

	return infoString;
}


#if defined( BUG_TRAP )

void CALLBACK CrashCallback(INT_PTR nErrHandlerParam)
{
    ::MessageBoxA(NULL, "BugTrp crash", "", MB_OK);
#if !defined( __PATH__ ) 
    ::MessageBoxA(NULL, "BugTrap Crash", "", MB_OK);
#endif

    g_kGlobalValue.RunLogProgram("18");

    FILE* file = fopen("crashlog.txt", "w");
    std::string crashLog;
    char temp[256] = { 0, };

    std::string strServerName(KncUtil::toNarrowString(g_kGlobalValue.ServerInfo.strServerName));
    std::string strUserNickName(KncUtil::toNarrowString(g_kGlobalValue.m_kUserInfo.strNickName));
    std::string strUserLogin(KncUtil::toNarrowString(g_kGlobalValue.m_kUserInfo.strLogin));

    char name[100] = "";
    char ip[100] = "";

    std::stringstream stmHeader;
    NewCrashInfo	newCrashInfo;

    try
    {
        if (KP2P::GetInstance()->IsConnectedToGameServer())
        {
            in_addr kAddInfo;
            kAddInfo.s_addr = g_kGlobalValue.ServerInfo.dwPublicIP;
            strcpy(ip, inet_ntoa(kAddInfo));

            stmHeader << "----------------------------------------------\n              Crash Header                    \n----------------------------------------------\n";
            stmHeader << "[" << GCUTIL_STR::GCStrWideToChar(NATION_CODE) << "]"
                << " " << strServerName
                << " : " << g_kGlobalValue.GetGameVersionA()
                << "\nNickName : " << strUserNickName
                << "\nLogin : " << strUserLogin
                << "\nUID : " << g_kGlobalValue.m_kUserInfo.dwUID
                << "\nIP : " << ip
                << "\nGlobalUserInfo_CurCharType : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType
                << "\nGlobalUserInfo_CurCharExp : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().biExp
                << "\nGlobalUserInfo_CurCharLevel : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel
                << "\nGlobalUserInfo_CurChar_PetName : " << GCUTIL_STR::GCStrWideToChar(g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_strName.c_str())
                << "\nGlobalUserInfo_CurChar_PetID : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwID
                << "\nGlobalUserInfo_CurChar_PetUID : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwUID
                << "\nGlobalUserInfo_CurChar_PetEXP : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwEXP
                << "\nTempPlayerUserInfo_CurCharType : " << g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType
                << "\nTempPlayerUserInfo_CurCharExp : " << g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().biExp
                << "\nTempPlayerUserInfo_CurCharLevel : " << g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iLevel
                << "\nTempPlayerUserInfo_CurChar_PetName : " << GCUTIL_STR::GCStrWideToChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_strName.c_str())
                << "\nTempPlayerUserInfo_CurChar_PetID : " << g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwID
                << "\nTempPlayerUserInfo_CurChar_PetUID : " << g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwUID
                << "\nTempPlayerUserInfo_CurChar_PetEXP : " << g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwEXP
                << "\n----------------------------------------------\n";

            crashLog += stmHeader.str().c_str();

            newCrashInfo.PushData(std::string("UserID"), strUserLogin);
            newCrashInfo.PushData(std::string("LoginUID"), boost::lexical_cast<std::string>(g_kGlobalValue.m_kUserInfo.dwUID));
            newCrashInfo.PushData(std::string("CharType"), boost::lexical_cast<std::string>(static_cast<int>(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType)));
        }
        else
        {
            PHOSTENT hostinfo;
            gethostname(name, sizeof(name));
            hostinfo = gethostbyname(name);
            strcpy(ip, inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list));


            stmHeader << "----------------------------------------------\n              Crash Header                    \n----------------------------------------------\n";
            stmHeader << "[" << GCUTIL_STR::GCStrWideToChar(NATION_CODE) << "]"
                << " " << strServerName
                << " : " << g_kGlobalValue.GetGameVersionA()
                << "\nNickName : " << strUserNickName
                << "\nLogin : " << strUserLogin
                << "\nUID : " << g_kGlobalValue.m_kUserInfo.dwUID
                << "\nIP : " << ip
                << "\n----------------------------------------------\n";

            crashLog += stmHeader.str().c_str();
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١��ɸ��� ���� ����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }
    try {

        sprintf(temp, "CrashMsg : %s\n", g_kGlobalValue.m_strCrashMsg.c_str());
        crashLog += temp;

        for (int i = 0; i < (int)g_kGlobalValue.m_vecProtectedTypeFail.size(); i++)
        {
            sprintf(temp, "ProtectedTypeFailVal : %s\n", g_kGlobalValue.m_vecProtectedTypeFail[i].c_str());
            crashLog += temp;
        }



        if (g_MyD3D == NULL)
        {
            crashLog += "g_MyD3D��ü�� ����";
        }
        if (g_pGraphicsHelper == NULL)
        {
            crashLog += "GraphicsHelper��ü�� ����";
        }
        if (g_pd3dDevice == NULL)
        {
            crashLog += "Device��ü ����";
        }
    }
    catch (...)
    {

    }
    try
    {
        // ���� ���� ��带 �����
        sprintf(temp, "Story : %d\n", g_MyD3D->Story);
        crashLog += temp;
        sprintf(temp, "GameState : %s\n", g_MyD3D->m_pStateMachine->GetStateString().c_str());
        newCrashInfo.PushData(std::string("GameState"), g_MyD3D->m_pStateMachine->GetStateString());
        crashLog += temp;
        sprintf(temp, "GameModeCategory : %s\n", (int)SiKGCRoomManager()->GetGameModeCategoryStr().c_str());
        crashLog += temp;
        sprintf(temp, "GameMode : %s\n", (int)SiKGCRoomManager()->GetGameModeStr().c_str());
        newCrashInfo.PushData(std::string("GameMode"), SiKGCRoomManager()->GetGameModeStr());
        crashLog += temp;
        sprintf(temp, "SubGameMode : %s\n", (int)SiKGCRoomManager()->GetSubGameModeStr().c_str());
        crashLog += temp;
        sprintf(temp, "ReplayState : %d\n", g_MyD3D->MyReplay->m_iCurrState);
        crashLog += temp;

        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        {
            // Jaeho.Ready
            //sprintf( temp, "(QuestState : %s)\n", g_MyD3D->m_pkQuestGameMgr->GetQuestProgressStateStr().c_str() );
            //crashLog += temp;
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١����� ���Ӹ�� ����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    try
    {
        if (g_kGlobalValue.m_strNullFunc != "")
        {
            crashLog += "CharInfo�� NULL�� �� �Լ� : ";
            crashLog += g_kGlobalValue.m_strNullFunc;
            crashLog += "\n";
        }

        // ���� �ڻ� �α�
        if (g_kGlobalValue.m_bMonsterSuicide == true)
        {
            crashLog += "�١١١١١١ٸ��� �ڻ��� �� �����١١١١١١�\n";
        }

        if (g_kGlobalValue.m_bSkillMotionIDErrorOccur)
        {
            crashLog += "SkillMotionID ������ ���� ����\n";
        }

        sprintf(temp, "ChannelName : %s\n", GCUTIL_STR::GCStrWideToChar(g_Selected_Channel_Name));
        crashLog += temp;

        if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME)
        {
            sprintf(temp, "Room(%d) : %s\n", (int)SiKGCRoomManager()->GetRoomInfo().usRoomID, GCUTIL_STR::GCStrWideToChar(SiKGCRoomManager()->GetRoomInfo().strRoomName.c_str()));
            crashLog += temp;

            for (int i = 0; i < MAX_PLAYER_NUM; i++)
            {
                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
                    continue;
                /************************************************************************/
                /* IsLive �� true �϶��� �α׿� ���
                /************************************************************************/
                int iFrame = (int)g_MyD3D->MyPlayer[i]->cFrame;
                sprintf(temp, "%sPlayer%d : CharNum = %s( %s ), NowMotion = %3d, Frame = %3d\n",
                    g_MyD3D->MyPlayer[i]->IsLocalPlayer() == true ? "�� M e ��" : "��Others��", i,
                    g_MyD3D->MyPlayer[i]->GetCharString().c_str(), g_MyD3D->MyPlayer[i]->GetFormString().c_str(),
                    (int)g_MyD3D->MyPlayer[i]->uiMotion, iFrame);

                crashLog += temp;

                if (g_pGraphicsHelper->GetPlayerActionMotion(g_MyD3D->MyPlayer[i]->uiMotion) == NULL)
                {
                    crashLog += "�١١١١١١١١١١١١١١١�Motion�����Ͱ� ��������. ����Ŀ���� ���;� �ϴµ�...�١١١١١١١١١١١١١١١�\n\n";
                }
                else
                {
                    if (g_MyD3D->MyPlayer[i]->VerifyMotion(g_MyD3D->MyPlayer[i]->uiMotion) == true)
                    {
                        if (g_MyD3D->MyPlayer[i]->cFrame >= (int)g_pGraphicsHelper->GetPlayerActionMotion(g_MyD3D->MyPlayer[i]->uiMotion)->GetNum_Frame())
                        {
                            crashLog += "�١١١١١١١١١١١١١١١ٸ���� Total���� ū Frame�١١١١١١١١١١١١١١١�\n\n";
                        }
                        else
                        {
                            crashLog += "��� ������ OK\n";
                        }
                    }
                    else
                    {
                        // 2007/5/18. daeuk. �̷��� ���� ���α׷��� �ֱ淡 ���� ���� ���. �������
                        crashLog += "�ٸ� ĳ���� ����� ���ϰ� �ִ�. ��ŷ�ǽ�\n\n";
                    }
                }
            }
        }
        else if (g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE_LOADING ||
            g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
        {
            std::stringstream stm;
            stm << "==================================================================================\n";
            stm << "Login ID     : " << GCUTIL_STR::GCStrWideToChar(g_kGlobalValue.m_kUserInfo.strLogin.c_str()) << "\n";
            stm << "UserNickName : " << GCUTIL_STR::GCStrWideToChar(g_kGlobalValue.m_kUserInfo.strNickName.c_str()) << "\n";
            stm << "Current Char : " << g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType << "\n";
            stm << "==================================================================================\n";

            KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;
            for (int i = 0; i < (int)vecChar.size(); ++i)
            {
                SCharInfo& Char = vecChar[i];
                stm << "CharType : " << Char.iCharType << "\n";
                for (std::set< char >::iterator sit = Char.setPromotion.begin(); sit != Char.setPromotion.end(); ++sit) {
                    stm << "Promotion : " << *sit << "  ";
                }
                stm << "\n" << "Level : " << Char.iLevel << "\n";
                if (Char.kPetInfo.m_dwUID != 0)
                    stm << "Pet : " << Char.kPetInfo.m_dwID << "(" << Char.kPetInfo.m_kCostume.m_dwUID << ")\n";

                for (int j = 0; j < (int)Char.vecItemInfo.size(); ++j)
                {
                    SUserItem& item = Char.vecItemInfo[j];
                    stm << "Item[" << j << "] : " << item.iItemID << "\t" << item.dwItemUID << "\n";
                    newCrashInfo.PushData(std::string("ItemList"), boost::lexical_cast<std::string>(item.dwItemUID));
                }
            }
        }
        else
        {
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١�Player�� Crash����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    try
    {
        // �±����� ���ܵ�
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
        {

            for (int i = 0; i < MAX_PLAYER_NUM; i++)
            {
                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
                    continue;

                crashLog += g_MyD3D->MyPlayer[i]->toTagStr();
                crashLog += g_MyD3D->m_akPet[i]->toTagStr();
            }
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١�Tag�� Crash����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    // �� ���� ���ܺ��߰���..
    try
    {
        // ������ ��
        if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME ||
            g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING ||
            g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
        {
            for (int i = 0; i < MAX_PLAYER_NUM; ++i)
            {
                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
                {
                    sprintf(temp, "[Pet%d] : ", i);
                    crashLog += temp;
                    crashLog += g_MyD3D->m_akPet[i]->toStr();
                }
            }
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١١�Pet�� Crash����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    try
    {
        // vecItemInfo �����ؼ� ���� �״� ����� �α� ���ܺ��ϴ�.
        if (g_MyD3D->m_pStateMachine->IsStateMyInfo()
            || g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE_LOADING || g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE)
        {
            if (g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex != -1)
            {
                // 2007/2/15. iridology. ���� �ɸ��� ����, �α�, Uid �����..
                sprintf(temp, "[Player] LoginId : %s Uid : %u\n", GCUTIL_STR::GCStrWideToChar(g_MyD3D->m_TempPlayer.GetPlayerName()), g_MyD3D->m_TempPlayer.m_kUserInfo.dwUID);
                crashLog += temp;

                sprintf(temp, "ItemID/Pattern(JLv)[Uid] : ");
                crashLog += temp;

                int iCnt = 0;
                for (int j = 0; j < (int)g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo.size(); ++j)
                {
                    iCnt++;
                    // 2007/2/15. iridology. ���� ItemId�� mapDB�� begin�� �������� �Ǿ�����
                    GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData(g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo[j].iItemID);
                    DWORD dwCharType = kItem->dwCharType;
                    if (!(g_MyD3D->m_TempPlayer.GetCharTypeDword(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) & dwCharType))
                    {
                        sprintf(temp, "��");
                        crashLog += temp;
                    }

                    sprintf(temp, "%d(%d)[%u], ", (int)g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo[j].iItemID * 10,
                        (int)kItem->iNeedChangeJobLevelForEquippingItem,
                        (int)g_MyD3D->m_TempPlayer.GetCurrentChar().vecItemInfo[j].dwItemUID);

                    crashLog += temp;

                    if (iCnt == 4)
                    {
                        iCnt = 0;
                        sprintf(temp, "\n");
                        crashLog += temp;
                    }
                }

                sprintf(temp, "\n\n");
                crashLog += temp;
            }
        }
        else
        {
            for (int i = 0; i < MAX_PLAYER_NUM; ++i)
            {
                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
                    continue;

                if (g_MyD3D->MyPlayer[i]->m_kUserInfo.cCharIndex != -1)
                {
                    // 2007/2/15. iridology. ���� �ɸ��� ����, �α�, Uid �����..
                    sprintf(temp, "[Player%d] LoginId : %s Uid : %u\n", i, GCUTIL_STR::GCStrWideToChar(g_MyD3D->MyPlayer[i]->GetPlayerName()), g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID);
                    crashLog += temp;

                    sprintf(temp, "[ItemInfo]\n");
                    crashLog += temp;

                    int iCnt = 0;
                    for (int j = 0; j < (int)g_MyD3D->MyPlayer[i]->GetCurrentChar().vecItemInfo.size(); ++j)
                    {
                        iCnt++;
                        // 2007/2/15. iridology. ���� ItemId�� mapDB�� begin�� �������� �Ǿ�����
                        GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData(g_MyD3D->MyPlayer[i]->GetCurrentChar().vecItemInfo[j].iItemID);
                        CONTINUE_NIL(kItem);
                        if (!(g_MyD3D->MyPlayer[i]->GetCharTypeDword(g_MyD3D->MyPlayer[i]->GetCurrentChar().iCharType) & kItem->dwCharType))
                        {
                            sprintf(temp, "��");
                            crashLog += temp;
                        }

                        sprintf(temp, "%d, ", (int)g_MyD3D->MyPlayer[i]->GetCurrentChar().vecItemInfo[j].iItemID * 10);

                        crashLog += temp;

                        if (iCnt == 4)
                        {
                            iCnt = 0;
                            sprintf(temp, "\n");
                            crashLog += temp;
                        }
                    }

                    sprintf(temp, "\n");
                    crashLog += temp;
                }
            }
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١�Player ����� Crash����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    try
    {
        // ���� ������ �����
        for (MAP_INT_PMONSTER::iterator vit = g_kMonsterManager.m_mapMonster.begin(); vit != g_kMonsterManager.m_mapMonster.end(); ++vit) {
            MONSTER* pMonster = vit->second;
            if (pMonster->m_bLive == true)
            {
                sprintf(temp, "Index : %d, Name : %s, Motion : %3d / MotionTotal = %d, Frame : %3d\n", vit->first,
                    GCUTIL_STR::GCStrWideToChar(pMonster->GetMonsterName()), pMonster->m_aiMotion[LATENCY_INDEX],
                    (int)pMonster->m_vecMotionList.size(), (int)pMonster->m_aiFrame[LATENCY_INDEX]);
                crashLog += temp;

                if (pMonster->m_aiFrame[LATENCY_INDEX] >= pMonster->m_vecMotionList[pMonster->m_aiMotion[LATENCY_INDEX]]->GetNum_Frame())
                {
                    crashLog += "Frame�� Total Frame�� �Ѿ��\n";
                    crashLog += pMonster->m_vecMotionList[pMonster->m_aiMotion[LATENCY_INDEX]]->GetDeviceID();
                }
            }

            // ���� �� ���� �ʾƼ� �α׸� ���ϴ�.
            if (g_kGlobalValue.m_errMonsterMotion.bErrorOccured == true)
            {
                crashLog += "Monster::GetRenderPosInOldGC() Error ���� ����\n";
                sprintf(temp, "������ Motion = %d, Frame = %d\n", g_kGlobalValue.m_errMonsterMotion.iMotion, g_kGlobalValue.m_errMonsterMotion.iFrame);
            }
            if (g_kGlobalValue.m_errFrmLoad.bErrorOccured == true)
            {
                crashLog += "Monster::StartMonster() Error ���� ����\n";
                crashLog += "������ strMotion : ";
                crashLog += g_kGlobalValue.m_errFrmLoad.strMotion;
                crashLog += " strWaitMotion";
                crashLog += g_kGlobalValue.m_errFrmLoad.strWaitMotion;
            }

        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١ٸ��� Crash ����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    try
    {
        crashLog += g_MyD3D->GetMyShortCutSlot()->Dump().c_str();
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١ٴ��� ���� �α׳���� ���� �߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

#if defined(DLL_INJECTION_CHECK)
    try
    {
        if (g_MyD3D->m_bCanUseDllchecker)
            crashLog += "==============================DllChecker Made ================================\n";
        else
            crashLog += "=========================Failed to make DllChecker============================\n";

        if (g_kGlobalValue.m_bFindMainFile)
            crashLog += "==============================Find Mainfile ==================================\n";
        else
        {
            crashLog += "======================== Can't find Mainfile =================================\n";
            crashLog += g_kGlobalValue.m_strGCProcessName;
            crashLog += "\n";
        }


        if (KDLLChecker::GetInstance()->GetbSnapCheck())
            crashLog += "==============================Checked Dll=====================================\n";
        std::set<std::string> setList = KDLLChecker::GetInstance()->GetNoList();
        if (!setList.empty())
        {
            crashLog += "==============================================================================\n";
            crashLog += "===========================It's not DLL in List===============================\n";
            crashLog += "==============================================================================\n";

            std::set<std::string>::iterator setIter;
            for (setIter = setList.begin(); setIter != setList.end(); ++setIter)
            {
                crashLog += *setIter;
                crashLog += "\n";
            }

            crashLog += "==============================================================================\n";
            crashLog += "==============================================================================\n";

            if (KDLLChecker::GetInstance()->GetbIsNT())
            {
                crashLog += "======================Windows NT==============================================\n";
                if (KDLLChecker::GetInstance()->HaveHiddenDll())
                {
                    crashLog += "======================Hidden DLL==============================================\n";
                    std::set<std::string> setNoListBypsapi = KDLLChecker::GetInstance()->GetNoListByPsapi();
                    std::set<std::string>::iterator setpsapiIter;
                    for (setpsapiIter = setNoListBypsapi.begin(); setpsapiIter != setNoListBypsapi.end(); ++setpsapiIter)
                    {
                        crashLog += *setpsapiIter;
                        crashLog += "\n";
                    }
                }

            }
            else
            {
                crashLog += "======================NO NT==============================================\n";
            }
            if (g_kGlobalValue.m_bHackCheck)
            {
                crashLog += "==========================Checked Dll=========================================\n";
                crashLog += g_kGlobalValue.m_strCheckingBlackDll;
                crashLog += "\n";
            }
            else
                crashLog += "==========================Didn't Check Dll===================================\n";

            sprintf(temp, "InitTime : %d \n", g_kGlobalValue.m_ulGanadara);

            crashLog += temp;

            sprintf(temp, "LeftTime : %d \n", g_kGlobalValue.m_ulRandomCount);

            crashLog += temp;


        }

        if (g_kGlobalValue.m_bDamageHack)
        {
            crashLog += "======================DamageHack Checked==============================================\n";

            sprintf(temp, "Before : %f \n", g_kGlobalValue.m_fBefore);

            crashLog += temp;

            sprintf(temp, "After : %f \n", g_kGlobalValue.m_fAter);

            crashLog += temp;
        }


    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١�DLL ���� �α׳���� ���� �߻�!!!!�١١١١١١١١١١١١١١١�\n";

    }
#endif


    try
    {
        crashLog += g_MyD3D->m_pStateMachine->Dump().c_str();
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١����� ���� �α׳���� ���� �߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    /*[ �������� ���μ��� ���� ����� ]
    ========================================================================*/
    try
    {
        std::string strProcessInfo(g_MyD3D->GetProcessInfo().c_str());
        crashLog += strProcessInfo;
        newCrashInfo.PushData(std::string("Process"), strProcessInfo);
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١����μ��� ���� �α׳���� ���� �߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }

    try {
        crashLog += g_pGCDeviceManager->GetDeviceErrorLog();
    }
    catch (...) {
        crashLog += "�١١١١١١١١١١١١١ٵ���̽� ���� �α׳���� ���� �߻�!!!!�١١١١١١١١١١١١١١١�\n";
    }


    // 	try
    // 	{
    // 		g_pGCDeviceManager->WriteDeviceList(file);
    // 	}
    // 	catch (...)
    // 	{
    // 		crashLog += "�١١١١١١١١١١١١١١١�WriteDeviceList ����� �����߻�!!!!�١١١١١١١١١١١١١١١�\n";
    // 	}
    try {
        fwrite(crashLog.c_str(), crashLog.size(), 1, file);

        KSystemAnalysis kSystem;
        //  �ϵ���� ���� ũ�����䰡 �ȳ��ܼ� ���ٰ� �Ǿ� �����ϴ�.
        std::string strHardwareLog = KncUtil::toNarrowString(kSystem.GetMySystemAnalysis());
        fwrite(strHardwareLog.c_str(), strHardwareLog.size(), 1, file);
        fclose(file);

        //std::wstring strFilePath = GCUTIL_STR::GCStrCharToWide(g_strCurrentPath.c_str());
        std::wstring strFilePath = std::experimental::filesystem::current_path().wstring() + L"\\";
        //GCUTIL:STR::GCStrCharToWide(g_strCurrentPatch.c_str());
        std::wstring strLogPath = strFilePath + L"crashlog.txt";
        BT_AddLogFile(strLogPath.c_str());

        //	�� ũ���� �α׾��
        FILE* fileForNewCrashInfo = fopen("crashlogDS.txt", "w");
        const std::string& strNewCrashInfo = newCrashInfo.GetInfoString();
        fwrite(strNewCrashInfo.c_str(), strNewCrashInfo.size(), 1, fileForNewCrashInfo);
        fclose(fileForNewCrashInfo);
        strLogPath = strFilePath + L"crashlogDS.txt";
        BT_AddLogFile(strLogPath.c_str());


        g_MyD3D->MakeClientFileList(NULL);
        strLogPath = strFilePath + L"clientfilelist.txt";
        BT_AddLogFile(strLogPath.c_str());

        g_kGlobalValue.MakeErrorMotionToFile("ErrorMotion.txt");
        strLogPath = strFilePath + L"ErrorMotion.txt";
        BT_AddLogFile(strLogPath.c_str());


        // 		kSystem.StoreMySystemAnalysis( "HardwareInfo.txt" );
        // 		strLogPath = strFilePath + L"HardwareInfo.txt";
        // 		BT_AddLogFile( strLogPath.c_str() );
        //	�ٽ��� �������
#if !defined ( NO_HACKSHIELD )
        strLogPath = strFilePath + L"HShield\\hshield.log";
        BT_AddLogFile(strLogPath.c_str());
#endif 

        MakeScreenShot(L"crashdata.dat");
        strLogPath = strFilePath + L"crashdata.dat";
        BT_AddLogFile(strLogPath.c_str());
        //HS_StopService();
    }
    catch (...)
    {

    }



    try
    {
        //HS_Uninitialize();
    }
    catch (...)
    {

    }

    try
    {
        std::vector<std::pair<std::wstring, std::wstring>> bufFilePath;

        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"script.first", L"data/stage/script.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"char_script.first", L"data/stage/char_script.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"ai.first", L"data/stage/ai/ai.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"fan_map.first", L"data/stage/fan_map/fan_map.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"ui.first", L"data/stage/ui/ui.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"playertemplate.first", L"data/stage/playertemplate.first"));

        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"pettemplate.first", L"data/stage/pettemplate.first"));

        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"aipet.first", L"data/stage/aipet.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"minigame.first", L"data/stage/MiniGame/minigame.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"map.first", L"data/stage/map.first"));
        bufFilePath.push_back(std::pair<std::wstring, std::wstring>(L"main.exe", L"main.exe"));

        KENU_VERIFY_ACCOUNT_REQ kReq;

        for (std::vector<std::pair<std::wstring, std::wstring>>::iterator vit = bufFilePath.begin(); vit != bufFilePath.end(); ++vit) {
            CheckFileSizeAndSHA(&kReq, vit->first, vit->second);
        }

        std::vector< std::wstring >::iterator vitPathList = g_kGlobalValue.m_vecSHAFilePathList.begin();
        std::vector< DWORD >::iterator vitSize = g_kGlobalValue.m_vecSHAFileSizeList.begin();
        std::vector< std::wstring >::iterator vitSHA = g_kGlobalValue.m_vecSHAFileSHA.begin();

        for (std::vector<std::pair<std::wstring, std::wstring>>::iterator vit = bufFilePath.begin(); vit != bufFilePath.end();
            ++vit, ++vitPathList, ++vitSize, ++vitSHA) {
            std::string strPath = g_kGlobalValue.ConvertWtoA(*vitPathList);
            std::string strSHA = g_kGlobalValue.ConvertWtoA(*vitSHA);

            sprintf(temp, "FilePath : %s\n", strPath.c_str());
            crashLog += temp;
            sprintf(temp, "FileSize : %d\n", *vitSize);
            crashLog += temp;
            sprintf(temp, "FileSHA : %s\n", strSHA.c_str());
            crashLog += temp;
        }
    }
    catch (...)
    {
        crashLog += "�١١١١١١١١١١١١١١١�SHA ���� ����� ���� �߻� �١١١١١١١١١١١١١١١�\n";
    }
}
#endif

#if defined( BUG_TRAP )

void CMyD3DApplication::InstallCrashReport()
{
    BT_InstallSehFilter();
    std::wstring strTitle = L"KR";

    BT_SetAppName(strTitle.c_str());
    BT_SetAppVersion(L"Chase Origin BR");

    BT_SetFlags(BTF_DETAILEDMODE | BTF_ATTACHREPORT | BTF_LISTPROCESSES);

    BT_SetSupportServer(L"https://bugtrap.chaseorigin.com.br/RequestHandler.aspx", 80);
    BT_SetReportFormat(BTRF_XML);
    BT_SetTerminate();
    BT_SetPreErrHandler(CrashCallback, 0);
    BT_SetActivityType(BTA_SENDREPORT);
    BT_ClearLogFiles();
}
#endif

void MakeScreenShot(std::wstring strfilename)
{
    try
    {

        HBITMAP hBitmap;
        RECT rect;
        HDC hMemDC;
        HDC hScrDC;
        HBITMAP hOldBitmap;

        //DC를 얻는다.

        HWND pDesktop = GetDesktopWindow();



        // 포커스를 가진 윈도우의 좌표를 조사한다.      
        GetClientRect( pDesktop, &rect );


        // 윈도우 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다.
        hScrDC = GetDC( pDesktop ); CreateDC( L"DISPLAY", NULL, NULL, NULL );
        hMemDC = CreateCompatibleDC( hScrDC );
        hBitmap = CreateCompatibleBitmap( hScrDC, rect.right - rect.left, rect.bottom - rect.top );
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

        // 화면을 메모리 비트맵으로 복사한다.
        BitBlt( hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
            hScrDC, rect.left, rect.top, SRCCOPY );
        SelectObject( hMemDC, hOldBitmap );

        CxImage cimage( CXIMAGE_FORMAT_PNG );

        if( cimage.CreateFromHBITMAP( hBitmap ) )
        {
            cimage.SetJpegQuality( 75 );
            cimage.Save( strfilename.c_str(), CXIMAGE_FORMAT_PNG );
        }

        DeleteDC( hMemDC );
        DeleteDC( hScrDC );
        DeleteObject( hBitmap );
        DeleteObject( hOldBitmap );
    }
    catch (...)
    {


    }
}


void CMyD3DApplication::RenderFPS()
{
    if( !Show_FPS )
        return;

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );

        g_pGCDeviceManager2->Draw_Text(.654f,-.674f,m_strFrameStats);

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
        g_pGCDeviceManager2->Draw_Text(.65f,-.67f,m_strFrameStats);
    }
    g_pGCDeviceManager2->PopState();
}

void CMyD3DApplication::InsertTestValue()
{
#ifndef __PATH__
    PLAYER* pPlayer = GetMyPlayer();
    if( pPlayer != NULL )
    {
        g_kStatsManager.InsertPage( L"PLAYER STATUS" );
        {
            g_kStatsManager.InsertValue( L"X", &pPlayer->vPos.x );
            g_kStatsManager.InsertValue( L"Y", &pPlayer->vPos.y );

            g_kStatsManager.InsertValue( L"XinQuest", &pPlayer->m_vPos_inQuest.x );
            g_kStatsManager.InsertValue( L"YinQuest", &pPlayer->m_vPos_inQuest.y );

			g_kStatsManager.InsertValue( L"IsRight", &pPlayer->bIsRight );

            g_kStatsManager.InsertValue( L"XSPEED", &pPlayer->x_Speed );
            g_kStatsManager.InsertValue( L"YSPEED", &pPlayer->y_Speed );
            g_kStatsManager.InsertValue( L"MP", &g_MyD3D->MyHead->Level );
            g_kStatsManager.InsertValue( L"HP", pPlayer->GetHPPtr() );
            g_kStatsManager.InsertValue( L"AP", &pPlayer->m_fAbility[ABILITY_ATK] );
            g_kStatsManager.InsertValue( L"DP", &pPlayer->m_fAbility[ABILITY_DEF] );
            g_kStatsManager.InsertValue( L"HPP", &pPlayer->m_fAbility[ABILITY_HP] );

            g_kStatsManager.InsertValue( L"SAP", &pPlayer->m_fAbility[ABILITY_SPECIAL_ATK] );
            g_kStatsManager.InsertValue( L"SDP", &pPlayer->m_fAbility[ABILITY_SPECIAL_DEF] );
            
            g_kStatsManager.InsertValue( L"HPDrain", &pPlayer->m_fAbility[ABILITY_HP_DRAIN_RATE] );
            g_kStatsManager.InsertValue( L"MPDrain", &pPlayer->m_fAbility[ABILITY_MP_DRAIN_RATE] );
            g_kStatsManager.InsertValue( L"HPRecovery", &pPlayer->m_fAbility[ABILITY_HP_RECOVERY_RATE] );
            g_kStatsManager.InsertValue( L"MPRecovery", &pPlayer->m_fAbility[ABILITY_MP_RECOVERY_RATE] );
            g_kStatsManager.InsertValue( L"Critical", &pPlayer->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] );
            g_kStatsManager.InsertValue( L"CounterDef", &pPlayer->m_fAbility[ABILITY_COUNTER_DEF] );


            g_kStatsManager.InsertValue( L"HellSpear", &pPlayer->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] );
            g_kStatsManager.InsertValue( L"CriResist", &pPlayer->m_fAbility[ABILITY_CRITICAL_DAMAGE_RESIST] );
            g_kStatsManager.InsertValue( L"CriDamage", &pPlayer->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] );

            g_kStatsManager.InsertValue( L"CatchResist", &pPlayer->m_fAbility[ABILITY_CATCH_DAMAGE_RESIST] );
            g_kStatsManager.InsertValue( L"PetMPRecovery", &pPlayer->m_fAbility[ABILITY_RECOVERY_UP_PET_MP] );

            g_kStatsManager.InsertValue( L"INCBuffMPRate", &pPlayer->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] );            
            
            g_kStatsManager.InsertValue( L"FRAME", &pPlayer->m_iFrame );
            g_kStatsManager.InsertValue( L"NowMotion", &pPlayer->uiMotion );

            g_kStatsManager.InsertValue( L"INCMP", &g_MyD3D->m_fIncMP );
            g_kStatsManager.InsertValue( L"INCMPRate", &g_MyD3D->m_fIncMPRatio );

            g_kStatsManager.InsertValue( L"INCSMP", &g_MyD3D->m_fIncMPShield );
            g_kStatsManager.InsertValue( L"INCHP", &pPlayer->GetCurFormTemplate().fIncHP );
            g_kStatsManager.InsertValue( L"1SP", &pPlayer->GetCurFormTemplate().SPECIAL1_W );
            g_kStatsManager.InsertValue( L"STORY", &g_MyD3D->Story );
            g_kStatsManager.InsertValue( L"Render", &pPlayer->m_bRender );

            g_kStatsManager.InsertValue( L"RenderBoundBox", &g_kGlobalValue.m_bRenderBoundBox );
            g_kStatsManager.InsertValue( L"RenderFootHold", &g_kGlobalValue.m_bRenderFootHold );
            g_kStatsManager.InsertValue( L"RenderMonsterStatus", &g_kGlobalValue.m_bRenderMonsterStatus );
            g_kStatsManager.InsertValue( L"FrameLock", &g_kGlobalValue.m_bFrameLock );
            g_kStatsManager.InsertValue( L"RenderCount", &g_kGlobalValue.m_iTestValue );
            g_kStatsManager.InsertValue( L"SendCount", &g_MyD3D->m_uiPacketPerSecond );
            g_kStatsManager.InsertValue( L"ShowControls", &g_kGlobalValue.m_bShowControls );

            g_kStatsManager.InsertValue( L"Metamolphosis", &pPlayer->MetamorphosisForm );
            g_kStatsManager.InsertValue( L"Render Position", &g_kGlobalValue.m_bRenderPosition );
            g_kStatsManager.InsertValue( L"Tutorial Motion", &g_MyD3D->MyPlayer[1]->m_iTutorialPlayerAction );
        }
    }

    /*g_kStatsManager.InsertPage( L"PLAYER_PING" );
    {
        for( int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if( MyPlayer[i]->m_kUserInfo.bLive == true )
            {
                std::wstring strName = MyPlayer[i]->m_kUserInfo.strNickName;
                g_kStatsManager.InsertValue( strName, &MyPlayer[i]->m_dwPing );
            }
        }
    }*/
#endif
}

void CMyD3DApplication::RenderDeviceStats()
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		g_pGCDeviceManager2->Draw_Text(-.5f,-.7f,g_MyD3D->m_strDeviceStats, D3DCOLOR_ARGB(255,255,0,255));
	}
	g_pGCDeviceManager2->PopState();
}

void CMyD3DApplication::RenderRightBottomText( WCHAR* pStr_ )
{
	g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
        g_pGCDeviceManager2->Draw_Text(.543f, -.68f, pStr_);
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
        g_pGCDeviceManager2->Draw_Text(.543f-.002f, -.68f+.002f, pStr_, D3DCOLOR_ARGB(255,255,0,0));
    }
    g_pGCDeviceManager2->PopState();
}
bool CMyD3DApplication::IsLocalPlayerObserver()
{
    std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;
    
    for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
    {
        if(vit->dwUID == g_kGlobalValue.m_kUserInfo.dwUID )
            return true;
    }
    return false;
}

MONSTER* CMyD3DApplication::FindMonster(EGCMonster eMonsterType)
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit !=g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if( mit->second->m_iMonsterType == eMonsterType )
			return mit->second;
	}

    return NULL;
}

bool CMyD3DApplication::IsExistPetInfo( GCITEMUID dwItemUID )
{
    if( dwItemUID <= 0 )
        return false;

    std::map< GCITEMUID, KPetInfo >::iterator mit = m_mapPetInfo.find( dwItemUID );

    if( mit != m_mapPetInfo.end() )
    {
        return true;
    }
    return false;
}

void CMyD3DApplication::GetClientHarwareInfo( KEVENT_STAT_CLIENT_INFO& kInfo_ )
{
    kInfo_.m_strCPU = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemCpuData();
    kInfo_.m_nCPUCore = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemCpuCore();
    kInfo_.m_nCPUClock = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemCpuClock();
    kInfo_.m_strDXversion = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemDirectXVersion();
    kInfo_.m_strGraphicChip = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemGraphicCardData();
    kInfo_.m_iVMemory = KSingleton<KSystemAnalysis>::GetInstance()->GetVideoMemory();
    kInfo_.m_strOSType = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemOsData();
    kInfo_.m_iMainMemory = KSingleton<KSystemAnalysis>::GetInstance()->GetSystemRam();
#if defined(USE_JOYSTICK)
	kInfo_.m_bjoystick = IsUseJoystic;
#endif
}

void CMyD3DApplication::CalcQuestPointByDamageType(int iPlayer, int iType)
{
    g_pkQuestManager->CalcQuestPointByDamageType(iPlayer, iType);
    g_pkQuestManager->CalcBackArielCount(iPlayer, iType);
}

void CMyD3DApplication::CalcSpecialAttackPoint(int iPlayer, int iMonster)
{ 
    const int iMotion = MyPlayer[iPlayer]->uiMotion;

    // 스페셜 공격이냐?
    if( iMotion == MyPlayer[iPlayer]->GetCurFormTemplate().SPECIAL1_B
        || iMotion == MyPlayer[iPlayer]->GetCurFormTemplate().SPECIAL1_W 
        )
    {
        // 플레이어가 1필을 쓴 후 이미 한대 맞아 있는 상태인가?
        if( 0 == MyPlayer[iPlayer]->m_mapS1AttackState[iMonster] )
            g_pkQuestManager->IncreaseSpecial1Point(iPlayer);

        MyPlayer[iPlayer]->m_mapS1AttackState[iMonster] = true;
    }

    // 2스페셜 공격이냐?
    if( iMotion == MyPlayer[iPlayer]->GetCurFormTemplate().SPECIAL2_B
        || iMotion == MyPlayer[iPlayer]->GetCurFormTemplate().SPECIAL2_W 
        )
    {
        // 플레이어가 2필을 쓴 후 이미 한대 맞아 있는 상태인가?
        if( 0 == MyPlayer[iPlayer]->m_mapS2AttackState[iMonster] )
            g_pkQuestManager->IncreaseSpecial2Point(iPlayer);

        MyPlayer[iPlayer]->m_mapS2AttackState[iMonster] = true;
    }

    // 3스페셜 공격이냐?
    if( iMotion == MyPlayer[iPlayer]->GetCurFormTemplate().SPECIAL3_B
        || iMotion == MyPlayer[iPlayer]->GetCurFormTemplate().SPECIAL3_W 
        )
    {
        // 플레이어가 1필을 쓴 후 이미 한대 맞아 있는 상태인가?
        if( 0 == MyPlayer[iPlayer]->m_mapS3AttackState[iMonster] )
            g_pkQuestManager->IncreaseSpecial3Point(iPlayer);

        MyPlayer[iPlayer]->m_mapS3AttackState[iMonster] = true;
    }
}

#if !defined( __PATH__ )

void CMyD3DApplication::GifFrameMove()
{
    m_iRecordFrameCount = ++m_iRecordFrameCount%m_iRecordFrame;
    if( iGC_FPS == 0 && 
        m_setGifRecording.second >= 0 && 
        m_setGifRecording.first == true )
    {
        --m_setGifRecording.second;
    }

    if( m_setGifRecording.second < 0 &&
        m_setGifRecording.first == true)
    {
        m_setGifRecording.first = false;
    }

    if( m_iRecordFrameCount == 0 && m_setGifRecording.first == true )
    {
        MakeJPEGFile();
        ++m_iGifRecordingFileCount;
    }
}
void CMyD3DApplication::StartGifRecored( int iSecond, int iFrame )
{
    m_setGifRecording.first = true; 
    m_setGifRecording.second = iSecond; 
    m_iGifRecordingFileCount = 0;
    m_iRecordFrame = iFrame;
    m_iRecordFrameCount = 0;
}
#endif

#if !defined(NO_GAMEGUARD)	|| defined( USE_XTRAP )
static bool sbMailSend = false;
static _Mail kMail;

void MailSendEnd( BOOL bSuccess_ )
{
    sbMailSend = true;

}

void CMyD3DApplication::SendCurrentGameGuardInfo( std::string strAddInfo )
{
#if defined(USE_HTTPPOST)
	KMailSender kMail;
	kMail.SendCurrentGameGuardInfo(strAddInfo);	
#endif
}
#endif

PLAYER* CMyD3DApplication::GetPlayerByUID( DWORD dwUID )
{
    int iIndex = MAX_PLAYER_NUM;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwUID )
        {
            iIndex = i;
            break;
        }
    }

    if ( iIndex == MAX_PLAYER_NUM )
    {
        return NULL;
    }

    return g_MyD3D->MyPlayer[iIndex];
}

void CMyD3DApplication::ActionForSpecialCheck( int SpecialCheckCase )
{
    switch ( SpecialCheckCase )
    {
	case CMyD3DApplication::NONE:
        break;
	case CMyD3DApplication::METAMORPHOSIS_WOLF:
        {
            // 달빛 반지의 경우는 한게임당 한번씩만 체크한다.
            if ( false == bEverSpecialCheck )
			{
				if( GetMyPlayer()->CheckEquipItem( ITEM_ID_MOONLIGHT_RING ) )
					g_pItemMgr->DecInventoryDurationItemForItemID( ITEM_ID_MOONLIGHT_RING );
			}
        }
        break;
    default:
        break;
    }
}

void CMyD3DApplication::SetShakeCameraFrame( int iFrame )
{
	g_kCamera.SetShakeCameraFrame( iFrame );
}

#ifndef __PATH__
#define INCLUDE_KOM_FOLDER
#endif

/// 주어진 소스 경로로부터 폴더 경로를 제외한 파일이름을 얻는다.
/// @param szDest output parameter
/// @param szSrc source path
/// @return void
void GetFileNameExceptFolder( char* szDest, const char* szSrc )
{
	int iDestFeed, iSrcFeed;
	for ( iDestFeed = 0, iSrcFeed = 0;
		NULL != szSrc[iSrcFeed];
		iDestFeed++, iSrcFeed++ )
	{
		szDest[iDestFeed] = szSrc[iSrcFeed];
		if ( '\\' == szSrc[iSrcFeed] ) iDestFeed = -1;
	}
	szDest[iDestFeed] = '\0';
}

bool MakeWritableFile( const char* szFileName )
{
	DWORD dwAtt;
	if ( 0xffffffff != ( dwAtt = ::GetFileAttributesA( szFileName ) ) )
	{
		if( ::SetFileAttributesA( szFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/ ) == FALSE)
		{
			START_LOG( cerr, L"File Attribute change Error : " << szFileName );
			return false;
		}
	}
	return true;
}

/// 파일의 지웁니다. (읽기 전용 무시 )
bool DeleteFileAbsolutely( const char* szFileName )
{
	DWORD dwError;

	MakeWritableFile( szFileName );
	if( !::DeleteFileA( szFileName ) )
	{
		dwError = ::GetLastError();
	}

	return true;
}

void replace(std::string& text,  const std::string& find_token,  const std::string& replace_token)
{
	size_t i = 0;
	while ((i = text.find(find_token)) != std::string::npos)
		text.replace(i, find_token.size(), replace_token);
}

bool CMyD3DApplication::InitDataFiles()
{
#ifdef __ORIGIN_DEV__
    for (int i = 0; i < 45; i++) {
        std::string dataFile = std::to_string(i);
        if (i < 10)
            dataFile = "0" + dataFile;

        g_pGCDeviceManager2->GetMassFileManager()->AddDirectory("./Data_raw/data" + dataFile + "/");
        g_pGCDeviceManager2->GetMassFileManager()->AddMassFile(("./Data/data" + dataFile) + EXT);
    }
#endif

#if defined(FONT_INSTALL_FUNC)
    CreateDirectoryA("Fonts", NULL);
#endif

    g_pGCDeviceManager2->m_pEdgeTexture = new GCDeviceTexture(g_pd3dDevice, "EdgeTexture.dds", false);
    g_pGCDeviceManager2->m_pEdgeTexture->AddRefCount();

    std::vector<std::string> vecStrScriptFiles;

#ifndef __ORIGIN_DEV__
    vecStrScriptFiles.push_back("./Data/Stage/PlayerTemplate" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/AIPet" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/Map" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/UI/UI" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/Fan_map/fan_map" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/AI/AI" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/Minigame/Minigame" EXT);
    vecStrScriptFiles.push_back("./Data/Stage/Char_Script" EXT);

    g_pGCDeviceManager2->GetMassFileManager()->AddDirectory("./Data/Stage/playertemplate/");
    g_pGCDeviceManager2->GetMassFileManager()->AddDirectory("./Data/Stage/script/");
    g_pGCDeviceManager2->GetMassFileManager()->AddDirectory("./Data/Stage/ui/ui/");
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/PlayerTemplate" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddMassFile("./Data/Stage/script" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/AIPet" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/Map" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/Fan_map/fan_map" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/AI/AI" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/Minigame/Minigame" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/Char_Script" EXT);
    g_pGCDeviceManager2->GetMassFileManager()->AddKeepFile("./Data/Stage/SkillTree" EXT);
#endif
    KLuaManager	luaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, "DataFiles.lua") == false)
    {
        MessageBoxA(NULL, "DataFiles.lua Load Failed!", NULL, MB_OK);
        exit(0);
    }

#if defined(CHECK_CLIENT_CRCLIST)
    std::vector<std::string> vecFileList;
#endif

    LUA_BEGIN_TABLE("DataFilesCommon", MessageBoxA(NULL, "Table DataFilesCommon Load Filed", NULL, MB_OK))
    {
        std::string strFileName;
        for (int i = 1; ; ++i)
        {
            LUA_GET_VALUE_NOASSERT(i, strFileName, break);
            if (g_pGCDeviceManager2->GetMassFileManager()->AddMassFile(strFileName.c_str()) == false)
            {
            }
#if defined(CHECK_CLIENT_CRCLIST)
            vecFileList.push_back(strFileName);
#endif
    }
}
    LUA_END_TABLE(MessageBoxA(NULL, "Table DataFilesCommon Load Filed", NULL, MB_OK));


    if (SiKGCMultipleLanguages()->UseMultipleLanguages())
    {
        switch (SiKGCMultipleLanguages()->GetLanguageTypeNumber())
        {
        case 0: // Eng
            LUA_BEGIN_TABLE("DataFilesEnglish", MessageBoxA(NULL, "Table DataFilesEnglish Load Filed", NULL, MB_OK))
            {
                std::string strFileName;
                for (int i = 1; ; ++i)
                {
                    LUA_GET_VALUE_NOASSERT(i, strFileName, break);
                    if (g_pGCDeviceManager2->GetMassFileManager()->AddMassFile(strFileName.c_str()) == false)
                    {
                    }
#if defined(CHECK_CLIENT_CRCLIST)
                    vecFileList.push_back(strFileName);
#endif
                }
            }
            LUA_END_TABLE(MessageBoxA(NULL, "Table DataFilesKoreaLoad Filed", NULL, MB_OK));

            break;

        case 2:
            LUA_BEGIN_TABLE("DataFilesKorea", MessageBoxA(NULL, "Table DataFilesKorea load Filed", NULL, MB_OK))
            {
                std::string strFileName;
                for (int i = 1; ; ++i)
                {
                    LUA_GET_VALUE_NOASSERT(i, strFileName, break);
                    if (g_pGCDeviceManager2->GetMassFileManager()->AddMassFile(strFileName.c_str()) == false)
                    {
                    }
#if defined(CHECK_CLIENT_CRCLIST)
                    vecFileList.push_back(strFileName);
#endif
                }
            }
            LUA_END_TABLE(MessageBoxA(NULL, "Table DataFilesKoreaLoad Filed", NULL, MB_OK));

            break;
            }
        }
    else
    {
        LUA_BEGIN_TABLE("DataFilesKorea", MessageBoxA(NULL, "Table DataFilesCommon Load Filed", NULL, MB_OK))
        {
            std::string strFileName;
            for (int i = 1; ; ++i)
            {
                LUA_GET_VALUE_NOASSERT(i, strFileName, break);
                if (g_pGCDeviceManager2->GetMassFileManager()->AddMassFile(strFileName.c_str()) == false)
                {
                }
#if defined(CHECK_CLIENT_CRCLIST)
                vecFileList.push_back(strFileName);
#endif
            }
        }
        LUA_END_TABLE(MessageBoxA(NULL, "Table DataFilesKoreaLoad Filed", NULL, MB_OK));

            }

    std::string strStr("str.stg");

    if (SiKGCMultipleLanguages()->UseMultipleLanguages())
    {
        strStr = SiKGCMultipleLanguages()->ConvertNationStringFileNameA("str_");
    }

    g_pkStrLoader->LoadFromKom(strStr.c_str());

    std::vector<std::string>::iterator vecIter = vecStrScriptFiles.begin();
#ifndef DEV_BUILD
    for (; vecIter != vecStrScriptFiles.end(); vecIter++)
    {
        if (!g_pGCDeviceManager2->GetMassFileManager()->HaveMassFile((*vecIter).c_str()))
        {
            exit(0);
        }
    }
#endif
    return true;
}

void CMyD3DApplication::AngelsEggHelpMessage()
{
    g_pkChatManager->AddChatMsg( 
    	g_pkStrLoader->GetReplacedString( STR_ID_DESC_HALLOWEENPUMPKIN_1, "l", 
    	g_pkUIScene->m_pkOption->GetKeySettingWnd()->GetKeyStr( g_MyD3D->MyCtrl->m_keyUseItem ) ) 
    	, KGCChatManager::CHAT_TYPE_ADMIN
    	);
    g_pkChatManager->AddChatMsg( 
    	g_pkStrLoader->GetReplacedString( STR_ID_DESC_HALLOWEENPUMPKIN_2, "l", 
    	g_pkUIScene->m_pkOption->GetKeySettingWnd()->GetKeyStr( g_MyD3D->MyCtrl->m_keyAttack ) )
    	, KGCChatManager::CHAT_TYPE_ADMIN
    	);
}

void CMyD3DApplication::CaptainHelpMessage()
{
	g_pkChatManager->AddChatMsg( 
		g_pkStrLoader->GetReplacedString( STR_ID_CAPTAIN_DESC1, "l", 
		g_pkUIScene->m_pkOption->GetKeySettingWnd()->GetKeyStr( g_MyD3D->MyCtrl->m_keyUseItem ) ) 
		, KGCChatManager::CHAT_TYPE_ADMIN
		);
	g_pkChatManager->AddChatMsg( 
		g_pkStrLoader->GetReplacedString( STR_ID_CAPTAIN_DESC2, "l", 
		g_pkUIScene->m_pkOption->GetKeySettingWnd()->GetKeyStr( g_MyD3D->MyCtrl->m_keyAttack ) )
		, KGCChatManager::CHAT_TYPE_ADMIN
		);
}

void CMyD3DApplication::ShowExplorer(BOOL bShow, std::wstring strCheckString)
{
	int max_wnd_count = 500;
	HWND hWnd = ::GetTopWindow( NULL );

	WCHAR strBuf[1024];
	memset( strBuf, 0, sizeof(strBuf) );
	std::wstring str = L"";

	static int restore_count = 0;

	if( restore_count >= 0 && bShow ) return;
	else if( restore_count < 0 && !bShow ) return;

	if( bShow ) restore_count += 1;
	else restore_count -= 1;

	HWND BasehWnd = NULL;

	while( max_wnd_count > 0 )
	{
		max_wnd_count--;
		BasehWnd = hWnd;

		str = strBuf;

		::GetClassName((HWND)BasehWnd, strBuf, 1023);
		if( lstrcmp(strBuf, L"IEFrame") == 0 )
		{
			if( (BasehWnd = ::FindWindowEx((HWND)BasehWnd, NULL, L"WorkerW", NULL)) )
			{
				::GetWindowText( hWnd, strBuf, 1023 );

				str = strBuf;
				if( str.find( strCheckString) != str.npos )
				{
					BasehWnd = ::GetNextWindow( hWnd, GW_HWNDNEXT );
					if( bShow ) ::ShowWindow( hWnd, SW_RESTORE );
					else ::ShowWindow( hWnd, SW_SHOWMINIMIZED );

					hWnd = BasehWnd;
					continue;
				}
			}
		}

		hWnd = ::GetNextWindow( hWnd, GW_HWNDNEXT );
	}
}

int CMyD3DApplication::GetPetPromotion( GCITEMUID dwUID )
{
	if( m_mapPetInfo.find( dwUID ) == m_mapPetInfo.end() )
		return -1;

	return m_mapPetInfo[dwUID].m_cPromotion;
}

KPetInfo CMyD3DApplication::GetMyPet( DWORD dwPetID, int iPromotion )
{
	std::map<GCITEMUID, KPetInfo>::iterator mit;
	for( mit = m_mapPetInfo.begin() ; mit != m_mapPetInfo.end() ; ++mit )
	{
		if( mit->second.m_dwID  == dwPetID &&
			mit->second.m_cPromotion == iPromotion )
			return mit->second;
	}

	//이쪽으로 올일이 없길 바람...
	KPetInfo kPet;
	kPet.Init();
	return kPet;
}

void CMyD3DApplication::LoadPvPNotUseItems( void )
{
	KLuaManager luaMgr;
	char strTemp[MAX_PATH] = {0,};    

	m_vtPvPNotUseItems.clear();

	if( GCFUNC::LoadLuaScript( luaMgr, "pvp_cant_items.lua" ) == false )
	{
		//MessageBoxA( NULL, "pvp_cant_items Load Failed!", NULL, MB_OK );
		return;
	}

	m_vtPvPNotUseItems.clear();

	LUA_BEGIN_TABLE( "ItemList", return )
	{
		for( int iItemList = 1; ; ++iItemList )
		{
			DWORD iItemID = -1;
			if( FAILED( luaMgr.GetValue( iItemList, iItemID ) ) )
				break;
			m_vtPvPNotUseItems.push_back( iItemID );
		}
		LUA_END_TABLE( return )
	}
}

void CMyD3DApplication::UnEquipMyPvPNotUseItems( void )
{
    g_MyD3D->m_OriginalEquip = g_kGlobalValue.m_kUserInfo;
	for(int i=0;i<g_kGlobalValue.m_kUserInfo.GetCharSize();++i)
	{
		std::vector<DWORD>::iterator itNoUseItem = m_vtPvPNotUseItems.begin();
		for(;m_vtPvPNotUseItems.end() != itNoUseItem; ++itNoUseItem )
		{
			g_pItemMgr->UnequipItemForItemID( &g_kGlobalValue.m_kUserInfo.vecCharInfo[i], (*itNoUseItem) );
		}
	}
	g_MyD3D->m_TempPlayer.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
	//SendEquipItemList();
    SendEquipItemList( &g_MyD3D->m_TempPlayer );
}

bool CMyD3DApplication::IsPvPNotUseItem( DWORD dwItemID )
{
	std::vector<DWORD>::iterator itNoUseItem = m_vtPvPNotUseItems.begin();
	for(;m_vtPvPNotUseItems.end() != itNoUseItem; ++itNoUseItem )
	{
		if( (*itNoUseItem) == dwItemID )
			return true;
	}

	return false;
}


bool CMyD3DApplication::AlertCheck_Mission()
{
    SCharInfo& sCharInfo = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();
    return ( (g_kGlobalValue.m_kUserMission.GetCompleteMissionNum( sCharInfo.iCharType ) > 0)
        || g_kGlobalValue.m_kUserMission.IsNewMission( sCharInfo.iCharType ) );
}

bool CMyD3DApplication::AlertCheck_MSN()
{
    return g_pkUIScene->m_pkNateOn->IsNew();
}

bool CMyD3DApplication::AlertCheck_Calendar()
{
    return ( g_kGlobalValue.CheckCalendarAlert() );
}

bool CMyD3DApplication::AlertCheck_MyInfoScene()
{	
    return g_pkUIScene->m_pkTitleCollection->IsCompleteCTMissionAndNotGet();
}

bool CMyD3DApplication::AlertCheck_SkillTree()
{
    return ( SiGCSKT()->GetNewSkillNum() > 0 );
}

bool CMyD3DApplication::AlertCheck_CoupleWnd()
{
    return SiKGCCoupleSystem()->GetPartnerState() == KGCCoupleSystem::GCCS_COUPLE_WND;
}

bool CMyD3DApplication::AlertCheck_Recommend()
{
    return g_pkUIScene->m_pkRecommendList->IsNew();
}

bool CMyD3DApplication::AlertCheck_Survey()
{
    return g_pkUIScene->m_pkSurveyDlg->IsHaveQuestion();
}

void CMyD3DApplication::UnequipCashSkill()
{	
	for(int i=0;i<g_kGlobalValue.m_kUserInfo.GetCharSize();++i)
	{
		SCharInfo* pCharInfo = &g_kGlobalValue.m_kUserInfo.vecCharInfo[i];

		for( int j=0; j< (int)pCharInfo->vecItemInfo.size(); ++j )
		{
			GCItem* kItem = g_pItemMgr->GetItemData( pCharInfo->vecItemInfo[j].iItemID );

			if( kItem->dwSkillPosition != 0 && kItem->eMoneyType == EMT_CASH )
				g_pItemMgr->UnequipItemForItemID( pCharInfo, pCharInfo->vecItemInfo[j].iItemID );
		}
	}
	g_MyD3D->m_TempPlayer.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    SendEquipItemList( &g_MyD3D->m_TempPlayer );
}



bool CMyD3DApplication::ConnectLoginServer()
{    
    g_kGlobalValue.ServerInfo.usServerPort = std::stoi(LOGIN_SERVER_PORT);
	g_kGlobalValue.ServerInfo.dwServerIP = ::inet_addr(LOGIN_SERVER_IP);

	if( KP2P::GetInstance()->Init( g_MyD3D->m_hWnd ) == false )
	{
		KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL1 ).c_str(),
			g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL2 ).c_str(),
			KMsgBox::MBOX_OK );
		g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL1 ).c_str(),
			g_pkStrLoader->GetString( STR_ID_NETWORK_INITIALIZE_FAIL2 ).c_str() );
		return false;
	}

	if( KP2P::GetInstance()->ConnectToLoginServer(LOGIN_SERVER_IP, g_kGlobalValue.ServerInfo.usServerPort, false ) == false )
	{
		g_MyD3D->ExitWithMsgDlg( g_pkStrLoader->GetString( STR_ID_LOGINSERVER_CONNECT_FAIL1 ).c_str(),
			g_pkStrLoader->GetString( STR_ID_LOGINSERVER_CONNECT_FAIL2 ).c_str() );       

		return false;
	}

	std::vector<int> veciEmpty;
	veciEmpty.clear();

    KP2P::GetInstance()->m_spNUserProxy->SendID( KNUserEvent::ENU_CLIENT_CONTENTS_FIRST_INIT_INFO_REQ );
    bool bCheck = g_MyD3D->WaitForServerAck( Result_Loading_image, INT_MAX, 120000, TIME_OUT_VALUE );

    return bCheck;
}

int CMyD3DApplication::GetDungeonLevel( void )
{
    int iDungeonLevel = 0;
    if ( SiKGCWorldMapManager()->IsHeroDungeon() )
    {
        iDungeonLevel = g_pkQuestManager->GetHeroDungeonLevel();
    }
    else
    {
        iDungeonLevel = SiKGCRoomManager()->GetRoomInfo().nDungeonLevel;
    }

    return iDungeonLevel;
}

/*[ 파일 변조 해킹 방지 작업 ] : 201017 EXXA
    스크립트의 파일 점유권을 읽기 모드로 가진다.
    NUM_FILE과 체크하는 스크립트 파일 목록의 수는 일치해야 한다.
========================================================================*/
size_t  GetNumOfScriptFile();   //  맨 밑에 정의되어 있다.
class LockScriptReadOnly
{
    friend size_t GetNumOfScriptFile();
public:
    enum
    {
        NUM_FILE = 10,
    };

    LockScriptReadOnly()
    {
        if( static_cast<size_t>(NUM_FILE) != GetNumOfScriptFile() ){
            ::MessageBoxA( NULL, "NUM_FILE과 스크립트 파일 숫자를 맞추세요", "지금 당장", MB_OK );
        }
#if defined(__PATH__)
        for( int i=0; i<NUM_FILE; ++i ){
            m_arrHandle[i]       = INVALID_HANDLE_VALUE;

            if ( INVALID_HANDLE_VALUE == ( m_arrHandle[i] = ::CreateFileA( m_strFileName[i],
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_READONLY,
                NULL ) ) )
            {
                this->UnLockScriptReadOnly();
                exit(0);    //  해킹범들은 이유를 알필요가 없다. 왜 프로그램 죽는지 이유도 알지 말고 죽어라.
            }
        }
#endif
    }

    ~LockScriptReadOnly()
    {
#if defined(__PATH__)
        this->UnLockScriptReadOnly();
#endif
    }
private:
    void UnLockScriptReadOnly()
    {
        for( int i=0; i<NUM_FILE; ++i ){
            if( INVALID_HANDLE_VALUE != m_arrHandle[i] ){
                ::CloseHandle( m_arrHandle[i] );
            }
        }
    }

    static char* m_strFileName[/*NUM_FILE*/];
    static HANDLE m_arrHandle[NUM_FILE];
};
char* LockScriptReadOnly::m_strFileName[] = 
{
    "./data/stage/script.first",
    "./data/stage/char_script.first",
    "./data/stage/ai/ai.first",
    "./data/stage/fan_map/fan_map.first",
    "./data/stage/ui/ui.first",
    "./data/stage/playertemplate.first",
    "./data/stage/aipet.first",
    "./data/stage/MiniGame/minigame.first",
    "./data/stage/map.first",
    "./main.exe"
};

size_t  GetNumOfScriptFile()
{
    return sizeof( LockScriptReadOnly::m_strFileName )/sizeof(char*);
}

std::string GetLanguageType()
{
    return SiKGCMultipleLanguages()->GetLanguageType();
}
HANDLE LockScriptReadOnly::m_arrHandle[NUM_FILE];

//  생성 소멸이 알아서 되게 하기 위해서 정적 전역 객체로.
//  이거 딴데서 쓸 필요가 없다.
static LockScriptReadOnly  g_lockScriptObject;
//========================================================================
//  파일변조 해킹 작업 끝
//========================================================================



std::string CMyD3DApplication::GetProcessInfo()
{
	std::string     strLog;
	//strLog  +=  std::string("[Process]\n");

	OSVERSIONINFO   OSversionInfo;
	OSversionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &OSversionInfo );

	//  윈도우 버젼이 NT계열이 아니면 "psaip.h"를 사용할수 없다.
	if( VER_PLATFORM_WIN32_NT != OSversionInfo.dwPlatformId ){
		strLog += std::string("Not Windows NT"); 
		return strLog;
	}

	typedef BOOL  (WINAPI *pEnumProcessModules)( HANDLE, HMODULE*, DWORD, LPDWORD );
	typedef DWORD  (WINAPI *pGetModuleBaseNameA)( HANDLE, HMODULE, LPSTR, DWORD );
	typedef BOOL  (WINAPI *pEnumProcesses)( DWORD*, DWORD, DWORD* );

	HINSTANCE hDllInstance = ::LoadLibraryA( "psapi.dll" );
	if( hDllInstance ){
		pEnumProcessModules     DllFunc_EnumProcessModules  = (pEnumProcessModules)GetProcAddress(hDllInstance,"EnumProcessModules");
		pGetModuleBaseNameA     DllFunc_GetModuleBaseNameA  = (pGetModuleBaseNameA)GetProcAddress(hDllInstance,"GetModuleBaseNameA");
		pEnumProcesses          DllFunc_EnumProcesses       = (pEnumProcesses)GetProcAddress(hDllInstance,"EnumProcesses");

		if( DllFunc_EnumProcessModules&& DllFunc_GetModuleBaseNameA && DllFunc_EnumProcesses ){
			DWORD   arProc[1024];
			DWORD   cb;
			char    str[256];
			char    FullPath[MAX_PATH] = {0,};

			DllFunc_EnumProcesses(arProc, sizeof(arProc),&cb);
			int nProc = cb/sizeof(DWORD);

			HMODULE hModule;

			for( int i=0; i<nProc; ++i ){
				HANDLE  hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, arProc[i] );
				if( hProcess ){
					BOOL    Result = DllFunc_EnumProcessModules( hProcess, &hModule, sizeof(hModule), &cb );
					if( Result == 0 ){
						//sprintf_s( str, "EnumProcessModules = Error %d", GetLastError() );
					}
					else{
						DllFunc_GetModuleBaseNameA( hProcess, hModule, FullPath, sizeof(FullPath) );
						//sprintf_s( str, "%5d - %s", arProc[i], FullPath );
						sprintf_s( str, "%s", FullPath );
						strLog += std::string(str) + '\n';
					}
					CloseHandle(hProcess);
				}
				else{
					//sprintf_s(str,"OpenProcess - Error %d", GetLastError() );
				}
			}
		}
		else{
			strLog += std::string("dll이 없다. 패치를 잘 안받았다는 소리");
		}
		FreeLibrary(hDllInstance);
	}

	return strLog;
}
void CMyD3DApplication::CheckBefor()
{
	//g_kGlobalValue.m_bHackCheck 이 값이 true 가 되어야만 g_kGlobalValue.m_ulRandomCount 값이 바뀔수 있다
	//즉 False 인데도 값이 바뀌었다면 메모리 핵.
	if (g_kGlobalValue.m_ulRandomCount != g_kGlobalValue.m_ulGanadara)
	{
		if (g_kGlobalValue.m_bHackCheck == false)
			exit(-1);
	}
		

}

void CMyD3DApplication::RandomShutdown()
{
    if (!g_kGlobalValue.m_bHackCheck)
        return;	

    if (g_kGlobalValue.m_ulRandomCount >0)
        g_kGlobalValue.m_ulRandomCount--;

    if (g_kGlobalValue.m_ulRandomCount <= 0)
    {
        try
        {
            std::wstringstream stm;
            stm<<"==================================================================================\n";
            stm<<"Login ID     : "<<GCUTIL_STR::GCStrWideToChar( g_kGlobalValue.m_kUserInfo.strLogin.c_str() )<<"\n";
            stm<<"UserNickName : "<<GCUTIL_STR::GCStrWideToChar( g_kGlobalValue.m_kUserInfo.strNickName.c_str() )<<"\n";
            stm<<"Current Char : "<<g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType<<"\n";
            stm<<"==================================================================================\n"; 

            if (KDLLChecker::GetInstance()->GetbSnapCheck())
                stm << "==============================Checked Dll=====================================\n";
            std::set<std::string> setList = KDLLChecker::GetInstance()->GetNoList();
            if (!setList.empty())
            {
                stm << "==============================================================================\n";
                stm << "===========================It's not DLL in List===============================\n"; 
                stm << "==============================================================================\n";

                std::set<std::string>::iterator setIter;
                for (setIter = setList.begin() ; setIter != setList.end() ; ++setIter)
                {
                    USES_CONVERSION;

                    std::wstring strTmp = std::wstring(A2W(setIter->c_str()));

                    stm << strTmp; 
                    stm << "\n";
                }

                stm << "==============================================================================\n";
                stm << "==============================================================================\n";
            }
            //SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_NOT_EQUAL_QUEST_INFO, stm.str() );

        }
        catch(...)
        {

        }
       
        ::MessageBoxW( NULL, L"Error 0xAE", L"Error", MB_OK );
        exit(-1);
    }
}

int CMyD3DApplication::GetTrainingNum( void )
{
    return SiKGCAgitGameManager()->GetBeforeBrokenTile();
}

void CMyD3DApplication::ClearTrainingTile( void )
{
#if defined( USE_AGIT_SYSTEM )
    SiKGCAgitGameManager()->ClearTrainingTileImg( g_pkAgitOverlayUI->GetTraingObjUID() );
#endif
}

int CMyD3DApplication::CheckDunMonsterSummonSlot(  )
{
    return g_kMonsterManager.GetNextCreateMonstersNum();
}

void CMyD3DApplication::AddGameItem( char cWhat, float fPosX, float fPosY, float fXSpeed, float fYSpeed, WORD wItemLife )
{
    if(false == g_kGlobalValue.m_kUserInfo.bHost )
        return;

    D3DXVECTOR2 vPos(fPosX, fPosY);
    MyItms->Add(cWhat, vPos, false, wItemLife, fXSpeed, fYSpeed);
}

void CMyD3DApplication::OnChaSelectSceneBtn()
{
    // 초기화 해줘야 할 정보들 
    g_MyD3D->Clear_PlayerData();
    SiKGCPostLetterManager()->ClearAllLetterInfo();

	g_MyD3D->m_pStateMachine->GoCharSelect();
}

void CMyD3DApplication::OnServerSceneBtn()
{
	g_MyD3D->m_pStateMachine->GoServer();
}

void CMyD3DApplication::DestoryProcess()
{
    if( KP2P::GetInstance()->IsConnectedToGameServer() )
    {
        KP2P::GetInstance()->Send_ExitNormalNot();

        if( g_kGlobalValue.m_bSendErrReport == false )
        {
#if defined( COLLECT_SKILL_HOTKEY_USED_INFO )
            if( g_kGlobalValue.IsEmptySSkillHotKeyUsedInfo() ) {
                KP2P::GetInstance()->Send_SSkillHotKeyUsedInfo( g_kGlobalValue.GetSSkillHotKeyUsedInfo() );
            }                    
#endif
            // 게임 끝날때 서버로 리포팅해야 하는 것들 여기로
            if(  g_MyD3D->m_pStateMachine &&
                g_MyD3D->m_pStateMachine->GetState() != GS_LOGO && g_MyD3D->m_pStateMachine->GetState() != GS_INIT_LOADING )
                KP2P::GetInstance()->Send_StatUserHistory( g_MyD3D->m_pStateMachine->GetUserHistory() );

            if( g_kGlobalValue.m_iCloseType == KEVENT_CLOSE_CONNECTION_NOT::CCT_DETECT_HACK )
            {
                SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_EXITWITHHACKALERT, g_kGlobalValue.m_stmHackComment.str().c_str() );
            }

            g_kGlobalValue.m_bSendErrReport = true;
        }

        int iScreenWidth = static_cast<int>( GC_SCREEN_WIDTH * g_pGCDeviceManager->GetWindowScaleX() );
        int iScreenHeight = static_cast<int>( GC_SCREEN_HEIGHT * g_pGCDeviceManager->GetWindowScaleY() );

        int iScreenMaxWidth = GC_SCREEN_WIDTH;
        int iScreenMaxHeight = GC_SCREEN_HEIGHT;
        DWORD dwNumModes = g_MyD3D->m_KGCOption.GetNumOfResolution();
        for (unsigned int i=dwNumModes - 1; i >= 0 ;i--)
        {
            D3DModeInfo sResolution;
            if( g_MyD3D->m_KGCOption.GetResolution( i, sResolution ) == true ){
                iScreenMaxWidth = sResolution.Width;
                iScreenMaxHeight = sResolution.Height;
                break;
            }
        }
        KP2P::GetInstance()->Send_StatGameResolution( iScreenWidth, iScreenHeight, iScreenMaxWidth, iScreenMaxHeight );

        SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, SiKGCRenderManager()->GetUsingVS() ? KClientErr::CE_USING_VS_TRUE : KClientErr::CE_USING_VS_FALSE );

        if ( GC_GLOBAL_DEFINE::bThreadLoadTexture ) 
        {
            DWORD dwTextureLoadCount = SiKTextureLoadManager()->GetSlowLoadCount();
            SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_SLOW_TEXTURE_LOAD_COUNT, dwTextureLoadCount );
        }

        //  종료직전 모은 에러 보낸다.
        SiGCClientErrorManager()->SendCollectError();
    }
    g_kGlobalValue.m_bIsOnGameClose = true;

    if(g_MyD3D->m_pStateMachine) // 이거 체크 안하면 안된다
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME ||
            g_MyD3D->m_pStateMachine->GetState() == GS_GAME_LOADING ||
            g_MyD3D->m_pStateMachine->GetState() == GS_GAME_END )
        {
            g_pStateGame->ClearGameResource();
            g_MyD3D->MyReplay->SaveReplayEnd();
        }
    }

    if( g_pBrowser )
        g_pBrowser->UnInit();
}

bool CMyD3DApplication::LoadLanguageFilterScript( OUT std::vector<std::pair<int, int>>& vecNickNameCheck_, OUT std::vector<std::pair<int, int>>& vecUnicodeCheck_ )
{
    vecNickNameCheck_.clear();
    vecUnicodeCheck_.clear();

    KLuaManager luaMgr;

    _JIF( GCFUNC::LoadLuaScript( luaMgr, "LanguageFilter.lua" ), return false );

    _JIF( luaMgr.BeginTable( "ADDPERMITNICKNAME" ) == S_OK, return false );
    for ( int i = 1 ;; ++i ) {
        int iStart =0;
        int iEnd = 0;
        std::pair<DWORD,DWORD> prIP;

        if( luaMgr.BeginTable( i ) != S_OK) break;
        _JIF( luaMgr.GetValue( 1, iStart ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 2, iEnd ) == S_OK, return false );
        _JIF( luaMgr.EndTable() == S_OK, return false );

        vecNickNameCheck_.push_back( std::make_pair( iStart, iEnd ) );
    }
    _JIF( luaMgr.EndTable() == S_OK, return false );

    _JIF( luaMgr.BeginTable( "ADD_UNICODE_CHECK" ) == S_OK, return false );
    for ( int i = 1 ;; ++i ) {
        int iStart =0;
        int iEnd = 0;
        std::pair<DWORD,DWORD> prIP;

        if( luaMgr.BeginTable( i ) != S_OK) break;
        _JIF( luaMgr.GetValue( 1, iStart ) == S_OK, return false );
        _JIF( luaMgr.GetValue( 2, iEnd ) == S_OK, return false );
        _JIF( luaMgr.EndTable() == S_OK, return false );

        vecUnicodeCheck_.push_back( std::make_pair( iStart, iEnd ) );
    }
    _JIF( luaMgr.EndTable() == S_OK, return false );

    return true;
}

void CMyD3DApplication::InitializeData()
{
    SiKGCDepotManager()->Initialize();
	SiKGCGiftBoxManager()->Reset();
}

void CMyD3DApplication::WinMessageBox( char *szMessage )
{
#if !defined( __PATH__ )
    ::MessageBoxA( NULL, szMessage, "WinMessageBox", MB_OK );
#endif
}

EGCGameMode CMyD3DApplication::GetCurrentGameMode( void )
{
    return SiKGCRoomManager()->GetGameMode();
}