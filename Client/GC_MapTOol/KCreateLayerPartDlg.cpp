// KCreateLayerPartDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KCreateLayerPartDlg.h"
#include "resource.h"
#include "LayerPart.h"
#include "LayerManager.h"
//#include "GCDeviceManager.h"
#include "WOGMapTOol.h"
//#include <assert.h>
#include "KCNewCategory.h"

#include "Command/StampObjectCmd.h"
#include "Command/CreateLayerPartStampObjCmd.h"
#include "Command/CreateLayerPartUnStampObj.h"
#include "Command/CreateLayerPartCreateCaterogyCmd.h"
#include "Command/CreateLayerPartDeleteCategory.h"
#include "Command/CreateLayerPartModifyFH.h"
#include "Command/CreateLayerPartModifyCmd.h"
#include "Command/CreateLayerPartCloneCmd.h"
#include "Command/CreateLayerPartDelPartCmd.h"
#include "Command/CommandRecv.h"
#include ".\kcreatelayerpartdlg.h"
//#include "EffectProp.h"
#include "ParticleListDlg.h"
//#include <windowsx.h>

#define KEY_INTERVAL        ((DWORD)100)
KCreateLayerPartDlg* g_LayerToolDlg = NULL;

// KCreateLayerPartDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCreateLayerPartDlg, CDialog)
KCreateLayerPartDlg::KCreateLayerPartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KCreateLayerPartDlg::IDD, pParent)
	, CRenderEyeInfo()
	, m_strPartName(_T(""))
	, m_strPartX(_T(""))
	, m_strPartY(_T(""))
	, m_strPartWidth(_T(""))
	, m_strPartHeight(_T(""))
	, m_strCategoryName(_T(""))
	, m_fLayerWidth(0)
	, m_fLayerHeight(0)
	, m_strLayerWidth(_T(""))
	, m_strLayerHeight(_T(""))
	, m_strFHWidth(_T(""))
	, m_iFHHeight(0)
	, m_iFHWidth(0)
	, m_strFHHeight(_T(""))
	, m_bIsShiftPushed(false)
	, m_bToolObjectAdd(false)
{
	g_LayerToolDlg = this;
	m_hwndRenderWindow = NULL;

	ThreadKill = false;

	m_pPartClone = NULL;

	m_bCheckDuplicate = false;

	PartX = 0;
	PartY = 0;
	PartWidth = 0;
	PartHeight = 0;
	SrcBlendMode = 0;
	DestBlendMode = 0;
	m_strFileName = "";
	m_strBeforePartName = "";

	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fEyeX = 0.0f;
	m_fEyeY = 0.0f;

	InitializeCriticalSection(&m_csSwap);

	m_pManager = NULL;
	m_vecCurObj.clear();

	g_pd3dDevice->GetViewport(&m_vpPrev);
	m_pCommandRecv = NULL;
	m_bDrag = false;
}

KCreateLayerPartDlg::~KCreateLayerPartDlg()
{
	g_LayerToolDlg = NULL;
	SAFE_DELETE(m_pPartClone);
	m_pSwapChain = NULL;

	DeleteCriticalSection(&m_csSwap);

	SAFE_DELETE(m_pManager);
	m_vecCurObj.clear();
	g_pd3dDevice->SetViewport(&m_vpPrev);
	SAFE_DELETE(m_pCommandRecv);
}

void KCreateLayerPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SRCBLEND, m_cbSrcBlend);
	DDX_Control(pDX, IDC_COMBO_DESTBLEND, m_cbDestBlend);
	DDX_Control(pDX, IDC_PART_CATEGORY, m_lbCategoryList);
	DDX_Control(pDX, IDC_CATEGORY_LIST, m_lbPartsList);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_PART_NAME, m_editPartName);
	DDX_Text(pDX, IDC_PART_NAME, m_strPartName);
	DDX_Control(pDX, IDC_SELECT_TEXTURE, m_btnSelectTexture);
	DDX_Text(pDX, IDC_PART_X, m_strPartX);
	DDX_Text(pDX, IDC_PART_Y, m_strPartY);
	DDX_Text(pDX, IDC_PART_WIDTH, m_strPartWidth);
	DDX_Text(pDX, IDC_PART_HEIGHT, m_strPartHeight);
	DDX_LBString(pDX, IDC_PART_CATEGORY, m_strCategoryName);
	DDX_Control(pDX, IDC_ADD_PART, m_btnAdd);
	DDX_Control(pDX, IDC_DEL_PART, m_btnDelPart);
	DDX_Control(pDX, IDC_DEL_CATEGORY, m_btnDelCategory);
	DDX_Control(pDX, IDC_MODIFY_PART, m_btnModify);
	DDX_Control(pDX, IDC_LAYER_WIDTH, m_eLayerWidth);
	DDX_Control(pDX, IDC_LAYER_HEIGHT, m_eLayerHeight);
	DDX_Text(pDX, IDC_LAYER_WIDTH, m_strLayerWidth);
	DDX_Text(pDX, IDC_LAYER_HEIGHT, m_strLayerHeight);
	DDX_Control(pDX, IDC_REVERSE_LEFTRIGHT, m_btnReverseLeftRight);
	DDX_Control(pDX, IDC_REVERSE_UPDOWN, m_btnReverseUpDown);
	DDX_Control(pDX, IDC_CLONE, m_btnClone);
	DDX_Control(pDX, IDC_FH_WIDTH, m_eFHWidth);
	DDX_Text(pDX, IDC_FH_WIDTH, m_strFHWidth);
	DDX_Control(pDX, IDC_FH_HEIGHT, m_eFHHeight);
	DDX_Text(pDX, IDC_FH_HEIGHT, m_strFHHeight);
	DDX_Control(pDX, IDC_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_APPLY_TEXTURE, m_btnApplyTex);
	DDX_Control(pDX, IDC_APPLY_LAYER, m_btnApplyLayer);
	DDX_Control(pDX, IDC_LOG_LIST, m_listLog);
	DDX_Control(pDX, IDC_SHOW_PARTICLE_LIST, m_btnShowParticle);
	DDX_Control(pDX, IDC_STATIC_PARTICLE_NUM, m_cStaticParticleNum);
}

