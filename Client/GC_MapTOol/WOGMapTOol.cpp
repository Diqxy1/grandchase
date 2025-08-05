//-----------------------------------------------------------------------------
// File: WOGMapTOol.cpp
//
// Desc: DirectX MFC dialog application created by the DirectX AppWizard
//-----------------------------------------------------------------------------
#define STRICT
#define DIRECTINPUT_VERSION 0x0800

//#include <ImportKncDebug.h>
#include "WOGMapTOol.h"
#include "resource.h"
#include "KCreateLayerPartDlg.h"
//#include "GCDeviceManager.h"
#include "LayerManager.h"
#include "KCChooseCategoryDlg.h"
#include "KGCMap.h"
#include "Layer.h"
//#include <assert.h>

#include "ToolObject.h"
#include "FootHold.h"
//#include "GCEffectManager.h"
#include "NewMapDlg.h"
#include "ResizingDlg.h"

// Prop Windows
#include "UserProp.h"
#include "PartList.h"
#include "LayerList.h"
#include "EffectProp.h"
#include "LayerProp.h"
#include "MoveProp.h"
#include "FootHoldValue.h"

#include "Command/WorldResizeCmd.h"
#include "Command/ModifyLayerPartCmd.h"
#include "Command/LayerPartKeyMoveCmd.h"
#include "Command/StampObjectCmd.h"
#include "Command/UnStampObjectCmd.h"
#include "Command/CommandRecv.h"

#include "GridGuideline/KGridGuideMgr.h"
#include "ChangeGridGuideTerm.h"

//#ifdef _DEBUG
//#include "vld.h"
//#include <dbg/dbg.hpp>
//#endif

//#include <boost/algorithm/string.hpp>

#define GC_FPS_LIMIT        (55)
#define GC_ELAPSED_TIME     (1.0f/GC_FPS_LIMIT)
#define KEY_INTERVAL        ((DWORD)100)

//-----------------------------------------------------------------------------
// Application globals
//-----------------------------------------------------------------------------
TCHAR*          g_strAppTitle = _T("WOGMapTOol Beta v0.5");
CApp            g_App;
HINSTANCE       g_hInst = NULL;
CAppForm*       g_AppFormView = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
GCDeviceShader*   g_pRenderState = NULL;
IDirect3DStateBlock9* g_pBlendStateBlock = NULL;
D3DXMATRIX g_matViewProj;
char g_Path[MAX_PATH];

bool g_bFootHoldImaging = false;

GUID g_guidApp = { 0xbb55e26d, 0x67ce, 0x4c31, { 0x95, 0xed, 0xc2, 0xd8, 0x10, 0xe0, 0x1c, 0xc1 } };

#define LAYERINDEXCONDITION if( m_iLayerIndex < SiCLayerManager()->GetMaxLayerNum() && !vecLayer.empty() )

//-----------------------------------------------------------------------------
// The MFC macros are all listed here
//-----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CAppDoc, CDocument)
IMPLEMENT_DYNCREATE(CAppFrameWnd, CFrameWnd)
IMPLEMENT_DYNCREATE(CAppForm, CFormView)

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CAppForm, CFormView)
	//{{AFX_MSG_MAP(CAppForm)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_LAYERMANAGER_CREATELAYERPART, OnLayermanagerCreatelayerpart)
	ON_COMMAND(ID_LAYERMANAGER_EDITLAYER, OnLayermanagerEditlayer)
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_ACCEL_FILE_LOAD, OnAccelFileLoad)
	ON_COMMAND(ID_ACCEL_EXIT, OnAccelExit)
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_ACCEL_RESETVIEW, OnAccelResetview)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_ACCEL_FILE_SAVE, OnAccelFileSave)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_ACCEL_FILE_NEW, OnAccelFileNew)
	ON_COMMAND(ID_MODE_LAYER, OnModeLayer)
	ON_COMMAND(ID_MODE_FH, OnModeFh)
	ON_COMMAND(ID_ACCEL_MODE_FH, OnAccelModeFh)
	ON_COMMAND(ID_ACCEL_MODE_LAYER, OnAccelModeLayer)
	ON_COMMAND(ID_ACCEL_FILE_CLOSE, OnAccelFileClose)
	ON_COMMAND(ID_WORLDRESIZE, OnWorldresize)
	ON_COMMAND(ID_ACCEL_WORLD_RESIZE, OnAccelWorldResize)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_DEBUG_ZORDERSORTING, OnDebugZordersorting)
	ON_COMMAND(ID_ACCEL_FILE_SAVE_AS, OnAccelFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_ACCEL_UNDO, OnAccelUndo)
	ON_COMMAND(ID_ACCEL_REDO, OnAccelRedo)
	ON_COMMAND(ID_GRIDGUIDELINE_CHANGE_TERM, OnGridguidelineChangeTerm)
	ON_COMMAND(ID_GRID_ADD_VERTICAL_GUIDE, OnGridAddVerticalGuide)
	ON_COMMAND(ID_GRID_ADD_HORIZON_GUIDE, OnGridAddHorizonGuide)
	ON_BN_CLICKED(IDC_H_GUIDELINE, OnBnClickedHGuideline)
	ON_BN_CLICKED(IDC_V_GUIDELINE, OnBnClickedVGuideline)
	ON_COMMAND(ID_FORCEDEBUG_MAKEFOOTHOLDIMAGE, OnForcedebugMakefootholdimage)
	ON_COMMAND(ID_ACCEL_FOOT_HOLD, OnAccelFootHold)
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
	:CFormView(IDD_FORMVIEW)
#ifdef _DEBUG
	, m_fpStream(NULL)
#endif
{
	//{{AFX_DATA_INIT(CAppForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	g_AppFormView = this;
	m_hwndRenderWindow = NULL;
	m_hWndTopLevelParent = NULL;

	// Override some CD3DApplication defaults:
	m_dwCreationWidth = 800;
	m_dwCreationHeight = 600;
	m_strWindowTitle = TEXT("WOGMapTOol");

	// Create a D3D font using d3dfont.cpp
	m_pFont = new CD3DFont(_T("Arial"), 12, D3DFONT_BOLD);
	m_bLoadingApp = TRUE;

	m_fWorldRotX = 0.0f;
	m_fWorldRotY = 0.0f;

	// Read settings from registry
	ReadSettings();

	SiKGCMap();
	SiCLayerManager();
	m_pSwapChain1 = NULL;
	m_pSwapChain2 = NULL;

	m_bListLoaded = false;
	m_bScriptLoaded = false;

	m_dwTotalFrame = 0;

	InitializeCriticalSection(&m_csSwap);

	m_strFileName = "";

	m_ppTObj = NULL;

	m_vecCurObj.clear();

	memset(m_Vertex, 0, sizeof(m_Vertex));
	memset(m_CurLayerVertex, 0, sizeof(m_CurLayerVertex));
	memset(m_MonitorVertex, 0, sizeof(m_MonitorVertex));
	memset(m_CurObjVertex, 0, sizeof(m_CurObjVertex));

	ResizedX = 0;
	ResizedY = 0;
	m_iChangeLayerIndex = 0;
	m_bRenderFH = false;

	m_pUserProp = NULL;
	m_pPartList = NULL;
	m_pLayerList = NULL;
	m_pLayerProp = NULL;
	m_pMoveProp = NULL;
	m_pFHValue = NULL;

#ifdef _DEBUG
	AllocConsole();
	m_fpStream = freopen("CONOUT$", "a", stdout); // redirect printf to console
#endif

	m_pCommandRecv = new CCommandRecv();

	m_bFootHoldImaging = false;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CAppForm::OneTimeSceneInit()
{
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
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, DXAPP_KEY,
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		// Read the stored window width/height.  This is just an example,
		// of how to use DXUtil_Read*() functions.
		DXUtil_ReadIntRegKey(hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth);
		DXUtil_ReadIntRegKey(hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight);

		RegCloseKey(hkey);
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

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, DXAPP_KEY,
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		// Write the window width/height.  This is just an example,
		// of how to use DXUtil_Write*() functions.
		DXUtil_WriteIntRegKey(hkey, TEXT("Width"), m_rcWindowClient.right);
		DXUtil_WriteIntRegKey(hkey, TEXT("Height"), m_rcWindowClient.bottom);

		RegCloseKey(hkey);
	}
}

