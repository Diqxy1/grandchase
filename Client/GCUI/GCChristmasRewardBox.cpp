#include "stdafx.h"
#include "GCChristmasRewardBox.h"

IMPLEMENT_CLASSNAME( KGCChristmasRewardBox );
IMPLEMENT_WND_FACTORY( KGCChristmasRewardBox );
IMPLEMENT_WND_FACTORY_NAME( KGCChristmasRewardBox, "gc_christmas_reward" );

KGCChristmasRewardBox::KGCChristmasRewardBox( void )
{
    m_pkBtnGetReward = NULL;
    m_pkBtnRewardText = NULL;
    m_pkBtnInvisible = NULL;
    m_pkRewardItemImgOver = NULL;
    m_pkFrame = NULL;
    LINK_CONTROL( "back_button", m_pkBtnGetReward );
    LINK_CONTROL( "btn_reward_text", m_pkBtnRewardText );
    LINK_CONTROL( "btn_invisible", m_pkBtnInvisible );
    LINK_CONTROL( "item_over_img", m_pkRewardItemImgOver );
    LINK_CONTROL( "frame", m_pkFrame );

    m_pkRewardItemImgWnd = NULL;
    LINK_CONTROL( "item_img_wnd", m_pkRewardItemImgWnd );



    //
    m_nState = CRRS_NOT_ENOUGH_POINT;
    m_vecRewardList.clear();

    m_dwGrade = 0;
}

KGCChristmasRewardBox::~KGCChristmasRewardBox( void )
{
}


void KGCChristmasRewardBox::OnCreate( void )
{
    m_pkBtnGetReward->InitState( true, true );       // 보여지는 용도
    m_pkBtnRewardText->InitState( true, true );      // 보여지는 용도
    m_pkBtnInvisible->InitState( true, true, this ); // 실제 버튼 동작
    m_pkRewardItemImgOver->InitState( true );        // 비활성화시 어둡게
    m_pkFrame->InitState( true );

    m_pkRewardItemImgWnd->InitState( true );

    InitRewardBox();
}


void KGCChristmasRewardBox::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnInvisible,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickGetReward );
}

void KGCChristmasRewardBox::FrameMoveInEnabledState( void )
{
    // 
    UpdateRewardBox();

    //
    DrawInfoWindow();
}


void KGCChristmasRewardBox::OnClickGetReward()
{
    g_KDSound.Play( "31" );

    if ( m_nState != CRRS_GET_REWARD )
        return;

    // 최초클릭과 동시에 보상 받음으로 상태 변환
    m_nState = CRRS_ALREADY_GOT_REWARD;

    if( m_dwGrade == g_pkUIScene->m_pkChristmasGiftDlg->GetRecentlyRecievedGrade() ){
        KEVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ kPacket_;

        kPacket_.m_dwGrade = m_dwGrade;
        kPacket_.m_vecReward = m_vecRewardList;
        kPacket_.m_dwBonusPoint = g_pkUIScene->m_pkChristmasGiftDlg->GetCurrentBonusPoint();

        KP2P::GetInstance()->Send_CharismasEventGradeRewardReq( kPacket_ );
        g_MyD3D->WaitForServerAck( Result_ReceiveChristmasReward, INT_MAX, 5000, TIME_OUT_VALUE );

    } else {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,g_pkStrLoader->GetString(STR_ID_GET_REWARD_LOWER_GRADE_FIRST) , L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false);
    }

}

void KGCChristmasRewardBox::SetRewardInfo( DWORD dwGrade, VEC_REWARD& vecReward )
{
    // 보상 리스트
    m_dwGrade = dwGrade;
    m_vecRewardList = vecReward;


    // 대표아이템 보여주기
    m_pkRewardItemImgWnd->SetItemInfo( m_vecRewardList.begin()->m_ItemID / 10 );
    m_pkRewardItemImgWnd->ShowItem (true );

}

void KGCChristmasRewardBox::InitRewardBox()
{
    // 프레임 활성화
    switch( m_nState )
    {
    case CRRS_NOT_ENOUGH_POINT:
        m_pkFrame->SetWndMode( D3DWM_LOCK );
        m_pkBtnGetReward->SetWndMode( D3DWM_LOCK );
        m_pkBtnRewardText->SetWndMode( D3DWM_LOCK );
        m_pkBtnInvisible->SetWndMode( D3DWM_LOCK );
        m_pkRewardItemImgOver->ToggleRender( true );
        break;
    case CRRS_ALREADY_GOT_REWARD:
        m_pkFrame->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnGetReward->SetWndMode( D3DWM_LOCK );
        m_pkBtnGetReward->SetWndUpdateLock( true );
        m_pkBtnRewardText->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnRewardText->SetWndUpdateLock( true );
        m_pkBtnInvisible->SetWndMode( D3DWM_LOCK );
        m_pkRewardItemImgOver->ToggleRender( false );
        break;
    case CRRS_GET_REWARD:
        m_pkFrame->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnGetReward->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnRewardText->SetWndMode( D3DWM_DEFAULT );
        m_pkBtnInvisible->SetWndMode( D3DWM_DEFAULT );
        m_pkRewardItemImgOver->ToggleRender( false );
        break;
    default:
        break;
    }
}

void KGCChristmasRewardBox::UpdateRewardBox()
{
    if( m_nState != CRRS_GET_REWARD )
        return;

    // 마우스 롤오버 클릭 동기화 (배경과 텍스트가 따로 노는 상태라서)
    m_pkBtnGetReward->SetWndMode( m_pkBtnInvisible->GetWndMode() );
    m_pkBtnRewardText->SetWndMode( m_pkBtnInvisible->GetWndMode() );
}


void KGCChristmasRewardBox::DrawInfoWindow()
{
    if( g_pkUIScene->m_pkChristmasGiftDlg->IsSubDlgRenderOn() )
        return;

    if( m_vecRewardList.empty() ) { 
        return;
    }

    D3DXVECTOR2 vMousePos = D3DXVECTOR2( static_cast<float>(g_pkInput->GetMousePos().x), static_cast<float>(g_pkInput->GetMousePos().y) );

    if( m_pkRewardItemImgWnd->CheckPosInWindowBound( vMousePos ) ) {
        GCITEMID dwItemID = m_vecRewardList[0].m_ItemID;
        GCItem* kItem = g_pItemMgr->GetItemData( dwItemID / 10 );

        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( kItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );

        D3DXVECTOR2 vecPos = this->GetCurrentWindowPos();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );

    }

}