BEGIN_MESSAGE_MAP(KCreateLayerPartDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_SRCBLEND, OnCbnSelchangeComboSrcblend)
	ON_CBN_SELCHANGE(IDC_COMBO_DESTBLEND, OnCbnSelchangeComboDestblend)
	ON_BN_CLICKED(IDC_CHECK_DUPLICATE, OnBnClickedCheckDuplicate)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_PART_NAME, OnEnChangePartName)
	ON_BN_CLICKED(IDC_SELECT_TEXTURE, OnBnClickedSelectTexture)
	ON_EN_CHANGE(IDC_PART_X, OnEnChangePartX)
	ON_EN_CHANGE(IDC_PART_Y, OnEnChangePartY)
	ON_EN_CHANGE(IDC_PART_WIDTH, OnEnChangePartWidth)
	ON_EN_CHANGE(IDC_PART_HEIGHT, OnEnChangePartHeight)
	ON_BN_CLICKED(IDC_ADD_PART, OnBnClickedAddPart)
	ON_BN_CLICKED(IDC_DEL_PART, OnBnClickedDelPart)
	ON_BN_CLICKED(IDC_DEL_CATEGORY, OnBnClickedDelCategory)
	ON_BN_CLICKED(IDC_ADD_CATEGORY, OnBnClickedAddCategory)
	ON_LBN_SELCHANGE(IDC_PART_CATEGORY, OnLbnSelchangeCategoryList)
	ON_LBN_SELCHANGE(IDC_CATEGORY_LIST, OnLbnSelchangePartsList)
	ON_BN_CLICKED(IDC_MODIFY_PART, OnBnClickedModifyPart)
	ON_BN_CLICKED(IDC_REVERSE_LEFTRIGHT, OnBnClickedReverseLeftright)
	ON_BN_CLICKED(IDC_REVERSE_UPDOWN, OnBnClickedReverseUpdown)
	ON_BN_CLICKED(IDC_CLONE, OnBnClickedClone)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_APPLY_TEXTURE, OnBnClickedApplyTexture)
	ON_BN_CLICKED(IDC_APPLY_LAYER, OnBnClickedApplyLayer)
	ON_EN_CHANGE(IDC_FH_WIDTH, OnEnChangeFhWidth)
	ON_EN_CHANGE(IDC_FH_HEIGHT, OnEnChangeFhHeight)
	ON_WM_MOUSEMOVE()
	ON_WM_ENTERIDLE()
	ON_COMMAND(ID_ACCEL_RESET, OnAccelReset)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_ACCEL_REDO, OnAccelRedo)
	ON_COMMAND(ID_ACCEL_UNDO, OnAccelUndo)
	ON_EN_CHANGE(IDC_LAYER_WIDTH, OnEnChangeLayerWidth)
	ON_EN_CHANGE(IDC_LAYER_HEIGHT, OnEnChangeLayerHeight)
	ON_BN_CLICKED(IDC_SHOW_PARTICLE_LIST, OnBnClickedShowParticleInfo)
	ON_BN_CLICKED(IDC_BTN_TEXUV, &KCreateLayerPartDlg::OnBnClickedBtnTexuv)
	//	ON_STN_CLICKED(IDC_PART_PREVIEW, &KCreateLayerPartDlg::OnStnClickedPartPreview)
END_MESSAGE_MAP()

HRESULT KCreateLayerPartDlg::FrameMove()
{
	return S_OK;
}

HRESULT KCreateLayerPartDlg::Render()
{
	EnterCriticalSection(&m_csSwap);

	if (m_pSwapChain != NULL)
	{
		LPDIRECT3DSURFACE9 pSurface = NULL;

		m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
		g_pd3dDevice->SetRenderTarget(0, pSurface);

		// Clear the viewport
		HRESULT hr = g_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L);

		// Begin the scene
		if (SUCCEEDED(g_pd3dDevice->BeginScene()))
		{
			g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
			CToolObject::PreRenderProcess();

			if (m_pPartClone != NULL)
				m_pPartClone->Render();

			RenderFH();

			CToolObject::PostRenderProcess();
			g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

			// End the scene.
			g_pd3dDevice->EndScene();
		}
		m_pSwapChain->Present(NULL, NULL, m_hwndRenderWindow, NULL, 0);

		pSurface->Release();
	}
	LeaveCriticalSection(&m_csSwap);
	return S_OK;
}

int KCreateLayerPartDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL KCreateLayerPartDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// Save static reference to the render window
	m_hwndRenderWindow = GetDlgItem(IDC_PART_PREVIEW)->GetSafeHwnd();
	m_hAccel = LoadAccelerators(((HMODULE)GetWindowLongPtr(m_hwndRenderWindow, GWLP_HINSTANCE)), MAKEINTRESOURCE(IDR_ACCELERATOR2));

	//m_cbSrcBlend.AddString( "D3DBLEND_ZERO" );
	//m_cbSrcBlend.SetItemData( 0, 1 );
	//m_cbSrcBlend.AddString( "D3DBLEND_ONE" );
	//m_cbSrcBlend.SetItemData( 1, 2 );
	//m_cbSrcBlend.AddString( "D3DBLEND_SRCALPHA" );
	//m_cbSrcBlend.SetItemData( 2, 5 );
	//m_cbSrcBlend.AddString( "D3DBLEND_INVSRCALPHA" );
	//m_cbSrcBlend.SetItemData( 3, 6 );
	//m_cbSrcBlend.AddString( "D3DBLEND_DESTALPHA" );
	//m_cbSrcBlend.SetItemData( 4, 7 );
	//m_cbSrcBlend.AddString( "D3DBLEND_INVDESTALPHA" );
	//m_cbSrcBlend.SetItemData( 5, 8 );

	//m_cbDestBlend.AddString( "D3DBLEND_ZERO" );
	//m_cbDestBlend.SetItemData( 0, 1 );
	//m_cbDestBlend.AddString( "D3DBLEND_ONE" );
	//m_cbDestBlend.SetItemData( 1, 2 );
	//m_cbDestBlend.AddString( "D3DBLEND_SRCALPHA" );
	//m_cbDestBlend.SetItemData( 2, 5 );
	//m_cbDestBlend.AddString( "D3DBLEND_INVSRCALPHA" );
	//m_cbDestBlend.SetItemData( 3, 6 );
	//m_cbDestBlend.AddString( "D3DBLEND_DESTALPHA" );
	//m_cbDestBlend.SetItemData( 4, 7 );
	//m_cbDestBlend.AddString( "D3DBLEND_INVDESTALPHA" );
	//m_cbDestBlend.SetItemData( 5, 8 );

	m_listLog.InsertColumn(0, "Log", LVCFMT_LEFT, 640);
	m_listLog.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_pManager = new CLayerManager();
	m_pManager->RegisterLuabind();
	m_pCommandRecv = new CCommandRecv();

	std::vector< std::string > CategoryList = m_pManager->GetCategoryList();

	for (int i = 0; i < (int)CategoryList.size(); ++i)
	{
		m_lbCategoryList.AddString(CategoryList[i].c_str());
		m_lbCategoryList.SetItemData(i, i);
	}

	ResetView();
	m_vecCurObj.clear();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCreateLayerPartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Render();

	//if( (m_vStart.x != m_vEnd.x) || (m_vStart.y != m_vEnd.y) )
	//{	DrawRectangle( m_vStart.x, m_vStart.y, m_vEnd.x, m_vEnd.y );   }
}

LRESULT KCreateLayerPartDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::DefWindowProc(message, wParam, lParam);
}

