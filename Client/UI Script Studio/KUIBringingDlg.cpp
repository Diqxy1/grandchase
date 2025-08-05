// KUIBringingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "KUIBringingDlg.h"
#include "UI Script StudioDlg.h"


// KUIBringingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KUIBringingDlg, CDialog)

KUIBringingDlg::KUIBringingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KUIBringingDlg::IDD, pParent)
{

}

KUIBringingDlg::~KUIBringingDlg()
{
}

void KUIBringingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPARE_LEFT_TREEVIEW, m_CtrlLeftTreeView);
	DDX_Control(pDX, IDC_COMPARE_RIGHT_TREEVIEW, m_CtrlRightTreeView);
}


BEGIN_MESSAGE_MAP(KUIBringingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OPEN, &KUIBringingDlg::OnBnClickedButtonFileOpen)
	ON_NOTIFY(NM_RCLICK, IDC_COMPARE_LEFT_TREEVIEW, &KUIBringingDlg::OnNMRclickCompareLeftTreeview)
	ON_NOTIFY(NM_RCLICK, IDC_COMPARE_RIGHT_TREEVIEW, &KUIBringingDlg::OnNMRclickCompareRightTreeview)
	ON_COMMAND(ID_TREE_COPY, &KUIBringingDlg::OnTreeCopy)
	ON_COMMAND(ID_TREE_PASTE, &KUIBringingDlg::OnTreePaste)
	ON_COMMAND(ID_RIGHT_TREE_COPY, &KUIBringingDlg::OnRightTreeCopy)
	ON_COMMAND(ID_TREE_DELETE, &KUIBringingDlg::OnTreeDelete)
	ON_COMMAND(ID_TREE_INSERT, &KUIBringingDlg::OnTreeInsert)
END_MESSAGE_MAP()


// KUIBringingDlg 메시지 처리기입니다.

