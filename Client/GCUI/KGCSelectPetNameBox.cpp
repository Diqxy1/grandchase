#include "stdafx.h"
#include ".\KGCSelectPetNameBox.h"

//

//


#include "MyD3D.h"
#include "KGCRadioButtonBox.h"
#include "KGCPetEvolutionBox.h" // enum 값 쓰기 위해서.. EGCPETEVOLUTIONTYPE
//

#include "../GCRenderManager.h"

IMPLEMENT_CLASSNAME( KGCSelectPetNameBox );
IMPLEMENT_WND_FACTORY( KGCSelectPetNameBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSelectPetNameBox, "gc_select_pet_name_box" );


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
#define MINZOOM 0.20f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM 0.20f

KGCSelectPetNameBox::KGCSelectPetNameBox(void)
{
    m_fZoom = DEFAULTZOOM;
    m_fAngleMove = 0.0f;

    m_pkPetModel = NULL;
    m_bInternal = false;

    m_iMotionCount = 0;

    m_pkTextEvolution       = NULL;
    m_pkTextDevolution      = NULL;
    m_pkTextHatch           = NULL;
    m_pkEvolutionButton     = NULL;
    m_pkDevolutionButton    = NULL;
    m_pkHatchButton         = NULL;
    m_pkRadioButtonBox      = NULL;
    m_pkExit                = NULL;
    m_pkSelectPetNameDesc   = NULL;

    LINK_CONTROL( "text_evolution",             m_pkTextEvolution );
    LINK_CONTROL( "text_devolution",            m_pkTextDevolution );
    LINK_CONTROL( "text_hatch",                 m_pkTextHatch );
    LINK_CONTROL( "evolution_button",           m_pkEvolutionButton );
    LINK_CONTROL( "devolution_button",          m_pkDevolutionButton );
    LINK_CONTROL( "hatch_button",               m_pkHatchButton );
    LINK_CONTROL( "radio_button_box",           m_pkRadioButtonBox );
    LINK_CONTROL( "exit_button",                m_pkExit );
    LINK_CONTROL( "select_name_desc_static",    m_pkSelectPetNameDesc );
}

KGCSelectPetNameBox::~KGCSelectPetNameBox(void)
{
    if( m_pkPetModel != NULL )
    {
        m_pkPetModel->EndPet();
        SAFE_DELETE( m_pkPetModel );
    }
}

