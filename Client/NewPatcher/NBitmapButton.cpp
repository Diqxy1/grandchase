// NBitmapButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NBitmapButton.h"
#include "resource.h"
#include "GCPatcherDlg.h"
#ifdef GRID_DOWN
#include "GCGridPatcherDlg.h"
#endif
#include "dbg/dbg.hpp"

// CNBitmapButton

IMPLEMENT_DYNAMIC(CNBitmapButton, CButton)
CNBitmapButton::CNBitmapButton()
: m_bCapture(FALSE)
, m_bNoPress(FALSE)
{
	m_bHover = FALSE;
	m_bPush = FALSE;
	m_bToolTip = FALSE;
    m_bDisable = FALSE;

    for( int i = 0 ; i < ID_BTNDISABLE + 1 ; ++i )
    {
        m_pBitmap[i] = NULL;
    }    
}

CNBitmapButton::~CNBitmapButton()
{
    for ( int i = 0; i < ID_BTNDISABLE + 1; i++ )
    {
        if( m_pBitmap[i] != NULL )
            m_pBitmap[i]->DeleteObject();
    }
}


BEGIN_MESSAGE_MAP(CNBitmapButton, CButton)
	//ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_DRAWITEM_REFLECT()
ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()



// CNBitmapButton 메시지 처리기입니다.
void CNBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/)
{
	if ((int)lpDIS->itemID < 0)
		return; 

	ASSERT(lpDIS->CtlType == ODT_BUTTON);	

	CDC *pDC = CDC::FromHandle(lpDIS->hDC);  // dc 를 얻어옴;

	CRect rcBtn(lpDIS->rcItem);

	int nBtnID = 0;

	if (lpDIS->itemState & ODS_DISABLED || m_bDisable)
		nBtnID = ID_BTNDISABLE;
	else if (m_bPush)
		nBtnID = ID_BTNDOWN;
	else if (m_bHover)
		nBtnID = ID_BTNHOVER;
	else
		nBtnID = ID_BTNUP;

	if (!m_pBitmap[nBtnID]->m_hObject)
		nBtnID = ID_BTNUP;
			
	if (!m_pBitmap[nBtnID]->m_hObject)
		return;

	DrawBitmap(nBtnID, rcBtn, pDC);
}

// 비트맵을 설정한다.

//UINT nIDUp = 0, UINT nIDDown = 0, UINT nIDFocus = 0, UINT nIDHover = 0, UINT nIDDisable = 0)
void CNBitmapButton::LoadBitmaps( Komfile * massfile, std::string strBtnName_ )
{
	SetButtonStyle(BS_OWNERDRAW, TRUE);
    std::string strPath_        = "patcher";
    std::string strBtn[ID_BTNDISABLE + 1];
    strBtn[ID_BTNUP]        = strPath_ + strBtnName_ + std::string( "Up" ) + std::string( ".bmp" );
    strBtn[ID_BTNDOWN]      = strPath_ + strBtnName_ + std::string( "Down" ) + std::string( ".bmp" );
    strBtn[ID_BTNHOVER]     = strPath_ + strBtnName_ + std::string( "Hover" ) + std::string( ".bmp" );
    strBtn[ID_BTNDISABLE]   = strPath_ + strBtnName_ + std::string( "Disable" ) + std::string( ".bmp" );
    for ( int i = 0; i < ID_BTNDISABLE + 1; i++ )
    {
        LoadBitmapFromMassFile(massfile,strBtn[i], &m_pBitmap[i] );
        if( m_pBitmap[i] == NULL )
        {
            START_LOG( cerr, strBtnName_ <<" Load Error : " << i );
        }
        //m_pBitmap[i] = g_pPatchDlg->LoadBmp( strBtn[i].c_str(), true );
    }
    BITMAP bm;
    m_pBitmap[ID_BTNUP]->GetObject(sizeof(BITMAP), &bm);
    SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE);
}

