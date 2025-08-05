#include "stdafx.h"
#include ".\gcglobalvalue.h"
#include <dbg/dbg.hpp>
#include "IniReader.h"
#include "GCUtil.h"
#include "utf16_cvt.h"

extern void AppendSlash( std::string& str );
extern void AppendBackSlash( std::string& str );
extern bool CreateDirectoryA( const char* szPath_ );

KGCGlobalValue::KGCGlobalValue(void)
{
}

KGCGlobalValue::~KGCGlobalValue(void)
{
}

std::string KGCGlobalValue::m_strProgramPath = "NULL";
std::string KGCGlobalValue::m_strSrcPath = "NULL";
DWORD KGCGlobalValue::m_dwChannelType = 0;

int KGCGlobalValue::m_iPatchMode = 0;
int KGCGlobalValue::m_iGridDownState = 0;
char KGCGlobalValue::m_szCPCookie[20480]={0,};
DWORD KGCGlobalValue::m_dwPatcherStartTime = 0;

bool KGCGlobalValue::m_bPreTestPath = false;
bool KGCGlobalValue::m_bReDirectFTP = false;
bool KGCGlobalValue::m_bFirstRedirectFTP = false;
std::string KGCGlobalValue::m_strReDirectFTP = "";
std::string KGCGlobalValue::m_strPatchPathReDirect = PATCH_PATH_FILE;
bool KGCGlobalValue::m_bMultilingual = false;
int KGCGlobalValue::m_nLanguage = 0;
bool KGCGlobalValue::m_bLastUpdateLog = false;
bool KGCGlobalValue::m_bCheckDownSpeed = false;

bool KGCGlobalValue::m_bServerSelectMode = false;
std::map<int,SServerSelectInfo> KGCGlobalValue::m_mapServerInfo;
std::vector<SServerSelectInfo> KGCGlobalValue::m_vecServerInfo;

std::string KGCGlobalValue::m_strSelectIP = "";
std::string KGCGlobalValue::m_strSelectPort = "";
int KGCGlobalValue::m_iSelectServerIndex = 0;
std::wstring KGCGlobalValue::m_wstrSelectServerName = L"";
int KGCGlobalValue::m_iSelectServerState = 0;
bool KGCGlobalValue::m_bIsStoped = false;
std::string KGCGlobalValue::m_strDownLoadLink = "";
std::string KGCGlobalValue::m_strDownLoadLinkFileName = "";
std::string KGCGlobalValue::m_strVersion = "";
bool KGCGlobalValue::m_bVersionInfo = false;
std::string KGCGlobalValue::m_strMacAddress = "";

void KGCGlobalValue::Init()
{
	m_strProgramPath = "NULL";
	m_strSrcPath = "NULL";
	m_dwChannelType = 0;

	memset( m_szCPCookie, 0, sizeof ( char ) * 20480 ); 

	m_iPatchMode = 0;
	m_iGridDownState = 0;
	m_dwPatcherStartTime = 0;
#if defined ( _THAILAND ) 
	m_bReDirectFTP = true;
	m_strReDirectFTP = "gcauto.gc.in.th";
	m_strPatchPathReDirect = "PatchPathNewS3_RD.dat";
	KGCGlobalValue::m_bLastUpdateLog = true;
	KGCGlobalValue::m_bCheckDownSpeed = false;
#elif defined( _THAILAND_TEST ) 
	m_bReDirectFTP = true;
	m_strReDirectFTP = "122.155.3.170";
	m_strPatchPathReDirect = "PatchPathNewS3_RD.dat";
	KGCGlobalValue::m_bLastUpdateLog = true;
	KGCGlobalValue::m_bCheckDownSpeed = false;
#endif	

#if defined ( _EU_TEST ) || defined ( _EU )
	m_bMultilingual = true;
#endif


}

