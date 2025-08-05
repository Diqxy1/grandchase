#include "stdafx.h"
#include ".\kgcpetpreview.h"


//
//
//
#include "../MyD3D.h"


//




#include "KGCPetMessage.h"
#include "../GCRenderManager.h"
#include "../gcui/KGCAskUseVirtualCash.h"

IMPLEMENT_CLASSNAME( KGCPetPreview );
IMPLEMENT_WND_FACTORY( KGCPetPreview );
IMPLEMENT_WND_FACTORY_NAME( KGCPetPreview, "gc_pet_preview" );


#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM    0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM    0.40f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM 0.25f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM 0.32f

KGCPetPreview::KGCPetPreview(void)
{
    m_pkPet = NULL;
    //m_pMeshKOGRoom  = NULL;

    m_pPetWaitMotion    = NULL;
    m_pPetActionMotion    = NULL;
    m_iMotionCount    = 0;
    m_fAngleMove    = 0.0f;

    m_fZoom = DEFAULTZOOM;
    m_fCharX = 0.0f;
    m_bAngleChanged = false;

    m_iPetDescState = -1;


    LINK_CONTROL( "btn_ok",                m_pkOk );    
    LINK_CONTROL( "btn_close",            m_pkClose );        
    LINK_CONTROL( "title_static",        m_pkPetTitle );    
    LINK_CONTROL( "name_static",        m_pkPetName );        
    LINK_CONTROL( "description_static", m_pkPetDesc );    
    LINK_CONTROL( "pet_message",        m_pkPetMsg );
    LINK_CONTROL( "buy_button",            m_pkBuy );
    LINK_CONTROL( "cancel_button",        m_pkCancle );
    LINK_CONTROL( "slash",                m_pkSlash );
}

KGCPetPreview::~KGCPetPreview(void)
{
    //if( m_pMeshKOGRoom != NULL )
    //SAFE_DELETE( m_pMeshKOGRoom );

    if( m_pkPet != NULL )
        m_pkPet->EndPet();
    SAFE_DELETE( m_pkPet);
    m_pPetWaitMotion = NULL;
    m_pPetActionMotion = NULL;
}

void KGCPetPreview::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        g_KDSound.Play( "30" );
        return;
    }

    if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK ) return;


    if( event.m_pWnd == m_pkClose || event.m_pWnd == m_pkOk || event.m_pWnd == m_pkCancle )
    {
        SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ) );
        m_pkPetMsg->SetPetID( -1, 0 );
        m_pkPet->EndPet();
    }

    if( event.m_pWnd == m_pkBuy )
    {
        SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ) );

        int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;
        // 인벤토리에 여유가 있다면..
        if ( g_MyD3D->m_kItemMgr.GetTotalInventorySize() < iInventoryCapacity )
        {
            int iItemID = g_kGlobalValue.GetPetTemplate(m_pkPet->m_iPetID,m_pkPet->m_iEvolutionLevel).dwGoodsID;
            GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( iItemID);
            RETURN_NIL(kItem);

#ifdef ENABLE_POINT_SYSTEM // Kawan>
            // 포인트 아이템일 경우..
            if( g_MyD3D->m_kItemMgr.IsPointShopItem( iItemID ) )
            {
                int iPointItemPrice = g_MyD3D->m_kItemMgr.GetPointItemPrice( iItemID );

                // 포인트가 충분하면..
                if( iPointItemPrice <= g_kGlobalValue.m_iGCPoint )
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
                        g_pkStrLoader->GetReplacedString( STR_ID_BUY_POINT_ITEM, "li", kItem->strItemName, iPointItemPrice ), L"",
                        KGCUIScene::GC_MBOX_USE_BUY_POINT_ITEM, iItemID, 1 );
                }
                else // 포인트가 부족하다면..
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetReplacedString( STR_ID_NOT_ENOUGH_POINT, "l", kItem->strItemName ), L"",
                        KGCUIScene::GC_MBOX_USE_NORMAL );
                }
            }
            else 
#endif
            if ( kItem->eMoneyType == EMT_CASH )// 캐쉬 아이템일 경우
            {
#ifdef _OPEN_TEST_
                {
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                        g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY1 ),
                        g_pkStrLoader->GetString( STR_ID_TEST_SVR_NOT_BUY2 ) );
                }
