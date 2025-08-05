// GCFileDiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCFileDiff.h"
#include "GCFileDiffDlg.h"
#include "EnumFile.h"
#include "KMsgBox.h"
#include "io.h"
#include ".\gcfilediffdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DeleteDirectoryAbsolutely( std::string  strDirectory_ );
void AppendBackSlash( std::string& str ) ;
void DeleteFileAbsolutely( const char* szFileName ) ;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffDlg dialog

CGCFileDiffDlg::CGCFileDiffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCFileDiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGCFileDiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCFileDiffDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGCFileDiffDlg)  
    //}}AFX_DATA_MAP
   
}

BEGIN_MESSAGE_MAP(CGCFileDiffDlg, CDialog)
	//{{AFX_MSG_MAP(CGCFileDiffDlg)
	//}}AFX_MSG_MAP
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_OLD_PATH, OnBtnOldPath)
    ON_BN_CLICKED(IDC_BTN_NEW_PATH, OnBtnNewPath)
    ON_BN_CLICKED(IDOK, OnOk)
    ON_BN_CLICKED(IDC_BTN_UPDATE_PATH, OnBnClickedBtnUpdatePath)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffDlg message handlers

BOOL CGCFileDiffDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    
	
	// TODO: Add extra initialization here

    m_editOldPath.SubclassDlgItem(IDC_EDIT_OLD_PATH, this);
    m_editNewPath.SubclassDlgItem(IDC_EDIT_NEW_PATH, this);
    m_editUpdatePath.SubclassDlgItem(IDC_EDIT_UPDATE_PATH, this);
    m_editOldPath.SetUseDir(TRUE);
    m_editNewPath.SetUseDir(TRUE);
    m_editUpdatePath.SetUseDir(TRUE);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGCFileDiffDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGCFileDiffDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGCFileDiffDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//------------------------------------------------------------------------------
// 퍼온 소스 시작.
//------------------------------------------------------------------------------

//******************************************************************************
// Function name     - GetFolder
// Description       - Get a folder path
// 콧診 溢怏淸魏打?  - 25.09.2000
// 碣?溢怏淸打尊循壯 - S. Sokolenko
// In                - strSelectedFolder -
//                         reference to string for store folder path
// Out               - lpszTitle - title for caption
//                     hwndOwner - reference to parent window 
//                     strRootFolder - root folder 
//                     strStartFolder - current foldet
// Return            - TRUE if user select OK, else FALSE.
//******************************************************************************

CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    TCHAR szDir[MAX_PATH];

    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        if (lpData)
        {
            strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
            SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
        }
        break;
    case BFFM_SELCHANGED:
        {
            if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir))
            {
                SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
            }
            break;
        }
    default:
        break;
    }

    return 0;
}

BOOL GetFolder(CString* strSelectedFolder,
               const char* lpszTitle,
               const HWND hwndOwner, 
               const char* strRootFolder, 
               const char* strStartFolder)
{
    char pszDisplayName[MAX_PATH];
    LPITEMIDLIST lpID;
    BROWSEINFOA bi;

    bi.hwndOwner = hwndOwner;
    if (strRootFolder == NULL)
    {
        bi.pidlRoot = NULL;
    }
    else
    {
        LPITEMIDLIST  pIdl = NULL;
        IShellFolder* pDesktopFolder;
        char          szPath[MAX_PATH];
        OLECHAR       olePath[MAX_PATH];
        ULONG         chEaten;
        ULONG         dwAttributes;

        strcpy(szPath, (LPCTSTR)strRootFolder);
        if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
        {
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
            pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
            pDesktopFolder->Release();
        }
        bi.pidlRoot = pIdl;
    }
    bi.pszDisplayName = pszDisplayName;
    bi.lpszTitle = lpszTitle;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
    bi.lpfn = BrowseCallbackProc;
    if (strStartFolder == NULL)
    {
        bi.lParam = FALSE;
    }
    else
    {
        strTmpPath.Format("%s", strStartFolder);
        bi.lParam = TRUE;
    }
    bi.iImage = NULL;
    lpID = SHBrowseForFolderA(&bi);
    if (lpID != NULL)
    {
        BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
        if (b == TRUE)
        {
            strSelectedFolder->Format("%s",pszDisplayName);
            return TRUE;
        }
    }
    else
    {
        strSelectedFolder->Empty();
    }
    return FALSE;
}

//------------------------------------------------------------------------------
// 퍼온 소스 끝.
//------------------------------------------------------------------------------

