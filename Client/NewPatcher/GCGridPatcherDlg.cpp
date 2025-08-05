/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

★ 설계자 ☞ 이상호
★ 설계일 ☞ 2010년 11월 10일
★ E-Mail ☞ shmhlove@naver.com
★ 클래스 ☞ Grid다운로드 패쳐 다이얼로그

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCGridPatcherDlg.h"
#include "KStateInfo.h"
#include <dbg/dbg.hpp>
#include "GCGlobalValue.h"
#include "define.h"
#include "GCPatcherStr.h"

// Global 함수 선언
extern void WriteLogFile(std::string strMsg,bool bNewFile = false);
extern void Send_ErrorReport(std::string strParseMsg, std::string strLastMsg, std::string strCapacity, int iType );

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 다이얼로그 생성자
CGCGridPatcherDlg::CGCGridPatcherDlg(bool mannual,boost::shared_ptr<KGCStringLoader> strloader,boost::shared_ptr<Komfile> imagemass,CWnd* pParent /*=NULL*/)
: CRGNDlg(CGCGridPatcherDlg::IDD, pParent, AfxGetInstanceHandle())
, m_bMannual( mannual )
, m_pStrLoader( strloader )
, m_pMassFile( imagemass )
, m_pT(NULL)
, m_pHtmlPR( NULL )
, m_pkBitmapBack( NULL )
, m_pkBitmapError( NULL )
, m_pkBitmapBottom( NULL )
, m_pkBitmapNormality( NULL )
, m_pGridDownloadSDK(NULL)
, m_bError( ERRORTYPE_NONE )
, m_fTotalRate( 0.0f )
, m_iGridFailCnt( 0 )
, m_dwGridDownSize( 0 )
, m_iNumDownloadFiles( 0 )
{
	m_pHtmlPR = new CGCHtmlView();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);    
    memset( m_strTotalDownLoadInfo, 0, sizeof(char)*128 );   
}


CGCGridPatcherDlg::~CGCGridPatcherDlg()
{
    DestroyWindow();
}


void CGCGridPatcherDlg::DoDataExchange(CDataExchange* pDX)
{
    CRGNDlg::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATIC_GRID_TOTAL_BAR_POS, m_kProgressTotal);
    DDX_Control(pDX, IDC_STATIC_GRID_DOWNLOAD_TOTALRATE, m_kStaticDownLoadTotalRate);
    DDX_Control(pDX, IDC_STATIC_GRID_HTML, m_StaticHTML);
	DDX_Control(pDX, IDC_GRID_CHECK, m_kGridDownCheckBox);
	DDX_Control(pDX, IDC_GRID_START, m_kStartDownloadBtn);
    DDX_Control(pDX, IDC_STATIC_GRID_AGGRIMENT, m_StaticAggriment);
}


BEGIN_MESSAGE_MAP(CGCGridPatcherDlg, CRGNDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_DRAWITEM()    	
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_GRID_START, OnDownLoadStart)      // 그리드 다운로드 시작 후 행동(UI처리 및 라이브러리 로드시작, 쓰레드 생성)
	ON_MESSAGE(WM_GRIDLOAD_EVENT_MSG, OnGridLoadProc)   // 그리드 라이브러리 로드시 행동(로드성공, 로드실패)
    ON_MESSAGE(WM_GRIDDOWN_EVENT_MSG, OnGridDownProc)   // 그리드 다운로드 중 행동(초기화, 프로그래스바, 에러...)
END_MESSAGE_MAP()


