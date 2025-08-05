// DlgChangeSequence.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgChangeSequence.h"
#include ".\dlgchangesequence.h"


// CDlgChangeSequence 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgChangeSequence, CDialog)
CDlgChangeSequence::CDlgChangeSequence(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangeSequence::IDD, pParent)
{
    m_iSequence = 0;
}

CDlgChangeSequence::~CDlgChangeSequence()
{
}

void CDlgChangeSequence::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_SEQUENCE, m_cbChangeSequence);

    for(int i=0;i<(int)m_vtChangeSequence.size();i++)
    {
        CString str;
        str.Format(L"%d", m_vtChangeSequence[i]);
        m_cbChangeSequence.InsertString(i, str);
    }

    if( m_cbChangeSequence.GetCount() > 0 )
        m_cbChangeSequence.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgChangeSequence, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgChangeSequence 메시지 처리기입니다.

void CDlgChangeSequence::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString str;
    if( m_cbChangeSequence.GetCount() > 0
        && m_cbChangeSequence.GetCount() > m_cbChangeSequence.GetCurSel() )
    {
        m_cbChangeSequence.GetLBText(m_cbChangeSequence.GetCurSel(), str);
        m_iSequence = _wtoi(str);
    }
    OnOK();
}

void CDlgChangeSequence::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CDlgChangeSequence::SetChangeSequenceList(std::vector<int> vtChangeSequence)
{
    m_vtChangeSequence.clear();
    m_vtChangeSequence = vtChangeSequence;
}