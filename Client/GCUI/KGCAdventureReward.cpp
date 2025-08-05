#include "stdafx.h"
#include "KGCAdventureReward.h"
#include "KGCAdventureRewardItemWnd.h"

IMPLEMENT_CLASSNAME( KGCAdventureReward );
IMPLEMENT_WND_FACTORY( KGCAdventureReward );
IMPLEMENT_WND_FACTORY_NAME( KGCAdventureReward, "gc_adventure_reward" );

KGCAdventureReward::KGCAdventureReward( void )
: m_pkRewardName( NULL )
, m_pkFinalRewardName(NULL)
, m_pkRewardBtn( NULL )
, m_pkContinentRewardImgWnd( NULL )
, m_bMouseBound(NULL)
, m_dwItemID(NULL)
, m_pkStaticContinentDesc(NULL)
, m_pkWholeContinentImage(NULL)
{
    LINK_CONTROL( "reward_name", m_pkRewardName );
    LINK_CONTROL( "final_reward_name", m_pkFinalRewardName );
    LINK_CONTROL( "reward_btn", m_pkRewardBtn );
    LINK_CONTROL( "continent_reward", m_pkContinentRewardImgWnd );
    LINK_CONTROL( "static_continent_desc", m_pkStaticContinentDesc );
    LINK_CONTROL( "whole_of_continent", m_pkWholeContinentImage );

    for( int i =0; i < MAX_CONTINENT_REWARD_ITEM_NUM; i++){
        char strTemp[ MAX_PATH ] = "";
        m_apkAdventureRewardItemBox[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "item_wnd%d", i);
        LINK_CONTROL(strTemp,m_apkAdventureRewardItemBox[i]);
    }

    for( int i =0; i < GCAdventureManager::CONTINENT_MAX; i++){
        char strTemp[ MAX_PATH ] = "";
        m_saCheckContinent[i] = NULL;
        sprintf_s( strTemp, MAX_PATH, "check_continent%d", i);
        LINK_CONTROL(strTemp,m_saCheckContinent[i]);
    }

    m_mapCollectItems.clear();
    m_mapCollectData.clear();
}

KGCAdventureReward::~KGCAdventureReward( void )
{
}

void KGCAdventureReward::OnCreate( void )
{
    m_pkRewardName->InitState( true, true, this );
    m_pkFinalRewardName->InitState( false, true, this );
    m_pkRewardBtn->InitState( true, true, this );
    m_pkRewardBtn->DisconnectAllEventProcedure();
    m_pkRewardBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCAdventureReward::OnClickRewardBtn );

    m_pkStaticContinentDesc->SetText(L"");
    m_pkStaticContinentDesc->SetAlign( DT_CENTER );
    m_pkContinentRewardImgWnd->InitState( true, true, this );
    m_pkWholeContinentImage->InitState( false, true, this );

    // 아이템 박스
    for ( int i = 0; i < MAX_CONTINENT_REWARD_ITEM_NUM; ++i )
    {
        m_apkAdventureRewardItemBox[i]->InitState( true, true, this );
    }

    //각 대륙 보상 받은때 대륙 켜지는 UI 
    for( int i =0; i < GCAdventureManager::CONTINENT_MAX; i++) {
        m_saCheckContinent[i]->InitState( false, true, this );
    }

}

void KGCAdventureReward::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkContinentRewardImgWnd, KD3DWnd::D3DWE_CURSOR_ENTER, OnMouseEnter );
    GCWND_MSG_MAP( m_pkContinentRewardImgWnd, KD3DWnd::D3DWE_CURSOR_LEAVE, OnMouseLeave );
}

void KGCAdventureReward::FrameMoveInEnabledState( void )
{
    ShowItemInfo();
}

void KGCAdventureReward::ShowItemInfo( void )
{
    if( m_bMouseBound ) {
        GCItem* pItem = g_pItemMgr->GetItemData( m_dwItemID / 10 );

        if( NULL == pItem ) { 
            return;
        }

        if( pItem->dwGoodsID == KItem::INVALID_ITEM ) { 
            return;
        }

        D3DXVECTOR2 dxvPos = m_pkContinentRewardImgWnd->GetCurrentWindowPos();

        dxvPos.x *= m_fWindowScaleX;
        dxvPos.y *= m_fWindowScaleY;
        dxvPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
    }
}

void KGCAdventureReward::OnMouseEnter( void )
{
    m_bMouseBound = true;
}

void KGCAdventureReward::OnMouseLeave( void )
{
    m_bMouseBound = false;
}

//대륙 보상 아이템 정보 세팅
void KGCAdventureReward::SetRewardItemBoxInfo( std::vector< KDropItemInfo > vecContinentReward )
{
   std::vector< KDropItemInfo > ::iterator vit = vecContinentReward.begin();
    m_dwItemID = vit->m_ItemID;

    m_pkContinentRewardImgWnd->SetItemInfo( vit->m_ItemID / 10 );
    m_pkContinentRewardImgWnd->ShowItem ( true );
    m_pkStaticContinentDesc->SetForceWordWrapText( g_pItemMgr->GetItemName( vit->m_ItemID / 10) , true );
    m_pkStaticContinentDesc->SetToolTip( g_pItemMgr->GetItemName( vit->m_ItemID / 10) );
}

