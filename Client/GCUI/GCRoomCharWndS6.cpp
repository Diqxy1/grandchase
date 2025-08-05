#include "stdafx.h"
#include "GCRoomCharWndS6.h"
#include "KGCFaceLift.h"
#include "Message.h"

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


IMPLEMENT_CLASSNAME( KGCRoomCharWndS6 );
IMPLEMENT_WND_FACTORY( KGCRoomCharWndS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomCharWndS6, "gc_room_char_wnd_s6" );

KGCRoomCharWndS6::KGCRoomCharWndS6( void )
: m_pkImgRenderPos(NULL)
, m_pTexTitleItem( NULL ) 
, m_pkBack_Color( NULL )
{
    LINK_CONTROL( "render_position", m_pkImgRenderPos );
	LINK_CONTROL( "back_color", m_pkBack_Color );

    m_pMeshKOGRoom = NULL;
    m_pShopMotion = NULL;
    m_iMotionCount = 0;
    m_cFrame = 0;
    m_fCharX = 0.0f;

    m_fAngleMove = 0.0f;
    m_fZoom = 0.25;
    m_bAngleChanged = false;

    m_iExtraCharNum = 0;

    m_pkPet = NULL;

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
}

KGCRoomCharWndS6::~KGCRoomCharWndS6( void )
{
    SAFE_DELETE(m_pMeshKOGRoom);
    SAFE_RELEASE( m_pTexTitleItem );
    m_pShopMotion = NULL;

    if( m_pkPet != NULL )
        m_pkPet->EndPet();

    SAFE_DELETE( m_pkPet );
}

void KGCRoomCharWndS6::OnDestroy( void )
{
    SAFE_RELEASE( m_pTexTitleItem );
}

void KGCRoomCharWndS6::OnCreate( void )
{
    m_pkImgRenderPos->InitState(false);
    m_pkBack_Color->InitState(false);

    if( m_pkPet == NULL )
        m_pkPet = new KPet();

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;

    ReloadMotion();
}

void KGCRoomCharWndS6::PostChildDraw( void )
{
    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

    DWORD dwColor = 0xff142231;
    g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, dwColor, 1.0f, 0L );

    D3DXMatrixScaling( &matScale ,m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
    D3DXMatrixRotationY( &matRot, m_fAngleMove );
    D3DXMatrixTranslation( &matTrans, 0.0f, 0.081f - ( MAXZOOM - m_fZoom ) * 0.25f, 0.32f );   

    // 방 출력 부분 
    matWorld = matScale * matRot * matTrans;
    if(m_pMeshKOGRoom != NULL){
        m_pMeshKOGRoom->SetWorldMat( &matWorld );
        g_RenderManager->Render( false , m_pMeshKOGRoom, EGCShaderTechnique::NOCARTOON_NOEDGE);
    }

    if( m_pShopMotion != NULL )
    {
        std::string strMotionName = "";
        if ( m_cFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
        {
            m_iMotionCount++;
            if( m_iMotionCount == 5 )
            {
                if( m_pActionMotion )
                {
                    // 모션이 바뀌었음
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

        m_Player.m_pObject->SetWorldMat( &matWorld );
        m_Player.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_cFrame ) );
        m_Player.SetRenderFaceLift( m_Player.GetDefaultFaceLiftType() );
        SiKGCFaceLift()->SetFaceLiftByMotionName( &m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );
        g_RenderManager->Render( false , m_Player.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);
        m_cFrame++;

        // 여기 그려주자 펫또..=ㅅ=; 캐릭터 옆에 위치시켜 줘야 한다
        // 사이즈도 조금 작게 들어가야 하겠지..
        if( m_pkPet->m_iPetID != -1 )
        {
            float fScale = 2.5f * m_pkPet->m_fScale;
            D3DXMATRIX mat1, mat2, mat3, mat4, matWorld;

            // 상하
            ::D3DXMatrixTranslation( &mat4, -0.3f / fScale, -0.57f / fScale, -0.2f / fScale);
            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
            ::D3DXMatrixRotationY( &mat2, m_fAngleMove );

            if ( m_pkPet->m_iPetID == EGCPETID::PET_KELBY )
                m_pkPet->usMotion = EGCPETMOTION::SHOP_ACTION;
            else
                m_pkPet->usMotion = EGCPETMOTION::WAIT;

            float fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.303f;

            fPosX += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Plus_x * m_fZoom * fScale;
            fPosY += m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ).Pos_y * m_fZoom * fScale + m_pkPet->m_fDistY * m_fZoom * 2.5f;

            fPosX = fPosX * cos( m_fAngleMove );

            ::D3DXMatrixTranslation( &mat3, fPosX, fPosY, fPosZ );
            matWorld = mat4 * mat1 * mat2 * mat3;
            m_pkPet->m_pObject->SetWorldMat( &matWorld );

            m_pkPet->m_pObject->SetFrame( &m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetFrameData( m_pkPet->cFrame ) );  

            m_pkPet->cFrame++;
            m_pkPet->cFrame %= m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame();
            m_pkPet->m_pObject->SetShellMatrixScale( fScale * m_fZoom );

            g_RenderManager->Render( false , m_pkPet->m_pObject );
        }
    }

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );

    if( m_pTexTitleItem )
    {
        D3DXVECTOR2 vPos = GetRelocatedWindowPos() + m_pkBack_Color->GetFixedWindowLocalPos();
        g_pItemMgr->Draw_ShopImage(vPos,static_cast<float>(m_pkBack_Color->GetWidth()/3) * m_fWindowScaleX, static_cast<float>(m_pkBack_Color->GetWidth()/3) * m_fWindowScaleY, m_pTexTitleItem, false );
    }

}

