#include "stdafx.h"
#include ".\gcsubmissionbar.h"
#include "mission/GCMissionManager.h"

IMPLEMENT_CLASSNAME( KGCSubMissionBar );
IMPLEMENT_WND_FACTORY( KGCSubMissionBar );
IMPLEMENT_WND_FACTORY_NAME( KGCSubMissionBar, "gc_submission_bar" );

KGCSubMissionBar::KGCSubMissionBar(void)
:m_eBarState( SMB_NORMAL )
{
    m_pkTitle = NULL;
    m_pkCount = NULL;
    for( int i = 0 ; i < SMB_NUM; ++i )
    {
        m_pkBar[i] = NULL;
    }

    LINK_CONTROL( "normal_bar", m_pkBar[SMB_NORMAL] );
    LINK_CONTROL( "hover_bar", m_pkBar[SMB_HOVER] );
    LINK_CONTROL( "down_bar", m_pkBar[SMB_DOWN] );
    LINK_CONTROL( "static_count", m_pkCount );
    LINK_CONTROL( "static_title", m_pkTitle );
}

KGCSubMissionBar::~KGCSubMissionBar(void)
{
}

void KGCSubMissionBar::ActionPerformed( const KActionEvent& event )
{
    
}

void KGCSubMissionBar::OnCreate()
{
    for( int i = 0 ; i < SMB_NUM ; ++i )
    {
        if( m_pkBar[i] == NULL )
        {
            ASSERT( !"스크립트가 이상합니다. 확인해주세요" );
        }
    }
    if( m_pkCount == NULL || m_pkTitle == NULL )
    {
        ASSERT( !"스크립트가 이상합니다. 확인해주세요" );
    }

    for( int i = 0 ; i <SMB_NUM ; ++i )
    {
        m_pkBar[i]->InitState( i == SMB_NORMAL, true, this );
    }
    m_pkCount->InitState( true, true, this );
    m_pkTitle->InitState( true, true, this );
    m_pkCount->SetAlign( DT_CENTER );
	m_pkTitle->SetAutoScroll( true );
}

void KGCSubMissionBar::FrameMoveInEnabledState()
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    if( this->CheckPosInWindowBound( vMousePos ) && m_iSubMissionID != -1 )
    {
        if( g_pkInput->BtnPressed( KInput::MBLEFT) )
        {
            SetCurrentState( SMB_DOWN );
        }
        else
        {
            SetCurrentState( SMB_HOVER );
        }

    }
    else
    {
        SetCurrentState( SMB_NORMAL );
    }
}

void KGCSubMissionBar::SetTitleString( std::wstring& strTitle )
{
    m_strTitle = strTitle;
    m_pkTitle->SetText( m_strTitle );
}

void KGCSubMissionBar::SetCount( int iCount, int iCompleteCount )
{
    std::wstringstream str;
    str<< iCount << L"/" << iCompleteCount;
    m_strCount = str.str();
    m_pkCount->SetText( m_strCount );
}

void KGCSubMissionBar::SetCurrentState( SMB_STATE eState )
{
    if( IsCorrectState( eState ) == false )
    {
        ASSERT( !"이상한 상태로 셋팅하려고 하네요?? 알아봅시다." );
        return;
    }

    m_eBarState = eState;
    for( int i = 0 ; i < SMB_NUM ; ++i )
    {
        m_pkBar[i]->ToggleRender( i == m_eBarState );
    }
}

bool KGCSubMissionBar::IsCorrectState( SMB_STATE eState )
{
    switch( eState )
    {
    case SMB_HOVER:
    case SMB_NORMAL:
    case SMB_DOWN:
        return true;
    default:
        return false;
    }
}

void KGCSubMissionBar::Reset()
{
    m_pkTitle->SetText();
    m_pkCount->SetText();
	m_iSubMissionID = -1;
	m_strCount.clear();
	m_strTitle.clear();
}

void KGCSubMissionBar::SetSubMissionID( int iSubMissionID )
{
    m_iSubMissionID = iSubMissionID;
}
