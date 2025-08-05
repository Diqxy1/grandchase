// KReasonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KReasonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KReasonDlg dialog


KReasonDlg::KReasonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KReasonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KReasonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_iSelect = 0;
}


void KReasonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KReasonDlg)
	DDX_Control(pDX, IDC_RADIO_IMPERSONATION, m_radioImpersonation);
	DDX_Control(pDX, IDC_RADIO_PAPER, m_radioPaper);
	DDX_Control(pDX, IDC_RADIO_CURSE, m_radioCurse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KReasonDlg, CDialog)
	//{{AFX_MSG_MAP(KReasonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KReasonDlg message handlers

void KReasonDlg::OnOK( void )
{
    UpdateData( TRUE );

    if ( 1 == m_radioCurse.GetCheck() ) m_iSelect = 0;
    else if ( 1 == m_radioPaper.GetCheck() ) m_iSelect = 1;
    else if ( 1 == m_radioImpersonation.GetCheck() ) m_iSelect = 2;

    CDialog::OnOK();
}

BOOL KReasonDlg::OnInitDialog( void )
{
    CDialog::OnInitDialog();

    m_radioCurse.SetCheck( 1 );

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
