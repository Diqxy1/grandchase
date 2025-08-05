#include "stdafx.h"
#include "GCSceneMenuS6.h"
#include "KGCShortMenuDlg.h"


IMPLEMENT_CLASSNAME( KGCSceneMenuBtnS6 );
IMPLEMENT_WND_FACTORY( KGCSceneMenuBtnS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCSceneMenuBtnS6, "gc_menubtn_list_s6" );

KGCSceneMenuBtnS6::KGCSceneMenuBtnS6( void )
: m_pkBack(NULL)
, m_pkLine(NULL)
, m_tBtnListActiveTime(0)
{
	LINK_CONTROL ("background",m_pkBack);
	LINK_CONTROL ("line",m_pkLine);

	RegisterFunc();
	LoadLuaTable();

}

KGCSceneMenuBtnS6::~KGCSceneMenuBtnS6( void )
{
}

void KGCSceneMenuBtnS6::RegisterFunc()
{
	m_mapEvent.clear();

	m_mapEvent["character_btn"] = &KGCSceneMenuBtnS6::OnClickCharSelectScene;
	m_mapEvent["mission_btn"] = &KGCSceneMenuBtnS6::OnClickMission;
	m_mapEvent["forge_btn"] = &KGCSceneMenuBtnS6::OnClickCreateBreadkup;
#ifndef DISABLE_HERO_SHOP_ICON
	m_mapEvent["hero_btn"] = &KGCSceneMenuBtnS6::OnClickHeroShop;
#endif
	m_mapEvent["rec_btn"] = &KGCSceneMenuBtnS6::OnClickReplayRecord;

	m_mapEvent["couple_btn"] = &KGCSceneMenuBtnS6::OnClickCouple;
	m_mapEvent["gacha_btn"] = &KGCSceneMenuBtnS6::OnClickGacha;
	m_mapEvent["square_btn"] = &KGCSceneMenuBtnS6::OnClickSquare;
	m_mapEvent["calendar_btn"] = &KGCSceneMenuBtnS6::OnClickCalendar;
	m_mapEvent["msn_btn"] = &KGCSceneMenuBtnS6::OnClickMSN;
	m_mapEvent["option_btn"] = &KGCSceneMenuBtnS6::OnClickOption;
	m_mapEvent["enchant_btn"] = &KGCSceneMenuBtnS6::OnClickEnchant;
	m_mapEvent["card_btn"] = &KGCSceneMenuBtnS6::OnClickMonster;
	m_mapEvent["event_btn"] = &KGCSceneMenuBtnS6::OnClickEventMiniGameBanner;
	m_mapEvent["depot_btn"] = &KGCSceneMenuBtnS6::OnClickDepot;
	m_mapEvent["channel_btn"] = &KGCSceneMenuBtnS6::OnClickServerScene;
#ifdef USE_VIP_MENU_BTN // Kawan>
	m_mapEvent["vip_btn"] = &KGCSceneMenuBtnS6::OnClickVipReward;
#endif
	m_mapEvent["skilltree_btn"] = &KGCSceneMenuBtnS6::OnClickSkillTree;
	m_mapEvent["agit_btn"] = &KGCSceneMenuBtnS6::OnClickAgit;
	m_mapEvent["post_btn"] = &KGCSceneMenuBtnS6::OnClickPost;
	m_mapEvent["guild_btn"] = &KGCSceneMenuBtnS6::OnClickGuild;
	m_mapEvent["enter_guild_btn"] = &KGCSceneMenuBtnS6::OnClickEnterGuild;
	m_mapEvent["rep_btn"] = &KGCSceneMenuBtnS6::OnClickReplay;
    m_mapEvent["royal_vip_btn"] = &KGCSceneMenuBtnS6::OnClickRoyalVip;
	m_mapEvent["guide_btn"] = &KGCSceneMenuBtnS6::OnClickGuide;
}

void KGCSceneMenuBtnS6::LoadLuaTable()
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return;

	if( GCFUNC::LoadLuaScript( luaMgr, "GCMenuBtnInfoS6.lua" ) == false )
		return;

	m_mapBtnInfo.clear();

	LUA_BEGIN_TABLE("MenuBtnInfo", return; ){
		for ( int i = 1 ; ; i++ )
		{
			MenuBtnInfo sTemp;

			LUA_BEGIN_TABLE(i, break;){
				LUA_GET_VALUE_NOASSERT("NUM",sTemp.iNum, LUA_END_TABLE(); break;);
				LUA_GET_VALUE_NOASSERT("ToolTip",sTemp.iToolTip, LUA_END_TABLE(); break;);
				LUA_GET_VALUE_NOASSERT("WindowName", sTemp.strUIName , LUA_END_TABLE(); break;);

			}LUA_END_TABLE(break;);

			m_mapBtnInfo.insert(std::make_pair( sTemp.iNum, sTemp ));
		}

	}LUA_END_TABLE(return;);
}

