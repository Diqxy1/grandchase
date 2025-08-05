#include "stdafx.h"
#include "GCAgitObjectContainer.h"
#include "GCAgitObjectList.h"
#include "GCAgitObject.h"
#include "LatencyTransfer.h"
#include "../gcui/GCGuideNewbie.h"
#include "KGCAgitGameManager.h"

KGCAgitObjectContainer::KGCAgitObjectContainer(void)
{
    m_pPickedObject = NULL;
}

KGCAgitObjectContainer::~KGCAgitObjectContainer(void)
{
}

void KGCAgitObjectContainer::InitLocalInventoryObject( const KGCAgitObjectInfoList* pkObjInfoList_, int iFlowerPotLimit /*= 5*/ )
{
    std::vector<KGCAgitObject*> vecObject;

    const KItemContainer::ItemList& mapInvecItem = g_pItemMgr->m_kInventory.GetFullItemList();
    for( KItemContainer::ItemList::const_iterator InvenIter = mapInvecItem.begin() ; InvenIter != mapInvecItem.end() ; ++InvenIter )
    {
        KGCAgitObjectInfoList::element_type* kInfo = pkObjInfoList_->GetObjectInfo( InvenIter->second.m_ItemID * 10 );
        if( kInfo == NULL )
            continue;

        GCITEMID itemID = InvenIter->second.m_ItemID * 10;
        GCITEMUID itemUID = InvenIter->second.m_ItemUID;

        //화분이고 가질 수 있는 수량이 넘었으면 패스
        if( itemID == KGCItemManager::ITEM_AGIT_FLOWER_POT_NORMAL * 10 ) 
		{

			std::vector<KGCAgitObject*>::size_type iCount = std::count_if( vecObject.begin(), vecObject.end(), 
				boost::bind( &KGCAgitObject::GetItemID, _1 ) == itemID );
			
			if ( static_cast<int>(iCount) >= iFlowerPotLimit )
				continue;
        }

        // 훈련장 나무 토막을 플레이어가 조작할 수 있는 오브덱트 아닙니다. 
        if( itemID == KGCItemManager::ITEM_AGIT_TRAINING_TILE * 10 )
            continue;

        AddObject( vecObject, itemUID, itemID, kInfo );
    }

    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        std::vector<KGCAgitObject*>::iterator vit;
        vit = std::find_if( vecObject.begin(), vecObject.end(), boost::bind( &KGCAgitObject::GetItemUID, _1 ) == pObj->GetItemUID() );

        if( vit == vecObject.end() )
            continue;

        (*vit)->SetIndexPos( pObj->GetIndexPosX(), pObj->GetIndexPosY() );

        const KFlowerPot* pFlowerInfo = pObj->GetFlowerPotInfo();
        if( pFlowerInfo ) {
            (*vit)->SetFlowerPotInfo( *pFlowerInfo );
        }
        
        const KTrainingObj* pTrainingObjInfo = pObj->GetTrainingObjInfo();
        if( pTrainingObjInfo )
            (*vit)->SetTrainingObjInfo( *pTrainingObjInfo, true );
    }

    do 
    {
        // Dummy 가 있으면 그것도 옮겨 줍니다.
        // 구매와 동시에 배치하기 위해서...
        // 필요 없을 때 지우는 건 RemoveDummyObject() 를 호출해야함.
        if( m_vecObject.empty() )
            break;

        std::vector<KGCAgitObject*>::iterator vit;
        vit = std::find_if( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::GetItemUID, _1 ) == 0 );

        if( vit == m_vecObject.end() )
            break;

        KGCAgitObjectInfoList::element_type* kInfo = pkObjInfoList_->GetObjectInfo( (*vit)->GetItemID() );
        if( kInfo == NULL )
            break;

        AddObject( vecObject, 0, (*vit)->GetItemID(), kInfo, (*vit)->GetIndexPosX(), (*vit)->GetIndexPosY() );
    } while (false);


    ClearObjectList();
    m_vecObject.swap( vecObject );

    SortObjectWithY();
}

void KGCAgitObjectContainer::SetObjectPosList( const std::vector<KAgitObjectPos>& vecObjPos_, const KGCAgitObjectInfoList* pkObjInfoList_ )
{
    for( std::vector<KAgitObjectPos>::const_iterator it = vecObjPos_.begin() ; it != vecObjPos_.end() ; ++it )
    {
        KGCAgitObjectInfoList::element_type* kInfo = pkObjInfoList_->GetObjectInfo( it->m_ID );
        if( kInfo == NULL )
            continue;

        GCITEMID itemID = it->m_ID;
        GCITEMUID itemUID = it->m_UID;
        int iPosX = it->m_nPosX;
        int iPosY = it->m_nPosY;

        AddObject( m_vecObject, itemUID, itemID, kInfo, iPosX, iPosY);
    }

    SortObjectWithY();
}

