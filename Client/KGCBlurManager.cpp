#include "stdafx.h"
#include ".\kgcblurmanager.h"
#include "MyD3D.h"
#include "./GCDeviceLib/GCDynamicVBManager.h"

KGCBlurManager::KGCBlurManager(void)
{
    m_pBlurTargetSurface = NULL;
    m_pBlurTexture = NULL;

    m_pBlurAccumSurface = NULL;
    m_pBlurAccumTexture = NULL;
    m_pBlurDepthSurface = NULL;
}

KGCBlurManager::~KGCBlurManager(void)
{
}

void KGCBlurManager::InitDeviceObjects()
{
    m_enable = false;
    m_firstEnable = true;
    m_blurAlpha = 0.0f;
    m_maxAlpha = 0.99f;
    m_minAlpha = 0.0f;
}

void KGCBlurManager::RestoreDeviceObjects()
{
    m_bCapable = InitBlur();

    if( m_bCapable == false )
    {
        g_MyD3D->m_KGCOption.SetMotionBlur( false );
    }
}

void KGCBlurManager::InvalidateDeviceObjects(void)
{
    SAFE_RELEASE( m_pBlurTargetSurface);
    SAFE_RELEASE( m_pBlurTexture );

    SAFE_RELEASE( m_pBlurAccumSurface);
    SAFE_RELEASE( m_pBlurAccumTexture );

    SAFE_RELEASE( m_pBlurDepthSurface );
}

void KGCBlurManager::DeleteDeviceObjects(void)
{
    InvalidateDeviceObjects();
}

bool KGCBlurManager::InitBlur()
{
    int BLUR_SX = 512;
    int BLUR_SY = 512;

    if( g_MyD3D->m_d3dpp.BackBufferWidth > 1100 )
        BLUR_SX = BLUR_SY = 1024;

    HRESULT hr = g_pd3dDevice->CreateTexture(	BLUR_SX ,BLUR_SY,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_pBlurTexture, NULL );

    if( hr != D3D_OK )
        goto InitBlurErr;

    hr = m_pBlurTexture->GetSurfaceLevel(0,&m_pBlurTargetSurface);

    if( hr != D3D_OK )
        goto InitBlurErr;

    hr = g_pd3dDevice->CreateTexture(	BLUR_SX,BLUR_SY,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_pBlurAccumTexture, NULL);

    if( hr != D3D_OK )
        goto InitBlurErr;

    hr = m_pBlurAccumTexture->GetSurfaceLevel(0,&m_pBlurAccumSurface);

    if( hr != D3D_OK )
        goto InitBlurErr;

    hr = g_pd3dDevice->CreateDepthStencilSurface(	BLUR_SX,BLUR_SY,
        D3DFMT_D16,
        D3DMULTISAMPLE_NONE, 0, true,
        &m_pBlurDepthSurface, NULL);

    if( hr != D3D_OK )
        goto InitBlurErr;

    SetMinBlurAlpha( 0.0f );
    SetMaxBlurAlpha( 0.99f );

    m_aAccumVertex[0] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,0.0f-0.5f,0.0f),1.0f,			0xFFFFFFFF, 0,0);
    m_aAccumVertex[1] = BLURVERTEX(D3DXVECTOR3(BLUR_SX-0.5f,0.0f-0.5f,0.0f),1.0f,		0xFFFFFFFF, 1,0);
    m_aAccumVertex[2] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,BLUR_SY-0.5f,0.0f),1.0f,		0xFFFFFFFF, 0,1);
    m_aAccumVertex[3] = BLURVERTEX(D3DXVECTOR3(BLUR_SX-0.5f,BLUR_SY-0.5f,0.0f),1.0f,	0xFFFFFFFF, 1,1);

    m_aRenderVertex[0] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,0.0f-0.5f,0.0f), 1.0f,                                             0xFFFFFFFF, 0,0); 
    m_aRenderVertex[1] = BLURVERTEX(D3DXVECTOR3(g_MyD3D->m_d3dpp.BackBufferWidth -0.5f,0.0f-0.5f,0.0f),1.0f,                        0xFFFFFFFF, 1,0);
    m_aRenderVertex[2] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,g_MyD3D->m_d3dpp.BackBufferHeight-0.5f,0.0f),1.0f,                       0xFFFFFFFF, 0,1);
    m_aRenderVertex[3] = BLURVERTEX(D3DXVECTOR3(g_MyD3D->m_d3dpp.BackBufferWidth-0.5f,g_MyD3D->m_d3dpp.BackBufferHeight-0.5f,0.0f),1.0f, 0xFFFFFFFF, 1,1);

    return true;

InitBlurErr:
    return false;
}

void KGCBlurManager::BeginBlur(void)
{
    if( IsEnable() == false ) return;
    HRESULT hr;
    hr = g_pd3dDevice->GetRenderTarget( 0, &m_pBackTargetSurface);
    hr = g_pd3dDevice->GetDepthStencilSurface(&m_pBackDepthSurface);

    hr = g_pd3dDevice->SetRenderTarget( 0, m_pBlurTargetSurface);
    g_pd3dDevice->SetDepthStencilSurface(m_pBlurDepthSurface);

    g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L );
}

