#include "stdafx.h"
#include "GCChallengeErrandPanel.h"

IMPLEMENT_CLASSNAME( KGCChallengeErrandPanel );
IMPLEMENT_WND_FACTORY( KGCChallengeErrandPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCChallengeErrandPanel, "gc_errand_panel" );

const int ERRAND_X_MOVE = 1;
const int ERRAND_Y_MOVE = 3;

const int NOT_MOVE_BOTTON_LEFT = 0;
const int NOT_MOVE_TOP_RIGHT = 2;

const DWORD dwLmitDelayTime = 2000;

KGCChallengeErrandPanel::KGCChallengeErrandPanel( void ):
m_iStage( 1 ),
m_pkErrandHome( NULL),
m_pkErrandPlayer( NULL ),
m_pkErrandBuyList( NULL ),
m_pkImgGameStart( NULL ),
m_pkImgGmaeEnd( NULL ),
m_pkImgGmaeSucces( NULL ),
m_fDetPos( 0.0f ),
m_dwAccTimeNot( 0 ),
m_dwStageDlayTime( 0 ),
m_nRandomMerry( 0 ),
m_nMarryAppearInterval( 0 ),
m_nMarryKeepAliveTime( 0 ),
m_dwAccMerryApperTime( 0 ),
m_bReciveReward( false ),
m_nPrevTime( -1 )
{

    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apErrandShop[i] = NULL;

        char strShop[128];
        sprintf_s( strShop, 127, "errand_shop%d", i );
        LINK_CONTROL( strShop, m_apErrandShop[i] );

        m_apStaticHaveNum[i] = NULL;
        char strBuyNum[128];
        sprintf_s( strBuyNum, 127, "static_have_num%d", i );
        LINK_CONTROL( strBuyNum, m_apStaticHaveNum[i] );
    }

    for( int i = 0; i < NUM_TIME_CIPHER; i++ )
    {
        m_apTime[i] = NULL;

        char strTime[128];
        sprintf_s( strTime, 127, "ui_time%d", i );
        LINK_CONTROL( strTime, m_apTime[i] );
    }

    for( int i = 0; i < NUM_MAX_COUNT; i++ )
    {
        m_apSuccesCount[i] = NULL;

        char strSucces[128];
        sprintf_s( strSucces, 127, "ui_succes_counter%d", i );
        LINK_CONTROL( strSucces, m_apSuccesCount[i] );
    }

    LINK_CONTROL( "errand_home", m_pkErrandHome );
    LINK_CONTROL( "errand_player", m_pkErrandPlayer );
    LINK_CONTROL( "errand_buy_list", m_pkErrandBuyList );

    LINK_CONTROL( "game_start_img", m_pkImgGameStart );
    LINK_CONTROL( "game_end_img", m_pkImgGmaeEnd );
    LINK_CONTROL( "game_succes_img", m_pkImgGmaeSucces );

    m_eGameNowState = ERRAND_CHECK_GAME;
    m_eMoveState = PLAYER_MOVE_NONE;

}

KGCChallengeErrandPanel::~KGCChallengeErrandPanel( void )
{
}

void KGCChallengeErrandPanel::OnCreate( void )
{

    m_fDetPos = 0.0f;
    m_dwAccTimeNot = 0;
    m_dwAccMerryApperTime = 0;
    m_dwPlayTime = 0;
    m_dwStageDlayTime = 0;
    m_iStage = 1;
    m_bReciveReward = false;

    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apErrandShop[i]->InitState( true, true, this );

        if( m_vecShopPos.size() != NUM_ERRAND_SHOP )
        {
            m_apErrandShop[i]->SetShopIndex( i );
            m_apErrandShop[i]->SetModuleIndex( i );
            m_vecShopPos.push_back( m_apErrandShop[i]->GetFixedWindowLocalPos() );
        }

        m_apStaticHaveNum[i]->InitState(true);
        m_apStaticHaveNum[i]->SetAlign(DT_CENTER);
        m_apStaticHaveNum[i]->SetText( 0 );
        m_apStaticHaveNum[i]->SetFontColor(D3DCOLOR_ARGB( 255, 255, 255, 255));
    }


    for ( int i = 0; i < NUM_TIME_CIPHER; i++ ) 
    {
        m_apTime[i]->InitState( true );
        m_apTime[i]->SetStringType( "errand_time_num" );
        m_apTime[i]->SetNumber( 0 );
    }

    for(int i = 0; i < NUM_MAX_COUNT; i++ )
    {
        m_apSuccesCount[i]->InitState( true );
        m_apSuccesCount[i]->SetStringType( "errand_succes_num" );
        m_apSuccesCount[i]->SetNumber( 0 );
    }

    m_pkErrandHome->InitState( true, true, this );
    m_pkErrandPlayer->InitState( true, true, this );
    m_pkErrandBuyList->InitState( false, false, this );

    m_pkImgGameStart->InitState( false );
    m_pkImgGmaeEnd->InitState( false );
    m_pkImgGmaeSucces->InitState( false );

}