void KGCSceneMenuBtnS6::CreateBtnList( void )
{
    for ( std::map<int, MenuBtnInfo>::iterator mit = m_mapBtnInfo.begin() ; mit != m_mapBtnInfo.end() ; ++mit ) {
		std::map<int, KD3DWnd*>::iterator mitTemp = m_mapUIBtnList.find( mit->first );
		if( mitTemp == m_mapUIBtnList.end() ) {
            
			KD3DWnd* pWnd = NULL;
			std::string strWndName = "scene_menu_btn_6\\menu_btn_list\\";
			strWndName += mit->second.strUIName;

			pWnd = g_pkUIMgr->CreateTemplateUIByFindName( this, "ui_scene_common.stg", mit->second.strUIName.c_str(), strWndName.c_str() );
			pWnd->SetWindowName(mit->second.strUIName);
			pWnd->InitState( false, true, this );
			if ( mit->second.iToolTip >= 0 )
				pWnd->SetToolTip( g_pkStrLoader->GetString(mit->second.iToolTip));
			m_mapUIBtnList.insert( std::make_pair( mit->first, pWnd ) );
		}
	} 
}

void KGCSceneMenuBtnS6::SetBtnGroupList( std::vector<int> &vecContents, std::vector<int> &vecEvents, std::vector<int> &vecFriends, std::vector<int> &vecSystem )
{
	m_vecContents.clear();
    m_vecEvents.clear();
    m_vecFriends.clear();
    m_vecSystem.clear();
    

    std::vector<int> vecDisableList; 
    vecDisableList.clear();
    vecDisableList = g_kGlobalValue.GetDisableMenuList();

	for ( std::map<int, KD3DWnd*>::iterator mit = m_mapUIBtnList.begin() ; mit != m_mapUIBtnList.end() ; ++mit ) {
		mit->second->ToggleRender(false);
	}
    //컨텐츠 리스트
	for ( std::vector<int>::iterator vit = vecContents.begin() ; vit != vecContents.end() ; ++vit ) {
		std::map<int, KD3DWnd*>::iterator mit = m_mapUIBtnList.find(*vit);
		if ( mit == m_mapUIBtnList.end() ) continue;
        
        std::vector<int>::iterator vit2 = std::find( vecDisableList.begin(), vecDisableList.end(), mit->first );
        if (vit2 != vecDisableList.end())
            continue;

		// 졸업하지 못한 캐릭터 메뉴 제외 한다.
        if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
        {
            if ( IsPrivateCharDiableMenu( mit->first ) )
                continue;
        }        
        
        m_vecContents.push_back( mit->second );
	}
    //이벤트 리스트
	for ( std::vector<int>::iterator vit = vecEvents.begin() ; vit != vecEvents.end() ; ++vit ) {
		std::map<int, KD3DWnd*>::iterator mit = m_mapUIBtnList.find(*vit);
		if ( mit == m_mapUIBtnList.end() ) continue;
        
        std::vector<int>::iterator vit2 = std::find( vecDisableList.begin(), vecDisableList.end(), mit->first );
        if (vit2 != vecDisableList.end())
            continue;

        // 졸업하지 못한 캐릭터 메뉴 제외 한다.
        if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
        {
            if ( IsPrivateCharDiableMenu( mit->first ) )
                continue;
        } 
		
        m_vecEvents.push_back( mit->second );
	}
    //친구 리스트 
    for ( std::vector<int>::iterator vit = vecFriends.begin() ; vit != vecFriends.end() ; ++vit ) {
        std::map<int, KD3DWnd*>::iterator mit = m_mapUIBtnList.find(*vit);
        if ( mit == m_mapUIBtnList.end() ) continue;

        std::vector<int>::iterator vit2 = std::find( vecDisableList.begin(), vecDisableList.end(), mit->first );
        if (vit2 != vecDisableList.end())
            continue;

        // 졸업하지 못한 캐릭터 메뉴 제외 한다.
        if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
        {
            if ( IsPrivateCharDiableMenu( mit->first ) )
                continue;
        } 

        m_vecFriends.push_back( mit->second );
    }
    //시스템 리스트
    for ( std::vector<int>::iterator vit = vecSystem.begin() ; vit != vecSystem.end() ; ++vit ) {
        std::map<int, KD3DWnd*>::iterator mit = m_mapUIBtnList.find(*vit);
        if ( mit == m_mapUIBtnList.end() ) continue;

        std::vector<int>::iterator vit2 = std::find( vecDisableList.begin(), vecDisableList.end(), mit->first );
        if (vit2 != vecDisableList.end())
            continue;

        // 졸업하지 못한 캐릭터 메뉴 제외 한다.
        if ( !SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
        {
            if ( IsPrivateCharDiableMenu( mit->first ) )
                continue;
        } 

        m_vecSystem.push_back( mit->second );
    }

}


void KGCSceneMenuBtnS6::SetBtnListPos( void )
{
	std::vector< KD3DWnd*>::iterator vit = m_vecSystem.begin();
	if ( vit == m_vecSystem.end() ) return;
	
	const int iNumWith = 7;

	const int iBaseBtnWidth = (*vit)->GetWidth();
	const int iBaseBtnHeight = (*vit)->GetHeight();

	const int iBaseLineHeight = m_pkLine->GetHeight();

	const float fStartXPos = 88.f;
	const float fStartYPos = 17.f;

	const float fSizingBoxTopGap = 2.f;

	const float fGap = 3.f;

	const float fLineGap = 10.f;
	const float fSizingBoxBottomGap = 10.f;

	const int iContentsSize = m_vecContents.size();
	const int iEventsSize = m_vecEvents.size();
    const int iFriendstSize = m_vecFriends.size();
    const int iSystemSize = m_vecSystem.size();


	
	//사이징 박스 세로 크기 설정.  아래 0.1f를 더해 주는 건 float 에서 3.0 == 2.9999999999999999 로 표현 될수 있기때문에 정수 변환하기전에 더해줌. 추가로 const를 정수로 변환이 바로 안됨.
	int iSizeingHeight = 
		static_cast<int>(fSizingBoxTopGap + 0.1f + fStartXPos) + ( (static_cast<int>(fGap + 0.1f )  + iBaseBtnWidth) * static_cast<int>( (iContentsSize == 0 ? 0 : (iContentsSize-1)) / iNumWith + 1)) 
		+ static_cast<int>(fLineGap + 0.1f) + iBaseLineHeight + static_cast<int>(fLineGap + 0.1f)
		+ iBaseBtnHeight + static_cast<int>(fSizingBoxBottomGap + 0.1f);
	m_pkBack->SetSize(m_pkBack->GetWidth(),  iSizeingHeight );
	SetHeightDirect(iSizeingHeight);

	//라인 위치 설정.
	/*float fLineY = fStartXPos + static_cast<float>( (static_cast<int>(fGap + 0.1f )  + iBaseBtnWidth) * static_cast<int>( (iContentsSize == 0 ? 0 : (iContentsSize-1)) / iNumWith + 1 )) + fLineGap;
	m_pkLine->SetWindowPosYDirect(fLineY);*/

	//컨텐츠 위치 설정.
	float fContenstStartY = fStartYPos;
    for ( int i = 0 ; i < iContentsSize ; ++i ) {
		float fPosX = fStartXPos + (( i % iNumWith ) * (fGap + iBaseBtnWidth) );
		float fPosY = fContenstStartY;

		m_vecContents[i]->SetWindowPosDirectFloat(fPosX,fPosY);

	}
	
	//이벤트 위치 설정
	float fEventStartY = fContenstStartY + fLineGap + iBaseLineHeight + iBaseBtnHeight - 1.f; 
	for ( int i = 0 ; i < iEventsSize; ++i ) {
		float fPosX = fStartXPos + (( i % iNumWith ) * (fGap + iBaseBtnWidth) );
		float fPosY = fEventStartY;

		m_vecEvents[i]->SetWindowPosDirectFloat(fPosX,fPosY);

	}

    //친구 위치 설정.
    float fFriendStartY = fEventStartY + fLineGap + iBaseLineHeight + iBaseBtnHeight - 1.f;
    for ( int i = 0 ; i < iFriendstSize; ++i ) {
        float fPosX = fStartXPos + (( i % iNumWith ) * (fGap + iBaseBtnWidth) );
        float fPosY = fFriendStartY;

        m_vecFriends[i]->SetWindowPosDirectFloat(fPosX,fPosY);

    }

    //시스템 위치 설정
    float fSystemStartY = fFriendStartY + fLineGap + iBaseLineHeight + iBaseBtnHeight - 1.f;
    for ( int i = 0 ; i < iSystemSize; ++i ) {
        float fPosX = fStartXPos + (( i % iNumWith ) * (fGap + iBaseBtnWidth) );
        float fPosY = fSystemStartY;

        m_vecSystem[i]->SetWindowPosDirectFloat(fPosX,fPosY);

    }

}


void KGCSceneMenuBtnS6::InitBtnList( void )
{

	for ( std::vector< KD3DWnd*>::iterator vit =  m_vecContents.begin() ; vit != m_vecContents.end() ; ++vit ) {
		(*vit)->InitState(true,true,this);
	}

	for ( std::vector< KD3DWnd*>::iterator vit =  m_vecEvents.begin() ; vit != m_vecEvents.end() ; ++vit ) {
		(*vit)->InitState(true,true,this);
	}

    for ( std::vector< KD3DWnd*>::iterator vit =  m_vecFriends.begin() ; vit != m_vecFriends.end() ; ++vit ) {
        (*vit)->InitState(true,true,this);
    }

    for ( std::vector< KD3DWnd*>::iterator vit =  m_vecSystem.begin() ; vit != m_vecSystem.end() ; ++vit ) {
        (*vit)->InitState(true,true,this);
    }

}

void KGCSceneMenuBtnS6::OnCreate( void )
{
#ifndef __PATH__
	LoadLuaTable();
#endif
	CreateBtnList();

	m_pkBack->InitState(true,false,NULL);
	m_pkLine->InitState(true,false,NULL);
	
	m_tBtnListActiveTime = GCUTIL_TIME::GetCurTime();
}

void KGCSceneMenuBtnS6::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_pWnd != NULL && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
		std::map<std::string, HANDLER>::iterator mit = m_mapEvent.find( event.m_pWnd->GetWindowName());
		if ( mit != m_mapEvent.end() ) {
			(this->*(mit->second))();
			
		}
	}
}
#ifdef USE_VIP_MENU_BTN // Kawan>
void KGCSceneMenuBtnS6::OnClickVipReward()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_VIP_REWARD); 
}
#endif
void KGCSceneMenuBtnS6::OnClickGacha()
{
    if( g_kGlobalValue.m_vecGachaUseVersion.empty() ) { 
        return;
    }
    
    SiKGCDepotManager()->Send_DepotInfoByCharTabReqForGacha();

     if( g_kGlobalValue.IsSingleGacha() ) {  
#if defined( USE_COORDI_CLEARSEAL )
        if( g_pkUIScene->GetClearSealDlg()->IsOpenUI() )
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0, g_kGlobalValue.GetSingleGachaVer() );  //m_iItemID, GachaVer
#else
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_CLEAR_SEAL_BEAD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,  0, g_kGlobalValue.GetSingleGachaVer() );  //m_iItemID, GachaVer
#endif
    }
    else {  
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SUPER_GACHA, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType,g_MyD3D->m_TempPlayer.GetCurrentChar().GetCurrentPromotion() );
    }
}