// 클릭했을 경우
void CNBitmapButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bPush = TRUE;

	m_bHover = FALSE;
	if (m_bCapture)
	{
		m_bCapture = FALSE;
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

// 마우스를 움직였을 경우
void CNBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( m_bNoPress ){
		SetPush(FALSE);
	}
	if (m_bPush) return;

	CRect rc;
	GetClientRect(rc);

	CDC * pDC = GetDC();

	// 현재 마우스의 움직음을 캡쳐 중이지 않을 경우
	// 즉 마으스 커서가 막 버튼위로 올라왔을 경우
	if (!m_bCapture)
	{
		SetCapture();
		m_bCapture = TRUE;
		//if (!m_bHover)
		//{
			m_bHover = TRUE;
			DrawBitmap(ID_BTNHOVER, rc, pDC);
	//		SetFocus();
		//}
	}
	
	// 현재 마우스의 움직임을 캡쳐 중일 경우
	// 즉 마우스 커서가 버튼 위에 있을 경우
	if (m_bCapture)
	{
		// 밖으로 벗어났을 경우
		if (!rc.PtInRect(point))
		{
			//if (m_bHover)
			//{
				m_bHover = FALSE;
				DrawBitmap(ID_BTNUP, rc, pDC);
				ReleaseCapture();
				m_bCapture = FALSE;
			//}
		}
	}

	ReleaseDC(pDC);
	CButton::OnMouseMove(nFlags, point);
}

// 눌려졌는지를 설정한다.
// 클릭했을 경우 자동으로 눌려졌다고 설정이 되기 때문에 
// 그걸 풀어주는게 필요하다.
// 풀려는 용도로 사용된다.
void CNBitmapButton::SetPush(bool bPush)
{
	m_bPush = bPush;
	Invalidate();
}

BOOL CNBitmapButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_bToolTip) m_MyToolTipCtrl.RelayEvent(pMsg);
	return CButton::PreTranslateMessage(pMsg);
}

void CNBitmapButton::SetToolTipText(CString strMessage)
{
	if (!m_MyToolTipCtrl.m_hWnd) 
	{
		m_MyToolTipCtrl.Create(this);
		m_MyToolTipCtrl.Activate (TRUE);
	}
	CRect rc;
	GetClientRect(rc);
	m_MyToolTipCtrl.AddTool(this, strMessage, &rc, 1);
	m_bToolTip = TRUE;
}

void CNBitmapButton::DrawBitmap(UINT nID, CRect rcBtn, CDC* pDC)
{
	if(pDC)
	{
		CDC memDC;
		if(memDC.CreateCompatibleDC(pDC)==TRUE)
		{
			BITMAP bm;
			m_pBitmap[nID]->GetObject(sizeof(BITMAP), &bm);

			CBitmap* pOld = memDC.SelectObject(m_pBitmap[nID]);

#if defined( _CHINA ) || defined(_CHINA_TEST)
			pDC->StretchBlt(rcBtn.left, rcBtn.top, rcBtn.Width(), rcBtn.Height(), &memDC, 
				0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
#else
			pDC->StretchBlt(rcBtn.left, rcBtn.top, rcBtn.Width(), rcBtn.Height(), &memDC, 
				rcBtn.left, rcBtn.top, rcBtn.Width(), rcBtn.Height(), SRCCOPY);
#endif				

			memDC.SelectObject(pOld);    
		}
	}
}

BOOL CNBitmapButton::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    return FALSE;

    return CButton::OnEraseBkgnd(pDC);
}

void CNBitmapButton::OnSetFocus(CWnd* pOldWnd)
{
    CButton::OnSetFocus(pOldWnd);

    //SendMessage(WM_KILLFOCUS, NULL);
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CNBitmapButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if ( m_bNoPress ){
        SetPush(FALSE);
    }

    CButton::OnLButtonUp(nFlags, point);
}

//void CNBitmapButton::OnBnClicked()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	int i = 0;
//}

//void CNBitmapButton::OnBnKillfocus()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CRect rc;
//	GetClientRect(rc);
//
//	CDC * pDC = GetDC();
//
//	m_bHover = FALSE;
//	DrawBitmap(ID_BTNUP, rc, pDC);
//	if ( m_bCapture ) {
//		ReleaseCapture();
//		m_bCapture = FALSE;
//	}
//}

void CNBitmapButton::OnCaptureChanged(CWnd *pWnd)
{
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    CRect rc;
    GetClientRect(rc);

    CDC * pDC = GetDC();

    m_bHover = FALSE;
    DrawBitmap(ID_BTNUP, rc, pDC);
    if ( m_bCapture ) {
        ReleaseCapture();
        m_bCapture = FALSE;
    }
    CButton::OnCaptureChanged(pWnd);
}

