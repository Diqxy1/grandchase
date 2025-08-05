// GCPatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCPatcherDlg.h"
#include "KStateInfo.h"
#include <dbg/dbg.hpp>
#include "GCGlobalValue.h"
#include "define.h"
#include "GCPatcherStr.h"


extern void WriteLogFile(std::string strMsg,bool bNewFile = false);
extern void Send_ErrorReport(std::string strParseMsg, std::string strLastMsg, std::string strCapacity, int iType );

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BEAN_FUN_OTP_PATCH_TIME (1000 * 300)

/////////////////////////////////////////////////////////////////////////////
// CGCPatcherDlg dialog
CGCPatcherDlg::CGCPatcherDlg(bool mannual,boost::shared_ptr<KGCStringLoader> strloader,
							 boost::shared_ptr<Komfile> imagemass,
							 int nMaxLang /*=0*/,
							 CWnd* pParent /*=NULL*/)
	: CDialog(CGCPatcherDlg::IDD, pParent),
	m_bMannual(mannual),
    m_fTotalRate( 0.0f ),
    m_fFileDownloadRate( 0.0f ),
	m_pStrLoader(strloader),
	m_pMassFile(imagemass),
	m_pHtmlPR(NULL)	,
	m_bMainError(false),
	m_bClickStart(true)
{
	m_pHtmlPR = new CGCHtmlView();//(CGCHtmlView*)CGCHtmlView::CreateObject();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);    
    
    memset( m_strTotalDownLoadInfo, 0, sizeof(char)*128 );   
    memset( m_strSubDownLoadInfo, 0, sizeof(char)*128 );   
    
    
    m_pkBitMap = NULL;    
	m_nMaxLang = nMaxLang;
	m_vecLangButton.clear();
}

void CGCPatcherDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGCPatcherDlg)
    DDX_Control(pDX, IDC_STATIC_FILE, m_StaticFile);
	DDX_Control(pDX, IDC_STATIC_FILE_NAME, m_StaticFileName);
    DDX_Control(pDX, IDC_STATIC_TOTAL, m_StaticTotal);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_STATIC_MSG, m_StaticMsg);
    DDX_Control(pDX, IDC_START, m_kStartBtn);
    DDX_Control(pDX, IDC_UPDATE_START, m_kUpdateStartBtn);
    DDX_Control(pDX, IDC_UPDATE_STOP, m_kUpdateStopBtn);
    DDX_Control(pDX, IDC_STATIC_FILE_BAR_POS, m_kProgressFile);
    DDX_Control(pDX, IDC_STATIC_TOTAL_BAR_POS, m_kProgressTotal);
    DDX_Control(pDX, IDC_STATIC_DOWNLOAD_TOTALRATE, m_kStaticDownLoadTotalRate);
    DDX_Control(pDX, IDC_STATIC_DOWNLOAD_SUBRATE, m_kStaticDownLoadSubRate);
    DDX_Control(pDX, IDC_STATIC_HTML, m_StaticBackGround);
    DDX_Control(pDX, IDC_END, m_kEndBtn);
	DDX_Control(pDX, IDC_STATIC_DATE, m_StaticDate);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_StaticSpeed);
}

BEGIN_MESSAGE_MAP(CGCPatcherDlg, CDialog)
	//{{AFX_MSG_MAP(CGCPatcherDlg)
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
//	ON_WM_NCHITTEST()
	//ON_MESSAGE( WM_ON_INVALIDATE, OnInvalidate )	
	//ON_MESSAGE( WM_ON_PROGRESS, OnProgress )
	//ON_MESSAGE( WM_ON_COMPLETE, OnComplete )	
	ON_BN_CLICKED(IDC_START, OnLaunch)
	ON_BN_CLICKED(IDC_UPDATE_START, OnUpdateStart)
	ON_BN_CLICKED(IDC_UPDATE_STOP, OnUpdateStop)
	ON_WM_DRAWITEM()    	
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_END, OnBnClickedEnd)
	ON_STN_CLICKED(IDC_STATIC_HTML, &CGCPatcherDlg::OnStnClickedStaticHtml)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCPatcherDlg message handlers

