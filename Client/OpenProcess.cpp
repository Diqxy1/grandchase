#include "stdafx.h"
//

#ifdef UNICODE
#   undef UNICODE
#   include <tlhelp32.h>
#   define UNICODE
#else
#   include <tlhelp32.h>
#endif
#include <io.h>
//
#include "MyD3D.h"
#include "OpenProcess.h"



#include "GlobalVariables.h"        // This file should be included last.

extern bool RecheckFileSHA();

KDLLChecker* KDLLChecker::m_pInstace = NULL;

KDLLChecker::KDLLChecker()
{
    SetDllList();

    m_bSnapCheck = false;
    m_bCheckedMainName = false;
	m_bIsNT = false;
	m_hPsapiDLL = NULL;

}
KDLLChecker::~KDLLChecker()
{
    m_setDllList.clear();
    m_setLoadList.clear();

    SAFE_DELETE( m_pInstace );
}


void KDLLChecker::SetDllList()
{

    m_setDllList.insert("ntdll.dll");			m_setDllList.insert("kernel32.dll");    m_setDllList.insert("KERNELBASE.dll");
    m_setDllList.insert("advapi32.dll");		m_setDllList.insert("msvcrt.dll");		m_setDllList.insert("sechost.dll");
    m_setDllList.insert("RPCRT4.dll");			m_setDllList.insert("user32.dll");		m_setDllList.insert("GDI32.dll");
    m_setDllList.insert("LPK.dll");				m_setDllList.insert("USP10.dll");		m_setDllList.insert("shell32.dll");
    m_setDllList.insert("SHLWAPI.dll");			m_setDllList.insert("comdlg32.dll");	m_setDllList.insert("COMCTL32.dll");
    m_setDllList.insert("winmm.dll");			m_setDllList.insert("winspool.drv");	m_setDllList.insert("oleacc.dll");
    m_setDllList.insert("MpaDecFilter.ax");		m_setDllList.insert("ole32.dll");		m_setDllList.insert("d3d9.dll");
    m_setDllList.insert("VERSION.dll");			m_setDllList.insert("d3d8thk.dll");		m_setDllList.insert("dwmapi.dll");			
	m_setDllList.insert("dinput8.dll");			m_setDllList.insert("dsound.dll");		m_setDllList.insert("POWRPROF.dll");
    m_setDllList.insert("SETUPAPI.dll");		m_setDllList.insert("CFGMGR32.dll");	m_setDllList.insert("OLEAUT32.dll");
    m_setDllList.insert("DEVOBJ.dll");			m_setDllList.insert("imm32.dll");		m_setDllList.insert("MSCTF.dll");
    m_setDllList.insert("ws2_32.dll");			m_setDllList.insert("NSI.dll");			m_setDllList.insert("zlib1.dll");
    m_setDllList.insert("mailmime.dll");		m_setDllList.insert("MFC42.DLL");		m_setDllList.insert("ODBC32.dll");
    m_setDllList.insert("mailsmtp.dll");		m_setDllList.insert("WSOCK32.dll");		m_setDllList.insert("kncfirewall.dll");
    m_setDllList.insert("bugtrap.dll");			m_setDllList.insert("WININET.dll");		m_setDllList.insert("urlmon.dll");
    m_setDllList.insert("CRYPT32.dll");			m_setDllList.insert("MSASN1.dll");		m_setDllList.insert("iertutil.dll");
    m_setDllList.insert("libcurl.dll");			m_setDllList.insert("WLDAP32.dll");		m_setDllList.insert("dbghelp.dll");
    m_setDllList.insert("iphlpapi.dll");		m_setDllList.insert("WINNSI.DLL");		m_setDllList.insert("odbcint.dll");
    m_setDllList.insert("uxtheme.dll");			m_setDllList.insert("CRYPTSP.dll");		m_setDllList.insert("rsaenh.dll");
    m_setDllList.insert("CRYPTBASE.dll");		m_setDllList.insert("apphelp.dll");		m_setDllList.insert("npggNT.des");
    m_setDllList.insert("npsc.des");			m_setDllList.insert("unicows.dll");		m_setDllList.insert("MPR.dll");
    m_setDllList.insert("oledlg.dll");			m_setDllList.insert("AVICAP32.dll");	m_setDllList.insert("MSVFW32.dll");
    m_setDllList.insert("IMKR14.IME");			m_setDllList.insert("MSVCR90.dll");		m_setDllList.insert("MSVCP90.dll");
    m_setDllList.insert("NateOnHook40u.dll");	m_setDllList.insert("CLBCatQ.DLL");		m_setDllList.insert("IMKRAPI.DLL");
    m_setDllList.insert("IMJKAPI.DLL");			m_setDllList.insert("nvd3dum.dll");		m_setDllList.insert("HID.DLL");
    m_setDllList.insert("WINTRUST.dll");		m_setDllList.insert("mswsock.dll");		m_setDllList.insert("wshtcpip.dll");
    m_setDllList.insert("comctl32.DLL");		m_setDllList.insert("SspiCli.dll");		m_setDllList.insert("profapi.dll");
    m_setDllList.insert("dnsapi.DLL");			m_setDllList.insert("MMDevApi.dll");	m_setDllList.insert("PROPSYS.dll");
    m_setDllList.insert("AUDIOSES.DLL");		m_setDllList.insert("FirewallAPI.dll");	m_setDllList.insert("IMKRTIP.DLL");
    m_setDllList.insert("IMETIP.DLL");			m_setDllList.insert("quartz.dll");		m_setDllList.insert("devenum.dll");
    m_setDllList.insert("ntmarta.dll");			m_setDllList.insert("msdmo.dll");		m_setDllList.insert("DShowRdpFilter.dll");
    m_setDllList.insert("WTSAPI32.dll");		m_setDllList.insert("WINSTA.dll");		m_setDllList.insert("slc.dll");
    m_setDllList.insert("qasf.dll");			m_setDllList.insert("mp3dmod.dll");		m_setDllList.insert("mfplat.DLL");
    m_setDllList.insert("AVRT.dll");			m_setDllList.insert("MSACM32.DLL");		m_setDllList.insert("l3codeca.acm");

    m_setDllList.insert("msmpeg2adec.dll");		m_setDllList.insert("wdmaud.drv");		m_setDllList.insert("ksuser.dll");
    m_setDllList.insert("msacm32.drv");			m_setDllList.insert("midimap.dll");		m_setDllList.insert("NLAapi.dll");
    m_setDllList.insert("winrnr.dll");			m_setDllList.insert("napinsp.dll");		m_setDllList.insert("pnrpnsp.dll");
    m_setDllList.insert("WLIDNSP.DLL");			m_setDllList.insert("PSAPI.DLL");		m_setDllList.insert("fwpuclnt.dll");
    m_setDllList.insert("rasadhlp.dll");		m_setDllList.insert("dxdiagn.dll");		m_setDllList.insert("dxgi.dll");
    m_setDllList.insert("d3d10.dll");			m_setDllList.insert("d3d10core.dll");	m_setDllList.insert("d3d10_1.dll");


    m_setDllList.insert("d3d10_1core.dll");		m_setDllList.insert("d3d11.dll");		m_setDllList.insert("wbemprox.dll");
    m_setDllList.insert("wbemcomn.dll");		m_setDllList.insert("RpcRtRemote.dll");	m_setDllList.insert("wbemsvc.dll");
    m_setDllList.insert("fastprox.dll");		m_setDllList.insert("NTDSAPI.dll");		m_setDllList.insert("winbrand.dll");

    m_setDllList.insert("msimg32.dll");			m_setDllList.insert("Normaliz.dll");	m_setDllList.insert("RASAPI32.dll");
    m_setDllList.insert("rasman.dll");			m_setDllList.insert("rtutils.dll");		m_setDllList.insert("sensapi.dll");
    m_setDllList.insert("wship6.dll");			m_setDllList.insert("D3DREF9.DLL");		m_setDllList.insert("d3dx9_42.dll");

	m_setDllList.insert("AUTHZ.dll");			m_setDllList.insert("AcLayers.DLL");	m_setDllList.insert("Cabinet.dll");
	m_setDllList.insert("DEVRTL.dll");			m_setDllList.insert("DWrite.dll");		m_setDllList.insert("GPAPI.dll");
	m_setDllList.insert("MLANG.dll");			m_setDllList.insert("MSHTML.dll");		m_setDllList.insert("Secur32.dll");

}



