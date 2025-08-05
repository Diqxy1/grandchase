#include "stdafx.h"
#include "GCSuperGachaDlg.h"
#include "GCRenderManager.h"
#include "KGCFaceLift.h"

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

IMPLEMENT_CLASSNAME( KGCSuperGachaDlg );
IMPLEMENT_WND_FACTORY( KGCSuperGachaDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSuperGachaDlg, "gc_super_gacha_dlg" );

KGCSuperGachaDlg::KGCSuperGachaDlg( void )
: m_pkBtnClose( NULL )
, m_pkBtnOK( NULL )
, m_pkHotMark( NULL )
, m_pkSelectWnd( NULL )
, m_iCharType( 0 )
, m_iPromotionLevel( 0 )
, m_iExtraCharNum( 0 )
, m_iVersion( 0 )
, m_pmeshStage( NULL )
, m_pShopMotion( NULL )
, m_pkPet( NULL )
, m_iMotionCount( 0 )
, m_cFrame( 0 )
, m_fCharPosX( 0.15f )
, m_fAngleMove( 0.0f )
, m_fStartAngle( 0.0f )
, m_fZoom( 0.25f )
, m_bAngleChanged( false )
{
    LINK_CONTROL( "btnColse",   m_pkBtnClose );
    LINK_CONTROL( "btnOK",      m_pkBtnOK );
    LINK_CONTROL( "hotMark",    m_pkHotMark );
    LINK_CONTROL( "gacha_select_wnd", m_pkSelectWnd );

    m_mapRewardInfo.clear();
    m_mapRewardSetInfo.clear();
    m_mapNeedKeyInfo.clear();
    m_mapAccEnable.clear();
    m_mapSealInfo.clear();

    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
}

KGCSuperGachaDlg::~KGCSuperGachaDlg( void )
{
    DeleteMeshObject();
}

void KGCSuperGachaDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSuperGachaDlg::OnClickClose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnOK->DisconnectAllEventProcedure();
    m_pkBtnOK->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSuperGachaDlg::OnClickOK );

    m_pkHotMark->InitState( false, false, NULL );
    m_pkSelectWnd->InitState( true, true, this );

    if( m_pkPet == NULL ) { 
        m_pkPet = new KPet();
    }

    D3DXMATRIX matLook, matScale, matTran;
    ::D3DXMatrixPerspectiveLH( &matLook, float( VIEWPORT_WIDTH ) / float( VIEWPORT_HEIGHT ), 1.0f, 1.0f, 100.0f );
    ::D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 0.01f );
    ::D3DXMatrixTranslation( &matTran, 0.0f, 0.172f, 0.5f );
    m_matProjection = matLook * matScale * matTran;

    ReloadMotion();
}

void KGCSuperGachaDlg::OnDestroy( void )
{
    DeleteMeshObject();
}

bool KGCSuperGachaDlg::InitDialog( IN const KDialogInfo& kInfo_ )
{
    m_iCharType = static_cast<int>( kInfo_.m_lParam );
    m_iPromotionLevel = static_cast<int>( kInfo_.m_lParam2 );
    SetSealInfo();

    m_pkSelectWnd->SetGachaListInfo( g_kGlobalValue.m_vecGachaUseVersion );
    m_pkSelectWnd->InitSelectWnd();

    m_pkSelectWnd->OnClickEquip();

    m_iVersion = m_pkSelectWnd->GetSelectVersion();

    Send_RewardListReq();
    Send_RewardSetListReq();

    SetPlayer();

    return true;
}

void KGCSuperGachaDlg::SetSelectVersion( const int iVer_ )
{
    m_iVersion = iVer_;

    Send_RewardListReq();
    Send_RewardSetListReq();
    SetPlayer();
}

void KGCSuperGachaDlg::SetSealInfo( void )
{
    m_mapSealInfo.clear();

    std::map<DWORD,int> mapGachaList = g_pItemMgr->GetGachaList();
    for(std::map<DWORD,int>::const_iterator mit = mapGachaList.begin() ; mit != mapGachaList.end() ; ++mit ) {
        m_mapSealInfo.insert( std::make_pair( mit->second, mit->first ) );
    }
}

void KGCSuperGachaDlg::ActionPerformed( const KActionEvent& event )
{
}

