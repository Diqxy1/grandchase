#include "stdafx.h"
#include "../MyD3D.h"
#include "gcui/KGCGiftBox.h"
#include "KGCGiftBoxState.h"

IMPLEMENT_CLASSNAME( KGCGiftBoxState );
IMPLEMENT_WND_FACTORY( KGCGiftBoxState );
IMPLEMENT_WND_FACTORY_NAME( KGCGiftBoxState, "gc_gift_box_wnd" );

KGCGiftBoxState::KGCGiftBoxState( void )
: m_nNewUserRemainTime(-1)
, m_dwAccTimeNot(0)
, m_bNewUserTimeOut(false)
, m_nPrevTime(-1)
, m_iEventtype(-1)
, m_bNextEventReward(false)
, m_iParticleTime(50)
, m_bBlinkOn(false)
{
    char str[MAX_PATH] = {0,};

    for ( int i=0; i<EVENT_TYPE_SIZE; ++i ) {
        m_apkWnd[i] = NULL;
        m_apkStatic[i] = NULL;
    }

    for ( int i=0; i<STATE_SIZE; ++i ) {
        m_apkBoxShape[i] = NULL;
    }

    LINK_CONTROL("time_static", m_apkStatic[EVENT_REWARD] );
    LINK_CONTROL("new_user_static", m_apkStatic[NEW_USER] );
    LINK_CONTROL("box_background", m_apkWnd[EVENT_REWARD]);
    LINK_CONTROL("new_user_reward_ready", m_apkWnd[NEW_USER]);
    LINK_CONTROL("box_normal" , m_apkBoxShape[STATE_NORMAL]);
    LINK_CONTROL("box_open_ready" , m_apkBoxShape[STATE_REWARD_READY]);

}

KGCGiftBoxState::~KGCGiftBoxState( void )
{
}

void KGCGiftBoxState::OnCreate( void )
{
    m_apkStatic[EVENT_REWARD]->SetText( L"00:00" );
    m_apkStatic[EVENT_REWARD]->SetFontSize(7);
    m_apkStatic[NEW_USER]->SetText( L"00 : 00" );
    
    m_apkWnd[NEW_USER]->SetSelfInputCheck( true );
    m_apkWnd[NEW_USER]->AddActionListener( this );
    m_apkWnd[NEW_USER]->ToggleRender(false);
    m_apkWnd[NEW_USER]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCGiftBoxState::ClickNewUserReward );

    m_apkBoxShape[STATE_REWARD_READY]->SetSelfInputCheck( true );
    m_apkBoxShape[STATE_REWARD_READY]->AddActionListener( this );
    m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(false);
    m_apkBoxShape[STATE_REWARD_READY]->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCGiftBoxState::ClickEventReward );

    m_apkWnd[EVENT_REWARD]->InitState(false, true, this);

    m_apkBoxShape[STATE_NORMAL]->InitState(false, true, this);
    m_apkBoxShape[STATE_REWARD_READY]->InitState(false, true, this);
}

void KGCGiftBoxState::OnInitialize( void )
{
    m_bBlinkOn = false;
}

void KGCGiftBoxState::SetGiftBoxInfo( int iEventType , bool IsRewardReady )
{
    m_iEventtype = iEventType;
    switch(m_iEventtype)
    {
    case NEW_USER:
        {
            m_apkStatic[NEW_USER]->ToggleRender(true);
            m_apkWnd[NEW_USER]->ToggleRender(false);

            m_apkStatic[EVENT_REWARD]->ToggleRender(false);
            m_apkWnd[EVENT_REWARD]->ToggleRender(false);
        }
        break;
    case EVENT_REWARD:
        {
            m_apkStatic[EVENT_REWARD]->ToggleRender(true);
            m_apkWnd[EVENT_REWARD]->ToggleRender(true);

            m_apkStatic[NEW_USER]->ToggleRender(false);
            m_apkWnd[NEW_USER]->ToggleRender(false);
        }
        break;
    }

    if (IsRewardReady) {
        m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(true);
        m_apkBoxShape[STATE_NORMAL]->ToggleRender(false);
    }
    else {
        m_apkBoxShape[STATE_NORMAL]->ToggleRender(true);
        m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(false);
    }
}

void KGCGiftBoxState::ActionPerformed( const KActionEvent& event )
{

}

void KGCGiftBoxState::FrameMoveInEnabledState( void )
{
    UpdateTime(m_iEventtype);
    CheckParticleTimeForBtn();

    if ( true == m_bBlinkOn && true == m_apkWnd[NEW_USER]->IsRenderOn() )
    {
        ShowConfirmBtnParticle();
    }
}

