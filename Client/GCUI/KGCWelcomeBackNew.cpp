#include "stdafx.h"
#include "KGCWelcomeBackNew.h"
#include "KGCWelcomBack_Recom.h"
#include "KGCWelcomBack_Reward.h"
#include "KGCWelcomBack_Itemlist.h"

IMPLEMENT_CLASSNAME( KGCWelcomeBackNew );
IMPLEMENT_WND_FACTORY( KGCWelcomeBackNew );
IMPLEMENT_WND_FACTORY_NAME( KGCWelcomeBackNew, "gc_welcome_back_new_dlg" );

KGCWelcomeBackNew::KGCWelcomeBackNew(void)
: m_pkStaticDesc( NULL )
, m_pkCloseBtn( NULL )
, m_pkStaticMyState( NULL )
, m_pkMyStateBackground( NULL )
{
    for ( int i = 0; i < RECOMMENDER_MAX_NUM; ++i )
    {
        m_pkRecommender[i] = NULL;

        if ( i < REWARD_MAX_NUM )
        {
            m_pkReward[i] = NULL;
            m_pkItemList[i] = NULL;
            m_pkDescReward[i] = NULL;
        }
    }
    m_nState = -1;

    LINK_CONTROL( "recommend_string0",  m_pkStaticDesc );
    LINK_CONTROL( "my_state_string",    m_pkStaticMyState );
    LINK_CONTROL( "close_btn",          m_pkCloseBtn );
    LINK_CONTROL( "background_my_state",m_pkMyStateBackground );

    char str[MAX_PATH];
    for ( int i = 0; i < RECOMMENDER_MAX_NUM ; ++i )
    {
        sprintf( str, "recommender%d", i );
        LINK_CONTROL( str, m_pkRecommender[i] );

        if ( i < REWARD_MAX_NUM )
        {
            sprintf( str, "item_info_box%d", i );
            LINK_CONTROL( str, m_pkItemList[i] );

            sprintf( str, "reward_for_rec%d", i );
            LINK_CONTROL( str, m_pkReward[i] );

            sprintf( str, "desc_for_rec%d", i );
            LINK_CONTROL( str, m_pkDescReward[i] );
        }
    }
}

KGCWelcomeBackNew::~KGCWelcomeBackNew(void)
{
}

void KGCWelcomeBackNew::OnCreate( void )
{
    m_pkStaticDesc->InitState( true );
    
    if (m_pkStaticMyState)
        m_pkStaticMyState->InitState( true );
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

    for ( int i = 0; i < RECOMMENDER_MAX_NUM ; ++i )
    {
        if( m_pkRecommender[i] != NULL ){
            m_pkRecommender[i]->InitState( true );
            m_pkRecommender[i]->SetMode( KD3DWnd::D3DWM_LOCK );
        }

        if ( i < REWARD_MAX_NUM)
        {
            m_pkItemList[i]->InitState( false, true, this );
            m_pkReward[i]->InitState( false, true, this );
            m_pkDescReward[i]->InitState( false, true, this );
        }
    }
    if (m_pkStaticMyState)
        m_pkStaticMyState->SetLineSpace( 1.0f );
    m_pkStaticDesc->SetLineSpace( 1.0f );
}

void KGCWelcomeBackNew::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == D3DWE_BUTTON_CLICK && event.m_pWnd == m_pkCloseBtn )
    {
        g_KDSound.Play( "31" );
        SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    }

    for ( int i = 0; i < REWARD_MAX_NUM; ++i )
    {    
        if( event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            if ( event.m_pWnd == m_pkReward[i])
            {
                KP2P::GetInstance()->Send_AttendBonusReq( i + 1 );
            }
            else if( event.m_pWnd == m_pkDescReward[i] )
                m_pkItemList[i]->ToggleRender( !m_pkItemList[i]->IsRenderOn() );
            else
                m_pkItemList[i]->ToggleRender( false );
        }
    }
}

void KGCWelcomeBackNew::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCWelcomeBackNew::FrameMoveInEnabledState( void )
{
    static int m_iAlphaCnt = 0;
    static int m_iSwitch = 8;

    m_iAlphaCnt += m_iSwitch;
    if( 255 <= m_iAlphaCnt || m_iAlphaCnt <= 0 )
    {
        m_iAlphaCnt = (255<=m_iAlphaCnt) ? 255:0;
        m_iSwitch *= -1;
    }

    if( m_pkStaticMyState->IsRenderOn() ) {
        m_pkStaticMyState->SetAllWindowAlpha( m_iAlphaCnt );
        m_pkMyStateBackground->SetAllWindowAlpha( m_iAlphaCnt );
    }
}

