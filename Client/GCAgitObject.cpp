#include "stdafx.h"
#include "GCAgitObject.h"
#include "GCAgitObjectList.h"
#include "GCAgitObjectContainer.h"
#include "GCAgitJumpAction.h"
#include "GCAgitDiceAction.h"
#include "GCAgitFlagAction.h"
#include "KGCAgitGameManager.h"

KGCAgitObject::KGCAgitObject( void )
: m_dxvParticleTrace( 0.f, 0.f, 0.5f )
{
    m_ItemID = KItem::INVALID_ITEM;
    m_ItemUID = KItem::INVALID_ITEM;
    m_iPosX = -1;
    m_iPosY = -1;
    m_iStackedLayer = -1;
    m_p2DObject = NULL;
    m_pObjectInfo = NULL;
    m_pkFlowerPot = NULL;
    m_pkContainer = NULL;
    m_iFlowerState = E_FLOWER_STATE_NONE;

    m_pFlowerParticle = NULL;
    m_vecObjectParticle.clear();

    m_bFootHoldExist = false;
    m_bNeedUpdate = false;

    m_pkTraingObj = NULL;
    m_vecSubObject.clear();
    m_pObjectAction = NULL;
}

KGCAgitObject::~KGCAgitObject(void)
{
    RemoveFootHold();
    SAFE_DELETE( m_p2DObject );
    SAFE_DELETE( m_pObjectInfo );
    SAFE_DELETE( m_pkFlowerPot );
    SAFE_DELETE( m_pkTraingObj );
    ClearSubObject();
    RemoveAllParticle();

    if ( m_pObjectAction ) 
        m_pObjectAction->Destroy();

    SAFE_DELETE( m_pObjectAction );

    //누군가 여기서 m_pkContainer 지우면 클라이언트 마구 죽습니다.
    // m_pkContainer는 포인터로 되어 있지만 다른곳에서 받아오는 것이니 지우면 안됨.
}

void KGCAgitObject::RemoveFlowerParticle()
{
    if( m_pFlowerParticle == NULL )
        return;

    g_ParticleManager->DeleteSequence( m_pFlowerParticle );
    m_pFlowerParticle = NULL;
}

void KGCAgitObject::SetItemID( GCITEMID itemID )
{
    m_ItemID = itemID;
}

void KGCAgitObject::SetItemUID( GCITEMUID itemUID )
{
    m_ItemUID = itemUID;
}

void KGCAgitObject::SetIndexPosX( int iPosX_ )
{
    if( iPosX_ < 0 )
        iPosX_ = -1;

    if( GetIndexPosX() == iPosX_ )
        return;

    RemoveFootHold();
    m_iPosX = iPosX_;
}

void KGCAgitObject::SetIndexPosY( int iPosY_ )
{
    if( iPosY_ < 0 )
        iPosY_ = -1;

    if( GetIndexPosY() == iPosY_ )
        return;

    RemoveFootHold();
    m_iPosY = iPosY_;
}

void KGCAgitObject::SetIndexPos( int iPosX_, int iPosY_ )
{
    SetIndexPosX( iPosX_ );
    SetIndexPosY( iPosY_ );

    if( IsHide() ) {
        SAFE_DELETE( m_p2DObject );
        RemoveAllParticle();
        SetPosUpdated( true );
        SetNeedUpdate( true );

        if( m_pObjectAction ) 
        {
            m_pObjectAction->EnableLive( false );
    }

    }

    if( m_p2DObject ) {
        SetPos( TO_AGIT_COORDI( GetIndexPosX() ), TO_AGIT_COORDI( GetIndexPosY() ) );
    }
}