void CGCFileDiffDlg::OnBtnOldPath(void)
{
    CString strFolderPath;
    if (TRUE == GetFolder(&strFolderPath,
                          "폴더 경로를 선택해 주세요.",
                          m_hWnd,
                          NULL,
                          NULL))
    {
        if (FALSE == strFolderPath.IsEmpty())
        {
            m_editOldPath.SetWindowText(strFolderPath);
        }
    }
}

void CGCFileDiffDlg::OnBtnNewPath(void)
{
    CString strFolderPath;
    if (TRUE == GetFolder(&strFolderPath,
                          "폴더 경로를 선택해 주세요.",
                          m_hWnd,
                          NULL,
                          NULL))
    {
        if (FALSE == strFolderPath.IsEmpty())
        {
            m_editNewPath.SetWindowText(strFolderPath);
        }
    }
}

void CGCFileDiffDlg::OnOk(void)
{
    KEnumFile kef;

    CString cstrOldPath;
    CString cstrNewPath;
    CString cstrUpdatePath;

    m_editOldPath.GetWindowText(cstrOldPath);
    m_editNewPath.GetWindowText(cstrNewPath);
    m_editUpdatePath.GetWindowText(cstrUpdatePath);

    std::string strOldPath = cstrOldPath;
    std::string strNewPath = cstrNewPath;
    std::string strUpdatePath = cstrUpdatePath;

    if ('\\' != strOldPath[strOldPath.size() - 1]) strOldPath.append("\\");
    if ('\\' != strNewPath[strNewPath.size() - 1]) strNewPath.append("\\");
    if ('\\' != strUpdatePath[strUpdatePath.size() - 1]) strUpdatePath.append("\\");

    if ("\\" != strOldPath && "\\" != strNewPath)
    {
        if (strUpdatePath !="\\")
        {
            if(::_access( strUpdatePath.c_str(), 0)!=-1)
            {
                DeleteDirectoryAbsolutely(strUpdatePath.c_str());
                MessageBox("기존 폴더를 삭제하고 압축을 풉니다.\n");
            }
            if( !CreateDirectory(strUpdatePath.c_str(), NULL ) )
            {   

                MessageBox("MKOM파일을 정리할 폴더가 이미 존재하거나 생성할 수 없습니다.\n");
                return;
            }
        }

        // 이전 버전 NewVer.xml 파일의 경로 생성.
        std::string strOldFileName = strOldPath;
        strOldFileName.append("newver.xml");

        // 이전 버전 NewVer.xml 파일이 존재하는지 확인하여
        // 버전을 초기화할 것인지 물어본다.
        if (-1 == ::_access(strOldFileName.c_str(), 0))
        {
            if (IDCANCEL == KMsgBox::Open("이전 버전의 NewVer.xml이 존재안함.",
                                          "버전을 초기화하겠습니까?", m_hWnd,
                                          KMsgBox::MBOX_OKCANCEL))
            {
                return;
            }
        }

        // 이전 버전의 NewVer.xml을 읽어 들임.
        TiXmlDocument xmlOldDoc_;
        TiXmlDocument xmlOldDoc(strOldFileName.c_str());
        if (false == xmlOldDoc_.LoadFile(strOldFileName.c_str()))
        {
            kef.CreateFileList(strOldPath.c_str(), xmlOldDoc);
        }
        else
        {
            xmlOldDoc.LoadFile();
        }

        // 유효성 체크섬 추가.
        KEnumFile::Signing(xmlOldDoc);
        xmlOldDoc.SaveFile();

        // 처리 중임을 알려주는 다이얼로그 띄우기.
        KMsgBox::Open("갱신된 NewVer.xml 생성 중", "기다려 주십시오.", m_hWnd);

        // 새 버전 NewVer.xml 파일의 경로 생성.
        std::string strNewFileName = strNewPath;
        strNewFileName.append("newver.xml");

        // 이미 새 버전 NewVer.xml이 존재하면 지운다.
        if (0 == ::_access(strNewFileName.c_str(), 0))
        {
            ::DeleteFile(strNewFileName.c_str());
        }

        std::string strNewCrcFileName = strNewPath;
        strNewCrcFileName.append("crclIstnew.kom");

        // 이미 새 버전 crclIstnew.kom이 존재하면 지운다.
        if (0 == ::_access(strNewCrcFileName.c_str(), 0))
        {
            ::DeleteFile(strNewCrcFileName.c_str());
        }

        // 새 버전 경로에서 file list를 생성.
        TiXmlDocument xmlNewDoc;
        kef.CreateFileList(strNewPath.c_str(), xmlNewDoc);

        if (strUpdatePath=="\\")
        {
            kef.SetMKomPath("");
        }
        else
        {
            kef.SetMKomPath(strUpdatePath);
        }


        // 위의 두 파일로부터 patch 정보가 갱신된 updated list를 생성
        TiXmlDocument xmlUpdatedDoc(strNewFileName.c_str());
        if (true == kef.CompareFileVersion(strOldPath.c_str(),
                                           xmlOldDoc,
                                           strNewPath.c_str(),
                                           xmlNewDoc,
                                           xmlUpdatedDoc))
        {
            KEnumFile::Signing(xmlUpdatedDoc);
            xmlUpdatedDoc.SaveFile();
            if (strUpdatePath!="\\")
            {
                //MKOM폴더에 NewVer을 복사해 준다.
                CopyFileA(strNewFileName.c_str(),(strUpdatePath+(std::string)"\\newver.xml").c_str(),FALSE);
            }
        }
        else
        {
            KMsgBox::Close();
            KMsgBox::Open("갱신된 newver.xml 파일을", "생성할 수 없습니다.",
                          m_hWnd, KMsgBox::MBOX_OK);
            return;
        }

        // 완료했음을 알리고 종료한다.
        KMsgBox::Close();
        KMsgBox::Open("주문하신 카라멜 마끼야또께서", "나오십니다.",
            m_hWnd );


        if ( KEnumFile::CreateCRCKOM( strNewPath.c_str() ,"newver.xml" , "crclIstnew.kom" ) )
        {
            if (strUpdatePath!="\\")
            {
                //MKOM폴더에 NewVer을 복사해 준다.
                CopyFileA((strNewPath+(std::string)"\\crclIstnew.kom").c_str(),(strUpdatePath+(std::string)"\\crclIstnew.kom").c_str(),FALSE);
            }
        }

        KMsgBox::Close();

        KMsgBox::Open("갱신된 newver.xml 파일 생성을", "완료했습니다.",
                      m_hWnd, KMsgBox::MBOX_OK);
        CDialog::OnOK();
    }
    else
    {
        KMsgBox::Open("폴더 경로가 잘못되었습니다.",
                      "폴더 경로를 다시 설정해주십시오.",
                      m_hWnd, KMsgBox::MBOX_OK);
    }
}
void CGCFileDiffDlg::OnBnClickedBtnUpdatePath()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strFolderPath;
    if (TRUE == GetFolder(&strFolderPath,
        "폴더 경로를 선택해 주세요.",
        m_hWnd,
        NULL,
        NULL))
    {
        if (FALSE == strFolderPath.IsEmpty())
        {
            m_editUpdatePath.SetWindowText(strFolderPath);
        }
    }
}

