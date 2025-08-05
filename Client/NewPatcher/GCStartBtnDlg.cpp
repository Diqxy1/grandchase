// GCStartBtnDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCStartBtnDlg.h"
#include "GCGlobalValue.h"


// CGCStartBtnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGCStartBtnDlg, CDialog)

CGCStartBtnDlg::CGCStartBtnDlg( CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass )
	: CDialog(CGCStartBtnDlg::IDD, pParent)
	, m_pMassfile(imagemass)
{
	CreateButtonList();
}

CGCStartBtnDlg::~CGCStartBtnDlg()
{
}

void CGCStartBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGCStartBtnDlg, CDialog)
	//ON_BN_CLICKED(CBB_START_BUTTON_SERVER_SELECT, OnClickServerSelect)
	//ON_BN_CLICKED(CBB_START_BUTTON_DOWNLOAD, OnClickStartGame)
    ON_MESSAGE( WM_DISABLE_BTN, SetServerSelectBtn )
END_MESSAGE_MAP()

BOOL CGCStartBtnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowLong( m_hWnd, GWL_EXSTYLE, ::GetWindowLong( m_hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB( 255, 0, 255), 0, LWA_COLORKEY/* | LWA_ALPHA */);

	SetLinkButtonPosition();
	Invalidate();
	return TRUE;  // return TRUE unless you set the focus to a control

}


BOOL CGCStartBtnDlg::OnEraseBkgnd( CDC* pDC )
{
	return FALSE;
}

BOOL CGCStartBtnDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			return TRUE;
		else if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	else if ( pMsg->message == WM_SYSKEYDOWN ) {
		if ( pMsg->wParam == VK_F4 ) {
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

}

void CGCStartBtnDlg::SetLinkButtonPosition()
{
	if ( m_mapButton.empty() == true ) return;


	CRect rect;
	int iStartX = 0;
	int iStartY = 0;
	int iGap = 23;

	rect.SetRect(iStartX,iStartY,iStartX+98,iStartY+98); 
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_START_BUTTON_SERVER_SELECT);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->EnableWindow(FALSE);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->ShowWindow(SW_HIDE);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->SetNoPress(TRUE);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->Invalidate();

	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_START_BUTTON_DOWNLOAD);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->EnableWindow(FALSE);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->ShowWindow(SW_HIDE);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->SetNoPress(TRUE);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->Invalidate();

}

void CGCStartBtnDlg::CreateButtonList()
{
	if ( m_mapButton.empty() == false ) return;

	m_mapButton[CBB_START_BUTTON_SERVER_SELECT] = new CNBitmapButton();
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->LoadBitmaps(m_pMassfile.get(), "ServerSelect");

	m_mapButton[CBB_START_BUTTON_DOWNLOAD] = new CNBitmapButton();
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->LoadBitmaps(m_pMassfile.get(), "StartGame");

}

void CGCStartBtnDlg::OnClickServerSelect()
{
	if ( KGCGlobalValue::m_iSelectServerIndex == 0 || KGCGlobalValue::m_iSelectServerState == 4) 
		return;

	m_pParentWnd->EndModalLoop(0);
}

void CGCStartBtnDlg::OnClickStartGame()
{
	m_pParentWnd->EndModalLoop(0);
}

void CGCStartBtnDlg::ReleaseAll()
{
	for ( std::map<int, CNBitmapButton*>::iterator mit = m_mapButton.begin() ; mit != m_mapButton.end() ; ++mit ) {
		SAFE_DELETE( mit->second);
	}

	m_mapButton.clear();

}

void CGCStartBtnDlg::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGCStartBtnDlg::OnServerSelectBtn()
{
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->SetWindowPos(&wndTop,0,0,98,98, SWP_NOSIZE);

	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->EnableWindow(FALSE);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->ShowWindow(SW_HIDE);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->EnableWindow(TRUE);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->ShowWindow(SW_SHOW);
	
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->SetWindowPos(&wndTop,0,100,98,198, SWP_NOSIZE);


}

void CGCStartBtnDlg::OnStartGameBtn()
{
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->SetWindowPos(&wndTop,0,0,98,98, SWP_NOSIZE);

	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->EnableWindow(FALSE);
	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->ShowWindow(SW_HIDE);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->EnableWindow(FALSE);
	m_mapButton[CBB_START_BUTTON_DOWNLOAD]->ShowWindow(SW_SHOW);

	m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->SetWindowPos(&wndTop,0,100,98,198, SWP_NOSIZE);

}

void CGCStartBtnDlg::OnComplete( bool success, bool bMsgBox )
{
	if ( success ) {
		m_mapButton[CBB_START_BUTTON_DOWNLOAD]->EnableWindow(TRUE);
	}
	else {
		//if (bMsgBox==true)
		//{
		//	if(true)
		//	{
		//		MessageBox(m_pStrLoader->GetString( STR_ID_CAN_NOT_DOWNLOAD_PATCH_FILE ).c_str() );
		//	}
		//	else
		//	{
		//		MessageBox(m_pStrLoader->GetString( STR_ID_MAIN_EXE_VERIFY_FAIL ).c_str() );
		//	}
		//}		
		//OnInvalidate();
	}
}


BOOL CGCStartBtnDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( nCode == BN_CLICKED ) {
		if ( CBB_START_BUTTON_DOWNLOAD == nID ) {
			OnClickStartGame();
		}
		else if ( CBB_START_BUTTON_SERVER_SELECT == nID ) { 
			OnClickServerSelect();
		}
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CGCStartBtnDlg::SetDisbleServerSelectBtn( BOOL bDisable )
{
    m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->SetDisable(bDisable);
}

LRESULT CGCStartBtnDlg::SetServerSelectBtn( WPARAM wParam, LPARAM lParam )
{
    if ( KGCGlobalValue::m_iSelectServerState == 4 )
    {
        if ( FALSE == m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->GetDisable() )
        {
            SetDisbleServerSelectBtn(TRUE);
        }
    }
    else
    {
        if ( TRUE == m_mapButton[CBB_START_BUTTON_SERVER_SELECT]->GetDisable() )
        {
            SetDisbleServerSelectBtn(FALSE);
        }
    }
    return HTCAPTION;
}




// CGCStartBtnDlg 메시지 처리기입니다.
