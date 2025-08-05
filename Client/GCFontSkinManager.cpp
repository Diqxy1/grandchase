#include "stdafx.h"
#include "GCItem.h"
#include "Procedure.h"
#include "KSingleton.h"
#include "GCItemManager.h"
#include "GCGlobalFunction.h"
#include "GCFontSkinManager.h"
#include "GCUI\GCCustomizarionDlg.h"

KGCFontSkinManager* g_pFontSkinMgr = NULL;

KGCFontSkinManager::KGCFontSkinManager()
{
    g_pFontSkinMgr = this;
    m_vecFontSkinList.clear();
}

KGCFontSkinManager::~KGCFontSkinManager()
{
}

void KGCFontSkinManager::SetFontSkinList()
{
    if ( !m_vecFontSkinList.empty() )
		return;
	KLuaManager luaMgr;
	if ( GCFUNC::LoadLuaScript( luaMgr, "InitFontSkin.lua" ) == false )
		return;
	luaMgr.BeginTable( "FontSkinTable" );
	{
		for (int i = 1; ; ++i)
		{
			if ( FAILED( luaMgr.BeginTable( i ) ) ) break;
			{
				FontSkinInfo TempFont;
				LUA_GET_VALUE_DEF("IsBuy",						 TempFont.m_bIsBuy,					 false);
				LUA_GET_VALUE_DEF("FontSkinID",					 TempFont.m_iItemID,					 0);
				LUA_GET_VALUE_DEF("FontSkinType",				 TempFont.m_iType,						 0);
                LUA_GET_VALUE_DEF("FontSkinParticle",			 TempFont.m_sFontParticle,				"");
				LUA_GET_VALUE_DEF("FontSkinExtraParticle",		 TempFont.m_sFontExtraParticle,			"");
				LUA_GET_VALUE_DEF("FontSkinExtraNumberParticle", TempFont.m_sFontExtraNumberParticle,	"");
				m_vecFontSkinList.emplace_back(TempFont);
			}
			luaMgr.EndTable();
		}
	}
	luaMgr.EndTable();
}

std::string KGCFontSkinManager::GetFontSkinParticle( int iPlayerID )
{
	for ( auto& element : m_vecFontSkinList )
		if ( g_MyD3D->MyPlayer[iPlayerID]->CheckEquipItem( element.m_iItemID / 10 ) )
			return element.m_sFontParticle;
	return "DamageTextCritical_";
}

std::string KGCFontSkinManager::GetExtraNumberFontSkinParticle( int iPlayerID )
{
	if ( !g_MyD3D->IsPlayerIndex( iPlayerID ) )
		return "";
	for (auto& element : m_vecFontSkinList)
		if (g_MyD3D->MyPlayer[iPlayerID]->CheckEquipItem(element.m_iItemID / 10))
			return element.m_sFontExtraNumberParticle;
	return "";
}

std::string KGCFontSkinManager::GetExtraFontSkinParticle( int iPlayerID )
{
	if (!g_MyD3D->IsPlayerIndex(iPlayerID))
		return "";
	for (auto& element : m_vecFontSkinList)
		if (g_MyD3D->MyPlayer[iPlayerID]->CheckEquipItem(element.m_iItemID / 10))
			return element.m_sFontExtraParticle;
	return "";
}

void KGCFontSkinManager::UnEquipFontskin( SCharInfo* pCharInfo, bool bUpdate )
{
	if ( !pCharInfo )
		return;
	for ( auto& element : pCharInfo->vecItemInfo )
		if ( GetCurrentFontSkin( element.iItemID * 10 ).m_iItemID > 0 )
			g_pItemMgr->UnequipItemForItemID( pCharInfo, element.iItemID );
	if( bUpdate )
		g_pkUIScene->m_pkCustomizationDlg->UpdateSubWindow( 0 );
}

FontSkinInfo KGCFontSkinManager::GetCurrentFontSkin( GCITEMID m_iItemID )
{
    FontSkinInfo TempFont;
    for ( auto & element : m_vecFontSkinList )
        if ( element.m_iItemID == m_iItemID )
            return element;
    return TempFont;
}

bool KGCFontSkinManager::EquipFontSkin( SCharInfo* pCharInfo, int iItemID )
{
	KItem* pInventoryItem = NULL;
	if ( g_pItemMgr->FindInventoryForItemID( iItemID / 10, &pInventoryItem ) )
	{
		if ( pCharInfo->vecItemInfo.size() < MAX_EQUIP_NUM )
		{
			UnEquipFontskin( pCharInfo, false );
			SUserItem _Item;
			_Item.dwItemUID = pInventoryItem->m_ItemUID;
			_Item.iItemID = pInventoryItem->m_ItemID;
			pCharInfo->vecItemInfo.push_back( _Item );
			g_pkUIScene->m_pkCustomizationDlg->UpdateSubWindow( 0 );
			return true;
		}
		else if ( g_pkUIScene != NULL )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NO_MORE_EQUIP1 ), g_pkStrLoader->GetString( STR_ID_NO_MORE_EQUIP2 ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
	}
    return false;
}