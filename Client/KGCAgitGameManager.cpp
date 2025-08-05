#include "stdafx.h"
#include "Procedure.h"
#include "Message.h"
#include "KGCObj.h"
#include "Controls.h"
#include "PetAI.h"
#include "GCStateGame.h"
#include "GCRenderManager.h"

#include "KncP2PLib/KncP2P.h"
#include "KncP2PLib/kncpeermanager.h"
#include "KSingleton.h"

#include "KGCRoomManager.h"
#include "KGCChannelManager.h"
#include "KGCAgitGameManager.h"
#include "GCUI/GCAgitOverlayUI.h"
#include "GCUI/GCAgitCharSetting.h"
#include "Buff/KGCBuffManager.h"

#include "GCAgitObject.h"
#include "Monster.h"


ImplementSingleton( KGCAgitGameManager );

KGCAgitGameManager::KGCAgitGameManager()
{
    m_prURServerIpPort.first = 0;
    m_prURServerIpPort.second = 0;

    m_bAgitEdit = false;
    m_bObjectPick = false;
    m_pPick2DObject = NULL;

    m_bNowShop = false;
    m_bShopEdit = false;

    //m_NowEdit = m_mapEditable.begin();

    m_bPrePlace = false;

    m_bRegistAgitList = false;
    m_bExitGame = false;

    m_prNowPlantPot.first = 0;
    m_prNowPlantPot.second = 0;

    m_bWatch = false;

    m_iBrokenTile = -1;

    m_iPickCharIndex = -1;
    m_iPlaceCharUID = 0;

    m_bAgitEntered = false;

    m_usMaxCharPosCount = 0;
}

KGCAgitGameManager::~KGCAgitGameManager( void )
{
    ClearObject();
}

bool KGCAgitGameManager::LoadObjectInfo( char* strFile )
{
    LoadMapInfo();
    LoadFairyTreeInfo();
    LoadAgitCharacterMotionInfo();
    LoadCashObjectLimitCount( strFile );
    LoadExceptAgitObjectID( strFile );
    return m_kObjInfoList.LoadObjectInfo( strFile );
}

bool KGCAgitGameManager::LoadCashObjectLimitCount( const char* strFile_ )
{
    KLuaManager luaMgr;

    if( false == GCFUNC::LoadLuaScript( luaMgr, strFile_ ) ) { 
        return false;
    }

    m_mapCashItemLimitCount.clear();

    if( SUCCEEDED( luaMgr.BeginTable( "CASH_ITEM_LIMIT_COUNT") ) ) {
        int iItemCount = 1;
        while( true ) { 
            if( FAILED( luaMgr.BeginTable( iItemCount ) ) ) {
                break;
            }

            GCITEMID itemID;
            int      iCount;
            LUA_GET_VALUE_DEF( 1, itemID, 0 );
            LUA_GET_VALUE_DEF( 2, iCount, 0 );

            m_mapCashItemLimitCount[ itemID ] = iCount;
            luaMgr.EndTable();
            ++iItemCount;
        }
        luaMgr.EndTable();
    }

    return true;
}

bool KGCAgitGameManager::LoadExceptAgitObjectID( const char* strFile_ )
{
    KLuaManager luaMgr;

    if( false == GCFUNC::LoadLuaScript( luaMgr, strFile_ ) ) { 
        return false;
    }

    m_setExceptObjectID.clear();

    if( SUCCEEDED( luaMgr.BeginTable( "EXCEPT_OBJECT_LIST") ) ) {

        for( int i=1 ; ; ++i )
        {
            GCITEMID itemID;
            if( FAILED( luaMgr.GetValue( i, itemID ) ) ) {
                break;
            }

            m_setExceptObjectID.insert( itemID );
        }

        luaMgr.EndTable();
    }
    return true;
}

bool KGCAgitGameManager::LoadSpecialObjectInfo( char* strFile )
{
    return m_kObjInfoList.LoadSpecialObjectInfo( strFile );
}

void KGCAgitGameManager::ClearObject( void )
{
    m_kObjContainer.ClearObjectList();
    ClearFairyTreeInfo();       // 나무도 정리 
}

void KGCAgitGameManager::ClearObjectInfo( void )
{
    // 종로 될 때 맵과 벡터에 저장된 모든 오브젝트 정보 정리 
    ClearObject();
}

void KGCAgitGameManager::ClearPosObject()
{
    m_kObjContainer.HideObjectAll();
    KP2P::GetInstance()->Send_SetAgitObjectPosReq( m_kObjContainer.GetUpdatedPosList() );
    m_kObjContainer.SetPosUpdated();
}
void KGCAgitGameManager::FrameMoveObject( void )
{
    m_kObjContainer.FrameMove();
}

bool KGCAgitGameManager::CheckPickToObjBound( KGC2DObject* pObj, D3DXVECTOR2 dxvMousePos )
{
    D3DXVECTOR4 dxvPickBound;

    dxvPickBound.x = pObj->GetPos().x;                          //left
    dxvPickBound.y = pObj->GetPos().y + pObj->GetHeight();      //top
    dxvPickBound.z = pObj->GetPos().x + pObj->GetWidth();       //right
    dxvPickBound.w = pObj->GetPos().y;                          //bottom

    if( ( dxvPickBound.x <= dxvMousePos.x && dxvMousePos.x <= dxvPickBound.z ) &&
        ( dxvPickBound.w <= dxvMousePos.y && dxvMousePos.y <= dxvPickBound.y ) )
        return true;

    return false;
}

bool KGCAgitGameManager::CheckPickToObjBound( KGC2DObject* pObj, D3DXVECTOR2 dxvMousePos, float fXWeight )
{
    D3DXVECTOR4 dxvPickBound;

    dxvPickBound.x = pObj->GetPos().x;                          //left
    dxvPickBound.y = pObj->GetPos().y + pObj->GetHeight();      //top
    dxvPickBound.z = pObj->GetPos().x + pObj->GetWidth();       //right
    dxvPickBound.w = pObj->GetPos().y;                          //bottom

    dxvPickBound.z += TO_AGIT_COORDI( fXWeight / 20.0f );

    if( ( dxvPickBound.x <= dxvMousePos.x && dxvMousePos.x <= dxvPickBound.z ) &&
        ( dxvPickBound.w <= dxvMousePos.y && dxvMousePos.y <= dxvPickBound.y ) )
        return true;

    return false;
}

bool KGCAgitGameManager::GetObjectInPos( const D3DXVECTOR2& dxvPickPos, OUT GCITEMUID& itemUID_ )
{
    return m_kObjContainer.GetObjectInMousePos( dxvPickPos, itemUID_ );
}

bool KGCAgitGameManager::PickUpObject( const D3DXVECTOR2& dxvMousePos )
{
    D3DXVECTOR2 vecPickPos = dxvMousePos;
    g_MyD3D->MyCtrl->Picking( vecPickPos );

    GCITEMUID itemUID = KItem::INVALID_ITEM;

    if( GetAgitEditState() ) {
        if( GetObjectInPos( vecPickPos, itemUID ) == false )
            return false;
    }
    else if( IsPrePlace() ) {
        if( GetObjectInPos( vecPickPos, itemUID ) == false )
            return false;

        if( itemUID != 0 ) {
            return false;
        }
    }
    else {
        return false;
    }

    m_vecPickOffset = vecPickPos;
    return m_kObjContainer.PickUpObject( itemUID, &m_vecPickOffset );
}

bool KGCAgitGameManager::PickUpObject( GCITEMUID itemUID,  const D3DXVECTOR2& dxvMousePos )
{
    if( dxvMousePos.x > 0.f ) {
        m_kObjContainer.ShowObject( itemUID, TO_INDEX( dxvMousePos.x ), TO_INDEX( dxvMousePos.y ) );
    }

    m_vecPickOffset.x = -1.f;
    return m_kObjContainer.PickUpObject( itemUID, &m_vecPickOffset );
}

void KGCAgitGameManager::UpdatePickedObjectPos( const D3DXVECTOR2& dxvMousePos )
{
    m_kObjContainer.UpdatePickedObjectPos( dxvMousePos, &m_vecPickOffset );
    return;
}

void KGCAgitGameManager::Render( int iLayer_ )
{
    switch( iLayer_ ) {
        case GC_LAYER_BETWEEN_BLOCK_CHAR:
            // 나무는 캐릭터 뒤쪽에 그려지도록 합니다~
            if( m_kFairyTree.m_p2DObject != NULL )
            {
                m_kFairyTree.m_p2DObject->Render(); // 나무 
                for( std::vector< KGC2DObject* >::iterator it = m_kFairyTree.m_vecFruits.begin() ; it != m_kFairyTree.m_vecFruits.end() ; ++it )
                    (*it)->Render();    // 열매 
            }

            // 나무 그리고 오브젝트 그리도록!
            m_kObjContainer.Render( AGIT_OBJ_LAYER0 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER1 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER2 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER3 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER4 );
            break;

        case GC_LAYER_BETWEEN_CHAR_FRONT:
            m_kObjContainer.Render( AGIT_OBJ_LAYER5 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER6 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER7 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER8 );
            m_kObjContainer.Render( AGIT_OBJ_LAYER9 );
            break;
    }
}

void KGCAgitGameManager::RenderAfterUI( void )
{
    m_kObjContainer.Render_PickedObj();
    RenderPickCharacter();
}


void KGCAgitGameManager::FrameMove( void )
{
    FrameMoveObject();  // 오브젝트 FrameMove
	UpdateChatBalloon();

    if( m_kAgitInfo.m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID ) { 
        g_pkAgitOverlayUI->UpdateFairyTreeLevelGaugePos();
    }
}

void KGCAgitGameManager::EnterMyAgit( void )
{
    // 아지트 입장 전에 해야 할 것들 
    if ( SiKGCPartyManager()->IsParty() )
        SiKGCPartyManager()->LeaveParty();
    EnterAgit( g_kGlobalValue.m_kUserInfo.dwUID );
    
    KP2P::GetInstance()->Send_AgitTutorialDoneReq();
}

void KGCAgitGameManager::EnterAgit( DWORD dwUID, std::wstring strPassword_ )
{
    Result_EnterAgitReq = INT_MAX;
	KP2P::GetInstance()->Send_EnterAgitReq( dwUID, g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, strPassword_ );
    g_MyD3D->WaitForServerAck( Result_EnterAgitReq, INT_MAX, 5000, TIME_OUT_VALUE );
}

void KGCAgitGameManager::InitAgitObjectPos( void )
{
    m_kObjContainer.Create2DObjectALL();	
}

void KGCAgitGameManager::UpdateAgitObjectPos( const std::vector<KAgitObjectPos>& vecPos_ )
{
    if( vecPos_.empty() )
        return;

    m_kObjContainer.SetObjectPosList( vecPos_, &m_kObjInfoList );
    m_kObjContainer.Create2DObjectALL();
}

void KGCAgitGameManager::RefreshInvenObject( void )
{
    int iFlowerPotLimit = 5;
    std::map<DWORD,KAgitMap>::iterator mit = m_kMapCatalog.m_mapMapList.find( m_kAgitInfo.m_dwMapID );

    if( mit != m_kMapCatalog.m_mapMapList.end() ) {
        iFlowerPotLimit = mit->second.m_usMaxFlowerPot;
    }

    m_kObjContainer.InitLocalInventoryObject( &m_kObjInfoList, iFlowerPotLimit );
    //m_kObjContainer.SetObjectPosList( m_kAgitInfo.m_vecObjectPos, &m_kObjInfoList );
    m_kObjContainer.Create2DObjectALL();
}

void KGCAgitGameManager::SetObjectPickOff( void )
{
    m_kObjContainer.PutDownObject();
    return;
}

void KGCAgitGameManager::SetAgitShop( void )
{
    m_mapAgitShopObject.clear();
    m_mapAgitShopObjectMaterial.clear();

    // 아이템 목록과 위치 정보 
    Result_AgitShopCatalog = INT_MAX;
    KP2P::GetInstance()->Send_AgitStoreCatalogReq();
    g_MyD3D->WaitForServerAck( Result_AgitShopCatalog, INT_MAX, 5000, TIME_OUT_VALUE );

	// 아이템 구매 조건 
    if( 0 == Result_AgitShopCatalog )
    {
        std::set< PAIR_USHORT_DWORD > setItemID;
        for( std::map< PAIR_USHORT_DWORD, KManufactureItem >::iterator mit = m_mapAgitShopObject.begin() ; mit != m_mapAgitShopObject.end() ; ++mit )
            setItemID.insert( mit->first );
        KP2P::GetInstance()->Send_AgitStoreMaterialReq( setItemID );   
    }    

    // 구매 가능한 맵 정보 
    KP2P::GetInstance()->Send_AgitMapCatalogueReq();
}