BOOL KUIBringingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 단축키~~
	m_hAccel = LoadAccelerators( AfxGetInstanceHandle() , MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

	// 초기화 & 생성
	m_pRightWndMgr = NULL;
	m_pLeftWndMgr = new KWndManager;
	m_pLeftWndMgr->Init();

	m_pClipBoard = NULL;

	// left static 상자에 파일이름 띄우기
	GetDlgItem( IDC_STATIC_LEFT_FILE_NAME )->SetWindowText( m_strLeftFilePath );

	// 왼쪽 트리 정보만 가져(복사)온다.
	LoadWndData( m_pOrgWndMgr->m_pWndRoot, m_pLeftWndMgr->m_pWndRoot );

	// make left tree
	InsertChild( m_pLeftWndMgr->m_pWndRoot, NULL, &m_CtrlLeftTreeView );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//------------------------------------------------------------------------
// 원본 파일에서 WndRoot 부분을 복사한다.
//------------------------------------------------------------------------
void KUIBringingDlg::LoadWndData(KD3DWndClass* pSrcWnd, KD3DWndClass* pCopyWnd)
{
	if(pSrcWnd)
	{
		pCopyWnd->DeepCopy( pSrcWnd );
		pCopyWnd->SetParent( NULL );
	}
}

//------------------------------------------------------------------------
// 트리에 노드 넣기
//------------------------------------------------------------------------
void KUIBringingDlg::InsertChild(KD3DWndClass* pkWnd,HTREEITEM hParent, CTreeCtrl *pCtrlTreeView, HTREEITEM hInsertAfter) // hInsertAfter = TVI_LAST
{
	hParent = pCtrlTreeView->InsertItem(pkWnd->GetWndName().c_str(), hParent, hInsertAfter);
	pCtrlTreeView->SetItemData(hParent,(DWORD)pkWnd);

	for( UINT i = 0 ; i < pkWnd->GetChildContainer().size() ; ++i )
	{
		InsertChild( pkWnd->GetChild(i), hParent, pCtrlTreeView );
	}
}

//------------------------------------------------------------------------
// Open 버튼 눌렀을 때 파일 불러오기
//------------------------------------------------------------------------
void KUIBringingDlg::OnBnClickedButtonFileOpen()
{
	CFileDialog Dlg(TRUE);

	if(IDOK == Dlg.DoModal()){
		m_strRightFilePath = Dlg.GetPathName();	// 경로와 파일이름을 받아옴~
		m_strRightFilePath.Replace('\\', '/');	// 기호 변경

		// 우측 static 창에 경로 표시
		GetDlgItem(IDC_STATIC_RIGHT_FILE_NAME)->SetWindowText(m_strRightFilePath);

		// WndMgr 객체생성 & 초기화
		SAFE_DELETE(m_pRightWndMgr);			// 기존 객체는 해제
		m_CtrlRightTreeView.DeleteAllItems();	// 트리 아이템들도 삭제
		m_pRightWndMgr = new KWndManager();
		m_pRightWndMgr->Init();

		// 파일 읽기

        // UI sign에 정보에서 텍스쳐 정보 가져온다.
        CUIScriptStudioDlg* m_pDlg = (CUIScriptStudioDlg*)(AfxGetApp()->GetMainWnd());
        CString FilePath = m_pDlg->m_LoadFileInfoWindow.GetScriptFromIndex(0).c_str();

        FilePath.Replace('\\', '/');

        CString strTempFilePath = FilePath.Right(4);

        if( FilePath.Right(4).CompareNoCase(_T(".stg")) )
        {
            MessageBox( _T("Ui Sign file Error"), _T(""), MB_OK );
        }
        else
        {
            m_pRightWndMgr->LoadLua( ToCharStr( FilePath.GetBuffer() ) );
        }
        
		m_pRightWndMgr->LoadLua( ToCharStr( m_strRightFilePath.GetString() ) );
		m_pRightWndMgr->DataMapping();

		// make right tree
		InsertChild( m_pRightWndMgr->m_pWndRoot, NULL, &m_CtrlRightTreeView);
	}
	else
		return;
}

//------------------------------------------------------------------------
// 다이얼로그 창이 닫히면 불러왔던 녀석은 삭제~!!
//------------------------------------------------------------------------
void KUIBringingDlg::OnOK()
{
	Release();

	CDialog::OnOK(); 
}

void KUIBringingDlg::OnCancel()
{	
	Release();

	CDialog::OnCancel();
}

//------------------------------------------------------------------------
// 다시 파일을 오픈 할 때 메모리 누수가 되지 않도록 기존 객체 삭제
//------------------------------------------------------------------------
void KUIBringingDlg::Release()
{
	// 트리 아이템 삭제~
	m_CtrlLeftTreeView.DeleteAllItems();
	m_CtrlRightTreeView.DeleteAllItems();

	//
	SAFE_DELETE(m_pRightWndMgr);
}

//------------------------------------------------------------------------
// 왼쪽트리에서 우측 버튼을 누르면 팝업창을 띄움
//------------------------------------------------------------------------
void KUIBringingDlg::OnNMRclickCompareLeftTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HMENU hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
	hMenu = GetSubMenu(hMenu,3); // IDR_MENU1 4번째 항목

	CPoint pt , pt2;
	HTREEITEM hItem;
	GetCursorPos(&pt);
	m_CtrlLeftTreeView.ScreenToClient(&pt);
	hItem = m_CtrlLeftTreeView.HitTest(pt);
	m_CtrlLeftTreeView.Select( hItem , TVGN_CARET);

	GetCursorPos(&pt2);
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,pt2.x,pt2.y,0,this->m_hWnd,NULL);

	*pResult = 0;
}

