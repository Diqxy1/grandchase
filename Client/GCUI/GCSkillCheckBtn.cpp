#include "stdafx.h"
#include "GCSkillCheckBtn.h"
//

IMPLEMENT_CLASSNAME( KGCSkillCheckBtn );
IMPLEMENT_WND_FACTORY( KGCSkillCheckBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillCheckBtn, "gc_skill_check_button" );

KGCSkillCheckBtn*   g_pkSkillCheckBtn       = NULL;

#define                 BAR_ANI_TIME                100
#define                 STAR_ANI_TIME               150

KGCSkillCheckBtn::KGCSkillCheckBtn( void )
{
    g_pkSkillCheckBtn          = this;
    LINK_CONTROL( "win_static", m_pkSkillDesc );
    LINK_CONTROL( "button0",    m_pkStar[0]);
    LINK_CONTROL( "button1",    m_pkStar[1]);
    LINK_CONTROL( "button2",    m_pkStar[2]);
    LINK_CONTROL( "button3",    m_pkStar[3]);
    LINK_CONTROL( "ani0",       m_pkBar[0]);
    LINK_CONTROL( "ani1",       m_pkBar[1]);
    LINK_CONTROL( "ani2",       m_pkBar[2]);
    LINK_CONTROL( "ani3",       m_pkBar[3]);
    m_uiBtnIndex = 0;
    m_iLinkMotionID = -1;
}

KGCSkillCheckBtn::~KGCSkillCheckBtn( void )
{
}

void KGCSkillCheckBtn::OnCreate( void )
{
    m_pkSkillDesc->SetSelfInputCheck( false );
    for ( int i = 0; i < 4; i++ )
    {
        m_pkStar[i]->SetSelfInputCheck(false);
        m_pkBar[i]->SetSelfInputCheck(false);
    }    
}

void KGCSkillCheckBtn::OnDestroy( void )
{
}


void KGCSkillCheckBtn::FrameMoveInEnabledState( void )
{
}

void KGCSkillCheckBtn::PostChildDraw( void )
{
}

void KGCSkillCheckBtn::Init()
{
    m_pkSkillDesc->ToggleRender( true );
    for ( int i = 0; i < 4; i++ )
    {
        m_pkStar[i]->ToggleRender(false);
        m_pkBar[i]->ToggleRender(false);
    }
    m_uiBarAniCount         = 0;
    m_uiStarAniCount        = 0;
    m_bBarAni               = false;
    m_bStarAni              = false;
    m_dwStarAniAccumulateTime = 0;
    m_dwBarAniAccumulateTime  = 0;
    m_uiBtnIndex = 0;
    m_iLinkMotionID = -1;
}
void KGCSkillCheckBtn::AddString( std::wstring str_, DWORD dwColor_ )
{
    m_pkSkillDesc->SetText( str_ );
    m_pkSkillDesc->SetFontColor( dwColor_ );
}
void KGCSkillCheckBtn::ChangeStringColor( DWORD dwColor_ )
{
    m_pkSkillDesc->SetFontColor( dwColor_ );
}

void KGCSkillCheckBtn::FrameMove( DWORD dwElapsedTime )
{
    if ( m_bBarAni )
        FrameMoveBarAni( dwElapsedTime );
    if ( m_bStarAni )
        FrameMoveStarAni( dwElapsedTime );
}

void KGCSkillCheckBtn::FrameMoveStarAni( DWORD dwElapsedTime )
{
    m_dwStarAniAccumulateTime += dwElapsedTime;
    if ( m_dwStarAniAccumulateTime >= STAR_ANI_TIME )
    {
        if ( m_uiStarAniCount == ANIMATION_COUNT - 1 )
        {
            m_uiStarAniCount = 0;
            m_bStarAni = false;
            m_dwStarAniAccumulateTime = 0;
            for ( int i = 0; i < ANIMATION_COUNT; i++ )
            {
                m_pkStar[i]->ToggleRender( false );
            }
            return;
        }
        else
        {
            m_uiStarAniCount++;
        }   
        m_dwStarAniAccumulateTime = 0;
    }
    for ( int i = 0; i < ANIMATION_COUNT; i++ )
    {
        m_pkStar[i]->ToggleRender( false );
    }
    m_pkStar[m_uiStarAniCount]->ToggleRender( true );
}
void KGCSkillCheckBtn::FrameMoveBarAni( DWORD dwElapsedTime )
{
    m_dwBarAniAccumulateTime+= dwElapsedTime;
    if ( m_dwBarAniAccumulateTime >= BAR_ANI_TIME )
    {
        if ( m_uiBarAniCount == ANIMATION_COUNT - 1 )
        {
            m_uiBarAniCount = 0;
            m_bBarAni = false;
            m_dwBarAniAccumulateTime = 0;
            for ( int i = 0; i < ANIMATION_COUNT; i++ )
            {
                m_pkBar[i]->ToggleRender( false );
            }
            return;
        }
        else
        {
            m_uiBarAniCount++;
        }
        m_dwBarAniAccumulateTime = 0;
    }
    for ( int i = 0; i < ANIMATION_COUNT; i++ )
    {
        m_pkBar[i]->ToggleRender( false );
    }
    m_pkBar[m_uiBarAniCount]->ToggleRender( true );
}

void KGCSkillCheckBtn::EnableStarAni()
{
    if ( m_uiStarAniCount != 0 )
        return;
    m_bStarAni = true;
}

void KGCSkillCheckBtn::EnableBarAni()
{
    if ( m_uiBarAniCount != 0 )
        return;
    m_bBarAni = true;
}

void KGCSkillCheckBtn::SetMotionID( int iMotionID_ )
{
    m_iLinkMotionID = iMotionID_;
}
void KGCSkillCheckBtn::SetBtnIndex( UINT uiIndex_ )
{
    m_uiBtnIndex = uiIndex_;
}