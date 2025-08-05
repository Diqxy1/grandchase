#include "stdafx.h"
#include "globalfunction.h"
#include "GCDllLoader.h"
#include "gcutil.h"
#include "KStateInfo.h"
#include "gcglobalvalue.h"
#include "../GCUtil/GenericHTTPClient.h"
#include "../GCFTP.h"
#include "windows.h"
#include <DSetup.h>
#include <ShlObj.h>


#pragma comment( lib, "Dsetup.lib" )


extern void GetCurrentPath( std::string& strPath_ );
extern void* GetLibraryFunction( std::string strFileName_ , std::string strFunc_ , HINSTANCE hInst );


LPVOID pCrashRptState = NULL;
LPVOID (*BT_InstallSehFilter)();
LPVOID (*BT_SetAppName)(LPCTSTR);        
LPVOID (*BT_SetAppVersion)(LPCTSTR);        
LPVOID (*BT_SetFlags)(DWORD);        
LPVOID (*BT_SetSupportServer)(LPCTSTR , SHORT);        
LPVOID (*BT_CallCppFilter)();        
LPVOID (*BT_SetPreErrHandler)(BT_ErrHandler , INT_PTR);        
LPVOID (*BT_SetActivityType)(BUGTRAP_ACTIVITY);        


// 로그 모음 벡터.. 다운로드를 일괄적으로 멀티쓰레드로 받으니 로그가 밀려서
// 어디서 다운로드 받았는지 곤란한 부분이 많아 모두 담아 두었다가 일괄적으로 보낸다.
std::string g_Msg;
std::vector<std::string> g_strLogs;

INT_PTR g_logFile;
KncCriticalSection csLog;

enum ReportType
{
    RT_ERROR = 0,
    RT_PATCHEND,
    RT_MANUAL_PATCHEND,
};


void Send_ErrorReport(std::string strParseMsg, std::string strLastMsg, std::string strCapacity, int iType )
{
    // 래진씨가 만든 패쳐로그 시스템,,,, 갑자기 이상한동작을 해서 사용을 안합니다.
    return;

    GenericHTTPClient http;
    std::string strPostMsg = (std::string)"http://gc_loggetter.kog.co.kr:7070/default_ver2.aspx";
    std::string strTitle = GCUTIL_STR::GCStrWideToChar(NATION_CODE);

#if defined(__OPEN_TEST__)
    strTitle += "[opentest]";	
#elif defined(_KOREA_INTERNAL_)
    strTitle += "[internal]";
#endif

    std::string strMsg="";
    char szTemp[256];

    // 로그중에 특정 문자열이 들어가 있는 녀석들은 다 포함시켜 준다.
    for (int i =0 ; i < g_strLogs.size() ; i++)
    {
        if (g_strLogs[i].find(strParseMsg) != std::string::npos)
        {
            strMsg+=g_strLogs[i];
        }
    }

    g_Msg = g_Msg + "\n" + strMsg;	

    if (g_Msg.length()>1500)
    {
        g_Msg=g_Msg.substr(g_Msg.length()-1500,1500);
    }

    if (iType == RT_ERROR)
    {
        http.AddPostArguments("ty","RT_ERROR");
        http.AddPostArguments("cd",strTitle.c_str());
        http.AddPostArguments("pm",strParseMsg.c_str());
        http.AddPostArguments("lm",strLastMsg.c_str());
        http.AddPostArguments("mg",g_Msg.c_str(),true);
        http.AddPostArguments("ip","None"); // CDN 서버 IP 주소
        http.AddPostArguments("ov","OldVersion");    // 구버젼
        http.AddPostArguments("nv","NewVersion");    // 새버젼
    }
    else //패치 종료 패킷인 경우.
    {
        strLastMsg = strCapacity; 
        http.AddPostArguments("ty", iType == RT_PATCHEND ? "RT_PATCHEND" : "RT_MANUAL_PATCHEND");
        http.AddPostArguments("cd", strTitle.c_str());          // 국가코드
        http.AddPostArguments("pm", strParseMsg.c_str());       // -- 성공 실패 여부
        http.AddPostArguments("lm", strLastMsg.c_str());	    // -- 패치용량.
        http.AddPostArguments("sz", (timeGetTime() - KGCGlobalValue::m_dwPatcherStartTime)/60000);       // 다운로드 시간
        http.AddPostArguments("ip", "None");                    // CDN 서버 IP 주소
        http.AddPostArguments("ov", "OldVersion");              // 구버젼
        http.AddPostArguments("nv", "NewVersion");              // 새버젼
    }

    BOOL bOK = http.Request(strPostMsg.c_str(),GenericHTTPClient::RequestPostMethod);
}


