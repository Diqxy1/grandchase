#include "GCBuffDonationManager.h"
#include "GCUI/GCMyInfoScene.h"

ImplementSingleton(GCBuffDonationManager);

GCBuffDonationManager::GCBuffDonationManager(void)
: m_dwDonationItemID( -1 )
, m_biTotalDonationPoint( 0 )
, m_nBuffStateFlag( BDBS_NONE )
, m_nRecoveryChance( 0 )
, m_biActiveBuffPoint( 0 )
, m_bParticleNeedToRefresh( true )
, m_bRenderBuffOnParticle( false )
, m_pParticleGuage( NULL )
, m_pParticleBead1( NULL )
, m_pParticleBead2( NULL )
{
    
}

GCBuffDonationManager::~GCBuffDonationManager(void)
{
}

void GCBuffDonationManager::SetBuffDonationInfo( IN KEVENT_BUFF_DONATION_LOGIN_INFO_NOT& kRecv_ )
{
    m_dwDonationItemID = kRecv_.m_dwDonationItemID;
    m_biTotalDonationPoint = kRecv_.m_biTotalDonationPoint;
    m_nRecoveryChance = kRecv_.m_nRecoveryChance;
    m_biActiveBuffPoint = kRecv_.m_biActiveBuffPoint;
    m_nBuffStateFlag = kRecv_.m_nBuffStateFlag;

    m_bParticleNeedToRefresh = true;
}

void GCBuffDonationManager::Donating( IN int nCountInput )
{
    // 수량 검사
    if( nCountInput <= 0 )
        return;

    if ( nCountInput > 1000 ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,g_pkStrLoader->GetString(STR_ID_BUFF_DONATION_STR_2) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false);
        return;
    }

    // 기부 아이템 수량 얻기
    int nCountOwn = GetDonationItemCount();

    if( nCountOwn < nCountInput ){ //수량이 부족하면 에러
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,g_pkStrLoader->GetString(STR_ID_BUFF_DONATION_STR_1) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false);
    } else { // 기부하기
        Result_ReceiveBuffDonatingReq = INT_MAX;
        KP2P::GetInstance()->Send_BuffDonationReq( m_dwDonationItemID, nCountInput );
        g_MyD3D->WaitForServerAck( Result_ReceiveBuffDonatingReq, INT_MAX, 3000, TIME_OUT_VALUE );
    }
}

int GCBuffDonationManager::GetDonationItemCount()
{
    int nCount = 0;

    KItem* pkInvenItem = NULL;
    g_pItemMgr->FindInventoryForItemID( m_dwDonationItemID / 10, &pkInvenItem);

    if( pkInvenItem ) {
        nCount = pkInvenItem->m_nCount;
    } else {
        nCount = 0;
    }

    return nCount;
}

void GCBuffDonationManager::OnEventBuffDonationTotalDataNot( IN KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kRecv_ )
{
    m_biTotalDonationPoint = kRecv_.m_biTotalDonationPoint;
    m_nBuffStateFlag = kRecv_.m_nBuffStateFlag;

    m_bParticleNeedToRefresh = true;
}

void GCBuffDonationManager::OnEventBuffDonationAck( IN KEVENT_BUFF_DONATION_ACK& kRecv_ )
{
    //int                m_nItemCount;           // 기부로 사용 된 아이템 수량
    //KItem              m_kDonationItem;        // 기부로 사용 된 아이템
    //std::vector<KItem> m_vecRewardItem;        // 기부 하고 교환 받은 아이템
    //__int64            m_biTotalDonationPoint; // 전체 기부 수량

    // 재료로 사용된 아이템 처리
    KItem* pkInvenItem = NULL;
    g_pItemMgr->FindInventory( kRecv_.m_kDonationItem.m_ItemUID, &pkInvenItem );
    if( pkInvenItem ) {
        pkInvenItem->m_nCount = kRecv_.m_kDonationItem.m_nCount;
    }

    if( pkInvenItem->m_nCount <= 0 ) { 
        g_pItemMgr->m_kInventory.RemoveItem(pkInvenItem->m_ItemUID );
        g_pItemMgr->RemoveInventoryItemList( pkInvenItem->m_ItemUID );
    }

    // 인벤토리 갱신
    if( g_MyD3D->m_pStateMachine->IsStateMyInfo() )
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

    // 교환된 아이템 처리
    g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecRewardItem, true );

    for( std::vector<KItem>::iterator it = kRecv_.m_vecRewardItem.begin(); it != kRecv_.m_vecRewardItem.end(); ++it ){
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
            L"", g_pkStrLoader->GetString( STR_ID_MISSION_DLG3 ),
            KGCUIScene::GC_MBOX_USE_NORMAL, (*it).m_ItemID * 10, (*it).m_ItemUID, false, true );
    }

    // 토탈 합계 갱신
    m_biTotalDonationPoint = kRecv_.m_biTotalDonationPoint;

    m_bParticleNeedToRefresh = true;
}

