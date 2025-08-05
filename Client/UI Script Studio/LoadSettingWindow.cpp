// LoadSettingWindow.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "LoadSettingWindow.h"
#include ".\loadsettingwindow.h"
#include "SelectDialog.h"
#include "SettingListDlg.h"


// CLoadSettingWindow 대화 상자입니다.
IMPLEMENT_DYNAMIC(CLoadSettingWindow, CDialog)
CLoadSettingWindow::CLoadSettingWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadSettingWindow::IDD, pParent)
{
}

CLoadSettingWindow::~CLoadSettingWindow()
{
}

static int CALLBACK MyBrowseCallbackProc(
    HWND hwnd, 
    UINT uMsg, 
    LPARAM lParam, 
    LPARAM lpData 
    )
{
    switch (uMsg) {
    case BFFM_INITIALIZED:
        ::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, lpData );
        break;
    }
    return 0;
}

void CLoadSettingWindow::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST2, m_lbTexture);
    DDX_Control(pDX, IDC_LIST3, m_lbScript);
    DDX_Control(pDX, IDC_RADIO_ONLY_NAME, m_raOnlyName);
    DDX_Control(pDX, IDC_RADIO_FULL, m_raFullPath);
}


BEGIN_MESSAGE_MAP(CLoadSettingWindow, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_ADD_TEXTURE, OnBnClickedButtonAddTexture)
    ON_BN_CLICKED(IDC_BUTTON_ADD_SCRIPT, OnBnClickedButtonAddScript)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_COMMAND(IDC_RADIO_ONLY_NAME, OnShowType)
    ON_COMMAND(IDC_RADIO_FULL, OnShowType)
    ON_LBN_DBLCLK(IDC_LIST2, OnLbnDblclkList2)
    ON_LBN_DBLCLK(IDC_LIST3, OnLbnDblclkList3)
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_BUTTON_TEX_DELETE, OnBnClickedButtonTexDelete)
    ON_BN_CLICKED(IDC_BUTTON_TEX_SCRIPT, OnBnClickedButtonTexScript)
    ON_BN_CLICKED(ID_SAVE_STATE_SETTING, &CLoadSettingWindow::OnBnClickedSaveSetting)    
END_MESSAGE_MAP()


// CLoadSettingWindow 메시지 처리기입니다.

void CLoadSettingWindow::OnBnClickedButtonAddTexture()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelectDialog dlg( TRUE, _T("*.*"), NULL, OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT, _T("Texture Folders or Kom files|*.kom||") );

	try
	{
		if( dlg.DoModal() == IDOK )
		{
			for( int i=0; i<dlg.m_SelectedItemList.GetCount(); i++ )
				AddList( dlg.m_SelectedItemList[i], &m_vtTextureList );
		}
	}
	catch (CException* e)
	{
		MessageBox( _T("파일을 불러오던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요."), _T("알림"), MB_OK);
	}
}

void CLoadSettingWindow::OnBnClickedButtonAddScript()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CFileDialog dlg(TRUE, _T("*.stg(*.stg)"), _T("*.stg"), OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | 
        OFN_FILEMUSTEXIST, _T("KOM_File(*.stg)|*.stg|"), NULL);

    CString strPath;// = SiCDlgDirectorySetting()->m_strQuestScriptPath;
    dlg.m_ofn.lpstrInitialDir = (LPSTR)(strPath.GetBuffer());

    try
    {
        if( IDOK == dlg.DoModal() )
        {
            POSITION pos = dlg.GetStartPosition();
            while(pos)
                AddList(dlg.GetNextPathName(pos).GetBuffer(), false);
        }
    }
    catch (...)
    {
        MessageBox( _T("파일을 불러오던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요."), _T("알림"), MB_OK);
    }
}

void CLoadSettingWindow::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    if( WriteToRegister() )
    {
        MessageBox( _T("저장에 성공하였습니다~!"), _T("Great~!"), MB_OK );
        OnOK();
    }
    else
    {
        MessageBox( _T("저장에 실패하였습니다.. T.T"), _T("Error"), MB_OK );
    }
}

void CLoadSettingWindow::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void CLoadSettingWindow::OpenDirectoryDialog(CString *pstrResult, CString strTitle, CString strPath)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    LPITEMIDLIST pidl = NULL;
    char pszPathname[2048]; // 경로 저장을 위한 변수
    BROWSEINFO BrInfo; 
    BrInfo.hwndOwner = GetSafeHwnd();
    BrInfo.pidlRoot = NULL; 
    memset( &BrInfo, 0, sizeof( BrInfo ) ); 
    BrInfo.lpszTitle = strTitle.GetBuffer();
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS; 
    BrInfo.lpfn = MyBrowseCallbackProc;
    BrInfo.pszDisplayName = strPath.GetBuffer();
    BrInfo.lParam = (LPARAM)strPath.GetBuffer();

    pidl = SHBrowseForFolder( &BrInfo );
    if( NULL != pidl ) 
    { 
        SHGetPathFromIDListA( pidl, pszPathname ); // pszPathname : 디렉토리 위치가 저장이 됩니다. 
        *pstrResult = pszPathname;
    }
}


