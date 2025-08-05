#include "stdafx.h"
#include ".\KGCPetEvolutionWnd.h"

//

//


#include "MyD3D.h"
#include "KGCPetPreview.h"
//
#include "../GCRenderManager.h"

IMPLEMENT_CLASSNAME( KGCPetEvolutionWnd );
IMPLEMENT_WND_FACTORY( KGCPetEvolutionWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCPetEvolutionWnd, "gc_pet_evolution_wnd" );


#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM	0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM	0.45f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM 0.25f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM 0.35f

KGCPetEvolutionWnd::KGCPetEvolutionWnd(void)
{
    m_fZoom = DEFAULTZOOM;
    m_fAngleMove = 0.0f;

    m_pkPetModel = NULL;
    m_bInternal = false;

    m_iMotionCount = 0;
}

KGCPetEvolutionWnd::~KGCPetEvolutionWnd(void)
{
    if( m_pkPetModel != NULL )
    {
        m_pkPetModel->EndPet();
        SAFE_DELETE( m_pkPetModel );
    }
}

void KGCPetEvolutionWnd::OnCreate( void )
{
    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X        = static_cast<DWORD>(( VIEWPORT_OFFSET_X + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y        = static_cast<DWORD>(( VIEWPORT_OFFSET_Y + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width    = static_cast<DWORD>(VIEWPORT_WIDTH * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height   = static_cast<DWORD>(VIEWPORT_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ     = 0.0f;
    m_viewport.MaxZ     = 1.0f;
}

void KGCPetEvolutionWnd::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
    }
}

bool KGCPetEvolutionWnd::GetEvolutionPet( std::string sPetName )
{
    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.begin();
    if ( mit->second.m_strName == GCUTIL_STR::GCStrCharToWide( sPetName.c_str() ) )
    {
        int iPetID = g_kGlobalValue.GetPetIDFromItemID( mit->second.m_dwID );
        int iPetEvolutionLevel = mit->second.m_cPromotion;
        if ( g_kGlobalValue.GetPetTemplate( iPetID, 0 ).iMaxEvolutionLevel >= iPetEvolutionLevel + 1 && g_kGlobalValue.GetPetTemplate( iPetID, 0 ).iMaxEvolutionLevel > 0 )
            return true;
        else
            return false;
    }
    if ( mit == g_MyD3D->m_mapPetInfo.end() )
        return false;
}

void KGCPetEvolutionWnd::FrameMoveInEnabledState()
{
    if( g_pkInput->ISWheelUp() )
    {
        //printf("줌업\n");
        ChangeCharWndZoom(true);
    }
    else if( g_pkInput->ISWheelDown() )
    {
        //printf("줌다운\n");
        ChangeCharWndZoom(false);
    }

    if( g_pkInput->BtnDown() == TRUE && CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == true )
    {
        m_ptStartMouse = g_pkInput->GetMousePos();
        m_fStartAngle = m_fAngleMove;
        m_bInternal = true;
    }
    else if( g_pkInput->BtnDown() == TRUE && CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == false )
        m_bInternal = false;

    if( g_pkInput->BtnPressed() && m_bInternal == true )
        ProcessCharWndAngle();

    if( m_pkPetModel != NULL && m_pkPetModel->m_iPetID != -1 )
    {
        if( m_pkPetModel->cFrame >= (int)m_pkPetModel->GetPetMotionFrm( m_pkPetModel->usMotion )->GetNum_Frame() - 1 )
        {
            m_iMotionCount++;
            if( m_iMotionCount == 5 )
            {
                m_pkPetModel->SetPetMotion( EGCPETMOTION::SHOP_ACTION );
                m_iMotionCount = 0;
            }
            else
            {
                m_pkPetModel->SetPetMotion( EGCPETMOTION::SHOP );
            }
        }
    }
}

void KGCPetEvolutionWnd::PostChildDraw( void )
{
    if( g_pkUIScene->m_pkPetPreview->IsCreate() )
        return;

	g_pGCDeviceManager2->Flush();

    if( m_pkPetModel == NULL )
        return;

    if( !GetEvolutionPet( GCUTIL_STR::GCStrWideToChar( m_pkPetModel->m_strPetName ) ) )
        return;

    D3DXMATRIX  mat1, mat2, mat3, matWorld;
    D3DXMATRIX  mat4;
    D3DVIEWPORT9 orgViewport;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS );
        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
        g_pd3dDevice->SetFVF( SKINVERTEX::FVF );

        // 퍼스펙티브 설정
        ::D3DXMatrixPerspectiveLH( &mat3, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
        ::D3DXMatrixScaling( &mat1, 1.0f, 1.0f, 0.01f );
        ::D3DXMatrixMultiply( &mat2, &mat3, &mat1 );
        ::D3DXMatrixTranslation( &mat3, 0.0f, 0.172f, 0.5f );
        ::D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
        g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat1 );

        ::D3DXMatrixIdentity(&matWorld);
        ::D3DXMatrixRotationX( &mat3, 0.0f );
        ::D3DXMatrixRotationY( &mat2, m_fAngleMove );
        ::D3DXMatrixMultiply( &mat1, &mat3, &mat2 );
        ::D3DXMatrixScaling( &mat2, m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
        ::D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
        ::D3DXMatrixTranslation( &mat2, 0.0f, 0.081f - ( MAXZOOM - m_fZoom ) * 0.25f, 0.32f );
        ::D3DXMatrixMultiply( &matWorld, &mat3, &mat2 );
        g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX(1), &matWorld );
   
        if( m_pkPetModel != NULL && m_pkPetModel->m_iPetID != -1 )
        {
            ::D3DXMatrixIdentity(&mat1);
            ::D3DXMatrixIdentity(&mat2);
            ::D3DXMatrixIdentity(&mat3);
            ::D3DXMatrixIdentity(&matWorld);

            float fScale = 4.0f * m_pkPetModel->m_fScale;
            float fPosX = 0.0f, fPosY = -0.12f, fPosZ = 0.303f;

            fPosX += m_pkPetModel->GetPetMotionFrm(m_pkPetModel->usMotion)->GetFrameData(m_pkPetModel->cFrame).Plus_x * m_fZoom * fScale;
            fPosY += m_pkPetModel->GetPetMotionFrm(m_pkPetModel->usMotion)->GetFrameData(m_pkPetModel->cFrame).Pos_y * m_fZoom * fScale;

            fPosX = fPosX * cos( m_fAngleMove );

            ::D3DXMatrixTranslation( &mat3, m_vPetAxisCorrection.x, m_vPetAxisCorrection.y, m_vPetAxisCorrection.z );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat3 );						
            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat1 );			
            ::D3DXMatrixRotationY( &mat2, m_fAngleMove );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat2 );			
            ::D3DXMatrixTranslation( &mat3, fPosX, fPosY, fPosZ );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat3 );

            m_pkPetModel->m_pObject->SetWorldMat( &matWorld );
            m_pkPetModel->m_pObject->SetFrame( &m_pkPetModel->GetPetMotionFrm(  m_pkPetModel->usMotion  )->GetFrameData( m_pkPetModel->cFrame ) );
            m_pkPetModel->cFrame = ( m_pkPetModel->cFrame + 1 ) % m_pkPetModel->GetPetMotionFrm(  m_pkPetModel->usMotion  )->GetNum_Frame();
            g_RenderManager->Render( false, m_pkPetModel->m_pObject );

        }
    }
    g_pGCDeviceManager2->PopState();
    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCPetEvolutionWnd::ProcessCharWndAngle()
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCPetEvolutionWnd::InitPetWnd( int iPetID, int iPetEvolutionLevel )
{
    if( m_pkPetModel == NULL )
        m_pkPetModel = new KPet();

    m_pkPetModel->StartPet( -1, iPetID, PET_LEVEL_IN_SHOP, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, 0, true );
    m_pkPetModel->SetPetMotion( EGCPETMOTION::SHOP );
    m_pkPetModel->m_pObject->SetShellMatrixScale( 0.25f );
    m_vPetAxisCorrection = m_pkPetModel->GetPetAxisCorrection();

    m_iMotionCount = 0;
    m_fAngleMove = 0.0f;
    m_fZoom = DEFAULTZOOM;
}

void KGCPetEvolutionWnd::Release()
{
    if( m_pkPetModel != NULL )
    {
        m_pkPetModel->EndPet();
        SAFE_DELETE( m_pkPetModel );
    }
}

void KGCPetEvolutionWnd::ChangeCharWndZoom( bool _bZoomUp )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if( CheckPosInWindowBound(vMousePos) == true )
    {
        if( _bZoomUp )
            m_fZoom += DELTAZOOM;
        else
            m_fZoom -= DELTAZOOM;

        if( m_fZoom > MAXZOOM )
            m_fZoom = MAXZOOM;

        if( m_fZoom < MINZOOM )
            m_fZoom = MINZOOM;				
    }
}

void KGCPetEvolutionWnd::SetInvalidPet()
{
    if( m_pkPetModel != NULL )
        m_pkPetModel->EndPet();
}

int KGCPetEvolutionWnd::GetPetID()
{
    return m_pkPetModel->m_iPetID;
}
int KGCPetEvolutionWnd::GetPetEvolutionLevel()
{
    return m_pkPetModel->m_iEvolutionLevel;
}
