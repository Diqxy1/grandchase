// GCPatcher.cpp : Defines the class behaviors for the application.
//


#include "stdafx.h"
#include <Shlwapi.h>
#include "GCPatcher.h"
#include "dbg/dbg.hpp"
#include "define.h"
#include "GCGlobalValue.h"
#include "GCPatcherStr.h"
#include "globalfunction.h"
#include "GCDllLoader.h"
#include "KLoginDlg.h"
#include "XTrap4Launcher.h"
#include "patch.h"
#include "../NMCrypt.h"
#include "GCPatcherBackDlg.h"
#include "../GCUtil/KOTPMD5.h"
#include "NetError.h"
#include "GCServerDlg.h"
#include "GCSystemBtnDlg.h"
#include "CYHJoin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern FILE *fto;
extern bool MakeWritableFile( const char* szFileName );
extern void Send_ErrorReport(std::string strParseMsg, std::string strLastMsg, std::string strCapacity, int iType );

/////////////////////////////////////////////////////////////////////////////
// CGCPatcherApp

BEGIN_MESSAGE_MAP(CGCPatcherApp, CWinApp)
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGCPatcherApp construction

CGCPatcherApp::CGCPatcherApp()
	:istest(false),skippatchtimecheck(false),patchtimecheck(false),ismanual(false),isrunwithoutpach(false),skippatcherpatch(false)
{	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGCPatcherApp object

CGCPatcherApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGCPatcherApp initialization




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



BOOL CGCPatcherApp::InitInstance()
{   
	//KOTPMD5 kotp;
	//std::string strCmdLine;
	//strCmdLine.clear();
	//strCmdLine.append("Main.exe " );
	//strCmdLine.append( kotp.GetOTP() );


	//boost::shared_ptr<KPatchLib> patchlibrun_direct;	
	//patchlibrun_direct.reset(new KPatchLib("",""));

	//if(patchlibrun_direct->Run(strCmdLine) ==false)
	//{
	//	START_LOG( cout,  L"Running GrandChase Failed." );
	//}
#if defined( _CHINA ) || defined(_CHINA_TEST)
	std::string patcherdir;
	std::string patchername;
	std::string patcherlogstr;

	char path[2048];
	GetModuleFileNameA(NULL,path,2048);
	std::string pathstr = path;
	std::transform(pathstr.begin(),pathstr.end(),pathstr.begin(),tolower);

	int last = pathstr.find_last_of('\\');	
	patchername = pathstr.substr(last+1);
	patcherdir = pathstr.substr(0,last+1);
	patcherlogstr = patcherdir;
	patcherlogstr += "cu_log.exe";

    KGCGlobalValue::m_strVersion = GCStrWideToChar(KGCGlobalValue::LoadVersionInfo().c_str());

	//return TRUE;
	RunLogProgram(patcherlogstr,"3");
#endif
	_set_invalid_parameter_handler(InvalidParameterHandler);
	_set_purecall_handler(PurecallHandler);
	_set_new_handler( NewHandler );

	//	글로벌 변수 초기화
	KGCGlobalValue::Init();
#if defined(KOMPASSWORD)
	Komfile::AddKey(KOMPASSWORD);
#else
    Komfile::AddKey(289942541);
#endif
		
#if !defined( __PATCHER_LOG__ )
    dbg::logfile::SetLogLevel( -1 );
    ASSERT_MBOX( "No Log" );
#endif
	AfxEnableControlContainer();

#if defined( USING_GCLOG_TEMPORARY )
	std::string reporturl = "http://gc_loggetter.kog.co.kr:7070/report/patcher.asmx/report";
	//std::string reporturl = "http://116.120.238.23:7070/default_ver2.aspx";
	std::string service = KncUtil::toNarrowString(NATION_CODE);
	std::string version = "TEST";
#if defined(_TEST)
	service+="[internal]";
#endif

#if defined(BUILD_ID)
	version = KncUtil::toNarrowString(BUILD_ID);
#endif

#else
	std::string reporturl = "http://crash-getter.kog.co.kr/report/Stat.asmx";
	std::string service = "TEST";
	std::string version = "TEST";

//#if defined(JOB_NAME)
	service = KncUtil::toNarrowString(L"EU_INTENAL");
//#endif
//#if defined(BUILD_ID)
	version = KncUtil::toNarrowString(L"2012-01-11 오후 7:38:44");
//#endif
#endif

	setlocale( LC_ALL, "" );
	xmlInitParser (); // xml 초기화

	curl_global_init( CURL_GLOBAL_ALL );

	dbg::logfile::SetFileNameAtTime( true );
	START_LOG( cout, L"START_LOG" );

	// 파라메터 분석하자 
	if(CheckArguments() == false)
	{
		return FALSE;
	}


	boost::shared_ptr<KPatchLib> patchlib;	
	if(ismanual)
	{
		patchlib.reset(new KPatchLib(ismanual,manualpatchpath));
	}
	else
	{
		patchlib.reset(new KPatchLib(id,pw));
	}

	// 일단 제대로 된 경로에서 실행중인지 확인하자 
	//
#if defined(VERIFY_PATH)
	START_LOG( cout, L"@@Start Certification" );
	if(patchlib->VerifyPatcherPath(KGCGlobalValue::GetProgramPath()) == false)
	{
		START_LOG( cerr, L"Get ProgramPath Fail!!!" );
		KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_PROGRAMPATH );
		return FALSE;
	}
#endif

    // 설치가 불가능한 폴더에 있는경우 경고를 띄워 줍니다.
    if( CheckValidFolder(patchlib, reporturl, service, version) == false )
        return FALSE;

    int re = 0;
		
#if defined( _CHINA ) || defined(_CHINA_TEST)
	boost::shared_ptr<KGCStringLoader> strloader(new KGCStringLoader());
	strloader->Load("patchstr.stg");

	// 이미지 로딩 	
	if(IsFileExists(patchlib->GetLocalPath()+"images.kom") == false)
	{
		KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_IMAGE_NO_EXIST );
		return FALSE;
	}

	boost::shared_ptr<Komfile> massfile(new Komfile());	
	if( massfile->Open("images.kom") == false )
	{
		KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_IMAGE_NO_LOAD );
		return FALSE;
	}

	//창유 플랫폼.
	if ( false ) {
		int iResult = join::CheckCYHallW(L"Grand Chase",L"YHMX",KGCGlobalValue::LoadVersionInfo().c_str(),L"MMORPG");

		if ( iResult != 2)
		{
			if ( iResult == 4 ) {
				KGCGlobalValue::PopUpWebPage(BUTTON_FLATFORM);
				return FALSE;
			}
			else {
				return FALSE;
			}
		}
	}


	KGCGlobalValue::InitServerSelectInfo();
	if ( KGCGlobalValue::m_bServerSelectMode ) {

		patchlib->DownloadServerInfoFile();
		if ( KGCGlobalValue::LoadServerSelectInfo() == false ) {
			KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_PATCHPATH_READ );		
			return FALSE;
		}

	}


	CGCPatcherBackDlg kViewDlg(massfile,strloader);

	kViewDlg.Create((UINT)IDD_GCPATCHER_BACK);
	kViewDlg.ShowWindow(SW_SHOW);
	kViewDlg.CreateAllDlg();