void KGCChallengeErrandPanel::OnCreateComplete( void )
{
    D3DXVECTOR2 vTargetPos= m_pkErrandPlayer->GetFixedWindowLocalPos();
    vTargetPos.x = m_pkErrandHome->StayLocationX();
    vTargetPos.y = m_pkErrandHome->StayLocationY() - ( m_pkErrandPlayer->GetHeight() );
    m_pkErrandPlayer->SetFixedWindowLocalPos( vTargetPos );

    m_dwPlayTime = SiGCChallengeErrandManager()->GetPlayTime();
    m_nMarryAppearInterval = SiGCChallengeErrandManager()->GetMarryAppearTime();

    m_apSuccesCount[NUM_SUCCES_COUNT]->SetNumber( m_pkErrandPlayer->GetSuccesCount());
    m_apSuccesCount[NUM_SUCCES_MAX]->SetNumber( SiGCChallengeErrandManager()->GetMaxStage() );

    m_eGameNowState = ERRAND_CHECK_GAME;

}

void KGCChallengeErrandPanel::ActionPerformed( const KActionEvent& event )
{

}

void KGCChallengeErrandPanel::OnStart()
{

    if( SiGCChallengeErrandManager()->GetPlayble() )
    {
        m_dwStageDlayTime = ::GetTickCount();
        m_pkImgGameStart->InitState(true);
        m_bReciveReward = false;
        m_dwPlayTime = SiGCChallengeErrandManager()->GetPlayTime();
        m_pkErrandPlayer->ResetEndGame();
        m_eGameNowState = ERRAND_READY_GAME;
        g_KDSound.Play( "Errand_Game_Start" );
    }
    else
    {
        // 플레이 불가.. 코인 없다.. 시간 기다려..
    }


}

void KGCChallengeErrandPanel::OnReward( void )
{
    m_bReciveReward = true;
    GameRewardReq();

    std::vector<KItem> vecKitem        = SiGCChallengeErrandManager()->GetRewardItem();
    std::vector<KItem> vecSpecialKitem = SiGCChallengeErrandManager()->GetSpecialRewardItem();

    if( vecSpecialKitem.empty() )
        g_KDSound.Play( "Errand_Game_Reward" );
    else
        g_KDSound.Play( "Errand_Game_Special_Reward" );

    if( !vecKitem.empty() )
    {
        int index = 0;
        std::vector<KItem>::iterator itor;
        for(itor = vecKitem.begin(); itor != vecKitem.end(); ++itor)
        {
            KItem kItem = *itor;
            m_apErrandShop[index]->SetRewardItem( kItem );
            index++;
            if( index == KGCChallengeErrandModule::NAME_HOME )
                break;
        }
    }

    if( !vecSpecialKitem.empty())
    {
        std::vector<KItem>::iterator itor;
        for(itor = vecSpecialKitem.begin(); itor != vecSpecialKitem.end(); ++itor)
        {
            KItem kItem = *itor;
            m_pkErrandHome->SetRewardItem( kItem );
            break;          // 첫번째 받은 아이템 디스플레이만.
        }
    }

    ClearShop();
    m_pkErrandPlayer->InitState( false );
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apErrandShop[i]->UpdateRewardRender( true );
    }
    m_pkErrandHome->UpdateRewardRender( true );
    m_pkErrandPlayer->ResetEndGame();
    

    m_eGameNowState = ERRAND_WAIT_REMAIN;

}


void KGCChallengeErrandPanel::RendomShop( void )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        D3DXVECTOR2 vPos = m_vecShopPos[i];
        m_apErrandShop[i]->SetWindowPos( vPos );
        m_apErrandShop[i]->SetModuleIndex( i );
    }
}

