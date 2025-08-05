// TextureViewDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"

#include <cmath>
#include "UI Script Studio.h"
#include "TextureViewDlg.h"
#include ".\textureviewdlg.h"
#include "KWndManager.h"
#include "..\jpeg/header\ximage.h"
#include "MagnificationDlg.h"

extern KWndManager*	g_WndMgr;
extern CMagnifiCationDlg    *g_Magnification;

// CTextureViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTextureViewDlg, CDialog)
CTextureViewDlg::CTextureViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureViewDlg::IDD, pParent)
	, m_strTexturePath(_T(""))
	, m_bRect(false)
{
	m_pD3D       = NULL; // Used to create the D3DDevice
	m_pd3dDevice = NULL; // Our rendering device
	m_pSprite	 = NULL;
	m_pSprite2	 = NULL;
	m_pCombo	 = NULL;
    m_pTexture   = NULL;

	m_bDrag		= false;
	m_bDraw		= false;
	m_bRect		= false;
	m_bOnMove	= false;
	m_bOnSize	= false;
	m_bDoMove	= false;
	m_bDoSize	= false;

	m_KeyPress	= false;

	m_vStart.x  = 0;
	m_vStart.y	= 0;
	m_vEnd.x    = 0;
	m_vEnd.y	= 0;
	m_nowPoint.x = 0;
	m_nowPoint.y = 0;

	m_width		 = 0;
	m_height	 = 0;
	m_Magnification_X = 0.0;
	m_Magnification_Y = 0.0;

	m_RectSize.SetPoint(1000, 513);

	m_RectDef.SetRect(0,0,0,0);
	m_RectHov.SetRect(0,0,0,0);
	m_RectSel.SetRect(0,0,0,0);
	m_RectAct.SetRect(0,0,0,0);
	m_RectLoc.SetRect(0,0,0,0);

	m_iDefNum = -1;
	m_iHovNum = -1;
	m_iSelNum = -1;
	m_iActNum = -1;
	m_iLocNum = -1;

	m_bShiftState	= false;
	m_bCtrlState	= false;

	m_iRvalue1 = 0;
	m_iGvalue1 = 102;
	m_iBvalue1 = 0;

    // 노란색은 눈아파열ㅠ ㅅ ㅠ
	//m_iRvalue2 = 255;
	//m_iGvalue2 = 255;
	//m_iBvalue2 = 0;
    m_iRvalue2 = 0;
    m_iGvalue2 = 102;
    m_iBvalue2 = 0;

	//	이동석 : 왜 초기화 리스트에 넣지 않았는가 의문이 든다.
	//	코드분석 완료하면 죄다 옮길예정.
	m_fMagnification	=	1.0f;
	m_iAdd_x			=	0;
	m_iAdd_y			=	0;
	m_bRDrag			=	false;

    m_strFindString     =   _T("");
}

CTextureViewDlg::~CTextureViewDlg()
{
}

void CTextureViewDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEXTURE, m_ctlTexture);
    DDX_Control(pDX, IDC_TEXTURENAME, m_TextureFileName);
    DDX_Control(pDX, IDC_STATE, m_cbTexState);
    DDX_Control(pDX, IDC_COMBO_FIND_TEXT, m_ctrlComboFindString);
}


BEGIN_MESSAGE_MAP(CTextureViewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_START_X, OnEnChangeStartX)
	ON_EN_CHANGE(IDC_START_Y, OnEnChangeStartY)
	ON_EN_CHANGE(IDC_END_X, OnEnChangeEndX)
	ON_EN_CHANGE(IDC_END_Y, OnEnChangeEndY)
	ON_EN_CHANGE(IDC_WIDTH, OnEnChangeWidth)
	ON_EN_CHANGE(IDC_HEIGHT, OnEnChangeHeight)
	ON_CBN_SELCHANGE(IDC_STATE, OnCbnSelchangeState)
	ON_CBN_SELCHANGE(IDC_TEXTURENAME, OnComboSelectChange)
	ON_BN_CLICKED(IDC_GOMAIN, OnBnClickedGomain)
	ON_BN_CLICKED(IDC_SETSIZE, OnBnClickedSetsize)
	ON_BN_CLICKED(IDC_ORIGINAL_SIZE, OnBnClickedOriginalSize)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHANGECOLOR, OnBnClickedChangecolor)
	ON_BN_CLICKED(IDC_CHANGECOLOR2, OnBnClickedChangecolor2)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
    ON_CBN_EDITCHANGE(IDC_COMBO_FIND_TEXT, &CTextureViewDlg::OnCbnEditchangeComboFindText)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CTextureViewDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

HRESULT CTextureViewDlg::InitD3D( HWND hWnd )
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Device state would normally be set here
	return S_OK;
}

VOID CTextureViewDlg::Cleanup()
{
	for(std::map<int,LPDIRECT3DTEXTURE9>::iterator mIt = m_mapTexture.begin(); mIt != m_mapTexture.end() ; mIt++)
    {
        if(mIt->second != NULL )
            mIt->second->Release();
    }

	for( int i = 0; i < (int)m_vecString.size(); i++ )
	{
		if( m_vecString[i] )
			m_vecString[i].ReleaseBuffer();
	}

	if( m_pd3dDevice != NULL) 
		m_pd3dDevice->Release();

	if( m_pD3D != NULL)
		m_pD3D->Release();

	/*if( m_pSprite != NULL )
		m_pSprite->Release();

	if( m_pSprite2 != NULL )
		m_pSprite2->Release();*/

	SAFE_RELEASE( m_pSprite );
	SAFE_RELEASE( m_pSprite2 );
}

