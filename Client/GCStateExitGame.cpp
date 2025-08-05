#include "stdafx.h"
//
#include "MyD3D.h"
#include "GCStateExitGame.h"

#include "GCStateMachine.h"
//

#define DISPLAY_TIME 275 // 55 * 5

KGCStateExitGame* g_pStateExitGame = NULL;

KGCStateExitGame::KGCStateExitGame( void )
{
    g_pStateExitGame = this;
    m_pImageTextureL = NULL;
    m_pImageTextureR = NULL;
	m_pImageTextureB = NULL;
    m_eExitType      = KGCGlobal::EXIT_GAME_STATE_DISCONNECT;
    m_iFrameCount    = 0;
}

KGCStateExitGame::~KGCStateExitGame( void )
{
    
}

bool KGCStateExitGame::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    if( S_OK == InitDevice() )
        return true;
    else
        return false;
}

bool KGCStateExitGame::OnDestroyBeforeStateChange()
{
    return true;
}

bool KGCStateExitGame::FrameMove( float fElapsedTime )
{
    ++m_iFrameCount;

    if( m_iFrameCount > DISPLAY_TIME )
    {
#if defined( RETRY_LOGIN_ON_CLOSE_CONNECTION )
        g_kGlobalValue.m_bServerReconnection = true;
        m_iFrameCount = 0;
        g_MyD3D->m_pStateMachine->GoState( GS_SERVER, false );
        g_MyD3D->m_pStateMachine->JumpToSafeState( GS_SERVER );        
#else
        PostMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );
#endif
    }

    return true;
}

bool KGCStateExitGame::Render()
{
    if ( BeginScene() )
    {
        g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0L );

		D3DXVECTOR4 rtWholeUv = D3DXVECTOR4(0,0,512,512);			//1번 텍스쳐 전체를 붙여버리는 uv좌표계.	
		D3DXVECTOR4 rtPartUv1 = D3DXVECTOR4(0,0,512,256);					//2번 텍스쳐를 잘라 붙이는 uv좌표계.
		D3DXVECTOR4 rtPartUv2 = D3DXVECTOR4(0,256, 512, 512);		//3번 텍스쳐를 잘라 붙이는 uv좌표계.

		RECT rtBackGround1 = {0,0,512,512};			//배경 그림1의 좌표
		RECT rtBackGround2 = {512,0,1024,512};		//배경 그림2의 좌표
		RECT rtBackGround3 = {0,512,512,768};		//배경 그림3의 좌표
		RECT rtBackGround4 = {512,512,1024,768};		//배경 그림4의 좌표

		Render2D( m_pImageTextureL , rtBackGround1 , rtWholeUv );
		Render2D( m_pImageTextureR , rtBackGround2 ,rtWholeUv );
		Render2D( m_pImageTextureB , rtBackGround3 ,rtPartUv1 );
		Render2D( m_pImageTextureB , rtBackGround4 ,rtPartUv2 );

        //버전 렌더링
        WCHAR strVer[MAX_PATH];
        swprintf( strVer, MAX_PATH, L"%s (%d)", g_kGlobalValue.GetGameVersionW().c_str(), m_nSocketError );

        g_pGCDeviceManager2->PushState();
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

        g_pkFontMgrOutline->OutTextXY( 10, 10, strVer );

        g_pGCDeviceManager2->PopState();
        EndScene();
    }
    g_MyD3D->m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
    return S_OK;
}

bool KGCStateExitGame::OnInit()
{
    return true;
}

void KGCStateExitGame::OnDestroy()
{
    DeleteDevice();
}

void KGCStateExitGame::OnSetupFSM()
{
    ON_SETUP_FSM( FAIL_N_SHUTDOWN, FAIL_N_SHUTDOWN );
    m_eStateForFail = FAIL_N_SHUTDOWN;
}

HRESULT KGCStateExitGame::InitDevice( void )
{
    HRESULT hr = LoadTexture();
    return hr;
}

