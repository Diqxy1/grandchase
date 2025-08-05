#include "stdafx.h"
#include "D3DUIManager.h"
//
//
//
//
//
//
//
//
#include "../MyD3D.h"
//
//
//

#include "KGCMultipleLanguages.h"
#include "./DragManager.h"
//

KD3DUIManager*  g_pkUIMgr       = NULL;
HWND            g_hUIMainWnd    = NULL;

// predefined scene factory objects
static KD3DUISceneFactory   g_kSceneFactory;

// predefined window factory objects
static KD3DWndFactory       	g_kWndFactory;
static KD3DFrameWndFactory  	g_kFrameWndFactory;
static KD3DEditFactory      	g_kEditFactory;
static KD3DStaticFactory    	g_kStaticFactory;
static KD3DImageIconFactory    	g_kImageIconFactory;
static KD3DListBoxFactory   	g_kListBoxFactory;
static KD3DScrollbarFactory 	g_kScrollbarFactory;
static KD3DTabFactory       	g_kTabFactory;
static KD3DComboBoxFactory  	g_kComboBoxFactory;
static KD3DCheckBoxFactory  	g_kCheckBoxFactory;
static KD3DStaticLinkFactory	g_kStaticLinkFactory;
static KD3DStaticBtnFactory		g_kStaticBtnFactory;
static KD3DSizingBoxBtnFactory	g_kSizingBoxBtnFactory;
static KD3DMoveWndFactory		g_kMoveWndFactory;
static KD3DWndActiveChildFactory	g_kWndActiveChildFactory;

void KD3DUIManager::ResetFonts()
{
    g_pkFontMgr_original->Release();
	g_pkFontMgrSmall_original->Release();
    g_pkFontMgrOutline_original->Release();
    g_pkFontMgrOutlineSmall_original->Release();
	g_pkFontMgr14_original->Release();
    g_pkFontMgr16_original->Release();
    g_pkFontMgrTitle_original->Release();
    g_pkFontMgrOutline14_original->Release();
    g_pkFontMgrOutline16_original->Release();
    g_pkFontMgrOutlineTitle_original->Release();

	g_pkFontMgr_mid_scale->Release();
	g_pkFontMgrSmall_mid_scale->Release();
	g_pkFontMgrOutline_mid_scale->Release();
	g_pkFontMgrOutlineSmall_mid_scale->Release();
	g_pkFontMgr14_mid_scale->Release();
	g_pkFontMgr16_mid_scale->Release();
	g_pkFontMgrTitle_mid_scale->Release();
	g_pkFontMgrOutline14_mid_scale->Release();
	g_pkFontMgrOutline16_mid_scale->Release();
	g_pkFontMgrOutlineTitle_mid_scale->Release();
    g_pkFontMgr->Release();
	g_pkFontMgrSmall->Release();
	g_pkFontMgrOutline->Release();
	g_pkFontMgrOutlineSmall->Release();
    g_pkFontMgr14->Release();
    g_pkFontMgr16->Release();
    g_pkFontMgrTitle->Release();
    g_pkFontMgrOutline14->Release();
    g_pkFontMgrOutline16->Release();
    g_pkFontMgrOutlineTitle->Release();

    CreateFonts();
}

// 2010/08/22, 이상호(shmhlove) : 리소스폴더에 있는 폰트를 윈도우에 설치
bool KD3DUIManager::InstallFont( IN const char *szCopyWinFontPath, IN char *szFontName )
{
    bool bResult = false;

    if( NULL == szCopyWinFontPath || NULL == szFontName )
    {   goto RETURN_INSTALLFONT;    }

    char *pszTemp = StrRChrA(szCopyWinFontPath, &szCopyWinFontPath[ lstrlenA(szCopyWinFontPath)-1 ], '\\');

    char szLocalFontPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, szLocalFontPath);
    lstrcatA(szLocalFontPath, "\\Fonts\\");
    lstrcatA(szLocalFontPath, pszTemp++);
    if( CopyFileA(szLocalFontPath, szCopyWinFontPath, FALSE) == FALSE)
    {   goto RETURN_INSTALLFONT;    }

    if( AddFontResourceA( szCopyWinFontPath ) == FALSE )
    {   goto RETURN_INSTALLFONT;    }

    SendMessage( HWND_BROADCAST, WM_FONTCHANGE, 0, 0 );

    OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    char szKey[255];
    GetVersionEx( &vi );
    switch( vi.dwPlatformId )
    {
    case VER_PLATFORM_WIN32_NT:
        lstrcpyA(szKey, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
        break;
    default:
        lstrcpyA(szKey, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Fonts");
        break;
    }

    HKEY hkResult;
    DWORD dwDisp;
    if( ERROR_SUCCESS != RegCreateKeyExA( HKEY_LOCAL_MACHINE, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkResult, &dwDisp) )
    {   goto RETURN_INSTALLFONT;    }
    if( ERROR_SUCCESS != RegSetValueExA( hkResult, szFontName, 0, REG_SZ, (LPBYTE)pszTemp, sizeof(TCHAR*)*(lstrlenA(pszTemp)+1)) )
    {   goto RETURN_INSTALLFONT;    }

    RegCloseKey( hkResult );
    bResult = true;

RETURN_INSTALLFONT:
    return bResult;
}

// 2011/02/18, 이상호(shmhlove) : 리소스폴더에 있는 폰트파일의 글꼴이름과 윈도우에 설치되었는지 확인
bool KD3DUIManager::CheckInstallFont( char *szFontFileName, char *szFontName ) 
{
    bool bSuccess = false;
    if( NULL == szFontFileName )
    {
        if( 0 <= g_kGlobalValue.m_iSelectFontIndex && g_kGlobalValue.m_iSelectFontIndex < static_cast<int>(g_kGlobalValue.m_vecFontList.size()) )
            szFontFileName = const_cast<char*>(g_kGlobalValue.m_vecFontList[g_kGlobalValue.m_iSelectFontIndex].c_str());
        else
            return false;
    }
    
    char szLocalFontPath[ 1024 ];
    GetCurrentDirectoryA(1024, szLocalFontPath);
    lstrcatA(szLocalFontPath, "\\Fonts\\");
    lstrcatA(szLocalFontPath, szFontFileName );

    if( 0 != _access(szLocalFontPath, _A_NORMAL) )
    {   goto RETURN_CHECKFONT;  }

    if( 0 == AddFontResourceA( szLocalFontPath ) )
    {   goto RETURN_CHECKFONT;  }

    HINSTANCE hDLL = LoadLibraryW(L"GDI32.DLL");
    typedef BOOL (WINAPI *PGFRI)(LPCWSTR, DWORD *, LPVOID, DWORD);
    PGFRI GetFontResourceInfoW = (PGFRI)GetProcAddress(hDLL, "GetFontResourceInfoW");
    if( GetFontResourceInfoW )
    {
        WCHAR szwBuff[1024] = {0};
        _wsetlocale (LC_ALL, L"korean"); 
        mbstowcs( szwBuff, szLocalFontPath, strlen(szLocalFontPath) );

        DWORD dwSize = 1024;
        WCHAR szwFontName[1024];
        if( GetFontResourceInfoW(szwBuff, &dwSize, szwFontName, 1) )
        {
            bSuccess = true;
            char szWinFontPath[ 1024 ];
            GetWindowsDirectoryA(szWinFontPath, 1024);
            lstrcatA(szWinFontPath, "\\Fonts\\");
            lstrcatA(szWinFontPath, szFontFileName );
            if( 0 != _access(szWinFontPath, _A_NORMAL) )
            {
                bSuccess = InstallFont( szWinFontPath, szFontName );
            }

            if( bSuccess )
            {
                g_kGlobalValue.m_bFontInstallSuccess = true;
                g_kGlobalValue.m_strFontName = szFontName;
            }
        }
    }

    RemoveFontResourceA( szLocalFontPath );

    if( hDLL ) FreeLibrary( hDLL );

RETURN_CHECKFONT:
    return bSuccess;
}

void KD3DUIManager::CreateFonts()
{
    if(SiKGCMultipleLanguages()->UseMultipleLanguages() == true
#if defined(NATION_CHINA)
		|| true
#endif
		)
    {
        SiKGCMultipleLanguages()->CreateFont();
        m_pkFontMgr =g_pkFontMgr;
		m_pkFontMgrSmall = g_pkFontMgrSmall;
        m_pkFontMgrOutline =g_pkFontMgrOutline;
        m_pkFontMgr14 =g_pkFontMgr14;
        m_pkFontMgr16 =g_pkFontMgr16;
        m_pkFontMgrTitle =g_pkFontMgrTitle;
        m_pkFontMgrOutline14 =g_pkFontMgrOutline14;
        m_pkFontMgrOutline16 =g_pkFontMgrOutline16;
        m_pkFontMgrOutlineTitle =g_pkFontMgrOutlineTitle;
        return;
    }
    else
    {
#if defined ( _PORTUGUESE ) || defined( _SPANISH )
        SiKGCMultipleLanguages()->SetDefaultFontSize(12);
        SiKGCMultipleLanguages()->SetMiddleFontSize(14);
        SiKGCMultipleLanguages()->SetLargeFontSize(16);
        SiKGCMultipleLanguages()->SetTitleFontSize(26);
#elif defined( NATION_USA ) || defined( NATION_PHILIPPINE ) || defined( NATION_EUROPE ) || defined(NATION_THAILAND)
        SiKGCMultipleLanguages()->SetDefaultFontSize(13);
        SiKGCMultipleLanguages()->SetMiddleFontSize(14);
        SiKGCMultipleLanguages()->SetLargeFontSize(16);
        SiKGCMultipleLanguages()->SetTitleFontSize(26);
#else
        SiKGCMultipleLanguages()->SetDefaultFontSize(12);
        SiKGCMultipleLanguages()->SetMiddleFontSize(14);
        SiKGCMultipleLanguages()->SetLargeFontSize(16);
        SiKGCMultipleLanguages()->SetTitleFontSize(26);
		//SiKGCMultipleLanguages()->SetDefaultFontSize(12);
		//SiKGCMultipleLanguages()->SetMiddleFontSize(14);
		//SiKGCMultipleLanguages()->SetLargeFontSize(16);
		//SiKGCMultipleLanguages()->SetTitleFontSize(32);
#endif

    }

#if defined( _KOREAN )   // 한국어
    {
        char szFontName[MAX_PATH] = "GULIMCHE";

#if defined(FONT_INSTALL_FUNC)
        // 윈도우에 폰트가 설치되었는지 확인하고 인스톨하도록...
        strcpy(szFontName, "GULIMCHE");
        CheckInstallFont( "gulim.ttc", szFontName );
#endif
        // 폰트 퀄리티 설정
        //SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        //SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE,  FE_FONTSMOOTHINGCLEARTYPE,  0,  SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        //SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST,  2200,   0,  SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

        g_pkFontMgr_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize());
        g_pkFontMgrOutline_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize(), 1 );
        g_pkFontMgr14_original  = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize());
        g_pkFontMgr16_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize());
        g_pkFontMgrTitle_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize());
        g_pkFontMgrOutline14_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 1 );
        g_pkFontMgrOutline16_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 1 );
        g_pkFontMgrOutlineTitle_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 2 );

        float fWindowScaleY = g_pGCDeviceManager2->GetWindowScaleY();

        //  최소 0.8 비율 이하로는 줄이지 않는다.
        if ( fWindowScaleY < 0.84f ) 
            fWindowScaleY = 0.84f;

		g_pkFontMgr         = m_pkFontMgr         = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize(),0, true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrOutline  = m_pkFontMgrOutline  = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY );
		g_pkFontMgr14           = m_pkFontMgr14           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgr16           = m_pkFontMgr16           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgrTitle        = m_pkFontMgrTitle        = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgrOutline14    = m_pkFontMgrOutline14    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY);
		g_pkFontMgrOutline16    = m_pkFontMgrOutline16    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgrOutlineTitle = m_pkFontMgrOutlineTitle = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD,fWindowScaleY );
		g_pkFontMgr_mid_scale             = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutline_mid_scale      = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgr14_mid_scale           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgr16_mid_scale           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrTitle_mid_scale        = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutline14_mid_scale    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutline16_mid_scale    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutlineTitle_mid_scale = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
	}
