#include "stdafx.h"
#include "GCChristmasGiftDlg.h"
#include "GCChristmasDonateDlg.h"
#include "GCChristmasSendGiftDlg.h"
#include "GCChristmasGiftItemBox.h"
#include "GCChristmasRewardBox.h"



IMPLEMENT_CLASSNAME( KGCChristmasGiftDlg );
IMPLEMENT_WND_FACTORY( KGCChristmasGiftDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCChristmasGiftDlg, "event_christmas_gift_dlg" );

KGCChristmasGiftDlg::KGCChristmasGiftDlg( void )
{
    m_pkBtnDonate = NULL;
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_donate", m_pkBtnDonate );
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkStaticMerrySay = NULL;
    m_pkStaticMySledgeStr = NULL;
    m_pkStaticMySledgeCount = NULL;
    m_pkStaticMyPointStr = NULL;
    m_pkStaticMyPointCount = NULL;
    LINK_CONTROL( "static_merry_say", m_pkStaticMerrySay );
    LINK_CONTROL( "static_my_point_str", m_pkStaticMySledgeStr );
    LINK_CONTROL( "static_my_point_cnt", m_pkStaticMySledgeCount );
    LINK_CONTROL( "static_my_mileagee_str", m_pkStaticMyPointStr );
    LINK_CONTROL( "static_my_mileagee_cnt", m_pkStaticMyPointCount );

    m_pkBonusPointBar = NULL;
    LINK_CONTROL( "bonus_point_bar", m_pkBonusPointBar );

    m_vecMerryMent.clear();

    for( int i=0 ; i<MAX_CHRISTMAS_GIFT_KIND ; ++i )
    {
        m_apkChristmasGiftItemBox[i] = NULL;

        char szWndName[100] = {0, };
        sprintf_s( szWndName, 99, "christmas_gift_item_box_%d", i );
        LINK_CONTROL( szWndName, m_apkChristmasGiftItemBox[i] );
    }

    for( int i=0 ; i<MAX_CHRISTMAS_REWARD ; ++i )
    {
        m_apkChristmasRewards[i] = NULL;

        char szWndName[100] = {0, };
        sprintf_s( szWndName, 99, "christmas_reward_%d", i );
        LINK_CONTROL( szWndName, m_apkChristmasRewards[i] );
    }


    m_pkChristmasDonateDlg = NULL;
    LINK_CONTROL( "christma_donate_box", m_pkChristmasDonateDlg );

    m_pkChristmasSendGiftDlg = NULL;
    LINK_CONTROL( "send_christmas_gift_box", m_pkChristmasSendGiftDlg );



    // 맴버 데이터 초기화
    m_SledgeItemID = 0;
    m_mapPresentItemInfo.clear();
    m_mapDonationInfo.clear();
    m_mapGradeRewardList.clear();
    m_mapGradeDesc.clear();

    m_dwSledgeCount = 0;
    m_dwRecentlyRecievedRewardGrade = 1;
    m_dwBonusPoint = 0;

    m_vecKeepMsg.clear();


}

KGCChristmasGiftDlg::~KGCChristmasGiftDlg( void )
{
}


