// DlgTGCheckHP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGCheckHP.h"


// CDlgTGCheckHP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGCheckHP, CDlgTGBase)

CDlgTGCheckHP::CDlgTGCheckHP(CWnd* pParent /*=NULL*/)
{

}

CDlgTGCheckHP::~CDlgTGCheckHP()
{
}

void CDlgTGCheckHP::DoDataExchange(CDataExchange* pDX)
{
	CDlgTGBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SLOT, m_comboSlot);
	DDX_Control(pDX, IDC_COMBO_SLOT_TYPE, m_comboType);
	DDX_Control(pDX, IDC_EDIT_PERCENT, m_editPercent);

	m_comboSlot.ResetContent();
	m_comboSlot.InsertString( 0, L"0" );
	m_comboSlot.InsertString( 1, L"1" );
	m_comboSlot.InsertString( 2, L"2" );
	m_comboSlot.InsertString( 3, L"3" );
	m_comboSlot.SetCurSel( 0 );

	m_comboType.ResetContent();
	m_comboType.InsertString( 0, L"Player" );
	m_comboType.InsertString( 1, L"Monstro" );
	m_comboType.SetCurSel( 0 );

	m_editPercent.Clear();

	Refresh();

	DDX_Control(pDX, IDC_STATIC_CHECKHP1, m_stCheckHP1);
	DDX_Control(pDX, IDC_STATIC_CHECKHP2, m_stCheckHP2);
	DDX_Control(pDX, IDC_STATIC_CHECKHP3, m_stCheckHP3);
}


BEGIN_MESSAGE_MAP(CDlgTGCheckHP, CDlgTGBase)
	ON_BN_CLICKED(IDOK, &CDlgTGCheckHP::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgTGCheckHP::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SLOT_TYPE, &CDlgTGCheckHP::OnCbnSelchangeComboSlotType)
END_MESSAGE_MAP()


// CDlgTGCheckHP 메시지 처리기입니다.

void CDlgTGCheckHP::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CDlgTGCheckHP::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnCancel();
}

void CDlgTGCheckHP::InitValue( void )
{

}

void CDlgTGCheckHP::OnCbnSelchangeComboSlotType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_comboType.GetCurSel() == 0 )
	{
		m_comboSlot.ResetContent();
		m_comboSlot.InsertString( 0, L"0" );
		m_comboSlot.InsertString( 1, L"1" );
		m_comboSlot.InsertString( 2, L"2" );
		m_comboSlot.InsertString( 3, L"3" );
	}
	else
	{
		m_comboSlot.ResetContent();
		m_comboSlot.InsertString( 0, L"0" );
		m_comboSlot.InsertString( 1, L"1" );
		m_comboSlot.InsertString( 2, L"2" );
		m_comboSlot.InsertString( 3, L"3" );
		m_comboSlot.InsertString( 4, L"4" );
		m_comboSlot.InsertString( 5, L"5" );
		m_comboSlot.InsertString( 6, L"6" );
		m_comboSlot.InsertString( 7, L"7" );
		m_comboSlot.InsertString( 8, L"8" );
		m_comboSlot.InsertString( 9, L"9" );
		m_comboSlot.InsertString( 10, L"10" );
		m_comboSlot.InsertString( 11, L"11" );
		m_comboSlot.InsertString( 12, L"12" );
		m_comboSlot.InsertString( 13, L"13" );
		m_comboSlot.InsertString( 14, L"14" );
	}
	m_comboSlot.SetCurSel( 0 );
}

void CDlgTGCheckHP::Refresh( void )
{
	if( m_comboSlot.GetCount() > m_CheckHP.m_iIndex ) m_comboSlot.SetCurSel(m_CheckHP.m_iIndex);
	if( m_comboType.GetCount() > m_CheckHP.m_iType ) m_comboType.SetCurSel(m_CheckHP.m_iType);

	std::wstringstream str;
	str<<m_CheckHP.m_fPercent * 100.0f;

	m_editPercent.SetWindowText( str.str().c_str() );
}

void CDlgTGCheckHP::CalcResult( void )
{
	//************************************************************************
	// 설명 생성
	//************************************************************************
	{
		CString strSlot = L"X";
		CString strType = L"X";
		CString strPercent = L"X";

		if( m_comboSlot.GetCurSel() >= 0 )
			m_comboSlot.GetLBText(m_comboSlot.GetCurSel(), strSlot);

		if( m_comboType.GetCurSel() >= 0 )
			m_comboType.GetLBText(m_comboType.GetCurSel(), strType);

		m_editPercent.GetWindowText( strPercent );

		CString strText1, strText2, strText3;
		m_stCheckHP1.GetWindowText(strText1);
		m_stCheckHP2.GetWindowText(strText2);
		m_stCheckHP3.GetWindowText(strText3);
		m_strDesc = MiddleBraceSidebyString(strSlot) + L" " + strText1 + L" " + MiddleBraceSidebyString(strType) + L" " + strText2 + L" " + MiddleBraceSidebyString( strPercent ) + strText3;
	}

	//************************************************************************
	// 값 저장
	//************************************************************************
	{
		m_CheckHP.m_iIndex = m_comboSlot.GetCurSel();
		m_CheckHP.m_iType = m_comboType.GetCurSel();

		CString strPercent;
		m_editPercent.GetWindowText( strPercent );
		m_CheckHP.m_fPercent = (float)( _wtof( strPercent.GetBuffer() ) / 100.0 );

		m_CheckHP.m_strDesc = GetDesc();
	}
}