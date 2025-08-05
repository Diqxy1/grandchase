#include "stdafx.h"
//
#include "GCPetWnd.h"
//
#include "../MyD3D.h"

//
//

#include "gcui/KGCPetMessage.h"
#include "gcui/GCMyInfoScene.h"
//


#include "GCRenderManager.h"
#include "GCStateMachine.h"

IMPLEMENT_CLASSNAME( KGCPetWnd );
IMPLEMENT_WND_FACTORY( KGCPetWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCPetWnd, "gc_pet_wnd" );

#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM	0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM	0.25f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM 0.18f

#define PET_EXP_LENGTH 306		//UI에 있는 토탈 게이지바 길이.

KGCPetWnd::KGCPetWnd( void )
{
	m_iExtraCharNum = 0;
	m_fAngleMove    = 0.0f;

	m_bShowInfo     = true;

	m_pShopMotion	= NULL;
	m_iMotionCount	= 0;

	m_pkInit        = NULL;
	m_pkExpBack		= NULL;
	m_pkExp			= NULL;
    m_pkUnEquip     = NULL;

	m_pkMagicStoneBtn = NULL;

	LINK_CONTROL( "exp_back",				m_pkExpBack );
	LINK_CONTROL( "exp",					m_pkExp );
	LINK_CONTROL( "exp_pecent_static",		m_pkExpPercentStatic );
	LINK_CONTROL( "pet_init_button",		m_pkInit );
    LINK_CONTROL( "pet_message",			m_pkPetMsg );
    LINK_CONTROL( "close_btn",		    	m_pkUnEquip );

	m_fZoom = 0.22f;
	m_bAngleChanged = false;

	m_pkViewPortRect = NULL;
	LINK_CONTROL( "view_port_rect",		    	m_pkViewPortRect );

	LINK_CONTROL( "pet_magicstone_button", m_pkMagicStoneBtn); 

	m_pkPet = NULL;
}

KGCPetWnd::~KGCPetWnd( void )
{
    if( m_pkPet != NULL )
	    m_pkPet->EndPet();
	SAFE_DELETE( m_pkPet );
}

void KGCPetWnd::OnDestroy( void )
{
}