void KGCChristmasGiftDlg::OnCreate( void  )
{
    // 버튼
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey (DIK_ESCAPE );

#if !defined( NATION_KOREA ) && !defined( NATION_IDN ) && !defined( NATION_USA ) && !defined( NATION_PHILIPPINE ) && !defined (NATION_BRAZIL) && !defined( NATION_TAIWAN ) && !defined( NATION_LATINAMERICA )
    m_pkBtnDonate->InitState( true, true, this );
#else
    m_pkBtnDonate->InitState( false );
#endif


    // 스트링
    m_pkStaticMerrySay->InitState( true );
    m_pkStaticMerrySay->SetAlign( DT_LEFT );
    m_pkStaticMerrySay->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));
    m_pkStaticMerrySay->SetMultiLine( true );
    
    m_pkStaticMySledgeStr->InitState( true );
    m_pkStaticMySledgeStr->SetAlign( DT_LEFT );
    m_pkStaticMySledgeStr->SetFontOutline( true );

    m_pkStaticMySledgeCount->InitState( true );
    m_pkStaticMySledgeCount->SetAlign( DT_RIGHT );
    m_pkStaticMySledgeCount->SetText( 0 );

    m_pkStaticMyPointStr->InitState( true );
    m_pkStaticMyPointStr->SetAlign( DT_LEFT );
    m_pkStaticMyPointStr->SetFontOutline( true );

    m_pkStaticMyPointCount->InitState( true );
    m_pkStaticMyPointCount->SetAlign( DT_RIGHT );
    m_pkStaticMyPointCount->SetText( 0 );

    m_pkBonusPointBar->InitState( true );

    // 선물 박스
    for ( int i = 0; i < MAX_CHRISTMAS_GIFT_KIND; ++i )
    {
        m_apkChristmasGiftItemBox[i]->InitState( true, true, this );
    }

    // 보상 목록
    for ( int i = 0; i < MAX_CHRISTMAS_REWARD; ++i )
    {
        m_apkChristmasRewards[i]->InitState( true, true, this );
    }

    // 기부하기
    m_pkChristmasDonateDlg->InitState( false, true, this );

    // 선물 보내기
    m_pkChristmasSendGiftDlg->InitState( false, true, this );


    // 서버에 이벤트 정보 요청
    KP2P::GetInstance()->Send_CharismasEventInfoReq();

    // 크리스마스 선물 이벤트 스크립트 로드
    LoadChristmasGiftEventInfo();

    // 리본으로 바뀌었으나, 원래는 썰매(Sledge) 였음.
    SetSledgeCount();

    // 이벤트 정보 세팅
    InitEventInfo();

    GetCurrentGameState = g_MyD3D->m_pStateMachine->GetState();
}


void KGCChristmasGiftDlg::SetSledgeCount()
{
    KItem* pkInvenItem = NULL;
    g_pItemMgr->FindInventoryForItemID( m_SledgeItemID / 10, &pkInvenItem);

    if( pkInvenItem ) {
        m_dwSledgeCount = pkInvenItem->m_nCount;
    } else {
        m_dwSledgeCount = 0;
    }
}


void KGCChristmasGiftDlg::InitEventInfo()
{
    // 리본 갯수
    m_pkStaticMySledgeStr->SetText( g_pkStrLoader->GetString( STR_ID_CURRENT_QUANTITY_OF_RIBBON ) );
    m_pkStaticMySledgeCount->SetText( static_cast<int>( m_dwSledgeCount ) );

    // 착한 일 점수
    m_pkStaticMyPointStr->SetText( g_pkStrLoader->GetString( STR_ID_CURRENT_MY_GOOD_JOB_POINT ) );
    m_pkStaticMyPointCount->SetText( static_cast<int>( m_dwBonusPoint ) );

    // 보너스 포인트 막대 갱신

    float fRate;
    if( m_dwBonusPoint == 0 ){
        fRate = 0.0f;
    } else if ( m_dwBonusPoint == m_mapGradeDesc[MAX_CHRISTMAS_REWARD].second ){
        fRate = 1.0f;
    } else {
        for ( UINT iGrade = 1; iGrade <= m_mapGradeDesc.size(); ++iGrade ){
            if( m_dwBonusPoint > m_mapGradeDesc[iGrade].second ){
                continue;
            } else {
                fRate = ( 1 / static_cast<float>(MAX_CHRISTMAS_REWARD) ) *
                    ( static_cast<float>( iGrade - 1 ) + ( static_cast<float>( m_dwBonusPoint - m_mapGradeDesc[iGrade].first ) / static_cast<float>( m_mapGradeDesc[iGrade].second - m_mapGradeDesc[iGrade].first ) ) );
                break;
            }
        }
    }




    static float fOriginalPosY = m_pkBonusPointBar->GetFixedWindowLocalPos().y;
    static float fOriginalHeight = static_cast<float>( m_pkBonusPointBar->GetHeight() );

    float fLength = fOriginalHeight * fRate;
    m_pkBonusPointBar->SetHeight( static_cast<DWORD>( fLength ) );
    m_pkBonusPointBar->SetWindowPosY( fOriginalPosY - fLength + fOriginalHeight );



    int i;
    // GiftItem
    std::map<KDropItemInfo,std::pair<DWORD,DWORD>>::iterator itrGift;
    for( i = 0, itrGift = m_mapPresentItemInfo.begin(); itrGift != m_mapPresentItemInfo.end(); ++itrGift ){
        m_apkChristmasGiftItemBox[i++]->SetGiftItemBoxInfo( itrGift->first.m_ItemID, itrGift->second.first );
    }

    // Reward
    std::map<DWORD,VEC_REWARD>::iterator itrReward;
    for( i = 0, itrReward = m_mapGradeRewardList.begin(); itrReward != m_mapGradeRewardList.end(); ++itrReward ){
        m_apkChristmasRewards[i++]->SetRewardInfo( itrReward->first, itrReward->second );
    }

    // 현재 포인트를 기준으로 버튼 활성화 비활성화를 세팅하기
    for( DWORD i = 0; i < MAX_CHRISTMAS_REWARD; ++i ){
        if( ( i+1 == MAX_CHRISTMAS_REWARD && m_dwBonusPoint >= m_mapGradeDesc[i+1].second ) || 
            ( m_dwBonusPoint > m_mapGradeDesc[i+1].second ) ) { 
            if( (i+1) < m_dwRecentlyRecievedRewardGrade )
                m_apkChristmasRewards[i]->SetRewardState( CRRS_ALREADY_GOT_REWARD );
            else
                m_apkChristmasRewards[i]->SetRewardState( CRRS_GET_REWARD );
        }
        else{
            m_apkChristmasRewards[i]->SetRewardState( CRRS_NOT_ENOUGH_POINT );
        }

        m_apkChristmasRewards[i]->InitRewardBox();
    }


}

