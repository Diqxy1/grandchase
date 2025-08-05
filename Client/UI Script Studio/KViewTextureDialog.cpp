// KViewTextureDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI Script Studio.h"
#include "KViewTextureDialog.h"
#include "KWndManager.h"
#include ".\kviewtexturedialog.h"

extern KWndManager *g_WndMgr;
extern KD3DWndClass *g_pkSelectWnd;

// KViewTextureDialog 대화 상자입니다.

#define GRITH_WIDTH 10

IMPLEMENT_DYNAMIC(KViewTextureDialog, CDialog)
KViewTextureDialog::KViewTextureDialog(CWnd* pParent /*=NULL*/)
	: CDialog(KViewTextureDialog::IDD, pParent),
    m_strText( _T("★ 사용하고 있는 텍스쳐 ★") ),
    m_iMaxTexture( 0 )
{
	m_pD3D          = NULL;
    m_pd3dDevice    = NULL;
    m_pSprite       = NULL;

    m_iSelect       = -1;

	// 2011. 08. 19
	m_pOldBackBuffer = NULL;
	m_pSurface = NULL;
}

KViewTextureDialog::~KViewTextureDialog()
{
    Cleanup();
}

void KViewTextureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_TEXTURELIST, m_strText);
    DDX_Control(pDX, IDC_CURRENT_TEXTURE, m_ctlTexture);
}


BEGIN_MESSAGE_MAP(KViewTextureDialog, CDialog)
    ON_LBN_SELCHANGE(IDC_LIST_TEXTURE, OnSelectChange)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KViewTextureDialog::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// KViewTextureDialog 메시지 처리기입니다.

BOOL KViewTextureDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_pkUseTextureList = (CListBox*)GetDlgItem( IDC_LIST_TEXTURE );

    m_iCountUseTexture = 0;

    for( int i = 0; i < m_iMaxTexture; ++i )
    {
        if( m_abUseTexture[i] )
        {
            CString strNode = "";

            char texture_num[10];
            //sprintf(texture_num, "%d", i );
			sprintf_s(texture_num, sizeof(texture_num), "%d", i );

            strNode += texture_num;
            strNode += _T("번 : ");
            strNode += g_WndMgr->m_vecStrTexFileName[i].c_str();
            strNode += _T("\n");

            m_pkUseTextureList->InsertString( m_iCountUseTexture, strNode );

            m_aiUseTextureList[m_iCountUseTexture] = i;

            ++m_iCountUseTexture;
        }
    }

    m_pkUseTextureList->SetCurSel( 0 );

    m_ctlTexture.SetWindowPos( NULL, 0 - GRITH_WIDTH, 0 - GRITH_WIDTH,
                               512 + GRITH_WIDTH, 512 + GRITH_WIDTH,
                               SWP_NOMOVE|SWP_NOZORDER );
    InitD3D( m_ctlTexture.GetSafeHwnd() );
    D3DXCreateSprite( m_pd3dDevice, &m_pSprite );

    LoadTexture();

    Render();

    return TRUE;
}
void KViewTextureDialog::OnSelectChange()
{
    m_iSelect = m_pkUseTextureList->GetCurSel();

    m_vecRect.clear();
    AddRectInfo( g_pkSelectWnd );

    Render();
}