CN_RESELECT_CENTER_SERVER:

	if ( KGCGlobalValue::m_bServerSelectMode ) {

		RunLogProgram(patcherlogstr,"4");

		kViewDlg.OnServerSelect();

		re = kViewDlg.RunModalLoop();

		if ( re != 0 ) { // 정상 종료가 아닐때
			if ( re == -1 ) { // 그냥 종료하자. 겜 안할란다.
				kViewDlg.ModalDestroyWindow();
				return FALSE;
			}
			kViewDlg.ModalDestroyWindow();
			return FALSE;
		}

		RunLogProgram(patcherlogstr,"5");
		RunLogProgram(patcherlogstr,"6");


	}
	//kViewDlg.CloseServerScene();
#endif
	if(!ismanual)
	{		
		// 패치패스를 얻는다 

        if( CheckSrcPath() == false )
            return FALSE;

        bool isDownloadError = false;
        if ( GetPatchTimeCheck(patchlib) == false ) {
            isDownloadError = true;
            START_LOG( cerr, L"Get Patchtimecheck Fail" );
        }

		patchpath = KGCGlobalValue::GetPatchPathFileName( KGCGlobalValue::m_bFirstRedirectFTP );

		if(patchtimecheck == false || skippatchtimecheck == true) // 패치타임 아니거나 스킵이면 
		{
			// 패치패스 얻자 
            if( GetPatchPathFile(patchlib) == false ) {
                isDownloadError = true;
                START_LOG( cerr, L"Get Patchpath Fail" );
            }

#if defined( GRID_DOWN )
            // 패치모드를 얻자 
            GetPatchMode(patchlib);
#endif
        }

#ifdef __VERSION_INFO_DOWNLOAD__
		if ( patchlib->DownloadVersionInfoFile() == false )
		{
			//::MessageBoxW( NULL, strloader->GetString( 25 ).c_str(), L"GrandChase", MB_OK );

		}
		else {
			KGCGlobalValue::m_strVersion = GCStrWideToChar(KGCGlobalValue::LoadVersionInfo().c_str());
		}

#endif



        if( isDownloadError ) {
            // PC방에서 인터넷 제한을 걸고 있을 가능성도 있음.
            // 다운로드가 원활하지 않다는 메시지와 함께 그래도 게임을 시작할 지 선택.
            // ( 이 선택은 인도네시아, 태국같은 네트워크를 가진 국가에 한함 )
            int iRet = IDNO;
#if defined( RUN_WITHOUT_PATCH )
            iRet = MessageBoxW( NULL, L"Server Auto-Patch tidak tersedia\nApakah kamu ingin memulai permainan?" , L"Patch", MB_YESNO );
#endif
            if( iRet == IDYES ) {
                START_LOG( cout, L"Start game without patch!!");
                isrunwithoutpach = true;
            }
            else {        
                KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_DOWN_PATCHPATH );
                return FALSE;
            }
        }

        if( ( patchtimecheck == false || skippatchtimecheck == true ) && ismanual == false ) // 패치타임 아니거나 스킵이면 
        {

            if( isrunwithoutpach == false ) {
                // 일단 다이얼로그를 띄우고
                CDownLoadDlg downloadDlg;
#if defined( _CHINA ) || defined(_CHINA_TEST)
				downloadDlg.Create(IDD_DOWNLOAD_DLG,&kViewDlg);
#else				
                downloadDlg.Create(IDD_DOWNLOAD_DLG);
#endif				
                downloadDlg.ShowWindow(SW_SHOW);
                downloadDlg.SetText( KGCPatcherStr::GetString( NetError::PS_INIT_PATCHER ) );

                // 서버로부터 패치 정보를 가져온다 
                START_LOG( cout, L"Get Server Patch Info"  );
                if(patchlib->GetServerPatchInfo() == false)
                {
                    START_LOG( cerr, L"Download NewVer.Xml Fail!!!" ); 
                    KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_NEWVER_DOWN );
                    return FALSE;
                }

                // 패쳐 파일이 바뀌었는지 테스트 하자 
                // 패쳐패치를 껏으면 하지 말자꾸나 
                START_LOG( cout, L"Check Patcher Change!!" );    
                downloadDlg.SetText( KGCPatcherStr::GetString( NetError::PS_CHECK_PATCHER ) );

                // NO_PATCHER_PATCH 를 사용하여 빌드하니까 자꾸 본섭에 딸려 들어간다.
                // 파라메터로 집어 넣습니다.
                if( patchlib->CheckPatcherFiles() == true && skippatcherpatch == false )
                {
                    START_LOG(cout, L"Start Patcher Patch" );
                    // 패쳐패치 하고 
                    if(patchlib->DownloadPatcherFiles() == false)
                    {
                        START_LOG( cout, L"Patcher Patch Failure" );
                        return FALSE;
                    }
                    else
                    {
                        START_LOG( cout, L"Patcher Patch Success" );
                    }

                    // 창 닫고
                    downloadDlg.ShowWindow(SW_HIDE);
#if defined( _CHINA ) || defined(_CHINA_TEST)					
					kViewDlg.ModalDestroyWindow();
#endif

                    // 다시 패쳐 실행
                    return RunPatcher(*patchlib.get());
                }
                else
                {		
                    START_LOG( cout, L"Patcher is not changed!!" );		
                }

				if ( KGCGlobalValue::m_bMultilingual ) 
				{
					START_LOG( cout, L"Get Langauge Info"  );
					//	langInfo 셋팅
					if(patchlib->GetLangInfo() == false)
					{
						START_LOG( cerr, L"Download LangInfo.Xml Fail!!!" ); 
						KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_DOWN_LANGINFO );
						return FALSE;
					}
					
					START_LOG( cout, L"Open Lang.Dat"  );
					//	langDat 불러오기
					if ( patchlib->LoadCurLang() == false ) 
					{
//						START_LOG( cerr, L"Open Lang.dat Fail!!!" ); 
					}			


				}


                patchlib->ClearPatcherFiles();
                downloadDlg.ShowWindow(SW_HIDE);	
                START_LOG( cout, L"DownlaodDlg is end!!" );
            }
        }
    }
	else
	{
		if(patchlib->GetMannualPatchInfo() == false)
		{
			START_LOG( cerr, L"Download NewVer.Xml Fail!!!" ); 
			KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_NEWVER_DOWN );
		}
	}

	// 유니코드 로딩
	KGCDllLoader::LoadDll("unicows.dll");
	// 버그트랩 로딩

	// BUG_TRAP 사용
	KGCDllLoader::LoadDll("BugTrap.dll");
	InstallCrashReport();	

	BugTrap_InstallSehFilter();

	// 패치 스트링 로딩 
	if(IsFileExists(patchlib->GetLocalPath()+"patchstr.stg" ) == false)
	{
		KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_PATCHSTR );		
		return FALSE;
	}