void KGCAgitObject::SetObjectInfo( const KAgitObjectInfo* kInfo_ )
{
    m_pObjectInfo = new KAgitObjectInfo( *kInfo_ );

    std::map<int,int> mapLow;
    BOOST_FOREACH( KAgitFootHold& kFoothold, m_pObjectInfo->vecFootHold ) {
        std::map<int,int>::iterator mit = mapLow.find( kFoothold.iPosX );

        if( mit == mapLow.end() ) {
            mit = mapLow.insert( std::map<int,int>::value_type( kFoothold.iPosX, kFoothold.iPosY ) ).first;
        }

        if( mit->second > kFoothold.iPosY ) {
            mit->second = kFoothold.iPosY;
            m_vecBottomFoothold.clear();
            m_vecBottomFoothold.push_back( kFoothold );
        }
        else if( mit->second == kFoothold.iPosY ) {
            m_vecBottomFoothold.push_back( kFoothold );
        }
    }
}

bool KGCAgitObject::Create2DObject()
{
    return Create2DObject( GetIndexPosX(), GetIndexPosY() );
}

bool KGCAgitObject::Create2DObject( int iPosX_, int iPosY_ )
{
    if( m_pObjectInfo == NULL )
        return false;

    if( m_p2DObject != NULL ) {
        SetIndexPos( iPosX_, iPosY_ );
        return true;
    }

    if( iPosX_ == -1 || iPosY_ == -1 )
        return false;

    m_p2DObject = new KGC2DObject;
    SetIndexPos( iPosX_, iPosY_ );
    static float sOffset = FOOTHOLDTERM;
    m_p2DObject->SetTransOffset( 0.f, sOffset );

    m_iStackedLayer = -1;

    if( m_pObjectInfo->bContact )
    {
        m_p2DObject->SetBouceCheckWithMap( false );
        m_p2DObject->SetGravityY( -0.0006f );
        m_p2DObject->SetFrictionalForceY( 0.1f );
    }

    // 텍스처 
    for( std::vector< std::string >::const_iterator it = m_pObjectInfo->vecTexture.begin() ; it != m_pObjectInfo->vecTexture.end() ; ++it )
    {
        m_p2DObject->AddTexture( it->c_str() );

        DWORD dwTexWidth = m_p2DObject->GetCurrTex()->GetWidth();
        DWORD dwTexHeight = m_p2DObject->GetCurrTex()->GetHeight();

        float fScaleX = static_cast<float>( dwTexWidth / 20.0f );
        float fScaleY = static_cast<float>( dwTexHeight / 20.0f );

        m_p2DObject->SetScale( TO_AGIT_COORDI( fScaleX ), TO_AGIT_COORDI( fScaleY ) );
        m_p2DObject->SetObjectOffsetX( TO_AGIT_COORDI( fScaleX * 0.5f ) );
    }

    // 오브젝트가 갱신되면 꽃도 갱신.
    SetFlowerState( E_FLOWER_STATE_NONE );
    UpdateFlowerState();

    SetNeedUpdate( true );
    return true;
}

bool KGCAgitObject::CheckAvailablePlace()
{
    bool bExistBlock = false;
    for( std::vector< KAgitFootHold >::const_iterator it = m_pObjectInfo->vecFootHold.begin() ; it != m_pObjectInfo->vecFootHold.end() ; ++it )
    {
        bExistBlock = SiKGCFantasticMap()->IsVaildBlockIndex( GetIndexPosX() + it->iPosX , GetIndexPosY() + it->iPosY + 1 );
        if( bExistBlock )
            break;
    }

    if( bExistBlock )
    {
        return false;
    }
    return true;
}

bool KGCAgitObject::CrashCheckWithFootHold()
{
    if( m_pObjectInfo->vecFootHold.empty() )
        return true;

    if( m_pObjectInfo->bContact == false )
        return true;

    for( std::vector< KAgitFootHold >::iterator it = m_vecBottomFoothold.begin() ; it != m_vecBottomFoothold.end() ; ++it )
    {
        D3DXVECTOR2 vecRet;
        if( SiKGCFantasticMap()->IsVaildBlockIndex(
            GetIndexPosX() + it->iPosX,
            GetIndexPosY() + it->iPosY, vecRet ) )
        {
            if( m_pkContainer ) {
                int iStackedLayer = m_pkContainer->GetStackedObjectLayer( GetItemUID(), GetIndexPosX() + it->iPosX, GetIndexPosY() + it->iPosY );
                SetStackedLayer( iStackedLayer );
            }
            else {
                SetStackedLayer( -1 );
            }
            SetPosY( vecRet.y - TO_AGIT_COORDI(it->iPosY) );
            return true;
        }
    }
    SetStackedLayer( -1 );
    return false;
}