void KDLLChecker::CheckDll()
{
	OSVERSIONINFO   OSversionInfo;
	OSversionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &OSversionInfo );

	//  윈도우 버젼이 NT계열이 아니면 "psaip.h"를 사용할수 없다.
	if( VER_PLATFORM_WIN32_NT == OSversionInfo.dwPlatformId )
	{
		m_bIsNT = true;
		m_hPsapiDLL = ::LoadLibraryA( "psapi.dll" );
	}
 

	GetDLLToProcess();
	if (m_bIsNT)
	{
		GetDllByPsapi();
		
	}

}

void KDLLChecker::GetDllByPsapi()
{   
	if (m_hPsapiDLL)
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ , FALSE , GetCurrentProcessId());

		if (hProcess == INVALID_HANDLE_VALUE)
			return;

		typedef BOOL  (WINAPI *pEnumProcessModules)( HANDLE, HMODULE*, DWORD, LPDWORD );
		typedef DWORD  (WINAPI *pGetModuleBaseNameA)( HANDLE, HMODULE, LPSTR, DWORD );

		pEnumProcessModules     DllFunc_EnumProcessModules  = (pEnumProcessModules)GetProcAddress(m_hPsapiDLL,"EnumProcessModules");
		pGetModuleBaseNameA     DllFunc_GetModuleBaseNameA  = (pGetModuleBaseNameA)GetProcAddress(m_hPsapiDLL,"GetModuleBaseNameA");


		if ( DllFunc_EnumProcessModules && DllFunc_GetModuleBaseNameA)
		{
			HMODULE hModule[1024];
			DWORD cb;

			BOOL bRet = DllFunc_EnumProcessModules(hProcess ,hModule , sizeof(hModule) , &cb );

			if (bRet != NULL)
			{
				int nModule = cb/sizeof(HMODULE);
				for (int i=1 ; i<nModule ; i++)
				{
					char strModuleName[MAX_PATH] = {0,};

					DllFunc_GetModuleBaseNameA(hProcess,hModule[i],strModuleName , sizeof(strModuleName));

					std::string strModule = strModuleName;

					std::set<std::string>::iterator vecIter;
					for (vecIter = m_setDllList.begin() ; vecIter !=m_setDllList.end() ; ++vecIter)
					{
						if (stricmp(vecIter->c_str(),strModule.c_str()) == 0)
							break;
					}

					if (vecIter == m_setDllList.end())
					{
						std::set<std::string>::iterator setIter = m_setNoList.begin();
						for (;setIter != m_setNoList.end() ; ++setIter)
						{
							if ( (*setIter) == strModule )
								break;
						}
						if (setIter == m_setNoList.end())
						{
							m_setNoListBypsapi.insert(strModule);
							CheckBlackDllByBlackList(strModule);
						}
					}

				}
			}

		}
		
		CloseHandle(hProcess);
	}  
}