bool CLoadSettingWindow::WriteToRegister(void)
{
    HKEY hkey;
    DWORD dwType = REG_DWORD;
    DWORD dwLength = sizeof(DWORD);
    
    RegDeleteKeyA( HKEY_CURRENT_USER, "KOG_UISCRIPT_FILE" );

    if( ERROR_SUCCESS == RegCreateKeyExA( HKEY_CURRENT_USER, "KOG_UISCRIPT_FILE", 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // Write the window width/height.  This is just an example,
        // of how to use DXUtil_Write*() functions.

        char szName[2048];
        BYTE szFilePath[2048];

        for(int i=0;i<(int)m_vtTextureList.size();++i)
        {
            //sprintf(szName, "Texture%d", i);
			sprintf_s(szName, sizeof(szName), "Texture%d", i);

            RegSetValueExA( hkey, szName, 0, REG_SZ, (BYTE*)m_vtTextureList[i].c_str(), m_vtTextureList[i].length()+1 );
        }

        for(int i=0;i<(int)m_vtScriptList.size();++i)
        {
            //sprintf(szName, "Script%d", i);
			sprintf_s(szName, sizeof(szName), "Script%d", i);

            RegSetValueExA( hkey, szName, 0, REG_SZ, (BYTE*)m_vtScriptList[i].c_str(), m_vtScriptList[i].length()+1 );
        }

        RegCloseKey( hkey );

        return true;
    }

    return false;
}

bool CLoadSettingWindow::GetFromRegister(void)
{
    HKEY hkey;
    if( ERROR_SUCCESS == RegOpenKeyExA( HKEY_CURRENT_USER, "KOG_UISCRIPT_FILE", 
        0, KEY_ALL_ACCESS, &hkey) )
    {
        // Read the stored window width/height.  This is just an example,
        // of how to use DXUtil_Read*() functions.
        char szName[2048];
        BYTE szFilePath[2048];
        DWORD data_size = 2048;

        m_vtTextureList.clear();
        m_vtScriptList.clear();

        for(int i=0;;++i)
        {
            //sprintf(szName, "Texture%d", i);
			sprintf_s(szName, sizeof(szName), "Texture%d", i);

            if( ERROR_SUCCESS != RegQueryValueExA( hkey, szName, 0, 0, szFilePath, &data_size ) )
                break;
            
            m_vtTextureList.push_back( (char*)szFilePath );
            
            data_size = 2048;
        }

        for(int i=0;;++i)
        {
            //sprintf(szName, "Script%d", i);
			sprintf_s(szName, sizeof(szName), "Script%d", i);

            if( ERROR_SUCCESS != RegQueryValueExA( hkey, szName, 0, 0, szFilePath, &data_size ) )
                break;

            m_vtScriptList.push_back( (char*)szFilePath );

            data_size = 2048;
        }

        RegCloseKey( hkey );

        return true;
    }

    return false;
}

void CLoadSettingWindow::OnDropFiles(HDROP hDropInfo)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    UINT  uNumFiles;
    TCHAR szNextFile [10240];

    // Get the # of files being dropped.
    uNumFiles = DragQueryFile ( hDropInfo, -1, NULL, 0 );

    bool bTexture = true;
    if( IDNO == MessageBox( _T("Is Texture?"), _T("Hm?"), MB_YESNO ) )
    {
        if( m_vtScriptList.size() )
        {
            MessageBox( _T("Script 파일이 1개 이상 존재 합니다. 스크립트 파일을 여러개 등록해두어도 맨 위의 스크립트만 사용됩니다."), _T("알림!"), MB_OK);
        }
        bTexture = false;
    }

    for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
    {
        // Get the next filename from the HDROP info.
        if ( DragQueryFile ( hDropInfo, uFile, szNextFile, 10240 ) > 0 )
        {
            SHFILEINFO shInfo;
            SHGetFileInfo(szNextFile,0,&shInfo,sizeof(SHFILEINFO)
                ,SHGFI_TYPENAME|SHGFI_ATTRIBUTES|SHGFI_SYSICONINDEX);

            if( shInfo.dwAttributes & SFGAO_FOLDER )
            {
                CString strPath;
                strPath.Format( _T("%s\\*.*"),szNextFile);

                AddFolderFileList( strPath, bTexture );
            }
            else
            {
                if( !AddList(szNextFile, bTexture) ) break;      // false 이면 더이상 추가하지 않고 리턴.
            }
        }
    }
    
    // Free up memory.
    DragFinish ( hDropInfo );

    CDialog::OnDropFiles(hDropInfo);
}