#if !defined( _CHINA ) && !defined(_CHINA_TEST)
	boost::shared_ptr<KGCStringLoader> strloader(new KGCStringLoader());
	strloader->Load("patchstr.stg");

	// 이미지 로딩 	
	if(IsFileExists(patchlib->GetLocalPath()+"images.kom") == false)
	{
		KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_IMAGE_NO_EXIST );
		return FALSE;
	}

	boost::shared_ptr<Komfile> massfile(new Komfile());	
	if( massfile->Open("images.kom") == false )
	{
		KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_IMAGE_NO_LOAD );
		return FALSE;
	}
#endif	

	// 패치 수행

    //중복 실행 검사
    {
        HWND hPatcher = ::FindWindowW( L"GrandChase", NULL );
        HWND hMain = ::FindWindowW( L"D3D Window", NULL );

        if ( hPatcher || hMain )
        {
            return FALSE;
        }
    }
    //클래스 이름 설정 중복 검사이후에 해줘야 함
    {
        WNDCLASS wc;
        ::GetClassInfo( AfxGetInstanceHandle(), L"#32770", &wc );
        wc.lpszClassName = L"GrandChase";
        AfxRegisterClass( &wc );

    }

#if !defined( _CHINA ) && !defined(_CHINA_TEST)
    CGCPatcherDlg patcherDlg(ismanual,strloader,massfile,patchlib->GetMaxLang());
#ifdef GRID_DOWN
    CGCGridPatcherDlg GridPatcherDlg(ismanual,strloader,massfile);
    if( 0 < KGCGlobalValue::m_iPatchMode )
    {
        GridPatcherDlg.Create(IDD_GRID_GCPATCHER_DIALOG);
        GridPatcherDlg.ShowWindow(SW_SHOW);
    }
    else
    {
        patcherDlg.Create(IDD_GCPATCHER_DIALOG);
        patcherDlg.ShowWindow(SW_SHOW);
    }
#else
    patcherDlg.Create(IDD_GCPATCHER_DIALOG);
    patcherDlg.ShowWindow(SW_SHOW);
#endif

	//가운데 들어갈 웹페이지 주소 default 브라질...    
#if defined( _NO_WEB_ )
	patcherDlg.HideWeb();
#else	
#ifdef GRID_DOWN
    if( 0 < KGCGlobalValue::m_iPatchMode )
    {
        if (patchtimecheck == true)	
            GridPatcherDlg.ShowWeb(KGCGlobalValue::GetBGPWeb());
        else
            GridPatcherDlg.ShowWeb(KGCGlobalValue::GetBGWeb());		
    }
    else
    {
        if (patchtimecheck == true)	
            patcherDlg.ShowWeb(KGCGlobalValue::GetBGPWeb());
        else
            patcherDlg.ShowWeb(KGCGlobalValue::GetBGWeb());		
    }
#else
    if (patchtimecheck == true)	
        patcherDlg.ShowWeb(KGCGlobalValue::GetBGPWeb());
    else
        patcherDlg.ShowWeb(KGCGlobalValue::GetBGWeb());		

#endif
#endif	

#if defined( GRID_DOWN_LOG )
    Send_ErrorReport("Success-PatcherRunning", "Kamuse", std::string("0"), 1 );