#elif defined( _CHINESE_SIMPLIFIED )
	{
		char szFontName[MAX_PATH] = "FZZhunYuan-M02";

#if defined(FONT_INSTALL_FUNC)
		// 윈도우에 폰트가 설치되었는지 확인하고 인스톨하도록...
		strcpy(szFontName, "FZZhunYuan-M02");
		CheckInstallFont( "FZZhunYuan.ttc", szFontName );
#endif
		// 폰트 퀄리티 설정
		//SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		//SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE,  FE_FONTSMOOTHINGCLEARTYPE,  0,  SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		//SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST,  2200,   0,  SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

		g_pkFontMgr_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize());
		g_pkFontMgrOutline_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize(), 1 );
		g_pkFontMgr14_original  = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize());
		g_pkFontMgr16_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize());
		g_pkFontMgrTitle_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize());
		g_pkFontMgrOutline14_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 1 );
		g_pkFontMgrOutline16_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 1 );
		g_pkFontMgrOutlineTitle_original = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 2 );

		float fWindowScaleY = g_pGCDeviceManager2->GetWindowScaleY();

		//  최소 0.8 비율 이하로는 줄이지 않는다.
		if ( fWindowScaleY < 0.84f ) 
			fWindowScaleY = 0.84f;

		g_pkFontMgr         = m_pkFontMgr         = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize(),0, true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrOutline  = m_pkFontMgrOutline  = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY );
		g_pkFontMgr14           = m_pkFontMgr14           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgr16           = m_pkFontMgr16           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgrTitle        = m_pkFontMgrTitle        = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgrOutline14    = m_pkFontMgrOutline14    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY);
		g_pkFontMgrOutline16    = m_pkFontMgrOutline16    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD,fWindowScaleY);
		g_pkFontMgrOutlineTitle = m_pkFontMgrOutlineTitle = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD,fWindowScaleY );
		g_pkFontMgr_mid_scale             = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutline_mid_scale      = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgr14_mid_scale           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgr16_mid_scale           = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrTitle_mid_scale        = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutline14_mid_scale    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutline16_mid_scale    = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
		g_pkFontMgrOutlineTitle_mid_scale = g_pGCDeviceManager2->CreateFontA( szFontName, SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD,fWindowScaleY / (1024.0f / 768.0f) );
	}
#elif defined( _THAI_LANG )	//태국은 Tahoma체를 사용합니다.

    float fWindowScaleY = g_pGCDeviceManager2->GetWindowScaleY();

    //  최소 0.8 비율 이하로는 줄이지 않는다.
    if ( fWindowScaleY < 0.84f ) 
        fWindowScaleY = 0.84f;


    g_pkFontMgr_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize());
    g_pkFontMgrOutline_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize(), 1 );
    g_pkFontMgr14_original  = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize());
    g_pkFontMgr16_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize());
    g_pkFontMgrTitle_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize());
    g_pkFontMgrOutline14_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1 );
    g_pkFontMgrOutline16_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 );
    g_pkFontMgrOutlineTitle_original = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 );

	g_pkFontMgr         = m_pkFontMgr         = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY );
	g_pkFontMgrOutline  = m_pkFontMgrOutline  = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY );
	g_pkFontMgr14           = m_pkFontMgr14           = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD, fWindowScaleY );
	g_pkFontMgr16           = m_pkFontMgr16           = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD, fWindowScaleY );
	g_pkFontMgrTitle        = m_pkFontMgrTitle        = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD, fWindowScaleY );
	g_pkFontMgrOutline14    = m_pkFontMgrOutline14    = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY );
	g_pkFontMgrOutline16    = m_pkFontMgrOutline16    = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD, fWindowScaleY );
	g_pkFontMgrOutlineTitle = m_pkFontMgrOutlineTitle = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD, fWindowScaleY );

	g_pkFontMgr_mid_scale             = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD,  fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgrOutline_mid_scale      = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgr14_mid_scale           = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgr16_mid_scale           = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgrTitle_mid_scale        = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgrOutline14_mid_scale    = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgrOutline16_mid_scale    = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
	g_pkFontMgrOutlineTitle_mid_scale = g_pGCDeviceManager2->CreateFontTHA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );

#elif defined( _ENGLISH )	//싱가폴도 영어를 쓰지만 Ntreev에서 Tahoma 체를 원해서...
	{
        float fWindowScaleY = g_pGCDeviceManager2->GetWindowScaleY();

        //  최소 0.8 비율 이하로는 줄이지 않는다.
        if ( fWindowScaleY < 0.84f ) 
            fWindowScaleY = 0.84f;


		g_pkFontMgr_original		= g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize());
		g_pkFontMgrOutline_original = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize(), 1 );
		g_pkFontMgr14_original		= g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize());
		g_pkFontMgr16_original		= g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize());
		g_pkFontMgrTitle_original	= g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize());
		g_pkFontMgrOutline14_original = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1 );
		g_pkFontMgrOutline16_original = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 );
		g_pkFontMgrOutlineTitle_original = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 );

		g_pkFontMgr             = m_pkFontMgr             = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY );
		g_pkFontMgr14           = m_pkFontMgr14           = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize() ,0, true, FW_BOLD, fWindowScaleY );
		g_pkFontMgr16           = m_pkFontMgr16           = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrTitle        = m_pkFontMgrTitle        = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 0 ,true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrOutline      = m_pkFontMgrOutline      = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize(), 1,true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrOutline14    = m_pkFontMgrOutline14    = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrOutline16    = m_pkFontMgrOutline16    = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD, fWindowScaleY );
		g_pkFontMgrOutlineTitle = m_pkFontMgrOutlineTitle = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD, fWindowScaleY );

        g_pkFontMgr_mid_scale             = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutline_mid_scale      = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgr14_mid_scale           = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgr16_mid_scale           = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrTitle_mid_scale        = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutline14_mid_scale    = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutline16_mid_scale    = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutlineTitle_mid_scale = g_pGCDeviceManager2->CreateFontA( "tahoma", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );

	}
#else
    {
#if defined ( _PORTUGUESE ) || defined( _SPANISH )
        {
            // East Asia Language Support 가 비활성화 되어 있으면 MS Gothic 폰트가 없다.
            std::string strFileName = g_strCurrentPath + "msgothic.ttc";
            if( AddFontResourceW( GCUTIL_STR::GCStrCharToWide(strFileName.c_str()) ) == 0 )
            {
                MessageBoxA( NULL, "Font Loading Failed!", "Error", MB_OK );
            }
            //  
            //SendMessageA( HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL );
            //-> 이 함수는 메세지 프로시져가 메세지를 처리하기 전까지는 리턴하지 않는다.
            PostMessageA( HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL );
        }
#endif

        float fWindowScaleY = g_pGCDeviceManager2->GetWindowScaleY();

        //  최소 0.8 비율 이하로는 줄이지 않는다.
        if ( fWindowScaleY < 0.84f ) 
            fWindowScaleY = 0.84f;

        g_pkFontMgr_original = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetDefualtFontSize());
        g_pkFontMgrOutline_original = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetDefualtFontSize(), 1 );
        g_pkFontMgr14_original  = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetMiddleFontSize());
        g_pkFontMgr16_original = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetLargeFontSize());
        g_pkFontMgrTitle_original = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetTitleFontSize());
        g_pkFontMgrOutline14_original = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1 );
        g_pkFontMgrOutline16_original = g_pGCDeviceManager2->CreateFontA( "MS MS Gothic", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 );
        g_pkFontMgrOutlineTitle_original = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 );

        g_pkFontMgr         = m_pkFontMgr         = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY );
        g_pkFontMgrOutline  = m_pkFontMgrOutline  = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY );
        g_pkFontMgr14           = m_pkFontMgr14           = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD, fWindowScaleY );
        g_pkFontMgr16           = m_pkFontMgr16           = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD, fWindowScaleY );
        g_pkFontMgrTitle        = m_pkFontMgrTitle        = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD, fWindowScaleY );
        g_pkFontMgrOutline14    = m_pkFontMgrOutline14    = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY );
        g_pkFontMgrOutline16    = m_pkFontMgrOutline16    = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD, fWindowScaleY );
        g_pkFontMgrOutlineTitle = m_pkFontMgrOutlineTitle = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD, fWindowScaleY );
        g_pkFontMgr_mid_scale             = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetDefualtFontSize() ,0, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutline_mid_scale      = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetDefualtFontSize() ,1, true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgr14_mid_scale           = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetMiddleFontSize(), 0 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgr16_mid_scale           = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetLargeFontSize(), 0 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrTitle_mid_scale        = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetTitleFontSize(), 0,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutline14_mid_scale    = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutline16_mid_scale    = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
        g_pkFontMgrOutlineTitle_mid_scale = g_pGCDeviceManager2->CreateFontA( "MS Gothic", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 ,true, FW_BOLD, fWindowScaleY / (1024.0f / 768.0f) );
    }
#endif
    return;
}

int CreateUIWnd( lua_State* L )
{
    int iArg = lua_gettop(L);
    if( iArg < 2 )
    {
        lua_getfield(L, LUA_GLOBALSINDEX, "print" );
        lua_pushstring(L, "CreateUIWnd : You need more argument!!");
        lua_pcall(L, 1, 0, 0 );

        return 0;
    }

    KD3DWnd* pWnd = g_pkUIMgr->CreateUI( lua_tostring(L, -2) );
    if( pWnd == NULL )
    {
        lua_getfield(L, LUA_GLOBALSINDEX, "print" );
        lua_pushstring(L, "CreateUIWnd : UI type is not exist!!");
        lua_pcall(L, 1, 0, 0 );

        return 0;
    }
    pWnd->SetWindowNamePtr( lua_tostring(L, -1 ) );
    lua_tinker::ptr2lua<lua_tinker::base_type<KD3DWnd>::type>::invoke(L, pWnd );
    lua_tinker::meta_push( L, pWnd->GetClassName() );

    if( lua_istable(L,-1) == false )
    {
        lua_pop(L,2);
        return 0;
    }
    lua_setmetatable(L, -2);

    return 1;
}

int GetWndTable( lua_State* L ) {
    int iTop = lua_gettop(L);

    if( iTop < 2 ) {
        lua_pushstring( L, "there is no parameter!" );
        return 1;
    }

    if( lua_isnumber( L, -1 ) == false ) {
        lua_pushstring( L, "parameter is not wnd uid!" );
        return 1;
    }

    int iUID = lua_tointeger( L, -1 );

    KD3DWnd* pWnd = g_pkUIMgr->GetWndFromID( iUID );
    if( pWnd == NULL) {
        lua_pushstring( L, "there is no wnd with this UID!!!" );
        return 1;
    }

    if( pWnd->PushTable() == false ) {
        lua_pushstring( L, "wnd table pushing is failed!" );
        return 1;
    }

    int iNewTop = lua_gettop(L);

    for( int i = iTop + 1; i < iNewTop ; ++i ) {
        lua_remove( L, iTop + 1 );
    }

    return 1;
}