std::string KGCGlobalValue::GetProgramPath()
{
    if( !GetProgramPath_Nation() )
    {
        return "NULL";
    }

    //START_LOG( clog, m_strProgramPath.c_str() );

    // 경로 끝부분에 '\\'가 빠져 있다면 추가해준다.
    AppendBackSlash( m_strProgramPath );

    // 프로그램 경로 상에 있는 디렉토리들을 만들어낸다.
    CreateDirectoryA( m_strProgramPath.c_str() );

    return m_strProgramPath;
}

std::string KGCGlobalValue::GetSrcPath()
{
#if !defined( _TEST )
    std::string strSrcPath( PATCH_ORG_SRC );
#else
    std::string strSrcPath( GetDataFromINI( "Path" ) );    
#endif

	// 패치 서버 경로 끝부분에 '/'가 빠져 있다면 추가해준다.
	if( strSrcPath.compare( "NULL" ) != 0 )
		AppendSlash( strSrcPath );

	if ( !m_strDownLoadLink.empty() ) {
		strSrcPath = m_strDownLoadLink;
	}

    m_strSrcPath = strSrcPath;
    return m_strSrcPath;
}

bool KGCGlobalValue::IsGameInstalled( KRegistry* kReg_, const std::string& strRegKey_ )
{
    if ( false == kReg_->OpenKey( KRegistry::localMachine, (char*)strRegKey_.c_str() ) )
    {        
        return false;
    }
    return true;
}

bool KGCGlobalValue::GetGamePathFromReg( KRegistry* kReg_ , std::string& strPath_ )
{
    if( kReg_->GetValue( "DestDir", strPath_ ) == false )
    {
        ASSERT_MBOX( "Get Key Error : DestDir");
        return false;
    }
    return true;
}

bool KGCGlobalValue::GetPatcherPath( std::string& strPath_ )
{
    char strFullPath[MAX_PATH] = {0,};
    char szTempPath[MAX_PATH] = {0,};
    char szDirPath[MAX_PATH] = {0,};
    char szDrive[20] = {0,};

    ::GetModuleFileNameA( NULL, szTempPath, sizeof(szTempPath));
    _splitpath(szTempPath,szDrive,szDirPath,NULL,NULL);
    sprintf( strFullPath, "%s%s", szDrive, szDirPath );

    strPath_ = strFullPath;
    return true;
}

bool KGCGlobalValue::GetProgramPath_Nation()
{
#if defined( _KOREAN )
    return GetProgramPath_Korea();
#elif defined( _LEVELUP_BRAZIL )
    return GetProgramPath_Brazil();
#elif defined( _TAIWAN )
    return GetProgramPath_Taiwan();
#elif defined( _HONGKONG )
    return GetProgramPath_Hongkong();
#elif defined( _USA )
	return GetProgramPath_USA();
#elif defined(_THAILAND)
	return GetProgramPatch_Thailand();
#elif defined( _INDONESIA )
    return GetProgramPath_Indonesia();
#elif defined( _PHILIPPINE )
    return GetProgramPath_Philippine();
#elif defined( _LATINAMERICA )
    return GetProgramPath_LATINAMERICA();
#elif defined( _EU )
    return GetProgramPath_EU();
#elif defined( _CHINA )
	return GetProgramPath_CN();
#else
    return GetProgramPath_Test();
#endif
}

#if defined( _KOREAN )
bool KGCGlobalValue::GetProgramPath_Korea()
{
    KRegistry       kReg;
    START_LOG( cout, L"Program Start of Service mode!" );
    std::string strRegKey = std::string( REG_KEY ) + GAME_NAME + "\\";

    // 게임이 설치되어 있지 않다면 메시지를 출력하고 종료한다.
    if( IsGameInstalled( &kReg, strRegKey ) == true)
    {
        // 레지스트리 읽기에 실패하면 종료한다.
        if( GetGamePathFromReg( &kReg, m_strProgramPath ) == false )
        {
            START_LOG( cerr, L"Read Reg Fail!!!" );
            return false;
        }
    }
    else
    {
        START_LOG( cwarn, L"GrandChase is not installed" );
        ASSERT_MBOX( "레지스트리 읽기 실패 : "<<strRegKey );
        return false;
    }

    return true;
}
#endif

