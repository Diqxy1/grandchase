#include "stdafx.h"
#include "KGCMatchSceneUserInfo.h"
//
#include "../MyD3D.h"
#include "GCCharacterSelectBox.h"
#include "KGCHeroDungeonDlg.h"
#include "KGCHeroDungeonShop.h"
#include "KGCCountryGroup.h"
#include "KGCFaceLift.h"
//



//
//

//




#include "GCRenderManager.h"

IMPLEMENT_CLASSNAME( KGCMatchSceneUserInfo );
IMPLEMENT_WND_FACTORY( KGCMatchSceneUserInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchSceneUserInfo, "gc_match_scene_user_info" );

#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM 0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM 0.28f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM 0.18f

KGCMatchSceneUserInfo::KGCMatchSceneUserInfo( void )
: m_ucFrame(0)
, m_bKOGRoom(true)
, m_bRenderPet(true)
{
    m_iExtraCharNum     = 0;
    m_fAngleMove        = 0.0f;

    m_pMeshKOGRoom      = NULL;

    m_pShopMotion       = NULL;
    m_iMotionCount      = 0;

    m_pkCountryGroup    = NULL;
    LINK_CONTROL( "country_group",  m_pkCountryGroup );

    char strWndName[MAX_PATH] = {0,};
    for( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        m_pkName[i] = NULL;
        sprintf( strWndName, "name%d", i );
        LINK_CONTROL( strWndName, m_pkName[i] );
    }

    m_fZoom             = 0.23f;
    m_fCharX            = 0.0f;
    m_bAngleChanged     = false;
    m_pkPet             = NULL;

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
   
}

KGCMatchSceneUserInfo::~KGCMatchSceneUserInfo( void )
{
    SAFE_DELETE( m_pMeshKOGRoom );
    m_pShopMotion = NULL;
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
    if( m_pkPet )
        m_pkPet->EndPet();
    SAFE_DELETE( m_pkPet );
}

void KGCMatchSceneUserInfo::OnDestroy( void )
{
}

void KGCMatchSceneUserInfo::OnCreate( void )
{
    if( m_pkPet == NULL )
        m_pkPet = new KPet();

    ReloadMotion();
}

void KGCMatchSceneUserInfo::OnActivate( void )
{
    //Initialize에 구현합시다..
}

void KGCMatchSceneUserInfo::FrameMoveInEnabledState( void )
{
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

    UpdateTransMatrix();
    m_ucFrame++;

    if( m_pkPet->m_iPetID != -1 && true == m_bRenderPet )
    {
        m_pkPet->cFrame++;
        int iMaxFrame = m_pkPet->GetPetMotionFrm(  m_pkPet->usMotion  )->GetNum_Frame();
        if( iMaxFrame == 0 ) {
            iMaxFrame = 1;
        }

        m_pkPet->cFrame %= iMaxFrame;
    }

    // 움직이는 아이템을 위한 프레임무브
    if( g_MyD3D->m_TempPlayer.m_pObject )
    {
        g_MyD3D->m_TempPlayer.SetRender( true );
        g_MyD3D->m_TempPlayer.EgoItemFrameMove( m_ucFrame );
        g_MyD3D->m_TempPlayer.m_pObject->ChildFrameMove();
    }
}

