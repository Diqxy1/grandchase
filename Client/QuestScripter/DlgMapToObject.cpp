// DlgMapToObject.cpp : 구현 파일입니다.
#include "stdafx.h"
#include ".\dlgmaptoobject.h"
#include "DlgDirectorySetting.h"
#include "xSkinButton.h"
#include "dlgTriggerContent.h"
#include "MonsterGenerater.h"
#include "../Kom/manager.h"

//#include "PlayerPosition.h"
//#include "ObjectGenerater.h"
//#include "PaletteStruct.h"
//#include "MapSystem/KQSLayerPart.h"
//#include "QuestScripter.h"
//#include "DlgMapToObject.h"
//#include "QuestScripterDlg.h"
//#include "DlgStageMonster_Popup.h"
//#include "DlgPlayerPosition_Popup.h"
//#include "DlgObjectGenerate_Popup.h"
//#include "DlgTrigger.h"
//#include "DlgTGMonsterGenerater.h"
//#include "DlgTGObjectGenerater.h"
//#include "DlgTGChildGenner.h"
//#include "DlgTGPlayerPosition.h"
//#include "DlgTGPosition.h"
//#include "MapSystem/QSFantasticMap.h"
//#include "MapSystem/KQSLayerManager.h"


LPDIRECT3DDEVICE9       g_pd3dDevice            = NULL;
IDirect3DStateBlock9*   g_pBlendStateBlock      = NULL;

CDlgMapToObject         *g_DlgMapToObject       = NULL;

#define SMOOTHRATE (1.0f)

IMPLEMENT_DYNAMIC(CDlgMapToObject, CDialog)
CDlgMapToObject::CDlgMapToObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMapToObject::IDD, pParent)
{
    m_pFont                     = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    m_pQuestScriptDlg           = NULL;
    m_pSwapChain1               = NULL;
    m_pSwapChain2               = NULL;
    m_bLoadScript               = false;
    g_DlgMapToObject            = this;

    bEndThread                  = false;
    IsShiftPushed               = false;
    m_fWidth                    = 1024.0f;
    m_fHeight                   = 768.0f;
    m_fEyeX                     = (m_fWidth/2.0f);
    m_fEyeY                     = (m_fHeight/2.0f);
    m_bLayerMode                = true;
    bControlPushed              = false;
    m_pDlgTGContent             = 0;
    m_bFoot                     = 0;
    m_fCameraZoom                     = 0;
    m_TGType                    = TG_TYPE_EACTION;
    m_enObjectType_EAction      = OBJECTTYPE_EACTION_PLAYER;
    m_enObjectType_Condition    = OBJECTTYPE_CONDITION_SELECT_MON;
    m_pObjectManager            = NULL;
    m_pFunctionManager          = NULL;
    InitializeCriticalSection( &cs );
}

CDlgMapToObject::~CDlgMapToObject()
{
    DeleteCriticalSection( &cs );
    FinalCleanup();

    SAFE_RELEASE( m_pSwapChain2 );
    SAFE_RELEASE( m_pSwapChain1 );
    SAFE_DELETE(m_pFont);
}

void CDlgMapToObject::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMapToObject, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_DESTROY()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_MESSAGE(WM_CLICK_TOOL_BUTTON, OnClickToolButton)
    ON_MESSAGE(WM_CLICK_TOOL_TYPE, OnChangeType)
    ON_MESSAGE(WM_MONSEL_FUNCTION, OnMonSelFunction)
    ON_MESSAGE(WM_AREASEL_FUNCTION, OnAreaSelFunction)    

    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CDlgMapToObject 메시지 처리기입니다.
HRESULT CDlgMapToObject::LoadMapToObject(CString str)
{
    try
    {
        EnterCriticalSection( &cs );
        m_bLoadScript = false;
        SiKQSFantasticMap()->UnLoad();
        SiKQSLayerManager()->UnLoad();
        
        std::string strFile = GCUTIL_STR::GCStrWideToChar( str.GetBuffer() );
        SiKQSLayerManager()->LoadScript( strFile );
        AfxGetMainWnd()->SendMessage(WM_LOAD_TOOL_ICON, 0, 0);
        m_bLoadScript = true;
        LeaveCriticalSection( &cs );
    }
    catch (...)
    {
        ShowWindow(SW_HIDE);
        MessageBox(L"맵을 표시하는데 문제가 있습니다.", L"알림", MB_OK);
    }

    return S_OK;
}

