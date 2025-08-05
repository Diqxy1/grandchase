#include "stdafx.h"
#include "KGCSquareChannelBar.h"

#include "MyD3D.h"
//

//
//
#include "../Square/GCSquare.h"
//#include "GCUI/KGCSquareEnterWnd.h"
#include "GCUI/KGCSquareEnterWndS6.h"
#include "KGCChannelManager.h"

#include "./GCRankingDlg.h"

IMPLEMENT_CLASSNAME( KGCSquareChannelBar );
IMPLEMENT_WND_FACTORY( KGCSquareChannelBar );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareChannelBar, "gc_square_channel_bar" );

KGCSquareChannelBar::KGCSquareChannelBar( void )
: m_pkChannelName(NULL)
, m_pkGage(NULL)
, m_dwSquareID(ULONG_MAX)
, m_iSelectIndex(0)
{
    LINK_CONTROL( "channel_name",   m_pkChannelName );
    LINK_CONTROL( "gage",           m_pkGage );
}

KGCSquareChannelBar::~KGCSquareChannelBar( void )
{
}

void KGCSquareChannelBar::OnCreate( void )
{
    m_vTextureCoords = m_pkGage->GetWndTexCoords();

    m_pkChannelName->InitState( true );
    m_pkGage->InitState( false );

    InitState( false, true, this );
}

void KGCSquareChannelBar::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( this, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickChannel );
}

void KGCSquareChannelBar::SetChannelName( const std::wstring& strChannelName )
{
    m_pkChannelName->SetText( strChannelName );
}

void KGCSquareChannelBar::SetGage( DWORD dwCurrent, DWORD dwMaximum )
{
    float fPercent = static_cast<float>( dwCurrent ) / static_cast<float>( dwMaximum );

    if( fPercent < 0.06f )
    {
        fPercent = 0.06f;
    }

    m_pkGage->ToggleRender( true );
    m_pkGage->SetWndTexCoords( D3DXVECTOR4( m_vTextureCoords.x,
                                            m_vTextureCoords.y,
                                            m_vTextureCoords.x + fPercent * ( m_vTextureCoords.z - m_vTextureCoords.x ),
                                            m_vTextureCoords.w ) );
    m_pkGage->SetWidth( (DWORD)( fPercent * MAX_SERVER_BAR_WIDTH ) );
}

void KGCSquareChannelBar::SetSquareChannelInfo( const KSquareInfo& kInfo, int iSelectIndex_ )
{
    m_kSquareInfo = kInfo;
    SetSquareID( m_kSquareInfo.m_dwID );
    SetChannelName( m_kSquareInfo.m_strName );
    SetGage( m_kSquareInfo.m_dwCurrentUserNum, m_kSquareInfo.m_dwMaxUserNum );
	m_iSelectIndex = iSelectIndex_;	
}

void KGCSquareChannelBar::OnClickChannel()
{
    //if( m_kSquareInfo.m_dwCurrentUserNum == m_kSquareInfo.m_dwMaxUserNum )
    //{
    //    g_pkUIScene->m_pkSquareEnterWnd->SetChannelSelectError( STR_ID_SQUARE_FULL );
    //    return;
    //}
	if( SiGCSquare()->GetSquareType() == KSquareInfo::ST_GUILD )
		SiGCSquare()->SetGuildSquareIndex(m_iSelectIndex);

    if( g_MyD3D->m_pStateMachine->IsJoinedChannel() )
    {
        SiGCSquare()->SetJoinedChannel( true );
        SiGCSquare()->SetChannelID( SiKGCChannelManager()->GetCurrentChannel() );
        SiGCSquare()->SetJoinedGuild( false );
    }
    else if( g_MyD3D->m_pStateMachine->IsJoinedGuild() )
    {
        SiGCSquare()->SetJoinedChannel( false );
        SiGCSquare()->SetJoinedGuild( true );
    }

    SiGCSquare()->SetPrevState( g_MyD3D->m_pStateMachine->GetState() );
    if( GS_SHOP_CASH == g_MyD3D->m_pStateMachine->GetState() || GS_SHOP_GP == g_MyD3D->m_pStateMachine->GetState() || GS_MY_INFO == g_MyD3D->m_pStateMachine->GetState() )
    {
        if( false == g_MyD3D->m_pStateMachine->SaveStateEmpty() )
        {
            SiGCSquare()->SetSavedState( g_MyD3D->m_pStateMachine->Get_SavedState() );
        }
    }
    SiGCSquare()->SetSquareID( m_dwSquareID );
	
	//# 랭킹 정보 미리 요청
	//g_pkUIScene->m_pkRanking->GuildRankReqAll();

    // 광장 입장
    g_pkUIScene->m_pkSquareEnterWndS6->OnClose();
    g_MyD3D->m_pStateMachine->GoSquareLoading();
}