BOOL CGCPatcherDlg::OnInitDialog()
{
	
    START_LOG( cout, "OnInitDialog() Start" );
	CDialog::OnInitDialog();

	m_kEndBtn.ShowWindow(FALSE);
    m_kStartBtn.EnableWindow( FALSE );
	m_kUpdateStartBtn.ShowWindow( SW_HIDE );
	m_kUpdateStartBtn.EnableWindow( FALSE );

	m_kUpdateStopBtn.ShowWindow( SW_HIDE );
	m_kUpdateStopBtn.EnableWindow( FALSE );
    
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
        NULL ) );                 // lpszFacename     
    VERIFY(m_kSmallFont.CreateFont(
        14,                        // nHeight
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
        NULL ) );                 // lpszFacename     
    
    
    //=============================================================================================
    // 배경 텍스쳐 로딩
    m_hInstance = AfxGetInstanceHandle();
    ::SetWindowText( m_hWnd, L"GrandChase" );	

	if(m_bMannual)
	{
		LoadBitmapFromMassFile( m_pMassFile.get(),"patcherbg.bmp", &m_pkBitMap );
	}
	else
	{
		LoadBitmapFromMassFile( m_pMassFile.get(),"patcherbg.bmp", &m_pkBitMap );
	}    
    
    if( m_pkBitMap == NULL )
    {
        START_LOG( cerr, L"PatcherBG.bmp Load Error" );
    }

    //===============================================================================
    // 버튼 초기화
    m_kStartBtn.LoadBitmaps( m_pMassFile.get(),"Start" );
    m_kEndBtn.LoadBitmaps( m_pMassFile.get(),"End" );

	if ( KGCGlobalValue::m_bMultilingual ) 
	{
		m_kUpdateStartBtn.LoadBitmaps( m_pMassFile.get(),"Start" );
		m_kUpdateStopBtn.LoadBitmaps( m_pMassFile.get(),"stop" );
	}



	if ( KGCGlobalValue::m_bMultilingual ) 
	{
		// 버튼 동적 생성
		CRect rect;
		int xStart = 512;
		int yStart = 132;
			
		for ( int i = 0; i < m_nMaxLang; ++i ) 
		{
			xStart += i * 40;

			CNBitmapButton* pButton = new CNBitmapButton;
			rect.SetRect(xStart,yStart,xStart+38,yStart+24); 
			
			pButton->Create(NULL,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,rect,this,IDC_LANG_BUTTON+i);
			pButton->ShowWindow(SW_SHOW);
			
			std::string strBitmap = boost::str( boost::format("lang%d")%i );
			pButton->LoadBitmaps( m_pMassFile.get(), strBitmap );

			m_vecLangButton.push_back( pButton );
		}
		
		// 값 셋팅
		//	선택된놈을 디지블 시켜서 이미지를 바꾸자
		if ( KGCGlobalValue::m_nLanguage <  m_vecLangButton.size() ) 
		{
			m_vecLangButton[KGCGlobalValue::m_nLanguage]->EnableWindow( FALSE );		
		}
	}

    

    //===============================================================================
    // 로딩바 초기화
    
    RECT rt;
    GetDlgItem( IDC_STATIC_TOTAL_BAR_POS )->GetClientRect(&rt);
    GetDlgItem( IDC_STATIC_TOTAL_BAR_POS )->ClientToScreen(&rt);
    this->ScreenToClient( &rt );
    m_kProgressTotal.InitializeCtrl( m_pMassFile.get(),"patcherbartotal", rt );
    
    START_LOG( clog, L"TOTAL_BAR_POS_ONINIT" )
        << BUILD_LOG( rt.top )
        << BUILD_LOG( rt.bottom )
        << BUILD_LOG( rt.left )
        << BUILD_LOG( rt.right );

    GetDlgItem( IDC_STATIC_FILE_BAR_POS)->GetClientRect(&rt);
    GetDlgItem( IDC_STATIC_FILE_BAR_POS )->ClientToScreen(&rt);
    this->ScreenToClient( &rt );
    m_kProgressFile.InitializeCtrl( m_pMassFile.get(), "patcherbarfile", rt );

    START_LOG( clog, L"FILE_BAR_POS_ONINIT" )
        << BUILD_LOG( rt.top )
        << BUILD_LOG( rt.bottom )
        << BUILD_LOG( rt.left )
        << BUILD_LOG( rt.right );


	m_kProgressTotal.SetProgress( 0 );
	m_kProgressFile.SetProgress( 0 );

    //===============================================================================
    // 윈도우 둥글게
    CRgn m_rgn;   
    CRect rcDialog;
    GetClientRect( rcDialog );
    m_rgn.CreateRoundRectRgn( rcDialog.TopLeft().x, rcDialog.TopLeft().y, rcDialog.BottomRight().x, rcDialog.BottomRight().y, 10, 10 );
    ::SetWindowRgn(GetSafeHwnd(), (HRGN) m_rgn, TRUE);    

    //===============================================================================
    // 글자 적기
    m_StaticFile.SetTextColor( RGB(237,176,11) );
    m_StaticFile.SetWindowText(  m_pStrLoader->GetString( STR_ID_FILE ).c_str() );
    m_StaticFile.CalcRect( this );
	
	m_StaticFileName.SetTextColor( RGB(237,176,11) );
	m_StaticFileName.SetWindowText(L"" );
	m_StaticFileName.CalcRect( this );	
	

    m_StaticTotal.SetTextColor( RGB(237,176,11) );
    m_StaticTotal.SetWindowText( m_pStrLoader->GetString( STR_ID_TOTAL ));
    m_StaticTotal.CalcRect( this );

    m_StaticMsg.SetTextColor( RGB(255,255,255) );
    m_StaticMsg.SetWindowText( m_pStrLoader->GetString( STR_ID_PATCH_START ) );
    m_StaticMsg.CalcRect( this );

	m_StaticDate.SetTextColor( RGB(255,255,255) );
	m_StaticDate.SetWindowText( LoadLastUpdateDate() );
	m_StaticDate.CalcRect( this );

	m_StaticSpeed.SetTextColor( RGB(255,255,255) );
	m_StaticSpeed.SetWindowText( L"" );
	m_StaticSpeed.CalcRect( this );
	

    m_kStaticDownLoadTotalRate.SetTextColor( RGB(0,0,0) );
    m_kStaticDownLoadTotalRate.SetWindowText( std::wstring( L"" ) );
    m_kStaticDownLoadTotalRate.CalcRect( this );

    m_kStaticDownLoadSubRate.SetTextColor( RGB(0,0,0) );
    m_kStaticDownLoadSubRate.SetWindowText( std::wstring( L"" ) );
    m_kStaticDownLoadSubRate.CalcRect( this );

    

	//	패치 시작을 시작버튼으로 조율한다.
	if ( KGCGlobalValue::m_bMultilingual ) 
	{
		m_bClickStart = false;
		m_kStartBtn.ShowWindow( SW_HIDE );
		m_kUpdateStartBtn.ShowWindow( SW_SHOW );
		m_kUpdateStartBtn.EnableWindow( TRUE );
	}

	return TRUE; 
}


