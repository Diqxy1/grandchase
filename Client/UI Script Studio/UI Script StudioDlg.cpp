// UI Script StudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include <d3d9.h>
//#include <strsafe.h>
#include <d3dx9tex.h>
#include "UI Script Studio.h"
#include "UI Script StudioDlg.h"
#include ".\ui script studiodlg.h"
#include "TextureViewDlg.h"
#include "KWndManager.h"
#include "KEditDialog.h"
#include "KViewTextureDialog.h"
#include "KD3DSizingBox.h"
#include "../GCUTIL/GCUtil.h"
#include "KUIBringingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_STRING_SIZE 100
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DTEXTURE9      g_pTexture   = NULL; // Our texture
LPD3DXSPRITE			g_pSprite	 = NULL;

KWndManager				*g_WndMgr	 = NULL;
KD3DWndClass			*g_pkWndClass = NULL;
KD3DWndClass            *g_pkSelectWnd = NULL;
CMagnifiCationDlg       *g_Magnification = NULL;
//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{

    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pd3dDevice != NULL) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL)
        g_pD3D->Release();

    if( g_pTexture != NULL )
        g_pTexture->Release();

    if( g_pSprite != NULL )
        g_pSprite->Release();

    //if( g_pkWndClass != NULL )
    //	delete g_pkWndClass;
}

CUIScriptStudioDlg::CUIScriptStudioDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUIScriptStudioDlg::IDD, pParent)
, m_pTextureDlg(NULL)	
, m_pFindUIDlg(NULL)
, m_strFilePath(_T(""))
{
    //{{AFX_DATA_INIT(CUIScriptStudioDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bShiftState = false;
    m_bCtrl		  = false;
    m_bDrag		  = false;
    m_bDragging   = false;
    m_bCaps		  = false;

    D3DXMatrixIdentity(&m_matTrans);
    m_pointStart.SetPoint(0,0);
    m_rectSelect.SetRect(0,0,0,0);
    m_bModeTest   = false;

    m_pTestMode	  = NULL;
    m_iState	  = -1;

    m_iRvalue = 0;
    m_iGvalue = 102;
    m_iBvalue = 0;

    m_TextureEditStatic = NULL;
    m_pInsertTextureDlg = NULL;

    m_bSizeControl = false;
    m_bSetSize	   = false;

    //////////////////////////////////////////////////////////////////////////
    /*20091009 이동석*/
    //	추가한 변수들 초기화.
    m_bEditSubItemActive	=	false;
    m_bViewOption			=	false;

    m_iSelectItem			=	-1;
    m_iCurSelectedItem		=	-1;

    m_iEditSubItemSelPos	=	0;
    //////////////////////////////////////////////////////////////////////////

    clearHistory();
}

void CUIScriptStudioDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUIScriptStudioDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_IMAGERECT, m_ImageRect);
    DDX_Control(pDX, IDC_CLASSTREEVIEW, m_CtlTreeView);
    DDX_Control(pDX, IDC_LISTCHILD, m_ListCtrl);
    DDX_Control(pDX, IDC_WINDOW_DATA, m_CtrlWindowData);
}

BEGIN_MESSAGE_MAP(CUIScriptStudioDlg, CDialog)
    //{{AFX_MSG_MAP(CUIScriptStudioDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_DESTROY()
    ON_COMMAND(ID_FILEOPEN, OnFileopen)
    ON_WM_CLOSE()
    ON_COMMAND(ID_FILESAVE, OnFilesave)
    //	ON_UPDATE_COMMAND_UI(ID_CHKVIEW, OnUpdateChkview)
    ON_UPDATE_COMMAND_UI(ID_MAGNIFCATION, OnUpdateMagnification)
    ON_COMMAND(ID_MAGNIFICATION, OnMagnification)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW, OnTvnSelchangedTreeview)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCHILD, OnLvnItemchangedListchild)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_WINDOW_DATA, OnLvnItemActivateWindowData)
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONDOWN()   
    ON_NOTIFY(NM_RCLICK, IDC_TREEVIEW, OnNMRclickTreeview)
    ON_COMMAND(ID_TREE_ADD, OnTreeAdd)
    ON_COMMAND(ID_TREE_DELETE, OnTreeDelete)
    ON_COMMAND(ID_TREE_FIND, OnTreeFind)
    ON_COMMAND(ID_TREE_COPY, OnTreeCopy)
    ON_COMMAND(ID_TREE_PASTE, OnTreePaste)
    ON_COMMAND(ID_TREE_VIEW_TEXTURE, OnTreeViewTexture)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(ID_BACK, OnBack)
    ON_COMMAND(ID_FORTH, OnForth)
    ON_COMMAND(ID_LAST, OnLast)
    ON_COMMAND(ID_FIRST, OnFirst)
    ON_COMMAND(ID_MODETEST, OnModetest)
    ON_NOTIFY(NM_RCLICK, IDC_LISTCHILD, OnNMRclickListchild)
    ON_COMMAND(ID_LISTVIEW_SET_FIND, OnListViewSetFind )
    ON_COMMAND(ID_LISTVIEW_SET_VISIBLE, OnListviewSetVisible)
    ON_COMMAND(ID_LISTVIEW_SET_INVISIBLE, OnListviewSetInvisible)
    ON_COMMAND(ID_LISTVIEW_SELECT_ALL, OnListviewSelectAll)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LISTCHILD, OnLvnItemActivateListchild)
    ON_COMMAND(ID_END, OnEnd)
    ON_COMMAND(ID_LISTVIEW_BACK, OnListviewBack)
    ON_COMMAND(ID_LISTVIEW_FORTH, OnListviewForth)
    ON_COMMAND(ID_LISTVIEW_BOTTOM, OnListviewBottom)
    ON_COMMAND(ID_LISTVIEW_TOP, OnListviewTop)
    ON_COMMAND(ID_CHANGE, OnChange)
    ON_WM_CREATE()
    ON_COMMAND(ID_SAMEPOSITION, OnSameposition)
    ON_COMMAND(ID_SAMEPOSITION_ALL, OnSamePosition_All)
    ON_COMMAND(ID_INSERTTEXTURE, OnInserttexture)
    ON_COMMAND(ID_MENU_ALL_VIEW_TEXTURE, OnAllViewTexture)
    ON_COMMAND(ID_MENU_ALL_FILE_SAVE, OnAllFileSave)
    ON_STN_CLICKED(IDC_IMAGERECT, OnStnClickedImagerect)
    ON_COMMAND(ID_PREREG_FILE, OnPreregFile)
    ON_COMMAND(ID_TEXTURE_DEF, OnTextureDef)
    ON_COMMAND(ID_TEXTURE_HOV, OnTextureHov)
    ON_COMMAND(ID_TEXTURE_SEL1, OnTextureSel)
    ON_COMMAND(ID_TEXTURE_ACT, OnTextureAct)
    ON_COMMAND(ID_TEXTURE_LOCK, OnTextureLock)
    ON_COMMAND(ID_Z_ODER_BACK, OnBack)
    ON_COMMAND(ID_Z_ODER_FORTH, OnForth)
    ON_NOTIFY(NM_CLICK, IDC_WINDOW_DATA, &CUIScriptStudioDlg::OnNMClickWindowData)
    ON_COMMAND(ID_Z_VIEWOPTION, &CUIScriptStudioDlg::OnZViewoption)
    //	ON_NOTIFY(NM_KILLFOCUS, IDC_WINDOW_DATA, &CUIScriptStudioDlg::OnNMKillfocusWindowData)
    ON_NOTIFY(NM_CLICK, IDC_LISTCHILD, &CUIScriptStudioDlg::OnNMClickListchild)
    ON_NOTIFY(NM_CLICK, IDC_TREEVIEW, &CUIScriptStudioDlg::OnNMClickTreeview)
    ON_COMMAND(ID_BRINGING, &CUIScriptStudioDlg::OnBringing)
    ON_UPDATE_COMMAND_UI(ID_BRINGING, &CUIScriptStudioDlg::OnUpdateBringing)
    ON_COMMAND(ID_SAVE_SCENE, &CUIScriptStudioDlg::OnSaveScene)
    ON_COMMAND(ID_FILE_SAVE_AS, &CUIScriptStudioDlg::OnFileSaveAs)
    ON_COMMAND(ID_FILE_SAVE_AS_SORT, &CUIScriptStudioDlg::OnFileSaveAsSort)
    //2013   7    12 추가
    ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCHILD, &CUIScriptStudioDlg::OnLvnBegindragListchild)//childlist안에서 항목간 드래그 이벤트 구현
    ON_COMMAND(ID_TREE_UI32898,&CUIScriptStudioDlg::OnTreeUi32898)//tree리스트에서 오른쪽 팝업창중 부모 크기 조정 기능 구현
    ON_COMMAND(ID_TREE_UI32896,&CUIScriptStudioDlg::OnTreeUi32896)//전체 자식의 크기에 맞추어 크기 셋팅
    ON_COMMAND(ID_RANGE_LEFT,&CUIScriptStudioDlg::OnSetCurrentTexturePositionLeft)
    ON_COMMAND(ID_RANGE_RIGHT,&CUIScriptStudioDlg::OnSetCurrentTexturePositionRight)
    ON_COMMAND(ID_RANGE_UP,&CUIScriptStudioDlg::OnSetCurrentTexturePositionUp)
    ON_COMMAND(ID_RANGE_DOWN,&CUIScriptStudioDlg::OnSetCurrentTexturePositionDown)
    ON_COMMAND(ID_RANGE_LRMiddle,&CUIScriptStudioDlg::OnSetCurrentTexturePositionLRMiddle)
    ON_COMMAND(ID_RANGE_UDMiddle,&CUIScriptStudioDlg::OnSetCurrentTexturePositionUDMiddle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIScriptStudioDlg message handlers

//------------------------------------------------------------------------
//	NAME : MemoryLeakCheck. 메모리 누수 점검
//	DESC : 디버그 모드일때 새는 메모리를 점검해준다.
//	DATE : 20091021
//------------------------------------------------------------------------
#ifdef _DEBUG
class MemoryLeakCheck
{
public:
    MemoryLeakCheck()
    {
        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
        //_CrtSetBreakAlloc( 5099/*멈출 메모리 번지 주소*/ ); 
    }
};

MemoryLeakCheck leakCheck;

#endif
//========================================================================



BOOL CUIScriptStudioDlg::OnInitDialog()
{
#ifdef _DEBUG
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    // TODO: Add extra initialization here

    /////////////////////
    //ImageShow Size 800 * 600
    m_ImageRect.SetWindowPos(NULL,0,0,GC_SCREEN_WIDTH,GC_SCREEN_HEIGHT,SWP_NOMOVE|SWP_NOZORDER);

    InitD3D( m_ImageRect.GetSafeHwnd() );

    D3DXCreateTextureFromFile( g_pd3dDevice, _T("control.jpg"), &m_TextureEditStatic );

    m_pSelectedWnd	= NULL;

    D3DXCreateSprite(g_pd3dDevice,&g_pSprite);

    SAFE_DELETE(m_pTextureDlg);
    SAFE_DELETE(g_Magnification);
    SAFE_DELETE(m_pFindUIDlg);
    SAFE_DELETE(g_WndMgr);

    g_WndMgr = new KWndManager();
    g_WndMgr->Init();

    m_pDataPackage = new KWndDataPackage();


    m_CtrlWindowData.SetExtendedStyle( LVS_LIST | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_ListCtrl.SetExtendedStyle(LVS_EX_CHECKBOXES);

    m_CtrlWindowData.InsertColumn(0, _T("Name"));
    m_CtrlWindowData.InsertColumn(1,_T("Value"));
    m_CtrlWindowData.SetColumnWidth(0,100);
    m_CtrlWindowData.SetColumnWidth(1,400);

    m_pTextureDlg = new CTextureViewDlg();
    g_Magnification = new CMagnifiCationDlg();

    m_pTextureDlg->Create(IDD_TEXTUREVIEW,this);
    g_Magnification->Create(IDD_Magnification, this);
    //m_pTextureDlg->ShowWindow(SW_HIDE);

    m_hAccel = LoadAccelerators( AfxGetInstanceHandle() , MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );

    m_LoadFileInfoWindow.GetFromRegister();

    //ADDBOXTYPE( gc_sizingbox );

    int iTopEdge= GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYEDGE );
    int iLeftEdge = GetSystemMetrics( SM_CXEDGE );
    g_Magnification->SetEdgeValue(iTopEdge, iLeftEdge);

    m_bIsLoaded = FALSE;	// 파일을 불러온 상태냐?!

    m_toolbar = new CToolBar();
    DWORD style = WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS;

    style |= (CBRS_ALIGN_TOP & CBRS_ALIGN_ANY);

    m_toolbar->CreateEx(this,TBSTYLE_FLAT,style);

    m_toolbar->LoadToolBar(IDR_TOOLBAR2);

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUIScriptStudioDlg::DrawRectangle()
{
    CRect gap(0,0,0,0);
    ClientToScreen(&gap);
    m_ImageRect.ScreenToClient(&gap);

    CRect rect;
    if( m_matTrans._41 > 0 )
    {
        rect.left = m_matTrans._41-gap.left;
        rect.right = (-gap.left)+GC_SCREEN_WIDTH;
    }
    else
    {
        rect.left = -gap.left;
        rect.right = m_matTrans._41-gap.left+GC_SCREEN_WIDTH;
    }
    if( m_matTrans._42 > 0 )
    {
        rect.top = m_matTrans._42-gap.top;
        rect.bottom = -gap.top+GC_SCREEN_HEIGHT;
    }
    else
    {
        rect.top = -gap.top;
        rect.bottom = m_matTrans._42-gap.top+GC_SCREEN_HEIGHT;
    }

    if( (rect.left == -gap.left) && (rect.top == -gap.top) )
        return;

    CDC* pdc = GetDC();
    CBrush NullBrush;
    NullBrush.CreateStockObject(NULL_BRUSH);

    CBrush *pOldBr = pdc->SelectObject(&NullBrush);

    pdc->Rectangle(rect);
    pdc->SelectObject( pOldBr );
    NullBrush.DeleteObject();	
    ReleaseDC(pdc);	
}

void CUIScriptStudioDlg::OnPaint()
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
        Render();
        DrawRectangle();
        DrawSelectRect();
        if( m_bSizeControl == true )
        {	drawSizeControl();   }

        CDialog::OnPaint();
    }
}

HCURSOR CUIScriptStudioDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CUIScriptStudioDlg::OnDestroy()
{
    CDialog::OnDestroy();
    Cleanup();

    if( m_pTextureDlg )
    {
        m_pTextureDlg->DestroyWindow();
        delete m_pTextureDlg;
    }

    /*if( m_pMagnificationDlg )
    {
    m_pMagnificationDlg->DestroyWindow();
    delete m_pMagnificationDlg;
    }*/
    if( m_pFindUIDlg )
    {
        m_pFindUIDlg->DestroyWindow();
        delete m_pFindUIDlg;
    }

    if( m_pDataPackage )
        delete m_pDataPackage;

    if( m_TextureEditStatic != NULL )
        m_TextureEditStatic->Release();

    if(m_toolbar){
        m_toolbar->DestroyWindow();
        delete m_toolbar;
    }

    SAFE_DELETE(g_Magnification);
    SAFE_DELETE( g_WndMgr );
    SAFE_DELETE( m_pInsertTextureDlg );
} 

void CUIScriptStudioDlg::SettingView(void)
{
    InsertChild(g_WndMgr->m_pWndRoot,NULL);
}

void CUIScriptStudioDlg::InsertChild(KD3DWndClass* pkWnd,HTREEITEM hParent)
{
    hParent = m_CtlTreeView.InsertItem(pkWnd->GetWndName().c_str(),NULL,NULL,hParent);
    m_CtlTreeView.SetItemData(hParent,(DWORD)pkWnd);

    for( UINT i = 0 ; i < pkWnd->GetChildContainer().size() ; i++ )
    {
        InsertChild(pkWnd->GetChild(i), hParent);
    }
}