void KGCChallengeErrandPanel::FrameMoveInEnabledState( void )
{
    /*
        1. ERRAND_END_GAME 
        게임이 끝난 상태 도전 포인트 삭제 한다.
        성공 카운트가 1이상일때는 보상을 받을수 있다. 
        0이면 바로 도전포인트 기다리는 상태
        게임을 종료 시키면 보상을 인벤에 넣어주고 다시 키면 ERRAND_WAIT_REMAIN 상태
        보상 받기를 누르면 ERRAND_WAIT_REMAIN 상태.
    */
    switch( m_eGameNowState )
    {
    case ERRAND_CHECK_GAME:
        CheckFrameMove();
         break;
    case ERRAND_READY_GAME:
        ReadyFrameMove();
        break;
    case ERRAND_PLAYING_GAME:
        PlayFrameMove();
        break;
    case ERRAND_PLAYER_MOVE:
        MovePlayer();
        break;
    case ERRAND_END_GAME:
        RemainTime();
        break;
    case ERRAND_WAIT_REMAIN:
        RemainTime();
        break;
    }
}

void KGCChallengeErrandPanel::CheckFrameMove( void )
{
    if( !SiGCChallengeErrandManager()->GetPlayble() )
    {
        m_eGameNowState = ERRAND_WAIT_REMAIN;
    }

}
void KGCChallengeErrandPanel::ReadyFrameMove( void )
{

    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apErrandShop[i]->UpdateRewardRender( false );
        m_apErrandShop[i]->ClearItem();
    }
    m_pkErrandHome->UpdateRewardRender( false );
    m_pkErrandHome->ClearItem();

    DWORD dwTmpTime = ::GetTickCount();

    if ( (dwTmpTime - m_dwStageDlayTime ) < dwLmitDelayTime )
        return;

    m_pkErrandPlayer->ResetPlayer();
    ResetStageClear();

    m_pkImgGameStart->InitState( false );
    m_pkErrandPlayer->InitState( true );
    m_apSuccesCount[NUM_SUCCES_COUNT]->SetNumber( m_pkErrandPlayer->GetSuccesCount() );


    SiGCChallengeErrandManager()->SetStageInfo( m_iStage );

    m_pkErrandBuyList->InitState( true );
    m_pkErrandBuyList->SetBuyList();

    RendomShop();
    m_eGameNowState = ERRAND_PLAYING_GAME;
}

void KGCChallengeErrandPanel::PlayFrameMove()
{
    SetPlayTime();
    PlayEndCheck();
    UpdateKey();
    MerryAppear();
}

void KGCChallengeErrandPanel::PlayEndCheck( void )
{

    if ( m_pkErrandPlayer->GetSuccesCount() == SiGCChallengeErrandManager()->GetMaxStage() )
    {
        if( m_pkErrandBuyList->IsRenderOn() )
        {
            m_pkErrandBuyList->InitState( false );
        }

        m_pkImgGmaeSucces->InitState( true );
        m_dwPlayTime = SiGCChallengeErrandManager()->GetPlayTime();
        m_dwStageDlayTime = ::GetTickCount();

        D3DXVECTOR2 vPosWindow = this->GetFixedWindowPos();
        vPosWindow.x += this->GetWidth() / 2;
        vPosWindow.y += this->GetHeight() / 2;
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Bingo_06", PARTICLEPOS_X(vPosWindow.x) , PARTICLEPOS_Y(vPosWindow.y), 0.5f ), GC_LAYER_UI );
        g_KDSound.Play( "Errand_Game_End_Succes" );
        GameEndReq();
        if( m_pkErrandPlayer->GetSuccesCount() == 0 )
            m_eGameNowState = ERRAND_WAIT_REMAIN;
        else
            m_eGameNowState = ERRAND_END_GAME;
    }
    else if( m_dwPlayTime <= 0 )
    {
        if( m_pkErrandBuyList->IsRenderOn() )
        {
            m_pkErrandBuyList->InitState( false );
        }
        m_pkImgGmaeEnd->InitState( true );
        m_dwPlayTime = SiGCChallengeErrandManager()->GetPlayTime();
        m_dwStageDlayTime = ::GetTickCount();
        g_KDSound.Play( "Errand_Game_Over" );
        GameEndReq();
        if( m_pkErrandPlayer->GetSuccesCount() == 0 )
            m_eGameNowState = ERRAND_WAIT_REMAIN;
        else
            m_eGameNowState = ERRAND_END_GAME;
    }
}

