// DlgTGReward.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGReward.h"


// CDlgTGReward 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGReward, CDlgTGBase)
CDlgTGReward::CDlgTGReward(CWnd* pParent /*=NULL*/)
{
}

CDlgTGReward::~CDlgTGReward()
{
}

void CDlgTGReward::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_EXP, m_ebEXP);
    DDX_Control(pDX, IDC_EDIT_GP, m_ebGP);
    DDX_Control(pDX, IDC_STATIC_REWARD_1, m_stReward1);
    DDX_Control(pDX, IDC_STATIC_REWARD_2, m_stReward2);
    DDX_Control(pDX, IDC_STATIC_REWARD_3, m_stReward3);
    DDX_Control(pDX, IDC_STATIC_REWARD_4, m_stReward4);
}


BEGIN_MESSAGE_MAP(CDlgTGReward, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGReward 메시지 처리기입니다.
void CDlgTGReward::Refresh(void)
{
    CString strEXP;
    strEXP.Format(L"%d", m_TGReward.m_iEXP);
    m_ebEXP.SetWindowText(strEXP);

    CString strGP;
    strGP.Format(L"%d", m_TGReward.m_iGP);
    m_ebGP.SetWindowText(strGP);
}


void CDlgTGReward::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strEXP;
    m_ebEXP.GetWindowText(strEXP);
    CString strGP;
    m_ebGP.GetWindowText(strGP);

    CString strText1, strText2, strText3, strText4;
    m_stReward1.GetWindowText(strText1);
    m_stReward2.GetWindowText(strText2);
    m_stReward3.GetWindowText(strText3);
    m_stReward4.GetWindowText(strText4);

    m_strDesc = strText1 + L" " + MiddleBraceSidebyString(strEXP) + L" " + strText2 + L" " + MiddleBraceSidebyString(strGP) 
                + L" " + strText3 + L" " + strText4;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_ebEXP.GetWindowText(strEXP);
    m_ebGP.GetWindowText(strGP);
    m_TGReward.m_iEXP = _wtoi(strEXP);
    m_TGReward.m_iGP = _wtoi(strGP);
    m_TGReward.m_strDesc = GetDesc();
}

void CDlgTGReward::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGReward::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}