#endif
#endif

	// 패치 시작 
	if(patchtimecheck == false || skippatchtimecheck == true) // 패치타임 아니거나 스킵이면 
	{	
#ifdef GRID_DOWN
        if( 0 < KGCGlobalValue::m_iPatchMode )
        {
            GridPatcherDlg.SetPatchLib( &patchlib );
            re = GridPatcherDlg.RunModalLoop();	 // 모달 루프를 돌자
            patchlib->Stop();
            GridPatcherDlg.BoostJoin();			 //스레드가 종료될때까지 대기
        }
        else
        {
            NETSTATRT_CALLBACK start = boost::bind(&CGCPatcherDlg::OnStart,&patcherDlg);
            NETPROGRESS_CALLBACK progress = boost::bind(&CGCPatcherDlg::OnProgress,&patcherDlg,_1);
            NETPROGRESS_CALLBACK progress2 = boost::bind(&CGCPatcherDlg::OnProgress2,&patcherDlg,_1);
            NETERROR_CALLBACK error = boost::bind(&CGCPatcherDlg::OnError,&patcherDlg,_1,_2);
            COMPLETE_CALLBACK complete = boost::bind(&CGCPatcherDlg::OnComplete,&patcherDlg,_1,_2);
            NETGRIDDOWNLOADADD_CALLBACK	addfile = NULL;
            NETGRIDDOWNLOADSTART_CALLBACK startgriddown = NULL;
			IS_START_CALLBACK isStart = NULL;
            boost::thread t(boost::bind(&KPatchLib::DownloadFiles,patchlib.get(),start,progress,progress2,error,complete, addfile, startgriddown,isStart));

            re = patcherDlg.RunModalLoop();	 // 모달 루프를 돌자
            patchlib->Stop();
            t.join();						 //스레드가 종료될때까지 대기
        }
#else
    #if defined( _CHINA ) || defined(_CHINA_TEST)
        NETSTATRT_CALLBACK start = kViewDlg.GetStartFunc();
        NETPROGRESS_CALLBACK progress = kViewDlg.GetProgressFunc();		
        NETPROGRESS_CALLBACK progress2 = kViewDlg.GetProgress2Func();
        NETERROR_CALLBACK error = kViewDlg.GetErrorFunc();
        COMPLETE_CALLBACK complete = kViewDlg.GetCompleteFunc();
        NETGRIDDOWNLOADADD_CALLBACK	addfile = NULL;
        NETGRIDDOWNLOADSTART_CALLBACK startgriddown = NULL;
        IS_START_CALLBACK isStart = kViewDlg.GetIsStartFunc();
    #else
        NETSTATRT_CALLBACK start = boost::bind(&CGCPatcherDlg::OnStart,&patcherDlg);
        NETPROGRESS_CALLBACK progress = boost::bind(&CGCPatcherDlg::OnProgress,&patcherDlg,_1);		
        NETPROGRESS_CALLBACK progress2 = boost::bind(&CGCPatcherDlg::OnProgress2,&patcherDlg,_1);
        NETERROR_CALLBACK error = boost::bind(&CGCPatcherDlg::OnError,&patcherDlg,_1,_2);
        COMPLETE_CALLBACK complete = boost::bind(&CGCPatcherDlg::OnComplete,&patcherDlg,_1,_2);
        NETGRIDDOWNLOADADD_CALLBACK	addfile = NULL;
        NETGRIDDOWNLOADSTART_CALLBACK startgriddown = NULL;
		IS_START_CALLBACK isStart = boost::bind(&CGCPatcherDlg::IsStart,&patcherDlg);
    #endif
		if ( !KGCGlobalValue::m_bMultilingual ) 
		{
    #if defined( _CHINA ) || defined(_CHINA_TEST)
            kViewDlg.OnDownload();
            RunLogProgram(patcherlogstr,"7");

            boost::thread t(boost::bind(&KPatchLib::DownloadFiles,patchlib.get(),start,progress,progress2,error,complete, addfile, startgriddown,isStart));
            re = kViewDlg.RunModalLoop();	 // 모달 루프를 돌자
            patchlib->Stop();		
            t.join();						 //스레드가 종료될때까지 대기
            if ( re == 1 && KGCGlobalValue::m_bServerSelectMode == true) {
                goto CN_RESELECT_CENTER_SERVER;
            }
            RunLogProgram(patcherlogstr,"8");
    #else
			boost::thread t(boost::bind(&KPatchLib::DownloadFiles,patchlib.get(),start,progress,progress2,error,complete, addfile, startgriddown,isStart));
			re = patcherDlg.RunModalLoop();	 // 모달 루프를 돌자
			patchlib->Stop();		
			t.join();						 //스레드가 종료될때까지 대기
    #endif
		}
		else 
		{
    #if !defined( _CHINA ) && !defined(_CHINA_TEST)
			//	다국어 기능이 있을때 중간에 스톱 기능도 들어가야 한다.
			bool bReDown = true;
			while ( bReDown ) 
			{
				boost::thread t(boost::bind(&KPatchLib::DownloadFiles,patchlib.get(),start,progress,progress2,error,complete, addfile, startgriddown,isStart));
				re = patcherDlg.RunModalLoop();	 // 모달 루프를 돌자
				patchlib->Stop();		
				t.join();						 //스레드가 종료될때까지 대기

				//	 0 정상성공
				//	 -1는 패쳐 종료
				//	 -2는 유저가 일시 중지
				if ( re == -2 ) 
				{
					patchlib->Start();
					patcherDlg.Stop();
				}
				else
				{
					bReDown = false;
				}
			}		
    #endif
		}




#endif
	}
	else
	{
#ifdef GRID_DOWN
        if( 0 < KGCGlobalValue::m_iPatchMode )
        {
            GridPatcherDlg.OnComplete(false,false);
            re = GridPatcherDlg.RunModalLoop();
        }
        else
        {
            patcherDlg.OnComplete(false,false);
            re = patcherDlg.RunModalLoop();
        }
#else
    #if !defined( _CHINA ) && !defined(_CHINA_TEST)
        patcherDlg.OnComplete(false,false);
        re = patcherDlg.RunModalLoop();
    #endif

#endif
	}	

	//	Lang.Data Save
	if ( KGCGlobalValue::m_bMultilingual ) 
	{
		if ( !patchlib->SaveCurLang() ) 
		{
			START_LOG( cout, "Error patchlib->SaveCurLang()" );
		}
	}


	xmlCleanupParser();	
	if(patchtimecheck == false || skippatchtimecheck == true) // 패치타임 아니거나 스킵이면 
	{
		// 클라이언트 실행
		if (patchlib->IsPatchComplete() == true && re == 0)
		{
#if defined( CHECK_DIRECTX_VERSION )
            if( false == CheckDXVersion() )
            {
                MessageBoxA( NULL, "DirectX 셋업을 실패하였습니다. Microsoft웹페이지에서 DirectX를 최신버전을 설치해주시길 바랍니다.", "DirectX Error", MB_OK );
                WinExec("explorer http://www.microsoft.com/downloads/ko-kr/default.aspx", FALSE);
            }
            else
            {
#endif
				if ( !isrunwithoutpach ) 
					SaveLastUpdateDate();				
				if(RunClient(*patchlib.get())){
					patchlib->Report(0,reporturl,service,version);
				}
				else{
					patchlib->Report(1,reporturl,service,version);
				}
                #if defined( GRID_DOWN_LOG )
                    Send_ErrorReport("Success-ClientRunning", "Kamuse", std::string("0"), 1 );
                #endif
#if defined( CHECK_DIRECTX_VERSION )
            }
#endif
		}
        else
        {
			version = patchlib->GetStrLastErr();
			patchlib->SetStrLastErr(re);
			patchlib->SetStrLastErr("Patch fail");


			patchlib->Report(2,reporturl,service,version);

#if defined( GRID_DOWN_LOG )
            if( !patchlib->IsPatchComplete() )
                Send_ErrorReport("Fail-ClientRunning( Diff Not Empty )", "Kamuse", std::string("0"), 1 );
            else
                Send_ErrorReport("Fail-ClientRunning( Dlg Return Value )", "Kamuse", std::string("0"), 1 );
#endif
        }
	}
    else
        Send_ErrorReport("Fail-ClientRunning( patchtimecheck, skippatchtimecheck )", "Kamuse", std::string("0"), 1 );

	KGCDllLoader::ReleaseAllDll();
