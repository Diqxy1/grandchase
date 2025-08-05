#include "StdAfx.h"
#include "KGCAfterImageRenderer.h"
#include "MyD3D.h"
#include "GCRenderManager.h"

#if !defined( USE_DYNAMIC_VERTEX_BUFFER )
GCDeviceVertexBuffer*	KGCAfterImageRenderer::m_pRenderVertex = NULL;
#endif

GCDeviceVertexBuffer*	KGCAfterImageRenderer::m_pTextureVertex = NULL;

KGCAfterImageRenderer::KGCAfterImageRenderer(void)
{
    ZeroMemory( m_pAfterImageTargetSurface, sizeof(m_pAfterImageTargetSurface) );
    ZeroMemory( m_pAfterImageTexture, sizeof(m_pAfterImageTexture) );
    ZeroMemory( m_pAfterImageDepthSurface, sizeof(m_pAfterImageDepthSurface) );
    m_bCapable = false;
    m_iMaxNotDrawFrame = 0;
    m_iRenderCnt = 0;
}

KGCAfterImageRenderer::~KGCAfterImageRenderer(void)
{
}

bool KGCAfterImageRenderer::Init()
{
    int AFTERIMAGE_SX = 512;
    int AFTERIMAGE_SY = 512;

    if( g_MyD3D->m_d3dpp.BackBufferWidth > 1100 )
        AFTERIMAGE_SX = AFTERIMAGE_SY = 1024;

    HRESULT hr;
    for( int i = 0 ; i < 2; i++)
    {
        hr = D3DXCreateTexture( g_pd3dDevice,AFTERIMAGE_SX,AFTERIMAGE_SY,1,D3DUSAGE_RENDERTARGET,
            D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT, &m_pAfterImageTexture[i] );

        if( hr != D3D_OK )
            goto InitAfterImageErr;

        hr = m_pAfterImageTexture[i]->GetSurfaceLevel(0,&m_pAfterImageTargetSurface[i]);

        if( hr != D3D_OK )
            goto InitAfterImageErr;

        hr = g_pd3dDevice->CreateDepthStencilSurface(	AFTERIMAGE_SX,AFTERIMAGE_SY,
            D3DFMT_D16,
            D3DMULTISAMPLE_NONE, 0, true,
            &m_pAfterImageDepthSurface[i], NULL);

        if( hr != D3D_OK )
            goto InitAfterImageErr;
    }

    BLURVERTEX* vTextureVertex = NULL;
    if( m_pTextureVertex == NULL ) {
        m_pTextureVertex = g_pGCDeviceManager2->CreateVertexBuffer( "AfterImgTexture_m_pVertex", 4*sizeof( BLURVERTEX ), D3DUSAGE_WRITEONLY, BLURVERTEX::FVF, D3DPOOL_DEFAULT, (void*)vTextureVertex );
    }

    vTextureVertex = ( BLURVERTEX* )m_pTextureVertex->Lock( (BYTE*)vTextureVertex );

    vTextureVertex[0] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,0.0f-0.5f,0.0f),1.0f,			D3DCOLOR_RGBA( 255, 255, 255, 240 ), 0,0);
    vTextureVertex[1] = BLURVERTEX(D3DXVECTOR3(AFTERIMAGE_SX-0.5f,0.0f-0.5f,0.0f),1.0f,		D3DCOLOR_RGBA( 255, 255, 255, 240 ), 1,0);
    vTextureVertex[2] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,AFTERIMAGE_SY-0.5f,0.0f),1.0f,		D3DCOLOR_RGBA( 255, 255, 255, 240 ), 0,1);
    vTextureVertex[3] = BLURVERTEX(D3DXVECTOR3(AFTERIMAGE_SX-0.5f,AFTERIMAGE_SY-0.5f,0.0f),1.0f,	D3DCOLOR_RGBA( 255, 255, 255, 240 ), 1,1);

    m_pTextureVertex->Unlock();

