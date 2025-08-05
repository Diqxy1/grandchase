// DlgTGNextStage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGNextStage.h"
#include ".\dlgtgnextstage.h"


// CDlgTGNextStage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGNextStage, CDlgTGBase)
CDlgTGNextStage::CDlgTGNextStage(CWnd* pParent /*=NULL*/)
{
}

CDlgTGNextStage::~CDlgTGNextStage()
{
}

void CDlgTGNextStage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_STAGE_N_PERCENT, m_lcStageNPercent);
    DDX_Control(pDX, IDC_EDIT_STAGE, m_ebStage);
    DDX_Control(pDX, IDC_EDIT_STAGE_PERCENT, m_ebPercent);
    DDX_Control(pDX, IDC_EDIT_REMAIN_PERCENT, m_ebRemainPercent);
    DDX_Control(pDX, IDC_EDIT_GANE_LV, m_ebLevel);

    m_lcStageNPercent.DeleteAllItems();
    m_lcStageNPercent.InsertColumn(0, L"스테이지", LVCFMT_LEFT, 60, 0);
    m_lcStageNPercent.InsertColumn(1, L"확    률", LVCFMT_LEFT, 52, 0);

    m_ebRemainPercent.SetReadOnly(1);
    DDX_Control(pDX, IDC_STATIC_TG_NEXTSTAGE_1, m_stNextStage1);
    DDX_Control(pDX, IDC_STATIC_TG_NEXTSTAGE_2, m_stNextStage2);
}


BEGIN_MESSAGE_MAP(CDlgTGNextStage, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_STAGE, OnBnClickedButtonStage)
    ON_BN_CLICKED(IDC_BUTTON_STAGE_DEL, OnBnClickedButtonStageDel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STAGE_N_PERCENT, OnLvnItemchangedListStageNPercent)
END_MESSAGE_MAP()


// CDlgTGNextStage 메시지 처리기입니다.

void CDlgTGNextStage::OnBnClickedButtonStage()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strStageNum;
    m_ebStage.GetWindowText(strStageNum);
    if( strStageNum.GetLength() <= 0 )
    {
        MessageBox(L"스테이지 번호를 입력해 주세요", L"알림", MB_OK);
        return;
    }

    CString strPercent;
    m_ebPercent.GetWindowText(strPercent);
    if( strPercent.GetLength() <= 0 )
    {
        if( IDNO == MessageBox(L"스테이지 확률을 입력해주세요. NO를 누르게 되면 100%로 설정됩니다.", L"알림", MB_YESNO) )
            strPercent.Format(L"%d", 100);
        else
            return;
    }

    if( _wtoi(strPercent)+GetCurTotalPercent() > 100 )
    {
        MessageBox(L"확률의 총합이 100%를 넘어갔습니다.", L"알림", MB_OK);
        return;
    }

    bool bEqual = false;
    for(int i=0;i<m_lcStageNPercent.GetItemCount();i++)
    {
        CString strLCStageNum = m_lcStageNPercent.GetItemText(i, 0);
        if( _wtoi(strLCStageNum) == _wtoi(strStageNum) )
        {
            MessageBox(L"중복된 스테이지 입니다. 다른 스테이지로 수정해주세요.", L"알림", MB_OK);
            return;
        }
    }

    if( _wtoi(strPercent) > 100 ) strPercent.Format(L"%d", 100);

    int index = m_lcStageNPercent.GetItemCount();
    m_lcStageNPercent.InsertItem(index, strStageNum);
    m_lcStageNPercent.SetItemText(index, 1, strPercent);

    RefreshRemainPercent();
}

void CDlgTGNextStage::OnBnClickedButtonStageDel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int nItem = -1;
    POSITION pos = m_lcStageNPercent.GetFirstSelectedItemPosition();
    if( pos )
        nItem = m_lcStageNPercent.GetNextSelectedItem(pos);

    if( -1 >= nItem || nItem >= m_lcStageNPercent.GetItemCount() )
        MessageBox(L"삭제할 항목을 다시 선택하여 주십시오.", L"알림", MB_OK);
    else
        m_lcStageNPercent.DeleteItem(nItem);

    RefreshRemainPercent();
}

