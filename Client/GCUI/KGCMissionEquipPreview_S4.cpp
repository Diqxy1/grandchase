#include "stdafx.h"
#include "KGCMissionEquipPreview_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionEquipPreview_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionEquipPreview_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionEquipPreview_S4, "gc_mission_equippreview_s4" );

KGCMissionEquipPreview_S4::KGCMissionEquipPreview_S4( void )
: m_pkBtn_Close( NULL )
, m_pkBackGround( NULL )
, m_pkAlphaFrame( NULL )
, m_pMotion( NULL )
, m_fZoom( 0.25f )
, m_cFrame( 0 )
, m_iCharIdx( 0 )
, m_iExtraIdx( 0 )
, m_fCharX( 0.0f )
, m_iMotionCount( 0 )
, m_fAngleMove( 0.0f )
, m_bAngleChanged( false )
{
    LINK_CONTROL( "btn_close", m_pkBtn_Close );
    LINK_CONTROL( "background", m_pkBackGround );
    LINK_CONTROL( "alpha_frame", m_pkAlphaFrame );

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
}

KGCMissionEquipPreview_S4::~KGCMissionEquipPreview_S4( void )
{
    m_kPlayer.ReleasePlayerResource();
}

bool KGCMissionEquipPreview_S4::InitDialog( IN const KDialogInfo& kInfo_ )
{
    if( m_pkBtn_Close )
    {
        m_pkBtn_Close->SetHotKey(DIK_ESCAPE);
        m_pkBtn_Close->InitState( true, true, this );
        m_pkBtn_Close->DisconnectAllEventProcedure();
        m_pkBtn_Close->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionEquipPreview_S4::OnClickClsoe );
        m_pkBtn_Close->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionEquipPreview_S4::OnEnterCursor );
    }
    if( m_pkBackGround )
        m_pkBackGround->InitState( true, true, this );
    if( m_pkAlphaFrame )
        m_pkAlphaFrame->InitState( true, true, this );

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProject = matLook * matScale * matTran;

    this->SetSelfInputCheck( true );
    this->SetWindowZOrder( D3DWZ_TOPMOST );
    SetPreview();


    ReloadMotion();

    return true;
}

void KGCMissionEquipPreview_S4::ActionPerformed( const KActionEvent& event ){}

void KGCMissionEquipPreview_S4::FrameMoveInEnabledState( void )
{
    if( !m_bAngleChanged )
    {
        m_fAngleMove += 0.005f;
        if ( m_fAngleMove >= 2 * D3DX_PI ) m_fAngleMove = 0.0f;
    }

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );
    if( this->CheckPosInWindowBoundWithChild(vMousePos, false) )
    {
        if( g_pkInput->ISWheelUp() )
            ChangeCharWndZoom( true );
        else if( g_pkInput->ISWheelDown() )
            ChangeCharWndZoom( false );

        if( g_pkInput->BtnDown() )
        {
            m_bAngleChanged = true;
            m_ptStartMouse = g_pkInput->GetMousePos();
            m_fStartAngle = m_fAngleMove;
        }

        if( g_pkInput->BtnDown( KInput::MBRIGHT ) )
            m_bAngleChanged = false;
    }

    if( g_pkInput->BtnPressed() && m_bAngleChanged )
        ProcessCharWndAngle();

    if( m_kPlayer.m_pObject )
    {
        m_kPlayer.SetRender( true );
        m_kPlayer.EgoItemFrameMove( UCHAR(m_cFrame) );
        m_kPlayer.m_pObject->ChildFrameMove();
    }
}