void WriteLogFile(std::string strMsg,bool bNewFile = false)
{
	KLocker lock(csLog);

	CTime ctime;


	if (bNewFile == true)
	{
		g_Msg = "";
		g_strLogs.clear();
	}

	//char temp[200];

	//로그를 남긴는 부분 대폭 정리 나중에 클라이언트 크러쉬에 암호화 하여 남길수도 있어서 일단 주석 처리한다.
	//std::string strTitle = GCUTIL_STR::GCStrWideToChar(NATION_CODE);
	//#if !defined(__PATH__)
	//	strTitle += "[internal]";
	//#endif	
	//strcpy_s(strMsgChar,strMsg.c_str());
	//for (int i=0;i<strMsg.size();i++)
	//{
	//	strMsgChar[i]=(strMsgChar[i] ^ ENCRYP_XOR_NUM) + ENCRYP_ENT_NUM ;
	//	//strMsgChar[i]=(strMsgChar[i] - ENCRYP_ENT_NUM) ^ ENCRYP_XOR_NUM;
	//	//sprintf(temp,"%d/",strMsgChar[i]);
	//	//g_Msg +=temp;
	//}

	ctime=CTime::GetCurrentTime();
	std::string strTime=GCUTIL_STR::GCStrWideToChar(ctime.Format(L" [%Y-%m-%d//%H:%M:%S] ").GetString());
	strMsg=strTime+strMsg;

	//SendReport로 쏘는 메시지는 암호화 하지 않는다.
	if (bNewFile==false)
	{
		g_Msg = g_Msg + "\n" + strMsg;
		g_strLogs.push_back("\n"+strMsg);
	}

	if (g_Msg.length()>1500)
	{
		g_Msg=g_Msg.substr(g_Msg.length()-1500,1500);
	}
}