void KGCAgitObject::FrameMove()
{
    do{
        if( m_p2DObject == NULL )
            break;

        if( m_p2DObject->IsPhysics() == false ) {
            SetFlowerState( E_FLOWER_STATE_NONE );
        }

        m_p2DObject->FrameMove_Origin();

        SetIndexPosX( TO_INDEX(m_p2DObject->GetPosX()) );
        SetIndexPosY( TO_INDEX(m_p2DObject->GetPosY()) );

        m_dxvParticleTrace.x = m_p2DObject->GetPosX();
        m_dxvParticleTrace.y = m_p2DObject->GetPosY();

        if( m_p2DObject->IsPhysics() == false )
            break;

        if(SiKGCAgitGameManager()->GetAgitEditState() == true || SiKGCAgitGameManager()->IsPrePlace() == true ){// 편집 모드일때만 사용한다.
            if( CrashCheckWithFootHold() == false ) {
                if( m_bFootHoldExist == true ) {
                    RemoveFootHold();
                }
                break;
            }
        }

        m_p2DObject->SetSpeed( 0.f, 0.f );
        SetFootHold( true );
        UpdateFlowerState();

        if ( m_pObjectAction ) 
            m_pObjectAction->FrameMove();

        if( m_pObjectInfo->bCanStack == false && m_iStackedLayer != -1 ) {
            SetHide();
        }
    }
    while(false);
}

void KGCAgitObject::Render()
{
    if( m_p2DObject ) {
        m_p2DObject->Render();
    }

    for( std::vector<KGC2DObject*>::iterator it = m_vecSubObject.begin() ; it != m_vecSubObject.end() ; ++it )
        (*it)->Render();
}

int KGCAgitObject::GetLayer()
{
    if( m_pObjectInfo == NULL )
        return -1;

    int iStackedLayer = GetStackedLayer();
    if( iStackedLayer >= 0 )
        return iStackedLayer;

    return m_pObjectInfo->iLayerNum;
}

bool KGCAgitObject::IsHide()
{
    if( GetIndexPosX() < 0 || GetIndexPosY() < 0 )
        return true;

    return false;
}

void KGCAgitObject::SetHide()
{
    SetIndexPos( -1, -1 );
}

bool KGCAgitObject::CheckBounce( const D3DXVECTOR2& dxvMousePos )
{
    if( m_p2DObject == NULL )
        return false;

    D3DXVECTOR4 dxvPickBound;

    dxvPickBound.x = m_p2DObject->GetPos().x;                          //left
    dxvPickBound.y = m_p2DObject->GetPos().y + m_p2DObject->GetHeight();      //top
    dxvPickBound.z = m_p2DObject->GetPos().x + m_p2DObject->GetWidth();       //right
    dxvPickBound.w = m_p2DObject->GetPos().y;                          //bottom

    if( ( dxvPickBound.x <= dxvMousePos.x && dxvMousePos.x <= dxvPickBound.z ) &&
        ( dxvPickBound.w <= dxvMousePos.y && dxvMousePos.y <= dxvPickBound.y ) )
        return true;

    return false;
}

bool KGCAgitObject::CheckBounce( const D3DXVECTOR2& dxvMousePos, const D3DXVECTOR4& dxvWeight )
{
    // Weight 만큼 범위를 조절한 다음 체크합니다. 
    if( m_p2DObject == NULL )
        return false;

    D3DXVECTOR4 dxvPickBound;

    dxvPickBound.x = m_p2DObject->GetPos().x;                                 //left
    dxvPickBound.y = m_p2DObject->GetPos().y + m_p2DObject->GetHeight();      //top
    dxvPickBound.z = m_p2DObject->GetPos().x + m_p2DObject->GetWidth();       //right
    dxvPickBound.w = m_p2DObject->GetPos().y;                                 //bottom

    dxvPickBound.x += dxvWeight.x;
    dxvPickBound.y += dxvWeight.y;
    dxvPickBound.z += dxvWeight.z;
    dxvPickBound.w += dxvWeight.w;

    if( ( dxvPickBound.x <= dxvMousePos.x && dxvMousePos.x <= dxvPickBound.z ) &&
        ( dxvPickBound.w <= dxvMousePos.y && dxvMousePos.y <= dxvPickBound.y ) )
        return true;

    return false;
}