//------------------------------------------------------------------------
// 오른쪽 트리는 복사하기 기능만 지원
//------------------------------------------------------------------------
void KUIBringingDlg::OnNMRclickCompareRightTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HMENU hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
	hMenu = GetSubMenu(hMenu,4); // IDR_MENU1 5번째 항목

	CPoint pt , pt2;
	HTREEITEM hItem;
	GetCursorPos(&pt);
	m_CtrlRightTreeView.ScreenToClient(&pt);
	hItem = m_CtrlRightTreeView.HitTest(pt);
	m_CtrlRightTreeView.Select( hItem , TVGN_CARET);

	GetCursorPos(&pt2);
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,pt2.x,pt2.y,0,this->m_hWnd,NULL);

	*pResult = 0;
}

//------------------------------------------------------------------------
// 팝업메뉴 복사기능
// 왼쪽
//------------------------------------------------------------------------
void KUIBringingDlg::OnTreeCopy()
{
	HTREEITEM hTreeItem;

	hTreeItem = m_CtrlLeftTreeView.GetSelectedItem();

	// 핸들이 없다면 노드를 선택한게 아니므로 리턴!!
	if( !hTreeItem )
		return;

	// 선택된 노드를 클립보드에 저장한다.
	// 공용 클립보드에 포인터를 저장한다.
	m_pLeftWndMgr->CopyWindow( (KD3DWndClass*) m_CtrlLeftTreeView.GetItemData( m_CtrlLeftTreeView.GetSelectedItem() ) ) ;
	m_pClipBoard = m_pLeftWndMgr->GetClipboardPointer();
}

//------------------------------------------------------------------------
// 오른쪽 트리 Copy
//------------------------------------------------------------------------
void KUIBringingDlg::OnRightTreeCopy()
{
	HTREEITEM hTreeItem;

	hTreeItem = m_CtrlRightTreeView.GetSelectedItem();

	// 핸들이 없다면 노드를 선택한게 아니므로 리턴!!
	if( !hTreeItem )
		return;

	// 클립보드는 공통적으로 m_pClipBoard 에 포인터를 저장함
	m_pRightWndMgr->CopyWindow( (KD3DWndClass*) m_CtrlRightTreeView.GetItemData( m_CtrlRightTreeView.GetSelectedItem() ) ) ;
	m_pClipBoard = m_pRightWndMgr->GetClipboardPointer();
}

//------------------------------------------------------------------------
// 팝업메뉴 붙여넣기 기능 (왼쪽 트리에만)
//------------------------------------------------------------------------
void KUIBringingDlg::OnTreePaste()
{
	// 공용클립보드가 참조하고 있는 것으로 붙여 넣기 해야 하므로
	// 왼쪽 트리의 클립보드의 내용을 잠시 pOldClipboard에 보관
	KD3DWndClass* pOldClipboard;

	HTREEITEM hTreeItem = m_CtrlLeftTreeView.GetSelectedItem();

	// 선택되지 않은 상태거나, 복사를 수행하지 않았다면 종료
	if( !hTreeItem || !m_pClipBoard)
		return;

	KD3DWndClass* pParent = (KD3DWndClass*)m_CtrlLeftTreeView.GetItemData(hTreeItem);
	KD3DWndClass* pClipWindow = 0;

	// 이전 포인터 저장
	pOldClipboard = m_pLeftWndMgr->GetClipboardPointer();

	// 공용 포인터로 지정
	m_pLeftWndMgr->SetClipboardPointer(m_pClipBoard);

	// 붙여넣기
	if( m_pLeftWndMgr->GetClipboardPointer() )
	{
		pClipWindow = new KD3DWndClass;
		pClipWindow->DeepCopy( m_pLeftWndMgr->GetClipboardPointer() );
		pClipWindow->SetParent( pParent );

		InsertChild( pClipWindow, hTreeItem, &m_CtrlLeftTreeView );
		m_pLeftWndMgr->InsertWindow( pClipWindow, pParent );
	}
	// 이전 포인터로 복구
	m_pLeftWndMgr->SetClipboardPointer(pOldClipboard);
}