void KCreateLayerPartDlg::OnCbnSelchangeComboSrcblend()
{
	SrcBlendMode = m_cbSrcBlend.GetItemData(m_cbSrcBlend.GetCurSel());

	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnCbnSelchangeComboDestblend()
{
	DestBlendMode = m_cbDestBlend.GetItemData(m_cbDestBlend.GetCurSel());

	UpdateLayerPart();
}

void KCreateLayerPartDlg::SetCategoryMember(std::string CategoryName)
{
	m_lbPartsList.ResetContent();
	assert(m_lbPartsList.GetCount() == 0);

	std::map< std::string, CLayerPart* >& Pool = m_pManager->GetPartPool();
	std::map< std::string, CLayerPart* >::iterator iter;
	int CategoryNum = 0;
	for (iter = Pool.begin(); iter != Pool.end(); ++iter)
	{
		CLayerPart* pPart = iter->second;
		if (pPart != NULL && pPart->GetPartCategory() == CategoryName)
		{
			m_lbPartsList.AddString(pPart->GetPartName().c_str());
			m_lbPartsList.SetItemData(CategoryNum, (DWORD_PTR)(Pool[pPart->GetPartName()]));
			CategoryNum++;
		}
	}

	m_lbPartsList.AddString("새로운 파트 만들기");
	m_lbPartsList.SetItemData(CategoryNum, -1);
}

void KCreateLayerPartDlg::OnBnClickedCheckDuplicate()
{
	UpdateData();

	std::string PartName = m_strPartName.GetBuffer();

	if (PartName == "")
	{
		AfxMessageBox("이름을 입력하세요");
		return;
	}

	std::map< std::string, CLayerPart* >& Pool = m_pManager->GetPartPool();
	std::map< std::string, CLayerPart* >::iterator iter;
	for (iter = Pool.begin(); iter != Pool.end(); ++iter)
	{
		if (iter->second != NULL && PartName == iter->second->GetPartName())
		{
			std::string strDuplicated = iter->second->GetPartCategory();
			strDuplicated += " 파트의 ";
			strDuplicated += PartName;
			strDuplicated += " 와 중복됩니다.";
			AfxMessageBox(strDuplicated.c_str());
			break;
		}
	}

	// 끝까지 같은 것을 못찾았다면.
	if (iter == Pool.end())
	{
		AfxMessageBox("사용할 수 있는 이름입니다.");
		m_bCheckDuplicate = true;

		if (m_lbPartsList.GetItemData(m_lbPartsList.GetCurSel()) == -1)
		{
			m_btnAdd.EnableWindow();
			m_btnModify.EnableWindow(FALSE);
		}
		else
		{
			m_btnAdd.EnableWindow(FALSE);
			m_btnModify.EnableWindow();
		}
	}
}

void KCreateLayerPartDlg::OnBnClickedOk()
{
	std::string DirPath = g_Path;
	DirPath += "/Stage/";
	std::string ListPath = DirPath + "PartsCategoryList.lua";

	std::ofstream f(ListPath.c_str());
	f << m_pManager->GetCategoryListString() << std::endl;
	f.close();

	std::map< std::string, CLayerPart* >& Pool = m_pManager->GetPartPool();
	std::vector< std::string > CategoryList = m_pManager->GetCategoryList();
	std::map< std::string, CLayerPart* >::iterator iter;

	for (int i = 0; i < (int)CategoryList.size(); ++i)
	{
		std::string FilePath = DirPath + CategoryList[i];

		std::ofstream f(FilePath.c_str(), std::ios_base::out | std::ios_base::trunc);
		//f << m_pManager->GetScriptHeader() << std::endl;

		f << SiKGCMap()->GetFootHoldEnum();
		for (iter = Pool.begin(); iter != Pool.end(); ++iter)
		{
			if (iter->second != NULL && iter->second->GetPartCategory() == CategoryList[i])
			{
				f << iter->second->ToString() << std::endl;
			}
		}
		f.close();
	}

	// 내용을 싹 간다. T^T 하드하당..
	SiCLayerManager()->SetCategoryList(CategoryList);

	std::map< std::string, CLayerPart* >& PartPool = SiCLayerManager()->GetPartPool();
	std::map< std::string, CLayerPart* >::iterator iter2;
	for (iter2 = PartPool.begin(); iter2 != PartPool.end(); ++iter2)
		SAFE_DELETE(iter2->second);
	PartPool.clear();

	for (iter = Pool.begin(); iter != Pool.end(); ++iter)
	{
		if (iter->second == NULL)
			continue;
		PartPool[iter->first] = iter->second->Clone();
	}

	CDialog::OnOK();
}

void KCreateLayerPartDlg::OnEnChangePartName()
{
	m_bCheckDuplicate = false;
	m_btnAdd.EnableWindow(FALSE);
	m_btnModify.EnableWindow(FALSE);
}

void KCreateLayerPartDlg::OnBnClickedSelectTexture()
{
	SetCurrentDirectoryA(g_Path);
	std::string strFolder = g_Path;
	strFolder += "\\Texture";
	CString strFileName;
	CString strFilter = "All Image File (*.DDS;*.TGA;*.BMP;*.JPG;*.JPEG;*.PNG)|*.DDS;*.TGA;*.BMP;*.JPG;*.JPEG;*.PNG|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, strFileName, OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, strFilter);

	dlg.m_ofn.lpstrInitialDir = strFolder.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	std::string FileName = dlg.GetFileName().GetBuffer();
	m_btnSelectTexture.SetWindowText(FileName.c_str());

	m_strFileName = FileName;
	UpdateLayerPart();
	SetCurrentDirectoryA(g_Path);
}

void KCreateLayerPartDlg::OnEnChangePartX()
{
	UpdateData();
	PartX = strtol(m_strPartX.GetBuffer(), NULL, 0);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnEnChangePartY()
{
	UpdateData();
	PartY = strtol(m_strPartY.GetBuffer(), NULL, 0);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnEnChangePartWidth()
{
	UpdateData();
	PartWidth = strtol(m_strPartWidth.GetBuffer(), NULL, 0);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnEnChangePartHeight()
{
	UpdateData();
	PartHeight = strtol(m_strPartHeight.GetBuffer(), NULL, 0);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::UpdateLayerPart()
{
	if (m_pPartClone == NULL)
		return;

	m_pPartClone->SetPartName(m_strPartName.GetBuffer());
	m_pPartClone->SetPartCategory(m_strCategoryName.GetBuffer());
	m_pPartClone->SetTexturePart(m_strFileName, KLayerRect<int>(PartX, PartY, PartWidth, PartHeight));
	m_pPartClone->SetLayerPos(KLayerRect<float>(0, 0, m_fLayerWidth, m_fLayerHeight));
	m_pPartClone->SetVertex();
	m_pPartClone->SetStartVertex(-1.0f, -1.0f);
	m_pPartClone->SetEndVertex(-1.0f, -1.0f);

	//int Width, Height;
	//m_pPartClone->GetTextureSize( Width, Height );

	CRect rect;
	::GetWindowRect(m_hwndRenderWindow, &rect);
	InvalidateRect(&rect);
}

void KCreateLayerPartDlg::OnBnClickedAddPart()
{
	UpdateLayerPart();

	if (m_lbCategoryList.GetCurSel() == LB_ERR)
	{
		AfxMessageBox("추가할 카테고리를 선택하세요");
		return;
	}

	if (m_bCheckDuplicate == false)
	{
		AfxMessageBox("중복체크 버튼을 누르세요");
		return;
	}

	m_pPartClone->SaveAndResetVertex();
	m_pManager->AddLayerPart(m_pPartClone);
	SetCategoryMember(m_pPartClone->GetPartCategory());
	m_lbPartsList.SetCurSel(m_lbPartsList.FindString(-1, m_pPartClone->GetPartName().c_str()));
	m_btnAdd.EnableWindow(FALSE);
	m_pPartClone = m_pPartClone->Clone();
}

void KCreateLayerPartDlg::InitControlData()
{
	m_lbPartsList.SetCurSel(-1);

	m_cbSrcBlend.SetCurSel(-1);
	m_cbDestBlend.SetCurSel(-1);
	m_strPartName = "";
	m_btnSelectTexture.SetWindowText("Select Texture");
	m_strPartX = "";
	m_strPartY = "";
	m_strPartWidth = "";
	m_strPartHeight = "";
	m_strLayerHeight = "";
	m_strLayerWidth = "";
	UpdateData(FALSE);

	CRect rect;
	::GetWindowRect(m_hwndRenderWindow, &rect);
	InvalidateRect(&rect);
}

void KCreateLayerPartDlg::OnBnClickedDelPart()
{
	if (AfxMessageBox("Part를 삭제하면 현재 스테이지의 해당 파트가 모두 사라집니다.\n 계속하시겠습니까?", MB_YESNO) == IDYES)
	{
		CCreateLayerPartDelPartCmd* pCmd = new CCreateLayerPartDelPartCmd();
		pCmd->SetDelPart(m_pPartClone);
		m_pManager->EraseLayerPart(m_pPartClone);

		std::vector< std::string > CategoryList = m_pManager->GetCategoryList();
		SetCategoryMember(CategoryList[m_lbCategoryList.GetCurSel()]);

		InitControlData();
		SAFE_DELETE(m_pPartClone);

		m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
		AddLog(pCmd->GetLogStr());
	}
}

void KCreateLayerPartDlg::OnBnClickedDelCategory()
{
	UpdateData();

	if (m_lbCategoryList.GetCurSel() == LB_ERR)
	{
		AfxMessageBox("삭제할 카테고리를 선택하세요");
		return;
	}

	if (AfxMessageBox("카테고리를 삭제하면 해당 카테고리의 모든 파트가 사라집니다.\n 계속하시겠습니까?", MB_YESNO) == IDYES)
	{
		// 카테고리 내의 모든 파트를 삭제 하고 카테고리도 삭제
		std::string strCategoryName = m_strCategoryName.GetBuffer();
		CCreateLayerPartDeleteCategory* pCmd = new CCreateLayerPartDeleteCategory();
		DeleteNewCategory(strCategoryName);
		pCmd->SetCategoryName(strCategoryName);
		m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
		AddLog(pCmd->GetLogStr());

		SAFE_DELETE(m_pPartClone);
	}
}

void KCreateLayerPartDlg::DeleteNewCategory(const std::string& strCategoryName)
{
	m_pManager->DeleteCategory(strCategoryName);

	m_lbCategoryList.ResetContent();
	std::vector< std::string > CategoryList = m_pManager->GetCategoryList();

	for (int i = 0; i < (int)CategoryList.size(); ++i)
	{
		m_lbCategoryList.AddString(CategoryList[i].c_str());
		m_lbCategoryList.SetItemData(i, i);
	}

	m_lbCategoryList.SetCurSel(-1);
	m_lbPartsList.ResetContent();
	m_strPartName = "";
	m_btnSelectTexture.SetWindowText("Select Texture");
	m_strPartX = "";
	m_strPartY = "";
	m_strPartWidth = "";
	m_strPartHeight = "";
	UpdateData(FALSE);

	CRect rect;
	::GetWindowRect(m_hwndRenderWindow, &rect);
	InvalidateRect(&rect);
}

void KCreateLayerPartDlg::AddNewCategory(const std::string& strCategoryName)
{
	if (strCategoryName == "")
		return;

	m_pManager->AddCategoryList(strCategoryName);

	m_lbCategoryList.ResetContent();
	assert(m_lbCategoryList.GetCount() == 0);

	std::vector< std::string > CategoryList = m_pManager->GetCategoryList();

	for (int i = 0; i < (int)CategoryList.size(); ++i)
	{
		m_lbCategoryList.AddString(CategoryList[i].c_str());
		m_lbCategoryList.SetItemData(i, i);
	}
	m_lbCategoryList.SetCurSel(m_lbCategoryList.FindString(-1, strCategoryName.c_str()));
	UpdateData();
	SetCategoryMember(CategoryList[m_lbCategoryList.GetCurSel()]);
	m_lbPartsList.SetCurSel(0);
	OnLbnSelchangePartsList();
}

void KCreateLayerPartDlg::OnBnClickedAddCategory()
{
	KCNewCategory dlg;

	INT_PTR nRet = -1;
	nRet = dlg.DoModal();

	switch (nRet)
	{
	case -1:
		AfxMessageBox("New Part Caterogy Dlg Cannot be Create!!");
		break;
	case IDOK:
	{
		// Category 추가 & 현재 선택한 카테고리 변경
		CCreateLayerPartCreateCaterogyCmd* pCmd = new CCreateLayerPartCreateCaterogyCmd();
		AddNewCategory(m_strNewCategoryName);
		pCmd->SetCategoryname(m_strNewCategoryName);
		m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
		AddLog(pCmd->GetLogStr());
	}
	break;
	case IDCANCEL:
		m_lbCategoryList.SetCurSel(-1);
		m_lbPartsList.SetCurSel(-1);
		m_lbPartsList.ResetContent();
		m_btnDelCategory.EnableWindow(FALSE);
		break;
	}

	UpdateData();
}

void KCreateLayerPartDlg::OnLbnSelchangeCategoryList()
{
	JIF(m_lbCategoryList.GetCurSel() != -1);

	std::vector< std::string > CategoryList = m_pManager->GetCategoryList();
	SetCategoryMember(CategoryList[m_lbCategoryList.GetCurSel()]);
	m_btnDelCategory.EnableWindow();

	UpdateData();
}

void KCreateLayerPartDlg::EnableAllWindow(BOOL bEnable)
{
	m_btnDelPart.EnableWindow(bEnable);
	m_btnModify.EnableWindow(bEnable);
	m_btnReverseUpDown.EnableWindow(bEnable);
	m_btnReverseLeftRight.EnableWindow(bEnable);
	m_btnClone.EnableWindow(bEnable);
	//m_btnApply.EnableWindow( bEnable );
	m_btnApplyTex.EnableWindow(bEnable);
	m_btnApplyLayer.EnableWindow(bEnable);
	m_btnAdd.EnableWindow(!bEnable);
	m_btnShowParticle.EnableWindow(bEnable);
}

void KCreateLayerPartDlg::OnLbnSelchangePartsList()
{
	JIF(m_lbPartsList.GetCurSel() != -1);

	if (m_lbPartsList.GetItemData(m_lbPartsList.GetCurSel()) == -1)
	{
		// 새로 만들기임
		SAFE_DELETE(m_pPartClone);
		m_pPartClone = new CLayerPart();
		m_pPartClone->SetLayerPos(KLayerRect<float>(0, 0, 600, 600));
		m_pPartClone->SetTextureRedirect(KLayerRect<int>(0, 0, 512, 512));
		EnableAllWindow(FALSE);
	}
	else
	{
		SAFE_DELETE(m_pPartClone);
		CLayerPart* pPart = (CLayerPart*)m_lbPartsList.GetItemData(m_lbPartsList.GetCurSel());
		m_pPartClone = pPart->Clone();
		EnableAllWindow(TRUE);
		m_strBeforePartName = m_pPartClone->GetPartName();
	}
	UpdateControlData();
	ResetView();
	m_vecCurObj.clear();
	UpdateLayerPart();
}

void KCreateLayerPartDlg::UpdateControlData()
{
	m_strFileName = m_pPartClone->GetFileName();
	KLayerRect<int> kRect;
	m_pPartClone->GetTexRect(kRect);
	PartX = kRect.m_X;
	PartY = kRect.m_Y;
	PartWidth = kRect.m_Width;
	PartHeight = kRect.m_Height;

	KLayerRect<float> kfRect;
	m_pPartClone->GetLayerRect(kfRect);
	m_fLayerWidth = kfRect.m_Width;
	m_fLayerHeight = kfRect.m_Height;

	m_btnSelectTexture.SetWindowText(m_pPartClone->GetFileName().c_str());
	m_strPartName = m_pPartClone->GetPartName().c_str();
	m_strPartX.Format("%d", PartX);
	m_strPartY.Format("%d", PartY);
	m_strPartWidth.Format("%d", PartWidth);
	m_strPartHeight.Format("%d", PartHeight);
	m_strLayerWidth.Format("%f", m_fLayerWidth);
	m_strLayerHeight.Format("%f", m_fLayerHeight);
	m_iFHWidth = m_pPartClone->GetFHWidth();
	m_strFHWidth.Format("%d", m_iFHWidth);
	m_iFHHeight = m_pPartClone->GetFHHeight();
	m_strFHHeight.Format("%d", m_iFHHeight);

	std::stringstream str;

	str << "적용된 파티클 갯수 : ";
	str << m_pPartClone->GetParticleCount();
	m_cStaticParticleNum.SetWindowText(str.str().c_str());

	UpdateData(FALSE);
}

void KCreateLayerPartDlg::OnBnClickedModifyPart()
{
	UpdateLayerPart();

	CCreateLayerPartModifyCmd* pCmd = new CCreateLayerPartModifyCmd();

	std::map< std::string, CLayerPart* >& Pool = m_pManager->GetPartPool();
	std::map< std::string, CLayerPart* >::iterator iter = Pool.find(m_strBeforePartName);
	CLayerPart* Temp = iter->second;
	m_pPartClone->InitEyeBlockInfo(static_cast<CRenderEyeInfo*>(g_LayerToolDlg));
	iter->second = m_pPartClone;
	m_pPartClone = iter->second->Clone();
	pCmd->SetModifyData(Temp, iter->second);
	SAFE_DELETE(Temp);
	SetCategoryMember(m_pPartClone->GetPartCategory());
	m_lbPartsList.SetCurSel(m_lbPartsList.FindString(-1, m_pPartClone->GetPartName().c_str()));

	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());

	AfxMessageBox("수정완료");
}

void KCreateLayerPartDlg::OnBnClickedReverseLeftright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	m_pPartClone->Inverse(true);
	KLayerRect<int> kRect;
	m_pPartClone->GetTexRect(kRect);
	PartX = kRect.m_X;
	PartY = kRect.m_Y;
	PartWidth = kRect.m_Width;
	PartHeight = kRect.m_Height;
	std::stringstream stm;
	stm << PartX;
	m_strPartX = stm.str().c_str();
	stm.str(std::string());
	stm << PartY;
	m_strPartY = stm.str().c_str();
	stm.str(std::string());
	stm << PartWidth;
	m_strPartWidth = stm.str().c_str();
	stm.str(std::string());
	stm << PartHeight;
	m_strPartHeight = stm.str().c_str();
	stm.str(std::string());
	UpdateData(FALSE);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnBnClickedReverseUpdown()
{
	UpdateData();
	m_pPartClone->Inverse(false);
	KLayerRect<int> kRect;
	m_pPartClone->GetTexRect(kRect);
	PartX = kRect.m_X;
	PartY = kRect.m_Y;
	PartWidth = kRect.m_Width;
	PartHeight = kRect.m_Height;
	std::stringstream stm;
	stm << PartX;
	m_strPartX = stm.str().c_str();
	stm.str(std::string());
	stm << PartY;
	m_strPartY = stm.str().c_str();
	stm.str(std::string());
	stm << PartWidth;
	m_strPartWidth = stm.str().c_str();
	stm.str(std::string());
	stm << PartHeight;
	m_strPartHeight = stm.str().c_str();
	stm.str(std::string());
	UpdateData(FALSE);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnBnClickedClone()
{
	UpdateLayerPart();
	std::stringstream stm;
	stm << m_pPartClone->GetPartName() << " copy";
	m_pPartClone->SetPartName(stm.str());

	CCreateLayerPartCloneCmd* pCmd = new CCreateLayerPartCloneCmd();
	pCmd->SetClonePart(m_pPartClone);
	m_pManager->AddLayerPart(m_pPartClone);
	SetCategoryMember(m_pPartClone->GetPartCategory());
	m_lbPartsList.SetCurSel(m_lbPartsList.FindString(-1, m_pPartClone->GetPartName().c_str()));
	m_btnAdd.EnableWindow(FALSE);
	m_pPartClone = m_pPartClone->Clone();

	OnLbnSelchangePartsList();

	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void KCreateLayerPartDlg::OnBnClickedApply()
{
	UpdateData();
	m_iFHWidth = strtol(m_strFHWidth.GetBuffer(), NULL, 0);
	m_iFHHeight = strtol(m_strFHHeight.GetBuffer(), NULL, 0);

	CCreateLayerPartModifyFH* pCmd = new CCreateLayerPartModifyFH();
	pCmd->SetModifyData(m_iFHWidth, m_iFHHeight, m_pPartClone);
	m_pPartClone->SetFHWidth(m_iFHWidth);
	m_pPartClone->SetFHHeight(m_iFHHeight);
	m_pPartClone->InitTOVector();
	UpdateLayerPart();
	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void KCreateLayerPartDlg::OnBnClickedApplyTexture()
{
	UpdateData();
	PartX = strtol(m_strPartX.GetBuffer(), NULL, 0);
	PartY = strtol(m_strPartY.GetBuffer(), NULL, 0);
	PartWidth = strtol(m_strPartWidth.GetBuffer(), NULL, 0);
	PartHeight = strtol(m_strPartHeight.GetBuffer(), NULL, 0);

	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnBnClickedApplyLayer()
{
	UpdateData();
	m_fLayerHeight = static_cast<float>(strtod(m_strLayerHeight.GetBuffer(), NULL));
	m_fLayerWidth = static_cast<float>(strtod(m_strLayerWidth.GetBuffer(), NULL));
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnEnChangeFhWidth()
{
	UpdateData();
	m_iFHWidth = strtol(m_strFHWidth.GetBuffer(), NULL, 0);
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnEnChangeFhHeight()
{
	UpdateData();
	m_iFHHeight = strtol(m_strFHHeight.GetBuffer(), NULL, 0);
	UpdateLayerPart();
}

#define SMOOTHRATE (1.0f)

void KCreateLayerPartDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		// 공통
		if (m_bIsShiftPushed)
		{
			float fDiffX = (float)(point.x - m_Point.x);
			float fDiffY = (float)(m_Point.y - point.y);

			m_fEyeX = m_StartPoint.x - fDiffX * SMOOTHRATE;
			m_fEyeY = m_StartPoint.y - fDiffY * SMOOTHRATE;

			D3DXMATRIX matView;
			D3DXVECTOR3 vFromPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 0.0f);
			D3DXVECTOR3 vLookatPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 1.0f);
			D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
			g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
		}
		else
		{
			if (m_bDrag)
			{
				D3DVECTOR MPos;
				Get3DWorldPos(&MPos);
				m_pPartClone->SetEndVertex(MPos.x, MPos.y);
			}

			if (!m_vecCurObj.empty())
			{
				UpdateMouseIndex();

				if (m_bToolObjectAdd == false)
				{
					if (!m_vecCurObj.empty())
					{
						CToolObject pTemp = m_vecCurObj[0];
						m_vecCurObj.clear();
						m_vecCurObj.push_back(pTemp);
					}
				}

				if (m_iDragStartIndexX != m_iMIndexX || m_iDragStartIndexY != m_iMIndexY)
				{
					if (m_iMIndexX >= m_iDragStartIndexX)
					{
						if (m_iMIndexY >= m_iDragStartIndexY)
						{
							for (int i = m_iDragStartIndexX; i != m_iMIndexX + 1; ++i)
							{
								if (i < 0 && i > m_pPartClone->GetFHWidth() - 1)
									break;

								for (int j = m_iDragStartIndexY; j != m_iMIndexY + 1; ++j)
								{
									if (j < 0 && j > m_pPartClone->GetFHHeight() - 1)
										break;

									AddToolObject(i, j);
								}
							}
						}
						else
						{
							for (int i = m_iDragStartIndexX; i != m_iMIndexX + 1; ++i)
							{
								if (i < 0 && i > m_pPartClone->GetFHWidth() - 1)
									break;

								for (int j = m_iDragStartIndexY; j != m_iMIndexY - 1; --j)
								{
									if (j < 0 && j > m_pPartClone->GetFHHeight() - 1)
										break;

									AddToolObject(i, j);
								}
							}
						}
					}
					else
					{
						if (m_iMIndexY >= m_iDragStartIndexY)
						{
							for (int i = m_iDragStartIndexX; i != m_iMIndexX - 1; --i)
							{
								if (i < 0 && i > m_pPartClone->GetFHWidth() - 1)
									break;

								for (int j = m_iDragStartIndexY; j != m_iMIndexY + 1; ++j)
								{
									if (j < 0 && j > m_pPartClone->GetFHHeight() - 1)
										break;

									AddToolObject(i, j);
								}
							}
						}
						else
						{
							for (int i = m_iDragStartIndexX; i != m_iMIndexX - 1; --i)
							{
								if (i < 0 && i > m_pPartClone->GetFHWidth() - 1)
									break;

								for (int j = m_iDragStartIndexY; j != m_iMIndexY - 1; --j)
								{
									if (j < 0 && j > m_pPartClone->GetFHHeight() - 1)
										break;

									AddToolObject(i, j);
								}
							}
						}
					}
				}
			}
		}
	}

	OnPaint();
	CDialog::OnMouseMove(nFlags, point);
}

#define CTRL_ZDELTA (128.0f)

void KCreateLayerPartDlg::ResetView()
{
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	m_fEyeX = static_cast<float>(m_fLayerWidth * 0.5f);
	m_fEyeY = static_cast<float>(m_fLayerHeight * 0.5f);

	D3DXMATRIX matView;
	D3DXVECTOR3 vFromPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 0.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(m_fEyeX, m_fEyeY, 1.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	m_fWidth = 640.0f + (CTRL_ZDELTA * 1.3f) * 2.0f;
	m_fHeight = 480.0f + CTRL_ZDELTA * 2.0f;

	// Set the projection matrix
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

BOOL KCreateLayerPartDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_CHAR &&
		(pMsg->hwnd == m_lbCategoryList.GetSafeHwnd() ||
			pMsg->hwnd == m_lbPartsList.GetSafeHwnd()))
		return TRUE;

	static DWORD dwInputTime = ::timeGetTime();
	BOOL bInput = FALSE;
	if (GetDlgItem(IDC_PART_PREVIEW)->GetFocus()->GetSafeHwnd() == GetFocus()->GetSafeHwnd())
	{
		if (::timeGetTime() - dwInputTime <= KEY_INTERVAL)
			return TRUE;

		m_bIsShiftPushed = GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000;
		m_bToolObjectAdd = GetAsyncKeyState(VK_CONTROL) & 0x8000;

		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			if (::timeGetTime() - dwInputTime > KEY_INTERVAL)
			{
				if (GetAsyncKeyState(VK_ADD) & 0x8000)
				{
					QuaterZoom(true);
					dwInputTime = ::timeGetTime();
					bInput = TRUE;
				}

				if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
				{
					QuaterZoom(false);
					dwInputTime = ::timeGetTime();
					bInput = TRUE;
				}
			}
		}

		if (m_pPartClone != NULL && m_pPartClone->GetValidateVertex())
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				if (!m_bToolObjectAdd)
				{
					D3DXVECTOR2 vecs = m_pPartClone->GetStartVertex();
					m_pPartClone->SetStartVertex(vecs.x + 1.0f, vecs.y);
				}
				D3DXVECTOR2 vec = m_pPartClone->GetEndVertex();
				m_pPartClone->SetEndVertex(vec.x + 1.0f, vec.y);
			}
			else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				if (!m_bToolObjectAdd)
				{
					D3DXVECTOR2 vecs = m_pPartClone->GetStartVertex();
					m_pPartClone->SetStartVertex(vecs.x - 1.0f, vecs.y);
				}
				D3DXVECTOR2 vec = m_pPartClone->GetEndVertex();
				m_pPartClone->SetEndVertex(vec.x - 1.0f, vec.y);
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				if (!m_bToolObjectAdd)
				{
					D3DXVECTOR2 vecs = m_pPartClone->GetStartVertex();
					m_pPartClone->SetStartVertex(vecs.x, vecs.y + 1.0f);
				}
				D3DXVECTOR2 vec = m_pPartClone->GetEndVertex();
				m_pPartClone->SetEndVertex(vec.x, vec.y + 1.0f);
			}
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				if (!m_bToolObjectAdd)
				{
					D3DXVECTOR2 vecs = m_pPartClone->GetStartVertex();
					m_pPartClone->SetStartVertex(vecs.x, vecs.y - 1.0f);
				}
				D3DXVECTOR2 vec = m_pPartClone->GetEndVertex();
				m_pPartClone->SetEndVertex(vec.x, vec.y - 1.0f);
			}

			OnPaint();
		}

		if (!m_vecCurObj.empty())
		{
			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000)
			{
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					std::for_each(m_vecCurObj.begin(), m_vecCurObj.end(),
						boost::bind(&CToolObject::SetProp, _1, CFootHold::FHP_NO_COME_RIGHT, "XOR"));
					dwInputTime = ::timeGetTime();
					bInput = TRUE;
				}
				else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					std::for_each(m_vecCurObj.begin(), m_vecCurObj.end(),
						boost::bind(&CToolObject::SetProp, _1, CFootHold::FHP_NO_COME_LEFT, "XOR"));
					dwInputTime = ::timeGetTime();
					bInput = TRUE;
				}
				else if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					std::for_each(m_vecCurObj.begin(), m_vecCurObj.end(),
						boost::bind(&CToolObject::SetProp, _1, CFootHold::FHP_NO_JUMP_DOWN, "XOR"));
					dwInputTime = ::timeGetTime();
					bInput = TRUE;
				}
				//else if( GetAsyncKeyState(VK_DOWN) & 0x8000 )
				//{
				//    std::for_each( m_vecCurObj.begin(), m_vecCurObj.end(),
				//        boost::bind( &CToolObject::SetProp, _1, CFootHold::FHP_NO_JUMP_UP,"XOR" ) );
				//    dwInputTime = ::timeGetTime();
				//    bInput = TRUE;
				//}
			}
			else
			{
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					CToolObject Temp = m_vecCurObj[0];
					ClearCurObj(false);
					Temp.MoveRight();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(Temp);
				}
				else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					CToolObject Temp = m_vecCurObj[0];
					ClearCurObj(false);
					Temp.MoveLeft();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(Temp);
				}

				if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					CToolObject Temp = m_vecCurObj[0];
					ClearCurObj(false);
					Temp.MoveUp();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(Temp);
				}
				else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					CToolObject Temp = m_vecCurObj[0];
					ClearCurObj(false);
					Temp.MoveDown();
					dwInputTime = ::timeGetTime();
					bInput = TRUE;

					m_vecCurObj.push_back(Temp);
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

			OnPaint();
		}
	}

	if (bInput == TRUE)
		return TRUE;

	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccel && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void KCreateLayerPartDlg::QuaterZoom(bool Zoom)
{
	if (Zoom == true)
	{
		if (m_fHeight > CTRL_ZDELTA)
			m_fHeight -= CTRL_ZDELTA;
		if (m_fWidth > CTRL_ZDELTA * 1.3f)
			m_fWidth -= CTRL_ZDELTA * 1.3f;
	}
	else
	{
		m_fHeight += CTRL_ZDELTA;
		m_fWidth += CTRL_ZDELTA * 1.3f;
	}

	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	OnPaint();
}
void KCreateLayerPartDlg::OnEnterIdle(UINT nWhy, CWnd* pWho)
{
	CDialog::OnEnterIdle(nWhy, pWho);

	OnPaint();
}