bool KGCAgitObject::CheckBounceEnableSitDownd( const D3DXVECTOR2& dxvMousePos )
{
    // 앉을 수 있는가 체크 
    if( NULL == m_p2DObject ) {
        return false;
    }

    D3DXVECTOR4 dxvPickBound;

    dxvPickBound.x = m_p2DObject->GetPos().x;                                 //left
    dxvPickBound.y = m_p2DObject->GetPos().y + m_p2DObject->GetHeight();      //top
    dxvPickBound.z = m_p2DObject->GetPos().x + m_p2DObject->GetWidth();       //right
    dxvPickBound.w = m_p2DObject->GetPos().y;                                 //bottom

    float fHalfWidth = m_p2DObject->GetWidth() * 0.5f;
    float fHalfHeight = m_p2DObject->GetHeight() * 0.5f;

    dxvPickBound.x += (fHalfWidth * 2.0f / 3.0f );
    dxvPickBound.y = dxvPickBound.w + (fHalfHeight * 2.0f / 3.0f );
    dxvPickBound.z -= (fHalfWidth * 2.0f / 3.0f ) ;

    if( ( dxvPickBound.x <= dxvMousePos.x && dxvMousePos.x <= dxvPickBound.z ) &&
        ( dxvPickBound.w <= dxvMousePos.y && dxvMousePos.y <= dxvPickBound.y ) ) {
        return true;
    }

    return false;
}

bool KGCAgitObject::CheckBounceEnableLie( const D3DXVECTOR2& dxvMousePos )
{
    // 누울 수 있는가 체크 
    if( NULL == m_p2DObject ) {
        return false;
    }

    D3DXVECTOR4 dxvPickBound;

    dxvPickBound.x = m_p2DObject->GetPos().x;                                 //left
    dxvPickBound.y = m_p2DObject->GetPos().y + m_p2DObject->GetHeight();      //top
    dxvPickBound.z = m_p2DObject->GetPos().x + m_p2DObject->GetWidth();       //right
    dxvPickBound.w = m_p2DObject->GetPos().y;                                 //bottom

    float fHalfHeight = m_p2DObject->GetHeight() * 0.5f;

    dxvPickBound.x += (m_p2DObject->GetWidth() * 3.0f / 10.0f );
    dxvPickBound.y = dxvPickBound.w + (fHalfHeight * 2.0f / 3.0f );
    dxvPickBound.z -= (m_p2DObject->GetWidth() * 3.0f / 10.0f ) ;

    if( ( dxvPickBound.x <= dxvMousePos.x && dxvMousePos.x <= dxvPickBound.z ) &&
        ( dxvPickBound.w <= dxvMousePos.y && dxvMousePos.y <= dxvPickBound.y ) ) {
            return true;
    }

    return false;
}

float KGCAgitObject::GetWidth()
{
    if( m_p2DObject == NULL )
        return 0.f;

    return m_p2DObject->GetWidth();
}

float KGCAgitObject::GetHeight()
{
    if( m_p2DObject == NULL )
        return 0.f;

    return m_p2DObject->GetHeight();
}

float KGCAgitObject::GetPosX()
{
    if( m_p2DObject == NULL )
        return -1.f;

    return m_p2DObject->GetPosX();
}

float KGCAgitObject::GetPosY()
{
    if( m_p2DObject == NULL )
        return -1.f;

    return m_p2DObject->GetPosY();
}

void KGCAgitObject::SetPosX( float fPosX_ )
{
    SetIndexPosX( static_cast<int>( fPosX_ * TOINDEXFACTOR ) );

    if( m_p2DObject )
        m_p2DObject->SetPositionX( TO_AGIT_COORDI( GetIndexPosX() ) );
}