void KGCSuperGachaDlg::OnClickClose( void )
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );    
}

#define BYPASS_CLEARSEAL // Kawan>

void KGCSuperGachaDlg::OnClickOK( void )
{
	std::map< int, std::set< GCITEMID > >       mapReward;
    std::map< int, std::vector< GCITEMID > >    mapRewardSet;

    GetRewardMap( mapReward );
    GetRewardSetMap( mapRewardSet );

    g_pkUIScene->GetNewClearSealDlg()->SetItemLevelList(m_mapNeedKeyInfo[m_iVersion]);
    g_pkUIScene->GetNewClearSealDlg()->SetMapGachaRewardList(GetSealID(), mapReward);
    g_pkUIScene->GetNewClearSealDlg()->SetRewardItemList(mapReward);
    g_pkUIScene->GetNewClearSealDlg()->SetMapGachaFinalRewardList(GetSealID(), mapRewardSet);
    g_pkUIScene->GetNewClearSealDlg()->SetFinalRewardtItemList(mapRewardSet);

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, GetSealID(), m_iVersion, true, false, true);

    OnClickClose();
}

GCITEMID KGCSuperGachaDlg::GetSealID( void )
{
    std::map< int, GCITEMID >::const_iterator mit = m_mapSealInfo.find( m_iCharType );
    if( mit == m_mapSealInfo.end() ) { 
        return 0;
    }   

    return mit->second;
}


int KGCSuperGachaDlg::GetCharType( const GCITEMID SealID_ )
{
    std::map< int, GCITEMID >::const_iterator mit = m_mapSealInfo.begin();
    for( ; mit != m_mapSealInfo.end() ; ++mit ) {
        if( mit->second == SealID_ ) { 
            return mit->first;
        }
    }

    return -1;
}

void KGCSuperGachaDlg::Send_RewardListReq( void  )
{
    GCITEMID SealID = GetSealID();
    if( SealID == 0 ) { 
        return;
    }

    // 버전으로 체크 
    std::map< int, std::map< int, std::set< GCITEMID > > >::const_iterator itVersion = m_mapRewardInfo.find( m_iVersion );

    if( itVersion == m_mapRewardInfo.end() ) { 
        KP2P::GetInstance()->Send_GachaRewardListReq( m_iVersion, SealID*10 );
        Result_Gacha_Reward_List = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
        return;
    }

    // 캐릭터로 체크 
    std::map< int, std::set< GCITEMID > >::const_iterator itList = itVersion->second.find( m_iCharType );
    if( itList == itVersion->second.end() ) { 
        KP2P::GetInstance()->Send_GachaRewardListReq( m_iVersion, SealID*10 );
        Result_Gacha_Reward_List = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
        return;
    }

    // 실제로 장비 정보 들고 있나도 체크 
    if( itList->second.empty() ) { 
        KP2P::GetInstance()->Send_GachaRewardListReq( m_iVersion, SealID*10 );
        Result_Gacha_Reward_List = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
    }
}

