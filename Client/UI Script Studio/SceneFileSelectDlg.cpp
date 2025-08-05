// SceneFileSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "SceneFileSelectDlg.h"
#include ".\scenefileselectdlg.h"



// CSceneFileSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSceneFileSelectDlg, CDialog)
CSceneFileSelectDlg::CSceneFileSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneFileSelectDlg::IDD, pParent)
{
}

CSceneFileSelectDlg::~CSceneFileSelectDlg()
{
}

void CSceneFileSelectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CONTROL_SCENE_SELECT, m_listCtrlSceneFile);
}


BOOL CSceneFileSelectDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_listCtrlSceneFile.SetExtendedStyle( LVS_LIST | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

    m_listCtrlSceneFile.InsertColumn(0, _T("File Name"));
    m_listCtrlSceneFile.InsertColumn(1,_T("Caption"));
    m_listCtrlSceneFile.SetColumnWidth(0,190);
    m_listCtrlSceneFile.SetColumnWidth(1,308);

    // 정렬
    std::sort( m_vecUIScriptFile.begin(), m_vecUIScriptFile.end() );

    // 검색을 하면 m_vecUIScriptFile가 재구성 되므로 원본을 기억
    m_vecOrgUIScriptFile = m_vecUIScriptFile;

    AddFileNameToListControl(m_vecUIScriptFile);

    m_listCtrlSceneFile.GetTopIndex();

    return TRUE;
}


BEGIN_MESSAGE_MAP(CSceneFileSelectDlg, CDialog)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_SCENE_FIND, &CSceneFileSelectDlg::OnBnClickedSceneFind)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CONTROL_SCENE_SELECT, &CSceneFileSelectDlg::OnNMClickListControlSceneSelect)
    ON_BN_CLICKED(IDC_SCENE_FIND_TYPE, &CSceneFileSelectDlg::OnBnClickedSceneFindType)
    ON_EN_CHANGE(IDC_SCENE_EDIT, &CSceneFileSelectDlg::OnEnChangeSceneEdit)
    ON_NOTIFY(NM_RETURN, IDC_LIST_CONTROL_SCENE_SELECT, &CSceneFileSelectDlg::OnNMReturnListControlSceneSelect)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CONTROL_SCENE_SELECT, &CSceneFileSelectDlg::OnLvnKeydownListControlSceneSelect)
    ON_NOTIFY(NM_SETFOCUS, IDC_LIST_CONTROL_SCENE_SELECT, &CSceneFileSelectDlg::OnNMSetfocusListControlSceneSelect)
END_MESSAGE_MAP()

void CSceneFileSelectDlg::OnOK()
{
    CString strFullPath = m_strFilePath;
    strFullPath += m_strSelectedFile; 
    g_WndMgr->LoadLua( ToCharStr( strFullPath.GetString() ) );
    g_WndMgr->DataMapping();

    CDialog::OnOK();
}

// CSceneFileSelectDlg 메시지 처리기입니다.

void CSceneFileSelectDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
}


// 검색코드 재작성 2011. 09. 05

//--------------------------------------------------
// 파일이름 검색
//--------------------------------------------------
void CSceneFileSelectDlg::OnBnClickedSceneFind()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strFindType;
    GetDlgItem(IDC_SCENE_EDIT)->GetWindowText(strFindType);
    if( 0 >= strFindType.GetLength() ) {
        //::MessageBoxA( NULL, "검색할 Type을 입력해 주세요!!", "알림", MB_OK );
        // 경고메시지 대신 검색하기 이전 모든 목록을 표시해 주자
        m_vecUIScriptFile = m_vecOrgUIScriptFile;
        AddFileNameToListControl( m_vecUIScriptFile );
    }
    else {
        m_vecUIScriptFile.clear();  // 다 지우고 검색된 목록으로 갱신한다.

        for( int i = 0; i < m_vecOrgUIScriptFile.size(); ++i) {
            if( strstr( m_vecOrgUIScriptFile[i].strFileName.c_str(), strFindType.GetString() ) )
                m_vecUIScriptFile.push_back( m_vecOrgUIScriptFile[i] );
        }
        AddFileNameToListControl( m_vecUIScriptFile );
    }
    
    // 처음 파일을 찾는 파일로 간주
    m_strSelectedFile = m_vecUIScriptFile.begin()->strFileName.c_str();
}