void KGCAgitGameManager::SetAgitShopCatalog( std::map<PAIR_USHORT_DWORD,KManufactureItem>& mapItemInfo, std::map<PAIR_USHORT_DWORD,PAIR_INT>& mapItemPos )
{
    if( m_mapAgitShopObject.empty() )
        m_mapAgitShopObject = mapItemInfo;

    if( m_mapAgitShopObjectPos.empty() )
        m_mapAgitShopObjectPos = mapItemPos;

    Result_AgitShopCatalog = 0;
}

void KGCAgitGameManager::SetAgitShopMaterial( std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterial )
{
    if( m_mapAgitShopObjectMaterial.empty() )
        m_mapAgitShopObjectMaterial = mapMaterial;
}

void KGCAgitGameManager::InitBeforeEnterAgit( void )
{
    // 이전 상태 
    if( true == SiKGCChannelManager()->CheckChannel( CT_DUNGEON ) )
        g_MyD3D->m_pStateMachine->SavePrevState( GS_WORLDMAP );
    else
        g_MyD3D->m_pStateMachine->SavePrevState( GS_MATCH );

    g_MyD3D->m_pStateMachine->SetJoinChannel( false );


    // 게임모드 
    SiKGCChannelManager()->SetCurrentChannelType( CT_AGIT );
    SiKGCRoomManager()->UpdateShowGameModeCategory( false, SiKGCChannelManager()->GetCurrentChannelType() );
    SiKGCRoomManager()->InitGameMode();

    SetAgitEditState( false );
    SetWatchState( false );

    // 리소스 
    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        g_MyD3D->MyPlayer[i]->ClearPlayerData();
        g_MyD3D->MyPlayer[i]->ClearWeaponChengeItem();

        if( g_MyD3D->MyPlayer[i]->m_pObject )
        {
            g_RenderManager->RemoveObject( g_MyD3D->MyPlayer[i]->m_pObject );
            SAFE_DELETE( g_MyD3D->MyPlayer[i]->m_pObject );
        }
    }

    g_MyD3D->MyPlayer[0]->UpdatePlayerResource();
    g_MyD3D->MyPlayer[0]->cFrame = 0;
    g_MyD3D->MyPlayer[0]->SetMyPlayer( -1, false );
    g_MyD3D->MyPlayer[0]->UpdateGlobalUserInfo();
}

void KGCAgitGameManager::BuyAgitShopObject( GCITEMID ItemID, PAIR_USHORT_DWORD pairIndex )
{
    GCItem* pItem = g_pItemMgr->GetItemData( ItemID );
    
    if( pItem == NULL )
        return;

    // 캐시 아이템 
    if( pItem->eMoneyType == EMT_CASH )
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( pItem->dwGoodsID );
#else
    #if defined (NATION_KOREA) || defined ( NATION_USA )
        g_pItemMgr->BuyItem( pItem->dwGoodsID, pItem->iItemTypeValue );
    #else
    #ifdef GAME_CASH
        {
        Result_CanBuyCashItem = INT_MAX;

        KP2P::GetInstance()->Send_CanBuyCashItem( pItem->dwGoodsID * 10 );
        g_MyD3D->WaitForServerAck( Result_CanBuyCashItem, INT_MAX, 5000, TIME_OUT_VALUE );

        /* 아이템 구매제한 체크
        0 : 성공 (구매가능)
        1 : 구매제한 아이템이 아님 (구매가능)

        2 : 구매제한 횟수 초과. (구매한도 초과)
        3 : 인벤토리가 충분하지 않음. (인벤토리 부족)
        4 : 코디인벤토리가 충분하지 않음. (인벤토리 부족)
        5 : 아지트인벤토리가 충분하지 않음.
        6 : 아이템 소지한도 초과.
        -98 : 아직 처리중인 작업.
        */
        switch( Result_CanBuyCashItem  )
        {
        case 0:
            g_pkUIScene->EnableBuyCashItemWnd( pItem->dwGoodsID, false, true );
            break;
        case 1:
            g_pkUIScene->EnableBuyCashItemWnd( pItem->dwGoodsID, false, true );
            break;
        case 2:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_LIMITED_BUY ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
            return;;
        case 3:
        case 4:                 
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_INVENTORY_FULL_ERROR1 ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);   
            return;
        case  5:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_AGIT_INVEN_FULL ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true, true );
            return;
        case 6:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_BUY_AGIT_ITEM_FAIL8 ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true, true );
            return;
        case -98:
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString( STR_ID_BINGO_ERROR7 ),L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);                
            return;
        }
    }
    #endif
    #endif
#endif
        return;
    }

    // 별사탕 아이템인가 찾아봄 
    std::map<PAIR_USHORT_DWORD,KManufactureItem>::iterator mit = m_mapAgitShopObject.find( pairIndex );
    if( mit == m_mapAgitShopObject.end() )
        return;

    // 구매 조건 체크 
    std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >::iterator ItrMaterial = m_mapAgitShopObjectMaterial.find( pairIndex );
    if( ItrMaterial == m_mapAgitShopObjectMaterial.end() )  // 재료 정보 없음
        return;

    // 인벤토리에 해당 재료아이템 있나?
    bool bCanbuy = true;
    for( std::vector< KDropItemInfo >::iterator vit = ItrMaterial->second.begin() ; vit != ItrMaterial->second.end() ; ++vit )
    {
        KItem* pInvenItem = NULL;
        if( false == g_pItemMgr->FindInventoryForItemID( vit->m_ItemID / 10 , &pInvenItem ) )
        {
            bCanbuy = false;
            break;
        }

        if( vit->m_nDuration > pInvenItem->m_nCount )
        {
            bCanbuy = false;
            break;
        }
    }

    // 있으면 구입 
    if( bCanbuy )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, 
            g_pkStrLoader->GetReplacedString( STR_ID_BUY_AGIT_SHOP,  "l", g_pItemMgr->GetItemName(pItem->dwGoodsID) ), L"",
            KGCUIScene::GC_MBOX_USE_BUY_AGIT_SHOP, pairIndex.second, pairIndex.first, true, true );
    }
    else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_BUY_AGIT_SHOP_NOT_ENOUGH_STAR ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
}

void KGCAgitGameManager::SetAgitUserInfo( std::map<DWORD,KAgitUserInfo>& mapAgitUser )
{
    m_mapAgitUser.clear();
    m_mapUserName.clear();
    m_mapAgitUser = mapAgitUser;
}

void KGCAgitGameManager::SetP2PConnectAtEnter( void )
{
    // 아지트 입장시 아지트 안에 있는 유저들과 P2P 연결 

    int nPlayerIndex = -1;

    for( std::map<DWORD,KAgitUserInfo>::iterator mit = m_mapAgitUser.begin() ; mit != m_mapAgitUser.end() ; ++mit )
    {
        if( mit->first != g_kGlobalValue.m_kUserInfo.dwUID )
        {
            SUserInfo tempInfo;
            tempInfo.dwUID = mit->first;
            tempInfo.vecIP.push_back( mit->second.m_prUserIPPort.first );
            tempInfo.vecPort.push_back( mit->second.m_prUserIPPort.second );
            tempInfo.bLive = true;

            AddConnectQ( tempInfo, true );

            in_addr in; // for Log
            in.S_un.S_addr = mit->second.m_prUserIPPort.first;
            std::ostringstream stm;
            stm << inet_ntoa(in); 
        }
        //  자기 인덱스 찾는다.
        else 
        {
            nPlayerIndex = mit->second.m_nPlayerIndex;
        }

        // 리소스 로드 
        LoadUserResource( mit->first );
    }

    UpdateAgitHost( nPlayerIndex );
}

void KGCAgitGameManager::OnInsertUser( DWORD dwAgitUID, KAgitUserInfo& userInfo )
{
    // 내가 있는 아지트에 누군가 들어올 때 들어온 유저랑 P2P 연결 
    if( dwAgitUID != m_kAgitInfo.m_dwUID )
        return;

    // 구경하기 중일 때는 다른 사람 못들어오도록 막는다. 
    if( m_bWatch )
        return;

    // 이미 들어 있나 확인
    std::map<DWORD, KAgitUserInfo>::iterator mit = m_mapAgitUser.find( userInfo.m_dwUID );
    if( mit != m_mapAgitUser.end() )
        return;
    m_mapAgitUser[ userInfo.m_dwUID ] = userInfo;
    
    SUserInfo tempInfo;
    tempInfo.dwUID = userInfo.m_dwUID;
    tempInfo.vecIP.push_back( userInfo.m_prUserIPPort.first );
    tempInfo.vecPort.push_back( userInfo.m_prUserIPPort.second );
    tempInfo.bLive = true;

    AddConnectQ( tempInfo, true );

    in_addr in; // for Log
    in.S_un.S_addr = mit->second.m_prUserIPPort.first;
    std::ostringstream stm;
    stm << inet_ntoa(in); 

    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_ENTER_USER, "l", userInfo.m_strNick ) );

    // 리소스 로드 
    LoadUserResource( userInfo.m_dwUID );

    // 아지트 정보바 갱신
    g_pkAgitOverlayUI->UpdateAgitInfo();

    
    //  Host 정보 갱신
    UpdateAgitHost( userInfo.m_nPlayerIndex );
}

void KGCAgitGameManager::OnDestroyUser( DWORD dwAgitUID, DWORD dwUserUID )
{
    // 내가 있는 아지트에서 누군가 나갈때 P2P 연결 끊음 
    if( dwAgitUID != m_kAgitInfo.m_dwUID )
        return;

    // 들어 있는 유저 맞나 확인
    std::map<DWORD, KAgitUserInfo>::iterator mit = m_mapAgitUser.find( dwUserUID );
    if( mit == m_mapAgitUser.end () )
        return;

    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_LEAVE_USER, "l", mit->second.m_strNick ) );

    // 리소스 정리 
    ReleaseUserResource( dwUserUID );
    m_mapAgitUser.erase( mit );
    KSingleton<KncP2P>::GetInstance()->DisConnectPeer( dwUserUID );

    // 말풍선 지우기
    g_pkUIScene->m_pkAgitOverlayUI->DeleteBalloonMsg(dwUserUID);

    // 아지트 정보바 갱신
    g_pkAgitOverlayUI->UpdateAgitInfo();

    //  아지트 호스트 갱신
    UpdateAgitHost( -1 );
}

void KGCAgitGameManager::LoadUserResource( DWORD dwUserUID )
{
    std::map< DWORD, KAgitUserInfo >::iterator mit = m_mapAgitUser.find( dwUserUID );
    
    if( mit == m_mapAgitUser.end() )
        return;

    if( g_MyD3D->IsPlayerIndex( mit->second.m_nPlayerIndex ) )
    {
        PLAYER *pPlayer = g_MyD3D->MyPlayer[mit->second.m_nPlayerIndex];

        if( g_kGlobalValue.m_kUserInfo.dwUID == dwUserUID )
        {
            pPlayer->m_kUserInfo = g_kGlobalValue.m_kUserInfo;

            // 카메라 타켓셋팅을 위해 임시로 살리고 HP를 셋팅합니다.
            pPlayer->m_kUserInfo.bLive = true;
            pPlayer->m_cLife = 1;
            g_kCamera.SetTargetPlayer(mit->second.m_nPlayerIndex);
            g_kCamera.InitCameraPosition();
        }
        else
        {
            pPlayer->SetAgitUserInfo( mit->second );
            pPlayer->m_kUserInfo.bLive = true;
        }

        // 기본 아이템 채워주기
        g_pItemMgr->EquipBaseItem( &(pPlayer->GetCurrentChar()) );

        // 이펙트 ( 목걸이 )
        pPlayer->EffectInit();

        pPlayer->m_kUserInfo.nUserState = GC_RUS_PLAYING;
        pPlayer->BeforeStartGame();
        pPlayer->CreatePlayer();
        pPlayer->MakeSoundLoadList();
        pPlayer->MakeFireInfo3DObjectLoadList();
        pPlayer->InitBonusBuff();
        // 이펙트 ( 목걸이 )
        pPlayer->LoadAgitMotion();

        pPlayer->bIsRight = true;
        pPlayer->vPos.x = 0.5f;
        pPlayer->vPos.y = 0.5f;
        pPlayer->All_Latency_Equal();
    }
#if !defined(__PATH__)
    else
        MessageBoxW(NULL, L"서버에서 플레이어 인덱스가 잘못 넘어옴", L"Error", MB_OK);
#endif
}