#if defined( _LEVELUP_BRAZIL )
bool KGCGlobalValue::GetProgramPath_Brazil()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _TAIWAN )
bool KGCGlobalValue::GetProgramPath_Taiwan()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _HONGKONG )
bool KGCGlobalValue::GetProgramPath_Hongkong()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _USA )
bool KGCGlobalValue::GetProgramPath_USA()
{
	return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _THAILAND )
bool KGCGlobalValue::GetProgramPatch_Thailand()
{
	return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _INDONESIA )
bool KGCGlobalValue::GetProgramPath_Indonesia()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _PHILIPPINE )
bool KGCGlobalValue::GetProgramPath_Philippine()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _LATINAMERICA )
bool KGCGlobalValue::GetProgramPath_LATINAMERICA()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif


#if defined( _EU )
bool KGCGlobalValue::GetProgramPath_EU()
{
    return GetPatcherPath( m_strProgramPath );
}
#endif

#if defined( _CHINA )
bool KGCGlobalValue::GetProgramPath_CN()
{
	return GetPatcherPath( m_strProgramPath );
}
#endif



#if defined( _TEST )
bool KGCGlobalValue::GetProgramPath_Test()
{
    return GetPatcherPath( m_strProgramPath );;
}
#endif

bool KGCGlobalValue::GetPatchPathFileName( std::string& strFileName_, bool bDirectFTP )
{
#if !defined( _TEST )
	std::string strSrcPath;
	if ( !bDirectFTP ) 
	{
		strSrcPath = PATCH_PATH_FILE;
	}
	else 
	{
		strSrcPath = m_strPatchPathReDirect;
	}
#else
	std::string strSrcPath( GetDataFromINI( "File" ) );    
#endif
	// 서버 리스트에서 선택된 서버의 링크를 남긴 부분이 있다면, 이 링크를 쓰도록 한다.
	if ( !m_strDownLoadLinkFileName.empty() ) {
		strFileName_ = m_strDownLoadLinkFileName;
		return true;
	}

	strFileName_ = strSrcPath;
	return true;
}

std::string KGCGlobalValue::GetPatchPathFileName( bool bDirectFTP )
{
#if !defined( _TEST )
	std::string strSrcPath;
	if ( !bDirectFTP ) 
	{
		strSrcPath = PATCH_PATH_FILE;
	}
	else 
	{
		strSrcPath = m_strPatchPathReDirect;
	}
#else
	std::string strSrcPath( GetDataFromINI( "File" ) );    
#endif	

#if defined( CUSTOM_DOWNLOAD_PATH )

#if defined( USE_SPARE_PARAMETER_FORM_COOKIE )
    if( KGCGlobalValue::m_bPreTestPath ) {
        //메시지 박스를 띠워서 Yes를 누르면 오픈테스트 패스 경로 파일을 지정하자
        if ( IDYES == MessageBox(NULL, L"Connect to Test Path?", L"Information", MB_YESNO|MB_ICONQUESTION) ) {
            strSrcPath = OPEN_PATCH_PATH_FILENAME;
            return GetSrcPath() + strSrcPath;
        }
    }
#endif

    char szTemp[1024] = {0,};
    {
        FILE* pFile = fopen("customPath.dat", "rt");
        if( pFile )
        {
            fgets(szTemp, 1024, pFile);
            fclose(pFile);
        }
    }

    if( strstr(szTemp, OPEN_PASSWORD) ) {
        //메시지 박스를 띠워서 Yes를 누르면 오픈테스트 패스 경로 파일을 지정하자
        if ( IDYES == MessageBox(NULL, L"Connect to Test Path?", L"Information", MB_YESNO|MB_ICONQUESTION) ) {
            strSrcPath = OPEN_PATCH_PATH_FILENAME;
        }
    } 

#endif
	if ( !m_strDownLoadLink.empty() && 	!m_strDownLoadLinkFileName.empty()) {
		return m_strDownLoadLink + m_strDownLoadLinkFileName;
	}

    return GetSrcPath() + strSrcPath;
}