BOOL CDlgMapToObject::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    // Set the window's initial width
    CRect rtWindow;
    GetWindowRect(rtWindow);
    rtWindow.right = rtWindow.left + m_fWidth;
    rtWindow.bottom = rtWindow.top + m_fHeight;
    DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
    AdjustWindowRect( &rtWindow, dwStyle, true );
    MoveWindow(rtWindow, 1);

    CD3DApplication::m_hWnd = this->GetSafeHwnd();
    CD3DApplication::Create( m_hinstance );

    g_pGCDeviceManager = new GCDeviceManager(m_pd3dDevice);
    g_pd3dDevice = m_pd3dDevice;
    ////////////////////////////////////////////////////////////////////////
    SiCDlgDirectorySetting()->GetFromRegister();
    try
    {
        std::string strPath = g_Path;
        std::string strDataDir;
        strDataDir = GCUTIL_STR::GCStrWideToChar( SiCDlgDirectorySetting()->m_strMapSubFilePath.GetBuffer() );
        strDataDir += "\\";
        g_pGCDeviceManager->GetMassFileManager()->AddDirectory( strDataDir );

        strDataDir = strPath + "\\Data\\Shader\\";
        g_pGCDeviceManager->GetMassFileManager()->AddDirectory( strDataDir );

        strDataDir = strPath + "\\Data\\Effect\\";
        g_pGCDeviceManager->GetMassFileManager()->AddDirectory( strDataDir );

        strDataDir = strPath + "\\Data\\Script\\";
        g_pGCDeviceManager->GetMassFileManager()->AddDirectory( strDataDir );

        strDataDir = strPath + "\\Data\\Texture\\";
        g_pGCDeviceManager->GetMassFileManager()->AddDirectory( strDataDir );

    }
    catch (...)
    {
        MessageBox(L"MapToObject의 렌더링 초기화를 실패하였습니다. 아래의 폴더와 파일이 있는지 확인해 주세요. \
                   RenderState.fx, Effect.lua 파일이 있나 확인해주세요.", L"알림", MB_OK);
    }
    ////////////////////////////////////////////////////////////////////////

    if( m_strMapScript.GetLength() )
        LoadMapToObject(m_strMapScript);

    m_ToolPalette.Create(CToolPalette::IDD, this);

    m_PosFunctionManager.SetListener( AfxGetMainWnd() );

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMapToObject::CleanUp3D()
{
    SAFE_RELEASE(m_pD3D);
    SAFE_RELEASE(m_pd3dDevice);
}