void CDlgTGNextStage::Refresh(void)
{
    CString strLevel;
    strLevel.Format(L"%d", m_TGNextStage.m_iLevel);
    m_ebLevel.SetWindowText(strLevel);

    m_lcStageNPercent.DeleteAllItems();
    for(int i=0;i<(int)m_TGNextStage.m_vtStage.size();i++)
    {
        CString strNum;
        strNum.Format(L"%d", m_TGNextStage.m_vtStage[i]);
        m_lcStageNPercent.InsertItem(i, strNum );

        CString strPercent;
        strPercent.Format(L"%d", m_TGNextStage.m_vtPercent[i]);        
        m_lcStageNPercent.SetItemText(i, 1, strPercent);
    }

    RefreshRemainPercent();
}

void CDlgTGNextStage::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strText1, strText2;
    CString strLevel;
    m_ebLevel.GetWindowText(strLevel);
    m_stNextStage1.GetWindowText(strText1);
    m_stNextStage2.GetWindowText(strText2);

    m_strDesc = MiddleBraceSidebyString(strLevel) + L" " + strText1 + L" " + strText2;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_ebLevel.GetWindowText(strLevel);
    m_TGNextStage.m_iLevel = _wtoi(strLevel);
    m_TGNextStage.m_vtStage.clear();
    m_TGNextStage.m_vtPercent.clear();
    for(int i=0;i<m_lcStageNPercent.GetItemCount();i++)
    {
        CString strNum = m_lcStageNPercent.GetItemText(i, 0);;
        CString strPercent = m_lcStageNPercent.GetItemText(i, 1);
        m_TGNextStage.m_vtStage.push_back(_wtoi(strNum));
        m_TGNextStage.m_vtPercent.push_back(_wtoi(strPercent));
    }
    m_TGNextStage.m_strDesc = GetDesc();
}

BOOL CDlgTGNextStage::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    if( (m_lcStageNPercent.GetSafeHwnd() == GetFocus()->GetSafeHwnd())
        && (GetKeyState(VK_DELETE) & 0x80000000) )
    {
        OnBnClickedButtonStageDel();
        m_ebStage.SetFocus();
        return true;
    }

    if( (m_ebPercent.GetSafeHwnd() == GetFocus()->GetSafeHwnd())
        && (GetKeyState(VK_RETURN) & 0x80000000) )
    {
        OnBnClickedButtonStage();
        m_lcStageNPercent.SetFocus();
        return true;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTGNextStage::OnLvnItemchangedListStageNPercent(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int nItem = -1;
    POSITION pos = m_lcStageNPercent.GetFirstSelectedItemPosition();
    if( pos )
        nItem = m_lcStageNPercent.GetNextSelectedItem(pos);

    if( -1 >= nItem || nItem >= m_lcStageNPercent.GetItemCount() )
    {
        m_ebStage.SetWindowText(L"0");
        m_ebPercent.SetWindowText(L"100");
    }
    else
    {
        CString strStage = m_lcStageNPercent.GetItemText(nItem, 0);
        CString strPercent = m_lcStageNPercent.GetItemText(nItem, 1);
        m_ebStage.SetWindowText(strStage);
        m_ebPercent.SetWindowText(strPercent);
    }

    *pResult = 0;
}

int CDlgTGNextStage::GetCurTotalPercent(void)
{
    int iTotalPercent = 0;
    for(int i=0;i<m_lcStageNPercent.GetItemCount();i++)
    {
        CString strPercent = m_lcStageNPercent.GetItemText(i, 1);
        iTotalPercent += _wtoi(strPercent);
    }
    return iTotalPercent;
}

void CDlgTGNextStage::RefreshRemainPercent(void)
{
    CString strReaminPercent;
    strReaminPercent.Format(L"%d", 100-GetCurTotalPercent());
    m_ebRemainPercent.SetWindowText(strReaminPercent);
}

void CDlgTGNextStage::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGNextStage::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}
