#include "stdafx.h"
//#include "KGCCharViewWnd.h"
////
//////
//////
//
//
//////
////
////
////
////
//////
////
////
//////
////
////
//
////
////
////
//
//
//IMPLEMENT_CLASSNAME( KGCCharViewWnd );
//IMPLEMENT_WND_FACTORY( KGCCharViewWnd );
//IMPLEMENT_WND_FACTORY_NAME( KGCCharViewWnd, "gc_mesh_wnd" );
//
//#define DELTAZOOM 0.01f
//#define MAXZOOM 0.4f
//#define MINZOOM 0.15f
//
//KGCCharViewWnd::KGCCharViewWnd( void )
//{
//    m_iExtraCharNum = 0;
//    m_pd3dDevice = NULL;
//    m_fAngleMove = 0.0f;
//
//    m_pMeshKOGRoom = NULL;
//
//    m_pShopMotion = NULL;
//    m_iMotionCount = 0;
//
//    m_fZoom = 0.25f;
//    m_fCharX = 0.0f;
//    m_bAngleChanged = false;
//
//    m_pkPet = NULL;
//}
//
//KGCCharViewWnd::~KGCCharViewWnd( void )
//{
//    SAFE_DELETE( m_pMeshKOGRoom );
//    m_pShopMotion = NULL;
//    if( m_pkPet != NULL )
//        m_pkPet->EndPet();
//    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
//    SAFE_DELETE( m_pkPet );
//}
//
//void KGCCharViewWnd::OnDestroy( void )
//{
//}
//
//void KGCCharViewWnd::OnCreate( void )
//{
//    m_pd3dDevice = *m_ppd3dDevice;
//
//    D3DXVECTOR2 vPos( GetFixedWindowPos() );
//    m_viewport.X = static_cast<DWORD>( VIEWPORT_OFFSET_X + vPos.x );
//    m_viewport.Y = static_cast<DWORD>( VIEWPORT_OFFSET_Y + vPos.y );
//    m_viewport.Width = VIEWPORT_WIDTH;
//    m_viewport.Height = VIEWPORT_HEIGHT;
//    m_viewport.MinZ = 0.0f;
//    m_viewport.MaxZ = 1.0f;
//
//    for( int j = 0; j < NUM_JOB_LEVEL; j++ )
//    {
//        for( int i = 0; i < GC_CHAR_NUM; i++ )
//        {
//            char strTemp[MAX_PATH];
//            std::string strShopMotion;
//            sprintf( strTemp, "shop_%d_%d", i, j );
//
//            strShopMotion = strTemp;
//
//            if( i == GC_CHAR_RYAN && j == 2 )
//            {
//                m_pWaitMotion[i + j * GC_CHAR_NUM ] = NULL;
//                m_pActionMotion[i + j * GC_CHAR_NUM ] = NULL;
//                continue;
//            }
//
//            if( j == 3 && i == GC_CHAR_RYAN )
//            {
//                m_pWaitMotion[i + j * GC_CHAR_NUM ] = NULL;
//                m_pActionMotion[i + j * GC_CHAR_NUM ] = NULL;
//                continue;
//            }
//
//            if( i == GC_CHAR_RONAN && j != 0 )
//            {
//                m_pWaitMotion[i + j * GC_CHAR_NUM ] = NULL;
//                m_pActionMotion[i + j * GC_CHAR_NUM ] = NULL;
//                continue;
//            }
//
//            // i + j * GC_CHAR_NUM == m_iExtraCharNum
//            m_pWaitMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_0.frm" );
//            if( m_pWaitMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
//            {
//                assert(0);
//                m_pWaitMotion[i + j * GC_CHAR_NUM ] = NULL;
//            }
//            m_pActionMotion[i + j * GC_CHAR_NUM] = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_1.frm" );
//            if( m_pActionMotion[i + j * GC_CHAR_NUM ]->IsDisable() )
//            {
//                assert(0);
//                m_pActionMotion[i + j * GC_CHAR_NUM ] = NULL;
//            }
//        }
//    }
//
//    if( m_pkPet == NULL )
//        m_pkPet = new KPet();
//    D3DXMATRIX matLook, matScale, matTran;
//    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ),
//        1.0f, 1.0f, 100.0f );
//    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
//    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
//    m_matProject = matLook * matScale * matTran;
//}
//
//void KGCCharViewWnd::OnActivate( void )
//{
//    //Initialize에 구현합시다..
//}
//
//void KGCCharViewWnd::FrameMoveInEnabledState( void )
//{
//    if( !m_bAngleChanged )
//    {
//        m_fAngleMove += 0.005f;
//        if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
//    }
//
//    if( g_pkInput->ISWheelUp() )
//    {
//        //printf("줌업\n");
//        ChangeCharWndZoom(true);
//    }
//    else if( g_pkInput->ISWheelDown() )
//    {
//        //printf("줌다운\n");
//        ChangeCharWndZoom(false);
//    }
//
//    static bool Internal = false;
//
//    if( g_pkInput->BtnDown() == TRUE && CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == true )
//    {
//        m_bAngleChanged = true;
//        m_ptStartMouse = g_pkInput->GetMousePos();
//        m_fStartAngle = m_fAngleMove;
//        Internal = true;
//    }
//    else if( g_pkInput->BtnDown() == TRUE && CheckPosInWindowBound( D3DXVECTOR2((float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y) ) == false )
//        Internal = false;
//
//    if( g_pkInput->BtnDown( 1 ) == TRUE )
//    {
//        m_bAngleChanged = false;
//    }
//
//    if( g_pkInput->BtnPressed() == 128 && m_bAngleChanged == true && Internal == true )
//        ProcessCharWndAngle();
//}
//
//void KGCCharViewWnd::PostDraw( void )
//{
//    if( g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex == -1 )
//        return;
//
//    D3DVIEWPORT9 orgViewport;
//
//    D3DXMATRIX matRot,matTrans,matScale,matWorld;
//    m_pd3dDevice->GetViewport( &orgViewport );
//    m_pd3dDevice->SetViewport( &m_viewport );
//
//    m_pd3dDevice->SetFVF( SKINVERTEX::FVF );
//    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
//
//    D3DXMatrixScaling( &matScale ,m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
//    D3DXMatrixRotationY( &matRot, m_fAngleMove );
//    D3DXMatrixTranslation( &matTrans, 0.0f, /*0.081f*/ 0.10f - ( MAXZOOM - m_fZoom ) * 0.25f, 0.32f );   
//
//    matWorld = matScale * matRot * matTrans;
//
//    m_pMeshKOGRoom->SetWorldMat( &matWorld );
//
//    g_RenderManager->Render( false , m_pMeshKOGRoom , EGCShaderTechnique::NOCARTOON_NOEDGE );
//
//    if( m_pShopMotion != NULL )
//    {
//        if( g_MyD3D->m_TempPlayer.Extra_Char_Num != m_iExtraCharNum )
//        {
//            int iSelectChar = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
//            g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( iSelectChar );
//            g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
//            SetChar(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
//        }
//
//        if ( g_MyD3D->m_TempPlayer.cFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
//        {
//            m_iMotionCount++;
//            if( m_iMotionCount == 5 )
//            {
//                if( m_pActionMotion[m_iExtraCharNum] != NULL )
//                {
//                    // 모션이 바뀌었음
//                    m_pShopMotion = m_pActionMotion[m_iExtraCharNum];
//                    m_iMotionCount = 0;
//                    m_fCharX = 0.0f;// X를 초기화
//                }
//            }
//            else
//            {
//                if( m_pWaitMotion[m_iExtraCharNum] != NULL )
//                {
//                    // 모션이 바뀌었음
//                    m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
//                    m_fCharX = 0.0f;// X를 초기화
//                }
//            }
//
//            g_MyD3D->m_TempPlayer.cFrame = 0;
//        }
//
//        float fPosX = m_fCharX * CHAR_SCALE_RATIO;
//        float fPosY = m_pShopMotion->GetFrameData(g_MyD3D->m_TempPlayer.cFrame).Pos_y * 0.1f - 0.06f;
//        float fPosZ = 0.303f;
//
//        m_fCharX += m_pShopMotion->GetFrameData(g_MyD3D->m_TempPlayer.cFrame).Plus_x;
//
//        fPosX = fPosX * cos( m_fAngleMove );
//
//        D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
//        D3DXMatrixRotationY( &matRot, m_fAngleMove );
//        D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );
//
//        matWorld = matScale * matRot * matTrans;
//        g_MyD3D->m_TempPlayer.m_pObject->SetWorldMat( &matWorld );
//        g_MyD3D->m_TempPlayer.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int) g_MyD3D->m_TempPlayer.cFrame ) );
//        g_RenderManager->Render( false , g_MyD3D->m_TempPlayer.m_pObject );
//        g_MyD3D->m_TempPlayer.cFrame++;
//
//        // 여기 그려주자 펫또..=ㅅ=; 캐릭터 옆에 위치시켜 줘야 한다
//        // 사이즈도 조금 작게 들어가야 하겠지..
//        if( m_pkPet->m_iPetID != -1 )
//        {
//            float fScale = 2.5f * m_pkPet->m_fScale;
//            D3DXMATRIX mat1, mat2, mat3, mat4, matWorld;
//
//            // 상하
//            ::D3DXMatrixTranslation( &mat4, -0.3f / fScale, -0.57f / fScale, -0.2f / fScale);
//            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
//            ::D3DXMatrixRotationY( &mat2, m_fAngleMove );
//
//            float fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.303f;
//
//            fPosX += m_pkPet->m_vecMotion[ m_pkPet->uiMotion ]->GetFrameData( m_pkPet->cFrame ).Plus_x * m_fZoom * fScale;
//            fPosY += m_pkPet->m_vecMotion[ m_pkPet->uiMotion ]->GetFrameData( m_pkPet->cFrame ).Pos_y * m_fZoom * fScale + m_pkPet->m_fDistY * m_fZoom * 2.5f;
//
//            fPosX = fPosX * cos( m_fAngleMove );
//
//            ::D3DXMatrixTranslation( &mat3, fPosX, fPosY, fPosZ );
//            matWorld = mat4 * mat1 * mat2 * mat3;
//            m_pkPet->m_pObject->SetWorldMat( &matWorld );
//
//            m_pkPet->m_pObject->SetFrame( &m_pkPet->m_vecMotion[ m_pkPet->uiMotion ]->GetFrameData( m_pkPet->cFrame ) );  
//
//            m_pkPet->cFrame++;
//            m_pkPet->cFrame %= m_pkPet->m_vecMotion[ m_pkPet->uiMotion ]->GetNum_Frame();
//            m_pkPet->m_pObject->SetShellMatrixScale( fScale * m_fZoom );
//
//            g_RenderManager->Render( false , m_pkPet->m_pObject );
//        }
//    }
//    //g_RenderManager->Render(false);
//    //else printf("ShopMotion이 NULL\n");
//
//    // 뷰포트 복구
//    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
//    m_pd3dDevice->SetViewport( &orgViewport );
//    m_pd3dDevice->SetTexture( 1, NULL );
//}
//
//void KGCCharViewWnd::SetChar(int iCharType)
//{
//    g_MyD3D->m_TempPlayer.AutoSetExtra();
//    g_MyD3D->m_TempPlayer.cFrame = 0;
//    m_fCharX = 0.0f;
//
//    m_iExtraCharNum = g_MyD3D->m_TempPlayer.Extra_Char_Num;
//
//    m_iMotionCount = 0;
//
//    if( m_pWaitMotion[m_iExtraCharNum] != NULL )
//        m_pShopMotion = m_pWaitMotion[m_iExtraCharNum];
//    //printf("케릭터가 %d으로 바꼈음 : FileName(%s)\n", m_iExtraCharNum, m_pShopMotion->GetDeviceID().c_str() );
//}
//
//void KGCCharViewWnd::Initialize()
//{
//    SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
//    //g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
//
//    KD3DWnd::Initialize();
//}
//
//void KGCCharViewWnd::UpdateMeshObject()
//{
//    g_MyD3D->m_TempPlayer.SetMyPlayer(-1);
//
//    g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
//    g_MyD3D->m_TempPlayer.m_pObject->SetShellMatrixScale( 0.25f );
//    // 펫 : 자기 정보창에서 표시해 주는 펫
//    // 펫 보는 창과 캐릭터 보는 창의 펫 정보를 모두 갱신한다
//    KPetInfo kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
//    int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
//    int iPetEvolutionLevel = (int)kPet.m_cPromotion;
//    int iPetLevel = kPet.m_iLevel;
//
//    if ( g_MyD3D->IsExistPetInfo( kPet.m_dwUID ) && iPetID != -1 )
//    {
//        if( iPetID != m_pkPet->m_iPetID ||
//            iPetEvolutionLevel != m_pkPet->m_iEvolutionLevel ||
//            iPetLevel != m_pkPet->m_iLevel )
//        {
//            m_pkPet->StartPet( -1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false );
//            m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
//            //m_pkPet->m_pObject->SetShellMatrixScale( 0.25f );  
//        }
//    }
//
//
//    if ( !m_pMeshKOGRoom )
//    {
//        m_pMeshKOGRoom = new GCObject();//g_RenderManager->CreateObject( EGCShaderTechnique::NOCARTOON_NOEDGE );
//#ifdef _DEBUG
//        m_pMeshKOGRoom->m_strDebugInfo = "룸";
//#endif
//        m_pMeshKOGRoom->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "MyRoom.p3m" ), g_pGCDeviceManager2->CreateTexture( "MyRoom.dds") );
//    } 
//}
//
//void KGCCharViewWnd::DeleteMeshObject()
//{
//    m_pkPet->EndPet();
//    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
//    if ( m_pMeshKOGRoom )
//    {
//        g_RenderManager->RemoveObject( m_pMeshKOGRoom );
//        SAFE_DELETE( m_pMeshKOGRoom );
//    }    
//}
//
//void KGCCharViewWnd::ActionPerformed( const KActionEvent& event )
//{
//    if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
//    {
//        g_KDSound.Play( "30" );
//        return;
//    }
//}
//
//void KGCCharViewWnd::ChangeCharWndZoom( bool _bZoomUp )
//{
//    POINT pt = g_pkInput->GetMousePos();
//    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
//
//    if( CheckPosInWindowBound(vMousePos) == true )
//    {
//        // 0.25~0.15
//        if( g_MyD3D->m_pStateMachine->GetState() == GS_MATCH )
//        {
//            if( _bZoomUp )
//                m_fZoom += DELTAZOOM;
//            else
//                m_fZoom -= DELTAZOOM;
//
//            if( m_fZoom > MAXZOOM )
//                m_fZoom = MAXZOOM;
//
//            if( m_fZoom < MINZOOM )
//                m_fZoom = MINZOOM;
//        }
//        else return;
//    }
//}
//
//void KGCCharViewWnd::ProcessCharWndAngle()
//{
//    POINT CurrentMousePoint = g_pkInput->GetMousePos();
//
//    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
//}