void DeleteDirectoryAbsolutely(std::string strDirectory_ )
{
    HANDLE          hFile;
    WIN32_FIND_DATAA	wfd;
    char strOldCurrentDirectory[_MAX_PATH] = {0,};
    char strNowCurrentDirectory[_MAX_PATH] = {0,};

    //strcpy(strNowCurrentDirectory ,strDirectory_);
    //::GetCurrentDirectoryA( _MAX_PATH, strOldCurrentDirectory );
    //::SetCurrentDirectoryA( strDirectory_ );

    AppendBackSlash(strDirectory_);
    hFile = FindFirstFileA( (strDirectory_+(std::string)"*.*").c_str(), &wfd );

    int nTempLen = 0;
    do
    {
        std::string strDeleteFile;
        strDeleteFile = strDirectory_.c_str();
        AppendBackSlash( strDeleteFile );
        strDeleteFile.append( wfd.cFileName );
        if (wfd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY &&
            !(strcmp(wfd.cFileName,".")==0 || strcmp(wfd.cFileName,"..")==0)) 
        {
            DeleteDirectoryAbsolutely((char *)strDeleteFile.c_str());
        }
        else
        {
            DeleteFileAbsolutely( strDeleteFile.c_str() ); 
        }
    }while( FindNextFileA(hFile, &wfd) != NULL );
    FindClose( hFile );
    ::RemoveDirectoryA(strDirectory_.c_str());     
}

void DeleteFileAbsolutely( const char* szFileName ) 
{
    ::SetFileAttributesA( szFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/ );
    ::DeleteFileA( szFileName );
}
void AppendBackSlash( std::string& str ) 
{
    if ( '\\' != str[str.size() - 1] )
    {
        str.append( "\\" );
    }
}
