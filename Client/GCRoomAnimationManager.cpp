#include "stdafx.h"
#include "GCItem.h"
#include "Procedure.h"
#include "KSingleton.h"
#include "GCItemManager.h"
#include "GCGlobalFunction.h"
#include "GCRoomAnimationManager.h"
#include "GCUI\GCCustomizarionDlg.h"

KGCRoomAnimationManager* g_pRoomAnimationMgr = NULL;

KGCRoomAnimationManager::KGCRoomAnimationManager()
{
    g_pRoomAnimationMgr = this;
    m_vecRoomAnimationList.clear();
}

KGCRoomAnimationManager::~KGCRoomAnimationManager()
{
}

void KGCRoomAnimationManager::SetRoomAnimationList()
{
    if ( !m_vecRoomAnimationList.empty() )
		return;
	KLuaManager luaMgr;
	if ( GCFUNC::LoadLuaScript( luaMgr, "InitRoomAnimation.lua" ) == false )
		return;
	luaMgr.BeginTable( "RoomAnimationTable" );
	{
		for (int i = 1; ; ++i)
		{
			if ( FAILED( luaMgr.BeginTable( i ) ) ) break;
			{
				RoomAnimationInfo TempRoomAnim;
				LUA_GET_VALUE_DEF( "RoomAnimationID",			TempRoomAnim.m_iItemID,		   -1 );
				LUA_GET_VALUE_DEF( "RoomAnimationMotion",		TempRoomAnim.m_iMotion,		   -1 );
                LUA_GET_VALUE_DEF( "RoomAnimationModel",		TempRoomAnim.m_iModel,		   -1 );
				LUA_GET_VALUE_DEF( "RoomAnimationExtra",		TempRoomAnim.m_iExtra,		   -1 );
				LUA_GET_VALUE_DEF( "RoomAnimationType",			TempRoomAnim.m_iType,		    0 );
				LUA_GET_VALUE_DEF( "IsBuy",						TempRoomAnim.m_bIsBuy,		false );
				if (FAILED(luaMgr.BeginTable(1))) break;
				{
					LUA_GET_VALUE_DEF(  1, TempRoomAnim.m_pkExtraRoomInfo.m_sExtraModelM, "" );
					LUA_GET_VALUE_DEF(  2, TempRoomAnim.m_pkExtraRoomInfo.m_sExtraModelF, "" );
					LUA_GET_VALUE_DEF(  3, TempRoomAnim.m_pkExtraRoomInfo.m_sExtraTextureM, "" );
					LUA_GET_VALUE_DEF(  4, TempRoomAnim.m_pkExtraRoomInfo.m_sExtraTextureF, "" );
					LUA_GET_VALUE_DEF(  5, TempRoomAnim.m_pkExtraRoomInfo.m_sExtraAnimationM, "" );
					LUA_GET_VALUE_DEF(  6, TempRoomAnim.m_pkExtraRoomInfo.m_sExtraAnimationF, "" );
					LUA_GET_VALUE_DEF(  7, TempRoomAnim.m_pkExtraRoomInfo.m_fExtraX, 0 );
					LUA_GET_VALUE_DEF(  8, TempRoomAnim.m_pkExtraRoomInfo.m_fExtraY, 0 );
					LUA_GET_VALUE_DEF(  9, TempRoomAnim.m_pkExtraRoomInfo.m_fExtraZ, 0 );
					LUA_GET_VALUE_DEF( 10, TempRoomAnim.m_pkExtraRoomInfo.m_fExtraScale, 0 );
					LUA_GET_VALUE_DEF( 11, TempRoomAnim.m_pkExtraRoomInfo.m_bIsLoop, false );
				}
				luaMgr.EndTable();
				m_vecRoomAnimationList.emplace_back( TempRoomAnim );
			}
			luaMgr.EndTable();
		}
	}
	luaMgr.EndTable();
}

RoomAnimationInfo KGCRoomAnimationManager::GetRoomAnimation( int iPlayerID )
{
	this->iPlayerID = iPlayerID;

	RoomAnimationInfo tempRoomAnimation;
	for (auto& element : m_vecRoomAnimationList)
	{
		if (g_MyD3D->MyPlayer[this->iPlayerID]->CheckEquipItem(element.m_iItemID / 10))
		{
			return element;
		}
	}
	return tempRoomAnimation;
}

RoomAnimationInfo KGCRoomAnimationManager::GetCurrentRoomAnimation( GCITEMID m_iItemID )
{
	RoomAnimationInfo tempRoomAnimation;
	for ( auto& element : m_vecRoomAnimationList )
		if ( element.m_iItemID == m_iItemID )
			return element;
	return tempRoomAnimation;
}

void KGCRoomAnimationManager::UnEquipRoomAnimation( SCharInfo* pCharInfo, bool bUpdate )
{
	if ( !pCharInfo )
		return;
	for ( auto& element : m_vecRoomAnimationList )
		g_pItemMgr->UnequipItemForItemID( pCharInfo, element.m_iItemID / 10 );
	if( bUpdate )
		g_pkUIScene->m_pkCustomizationDlg->UpdateSubWindow( 1 );
}

bool KGCRoomAnimationManager::EquipRoomAnimation( SCharInfo* pCharInfo, int iItemID )
{
	KItem* pInventoryItem = NULL;
	if ( g_pItemMgr->FindInventoryForItemID( iItemID / 10, &pInventoryItem ) )
	{
		if ( pCharInfo->vecItemInfo.size() < MAX_EQUIP_NUM )
		{
			UnEquipRoomAnimation( pCharInfo, false );
			SUserItem _Item;
			_Item.dwItemUID = pInventoryItem->m_ItemUID;
			_Item.iItemID   = pInventoryItem->m_ItemID;
			pCharInfo->vecItemInfo.push_back( _Item );
			g_pkUIScene->m_pkCustomizationDlg->UpdateSubWindow( 1 );
			return true;
		}
		else if ( g_pkUIScene != NULL )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_MORE_EQUIP1 ), g_pkStrLoader->GetString( STR_ID_NO_MORE_EQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
	}
    return false;
}