KD3DUIManager::KD3DUIManager( void )
{
	g_pkUIMgr       = this;
	g_hUIMainWnd    = NULL;
    m_bCursorOnUI   = false;

	//98에는 휴먼옛체가 없는 관계로 배포합니다.
	/*std::string strFileName = g_strCurrentPath + "HMFMOLD.TTF";
	if( AddFontResourceW( GCUTIL_STR::GCStrCharToWide(strFileName.c_str()) ) == 0 )
	MessageBoxA( NULL, "Font Loading Failed!", "Error", MB_OK );
	SendMessageA( HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL );*/	

    CreateFonts();

    m_apDeviceTexture = NULL;


	m_vecStrTexFileName.clear();

	m_mapScene.clear();
	m_strCurrScene.erase();
	m_strNextScene.erase();

	m_mapWndFactory.clear();
	m_pActionListener = NULL;

    m_vecUIWndClass.clear();
	// register predefined scene factory objects
	RegisterSceneFactory( "default", &g_kSceneFactory );

    // register predefined window factory objects
    RegisterWindowFactory( "window",    &g_kWndFactory );
    RegisterWindowFactory( "frame",     &g_kFrameWndFactory );
    RegisterWindowFactory( "edit",      &g_kEditFactory );
    RegisterWindowFactory( "static",    &g_kStaticFactory );
	RegisterWindowFactory( "imageicon", &g_kImageIconFactory );
    RegisterWindowFactory( "listbox",   &g_kListBoxFactory );
    RegisterWindowFactory( "scrollbar", &g_kScrollbarFactory );
    RegisterWindowFactory( "tab",       &g_kTabFactory );
    RegisterWindowFactory( "combobox",  &g_kComboBoxFactory );
    RegisterWindowFactory( "checkbox",   &g_kCheckBoxFactory );
    RegisterWindowFactory( "staticlink", &g_kStaticLinkFactory );
    RegisterWindowFactory( "staticbtn",	&g_kStaticBtnFactory );
    RegisterWindowFactory( "sizingboxbtn", &g_kSizingBoxBtnFactory );
	RegisterWindowFactory( "movewnd",      &g_kMoveWndFactory );

	// register predefined spline objects
	RegisterSpline( "flat",             SPLINE_FLAT );
	RegisterSpline( "linear",           SPLINE_LINEAR );
	RegisterSpline( "exponential",      SPLINE_EXPONENTIAL );
	RegisterSpline( "logarithm",        SPLINE_LOGARITHM );
	RegisterSpline( "sine_linear",      SPLINE_SINE_LINEAR );
	RegisterSpline( "sine_exponential", SPLINE_SINE_EXPONENTIAL );
	RegisterSpline( "sine_logarithm",   SPLINE_SINE_LOGARITHM );

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    lua_tinker::class_add<KD3DUIManager>( L, "KD3DUIManagerClass" );
    lua_tinker::class_def<KD3DUIManager>( L, "GetMousePos",                 &KD3DUIManager::GetMousePos );
    lua_tinker::class_def<KD3DUIManager>( L, "CreateTemplateUIByFindName",  &KD3DUIManager::CreateTemplateUIByFindName );

    lua_tinker::decl( L, "UIMgr", this );

    lua_tinker::meta_push( L, "KD3DUIManagerClass" );
    lua_pushcclosure( L, CreateUIWnd, 0 );
    lua_setfield( L, -2, "CreateUIWnd");
    lua_pop(L, 1);

    lua_tinker::meta_push( L, "KD3DUIManagerClass" );
    lua_pushcclosure( L, GetWndTable, 0 );
    lua_setfield( L, -2, "GetWndTable");
    lua_pop(L, 1);

    lua_tinker::class_add<KInput>( L, "KInputClass" );
    lua_tinker::decl( L, "KInput", &m_kInput );

    lua_tinker::class_add<KActionEvent>( L, "KActionEvent" );
    lua_tinker::class_mem<KActionEvent>( L, "wnd", &KActionEvent::m_pWnd );
    lua_tinker::class_mem<KActionEvent>( L, "code", &KActionEvent::m_dwCode );
    lua_tinker::class_mem<KActionEvent>( L, "param", &KActionEvent::m_dwlParam );
    lua_tinker::class_mem<KActionEvent>( L, "param2", &KActionEvent::m_dwlParam2 );
    lua_tinker::class_con<KActionEvent>( L, lua_tinker::constructor<>() );

    m_tmpTexture=NULL;
}

KD3DUIManager::~KD3DUIManager( void )
{

	g_pkUIMgr    = NULL;
	g_hUIMainWnd = NULL;


	SAFE_RELEASE( m_pkFontMgr );
	SAFE_RELEASE( m_pkFontMgr16 );
	SAFE_RELEASE( m_pkFontMgrTitle );
	SAFE_RELEASE( m_pkFontMgrOutline );

	ClearTexture();

    SAFE_DELETE_ARRAY( m_apDeviceTexture );
	m_vecStrTexFileName.clear();

	ClearUIScene();
	m_strCurrScene.erase();
	m_strNextScene.erase();

	m_mapWndFactory.clear();
	m_pActionListener = NULL;
}
//void KD3DUIManager::ResetDeviceFont()
//{
//#if defined( _KOREAN ) || defined( _CHINESE_SIMPLIFIED )   // 한국어 (한국)와 중국
//
//	//g_MyD3D->Resize3DEnvironment();에서 알아서 정리해 주시는듯
//	//SAFE_RELEASE(m_pkFontMgr);
//	//SAFE_RELEASE(m_pkFontMgrOutline);
//
//	//SAFE_RELEASE(m_pkFontMgr14);
//	//SAFE_RELEASE(m_pkFontMgr16);
//	//SAFE_RELEASE(m_pkFontMgrTitle);
//	//SAFE_RELEASE(m_pkFontMgrOutline14);
//	//SAFE_RELEASE(m_pkFontMgrOutline16);
//	//SAFE_RELEASE(m_pkFontMgrOutlineTitle);
//
//	g_pkFontMgr         = m_pkFontMgr         = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetDefualtFontSize() );
//	g_pkFontMgrOutline  = m_pkFontMgrOutline  = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetDefualtFontSize(), 1 );
//
//	g_pkFontMgr14           = m_pkFontMgr14           = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetMiddleFontSize() );
//	g_pkFontMgr16           = m_pkFontMgr16           = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetLargeFontSize() );
//	g_pkFontMgrTitle        = m_pkFontMgrTitle        = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetTitleFontSize() );
//	g_pkFontMgrOutline14    = m_pkFontMgrOutline14    = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetMiddleFontSize(), 1 );
//	g_pkFontMgrOutline16    = m_pkFontMgrOutline16    = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetLargeFontSize(), 1 );
//	g_pkFontMgrOutlineTitle = m_pkFontMgrOutlineTitle = g_pGCDeviceManager2->CreateFontA( "GULIMCHE", SiKGCMultipleLanguages()->GetTitleFontSize(), 2 );
//		
//	
//#endif
//}
void KD3DUIManager::MoveUIScene( const std::string& strSceneName )
{
	if ( m_strCurrScene == strSceneName || m_strNextScene == strSceneName ) return;

	if ( m_mapScene.end() != m_mapScene.find( strSceneName ) )
	{
		if ( m_strCurrScene.empty() )
		{
			m_strCurrScene = strSceneName;
			m_strNextScene = strSceneName;
			m_mapScene[m_strCurrScene]->EnterUIScene();
		}
		else
		{
			m_strNextScene = strSceneName;
			m_mapScene[m_strCurrScene]->LeaveUIScene();
		}
	}
}

HRESULT KD3DUIManager::FrameMove( float fTime )
{
    PROFILE_SET("KD3DUIManager::FrameMove");

	g_fUITime = fTime;
    m_bCursorOnUI = false;
	m_kInput.UpdateInput();

    std::vector<BYTE> vecKeys = m_kInput.GetKeyList( &KInput::IsDown );
    BOOST_FOREACH( BYTE key, vecKeys ) {
        SendEvent( KActionEvent( NULL, KD3DWnd::D3DWE_KEY_DOWN, key ) );
    }

	if ( !m_strCurrScene.empty() )
	{
		m_mapScene[m_strCurrScene]->FrameMove();
	}

	if ( m_strCurrScene != m_strNextScene )
	{
		if ( KD3DUIScene::D3DUIS_DISABLED == m_mapScene[m_strCurrScene]->GetUISceneState() )
		{
			if ( !( m_strCurrScene = m_strNextScene ).empty() )
			{
				m_mapScene[m_strCurrScene]->EnterUIScene();
			}
		}
	}

    // 텍스쳐 사용 카운트를 업데이트 하자
    for(std::set<GCUITextureCache*>::iterator i = m_setUITextureSection.begin();
        i!=m_setUITextureSection.end();i++)
    {
        (*i)->Update();
    }
	return S_OK;
}

HRESULT KD3DUIManager::Render( void )
{
	if ( !m_strCurrScene.empty() )
	{
        g_pGCDeviceManager2->PushState();
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );

		m_mapScene[m_strCurrScene]->Render();
		g_pGCDeviceManager2->PopState();
	}

#if !defined(__PATH__)
    if( g_kGlobalValue.bRenderTempTexture && g_kGlobalValue.m_pTempTex )
    {
        int iWidth = g_kGlobalValue.m_pTempTex->GetWidth();
        int iHeight = g_kGlobalValue.m_pTempTex->GetHeight();
        g_pGCDeviceManager2->DrawInScreenResize(g_kGlobalValue.m_pTempTex,
            g_kGlobalValue.vTempTexRenderPos.x, g_kGlobalValue.vTempTexRenderPos.y,
            g_kGlobalValue.vTempTexRenderPos.x+iWidth, g_kGlobalValue.vTempTexRenderPos.y+iHeight,
            0.0f,0.0f,1.0f,1.0f );
    }
#endif

	return S_OK;
}

HRESULT KD3DUIManager::OneTimeSceneInit( HWND hWnd, const std::string& strScriptFileName )
{
	g_hUIMainWnd = hWnd;
	KLuaManager kLuaManager;

	return LoadUIScript( &kLuaManager, strScriptFileName );
}

HRESULT KD3DUIManager::InitDeviceObjects()
{
	m_kInput.InitDevice( g_hUIMainWnd, TRUE );
	m_kInput.ShowCursor( TRUE );

    for ( UINT i = 0; i < m_vecStrTexFileName.size(); ++i )
    {
//         if(i==3 || i==4 || i == 69 )
//             m_apDeviceTexture[i] = g_pGCDeviceManager2->CreateTexture( m_vecStrTexFileName[i] );
//         else
            m_apDeviceTexture[i] = g_pGCDeviceManager2->CreateTextureWithNoLoad( m_vecStrTexFileName[i] );
    }

	return S_OK;
}

HRESULT KD3DUIManager::DeleteDeviceObjects( void )
{

	g_pd3dDevice = NULL;
	ClearTexture();

	return S_OK;
}


HRESULT KD3DUIManager::FinalCleanup( void )
{
	g_hUIMainWnd = NULL;

	m_vecStrTexFileName.clear();

	ClearUIScene();

	return S_OK;
}

void KD3DUIManager::ClearTexture( void )
{
    SAFE_RELEASE(m_tmpTexture);


    for(std::set<GCUITextureCache*>::iterator i = m_setUITextureSection.begin();
        i!=m_setUITextureSection.end();i++)
    {
		SAFE_DELETE(const_cast<GCUITextureCache *> (*i) );
    }
    m_setUITextureSection.clear();

    while(!m_listBuildTexture.empty())
    {
        SAFE_DELETE( m_listBuildTexture.back() );
        m_listBuildTexture.pop_back();
    }

    if( !m_apDeviceTexture ) return;

    for ( int i = 0; i < (int)m_vecStrTexFileName.size(); i++ )
    {
        if( m_apDeviceTexture[i] )
            m_apDeviceTexture[i]->Release();
    }

}


void KD3DUIManager::ClearUnusedTexture(int i)
{
    // 잘 안쓰는 놈들 지우자
    std::set<GCUITextureCache*>::iterator it = m_setUITextureSection.begin();
    std::list<GCUITextureCache*> deletelist;
    while(it!=m_setUITextureSection.end())
	{
        if((*it)->IsNotUse(i))
		{
            (*it)->Clear();
            delete (*it);
            deletelist.push_back(*it);
        }
        it++;
    }

    while(!deletelist.empty())
    {
        m_setUITextureSection.erase(deletelist.back());
        deletelist.pop_back();
    }
}


void KD3DUIManager::ClearUIScene( void )
{
	std::map<std::string, KD3DUIScene*>::iterator i;
	for ( i = m_mapScene.begin(); i != m_mapScene.end(); i++ )
	{
		i->second->RemoveAllWindow();
		SAFE_DELETE( i->second );
	}
	m_mapScene.clear();
}

void KD3DUIManager::ReloadNumTable( const char* strScriptFileName )
{
    KLuaManager luaMgr;
    GCFUNC::LoadLuaScript( luaMgr, strScriptFileName );
    LoadNumTable( &luaMgr );
}

HRESULT KD3DUIManager::LoadLuaGlobal( KLuaManager* pkLuaMgr )
{
    HRESULT hr;
    if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( "global" ) ) )
    {
        for ( UINT i = 1; ; ++i )
        {
            std::string strUiScriptFile;
            if ( SUCCEEDED( hr = pkLuaMgr->GetValue( i, strUiScriptFile ) ) )
            {
                if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strUiScriptFile ) == false )
                    return E_FAIL;
            }
            else break; // no more data
        }

        if ( FAILED( hr = pkLuaMgr->EndTable() ) )
            return hr; // invalid file format
    }

    return hr;
}

HRESULT KD3DUIManager::LoadLuaClass( KLuaManager* pkLuaMgr )
{
    HRESULT hr;
    // read scene class tables
    if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( "class" ) ) )
    {
        for ( UINT i = 1; ; ++i )
        {
            std::string strUiScriptFile;
            if ( SUCCEEDED( hr = pkLuaMgr->GetValue( i, strUiScriptFile ) ) )
            {
                if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), strUiScriptFile ) == false )
                    return E_FAIL;
            }
            else break; // no more data
        }

        if ( FAILED( hr = pkLuaMgr->EndTable() ) )
            return hr; // invalid file format
    }

    return hr;
}