void KGCChallengeErrandPanel::UpdateKey()
{

    if( m_pkErrandBuyList->IsRenderOn() )
        return;

    if( g_pkInput->IsDown( DIK_RIGHT ) )
    {
        PlayerMoveRight();
    }
    else if( g_pkInput->IsDown( DIK_LEFT ) )
    {
        PlayerMoveLeft();
    }
    else if( g_pkInput->IsDown( DIK_UP ) )
    {
        PlayerMoveUp();
    }
    else if( g_pkInput->IsDown( DIK_DOWN ) )
    {
        PlayerMoveDown();
    }
    else if( g_pkInput->IsDown( DIK_SPACE) )
    {
        PlayerInputSpace();
    }
}

void KGCChallengeErrandPanel::PlayerInputSpace( void )
{
    int iCurrentIndex = m_pkErrandPlayer->GetCurrentLocation();

    if( iCurrentIndex == KGCChallengeErrandModule::NAME_HOME )
    {
        bool bSucces = CompareItemList();

        if( bSucces )
        {
            m_pkErrandPlayer->AddSuccesCount();
            m_pkErrandPlayer->SetPlayerEmotion(true, D3DWM_DEFAULT );
            m_iStage++;
        }
        else
        {
            m_pkErrandPlayer->SetPlayerEmotion(true, D3DWM_HOVER );
        }

        m_eGameNowState = ERRAND_READY_GAME;
    }
    else
    {
        BuyErrandItem();
    }
}

void KGCChallengeErrandPanel::UpdateErrandObj( )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apErrandShop[i]->SetImgRenderOff( );
    }
    m_pkErrandHome->SetImgRenderOff( );

    int iCurrent = m_pkErrandPlayer->GetCurrentLocation();
    if( iCurrent == KGCChallengeErrandModule::NAME_HOME )
    {
        m_pkErrandHome->SetImgRenderOn( );
    }
    else
    {
        m_apErrandShop[iCurrent]->SetImgRenderOn( );
    }

    m_pkErrandPlayer->PlayerDirection( false );
    m_pkErrandPlayer->SetAnimDefalult( );
}

void KGCChallengeErrandPanel::BuyErrandItem( void )
{
    int iCurrentIndex = m_pkErrandPlayer->GetCurrentLocation();
    int iType = m_apErrandShop[iCurrentIndex]->GetShopName();

    if( iCurrentIndex >= NUM_ERRAND_SHOP || iType >= NUM_ERRAND_SHOP )
        return;

    int iBuyDefalutBuy = 1;

    ErrandItemBuyList eBuyList;
    eBuyList.m_nType = iType;
    eBuyList.m_nItemNum = iBuyDefalutBuy;

    int iBuyNum = m_pkErrandPlayer->AddBuyList( eBuyList );

    if( eBuyList.m_nType < KGCChallengeErrandModule::NAME_HOME )
    {
        m_apStaticHaveNum[iType]->SetText( iBuyNum );
        m_pkErrandPlayer->SetShowSuccesParticle( );
        g_KDSound.Play( "Errand_Game_Buy" );
    }

}

bool BuyListCompare( ErrandItemBuyList& lhs, ErrandItemBuyList& rhs )
{
    return lhs.m_nType > rhs.m_nType;
}

bool KGCChallengeErrandPanel::CompareItemList( void )
{
    std::vector<ErrandItemBuyList> vecBuyList = SiGCChallengeErrandManager()->GetBuyInfo();
    std::vector<ErrandItemBuyList> vecPlayerBuyList = m_pkErrandPlayer->GetPlayerBuyInfo();

    std::sort( vecBuyList.begin(), vecBuyList.end(), BuyListCompare );
    std::sort( vecPlayerBuyList.begin(), vecPlayerBuyList.end(), BuyListCompare );

    if( vecBuyList == vecPlayerBuyList )
    {
        int nCurrentTime = m_dwPlayTime;
        m_pkErrandPlayer->SetShowSuccesParticle();
        m_dwPlayTime += (nCurrentTime / 100) * SiGCChallengeErrandManager()->GetTimeIncreaseRate();
        g_KDSound.Play( "Errand_Game_succes" );
        return true;
    }
    else
    {
        int nCurrentTime = m_dwPlayTime;
        m_dwPlayTime -= (nCurrentTime / 100) * SiGCChallengeErrandManager()->GetTimeIncreaseRate();
        g_KDSound.Play( "Errand_Game_Fail" );
        return false;
    }
}