void KGCAgitGameManager::ReleaseUserResource( DWORD dwUserUID )
{
    std::map< DWORD, KAgitUserInfo >::iterator mit = m_mapAgitUser.find( dwUserUID );

    if( mit == m_mapAgitUser.end() )
        return;

    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == dwUserUID )
        {
            g_MyD3D->MyPlayer[i]->EndGame();
            g_MyD3D->MyPlayer[i]->EffectClear();
            g_MyD3D->MyPlayer[i]->ClearMotionParticleList();
            g_MyD3D->MyPlayer[i]->ClearPlayerData();
            g_MyD3D->MyPlayer[i]->ClearWeaponChengeItem();

            if( g_MyD3D->MyPlayer[i]->m_pObject )
            {
                g_RenderManager->RemoveObject( g_MyD3D->MyPlayer[i]->m_pObject );
                SAFE_DELETE( g_MyD3D->MyPlayer[i]->m_pObject );
            }

            g_MyD3D->m_akPet[i]->EndPet();
            g_MyD3D->m_akPet[i]->ClearParticleEffect();
        }
    }
}

void KGCAgitGameManager::SetMyAgitOption( std::wstring strName_, UCHAR ucMaxUser_, UCHAR ucOpenType_, std::wstring strPassword_, UCHAR ucInviteType_, std::wstring strProfile_ )
{
    // 옵션 바꾸기 날리기 전에 유효한지 검사하는 부분 필요 
    KP2P::GetInstance()->Send_SetAgitOptionReq( strName_, ucMaxUser_, ucOpenType_, strPassword_, ucInviteType_, strProfile_ );
}

bool KGCAgitGameManager::IsUserInAgit( DWORD dwUserUID )
{
    std::map<DWORD,KAgitUserInfo>::iterator mit = m_mapAgitUser.find( dwUserUID );

    return( mit != m_mapAgitUser.end() );
}

void KGCAgitGameManager::BanUser( DWORD dwBanUserUID )
{
    // 내아지트일때만 다른 유저 추방 가능
    if( m_kAgitInfo.m_dwUID != g_kGlobalValue.m_kUserInfo.dwUID )
        return;

    KP2P::GetInstance()->Send_AgitBanUserReq( dwBanUserUID );

    // 아지트 정보바 갱신
    g_pkAgitOverlayUI->UpdateAgitInfo();
}

void KGCAgitGameManager::OnBanUser( DWORD dwAgitUID )
{
    // 이걸 받는 아이는 집주인에게 강퇴당한것임
    if( m_kAgitInfo.m_dwUID != dwAgitUID )  // 내가 있는 아지트?
        return; 

    g_pStateGame->OnExitGame();
}

void KGCAgitGameManager::SaveAgitObject()
{
    if( m_bWatch )  // 구경중에는 배치는 되지만 저장은 안되게 
        return;

    if( IsPosUpdated() == false )
        return;

	if ( g_kGlobalValue.m_kUserInfo.dwUID != m_kAgitInfo.m_dwUID ) 
		return;

    KP2P::GetInstance()->Send_SetAgitObjectPosReq( m_kObjContainer.GetUpdatedPosList() );
    m_kObjContainer.SetPosUpdated();
}

void KGCAgitGameManager::SetAgitOption( KAgitOption& Info_ )
{
    Info_.m_dwAgitUID = m_kAgitInfo.m_dwUID;
    m_kAgitInfo.SetOption( Info_ );
}

void KGCAgitGameManager::RegisterAgitList( KAgitOption& Info_ )
{
    // 손님은 등록할 일 없다 
    if( g_kGlobalValue.m_kUserInfo.dwUID != m_kAgitInfo.m_dwUID )
        return;

    KP2P::GetInstance()->Send_RegisterAgitPageReq( Info_.m_strName, Info_.m_ucMaxUser, Info_.m_ucOpenType, Info_.m_strPassword );
    m_bRegistAgitList = true;
}

void KGCAgitGameManager::UnRegisterAgitList( void )
{
    if( g_kGlobalValue.m_kUserInfo.dwUID != m_kAgitInfo.m_dwUID )
        return;

    if( false == m_bRegistAgitList )
        return;

    KP2P::GetInstance()->Send_UnregisterAgitPageReq();
    m_bRegistAgitList = false;
}

bool KGCAgitGameManager::IsUseKeyCheck( void )
{
    return g_pkAgitOverlayUI->IsUseKeyCheck();   
}

bool KGCAgitGameManager::ChecKBoundToflowerPot( D3DXVECTOR2& dxvPos )
{
    if( g_kGlobalValue.m_kUserInfo.dwUID != m_kAgitInfo.m_dwUID )
        return false;

    GCITEMUID itemUID = KItem::INVALID_ITEM;
    if( m_kObjContainer.GetFlowerPotInMousePos( dxvPos, itemUID ) == false )
        return false;

    m_prNowPlantPot.first = itemUID;

    if( m_kObjContainer.IsHarvestableFlower( itemUID ) ) {

        D3DXVECTOR3 vPos;
        D3DXMATRIX matBasic;

        g_MyD3D->GetMyPlayer()->Get_BasicMatrix( &matBasic, true, true, false );
        D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3(0,0,0), &matBasic );

        // offset
        float fCharX = g_MyD3D->GetMyPlayer()->GetPosition().x;
        float fFlowerPotX = m_kObjContainer.GetObjectPos( itemUID ).x;

        vPos.x -= ( 0.15f + ( fCharX - fFlowerPotX ) );
        vPos.y -= 0.145f;

        // world coordi-> screen coordi
        float fScreenX = ( vPos.x + 1.0f ) * GC_SCREEN_DIV_WIDTH_HALF + 0.5f;
        float fScreenY = ( -vPos.y + 0.75f ) * GC_SCREEN_DIV_WIDTH_HALF  +0.5f;

        g_pkAgitOverlayUI->ShowHarvestGauge( true, fScreenX, fScreenY );
    }
    else if( m_kObjContainer.IsPlantableFlower( itemUID ) ) {
        // 여서 확인 물어보소 
        if ( m_kObjContainer.GetFlowerPotState( m_prNowPlantPot.first ) == KGCAgitObject::E_FLOWER_STATE_BUD )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString( STR_ID_SEED_PLANT_CONFIRM, "i", m_kObjContainer.GetRemainBloomMinute( m_prNowPlantPot.first ) ), L"", 
                KGCUIScene::GC_MBOX_USE_SEED_PLANT_CONFIRM, 0, 0, true, true );
        }
        else
            g_pkAgitOverlayUI->ShowSeedPlant( true );
    }
    else {
        return false;
    }

    return true;
}

bool KGCAgitGameManager::CheckPlantEnable( void )
{
    KItem* pSeed = NULL;

    // 인벤에 씨앗 있나??
    if( false == g_pItemMgr->FindInventoryForItemID( KGCItemManager::ITEM_AGIT_FLOWER_POT_SEED, &pSeed ) )
        return false;

    // 1개 이상 있나???
    if( pSeed->m_nCount <= 0 )
        return false;

    m_prNowPlantPot.second = pSeed->m_ItemUID;
    return true;
}

void KGCAgitGameManager::ClearNowPlantPot( void )
{
    m_prNowPlantPot.first = 0;
    m_prNowPlantPot.second = 0;
}

bool KGCAgitGameManager::SetFlowerPotInfo( KFlowerPot& info_ )
{
    m_kObjContainer.SetFlowerPotInfo( info_ );
    return true;
}

bool KGCAgitGameManager::SetSeedInfo( KItem* pSeed_ )
{
    KItem* pItem_ = NULL;

    if( false == g_pItemMgr->FindInventoryForItemID( pSeed_->m_ItemID / 10, &pItem_ ) )
        return false;

    pItem_->m_nCount = pSeed_->m_nCount;
    return true;
}

void KGCAgitGameManager::SetUserNameMap( void )
{
    m_mapUserName.clear();

    // 자기 이름도 포함
    m_mapUserName[ g_kGlobalValue.m_kUserInfo.dwUID ] = g_kGlobalValue.m_kUserInfo.strNickName;    

    for( std::map<DWORD,KAgitUserInfo>::iterator mit = m_mapAgitUser.begin() ; mit != m_mapAgitUser.end() ; ++mit )
        m_mapUserName[ mit->first ] = mit->second.m_strNick;
}

std::wstring KGCAgitGameManager::GetOwnerName( void )
{
    if( m_mapUserName.empty() )
        return std::wstring();

    std::map< DWORD, std::wstring >::iterator mit = m_mapUserName.find( m_kAgitInfo.m_dwUID );
    if( mit == m_mapUserName.end() )
        return std::wstring();

    return mit->second;
}

void KGCAgitGameManager::SetMapCatalog( KEVENT_AGIT_MAP_CATALOGUE_ACK& catalog )
{
    m_kMapCatalog.m_PayItemID = 0;
    m_kMapCatalog.m_mapMapList.clear();

    m_kMapCatalog = catalog;
}

void KGCAgitGameManager::SortObjectForY()
{
    std::map< int, std::vector< KGC2DObject* > >::iterator mit;
    for( mit = m_map2DObj.begin() ; mit != m_map2DObj.end() ; ++mit ) {
        std::vector< KGC2DObject* >& vecLayer = mit->second;

        std::sort( vecLayer.begin(), vecLayer.end(), boost::bind( &KGC2DObject::GetPosY, _1 ) < boost::bind( &KGC2DObject::GetPosY, _2 ) );
    }
}

void KGCAgitGameManager::UpdateChatBalloon()
{   
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
	{
		PLAYER* pPlyer = g_MyD3D->MyPlayer[i];
		if ( pPlyer->m_kUserInfo.CheckState( GC_RUS_NONE ) ) continue;

		D3DXVECTOR3 vPos;
		D3DXMATRIX matBasic;


		pPlyer->Get_BasicMatrix( &matBasic, true, true, false );
		D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3(0,0,0), &matBasic );

		// offset
		vPos.x -= 0.5f;
		vPos.y += 0.8f;

		// world coordinates -> screen coordinates
		int fScreenX = int( ( vPos.x + 1.0f ) * 400.0f + 0.5f );
		int fScreenY = int( ( -vPos.y + 0.75f ) * 400.0f + 0.5f );

		g_pkUIScene->m_pkAgitOverlayUI->ReLocateBalloon
			( D3DXVECTOR2( (float)fScreenX, (float)fScreenY ), pPlyer->m_kUserInfo.dwUID);
	}


    for( int i=0 ; i<GC_CHAR_NUM ; ++i ) {
		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER* pMonster = mit->second;

			if( pMonster->GetCharType() == i && pMonster->GetRender() ) {
				D3DXMATRIX matBasic, mat1, mat2;
				if( pMonster->bIsRight ) {
					D3DXMatrixScaling( &matBasic, pMonster->m_fScale, pMonster->m_fScale, pMonster->m_fScale );
				}
				else {
					D3DXMatrixScaling( &mat1, pMonster->m_fScale, pMonster->m_fScale, pMonster->m_fScale );
					D3DXMatrixRotationY( &mat2, 3.1415f );
					D3DXMatrixMultiply( &matBasic, &mat1, &mat2 );
				}

				D3DXMatrixTranslation( &mat1, pMonster->vRenderPos.x - 1.0f, pMonster->vRenderPos.y - 0.48f, 0.5f );
				D3DXMatrixMultiply( &mat2, &matBasic, &mat1 );
				D3DXMatrixMultiply( &matBasic, &mat2, &g_kCamera.m_matCamera );

				D3DXVECTOR3 dxvPos;
				D3DXVec3TransformCoord( &dxvPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &matBasic );

                dxvPos.x += 0.5f;
                dxvPos.y += 1.4f;

				int iScreenX = static_cast<int>( (dxvPos.x + 1.0f) * 400.0f  + 0.5f );
				int iScreenY = static_cast<int>( (-dxvPos.y + 0.75f) * 400.0f  + 0.5f );

				g_pkAgitOverlayUI->ReLocateBalloon( D3DXVECTOR2( static_cast<float>(iScreenX), static_cast<float>(iScreenY) ), 
					pMonster->GetAgitMonCharacterUID() );
				break;
			}


		}
	}
		
		

};

void KGCAgitGameManager::BeforeWatchAgit( void )
{
    SetWatchState( true );

    // 배치정보 저장 하고 다 숨깁니다. 
    m_mapBeforeWatchPos.clear();
    m_kObjContainer.SaveCurrentPosition();
    m_kObjContainer.GetCurrentPosition( m_mapBeforeWatchPos );
    m_kObjContainer.HideObjectAll();

    // 아지트 메인 UI 말고는 다 끕니다. 
    g_pkAgitOverlayUI->CloseAllUI();
}