void KGCSuperGachaDlg::SetRewardList( const KGachaRewardList& list_ )
{
    int iRewardCharType = GetCharType( list_.m_dwGachaItemID / 10 );
    if( iRewardCharType == -1 ) { 
        return;
    }

    std::map< int, std::map< int, std::set< GCITEMID > > >::iterator itVersion = m_mapRewardInfo.find( list_.m_nVersion );

    if( itVersion == m_mapRewardInfo.end() ) {  // 버전 정보를 가지고 있지 않음
        std::set< GCITEMID > setReward;
        std::map< int, std::set< GCITEMID > >::const_iterator itRewardList = list_.m_mapCollectionList.begin();
        
        for( ; itRewardList != list_.m_mapCollectionList.end() ; ++itRewardList ) { 
            std::set< GCITEMID >::const_iterator itList = itRewardList->second.begin();
            for( ; itList != itRewardList->second.end() ; ++itList ) { 
                setReward.insert( *itList );
            }
        }

        m_mapRewardInfo[list_.m_nVersion].insert( std::make_pair( iRewardCharType, setReward ) );
    }
    else {      // 버전 정보를 가지고 있음 
        std::map< int, std::set< GCITEMID > >::iterator itCharReward = itVersion->second.find( iRewardCharType );

        if( itCharReward == itVersion->second.end() ) {     // 캐릭터 보상 정보 없음 
            std::set< GCITEMID > setReward;
            std::map< int, std::set< GCITEMID > >::const_iterator itRewardList = list_.m_mapCollectionList.begin();

            for( ; itRewardList != list_.m_mapCollectionList.end() ; ++itRewardList ) { 
                std::set< GCITEMID >::const_iterator itList = itRewardList->second.begin();
                for( ; itList != itRewardList->second.end() ; ++itList ) { 
                    setReward.insert( *itList );
                }
            }
            
            m_mapRewardInfo[list_.m_nVersion].insert( std::make_pair( iRewardCharType, setReward ) );
        }
        else { // 캐릭터 보상정보 있음 
            std::map< int, std::set< GCITEMID > >::const_iterator itRewardList = list_.m_mapCollectionList.begin();

            for( ; itRewardList != list_.m_mapCollectionList.end() ; ++itRewardList ) { 
                std::set< GCITEMID >::const_iterator itList = itRewardList->second.begin();
                for( ; itList != itRewardList->second.end() ; ++itList ) { 
                    itCharReward->second.insert( *itList );
                }
            }
        }
    }

    // 해제서 개수 
    std::map< int, std::map< int, int > >::const_iterator itKey =  m_mapNeedKeyInfo.find( list_.m_nVersion );
    if( itKey == m_mapNeedKeyInfo.end() ) { 
        m_mapNeedKeyInfo.insert( std::make_pair( list_.m_nVersion, list_.m_mapNeedKeyInfo ) );
    }

    std::map< int, bool >::const_iterator itAcc = m_mapAccEnable.find( list_.m_nVersion );
    if( itAcc == m_mapAccEnable.end() ) { 
        m_mapAccEnable.insert( std::make_pair( list_.m_nVersion, list_.m_bAccessoriesEnable ) );
    }
    
}

void KGCSuperGachaDlg::Send_RewardSetListReq( void )
{
    GCITEMID SealID = GetSealID();
    if( SealID == 0 ) { 
        return;
    }

    // 버전으로 체크 
    std::map< int, std::map< int, std::vector< GCITEMID > > >::const_iterator itVersion = m_mapRewardSetInfo.find( m_iVersion );

    if( itVersion == m_mapRewardSetInfo.end() ) { 
        KP2P::GetInstance()->Send_GachaSetRewardListReq( m_iVersion, SealID*10 );
        Result_Gacha_Reward_List = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
        return;
    }

    // 캐릭터로 체크 
    std::map< int, std::vector< GCITEMID > >::const_iterator itList = itVersion->second.find( m_iCharType );
    if( itList == itVersion->second.end() ) { 
        KP2P::GetInstance()->Send_GachaSetRewardListReq( m_iVersion, SealID*10 );
        Result_Gacha_Reward_List = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
    }

    // 실제로 장비 정보 들고 있나도 체크 
    if( itList->second.empty() ) {
        KP2P::GetInstance()->Send_GachaSetRewardListReq( m_iVersion, SealID*10 );
        Result_Gacha_Reward_List = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_Gacha_Reward_List, INT_MAX, 5000, NULL);
    }
}