void KGCAgitObject::SetPosY( float fPosY_ )
{
    SetIndexPosY( static_cast<int>( fPosY_ * TOINDEXFACTOR ) );

    if( m_p2DObject )
        m_p2DObject->SetPositionY( TO_AGIT_COORDI( GetIndexPosY() ) );
}

void KGCAgitObject::SetPos( float fPosX_, float fPosY_ )
{
    SetPosX( fPosX_ );
    SetPosY( fPosY_ );
}

void KGCAgitObject::RemoveFootHold()
{
    if( m_bFootHoldExist == false )
        return;

    for( std::vector< KAgitFootHold >::iterator it = m_pObjectInfo->vecFootHold.begin() ; it != m_pObjectInfo->vecFootHold.end() ; ++it ){
        SiKGCFantasticMap()->RemoveDungeonFootHold(
            static_cast<float>( ( GetIndexPosX() + it->iPosX ) * FOOTHOLDSIZE ),
            static_cast<float>( ( GetIndexPosY() + it->iPosY + 1) * FOOTHOLDSIZE ),
            NULL );
    }
    m_bFootHoldExist = false;
}

void KGCAgitObject::SetFootHold( bool bForce_ /*= false*/ )
{
    if( m_bFootHoldExist )
        return;

    if( bForce_ == false && CheckAvailablePlace() == false )
    {
        SetIndexPos( -1, -1 );
        return;
    }

    // 맵에 발판을 셋팅한다.
    for( std::vector< KAgitFootHold >::iterator it = m_pObjectInfo->vecFootHold.begin() ; it != m_pObjectInfo->vecFootHold.end() ; ++it )
    {
        SiKGCFantasticMap()->AddDungeonFootHoldByValue(
            static_cast<float>( (GetIndexPosX() + it->iPosX) * FOOTHOLDSIZE ),
            static_cast<float>( (GetIndexPosY() + it->iPosY + 1) * FOOTHOLDSIZE ),
            it->dwProp, NULL, "" );
    }
    m_bFootHoldExist = true;
    SetPosUpdated( true );

    CreateObjectParticle();
    g_KDSound.Play( "984030" );

    if( m_ItemID == KGCItemManager::ITEM_AGIT_TRAINING * 10 )
        UpdateTrainingTile( true );



    if ( m_pObjectAction ) 
    {
        // 여기서 기본동작에 대한 Init을 설정한다.
        m_pObjectAction->SetPosX(GetPosX());
        m_pObjectAction->SetPosY(GetPosY());
        m_pObjectAction->SetWidth(GetWidth());
        m_pObjectAction->SetHeight(GetHeight());
        m_pObjectAction->EnableLive( true );
    }
}

bool KGCAgitObject::IsNeedUpdate()
{
    if( m_bNeedUpdate ) {
        m_bNeedUpdate = false;
        return true;
    }

    return false;
}

bool KGCAgitObject::IsPosUpdated()
{
    return m_bPosUpdated;
}

void KGCAgitObject::UpdateFlowerState()
{
    if( m_pkFlowerPot == NULL )
        return;

    int iTempState = E_FLOWER_STATE_NONE;

    if( m_pkFlowerPot->m_nSeedType == KFlowerPot::ST_NONE ) {
        SetFlowerState( E_FLOWER_STATE_NONE );
    }
    else {
        time_t tmCurTime = g_kGlobalValue.TimeToInt( g_kGlobalValue.m_tmServerTime );
        if( m_pkFlowerPot->m_tmFlowerTime > tmCurTime ) {
            SetFlowerState( E_FLOWER_STATE_BUD );
        }
        else {
            SetFlowerState( E_FLOWER_STATE_BLOOM );
        }
    }
}