void KGCRoomCharWndS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCRoomCharWndS6::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    bool bCheckBound = this->CheckPosInWindowBoundWithChild(vMousePos, false);

    if( !m_bAngleChanged )
    {
        m_fAngleMove += 0.005f;
        if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
    }

    if( g_pkInput->ISWheelUp() && bCheckBound )
        ChangeCharWndZoom(true);
    else if( g_pkInput->ISWheelDown() && bCheckBound )
        ChangeCharWndZoom(false);

    static bool Internal = false;

    if( g_pkInput->BtnDown() == true &&  bCheckBound )
    {
        m_bAngleChanged = true;
        m_ptStartMouse = g_pkInput->GetMousePos();
        m_fStartAngle = m_fAngleMove;
        Internal = true;
    }
    else if ( g_pkInput->BtnDown() == true && !bCheckBound ) 
    {
        Internal = false;
    }

    if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE && bCheckBound )
    {
        m_bAngleChanged = false;
    }

    if( g_pkInput->BtnPressed() && m_bAngleChanged == true && Internal == true )
        ProcessCharWndAngle();

    if( m_Player.m_pObject )
    {
        m_Player.SetRender( true );
        m_Player.EgoItemFrameMove( UCHAR(m_cFrame) );
        m_Player.m_pObject->ChildFrameMove();
    }
}