void KViewTextureDialog::AddRectInfo( KD3DWndClass* kWnd )
{
    RectInfo kRectInfo;
    if( kWnd->GetDefTextureIDx() > -1 )
    {
        kRectInfo.iTexture = kWnd->GetDefTextureIDx();
        kRectInfo.eState = EUISTATE_DEFAULT;
        kRectInfo.x1 = kWnd->GetDefTexCoords().x;
        kRectInfo.y1 = kWnd->GetDefTexCoords().y;
        kRectInfo.x2 = kWnd->GetDefTexCoords().z;
        kRectInfo.y2 = kWnd->GetDefTexCoords().w;
        m_vecRect.push_back( kRectInfo );
    }
    if( kWnd->GetHovTextureIDx() > -1 )
    {
        kRectInfo.iTexture = kWnd->GetHovTextureIDx();
        kRectInfo.eState = EUISTATE_HOVER;
        kRectInfo.x1 = kWnd->GetHovTexCoords().x;
        kRectInfo.y1 = kWnd->GetHovTexCoords().y;
        kRectInfo.x2 = kWnd->GetHovTexCoords().z;
        kRectInfo.y2 = kWnd->GetHovTexCoords().w;
        m_vecRect.push_back( kRectInfo );
    }
    if( kWnd->GetSelTextureIDx() > -1 )
    {
        kRectInfo.iTexture = kWnd->GetSelTextureIDx();
        kRectInfo.eState = EUISTATE_SELECT;
        kRectInfo.x1 = kWnd->GetSelTexCoords().x;
        kRectInfo.y1 = kWnd->GetSelTexCoords().y;
        kRectInfo.x2 = kWnd->GetSelTexCoords().z;
        kRectInfo.y2 = kWnd->GetSelTexCoords().w;
        m_vecRect.push_back( kRectInfo );
    }
    if( kWnd->GetActTextureIDx() > -1 )
    {
        kRectInfo.iTexture = kWnd->GetActTextureIDx();
        kRectInfo.eState = EUISTATE_ACTIVE;
        kRectInfo.x1 = kWnd->GetActTexCoords().x;
        kRectInfo.y1 = kWnd->GetActTexCoords().y;
        kRectInfo.x2 = kWnd->GetActTexCoords().z;
        kRectInfo.y2 = kWnd->GetActTexCoords().w;
        m_vecRect.push_back( kRectInfo );
    }
    if( kWnd->GetLocTextureIDx() > -1 )
    {
        kRectInfo.iTexture = kWnd->GetLocTextureIDx();
        kRectInfo.eState = EUISTATE_LOCK;
        kRectInfo.x1 = kWnd->GetLocTexCoords().x;
        kRectInfo.y1 = kWnd->GetLocTexCoords().y;
        kRectInfo.x2 = kWnd->GetLocTexCoords().z;
        kRectInfo.y2 = kWnd->GetLocTexCoords().w;
        m_vecRect.push_back( kRectInfo );
    }

    for( int i = 0; i < (int)kWnd->GetChildNum(); ++i )
        AddRectInfo( kWnd->GetChild(i) );
}

void KViewTextureDialog::Render()
{
    if( NULL == m_pd3dDevice )
        return;

    D3DRECT Rect1;
    Rect1.x1 = 0;
    Rect1.y1 = 0;
    Rect1.x2 = 512 + GRITH_WIDTH * 2;
    Rect1.y2 = 512 + GRITH_WIDTH * 2;

    m_pd3dDevice->Clear( 1, &Rect1, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 102, 0 ), 1.0f, 0 );

	// Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        RenderWindow(m_iSelect);
		RenderLine(m_iSelect);
        m_pd3dDevice->EndScene();
    }
    // Present the backbuffer contents to the display
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	// Test
    //DrawRectangle( 100, 100, 200, 200 );

/* 2011. 08. 23  // RenderLine(m_iSelect); 사용으로 아래를 대신함
    // 현재 사용중인 텍스쳐 위치 표시
    for( int i = 0; i < (int)m_vecRect.size(); ++i )
    {
        //m_vecRect[i].eState;

        if( m_vecRect[i].iTexture == m_aiUseTextureList[ m_iSelect ] )
            DrawRectangle( m_vecRect[i].x1, m_vecRect[i].y1, m_vecRect[i].x2, m_vecRect[i].y2 );
    }
*/
}