void KGCWelcomeBackNew::SetItemInfo( KEVENT_CHECK_RECOMMEND_ENABLE_ACK kRecv )
{
    if( m_mapRecomBonus.size() != 0 )
        return;

    m_mapRecomBonus = kRecv.m_mapRecomBonus;
    // 툴팁에 넣을 아이템 리스트
    for(int i=0; i< REWARD_MAX_NUM; ++i)
    {
        std::vector<KDropItemInfo>& vecRecomBunus = m_mapRecomBonus[ (i+1) ];
        std::vector<KDropItemInfo>::iterator it;

        m_pkItemList[i]->InitItemList();

        for( it = vecRecomBunus.begin() ; it != vecRecomBunus.end(); it++)
        {
            m_pkItemList[i]->SetItemList( it->m_ItemID, it->m_nDuration );
        }
    }
}

void KGCWelcomeBackNew::UpdateState( void )
{
    // 애들 출석 체크, 버튼 활성/비활성화
    // 연계된 애들 파랗게
    int iMyState = SiKFriendShip()->GetMyRecommendState();
    int iAgreed = SiKFriendShip()->m_vecContactFriendList.size();
    if( iMyState == 1 )
        iAgreed--;

    for ( int i = 0; i < RECOMMENDER_MAX_NUM ; ++i )
        m_pkRecommender[i]->SetMode( KD3DWnd::D3DWM_LOCK );


    for( int j=0; j < iAgreed; j++ )
    {
        m_pkRecommender[j]->SetMode( KD3DWnd::D3DWM_DEFAULT );
    }

    // 출석한 애들 체크
    std::vector< KRecommenderInfo >::iterator it = SiKFriendShip()->m_vecRecommenders.begin();
    int temp = 0;
    for( int i=0  ; it != SiKFriendShip()->m_vecRecommenders.end(); it++)
    {
        if( it->m_nState == KFriendShip::ORS_ATTENDANCE_FINISH || it->m_nState == KFriendShip::ORS_ITEM_RECEIVED )
            i++;

        temp = i;
    }
    for( int j = 0; j < temp ; j++)
    {
        m_pkRecommender[j]->SetSupply();
    }

    UpdateRewardState();
}

void KGCWelcomeBackNew::OnCreateComplete( void )
{
    UpdateState();
}

void KGCWelcomeBackNew::SetRewardState(int nState)
{
    m_nState = nState;
    UpdateState();
}

void KGCWelcomeBackNew::UpdateRewardState()
{
    int iAttended = SiKFriendShip()->GetAttendedRecommender();
    int iMyState = SiKFriendShip()->GetMyRecommendState();
    int iAgreed = SiKFriendShip()->m_vecContactFriendList.size();
    if( iMyState == 1 )
        iAgreed--;

    if( SiKFriendShip()->m_vecContactFriendList.size() == 0 )
        iAgreed = 0;

    std::wstring wstrMessage;
    wstrMessage = g_pkStrLoader->GetReplacedString(STR_ID_USERS_RECOMMEND_YOU, "i", iAgreed);
    wstrMessage += g_pkStrLoader->GetReplacedString(STR_ID_USERS_ATTEND, "i", iAttended);
    m_pkStaticDesc->SetTextAutoMultiline(wstrMessage);

    int i;
    for( i = 0 ; i< REWARD_MAX_NUM ; ++i )
    {
        int iNum = (i+1);
        if( iNum <= m_nState) // 이미 받았다
        {
            m_pkReward[i]->ToggleRender( true );
            m_pkReward[i]->SetLock();
            m_pkReward[i]->Lock(true);
            m_pkDescReward[i]->ToggleRender( false );
        }
        else if( iNum > m_nState && iNum <= iAttended) // 받을 수 있는데 아직 안받았다. 
        {
            m_pkReward[i]->ToggleRender( true );
            m_pkDescReward[i]->ToggleRender( false );
        }
        else
        {
            m_pkReward[i]->ToggleRender( false );
            m_pkDescReward[i]->ToggleRender( true);
        }
    }
}


void KGCWelcomeBackNew::UpdateMyState( int _state)
{
    switch ( _state )
    {
    case 1: //신규유저 
    case 2: //복귀유저       
        m_pkStaticMyState->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_WELCOME_FIRST_VISIT) );
        break;
    case 4: //활성유저
        m_pkStaticMyState->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_WELCOME_BACK_NEW) );
        break;       
    }
}