void KGCMissionEquipPreview_S4::PostChildDraw()
{
    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
    g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,72,64,62), 1.0f, 0 );
    m_pkAlphaFrame->Render();
    m_pkBtn_Close->Render();

    if( m_pMotion )
    {
        if ( m_cFrame > int(m_pMotion->GetNum_Frame() - 1) )
        {
            m_iMotionCount++;
            if( 5 == m_iMotionCount )
            {
                if( m_pActionMotion )
                {
                    // 모션이 바뀌었음
                    m_pMotion = m_pActionMotion;
                    m_iMotionCount = 0;
                    m_fCharX = 0.0f;// X를 초기화                    
                }
            }
            else
            {
                if( m_pWaitMotion )
                {
                    // 모션이 바뀌었음
                    m_pMotion = m_pWaitMotion;
                    m_fCharX = 0.0f;// X를 초기화                    
                }
            }
            m_cFrame = 0;
        }

        D3DXVECTOR3 vecPos(0.0f, 0.0f, 0.303f);
        D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
        D3DXMatrixRotationY( &matRot, m_fAngleMove );
        D3DXMatrixTranslation( &matTrans, vecPos.x, vecPos.y, vecPos.z );
        matWorld = matScale * matRot * matTrans;

        m_kPlayer.m_pObject->SetWorldMat( &matWorld );
        m_kPlayer.m_pObject->SetFrame( &m_pMotion->GetFrameData( (int)m_cFrame ) );
        g_RenderManager->Render( false , m_kPlayer.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM );
        m_cFrame++;
    }

    // 뷰포트 복구
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_MyD3D->matProj );
    g_pd3dDevice->SetViewport( &orgViewport );
    g_pd3dDevice->SetTexture( 1, NULL );
}

void KGCMissionEquipPreview_S4::OnClickClsoe()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
 }

void KGCMissionEquipPreview_S4::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCMissionEquipPreview_S4::ChangeCharWndZoom( bool bZoom )
{
    if( bZoom )                 m_fZoom += DELTAZOOM;
    else                        m_fZoom -= DELTAZOOM;
    if( m_fZoom > MAXZOOM )       m_fZoom = MAXZOOM;
    if( m_fZoom < 0.17f )       m_fZoom = 0.17f;
}

void KGCMissionEquipPreview_S4::ProcessCharWndAngle( void )
{
    POINT CurPoint = g_pkInput->GetMousePos();
    m_fAngleMove = m_fStartAngle - (CurPoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

void KGCMissionEquipPreview_S4::SetPreviewWindow( std::vector<GCITEMID> &vecItemID )
{
    if( 0 >= vecItemID.size() )
        return;

    m_vecPriviewEquip.clear();
    m_vecPriviewEquip = vecItemID;
}

void KGCMissionEquipPreview_S4::SetPreview()
{
    if( 0>= m_vecPriviewEquip.size() )
        return;

    GCItem *pItem = g_pItemMgr->GetItemData( m_vecPriviewEquip[0] );
    SetChar( pItem->GetCharType() );

    D3DXVECTOR2 vPos( GetFixedWindowPos() );
    m_viewport.X = static_cast<DWORD>(( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_OFFSET_X) + vPos.x ) * g_pGCDeviceManager2->GetWindowScaleX());
    m_viewport.Y = static_cast<DWORD>(( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_OFFSET_Y) + vPos.y ) * g_pGCDeviceManager2->GetWindowScaleY());
    m_viewport.Width = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT( VIEWPORT_WIDTH ) * g_pGCDeviceManager2->GetWindowScaleX() );
    m_viewport.Height = static_cast<DWORD>( GC_SCREEN_DIV_SIZE_INT(VIEWPORT_HEIGHT)  * g_pGCDeviceManager2->GetWindowScaleY() );
    m_viewport.MinZ = 0.0f;
    m_viewport.MaxZ = 1.0f;

    UpdateMeshObject();
}

void KGCMissionEquipPreview_S4::SetChar( int iCharType )
{
    InitCharacterInfo();
    m_kPlayer.m_kUserInfo.SetCurrentChar(iCharType);
    m_iCharIdx = iCharType;

    m_cFrame = 0;
    m_fCharX = 0.0f;
    m_iMotionCount = 0;
}

