#include "stdafx.h"
#include "GCRaidDungeonInfoS6.h"
#include "KGCInfinityDungeonManager.h"
#include "GCRoomListBtnSetS6.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCRaidDungeonInfoS6 );
IMPLEMENT_WND_FACTORY( KGCRaidDungeonInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRaidDungeonInfoS6, "gc_raid_dungeon_sub_info_s6" );

KGCRaidDungeonInfoS6::KGCRaidDungeonInfoS6( void )
: m_pkOpenTime(NULL)
, m_pkCondition(NULL)
{
	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for (int i = KDSI_BTN_SET_0; i < KDSI_BTN_SET_NUM; i++)
	{
		m_saBtnSet[i] = nullptr;
		LINK_CONTROL_STM("btn_set_" << i, m_saBtnSet[i]);
	}

	for ( int i = 0 ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		sprintf(szTemp, "item_img_view_temp_%d", i);
		m_saItemImage[i] = NULL;
		LINK_CONTROL(szTemp,m_saItemImage[i]);
	}

	for ( int i = 0 ; i < KRDI_JOIN_TYPE_NUM ; i++ ) {
		m_saJoinTypeWnd[i] = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		sprintf(szTemp, "corruption_%d", i);
		m_pCorruptionIcon[i] = NULL;
		LINK_CONTROL(szTemp, m_pCorruptionIcon[i]);
	}

	LINK_CONTROL("level_background", LevelBG);
	LINK_CONTROL("static_time",m_pkOpenTime);	
	LINK_CONTROL("static_condition",m_pkCondition);	

	LINK_CONTROL("state_enable",m_saJoinTypeWnd[KRDI_JOIN_OK]);
	LINK_CONTROL("state_disable",m_saJoinTypeWnd[KRDI_JOIN_NOT]);
	LINK_CONTROL("state_not",m_saJoinTypeWnd[KRDI_JOIN_BELOW]);
}

KGCRaidDungeonInfoS6::~KGCRaidDungeonInfoS6( void )
{
}

void KGCRaidDungeonInfoS6::ActionPerformed( const KActionEvent& event )
{
}

void KGCRaidDungeonInfoS6::OnCreate( void )
{
	for (int i = KDSI_BTN_SET_0; i < KDSI_BTN_SET_NUM; i++) {
		m_saBtnSet[i]->InitState(false, true, this);
	}

	for ( int i = 0 ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		m_saItemImage[i]->InitState(false);
	}
	for ( int i = 0 ; i < KRDI_JOIN_TYPE_NUM ; i++ ) {
		m_saJoinTypeWnd[i]->InitState(false);
	}

	for (int i = 0; i < 2; ++i)
		if(m_pCorruptionIcon[i])
			m_pCorruptionIcon[i]->InitState(false, true, this);

	LevelBG->InitState(false, true, this);

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_SELECT);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_SELECT, this, &KGCRaidDungeonInfoS6::SetItemInfo);

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL, this, &KGCRaidDungeonInfoS6::SetItemInfo);

	SetDungeonInfo();
}

void KGCRaidDungeonInfoS6::SetDungeonInfo()
{
	SetRaidDungeon();
	SetItemInfo();
	SetCorruptionIcon();

	m_pkOpenTime->ToggleRender(true);
}

void KGCRaidDungeonInfoS6::SetCorruptionIcon( void )
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
			if(m_pCorruptionIcon[0])
				m_pCorruptionIcon[0]->ToggleRender( false );
			if (m_pCorruptionIcon[1])
				m_pCorruptionIcon[1]->ToggleRender( true );
		}
		else
		{
			if (m_pCorruptionIcon[0])
				m_pCorruptionIcon[0]->ToggleRender( true );
			if (m_pCorruptionIcon[1])
				m_pCorruptionIcon[1]->ToggleRender( false );
		}
	}
	else
	{
		if (m_pCorruptionIcon[0])
			m_pCorruptionIcon[0]->ToggleRender( false );
		if (m_pCorruptionIcon[1])
			m_pCorruptionIcon[1]->ToggleRender( false );
	}
}