//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CAppForm::ConfirmDevice(D3DCAPS9* pCaps, DWORD dwBehavior,
	D3DFORMAT Format)
{
	BOOL bCapsAcceptable;

	bCapsAcceptable = TRUE;

	if (bCapsAcceptable)
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
	// Init the font
	m_pFont->InitDeviceObjects(m_pd3dDevice);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::RestoreDeviceObjects()
{
	// Set up the textures
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// Set miscellaneous render states

	m_pd3dDevice->BeginStateBlock();
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->EndStateBlock(&g_pBlendStateBlock);

	m_pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	m_pd3dDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pd3dDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	g_pBlendStateBlock->Apply();

	// Set the world matrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	D3DXMATRIX matView;
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(400.0f, 300.0f, 0.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(400.0f, 300.0f, 1.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Set the projection matrix
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, 800.0f, 600.0f, 0.0f, 3000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	SAFE_RELEASE(m_pSwapChain1);
	SAFE_RELEASE(m_pSwapChain2);

	m_pd3dDevice->GetSwapChain(0, &m_pSwapChain1);
	m_pd3dDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pSwapChain2);

	// Restore the font
	m_pFont->RestoreDeviceObjects();

	if (g_pGCDeviceManager)
		g_pGCDeviceManager->OnResetDevice();

	return S_OK;
}
DWORD CAppForm::dwLastFrame = (DWORD)0;

//-----------------------------------------------------------------------------
// Name: CAppForm::FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::FrameMove()
{
	const DWORD OneFrameTime = 1000 / 55;
	DWORD CurTime = timeGetTime();

	// 55프레임 맞추는 코드
	while (CurTime - dwLastFrame > OneFrameTime)
	{
		m_dwTotalFrame++;
		dwLastFrame += OneFrameTime;

		if (m_bScriptLoaded == false)
			break;

		SiKGridGuideMgr()->FrameMove();
		//SiKGCEffectManager()->FrameMove( GC_ELAPSED_TIME, 0 );
		SiKGCMap()->FrameMove(GC_ELAPSED_TIME);

		CLayerPart* pPart = m_pLayerList->GetCurItemData();
		if (pPart != NULL && !m_RenderView.GetAltPushed())
			SetBoxObj(pPart);

		// FrameMove에서 해야할 일을.. 커서는 레이어가 없다 ㅠ.ㅠ
		std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
		if (!vecLayer.empty() && m_iLayerIndex >= 0 && m_RenderView.GetCursor() != NULL)
		{
			float LayerXSize = vecLayer[m_iLayerIndex]->GetLayerXSize();
			float LayerYSize = vecLayer[m_iLayerIndex]->GetLayerYSize();

			float PlayingWidth = (float)SiKGCMap()->GetPixelWidth();
			float PlayingHeight = (float)SiKGCMap()->GetPixelHeight();

			float ScrollX = 0.0f;
			float ScrollY = 0.0f;
			float cwx = m_RenderView.GetEyeX() - WOG_WIDTH * 0.5f;
			float cwy = m_RenderView.GetEyeY() - WOG_HEIGHT * 0.5f;
			ScrollX = ((LayerXSize - WOG_WIDTH) / (PlayingWidth - WOG_WIDTH)) * cwx;
			ScrollY = ((LayerYSize - WOG_HEIGHT) / (PlayingHeight - WOG_HEIGHT)) * cwy;

			if (LayerXSize == 800)
				ScrollX = 0.0f;
			if (LayerYSize == 600)
				ScrollY = 0.0f;

			if (m_RenderView.GetCursor() != NULL)
				m_RenderView.SetCursorPos();

			m_RenderView.GetCursor()->SetScrollPos(-ScrollX + cwx, -ScrollY + cwy);
		}

		SetMonitorVertex(m_RenderView.GetEyeX() - 400.0f, m_RenderView.GetEyeY() - 300.0f, m_RenderView.GetEyeX() + 400.0f, m_RenderView.GetEyeY() + 300.0f);
	}

	if (g_bFootHoldImaging == true)
	{
		TakeFootHoldImage();
	}
	return S_OK;
}

void CAppForm::TakeFootHoldImage()
{
	static int iProceedCount = 0;
	static int iTakePicture = 0;
	static std::stringstream stm;
	static float fEyeX = 300.0f;
	static float fEyeY = 300.0f;

	if (iProceedCount == 0)
	{
		g_AppFormView->FootHoldImageingPrepare();
		iProceedCount++;
	}
	else
	{
		if (iProceedCount % 2 == 0)
		{
			stm.str("");
			stm << g_AppFormView->GetScriptName() << "_" << iTakePicture << ".jpg";
			g_AppFormView->GetRenderView()->MakeFootHoldImage(stm.str().c_str());
			iTakePicture++;

			fEyeX += 600.0f;
			if (fEyeX >= SiKGCMap()->GetPixelWidth() + 300.0f)
			{
				fEyeY += 600.0f;
				fEyeX = 300.0f;
			}

			if (fEyeY >= SiKGCMap()->GetPixelHeight() + 300.0f)
			{
				iProceedCount = 0;
				FootHoldImageingRestore();
				g_bFootHoldImaging = false;
				iTakePicture = 0;
				fEyeX = 300.0f;
				fEyeY = 300.0f;
				return;
			}
		}
		else if (iProceedCount % 2 == 1)
		{
			g_AppFormView->GetRenderView()->MakeFootHoldImagePrepare(fEyeX, fEyeY);
		}
		iProceedCount++;
	}
}

void CAppForm::RenderFH()
{
	CToolObject::PreRenderProcess();
	std::vector< CToolObject* > vecHead;
	int ScreenWidthMin = (int)((m_RenderView.GetEyeX() - m_RenderView.GetScreenWidth() * 0.6f) / FOOTHOLDSIZE);
	int ScreenWidthMax = (int)((m_RenderView.GetEyeX() + m_RenderView.GetScreenWidth() * 0.6f) / FOOTHOLDSIZE);
	int ScreenHeightMin = (int)((m_RenderView.GetEyeY() - m_RenderView.GetScreenHeight() * 0.6f) / FOOTHOLDSIZE);
	int ScreenHeightMax = (int)((m_RenderView.GetEyeY() + m_RenderView.GetScreenHeight() * 0.6f) / FOOTHOLDSIZE);
	int BlockWidth = SiKGCMap()->GetBlockWidth();
	int BlockHeight = SiKGCMap()->GetBlockHeight();

	if (m_ppTObj != NULL)
	{
		for (int i = ScreenWidthMin; i <= ScreenWidthMax; ++i)
		{
			for (int j = ScreenHeightMin; j <= ScreenHeightMax; ++j)
			{
				if (i < 0 || j < 0)
					continue;

				if (i >= BlockWidth || j >= BlockHeight)
					continue;

				//m_ppTObj[i][j].Render();

				if (m_ppTObj[i][j].GetType() == CFootHold::FHT_HEAD)
				{
					CToolObject* pObj = &m_ppTObj[i][j];

					int k;
					for (k = 0; k < (int)vecHead.size(); ++k)
					{
						if (pObj == vecHead[k])
							break;
					}
					// 없으면 추가
					if (k == (int)vecHead.size())
						vecHead.push_back(&m_ppTObj[i][j]);
				}
			}
		}
	}

	KGridGuide::PreRenderProcess();
	SiKGridGuideMgr()->RenderFH();
	SiKGridGuideMgr()->Render();
	m_RenderView.RenderCurGridGuide();
	KGridGuide::PostRenderProcess();

	for (int i = 0; i < (int)vecHead.size(); ++i)
	{
		vecHead[i]->Render();
	}
	SiCLayerManager()->RenderHeadFH();

	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		if ((m_vecCurObj[i]->GetIndexX() > ScreenWidthMin) &&
			((m_vecCurObj[i]->GetIndexX() + 1) < ScreenWidthMax) &&
			(m_vecCurObj[i]->GetIndexY() > ScreenHeightMin) &&
			((m_vecCurObj[i]->GetIndexY() + 1) < ScreenHeightMax))
		{
			m_vecCurObj[i]->Render();
		}
	}
	CToolObject::PostRenderProcess();

	RenderCursorIndex();
}

#define TO_INT(p) static_cast<int>( static_cast<int>( ( p + FOOTHOLDSIZE*0.5 ) / FOOTHOLDSIZE ) * FOOTHOLDSIZE )
#define TO_INT_INDEX(p) static_cast<int>( p / FOOTHOLDSIZE )

void CAppForm::RenderCursorIndex()
{
	if (g_bFootHoldImaging == true)
		return;

	D3DCOLOR Color = 0xff00ffff;
	std::wstringstream stm;
	D3DVECTOR MPos;
	m_RenderView.Get3DWorldPos(&MPos);
	stm << TO_INT_INDEX(MPos.x) << L", " << TO_INT_INDEX(MPos.y);

	D3DXVECTOR3 vPos;
	D3DXMATRIX matView, matProj, matWorld;
	memset(&matView, 0, sizeof(matView));
	memset(&matProj, 0, sizeof(matProj));
	D3DVIEWPORT9 vp;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->GetViewport(&vp);
	g_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	vPos = D3DXVECTOR3(MPos.x, MPos.y, 0.3f);
	D3DXVec3Project(&vPos, &vPos, &vp, &matProj, &matView, &matWorld);
	m_pkFont->OutTextXY((int)(vPos.x + 20.0f), (int)(vPos.y + 20.0f), stm.str().c_str(), Color);
}

//-----------------------------------------------------------------------------
// Name: CAppForm::Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::Render()
{
	EnterCriticalSection(&m_csSwap);
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	m_pSwapChain1->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pd3dDevice->SetRenderTarget(0, pBackBuffer);

	// Clear the viewport
	switch (m_eMode)
	{
	case WOGMAPTOOLMODE::MODE_FH:
	{
		m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x00000000, 1.0f, 0L);
	}
	break;
	case WOGMAPTOOLMODE::MODE_LAYER:
	{
		m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x000000ff, 1.0f, 0L);
	}
	break;
	}

	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		KGCMap* pMap = SiKGCMap();

		switch (m_eMode)
		{
		case WOGMAPTOOLMODE::MODE_FH:
		{
			if (m_bScriptLoaded == true && m_bFootHoldImaging == false)
			{
				pMap->Render();
			}
			RenderFH();
			break;
		}
		case WOGMAPTOOLMODE::MODE_LAYER:
		{
			if (m_bScriptLoaded == true)
			{
				pMap->Render();
			}
			if (m_bRenderFH)
			{
				RenderFH();
			}

			if (m_bScriptLoaded == true)
			{
				if (m_RenderView.GetCursor() != NULL)
					m_RenderView.GetCursor()->Render();

				g_pd3dDevice->SetTexture(0, NULL);
				g_pd3dDevice->SetFVF(WorldVertex::FVF);
				g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, m_Vertex, sizeof(WorldVertex));

				POSITION CurLayerSel = m_pLayerList->GetCurLayerPos();
				if (CurLayerSel != NULL)
				{
					g_pd3dDevice->SetTexture(0, NULL);
					g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, m_CurLayerVertex, sizeof(WorldVertex));
				}

				CLayerPart* pPart = m_pLayerList->GetCurItemData();
				if (pPart != NULL)
				{
					g_pd3dDevice->SetTexture(0, NULL);
					g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, m_CurObjVertex, sizeof(WorldVertex));
				}

				g_pd3dDevice->SetTexture(0, NULL);
				g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, m_MonitorVertex, sizeof(WorldVertex));
			}
		}
		break;
		}

		// Render stats and help text
		if (m_bFootHoldImaging == false)
		{
			RenderText();
		}

		// End the scene.
		m_pd3dDevice->EndScene();
	}
	//m_pSwapChain1->Present( NULL, NULL, m_hwndRenderWindow, NULL, 0 );

	pBackBuffer->Release();
	LeaveCriticalSection(&m_csSwap);
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RenderText()
// Desc: Renders stats and help text to the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::RenderText()
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 0);
	D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255, 0, 255, 255);
	TCHAR szMsg[MAX_PATH] = TEXT("");

	// Output display stats
	FLOAT fNextLine = 60.0f;

	lstrcpy(szMsg, m_strDeviceStats);
	fNextLine -= 20.0f;
	m_pFont->DrawText(2, fNextLine, fontColor, szMsg);

	lstrcpy(szMsg, m_strFrameStats);
	fNextLine -= 20.0f;
	m_pFont->DrawText(2, fNextLine, fontColor, szMsg);

	static std::string strFHMode = "Press F2 Key To Layer Mode";
	static std::string strLayerMode = "Press F1 Key To FootHold Mode";

	switch (m_eMode)
	{
	case WOGMAPTOOLMODE::MODE_FH:
		lstrcpy(szMsg, strFHMode.c_str());
		break;
	case WOGMAPTOOLMODE::MODE_LAYER:
		lstrcpy(szMsg, strLayerMode.c_str());
		break;
	}

	fNextLine -= 20.0f;
	m_pFont->DrawText(2, fNextLine, fontColor, szMsg);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CAppForm::Pause(BOOL bPause)
{
	// Get access to the list of semantically-mapped input devices
	// to zero the state of all InputDeviceState structs.  This is needed
	// because when using DISCL_FOREGROUND, the action mapper will not
	// record actions when the focus switches, for example if a dialog appears.
	// This causes a problem when a button held down when loosing focus, and let
	// go when the focus is lost.  The app will not record that the button
	// has been let go, so the state will be incorrect when focus returns.
	// To fix this either use DISCL_BACKGROUND or zero the state when
	// loosing focus.

	CD3DApplication::Pause((bool)bPause);
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
	DDX_Control(pDX, IDC_RENDERVIEW, m_RenderView);
	DDX_Control(pDX, IDC_LOG_LIST, m_lcLog);
	DDX_Control(pDX, IDC_H_GUIDELINE, m_btnHorizonGuide);
	DDX_Control(pDX, IDC_V_GUIDELINE, m_btnVerticalGuide);
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc:
//-----------------------------------------------------------------------------
LRESULT CAppForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CFormView::WindowProc(message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: OnChangeDevice()
// Desc: Needed to enable dlg menu item
//-----------------------------------------------------------------------------
void CAppFrameWnd::OnChangeDevice()
{
}

//-----------------------------------------------------------------------------
// Name: CAppForm::InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CAppForm::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pSwapChain2);
	SAFE_RELEASE(m_pSwapChain1);

	m_pFont->InvalidateDeviceObjects();

	if (g_pGCDeviceManager)
		g_pGCDeviceManager->OnLostDevice();

	SAFE_RELEASE(g_pBlendStateBlock);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::DeleteDeviceObjects()
{
	m_pFont->DeleteDeviceObjects();

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CAppForm::FinalCleanup()
{
	// Cleanup D3D font
	SAFE_DELETE(m_pFont);

	// Write the settings to the registry
	WriteSettings();

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
	AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CAppDoc),
		RUNTIME_CLASS(CAppFrameWnd),
		RUNTIME_CLASS(CAppForm)));

	// Dispatch commands specified on the command line (req'd by MFC). This
	// also initializes the the CAppDoc, CAppFrameWnd, and CAppForm classes.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	g_AppFormView->GetParentFrame()->RecalcLayout();
	g_AppFormView->ResizeParentToFit(FALSE);

	if (g_AppFormView->GetFileName().compare("") != 0)
	{
		m_pMainWnd->SetWindowText(g_AppFormView->GetFileName().c_str());
	}
	else
	{
		m_pMainWnd->SetWindowText(g_strAppTitle);
	}
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

	LoadAccelTable(MAKEINTRESOURCE(IDR_MAIN_ACCEL));

	return bResult;
}