void KDLLChecker::GetDLLToProcess()
{

    // 모듈 정보 구조체
    MODULEENTRY32 ModuleEntry = { sizeof(ModuleEntry) };
    // CreateToolhelp32Snapshot를 이용하여 프로세스에 로드된 DLL정보가 저장된 메모리의 핸들을 얻어냄.
    HANDLE  hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId());
    // DLL리스트 중 가장 첫번째 항목으로 이동( 실패일 경우 스냅샷 핸들을 반환하고 종료)
    if( !Module32First( hSnapShot , &ModuleEntry ) )
    {
        goto END_DLLFUNC;
    }

    while( Module32Next( hSnapShot, &ModuleEntry ) )
    {

        //m_setLoadList.insert(ModuleEntry.szModule);
        /////////////////////////////////////////////////////////////////////////

        std::set<std::string>::iterator vecIter;
        for (vecIter = m_setDllList.begin() ; vecIter !=m_setDllList.end() ; ++vecIter)
        {
            if (stricmp(vecIter->c_str(),ModuleEntry.szModule) == 0)
            {
                break;
            }
        }

        if (vecIter == m_setDllList.end())
        {
            m_bSnapCheck = true;
            std::string strTmp ; 
            strTmp = ModuleEntry.szModule;

            m_setNoList.insert(strTmp);

            //CheckBlackDllBypattern( strTmp );
            CheckBlackDllByBlackList( strTmp );

            
//             FILE *fp = fopen("DllList.txt","w");
//             if (fp == NULL)
//                 continue;
//             fputs(ModuleEntry.szModule , fp);
// 
//             fclose(fp);
            
      
        }

    }

END_DLLFUNC:
    CloseHandle( hSnapShot );
}

