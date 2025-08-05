#include "stdafx.h"
#include "GCSquareLoadingScene.h"


IMPLEMENT_CLASSNAME( KGCSquareLoadingScene );
IMPLEMENT_WND_FACTORY( KGCSquareLoadingScene );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareLoadingScene, "gc_square_loading_scene" );

#define DOOR_OPEN_WAIT (1000)

KGCSquareLoadingScene::KGCSquareLoadingScene(void)
: m_pkBarGage(NULL)
, m_pkOpenDoor(NULL)
, m_pkOpenDoor2(NULL)
, m_pkBar(NULL)
, m_pkBar2(NULL)
, m_pkdoor(NULL)
, m_pkdoor2(NULL)
, m_pkCuteElesis(NULL)
, m_pkLoading1(NULL)
, m_pkLoading2(NULL)
, m_pkLoadingGuild1(NULL)
, m_pkLoadingGuild2(NULL)
, m_vCompletePos( 640.0f, 0 ) // y는 상관없다
, m_vElesisGoalPos( 738.0f, 0 ) // y는 상관없다
, m_bEventEnd( false )
, m_bFirst( true )
, m_dwDoorOpenTime( 0 )
{
	LINK_CONTROL( "loading1", 		m_pkLoading1 );
	LINK_CONTROL( "loading2", 		m_pkLoading2 );
	LINK_CONTROL( "loading1_guild", m_pkLoadingGuild1 );
	LINK_CONTROL( "loading2_guild", m_pkLoadingGuild2 );

    LINK_CONTROL( "bar_gage", m_pkBarGage );
	LINK_CONTROL( "door_open", m_pkOpenDoor );
	LINK_CONTROL( "cute_elesis", m_pkCuteElesis );
	LINK_CONTROL( "door_open2", m_pkOpenDoor2 );

	LINK_CONTROL( "bar", m_pkBar );
	LINK_CONTROL( "bar2", m_pkBar2 );
	LINK_CONTROL( "door", m_pkdoor );
	LINK_CONTROL( "door2", m_pkdoor2 );
}

KGCSquareLoadingScene::~KGCSquareLoadingScene(void)
{
}

void KGCSquareLoadingScene::OnCreate()
{
    m_pkBarGage->InitState( true );
	m_pkCuteElesis->InitState( true );
	m_pkOpenDoor->InitState( false );
	m_pkOpenDoor2->InitState( false );

    if( true == m_bFirst )
    {
        m_bFirst = false;
        m_vElesisPos = m_pkCuteElesis->GetFixedWindowPos();
        m_vBarPos = m_pkBarGage->GetFixedWindowPos();
    }
    m_bEventEnd = false;
    m_dwDoorOpenTime = 0;

	RenderLoadingType();
}

void KGCSquareLoadingScene::SetGage( float fPercent )
{
    ASSERT( fPercent >= 0.0f && fPercent <= 1.0f );

    if( fPercent == 1.0 )
    {
        return EventFrameMove();
    }

    D3DXVECTOR2 vCurPos = m_pkCuteElesis->GetFixedWindowPos();
    float fXTerm = ( m_vCompletePos.x - m_vElesisPos.x ) * fPercent;
    vCurPos.x = m_vElesisPos.x + fXTerm;
    m_pkCuteElesis->SetWindowPos( vCurPos );
    m_pkBarGage->SetWidth( 1 + static_cast<DWORD>(vCurPos.x - m_vBarPos.x) );
}

void KGCSquareLoadingScene::EventFrameMove()
{
    if( 0 == m_dwDoorOpenTime )
    {
        m_dwDoorOpenTime = ::timeGetTime();
        return;
    }
    if( ::timeGetTime() - m_dwDoorOpenTime <= (DWORD)DOOR_OPEN_WAIT )
    {
        return;
    }

	if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD )
		m_pkOpenDoor2->ToggleRender( true );
	else
		m_pkOpenDoor->ToggleRender( true );

    D3DXVECTOR2 vCurPos = m_pkCuteElesis->GetFixedWindowPos();
    if( vCurPos.x >= m_vElesisGoalPos.x )
    {
        m_bEventEnd = true;
        return;
    }

    // 55 프레임동안 100
    const float fXTerm = 1.81818f;
    vCurPos.x += fXTerm;
    m_pkCuteElesis->SetWindowPos( vCurPos );
    m_pkBarGage->SetWidth( 1 + static_cast<DWORD>(vCurPos.x - m_vBarPos.x) );
    m_bEventEnd = false;
}

void KGCSquareLoadingScene::Init()
{
    m_pkOpenDoor->ToggleRender( false );
	m_pkOpenDoor2->ToggleRender( false );
    m_pkCuteElesis->SetWindowPos( m_vElesisPos );
    m_pkBarGage->SetWindowPos( m_vBarPos );
    m_pkBarGage->SetWidth( 1 );
    m_dwDoorOpenTime = 0;
    m_bEventEnd = false;
}

void KGCSquareLoadingScene::RenderLoadingType()
{
	bool bNormalSquare;
	if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD )
		bNormalSquare = false;
	else
		bNormalSquare = true;

	m_pkLoading1->ToggleRender(bNormalSquare);
	m_pkLoading2->ToggleRender(bNormalSquare);
	m_pkBar->ToggleRender(bNormalSquare);
	m_pkdoor->ToggleRender(bNormalSquare);

	m_pkLoadingGuild1->ToggleRender(!bNormalSquare);
	m_pkLoadingGuild2->ToggleRender(!bNormalSquare);
	m_pkBar2->ToggleRender(!bNormalSquare);
	m_pkdoor2->ToggleRender(!bNormalSquare);


}

void KGCSquareLoadingScene::FrameMoveInEnabledState()
{

}

void KGCSquareLoadingScene::ActionPerformed( const KActionEvent& event )
{

}

void KGCSquareLoadingScene::OnEnterScene()
{
    g_pkUIScene->m_pkOnGameDlg->ToggleRender( false );
    g_pkUIScene->m_pkToolTip->ToggleRender( false );
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );
    g_pkUIScene->EnableMessageBox( false );
}