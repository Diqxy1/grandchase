#include "stdafx.h"
#include "KGCGuildMatchRoomBar.h"


IMPLEMENT_CLASSNAME( KGCGuildMatchRoomBar );
IMPLEMENT_WND_FACTORY( KGCGuildMatchRoomBar );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMatchRoomBar, "gc_guildmatch_room_bar" );

KGCGuildMatchRoomBar::KGCGuildMatchRoomBar(void)
: m_pkGuildVs( NULL )
, m_pkGuildMarkLeftTeam(NULL)
, m_pkGuildMarkRightTeam(NULL)
{
    LINK_CONTROL( "guild_room_vs", m_pkGuildVs );

    for(int i = 0 ; i < GMTT_MAX; ++i)
    {
        m_pkGuildMarkTexureBack[i] = NULL;

        char strTemp[ MAX_PATH ] = {0, };
        sprintf_s( strTemp, MAX_PATH, "guild_mark_back%d", i);
    }
	LINK_CONTROL( "guild_mark_left_team", m_pkGuildMarkLeftTeam );
	LINK_CONTROL( "guild_mark_right_team", m_pkGuildMarkRightTeam );
} 

KGCGuildMatchRoomBar::~KGCGuildMatchRoomBar(void)
{
}

void KGCGuildMatchRoomBar::OnCreate()
{
    KGCRoomBar::OnCreate();
    m_pkGuildVs->InitState( true );   
	m_pkGuildMarkLeftTeam->InitState(true,true,this);
	m_pkGuildMarkRightTeam->InitState(true,true,this);
}


void KGCGuildMatchRoomBar::SetGuildMarkTexture(std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_ )
{
	SiKGCIconImageManager()->LoadIconImageFromHTTP(prGuildMarkName_.first);	
	SiKGCIconImageManager()->LoadIconImageFromHTTP(prGuildMarkName_.second);	

	m_pkGuildMarkLeftTeam->SetTextureName(prGuildMarkName_.first);
	m_pkGuildMarkRightTeam->SetTextureName(prGuildMarkName_.second);
	m_pkGuildMarkLeftTeam->SetToolTip(prGuildName_.first);
	m_pkGuildMarkRightTeam->SetToolTip(prGuildName_.second);
}

void KGCGuildMatchRoomBar::PostChildDraw( void )
{
}

void KGCGuildMatchRoomBar::SetRoomInfo( KSimpleRoomInfo kRoomInfo )
{
	KGCRoomBar::SetRoomInfo( kRoomInfo );
	SetGuildMarkTexture(kRoomInfo.m_pairGuildMarkName, kRoomInfo.m_pairGuildName);
}