void KGCBlurManager::ApplyAccumBlur(void)
{
    if(m_firstEnable == true )
    {
        m_firstEnable = false;
        return;
    }

    // 여러가지 알파 혼합을 시도 해본다. 
    g_pGCDeviceManager2->PushState();    
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        
        g_pGCDeviceManager2->SetRenderState( D3DRS_COLORVERTEX ,TRUE);
        g_pGCDeviceManager2->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_COLOR1 );

        g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

        g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        g_pGCDeviceManager2->SetTextureStageState(0,  D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

        DWORD s_dwAlpha = D3DCOLOR_RGBA( 255, 255, 255, (DWORD)( (GetBlurAlpha()*255.0f) ) );
        m_aAccumVertex[0].color = s_dwAlpha;
        m_aAccumVertex[1].color = s_dwAlpha;
        m_aAccumVertex[2].color = s_dwAlpha;
        m_aAccumVertex[3].color = s_dwAlpha;

        //누적 블러 텍스쳐를 찍는다.
#if defined( USE_DYNAMIC_VERTEX_BUFFER )
        g_pd3dDevice->SetTexture(0,m_pBlurAccumTexture);
        g_pGCDeviceManager->GetDVBManager()->DrawPrimitive( KGCDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, &m_aAccumVertex );
#else
        g_pd3dDevice->SetTexture(0,m_pBlurAccumTexture);
        g_pd3dDevice->SetFVF( BLURVERTEX::FVF );
        g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_aAccumVertex,sizeof(BLURVERTEX));
#endif
    }
    g_pGCDeviceManager2->PopState();
}

void KGCBlurManager::UpdateAccumBlur(void)
{
    HRESULT hr;
    //누적 블러로 갱신.
    hr = g_pd3dDevice->SetRenderTarget( 0, m_pBlurAccumSurface );

    //현재의 블러 렌더 타겟 텍스쳐를 누적 블러 텍스쳐로 찍는다.
    g_pd3dDevice->SetTexture(0,m_pBlurTexture);
    g_pd3dDevice->SetFVF( BLURVERTEX::FVF );

	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);

		m_aAccumVertex[0].color = 0xFFFFFFFF;
		m_aAccumVertex[1].color = 0xFFFFFFFF;
		m_aAccumVertex[2].color = 0xFFFFFFFF;
		m_aAccumVertex[3].color = 0xFFFFFFFF;

#if defined( USE_DYNAMIC_VERTEX_BUFFER )
        g_pGCDeviceManager->GetDVBManager()->DrawPrimitive( KGCDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, &m_aAccumVertex );
#else
        g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_aAccumVertex,sizeof(BLURVERTEX));
#endif
    }
    g_pGCDeviceManager2->PopState();    
}

void KGCBlurManager::ApplyBlur(void)
{
    //렌더 타겟을 원래 데로.

    g_pd3dDevice->SetRenderTarget( 0, m_pBackTargetSurface );
    g_pd3dDevice->SetDepthStencilSurface(m_pBackDepthSurface);

    SAFE_RELEASE( m_pBackTargetSurface );
    SAFE_RELEASE( m_pBackDepthSurface );

    //최종 블러 텍스쳐를 화면에다가 찍는다.
    g_pd3dDevice->SetTexture(0,m_pBlurTexture);
    g_pd3dDevice->SetFVF( BLURVERTEX::FVF );

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);
#if defined( USE_DYNAMIC_VERTEX_BUFFER )
        g_pGCDeviceManager->GetDVBManager()->DrawPrimitive( KGCDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, &m_aRenderVertex );
#else
        g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_aRenderVertex,sizeof(BLURVERTEX));
#endif   
    }
    g_pGCDeviceManager2->PopState();    

    //테스트... 졸라 느리다. 이것들은 이용 못하겠다.
    //g_pd3dDevice->UpdateSurface(m_pBlurAccumSurface,NULL,m_pBlurTargetSurface,NULL);
    //g_pd3dDevice->GetRenderTargetData(m_pBlurTargetSurface,m_pBlurAccumSurface);
}

void KGCBlurManager::EndBlur(void)
{
    if( IsEnable() == false) return;

    //렌더링된 화면에다가 
    //전 프레임까지 누적된 블러 텍스쳐를 찍어 버린다. 
    ApplyAccumBlur();

    //결과(이번프레임+누적된 블러 텍스쳐)를 누적 블러 텍스쳐에다가 보관한다.
    UpdateAccumBlur();

    //화면에 복사한다.
    ApplyBlur();
}

void KGCBlurManager::Enable(bool flg)
{
    if( IsEnable() == false )
    {
        m_firstEnable= true; 
        m_blurAlpha = m_minAlpha;
    }

    m_enable = flg;
}

void KGCBlurManager::BlurEnd()
{
    m_firstEnable= true;
    m_blurAlpha = m_minAlpha;
    m_enable = false;
}

void KGCBlurManager::SetBlurAlpha(float alpha)
{
    if( alpha < GetMinBlurAlpha() ) alpha = GetMinBlurAlpha();
    if( alpha > GetMaxBlurAlpha() ) alpha = GetMaxBlurAlpha();

    m_blurAlpha = alpha;
}

float KGCBlurManager::GetBlurAlpha(void)
{
    return m_blurAlpha;
}

void KGCBlurManager::SetMaxBlurAlpha(float alpha)
{
    if(alpha < 0.0f) alpha = 0.0f;
    if(alpha > 0.99f) alpha = 0.99f;

    m_maxAlpha = alpha;
}

void KGCBlurManager::SetMinBlurAlpha(float alpha)
{
    if(alpha < 0.0f) alpha = 0.0f;
    if(alpha > 0.99f) alpha = 0.99f;

    m_minAlpha = alpha;
}

void KGCBlurManager::AddBlurAlpha(float a)
{
    if( g_MyD3D->m_KGCOption.IsMotionBlur() == false || m_bCapable == false )
        return;

    SetBlurAlpha( GetBlurAlpha() + a );

    if( GetBlurAlpha() > GetMinBlurAlpha() )
    {
        Enable(true);
    }
    else
    {
        Enable(false);
    }
}