void KGCChristmasGiftDlg::ActionPerformed( const KActionEvent& event )
{
#if !defined( NATION_KOREA ) && !defined( NATION_USA ) && !defined (NATION_BRAZIL)
    _GCWND_MSG_MAP( m_pkBtnDonate,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickDonate );
#endif
    _GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
}

void KGCChristmasGiftDlg::FrameMoveInEnabledState( void )
{
    if (GetCurrentGameState != g_MyD3D->m_pStateMachine->GetState())
    {
        OnClickClose();
    }
    MerrySayFrameMove();
}


void KGCChristmasGiftDlg::OnClickClose()
{
    if( m_pkChristmasDonateDlg->IsRenderOn() ){
        m_pkChristmasDonateDlg->ToggleRender( false );
    }
    else if( m_pkChristmasSendGiftDlg->IsRenderOn() ){
        m_pkChristmasSendGiftDlg->ToggleRender( false );
    }
    else
        SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}


void KGCChristmasGiftDlg::OnClickDonate()
{
    g_KDSound.Play( "31" );
    m_pkChristmasDonateDlg->ToggleRender( true );
}


void KGCChristmasGiftDlg::MerrySayFrameMove()
{
    static DWORD dwOldTime = GetTickCount();
    static int iCurrentMent = 0;

    DWORD dwCurrentTime = GetTickCount();
    if(dwCurrentTime > dwOldTime + MENT_REFRESH_TIME){
        dwOldTime = dwCurrentTime;

        // 맨트 바꾸기, 중복 x 랜덤 o
        int iNextMent;
        while( 1 ){
            iNextMent = rand() % m_vecMerryMent.size();

            if( iNextMent != iCurrentMent ){
                iCurrentMent = iNextMent;
                break;
            }
        }
    }
    else{
        m_pkStaticMerrySay->SetTextAutoMultiline( g_pkStrLoader->GetString( m_vecMerryMent[iCurrentMent] ) );
    }
}


void KGCChristmasGiftDlg::LoadChristmasGiftEventInfo()
{
    if( m_vecMerryMent.empty() )
    {
        KLuaManager luaMgr;

        if( !GCFUNC::LoadLuaScript( luaMgr, "ChristmasGiftEventInfo.lua", true ) )
            return;

        // 매리 맨트 정보
        if( SUCCEEDED( luaMgr.BeginTable( "MERRY_MENTS" ) ) ){
            // 매리 
            for( int i = 1; ; ++i ) {
                int iStringID;

                if( FAILED( luaMgr.GetValue(i, iStringID) ) )
                    break;

                m_vecMerryMent.push_back( iStringID );
            }
        }
        luaMgr.EndTable();
    }
}