void KGCAgitObject::SetFlowerPotInfo( const KFlowerPot& kFlowerPot_, bool bOnFlowerInfoUpdate_ )
{
    if( m_pkFlowerPot == NULL ) {
        m_pkFlowerPot = new KFlowerPot();
    }

    m_pkFlowerPot->m_nSeedType = kFlowerPot_.m_nSeedType;
    if ( true == bOnFlowerInfoUpdate_ ) {
        m_pkFlowerPot->m_tmFlowerTime = kFlowerPot_.m_tmFlowerTime + g_kGlobalValue.m_tmServerSyncTime;
    }
    else {
        m_pkFlowerPot->m_tmFlowerTime = kFlowerPot_.m_tmFlowerTime;
    }
    m_pkFlowerPot->m_tmSeedTime     = kFlowerPot_.m_tmSeedTime + g_kGlobalValue.m_tmServerSyncTime;
    m_pkFlowerPot->m_usFlowerCount  = kFlowerPot_.m_usFlowerCount;

    SetFlowerState( E_FLOWER_STATE_NONE );
    UpdateFlowerState();
}

void KGCAgitObject::CreateFlowerParticle()
{
    RemoveFlowerParticle();

    if( IsHide() )
        return;

    if( m_pObjectInfo->mapFlowerPotParticle.empty() == false &&
        m_pkFlowerPot->m_nSeedType != KFlowerPot::ST_NONE )
    {
        std::map< int, KAgitObjectParticle >::iterator mit;
        if( IsHarvestableFlowerPot() )  // 피었나??
            mit = m_pObjectInfo->mapFlowerPotParticle.find( KFlowerPot::ST_BEST + 1 );
        else                            // 싹인가??
            mit = m_pObjectInfo->mapFlowerPotParticle.find( m_pkFlowerPot->m_nSeedType );
        
        // 파티클 정보 없음 
        if( mit == m_pObjectInfo->mapFlowerPotParticle.end() )
            return;

        m_pFlowerParticle = g_ParticleManager->CreateSequence( mit->second.strSquence, GetPosX() + mit->second.fX, GetPosY() + mit->second.fY );

        if( GetLayer() >=AGIT_OBJ_LAYER0 &&
            GetLayer() < AGIT_OBJ_LAYER5) {
                g_ParticleManager->SetLayer( m_pFlowerParticle, GC_LAYER_BETWEEN_BLOCK_CHAR );
        }
        else {
            g_ParticleManager->SetLayer( m_pFlowerParticle, GC_LAYER_BETWEEN_CHAR_FRONT );
        }

        if (m_pFlowerParticle){
            TRACE_INFO stTrace;
            stTrace.m_fParticleTraceTime = m_pFlowerParticle->GetKillTime();
            stTrace.m_fXOffset = mit->second.fX;
            stTrace.m_fYOffset = mit->second.fY;
            stTrace.m_pvPos = &m_dxvParticleTrace;
            m_pFlowerParticle->SetTrace( &stTrace );
        }
        
    }
}

bool KGCAgitObject::IsFlowerPot()
{
    return m_pkFlowerPot != NULL;
}

bool KGCAgitObject::IsPlantableFlowerPot()
{
    if( m_pkFlowerPot == NULL )
        return false;

    time_t tmCurTime = g_kGlobalValue.TimeToInt( g_kGlobalValue.m_tmServerTime );
    if( m_pkFlowerPot->m_nSeedType != KFlowerPot::ST_NONE && m_pkFlowerPot->m_tmFlowerTime <= tmCurTime )
        return false;

    return true;
}

bool KGCAgitObject::IsHarvestableFlowerPot()
{
    if( m_pkFlowerPot == NULL )
        return false;

    time_t tmCurTime = g_kGlobalValue.TimeToInt( g_kGlobalValue.m_tmServerTime );
    if( m_pkFlowerPot->m_nSeedType != KFlowerPot::ST_NONE && m_pkFlowerPot->m_tmFlowerTime <= tmCurTime )
        return true;

    return false;
}

void KGCAgitObject::SetFlowerState( int iState )
{
    if( m_pkFlowerPot == NULL )
        return;

    if( m_iFlowerState == iState )
        return;

    m_iFlowerState = iState;
    EventChangeFlowerState();
}