//------------------------------------------------------------------------
// 삭제
//------------------------------------------------------------------------
void KUIBringingDlg::OnTreeDelete()
{
	if( MessageBox( _T("해당 항목을 삭제하면 복구할 수 없습니다.  \n\n\t정말 삭제하시겠습니까?"), _T("알림"),MB_OKCANCEL) == IDOK )
	{
		if( MessageBox( _T("\t    진짜 ?"), _T("알림"),MB_OKCANCEL) == IDOK )
		{
			HTREEITEM hTreeItem = m_CtrlLeftTreeView.GetSelectedItem();

			if( !hTreeItem )
				return;

			KD3DWndClass* pkWnd = (KD3DWndClass*)m_CtrlLeftTreeView.GetItemData(hTreeItem);
			if( !pkWnd->GetParent() )
			{
				::MessageBoxA( NULL, "네 그렇습니다.", "최상위 윈도우라서 삭제 못해요", MB_OK );
				return;
			}
			m_pLeftWndMgr->DeleteWindow( pkWnd );

			m_CtrlLeftTreeView.DeleteItem(hTreeItem);
		}
	}
	else {	return;   }
}

//------------------------------------------------------------------------
// Ctrl+C, Ctrl+V, Ctrl+X 등 지원
//------------------------------------------------------------------------
BOOL KUIBringingDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( m_hAccel )
	{
		if( ::TranslateAccelerator( m_hWnd, m_hAccel, pMsg ) ) 
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------
// 중간에 삽입
// 선택한 노드 위치에 끼워넣기 한다.
//------------------------------------------------------------------------
void KUIBringingDlg::OnTreeInsert()
{
	// 복사하지 않은 상태에서 끼워넣기를 할 경우 종료
	if(!m_pClipBoard)
		return;

	// 1. 선택 된 노드 핸들 & 데이터 얻기
	HTREEITEM hTreeItem = m_CtrlLeftTreeView.GetSelectedItem();

	KD3DWndClass* pkWnd = (KD3DWndClass*)m_CtrlLeftTreeView.GetItemData(hTreeItem);
	KD3DWndClass* pkParent = pkWnd->GetParent();

	if(!pkParent){
		AfxMessageBox(_T("루트 노드에는 끼워넣기 할 수 없습니다."));
		return;
	}

	// 2. 선택 된 노드가 몇 번째(i번째) 자식인지 판별하기
	int i;
	std::vector<KD3DWndClass*> pVecChild = pkParent->GetChildContainer();

	for( i = 0; i < pVecChild.size(); ++i ){
		if( pVecChild[i] == pkWnd )
			break;
	}

	// 3. 윈도우 메니져에 해당 번째 자식으로 등록하기
	// 임시노드에 복사한 내용으로 윈도우클래스를 구성함
	KD3DWndClass* pkTemp = new KD3DWndClass;
	pkTemp->DeepCopy( m_pClipBoard );

	// 새로 구성한 노드를 윈도우에 삽입!!
	m_pLeftWndMgr->InsertWindow(pkTemp, pkParent, i); // i번째 자식으로!!

	// 4. 트리 컨트롤러에서도 보여줘야 함
	// 트리에서 부모 핸들, 현재 핸들 얻기
	HTREEITEM hParent = m_CtrlLeftTreeView.GetParentItem(hTreeItem);

	// 처음 위치라면
	if( i == 0 )
		InsertChild( pkTemp, hParent, &m_CtrlLeftTreeView, TVI_FIRST );
	else{ // 그 외는 현재 선택된 자리
		HTREEITEM hPrevItem = m_CtrlLeftTreeView.GetPrevSiblingItem(hTreeItem);
		InsertChild (pkTemp, hParent, &m_CtrlLeftTreeView, hPrevItem );
	}
	// 마지막에 넣고 싶으면 그냥 부모노드에서 붙여넣기(ctrl+v)

}