void KGCAgitObjectContainer::AddObject( std::vector<KGCAgitObject*>& vecList, GCITEMUID itemUID, GCITEMID itemID, 
                                        KGCAgitObjectInfoList::element_type* kInfo, int iPosX, int iPosY )
{
    std::vector<KGCAgitObject*>::iterator vit = std::find_if( vecList.begin(), vecList.end(), boost::bind( &KGCAgitObject::GetItemUID, _1 ) == itemUID );

    KGCAgitObject* pObj = NULL;
    if( vit != vecList.end() ) {
        pObj = (*vit);
        pObj->SetIndexPos( iPosX, iPosY );
        return;
    }

    pObj = new KGCAgitObject();
    pObj->SetItemID( itemID );
    pObj->SetItemUID( itemUID );
    pObj->SetIndexPos( iPosX, iPosY );
    pObj->SetObjectInfo( kInfo );
    pObj->SetContainer( this );

    if( itemID == KGCItemManager::ITEM_AGIT_FLOWER_POT_NORMAL * 10  || 
        itemID == KGCItemManager::ITEM_AGIT_FLOWER_POT_CASH * 10 ) {
            KFlowerPot kFlowerPot;
            kFlowerPot.m_UID = itemUID;
            kFlowerPot.m_ItemID = itemID;
            kFlowerPot.m_nSeedType = KFlowerPot::ST_NONE;

            pObj->SetFlowerPotInfo( kFlowerPot );
    }

    if ( SiKGCAgitGameManager()->IsSpecialObject( itemID ) ) 
    {
        pObj->SetObjectAction( SiKGCAgitGameManager()->GetObjectAction( itemID, itemUID ) );
    }
 


    vecList.push_back( pObj );
}

void KGCAgitObjectContainer::ClearObjectList()
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        SAFE_DELETE( pObj );
    }
    m_vecObject.clear();
}

void KGCAgitObjectContainer::Create2DObjectALL()
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        Create2DObject( pObj );
    }
}

bool KGCAgitObjectContainer::Create2DObject( GCITEMUID itemUID_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );

    if( pObj == NULL )
        return false;

    return Create2DObject( pObj );
}

bool KGCAgitObjectContainer::Create2DObject( KGCAgitObject* pObj )
{
    if( pObj == NULL )
        return false;

    return pObj->Create2DObject();
}

void KGCAgitObjectContainer::FrameMove()
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( pObj == NULL )
            continue;

        pObj->FrameMove();
    }
}

void KGCAgitObjectContainer::Render( EGCAgitObjectLayer eAgitLayer_ )
{
    // 레이어 별로 오브젝트 그려준다
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( pObj == NULL )
            continue;

        if( pObj == m_pPickedObject )
            continue;

        if( pObj->IsHide() )
            continue;

        if( pObj->GetLayer() != eAgitLayer_ )
            continue;


        pObj->Render();
    }
}

void KGCAgitObjectContainer::SortObjectWithY()
{
    std::sort( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::GetIndexPosY, _1 ) < boost::bind( &KGCAgitObject::GetIndexPosY, _2 ) );
}

std::map< GCITEMID, int > KGCAgitObjectContainer::GetHideObjectCount()
{
    std::map< GCITEMID, int > mapCount;
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( mapCount.find( pObj->GetItemID() ) == mapCount.end() ) {
            mapCount[pObj->GetItemID()] = 0;
        }

        if( pObj->IsHide() ) {
            mapCount[pObj->GetItemID()] += 1;
        }
    }

    return mapCount;
}

GCITEMUID KGCAgitObjectContainer::GetHideObjecctUID( GCITEMID itemID_ )
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( pObj == NULL )
            continue;

        if( pObj->IsHide() == false )
            continue;

        if( pObj->GetItemID() != itemID_ )
            continue;

        return pObj->GetItemUID();
    }
    return KItem::INVALID_ITEM;
}

void KGCAgitObjectContainer::Render_PickedObj()
{
    if( m_pPickedObject == NULL )
        return;

    m_pPickedObject->Render();
}