BOOL CGCGridPatcherDlg::OnInitDialog()
{
    START_LOG( cout, "OnInitDialog() Start" );
	CRGNDlg::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    VERIFY(m_kFont.CreateFont(
        15,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        NULL ) );                  // lpszFacename     

    m_hInstance = AfxGetInstanceHandle();
    ::SetWindowText( m_hWnd, L"GrandChase" );

    //=============================================================================================
    // 배경 텍스쳐 로딩
    LoadBitmapFromMassFile( m_pMassFile.get(),"gridpatcherbg.bmp", &m_pkBitmapBack );
    LoadBitmapFromMassFile( m_pMassFile.get(),"kamuse.bmp", &m_pkBitmapBottom );
    LoadBitmapFromMassFile( m_pMassFile.get(),"gridnormality.bmp", &m_pkBitmapNormality );
    LoadBitmapFromMassFile( m_pMassFile.get(),"griderror.bmp", &m_pkBitmapError );

    //===============================================================================
    // 프로그래스 로딩바 초기화
    RECT rt;
    GetDlgItem( IDC_STATIC_GRID_TOTAL_BAR_POS )->GetClientRect(&rt);
    GetDlgItem( IDC_STATIC_GRID_TOTAL_BAR_POS )->ClientToScreen(&rt);
    this->ScreenToClient( &rt );
    m_kProgressTotal.InitializeCtrl( m_pMassFile.get(),"patcherbartotal", rt );
    
    START_LOG( clog, L"TOTAL_BAR_POS_ONINIT" )
        << BUILD_LOG( rt.top )
        << BUILD_LOG( rt.bottom )
        << BUILD_LOG( rt.left )
        << BUILD_LOG( rt.right );

	m_kProgressTotal.SetProgress( 0 );

    //===============================================================================
    // 윈도우 투명
    //SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
    //SetLayeredWindowAttributes(RGB(0,0,0), 128, LWA_ALPHA | LWA_COLORKEY);

    //===============================================================================
    // 윈도우 둥글게
    //CRgn m_rgn;   
    //CRect rcDialog;
    //GetClientRect( rcDialog );
    //m_rgn.CreateRoundRectRgn( rcDialog.TopLeft().x, rcDialog.TopLeft().y, rcDialog.BottomRight().x, rcDialog.BottomRight().y, 15, 15 );
    //::SetWindowRgn(GetSafeHwnd(), (HRGN) m_rgn, TRUE);    

    //===============================================================================
    // 글자 적기
    m_kStaticDownLoadTotalRate.SetTextColor( RGB(255,255,255) );
    m_kStaticDownLoadTotalRate.SetWindowText( m_pStrLoader->GetString( STR_ID_PATCH_START ) );
    m_kStaticDownLoadTotalRate.CalcRect( this );

	//===============================================================================
	// 그리드 다운로드 동의 관련 UI
    m_StaticAggriment.SetTextColor( RGB(255,255,255) );
    m_StaticAggriment.SetWindowText( m_pStrLoader->GetString( STR_ID_GRID_AGGRIMENT ) );
    m_StaticAggriment.CalcRect( this );

	m_kStartDownloadBtn.LoadBitmaps(m_pMassFile.get(),"downloadstart");
	m_kStartDownloadBtn.ShowWindow(SW_SHOW);
    m_kStartDownloadBtn.EnableWindow( TRUE );
    m_kGridDownCheckBox.ShowWindow(SW_SHOW);
	m_kGridDownCheckBox.SetCheck(1);

    m_strCapacityRead = "0";
    m_strCapacityTotal = "0";

	return TRUE; 
}


BOOL CGCGridPatcherDlg::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_ESCAPE)
            return 0;
    }  
    return CRGNDlg::PreTranslateMessage(pMsg);
}


HCURSOR CGCGridPatcherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CGCGridPatcherDlg::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}


BOOL CGCGridPatcherDlg::DestroyWindow()
{
    return CRGNDlg::DestroyWindow();
}


void CGCGridPatcherDlg::OnDestroy()
{
    CRGNDlg::OnDestroy();
    
    if( m_pT )                 delete m_pT;
    if( m_kFont.m_hObject )    m_kFont.DeleteObject();
    if( m_pkBitmapBack )       m_pkBitmapBack->DeleteObject();
    if( m_pkBitmapBottom )     m_pkBitmapBottom->DeleteObject();
    if( m_pkBitmapError )      m_pkBitmapError->DeleteObject();
    if( m_pkBitmapNormality )  m_pkBitmapNormality->DeleteObject();
	if(m_pHtmlPR)              m_pHtmlPR->DestroyWindow();

    OnGridJobAllDestroy();
}