#ifdef GRID_DOWN
    // 패쳐 윈도우 강제종료
    if( 0 < KGCGlobalValue::m_iPatchMode )
    {
        GridPatcherDlg.ModalDestroyWindow();
    }
    else
    {
        patcherDlg.ModalDestroyWindow();
    }
#else

#if defined( _CHINA ) || defined(_CHINA_TEST)
    kViewDlg.ModalDestroyWindow();
#else
    patcherDlg.ModalDestroyWindow();
#endif

#endif


#if defined( GRID_DOWN_LOG )
    Send_ErrorReport("Success-PatcherEnd", "Kamuse", std::string("0"), 1 );
#endif
	return TRUE;
}


void CGCPatcherApp::GetArguments( std::vector<std::string>& tokens) 
{
	char* pStrCommandLine = GetCommandLineA();
	// 패처 실행파일 이름과 전달 인자의 경계를 찾는다.
	char* pcSep = ::StrStrIA( pStrCommandLine, ".exe" );
	char* strToken;
	std::string strToken_;
	if( !pcSep )
	{
		// 명령줄이 이상하다. .exe 라는 말이 없다.
		strToken = strtok( pStrCommandLine, " " );
		while( strToken )
		{
			strToken_ = strToken;
			tokens.push_back( strToken_ );
			strToken = strtok( NULL, " " );
		}
	}
	else
	{
		pcSep = ::StrStrIA( pcSep, " " );
		if( !pcSep )
		{
			// 전달 인자가 없다.
			tokens.push_back( pStrCommandLine );
		}
		else
		{
			char szArg0[MAX_PATH];
			strncpy( szArg0, pStrCommandLine, pcSep - pStrCommandLine );
			szArg0[pcSep - pStrCommandLine] = 0;

			tokens.push_back( szArg0 );

			strToken = strtok( pcSep, " " );
			while( strToken )
			{
				strToken_ = strToken;
				tokens.push_back( strToken_ );
				strToken = strtok( NULL, " " );
			}
		}
	}
}


char g_szClipBoardData[20*1024];


//유니코드를 멀티 코드로 변환
std::string wstrtostr(const std::wstring &wstr) 
{ 
    // Convert a Unicode string to an ASCII string 
    std::string strTo; 

    char *szTo = new char[wstr.length() + 1]; 

    szTo[wstr.size()] = '\0'; 
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL); 
    strTo = szTo; 
    delete[] szTo; 
    return strTo; 
} 



//멀티코드를 유니 코드로 변환
std::wstring strtowstr(const std::string &str) 
{ 
    // Convert an ASCII string to a Unicode String 
    std::wstring wstrTo; 
    wchar_t *wszTo = new wchar_t[str.length() + 1]; 
    wszTo[str.size()] = L'\0'; 
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length()); 
    wstrTo = wszTo;
    delete[] wszTo; 
    return wstrTo; 
} 

