#include "StdAfx.h"
#include "MyD3D.h"
#include "uifx/D3DView.h"
#include "GCPetCostumeInven.h"
#include "GCPetCostumeWnd.h"
#include "../GCRenderManager.h"


IMPLEMENT_CLASSNAME( KGCPetCostumeInven );
IMPLEMENT_WND_FACTORY( KGCPetCostumeInven );
IMPLEMENT_WND_FACTORY_NAME( KGCPetCostumeInven, "gc_kgcpetcostumeinven" );

#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM   0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM     0.32f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM     0.25f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM MAXZOOM

KGCPetCostumeInven::KGCPetCostumeInven(void)
: m_iFirstSlotIndex( 0 )
, m_dwPetCostume( 0 )
, m_pkPet( NULL )
, m_fAngleMove( 0.f )
, m_fZoom( DEFAULTZOOM )
, m_bAngleChanged( false )
, m_fStartAngle( 0.f )
, m_dwPetItemID( 0 )
, m_iPromotionLv( 0 )
{
	m_pkPreview = NULL;
	m_apkPageBtn[GCPCI_BTN_LEFT] = NULL;
	m_apkPageBtn[GCPCI_BTN_RIGHT] = NULL;
	m_pkHowToGetPetCostume = NULL;
	m_pkClose = NULL;
	LINK_CONTROL( "btn_close", m_pkClose );
	LINK_CONTROL( "static_howtogetcostume", m_pkHowToGetPetCostume );
	LINK_CONTROL( "left_button",	m_apkPageBtn[GCPCI_BTN_LEFT] );
	LINK_CONTROL( "right_button",	m_apkPageBtn[GCPCI_BTN_RIGHT] );
	LINK_CONTROL( "preview", m_pkPreview );

	for( int i = 0 ; i < GCPCI_SLOT_NUM ; ++i )
	{
		m_apkPetItemSlot[i] = NULL;
		LINK_CONTROL_STM( "petslot"<<i, m_apkPetItemSlot[i] );
	}
}

KGCPetCostumeInven::~KGCPetCostumeInven(void)
{
}

void KGCPetCostumeInven::OnCreate( void )
{
	if( KP2P::GetInstance()->IsConnectedToGameServer() == false )
		return;

	m_pkPreview->InitState( true );
	m_apkPageBtn[GCPCI_BTN_LEFT]->InitState( true, true, this );
	m_apkPageBtn[GCPCI_BTN_RIGHT]->InitState( true, true, this );
#if !defined (NATION_THAILAND)
    m_pkHowToGetPetCostume->InitState( true );
	m_pkHowToGetPetCostume->SetAlign( DT_CENTER );
	m_pkHowToGetPetCostume->SetText( g_pkStrLoader->GetString( STR_ID_PET_COSTUME0 ) );
	m_pkHowToGetPetCostume->SetToolTip( g_pkStrLoader->GetString( STR_ID_PET_COSTUME1 ) );
#endif
	m_pkClose->InitState( true, true, this );
    m_pkClose->SetHotKey( DIK_ESCAPE );
    m_pkClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPetCostumeInven::OnClose );
	m_dwPetItemID = g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwID;
	int iPetID = g_kGlobalValue.GetPetIDFromItemID(m_dwPetItemID);
	m_iPromotionLv = g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_cPromotion;

	m_vecCostumeList.clear();
	m_vecCostumeList=g_kGlobalValue.GetPetTemplateFromItemID( m_dwPetItemID, m_iPromotionLv ).vecCostumeList;
	m_vecCostumeList.insert( m_vecCostumeList.begin(), m_dwPetItemID );

	int iCostumeNum = (int)m_vecCostumeList.size();
	bool bArrowBtnLock = iCostumeNum < GCPCI_SLOT_NUM;
	m_apkPageBtn[GCPCI_BTN_LEFT]->Lock( bArrowBtnLock );
	m_apkPageBtn[GCPCI_BTN_RIGHT]->Lock( bArrowBtnLock );
	
	m_iFirstSlotIndex = 0;
	for( int i = 0 ; i < GCPCI_SLOT_NUM ; ++i )
	{
		if( iCostumeNum <= i + m_iFirstSlotIndex )
		{
			m_apkPetItemSlot[i]->HideImage();
			m_apkPetItemSlot[i]->SetBaseItemID(0, 0);
			continue;
		}
		m_apkPetItemSlot[i]->ShowImage();
		m_apkPetItemSlot[i]->SetBaseItemID(m_dwPetItemID, m_iPromotionLv);
		m_apkPetItemSlot[i]->InitState( true, true, this );
		m_apkPetItemSlot[i]->SetItemID( m_vecCostumeList[i + m_iFirstSlotIndex] );
	}

	m_dwPetCostume = g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID;

    m_bAngleChanged = false;

	SetPetModel( iPetID, m_iPromotionLv, m_dwPetCostume );

    // 뷰 포트 초기화.
    D3DXVECTOR2 vPos( m_pkPreview->GetFixedWindowPos() );
    m_viewport.X        = static_cast<DWORD>( vPos.x * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Y        = static_cast<DWORD>( vPos.y * g_pGCDeviceManager2->GetWindowScaleY() );
    m_viewport.Width    = static_cast<DWORD>(m_pkPreview->GetWidth() * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height   = static_cast<DWORD>(m_pkPreview->GetHeight() * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ     = 0.0f;
    m_viewport.MaxZ     = 1.0f;
}

void KGCPetCostumeInven::FrameMoveInEnabledState( void )
{
	if( m_dwPetCostume != g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID )
	{
		DWORD dwPetItemID = g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_dwID;
		int iPetID = g_kGlobalValue.GetPetIDFromItemID(dwPetItemID);
		int iPromotionLv = g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_cPromotion;
		m_dwPetCostume = g_kGlobalValue.m_kUserInfo.GetCurrentChar().kPetInfo.m_kCostume.m_dwID;
		SetPetModel( iPetID, iPromotionLv, m_dwPetCostume );
	}

    if( !m_bAngleChanged )
    {
        m_fAngleMove += 0.005f;
        if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
    }

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

    static bool Internal = false;
    if( g_pkInput->BtnDown() == TRUE )
    {
        if( m_pkPreview->CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == true )
        {
            m_bAngleChanged = true;
            m_ptStartMouse = g_pkInput->GetMousePos();
            m_fStartAngle = m_fAngleMove;
            Internal = true;
        }
        else
        {
            Internal = false;
        }
    }

    if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE )
    {
        m_bAngleChanged = false;
    }

    if( g_pkInput->BtnPressed() && m_bAngleChanged == true && Internal == true )
        ProcessCharWndAngle();
}

void KGCPetCostumeInven::ProcessCharWndAngle()
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCPetCostumeInven::ChangeCharWndZoom( bool _bZoomUp )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if( m_pkPreview->CheckPosInWindowBound(vMousePos) == true )
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

void KGCPetCostumeInven::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_UP )
	{
		if( event.m_pWnd == m_apkPageBtn[GCPCI_BTN_RIGHT] )
			OnClickPageMove( true );
		else if( event.m_pWnd == m_apkPageBtn[GCPCI_BTN_LEFT] )
			OnClickPageMove( false );

		for( int i = 0 ; i < GCPCI_SLOT_NUM ; ++i )
		{
			if( event.m_pWnd != m_apkPetItemSlot[i] )
				continue;

			m_apkPetItemSlot[i]->OnClick();
			break;
		}
	}
}

