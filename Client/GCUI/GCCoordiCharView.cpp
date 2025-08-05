#include "stdafx.h"

#ifdef USE_COORDI_SHOP // Kawan>

#include "GCCoordiCharView.h"
#include "GCUI\GCCoordiShopScene.h"
#include "KGCFaceLift.h"

#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM   0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM     0.25f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM     0.25f

#ifdef DEFAULTZOOM
#undef DEFAULTZOOM
#endif
#define DEFAULTZOOM MAXZOOM


IMPLEMENT_CLASSNAME( KGCCoordiCharView );
IMPLEMENT_WND_FACTORY( KGCCoordiCharView );
IMPLEMENT_WND_FACTORY_NAME( KGCCoordiCharView, "gc_coordi_char_view" );

KGCCoordiCharView::KGCCoordiCharView( void )
{
    m_pkBtnDisrobe      = NULL;
    m_pkBtnDress        = NULL;

    LINK_CONTROL( "btn_disrobe", m_pkBtnDisrobe );
    LINK_CONTROL( "btn_dress", m_pkBtnDress );

    m_Player = NULL;

    m_pMeshFootStage = NULL;
    m_pShopMotion = NULL;
    m_pkBackTexture = NULL;
    m_iMotionCount = 0;
    m_cFrame = 0;
    m_fCharX = 0.0f;

    m_fAngleMove = 0.0f;
    m_fStageAngleMove = 0.0f;
    m_fZoom = 0.25;
    m_bAngleChanged = false;

    m_iExtraCharNum = 0;
    m_iCurrentCharacter = 0; 
    m_bShowDefault = false;

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
}

KGCCoordiCharView::~KGCCoordiCharView( void )
{
}

void KGCCoordiCharView::OnCreate( void )
{
    m_pkBtnDisrobe->InitState( true, true, this );
    m_pkBtnDress->InitState( true, true, this );


    D3DXMATRIX matLook, matScreen, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
	::D3DXMatrixOrthoOffCenterLH(&matScreen, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0, 0.f, 100.f);
	::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;
	m_matScreenProj = matScreen;

    // 배경 로드 
    m_pkBackTexture = g_pGCDeviceManager2->CreateTexture( "showroom.dds" );

    ReloadMotion();
}

void KGCCoordiCharView::OnDestroy( void )
{
    DeleteMeshObject();
    SAFE_RELEASE( m_pkBackTexture );
}

void KGCCoordiCharView::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnDisrobe,  KD3DWnd::D3DWE_BUTTON_CLICK,    OnDisrobe );
    GCWND_MSG_MAP( m_pkBtnDress,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnDress );
}