void KDLLChecker::CheckBlackDllByBlackList(std::string strDll_)
{
    USES_CONVERSION;

    std::wstring strTmp = std::wstring(A2W( strDll_.c_str() ));

	transform(strTmp.begin() , strTmp.end(),strTmp.begin() , tolower);

    std::set<std::wstring>::iterator setIter;

    std::set<std::wstring> SetBlackDllList = g_kGlobalValue.m_setBlackDllList;


	std::wstring strTmpInCon;
    for (setIter = SetBlackDllList.begin() ; setIter != SetBlackDllList.end() ; ++setIter)
    {
		strTmpInCon = *setIter;
		transform(strTmpInCon.begin() , strTmpInCon.end() , strTmpInCon.begin() , tolower);
		if (strTmp == strTmpInCon)
        {
			g_kGlobalValue.m_strCheckingBlackDll = strDll_;
			SettingRandShutdownCount(1000);
            return;

        }
    }

}

void KDLLChecker::CheckBlackDllBypattern(std::string strDll_)
{
    USES_CONVERSION;

    std::wstring strTmp = std::wstring(A2W(strDll_.c_str()));

    if (strTmp.find(L".cf") != std::wstring::npos )
    {
        SettingRandShutdownCount(100);
        return;
    }

}

void KDLLChecker::SettingRandShutdownCount(int iAddFrameValue_)
{
    g_kGlobalValue.m_bHackCheck = true;
}

bool KDLLChecker::HaveHiddenDll()
{
	if (m_setNoListBypsapi.empty())
		return false;

	return true;

}