bool KGCGlobalValue::GetPatchTimeFileName( std::string& strFileName_ )
{
#if !defined( _TEST )
    std::string strSrcPath( PATCH_TIME_CHECK );
#else
    std::string strSrcPath( GetDataFromINI( "File2" ) );    
#endif	
    strFileName_ = strSrcPath;
    return false;
}

std::string KGCGlobalValue::GetPatchTimeFileName()
{
#if !defined( _TEST )
    std::string strSrcPath( PATCH_TIME_CHECK );
#else
    std::string strSrcPath( GetDataFromINI( "File2" ) );    
#endif
    return GetSrcPath() + strSrcPath;
}

/// 패치경로 파일로부터 현재 버전의 패치 경로를 읽는다.
/// @param szSrcPath output parameter
/// @param strPatchPathFileName_ 패치경로 파일의 파일명
/// @return 성공하면 true, 실패하면 false
bool KGCGlobalValue::ReadPatchPathFile( char* szSrcPath , std::string& strPatchPathFileName_ )
{
    HANDLE hFile    = NULL;
    DWORD  dwRead   = 0;
    BYTE   curByte  = 0;
    int    iFeed    = -1;
    bool   bSuccess = false;

    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFileA( strPatchPathFileName_.c_str(),
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL ) ) )
    {
        return false;
    }

    while ( FALSE != ::ReadFile( hFile, &curByte, 1, &dwRead, NULL ) )
    {
        if ( 0 == dwRead ) break;

        switch ( curByte )
        {
        case '\n': case '\t': case ' ': case '\0':                 // skip
            break;
        case '<': if (-1 == iFeed) iFeed = 0; else goto Exit;      // begin
            break;
        case '>': if (-1 != iFeed) { bSuccess = true; goto Exit; } // end
                  break;
        default:  if (-1 != iFeed) szSrcPath[iFeed++] = curByte;   // read
            break;
        }
    }

Exit:
    ::CloseHandle( hFile );    
    return bSuccess;
}

/// 패치경로 파일로부터 현재 버전의 패치 경로를 읽는다.
/// @param strSrcPath output parameter
/// @return 성공하면 true, 실패하면 false
bool KGCGlobalValue::ReadPatchPathFile( std::string& strSrcPath , std::string& strPatchPathFileName_ )
{
    char szSrcPath[MAX_PATH] = { 0, };
    bool bRet  = ReadPatchPathFile( szSrcPath , strPatchPathFileName_);
    strSrcPath = szSrcPath;
    return bRet;
}

std::string KGCGlobalValue::GetBGWeb()
{
#ifndef BACKGROUND_URL2
#define BACKGROUND_URL2 BACKGROUND_URL
#endif
	std::string url;

#if defined(_KOREAN)
    switch( KGCGlobalValue::m_dwChannelType )
    {
    case 1:
        url =  BACKGROUND_URL2;
        break;
    case 2:
        url =  BACKGROUND_URL3;
        break;
    case 3:
        url =  BACKGROUND_URL4;
        break;
    default:
        url =  BACKGROUND_URL;
        break;
    }
#else
    if( KGCGlobalValue::m_dwChannelType == 0 )
        url =  BACKGROUND_URL;
    else
        url =  BACKGROUND_URL2;
#endif

#if defined( _TEST )
    url = GetDataFromINI( "Web" );
	if(url=="NULL")
		url = BACKGROUND_URL;
#endif

    if(m_bMultilingual){
        if(KGCGlobalValue::m_nLanguage == 0){
            url = "http://grandchase-eu.com:58888/game/launcher.aspx";
        }
        else if(KGCGlobalValue::m_nLanguage == 1){
            url = "http://grandchase-eu.com:58888/de/game/launcher.aspx";
        }
    }

	if ( m_bServerSelectMode ) {
		url = "http://gc.changyou.com/content/launcher/launcher2013.shtml";
	}
	
	return url;
}