void CDlgMapToObject::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgMapToObject::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
// Name: CAppForm::FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CDlgMapToObject::FinalCleanup()
{
    // TODO: Perform any final cleanup needed
    // Cleanup D3D font

    SiKQSFantasticMap()->UnLoad();
    SiKQSFantasticMap()->ReleaseInstance();
    SiKQSLayerManager()->ReleaseInstance();

    if( g_pGCDeviceManager )
        g_pGCDeviceManager->OnDestroyDevice();

    SAFE_DELETE(g_pGCDeviceManager);
    SAFE_RELEASE( g_pBlendStateBlock );
    Cleanup3DEnvironment();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CDlgMapToObject::InitDeviceObjects()
{
    //// Init the font
    if( m_pFont ) m_pFont->InitDeviceObjects( m_pd3dDevice );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CDlgMapToObject::DeleteDeviceObjects()
{
    //// TODO: Cleanup any objects created in InitDeviceObjects()
    if( m_pFont ) m_pFont->DeleteDeviceObjects();
    if( g_pGCDeviceManager )
        g_pGCDeviceManager->OnLostDevice();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CDlgMapToObject::RestoreDeviceObjects()
{
    //// TODO: setup render states
    g_pd3dDevice = m_pd3dDevice;

    // Set up the textures
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

    // Set miscellaneous render states

    m_pd3dDevice->BeginStateBlock();
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
    m_pd3dDevice->EndStateBlock( &g_pBlendStateBlock );

    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,        FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,       FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );
    m_pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
    m_pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );

    g_pBlendStateBlock->Apply();

    // Set the world matrix
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );
    m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );
    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( (m_fWidth/2.0f), (m_fHeight/2.0f), 0.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( (m_fWidth/2.0f), (m_fHeight/2.0f), 1.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // Set the projection matrix
    D3DXMATRIX matProj;
    D3DXMatrixOrthoLH( &matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    // Restore the font
    m_pFont->RestoreDeviceObjects();

    if( g_pGCDeviceManager )
        g_pGCDeviceManager->OnResetDevice();

    SAFE_RELEASE( m_pSwapChain1 );
    SAFE_RELEASE( m_pSwapChain2 );

    m_pd3dDevice->GetSwapChain( 0, &m_pSwapChain1 );
    m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp, &m_pSwapChain2 );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CAppForm::InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CDlgMapToObject::InvalidateDeviceObjects()
{
    // TODO: Cleanup any objects created in RestoreDeviceObjects()
    SAFE_RELEASE( m_pSwapChain2 );
    SAFE_RELEASE( m_pSwapChain1 );

    if( m_pFont ) m_pFont->InvalidateDeviceObjects();

    if( g_pGCDeviceManager )
        g_pGCDeviceManager->OnLostDevice();

    return S_OK;
}

HRESULT CDlgMapToObject::FrameMove(void)
{
    static DWORD dwLastFrame = timeGetTime();   
    const DWORD OneFrameTime = 1000 / 55;
    DWORD CurTime = timeGetTime();
    // 55프레임 맞추는 코드
    while( CurTime - dwLastFrame > OneFrameTime )
    {
        dwLastFrame += OneFrameTime;

        if( m_bLoadScript == false )    
            break;

        SiKQSLayerManager()->FrameMove( QS_ELAPSED_TIME );
        SiKQSFantasticMap()->FrameMove( QS_ELAPSED_TIME );

        if( this->IsWindowVisible() )
        {
            if( m_pObjectManager ) m_pObjectManager->FrameMoveMode();
            if( m_pFunctionManager ) m_pFunctionManager->FrameMoveMode();
        }
    }

    return S_OK;
}

HRESULT CDlgMapToObject::Render(void)
{
    if( !m_pd3dDevice ) return E_FAIL;
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
    m_pSwapChain1->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    m_pd3dDevice->SetRenderTarget( 0, pBackBuffer );

    // Set the world matrix
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );
    m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );
    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_fEyeX, m_fEyeY, 0.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_fEyeX, m_fEyeY, 1.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // Set the projection matrix
    D3DXMATRIX matProj;
    D3DXMatrixOrthoLH( &matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
        0x000000ff, 1.0f, 0L );

    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        if( m_bLoadScript == true )
        {
            if( this->IsWindowVisible() )
            {
                D3DXMATRIX matIdentity, mat1, mat2;
                float m_fZoom = 1.0f;
                float fx = m_fEyeX/400.0f;
                float fy = m_fEyeY/400.0f;
                D3DXMatrixIdentity( &matIdentity );
                D3DXMatrixScaling ( &mat1, m_fZoom, m_fZoom, 1.0f );
                D3DXMatrixTranslation( &mat2, -fx*m_fZoom, -fy*m_fZoom - 0.325f, 0.0f );
                D3DXMatrixMultiply( &matIdentity, &mat1, &mat2 );
                D3DXMatrixIdentity( &matIdentity );
                SiKQSLayerManager()->SetCameraMatrix(matIdentity);
                SiKQSFantasticMap()->SetCameraMatrix(matIdentity);
                FRect_Quest rt;
                rt.left = fx - (m_fWidth/2.0f);
                rt.right = fx + (m_fWidth/2.0f);
                rt.top = fy - (m_fHeight/2.0f);
                rt.bottom = fy + (m_fHeight/2.0f);
                SiKQSLayerManager()->SetCameraPosToQSLayer(m_fEyeX, m_fEyeY);
                SiKQSLayerManager()->SetCameraRectToQSLayer(rt.left, rt.right, rt.top, rt.bottom);
                
                SiKQSFantasticMap()->Render();
                SiKQSLayerManager()->AlphaRender();

                //************************************************************************
                // 오브젝트 관리 클래스들은 m_ToolPalette에 따라서 표시 그려지는 여부를 결정
                //************************************************************************
                if( m_ToolPalette.m_bMonster ) m_MonsterManager.DrawMode();
                if( m_ToolPalette.m_bPlayer ) m_PlayerManager.DrawMode();
                if( m_ToolPalette.m_bObject ) m_DecorObjectManager.DrawMode();
                if( m_ToolPalette.m_bDoor ) m_GateObjectManager.DrawMode();
                if( m_ToolPalette.m_bDamage ) m_DamageObjectManager.DrawMode();
                if( m_pFunctionManager ) m_pFunctionManager->DrawMode();

                m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
                m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_SRCALPHA );

                //************************************************************************
                // 팔레트에서 현재 선택되어 진 오브젝트를 그린다.
                //************************************************************************
                if( m_pObjectManager ) m_pObjectManager->DrawSelectedObject();

                if( m_bFoot ) RenderFH();

                m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

                //Render stats and help text  
                RenderText();
            }
        }

        // End the scene.
        m_pd3dDevice->EndScene();
    }
    //m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
    pBackBuffer->Release();

    return S_OK;
}