void KGCAgitGameManager::AfterWatchAgit( void )
{
    SetWatchState( false );

    // 배치 정보 따라 다시 배치해줍니다. 
    for( std::map< GCITEMUID, std::pair<int, int> >::iterator mit = m_mapBeforeWatchPos.begin() ; mit != m_mapBeforeWatchPos.end() ; ++mit )
        m_kObjContainer.ShowObject( mit->first, mit->second.first, mit->second.second );
}

void KGCAgitGameManager::LeaveWatchAgit( void )
{
    std::string strMap = SiKGCAgitGameManager()->GetMapFileName( m_kAgitInfo.m_dwMapID );
    SiKGCLayerManager()->LoadScript(strMap);
    AfterWatchAgit();
}

std::string KGCAgitGameManager::GetMapFileName( DWORD dwMapID )
{
    std::map<DWORD, kMapInfo >::iterator mit = m_mapMapInfo.find( dwMapID );
    if( mit == m_mapMapInfo.end() )
        return std::string();

    return mit->second.m_strMapFile;
}

D3DXVECTOR2 KGCAgitGameManager::GetFairyTreePos( DWORD dwMapID )
{
    std::map< DWORD, kMapInfo>::iterator mit = m_mapMapInfo.find( dwMapID );
    if( mit == m_mapMapInfo.end() )
        return D3DXVECTOR2(-1.f, -1.f);

    return mit->second.m_dxvFairyTreePos;
}

int KGCAgitGameManager::GetMapNameString( DWORD dwMapID )
{
    std::map< DWORD, kMapInfo >::iterator mit = m_mapMapInfo.find( dwMapID );
    if( mit == m_mapMapInfo.end() )
        return 0;

    return mit->second.m_iMapNameString;
}

int KGCAgitGameManager::GetAgitUserNum( void )
{
    // 아지트에 있는 유저 수 
    return static_cast<int>( m_mapAgitUser.size() ) - 1;
}

void KGCAgitGameManager::MoveAgit( void )
{
    g_pkUIScene->DestroyMsgBox();

    // 싹 회수
    ClearPosObject();

    // 현재 아지트 나가고 
    g_pStateGame->OnExitGame(); 

    // 다시 입장~
    EnterMyAgit();
}

bool KGCAgitGameManager::LoadMapInfo( void )
{
    // 맵 정보 읽어온다 
    m_mapMapInfo.clear();

    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "AgitMapInfo.stg" ) == false )
        return false;

    if( SUCCEEDED( luaMgr.BeginTable( "AgitMapList" ) ) )
    {
        int iMapIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iMapIndex++ ) ) ) 
                break;

            DWORD dwMapID;
            kMapInfo tempInfo;

            LUA_GET_VALUE_DEF( 1, dwMapID, 0 );
            LUA_GET_VALUE_DEF( 2, tempInfo.m_strMapFile, "" );
            LUA_GET_VALUE_DEF( 3, tempInfo.m_dxvFairyTreePos.x, 0.0f );
            LUA_GET_VALUE_DEF( 4, tempInfo.m_dxvFairyTreePos.y, 0.0f );
            LUA_GET_VALUE_DEF( 5, tempInfo.m_iMapNameString, 0 );

            m_mapMapInfo[ dwMapID ] = tempInfo;

            luaMgr.EndTable();          

        }
    }
    luaMgr.EndTable();
    return true;
}

bool KGCAgitGameManager::LoadFairyTreeInfo( void )
{
    m_mapFairyTreeInfo.clear();

    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "AgitFairyTreeInfo.stg" ) == false )
        return false;

    if( SUCCEEDED( luaMgr.BeginTable( "FairyTreeList" ) ) )
    {
        int iTreeIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iTreeIndex++ ) ) ) 
                break;

            KFairyTreeInfo tempInfo;

            luaMgr.GetValue( "Level",       tempInfo.m_iLevel );
            luaMgr.GetValue( "Texture",     tempInfo.m_strTree );


            if( SUCCEEDED( luaMgr.BeginTable( "FootHoldData" ) ) )
            {
                int iFootHoldIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iFootHoldIndex++ ) ) )
                        break;

                    KAgitFootHold tempFootHold;

                    LUA_GET_VALUE_DEF( 1, tempFootHold.iPosX, 0 );
                    LUA_GET_VALUE_DEF( 2, tempFootHold.iPosY, 0 );
                    LUA_GET_VALUE_DEF( 3, tempFootHold.dwProp, 0 );

                    tempInfo.m_vecFootHold.push_back( tempFootHold );
                    luaMgr.EndTable();  // 발판 하나 끝 
                }
                luaMgr.EndTable();  // 발판 정보 리스트 끝   
            }

            if( SUCCEEDED( luaMgr.BeginTable( "FruitData" ) ) )
            {
                int iFruitIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iFruitIndex++ ) ) )
                        break;

                    KFruitInfo tempFruit;

                    LUA_GET_VALUE_DEF( 1, tempFruit.m_strTex, "" );
                    LUA_GET_VALUE_DEF( 2, tempFruit.m_dxvPos.x, 0.0f );
                    LUA_GET_VALUE_DEF( 3, tempFruit.m_dxvPos.y, 0.0f );

                    tempInfo.m_vecFruits.push_back( tempFruit );
                    luaMgr.EndTable();  // 과일 하나 끝 
                }
                luaMgr.EndTable();  // 과일  정보 리스트 끝   
            }

            if( SUCCEEDED( luaMgr.BeginTable( "Particle" ) ) )
            {
                int iParticleIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iParticleIndex++ ) ) )
                        break;

                    KAgitObjectParticle tempParticle;

                    LUA_GET_VALUE_DEF( 1, tempParticle.strSquence, "" );
                    LUA_GET_VALUE_DEF( 2, tempParticle.fX, 0.0f );
                    LUA_GET_VALUE_DEF( 3, tempParticle.fY, 0.0f );

                    tempInfo.m_vecParticle.push_back( tempParticle );
                    luaMgr.EndTable();  // 파티클 하나 끝 
                }
                luaMgr.EndTable();  // 파티클 정보 리스트 끝   
            }

            if( SUCCEEDED( luaMgr.BeginTable( "GaugePos" ) ) ) {
                D3DXVECTOR2 tempGaugePos;
                LUA_GET_VALUE_DEF( 1, tempGaugePos.x, 0.0f );
                LUA_GET_VALUE_DEF( 2, tempGaugePos.y, 0.0f );
				tempGaugePos.x *= GC_SCREEN_DIV_WIDTH;
				tempGaugePos.y *= GC_SCREEN_DIV_WIDTH;
                tempInfo.m_dxvGaugePos = tempGaugePos;
                luaMgr.EndTable();
            }
            
            m_mapFairyTreeInfo[ tempInfo.m_iLevel ] = tempInfo;
            luaMgr.EndTable();  // 나무 하나 끝           
        }

    }
    luaMgr.EndTable();  // 나무 정보 리스트 끝 
    return true;
}

void KGCAgitGameManager::SetFairyTreeInfo( KFairyTree& info, DWORD dwUID /*= 0x00000000*/ )
{
    // 나무 정보 있나 보고 
    std::map< int, KFairyTreeInfo >::iterator treeIter = m_mapFairyTreeInfo.find( info.m_nLv );
    //std::map< int, KFairyTreeInfo >::iterator treeIter = m_mapFairyTreeInfo.find( iTreeLevel );
    if( treeIter == m_mapFairyTreeInfo.end() )
        return;

    // 맵 정보 있나 보고 
    std::map< DWORD, kMapInfo >::iterator mapIter = m_mapMapInfo.find( m_kAgitInfo.m_dwMapID );
    if( mapIter == m_mapMapInfo.end() )
        return;

    // 경험치 정보 보고 
    DWORD dwCurExp, dwNextExp;
    if( false == GetFairyTreeExpInfo( info.m_nLv, info.m_dwExp, &dwCurExp, &dwNextExp ) ) {
        return;
    }

    // 앞에거 지우고 정보 세팅 
    ClearFairyTreeInfo();
    m_kFairyTree.m_TreeInfo = info;
    m_kFairyTree.m_p2DObject = new KGC2DObject;
    
    // 나무 텍스쳐 
    m_kFairyTree.m_p2DObject->AddTexture( treeIter->second.m_strTree.c_str() );
    DWORD dwTexWidth = m_kFairyTree.m_p2DObject->GetCurrTex()->GetWidth();
    DWORD dwTexHeight = m_kFairyTree.m_p2DObject->GetCurrTex()->GetHeight();
    float fScaleX = static_cast<float>( dwTexWidth / 20.0f );
    float fScaleY = static_cast<float>( dwTexHeight / 20.0f );

    // 위치 
    D3DXVECTOR2 dxvPos = mapIter->second.m_dxvFairyTreePos;
    m_kFairyTree.m_p2DObject->SetScale( TO_AGIT_COORDI( fScaleX ), TO_AGIT_COORDI( fScaleY ) );
    m_kFairyTree.m_p2DObject->SetPosition( TO_AGIT_COORDI( dxvPos.x ) - TO_AGIT_COORDI( fScaleX * 0.5f ), TO_AGIT_COORDI( dxvPos.y ) );

    // 열매 
    for( std::vector< KFruitInfo >::iterator vit = treeIter->second.m_vecFruits.begin() ; vit != treeIter->second.m_vecFruits.end() ; ++vit )
    {
       KGC2DObject* p2DObj = new KGC2DObject;
       
       // 위치 
       D3DXVECTOR2 dxvFruitPos = vit->m_dxvPos;
       p2DObj->SetPosition( TO_AGIT_COORDI( dxvPos.x ) + TO_AGIT_COORDI( dxvFruitPos.x ), TO_AGIT_COORDI( dxvPos.y ) + TO_AGIT_COORDI( dxvFruitPos.y ) );

       // 열매 텍스쳐
       p2DObj->AddTexture( vit->m_strTex.c_str() );
       dwTexWidth = p2DObj->GetCurrTex()->GetWidth();
       dwTexHeight = p2DObj->GetCurrTex()->GetHeight();
       fScaleX = static_cast<float>( dwTexWidth / 20.0f );
       fScaleY = static_cast<float>( dwTexHeight / 20.0f );
       p2DObj->SetScale( TO_AGIT_COORDI( fScaleX ), TO_AGIT_COORDI( fScaleY ) );
       p2DObj->SetObjectOffsetX( TO_AGIT_COORDI( fScaleX * 0.5f ) );

       m_kFairyTree.m_vecFruits.push_back( p2DObj );
    }

    // 나무 파티클
    std::vector< KAgitObjectParticle >::iterator vit = treeIter->second.m_vecParticle.begin();
    std::vector< KAgitObjectParticle >::iterator end = treeIter->second.m_vecParticle.end();

    float fPosX = m_kFairyTree.m_p2DObject->GetPosX();
    float fPosY = m_kFairyTree.m_p2DObject->GetPosY();

    CParticleEventSeqPTR pParticle = NULL;

    for(  ; vit != end;  ++vit )
    {
        pParticle = g_ParticleManager->CreateSequence( vit->strSquence, fPosX + vit->fX, fPosY + vit->fY, 0.5f );
        g_ParticleManager->SetLayer( pParticle, GC_LAYER_BETWEEN_BLOCK_CHAR );
        
        if ( pParticle ) 
            m_kFairyTree.m_vecParticle.push_back( pParticle );
    }

    // 나무 업그레이드 모션
    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( dwUID );
    if( pPlayer )
    {
        int iMontionID = MID_AZIT_TREEUP_GRIL;
        if( SEX_MAN == g_kGlobalValue.IsSexCharacter( pPlayer->GetCurrentChar().iCharType ) )
            iMontionID = MID_AZIT_TREEUP_MEN;
        pPlayer->SetPlayerState( iMontionID );
        pPlayer->SetFrameIndex( 0 );
    }

    // 내 아지트라면 성장 게이지 표시 
    if( m_kAgitInfo.m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID ) { 
        g_pkAgitOverlayUI->SetFariyTreeLevelGauge( true, m_kFairyTree.m_TreeInfo.m_nLv, dwCurExp, dwNextExp, m_kFairyTree.m_p2DObject->GetPos(), treeIter->second.m_dxvGaugePos );
    }
    else { 
        g_pkAgitOverlayUI->SetFariyTreeLevelGauge( false );
    }
}