bool CGCPatcherApp::CheckArguments()
{	
	GetArguments(arguments); // 아규먼트 분리하고

#if defined( PATCHER_LOG_LEVEL )
	{
		dbg::logfile::SetLogLevel( 2 );
		START_LOG( cout, L"START_LOG" );
	}
#endif


#if defined( NO_PATCHER_PATCH )
    if( std::find( arguments.begin(), arguments.end(), "__kogstudios_skip_patcherpatch__" ) != arguments.end() ) {
        skippatcherpatch = true;
    }
#endif

    if( std::find( arguments.begin(), arguments.end(), "__kogstudios_enable_log__" ) != arguments.end() ) 
	{
        dbg::logfile::SetLogLevel( 2 );
        START_LOG( cout, L"START_LOG" );
    }

	if( std::find( arguments.begin(), arguments.end(), "__kogstudios_redirect_ftp___" ) != arguments.end() ) {
		KGCGlobalValue::m_bFirstRedirectFTP = true;
	}

	if ( 1 == arguments.size() ) // 웹 페이지의 스타터가 실행한 것이 아니라면 종료한다.
	{
		START_LOG( clog, L"Check Parameter : Web Starter" );
#ifdef _NEXON_JAPAN
		::WinExec("C:\\Program Files\\Internet explorer\\iexplore.exe http://grandchase.nexon.co.jp", SW_SHOW);
#elif  _KOREAN
		::WinExec("C:\\Program Files\\Internet explorer\\iexplore.exe http://grandchase.netmarble.net", SW_SHOW);
		//웹에서 그랜드 체이스 실행해라!
#elif defined( _TEST )
		START_LOG( clog, L"Check Parameter : _TEST" );
		istest = true;
		return true;
#elif defined( _LEVELUP_BRAZIL ) || defined( _TAIWAN ) || defined( _HONGKONG ) || defined( _USA ) || defined(_THAILAND) || defined( _INDONESIA ) || defined( _PHILIPPINE ) || defined( _LATINAMERICA ) || defined( _EU ) || defined( _CHINA )
		return true;
#endif
		return false;
	}
	else if ( arguments[1] == "__kogstudios_default_test__" )  
	{
		START_LOG( clog, L"Check Parameter : __kogstudios_default_test__" );		
		istest = false;
	}
	else if( 0 == ::strcmp( arguments[1].c_str(), "__kogstudios_default_test_new_patcher__"  ) )
	{
		START_LOG( clog, L"Check Parameter : __kogstudios_default_test_new_patcher__" );
		istest = true;
	}
#if defined(USE_MANUAL_PATCHSYSTEM)
	else if( arguments[1]=="__kogstudios_manualpatch_system_new_patcher__")
	{
		START_LOG( clog, L"Check Parameter : __kogstudios_manualpatch_system_new_patcher__" );
		ismanual = true;
		manualpatchpath = arguments[2];
	}
#endif
	else if ( 0 == ::strcmp( arguments[1].c_str(), "__kogstudios_generic_test__" ) )
	{
		START_LOG( clog, L"Check Parameter : __kogstudios_generic_test__" );
		istest = true; // 사내 테스트 모드를 켜놓는다.

		if ( 5 != arguments.size() )
		{
#ifdef _NEXON_JAPAN            
			MessageBox( NULL,L"Invalid Test Parameter!", L"GrandChase" ,0);
#elif _KOREAN
			MessageBox( NULL,L"테스트 모드의 인자가 잘못되었습니다.", L"GrandChase" ,0);
#endif//테스트 인자가 잘못 되었음.
			return false;
		}
	}
#if defined( _BEAN_FUN_ )
	else if ( 3 <= arguments.size() )
	{
		id = arguments[1];
		pw = arguments[2];

		if( arguments.size() >= 4 )
		{
			if( arguments[3]=="__kogstudios_skip_patchtimecheck__" ) 
			{
				skippatchtimecheck = true;
			}
		}
		istest = false;

		return true;
	}
#endif
	else if (arguments[1]=="wpqkfwhadufdjqhwlakfkqmfkwlfakstp" ) 
	{
		skippatchtimecheck = true;
#if defined( _TEST )
		START_LOG( clog, L"Check Parameter : _TEST" );
		istest = true;
		return true;
#elif defined( _LEVELUP_BRAZIL ) || defined( _TAIWAN ) || defined( _HONGKONG ) || defined( _USA ) || defined( _EU ) || defined( _CHINA )
		return true;
#endif
	}


#ifdef _DEBUG
    ///테스트///////////////////////////////////////////////////////////////////
    //Spare파라미터값이 어떤게 있는지 체크
    {
        for ( std::vector<std::string>::iterator itor = arguments.begin() ; itor != arguments.end() ; ++itor ) {
            std::wstring wStrTemp = strtowstr( *itor );
            MessageBox(NULL, wStrTemp.c_str(), L"Information", MB_OK);
        }
    }
#endif


//한국에서는 클립보드도 읽어주신단다~
	{	
		char szDataBuf[20 * 1024] = { 0, };

		// 클립보드에서 인자 데이터를 읽어온다(한번 얻어오면 삭제됨)
		// 파라미터인 __argv[1]가 해독키가 된다.

		if ( !GetNMClipData( szDataBuf, sizeof( szDataBuf ) - 1, arguments[1].c_str(), false ) )
		{
			return true;
        }

		if ( 0 == ::strlen( szDataBuf ) ) // 데이터가 아무것도 없다면 잘못된 파라미터이다.
		{
			return true;
		}

        strcpy( g_szClipBoardData, szDataBuf );
		std::string strParam( szDataBuf );

		char szSpareParam[2 * 1024]        = { 0, };
		int iWord                   = 0;
		int iFeed                   = 0;

		for( UINT i = 0; i < (int)strParam.size(); ++i )
		{
			if ( ',' == strParam[i] ) // ','로 구분하여 다음 데이터로 넘어간다.
			{
				++iWord;
				iFeed = 0;
				continue;
			}

			switch ( iWord )
			{			
			case 5: // (스페어파람)
				{
					szSpareParam[iFeed]    = strParam[i];
					szSpareParam[iFeed+1]  = NULL;
					break;
				}
			}
			++iFeed;
		}

        memset( KGCGlobalValue::m_szCPCookie, 0, sizeof(char) * 20480 );
        char *szTemp = strstr(szDataBuf, "DCCode");
        if( szTemp ) strcpy( KGCGlobalValue::m_szCPCookie, szTemp );

		std::string strSpareParam = szSpareParam;



		if( strSpareParam.find("toon") != std::string::npos )		
			KGCGlobalValue::m_dwChannelType = 1;
        else if( strSpareParam.find("nate") != std::string::npos )		
            KGCGlobalValue::m_dwChannelType = 2;
        else if( strSpareParam.find("naver") != std::string::npos )		
            KGCGlobalValue::m_dwChannelType = 3;
        else if( strSpareParam.find(OPEN_PASSWORD) != std::string::npos )
            KGCGlobalValue::m_bPreTestPath = true;
         
	}
	return true;
}

