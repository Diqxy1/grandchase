// DlgPaletteObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgPaletteObject.h"
#include ".\dlgpaletteobject.h"
#include "xSkinButton.h"

// CDlgPaletteObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPaletteObject, CDialog)
CDlgPaletteObject::CDlgPaletteObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaletteObject::IDD, pParent)
{
}

CDlgPaletteObject::~CDlgPaletteObject()
{
    for(int i=0;i<(int)m_vtButton.size();i++)
    {
        m_vtButton[i]->DestroyWindow();
        SAFE_DELETE(m_vtButton[i]);
    }
    m_vtButton.clear();
}

void CDlgPaletteObject::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ID, m_cbToolPalette);
    m_cbToolPalette.Init();
}


BEGIN_MESSAGE_MAP(CDlgPaletteObject, CDialog)
    ON_MESSAGE(WM_CLICK_TOOL_BUTTON, OnClickToolButton)
    ON_CBN_SELCHANGE(IDC_COMBO_ID, OnCbnSelchangeComboId)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)

    ON_WM_SIZE()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CDlgPaletteObject 메시지 처리기입니다.

LRESULT CDlgPaletteObject::OnClickToolButton(WPARAM wParam, LPARAM lParam)
{
    if( GetParent() )
        GetParent()->PostMessage(WM_CLICK_TOOL_BUTTON, 0, lParam);

    CString strMonID;
    ((CxSkinButton*)lParam)->GetTooltipText(&strMonID);
    m_cbToolPalette.SetWindowText(strMonID);
    return S_OK;
}

void CDlgPaletteObject::OnCbnSelchangeComboId()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( m_cbToolPalette.GetCurSel() >= 0 
        && m_cbToolPalette.GetCount() > m_cbToolPalette.GetCurSel() 
        )
    {
        CString strText;
        m_cbToolPalette.GetLBText(m_cbToolPalette.GetCurSel(), strText);
        for(int i=0;i<(int)m_vtButton.size();i++)
        {
            CString strToolTip;
            m_vtButton[i]->GetTooltipText(&strToolTip);
            if( strToolTip == strText )
            {
                if( GetParent() )
                    GetParent()->PostMessage(WM_CLICK_TOOL_BUTTON, 0, (LPARAM)m_vtButton[i]);
            }
        }
    }
}

void CDlgPaletteObject::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

    SCROLLINFO scrollinfo;
    GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
    int iTotalVert = ReSizeButton(0);
    scrollinfo.nMin = 0;
    scrollinfo.nMax = iTotalVert;
    scrollinfo.cbSize = sizeof(SCROLLINFO) ;
    scrollinfo.nPage = 0;
    scrollinfo.nPos = 0;
    scrollinfo.nTrackPos = 0;
    scrollinfo.fMask = SIF_ALL;
    SetScrollInfo(SB_VERT, &scrollinfo, 1);
    Invalidate(1);
}

