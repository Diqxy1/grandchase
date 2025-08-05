// KChangeDBInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KChangeDBInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KChangeDBInfoDlg dialog


KChangeDBInfoDlg::KChangeDBInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KChangeDBInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KChangeDBInfoDlg)
	//}}AFX_DATA_INIT
    m_kData.m_cAuthLevel = 1;
}


void KChangeDBInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(KChangeDBInfoDlg)
    DDX_Control(pDX, IDC_COMBO_AUTH_LEVEL, m_ctrlCbAuthLv);
    DDX_Text(pDX, IDC_EDIT_DB_GP, m_kData.m_iGP);
    DDV_MinMaxInt(pDX, m_kData.m_iGP, 0, 999999);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_LIST1, m_list);

    UpdateList( pDX->m_bEditLastControl );
}


BEGIN_MESSAGE_MAP(KChangeDBInfoDlg, CDialog)
	//{{AFX_MSG_MAP(KChangeDBInfoDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_KICK_USER, OnButtonKickUser)
	ON_BN_CLICKED(IDC_BUTTON_DROP_USER, OnButtonDropUser)
	ON_BN_CLICKED(IDC_BUTTON_EXP_RESET, OnButtonExpReset)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_EDIT_COMMITTED, OnEditCommitted) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KChangeDBInfoDlg message handlers

void KChangeDBInfoDlg::OnOk() 
{
    m_kData.m_cAuthLevel = m_ctrlCbAuthLv.GetCurSel() - 3;

    //if ( IDOK == ::AfxMessageBox( L"Really change?", MB_OKCANCEL ) )
    //{
        CDialog::OnOK();
    //}
}

