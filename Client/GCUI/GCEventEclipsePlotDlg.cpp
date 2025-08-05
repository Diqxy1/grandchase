#include "stdafx.h"
#include "GCEventEclipsePlotDlg.h"

IMPLEMENT_CLASSNAME( KGCEventEclipsePlotDlg );
IMPLEMENT_WND_FACTORY( KGCEventEclipsePlotDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCEventEclipsePlotDlg, "event_eclipse_plot_dlg" );

KGCEventEclipsePlotDlg::KGCEventEclipsePlotDlg( void )
{
    for ( int i=0; i<MAX_BTN; ++i ) {
        m_apkBtn[i] = NULL;
    }

    for ( int i=0; i<MAX_STATIC; ++i ) {
        m_apkStatic[i] = NULL;
    }

    for ( int i=0; i<MAX_REWARD_ITEM; ++i ) {
        m_apkImgWnd[i] = NULL;
    }


    LINK_CONTROL( "btn_close", m_apkBtn[BTN_CLOSE] );
    LINK_CONTROL( "btn_reward_1", m_apkBtn[BTN_REWARD_1] );
    LINK_CONTROL( "btn_reward_2", m_apkBtn[BTN_REWARD_2] );
    LINK_CONTROL( "btn_reward_final", m_apkBtn[BTN_REWARD_FINAL] );

    LINK_CONTROL( "static_reward_1_str",  m_apkStatic[STATIC_REWARD_1_STR] );
    LINK_CONTROL( "static_reward_2_str",  m_apkStatic[STATIC_REWARD_2_STR] );
    LINK_CONTROL( "static_left_time_str", m_apkStatic[STATIC_LEFT_TIME_STR] );
    LINK_CONTROL( "static_left_time",     m_apkStatic[STATIC_LEFT_TIME] );
    LINK_CONTROL( "static_material_str",  m_apkStatic[STATIC_MATERIAL_STR] );
    LINK_CONTROL( "static_material",      m_apkStatic[STATIC_MATERIAL] );
    LINK_CONTROL( "static_reward_1_desc", m_apkStatic[STATIC_REWARD_1_DESC] );
    LINK_CONTROL( "static_reward_2_desc", m_apkStatic[STATIC_REWARD_2_DESC] );
    LINK_CONTROL( "static_reward_final_desc", m_apkStatic[STATIC_REWARD_FINAL_DESC] );

    LINK_CONTROL( "imgwnd_reward_1", m_apkImgWnd[REWARD_1] );
    LINK_CONTROL( "imgwnd_reward_2", m_apkImgWnd[REWARD_2] );
    LINK_CONTROL( "imgwnd_reward_final", m_apkImgWnd[REWARD_FINAL] );

    LINK_CONTROL( "wnd_pointbar", m_pkPointBar );


    //
    m_kStopWatch.Init();
    m_kSecondCounter.Init();

    //
    m_vecTimeRewardItem.clear();
    m_vecHuntRewardItem.clear();
    m_vecFinalRewardItem.clear();
    m_nMaxProgress = 0;
    m_nProgress = 0;
    m_HuntItemID = 0;
    m_nNecessaryHuntItemCount = 0;
    m_nHuntItemCount = 0;
    m_bGotFinalReward = true;
    m_bIsTimeToReward = false;
    m_dwLeftTimeToReward = 0;
    m_dwEventPeriod = 0;
}

KGCEventEclipsePlotDlg::~KGCEventEclipsePlotDlg( void )
{
}