void KGCChristmasGiftDlg::OnEventChristmasEventNot( KEVENT_CHARISMAS_EVENT_NOT& kRecv_ )
{
    m_SledgeItemID = kRecv_.m_SledgeItem;
    m_mapPresentItemInfo = kRecv_.m_mapPresentItemInfo; // 
    m_mapDonationInfo = kRecv_.m_mapDonationInfo;
    m_mapGradeRewardList = kRecv_.m_mapGradeRewardList;
    m_mapGradeDesc = kRecv_.m_mapGradeDesc;
}


void KGCChristmasGiftDlg::OnEventChristmasEventInfoAck( KEVENT_CHARISMAS_EVENT_INFO_ACK& kRecv_ )
{
    m_dwRecentlyRecievedRewardGrade = kRecv_.m_dwGrade;

    if( kRecv_.m_dwBonusPoint > m_mapGradeDesc[MAX_CHRISTMAS_REWARD].second ){
        m_dwBonusPoint = m_mapGradeDesc[MAX_CHRISTMAS_REWARD].second;
    } else {
        m_dwBonusPoint = kRecv_.m_dwBonusPoint;
    }

    // 이벤트 정보 세팅
    InitEventInfo();
}



void KGCChristmasGiftDlg::SendChristmasEventDonateReq( DWORD dwCount )
{
    KEVENT_CHARISMAS_EVENT_DONATION_REQ kPacket_;

    KItem* pkInvenItem = NULL;
    g_pItemMgr->FindInventoryForItemID( m_SledgeItemID / 10, &pkInvenItem);

    kPacket_.m_dwUseItemCnt = dwCount;                          // 사용 Item Cnt.
    kPacket_.m_dwAddBonusPoint = m_mapDonationInfo[dwCount];    // 서버에서 사용.
    kPacket_.m_kSledgeItem = *pkInvenItem;                      // 서버에서 사용.
    kPacket_.m_dwGrade = m_dwRecentlyRecievedRewardGrade;
    kPacket_.m_dwBonusPoint = m_dwBonusPoint;

    KP2P::GetInstance()->Send_CharismasEventDonationReq( kPacket_ );
}


void KGCChristmasGiftDlg::OnEventChristmasEventDonationAck( KEVENT_CHARISMAS_EVENT_DONATION_ACK& kRecv_ )
{
    int iSubtractedSledgeItem = static_cast<int>( m_dwSledgeCount ) - static_cast<int>( kRecv_.m_kSledgeItem.m_nCount );
    int iAddedBonusPoint = static_cast<int>( kRecv_.m_dwBonusPoint ) - static_cast<int>( m_dwBonusPoint );

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,g_pkStrLoader->GetReplacedString(STR_ID_DONATION_RESULT, "ii", iSubtractedSledgeItem, iAddedBonusPoint) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false);

    m_dwRecentlyRecievedRewardGrade = kRecv_.m_dwGrade;
    m_dwBonusPoint = kRecv_.m_dwBonusPoint;

    KItem* pkInvenItem = NULL;

    g_pItemMgr->FindInventory( kRecv_.m_kSledgeItem.m_ItemUID, &pkInvenItem );

    if( pkInvenItem ){
        pkInvenItem->m_nCount = kRecv_.m_kSledgeItem.m_nCount;

        if( pkInvenItem->m_ItemID * 10 == m_SledgeItemID )
            m_dwSledgeCount =  kRecv_.m_kSledgeItem.m_nCount;
    }

    if( pkInvenItem->m_nCount == 0 )
        g_pItemMgr->m_kInventory.RemoveItem( pkInvenItem->m_ItemUID );

    if( g_MyD3D->m_pStateMachine->IsStateMyInfo() )
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

    InitEventInfo();

}


void KGCChristmasGiftDlg::ActivateSendGiftItemDlg( GCITEMID dwItemID )
{
    std::map<KDropItemInfo,std::pair<DWORD,DWORD>>::iterator itr;
    for ( itr = m_mapPresentItemInfo.begin(); itr != m_mapPresentItemInfo.end(); ++itr )
    {
        if( itr->first.m_ItemID == dwItemID )
            break;
    }

    m_pkChristmasSendGiftDlg->SetSendGiftItemInfo( dwItemID, itr->first );
    m_pkChristmasSendGiftDlg->ToggleRender( true );
}


