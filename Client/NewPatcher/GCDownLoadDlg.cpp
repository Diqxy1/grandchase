// GCDownLoadDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCDownLoadDlg.h"
#include "NBitmapButton.h"
#include "patch.h"
#include "GCGlobalValue.h"
#include "KStateInfo.h"



// CGCDownLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGCDownLoadDlg, CDialog)

CGCDownLoadDlg::CGCDownLoadDlg(CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass,
							   boost::shared_ptr<KGCStringLoader> strLoader)
	: CDialog(CGCDownLoadDlg::IDD, pParent)
	, m_pMassfile(imagemass)
	, m_pHtmlPR(NULL)
	, m_pkBitMap(NULL)
	, m_pkBitMapProgress(NULL)
	, m_iTotalProgress(0)
	, m_spStrLoader(strLoader)
	, m_bPackageSizeCheck(false)
	, m_i64OldDownLoad(0)
	, m_i64CurrentDownLoad(0)
	, m_iSpeedCheckTime(5)
	, m_bEraseBack(FALSE)
{
	m_pHtmlPR = new CGCHtmlView();

	CreateLinkButtonList();
}

CGCDownLoadDlg::~CGCDownLoadDlg()
{
}

void CGCDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGCDownLoadDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//ON_BN_CLICKED(CBB_DOWNLOAD_PACKAGE, OnPackageDownBtn)
	//ON_BN_CLICKED(CBB_DOWNLOAD_SERVER_RESELECT, OnReselectServerBtn)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CGCDownLoadDlg::CreateLinkButtonList()
{
	m_pkBtnPackageDownload = new CNBitmapButton();
	m_pkBtnPackageDownload->LoadBitmaps(m_pMassfile.get(), "Package");

	m_pkBtnSelectServer = new CNBitmapButton();
	m_pkBtnSelectServer->LoadBitmaps(m_pMassfile.get(), "AreaReSel");

	
}

BOOL CGCDownLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetWindowLong( m_hWnd, GWL_EXSTYLE, ::GetWindowLong( m_hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB( 255, 0, 255), 0, LWA_COLORKEY/* | LWA_ALPHA */);

	LoadBitmapFromMassFile( m_pMassfile.get(),"bg02.bmp", &m_pkBitMap );
	LoadBitmapFromMassFile( m_pMassfile.get(),"progress01.bmp", &m_pkBitMapProgress );

	SetLinkButtonPosition();

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


	CRect rName;
	int iNameStartX = 110;
	int iNameStartY = 335;
	int iNameWidth = 100;
	int iNameHeight = 38;
	rName.SetRect(iNameStartX,iNameStartY,iNameStartX+iNameWidth,iNameStartY+iNameHeight); 

	m_stServerName.CalcRect(this);
	m_stServerName.CalcRect(rName);
	m_stServerName.SetTextColor(RGB(255,255,0));
	m_stServerName.ShowWindow(SW_SHOW);

	iNameStartX = 16;
	iNameStartY = 277;
	iNameWidth = 130;
	iNameHeight = 38;
	rName.SetRect(iNameStartX,iNameStartY,iNameStartX+iNameWidth,iNameStartY+iNameHeight); 

	m_stPackage.CalcRect(this);
	m_stPackage.CalcRect(rName);
	m_stPackage.SetTextColor(RGB(255,255,255));
	m_stPackage.ShowWindow(SW_SHOW);

	iNameStartX = 156;
	iNameStartY = 277;
	iNameWidth = 130;
	iNameHeight = 38;
	rName.SetRect(iNameStartX,iNameStartY,iNameStartX+iNameWidth,iNameStartY+iNameHeight); 	

	m_stTime.CalcRect(this);
	m_stTime.CalcRect(rName);
	m_stTime.SetTextColor(RGB(255,255,255));
	m_stTime.ShowWindow(SW_SHOW);

	iNameStartX = 296;
	iNameStartY = 277;
	iNameWidth = 130;
	iNameHeight = 38;
	rName.SetRect(iNameStartX,iNameStartY,iNameStartX+iNameWidth,iNameStartY+iNameHeight); 	

	m_stSpeed.CalcRect(this);
	m_stSpeed.CalcRect(rName);
	m_stSpeed.SetTextColor(RGB(255,255,255));
	m_stSpeed.ShowWindow(SW_SHOW);


	




	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.

}