BOOL KChangeDBInfoDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    m_list.SetEditable( TRUE );
    m_list.SetSortable( FALSE );
    m_list.SetGridLines( TRUE );
    m_list.SetLimitText( 10 );
    m_list.SetMoreEditStyle( ES_NUMBER );

    // 이름 설정.
    CString cstrName;
    cstrName.Format( L"%s", m_kData.m_strName.c_str() );
    SetDlgItemText( IDC_EDIT_UserID, cstrName );

    cstrName.Format( L"%s", m_kData.m_strNickName.c_str() );
    SetDlgItemText( IDC_EDIT_NickName, cstrName );

    cstrName.Format( L"(%d)", m_kData.m_cWarning );
    SetDlgItemText( IDC_STATIC_Warning, cstrName );

    // GP의 초기값은 자동으로 설정된다.

    m_ctrlCbAuthLv.SetCurSel( m_kData.m_cAuthLevel + 3 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void KChangeDBInfoDlg::OnButtonKickUser()
{
    EndDialog( IDC_BUTTON_KICK_USER );
}

void KChangeDBInfoDlg::OnButtonDropUser()
{
    EndDialog( IDC_BUTTON_DROP_USER );
}

void KChangeDBInfoDlg::OnButtonExpReset() 
{
    EndDialog( IDC_BUTTON_EXP_RESET );
}

afx_msg LRESULT KChangeDBInfoDlg::OnEditCommitted(WPARAM wParam, LPARAM lParam)
{
    CPoint cPosition( wParam, lParam );

    TRACE( "Edited Position : %d, %d \n", cPosition.x, cPosition.y );

    return 0;
}

void KChangeDBInfoDlg::UpdateList( BOOL bSaveAndValidate_ )
{    
    if( bSaveAndValidate_ ) // 컨트롤에서 값을 읽어들임.
    {
        int         nCharID = 0;
        std::wstring strBuff;
        std::map<char,KCharacterInfo>::iterator mit;

        for( int i = 0; i < m_list.GetItemCount(); i++ )
        {
            strBuff = m_list.GetItemText( i, COLUMN_ID );
            nCharID = ::_wtoi( strBuff.c_str() );

            mit = m_kData.m_mapCharacter.find( nCharID );

            ASSERT( mit != m_kData.m_mapCharacter.end() );

            std::wstring strTest0 = m_list.GetItemText( i, COLUMN_ID );
            std::wstring strTest1 = m_list.GetItemText( i, COLUMN_JOB );
            std::wstring strTest2 = m_list.GetItemText( i, COLUMN_CHAR_NAME );
            std::wstring strTest3 = m_list.GetItemText( i, COLUMN_PROMOTION );
            std::wstring strTest4 = m_list.GetItemText( i, COLUMN_EXP );
            std::wstring strTest5 = m_list.GetItemText( i, COLUMN_LEVEL );
            std::wstring strTest6 = m_list.GetItemText( i, COLUMN_WIN );
            std::wstring strTest7 = m_list.GetItemText( i, COLUMN_LOSE );


            strBuff = m_list.GetItemText( i, COLUMN_EXP );
            mit->second.m_biExp = ::_wtoi( strBuff.c_str() );
            //strBuff = m_list.GetItemText( i, COLUMN_BLESSING );
            //mit->second.m_dwBlessing = ::_wtoi( strBuff.c_str() );
        }

   }
    else // 컨트롤에 값을 표기함.
    {
        m_list.DeleteAllColumns();

        int nScrollSize = ::GetSystemMetrics( SM_CYVTHUMB );
        CRect rt;
        m_list.GetClientRect( &rt );
        m_list.InsertColumn( COLUMN_ID,         L"ID",           LVCFMT_CENTER, 30 );
        m_list.InsertColumn( COLUMN_JOB,        L"Job",          LVCFMT_CENTER, 70 );
        m_list.InsertColumn( COLUMN_CHAR_NAME,  L"Name",         LVCFMT_CENTER, rt.Width() - 430 /*- nScrollSize*/ );
        m_list.InsertColumn( COLUMN_PROMOTION,  L"Promotion",    LVCFMT_CENTER, 60 );
        m_list.InsertColumn( COLUMN_EXP,        L"Exp",          LVCFMT_CENTER, 80 );
        m_list.InsertColumn( COLUMN_LEVEL,      L"Level",        LVCFMT_CENTER, 60 );
        m_list.InsertColumn( COLUMN_WIN,        L"Win",          LVCFMT_CENTER, 60 );
        m_list.InsertColumn( COLUMN_LOSE,       L"Lose",         LVCFMT_CENTER, 60 );
        
        m_list.DeleteAllItems();

        wchar_t buff[16] = {0};
        wchar_t szCharacterJobText[][ 32 ] = {
            L"Elesis",
            L"Lire",
            L"Arme",
            L"Lass",
            L"Ryan",
            L"Ronan",
            L"Amy",
            L"Jin",
			L"Sieghart",
			L"Mari",
			L"Dio",
			L"Zero",
			L"Ley",
			L"Lupus",
			L"Rin",
			L"Asin",
			L"Lime",
            L"Edel",
            L"Beigas",
        };

        int i = 0;
        std::map<char, KCharacterInfo>::iterator mit;
        for( mit = m_kData.m_mapCharacter.begin(); mit != m_kData.m_mapCharacter.end(); mit++ )
        {
            m_list.InsertItem( i, ::_itow( mit->first, buff, 10 ) );  // TypeID
            m_list.SetItemText( i, COLUMN_JOB,         szCharacterJobText[mit->first] ); // Job
            m_list.SetItemText( i, COLUMN_CHAR_NAME,   mit->second.m_strCharName.c_str() ); // Name
            m_list.SetItemText( i, COLUMN_PROMOTION,   static_cast<int>(mit->second.m_cPromotion) ); // Promotion
            m_list.SetItemText( i, COLUMN_EXP,         mit->second.m_biExp ); // Exp
            m_list.SetItemText( i, COLUMN_LEVEL,       mit->second.m_dwLevel ); // Level
            m_list.SetItemText( i, COLUMN_WIN,         mit->second.m_iWin ); // Won
            m_list.SetItemText( i, COLUMN_LOSE,        mit->second.m_iLose ); // Lose
            m_list.SetItemTextColor( i, COLUMN_EXP, RGB( 255, 0, 0 ), TRUE );

            ++i;
        }
    }
}