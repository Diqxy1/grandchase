// DlgTGClearOfStage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGClearOfStage.h"


// CDlgTGClearOfStage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGClearOfStage, CDlgTGBase)
CDlgTGClearOfStage::CDlgTGClearOfStage(CWnd* pParent /*=NULL*/)
{
}

CDlgTGClearOfStage::~CDlgTGClearOfStage()
{
}

void CDlgTGClearOfStage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_STAGEOFCLEAR_1, m_stStageClear);
}


BEGIN_MESSAGE_MAP(CDlgTGClearOfStage, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGClearOfStage 메시지 처리기입니다.

void CDlgTGClearOfStage::Refresh(void)
{

}

void CDlgTGClearOfStage::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strText1;
    m_stStageClear.GetWindowText(strText1);

    m_strDesc = strText1;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    //m_ebName.GetWindowText(m_TGOnOff.m_szTriggerName, sizeof(m_TGOnOff.m_szTriggerName));
    m_TGClearOfStage.m_strDesc = GetDesc();
}

void CDlgTGClearOfStage::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGClearOfStage::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}