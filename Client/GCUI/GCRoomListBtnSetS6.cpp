#include "stdafx.h"
#include "GCRoomListBtnSetS6.h"
#include "KGCWorldMapManager.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME( KGCRoomListBtnSetS6 );
IMPLEMENT_WND_FACTORY( KGCRoomListBtnSetS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomListBtnSetS6, "gc_room_list_dungeon_info_btn_set" );

KGCRoomListBtnSetS6::KGCRoomListBtnSetS6( void )
: m_pkHellLevelBtn(NULL)
, m_pkHellLevelClose(NULL)
, m_pkEventHellLevelBtn(NULL)
, m_pkEventHellLevelClose(NULL)
, m_pkBreakLevelBtn(NULL)
, m_pkBreakLevelClose(NULL)
, m_pkEventBreakLevelBtn(NULL)
, m_pkEventBreakLevelClose(NULL)
, m_pkCheck(NULL)
{
	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
		sprintf(szTemp, "dungeon_level_btn_%d", i + 1);
		m_saLevelBtn[i] = NULL;
		LINK_CONTROL(szTemp,m_saLevelBtn[i]);
	}

	LINK_CONTROL("dungeon_level_btn_hell", m_pkHellLevelBtn);
	LINK_CONTROL("dungeon_level_btn_event_hell", m_pkEventHellLevelBtn);

	LINK_CONTROL("dungeon_level_btn_time", m_pkBreakLevelBtn);
	LINK_CONTROL("dungeon_level_btn_event_time", m_pkEventBreakLevelBtn);


	for ( int i = 0 ; i < KDSI_CLOSE_LEVEL_NUM ; i++ ) {
		sprintf(szTemp, "close_wnd_%d", i + 2);
		m_saClose[i] = NULL;
		LINK_CONTROL(szTemp,m_saClose[i]);
	}

	LINK_CONTROL("close_wnd_hell", m_pkHellLevelClose);
	LINK_CONTROL("close_wnd_event_hell", m_pkEventHellLevelClose);

	LINK_CONTROL("close_wnd_time", m_pkBreakLevelClose);
	LINK_CONTROL("close_wnd_event_time", m_pkEventBreakLevelClose);

	LINK_CONTROL("check",m_pkCheck);

}
KGCRoomListBtnSetS6::~KGCRoomListBtnSetS6( void )
{
}

void KGCRoomListBtnSetS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCRoomListBtnSetS6::OnCreate( void )
{
	for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
		if( m_saLevelBtn[i] == NULL) continue;

		m_saLevelBtn[i]->InitState(true,true,this);
		m_saLevelBtn[i]->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListBtnSetS6::OnClickBtn);
	}

	if (m_pkHellLevelBtn) {
		m_pkHellLevelBtn->InitState(false, true, this);
		m_pkHellLevelBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListBtnSetS6::OnClickBtn);
	}
	if (m_pkEventHellLevelBtn) {
		m_pkEventHellLevelBtn->InitState(false, true, this);
		m_pkEventHellLevelBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListBtnSetS6::OnClickBtn);
	}

	if (m_pkBreakLevelBtn) {
		m_pkBreakLevelBtn->InitState(false, true, this);
		m_pkBreakLevelBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListBtnSetS6::OnClickBtn);
	}
	if (m_pkEventBreakLevelBtn) {
		m_pkEventBreakLevelBtn->InitState(false, true, this);
		m_pkEventBreakLevelBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListBtnSetS6::OnClickBtn);
	}


	for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM - 1 ; i++ ) {
		if( m_saClose[i] == NULL) continue;

		m_saClose[i]->InitState(false,false,NULL);
	}

	if (m_pkHellLevelClose) {
		m_pkHellLevelClose->InitState(false, false, NULL);
	}
	if (m_pkEventHellLevelClose) {
		m_pkEventHellLevelClose->InitState(false, false, NULL);
	}

	if (m_pkBreakLevelClose) {
		m_pkBreakLevelClose->InitState(false, false, NULL);
	}
	if (m_pkEventBreakLevelClose) {
		m_pkEventBreakLevelClose->InitState(false, false, NULL);
	}

	m_pkCheck->InitState(true, false, NULL);

	SetWndState();
	SetBtnState();

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL, this, &KGCRoomListBtnSetS6::SetBtnState );
}