//대륙 최종 보상 아이템 정보 세팅
void KGCAdventureReward::SetFinalRewardItemBoxInfo()
{
    m_dwItemID = SiGCAdventureManager()->GetFinalRewardItemGoodsID();

    m_pkContinentRewardImgWnd->SetItemInfo( SiGCAdventureManager()->GetFinalRewardItemGoodsID() / 10 );
    m_pkContinentRewardImgWnd->ShowItem ( true );
    m_pkStaticContinentDesc->SetForceWordWrapText( g_pItemMgr->GetItemName( SiGCAdventureManager()->GetFinalRewardItemGoodsID() / 10) , true );
    m_pkStaticContinentDesc->SetToolTip( g_pItemMgr->GetItemName( SiGCAdventureManager()->GetFinalRewardItemGoodsID() / 10) );
}

//최종 보상 획득시, 문구를 띄워준다.
void KGCAdventureReward::OnClickRewardBtn()
{
    g_KDSound.Play( "31" );
    if ( SiGCAdventureManager()->GetCurrentContinent() == WHOLE_CONTINENT)
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_GC_ADVENTURE_ASK_FINAL_REWARD ),
            L"", KGCUIScene::GC_MBOX_USE_GC_ADVENTURE_FINAL_REWARD , 0, 0, false, true );
    }
    else{
        SiGCAdventureManager()->ContinentRewardReq( SiGCAdventureManager()->GetCurrentContinent() );
    }
}

//각 대륙의 전리품 획득 정보를 세팅 한다.
void KGCAdventureReward::SetContinentInfo( int iIndex)
{
    m_mapCollectItems.clear();
    m_mapCollectData.clear();

    if ( false == SiGCAdventureManager()->GetContinentItemInfo( iIndex , m_mapCollectItems ) )
        return;
    if ( false == SiGCAdventureManager()->GetObtainCnt( iIndex , m_mapCollectData ))
        return;
    
    for ( int i = 0; i < MAX_CONTINENT_REWARD_ITEM_NUM; ++i )
    {
        if ( i == static_cast<int> ( m_mapCollectItems[i].m_dwCollectID ) ) {
            m_apkAdventureRewardItemBox[i]->SetRewardItemBoxInfo( m_mapCollectItems[i].m_dwDisplayItemID );
            m_apkAdventureRewardItemBox[i]->SetContinentObtainCnt ( m_mapCollectData[static_cast<DWORD>(i)] , m_mapCollectItems[i].m_dwMaxCount );
            m_apkAdventureRewardItemBox[i]->IsRender( true );
        }
        else 
        {
            m_apkAdventureRewardItemBox[i]->IsRender( false );
        }
    }
}

void KGCAdventureReward::CheckContinentRewardBtn ( int iContinentID )
{
    //전체 대륙 선택중이 아니고, 각 대륙에 보상을 획득 할 수 있나?
    if ( iContinentID != WHOLE_CONTINENT && SiGCAdventureManager()->CanObtainContinentReward( iContinentID ) )
        m_pkRewardBtn->SetWndMode(D3DWM_DEFAULT);
    //전체 대륙을 선택했고, 최종 보살을 받을 상태 이면, 리워드 버튼 활성화
    else if ( SiGCAdventureManager()->CanObtainFinalReward() && SiGCAdventureManager()->GetCurrentContinent() == WHOLE_CONTINENT)
        m_pkRewardBtn->SetWndMode(D3DWM_DEFAULT);
    else
        m_pkRewardBtn->SetWndMode( D3DWM_LOCK );
    
}

void KGCAdventureReward::WholeContinentRender( bool bRender )
{
    m_pkFinalRewardName->ToggleRender( bRender);
    m_pkWholeContinentImage->ToggleRender( bRender);
}

//전체 대륙 클릭시, 각 대륙의 보상을 받았는지 체크 하고, Render를 해준다.
void KGCAdventureReward::CheckContinentReward()
{
    for( int i =0; i < GCAdventureManager::CONTINENT_MAX; i++) {
        bool bRender = SiGCAdventureManager()->CheckContinentReward( i );
        m_saCheckContinent[i]->ToggleRender( bRender );
    }
}

//최종 보상 받고, 체크된 대륙 이미지들을 초기화 시켜준다.
void KGCAdventureReward::OffCheckContinentImage()
{
    for( int i =0; i < GCAdventureManager::CONTINENT_MAX; i++) {
        m_saCheckContinent[i]->ToggleRender( false );
    }
}

void KGCAdventureReward::RewardBtnWndModeLock()
{
    m_pkRewardBtn->SetWndMode( D3DWM_LOCK );
}
