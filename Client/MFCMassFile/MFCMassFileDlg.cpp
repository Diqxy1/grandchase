// MFCMassFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCMassFile.h"
#include "MFCMassFileDlg.h"
#include "MassFileLib/GCMassFileManager.h"
#include <Mmsystem.h>
#include <afxcoll.h>
#include ".\mfcmassfiledlg.h"
//#include <algorithm>
#include "io.h"


/////////////////////////////////////////////////////////////////////////////
// CMFCMassFileDlg dialog

CMFCMassFileDlg*		g_pMassFileDlg = NULL;

bool CMFCMassFileDlg::s_bSortReverse = false;

CMFCMassFileDlg::CMFCMassFileDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMFCMassFileDlg::IDD, pParent)
, m_bUseMKOM(FALSE)
, m_bUseCRC(FALSE)
{
    g_pMassFileDlg = this;
    //{{AFX_DATA_INIT(CMFCMassFileDlg)
    m_KomFilePath = _T("");
    m_bZip = FALSE;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCMassFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFCMassFileDlg)
	DDX_Control(pDX, IDC_FILENAME, m_ListFileName);
	DDX_Text(pDX, IDC_KOMFILE, m_KomFilePath);
	DDX_Check(pDX, IDC_Zip, m_bZip);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_USE_MKOM, m_bUseMKOM);
	DDX_Check(pDX, IDC_NO_CRC, m_bUseCRC);
	DDX_Control(pDX, IDC_DECOMPRESS, btnDecompress);
}

BEGIN_MESSAGE_MAP(CMFCMassFileDlg, CDialog)
    //{{AFX_MSG_MAP(CMFCMassFileDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(LVN_BEGINDRAG, IDC_FILENAME, OnBegindragFilelist)
    ON_BN_CLICKED(IDC_ADD, OnAdd)
    ON_BN_CLICKED(IDC_REMOVE, OnRemove)
    ON_BN_CLICKED(IDC_DECOMPRESS, OnDecompress)
    ON_BN_CLICKED(IDC_OPENKOM, OnOpenkom)
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_DELETEALL, OnDeleteall)
    ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
    ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
    ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
    //}}AFX_MSG_MAP
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILENAME, OnLvnItemchangedFilename)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILENAME, OnLvnColumnclickFilename)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCMassFileDlg message handlers

BOOL CMFCMassFileDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    DragAcceptFiles();

    // TODO: Add extra initialization here
    //기본 crc.xml파일 사용
    m_bUseCRC=TRUE;
    UpdateData(FALSE);
    m_ListFileName.InsertColumn(0,"FileName",LVCFMT_LEFT,650);
    m_ListFileName.InsertColumn(1,"Size",LVCFMT_LEFT,100);

    CString strTotal;
    strTotal.Format("Total Files : %d",0);
    SetDlgItemText(IDC_FILEINFO, strTotal);
    //m_ListFileName.InsertItem(0,strTotal);

    //CommandLine을 받아서 인자가 있다면 kom파일을 더블클릭해서 실행한 것이라고 본다.
    //kom파일을 더블 클릭해서 실행하면 argc로 해당 파일명과 경로가 입력된다.

    bool shortcut = false;
    std::string winParam = ::GetCommandLine();		// 프로그램의 시작 파라메터(argc)를 읽어들입니다. ex) "D:\perfoceroot\tools\massfile_net\debug\massfile_net.exe 파라메터" 이런식으로 넘어옴.	

    while( winParam.find('"') != std::string::npos )
    {
        winParam.replace(winParam.find('"'), 1, "");
    }


    winParam.replace(0, winParam.find(":\\") + 1 , "" );
    if( winParam.find(":\\") != std::string::npos )
    {
        winParam.replace(0, winParam.find(":\\") - 2, "");
        shortcut = true;
    }

    if( winParam.find(" ") == 0 )
        winParam.replace(0, 1, "");

    if( winParam.compare("") != 0 && shortcut )	// 파라메터가 있는지 확인합니다.
    {
        if(GCMassFile::IsKomFile(winParam))	//파일 kom 파일인지 확인합니다.
        {
            m_KomFilePath = winParam.c_str();
            Load(m_KomFilePath);
            UpdateData(FALSE);
        }
    }	
    char strTitle[MAX_PATH];
    sprintf( strTitle,"GCR Manager", MASSFILE_VERSION/10 , MASSFILE_VERSION % 10 );

	SetWindowText(strTitle); // "KOM 압축툴 (MASSFILE V0.3)");
    return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCMassFileDlg::OnPaint() 
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
HCURSOR CMFCMassFileDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CMFCMassFileDlg::OnAdd() 
{
    static CString strFileName;

    CString strFilter = "All Files (*.*)|*.*|";

    CFileDialog dlg(TRUE,NULL,strFileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);

    if( dlg.DoModal() != IDOK )
        return;

    POSITION pos = dlg.GetStartPosition();

    while(pos != NULL)
    {
        int InsertPosition = m_ListFileName.GetItemCount();

        CString strFileName = dlg.GetNextPathName(pos);
        AddFile( (LPCTSTR)strFileName, m_MassFile.GetFileSize((LPTSTR)(LPCTSTR)strFileName) );
    }
    UpdateTotalCount();
}