void KViewTextureDialog::RenderWindow(int nIndex)
{
    if( nIndex == -1 )
        return;

    if( m_mapTexture.empty() )
        return;

    D3DXMATRIX mat;
    D3DXMatrixIdentity( &mat );
    D3DXMatrixScaling( &mat, 1.0f, 1.0f, 1.0f );

    CRect rect;
    rect.SetRect( 0, 0, 512, 512 );
    ClientToScreen( &rect );
    m_ctlTexture.ScreenToClient( &rect );

    rect.SetRect( 0, 0, 512, 512 );
    m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );

    m_pSprite->SetTransform( &mat );

    int iCheck = -1;
    if(m_mapTexture.find(nIndex) != m_mapTexture.end() && m_mapTexture[ nIndex ] != NULL){
        iCheck = m_pSprite->Draw( m_mapTexture[ nIndex ], &rect, NULL, NULL, 0xFF222222 );	// 2011. 08. 19 
    }

// 2011. 08. 19 
		
	CRect rt;
	D3DXVECTOR3 pos(0, 0, 0);

	for( int i = 0; i < (int)m_vecRect.size(); ++i )
	{
		if( m_vecRect[i].iTexture == m_aiUseTextureList[ nIndex ] ){
			rt.SetRect( m_vecRect[i].x1, m_vecRect[i].y1, m_vecRect[i].x2, m_vecRect[i].y2 );
	
			pos.x = rt.left;
			pos.y = rt.top;

			m_pSprite->Draw( m_mapTexture[ nIndex ], &rt, NULL, &pos, 0xFFFFFFFF );
		}
	}
//
    m_pSprite->End();
}

HRESULT KViewTextureDialog::InitD3D( HWND hWnd )
{
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here
    return S_OK;
}

VOID KViewTextureDialog::Cleanup()
{
    for(std::map<int,LPDIRECT3DTEXTURE9>::iterator mIt = m_mapTexture.begin(); mIt != m_mapTexture.end();mIt++)
    {
        if( mIt->second != NULL )
            mIt->second->Release();		
    }
    if( m_pd3dDevice != NULL) 
        m_pd3dDevice->Release();

    if( m_pD3D != NULL)
        m_pD3D->Release();

    if( m_pSprite != NULL )
        m_pSprite->Release();
}

void KViewTextureDialog::LoadTexture()
{
    for( UINT i = 0; i < m_iCountUseTexture; ++i )
    {
        LPDIRECT3DTEXTURE9 pTexture;

        int iTexture = m_aiUseTextureList[i];
        std::string texName = g_WndMgr->m_vecStrTexFileName[iTexture];

        GCMassFile::MASSFILE_MEMBERFILEINFO* Info = g_WndMgr->GetMassFileManager()->LoadDataFile( texName );

        if( Info == NULL )
        {
            pTexture = NULL;
            continue ;
        }

        D3DXIMAGE_INFO TexInfo;

        D3DFORMAT dwFormat = D3DFMT_UNKNOWN;

        if( FAILED( D3DXCreateTextureFromFileInMemoryEx( m_pd3dDevice, (void*)Info->pRealData, Info->ulSize,
            D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, dwFormat/*3DFMT_A8R8G8B8*/, 
            D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, 
            &TexInfo,
            NULL, &pTexture ) ))
        {
            pTexture = NULL;
            continue ;
        }

        m_mapTexture.insert(std::pair<int,LPDIRECT3DTEXTURE9>(i,pTexture));
    }
}

void KViewTextureDialog::DrawRectangle( long x1, long y1, long x2, long y2 )
{
    CRect gap( 0, 0, 0, 0 );
    ClientToScreen( &gap );
    m_ctlTexture.ScreenToClient( &gap );

    CRect rect;
    rect.SetRect( x1, y1, x2, y2 );

    rect.left -= gap.left;
    rect.bottom -= gap.bottom;
    rect.right -= gap.right;
    rect.top -= gap.top;

    CDC* pdc = GetDC();
    CBrush NullBrush;
    NullBrush.CreateStockObject( NULL_BRUSH );

    CPen pen( PS_SOLID, 1, RGB( 255, 0, 50 ) );

    CPen *poldpen = pdc->SelectObject( &pen );
    CBrush *pOldBr = pdc->SelectObject( &NullBrush );

    pdc->Rectangle( rect );
    pdc->SelectObject( pOldBr );
    pdc->SelectObject( poldpen );

    NullBrush.DeleteObject();
    pen.DeleteObject();
    ReleaseDC( pdc );
}