void KGCSelectPetNameBox::OnCreate( void )
{   

    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X        = static_cast<DWORD>(( VIEWPORT_OFFSET_X + vPos.x) * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Y        = static_cast<DWORD>(( VIEWPORT_OFFSET_Y + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY() );
    m_viewport.Width    = static_cast<DWORD>(VIEWPORT_WIDTH * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height   = static_cast<DWORD>(VIEWPORT_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ     = 0.0f;
    m_viewport.MaxZ     = 1.0f;

    m_pkTextEvolution->InitState( true );
    m_pkTextDevolution->InitState( false );
    m_pkTextHatch->InitState( false );
    m_pkEvolutionButton->InitState( true, true, this );
    m_pkDevolutionButton->InitState( false, true, this );
    m_pkHatchButton->InitState( false ,true, this );
    m_pkRadioButtonBox->InitState( true, true, this );
    m_pkExit->InitState( true, true, this );
    m_pkSelectPetNameDesc->InitState( true );

    m_pkExit->SetHotKey( DIK_ESCAPE );

    m_pkSelectPetNameDesc->SetText( g_pkStrLoader->GetString( STR_ID_PET_EVOLUTION_MESSAGE3 ) );
}

void KGCSelectPetNameBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
        g_KDSound.Play( "30" );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkExit )
        {
            g_KDSound.Play( "31" );
            SpeakToActionListener( KActionEvent( this, EXIT ) );
        }
        else if( event.m_pWnd == m_pkEvolutionButton || event.m_pWnd == m_pkDevolutionButton )
        {
            g_KDSound.Play( "31" );
            SpeakToActionListener( KActionEvent( this, TRANSFORM_SELECTNAME ) );
        }
    }
}

void KGCSelectPetNameBox::FrameMoveInEnabledState()
{
    if( m_pkPetModel != NULL && m_pkPetModel->m_iPetID != -1 )
    {
        if( m_pkPetModel->cFrame >= (int)m_pkPetModel->GetPetMotionFrm(  m_pkPetModel->usMotion  )->GetNum_Frame() - 1 )
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

void KGCSelectPetNameBox::PostChildDraw( void )
{
    D3DXMATRIX  mat1, mat2, mat3, matWorld;
    D3DXMATRIX  mat4;
    D3DVIEWPORT9 orgViewport;
	g_pGCDeviceManager2->Flush();	
    
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZENABLE,      TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

        g_pGCDeviceManager2->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS );

        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

		g_pd3dDevice->GetViewport( &orgViewport );
		g_pd3dDevice->SetViewport( &m_viewport );

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

            fPosX += m_pkPetModel->GetPetMotionFrm(  m_pkPetModel->usMotion  )->GetFrameData( m_pkPetModel->cFrame ).Plus_x * m_fZoom * fScale;
            fPosY += m_pkPetModel->GetPetMotionFrm(  m_pkPetModel->usMotion  )->GetFrameData( m_pkPetModel->cFrame ).Pos_y * m_fZoom * fScale;

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
		// 뷰포트 복구
		g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
		g_pd3dDevice->SetViewport( &orgViewport );
		g_pd3dDevice->SetTexture( 1, NULL );
    }
    g_pGCDeviceManager2->PopState();
}

void KGCSelectPetNameBox::InitPet( int iPetID, int iPetEvolutionLevel )
{
    if( m_pkPetModel == NULL )
        m_pkPetModel = new KPet();

    m_pkPetModel->StartPet( -1, iPetID, PET_LEVEL_IN_SHOP, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"" , false, 0, true );
    m_pkPetModel->SetPetMotion( EGCPETMOTION::SHOP );
    m_pkPetModel->m_pObject->SetShellMatrixScale( 0.25f );
    m_vPetAxisCorrection = m_pkPetModel->GetPetAxisCorrection();

    m_iMotionCount = 0;
    m_fAngleMove = 0.0f;
    m_fZoom = DEFAULTZOOM;
}

void KGCSelectPetNameBox::Release()
{
    m_dwPetUID = 0;

    if( m_pkPetModel != NULL )
    {
        m_pkPetModel->EndPet();
        SAFE_DELETE( m_pkPetModel );
    }
}

int KGCSelectPetNameBox::GetPetID()
{
    return m_pkPetModel->m_iPetID;
}
int KGCSelectPetNameBox::GetPetEvolutionLevel()
{
    return m_pkPetModel->m_iEvolutionLevel;
}

void KGCSelectPetNameBox::SetSelectPetNameBox( GCITEMUID dwPetUID, std::wstring strOriginalName, std::wstring strNewName, DWORD dwUseItemID, int iEvolutionType )
{
    // 진화상자인지 퇴화상자인지..
    m_iEvolutionType = iEvolutionType;
    m_dwUseItemID = dwUseItemID;

    m_pkTextEvolution->ToggleRender( m_iEvolutionType == EGCPETEVOLUTIONTYPE::EVOLUTION );
    m_pkTextDevolution->ToggleRender( m_iEvolutionType == EGCPETEVOLUTIONTYPE::DEVOLUTION );

    m_pkEvolutionButton->ToggleRender( m_iEvolutionType == EGCPETEVOLUTIONTYPE::EVOLUTION );
    m_pkDevolutionButton->ToggleRender( m_iEvolutionType == EGCPETEVOLUTIONTYPE::DEVOLUTION );

    // 펫의 이름
    m_strOriginalName = strOriginalName;
    m_strNewName = strNewName;

    m_dwPetUID = dwPetUID;

    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( dwPetUID );
    if( mit == g_MyD3D->m_mapPetInfo.end() )
    {
        ASSERT( !"Pet정보가 없다" );
        return;
    }

    KPetInfo kPetInfo = mit->second;
    int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPetInfo.m_dwID );
    int iPetEvolutionLevel;

    switch( m_iEvolutionType )
    {
        case EGCPETEVOLUTIONTYPE::EVOLUTION:
            iPetEvolutionLevel = kPetInfo.m_cPromotion + 1;
            break;
        case EGCPETEVOLUTIONTYPE::DEVOLUTION:
            iPetEvolutionLevel = kPetInfo.m_cPromotion - 1;
            break;
    }
    
    m_strCurrentPetName = kPetInfo.m_strName;

    DWORD dwOriginalPetExp = kPetInfo.m_mapInitExp[ kPetInfo.m_cPromotion ];
    DWORD dwNewPetExp = kPetInfo.m_mapInitExp[ (char)iPetEvolutionLevel ];

    int iOriginalPetLevel = GetPetLevel( dwOriginalPetExp );
    int iNewPetLevel = GetPetLevel( dwNewPetExp );

    // 펫이름 고르는 부분 세팅
    m_pkRadioButtonBox->SetPetName( m_strCurrentPetName, iOriginalPetLevel, m_strNewName, iNewPetLevel, m_strCurrentPetName.compare( strOriginalName ) != 0 );
    
    // 그려질 펫 세팅
    InitPet( iPetID, iPetEvolutionLevel );
}

void KGCSelectPetNameBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, DESTROYED ) );
}

std::wstring KGCSelectPetNameBox::GetSelectedPetName()
{
    return m_pkRadioButtonBox->IsCheckFirst() ? m_strCurrentPetName : m_strNewName;
}

bool KGCSelectPetNameBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    int iEvolutionType = 0;
    switch( kInfo_.m_nMsgBoxUse )
    {
    case KGCUIScene::GC_MBOX_USE_EVOLUTION_PET_NAME_SELECT:
        iEvolutionType = 0;
        break;
    case KGCUIScene::GC_MBOX_USE_DEVOLUTION_PET_NAME_SELECT:
        iEvolutionType = 1;
        break;
    }
    this->SetSelectPetNameBox(
        kInfo_.m_lParam, 
        kInfo_.m_strText1,
        kInfo_.m_strText2,
        static_cast<GCITEMID>(kInfo_.m_lParam2),
        iEvolutionType );

    return true;
}