#ifndef DISABLE_HERO_SHOP_ICON
void KGCSceneMenuBtnS6::OnClickHeroShop()
{
	g_MyD3D->OnHeroShopButton();
}
#endif

void KGCSceneMenuBtnS6::OnClickSkillTree()
{
	g_MyD3D->OnSkillTreeButton();
}

void KGCSceneMenuBtnS6::OnClickCouple()
{
	g_MyD3D->OnCoupleButton();
}

void KGCSceneMenuBtnS6::OnClickSquare()
{
	g_MyD3D->OnSquareButton();
}

void KGCSceneMenuBtnS6::OnClickCalendar()
{
	g_MyD3D->OnCalendarButton();
}

void KGCSceneMenuBtnS6::OnClickMission()
{
	g_MyD3D->OnMissionButton();
}

void KGCSceneMenuBtnS6::OnClickMSN()
{
	g_MyD3D->OnMSNButton();
}

void KGCSceneMenuBtnS6::OnClickOption()
{
	g_MyD3D->OnOptionButton();
}

void KGCSceneMenuBtnS6::OnClickCreateBreadkup()
{
	g_MyD3D->OnItemCreateAndBreakUpButton();
}

void KGCSceneMenuBtnS6::OnClickEventBanner()
{
}

void KGCSceneMenuBtnS6::OnClickEventMiniGameBanner()
{
#if !defined( LOADING_IMAGE_FROM_SERVER_SCRIPT )

	if(g_kGlobalValue.m_vecEventOpenInfo.empty())
	{
		KLuaManager luaMgr;
		if( !GCFUNC::LoadLuaScript( luaMgr, "EventMinigameInfo.stg", 0 ) )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_NOTHING_EVENT_LIST ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
			return;
		}

		LUA_BEGIN_TABLE("EventOpenInfo", return)
		{
			for ( int i = 1; ; ++i )
			{
				std::string strTmp;
				KEventOpenInfo kEventOpenInfo;

				LUA_BEGIN_TABLE( i, break );

				luaMgr.GetValue( "EventID", kEventOpenInfo.m_dwEventID );
				luaMgr.GetValue( "EventMBoxID", kEventOpenInfo.m_dwMBoxID );
				luaMgr.GetValue( "EventTextureFileName", strTmp );
				kEventOpenInfo.m_wstrFileName = KncUtil::toWideString( strTmp );
				LUA_BEGIN_TABLE( "EventItemList", return );
				for ( int j = 1; ; ++j )
				{
					GCITEMID dwItemID;
					if( luaMgr.GetValue( j, dwItemID ) != S_OK ) break;
					kEventOpenInfo.m_vecItemList.push_back( dwItemID );
				}
				LUA_END_TABLE(return);

				LUA_END_TABLE(return);

				g_kGlobalValue.m_vecEventOpenInfo.push_back( kEventOpenInfo );
			}        
		}
		LUA_END_TABLE(return);
	}