void KGCRoomCharWndS6::SetPreviewWindow( PLAYER* pPlayer )
{
    D3DXVECTOR2 vPos( GetFixedWindowPos() );

    int nWidthX = m_pkImgRenderPos->GetWidth();
    int nHeightX = m_pkImgRenderPos->GetHeight();        
    D3DXVECTOR2 vtPos = m_pkImgRenderPos->GetCurrentWindowLocalPos();

    m_viewport.X = static_cast<DWORD>(( static_cast<int>(vtPos.x) + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>(( static_cast<int>(vtPos.y) + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>( nWidthX * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Height = static_cast<DWORD>( nHeightX * g_pGCDeviceManager2->GetWindowScaleY() );

    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;


    m_Player = *pPlayer;
	
	g_bCheckTuxedoWeapon = true;
    SetChar();
    UpdateMeshObject();
    UpdatePet();
}

void KGCRoomCharWndS6::SetChar()
{
    m_cFrame = 0;
    m_fCharX = 0.0f;
    m_iMotionCount = 0;


}

void KGCRoomCharWndS6::UpdateMeshObject( void )
{
    m_Player.SetMyPlayer(-1, true);	
    m_Player.ReleasePlayerResource();

    m_Player.AutoSetExtra();

    int iBeforeExtraCharNum = m_iExtraCharNum;
    m_iExtraCharNum = m_Player.Extra_Char_Num;

    //  캐릭터가 바꼈을 경우
    if ( m_iExtraCharNum != iBeforeExtraCharNum ) 
    {
        ReloadMotion();
    }

    if( m_pWaitMotion != NULL )
        m_pShopMotion = m_pWaitMotion;


    m_Player.m_iPlayerIndex = 0;
    m_Player.m_kUserInfo.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;

    if (g_pkUIScene->IsRenderOn(KGCUIScene::GC_MBOX_ROOM_USER_INFO_POPUP))
    {
        m_Player.UpdatePlayerResourceClean(true);
    }
    else {
        m_Player.UpdatePlayerResource(true);
    }

    switch( m_iExtraCharNum % GC_CHAR_NUM )
    {
    case GC_CHAR_DIO:
        m_Player.ToggleExtraMesh( "WEAPON", true );
        break;
    case GC_CHAR_ZERO:
        m_Player.ToggleExtraMesh( "GRANDARK_2ND", false );
        m_Player.ToggleExtraMesh( "GRANDARK_2ND_1", false );
        m_Player.ToggleExtraMesh( "GRANDARK_2ND_2", false );
        m_Player.ToggleExtraMesh( "DOG_GRANDARK", false );
        break;
    case GC_CHAR_LUPUS:
        m_Player.ToggleExtraMesh( "WEAPON_0", true );
        m_Player.ToggleExtraMesh( "WEAPON_1", true );
        break;
    default:
        m_Player.ToggleExtraMesh( "WEAPON", m_Player.GetCurFormTemplate().WeaponRenderBasically );
        m_Player.ToggleExtraMesh( "MERGE_WEAPON", m_Player.GetCurFormTemplate().LeftWeaponRenderBasically );
        break;
    }

    if( m_Player.m_pObject )
        m_Player.m_pObject->SetShellMatrixScale( 0.25f );  

    if ( !m_pMeshKOGRoom )
    {
        m_pMeshKOGRoom = new GCObject();
    }

    int iTitleITemID = g_pItemMgr->GetEquipTitleItem( &m_Player.GetCurrentChar() );
    SetTitleItemID( iTitleITemID );
}

void KGCRoomCharWndS6::DeleteMeshObject( void )
{
    m_pkPet->EndPet();
    m_Player.ReleasePlayerResource();
    if ( m_pMeshKOGRoom )
    {
        g_RenderManager->RemoveObject( m_pMeshKOGRoom );
        SAFE_DELETE( m_pMeshKOGRoom );
    }
}

void KGCRoomCharWndS6::ChangeCharWndZoom( bool bZoom )
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

void KGCRoomCharWndS6::ProcessCharWndAngle( void )
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;

}

void KGCRoomCharWndS6::UpdatePet()
{
    // 펫 : 자기 정보창에서 표시해 주는 펫
    // 펫 보는 창과 캐릭터 보는 창의 펫 정보를 모두 갱신한다
    KPetInfo kPet = m_Player.GetCurrentChar().kPetInfo;
    int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
    int iPetEvolutionLevel = (int)kPet.m_cPromotion;
    int iPetLevel = kPet.m_iLevel;
    DWORD dwPetCostumeID = kPet.m_kCostume.m_dwID;

    // 2008/01/30 Jaeho 이부분이 없어서
    // 1. 펫있는 캐릭터 선택상태 2. 펫 없는 캐릭터 변경
    // 의 경우. 1번 캐릭터가 가지고 있떤 펫이 사라지지 않고 유지됨...
    // breadceo. 밑에 적용되어있음.. 동시에 작업했나보네
    //if( m_pkPet )
    //    m_pkPet->EndPet();

    // 장착하고 있는 펫이 있다
    if( kPet.m_dwUID != 0 && iPetID != -1 )
    {
        if( iPetID != m_pkPet->m_iPetID ||
            iPetEvolutionLevel != m_pkPet->m_iEvolutionLevel ||
            iPetLevel != m_pkPet->m_iLevel ||
            dwPetCostumeID != m_pkPet->GetCostumeID() )
        {
            m_pkPet->StartPet( -1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, kPet.m_kCostume.m_dwID, true );
            m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
        }
    }
    // 장착하고 있는 펫이 없다
    else
    {
        m_pkPet->EndPet();
    }
}

void KGCRoomCharWndS6::SetTitleItemID( int iTitleItemID )
{
    SAFE_RELEASE( m_pTexTitleItem );

    if( -1 == iTitleItemID )
        return;

    GCItem *pItem = g_pItemMgr->GetItemData( iTitleItemID );
    if( NULL == pItem )
        return;

    if( GIK_TITLE_ITEM == pItem->eItemKind
        || g_pItemMgr->IsRallyTitleItemID( pItem->dwGoodsID ) )
    {
        m_pTexTitleItem = g_pItemMgr->CreateItemTexture( iTitleItemID, 0 );
        if( 0 == m_pTexTitleItem->GetDeviceTexture() )
            SAFE_RELEASE( m_pTexTitleItem );    
    }
}

void KGCRoomCharWndS6::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );
}