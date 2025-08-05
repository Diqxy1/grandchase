// GCServerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCServerDlg.h"
#include "NBitmapButton.h"
#include "patch.h"
#include "GCGlobalValue.h"

// CGCServerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGCServerDlg, CDialog)

CGCServerDlg::CGCServerDlg(CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass)
	: CDialog(CGCServerDlg::IDD, pParent)
	, m_pMassfile(imagemass)
{
    m_hStartBtn = NULL;
	CreateLinkButtonList();

}

CGCServerDlg::~CGCServerDlg()
{
	ReleaseAll();
}

void CGCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_SERVER_NAME, m_stServerName);

}


BEGIN_MESSAGE_MAP(CGCServerDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	
END_MESSAGE_MAP()


// CGCServerDlg 메시지 처리기입니다.

BOOL CGCServerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;

//	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CGCServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowLong( m_hWnd, GWL_EXSTYLE, ::GetWindowLong( m_hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB( 255, 0, 255), 0, LWA_COLORKEY/* | LWA_ALPHA */);

	LoadBitmapFromMassFile( m_pMassfile.get(),"bg01.bmp", &m_pkBitMap );

	SetLinkButtonPosition();
	SetLinkButtonInfo();
	
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
	int iNameStartX = 210;
	int iNameStartY = 327;
	int iNameWidth = 100;
	int iNameHeight = 38;
	rName.SetRect(iNameStartX,iNameStartY,iNameStartX+iNameWidth,iNameStartY+iNameHeight); 

	m_stServerName.CalcRect(this);
	m_stServerName.CalcRect(rName);
	m_stServerName.SetTextColor(RGB(255,255,255));

	m_stServerName.ShowWindow(SW_SHOW);

	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGCServerDlg::ReleaseAll()
{
	if( m_pkBitMap != NULL )    
		m_pkBitMap->DeleteObject();		

	for ( std::map<int, CNBitmapDecoButton*>::iterator mit = m_mapServerButton.begin() ; mit != m_mapServerButton.end() ; ++mit ) {
		SAFE_DELETE( mit->second);
	}

	m_mapServerButton.clear();
}



void CGCServerDlg::CreateLinkButtonList()
{
	if ( m_mapServerButton.empty() == false ) return;


	std::map<int,std::pair<std::string,CRect>> mapDecoName_;
	
	int iWidth = 35;
	int iHeight = 37;
	int iStartX = 1;
	int iStartY = 1;
	mapDecoName_[0] = std::pair<std::string,CRect>("newsever.bmp", CRect(iStartX,iStartY,iStartX + iWidth, iStartY + iHeight));

	iWidth = 60;
	iHeight = 20;
	iStartX = 145;
	iStartY = 50;
	mapDecoName_[1] = std::pair<std::string,CRect>("sever_busy.bmp", CRect(iStartX,iStartY,iStartX + iWidth, iStartY + iHeight));
	mapDecoName_[2] = std::pair<std::string,CRect>("sever_free.bmp", CRect(iStartX,iStartY,iStartX + iWidth, iStartY + iHeight));
	mapDecoName_[3] = std::pair<std::string,CRect>("sever_full.bmp", CRect(iStartX,iStartY,iStartX + iWidth, iStartY + iHeight));
	mapDecoName_[4] = std::pair<std::string,CRect>("sever_repair.bmp", CRect(iStartX,iStartY,iStartX + iWidth, iStartY + iHeight));
	
	m_mapServerButton[CBB_SEVER_BUTTON_N_1] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_N_1]->LoadBitmaps(m_pMassfile.get(), "Btn01",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_N_2] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_N_2]->LoadBitmaps(m_pMassfile.get(), "Btn02",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_N_3] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_N_3]->LoadBitmaps(m_pMassfile.get(), "Btn03",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_N_4] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_N_4]->LoadBitmaps(m_pMassfile.get(), "Btn04",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_N_5] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_N_5]->LoadBitmaps(m_pMassfile.get(), "Btn05",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_N_6] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_N_6]->LoadBitmaps(m_pMassfile.get(), "Btn06",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_S_1] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_S_1]->LoadBitmaps(m_pMassfile.get(), "Btn07",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_S_2] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_S_2]->LoadBitmaps(m_pMassfile.get(), "Btn08",mapDecoName_);

	m_mapServerButton[CBB_SEVER_BUTTON_S_3] = new CNBitmapDecoButton();
	m_mapServerButton[CBB_SEVER_BUTTON_S_3]->LoadBitmaps(m_pMassfile.get(), "Btn09",mapDecoName_);

}