void CGCGridPatcherDlg::OnGridJobAllDestroy()
{
    if( m_pGridDownloadSDK )
    {
        std::vector<HGRIDJOB>::iterator vecIter = m_vecGridDownloadJob.begin();
        for(;vecIter != m_vecGridDownloadJob.end();vecIter++)
        {
            DestroyGridSDK( *vecIter );
        }
        delete m_pGridDownloadSDK;
        m_pGridDownloadSDK = NULL;
    }
}


void CGCGridPatcherDlg::DestroyGridSDK( HGRIDJOB a_hJob )
{
    if( m_pGridDownloadSDK && a_hJob )
    {
        std::vector<HGRIDJOB>::iterator iter = std::find(m_vecGridDownloadJob.begin(), m_vecGridDownloadJob.end(), a_hJob);
        if( iter != m_vecGridDownloadJob.end() )
        {
            m_vecGridDownloadJob.erase( iter );
            m_pGridDownloadSDK->Stop(a_hJob, TRUE);
            m_pGridDownloadSDK->DestroyJob(a_hJob);
            m_iNumGridJob = m_vecGridDownloadJob.size();
            return;
        }
    }
}


void CGCGridPatcherDlg::OnBnClickedEnd()
{
    EndModalLoop(-1);
}


void CGCGridPatcherDlg::OnStart() 
{  
    OnInvalidate();
}


void CGCGridPatcherDlg::OnPaint() 
{
    PAINTSTRUCT ps;
    CDC* pDC = BeginPaint( &ps );

    if( pDC )
    {
        BITMAP bmp;
        CDC memdc, bmpdc;
        CBitmap membitmap;
        CRect m_rtCtrlSize;
        GetClientRect(&m_rtCtrlSize);

        if( memdc.CreateCompatibleDC(pDC) && bmpdc.CreateCompatibleDC(pDC) )
        {
            membitmap.CreateCompatibleBitmap(pDC, m_rtCtrlSize.Width(), m_rtCtrlSize.Height());  
            CBitmap *pOldBitmap = memdc.SelectObject(&membitmap);

            // 백그라운드
            if( m_pkBitmapBack )
            {
                m_pkBitmapBack->GetBitmap(&bmp);
                CBitmap *pOldBmp =bmpdc.SelectObject(m_pkBitmapBack);
                memdc.TransparentBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &bmpdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255,0,255) );
                bmpdc.SelectObject(pOldBmp);
            }

            // 업데이트 시작 후
            if( m_pT )
            {
                m_kProgressTotal.DrawProgressBar( &memdc );

                if( m_kFont.m_hObject )
                    m_kStaticDownLoadTotalRate.DrawText( &memdc, &m_kFont );

                // 다운로드 상태 출력
                if( ERRORTYPE_NONE == m_bError )
                {
                    m_pkBitmapNormality->GetBitmap(&bmp);
                    CBitmap * pOldBmp =bmpdc.SelectObject(m_pkBitmapNormality);
                    memdc.BitBlt( 422, 424, bmp.bmWidth, bmp.bmHeight, &bmpdc, 0, 0, SRCCOPY );
                    bmpdc.SelectObject(pOldBmp);
                }
                else
                {
                    m_pkBitmapError->GetBitmap(&bmp);
                    CBitmap * pOldBmp =bmpdc.SelectObject(m_pkBitmapError);
                    memdc.BitBlt( 422, 424, bmp.bmWidth, bmp.bmHeight, &bmpdc, 0, 0, SRCCOPY );
                    bmpdc.SelectObject(pOldBmp);
                }
            }

            // 업데이트 시작 전
            else
            {
                if( m_pkBitmapBottom )
                {
                    m_pkBitmapBottom->GetBitmap(&bmp);
                    CBitmap * pOldBmp =bmpdc.SelectObject(m_pkBitmapBottom);
                    memdc.BitBlt( 0, 411, bmp.bmWidth, bmp.bmHeight, &bmpdc, 0, 0, SRCCOPY );
                    bmpdc.SelectObject(pOldBmp);
                }
                if( m_kFont.m_hObject )
                    m_StaticAggriment.DrawText( &memdc, &m_kFont, DT_LEFT );
            }

            pDC->BitBlt(0, 0, m_rtCtrlSize.Width(), m_rtCtrlSize.Height(), &memdc, 0, 0, SRCCOPY);	
            memdc.SelectObject(pOldBitmap);

            membitmap.DeleteObject();
            memdc.DeleteDC();
            bmpdc.DeleteDC();
        }
    }
    EndPaint(&ps);
}


