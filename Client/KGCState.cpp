#include "stdafx.h"
// KGCState.cpp: implementation of the KGCState class.
//
//////////////////////////////////////////////////////////////////////

#include "KGCState.h"
//

//

////
#include "GCUI/GCItemReceiveBox.h"
#include "gcui/KGCTitleWindow.h"
#include "gcui/KGCTreasureItemBox.h"
#include "MyD3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCState::KGCState()
{
	m_bStateLock = false;
	m_eStateForFail = GS_SERVER;
}

KGCState::~KGCState()
{

}

bool KGCState::OnCheckNextState( GAME_STATE eNextState_ )
{
	std::vector<std::pair<GAME_STATE, GAME_STATE>>::iterator vit;

	for( vit = m_vecVaildNextState.begin() ; vit != m_vecVaildNextState.end() ; vit++ )
	{
		if( vit->first == eNextState_ )
			return true;
	}

	return false;
}

GAME_STATE KGCState::GetStateForFail( GAME_STATE  eTargetState_, bool bComplete_  )
{
	if ( bComplete_ == true )
		return m_eStateForFail;
	std::vector<std::pair<GAME_STATE, GAME_STATE>>::iterator vit;
	for ( vit = m_vecVaildNextState.begin() ; vit != m_vecVaildNextState.end() ; vit++ )
	{
		if ( vit->first ==  eTargetState_ )
			return vit->second;
	}
	return m_eStateForFail;
}

void KGCState::ProcessNotifyMessage()
{
    // 가이드가 진행 중이라서 UI를 띄울 수 없다면 나중에 띄우자
    if ( g_MyD3D->m_kGuideMgr.CanUseMsgBox() == false )
        return;

	// 던전에서 획득한 칭호가 있는지 확인하고 있다면 표시 해줍니다.
	std::vector<int> vtItem;
	{
		std::vector< std::pair< GCITEMUID, int > >  vecItem = g_pkUIScene->m_pkItemReceiveBox->GetItemVector(); 
		std::vector< std::pair< GCITEMUID, int > >::iterator it = vecItem.begin();

		for(;it != vecItem.end();it++)
		{
			KItem* pInventory = NULL;
			if( g_pItemMgr->FindInventory(it->first, &pInventory ) && pInventory && g_pItemMgr->CheckItemKind( pInventory->m_ItemID, GIK_TITLE_ITEM ) )
				vtItem.push_back( pInventory->m_ItemID );
		}


	}
	
	
	if( vtItem.empty() == false )
	{		
		g_pkUIScene->m_pkTreaureItemBox->SetItem( vtItem[0] );
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX );
	}
	else
	{
		// 던전에서 획득한 칭호가 없고, 완료한 칭호 미션이 있으면 메시지 박스를 띄워줍니다.
		const int iNewTitleID = g_kGlobalValue.m_kUserCollectionMission.GetNewCompleteTitleID();
		if( -1 != iNewTitleID )
		{			
			if( !g_pItemMgr->FindInventoryForItemID( iNewTitleID / 10 ) )
			{
				GCItem *pItem = g_pItemMgr->GetItemData( iNewTitleID / 10 );
				if( pItem )
				{
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK
					, g_pkStrLoader->GetReplacedString( STR_ID_TITLE_MISSION_COMPLETE_MSG1, "s", pItem->strItemName.c_str() )
					, g_pkStrLoader->GetString( STR_ID_TITLE_MISSION_COMPLETE_MSG2 ) );
				}
			}
		}		
	}
	g_kGlobalValue.m_kUserCollectionMission.ClearThisTimeCompleteMission();

	g_pkUIScene->m_pkTitleCollection->RefreshTitleList();

    std::vector< DWORD > vecCompleteThisTime = g_kGlobalValue.m_kUserMission.GetThisTimeCompleteMission();
    g_kGlobalValue.m_kUserMission.ClearThisTimeCompleteMission();

    if( !vecCompleteThisTime.empty() )
    {
		if (  SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) == true || 
            g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL_DUNGEON_OUT &&
            !g_MyD3D->m_kGuideMgr.IsCompleteGuide(12) ) // 튜토리얼중이고 에픽미션가이드가 완료되지 않으면 미션창을 보여줘야한다
			return;
		//g_pkUIScene->EnableMessageBox( false );
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG );
    }
}

bool KGCState::BeginScene()
{
    if( SUCCEEDED( g_MyD3D->m_pd3dDevice->BeginScene() ) )
        return true;
    return false;
}

void KGCState::EndScene()
{
    g_pGCDeviceManager2->Flush();
    g_MyD3D->m_pd3dDevice->EndScene();
}