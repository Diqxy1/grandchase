//-----------------------------------------------------------------------------
// File: Tool_Particle.cpp
//
// Desc: DirectX MFC dialog application created by the DirectX AppWizard
//-----------------------------------------------------------------------------
#define STRICT
#include "stdafx.h"
#include "Tool_Particle.h"
#include ".\tool_particle.h"
#include "ParticleEditor.h"
#include "GCDeviceManager.h"
#include <afxdlgs.h>	//CColorDialog
#include "KD3DDeviceState.h"


//-----------------------------------------------------------------------------
// Application globals
//-----------------------------------------------------------------------------
TCHAR*          g_strAppTitle       = _T( "Tool_Particle" );
CApp            g_App;
HINSTANCE       g_hInst = NULL;
CAppForm*       g_AppFormView = NULL;


//-----------------------------------------------------------------------------
// The MFC macros are all listed here
//-----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE( CAppDoc,      CDocument )
IMPLEMENT_DYNCREATE( CAppFrameWnd, CFrameWnd )
IMPLEMENT_DYNCREATE( CAppForm,     CFormView )




BEGIN_MESSAGE_MAP( CApp, CWinApp )
    //{{AFX_MSG_MAP(CApp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP( CAppForm, CFormView )
    //{{AFX_MSG_MAP(CAppForm)
    ON_COMMAND(    IDC_VIEWFULLSCREEN, OnToggleFullScreen )
    ON_BN_CLICKED(IDC_CHANGEDEVICE, OnChangeDevice)
    //}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_RENDERVIEW, OnStnClickedRenderview)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_OPENFILE, OnBnClickedOpenfile)	
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_COMMAND(ID_PARTICLE_NEW, OnParticleNew)
	ON_COMMAND(ID_COLOR, OnColor)
	ON_COMMAND(ID_40026, OnBKImg)
	ON_CBN_SELCHANGE(IDC_COMBO_SRC_BLEND, OnCbnSelchangeComboSrcBlend)
	ON_CBN_SELCHANGE(IDC_COMBO_DEST_BLEND, OnCbnSelchangeComboSrcBlend)
	ON_BN_CLICKED(IDC_BT_GRAVITY, OnBnClickedBtGravity)
	ON_BN_CLICKED(IDC_BT_EMITRADIUS, OnBnClickedBtEmitradius)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_BN_CLICKED(IDC_SCRIPTEDIT, OnBnClickedScriptedit)
	ON_BN_CLICKED(IDC_BT_EMITRATE, OnBnClickedBtEmitrate)	
	ON_COMMAND(ID_PARTICLE_FIND, OnParticleFind)
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP(CAppDoc, CDocument)
    //{{AFX_MSG_MAP(CAppDoc)
            // NOTE - the ClassWizard will add and remove mapping macros here.
            //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP(CAppFrameWnd, CFrameWnd)
    //{{AFX_MSG_MAP(CAppFrameWnd)
    ON_COMMAND(IDM_CHANGEDEVICE, OnChangeDevice)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()




//-----------------------------------------------------------------------------
// Name: CAppForm()
// Desc: Constructor for the dialog resource form
//-----------------------------------------------------------------------------
CAppForm::CAppForm()
         :CFormView( IDD_FORMVIEW )
		 , m_strName(_T(""))
		 , m_uiMaxParticle(0)
		 , m_strLifeTime(_T(""))
		 , m_strTexture(_T(""))
		 , m_strEmitRate(_T(""))
		 , m_strGravity(_T(""))
		 , m_strEmitRadius(_T(""))
		 , m_fKillTime(0.0f)
		 , m_uiKillNum(0)
{
    //{{AFX_DATA_INIT(CAppForm)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    g_AppFormView          = this;
    m_hwndRenderWindow     = NULL;
    m_hwndRenderFullScreen = NULL;
    m_hWndTopLevelParent   = NULL;

    // Override some CD3DApplication defaults:
    m_dwCreationWidth           = 500;
    m_dwCreationHeight          = 375;
    m_strWindowTitle            = TEXT( "Tool_Particle" );
    m_bUseDepthBuffer           = TRUE;

    //m_pD3DXFont                 = NULL;
    m_bLoadingApp               = TRUE;

    ZeroMemory( &m_UserInput, sizeof(m_UserInput) );
    m_fWorldRotX                = 0.0f;
    m_fWorldRotY                = 0.0f;

	m_ColorBK = 0x000000ff;

    // Read settings from registry
    ReadSettings();

	m_pBKImg = NULL;

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
}