void CMFCMassFileDlg::AddFile(const char* strFileName, DWORD dwFileSize )
{
    //중복되는 파일이 있는지 체크해보자..
    char strTemp1[MAX_PATH];
    char strTemp2[MAX_PATH];
    char strFileInList[MAX_PATH];

    m_MassFile.GetFileName( strTemp1, sizeof(strTemp1), (char*)strFileName );
    strlwr(strTemp1);

    for(int i = 0; i < m_ListFileName.GetItemCount(); i++)
    {
        m_ListFileName.GetItemText(i,0,strFileInList,MAX_PATH);
        m_MassFile.GetFileName( strTemp2, sizeof(strTemp2), strFileInList );

        strlwr(strTemp2);

        if( strcmp( strTemp1, strTemp2 ) == 0 )
        {
            //동일한 이름의 파일이 존재함
            if( MessageBox("동일한 이름의 파일이 존재합니다.\n기존의 파일을 대체하겠습니까?", strFileInList, MB_YESNO ) == IDYES )
            {
                delete (CString*)m_ListFileName.GetItemData(i);
                m_ListFileName.DeleteItem(i);
                break;
            }
            else
            {
                return;
            }
        }		
    }

    int InsertPosition = m_ListFileName.GetItemCount();
    char strfilesize[33];
    sprintf(strfilesize,"%d KB", dwFileSize / 1024 );

    m_ListFileName.InsertItem(InsertPosition,strFileName);
    m_ListFileName.SetItemData(InsertPosition,(DWORD_PTR)new CString(strFileName));
    m_ListFileName.SetItem(InsertPosition,1,LVIF_TEXT,strfilesize,0,0,0,0);
	btnDecompress.EnableWindow(FALSE);
}

void CMFCMassFileDlg::DeleteDirectoryAbsolutely( const char* strDirectory_ )
{
	HANDLE          hFile;
	WIN32_FIND_DATAA	wfd;
	char strOldCurrentDirectory[_MAX_PATH] = {0,};
	char strNowCurrentDirectory[_MAX_PATH] = {0,};

	strcpy(strNowCurrentDirectory ,strDirectory_);
	::GetCurrentDirectoryA( _MAX_PATH, strOldCurrentDirectory );
	::SetCurrentDirectoryA( strDirectory_ );
	hFile = FindFirstFileA( "*.*", &wfd );

	int nTempLen = 0;
	do
	{
		std::string strDeleteFile;
		strDeleteFile = strDirectory_;
		AppendBackSlash( strDeleteFile );
		strDeleteFile.append( wfd.cFileName );      
		DeleteFileAbsolutely( strDeleteFile.c_str() ); 
	}while( FindNextFileA(hFile, &wfd) != NULL );
	FindClose( hFile );

	::SetCurrentDirectoryA( strOldCurrentDirectory );

	::RemoveDirectoryA(strNowCurrentDirectory);     
}