std::string KGCGlobalValue::GetBGPWeb()
{
	std::string url;
#if defined( _TEST )
    url= GetDataFromINI( "PTimeWeb" );
	if(url=="NULL")
		url = BACKGROUND_PATCHTIME_URL;
#else
    url= BACKGROUND_PATCHTIME_URL;
#endif
	return url;
}

void KGCGlobalValue::PopUpWebPage( const WCHAR* strURL_ )
{
	//::PostMessage( hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );	
	ShellExecute(NULL, L"open", strURL_, NULL, NULL, SW_SHOWNORMAL);
}

std::string KGCGlobalValue::GetXTrapHttpUrlKey()
{
#if defined( _TEST)
	return GetDataFromINI( "XTrapKey" );
#else
	return XTRAP_HTTP_URL_KEY_PATCHER;
#endif
}

#if defined( _TEST )
std::string KGCGlobalValue::GetTestSrvIP()
{
    return GetDataFromINI( "TestIP" );
}

std::string KGCGlobalValue::GetTestSrvPort()
{
    return GetDataFromINI( "TestPort" );
}

std::string KGCGlobalValue::GetDataFromINI( std::string _strData )
{
    std::string strSrcPath;
    CIniReader cPatchIni( L".\\TestPatchServer.ini" );
    std::wstring strPath( cPatchIni.ReadString( L"WORLD_POSITION", L"Nation", L"NULL" ) );
    if( strPath.compare( L"NULL" ) != 0 )
    {   
        std::wstring strFile( cPatchIni.ReadString( (wchar_t*)strPath.c_str() , (wchar_t*)KncUtil::toWideString( _strData ).c_str() , L"NULL" ) );        
        if( strFile.compare( L"NULL" ) != 0 )
        {
            strSrcPath = GCUTIL_STR::GCStrWideToChar( strFile.c_str() );
        }
        else
        {
            strSrcPath = "NULL";
        }
    }
    else
    {
        strSrcPath = "NULL";
    }

    if( strSrcPath == "NULL" )
    {
        if( _strData.compare( "Web" ) == 0 )
        {
            return BACKGROUND_URL;
        }
        else if( _strData.compare( "Path" ) == 0 )
        {
            return PATCH_TEST_SRC;
        }
        else if( _strData.compare( "PTimeWeb" ) == 0 )
        {
            return BACKGROUND_PATCHTIME_URL;
        }
		else if(_strData.compare( "XTrapKey") == 0 )
		{
			return XTRAP_HTTP_URL_KEY_PATCHER;
		}
    }
    return strSrcPath;
}
#endif
void KGCGlobalValue::InitServerSelectInfo()
{
#if defined( _CHINA ) || defined(_CHINA_TEST)
	m_bServerSelectMode = true;
#else
	m_bServerSelectMode = false;
#endif

}