void KGCRaidDungeonInfoS6::UpdateDificulty(void)
{
	for (int i = KDSI_BTN_SET_0; i < KDSI_BTN_SET_NUM; i++)
	{
		m_saBtnSet[i]->ToggleRender(false);
	}

	EGCGameMode emGameMode = GC_GM_INVALID;

	if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
	{
		emGameMode = SiKGCRoomManager()->GetGameMode();
	}
	else
	{
		emGameMode = SiKGCWorldMapManager()->GetGameMode();
	}

	std::pair<int, int> pairLevel = SiKGCWorldMapManager()->GetCurrentDungeonPlayLevelAndLastLevel(emGameMode);

	for (int i = 0; i < KRDI_JOIN_TYPE_NUM; i++) {
		m_saJoinTypeWnd[i]->ToggleRender(false);
	}

	if (SiKGCWorldMapManager()->IsHellMode(emGameMode))
	{
		RenderBtnSet(pairLevel.second);
	}
}

void KGCRaidDungeonInfoS6::RenderBtnSet(int index)
{
	if (index < KDSI_BTN_SET_0 || index > KDSI_BTN_SET_5)
	{
		return;
	}

	for (int i = 0; i < KRDI_JOIN_TYPE_NUM; i++) {
		m_saJoinTypeWnd[i]->ToggleRender(false);
	}

	LevelBG->ToggleRender(false);

	m_saBtnSet[index]->SetWndState();
	m_saBtnSet[index]->SetBtnState();
	m_saBtnSet[index]->ToggleRender(true);
}


std::wstring KGCRaidDungeonInfoS6::GetRaidDungeonConditionString( int iJoinType )
{
	std::pair<int, int> pairTemp;
	std::vector<KHeroDungeon>::iterator vit;
	int iGameMode_;
	int	iChar;

	switch ( iJoinType ) {
		case KGCWorldMapManager::GRPT_NOT_INFO :
		case KGCWorldMapManager::GRPT_ADMISSION :
			pairTemp = g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(SiKGCWorldMapManager()->GetGameMode());
			return g_pkStrLoader->GetReplacedString(STR_ID_RAID_CLEAR_COUNT, "ii",pairTemp.first ,pairTemp.second );
		case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION :
			return g_pkStrLoader->GetString(STR_ID_RAID_CLEAR_COMPLETE);
		case KGCWorldMapManager::GRPT_CONDITIONS_BELOW :
			iGameMode_ = SiKGCWorldMapManager()->GetGameMode();
			iChar = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
			vit = std::find_if( g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.begin(), 
				g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end(),
				boost::bind(&KHeroDungeon::m_nModeID,_1) == iGameMode_ );

			if ( vit == g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end() )
				return g_pkStrLoader->GetReplacedString(STR_ID_RAID_CONDITION, "is",1 ,L"F");

			return g_pkStrLoader->GetReplacedString(STR_ID_RAID_CONDITION, "ii", vit->m_dwMinCharacterLv, vit->m_nMinUserGrade);
		case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME :
			return g_pkStrLoader->GetString(STR_ID_RAID_JOIN_TIME);
	}
	return L"";
}