void CGCDownLoadDlg::SetLinkButtonPosition()
{
	CRect rect;
	int iStartX = 16;
	int iStartY = 13;

	int iWidth = 703;
	int iHeight = 231;

	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+iHeight); 

	m_pkWebStatic.Create(NULL,WS_CHILD|WS_VISIBLE,rect,this,CBB_DOWNLOAD_WEB_VIEW_STATIC);

	iStartX = 625;
	iStartY = 270;

	iWidth = 100;
	iHeight = 24;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+iHeight); 

	m_pkBtnPackageDownload->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_DOWNLOAD_PACKAGE);
	m_pkBtnPackageDownload->EnableWindow(TRUE);
	m_pkBtnPackageDownload->ShowWindow(SW_SHOW);
	m_pkBtnPackageDownload->SetNoPress(true);
	m_pkBtnPackageDownload->Invalidate();

	iStartX = 16;
	iStartY = 330;

	iWidth = 85;
	iHeight = 24;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+iHeight); 

	m_pkBtnSelectServer->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_DOWNLOAD_SERVER_RESELECT);
	m_pkBtnSelectServer->EnableWindow(TRUE);
	m_pkBtnSelectServer->ShowWindow(SW_SHOW);
	m_pkBtnSelectServer->SetNoPress(true);
	m_pkBtnSelectServer->Invalidate();


	

}

void CGCDownLoadDlg::OnStartDownload()
{
	//std::wstring wstr = m_spStrLoader->GetString( STR_ID_PATCH_END );
	m_stServerName.SetWindowText(KGCGlobalValue::m_wstrSelectServerName);
	m_stPackage.SetWindowText(std::wstring(m_spStrLoader->GetString( STR_ID_CHECK_VERSION_DIFF )));

	InvalidateRect(&m_stServerName.m_kRect);
	InvalidateRect(&m_stPackage.m_kRect);
	ShowWeb(KGCGlobalValue::GetBGWeb());
}

void CGCDownLoadDlg::OnPaint()
{
	PAINTSTRUCT ps;
	CDC* pDC = BeginPaint(&ps);

	if(pDC)
	{
		CDC memDC;
		if(memDC.CreateCompatibleDC(pDC)==TRUE)
		{
			BITMAP bm;
			CDC memBackBitmapDC;
			if(memBackBitmapDC.CreateCompatibleDC(pDC)==TRUE)
			{
				m_pkBitMap->GetObject(sizeof(BITMAP), &bm);

				CBitmap* pOld = memDC.SelectObject(m_pkBitMap);

				pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);	

				CDC memProgressDC;
				int iWidth = 6.87 * (float)m_iTotalProgress;
				if(memProgressDC.CreateCompatibleDC(pDC)==TRUE) {
					CBitmap* pOldProgress = memProgressDC.SelectObject(m_pkBitMapProgress);

					pDC->TransparentBlt(16+0,306+0,6,8, &memProgressDC,0,0,6,8,RGB(255,0,255));
					pDC->TransparentBlt(16+6,306+0,iWidth,8, &memProgressDC,6,0,1,8,RGB(255,0,255));
					pDC->TransparentBlt(16+6+iWidth,306+0,6,8, &memProgressDC,7,0,6,8,RGB(255,0,255));
					memProgressDC.SelectObject(pOldProgress);
				}

				memDC.SelectObject(pOld);
				//m_pkBitMap->;
			}

			m_stServerName.DrawText(pDC,&m_kFont,DT_LEFT);
			m_stPackage.DrawText(pDC,&m_kFont,DT_LEFT);
			m_stSpeed.DrawText(pDC,&m_kFont,DT_LEFT);
			m_stTime.DrawText(pDC,&m_kFont,DT_LEFT);
		}
	}
	EndPaint(&ps);


}

void CGCDownLoadDlg::ReleaseAll()
{

}

BOOL CGCDownLoadDlg::PreTranslateMessage( MSG* pMsg )
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			return TRUE;
		else if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	else if ( pMsg->message == WM_SYSKEYDOWN ) {
		if ( pMsg->wParam == VK_F4 ) {
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

}

void CGCDownLoadDlg::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct )
{

}

BOOL CGCDownLoadDlg::OnEraseBkgnd( CDC* pDC )
{
	if ( m_bEraseBack ) {
		m_bEraseBack =false;
		return CDialog::OnEraseBkgnd(pDC);
	}
	return FALSE;
}


void CGCDownLoadDlg::ShowWeb( std::string url )
{
	m_pHtmlPR->AttachToDialog(this, CBB_DOWNLOAD_WEB_VIEW_STATIC, url.c_str() );   	
}

void CGCDownLoadDlg::HideWeb()
{
	//GetDlgItem( IDC_STATIC_HTML )->ShowWindow(SW_HIDE);	
}