HRESULT KD3DUIManager::LoadUIScript( KLuaManager* pkLuaMgr, const std::string& strScriptFileName )
{
	HRESULT hr;
    KLuaManager& luaMgr = *pkLuaMgr;

    std::string strSubScript;

	// read script file
    GCFUNC::LoadLuaScript( *pkLuaMgr, strScriptFileName );

    LoadTextureTable( pkLuaMgr );
    LoadNumTable( pkLuaMgr );
    LoadLuaGlobal( pkLuaMgr );

	// read scene tables
	// 우선 ㅡ ui 파일 목록을 가져온다.
	std::vector<std::string> vecUIFileName;
	if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( "scene" ) ) )
	{
		for ( UINT i = 1; ; ++i )
		{
			std::string strUiScriptFile;
			if ( SUCCEEDED( hr = pkLuaMgr->GetValue( i, strUiScriptFile ) ) )
			{
				vecUIFileName.push_back( strUiScriptFile );
			}
			else break; // no more data
		}

		if ( FAILED( hr = pkLuaMgr->EndTable() ) )
			return hr; // invalid file format
	}


	std::string strSceneName;
	std::string strSceneType;

	strSceneName = "main";
	strSceneType = "gc_scene";

	KD3DUIScene* pkScene = m_mapSceneFactory[strSceneType]->Create( strSceneName );

#if defined(BINARY_LOAD)
    if ( FAILED( hr = LoadUIWindowTable("UIBinary.dat", pkScene  ) ))
        return hr;
#else
	// read windows

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_pushstring( L, "scene_part" );
    lua_newtable(L);
    lua_settable( L, LUA_GLOBALSINDEX );
    lua_getglobal( L, "scene_part" );
    lua_pushstring( L, "top_scene" );
    lua_setfield( L, -2, "_uitype" );
    lua_pushstring( L, "_class");
    lua_tinker::push( L, pkScene );
    lua_settable( L, -3 );

    for ( unsigned int i = 0; i < (unsigned int)vecUIFileName.size() ; ++i )
    {
        KLuaManager LuaMgr;
        GCFUNC::LoadLuaScript( LuaMgr, strScriptFileName );
        GCFUNC::LoadLuaScript( LuaMgr, vecUIFileName[i] );
        
        if ( SUCCEEDED( hr = LuaMgr.BeginTable( "scene_part" ) ) )
        {
            if ( FAILED( hr = LoadUISceneTable( &LuaMgr, pkScene ,vecUIFileName[i]) ) )
                return hr;
        }
    }

    lua_pop( L, -1 );

    //WriteUIData_Binary();
#endif
    
    if( FAILED( LoadLuaClass( pkLuaMgr ) ) )
        return E_FAIL;

	RegisterUIScene( strSceneName, pkScene ); // register scene
	return S_OK;
}

HRESULT KD3DUIManager::LoadTextureTable( KLuaManager* pkLuaMgr )
{
    // read texture tables
    HRESULT hr = S_OK;
    m_vecStrTexFileName.clear();

	bool bSwap = false;
	if (g_kGlobalValue.IsExceptMotion(-200)==true)
		bSwap=true;

    if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( "texture" ) ) )
    {
        for ( UINT i = 1; ; ++i )
        {
            std::string strTexFileName;
            if ( SUCCEEDED( hr = pkLuaMgr->GetValue( i, strTexFileName ) ) )
            {

				if (bSwap==true)
				{
					if (strTexFileName=="square_load1.dds")
					{
						strTexFileName="square_load1_2.dds";
					}

					if (strTexFileName=="square_load2.dds")
					{
						strTexFileName="square_load2_2.dds";
					}
				}
                m_vecStrTexFileName.push_back( strTexFileName );
            }
            else break; // no more data
        }

        if ( FAILED( hr = pkLuaMgr->EndTable() ) )
            return hr; // invalid file format
    }

    // allocate memory for texture interface pointer array
    SAFE_DELETE_ARRAY( m_apDeviceTexture );    
    m_apDeviceTexture = new GCDeviceTexture*[m_vecStrTexFileName.size()];
    ::ZeroMemory( m_apDeviceTexture, m_vecStrTexFileName.size() * sizeof( GCDeviceTexture* ) );

    return hr;
}

HRESULT KD3DUIManager::LoadNumTable( KLuaManager* pkLuaMgr )
{

    HRESULT hr = S_OK;


	std::string strNumFileName;
	if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( "number" ) ) )
	{
		if ( SUCCEEDED( hr = pkLuaMgr->GetValue( 1, strNumFileName ) ) )
		{

		}

		if ( FAILED( hr = pkLuaMgr->EndTable() ) )
			return hr; // invalid file format
	}

	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, strNumFileName );

	// read number info
	if ( SUCCEEDED( hr = luaMgr.BeginTable( "number" ) ) )
	{
		for ( UINT i = 1; ; ++i )
		{
			if ( SUCCEEDED( hr = luaMgr.BeginTable( i ) ) )
			{
				std::string     strNumType;
				KD3DUINumInfo   kNumInfo;

				if ( FAILED( hr = luaMgr.GetValue( "type", strNumType ) ) )           return hr;
				if ( FAILED( hr = luaMgr.GetValue( "width", kNumInfo.dwWidth ) ) )    return hr;
				if ( FAILED( hr = luaMgr.GetValue( "height", kNumInfo.dwHeight ) ) )  return hr;
				if ( FAILED( hr = luaMgr.GetValue( "tex_idx", kNumInfo.iTexIdx ) ) )  return hr;
				if ( FAILED( hr = luaMgr.GetValue( "tu1", kNumInfo.vTexCoords.x ) ) ) return hr;
				if ( FAILED( hr = luaMgr.GetValue( "tv1", kNumInfo.vTexCoords.y ) ) ) return hr;
				if ( FAILED( hr = luaMgr.GetValue( "tu2", kNumInfo.vTexCoords.z ) ) ) return hr;
				if ( FAILED( hr = luaMgr.GetValue( "tv2", kNumInfo.vTexCoords.w ) ) ) return hr;
				if ( FAILED( hr = luaMgr.GetValue( "offset", kNumInfo.fOffset ) ) )   return hr;

				float fMagicNumber = 0.5f;
				if (kNumInfo.vTexCoords.x > kNumInfo.vTexCoords.z)
				{
					fMagicNumber *=-1;					
				}
				kNumInfo.vTexCoords.x+= fMagicNumber / 512.0f;
				kNumInfo.vTexCoords.z-= fMagicNumber / 512.0f;


				fMagicNumber = 0.5f;
				if (kNumInfo.vTexCoords.y > kNumInfo.vTexCoords.w)
				{
					fMagicNumber *=-1;					
				}
				kNumInfo.vTexCoords.y+= fMagicNumber / 512.0f;
				kNumInfo.vTexCoords.w-= fMagicNumber / 512.0f;

				m_mapNumInfo[strNumType] = kNumInfo;

				if ( FAILED( hr = luaMgr.EndTable() ) )
					return hr; // invalid file format
			}
			else break; // no more table
		}

		if ( FAILED( hr = luaMgr.EndTable() ) )
			return hr; // invalid file format
	}




    return hr;
}

HRESULT KD3DUIManager::LoadUISceneTable( KLuaManager* pkLuaMgr, KD3DUIScene* pkUIScene ,std::string strFileName)
{
	HRESULT hr = S_OK;
    KLuaManager& luaMgr = *pkLuaMgr;

	// TODO : Read scene values

	// read window tables
	for ( UINT i = 1; ; ++i )
	{
		if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( i ) ) )
		{
			std::string strWndType;
            LUA_GET_VALUE( "type", strWndType, return E_FAIL );


            //////////////////////////////////////////////////////////////////////////
            // check unregistered window type
//             if ( m_mapWndFactory.end() == m_mapWndFactory.find( strWndType ) )
//             {
//                 ASSERT_MBOX( "No Factory : " << strWndType );
//                 strWndType = "window";
//                 //return E_FAIL;
//             }
// 
// 
//             if(m_mapWndFactory[strWndType]->CanCreate())
//             {
//                 KD3DWnd* pkWnd = m_mapWndFactory[strWndType]->Create();
// 
//                 if ( SUCCEEDED( hr = LoadUIWindowTable_FromScript( pkLuaMgr,  pkWnd ) ) )			
//                 {
//                     if(!m_mapWndFactory[strWndType]->IsStatic())
//                     {
//                         pkUIScene->AddWindow( pkWnd );
//                     }
//                     else
//                         return hr; // invalid file format
//                 }
//             }
            //////////////////////////////////////////////////////////////////////////

            // check unregistered window type
            KD3DWnd* pkWnd = this->CreateWndFromFactory( strWndType );
            if( pkWnd != NULL ) {
                if ( SUCCEEDED( hr = LoadUIWindowTable_FromScript( pkLuaMgr,  pkWnd ) ) ) {
                    pkWnd->SetFileName( strFileName );
                    if( !m_mapWndFactory[strWndType]->IsStatic() ) {
                        pkUIScene->AddWindow( pkWnd );
                    }
                    else {
                        return hr; // invalid file format
                    }
                }
            }
            else { // 기본 윈도우 조차 없는 경우
                return E_FAIL;
            }


			if ( FAILED( hr = pkLuaMgr->EndTable() ) )
                return hr; // invalid file format
		}
		else break; // no more table
	}

	return S_OK;
}

HRESULT KD3DUIManager::LoadUIWindowTable(std::string strFileName, KD3DUIScene* pkUIScene)
{
	bool    static_instance = false;
	KD3DWndClass kWndClass;
	

    //	//ToolTip , TopMost, Hotkey, HotkeyStyle ,PixelValue, Motion, Alpha, Friend 사용안해서 지웁니다.
    // 
    ReadUIData_Binary(strFileName);

#define MAX_DEEPS	100  // UI가 아무리 복잡해도 100번 넘게 파고들겠음?
	KSafeArray<KD3DWnd*,MAX_DEEPS> pkWndDeepsTabel;
	pkWndDeepsTabel[0]= NULL;  // Root? = NULL?

    for ( unsigned int i=0;i<m_vecUIWndClass.size();i++)
    {
        kWndClass=m_vecUIWndClass[i];

        if (kWndClass.iDeep==0)
        {
            //한 Scene을 모두 돌았으면, UIScene에 등록한다.
            if (i!=0)
            {
                pkUIScene->AddWindow( pkWndDeepsTabel[0] );
            }
        }

        KD3DWnd* pkChildWnd = m_mapWndFactory[kWndClass.strType]->Create();
        // set auto create flag
        pkChildWnd->SetAutoCreate( kWndClass.bAutoCreate );

        //// set predraw flag
        pkChildWnd->SetPredraw( kWndClass.bPredraw );

        ////visible
        pkChildWnd->ToggleRender(kWndClass.bRender);

#ifdef _DEBUG
        if (kWndClass.iDeep>=MAX_DEEPS)
            ASSERT("도데체 UI가 얼마나 복잡하기에 여기에 잡힘?");
#endif

		// Register하면서 부모의 포인터 값을 참고 하여 AddChild한다.
		// But, 파일에서 읽어들인 부모포인터가 멀쩡할리가 없다.
		// 부모 포인터를 iDeep값을 기반으로 새로 셋팅 해준다.
        // Register하면서 부모의 포인터 값을 참고 하여 AddChild한다.
        // But, 파일에서 읽어들인 부모포인터가 멀쩡할리가 없다.
        // 부모 포인터를 iDeep값을 기반으로 새로 셋팅 해준다.
        if (kWndClass.iDeep==0)
        {
            kWndClass.pParent = NULL;
        }
        else
        {
            kWndClass.pParent = pkWndDeepsTabel[kWndClass.iDeep-1];
        }


        // 자기 포인터 위치 기록함.
        pkWndDeepsTabel[kWndClass.iDeep] = pkChildWnd;


        // register window information
        pkChildWnd->Register( kWndClass );

        // Recursive 돌면서 기록된 자료를 Base로 쓰기 때문에,
        // 기록된 값이 부모를 등록할수 없는 상황이 오지는 않는다.
    }

    //다돌았으면 마지막 Scene도 등록
    pkUIScene->AddWindow( pkWndDeepsTabel[0] );


    return S_OK;
}