bool KGCAgitObjectContainer::GetObjectInMousePos( IN const D3DXVECTOR2& dxvMousePos, OUT GCITEMUID& itemUID_ )
{
    std::vector<KGCAgitObject*>::const_reverse_iterator vit = std::find_if( m_vecObject.rbegin(), m_vecObject.rend(), boost::bind( &KGCAgitObject::CheckBounce, _1, dxvMousePos ) == true );

    if( vit == m_vecObject.rend() ) {
        itemUID_ = KItem::INVALID_ITEM;
        return false;
    }

    itemUID_ = (*vit)->GetItemUID();
    return true;
}


bool KGCAgitObjectContainer::GetFlowerPotInMousePos( IN const D3DXVECTOR2& dxvMousePos, OUT GCITEMUID& itemUID_ )
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( pObj->IsFlowerPot() == false )
            continue;

        if( pObj->CheckBounce( dxvMousePos ) == false )
            continue;

        itemUID_ = pObj->GetItemUID();
        return true;
    }
    return false;
}

bool KGCAgitObjectContainer::PickUpObject( GCITEMUID itemUID_, D3DXVECTOR2* pOffset )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( pObj == NULL )
        return false;

    SaveCurrentPosition();

    if( pObj->IsHide() ) {
        ShowObject( itemUID_, 1, 1 );
    }

    m_pPickedObject = pObj;

    if( pOffset ) {
        if( pOffset->x < 0.f ) {
            pOffset->x = m_pPickedObject->GetWidth() / 2.f;
            pOffset->y = m_pPickedObject->GetHeight() / 2.f;
        }
        else {
            pOffset->x = pOffset->x - m_pPickedObject->GetPosX();
            pOffset->y = pOffset->y - m_pPickedObject->GetPosY();
        }
    }

    m_pPickedObject->RemoveFootHold();
    m_pPickedObject->Get2DObject()->SetPhysics( false );
    m_pPickedObject->ClearSubObject();

    SetPickedObjAlpha( 150 );
    return true;
}

void KGCAgitObjectContainer::PutDownObject()
{
    SetPickedObjRGB( 255, 255, 255 );
    SetPickedObjAlpha( 255 );

    if( NULL == m_pPickedObject )
        return;

    KGC2DObject* pObj = m_pPickedObject->Get2DObject();
    
    if( pObj == NULL )
        return;

    pObj->SetPhysics( true );
    pObj->SetSpeed( 0.f, 0.f );

    if( m_pPickedObject->CheckAvailablePlace() == false ) {
        // 놓으려는 위치가 옳지 않다.
        RollbackPosition();
        g_KDSound.Play( "984031" );
    }

    SortObjectWithY();

    PackLatencyInfo( m_pPickedObject );
    m_pPickedObject = NULL;

    g_pkUIScene->m_pkGuideAzit->SetCompleateActionNum(1);
}

void KGCAgitObjectContainer::UpdatePickedObjectPos( const D3DXVECTOR2& dxvMousePos, D3DXVECTOR2* pOffset )
{
    if( m_pPickedObject == NULL )
        return;

    D3DXVECTOR2 vecOffset(0.f, 0.f);

    if( pOffset != NULL ) {
        vecOffset = (*pOffset);
    }

    BYTE r = 255, g = 255, b = 255;
    if( m_pPickedObject->CheckAvailablePlace() == false ) {
        r = 255;
        g = 50;
        b = 50;
    }
    SetPickedObjRGB( r, g, b );

    m_pPickedObject->SetPos(
        (dxvMousePos.x - vecOffset.x),
        (dxvMousePos.y - vecOffset.y)
        );

    PackLatencyInfo( m_pPickedObject );
}

void KGCAgitObjectContainer::PackLatencyInfo( KGCAgitObject* pObj )
{
    if( pObj == NULL )
        return;

    // 요기서 동기화용 정보를 날릴 수 있겠음.
    LTP_AGIT_OBJECT_POS kPacket;
    kPacket.m_dwSenderUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kPacket.m_itemUID = pObj->GetItemUID();
    kPacket.m_iPosX = pObj->GetIndexPosX();
    kPacket.m_iPosY = pObj->GetIndexPosY();

    KGC2DObject* p2DObj = pObj->Get2DObject();
    if( p2DObj ) {
        kPacket.m_bPhysics = p2DObj->IsPhysics();
        p2DObj->GetRGB( kPacket.m_R, kPacket.m_G, kPacket.m_B );
        kPacket.m_cAlpha = p2DObj->GetAlpha();
    }

    g_LatencyTransfer.PushPacket( &kPacket );
}