bool CGCPatcherApp::RunClient(KPatchLib & patchlib)
{
	std::string strCmdLine;

	std::string strFile(patchlib.GetLocalPath());
	strFile.append("Main.exe ");

	if( false == istest )
	{
#if defined( _KOREAN )

	#if defined( PRE_CLIP_BOARD_READ)
        strCmdLine.append(g_szClipBoardData);
	#else
		strCmdLine.append( arguments[1] );
	#endif

#elif defined(_BEAN_FUN_)
		strCmdLine += id;
		strCmdLine += " ";
		strCmdLine += pw;
		strCmdLine += " __kogstudios_original_service__";		

#elif defined(_THAILAND)
		strCmdLine.append( "__kogstudios_original_serviceTH__" );
#else
		strCmdLine.append( "__kogstudios_original_service__" );
#endif
	}
#if !defined( _CHINA ) && !defined(_CHINA_TEST)	
	else
	{
		KLoginDlg           dlg;
		std::string         strID;
		std::ostringstream  codeStream;
		std::ostringstream  paramStream;

		DWORD dwUniqueNum = 0;
		char  szRandom[]  = { "NETMARBLE" };
		int   iRandVal    = static_cast<int>( szRandom[::rand() % ::strlen( szRandom )] );

		// 테스트용일 때에는 로그인 아이디를 받는다.
		while ( true == strID.empty() )
		{
			if ( IDOK == dlg.DoModal() )
			{
				START_LOG( cout,  L"ID entered." );

				strID = dlg.GetID();
			}
			else
			{
				START_LOG( cout,  L"GrandChase execution canceled." );
				return true;
			}
		}
		// 테스트를 위한 더미 파라미터 생성.
		// 앞부분 최대 4글자까지만 참고하여 더미 파라미터를 생성한다.
		for ( UINT i = 0; i < strID.size() && i < 4; i++ )
		{
			dwUniqueNum <<= 8;
			dwUniqueNum |= strID[i];
		}

		codeStream << strID << std::setiosflags( std::ios::uppercase )
			<< std::hex << std::setw( 8 ) << std::setfill( '0' ) << dwUniqueNum
			<< std::hex << std::setw( 2 ) << std::setfill( '0' ) << iRandVal;

		std::string strCode = codeStream.str();
		START_LOG( cout,  L"GrandChase encryption code generated." )
			<< BUILD_LOG( strCode.c_str() );

#if defined( _TEST )
		std::string strTestIP( KGCGlobalValue::GetTestSrvIP() );
		std::string strTestPort( KGCGlobalValue::GetTestSrvPort() );

		if( strTestIP.compare( "NULL" ) != 0 && strTestPort.compare( "NULL" ) != 0 )
		{
			paramStream << ( ( 5 == arguments.size() ) ? arguments[3].c_str() : strTestIP ) << ','
				<< ( ( 5 == arguments.size() ) ? arguments[4].c_str() : strTestPort )
				<< ( ::rand() % 1 ) << ',' << strID << ',' << strCode;
		}
		else
		{
			paramStream << ( ( 5 == arguments.size() ) ? arguments[3].c_str() : DEF_TEST_SVRIP ) << ','
				<< ( ( 5 == arguments.size() ) ? arguments[4].c_str() : DEF_TEST_SVRPORT )
				<< ( ::rand() % 1 ) << ',' << strID << ',' << strCode;
		}        
#else
		paramStream << ( ( 5 == arguments.size() ) ? arguments[3].c_str() : DEF_TEST_SVRIP ) << ','
			<< ( ( 5 == arguments.size() ) ? arguments[4].c_str() : DEF_TEST_SVRPORT )
			<< ( ::rand() % 1 ) << ',' << strID << ',' << strCode;
#endif

		std::string strParam = paramStream.str();
		START_LOG( cout,  L"Parameter generated." )
			<< BUILD_LOG( strParam.c_str() );

		strCmdLine.append( strParam.c_str() );
		strCmdLine.append( " __kogstudios_default_test__" );
	}
#endif

#if defined(USE_XTRAP) && !defined( _DEBUG)
	XTrap_L_Patch( KGCGlobalValue::GetXTrapHttpUrlKey().c_str(), NULL, XTRAP_DOWNLOAD_TIME_OUT);
#endif

    /*[ OTP TestCode ] : 20101114 exxa222
    ========================================================================*/
	#define MD5_PATCHER_PARAMETER
    #if defined( _LEVELUP_BRAZIL ) && defined(MD5_PATCHER_PARAMETER)
    KOTPMD5 kotp;
    strCmdLine.clear();
#if defined( _CHINA ) || defined(_CHINA_TEST)
    strCmdLine.append("Main.exe " );
#endif
    strCmdLine.append( kotp.GetOTP() );
    #endif

	if ( KGCGlobalValue::m_bMultilingual ) 
	{
		strCmdLine.append( " -ml ");
		strCmdLine.append( patchlib.GetLangName( KGCGlobalValue::m_nLanguage ));
	}

#if defined( _CHINA ) || defined(_CHINA_TEST)
    if ( KGCGlobalValue::m_bServerSelectMode ) 
    {
        strCmdLine.append( " -ip ");
        strCmdLine.append( KGCGlobalValue::m_strSelectIP );
        strCmdLine.append( " ");
        strCmdLine.append( KGCGlobalValue::m_strSelectPort );
    }

    if ( KGCGlobalValue::m_bVersionInfo ) {
        strCmdLine.append( " -ver ");
        strCmdLine.append( KGCGlobalValue::m_strVersion );

    }
#endif

	if(patchlib.RunExecute(strFile,strCmdLine) == false)
	{
		START_LOG( cout,  L"Running GrandChase Failed." );
		return false;
	}
	START_LOG( cout,  L"GrandChase was successfully started." );
    return true;
}