void CGCGridPatcherDlg::OnInvalidate()
{	
    m_kProgressTotal.SetProgress( m_fTotalRate );

    if( m_fTotalRate > 0 )
        sprintf( m_strTotalDownLoadInfo, "%.1f%%( %sKB / %sKB )", m_fTotalRate, m_strCapacityRead.c_str(), m_strCapacityTotal.c_str() );
    else
        strcpy(m_strTotalDownLoadInfo, KncUtil::toNarrowString(m_pStrLoader->GetString( STR_ID_CHECK_VERSION_DIFF )).c_str());

    m_kStaticDownLoadTotalRate.SetTextColor( RGB(255, 255, 255) );
    m_kStaticDownLoadTotalRate.SetWindowText( KncUtil::toWideString(m_strTotalDownLoadInfo) );

    RECT rect;
    rect.left = 13; 	rect.top = 415;
    rect.right = 518;   rect.bottom = 482;
    InvalidateRect(&rect, FALSE);
}


void CGCGridPatcherDlg::OnProgress( SProgressInfo & progressinfo )
{
    ChangeStringFromNumber( ((progressinfo.totalreadlength + m_dwGridDownSize)/1024), &m_strCapacityRead );
    ChangeStringFromNumber( (progressinfo.totallength/1024), &m_strCapacityTotal );
    m_fTotalRate = min((progressinfo.totalreadlength + m_dwGridDownSize) * 100.0f / progressinfo.totallength, 100);
	OnInvalidate();
}


void CGCGridPatcherDlg::OnComplete( bool success, bool bMsgBox )
{
    if( success )
    {
        m_fTotalRate = 100.0f;
        m_strCapacityRead = m_strCapacityTotal;
    }
    else
    {
        if( bMsgBox )
        {
            if( ERRORTYPE_MAIN == m_bError )
                ::MessageBoxW( NULL, m_pStrLoader->GetString( STR_ID_MAIN_EXE_VERIFY_FAIL ).c_str(), L"GrandChase", MB_OK );
            else
                ::MessageBoxW( NULL, m_pStrLoader->GetString( STR_ID_CAN_NOT_DOWNLOAD_PATCH_FILE ).c_str(), L"GrandChase", MB_OK );
        }
    }
    OnInvalidate();
    EndModalLoop( success ? 0 : -1 );
}


BOOL CGCGridPatcherDlg::OnEraseBkgnd( CDC* pDC )
{
	return FALSE;
}


// 우리패쳐로 다운로드는 정상적으로 완료 되었으나 패치인포가 다를때 호출됨
void CGCGridPatcherDlg::OnError( std::string filename,int errorcode )
{
    m_bError = ERRORTYPE_ETC;
    if( filename == "main.exe" && -6 == errorcode )
		m_bError = ERRORTYPE_MAIN;
}


void CGCGridPatcherDlg::ShowWeb( std::string url )
{
    m_pHtmlPR->AttachToDialog(this, IDC_STATIC_GRID_HTML, url.c_str(), true );   	
}