void KCreateLayerPartDlg::OnAccelReset()
{
	ResetView();
	OnPaint();
}

void KCreateLayerPartDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	RECT rt;
	GetDlgItem(IDC_PART_PREVIEW)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (PtInRect(&rt, point) == false)
		return;

	if (nFlags & MK_LBUTTON)
	{
		D3DVECTOR MPos;
		Get3DWorldPos(&MPos);

		if (m_bIsShiftPushed)
		{
			m_Point = point;
			m_StartPoint.x = (LONG)m_fEyeX;
			m_StartPoint.y = (LONG)m_fEyeY;
			return;
		}
		else if (::GetAsyncKeyState(VK_LMENU) & 0x8000)
		{
			if (m_pPartClone != NULL)
			{
				m_pPartClone->SetStartVertex(MPos.x, MPos.y);
				m_pPartClone->SetEndVertex(MPos.x, MPos.y);
				m_pPartClone->SetRectangle();
			}
		}
		else
		{
			if (false == m_bDrag)
			{
				if (m_pPartClone != NULL)
				{
					m_bDrag = true;
					m_pPartClone->SetStartVertex(MPos.x, MPos.y);
					m_pPartClone->SetEndVertex(MPos.x, MPos.y);
				}
			}

			UpdateMouseIndex();
			m_iDragStartIndexX = m_iMIndexX;
			m_iDragStartIndexY = m_iMIndexY;

			if (m_bToolObjectAdd == false)
			{
				ClearCurObj();
				AddToolObject(m_iMIndexX, m_iMIndexY);
				return;
			}
			else
			{
				AddToolObject(m_iMIndexX, m_iMIndexY);
			}
		}
	}

	OnPaint();
	CDialog::OnLButtonDown(nFlags, point);
}

void KCreateLayerPartDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDrag = false;

	D3DVECTOR MPos;
	Get3DWorldPos(&MPos);

	if (m_pPartClone != NULL && !m_bIsShiftPushed && m_pPartClone->GetValidateVertex() && !(::GetAsyncKeyState(VK_LMENU)))
	{
		m_pPartClone->SetEndVertex(MPos.x, MPos.y);
	}

	OnPaint();
	CDialog::OnLButtonUp(nFlags, point);
}

void KCreateLayerPartDlg::Get3DWorldPos(D3DVECTOR* PosOut)
{
	CPoint point;
	GetCursorPos(&point);
	GetDlgItem(IDC_PART_PREVIEW)->ScreenToClient(&point);

	D3DVECTOR v;
	D3DMATRIX matProj;
	memset(&matProj, 0, sizeof(matProj));

	// [8/16/2007 breadceo] Viewport 문제..
	D3DVIEWPORT9 vp;
	//g_pd3dDevice->GetViewport(&vp);
	vp.X = vp.Y = 0;
	vp.Width = 644;
	vp.Height = 495;
	vp.MinZ = 0.f;
	vp.MaxZ = 1.f;

	HRESULT hr = g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	v.x = (((((point.x - vp.X) * 2.0f / vp.Width) - 1.0f)) - matProj._31) / matProj._11;
	v.y = ((-(((point.y - vp.Y) * 2.0f / vp.Height) - 1.0f)) - matProj._32) / matProj._22;
	v.z = 0.0f;

	D3DXMATRIX m, matView;
	D3DVECTOR vPickRayDir, vPickRayOrig;
	g_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&m, NULL, &matView);
	vPickRayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	PosOut->x = vPickRayOrig.x + vPickRayDir.x;
	PosOut->y = vPickRayOrig.y + vPickRayDir.y;
	PosOut->z = 0.0f;
}