void KGCPetCostumeInven::OnClose()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCPetCostumeInven::OnClickPageMove( bool bRight )
{
	m_iFirstSlotIndex = m_iFirstSlotIndex + (bRight?1:-1);

	if( m_iFirstSlotIndex < 0 )
		m_iFirstSlotIndex = 0;

	if( m_iFirstSlotIndex + GCPCI_SLOT_NUM >= (int)m_vecCostumeList.size() )
		m_iFirstSlotIndex = (int)m_vecCostumeList.size() - GCPCI_SLOT_NUM;

    SetSlotInfo();
}

void KGCPetCostumeInven::PostChildDraw( void )
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
        ::D3DXMatrixPerspectiveLH( &mat3, float( m_pkPreview->GetWidth() ) / float( m_pkPreview->GetHeight() ),
            1.0f, 1.0f, 100.0f );
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
        //m_pMeshKOGRoom->Render();


        if( m_pkPet->m_iPetID != -1 )
        {
            ::D3DXMatrixIdentity(&mat1);
            ::D3DXMatrixIdentity(&mat2);
            ::D3DXMatrixIdentity(&mat3);
            ::D3DXMatrixIdentity(&matWorld);

            float fScale = 4.0f * m_pkPet->m_fScale;
            float fPosX = 0.0f, fPosY = -0.15f, fPosZ = 0.303f;

            fPosX += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Plus_x * m_fZoom * fScale;
            fPosY += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Pos_y * m_fZoom * fScale;

            fPosX = fPosX * cos( m_fAngleMove );

            ::D3DXMatrixTranslation( &mat3, m_vPetAxisCorrection.x, m_vPetAxisCorrection.y, m_vPetAxisCorrection.z );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat3 );                        
            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat1 );            
            ::D3DXMatrixRotationY( &mat2, m_fAngleMove );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat2 );            
            ::D3DXMatrixTranslation( &mat3, fPosX, fPosY, fPosZ );
            D3DXMatrixMultiply( &matWorld, &matWorld, &mat3 );

            m_pkPet->m_pObject->SetWorldMat( &matWorld );

            m_pkPet->m_pObject->SetFrame( &m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ) );

            m_pkPet->cFrame = ( m_pkPet->cFrame + 1 ) % m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame();

            g_RenderManager->Render(false, m_pkPet->m_pObject );

        }
        //else printf("ShopMotion이 NULL\n");
    }
    g_pGCDeviceManager2->PopState();

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCPetCostumeInven::SetPetModel( int iPetID, int iPetPromotion, DWORD dwCostume )
{
	// 펫 모델 로드
	if( m_pkPet == NULL )
		m_pkPet = new KPet();

	m_pkPet->StartPet( -1, iPetID, PET_LEVEL_IN_SHOP, iPetPromotion, EGCPETLOCATION::SHOP, L"" ,false,dwCostume, true );
	m_pkPet->SetPetMotion( EGCPETMOTION::SHOP );
	m_pkPet->m_pObject->SetShellMatrixScale( 0.25f );
    m_vPetAxisCorrection = m_pkPet->GetPetAxisCorrection();
}

void KGCPetCostumeInven::SetSlotInfo()
{
    for( int i = 0 ; i < GCPCI_SLOT_NUM ; ++i )
    {
        if( (int)m_vecCostumeList.size() <= i + m_iFirstSlotIndex )
        {
            m_apkPetItemSlot[i]->HideImage();
            m_apkPetItemSlot[i]->SetBaseItemID(0, 0);
            continue;
        }
        m_apkPetItemSlot[i]->ShowImage();
        m_apkPetItemSlot[i]->SetBaseItemID(m_dwPetItemID, m_iPromotionLv);
        m_apkPetItemSlot[i]->InitState( true, true, this );
        m_apkPetItemSlot[i]->SetItemID( m_vecCostumeList[i + m_iFirstSlotIndex] );
    }
}