void CGCGridPatcherDlg::OnDownLoadStart()
{
    // 컨트롤 정리
    m_kStartDownloadBtn.ShowWindow(SW_HIDE);
    m_kGridDownCheckBox.ShowWindow(SW_HIDE);
    m_StaticAggriment.ShowWindow(SW_HIDE);

    // 윈도우 갱신
    InvalidateRect(NULL, TRUE);
    UpdateWindow();

    // 웹 브라우져 갱신
    if( m_pHtmlPR )
    {
        GetDlgItem( IDC_STATIC_GRID_HTML )->InvalidateRect(NULL, TRUE);
        GetDlgItem( IDC_STATIC_GRID_HTML )->UpdateWindow();
        GetDlgItem( IDC_STATIC_GRID_HTML )->ShowWindow(SW_SHOW);
    }

    // 그리드 라이브러리 로드
    NETGRIDDOWNLOADADD_CALLBACK	addfile = NULL;
    NETGRIDDOWNLOADSTART_CALLBACK startgriddown = NULL;
    if( GridLoad_InitializeMsg(m_hWnd, NULL) && GridLoad_LoadLib() )
    {
        m_iGridFailCnt = 0;
        addfile = boost::bind(&CGCGridPatcherDlg::AddGridDownLoadFile,this, _1);
        startgriddown = boost::bind(&CGCGridPatcherDlg::GridDownloadStart,this);
    }

    // 라이브러리 로드 실패시 무조건 우리 패쳐로 다운받을 수 있도록 하자.
    if( addfile == NULL || startgriddown == NULL )
    {
        addfile = NULL;
        startgriddown = NULL;
        KGCGlobalValue::m_iGridDownState = KGCGlobalValue::GRID_FAIL;
    }

    // patch쓰레드 함수 바인딩
    NETSTATRT_CALLBACK start = boost::bind(&CGCGridPatcherDlg::OnStart, this);
    NETPROGRESS_CALLBACK progress = boost::bind(&CGCGridPatcherDlg::OnProgress,this,_1);
    NETPROGRESS_CALLBACK progress2 = boost::bind(&CGCGridPatcherDlg::OnProgress,this,_1);
    NETERROR_CALLBACK error = boost::bind(&CGCGridPatcherDlg::OnError,this,_1,_2);
    COMPLETE_CALLBACK complete = boost::bind(&CGCGridPatcherDlg::OnComplete,this,_1,_2);
	IS_START_CALLBACK isStart = NULL;
    m_pT = new boost::thread(boost::bind(&KPatchLib::DownloadFiles,m_pPatchlib->get(),start,progress,progress2,error,complete,addfile,startgriddown,isStart));
}


void CGCGridPatcherDlg::OnGridLoadSuccess( const char* a_szFullPath, void* a_pUserData )
{
	GridLoad_Finalize();
	if(m_pGridDownloadSDK == NULL)
		m_pGridDownloadSDK = new CGridDownLib(a_szFullPath, m_hWnd , NULL);
    SendLog();
}


void CGCGridPatcherDlg::OnGridLoadFail( void* a_pUserData )
{
	GridLoad_Finalize();
    KGCGlobalValue::m_iGridDownState = KGCGlobalValue::GRID_FAIL;
#if defined( GRID_DOWN_LOG )
    Send_ErrorReport("GridClass-OnGridLoadFail-WePatcher", "Kamuse", std::string("0"), 1 );
#endif
}


void CGCGridPatcherDlg::SendLog()
{
    if( m_pGridDownloadSDK )
    {
        ST_GDX_SEND_LOG stLog = { 0, };

        char szTemp[1024];
        switch( KGCGlobalValue::m_dwChannelType )
        {
#if defined(_KOREAN)
        case 1: // 투니
            strcpy(szTemp, BACKGROUND_URL2);
            break;
        case 2: // 네이트
            strcpy(szTemp, BACKGROUND_URL3);
            break;
        case 3: // 네이버
            strcpy(szTemp, BACKGROUND_URL4);
            break;
#endif
        default:// 넷마블
            strcpy(szTemp, BACKGROUND_URL);
            break;
        }
        char* pszTemp = strstr(szTemp, "chase");
        pszTemp = strchr(pszTemp, '/');
        *pszTemp = NULL;

        strcpy_s(stLog.m_szCPCookie, KGCGlobalValue::m_szCPCookie);
        strcpy_s(stLog.m_szLogDomain, szTemp);
        strcpy_s(stLog.m_szGameCode, GAME_NAME);
        strcpy_s(stLog.m_szErrorFileName, "");
        strcpy_s(stLog.m_szContentType, "Patch");

        stLog.m_nIRunType = 1;
        stLog.m_nIsPageStyle = 2;
        stLog.m_nIsAgree = m_kGridDownCheckBox.GetCheck() ? 1 : 0;
        stLog.m_nIsAgreeStartPage = m_kGridDownCheckBox.GetCheck() ? 1 : 0;
        stLog.m_nIReturnGridError = 0;

        m_pGridDownloadSDK->SendLog(&stLog);
    }
}


