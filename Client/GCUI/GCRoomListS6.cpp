#include "stdafx.h"
#include "GCRoomListS6.h"
#include "KGCRoomManager.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME( KGCRoomListS6 );
IMPLEMENT_WND_FACTORY( KGCRoomListS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomListS6, "gc_room_list_s6" );

KGCRoomListS6::KGCRoomListS6( void )
: m_pkPrePageBtn(NULL)
, m_pkNextPageBtn(NULL)
, m_pkAllBtn(NULL)
, m_pkWatingBtn(NULL)
, m_iRoomListType(0)
, m_pkRoomFilterBox(NULL)
, m_pkCategoryTitle(NULL)
, m_pkFilterBtn(NULL)
, m_pkFilterOnBtn(NULL)
{	
	LINK_CONTROL("pre_btn", m_pkPrePageBtn);	
	LINK_CONTROL("next_btn", m_pkNextPageBtn);	
	LINK_CONTROL("all_btn", m_pkAllBtn);	
	LINK_CONTROL("wait_btn", m_pkWatingBtn);	
	LINK_CONTROL( "room_filter_box",    m_pkRoomFilterBox );
	LINK_CONTROL( "current_category",    m_pkCategoryTitle );
	

	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for ( int i = 0 ; i < KRL_LEVEL_MAX_NUM ; i++ ) {
		sprintf(szTemp, "title_%d", i);
		m_saLevelTitle[i] = NULL;
		LINK_CONTROL(szTemp,m_saLevelTitle[i]);
	}

	LINK_CONTROL("filter_btn", m_pkFilterBtn);
	LINK_CONTROL("filter_on_btn", m_pkFilterOnBtn);
}

KGCRoomListS6::~KGCRoomListS6( void )
{
}

void KGCRoomListS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkRoomFilterBox, KD3DWnd::D3DWE_BUTTON_CLICK, OnRoomFilterBox );

}

void KGCRoomListS6::OnCreate( void )
{
	m_pkPrePageBtn->InitState(true,true,this);
	m_pkNextPageBtn->InitState(true,true,this);

	bool bWaitRoom = SiKGCRoomListManager()->GetWaitRoom();
	m_pkAllBtn->InitState(bWaitRoom,true,this);
	m_pkWatingBtn->InitState(!bWaitRoom,true,this);

	if ( m_pkRoomFilterBox != NULL ) {
		m_pkRoomFilterBox->InitState(true,true,this);
	}

	if ( m_pkCategoryTitle != NULL )
		m_pkCategoryTitle->InitState(true,false,NULL);

	if ( CreateRoomListUI() ) {
		SetRoomListPos();
		MoveWndOnTop(m_pkRoomFilterBox);
	}

	for ( std::vector<std::pair<DWORD,KGCRoomBtnS6*>>::iterator vit = m_vecRoomList.begin() ; vit != m_vecRoomList.end() ; vit++ ) {
		vit->second->InitState(false,true,this);
		vit->second->DisconnectAllEventProcedure();
		vit->second->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnClickRoomListBar);
	}

	if (m_pkFilterBtn && m_pkFilterOnBtn)
	{
		KRoomOptions options;
		bool bFilterOn = !(options == SiKGCRoomListManager()->GetRoomListFilterOptions());

		m_pkFilterBtn->InitState(!bFilterOn, true, this);
		m_pkFilterBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnFilterBtnClick);
		m_pkFilterOnBtn->InitState(bFilterOn, true, this);
		m_pkFilterOnBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnFilterBtnClick);
	}

	InitRoomList();
	
	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_UPDATE_ROOM_LIST);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_UPDATE_ROOM_LIST, this, &KGCRoomListS6::UpdateRoomList );

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL, this, &KGCRoomListS6::UpdateLevelTitle );


	m_pkPrePageBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnPrevPage );
	m_pkNextPageBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnNextPage );
	m_pkAllBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnAllRoom );
	m_pkWatingBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnWaitRoom );

	Update();

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_SELECT);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_SELECT, this, &KGCRoomListS6::Update);


}

void KGCRoomListS6::Update( void )
{
	SiKGCRoomListManager()->SetCurrentPage( 1 );
	UpdateLevelTitle();

}

void KGCRoomListS6::InitRoomList( void )
{
	for ( std::vector<std::pair<DWORD,KGCRoomBtnS6*>>::iterator vit = m_vecRoomList.begin() ; vit != m_vecRoomList.end() ; vit++ ) {
		vit->first = 0;
		(vit->second)->InitState( m_iRoomListType == KRL_ROOM_LIST_TYPE_DUNGEON ? true:false,true,this);
		(vit->second)->Lock(true);
	}
}