void KGCAgitGameManager::ClearFairyTreeInfo( void )
{
    if( m_kFairyTree.m_p2DObject )
    {
        m_kFairyTree.m_p2DObject->RemoveAllTexture();
        SAFE_DELETE( m_kFairyTree.m_p2DObject );
    }

    for( std::vector< KGC2DObject* >::iterator it = m_kFairyTree.m_vecFruits.begin() ; it != m_kFairyTree.m_vecFruits.end() ; ++it )
    {
        if( (*it) )
        {
            (*it)->RemoveAllTexture();
            SAFE_DELETE( (*it) );
        }
    }
    m_kFairyTree.m_vecFruits.clear();

    std::vector< CParticleEventSeqPTR >::iterator vit;
    for( vit = m_kFairyTree.m_vecParticle.begin() ; vit != m_kFairyTree.m_vecParticle.end() ; ++vit )
    {
        g_ParticleManager->DeleteSequence( (*vit) );
        (*vit) = NULL;
    }
    m_kFairyTree.m_vecParticle.clear();
}



//	화분갯수 제한한다.
void KGCAgitGameManager::UpdateCountPos()
{
	int iFlowerPotLimit = 5;
	std::map<DWORD,KAgitMap>::iterator mit = m_kMapCatalog.m_mapMapList.find( m_kAgitInfo.m_dwMapID );

	if( mit != m_kMapCatalog.m_mapMapList.end() ) {
		iFlowerPotLimit = mit->second.m_usMaxFlowerPot;
	}

	std::sort( m_kAgitInfo.m_vecObjectPos.begin(), m_kAgitInfo.m_vecObjectPos.end() );
	
	int nPotCount = 0;

	std::vector<KAgitObjectPos>::iterator pos = m_kAgitInfo.m_vecObjectPos.begin();
	for ( ; pos != m_kAgitInfo.m_vecObjectPos.end(); )
	{
		KAgitObjectPos objPos = *pos;
		if( objPos.m_ID == KGCItemManager::ITEM_AGIT_FLOWER_POT_NORMAL * 10 ) 
		{
			nPotCount++;

			if ( nPotCount > iFlowerPotLimit )
			{
				pos = m_kAgitInfo.m_vecObjectPos.erase( pos );
				continue;
			}
		}

		++pos;
	}
}

void KGCAgitGameManager::SetAgitInfo( KAgitInfo& info_ )
{
    m_kAgitInfo = info_;

    m_kObjContainer.ClearObjectList();

	UpdateCountPos();
	

    if( m_kAgitInfo.m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID )
	{
        // 배치 가능한 오브젝트 목록도 만든다. 
        RefreshInvenObject();
    }

    m_kObjContainer.SetObjectPosList( m_kAgitInfo.m_vecObjectPos, &m_kObjInfoList );
}

GCITEMUID KGCAgitGameManager::GetHideObjectUID( GCITEMID itemID_ )
{
    return m_kObjContainer.GetHideObjecctUID( itemID_ );
}

void KGCAgitGameManager::RemovePickedObject()
{
    m_kObjContainer.HideObject( m_kObjContainer.GetPickedObjUID() );
}

bool KGCAgitGameManager::IsAgitOwner()
{	
	if ( m_kAgitInfo.m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID )
		return true; 

	return false;
}

bool KGCAgitGameManager::CheckBoundToFairyTree( D3DXVECTOR2& dxvPos )
{
    static time_t tmBeforeWatering = 0;

    if( g_kGlobalValue.m_kUserInfo.dwUID != m_kAgitInfo.m_dwUID )
        return false;

    // 웨이트 상태가 아니면 무시
    int iPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayer ) && false == g_MyD3D->MyPlayer[ iPlayer ]->IsWaitMotion() )
        return false;

    // 나무레벨 1, 2, 3이면 충돌 범위를 좀 더 늘려준다. 
    float fXWeight = 0.0f;
    if( m_kFairyTree.m_TreeInfo.m_nLv == 1 )
        fXWeight = 110.0f;
    else if ( m_kFairyTree.m_TreeInfo.m_nLv == 2 )
        fXWeight = 105.0f;
    else if ( m_kFairyTree.m_TreeInfo.m_nLv == 3 )
        fXWeight = 90.0f;

    //if( m_kFairyTree.m_p2DObject != NULL && CheckPickToObjBound( m_kFairyTree.m_p2DObject, dxvPos ) )
    if( m_kFairyTree.m_p2DObject != NULL && CheckPickToObjBound( m_kFairyTree.m_p2DObject, dxvPos, fXWeight ) )
    {
        if( EnableWateringFairyTree() ) 
        {
            D3DXVECTOR4 dxvPickBound;

            dxvPickBound.x = m_kFairyTree.m_p2DObject->GetPos().x;                                              //left
            dxvPickBound.y = m_kFairyTree.m_p2DObject->GetPos().y + m_kFairyTree.m_p2DObject->GetHeight();      //top
            dxvPickBound.z = m_kFairyTree.m_p2DObject->GetPos().x + m_kFairyTree.m_p2DObject->GetWidth();       //right
            dxvPickBound.w = m_kFairyTree.m_p2DObject->GetPos().y;                                              //bottom

            float fCenterX = dxvPickBound.x + ( m_kFairyTree.m_p2DObject->GetWidth() / 2.0f );

            // 물주기 모션 
            bool bRight = false;
            if( fCenterX <= dxvPos.x )  // 캐릭터가 나무 오른쪽이냐 왼쪽이냐 
                bRight = true;

            if( g_MyD3D->IsPlayerIndex( iPlayer ) )
            {
                // 남캐냐 여캐나 
                int iSexCharacter = g_kGlobalValue.IsSexCharacter( g_MyD3D->MyPlayer[ iPlayer ]->GetCurrentChar().iCharType );

                // 나무 레벨 1, 2일때 여캐는 A, 남캐는 B 모션 쓰고, 3~7은 공용으로 C 모션 씁니다. 
                int iMotionID = MID_AZIT_WARERRING_A;
                if( 1 == m_kFairyTree.m_TreeInfo.m_nLv || 2 == m_kFairyTree.m_TreeInfo.m_nLv )
                {
                    if( iSexCharacter == SEX_MAN )
                        iMotionID = MID_AZIT_WARERRING_B;
                }
                else
                    iMotionID = MID_AZIT_WARERRING_C;

                g_MyD3D->MyPlayer[iPlayer]->SetIsRight( !bRight );
                g_MyD3D->MyPlayer[iPlayer]->SetPlayerState( iMotionID );
                g_MyD3D->MyPlayer[iPlayer]->SetFrameIndex( 0 );

                g_KDSound.Play( "984036" );
            }
        }
        else    // 열매 먹기 
            KP2P::GetInstance()->Send_UseFairyTreeFruitReq();
    }

    return false;
}

bool KGCAgitGameManager::IsSameAgit( DWORD dwAigtUID )
{
	if ( m_kAgitInfo.m_dwUID == dwAigtUID )
		return true;

	return false;
}

bool KGCAgitGameManager::EnableWateringFairyTree( void )
{
    //if( m_kFairyTree.m_TreeInfo.m_nLv == 1 || m_kFairyTree.m_TreeInfo.m_nLv == 2 )      // 1, 2 레벨은 열매 없음 -> 물주기 가능 
    //    return true;
    //else if ( m_kFairyTree.m_TreeInfo.m_nFruitType == KFairyTreeConfig::FBT_NONE )      // 열매 없음 -> 물주기 가능
    if ( m_kFairyTree.m_TreeInfo.m_nFruitType == KFairyTreeConfig::FBT_NONE )           // 열매 없음 -> 물주기 가능
        return true;
    else                                                                                // 열매 있는 상태에서는 먹었냐 안먹었냐에 따라 
        return m_kFairyTree.m_TreeInfo.m_bUseFruit;
}

void KGCAgitGameManager::SetFairyTreeBuff( void )
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG)
        return;

    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) || SiKGCChannelManager()->IsPVPBallanceServer() )
        return;

    if( KFairyTreeConfig::FBT_NONE == m_kFairyTree.m_kBuffInfo.m_nFruitType )   // 열매 없음여 
        return;

    if( m_kFairyTree.m_kBuffInfo.m_dwUserUID != g_kGlobalValue.m_kUserInfo.dwUID )  // 내꺼 아님
        return;

    int iBuffLevel = 0;
    switch( m_kFairyTree.m_kBuffInfo.m_nTreeLv ) { 
        case 3: iBuffLevel = 0; break;
        case 4: iBuffLevel = 1; break;
        case 5: iBuffLevel = 2; break;
        case 6: iBuffLevel = 3; break;
        case 7: iBuffLevel = 4; break;

        default:iBuffLevel = 0; break;
    }

    int iPlayerIndex = g_MyD3D->Get_MyPlayer();

    if ( !g_MyD3D->IsPlayerIndex( iPlayerIndex ) )
        return;


    int iCharType = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentCharType();

    // 캐릭터 전용 던전이 있는 캐릭터 일경우 졸업 여부 체크한다.
    if ( SiKGCPrivateDungeonManager()->IsHavePrivateDungeon( iCharType ) && !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( iCharType ) )
        return;

    if( m_kFairyTree.m_kBuffInfo.m_nFruitType != KFairyTreeConfig::FBT_NEW_BUFF ) { 
        return;
    }

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AUTOMATCH ) { 
        g_pMagicEffect->SetMagicEffect( iPlayerIndex, EGC_AGIT_NEW_BUFF_MATCH_LV1 + iBuffLevel, 999999.0f );
    }
    else { 
        g_pMagicEffect->SetMagicEffect( iPlayerIndex, EGC_AGIT_NEW_BUFF_DUNGEON_LV1 + iBuffLevel, 999999.0f );
	}
}

void KGCAgitGameManager::SetFairyTreeBuffInfo( KFairyTreeFruitBuff& buffInfo )
{
    m_kFairyTree.m_kBuffInfo = buffInfo;
    m_kFairyTree.m_TreeInfo.m_bUseFruit = true;
    SetFairyTreeBuff();
}

void KGCAgitGameManager::SetFairyTreeLevelTable( std::map<int, DWORD>& table_ )
{
    if( false == m_mapFairyTreeExpTable.empty() ) { 
        return;
    }

    m_mapFairyTreeExpTable = table_;
}

bool KGCAgitGameManager::GetFairyTreeExpInfo( const int iLevel_, const DWORD dwCurExp, OUT DWORD* pdwCurExp, OUT DWORD* pdwNextExp )
{
    int iNextLv = iLevel_ + 1;
    if( iNextLv > 7 ) { 
        iNextLv = 8;
        return true;
    }

    std::map< int, DWORD >::iterator curIter = m_mapFairyTreeExpTable.find( iLevel_ );
    std::map< int, DWORD >::iterator nextIter = m_mapFairyTreeExpTable.find( iNextLv );

    if( curIter == m_mapFairyTreeExpTable.end() || nextIter == m_mapFairyTreeExpTable.end() ) { 
        return false;
    }

    *pdwCurExp = ( dwCurExp - curIter->second );
    *pdwNextExp = ( nextIter->second - curIter->second );
    return true;
}

bool KGCAgitGameManager::AddDummyObject( GCITEMID itemID_ )
{
    return m_kObjContainer.AddDummyObject( itemID_, &m_kObjInfoList );
}

void KGCAgitGameManager::RemoveDummyObject()
{
    m_kObjContainer.RemoveDummyObject();
}

void KGCAgitGameManager::BuyDummyObject()
{
    m_kObjContainer.BuyDummyObject();
    SaveAgitObject();
}

void KGCAgitGameManager::SetPrePlace( bool bSet )
{
    m_bPrePlace = bSet;
}

bool KGCAgitGameManager::IsHideObject( GCITEMUID itemUID_ )
{
    return m_kObjContainer.IsHideObject( itemUID_ );
}

void KGCAgitGameManager::SetMaxChannel( int nMaxChannel_ )
{
	g_pkAgitOverlayUI->SetMaxChannel( nMaxChannel_ );
}

void KGCAgitGameManager::RollbackObjectPos()
{
    m_kObjContainer.RollbackPosition();
}

void KGCAgitGameManager::UpdateLatencyInfo( const LTP_AGIT_OBJECT_POS& kPacket )
{
    m_kObjContainer.UpdateLatencyInfo( kPacket );
}

int KGCAgitGameManager::GetInvenItemCount( GCITEMID itemID_ )
{
    KItem* pItem = NULL;
    g_pItemMgr->FindInventoryForItemID( itemID_ / 10 , &pItem );
    if( NULL == pItem ) { 
        return 0;
    }
    
    // 수량이냐 아니냐에 따라 
    if( pItem->m_nCount != KItem::UNLIMITED_ITEM ) { 
        return pItem->m_nCount;
    }

    return m_kObjContainer.GetItemCount( itemID_ );
}

int KGCAgitGameManager::GetCashItemLimitCount( GCITEMID itemID_ )
{
    std::map< GCITEMID, int >::iterator mit = m_mapCashItemLimitCount.find( itemID_ );
    if( mit == m_mapCashItemLimitCount.end() ) { 
        return 0;
    }

    return mit->second;
}