#endif

	if(g_kGlobalValue.m_vecEventOpenInfo.empty())
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_NOTHING_EVENT_LIST ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
		return;
	}

	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_MINIGAME_LIST_DLG );
}

void KGCSceneMenuBtnS6::OnClickGuild()
{
	if( g_pkUIScene->m_pkSquareOverlayUI == NULL )
		return;

	if( g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu == NULL )
		return;

	if( g_pkUIScene->m_pkSquareOverlayUI->IsRenderOn() )
		g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->ToggleRender( g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->IsRenderOn() == false );
	else
	{
		g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->ToggleRender( g_pkUIScene->m_pkSquareOverlayUI->m_pkGuildSquareMenu->IsRenderOn() == false );
		g_pkUIScene->m_pkSquareOverlayUI->ToggleRender(true);
	}
}

void KGCSceneMenuBtnS6::OnClickReplayRecord()
{
	g_MyD3D->OnReplayRecordBtn();
}


void KGCSceneMenuBtnS6::OnClickUserMigration()
{
	if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY )
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_USER_MIGRATION_DLG );
	}
}

void KGCSceneMenuBtnS6::OnClickEnchant()
{
	if ( SiKGCPartyManager()->IsParty() )
		SiKGCPartyManager()->LeaveParty();

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_S6_DLG );
}

void KGCSceneMenuBtnS6::OnClickSurvey()
{
	if ( g_pkUIScene->m_pkSurveyDlg->IsHaveQuestion() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SURVEY_DLG );

		g_pkUIScene->m_pkSurveyDlg->RenderRewardItem();
	}
}

void KGCSceneMenuBtnS6::OnClickMonster()
{
	if ( GC_GLOBAL_DEFINE::bMonsterCardMix ) {

	if ( true == g_pkUIScene->m_pkMonsterCardCompose->GetSendReq() ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MONSTER_CARD_COMPOSE_DLG );
	}
	else {
		// 서버에서 받은 스크립트 정보가 없다면 요청
		KP2P::GetInstance()->Send_MonsterCardMixInfoReq();
	}

	} else {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NOT_EVENT_PERIOD ) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true );
		return;
	}
}

