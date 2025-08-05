#include "stdafx.h"
#include "GCAttendanceStampSlot.h"


IMPLEMENT_CLASSNAME( KGCAttendanceStampSlot );
IMPLEMENT_WND_FACTORY( KGCAttendanceStampSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAttendanceStampSlot, "gc_stamp" );

KGCAttendanceStampSlot::KGCAttendanceStampSlot( void )
{
    m_pkSlotActive = NULL;
    m_pkSlotPassive = NULL;
    m_pkStamped = NULL;
    m_pkStaticDays = NULL;
    m_pkRewardItemImgWnd = NULL;

    LINK_CONTROL( "slot_active", m_pkSlotActive );   /// 투명
    LINK_CONTROL( "slot_passive", m_pkSlotPassive ); // 슬롯 이미지
    LINK_CONTROL( "stamped", m_pkStamped );
    LINK_CONTROL( "days", m_pkStaticDays );
    LINK_CONTROL( "reward_item_img", m_pkRewardItemImgWnd );
    LINK_CONTROL( "timed", m_pkTimed );
    LINK_CONTROL( "duration", m_pkStaticDuration );

    m_nIndex = 0;
    m_vecReward.clear();
    m_eState = ASS_LOCK;
}

KGCAttendanceStampSlot::~KGCAttendanceStampSlot( void )
{
}


void KGCAttendanceStampSlot::OnCreate( void )
{
    m_pkSlotActive->InitState( true, true, this );
    m_pkSlotPassive->InitState( true, false );
    m_pkStamped->InitState( true, false );
    m_pkStaticDays->InitState( true );
    m_pkStaticDays->SetAlign( DT_CENTER );
    m_pkRewardItemImgWnd->InitState( true );
    m_pkTimed->InitState( false, false );
    m_pkStaticDuration->InitState( true );
    m_pkStaticDuration->SetAlign( DT_LEFT );
}

void KGCAttendanceStampSlot::FrameMoveInEnabledState( void )
{
    // 
    UpdateWndState();

    // 아이템 정보 표시
    //DrawInfoWindow();
}


void KGCAttendanceStampSlot::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkSlotActive,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClaimReward );
}

void KGCAttendanceStampSlot::SetSlotInfo( int nIndex )
{
    m_nIndex = nIndex;
    m_eState = SiGCAttendanceStampManager()->GetSlotState( nIndex );

    // 보상 아이템 이미지
    if( SiGCAttendanceStampManager()->GetSlotReward( nIndex, m_vecReward ) ){
        VEC_REWARD::iterator it = m_vecReward.begin();
        GCItem* pItem = g_pItemMgr->GetItemData( (*it).m_ItemID / 10 );

        if( pItem == NULL ){
            m_pkRewardItemImgWnd->SetItemInfo( 0 );
            return;
        }

        if( pItem->iSetItemNumber != -1 && pItem->dwSlotPosition & ESP_SET_ITEM )
            m_pkRewardItemImgWnd->SetItemInfo( (*it).m_ItemID / 10, -1, true, pItem->iSetItemNumber );
        else
            m_pkRewardItemImgWnd->SetItemInfo( (*it).m_ItemID / 10 );

        m_pkRewardItemImgWnd->ShowItem ( true );

        if ( (*it).m_nPeriod != KItem::UNLIMITED_ITEM )
        {
            std::wstringstream tempPeriod;
            tempPeriod << (*it).m_nPeriod << L"D";
            m_pkTimed->ToggleRender( true );
            m_pkTimed->SetToolTip( tempPeriod.str() );
        }
        else
        {
            m_pkTimed->ToggleRender( false );
            m_pkTimed->SetToolTip( L"" );
        }

        if ( (*it).m_nDuration != KItem::UNLIMITED_ITEM )
        {
            std::wstringstream tempDuration;
            tempDuration << (*it).m_nDuration;
            m_pkStaticDuration->SetText( tempDuration.str() );
        }
        else
        {
            m_pkStaticDuration->SetText(L"");
        }
    }

    // static
    m_pkStaticDays->SetText(g_pkStrLoader->GetReplacedString( STR_ID_ATTENDANCE_STAMP_STR_1, "i", nIndex ));

    // 슬롯 State에 따라 세팅
    InitWndState();
}