//동적 생성하는 것들 때문에-_-; 출력용에 포함되면 안되는 것들 안 출력하구 읽는 원본 루틴
HRESULT KD3DUIManager::LoadUIWindowTable_FromScriptOriginal( KLuaManager* pkLuaMgr, KD3DWnd* pkWnd, KD3DWnd* pkParent , std::string strWndName)
{
    ASSERT( pkLuaMgr != NULL );

    HRESULT hr;

    pkWnd->LoadWnd( pkLuaMgr, pkParent, strWndName );

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    pkWnd->RegistToLua();
    pkWnd->OnLoad();

    // read child window tables
    for ( UINT i = 1; ; ++i )
    {
        if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( i ) ) )
        {
            std::string strWndType;
            if ( SUCCEEDED( hr = pkLuaMgr->GetValue( "type", strWndType ) ) )
            {

                KD3DWnd* pkChildWnd = this->CreateWndFromFactory( strWndType );
                if( pkChildWnd != NULL ) {

                    if(!m_mapWndFactory[strWndType]->IsStatic())
                    {
                        if ( FAILED( hr = LoadUIWindowTable_FromScriptOriginal( pkLuaMgr, pkChildWnd, pkWnd ) ) )
                            return hr;
                    }
                    else
                    {
                        pkWnd->AddChild(pkChildWnd);
                    }
                }
                else {
                    return E_FAIL;
                }
            }
            else
                return hr; // invalid file format

            if ( FAILED( hr = pkLuaMgr->EndTable() ) )
                return hr; // invalid file format
        }
        else break; // no more table
    }

    lua_pop( L, 1 );

    return S_OK;

}

// Script 로드님
HRESULT KD3DUIManager::LoadUIWindowTable_FromScript( KLuaManager* pkLuaMgr, KD3DWnd* pkWnd, KD3DWnd* pkParent , std::string strWndName)
{
    ASSERT( pkLuaMgr != NULL );

    HRESULT hr;

    // register window information
    pkWnd->LoadWnd( pkLuaMgr, pkParent, strWndName );

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    pkWnd->RegistToLua();
    pkWnd->OnLoad();

    // read child window tables
    for ( UINT i = 1; ; ++i )
    {
        if ( SUCCEEDED( hr = pkLuaMgr->BeginTable( i ) ) )
		{
            std::string strWndType;
            if ( SUCCEEDED( hr = pkLuaMgr->GetValue( "type", strWndType ) ) )
            {
                // check unregistered window type
                KD3DWnd* pkChildWnd = this->CreateWndFromFactory( strWndType );
                if( pkChildWnd != NULL ) {

                    if(!m_mapWndFactory[strWndType]->IsStatic())
                    {
                        if ( FAILED( hr = LoadUIWindowTable_FromScript( pkLuaMgr, pkChildWnd, pkWnd ) ) )
                            return hr;
                    }
                    else
                    {
                        pkWnd->AddChild(pkChildWnd);
                    }
                }
                else {
                    return E_FAIL;
                }
            }
            else
                return hr; // invalid file format

            if ( FAILED( hr = pkLuaMgr->EndTable() ) )
                return hr; // invalid file format
        }
        else break; // no more table
    }

    lua_pop( L, 1 );

    return S_OK;
}

void KD3DUIManager::ReadUIData_Binary(std::string strUIFileName)
{
    KD3DWndClass kD3dWnd;

    GCMemoryFile MemFile (g_pGCDeviceManager2->GetMassFileManager(), strUIFileName );

    if( !MemFile.IsLoaded())
    {
        ASSERT("UI 파일이 없다고!?!?!");
        return ;
    }

    char* m_pUIFile = MemFile.GetDataPointer();

    DWORD dwTotalUI = NULL; //전체 UI수
    int iIndex = 0;

    //dwTotalUI=30000;

    memcpy( &dwTotalUI, m_pUIFile, sizeof(DWORD) );
    iIndex += sizeof(DWORD);
    if( dwTotalUI == NULL )
    {
        ASSERT("UI 갯수가 0개일리가 없지않음?");
        return ;
    }
    m_vecUIWndClass.clear();
    m_vecUIWndClass.reserve(dwTotalUI);

    //memcpy( &m_vecUIWndClass[0], m_pUIFile , sizeof( KD3DWndClass )*dwTotalUI );
    for(DWORD i = 0; i < dwTotalUI; i++ )
    {
        memcpy( &kD3dWnd, m_pUIFile + iIndex, sizeof( KD3DWndClass ) );
        m_vecUIWndClass.push_back(kD3dWnd);
        iIndex += sizeof( KD3DWndClass );
#ifdef _DEBUG
        if (iIndex > MemFile.GetFileSize())
        {
            ASSERT("뭔가 파일 크기 보다 많은 데이터를 읽으려 한다.");
        }
#endif
    }

}

void KD3DUIManager::WriteUIData_Binary()
{
    KD3DWndClass kD3dWnd;
    DWORD dwTotalUI = m_vecUIWndClass.size();

    FILE *fp;
    fp = fopen("UIBinary.dat","wb"); 

    fwrite(&dwTotalUI, sizeof(DWORD), 1 , fp);
    for (int i=0;i<(int)dwTotalUI;i++)
    {
        kD3dWnd = m_vecUIWndClass[i];
        if (fwrite(&kD3dWnd, sizeof(KD3DWndClass),1 , fp)!=1)
        {
            ASSERT("파일 쓰기 실패?");
        } 
    }
    m_vecUIWndClass.clear();
    fclose(fp); 
    return;

}

void KD3DUIManager::RenderNumber( const std::string& strType,
                                 const D3DXVECTOR2& vPos,
                                 const char* strChar,
                                 D3DXCOLOR argb,
                                 bool bWriteToRightSide /* = true */,           
                                 bool bShadow /* = true */,
                                 float fInterval, /*= -1.0f*/
                                 bool bScale,// = true
								 bool bNotFrontZero, /*= false*/
								 float fScale /*=1.0f*/)
{
    std::map<std::string, KD3DUINumInfo>::iterator itor = m_mapNumInfo.find(strType);

    if( itor == m_mapNumInfo.end() )
        return;

    KD3DUINumInfo &kNumInfo = itor->second;

    if ( -1 == kNumInfo.iTexIdx )
        return;

    int iSize = (int)strlen(strChar);
    float fTemp = 0.0f;
    float fScaleX = 1.0f;
    float fScaleY = 1.0f;

	if (bScale==true)
	{
		fScaleX=g_pGCDeviceManager->GetWindowScaleX() * fScale;
		fScaleY=g_pGCDeviceManager->GetWindowScaleY() * fScale;
	}	
	else
	{
		fScaleX = fScale;
		fScaleY = fScale;
	}

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    for ( int i = 0; i < iSize; ++i )
    {
		// 앞자리에 0이 올 수 없는 상황에서 2자리이상인 수의 첫 자리가 0이라면 0을 그리지 않겠다.
		if( iSize != 1 && strChar[0] == '0' && bNotFrontZero )
			continue;

        DWORD num = (DWORD)(strChar[i] - '0');
        //원래 num >9 까지만 체크해야 되는데.. +와 ,를 사용하는 곳이 있어서.. 우선 수정 해둡니다.
        if(num<0 || num>11)
            num=0;

        if( bWriteToRightSide )
        {
            fTemp = (fInterval==-1?(float)kNumInfo.dwWidth:fInterval) * i;
        }
        else
        {
            fTemp = (fInterval==-1?(float)kNumInfo.dwWidth:fInterval) * ( (i + 1 ) - iSize);
        }
        SETVECTOR4( m_rectNumRender[0].m_vPos, vPos.x + (fTemp)*fScaleX, vPos.y, 0.0f, 1.0f );
        SETVECTOR4( m_rectNumRender[1].m_vPos, vPos.x + (fTemp + kNumInfo.dwWidth)*fScaleX, vPos.y, 0.0f, 1.0f );
        SETVECTOR4( m_rectNumRender[2].m_vPos, vPos.x + fTemp*fScaleX, vPos.y + kNumInfo.dwHeight*fScaleY, 0.0f, 1.0f );
        SETVECTOR4( m_rectNumRender[3].m_vPos, vPos.x + (fTemp + kNumInfo.dwWidth)*fScaleX, vPos.y + kNumInfo.dwHeight*fScaleY, 0.0f, 1.0f );

  //  	KLuaManager luaMgr;
  //  	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

		//float offsetx = 0.0f;
		//float offsety = 0.0f;
		//LUA_GET_VALUE_DEF( "offsetx", offsetx, 0.003f );
		//LUA_GET_VALUE_DEF( "offsety", offsety, 0.003f );

        fTemp = kNumInfo.fOffset * num;
        //fTemp = offset * num;
        //원래 num >9 까지만 체크해야 되는데.. +와 ,를 사용하는 곳이 있어서.. 우선 수정 해둡니다.
        if(num<0 || num>11)
            num=0;
		
        SETVECTOR2( m_rectNumRender[0].m_vTex, kNumInfo.vTexCoords.x + fTemp, kNumInfo.vTexCoords.y - 0.002f);
        SETVECTOR2( m_rectNumRender[1].m_vTex, kNumInfo.vTexCoords.z + fTemp, kNumInfo.vTexCoords.y - 0.002f);
        SETVECTOR2( m_rectNumRender[2].m_vTex, kNumInfo.vTexCoords.x + fTemp, kNumInfo.vTexCoords.w );
        SETVECTOR2( m_rectNumRender[3].m_vTex, kNumInfo.vTexCoords.z + fTemp, kNumInfo.vTexCoords.w );


        GCUITextureCache * cache=NULL;

        if ( bShadow )
        {

            KD3DUIVertex rectShadow[4];
			D3DXVECTOR4  vOffset( 2.0f * fScaleX, 2.0f * fScaleY, 0.0f, 0.0f );

            ::memcpy( rectShadow, m_rectNumRender, 4 * sizeof( KD3DUIVertex ) );

            rectShadow[0].m_vPos += vOffset; rectShadow[0].m_diff = 0xff000000;
            rectShadow[1].m_vPos += vOffset; rectShadow[1].m_diff = 0xff000000;
            rectShadow[2].m_vPos += vOffset; rectShadow[2].m_diff = 0xff000000;
            rectShadow[3].m_vPos += vOffset; rectShadow[3].m_diff = 0xff000000;


            DrawUIInScreen(NULL,kNumInfo.iTexIdx,rectShadow);
        }

        m_rectNumRender[0].m_diff = argb;
        m_rectNumRender[1].m_diff = argb;
        m_rectNumRender[2].m_diff = argb;
        m_rectNumRender[3].m_diff = argb;


        DrawUIInScreen(NULL,kNumInfo.iTexIdx,m_rectNumRender);

    }

    g_pGCDeviceManager2->PopState();
}

void KD3DUIManager::RenderNumber( const std::string& strType,
                                 const D3DXVECTOR2& vPos,
                                 UINT64 dwNum,
                                 bool bWriteToRightSide /* = true */,
                                 bool bShadow, /* = true */
                                 float fInterval,/*= -1.0f*/ 
                                 bool bScale)
{
    char str[MAX_PATH] = {0,};
    sprintf(str,"%I64u",dwNum);
    RenderNumber(strType, vPos, str, D3DXCOLOR( 0xffffffff ), bWriteToRightSide, bShadow, fInterval ,bScale);
}

void KD3DUIManager::RenderNumber( const std::string& strType, 
                                  const D3DXVECTOR2& vPos, 
                                  DWORD dwNum, 
                                  bool bWriteToRightSide,
                                  bool bShadow,
                                  float fInterval,
                                  bool bScale,
                                  int iPrecision,
                                  bool bPrecision )
{
    if( bPrecision ) { 
    std::ostringstream stm;
    stm << setw( iPrecision ) << setfill( '0' ) << dwNum;
    RenderNumber(strType, vPos, stm.str().c_str(), D3DXCOLOR( 0xffffffff ), bWriteToRightSide, bShadow, fInterval ,bScale);
    }
    else { 
        RenderNumber( strType, vPos, dwNum, bWriteToRightSide, bShadow, fInterval, bScale );
    }
}

void KD3DUIManager::RenderNumberScale( const std::string& strType,
								 const D3DXVECTOR2& vPos,
								 DWORD dwNum,								 
								 float fScale /*=1.0*/,
                                 DWORD dwColor,
                                 bool bShadow /*=true*/, 
								 bool bScale,
                                 bool bWriteToRightSide, /* = true */
								 float fInterval /* = -1.0 */)
{
	char str[MAX_PATH] = {0,};
	sprintf(str,"%d",dwNum);
	RenderNumber(strType, vPos, str, dwColor, bWriteToRightSide, bShadow, fInterval, bScale, false, fScale);
}