#else // !_OPEN_TEST_
                {
                    if ( g_kGlobalValue.m_kUserInfo.iAge < CASH_BUY_LIMIT_AGE ) // 만 7세 미만인지 확인
                    {
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                            g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY1 ),
                            g_pkStrLoader->GetString( STR_ID_LESSTHAN12_NOT_BUY2 ) );
                    }
                    else
                    {
                        bool bOpenBuyWnd = true;
                        /*
                        #if defined( VIRTUAL_CASH ) && !defined( GAME_CASH )
                        if( g_kGlobalValue.m_dwVirtualCash >= kItem->dwPrice )
                        {
                            g_pkUIScene->m_pkAskUseVirtualCash->SetItemInfo( iItemID, -1 );
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_BUY_VIRTUAL_CASH );
                            bOpenBuyWnd = false;
                        }
                        #endif
                        */

                        if( bOpenBuyWnd )
                        {
                    #if defined ( USE_VIP_POINT )
                            g_pItemMgr->BuyCashItemS6( kItem->dwGoodsID );
                    #else
                        #ifdef GAME_CASH
                            g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false );
                        #else    
                            g_MyD3D->m_kItemMgr.BuyItem( iItemID, -1  );
                        #endif
                    #endif
                        }
                    }
                }
#endif // end _OPEN_TEST_
            }
            else // GP 이면..
            {
                std::wostringstream strmText1;
                std::wostringstream strmText2;

#if defined ( _KOREAN ) || defined( _SPANISH )
                strmText1 << kItem->strItemName;
                strmText2 << g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "i", kItem->dwPrice );
#elif defined( _ENGLISH ) || defined(_THAI_LANG)
                strmText1<< g_pkStrLoader->GetReplacedString( STR_ID_BUY_PRICE, "li", kItem->strItemName, (int)kItem->dwPrice );
#else
                strmText1 << kItem->strItemName << g_pkStrLoader->GetString( STR_ID_OBJECT_POSTPOSITION );
                strmText2 << kItem->dwPrice << ' ' << g_pkStrLoader->GetString( STR_ID_BUY_PRICE );
#endif

                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO,
                    strmText1.str(), strmText2.str(),
                    KGCUIScene::GC_MBOX_USE_BUY, iItemID, -1 );
            }
        }

        m_pkPet->EndPet();
    }
}