void KGCCoordiCharView::PostChildDraw( void )
{
    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

	D3DXMATRIX matRot, matTrans, matScale, matWorld;
	g_pd3dDevice->GetViewport(&orgViewport);
	g_pd3dDevice->SetViewport(&m_viewport);

    // 배경 출력
    g_pGCDeviceManager2->DrawInScreen(m_pkBackTexture,
        541.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(),
        180.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY(),
        790.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleX(),
        519.0f * GC_SCREEN_DIV_WIDTH * g_pGCDeviceManager2->GetWindowScaleY(),
        0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 0xffffffff);

    // 발판 출력  
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    float fScale = 4.3f;
    float fFootStageY = -0.15f;
    float fFootStageZ = 0.38f;
    float fFootStageMul = 0.44f;
    float fAngle = 0.9f;

    //LUA_GET_VALUE_DEF( "footstageY", fFootStageY, 0.001f );
    //LUA_GET_VALUE_DEF( "footstageZ", fFootStageZ, 0.001f );
    //LUA_GET_VALUE_DEF( "footStageMul", fFootStageMul, 0.27f );
    //LUA_GET_VALUE_DEF( "footStageScale", fScale, 3.0f );
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

    D3DXMatrixScaling( &matScale ,m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
    D3DXMatrixRotationY( &matRot, m_fStageAngleMove );
    D3DXMatrixTranslation( &matTrans, 0.0f, fFootStageY + ( MAXZOOM - m_fZoom ) * fFootStageMul, fFootStageZ );   

    matWorld = matScale * matRot * matTrans;
    if( m_pMeshFootStage != NULL )
    {
        m_pMeshFootStage->SetWorldMat( &matWorld );
        g_RenderManager->Render( false, m_pMeshFootStage, EGCShaderTechnique::CARTOON_NOEDGE );
    }

    if( m_pShopMotion != NULL )
    {
        if( g_MyD3D->m_TempPlayer.Extra_Char_Num != m_iExtraCharNum || !g_MyD3D->m_TempPlayer.m_pObject )
        {
            int iSelectChar = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
            g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( iSelectChar );
            SetChangeCharacter( iSelectChar );
            SetCoordiView(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
			if (g_MyD3D->m_TempPlayer.m_pObject != NULL)
			{
				g_RenderManager->RemoveObject( g_MyD3D->m_TempPlayer.m_pObject );
				SAFE_DELETE( g_MyD3D->m_TempPlayer.m_pObject );
			}
            g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
            ResetPlayer();            
        }

        std::string strMotionName = "";
        if ( m_cFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
        {
            m_iMotionCount++;
            if( m_iMotionCount == 5 )
            {
                if( m_pActionMotion )
                {
                    m_pShopMotion = m_pActionMotion;
                    m_iMotionCount = 0;
                    m_fCharX = 0.0f;// X를 초기화

                    SiKGCFaceLift()->SetCurMotionName( m_pActionMotion->GetDeviceID() );
                }
            }
            else
            {
                if( m_pWaitMotion )
                {
                    // 모션이 바뀌었음
                    m_pShopMotion = m_pWaitMotion;
                    m_fCharX = 0.0f;// X를 초기화

                    SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion->GetDeviceID() );
                }
            }

            m_cFrame = 0;
        }

        float fPosX = m_fCharX * CHAR_SCALE_RATIO;
        float fPosY = m_pShopMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.06f;
        float fPosZ = 0.303f;

        m_fCharX += m_pShopMotion->GetFrameData(m_cFrame).Plus_x;

        fPosX = fPosX * cos( m_fAngleMove );

        D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
        D3DXMatrixRotationY( &matRot, m_fAngleMove );
        D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

        matWorld = matScale * matRot * matTrans;

        m_Player->m_pObject->SetWorldMat( &matWorld );
        m_Player->m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_cFrame ) );

        m_Player->SetRenderFaceLift( m_Player->GetDefaultFaceLiftType() );
        SiKGCFaceLift()->SetFaceLiftByMotionName( m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );

        g_RenderManager->Render( false ,m_Player->m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);
        m_cFrame++;
    }

    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCCoordiCharView::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    bool bCheckBound = this->CheckPosInWindowBoundWithChild(vMousePos, true);

    if( !m_bAngleChanged )
    {
        m_fAngleMove += 0.005f;
        m_fStageAngleMove += 0.0035f;
        if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
        if ( m_fStageAngleMove >= 2 * D3DX_PI ) m_fStageAngleMove = 0.0f;
    }

    if( g_pkInput->ISWheelUp() && bCheckBound )
        ChangeCharWndZoom(true);
    else if( g_pkInput->ISWheelDown() && bCheckBound )
        ChangeCharWndZoom(false);

    static bool bInternal = false;

    if( g_pkInput->BtnDown() == true &&  bCheckBound )
    {
        m_bAngleChanged = true;
        m_ptStartMouse = g_pkInput->GetMousePos();
        m_fStartAngle = m_fAngleMove;
        m_fStartStageAngle = m_fStageAngleMove;
        bInternal = true;
    }
    else if( g_pkInput->BtnDown() == true &&  !bCheckBound )
        bInternal = false;

    if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE  )
    {
        m_bAngleChanged = false;
    }

    if( g_pkInput->BtnPressed() && m_bAngleChanged == true && bInternal == true )
        ProcessCharWndAngle();

    if( m_Player->m_pObject )
    {
        m_Player->SetRender( true );
        m_Player->EgoItemFrameMove( UCHAR(m_cFrame) );
        m_Player->m_pObject->ChildFrameMove();
    }
}

void KGCCoordiCharView::ChangeCharWndZoom( bool bZoom )
{
    if( bZoom )
        m_fZoom += DELTAZOOM;
    else
        m_fZoom -= DELTAZOOM;

    if( m_fZoom > MAXZOOM )
        m_fZoom = MAXZOOM;

    if( m_fZoom < MINZOOM )
        m_fZoom = MINZOOM;
}