void KGCChallengeErrandPanel::ResetStageClear( void )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apStaticHaveNum[i]->SetText( 0 );
    }
    ClearShop();
}

void KGCChallengeErrandPanel::ResetGame( void )
{
    m_iStage = 1;
    m_pkErrandPlayer->SetCurrentLocation( KGCChallengeErrandModule::NAME_HOME);
    D3DXVECTOR2 vTargetPos= m_pkErrandPlayer->GetFixedWindowLocalPos();
    vTargetPos.x = m_pkErrandHome->StayLocationX();
    vTargetPos.y = m_pkErrandHome->StayLocationY() - ( m_pkErrandPlayer->GetHeight() );
    m_pkErrandPlayer->SetFixedWindowLocalPos( vTargetPos );
}

void KGCChallengeErrandPanel::PlayerMoveRight()
{
    int iCurrentIndex = m_pkErrandPlayer->GetCurrentLocation();
    int iMoveIndex = iCurrentIndex % 3;


    if( iMoveIndex != NOT_MOVE_TOP_RIGHT && iCurrentIndex < KGCChallengeErrandModule::NAME_MEAT )
    {
        iCurrentIndex += ERRAND_X_MOVE;

        m_fDetPos = m_apErrandShop[iCurrentIndex]->StayLocationX();
        m_pkErrandPlayer->SetCurrentLocation( iCurrentIndex );
        m_pkErrandPlayer->PlayerDirection( true );

        m_eGameNowState = ERRAND_PLAYER_MOVE;
        m_eMoveState = PLAYER_MOVE_RIGHT;
    }
    else if ( iCurrentIndex == KGCChallengeErrandModule::NAME_MEAT )
    {
        iCurrentIndex += ERRAND_X_MOVE;
        m_fDetPos = m_pkErrandHome->StayLocationX();
        m_pkErrandPlayer->SetCurrentLocation( iCurrentIndex );
        m_pkErrandPlayer->PlayerDirection( true );

        m_eGameNowState = ERRAND_PLAYER_MOVE;
        m_eMoveState = PLAYER_MOVE_RIGHT;
    }
}

void KGCChallengeErrandPanel::PlayerMoveLeft()
{
    int iCurrentIndex = m_pkErrandPlayer->GetCurrentLocation();
    int iMoveIndex = iCurrentIndex % 3;

    if( iMoveIndex != NOT_MOVE_BOTTON_LEFT || iCurrentIndex == KGCChallengeErrandModule::NAME_HOME )
    {
        iCurrentIndex -= ERRAND_X_MOVE;
        m_fDetPos = m_apErrandShop[iCurrentIndex]->StayLocationX();
        m_pkErrandPlayer->SetCurrentLocation( iCurrentIndex );
        m_pkErrandPlayer->PlayerDirection( false );

        m_eGameNowState = ERRAND_PLAYER_MOVE;
        m_eMoveState = PLAYER_MOVE_LEFT;
    }
}

void KGCChallengeErrandPanel::PlayerMoveUp()
{
    int iCurrentIndex = m_pkErrandPlayer->GetCurrentLocation();
    int iMoveIndex = iCurrentIndex / 3;

    if( iMoveIndex != NOT_MOVE_TOP_RIGHT && iCurrentIndex < KGCChallengeErrandModule::NAME_HOME )
    {
        iCurrentIndex += ERRAND_Y_MOVE;
        m_fDetPos = m_apErrandShop[iCurrentIndex]->StayLocationY()- ( m_pkErrandPlayer->GetHeight() );
        m_pkErrandPlayer->SetCurrentLocation( iCurrentIndex );

        m_eGameNowState = ERRAND_PLAYER_MOVE;
        m_eMoveState = PLAYER_MOVE_UP;
    }
}

void KGCChallengeErrandPanel::PlayerMoveDown()
{
    int iCurrentIndex = m_pkErrandPlayer->GetCurrentLocation();
    int iMoveIndex = iCurrentIndex / 3;

    if( iMoveIndex != NOT_MOVE_BOTTON_LEFT && iCurrentIndex < KGCChallengeErrandModule::NAME_HOME )
    {
        iCurrentIndex -= ERRAND_Y_MOVE;
        m_fDetPos = m_apErrandShop[iCurrentIndex]->StayLocationY() - ( m_pkErrandPlayer->GetHeight() );
        m_pkErrandPlayer->SetCurrentLocation( iCurrentIndex );

        m_eGameNowState = ERRAND_PLAYER_MOVE;
        m_eMoveState = PLAYER_MOVE_DOWN;
    }
}


