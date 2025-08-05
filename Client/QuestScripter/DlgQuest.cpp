// DlgQuest.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgQuest.h"
#include ".\dlgquest.h"
#include "QuestScripterDlg.h"
#include "DlgTileRowCol.h"

// CDlgQuest 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgQuest, CDialog)
CDlgQuest::CDlgQuest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuest::IDD, pParent)
{
    m_bCreated = 0;

}

CDlgQuest::~CDlgQuest()
{
}

void CDlgQuest::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_ROW_COL_GROUP, m_stRowColGroup);
    DDX_Control(pDX, IDC_STATIC_ROW_COL, m_stRowCol);
    DDX_Control(pDX, IDC_BUTTON_RESIZE, m_btReSize);

    ((CButton*)GetDlgItem(IDC_RADIO_DIRECTION))->SetCheck(1);
    DDX_Control(pDX, IDC_CHECK_SORT_DIRECTION_STAGE, m_chSortDirectionStage);
}

BEGIN_MESSAGE_MAP(CDlgQuest, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_RESIZE, OnBnClickedButtonResize)    
    ON_BN_CLICKED(IDC_RADIO_DIRECTION, OnBnClickedRadioDirection)
    ON_BN_CLICKED(IDC_RADIO_STAGE, OnBnClickedRadioStage)
END_MESSAGE_MAP()


// CDlgQuest 메시지 처리기입니다.

int CDlgQuest::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
    m_dlgTileBG.Create(CDlgTileBG::IDD, this);
    m_bCreated = 1;

    return 0;
}

void CDlgQuest::SetValue(Quest *quest)
{
    m_dlgTileBG.CopyFromQuest(quest);
}

void CDlgQuest::GetValue(Quest *quest)
{
    m_dlgTileBG.CopyToQuest(quest);
}

void CDlgQuest::SetDisableControl(bool bDisable)
{
}

void CDlgQuest::SetComboBoxValue(CQuestScripterDlg *pQuestScripter)
{
}

void CDlgQuest::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgQuest::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}

void CDlgQuest::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    if( m_dlgTileBG.GetSafeHwnd() )
    {
        CRect rtClient;
        GetClientRect(rtClient);
        rtClient.left       += 10;
        rtClient.right      -= 10;
        rtClient.top        += 60;
        rtClient.bottom     -= 10;
        m_dlgTileBG.MoveWindow(rtClient, 1);
    }

    if( m_stRowColGroup.GetSafeHwnd() )
    {
        CRect rtClient;
        GetClientRect(rtClient);

        CRect rtWindowRect;
        m_stRowColGroup.GetWindowRect(rtWindowRect);
        m_dlgTileBG.ScreenToClient(rtWindowRect);
        rtWindowRect.right = rtClient.right - 10;
        rtWindowRect.top = rtClient.top + 5;
        rtWindowRect.left = rtWindowRect.right - 200;
        rtWindowRect.bottom = rtWindowRect.top + 45;
        m_stRowColGroup.MoveWindow(rtWindowRect, 1);
        m_stRowColGroup.Invalidate(1);

        m_stRowCol.GetWindowRect(rtWindowRect);
        m_dlgTileBG.ScreenToClient(rtWindowRect);
        rtWindowRect.right = rtClient.right - 130;
        rtWindowRect.top = rtClient.top + 20;
        rtWindowRect.left = rtWindowRect.right - 70;
        rtWindowRect.bottom = rtWindowRect.top + 20;
        m_stRowCol.MoveWindow(rtWindowRect, 1);
        m_stRowCol.Invalidate(1);

        m_btReSize.GetWindowRect(rtWindowRect);
        m_dlgTileBG.ScreenToClient(rtWindowRect);
        rtWindowRect.right = rtClient.right - 20;
        rtWindowRect.top = rtClient.top + 18;
        rtWindowRect.left = rtWindowRect.right - 75;
        rtWindowRect.bottom = rtWindowRect.top + 25;
        m_btReSize.MoveWindow(rtWindowRect, 1);
        m_btReSize.Invalidate(1);
    }
}

void CDlgQuest::OnBnClickedButtonResize()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDlgTileRowCol dlgTileRowCol;
    dlgTileRowCol.m_iRow = m_dlgTileBG.m_iRow;
    dlgTileRowCol.m_iCol = m_dlgTileBG.m_iCol;

    if( IDOK == dlgTileRowCol.DoModal() )
    {
        m_dlgTileBG.DestroyButtons();
        m_dlgTileBG.CreateButtons(dlgTileRowCol.m_iRow, dlgTileRowCol.m_iCol);
        SetRowColText(dlgTileRowCol.m_iRow, dlgTileRowCol.m_iCol);
    }
}

void CDlgQuest::SetRowColText(int iRow, int iCol)
{
    CString strRowCol;
    strRowCol.Format(L"%d X %d", iRow, iCol);
    m_stRowCol.SetWindowText(strRowCol);
}

void CDlgQuest::OnBnClickedRadioDirection()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_dlgTileBG.SetDirectionMode();

}

void CDlgQuest::OnBnClickedRadioStage()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_dlgTileBG.SetStageMode();
}