void CALLBACK CrashCallback( INT_PTR nErrHandlerParam )
{
	try
	{
		std::string strFileName;
		GetCurrentPath(strFileName);
		strFileName=strFileName+=LOG_FILE_NAME;

		LPVOID (*BT_AddLogFile)(LPCTSTR);        
		if( (BT_AddLogFile = (LPVOID (*)(LPCTSTR))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_AddLogFile" )) != NULL )
		{
			pCrashRptState = (*BT_AddLogFile)(GCUTIL_STR::GCStrCharToWide(strFileName.c_str()));
		}
	}
	catch (...)
	{

	}

	return;
}

void BugTrap_InstallSehFilter()
{
	if( (BT_InstallSehFilter = (LPVOID (*)())KGCDllLoader::CallFunction( "BugTrap.dll", "BT_InstallSehFilter" )) != NULL )
	{
		pCrashRptState = (*BT_InstallSehFilter)();
	}
}

void InstallCrashReport()
{       
	if( (BT_InstallSehFilter = (LPVOID (*)())KGCDllLoader::CallFunction( "BugTrap.dll", "BT_InstallSehFilter" )) != NULL )
	{
		pCrashRptState = (*BT_InstallSehFilter)();
	}

	std::wstring strTitle = NATION_CODE;
#if defined(_TEST)
	strTitle += L" internal ";
#endif	

	if( (BT_SetAppName = (LPVOID (*)( LPCTSTR))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_SetAppName" )) != NULL )
	{
#if defined(JOB_NAME )
		(*BT_SetAppName)(JOB_NAME);
#else
		(*BT_SetAppName)(strTitle.c_str());
#endif
	}


	if( (BT_SetAppVersion = (LPVOID (*)( LPCTSTR))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_SetAppVersion" )) != NULL )
	{
#if defined(BUILD_ID)
		(*BT_SetAppVersion)( BUILD_ID );
#else
		(*BT_SetAppVersion)( BUILD_VERSION );
#endif	
	}


	if( (BT_SetFlags = (LPVOID (*)(DWORD))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_SetFlags" )) != NULL )
	{
		(*BT_SetFlags)(BTF_DETAILEDMODE | BTF_ATTACHREPORT | BTF_SCREENCAPTURE | BTF_LISTPROCESSES );
	}

	if( (BT_SetSupportServer = (LPVOID (*)(LPCTSTR , SHORT))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_SetSupportServer" )) != NULL )
	{
		(*BT_SetSupportServer)(L"Crash-getter.kog.co.kr", 9999 );
	}

	if( (BT_CallCppFilter = (LPVOID (*)())KGCDllLoader::CallFunction( "BugTrap.dll", "BT_CallCppFilter" )) != NULL )
	{
		set_terminate(reinterpret_cast<terminate_function>(*BT_CallCppFilter));
	}

	if( (BT_SetPreErrHandler = (LPVOID (*)(BT_ErrHandler , INT_PTR ))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_SetPreErrHandler" )) != NULL )
	{
		(*BT_SetPreErrHandler)(CrashCallback, 0 );
	}

	if( (BT_SetActivityType = (LPVOID (*)(BUGTRAP_ACTIVITY))KGCDllLoader::CallFunction( "BugTrap.dll", "BT_SetActivityType" )) != NULL )
	{
		(*BT_SetActivityType)(BTA_SENDREPORT);
	}
	return;
}


void LoadBitmapFromMassFile( Komfile * pMassFile,std::string strFileName_, CBitmap** ppBitmap_ )
{
	/*Komfile::MASSFILE_MEMBERFILEINFO kMassMemberFile;
	kMassMemberFile.dwFlag = MFI_REALDATA;

	if(pMassFile->GetMemberFile( strFileName_.c_str(), &kMassMemberFile ) == false )
	{
		START_LOG( cerr, strFileName_ << " load Failed" );
	}

	LPBYTE pBuffer = (LPBYTE)kMassMemberFile.pRealData;
	UINT uiSize_ = (UINT)kMassMemberFile.ulSize;
	UINT uiOffset= 0;
	*/

	KSubfile subfile = pMassFile->GetSubfile(strFileName_);
	
	UINT uiSize_ = subfile.GetSize();	
	if ( uiSize_ == 0 ) return;
	boost::scoped_array<char> pBuffer(new char[uiSize_]);
	subfile.WriteDecompressed(pBuffer.get());
	CxImage image((BYTE*)pBuffer.get(),uiSize_,CXIMAGE_FORMAT_BMP);

	HBITMAP hBitmap=image.MakeBitmap();
	*ppBitmap_ = CBitmap::FromHandle( hBitmap );
}



bool CheckDXVersion()
{
	bool bRet = false;
	DWORD dwVersion, dwRevision;
    if( DirectXSetupGetVersion(&dwVersion, &dwRevision) )
	{
        if( dwVersion >= 0x00040009 )
        {
            if( dwRevision >= 0x00000388 )
                bRet = true;
        }
	}

    if( false == bRet )
    {
        char szTemp[1024];
        DWORD dwExitCode = 0;
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        GetCurrentDirectoryA( 1024, szTemp);
        strcat(szTemp, "\\dxwebsetup.exe");
        memset(&si, 0, sizeof(STARTUPINFOA));
        si.cb = sizeof(si);

        if( CreateProcessA( NULL, szTemp, NULL, NULL, FALSE, NULL, NULL, NULL, (LPSTARTUPINFOA)&si, &pi) )
        {
            bRet = true;
            do
            {
                GetExitCodeProcess( pi.hProcess, &dwExitCode);
            }while( STILL_ACTIVE == dwExitCode );
            CloseHandle(pi.hProcess);   
            CloseHandle(pi.hThread);
        }
    }

	return bRet;
}



std::string GetSystemFolderPath( int csidl ) {

    char desktop_path[MAX_PATH] = "";

    SHGetSpecialFolderPathA(NULL,desktop_path,csidl,0);

    return desktop_path;
}