//-----------------------------------------------------------------------------
// Name: CAppForm::OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CAppForm::OneTimeSceneInit()
{
    // TODO: perform one time initialization

    m_bLoadingApp = FALSE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadSettings()
// Desc: Read the app settings from the registry
//-----------------------------------------------------------------------------
VOID CAppForm::ReadSettings()
{
    HKEY hkey;
    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // TODO: change as needed

        // Read the stored window width/height.  This is just an example,
        // of how to use DXUtil_Read*() functions.
        DXUtil_ReadIntRegKey( hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth );
        DXUtil_ReadIntRegKey( hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight );

        RegCloseKey( hkey );
    }
}




//-----------------------------------------------------------------------------
// Name: WriteSettings()
// Desc: Write the app settings to the registry
//-----------------------------------------------------------------------------
VOID CAppForm::WriteSettings()
{
    HKEY hkey;
    DWORD dwType = REG_DWORD;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // TODO: change as needed

        // Write the window width/height.  This is just an example,
        // of how to use DXUtil_Write*() functions.
        DXUtil_WriteIntRegKey( hkey, TEXT("Width"), m_rcWindowClient.right );
        DXUtil_WriteIntRegKey( hkey, TEXT("Height"), m_rcWindowClient.bottom );

        RegCloseKey( hkey );
    }
}





