// TextureViewDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"

#include <cmath>
#include "UI Script Studio.h"
#include "MagnificationDlg.h"
#include "KWndManager.h"


extern KWndManager*	g_WndMgr;

// CMagnifiCationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMagnifiCationDlg, CDialog)
CMagnifiCationDlg::CMagnifiCationDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMagnifiCationDlg::IDD, pParent)
{
    m_pDc = NULL;
	m_iMagnification = 40;
    m_MousePoint= &(CPoint(0,0));
    m_bOnRender = false;
}

CMagnifiCationDlg::~CMagnifiCationDlg()
{
}

void CMagnifiCationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMagnifiCationDlg, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



BOOL CMagnifiCationDlg::OnInitDialog()
{
    /*int iEdge = GetSystemMetrics( SM_CYEDGE );
    int m_iTopEdge= GetSystemMetrics( SM_CYCAPTION ) + iEdge + 20;
    int m_iLeftEdge = GetSystemMetrics( SM_CXEDGE );*/
	CDialog::OnInitDialog();
	return false;
}
//int CMagnifiCationDlg::GetMenuBarHeight()
//{
//    int iEdge = GetSystemMetrics( SM_CYEDGE );
//    int iHeight = GetSystemMetrics( SM_CYCAPTION ) + iEdge + 20;
//
//    return iHeight;
//}
void CMagnifiCationDlg::SetEdgeValue(int top, int left)
{
    m_iTopEdge = top * 2 - 4;
    m_iLeftEdge = left;
}
void CMagnifiCationDlg::CaptureScreen()
{
    int cx, cy;
    CDC memDC;
    CBitmap bitmap;
    CRect ParentRect;
    CRect ChildRect;

    HCURSOR hCursor = GetCursor();

    //자식의 크기를 구해온다.
    this->GetClientRect(ChildRect);

    //부모(툴)의 크기를 구해온다.    
    GetParent()->GetWindowRect(ParentRect);
    GetCursorPos(m_MousePoint);
    
    m_MousePoint->x = m_MousePoint->x - ParentRect.left - m_iLeftEdge;
    m_MousePoint->y = m_MousePoint->y - ParentRect.top - m_iTopEdge ;

    //마우스 위,아래,좌,우 얼만큼의 영역을 그릴지 정한다.
    m_DrawScaleRect.left   = m_MousePoint->x - m_iMagnification;
    m_DrawScaleRect.right  = m_MousePoint->x + m_iMagnification;
    m_DrawScaleRect.top    = m_MousePoint->y - m_iMagnification;
    m_DrawScaleRect.bottom = m_MousePoint->y + m_iMagnification;

    
    //화면의 DC를 만들고
    CClientDC dc(this);

    //부모의 DC를 얻어온다.
    //GetDlgItem(IDD_TEXTUREVIEW);
    CClientDC ScreenDC(GetParent());//GetDesktopWindow());

    //부모의 DC와 호환되는 DC를 만들고
    memDC.CreateCompatibleDC(&ScreenDC);

    //부모의 DC와 호환되는 비트맵을 만든다.
    bitmap.CreateCompatibleBitmap(&ScreenDC, ChildRect.Width(), ChildRect.Height());

    //비트맵 DC를 선택하고
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    //캡쳐해온 화면을 비트맵에 원하는 배율로 복사한다.
    memDC.StretchBlt(ChildRect.left,ChildRect.top,ChildRect.Width(),ChildRect.Height(),&ScreenDC,
        m_DrawScaleRect.left,m_DrawScaleRect.top,m_DrawScaleRect.Width(),m_DrawScaleRect.Height(), SRCCOPY);

    //화면에 그린다. 화면에 그릴때 윈도우가 이동하는 만큼 + 해줘야 한다. 현재는 딱 정해진 위치 만큼만 그려진다.
    //MoveWindow 메시지를 받아와서 좌상단의 좌표를 항상 갱신 받아서 bitblt에 더해줘야 한다.
    
    dc.BitBlt(0, 0, ChildRect.Width(), ChildRect.Height(), &memDC, ChildRect.left,ChildRect.top, SRCCOPY);
    DrawIcon(dc, ChildRect.Width() * 0.5f , ChildRect.Height() * 0.5f , (HICON)hCursor);

    memDC.SelectObject(pOldBitmap);


    
}
BOOL CMagnifiCationDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if( GetKeyState(VK_LCONTROL) & 0x8000 )
    {
        if( zDelta > 0 )
        {
            SetScale(2);
        }
        else
        {
            SetScale(-2);
        }

    }
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CMagnifiCationDlg::SetZoom(int zoom)
{
    m_iMagnification = zoom;
}



void CMagnifiCationDlg::SetScale(int scale)
{
    if(m_iMagnification > 3)
        m_iMagnification += scale;

    if(m_iMagnification < 3 && m_iMagnification > 0)
        m_iMagnification = 6;
}
const int CMagnifiCationDlg::GetScale()
{
    return m_iMagnification;
}

void CMagnifiCationDlg::SetRenderState(bool brender)
{
    m_bOnRender = brender;
}
bool CMagnifiCationDlg::GetRenderState()
{

    return m_bOnRender;
}

void CMagnifiCationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    DefWindowProc(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

    CDialog::OnLButtonDown(nFlags, point);
}
void CMagnifiCationDlg::OnPaint(){}

void CMagnifiCationDlg::OnRButtonDown(UINT nFlags, CPoint point){}
void CMagnifiCationDlg::OnLButtonUp(UINT nFlags, CPoint point){}
void CMagnifiCationDlg::OnRButtonUp(UINT nFlags, CPoint point){}
void CMagnifiCationDlg::OnMouseMove(UINT nFlags, CPoint point){}