HRESULT CDlgMapToObject::RenderText()
{
    CString str;
    str.Format(L"%d", timeGetTime());
    D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    FLOAT fNextLine = 60.0f; 

    fNextLine += 60.0f;
    CString strEyes;
    strEyes.Format(L"EyeX : %f, EyeY : %f", m_fEyeX, m_fEyeY);
    m_pFont->DrawText( 2, fNextLine, fontColor, strEyes.GetBuffer() );

    return S_OK;
}

HRESULT CDlgMapToObject::OneTimeSceneInit()
{
    return S_OK;
}

void CDlgMapToObject::OnDestroy()
{
    __super::OnDestroy();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CDlgMapToObject::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

    if( GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000 )
        IsShiftPushed = true;
    else
        IsShiftPushed = false;

    if( GetAsyncKeyState(VK_CONTROL) & 0x8000 )
        bControlPushed = true;
    else
        bControlPushed = false;

    return __super::PreTranslateMessage(pMsg);
}

void CDlgMapToObject::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    // 마우스 버튼을 클릭했을 경우
    if( nFlags & MK_LBUTTON )
    {
        if( IsShiftPushed )     // 시프트 버튼을 눌르고 있나?
        {
            m_Point = point;
            m_StartPoint.x = (LONG)m_fEyeX;
            m_StartPoint.y = (LONG)m_fEyeY;
            return;
        }
        else
        {
            D3DVECTOR pos;
            Get3DWorldPos(&pos);
            if( m_pObjectManager ) m_pObjectManager->MouseLBDownMode(pos);
            if( m_pFunctionManager ) m_pFunctionManager->MouseLBDownMode(pos);
        }
    }

    __super::OnLButtonDown(nFlags, point);
}

void CDlgMapToObject::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    D3DVECTOR pos;
    Get3DWorldPos(&pos);
    m_ptDragItem.SetPoint((int)pos.x, (int)pos.y);

    if( m_pObjectManager ) m_pObjectManager->MouseMoveMode(pos);
    if( m_pFunctionManager ) m_pFunctionManager->MouseMoveMode(pos);


    if( nFlags & MK_LBUTTON )
    {
        // 공통
        if( IsShiftPushed )
        {
            float fDiffX = (float)(point.x - m_Point.x);
            float fDiffY = (float)(m_Point.y - point.y);

            m_fEyeX = m_StartPoint.x - fDiffX * SMOOTHRATE;
            m_fEyeY = m_StartPoint.y - fDiffY * SMOOTHRATE;

            float fCheckTermX = 0.0f;
            if( m_fWidth < (m_fWidth/2.0f) )
                fCheckTermX = m_fWidth * 0.5f;
            else
                fCheckTermX = (m_fWidth/2.0f);

            float fCheckTermY = 0.0f;
            if( m_fHeight < (m_fHeight/2.0f) )
                fCheckTermY = m_fHeight * 0.5f;
            else
                fCheckTermY = (m_fHeight/2.0f);

            const float fPixelWidth = SiKQSFantasticMap()->GetPixelWidth()*400.0f;
            const float fPixelHeight = SiKQSFantasticMap()->GetPixelHeight()*400.0f;

            if( m_fEyeX < fCheckTermX )
                m_fEyeX = fCheckTermX;
            else if( m_fEyeX + fCheckTermX > fPixelWidth )
                m_fEyeX = fPixelWidth - fCheckTermX;

            if( m_fEyeY < fCheckTermY )
                m_fEyeY = fCheckTermY;
            else if( m_fEyeY + fCheckTermY > fPixelHeight )
                m_fEyeY = fPixelHeight - fCheckTermY;

            D3DXMATRIX matView;
            D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_fEyeX, m_fEyeY, 0.0f );
            D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_fEyeX, m_fEyeY, 1.0f );
            D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
            g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
        }
    }

    __super::OnMouseMove(nFlags, point);
}

