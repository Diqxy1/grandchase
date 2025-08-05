#include "stdafx.h"
#include "KGCMonsterHuntPanel.h"
//
#include "GCChangeBox.h"
//
#include "KGCRoomManager.h"

//
#include "MyD3D.h"
#include "KGCUIHelper.h"

IMPLEMENT_CLASSNAME( KGCMonsterHuntPanel );
IMPLEMENT_WND_FACTORY( KGCMonsterHuntPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterHuntPanel, "gc_monster_hunt_panel" );

KGCMonsterHuntPanel::KGCMonsterHuntPanel(void)
{
    m_pkLevelChangeBox = NULL;

    LINK_CONTROL( "level_change_box", m_pkLevelChangeBox );
}

KGCMonsterHuntPanel::~KGCMonsterHuntPanel(void)
{
}

void KGCMonsterHuntPanel::OnCreate( void )
{
    m_pkLevelChangeBox->InitState( true, true, this );
}

void KGCMonsterHuntPanel::ActionPerformed( const KActionEvent& event )
{
    if( g_kGlobalValue.m_kUserInfo.bHost ||
        g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) // 방장인 경우만 조작 가능
    {
#if !defined( NATION_USA ) && !defined( NATION_EU ) && !defined( NATION_KOREA )
        if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
        {
            // 방장인 내가 아닌 옵저버가 있다면 방장 기능을 상실한다.
            if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
                return;
        }        
#endif

        if( event.m_pWnd == m_pkLevelChangeBox )
        {
            if( g_pkUIHelper->GetUILockTime( LOCK_ROOM ) == 0 )
            {
                switch ( event.m_dwCode )
                {
                    case KGCChangeBox::RIGHT_BUTTON: // 오른쪽 버튼을 클릭했을 때
                    {
                        ChangeLevel( true );

                        g_KDSound.Play( "31" );
                        break;
                    }
                    case KGCChangeBox::LEFT_BUTTON: // 왼쪽 버튼을 클릭했을 때
                    {
                        ChangeLevel( false );

                        g_KDSound.Play( "31" );
                        break;
                    }
                }
            }
        }
    }
}

void KGCMonsterHuntPanel::UpdateGameData()
{
    int iLevel = SiKGCRoomManager()->GetDungeonLevel() + 1;

    WCHAR str[MAX_PATH];
    swprintf( str, MAX_PATH, L"%d %s", iLevel, g_pkStrLoader->GetString( STR_ID_MON_HUNT_LEVEL ).c_str() );
    m_pkLevelChangeBox->SetText( str );

    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
        m_pkLevelChangeBox->EnableChangeBox( g_MyD3D->IsLocalPlayerObserver() );
    else
        m_pkLevelChangeBox->EnableChangeBox( g_kGlobalValue.m_kUserInfo.bHost );
}

void KGCMonsterHuntPanel::ChangeLevel( bool bRight )
{
    int iMaxLevel = (int)g_kGlobalValue.m_mapMonsterHuntInfo.size();
    int iLevel = SiKGCRoomManager()->GetDungeonLevel();

    if( bRight )
    {
        ++iLevel;
        if( iLevel >= iMaxLevel )
            iLevel = 0;
    }
    else if( !bRight )
    {
        --iLevel;
        if( iLevel < 0 )
            iLevel = iMaxLevel-1;
    }

    SiKGCRoomManager()->SetDungeonLevel( iLevel );

    SiKGCRoomManager()->SendRoomInfo();
    UpdateGameData();
    g_pkUIScene->RoomButtonSetStartDelay( 219 );

    g_pkUIHelper->SetUILockTime( LOCK_ROOM, BUTTON_LOCK_FRAME );
}