void KGCAgitGameManager::UpdateAgitVisitorCount( KEVENT_AGIT_VISITER_COUNT_BROAD countInfo )
{
    // 지금 있는 아지트??
    if( m_kAgitInfo.m_dwUID != countInfo.m_dwAgitUID )
        return;

    // 갱신
    m_kAgitInfo.m_nTodayVisiter = countInfo.m_nTodayVisiterCount;
    m_kAgitInfo.m_nTotalVisiter = countInfo.m_nTotalVisiterCount;

    g_pkAgitOverlayUI->UpdateAgitInfo();
}

void KGCAgitGameManager::SetAgitEditState( bool bEdit )
{
    m_bAgitEdit = bEdit;
    if( m_bAgitEdit ) {
        m_kObjContainer.GetCurrentPosition( m_mapTranjection );
    }
}

void KGCAgitGameManager::RollBackBeforeEdit( void )
{
    m_kObjContainer.HideObjectAll();
    for( std::map< GCITEMUID, std::pair<int, int> >::iterator mit = m_mapTranjection.begin() ; mit != m_mapTranjection.end() ; ++mit )
        m_kObjContainer.ShowObject( mit->first, mit->second.first, mit->second.second );
}

bool KGCAgitGameManager::CheckBoundToGuestBook( D3DXVECTOR2& dxvPos )
{
    GCITEMUID itemUID = KItem::INVALID_ITEM;
    if( m_kObjContainer.CheckBoundToMousePos( dxvPos, static_cast<GCITEMID>( KGCItemManager::ITEM_AGIT_GUEST_BOOK ), itemUID ) == false )
        return false;

    g_pkAgitOverlayUI->ShowGuestBook( true );
    return true;
}

int KGCAgitGameManager::ConvertMonsterIDToCharType( int iCharType )
{
    switch ( iCharType )
    {
    case GC_CHAR_ELESIS:
        return MON_AGIT_BREAK_ELESIS;
    case GC_CHAR_LIRE:
        return MON_AGIT_BREAK_LIRE;
    case GC_CHAR_ARME:
        return MON_AGIT_BREAK_ARME;
    case GC_CHAR_LAS:
        return MON_AGIT_BREAK_LAS;
    case GC_CHAR_RYAN:
        return MON_AGIT_BREAK_RYAN;
    case GC_CHAR_RONAN:
        return MON_AGIT_BREAK_RONAN;
    case GC_CHAR_AMY :
        return MON_AGIT_BREAK_AMY;
    case GC_CHAR_JIN :
        return MON_AGIT_BREAK_JIN;
    case GC_CHAR_SIEG:
        return MON_AGIT_BREAK_SIEG;
    case GC_CHAR_MARI:
        return MON_AGIT_BREAK_MARI;
    case GC_CHAR_DIO:
        return MON_AGIT_BREAK_DIO;
    case GC_CHAR_ZERO:
        return MON_AGIT_BREAK_ZERO;
    case GC_CHAR_LEY:
        return MON_AGIT_BREAK_LEY;
    case GC_CHAR_LUPUS:
        return MON_AGIT_BREAK_LUPUS;
    case GC_CHAR_RIN:
        return MON_AGIT_BREAK_RIN;
    case GC_CHAR_ASIN:
        return MON_AGIT_BREAK_ASIN;
    case GC_CHAR_LIME:
        return MON_AGIT_BREAK_LIME;
    case GC_CHAR_EDEL:
        return MON_AGIT_BREAK_EDEL;
    case GC_CHAR_BEIGAS:
        return MON_AGIT_BREAK_BEIGAS;
	case GC_CHAR_UNO:
		return MON_AGIT_BREAK_UNO;
#ifdef PERYTON // Kawan>
    case GC_CHAR_PERYTON:
        return MON_AGIT_BREAK_PERYTON;
#endif
#ifdef ALICE // Kawan>
    case GC_CHAR_ALICE:
        return MON_AGIT_BREAK_ALICE;
#endif
#ifdef DECANEE // Kawan>
    case GC_CHAR_DECANEE:
        return MON_AGIT_BREAK_DECANEE;
#endif
    }
    return 0;
}

bool KGCAgitGameManager::CheckBoundToTraining( D3DXVECTOR2& dxvPos )
{
    int iPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayer ) )
    {
        if( false == g_MyD3D->MyPlayer[ iPlayer ]->IsWaitMotion() )
            return false;

        if( false == g_MyD3D->MyPlayer[ iPlayer]->GetRender() )
            return false;
    }

    GCITEMUID itemUID = KItem::INVALID_ITEM;
    if( m_kObjContainer.CheckBoundToMousePos( dxvPos, static_cast<GCITEMID>( KGCItemManager::ITEM_AGIT_TRAINING ), itemUID )== false )
        return false;

    g_pkAgitOverlayUI->ShowTrainingUI( true, itemUID );
    return true;
}

bool KGCAgitGameManager::SetTrainingObjInfo( KTrainingObj& info_, bool ImgUpdate_ )
{
    m_kObjContainer.SetTrainingObjInfo( info_, ImgUpdate_);
    return true;
}

int KGCAgitGameManager::GetBrokenTrainingTile( GCITEMUID itemUID )
{
    return m_kObjContainer.GetBrokenTrainingTile( itemUID );
}

D3DXVECTOR2 KGCAgitGameManager::GetTrainingObjPos( GCITEMUID itemUID )
{
    return m_kObjContainer.GetTrainigObjPos( itemUID );
}

void KGCAgitGameManager::ClearTrainingTileImg( GCITEMUID itemUID )
{
    m_kObjContainer.ClearTrainingTileImg( itemUID );
}


void KGCAgitGameManager::InitAgitCharacter( void )
{
    for( std::map< int, KAgitCharacterPos >::iterator mit = m_kAgitInfo.m_mapCharacterPos.begin() ; mit != m_kAgitInfo.m_mapCharacterPos.end() ; ++mit ) {
        
        if( -1 == mit->second.m_nPosX && -1 == mit->second.m_nPosY ) {
            continue;
        }

        float fPosX = mit->second.m_nPosX * 0.001f;
        float fPosY = mit->second.m_nPosY * 0.001f;

        CreatePlaceCharacter( mit->first, fPosX, fPosY );
    }
}

void KGCAgitGameManager::DestroyAgitCharacter( void )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( true == pMonster->IsLive() ) { 
			int iSaybaloonUID = pMonster->GetAgitMonCharacterUID();
			pMonster->EndMonster();
			pMonster->Release();
			pMonster->m_bLive = false;
			pMonster->m_bRender = false;
			g_pkAgitOverlayUI->DeleteBalloonMsg( iSaybaloonUID );
		}
	}
}

bool KGCAgitGameManager::PickUpCharacter( const D3DXVECTOR2& dxvMousePos )
{
    D3DXVECTOR2 vecPickPos = dxvMousePos;
    g_MyD3D->MyCtrl->Picking( vecPickPos );

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		int iNumPushRect = pMonster->GetPushRectNum();

		for( int j=0 ; j<iNumPushRect ; ++j ) {
			GCCollisionRect<float> rect = pMonster->GetPushBoundRect( j, true );
			rect.m_Left  += 1.0f;
			rect.m_Right += 1.0f;
			rect.m_Top   += 0.25f;
			rect.m_Bottom+= 0.25f;

			if( true == rect.PtInRect( vecPickPos.x, vecPickPos.y ) ) {
				m_iPickCharIndex = mit->first;
				return true;
			}
		}
	}

    return false;
}

bool KGCAgitGameManager::PickUpCharacter( const int iCharType, const D3DXVECTOR2& dxvMousePos )
{
    std::map< int, KAgitCharacterPos >::iterator mit =  m_kAgitInfo.m_mapCharacterPos.find( iCharType );
    if( mit == m_kAgitInfo.m_mapCharacterPos.end() ) {
        return false;
    }

    D3DXVECTOR2 vecPickPos = dxvMousePos;
    g_MyD3D->MyCtrl->Picking( vecPickPos );
    m_iPickCharIndex = CreatePlaceCharacter( iCharType, vecPickPos.x, vecPickPos.y ); 
    g_pkAgitOverlayUI->GetCharSetting()->InitUI(iCharType);
    g_pkAgitOverlayUI->GetCharSetting()->SaveCharacterMent();
    return true;
}

void KGCAgitGameManager::UpdatePickedCharacterPos( const D3DXVECTOR2& dxvMousePos )
{
	std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.find(m_iPickCharIndex);
	if( mit == g_kMonsterManager.m_mapMonster.end() ) return;
	MONSTER* pMonster = mit->second;

    pMonster->SetX( dxvMousePos.x );
    pMonster->SetY( dxvMousePos.y );
}

void KGCAgitGameManager::SetCharacterPickOff( void )
{
    m_iPickCharIndex = -1;
}

void KGCAgitGameManager::SetCharacterObject( const std::map<int, KAgitCharacterPos>& mapInfo )
{
    if( mapInfo.empty() ) { 
        return;
    }

    m_kAgitInfo.m_mapCharacterPos.clear();
    m_kAgitInfo.m_mapCharacterPos = mapInfo;
}

int KGCAgitGameManager::GetAgitCharacterMonType( int iCharType )
{
    switch( iCharType ) {
        case GC_CHAR_ELESIS:
            return MON_AGIT_PLACE_CHAR_ELESIS;
        case GC_CHAR_LIRE:
            return MON_AGIT_PLACE_CHAR_LIRE;
        case GC_CHAR_ARME:
            return MON_AGIT_PLACE_CHAR_ARME;
        case GC_CHAR_LAS:
            return MON_AGIT_PLACE_CHAR_LAS;
        case GC_CHAR_RYAN:
            return MON_AGIT_PLACE_CHAR_RYAN;
        case GC_CHAR_RONAN:
            return MON_AGIT_PLACE_CHAR_RONAN;
        case GC_CHAR_AMY:
            return MON_AGIT_PLACE_CHAR_AMY;
        case GC_CHAR_JIN:
            return MON_AGIT_PLACE_CHAR_JIN;
        case GC_CHAR_SIEG:
            return MON_AGIT_PLACE_CHAR_SIEG;
        case GC_CHAR_MARI:
            return MON_AGIT_PLACE_CHAR_MARI;
        case GC_CHAR_DIO:
            return MON_AGIT_PLACE_CHAR_DIO;
        case GC_CHAR_ZERO:
            return MON_AGIT_PLACE_CHAR_ZERO;
        case GC_CHAR_LEY:
            return MON_AGIT_PLACE_CHAR_LEY;
        case GC_CHAR_LUPUS:
            return MON_AGIT_PLACE_CHAR_LUPUS;
        case GC_CHAR_RIN:
            return MON_AGIT_PLACE_CHAR_RIN;
        case GC_CHAR_ASIN:
            return MON_AGIT_PLACE_CHAR_ASIN;
        case GC_CHAR_LIME:
            return MON_AGIT_PLACE_CHAR_LIME;
        case GC_CHAR_EDEL:
            return MON_AGIT_PLACE_CHAR_EDEL;
        case GC_CHAR_BEIGAS:
            return MON_AGIT_PLACE_CHAR_BEIGAS;
		case GC_CHAR_UNO:
			return MON_AGIT_PLACE_CHAR_UNO; 
#ifdef PERYTON // Kawan>
        case GC_CHAR_PERYTON:
            return MON_AGIT_PLACE_CHAR_PERYTON;
#endif
#ifdef ALICE // Kawan>
        case GC_CHAR_ALICE:
            return MON_AGIT_PLACE_CHAR_ALICE;
#endif
#ifdef DECANEE // Kawan>
        case GC_CHAR_DECANEE:
            return MON_AGIT_PLACE_CHAR_DECANEE;
#endif
        default:
            return MON_AGIT_PLACE_CHAR_ELESIS;
    }
}

