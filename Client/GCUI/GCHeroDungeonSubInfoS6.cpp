#include "stdafx.h"
#include "GCHeroDungeonSubInfoS6.h"
#include "GCTowerLevelSelector.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonSubInfoS6 );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonSubInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonSubInfoS6, "gc_hero_dungeon_sub_info_s6" );

KGCHeroDungeonSubInfoS6::KGCHeroDungeonSubInfoS6( void )
: m_pkClearNum(NULL)
, m_pkRankingBtn( NULL )
, m_pkTowerLevelSelector( NULL )
{
	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for ( int i = 0 ; i < KHDSI_CIRCLE_NUM ; i++ ) {
		sprintf(szTemp, "circle%d", i);
		m_saCircle[i] = NULL;
		LINK_CONTROL(szTemp,m_saCircle[i]);
	}

	for ( int i = 0 ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		sprintf(szTemp, "item_img_view_temp_%d", i);
		m_saItemImage[i] = NULL;
		LINK_CONTROL(szTemp,m_saItemImage[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		sprintf(szTemp, "corruption_%d", i);
		m_pCorruptionIcon[i] = NULL;
		LINK_CONTROL(szTemp, m_pCorruptionIcon[i]);
	}

	LINK_CONTROL("tower_level", m_pkTowerLevelSelector);	
	LINK_CONTROL("static_clear_count",m_pkClearNum);		
    LINK_CONTROL("ranking_btn", m_pkRankingBtn );
	LINK_CONTROL("check_reward", m_pkCheckReward);
	LINK_CONTROL("get_reward", m_pkGetReward);
}

KGCHeroDungeonSubInfoS6::~KGCHeroDungeonSubInfoS6( void )
{
}

void KGCHeroDungeonSubInfoS6::ActionPerformed( const KActionEvent& event )
{

}

void KGCHeroDungeonSubInfoS6::SetCorruptionIcon( void )
{
	int tempCorruptionRecomendation = SiKGCWorldMapManager()->GetCorruptionRecomended( SiKGCWorldMapManager()->GetGameMode() );

	if ( tempCorruptionRecomendation > 0 )
	{
		std::wstring tempCorruptionString = g_pkStrLoader->GetReplacedString( STR_ID_RECOMENDED_CORRUPTION_RESIST, "i", tempCorruptionRecomendation );

		for ( int i = 0; i < 2; i++ )
			if ( m_pCorruptionIcon[i] )
				m_pCorruptionIcon[i]->SetToolTip( tempCorruptionString );

		if ( SiKGCWorldMapManager()->IsHellMode() )
		{
			if (m_pCorruptionIcon[0])
				m_pCorruptionIcon[0]->ToggleRender(false);
			if (m_pCorruptionIcon[1])
				m_pCorruptionIcon[1]->ToggleRender(true);
		}
		else
		{
			if (m_pCorruptionIcon[0])
				m_pCorruptionIcon[0]->ToggleRender(true);
			if (m_pCorruptionIcon[1])
				m_pCorruptionIcon[1]->ToggleRender(false);
		}
	}
	else
	{
		if (m_pCorruptionIcon[0])
			m_pCorruptionIcon[0]->ToggleRender(false);
		if (m_pCorruptionIcon[1])
			m_pCorruptionIcon[1]->ToggleRender(false);
	}
}

void KGCHeroDungeonSubInfoS6::SetRenderHeroDungeonV1_2( bool bOn  )
{
	for ( int i = 0 ; i < KHDSI_CIRCLE_NUM ; i++ ) {
		m_saCircle[i]->ToggleRender(bOn);
	}
	m_pkTowerLevelSelector->ToggleRender(false);
}

void KGCHeroDungeonSubInfoS6::SetRenderHeroDungeonV1_3( bool bOn )
{
    if ( m_pkRankingBtn )
    {
        m_pkRankingBtn->ToggleRender( bOn );
    }
    m_pkClearNum->ToggleRender( !bOn );
	m_pkTowerLevelSelector->ToggleRender(false);
}

void KGCHeroDungeonSubInfoS6::SetRenderHeroDungeonV30( bool bOn )
{
	m_pkTowerLevelSelector->ToggleRender(bOn);
}

void KGCHeroDungeonSubInfoS6::SetHeroDungeonV1_2( void )
{
	SetRenderHeroDungeonV30(false);
	SetRenderHeroDungeonV1_2(true);
    SetRenderHeroDungeonV1_3(false);

	pairNum = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(SiKGCWorldMapManager()->GetGameMode());

	for ( int i = 0 ; i < KHDSI_CIRCLE_NUM ; i++ ) {
		m_saCircle[i]->Lock( i < pairNum.first );
	}

	m_pkClearNum->SetText(g_pkStrLoader->GetReplacedString( STR_ID_HERO_DUNGEON_CLEAR_COUNT, "ii", pairNum.first, pairNum.second ));
	m_pkClearNum->SetAlign(DT_CENTER);
    m_pkClearNum->ToggleRender(true);
	
}

void KGCHeroDungeonSubInfoS6::SetHeroDungeonV1_3( void )
{
	SetHeroDungeonV1_2();
}

void KGCHeroDungeonSubInfoS6::SetHeroDungeonV30( void )
{
	SetRenderHeroDungeonV1_2(false);
	SetRenderHeroDungeonV1_3(false);
	SetRenderHeroDungeonV30(true);

	pairNum = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(SiKGCWorldMapManager()->GetGameMode());

	m_pkClearNum->SetText(g_pkStrLoader->GetReplacedString(STR_ID_HERO_DUNGEON_CLEAR_COUNT, "ii", pairNum.first, pairNum.second));
	m_pkClearNum->SetAlign(DT_CENTER);
	m_pkClearNum->ToggleRender(true);

	int iEnableLevel = SiKGCWorldMapManager()->GetPlayEnableDungeonLevel(SiKGCWorldMapManager()->GetGameMode(),
		g_kGlobalValue.m_kUserInfo.mapStagePlayable);

	if (SiKGCWorldMapManager()->GetGameMode() == GC_GM_QUEST52)
	{
		int nSelect = SiKGCWorldMapManager()->GetDungeonRewardNum(SiKGCWorldMapManager()->GetGameMode(), g_kGlobalValue.m_kUserInfo.mapStagePlayable);
		bool bOpen = nSelect > 0 ? true : false;

		m_pkCheckReward->ToggleRender(!bOpen);
		m_pkGetReward->ToggleRender(bOpen);
		g_pkUIScene->m_pkHeroRewardS6->SetLock(!bOpen);
	}

	SiKGCRoomListManager()->SetCurrentLevel(iEnableLevel);
	m_pkTowerLevelSelector->SetDungeonLevel();
}


void KGCHeroDungeonSubInfoS6::OnCreate( void )
{
	for ( int i = 0 ; i < KHDSI_CIRCLE_NUM ; i++ ) {
		m_saCircle[i]->InitState(false);
	}

	for ( int i = 0 ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		m_saItemImage[i]->InitState(false);
	}

	for (int i = 0; i < 2; ++i)
		if(m_pCorruptionIcon[i])
			m_pCorruptionIcon[i]->InitState(false, true, this);

	m_pkClearNum->InitState(false);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_SELECT);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_SELECT, this, &KGCHeroDungeonSubInfoS6::SetHeroDungeonInfo);

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL, this, &KGCHeroDungeonSubInfoS6::SetItemInfo);

    if ( m_pkRankingBtn )
    {
        m_pkRankingBtn->InitState( false, true, this );
        m_pkRankingBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroDungeonSubInfoS6::OnRankingButton);
    }

	m_pkCheckReward->InitState(false, true, this);
	m_pkCheckReward->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroDungeonSubInfoS6::OnCheckRewards);
	m_pkGetReward->InitState(false, true, this);
	m_pkGetReward->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroDungeonSubInfoS6::OnCheckRewards);

	SetHeroDungeonInfo();
}