void CUIScriptStudioDlg::OnFileopen()
{
    if( IDNO == MessageBox( _T("파일을 다시 불러 오겠습니까? 이전 정보는 저장하지 않습니다. 계속하시겠습니까?"), _T("알림"), MB_YESNO ) )
        return;

    static bool isfirst = true;

    if( !isfirst )
    {
        m_CtrlWindowData.DeleteAllItems();
        m_ListCtrl.DeleteAllItems();
        m_CtlTreeView.DeleteAllItems();

        SAFE_DELETE(m_pDataPackage);

        //	D3DXMemoryLeak
        if( m_pTextureDlg )
            m_pTextureDlg->DestroyWindow();
        SAFE_DELETE(m_pTextureDlg);
        g_pkWndClass = NULL;

        if( m_pFindUIDlg )
            m_pFindUIDlg->DestroyWindow();
        SAFE_DELETE(m_pFindUIDlg);

        //------------------------------------------------------------------------
        //	DESC : 윈도우 메니저를 다시 생성하기 전에 클립보드 내용을 보존한다.
        //	DATE : 20091021 이동석
        KD3DWndClass*	pTempClipWindow = 0;
        if( g_WndMgr->GetClipboardPointer() )
        {
            pTempClipWindow	= new KD3DWndClass;
            pTempClipWindow->DeepCopy( g_WndMgr->GetClipboardPointer() );
        }
        //------------------------------------------------------------------------

        SAFE_DELETE(g_WndMgr);


        //SAFE_DELETE(m_pTextureDlg);

        g_WndMgr = new KWndManager();
        g_WndMgr->Init();

        //------------------------------------------------------------------------
        //	DESC : 윈도우 메니저생성이 끝났으므로 다시 윈도우 메니저에 클립보드내용을 세팅
        g_WndMgr->SetClipboardPointer( pTempClipWindow );
        //------------------------------------------------------------------------

        m_pDataPackage = new KWndDataPackage();
        m_pTextureDlg = new CTextureViewDlg();
        m_pTextureDlg->Create(IDD_TEXTUREVIEW,this);

    }
    isfirst = false;

    int nCount = 0;

    if( m_LoadFileInfoWindow.GetTextureListCnt() <= 0 )
    {
        MessageBox( _T("로드할 텍스쳐 리스트가 없습니다."), _T("Error"), MB_OK);
        return;
    }

    if( m_LoadFileInfoWindow.GetScriptListCnt() <= 0 )
    {
        MessageBox(_T("로드할 스크립트 리스트가 없습니다."), _T("Error"), MB_OK);
        return;
    }

    for(int i=0;i<m_LoadFileInfoWindow.GetTextureListCnt();++i)
    {
        std::string& strFileOrFolder = m_LoadFileInfoWindow.GetTextureFromIndex(i);
        if( strFileOrFolder.substr( strFileOrFolder.size() - 4, 4 ).compare( ".kom" ) == 0 )
        {
            g_WndMgr->GetMassFileManager()->AddMassFile( strFileOrFolder );
        }
        else
        {
            g_WndMgr->GetMassFileManager()->AddDirectory( strFileOrFolder);
        }		
    }

    CString FilePath = m_LoadFileInfoWindow.GetScriptFromIndex(0).c_str();

    FilePath.Replace('\\', '/');

    CString strTempFilePath = FilePath.Right(4);

    if( FilePath.Right(4).CompareNoCase(_T(".stg")) )
    {
        CStdioFile file;
        file.Open(FilePath, CFile::modeRead);
        CString temp;
        CStringArray filename; 

        while( file.ReadString(temp) )
        {
            filename.Add(temp);
        }

        temp = filename[0];
        m_strFilePath = temp.Left(temp.ReverseFind('/')+1);

        file.Close();

        for(INT_PTR i = 0 ; i < filename.GetSize(); i++)
        {
            g_WndMgr->LoadLua( ToCharStr( filename[i].GetBuffer() ));
        }
    }

    else
    {
        g_WndMgr->LoadLua( ToCharStr( FilePath.GetBuffer() ) );
        m_strFilePath = FilePath.Left(FilePath.ReverseFind('/')+1);
    }


    if ( g_WndMgr->LoadUIScript() != S_OK )
        AfxMessageBox( _T( "Texture 파일을 찾을 수 없거나 스크립트 에러 입니다." ));

    m_SceneFileDlg.m_vecUIScriptFile = g_WndMgr->m_vecUIScriptFile;
    m_SceneFileDlg.m_strFilePath = m_strFilePath;

    if( IDOK == m_SceneFileDlg.DoModal() )
    {
        // 파일을 불러온 상태가 아니면 다른파일에서 가져오기 기능을 사용할 수 없음
        m_bIsLoaded = TRUE;

        //g_WndMgr->DataMapping();
        m_strFileNameWithPath = m_strFilePath + m_SceneFileDlg.m_strSelectedFile; // 파일패스 + 파일이름

        m_pTextureDlg->GetTextureFilePath(m_strFilePath);
        for( UINT i = 0; i < g_WndMgr->m_vecStrTexFileName.size(); ++i )
        {
            m_pTextureDlg->AddFileName( g_WndMgr->m_vecStrTexFileName[i].c_str() );
        }
        m_pTextureDlg->LoadTexture();

        InsertChild( g_WndMgr->m_pWndRoot, NULL );

        D3DXMatrixIdentity(&m_matTrans);

        clearHistory();

        OnPaint();
    }
}

void CUIScriptStudioDlg::OnFilesave()
{
    // 불러온 파일 아니라면 새로 저장하기
    if( m_strFileNameWithPath.IsEmpty() ) {
        OnFileSaveAs();
    } else {
        // 현재 파일에 저장
        g_WndMgr->SaveToFile( ToCharStr( m_strFileNameWithPath.GetBuffer() ));

        CString Msg = m_strFileNameWithPath + _T("\n저장 완료~!!");
        AfxMessageBox( Msg, MB_ICONASTERISK );
    }
}

void CUIScriptStudioDlg::OnMagnification()
{
    if(g_Magnification->ShowWindow(SW_SHOW))
    {
        g_Magnification->ShowWindow( SW_HIDE );
        g_Magnification->SetRenderState( false );

    }
    else
    {
        g_Magnification->ShowWindow(SW_SHOW);
        g_Magnification->SetRenderState( true );
        g_Magnification->SetZoom(40);
    }
}
void CUIScriptStudioDlg::OnUpdateChkview(CCmdUI *pCmdUI)
{
    /*if( m_strFilePath == "" )
    {
    MessageBox("Script File A≫ ¸OAu ¿­°i, Texture View¸| º¸¼¼¿a", "°æ°i!" );
    return;
    }*/
    CMenu* p_menu = GetMenu();

    if( m_pTextureDlg->ShowWindow(SW_SHOW) )
    {
        m_pTextureDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        m_pTextureDlg->ShowWindow(SW_SHOW);
    }	
}

void CUIScriptStudioDlg::OnUpdateMagnification(CCmdUI *pCmdUI)
{
    CMenu* p_menu = GetMenu();

    if(g_Magnification->ShowWindow(SW_SHOW))
        g_Magnification->ShowWindow( SW_HIDE );
    else
        g_Magnification->ShowWindow(SW_SHOW);

}
void CUIScriptStudioDlg::OnTvnSelchangedTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    *pResult = 0;

    UpdateCtrl();

    if( m_pTextureDlg && !GetAsyncKeyState( VK_LCONTROL ) )
    {
        if( m_CtlTreeView.GetSelectedItem() )
        {
            m_pSelectedWnd = (KD3DWndClass*)m_CtlTreeView.GetItemData(m_CtlTreeView.GetSelectedItem());
            m_pTextureDlg->SetSelectedWindowTextrueRect( m_pSelectedWnd );
        }
    }
}

void CUIScriptStudioDlg::Render(void)
{
    if(g_pkWndClass == NULL)
        return;
    if( NULL == g_pd3dDevice )
        return;

    m_bNowRendering = TRUE;

    // Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue,m_iGvalue,m_iBvalue), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        RenderWindow(g_pkWndClass);
        RenderModeTest(m_pTestMode, m_iState, m_iPos_x, m_iPos_y);

        g_pd3dDevice->EndScene();
    }
    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

    m_bNowRendering = FALSE;
}

void CUIScriptStudioDlg::RenderWindow(KD3DWndClass* pkWndClass, int x, int y, bool bSelect)
{
    if( !pkWndClass->IsVisible() )
        return;

    DWORD width = pkWndClass->GetWidth(), height = pkWndClass->GetHeight();
    float def_tu1 = pkWndClass->GetDefTexCoords().x;
    float def_tv1 = pkWndClass->GetDefTexCoords().y;
    float def_tu2 = pkWndClass->GetDefTexCoords().z;
    float def_tv2 = pkWndClass->GetDefTexCoords().w;
    float window_x = pkWndClass->GetWindowPos().x + x, window_y = pkWndClass->GetWindowPos().y + y;
    int idefTexture = pkWndClass->GetDefTextureIDx();

    if( pkWndClass->GetWndType().compare("edit") == 0 )
    {
        def_tu1 = 0;
        def_tv2 = 0;
        def_tu2 = 64;
        def_tv2 = 36;
    }
    else if( pkWndClass->GetWndType().compare("static") == 0 )
    {
        def_tu1 = 66;
        def_tv1 = 0;
        def_tu2 = 130;
        def_tv2 = 36;
    }
    else if( pkWndClass->GetWndType().compare("listbox") == 0 )
    {
        def_tu1 = 132;
        def_tv1 = 0;
        def_tu2 = 196;
        def_tv2 = 36;
    }

    float tex_width = 1.0f;
    float tex_height = 1.0f;

    D3DXMATRIX mat, matRot;
    D3DXMatrixIdentity(&mat);
    D3DXMatrixIdentity(&matRot);

    float X = window_x;
    float Y = window_y;

    if( (def_tu1 < def_tu2) && (def_tv1 > def_tv2))
    {
        D3DXMatrixRotationZ(&matRot,D3DXToRadian(90.0f));
        X = cos(D3DXToRadian(-90.0f))*(window_x) - sin(D3DXToRadian(-90.0f))*(window_y);
        Y = sin(D3DXToRadian(-90.0f))*(window_x) + cos(D3DXToRadian(-90.0f))*(window_y);

        tex_width = def_tv1 - def_tv2;
        tex_height = def_tu2 - def_tu1;
    }
    else if( (def_tu1 > def_tu2) && ( def_tv1 < def_tv2))
    {
        D3DXMatrixRotationZ(&matRot,D3DXToRadian(-90.0f));
        X = cos(D3DXToRadian(90.0f))*(window_x) - sin(D3DXToRadian(90.0f))*(window_y);
        Y = sin(D3DXToRadian(90.0f))*(window_x) + cos(D3DXToRadian(90.0f))*(window_y);

        tex_width = def_tv2 - def_tv1;
        tex_height = def_tu1 - def_tu2;
    }
    else
    {
        tex_width = def_tu2-def_tu1;
        tex_height = def_tv2-def_tv1;
    }

    float scaling_x = ((float)width )/(tex_width);
    float scaling_y = ((float)height )/(tex_height);

    D3DXMatrixScaling(&mat,scaling_x,scaling_y,1.0f);

    mat = mat * matRot * m_matTrans;
    //////////
    D3DXVECTOR3 pos(X/scaling_x,Y/scaling_y,0);

    //D3DXSPRITE_ALPHABLEND
    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND );  

    CRect rect;
    rect.SetRect((int)def_tu1,(int)def_tv1,(int)def_tu2,(int)def_tv2);

    g_pSprite->SetTransform(&mat);

    if((pkWndClass->GetWndType().compare("edit") == 0) || (pkWndClass->GetWndType().compare("static") == 0 ) || (pkWndClass->GetWndType().compare("listbox") == 0 ) )
    {
        g_pSprite->Draw( m_TextureEditStatic ,&rect,NULL,&pos,0xffffffff);
    }
    else if( g_WndMgr->m_mapTexture.end() != g_WndMgr->m_mapTexture.find(idefTexture) )
    {
        //	이동석, 선택된 차일드는 반투명 표시
        if( bSelect && m_bViewOption )
            g_pSprite->Draw( g_WndMgr->m_mapTexture[idefTexture] ,&rect,NULL,&pos,0x40ffffff);
        else
            g_pSprite->Draw( g_WndMgr->m_mapTexture[idefTexture] ,&rect,NULL,&pos,0xffffffff);
    }

    g_pSprite->End();

    for(UINT i = 0; i < pkWndClass->GetChildNum(); ++i )
    {
        //	이동석, 선택된 차일드는 true를 집어넣는다.
        bool bSelectThis = ( pkWndClass->GetChild(i) == m_pSelectedWnd || bSelect ) ? true : false;
        RenderWindow(pkWndClass->GetChild(i),(int)window_x,(int)window_y, bSelectThis );
    }
}

void CUIScriptStudioDlg::OnLvnItemchangedListchild(NMHDR *pNMHDR, LRESULT *pResult)
{

    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    if(m_bNowLoading)
        return;
    /*for(UINT i = 0 ; i < g_pkWndClass->GetChildNum() ; i++)
    g_pkWndClass->GetChild(i)->bVisible = m_ListCtrl.GetCheck(i);*/
    for( UINT i = 0; i < m_ListCtrl.GetItemCount(); i++ )
    {
        ( (KD3DWndClass*)m_ListCtrl.GetItemData( i ) )->SetVisible( m_ListCtrl.GetCheck(i) );
    }

    m_pSelectedWnd = (KD3DWndClass*)m_ListCtrl.GetItemData(pNMIA->iItem);

    CRect gap(0,0,0,0);
    ClientToScreen(&gap);
    m_ImageRect.ScreenToClient(&gap);
    float x = g_pkWndClass->GetWindowPos().x-gap.left + m_matTrans._41;
    float y = g_pkWndClass->GetWindowPos().y-gap.top + m_matTrans._42; 
    float pos_x = m_pSelectedWnd->GetWindowPos().x + x;
    float pos_y = m_pSelectedWnd->GetWindowPos().y + y; 

    m_rectSelect.SetRect(pos_x, pos_y, pos_x+m_pSelectedWnd->GetWidth(), pos_y+m_pSelectedWnd->GetHeight());

    m_pTextureDlg->SetSelectedWindowTextrueRect(m_pSelectedWnd);

    OnPaint();
}

//------------------------------------------------------------------------
//	NAME : OnLvnItemActivateWindowData()
//	DESC : 좌하단 리스트 뷰컨트롤이 활성화 됐을 때 실행되는 함수
//	DATE : 20091021
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnLvnItemActivateWindowData(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    OnEditSubItemBegin( pNMIA, pResult );
}