void CNBitmapButton::SetDisable( bool bDisable )
{
    m_bDisable = bDisable;
    Invalidate();
}


IMPLEMENT_DYNAMIC(CNBitmapDecoButton, CButton)
CNBitmapDecoButton::CNBitmapDecoButton()
: m_bPush(FALSE)
, m_bCapture(FALSE)
, m_bHover(FALSE)
{
    for( int i = 0 ; i < 2 ; ++i )
    {
        m_pBitmap[i] = NULL;
    }    

}

CNBitmapDecoButton::~CNBitmapDecoButton()
{

}

BEGIN_MESSAGE_MAP(CNBitmapDecoButton, CButton)
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_DRAWITEM_REFLECT()
    //	ON_CONTROL_REFLECT(BN_CLICKED, &CNBitmapDecoButton::OnBnClicked)
END_MESSAGE_MAP()

BOOL CNBitmapDecoButton::PreTranslateMessage( MSG* pMsg )
{
    return CButton::PreTranslateMessage(pMsg);
}

void CNBitmapDecoButton::LoadBitmaps( Komfile * massfile,std::string strBackName, std::map<int,std::pair<std::string,CRect>> mapDecoName_ )
{
    SetButtonStyle(BS_OWNERDRAW, TRUE);
    std::string strPath_        = "server";
    std::string strBtn[3];
    strBtn[ID_BTNUP]        = strPath_ + strBackName + std::string( "Up" ) + std::string( ".bmp" );
    strBtn[ID_BTNDOWN]      = strPath_ + strBackName + std::string( "Down" ) + std::string( ".bmp" );
    strBtn[ID_BTNHOVER]      = strPath_ + strBackName + std::string( "Hover" ) + std::string( ".bmp" );

    for ( int i = 0; i < 3 ; i++ )
    {
        LoadBitmapFromMassFile(massfile,strBtn[i], &m_pBitmap[i] );
    }

    for ( std::map<int,std::pair<std::string,CRect>>::iterator mit = mapDecoName_.begin() ; mit != mapDecoName_.end() ; ++mit ){
        m_mapDeco[mit->first] = NULL;
        m_mapDecoRenderPos[mit->first] = std::pair<bool, CRect>(FALSE, mit->second.second);
        LoadBitmapFromMassFile(massfile, mit->second.first, &m_mapDeco[mit->first] );
    }

    BITMAP bm;
    m_pBitmap[ID_BTNUP]->GetObject(sizeof(BITMAP), &bm);
    SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE);

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

    m_stName.SetTextColor(RGB(255,255,255));
}


BOOL CNBitmapDecoButton::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    return FALSE;
}

void CNBitmapDecoButton::DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/)
{
    if ((int)lpDIS->itemID < 0)
        return; 

    CDC *pDC = CDC::FromHandle(lpDIS->hDC);  // dc 를 얻어옴;

    CRect rcBtn(lpDIS->rcItem);

    int nBtnID = ID_BTNUP;

    if (lpDIS->itemState & ODS_DISABLED)
        nBtnID = ID_BTNUP;
    else if (m_bPush)
        nBtnID = ID_BTNDOWN;
    else if (m_bHover)
        nBtnID = ID_BTNHOVER;
    else
        nBtnID = ID_BTNUP;

    if (!m_pBitmap[nBtnID]->m_hObject)
        nBtnID = ID_BTNUP;

    if (!m_pBitmap[nBtnID]->m_hObject)
        return;

    DrawBitmap(nBtnID, rcBtn, pDC);
}