void CGCDownLoadDlg::OnStart()
{
	SetTimer(1,1000 * m_iSpeedCheckTime,0);

	m_bPackageSizeCheck = false;
	m_i64CurrentDownLoad = 0;
	m_i64OldDownLoad = 0;

	WCHAR wcTemp[30];
	std::wstring strTemp = m_spStrLoader->GetString( STR_ID_DOWNLOAD_SPEED );
	strTemp += L" : ";

	if ( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * m_iSpeedCheckTime) >= 1024 && (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * 1024 * m_iSpeedCheckTime) < 1024) {
		strTemp += _itow( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * 1024 * m_iSpeedCheckTime), wcTemp, 10);
		strTemp += L"MB/s";
	}
	else if ( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * 1024 * m_iSpeedCheckTime) >= 1024 ) {
		strTemp += _itow( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * m_iSpeedCheckTime), wcTemp, 10);
		strTemp += L"GB/s";
	}
	else {
		strTemp += _itow( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * m_iSpeedCheckTime), wcTemp, 10);
		strTemp += L"KB/s";
	}

	m_stSpeed.SetWindowText(strTemp);

	INT64 i64Time = 0;
	i64Time = m_i64TotalDownLoad - m_i64CurrentDownLoad;
	int iDivisionTemp = (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (m_iSpeedCheckTime);
	i64Time = iDivisionTemp == 0 ? 0 : i64Time/iDivisionTemp;

	strTemp = m_spStrLoader->GetString( STR_ID_DOWNLOAD_TIME );
	strTemp += L" : ";
	strTemp += _itow( (int)( i64Time / 360 ), wcTemp, 10);
	strTemp += L":";
	strTemp += _itow( (int)( (i64Time % 360) / 60  ), wcTemp, 10);
	strTemp += L":";
	strTemp += _itow( (int)( i64Time % 60 ), wcTemp, 10);

	m_stTime.SetWindowText(strTemp);

	m_iTotalProgress =0;
	
	m_bEraseBack = true;
	Invalidate(TRUE);

}

void CGCDownLoadDlg::OnReselectServerBtn()
{
	m_stPackage.SetWindowText(std::wstring(m_spStrLoader->GetString( STR_ID_SERVER_SELECLT )));
	m_pParentWnd->EndModalLoop(1);

}


void CGCDownLoadDlg::OnPackageDownBtn()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_PACKAGE);
}

void CGCDownLoadDlg::OnProgress( SProgressInfo & progressinfo )
{
	//std::wstring msg = KncUtil::toWideString(progressinfo.currentfilename);
	//msg += L" ";
	//msg.append( m_pStrLoader->GetString( STR_ID_FILE_DOWNLOAD ) );
	//m_StaticMsg.SetWindowText( msg );

	//m_sProgressInfo.Assign( progressinfo );

	//m_fFileDownloadRate = m_sProgressInfo.m_currentreadlength *100.0f/ m_sProgressInfo.m_currentlength;	
	//m_fTotalRate	= m_sProgressInfo.m_totalreadlength * 100.0f/ m_sProgressInfo.m_totallength;

	////	if ( KGCGlobalValue::m_bCheckDownSpeed ) 
	////		OnDownSpeed( progressinfo );
	KLocker lock(m_csDownloadInfo);

	if ( m_i64OldDownLoad == 0 )
		m_i64OldDownLoad = progressinfo.totalreadlength;

	m_i64CurrentDownLoad = progressinfo.totalreadlength;
	m_i64TotalDownLoad = progressinfo.totallength;

	INT64 iTotalRead = (INT64)progressinfo.totalreadlength * 100;
	INT64 iTotal = progressinfo.totallength;
	int iResult = (int)(iTotalRead / iTotal);
	if ( m_bPackageSizeCheck == false ) {
		INT64 iTotalMega = (iTotal * 10)/ ( 1024 * 1024 );
		WCHAR wcTemp[30];
		

		std::wstring strTemp = m_spStrLoader->GetString( STR_ID_PACKAGE_SIZE );
		strTemp += L" : ";
		if ( (iTotalMega/10) >= 1024 ) {
			int iTemp = (int)iTotalMega/1024;
			strTemp += _itow( iTemp/10, wcTemp, 10);
			strTemp += L".";
			strTemp += _itow( iTemp%10, wcTemp, 10);
			strTemp += L"GB";
		}
		else {
			strTemp += _itow( (int)iTotalMega/10, wcTemp, 10);
			strTemp += L".";
			strTemp += _itow( (int)iTotalMega%10, wcTemp, 10);
			strTemp += L"MB";
		}
		m_stPackage.SetWindowText(strTemp);
		m_bPackageSizeCheck = true;
		InvalidateRect(&m_stPackage.m_kRect);
	}	
	
	if ( m_iTotalProgress != iResult ) {
		m_iTotalProgress = iResult;
		RECT rect;
		rect.left=16;
		rect.top = 306;
		rect.right = 719;
		rect.bottom = 314;
		InvalidateRect(&rect,FALSE);
	}



}

