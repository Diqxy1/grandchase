// KUserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "GCServerManager.h"
#include "GCServerManagerView.h"
#include "KUserInfoDlg.h"
#include "KReasonDlg.h"
#include ".\kuserinfodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KUserInfoDlg dialog


KUserInfoDlg::KUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KUserInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KUserInfoDlg)
	m_cstrStatus = _T("");
	//m_cstrZender = _T("");	
	m_cstrLogin = _T("");
    m_cstrLoginNickname = _T("");    
	//}}AFX_DATA_INIT
    m_iIdxInfo = 0;
    m_dwUserUIDInfo = 0;
    m_pChildView = static_cast<CGCServerManagerView*>( pParent );
}


void KUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KUserInfoDlg)
	DDX_Text(pDX, IDC_EDIT_STATUS, m_cstrStatus);
	//DDX_Text(pDX, IDC_EDIT_ZENDER, m_cstrZender);
  
	DDX_Text(pDX, IDC_EDIT_USER_LOGIN_ID, m_cstrLoginNickname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KUserInfoDlg, CDialog)
	//{{AFX_MSG_MAP(KUserInfoDlg)
	ON_BN_CLICKED(IDC_BUTTON_KICK, OnButtonKick)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_DB_INFO, OnButtonChangeDbInfo)
	//}}AFX_MSG_MAP
    ON_EN_CHANGE(IDC_EDIT_USER_LOGIN_ID, OnEnChangeEditUserLoginId)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KUserInfoDlg message handlers

BOOL KUserInfoDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    const KInviteUserInfo& usrInfo = m_pChildView->m_vecUsrInfo[m_iIdxInfo]; // KUserInfo->KInviteUserInfo

    //m_cstrLogin     = usrInfo.m_strLogin.c_str();
    m_cstrLogin.Format( _T("%d"), usrInfo.m_dwUserUID );
    m_cstrLoginNickname = usrInfo.m_strNickName.c_str();
    m_dwUserUIDInfo = usrInfo.m_dwUserUID;
    //m_cstrLoginNickname += L"(" + m_cstrLogin + L")";
    //m_cstrZender    = ( true == usrInfo.m_bMale ? L"male" : L"female" );

    // channel: 1, playing: 2, room: 3, square: 4

    /*switch( usrInfo.m_cState )
    {
    case 1:
        m_cstrStatus = L"Lobby";
        break;
    case 2:
        m_cstrStatus = L"Playing";
        break;
    case 3:
        m_cstrStatus.Format( _T("Waiting(방ID:%d)"), usrInfo.m_usRoomID );
        break;
    case 4:
        m_cstrStatus = L"Square";
        break;
    }*/ //기능이 너무 줄어든다..

    static std::wstring strTabGrade[] =
    {
        L"Wanderer Knight(bronze)", L"Wanderer Knight(silver)", L"Wanderer Knight(gold)",
        L"Mercenary Knight(bronze)", L"Mercenary Knight(silver)", L"Mercenary Knight(gold)",
        L"Knight of King(bronze)", L"Knight of King(silver)", L"Knight of King(gold)",
        L"Knight of God(bronze)", L"Knight of God(silver)", L"Knight of God(gold)",
    };
    
    if ( usrInfo.m_cGrade >= 0 && usrInfo.m_cGrade < sizeof( strTabGrade ) / sizeof( std::string ) )
    {
        m_cstrStatus = strTabGrade[usrInfo.m_cGrade-1].c_str();
    }

    UpdateData( FALSE );

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void KUserInfoDlg::OnButtonKick( void )
{
    if ( IDOK == ::AfxMessageBox( _T("Really kick out?"), MB_OKCANCEL ) )
    {
        m_pChildView->m_server.Send_KickUserReq( m_dwUserUIDInfo );
        m_pChildView->m_pMainFrm->m_logFile << L"[request] " << (const wchar_t*)m_cstrLogin
                                            << L" kick out user." << std::endl;
    }
}

void KUserInfoDlg::OnButtonChangeDbInfo( void )
{
    /*m_pChildView->m_server.Send_UserDBInfoReq( m_pChildView->m_strWideServerName.c_str(),
                                               (const wchar_t*)m_cstrLogin );
    m_pChildView->m_pMainFrm->m_logFile << L"[request] " << (const wchar_t*)m_cstrLogin
                                        << L" We request DataBase Information of user." << std::endl;*/

    m_pChildView->m_server.Send_UserDBInfoReq( m_pChildView->m_strWideServerName.c_str(), std::wstring( m_cstrLoginNickname ), true );
    m_pChildView->m_pMainFrm->m_logFile << L"[request] " << std::wstring( m_cstrLoginNickname )
                                        << L" We request DataBase Information of user." << std::endl;
}

void KUserInfoDlg::OnEnChangeEditUserLoginId()
{
    // TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
    // CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
    // CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
    // 이 알림을 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void KUserInfoDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
}