void KGCSuperGachaDlg::SetRewardSetList( const KGachaRewardList& list_ )
{
    int iRewardCharType = GetCharType( list_.m_dwGachaItemID / 10 );
    if( iRewardCharType == -1 ) { 
        return;
    }

    std::map< int, std::map< int, std::vector< GCITEMID > > >::iterator itVersion = m_mapRewardSetInfo.find( list_.m_nVersion );

    if( itVersion == m_mapRewardSetInfo.end() ) {  // 버전 정보를 가지고 있지 않음
        std::vector< GCITEMID > vecRewardSet;
        std::map< int, std::vector< GCITEMID > >::const_iterator itRewardSetList = list_.m_mapSetRewardList.begin();

        for( ; itRewardSetList != list_.m_mapSetRewardList.end() ; ++itRewardSetList ) { 
            std::vector< GCITEMID >::const_iterator itList = itRewardSetList->second.begin();
            for( ; itList != itRewardSetList->second.end() ; ++itList ) { 
                vecRewardSet.push_back( *itList );
            }
        }

        m_mapRewardSetInfo[list_.m_nVersion].insert( std::make_pair( iRewardCharType, vecRewardSet ) );
    }
    else {      // 버전 정보를 가지고 있음 
        std::map< int, std::vector< GCITEMID > >::iterator itCharReward = itVersion->second.find( iRewardCharType );

        if( itCharReward == itVersion->second.end() ) {     // 캐릭터 보상 정보 없음 
            std::vector< GCITEMID > vecRewardSet;
            std::map< int, std::vector< GCITEMID > >::const_iterator itRewardSetList = list_.m_mapSetRewardList.begin();

            for( ; itRewardSetList != list_.m_mapSetRewardList.end() ; ++itRewardSetList ) { 
                std::vector< GCITEMID >::const_iterator itList = itRewardSetList->second.begin();
                for( ; itList != itRewardSetList->second.end() ; ++itList ) { 
                    vecRewardSet.push_back( *itList );
                }
            }

            m_mapRewardSetInfo[list_.m_nVersion].insert( std::make_pair( iRewardCharType, vecRewardSet ) );
        }
        else { // 캐릭터 보상정보 있음 
            std::map< int, std::vector< GCITEMID > >::const_iterator itRewardSetList = list_.m_mapSetRewardList.begin();

            for( ; itRewardSetList != list_.m_mapSetRewardList.end() ; ++itRewardSetList ) { 
                std::vector< GCITEMID >::const_iterator itList = itRewardSetList->second.begin();
                for( ; itList != itRewardSetList->second.end() ; ++itList ) { 
                    itCharReward->second.push_back( *itList );
                }
            }
        }
    }
}

void KGCSuperGachaDlg::SetPlayer( void )
{
    m_Player = g_MyD3D->m_TempPlayer;
    m_Player.m_kUserInfo = g_kGlobalValue.m_kUserInfo;
    m_Player.m_kUserInfo.SetCurrentChar( m_iCharType );
    
    m_Player.AutoSetExtra();

    int iBeforeExtraCharNum = m_iExtraCharNum;
    m_iExtraCharNum = m_Player.Extra_Char_Num;

    m_cFrame = 0;
    m_iMotionCount = 0;

    m_fCharPosX = 0.15f;

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

    SetPet();
    EquipItem();
    UpdateMeshObject();
}

void KGCSuperGachaDlg::SetPet( void )
{
    std::vector< GCITEMID > vecRewardSetList = m_mapRewardSetInfo[m_iVersion][m_iCharType];
    std::vector< GCITEMID >::const_iterator itRewardSet = vecRewardSetList.begin();
    for( ; itRewardSet != vecRewardSetList.end() ; ++itRewardSet ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *itRewardSet );
        CONTINUE_NIL( pItem );

        // 가챠 최종 보상을 패키지로 지급 하는 경우에 대한 처리 추가 (슈퍼가챠 창에서 가챠 장비 입은 캐릭터와 펫 그려주기 위해서)
        if( g_pItemMgr->IsPackageItemID( pItem->dwGoodsID * 10 ) ) {
            std::vector< KDropItemInfo > vecList;
            g_pItemMgr->GetPackageElements( pItem->dwGoodsID * 10, vecList );

            std::vector< KDropItemInfo >::const_iterator itPackage = vecList.begin();
            for( ; itPackage != vecList.end() ; ++itPackage ) {
                GCItem* pPackaceElement = g_pItemMgr->GetItemData( itPackage->m_ItemID / 10 );
                if( NULL == pPackaceElement ) {
                    continue;
                }

                if( pPackaceElement->eItemKind == GIK_PET ) {
                   pItem = pPackaceElement;
                   break;
                }
            }
        }

        if( pItem->eItemKind != GIK_PET ) {
            continue;
        }

        if( m_pkPet ) { 
            const SPetTemplate petInfo = g_kGlobalValue.GetPetTemplateFromItemID( pItem->dwGoodsID, 0 );
            m_pkPet->StartPet( -1, petInfo.iPetID, 1, 0, EGCPETLOCATION::SHOP, L"", false, 0, true );
            m_pkPet->SetPetMotion( EGCPETMOTION::WAIT );
            break;
        }
    }
}