void CGCDownLoadDlg::OnProgress2( SProgressInfo & progressinfo )
{
//	m_iTotalProgress = (progressinfo.totalreadlength * 100) / progressinfo.totallength;

	//m_fFileDownloadRate = m_sProgressInfo.m_currentreadlength *100.0f/ m_sProgressInfo.m_currentlength;	
	//m_fTotalRate	= m_sProgressInfo.m_totalreadlength * 100.0f/ m_sProgressInfo.m_totallength;

	//	if ( KGCGlobalValue::m_bCheckDownSpeed ) 
	//		OnDownSpeed( progressinfo );

	//OnInvalidate();	
}

void CGCDownLoadDlg::OnError( std::string filename,int errorcode )
{
	//if(filename=="main.exe" && errorcode == -6)
	//	m_bMainError=true;
}

void CGCDownLoadDlg::OnComplete( bool success, bool bMsgBox )
{	

	if(success==true)
	{
		KillTimer(1);
		m_iTotalProgress = 100;
		RECT rect;
		rect.left=16;
		rect.top = 306;
		rect.right = 719;
		rect.bottom = 314;
		InvalidateRect(&rect,FALSE);

		m_stPackage.SetWindowText(m_spStrLoader->GetString( STR_ID_PATCH_END ));
		InvalidateRect(&m_stPackage.m_kRect);

		m_stTime.SetWindowText(L"");
		InvalidateRect(&m_stTime.m_kRect);
		m_stSpeed.SetWindowText(L"");
		InvalidateRect(&m_stSpeed.m_kRect);
		
	}

}


// CGCDownLoadDlg 메시지 처리기입니다.

void CGCDownLoadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KLocker lock(m_csDownloadInfo);

	if ( nIDEvent == 1 ) {
		WCHAR wcTemp[30];
		std::wstring strTemp = m_spStrLoader->GetString( STR_ID_DOWNLOAD_SPEED );
		strTemp += L" : ";

		if ( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * m_iSpeedCheckTime) >= 1024 && (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * 1024 * m_iSpeedCheckTime) < 1024) {
			strTemp += _itow( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * 1024 * m_iSpeedCheckTime), wcTemp, 10);
			strTemp += L"MB/s";
		}
		else if ( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * 1024 * m_iSpeedCheckTime) >= 1024 ) {
			strTemp += _itow( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * m_iSpeedCheckTime), wcTemp, 10);
			strTemp += L"GB/s";
		}
		else {
			strTemp += _itow( (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (1024 * m_iSpeedCheckTime), wcTemp, 10);
			strTemp += L"KB/s";
		}

		m_stSpeed.SetWindowText(strTemp);
		InvalidateRect(&m_stSpeed.m_kRect,FALSE);

		INT64 i64Time = 0;
		i64Time = m_i64TotalDownLoad - m_i64CurrentDownLoad;
		int iDivisionTemp = (int)( m_i64CurrentDownLoad - m_i64OldDownLoad) / (m_iSpeedCheckTime);
		i64Time = iDivisionTemp == 0 ? 0 : i64Time/iDivisionTemp;

		strTemp = m_spStrLoader->GetString( STR_ID_DOWNLOAD_TIME );
		strTemp += L" : ";
		strTemp += _itow( (int)( i64Time / 360 ), wcTemp, 10);
		strTemp += L":";
		strTemp += _itow( (int)( (i64Time % 360) / 60  ), wcTemp, 10);
		strTemp += L":";
		strTemp += _itow( (int)( i64Time % 60 ), wcTemp, 10);

		m_stTime.SetWindowText(strTemp);
		InvalidateRect(&m_stTime.m_kRect,FALSE);

		m_i64CurrentDownLoad = 0;
		m_i64OldDownLoad = 0;

	}

	CDialog::OnTimer(nIDEvent);
}

void CGCDownLoadDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_pkBitMap->DeleteObject();
	m_pkBitMap = NULL;

	m_pkBitMapProgress->DeleteObject();
	m_pkBitMapProgress = NULL;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CGCDownLoadDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//ON_BN_CLICKED(CBB_DOWNLOAD_PACKAGE, OnPackageDownBtn)
	//ON_BN_CLICKED(CBB_DOWNLOAD_SERVER_RESELECT, OnReselectServerBtn)

	if ( nCode == BN_CLICKED ) {
		if ( CBB_DOWNLOAD_PACKAGE == nID ) {
			OnPackageDownBtn();
		}
		else if ( CBB_DOWNLOAD_SERVER_RESELECT == nID ) { 
			OnReselectServerBtn();
		}
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