void KGCGiftBoxState::UpdateTime(int iEventType)
{
    if ( m_bNextEventReward == true )
    {
        static std::wstring strTime;

        strTime = boost::str( boost::wformat(L"%02d :00")
            % ( SiKGCGiftBoxManager()->CheckTime(GIFT_BOX_EVENT_USER, true) )
            );

        m_apkStatic[EVENT_REWARD]->SetText(strTime);
        return;
    }

    int nPrevTime = -1;
    if ( iEventType == GIFT_BOX_NEW_USER ) 
        nPrevTime = SiKGCGiftBoxManager()->GetNewUserRemainTime();
    else if ( iEventType == GIFT_BOX_EVENT_USER )
        nPrevTime = SiKGCGiftBoxManager()->GetEventRemainTime();
    else 
        return;

    if ( m_nPrevTime != nPrevTime ) 
    {
        m_nPrevTime = nPrevTime;

        std::wostringstream strm;

        WCHAR strMTemp[20];
        WCHAR strSTemp[20];
        wcsnset(strMTemp,'\0',20);
        wcsnset(strSTemp,'\0',20);

        nPrevTime = nPrevTime % ( 60 * 60 );
        swprintf( strMTemp, 20, L"%02d", (int)( nPrevTime / 60 ) );

        nPrevTime = nPrevTime % 60;
        swprintf( strSTemp, 20, L"%02d", nPrevTime );
        strm<<strMTemp<<L":"<<strSTemp;

        if ( iEventType == GIFT_BOX_NEW_USER ) 
            m_apkStatic[NEW_USER]->SetText(strm.str().c_str());
        else if ( iEventType == GIFT_BOX_EVENT_USER )
            m_apkStatic[EVENT_REWARD]->SetText(strm.str().c_str());
    }

    if (m_nPrevTime <= 0 && iEventType == GIFT_BOX_NEW_USER)
    {
        if ( !SiKGCGiftBoxManager()->GetPossibleReward(GIFT_BOX_NEW_USER) ) {
            m_apkStatic[NEW_USER]->SetText(g_pkStrLoader->GetString(STR_GIFTBOX_REWARD_WAIT).c_str());
            m_apkStatic[NEW_USER]->ToggleRender(true);
            m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(false );
            m_apkBoxShape[STATE_NORMAL]->ToggleRender(true);
        }
        else {
            m_apkStatic[NEW_USER]->ToggleRender(false);
            m_apkWnd[NEW_USER]->ToggleRender(true);

            m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(true );
            m_apkBoxShape[STATE_NORMAL]->ToggleRender(false);
        }
    }
    else if (m_nPrevTime <= 0 && iEventType == GIFT_BOX_EVENT_USER)
    {
        if ( !SiKGCGiftBoxManager()->GetPossibleReward(GIFT_BOX_EVENT_USER) ) {
            m_apkStatic[EVENT_REWARD]->SetText(g_pkStrLoader->GetString(STR_GIFTBOX_REWARD_WAIT).c_str());
            m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(false );
            m_apkBoxShape[STATE_NORMAL]->ToggleRender(true);
            m_apkWnd[EVENT_REWARD]->ToggleRender(true);
            m_apkWnd[NEW_USER]->ToggleRender(false);
        }
        else {
            m_apkStatic[EVENT_REWARD]->SetText(g_pkStrLoader->GetString(STR_GIFTBOX_REWARD_WAIT).c_str());
            m_apkStatic[EVENT_REWARD]->ToggleRender(false);
            m_apkWnd[EVENT_REWARD]->ToggleRender(false);
            m_apkWnd[NEW_USER]->ToggleRender(false);

            m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(true);
            m_apkBoxShape[STATE_NORMAL]->ToggleRender(false);
        }
    }
    else
    {
        m_apkBoxShape[STATE_REWARD_READY]->ToggleRender(false);
        m_apkBoxShape[STATE_NORMAL]->ToggleRender(true);
    }

}

void KGCGiftBoxState::ClickNewUserReward(void)
{
    ShowRewardParticle();
    KP2P::GetInstance()->Send_Giftbox_NewUser_Reward_Req();
}

void KGCGiftBoxState::ClickEventReward(void)
{
    if ( m_iEventtype == GIFT_BOX_EVENT_USER) {
        ShowRewardParticle();
        KP2P::GetInstance()->Send_Giftbox_EventUser_Reward_Req();
    }
}

void KGCGiftBoxState::SetNextEventReward(bool bNextEvent)
{
    m_bNextEventReward = bNextEvent;
}

void KGCGiftBoxState::SetNewUserToolTip( GCITEMID gcItemID )
{
    if ( gcItemID < 0  || g_pItemMgr->GetItemData(gcItemID) == NULL) {
        m_apkBoxShape[STATE_NORMAL]->SetToolTip( L"" );
        m_apkBoxShape[STATE_REWARD_READY]->SetToolTip( L"" );
        return;
    }

    GCItem* pItem = g_pItemMgr->GetItemData( gcItemID / 10 );
    m_apkBoxShape[STATE_NORMAL]->SetToolTip( pItem->strItemName );
    m_apkBoxShape[STATE_REWARD_READY]->SetToolTip(pItem->strItemName);
}

void KGCGiftBoxState::ShowConfirmBtnParticle()
{
    //확인 창 깜빡임
    if ( true == m_apkWnd[NEW_USER]->IsRenderOn() ) {
        g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Presentbox_Clear_01", 0.258f, 0.621f, 0.5f), GC_LAYER_UI);
    }

    m_iParticleTime = 50;
    m_bBlinkOn = false;
}

void KGCGiftBoxState::CheckParticleTimeForBtn()
{
    if( m_iParticleTime == 0)
        return;

    m_iParticleTime--;

    if( m_iParticleTime == 0 )
    {
        m_bBlinkOn = true;
    }
}

void KGCGiftBoxState::ShowRewardParticle()
{
    D3DXVECTOR2 vPos = GetFixedWindowLocalPos();
    vPos.x += m_apkBoxShape[STATE_REWARD_READY]->GetFixedWindowLocalPos().x;
    vPos.y += m_apkBoxShape[STATE_REWARD_READY]->GetFixedWindowLocalPos().y;
    vPos.x += static_cast<float>(m_apkBoxShape[STATE_REWARD_READY]->GetWidth()/2);
    vPos.y += static_cast<float>(m_apkBoxShape[STATE_REWARD_READY]->GetHeight()/2);

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Presentbox_Open_01",PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), 0.5f) , GC_LAYER_UI);
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Presentbox_Open_02",PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), 0.5f), GC_LAYER_UI );
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Presentbox_Open_03",PARTICLEPOS_X(vPos.x) , PARTICLEPOS_Y(vPos.y), 0.5f) , GC_LAYER_UI);
}