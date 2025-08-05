// FindUI.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "FindUI.h"
#include "D3DWndClass.h"


// CFindUI 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFindUI, CDialog)

CFindUI::CFindUI(CWnd* pParent /*=NULL*/)
	: CDialog(CFindUI::IDD, pParent)
{
    m_bTreeviewWindow = true;
    m_pCtlTreeView = NULL;
    m_pListCtrlView = NULL;
}

CFindUI::CFindUI(CWnd* pParent /*=NULL*/, bool IsTreeViewWindow )
: CDialog(CFindUI::IDD, pParent)
{
    m_bTreeviewWindow = IsTreeViewWindow;
    m_pCtlTreeView = NULL;
    m_pListCtrlView = NULL;
}

CFindUI::~CFindUI()
{
}

void CFindUI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFindUI, CDialog)
    ON_BN_CLICKED(IDC_FIND, &CFindUI::OnBnClickedFind)
END_MESSAGE_MAP()


// CFindUI 메시지 처리기입니다.

void CFindUI::SetFocuse()
{
    ::SetFocus( *GetDlgItem( IDC_FIND_EDIT ) );
}

void CFindUI::OnBnClickedFind()
{
    if( m_bTreeviewWindow )
        FindTreeViewWindows();
    else
        FindListViewWindows();

}

void CFindUI::FindTreeViewWindows( void )
{
    if( m_pCtlTreeView )
    {
        // 검색할 UI이름 가져오기
        CString strFindName;
        GetDlgItem(IDC_FIND_EDIT)->GetWindowText(strFindName);
        if( 0 >= strFindName.GetLength() )
        {
            ::MessageBoxA( NULL, "검색할 Type을 입력해 주세요!!", "알림", MB_TASKMODAL );
            return;
        }

        // 트리뷰 핸들얻기
        HWND hwnd = m_pCtlTreeView->GetSafeHwnd();

        // 현재선택된아이템가져오기
        KD3DWndClass *g_pkWndClass;
        HTREEITEM iNexttem;
        HTREEITEM item = m_pCtlTreeView->GetSelectedItem();

        if( NULL == item )
            item = TreeView_GetRoot( hwnd );

        bool bCheck = false;
        while( item )
        {
            if( bCheck )
            {
                bCheck = false;
                g_pkWndClass = (KD3DWndClass*)m_pCtlTreeView->GetItemData(item);
                if( strstr(g_pkWndClass->GetWndName().c_str(), strFindName) )
                    goto END_FIND;
            }

            // 차일드트리
            while( iNexttem = TreeView_GetChild( hwnd, item ) )
            {
                item = iNexttem;

                g_pkWndClass = (KD3DWndClass*)m_pCtlTreeView->GetItemData(item);
                if( strstr(g_pkWndClass->GetWndName().c_str(), strFindName) )
                    goto END_FIND;
            }

            // 형제트리
            if( iNexttem = TreeView_GetNextSibling( hwnd, item ) )
            {
                item = iNexttem;
                bCheck = true;
                continue;
            }

            // 부모트리
GOTO_PARENT:
            if( iNexttem = TreeView_GetParent( hwnd, item ) )
            {
                item = TreeView_GetNextSibling( hwnd, iNexttem );
                if( NULL == item && iNexttem != TreeView_GetRoot( hwnd ))
                {
                    item = iNexttem;
                    goto GOTO_PARENT;
                }

                bCheck = true;
                continue;
            }
            break;
        }

END_FIND:
        if( item )
        {
            m_pCtlTreeView->SelectItem( item );
        }
        else
        {
            ::MessageBoxA(NULL, "해당 이름을 가진 UI를 찾지 못했습니다.", "알림", MB_TASKMODAL);
        }
    }
}

void CFindUI::FindListViewWindows( void )
{
    RECT *rect;
    this->GetWindowRect(rect);
    if( m_pListCtrlView )
    {
        CString strFindName;
        GetDlgItem(IDC_FIND_EDIT)->GetWindowText(strFindName);
        if( 0 >= strFindName.GetLength() )
        {
            ::MessageBoxA( NULL, "검색할 Type을 입력해 주세요!!", "알림", MB_TASKMODAL );
            return;
        }

        KD3DWndClass *g_pkWndClass;
        POSITION pos = m_pListCtrlView->GetFirstSelectedItemPosition();
        bool bFind = false;

        for( int i = (int)pos ; i < m_pListCtrlView->GetItemCount() ; i++ )
        {
            g_pkWndClass = (KD3DWndClass*)m_pListCtrlView->GetItemData(i);
            if( strstr(g_pkWndClass->GetWndName().c_str(), strFindName) )
            {
                m_pListCtrlView->SetItemState(-1,0,LVIS_SELECTED|LVIS_FOCUSED);
                m_pListCtrlView->SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
                m_pListCtrlView->EnsureVisible(i,FALSE);

                POSITION cPos = m_pListCtrlView->GetFirstSelectedItemPosition();

                if( pos != cPos )
                {
                    bFind = true;
                    break;
                }
            }
        }

        if( bFind == false )
        {
            m_pListCtrlView->SetItemState(-1,0,LVIS_SELECTED|LVIS_FOCUSED);
            m_pListCtrlView->SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
            m_pListCtrlView->EnsureVisible(0,FALSE);
            ::MessageBoxA(NULL, "해당 이름을 가진 UI를 찾지 못했습니다.", "알림", MB_TASKMODAL);
        }
    }
}
