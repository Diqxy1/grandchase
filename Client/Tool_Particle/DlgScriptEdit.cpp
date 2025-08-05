// DlgScriptEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool_Particle.h"
#include "DlgScriptEdit.h"
#include ".\dlgscriptedit.h"
#include "ParticleEditor.h"


// CDlgScriptEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgScriptEdit, CDialog)
CDlgScriptEdit::CDlgScriptEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScriptEdit::IDD, pParent)
	, m_strScript(_T(""))
{
}

CDlgScriptEdit::~CDlgScriptEdit()
{
}

void CDlgScriptEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strScript);
}


BEGIN_MESSAGE_MAP(CDlgScriptEdit, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgScriptEdit 메시지 처리기입니다.

void CDlgScriptEdit::OnBnClickedOk()
{
	UpdateData();
	if( !CheckScript() )
	{
		MessageBox("스크립트를 다시한번 확인해주세요" );
		return;
	}
	OnOK();
}

bool CDlgScriptEdit::CheckScript()
{
	CKTDGParticleSystem::CParticleEventSequence *newseq = g_pParticleEditor->OnCreateParticle( m_strScript, false );
	if( newseq != NULL )
	{
		SAFE_DELETE(newseq);
		return true;
	}
	return false;	
}
