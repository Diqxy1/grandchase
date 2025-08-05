#include "stdafx.h"
#include "GCChallengeErrandPlayer.h"

IMPLEMENT_CLASSNAME( KGCChallengeErrandPlayer );
IMPLEMENT_WND_FACTORY( KGCChallengeErrandPlayer );
IMPLEMENT_WND_FACTORY_NAME( KGCChallengeErrandPlayer, "gc_errand_player" );


KGCChallengeErrandPlayer::KGCChallengeErrandPlayer( void ):
m_pkPlayerRight( NULL ),
m_pkPlayerLeft( NULL ),
m_pkPlayer( NULL ),
m_pkPlayerEmotion( NULL )
{
    m_iCurrentLocation = KGCChallengeErrandModule::NAME_HOME;
    m_bRight = false;
    m_iAniFrame = 0;

    LINK_CONTROL( "player_right", m_pkPlayerRight );
    LINK_CONTROL( "player_left", m_pkPlayerLeft);
    LINK_CONTROL( "player_emotion", m_pkPlayerEmotion);

}

KGCChallengeErrandPlayer::~KGCChallengeErrandPlayer( void )
{
}

void KGCChallengeErrandPlayer::OnCreate( void )
{
    m_pkPlayerLeft->InitState( true );
    m_pkPlayerLeft->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f,0.0f));
    m_pkPlayerRight->InitState( true );
    m_pkPlayerRight->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));

    m_pkPlayerEmotion->InitState( true );
    m_pkPlayerEmotion->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));

    m_pkPlayer = m_pkPlayerLeft;
    m_iCurrentLocation = KGCChallengeErrandModule::NAME_HOME;

    m_vecPlayerBuyList.clear();
    m_iSuccessCount = 0;
}

void KGCChallengeErrandPlayer::ResetEndGame( void )
{
    m_iSuccessCount = 0;
    m_bRight = false;

    PlayerDirection( m_bRight );
    SetPlayerEmotion( false );
    SetAnimDefalult();
}

void KGCChallengeErrandPlayer::ResetPlayer( void )
{
    m_vecPlayerBuyList.clear();
    m_iCurrentLocation = KGCChallengeErrandModule::NAME_HOME;
}

void KGCChallengeErrandPlayer::ActionPerformed( const KActionEvent& event )
{
}


void KGCChallengeErrandPlayer::FrameMoveInEnabledState( void )
{

}

void KGCChallengeErrandPlayer::MoveAnim( )
{

    if ( m_iAniFrame < NUM_WINDOW_MODE )
    {
        if (m_pkPlayer)
            m_pkPlayer->SetWndMode(static_cast<ED3DWndMode>(m_iAniFrame));
        m_iAniFrame++;
    }
    else
    {
        m_iAniFrame = 0;
    }
    return;

}

void KGCChallengeErrandPlayer::PlayerDirection( bool bRight )
{
    m_bRight = bRight;
    m_iAniFrame = 0;

    if (m_bRight)
    {
        m_pkPlayerRight->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f,0.0f));
        m_pkPlayerLeft->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));

        m_pkPlayer = m_pkPlayerRight;
    }
    else
    {
        m_pkPlayerRight->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));
        m_pkPlayerLeft->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f,0.0f));
        m_pkPlayer = m_pkPlayerLeft;
    }
}

void KGCChallengeErrandPlayer::SetAnimDefalult( void )
{
    m_iAniFrame = 0;

    if (m_pkPlayer)
        m_pkPlayer->SetWndMode(static_cast<ED3DWndMode>(m_iAniFrame));
}

void KGCChallengeErrandPlayer::SetPlayerEmotion(bool bRender, ED3DWndMode eMode )
{
    if( bRender )
    {
        m_pkPlayerLeft->InitState( false );
        m_pkPlayerEmotion->SetWndMode( eMode );
        m_pkPlayerEmotion->SetFixedWindowLocalPos(D3DXVECTOR2(0.0f, -25.0f));
    }
    else
    {
        m_pkPlayerEmotion->SetFixedWindowLocalPos(D3DXVECTOR2(1000.0f,1000.0f));
        m_pkPlayerLeft->InitState( true );
    }
}

int KGCChallengeErrandPlayer::AddBuyList( ErrandItemBuyList& eBuyList )
{
    if( m_vecPlayerBuyList.empty() )
    {
        m_vecPlayerBuyList.push_back( eBuyList );
        return eBuyList.m_nItemNum;
    }

    std::vector<ErrandItemBuyList>::iterator itor;
    for(itor = m_vecPlayerBuyList.begin(); itor != m_vecPlayerBuyList.end(); ++itor)
    {
        if( itor->m_nType == eBuyList.m_nType )
        {
            itor->m_nItemNum += eBuyList.m_nItemNum; 
            return itor->m_nItemNum;
        }
    }

    if( itor == m_vecPlayerBuyList.end() )
    {
        m_vecPlayerBuyList.push_back( eBuyList );
        return eBuyList.m_nItemNum;
    }

    return -1;
}

ErrandItemBuyList KGCChallengeErrandPlayer::LostItem( void )
{
    std::vector<ErrandItemBuyList>::iterator itor;

    if( m_vecPlayerBuyList.empty() )
        return *(itor);

    for(itor = m_vecPlayerBuyList.begin(); itor != m_vecPlayerBuyList.end(); itor++)
    {
        if(itor->m_nItemNum != 0 )
        {
            itor->m_nItemNum--;
            return *(itor);
        }
        else
        {
            return *(itor);
        }
    }

    return *(itor);

}

void KGCChallengeErrandPlayer::SetShowSuccesParticle()
{
    D3DXVECTOR2 vPosWindow = this->GetFixedWindowPos();
    vPosWindow.x += this->GetWidth() / 2;
    vPosWindow.y += this->GetHeight() / 2;

#if defined( NATION_KOREA ) || defined( NATION_THAILAND ) || defined( NATION_USA ) || defined( NATION_IDN ) || defined( NATION_LATINAMERICA )
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Special_Reward_03", PARTICLEPOS_X(vPosWindow.x) , PARTICLEPOS_Y(vPosWindow.y), 0.5f ), GC_LAYER_UI );
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Special_Reward_03", PARTICLEPOS_X(vPosWindow.x) , PARTICLEPOS_Y(vPosWindow.y), 0.5f ), GC_LAYER_UI );
#else
    // 한국리소스에 있는 파티클과 브라질 파티클 위치가 다르다.
    // gravity * 화면 높이 및 넓이
    float fValnceX = 163.84f;

    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Special_Reward_03", PARTICLEPOS_X(vPosWindow.x - fValnceX)  , PARTICLEPOS_Y(vPosWindow.y ), 0.5f ), GC_LAYER_UI );
    g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("Special_Reward_03", PARTICLEPOS_X(vPosWindow.x - fValnceX) , PARTICLEPOS_Y(vPosWindow.y ), 0.5f ), GC_LAYER_UI );
#endif

}