void CGCServerDlg::SetLinkButtonPosition()
{
	if ( m_mapServerButton.empty() == true ) return;

	CRect rect;
	int iStartX = 102;
	int iStartY = 12;

	int iWidth = 221;
	int iHeight = 85;

	int iGapX = 8;
	int iGapY = 15;

// 	int arStartX[3] = { 102, 318, 534 };
// 	int arStartY[3] = { 12, 96, 220 };

    int arStartX[3] = { 55, 282, 509 };
    int arStartY[2] = { 73, 195 };

	for ( int i = CBB_SEVER_BUTTON_N_1 ; i < CBB_SEVER_BUTTON_MAX ; ++i ) {
		int iPosIndexX = i - CBB_SEVER_BUTTON_N_1;
		iPosIndexX = iPosIndexX % 3;

		int iPosIndexY = i - CBB_SEVER_BUTTON_N_1;
		iPosIndexY = iPosIndexY / 3;

		rect.SetRect(arStartX[iPosIndexX],arStartY[iPosIndexY],arStartX[iPosIndexX]+iWidth,arStartY[iPosIndexY]+iHeight); 
		m_mapServerButton[i]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,i);
		//m_mapServerButton[i]->NotifyWinEvent()
	}

	




}

void CGCServerDlg::OnPaint()
{
	PAINTSTRUCT ps;
	CDC* pDC = BeginPaint(&ps);

	if(pDC)
	{
		CDC memDC;
		if(memDC.CreateCompatibleDC(pDC)==TRUE)
		{
			BITMAP bm;
			m_pkBitMap->GetObject(sizeof(BITMAP), &bm);

			CBitmap* pOld = memDC.SelectObject(m_pkBitMap);

			pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 
				0, 0,SRCCOPY);

			memDC.SelectObject(pOld);    

			m_stServerName.DrawText(pDC,&m_kFont,DT_CENTER);

		}
	}
	EndPaint(&ps);

}

BOOL CGCServerDlg::PreTranslateMessage(MSG* pMsg)
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

void CGCServerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGCServerDlg::SetLinkButtonInfo()
{
	//각 서버 버튼 UID 부여하기
	int iServiceType[5] = {CBB_SEVER_BUTTON_NONE, CBB_SEVER_BUTTON_N_1,CBB_SEVER_BUTTON_N_1+6,CBB_SEVER_BUTTON_N_1+6,CBB_SEVER_BUTTON_N_1+6};
	for ( std::vector<SServerSelectInfo>::iterator vit = KGCGlobalValue::m_vecServerInfo.begin() ; vit != KGCGlobalValue::m_vecServerInfo.end() ; ++vit ) {
		KGCGlobalValue::m_mapServerInfo[iServiceType[vit->iServiceType]] = *vit;
		iServiceType[vit->iServiceType] += 1;
	}
	CRect rName;
	int iNameStartX = 110;
	int iNameStartY = 23;
	int iNameWidth = 100;
	int iNameHeight = 38;
	rName.SetRect(iNameStartX,iNameStartY,iNameStartX+iNameWidth,iNameStartY+iNameHeight); 
	//서버 상태 정하기 ( 접속자 많음/신 서버/추천 서버 등 )
	for ( std::map<int,SServerSelectInfo>::iterator mit = KGCGlobalValue::m_mapServerInfo.begin() ; mit != KGCGlobalValue::m_mapServerInfo.end() ; ++mit ) {
		if ( mit->second.iServerState == 10 ) {
			continue;
		}
		m_mapServerButton[mit->first]->EnableWindow(TRUE);
		m_mapServerButton[mit->first]->ShowWindow(SW_SHOW);
		m_mapServerButton[mit->first]->SetName(rName,mit->second.wstrServerName);
		if ( mit->second.bNewServer ) 
			m_mapServerButton[mit->first]->SetDecoRender(0,TRUE);
		
		if ( mit->second.iServerState == 0 ) 
			m_mapServerButton[mit->first]->SetDecoRender(3,TRUE);
		else if ( mit->second.iServerState == 1 ) 
			m_mapServerButton[mit->first]->SetDecoRender(1,TRUE);
		else if ( mit->second.iServerState == 4 ) 
			m_mapServerButton[mit->first]->SetDecoRender(4,TRUE);
		else
			m_mapServerButton[mit->first]->SetDecoRender(2,TRUE);
	}
	//정보가 없는 버튼 끄는 부분
	for ( std::map<int, CNBitmapDecoButton*>::iterator mit = m_mapServerButton.begin() ; mit != m_mapServerButton.end() ; ++mit ) {	
		std::map<int,SServerSelectInfo>::iterator mitInfo = KGCGlobalValue::m_mapServerInfo.find(mit->first);
		if ( mitInfo != KGCGlobalValue::m_mapServerInfo.end() ) {
			continue;
		}
		mit->second->ShowWindow(SW_HIDE);
		mit->second->EnableWindow(FALSE);	
	}
	//추천 서버가 있을 경우 초기에 선택 하여준다.
	for ( std::map<int, CNBitmapDecoButton*>::iterator mit = m_mapServerButton.begin() ; mit != m_mapServerButton.end() ; ++mit ) {	
		std::map<int,SServerSelectInfo>::iterator mitInfo = KGCGlobalValue::m_mapServerInfo.find(mit->first);
		if ( mitInfo != KGCGlobalValue::m_mapServerInfo.end() ) {
			if ( mitInfo->second.bRecomServer ) {
				mit->second->OnBtOn();
				KGCGlobalValue::m_iSelectServerIndex = mit->first;
				KGCGlobalValue::m_strSelectIP = GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[mit->first].strServerIP.c_str());
				KGCGlobalValue::m_strSelectPort =GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[mit->first].strServerPort.c_str());
				KGCGlobalValue::m_strDownLoadLink = GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[mit->first].strDownloadLink.c_str());
				KGCGlobalValue::m_strDownLoadLinkFileName = GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[mit->first].strDownloadLinkFile.c_str());

				
				
				m_stServerName.SetWindowText(KGCGlobalValue::m_mapServerInfo[mit->first].wstrServerName);
				KGCGlobalValue::m_iSelectServerState = KGCGlobalValue::m_mapServerInfo[mit->first].iServerState;
				KGCGlobalValue::m_wstrSelectServerName = KGCGlobalValue::m_mapServerInfo[mit->first].wstrServerName;

				InvalidateRect(&m_stServerName.m_kRect);
				break;
			}
		}
	}
}

