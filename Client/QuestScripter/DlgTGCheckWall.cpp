// DlgTGCheckWall.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTGCheckWall.h"


// CDlgTGCheckWall 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTGCheckWall, CDlgTGBase)

CDlgTGCheckWall::CDlgTGCheckWall(CWnd* pParent /*=NULL*/)
{

}

CDlgTGCheckWall::~CDlgTGCheckWall()
{
}

void CDlgTGCheckWall::DoDataExchange(CDataExchange* pDX)
{
	CDlgTGBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHECK_WALL_SLOT, m_ebWallSlot);
	DDX_Control(pDX, IDC_COMBO_CHECK_WALL_LIVE, m_cbIsLive);

    CString strTemp;
    strTemp.Format(L"%d", m_TGCheckWall.m_iSlot);
    m_ebWallSlot.SetWindowText( strTemp );

	m_cbIsLive.ResetContent();
	m_cbIsLive.InsertString( 0, L"Sim" );
	m_cbIsLive.InsertString( 1, L"Não" );
    if( m_TGCheckWall.m_bLive )
	    m_cbIsLive.SetCurSel( 0 );
    else
        m_cbIsLive.SetCurSel( 1 );

	Refresh();
}


BEGIN_MESSAGE_MAP(CDlgTGCheckWall, CDlgTGBase)
	ON_BN_CLICKED(IDOK, &CDlgTGCheckWall::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgTGCheckWall::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SLOT_TYPE, &CDlgTGCheckWall::OnCbnSelchangeComboSlotType)
END_MESSAGE_MAP()


// CDlgTGCheckWall 메시지 처리기입니다.

void CDlgTGCheckWall::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CDlgTGCheckWall::OnBnClickedCancel()
{
}

void CDlgTGCheckWall::InitValue( void )
{

}

void CDlgTGCheckWall::OnCbnSelchangeComboSlotType()
{
}

void CDlgTGCheckWall::Refresh( void )
{
    CString strTemp;
    strTemp.Format(L"%d", m_TGCheckWall.m_iSlot);
    m_ebWallSlot.SetWindowText( strTemp );

	if( 0 < m_cbIsLive.GetCount() )
    {
        if( m_TGCheckWall.m_bLive )
            m_cbIsLive.SetCurSel( 0 );
        else
            m_cbIsLive.SetCurSel( 1 );
    }
}

void CDlgTGCheckWall::CalcResult( void )
{
	//************************************************************************
	// 설명 생성
	//************************************************************************
	{
        CString strSlot, strLive;
        m_ebWallSlot.GetWindowText(strSlot);
        int iIsDestroyIndex = m_cbIsLive.GetCurSel();
        if( 0 < iIsDestroyIndex )
            m_strDesc =  L"[" + strSlot + L"] Não há paredes nesse slot";
        else
            m_strDesc =  L"[" + strSlot + L"] Há paredes nesse slot";
	}

	//************************************************************************
	// 값 저장
	//************************************************************************
	{
        CString strTemp;
        m_ebWallSlot.GetWindowText(strTemp);
        m_TGCheckWall.m_iSlot = (int)_wtof(strTemp);
        m_TGCheckWall.m_bLive = (0 == m_cbIsLive.GetCurSel());
		m_TGCheckWall.m_strDesc = GetDesc();
	}
}