void CGCPatcherDlg::ShowWeb( std::string url )
{
	m_pHtmlPR->AttachToDialog(this, IDC_STATIC_HTML, url.c_str() );   	
}

void CGCPatcherDlg::HideWeb()
{
	GetDlgItem( IDC_STATIC_HTML )->ShowWindow(SW_HIDE);	
}


void CGCPatcherDlg::OnPaint() 
{
	PAINTSTRUCT ps;
	CDC* pDC = BeginPaint(&ps);

	if(pDC)
	{
		// 더블버퍼링 : 더블 버퍼링을 위한 DC과 BITMAP 생성
		CDC memdc;
		CBitmap membitmap;
		CRect m_rtCtrlSize;
		GetClientRect(&m_rtCtrlSize);

		// 더블버퍼링 : MDC와 비트맵에 현재 dc를 정의 및 설정
		if(memdc.CreateCompatibleDC(pDC) == TRUE)
		{
			membitmap.CreateCompatibleBitmap(pDC, m_rtCtrlSize.Width(), m_rtCtrlSize.Height());  
			CBitmap * pOldMem = memdc.SelectObject(&membitmap);

			if( m_pkBitMap)
			{
				BITMAP bmp;
				CDC bmpdc;
				m_pkBitMap->GetBitmap(&bmp);
				if(bmpdc.CreateCompatibleDC(pDC) == TRUE)
				{
					CBitmap * pOldBmp =bmpdc.SelectObject(m_pkBitMap);
					memdc.BitBlt( 0, 0, bmp.bmWidth, bmp.bmHeight, &bmpdc, 0, 0, SRCCOPY );		
					bmpdc.SelectObject(pOldBmp);
				}
			}


			m_kProgressTotal.DrawProgressBar( &memdc );	

			if(!m_bMannual)
			{
				m_kProgressFile.DrawProgressBar( &memdc );
			}			


			if( m_kFont.m_hObject && m_kSmallFont.m_hObject)
			{
				m_kStaticDownLoadTotalRate.DrawText( &memdc, &m_kFont );
                m_kStaticDownLoadSubRate.DrawText( &memdc, &m_kSmallFont );
				m_StaticMsg.DrawText( &memdc, &m_kSmallFont );
				m_StaticFile.DrawText( &memdc, &m_kFont, DT_LEFT );	
				m_StaticDate.DrawText( &memdc, &m_kSmallFont, DT_LEFT );
				m_StaticSpeed.DrawText( &memdc, &m_kSmallFont, DT_LEFT );
				
				if(!m_bMannual)
				{
					m_StaticFileName.DrawText( &memdc, &m_kSmallFont, DT_LEFT );
				}				
				m_StaticTotal.DrawText( &memdc, &m_kFont, DT_LEFT );
			}

			// 더블버퍼링 : 일반 dc에서 메모리 dc의 내용을 가져온다.
			pDC->BitBlt(0, 0, m_rtCtrlSize.Width(), m_rtCtrlSize.Height(), &memdc, 0, 0, SRCCOPY);	
			memdc.SelectObject(pOldMem);
		}
			
	}
	EndPaint(&ps);
		
}