BOOL CUIScriptStudioDlg::PreTranslateMessage(MSG* pMsg)
{
    if( OnPreTransEditSubItem( pMsg ) ) return TRUE;

    if ( m_hAccel )
    {
        if( ::TranslateAccelerator( m_hWnd, m_hAccel, pMsg ) ) 
        {
            return TRUE;
        }
    }

    if( pMsg->message == WM_LBUTTONDOWN )
        g_Magnification->CaptureScreen();   
    /*20091009 이동석 추가*/
    if( pMsg->message == WM_LBUTTONUP )
    {
        g_Magnification->CaptureScreen();   
        m_bDrag				=	false;
        m_bCtrlWindowDrag	=	false;
        //return TRUE;
    }

    if ( pMsg->message == WM_MOUSEWHEEL )
    {
        if( OnPreTransMouseWheel() )
            return TRUE;
    }
    //////////////////////////////////////////////////////////////////////////
    int topIndex = 0;

    if( pMsg->message == WM_KEYDOWN )
    {
        g_Magnification->CaptureScreen();   
        if( pMsg->wParam == VK_SHIFT )
        {
            m_bShiftState = true;
            return TRUE;
        }
        if( pMsg->wParam == VK_CONTROL )
        {
            m_bCtrl = true;
            return TRUE;
        }
        if(pMsg->wParam == VK_CAPITAL){
            m_bCaps = true;
            return TRUE;
        }
        if( pMsg->wParam == 'Z' || pMsg->wParam == 'z' )
        {
            KeyDown(pMsg->wParam);
            //return TRUE;
        }
        if( pMsg->wParam == 'Y' || pMsg->wParam == 'y' )
        {
            KeyDown(pMsg->wParam);
            //return TRUE;
        }
        if( pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP || pMsg->wParam == VK_RIGHT )
        {

            /*20091009 이동석 추가*/
            if( m_EditSubItem.m_hWnd )
            {
                switch( pMsg->wParam )
                {
                case VK_UP:
                    if( OnPrevSelectEditSubItem() ) return TRUE;
                    break;
                case VK_DOWN:
                    if( OnNextSelectEditSubItem() ) return TRUE;
                    break;
                case VK_LEFT:
                    if( --m_iEditSubItemSelPos < 0 ) m_iEditSubItemSelPos = 0;
                    m_EditSubItem.SetSel(m_iEditSubItemSelPos, m_iEditSubItemSelPos );
                    return TRUE;
                case VK_RIGHT:
                    if( ++m_iEditSubItemSelPos > m_EditSubItem.LineLength() )
                        m_iEditSubItemSelPos = m_EditSubItem.LineLength();
                    m_EditSubItem.SetSel(m_iEditSubItemSelPos, m_iEditSubItemSelPos );
                    return TRUE;
                }
            }          
            recordHistory( m_pSelectedWnd );
            KeyDown(pMsg->wParam);
            m_KeyPress = true;
            return TRUE;
        }

        if( pMsg->wParam == VK_RETURN )
        {
            OnNextSelectEditSubItem();	//	20091007 이동석
            return TRUE;
        }

        if( pMsg->wParam == VK_ESCAPE )
        {
            OnEditSubItemEnd();	//	20091007 이동석
            return TRUE;
        }
        if( GetKeyState( VK_LCONTROL ) & 0x8000 )
        {
            if (pMsg->wParam == 'F' || pMsg->wParam == 'f')
            {
                OnTreeFind();
                return true;
            }
        }
    }

    if( pMsg->message == WM_KEYUP )
    {
        g_Magnification->CaptureScreen();   
        if( pMsg->wParam == VK_SHIFT )
        {
            m_bShiftState = false;
            return TRUE;
        }
        if( pMsg->wParam == VK_CONTROL )
        {
            m_bCtrl = false;
            return TRUE;
        }
        if(pMsg->wParam == VK_CAPITAL){

            m_bCaps = false;
            return TRUE;
        }
        if( pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP || pMsg->wParam == VK_RIGHT )
        {
            recordHistory( m_pSelectedWnd );
            m_KeyPress = false;;
            return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CUIScriptStudioDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    OnEditSubItemEnd();//20091020 이동석

    if( m_bModeTest )
    {
        ChkModeTest(1, point);
        return;
    }

    if( m_bSizeControl == true )
    {
        m_bSetSize = true;
        return;
    }

    m_pointStart = point;
    if( (m_bShiftState == false) && (m_pointStart == point))
    {
        m_bDrag = true;
        m_pSelectedWnd = NULL;

        CPoint Chk = point;
        ClientToScreen(&Chk);
        m_ImageRect.ScreenToClient(&Chk);	
        if( ((0 < Chk.x) && (Chk.x <= GC_SCREEN_WIDTH)) && (( 0 < Chk.y)&& (Chk.y <= GC_SCREEN_HEIGHT )) )
        {
            HTREEITEM item = m_CtlTreeView.GetSelectedItem();
            if( !item )
                return;
            //g_pkWndClass = g_WndMgr->GetWnd((LPCTSTR)m_CtlTreeView.GetItemText(item));
            g_pkWndClass = (KD3DWndClass*)m_CtlTreeView.GetItemData(item);

            CRect gap(0,0,0,0);
            ClientToScreen(&gap);
            m_ImageRect.ScreenToClient(&gap);

            float pos_x = g_pkWndClass->GetWindowPos().x-gap.left + m_matTrans._41;
            float pos_y = g_pkWndClass->GetWindowPos().y-gap.top + m_matTrans._42; 

            for(int i = g_pkWndClass->GetChildNum()-1; i >= 0; i-- )
            {
                if(ChkSelectImageRect(g_pkWndClass->GetChild(i), point, pos_x, pos_y))
                {
                    m_pSelectedWnd = g_pkWndClass->GetChild(i);

                    OnPaint();
                    CPoint Start( m_pSelectedWnd->GetDefTexCoords().x, m_pSelectedWnd->GetDefTexCoords().y );
                    CPoint End( m_pSelectedWnd->GetDefTexCoords().z, m_pSelectedWnd->GetDefTexCoords().w);

                    m_pTextureDlg->UpdataRect(0, m_pSelectedWnd->GetDefTextureIDx(), Start, End);

                    Start.SetPoint( m_pSelectedWnd->GetHovTexCoords().x, m_pSelectedWnd->GetHovTexCoords().y );
                    End.SetPoint( m_pSelectedWnd->GetHovTexCoords().z, m_pSelectedWnd->GetHovTexCoords().w);
                    m_pTextureDlg->UpdataRect(1, m_pSelectedWnd->GetHovTextureIDx(), Start, End);

                    Start.SetPoint( m_pSelectedWnd->GetSelTexCoords().x, m_pSelectedWnd->GetSelTexCoords().y );
                    End.SetPoint( m_pSelectedWnd->GetSelTexCoords().z, m_pSelectedWnd->GetSelTexCoords().w);
                    m_pTextureDlg->UpdataRect(2, m_pSelectedWnd->GetSelTextureIDx(), Start, End);

                    Start.SetPoint( m_pSelectedWnd->GetActTexCoords().x, m_pSelectedWnd->GetActTexCoords().y );
                    End.SetPoint( m_pSelectedWnd->GetActTexCoords().z, m_pSelectedWnd->GetActTexCoords().w);
                    m_pTextureDlg->UpdataRect(3, m_pSelectedWnd->GetActTextureIDx(), Start, End);

                    Start.SetPoint( m_pSelectedWnd->GetLocTexCoords().x, m_pSelectedWnd->GetLocTexCoords().y );
                    End.SetPoint( m_pSelectedWnd->GetLocTexCoords().z, m_pSelectedWnd->GetLocTexCoords().w);
                    m_pTextureDlg->UpdataRect(4, m_pSelectedWnd->GetLocTextureIDx(), Start, End);

                    recordHistory( m_pSelectedWnd, true );

                    return;
                }
            }
            m_rectSelect.SetRect(0,0,0,0);
            OnPaint();
        }
        return;
    }

    if( !m_bShiftState )
    {
        m_bDrag = true;
        m_pointStart = point;
        return;
    }

    m_bDrag = true;
    CDialog::OnLButtonDown(nFlags, point);
}

bool CUIScriptStudioDlg::ChkSelectImageRect(KD3DWndClass* pkWndClass, CPoint point, int x, int y)
{
    float pos_x = pkWndClass->GetWindowPos().x + x;
    float pos_y = pkWndClass->GetWindowPos().y + y; 

    if((pos_x <= point.x) && (point.x <= (pos_x+pkWndClass->GetWidth())))
    {
        if((pos_y <= point.y) && (point.y <= (pos_y+pkWndClass->GetHeight())))
        {
            if( pkWndClass->IsVisible() == FALSE )
            {
                m_rectSelect.SetRect(0,0,0,0);
                return false;
            }
            m_rectSelect.SetRect(pos_x, pos_y, pos_x+pkWndClass->GetWidth(), pos_y+pkWndClass->GetHeight());
            return true;
        }
    }
    m_rectSelect.SetRect(0,0,0,0);
    return false;
}

void CUIScriptStudioDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
    if( m_bModeTest )
    {
        ChkModeTest(2,point);
        return;
    }
    m_rectSelect.left -= m_matTrans._41;
    m_rectSelect.top -= m_matTrans._42;
    m_rectSelect.right -= m_matTrans._41;
    m_rectSelect.bottom -= m_matTrans._42;

    m_matTrans._41 = 0;
    m_matTrans._42 = 0;

    OnPaint();
    CDialog::OnMButtonDown(nFlags, point);
}

void CUIScriptStudioDlg::DrawSelectRect()
{
    if( m_bModeTest )
        return;

    CRect temp = m_rectSelect;
    ClientToScreen(&temp);
    m_ImageRect.ScreenToClient(&temp);

    if(temp.left < 1)
        temp.left = 1;
    else if( temp.left > GC_SCREEN_WIDTH )
        temp.left = GC_SCREEN_WIDTH;
    if(temp.top < 1)
        temp.top = 1;
    else if( temp.top > GC_SCREEN_HEIGHT )
        temp.top = GC_SCREEN_HEIGHT;
    if(temp.bottom > GC_SCREEN_HEIGHT)
        temp.bottom = GC_SCREEN_HEIGHT;
    else if( temp.bottom < 1 )
        temp.bottom = 1;
    if(temp.right > GC_SCREEN_WIDTH ) 
        temp.right = GC_SCREEN_WIDTH;
    else if( temp.right < 1)
        temp.right = 1;

    CRect gap(0,0,0,0);
    ClientToScreen(&gap);
    m_ImageRect.ScreenToClient(&gap);

    temp.left -= gap.left;
    temp.bottom -= gap.bottom;
    temp.right -= gap.right;
    temp.top -= gap.top;


    CDC* pdc = GetDC();
    CBrush NullBrush;
    NullBrush.CreateStockObject(NULL_BRUSH);

    CPen pen(PS_SOLID, 2, RGB(255,0,102));

    CPen *poldpen = pdc->SelectObject(&pen);
    CBrush *pOldBr = pdc->SelectObject(&NullBrush);

    pdc->Rectangle(temp);
    pdc->SelectObject( pOldBr );
    pdc->SelectObject(poldpen);

    NullBrush.DeleteObject();
    pen.DeleteObject();
    ReleaseDC(pdc);
}

void CUIScriptStudioDlg::OnNMRclickTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    HMENU hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
    hMenu = GetSubMenu(hMenu,0);

    CPoint pt , pt2;
    HTREEITEM hItem;
    GetCursorPos(&pt);
    m_CtlTreeView.ScreenToClient(&pt);
    hItem = m_CtlTreeView.HitTest(pt);
    m_CtlTreeView.Select( hItem , TVGN_CARET);

    GetCursorPos(&pt2);
    TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,pt2.x,pt2.y,0,this->m_hWnd,NULL);

    *pResult = 0;
}


//------------------------------------------------------------------------
//	NAME : ADDCHILD MACRO
//	DESC : gc_sizingbox의 노가다 생성을 줄이기 위해 만든 매크로
//			아래 OnTreeAdd() 에서 사용
//------------------------------------------------------------------------
#define ADDCHILD( id ) \
    KD3DWndClass* pWnd_##id = new KD3DWndClass(); \
    pWnd_##id->SetWndName( std::string(#id) ); \
    g_WndMgr->InsertWindow( pWnd_##id, pkWindow ); \
    hAddedChild = m_CtlTreeView.InsertItem( pWnd_##id->GetWndName().c_str(), hSizingbox ); \
    pWnd_##id->SetWndType( "window" ); \
    m_CtlTreeView.SetItemData( hAddedChild, (DWORD) pWnd_##id )


//------------------------------------------------------------------------
//	NAME : OnTreeAdd()
//	DESC : 지금은 만드는 박스 종류가 2가지 밖에 없어서 놔두지만,
//			박스 종류가 많아질경우 아래의 if문을 없애고 함수포인터를
//			사용하는것이 좋을 것 같다.
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnTreeAdd()
{
    HTREEITEM hTreeItem = m_CtlTreeView.GetSelectedItem();

    if( !hTreeItem )
        return;

    KD3DWndClass* pkParent = (KD3DWndClass*)m_CtlTreeView.GetItemData(hTreeItem);
    KD3DWndClass* pkWindow = 0;

    KEditDialog dlg;
    dlg.m_strName = _T("name");
    dlg.m_strValue = _T("이름을 입력해 주세요.");

    if( dlg.DoModal() ==IDOK )
    {
        if( dlg.m_strBoxType == "gc_sizingbox" )
        {
            pkWindow = new KD3DWndClass();
            pkWindow->SetWndName( ToCharStr( dlg.m_strValue.GetBuffer() ) );
            g_WndMgr->InsertWindow ( pkWindow,pkParent );
            HTREEITEM hAddedChild = m_CtlTreeView.InsertItem ( pkWindow->GetWndName().c_str(), hTreeItem );
            pkWindow->SetWndType( "gc_sizingbox" );
            m_CtlTreeView.SetItemData( hAddedChild, (DWORD) pkWindow );

            HTREEITEM hSizingbox = hAddedChild;	

            ADDCHILD( left_top );
            ADDCHILD( top );
            ADDCHILD( right_top );
            ADDCHILD( left );
            ADDCHILD( center );
            ADDCHILD( right );
            ADDCHILD( left_bottom );
            ADDCHILD( bottom );
            ADDCHILD( right_bottom );
        }
        else
        {
            pkWindow = new KD3DWndClass();
            pkWindow->SetWndName( ToCharStr( dlg.m_strValue.GetBuffer() ) );
            g_WndMgr->InsertWindow ( pkWindow,pkParent );
            HTREEITEM hAddedChild = m_CtlTreeView.InsertItem ( pkWindow->GetWndName().c_str(), hTreeItem );
            pkWindow->SetWndType( "window" );
            m_CtlTreeView.SetItemData( hAddedChild, (DWORD) pkWindow );
        }
    }
    UpdateCtrl();
}


//------------------------------------------------------------------------
//	NAME : OnTreeDelete()
//	DESC : 우측의 트리뷰에서 윈도우를 삭제할때 호출
//	DATE : 20091021
//			최상위 윈도우를 삭제할경우 '뿜' 되는 경우 예외처리
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnTreeDelete()
{
    if( MessageBox( _T("해당 항목을 삭제하면 복구할 수 없습니다.  \n\n\t정말 삭제하시겠습니까?"), _T("알림"),MB_OKCANCEL) == IDOK )
    {
        if( MessageBox( _T("\t    진짜 ?"), _T("알림"),MB_OKCANCEL) == IDOK )
        {
            HTREEITEM hTreeItem = m_CtlTreeView.GetSelectedItem();

            if( !hTreeItem )
                return;

            KD3DWndClass* pkWnd = (KD3DWndClass*)m_CtlTreeView.GetItemData(hTreeItem);
            if( !pkWnd->GetParent() )
            {
                ::MessageBoxA( NULL, "네 그렇습니다.", "최상위 윈도우라서 삭제 못해요", MB_OK );
                return;
            }
            g_WndMgr->DeleteWindow( pkWnd );

            m_CtlTreeView.DeleteItem(hTreeItem);
            UpdateCtrl();
        }
    }
    else
    {	return;   }

}


//------------------------------------------------------------------------
//	NAME : OnTreeDelete()
//	DESC : 우측의 트리뷰에서 윈도우를 삭제할때 호출
//	DATE : 20091021
//			최상위 윈도우를 삭제할경우 '뿜' 되는 경우 예외처리
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnTreeFind()
{
    SAFE_DELETE(m_pFindUIDlg);

    m_pFindUIDlg = new CFindUI( NULL, TRUE );
    m_pFindUIDlg->Create(IDD_DIALOG_FINDUI,this);
    m_pFindUIDlg->SetWindowText( _T("트리뷰 검색"));

    if( m_pFindUIDlg->ShowWindow(SW_SHOW) )
    {
        m_pFindUIDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        RECT rect;
        CPoint pt;
        GetCursorPos(&pt);
        m_pFindUIDlg->GetWindowRect( &rect );
        m_pFindUIDlg->SetTreeView( &m_CtlTreeView );
        m_pFindUIDlg->SetWindowPos( NULL, pt.x, pt.y, rect.right, rect.bottom, SW_SHOW );
        ::SetFocus( m_pFindUIDlg->GetSafeHwnd() );
        m_pFindUIDlg->SetFocuse();
    }
}


void CUIScriptStudioDlg::OnTreeCopy()
{
    //------------------------------------------------------------------------
    //	20091029 : 이동석
    if( m_EditSubItem.m_hWnd ) return;
    //========================================================================


    HTREEITEM hTreeItem = m_CtlTreeView.GetSelectedItem();

    if( !hTreeItem )
        return;

    // m_pSourceObject = (KD3DWndClass*) m_CtlTreeView.GetItemData( m_CtlTreeView.GetSelectedItem() );

    g_WndMgr->CopyWindow( (KD3DWndClass*) m_CtlTreeView.GetItemData( m_CtlTreeView.GetSelectedItem() ) ) ;
}


//------------------------------------------------------------------------
//	NAME : OnTreePaste()
//	DESC : 기전의 포인터만 복사하던 기능을 깊은 복사로 수정
//	DATE : 20091021
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnTreePaste()
{
    //------------------------------------------------------------------------
    //	20091029 : 이동석
    if( m_EditSubItem.m_hWnd ) return;
    //========================================================================

    HTREEITEM hTreeItem = m_CtlTreeView.GetSelectedItem();

    if( !hTreeItem )
        return;

    KD3DWndClass* pParent = (KD3DWndClass*)m_CtlTreeView.GetItemData(hTreeItem);
    KD3DWndClass* pClipWindow = 0;

    if( g_WndMgr->GetClipboardPointer() )
    {
        pClipWindow = new KD3DWndClass;
        pClipWindow->DeepCopy( g_WndMgr->GetClipboardPointer() );
        pClipWindow->SetParent( pParent );

        InsertChild( pClipWindow, hTreeItem );			//	현재 선택한 윈도우에 클립보드윈도우를 차일드로 등록하고
        g_WndMgr->InsertWindow( pClipWindow, pParent );	//	윈도우 메니저에도 신고해야 한다. 안그러면 메모리가 줄줄 센다.
    }

    UpdateCtrl();
    OnPaint();
}



void CUIScriptStudioDlg::OnTreeViewTexture()
{
    HTREEITEM hTreeItem = m_CtlTreeView.GetSelectedItem();
    CString strName = m_CtlTreeView.GetItemText( hTreeItem );

    if( !hTreeItem )
        return;

    KD3DWndClass* pkWnd = (KD3DWndClass*)m_CtlTreeView.GetItemData( hTreeItem );
    g_pkSelectWnd = pkWnd;

    int iTexture = (int)g_WndMgr->m_vecStrTexFileName.size();

    bool *pbUseTexture = new bool[ iTexture ];

    for( int i = 0; i < iTexture; ++i )
        pbUseTexture[i] = false;

    GetTextureList( pkWnd, pbUseTexture );

    KViewTextureDialog dlg;
    dlg.m_iMaxTexture = iTexture;

    for( int i = 0; i < iTexture; ++i )
    {
        dlg.m_abUseTexture[i] = pbUseTexture[i];
    }

    delete [] pbUseTexture;

    dlg.DoModal();
}

void CUIScriptStudioDlg::GetTextureList( KD3DWndClass* pkWnd, bool pbUseTexture[] )
{
    // 사용중인 텍스쳐들을 모두 표시해 준다

    if( pkWnd->GetDefTextureIDx() > -1 )
        pbUseTexture[ pkWnd->GetDefTextureIDx() ] = true;
    if( pkWnd->GetHovTextureIDx() > -1 )
        pbUseTexture[ pkWnd->GetHovTextureIDx() ] = true;
    if( pkWnd->GetSelTextureIDx() > -1 )
        pbUseTexture[ pkWnd->GetSelTextureIDx() ] = true;
    if( pkWnd->GetActTextureIDx() > -1 )
        pbUseTexture[ pkWnd->GetActTextureIDx() ] = true;
    if( pkWnd->GetLocTextureIDx() > -1 )
        pbUseTexture[ pkWnd->GetLocTextureIDx() ] = true;

    for( int i = 0; i < (int)pkWnd->GetChildNum(); ++i )
        GetTextureList( pkWnd->GetChild(i), pbUseTexture );
}

void CUIScriptStudioDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if( m_bModeTest )
        return;

    CPoint Chk = point;
    ClientToScreen(&Chk);
    m_ImageRect.ScreenToClient(&Chk);	
    if( ((0 < Chk.x) && (Chk.x <= GC_SCREEN_WIDTH)) && (( 0 < Chk.y)&& (Chk.y <= GC_SCREEN_HEIGHT )) )
    {
        HTREEITEM item = m_CtlTreeView.GetSelectedItem();
        if( !item )
            return;
        g_pkWndClass = (KD3DWndClass*)m_CtlTreeView.GetItemData(item);

        CRect gap(0,0,0,0);
        ClientToScreen(&gap);
        m_ImageRect.ScreenToClient(&gap);

        float pos_x = g_pkWndClass->GetWindowPos().x-gap.left + m_matTrans._41;
        float pos_y = g_pkWndClass->GetWindowPos().y-gap.top + m_matTrans._42; 


        for(int i = g_pkWndClass->GetChildNum()-1; i >= 0; i-- )
        {
            if(ChkSelectImageRect(g_pkWndClass->GetChild(i), point, pos_x, pos_y))
            { 
                m_pSelectedWnd = g_pkWndClass->GetChild(i);
                HTREEITEM child = m_CtlTreeView.GetNextItem(item, TVGN_CHILD);
                if( i == 0 )
                {
                    m_CtlTreeView.SelectItem(child);
                    return;
                }
                while( child )
                {	
                    child = m_CtlTreeView.GetNextSiblingItem(child);
                    if( g_pkWndClass->GetChild(i) == (KD3DWndClass*)m_CtlTreeView.GetItemData(child) )
                    {
                        m_CtlTreeView.SelectItem(child);
                        return;
                    }
                }
                return;
            }
        }
        m_rectSelect.SetRect(0,0,0,0);
    }
    CDialog::OnLButtonDblClk(nFlags, point);
}

void CUIScriptStudioDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    if( m_bModeTest )
    {
        ChkModeTest(3,point);
        return;
    }

    if( !m_pSelectedWnd )
        return;
    CPoint Chk = point;
    ClientToScreen(&Chk);
    CPoint ptScreen = Chk;

    m_ImageRect.ScreenToClient(&Chk);	
    if( (0 < Chk.x) && (Chk.x <= GC_SCREEN_WIDTH) && ( 0 < Chk.y)&& (Chk.y <= GC_SCREEN_HEIGHT ))
    {
        HMENU hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
        hMenu = GetSubMenu(hMenu,1);
        TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,ptScreen.x,ptScreen.y,1,AfxGetMainWnd()->m_hWnd,NULL);
    }
    else
    {
        return;
    }
    CDialog::OnRButtonDown(nFlags, point);
}


