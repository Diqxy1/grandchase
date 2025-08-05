#include "stdafx.h"
#include "GCRoomMatchPanelS6.h"

IMPLEMENT_CLASSNAME( KGCRoomMatchPanelS6 );
IMPLEMENT_WND_FACTORY( KGCRoomMatchPanelS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomMatchPanelS6, "gc_room_match_panel_s6" );

KGCRoomMatchPanelS6::KGCRoomMatchPanelS6( void )
: m_pkTeamBox( NULL )
, m_pkMatchInfo(NULL)
, m_pkMatchMapBox(NULL)
{
    LINK_CONTROL( "room_team_box",              m_pkTeamBox );
    LINK_CONTROL( "room_match_info",            m_pkMatchInfo );
    LINK_CONTROL( "room_map_select",            m_pkMatchMapBox );
}

KGCRoomMatchPanelS6::~KGCRoomMatchPanelS6( void )
{
}

void KGCRoomMatchPanelS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCRoomMatchPanelS6::OnCreate( void )
{

}

void KGCRoomMatchPanelS6::UpdateGameData()
{
    m_pkTeamBox->UpdateGameData();
    m_pkMatchInfo->UpdateGameData();
    m_pkMatchMapBox->UpdateGameData();
}