BOOL CGCPatcherDlg::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
            return 0;
    }   

    return CDialog::PreTranslateMessage(pMsg);
}


//HCURSOR CGCPatcherDlg::OnQueryDragIcon()
//{
//	return (HCURSOR) m_hIcon;
//}


//LRESULT CGCPatcherDlg::OnNcHitTest(CPoint point) 
//{
//	return HTCAPTION;//CDialog::OnNcHitTest(point);
//}

void CGCPatcherDlg::OnStart() 
{  
	m_StaticMsg.SetWindowText(std::wstring(m_pStrLoader->GetString( STR_ID_CHECK_VERSION_DIFF )));		    
	OnInvalidate();
}


BOOL CGCPatcherDlg::DestroyWindow()
{
    return CDialog::DestroyWindow();
}


void CGCPatcherDlg::OnDestroy()
{
	
    CDialog::OnDestroy();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

    if( m_kFont.m_hObject != NULL )
        m_kFont.DeleteObject();
    if( m_kSmallFont.m_hObject != NULL )
        m_kSmallFont.DeleteObject();
    if( m_pkBitMap != NULL )    
        m_pkBitMap->DeleteObject();		

	if(m_pHtmlPR)
		m_pHtmlPR->DestroyWindow();
}

CGCPatcherDlg::~CGCPatcherDlg()
{
	DestroyWindow();
}

void CGCPatcherDlg::OnProgress( SProgressInfo & progressinfo )
{
	std::wstring msg = KncUtil::toWideString(progressinfo.currentfilename);
	msg += L" ";
	msg.append( m_pStrLoader->GetString( STR_ID_FILE_DOWNLOAD ) );
	m_StaticMsg.SetWindowText( msg );

    m_sProgressInfo.Assign( progressinfo );

	m_fFileDownloadRate = m_sProgressInfo.m_currentreadlength *100.0f/ m_sProgressInfo.m_currentlength;	
	m_fTotalRate	= m_sProgressInfo.m_totalreadlength * 100.0f/ m_sProgressInfo.m_totallength;
	
//	if ( KGCGlobalValue::m_bCheckDownSpeed ) 
//		OnDownSpeed( progressinfo );

	OnInvalidate();	
}

void CGCPatcherDlg::OnProgress2( SProgressInfo & progressinfo )
{
    m_sProgressInfo.m_currentreadlength = progressinfo.currentreadlength;

    m_fFileDownloadRate = m_sProgressInfo.m_currentreadlength *100.0f/ m_sProgressInfo.m_currentlength;	
    m_fTotalRate	= m_sProgressInfo.m_totalreadlength * 100.0f/ m_sProgressInfo.m_totallength;

    //	if ( KGCGlobalValue::m_bCheckDownSpeed ) 
    //		OnDownSpeed( progressinfo );

    OnInvalidate();	
}