void KGCAgitObjectContainer::HideObject( GCITEMUID itemUID_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( pObj == NULL )
        return;

    pObj->SetIndexPos( -1, -1 );
    pObj->RemoveObjectParticle();

    if( m_pPickedObject == pObj ) {
        m_pPickedObject = NULL;
    }
}

void KGCAgitObjectContainer::SetPickedObjAlpha( BYTE alpha_ )
{
    if( m_pPickedObject == NULL )
        return;

    KGC2DObject* pObj = m_pPickedObject->Get2DObject();
    if( pObj == NULL )
        return;

    pObj->SetAlpha( alpha_ );
}

void KGCAgitObjectContainer::SetPickedObjRGB( BYTE R_, BYTE G_, BYTE B_ )
{
    if( m_pPickedObject == NULL )
        return;

    KGC2DObject* pObj = m_pPickedObject->Get2DObject();
    if( pObj == NULL )
        return;

    pObj->SetRGB( R_, G_, B_ );
}

GCITEMUID KGCAgitObjectContainer::GetPickedObjUID()
{
    if( m_pPickedObject == NULL )
        return KItem::INVALID_ITEM;

    return m_pPickedObject->GetItemUID();
}

void KGCAgitObjectContainer::SaveCurrentPosition()
{
    std::map< GCITEMUID, std::pair<int,int> > mapPos;
    GetCurrentPosition(mapPos);

    m_vecSavedStep.push_back( mapPos );
}

void KGCAgitObjectContainer::RollbackPosition()
{
    if( m_vecSavedStep.empty() )
        return;

    std::map< GCITEMUID, std::pair<int,int> > mapPos = m_vecSavedStep.back();
    m_vecSavedStep.pop_back();

    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        std::map< GCITEMUID, std::pair<int,int> >::iterator mit;

        mit = mapPos.find( pObj->GetItemUID() );

        if( mit == mapPos.end() )
            continue;

        pObj->SetIndexPos( mit->second.first, mit->second.second );
        pObj->Create2DObject();

        PackLatencyInfo( pObj );
    }
}

void KGCAgitObjectContainer::ShowObject( GCITEMUID itemUID_, int iPosX_, int iPosY_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( pObj == NULL )
        return;

    pObj->Create2DObject( iPosX_, iPosY_ );
}

bool KGCAgitObjectContainer::IsNeedUpdate()
{
    std::vector<KGCAgitObject*>::iterator vit = std::find_if( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::IsNeedUpdate, _1) == true );

    if( vit == m_vecObject.end() )
        return false;

    return true;
}

bool KGCAgitObjectContainer::IsPosUpdated()
{
    std::vector<KGCAgitObject*>::iterator vit = std::find_if( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::IsPosUpdated, _1) == true );

    if( vit == m_vecObject.end() )
        return false;

    return true;
}

std::vector<KAgitObjectPos> KGCAgitObjectContainer::GetUpdatedPosList()
{
    std::vector<KAgitObjectPos> vecPos;
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( pObj->IsPosUpdated() == false && pObj->IsHide() == true )
            continue;

        if( pObj->GetItemUID() == KItem::INVALID_ITEM )
            continue;

        KAgitObjectPos kPos;

        kPos.m_ID = pObj->GetItemID();
        kPos.m_UID = pObj->GetItemUID();
        kPos.m_nPosX = pObj->GetIndexPosX();
        kPos.m_nPosY = pObj->GetIndexPosY();

        vecPos.push_back( kPos );
    }

    return vecPos;
}

void KGCAgitObjectContainer::SetPosUpdated()
{
    std::for_each( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::SetPosUpdated, _1, false ) );
}

void KGCAgitObjectContainer::HideObjectAll()
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        pObj->SetHide();
    }
}

void KGCAgitObjectContainer::SetFlowerPotInfo( KFlowerPot& info_ )
{
    KGCAgitObject* pObj = GetObject( info_.m_UID );
    if( pObj == NULL )
        return;

    pObj->SetFlowerPotInfo( info_, true );
}

bool KGCAgitObjectContainer::IsHarvestableFlower( GCITEMUID itemUID_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( pObj == NULL )
        return false;

    return pObj->IsHarvestableFlowerPot();
}

bool KGCAgitObjectContainer::IsPlantableFlower( GCITEMUID itemUID_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( pObj == NULL )
        return false;

    return pObj->IsPlantableFlowerPot();
}

