// DlgFindParticle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool_Particle.h"
#include "DlgFindParticle.h"
#include ".\dlgfindparticle.h"


// CDlgFindParticle 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgFindParticle, CDialog)
CDlgFindParticle::CDlgFindParticle(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindParticle::IDD, pParent)
	, m_strFind(_T(""))
{
}

CDlgFindParticle::~CDlgFindParticle()
{
}

void CDlgFindParticle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_strFind);
}


BEGIN_MESSAGE_MAP(CDlgFindParticle, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()


// CDlgFindParticle 메시지 처리기입니다.

void CDlgFindParticle::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str = m_strFind;
}