void KGCChristmasGiftDlg::OnEventChristmasEventPresentAck( KEVENT_CHARISMAS_EVENT_PRESENT_ACK& kRecv_ )
{
    int iAddedBonusPoint = static_cast<int>(kRecv_.m_dwBonusPoint) - static_cast<int>(m_dwBonusPoint);

    m_dwRecentlyRecievedRewardGrade = kRecv_.m_dwGrade;
    m_dwBonusPoint = kRecv_.m_dwBonusPoint;
    //kRecv_.m_dwReceiveUserUID;
    //kRecv_.m_kSledgeItem;

    KItem* pkInvenItem = NULL;
    std::vector<KItem>::iterator itr;
    for( itr = kRecv_.m_vecUseItem.begin(); itr != kRecv_.m_vecUseItem.end(); ++itr ){
        pkInvenItem = NULL;
        g_pItemMgr->FindInventory( itr->m_ItemUID, &pkInvenItem );
        if( pkInvenItem ) {
            pkInvenItem->m_nCount = itr->m_nCount;
            m_dwSledgeCount = itr->m_nCount;
        }

        if( pkInvenItem->m_nCount <= 0 ) { 
            g_pItemMgr->m_kInventory.RemoveItem(pkInvenItem->m_ItemUID );
            g_pItemMgr->RemoveInventoryItemList( pkInvenItem->m_ItemUID );
        }
    }

    if( pkInvenItem->m_nCount <= 0 )
        g_pItemMgr->m_kInventory.RemoveItem( pkInvenItem->m_ItemUID );
        g_pItemMgr->RemoveInventoryItemList( pkInvenItem->m_ItemUID );

    if( g_MyD3D->m_pStateMachine->IsStateMyInfo() )
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,g_pkStrLoader->GetReplacedString(STR_ID_SUCESS_TO_SEND_THE_GIFT, "si", m_pkChristmasSendGiftDlg->GetUserNameToRecieve().c_str(), iAddedBonusPoint ) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false);

    InitEventInfo();
}


void KGCChristmasGiftDlg::OnEventChristmasEventPresentNot( KEVENT_CHARISMAS_EVENT_PRESENT_NOT& kRecv_, bool bForce )
{
    for( UINT i = 0; i < (int)kRecv_.m_vecPresentItem.size(); ++i ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
            g_pkStrLoader->GetReplacedString( STR_ID_GIFT_MESSAGE, "s", kRecv_.m_strNickName.c_str() ),
            g_pkStrLoader->GetString( STR_ID_THE_GIFT_HAS_ARRIVED ),
            KGCUIScene::GC_MBOX_USE_NORMAL, kRecv_.m_vecPresentItem[i].m_ItemID * 10, kRecv_.m_vecPresentItem[i].m_ItemUID, false, bForce );
    }
}

void KGCChristmasGiftDlg::OnEventChristmasEventGradeRewadAck( KEVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK& kRecv_ )
{
    if( kRecv_.bInitEnable ){
        m_dwBonusPoint = 0;
    } else {
        m_dwBonusPoint += kRecv_.m_dwBonusPoint;
    }

    m_dwRecentlyRecievedRewardGrade = kRecv_.m_dwGrade;

    for( UINT i = 0; i < (int)kRecv_.m_vecReward.size(); ++i ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
            L"", g_pkStrLoader->GetString(STR_ID_THE_GIFT_HAS_ARRIVED),
            KGCUIScene::GC_MBOX_USE_NORMAL, kRecv_.m_vecReward[i].m_ItemID * 10, kRecv_.m_vecReward[i].m_ItemUID, false, true );
    }

    InitEventInfo();
}


bool KGCChristmasGiftDlg::IsBonusPointMax()
{
    if( m_dwBonusPoint == m_mapGradeDesc[MAX_CHRISTMAS_REWARD].second )
        return true;
    else
        return false;
}


void KGCChristmasGiftDlg::ShowGiftMsg()
{
    if ( m_vecKeepMsg.empty() ) {
        return;
    } else {
        for( UINT i = 0; i < (int)m_vecKeepMsg.size(); ++i ) {
            OnEventChristmasEventPresentNot( m_vecKeepMsg[i], false );
        }

        m_vecKeepMsg.clear();
    }
}


bool KGCChristmasGiftDlg::IsSubDlgRenderOn()
{ 
    if ( m_pkChristmasDonateDlg->IsRenderOn() || m_pkChristmasSendGiftDlg->IsRenderOn() ) {
        return true;
    } else {
        return false;
    }
}