void KGCAgitObject::EventChangeFlowerState()
{
    int iSeedType = KFlowerPot::ST_NONE;
    switch( m_iFlowerState ) {
        case E_FLOWER_STATE_NONE:
            RemoveFlowerParticle();
            break;
        case E_FLOWER_STATE_BUD:
            iSeedType = KFlowerPot::ST_NONE;
            CreateFlowerParticle();
            break;
        case E_FLOWER_STATE_BLOOM:
            iSeedType = m_pkFlowerPot->m_nSeedType;
            CreateFlowerParticle();
            break;
    }

    if( m_p2DObject != NULL ) {
        m_p2DObject->SetCurrTexIndex( iSeedType + 1 );

        DWORD dwTexWidth = m_p2DObject->GetCurrTex()->GetWidth();
        DWORD dwTexHeight = m_p2DObject->GetCurrTex()->GetHeight();

        float fScaleX = static_cast<float>( dwTexWidth / 20.0f );
        float fScaleY = static_cast<float>( dwTexHeight / 20.0f );

        m_p2DObject->SetScale( TO_AGIT_COORDI( fScaleX ), TO_AGIT_COORDI( fScaleY ) );
        m_p2DObject->SetObjectOffsetX( TO_AGIT_COORDI( fScaleX * 0.5f ) );
    }
}

bool KGCAgitObject::IsMyFootHold( int iPosX_, int iPosY_ )
{
    if( m_pObjectInfo == NULL )
        return false;
    BOOST_FOREACH( KAgitFootHold& pObj, m_pObjectInfo->vecFootHold ) {
        if( (GetIndexPosX() + pObj.iPosX) != iPosX_ )
            continue;

        if( (GetIndexPosY() + pObj.iPosY + 1) != iPosY_ )
            continue;

        return true;
    }

    return false;
}

void KGCAgitObject::SetContainer( KGCAgitObjectContainer* pkContainer_ )
{
    m_pkContainer = pkContainer_;
}

int KGCAgitObject::GetRemainBloomMinute( void )
{
    time_t tmCurTime = g_kGlobalValue.TimeToInt( g_kGlobalValue.m_tmServerTime );
    time_t tmRemianTime = m_pkFlowerPot->m_tmFlowerTime - tmCurTime;
    
    int iMin = tmRemianTime / 60 ;
    if( iMin == 0 )
        iMin = 1;

    return iMin;
}

void KGCAgitObject::RemoveObjectParticle( void )
{
    std::vector< CParticleEventSeqPTR >::iterator vit;
    for( vit = m_vecObjectParticle.begin() ; vit != m_vecObjectParticle.end() ; ++vit )
    {
        g_ParticleManager->DeleteSequence( (*vit) );
        (*vit) = NULL;
    }

    m_vecObjectParticle.clear();
}

void KGCAgitObject::CreateObjectParticle( void )
{
    RemoveObjectParticle();

    if( m_pObjectInfo->vecObjectParticle.empty() == false )
    {
        std::vector< KAgitObjectParticle >::iterator vit;

        for( vit = m_pObjectInfo->vecObjectParticle.begin() ; vit != m_pObjectInfo->vecObjectParticle.end() ; ++vit )
        {
            CParticleEventSeqPTR pParticle = g_ParticleManager->CreateSequence( vit->strSquence, GetPosX() + vit->fX, GetPosY() + vit->fY );

            if( GetLayer() >=AGIT_OBJ_LAYER0 && GetLayer() < AGIT_OBJ_LAYER5)
                g_ParticleManager->SetLayer( pParticle, GC_LAYER_BETWEEN_BLOCK_CHAR );
            else 
                g_ParticleManager->SetLayer( pParticle, GC_LAYER_BETWEEN_CHAR_FRONT );


            if (pParticle){
                TRACE_INFO stTrace;
                stTrace.m_fParticleTraceTime = pParticle->GetKillTime();
                stTrace.m_fXOffset = vit->fX;
                stTrace.m_fYOffset = vit->fY;
                stTrace.m_pvPos = &m_dxvParticleTrace;
                pParticle->SetTrace( &stTrace );

                m_vecObjectParticle.push_back( pParticle );

            }
            
        }
    }
}

void KGCAgitObject::RemoveAllParticle( void )
{
    RemoveFlowerParticle();
    RemoveObjectParticle();
}