void KGCAttendanceStampSlot::InitWndState()
{
    // 슬롯 상태에 따른 처리
    switch( m_eState ) {
        case ASS_OPEN :
            m_pkSlotActive->SetWndMode( D3DWM_LOCK );
            m_pkSlotPassive->SetWndMode( D3DWM_DEFAULT );
            m_pkStamped->SetWndMode( D3DWM_LOCK );
            // 흰색
            m_pkStaticDays->SetFontColor( D3DCOLOR_ARGB(255, 153, 153, 153) );
            m_pkRewardItemImgWnd->ToggleRender( true );
            m_pkRewardItemImgWnd->SetColor( D3DCOLOR_ARGB( 255, 153, 153, 153 ) );
            break;
        case ASS_STAMPED :
            m_pkSlotActive->SetWndMode( D3DWM_DEFAULT );
            m_pkSlotPassive->SetWndMode( D3DWM_DEFAULT );
            m_pkStamped->SetWndMode( D3DWM_DEFAULT );
            // 흰색
            m_pkStaticDays->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
            m_pkRewardItemImgWnd->ToggleRender( true );
            m_pkRewardItemImgWnd->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
            break;
        case ASS_CLAIMABLE :
            m_pkSlotActive->SetWndMode( D3DWM_DEFAULT );
            m_pkSlotPassive->SetWndMode( D3DWM_DEFAULT );
            m_pkStamped->SetWndMode( D3DWM_LOCK );
            // 오랜지색
            m_pkStaticDays->SetFontColor( D3DCOLOR_ARGB(255, 255, 204, 51) );
            m_pkRewardItemImgWnd->ToggleRender( true );
            m_pkRewardItemImgWnd->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
            break;
        case ASS_LOCK :
        default :
            m_pkSlotActive->SetWndMode( D3DWM_LOCK );
            m_pkSlotPassive->SetWndMode( D3DWM_LOCK );
            m_pkStamped->SetWndMode( D3DWM_LOCK );
            // 회색
            m_pkStaticDays->SetFontColor( D3DCOLOR_ARGB(255, 153, 153, 153) );
            m_pkRewardItemImgWnd->ToggleRender( false );
            break;
    }
}

void KGCAttendanceStampSlot::UpdateWndState()
{
    if( m_eState == ASS_CLAIMABLE ) // 상태값이 이상해서 다시 검사
        m_eState = SiGCAttendanceStampManager()->GetSlotState( m_nIndex );

    if( m_eState == ASS_LOCK )
        return;

    if( m_eState == ASS_CLAIMABLE )
        m_pkSlotPassive->SetWndMode( m_pkSlotActive->GetWndMode() );
    else
        m_pkSlotPassive->SetWndMode( D3DWM_DEFAULT );

}

void KGCAttendanceStampSlot::OnClickClaimReward()
{
    g_KDSound.Play( "31" );

    if( m_eState == ASS_CLAIMABLE ){
        Result_ReceiveAttendanceStampReward = INT_MAX;
        KP2P::GetInstance()->Send_AccumulateAttendanceRewardReq( m_nIndex );
        g_MyD3D->WaitForServerAck( Result_ReceiveAttendanceStampReward, INT_MAX, 3000, TIME_OUT_VALUE );
    }
}

void KGCAttendanceStampSlot::DrawInfoWindow()
{
    if( m_vecReward.empty() ) { 
        return;
    }

    D3DXVECTOR2 vMousePos = D3DXVECTOR2( static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y) );

    if( m_pkRewardItemImgWnd->CheckPosInWindowBound( vMousePos ) ) {
        VEC_REWARD::iterator it = m_vecReward.begin();
        GCItem* kItem = g_pItemMgr->GetItemData( (*it).m_ItemID / 10 );

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );

        D3DXVECTOR2 vecPos = this->GetCurrentWindowPos();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );

    }

}