bool KGCAgitObjectContainer::AddDummyObject( GCITEMID itemID_, const KGCAgitObjectInfoList* pkObjInfoList_, int iPosX /*= -1*/, int iPosY /*= -1 */ )
{
    if( pkObjInfoList_ == NULL )
        return false;

    KGCAgitObjectInfoList::element_type* pkObjInfo = pkObjInfoList_->GetObjectInfo( itemID_ );
    if( pkObjInfo == NULL )
        return false;

    AddObject( m_vecObject, 0, itemID_, pkObjInfo, iPosX, iPosY );
    return true;
}

void KGCAgitObjectContainer::RemoveDummyObject()
{
    std::vector<KGCAgitObject*>::iterator vit;

    while( true ) {
        vit = std::find_if( m_vecObject.begin(), m_vecObject.end(),
            boost::bind( &KGCAgitObject::GetItemUID, _1 ) == 0 );

        if( vit == m_vecObject.end() )
            break;

        if( m_pPickedObject == (*vit) ) {
            m_pPickedObject = NULL;
        }

        SAFE_DELETE( (*vit) );
        m_vecObject.erase( vit );
    }
}

bool KGCAgitObjectContainer::IsHideObject( GCITEMUID itemUID_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( pObj == NULL )
        return true;

    return pObj->IsHide();
}

int KGCAgitObjectContainer::GetStackedObjectLayer( const GCITEMUID& itemUID_, int iFootholdX_, int iFootholdY_ )
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        if( pObj->GetItemUID() == itemUID_ )
            continue;

        if( pObj->IsHide() )
            continue;

        if( pObj->IsMyFootHold( iFootholdX_, iFootholdY_ ) )
            return pObj->GetLayer();
    }
    return -1;
}

std::map< GCITEMUID, std::pair<int,int> > KGCAgitObjectContainer::GetCurSavedPos( void )
{
    if( m_vecSavedStep.empty() )
        return std::map< GCITEMUID, std::pair<int,int> >();

    return m_vecSavedStep.back();
}

void KGCAgitObjectContainer::UpdateLatencyInfo( const LTP_AGIT_OBJECT_POS& kPacket )
{
    if( kPacket.m_dwSenderUID == g_kGlobalValue.m_kUserInfo.dwUID )
        return;

    KGCAgitObject* pObj = GetObject( kPacket.m_itemUID );
    if( pObj == NULL )
        return;

    pObj->SetIndexPos( kPacket.m_iPosX, kPacket.m_iPosY );
    KGC2DObject* p2DObj = pObj->Get2DObject();

    if( p2DObj ) {
        p2DObj->SetAlpha( kPacket.m_cAlpha );
        p2DObj->SetRGB( kPacket.m_R, kPacket.m_G, kPacket.m_B );
        p2DObj->SetPhysics( kPacket.m_bPhysics );
    }
}

void KGCAgitObjectContainer::BuyDummyObject()
{
    std::vector<KGCAgitObject*>::iterator vit = std::find_if( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::GetItemUID, _1) == 0 );

    if( vit == m_vecObject.end() )
        return;

    GCITEMUID itemUID = GetHideObjecctUID( (*vit)->GetItemID() );
    if( itemUID == KItem::INVALID_ITEM ) {
        SAFE_DELETE( *vit );
        m_vecObject.erase(vit);
        return;
    }

    KGCAgitObject* pObj = GetObject( itemUID );
    if( pObj == NULL )
        return;

    pObj->Create2DObject( (*vit)->GetIndexPosX(), (*vit)->GetIndexPosY() );
    pObj->SetPosUpdated( true );
    RemoveDummyObject();
}

KGCAgitObject* KGCAgitObjectContainer::GetObject( GCITEMUID itemUID_ )
{
    std::vector<KGCAgitObject*>::iterator vit = std::find_if( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::GetItemUID, _1) == itemUID_ );

    if( vit == m_vecObject.end() )
        return NULL;

    return (*vit);
}

int KGCAgitObjectContainer::GetItemCount( GCITEMID itemID_ )
{
    std::vector<KGCAgitObject*>::size_type iCount = std::count_if( m_vecObject.begin(), m_vecObject.end(), boost::bind( &KGCAgitObject::GetItemID, _1 ) == itemID_ );
    return static_cast<int>(iCount);
}