void KGCRoomListS6::RenderOffRoomList( void )
{
	for (  std::vector<std::pair<DWORD,KGCRoomBtnS6*>>::iterator vit = m_vecRoomList.begin() ; vit != m_vecRoomList.end() ; vit++ ) {
		vit->first = 0;
		vit->second->ClearBarInfo();
		if ( m_iRoomListType == KRL_ROOM_LIST_TYPE_DUNGEON ) {
			vit->second->Lock(true);
		}
		else if ( m_iRoomListType == KRL_ROOM_LIST_TYPE_MATCH ) {
			vit->second->ToggleRender(false);
		}
	}
}

void KGCRoomListS6::UpdateLevelTitle( void )
{
    EGCGameMode eMode = SiKGCWorldMapManager()->GetGameMode();
    if( SiKGCWorldMapManager()->IsHeroDungeon( eMode ) ||
        SiKGCWorldMapManager()->IsRelayHeroDungeon( eMode ) ||
        SiKGCWorldMapManager()->IsRaidHeroDungeon( eMode ) ) { 
        for( int i=0 ; i<KRL_LEVEL_MAX_NUM ; ++i ) { 
            if( m_saLevelTitle[i] ) { 
                m_saLevelTitle[i]->ToggleRender( false );
            }
        }

        return;
    }

	int iLevelIndex = SiKGCRoomListManager()->GetCurrentLevel();
	for ( int i = 0 ; i < KRL_LEVEL_MAX_NUM ; i++ ) {
        if( m_saLevelTitle[i] )
		    m_saLevelTitle[i]->ToggleRender( i == iLevelIndex);
	}

	if ( iLevelIndex == KGCRoomListManager::KRLM_LEVEL_HELL ) {
        if( SiKGCWorldMapManager()->IsEventDungeon( GC_GM_INVALID ) )
        {
            if( m_saLevelTitle[4] )
                m_saLevelTitle[4]->ToggleRender(true);
        }
        else
        {
            if( m_saLevelTitle[3] )
		        m_saLevelTitle[3]->ToggleRender(true);
        }
	}

	if (iLevelIndex == KGCRoomListManager::KRLM_LEVEL_BREAK) {
		if (SiKGCWorldMapManager()->IsEventDungeon(GC_GM_INVALID))
		{
			if (m_saLevelTitle[6])
				m_saLevelTitle[6]->ToggleRender(true);
		}
		else
		{
			if (m_saLevelTitle[5])
				m_saLevelTitle[5]->ToggleRender(true);
		}
	}

}


void KGCRoomListS6::UpdateRoomList( void )
{
	RenderOffRoomList();
	
	//룸리스트 세팅 합니당~!
	std::vector<KSimpleRoomInfo>& vecRoomList = SiKGCRoomListManager()->GetRoomList();

	int i = 0;
	for( std::vector<KSimpleRoomInfo>::iterator vit = vecRoomList.begin() ; vit !=  vecRoomList.end() ; vit++,i++ ) {
		if(i >= (m_iRoomListType == KRL_ROOM_LIST_TYPE_DUNGEON ? KRL_ROOM_MAX_NUM : KRL_MATCH_ROOM_MAX_NUM ) ) break;

		m_vecRoomList[i].second->Lock(false);
		m_vecRoomList[i].first = vit->m_usRoomID;

		m_vecRoomList[i].second->SetRoomBtnInfo( *vit );
		m_vecRoomList[i].second->ToggleRender(true);
	}

	if ( m_pkRoomFilterBox != NULL )
		m_pkRoomFilterBox->Update();

	if ( m_pkCategoryTitle != NULL )
		m_pkCategoryTitle->UpdateCategory(static_cast<char>(SiKGCRoomListManager()->GetRoomFilter()));

}


void KGCRoomListS6::SetRoomListPos( void )
{
	int i = 0;

	float fGapY = m_iRoomListType == KRL_ROOM_LIST_TYPE_DUNGEON ? 34.f : 35.f;

	float fStartPosY = 0.f;

	if ( m_vecRoomList.begin() != m_vecRoomList.end() ) {
		fStartPosY = m_vecRoomList[0].second->GetFixedWindowLocalPos().y;
	}

	for (  std::vector<std::pair<DWORD,KGCRoomBtnS6*>>::iterator vit = m_vecRoomList.begin() ; vit != m_vecRoomList.end() ; vit++, i++ ) {
		(vit->second)->SetWindowPosY( fStartPosY + ( fGapY * i ));
	}
}

