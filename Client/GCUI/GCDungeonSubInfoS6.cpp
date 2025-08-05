#include "stdafx.h"
#include "GCDungeonSubInfoS6.h"
#include "GCRoomListBtnSetS6.h"
#include "KGCWorldMapManager.h"
#include "KGCRoomListManager.h"
#include "GCTowerLevelSelector.h"

IMPLEMENT_CLASSNAME( KGCDungeonSubInfoS6 );
IMPLEMENT_WND_FACTORY( KGCDungeonSubInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonSubInfoS6, "gc_dungeon_sub_info_s6" );

KGCDungeonSubInfoS6::KGCDungeonSubInfoS6( void )
: m_pkTowerLevelSelector( NULL )
, m_pkTimesBG( NULL )
, m_pkClearNum( NULL )
{
	char szTemp[128];
	memset(szTemp, 0, sizeof(char) * 128);

	for ( int i = KDSI_BTN_SET_0 ; i < KDSI_BTN_SET_NUM ; i++ ) {
		sprintf(szTemp, "btn_set_%d", i);
		m_saBtnSet[i] = NULL;
		LINK_CONTROL(szTemp,m_saBtnSet[i]);
	}

	for ( int i = 0 ; i < KDSI_BTN_SET_4; i++ ) {
		sprintf(szTemp, "item_img_view_temp_%d", i);
		m_saItemImage[i] = NULL;
		LINK_CONTROL(szTemp,m_saItemImage[i]);
	}

	for ( int i = 0; i < 2; i++ )
	{
		sprintf(szTemp, "corruption_%d", i);
		m_pCorruptionIcon[i] = NULL;
		LINK_CONTROL(szTemp, m_pCorruptionIcon[i]);
	}

	LINK_CONTROL("tower_level", m_pkTowerLevelSelector);
	LINK_CONTROL("times_bg", m_pkTimesBG);
	LINK_CONTROL("static_clear_count", m_pkClearNum);
}

KGCDungeonSubInfoS6::~KGCDungeonSubInfoS6( void )
{

}

void KGCDungeonSubInfoS6::ActionPerformed( const KActionEvent& event )
{

}

//던전 변경시에만 불려야 한다.
void KGCDungeonSubInfoS6::UpdateChangeState( void )
{
	SiKGCRoomListManager()->SetCurrentLevel(0);
    UpdateData();
}

void KGCDungeonSubInfoS6::UpdateData( void )
{
    for ( int i = KDSI_BTN_SET_0 ; i < KDSI_BTN_SET_NUM ; i++ ) {
        m_saBtnSet[i]->ToggleRender(false);
    }

    EGCGameMode emGameMode = GC_GM_INVALID;

	if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM)
	{
		emGameMode = SiKGCRoomManager()->GetGameMode();
		if (SiKGCWorldMapManager()->IsHeroDungeonMode(emGameMode))
		{
			if (m_pkTimesBG)
				m_pkTimesBG->ToggleRender(true);
			std::pair<int, int> pairNum(g_kGlobalValue.m_kHeroDungeonInfo.GetClearInfo(emGameMode));
			if (m_pkClearNum)
				m_pkClearNum->SetText(g_pkStrLoader->GetReplacedString(STR_ID_HERO_DUNGEON_CLEAR_COUNT, "ii", pairNum.first, pairNum.second));
		}
		else
		{
			if (m_pkTimesBG)
				m_pkTimesBG->ToggleRender(false);
			if (m_pkClearNum)
				m_pkClearNum->SetText(L"");
		}
	}
	else
	{
		emGameMode = SiKGCWorldMapManager()->GetGameMode();
	}

	bool bHell = SiKGCWorldMapManager()->IsHellMode(emGameMode);
	bool bBreak = SiKGCWorldMapManager()->IsBreakMode(emGameMode);

    std::pair<int, int> pairLevel = SiKGCWorldMapManager()->GetCurrentDungeonPlayLevelAndLastLevel( emGameMode );

	if (SiKGCWorldMapManager()->GetGameMode() == GC_GM_QUEST52)
	{
		if (m_pkTowerLevelSelector)
			m_pkTowerLevelSelector->ToggleRender(true);
	}
	else
	{
		if (bHell == false) {
			RenderBtnSet(KDSI_BTN_SET_0);
		}
		else if (bBreak == true) {
			if (pairLevel.second < 2) {
				RenderBtnSet(KDSI_BTN_SET_5);
			}
			else {
				RenderBtnSet(KDSI_BTN_SET_4);
			}
		}
		else {
			RenderBtnSet(pairLevel.second);
		}
		if (m_pkTowerLevelSelector)
			m_pkTowerLevelSelector->ToggleRender(false);
	}
	if (m_pkTowerLevelSelector)
		m_pkTowerLevelSelector->SetDungeonLevel();

	SetCorruptionIcon();
    SetItemInfo();
}