//--------------------------------------------------
// 타입 검색
//--------------------------------------------------
void CSceneFileSelectDlg::OnBnClickedSceneFindType()
{
    CString strFindType;
    GetDlgItem(IDC_SCENE_EDIT)->GetWindowText(strFindType);
    if( 0 >= strFindType.GetLength() ) {
        //::MessageBoxA( NULL, "검색할 Type을 입력해 주세요!!", "알림", MB_OK );
        // 경고메시지 대신 검색하기 이전 모든 목록을 표시해 주자
        m_vecUIScriptFile = m_vecOrgUIScriptFile;
        AddFileNameToListControl( m_vecUIScriptFile );
    } else {
        FILE *pFile;
        char szTemp[1024];
        CString strFullPath;

        m_vecUIScriptFile.clear();

        for ( int i = 0;i < m_vecOrgUIScriptFile.size(); i++ ) {
            strFullPath = m_strFilePath;
            strFullPath += m_vecOrgUIScriptFile[i].strFileName.c_str();
            pFile = fopen( strFullPath, "rt" );

            if( pFile ) {
                while( fgets(szTemp, 1024, pFile) ) {
                    if( strstr(szTemp, strFindType) ) {
                        m_vecUIScriptFile.push_back( m_vecOrgUIScriptFile[i] );
                        break;
                    }
                }
                fclose( pFile );
            }
        }

        AddFileNameToListControl( m_vecUIScriptFile );
        //else
        //::MessageBoxA( NULL, "해당 Type을 사용하는 UI가 없습니다.", "알림", MB_OK );
    }
}

//void CSceneFileSelectDlg::AddFileList( std::vector<std::string> &vecFileName)
//{
//    m_listbox_scene.ResetContent();
//    for ( int i = 0;i < vecFileName.size(); i++ )
//    {
//        m_listbox_scene.AddString( vecFileName[i].c_str() );
//    }
//
//    if( m_listbox_scene.GetCount() )
//        m_listbox_scene.SetCurSel(0);
//}

//--------------------------------------------------
// 데이터 구조가 바뀌면서 위 코드 대신
//--------------------------------------------------
void CSceneFileSelectDlg::AddFileNameToListControl(std::vector<tagUIScriptFile> &vecFileName)
{
    m_listCtrlSceneFile.DeleteAllItems();

    for( int i=0; i < vecFileName.size(); ++i ){
        m_listCtrlSceneFile.InsertItem( i, vecFileName[i].strFileName.c_str() );
        m_listCtrlSceneFile.SetItemText(i, 1, vecFileName[i].strFileCaption.c_str() );
        
    }
}

//--------------------------------------------------
// 현재 클릭한 Item에서 파일이름 저장하기
// 이후 ok 버튼을 누르면 저장된 파일이름으로 바로 로드
//--------------------------------------------------
void CSceneFileSelectDlg::OnNMClickListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    m_strSelectedFile = m_listCtrlSceneFile.GetItemText(pNMIA->iItem, 0);

    // 하단 static box에 설명의 내용을 모두 표시해 준다. (장문 설명일 경우 리스트에서 전부 볼 수가 없음)
    CString m_strCaption;

    m_strCaption = m_listCtrlSceneFile.GetItemText(pNMIA->iItem, 1);
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(m_strCaption);

    *pResult = 0;
}


void CSceneFileSelectDlg::OnEnChangeSceneEdit()
{
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
    // ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
    // 해당 알림 메시지를 보냅니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnBnClickedSceneFind();

    ListView_SetItemState( m_listCtrlSceneFile.GetSafeHwnd(), 0, LVIS_SELECTED, 0x000F );
}

void CSceneFileSelectDlg::OnNMReturnListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();

    *pResult = 0;
}

void CSceneFileSelectDlg::OnLvnKeydownListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int nItem = 0;

    // 방향키로 리스트에서 선택하면 그 UI 파일 이름을 기억해 두자
    if( pLVKeyDow->wVKey == VK_DOWN ) {
        nItem = m_listCtrlSceneFile.GetSelectionMark() + 1;

        if( nItem >= m_vecUIScriptFile.size() )
            nItem = m_vecUIScriptFile.size() - 1;

        m_strSelectedFile = m_vecUIScriptFile[nItem].strFileName.c_str();
    } else if( pLVKeyDow->wVKey == VK_UP ){
        nItem = m_listCtrlSceneFile.GetSelectionMark() - 1;

        if( nItem < 0 )
            nItem = 0;

        m_strSelectedFile = m_vecUIScriptFile[nItem].strFileName.c_str();
    }

    *pResult = 0;
}

void CSceneFileSelectDlg::OnNMSetfocusListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int nItem = nItem = m_listCtrlSceneFile.GetSelectionMark();

    // 포커스가 넘어왔을 때 인덱스가 0보다 작으면 첫번째으로 맞춤.
    if( nItem < 0 )
        ListView_SetItemState( m_listCtrlSceneFile.GetSafeHwnd(), 0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F );

    *pResult = 0;

}