bool CGCPatcherApp::RunExecute( std::string filename, std::string param )
{

    _SHELLEXECUTEINFOA execinfo;
    ZeroMemory(&execinfo,sizeof(_SHELLEXECUTEINFOA));
    execinfo.cbSize = sizeof(_SHELLEXECUTEINFOA);
    execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    execinfo.nShow = SW_SHOW;
    execinfo.lpVerb = "open";
    execinfo.lpFile = filename.c_str();
    execinfo.lpParameters = param.c_str();

    bool bExcueSuccess = ShellExecuteExA( &execinfo ); 
    if(bExcueSuccess == true)
    {
        return true;
    }

    return false;
}

bool CGCPatcherApp::RunLogProgram( std::string strFile,std::string arg, std::string mt /*= "1"*/ )
{
    std::string strCmdLine;
    std::string strMediaType;

    strCmdLine.append( KGCGlobalValue::m_strVersion.empty() ? "0": KGCGlobalValue::m_strVersion );
    strCmdLine.append( " ");
    strCmdLine.append( arg );

    if ( !arg.compare("2") )
    {
        strCmdLine.append( " ");
        strCmdLine.append( mt );
    }

    if(RunExecute(strFile,strCmdLine) == false)
    {
        START_LOG( cout,  L"Running GrandChase Failed." );
        return false;
    }
    START_LOG( cout,  L"GrandChase was successfully started." );
    return true;
}

bool CGCPatcherApp::RunPatcher( KPatchLib & patchlib )
{
	return patchlib.Run(GetCommandLineA());
}

bool CGCPatcherApp::CheckSrcPath()
{
    START_LOG( cout, L"Make Patch Server Path" );
    serverurl = KGCGlobalValue::GetSrcPath();
    if(serverurl == "NULL")
    {
        START_LOG( cerr, L"StrSrcPath read Fail!!!" );        
        KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_PATCHSRC );

        return false;
    }
    START_LOG( clog, serverurl.c_str() );

    return true;
}

bool CGCPatcherApp::GetPatchTimeCheck( boost::shared_ptr<KPatchLib> patchlib )
{
    patchtimecheckpath = KGCGlobalValue::GetPatchTimeFileName();

    // 타임체크 얻고	
    START_LOG( cout, L"Check PatchTimeCheck" );
    START_LOG( clog, patchtimecheckpath.c_str() );
    if(patchlib->GetBooleanFromDat(patchtimecheckpath,patchtimecheck) == false)
    {
        return false;
    }

    return true;
}

bool CGCPatcherApp::GetPatchPathFile( boost::shared_ptr<KPatchLib> patchlib ) 
{
    START_LOG( cout, L"Get Patchpath" );		
    START_LOG( clog, patchpath.c_str() );
    if(patchlib->GetPatchPath(patchpath) == false)
    {
        return false;
    }
    return true;
}

void CGCPatcherApp::GetPatchMode( boost::shared_ptr<KPatchLib> patchlib )
{
    int iEnd = patchpath.find_last_of("/");
    patchmode = patchpath.substr(0, iEnd);
    patchmode = patchmode + "/PatchMode.dat";

    START_LOG( cout, L"Get PatchMode" );		
    START_LOG( clog, patchmode.c_str() );
    if( 0 > (KGCGlobalValue::m_iPatchMode = patchlib->GetPatchMode(patchmode)) )
    {
        KGCGlobalValue::m_iPatchMode = 0;
    }
}

bool CGCPatcherApp::CheckValidFolder( boost::shared_ptr<KPatchLib> patchlib, std::string reporturl, std::string service, std::string version )
{
    std::string strPath = KGCGlobalValue::GetProgramPath();

    std::set<int> setFolders;
    setFolders.insert(CSIDL_WINDOWS);        
    setFolders.insert(CSIDL_SYSTEM);

    BOOST_FOREACH( int id, setFolders ) 
    {
        std::string strSystemPath = GetSystemFolderPath(id);
        strSystemPath.append( "\\" );
        if( strPath.find( strSystemPath ) != std::string::npos ) 
        {
            KGCPatcherStr::ShowErrorMsgBox( KGCPatcherStr::PE_EXECUTE_IN_SYSTEM_FOLDER );

            patchlib->OnError(strPath,KPatchLib::SYSTEMFOLDER);
            patchlib->Report(false,reporturl,service,version);
            return false;
        }
    }
    return true;
}

void CGCPatcherApp::SaveLastUpdateDate()
{
	if ( !KGCGlobalValue::m_bLastUpdateLog ) 
		return;

	FILE* pFile = fopen( "LastUpdateDate.txt", "wt" );
	if ( pFile ) 
	{
		CTime time = CTime::GetCurrentTime();
		std::wstring strTime = time.Format( "%Y-%m-%d %H:%M:%S" );
		fwprintf( pFile, L"%s", strTime.c_str() );
		fclose( pFile );
	}
}