//------------------------------------------------------------------------
//	NAME : On~()
//	DESC : 윈도우의 정렬 순서를 조정하는 함수
//	DATE : 20091021
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnBack()	//	한칸 뒤로
{
    g_WndMgr->Z_PosBack(m_pSelectedWnd);
    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::OnForth()	//	최전방으로
{
    g_WndMgr->Z_PosForth(m_pSelectedWnd);
    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::OnLast()	//	최후방으로
{
    g_WndMgr->Z_PosBottom(m_pSelectedWnd);
    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::OnFirst()	//	한칸 앞으로
{
    g_WndMgr->Z_PosTop(m_pSelectedWnd);
    UpdateCtrl();
    OnPaint();
}
//========================================================================



void CUIScriptStudioDlg::OnInserttexture()
{
    SAFE_DELETE(m_pInsertTextureDlg);
    m_pInsertTextureDlg = new DlgInsertTexture();

    if( m_pInsertTextureDlg->DoModal() == IDOK )
    {
        //20091021 이동석 D3DXMemoryLeak delete전에 호출하라고 했는데 호출해도 안잡히네-_-
        if( m_pTextureDlg )
            m_pTextureDlg->DestroyWindow();

        SAFE_DELETE(m_pTextureDlg);

        m_pTextureDlg = new CTextureViewDlg();
        m_pTextureDlg->Create(IDD_TEXTUREVIEW,this);
        m_pTextureDlg->GetTextureFilePath(m_strFilePath);
        for( UINT i = 0; i < g_WndMgr->m_vecStrTexFileName.size(); ++i )
        {
            m_pTextureDlg->AddFileName( g_WndMgr->m_vecStrTexFileName[i].c_str() );
        }
        m_pTextureDlg->LoadTexture();
    }
}

void CUIScriptStudioDlg::OnModetest()
{
    CMenu* p_menu = GetMenu();

    if( MF_CHECKED == p_menu->GetMenuState(ID_MODETEST,MF_BYCOMMAND))
    {
        p_menu->CheckMenuItem(ID_MODETEST, MF_UNCHECKED);
        m_bModeTest = false;
    }
    else
    {
        p_menu->CheckMenuItem(ID_MODETEST, MF_CHECKED);
        m_bModeTest = true;
    }
    OnPaint();
}

void CUIScriptStudioDlg::RenderModeTest(KD3DWndClass *pkWndClass, int state, int pos_x, int pos_y)
{
    if( !m_bModeTest )
        return;
    if(m_iState == -1)
        return;
    if( !m_pTestMode )
        return;

    DWORD width = pkWndClass->GetWidth();
    DWORD height = pkWndClass->GetHeight();
    float window_x = pkWndClass->GetWindowPos().x + pos_x;
    float window_y = pkWndClass->GetWindowPos().y + pos_y;
    float tu1, tv1, tu2, tv2;
    int TextureNum;

    switch( state )
    {
    case 0:		 // Hov
        if(pkWndClass->GetHovTextureIDx() == -1)
            return;
        TextureNum = pkWndClass->GetHovTextureIDx();
        tu1 = pkWndClass->GetHovTexCoords().x;
        tv1 = pkWndClass->GetHovTexCoords().y;
        tu2 = pkWndClass->GetHovTexCoords().z;
        tv2 = pkWndClass->GetHovTexCoords().w; 
        break;
    case 1:		 //Sel
        if(pkWndClass->GetSelTextureIDx() == -1)
            return;
        TextureNum = pkWndClass->GetSelTextureIDx();
        tu1 = pkWndClass->GetSelTexCoords().x;
        tv1 = pkWndClass->GetSelTexCoords().y;
        tu2 = pkWndClass->GetSelTexCoords().z;
        tv2 = pkWndClass->GetSelTexCoords().w; 
        break;
    case 2:		 //Act
        if(pkWndClass->GetActTextureIDx() == -1)
            return;
        TextureNum = pkWndClass->GetSelTextureIDx();
        tu1 = pkWndClass->GetActTexCoords().x;
        tv1 = pkWndClass->GetActTexCoords().y;
        tu2 = pkWndClass->GetActTexCoords().z;
        tv2 = pkWndClass->GetActTexCoords().w; 
        break;
    case 3:		//loc
        if(pkWndClass->GetLocTextureIDx() == -1)
            return;
        TextureNum = pkWndClass->GetLocTextureIDx();
        tu1 = pkWndClass->GetLocTexCoords().x;
        tv1 = pkWndClass->GetLocTexCoords().y;
        tu2 = pkWndClass->GetLocTexCoords().z;
        tv2 = pkWndClass->GetLocTexCoords().w; 
        break;
    default:
        return;
    }
    float tex_width = 1.0f;
    float tex_height = 1.0f;

    D3DXMATRIX mat, matRot;
    D3DXMatrixIdentity(&mat);
    D3DXMatrixIdentity(&matRot);

    float X = window_x;
    float Y = window_y;

    if( (tu1 < tu2) && (tv1 > tv2))
    {
        D3DXMatrixRotationZ(&matRot,D3DXToRadian(90.0f));
        X = cos(D3DXToRadian(-90.0f))*(window_x) - sin(D3DXToRadian(-90.0f))*(window_y);
        Y = sin(D3DXToRadian(-90.0f))*(window_x) + cos(D3DXToRadian(-90.0f))*(window_y);

        tex_width = tv1 - tv2;
        tex_height = tu2 - tu1;
    }
    else if( (tu1 > tu2) && ( tv1 < tv2))
    {
        D3DXMatrixRotationZ(&matRot,D3DXToRadian(-90.0f));
        X = cos(D3DXToRadian(90.0f))*(window_x) - sin(D3DXToRadian(90.0f))*(window_y);
        Y = sin(D3DXToRadian(90.0f))*(window_x) + cos(D3DXToRadian(90.0f))*(window_y);

        tex_width = tv2 - tv1;
        tex_height = tu1 - tu2;
    }
    else
    {
        tex_width = tu2 - tu1;
        tex_height = tv2 - tv1;
    }

    float scaling_x = ((float)width)/(tex_width);
    float scaling_y = ((float)height)/(tex_height);

    D3DXMatrixScaling(&mat,scaling_x,scaling_y,1.0f);

    mat = mat * matRot;
    //////////
    D3DXVECTOR3 pos(X/scaling_x,Y/scaling_y,0);

    //D3DXSPRITE_ALPHABLEND
    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND );  

    CRect rect;
    rect.SetRect((int)tu1,(int)tv1,(int)tu2,(int)tv2);

    g_pSprite->SetTransform(&mat);

    if(g_WndMgr->m_mapTexture.find(TextureNum) != g_WndMgr->m_mapTexture.end())
        g_pSprite->Draw( g_WndMgr->m_mapTexture[TextureNum] ,&rect,NULL,&pos,0xffffffff);
    g_pSprite->End();
}

void CUIScriptStudioDlg::OnNMRclickListchild(NMHDR *pNMHDR, LRESULT *pResult)
{
    HMENU hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
    hMenu = GetSubMenu(hMenu,2);

    POINT pt;
    GetCursorPos(&pt);

    TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,pt.x,pt.y,0,AfxGetMainWnd()->m_hWnd,NULL);

    *pResult = 0;
}

void CUIScriptStudioDlg::OnListviewSetVisible()
{
    for( int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++ )
    {
        m_ListCtrl.SetCheck(i , FALSE);
    }
    int index;
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    for( int j = 0; j < m_ListCtrl.GetSelectedCount(); j++ )
    {
        index = m_ListCtrl.GetNextSelectedItem(pos);
        m_ListCtrl.SetCheck(index);
    }
}

void CUIScriptStudioDlg::OnListviewSetInvisible()
{
    for( int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++ )
    {
        m_ListCtrl.SetCheck( i );
    }
    int index;
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    for( int j = 0; j < m_ListCtrl.GetSelectedCount(); j++ )
    {
        index = m_ListCtrl.GetNextSelectedItem(pos);
        m_ListCtrl.SetCheck( index , FALSE );
    }
}

void CUIScriptStudioDlg::OnListviewSelectAll()
{
    for( int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++ )
    {
        m_ListCtrl.SetItemState( i , 3 , LIS_FOCUSED | LIS_ENABLED | LIS_VISITED );
    }
}

void CUIScriptStudioDlg::UpdateCtrl()
{
    HTREEITEM item = m_CtlTreeView.GetSelectedItem();
    if( !item )
        return;

    g_pkWndClass = (KD3DWndClass*) m_CtlTreeView.GetItemData(item);

    if ( g_pkWndClass == NULL )
        return ;
    m_bNowLoading = true;

    m_pDataPackage->Clear();

    m_pDataPackage->PushWnd(g_pkWndClass);
    //ListCtrl Setting

    m_ListCtrl.DeleteAllItems();

    for ( UINT j = 0; j < g_pkWndClass->GetChildNum(); j++ )
    {
        m_ListCtrl.InsertItem( j , g_pkWndClass->GetChild(j)->GetWndName().c_str() );
        m_ListCtrl.SetItemData( j, (DWORD)g_pkWndClass->GetChild(j) );
        m_ListCtrl.SetCheck(j, g_pkWndClass->GetChild(j)->IsVisible() );
    }

    std::string strName;
    std::string strValue;

    // ListCtrl 한줄(Row)의 크기를 구한다.
    CRect ref;
    m_CtrlWindowData.GetItemRect(0,ref,LVIR_BOUNDS);

    const int ciNumOfOldItem = m_CtrlWindowData.GetScrollPos(SB_VERT);
    m_CtrlWindowData.DeleteAllItems();

    for(int i = 0; ;i ++)
    {
        if(!m_pDataPackage->GetData(i,&strName,&strValue))
            break;
        m_CtrlWindowData.InsertItem(i,strName.c_str());
        m_CtrlWindowData.SetItemText(i,1,strValue.c_str());
    }

    if( m_CtrlWindowData.GetItemCount() > ciNumOfOldItem )
    {
        CSize szScrollOffset(0,ref.Height()*ciNumOfOldItem);
        m_CtrlWindowData.Scroll(szScrollOffset); 
        m_CtrlWindowData.Invalidate();
    }

    m_bNowLoading = false;  
    m_rectSelect.SetRect(0,0,0,0);
    OnPaint();
}

void CUIScriptStudioDlg::OnLvnItemActivateListchild(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    *pResult = 0;

    HTREEITEM hItem = m_CtlTreeView.GetSelectedItem();
    HTREEITEM hChild = m_CtlTreeView.GetChildItem(hItem);

    while( m_CtlTreeView.GetItemData(hChild) != m_ListCtrl.GetItemData(pNMIA->iItem) )
    {
        hChild = m_CtlTreeView.GetNextSiblingItem(hChild);
        if( !hChild )
            return;
    }

    m_CtlTreeView.Select(hChild,TVGN_CARET);

    //UpdateCtrl();
}

void CUIScriptStudioDlg::KeyDown(UINT nChar)
{
    //VK_LEFT  VK_UP  VK_RIGHT  VK_DOWN  
    ///////////////////////
    if ( !m_pSelectedWnd )
        return;

    float dragX = 0, dragY = 0;
    int iMulti = 1;
    if( m_KeyPress == true )
    {	iMulti = 5;   }
    if( GetAsyncKeyState( VK_LMENU ) & 0x8000 )
    {	iMulti = 10;   }

    //if( m_bCtrl )
    if( GetKeyState( VK_LCONTROL ) & 0x8000 )
    {
        switch( nChar )
        {
        case 'z':
        case 'Z':
            Undo();
            break;

        case 'y':
        case 'Y':
            Redo();
            break;
        }
    }

    if( m_bShiftState )
    {
        switch( nChar )
        {
        case VK_LEFT:
            dragX = -1 * iMulti;
            break;
        case VK_UP:
            dragY = -1 * iMulti;
            break;
        case VK_RIGHT:
            dragX = 1 * iMulti;
            break;
        case VK_DOWN:
            dragY = 1 * iMulti;
            break;
        default:
            return;
        }

        m_rectSelect.right	+= dragX;
        m_rectSelect.bottom	+= dragY;

        m_pSelectedWnd->SetSize( m_pSelectedWnd->GetWidth() + dragX, m_pSelectedWnd->GetHeight() + dragY );
    }
    else
    {
        if(m_bCtrl){
            OnChildListItemMoveByKey(nChar);
        }
        else{
            if(m_bCaps){
                OnChildListItemSelectByKey(nChar);
            }else{
                switch( nChar )
                {
                case VK_LEFT:
                    dragX = -1 * iMulti;
                    break;
                case VK_UP:
                    dragY = -1 * iMulti;
                    break;
                case VK_RIGHT:
                    dragX = 1 * iMulti;
                    break;
                case VK_DOWN:
                    dragY = 1 * iMulti;
                    break;
                default:
                    return;
                }

                m_rectSelect.left += dragX;
                m_rectSelect.right += dragX;
                m_rectSelect.top += dragY;
                m_rectSelect.bottom += dragY;

                m_pSelectedWnd->SetWindowPos( D3DXVECTOR2(m_pSelectedWnd->GetWindowPos().x + dragX, m_pSelectedWnd->GetWindowPos().y + dragY) );
            }
        }
    }
    OnPaint();
}

void CUIScriptStudioDlg::ChkModeTest( int iState, CPoint point )
{
    CPoint Chk = point;
    ClientToScreen(&Chk);
    m_ImageRect.ScreenToClient(&Chk);	
    if( ((0 < Chk.x) && (Chk.x <= GC_SCREEN_WIDTH)) && (( 0 < Chk.y)&& (Chk.y <= GC_SCREEN_HEIGHT )) )
    {
        HTREEITEM item = m_CtlTreeView.GetSelectedItem();
        if( !item )
            return;
        g_pkWndClass = (KD3DWndClass*)m_CtlTreeView.GetItemData(item);

        CRect gap(0,0,0,0);
        ClientToScreen(&gap);
        m_ImageRect.ScreenToClient(&gap);

        float pos_x = g_pkWndClass->GetWindowPos().x + m_matTrans._41 - gap.left;
        float pos_y = g_pkWndClass->GetWindowPos().y + m_matTrans._42 - gap.top; 

        for(int i = g_pkWndClass->GetChildNum()-1; i >= 0; i-- )
        {
            if(ChkSelectImageRect(g_pkWndClass->GetChild(i), point, pos_x, pos_y))
            { 
                m_pSelectedWnd = g_pkWndClass->GetChild(i);

                m_iState = iState;
                m_pTestMode = m_pSelectedWnd;
                m_iPos_x = pos_x +gap.left;
                m_iPos_y = pos_y +gap.top;
                OnPaint();
                return;
            }
        }
        m_iState = -1;
        m_pTestMode = NULL;
        OnPaint();
    }
}

void CUIScriptStudioDlg::OnEnd()
{
    if( IDYES == MessageBox( _T("저장하시겠습니까?"), _T("끝"), MB_YESNO) )
    {
        OnFilesave();
    }
    ::SendMessage(m_hWnd,WM_CLOSE, 0 , 0);
}


//------------------------------------------------------------------------
//	NAME : OnListview~()
//	DESC : 윈도우의 정렬 순서를 조정하는 함수
//	DATE : 20091021
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnListviewBack()
{
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int index = m_ListCtrl.GetNextSelectedItem( pos ) ;

    if (index == -1 )
        return;

    //g_WndMgr->Z_PosBack( (KD3DWndClass*)m_ListCtrl.GetItemData(index) );
    SelectedWndListUpdate();
    size_t	iLimit	=	m_vecSelectedWnd.size();
    for( size_t i=0; i<iLimit ; ++i )
        //for( size_t i = iLimit-1; i <-1; --i )
        g_WndMgr->Z_PosBack( m_vecSelectedWnd[i] );

    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::OnListviewForth()
{
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int index = m_ListCtrl.GetNextSelectedItem( pos ) ;

    if (index == -1 )
        return;

    //g_WndMgr->Z_PosForth( (KD3DWndClass*)m_ListCtrl.GetItemData(index) );
    SelectedWndListUpdate();
    size_t	iLimit	=	m_vecSelectedWnd.size();
    for( size_t i = iLimit-1; i <-1; --i )
        //for( size_t i=0; i<iLimit ; ++i )
        g_WndMgr->Z_PosForth( m_vecSelectedWnd[i] );

    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::OnListviewBottom()
{
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int index = m_ListCtrl.GetNextSelectedItem( pos ) ;

    if (index == -1 )
        return;

    //g_WndMgr->Z_PosBottom( (KD3DWndClass*)m_ListCtrl.GetItemData(index) );
    SelectedWndListUpdate();
    size_t	iLimit	=	m_vecSelectedWnd.size();
    //for( size_t i=0; i<iLimit ; ++i )
    for( size_t i = iLimit-1; i <-1; --i )
        g_WndMgr->Z_PosBottom( m_vecSelectedWnd[i] );

    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::OnListviewTop()
{
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int index = m_ListCtrl.GetNextSelectedItem( pos ) ;

    if (index == -1 )
        return;

    //g_WndMgr->Z_PosTop( (KD3DWndClass*)m_ListCtrl.GetItemData(index) );
    SelectedWndListUpdate();
    size_t	iLimit	=	m_vecSelectedWnd.size();
    for( size_t i=0; i<iLimit ; ++i )
        //for( size_t i = iLimit-1; i <-1; --i )
        g_WndMgr->Z_PosTop( m_vecSelectedWnd[i] );

    UpdateCtrl();
    OnPaint();
}
//========================================================================


void CUIScriptStudioDlg::OnChange()
{
    CColorDialog dlg;
    if( dlg.DoModal() != IDOK )
        return;
    COLORREF color = dlg.GetColor();

    m_iRvalue = GetRValue(color);
    m_iGvalue = GetGValue(color);
    m_iBvalue = GetBValue(color);

    OnPaint();
}

void CUIScriptStudioDlg::OnSameposition()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    if( g_pkWndClass == NULL )
        return;
    g_pkWndClass->SetCreatedPos(g_pkWndClass->GetWindowPos());
    g_pkWndClass->SetDestroyPos(g_pkWndClass->GetWindowPos());
    UpdateCtrl();
}

void CUIScriptStudioDlg::OnSamePosition_All()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    if( g_pkWndClass == NULL )
        return;

    SetSamePosition( g_pkWndClass );

    UpdateCtrl();
}

void CUIScriptStudioDlg::SetSamePosition( KD3DWndClass *kWndClass )
{
    kWndClass->SetCreatedPos(kWndClass->GetWindowPos());
    kWndClass->SetDestroyPos(kWndClass->GetWindowPos());

    for( int i = 0; i < (int)kWndClass->GetChildNum(); ++i )
        SetSamePosition( kWndClass->GetChild(i) );
}

void CUIScriptStudioDlg::OnStnClickedImagerect()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

/////////////////////////////////////////////////////////
// 편집창에서 Ctrl-Z 기능 추가
void CUIScriptStudioDlg::clearHistory()
{
    m_dqHistory.clear();
    m_dqitNow = m_dqHistory.begin();
}

//------------------------------------------------------------------------
//	NAME : Undo()
//	DESC : 이전 동작을 취소한다.
//	DATE : 20091021
//			인덱스 유효성 점검 조건이 잘못된것 수정
//------------------------------------------------------------------------
void CUIScriptStudioDlg::Undo()
{
    // 목표대상이 없거나 가장 첫 번째 History인 경우
    if( m_dqHistory.empty() )
    {	return;   }

    // 히스토리에서 더 삭제할 것이 없는 경우
    if( m_dqitNow != m_dqHistory.begin() )
        --m_dqitNow;

    // 그림 속성과
    *((*m_dqitNow).WndID) = (*m_dqitNow).properties;
    // 빨간 테두리 위치값
    m_rectSelect.left	= (*m_dqitNow).SelRect.left;
    m_rectSelect.right	= (*m_dqitNow).SelRect.right;	
    m_rectSelect.top	= (*m_dqitNow).SelRect.top;
    m_rectSelect.bottom	= (*m_dqitNow).SelRect.bottom;

    UpdateCtrl();
    OnPaint();
}



void CUIScriptStudioDlg::Redo()
{
    std::deque<HISTORY>::iterator dqIterRear	= m_dqitNow;
    dqIterRear++;
    if( (m_dqHistory.begin()==m_dqHistory.end()) || (dqIterRear == m_dqHistory.end()) )
    {	return;   }


    m_dqitNow++;

    // 그림 속성과
    *((*m_dqitNow).WndID) = (*m_dqitNow).properties;
    // 빨간 테두리 위치값
    m_rectSelect.left	= (*m_dqitNow).SelRect.left;
    m_rectSelect.right	= (*m_dqitNow).SelRect.right;
    m_rectSelect.top	= (*m_dqitNow).SelRect.top;
    m_rectSelect.bottom	= (*m_dqitNow).SelRect.bottom;

    UpdateCtrl();
    OnPaint();
}

void CUIScriptStudioDlg::recordHistory( KD3DWndClass* _WndID , bool _btDown )
{
    if( _WndID == NULL )
    {	return;   }

    std::deque<HISTORY>::iterator dqIter = m_dqitNow;

    if(( m_dqitNow != m_dqHistory.begin()) && _btDown == true )
    {
        dqIter--;
        if( ((*dqIter).WndID) == _WndID )
        {	return;   }
    }

    HISTORY dataPair;
    dqIter = m_dqitNow;

    // 현재 위치 뒤에 있는 History 날리기
    if( dqIter != m_dqHistory.end() )
    {
        dqIter++;
        m_dqHistory.erase( dqIter, m_dqHistory.end() );
    }

    // 저장된 History가 40개 이상이면
    // 앞에서 빼고
    if( m_dqHistory.size() > 40 )
    {	m_dqHistory.pop_front();   }

    // 뒤에서 밀어 넣는다
    dataPair.WndID		= _WndID;
    dataPair.properties	= (*_WndID);
    dataPair.SelRect.left	= m_rectSelect.left;
    dataPair.SelRect.right	= m_rectSelect.right;
    dataPair.SelRect.top	= m_rectSelect.top;
    dataPair.SelRect.bottom	= m_rectSelect.bottom;
    m_dqHistory.push_back( dataPair );

    // 이 상태에서는 항상 가장 뒤에 있는 Data를 참조하므로
    m_dqitNow = m_dqHistory.end();
    m_dqitNow--;
}

/////////////////////////////////////////////////////////
// 마우스로 Size 조절하기 기능 추가
void CUIScriptStudioDlg::chckSizeControl(CPoint _point)
{
    if( (m_rectSelect.left == 0) && (m_rectSelect.top == 0) && (m_rectSelect.right == 0) && (m_rectSelect.bottom == 0) )
    {	m_bSizeControl = false;   }

    else if( (_point.x<m_rectSelect.right+1) && (_point.x>m_rectSelect.right-9) && (_point.y<m_rectSelect.bottom+1) && (_point.y>m_rectSelect.bottom-9) )
    {	m_bSizeControl = true;   }

    else
    {	m_bSizeControl = false;   }
}

void CUIScriptStudioDlg::drawSizeControl()
{
    CDC* pdc = GetDC();
    CPen pen(PS_SOLID, 2, RGB(255,255,0));
    CPen *poldpen = pdc->SelectObject(&pen);
    CPoint point;

    CBrush NullBrush;
    NullBrush.CreateStockObject(NULL_BRUSH);

    pdc->Rectangle( m_rectSelect.right-9, m_rectSelect.bottom-9, m_rectSelect.right+1, m_rectSelect.bottom+1 );	// 범위셈
    pdc->SelectObject(poldpen);

    NullBrush.DeleteObject();
    pen.DeleteObject();
    ReleaseDC(pdc);
}

void CUIScriptStudioDlg::SetNewSize(CPoint _point)
{
    m_rectSelect.right	= _point.x;
    m_rectSelect.bottom	= _point.y;

    DWORD dwTmpWidth  = _point.x - m_rectSelect.left;
    DWORD dwTmpHeight = _point.y - m_rectSelect.top;

    if( 0 < dwTmpWidth )
    {	m_pSelectedWnd->SetWidth( dwTmpWidth );   }
    if( 0 < dwTmpHeight )
    {	m_pSelectedWnd->SetHeight( dwTmpHeight );   }
}


/////////////////////////////////////////////////////////
// P4에서의 파일 상태를 체크하고 읽기전용이면 풀어주고 열려있으면 경고메세지 때려준다.
//bool CUIScriptStudioDlg::CheckP4MissionScriptState()
//{
//	//open되어있는 파일을 찾는다.
//	if ( false == CheckOpenedfile() ) // checked  'step by step' process. 070312  woosh. 
//		return false;
//
//	ForceSyncP4Files();
//	OpenForEditP4Files();
//
//	return true;
//}
//void CUIScriptStudioDlg::ForceSyncP4Files(void)
//{
//	//force 싱크를 받는다.
//	UpdateData(TRUE);
//	//ForceSyncMission.bat.bat 파일에 내용을 쓴다.
//	CString strBaseMission_P4_Path;
//
//	strBaseMission_P4_Path = L"p4 sync -f ";
//	strBaseMission_P4_Path += L"\"";
//	strBaseMission_P4_Path += m_mapMissionName_FullPath[L"BASE"].c_str();
//	strBaseMission_P4_Path += L"\"";
//	strBaseMission_P4_Path += " \r\n\r\n";
//	strBaseMission_P4_Path += "pause";
//	//strBaseMission_P4_Path += "\n";  // test for sequence running on batch files.
//	//strBaseMission_P4_Path += ":Loop";
//	//strBaseMission_P4_Path += "\n";
//	//strBaseMission_P4_Path += "dir/w";
//	//strBaseMission_P4_Path += "\n";
//	//strBaseMission_P4_Path += "goto Loop";;
//
//	char szAnsiBaseMission_p4_Path[MAX_PATH];
//
//	::ZeroMemory( szAnsiBaseMission_p4_Path, MAX_PATH);
//
//	KWideToAnsi( (LPCTSTR)strBaseMission_P4_Path, szAnsiBaseMission_p4_Path);
//
//	CFile cFile;
//	cFile.Open(L"ForceSyncMission.bat", CFile::modeCreate | CFile::modeWrite );
//	cFile.Write(szAnsiBaseMission_p4_Path, strBaseMission_P4_Path.GetLength());
//	cFile.Close();
//
//	//실행한다.
//	char cmd[MAX_PATH];      
//	ZeroMemory(cmd, MAX_PATH);
//	sprintf( cmd , "ForceSyncMission.bat" );
//	system( cmd );
//}
//
//void CUIScriptStudioDlg::OpenForEditP4Files(void)
//{
//	UpdateData(TRUE);
//	//AfxMessageBox(L"OpenForEditP4Files func is called, ForceSyncP4Files done?");
//	//force sync한 미션을 open for edit하기위해서 OpenSyncMission.bat 에 내용을 쓴다.
//	std::string strOpenForEdit;
//
//	strOpenForEdit = "p4 edit ";
//	strOpenForEdit += "\"";
//	strOpenForEdit += m_mapMissionName_FullPath[L"BASE"];
//	strOpenForEdit += "\"";
//	strOpenForEdit += "\r\n";
//	strOpenForEdit += "p4 lock ";
//	strOpenForEdit += "\"";
//	strOpenForEdit += m_mapMissionName_FullPath[L"BASE"];
//	strOpenForEdit += "\"";
//	strOpenForEdit += "\r\n";
//	strOpenForEdit += "pause";
//
//	CFile cFile;
//	cFile.Open(L"OpenSyncMission.bat", CFile::modeCreate | CFile::modeWrite );
//	cFile.Write(strOpenForEdit.c_str(), strOpenForEdit.size() );
//	cFile.Close();
//
//	//OpenSyncMission.bat 실행한다.
//	char cmd[MAX_PATH];      
//	ZeroMemory(cmd, MAX_PATH);
//	sprintf( cmd , "OpenSyncMission.bat");
//	system( cmd);
//}
//bool CUIScriptStudioDlg::CheckOpenedfile(void)
//{
//	//opened 파일 목록을 txt파일로 빼냄.
//	char cmd[MAX_PATH];      
//	ZeroMemory(cmd, MAX_PATH);
//	sprintf( cmd , "VerifyOpenMission.bat");
//	system( cmd);
//
//	FILE * pfile;
//	char szMdbName[MAX_PATH];
//
//	//AfxMessageBox(L"Is dir/w command finished?");
//	//open된 파일 목록이 아래 txt파일에 있다. 
//	//텍스트 파일을 열어서 mdb파일이 포함되어있는지 체크한다.
//	if ( (pfile = fopen("D:\\PerforceRoot2\\기획\\미션 및 드랍 툴\\DepotOpenFiles.txt", "r") ) != NULL ) 
//	{
//		while( true )
//		{
//			if ( NULL == fgets( szMdbName, MAX_PATH, pfile ) ) //한줄씩 읽어온다. NULL이면 더이상 읽어올것이 없다는 의미.
//				break;
//
//			std::map<CString, std::string>::iterator mit;
//			for ( mit = m_mapMissionName_FullPath.begin(); mit != m_mapMissionName_FullPath.end(); mit++ )
//			{
//				std::string strMdbName;
//				strMdbName = szMdbName;
//
//				if ( 0 == strMdbName.find( mit->second.c_str(), 0 ) )
//				{
//					AfxMessageBox(L"Opened Mdb file exist. Please veirfy it and submit or revert it");
//					return false;
//				}//if
//			}
//		}//while		
//	}//if
//
//	fclose( pfile );
//
//	return true;
//}

void CUIScriptStudioDlg::OnPreregFile()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    m_LoadFileInfoWindow.DoModal();
}

void CUIScriptStudioDlg::OnTextureDef()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.

    if( !m_pTextureDlg )
        return;

    if( m_pTextureDlg->ShowWindow(SW_SHOW) )
    {
        m_pTextureDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        m_pTextureDlg->ShowWindow(SW_SHOW);
    }	

    m_pTextureDlg->m_cbTexState.SetCurSel( 0 );
    m_pTextureDlg->OnCbnSelchangeState();
}