void CMFCMassFileDlg::DeleteFileAbsolutely( const char* szFileName ) const
{
	DWORD dwAtt;
	if ( 0xffffffff != ( dwAtt = ::GetFileAttributesA( szFileName ) ) )
	{
		::SetFileAttributesA( szFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/ );
	}
	::DeleteFileA( szFileName );
}
void CMFCMassFileDlg::AppendBackSlash( std::string& str ) const
{
	if ( '\\' != str[str.size() - 1] )
	{
		str.append( "\\" );
	}
}

void CMFCMassFileDlg::OnOK() 
{		
    if (m_ListFileName.GetItemCount()==0)
        return;
    UpdateData();

    CString strFilter = "All GCR Files (*.first)|*.first|";

    CFileDialog dlg(FALSE,"gcr",m_KomFilePath,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

    if( dlg.DoModal() != IDOK )
        return;


    //여기서 압축!!!!
    std::vector<std::string> vecFileName;
    char strFileName[MAX_PATH];
    for(int i = 0; i < m_ListFileName.GetItemCount(); i++)
    {
        m_ListFileName.GetItemText(i,0,strFileName,MAX_PATH);
        vecFileName.push_back(strFileName);
    }
    //m_MassFile.SaveMassFile((LPTSTR)(LPCTSTR)dlg.GetPathName(),&vecFileName,m_bZip == TRUE);
    int rt;
    rt = m_MassFile.ModifyMassFile(&m_MassFile,(LPTSTR)(LPCTSTR)dlg.GetPathName(),&vecFileName,FBOOL(m_bUseCRC));

    if( rt == 0 )
    {
        MessageBox("Compression was successful.");
        Load(dlg.GetPathName().GetString());
        //추가된 파일등을 넣어서 저장 하였으면 MKOM으로 만들기 위해 경로가 사라진 목록을 다시 로드하여 목록 추출
        vecFileName.clear();
        for(int i = 0; i < m_ListFileName.GetItemCount(); i++)
        {
            m_ListFileName.GetItemText(i,0,strFileName,MAX_PATH);
            vecFileName.push_back(strFileName);
        }
        if (m_bUseMKOM==TRUE)
        {
            //압축을 풀 폴더 경로를 만年求?
            std::string strDestPath=dlg.GetPathName().GetString();
            int iWhere = (int)strDestPath.find_last_of( ".") ;
            strDestPath.erase( iWhere, strDestPath.length() - iWhere );
            strDestPath.append("\\");
            if(!(::_access( strDestPath.c_str(), 0)))
            {
                DeleteDirectoryAbsolutely(strDestPath.c_str());
                AfxMessageBox("Delete the existing folder and create the gcr file...");
            }
            if( !CreateDirectory(strDestPath.c_str(), NULL ) )
            {   

                AfxMessageBox("The folder to create the GCR file already exists or can not be created.");
                return;
            }
            m_MassFile.ModifyMiniMassFile(&m_MassFile,(char*)strDestPath.c_str(),&vecFileName);
            m_MassFile.GenerateMassFileXml(dlg.GetPathName().GetString(),strDestPath+"crc.xml"); //CRC파일 출력해준다.
        }
        return;
    }
    else
    {
        MessageBox("★★★★★Compression failed★★★★★");
        return;
    }	
}

void CMFCMassFileDlg::CompressFile( CFileDialog &dlg, std::vector<std::string>& vecFileName)
{	
    int rt;
    rt = m_MassFile.ModifyMassFile(&m_MassFile,(LPTSTR)(LPCTSTR)dlg.GetPathName(),&vecFileName,FBOOL(m_bUseCRC));

    if (m_bUseMKOM==TRUE)
    {
        std::string strPathName=(LPTSTR)(LPCTSTR)dlg.GetPathName();
        strPathName=strPathName.substr(0,strPathName.find_last_of("."));
        strPathName.append("\\");

        //압축을 풀 폴더 경로를 만年求?
        std::string strDestPath=dlg.GetPathName().GetString();
        int iWhere = (int) strDestPath.find_last_of( ".") ;
        strDestPath.erase( iWhere, strDestPath.length() - iWhere );
        strDestPath.append("\\");
        if( !CreateDirectory(strDestPath.c_str(), NULL ) )
        {        
            AfxMessageBox("MKOM파일을 정리할 폴더가 이미 존재하거나 생성할 수 없습니다.");
            return;
        }
        rt = m_MassFile.ModifyMiniMassFile(&m_MassFile,(char*)strDestPath.c_str(),&vecFileName);
    }

    if( rt == 0 )
    {
        MessageBox("Compression was successful.");		
    }
    else
    {
        MessageBox("★★★★★Compression failed★★★★★");		
    }
}

void CMFCMassFileDlg::OnRemove() 
{
    POSITION pos = m_ListFileName.GetFirstSelectedItemPosition();

    while(pos != NULL)
    {	
        pos = m_ListFileName.GetFirstSelectedItemPosition();
        int nSel = m_ListFileName.GetNextSelectedItem(pos);

        if( nSel < 0 || m_ListFileName.GetItemCount() <= nSel)
            continue;

        delete (CString*)m_ListFileName.GetItemData(nSel);
        m_ListFileName.DeleteItem(nSel);
    }
    UpdateTotalCount();
}



void CMFCMassFileDlg::OnDecompress() 
{
    if( m_KomFilePath == "" )
    {
        MessageBox("You have not loaded any GCR file. Unable to decompress.");
        return;
    }
    //	DecompressTest();
    //	return;
    int selectNum = m_ListFileName.GetSelectedCount();
    if( (selectNum == 0) )
    {		
        //MessageBox("먼저 파일을 선택해 주세요");
        //return;
        for( int i = 0; i < m_ListFileName.GetItemCount(); i++ )
            m_ListFileName.SetItemState(i, LVIS_SELECTED, -1);

        selectNum = m_ListFileName.GetSelectedCount();
    }

    CString* strSelected;
    if( selectNum != 0 )
    {
        strSelected = new CString [selectNum];
        POSITION pos = m_ListFileName.GetFirstSelectedItemPosition();
        int i = 0;
        while (pos)
        {
            int nItem; 
            nItem = m_ListFileName.GetNextSelectedItem(pos);
            strSelected[i] = *(CString*)( m_ListFileName.GetItemData( nItem ) );
            i++;
        }
    }

    ITEMIDLIST*		pidlBrowse;
    char            pszPathname[MAX_PATH];
    BROWSEINFO		BrInfo;

    BrInfo.hwndOwner = GetSafeHwnd();
    BrInfo.pidlRoot = NULL;

    memset( &BrInfo, 0, sizeof(BrInfo) );
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = "압축을 풀 디렉토리를 선택하세요";
    BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;


    // 다이얼로그를 띄우기
	//모달리스 창이 뜨므로 모달형태로 변경해 준다.
	this->EnableWindow(FALSE);
    pidlBrowse = ::SHBrowseForFolder(&BrInfo);    
	this->EnableWindow(TRUE);

    if( pidlBrowse == NULL)
    {
        //leak 제거
        SAFE_DELETE_ARRAY(strSelected);
        return;
    }

    // 패스를 얻어옴
    ::SHGetPathFromIDList(pidlBrowse, pszPathname);

    //CMassFile을 이용해 파일을 읽어냄
    m_MassFile.LoadMassFile(m_KomFilePath);
    UINT FileNo = m_MassFile.GetFileCount();
    GCMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
    memset(&FileInfo,0,sizeof(FileInfo));
    FileInfo.dwFlag = MFI_REALDATA;

    CString OutFileName;

    if( m_ListFileName.GetSelectedCount() != 0 )
    {
        for( int i = 0; i < selectNum; i++ )
        {
            m_MassFile.GetMemberFile((LPCTSTR)strSelected[i],&FileInfo);
            OutFileName.Format("%s\\%s",pszPathname,FileInfo.strFileName);
            FILE* file = fopen(OutFileName,"wb");
            if(file == NULL)
            {	
                MessageBox("압축해제 실패");				
                SAFE_DELETE_ARRAY(strSelected);

                return;
            }		
            fwrite(FileInfo.pRealData,1,FileInfo.ulSize,file);
            fclose(file);

            FileInfo.pRealData = NULL;
        }

        MessageBox("압축해제 완료");		
        SAFE_DELETE_ARRAY(strSelected);

        return;
    }
    for(int i = 0; i < (int)FileNo; i++)
    {
        m_MassFile.GetMemberFile(i,&FileInfo);
        OutFileName.Format("%s\\%s",pszPathname,FileInfo.strFileName);
        FILE* file = fopen(OutFileName,"wb");
        if(file == NULL)
        {
            MessageBox("압축해제 실패");			
            SAFE_DELETE_ARRAY(strSelected);

            return;
        }		
        fwrite(FileInfo.pRealData,1,FileInfo.ulSize,file);
        fclose(file);

        FileInfo.pRealData = NULL;
    }

    MessageBox("압축해제 완료");
    SAFE_DELETE_ARRAY(strSelected);
}

void CMFCMassFileDlg::OnOpenkom() 
{
    CString strFilter = "All GCR Files (*.first)|*.first|";

    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

    if( dlg.DoModal() != IDOK )
        return;

    Load(dlg.GetPathName());
}

void CMFCMassFileDlg::UpdateTotalCount()
{
    CString strTotal;
    strTotal.Format("Total Files : %d",m_ListFileName.GetItemCount());
    SetDlgItemText(IDC_FILEINFO, strTotal);
}

void CMFCMassFileDlg::OnDropFiles(HDROP hDropInfo) 
{
    // TODO: Add your message handler code here and/or call default

    char strFilePath[MAX_PATH];

    UINT iCount = DragQueryFile( hDropInfo, 0xFFFFFFFF, strFilePath, MAX_PATH);

    for(int i = 0; i < (int)iCount; i++)
    {
        DragQueryFile( hDropInfo, i, strFilePath, MAX_PATH);
        //if( strcmp( &strFilePath[strlen(strFilePath)-4], ".kom" ) == 0 )
        if(GCMassFile::IsKomFile(strFilePath))
        {
            Load(strFilePath);
        }
        else
        {
            CheckAddDirectory(strFilePath);
        }
    }

    UpdateTotalCount();

    CDialog::OnDropFiles(hDropInfo);
    return;
}
void CMFCMassFileDlg::Load(const char* strFileName)
{
	btnDecompress.EnableWindow(TRUE);
    m_KomFilePath = strFileName;
    m_MassFile.LoadMassFile(strFileName);

    GCMassFile::MASSFILE_MEMBERFILEINFO Info;
    ZeroMemory(&Info,sizeof(Info));

    for(int e = 0; e < m_ListFileName.GetItemCount(); e++)
    {
        delete (CString*)m_ListFileName.GetItemData(e);
    }
    m_ListFileName.DeleteAllItems();

    CString Temp;
    for(int i = 0; i < (int)m_MassFile.GetFileCount(); i++)
    {
        m_MassFile.GetMemberFile(i,&Info);

        m_ListFileName.InsertItem(i,Info.strFileName);
        m_ListFileName.SetItemData(i,(DWORD_PTR)(new CString(Info.strFileName)));
        Temp.Format("%d KB", Info.ulSize/1024);
        m_ListFileName.SetItem(i,1,LVIF_TEXT,Temp,0,0,0,0);
    }
    UpdateTotalCount();
    UpdateData(FALSE);
}

void CMFCMassFileDlg::CheckAddDirectory(const char* strFileName)
{
    CFileFind finder;
    bool bEnd;
    bool isDir = false;
    CString strAll;	
    std::string filePath = strFileName;
    std::string fileName = strFileName;

    //받은 파일명으로부터 경로만 추출
    filePath.replace(filePath.find_last_of("\\",filePath.length()) + 1 , filePath.length(), "");	
    filePath = filePath + "*.*";

    //파라메터로 부터 파일명만 추출
    fileName.replace(0, fileName.find_last_of("\\", fileName.length()) + 1 , "");

    //해당 경로가 디렉토리인지 확인.	
    bEnd = FBOOL(finder.FindFile(&filePath[0]));

    if( fileName.length() != 0 ) //length가 0이면 디렉토리, 0이 아니면 확인 필요
    {
        while(bEnd)
        {
            bEnd = FBOOL(finder.FindNextFile());
            if( strcmp(finder.GetFileName().GetString(), fileName.c_str())==0)
            {
                if( finder.IsDirectory() )
                {
                    isDir = true;
                    break;
                }
                else
                {
                    isDir = false;
                    break;
                }
            }
        }		
    }
    else
    {
        isDir = true;
    }

    if( isDir )
    {
        //디렉토리라면 내부에 있는 파일까지 모두 읽어오기
        strAll = strFileName;
        strAll = strAll + "\\*.*";
        bEnd = FBOOL(finder.FindFile( strAll ));

        while(bEnd)
        {
            bEnd = FBOOL(finder.FindNextFile());
            if( finder.IsDots() )
                continue;

            if( finder.IsDirectory() )
            {
                CheckAddDirectory( finder.GetFilePath() + "\\" );
            }
            else
            {
                AddFile(finder.GetFilePath(), (DWORD)finder.GetLength() );		
            }			
        }
    }
    else
    {
        AddFile( strFileName, m_MassFile.GetFileSize((LPTSTR)(LPCTSTR)strFileName) );			
    }

    finder.Close();
    return;
}


void CMFCMassFileDlg::OnBegindragFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NMLISTVIEW*		pNMLV = (NMLISTVIEW*) pNMHDR;
    COleDataSource	datasrc;
    HGLOBAL			hgDrop;
    DROPFILES*		pDrop;
    CStringList		lsDraggedFiles;
    POSITION		pos;
    int				nSelItem;
    CString			sFile;
    UINT			uBuffSize = 0;
    TCHAR*			pszBuff;
    FORMATETC		etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    bool			bDeleteCheck = false;

    *pResult = 0;   // return value ignored



    // For every selected item in the list, put the filename into lsDraggedFiles.

    pos = m_ListFileName.GetFirstSelectedItemPosition();

    while( NULL != pos )
    {
        nSelItem = m_ListFileName.GetNextSelectedItem ( pos );
        sFile = m_ListFileName.GetItemText ( nSelItem, 0 );

        if( sFile.Find( "C:\\" ) < 0 )
        {
            if( sFile.Find( "D:\\" ) < 0 )
            {
                if( m_KomFilePath != "" )
                {
                    CString csTemp = _T("");
                    csTemp = Decompress( sFile );
                    if( csTemp != "" )
                    {
                        sFile = _T("");
                        sFile = csTemp;
                        bDeleteCheck = true;
                    }
                }
            }
        }

        lsDraggedFiles.AddTail ( sFile );

        uBuffSize += lstrlen ( sFile ) + 1;
    }

    // Add 1 extra for the final null char, and the size of the DROPFILES struct.

    uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

    // Allocate memory from the heap for the DROPFILES struct.
    hgDrop = GlobalAlloc ( GHND | GMEM_SHARE, uBuffSize );

    if ( NULL == hgDrop )
        return;

    pDrop = (DROPFILES*) GlobalLock ( hgDrop );

    if ( NULL == pDrop )
    {
        GlobalFree ( hgDrop );
        return;
    }

    // Fill in the DROPFILES struct.

    pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
    // If we're compiling for Unicode, set the Unicode flag in the struct to
    // indicate it contains Unicode strings.

    pDrop->fWide = TRUE;
#endif;

    // Copy all the filenames into memory after the end of the DROPFILES struct.
    pos = lsDraggedFiles.GetHeadPosition();
    pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));

    while ( NULL != pos )
    {
        lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
        pszBuff = 1 + _tcschr ( pszBuff, '\0' );
    }

    GlobalUnlock ( hgDrop );

    // Put the data in the data source.

    datasrc.CacheGlobalData ( CF_HDROP, hgDrop, &etc );

    // Add in our own custom data, so we know that the drag originated from our 
    // window.  CMyDropTarget::DragEnter() checks for this custom format, and
    // doesn't allow the drop if it's present.  This is how we prevent the user
    // from dragging and then dropping in our own window.
    // The data will just be a dummy bool.
    HGLOBAL hgBool;

    hgBool = GlobalAlloc ( GHND | GMEM_SHARE, sizeof(bool) );

    if ( NULL == hgBool )
    {
        GlobalFree ( hgDrop );
        return;
    }

    // Put the data in the data source.

    etc.cfFormat = g_uCustomClipbrdFormat;

    datasrc.CacheGlobalData ( g_uCustomClipbrdFormat, hgBool, &etc );


    // Start the drag 'n' drop!

    DROPEFFECT dwEffect = DROPEFFECT_MOVE;
    if( bDeleteCheck )
    {
        dwEffect = datasrc.DoDragDrop ( DROPEFFECT_MOVE );
    }
    else
    {
        dwEffect = datasrc.DoDragDrop ( DROPEFFECT_COPY );// | DROPEFFECT_MOVE );
    }

    // If the DnD completed OK, we remove all of the dragged items from our
    // list.

    switch ( dwEffect )
    {
    case DROPEFFECT_COPY:
        break;
    case DROPEFFECT_MOVE:
        {
            // Note: Don't call GlobalFree() because the data will be freed by the drop target.

            if( bDeleteCheck == false )
            {
                for ( nSelItem = m_ListFileName.GetNextItem ( -1, LVNI_SELECTED );
                    nSelItem != -1;
                    nSelItem = m_ListFileName.GetNextItem ( nSelItem, LVNI_SELECTED ) )
                {
                    m_ListFileName.DeleteItem ( nSelItem );
                    nSelItem--;
                }

                // Resize the list columns.

                m_ListFileName.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
                m_ListFileName.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
                m_ListFileName.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
            }
        }
        break;

    case DROPEFFECT_NONE:
        {
            // This needs special handling, because on NT, DROPEFFECT_NONE
            // is returned for move operations, instead of DROPEFFECT_MOVE.
            // See Q182219 for the details.
            // So if we're on NT, we check each selected item, and if the
            // file no longer exists, it was moved successfully and we can
            // remove it from the list.

            if ( g_bNT )
            {
                bool bDeletedAnything = true; /* false; */
                /*
                for ( nSelItem = m_ListFileName.GetNextItem ( -1, LVNI_SELECTED );
                nSelItem != -1;
                nSelItem = m_ListFileName.GetNextItem ( nSelItem, LVNI_SELECTED ) )
                {
                CString sFilename = m_ListFileName.GetItemText ( nSelItem, 0 );

                if ( 0xFFFFFFFF == GetFileAttributes ( sFile ) &&
                GetLastError() == ERROR_FILE_NOT_FOUND )
                {
                // We couldn't read the file's attributes, and GetLastError()
                // says the file doesn't exist, so remove the corresponding 
                // item from the list.

                m_ListFileName.DeleteItem ( nSelItem );

                nSelItem--;
                bDeletedAnything = true;
                }
                }
                */

                // Resize the list columns if we deleted any items.

                if ( bDeletedAnything )
                {
                    /*
                    m_ListFileName.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
                    m_ListFileName.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
                    m_ListFileName.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
                    */

                    // Note: Don't call GlobalFree() because the data belongs to 
                    // the caller.
                }
                else
                {
                    // The DnD operation wasn't accepted, or was canceled, so we 
                    // should call GlobalFree() to clean up.

                    GlobalFree ( hgDrop );
                    GlobalFree ( hgBool );
                }
            }   // end if (NT)
            else
            {
                // We're on 9x, and a return of DROPEFFECT_NONE always means
                // that the DnD operation was aborted.  We need to free the
                // allocated memory.

                GlobalFree ( hgDrop );
                GlobalFree ( hgBool );
            }
        }
        break;  // end case DROPEFFECT_NONE
    }   // end switch
}