//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CAppForm::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    BOOL bCapsAcceptable;

	if( dwBehavior & D3DCREATE_PUREDEVICE )
		return E_FAIL;
	

    // TODO: Perform checks to see if these display caps are acceptable.
    bCapsAcceptable = TRUE;

    if( bCapsAcceptable )         
        return S_OK;
    else
        return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::InitDeviceObjects()
{    
	g_pGCDeviceManager = new GCDeviceManager(g_pd3dDevice);
	g_pParticleEditor = new CParticleEditor();
	
	CFileFind finder;
	BOOL bWorking = finder.FindFile("./*.kom");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if( finder.IsDirectory() || finder.IsDots() )
			continue;
		g_pGCDeviceManager->GetMassFileManager()->AddMassFile((LPCTSTR)finder.GetFilePath());
	}
	g_pGCDeviceManager->GetMassFileManager()->AddDirectory("./");

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::RestoreDeviceObjects()
{
    // TODO: setup render states
    HRESULT hr;

    // Setup a material
    D3DMATERIAL9 mtrl;
    D3DUtil_InitMaterial( mtrl, 1.0f, 0.0f, 0.0f );
    m_pd3dDevice->SetMaterial( &mtrl );

    // Set up the textures
    KD3DDeviceState::SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    KD3DDeviceState::SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    KD3DDeviceState::SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    KD3DDeviceState::SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    KD3DDeviceState::SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    // Set miscellaneous render states
    KD3DDeviceState::SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    KD3DDeviceState::SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    KD3DDeviceState::SetRenderState( D3DRS_ZENABLE,        TRUE );
	KD3DDeviceState::SetRenderState( D3DRS_ZWRITEENABLE,   TRUE );
    KD3DDeviceState::SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );

    // Set the world matrix
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );
    m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // Set the projection matrix
    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
    //D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );
	D3DXMatrixOrthoLH( &matProj, 2.0f, 1.5f, .1f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    // Set up lighting states
    D3DLIGHT9 light;
    D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 2.0f );
    m_pd3dDevice->SetLight( 0, &light );
    m_pd3dDevice->LightEnable( 0, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Create a D3D font using D3DX
    //HFONT hFont = CreateFontA( 20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
    //                          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    //                          ANTIALIASED_QUALITY, FF_DONTCARE, "Arial" );      
    //if( FAILED( hr = D3DXCreateFont( m_pd3dDevice, hFont, &m_pD3DXFont ) ) )
    //    return DXTRACE_ERR_NOMSGBOX( "D3DXCreateFont", hr );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::FrameMove()
{
    // TODO: update world
	g_pParticleEditor->FrameMove(m_fElapsedTime);

    // Update user input state
    UpdateInput( &m_UserInput );

    // Update the world state according to user input
    D3DXMATRIX matWorld;
    D3DXMATRIX matRotY;
    D3DXMATRIX matRotX;

    if( m_UserInput.bRotateLeft && !m_UserInput.bRotateRight )
        m_fWorldRotY += m_fElapsedTime;
    else if( m_UserInput.bRotateRight && !m_UserInput.bRotateLeft )
        m_fWorldRotY -= m_fElapsedTime;

    if( m_UserInput.bRotateUp && !m_UserInput.bRotateDown )
        m_fWorldRotX += m_fElapsedTime;
    else if( m_UserInput.bRotateDown && !m_UserInput.bRotateUp )
        m_fWorldRotX -= m_fElapsedTime;

    D3DXMatrixRotationX( &matRotX, m_fWorldRotX );
    D3DXMatrixRotationY( &matRotY, m_fWorldRotY );

    D3DXMatrixMultiply( &matWorld, &matRotX, &matRotY );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Update the user input.  Called once per frame 
//-----------------------------------------------------------------------------
void CAppForm::UpdateInput( UserInput* pUserInput )
{
    m_bHasFocus = ( m_hWndTopLevelParent == GetForegroundWindow()->GetSafeHwnd() );
    /*
	pUserInput->bRotateUp    = ( m_bHasFocus && (GetAsyncKeyState( VK_UP )    & 0x8000) == 0x8000 );
    pUserInput->bRotateDown  = ( m_bHasFocus && (GetAsyncKeyState( VK_DOWN )  & 0x8000) == 0x8000 );
    pUserInput->bRotateLeft  = ( m_bHasFocus && (GetAsyncKeyState( VK_LEFT )  & 0x8000) == 0x8000 );
    pUserInput->bRotateRight = ( m_bHasFocus && (GetAsyncKeyState( VK_RIGHT ) & 0x8000) == 0x8000 );
	*/
}

void CAppForm::RenderBKImg()
{
	if( m_pBKImg == NULL )
		return;
	BKVertex v[4];		
	const float fZ = 0.0f;
	v[0].m_vTex = D3DXVECTOR2(0.0f,0.0f);
	v[0].m_vPos = D3DXVECTOR4(0.0f,0.0f,fZ,0.0f);

	v[1].m_vTex = D3DXVECTOR2(1.0f,0.0f);
	v[1].m_vPos = D3DXVECTOR4(800.0f,0.0f,fZ,0.0f);

	v[2].m_vTex = D3DXVECTOR2(0.0f,1.0f);
	v[2].m_vPos = D3DXVECTOR4(0.0f,600.0f,fZ,0.0f);

	v[3].m_vTex = D3DXVECTOR2(1.0f,1.0f);
	v[3].m_vPos = D3DXVECTOR4(800.0f,600.0f,fZ,0.0f);

	KD3DDeviceState::PushRenderStateSet();
	KD3DDeviceState::PushRenderState( D3DRS_ZENABLE, FALSE );
	KD3DDeviceState::PushRenderState( D3DRS_ZWRITEENABLE, FALSE );
	g_pd3dDevice->SetTexture(0,m_pBKImg);
	g_pd3dDevice->SetVertexShader(NULL);
    g_pd3dDevice->SetFVF( BKVertex::FVF );
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP ,2,v,sizeof(BKVertex));
	KD3DDeviceState::PopRenderStateSet();
}


//-----------------------------------------------------------------------------
// Name: CAppForm::Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::Render()
{
    // Clear the viewport
    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         m_ColorBK, 1.0f, 0L );

    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
		RenderBKImg();		
        // TODO: render world
		g_pParticleEditor->Render();
        
        // Render stats and help text  
        RenderText();

        // End the scene.
        m_pd3dDevice->EndScene();
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RenderText()
// Desc: Renders stats and help text to the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::RenderText()
{
    //D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    //D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
    //TCHAR szMsg[MAX_PATH] = TEXT("");
    //RECT rct;
    //ZeroMemory( &rct, sizeof(rct) );       

    //m_pD3DXFont->Begin();
    //rct.left   = 2;
    //rct.right  = m_d3dsdBackBuffer.Width - 20;

    //// Output display stats
    //INT nNextLine = 40; 

    //lstrcpy( szMsg, m_strDeviceStats );
    //nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    //m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    //lstrcpy( szMsg, m_strFrameStats );
    //nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    //m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    //// Output statistics & help
    //nNextLine = m_d3dsdBackBuffer.Height; 

    //wsprintf( szMsg, TEXT("Arrow keys: Up=%d Down=%d Left=%d Right=%d"), 
    //          m_UserInput.bRotateUp, m_UserInput.bRotateDown, m_UserInput.bRotateLeft, m_UserInput.bRotateRight );
    //nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    //m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    //wsprintf( szMsg, TEXT("World State: %0.3f, %0.3f"), 
    //                m_fWorldRotX, m_fWorldRotY );
    //nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    //m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    //lstrcpy( szMsg, TEXT("Use arrow keys to update input") );
    //nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    //m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    //lstrcpy( szMsg, TEXT("Press 'F2' to configure display") );
    //nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    //m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    //m_pD3DXFont->End();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DoDataExchange()
// Desc: DDX/DDV support
//-----------------------------------------------------------------------------
void CAppForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppForm)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST2, m_ListParticle);
	DDX_Control(pDX, IDC_COMBO_SRC_BLEND, m_BlendSrc);
	DDX_Control(pDX, IDC_COMBO_DEST_BLEND, m_BlendDest);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_PARTICLENUM, m_uiMaxParticle);
	DDX_Text(pDX, IDC_LIFETIME, m_strLifeTime);
	DDX_Control(pDX, IDC_LAYER, m_ComboLayer);
	DDX_Text(pDX, IDC_TEXTURE, m_strTexture);
	DDX_Text(pDX, IDC_EMITTIME, m_strEmitRate);
	DDX_Text(pDX, IDC_EDIT_GRAVITY, m_strGravity);
	DDX_Text(pDX, IDC_EMIT_RADIUS, m_strEmitRadius);

	DDX_GridControl(pDX, IDC_GRID_TIMELINE, m_GridTimeLine);
	DDX_Text(pDX, IDC_KILLTIME, m_fKillTime);
	DDX_Text(pDX, IDC_KILLNUM, m_uiKillNum);
}