int KGCRaidDungeonInfoS6::ConvertErrType( int iJoinType )
{
	//	GRPT_NOT_INFO = -1, // 던전 정보가 없음.(레이드 OR 영웅던전이 아님)
	//	GRPT_ADMISSION = 0, // 입장가능
	//	GRPT_IMPOSSIBLE_ADMISSION = 1, // 입장불가능 (클리어 완료)
	//	GRPT_CONDITIONS_BELOW = 2, // 입장불가능 (조건 미달)
	//	GRPT_IMPOSSIBLE_TIME = 3, // 입장불가능 (입장가능한 시간이 아님)

	switch ( iJoinType ) {
		case KGCWorldMapManager::GRPT_NOT_INFO :
			return KRDI_JOIN_OK;
		case KGCWorldMapManager::GRPT_ADMISSION :
			return KRDI_JOIN_OK;
		case KGCWorldMapManager::GRPT_IMPOSSIBLE_ADMISSION :
		case KGCWorldMapManager::GRPT_IMPOSSIBLE_TIME :
			return KRDI_JOIN_NOT;
		case KGCWorldMapManager::GRPT_CONDITIONS_BELOW :
			return KRDI_JOIN_BELOW;
	}

	return KRDI_JOIN_OK;
}


void KGCRaidDungeonInfoS6::SetRaidDungeon( void )
{
	m_pkOpenTime->SetText(L"");
	m_pkOpenTime->SetAlign(DT_CENTER);

	int iJoinType = SiKGCWorldMapManager()->GetHeroDungeonPlayType(SiKGCWorldMapManager()->GetGameMode(), g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);

	m_pkCondition->SetText(GetRaidDungeonConditionString(iJoinType));
	m_pkCondition->SetAlign(DT_CENTER);

	int iJoinUIType = ConvertErrType(iJoinType);

	if (SiKGCWorldMapManager()->IsInfinityDungeon(SiKGCWorldMapManager()->GetGameMode()))
	{
		m_pkCondition->SetText(L"");
		m_pkOpenTime->SetText(L"∞");
		if (SiKGCInfinityDungeonManager()->GetInfinityDungeon(SiKGCWorldMapManager()->GetGameMode())->GetTotalATK() > SiKGCWorldMapManager()->GetChangeUserGradeInfo(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType))
			iJoinUIType = KRDI_JOIN_BELOW;
	}

	for (int i = 0; i < KRDI_JOIN_TYPE_NUM; i++)
		m_saJoinTypeWnd[i]->ToggleRender(iJoinUIType == i);

	LevelBG->ToggleRender(true);

	if (iJoinUIType == 0)
		UpdateDificulty();
}

void KGCRaidDungeonInfoS6::SetItemInfo( void )
{
	std::vector<GCITEMID> vecTemp = SiKGCRoomListManager()->GetCurrentModeAndLevelRewardItem();
	int i = 0; 
	for ( ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		m_saItemImage[i]->ToggleRender(false);
		m_saItemImage[i]->SetItemInfo(0);
	}

	i = 0;
	for ( std::vector<GCITEMID>::iterator vit = vecTemp.begin() ; vit != vecTemp.end() && i < KDSI_ITEM_VIEW_NUM ; ++vit, ++i ) {
		GCItem *pkItem = g_pItemMgr->GetItemData( *vit );

		if ( pkItem == NULL ) break;

		m_saItemImage[i]->SetItemInfo(*vit , -1, pkItem->dwSlotPosition & ESP_SET_ITEM , pkItem->iSetItemNumber);
		m_saItemImage[i]->ToggleRender(true);
		m_saItemImage[i]->ShowItem(true);
	} 
}

void KGCRaidDungeonInfoS6::FrameMoveInEnabledState( void )
{
	for (int i= 0 ; i < KDSI_ITEM_VIEW_NUM ; i++ ) {
		if( m_saItemImage[i]->IsMouseInWindow()){
			DrawItemInfo(m_saItemImage[i]->GetItemID(), static_cast<KD3DWnd*>(m_saItemImage[i]));
			break;
		}
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}

void KGCRaidDungeonInfoS6::DrawItemInfo(GCITEMID itemID, KD3DWnd *pWnd)
{
    GCItem* pItem = g_pItemMgr->GetItemData( itemID );
    if( NULL == pItem ) { 
        return;
    }

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( g_pkUIScene->m_pkItemInformationBox->GetNewPosition( pWnd ) );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);

}