VOID CTextureViewDlg::RenderWindow()
{	
	CComboBox *pCombo = (CComboBox*)GetDlgItem( IDC_TEXTURENAME);
	if( pCombo->GetCurSel() == -1 )
		return;
	if( m_mapTexture.empty() )
		return;

	CRect rect;
	rect.SetRect( m_vStart.x , m_vStart.y , m_vEnd.x , m_vEnd.y);
	ClientToScreen(&rect);
	m_ctlTexture.ScreenToClient(&rect);

	D3DXMATRIX mat, matRot;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixIdentity(&matRot);
	float tex_width, tex_height;
	float X, Y;
	int width = m_width, height = m_height;
	if( (m_vStart.x < m_vEnd.x) && (m_vStart.y > m_vEnd.y)) // 우상방향으로 긋는 경우
	{
		D3DXMatrixRotationZ(&matRot,D3DXToRadian(90.0f));
		X= cos(D3DXToRadian(-90.0f))*550 - sin(D3DXToRadian(-90.0f))*50;
		Y = sin(D3DXToRadian(-90.0f))*550 + cos(D3DXToRadian(-90.0f))*50;
		tex_width = m_vStart.y - m_vEnd.y;
		tex_height = m_vEnd.x - m_vStart.x;
		width = m_height;
		height = m_width;
	}
	else if( (m_vStart.x > m_vEnd.x) && ( m_vStart.y < m_vEnd.y )) // 좌하방향으로 긋는 경우
	{
		D3DXMatrixRotationZ(&matRot,D3DXToRadian(-90.0f));
		X = cos(D3DXToRadian(90.0f))*550 - sin(D3DXToRadian(90.0f))*50;
		Y = sin(D3DXToRadian(90.0f))*550 + cos(D3DXToRadian(90.0f))*50;
		tex_width = m_vEnd.y - m_vStart.y;
		tex_height = m_vStart.x - m_vEnd.x;
		width = m_height;
		height = m_width;
	}
	//	20091008 이동석, 확대 축소 기능 추가 : 그런데 왜 전임자가 이렇게 코딩했는지는 파악하지 않았다...
	else if( (m_vStart.x > m_vEnd.x) && ( m_vStart.y > m_vEnd.y )) // 좌상방향으로 긋는 경우
	{
		D3DXMatrixRotationZ(&matRot,D3DXToRadian(180.0f));
		X = cos(D3DXToRadian(180.0f))*550 - sin(D3DXToRadian(180.0f))*50;
		Y = sin(D3DXToRadian(180.0f))*550 + cos(D3DXToRadian(180.0f))*50;
		tex_width = m_vStart.y - m_vEnd.y;
		tex_height = m_vStart.x - m_vEnd.x;
		width = m_height;
		height = m_width;
	}
	else //그 외의 경우
	{
		tex_width = m_vEnd.x - m_vStart.x;
		tex_height = m_vEnd.y - m_vStart.y;
		X = 550;
		Y = 50;
	}

	//float scaling_x = ((float)width)/(tex_width);
	//float scaling_y = ((float)height)/(tex_height);
	//D3DXMatrixScaling(&mat,scaling_x,scaling_y,1.0f);

	D3DXMatrixScaling(&mat,m_fMagnification,m_fMagnification,1.0f);
	
	mat = mat * matRot;
	mat._41	=	550.0f + m_iAdd_x;
	mat._42	=	50.0f + m_iAdd_y;
	mat._43	=	0.0f;

	//D3DXVECTOR3 pos(X/scaling_x,Y/scaling_y,0);
    if ( m_mapTexture.find(pCombo->GetCurSel()) != m_mapTexture.end() ){
        if ( NULL != m_mapTexture[pCombo->GetCurSel()] ) {
        m_pSprite2->Begin(D3DXSPRITE_ALPHABLEND );
        m_pSprite2->SetTransform(&mat);
		m_pSprite2->Draw(m_mapTexture[pCombo->GetCurSel()],&rect,NULL,NULL,0xffffffff);
        m_pSprite2->End();
        rect.SetRect( 0, 0, 512, 512 );
        m_pSprite->Begin(D3DXSPRITE_ALPHABLEND );
        m_pSprite->Draw(m_mapTexture[pCombo->GetCurSel()],&rect,NULL,NULL,0xffffffff);
        m_pSprite->End();
        m_pTexture = m_mapTexture[pCombo->GetCurSel()];
    }
    }
}

VOID CTextureViewDlg::Render()
{
	if( NULL == m_pd3dDevice )
		return;
	
	//	20091013 이동석
	//	아래의 코드를 사용하면 더 깔끔할것 같은데, 이렇게 하면 배경색이 바뀌지 않는다. 대체 왜...
	//D3DRECT	RectArr[] =
	//{
	//	{ 0,   0,  512,           512,           },
	//	{ 512, 0,  1000,          512,           },
	//	{ 550, 50, 550 + m_width, 50 + m_height, },
	//};

	//m_pd3dDevice->Clear( 1, RectArr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue1,m_iGvalue1,m_iBvalue1), 1.0f, 0 );
	//m_pd3dDevice->Clear( 2, RectArr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue2,m_iGvalue2,m_iBvalue2), 1.0f, 0 );
	//m_pd3dDevice->Clear( 3, RectArr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue1,m_iGvalue1,m_iBvalue1), 1.0f, 0 );

	D3DRECT Rect1; // 좌측
	D3DRECT Rect2; // 우측
	D3DRECT Rect3; // 좌측에서 따온만큼 표시되는 영역
	Rect1.x1 = 0;   Rect1.x2 = 512;   Rect1.y1 = 0;   Rect1.y2 = 512;
	Rect2.x1 = 512; Rect2.x2 = 1000;  Rect2.y1 = 0;   Rect2.y2 = 512;
	Rect3.x1 = 550 + m_iAdd_x;   Rect3.x2 = (550+ (m_width*m_fMagnification) +m_iAdd_x );   Rect3.y1 = 50+m_iAdd_y;   Rect3.y2 = (50+ (m_height*m_fMagnification)+m_iAdd_y );
	m_pd3dDevice->Clear( 1, &Rect1, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue1,m_iGvalue1,m_iBvalue1), 1.0f, 0 );
	m_pd3dDevice->Clear( 1, &Rect2, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue2,m_iGvalue2,m_iBvalue2), 1.0f, 0 );
	m_pd3dDevice->Clear( 1, &Rect3, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_iRvalue1,m_iGvalue1,m_iBvalue1), 1.0f, 0 );


	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		RenderWindow();
		m_pd3dDevice->EndScene();
	}
	// Present the backbuffer contents to the display
	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void CTextureViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Render();

	if( (m_vStart.x != m_vEnd.x) || (m_vStart.y != m_vEnd.y) )
	{	DrawRectangle( m_vStart.x, m_vStart.y, m_vEnd.x, m_vEnd.y );   }

	if( m_bOnMove == true && m_bRect == true )
	{	DrawMoveRect();   }
	else if( m_bOnSize == true && m_bRect == true )
	{	DrawSizeRect();   }

	MouseMove();
	DrawCoordinateAxis();
}

void CTextureViewDlg::OnDestroy()
{
	CDialog::OnDestroy();

	Cleanup();
}

BOOL CTextureViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctlTexture.SetWindowPos(NULL,0,10,m_RectSize.x+1,m_RectSize.y+1,SWP_NOMOVE|SWP_NOZORDER);
	
	InitD3D( m_ctlTexture.GetSafeHwnd() );

	//Sprite : 2D 그리는데 좋아? 
	//SAFE_RELEASE( m_pSprite );
	//SAFE_RELEASE( m_pSprite2 );

	D3DXCreateSprite(m_pd3dDevice,&m_pSprite);
	D3DXCreateSprite(m_pd3dDevice,&m_pSprite2);
	
	SetDlgItemText(IDC_TEXTURENAME, _T("작업할 Texture를 선택하세요"));
	SetDlgItemText(IDC_STATE, _T("어떤 상태에 적용할 Texture"));

	SetEditBox();
	return false;
}