void KGCPetWnd::ResetUIforResolution()
{

	D3DXVECTOR2 vPos( GetFixedWindowPos() );	
	D3DXVECTOR2 vAddLocalPos(m_pkViewPortRect->GetFixedWindowLocalPos());
	m_viewport.X = static_cast<DWORD>((vPos.x +vAddLocalPos.x) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>((vPos.y + vAddLocalPos.y) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( m_pkViewPortRect->GetWidth() * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( m_pkViewPortRect->GetHeight() * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ     = 0.0f;
	m_viewport.MaxZ     = 1.0f;
	KD3DWnd::ResetUIforResolution();
}

void KGCPetWnd::OnCreate( void )
{
	if( m_pkPet == NULL )
		m_pkPet = new KPet();

	m_pkInit->SetSelfInputCheck( true );
	m_pkInit->AddActionListener( this );

	if (m_pkMagicStoneBtn)
	{
		m_pkMagicStoneBtn->InitState(true, true, this);
	}

	m_pkPetMsg->InitState(false, true);	

	m_pkUnEquip->InitState(true, true, this);

	D3DXVECTOR2 vPos( GetFixedWindowPos() );	
	D3DXVECTOR2 vAddLocalPos(m_pkViewPortRect->GetFixedWindowLocalPos());
	m_viewport.X = static_cast<DWORD>((vPos.x +vAddLocalPos.x) * g_pGCDeviceManager2->GetWindowScaleX());
	m_viewport.Y = static_cast<DWORD>((vPos.y + vAddLocalPos.y) * g_pGCDeviceManager2->GetWindowScaleY());
	m_viewport.Width = static_cast<DWORD>( m_pkViewPortRect->GetWidth() * g_pGCDeviceManager2->GetWindowScaleX() );
	m_viewport.Height = static_cast<DWORD>( m_pkViewPortRect->GetHeight() * g_pGCDeviceManager2->GetWindowScaleY() );
	m_viewport.MinZ     = 0.0f;
	m_viewport.MaxZ     = 1.0f;

	for( int j = 0; j < 1; j++ )	// 2차 전직까지
	{
		for( int i = 0; i < GC_CHAR_NUM; i++ )
		{
			char strTemp[MAX_PATH];
			std::string strShopMotion;
			sprintf( strTemp, "shop_%d_%d", i, j );

			strShopMotion = strTemp;
			


			// i + j * GC_CHAR_NUM == m_iExtraCharNum
			m_pWaitMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_0.frm" );
			if( m_pWaitMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
			{				
				SAFE_RELEASE(m_pWaitMotion[i + j * GC_CHAR_NUM ]);
			}
			m_pActionMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_1.frm" );
			if( m_pActionMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
			{				
				SAFE_RELEASE(m_pActionMotion[i + j * GC_CHAR_NUM ]);
			}
		}
	}

	m_pkViewPortRect->InitState(false);

}

void KGCPetWnd::OnActivate( void )
{
	//Initialize에 구현합시다..
}

void KGCPetWnd::SetPetExp( bool bRender )
{	
	KPetInfo kPetinfo = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;

	float fPercent = 0.0f;
	if( bRender && kPetinfo.m_dwUID != 0 )
	{
		fPercent = GetPetExpPercent( kPetinfo.m_dwEXP );
	}
	int iPercentWidth = (int)(PET_EXP_LENGTH * fPercent / 100.0f);

	m_pkExp->SetWidth(iPercentWidth);

	WCHAR strTemp[10];
	swprintf(strTemp, 10, L"%3.1f%%", fPercent);
	m_pkExpPercentStatic->SetText(strTemp);
}

void KGCPetWnd::FrameMoveInEnabledState( void )
{
	if( !m_bAngleChanged )
	{
		m_fAngleMove += 0.005f;
		if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
	}

	//if( m_pkPet->m_iPetID != -1 )
	//	SetPetExp();

	if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwUID != 0 )
	{
		m_pkPetMsg->SetPetID( g_kGlobalValue.GetPetIDFromItemID( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwID ), 
                                g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_cPromotion );
	}	
	else
	{
		m_pkPetMsg->SetPetID( -1, 0 );
	}

	m_pkPetMsg->MsgFrameMove();

// 	if ( m_bShowInfo )
// 	{
// 		ShowPetInfo();
// 	}

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

	if( g_pkInput->BtnDown() == TRUE && CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == true )
	{
		m_bAngleChanged = true;
		m_ptStartMouse = g_pkInput->GetMousePos();
		m_fStartAngle = m_fAngleMove;
		Internal = true;
	}
	else if( g_pkInput->BtnDown() == TRUE && CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == false )
		Internal = false;

	if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE )
	{
		m_bAngleChanged = false;
	}

	if( g_pkInput->BtnPressed() && m_bAngleChanged == true && Internal == true )
		ProcessCharWndAngle();

    if( m_pkPet->m_iPetID != -1 )
    {   
        if ( m_pkPet->cFrame >= (int)m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame() - 1 )
        {
            m_iMotionCount++;
            if( m_iMotionCount == 5 )
            {
                m_pkPet->SetPetMotion( EGCPETMOTION::SHOP_ACTION );
                m_iMotionCount = 0;
            }
            else
            {
                m_pkPet->SetPetMotion( EGCPETMOTION::SHOP );
            }
        }
    }
}

void KGCPetWnd::PostDraw( void )
{
	if( m_pkPet->m_iPetID == -1 )
		return;
	g_pGCDeviceManager2->Flush();
	D3DXMATRIX  mat1, mat2, mat3, matWorld;
	D3DVIEWPORT9 orgViewport;

	

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
		::D3DXMatrixPerspectiveLH( &mat3, float( m_pkViewPortRect->GetWidth() )/ float( m_pkViewPortRect->GetHeight() ),
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
		g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX(4), &matWorld );
		//m_pMeshKOGRoom->Render();


		// 여기 그려주자 펫또..=ㅅ=;
		::D3DXMatrixIdentity(&mat1);
		::D3DXMatrixIdentity(&mat2);
        ::D3DXMatrixIdentity(&mat3);
        ::D3DXMatrixIdentity(&matWorld);

		float fScale = 6.0f * m_pkPet->m_fScale;
		//중심축 보정			

		float fPosX = 0.0f, fPosY = -0.14f, fPosZ = 0.303f;

		fPosX += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Plus_x * m_fZoom * fScale;
		fPosY += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Pos_y * m_fZoom * fScale;// - (m_fZoom * fScale) * 0.25;

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
        
        m_pkPet->m_pObject->SetShellMatrixScale( m_fZoom * fScale );

        g_RenderManager->Render( false , m_pkPet->m_pObject );
        
	}
	g_pGCDeviceManager2->PopState();

	// 뷰포트 복구
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
	g_pd3dDevice->SetViewport( &orgViewport );
	g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCPetWnd::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
	{
		g_KDSound.Play( "30" );
		return;
	}

	if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK ) return;

	//if ( event.m_pWnd == m_pkLeft )
	//{
	//	g_KDSound.Play( "31" );
	//	g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex--;
	//	if ( g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex < 0 ) 
	//		g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCharSize() - 1;

	//	//printf( "선택한 케릭터 : %d 잡레벨 : %d\n", static_cast<int>(g_MyD3D->m_TempPlayer.m_UserInfo.iSelectChar),
	//	//	static_cast<int>(g_MyD3D->m_TempPlayer.m_UserInfo.GetCurrentChar().iJobLevel) );

	//	g_MyD3D->m_TempPlayer.Extra_Char_Num = GC_CHAR_NUM * static_cast<int>(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iPromotionLevel );
	//	g_MyD3D->m_TempPlayer.UpdatePlayerResource();
	//	SetChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);		
	//	SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );
	//}
	//else if ( event.m_pWnd == m_pkRight )
	//{
	//	g_KDSound.Play( "31" );
	//	g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex++;
	//	if ( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCharSize() == g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex ) 
	//		g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = 0;

	//	//printf( "선택한 케릭터 : %d 잡레벨 : %d\n", static_cast<int>(g_MyD3D->m_TempPlayer.m_UserInfo.iSelectChar),
	//	//	static_cast<int>(g_MyD3D->m_TempPlayer.m_UserInfo.GetCurrentChar().iJobLevel) );

	//	g_MyD3D->m_TempPlayer.Extra_Char_Num = GC_CHAR_NUM * static_cast<int>(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iPromotionLevel );		
	//	g_MyD3D->m_TempPlayer.UpdatePlayerResource();
	//	SetChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
	//	SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );		
	//}
	
	if ( event.m_pWnd == m_pkInit )
	{
		g_KDSound.Play( "31" );

// 		SCharInfo &SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
// 
// 		if(int i = 0 ; i < SChar->vecItemInfo.size(); ++i)
// 		{
// 			GCItem* pItem = g_pItemMgr->GetItemData(SChar->vecItemInfo[i].iItemID);
// 			if(pItem->dwSlotPosition & ESP_PET)
// 			{
// 				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PET_COSTUME_INVEN );
// 				return;
// 			}
// 		}

#if defined( PET_COSTUME )
		if( g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo.m_dwUID == 0 )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_EQUIP_PET ) );
		}
		else
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PET_COSTUME_INVEN );
		}

		return;