//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: 
//-----------------------------------------------------------------------------
LRESULT CAppForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    return CFormView ::WindowProc(message, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: OnChangeDevice()
// Desc: Needed to enable dlg menu item 
//-----------------------------------------------------------------------------
void CAppFrameWnd::OnChangeDevice() 
{
    g_AppFormView->OnChangeDevice();
}




//-----------------------------------------------------------------------------
// Name: CAppForm::InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CAppForm::InvalidateDeviceObjects()
{
    // TODO: Cleanup any objects created in RestoreDeviceObjects()
    //SAFE_RELEASE( m_pD3DXFont );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::DeleteDeviceObjects()
{
    // TODO: Cleanup any objects created in InitDeviceObjects()

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CAppForm::FinalCleanup()
{
    // TODO: Perform any final cleanup needed
    // Write the settings to the registry
    WriteSettings();

	SAFE_DELETE(g_pParticleEditor);
	SAFE_DELETE(g_pGCDeviceManager);
	SAFE_RELEASE(m_pBKImg);

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitInstance()
// Desc: This is the main entry point for the application. The MFC window stuff
//       is initialized here. See also the main initialization routine for the
//       CAppForm class, which is called indirectly from here.
//-----------------------------------------------------------------------------
BOOL CApp::InitInstance()
{
    // Asscociate the MFC app with the frame window and doc/view classes
    AddDocTemplate( new CSingleDocTemplate( IDR_MAINFRAME,
                                            RUNTIME_CLASS(CAppDoc),
                                            RUNTIME_CLASS(CAppFrameWnd),
                                            RUNTIME_CLASS(CAppForm) ) );

    // Dispatch commands specified on the command line (req'd by MFC). This
    // also initializes the the CAppDoc, CAppFrameWnd, and CAppForm classes.
    CCommandLineInfo cmdInfo;
    ParseCommandLine( cmdInfo );
    if( !ProcessShellCommand( cmdInfo ) )
        return FALSE;

    if( !g_AppFormView->IsReady() )
        return FALSE;

    g_AppFormView->GetParentFrame()->RecalcLayout();
    g_AppFormView->ResizeParentToFit( FALSE ); 
    
    m_pMainWnd->SetWindowText( g_strAppTitle );
    m_pMainWnd->UpdateWindow();

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: LoadFrame()
// Desc: Uses idle time to render the 3D scene.
//-----------------------------------------------------------------------------
BOOL CAppFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
    BOOL bResult = CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);

    LoadAccelTable( MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

    return bResult;
}




//-----------------------------------------------------------------------------
// Name: OnIdle()
// Desc: Uses idle time to render the 3D scene.
//-----------------------------------------------------------------------------
BOOL CApp::OnIdle( LONG )
{
    // Do not render if the app is minimized
    if( m_pMainWnd->IsIconic() )
        return FALSE;

    TCHAR strStatsPrev[200];

    lstrcpy(strStatsPrev, g_AppFormView->PstrFrameStats());

    // Update and render a frame
    if( g_AppFormView->IsReady() )
    {
        g_AppFormView->CheckForLostFullscreen();
        g_AppFormView->RenderScene();
    }

    // Keep requesting more idle time
    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: PreCreateWindow()
// Desc: Change the window style (so it cannot maximize or be sized) before
//       the main frame window is created.
//-----------------------------------------------------------------------------
BOOL CAppFrameWnd::PreCreateWindow( CREATESTRUCT& cs )
{
    cs.style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;

    return CFrameWnd::PreCreateWindow( cs );
}




//-----------------------------------------------------------------------------
// Name: ~CAppForm()
// Desc: Destructor for the dialog resource form. Shuts down the app
//-----------------------------------------------------------------------------
CAppForm::~CAppForm()
{
    Cleanup3DEnvironment();
}




//-----------------------------------------------------------------------------
// Name: OnToggleFullScreen()
// Desc: Called when user toggles the fullscreen mode
//-----------------------------------------------------------------------------
void CAppForm::OnToggleFullScreen()
{
    ToggleFullscreen();
}




//-----------------------------------------------------------------------------
// Name: OnChangeDevice()
// Desc: Use hit the "Change Device.." button. Display the dialog for the user
//       to select a new device/mode, and call Change3DEnvironment to
//       use the new device/mode.
//-----------------------------------------------------------------------------
VOID CAppForm::OnChangeDevice()
{
    Pause(TRUE);

    UserSelectNewDevice();

    // Update UI
    UpdateUIForDeviceCapabilites();

    Pause(FALSE);
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Adjusts the window properties for windowed or fullscreen mode
//-----------------------------------------------------------------------------
HRESULT CAppForm::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        ::ShowWindow( m_hwndRenderFullScreen, SW_HIDE );
        CD3DApplication::m_hWnd = m_hwndRenderWindow;
    }
    else
    {
        if( ::IsIconic( m_hwndRenderFullScreen ) )
            ::ShowWindow( m_hwndRenderFullScreen, SW_RESTORE );
        ::ShowWindow( m_hwndRenderFullScreen, SW_SHOW );
        CD3DApplication::m_hWnd = m_hwndRenderFullScreen;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FullScreenWndProc()
// Desc: The WndProc funtion used when the app is in fullscreen mode. This is
//       needed simply to trap the ESC key.
//-----------------------------------------------------------------------------
LRESULT CALLBACK FullScreenWndProc( HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam )
{
    if( msg == WM_CLOSE )
    {
        // User wants to exit, so go back to windowed mode and exit for real
        g_AppFormView->OnToggleFullScreen();
        g_App.GetMainWnd()->PostMessage( WM_CLOSE, 0, 0 );
    }
    else if( msg == WM_SETCURSOR )
    {
        SetCursor( NULL );
    }
    else if( msg == WM_KEYUP && wParam == VK_ESCAPE )
    {
        // User wants to leave fullscreen mode
        g_AppFormView->OnToggleFullScreen();
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: CheckForLostFullscreen()
// Desc: If fullscreen and device was lost (probably due to alt-tab), 
//       automatically switch to windowed mode
//-----------------------------------------------------------------------------
HRESULT CAppForm::CheckForLostFullscreen()
{
    HRESULT hr;

    if( m_bWindowed )
        return S_OK;

    if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
        ForceWindowed();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateUIForDeviceCapabilites()
// Desc: Whenever we get a new device, call this function to enable/disable the
//       appropiate UI controls to match the device's capabilities.
//-----------------------------------------------------------------------------
VOID CAppForm::UpdateUIForDeviceCapabilites()
{
    // TODO: Check the capabilities of the device and update the UI as needed
    DWORD dwCaps = m_d3dCaps.RasterCaps;
}




//-----------------------------------------------------------------------------
// Name: OnInitialUpdate()
// Desc: When the AppForm object is created, this function is called to
//       initialize it. Here we getting access ptrs to some of the controls,
//       and setting the initial state of some of them as well.
//-----------------------------------------------------------------------------
VOID CAppForm::OnInitialUpdate()
{
    // Update the UI
    CFormView::OnInitialUpdate();

    // Get the top level parent hwnd
    m_hWndTopLevelParent = GetTopLevelParent()->GetSafeHwnd();

    // Save static reference to the render window
    m_hwndRenderWindow = GetDlgItem(IDC_RENDERVIEW)->GetSafeHwnd();

    // Register a class for a fullscreen window
    WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW, FullScreenWndProc, 0, 0, NULL,
                          NULL, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL,
                          _T("Fullscreen Window") };
    RegisterClass( &wndClass );

    // We create the fullscreen window (not visible) at startup, so it can
    // be the focus window.  The focus window can only be set at CreateDevice
    // time, not in a Reset, so ToggleFullscreen wouldn't work unless we have
    // already set up the fullscreen focus window.
    m_hwndRenderFullScreen = CreateWindow( _T("Fullscreen Window"), NULL,
                                           WS_POPUP, CW_USEDEFAULT,
                                           CW_USEDEFAULT, 100, 100,
                                           m_hWndTopLevelParent, 0L, NULL, 0L );

    // Note that for the MFC samples, the device window and focus window
    // are not the same.
    CD3DApplication::m_hWnd = m_hwndRenderWindow;
    CD3DApplication::m_hWndFocus = m_hwndRenderFullScreen;
    CD3DApplication::Create( AfxGetInstanceHandle() );

    // TODO: Update the UI as needed
	m_ListParticle.InitList();	

	for( int i = 0; i < CKTDGParticleSystem::CParticleEmitterTokenizer::NUMBLENDINGMODES; i++ )
	{
		m_BlendSrc.AddString( CKTDGParticleSystem::CParticleEmitterTokenizer::m_BlendingModes[i].name );
		m_BlendDest.AddString( CKTDGParticleSystem::CParticleEmitterTokenizer::m_BlendingModes[i].name );
	}

	m_ComboLayer.AddString( "LAYER_SKY" );
	m_ComboLayer.AddString( "LAYER_BETWEEN_SKY_BLOCK" );
	m_ComboLayer.AddString( "LAYER_BLOCK" );
	m_ComboLayer.AddString( "LAYER_BETWEEN_BLOCK_CHAR" );
	m_ComboLayer.AddString( "LAYER_CHAR" );
	m_ComboLayer.AddString( "LAYER_BETWEEN_CHAR_FRONT" );
	m_ComboLayer.AddString( "LAYER_FRONT_MAP" );
	m_ComboLayer.AddString( "LAYER_FACE" );
	m_ComboLayer.AddString( "LAYER_UI" );
}

void CAppForm::OnStnClickedRenderview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAppForm::OnFileOpen()
{
	CString strFilter = "GC Lua Script File (*.stg)|*.stg| Lua Script File (*.lua)|*.lua";
	CFileDialog dlg(TRUE,"stg", m_strOpenFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this );
	if( dlg.DoModal() != IDOK ) 
		return;

	m_strOpenFile = dlg.GetPathName();
	m_ListParticle.m_bLock = true;
	g_pParticleEditor->SetWorkParticle(NULL);

	g_pParticleEditor->LoadScript(m_strOpenFile);
	m_ListParticle.m_bLock = false;
}

void CAppForm::OnFileSave()
{
	CString strFilter = "GC Lua Script File (*.stg)|*.stg| Lua Script File (*.lua)|*.lua";
	CFileDialog dlg(FALSE,"stg", m_strOpenFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this );
	if( dlg.DoModal() != IDOK ) 
		return;

	m_strOpenFile = dlg.GetPathName();

	g_pParticleEditor->SaveScript(m_strOpenFile);
}

void CAppForm::OnBnClickedOpenfile()
{
	CString strFilter = "DirectX Texture File (*.dds)|*.dds| TGA (*.tga)|*.tga";
	CFileDialog dlg(TRUE,"dds", m_strOpenTextureFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this );
	if( dlg.DoModal() != IDOK ) 
		return;

	m_strOpenTextureFile = dlg.GetPathName();
	m_strTexture = dlg.GetFileName();
	UpdateData(FALSE);
}
/*
void CAppForm::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( pNMLV->uNewState & LVIS_SELECTED )
	{	
		if( !m_bLockList )
		{
			
			g_pParticleEditor->SetWorkParticle((CKTDGParticleSystem::CParticleEventSequence*) pNMLV->lParam );
			m_iSelItem = pNMLV->iItem;
		}
		//m_ListParticle.GetItemData(pInfo->iItem) );
		*pResult = 0;
		m_ListParticle.SetFocus();
	}
}
*/

void CAppForm::OnBnClickedApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pParticleEditor->UpdateParticleProperty();
}


void CAppForm::OnParticleNew()
{
	g_pParticleEditor->OnNewParticle();
}

void CAppForm::OnColor()
{
	CColorDialog dlg(0,0,this);
	if( dlg.DoModal() != IDOK )
		return;

	m_ColorBK = D3DCOLOR_ARGB(0,GetRValue(dlg.GetColor()),GetGValue(dlg.GetColor()),GetBValue(dlg.GetColor()) );
}

void CAppForm::OnBKImg()
{
	CString strFilter = "DirectX Texture File|*.dds;*.jpg;*.tga;*.bmp|";
	CFileDialog dlg(TRUE,"dds", m_strOpenTextureFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this );
	if( dlg.DoModal() != IDOK ) 
		return;

	SAFE_RELEASE(m_pBKImg);
	D3DXCreateTextureFromFileEx( g_pd3dDevice,(LPCTSTR)dlg.GetPathName(),D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, NULL, NULL, &m_pBKImg );

}

void CAppForm::OnCbnSelchangeComboSrcBlend()
{
	g_pParticleEditor->UpdateParticleProperty();
}

void CAppForm::OnBnClickedBtGravity()
{
	g_pParticleEditor->SetGravity();
}

void CAppForm::OnBnClickedBtEmitradius()
{
	g_pParticleEditor->SetEmitRadius();
}

void CAppForm::OnBnClickedScriptedit()
{
	g_pParticleEditor->OnScriptEdit();
}

void CAppForm::OnBnClickedBtEmitrate()
{
	g_pParticleEditor->SetEmitRate();
}

void CAppForm::OnParticleFind()
{
	//g_pParticleEditor->FindParticle();
}