void CTextureViewDlg::OnComboSelectChange()
{
	Render();
}

void CTextureViewDlg::AddFileName(CString Name)
{
	m_strArrayName.Add( Name );
}

void CTextureViewDlg::LoadTexture()
{
	

	m_pCombo = (CComboBox*)GetDlgItem(IDC_STATE);	
	m_pCombo->AddString(_T("def_tex_idx"));
	m_pCombo->AddString(_T("hov_tex_idx"));
	m_pCombo->AddString(_T("sel_tex_idx"));
	m_pCombo->AddString(_T("act_tex_idx"));
	m_pCombo->AddString(_T("loc_tex_idx"));

	m_pCombo = (CComboBox*)GetDlgItem(IDC_TEXTURENAME);

	for(UINT i = 0; i < m_strArrayName.GetSize(); i++)
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		/*
		if( FAILED( hr = D3DXCreateTextureFromFile( g_pd3dDevice, m_vecStrTexFileName[i].c_str() , &pTexture ) ) )
		return hr;
		*/

		std::string texName = ToCharStr( m_strArrayName[i].GetBuffer() );

		GCMassFile::MASSFILE_MEMBERFILEINFO* Info = g_WndMgr->GetMassFileManager()->LoadDataFile( texName );

		if( Info != NULL )
		{
		    D3DXIMAGE_INFO	TexInfo;

		    D3DFORMAT dwFormat = D3DFMT_UNKNOWN;

		    if( FAILED(D3DXCreateTextureFromFileInMemoryEx( m_pd3dDevice, (void*)Info->pRealData, Info->ulSize,
			    D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, dwFormat/*3DFMT_A8R8G8B8*/, 
			    D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, 
			    &TexInfo,
			    NULL, &pTexture ) ))
		    {		
			    pTexture = NULL;
		    }
		}

            m_mapTexture.insert(std::pair<int,LPDIRECT3DTEXTURE9>(i,pTexture));

		CString temp;
		temp.Format( _T("%d번 %s"),i, m_strArrayName[i]);
		m_vecString.push_back( temp );
		m_pCombo->AddString( temp.GetString() );
	}	
}

void CTextureViewDlg::GetTextureFilePath(CString FilePath)
{	m_strTexturePath = FilePath;   }

void CTextureViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    
	//	20091008 이동석, 작업영역 이외 클릭시 입력 무시
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);
	CPoint rp( m_nowPoint.x + gap.left, m_nowPoint.y + gap.top );

	if( rp.x >= 0 &&
		rp.x < 512 &&
		rp.y >= 0 &&
		rp.y < 512 )
	{}
	else if( rp.x > 512 &&
			rp.x < 1000 &&
			rp.y >= 0 &&
			rp.y < 512 )
	{
		m_BtnDownOnRviewPoint = rp;
		m_RviewAddPoint.x	=	m_iAdd_x + gap.left;
		m_RviewAddPoint.y	=	m_iAdd_y + gap.top;
		m_bRDrag = true;
		return;
	}
	else
		return;
	//////////////////////////////////////////////////////////////////////////


	if( m_bOnMove == true )		// Rectangle 좌측상단에 찍었을 때 (이동)
	{
		m_bDoMove = true;
		m_bDrag = true;
	}

	else if( m_bOnSize == true )	// Rectangle 우측하단에 찍었을 때 (크기)
	{
		m_bDoSize = true;
		m_bDrag = true;
	}

	else		// 맨땅에 찍었을 때
	{
		m_bRect = false;
		m_bDrag = true;

		m_vStart.x = m_nowPoint.x;
		m_vStart.y = m_nowPoint.y;
		m_vEnd.x = m_nowPoint.x;
		m_vEnd.y = m_nowPoint.y;
	} 

	SetEditBox();
	OnPaint();
	CDialog::OnLButtonDown(nFlags, point);
}

void CTextureViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    if( !m_pTexture )
        return;

	if( m_bDraw == true )
	{
		m_bDraw = false;
		m_bRect = true;
	}
	m_bDrag	  = false;
	m_bDoMove = false;
	m_bDoSize = false;

	//	20091013 이동석
	if( m_bRDrag )
	{
		m_bRDrag = false;
		
	}
	

    //////////////////////////////////////////////////////////////////////////
    // jemitgge 오후 9:45 2008-02-13 
    // 드래그 해서 왼버튼 업 했을 경우에, 범위 안에 속하는 픽셀들을 모두 검사하여
    // 알파 값이 있는 모서리에 맞게 사각형 축소!
	// milennium9 오후 10:15 2008-07-24
	// 문자열은 여유를 둘 필요가 있기 때문에 Ctrl 키 눌렀을 때만 축소 적용.
    //////////////////////////////////////////////////////////////////////////
	if( ::GetAsyncKeyState( VK_LCONTROL ) )
	{
		D3DLOCKED_RECT	d3drc;
		m_pTexture->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

		CRect gap(0,0,0,0);
		ClientToScreen(&gap);
		m_ctlTexture.ScreenToClient(&gap);

		// top
		bool bIsCheck = FALSE;
		for (int j = m_vStart.y ; j < m_vEnd.y ; j++)
		{
			for (int i = m_vStart.x ; i < m_vEnd.x ; i++)
			{
				if ( 0x00 < (*((LPDWORD)d3drc.pBits + (i+gap.left) + (j+gap.top) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
				{
					m_vStart.y = j;
					bIsCheck = true;
					break;
				}
			}
			if (bIsCheck)
				break;
		}

		// left
		bIsCheck = FALSE;
		for (int i = m_vStart.x ; i < m_vEnd.x ; i++)
		{
			for (int j = m_vStart.y ; j < m_vEnd.y ; j++)
			{
				if ( 0x00 < (*((LPDWORD)d3drc.pBits + (i+gap.left) + (j+gap.top) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
				{
					m_vStart.x = i;
					bIsCheck = true;
					break;
				}
			}
			if (bIsCheck)
				break;
		}

		// bottom
		bIsCheck = FALSE;
		for (int j = m_vEnd.y ; j > m_vStart.y ; j--)
		{
			for (int i = m_vStart.x ; i < m_vEnd.x ; i++)
			{
				if ( 0x00 < (*((LPDWORD)d3drc.pBits + (i+gap.left) + (j+gap.top) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
				{
					m_vEnd.y = j+1;
					bIsCheck = true;
					break;
				}
			}
			if (bIsCheck)
				break;
		}

		// right
		bIsCheck = FALSE;
		for (int j = m_vEnd.x ; j > m_vStart.x ; j--)
		{
			for (int i = m_vStart.y ; i < m_vEnd.y ; i++)
			{
				if ( 0x00 < (*((LPDWORD)d3drc.pBits + (j+gap.left) + (i+gap.top) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
				{
					m_vEnd.x = j+1;
					bIsCheck = true;
					break;
				}
			}
			if (bIsCheck)
				break;
		}

		m_pTexture->UnlockRect(0);
	}
    //////////////////////////////////////////////////////////////////////////

    if( ::GetAsyncKeyState( VK_LMENU ) )
    {
        D3DLOCKED_RECT	d3drc;
        m_pTexture->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

        CRect gap(0,0,0,0);
        ClientToScreen(&gap);
        m_ctlTexture.ScreenToClient(&gap);

        CPoint startPoint = point;
        startPoint.x = m_vEnd.x + gap.left;
        startPoint.y = m_vEnd.y + gap.top;

		//	땜빵 디버그 코드, 20091016 이동석
		if( startPoint.x >= 512 )
			startPoint.x = 511;
		if( startPoint.x < 0 )
			startPoint.x = 0;
		if( startPoint.y >= 512 )
			startPoint.y = 511;
		if( startPoint.y < 0 )
			startPoint.y = 0;
		//	아래의 픽셀 메모리 조작 코드가 픽셀 영역을 벗어나는 주소를 참조함.
		//	그래서 뻑! 뿜!
		//	화면 영역은 512 * 512
		//////////////////////////////////////////////////////////////////////////


        // top
        bool bIsCheck = false;

        int itop = 0;
        int ibottom = 0;
        int ileft = 0;
        int iright = 0;

        bool bEnd = false;
        while( !bEnd )
        {
             bEnd = true;
            for( int i = ileft ; i <= iright ; ++i )
            {
                if ( 0x00 < (*((LPDWORD)d3drc.pBits + (i+startPoint.x) + (startPoint.y-1+itop) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
                {
                    bIsCheck = true;
                    break;
                }
            }

            if( bIsCheck )
            {
                if( startPoint.y-1+itop > 0 )
                {
                    --itop;
                    bEnd &= false;
                }
                bIsCheck = false;
            }

            for( int i = ileft ; i <= iright ; ++i )
            {
                if ( 0x00 < (*((LPDWORD)d3drc.pBits + (i+startPoint.x) + (ibottom+1+startPoint.y) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
                {
                    bIsCheck = true;
                    break;
                }
            }

            if( bIsCheck )
            {
                if( ibottom+1+startPoint.y < 512 )
                {
                    ++ibottom;
                    bEnd &= false;
                }
                bIsCheck = false;
            }

            for( int i = itop ; i <= ibottom ; ++i )
            {
                if ( 0x00 < (*((LPDWORD)d3drc.pBits + (startPoint.x-1+ileft) + (i+startPoint.y) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
                {
                    bIsCheck = true;
                    break;
                }
            }

            if( bIsCheck )
            {
                if( startPoint.x-1+ileft > 0 )
                {
                    --ileft;
                    bEnd &= false;
                }
                bIsCheck = false;
            }

            for( int i = itop ; i <= ibottom ; ++i )
            {
                if ( 0x00 < (*((LPDWORD)d3drc.pBits + (iright+1+startPoint.x) + (i+startPoint.y) * (d3drc.Pitch/4)) & 0xff000000) >> 24 )
                {
                    bIsCheck = true;
                    break;
                }
            }

            if( bIsCheck )
            {
                if( iright+1+startPoint.x < 512 )
                {
                    ++iright;
                    bEnd &= false;
                }
                bIsCheck = false;
            }
        }

        m_vStart.x = startPoint.x + ileft - gap.left;
        m_vStart.y = startPoint.y + itop - gap.top;
        m_vEnd.x = startPoint.x + iright - gap.left + 1;
        m_vEnd.y = startPoint.y + ibottom - gap.top + 1;

        m_pTexture->UnlockRect(0);
    }

    int sub = m_vEnd.x - m_vStart.x;
    if( sub < 0 ) sub = sub*(-1);
    m_width = sub;

    sub = m_vEnd.y - m_vStart.y;
    if( sub < 0 ) sub = sub*(-1);
    m_height = sub;
	
	SetEditBox();
	OnPaint();
	CDialog::OnLButtonUp(nFlags, point);
}

void CTextureViewDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	SetEditBox();
	OnPaint();
    g_Magnification->CaptureScreen();
	CDialog::OnMouseMove(nFlags, point);
}

void CTextureViewDlg::MouseMove()
{
	GetCursorPos(&m_nowPoint);
	ScreenToClient(&m_nowPoint);

	// Rectangle 조절칸에 들어올 때
	if( (m_bOnMove==false) && (m_bOnSize==false) )
	{
		// Rectangle 좌측상단 검사
		if( m_bDraw == false )
		{
			if( Cursor_OnRectSize() == true )
			{	m_bOnSize = true;   }
			// Rectangle 우측하단 검사
			else if( Cursor_OnRectMove() == true )
			{	m_bOnMove = true;   }
		}
	}
	// Rectangle 조절칸에서 나갈 때
	else		// if( (m_bOnMove==true) || (m_bOnSize==true) )
	{
		// Rectangle 외의 좌표
		if( (Cursor_OnRectMove()==false) && (Cursor_OnRectSize()==false) )
		{
			m_bOnMove = false;
			m_bOnSize = false;
		}
	}

	/*20091013 이동석*/
	if( m_bRDrag )
	{
		m_iAdd_x	=	m_RviewAddPoint.x + m_nowPoint.x - m_BtnDownOnRviewPoint.x;
		m_iAdd_y	=	m_RviewAddPoint.y + m_nowPoint.y - m_BtnDownOnRviewPoint.y;
	}

	// 드래그 중일 때
	if( m_bDrag == true )
	{
		// Rectangle이 이미 그려져 있다면
		if( m_bRect == true )
		{
			// Rectangle 위치 이동
			if( m_bDoMove == true )
			{
				FLOAT tmpX = m_vStart.x;
				FLOAT tmpY = m_vStart.y;

				m_vStart.x = m_nowPoint.x;
				m_vStart.y = m_nowPoint.y;
				m_vEnd.x += (m_vStart.x-tmpX);
				m_vEnd.y += (m_vStart.y-tmpY);
			}
			// Rectangle 크기 조절
			else if( m_bDoSize == true )
			{
				if( m_bShiftState )
				{
					m_vEnd.x = m_nowPoint.x;
					m_vEnd.y = m_vStart.y + (m_nowPoint.x-m_vStart.x);
				}
				else
				{
					m_vEnd.x = m_nowPoint.x;
					m_vEnd.y = m_nowPoint.y;
				}

				m_bDraw = true;
			}
		}
		// Rectangle 새로 그리기
		else
		{
			if( m_bShiftState )
			{
				m_vEnd.x = m_nowPoint.x;
				m_vEnd.y = m_vStart.y + (m_nowPoint.x-m_vStart.x);
			}
			else	
			{
				m_vEnd.x = m_nowPoint.x;
				m_vEnd.y = m_nowPoint.y;
			}

			m_bDraw = true;	// 현재 그리는 중
		}
	}

	SetEditBox();
}

void CTextureViewDlg::DrawRectangle(long tx, long ty, long bx, long by )
{
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);

	CRect rect;
	rect.SetRect( tx, ty, bx, by);
	GetTextureViewRect(rect);
	
	rect.left -= gap.left;
	rect.bottom -= gap.bottom;
	rect.right -= gap.right;
	rect.top -= gap.top;

	CDC* pdc = GetDC();
	CBrush NullBrush;
	NullBrush.CreateStockObject(NULL_BRUSH);

	CPen pen(PS_SOLID, 2, RGB(255,0,102));

	CPen *poldpen = pdc->SelectObject(&pen);
	CBrush *pOldBr = pdc->SelectObject(&NullBrush);

	pdc->Rectangle(rect);
	pdc->SelectObject( pOldBr );
	pdc->SelectObject(poldpen);

	NullBrush.DeleteObject();
	pen.DeleteObject();
	ReleaseDC(pdc);
}

void CTextureViewDlg::DrawCoordinateAxis()
{
	CDC* pdc = GetDC();
	CPen pen( PS_SOLID, 1, RGB((255-m_iRvalue1), (255-m_iGvalue1), (255-m_iBvalue1)) );
	CPen *poldpen = pdc->SelectObject(&pen);
	CPoint Point2DrawLine;

	if( Cursor_InTextureRect() == false )
	{	return;   }

	// 신나는 가로줄 그리기
	Point2DrawLine.x = 13;	// x축 시작점
	Point2DrawLine.y = m_nowPoint.y;
	pdc->MoveTo(Point2DrawLine);
	Point2DrawLine.x = m_RectSize.x+10;	// x축 끝점
	pdc->LineTo(Point2DrawLine);

	// 더 신나는 세로줄 그리기
	Point2DrawLine.x = m_nowPoint.x;
	Point2DrawLine.y = 35;	// y축 시작점
	pdc->MoveTo(Point2DrawLine);
	Point2DrawLine.y = m_RectSize.y+32;	// y축 끝점
	pdc->LineTo(Point2DrawLine);

	// 뒷정리 'ㅅ' 뿌우
	pen.DeleteObject();
	ReleaseDC(pdc);
}

void CTextureViewDlg::DrawMoveRect()
{
	if( Cursor_InTextureRect() == false )
	{	return;   }

	CDC* pdc = GetDC();
	CPen pen(PS_SOLID, 2, RGB(255,255,0));
	CPen *poldpen = pdc->SelectObject(&pen);
	CPoint point;

	CBrush NullBrush;
	NullBrush.CreateStockObject(NULL_BRUSH);

	pdc->Rectangle((m_vStart.x-1), (m_vStart.y-1), (m_vStart.x+9), (m_vStart.y+9));
	pdc->SelectObject(poldpen);

	NullBrush.DeleteObject();
	pen.DeleteObject();
	ReleaseDC(pdc);
}

void CTextureViewDlg::DrawSizeRect()
{
	if( Cursor_InTextureRect() == false )
	{	return;   }

	CDC* pdc = GetDC();
	CPen pen(PS_SOLID, 2, RGB(255,255,0));
	CPen *poldpen = pdc->SelectObject(&pen);
	CPoint point;

	CBrush NullBrush;
	NullBrush.CreateStockObject(NULL_BRUSH);

	pdc->Rectangle((m_vEnd.x-9), (m_vEnd.y-9), (m_vEnd.x+1), (m_vEnd.y+1));
	pdc->SelectObject(poldpen);

	NullBrush.DeleteObject();
	pen.DeleteObject();
	ReleaseDC(pdc);
}

void CTextureViewDlg::GetTextureViewRect(CRect &rect)
{
	ClientToScreen(&rect);
	m_ctlTexture.ScreenToClient(&rect);

	if(rect.left < 0)
		rect.left = 0;
	else if( rect.left > m_RectSize.y )
		rect.left = m_RectSize.y;
	if(rect.top < 0)
		rect.top = 0;
	else if( rect.top > m_RectSize.y )
		rect.top = m_RectSize.y;
	if(rect.bottom > m_RectSize.y)
		rect.bottom = m_RectSize.y;
	else if( rect.bottom < 0 )
		rect.bottom = 0;
	if(rect.right > m_RectSize.y ) 
		rect.right = m_RectSize.y;
	else if( rect.right < 0)
		rect.right = 0;
}

void CTextureViewDlg::SetEditBox()
{
	CRect TextureRect;
	TextureRect.SetRect( m_vStart.x, m_vStart.y, m_vEnd.x, m_vEnd.y );
	GetTextureViewRect(TextureRect);
	
	CString temp;
	temp.Format( _T("%d"),TextureRect.left );
	SetDlgItemText(IDC_START_X, temp);
	temp.Format(  _T("%d"), TextureRect.top );
	SetDlgItemText( IDC_START_Y, temp);
	temp.Format(  _T("%d"),TextureRect.right );
	SetDlgItemText( IDC_END_X, temp );
	temp.Format(  _T("%d"),TextureRect.bottom );
	SetDlgItemText( IDC_END_Y, temp );
	
	temp.Format( _T("%d"), m_width);
	SetDlgItemText( IDC_WIDTH, temp );
	temp.Format( _T("%d"), m_height);
	SetDlgItemText( IDC_HEIGHT, temp );

	GetMagnification();
	temp.Format( _T("%.2f"), m_Magnification_X );
	SetDlgItemText( IDC_MAGNIFICATION_X, temp );
	temp.Format( _T("%.2f"), m_Magnification_Y );
	SetDlgItemText( IDC_MAGNIFICATION_Y, temp );
}

void CTextureViewDlg::OnEnChangeStartX()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);

	CString temp;
	GetDlgItemText(IDC_START_X, temp);
	m_vStart.x = atoi( ToCharStr( temp.GetBuffer() )) - gap.left;
}

void CTextureViewDlg::OnEnChangeStartY()
{
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	GetDlgItemText(IDC_START_Y, temp);
	m_vStart.y = atoi( ToCharStr( temp )) - gap.top;
}

void CTextureViewDlg::OnEnChangeEndX()
{
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	GetDlgItemText(IDC_END_X, temp);
	m_vEnd.x = atoi(ToCharStr( temp )) - gap.right;
}

void CTextureViewDlg::OnEnChangeEndY()
{
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	GetDlgItemText(IDC_END_Y, temp);
	m_vEnd.y = atoi(ToCharStr( temp )) - gap.bottom;
}

extern KD3DWndClass			*g_pkWndClass;
void CTextureViewDlg::OnBnClickedGomain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !g_pkWndClass )
		return;
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_STATE);
	CComboBox *pComboTexture = (CComboBox*)GetDlgItem(IDC_TEXTURENAME);

	CString x, y, z, w;
	GetDlgItemText(IDC_START_X, x);
	GetDlgItemText(IDC_START_Y, y);
	GetDlgItemText(IDC_END_X, z);
	GetDlgItemText(IDC_END_Y, w);

    D3DXVECTOR4 pVec;

    pVec.x = atoi(ToCharStr( x ));
    pVec.y = atoi(ToCharStr( y ));
    pVec.z = atoi(ToCharStr( z ));
    pVec.w = atoi(ToCharStr( w ));

	switch( pCombo->GetCurSel() )
	{
	case -1:
		MessageBox( _T("어떤 상태로..?"), _T("에러"));
		return;
	case 0:		//def_tex_idx
		g_pkWndClass->SetDefTexCoords( pVec );
		g_pkWndClass->SetDefTextureIDx(pComboTexture->GetCurSel());
		break;
	case 1:		//hov_tex_idx
		g_pkWndClass->SetHovTexCoords( pVec );
		g_pkWndClass->SetHovTextureIDx(pComboTexture->GetCurSel());
		break;
	case 2:		//sel_tex_idx
		g_pkWndClass->SetSelTexCoords( pVec );
		g_pkWndClass->SetSelTextureIDx(pComboTexture->GetCurSel());
		break;
	case 3:		//act_tex_idx
		g_pkWndClass->SetActTexCoords( pVec );
		g_pkWndClass->SetActTextureIDx(pComboTexture->GetCurSel());
		break;
	case 4:		//loc_tex_idx
		g_pkWndClass->SetLocTexCoords( pVec );
		g_pkWndClass->SetLocTextureIDx(pComboTexture->GetCurSel());
		break;
	default:
		MessageBox( _T("어떤 상태로..?"), _T("에러"));
		return;
	}
}

void CTextureViewDlg::OnEnChangeWidth()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	GetDlgItemText(IDC_WIDTH, temp);
	m_width = atoi(ToCharStr( temp ));
}

void CTextureViewDlg::OnEnChangeHeight()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	GetDlgItemText(IDC_HEIGHT, temp);
	m_height = atoi(ToCharStr( temp ));
}

void CTextureViewDlg::KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// VK_LEFT  VK_UP  VK_RIGHT  VK_DOWN  
	int iMulti = 1;
	if( m_KeyPress == true )
	{	iMulti = 5;   }
	if( GetAsyncKeyState( VK_LMENU ) & 0x8000 )
		iMulti = 10;

	/////////////////////// Rectangle Size
	if( m_bShiftState )
	{
		switch( nChar )
		{
		case VK_LEFT:
			m_vEnd.x -= 1 * iMulti;
			break;
		case VK_UP:
			m_vEnd.y -= 1 * iMulti;
			break;
		case VK_RIGHT:
			m_vEnd.x += 1 * iMulti;
			break;
		case VK_DOWN:
			m_vEnd.y += 1 * iMulti;
			break;
		case VK_PRIOR:
			m_vStart.y = 35;
			break;
		case VK_NEXT:
			m_vEnd.y = 546;
			break;
		case VK_HOME:
			m_vStart.x = 13;
			break;
		case VK_END:
			m_vEnd.x = 525;
			break;

		default:
			return;
		}

		int sub = m_vEnd.x - m_vStart.x;
		if( sub < 0 ) sub = sub*(-1);
		m_width = sub;

		sub = m_vEnd.y - m_vStart.y;
		if( sub < 0 ) sub = sub*(-1);
		m_height = sub;

		SetEditBox();
		OnPaint();

		return;
	}

	///////////////////////선택된 영역 배율 조정
	if( m_bCtrlState )
	{
		switch( nChar )
		{
		case VK_LEFT:
			m_width -= 1 * iMulti;
			break;
		case VK_UP:
			m_height -= 1 * iMulti;
			break;
		case VK_RIGHT:
			m_width += 1 * iMulti;
			break;
		case VK_DOWN:
			m_height += 1 * iMulti;
			break;
		case VK_PRIOR:
			m_height -= 15;
			break;
		case VK_NEXT:
			m_height += 15;
			break;
		case VK_HOME:
			m_width -= 15;
			break;
		case VK_END:
			m_width += 15;
			break;

		default:
			return;
		}

		SetEditBox();
		OnPaint();

		return;
	}

	///////////////////////선택된 네모 이동..
	switch( nChar )
	{
	case VK_LEFT:
		m_vStart.x -= 1 * iMulti;
		m_vEnd.x -= 1 * iMulti;
		break;
	case VK_UP:
		m_vStart.y -= 1 * iMulti;
		m_vEnd.y  -= 1 * iMulti;
		break;
	case VK_RIGHT:
		m_vStart.x += 1 * iMulti;
		m_vEnd.x += 1 * iMulti;
		break;
	case VK_DOWN:
		m_vStart.y += 1 * iMulti;
		m_vEnd.y += 1 * iMulti;
		break;
	case VK_PRIOR:
		m_vEnd.y -= (m_vStart.y - 35);
		m_vStart.y = 35;
		break;
	case VK_NEXT:
		m_vStart.y += (546 - m_vEnd.y);
		m_vEnd.y = 546;
		break;
	case VK_HOME:
		m_vEnd.x -= (m_vStart.x - 13);
		m_vStart.x = 13;
		break;
	case VK_END:
		m_vStart.x += (525 - m_vEnd.x);
		m_vEnd.x = 525;
		break;

	default:
		return;
	}

	SetEditBox();
	OnPaint();
}

BOOL CTextureViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_SHIFT )
		{
			m_bShiftState = true;
			return TRUE;
		}
		if( pMsg->wParam == VK_CONTROL )
		{
			m_bCtrlState = true;
			return TRUE;
		}
		if( pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP || pMsg->wParam == VK_RIGHT )
		{
			KeyDown(pMsg->wParam,0,0);
			m_KeyPress = true;
			return TRUE;
		}
		if( pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT || pMsg->wParam == VK_HOME || pMsg->wParam == VK_END )
		{
			KeyDown(pMsg->wParam,0,0);
			return TRUE;
		}
		if( pMsg->wParam == VK_RETURN )
		{	return TRUE;   }
	}
	else if( pMsg->message == WM_KEYUP )
	{
		if( pMsg->wParam == VK_SHIFT )
		{
			m_bShiftState = false;
			return TRUE;
		}
		if( pMsg->wParam == VK_CONTROL )
		{
			m_bCtrlState = false;
			return TRUE;
		}
		if( pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP || pMsg->wParam == VK_RIGHT )
		{
			m_KeyPress = false;
			return TRUE;
		}
		if( pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT || pMsg->wParam == VK_HOME || pMsg->wParam == VK_END )
		{	return TRUE;   }
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// 크기 맞추기 버튼 클릭
void CTextureViewDlg::OnBnClickedSetsize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( (m_vStart.x < m_vEnd.x) && (m_vStart.y > m_vEnd.y) ||
        (m_vStart.x > m_vEnd.x) && ( m_vStart.y < m_vEnd.y ) )
	{
		g_pkWndClass->SetWidth( m_height );
		g_pkWndClass->SetHeight( m_width );
	}
	else
	{
		if( g_pkWndClass != NULL )
        {
			g_pkWndClass->SetWidth( m_width );
			g_pkWndClass->SetHeight( m_height );
		}
		else
		{	MessageBox( _T("크기를 맞출 대상이 존재하지 않습니다\n\n먼저 대상을 지정하세요 ^ -')/"), _T("알림"), NULL);   }
	}
}

// 원래 크기로 버튼 클릭
void CTextureViewDlg::OnBnClickedOriginalSize()
{
	int sub = m_vEnd.x - m_vStart.x;
	if( sub < 0 ) sub = sub*(-1);
	m_width = sub;

	sub = m_vEnd.y - m_vStart.y;
	if( sub < 0 ) sub = sub*(-1);
	m_height = sub;

	SetEditBox();
	OnPaint();
}

void CTextureViewDlg::UpdataRect(int iState, int iTextureNum, CPoint Start, CPoint End)
{
	CRect gap(0,0,0,0);
	ClientToScreen(&gap);
	m_ctlTexture.ScreenToClient(&gap);

	switch(iState)
	{
	case 0:		//def
		m_iDefNum = iTextureNum;
		m_RectDef.left	= Start.x - gap.left;
		m_RectDef.top	= Start.y - gap.top;
		m_RectDef.right = End.x - gap.left;
		m_RectDef.bottom = End.y - gap.top;
		break;
	case 1:		//hov
		m_iHovNum = iTextureNum;
		m_RectHov.left	= Start.x - gap.left;
		m_RectHov.top	= Start.y - gap.top;
		m_RectHov.right = End.x - gap.left;
		m_RectHov.bottom = End.y - gap.top;
		break;
	case 2:		//sel
		m_iSelNum = iTextureNum;
		m_RectSel.left	= Start.x - gap.left;
		m_RectSel.top	= Start.y - gap.top;
		m_RectSel.right = End.x - gap.left;
		m_RectSel.bottom = End.y - gap.top;
		break;
	case 3:		//act
		m_iActNum = iTextureNum;
		m_RectAct.left	= Start.x - gap.left;
		m_RectAct.top	= Start.y - gap.top;
		m_RectAct.right = End.x - gap.left;
		m_RectAct.bottom = End.y - gap.top;
		break;
	case 4:		//loc
		m_iLocNum = iTextureNum;
		m_RectLoc.left	= Start.x - gap.left;
		m_RectLoc.top	= Start.y - gap.top;
		m_RectLoc.right = End.x - gap.left;
		m_RectLoc.bottom = End.y - gap.top;
		break;
	default:
		return;
	}
	CComboBox *pComboTexture = (CComboBox*)GetDlgItem(IDC_TEXTURENAME);
	pComboTexture->SetCurSel(m_iDefNum);

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_STATE);
	pCombo->SetCurSel(0);
	OnCbnSelchangeState();
}

void CTextureViewDlg::OnCbnSelchangeState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CComboBox *pComboTexture = (CComboBox*)GetDlgItem(IDC_TEXTURENAME);
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_STATE);
	int iState = pCombo->GetCurSel();
	int iTextureNum;

	switch( iState )
	{
	case 0:		//def
		iTextureNum = m_iDefNum;
		m_vStart.x = m_RectDef.left;	
		m_vStart.y = m_RectDef.top;	
		m_vEnd.x = m_RectDef.right;
		m_vEnd.y = m_RectDef.bottom;
		break;
	case 1:		//hov
		iTextureNum = m_iHovNum;
		m_vStart.x = m_RectHov.left;	
		m_vStart.y = m_RectHov.top;	
		m_vEnd.x = m_RectHov.right;
		m_vEnd.y = m_RectHov.bottom;
		break;
	case 2:		//sel
		iTextureNum = m_iSelNum;
		m_vStart.x = m_RectSel.left;	
		m_vStart.y = m_RectSel.top;	
		m_vEnd.x = m_RectSel.right;
		m_vEnd.y = m_RectSel.bottom;
		break;
	case 3:		//act
		iTextureNum = m_iActNum;
		m_vStart.x = m_RectAct.left;	
		m_vStart.y = m_RectAct.top;	
		m_vEnd.x = m_RectAct.right;
		m_vEnd.y = m_RectAct.bottom;
		break;
	case 4:		//loc
		iTextureNum = m_iLocNum;
		m_vStart.x = m_RectLoc.left;	
		m_vStart.y = m_RectLoc.top;	
		m_vEnd.x = m_RectLoc.right;
		m_vEnd.y = m_RectLoc.bottom;
		break;
	default:
		return;
	}

	int sub = m_vEnd.x - m_vStart.x;
	if( sub < 0 ) sub = sub*(-1);
	m_width = sub;

	sub = m_vEnd.y - m_vStart.y;
	if( sub < 0 ) sub = sub*(-1);
	m_height = sub;

	if( iTextureNum != -1 )
		pComboTexture->SetCurSel(iTextureNum);
	else
	{
		m_vStart.x = 0;	
		m_vStart.y = 0;	
		m_vEnd.x = 0;
		m_vEnd.y = 0;
	}

	SetEditBox();
	OnPaint();
}


//------------------------------------------------------------------------
//	NAME : OnBnClickedButton2()
//	DESC : '현재 프로젝트에서 제외' 버튼을 누르면 호출된다.
//	DATE : 20091021
//			유효하지 않은 인덱스 참조시 예외처리가 안되있어서 수정
//------------------------------------------------------------------------
void CTextureViewDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CComboBox *pComboTexture = (CComboBox*)GetDlgItem(IDC_TEXTURENAME);

	extern KWndManager* g_WndMgr;
	int index = pComboTexture->GetCurSel();

	if( index != -1 )
		g_WndMgr->DropTexture( index );	
	else
	{
		::MessageBoxA( this->m_hWnd, "유효하지 않은 텍스처 인덱스. 먼저 선택을 해주세요.", __FILE__, MB_OK );
		return;
	}

    if(m_mapTexture.find(index) != m_mapTexture.end()){
        if(m_mapTexture[index])
            m_mapTexture[index]->Release();
        m_mapTexture.erase(m_mapTexture.find(index) );
    }

	m_strArrayName.RemoveAt(index);

	pComboTexture->Clear();
	while( pComboTexture->GetCount() )
		pComboTexture->DeleteString( 0 );

	for(UINT i = 0; i < m_strArrayName.GetSize(); i++)
	{
		CString temp;
		temp.Format( _T("%d"),i);
		temp += _T("번 ") + m_strArrayName[i];
		pComboTexture->AddString( temp );
	}

}

void CTextureViewDlg::OnBnClickedChangecolor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CColorDialog dlg;
	if( dlg.DoModal() != IDOK )
		return;
	COLORREF color = dlg.GetColor();

	m_iRvalue1 = GetRValue(color);
	m_iGvalue1 = GetGValue(color);
	m_iBvalue1 = GetBValue(color);

	OnPaint();
}

void CTextureViewDlg::OnBnClickedChangecolor2()
{
	CColorDialog dlg;
	if( dlg.DoModal() != IDOK )
		return;
	COLORREF color = dlg.GetColor();

	m_iRvalue2 = GetRValue(color);
	m_iGvalue2 = GetGValue(color);
	m_iBvalue2 = GetBValue(color);

	OnPaint();
}

bool CTextureViewDlg::Cursor_InTextureRect()
{
	if( (m_nowPoint.x<13) || (m_nowPoint.x>1010) || (m_nowPoint.y<35) || (m_nowPoint.y>545) )
	{	return false;   }
	
	return true;
}

bool CTextureViewDlg::Cursor_OnRectMove()
{
	if( (m_vStart.x-1) <= m_nowPoint.x && m_nowPoint.x <= (m_vStart.x+9) && (m_vStart.y-1) <= m_nowPoint.y && m_nowPoint.y <= (m_vStart.y+9) )
	{	return true;   }

	return false;
}

bool CTextureViewDlg::Cursor_OnRectSize()
{
	if( m_nowPoint.x <= (m_vEnd.x+1) && (m_vEnd.x-9) <= m_nowPoint.x && m_nowPoint.y <= (m_vEnd.y+1) && (m_vEnd.y-9) <= m_nowPoint.y )
	{	return true;   }

	return false;
}

double CTextureViewDlg::Magnificator( int _length, long _point1, long _point2 )
{
	double dMagnification = _length;

	return ( dMagnification /= ( _point2 - _point1 ) ) * 100;
}

void CTextureViewDlg::GetMagnification()
{
	if( m_bRect == true )
	{	
		m_Magnification_X = Magnificator( m_width, m_vStart.x, m_vEnd.x );
		m_Magnification_Y = Magnificator( m_height, m_vStart.y, m_vEnd.y );
	}
	else
	{
		m_Magnification_X = 0.0;
		m_Magnification_Y = 0.0;
	}
}

void CTextureViewDlg::SetSelectedWindowTextrueRect( KD3DWndClass *pSelectedWnd )
{
    CPoint Start( pSelectedWnd->GetDefTexCoords().x, pSelectedWnd->GetDefTexCoords().y );
    CPoint End( pSelectedWnd->GetDefTexCoords().z, pSelectedWnd->GetDefTexCoords().w);

    UpdataRect(0, pSelectedWnd->GetDefTextureIDx(), Start, End);

    Start.SetPoint( pSelectedWnd->GetHovTexCoords().x, pSelectedWnd->GetHovTexCoords().y );
    End.SetPoint( pSelectedWnd->GetHovTexCoords().z, pSelectedWnd->GetHovTexCoords().w);
    UpdataRect(1, pSelectedWnd->GetHovTextureIDx(), Start, End);

    Start.SetPoint( pSelectedWnd->GetSelTexCoords().x, pSelectedWnd->GetSelTexCoords().y );
    End.SetPoint( pSelectedWnd->GetSelTexCoords().z, pSelectedWnd->GetSelTexCoords().w);
    UpdataRect(2, pSelectedWnd->GetSelTextureIDx(), Start, End);

    Start.SetPoint( pSelectedWnd->GetActTexCoords().x, pSelectedWnd->GetActTexCoords().y );
    End.SetPoint( pSelectedWnd->GetActTexCoords().z, pSelectedWnd->GetActTexCoords().w);
    UpdataRect(3, pSelectedWnd->GetActTextureIDx(), Start, End);

    Start.SetPoint( pSelectedWnd->GetLocTexCoords().x, pSelectedWnd->GetLocTexCoords().y );
    End.SetPoint( pSelectedWnd->GetLocTexCoords().z, pSelectedWnd->GetLocTexCoords().w);
    UpdataRect(4, pSelectedWnd->GetLocTextureIDx(), Start, End);
}


//	20091007 이동석, 획대 축소 기능을 위해 추가
//	상수를 넣은 코드를 다 제거해야 하는데...-_-
BOOL CTextureViewDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if( GetKeyState(VK_LCONTROL) & 0x8000 )
	{
		if( zDelta > 0 )
		{
			m_fMagnification	+=	1.0f;
		}
		else
		{
			m_fMagnification	-=	1.0f;
		}
		if( m_fMagnification >= 5.0f ) m_fMagnification = 4.0f;
		if( m_fMagnification <= 1.0f ) m_fMagnification = 1.0f;

		::SendMessage( this->m_hWnd, WM_PAINT, 0, 0 );	
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CTextureViewDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_iAdd_x	=	0;
	m_iAdd_y	=	0;
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CTextureViewDlg::OnCbnEditchangeComboFindText()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_ctrlComboFindString.ShowDropDown( true );

    m_ctrlComboFindString.GetWindowText( m_strFindString );

    for ( int i = m_ctrlComboFindString.GetCount() -1; i >= 0; i-- ) {
        m_ctrlComboFindString.DeleteString( i );
    }

    std::vector<std::string>::iterator vitTexFileName;
    for ( vitTexFileName = g_WndMgr->m_vecStrTexFileName.begin(); vitTexFileName != g_WndMgr->m_vecStrTexFileName.end(); vitTexFileName++ ) {
        if ( std::string::npos != vitTexFileName->find( m_strFindString ) ) {
            m_ctrlComboFindString.AddString( CA2T(vitTexFileName->c_str()) );
        }
    }

    m_ctrlComboFindString.ShowDropDown( true );
    SendMessage( WM_SETCURSOR, 0, 0 );
}

void CTextureViewDlg::OnCbnSelchangeCombo1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CComboBox *pComboTexture = (CComboBox*)GetDlgItem( IDC_TEXTURENAME );

    int nSelected = m_ctrlComboFindString.GetCurSel();
    CString strSelect;
    m_ctrlComboFindString.GetLBText( nSelected, strSelect );

    for ( int nIndex = 0; nIndex < m_strArrayName.GetSize(); nIndex++ ) {
        if ( strSelect == m_strArrayName[nIndex] ) {
            pComboTexture->SetCurSel( nIndex );
            break;
        }
    }
}