bool KGCRoomListS6::CreateRoomListUI( void )
{
	if(m_vecRoomList.empty() == false) return false;

	std::string strUIFile;
	std::string strUIName;
	int iRoomListNum = 0;

	if (m_iRoomListType == KRL_ROOM_LIST_TYPE_DUNGEON) {
		strUIFile = "ui_dungeon_room_list_dlg_s6.stg";
		strUIName = "dungeon_room_list_dlg_s6\\room_list_s6\\room_btn";
		iRoomListNum = KRL_ROOM_MAX_NUM;
	}
	else {
		strUIFile = "ui_scene_match_s6.stg";
		strUIName = "match_scene_s6\\match_window_s6\\room_list_s6\\room_btn";
		iRoomListNum = KRL_MATCH_ROOM_MAX_NUM;
	}

	std::vector<KD3DWnd*> &vecRoomList =  g_pkUIMgr->CreateTemplateUIListByFindName(this, strUIFile.c_str(), "",strUIName.c_str(),true, iRoomListNum );

	for ( std::vector<KD3DWnd*>::iterator vit = vecRoomList.begin() ; vit != vecRoomList.end() ; vit++ ) {
		m_vecRoomList.push_back( std::pair<DWORD,KGCRoomBtnS6*>(0,static_cast<KGCRoomBtnS6*>(*vit)));
		(*vit)->InitState(false,true,this);
		static_cast<KGCRoomBtnS6*>(*vit)->InitWnd();

		(*vit)->DisconnectAllEventProcedure();
		(*vit)->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListS6::OnClickRoomListBar);
	}
	
	return true;
}

void KGCRoomListS6::OnPrevPage()
{
	SiKGCRoomListManager()->SetCurrentPage( SiKGCRoomListManager()->GetCurrentPage() - 1 );
}

void KGCRoomListS6::OnNextPage()
{
	SiKGCRoomListManager()->SetCurrentPage( SiKGCRoomListManager()->GetCurrentPage() + 1 );
}

void KGCRoomListS6::FrameMoveInEnabledState()
{
	SiKGCRoomListManager()->SendRoomListReq();
}

void KGCRoomListS6::OnAllRoom()
{
	m_pkWatingBtn->ToggleRender( true );
	m_pkAllBtn->ToggleRender( false );
	SiKGCRoomListManager()->SetWaitRoom( false );
}

void KGCRoomListS6::OnWaitRoom()
{
	m_pkWatingBtn->ToggleRender( false );
	m_pkAllBtn->ToggleRender( true );
	SiKGCRoomListManager()->SetWaitRoom( true );
}

void KGCRoomListS6::OnClickRoomListBar(const KActionEvent& event)
{
	for (  std::vector<std::pair<DWORD,KGCRoomBtnS6*>>::iterator vit = m_vecRoomList.begin() ; vit != m_vecRoomList.end() ; vit++ ) {
		if(vit->second == event.m_pWnd){
			ClickRoomListBar( vit->second );
		}
	}
}

void KGCRoomListS6::ClickRoomListBar( KGCRoomBtnS6* pWnd )
{
	//Joinroom 부분 작업 합니다.
	KSimpleRoomInfo kRoomInfo = pWnd->GetRoomInfo();


	// 입장 티켓조건이 걸린 던전인지 확인
	if( g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP )
	{
		if( KGCRoomManager::ENTER_IMPOSSIBLE == SiKGCRoomManager()->CheckTicketToDungeonEnter( true, kRoomInfo.m_nGameMode ) )
			return;
	}

	if( (EGCGameModeCategory)kRoomInfo.m_cGameCategory == GC_GMC_DEATHMATCH || 
        (EGCGameModeCategory)kRoomInfo.m_cGameCategory == GC_GMC_FATAL_DEATHMATCH )
    {   
        if( kRoomInfo.m_bPlaying )
	    {
		    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_ROOM1 ),
			    g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ENTER_ROOM2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		    return;
	    }
    }

	if ( !SiKGCWorldMapManager()->IsEnableJoinRoomCheck( static_cast<EGCGameMode>(kRoomInfo.m_nGameMode), kRoomInfo.m_nDifficulty, kRoomInfo.m_bClearLastDifficult ) ) 
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_HERO_DUNGEON_DIFF_LEVEL ),
			L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}


	SiKGCRoomManager()->GetRoomInfo().usRoomID = kRoomInfo.m_usRoomID;


	if( kRoomInfo.m_bPublic )
	{
		SiKGCRoomManager()->JoinRoom( KJoinRoomReqInfo::JOIN_NORMAL, 
			SiKGCRoomManager()->GetRoomInfo().usRoomID,
			L"", SiKGCRoomManager()->IsChannelRoomList() );
	}
	else
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PASSWORD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}


}

void KGCRoomListS6::OnRoomFilterBox()
{
	if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) && m_pkRoomFilterBox )
		m_pkRoomFilterBox->ShowList();
}

void KGCRoomListS6::OnFilterBtnClick()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_LIST_FILTER_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}