void KGCSceneMenuBtnS6::OnClickDepot()
{
    bool bActive = SiKGCVirtualDepotManager()->IsEmptyItemList();

    if( bActive )
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_DEPOT );
    else // 스트링 변경 필요함
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MSG2) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0,0, true, true, false);
}
void KGCSceneMenuBtnS6::OnClickCharSelectScene()
{
	g_MyD3D->OnChaSelectSceneBtn();

}

void KGCSceneMenuBtnS6::OnClickServerScene()
{
	g_MyD3D->OnServerSceneBtn();
}

void KGCSceneMenuBtnS6::SetBtnInfo( std::vector<int> &vecContents, std::vector<int> &vecEvents, std::vector<int> &vecFriends, std::vector<int> &vecSystem )
{
#ifndef __PATH__
	LoadLuaTable();
	CreateBtnList();
#endif

	SetBtnGroupList( vecContents, vecEvents, vecFriends, vecSystem  );
	SetBtnListPos();
	InitBtnList();
}

void KGCSceneMenuBtnS6::OnClickAgit()
{
	SiKGCAgitGameManager()->EnterMyAgit();
}

void KGCSceneMenuBtnS6::OnClickPost()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_POST_LETTER );
}

void KGCSceneMenuBtnS6::OnClickEnterGuild()
{
	if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) || g_kGlobalValue.m_kGuildInfo.m_dwUID == 0 || g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_ENTER_CHANNEL_ERROR2), L"" );
		return;
	}
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_ENTER_DLG );
}

void KGCSceneMenuBtnS6::OnClickReplay()
{
#if defined( REPLAY_BOX_NEW )
	if( g_kGlobalValue.m_bUseOldReplay )
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_S6 );
	else
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_BOX_NEW );
#else
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_S6 );
#endif

}

void KGCSceneMenuBtnS6::OnClickRoyalVip()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_VIP_GACHA_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}

void KGCSceneMenuBtnS6::OnClickGuide()
{
}

bool KGCSceneMenuBtnS6::IsPrivateCharDiableMenu( int nMenu )
{
    switch ( nMenu ) 
    {
    case SMB_HERO:
    case SMB_ENCHANT:
    case SMB_FORGE:
    case SMB_GACHA:
    case SMB_ENTER_GUILD:
    case SMB_AGIT_ENTER:
    case SMB_COUPLE:
    case SMB_DEPOT:
    case SMB_SQUARE:
        return true;
    default:
        return false;
    }

    return true;
}

IMPLEMENT_CLASSNAME( KGCSceneMenuS6 );
IMPLEMENT_WND_FACTORY( KGCSceneMenuS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCSceneMenuS6, "gc_scenemenubtn_6" );

const float ButtonUpSpeed = 0.30f;

KGCSceneMenuS6::KGCSceneMenuS6( void )
: m_pkOpenBtn(NULL)
, m_pkCloseBtn(NULL)
, m_pkBtnList(NULL)
, m_pkMenuBtn(NULL)
, m_bOpenMenu(false)
, m_bOpenPost(false)
, m_pkMSN(NULL)
, m_pkMSNBlink(NULL)
, m_pkSkilltreeBtn(NULL)
, m_pkPostBtn(NULL)
, m_pkPostBlinkBtn(NULL)
, m_vecSkilltreebtnPos(0.f, 0.0f)
, m_vecCreateSkilltreebtnPos(0.f, 0.0f)
, m_iAlphaTime(0)
, m_iheight(0)
, m_Percent(0)
, m_bUp(false)
//, m_bNewSkill(false)
{
	LINK_CONTROL("menu_open_btn" , m_pkOpenBtn);
	LINK_CONTROL("menu_close_btn" , m_pkCloseBtn);
	LINK_CONTROL("menu_btn" , m_pkMenuBtn);
	LINK_CONTROL("menu_btn_list" , m_pkBtnList);
	//LINK_CONTROL("menu_btn_list" , m_pkBtnList);
	LINK_CONTROL("msn_btn" , m_pkMSN);
    LINK_CONTROL("msn_blink_btn" , m_pkMSNBlink);
    
    LINK_CONTROL("skilltree_btn" , m_pkSkilltreeBtn);
    LINK_CONTROL("skilltree_alram" , m_pkSkilltreeAlram);

   LINK_CONTROL("post_btn" , m_pkPostBtn); 
   LINK_CONTROL("post_blink_btn", m_pkPostBlinkBtn );

   LINK_CONTROL("guide_btn" , m_pkGuideBtn); 
   LINK_CONTROL("guide_blink_btn", m_pkGuideBlinkBtn );
	

	m_iToggleFrame = POST_ICON_TOGGLE_FRAME;

	LoadLuaTable();
	m_iAlphaTime = 0;
}

KGCSceneMenuS6::~KGCSceneMenuS6( void )
{
}