int KGCAgitGameManager::CreatePlaceCharacter( const int iCharType, float fPosX, float fPosY )
{
    // 배치된거 있으면 없애고 
    DeletePlaceCharacter( iCharType );

    // 위치 
    float fX = fPosX / SiKGCFantasticMap()->GetPixelWidth();
    float fY = fPosY;

    // 추가 
    int iMonCursor = g_MyD3D->AddMonster( GetAgitCharacterMonType( iCharType ), fX, fY, true, false, 0, -1, 0x00000000, true, true );

	MONSTER *pMonster = g_kMonsterManager.GetMonster(iMonCursor);

	if ( pMonster == NULL ) return -1;

    pMonster->m_bLive = true;
    pMonster->m_bRender = true;
    pMonster->SetMonsterKind( MONSTER::EMON_AGIT_CHAR );
    pMonster->InitBuffer( fPosX, fPosY, true );
	pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;

    // 모션 세팅 
    std::string strState;
    if( true == GetAgitCharacterState( iCharType, m_kAgitInfo.m_mapCharacterPos[iCharType].m_nMotionID, strState ) ) {
        pMonster->SetState( strState.c_str() );
    }

    // 아이템 세팅 
    std::vector< GCITEMID >  vecEquipID;
    std::vector< GCITEMUID > vecEquipUID;
    std::map<DWORD,KSimpleCoordiItem> mapEquip = m_kAgitInfo.m_mapCharacterPos[iCharType].m_mapCoordi;
    for( std::map<DWORD,KSimpleCoordiItem>::iterator mit = mapEquip.begin() ; mit != mapEquip.end() ; ++mit ) { 
        
        // 내 아지트 일때 
        if( m_kAgitInfo.m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID ) { 
            // 내 인벤토리에 있나 / 현재 코디팀인가 한번 확인 해주세영
            KSimpleCoordiItem kSimpleCoordi = mit->second;

            GCITEMID itemID = kSimpleCoordi.m_ItemID;
            if ( kSimpleCoordi.m_DesignCoordiID != 0 )
            {
                itemID = kSimpleCoordi.m_DesignCoordiID;
            }

            vecEquipID.push_back( itemID / 10 );
            vecEquipUID.push_back( kSimpleCoordi.m_ItemUID );
        }
        else {
            KSimpleCoordiItem kSimpleCoordi = mit->second;
            GCItem* pItem = g_pItemMgr->GetItemData( kSimpleCoordi.m_ItemID / 10 );
            if( NULL == pItem ) {
                continue;
            }

            GCITEMID itemID = kSimpleCoordi.m_ItemID;
            if ( kSimpleCoordi.m_DesignCoordiID != 0 )
            {
                itemID = kSimpleCoordi.m_DesignCoordiID;
            }

            vecEquipID.push_back( itemID / 10 );
        }
    }

    // 내 아지트 일 때 코디 정보 달라진거 있으면 저장정보 새로 보냅니다. 
    if( m_kAgitInfo.m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID ) {
        std::map< DWORD, GCITEMUID > mapNewEquipInfo;
        if( mapEquip.size() != vecEquipUID.size() ) {
            for( std::vector< GCITEMUID >::iterator vit = vecEquipUID.begin() ; vit != vecEquipUID.end() ; ++vit ) { 
                GCItem* pItem = NULL;
                g_pItemMgr->FindInventory( *vit, &pItem );

                mapNewEquipInfo[ pItem->dwSlotPosition ] = *vit;
            }
            // 캐릭터분리에서는 다른 캐릭 인벤을 안가지고 있다. 서버랑 동기화 하지 않도록 한다.
            //KP2P::GetInstance()->Send_SetAgitCharCoordiReq( iCharType, mapNewEquipInfo );
        }
    }
    pMonster->SetItemList( vecEquipID );

    // UID
    pMonster->SetAgitMonCharUID( GetPlaceCharacterUID() );

    int iNumPushRect = pMonster->GetPushRectNum();
    for( int i=0 ; i<iNumPushRect ; ++i ) {
        pMonster->SetPushCheck( false, i );
    }

    return iMonCursor;
}

void KGCAgitGameManager::RemovePickCharacter( void )
{
    if( -1 == m_iPickCharIndex ) { 
        return;
    }
	std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.find(m_iPickCharIndex);
	if( mit == g_kMonsterManager.m_mapMonster.end() ) return;
	MONSTER* pMonster = mit->second;

    m_kAgitInfo.m_mapCharacterPos[ pMonster->GetCharType() ].m_nPosX = -1;
    m_kAgitInfo.m_mapCharacterPos[ pMonster->GetCharType() ].m_nPosY = -1;

    int iSaybaloonUID = pMonster->GetAgitMonCharacterUID();
    pMonster->EndMonster();
    pMonster->Release();
    pMonster->m_bLive = false;
    pMonster->m_bRender = false;

    g_pkAgitOverlayUI->DeleteBalloonMsg( iSaybaloonUID );
    m_iPickCharIndex = -1;
}

void KGCAgitGameManager::DeletePlaceCharacter( const int iCharType )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pMonster->GetCharType() == iCharType ) { 

			m_kAgitInfo.m_mapCharacterPos[ pMonster->GetCharType() ].m_nPosX = -1;
			m_kAgitInfo.m_mapCharacterPos[ pMonster->GetCharType() ].m_nPosY = -1;

			int iSaybaloonUID = pMonster->GetAgitMonCharacterUID();
			pMonster->EndMonster();
			pMonster->Release();
			pMonster->m_bLive = false;
			pMonster->m_bRender = false;
			g_pkAgitOverlayUI->DeleteBalloonMsg( iSaybaloonUID );
			break;
		}

	}
}

void KGCAgitGameManager::SaveAgitCharacterPos( void )
{
    // 보유 캐릭터 정보 
    std::vector< int > vecCharType;
    for( std::map< int, KAgitCharacterPos >::iterator mit = m_kAgitInfo.m_mapCharacterPos.begin() ; mit != m_kAgitInfo.m_mapCharacterPos.end() ; ++mit ) {
        vecCharType.push_back( mit->first );
    }

    // 보유 캐릭터들 위치 파악
    std::vector< KSimpleAgitCharPos > vecCharPos;
    for( std::vector< int >::iterator vit = vecCharType.begin() ; vit != vecCharType.end() ; ++vit ) {

        bool bPlace = false;
        KSimpleAgitCharPos charPos;
		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER* pMonster = mit->second;
			if( pMonster->GetCharType() == (*vit) ) {
				charPos.m_nCharType = *vit;
				//charPos.m_nPosX = static_cast<int>( g_kMonsterManager.m_mapMonster[i]->GetX() * 20.0f );
				//charPos.m_nPosY = static_cast<int>( g_kMonsterManager.m_mapMonster[i]->GetY() * 20.0f );
				charPos.m_nPosX = static_cast<int>( pMonster->GetX() * 1000.0f );
				charPos.m_nPosY = static_cast<int>( pMonster->GetY() * 1000.0f );
				vecCharPos.push_back( charPos );
				bPlace = true;
				break;
			}

		}

        if( false == bPlace ) {
            charPos.m_nCharType = *vit;
            charPos.m_nPosX = -1;
            charPos.m_nPosY = -1;
            vecCharPos.push_back( charPos );
        }
        
    }

    // 저장
    KP2P::GetInstance()->Send_SetAgitCharPosReq( vecCharPos );
}

void KGCAgitGameManager::UpdateAgitCharacterInfo( KAgitCharacterPos& Info_ )
{
    m_kAgitInfo.m_mapCharacterPos[ Info_.m_nCharType ] = Info_;

    SetAgitCharacterPos( Info_.m_nCharType, Info_.m_nPosX, Info_.m_nPosY );
    UpdateAgitCharacterMotion( Info_.m_nCharType );
    UpdateAgitCharacterEquip( Info_ );
}

void KGCAgitGameManager::UpdateAgitCharacterMotion( int iCharType_ )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pMonster->GetCharType() == iCharType_ ) { 
			// 모션 세팅 
			std::string strState;
			if( true == GetAgitCharacterState( iCharType_, m_kAgitInfo.m_mapCharacterPos[iCharType_].m_nMotionID, strState ) ) {
				pMonster->SetState( strState.c_str() );
			}
			break;
		}
	}
}

KAgitCharacterPos KGCAgitGameManager::GetAgitCharacter( int iCharType_ )
{
    return m_kAgitInfo.m_mapCharacterPos[ iCharType_ ];
}

void KGCAgitGameManager::RenderPickCharacter( void )
{
    //if( -1 == m_iPickCharIndex ) { 
    //    return;
    //}

    //g_kMonsterManager.m_mapMonster[ m_iPickCharIndex ]->m_pObject->Render();
}

bool KGCAgitGameManager::LoadAgitCharacterMotionInfo( void )
{
    m_vecCharacterMotionInfo.clear();

    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "AgitNpcInfo.stg" ) == false )
        return false;

    if( SUCCEEDED( luaMgr.BeginTable( "AGIT_NPC_ACTION_LIST" ) ) )
    {
        int iActionIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iActionIndex++ ) ) ) 
                break;

            KAgitCharacterMotion tempInfo;

            luaMgr.GetValue( "Index",       tempInfo.m_iIndex );
            luaMgr.GetValue( "StringID",    tempInfo.m_iStringID );


            if( SUCCEEDED( luaMgr.BeginTable( "MotionMan" ) ) )
            {
                int iManMotionIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iManMotionIndex++ ) ) )
                        break;

                    std::string strState, strMotion;

                    LUA_GET_VALUE_DEF( 1, strState, "" );
                    LUA_GET_VALUE_DEF( 2, strMotion, "" );

                    tempInfo.m_vecManCharMotion.push_back( std::make_pair( strState, strMotion ) );
                    luaMgr.EndTable(); 
                }
                luaMgr.EndTable();
            }

            if( SUCCEEDED( luaMgr.BeginTable( "MotionGirl" ) ) )
            {
                int iGirlMotionIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iGirlMotionIndex++ ) ) )
                        break;

                    std::string strState, strMotion;

                    LUA_GET_VALUE_DEF( 1, strState, "" );
                    LUA_GET_VALUE_DEF( 2, strMotion, "" );

                    tempInfo.m_vecGirlCharMotion.push_back( std::make_pair( strState, strMotion ) );
                    luaMgr.EndTable(); 
                }
                luaMgr.EndTable(); 
            }

            m_vecCharacterMotionInfo.push_back( tempInfo );
            luaMgr.EndTable(); 
        }
    }
    luaMgr.EndTable();
    return true;
}

int KGCAgitGameManager::GetCharacterMotionID( int iCharType_ )
{
    std::map<int,KAgitCharacterPos>::iterator mit = m_kAgitInfo.m_mapCharacterPos.find( iCharType_ );
    if( mit == m_kAgitInfo.m_mapCharacterPos.end() ) {
        return -1;
    }

    return mit->second.m_nMotionID;
}

bool KGCAgitGameManager::GetAgitCharacterState( IN int iCharType_, IN int iMotionID_, OUT std::string& strState_ )
{
    if( iMotionID_ < 0 || iMotionID_ > static_cast<int>( m_vecCharacterMotionInfo.size() ) ) {
        return false;
    }

    int iCharSex = g_kGlobalValue.IsSexCharacter( iCharType_ );
    
    if( SEX_NONE == iCharSex ) { 
        return false;
    }

    if( SEX_MAN == iCharSex ) {
        strState_ = m_vecCharacterMotionInfo[ iMotionID_ ].m_vecManCharMotion[0].first;
    }
    else {
        strState_ = m_vecCharacterMotionInfo[ iMotionID_ ].m_vecGirlCharMotion[0].first;
    }

    return true;
}

bool KGCAgitGameManager::GetAgitCharacterMotion( IN int iCharType_, IN int iMotionID_, OUT std::string& strMotion_ )
{
    if( iMotionID_ < 0 || iMotionID_ > static_cast<int>( m_vecCharacterMotionInfo.size() ) ) {
        return false;
    }

    int iCharSex = g_kGlobalValue.IsSexCharacter( iCharType_ );

    if( SEX_NONE == iCharSex ) { 
        return false;
    }

    if( SEX_MAN == iCharSex ) {
        strMotion_ = m_vecCharacterMotionInfo[ iMotionID_ ].m_vecManCharMotion[0].second;
    }
    else {
        strMotion_ = m_vecCharacterMotionInfo[ iMotionID_ ].m_vecGirlCharMotion[0].second;
    }

    return true;
}

std::wstring KGCAgitGameManager::GetAgitCharacterMent( int iCharType_, int iSpeechType_ )
{
    std::map<int,KAgitCharacterPos>::iterator charIter = m_kAgitInfo.m_mapCharacterPos.find( iCharType_ );
    if( charIter == m_kAgitInfo.m_mapCharacterPos.end() ) {
        return std::wstring();
    }

    std::map<USHORT,std::wstring>::iterator speechIter = charIter->second.m_mapSpeech.find( static_cast<USHORT>( iSpeechType_ ) );
    if( speechIter == charIter->second.m_mapSpeech.end() ) {
        return std::wstring();
    }

    return speechIter->second;
}