#if defined( USE_DYNAMIC_VERTEX_BUFFER )
    m_vRenderVertex[0] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,0.0f-0.5f,0.0f),1.0f, D3DCOLOR_RGBA( 220, 220, 220, 240 ), 0,0); 
    m_vRenderVertex[1] = BLURVERTEX(D3DXVECTOR3(g_MyD3D->m_d3dpp.BackBufferWidth -0.5f,0.0f-0.5f,0.0f),1.0f, D3DCOLOR_RGBA( 220, 220, 220, 240 ), 1, 0);
    m_vRenderVertex[2] = BLURVERTEX(D3DXVECTOR3(0.0f-0.5f,g_MyD3D->m_d3dpp.BackBufferHeight-0.5f,0.0f), 1.0f, D3DCOLOR_RGBA( 220, 220, 220, 240 ), 0, 1);
    m_vRenderVertex[3] = BLURVERTEX(D3DXVECTOR3(g_MyD3D->m_d3dpp.BackBufferWidth-0.5f,g_MyD3D->m_d3dpp.BackBufferHeight-0.5f,0.0f), 1.0f, D3DCOLOR_RGBA( 220, 220, 220, 240 ), 1,1);
#else    
    BLURVERTEX* vRenderVertex = NULL;
    if( m_pRenderVertex == NULL ) {
        m_pRenderVertex = g_pGCDeviceManager2->CreateVertexBuffer( "AfterImgRender_m_pVertex", 4*sizeof( BLURVERTEX ), D3DUSAGE_WRITEONLY, BLURVERTEX::FVF, D3DPOOL_DEFAULT, (void*)vRenderVertex );
    }

    vRenderVertex = ( BLURVERTEX* )m_pRenderVertex->Lock( (BYTE*)vRenderVertex );

    vRenderVertex[0] = BLURVERTEX(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,D3DCOLOR_RGBA( 220, 220, 220, 240 ), 0,0); 
    vRenderVertex[1] = BLURVERTEX(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,D3DCOLOR_RGBA( 220, 220, 220, 240 ), 1,0);
    vRenderVertex[2] = BLURVERTEX(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,D3DCOLOR_RGBA( 220, 220, 220, 240 ), 0,1);
    vRenderVertex[3] = BLURVERTEX(D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,D3DCOLOR_RGBA( 220, 220, 220, 240 ), 1,1);

    m_pRenderVertex->Unlock();
#endif        

    m_DrawFrame = 1;

    return true;

InitAfterImageErr:
    return false;
}


void KGCAfterImageRenderer::RestoreDeviceObjects()
{
    m_bCapable = Init();
}

void KGCAfterImageRenderer::InvalidateDeviceObjects( void )
{
    for( int i = 0; i < 2; i++ )
    {
        SAFE_RELEASE( m_pAfterImageTargetSurface[i]);
        SAFE_RELEASE( m_pAfterImageTexture[i] );
        SAFE_RELEASE( m_pAfterImageDepthSurface[i] );
    }
    
}

void KGCAfterImageRenderer::InitDeviceObjects()
{
    m_setObject.clear();   
}

void KGCAfterImageRenderer::Render()
{
    if( !m_bCapable || !m_DrawFrame /*|| g_MyD3D->m_KGCBlurMgr.IsEnable()*/ )
        return;    

#if defined( USE_DYNAMIC_VERTEX_BUFFER )
    //m_vRenderVertex[0].pos = D3DXVECTOR3(0.0f-0.5f,0.0f-0.5f,0.0f);
    //m_vRenderVertex[1].pos = D3DXVECTOR3(desc.Width -0.5f,0.0f-0.5f,0.0f);
    //m_vRenderVertex[2].pos = D3DXVECTOR3(0.0f-0.5f,desc.Height-0.5f,0.0f);
    //m_vRenderVertex[3].pos = D3DXVECTOR3(desc.Width-0.5f,desc.Height-0.5f,0.0f);
#else
    LPDIRECT3DSURFACE9  pBackTargetSurface;
    g_pd3dDevice->GetRenderTarget( 0, &pBackTargetSurface);
    D3DSURFACE_DESC desc;
    pBackTargetSurface->GetDesc( &desc );

    BLURVERTEX* vRenderVertex = NULL;
    if( m_pRenderVertex != NULL ) {
        vRenderVertex = ( BLURVERTEX* )m_pRenderVertex->Lock( (BYTE*)vRenderVertex );

        vRenderVertex[0].pos = D3DXVECTOR3(0.0f-0.5f,0.0f-0.5f,0.0f);
        vRenderVertex[1].pos = D3DXVECTOR3(desc.Width -0.5f,0.0f-0.5f,0.0f);
        vRenderVertex[2].pos = D3DXVECTOR3(0.0f-0.5f,desc.Height-0.5f,0.0f);
        vRenderVertex[3].pos = D3DXVECTOR3(desc.Width-0.5f,desc.Height-0.5f,0.0f);

        m_pRenderVertex->Unlock();
    }

    pBackTargetSurface->Release();
#endif

    //
    
    g_pGCDeviceManager2->PushState();    
    {
#if !defined( USE_DYNAMIC_VERTEX_BUFFER )
        if( m_pRenderVertex ) {
#endif
            g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE, FALSE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

            g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
            g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
            
#if defined( USE_DYNAMIC_VERTEX_BUFFER )
            g_pd3dDevice->SetTexture(0,m_pAfterImageTexture[0]);
            g_pGCDeviceManager->GetDVBManager()->DrawPrimitive( KGCDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, &m_vRenderVertex );
#else
            g_pd3dDevice->SetTexture(0,m_pAfterImageTexture[0]);
            g_pd3dDevice->SetFVF( BLURVERTEX::FVF );
            g_pd3dDevice->SetStreamSource( 0, m_pRenderVertex->GetBuffer(), 0, sizeof( BLURVERTEX ) );
            g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
        }
#endif
    }
    g_pGCDeviceManager2->PopState();


    
}

