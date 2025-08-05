#include "stdafx.h"
#include "GCHeroItemPreview.h"
#include "KGCFaceLift.h"

IMPLEMENT_CLASSNAME( KGCHeroItemPreview );
IMPLEMENT_WND_FACTORY( KGCHeroItemPreview );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroItemPreview, "gc_hero_item_preview" );

KGCHeroItemPreview::KGCHeroItemPreview( void )
: m_pkImgRenderPos(NULL)
{
    for(int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        m_apkCharacterName[i] = NULL;
        m_apkCharacterFace[i] = NULL;

        char szBuf[MAX_PATH];
        sprintf_s(szBuf, MAX_PATH - 1, "character_name%d", i);
        LINK_CONTROL( szBuf, m_apkCharacterName[i] );

        sprintf_s(szBuf, MAX_PATH - 1, "character_face%d", i);
        LINK_CONTROL( szBuf, m_apkCharacterFace[i] );
    }

    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_close_preview", m_pkBtnClose );
    LINK_CONTROL( "render_position", m_pkImgRenderPos );
    m_bClose = true;

	m_pkHeroItemPrevInfo = NULL;
    LINK_CONTROL( "hero_item_prev_info", m_pkHeroItemPrevInfo );

    m_iCurrentCharacterName = -1;
    
    m_vecItem.clear();

    m_pMeshKOGRoom = NULL;
    m_pShopMotion = NULL;
    m_iMotionCount = 0;
    m_cFrame = 0;
    m_fCharX = 0.0f;
    
    m_fAngleMove = 0.0f;
    m_fZoom = 0.25;
    m_bAngleChanged = false;

    m_iExtraCharNum = 0;

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;

}

KGCHeroItemPreview::~KGCHeroItemPreview( void )
{
    SAFE_DELETE(m_pMeshKOGRoom);
    m_pShopMotion = NULL;
}

void KGCHeroItemPreview::OnCreate( void )
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
		if ( m_apkCharacterName[i] )
		{
			m_apkCharacterName[i]->InitState(false, false, this);
        }
        if ( m_apkCharacterFace[i] )
        {
            m_apkCharacterFace[i]->InitState(false, false, this);
        }
	}

    m_pkBtnClose->InitState(true, true, this);
    if( m_pkImgRenderPos )
        m_pkImgRenderPos->InitState(false);

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;
 
    m_pkHeroItemPrevInfo->InitState(true, true, this);

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    ReloadMotion();
}

void KGCHeroItemPreview::PostChildDraw( void )
{
    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
    g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0L );

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
    }

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCHeroItemPreview::ActionPerformed( const KActionEvent& event )
{
    if ( m_pkImgRenderPos ) {
        if( event.m_pWnd == m_pkBtnClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )         //가차 미리보기는 m_pkImgRenderPos 가 있다. 단지 아래쪽에 영웅상점으로 빠지기 않기 위한 코드
        {
            this->ToggleRender(false);

        }
    }else if( event.m_pWnd == m_pkBtnClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )   
    {
        m_bClose = true;        
        if( g_pkUIScene->m_pkHeroDungeonDlg->IsRenderOn() )
            g_pkUIScene->m_pkHeroDungeonDlg->ReleaseLock();
        else
            this->ToggleRender(false);
    }
}

void KGCHeroItemPreview::FrameMoveInEnabledState( void )
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

    if( g_pkInput->BtnDown() == true &&  bCheckBound )
    {
        m_bAngleChanged = true;
        m_ptStartMouse = g_pkInput->GetMousePos();
        m_fStartAngle = m_fAngleMove;
    }

    if( g_pkInput->BtnDown( KInput::MBRIGHT ) == TRUE && bCheckBound )
    {
        m_bAngleChanged = false;
    }

    if( g_pkInput->BtnPressed() && m_bAngleChanged == true )
        ProcessCharWndAngle();

    if( m_Player.m_pObject )
    {
        m_Player.SetRender( true );
        m_Player.EgoItemFrameMove( UCHAR(m_cFrame) );
        m_Player.m_pObject->ChildFrameMove();
    }
}