void KCreateLayerPartDlg::UpdateMouseIndex()
{
	if (m_pPartClone == NULL)
		return;

	D3DVECTOR MPos;
	Get3DWorldPos(&MPos);

	m_iMIndexX = (int)(MPos.x / FOOTHOLDSIZE);
	m_iMIndexY = (int)(MPos.y / FOOTHOLDSIZE);

	if (m_iMIndexX < 0)
		m_iMIndexX = 0;
	if (m_iMIndexX > m_pPartClone->GetFHWidth() - 1)
		m_iMIndexX = m_pPartClone->GetFHWidth() - 1;

	if (m_iMIndexY < 0)
		m_iMIndexY = 0;
	if (m_iMIndexY > m_pPartClone->GetFHHeight() - 1)
		m_iMIndexY = m_pPartClone->GetFHHeight() - 1;
}

class KFindToolObjectRef : public std::unary_function< CToolObject, bool >
{
public:
	KFindToolObjectRef(int iIndexX, int iIndexY) : m_iIndexX(iIndexX), m_iIndexY(iIndexY) {}

	bool operator()(CToolObject& comp) const
	{
		return (comp.GetIndexX() == m_iIndexX) && (comp.GetIndexY() == m_iIndexY);
	}

	int m_iIndexX;
	int m_iIndexY;
};