void CDlgPaletteObject::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    SCROLLINFO scrollinfo;
    GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
    switch(nSBCode)
    {
    case SB_LINEDOWN:
        {
            if( scrollinfo.nPos + 1 >= scrollinfo.nMax )
                scrollinfo.nPos = scrollinfo.nMax-1;
            else
                scrollinfo.nPos = scrollinfo.nPos + 1;
            SetScrollInfo(SB_VERT, &scrollinfo, 1);
            ReSizeButton(scrollinfo.nPos);
        }
        break;
    case SB_LINEUP:
        {
            if( scrollinfo.nPos - 1 > 0 )
                scrollinfo.nPos = scrollinfo.nPos - 1;
            else
                scrollinfo.nPos = 0;
            SetScrollInfo(SB_VERT, &scrollinfo, 1);
            ReSizeButton(scrollinfo.nPos);
        }
        break;
    case SB_PAGEDOWN:
        {
            if( (int)(nPos + (scrollinfo.nMax/10)) >= scrollinfo.nMax )
                scrollinfo.nPos = scrollinfo.nMax-1;
            else
                scrollinfo.nPos += (scrollinfo.nMax/10) ;
            SetScrollInfo(SB_VERT, &scrollinfo, 1);
            ReSizeButton(scrollinfo.nPos);
        }
        break;
    case SB_PAGEUP:
        {
            if( scrollinfo.nPos - (scrollinfo.nMax/10) < 0 )
                scrollinfo.nPos = 0;
            else
                scrollinfo.nPos -= (scrollinfo.nMax/10);
            SetScrollInfo(SB_VERT, &scrollinfo, 1);
            ReSizeButton(scrollinfo.nPos);
        }
        break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            scrollinfo.nPos = scrollinfo.nTrackPos;
            SetScrollInfo(SB_VERT, &scrollinfo, 1);
            ReSizeButton(scrollinfo.nPos);
        }
        break;
    }

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgPaletteObject::CreateButtonWindow(vector<TPObject> &vtTPObject)
{
    const int ibtWidth = 30;
    const int ibtHeight = 30;

    CRect rtClient;
    GetClientRect(rtClient);

    if( rtClient.Width() < ibtWidth ) return;

    for(int i=0;i<(int)m_vtButton.size();i++)
    {
        if( m_vtButton[i] )
            m_vtButton[i]->DestroyWindow();
        SAFE_DELETE(m_vtButton[i]);
    }
    m_vtButton.clear();

    CRect rtButton;
    rtButton.left       = START_POS_X;
    rtButton.right      = rtButton.left + ibtWidth;
    rtButton.top        = START_POS_Y;
    rtButton.bottom     = rtButton.top + ibtHeight;

    int iSBCnt = 0;
    for(int i=0;i<(int)vtTPObject.size();i++)
    {
        CxSkinButton *pButton;
        pButton = new CxSkinButton;

        pButton->Create("", WS_CHILD | WS_VISIBLE, rtButton, this, WM_USER+i);

        pButton->SetToolTipText(vtTPObject[i].strID);
        CString strFileName = vtTPObject[i].strBitmap;
        pButton->SetSkin(strFileName, strFileName, strFileName, strFileName, strFileName, "", 1, 1, 0);

        m_vtButton.push_back(pButton);

        // 가로 조절
        if( rtClient.Width() < (rtButton.right + ibtWidth) )
        {
            rtButton.left = START_POS_X; rtButton.right = rtButton.left + ibtWidth;
            rtButton.OffsetRect(0, ibtHeight+5);
            iSBCnt++;
        }
        else
        { rtButton.OffsetRect(ibtWidth+5, 0); }
    }

    SCROLLINFO scrollinfo;
    scrollinfo.nMin = 0;
    scrollinfo.nMax = iSBCnt;
    scrollinfo.cbSize = sizeof(SCROLLINFO) ;
    scrollinfo.nPage = 0;
    scrollinfo.nPos = 0;
    scrollinfo.nTrackPos = 0;
    scrollinfo.fMask = SIF_ALL;
    SetScrollInfo(SB_VERT, &scrollinfo, 1);
}

int CDlgPaletteObject::ReSizeButton(int iStartVerLine)
{
    const int ibtWidth = 30;
    const int ibtHeight = 30;

    CRect rtButton, rtClient;
    GetClientRect(rtClient);

    rtButton.left       = START_POS_X;
    rtButton.right      = rtButton.left + ibtWidth;
    rtButton.top        = START_POS_Y;
    rtButton.bottom     = rtButton.top + ibtHeight;

    int iVerCnt = 0;
    for(int i=0;i<(int)m_vtButton.size();i++)
    {
        if( iVerCnt < (iStartVerLine-1) )
            m_vtButton[i]->ShowWindow(SW_HIDE);
        else
            m_vtButton[i]->ShowWindow(SW_SHOW);

        if( m_vtButton[i] )
            m_vtButton[i]->MoveWindow(rtButton, 1);

        if( rtClient.Width() < (rtButton.right + ibtWidth) )
        {
            if( iVerCnt > (iStartVerLine-1) )
                rtButton.OffsetRect(0, ibtHeight+5);
            rtButton.left = START_POS_X; rtButton.right = rtButton.left + ibtWidth;
            iVerCnt++;
        }
        else
        {
            rtButton.OffsetRect(ibtWidth+5, 0);
        }
    }

    return iVerCnt;
}

BOOL CDlgPaletteObject::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    Refresh();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgPaletteObject::Refresh(void)
{
    int cnt = 0;
    m_cbToolPalette.ResetContent();
    for(map<CString, CString>::iterator itObj = m_mapObjID.begin();itObj != m_mapObjID.end();itObj++, cnt++)
    {
        m_cbToolPalette.AddSearchString(itObj->first);
        m_cbToolPalette.InsertString(cnt, itObj->first);
    }
    if( m_cbToolPalette.GetCount() > 0 ) m_cbToolPalette.SetCurSel(0);

    CreateButtonWindow(m_vtTPObject);
}

void CDlgPaletteObject::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgPaletteObject::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgPaletteObject::SetObjectID(map<CString, CString> &vtString)
{
    m_mapObjID.clear();
    int cnt = 0;
    for(map<CString, CString>::iterator itObj = vtString.begin();itObj != vtString.end();itObj++, cnt++)
        m_mapObjID[itObj->first] = itObj->second;
}

void CDlgPaletteObject::SetObjectImage(vector<TPObject> &vtTPObject)
{
    m_vtTPObject.clear();
    m_vtTPObject = vtTPObject;
}