CString CMFCMassFileDlg::Decompress( CString& csComFile )
{
    CString OutFileName;
    GCMassFile::MASSFILE_MEMBERFILEINFO FileInfo;	

    if( m_ListFileName.GetSelectedCount() != 0 )
    {
        m_MassFile.GetMemberFile((LPCTSTR)csComFile,&FileInfo);		
        char dir[MAX_PATH] = _T("");
        GetCurrentDirectory( MAX_PATH, dir );
        OutFileName.Format("%s\\%s",dir, FileInfo.strFileName);
        FILE* file = fopen(OutFileName,"wb");
        if(file == NULL)
        {	
            MessageBox("압축해제 실패");				
            //SAFE_DELETE_ARRAY(strSelected);
            return _T("");
        }		
        fwrite(FileInfo.pRealData,1,FileInfo.ulSize,file);
        fclose(file);
    }

    return OutFileName;
}


void CMFCMassFileDlg::OnDeleteall() 
{
    for(int e = 0; e < m_ListFileName.GetItemCount(); e++)
    {
        delete (CString*)m_ListFileName.GetItemData(e);
    }
    m_ListFileName.DeleteAllItems();
    UpdateTotalCount();
}

void CMFCMassFileDlg::OnButton1() 
{

}

void CMFCMassFileDlg::OnButton2() 
{
    // TODO: Add your control notification handler code here


}