void KD3DUIManager::RenderNumber(	const std::string& strType,
                                 const D3DXVECTOR2& vPos,
                                 UINT64 dwNum,
                                 D3DXCOLOR argb /*= D3DXCOLOR( 0xff000000 )*/,
                                 bool bWriteToRightSide /*= true*/,
                                 bool bShadow /*= true */,
                                 float fInterval /*= -1.0f*/,
                                 bool bScale)
{

    char str[MAX_PATH];
    sprintf(str,"%I64u",dwNum);
    RenderNumber(strType, vPos, str, argb , bWriteToRightSide, bShadow, fInterval ,bScale);
}

bool GCUITextureCache::IsContain( int itex,D3DXVECTOR4 *vec )
{
    if(m_iTexIndex!=itex)
        return false;


    if(m_uv.x > vec->x ||
        m_uv.y > vec->y ||
        m_uv.z < vec->z ||
        m_uv.w < vec->w)
        return false;

    return true;
}

bool GCUITextureCache::IsContained( int itex,D3DXVECTOR4 *vec )
{
    if(m_iTexIndex!=itex)
        return false;

    if(m_uv.x < vec->x ||
        m_uv.y < vec->y ||
        m_uv.z > vec->z ||
        m_uv.w > vec->w)
        return false;

    return true;
}



GCUITextureCache::GCUITextureCache( int index,D3DXVECTOR4 *uv )
:m_iTexIndex(index),m_uv(*uv),m_BuildTexture(NULL)
{
    float x=0;
    float y=0;
    float z=1;
    float w=1;
    float delta=0.125f;

    while(x+delta < m_uv.x)
        x+=delta;

    while(y+delta < m_uv.y)
        y+=delta;

    while(z-delta > m_uv.z)
        z-=delta;

    while(w-delta > m_uv.w)
        w-=delta;

    m_uv.x=x;
    m_uv.y=y;
    m_uv.w=w;
    m_uv.z=z;
}

GCUITextureCache::~GCUITextureCache()
{
}

void GCUITextureCache::DrawInScreen( KD3DUIVertex* vert )
{
    Use();
    m_BuildTexture->Use();
    g_pGCDeviceManager2->DrawUI(m_BuildTexture,vert,&m_BuildSection.uv,&m_uv);
   // g_pGCDeviceManager2->Draw_Wether(m_BuildTexture,vert,&m_BuildSection.uv,&m_uv);    
}

void GCUITextureCache::Clear()
{
    if(m_BuildTexture)
        m_BuildTexture->SetFreeTextureSection(&m_BuildSection);
}

bool GCUITextureCache::SetTexture( GCBuildTexture * tex,GCTmpTexture * tmptex )
{
    m_BuildTexture = tex;
    return tex->AddTexture(tmptex,&m_BuildSection,&m_uv);
}
/*void KD3DUIManager::DrawInScreen( int iIndex,KD3DUIVertex* src )
{
if(iIndex<0 || m_vecStrTexFileName.size()<=iIndex)
return;

if(iIndex==3 || iIndex==4)
{
g_pGCDeviceManager2->DrawInScreen(GetDeviceTexture(iIndex),src);
return;
}

D3DXVECTOR4 vec;

if ( src[0].m_vTex.x <= src[3].m_vTex.x)
{
vec.x = src[0].m_vTex.x;
vec.z = src[3].m_vTex.x;
}
else
{
vec.x = src[3].m_vTex.x;
vec.z = src[0].m_vTex.x;
}

if ( src[0].m_vTex.y <= src[3].m_vTex.y)
{
vec.y = src[0].m_vTex.y;
vec.w = src[3].m_vTex.y;
}
else
{
vec.y = src[3].m_vTex.y;
vec.w = src[0].m_vTex.y;
}

// 현재 텍스쳐 캐시에 있나 보자
for(std::list<GCUITextureCache*>::iterator i = m_listUITextureSection.begin();
i!=m_listUITextureSection.end();i++)
{
if((*i)->IsContain(iIndex,&vec))
{
(*i)->DrawInScreen(src);
return;
}
}

// 텍스쳐가 바뀌니까 일단 플러슁 하자
g_pGCDeviceManager2->Flush();

// 없으면 새로 추가하자

// 새 캐시를 만들어 넣는다
GCUITextureCache * section = new GCUITextureCache(iIndex,&vec);

// 일단 추가하기 전에 기존에 캐시중에 하위에 있는게 있으면 빼버리자
std::list<GCUITextureCache*>::iterator iter = m_listUITextureSection.begin();
while(iter!=m_listUITextureSection.end())
{
if((*iter)->IsContained(section))
{
delete (*iter);
iter = m_listUITextureSection.erase(iter);
}
else
iter++;
}
m_listUITextureSection.push_back(section);
m_listBuildTexture.sort(INotUseCountable::LessNotUse());

// 기존에 사용하던 템프 텍스쳐인지 확인하자
std::string tex = m_vecStrTexFileName[iIndex];
if(m_tmpTexture)
{
if(m_tmpTexture->GetFileName()!=tex)
{
m_tmpTexture->Release();
m_tmpTexture = g_pGCDeviceManager2->CreateTmpTexture(tex);
}
}
else
{
m_tmpTexture = g_pGCDeviceManager2->CreateTmpTexture(tex);
}


for(std::list<GCBuildTexture*>::iterator i = m_listBuildTexture.begin();
i!=m_listBuildTexture.end();i++)
{
if(section->SetTexture((*i),m_tmpTexture))
{
section->DrawInScreen(src);
return;
}
}

// 기존 텍스쳐에 빈공간이 없으면 별수없다 텍스쳐를 추가하자
GCBuildTexture * texture = new GCBuildTexture(g_pd3dDevice,D3DFMT_A8R8G8B8,false);
m_listBuildTexture.push_back(texture);
if(section->SetTexture(texture,m_tmpTexture))
{
TRACE(L"Texture Count = %d\n",m_listBuildTexture.size());
section->DrawInScreen(src);
return;
}
assert(!"여기로오면 안되아!");
}*/




GCUITextureCache* KD3DUIManager::GetUITextureCache( int iIndex,D3DXVECTOR4* vec )
{
    // 현재 텍스쳐 캐시에 있나 보자
    for(std::set<GCUITextureCache*>::iterator i = m_setUITextureSection.begin();
        i!=m_setUITextureSection.end();i++)
    {
        if((*i)->IsContain(iIndex,vec))
        {
            return (*i);
        }
    }
	// MoveGapTime은 부모를 따릅니다.

    // 새 캐시를 만들어 넣는다
    GCUITextureCache * section = new GCUITextureCache(iIndex,vec);

    // 일단 추가하기 전에 기존에 캐시중에 하위에 있는게 있으면 빼버리자
    std::set<GCUITextureCache*>::iterator iter = m_setUITextureSection.begin();
    std::list<GCUITextureCache*> deletelist;
    while(iter!=m_setUITextureSection.end())
    {
        if((*iter)->IsContained(section))
        {
            (*iter)->Clear();
            delete (*iter);
            deletelist.push_back(*iter);
        }
        iter++;
    }

    while(!deletelist.empty())
    {
        m_setUITextureSection.erase(deletelist.back());
        deletelist.pop_back();
    }

    m_setUITextureSection.insert(section);

    m_listBuildTexture.sort(INotUseCountable::LessNotUse());

    // 기존에 사용하던 템프 텍스쳐인지 확인하자
    std::string tex = m_vecStrTexFileName[iIndex];
    if(m_tmpTexture)
    {
        if(m_tmpTexture->GetFileName()!=tex)
        {
            m_tmpTexture->Release();
            m_tmpTexture = g_pGCDeviceManager2->CreateTmpTexture(tex);
        }
    }
    else
    {
        m_tmpTexture = g_pGCDeviceManager2->CreateTmpTexture(tex);
    }


    for(std::list<GCBuildTexture*>::iterator i = m_listBuildTexture.begin();
        i!=m_listBuildTexture.end();i++)
    {
        if(section->SetTexture((*i),m_tmpTexture))
        {
            return section;
        }
    }

    // 기존 텍스쳐에 빈공간이 없으면 별수없다 텍스쳐를 추가하자
    GCBuildTexture * texture = new GCBuildTexture(g_pd3dDevice,D3DFMT_A8R8G8B8,false);
    m_listBuildTexture.push_back(texture);
    if(section->SetTexture(texture,m_tmpTexture))
    {
        TRACE(L"Texture Count = %d\n",m_listBuildTexture.size());
        return section;
    }
    assert(!"여기로오면 안되아!");
    return NULL;
}



GCUITextureCache* KD3DUIManager::DrawUIInScreen( GCUITextureCache* cache,int iIndex,KD3DUIVertex* src )
{
    if(iIndex<0 || (int)m_vecStrTexFileName.size()<=iIndex)
        return NULL;

    if(iIndex==3 || iIndex==4 || iIndex == 69)
    {
        g_pGCDeviceManager2->DrawInScreen(GetDeviceTexture(iIndex),src);
        return NULL;
    }

    D3DXVECTOR4 vec;

    if ( src[0].m_vTex.x <= src[3].m_vTex.x)
    {
        vec.x = src[0].m_vTex.x;
        vec.z = src[3].m_vTex.x;
    }
    else
    {
        vec.x = src[3].m_vTex.x;
        vec.z = src[0].m_vTex.x;
    }

    if ( src[0].m_vTex.y <= src[3].m_vTex.y)
    {
        vec.y = src[0].m_vTex.y;
        vec.w = src[3].m_vTex.y;
    }
    else
    {
        vec.y = src[3].m_vTex.y;
        vec.w = src[0].m_vTex.y;
    }

    if(m_setUITextureSection.find(cache) == m_setUITextureSection.end() ||
        cache->IsContain(iIndex,&vec) == false)
    {
        cache = GetUITextureCache(iIndex,&vec);
    }

    if(cache)
        cache->DrawInScreen(src);

    return cache;
}

void KD3DUIManager::ResetAllUIforResolution()
{
	for(std::map<std::string, KD3DUIScene*>::iterator it=m_mapScene.begin();it != m_mapScene.end();++it)
	{
		it->second->ResetResolution();
	}	
}

KD3DWnd* KD3DUIManager::CreateTemplateUI( IN KD3DWnd* pkParent, IN std::string strType, IN std::string strScriptFile, IN std::string strWndName, IN bool bCreate, bool bPushTable /*= false */ )
{
    if( pkParent == NULL )
        return NULL;

    if( m_mapWndFactory.find( strType ) == m_mapWndFactory.end() )
        return NULL;

    KLuaManager luaMgr;
	if( GCFUNC::LoadLuaScript( luaMgr, "ui_sign.stg" ) == false )
	    return NULL;
	if( GCFUNC::LoadLuaScript( luaMgr, strScriptFile ) == false )
		return NULL;

    KD3DWnd* pkCreated = NULL;
    pkCreated = m_mapWndFactory[strType]->Create();

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    if( bPushTable ) {
        pkParent->PushTable();
    }

    LUA_BEGIN_TABLE( "scene_part", SAFE_DELETE( pkCreated ); return NULL )
    {
        LUA_BEGIN_TABLE( 1, SAFE_DELETE( pkCreated ); return NULL )
        {
            LoadUIWindowTable_FromScriptOriginal( &luaMgr, pkCreated, pkParent, strWndName);
        }
        LUA_END_TABLE( SAFE_DELETE( pkCreated ); return NULL )
    }
    LUA_END_TABLE( SAFE_DELETE( pkCreated ); return NULL );

    if( bPushTable ) {
        pkParent->PopTable( true );
    }

    //pkParent->AddChild( pkCreated );
    if( pkParent->GetWndState() != KD3DWnd::D3DWS_DISABLED && bCreate )
        pkCreated->Create( pkParent->IsAutoCreate() );
    return pkCreated;
}

KD3DWnd* KD3DUIManager::CreateUI( const char* strType )
{
    std::map<std::string, KD3DWndFactory*>::iterator mit;
    mit = m_mapWndFactory.find( strType );
    if( mit == m_mapWndFactory.end() )
        return NULL;

    return mit->second->Create();
}