D3DXVECTOR2 KGCAgitObjectContainer::GetObjectPos( GCITEMUID itemUID_ )
{
    KGCAgitObject* pObj = GetObject( itemUID_ );
    if( NULL == pObj )
        return D3DXVECTOR2( -1.0f, -1.0f );
    else
        return D3DXVECTOR2( pObj->GetPosX() + ( pObj->GetWidth() / 2.0f ), pObj->GetPosY() );
}

int KGCAgitObjectContainer::GetFlowerPotState( GCITEMUID itemUID_ )
{
    if( GetObject( itemUID_ ) != NULL )
        return GetObject( itemUID_ )->GetFlowerState();
    return KGCAgitObject::E_FLOWER_STATE_NONE;
}

int KGCAgitObjectContainer::GetRemainBloomMinute( GCITEMUID itemUID_ )
{
    if( GetObject( itemUID_ ) != NULL )
        return GetObject( itemUID_ )->GetRemainBloomMinute();
    return 0;
}

void KGCAgitObjectContainer::GetCurrentPosition( std::map< GCITEMUID, std::pair<int,int> > &mapPos )
{
    mapPos.clear();
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {
        mapPos[pObj->GetItemUID()] = std::pair<int,int>( pObj->GetIndexPosX(), pObj->GetIndexPosY() );
    }
}

bool KGCAgitObjectContainer::CheckBoundToMousePos( const D3DXVECTOR2& dxvMousePos, GCITEMID itemID, OUT GCITEMUID& itemUID, D3DXVECTOR4 dxvWeight )
{
    itemID *= 10;
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {

        if( pObj->GetItemID() != itemID )
            continue;

        if( pObj->CheckBounce( dxvMousePos, dxvWeight ) == false )
            continue;

        itemUID = pObj->GetItemUID();
        return true;
    }

    itemUID = KItem::INVALID_ITEM;
    return false;
}

void KGCAgitObjectContainer::SetTrainingObjInfo( KTrainingObj& info_, bool ImgUpdate_ )
{
    KGCAgitObject* pObj = GetObject( info_.m_ItemUID );
    if( pObj == NULL )
        return;

    pObj->SetTrainingObjInfo( info_, ImgUpdate_ );
}

int KGCAgitObjectContainer::GetBrokenTrainingTile( GCITEMUID itemUID )
{
    if( GetObject( itemUID ) != NULL )
        return GetObject( itemUID )->GetTrainingObjTile();

    return 0;
}

D3DXVECTOR2 KGCAgitObjectContainer::GetTrainigObjPos( GCITEMUID itemUID )
{
    if( GetObject( itemUID ) != NULL )
        return GetObject( itemUID )->GetTrainingObjPos();

    return D3DXVECTOR2( -1.0f, -1.0f );
}

void KGCAgitObjectContainer::ClearTrainingTileImg( GCITEMUID itemUID )
{
    if( GetObject( itemUID ) != NULL )
        GetObject( itemUID )->ClearSubObject();
}

bool KGCAgitObjectContainer::CheckEnableSitDownObject( D3DXVECTOR2& dxvPos_ )
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {

        if( false == pObj->IsEnableSitDown() ) { 
            continue;
        }

        if( false == pObj->CheckBounceEnableSitDownd( dxvPos_ ) ) {
            continue;
        }

        return true;
    }

    return false;
}

void KGCAgitObjectContainer::DoAction( GCITEMUID _itemUID, const std::vector<DWORD>& vecParam )
{
    KGCAgitObject* pAgitObj = GetObject( _itemUID );
    if ( !pAgitObj ) return;

    pAgitObj->DoAction( vecParam );

}

std::vector<KGCAgitObject*> KGCAgitObjectContainer::GetObjectList( GCITEMID _itemID )
{
    std::vector<KGCAgitObject*> vecObject;
    
    std::vector<KGCAgitObject*>::iterator pos = m_vecObject.begin();
    std::vector<KGCAgitObject*>::iterator end = m_vecObject.end();

    for ( ; pos != end; ++pos ) 
    {
        KGCAgitObject* pAgitObject = *pos;

        if ( _itemID == pAgitObject->GetItemID() )
            vecObject.push_back( pAgitObject );
    }

    return vecObject;
}
bool KGCAgitObjectContainer::CheckEnableLieObject( D3DXVECTOR2& dxvPos_ )
{
    BOOST_FOREACH( KGCAgitObject* pObj, m_vecObject ) {

        if( false == pObj->IsEnableLie() ) { 
            continue;
        }

        if( false == pObj->CheckBounceEnableLie( dxvPos_ ) ) {
            continue;
        }

        return true;
    }

    return false;
}