void KGCChallengeErrandPanel::MovePlayer( )
{
    m_pkErrandPlayer->SetPlayerEmotion( false , D3DWM_DEFAULT );

    D3DXVECTOR2 vPlayerPos = m_pkErrandPlayer->GetFixedWindowLocalPos();

    switch( m_eMoveState )
    {
    case PLAYER_MOVE_LEFT:
        {
            if( m_fDetPos < vPlayerPos.x)
            {
                vPlayerPos.x -= m_pkErrandPlayer->GetPlayerMoveSpeed();
                m_pkErrandPlayer->SetFixedWindowLocalPos( vPlayerPos );
                m_pkErrandPlayer->MoveAnim( );
            }
            else
            {
                MerryMeetCheck();
                UpdateErrandObj();
                m_eGameNowState = ERRAND_PLAYING_GAME;
            }
        }
        break;
    case PLAYER_MOVE_RIGHT:
        {
            if( m_fDetPos > vPlayerPos.x)
            {
                vPlayerPos.x += m_pkErrandPlayer->GetPlayerMoveSpeed();
                m_pkErrandPlayer->SetFixedWindowLocalPos( vPlayerPos );
                m_pkErrandPlayer->MoveAnim( );
            }
            else
            {
                MerryMeetCheck();
                UpdateErrandObj();
                m_eGameNowState = ERRAND_PLAYING_GAME;
            }
        }
        break;
    case PLAYER_MOVE_UP:
        {
            if( m_fDetPos < vPlayerPos.y)
            {
                vPlayerPos.y -= m_pkErrandPlayer->GetPlayerMoveSpeed();
                m_pkErrandPlayer->SetFixedWindowLocalPos( vPlayerPos );
                m_pkErrandPlayer->MoveAnim();
            }
            else
            {
                MerryMeetCheck();
                UpdateErrandObj();
                m_eGameNowState = ERRAND_PLAYING_GAME;
            }
        }
        break;
    case PLAYER_MOVE_DOWN:
        {
            if( m_fDetPos > vPlayerPos.y)
            {
                vPlayerPos.y += m_pkErrandPlayer->GetPlayerMoveSpeed();
                m_pkErrandPlayer->SetFixedWindowLocalPos( vPlayerPos );
                m_pkErrandPlayer->MoveAnim();
            }
            else
            {
                MerryMeetCheck();
                UpdateErrandObj();
                m_eGameNowState = ERRAND_PLAYING_GAME;
            }
        }
        break;
    }
}

void KGCChallengeErrandPanel::SetPlayTime( void )
{
    int iTime = (timeGetTime() - m_dwAccTimeNot );

    if( iTime >= 1000 )
    {
        m_dwAccTimeNot = timeGetTime();
        if( m_dwPlayTime > 0 )
            m_dwPlayTime--;
    }

    int nMinOne = ( m_dwPlayTime / 60 ) / 10;
    int nMinTwo = ( m_dwPlayTime / 60 ) % 10;
    int nSecOne = ( m_dwPlayTime % 60 ) / 10;
    int nSecTwo = ( m_dwPlayTime % 60 ) % 10;

    m_apTime[NUM_TIME_MIT_ONE]->SetNumber( nMinOne );
    m_apTime[NUM_TIME_MIT_TWO]->SetNumber( nMinTwo );
    m_apTime[NUM_TIME_SEC_ONE]->SetNumber( nSecOne );
    m_apTime[NUM_TIME_SEC_TWO]->SetNumber( nSecTwo );
}

