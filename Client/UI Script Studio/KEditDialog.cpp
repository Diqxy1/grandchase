// KEditDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "KEditDialog.h"
//#include ".\keditdialog.h"

// KEditDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(KEditDialog, CDialog)
KEditDialog::KEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(KEditDialog::IDD, pParent)
	, m_strName(_T(""))
	, m_strValue(_T(""))
	, m_iBoxType(0)
{
}

KEditDialog::~KEditDialog()
{
}

void KEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strValue);
	DDX_Control(pDX, IDC_COMBO_BOXTYPE, m_ComboBoxBoxType);
	DDX_CBIndex( pDX, IDC_COMBO_BOXTYPE, m_iBoxType );
}


BEGIN_MESSAGE_MAP(KEditDialog, CDialog)
	
	ON_CBN_SELCHANGE(IDC_COMBO_BOXTYPE, &KEditDialog::OnCbnSelchangeComboBoxtype)
END_MESSAGE_MAP()

//------------------------------------------------------------------------
//	NAME : OnInitDialog()
//	DESC : 20091012 이동석 추가, 재정의 함수
//------------------------------------------------------------------------
BOOL KEditDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CString boxtype( "gc_sizingbox" );
	m_ComboBoxBoxType.AddString( "gc_sizingbox" ); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KEditDialog::OnCbnSelchangeComboBoxtype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	m_ComboBoxBoxType.GetLBText( m_iBoxType, m_strBoxType );

	//mapBoxTypePorcItr itr = m_mapBoxTypeProc.find( str );
	//if( m_mapBoxTypeProc.end() != itr )
	//{
	//	CUIScriptStudioDlg* kWnd;
	//	(itr->second)( str, kWnd );
	//}
}