void CUIScriptStudioDlg::OnTextureHov()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.

    if( !m_pTextureDlg )
        return;

    if( m_pTextureDlg->ShowWindow(SW_SHOW) )
    {
        m_pTextureDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        m_pTextureDlg->ShowWindow(SW_SHOW);
    }	

    m_pTextureDlg->m_cbTexState.SetCurSel( 1 );
    m_pTextureDlg->OnCbnSelchangeState();
}

void CUIScriptStudioDlg::OnTextureSel()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.

    if( !m_pTextureDlg )
        return;

    if( m_pTextureDlg->ShowWindow(SW_SHOW) )
    {
        m_pTextureDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        m_pTextureDlg->ShowWindow(SW_SHOW);
    }	

    m_pTextureDlg->m_cbTexState.SetCurSel( 2 );
    m_pTextureDlg->OnCbnSelchangeState();
}

void CUIScriptStudioDlg::OnTextureAct()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.

    if( !m_pTextureDlg )
        return;

    if( m_pTextureDlg->ShowWindow(SW_SHOW) )
    {
        m_pTextureDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        m_pTextureDlg->ShowWindow(SW_SHOW);
    }

    m_pTextureDlg->m_cbTexState.SetCurSel( 3 );
    m_pTextureDlg->OnCbnSelchangeState();
}