void KDLLChecker::CheckProcessList()
{


	PROCESSENTRY32 ProcessEntry;

	ProcessEntry.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if( !Process32First( hSnapShot, &ProcessEntry ) )
	{
		goto END_PROFUNC;
	}

	do 
	{
		CheckBlackDllByBlackList(ProcessEntry.szExeFile);
		
	} while ( Process32Next(hSnapShot, &ProcessEntry) );

	// 스냅샷 핸들반환
END_PROFUNC:
	CloseHandle( hSnapShot );    

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

boost::shared_ptr<KSHAChecker> KSHAChecker::m_pInstace( static_cast<KSHAChecker*>(0) );

KSHAChecker::KSHAChecker(){
m_hQuery = NULL;
m_hCounter = NULL;

m_bWork = false;
m_bON = false;

m_lHighestCPUUsage = 0;
m_lLowestCPUUsage = LONG_MAX;
m_lStandardCPUUsage = 0;
m_lCurrenCPUUsage = 0;

m_lCntLimitTime = 0;
}

KSHAChecker::~KSHAChecker(){

}

void KSHAChecker::DeInit()
{
    if (m_hQuery) 
        PdhCloseQuery(&m_hQuery);

    
}

bool KSHAChecker::Init()
{
    DeInit();

    PDH_STATUS status;

    status = PdhOpenQuery(0,0, &m_hQuery);
    if ( status != ERROR_SUCCESS) {
        return false;
    }
   
    status = PdhAddCounter(m_hQuery , _T("\\Processor(_Total)\\% Processor Time"), 0, &m_hCounter);
    if (status != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

void KSHAChecker::GetCPUUsage()
{
    PDH_STATUS status;

    PDH_FMT_COUNTERVALUE PDHvalue;

    status = PdhCollectQueryData(m_hQuery);
    if(status != ERROR_SUCCESS) {
        m_lCurrenCPUUsage = FAIL_GET_CPUUSAGE;
        return;
    }
    status = PdhGetFormattedCounterValue(m_hCounter , PDH_FMT_LONG , 0 , &PDHvalue);
    if (status != ERROR_SUCCESS) {
        m_lCurrenCPUUsage = FAIL_GET_CPUUSAGE;
        return;
    }

    m_lCurrenCPUUsage = PDHvalue.longValue;

    if (m_lHighestCPUUsage < PDHvalue.longValue)
        m_lHighestCPUUsage = PDHvalue.longValue;

    if (m_lLowestCPUUsage > PDHvalue.longValue)
        m_lLowestCPUUsage = PDHvalue.longValue;

    
    m_lStandardCPUUsage = ((m_lHighestCPUUsage + m_lLowestCPUUsage)>>2) * 3;

}

void KSHAChecker::RecordAndSendErrUserInfo()
{
    try
    {
        std::wstringstream stm;
        stm<<"==================================================================================\n";
        stm<<"Login ID     : "<<GCUTIL_STR::GCStrWideToChar( g_kGlobalValue.m_kUserInfo.strLogin.c_str() )<<"\n";
        stm<<"UserNickName : "<<GCUTIL_STR::GCStrWideToChar( g_kGlobalValue.m_kUserInfo.strNickName.c_str() )<<"\n";
        stm<<"Current Char : "<<g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType<<"\n";
        stm<<"==================================================================================\n"; 

        //SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_NOT_EQUAL_QUEST_INFO, stm.str() );
    }
    catch (...)
    {
    	
    }

    
}

void KSHAChecker::CheckSHA()
{
    if ( m_lCntLimitTime > LIMIT_CHECK_TIME )
        goto Mustdo;

    //CUP 사용량 정보를 얻어오지 못했거나 기준에 맞지 않는 경우 && 
    //임계치 시간이 넘었을 경우
    if ( m_lCurrenCPUUsage == FAIL_GET_CPUUSAGE || m_lCurrenCPUUsage > m_lStandardCPUUsage )
        return;


Mustdo:
    if (RecheckFileSHA()==false){
        RecordAndSendErrUserInfo();
        ExitWithHackAlert( L"ScriptHackDetect()" );
    }

    m_lCntLimitTime = 0;

}

void KSHAChecker::WorkFunc()
{
    GetCPUUsage();

    if (m_bWork)
        CheckSHA();

}

void KSHAChecker::ThreadFunc()
{
/////////////////초기화//////////////////////////////////////////////////////
    int nFailCnt = 0;
    while ( !Init() ) {
        nFailCnt++;

        //실패횟수가 일정 횟수를 넘어가면 함수를 끝내버린다.
        if ( nFailCnt > LIMIT_INIT_FAIL )
            return;
        
        //실패할때마다 10초 쉬고 재시도한다.
        boost::this_thread::sleep(boost::posix_time::seconds(10));
    }
//////////////////////////////////////////////////////////////////////////////

    while(1) { 

        //던전때만 체크
        if ( g_MyD3D->m_pStateMachine ){

            if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME ){
                KP2P::GetInstance()->Send_EventSHAEnableReq();

                if (m_bON)
                    WorkFunc();
            }

        }
        //한번 체크하고 일정 시간동안 쉰다.
        boost::this_thread::sleep_for(boost::chrono::seconds(REST_TIME));
    }
    

    DeInit();

    if (m_hCounter) 
        PdhRemoveCounter(m_hCounter);

}

void KSHAChecker::StartThread()
{
    if (m_pThread.get() == NULL)
        m_pThread.reset( new boost::thread( boost::bind(&KSHAChecker::ThreadFunc , this) ) ); 

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

boost::shared_ptr<KHackCheckManager> KHackCheckManager::m_pInstace( static_cast<KHackCheckManager*>(0) );

KHackCheckManager::KHackCheckManager(){

}

KHackCheckManager::~KHackCheckManager(){

}

void KHackCheckManager::BeforeCheckApRatio(KProtectedType<float> *pfIncApRatio_ , float *pfOriginValue)
{
#if defined(USE_HACK_CHECK_MANAGER)
	m_fOriginValue = *pfOriginValue;
	//보통 3배 이상은 되지 않는다.
	if ( *pfIncApRatio_ > LIMIT_INC_DAMAGE )
	{
		g_kGlobalValue.m_bDamageHack = true;
		g_kGlobalValue.m_fBefore = *pfIncApRatio_;

	#if defined(__PATH__)
		*pfIncApRatio_ = 1.0f;
	#else
		ExitWithHackAlert(L"DamageHack");
	#endif
	}
#endif	
	
}

void KHackCheckManager::AfterCheckApRatio(float* pfResult_)
{
#if defined(USE_HACK_CHECK_MANAGER)
	float fOrigine = (*pfResult_)/m_fOriginValue;

	if ( fOrigine > LIMIT_INC_DAMAGE )
	{

		g_kGlobalValue.m_bDamageHack = true;

		g_kGlobalValue.m_fAter = fOrigine;
#if !defined(__PATH__)
		MessageBox(NULL,L"DamageHack",L"err",NULL);
#endif
		*pfResult_ = m_fOriginValue;
	}	

#endif
}