void KGCEventEclipsePlotDlg::OnCreate( void )
{
    // 버튼
    for( int i=0 ; i<MAX_BTN ; ++i ) {
        m_apkBtn[i]->InitState( true, true, this );
    }

    m_apkBtn[BTN_CLOSE]->SetHotKey( DIK_ESCAPE );
    m_apkBtn[BTN_REWARD_1]->SetWndMode( D3DWM_LOCK );
    m_apkBtn[BTN_REWARD_2]->SetWndMode( D3DWM_LOCK );
    m_apkBtn[BTN_REWARD_FINAL]->SetWndMode( D3DWM_LOCK );


    // 스트링
    for( int i=0 ; i<MAX_STATIC ; ++i ) {
        m_apkStatic[i]->InitState( true );
    }

    m_apkStatic[STATIC_REWARD_1_STR]->SetFontOutline( true );
    m_apkStatic[STATIC_REWARD_2_STR]->SetFontOutline( true );
    m_apkStatic[STATIC_LEFT_TIME_STR]->SetFontOutline( true );
    m_apkStatic[STATIC_MATERIAL_STR]->SetFontOutline( true );

    m_apkStatic[STATIC_LEFT_TIME]->SetAlign( DT_CENTER );
    m_apkStatic[STATIC_MATERIAL]->SetAlign( DT_CENTER );

    m_apkStatic[STATIC_REWARD_1_DESC]->SetMultiLine( true );
    m_apkStatic[STATIC_REWARD_2_DESC]->SetMultiLine( true );
    m_apkStatic[STATIC_REWARD_FINAL_DESC]->SetMultiLine( true );

    m_apkStatic[STATIC_LEFT_TIME_STR]->SetText( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_EVENT_1 ) );
    m_apkStatic[STATIC_MATERIAL_STR]->SetText( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_EVENT_2 ) );
    m_apkStatic[STATIC_REWARD_1_DESC]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_EVENT_3 ) );
    m_apkStatic[STATIC_REWARD_2_DESC]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_EVENT_4 ) );
    m_apkStatic[STATIC_REWARD_FINAL_DESC]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_EVENT_5 ) );

    m_apkStatic[STATIC_REWARD_1_STR]->SetFontColor( 0xffffcc99 );
    m_apkStatic[STATIC_REWARD_2_STR]->SetFontColor( 0xffffcc99 );
    m_apkStatic[STATIC_LEFT_TIME]->SetFontColor( 0xffffcc00 );
    m_apkStatic[STATIC_MATERIAL]->SetFontColor( 0xffffcc00 );

    // 이미지 윈도우
    for( int i=0 ; i<MAX_REWARD_ITEM ; ++i ) {
        m_apkImgWnd[i]->InitState( true );
    }

    // 포인트 게이지바
    m_pkPointBar->InitState( true );

    // 서버에 이벤트 정보 요청
    if( m_kStopWatch.IsStop() && SiGCEclipseEventManager()->IsEventTime() == true ){
        Result_ReceiveEclipsePlotReward = INT_MAX;
        KP2P::GetInstance()->Send_EclipsePlotEventInfoReq();
        g_MyD3D->WaitForServerAck( Result_ReceiveEclipsePlotReward, INT_MAX, 1000, TIME_OUT_VALUE );
        m_kStopWatch.SetStopWatch( 5000 );

        // 이벤트 주기
        m_dwEventPeriod = SiGCEclipseEventManager()->GetEventTimePeriod();
    }

    Update();
}

void KGCEventEclipsePlotDlg::FrameMoveInEnabledState( void )
{
    // 남은시간 갱신
    UpdateTime();
}

void KGCEventEclipsePlotDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_apkBtn[BTN_CLOSE],          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
    _GCWND_MSG_MAP( m_apkBtn[BTN_REWARD_1],       KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRewardTime );
    _GCWND_MSG_MAP( m_apkBtn[BTN_REWARD_2],       KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRewardHunt );
    _GCWND_MSG_MAP( m_apkBtn[BTN_REWARD_FINAL],   KD3DWnd::D3DWE_BUTTON_CLICK, OnClickRewardFinal );
}

void KGCEventEclipsePlotDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCEventEclipsePlotDlg::OnClickRewardTime()
{
    if ( m_bIsTimeToReward == false )
        return;
    else
        m_bIsTimeToReward = false;

    Result_ReceiveEclipsePlotReward = INT_MAX;
    KP2P::GetInstance()->Send_EclipsePlotEventTimeRewardReq();
    g_MyD3D->WaitForServerAck( Result_ReceiveEclipsePlotReward, INT_MAX, 2000, TIME_OUT_VALUE );

    Update();
}