void KCreateLayerPartDlg::AddToolObject(int iIndexX, int iIndexY)
{
	if (m_pPartClone == NULL)
		return;
	if (iIndexX < 0 || iIndexX >= m_pPartClone->GetFHWidth())
		return;
	if (iIndexY < 0 || iIndexY >= m_pPartClone->GetFHHeight())
		return;

	std::vector< CToolObject >::iterator vit = std::find_if(m_vecCurObj.begin(), m_vecCurObj.end(),
		KFindToolObjectRef(iIndexX, iIndexY));

	// 없으면 추가
	if (vit == m_vecCurObj.end())
	{
		CToolObject Temp;
		Temp.SetEyeInfo(static_cast<CRenderEyeInfo*>(this));
		Temp.SetBlockInfo(static_cast<CFHBlockInfo*>(m_pPartClone));
		Temp.SetPos(iIndexX, iIndexY, CFootHold::FHT_HEAD);
		m_vecCurObj.push_back(Temp);
	}
}

void KCreateLayerPartDlg::ClearCurObj(bool bClearAll/* = true */)
{
	if (m_vecCurObj.empty())
		return;

	// [8/16/2007 breadceo] 이제 안쓴다...
	int i;
	if (bClearAll)
	{
		i = 0;
	}
	else
	{
		i = 1;
	}
	m_vecCurObj.clear();
}