void CNBitmapDecoButton::DrawBitmap(UINT nID, CRect rcBtn, CDC* pDC)
{
    if(pDC)
    {
        CDC memDC;
        if(memDC.CreateCompatibleDC(pDC)==TRUE)
        {
            BITMAP bm;
            m_pBitmap[nID]->GetObject(sizeof(BITMAP), &bm);

            CBitmap* pOld = memDC.SelectObject(m_pBitmap[nID]);

            pDC->TransparentBlt(rcBtn.left, rcBtn.top, rcBtn.Width(), rcBtn.Height(), &memDC, 
                0, 0, bm.bmWidth, bm.bmHeight,  RGB(255,0,255));
            memDC.SelectObject(pOld);    

            for ( std::map<int,std::pair<bool, CRect>>::iterator mit = m_mapDecoRenderPos.begin() ; mit != m_mapDecoRenderPos.end() ; ++mit ) {
                if ( mit->second.first == FALSE ) continue;

                std::map<int, CBitmap*>::iterator mitBmp = m_mapDeco.find(mit->first);
                if ( mitBmp == m_mapDeco.end() ) continue;

                BITMAP bmp;
                CDC bmpdc;
                mitBmp->second->GetBitmap(&bmp);
                if(bmpdc.CreateCompatibleDC(pDC) == TRUE)
                {
                    CBitmap * pOldBmp = bmpdc.SelectObject(mitBmp->second);
                    pDC->TransparentBlt( mit->second.second.left, mit->second.second.top, mit->second.second.Width(), mit->second.second.Height(), &bmpdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255,0,255));
                    bmpdc.SelectObject(pOldBmp);

                }
                bmpdc.DeleteDC();

            }
            m_stName.DrawText(pDC,&m_kFont,DT_CENTER);

        }
        memDC.DeleteDC();
    }
}

void CNBitmapDecoButton::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if ( m_bPush ) return;

    CRect rc;
    GetClientRect(rc);

    CDC * pDC = GetDC();


    if (!m_bCapture)
    {
        SetCapture();
        m_bCapture = TRUE;
        m_bHover = FALSE;
        DrawBitmap(ID_BTNHOVER, rc, pDC);
    }

    if (m_bCapture)
    {
        if (!rc.PtInRect(point))
        {
            DrawBitmap(ID_BTNUP, rc, pDC);
            ReleaseCapture();
            m_bCapture = FALSE;
        }
    }

    ReleaseDC(pDC);
    CButton::OnMouseMove(nFlags, point);
}
void CNBitmapDecoButton::OnBnClicked()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    m_bPush = !m_bPush;

    CRect rc;
    GetClientRect(rc);

    CDC * pDC = GetDC();

    int nBtnID = ID_BTNUP;
    if (m_bPush)
        nBtnID = ID_BTNDOWN;
    else 
        m_bCapture = FALSE;

    DrawBitmap(nBtnID, rc, pDC);

}

void CNBitmapDecoButton::OnBtOff()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if ( m_bPush == FALSE) return;

    m_bPush = FALSE;

    CRect rc;
    GetClientRect(rc);

    CDC * pDC = GetDC();

    int nBtnID = ID_BTNUP;

    m_bCapture = FALSE;

    DrawBitmap(nBtnID, rc, pDC);

    InvalidateRect(rc);
}

void CNBitmapDecoButton::OnBtOn()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if ( m_bPush == TRUE) return;

    m_bPush = TRUE;

    CRect rc;
    GetClientRect(rc);

    CDC * pDC = GetDC();

    int nBtnID = ID_BTNDOWN;

    DrawBitmap(nBtnID, rc, pDC);

    InvalidateRect(rc);

}

void CNBitmapDecoButton::SetDecoRender( int iID, bool bRender )
{
    std::map<int,std::pair<bool, CRect>>::iterator mit = m_mapDecoRenderPos.find(iID);
    if ( mit == m_mapDecoRenderPos.end() ) return;

    mit->second.first = bRender;
}

void CNBitmapDecoButton::SetName( CRect rt, std::wstring strName )
{
    m_stName.SetWindowText(strName);
    m_stName.CalcRect(this);
    m_stName.CalcRect(rt);
    m_stName.ShowWindow(SW_SHOW);
}

void CNBitmapDecoButton::DoDataExchange(CDataExchange* pDX)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

    CButton::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATIC, m_stName);

}

void CNBitmapDecoButton::OnCaptureChanged(CWnd *pWnd)
{
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    CRect rc;
    GetClientRect(rc);

    CDC * pDC = GetDC();

    m_bHover = FALSE;
    DrawBitmap(ID_BTNUP, rc, pDC);
    if ( m_bCapture ) {
        ReleaseCapture();
        m_bCapture = FALSE;
    }
    CButton::OnCaptureChanged(pWnd);
}