void KGCAgitGameManager::SetAgitCharacterSayBaloon( int iCharType_, int iUID_, KAgitCharacterPos::SPEECH_TYPE eMentType_ )
{
    std::wstring strMent = GetAgitCharacterMent( iCharType_, eMentType_ );
    if( !strMent.empty() ) {
        g_pkAgitOverlayUI->DeleteBalloonMsg( iUID_ );
        g_pkAgitOverlayUI->SetBalloonMsg( strMent, iUID_, KChatData::MSG_COMMON, 0xffffffff );
    }
}

std::pair<int, int> KGCAgitGameManager::CheckPickToAgitChar( D3DXVECTOR2& dxvMousePos_ )
{
    D3DXVECTOR2 vecPickPos = dxvMousePos_;
    g_MyD3D->MyCtrl->Picking( vecPickPos );

    std::pair<int, int> ptResult(-1, -1);
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		int iNumPushRect = pMonster->GetPushRectNum();

		for( int j=0 ; j<iNumPushRect ; ++j ) {
			GCCollisionRect<float> rect = pMonster->GetPushBoundRect( j, true );
			rect.m_Left  += 1.0f;
			rect.m_Right += 1.0f;
			rect.m_Top   += 0.25f;
			rect.m_Bottom+= 0.25f;

			if( true == rect.PtInRect( vecPickPos.x, vecPickPos.y ) ) {
				ptResult.first = pMonster->GetAgitMonCharacterUID();
				ptResult.second = pMonster->GetCharType();
				return ptResult;
			}
		}

	}

    return ptResult;
}

bool KGCAgitGameManager::CheckEnableSitDownObject( D3DXVECTOR2& dxvPos_ )
{
    return m_kObjContainer.CheckEnableSitDownObject( dxvPos_ );
}

bool KGCAgitGameManager::IsOpenAgitCharacter( const int iCharType_ )
{
    std::map< int, KAgitCharacterPos >::iterator mit = m_kAgitInfo.m_mapCharacterPos.find( iCharType_ );

    if( mit == m_kAgitInfo.m_mapCharacterPos.end() ) { 
        return false;
    }

    return true;
}

int KGCAgitGameManager::GetPlaceCharacterUID( void )
{
    return m_iPlaceCharUID++;
}

void KGCAgitGameManager::ClearPlaceCharUID( void )
{
    m_iPlaceCharUID = 0;
}

void KGCAgitGameManager::UpdateAgitCharacterPos( const std::vector<KSimpleAgitCharPos>& vecPos_ )
{
    for( std::vector< KSimpleAgitCharPos >::const_iterator vit = vecPos_.begin() ; vit != vecPos_.end() ; ++vit ) { 
        std::map<int,KAgitCharacterPos>::iterator mit = m_kAgitInfo.m_mapCharacterPos.find( vit->m_nCharType );
        if( mit == m_kAgitInfo.m_mapCharacterPos.end() ) { 
            continue;
        }

        if( mit->second.m_nPosX == vit->m_nPosX &&
            mit->second.m_nPosY == vit->m_nPosY ) {
                continue;
        }

        int iBeforeX = mit->second.m_nPosX;
        int iBeforeY = mit->second.m_nPosY;

        mit->second.m_nPosX = vit->m_nPosX;
        mit->second.m_nPosY = vit->m_nPosY;

        if( m_kAgitInfo.m_dwUID != g_kGlobalValue.m_kUserInfo.dwUID ) { 
            float fX = mit->second.m_nPosX * 0.001f;
            float fY = mit->second.m_nPosY * 0.001f;
            CreatePlaceCharacter( mit->second.m_nCharType, fX, fY );
        }
    }

    for( std::map<int, KAgitCharacterPos >::iterator mit = m_kAgitInfo.m_mapCharacterPos.begin() ; mit != m_kAgitInfo.m_mapCharacterPos.end() ; ++mit ) { 
        if( -1 == mit->second.m_nPosX || -1 == mit->second.m_nPosY ) { 
            DeletePlaceCharacter( mit->first );
            continue;
        }
    }
}

void KGCAgitGameManager::UpdateAgitCharacterEquip( KAgitCharacterPos& Info_ )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pMonster->GetCharType() == Info_.m_nCharType ) {
			float fX = Info_.m_nPosX * 0.001f;
			float fY = Info_.m_nPosY * 0.001f;

			int iSaybaloonUID = pMonster->GetAgitMonCharacterUID();
			pMonster->EndMonster();
			pMonster->Release();
			pMonster->m_bLive = false;
			pMonster->m_bRender = false;
			g_pkAgitOverlayUI->DeleteBalloonMsg( iSaybaloonUID );

			if( Info_.m_nPosX != -1 && Info_.m_nPosY != -1 ) { 
				CreatePlaceCharacter( Info_.m_nCharType, fX, fY );
			}
			break;
		}
	}
}

void KGCAgitGameManager::SetAgitCharacterPos( const int iCharType_, const int iPosX_, const int iPosY_ )
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pMonster->GetCharType() == iCharType_ ) { 

			if( iPosX_ == -1 || iPosY_ == -1 ) { 
				DeletePlaceCharacter( iCharType_ );
			}

			float fX = TO_AGIT_COORDI( iPosX_ );
			float fY = TO_AGIT_COORDI( iPosY_ );

			pMonster->SetX( fX );
			pMonster->SetY( fY );

			break;
		}

	}
}

int KGCAgitGameManager::GetCharTypeFromAgitMon( int iMon_ )
{
    switch( iMon_ ) {        
        case MON_AGIT_PLACE_CHAR_ELESIS:
            return GC_CHAR_ELESIS;        
        case MON_AGIT_PLACE_CHAR_LIRE:
            return GC_CHAR_LIRE;        
        case MON_AGIT_PLACE_CHAR_ARME:
            return GC_CHAR_ARME;        
        case MON_AGIT_PLACE_CHAR_LAS:
            return GC_CHAR_LAS;        
        case MON_AGIT_PLACE_CHAR_RYAN:
            return GC_CHAR_RYAN;        
        case MON_AGIT_PLACE_CHAR_RONAN:
            return GC_CHAR_RONAN;        
        case MON_AGIT_PLACE_CHAR_AMY:
            return GC_CHAR_AMY;        
        case MON_AGIT_PLACE_CHAR_JIN:
            return GC_CHAR_JIN;        
        case MON_AGIT_PLACE_CHAR_SIEG:
            return GC_CHAR_SIEG;        
        case MON_AGIT_PLACE_CHAR_MARI:
            return GC_CHAR_MARI;        
        case MON_AGIT_PLACE_CHAR_DIO:
            return GC_CHAR_DIO;        
        case MON_AGIT_PLACE_CHAR_ZERO:
            return GC_CHAR_ZERO;
        case MON_AGIT_PLACE_CHAR_LEY:
            return GC_CHAR_LEY;
        case MON_AGIT_PLACE_CHAR_LUPUS:
            return GC_CHAR_LUPUS;
        case MON_AGIT_PLACE_CHAR_RIN:
            return GC_CHAR_RIN;
        case MON_AGIT_PLACE_CHAR_ASIN:
            return GC_CHAR_ASIN;
        case MON_AGIT_PLACE_CHAR_LIME:
            return GC_CHAR_LIME;
        case MON_AGIT_PLACE_CHAR_EDEL:
            return GC_CHAR_EDEL;
        case MON_AGIT_PLACE_CHAR_BEIGAS:
            return GC_CHAR_BEIGAS;
		case MON_AGIT_PLACE_CHAR_UNO:
			return GC_CHAR_UNO;
        case MON_AGIT_PLACE_CHAR_PERYTON:
            return GC_CHAR_PERYTON;
        case MON_AGIT_PLACE_CHAR_ALICE:
            return GC_CHAR_ALICE;
#ifdef DECANEE // Kawan>
        case MON_AGIT_PLACE_CHAR_DECANEE:
            return GC_CHAR_DECANEE;
#endif
        default:
            return GC_CHAR_INVALID;
    }
}

void KGCAgitGameManager::UpdateAgitHost( int nPlayerIndex )
{
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
    {
        g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost = false;
    }

    g_kGlobalValue.m_kUserInfo.bHost = false;

    bool bSelectHost = false;

    for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive ) 
        {
            //  새로 들어온 유저나 삭제한 유저는 방장 하면 안된다.
            if ( nPlayerIndex == i ) 
                continue;

            bSelectHost = true;
            
            g_MyD3D->MyPlayer[i]->m_kUserInfo.bHost = true;

            if ( g_kGlobalValue.m_kUserInfo.dwUID == g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) 
            {
                g_kGlobalValue.m_kUserInfo.bHost = true;
            }
            return;
        }
    }


    //  방장을 못찼았다. 아지트에 처음 들어올경우에만 발생한다.
    if ( !bSelectHost && g_MyD3D->IsPlayerIndex( nPlayerIndex ) ) 
    {
        if ( g_MyD3D->MyPlayer[nPlayerIndex]->m_kUserInfo.bLive ) 
        {
            g_MyD3D->MyPlayer[nPlayerIndex]->m_kUserInfo.bHost = true;

            if ( g_kGlobalValue.m_kUserInfo.dwUID == g_MyD3D->MyPlayer[nPlayerIndex]->m_kUserInfo.dwUID ) 
            {
                g_kGlobalValue.m_kUserInfo.bHost = true;
            }
        }
    }
}

bool KGCAgitGameManager::IsSpecialObject( GCITEMID _itemID )
{
    return m_kObjInfoList.IsSpecialObject( _itemID );
}

KGCAgitObjectAction* KGCAgitGameManager::GetObjectAction( GCITEMID _itemID, GCITEMUID _itemUID )
{
    return m_kObjInfoList.GetObjectAction( _itemID, _itemUID );
}

void KGCAgitGameManager::UpdateDiceObject( const LTP_AGIT_DICE_OBJECT_STATE& kPacket )
{
    std::vector<DWORD> vecParam;
    vecParam.push_back( kPacket.m_dwUserUID );
    vecParam.push_back( kPacket.m_nDiceNum1 );
    vecParam.push_back( kPacket.m_nDiceNum2 );

    m_kObjContainer.DoAction( kPacket.m_dwItemUID, vecParam );

    return;
}

void KGCAgitGameManager::UpdateFlagObject( const LTP_AGIT_FLAG_OBJECT_STATE& kPacket )
{
    std::vector<DWORD> vecParam;

    DWORD dwUpdate = kPacket.m_bUpdate == true ? 1 : 0;
    
    vecParam.push_back( dwUpdate );
    vecParam.push_back( kPacket.m_dwUserUID );

    m_kObjContainer.DoAction( kPacket.m_dwItemUID, vecParam );

    return;
}

//  깃발 오브젝트를 모두 찾아서 이미 가지고 있는지 찾아본다.
bool KGCAgitGameManager::IsHaveFlag( DWORD dwUserID )
{
    if ( dwUserID == UINT_MAX )
        return false;

    MAP_AGIT_FLAG_OBJECT& mapFlagObject = m_kObjInfoList.GetMapFlagObject();

    MAP_AGIT_FLAG_OBJECT_ITER pos = mapFlagObject.begin();
    MAP_AGIT_FLAG_OBJECT_ITER end = mapFlagObject.end();

    for ( ; pos != end; ++pos )
    {
        GCITEMID _itemID = pos->first;

        std::vector<KGCAgitObject*> vecObject = m_kObjContainer.GetObjectList( _itemID );

        for ( size_t i = 0; i < vecObject.size(); ++i ) 
        {
            KGCAgitObject* pObject = vecObject[i];

            if ( pObject->GetUserUID() == dwUserID ) 
                return true;
        }
    }

    return false;
}

bool KGCAgitGameManager::CheckEnableLieObject( D3DXVECTOR2& dxvPos_ )
{
    return m_kObjContainer.CheckEnableLieObject( dxvPos_ );
}

bool KGCAgitGameManager::IsAgitObject( GCITEMID itemID_ )
{
    bool bObject = m_kObjInfoList.IsAgitObject( itemID_ );
    if( true == bObject ) { 
        return true;
    }

    std::set< GCITEMID >::iterator sit = m_setExceptObjectID.find( itemID_ * 10 );
    if( sit == m_setExceptObjectID.end() ) { 
        return false;
    }

    return true;
}

USHORT KGCAgitGameManager::GetAgitCurCharPosCount()
{
    USHORT usCount = 0;
    int nCharType = 0;

    std::map<int,KAgitCharacterPos>::iterator mit;

    for ( mit = m_kAgitInfo.m_mapCharacterPos.begin(); mit != m_kAgitInfo.m_mapCharacterPos.end(); mit++ ) {
        nCharType = mit->first;

        for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
            MONSTER* pMonster = mit->second;
            if( pMonster->GetCharType() == nCharType ) { 
                    ++usCount;
                break;
            }
        }
    }

    return usCount;
}