void KGCMatchSceneUserInfo::PostDraw( void )
{    
    if( g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex == -1 )
        return;

    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

    D3DXMatrixScaling( &matScale ,m_fZoom * 4.0f, m_fZoom * 4.0f, m_fZoom * 4.0f );
    D3DXMatrixRotationY( &matRot, m_fAngleMove );
    D3DXMatrixTranslation( &matTrans, 0.0f, 0.071f - ( MAXZOOM - m_fZoom ) * 0.25f, 0.32f );   

    matWorld = matScale * matRot * matTrans;

    if( m_bKOGRoom && m_pMeshKOGRoom != NULL )
    {
        m_pMeshKOGRoom->SetWorldMat( &matWorld );
        g_RenderManager->Render( false , m_pMeshKOGRoom , EGCShaderTechnique::NOCARTOON_NOEDGE );
    }

    if( m_pShopMotion != NULL )
    {

        if( g_MyD3D->m_TempPlayer.Extra_Char_Num != m_iExtraCharNum  || !g_MyD3D->m_TempPlayer.m_pObject)
        {
            int iSelectChar = g_MyD3D->m_TempPlayer.Extra_Char_Num % GC_CHAR_NUM;
            g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( iSelectChar );
            SetChar( iSelectChar );
            UpdateMeshObject( m_bKOGRoom, m_bRenderPet );
        }

        if ( m_ucFrame > (int)m_pShopMotion->GetNum_Frame() - 1 )
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

            m_ucFrame = 0;
            UpdateTransMatrix();
           
        }

        D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
        D3DXMatrixRotationY( &matRot, m_fAngleMove );

        matWorld = matScale * matRot * m_matTrans;

		g_MyD3D->m_TempPlayer.m_pObject->SetWorldMat( &matWorld );
        g_MyD3D->m_TempPlayer.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_ucFrame ) );

        g_MyD3D->m_TempPlayer.SetRenderFaceLift( g_MyD3D->m_TempPlayer.GetDefaultFaceLiftType() );
        SiKGCFaceLift()->SetFaceLiftByMotionName( &g_MyD3D->m_TempPlayer, SiKGCFaceLift()->GetCurMotionName(),(int)m_ucFrame );

        g_RenderManager->Render( false , g_MyD3D->m_TempPlayer.m_pObject );

        // 여기 그려주자 펫또..=ㅅ=; 캐릭터 옆에 위치시켜 줘야 한다
        // 사이즈도 조금 작게 들어가야 하겠지..
        if( m_pkPet->m_iPetID != -1 && true == m_bRenderPet )
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

            m_pkPet->m_pObject->SetShellMatrixScale( fScale * m_fZoom );

            g_RenderManager->Render( false , m_pkPet->m_pObject );
        }

    }
    //g_RenderManager->Render(false);
    //else printf("ShopMotion이 NULL\n");

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );

    if( g_pkUIScene->m_pkHeroDungeonDlg->IsRenderItemPreview() )
        g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
}

