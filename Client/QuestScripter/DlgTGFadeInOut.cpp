// DlgTGFadeInOut.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGFadeInOut.h"
#include "FunctionToScriptFile.h"


// CDlgTGFadeInOut 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGFadeInOut, CDlgTGBase)
CDlgTGFadeInOut::CDlgTGFadeInOut(CWnd* pParent /*=NULL*/)
{
}

CDlgTGFadeInOut::~CDlgTGFadeInOut()
{
}

void CDlgTGFadeInOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TG_ONOFF, m_cbFadeInOut);
	DDX_Control(pDX, IDC_EDIT1, editRed);
	DDX_Control(pDX, IDC_EDIT2, editGreen);
	DDX_Control(pDX, IDC_EDIT3, editBlue);
	DDX_Control(pDX, IDC_COMBO1, comboSpeed);

	editRed.Clear();
	editGreen.Clear();
	editBlue.Clear();

	comboSpeed.ResetContent();
	comboSpeed.InsertString( 0, L"lento" );
	comboSpeed.InsertString( 1, L"normal" );
	comboSpeed.InsertString( 2, L"rapido" );
	if( comboSpeed.GetCount() > 0 ) comboSpeed.SetCurSel(2);
	

	m_cbFadeInOut.ResetContent();
	m_cbFadeInOut.InsertString(0, L"Fade out");
	m_cbFadeInOut.InsertString(1, L"Fade in");
	if( m_cbFadeInOut.GetCount() > 0 ) m_cbFadeInOut.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgTGFadeInOut, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTGFadeInOut 메시지 처리기입니다.

void CDlgTGFadeInOut::Refresh(void)
{
	switch( m_TGFadeInOut.m_iSpeed )
	{
	case 9:
		comboSpeed.SetCurSel( 2 );
		break;
	case 6:
		comboSpeed.SetCurSel( 1 );
		break;
	case 3:
		comboSpeed.SetCurSel( 0 );
		break;
	default:
		comboSpeed.SetCurSel( 2 );
		break;
	}

	TCHAR strColorKey[MAX_PATH];
	editRed.SetWindowText( _itow( m_TGFadeInOut.m_Red, strColorKey, 10 ) );
	editGreen.SetWindowText( _itow( m_TGFadeInOut.m_Green, strColorKey, 10 ) );
	editBlue.SetWindowText( _itow( m_TGFadeInOut.m_Blue, strColorKey, 10 ) );

    if( m_cbFadeInOut.GetCount() > (int)m_TGFadeInOut.m_bOnOff ) m_cbFadeInOut.SetCurSel((int)m_TGFadeInOut.m_bOnOff);
}

void CDlgTGFadeInOut::CalcResult(void)
{
    //************************************************************************
    // 값 저장
    //************************************************************************
    CString strSpeed;
	if( comboSpeed.GetCurSel() >= 0 && comboSpeed.GetCount() > comboSpeed.GetCurSel() )
		comboSpeed.GetLBText(comboSpeed.GetCurSel(), strSpeed);

    CString strOnOff;
    if( m_cbFadeInOut.GetCurSel() >= 0 && m_cbFadeInOut.GetCount() > m_cbFadeInOut.GetCurSel() )
        m_cbFadeInOut.GetLBText(m_cbFadeInOut.GetCurSel(), strOnOff);

    //************************************************************************
    // 설명 생성
	//************************************************************************
	m_strDesc = MiddleBraceSidebyString(strSpeed) + L" " + MiddleBraceSidebyString(strOnOff);

    m_TGFadeInOut.m_iSpeed				= ( comboSpeed.GetCurSel() + 1 ) * 3;

	CString strColor;

	editRed.GetWindowText( strColor );
	m_TGFadeInOut.m_Red					= _wtoi( strColor.GetBuffer() );
	editGreen.GetWindowText( strColor );
	m_TGFadeInOut.m_Green					= _wtoi( strColor.GetBuffer() );
	editBlue.GetWindowText( strColor );
	m_TGFadeInOut.m_Blue					= _wtoi( strColor.GetBuffer() );

    m_TGFadeInOut.m_bOnOff               = (bool)m_cbFadeInOut.GetCurSel();
    m_TGFadeInOut.m_strDesc = GetDesc();
}

void CDlgTGFadeInOut::OnBnClickedOk()
{

}

void CDlgTGFadeInOut::OnBnClickedCancel()
{

}