void CDlgMapToObject::Get3DWorldPos( D3DVECTOR* PosOut )
{
    CPoint point;
    GetCursorPos( &point );
    ScreenToClient( &point );

    D3DVECTOR v;
    D3DXMATRIX matProj;
    memset( &matProj, 0, sizeof(matProj) );
    D3DVIEWPORT9 vp;
    //g_pd3dDevice->GetViewport(&vp);
    RECT rect;
    GetWindowRect( &rect );
    vp.X = vp.Y = 0;
    vp.Width = (rect.right - rect.left);
    vp.Height = (rect.bottom - rect.top);
    vp.MinZ = 0.f;
    vp.MaxZ = 1.f;

    //HRESULT hr = g_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );
    D3DXMatrixOrthoLH( &matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f );

    v.x = ( ( ( ( ( point.x-vp.X ) * 2.0f / vp.Width ) - 1.0f ) ) - matProj._31 ) / matProj._11;
    v.y = ( ( - ( ( ( point.y-vp.Y ) * 2.0f / vp.Height ) - 1.0f) ) - matProj._32 ) / matProj._22;
    v.z = 0.0f;

    D3DXMATRIX m, matView;
    D3DVECTOR vPickRayDir, vPickRayOrig;
    //g_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_fEyeX, m_fEyeY, 0.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_fEyeX, m_fEyeY, 1.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );

    D3DXMatrixInverse( &m, NULL, &matView );
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

HRESULT CDlgMapToObject::Create( HINSTANCE hInstance )
{
    return S_OK;
}

BOOL CDlgMapToObject::Create(CWnd* pParentWnd, CString strScript)
{
    m_strMapScript = strScript;
    if( !CDialog::Create(IDD_DLGMAPTOOBJECT, pParentWnd) )
        return false;

    return true;
}

BOOL CDlgMapToObject::ShowWindow(int nCmdShow)
{
    bool bShow = CDialog::ShowWindow(nCmdShow);

    //m_bLoadScript = ( SW_SHOW == nCmdShow );
    if( m_ToolPalette.GetSafeHwnd() )
    {
        m_ToolPalette.ShowWindow(nCmdShow);
        switch( (int)m_TGType )
        {
        case TG_TYPE_EACTION:
            m_ToolPalette.SetType(m_TGType, (int)m_enObjectType_EAction);
            break;
        case TG_TYPE_CONDITION:
            m_ToolPalette.SetType(m_TGType, (int)m_enObjectType_Condition);
            break;
        }
        m_ToolPalette.OnCbnSelchangeComboType();
    }

    if( AfxGetMainWnd() )
        AfxGetMainWnd()->PostMessage(WM_FOCUS_WINDOW, 0, 0);

    if( m_bLoadScript )
        SetTimer( 0, 1, NULL );

    return bShow;
}

void CDlgMapToObject::SetMonsterList(std::vector<CBaseEventAction*> &vtMonList)
{
    m_vtMonsterGen.clear();
    for(int i=0;i<(int)vtMonList.size();++i)
        m_vtMonsterGen.push_back( (CMonsterGenerater*)vtMonList[i] );

    m_MonsterManager.SetMonsterList(&m_vtMonsterGen);
    m_MonsterManager.SetFont(m_pFont);
    m_MonselFunctionManager.SetMonsterList(&m_vtMonsterGen);
    m_MonselFunctionManager.SetListener(this);
}

void CDlgMapToObject::SetPlayerPosition(std::vector<CBaseEventAction*> &vtPlayerList)
{
    m_vtPlayerPosition.clear();
    for(int i=0;i<(int)vtPlayerList.size();++i)
        m_vtPlayerPosition.push_back( (CPlayerPosition*)vtPlayerList[i] );

    m_PlayerManager.SetPlayerList(&m_vtPlayerPosition);
    m_PlayerManager.SetFont(m_pFont);
}

void CDlgMapToObject::SetObjectList(std::vector<CBaseEventAction*> &vtObjectList)
{
    m_vtObjectGen.clear();
    for(int i=0;i<(int)vtObjectList.size();++i)
        m_vtObjectGen.push_back( (CObjectGenerater*)vtObjectList[i] );

    m_DecorObjectManager.SetDecorList(&m_vtObjectGen);
    m_DecorObjectManager.SetFont(m_pFont);
    m_AreaFunctionManager.SetListener(this);
}

void CDlgMapToObject::SetGateObjectList(std::vector<CBaseEventAction*> &vtGateObjectList)
{
    m_vtGateObjectGen.clear();
    for(int i=0;i<(int)vtGateObjectList.size();++i)
        m_vtGateObjectGen.push_back( (CGateObjectGenerater*)vtGateObjectList[i] );

    m_GateObjectManager.SetGateList(&m_vtGateObjectGen);
    m_GateObjectManager.SetFont(m_pFont);
}

void CDlgMapToObject::SetDamageObjectList(std::vector<CBaseEventAction*> &vtDamageObjectList)
{
    m_vtDamageObjectGen.clear();
    for(int i=0;i<(int)vtDamageObjectList.size();++i)
        m_vtDamageObjectGen.push_back( (CAddDamage*)vtDamageObjectList[i] );

    m_DamageObjectManager.SetDamageList(&m_vtDamageObjectGen);
    m_DamageObjectManager.SetFont(m_pFont);
}

void CDlgMapToObject::SetDlgTGContent(CdlgTriggerContent *pDlgTGContent)
{
    m_pDlgTGContent = pDlgTGContent;

    m_MonsterManager.SetDlgTGContent(pDlgTGContent);
    m_PlayerManager.SetDlgTGContent(pDlgTGContent);
    m_DecorObjectManager.SetDlgTGContent(pDlgTGContent);
    m_GateObjectManager.SetDlgTGContent(pDlgTGContent);
    m_DamageObjectManager.SetDlgTGContent(pDlgTGContent);
}

void CDlgMapToObject::SetQuestScripterDlg(CQuestScripterDlg *pQuestScriptDlg)
{
    m_pQuestScriptDlg = pQuestScriptDlg;
}

LRESULT CDlgMapToObject::OnClickToolButton(WPARAM wParam, LPARAM lParam)
{
    // 화면에 변경된 캐릭터로 위치 지정 할 수 있게 설정.

    CString strID;
    ((CxSkinButton*)lParam)->GetTooltipText(&strID);

    if( m_pObjectManager ) m_pObjectManager->SetObjectID(strID);

    return S_OK;
}

void CDlgMapToObject::SetTGType(TG_TYPE Type)
{
    m_TGType = Type;
    SetMapFunctionMode(m_TGType, m_enObjectType_EAction, m_enObjectType_Condition);
}

void CDlgMapToObject::SetObjectType_EAction(OBJECT_TYPE_EACTION enType)
{
    m_enObjectType_EAction = enType;

    m_ToolPalette.m_bPlayer       = false;
    m_ToolPalette.m_bObject       = false;
    m_ToolPalette.m_bMonster      = false;
    m_ToolPalette.m_bDoor         = false;
    switch(m_enObjectType_EAction)
    {
    case OBJECTTYPE_EACTION_PLAYER:
        m_ToolPalette.m_bPlayer       = true;
        SetWindowText(L"이벤트액션_모드 - 플레이어");
        break;
    case OBJECTTYPE_EACTION_MON:
        m_ToolPalette.m_bMonster      = true;
        SetWindowText(L"이벤트액션_모드 - 몬스터");
        break;
    case OBJECTTYPE_EACTION_OBJECT:
        m_ToolPalette.m_bObject       = true;
        SetWindowText(L"이벤트액션_모드 - 오브젝트");
        break;
    case OBJECTTYPE_EACTION_DOOR:
        m_ToolPalette.m_bDoor         = true;
        SetWindowText(L"이벤트액션_모드 - 게이트");
        break;
    case OBJECTTYPE_EACTION_DAMAGE:
        m_ToolPalette.m_bDamage       = true;
        SetWindowText(L"이벤트액션_모드 - 데미지");
        break;
    }

    SetMapFunctionMode(m_TGType, m_enObjectType_EAction, m_enObjectType_Condition);
}

void CDlgMapToObject::SetObjectType_Condition(OBJECT_TYPE_CONDITION enType)
{
    m_enObjectType_Condition = enType;

    m_ToolPalette.m_bPlayer       = false;
    m_ToolPalette.m_bObject       = false;
    m_ToolPalette.m_bMonster      = false;
    switch(m_enObjectType_Condition)
    {
        case OBJECTTYPE_CONDITION_SELECT_MON:
            m_ToolPalette.m_bMonster      = true;
            SetWindowText(L"컨디션_모드 - 몬스터선택");
            break;
        case OBJECTTYPE_CONDITION_SELECT_AREA:
            m_ToolPalette.m_bObject       = true;
            SetWindowText(L"컨디션_모드 - 영역설정");
            break;
        case OBJECTTYPE_CONDITION_SELECT_POS:
            SetWindowText(L"컨디션_모드 - 위치설정");
            break;
    }

    SetMapFunctionMode(m_TGType, m_enObjectType_EAction, m_enObjectType_Condition);
}

void CDlgMapToObject::RenderFH()
{
    SiKQSFantasticMap()->DebugRender();
}

VOID CDlgMapToObject::RenderScene()
{
    Render3DEnvironment();
}

void CDlgMapToObject::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    D3DVECTOR pos;
    Get3DWorldPos(&pos);
    if( m_pObjectManager ) m_pObjectManager->MouseLBUpMode(pos);
    if( m_pFunctionManager ) m_pFunctionManager->MouseLBUpMode(pos);

    __super::OnLButtonUp(nFlags, point);
}