void CUIScriptStudioDlg::OnTextureLock()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.

    if( !m_pTextureDlg )
        return;

    if( m_pTextureDlg->ShowWindow(SW_SHOW) )
    {
        m_pTextureDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        m_pTextureDlg->ShowWindow(SW_SHOW);
    }

    m_pTextureDlg->m_cbTexState.SetCurSel( 4 );
    m_pTextureDlg->OnCbnSelchangeState();
}


//------------------------------------------------------------------------
//	함수구현 추가 시작 : 20091007 이동석
//========================================================================

//------------------------------------------------------------------------
//	NAME : SelectedWndListUpdate()
//	DATE : 20091009 이동석
//	DESC : 
//			m_ListCtrl의 선택된 아이템들의 윈도우 포인터를
//			m_vecSelectedWnd에 모두 push_back한다.
//------------------------------------------------------------------------
void CUIScriptStudioDlg::SelectedWndListUpdate()
{
    m_vecSelectedWnd.clear();
    int nItem = -1;
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();

    if (pos == NULL)
        return;
    else
    {
        do
        {
            nItem = m_ListCtrl.GetNextSelectedItem(pos);
            m_vecSelectedWnd.push_back( (KD3DWndClass*)m_ListCtrl.GetItemData(nItem) );

        }while (pos);
    }
}


//------------------------------------------------------------------------
//	NAME : OnEditSubItemBegin()
//	DATE : 20091009 이동석
//	DESC : 
//			m_CtrlWindowData가 Active상태가 되면, 선택된 항목의 정보를 얻고
//			m_EditSubItem가 에디트박스를 생성하여 입력 대기.
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnEditSubItemBegin( LPNMITEMACTIVATE pNMIA, LRESULT *pResult )
{
    if( m_bEditSubItemActive ) return false;

    m_bEditSubItemActive = true;

    m_strSelectItemName		=	m_CtrlWindowData.GetItemText(pNMIA->iItem,0);
    m_strSelectItemValue	=	m_CtrlWindowData.GetItemText(pNMIA->iItem,1);
    m_iSelectItem			=	pNMIA->iItem;

    m_EditSubItem.Create( 
        WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL,
        CRect(),
        &m_CtrlWindowData, 0 );
    m_EditSubItem.ShowWindow( SW_SHOW );

    m_EditSubItem.SetWindowText( m_strSelectItemValue );
    m_EditSubItem.SetSel(0, m_strSelectItemValue.GetLength() );

    CRect rt_editDlg;
    m_CtrlWindowData.GetSubItemRect( pNMIA->iItem, 1, LVIR_BOUNDS, rt_editDlg );

    m_EditSubItem.MoveWindow( &rt_editDlg );
    m_EditSubItem.SetFocus();

    //m_CtrlWindowData.InvalidateRect( 0 );
    m_CtrlWindowData.InvalidateRect( &rt_editDlg );

    return true;
}


//------------------------------------------------------------------------
//	NAME : OnEditSubItemEnd()
//	DATE : 20091009 이동석
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnEditSubItemEnd()
{
    if( !m_bEditSubItemActive )				return false;
    if( !m_strSelectItemName.GetLength() )	return false;

    UpdateEditSubItem();

    m_EditSubItem.DestroyWindow();
    m_bEditSubItemActive = false;
    m_CtrlWindowData.InvalidateRect( 0 );
    return true;
}


//------------------------------------------------------------------------
//	NAME : OnPreTransMouseWheel()
//	DATE : 20091009 이동석
//	DESC : 먼저 휠 메세지를 가로채야 한다.
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnPreTransMouseWheel()
{
    m_CtrlWindowData.InvalidateRect( 0 );
    return ( m_bEditSubItemActive ) ? true : false ;
}


//------------------------------------------------------------------------
//	NAME : OnPrevSelectEditSubItem()
//	DATE : 20091009 이동석
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnPrevSelectEditSubItem()
{
    return OnMoveSelectEditSubItem( 0, false );
}


//------------------------------------------------------------------------
//	NAME : OnNextSelectEditSubItem()
//	DATE : 20091009 이동석
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnNextSelectEditSubItem()
{
    return OnMoveSelectEditSubItem( m_CtrlWindowData.GetItemCount(), true );
}


//------------------------------------------------------------------------
//	NAME : OnNMClickWindowData() : MFC로 자동 생성한 함수
//	DATE : 20091009 이동석
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnNMClickWindowData(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnEditSubItemEnd();
    *pResult = 0;
}


//------------------------------------------------------------------------
//	NAME : OnMoveSelectEditSubItem() : private 함수
//	DATE : 20091009 이동석
//	DESC : m_CtrlWindowData 의 SubItem 항목 선택 기능을 하는 함수.
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnMoveSelectEditSubItem( const int limit, bool bNext /* = true */ )
{
    if( !m_bEditSubItemActive ) return false;

    UpdateEditSubItem();

    //	선택이 바뀌었으므로 스크롤 처리
    const int cuiSubItemHeight = 17;
    const int cuiSubItemViewCount = 20;

    if( bNext )	//	아래로 선택
    {
        if( ++m_iSelectItem >= limit )
            m_iSelectItem = limit-1;

        if( m_iSelectItem - m_CtrlWindowData.GetTopIndex() > cuiSubItemViewCount )
            m_CtrlWindowData.Scroll( CSize( 0, cuiSubItemHeight) );
    }
    else		//	위로 선택
    {
        if( --m_iSelectItem < 0 )
            m_iSelectItem = 0;

        if( m_iSelectItem < m_CtrlWindowData.GetTopIndex() )
            m_CtrlWindowData.Scroll( CSize( 0, -cuiSubItemHeight) );
    }

    m_strSelectItemName		=	m_CtrlWindowData.GetItemText(m_iSelectItem,0);
    m_strSelectItemValue	=	m_CtrlWindowData.GetItemText(m_iSelectItem,1);
    m_EditSubItem.SetWindowText( m_strSelectItemValue );
    m_EditSubItem.SetSel(0, m_strSelectItemValue.GetLength() );

    m_CtrlWindowData.SetItem( m_iSelectItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);

    CRect rt_editDlg;
    m_CtrlWindowData.GetSubItemRect( m_iSelectItem, 1, LVIR_BOUNDS, rt_editDlg );

    m_EditSubItem.MoveWindow( &rt_editDlg );
    m_EditSubItem.SetFocus();

    //m_CtrlWindowData.InvalidateRect( 0 );
    m_CtrlWindowData.InvalidateRect( &rt_editDlg );

    return true;
}

//------------------------------------------------------------------------
//	NAME : UpdateEditSubItem() : private 함수
//	DATE : 20091015 이동석
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::UpdateEditSubItem()
{
    SelectedWndListUpdate();
    //	선택이 비어있으면 m_CtrlWndData를 수정
    if( m_vecSelectedWnd.empty() )
    {
        HTREEITEM item = m_CtlTreeView.GetSelectedItem();
        if( !item )		return false;

        g_pkWndClass = (KD3DWndClass*) m_CtlTreeView.GetItemData(item);
        if ( g_pkWndClass == NULL )	return false;

        m_bNowLoading = true;
        m_pDataPackage->Clear();
        m_pDataPackage->PushWnd(g_pkWndClass);

        CString	strItem;
        if( m_EditSubItem.m_hWnd )
            m_EditSubItem.GetWindowText( strItem );

        if( strItem.GetLength() )
            m_pDataPackage->SetValue( ToCharStr( m_strSelectItemName.GetBuffer() ), (char*)ToCharStr( strItem.GetBuffer() ) );

        //------------------------------------------------------------------------
        //	수정된 값이 name일 경우 tree의 값도 변경해 줘야 한다.
        if( m_strSelectItemName == "name" )
        {
            m_CtlTreeView.SetItemText( item , g_pkWndClass->GetWndName().c_str() );
            UpdateCtrl();
        }
        //////////////////////////////////////////////////////////////////////////

    }
    //	선택한 것이 있으면 선택한 것들 모두 수정
    else
    {
        size_t	iLimit = m_vecSelectedWnd.size();
        for( size_t i=0; i<iLimit ; ++i )
        {
            m_bNowLoading = true;
            m_pDataPackage->Clear();
            m_pDataPackage->PushWnd( m_vecSelectedWnd[i] );

            CString	strItem;
            if( m_EditSubItem.m_hWnd )
                m_EditSubItem.GetWindowText( strItem );

            if( strItem.GetLength() )
                m_pDataPackage->SetValue( ToCharStr( m_strSelectItemName.GetBuffer() ), (char*)ToCharStr( strItem.GetBuffer() ) );
            UpdateCtrl();
        }
    }
    UpdateCtrl();

    return true;
}


//------------------------------------------------------------------------
//	NAME : OnZViewoption()
//	DATE : 20091009 이동석
//	DESC : 반투명 옵션 처리
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnZViewoption()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    m_bViewOption = !m_bViewOption;
}


//------------------------------------------------------------------------
//	NAME : OnNMClickListchild()
//	DESC : EditSubItem이 포커스를 잃었을때를 위한 함수인데, 제거해야 할듯
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnNMClickListchild(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnEditSubItemEnd();//이동석
    *pResult = 0;
}


//------------------------------------------------------------------------
//	NAME : OnNMClickTreeview()
//	DESC : EditSubItem이 포커스를 잃었을때를 위한 함수인데, 제거해야 할듯
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnNMClickTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnEditSubItemEnd();
    *pResult = 0;
}

//------------------------------------------------------------------------
//	NAME : OnPreTransEditSubItem()
//	DESC : EditSubItem이 메세지를 전처리 해야 할 경우
//------------------------------------------------------------------------
bool CUIScriptStudioDlg::OnPreTransEditSubItem( MSG* pMsg )
{
    if( m_EditSubItem.m_hWnd ) //m_CtrlWindowData
    {
        if( GetKeyState( VK_LCONTROL ) & 0x8000 )
        {
            if( WM_KEYDOWN == pMsg->message	&& 'C' == pMsg->wParam )
            {
                /*m_EditSubItem.GetWindowText( m_strClipEdit );
                m_EditSubItem.SetWindowText( m_strClipEdit );*/
                //////////////////////////////////////////////////////////////////////////
                CString	cstr;
                m_EditSubItem.GetWindowText( cstr );

                if( ::OpenClipboard( m_hWnd ) )
                {
                    ::EmptyClipboard();

                    HANDLE	hData	=	::GlobalAlloc( GMEM_MOVEABLE, cstr.GetLength() + 1 );
                    LPSTR	pData	=	(LPSTR)::GlobalLock( hData );
                    ::strcpy( pData, cstr );
                    ::GlobalUnlock( hData );
                    ::SetClipboardData( CF_TEXT, hData );
                    ::CloseClipboard();
                }

            }
            return true;
        }

        if( GetKeyState( VK_LCONTROL ) & 0x8000 )
        {
            if( WM_KEYDOWN == pMsg->message	&& 'X' == pMsg->wParam )
            {
                /*CString cstr(" ");
                m_EditSubItem.GetWindowText( m_strClipEdit );
                m_EditSubItem.SetWindowText( cstr );*/
                //////////////////////////////////////////////////////////////////////////
                CString	cstr(" ");
                if( ::OpenClipboard( m_hWnd ) )
                {
                    if(  ::IsClipboardFormatAvailable( CF_TEXT ) )
                    {
                        HANDLE	hData	=	::GetClipboardData( CF_TEXT );
                        LPCSTR	pData	=	(LPCSTR)::GlobalLock( hData );
                        cstr			=	pData;
                        ::GlobalUnlock( hData );

                        m_EditSubItem.SetWindowText( cstr );
                    }
                    ::CloseClipboard();
                }

            }
            return true;
        }

        if( GetKeyState( VK_LCONTROL ) & 0x8000 )
        {
            if( WM_KEYDOWN == pMsg->message	&& 'V' == pMsg->wParam )
            {
                //CString	cstr;
                //m_EditSubItem.GetWindowText( cstr );
                //cstr += m_strClipEdit;
                //m_EditSubItem.SetWindowText( cstr );

                //////////////////////////////////////////////////////////////////////////

                CString	cstr;
                if( ::OpenClipboard( m_hWnd ) )
                {
                    if(  ::IsClipboardFormatAvailable( CF_TEXT ) )
                    {
                        HANDLE	hData	=	::GetClipboardData( CF_TEXT );
                        LPCSTR	pData	=	(LPCSTR)::GlobalLock( hData );
                        cstr			=	pData;
                        ::GlobalUnlock( hData );

                        CString	strTemp;
                        m_EditSubItem.GetWindowText( strTemp );
                        // 20101125 박현민 ----------------------------------------------------------
                        // Edit탭 복사 붙여넣기 하면 계속 붙는 문제 
                        strTemp = cstr;
                        //----------------------------------------------------------------------------
                        m_EditSubItem.SetWindowText( strTemp );
                    }
                    ::CloseClipboard();
                }

            }
            return true;
        }

        if( WM_KEYDOWN == pMsg->message	&& VK_DELETE == pMsg->wParam )
        {
            CString cstr(" ");
            m_EditSubItem.SetWindowText( cstr );
            return true;
        }     

    }

    return false;
}


//------------------------------------------------------------------------
// 2011. 08. 04 (이재준)
// 추가 : 다른 Stage 파일과 비교하여 필요한 부분을 포함시키는 기능
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnBringing()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    KUIBringingDlg Dlg;

    // g_WndMgr 포인터를 넘겨 줘서 구성한다 (왼쪽트리는 기존 데이터, 오른쪽은 비교대상)
    Dlg.m_pOrgWndMgr = g_WndMgr;

    // 현재 파일이름도 넘겨 줌!!
    Dlg.m_strLeftFilePath = m_strFileNameWithPath;

    // 사용자가 수락을 하면 그대로 두고
    // 취소하면 파일을 새로이 불러와서 구성.
    if( IDOK == Dlg.DoModal() ){		
        // 데이터를 갱신한다.(포인터를 바꾼다~)
        KD3DWndClass* pTemp;
        pTemp = g_WndMgr->m_pWndRoot;
        g_WndMgr->m_pWndRoot = Dlg.m_pLeftWndMgr->m_pWndRoot;
        Dlg.m_pLeftWndMgr->m_pWndRoot = pTemp;

        // 트리를 갱신한다.
        m_CtlTreeView.DeleteAllItems();
        InsertChild(g_WndMgr->m_pWndRoot, NULL);
    }
    else{ // IDCANCEL
        // 작업을 취소하였으므로 기존정보(g_WndMgr) 다시 로드
        // 트리를 갱신한다.
        m_CtlTreeView.DeleteAllItems();
        InsertChild(g_WndMgr->m_pWndRoot, NULL);
    }

}