void KGCDungeonSubInfoS6::RenderBtnSet( int index )
{
	if(index < KDSI_BTN_SET_0 || index > KDSI_BTN_SET_5) return;
	m_saBtnSet[index]->SetWndState();
	m_saBtnSet[index]->SetBtnState();
	m_saBtnSet[index]->ToggleRender(true);
}

void KGCDungeonSubInfoS6::OnCreate( void )
{
	for ( int i = KDSI_BTN_SET_0 ; i < KDSI_BTN_SET_NUM ; i++ ) {
		m_saBtnSet[i]->InitState(false,true,this);
	}

	for ( int i = 0 ; i < KDSI_BTN_SET_4; i++ ) {
		m_saItemImage[i]->InitState(false,false,NULL);
	}

	if (m_pkTowerLevelSelector)
		m_pkTowerLevelSelector->InitState(false, true, this);

	if (m_pkTimesBG)
		m_pkTimesBG->InitState(false, true, this);

	if (m_pkClearNum)
	{
		m_pkClearNum->InitState(true, true, this);
		m_pkClearNum->SetAlign(DT_CENTER);
	}

	for (int i = 0; i < 2; i++)
		if(m_pCorruptionIcon[i])
			m_pCorruptionIcon[i]->InitState(false, true, this);

	UpdateChangeState();
}

void KGCDungeonSubInfoS6::SetCorruptionIcon( void )
{
	EGCGameMode tempGameMode;
	bool isHellMode;

	if ( g_MyD3D->m_pStateMachine->GetState() != GS_ROOM )
	{
		tempGameMode = SiKGCWorldMapManager()->GetGameMode();
		isHellMode = SiKGCWorldMapManager()->IsHellMode();
	}
	else
	{
		tempGameMode = SiKGCRoomManager()->GetGameMode();
		isHellMode = SiKGCRoomManager()->IsHellMode();
	}

	int tempCorruptionRecomendation = SiKGCWorldMapManager()->GetCorruptionRecomended( tempGameMode );

	if ( tempCorruptionRecomendation > 0 )
	{
		std::wstring tempCorruptionString = g_pkStrLoader->GetReplacedString( STR_ID_RECOMENDED_CORRUPTION_RESIST, "i", tempCorruptionRecomendation );

		for ( int i = 0; i < 2; i++ )
			if ( m_pCorruptionIcon[i] )
				m_pCorruptionIcon[i]->SetToolTip( tempCorruptionString );

		if ( isHellMode )
		{
			if ( m_pCorruptionIcon[0] )
				m_pCorruptionIcon[0]->ToggleRender( false );
			if ( m_pCorruptionIcon[1] )
				m_pCorruptionIcon[1]->ToggleRender( true );
		}
		else
		{
			if ( m_pCorruptionIcon[0] )
				m_pCorruptionIcon[0]->ToggleRender( true );
			if ( m_pCorruptionIcon[1] )
				m_pCorruptionIcon[1]->ToggleRender( false );
		}
	}
	else
	{
		if ( m_pCorruptionIcon[0] )
			m_pCorruptionIcon[0]->ToggleRender( false );
		if ( m_pCorruptionIcon[1] )
			m_pCorruptionIcon[1]->ToggleRender( false );
	}
}

void KGCDungeonSubInfoS6::SetItemInfo( void )
{
 	std::vector<GCITEMID> vecTemp = SiKGCRoomListManager()->GetCurrentModeAndLevelRewardItem();
	int i = 0; 
	for ( ; i < KDSI_BTN_SET_4; i++ ) {
		m_saItemImage[i]->ToggleRender(false);
		m_saItemImage[i]->SetItemInfo(0);
	}
	
	i = 0;
	for ( std::vector<GCITEMID>::iterator vit = vecTemp.begin() ; vit != vecTemp.end() && i < KDSI_BTN_SET_4; ++vit, ++i ) {

		GCItem *pkItem = g_pItemMgr->GetItemData( *vit );

		if ( pkItem == NULL ) break;

		m_saItemImage[i]->SetItemInfo(*vit , -1, pkItem->dwSlotPosition & ESP_SET_ITEM , pkItem->iSetItemNumber);
		m_saItemImage[i]->ToggleRender(true);
		m_saItemImage[i]->ShowItem(true);
	} 
}



void KGCDungeonSubInfoS6::FrameMoveInEnabledState( void )
{
	for (int i= 0 ; i < KDSI_BTN_SET_4; i++ ) {
		if( m_saItemImage[i]->IsMouseInWindow()){
			DrawItemInfo(m_saItemImage[i]->GetItemID(), static_cast<KD3DWnd*>(m_saItemImage[i]));
			break;
		}
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
	}
}

void KGCDungeonSubInfoS6::DrawItemInfo(GCITEMID itemID, KD3DWnd *pWnd)
{
    if( itemID == 0)
        return;

	GCItem* pItem = g_pItemMgr->GetItemData( itemID );

    if( pItem == NULL || pItem->dwGoodsID == 0 ) { 
        return;
    }

	g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
	g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( g_pkUIScene->m_pkItemInformationBox->GetNewPosition( pWnd) );
	g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( D3DWZ_TOPMOST );

}