void KGCChallengeErrandPanel::RemainTime( void )
{

    DWORD dwTmpTime = ::GetTickCount();

    if ( (dwTmpTime - m_dwStageDlayTime ) < dwLmitDelayTime )
        return;

    m_pkImgGmaeEnd->InitState( false );
    m_pkImgGmaeSucces->InitState( false );


    if( SiGCChallengeErrandManager()->GetPlayble() )
    {
        if( m_bReciveReward == false )
        {
            GameRewardReq();
            ClearShop();
            m_pkErrandPlayer->ResetEndGame();
        }

        ResetGame();
        m_eGameNowState = ERRAND_CHECK_GAME;
    }

    int nPrevTime = SiGCChallengeErrandManager()->GetLastRewardTime( );

    if ( m_nPrevTime != nPrevTime ) 
    {

        m_nPrevTime = nPrevTime % ( 60 * 60 );
        
        int nMinOne = ( m_nPrevTime / 60 ) / 10;
        int nMinTwo = ( m_nPrevTime / 60 ) % 10;
        int nSecOne = ( m_nPrevTime % 60 ) / 10;
        int nSecTwo = ( m_nPrevTime % 60 ) % 10;

        m_apTime[NUM_TIME_MIT_ONE]->SetNumber( nMinOne );
        m_apTime[NUM_TIME_MIT_TWO]->SetNumber( nMinTwo );
        m_apTime[NUM_TIME_SEC_ONE]->SetNumber( nSecOne );
        m_apTime[NUM_TIME_SEC_TWO]->SetNumber( nSecTwo );
    }

}

void KGCChallengeErrandPanel::MerryAppear( void )
{

    int iTime = (timeGetTime() - m_dwAccMerryApperTime );
     
    if( iTime >= 1000 )
    {
        m_dwAccMerryApperTime = timeGetTime();
        if( m_nMarryAppearInterval > 0 )
        {
            m_nMarryAppearInterval--;
            if( m_nMarryAppearInterval == 0 )
            {
                m_nRandomMerry = RandomNumber( KGCChallengeErrandModule::NAME_BREAD, KGCChallengeErrandModule::NAME_MEAT );
                m_apErrandShop[m_nRandomMerry]->MerryRender( true );
                m_apErrandShop[m_nRandomMerry]->SetMerry( true );
            }
        }
        else
        {
             if(m_nMarryKeepAliveTime == SiGCChallengeErrandManager()->GetMarryKeepAliveTime() )
             {
                m_nMarryKeepAliveTime = 0;
                m_apErrandShop[m_nRandomMerry]->MerryRender( false );
                m_apErrandShop[m_nRandomMerry]->SetMerry( false );
                m_nMarryAppearInterval = SiGCChallengeErrandManager()->GetMarryAppearTime();
             }
             m_nMarryKeepAliveTime++;
        }
    }
}


bool KGCChallengeErrandPanel::MerryMeetCheck( void )
{
    bool bMerry = false;
    int iCurrent = m_pkErrandPlayer->GetCurrentLocation();

    if( iCurrent >= KGCChallengeErrandModule::NAME_HOME )
        return false;

    int nMerryIndex = m_apErrandShop[iCurrent]->GetMerry();

    if( nMerryIndex )
    {
        m_apErrandShop[m_nRandomMerry]->MoveMerryAmin();
        m_pkErrandPlayer->SetPlayerEmotion(true, D3DWM_SELECT );
        std::vector<ErrandItemBuyList> vecPlayerBuyList = m_pkErrandPlayer->GetPlayerBuyInfo();
        g_KDSound.Play( "Errand_Game_Merry" );

        if( vecPlayerBuyList.empty() )
            return false;

        ErrandItemBuyList eBuyList = m_pkErrandPlayer->LostItem();
        if( eBuyList.m_nType < KGCChallengeErrandModule::NAME_HOME )
            m_apStaticHaveNum[eBuyList.m_nType]->SetText( eBuyList.m_nItemNum );
        return true;
    }

    return false;
}

void KGCChallengeErrandPanel::ClearShop( void )
{
    for( int i = 0; i < NUM_ERRAND_SHOP; i++ )
    {
        m_apErrandShop[i]->SetMerry( false );
        m_apErrandShop[i]->MerryRender( false );
    }
}
void KGCChallengeErrandPanel::GameEndReq( void )
{
    Result_ReceiveErrandEnd = INT_MAX;
    KP2P::GetInstance()->Send_ErrandGameEndReq( m_pkErrandPlayer->GetSuccesCount() );
    g_MyD3D->WaitForServerAck( Result_ReceiveErrandEnd, INT_MAX, 3000, TIME_OUT_VALUE );
}

void KGCChallengeErrandPanel::GameRewardReq( void )
{
    if( m_pkErrandPlayer->GetSuccesCount() == 0 )
        return;

    Result_ReceiveErrandReward = INT_MAX;
    KP2P::GetInstance()->Send_ErrandRewardReq( m_pkErrandPlayer->GetSuccesCount() );
    g_MyD3D->WaitForServerAck( Result_ReceiveErrandReward, INT_MAX, 3000, TIME_OUT_VALUE );

}