void KGCRoomListBtnSetS6::OnClickBtn( const KActionEvent& event )
{
    //  룸안에서 동작
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM ) 
    {
		if (event.m_pWnd == m_pkBreakLevelBtn || event.m_pWnd == m_pkEventBreakLevelBtn) {
            g_pkUIScene->m_pkRoomSceneS6->ChangeCurrentLevel( GC_DUNGEON_LEVEL_BREAK );
        }
		else if (event.m_pWnd == m_pkHellLevelBtn || event.m_pWnd == m_pkEventHellLevelBtn) {
			g_pkUIScene->m_pkRoomSceneS6->ChangeCurrentLevel( GC_DUNGEON_LEVEL_HELL );
		}
        else{
            for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
                if ( m_saLevelBtn[i] == event.m_pWnd ) {
                    //선택한 난이도 정보를 세팅한다. 헬모드는 GC_DUNGEON_LEVEL_HELL로 세팅해 줍니다.
                    g_pkUIScene->m_pkRoomSceneS6->ChangeCurrentLevel( i );
                    break;
                }
            }
        }
    }
    else
    {
		if (event.m_pWnd == m_pkBreakLevelBtn || event.m_pWnd == m_pkEventBreakLevelBtn) {
		    SiKGCRoomListManager()->SetCurrentLevel( GC_DUNGEON_LEVEL_BREAK );	
	    }
		else if (event.m_pWnd == m_pkHellLevelBtn || event.m_pWnd == m_pkEventHellLevelBtn) {
			SiKGCRoomListManager()->SetCurrentLevel(GC_DUNGEON_LEVEL_HELL);
		}
	    else{
		    for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
			    if ( m_saLevelBtn[i] == event.m_pWnd ) {
				    //선택한 난이도 정보를 세팅한다. 헬모드는 GC_DUNGEON_LEVEL_HELL로 세팅해 줍니다.
				    SiKGCRoomListManager()->SetCurrentLevel( i );
				    break;
			    }
		    }
	    }
    }
}

void KGCRoomListBtnSetS6::SetBtnCheckMark( KD3DWnd *pkBaseWnd ) 
{
	float xpos = pkBaseWnd->GetFixedWindowLocalPos().x + 2.f;
	
	xpos += static_cast<float>(pkBaseWnd->GetWidth()) / 2.f - static_cast<float>(m_pkCheck->GetWidth()) / 2.f;

	m_pkCheck->SetWindowPosXDirect(xpos);
}

void KGCRoomListBtnSetS6::SetBtnState( void ) 
{
    int iLevel = SiKGCRoomListManager()->GetCurrentLevel();

    if ( g_MyD3D->m_pStateMachine->IsStateRoom() ) 
    {
        iLevel = SiKGCRoomManager()->GetDungeonLevel();
    }

    for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
	    if ( m_saLevelBtn[i] == NULL) continue;
	    m_saLevelBtn[i]->Lock( i == iLevel );
        
        if( SiKGCWorldMapManager()->GetDungeonType( SiKGCWorldMapManager()->GetGameMode() ) == DUNGEON_TYPE_RELAY_HERO )
            m_saLevelBtn[i]->Lock(true);

	    if( i == iLevel)
		    SetBtnCheckMark(m_saLevelBtn[i]);
    }

    if( SiKGCWorldMapManager()->IsEventDungeon( GC_GM_INVALID ) )
    {
        if ( m_pkEventHellLevelBtn ) {
            m_pkEventHellLevelBtn->Lock( iLevel == GC_DUNGEON_LEVEL_HELL );
            if( iLevel == GC_DUNGEON_LEVEL_HELL )
                SetBtnCheckMark(m_pkEventHellLevelBtn);
        }
		if ( m_pkEventBreakLevelBtn ) {
			m_pkEventBreakLevelBtn->Lock(iLevel == GC_DUNGEON_LEVEL_BREAK);
			if (iLevel == GC_DUNGEON_LEVEL_BREAK)
				SetBtnCheckMark(m_pkEventBreakLevelBtn);
		}
    }
    else
    {
		if ( m_pkHellLevelBtn ) {
			m_pkHellLevelBtn->Lock(iLevel == GC_DUNGEON_LEVEL_HELL);
			if (iLevel == GC_DUNGEON_LEVEL_HELL)
				SetBtnCheckMark(m_pkHellLevelBtn);
		}
		if ( m_pkBreakLevelBtn ) {
			m_pkBreakLevelBtn->Lock(iLevel == GC_DUNGEON_LEVEL_BREAK);
			if (iLevel == GC_DUNGEON_LEVEL_BREAK)
				SetBtnCheckMark(m_pkBreakLevelBtn);
		}
    }
}