void CMFCMassFileDlg::OnButton3() 
{
    // TODO: Add your control notification handler code here

}

void CMFCMassFileDlg::OnLvnItemchangedFilename(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    *pResult = 0;
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,  LPARAM lParamSort)
{
    CString strItem1 = *((CString*)lParam1);
    CString strItem2 = *((CString*)lParam2);

    if( !CMFCMassFileDlg::s_bSortReverse )
        return strcmp(strItem2, strItem1);
    return -1 * strcmp(strItem2, strItem1);
}


void CMFCMassFileDlg::OnLvnColumnclickFilename(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    m_ListFileName.SortItems(CompareFunc, 0);	
    s_bSortReverse = !s_bSortReverse;

    *pResult = 0;
}

CString CMFCMassFileDlg::ExecuteConsoleProgram(CString strExec)
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hChildStdoutRd, hChildStdoutWr, hChildStdoutRdDup, hSaveStdout;

    STARTUPINFO siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(siStartInfo);

    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(piProcInfo));

    DWORD dwRead;

    //DWORD dwProcRetCode;
#define MAXBUFSIZE (3000)

    char chBuf[MAXBUFSIZE];

    CString strOutPut;

    if (!AllocConsole())
    {
        MessageBox("Console allocation failed");
        return "";
    }

    hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
    {
        MessageBox("Stdout pipe creation failed");
        return "";
    }

    if (!SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
    {
        MessageBox("Redirecting STDOUT failed");
        return "";
    }

    if (!DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
        GetCurrentProcess(), &hChildStdoutRdDup , 0,
        FALSE, DUPLICATE_SAME_ACCESS))
    {
        MessageBox("DuplicateHandle failed");
        return "";
    }
    CloseHandle(hChildStdoutRd);

    if (!CreateProcess(NULL, (LPTSTR)(LPCTSTR)strExec, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo))
    {
        MessageBox("Failed to Create New Process for ds1 Editor");
        return "";
    }

    if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
    {
        MessageBox("Re-redirecting Stdout failed\n");
        return "";
    }

    if (!CloseHandle(hChildStdoutWr))
    {
        MessageBox("Closing handle failed");
        return "";
    }

    while (WaitForSingleObject(piProcInfo.hProcess, 0) != WAIT_OBJECT_0)
    {
        if(!ReadFile(hChildStdoutRdDup, chBuf, MAXBUFSIZE-1, &dwRead, NULL) || dwRead == 0)
            break;
        chBuf[dwRead] = 0;
        strOutPut += chBuf;
    }
    /*GetExitCodeProcess(piProcInfo.hProcess, &dwProcRetCode);

    if (dwProcRetCode)
    {
    char ErrorMsg[100];
    sprintf(ErrorMsg, "Editor's process Exited with code %d", dwProcRetCode);
    GLBLERRORMSGBOX(ErrorMsg);
    }*/

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    FreeConsole();
    return strOutPut;
}

void CMFCMassFileDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    OnDeleteall();
}
void CMFCMassFileDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
}