void KGCSuperGachaDlg::UpdateMeshObject( void )
{
    m_Player.ReleasePlayerResource();
    m_Player.SetMyPlayer( -1, true );	
    m_Player.UpdatePlayerResource(true);

    if( m_Player.m_pObject )
        m_Player.m_pObject->SetShellMatrixScale( 0.25f );  

    m_Player.LoadExtraMesh( m_Player.GetCurFormTemplate().vecExtraMesh );

    if( !m_pmeshStage )
    {
        m_pmeshStage = new GCObject();
#ifdef _DEBUG
        m_pmeshStage->m_strName = "슈퍼가챠 발판";
#endif
        m_pmeshStage->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( "footstage.p3m" ), g_pGCDeviceManager2->CreateTexture( "footstage.dds" ) );
    }
}

void KGCSuperGachaDlg::DeleteMeshObject( void )
{
    m_Player.ReleasePlayerResource();

    if( m_pmeshStage )
    {
        g_RenderManager->RemoveObject( m_pmeshStage );
        SAFE_DELETE( m_pmeshStage );
    }

    if( m_pkPet ) { 
        m_pkPet->EndPet();
        SAFE_DELETE( m_pkPet );
    }
}

void KGCSuperGachaDlg::EquipItem( void )
{
    if( m_mapRewardInfo[m_iVersion][m_iCharType].empty() || m_mapRewardSetInfo[m_iVersion][m_iCharType].empty() ) { 
        return;
    }

    // 장착있는 코디 다 벗기고 
    g_pItemMgr->UnequipLookItemAll( &( m_Player.GetCurrentChar() ) );

    // 장비들
    std::set< GCITEMID > setRewardList = m_mapRewardInfo[m_iVersion][m_iCharType];
    std::set< GCITEMID >::const_iterator itReward = setRewardList.begin();
    for( ; itReward != setRewardList.end() ; ++itReward ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *itReward );
        CONTINUE_NIL( pItem );
        g_pItemMgr->EquipItem( &(m_Player.GetCurrentChar() ), pItem, false, 0, pItem->eItemKind == GIK_COORDI_ITEM );
    }

    // 무기
    std::vector< GCITEMID > vecRewardSetList = m_mapRewardSetInfo[m_iVersion][m_iCharType];
    std::vector< GCITEMID >::const_iterator itRewardSet = vecRewardSetList.begin();
    for( ; itRewardSet != vecRewardSetList.end() ; ++itRewardSet ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *itRewardSet );
        CONTINUE_NIL( pItem );

        if( ( pItem->dwSlotPosition & ESP_WEAPON ) == 0x00000000){ 
            continue;
        }

        // 전직따라 무기 바뀌는 아이들과 무기 안바뀌는 아이들 구분
        if( !g_kGlobalValue.IsHybridPlayer( m_iCharType ) && !g_kGlobalValue.IsQuickSkillPlayer( m_iCharType ) ) { 
            if( pItem->iNeedChangeJobLevelForEquippingItem == m_iPromotionLevel ) { 
                g_pItemMgr->EquipItem( &(m_Player.GetCurrentChar() ), pItem, false, 0, pItem->eItemKind == GIK_COORDI_ITEM );
                break;
            }
        }
        else { 
            if( pItem->iNeedChangeJobLevelForEquippingItem <= m_iPromotionLevel ) { 
                g_pItemMgr->EquipItem( &(m_Player.GetCurrentChar() ), pItem, false, 0, pItem->eItemKind == GIK_COORDI_ITEM );
                break;
            }
        }
    }
}