BOOL CGCServerDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( nCode == BN_CLICKED && KGCGlobalValue::m_iSelectServerIndex != nID) {
		for ( std::map<int, CNBitmapDecoButton*>::iterator mit = m_mapServerButton.begin() ; mit != m_mapServerButton.end() ; ++mit ) {
			if ( mit->first == nID ) {
				mit->second->OnBtOn();
				KGCGlobalValue::m_iSelectServerIndex = nID;
				KGCGlobalValue::m_strSelectIP = GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[nID].strServerIP.c_str());
				KGCGlobalValue::m_strSelectPort =GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[nID].strServerPort.c_str());
				KGCGlobalValue::m_strDownLoadLink = GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[nID].strDownloadLink.c_str());
				KGCGlobalValue::m_strDownLoadLinkFileName = GCStrWideToChar(KGCGlobalValue::m_mapServerInfo[nID].strDownloadLinkFile.c_str());

				m_stServerName.SetWindowText(KGCGlobalValue::m_mapServerInfo[nID].wstrServerName);
				KGCGlobalValue::m_iSelectServerState = KGCGlobalValue::m_mapServerInfo[nID].iServerState;
				KGCGlobalValue::m_wstrSelectServerName = KGCGlobalValue::m_mapServerInfo[nID].wstrServerName;
				InvalidateRect(&m_stServerName.m_kRect);

                ::SendMessage(m_hStartBtn, WM_DISABLE_BTN, NULL, NULL);
			}
			else {
				mit->second->OnBtOff();
			}
		}
		
	}

	if ( nCode == BN_DBLCLK ) {
		for ( std::map<int, CNBitmapDecoButton*>::iterator mit = m_mapServerButton.begin() ; mit != m_mapServerButton.end() ; ++mit ) {
			if ( mit->first == nID ) {
				mit->second->OnBtOn();
				KGCGlobalValue::m_iSelectServerIndex = nID;
				m_stServerName.SetWindowText(KGCGlobalValue::m_mapServerInfo[nID].wstrServerName);
				KGCGlobalValue::m_iSelectServerState = KGCGlobalValue::m_mapServerInfo[nID].iServerState;
				KGCGlobalValue::m_wstrSelectServerName = KGCGlobalValue::m_mapServerInfo[nID].wstrServerName;

			}
			else {
				mit->second->OnBtOff();
			}
		}
        if ( KGCGlobalValue::m_iSelectServerState != 4 )
            m_pParentWnd->EndModalLoop(0);

	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