void KGCRoomListBtnSetS6::SetWndState( void ) 
{
    if ( g_MyD3D->m_pStateMachine->IsStateRoom() ) 
    {
        //클리어한 던전 난이도 및 라스트 난이도 정보를 가져온다.

        std::map<int,KDungeonUserInfo> mapStagePlayable;

        EGCGameMode eGameMode = SiKGCRoomManager()->GetStagePlayable( mapStagePlayable );

        int nEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel(eGameMode, mapStagePlayable);

		bool bHell = (SiKGCRoomManager()->GetDungeonLevel() == GC_DUNGEON_LEVEL_HELL);
		bool bBreak = (SiKGCRoomManager()->GetDungeonLevel() >= GC_DUNGEON_LEVEL_BREAK);

	    for ( int i = 0 ; i < KDSI_CLOSE_LEVEL_NUM ; i++ ) {
		    if( m_saClose[i] == NULL) continue;
		    //입구를 제외한 자신이 깬 레벨의 상위레벨까지는 풀린다.
    		
		    m_saClose[i]->ToggleRender( nEnableLevel < ( i+1 ) && !bHell );
	    }

	    for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
		    if ( m_saLevelBtn[i] == NULL ) continue;

		    m_saLevelBtn[i]->ToggleRender( nEnableLevel >= i && !bHell );
	    }

	    if( m_pkHellLevelBtn )
			m_pkHellLevelBtn->ToggleRender( false );
	    if( m_pkHellLevelClose )
			m_pkHellLevelClose->ToggleRender( false );
	    if( m_pkEventHellLevelBtn )
			m_pkEventHellLevelBtn->ToggleRender( false );
	    if( m_pkEventHellLevelClose )
			m_pkEventHellLevelClose->ToggleRender( false );
		if( m_pkBreakLevelBtn )
			m_pkBreakLevelBtn->ToggleRender( false );
	    if( m_pkBreakLevelClose )
			m_pkBreakLevelClose->ToggleRender( false );
	    if( m_pkEventBreakLevelBtn )
			m_pkEventBreakLevelBtn->ToggleRender( false );
	    if( m_pkEventBreakLevelClose )
			m_pkEventBreakLevelClose->ToggleRender( false );

	    if( SiKGCWorldMapManager()->IsEventDungeon( GC_GM_INVALID ) )
	    {
	        if ( m_pkEventHellLevelBtn ) {
	            m_pkEventHellLevelBtn->ToggleRender( bHell );
	        }

			if ( m_pkEventBreakLevelBtn ) {
				m_pkEventBreakLevelBtn->ToggleRender( bBreak );
			}
	    }
	    else
	    {
	        if ( m_pkHellLevelBtn ) {
	            m_pkHellLevelBtn->ToggleRender( bHell );
	        }

			if ( m_pkBreakLevelBtn ) {
				m_pkBreakLevelBtn->ToggleRender( bBreak );
			}
	    }

    }
    else
    {
        //클리어한 던전 난이도 및 라스트 난이도 정보를 가져온다.
        std::pair<int, int> pairLevel = SiKGCWorldMapManager()->GetCurrentDungeonPlayLevelAndLastLevel();

        for ( int i = 0 ; i < KDSI_CLOSE_LEVEL_NUM ; i++ ) {
            if( m_saClose[i] == NULL) continue;
            //입구를 제외한 자신이 깬 레벨의 상위레벨까지는 풀린다.

            m_saClose[i]->ToggleRender( pairLevel.first < ( i+1 ) );
        }

        for ( int i = 0 ; i < KDSI_BTN_LEVEL_NUM ; i++ ) {
            if ( m_saLevelBtn[i] == NULL ) continue;

            m_saLevelBtn[i]->ToggleRender( pairLevel.first >= i );
        }

	    if( m_pkHellLevelBtn )
			m_pkHellLevelBtn->ToggleRender( false );
	    if( m_pkHellLevelClose )
			m_pkHellLevelClose->ToggleRender( false );
	    if( m_pkEventHellLevelBtn )
			m_pkEventHellLevelBtn->ToggleRender( false );
	    if( m_pkEventHellLevelClose )
			m_pkEventHellLevelClose->ToggleRender( false );
		if( m_pkBreakLevelBtn )
			m_pkBreakLevelBtn->ToggleRender( false );
	    if( m_pkBreakLevelClose )
			m_pkBreakLevelClose->ToggleRender( false );
	    if( m_pkEventBreakLevelBtn )
			m_pkEventBreakLevelBtn->ToggleRender( false );
	    if( m_pkEventBreakLevelClose )
			m_pkEventBreakLevelClose->ToggleRender( false );

	    if( SiKGCWorldMapManager()->IsEventDungeon( GC_GM_INVALID ) )
	    {
	        if ( m_pkEventHellLevelClose ) {
	            m_pkEventHellLevelClose->ToggleRender( SiKGCWorldMapManager()->IsOpenHellMode() != true );
	        }
	        if ( m_pkEventHellLevelBtn ) {
	            m_pkEventHellLevelBtn->ToggleRender( SiKGCWorldMapManager()->IsOpenHellMode() == true );
	        }

			if (m_pkEventBreakLevelClose) {
				m_pkEventBreakLevelClose->ToggleRender(SiKGCWorldMapManager()->IsOpenBreakMode() != true);
			}
			if (m_pkEventBreakLevelBtn) {
				m_pkEventBreakLevelBtn->ToggleRender(SiKGCWorldMapManager()->IsOpenBreakMode() == true);
			}
	    }
	    else
	    {
	        if ( m_pkHellLevelClose ) {
	            m_pkHellLevelClose->ToggleRender( SiKGCWorldMapManager()->IsOpenHellMode() != true );
	        }
	        if ( m_pkHellLevelBtn ) {
	            m_pkHellLevelBtn->ToggleRender( SiKGCWorldMapManager()->IsOpenHellMode() == true );
	        }

			if (m_pkBreakLevelClose) {
				m_pkBreakLevelClose->ToggleRender(SiKGCWorldMapManager()->IsOpenBreakMode() != true);
			}
			if (m_pkBreakLevelBtn) {
				m_pkBreakLevelBtn->ToggleRender(SiKGCWorldMapManager()->IsOpenBreakMode() == true);
			}
	    }
    }
}