void KGCEventEclipsePlotDlg::OnClickRewardHunt()
{
    KEVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ kPacket_;

    kPacket_.m_ItemID = m_HuntItemID;
    kPacket_.m_nItemCount = m_nHuntItemCount;

    Result_ReceiveEclipsePlotReward = INT_MAX;
    KP2P::GetInstance()->Send_EclipsePlotEventHuntRewardReq( kPacket_ );
    g_MyD3D->WaitForServerAck( Result_ReceiveEclipsePlotReward, INT_MAX, 2000, TIME_OUT_VALUE );
}

void KGCEventEclipsePlotDlg::OnClickRewardFinal()
{
    Result_ReceiveEclipsePlotReward = INT_MAX;
    KP2P::GetInstance()->Send_EclipsePlotEventFinalRewardReq();
    g_MyD3D->WaitForServerAck( Result_ReceiveEclipsePlotReward, INT_MAX, 2000, TIME_OUT_VALUE );
}

void KGCEventEclipsePlotDlg::OnEventEclipsePlotInfoAck( KEVENT_ECLIPSE_PLOT_INFO_ACK& kRecv_ )
{
    m_vecTimeRewardItem = kRecv_.m_vecTimeRewardItem;
    m_vecHuntRewardItem = kRecv_.m_vecHuntRewardItem;
    m_vecFinalRewardItem = kRecv_.m_vecFinalRewardItem;
    m_nMaxProgress = kRecv_.m_nMaxProgress;
    m_nProgress = kRecv_.m_nProgress;
    m_HuntItemID = kRecv_.m_prHuntItem.first;
    m_nNecessaryHuntItemCount = kRecv_.m_prHuntItem.second;
    m_bGotFinalReward = kRecv_.m_bGotFinalReward;

    Update();
}

void KGCEventEclipsePlotDlg::Update()
{
    // 보상1 남은시간 갱신
    UpdateTime();
    
    // 보상2 현재 아이템 수량 갱신
    UpdateHuntItem();

    // 진행도에 따른 포인트 막대 길이
    UpdateProgress();

    // 보상 / 보상아이템 정보
    UpdateRewardInfo();
}

void KGCEventEclipsePlotDlg::UpdateRewardInfo()
{
    // 이벤트 체크
    if ( SiGCEclipseEventManager()->IsEventTime() == false ) {
        return;
    }

    if( m_vecTimeRewardItem.empty() || m_vecHuntRewardItem.empty() || m_vecFinalRewardItem.empty() ) {
        return;
    }

    // 보상 아이템 이름
#if defined( NATION_KOREA ) 
    m_apkStatic[STATIC_REWARD_1_STR]->SetText( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_REWARD_DESC1 ) );
    m_apkStatic[STATIC_REWARD_2_STR]->SetText( g_pkStrLoader->GetString( STR_ID_ECLIPSE_PLOT_REWARD_DESC2 ) );
#else 
    GCItem* m_kItemData;
    m_kItemData = g_MyD3D->m_kItemMgr.GetItemData( m_vecTimeRewardItem[0].m_ItemID / 10 );
    m_apkStatic[STATIC_REWARD_1_STR]->SetText( m_kItemData->strItemName );
    m_kItemData = g_MyD3D->m_kItemMgr.GetItemData( m_vecHuntRewardItem[0].m_ItemID / 10 );
    m_apkStatic[STATIC_REWARD_2_STR]->SetText( m_kItemData->strItemName );
