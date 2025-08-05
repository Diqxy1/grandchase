// DlgTGTitleText.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGTitleText.h"
#include ".\dlgtgtitletext.h"


// CDlgTGTitleText 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGTitleText, CDlgTGBase)
CDlgTGTitleText::CDlgTGTitleText(CWnd* pParent /*=NULL*/)
{
}

CDlgTGTitleText::~CDlgTGTitleText()
{
}

void CDlgTGTitleText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TITLETEXT, m_ebTitleText);
	DDX_Control(pDX, IDC_STATIC_TITLETEXT_1, m_stTileText1);

	Refresh();    
	DDX_Control(pDX, IDC_STATIC_TITLETEXT_2, m_strTitleText2);
}


BEGIN_MESSAGE_MAP(CDlgTGTitleText, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_EN_CHANGE(IDC_EDIT_TITLETEXT, OnEnChangeEditTitletext)
END_MESSAGE_MAP()


// CDlgTGTitleText 메시지 처리기입니다.

void CDlgTGTitleText::Refresh(void)
{
    if( m_ebTitleText.GetSafeHwnd() )
	{
		CString strTitleID;
		strTitleID.Format(L"%d", m_TGTitleText.m_iStringID);
		m_ebTitleText.SetWindowText(strTitleID);
	}
}

void CDlgTGTitleText::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strTitleText;
    m_ebTitleText.GetWindowText(strTitleText);
	m_TGTitleText.m_iStringID = _wtoi(strTitleText);

    CString strText1, strText2;
    m_stTileText1.GetWindowText(strText1);
	m_strTitleText2.GetWindowText(strText2);

    m_strDesc = MiddleBraceSidebyString(strTitleText) + L" " + strText2 + L" " + strText1;

    //************************************************************************
    // 설명 생성
    //************************************************************************
    m_TGTitleText.m_strDesc = GetDesc();
}

void CDlgTGTitleText::OnBnClickedOk()
{

}

void CDlgTGTitleText::OnBnClickedCancel()
{

}
void CDlgTGTitleText::OnEnChangeEditTitletext()
{
    // TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
    // CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
    // CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
    // 이 알림을 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strTitleText;
    m_ebTitleText.GetWindowText(strTitleText);
    if( strTitleText.GetLength() >= MAX_TG_STR_LENGTH )
    {
        CString strMsg;
        strMsg.Format(L"입력되는 TitleText 수는 %d자를 넘을 수 없습니다.", MAX_TG_STR_LENGTH);
        MessageBox(strMsg, L"알림", MB_OK);
        m_ebTitleText.SetWindowText(strTitleText.Mid(0, MAX_TG_STR_LENGTH-1));
    }

}
