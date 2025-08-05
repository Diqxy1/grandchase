#include "stdafx.h"
#include "KGCDeathMatchScoreBoard.h"
#include "KGCDeathMatchCharInfoBar.h"
//
//
#include "GCStateMachine.h"
#include "GCStateRoom.h"
#include "KGCChannelManager.h"
#include "MyD3D.h"
#include "KGCRoomManager.h"
#include "KGCGameBoard.h"


IMPLEMENT_CLASSNAME( KGCDeathMatchScoreBoard );
IMPLEMENT_WND_FACTORY( KGCDeathMatchScoreBoard );
IMPLEMENT_WND_FACTORY_NAME( KGCDeathMatchScoreBoard, "gc_deathmatch_scoreboard" );

KGCDeathMatchScoreBoard::KGCDeathMatchScoreBoard(void)
{
    m_pkStaticServer        = NULL;
    m_pkStaticChannel       = NULL;
    m_pkStaticRoomName      = NULL;
    m_pkStaticOpenClose     = NULL;

    m_pkTextList            = NULL;
    m_pkTeamComponent       = NULL;
    m_pkPlayerListTeam      = NULL;

    m_pkScorePosHelperSerdin = NULL;
    m_pkScorePosHelperCanaban = NULL;

    m_pkDeathMatchTitle = NULL;
    m_pkFatalDeathMatchTitle = NULL;



    m_pkBackPan             = NULL;



    for ( int i = 0; i < CHAR_INFO_BAR_NUM; i++ )
    {
        m_apkCharInfo[i] = NULL;
    }

    LINK_CONTROL( "static_server_name",  m_pkStaticServer     );     
    LINK_CONTROL( "static_room_name",    m_pkStaticRoomName   );
    LINK_CONTROL( "static_channel_name", m_pkStaticChannel    );
    
    LINK_CONTROL( "property_set",        m_pkTextList           );
    LINK_CONTROL( "team_component",      m_pkTeamComponent       );
    LINK_CONTROL( "team_back_set",       m_pkPlayerListTeam       );

    LINK_CONTROL( "serdin_point_pos",    m_pkScorePosHelperSerdin       );
    LINK_CONTROL( "canaban_point_pos",   m_pkScorePosHelperCanaban      );
    
    LINK_CONTROL( "static_open_close",   m_pkStaticOpenClose      );

    LINK_CONTROL( "title",   m_pkDeathMatchTitle );
    LINK_CONTROL( "title2",  m_pkFatalDeathMatchTitle );

    

    LINK_CONTROL( "back_frame",         m_pkBackPan       );

    for (int i = 0 ; i < CHAR_INFO_BAR_NUM; i++ )
    {
        std::stringstream stm;
        stm << "charinfo" << i+1;
        LINK_CONTROL( stm.str().c_str(), m_apkCharInfo[i] );
        
    }
}

KGCDeathMatchScoreBoard::~KGCDeathMatchScoreBoard(void)
{
}

static bool IsFirstLoad = true;
void KGCDeathMatchScoreBoard::OnCreate()
{
    //m_pkStaticGrade->SetText(   g_pkStrLoader->GetString( STR_ID_GRADE )    );   
    //m_pkStaticGuild->SetText(   g_pkStrLoader->GetString( STR_ID_GUILD )    );   
    //m_pkStaticNickName->SetText(g_pkStrLoader->GetString( STR_ID_NICKNAME ) ); 
    //m_pkStaticDamage->SetText(  g_pkStrLoader->GetString( STR_ID_DAMAGE )   ); 
    //m_pkStaticKill->SetText(    g_pkStrLoader->GetString( STR_ID_KILL )     );     
    //m_pkStaticDeath->SetText(   g_pkStrLoader->GetString( STR_ID_DEATH )    );    
    //m_pkStaticPing->SetText(    g_pkStrLoader->GetString( STR_ID_PING )     );    
 
    for (int i = 0 ; i < CHAR_INFO_BAR_NUM; i++ )
    {
        m_apkCharInfo[i]->m_iPlayerIndex = i;
    }

    if ( IsFirstLoad )
    {
        IsFirstLoad = false;
        m_vPlayerListPos = m_pkPlayerListTeam->GetFixedWindowLocalPos();
        m_vTextListPos = m_pkTextList->GetFixedWindowLocalPos();
        m_fCharInfoPosX = m_apkCharInfo[0]->GetFixedWindowLocalPos().x;
    }
    m_pkStaticOpenClose->InitState( true );
    m_pkStaticOpenClose->SetText( g_pkStrLoader->GetString( STR_ID_DEATHMATCH_BOARD_OPEN_CLOSE ) );

    m_pkDeathMatchTitle->InitState( false, false, false );
    m_pkFatalDeathMatchTitle->InitState( false, false, false );

    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH )
    {
        m_pkDeathMatchTitle->ToggleRender( true );
    }
    else
    {
        m_pkFatalDeathMatchTitle->ToggleRender( true );
    }

}


void KGCDeathMatchScoreBoard::UpdateGameData()
{    
    if ( IsFirstLoad )
    {
        return;
    }
    
    m_pkStaticServer->SetText( g_kGlobalValue.ServerInfo.strServerName  );
    KGCStateRoom* pRoom = (KGCStateRoom*) g_MyD3D->m_pStateMachine->GetStatePtr( GS_ROOM );
    m_pkStaticRoomName->SetText( pRoom->GetRoomName() );
    m_pkStaticChannel->SetText(  SiKGCChannelManager()->GetCurrentChannelName() );

    for ( int i = 0; i < CHAR_INFO_BAR_NUM; i++ )
    {
        m_apkCharInfo[i]->UpdateGameData();
    }
    D3DXVECTOR2 vPlayerListPos = m_vPlayerListPos;
    D3DXVECTOR2 vTextListPos = m_vTextListPos;
    float fCharInfoPosX = m_fCharInfoPosX;

    if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_SURVIVAL ||
        SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_SURVIVAL )
    {
        m_pkTeamComponent->ToggleRender( false );
        vPlayerListPos.x += 50.0f;
        vTextListPos.x += 50.0f;
        fCharInfoPosX += 50.0f;
    }
    else
        m_pkTeamComponent->ToggleRender( true);

    for ( int i = 0; i < CHAR_INFO_BAR_NUM; i++ )
    {
        D3DXVECTOR2 vPos = m_apkCharInfo[i]->GetFixedWindowLocalPos();
        vPos.x = fCharInfoPosX;
        m_apkCharInfo[i]->SetAllWindowPos( vPos );

    }

    m_pkPlayerListTeam->SetAllWindowPos( vPlayerListPos );
    m_pkTextList->SetAllWindowPos( vTextListPos );
}

void KGCDeathMatchScoreBoard::PostChildDraw( void )
{
    if( SiKGCRoomManager()->GetGameMode() != GC_GM_DEATH_TEAM &&
        SiKGCRoomManager()->GetGameMode() != GC_GM_FATAL_DEATH_TEAM )
        return;
	
    g_MyD3D->m_pkUIMgr->RenderNumber( "orange_gradation_num", m_pkScorePosHelperSerdin->GetRelocatedWindowPos(), (DWORD)g_pGameBoard->GetTeamScore( 0 ), D3DXCOLOR(0xffffffff) , false, true, -1.0f,true);
    g_MyD3D->m_pkUIMgr->RenderNumber( "orange_gradation_num", m_pkScorePosHelperCanaban->GetRelocatedWindowPos(), (DWORD)g_pGameBoard->GetTeamScore( 1 ), D3DXCOLOR(0xffffffff) , false, true, -1.0f,true);
}