void KGCSuperGachaDlg::PostChildDraw( void )
{
    if( m_Player.m_pObject == NULL || m_pkPet->m_pObject == NULL ) { 
        return;
    }

    // 행렬 구하는 곳 
    g_pGCDeviceManager2->Flush();
    D3DVIEWPORT9 orgViewport;

    D3DXMATRIX matRot,matTrans,matScale,matWorld;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    g_pd3dDevice->SetFVF( SKINVERTEX::FVF );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProjection );

    // 발판 출력  
    float fScale = 5.2f;
    float fFootStageY = -0.2f;
    float fFootStageZ = 0.38f;
    float fFootStageMul = 0.44f;

    D3DXMatrixScaling( &matScale ,m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
    D3DXMatrixRotationY( &matRot, m_fStageAngleMove );
    D3DXMatrixTranslation( &matTrans, 0.0f, fFootStageY + ( MAXZOOM - m_fZoom ) * fFootStageMul, fFootStageZ );   

    matWorld = matScale * matRot * matTrans;
    if( m_pmeshStage != NULL )
    {
        m_pmeshStage->SetWorldMat( &matWorld );
        g_RenderManager->Render( false, m_pmeshStage, EGCShaderTechnique::CARTOON_NOEDGE );
    }
    
   
    if( m_pShopMotion != NULL )
    {
        // 캐릭터 그리고 
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
                    m_fCharPosX = 0.15f;// X를 초기화

                    SiKGCFaceLift()->SetCurMotionName( m_pActionMotion->GetDeviceID() );
                }
            }
            else
            {
                if( m_pWaitMotion )
                {
                    // 모션이 바뀌었음
                    m_pShopMotion = m_pWaitMotion;
					m_fCharPosX = 0.15f;// X를 초기화

                    SiKGCFaceLift()->SetCurMotionName( m_pWaitMotion->GetDeviceID() );
                }
            }

            m_cFrame = 0;
        }

        float fPosX = m_fCharPosX * CHAR_SCALE_RATIO;
        float fPosY = m_pShopMotion->GetFrameData(m_cFrame).Pos_y * 0.1f - 0.06f;
        float fPosZ = 0.303f;

        m_fCharPosX += m_pShopMotion->GetFrameData(m_cFrame).Plus_x;

        fPosX = fPosX * cos( m_fAngleMove );

        D3DXMatrixScaling( &matScale , m_fZoom, m_fZoom, m_fZoom );
        D3DXMatrixRotationY( &matRot, m_fAngleMove );
        D3DXMatrixTranslation( &matTrans, fPosX, fPosY, fPosZ );

        matWorld = matScale * matRot * matTrans;

        m_Player.m_pObject->SetWorldMat( &matWorld );
        m_Player.m_pObject->SetFrame( &m_pShopMotion->GetFrameData( (int)m_cFrame ) );

        m_Player.SetRenderFaceLift( m_Player.GetDefaultFaceLiftType() );
        SiKGCFaceLift()->SetFaceLiftByMotionName( &m_Player, SiKGCFaceLift()->GetCurMotionName(), (int)m_cFrame );

        g_RenderManager->Render( false ,m_Player.m_pObject, EGCShaderTechnique::TECHNIQUE_NUM);
        m_cFrame++;

        // 펫 그리고 
        if( m_pkPet->m_iPetID != -1 )
        {
            float fScale = 2.5f * m_pkPet->m_fScale;
            D3DXMATRIX mat1, mat2, mat3, mat4, matWorld;

            // 상하
            ::D3DXMatrixTranslation( &mat4, -0.3f / fScale, -0.57f / fScale, -0.2f / fScale);
            ::D3DXMatrixScaling( &mat1, m_fZoom * fScale, m_fZoom * fScale, m_fZoom * fScale );
            ::D3DXMatrixRotationY( &mat2, m_fAngleMove );

            if ( m_pkPet->m_iPetID == EGCPETID::PET_KELBY ) { 
                m_pkPet->usMotion = EGCPETMOTION::SHOP_ACTION;
            }
            else { 
                m_pkPet->usMotion = EGCPETMOTION::WAIT;
            }

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
}

void KGCSuperGachaDlg::FrameMoveInEnabledState( void )
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

    if( m_Player.m_pObject )
    {
        m_Player.SetRender( true );
        m_Player.EgoItemFrameMove( UCHAR(m_cFrame) );
        m_Player.m_pObject->ChildFrameMove();
    }
}

void KGCSuperGachaDlg::ChangeCharWndZoom( bool bZoom )
{
    if( bZoom ) {
        m_fZoom += DELTAZOOM;
    }
    else {
        m_fZoom -= DELTAZOOM; 
    }

    if( m_fZoom > MAXZOOM ) {
        m_fZoom = MAXZOOM;
    }

    if( m_fZoom < MINZOOM ) { 
        m_fZoom = MINZOOM;
    }
}

