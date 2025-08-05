#include "stdafx.h"
#include "GCItem.h"
#include "Procedure.h"
#include "KSingleton.h"
#include "GCItemManager.h"
#include "GCGlobalFunction.h"
#include "GCCutInSkinManager.h"
#include "GCUI\GCCustomizarionDlg.h"

KGCCutInSkinManager* g_pCutInSkinMgr = NULL;

KGCCutInSkinManager::KGCCutInSkinManager()
{
    g_pCutInSkinMgr = this;
    m_vecCutInSkinList.clear();
}

KGCCutInSkinManager::~KGCCutInSkinManager()
{
}

void KGCCutInSkinManager::SetCutInSkinList()
{
    if ( !m_vecCutInSkinList.empty() )
		return;

	KLuaManager luaMgr;
	if ( GCFUNC::LoadLuaScript( luaMgr, "initcustomcutin.lua" ) == false )
		return;

	luaMgr.BeginTable( "CutInSkinTable" );
	{
		for ( int i = 1; ; ++i )
		{
			if ( FAILED( luaMgr.BeginTable( i ) ) ) break;
			{
				CutInSkinInfo TempCutIn;
				LUA_GET_VALUE_DEF( "ItemID",	TempCutIn.m_iItemID,		 0 );
                LUA_GET_VALUE_DEF( "Texture",	TempCutIn.m_sCutInSkinFace,	"" );
				LUA_GET_VALUE_DEF( "Type",		TempCutIn.m_iType,			 0 );
				LUA_GET_VALUE_DEF( "IsBuy",		TempCutIn.m_bIsBuy,		  false);
				m_vecCutInSkinList.emplace_back( TempCutIn );
			}
			luaMgr.EndTable();
		}
	}
	luaMgr.EndTable();
}

std::string KGCCutInSkinManager::GetCutInSkinTexture( const int iPlayerID )
{
	for ( auto& CutIn : m_vecCutInSkinList )
		if ( g_MyD3D->MyPlayer[ iPlayerID ]->CheckEquipItem( CutIn.m_iItemID / 10, true ) )
			return CutIn.m_sCutInSkinFace;
	return g_MyD3D->MyPlayer[iPlayerID]->GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName;
}

std::string KGCCutInSkinManager::GetCutInSkinTextureID( const int iItemID )
{
	for ( auto& CutIn : m_vecCutInSkinList )
		if( ( CutIn.m_iItemID / 10 ) == iItemID )
			return CutIn.m_sCutInSkinFace;
	return "";
}

void KGCCutInSkinManager::UnEquipCutInskin(SCharInfo* pCharInfo, bool bUpdate)
{
	if ( !pCharInfo )
		return;
	for ( auto& element : m_vecCutInSkinList )
		g_pItemMgr->UnequipItemForItemID( pCharInfo, element.m_iItemID / 10 );
	if ( bUpdate )
		g_pkUIScene->m_pkCustomizationDlg->UpdateSubWindow( 2 );
}

CutInSkinInfo KGCCutInSkinManager::GetCurrentCutInSkin(GCITEMID m_iItemID)
{
	CutInSkinInfo TempFont;
	TempFont.m_iItemID = 0;
	TempFont.m_iType = 0;
	TempFont.m_sCutInSkinFace = "";
	TempFont.m_bIsBuy = false;
	for (auto& element : m_vecCutInSkinList)
		if (element.m_iItemID == m_iItemID)
			return element;
	return TempFont;
}

bool KGCCutInSkinManager::EquipCutInSkin(SCharInfo* pCharInfo, int iItemID)
{
	KItem* pInventoryItem = NULL;
	if (g_pItemMgr->FindInventoryForItemID(iItemID / 10, &pInventoryItem))
	{
		if (pCharInfo->vecItemInfo.size() < MAX_EQUIP_NUM)
		{
			UnEquipCutInskin(pCharInfo, false);
			SUserItem _Item;
			_Item.dwItemUID = pInventoryItem->m_ItemUID;
			_Item.iItemID = pInventoryItem->m_ItemID;
			pCharInfo->vecItemInfo.push_back(_Item);
			g_pkUIScene->m_pkCustomizationDlg->UpdateSubWindow(2);
			return true;
		}
		else if (g_pkUIScene != NULL)
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1), g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
	}
	return false;
}