void CGCPatcherDlg::OnDownSpeed(SProgressInfo & info)
{
// 	static DWORD dwBeforeTick = 0;
// 	static UINT nBeforeRead = 0;
// 	static UINT nBeforeOffset = 0;
// 
// 	static DWORD dwTotalTick = 0;
// 	static UINT nReadAll = 0;
// 
// 
// 	if ( dwBeforeTick == 0 ) 
// 	{
// 		dwBeforeTick = GetTickCount();
// 		nBeforeRead = progressinfo.currentreadlength;
// 	}
// 	else
// 	{
// 
// 		int nDiff = progressinfo.currentreadlength - nBeforeRead;
// 
// 
// 
// 		DWORD dwAfterTick = GetTickCount();
// 		DWORD dwTickDiff = dwAfterTick - dwBeforeTick;
// 		static DWORD dwLastUpdateTick = dwAfterTick;
// 
// 		nBeforeRead = progressinfo.currentreadlength;
// 		dwBeforeTick = dwAfterTick;		
// 
// 		if ( dwTickDiff == 0 ) 
// 		{
// 			nDiff = 0;
// 		}
// 
// 		if ( nDiff > 0 || dwTickDiff > 0 ) 
// 		{
// 			nReadAll += nDiff;
// 		
// 			dwTotalTick += dwTickDiff;
// 
// 			float fSpeed = ((float)nReadAll / (float)dwTotalTick) * 1000 / 1024;
// 
// 			// 1초마다 시간 갱신
// 			if ( dwLastUpdateTick + 1000 <  dwAfterTick ) 
// 			{
// 				std::wstring strSpeed = boost::str(boost::wformat( L"Speed : %2.2fKB/S" )%fSpeed);
// 				m_StaticSpeed.SetWindowText( strSpeed );
// 
// 				dwLastUpdateTick = dwAfterTick;
// 			
// 			}
// 
//  			START_LOG( clog, L"fSpeed : " << fSpeed );
//  			START_LOG( clog, L"dwTotalTick : " << dwTotalTick << " dwTickDiff : " << dwTickDiff );
//  			START_LOG( clog, L"nReadAll : " << nReadAll << " nDiff : " << nDiff );
// 			
// 
// 			//	10초 평균이면 
// 			if ( dwTotalTick > 10000 ) 
// 			{
// 				dwTotalTick = dwTotalTick * 0.9;
// 				nReadAll = nReadAll * 0.9;
// 			}
// 		}
// 	}
}

void CGCPatcherDlg::OnComplete( bool success, bool bMsgBox )
{	
	m_kEndBtn.ShowWindow(TRUE);

	if(success==false)
	{
		m_StaticMsg.SetWindowText(m_pStrLoader->GetString( STR_ID_CAN_NOT_DOWNLOAD_PATCH_FILE ));		

		if (bMsgBox==true)
		{
			if(m_bMainError==false)
			{
				MessageBox(m_pStrLoader->GetString( STR_ID_CAN_NOT_DOWNLOAD_PATCH_FILE ).c_str() );
			}
			else
			{
				MessageBox(m_pStrLoader->GetString( STR_ID_MAIN_EXE_VERIFY_FAIL ).c_str() );
			}
		}		
		OnInvalidate();
	}
	else
	{
		// 설정하고
		m_fFileDownloadRate = 100.0f;
		m_fTotalRate = 100.0f;

		m_StaticMsg.SetWindowText(m_pStrLoader->GetString( STR_ID_PATCH_END ));
		m_StaticFileName.SetWindowText(L"");
		OnInvalidate();

// 시작 버튼 활성화
#if defined(_TAIWAN) || defined ( _USA ) || defined( _EU_TEST ) || defined(_EU)
        OnLaunch();
#else
		m_kStartBtn.EnableWindow( TRUE );
#endif
	}	
}