//-----------------------------------------------------------------------------
// Name: OnIdle()
// Desc: Uses idle time to render the 3D scene.
//-----------------------------------------------------------------------------
BOOL CApp::OnIdle(LONG)
{
	// Do not render if the app is minimized
	if (m_pMainWnd->IsIconic())
		return FALSE;

	TCHAR strStatsPrev[200];

	lstrcpy(strStatsPrev, g_AppFormView->PstrFrameStats());

	// Update and render a frame
	g_AppFormView->RenderScene();

	// Keep requesting more idle time
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: PreCreateWindow()
// Desc: Change the window style (so it cannot maximize or be sized) before
//       the main frame window is created.
//-----------------------------------------------------------------------------
BOOL CAppFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	return CFrameWnd::PreCreateWindow(cs);
}

//-----------------------------------------------------------------------------
// Name: ~CAppForm()
// Desc: Destructor for the dialog resource form. Shuts down the app
//-----------------------------------------------------------------------------
CAppForm::~CAppForm()
{
	//g_kLuabinder.Destroy();
	if (m_ppTObj != NULL)
	{
		for (int i = 0; i < SiKGCMap()->GetBlockWidth(); i++)
		{
			if (m_ppTObj[i] != NULL)
				SAFE_DELETE_ARRAY(m_ppTObj[i]);
		}
	}

	SAFE_DELETE_ARRAY(m_ppTObj);

	ClearCurObj();

	CLayerManager::ReleaseInstance();
	KGCMap::ReleaseInstance();
	KGridGuideMgr::ReleaseInstance();
	//SiKGCEffectManager()->ReleaseInstance();
	SAFE_DELETE(m_pCommandRecv);

	DestroyModeless();
	SAFE_DELETE(g_pGCDeviceManager);

	SAFE_RELEASE(m_pSwapChain1);
	SAFE_RELEASE(m_pSwapChain2);

	Cleanup3DEnvironment();
	SAFE_DELETE(m_pFont);

	DeleteCriticalSection(&m_csSwap);
}

//-----------------------------------------------------------------------------
// Name: UpdateUIForDeviceCapabilites()
// Desc: Whenever we get a new device, call this function to enable/disable the
//       appropiate UI controls to match the device's capabilities.
//-----------------------------------------------------------------------------
VOID CAppForm::UpdateUIForDeviceCapabilites()
{
	DWORD dwCaps = m_d3dCaps.RasterCaps;
}

void CAppForm::AddMassFile(const std::string& strFileName_)
{
	// g_pGCDeviceManager->GetMassFileManager()->AddMassFile("./Stage/script.kom");

	// [1/18/2008] breadceo. 이거
	//std::string strFile_ = strFileName_;
	//boost::to_upper( strFile_ );

	//CFileFind find;
	//BOOL bWorking = find.FindFile( strFile_.c_str() );

	//while( bWorking )
	//{
	//    bWorking = find.FindNextFile();

	//    if( find.IsDots() )
	//        continue;

	//    if( find.IsDirectory() )
	//    {
	//        AddMassFile( find.GetFilePath().GetBuffer() );
	//    }
	//    else
	//    {
	//        std::string strFile = find.GetFileName().GetBuffer();
	//        boost::to_upper( strFile );
	//        if( strFile.find_first_of("MAP") != (std::string::size_type)0 )
	//            continue;
	//        //GCMassFile* pMassFile =
	//        g_pGCDeviceManager->GetMassFileManager()->AddMassFile( find.GetFilePath().GetBuffer() );
	//        //if( pMassFile != NULL )
	//        //{
	//        //    START_LOG( clog, strFile );
	//        //}
	//        //else
	//        //{
	//        //    START_LOG( cerr, strFile );
	//        //}
	//    }
	//}

	// [1/18/2008] breadceo. 구조 변경하기 전에는 어쩔수 없음
	//KLuaManager	luaMgr;
	//GCMemoryFile* pMemFile = g_pGCDeviceManager->GetMassFileManager()->LoadMemoryFile( "DataFiles.lua" );
	//if( pMemFile == NULL )
	//    return;

	//if( FAILED( luaMgr.DoString( pMemFile->GetDataPointer() ) ) )
	//    return;

	//LUA_BEGIN_TABLE( "DataFilesKorea", return )
	//{
	//    std::string strFileName;
	//    for( int i = 1; ; ++i )
	//    {
	//        LUA_GET_VALUE_NOASSERT( i, strFileName, break );
	//        if( g_pGCDeviceManager->GetMassFileManager()->AddMassFile( strFileName.c_str() ) == NULL )
	//            continue;
	//    }
	//}
	//LUA_END_TABLE( return );
}