void CGCGridPatcherDlg::OnGridJobInitialize(HGRIDJOB a_hJob, PST_GDX_FILE_INITIALIZE a_pstInit, void* a_pUserData){}
void CGCGridPatcherDlg::OnGridJobProgress(HGRIDJOB a_hJob, PST_GDX_FILE_PROGRESS a_pstProgress, void* a_pUserData)
{
    m_dwGridDownSize = a_pstProgress->m_nFileRecvSize;
    m_pPatchlib->get()->CallPrograssBar( boost::bind(&CGCGridPatcherDlg::OnProgress, this, _1) );
}


void CGCGridPatcherDlg::OnGridJobFileComplete(HGRIDJOB a_hJob, PST_GDX_FILE_COMPLETE a_pstFileComp, void* a_pUserData)
{
    if( m_pPatchlib )
    {
        char* pszTemp = strrchr(a_pstFileComp->m_szFileInfo, '\\');
        pszTemp++;

        std::map<std::string,KPatchFileInfo>::iterator mapIter = m_pPatchlib->get()->GetPatchFileInfo()->begin();
        for(; mapIter != m_pPatchlib->get()->GetPatchFileInfo()->end(); ++mapIter)
        {
            if( strstr(mapIter->first.c_str(), pszTemp) )
            {
                m_pPatchlib->get()->EraseDiffInfoFromRecvFile( mapIter->first.c_str() );
                return;
            }
        }
    }
}


void CGCGridPatcherDlg::OnGridJobComplete(HGRIDJOB a_hJob, PST_GDX_DOWNLOAD_COMPLETE a_pstJobComp, void* a_pUserData)
{
    DestroyGridSDK( a_hJob );
}


BOOL CGCGridPatcherDlg::AddGridDownLoadFile(SGridDownFileInfo &a_fileinfo)
{	
    // 그리드 다운로드 실패
    if( KGCGlobalValue::m_iGridDownState == KGCGlobalValue::GRID_FAIL )
    {   return FALSE;   }

    if( NULL == m_pGridDownloadSDK )
    {
        KGCGlobalValue::m_iGridDownState = KGCGlobalValue::GRID_FAIL;
        return FALSE;
    }

    // 새로운 잡을 생성할 것인지 현재 잡에 파일을 추가할 것인지
    eGDX_RESULT resultAddfile;
	if( (m_iNumDownloadFiles % MAX_GRID_DOWN_FILE_LIST_SIZE)  == 0 )
    {
        HGRIDJOB hRegGridDownloadJob = NULL;
        resultAddfile = m_pGridDownloadSDK->CreateJobEx(hRegGridDownloadJob, NULL);
        if( eGDX_RESULT_OK != resultAddfile || NULL == hRegGridDownloadJob )
            return FALSE;

        m_vecGridDownloadJob.push_back( hRegGridDownloadJob );
    }

	resultAddfile = m_pGridDownloadSDK->AddFileEx( *(m_vecGridDownloadJob.end()-1), 
                                                    a_fileinfo.szURL, a_fileinfo.szDIR, a_fileinfo.szFileName, a_fileinfo.ldFileSize, eGDX_DFT_GRID );
	if( eGDX_RESULT_OK != resultAddfile )
        return FALSE;

    m_iNumDownloadFiles++;
	return TRUE;
}