void KGCSuperGachaDlg::ProcessCharWndAngle( void )
{
    POINT CurrentMousePoint = g_pkInput->GetMousePos();

    m_fAngleMove = m_fStartAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
    m_fStageAngleMove = m_fStartStageAngle - (CurrentMousePoint.x - m_ptStartMouse.x) * DELTAZOOM;
}

bool KGCSuperGachaDlg::GetRewardMap( OUT std::map< int, std::set< GCITEMID > >& mapReward_ )
{
    mapReward_.clear();

    if( m_mapRewardInfo[m_iVersion].empty() ) { 
        return false;
    }

    if( m_mapRewardInfo[m_iVersion][m_iCharType].empty() ) { 
        return false;
    }

    std::set< GCITEMID >::const_iterator sit = m_mapRewardInfo[m_iVersion][m_iCharType].begin();
    for( ; sit != m_mapRewardInfo[m_iVersion][m_iCharType].end() ; ++sit ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *sit );
        CONTINUE_NIL( pItem );

        if( pItem->dwSlotPosition & ESP_HELMET || 
            pItem->dwSlotPosition & ESP_JACKET || 
            pItem->dwSlotPosition & ESP_PANTS  || 
            pItem->dwSlotPosition & ESP_GLOVES || 
            pItem->dwSlotPosition & ESP_SHOES  || 
            pItem->dwSlotPosition & ESP_MANTLE ) { 
                mapReward_[0].insert( *sit );   // 0 = 장비
                continue;
        }

        if( pItem->dwSlotPosition & ESP_A_UPPER_HELMET  ||
            pItem->dwSlotPosition & ESP_A_DOWN_HELMET   ||
            pItem->dwSlotPosition & ESP_A_JACKET        ||
            pItem->dwSlotPosition & ESP_A_PANTS         ||
            pItem->dwSlotPosition & ESP_A_ARM           ||
            pItem->dwSlotPosition & ESP_A_SHOES ) { 
                mapReward_[1].insert( *sit );   // 1 = 악세 
                continue;
        }
    }

    return true;
}

bool KGCSuperGachaDlg::GetRewardMap( const int iVer_, const int iCharType_, OUT std::map< int, std::set< GCITEMID > >& mapReward_ )
{
    mapReward_.clear();

    if( m_mapRewardInfo[iVer_].empty() ) { 
        return false;
    }

    if( m_mapRewardInfo[iVer_][iCharType_].empty() ) { 
        return false;
    }

    std::set< GCITEMID >::const_iterator sit = m_mapRewardInfo[iVer_][iCharType_].begin();
    for( ; sit != m_mapRewardInfo[m_iVersion][iCharType_].end() ; ++sit ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *sit );
        CONTINUE_NIL( pItem );

        if( pItem->dwSlotPosition & ESP_HELMET || 
            pItem->dwSlotPosition & ESP_JACKET || 
            pItem->dwSlotPosition & ESP_PANTS  || 
            pItem->dwSlotPosition & ESP_GLOVES || 
            pItem->dwSlotPosition & ESP_SHOES  || 
            pItem->dwSlotPosition & ESP_MANTLE ) { 
                mapReward_[0].insert( *sit );   // 0 = 장비
                continue;
        }

        if( pItem->dwSlotPosition & ESP_A_UPPER_HELMET  ||
            pItem->dwSlotPosition & ESP_A_DOWN_HELMET   ||
            pItem->dwSlotPosition & ESP_A_JACKET        ||
            pItem->dwSlotPosition & ESP_A_PANTS         ||
            pItem->dwSlotPosition & ESP_A_ARM           ||
            pItem->dwSlotPosition & ESP_A_SHOES ) { 
                mapReward_[1].insert( *sit );   // 1 = 악세 
                continue;
        }
    }

    return true;    
}