void KGCAgitObject::SetTrainingObjInfo( const KTrainingObj& info_,bool ImgUpdate_ )
{
    if( NULL == m_pkTraingObj )
        m_pkTraingObj = new KTrainingObj();

    if( m_ItemUID != info_.m_ItemUID )
        return;

    m_pkTraingObj->m_ItemUID = info_.m_ItemUID;
    m_pkTraingObj->m_nAccCount = info_.m_nAccCount;
    m_pkTraingObj->m_tmDate = info_.m_tmDate;

    if( ImgUpdate_ )
        UpdateTrainingTile( true );
}

void KGCAgitObject::UpdateTrainingTile( bool bUpdata_ )
{
    if( NULL == m_pkTraingObj )
        return;

    if( NULL == m_p2DObject )
        return;

    if( bUpdata_ && m_pkTraingObj->m_nAccCount != 0 )
    {
        ClearSubObject();
        int iCount = 0;
        for( std::vector< KAgit2DSubObject >::iterator it = m_pObjectInfo->vec2DSubObject.begin() ; it != m_pObjectInfo->vec2DSubObject.end() ; ++it )
        {
            KGC2DObject* pObj = new KGC2DObject;
            
            D3DXVECTOR2 dxvPos = m_p2DObject->GetPos();
            pObj->SetPosition( dxvPos.x + TO_AGIT_COORDI( it->fX ), dxvPos.y + TO_AGIT_COORDI( it->fY ) );
            pObj->SetBouceCheckWithMap( false );

            pObj->AddTexture( it->strTexture.c_str() );

            DWORD dwTexWidth = pObj->GetCurrTex()->GetWidth();
            DWORD dwTexHeight = pObj->GetCurrTex()->GetHeight();

            float fScaleX = static_cast<float>( dwTexWidth / 20.0f );
            float fScaleY = static_cast<float>( dwTexHeight / 20.0f );

            pObj->SetScale( TO_AGIT_COORDI( fScaleX ), TO_AGIT_COORDI( fScaleY ) );
            pObj->SetObjectOffsetX( TO_AGIT_COORDI( fScaleX * 0.5f ) );
            m_vecSubObject.push_back( pObj );
           
           ++iCount;
           if( m_pkTraingObj->m_nAccCount == iCount )
               break;
        }
    }
    else if ( bUpdata_ && m_pkTraingObj->m_nAccCount == 0 ) {
        ClearSubObject();
    }
}

int KGCAgitObject::GetTrainingObjTile( void )
{
    if( NULL == m_pkTraingObj )
        return 0;

    return m_pkTraingObj->m_nAccCount;
}

void KGCAgitObject::ClearSubObject( void )
{
    for( std::vector<KGC2DObject*>::iterator it = m_vecSubObject.begin() ; it != m_vecSubObject.end() ; ++it )
    {
        (*it)->RemoveAllTexture();
        SAFE_DELETE( (*it) );
    }
    m_vecSubObject.clear();
}

D3DXVECTOR2 KGCAgitObject::GetTrainingObjPos( void )
{
    if( NULL == m_pkTraingObj )
        return D3DXVECTOR2( -1.0f, 1.0f );

    return m_p2DObject->GetPos();
}

bool KGCAgitObject::IsEnableSitDown( void )
{
    if( NULL == m_pObjectInfo ) {
        return false;
    }

    return m_pObjectInfo->bEnableSitDown;

}

void KGCAgitObject::SetObjectAction( KGCAgitObjectAction* pObjectAction )
{
    m_pObjectAction = pObjectAction;
}

void KGCAgitObject::DoAction ( const std::vector<DWORD>& vecParam )
{
    if ( m_pObjectAction )
        m_pObjectAction->DoAction( vecParam );
}

DWORD KGCAgitObject::GetUserUID()
{
    if ( !m_pObjectAction ) 
        return UINT_MAX;

    return m_pObjectAction->GetUserUID();
}

bool KGCAgitObject::IsEnableLie( void )
{
    if( NULL == m_pObjectInfo ) { 
        return false; 
    }

    return m_pObjectInfo->bEnableLie;
}