void CGCGridPatcherDlg::GridDownloadStart()
{
#if defined( GRID_DOWN_LOG )
    Send_ErrorReport("GridClass-GridDownloadStart-EnterFunce", "Kamuse", std::string("0"), 1 );
#endif
    std::vector<HGRIDJOB> vecFailList;
    std::vector<HGRIDJOB>::iterator vecIter = m_vecGridDownloadJob.begin();
    for(;vecIter != m_vecGridDownloadJob.end();vecIter++)
	{
		eGDX_RESULT resultAddfile = m_pGridDownloadSDK->Start(*vecIter, TRUE);
        if( eGDX_RESULT_OK != resultAddfile )
        {
            ++m_iGridFailCnt;
            vecFailList.push_back( *vecIter );
        }
	}

    // Start실패한 Job을 vecGridDownloadJob에서 제거
    for(vecIter = vecFailList.begin(); vecIter != vecFailList.end(); ++vecIter )
        DestroyGridSDK( *vecIter );

    // 3회이상 실패했을때 그리드 다운로드 취소 처리
    if( m_iGridFailCnt > 3 )
        KGCGlobalValue::m_iGridDownState = KGCGlobalValue::GRID_FAIL;

    // 다운로드가 완료될때까지 블럭킹함수로 만든다.
    m_iNumGridJob = m_vecGridDownloadJob.size();
	while(m_iNumGridJob > 0)
    {
        if( m_pPatchlib->get()->IsError() || m_pPatchlib->get()->IsStoped() )
        {   break;     }
        Sleep(1);
    }
	m_vecGridDownloadJob.clear();
#if defined( GRID_DOWN_LOG )
    Send_ErrorReport("GridClass-GridDownloadStart-LeaveFunce", "Kamuse", std::string("0"), 1 );
#endif
}


void CGCGridPatcherDlg::OnGridJobError(HGRIDJOB a_hJob, PST_GDX_ERROR a_pstError, void* a_pUserData)
{
    // 3회이상 실패했을때 그리드 다운로드 취소 처리
    if( (++m_iGridFailCnt) > 3 )
    {
        KGCGlobalValue::m_iGridDownState = KGCGlobalValue::GRID_FAIL;
#if defined( GRID_DOWN_LOG )
        Send_ErrorReport("GridClass-OnGridJobError-WePatcher", "Kamuse", std::string("0"), 1 );
#endif
    }
    DestroyGridSDK( a_hJob );
}


void CGCGridPatcherDlg::OnGridEngineUpdateInitialize(PST_GDX_ENGINE_UPDATE_INITIALIZE a_pstInit, void* a_pUserData){}
void CGCGridPatcherDlg::OnGridEngineUpdateProgress(PST_GDX_ENGINE_UPDATE_PROGRESS a_pstProgress, void* a_pUserData){}
void CGCGridPatcherDlg::OnGridEngineUpdateComplete(void* a_pUserData){}
void CGCGridPatcherDlg::OnGridEngineUpdateFail(void* a_pUserData)
{
    KGCGlobalValue::m_iGridDownState = KGCGlobalValue::GRID_FAIL;
#if defined( GRID_DOWN_LOG )
    Send_ErrorReport("GridClass-OnGridEngineUpdateFail-WePatcher", "Kamuse", std::string("0"), 1 );
#endif
}


void CGCGridPatcherDlg::ChangeStringFromNumber(IN int iNumber, OUT std::string* pszStr)
{
    if( NULL == pszStr )
        return;

    char szTemp[3];
    std::vector<std::string> vecStr;
    while( iNumber > 0 )
    {
        sprintf(szTemp, "%d", iNumber%1000);
        while( iNumber > 1000 && (szTemp[0] == NULL || szTemp[1] == NULL || (szTemp[2] == NULL)) )
        {
            std::string strTemp = "0";
            strTemp += szTemp;
            strcpy( szTemp, strTemp.c_str() );
        }
        vecStr.push_back( std::string(szTemp) );
        iNumber /= 1000;
    }

    pszStr->clear();
    while( vecStr.size() )
    {
        *pszStr += *(vecStr.end()-1);
        vecStr.pop_back();
        if( vecStr.empty() )    break;
        *pszStr += ",";
    }

    if( !pszStr->size() )
        *pszStr = "0";
}

BOOL CGCGridPatcherDlg::ModalDestroyWindow()
{
    return DestroyWindow();
}