HRESULT KGCStateExitGame::LoadTexture( void )
{   
    std::wstring wstrTex1;
	std::wstring wstrTex2;
	std::wstring wstrTex3;

    switch( m_eExitType )
    {
    case KGCGlobal::EXIT_GAME_STATE_DISCONNECT:
    case KGCGlobal::EXIT_GAME_STATE_SHUTDOWN:
        wstrTex1 = L"Texture\\loading.kom\\Exit_1.dds";
		wstrTex2 = L"Texture\\loading.kom\\Exit_2.dds";
		wstrTex3 = L"Texture\\loading.kom\\Exit_3.dds";
        break;

    case KGCGlobal::EXIT_GAME_STATE_UPDATE:
        wstrTex1 = L"Texture\\loading.kom\\Update_1.dds";
        wstrTex2 = L"Texture\\loading.kom\\Update_2.dds";
		wstrTex3 = L"Texture\\loading.kom\\Exit_2.dds";
        break;
    }

    if(FAILED ( D3DXCreateTextureFromFileEx( g_MyD3D->m_pd3dDevice, wstrTex1.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, 
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, 
        NULL, NULL, &m_pImageTextureL ) ) )
        return E_FAIL;

    if(FAILED ( D3DXCreateTextureFromFileEx( g_MyD3D->m_pd3dDevice, wstrTex2.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, 
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, 
        NULL, NULL, &m_pImageTextureR ) ) )
        return E_FAIL;
	if(FAILED ( D3DXCreateTextureFromFileEx( g_MyD3D->m_pd3dDevice, wstrTex3.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, 
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, 
		NULL, NULL, &m_pImageTextureB ) ) )
		return E_FAIL;


    return S_OK;
}

void KGCStateExitGame::Render2D( LPDIRECT3DTEXTURE9 pTexture , RECT rtScreen ,D3DXVECTOR4 rtUv, float fWidthRatio /*= 1.0f*/, bool bRotate/*=false */ )
{
    float fRight = (float)rtScreen.left + (float)(rtScreen.right - rtScreen.left ) * fWidthRatio;

    if(!bRotate)
        rtUv += D3DXVECTOR4(0.5f,0.5f,0.5f,0.5f);
    else
    {
        rtUv += D3DXVECTOR4(0.5f,-0.5f,0.5f,-0.5f);
    }
    rtUv /= 512.0f;

    m_rect[0].m_vPos = D3DXVECTOR4((float)rtScreen.left ,(float)rtScreen.top , 0.8f , 1.0f);
    m_rect[0].m_diff = 0xffffffff;	
    m_rect[0].m_vTex = D3DXVECTOR2((float)rtUv.x,(float)rtUv.y);

    m_rect[1].m_vPos = D3DXVECTOR4(		fRight			,(float)rtScreen.top , 0.8f , 1.0f);
    m_rect[1].m_diff = 0xffffffff;
    if( !bRotate )
        m_rect[1].m_vTex = D3DXVECTOR2((float)rtUv.z,(float)rtUv.y);
    else
        m_rect[1].m_vTex = D3DXVECTOR2((float)rtUv.x,(float)rtUv.w);

    m_rect[2].m_vPos = D3DXVECTOR4((float)rtScreen.left ,(float)rtScreen.bottom , 0.8f , 1.0f);
    m_rect[2].m_diff = 0xffffffff;
    if( !bRotate )
        m_rect[2].m_vTex = D3DXVECTOR2((float)rtUv.x,(float)rtUv.w);
    else
        m_rect[2].m_vTex = D3DXVECTOR2((float)rtUv.z,(float)rtUv.y);


    m_rect[3].m_vPos = D3DXVECTOR4(		fRight			,(float)rtScreen.bottom , 0.8f , 1.0f);
    m_rect[3].m_diff = 0xffffffff;
    m_rect[3].m_vTex = D3DXVECTOR2((float)rtUv.z,(float)rtUv.w);

    if (g_pGCDeviceManager->GetWindowScaleX() > 1.0f || g_pGCDeviceManager->GetWindowScaleY() > 1.0f)
    {
        for (int i=0 ;i<4;i++)
        {
            m_rect[i].m_vPos.x = m_rect[i].m_vPos.x*g_pGCDeviceManager->GetWindowScaleX();
            m_rect[i].m_vPos.y = m_rect[i].m_vPos.y*g_pGCDeviceManager->GetWindowScaleY();
        }
    }


    g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,          FALSE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
    g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_MyD3D->m_pd3dDevice->SetTexture( 0, pTexture );	
    g_MyD3D->m_pd3dDevice->SetFVF( KD3DUIVertex::FVF );
    g_MyD3D->m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_rect, sizeof( KD3DUIVertex ) );
}

void KGCStateExitGame::DeleteDevice( void )
{
    SAFE_RELEASE( m_pImageTextureL );
    SAFE_RELEASE( m_pImageTextureR );
}

void KGCStateExitGame::SetExitType( KGCGlobal::GAME_EXIT_TYPE eExitType, int nSocketError )
{
    if( eExitType < KGCGlobal::EXIT_GAME_STATE_DISCONNECT || eExitType >= KGCGlobal::NUM_EXIT_GAME_STATE )
        m_eExitType = KGCGlobal::EXIT_GAME_STATE_DISCONNECT;
    else
        m_eExitType = eExitType;

    m_nSocketError = nSocketError;
}