//------------------------------------------------------------------------
// 기능 잠금
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnUpdateBringing(CCmdUI *pCmdUI)
{
    // 파일을 불러온 상태에서만 비교기능 실행 가능
    if(m_bIsLoaded)
        pCmdUI->Enable(TRUE);
    else{
        AfxMessageBox(_T("다른 파일에서 가져오려면 먼저 작업할 파일을 로드해야 합니다."));
        pCmdUI->Enable(FALSE);
    }
}


//------------------------------------------------------------------------
// 현재 장면을 저장한다.  2011. 08. 22
//------------------------------------------------------------------------
void CUIScriptStudioDlg::OnSaveScene()
{
    // 백버퍼를 받아올 포인터
    IDirect3DSurface9* pBackBuffer;

    g_pd3dDevice->GetRenderTarget(0, &pBackBuffer);

    // 저장할 파일경로(이름) 받아오기
    CString strFilePath;
    CFileDialog dlg(FALSE, "dds", "scene", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, ".dds");

    if( IDOK == dlg.DoModal() ){
        strFilePath = dlg.GetPathName();
    }
    else
        return;


    // 파일로 저장
    if(FAILED( D3DXSaveSurfaceToFile( strFilePath, D3DXIFF_DDS, pBackBuffer, NULL, NULL) )){
        AfxMessageBox(_T("파일로 저장하기 실패"));
        return;
    }

    // 참조카운트--
    pBackBuffer->Release();
}

void CUIScriptStudioDlg::OnFileSaveAs()
{
    CString strFilter = _T("Script 파일(*.stg)|*.stg|");

    CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if( dlg.DoModal() != IDOK )
        return;

    CString FilePath = dlg.GetPathName();


    if( FilePath.Right(4).CompareNoCase(_T(".stg")) )
        FilePath += _T(".stg");

    g_WndMgr->SaveToFile( ToCharStr( FilePath.GetBuffer() ));
}

void CUIScriptStudioDlg::OnFileSaveAsSort()
{
    CString strFilter = _T("Script 파일(*.stg)|*.stg|");

    CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if( dlg.DoModal() != IDOK )
        return;

    CString FilePath = dlg.GetPathName();


    if( FilePath.Right(4).CompareNoCase(_T(".stg")) )
        FilePath += _T(".stg");

    g_WndMgr->SaveToFile( ToCharStr( FilePath.GetBuffer()), true );
}
BOOL CUIScriptStudioDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if( GetKeyState(VK_LCONTROL) & 0x8000 )
    {
        if( zDelta > 0 )
        {
            g_Magnification->SetScale(2);
        }
        else
        {
            g_Magnification->SetScale(-2);
        }

    }
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
void CUIScriptStudioDlg::OnAllFileSave()
{
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
        strTempFilePath = FilePath;
        g_WndMgr->LoadLua( ToCharStr( strTempFilePath.GetBuffer() ) );
        m_strFilePath = FilePath.Left(FilePath.ReverseFind('/')+1);
    }


    if ( g_WndMgr->LoadUIScript() != S_OK )
    {
        MessageBox( "LoadUIScript Error", "", MB_OK );
        return;
    }

    for ( int i = 0; i < g_WndMgr->m_vecUIScriptFile.size(); ++i ) 
    {

        KWndManager* pRightWndMgr = new KWndManager();
        pRightWndMgr->Init();


        pRightWndMgr->LoadLua( ToCharStr( FilePath.GetBuffer() ) );
        if ( pRightWndMgr->LoadUIScript() != S_OK )
            continue;

        // 파일 읽기
        CString strFullPath = m_strFilePath;
        strFullPath += g_WndMgr->m_vecUIScriptFile[i].strFileName.c_str(); 
        pRightWndMgr->LoadLua( ToCharStr( strFullPath.GetString() ) );
        pRightWndMgr->DataMapping();
        pRightWndMgr->SaveToFile( strFullPath.GetString() );

        SAFE_DELETE( pRightWndMgr );
    }

    MessageBox( "Complate All File Save", "", MB_OK );

}

void CUIScriptStudioDlg::OnAllViewTexture()
{
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
        strTempFilePath = FilePath;
        g_WndMgr->LoadLua( ToCharStr( strTempFilePath.GetBuffer() ) );
        m_strFilePath = FilePath.Left(FilePath.ReverseFind('/')+1);
    }


    if ( g_WndMgr->LoadUIScript() != S_OK )
    {
        MessageBox( "LoadUIScript Error", "", MB_OK );
        return;
    }

    int iTexture = (int)g_WndMgr->m_vecStrTexFileName.size();
    if ( iTexture <= 0 ) 
    {
        MessageBox( "Texture Num Error", "", MB_OK );
        return;
    }

    bool *pbUseTexture = new bool[ iTexture ];

    for( int i = 0; i < iTexture; ++i )
        pbUseTexture[i] = false;


    for ( int i = 0; i < g_WndMgr->m_vecUIScriptFile.size(); ++i ) 
    {
        // 파일 로드

        KWndManager* pRightWndMgr = new KWndManager();
        pRightWndMgr->Init();


        pRightWndMgr->LoadLua( ToCharStr( FilePath.GetBuffer() ) );

        // 파일 읽기
        CString strFullPath = m_strFilePath;
        strFullPath += g_WndMgr->m_vecUIScriptFile[i].strFileName.c_str(); 
        pRightWndMgr->LoadLua( ToCharStr( strFullPath.GetString() ) );
        pRightWndMgr->DataMapping();

        // 해당 파일 셋팅
        KD3DWndClass* pkWnd = pRightWndMgr->m_pWndRoot;
        GetTextureList( pkWnd, pbUseTexture ); 

        SAFE_DELETE( pRightWndMgr );
    }


    // 파일 로깅
    FILE* pFile = NULL;
    pFile = fopen( "UseTextureLog.txt", "wb" );
    if ( pFile )
    {
        for ( int i = 0; i < iTexture; ++i ) 
        {
            if ( !pbUseTexture[i] ) 
            {
                fprintf( pFile, "NOT USE : %04d\t%s\n", i, g_WndMgr->m_vecStrTexFileName[i].c_str() );
            }
        }

        fclose( pFile );
    }



    delete [] pbUseTexture;


    MessageBox( "Complate UseTextureLog", "", MB_OK );

    return;

}

void CUIScriptStudioDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_bDragging){
        if(m_pDragImage == NULL) return;

        CPoint ptCursor(point);
        ClientToScreen(&ptCursor);
        m_pDragImage->DragMove(ptCursor);

        INT nTarget = GetHitIndex(point);

        if(nTarget != -1){
            if(nTarget > m_nDragIndex){
                m_ListCtrl.EnsureVisible(nTarget,FALSE);
            }else{
                m_ListCtrl.EnsureVisible(nTarget/2,FALSE);
            }

            if(nTarget != m_nDragIndex){
                m_pDragImage->DragLeave(NULL);
                m_ListCtrl.SetItemState(nTarget,LVIS_DROPHILITED, LVIS_DROPHILITED);

                m_ListCtrl.SetItemState(m_nOldTarget,0,LVIS_DROPHILITED);

                m_nOldTarget = nTarget;

                m_pDragImage->DragEnter(NULL, ptCursor);
                m_ListCtrl.UpdateWindow();
            }

        }
    }else{
        g_Magnification->CaptureScreen();
        if(m_bModeTest) 
        {
            ChkModeTest(0,point);
            return;
        }

        chckSizeControl(point);
        OnPaint();

        if( m_bSetSize == true )
        {
            SetNewSize(point);
            OnPaint();
        }

        if( !m_bDrag )
        {	return;   }

        if( m_bDrag && m_bShiftState )
        {
            float dragX = point.x - m_pointStart.x;
            float dragY = point.y - m_pointStart.y;
            m_rectSelect.left += dragX;
            m_rectSelect.top += dragY;
            m_rectSelect.right += dragX;
            m_rectSelect.bottom += dragY;

            m_matTrans._41 += dragX;
            m_matTrans._42 += dragY;
            m_pointStart = point;

            OnPaint();
            return;
        }

        float dragX = point.x - m_pointStart.x;
        float dragY = point.y - m_pointStart.y;
        m_rectSelect.left += dragX;
        m_rectSelect.top += dragY;
        m_rectSelect.right += dragX;
        m_rectSelect.bottom += dragY;

        if ( m_pSelectedWnd )
        {
            m_pSelectedWnd->SetWindowPos( D3DXVECTOR2(m_pSelectedWnd->GetWindowPos().x + dragX, m_pSelectedWnd->GetWindowPos().y + dragY) );
        }
        m_pointStart = point;

        OnPaint();
    }

    CDialog::OnMouseMove(nFlags, point);
}

INT CUIScriptStudioDlg::GetHitIndex(CPoint point){
    CRect rcList;
    m_ListCtrl.GetWindowRect(&rcList);
    ScreenToClient(reinterpret_cast<LPPOINT>(&rcList));

    LVHITTESTINFO HitInfo;
    HitInfo.pt.x = point.x - rcList.left;
    HitInfo.pt.y = point.y - rcList.top;

    return m_ListCtrl.HitTest(&HitInfo);
}

void CUIScriptStudioDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_bDragging){
        if(m_pDragImage == NULL) return;

        m_ListCtrl.SetItemState(m_nOldTarget,0,LVIS_DROPHILITED);

        m_pDragImage->DragLeave(NULL);
        m_pDragImage->EndDrag();

        ReleaseCapture();

        m_bDragging = false;
        delete m_pDragImage;
        m_pDragImage = NULL;

        INT nDest = GetHitIndex(point);
        if(-1 != nDest)
            MoveListItem(m_nDragIndex, nDest);
        else
            m_bCheck = false;

        if(m_bCheck){
            if(nDest > m_nDragIndex){
                m_ListCtrl.SetCheck(nDest-1,1);
            }else{
                m_ListCtrl.SetCheck(nDest,1);
            }
            m_bCheck = false;
        }

    }else{
        if( m_bCtrlWindowDrag )
        {
            m_bCtrlWindowDrag = false;
            m_CtrlWindowData.GetSelectedCount();
        }

        if( !g_pkWndClass )
            return;

        recordHistory( m_pSelectedWnd );

        if( m_bModeTest )
        {	return;   }

        if( m_bSetSize == true )
        {
            m_bSetSize = false;
            OnPaint();
            return;
        }

        if( (m_bShiftState == false) && (m_bDrag == true) && (m_pointStart != point))
        {
            if((m_rectSelect.left == 0)&&(m_rectSelect.top == 0)&&(m_rectSelect.right == 0)&&(m_rectSelect.bottom == 0))
                return;
            float dragX = point.x - m_pointStart.x;
            float dragY = point.y - m_pointStart.y;
            m_rectSelect.left += dragX;
            m_rectSelect.top += dragY;
            m_rectSelect.right += dragX;
            m_rectSelect.bottom += dragY;

            if ( m_pSelectedWnd )
            {
                m_pSelectedWnd->SetWindowPos( D3DXVECTOR2(m_pSelectedWnd->GetWindowPos().x + dragX, m_pSelectedWnd->GetWindowPos().y + dragY) );
            }

            m_bDrag = false;
            OnPaint();
            return;
        }

        float X = point.x - m_pointStart.x;
        float Y = point.y - m_pointStart.y;;

        m_matTrans._41 += X;
        m_matTrans._42 += Y;

        m_rectSelect.left += X;
        m_rectSelect.top += Y;
        m_rectSelect.right += X;
        m_rectSelect.bottom += Y;

        m_bShiftState = false;
        m_bDrag = false;

        OnPaint();
    }
    CDialog::OnLButtonUp(nFlags, point);
}

void CUIScriptStudioDlg::MoveListItem(const INT _nSource, const INT _nDest){
    if((_nSource == _nDest) || (_nDest < 0)) return;

    INT nSource = _nSource;

    INT nColumn = m_ListCtrl.GetItemCount();
    TCHAR Name[256+1] = {0,};

    LVITEM item;
    item.iItem = nSource;
    item.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM|LVIF_STATE;
    item.stateMask = LVIS_FOCUSED|LVIS_SELECTED|LVIS_STATEIMAGEMASK|LVIS_OVERLAYMASK;
    item.pszText = Name;
    item.cchTextMax = 256;
    item.iSubItem = 0;
    m_ListCtrl.GetItem(&item);

    item.iItem = _nDest;
    m_ListCtrl.InsertItem(&item);

    if(nSource > _nDest) nSource++;

    for(INT i = 1; i < nColumn ; i++){
        CString sText = m_ListCtrl.GetItemText(nSource,i);
        m_ListCtrl.SetItemText(_nDest,i,sText);
    }
    m_ListCtrl.DeleteItem(nSource);
}

//추가
void CUIScriptStudioDlg::OnLvnBegindragListchild(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    *pResult = 0;

    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    m_nDragIndex = pNMLV->iItem;

    POINT ptImg;
    m_pDragImage = m_ListCtrl.CreateDragImage(m_nDragIndex, &ptImg);
    if(m_pDragImage == NULL) return;

    CPoint ptSpot;
    ptSpot.x = pNMLV->ptAction.x - ptImg.x;
    ptSpot.y = pNMLV->ptAction.y - ptImg.y;
    m_pDragImage->BeginDrag(0,ptSpot);

    ptImg.x = pNMLV->ptAction.x;
    ptImg.y = pNMLV->ptAction.y;
    ClientToScreen(&ptImg);
    m_pDragImage->DragEnter(NULL,ptImg);

    m_nOldTarget = m_nDragIndex;
    m_bDragging = TRUE;

    if(m_ListCtrl.GetCheck(m_nDragIndex))
        m_bCheck = true;
    else
        m_bCheck = false;

    m_pDropWnd = m_ListCtrl.GetWindow(GW_OWNER);

    SetCapture();
}

//자식들의 크기와 좌표를 확인해서 부모의 좌표와 크기를 수정하는 부분
//처음 부모의 좌표를 수정하게 될 경우 부모의 좌표에 맞추어 자식들의 좌표가 써지므로 정상적으로 텍스쳐가 그러지지 않는 문제가 발생
//자식들의 좌표를 부모의 좌표를 참조하여 수정하여야하는 문제발생
void CUIScriptStudioDlg::OnTreeUi32898()
{
    // TODO: 여기에 명령 처리기 코드를 추가합니다.
    HTREEITEM mtree = m_CtlTreeView.GetSelectedItem();

    CString strName = m_CtlTreeView.GetItemText(mtree);

    if(!mtree)
        return;

    KD3DWndClass* pkWnd = (KD3DWndClass*)m_CtlTreeView.GetItemData(mtree);
    std::vector<KD3DWndClass*> tempchild;

    tempchild = pkWnd->GetChildContainer();

    if(tempchild.size() < 1){
        MessageBox("포함된 자식이 없네요. ㅠ");
        return;
    }

    float MinX = INT_MAX;
    float MinY = INT_MAX;
    float MaxX = 0;
    float MaxY = 0;
    bool checkTexture = false;
    int countcheckTexture = 0;

    for(int i = 0 ; i < tempchild.size() ; i++){
        if(tempchild[i]->IsVisible() == 1){
            countcheckTexture++;


            if(!checkTexture){
                MaxX = tempchild[i]->GetWindowPos().x + tempchild[i]->GetWidth();
                MaxY = tempchild[i]->GetWindowPos().y + tempchild[i]->GetHeight();
                checkTexture = true;
            }

            D3DXVECTOR2 temppos = tempchild[i]->GetWindowPos();

            if(temppos.x < MinX)
                MinX = temppos.x;

            if(temppos.y < MinY)
                MinY = temppos.y;

            if((temppos.x + tempchild[i]->GetWidth()) > MaxX)
                MaxX = temppos.x + tempchild[i]->GetWidth();

            if((temppos.y + tempchild[i]->GetHeight()) > MaxY)
                MaxY = temppos.y + tempchild[i]->GetHeight();
        }
    }

    //if(countcheckTexture == tempchild.size()){
    pkWnd->SetWidth(MaxX - MinX);
    pkWnd->SetHeight(MaxY - MinY);
    /*}else{
    int tempfframew = pkWnd->GetWidth();
    int tempfframeh = pkWnd->GetHeight();

    if(tempfframeh == 0 && tempfframew == 0){
    pkWnd->SetWidth(MaxX - MinX);
    pkWnd->SetHeight(MaxY - MinY);
    }
    }*/

    D3DXVECTOR2 temppos;

    temppos.x = MinX;
    temppos.y = MinY;

    OnSetCurrentAndFirstSubSetSize(pkWnd,temppos);

    return;
}