bool KGCSuperGachaDlg::GetRewardSetMap( OUT std::map< int, std::vector< GCITEMID > >& mapRewardSet_ )
{
    mapRewardSet_.clear();

    if( m_mapRewardSetInfo[m_iVersion].empty() ) { 
        return false;
    }

    if( m_mapRewardSetInfo[m_iVersion][m_iCharType].empty() ) { 
        return false;
    }

    std::vector< GCITEMID >::const_iterator vit = m_mapRewardSetInfo[m_iVersion][m_iCharType].begin();
    for( ; vit != m_mapRewardSetInfo[m_iVersion][m_iCharType].end() ; ++vit ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *vit );
        
        // 가챠 최종 보상을 패키지로 지급 하는 경우에 대한 처리 추가. 
        bool bPetItem = false;
        bool bPackage = g_pItemMgr->IsPackageItemID( pItem->dwGoodsID * 10 );
        if( bPackage ) {
            std::vector< KDropItemInfo > vecList;
            g_pItemMgr->GetPackageElements( pItem->dwGoodsID * 10, vecList );

            std::vector< KDropItemInfo >::const_iterator itPackage = vecList.begin();
            for( ; itPackage != vecList.end() ; ++itPackage ) {
                GCItem* pPackaceElement = g_pItemMgr->GetItemData( itPackage->m_ItemID / 10 );
                if( NULL == pPackaceElement ) {
                    continue;
                }

                if( pPackaceElement->eItemKind == GIK_PET ) {
                    bPetItem = true;
                    break;
                }
            }
        }

        if( pItem->eItemKind == GIK_PET || (bPackage && bPetItem) ) { 
            mapRewardSet_[1].push_back( *vit );     // 1 = 펫
        }
        else { 
            mapRewardSet_[0].push_back( *vit );     // 0 = 무기 
        }
    }

    return true;
}

bool KGCSuperGachaDlg::GetRewardSetMap( const int iVer_, const int iCharType_, OUT std::map< int, std::vector< GCITEMID > >& mapRewardSet_ )
{
    mapRewardSet_.clear();

    if( m_mapRewardSetInfo[iVer_].empty() ) { 
        return false;
    }

    if( m_mapRewardSetInfo[iVer_][iCharType_].empty() ) { 
        return false;
    }
    
    std::vector< GCITEMID >::const_iterator vit = m_mapRewardSetInfo[iVer_][iCharType_].begin();
    for( ; vit != m_mapRewardSetInfo[m_iVersion][iCharType_].end() ; ++vit ) { 
        GCItem* pItem = g_pItemMgr->GetItemData( *vit );

        // 가챠 최종 보상을 패키지로 지급 하는 경우에 대한 처리 추가. 
        bool bPetItem = false;
        bool bPackage = g_pItemMgr->IsPackageItemID( pItem->dwGoodsID * 10 );
        if( bPackage ) {
            std::vector< KDropItemInfo > vecList;
            g_pItemMgr->GetPackageElements( pItem->dwGoodsID * 10, vecList );

            std::vector< KDropItemInfo >::const_iterator itPackage = vecList.begin();
            for( ; itPackage != vecList.end() ; ++itPackage ) {
                GCItem* pPackaceElement = g_pItemMgr->GetItemData( itPackage->m_ItemID / 10 );
                if( NULL == pPackaceElement ) {
                    continue;
                }

                if( pPackaceElement->eItemKind == GIK_PET ) {
                    bPetItem = true;
                    break;
                }
            }
        }

        if( pItem->eItemKind == GIK_PET || (bPackage && bPetItem) ) { 
            mapRewardSet_[1].push_back( *vit );     // 1 = 펫
        }
        else { 
            mapRewardSet_[0].push_back( *vit );     // 0 = 무기 
        }
    }

    return true;
}

bool KGCSuperGachaDlg::GetNeedKeyInfo( const int iVer_, OUT std::map<int, int>& mapKeyInfo_ )
{
    mapKeyInfo_.clear();
    if( m_mapNeedKeyInfo[iVer_].empty() ) { 
        return false;
    }

    mapKeyInfo_ = m_mapNeedKeyInfo[iVer_];
    return true;
}

bool KGCSuperGachaDlg::GetAccEnable( const int iVer_ )
{
    std::map< int, bool >::const_iterator mit = m_mapAccEnable.find( iVer_ );
    if( mit == m_mapAccEnable.end() ) { 
        return false;
    }

    return mit->second;
}

void KGCSuperGachaDlg::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion( m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion );
}