void KGCSceneMenuS6::OnCreate( void )
{
#ifndef __PATH__
	LoadLuaTable();
#endif

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KD3DWnd::D3DWE_BUTTON_CLICK);
	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnClickEvent );

	m_pkOpenBtn->InitState(true,true,this);
	m_pkOpenBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnOpenBtnClick );
	m_pkOpenBtn->SetHotKey( DIK_ESCAPE );

	m_pkCloseBtn->InitState(false,true,this);
	m_pkCloseBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnCloseBtnClick );
	m_pkCloseBtn->SetHotKey( DIK_ESCAPE );

	m_pkMenuBtn->InitState(false,true,this);
	m_pkMenuBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnMenuBtnClick );
	m_pkMenuBtn->SetHotKey( DIK_ESCAPE );

	m_pkBtnList->InitState(false,true,this);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KD3DWnd::D3DWE_SCENE_CHANGE);
	g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCSceneMenuS6::OnChangeState );

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF, this, &KGCSceneMenuS6::OnHideBtnMSG );

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_ON);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_ON, this, &KGCSceneMenuS6::OnShowBtnMSG );

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_SQUARE_SCENE_MENUBTN_RENDER_OFF, this, &KGCSceneMenuS6::OnHideBtnMSG );

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_REPLAY_ON);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_REPLAY_ON, this, &KGCSceneMenuS6::OnHideBtnMSG );

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_OFF_GCMENU);
	g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_OFF_GCMENU, this, &KGCSceneMenuS6::OffMenu );

	m_pkMSN->InitState(true ,true ,this);
    m_pkMSNBlink->InitState( false, true, this );
	//m_pkMSNBlink->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnPostBtnClick );
    m_pkSkilltreeBtn->InitState( false, true, this );
    //m_pkSkilltreeBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnSkillTreeBtnClick );
    m_pkSkilltreeAlram->InitState( false, true, this );
    m_pkPostBtn->InitState( false, true, this);
    m_pkPostBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnPostBtnClick );
    m_pkPostBlinkBtn->InitState( false, true, this );
    m_pkPostBlinkBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSceneMenuS6::OnPostBtnClick );

	m_pkGuideBlinkBtn->InitState(false);
	m_pkGuideBtn->InitState(false);

	OnChangeState();
	m_bOpenMenu = false;
	m_iAlphaTime = 0;
                            
    
    m_vecCreateSkilltreebtnPos = m_pkSkilltreeBtn->GetCurrentWindowPos();
    m_vecCreateSkilltreebtnPos.y -= 15.f;
    m_vecSkilltreebtnPos = m_vecCreateSkilltreebtnPos;
}
void KGCSceneMenuS6::OnInit()
{
    m_vecSkilltreebtnPos = m_vecCreateSkilltreebtnPos;
  
    m_bOpenPost = false;
    m_bUp = false;
    m_Percent = 0;

}
void KGCSceneMenuS6::ActionPerformed( const KActionEvent& event )
{
	if(event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		if( event.m_pWnd == m_pkMSNBlink || event.m_pWnd == m_pkMSN )
		{
			g_MyD3D->OnMSNButton();
		}
	}
}

void KGCSceneMenuS6::FrameMoveInEnabledState( )
{
   //메신저 알람은 광장에서 반짝거려야 하기 때문.
   BlinkButton();
   if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE || g_MyD3D->m_pStateMachine->GetState() == GS_GUILD )
   {
        ShowNotice( false );
        return;
    }

    UpdateMarks();
    
    if(SiKGCPostLetterManager()->IsNewLetterRecv() == true )
        m_bOpenPost = false;
    
    if( (!m_bOpenPost) && (SiKGCPostLetterManager()->GetTotalNotReadLetterCount()))
    {
        m_pkPostBtn->ToggleRender( true );
        m_pkPostBlinkBtn->ToggleRender( true );
        
    }
    else if( !SiKGCPostLetterManager()->GetTotalNotReadLetterCount() )
    {
        m_pkPostBtn->ToggleRender( false );
        m_pkPostBlinkBtn->ToggleRender( false );
    }
  
    //[스킬 알림이]
    if( SiGCSKT()->GetNewSkillNum() > 0 )
    {
#ifndef DISABLE_MAP_SHOP // Kawan>
        if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM || g_MyD3D->m_pStateMachine->GetState() ==  GS_SHOP_GP )
#else
		if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
#endif
        {
            m_pkSkilltreeAlram->ToggleRender( false );
            return;
        }
        //g_pkUIScene->ShowNewSkillMsg();
        m_pkSkilltreeAlram->ToggleRender( true );

        if( m_vecSkilltreebtnPos.y > m_vecCreateSkilltreebtnPos.y - 18.f && !m_bUp )
        {
            m_vecSkilltreebtnPos.y -= ButtonUpSpeed;
        }
        else
        {
            m_bUp = true;
        }

        if( m_vecSkilltreebtnPos.y  < m_vecCreateSkilltreebtnPos.y - 13.f && m_bUp )
        {
            m_vecSkilltreebtnPos.y += ButtonUpSpeed;
        }
        else
        {
            m_bUp = false;
        }
        m_pkSkilltreeAlram->SetWindowPosYDirect( m_vecSkilltreebtnPos.y );
    }
    else if( SiGCSKT()->GetNewSkillNum() <= 0 )
    {
        m_pkSkilltreeAlram->ToggleRender( false );
    }
}
void KGCSceneMenuS6::BlinkButton()
{
    if ( !m_bOpenPost || g_MyD3D->AlertCheck_MSN() )
    {
        int iOpacity = POST_ICON_TOGGLE_FRAME;
        switch( m_iToggleFrame / POST_ICON_TOGGLE_FRAME )
        {
        case 0: // 0 ~ ( ALERT_TOGGLE_FRAME - 1 )
            iOpacity = m_iToggleFrame;
            break;
        case 1: // ( ALERT_TOGGLE_FRAME ) ~ ( ALERT_TOGGLE_FRAME * 2 - 1 )
            iOpacity = POST_ICON_TOGGLE_FRAME;
            break;
        case 2: // ( ALERT_TOGGLE_FRAME * 2 ) ~ ( ALERT_TOGGLE_FRAME * 3 - 1 )
            iOpacity = POST_ICON_TOGGLE_FRAME * 3 - m_iToggleFrame;
            break;
        }

        BYTE alpha = static_cast<BYTE>( 255.0f * static_cast<float>( iOpacity ) / static_cast<float>( POST_ICON_TOGGLE_FRAME ) );

        m_pkMSNBlink->SetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
        m_pkPostBlinkBtn->SetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
        m_iToggleFrame = ( m_iToggleFrame + 1 ) % ( POST_ICON_TOGGLE_FRAME * 3 );
    }
    else
    {
        m_pkPostBlinkBtn->ToggleRender( false );
        m_pkMSNBlink->ToggleRender( false );
    }
}