//------------------------------------------------------------------------
//
// 2011. 08. 19 (이재준)
// 텍스쳐 내보내기 기능 추가 (export)
// 텍스쳐 마스크 처리(미사용부분) 후 저장 및 원본 텍스쳐 파일 뽑아주기
//
//------------------------------------------------------------------------
void KViewTextureDialog::OnBnClickedButtonExport()
{
	// 1. 저장할 폴더의 경로를 받아오기
	if( !GetSaveFolerPath() )	return;

	// 2. 렌더링 & 백버퍼 내용 폴더에 저장하기
	ExportMaskedTexture();

	// 3. 원본파일도 복사해서 폴더에 넣기
	CopyOrginalFile();

	// 4. 종료 메시지
	AfxMessageBox(_T("텍스쳐 내보내기 완료!!"));
}

//------------------------------------------------------------------------
// 저장할 폴더 경로 얻어오기
//------------------------------------------------------------------------
bool KViewTextureDialog::GetSaveFolerPath()
{
    bool retVal = false;

    // The BROWSEINFO struct tells the shell 
    // how it should display the dialog.
    BROWSEINFO bi;
    memset(&bi, 0, sizeof(bi));

    bi.ulFlags   = BIF_USENEWUI;
    bi.hwndOwner = NULL;
    bi.lpszTitle = _T("폴더 탐색기");

    // must call this if using BIF_USENEWUI
    ::OleInitialize(NULL);

    // Show the dialog and get the itemIDList for the 
    // selected folder.
    LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

    if(pIDL != NULL)
    {
        // Create a buffer to store the path, then 
        // get the path.
        char buffer[_MAX_PATH] = {'\0'};
        if(::SHGetPathFromIDList(pIDL, (LPSTR)buffer) != 0)
        {
            // Set the string value.
            m_strFolderPath = buffer;
            retVal = true;
        }

        // free the item id list
        CoTaskMemFree(pIDL);
    }

    if(retVal)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------
// 마스킹한 텍스쳐 저장
//------------------------------------------------------------------------
void KViewTextureDialog::ExportMaskedTexture()
{
	if( NULL == m_pd3dDevice )	// NULL일리가 없겠지만 다시 확인
		return;

    // Rect 정보 받아오자. (텍스쳐 목록을 클릭하지 않을시 RECT가 표시되지 않는 문제 해결) // 2011. 09. 15
    AddRectInfo( g_pkSelectWnd );

	// 기존 렌더링타겟 보존
	m_pd3dDevice->GetRenderTarget(0, &m_pOldBackBuffer);

	// 기존표면 정보에서 좀 빼오자
	D3DSURFACE_DESC desc;
	m_pOldBackBuffer->GetDesc(&desc);

	// 랜더타겟 생성
	m_pd3dDevice->CreateRenderTarget(512, 512, D3DFMT_A8R8G8B8, 
		desc.MultiSampleType, desc.MultiSampleQuality, false, &m_pSurface, NULL);

	// 렌더링타겟 변경
	m_pd3dDevice->SetRenderTarget(0, m_pSurface);

	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		// 현재 사용하는 텍스쳐 모두
		for( int i=0; i < m_iCountUseTexture; ++i ){
			m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 128, 128, 128 ), 1.0f, 0 );	// 바탕은 회색
			RenderWindow(i);
			RenderLine(i);
			SaveSurfaceToFile(i);
		}		
	}
	m_pd3dDevice->EndScene();

	// 기존타겟 복구
	m_pd3dDevice->SetRenderTarget(0, m_pOldBackBuffer);

	// 랜더타겟 해제
	m_pSurface->Release();
}