void CDlgMapToObject::OnTimer(UINT nIDEvent)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    //if( m_bLoadScript ) 
    {
        EnterCriticalSection( &cs );
        RenderScene();
        LeaveCriticalSection( &cs );
    }
    CDialog::OnTimer(nIDEvent);
}

void CDlgMapToObject::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if( VK_TAB == nChar ) m_bFoot = !m_bFoot;
    __super::OnKeyUp(nChar, nRepCnt, nFlags);
}

LRESULT CDlgMapToObject::OnChangeType(WPARAM wParam, LPARAM lParam)
{
    SetTGType((TG_TYPE)wParam);
    switch( wParam )
    {
        case TG_TYPE_EACTION:
            SetObjectType_EAction((OBJECT_TYPE_EACTION)lParam);
            break;
        case TG_TYPE_CONDITION:
            SetObjectType_Condition((OBJECT_TYPE_CONDITION)lParam);
            break;
    }

    return S_OK;
}

LRESULT CDlgMapToObject::OnMonSelFunction(WPARAM wParam, LPARAM lParam)
{
    CMonsterGenerater *pMonster = (CMonsterGenerater*)wParam;
    if( m_pDlgTGContent && pMonster )
    {
        m_pDlgTGContent->m_dlgTGChildGenner.SetMonsterInfo(0, pMonster->m_iMonIndex, pMonster->m_iMonType);
        if( IDYES == MessageBox(L"몬스터가 선택 되었습니다. 이걸로 하시겠습니까?", L"알림", MB_YESNO) )
            ShowWindow(SW_HIDE);
    }
    return S_OK;
}