void KGCCoordiCharView::ProcessCharWndAngle( void )
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
    m_fStageAngleMove = m_fStartStageAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCCoordiCharView::SetCoordiView( int iCharType_ )
{
    m_iCurrentCharacter = iCharType_;
    m_Player = &(g_MyD3D->m_TempPlayer);
    m_Player->m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    m_Player->m_kUserInfo.SetCurrentChar(m_iCurrentCharacter);
    m_Player->AutoSetExtra();

    m_cFrame = 0;
    m_fCharX = 0.0f;
    m_iMotionCount = 0;

    int iBeforeExtraCharNum = m_iExtraCharNum;
    m_iExtraCharNum = m_Player->Extra_Char_Num;

    //  캐릭터가 바꼈을 경우
    if ( m_iExtraCharNum != iBeforeExtraCharNum ) 
    {
        ReloadMotion();
    }

    if( m_pWaitMotion != NULL )
        m_pShopMotion = m_pWaitMotion;

    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X = static_cast<DWORD>(( VIEWPORT_OFFSET_X + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>(( VIEWPORT_OFFSET_Y + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_WIDTH) * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Height = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_HEIGHT) * g_pGCDeviceManager2->GetWindowScaleY() );
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    UpdateMeshObject();
}

void KGCCoordiCharView::UpdateMeshObject( void )
{
    m_Player->SetMyPlayer(-1, true);
    m_Player->ReleasePlayerResource();

    if( m_Player->m_pObject )
        m_Player->m_pObject->SetShellMatrixScale( 0.25f ); 

    m_Player->UpdatePlayerResource(true);

    if( !m_pMeshFootStage )
    {
        m_pMeshFootStage = new GCObject();
#ifdef _DEBUG
        m_pMeshFootStage->m_strName = "코디샵 발판";
#endif
        m_pMeshFootStage->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "footstage.p3m" ), g_pGCDeviceManager2->CreateTexture( "footstage.dds" ) );
    }
}

void KGCCoordiCharView::DeleteMeshObject( void )
{
    m_Player->ReleasePlayerResource();
    if( m_pMeshFootStage )
    {
        g_RenderManager->RemoveObject( m_pMeshFootStage );
        SAFE_DELETE( m_pMeshFootStage );
    }
}

void KGCCoordiCharView::SetChangeCharacter( int iCharacter_ )
{
    m_iCurrentCharacter = iCharacter_;
    SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );
}

void KGCCoordiCharView::OnDisrobe( void )
{
    // 무기는 착용하고 나머지는 해제 
    SCharInfo CharInfo = m_Player->GetCurrentChar();

    for( std::vector<SUserItem>::iterator it = CharInfo.vecItemInfo.begin() ; it != CharInfo.vecItemInfo.end() ; ++ it )
    {
        GCItem* pItem = g_pItemMgr->GetItemData( it->iItemID );
        
        if( pItem->dwSlotPosition & ESP_WEAPON )
            continue;

        g_pItemMgr->UnequipItemForItemUID( &(m_Player->GetCurrentChar()), it->dwItemUID );
    }

    for( std::vector<SUserItem>::iterator it = CharInfo.vecLookInfo.begin() ; it != CharInfo.vecLookInfo.end() ; ++ it )
        g_pItemMgr->UnequipItemForItemUID( &(m_Player->GetCurrentChar()), it->dwItemUID );

    m_bShowDefault = !m_bShowDefault;
    if( g_pkUIScene->m_pkCoordiShopScene )
    {
        g_pkUIScene->m_pkCoordiShopScene->ClearEquipItemList();
        g_pkUIScene->m_pkCoordiShopScene->UpdateItemBox();
    }
    

    UpdateMeshObject();
}

void KGCCoordiCharView::OnDress( void )
{
    // 원래 장비대로 
    m_Player = &(g_MyD3D->m_TempPlayer);
    m_Player->m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    //m_Player->SetMyPlayer( m_iCurrentCharacter );
    UpdateMeshObject();
    m_bShowDefault = false;
    if( g_pkUIScene->m_pkCoordiShopScene )
    {
        g_pkUIScene->m_pkCoordiShopScene->ClearEquipItemList();
        g_pkUIScene->m_pkCoordiShopScene->UpdateItemBox();

    }
    
}

SCharInfo KGCCoordiCharView::GetCurrentCharInfo( void )
{
    return m_Player->GetCurrentChar();
}

void KGCCoordiCharView::ResetUIforResolution( void )
{
    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X = static_cast<DWORD>(( VIEWPORT_OFFSET_X + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>(( VIEWPORT_OFFSET_Y + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_WIDTH) * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Height = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_HEIGHT) * g_pGCDeviceManager2->GetWindowScaleY() );
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    KD3DWnd::ResetUIforResolution();
}

void KGCCoordiCharView::ResetPlayer( void )
{
    // 플레이어 정보 새로 받기 
    m_Player = &(g_MyD3D->m_TempPlayer);
    m_Player->m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    //m_Player->SetMyPlayer( m_iCurrentCharacter );
    UpdateMeshObject();
}

void KGCCoordiCharView::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );
}

#endif