void CUIScriptStudioDlg::OnSetCurrentAndFirstSubSetSize(KD3DWndClass *fframe, D3DXVECTOR2 point)//현재 자신과 자신의 1차자식들간의 windowpos를 변경하는 함수
{
    std::vector<KD3DWndClass*> tempchild;
    D3DXVECTOR2 temppos;
    tempchild = fframe->GetChildContainer();

    temppos = fframe->GetWindowPos();

    if(temppos.x == 0)
        temppos.x = point.x;
    else{
        temppos.x += point.x;
    }

    if(temppos.y == 0)
        temppos.y = point.y;
    else{
        temppos.y += point.y;
    }

    (fframe)->SetWindowPos(temppos);

    if(tempchild.size() < 1){//종료조건
        return;
    }else{//자신을 수정하고 자신의 좌표까지 합한값으로 자식들을 수정.
        for(int i = 0 ; i < tempchild.size() ; i++){
            D3DXVECTOR2 subpos;

            subpos = tempchild[i]->GetWindowPos();

            if(subpos.x == 0)
                subpos.x = point.x;
            else{
                subpos.x -= point.x;
            }

            if(subpos.y == 0)
                subpos.y = point.y;
            else{
                subpos.y -= point.y;
            }

            tempchild[i]->SetWindowPos(subpos);
        }

    }
}

void CUIScriptStudioDlg::OnTreeUi32896(){
    HTREEITEM mtree = m_CtlTreeView.GetSelectedItem();

    if(!mtree)
        return;

    KD3DWndClass* pkWnd = (KD3DWndClass*)m_CtlTreeView.GetItemData(mtree);
    std::vector<KD3DWndClass*> tempchild;

    tempchild = pkWnd->GetChildContainer();

    if(tempchild.size() < 1){
        MessageBox("포함된 자식이 없네요. ㅠ");
        return;
    }

    OnSearchOneSubSet(pkWnd);
}
//현재 선택된 텍스쳐의 모든 자식들을 순회하면서 밑에 자식에서부터 size를 맞춰온다.
bool CUIScriptStudioDlg::OnSearchOneSubSet(KD3DWndClass *fframe){
    std::vector<KD3DWndClass*> tempchild;

    tempchild = fframe->GetChildContainer();
    int OneSubCheck = 0;

    for(int i = 0 ; i < tempchild.size() ; i++){
        if(tempchild[i]->GetChildContainer().size() == 0){//자식들이 하나의 텍스쳐로 이루어진 경우 이때부터 size를 맞춰가기 시작한다.
            OneSubCheck++;
        }
    }

    bool *tempsubcheck = new bool[tempchild.size()];
    for(int i = 0 ; i < tempchild.size() ; i++)
        tempsubcheck[i] = false;

    if(OneSubCheck == tempchild.size()){
        float MinX = INT_MAX;
        float MinY = INT_MAX;
        float MaxX = 0;
        float MaxY = 0;
        bool checkTexture = false;
        int countcheckTexture = 0;

        for(int i = 0 ; i < tempchild.size() ; i++){
            if(tempchild[i]->IsVisible() == 1){
                countcheckTexture++;

                if(!checkTexture){
                    MaxX = tempchild[i]->GetWindowPos().x + tempchild[i]->GetWidth();
                    MaxY = tempchild[i]->GetWindowPos().y + tempchild[i]->GetHeight();
                    checkTexture = true;
                }

                D3DXVECTOR2 temppos = tempchild[i]->GetWindowPos();

                if(temppos.x < MinX)
                    MinX = temppos.x;

                if(temppos.y < MinY)
                    MinY = temppos.y;

                if((temppos.x + tempchild[i]->GetWidth()) > MaxX)
                    MaxX = temppos.x + tempchild[i]->GetWidth();

                if((temppos.y + tempchild[i]->GetHeight()) > MaxY)
                    MaxY = temppos.y + tempchild[i]->GetHeight();
            }
        }

        if(fframe->IsVisible() == 1){
            fframe->SetWidth(MaxX - MinX);
            fframe->SetHeight(MaxY - MinY);

            D3DXVECTOR2 temppos;

            temppos.x = MinX;
            temppos.y = MinY;

            OnSetCurrentAndFirstSubSetSize(fframe,temppos);
        }

        return true;
    }else{
        for(int i = 0 ; i < tempchild.size() ; i++){
            if(tempchild[i]->GetChildContainer().size() == 0)
                tempsubcheck[i] = true;
            else{
                tempsubcheck[i] = OnSearchOneSubSet(tempchild[i]);
            }
        }

        while(1){
            int counttempcheck = 0;

            for(int i = 0 ; i < tempchild.size() ; i++)
                if(tempsubcheck[i])
                    counttempcheck++;

            if(counttempcheck == tempchild.size()){
                float MinX = INT_MAX;
                float MinY = INT_MAX;
                float MaxX = 0;
                float MaxY = 0;
                bool checkTexture = false;
                int countcheckTexture = 0;

                for(int i = 0 ; i < tempchild.size() ; i++){
                    if(tempchild[i]->IsVisible() == 1){
                        countcheckTexture++;


                        if(!checkTexture){
                            MaxX = tempchild[i]->GetWindowPos().x + tempchild[i]->GetWidth();
                            MaxY = tempchild[i]->GetWindowPos().y + tempchild[i]->GetHeight();
                            checkTexture = true;
                        }
                        D3DXVECTOR2 temppos = tempchild[i]->GetWindowPos();

                        if(temppos.x < MinX)
                            MinX = temppos.x;

                        if(temppos.y < MinY)
                            MinY = temppos.y;

                        if((temppos.x + tempchild[i]->GetWidth()) > MaxX)
                            MaxX = temppos.x + tempchild[i]->GetWidth();

                        if((temppos.y + tempchild[i]->GetHeight()) > MaxY)
                            MaxY = temppos.y + tempchild[i]->GetHeight();
                    }
                }

                if(fframe->IsVisible() == 1){

                    fframe->SetWidth(MaxX - MinX);
                    fframe->SetHeight(MaxY - MinY);

                    D3DXVECTOR2 temppos;

                    temppos.x = MinX;
                    temppos.y = MinY;

                    OnSetCurrentAndFirstSubSetSize(fframe,temppos);
                }

                return true;
            }
        }
    }
}

void CUIScriptStudioDlg::OnSetCurrentTexturePositionLeft(){
    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    int listindex;

    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            listindex = i;

    KD3DWndClass *NowSelectListItem = (KD3DWndClass*)m_ListCtrl.GetItemData(listindex);

    KD3DWndClass *NowSelectListItemParent = NowSelectListItem->GetParent();

    int pwid = NowSelectListItemParent->GetWidth();
    int phei = NowSelectListItemParent->GetHeight();

    if(phei == 0 && phei == 0){
        MessageBox("현재 텍스쳐의 부모텍스쳐의 크기가 정해져 있지 않아요.ㅠㅠ\n부모크기부터 정하고 해주세요.^^*",MB_OK);
        return;
    }

    D3DXVECTOR2 NowSelectItemPos = NowSelectListItem->GetWindowPos();
    NowSelectItemPos.x = 0;

    NowSelectListItem->SetWindowPos(NowSelectItemPos);

    OnPaint();
}

void CUIScriptStudioDlg::OnSetCurrentTexturePositionRight(){
    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    int listindex;

    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            listindex = i;

    KD3DWndClass *NowSelectListItem = (KD3DWndClass*)m_ListCtrl.GetItemData(listindex);

    KD3DWndClass *NowSelectListItemParent = NowSelectListItem->GetParent();

    int pwid = NowSelectListItemParent->GetWidth();
    int phei = NowSelectListItemParent->GetHeight();

    if(phei == 0 && phei == 0){
        MessageBox("현재 텍스쳐의 부모텍스쳐의 크기가 정해져 있지 않아요.ㅠㅠ\n부모크기부터 정하고 해주세요.^^*",MB_OK);
        return;
    }

    D3DXVECTOR2 NowSelectItemPos = NowSelectListItem->GetWindowPos();
    NowSelectItemPos.x = NowSelectListItemParent->GetWidth() - NowSelectListItem->GetWidth();

    NowSelectListItem->SetWindowPos(NowSelectItemPos);

    OnPaint();
}

void CUIScriptStudioDlg::OnSetCurrentTexturePositionUp(){
    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    int listindex;

    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            listindex = i;

    KD3DWndClass *NowSelectListItem = (KD3DWndClass*)m_ListCtrl.GetItemData(listindex);

    KD3DWndClass *NowSelectListItemParent = NowSelectListItem->GetParent();

    int pwid = NowSelectListItemParent->GetWidth();
    int phei = NowSelectListItemParent->GetHeight();

    if(phei == 0 && phei == 0){
        MessageBox("현재 텍스쳐의 부모텍스쳐의 크기가 정해져 있지 않아요.ㅠㅠ\n부모크기부터 정하고 해주세요.^^*",MB_OK);
        return;
    }

    D3DXVECTOR2 NowSelectItemPos = NowSelectListItem->GetWindowPos();
    NowSelectItemPos.y = 0;

    NowSelectListItem->SetWindowPos(NowSelectItemPos);

    OnPaint();
}

void CUIScriptStudioDlg::OnSetCurrentTexturePositionDown(){
    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    int listindex;

    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            listindex = i;

    KD3DWndClass *NowSelectListItem = (KD3DWndClass*)m_ListCtrl.GetItemData(listindex);

    KD3DWndClass *NowSelectListItemParent = NowSelectListItem->GetParent();

    int pwid = NowSelectListItemParent->GetWidth();
    int phei = NowSelectListItemParent->GetHeight();

    if(phei == 0 && phei == 0){
        MessageBox("현재 텍스쳐의 부모텍스쳐의 크기가 정해져 있지 않아요.ㅠㅠ\n부모크기부터 정하고 해주세요.^^*",MB_OK);
        return;
    }

    D3DXVECTOR2 NowSelectItemPos = NowSelectListItem->GetWindowPos();
    NowSelectItemPos.y = NowSelectListItemParent->GetHeight() - NowSelectListItem->GetHeight();

    NowSelectListItem->SetWindowPos(NowSelectItemPos);

    OnPaint();
}

void CUIScriptStudioDlg::OnSetCurrentTexturePositionUDMiddle(){
    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    int listindex;

    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            listindex = i;

    KD3DWndClass *NowSelectListItem = (KD3DWndClass*)m_ListCtrl.GetItemData(listindex);

    KD3DWndClass *NowSelectListItemParent = NowSelectListItem->GetParent();

    int pwid = NowSelectListItemParent->GetWidth();
    int phei = NowSelectListItemParent->GetHeight();

    if(phei == 0 && phei == 0){
        MessageBox("현재 텍스쳐의 부모텍스쳐의 크기가 정해져 있지 않아요.ㅠㅠ\n부모크기부터 정하고 해주세요.^^*",MB_OK);
        return;
    }

    D3DXVECTOR2 NowSelectItemPos = NowSelectListItem->GetWindowPos();
    NowSelectItemPos.y = phei/2 - NowSelectListItem->GetHeight()/2;

    NowSelectListItem->SetWindowPos(NowSelectItemPos);

    OnPaint();
}

void CUIScriptStudioDlg::OnSetCurrentTexturePositionLRMiddle(){
    if(m_ListCtrl.GetSelectedCount() <= 0) return;

    int listindex;

    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            listindex = i;

    KD3DWndClass *NowSelectListItem = (KD3DWndClass*)m_ListCtrl.GetItemData(listindex);

    KD3DWndClass *NowSelectListItemParent = NowSelectListItem->GetParent();

    int pwid = NowSelectListItemParent->GetWidth();
    int phei = NowSelectListItemParent->GetHeight();

    if(phei == 0 && phei == 0){
        MessageBox("현재 텍스쳐의 부모텍스쳐의 크기가 정해져 있지 않아요.ㅠㅠ\n부모크기부터 정하고 해주세요.^^*",MB_OK);
        return;
    }

    D3DXVECTOR2 NowSelectItemPos = NowSelectListItem->GetWindowPos();
    NowSelectItemPos.x = pwid/2 - NowSelectListItem->GetWidth()/2;

    NowSelectListItem->SetWindowPos(NowSelectItemPos);

    OnPaint();
}

void CUIScriptStudioDlg::OnChildListItemMoveByKey(UINT nChar)
{

    if( m_ListCtrl.GetSelectedCount() != 1 ) 
        return;

    int nSelect = 0;
    for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
    {
        if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
            nSelect = i;
    }


    switch ( nChar )
    {
    case VK_LEFT:
        {
            g_WndMgr->Z_PosMove( m_pSelectedWnd, -6 );
            nSelect -= 6;
        }
        break;
    case VK_RIGHT:
        {
            g_WndMgr->Z_PosMove( m_pSelectedWnd, 6 );
            nSelect += 6;
        }
        break;
    case VK_DOWN:
        {
            g_WndMgr->Z_PosForth(m_pSelectedWnd);
            nSelect++;
        }
        break;
    case VK_UP:
        {
            g_WndMgr->Z_PosBack(m_pSelectedWnd);
            nSelect--;
        }
        break;
    default:
        return;
    }

    if ( nSelect < 0 )
        nSelect = 0;
    else if ( nSelect >= m_ListCtrl.GetItemCount() )
    {
        nSelect = m_ListCtrl.GetItemCount() - 1;
    }


    UpdateCtrl();
    OnPaint();

    m_ListCtrl.SetItemState(-1,0,LVIS_SELECTED|LVIS_FOCUSED);
    m_ListCtrl.SetItemState(nSelect,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
    m_ListCtrl.EnsureVisible(nSelect,FALSE);

    return;
}


void CUIScriptStudioDlg::OnChildListItemSelectByKey(UINT nChar)
{
    if(m_ListCtrl.GetSelectedCount() > 0){
        int listindex;
        int destindex = -1;

        for(int i = 0 ; i < m_ListCtrl.GetItemCount() ; i++)
            if(m_ListCtrl.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
                listindex = i;

        if(nChar == VK_UP){
            if(m_ListCtrl.GetItemCount() > 1){
                if((listindex % 6) != 0){
                    destindex = listindex - 1;
                }
            }
        }else if(nChar == VK_RIGHT){
            if(m_ListCtrl.GetItemCount() > 6){
                if((listindex + 6) <= (m_ListCtrl.GetItemCount()-1))
                    destindex = listindex +6;
                else
                    destindex = m_ListCtrl.GetItemCount()-1;
            }

        }else if(nChar == VK_LEFT){
            if(m_ListCtrl.GetItemCount() > 6){
                if(listindex > 5){
                    destindex = listindex - 6;
                }
            }
        }else if(nChar == VK_DOWN){
            if(m_ListCtrl.GetItemCount() > 1){
                int i;
                for(i = listindex ; i > 5 ; i-=6){}

                if(i != 5){
                    if(listindex != m_ListCtrl.GetItemCount()-1)
                        destindex = listindex + 1;
                }
            }
        }

        if(destindex != -1){
            m_ListCtrl.SetItemState(-1,0,LVIS_SELECTED|LVIS_FOCUSED);
            m_ListCtrl.SetItemState(destindex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
            m_ListCtrl.EnsureVisible(destindex,FALSE);
        }
    }
}

void CUIScriptStudioDlg::OnListViewSetFind()
{

    SAFE_DELETE(m_pFindUIDlg);

    // 생성할때 두번째 인자로 체크
    m_pFindUIDlg = new CFindUI( NULL, FALSE ); 
    m_pFindUIDlg->Create(IDD_DIALOG_FINDUI,this);
    m_pFindUIDlg->SetWindowText( _T("리스트뷰 검색"));

    if( m_pFindUIDlg->ShowWindow(SW_SHOW) )
    {
        m_pFindUIDlg->ShowWindow(SW_HIDE);
    }
    else
    {
        RECT rect;
        CPoint pt;
        GetCursorPos(&pt);
        m_pFindUIDlg->GetWindowRect( &rect );
        m_pFindUIDlg->SetListView( &m_ListCtrl );
        m_pFindUIDlg->SetWindowPos( NULL, pt.x, pt.y, rect.right, rect.bottom, SW_SHOW );
        ::SetFocus( m_pFindUIDlg->GetSafeHwnd() );
        m_pFindUIDlg->SetFocuse();
    }

}