LRESULT CDlgMapToObject::OnAreaSelFunction(WPARAM wParam, LPARAM lParam)
{
    CRect *rtArea = (CRect*)wParam;
    if( m_pDlgTGContent && rtArea )
    {
        m_pDlgTGContent->m_dlgTGChildGenner.SetArea(*rtArea);
        if( IDYES == MessageBox(L"선택한 위치로 영역을 지정하겠습니까?", L"알림", MB_YESNO) )
            ShowWindow(SW_HIDE);
    }
    return S_OK;
}

BOOL CDlgMapToObject::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
    m_fWidth += ((zDelta/120.0f) * 1.33f) * 50.0f;
    m_fHeight += (zDelta/120.0f) * 50.0f;
    m_fCameraZoom += (zDelta/120.0f) * 50.0f;
    ResetWindowSize();
    return true;
}

void CDlgMapToObject::OnSize(UINT nType, int cx, int cy)
{
    RECT rect;
    GetWindowRect( &rect );
    rect.left += (m_fCameraZoom * 1.33f);
    rect.right += (m_fCameraZoom * 1.33f);
    rect.top += m_fCameraZoom;
    rect.bottom += m_fCameraZoom;
    m_fWidth = (rect.right - rect.left);
    m_fHeight = (rect.bottom - rect.top);
    ResetWindowSize();
}