void CGCPatcherDlg::OnInvalidate()
{

	m_kProgressTotal.SetProgress( m_fTotalRate );
	m_kProgressFile.SetProgress( m_fFileDownloadRate );
	_snprintf_s( m_strTotalDownLoadInfo,128, "%2.2f%%", m_fTotalRate );
    _snprintf_s( m_strSubDownLoadInfo,128, "%2.2f%%", m_fFileDownloadRate );

	if (m_fTotalRate<50.0f)
	{
		m_kStaticDownLoadTotalRate.SetTextColor(RGB(237,176,11));
	}
	else
	{
		m_kStaticDownLoadTotalRate.SetTextColor(RGB(25,25,25));
	}	


    m_kStaticDownLoadSubRate.SetTextColor(RGB(237,176,11));


	m_kStaticDownLoadTotalRate.SetWindowText( KncUtil::toWideString(m_strTotalDownLoadInfo ) );
    m_kStaticDownLoadSubRate.SetWindowText( KncUtil::toWideString(m_strSubDownLoadInfo ) );


	RECT rect;
	rect.left=15;
	rect.top = 493;
	rect.right = 487;
	rect.bottom = 566;
	InvalidateRect(&rect,FALSE);
	InvalidateRect(&m_StaticSpeed.m_kRect, FALSE);
}

BOOL CGCPatcherDlg::OnEraseBkgnd( CDC* pDC )
{
	return FALSE;
}

void CGCPatcherDlg::ShowEndButton()
{
    m_kEndBtn.EnableWindow( TRUE );
    m_kEndBtn.ShowWindow( SW_SHOW );
}

void CGCPatcherDlg::OnLaunch()
{
	EndModalLoop(0);
}

void CGCPatcherDlg::OnBnClickedEnd()
{
	EndModalLoop(-1);
}

void CGCPatcherDlg::OnUpdateStop()
{
	EndModalLoop(-2);
}

void CGCPatcherDlg::Stop()
{
	m_bClickStart = false;
	m_kUpdateStartBtn.ShowWindow( SW_SHOW );
	m_kUpdateStopBtn.ShowWindow( SW_HIDE );

	m_fFileDownloadRate = 0.0f;	
	m_fTotalRate	= 0.0f;
	m_StaticMsg.SetWindowText( m_pStrLoader->GetString( STR_ID_PATCH_START ) );
	OnInvalidate();
}

void CGCPatcherDlg::OnUpdateStart()
{
	m_bClickStart = true;
	m_kUpdateStartBtn.ShowWindow( SW_HIDE );
	m_kUpdateStopBtn.ShowWindow( SW_SHOW );
	m_kUpdateStopBtn.EnableWindow( TRUE );
}

void CGCPatcherDlg::OnError( std::string filename,int errorcode )
{
	if(filename=="main.exe" && errorcode == -6)
		m_bMainError=true;
}

BOOL CGCPatcherDlg::ModalDestroyWindow()
{
    return DestroyWindow();
}
BOOL CGCPatcherDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int wmID = LOWORD( wParam );

	if ( wmID >= IDC_LANG_BUTTON && wmID < IDC_LANG_BUTTON+m_nMaxLang )
	{
		OnClickLang( wmID - IDC_LANG_BUTTON );
	}


	return CDialog::OnCommand(wParam, lParam);
}

void CGCPatcherDlg::OnClickLang( int nSelect )
{
	if ( m_bClickStart ) 
		return;

	for ( int i = 0; i < m_vecLangButton.size(); ++i ) 
	{
		if ( i == nSelect ) 
		{
			m_vecLangButton[i]->EnableWindow( FALSE );
		}
		else
		{
			m_vecLangButton[i]->EnableWindow( TRUE );
		}
	}

	KGCGlobalValue::m_nLanguage = nSelect;
    ShowWeb(KGCGlobalValue::GetBGWeb());
	return;

}

const std::wstring CGCPatcherDlg::LoadLastUpdateDate()
{
	if ( !KGCGlobalValue::m_bLastUpdateLog )
		return L"";

	FILE* pFile = fopen( "LastUpdateDate.txt", "rt" );
	if ( pFile ) 
	{
		WCHAR szTemp[MAX_PATH];
		fgetws(szTemp, MAX_PATH, pFile);
		fclose( pFile );

		return szTemp;
	}

	return L"";
}
void CGCPatcherDlg::OnStnClickedStaticHtml()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