void KGCGlobalValue::AddServerInfo(std::wstring strInfo) {
	std::vector<std::wstring> vecString;
	
	std::wstring strOut;
	int iEndChar = 0;
	int iStartChar = 0;
	int iStrCount = strInfo.size();

	if (strInfo.find(L'\n') != std::wstring::npos )
		strInfo.erase(strInfo.find(L'\n'));
	if (strInfo.find(L'\r') != std::wstring::npos )
		strInfo.erase(strInfo.find(L'\r'));


	while( iEndChar != -1 ) {
		iEndChar = strInfo.find(L',');
		vecString.push_back(strInfo.substr(0,iEndChar));
		strInfo = strInfo.substr(iEndChar+1);
	}

	if ( vecString.size() < 15 ) return;

	SServerSelectInfo sTemp;
	sTemp.bNewServer = _wtoi( vecString[6].c_str() ) != 0;
	sTemp.bRecomServer = _wtoi( vecString[5].c_str() ) != 0;
	sTemp.iServerState = _wtoi( vecString[4].c_str() );
    if ( (int)vecString[14].size() > 1 )
        sTemp.iServiceType = 1;
    else
	    sTemp.iServiceType = _wtoi( vecString[14].c_str() );
	sTemp.wstrServerName = vecString[1];
	sTemp.strServerIP = vecString[8].substr(0,vecString[8].find(L':'));
	sTemp.strServerPort = vecString[8].substr(vecString[8].find(L':')+1);

	if ( vecString.size() < 16 ) {
		m_vecServerInfo.push_back(sTemp);
		return;
	}

	sTemp.strDownloadLink = vecString[15].substr(0,vecString[15].find_last_of(L'/')+1);
	sTemp.strDownloadLinkFile = vecString[15].substr(vecString[15].find_last_of(L'/')+1);
	m_vecServerInfo.push_back(sTemp);

}

std::wstring KGCGlobalValue::LoadVersionInfo()
{
	std::wstring strfilename =L"versioninfo.txt";

	std::wifstream  m_wFile;
	IMBUE_NULL_CODECVT( m_wFile );
	m_wFile.open( strfilename.c_str(), std::ios::binary | std::ios::in );


	if( !m_wFile.is_open() )
	{
		return L"";
	}
	wchar_t GarvStr[2048];
	m_wFile.read( GarvStr, 1 );

	std::wstring strOut;

	std::getline( m_wFile, strOut, L'\n' );
	m_bVersionInfo = true;

	return strOut;
}

bool KGCGlobalValue::LoadServerSelectInfo()
{
#if defined( _CHINA ) || defined(_CHINA_TEST)
	std::wstring strfilename =PATCH_SERVER_FILE_NAME_W;

	std::wifstream  m_wFile;
	IMBUE_NULL_CODECVT( m_wFile );
	m_wFile.open( strfilename.c_str(), std::ios::binary | std::ios::in );


	if( !m_wFile.is_open() )
	{
		std::cout << "can't file open" << std::endl;
		return false;
	}
	wchar_t GarvStr[2048];
	m_wFile.read( GarvStr, 1 );

	std::wstring strOut;

	while( std::getline( m_wFile, strOut, L'\n' ) ) 
	{
		if( strOut.empty() )
			continue;

		AddServerInfo(strOut);
	}

	if ( m_vecServerInfo.empty() )
		return false;

	m_bServerSelectMode = true;
#endif
	return true;
}

bool KGCGlobalValue::IsStoped()
{
	return m_bIsStoped;
}

void KGCGlobalValue::GetMacAddress( )
{
	PIP_ADAPTER_INFO Info;
	DWORD size = sizeof(PIP_ADAPTER_INFO);
	ZeroMemory( &Info, size );
	int result = GetAdaptersInfo( Info, &size );        // 첫번째 랜카드 MAC address 가져오기
	if (result == ERROR_BUFFER_OVERFLOW)    // GetAdaptersInfo가 메모리가 부족하면 재 할당하고 재호출
	{
		Info = (PIP_ADAPTER_INFO)malloc(size);
		GetAdaptersInfo( Info, &size );
	}
	
	if ( Info  == NULL) return;

	char strMac[256];
	ZeroMemory( strMac, 256 );
	sprintf(strMac, "%0.2X-%0.2X-%0.2X-%0.2X-%0.2X-%0.2X", 
		Info->Address[0], Info->Address[1], Info->Address[2], Info->Address[3], Info->Address[4], Info->Address[5] );

	m_strMacAddress = strMac;
}