bool GCBuffDonationManager::IsBuffOn()
{
    if( m_nBuffStateFlag == BDBS_ACTIVE ) {
        return true;
    } else 
        return false;
}

void GCBuffDonationManager::SetRandomBuff()
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();

    if( pPlayer == NULL )
        return;

    // 버프를 걸어줌, 각자 버프
    pPlayer->SetMagicEffect( EGC_EFFECT_BUFF_DONATION_BUFF_1 + ( rand()%5 ), 999999.0f, 0, true );
}

void GCBuffDonationManager::FullHPMP()
{
    // 던전에서만 적용
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        return;

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if( pPlayer == NULL )
        return;

    if( rand()%100 < m_nRecoveryChance ) 
    {
        // HP
        pPlayer->SetHPFull();

        // MP
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
    }
}

float GCBuffDonationManager::GetGuageCoordinateV()
{
    // 버프가 발동되었다면 가득찬 상태로 보여주자
    //if( m_nBuffStateFlag == BDBS_ACTIVE )
    //    return 0.0f;

    if( m_biTotalDonationPoint == 0 )
        return 1.0f;

    float fPercent = static_cast<float>( m_biTotalDonationPoint ) / static_cast<float>( m_biActiveBuffPoint );

    if( fPercent >= 1.0f )
        return 0.0f;
    else
        return ( 1.0f - fPercent );
}

float GCBuffDonationManager::GetGuageScale()
{
    return ( 1.0f - GetGuageCoordinateV() );
}

void GCBuffDonationManager::ClearParticle()
{
    g_ParticleManager->DeleteSequence( m_pParticleGuage );
    g_ParticleManager->DeleteSequence( m_pParticleBead1 );
    g_ParticleManager->DeleteSequence( m_pParticleBead2 );

    m_pParticleGuage = NULL;
    m_pParticleBead1 = NULL;
    m_pParticleBead2 = NULL;

    m_bRenderBuffOnParticle = false;
    m_bParticleNeedToRefresh = true;
}

void GCBuffDonationManager::UpdateParticles()
{
    // 이벤트 기간이 아닐 경우 그리지 말자
    if( m_nBuffStateFlag == BDBS_NONE ){
        if( m_pParticleBead1 != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleBead1 );
            m_pParticleBead1 = NULL;
        }
        if( m_pParticleBead2 != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleBead2 );
            m_pParticleBead2 = NULL;
        }
        if( m_pParticleGuage != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleGuage );
            m_pParticleGuage = NULL;
        }

        return;
    }

    // 버프가 발동되었을 때 구슬에 빛나는 파티클
    if( m_bRenderBuffOnParticle == false && m_nBuffStateFlag == BDBS_ACTIVE ){
        if( m_pParticleBead1 != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleBead1 );
            m_pParticleBead1 = NULL;
        }
        if( m_pParticleBead2 != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleBead2 );
            m_pParticleBead2 = NULL;
        }
        if( m_pParticleGuage != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleGuage );
            m_pParticleGuage = NULL;
        }

        m_pParticleBead1 = g_ParticleManager->CreateSequence( "Park_FlowerSpirit_Event_01", 4.74f, 0.346f, 0.5f );
        m_pParticleBead2 = g_ParticleManager->CreateSequence( "Park_FlowerSpirit_Event_02", 4.74f, 0.346f, 0.5f );
        
        m_bRenderBuffOnParticle = true;

    } else if ( m_bRenderBuffOnParticle == true && m_nBuffStateFlag != BDBS_ACTIVE ){
        if( m_pParticleBead1 != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleBead1 );
            m_pParticleBead1 = NULL;
        }
        if( m_pParticleBead2 != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleBead2 );
            m_pParticleBead2 = NULL;
        }

        m_bRenderBuffOnParticle = false;
    }

    // 게이지에 표시되는 반짝이 파티클
    if( m_bParticleNeedToRefresh == true ) {
        if( m_pParticleGuage != NULL ){
            g_ParticleManager->DeleteSequence( m_pParticleGuage );
            m_pParticleGuage = NULL;
        }
        m_bParticleNeedToRefresh = false;

        // 버프가 발동되었으면 그리지 않음
        //if( m_nBuffStateFlag == BDBS_ACTIVE )
        //    return;

        // 5% 미만일 경우 그리지 않음 (안 이뻐 -ㅅ-)
        if( GetGuageScale() < 0.05f )
            return;

        float fPosY = -0.195f;               // 최저 위치
        float fHeightPerPercent = 0.005f;   // 1%당 파티클 y이동

        // 파티클의 y 위치를 구함
        fPosY += GetGuageScale() * 100.0f * fHeightPerPercent;

        // test
        //KLuaManager luaMgr;
        //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
        //LUA_GET_VALUE_DEF( "y", fPosY, 0.0f );

        m_pParticleGuage = g_ParticleManager->CreateSequence( "Park_FlowerSpirit_Event_03", 4.74f, fPosY, 0.5f );
    } 

}