void KGCHeroItemPreview::SetPreviewWindow(int iCharacterType, std::vector<GCItem*>& vecItem_)
{
    m_vecItem = vecItem_;

    if( !m_vecItem.empty() )
    {
        m_pkHeroItemPrevInfo->SetItemInformation(m_vecItem[0]); //영웅던전에 프리뷰 보여줄 때는 아이템 한개만 장착시키고 아이템 정보도 같이 뛰워준다.
       // m_pkHeroItemPrevInfo->SetAllWindowAlpha(255);
    }
    D3DXVECTOR2 vPos( GetFixedWindowPos() );

    if ( m_pkImgRenderPos ) {
        int nWidthX = m_pkImgRenderPos->GetWidth();
        int nHeightX = m_pkImgRenderPos->GetHeight();        
        D3DXVECTOR2 vtPos = m_pkImgRenderPos->GetCurrentWindowLocalPos();

        m_viewport.X = static_cast<DWORD>(( static_cast<int>(vtPos.x) + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
        m_viewport.Y = static_cast<DWORD>(( static_cast<int>(vtPos.y) + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
        m_viewport.Width = static_cast<DWORD>( nWidthX * g_pGCDeviceManager2->GetWindowScaleX() );
        m_viewport.Height = static_cast<DWORD>( nHeightX * g_pGCDeviceManager2->GetWindowScaleY() );
    }else
    {
		float fAddtionValueX = GC_SCREEN_DIV_WIDTH;
		float fAddtionValueY = GC_SCREEN_DIV_HEIGHT;
//#if defined(NATION_IDN)
//		fAddtionValueX = 1.0f;
//		fAddtionValueY = 1.0f;
//#endif
        m_viewport.X = static_cast<DWORD>(( VIEWPORT_OFFSET_X * fAddtionValueX + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
        m_viewport.Y = static_cast<DWORD>(( VIEWPORT_OFFSET_Y * fAddtionValueY + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
        m_viewport.Width = static_cast<DWORD>( VIEWPORT_WIDTH * fAddtionValueX * g_pGCDeviceManager2->GetWindowScaleX() );
        m_viewport.Height = static_cast<DWORD>( VIEWPORT_HEIGHT * fAddtionValueY * g_pGCDeviceManager2->GetWindowScaleY() );
    }
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    SetCharacterName(iCharacterType);
    SetChar(iCharacterType);
    UpdateMeshObject();
}

void KGCHeroItemPreview::SetCharacterName( int iCharacterType )
{
    if( m_iCurrentCharacterName < 0 )
    {
        m_iCurrentCharacterName = iCharacterType;
        if ( m_apkCharacterName[m_iCurrentCharacterName] )
            m_apkCharacterName[m_iCurrentCharacterName]->ToggleRender(true);
        if ( m_apkCharacterFace[m_iCurrentCharacterName] )
            m_apkCharacterFace[m_iCurrentCharacterName]->ToggleRender(true);
    }
    else
    {

        if ( m_apkCharacterName[m_iCurrentCharacterName] )
            m_apkCharacterName[m_iCurrentCharacterName]->ToggleRender(false);
        if ( m_apkCharacterFace[m_iCurrentCharacterName] )
            m_apkCharacterFace[m_iCurrentCharacterName]->ToggleRender(false);
        m_iCurrentCharacterName = iCharacterType;
        if ( m_apkCharacterName[m_iCurrentCharacterName] )
            m_apkCharacterName[m_iCurrentCharacterName]->ToggleRender(true);
        if ( m_apkCharacterFace[m_iCurrentCharacterName] )
        m_apkCharacterFace[m_iCurrentCharacterName]->ToggleRender(true);
    }
}

void KGCHeroItemPreview::SetChar( int iCharType )
{
    InitCharacterInfo();
    m_Player.m_kUserInfo.SetCurrentChar(iCharType);



    m_cFrame = 0;
    m_fCharX = 0.0f;
    m_iMotionCount = 0;

}

void KGCHeroItemPreview::UpdateMeshObject( void )
{
    m_Player.SetMyPlayer(-1, true);	
    m_Player.ReleasePlayerResource();

    for( int i = 0; i < static_cast<int>(m_vecItem.size()); i++ )
    {
        if( m_vecItem[i] != NULL )
            g_pItemMgr->EquipItem(&(m_Player.m_kUserInfo.GetCurrentChar()), m_vecItem[i] );
    }

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

    m_Player.UpdatePlayerResource(true);

    switch( m_iExtraCharNum % GC_CHAR_NUM )
    {
    case GC_CHAR_DIO:
        m_Player.ToggleExtraMesh( "WEAPON", true );
        m_Player.ToggleExtraMesh( "MERGE_WEAPON", false );
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
#ifdef _DEBUG
        m_pMeshKOGRoom->m_strName = "룸";
#endif
        m_pMeshKOGRoom->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "MyRoom.p3m" ), g_pGCDeviceManager2->CreateTexture( "MyRoom.dds") );
    }

}

void KGCHeroItemPreview::DeleteMeshObject( void )
{
    m_Player.ReleasePlayerResource();
    if ( m_pMeshKOGRoom )
    {
        g_RenderManager->RemoveObject( m_pMeshKOGRoom );
        SAFE_DELETE( m_pMeshKOGRoom );
    }
}

void KGCHeroItemPreview::InitCharacterInfo()
{
    std::map<int, std::set < char > > mapCharInfo = g_kGlobalValue.GetOpenedCharacterInfo();
    std::map<int, std::set < char > >::iterator mit = mapCharInfo.begin();

    m_Player.m_kUserInfo.Init();

    for( ; mit != mapCharInfo.end(); ++mit )
    {
        SCharInfo charInfo;
        charInfo.iCharType = mit->first;

		if( g_kGlobalValue.IsQuickSkillPlayer( charInfo.iCharType ) ||
            g_kGlobalValue.IsHybridPlayer( charInfo.iCharType ) ) {
            charInfo.iPromotionLevel = g_kGlobalValue.m_kUserInfo.GetCurrentChar( charInfo.iCharType ).GetMaxPromotionNum();
			charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( charInfo.iCharType ).setPromotion;
		} else {
            charInfo.iPromotionLevel = mit->second.size() - 1;
			charInfo.setPromotion = mit->second;
		}

        m_Player.m_kUserInfo.AddChar(charInfo);
    }
}

void KGCHeroItemPreview::ChangeCharWndZoom( bool bZoom )
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

void KGCHeroItemPreview::ProcessCharWndAngle( void )
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;

}


void KGCHeroItemPreview::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );
}