//-----------------------------------------------------------------------------
// Name: OnInitialUpdate()
// Desc: When the AppForm object is created, this function is called to
//       initialize it. Here we getting access ptrs to some of the controls,
//       and setting the initial state of some of them as well.
//-----------------------------------------------------------------------------
VOID CAppForm::OnInitialUpdate()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF);

	// Update the UI
	CFormView::OnInitialUpdate();

	// Load Accelerators
	m_hKey = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// Get the top level parent hwnd
	m_hWndTopLevelParent = GetTopLevelParent()->GetSafeHwnd();

	// Save static reference to the render window
	m_hwndRenderWindow = GetDlgItem(IDC_RENDERVIEW)->GetSafeHwnd();

	// Note that for the MFC samples, the device window and focus window
	 // are not the same.
	CD3DApplication::m_hWnd = m_hwndRenderWindow;
	CD3DApplication::Create(AfxGetInstanceHandle());

	// Set the world matrix
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	D3DXMATRIX matView;
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(400.0f, 300.0f, 0.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(400.0f, 300.0f, 1.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Set the projection matrix
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, 800.0f, 600.0f, 0.0f, 3000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	GetCurrentDirectoryA(MAX_PATH, g_Path);

	g_pGCDeviceManager = new GCDeviceManager(m_pd3dDevice);
	g_pd3dDevice = m_pd3dDevice;

	std::string strPath = g_Path;
	std::string strTexDir;
	strTexDir = strPath + "\\Texture\\";
	g_pGCDeviceManager->GetMassFileManager()->AddDirectory(strTexDir);

	CFileFind finder;

	// build a string with wildcards
	CString strWildcard;
	strWildcard = strTexDir.c_str();
	strWildcard += _T("\*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			str += _T("\\");
			CT2CA convert(str);
			std::string temp(convert);
			if (std::string::npos != temp.find(".svn"))
				continue;

			g_pGCDeviceManager->GetMassFileManager()->AddDirectory(temp);
		}
	}

	finder.Close();

	std::string strScriptDir;
	strScriptDir = strPath + "\\Script\\";
	g_pGCDeviceManager->GetMassFileManager()->AddDirectory(strScriptDir);

	//strDataDir = strPath + "\\Data\\Effect\\";
	//g_pGCDeviceManager->GetMassFileManager()->AddDirectory( strDataDir );

	g_pGCDeviceManager->GetMassFileManager()->AddMassFile("./Texture/ForTool.kom");
	AddMassFile(strTexDir + "*.kom");

	m_pkFont = g_pGCDeviceManager->CreateFontA("GULIMCHE", GCDeviceFontBase::DEFAULT_FONT_SIZE);
	SiKGridGuideMgr();

	std::string strShader = g_Path;
	strShader += "\\GrandChase.fx";
	g_pRenderState = g_pGCDeviceManager->CreateShader(strShader.c_str());
	if (g_pRenderState->SetTechnique("RenderState") == false)
		AfxMessageBox("g_pRenderState->SetTechnique( ""RenderState"" ) 실패");

	g_matViewProj = matView * matProj;

	//SiKGCEffectManager()->Init( "Data/Script/EffectHeader.lua", "Data/Script/Effect.lua" );

	m_eMode = WOGMAPTOOLMODE::MODE_LAYER;

	CreateModaless();

	m_lcLog.InsertColumn(0, "Log", LVCFMT_LEFT, 800);
	m_lcLog.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 최근 로딩한 파일이 있으면
	CFileFind fileFind;
	if (m_strFileName.compare("") != 0 && fileFind.FindFile(m_strFileName.c_str()) != 0)
	{
		std::string strMessage;
		strMessage = "최근 작업한 파일 : ";
		strMessage += m_strFileName;
		strMessage += "\n이 있습니다 로딩하시겠습니까?";

		int nRet = AfxMessageBox(strMessage.c_str(), MB_YESNO);
		switch (nRet)
		{
		case IDYES:
			FileLoad(m_strFileName);
			break;
		case IDNO:
			m_strFileName = "";
			break;
		}
	}
	dwLastFrame = ::timeGetTime();
}

void CAppForm::ArrangeModaless(CRect& rtMain)
{
	CRect rtTemp;
	m_pPartList->GetWindowRect(&rtTemp);
	rtTemp.MoveToXY(CPoint(rtMain.right + 20, rtMain.top));
	m_pPartList->MoveWindow(&rtTemp, TRUE);

	CRect rtTemp7;
	m_pFHValue->GetWindowRect(&rtTemp7);
	rtTemp7.MoveToXY(CPoint(rtMain.right + 20, rtMain.top));
	m_pFHValue->MoveWindow(&rtTemp7, TRUE);

	CRect rtTemp2;
	m_pLayerList->GetWindowRect(&rtTemp2);
	rtTemp2.MoveToXY(CPoint(rtMain.right + 20, rtTemp.bottom + 10));
	m_pLayerList->MoveWindow(&rtTemp2, TRUE);

	CRect rtTemp3;
	m_pUserProp->GetWindowRect(&rtTemp3);
	rtTemp3.MoveToXY(CPoint(rtMain.left, rtMain.bottom + 110));
	m_pUserProp->MoveWindow(&rtTemp3, TRUE);

	CRect rtTemp4;
	m_pLayerProp->GetWindowRect(&rtTemp4);
	rtTemp4.MoveToXY(CPoint(rtTemp3.right + 70, rtMain.bottom + 110));
	m_pLayerProp->MoveWindow(&rtTemp4, TRUE);

	CRect rtTemp5;
	m_pMoveProp->GetWindowRect(&rtTemp5);
	rtTemp5.MoveToXY(CPoint(rtTemp4.right + 70, rtMain.bottom + 110));
	m_pMoveProp->MoveWindow(&rtTemp5, TRUE);

	m_pUserProp->ShowWindow(SW_SHOW);
	m_pPartList->ShowWindow(SW_SHOW);
	m_pLayerList->ShowWindow(SW_SHOW);
	m_pLayerProp->ShowWindow(SW_SHOW);
	m_pMoveProp->ShowWindow(SW_SHOW);
	m_pFHValue->ShowWindow(SW_SHOW);
}

#define CREATE_MODALESS(p, q, r)    do\
                                    {\
                                        if( p == NULL )\
                                        {\
                                            p = new q();\
                                            p->Create( r, this );\
                                        }\
                                        else\
                                        {\
                                            p->SetFocus();\
                                        }\
                                    } while(0);
#define DELETE_MODALESS(p)  do\
                            {\
                                p->DestroyWindow();\
                                SAFE_DELETE(p);\
                            } while(0);

void CAppForm::CreateModaless()
{
	// Prop Dlg들 생성

	CREATE_MODALESS(m_pUserProp, CUserProp, IDD_USER_PROP);
	CREATE_MODALESS(m_pPartList, CPartList, IDD_PARTLIST);
	CREATE_MODALESS(m_pLayerList, CLayerList, IDD_LAYERLIST);
	CREATE_MODALESS(m_pLayerProp, CLayerProp, IDD_LAYER_SIZE);
	CREATE_MODALESS(m_pMoveProp, CMoveProp, IDD_MOVE_PROP);
	CREATE_MODALESS(m_pFHValue, CFootHoldValue, IDD_FHVALUE);
}

void CAppForm::DestroyModeless()
{
	DELETE_MODALESS(m_pUserProp);
	DELETE_MODALESS(m_pPartList);
	DELETE_MODALESS(m_pLayerList);
	DELETE_MODALESS(m_pLayerProp);
	DELETE_MODALESS(m_pMoveProp);
	DELETE_MODALESS(m_pFHValue);
}

void CAppForm::InitDungeon(int Width, int Height)
{
	if (m_ppTObj != NULL)
		return;

	m_ppTObj = new CToolObject*[Width];
	for (int i = 0; i < Width; i++)
		m_ppTObj[i] = new CToolObject[Height];

	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			m_ppTObj[i][j].SetBlockInfo(static_cast<CFHBlockInfo*>(SiKGCMap()));
			m_ppTObj[i][j].SetEyeInfo(static_cast<CRenderEyeInfo*>(&m_RenderView));
			m_ppTObj[i][j].SetPos(i, j, CFootHold::FHT_INIT);
		}
	}
}

void CAppForm::DeleteDungeon()
{
	if (m_ppTObj != NULL)
	{
		for (int i = 0; i < SiKGCMap()->GetBlockWidth(); i++)
		{
			if (m_ppTObj[i] != NULL)
				SAFE_DELETE_ARRAY(m_ppTObj[i]);
		}
	}

	SAFE_DELETE_ARRAY(m_ppTObj);
}

void CAppForm::OnLayermanagerCreatelayerpart()
{
	KCreateLayerPartDlg LayerTool;
	LayerTool.SetSwapChain(m_pSwapChain2);

	INT_PTR nRet = -1;
	nRet = LayerTool.DoModal();

	dwLastFrame = ::timeGetTime();
	m_RenderView.RestoreView();

	switch (nRet)
	{
	case -1:
		AfxMessageBox("LayerTool Cannot be Create!!");
		break;
	case IDOK:
	{
		// 다시 로드!
		m_pPartList->UpdateCategoryList();
	}
	break;
	case IDCANCEL:
		break;
	}
}

void CAppForm::OnLayermanagerEditlayer()
{
	KCChooseCategoryDlg ChooseTool;
	INT_PTR nRet = -1;
	nRet = ChooseTool.DoModal();

	switch (nRet)
	{
	case -1:
		AfxMessageBox("ChooseTool Cannot be Create!!");
		break;
	case IDOK:
		// 저장
		break;
	case IDCANCEL:
		break;
	}
}