void CDlgMapToObject::OnMove(int cx, int cy)
{
    RECT rect;
    GetWindowRect( &rect );
    rect.left += (m_fCameraZoom * 1.33f);
    rect.right += (m_fCameraZoom * 1.33f);
    rect.top += m_fCameraZoom;
    rect.bottom += m_fCameraZoom;
    m_fWidth = (rect.right - rect.left);
    m_fHeight = (rect.bottom - rect.top);
    ResetWindowSize();
}

void CDlgMapToObject::ResetWindowSize()
{
    float fCheckTermX = 0.0f;
    if( m_fWidth < (m_fWidth/2.0f) )
        fCheckTermX = m_fWidth * 0.5f;
    else
        fCheckTermX = (m_fWidth/2.0f);

    float fCheckTermY = 0.0f;
    if( m_fHeight < (m_fHeight/2.0f) )
        fCheckTermY = m_fHeight * 0.5f;
    else
        fCheckTermY = (m_fHeight/2.0f);

    const float fPixelWidth = SiKQSFantasticMap()->GetPixelWidth()*400.0f;
    const float fPixelHeight = SiKQSFantasticMap()->GetPixelHeight()*400.0f;

    if( m_fEyeX < fCheckTermX )
        m_fEyeX = fCheckTermX;
    else if( m_fEyeX + fCheckTermX > fPixelWidth )
        m_fEyeX = fPixelWidth - fCheckTermX;

    if( m_fEyeY < fCheckTermY )
        m_fEyeY = fCheckTermY;
    else if( m_fEyeY + fCheckTermY > fPixelHeight )
        m_fEyeY = fPixelHeight - fCheckTermY;

    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_fEyeX, m_fEyeY, 0.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( m_fEyeX, m_fEyeY, 1.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIX matProj;
    D3DXMatrixOrthoLH( &matProj, m_fWidth, m_fHeight, 0.0f, 3000.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    m_StartPoint.x = (LONG)m_fEyeX;
    m_StartPoint.y = (LONG)m_fEyeY;
}

void CDlgMapToObject::SetMapFunctionMode(TG_TYPE TGType, OBJECT_TYPE_EACTION enObjectType_EAction, OBJECT_TYPE_CONDITION enObjectType_Condition)
{
    m_pObjectManager        = NULL;
    m_pFunctionManager      = NULL;

    switch(TGType)
    {
    case TG_TYPE_EACTION:
        {
            switch(enObjectType_EAction)
            {
            case OBJECTTYPE_EACTION_PLAYER:
                m_pObjectManager = &m_PlayerManager;
                break;
            case OBJECTTYPE_EACTION_MON:
                m_pObjectManager = &m_MonsterManager;
                break;
            case OBJECTTYPE_EACTION_OBJECT:
                m_pObjectManager = &m_DecorObjectManager;
                break;
            case OBJECTTYPE_EACTION_DOOR:
                m_pObjectManager = &m_GateObjectManager;
                break;
            case OBJECTTYPE_EACTION_DAMAGE:
                m_pObjectManager = &m_DamageObjectManager;
                break;
            }
        }
        break;
    case TG_TYPE_CONDITION:
        {
            switch(enObjectType_Condition)
            {
            case OBJECTTYPE_CONDITION_SELECT_MON:
                m_pFunctionManager = &m_MonselFunctionManager;
                break;
            case OBJECTTYPE_CONDITION_SELECT_AREA:
                m_pFunctionManager = &m_AreaFunctionManager;
                break;
            case OBJECTTYPE_CONDITION_SELECT_POS:
                m_pFunctionManager = &m_PosFunctionManager;
                break;
            }
			break;
       }
    }
}