#endif

    // 보상아이템 이미지
    m_apkImgWnd[REWARD_1]->SetItemInfo( m_vecTimeRewardItem[0].m_ItemID / 10 );
    m_apkImgWnd[REWARD_1]->ShowItem ( true );

    m_apkImgWnd[REWARD_2]->SetItemInfo( m_vecHuntRewardItem[0].m_ItemID / 10 );
    m_apkImgWnd[REWARD_2]->ShowItem ( true );

    m_apkImgWnd[REWARD_FINAL]->SetItemInfo( m_vecFinalRewardItem[0].m_ItemID / 10 );
    m_apkImgWnd[REWARD_FINAL]->ShowItem ( true );

    // 보상버튼 모드
    if( m_bIsTimeToReward == false )
        m_apkBtn[BTN_REWARD_1]->SetWndMode( D3DWM_LOCK );
    else
        m_apkBtn[BTN_REWARD_1]->SetWndMode( D3DWM_DEFAULT );

    if( m_nHuntItemCount >= m_nNecessaryHuntItemCount )
        m_apkBtn[BTN_REWARD_2]->SetWndMode( D3DWM_DEFAULT );
    else
        m_apkBtn[BTN_REWARD_2]->SetWndMode( D3DWM_LOCK );

    if( m_bGotFinalReward == true )
        m_apkBtn[BTN_REWARD_FINAL]->SetWndMode( D3DWM_LOCK );
    else if( m_nProgress >= m_nMaxProgress )
        m_apkBtn[BTN_REWARD_FINAL]->SetWndMode( D3DWM_DEFAULT );
    else
        m_apkBtn[BTN_REWARD_FINAL]->SetWndMode( D3DWM_LOCK );
}


void KGCEventEclipsePlotDlg::UpdateTime()
{
    // 보상 버튼이 활성화 된 상태에서 보상을 받지 않았으면 다음 시간 표시 x
    if( m_bIsTimeToReward ){
        m_apkStatic[STATIC_LEFT_TIME]->SetText( L"00 : 00" );
        return;
    }

    if( m_kSecondCounter.IsStop() == false )
        return;
    else {
        // 시간보상까지 남은 시간
        if( SiGCEclipseEventManager()->IsEventTime() ) {
            m_dwLeftTimeToReward = m_dwEventPeriod - SiGCEclipseEventManager()->GetLeftEventTime() + 60;

            // 잘못된 값이 들어올 경우
            if( m_dwEventPeriod < m_dwLeftTimeToReward )    m_dwLeftTimeToReward = 0;
            else if ( m_dwLeftTimeToReward < 0 )            m_dwLeftTimeToReward = 0;

        } else {
            m_dwLeftTimeToReward = 0;
        }

        // 시간 표시
        static std::wstring strTime;

        strTime = boost::str( boost::wformat(L"%02d : %02d")
            % static_cast<int>( m_dwLeftTimeToReward / 3600 )
            % static_cast<int>( m_dwLeftTimeToReward / 60 )
        );

        m_apkStatic[STATIC_LEFT_TIME]->SetText( strTime );

        m_kSecondCounter.SetStopWatch( 5000 ); // 5초마다 갱신
    }
}

void KGCEventEclipsePlotDlg::UpdateHuntItem()
{
    // 아이템 수량 얻기
    KItem* pkInvenItem = NULL;
    g_pItemMgr->FindInventoryForItemID( m_HuntItemID / 10, &pkInvenItem);

    if( pkInvenItem ) {
        m_nHuntItemCount = pkInvenItem->m_nCount;
    } else {
        m_nHuntItemCount = 0;
    }

    // 수량 표시
    std::wstring strMaterial;

    strMaterial = boost::str( boost::wformat(L"%d / %d")
        % m_nHuntItemCount
        % m_nNecessaryHuntItemCount
    );

    m_apkStatic[STATIC_MATERIAL]->SetText( strMaterial );
}