void KGCPetPreview::OnCreate( void )
{
    if( m_pkPet == NULL )
        m_pkPet = new KPet();

    m_pkOk->SetSelfInputCheck( true );
    m_pkOk->AddActionListener( this );
    m_pkClose->SetSelfInputCheck( true );
    m_pkClose->AddActionListener( this );
    m_pkBuy->SetSelfInputCheck( true );
    m_pkBuy->AddActionListener( this );
    m_pkCancle->SetSelfInputCheck( true );
    m_pkCancle->AddActionListener( this );
    m_pkSlash->SetSelfInputCheck( false );
    m_pkSlash->AddActionListener( NULL );

    m_pkPetName->SetAlign( DT_LEFT );
    m_pkPetDesc->SetAlign( DT_LEFT );
    m_pkPetDesc->SetMultiLine( true );
    m_pkPetDesc->SetLineSpace( 1.2f );    
    m_pkPetTitle->SetAlign( DT_LEFT );    
    m_pkPetTitle->SetText( g_pkStrLoader->GetString( STR_ID_PET_PREVIEW ) );

    m_pkPetMsg->InitState(false, true);

    m_fZoom = DEFAULTZOOM;
    m_fCharX = 0.0f;
    m_bAngleChanged = false;

    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X        = static_cast<DWORD>(( VIEWPORT_OFFSET_X + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y        = static_cast<DWORD>(( VIEWPORT_OFFSET_Y + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width    = static_cast<DWORD>(VIEWPORT_WIDTH * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height   = static_cast<DWORD>(VIEWPORT_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ     = 0.0f;
    m_viewport.MaxZ     = 1.0f;

    //m_pMeshKOGRoom = new GCObject();
    //m_pMeshKOGRoom->SetMesh( g_pGCDeviceManager2->CreateMeshP3M( "MyRoom.p3m" ) );
    //m_pMeshKOGRoom->AddTexture( g_pGCDeviceManager2->CreateTexture( "MyRoom.dds") );

}

void KGCPetPreview::SetShopPreview()        //샵에서는 구입버튼과 취소 버튼이 활성화 됩니다.
{
    m_pkBuy->InitState( true, true, this );
    m_pkCancle->InitState( true, true, this );
    m_pkSlash->InitState( true, true, this );
    m_pkOk->InitState( false, false, NULL );

    m_pkPetMsg->SetMentState( PET_MENT_SHOP );        
}

void KGCPetPreview::SetMyInfoPreview()    //인벤토리에서는 확인버튼만 활성화 됩니다.
{
    m_pkBuy->InitState( false, false, NULL );
    m_pkCancle->InitState( false, false, NULL );
    m_pkSlash->InitState( false, false, NULL );
    m_pkOk->InitState( true, true, this );

    m_pkPetMsg->SetMentState( PET_MENT_MYINFO );    
}

void KGCPetPreview::SetGachaPreview()
{
	m_pkBuy->InitState( false, false, NULL );
	m_pkCancle->InitState( false, false, NULL );
	m_pkSlash->InitState( false, false, NULL );
	m_pkOk->InitState( true, true, this );
}

void KGCPetPreview::FrameMoveInEnabledState( void )
{    
    m_pkPetMsg->SetPetID( m_pkPet->m_iPetID, m_pkPet->m_iEvolutionLevel );
    m_pkPetMsg->MsgFrameMove( false );

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
        if( m_pkPet->cFrame >= (int)m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame() - 1 )
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

    if( g_pkInput->IsDown( DIK_RETURN ) || g_pkInput->IsDown( DIK_ESCAPE ) 
#ifdef USE_JOYSTICK
        || g_MyD3D->GetJoystickKey() == EJK_KEY_SELECT
        || g_MyD3D->GetJoystickKey() == EJK_KEY_START
#endif
    )
    {
        SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ) );
        m_pkPetMsg->SetPetID( -1, 0 );
        m_pkPet->EndPet();
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
    }
}

void KGCPetPreview::PostChildDraw( void )
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
        g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );        

        g_pGCDeviceManager2->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
        g_pGCDeviceManager2->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS );

        g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

		
		g_pd3dDevice->GetViewport( &orgViewport );
		g_pd3dDevice->SetViewport( &m_viewport );

        g_pd3dDevice->SetFVF( SKINVERTEX::FVF );

        // 퍼스펙티브 설정
        ::D3DXMatrixPerspectiveLH( &mat3, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ),
            1.0f, 1.0f, 100.0f );
        ::D3DXMatrixScaling( &mat1, 1.0f, 1.0f, 0.01f );
        ::D3DXMatrixMultiply( &mat2, &mat3, &mat1 );
        ::D3DXMatrixTranslation( &mat3, 0.0f, 0.172f, 0.5f );
        ::D3DXMatrixMultiply( &mat1, &mat2, &mat3 );
        g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat1 );
		g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );


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

			if (m_pkPet->m_iPetID == 147)
			{
				fPosY = -0.46f;
			}

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

void KGCPetPreview::ChangeCharWndZoom( bool _bZoomUp )
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

void KGCPetPreview::ProcessCharWndAngle()
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCPetPreview::SetNowPet( int iPetID_, int iPetEvolutionLevel )
{    
    if( m_pkPet == NULL )
        m_pkPet = new KPet();

    m_pkPet->StartPet( -1, iPetID_, PET_LEVEL_IN_SHOP, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"" ,false, 0, true );
    m_pkPet->SetPetMotion( EGCPETMOTION::SHOP );    
    m_pkPet->m_pObject->SetShellMatrixScale( 0.25f );
    m_vPetAxisCorrection = m_pkPet->GetPetAxisCorrection();
    m_pkPetName->SetText( g_pkStrLoader->GetString(m_pkPet->GetPetNameID() ) );    
    if( m_iPetDescState != -1 )
    {
        m_pkPetDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( m_pkPet->GetPetDesc( m_iPetDescState ) ) );
    }
}