//------------------------------------------------------------------------
// 빨간 박스 그리기 line 4번
//------------------------------------------------------------------------
void KViewTextureDialog::RenderLine( int nIndex )
{
	// 사각형을 만들 정점 인덱스
	LineVertex box[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xffff0000 }, 
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xffff0000 },	
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xffff0000 }, 
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xffff0000 },	
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xffff0000 }, 
	};
	
	// 정점 포멧
	m_pd3dDevice->SetFVF(D3DFVF_LINEVERTEX);

	// 빨간 사각형 그리기
	// 선 4개를 그려서 사각형을 완성
	for( int i = 0; i < (int)m_vecRect.size(); ++i )
	{
		if( m_vecRect[i].iTexture == m_aiUseTextureList[ nIndex ] ){
			box[0].x = m_vecRect[i].x1;
			box[0].y = m_vecRect[i].y1;
			box[1].x = m_vecRect[i].x2;
			box[1].y = m_vecRect[i].y1;
			box[2].x = m_vecRect[i].x2;
			box[2].y = m_vecRect[i].y2;
			box[3].x = m_vecRect[i].x1;
			box[3].y = m_vecRect[i].y2;
			box[4] = box[0];			// 4 == 0
			
			m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &box, sizeof(LineVertex));
		}
	}	
}

//------------------------------------------------------------------------
// 백버퍼를 dds파일로 저장하기
//------------------------------------------------------------------------
void KViewTextureDialog::SaveSurfaceToFile( int nIndex )
{
	// 폴더경로
	CString strFilePath;
	
	// 텍스쳐 번호
	int nTexIndex = m_aiUseTextureList[nIndex];

	// 최종 파일경로
	strFilePath.Format("%s\\__%s", m_strFolderPath, g_WndMgr->m_vecStrTexFileName[nTexIndex].c_str() );

	//strFilePath.Replace(_T(".dds"), _T(".bmp")); // 확장자 바꾸고 싶으면, 아래 포멧이랑 같이 변환

	// 파일로 저장
	if(FAILED( D3DXSaveSurfaceToFile( strFilePath, D3DXIFF_DDS, m_pSurface, NULL, NULL) )){
		AfxMessageBox(_T("표면정보 파일로 저장하기 실패"));
		return;
	}
}

//------------------------------------------------------------------------
// 원본 파일 Copy 만들기
//------------------------------------------------------------------------
void KViewTextureDialog::CopyOrginalFile()
{
	FILE *fSour, *fDest;
	UINT readSize = 0;
	UINT nBufferSize = 1024; // define buffer size 1024
	char buffer[1024];

	std::string strSourPath;
	CString strDestPath;

	// 파일 메니져
	GCMassFileManager* pFileManager = g_WndMgr->GetMassFileManager();

	// 
	for( int i=0; i < m_iCountUseTexture; ++i ){
		// 텍스쳐 번호
		int nTexIndex = m_aiUseTextureList[i];

		// 원본파일 경로 얻어오기
		strSourPath = pFileManager->GetResFullPath( g_WndMgr->m_vecStrTexFileName[nTexIndex] );

		// 복사본 파일경로
		CString strFilePath;

		// 저장할 경로
		strDestPath.Format("%s\\%s", m_strFolderPath, g_WndMgr->m_vecStrTexFileName[nTexIndex].c_str() );

		// 파일 열기
		fSour = fopen(strSourPath.c_str(), "rb");
		fDest = fopen(strDestPath.GetString(), "wb");

		if(fSour == NULL){
			AfxMessageBox(_T("원본파일 열기 실패"));
			return;
		}
		if(fDest == NULL){
			AfxMessageBox(_T("복사본파일 생성 실패"));
			fclose(fSour);
			return;
		}

		// 안해도 되지만..
		fseek(fSour, 0, SEEK_SET);

		do{
			readSize = fread(buffer, sizeof(char), nBufferSize, fSour);
			fwrite(buffer, sizeof(char), readSize, fDest);
		}while(readSize == nBufferSize);

		// 끝
		fclose(fSour);
		fclose(fDest);
	}//
}


