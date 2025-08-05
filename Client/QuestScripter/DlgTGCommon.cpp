// DlgTGCommon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGCommon.h"
#include ".\dlgtgcommon.h"


// CDlgTGCommon 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGCommon, CDlgTGBase)
CDlgTGCommon::CDlgTGCommon(CWnd* pParent /*=NULL*/)
{
    m_bCheck = false;
}

CDlgTGCommon::~CDlgTGCommon()
{
}

void CDlgTGCommon::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK_REPEAT, m_btRepeat);
}


BEGIN_MESSAGE_MAP(CDlgTGCommon, CDialog)
    ON_BN_CLICKED(IDC_CHECK_REPEAT, OnBnClickedCheckRepeat)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGCommon 메시지 처리기입니다.

void CDlgTGCommon::SetValue(CBaseTGFunction *pValue)
{
    m_bCheck = pValue->m_bRepeat;
    if( m_btRepeat.GetSafeHwnd() )
        m_btRepeat.SetCheck(m_bCheck);
}

void CDlgTGCommon::GetValue(CBaseTGFunction *pValue)
{
    if( m_btRepeat.GetSafeHwnd() )
        m_bCheck = m_btRepeat.GetCheck();

    pValue->m_bRepeat = m_bCheck;
}

void CDlgTGCommon::OnBnClickedCheckRepeat()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if( m_btRepeat.GetSafeHwnd() )
        m_bCheck = m_btRepeat.GetCheck();
}

void CDlgTGCommon::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();
}

void CDlgTGCommon::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnCancel();
}