BOOL CAppForm::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_RENDERVIEW:
	{
		if (m_RenderView.GetCursor() != NULL)
		{
			SetCursor(NULL);
			return TRUE;
		}
		else if (m_RenderView.EnableHand() == true)
		{
			HCURSOR hCursor;
			hCursor = AfxGetApp()->LoadCursor(IDC_MOVE_CURSOR);
			SetCursor(hCursor);
			return TRUE;
		}
		else
		{
			D3DVECTOR MPos;
			m_RenderView.Get3DWorldPos(&MPos);
			//if( kGrid.IsMouseOver( MPos ) ==  true )
			//{
			//    if( kGrid.GetVertical() == false )
			//    {
			//        HCURSOR hCursor;
			//        hCursor = AfxGetApp()->LoadCursor( IDC_V_GRID );
			//        SetCursor( hCursor );
			//        return TRUE;
			//    }
			//}
			//else if( kGrid2.IsMouseOver( MPos ) ==  true )
			//{
			//    if( kGrid2.GetVertical() == true )
			//    {
			//        HCURSOR hCursor;
			//        hCursor = AfxGetApp()->LoadCursor( IDC_H_GRID );
			//        SetCursor( hCursor );
			//        return TRUE;
			//    }
			//}
		}
	}
	break;
	}
	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CAppForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAppForm::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CAppForm::PreTranslateMessage(MSG* pMsg)
{
	static DWORD dwInputTime = ::timeGetTime();

	m_RenderView.SetEnableHand(GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000);
	m_RenderView.SetAltPushed(GetAsyncKeyState(VK_LMENU) & 0x8000 || GetAsyncKeyState(VK_LMENU) & 0x8000);

	//if( m_eMode == WOGMAPTOOLMODE::MODE_FH )
	m_RenderView.SetToolObjectAdd(GetAsyncKeyState(VK_CONTROL) & 0x8000);
	//else
		//m_RenderView.SetToolObjectAdd( false );

	BOOL bInput = FALSE;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		if (::timeGetTime() - dwInputTime > KEY_INTERVAL)
		{
			if (GetAsyncKeyState(VK_ADD) & 0x8000)
			{
				m_RenderView.QuaterZoom(true);
				dwInputTime = ::timeGetTime();
				bInput = TRUE;
			}

			if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
			{
				m_RenderView.QuaterZoom(false);
				dwInputTime = ::timeGetTime();
				bInput = TRUE;
			}

			// C
			if (GetAsyncKeyState(0x43) & 0x8000)
			{
				m_pLayerList->CopyObject();
				dwInputTime = ::timeGetTime();
				bInput = TRUE;
			}
			// V
			else if (GetAsyncKeyState(0x56) & 0x8000)
			{
				m_pLayerList->PasteCurObject();
				dwInputTime = ::timeGetTime();
				bInput = TRUE;
			}
		}
	}

	if (::timeGetTime() - dwInputTime > KEY_INTERVAL && GetAsyncKeyState(0x46) & 0x8000)      // F
	{
		dwInputTime = ::timeGetTime();
		bInput = TRUE;
		m_bRenderFH = !m_bRenderFH;
	}

	switch (m_eMode)
	{
	case WOGMAPTOOLMODE::MODE_FH:
	{
		if (::timeGetTime() - dwInputTime > KEY_INTERVAL && !m_vecCurObj.empty())
		{
			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000)
			{
				if (GetAsyncKeyState(VK_END) & 0x8000)
				{
					for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
					{
						m_vecCurObj[i]->SetProp(CFootHold::FHP_DEAD_FOOTHOLD);
						dwInputTime = ::timeGetTime();
						bInput = TRUE;
					}
				}
				else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
					{
						m_vecCurObj[i]->SetProp(CFootHold::FHP_NO_COME_RIGHT);
						dwInputTime = ::timeGetTime();
						bInput = TRUE;
					}
				}
				else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
					{
						m_vecCurObj[i]->SetProp(CFootHold::FHP_NO_COME_LEFT);
						dwInputTime = ::timeGetTime();
						bInput = TRUE;
					}
				}
				else if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
					{
						m_vecCurObj[i]->SetProp(CFootHold::FHP_NO_JUMP_DOWN);
						dwInputTime = ::timeGetTime();
						bInput = TRUE;
					}
				}
				//else if( GetAsyncKeyState(VK_DOWN) & 0x8000 )
				//{
				//    for( int i = 0; i < (int)m_vecCurObj.size(); ++i )
				//    {
				//        m_vecCurObj[i]->SetProp( CFootHold::FHP_NO_JUMP_UP );
				//        dwInputTime = ::timeGetTime();
				//        bInput = TRUE;
				//    }
				//}
			}
			else
			{
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					CToolObject* pTemp = m_vecCurObj[0];
					ClearCurObj(false);
					pTemp->MoveRight();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(pTemp);
				}
				else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					CToolObject* pTemp = m_vecCurObj[0];
					ClearCurObj(false);
					pTemp->MoveLeft();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(pTemp);
				}

				if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					CToolObject* pTemp = m_vecCurObj[0];
					ClearCurObj(false);
					pTemp->MoveUp();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(pTemp);
				}
				else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					CToolObject* pTemp = m_vecCurObj[0];
					ClearCurObj(false);
					pTemp->MoveDown();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(pTemp);
				}
			}

			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				StampObject();
				dwInputTime = ::timeGetTime();
				bInput = TRUE;
			}

			if (GetAsyncKeyState(VK_DELETE) & 0x8000)
			{
				UnStampObject();
				dwInputTime = ::timeGetTime();
				bInput = TRUE;
			}
		}
	}
	break;
	case WOGMAPTOOLMODE::MODE_LAYER:
	{
		CLayerPart* pPart = m_RenderView.GetDragPart();
		bool bKeyDown = false;
		if (pPart != NULL)
		{
			if (::timeGetTime() - dwInputTime > KEY_INTERVAL)
			{
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					CLayerPartKeyMoveCmd* pCmd = new CLayerPartKeyMoveCmd();
					pCmd->SetMove(CLayerPartKeyMoveCmd::RIGHT, pPart);
					pPart->MoveRight();
					dwInputTime = ::timeGetTime();
					bKeyDown = true;
					bInput = TRUE;
					m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
					g_AppFormView->AddLog(pCmd->GetLogStr());
				}
				else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					CLayerPartKeyMoveCmd* pCmd = new CLayerPartKeyMoveCmd();
					pCmd->SetMove(CLayerPartKeyMoveCmd::LEFT, pPart);
					pPart->MoveLeft();
					dwInputTime = ::timeGetTime();
					bKeyDown = true;
					bInput = TRUE;
					m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
					g_AppFormView->AddLog(pCmd->GetLogStr());
				}

				if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					CLayerPartKeyMoveCmd* pCmd = new CLayerPartKeyMoveCmd();
					pCmd->SetMove(CLayerPartKeyMoveCmd::UP, pPart);
					pPart->MoveUp();
					dwInputTime = ::timeGetTime();
					bKeyDown = true;
					bInput = TRUE;
					m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
					g_AppFormView->AddLog(pCmd->GetLogStr());
				}
				else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					CLayerPartKeyMoveCmd* pCmd = new CLayerPartKeyMoveCmd();
					pCmd->SetMove(CLayerPartKeyMoveCmd::DOWN, pPart);
					pPart->MoveDown();
					dwInputTime = ::timeGetTime();
					bKeyDown = true;
					bInput = TRUE;
					m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
					g_AppFormView->AddLog(pCmd->GetLogStr());
				}

				if (bKeyDown)
				{
					pPart->SetVertex();
					SetControlValue(pPart);
				}
			}
		}
	}
	break;
	}

	if (bInput == TRUE)
		return TRUE;

	if (TranslateAccelerator(AfxGetMainWnd()->m_hWnd, m_hKey, pMsg))
		return TRUE;

	return __super::PreTranslateMessage(pMsg);
}

void CAppForm::SetBoxObj(CLayerPart* pObj)
{
	if (pObj != NULL)
	{
		KLayerRect<float> kRect;
		float fScrollX, fScrollY;
		pObj->GetLayerRect(kRect);
		pObj->GetScrollPos(fScrollX, fScrollY);

		SetCurObjVertex(kRect.m_X + fScrollX, kRect.m_Y + fScrollY,
			kRect.m_X + fScrollX + (float)kRect.m_Width, kRect.m_Y + fScrollY + (float)kRect.m_Height);
	}
	else
	{
		SetCurObjVertex(0, 0, 0, 0);
	}
}

BOOL CAppForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CAppForm::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

void CAppForm::OnAppExit()
{
	if (IsNeedtoSave())
	{
		int nRet = AfxMessageBox("저장하시겠습니까?", MB_YESNO);
		switch (nRet)
		{
		case IDYES:
			// 저장하자
			OnFileSave();
			break;
		}
	}

	int nRet2 = AfxMessageBox("맵툴을 종료하시겠습니까", MB_YESNO);
	switch (nRet2)
	{
	case IDYES:
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
		break;
	case IDNO:
		return;
		break;
	}
}

void CAppForm::DisableAllProperty(BOOL bDisable)
{
	m_pUserProp->DisableAllProperty(bDisable);
	m_pLayerProp->DisableAllProperty(bDisable);
	m_pMoveProp->DisableAllProperty(bDisable);
	m_pLayerList->DisableAllProperty(bDisable);
}

void CAppForm::SetControlValue(CLayerPart* pPart)
{
	m_pUserProp->SetControlValue(pPart);
	m_pLayerProp->SetControlValue(pPart);
	m_pMoveProp->SetControlValue(pPart);

	SetBoxObj(pPart);
}

void CAppForm::UpdateControlValue()
{
	m_pUserProp->UpdateControlValue();
	m_pLayerProp->UpdateControlValue();
	m_pMoveProp->UpdateControlValue();
}