KD3DWnd* KD3DUIManager::CreateDefaultUI( IN KD3DWnd* pkParent, IN std::string strType, IN std::string strWndName )
{
    //m_dwTotalWnd++;

    if( pkParent == NULL )
        return NULL;

    if( m_mapWndFactory.find( strType ) == m_mapWndFactory.end() )
        return NULL;

    KD3DWnd* pkCreated = NULL;
    pkCreated = m_mapWndFactory[strType]->Create();

    pkCreated->Create( pkParent->IsAutoCreate() );

    DWORD   dwWidth;
    DWORD   dwHeight;

    //	int		bPixelValue;
    int     iDefTexIdx; // "default" texture index
    int     iHovTexIdx; // "hover"   texture index
    int     iSelTexIdx; // "select"  texture index
    int     iActTexIdx; // "active"  texture index
    int     iLocTexIdx; // "lock"    texture index

    int     iHotKey;
    std::string strHotKey;

    float   tu1def, tv1def, tu2def, tv2def; // "default" texture coordinates
    float   tu1hov, tv1hov, tu2hov, tv2hov; // "hover"   texture coordinates
    float   tu1sel, tv1sel, tu2sel, tv2sel; // "select"  texture coordinates
    float   tu1act, tv1act, tu2act, tv2act; // "active"  texture coordinates
    float   tu1loc, tv1loc, tu2loc, tv2loc; // "lock"    texture coordinates

    float   fCreatePosX, fCreatePosY;
    float   fWindowPosX, fWindowPosY;
    float   fDestroyPosX, fDestroyPosY;

    std::string strZOrd;
    float   fMoveGapTime;
    //std::string strMotion;
    //std::string strAlpha;
    std::string strToolTip;
    //std::string strType;
    bool    bAutoCreate;
    bool    bPredraw;
    bool	bRender;

    bool    static_instance = false;

    std::string strMotion = "linear";
    std::string strAlpha = "linear";

    std::string strFriend = "";


    dwWidth = 0;
    dwHeight = 0;

    iDefTexIdx = -1;
    //bPixelValue = 0;
    tu1def = 0.0f;
    tv1def = 0.0f;
    tu2def = 1.0f;
    tv2def = 1.0f;

    iHovTexIdx = -1;
    tu1hov = 0.0f;
    tv1hov = 0.0f;
    tu2hov = 1.0f;
    tv2hov = 1.0f;

    iSelTexIdx = -1;
    tu1sel = 0.0f;
    tv1sel = 0.0f;
    tu2sel = 1.0f;
    tv2sel = 1.0f;

    iActTexIdx = -1;
    tu1act = 0.0f;
    tv1act = 0.0f;
    tu2act = 1.0f;
    tv2act = 1.0f;

    iLocTexIdx = -1;
    tu1loc = 0.0f;
    tv1loc = 0.0f;
    tu2loc = 1.0f;
    tv2loc = 1.0f;

    fCreatePosX = 0.0f;
    fCreatePosY = 0.0f;
    fWindowPosX = 0.0f;
    fWindowPosY = 0.0f;
    fDestroyPosX = 0.0f;
    fDestroyPosY = 0.0f;

    fMoveGapTime = 0.0f;
    strMotion = "linear";
    strAlpha = "linear";
    bAutoCreate = true;
    bPredraw = false;
    iHotKey = -1;
    strHotKey = "";
    strToolTip = "";

    bRender = true;

    /*if ( SUCCEEDED( hr = pkLuaMgr->GetValue( "z_order", strZOrd ) ) && ( strZOrd == "topmost" ) )
    {
    pkWnd->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    }*/


    //ToolTip , TopMost, Hotkey, HotkeyStyle ,PixelValue, Motion, Alpha, Friend 사용안해서 지웁니다.
    pkCreated->SetHotKey( iHotKey );
    pkCreated->SetHotKeyStyle( strHotKey );

    // set auto create flag
    pkCreated->SetAutoCreate( bAutoCreate );

    // set predraw flag
    pkCreated->SetPredraw( bPredraw );

    //visible
    pkCreated->ToggleRender(bRender);

    /*if( bPixelValue == 1 )
    {*/
	float fMagicNumber=0.5f;
	if (tu1def>tu2def)
	{
		fMagicNumber*=-1.0f;
	}
	tu1def = (tu1def+fMagicNumber ) / 512.0f;
	tu2def = (tu2def-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tv1def>tv2def)
	{
		fMagicNumber*=-1;
	}
	tv1def = (tv1def+fMagicNumber ) / 512.0f;
	tv2def = (tv2def-fMagicNumber ) / 512.0f;

	//
	fMagicNumber=0.5f;
	if (tu1hov>tu2hov)
	{
		fMagicNumber*=-1;
	}
	tu1hov = (tu1hov+fMagicNumber ) / 512.0f;
	tu2hov = (tu2hov-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tv1hov>tv2hov)
	{
		fMagicNumber*=-1;
	}
	tv1hov = (tv1hov+fMagicNumber ) / 512.0f;
	tv2hov = (tv2hov-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tu1sel>tu2sel)
	{
		fMagicNumber*=-1;
	}
	tu1sel = (tu1sel+fMagicNumber ) / 512.0f;
	tu2sel = (tu2sel-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tv1sel>tv2sel)
	{
		fMagicNumber*=-1;
	}
	tv1sel = (tv1sel+fMagicNumber ) / 512.0f;
	tv2sel = (tv2sel-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tu1act>tu2act)
	{
		fMagicNumber*=-1;
	}
	tu1act = (tu1act+fMagicNumber ) / 512.0f;
	tu2act = (tu2act-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tv1act>tv2act)
	{
		fMagicNumber*=-1;
	}
	tv1act = (tv1act+fMagicNumber ) / 512.0f;
	tv2act = (tv2act-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tu1loc>tu2loc)
	{
		fMagicNumber*=-1;
	}
	tu1loc = (tu1loc+fMagicNumber ) / 512.0f;
	tu2loc = (tu2loc-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (tv1loc>tv2loc)
	{
		fMagicNumber*=-1;
	}
	tv1loc = (tv1loc+fMagicNumber ) / 512.0f;
	tv2loc = (tv2loc-fMagicNumber ) / 512.0f;

	TEXEL_BOUND(tu1def);
	TEXEL_BOUND(tv1def);
	TEXEL_BOUND(tu2def);
	TEXEL_BOUND(tv2def);

	TEXEL_BOUND(tu1hov);
	TEXEL_BOUND(tv1hov);
	TEXEL_BOUND(tu2hov);
	TEXEL_BOUND(tv2hov);

	TEXEL_BOUND(tu1act);
	TEXEL_BOUND(tv1act);
	TEXEL_BOUND(tu2act);
	TEXEL_BOUND(tv2act);

	TEXEL_BOUND(tu1loc);
	TEXEL_BOUND(tv1loc);
	TEXEL_BOUND(tu2loc);
	TEXEL_BOUND(tv2loc);

    // check unregistered spline type
    if ( 0 == m_mapSpline.count( strMotion ) ) return pkCreated;
    if ( 0 == m_mapSpline.count( strAlpha ) ) return pkCreated;

    // set spline
    pkCreated->SetMotionSpline( m_mapSpline[strMotion] );
    pkCreated->SetAlphaSpline( m_mapSpline[strAlpha] );

    int iStrIndex = atoi( strToolTip.c_str() );
    if ( iStrIndex )
        pkCreated->SetToolTip( g_pkStrLoader->GetString( iStrIndex ) );


    if (strWndName.length()>MAX_WND_SIZE) {
        ASSERT("UI 이름을 도데체 얼마나 거창하게 지은거임?");
    }

    KD3DWndClass kWndClass;
    strcpy(kWndClass.strWndName , strWndName.c_str());
    strcpy(kWndClass.strType , strType.c_str());
    //kWndClass.strFriend = strFriend;
    kWndClass.pParent = pkParent;
    kWndClass.dwWidth = dwWidth;
    kWndClass.dwHeight = dwHeight;
    kWndClass.vCreatePos = D3DXVECTOR2( fCreatePosX, fCreatePosY );
    kWndClass.vWindowPos = D3DXVECTOR2( fWindowPosX, fWindowPosY );
    kWndClass.vDestroyPos = D3DXVECTOR2( fDestroyPosX, fDestroyPosY );

    // MoveGapTime은 부모를 따릅니다.
    if( pkParent == NULL )
        kWndClass.fMoveGap = fMoveGapTime;
    else
        kWndClass.fMoveGap = pkParent->GetMoveGapTime();

    kWndClass.iDefTex = iDefTexIdx;
    kWndClass.iHovTex = iHovTexIdx;
    kWndClass.iSelTex = iSelTexIdx;
    kWndClass.iActTex = iActTexIdx;
    kWndClass.iLocTex = iLocTexIdx;

    kWndClass.vDefTexCoords = D3DXVECTOR4( tu1def, tv1def, tu2def, tv2def );
    kWndClass.vHovTexCoords = D3DXVECTOR4( tu1hov, tv1hov, tu2hov, tv2hov );
    kWndClass.vSelTexCoords = D3DXVECTOR4( tu1sel, tv1sel, tu2sel, tv2sel );
    kWndClass.vActTexCoords = D3DXVECTOR4( tu1act, tv1act, tu2act, tv2act );
    kWndClass.vLocTexCoords = D3DXVECTOR4( tu1loc, tv1loc, tu2loc, tv2loc );

    kWndClass.bAutoCreate=bAutoCreate;
    kWndClass.bPredraw=bPredraw;
    kWndClass.bRender=bRender;

    // register window information
    pkCreated->Register( kWndClass );

    return pkCreated;
}

std::vector<KD3DWnd*> KD3DUIManager::CreateTemplateUIListByFindName( IN KD3DWnd* pkParent_, IN const char* strScriptFile_, IN const char* strMakeWndName_, IN const char* strWndFindName_, IN bool bCreate_ /*= true*/, int iCount /*= 1*/ )
{
    std::vector<KD3DWnd*> vecCreatedWnd;

    if( pkParent_ == NULL )
        return vecCreatedWnd;

    KLuaManager luaMgr;
	if( GCFUNC::LoadLuaScript( luaMgr, "ui_sign.stg" ) == false )
		return vecCreatedWnd;
    if( GCFUNC::LoadLuaScript( luaMgr, strScriptFile_ ) == false )
        return vecCreatedWnd;

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK( L );
    pkParent_->PushTable();
    BOOST_SCOPE_EXIT( (&pkParent_) ){
        pkParent_->PopTable();
    }BOOST_SCOPE_EXIT_END;

    if( SUCCEEDED( luaMgr.BeginTable( "scene_part" ) ) )
    {
        LoadUIScriptByFindName( &luaMgr, vecCreatedWnd, pkParent_, strMakeWndName_, strWndFindName_, iCount );
        LUA_END_TABLE();
    }

    if( pkParent_->GetWndState() != KD3DWnd::D3DWS_DISABLED && bCreate_ )
    {
        std::vector<KD3DWnd*>::iterator vit;
        for( vit = vecCreatedWnd.begin() ; vit != vecCreatedWnd.end() ; ++vit ) {
            (*vit)->SetAlignType( pkParent_->GetAlignType() );
            (*vit)->Create( pkParent_->IsAutoCreate() );
        }
    }


    return vecCreatedWnd;
}

KD3DWnd* KD3DUIManager::CreateTemplateUIByFindName( IN KD3DWnd* pkParent_, IN const char* strScriptFile_, IN const char* strMakeWndName_, IN const char* strWndFindName_,  IN bool bCreate_ )
{
    if( pkParent_ == NULL )
        return NULL;

    std::vector<KD3DWnd*> vecCreatedWnd = CreateTemplateUIListByFindName( pkParent_, strScriptFile_, strMakeWndName_, strWndFindName_, bCreate_, 1 );

    if( vecCreatedWnd.empty() )
        return NULL;

    return vecCreatedWnd[0];
}

HRESULT KD3DUIManager::LoadUIScriptByFindName( KLuaManager* pkLuaMgr_, std::vector<KD3DWnd*>& vecWnd_, KD3DWnd* pkParent_, std::string strMakeWndName_, std::string strFindWndName_, int iCount )
{
    for( int i = 1; ; ++i )
    {
        if( FAILED( pkLuaMgr_->BeginTable(i) ) )
            return E_FAIL;

        std::string strWndName;
        pkLuaMgr_->GetValue( "name", strWndName );

        std::string strFindString = "\\";
        size_t iFindIndex = strFindWndName_.find( strFindString, 0);
        
        if( std::string::npos != iFindIndex )
        {
            if( 0 == strcmp( strWndName.c_str(), strFindWndName_.substr( 0, iFindIndex).c_str() ) )
            {
                HRESULT hResult = LoadUIScriptByFindName( pkLuaMgr_, vecWnd_, pkParent_, strMakeWndName_, strFindWndName_.substr( iFindIndex + strFindString.size() ), iCount );
                pkLuaMgr_->EndTable();
                return hResult;
            }
        }
        else if( 0 == strcmp( strWndName.c_str(), strFindWndName_.c_str() ) )
        {
            std::string strType;
            if ( FAILED( pkLuaMgr_->GetValue( "type", strType ) ) ) {
                strType = "window";
            }
            
            for( int i = 0 ; i < iCount ; ++i ) {
                KD3DWnd* pkWnd = NULL;
                pkWnd = this->CreateWndFromFactory( strType );
                if( pkWnd == NULL) {
                    pkLuaMgr_->EndTable();
                    continue;
                }

                HRESULT hResult = LoadUIWindowTable_FromScriptOriginal( pkLuaMgr_, pkWnd, pkParent_, strMakeWndName_);
                vecWnd_.push_back( pkWnd );
            }
            pkLuaMgr_->EndTable();
            return S_OK;
        }

        if( FAILED( pkLuaMgr_->EndTable() ) )
            return E_FAIL;
    }
}

D3DXVECTOR2 KD3DUIManager::GetMousePos()
{
    POINT ptMouse = g_pkInput->GetMousePos();
    return D3DXVECTOR2( (float)ptMouse.x, (float)ptMouse.y );
}

void KD3DUIManager::SendEvent( const KActionEvent& kEvent_, bool bSendLua_ /*= true*/ )
{
	if(SiKGCDragManager()->GetDragState() == KGCDragManager::KGDM_DRAG_PROGRESS) return;

    if( bSendLua_ ) {
        switch( kEvent_.m_dwCode ) {
            case KD3DWnd::D3DWE_BUTTON_DOWN:
            //case KD3DWnd::D3DWE_CURSOR_ENTER:
            //case KD3DWnd::D3DWE_CURSOR_LEAVE:
            case KD3DWnd::D3DWE_BUTTON_CLICK:
            case KD3DWnd::D3DWE_SCENE_CHANGE:
            case KD3DWnd::D3DWE_CREATE:
            case KD3DWnd::D3DWE_CREATE_COMPLETE:
            case KD3DWnd::D3DWE_DESTROY:
            case KD3DWnd::D3DWE_DESTROY_COMPLETE:
            case KD3DWnd::D3DWE_MESSAGEBOX_OPEN:
            case KD3DWnd::D3DWE_MESSAGEBOX_CLOSE:
                SendEventToLua( kEvent_ );
                break;
            default:
                break;
        }
    }

    int iUID = -1;
    if( kEvent_.m_pWnd != NULL ) {
        iUID = kEvent_.m_pWnd->GetUID();
    }

    CallWndProcedure( iUID, kEvent_ );
    if( iUID != -1 )
        CallWndProcedure( -1, kEvent_ );
}

void KD3DUIManager::SendEventToLua( const KActionEvent& kEvent_ )
{
//     lua_State* L = KGCLuabinder::getInstance().GetLuaState();
//     CHECK_STACK(L)
// 
//     lua_checkstack( L, lua_gettop(L) + 5 );
// 
//     lua_getfield( L, LUA_GLOBALSINDEX, "UIFunc" );
// 
//     if( lua_istable( L, -1 ) == false ) {
//         lua_pop( L, 1 );
//         return;
//     }
// 
//     lua_getfield( L, -1, "SendEvent" );
//     if( lua_isfunction( L, -1 ) == false ) {
//         lua_pop( L, 2 );
//         return;
//     }
// 
//     lua_getfield( L, LUA_GLOBALSINDEX, "UIFunc" );
//     if( lua_istable( L, -1 ) == 0 ){
//         lua_pop( L, 3 );
//         return;
//     }
// 
//     lua_tinker::push( L, kEvent_ );
// 
//     if( lua_pcall( L, 2, 0, 0 ) != 0 ) {
//         START_LOG( cerr, lua_tostring( L, -1 ) );
//         lua_pop( L, 1 );
//         return;
//     }
// 
//     lua_pop( L, 1 );
}

void KD3DUIManager::ConnectEventProcedure( KD3DWnd::uid_type UID_, DWORD dwCode_, boost::shared_ptr<KD3DWndEventProcedure> func_ )
{
    m_mapProcedure[UID_][dwCode_].push_back( func_ );
}

void KD3DUIManager::ConnectEventProcedure( DWORD dwCode_, boost::shared_ptr<KD3DWndEventProcedure> func_ )
{
    ConnectEventProcedure( -1, dwCode_, func_ );
}

void KD3DUIManager::CallWndProcedure( KD3DWnd::uid_type UID_, const KActionEvent& kEvent_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.find( UID_ );

    if( itcode == m_mapProcedure.end() )
        return;
	
	if (!itcode->second.empty())
	{
		procedurewnd_type procedure_wnd = itcode->second;
		CallCodeProcedure(UID_, procedure_wnd, kEvent_.m_dwCode, kEvent_);
		CallCodeProcedure(UID_, procedure_wnd, KD3DWnd::D3DWE_ALL_EVENT, kEvent_);
	}

    if( itcode->second.empty() )
    {
        m_mapProcedure.erase( itcode );
        return;
    }
}

void KD3DUIManager::CallCodeProcedure( KD3DWnd::uid_type UID_, procedurewnd_type& procedure_wnd, DWORD dwCode_, const KActionEvent& kEvent_ )
{
    procedurewnd_type::iterator itwnd = procedure_wnd.find( dwCode_ );
    if( itwnd == procedure_wnd.end() ) {
        return;
    }

    if( itwnd->second.empty() )
    {
        procedure_wnd.erase( itwnd );
        return;
    }

    procedurefunc_type& procedures = itwnd->second;
    procedurefunc_type::iterator itproc;
    for( itproc = procedures.begin() ; itproc != procedures.end() ; ++itproc ) {
        if( (*itproc)->CallProcedure( kEvent_ ) == false ) {
            DisconnectEventProcedure_Detail( UID_, dwCode_, (*itproc)->GetUID() );
        }
    }
}

KD3DWnd* KD3DUIManager::CreateWndFromFactory( std::string& strType_ )
{
    if ( m_mapWndFactory.end() == m_mapWndFactory.find( strType_ ) )
    {
        MessageBoxA( NULL, "No Factory" , strType_.c_str(), MB_OK );
        strType_ = "window";
    }

    if(m_mapWndFactory[strType_]->CanCreate() == false )
        return NULL;

    KD3DWnd* pWnd = m_mapWndFactory[strType_]->Create();
    m_mapWndList.insert( wndlist_type::value_type( pWnd->GetUID(), pWnd ) );
    return pWnd;
}

KD3DWnd* KD3DUIManager::GetWndFromID( KD3DWnd::uid_type UID_ )
{
    wndlist_type::iterator mit = m_mapWndList.find( UID_ );

    if( mit == m_mapWndList.end() )
        return NULL;

    return mit->second;
}

void KD3DUIManager::DisconnectAllEventProcedure( KD3DWnd::uid_type UID_ )
{
    procedurelist_type::iterator mit = m_mapProcedure.find( UID_ );

    if( mit == m_mapProcedure.end() )
        return;

    m_mapProcedure.erase( mit );
}

void KD3DUIManager::RemoveUID( KD3DWnd::uid_type UID_ )
{
    wndlist_type::iterator mit = m_mapWndList.find( UID_ );

    if( mit == m_mapWndList.end() )
        return;

    m_mapWndList.erase( mit );
}

void KD3DUIManager::DisconnectEventProcedure_Detail( KD3DWnd::uid_type UID_, DWORD dwCode_, int iProcedureUID_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.find( UID_ );

    if( itcode == m_mapProcedure.end() )
        return;

    if( itcode->second.empty() )
    {
        m_mapProcedure.erase( itcode );
        return;
    }

    procedurewnd_type::iterator itwnd = itcode->second.find( dwCode_ );
    if( itwnd == itcode->second.end() ) {
        return;
    }

    if( itwnd->second.empty() )
    {
        itcode->second.erase( itwnd );
        return;
    }

    procedurefunc_type& procedures = itwnd->second;
    procedurefunc_type::iterator itproc;
    itproc = std::find_if( procedures.begin(), procedures.end(), boost::bind( &KD3DWndEventProcedure::GetUID, _1 ) == iProcedureUID_ );
    if( itproc != procedures.end() ) {
        procedures.erase( itproc );
    }
}

void KD3DUIManager::DisconnectEventProcedure_ProcID( KD3DWnd::uid_type UID_, int iProcedureUID_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.find( UID_ );

    if( itcode == m_mapProcedure.end() )
        return;

    if( itcode->second.empty() )
    {
        m_mapProcedure.erase( itcode );
        return;
    }

    procedurewnd_type::iterator itwnd = itcode->second.begin();
    while( itwnd != itcode->second.end() ) {
        procedurefunc_type& procedures = itwnd->second;
        procedurefunc_type::iterator itproc;
        itproc = std::find_if( procedures.begin(), procedures.end(), boost::bind( &KD3DWndEventProcedure::GetUID, _1 ) == iProcedureUID_ );
        if( itproc != procedures.end() ) {
            procedures.erase( itproc );
        }
    }
}

void KD3DUIManager::DisconnectEventProcedure( int iProcedureUID_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.begin();
    while( itcode != m_mapProcedure.end() ) {
        DisconnectEventProcedure_ProcID( itcode->first, iProcedureUID_ );
    }
}

void KD3DUIManager::DisconnectEventProcedure_Code( KD3DWnd::uid_type UID_, DWORD dwCode_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.find( UID_ );

    if( itcode == m_mapProcedure.end() )
        return;

    if( itcode->second.empty() )
    {
        m_mapProcedure.erase( itcode );
        return;
    }

    procedurewnd_type::iterator itwnd = itcode->second.find( dwCode_ );
    if( itwnd == itcode->second.end() ) {
        return;
    }

    itcode->second.erase( itwnd );
    if( itcode->second.empty() )
    {
        m_mapProcedure.erase( itcode );
        return;
    }
}

void KD3DUIManager::DisconnectGlobalEventProcedure( KD3DWnd::uid_type UID_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.find( -1 );
    if( itcode == m_mapProcedure.end() )
        return;

    if( itcode->second.empty() ) {
        m_mapProcedure.erase( itcode );
        return;
    }

    procedurewnd_type::iterator itwnd = itcode->second.begin();
    while( itwnd != itcode->second.end() ) {
        DisconnectGlobalEventProcedure( UID_, itwnd->second );
        ++itwnd;
    }
}

void KD3DUIManager::DisconnectGlobalEventProcedure( KD3DWnd::uid_type UID_, procedurefunc_type& procedures_ )
{
    procedurefunc_type::iterator itor = procedures_.begin();

    for( itor = procedures_.begin() ; itor != procedures_.end() ; ) {
        if( (*itor)->m_ID == UID_ ) {
            itor = procedures_.erase( itor );
        }
        else {
            ++itor;
        }
    }
}

void KD3DUIManager::DisconnectGlobalEventProcedure( KD3DWnd::uid_type UID_, DWORD dwCode_ )
{
    procedurelist_type::iterator itcode = m_mapProcedure.find( -1 );
    if( itcode == m_mapProcedure.end() )
        return;

    if( itcode->second.empty() ) {
        m_mapProcedure.erase( itcode );
        return;
    }

    procedurewnd_type::iterator itwnd = itcode->second.find( dwCode_ );
    if( itwnd == itcode->second.end() ) {
        return;
    }

    if( itwnd->second.empty() )
    {
        itcode->second.erase( itwnd );
        return;
    }

    DisconnectGlobalEventProcedure( UID_, itwnd->second );
}

void KD3DUIManager::GetTypeNumberInfo( const std::string& strType, DWORD& dwWidth_ , DWORD& dwHeight_  )
{
	std::map<std::string, KD3DUINumInfo>::iterator itor = m_mapNumInfo.find(strType);
	if( itor == m_mapNumInfo.end() )
		return;

	KD3DUINumInfo &kNumInfo = itor->second;

	if ( -1 == kNumInfo.iTexIdx )
		return;

	dwWidth_ = kNumInfo.dwWidth;
	dwHeight_ = kNumInfo.dwHeight;

}