void KGCSceneMenuS6::OnPostBtnClick( )
{

    if( SiKGCMatchManager()->GetCurrentState() == KGCMatchManager::MSTATE_WAIT_STATE ||
         SiKGCPartyManager()->IsMatchReady(false))
           return;
 
    m_bOpenPost = true;
    m_Percent = 0;

    if(SiKGCPostLetterManager()->GetTotalNotReadLetterCount())
        SiKGCPostLetterManager()->RefreshPostLetterPageInfo(1 , KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL );

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_POST_LETTER );
}

void KGCSceneMenuS6::UpdateMarks()
{
    if( g_MyD3D->AlertCheck_MSN() )
    {
        m_pkMSN->ToggleRender(true);
        m_pkMSNBlink->ToggleRender(true);
    }
    else
    {
        m_pkMSN->ToggleRender(false);
        m_pkMSNBlink->ToggleRender(false);
    }
}



void KGCSceneMenuS6::OnHideBtnMSG( )
{
	m_pkBtnList->ToggleRender( false );
	m_pkOpenBtn->ToggleRender(false);
	m_pkCloseBtn->ToggleRender(false);
	m_pkMenuBtn->ToggleRender(false);
}

void KGCSceneMenuS6::OnShowBtnMSG( )
{
	m_pkMenuBtn->ToggleRender(true);
}


void KGCSceneMenuS6::OnOpenBtnClick( )
{
	if (IsMenuOpened()) return;

	if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT) return;

	if ( SiKGCMatchManager()->GetCurrentState() == KGCMatchManager::MSTATE_WAIT_STATE ) 
	{
		return;
	}

	if ( SiKGCPartyManager()->IsMatchReady(false) ) 
	{
		return;
	}

	if( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY )
	{
		return;
	}

    UpdateMenuList();

	m_bOpenMenu = true;
	m_pkOpenBtn->ToggleRender(false);
	m_pkCloseBtn->ToggleRender(true);

	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SHORT_MENU_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, false, 0, true, true);
}

void KGCSceneMenuS6::OnCloseBtnClick( )
{
	if (!IsMenuOpened()) return;
	m_bOpenMenu = false;
	m_pkOpenBtn->ToggleRender(true);
	m_pkCloseBtn->ToggleRender(false);
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SHORT_MENU_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, false, 0, true, true);
}

void KGCSceneMenuS6::OnMenuBtnClick( )
{
	if ( !g_pkUIScene->m_pkShortMenuCutBox->IsWndState( D3DWS_ENABLED ) )
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SHORT_MENU_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, true, 0, true, true);
}

void KGCSceneMenuS6::OnClickEvent( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	if ( m_pkOpenBtn->CheckPosInWindowBound(vMousePos) ) {
		return;
	}
	if ( m_pkCloseBtn->CheckPosInWindowBound(vMousePos) ) {
		return;
	}
	if ( m_pkMenuBtn->CheckPosInWindowBound(vMousePos)  ) {
		return;
	}
	if ( m_pkBtnList->CheckPosInWindowBound(vMousePos)  ) {
		return;
	}

	int iState = static_cast<int>(g_MyD3D->m_pStateMachine->GetState());

	std::map<int, MenuListInfo>::iterator mit = m_mapMenuListInfo.find(	iState );
	if ( mit == m_mapMenuListInfo.end() ) {
		ToggleRender(false);
		return;
	}

	m_bOpenMenu = false;

	if ( iState == GS_SQUARE ) {
		m_pkOpenBtn->ToggleRender(false);
		m_pkCloseBtn->ToggleRender(false);
		m_pkMenuBtn->ToggleRender(true);
	}
	else {
		m_pkOpenBtn->ToggleRender(!m_bOpenMenu);
		m_pkCloseBtn->ToggleRender(m_bOpenMenu);
		m_pkMenuBtn->ToggleRender(false);

	}

	m_pkBtnList->ToggleRender(false);
}


void KGCSceneMenuS6::OnChangeState( void )
{
#ifndef __PATH__
	LoadLuaTable();
#endif
	int iState = static_cast<int>(g_MyD3D->m_pStateMachine->GetState());

    if ( iState == GS_SQUARE || iState == GS_GUILD ) {
		m_pkOpenBtn->ToggleRender(false);
		m_pkCloseBtn->ToggleRender(false);
		m_pkMenuBtn->ToggleRender(true);
	}
	else {
		m_pkOpenBtn->ToggleRender(!m_bOpenMenu);
		m_pkCloseBtn->ToggleRender(m_bOpenMenu);
		m_pkMenuBtn->ToggleRender(false);
	}

    UpdateMenuList();
}