void KGCAfterImageRenderer::RegisterObject( GCObject* pObj, int iMaxNotDrawFrame )
{

    m_setObject.insert( pObj );
    m_DrawFrame = INT_MAX;
    m_iMaxNotDrawFrame = iMaxNotDrawFrame;
    m_iRenderCnt = 0;
}

void KGCAfterImageRenderer::UnRegisterObject( GCObject* pObj )
{
    bool bSetEmpty = m_setObject.empty();
    m_setObject.erase( pObj );

    if( m_setObject.empty() != bSetEmpty )
        m_DrawFrame = 100;
}

void KGCAfterImageRenderer::DeleteDeviceObjects( void )
{
    InvalidateDeviceObjects();
}

void KGCAfterImageRenderer::RenderAfterImage()
{
    //return;
    if( !m_bCapable || !m_DrawFrame )
        return;    

    if( m_iRenderCnt < m_iMaxNotDrawFrame )
    {
        ++m_iRenderCnt;
        return;
    }
    m_iRenderCnt = 0;

    g_pGCDeviceManager2->Flush();

    //프론트버퍼 백업용 포인터
    LPDIRECT3DSURFACE9  pBackDepthSurface;
    LPDIRECT3DSURFACE9  pBackTargetSurface;

    g_pd3dDevice->GetRenderTarget( 0, &pBackTargetSurface);
    g_pd3dDevice->GetDepthStencilSurface(&pBackDepthSurface);

    g_pd3dDevice->SetRenderTarget( 0, m_pAfterImageTargetSurface[1] );
    g_pd3dDevice->SetDepthStencilSurface(m_pAfterImageDepthSurface[1] );    

    g_pGCDeviceManager2->PushState();        
    {
        if( m_DrawFrame == 1 )
            g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0f, 0L );
        else
            g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0L );

        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DBLENDOP_ADD );
        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );    
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE, FALSE );        
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );        
      
        g_pd3dDevice->SetFVF( BLURVERTEX::FVF );
        g_pd3dDevice->SetTexture(0, m_pAfterImageTexture[0] );
        g_pd3dDevice->SetStreamSource( 0, m_pTextureVertex->GetBuffer(), 0, sizeof( BLURVERTEX ) );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
    }
    g_pGCDeviceManager2->PopState();    
    
    {
        int iTech = SiKGCRenderManager()->GetRenderTech();
        if( SiKGCRenderManager()->GetUsingVS() && iTech == 0 ) {
            iTech = 1;
        } else if( SiKGCRenderManager()->GetUsingVS() ==false && iTech == 0 ) {
            iTech = 1;
        }
        std::set<GCObject*>::iterator it;
        for( it = m_setObject.begin(); it != m_setObject.end(); ++it )
        {
            SiKGCRenderManager()->ForcedRenderVS( true, *it, iTech );
        }
    }
    
    g_pGCDeviceManager2->Flush();

    g_pd3dDevice->SetRenderTarget( 0, pBackTargetSurface);
    g_pd3dDevice->SetDepthStencilSurface(pBackDepthSurface);

    D3DXLoadSurfaceFromSurface( m_pAfterImageTargetSurface[0], NULL, NULL, m_pAfterImageTargetSurface[1], NULL,NULL, D3DX_FILTER_POINT, 0 );

    pBackDepthSurface->Release();
    pBackTargetSurface->Release();

    --m_DrawFrame;

}

bool KGCAfterImageRenderer::IsRegisterObject( GCObject* pObj )
{
    return (m_setObject.find( pObj ) != m_setObject.end());
}