class KFunctorAddHead
	: std::unary_function< CToolObject, void >
{
public:
	KFunctorAddHead(std::vector< CToolObject >& vecHead) : m_vecHead(vecHead) {}

	void operator()(CToolObject& _Left)
	{
		if (_Left.GetType() == CFootHold::FHT_HEAD)
		{
			std::vector< CToolObject >::iterator vit;
			vit = std::find(m_vecHead.begin(), m_vecHead.end(), _Left);
			if (vit == m_vecHead.end())
				m_vecHead.push_back(_Left);
		}
	}
	std::vector< CToolObject >& m_vecHead;
};

void KCreateLayerPartDlg::RenderFH()
{
	if (m_pPartClone == NULL)
		return;

	std::vector< CToolObject > vecHead;
	std::vector< CToolObject >& vecTO = m_pPartClone->GetToolObjectVector();

	vecHead = std::for_each(vecTO.begin(), vecTO.end(), KFunctorAddHead(vecHead)).m_vecHead;
	std::for_each(vecHead.begin(), vecHead.end(),
		boost::bind(&CToolObject::Render, _1));

	std::for_each(m_vecCurObj.begin(), m_vecCurObj.end(),
		boost::bind(&CToolObject::Render, _1));
}

void KCreateLayerPartDlg::StampObject()
{
	if (0 == m_vecCurObj.size() || m_pPartClone == NULL)
		return;

	CCreateLayerPartStampObjCmd* pCmd = new CCreateLayerPartStampObjCmd();
	std::vector< CToolObject >& vecTO = m_pPartClone->GetToolObjectVector();
	pCmd->SetStampObj(m_pPartClone, m_vecCurObj);

	int ZeroIndexX;
	int ZeroIndexY;
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kTO = m_vecCurObj[i];
		if (i == 0)
		{
			ZeroIndexX = kTO.GetIndexX();
			ZeroIndexY = kTO.GetIndexY();
		}
		CToolObject& kObj = vecTO[kTO.GetIndexY() * m_pPartClone->GetFHWidth() + kTO.GetIndexX()];
		kObj.SetEyeInfo(static_cast<CRenderEyeInfo*>(this));
		kObj.SetBlockInfo(static_cast<CFHBlockInfo*>(m_pPartClone));
		kObj.SetPos(kTO.GetIndexX(), kTO.GetIndexY(), CFootHold::FHT_HEAD);
		kObj.SetProp(kTO.GetProp(), "OR");
	}

	ClearCurObj();

	CToolObject kStart;
	kStart.SetEyeInfo(static_cast<CRenderEyeInfo*>(this));
	kStart.SetBlockInfo(static_cast<CFHBlockInfo*>(m_pPartClone));
	kStart.SetPos(ZeroIndexX, ZeroIndexY, CFootHold::FHT_HEAD);
	m_vecCurObj.push_back(kStart);

	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void KCreateLayerPartDlg::UnStampObject()
{
	if (0 == m_vecCurObj.size() || m_pPartClone == NULL)
		return;

	CCreateLayerPartUnStampObj* pCmd = new CCreateLayerPartUnStampObj();
	std::vector< CToolObject >& vecTO = m_pPartClone->GetToolObjectVector();
	pCmd->SetUnStampObj(m_pPartClone, m_vecCurObj);

	int ZeroIndexX;
	int ZeroIndexY;
	for (int i = 0; i < (int)m_vecCurObj.size(); ++i)
	{
		CToolObject& kTO = m_vecCurObj[i];
		if (i == 0)
		{
			ZeroIndexX = kTO.GetIndexX();
			ZeroIndexY = kTO.GetIndexY();
		}
		CToolObject& kObj = vecTO[kTO.GetIndexY() * m_pPartClone->GetFHWidth() + kTO.GetIndexX()];
		kObj.SetEyeInfo(static_cast<CRenderEyeInfo*>(this));
		kObj.SetBlockInfo(static_cast<CFHBlockInfo*>(m_pPartClone));
		kObj.SetPos(kTO.GetIndexX(), kTO.GetIndexY(), CFootHold::FHT_INIT);
		kObj.SetProp(kTO.GetProp(), "INIT");
		kObj.SetValue("");
	}

	m_pCommandRecv->AddCommand(static_cast<ICommand*>(pCmd));
	AddLog(pCmd->GetLogStr());
}

void KCreateLayerPartDlg::AddLog(const std::string& strLog)
{
	int nItem = m_listLog.GetItemCount();
	nItem = m_listLog.InsertItem(nItem, strLog.c_str());
	m_listLog.EnsureVisible(nItem, FALSE);
}

void KCreateLayerPartDlg::ClearLog()
{
	m_listLog.DeleteAllItems();
	m_pCommandRecv->Init();
}

void KCreateLayerPartDlg::OnAccelRedo()
{
	if (m_pCommandRecv->Redo() == true)
		AddLog(m_pCommandRecv->GetLastLog());
}

void KCreateLayerPartDlg::OnAccelUndo()
{
	if (m_pCommandRecv->Undo() == true)
		AddLog(m_pCommandRecv->GetLastLog());
}

void KCreateLayerPartDlg::SetClonePart(CLayerPart* pPart)
{
	SAFE_DELETE(m_pPartClone);
	m_pPartClone = pPart->Clone();
}

void KCreateLayerPartDlg::OnEnChangeLayerWidth()
{
	UpdateData();
	m_fLayerWidth = static_cast<float>(strtod(m_strLayerWidth.GetBuffer(), NULL));
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnEnChangeLayerHeight()
{
	UpdateData();
	m_fLayerHeight = static_cast<float>(strtod(m_strLayerHeight.GetBuffer(), NULL));
	UpdateLayerPart();
}

void KCreateLayerPartDlg::OnBnClickedShowParticleInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CParticleListDlg dlg;
	dlg.SetControlValue(m_pPartClone);
	dlg.DoModal();
	UpdateControlData();
}

void KCreateLayerPartDlg::OnBnClickedBtnTexuv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pPartClone != NULL)
	{
		UpdateData();

		m_pPartClone->SaveAndResetVertex();
		KLayerRect<float> layerRect;
		KLayerRect<int> texRect;
		m_pPartClone->GetLayerRect(layerRect);
		m_pPartClone->GetTexRect(texRect);

		PartX = texRect.m_X;
		PartY = texRect.m_Y;
		PartWidth = texRect.m_Width;
		PartHeight = texRect.m_Height;

		m_fLayerWidth = layerRect.m_X + layerRect.m_Width;
		m_fLayerHeight = layerRect.m_Y + layerRect.m_Height;
		UpdateLayerPart();
	}
}

void KCreateLayerPartDlg::DrawRectangle(long tx, long ty, long bx, long by)
{
	CRect gap(0, 0, 0, 0);
	ClientToScreen(&gap);
	//m_ctlTexture.ScreenToClient(&gap);

	CRect rect;
	rect.SetRect(tx, ty, bx, by);
	//GetTextureViewRect(rect);

	rect.left -= gap.left;
	rect.bottom -= gap.bottom;
	rect.right -= gap.right;
	rect.top -= gap.top;

	CDC* pdc = GetDC();
	CBrush NullBrush;
	NullBrush.CreateStockObject(NULL_BRUSH);

	CPen pen(PS_SOLID, 2, RGB(255, 0, 102));

	CPen *poldpen = pdc->SelectObject(&pen);
	CBrush *pOldBr = pdc->SelectObject(&NullBrush);

	pdc->Rectangle(rect);
	pdc->SelectObject(pOldBr);
	pdc->SelectObject(poldpen);

	NullBrush.DeleteObject();
	pen.DeleteObject();
	ReleaseDC(pdc);
}