#else
		//2006.12.18 wony
		// 펫 : 펫 장착해제
		ReleaseCapture();
		
		SCharInfo &SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
		g_MyD3D->m_TempPlayer.UnEquipPet( SChar.iCharType );
		
		g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
		g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
		g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false,g_pkUIScene->m_pkMyInfoScene->GetEquipCategory() );
		
		return;
#endif
	}
    else if ( event.m_pWnd == m_pkUnEquip )
    {
        // 펫 : 펫 장착해제
        ReleaseCapture();

        SCharInfo &SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
        g_MyD3D->m_TempPlayer.UnEquipPet( SChar.iCharType );

        g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
        g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData( false,g_pkUIScene->m_pkMyInfoScene->GetEquipCategory() );
    }

	if ( event.m_pWnd == m_pkMagicStoneBtn )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBAX_PET_MAGIC_STONE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
}

void KGCPetWnd::ChangeCharWndZoom( bool _bZoomUp )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	if( CheckPosInWindowBound(vMousePos) == true )
	{
		// 0.25~0.15
		if( g_MyD3D->m_pStateMachine->IsStateMyInfo() || g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH || g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP )
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
		else return;
	}
}

void KGCPetWnd::ProcessCharWndAngle()
{
	POINT CurrentMousePoint = g_pkInput->GetMousePos();

	m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}