void KGCMissionEquipPreview_S4::InitCharacterInfo()
{
    std::map< int, std::set< char> > mapCharInfo = g_kGlobalValue.GetOpenedCharacterInfo();
    std::map< int, std::set< char> >::iterator mit = mapCharInfo.begin();

    m_kPlayer.m_kUserInfo.Init();

    for( ; mit != mapCharInfo.end(); ++mit )
    {
        SCharInfo charInfo;
        charInfo.iCharType = mit->first;
        if ( charInfo.iCharType == GC_CHAR_DIO )
            charInfo.setPromotion = g_kGlobalValue.m_kUserInfo.GetCurrentChar( GC_CHAR_DIO ).setPromotion;
        else
            charInfo.setPromotion = mit->second;
        m_kPlayer.m_kUserInfo.AddChar(charInfo);
    }
}

void KGCMissionEquipPreview_S4::UpdateMeshObject()
{
    GCItem *pItem;

    int iBeforeiExtraIdx = m_iExtraIdx;
    m_iExtraIdx = m_iCharIdx;    
    
    
    m_kPlayer.ReleasePlayerResource();
    std::vector<GCITEMID>::iterator vecIter = m_vecPriviewEquip.begin();
    for(; vecIter != m_vecPriviewEquip.end(); ++vecIter)
    {
        pItem = g_pItemMgr->GetItemData( *vecIter );
        if( pItem->dwSlotPosition == ESP_SET_ITEM ) 
        {
            std::vector<int> vecSetList = g_pItemMgr->GetSetItemList( pItem->iSetItemNumber );
            for(std::vector<int>::iterator vecSetIter = vecSetList.begin(); vecSetIter != vecSetList.end(); ++vecSetIter)
            {
                pItem = g_pItemMgr->GetItemData( *vecSetIter );
                g_pItemMgr->EquipItem(&(m_kPlayer.m_kUserInfo.GetCurrentChar()), pItem);
            }
            continue;
        }
        else if( pItem->dwSlotPosition == ESP_WEAPON )
        {
            m_iExtraIdx = (pItem->iNeedChangeJobLevelForEquippingItem * GC_CHAR_NUM) + m_iCharIdx;
        }
        g_pItemMgr->EquipItem(&(m_kPlayer.m_kUserInfo.GetCurrentChar()), pItem);
    }

    //  캐릭터가 바꼈을 경우
    if ( m_iExtraIdx != iBeforeiExtraIdx ) 
    {
        ReloadMotion();
    }

    if( m_pWaitMotion != NULL )
        m_pMotion = m_pWaitMotion;

    m_kPlayer.SetMyPlayer( -1, true );

    m_kPlayer.m_iPlayerIndex = 0;
    m_kPlayer.m_kUserInfo.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;

    m_kPlayer.UpdatePlayerResource(true);


    switch( m_iExtraIdx % GC_CHAR_NUM )
    {
    case GC_CHAR_DIO:
        m_kPlayer.ToggleExtraMesh( "WEAPON", true );
        m_kPlayer.ToggleExtraMesh( "MERGE_WEAPON", false );
        break;
    case GC_CHAR_ZERO:
        m_kPlayer.ToggleExtraMesh( "GRANDARK_2ND", false );
        m_kPlayer.ToggleExtraMesh( "GRANDARK_2ND_1", false );
        m_kPlayer.ToggleExtraMesh( "GRANDARK_2ND_2", false );
        m_kPlayer.ToggleExtraMesh( "DOG_GRANDARK", false );
        break;
    case GC_CHAR_LUPUS:
        m_kPlayer.ToggleExtraMesh( "WEAPON_0", true );
        m_kPlayer.ToggleExtraMesh( "WEAPON_1", true );
        break;
    default:
        m_kPlayer.ToggleExtraMesh( "WEAPON", m_kPlayer.GetCurFormTemplate().WeaponRenderBasically );
        m_kPlayer.ToggleExtraMesh( "MERGE_WEAPON", m_kPlayer.GetCurFormTemplate().LeftWeaponRenderBasically );
        break;
    }

    if( m_kPlayer.m_pObject )
        m_kPlayer.m_pObject->SetShellMatrixScale( 0.25f );
}


void KGCMissionEquipPreview_S4::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraIdx, &m_pWaitMotion, &m_pActionMotion );
}