void KGCMatchSceneUserInfo::SetChar(int iCharType)
{
    for( int i = 0; i < GC_CHAR_NUM; ++i )
    {
        if(m_pkName[i])
            m_pkName[i]->ToggleRender( i == iCharType );
    }
    g_MyD3D->m_TempPlayer.AutoSetExtra();
    m_ucFrame = 0;
    m_fCharX = 0.0f;

    int iBeforeExtraCharNum = m_iExtraCharNum;	
    m_iExtraCharNum = g_MyD3D->m_TempPlayer.Extra_Char_Num;

    m_iMotionCount = 0;

    if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH )
    {
        g_pkUIScene->m_pkCashShopScene->SetCharItem( (EGCCharType)((int)g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) );
    }
    else if( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
    {
        g_pkUIScene->m_pkGPShopScene->SetCharItem( (EGCCharType)((int)g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) );
    }

#if defined USE_COORDI_SHOP
    if( g_MyD3D->m_pStateMachine->GetState() == GS_COORDI_SHOP )
        if( g_pkUIScene->m_pkCoordiShopScene )
            g_pkUIScene->m_pkCoordiShopScene->SetChangeCharacter( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
#endif

    //  캐릭터가 바꼈을 경우
    if ( m_iExtraCharNum != iBeforeExtraCharNum ) 
    {
        ReloadMotion();
    }

    if( m_pWaitMotion != NULL )
        m_pShopMotion = m_pWaitMotion;
}

void KGCMatchSceneUserInfo::OnInitialize()
{
    SetChar( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
	SetProjectionMatrix( VIEWPORT_OFFSET_X, VIEWPORT_OFFSET_Y, GC_SCREEN_DIV_SIZE_INT(VIEWPORT_WIDTH), GC_SCREEN_DIV_SIZE_INT(VIEWPORT_HEIGHT) );

    if( m_pkCountryGroup != NULL )
        m_pkCountryGroup->SetCountryGroup(g_kGlobalValue.m_kUserInfo.strNation, true, STR_ID_PLAY_SAME_COUNTRY_MESSAGE0);
}

void KGCMatchSceneUserInfo::ResetUIforResolution()
{
	//View port 재 구성 해줌..
	SetProjectionMatrix( VIEWPORT_OFFSET_X, VIEWPORT_OFFSET_Y, GC_SCREEN_DIV_SIZE_INT(VIEWPORT_WIDTH), GC_SCREEN_DIV_SIZE_INT(VIEWPORT_HEIGHT) );
	KD3DWnd::ResetUIforResolution();
}

void KGCMatchSceneUserInfo::UpdateMeshObject( bool bRenderRoom/* = true*/, bool bRenderPet/* = true*/ )
{
	g_MyD3D->m_TempPlayer.SetMyPlayer(-1);	
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();
	g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
    if( g_MyD3D->m_TempPlayer.m_pObject )
	    g_MyD3D->m_TempPlayer.m_pObject->SetShellMatrixScale( 0.25f );   
    
    
    // 펫 : 자기 정보창에서 표시해 주는 펫
    // 펫 보는 창과 캐릭터 보는 창의 펫 정보를 모두 갱신한다
    KPetInfo kPet = g_MyD3D->m_TempPlayer.GetCurrentChar().kPetInfo;
    int iPetID = g_kGlobalValue.GetPetIDFromItemID( kPet.m_dwID );
    int iPetEvolutionLevel = (int)kPet.m_cPromotion;
    int iPetLevel = kPet.m_iLevel;

    m_bRenderPet = bRenderPet;
    if( true == m_bRenderPet )
    {
        if ( g_MyD3D->IsExistPetInfo( kPet.m_dwUID ) && iPetID != -1 )
        {
            if( iPetID != m_pkPet->m_iPetID ||
                iPetEvolutionLevel != m_pkPet->m_iEvolutionLevel ||
                iPetLevel != m_pkPet->m_iLevel )
            {
                m_pkPet->StartPet( -1, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::SHOP, L"", false, kPet.m_kCostume.m_dwID, true );
                m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
                //m_pkPet->m_pObject->SetShellMatrixScale( 0.25f );  
            }
        }
        else
        {
            m_pkPet->EndPet();
        }
    }

    m_bKOGRoom = bRenderRoom;
    if( m_bKOGRoom )
    {
        if ( !m_pMeshKOGRoom )
        {
            m_pMeshKOGRoom = new GCObject();//g_RenderManager->CreateObject( EGCShaderTechnique::NOCARTOON_NOEDGE );
    #ifdef _DEBUG
            m_pMeshKOGRoom->m_strName = "룸";
    #endif
            m_pMeshKOGRoom->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "MyRoom.p3m" ), g_pGCDeviceManager2->CreateTexture( "MyRoom.dds") );
        }
    }

	FrameMoveInEnabledState();
}

void KGCMatchSceneUserInfo::DeleteMeshObject()
{
    m_pkPet->EndPet();
    g_MyD3D->m_TempPlayer.ReleasePlayerResource();

    if( m_bKOGRoom )
    {
        if ( m_pMeshKOGRoom )
        {
            g_RenderManager->RemoveObject( m_pMeshKOGRoom );
            SAFE_DELETE( m_pMeshKOGRoom );
        }
    }
}

void KGCMatchSceneUserInfo::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        g_KDSound.Play( "30" );
        return;
    }

    if ( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK ) return;

}

void KGCMatchSceneUserInfo::ChangeCharWndZoom( bool _bZoomUp )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if( CheckPosInWindowBound( vMousePos ) == true )
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

void KGCMatchSceneUserInfo::ProcessCharWndAngle()
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCMatchSceneUserInfo::SetProjectionMatrix( int iOffsetX, int iOffsetY, int iWidth, int iHeight )
{
    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X = static_cast<DWORD>(( iOffsetX + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>(( iOffsetY + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>(iWidth * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Height = static_cast<DWORD>(iHeight * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( iWidth ) / float( iHeight ), 1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;
}

void KGCMatchSceneUserInfo::UpdateTransMatrix()
{
	if( m_pShopMotion == NULL )
		return;

    float fPosX = m_fCharX * CHAR_SCALE_RATIO;
    float fPosY = m_pShopMotion->GetFrameData(m_ucFrame).Pos_y * 0.1f - 0.06f;
    float fPosZ = 0.303f;
    m_fCharX += m_pShopMotion->GetFrameData(m_ucFrame).Plus_x;
    fPosX = fPosX * cos( m_fAngleMove );
    D3DXMatrixTranslation( &m_matTrans, fPosX, fPosY, fPosZ );
}

void KGCMatchSceneUserInfo::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );
}