void CAppForm::UpdateSelectPart()
{
	CLayerPart* pPart = m_pLayerList->GetCurItemData();
	if (pPart == NULL)
		return;

	CModifyLayerPartCmd* pCmd = new CModifyLayerPartCmd();
	pCmd->StoreOrigPart(pPart);

	m_pUserProp->UpdateSelectPart(pPart);
	m_pLayerProp->UpdateSelectPart(pPart);
	m_pMoveProp->UpdateSelectPart(pPart);

	pCmd->SetOrigPart(pPart);
	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void CAppForm::OnAccelFileLoad()
{
	OnFileLoad();
}

void CAppForm::OnAccelExit()
{
	OnAppExit();
}

void CAppForm::OnFileLoad()
{
	SetCurrentDirectoryA(g_Path);
	char szFilters[] = "Lua Script Files (*.lua) |*.lua||";

	std::string strFolder = g_Path;
	strFolder += "/Stage/*.lua";

	CFileDialog dlg(TRUE, strFolder.c_str(), NULL, OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, AfxGetMainWnd());
	INT_PTR nRet = dlg.DoModal();

	if (nRet == IDOK)
	{
		FileLoad(dlg.GetPathName().GetBuffer());
	}

	SetCurrentDirectoryA(g_Path);
}

void CAppForm::OnAccelResetview()
{
	switch (m_eMode)
	{
	case WOGMAPTOOLMODE::MODE_FH:
		break;
	case WOGMAPTOOLMODE::MODE_LAYER:
		m_RenderView.ResetView();
		break;
	}
}

void CAppForm::OnFileSave()
{
	if (m_bScriptLoaded == false)
		return;

	if (m_strFileName.compare("") == 0)
		OnFileSaveAs();

	SetCurrentDirectoryA(g_Path);
	FileSave(m_strFileName);
	SetCurrentDirectoryA(g_Path);
}

void CAppForm::OnAccelFileSave()
{
	OnFileSave();
}

void CAppForm::OnFileClose()
{
	if (m_bScriptLoaded == false)
		return;

	if (IsNeedtoSave())
	{
		int nRet = AfxMessageBox("저장하시겠습니까?", MB_YESNO);
		switch (nRet)
		{
		case IDYES:
			// 저장하자
			OnFileSave();
			break;
		}
	}

	SiKGCMap()->UnLoad();
	SiCLayerManager()->RegisterLuabind();

	ClearCurObj();

	memset(m_Vertex, 0, sizeof(m_Vertex));
	memset(m_CurLayerVertex, 0, sizeof(m_CurLayerVertex));
	memset(m_MonitorVertex, 0, sizeof(m_MonitorVertex));
	memset(m_CurObjVertex, 0, sizeof(m_CurObjVertex));

	m_pPartList->ResetContent();
	m_pPartList->EnableWindow(FALSE);
	m_pLayerList->DeleteAllItems();
	m_RenderView.Clear();

	DisableAllProperty(TRUE);

	m_bScriptLoaded = false;

	g_AppFormView->GetSafeOwner()->SetWindowText(m_strFileName.c_str());
	g_AppFormView->GetSafeOwner()->UpdateWindow();

	ClearLog();
	AddLog("파일 닫기 성공");

	SiKGridGuideMgr()->clear();
}

bool CAppForm::IsNeedtoSave()
{
	bool bNeedToSave = false;

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	if (!vecLayer.empty())
	{
		for (int i = 0; i < SiCLayerManager()->GetMaxLayerNum(); ++i)
		{
			std::vector< CLayerPart* > PartVector = vecLayer[i]->GetPartVector();
			if (0 < (int)PartVector.size())
			{
				bNeedToSave = true;
				break;
			}
		}
	}

	if (m_ppTObj != NULL)
	{
		for (int i = 0; i < SiKGCMap()->GetBlockWidth(); ++i)
		{
			for (int j = 0; j < SiKGCMap()->GetBlockHeight(); ++j)
			{
				if (m_ppTObj[i][j].GetType() == CFootHold::FHT_HEAD)
				{
					bNeedToSave = true;
					break;
				}
			}
		}
	}

	return bNeedToSave;
}

void CAppForm::OnFileNew()
{
	if (IsNeedtoSave())
	{
		int nRet = AfxMessageBox("저장하시겠습니까?", MB_YESNO);
		switch (nRet)
		{
		case IDYES:
			// 저장하자
			OnFileSave();
			break;
		}
	}

	CNewMapDlg dlg;
	INT_PTR nRet = dlg.DoModal();

	switch (nRet)
	{
	case IDOK:
	{
		ClearCurObj();

		SiCLayerManager()->RegisterLuabind();

		m_pPartList->ResetContent();

		SetWorldVertex();

		m_pPartList->UpdateCategoryList();

		float right = SiKGCMap()->GetPixelWidth();
		float top = SiKGCMap()->GetPixelHeight();
		SiCLayerManager()->CreateEmptyDefaultLayer(right, top);

		m_pLayerList->ReloadLayerList();
	}
	break;
	case IDCANCEL:
		return;
	}

	CTime time = CTime::GetCurrentTime();

	std::stringstream stmNewFileName;
	stmNewFileName << g_Path << "\\Stage\\New" << "_"
		<< time.GetYear() << "년" << time.GetMonth() << "월" << time.GetDay() << "일_"
		<< time.GetHour() << "시" << time.GetMinute() << "분" << time.GetSecond() << "초.lua";

	m_strFileName = stmNewFileName.str();

	g_AppFormView->GetSafeOwner()->SetWindowText(m_strFileName.c_str());
	g_AppFormView->GetSafeOwner()->UpdateWindow();

	OnModeLayer();
	m_bScriptLoaded = true;
	ClearLog();
	std::stringstream stm;
	stm << "새 파일 " << m_strFileName << " 생성";
	AddLog(stm.str());

	SiKGridGuideMgr()->BuildFH();
	SiKGridGuideMgr()->BuildGridGuide();
}

void CAppForm::SetWorldVertex()
{
	float left = 0;
	float bottom = 0;
	float top;
	float right;

	right = (float)SiKGCMap()->GetPixelWidth();
	top = (float)SiKGCMap()->GetPixelHeight();

	m_Vertex[0].Set(left, bottom, 0.1f);
	m_Vertex[1].Set(left, top, 0.1f);
	m_Vertex[2].Set(left, top, 0.1f);
	m_Vertex[3].Set(right, top, 0.1f);
	m_Vertex[4].Set(right, top, 0.1f);
	m_Vertex[5].Set(right, bottom, 0.1f);
	m_Vertex[6].Set(right, bottom, 0.1f);
	m_Vertex[7].Set(left, bottom, 0.1f);
}

void CAppForm::SetCurLayerVertex(float left, float top, float right, float bottom)
{
	m_CurLayerVertex[0].Set(left, bottom, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[1].Set(left, top, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[2].Set(left, top, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[3].Set(right, top, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[4].Set(right, top, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[5].Set(right, bottom, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[6].Set(right, bottom, 0.1f, 0.0f, 0.0f, 0xff00ff00);
	m_CurLayerVertex[7].Set(left, bottom, 0.1f, 0.0f, 0.0f, 0xff00ff00);
}

void CAppForm::SetCurObjVertex(float left, float top, float right, float bottom)
{
	m_CurObjVertex[0].Set(left, bottom, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[1].Set(left, top, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[2].Set(left, top, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[3].Set(right, top, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[4].Set(right, top, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[5].Set(right, bottom, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[6].Set(right, bottom, 0.1f, 0.0f, 0.0f, 0xffff0000);
	m_CurObjVertex[7].Set(left, bottom, 0.1f, 0.0f, 0.0f, 0xffff0000);
}

void CAppForm::SetMonitorVertex(float left, float top, float right, float bottom)
{
	m_MonitorVertex[0].Set(left, bottom, 0.1f, 0.0f, 0.0f, 0xffffff00);
	m_MonitorVertex[1].Set(left, top, 0.1f, 0.0f, 0.0f, 0xffffff00);
	m_MonitorVertex[2].Set(left, top, 0.1f, 0.0f, 0.0f, 0xffffff00);
	m_MonitorVertex[3].Set(right, top, 0.1f, 0.0f, 0.0f, 0xffffff00);
	m_MonitorVertex[4].Set(right, top, 0.1f, 0.0f, 0.0f, 0xffffff00);
	m_MonitorVertex[5].Set(right, bottom, 0.1f, 0.0f, 0.0f, 0xffffFf00);
	m_MonitorVertex[6].Set(right, bottom, 0.1f, 0.0f, 0.0f, 0xffffff00);
	m_MonitorVertex[7].Set(left, bottom, 0.1f, 0.0f, 0.0f, 0xffffff00);
}

void CAppForm::OnAccelFileNew()
{
	OnFileNew();
}

void CAppForm::OnModeLayer()
{
	static bool s_bLayerShow = false;

	if (m_eMode == WOGMAPTOOLMODE::MODE_LAYER && m_bScriptLoaded == true)
	{
		ToggleLayerWindow(!s_bLayerShow);
		s_bLayerShow = !s_bLayerShow;
		return;
	}

	s_bLayerShow = true;
	m_eMode = WOGMAPTOOLMODE::MODE_LAYER;
	ClearCurObj();
	m_RenderView.ClearCurGridGuide();

	CRect rt;
	m_RenderView.GetWindowRect(&rt);
	ArrangeModaless(rt);

	ShowChildWindow(true);

	SetFocus();
}

void CAppForm::ToggleFHWindow(bool bShow)
{
	m_pFHValue->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	SetFocus();
}

void CAppForm::ToggleLayerWindow(bool bShow)
{
	m_pUserProp->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_pPartList->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_pLayerList->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_pLayerProp->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_pMoveProp->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	SetFocus();
}

void CAppForm::ShowChildWindow(bool bLayer)
{
	m_pUserProp->ShowWindow(bLayer ? SW_SHOW : SW_HIDE);
	m_pPartList->ShowWindow(bLayer ? SW_SHOW : SW_HIDE);
	m_pLayerList->ShowWindow(bLayer ? SW_SHOW : SW_HIDE);
	m_pLayerProp->ShowWindow(bLayer ? SW_SHOW : SW_HIDE);
	m_pMoveProp->ShowWindow(bLayer ? SW_SHOW : SW_HIDE);
	m_pFHValue->ShowWindow(!bLayer ? SW_SHOW : SW_HIDE);
	m_pFHValue->DisableAllProperty(bLayer);

	SetFocus();
}

void CAppForm::OnModeFh()
{
	static bool s_bFHShow = false;

	if (m_eMode == WOGMAPTOOLMODE::MODE_FH && m_bScriptLoaded == true)
	{
		ToggleFHWindow(!s_bFHShow);
		s_bFHShow = !s_bFHShow;
		return;
	}

	s_bFHShow = true;

	m_eMode = WOGMAPTOOLMODE::MODE_FH;
	m_RenderView.ClearCursor();
	m_RenderView.ClearDragPart();

	CRect rt;
	m_RenderView.GetWindowRect(&rt);
	ArrangeModaless(rt);

	ShowChildWindow(false);

	SetFocus();
}

void CAppForm::OnAccelModeFh()
{
	OnModeFh();
}

void CAppForm::OnAccelModeLayer()
{
	OnModeLayer();
}

void CAppForm::StampObject()
{
	if (0 == m_vecCurObj.size())
		return;

	CStampObjectCmd* pCmd = new CStampObjectCmd();
	pCmd->SetStampObj(m_vecCurObj);

	int ZeroIndexX;
	int ZeroIndexY;
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		if (i == 0)
		{
			ZeroIndexX = m_vecCurObj[i]->GetIndexX();
			ZeroIndexY = m_vecCurObj[i]->GetIndexY();
		}
		CToolObject* pObj = &m_ppTObj[m_vecCurObj[i]->GetIndexX()][m_vecCurObj[i]->GetIndexY()];
		pObj->SetPos(m_vecCurObj[i]->GetIndexX(), m_vecCurObj[i]->GetIndexY(), CFootHold::FHT_HEAD);
		pObj->SetProp(m_vecCurObj[i]->GetProp(), "OR");
	}

	ClearCurObj();

	CToolObject* pStart = new CToolObject(static_cast<CRenderEyeInfo*>(&m_RenderView), static_cast<CFHBlockInfo*>(SiKGCMap()));
	pStart->SetPos(ZeroIndexX, ZeroIndexY, CFootHold::FHT_HEAD);
	m_vecCurObj.push_back(pStart);

	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void CAppForm::UnStampObject()
{
	if (0 == m_vecCurObj.size())
		return;

	CUnStampObjectCmd* pCmd = new CUnStampObjectCmd();
	pCmd->SetUnStampObj(m_vecCurObj);

	int ZeroIndexX;
	int ZeroIndexY;
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		if (i == 0)
		{
			ZeroIndexX = m_vecCurObj[i]->GetIndexX();
			ZeroIndexY = m_vecCurObj[i]->GetIndexY();
		}
		CToolObject* pObj = &m_ppTObj[m_vecCurObj[i]->GetIndexX()][m_vecCurObj[i]->GetIndexY()];
		pObj->SetPos(m_vecCurObj[i]->GetIndexX(), m_vecCurObj[i]->GetIndexY(), CFootHold::FHT_INIT);
		pObj->SetProp(m_vecCurObj[i]->GetProp(), "INIT");
		pObj->SetValue("");
	}

	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void CAppForm::OnAccelFileClose()
{
	OnFileClose();
}

void CAppForm::OnWorldresize()
{
	if (m_bScriptLoaded == false)
		return;

	if (m_eMode == WOGMAPTOOLMODE::MODE_LAYER)
		AfxMessageBox("*************경   고*************\nLAYER 모드에서 사용하면 위험할 수 있습니다.", MB_OK | MB_ICONWARNING);

	CResizingDlg dlg;

	ResizedX = SiKGCMap()->GetBlockWidth();
	ResizedY = SiKGCMap()->GetBlockHeight();

	INT_PTR nRet = dlg.DoModal();

	switch (nRet)
	{
	case IDOK:
	{
		CWorldResizeCmd* pCmd = new CWorldResizeCmd();

		// 새로운 크기를 받았다.
		CToolObject** ppTObj;
		ppTObj = new CToolObject*[ResizedX];
		for (int i = 0; i < ResizedX; i++)
			ppTObj[i] = new CToolObject[ResizedY];

		for (int i = 0; i < ResizedX; ++i)
		{
			for (int j = 0; j < ResizedY; ++j)
			{
				ppTObj[i][j].SetBlockInfo(static_cast<CFHBlockInfo*>(SiKGCMap()));
				ppTObj[i][j].SetEyeInfo(static_cast<CRenderEyeInfo*>(&m_RenderView));
				ppTObj[i][j].SetPos(i, j, CFootHold::FHT_INIT);
			}
		}

		int BeforeX = SiKGCMap()->GetBlockWidth();
		int BeforeY = SiKGCMap()->GetBlockHeight();
		pCmd->SetOldWorld(BeforeX, BeforeY, m_ppTObj);

		// 작은 거만 골라서 복사
		int LoopX = BeforeX < ResizedX ? BeforeX : ResizedX;
		int LoopY = BeforeY < ResizedY ? BeforeY : ResizedY;

		for (int i = 0; i < LoopX; ++i)
		{
			for (int j = 0; j < LoopY; ++j)
			{
				ppTObj[i][j].SetPos(i, j, m_ppTObj[i][j].GetType());
				ppTObj[i][j].SetProp(m_ppTObj[i][j].GetProp(), "FOR");
			}
		}
		pCmd->SetNewWorld(ResizedX, ResizedY, ppTObj);

		// 교체
		CToolObject** ppDelObj = m_ppTObj;
		m_ppTObj = ppTObj;

		// 지워줌
		if (ppDelObj != NULL)
		{
			for (int i = 0; i < SiKGCMap()->GetBlockWidth(); i++)
			{
				if (ppDelObj[i] != NULL)
					SAFE_DELETE_ARRAY(ppDelObj[i]);
			}
		}

		SAFE_DELETE_ARRAY(ppDelObj);

		SiKGCMap()->SetBlockWidth(ResizedX);
		SiKGCMap()->SetBlockHeight(ResizedY);
		SetWorldVertex();
		ReloadLayer();

		m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
		AddLog(pCmd->GetLogStr());

		SiKGridGuideMgr()->BuildFH();
		SiKGridGuideMgr()->BuildGridGuide();
	}
	break;
	}
}

void CAppForm::ReloadLayer()
{
	float right = SiKGCMap()->GetPixelWidth();
	float top = SiKGCMap()->GetPixelHeight();

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	for (int i = 0; i < SiCLayerManager()->GetMaxLayerNum(); ++i)
	{
		float LayerXSize = vecLayer[i]->GetLayerXSize();
		float LayerYSize = vecLayer[i]->GetLayerYSize();

		if (LayerXSize != 800.0f)
			vecLayer[i]->SetLayerXSize(right);

		if (LayerYSize != 600.0f)
			vecLayer[i]->SetLayerYSize(top);
	}

	m_pLayerList->ReloadCurLayerListObjectList();
	m_RenderView.ResetView();
}

void CAppForm::OnAccelWorldResize()
{
	OnWorldresize();
}

void CAppForm::OnDropFiles(HDROP hDropInfo)
{
	TCHAR tcDropFile[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcDropFile, MAX_PATH);

	std::string strDropFile;
	strDropFile = tcDropFile;

	OnFileClose();

	DisableAllProperty(TRUE);

	m_pPartList->EnableWindow();

	m_strFileName = strDropFile;

	SiCLayerManager()->RegisterLuabind();
	SiCLayerManager()->LoadScript(strDropFile);

	ResizedX = SiKGCMap()->GetBlockWidth();
	ResizedY = SiKGCMap()->GetBlockHeight();

	SetWorldVertex();

	m_pPartList->UpdateCategoryList();

	m_pLayerList->ReloadLayerList();
	SiCLayerManager()->UpdateCategoryUse();

	ClearCurObj();

	SetCurrentDirectoryA(g_Path);

	g_AppFormView->GetSafeOwner()->SetWindowText(m_strFileName.c_str());
	g_AppFormView->GetSafeOwner()->UpdateWindow();

	OnModeLayer();
	m_bScriptLoaded = true;

	SiKGridGuideMgr()->BuildFH();
	SiKGridGuideMgr()->BuildGridGuide();

	__super::OnDropFiles(hDropInfo);
}

void CAppForm::ClearCurObj(bool bClearAll/* = true */)
{
	int i;
	if (bClearAll)
	{
		i = 0;
	}
	else
	{
		i = 1;
	}
	for (; i < (int)m_vecCurObj.size(); ++i)
	{
		SAFE_DELETE(m_vecCurObj[i]);
	}
	m_vecCurObj.clear();
}

void CAppForm::OnDebugZordersorting()
{
	int iLayer;
	if (m_pLayerList->GetCurLayer(iLayer) == true)
	{
		m_iLayerIndex = iLayer;
		std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

		LAYERINDEXCONDITION
		{
			vecLayer[m_iLayerIndex]->ResetZOrder();
		}

		m_pLayerList->ReloadObjectList(m_iLayerIndex);
	}
}

void CAppForm::OnAccelFileSaveAs()
{
	OnFileSaveAs();
}

class CFunctorAddFHtoMap : public std::unary_function< CToolObject, void >
{
public:
	CFunctorAddFHtoMap() {}
	virtual ~CFunctorAddFHtoMap() {}

	void operator()(CToolObject& _Left)
	{
		if (_Left.GetType() == CFootHold::FHT_HEAD)
		{
			int iIndexX = _Left.GetIndexX();
			int iIndexY = _Left.GetIndexY();
			if (iIndexX >= 0 && iIndexX <= SiKGCMap()->GetBlockWidth() - 1 &&
				iIndexY >= 0 && iIndexY <= SiKGCMap()->GetBlockHeight() - 1)
			{
				g_AppFormView->m_ppTObj[iIndexX][iIndexY].SetPos(iIndexX, iIndexY, _Left.GetType());
				g_AppFormView->m_ppTObj[iIndexX][iIndexY].SetProp(_Left.GetProp(), "OR");
			}
		}
	}
};

void CAppForm::FileSave(std::string strFielName)
{
	CString FileName = strFielName.c_str();

	std::stringstream Script;
	Script << SiKGCMap()->GetScriptHeader() << std::endl;
	Script << SiCLayerManager()->GetLayerInfoString() << std::endl;
	Script << SiCLayerManager()->GetLoadPartsString() << std::endl;
	Script << SiCLayerManager()->GetCreateLayerHeader();

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	for (int i = 0; i < SiCLayerManager()->GetMaxLayerNum(); ++i)
	{
		// ZOrder 대로 소팅
		vecLayer[i]->SortByZOrder();

		Script << vecLayer[i]->GetLayerHeader();
		std::vector< CLayerPart* >& PartVector = vecLayer[i]->GetPartVector();
		for (int j = 0; j < (int)PartVector.size(); ++j)
		{
			CLayerPart* pPart = PartVector[j];
			Script << pPart->ToObjectString();

			std::vector< CToolObject >& vecTO = pPart->GetToolObjectVector();
			if (vecTO.empty())
				continue;

			std::for_each(vecTO.begin(), vecTO.end(), CFunctorAddFHtoMap());
		}
		Script << "\t},\n";
	}
	Script << "}\n" << std::endl;

	Script << SiCLayerManager()->GetLayerSizeHeader();
	for (int i = 0; i < SiCLayerManager()->GetMaxLayerNum(); ++i)
	{
		Script << vecLayer[i]->GetLayerSizeData();
	}
	Script << "}\n" << std::endl;

	KGCMap* pMap = SiKGCMap();
	Script << pMap->GetFootHoldHeader();
	for (int i = 0; i < pMap->GetBlockWidth(); ++i)
	{
		for (int j = 0; j < pMap->GetBlockHeight(); ++j)
		{
			if (m_ppTObj[i][j].IsIndexValid() && m_ppTObj[i][j].GetType() == CFootHold::FHT_HEAD)
			{
				Script << m_ppTObj[i][j].ToString();
			}
		}
	}
	Script << "}\n";

	std::ofstream f;
	f.open(FileName.GetBuffer());
	if (f.is_open() == false)
	{
		AfxMessageBox("잘못된파일명입니다.");
		return;
	}

	f << Script.str();
	f.close();
	AfxMessageBox("저장성공했습니다.");
}

void CAppForm::OnFileSaveAs()
{
	if (m_bScriptLoaded == false)
		return;

	SetCurrentDirectoryA(g_Path);
	char szFilters[] = "Lua Script Files (*.lua) |*.lua||";

	std::string strFolder = g_Path;
	strFolder += "/Stage/*.lua";

	CFileDialog dlg(FALSE, strFolder.c_str(), NULL, OFN_NONETWORKBUTTON | OFN_HIDEREADONLY, szFilters, AfxGetMainWnd());
	INT_PTR nRet = dlg.DoModal();

	if (nRet == IDOK)
	{
		FileSave(dlg.GetPathName().GetBuffer());
	}

	m_strFileName = dlg.GetPathName().GetBuffer();

	g_AppFormView->GetParent()->SetWindowText(m_strFileName.c_str());
	g_AppFormView->GetParent()->UpdateWindow();

	SetCurrentDirectoryA(g_Path);
}

int CAppForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	HKEY key;
	DWORD dwDisp;
	DWORD Size = 4096;

	char strLatestFile[4096];
	memset((void*)strLatestFile, 0, 4096);
	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Kog\\WogMaptool", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &key, &dwDisp);

	LONG lRet;
	if ((lRet = RegQueryValueEx(key, "LatestFile", 0, NULL, (LPBYTE)strLatestFile, &Size)) != ERROR_SUCCESS)
	{
		m_strFileName = "";
	}
	else
	{
		m_strFileName = strLatestFile;
	}
	RegCloseKey(key);

	return 0;
}

void CAppForm::FileLoad(std::string strFielName)
{
	OnFileClose();

	DisableAllProperty(TRUE);

	m_pPartList->EnableWindow();

	std::string Script = strFielName;
	m_strFileName = Script;

	SiCLayerManager()->RegisterLuabind();
	SiCLayerManager()->LoadScript(Script);

	ResizedX = SiKGCMap()->GetBlockWidth();
	ResizedY = SiKGCMap()->GetBlockHeight();

	SetWorldVertex();

	m_pPartList->UpdateCategoryList();
	m_pLayerList->ReloadLayerList();
	m_pLayerList->Reset();

	SiCLayerManager()->UpdateCategoryUse();

	ClearCurObj();
	SetBoxObj(NULL);

	g_AppFormView->GetParent()->SetWindowText(m_strFileName.c_str());
	g_AppFormView->GetParent()->UpdateWindow();

	OnModeLayer();
	m_bScriptLoaded = true;
	ClearLog();

	std::stringstream stm;
	stm << m_strFileName << " 로드 성공";
	AddLog(stm.str());

	SiKGridGuideMgr()->BuildFH();
	SiKGridGuideMgr()->BuildGridGuide();
}

void CAppForm::OnDestroy()
{
	__super::OnDestroy();

	if (m_strFileName == "")
		return;

	HKEY key;
	DWORD dwDisp;
	DWORD Size;

	char strLatestFile[4096];
	memset((void*)strLatestFile, 0, 4096);
	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Kog\\WogMaptool", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &key, &dwDisp);
	strncpy(strLatestFile, m_strFileName.c_str(), strlen(m_strFileName.c_str()));
	Size = sizeof(strLatestFile);
	RegSetValueEx(key, "LatestFile", 0, REG_SZ, (LPBYTE)strLatestFile, Size);
	RegCloseKey(key);
}

void CAppForm::ClearLog()
{
	m_lcLog.DeleteAllItems();
	m_pCommandRecv->Init();
}

void CAppForm::AddLog(const std::string& str)
{
	int nItem = m_lcLog.GetItemCount();
	nItem = m_lcLog.InsertItem(nItem, str.c_str());
	m_lcLog.EnsureVisible(nItem, FALSE);
}

void CAppForm::OnAccelUndo()
{
	if (m_pCommandRecv->Undo() == true)
		AddLog(m_pCommandRecv->GetLastLog());
}

void CAppForm::OnAccelRedo()
{
	if (m_pCommandRecv->Redo() == true)
		AddLog(m_pCommandRecv->GetLastLog());
}

void CAppForm::OnGridguidelineChangeTerm()
{
	CChangeGridGuideTerm dlg;

	INT_PTR nRet = -1;
	nRet = dlg.DoModal();

	switch (nRet)
	{
	case -1:
		break;
	case IDOK:
		SiKGridGuideMgr()->BuildGridGuide();
		break;
	}
}

void CAppForm::OnGridAddVerticalGuide()
{
	D3DVECTOR MPos;
	m_RenderView.Get3DWorldPos(&MPos);

	if (MPos.x < 0.0f || MPos.x > SiKGCMap()->GetPixelWidth())
	{
		MPos.x = 0.0f;
	}

	int x = TO_INT(MPos.x);

	KGridGuide* pGuide = new KGridGuide();
	pGuide->SetGridGuidePos(D3DXVECTOR2((float)x, 0.0f));
	pGuide->SetDefault(false);
	pGuide->SetVertical(true);

	m_RenderView.ClearCurGridGuide();
	SiKGridGuideMgr()->AddGridGuide(pGuide);
}

void CAppForm::OnGridAddHorizonGuide()
{
	D3DVECTOR MPos;
	m_RenderView.Get3DWorldPos(&MPos);

	if (MPos.y < 0.0f || MPos.y > SiKGCMap()->GetPixelHeight())
	{
		MPos.y = 0.0f;
	}

	int y = TO_INT(MPos.y);

	KGridGuide* pGuide = new KGridGuide();
	pGuide->SetGridGuidePos(D3DXVECTOR2(0.0f, (float)y));
	pGuide->SetDefault(false);
	pGuide->SetVertical(false);

	m_RenderView.ClearCurGridGuide();
	SiKGridGuideMgr()->AddGridGuide(pGuide);
}

void CAppForm::OnBnClickedHGuideline()
{
	OnGridAddHorizonGuide();
}

void CAppForm::OnBnClickedVGuideline()
{
	OnGridAddVerticalGuide();
}

void CAppForm::OnForcedebugMakefootholdimage()
{
	OnAccelFootHold();
}

void CAppForm::FootHoldImageingRestore()
{
	// [11/23/2007] breadceo.원래대로 돌려준다
	m_RenderView.ResetView();
	m_RenderView.MoveWindow(m_rtRenderView.left, m_rtRenderView.top, 800, 600, TRUE);
	m_bFootHoldImaging = false;
	m_btnVerticalGuide.ShowWindow(SW_SHOW);
	m_btnHorizonGuide.ShowWindow(SW_SHOW);
}

void CAppForm::FootHoldImageingPrepare()
{
	m_btnVerticalGuide.ShowWindow(SW_HIDE);
	m_btnHorizonGuide.ShowWindow(SW_HIDE);
	m_bFootHoldImaging = true;
	OnModeFh();
	::GetWindowRect(m_hwndRenderWindow, &m_rtRenderView);
	ScreenToClient(&m_rtRenderView);
	m_RenderView.MoveWindow(50, 100, 512, 512, TRUE);
}

void CAppForm::OnAccelFootHold()
{
	g_bFootHoldImaging = true;
}