void CLoadSettingWindow::AddFolderFileList(CString strFolder, bool bTexture)
{
    CFileFind finder;
    SHFILEINFO shInfo;

    if(finder.FindFile(strFolder)==TRUE)
    {
        BOOL bFlag=true;
        while(bFlag)
        {
            bFlag=finder.FindNextFile();
            SHGetFileInfo(finder.GetFilePath(),0,&shInfo,sizeof(SHFILEINFO)
                ,SHGFI_TYPENAME|SHGFI_ATTRIBUTES|SHGFI_SYSICONINDEX);

            if( !finder.IsDots()                           //'.'과'..'은 뛰어 넘는다.
                && !finder.IsHidden() )
            {
                if( !(shInfo.dwAttributes & SFGAO_FOLDER) )
                {
                    if( !AddList(finder.GetFilePath(), bTexture) ) return;    // false 이면 더이상 추가 하지 않고 리턴.
                }
            }
        }
    }
}

bool CLoadSettingWindow::AddList(CString strFileName, bool bTexture)
{
    strFileName.Replace('\\', '/');
    if( bTexture )
    {
		AddTexture( strFileName );
    }
    else
    {
		AddScript( strFileName );
    }

    return true;
}

void CLoadSettingWindow::AddTexture( CString strFileName )
{
	// 폴더면 끝에 "/" 를 붙여준다.
	if( strFileName.Right(4).CompareNoCase( _T(".KOM") ) != 0 )
	{
		strFileName.Append( _T("/" ) );
	}

	m_lbTexture.InsertString( m_lbTexture.GetCount(), strFileName.GetBuffer() );
	m_vtTextureList.push_back( ToCharStr( strFileName.GetBuffer() ) );
}

void CLoadSettingWindow::AddScript( CString strFileName )
{
	m_lbScript.InsertString( m_lbScript.GetCount(), strFileName.GetBuffer() );
	m_vtScriptList.push_back( ToCharStr( strFileName.GetBuffer() ) );
}

BOOL CLoadSettingWindow::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.

    GetFromRegister();

    FullPatthMode( true );

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLoadSettingWindow::FullPatthMode(bool bShow)
{
    m_raFullPath.SetCheck( bShow );
    m_raOnlyName.SetCheck( !bShow );

    m_lbTexture.ResetContent();
    m_lbScript.ResetContent();

    if( bShow )
    {
        for(int i=0;i<(int)m_vtTextureList.size();++i)
        {
            m_lbTexture.InsertString(i, m_vtTextureList[i].c_str());
        }

        for(int i=0;i<(int)m_vtScriptList.size();++i)
        {
            m_lbScript.InsertString(i, m_vtScriptList[i].c_str());
        }
    }
    else
    {
        char szFileName[2048];
        char szExt[2048];
        for(int i=0;i<(int)m_vtTextureList.size();++i)
        {
            CString strName;
            _splitpath(m_vtTextureList[i].c_str(), 0, 0, szFileName, szExt);
            strName = szFileName;
            //strName += ".";
            strName += szExt;
            m_lbTexture.InsertString(i, strName);
        }

        for(int i=0;i<(int)m_vtScriptList.size();++i)
        {
            CString strName;
            _splitpath(m_vtTextureList[i].c_str(), 0, 0, szFileName, szExt);
            strName = szFileName;
            //strName += ".";
            strName += szExt;
            m_lbScript.InsertString(i, strName);
        }
    }
}

void CLoadSettingWindow::OnShowType()
{
    FullPatthMode( m_raFullPath.GetCheck() );
}

void CLoadSettingWindow::DeleteList(int iIndex, bool bTexture)
{
    if( bTexture )
    {
        m_lbTexture.DeleteString(iIndex);
        m_vtTextureList.erase( m_vtTextureList.begin() + iIndex );
    }
    else
    {
        m_lbScript.DeleteString(iIndex);
        m_vtScriptList.erase( m_vtScriptList.begin() + iIndex );
    }
}

void CLoadSettingWindow::OnLbnDblclkList2()
{
    //TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    DeleteList( m_lbTexture.GetCurSel(), true );
}

void CLoadSettingWindow::OnLbnDblclkList3()
{
    //TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    DeleteList( m_lbScript.GetCurSel(), false );
}

void CLoadSettingWindow::OnBnClickedButtonTexDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_vtTextureList.clear();
    m_lbTexture.ResetContent();
}

void CLoadSettingWindow::OnBnClickedButtonTexScript()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_vtScriptList.clear();
    m_lbScript.ResetContent();
}

//------------------------------------------------------------------------
//	NAME : SelectedWndListUpdate()
//	DATE : 20101130 박현민
//	DESC : Setting 버튼을 눌렀을때, SETTINGLIST_DIALOG를 호출한다.
//------------------------------------------------------------------------
void CLoadSettingWindow::OnBnClickedSaveSetting()
{
    CSettingListDlg dlg( NULL );
    dlg.SetLoadSettingInstance( this );
    dlg.DoModal();
}