void KGCHeroDungeonSubInfoS6::OnCheckRewards() {
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_BOX_HERO_REWARD, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}

void KGCHeroDungeonSubInfoS6::SetHeroDungeonInfo()
{
	if ( SiKGCWorldMapManager()->IsRelayHeroDungeon() ) {
		SetHeroDungeonV30();
	}
    else if ( SiKGCWorldMapManager()->GetGameMode() == GC_GM_QUEST81 )
    {
        SetHeroDungeonV1_3();
    }
	else if ( SiKGCWorldMapManager()->IsHeroDungeon() ) {
		SetHeroDungeonV1_2();
	}
    else { 
        return;
    }

	SetCorruptionIcon();
	SetItemInfo();	
}

void KGCHeroDungeonSubInfoS6::OnRankingButton()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_DUNGEON_RANKING_DLG,L"",L"",KGCUIScene::GC_MBOX_USE_NORMAL, 0,0,false,true);
}

void KGCHeroDungeonSubInfoS6::SetItemInfo( void )
{
	std::vector<GCITEMID> vecTemp = SiKGCRoomListManager()->GetCurrentModeAndLevelRewardItem();
    int i = 0;
	for ( ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		m_saItemImage[i]->ToggleRender(false);
		m_saItemImage[i]->SetItemInfo(0);
	}

    i = 0;
	for ( std::vector<GCITEMID>::iterator vit = vecTemp.begin() ; vit != vecTemp.end() && i < KDSI_ITEM_VIEW_NUM ; ++vit, ++i )
    {
        GCItem *pkItem = g_pItemMgr->GetItemData( *vit );
        if ( pkItem == NULL ) break;

        m_saItemImage[i]->SetItemInfo(*vit , -1, pkItem->dwSlotPosition & ESP_SET_ITEM , pkItem->iSetItemNumber);
		m_saItemImage[i]->ToggleRender(true);
		m_saItemImage[i]->ShowItem(true);
	} 
}

void KGCHeroDungeonSubInfoS6::FrameMoveInEnabledState( void )
{
	for (int i= 0 ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		if( m_saItemImage[i]->IsMouseInWindow()){
			DrawItemInfo(m_saItemImage[i]->GetItemID(), static_cast<KD3DWnd*>(m_saItemImage[i]));
			break;
		}
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}

void KGCHeroDungeonSubInfoS6::DrawItemInfo(GCITEMID itemID, KD3DWnd *pWnd)
{
	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInfoByItemID(pWnd, itemID);
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);

}