void KGCEventEclipsePlotDlg::UpdateProgress()
{
    float fRate;

    if( m_nMaxProgress == 0 ) fRate = 0.0f;
    else fRate = static_cast<float>(m_nProgress) / static_cast<float>(m_nMaxProgress);

    if( fRate > 1.0f )         fRate = 1.0f;
    else if( fRate < 0.0f )    fRate = 0.0f;

    float fLength = MAX_POINT_BAR_LENGTH * fRate;
    m_pkPointBar->SetWidth( static_cast<DWORD>( fLength ) );
}

void KGCEventEclipsePlotDlg::OnEventEclipsePlotTimeRewardReadyNot()
{
    m_bIsTimeToReward = true;

    m_apkBtn[BTN_REWARD_1]->SetWndMode( D3DWM_DEFAULT );

    Update();
}

void KGCEventEclipsePlotDlg::OnEventEclipsePlotHuntRewardAck( KEVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK& kRecv_ )
{
    // 잠시 비활성화
    m_apkBtn[BTN_REWARD_2]->SetWndMode( D3DWM_LOCK );

    // 인벤토리에 있는 재료 아이템 수량 갱신
    KItem* pkInvenItem = NULL;
    g_pItemMgr->FindInventoryForItemID( kRecv_.m_kHuntItem.m_ItemID / 10, &pkInvenItem);

    if( pkInvenItem ) {
        pkInvenItem->m_nCount = kRecv_.m_kHuntItem.m_nCount;
        
    // 0개 라면 삭제
    if( pkInvenItem->m_nCount == 0 )
        g_pItemMgr->m_kInventory.RemoveItem( pkInvenItem->m_ItemUID );
    }
    g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecReward, true );

    if( g_MyD3D->m_pStateMachine->IsStateMyInfo() )
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

    // 보상받았다는 창을 띄움
    for( UINT i = 0; i < kRecv_.m_vecReward.size(); ++i ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
            L"", g_pkStrLoader->GetString(STR_ID_THE_GIFT_HAS_ARRIVED),
            KGCUIScene::GC_MBOX_USE_NORMAL, kRecv_.m_vecReward[i].m_ItemID * 10, kRecv_.m_vecReward[i].m_nCount, false, true );
    }

    // 진행도 갱신
    m_nProgress = kRecv_.m_nProgress;

    Update();

}

void KGCEventEclipsePlotDlg::OnEventEclipsePlotTimeRewardAck( KEVENT_ECLIPSE_PLOT_TIME_REWARD_ACK& kRecv_ )
{
    // 연속해서 받을 수 없을테니 
    m_bIsTimeToReward = false;

    // 잠시 비활성화
    m_apkBtn[BTN_REWARD_1]->SetWndMode( D3DWM_LOCK );

    // 보상받았다는 창을 띄움
    for( UINT i = 0; i < kRecv_.m_vecReward.size(); ++i ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
            L"", g_pkStrLoader->GetString(STR_ID_THE_GIFT_HAS_ARRIVED),
            KGCUIScene::GC_MBOX_USE_NORMAL, kRecv_.m_vecReward[i].m_ItemID * 10, kRecv_.m_vecReward[i].m_nCount, false, true );
    }

    // 진행도 갱신
    m_nProgress = kRecv_.m_nProgress;

    // 시간보상까지 남은 시간 다시!!
    if( SiGCEclipseEventManager()->IsEventTime() ) {
        m_dwLeftTimeToReward = SiGCEclipseEventManager()->GetLeftEventTime();
    }

    Update();
}

void KGCEventEclipsePlotDlg::OnEventEclipsePlotFinalRewardAck( KEVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK& kRecv_ )
{
    // 최종 보상을 받았음
    m_bGotFinalReward = true;

    // 보상받았다는 창을 띄움
    for( UINT i = 0; i < kRecv_.m_vecReward.size(); ++i ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
            L"", g_pkStrLoader->GetString(STR_ID_THE_GIFT_HAS_ARRIVED),
            KGCUIScene::GC_MBOX_USE_NORMAL, kRecv_.m_vecReward[i].m_ItemID * 10, kRecv_.m_vecReward[i].m_nCount, false, true );
    }

    Update();
}