void KGCSceneMenuS6::OnChangePos( MenuListInfo &kInfo )
{
	SetWindowPosDirect(kInfo.vMenuBtnPos);
	m_pkOpenBtn->SetWindowPosXDirect(kInfo.fBtnListPosX);
	m_pkCloseBtn->SetWindowPosXDirect(kInfo.fBtnListPosX);
	m_pkMenuBtn->SetWindowPosXDirect(kInfo.fBtnListPosX);
	

    //광장에서의 위치만 다르다.
    float fMsnBtnPosX = 0.f;
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ) 
        fMsnBtnPosX = -100.f;
    else
        fMsnBtnPosX = 84.f;
    
    m_pkMSN->SetWindowPosXDirect(kInfo.fBtnListPosX - fMsnBtnPosX);
    m_pkMSNBlink->SetWindowPosXDirect(kInfo.fBtnListPosX - fMsnBtnPosX);    

    if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM ) //룸에서의 위치만 다르다.
        m_pkSkilltreeAlram->SetWindowPosXDirect(m_vecCreateSkilltreebtnPos.x - 82.f ); //스킬트리 알람아이콘 위치 ui_scene_common.stg에 저장된 위치 기준
    else
        m_pkSkilltreeAlram->SetWindowPosXDirect(m_vecCreateSkilltreebtnPos.x - 260.f ); 
    
}

void KGCSceneMenuS6::LoadLuaTable()
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
		return;

	if( GCFUNC::LoadLuaScript( luaMgr, "GCMenuBtnInfoS6.lua" ) == false )
		return;

	m_mapMenuListInfo.clear();

	LUA_BEGIN_TABLE("GCMenuBtnInfoS6", return; ){
		for ( int i = 1 ; ; i++ )
		{
			MenuListInfo sListTemp;

			LUA_BEGIN_TABLE(i, break;)
            {
				LUA_GET_VALUE_NOASSERT("SceneNum",sListTemp.iSceneNum, LUA_END_TABLE(); break;);
				LUA_GET_VALUE_NOASSERT("MenuPosX",sListTemp.vMenuBtnPos.x, LUA_END_TABLE(); break;);
				LUA_GET_VALUE_NOASSERT("MenuPosY",sListTemp.vMenuBtnPos.y, LUA_END_TABLE(); break;);
				LUA_GET_VALUE_NOASSERT("ListPosX",sListTemp.fBtnListPosX, LUA_END_TABLE(); break;);



				LUA_BEGIN_TABLE( "Contents", break;)
                {
					for ( int j = 1 ; ; j++ )
					{
						int iBtnNum = 0;
						LUA_GET_VALUE_NOASSERT( j ,iBtnNum, break;);
						sListTemp.vecContents.push_back(iBtnNum);
					}

				}
                LUA_END_TABLE(break;);

				LUA_BEGIN_TABLE( "Events", break;)
                {
					for ( int j = 1 ; ; j++ )
					{
						int iBtnNum = 0;
						LUA_GET_VALUE_NOASSERT( j ,iBtnNum, break;);
						sListTemp.vecEvents.push_back(iBtnNum);
					}

				}
                LUA_END_TABLE(break;);

                LUA_BEGIN_TABLE( "Friends", break;)
                {
                    for ( int j = 1 ; ; j++ )
                    {
                        int iBtnNum = 0;
                        LUA_GET_VALUE_NOASSERT( j ,iBtnNum, break;);
                        sListTemp.vecFriends.push_back(iBtnNum);
                    }

                }
                LUA_END_TABLE(break;);

                LUA_BEGIN_TABLE( "System", break;)
                {
                    for ( int j = 1 ; ; j++ )
                    {
                        int iBtnNum = 0;
                        LUA_GET_VALUE_NOASSERT( j ,iBtnNum, break;);

						if(SMB_GUIDE == iBtnNum)
							continue;

                        sListTemp.vecSystem.push_back(iBtnNum);
                    }

                }
                LUA_END_TABLE(break;);

			}
            LUA_END_TABLE(break;);

			m_mapMenuListInfo.insert(std::make_pair( sListTemp.iSceneNum, sListTemp ));
		}

	}LUA_END_TABLE(return;);



}

void KGCSceneMenuS6::OffMenu( const KActionEvent& event )
{
	ToggleRender(static_cast<bool>(!event.m_dwlParam));
}

void KGCSceneMenuS6::ShowNotice( bool bRender )
{

    m_pkPostBtn->ToggleRender( bRender );
    m_pkPostBlinkBtn->ToggleRender( bRender );
}

bool KGCSceneMenuS6::IsUpPostBtn()
{
    return m_bUp;
}

void KGCSceneMenuS6::UpdateMenuList()
{
	int iState = static_cast<int>(g_MyD3D->m_pStateMachine->GetState());

    std::map<int, MenuListInfo>::iterator mit = m_mapMenuListInfo.find(	iState );
    if ( mit == m_mapMenuListInfo.end() ) {
        ToggleRender(false);
        return;
    }

    OnChangePos(mit->second);

    m_pkBtnList->SetBtnInfo( mit->second.vecContents, mit->second.vecEvents, mit->second.vecFriends, mit->second.vecSystem);
    ToggleRender(true);
}


bool KGCSceneMenuS6::IsMenuOpened()
{
    return m_pkCloseBtn->IsRenderOn();
}

void KGCSceneMenuS6::OnSkillTreeBtnClick()
{
    g_MyD3D->OnSkillTreeButton();
}