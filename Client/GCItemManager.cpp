#include "stdafx.h"
#include <CommDlg.h>
/// @file GCItemManager.cpp
/// @author pch413@kogstudios.com
/// @since 2004-09-07 ¿ÀÈÄ 4:39:13

#pragma warning(disable:4786)

#include "GCItemManager.h"
#include "GCUI/GCCashShopScene.h"
#include "GCUI/GCGPShopScene.h"
#include "MyD3D.h"
#include "Procedure.h"
#include "PetAI.h"
#include "KGCRoomManager.h"
#include "EnchantStrong.h"
#include "KSingleton.h"
#include "gcui/GCMyInfoScene.h"
#include "gcui/KGCTitleWindow.h"
#include "../GCStateMachine.h"
#include "GCShortCutItem.h"
#include "GCSKT.h"
#include "gcui/GCGameOverlayUI.h"
#include "GCCoupleSystem.h"
#include "GCEventManager.h"
#include "KGCMultipleLanguages.h"
#include "KGCAgitGameManager.h"
#include "KGCDepotManager.h"
#include "GCUI/GCBuyCashItemChooseCashTypeDlg.h"
#include "KGCVirtualDepotManager.h"
#include "GCUI/GCPresentCashItemWnd.h"
#include "GCUI/GCCustomizarionDlg.h"

KGCItemManager* g_pItemMgr = NULL;

#if defined( NATION_USA )
#define NONESETITEM_COMPONENT ( ESP_SET_ITEM|ESP_WEAPON|ESP_A_UPPER_HELMET|ESP_A_DOWN_HELMET|ESP_A_NECKLET|ESP_A_JACKET|ESP_A_PANTS|ESP_A_ARM|ESP_A_FINGER|ESP_A_SHOES|ESP_A_EARRING )
#else
#define NONESETITEM_COMPONENT ( ESP_SET_ITEM|ESP_MANTLE|ESP_WEAPON|ESP_A_UPPER_HELMET|ESP_A_DOWN_HELMET|ESP_A_NECKLET|ESP_A_JACKET|ESP_A_PANTS|ESP_A_ARM|ESP_A_FINGER|ESP_A_SHOES|ESP_A_EARRING )
#endif

//------------------------------------------------------------------------
//	20091026 [±¤Àå ¼¼Æ® ¾ÆÀÌÅÛ ¸ð¼Ç ÀÛ¾÷] : ÀÌµ¿¼®
#include "GCGraphicsHelper.h"						//	enum
#include "square\\gcsquarepeoplestatefunctions.h"	//	¸ð¼Ç ÇÔ¼ö¸¦ ´ãÀ» ¸Ê ÄÁÅ×ÀÌ³Ê
#include "square\\gcsquarepeoplecommon.h"			//	¸ð¼Ç ÇÔ¼ö
#include "square\\gcsquarepeopleelesis.h"			//	¸ð¼Ç ÇÔ¼ö

//#	ÀÏ¹Ý ¸ð¼Ç ÇÔ¼ö
#define SETSQUAREFUNC(p,q) do													\
{																				\
	GCSquarePeopleStateFunctions::ms_mapStateFunction[p] = q;					\
} while(0)

//#	ÀÚ½Å ÀÌ¿ÜÀÇ Á¢¼ÓµÈ À¯ÀúµéÀÇ ¸ð¼Ç ÇÔ¼ö
#define SETSQUAREFUNCREMOTE(p,q) do												\
{																				\
	GCSquarePeopleStateFunctions::ms_mapStateFunctionRemote[p] = q;				\
} while(0)
//========================================================================

#include "GCPetMagicStone.h"

std::wstring KInventoryItem::GetRemainTime()
{
	if (CTime(m_tEndDate + g_kGlobalValue.m_tmServerSyncTime) < g_kGlobalValue.m_tmServerTime)
	{
		return g_pkStrLoader->GetString(STR_ID_TIME_EXPIRED);
	}

	CTimeSpan cSpan = CTime(m_tEndDate + g_kGlobalValue.m_tmServerSyncTime) - g_kGlobalValue.m_tmServerTime;

	if (cSpan.GetDays() == 0)
	{
		if (cSpan.GetHours() == 0)
			return g_pkStrLoader->GetReplacedString(STR_ID_TIME_MINUIT, "i", (int)cSpan.GetMinutes());

		return g_pkStrLoader->GetReplacedString(STR_ID_TIME_HOUR_MINUIT, "ii", (int)cSpan.GetHours(), (int)cSpan.GetMinutes());

		// xºÐ

	}

	// xÀÏ x½Ã°£
	//CString strTime = cSpan.Format( "%DÀÏ %H½Ã°£" );
	return g_pkStrLoader->GetReplacedString(STR_ID_TIME_DAY_HOUR, "ii", (int)cSpan.GetDays(), (int)cSpan.GetHours());
}

class KItemSortTypeGrade
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->cItemGrade > g_pItemMgr->GetItemData(q)->cItemGrade;
	}
};

class KItemSortSlotPosition
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->dwSlotPosition < g_pItemMgr->GetItemData(q)->dwSlotPosition;
	}
};

class KClosetSortType
{
public:
	bool operator()(const std::pair<int, int>& p, const std::pair<int, int>& q) const
	{
		if (p.second == q.second)
			return p.first > q.first;
		return p.second > q.second;
	}
};

class KItemCurrentCharLevel
{
	int m_nLv;
public:
	KItemCurrentCharLevel(int nLv_) : m_nLv(nLv_) {}
	bool operator()(const DWORD p, const DWORD q)const
	{
		int nLeft_ = g_pItemMgr->GetItemData(p)->iNeedLevelForEquippingItem;
		int nRight_ = g_pItemMgr->GetItemData(q)->iNeedLevelForEquippingItem;

		if (m_nLv > nLeft_ && m_nLv > nRight_) {
			if (nLeft_ > nRight_) return true;
			return false;
		}
		else if (m_nLv < nLeft_ && m_nLv < nRight_) {
			if (nLeft_ < nRight_) return true;
			return false;
		}

		if (m_nLv == nLeft_ && m_nLv == nRight_) {
			return false;
		}

		if (m_nLv == nLeft_) {
			return true;
		}

		if (m_nLv == nRight_) {
			return false;
		}

		if (nLeft_ < nRight_)
			return true;

		return false;
	}
};

class KItemSortNewItem
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return (g_pItemMgr->GetItemRecomData(p) & NEW_MARK_MASK) > (g_pItemMgr->GetItemRecomData(q) & NEW_MARK_MASK);
	}
};

class KItemSortHighItemID
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->dwGoodsID > g_pItemMgr->GetItemData(q)->dwGoodsID;
	}
};

class KItemSortLevelHigh
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->iNeedLevelForEquippingItem > g_pItemMgr->GetItemData(q)->iNeedLevelForEquippingItem;
	}
};

class KItemSortLevelLow
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->iNeedLevelForEquippingItem < g_pItemMgr->GetItemData(q)->iNeedLevelForEquippingItem;
	}
};

class KItemSortName
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return wcscoll(g_pItemMgr->GetItemData(p)->strItemName.c_str(), g_pItemMgr->GetItemData(q)->strItemName.c_str()) < 0;
	}
};

class KItemSortRecom
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return (g_pItemMgr->GetItemRecomData(p) & SORT_MARK_MASK) < (g_pItemMgr->GetItemRecomData(q) & SORT_MARK_MASK);
	}
};

class KItemSortKind
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->eItemKind > g_pItemMgr->GetItemData(q)->eItemKind;
	}
};

class KItemSortItemTypeValue
{
public:
	bool operator()(const GCItem* src_, const GCItem* dest_)const
	{
		if (src_->iItemTypeValue == -1)
			return false;
		if (dest_->iItemTypeValue == -1)
			return true;
		return src_->iItemTypeValue < dest_->iItemTypeValue;
	}
};

class KItemSortLowItemID
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return g_pItemMgr->GetItemData(p)->dwGoodsID < g_pItemMgr->GetItemData(q)->dwGoodsID;
	}
};

class KItemSortLowLevelLowItemIDHighRecom
{
public:
	bool operator()(const DWORD p, const DWORD q)const
	{
		return (g_pItemMgr->GetItemRecomData(p) & SORT_MARK_MASK) < (g_pItemMgr->GetItemRecomData(q) & SORT_MARK_MASK);
	}
};

KGCItemManager::KGCItemManager(void)
{
	g_pItemMgr = this;

	// old data field initialization
	shMyAttack = 0;
	shMyDefence = 0;
	shMyHpInc = 0;

	// data field initialization
	m_dwNumItem = 0;

	// map ÃÊ±âÈ­.
	m_mapDB.clear();
	m_mapCashShop.clear();
	_wsetlocale(LC_COLLATE, L".OCP");
	m_mapPTypeCheckPool.clear();

	for (int i = 0; i < (int)m_aStringColorTable.size(); ++i)
	{
		m_aStringColorTable[i] = 0xffffffff;
	}

	m_aStringColorTable[GIK_NORMAL] = 0xffffffff;
	m_aStringColorTable[GIK_STRONG_ITEM] = 0xffffff00;
	m_aStringColorTable[GIK_STRONG_ITEM_NOT_STRONG] = 0xffffff00;
	m_aStringColorTable[GIK_MISSION_SCROLL] = 0xffC053DB;
	m_aStringColorTable[GIK_PET] = 0xffffffff;
	m_aStringColorTable[GIK_POINT_ITEM] = 0xffffffff;
	m_aStringColorTable[GIK_ENCHANT_ASIST_ITEM] = 0xffC053DB;
	m_aStringColorTable[GIK_ENCHANT_ENABLE_ITEM] = 0xffC053DB;
	m_aStringColorTable[GIK_ENCHANT_CORE] = 0xffC053DB;
	m_aStringColorTable[GIK_RECIPE] = 0xff159600;
	m_aStringColorTable[GIK_STUFF] = 0xff159600;
	m_aStringColorTable[GIK_BOSS_RARE] = 0xffffff00;
	m_aStringColorTable[GIK_EQUIP] = 0xffffffff;
	m_aStringColorTable[GIK_EQUIP_RARE] = 0xffffff00;
	m_aStringColorTable[GIK_MISSION_STUFF] = 0xffC053DB;
	m_aStringColorTable[GIK_SHORTCUT_SLOT] = 0xffffffff;
	m_aStringColorTable[GIK_OPEN_UP_ITEM] = 0xffffffff;
	m_aStringColorTable[GIK_TREASUREBOX_ITEM] = 0xffffffff;
	m_aStringColorTable[GIK_MONSTER_CARD] = 0Xffffffff;

	m_dwHotItemType = EM_HOT_ITEM_NONE_TYPE;

	//¾ÆÀÌÅÛÀÌ ¾î¶² Ä«Å×°í¸®¿¡ ¼ÓÇÏ´ÂÁö ¾Ë±â À§ÇØ.. GCUserItemBox¿¡ ÀÖÀ½..
	m_dwItemCategoryFlag[UEC_EQUIP1] = ESP_HELMET | ESP_JACKET | ESP_PANTS | ESP_GLOVES | ESP_SHOES | ESP_MANTLE | ESP_WEAPON | ESP_HELMET_SOCKET | ESP_WEAPON_SOCKET | ESP_JACKET_SOCKET;

	m_dwItemCategoryFlag[UEC_EQUIP2] = ESP_A_FINGER | ESP_A_JACKET | ESP_A_PANTS | ESP_A_UPPER_HELMET | ESP_A_DOWN_HELMET |
		ESP_A_ARM | ESP_A_SHOES | ESP_A_NECKLET | ESP_A_HELMET_SOCKET | ESP_A_EARRING;

	m_dwItemCategoryFlag[UEC_PET] = EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM | EPET_EQUIP_ITEM_SLOT_SKILL;

	m_dwItemCategoryFlag[UEC_SHORTCUT_SLOT] = ESP_SLOT_ITEM | ESP_SLOT_EMOT;

	m_dwItemCategoryFlag[UEC_SKILL] = ESP_SKC_WHITE_ONE | ESP_SKC_WHITE_TWO | ESP_SKC_WHITE_THREE |
		ESP_SKC_WHITE_CHANGEJOB_ONE | ESP_SKC_WHITE_CHANGEJOB_TWO | ESP_SKC_WHITE_CHANGEJOB_THREE |
		ESP_SKC_WHITE_CHANGEJOB2_ONE | ESP_SKC_WHITE_CHANGEJOB2_TWO | ESP_SKC_WHITE_CHANGEJOB2_THREE;

	m_dwItemCategoryFlag[UEC_SKILL_WHITE] = ESP_SKC_WHITE_ONE | ESP_SKC_WHITE_TWO | ESP_SKC_WHITE_THREE |
		ESP_SKC_WHITE_CHANGEJOB_ONE | ESP_SKC_WHITE_CHANGEJOB_TWO | ESP_SKC_WHITE_CHANGEJOB_THREE |
		ESP_SKC_WHITE_CHANGEJOB2_ONE | ESP_SKC_WHITE_CHANGEJOB2_TWO | ESP_SKC_WHITE_CHANGEJOB2_THREE;


	m_dwItemCategoryFlag[UEC_SKILL_BLACK] = ESP_SKC_DARK_ONE | ESP_SKC_DARK_TWO | ESP_SKC_DARK_THREE |
		ESP_SKC_DARK_CHANGEJOB_ONE | ESP_SKC_DARK_CHANGEJOB_TWO | ESP_SKC_DARK_CHANGEJOB_THREE |
		ESP_SKC_DARK_CHANGEJOB2_ONE | ESP_SKC_DARK_CHANGEJOB2_TWO | ESP_SKC_DARK_CHANGEJOB2_THREE;

	m_mapTradeItem.clear();

	m_SocketChangeItemID = 0;

	// Kawan> default items
#ifdef NEW_3D_BASE // Novos Modelos
	m_vecBaseItem.push_back(163031); m_vecBaseItem.push_back(163032); m_vecBaseItem.push_back(163033); m_vecBaseItem.push_back(163034); m_vecBaseItem.push_back(163035); m_vecBaseItem.push_back(163036); m_vecBaseItem.push_back(163157);  //Elesis
	m_vecBaseItem.push_back(163037); m_vecBaseItem.push_back(163038); m_vecBaseItem.push_back(163039); m_vecBaseItem.push_back(163040); m_vecBaseItem.push_back(163041); m_vecBaseItem.push_back(163042); m_vecBaseItem.push_back(163158); //Lire
	m_vecBaseItem.push_back(163043); m_vecBaseItem.push_back(163044); m_vecBaseItem.push_back(163045); m_vecBaseItem.push_back(163046); m_vecBaseItem.push_back(163047); m_vecBaseItem.push_back(163048); m_vecBaseItem.push_back(163159); //Arme
	m_vecBaseItem.push_back(163049); m_vecBaseItem.push_back(163050); m_vecBaseItem.push_back(163051); m_vecBaseItem.push_back(163052); m_vecBaseItem.push_back(163053); m_vecBaseItem.push_back(163054); m_vecBaseItem.push_back(163160); //Las
	m_vecBaseItem.push_back(163055); m_vecBaseItem.push_back(163056); m_vecBaseItem.push_back(163057); m_vecBaseItem.push_back(163058); m_vecBaseItem.push_back(163059); m_vecBaseItem.push_back(163060); m_vecBaseItem.push_back(163161); //Ryan
	m_vecBaseItem.push_back(163061); m_vecBaseItem.push_back(163062); m_vecBaseItem.push_back(163063); m_vecBaseItem.push_back(163064); m_vecBaseItem.push_back(163065); m_vecBaseItem.push_back(163066); m_vecBaseItem.push_back(163162); //Ronan
	m_vecBaseItem.push_back(163067); m_vecBaseItem.push_back(163068); m_vecBaseItem.push_back(163069); m_vecBaseItem.push_back(163070); m_vecBaseItem.push_back(163071); m_vecBaseItem.push_back(163072); m_vecBaseItem.push_back(163163); //Amy
	m_vecBaseItem.push_back(163073); m_vecBaseItem.push_back(163074); m_vecBaseItem.push_back(163075); m_vecBaseItem.push_back(163076); m_vecBaseItem.push_back(163077); m_vecBaseItem.push_back(163078); m_vecBaseItem.push_back(163164); //Jin
	m_vecBaseItem.push_back(163079); m_vecBaseItem.push_back(163080); m_vecBaseItem.push_back(163081); m_vecBaseItem.push_back(163082); m_vecBaseItem.push_back(163083); m_vecBaseItem.push_back(163084); m_vecBaseItem.push_back(163165); //Sieghart
	m_vecBaseItem.push_back(163085); m_vecBaseItem.push_back(163086); m_vecBaseItem.push_back(163087); m_vecBaseItem.push_back(163088); m_vecBaseItem.push_back(163089); m_vecBaseItem.push_back(163090); m_vecBaseItem.push_back(163166); //Mari
	m_vecBaseItem.push_back(163091); m_vecBaseItem.push_back(163092); m_vecBaseItem.push_back(163093); m_vecBaseItem.push_back(163094); m_vecBaseItem.push_back(163095); m_vecBaseItem.push_back(163096); m_vecBaseItem.push_back(163167); //Dio
	m_vecBaseItem.push_back(163097); m_vecBaseItem.push_back(163098); m_vecBaseItem.push_back(163099); m_vecBaseItem.push_back(163100); m_vecBaseItem.push_back(163101); m_vecBaseItem.push_back(163102); m_vecBaseItem.push_back(163168); //Zero
	m_vecBaseItem.push_back(163103); m_vecBaseItem.push_back(163104); m_vecBaseItem.push_back(163105); m_vecBaseItem.push_back(163106); m_vecBaseItem.push_back(163107); m_vecBaseItem.push_back(163108); m_vecBaseItem.push_back(163169); //Rey
	m_vecBaseItem.push_back(163109); m_vecBaseItem.push_back(163110); m_vecBaseItem.push_back(163111); m_vecBaseItem.push_back(163112); m_vecBaseItem.push_back(163113); m_vecBaseItem.push_back(163114); m_vecBaseItem.push_back(163170); //Rufus
	m_vecBaseItem.push_back(163115); m_vecBaseItem.push_back(163116); m_vecBaseItem.push_back(163117); m_vecBaseItem.push_back(163118); m_vecBaseItem.push_back(163119); m_vecBaseItem.push_back(163120); m_vecBaseItem.push_back(163171); //Rin
	m_vecBaseItem.push_back(163121); m_vecBaseItem.push_back(163122); m_vecBaseItem.push_back(163123); m_vecBaseItem.push_back(163124); m_vecBaseItem.push_back(163125); m_vecBaseItem.push_back(163126); m_vecBaseItem.push_back(163172); //Asin
	m_vecBaseItem.push_back(163127); m_vecBaseItem.push_back(163128); m_vecBaseItem.push_back(163129); m_vecBaseItem.push_back(163130); m_vecBaseItem.push_back(163131); m_vecBaseItem.push_back(163132); m_vecBaseItem.push_back(163173); //Lime
	m_vecBaseItem.push_back(163133); m_vecBaseItem.push_back(163134); m_vecBaseItem.push_back(163135); m_vecBaseItem.push_back(163136); m_vecBaseItem.push_back(163137); m_vecBaseItem.push_back(163138); m_vecBaseItem.push_back(163174); //Edel
	m_vecBaseItem.push_back(163139); m_vecBaseItem.push_back(163140); m_vecBaseItem.push_back(163141); m_vecBaseItem.push_back(163142); m_vecBaseItem.push_back(163143); m_vecBaseItem.push_back(163144); m_vecBaseItem.push_back(163175); //Beigas
	m_vecBaseItem.push_back(163145); m_vecBaseItem.push_back(163146); m_vecBaseItem.push_back(163147); m_vecBaseItem.push_back(163148); m_vecBaseItem.push_back(163149); m_vecBaseItem.push_back(163150); m_vecBaseItem.push_back(163176); //Uno
	m_vecBaseItem.push_back(163145); m_vecBaseItem.push_back(163146); m_vecBaseItem.push_back(163147); m_vecBaseItem.push_back(163148); m_vecBaseItem.push_back(163149); m_vecBaseItem.push_back(163150); m_vecBaseItem.push_back(163176); //Uno
	//m_vecBaseItem.push_back(163151); m_vecBaseItem.push_back(163152); m_vecBaseItem.push_back(163153); m_vecBaseItem.push_back(163154); m_vecBaseItem.push_back(163155); m_vecBaseItem.push_back(163156); m_vecBaseItem.push_back(163177); //Peryton
#else
	m_vecBaseItem.push_back(0);   m_vecBaseItem.push_back(1);   m_vecBaseItem.push_back(2);   m_vecBaseItem.push_back(3);
	m_vecBaseItem.push_back(5);   m_vecBaseItem.push_back(6);   m_vecBaseItem.push_back(7);   m_vecBaseItem.push_back(8);
	m_vecBaseItem.push_back(10);   m_vecBaseItem.push_back(11);   m_vecBaseItem.push_back(12);   m_vecBaseItem.push_back(13);
	m_vecBaseItem.push_back(1858);   m_vecBaseItem.push_back(1859);   m_vecBaseItem.push_back(1860);   m_vecBaseItem.push_back(1861);
	m_vecBaseItem.push_back(3193);   m_vecBaseItem.push_back(3194);   m_vecBaseItem.push_back(3195);   m_vecBaseItem.push_back(3196);
	m_vecBaseItem.push_back(5521);   m_vecBaseItem.push_back(5522);   m_vecBaseItem.push_back(5523);   m_vecBaseItem.push_back(5524);
	m_vecBaseItem.push_back(8176);   m_vecBaseItem.push_back(8177);   m_vecBaseItem.push_back(8178);   m_vecBaseItem.push_back(8179);
	m_vecBaseItem.push_back(11978);   m_vecBaseItem.push_back(11979);   m_vecBaseItem.push_back(11980);   m_vecBaseItem.push_back(11981);
	m_vecBaseItem.push_back(15873);   m_vecBaseItem.push_back(15874);   m_vecBaseItem.push_back(15875);   m_vecBaseItem.push_back(15876);
	m_vecBaseItem.push_back(21634);   m_vecBaseItem.push_back(21635);   m_vecBaseItem.push_back(21636);   m_vecBaseItem.push_back(21637);
	m_vecBaseItem.push_back(34178);   m_vecBaseItem.push_back(34179);   m_vecBaseItem.push_back(34180);   m_vecBaseItem.push_back(34181);
	m_vecBaseItem.push_back(44794);   m_vecBaseItem.push_back(44795);   m_vecBaseItem.push_back(44796);   m_vecBaseItem.push_back(44797);
	m_vecBaseItem.push_back(52196);   m_vecBaseItem.push_back(52197);   m_vecBaseItem.push_back(52198);   m_vecBaseItem.push_back(52315);
	m_vecBaseItem.push_back(69396);   m_vecBaseItem.push_back(69397);   m_vecBaseItem.push_back(69398);   m_vecBaseItem.push_back(69697);
	m_vecBaseItem.push_back(70835);   m_vecBaseItem.push_back(70836);   m_vecBaseItem.push_back(70837);   m_vecBaseItem.push_back(71136);
	m_vecBaseItem.push_back(86303);   m_vecBaseItem.push_back(86304);   m_vecBaseItem.push_back(86305);   m_vecBaseItem.push_back(86598);
	m_vecBaseItem.push_back(93074);   m_vecBaseItem.push_back(93075);   m_vecBaseItem.push_back(93076);   m_vecBaseItem.push_back(93827);
	m_vecBaseItem.push_back(115836);   m_vecBaseItem.push_back(115837);   m_vecBaseItem.push_back(115838);   m_vecBaseItem.push_back(117133);
	m_vecBaseItem.push_back(123971);   m_vecBaseItem.push_back(123972);   m_vecBaseItem.push_back(123973);   m_vecBaseItem.push_back(123974);
	m_vecBaseItem.push_back(149585);   m_vecBaseItem.push_back(149586);   m_vecBaseItem.push_back(149587);   m_vecBaseItem.push_back(150739);
	m_vecBaseItem.push_back(172051);   m_vecBaseItem.push_back(172052);   m_vecBaseItem.push_back(172053);   m_vecBaseItem.push_back(172054);
	m_vecBaseItem.push_back(180322);   m_vecBaseItem.push_back(180323);   m_vecBaseItem.push_back(180324);   m_vecBaseItem.push_back(180325);
	m_vecBaseItem.push_back(219025);   m_vecBaseItem.push_back(219026);   m_vecBaseItem.push_back(219027);   m_vecBaseItem.push_back(219028);
#endif

	pSort = NULL;

	InitEnchantItemStatTable();

	m_mapInvenBuffItem.clear();
	m_mapChannelItemList.clear();
	m_vecExpItemList.clear();

	m_mapEquipDisableItem.clear();

	m_bItemBuyInfo = false;

	ClearPackageInfo();

	m_dwCurrentGoodsID = 0;

	m_iPackageIndexMax = 0;
	m_iPackageCurrendIndex = 0;
	m_bIsPackagInfo = false;
	m_bIsInPresentState = false;
}

void KGCItemManager::LoadItemData()
{
	m_iMaxCoordiInvenSize = 200;


	LoadAttributeTable();
	LoadSocketTable();
	LoadGradeTable();
	LoadItemDataFile();
	LoadTableScript();
	LoadSetItemList();
	LoadCoordiSeasonList();
	LoadCoordiSetItemList();

#ifdef ENABLE_POINT_SYSTEM // Kawan>
	LoadPointItemList();
#endif

	LoadNormalTitleItem();
	LoadMonsterCardManager();
	LoadItemAbilityCalcTable();
	LoadItemAbilitCalcExceptionList();
	LoadBuyLimitItem();
	LoadExceptionConuntItem();
	LoadHeroItemSpecialAbility();

#if defined EVENT_ITEM_CHANGE
	LoadEventChangeItemList();
#endif

#if defined EVENT_VIEW_ITEM
	LoadEventViewItemList();
#endif

	LoadGachaList();

	InitNonCountItemSize();

	LoadBaseCoordiItem(); // Kawan> Keep

#if defined BUY_CASH_ITEM_WITH_POINT
	LoadPointShopValidItemList();
#endif
	LoadRemovePartsItem();
	LoadChannelItemList();

	LoadEquipDisableItemList();
	LoadDepotNonvisibelItem();
	//LoadCashCharacterPackageItemScript();

	if (GetItemBuyInfo())
	{//ENU_ITEM_BUY_INFO_NOT°¡ ·ÎµåÀü¿¡ ÀÌ¹Ì µµÂøÇÏ¿© ÀúÀåÇØ³õÀº µ¥ÀÌÅÍ·Î °»½ÅÇØÁØ´Ù
		SetItemBuyInfoData(m_KItemBuyInfoData);
	}
	else
	{//ENU_ITEM_BUY_INFO_NOT°¡ µµÂøÇÏ¸é °»½ÅÇÒ ¼ö ÀÖµµ·Ï true·Î º¯°æ
		SetItemBuyInfo(true);
	}

	LoadSkillScroolList();
}


KGCItemManager::~KGCItemManager(void)
{
	m_SetList.clear();
	m_vecCoordiSensonList.clear();
	UnloadItemDataFile();
	m_kInventory.ClearItem();
	m_mapPTypeCheckPool.clear();
	g_pItemMgr = NULL;
}

GCItem* KGCItemManager::GetItemData(GCITEMID dwItemID)
{
	// °°Àº ¾ÆÀÌÅÛÀ» ¿¬¼ÓÀûÀ¸·Î ¿äÃ»ÇÏ´Â °æ¿ì°¡ ¸¹´õ¶ó.
	// ¶È°°Àº ¾ÆÀÌµð¸¦ ¿¬¼ÓÀ¸·Î ¿äÃ»ÇÏ¸é ¹Ù·Î ÁÖÀÚ.
	static GCItem** ppReserve = NULL;
	if (ppReserve != NULL) {
		if (*ppReserve != NULL) {
			if ((*ppReserve)->dwGoodsID == dwItemID)
				return *ppReserve;
		}
	}

	stdext::hash_map<GCITEMID, GCItem*>::iterator itor = m_mapDB.find(dwItemID);

	if (m_mapDB.end() == itor) {
		//ASSERT( !"This is not in sort script!" );
		return NULL;
	}

	ppReserve = &itor->second;

	return itor->second;
}

DWORD KGCItemManager::ItemCharTypeToItemFlag(int iCharType)
{
	// NOTE : Ä³¸¯ÅÍ Å¸ÀÔÀº °¢°¢ 0, 1, 2ÀÌ°í ÇÃ·¡±×´Â 1, 2, 4ÀÌ´Ù.
	return 1 << iCharType;
}

void KGCItemManager::LoadTableScript(std::string strFileName_, map_same_image_index_type_type& mapBuf, bool div)
{
	mapBuf.clear();
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, strFileName_.c_str()) != false)
	{
		LUA_BEGIN_TABLE("Table", return;)
		{
			for (int i = 1; ; i++) {
				int GoodsID, iID;
				GoodsID = 0;
				iID = 0;
				LUA_BEGIN_TABLE(i, break)
				{
					LUA_GET_VALUE_NOASSERT(1, GoodsID,/*IS_ACTIVE_EVENT[i]*/break);
					LUA_GET_VALUE_NOASSERT(2, iID,/*IS_ACTIVE_EVENT[i]*/break);
					if (GoodsID == 0) break;
					GoodsID /= 10;
					if (div)
					{
						iID /= 10;
					}

					map_same_image_index_type_type::iterator mit = mapBuf.find(iID);
					if (mit != mapBuf.end()) {
						iID = mit->second;
					}

					mapBuf.insert(map_same_image_index_type_type::value_type(GoodsID, iID));
				}
				LUA_END_TABLE(/* i */;);
			}
		}
		LUA_END_TABLE(/* RecomLevel */;);
	}
}

bool KGCItemManager::LoadRecomData(KLuaManager& luaMgr, std::map<DWORD, DWORD>& mapItemRecom)
{
	LUA_BEGIN_TABLE("Table", return false) {
		for (int i = 1; ; ++i) {
			GCITEMID dwItemID = 0;
			DWORD dwRecom = 0;
			LUA_BEGIN_TABLE(i, break) {
				LUA_GET_VALUE_NOASSERT(1, dwItemID, LUA_END_TABLE(break); break);
				LUA_GET_VALUE_NOASSERT(2, dwRecom, LUA_END_TABLE(break); break);
			} LUA_END_TABLE(break);

			mapItemRecom.insert(std::make_pair(dwItemID, dwRecom));
		}
	} LUA_END_TABLE(return false);

	return true;
}


bool KGCItemManager::LoadRecomFile(std::string strFileName, std::map<DWORD, DWORD>& mapData)
{
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, strFileName.c_str()) == false) {
		return false;
	}

	if (LoadRecomData(luaMgr, mapData) == false) {
		return false;
	}

	return true;
}


bool KGCItemManager::LoadRecomFiles()
{
	for (int i = 0; i < GC_CHAR_NUM; ++i) {
		std::string strRecomFilePath;
		std::map<DWORD, DWORD> mapRecomInfo;

		char szCharNum[10];
		itoa(i, szCharNum, 10);

		strRecomFilePath += "recom";
		strRecomFilePath += szCharNum;
		strRecomFilePath += ".lua";

		if (LoadRecomFile(strRecomFilePath, mapRecomInfo) == false)
			return false;

		m_mapItemRecomLevel.insert(std::make_pair(i, mapRecomInfo));
	}

	return true;
}


DWORD KGCItemManager::GetItemRecomData(GCITEMID dwItemID)
{
	return g_pItemMgr->GetItemData(dwItemID)->iRecomLevel;
}

bool KGCItemManager::LoadItemDataFile(void)
{
	int iClosetType = 0;
	std::stringstream sTextureTempName;

	m_dwNumItem = 0;

	std::string strItemDataFileName("Sort.stg");
	GCMemoryFile MemFile(g_pGCDeviceManager2->GetMassFileManager(), strItemDataFileName);

	if (!MemFile.IsLoaded())
	{
		return false;
	}

	KLuaManager luaMgr;

	if (SiKGCMultipleLanguages()->UseMultipleLanguages())
	{
		SiKGCMultipleLanguages()->LoadSortStringFile();
	}

	LoadRecomFiles();
	LoadSameImageIndexScript();
	LoadSameTextureIndexScript();
	m_mapSetItemList.clear();

	char* m_pSortFile = MemFile.GetDataPointer();

	DWORD dwTotalItem = NULL; //ÀüÃ¼ ¾ÆÀÌÅÛ ¼ö(Áßº¹ Æ÷ÇÔÇÏÁö ¾ÊÀ½)
	DWORD dwTotalItemInfo = NULL; //ÀüÃ¼ ¾ÆÀÌÅÛ ¼ö(Áßº¹ Æ÷ÇÔ)
	int iIndex = 0;

	memcpy(&dwTotalItemInfo, m_pSortFile, sizeof(DWORD));
	if (dwTotalItemInfo == NULL)
		return false;

	iIndex += sizeof(DWORD);

	memcpy(&dwTotalItem, m_pSortFile + iIndex, sizeof(DWORD));
	if (dwTotalItem == NULL)
		return false;

	iIndex += sizeof(DWORD);

	m_dwNumItem = dwTotalItem;

	m_dwTotalNumItem = dwTotalItemInfo;

	GCItemData  _ItemData;
	DWORD _ItemKey;

	DWORD dwAllChar = 1;

	for (int i = 1; i < GC_CHAR_NUM; i++)
	{
		dwAllChar = dwAllChar << 1;
		dwAllChar += 1;
	}

	GCItem* pItem = NULL;

	for (DWORD i = 0; i < dwTotalItemInfo; ++i)
	{
		memcpy(&_ItemData, m_pSortFile + iIndex, sizeof(GCItemData));

		iIndex += sizeof(GCItemData);

		_ItemData.dwGoodsID = _ItemData.dwGoodsID / 10;

		pItem = new GCItem();
		*pItem = _ItemData;

		pItem->dwCharType = pItem->dwCharType & dwAllChar;

#ifdef COMMON_MANUAL
		if (pItem->dwCharType == (DWORD)1048575)
			pItem->dwCharType = ECT_ALL;
#endif

#ifndef MERGE_COORDI_SHOP // Kawan>
		// 2011.03.07 ÄÚµð¼¥ ¾ÆÀÌÅÛ ÆÐÅ°Áö ¹øÈ£ ÀúÀå - iNeedBlessingÀÌ »ç¿ëµÇÁö ¾Ê´Â °ÍÀ¸·Î º¸¿© ÄÚµð ¾ÆÀÌÅÛ
		// ¼¼Æ® ¹øÈ£ ÀúÀå¿¡ »ç¿ëÇÕ´Ï´Ù. ¸¸¾à ´Ù¸¥°÷¿¡¼­ »ç¿ëÇÑ´Ù¸é ÄÚµð ÆÐÅ°Áö ¹øÈ£¸¦ ´Ù¸¥°÷¿¡ ÀúÀåÇÏµµ·Ï ÇÕ´Ï´Ù. 
		if (_ItemData.iNeedBlessing != 0 && pItem->eItemKind == GIK_COORDI_ITEM) {
			int itest = static_cast<int>(pItem->eItemKind);
			pItem->iCoordiShopPackage = _ItemData.iNeedBlessing;
			m_mapCoordishopItem[pItem->iCoordiShopPackage].InsertItem(pItem);
		}

		if (pItem->eItemKind == GIK_COORDIBOX) {
			m_mapCoordishopRandomBox.insert(std::pair<GCITEMID, GCItem*>(pItem->dwGoodsID, pItem));
		}
#else
		if (_ItemData.iNeedBlessing != 0 && pItem->eItemKind == GIK_COORDI_ITEM) {
			pItem->iCoordiShopPackage = _ItemData.iNeedBlessing;
		}
#endif

		if (IsMetamorphosisItem(pItem->dwGoodsID))
		{
			// ÀÌ°Å ¸ÓÁÒ? ¿Ö ³Ö´Â °ÅÁÒ?
			//pItem->dwSkillPosition |= ESP_SKC_WHITE_CHANGEJOB2_THREE;

			// ÀÌ°Ç.. ´Á´ë ½ÅÇÊ slotposition °íÄ¡´Â ÄÚµå Á¨Àå;;
			switch (pItem->dwGoodsID)
			{
			case 3225:
			case 3227:
			case 3253:
			case 13713:
			case 13714:
				pItem->dwSkillPosition = ESP_SKC_WHITE_TWO + ESP_SKC_WHITE_CHANGEJOB_TWO + ESP_SKC_WHITE_CHANGEJOB2_TWO;
				break;
			case 3953:
			case 3954:
			case 13715:
				pItem->dwSkillPosition = ESP_SKC_WHITE_CHANGEJOB_THREE + ESP_SKC_WHITE_CHANGEJOB2_THREE;
				break;
			}
		}

		_ItemKey = pItem->dwGoodsID;

		if (pItem->iSetItemNumber != -1)
		{
			std::map<int, std::vector<int>>::iterator itorSetItemList;
			itorSetItemList = m_mapSetItemList.find(pItem->iSetItemNumber);
			if (m_mapSetItemList.find(pItem->iSetItemNumber) == m_mapSetItemList.end())
			{
				std::vector<int> vecItemList;
				vecItemList.clear();
				vecItemList.push_back(_ItemKey);
				m_mapSetItemList.insert(std::make_pair(pItem->iSetItemNumber, vecItemList));
			}
			else
			{
				itorSetItemList->second.push_back(_ItemKey);
			}
		}

		if (SiKGCMultipleLanguages()->UseMultipleLanguages())
		{
			SiKGCMultipleLanguages()->GetItemString(pItem->dwGoodsID, pItem->strItemName, pItem->strItemDesc);
		}

		if (pItem->eMoneyType == EMT_CASH)
		{
			m_mapCashShop[pItem->dwGoodsID].push_back(pItem);
		}

		if (pItem->eItemType == GCIT_COUNT || (pItem->iItemTypeValue == -1 && pItem->eItemType == GCIT_PERIOD))
		{
			m_mapCheapItem[pItem->dwGoodsID] = pItem->dwPrice;

			auto mitDel = m_mapDB.find(_ItemKey);
			if (mitDel != m_mapDB.end())
			{
				m_mapDB.erase(mitDel);
			}

			m_mapDB.insert(std::make_pair(_ItemKey, pItem));
		}
		else
		{
			if (m_mapDB.find(_ItemKey) != m_mapDB.end())
			{
				if (m_mapDB[_ItemKey]->dwPrice < pItem->dwPrice)
				{
					m_mapDB[_ItemKey] = pItem;
				}
				else
				{
					continue;
				}

				if (m_mapCheapItem[pItem->dwGoodsID] > pItem->dwPrice)
					m_mapCheapItem[pItem->dwGoodsID] = pItem->dwPrice;
			}
			else
			{
				m_mapDB.insert(std::make_pair(_ItemKey, pItem));
				m_mapCheapItem[pItem->dwGoodsID] = pItem->dwPrice;
			}
		}

		if (IsClosetItem(pItem))
		{
			if (!g_kGlobalValue.IsItemInVIPList(pItem->dwGoodsID))
			{
				if (pItem->eMoneyType == EMT_CRYSTAL)
					iClosetType = 1;
				else
					iClosetType = 0;
			}
			else
				iClosetType = 2;

			PAIR_INT mTempPairTrue = std::make_pair(pItem->dwGoodsID * 10, iClosetType);
			PAIR_INT mTempPair = std::make_pair(GetSameImageIndex(pItem->dwGoodsID) * 10, iClosetType);
			if (std::find(g_kGlobalValue.CoordiViewVector.begin(), g_kGlobalValue.CoordiViewVector.end(), mTempPair) == g_kGlobalValue.CoordiViewVector.end())
			{
				sTextureTempName.str("");
				sTextureTempName << g_pGCDeviceManager2->GetAbtaName((mTempPair.first / 10), "sbta") << ".dds";
				if (true == g_pGCDeviceManager2->CheckFileExist(sTextureTempName.str().c_str()))
					g_kGlobalValue.CoordiViewVector.emplace_back(mTempPairTrue);
			}
		}

		pItem = NULL;
	}

	if (SiKGCMultipleLanguages()->UseMultipleLanguages())
		SiKGCMultipleLanguages()->ClearSortString();

	std::sort(g_kGlobalValue.CoordiViewVector.begin(), g_kGlobalValue.CoordiViewVector.end(), KClosetSortType());
	g_kGlobalValue.SetCoordiViewItemInfo();

	return true;
}

// bool KGCItemManager::SaveItemDataFile()
// {
//     //Sort ÆÄÀÏÀº ¾ð¾î°¡ ¾Æ´Ñ ±¹°¡º°·Î ºÐ·ù ÇØ¾ß ÇÑ´Ù.
//     WCHAR strFileName[MAX_PATH] = L"";
// 
//     OPENFILENAME ofn;
//     memset( &ofn, 0, sizeof(ofn));
//     ofn.lStructSize = sizeof(OPENFILENAME);
//     ofn.lpstrFile = strFileName;
//     ofn.nMaxFile = MAX_PATH;    
//     ofn.hwndOwner = g_MyD3D->Get_hWnd();
//     ofn.lpstrFilter = TEXT("stgÆÄÀÏ(*.stg)\0*.*\0");
//     if( !GetSaveFileName( &ofn ) )
//         return false;
// 
//     std::string strItemDataFileName = "sort_";
//     strItemDataFileName += GCUTIL_STR::GCStrWideToChar( NATION_CODE );
//     strItemDataFileName += ".stg";
// 
//     
//     FILE *fp = fopen( GCUTIL_STR::GCStrWideToChar(strFileName) ,"wb" );
// 
//     DWORD dwSize = m_mapDB.size();
//     fwrite( &dwSize, sizeof(DWORD) , 1 , fp );
//     fwrite( &dwSize, sizeof(DWORD) , 1 , fp );
// 
//     GCItemData  _ItemData;
// 
//     std::unordered_map< GCITEMID, GCItem* >::iterator mit;   
// 
//     for( mit = m_mapDB.begin(); mit != m_mapDB.end(); mit++ )
//     {
//         _ItemData = *(mit->second);
// 
//         _ItemData.dwGoodsID *= 10;
// 
//         fwrite( &_ItemData, sizeof(_ItemData) , 1 , fp );
//     }
// 
//     fclose( fp );
// 
//     return true;
// }

class KSafeDelete
{
public:
	KSafeDelete() {}
	~KSafeDelete() {}

	template< typename T >
	void operator () (const T* ptr)
	{
		SAFE_DELETE(ptr);
	}
};

void KGCItemManager::UnloadItemDataFile(void)
{
	m_dwNumItem = 0;
	//SAFE_DELETE_ARRAY(m_pCheapItem);

	stdext::hash_map< GCITEMID, GCItem* >::iterator mit2;
	for (mit2 = m_mapDB.begin(); mit2 != m_mapDB.end(); ++mit2)
	{
		if (m_mapCashShop.find(mit2->first) == m_mapCashShop.end())
		{
			SAFE_DELETE(mit2->second);
		}
	}
	m_mapDB.clear();

	stdext::hash_map< DWORD, std::vector<GCItem*> >::iterator mit;
	for (mit = m_mapCashShop.begin(); mit != m_mapCashShop.end(); ++mit)
	{
		std::vector<GCItem*>& vecItem = mit->second;
		std::for_each(vecItem.begin(), vecItem.end(), KSafeDelete());
		vecItem.clear();
	}
	m_mapCashShop.clear();
}

std::vector<KItem> KGCItemManager::GetInvenItemFromKind(GCITEM_KIND eItemKind_)
{
	std::set<GCITEMID> vecRange;
	stdext::hash_map<GCITEMID, GCItem*>::iterator itor;
	for (itor = m_mapDB.begin(); itor != m_mapDB.end(); ++itor)
	{
		if (itor->second->eItemKind == eItemKind_)
			vecRange.insert(itor->second->dwGoodsID);
	}

	return m_kInventory.GetItemListInRange(vecRange);
}

std::wstring KGCItemManager::GetItemName(DWORD dwGoodsID_)
{
	GCItem* pItem = GetItemData(dwGoodsID_);
	if (!pItem)
		return L"";

	return pItem->strItemName;
}

void KGCItemManager::SetDurationItem(IN OUT std::vector<KItem>& vecDurationItemInfo)
{
	//Áö¿ì°í »õ·Î ³Ö´Â´Ù.
	m_kInventory.RemoveItemList(vecDurationItemInfo);
	m_kInventory.AddItemList(vecDurationItemInfo);
}

bool KGCItemManager::FindInventory(GCITEMUID itemUID_)
{
	return m_kInventory.FindItemByItemUID(itemUID_) != NULL;
}

bool KGCItemManager::FindInventory(GCITEMUID dwUID, KItem** ppInventoryItem)
{
	KItem* pkTempItem = m_kInventory.FindItemByItemUID(dwUID);

	if (pkTempItem == NULL)
		return false;

	*ppInventoryItem = pkTempItem;

	return true;
}

bool KGCItemManager::FindInventory(GCITEMUID dwUID, GCItem** ppGCItem)
{
	if ((*ppGCItem) != NULL)
	{
		if (dwUID == 0 && IsBaseItem((*ppGCItem)->dwGoodsID))
			return true;
	}

	KItem* pkInvenItem = m_kInventory.FindItemByItemUID(dwUID);

	if (pkInvenItem == NULL)
		return false;

	if ((*ppGCItem) == NULL)
	{
		(*ppGCItem) = GetItemData(pkInvenItem->m_ItemID);
	}

	return true;
}

bool KGCItemManager::FindInventory(GCITEMUID dwUID, KItem** ppInventoryItem, GCItem** ppGCItem)
{
	if ((*ppGCItem) != NULL)
	{
		if (dwUID == 0 && IsBaseItem((*ppGCItem)->dwGoodsID))
			return true;
	}

	KItem* kTempItem = m_kInventory.FindItemByItemUID(dwUID);

	if (kTempItem == NULL)
		return false;

	*ppInventoryItem = kTempItem;

	if ((*ppGCItem) == NULL)
	{
		(*ppGCItem) = GetItemData((*ppInventoryItem)->m_ItemID);
	}
	return true;
}

bool KGCItemManager::FindInventoryForItemID(DWORD dwItemID)
{
	return m_kInventory.FindItemByItemID(dwItemID) != NULL;
}

bool KGCItemManager::FindInventoryForItemIDCoordi(DWORD dwItemID)
{
	return m_kInventory.FindItemByItemIDCoordi(dwItemID) != NULL;
}

bool KGCItemManager::FindInventoryForItemID(DWORD dwItemID, KItem** ppInventoryItem)
{
	KItem* pkTemp = m_kInventory.FindItemByItemID(dwItemID);

	if (pkTemp == NULL)
		return false;

	*ppInventoryItem = pkTemp;
	return true;
}

bool KGCItemManager::FindInventoryForItemID(DWORD dwItemID, GCItem** ppGCItem)
{
	KItem* pkTemp = m_kInventory.FindItemByItemID(dwItemID);

	if (pkTemp != NULL)
	{
		*ppGCItem = GetItemData(dwItemID);
		return true;
	}

	return false;
}

bool KGCItemManager::FindInventoryForItemID(DWORD dwItemID, KItem** ppInventoryItem, GCItem** ppGCItem)
{
	KItem* pkTemp = m_kInventory.FindItemByItemID(dwItemID);

	if (pkTemp != NULL)
	{
		*ppGCItem = GetItemData(dwItemID);
		*ppInventoryItem = pkTemp;
		return true;
	}
	return false;
}

bool KGCItemManager::FindInventoryForItemIdAndLevel(DWORD dwItemID, int iLevel_)
{
	return m_kInventory.FindItemByItemIdAndLevel(dwItemID, iLevel_) != NULL;
}

int KGCItemManager::GetInventoryItemDuration(int iItemID)
{
	KItem* pkItem = m_kInventory.FindItemByItemID(iItemID);
	if (pkItem == NULL)
	{
		return 0;
	}
	return pkItem->m_nCount;
}

// pVector == NULL ÀÌ¸é, »ç¿ëÇß´Ù°í ¼­¹ö¿¡°Ô ¾Ë¸².
bool KGCItemManager::DecDurationItem(GCITEMUID dwUID, std::vector<GCITEMUID>* pVector)
{
	KItem* pkItem = m_kInventory.FindItemByItemUID(dwUID);
	if (pkItem == NULL)
		return false;


	GCItem* ItemInfo = NULL;

	if (pkItem->m_nCount <= 0)
	{
		m_kInventory.RemoveItem(pkItem->m_ItemUID);
		return false;
	}

	--pkItem->m_nCount;

	ItemInfo = g_MyD3D->m_kItemMgr.GetItemData(pkItem->m_ItemID);
	if (pkItem->m_nCount == 0)
	{
		m_kInventory.RemoveItem(pkItem->m_ItemUID);
	}

	if (pVector == NULL)
	{
		std::vector<GCITEMUID> vecDecItemUID;
		vecDecItemUID.push_back(dwUID);
		KP2P::GetInstance()->Send_UseDurationItemReq(vecDecItemUID);
	}
	else
	{
		pVector->push_back(dwUID);
	}

	return true;
}

bool KGCItemManager::DecDurationItemForItemID(SCharInfo* pCharInfo, int iItemID, std::vector<GCITEMUID>* pVector)
{
	KUserItemVector::iterator vit = std::find_if(pCharInfo->vecItemInfo.begin(), pCharInfo->vecItemInfo.end(), boost::bind(&SUserItem::iItemID, _1) == iItemID);

	if (pCharInfo->vecItemInfo.end() != vit)
		return DecDurationItem((*vit).dwItemUID, pVector);

	return false;
}

bool KGCItemManager::DecInventoryDurationItemForItemID(int iItemID, std::vector<GCITEMUID>* pVector, int iCount /* = 1 */)
{
	KItem* kInvenItem = NULL;
	if (!FindInventoryForItemID(iItemID, &kInvenItem))
		return false;

	bool bResult;
	for (int i = 0; i < iCount; ++i)
	{
		bResult = DecDurationItem(kInvenItem->m_ItemUID, pVector);

		if (bResult == false)
			break;
	}

	return bResult;
}

void KGCItemManager::DecInventoryDurationItemForItemIdWithOutAck(int iItemID)
{
	KItem* pkInvenItem = m_kInventory.FindItemByItemID(iItemID);

	if (pkInvenItem == NULL)
		return;

	--pkInvenItem->m_nCount;
	if (pkInvenItem->m_nCount <= 0)
	{
		g_MyD3D->GetMyShortCutSlot()->DischargeEmot(iItemID);
		m_kInventory.RemoveItem(pkInvenItem->m_ItemUID);
	}
}

void CashPatternUpWebEnd()
{
	Result_Buy = 0;

	KP2P::GetInstance()->Send_InventoryInfoReq("Server");

	g_MyD3D->WaitForServerAck(Result_Buy, 0, 3000, 1);
}

void BuyCashItemWebClose()
{
#if defined( NATION_USA ) || defined( NATION_EU )
	g_kGlobalValue.UpdateCashData();
	g_pkUIScene->m_pkCashShopScene->UpdateUserInfo();
	g_pkUIScene->m_pkGPShopScene->UpdateUserInfo();
#endif
	Result_Buy = INT_MAX;

	if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME)
	{
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
			KP2P::GetInstance()->Send_GetWebCashItemReq(KItem::EICT_AGIT);
	}
	else
	{
		KP2P::GetInstance()->Send_GetWebCashItemReq(g_kGlobalValue.m_kUserInfo.GetCurrentCharType());
	}

	if (true == g_MyD3D->WaitForServerAck(Result_Buy, INT_MAX, 300000, 2))
	{
		if (SiGCSKT()->GetBuyingReqGroup() != -1)
		{
			SiGCSKT()->Process_UnlockGroupItem(SiGCSKT()->GetBuyingReqGroup(), true);
		}
	}
	if (g_pkUIScene->IsMsgBoxModal() == false)
		g_pkUIScene->Stop(false);

	::SetFocus(g_hUIMainWnd);
}

void KGCItemManager::BuyItem(int iItemID, int iPeriod, bool bMessageBox)
{
	GCItem* kItem = GetItemData(iItemID);
	RETURN_NIL(kItem);

	Result_CanBuyCashItem = INT_MAX;

	KP2P::GetInstance()->Send_CanBuyCashItem(iItemID * 10);
	g_MyD3D->WaitForServerAck(Result_CanBuyCashItem, INT_MAX, 5000, TIME_OUT_VALUE);

	switch (Result_CanBuyCashItem)
	{
	case 0:
	case 1:
		break;
	case 2:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_LIMITED_BUY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	case 3:
	case 4:
		if (g_pItemMgr->m_bIsInPresentState == false)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
			return;
		}
		else
			break;
		return;
	case  5:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_AGIT_INVEN_FULL), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	case 6:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_BUY_AGIT_ITEM_FAIL8), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;

	case -98:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_BINGO_ERROR7), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	}

	if (kItem->eMoneyType != EMT_CASH)
	{
		ASSERT(g_pkUIScene != NULL);
		Result_Buy = 0;
		{
			if (GCIT_PERIOD == kItem->eItemType)
				KP2P::GetInstance()->Send_BuyItemForGPReq(10 * iItemID, -1, iPeriod);
			else if (GCIT_COUNT == kItem->eItemType)
				KP2P::GetInstance()->Send_BuyItemForGPReq(10 * iItemID, iPeriod, -1);
			else
				KP2P::GetInstance()->Send_BuyItemForGPReq(10 * iItemID, -1, -1);

			g_MyD3D->WaitForServerAck(Result_Buy, 0, 3000, 2);

			if (bMessageBox == true)
			{
				if (Result_Buy == 1)
				{
					g_KDSound.Play("78");
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
						g_pkStrLoader->GetString(STR_ID_BUY_ITEM_SUCCEEDED1),
						g_pkStrLoader->GetString(STR_ID_BUY_ITEM_SUCCEEDED2),
						KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
						true, false);

					if (g_pkUIScene->m_pkCustomizationDlg->IsWndState(KD3DWnd::ED3DWndState::D3DWS_ENABLED))
						g_pkUIScene->m_pkCustomizationDlg->UpdateAllSubWindows();
				}
				else if (Result_Buy == 2)
				{
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
						g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1),
						g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2),
						KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
						true, false);
				}
				else if (Result_Buy == 4)
				{
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
						g_pkStrLoader->GetString(STR_ID_GUILD_OVER_PURCHASELIMIT),
						L"",
						KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
						true, false);
				}
				else if (Result_Buy == -98)
				{
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PACKET_DELAY), L"",
						KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
						true, false);

				}
				else
				{
					std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_FOR_GP_ACK, Result_Buy);
					g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_BUY_GP_ITEM_FAILED), strTemp,
						KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0,
						true, false);
				}
			}
		}
	}
}

bool KGCItemManager::IsBaseItem(int iItemID)
{
	return std::find(m_vecBaseItem.begin(), m_vecBaseItem.end(), iItemID) != m_vecBaseItem.end();
}


bool KGCItemManager::CheckEquipItem(SUserInfo* pUserInfo, GCITEMUID dwUID)
{
	KCharInfoVector::iterator vit = pUserInfo->vecCharInfo.begin();

	for (; vit != pUserInfo->vecCharInfo.end(); ++vit)
	{
		if (CheckEquipItemByItemUID(&(*vit), dwUID))
			return true;
	}
	return false;
}

bool KGCItemManager::CheckEquipItem(GCITEMUID dwUID)
{
	SUserInfo* pUserInfo = &g_kGlobalValue.m_kUserInfo;
	if (pUserInfo == NULL) return false;
	return CheckEquipItem(pUserInfo, dwUID);
}

bool KGCItemManager::CheckEquipItemForItemID(SCharInfo* pCharInfo, GCITEMID itemID_, bool bCoordi)
{
	KUserItemVector::iterator vitInven = std::find_if(pCharInfo->vecItemInfo.begin(), pCharInfo->vecItemInfo.end(), boost::bind(&SUserItem::iItemID, _1) == itemID_);
	if (bCoordi)
	{
		KUserItemVector::iterator vitCoordi = std::find_if(pCharInfo->vecLookInfo.begin(), pCharInfo->vecLookInfo.end(), boost::bind(&SUserItem::iItemID, _1) == itemID_);
		return (pCharInfo->vecItemInfo.end() != vitInven || pCharInfo->vecLookInfo.end() != vitCoordi);
	}

	if (vitInven == pCharInfo->vecItemInfo.end()) {
		return (pCharInfo->kPetInfo.m_dwID == itemID_);
	}
	else {
		return (pCharInfo->vecItemInfo.end() != vitInven);
	}
}

bool KGCItemManager::CheckEquipItemIDForEffect(SCharInfo* pCharInfo, GCITEMID itemID_, bool bCoordi)
{
	KUserItemVector::iterator vitInven = std::find_if(pCharInfo->vecItemInfo.begin(), pCharInfo->vecItemInfo.end(), boost::bind(&SUserItem::iItemID, _1) == itemID_);
	if (bCoordi)
	{
		KUserItemVector::iterator vitCoordi = std::find_if(pCharInfo->vecLookInfo.begin(), pCharInfo->vecLookInfo.end(), boost::bind(&SUserItem::iItemID, _1) == itemID_);
		if (pCharInfo->vecLookInfo.end() == vitCoordi) {
			for (auto& element : pCharInfo->vecLookInfo) {
				if (element.m_DesignCoordiID == itemID_)
					return true;
			}
		}
		return (pCharInfo->vecItemInfo.end() != vitInven || pCharInfo->vecLookInfo.end() != vitCoordi);
	}

	if (vitInven == pCharInfo->vecItemInfo.end()) {
		return (pCharInfo->kPetInfo.m_dwID == itemID_);
	}
	else {
		return (pCharInfo->vecItemInfo.end() != vitInven);
	}
}

bool KGCItemManager::CheckEquipItemForItemID(SCharInfo* pCharInfo, GCITEMID itemID_, KItem*& pInventoryItem)
{
	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); i++)
	{
		if (pCharInfo->vecItemInfo[i].iItemID == itemID_)
		{
			pInventoryItem = m_kInventory.FindItemByItemUID(pCharInfo->vecItemInfo[i].dwItemUID);
			return pInventoryItem != NULL;
		}
	}
	for (int i = 0; i < (int)pCharInfo->vecLookInfo.size(); i++)
	{
		if (pCharInfo->vecLookInfo[i].iItemID == itemID_)
		{
			pInventoryItem = m_kInventory.FindItemByItemUID(pCharInfo->vecLookInfo[i].dwItemUID);
			return pInventoryItem != NULL;
		}
	}
	return false;
}

bool KGCItemManager::CheckEquipItemForItemID(SCharInfo* pCharInfo, GCITEMID itemID_, GCItem*& pGCItem)
{
	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); i++)
	{
		if (pCharInfo->vecItemInfo[i].iItemID != itemID_)
			continue;

		GCITEMUID itemUID = pCharInfo->vecItemInfo[i].dwItemUID;
		KItem* pInventoryItem = m_kInventory.FindItemByItemUID(itemUID);

		if (itemUID == 0 && pGCItem != NULL) {
			if (IsBaseItem(pGCItem->dwGoodsID))
				return true;
		}
		else {
			pGCItem = GetItemData(itemID_);
		}

		return pInventoryItem != NULL;
	}
	for (int i = 0; i < (int)pCharInfo->vecLookInfo.size(); i++)
	{
		if (pCharInfo->vecLookInfo[i].iItemID != itemID_)
			continue;

		GCITEMUID itemUID = pCharInfo->vecLookInfo[i].dwItemUID;
		KItem* pInventoryItem = m_kInventory.FindItemByItemUID(itemUID);

		if (itemUID == 0 && pGCItem != NULL) {
			if (IsBaseItem(pGCItem->dwGoodsID))
				return true;
		}
		else {
			pGCItem = GetItemData(itemID_);
		}

		return pInventoryItem != NULL;
	}
	return false;
}

bool KGCItemManager::CheckEquipItemForItemID(SCharInfo* pCharInfo, GCITEMID itemID_, KItem*& pInventoryItem, GCItem*& pGCItem)
{
	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); i++)
	{
		if (pCharInfo->vecItemInfo[i].iItemID == itemID_)
		{
			GCITEMUID itemUID = pCharInfo->vecItemInfo[i].dwItemUID;
			pInventoryItem = m_kInventory.FindItemByItemUID(itemUID);

			if (itemUID == 0 && pGCItem != NULL) {
				if (IsBaseItem(pGCItem->dwGoodsID))
					return true;
			}
			else {
				pGCItem = GetItemData(itemID_);
			}

			return pInventoryItem != NULL;
		}
	}
	for (int i = 0; i < (int)pCharInfo->vecLookInfo.size(); i++)
	{
		if (pCharInfo->vecLookInfo[i].iItemID == itemID_)
		{
			GCITEMUID itemUID = pCharInfo->vecLookInfo[i].dwItemUID;
			pInventoryItem = m_kInventory.FindItemByItemUID(itemUID);

			if (itemUID == 0 && pGCItem != NULL) {
				if (IsBaseItem(pGCItem->dwGoodsID))
					return true;
			}
			else {
				pGCItem = GetItemData(itemID_);
			}

			return pInventoryItem != NULL;
		}
	}
	return false;
}

void KGCItemManager::MakeSetItemList(std::vector<GCItem*>& pVecSetItem, int iSetID)
{
	pVecSetItem.clear();
	stdext::hash_map< GCITEMID, GCItem* >::iterator i;
	for (i = m_mapDB.begin(); i != m_mapDB.end(); i++)
	{
		if (i->second->iSetItemNumber == iSetID)
			pVecSetItem.push_back(i->second);
	}
}

bool KGCItemManager::CheckEquipSetItem(SCharInfo* pCharInfo, int iSetID)
{
	//¹®Á¦Á¡ : Ã£´Â ¼¼Æ®°¡ ¾øÀ¸¸é ÂüÀ» ¹ÝÈ¯ÇÑ´Ù.

	bool bAllEquip = true;

	std::vector<GCItem*> vecSetItem;
	MakeSetItemList(vecSetItem, iSetID);

	for (int i = 0; i < (int)vecSetItem.size(); i++)
	{
		GCItem* kData = vecSetItem[i];

		if (!(kData->dwSlotPosition & NONESETITEM_COMPONENT))
		{
			//ÀåÂø¾ÆÀÌÅÛ¿¡ ÀÌ ¾ÆÀÌÅÛÀÌ ÀÖ´Â°¡?? ¾øÀ¸¸é...bAllEquip = false;
			bool bEquipItemCurrent = false;
			for (int j = 0; j < (int)pCharInfo->vecItemInfo.size(); ++j)
			{
				if (kData->dwGoodsID == pCharInfo->vecItemInfo[j].iItemID)//ÀÔ°í ÀÖÀ½
				{
					bEquipItemCurrent = true;
					break;
				}
			}
			if (!bEquipItemCurrent)
			{
				return false;
			}
		}
	}
	return true;
}

void KGCItemManager::UnEquipBaseItem(SCharInfo* pCharInfo)
{
	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); ++i)
	{
		// ¸¸¾à ±âº» Àåºñ¶ó¸é?
		if (IsBaseItem(pCharInfo->iCharType, pCharInfo->vecItemInfo[i].iItemID))
		{
			// ´ëÈ¸¿ë ÄªÈ£¸é ÀåºñÃ¢ Ä³¸¯ÅÍ Ã¢¿¡ ±×¸²À» Á¦°Å ÇØÁà¾ßÇÑ´Ù.
			if (IsRallyTitleItemID(pCharInfo->vecItemInfo[i].iItemID))
				g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);

			pCharInfo->vecItemInfo.erase(pCharInfo->vecItemInfo.begin() + i);
			--i;
		}
	}
}

void KGCItemManager::EquipBaseItem(SCharInfo* pCharInfo)
{
	DWORD dwFlag = 0;
	int iCharType = pCharInfo->iCharType;
	if (0 > iCharType || iCharType >= GC_CHAR_NUM)
		return;

	KItemIDVector vecViewItemList = pCharInfo->GetEquipmentForLoadModel();
	KItemIDVector::iterator iter = vecViewItemList.begin();

	for (; iter != vecViewItemList.end(); ++iter)
	{
		GCItem* pItem = GetItemData(*iter);
		CONTINUE_NIL(pItem);
		dwFlag |= pItem->dwSlotPosition;
	}

	DWORD aSlotPos[4] = { ESP_HELMET,ESP_JACKET,ESP_PANTS,ESP_WEAPON };
	for (int i = 0; i < 4; i++)
	{
		int iItemIndex = iCharType * 4 + i;
		if (!(dwFlag & aSlotPos[i]))
		{
			GCItem* ItemInfo = GetItemData(m_vecBaseItem[iItemIndex]);
			EquipItem(pCharInfo, ItemInfo, true);
			break;
		}
	}
}

void KGCItemManager::EquipItem(SCharInfo* pCharInfo, GCItem*& pItemInfo, bool bEquipBase_, GCITEMUID itemUID_, bool bCoordi_)
{
	if (pItemInfo == NULL)
		return;

	if (GIK_MONSTER_CARD == pItemInfo->eItemKind)
		return;

	if (GIK_ENCHANT_ASIST_ITEM == pItemInfo->eItemKind)
		return;

	if (pItemInfo->eItemKind == GIK_PET && (pItemInfo->ePetItemKind == EPET_KIND_PET || pItemInfo->ePetItemKind == EPET_KIND_EQUIP_ITEM))
		return;

	if (pItemInfo->dwSlotPosition & ESP_TITLE)
	{
		if (bCoordi_)
		{
			g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(pItemInfo->dwGoodsID);
		}
		else
		{
			EquipTitleItem(pCharInfo, pItemInfo->dwGoodsID);
			return;
		}
	}

	if (pItemInfo->dwSlotPosition == 0 && pItemInfo->dwSkillPosition == 0 && pItemInfo->dwPetEquipItemSlot == 0)
	{
		if (g_pkUIScene != NULL)
		{
#ifndef DEV_BUILD
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_UNABLE_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_UNABLE_EQUIP2));
#endif
		}
		return;
	}

	KUserItemVector* pvecItemInfo = NULL;
	if (bCoordi_) {
		pvecItemInfo = &pCharInfo->vecLookInfo;
	}
	else {
		pvecItemInfo = &pCharInfo->vecItemInfo;
	}

	KUserItemVector::iterator iter = (*pvecItemInfo).begin();
	while (iter != (*pvecItemInfo).end())
	{
		if (!bEquipBase_)
		{
			GCItem* pItem = GetItemData(iter->iItemID);

			if (pItem &&
				(pItemInfo->dwSlotPosition != 0 && pItemInfo->dwSlotPosition & pItem->dwSlotPosition)
				|| (pItemInfo->dwSkillPosition != 0 && pItemInfo->dwSkillPosition & pItem->dwSkillPosition)
				|| (pItemInfo->dwPetEquipItemSlot != 0 && pItemInfo->dwPetEquipItemSlot & pItem->dwPetEquipItemSlot))
			{
				if (IsRallyTitleItemID(iter->iItemID))
					g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);

				iter = (*pvecItemInfo).erase(iter);
				continue;
			}
		}
		iter++;
	}

	if (IsRallyTitleItemID(pItemInfo->dwGoodsID))
	{
		EquipTitleItem(pCharInfo, pItemInfo->dwGoodsID);
		return;
	}

	if ((*pvecItemInfo).size() < MAX_EQUIP_NUM)
	{
		SUserItem _Item;
		_Item.dwItemUID = itemUID_;
		_Item.iItemID = pItemInfo->dwGoodsID;
		_Item.cEnchantLevel = 0;
		_Item.vecItemAttribute = pItemInfo->m_vecAttribute;
		_Item.vecItemSocket = pItemInfo->m_vecSocketInfo;
		_Item.cGradeID = pItemInfo->cItemGrade;
		(*pvecItemInfo).push_back(_Item);
	}
	else
	{
		if (g_pkUIScene != NULL)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2));
		}
	}

	EquipBaseItem(pCharInfo);
}


void KGCItemManager::EquipChangeWeaponItem(SCharInfo* pCharInfo, GCItem*& pItemInfo, KItem* pInvenItemInfo)
{
	for (int i = (int)pCharInfo->vecItemInfo.size() - 1; i >= 0; --i)
	{
		GCItem* pItem = GetItemData(pCharInfo->vecItemInfo[i].iItemID);

		if (pItem && (pItemInfo->dwSlotPosition != 0 && pItemInfo->dwSlotPosition & pItem->dwSlotPosition))
		{
			pCharInfo->vecItemInfo.erase(pCharInfo->vecItemInfo.begin() + i);
		}
	}

	// ¾ÆÀÌÅÛÀ» Ãß°¡·Î ´õ ÀåÂøÇÒ ¼ö ÀÖ´ÂÁö È®ÀÎ
	if (pCharInfo->vecItemInfo.size() < MAX_EQUIP_NUM && pInvenItemInfo->m_ItemID != -1)
	{
		SUserItem _Item;
		_Item = *pInvenItemInfo;

		pCharInfo->vecItemInfo.push_back(_Item);
	}

	// ¾ø´Â ºÎÀ§¸¦ ±âº»¸öÅëÀ¸·Î Ã¤¿î´Ù.
	EquipBaseItem(pCharInfo);
}

// ÀÎº¥Åä¸®¿¡ Á¸ÀçÇÏ´Â ¾ÆÀÌÅÛÀ» ÀÔ½À´Ï´Ù
// ÀÎº¥Åä¸®¿¡ Á¸ÀçÇÏÁö ¾ÊÀ¸¸é ÀÔÀ» ¼ö ¾ø½À`´Ï´Ù
bool KGCItemManager::EquipInventoryItem(SCharInfo* pCharInfo, GCITEMUID dwUID, bool bCoordi_ /*= false*/, bool checkClass)
{
	GCItem* kItem = NULL;
	KItem* kInvenItem = m_kInventory.FindItemByItemUID(dwUID);

	if (kInvenItem == NULL)
		return false;

	kItem = GetItemData(kInvenItem->m_ItemID);
	if (kItem == NULL)
		return false;

	// ¼Ó¼º ´É·ÂÄ¡ Àû¿ë!
	///////////////////////////////////////////////////////////////////////////////////////
	std::vector< KAttributeInfo > vecAttribute = kInvenItem->m_vecAttribute;
	std::vector< KAttributeInfo >::iterator vitAttribute = vecAttribute.begin();

	char cEquipDecLvValue = 0;
	char cEquipWithBlacksmithPermit = 0;
	while (vecAttribute.end() != vitAttribute)
	{
		if ((*vitAttribute).m_cType == ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN)
		{
			cEquipDecLvValue = static_cast<char>((*vitAttribute).m_fValue);
		}
		++vitAttribute;
	}

	if (FindInventoryForItemID(ITEM_BLACKSMITH_PERMIT)) {
		cEquipWithBlacksmithPermit = 10;
	}
	else {
		cEquipWithBlacksmithPermit = 0;
	}

	if (pCharInfo->iLevel < (GetlItemEquipLevel(kItem, kInvenItem, NULL, true) - cEquipDecLvValue) - cEquipWithBlacksmithPermit)
		return false;//Level제한에 걸렸음

	if (checkClass && (pCharInfo->iPromotionLevel < kItem->iNeedChangeJobLevelForEquippingItem && kItem->dwSlotPosition != 0))
	{
		if (g_pkUIScene != NULL)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_CERT_NEED_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_CERT_NEED_EQUIP2));
		}
		return false;//ÀüÁ÷ Á¦ÇÑ¿¡ °É·ÈÀ½
	}
	//==========================ÀÎº¥Åä¸® ¹× Á¦¾àÁ¶°Ç È®ÀÎ ¿Ï·á==============================//   

	bool bResult = false;

	if (!bCoordi_)
	{
		bResult = NormalEquipInventoryItem(pCharInfo, kItem, kInvenItem);
	}
	else
	{
		bResult = CoordiEquipInventoryItem(pCharInfo, kItem, kInvenItem);
	}

	return bResult;
}

bool KGCItemManager::NormalEquipInventoryItem(SCharInfo* pCharInfo, GCItem*& pItemInfo, KItem*& pInvenItemInfo)
{
	if (GIK_MONSTER_CARD == pItemInfo->eItemKind)
		return false;

	if (pItemInfo->eItemKind == GIK_PET && (pItemInfo->ePetItemKind == EPET_KIND_PET || pItemInfo->ePetItemKind == EPET_KIND_EQUIP_ITEM))
		return false;

	if (pItemInfo->eItemKind == GIK_TITLE_ITEM)
	{
		return EquipTitleItem(pCharInfo, pItemInfo->dwGoodsID);
	}

	if (!(pItemInfo->dwCharType & ItemCharTypeToItemFlag(pCharInfo->iCharType)))
		return false;

	if (pItemInfo->eItemKind == GIK_ENCHANT_ASIST_ITEM || pItemInfo->dwSlotPosition == 0 && pItemInfo->dwSkillPosition == 0 && pItemInfo->dwPetEquipItemSlot == 0 && !SiGCPetMagicStone()->IsPetGlyph(pInvenItemInfo->m_ItemID))
	{
		if (g_pkUIScene != NULL && g_MyD3D->m_pStateMachine->GetState() != GS_SERVER)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_UNABLE_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_UNABLE_EQUIP2));
		}
		return false;
	}

	KUserItemVector* pvecItemInfo = NULL;
	pvecItemInfo = &pCharInfo->vecItemInfo;


	KUserItemVector vecEquipEarring;

	for (int i = 0; i < static_cast<int>(pCharInfo->vecItemInfo.size()); i++)
	{
		GCItem* pItem = GetItemData(pCharInfo->vecItemInfo[i].iItemID);
		if (pItem->dwSlotPosition & ESP_A_EARRING)
			vecEquipEarring.push_back(pCharInfo->vecItemInfo[i]);
	}

	bool bChangeCheck = false;

	KUserItemVector::iterator iter = (*pvecItemInfo).begin();
	while (iter != (*pvecItemInfo).end())
	{
		GCItem* pItem = GetItemData(iter->iItemID);

		if (pItem &&
			(pItemInfo->dwSlotPosition != 0 && pItemInfo->dwSlotPosition & pItem->dwSlotPosition)
			|| (pItemInfo->dwSkillPosition != 0 && pItemInfo->dwSkillPosition & pItem->dwSkillPosition)
			|| (pItemInfo->dwPetEquipItemSlot != 0 && pItemInfo->dwPetEquipItemSlot & pItem->dwPetEquipItemSlot))
		{
			if (IsRallyTitleItemID(iter->iItemID))
				g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);

			if (pItem->dwSlotPosition & ESP_A_EARRING && vecEquipEarring.size() < EMAX_HERO_EARRING_SLOT)
			{
				if (pItemInfo->dwGoodsID == iter->iItemID)
				{
					iter = pCharInfo->vecItemInfo.erase(iter);
					continue;
				}
			}
			else if (pItem->dwSlotPosition & ESP_A_EARRING && vecEquipEarring.size() == EMAX_HERO_EARRING_SLOT)
			{
				if (pItemInfo->dwGoodsID == iter->iItemID)
				{
					iter = pCharInfo->vecItemInfo.erase(iter);
					bChangeCheck = true;
					continue;
				}

				if (!bChangeCheck)
				{
					bool bSameCheck = false;
					for (int i = 0; i < static_cast<int>(vecEquipEarring.size()); i++)
					{
						if (vecEquipEarring[i].iItemID == pItemInfo->dwGoodsID)
							bSameCheck = true;
					}

					if (!bSameCheck)
					{
						iter = (*pvecItemInfo).erase(iter);
						bChangeCheck = true;
						continue;
					}
				}
			}
			else
			{
				iter = pCharInfo->vecItemInfo.erase(iter);
				continue;
			}
		}

		if (pItemInfo->dwGoodsID == KGCItemManager::ITEM_FULL_MOON_RING &&
			(pItem->dwSkillPosition & ESP_SKC_WHITE_TWO ||
				pItem->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB_TWO ||
				pItem->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB2_TWO))
		{
			iter = (*pvecItemInfo).erase(iter);
			continue;
		}
		iter++;
	}

	if (IsRallyTitleItemID(pItemInfo->dwGoodsID))
	{
		return EquipTitleItem(pCharInfo, pItemInfo->dwGoodsID);;
	}

	if ((*pvecItemInfo).size() < MAX_EQUIP_NUM)
	{
		SUserItem _Item;
		_Item = *pInvenItemInfo;
		(*pvecItemInfo).push_back(_Item);
	}
	else
	{
		if (g_pkUIScene != NULL)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2));
		}
	}

	EquipBaseItem(pCharInfo);
	return true;
}

bool KGCItemManager::CoordiEquipInventoryItem(SCharInfo* pCharInfo, GCItem*& pItemInfo, KItem*& pInvenItemInfo)
{
	// ÄÚµð °³Æí ¾Èµé¾î°¡¸é ÄÚµð ¹«±â ÀåÂø ¸øÇÏ°Ô ¸·½À´Ï´Ù.
	if (pItemInfo->dwSlotPosition & ESP_WEAPON && !GC_GLOBAL_DEFINE::bWeaponCoordi)
		return false;

	if (GIK_MONSTER_CARD == pItemInfo->eItemKind)
		return false;

	// ÆêÀÌ³ª Æê¾ÆÀÌÅÛÀÏ °æ¿ì ¿©±â¿¡ ÀåÂøÇÏÁö ¾Ê´Â´Ù
	if (pItemInfo->eItemKind == GIK_PET && (pItemInfo->ePetItemKind == EPET_KIND_PET || pItemInfo->ePetItemKind == EPET_KIND_EQUIP_ITEM))
		return false;

	// ÇØ´ç Ä³¸¯ÅÍ¿Í ÀÏÄ¡ÇÏ´Â ÀåºñÀÎÁö Ã¼Å©
	if (!(pItemInfo->dwCharType & ItemCharTypeToItemFlag(pCharInfo->iCharType)))
		return false;

	// ÀåÂø¿ë ÀåºñÀÎÁö Ã¼Å©
	if (pItemInfo->dwSlotPosition == 0 && pItemInfo->dwSkillPosition == 0 && pItemInfo->dwPetEquipItemSlot == 0)
	{
		if (g_pkUIScene != NULL && g_MyD3D->m_pStateMachine->GetState() != GS_SERVER)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_UNABLE_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_UNABLE_EQUIP2));
		}
		return false;
	}

	KUserItemVector* pvecItemInfo = NULL;
	pvecItemInfo = &pCharInfo->vecLookInfo;


	bool bChangeCheck = false;

	KUserItemVector::iterator iter = (*pvecItemInfo).begin();

	while (iter != (*pvecItemInfo).end())
	{
		GCItem* pItem = GetItemData(iter->iItemID);

		if (pItem && (pItemInfo->dwSlotPosition != 0 && pItemInfo->dwSlotPosition & pItem->dwSlotPosition))
		{
			//  ¹«±â ÄÚµð ÀåÂø Á¶°Ç
			if (pItem->dwSlotPosition & ESP_WEAPON)
			{
				if (pItemInfo->iNeedChangeJobLevelForEquippingItem == pItem->iNeedChangeJobLevelForEquippingItem)
				{
					iter = pCharInfo->vecLookInfo.erase(iter);
					continue;
				}
			}
			else
			{
				if (pItem->dwSlotPosition & ESP_TITLE)
				{
					GCItem* tempTitle;
					GCITEMID tempTitleID = -1;
					for (auto normalTitle : pCharInfo->vecItemInfo)
					{
						tempTitle = g_pItemMgr->GetItemData(normalTitle.iItemID);
						if (tempTitle->dwSlotPosition & ESP_TITLE)
						{
							tempTitleID = tempTitle->dwGoodsID;
							break;
						}
					}
					g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(tempTitleID);
				}
				iter = pCharInfo->vecLookInfo.erase(iter);
				continue;
			}
		}

		iter++;
	}

	if ((*pvecItemInfo).size() < MAX_EQUIP_NUM)
	{
		SUserItem _Item;
		_Item = *pInvenItemInfo;

		(*pvecItemInfo).push_back(_Item);
	}
	else
	{
		if (g_pkUIScene != NULL)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1),
				g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2));
		}
	}

	if (pItemInfo->dwSlotPosition & ESP_TITLE)
	{
		g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(pItemInfo->dwGoodsID);
	}

	EquipBaseItem(pCharInfo);
	return true;
}

void KGCItemManager::EquipShopItem(SCharInfo* pCharInfo, DWORD _ItemKey)
{
	if (m_mapDB.find(_ItemKey) == m_mapDB.end()) //Á¸ÀçÇÏÁö ¾Ê´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù
		return;

	EquipItem(pCharInfo, m_mapDB[_ItemKey]);
}

void KGCItemManager::EquipSetItem(SCharInfo* pCharInfo, int iSetID)//Set¸¦ ¸ðµÎ ÀÔÈü´Ï´Ù.
{
	std::vector<GCItem*> vecSetItem;
	MakeSetItemList(vecSetItem, iSetID);

	// ¼ÂÆ® ±¸¼ºÁß ¾ø´Â ¾ÆÀÌÅÛ ¶§¹®¿¡ ¹®Á¦°¡ »ý°Ü¼­ ¼öÁ¤
	for (int i = 0; i < (int)m_vecAlreadyEquipSetItem.size(); ++i)
		UnequipItemForItemID(pCharInfo, m_vecAlreadyEquipSetItem[i]->dwGoodsID);
	m_vecAlreadyEquipSetItem.clear();

	for (int i = 0; i < (int)vecSetItem.size(); i++)
	{
		GCItem* kData = vecSetItem[i];
		if (!(kData->dwSlotPosition & NONESETITEM_COMPONENT))
		{
			EquipItem(pCharInfo, kData);
			m_vecAlreadyEquipSetItem.push_back(kData);
		}
	}
}

void KGCItemManager::UnequipItemAll(SCharInfo* pCharInfo)
{
	if (!pCharInfo)
		return;

	pCharInfo->vecItemInfo.clear();
	pCharInfo->vecLookInfo.clear();
	EquipBaseItem(pCharInfo);
}

void KGCItemManager::UnequipLookItemAll(SCharInfo* pCharInfo)
{
	if (!pCharInfo)
		return;

	pCharInfo->vecLookInfo.clear();
	EquipBaseItem(pCharInfo);
}

void KGCItemManager::UnEquipItem(SCharInfo* pCharInfo)
{
	if (!pCharInfo)
		return;

	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); ++i)
	{
		// ´ëÈ¸¿ë ÄªÈ£¸é ÀåºñÃ¢ Ä³¸¯ÅÍ Ã¢¿¡ ±×¸²À» Á¦°Å ÇØÁà¾ßÇÑ´Ù.
		if (IsRallyTitleItemID(pCharInfo->vecItemInfo[i].iItemID))
			g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);
	}

	pCharInfo->vecItemInfo.clear();
	EquipBaseItem(pCharInfo);
}

void KGCItemManager::UnEquipAllItem(SCharInfo* pCharInfo)
{
	if (!pCharInfo)
		return;

	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); ++i)
	{
		// ´ëÈ¸¿ë ÄªÈ£¸é ÀåºñÃ¢ Ä³¸¯ÅÍ Ã¢¿¡ ±×¸²À» Á¦°Å ÇØÁà¾ßÇÑ´Ù.
		if (IsRallyTitleItemID(pCharInfo->vecItemInfo[i].iItemID))
			g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);
	}

	pCharInfo->vecItemInfo.clear();
	pCharInfo->vecLookInfo.clear();

	EquipBaseItem(pCharInfo);
}


void KGCItemManager::UnequipItemForItemUID(SCharInfo* pCharInfo, GCITEMUID itemUID)
{
	for (int i = (int)pCharInfo->vecItemInfo.size() - 1; i >= 0; i--)
	{
		if (pCharInfo->vecItemInfo[i].dwItemUID == itemUID)
		{
			// ´ëÈ¸¿ë ÄªÈ£¸é ÀåºñÃ¢ Ä³¸¯ÅÍ Ã¢¿¡ ±×¸²À» Á¦°Å ÇØÁà¾ßÇÑ´Ù.
			if (IsRallyTitleItemID(pCharInfo->vecItemInfo[i].iItemID))
				g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);

			pCharInfo->vecItemInfo.erase(pCharInfo->vecItemInfo.begin() + i);
		}
	}

	for (int i = (int)pCharInfo->vecLookInfo.size() - 1; i >= 0; --i) {
		if (pCharInfo->vecLookInfo[i].dwItemUID == itemUID) {
			pCharInfo->vecLookInfo.erase(pCharInfo->vecLookInfo.begin() + i);
		}
	}

	for (auto i = (int)pCharInfo->vecPetGlyphInfo.size() - 1; i >= 0; --i)
	{
		if (pCharInfo->vecPetGlyphInfo[i].m_dwUID == itemUID)
		{
			pCharInfo->vecPetGlyphInfo.erase(pCharInfo->vecPetGlyphInfo.begin() + i);
		}
	}

	EquipBaseItem(pCharInfo);
}

void KGCItemManager::UnequipItemForItemID(SCharInfo* pCharInfo, int iItemID)
{
	for (int i = (int)pCharInfo->vecItemInfo.size() - 1; i >= 0; i--)
	{
		if (pCharInfo->vecItemInfo[i].iItemID == iItemID)
		{
			// ´ëÈ¸¿ë ÄªÈ£¸é ÀåºñÃ¢ Ä³¸¯ÅÍ Ã¢¿¡ ±×¸²À» Á¦°Å ÇØÁà¾ßÇÑ´Ù.
			if (IsRallyTitleItemID(iItemID))
				g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);

			pCharInfo->vecItemInfo.erase(pCharInfo->vecItemInfo.begin() + i);
		}
	}

	for (KUserItemVector::iterator it = pCharInfo->vecLookInfo.begin(); it != pCharInfo->vecLookInfo.end(); ++it)
	{
		if (it->iItemID == iItemID)
		{
			it = pCharInfo->vecLookInfo.erase(it);
			break;
		}
	}

	//  ºê¶óÁúÀº Æê ÀåÂøµµ ±ÝÁö
	if (pCharInfo->kPetInfo.m_dwID == iItemID) {
		pCharInfo->kPetInfo.Init();
	}

	for (auto i = (int)pCharInfo->vecPetGlyphInfo.size() - 1; i >= 0; --i)
	{
		if (pCharInfo->vecPetGlyphInfo[i].m_dwID == (iItemID / 10))
		{
			pCharInfo->vecPetGlyphInfo.erase(pCharInfo->vecPetGlyphInfo.begin() + i);
		}
	}

	EquipBaseItem(pCharInfo);
}

bool KGCItemManager::IsBaseItem(int iCharType, DWORD ItemID)
{
	int iBegin = iCharType * 4;
	int iEnd = (iCharType + 1) * 4;

	return std::find(m_vecBaseItem.begin() + iBegin, m_vecBaseItem.begin() + iEnd, ItemID) != m_vecBaseItem.begin() + iEnd;
}

//´Ù¾´ ¼ö·®¼º ¾ÆÀÌÅÛÀ» ¸ðµÎ Áö¿ì°í
//ÀåÂøÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÁß.. ´Ù¾´°ÍÀº Á¦°ÅÇÔ
bool KGCItemManager::RemoveEmptyItem(SUserInfo* pUserInfo, std::vector< KSimpleItem >* pvecOut)
{
	bool bChange = false;
	std::vector<KItem> vecEmptyItem;
	m_kInventory.GetZeroCountItemList(vecEmptyItem);
	m_kInventory.RemoveItemList(vecEmptyItem);
	bChange = !vecEmptyItem.empty();

	for (int iChar = 0; iChar < pUserInfo->GetCharSize(); iChar++)
	{
		SCharInfo* pCharInfo = &pUserInfo->vecCharInfo[iChar];
		if (pCharInfo == NULL)
			g_kGlobalValue.m_strNullFunc = __FUNCTION__;

		for (int j = (int)pCharInfo->vecItemInfo.size() - 1; j >= 0; j--)
		{
			// ±âº» ¾ÆÀÌÅÛÀÌ¸é ³Ñ¾î°¡ÀÚ
			if (IsBaseItem(pCharInfo->iCharType, pCharInfo->vecItemInfo[j].iItemID))
				continue;

			if (m_kInventory.FindItemByItemUID(pCharInfo->vecItemInfo[j].dwItemUID) == NULL)
			{
				KSimpleItem kSimpleItem;
				kSimpleItem.m_cItemType = GetItemData(pCharInfo->vecItemInfo[j].iItemID)->eItemType;
				kSimpleItem.m_dwUID = pCharInfo->vecItemInfo[j].dwItemUID;
				kSimpleItem.m_dwID = pCharInfo->vecItemInfo[j].iItemID * 10;
				(*pvecOut).push_back(kSimpleItem);

				pCharInfo->vecItemInfo.erase(pCharInfo->vecItemInfo.begin() + j);
				bChange = true;
			}
		}
		EquipBaseItem(pCharInfo);
	}
	return bChange;
}

void KGCItemManager::UnequipSetItem(SCharInfo* pCharInfo, int iSetID)//Set¸¦ ¸ðµÎ ¹þ±é´Ï´Ù.
{
	std::vector<GCItem*> vecSetItem;
	MakeSetItemList(vecSetItem, iSetID);

	for (int i = 0; i < (int)vecSetItem.size(); i++)
	{
		GCItem* kData = vecSetItem[i];
		if (!(kData->dwSlotPosition & NONESETITEM_COMPONENT))
		{
			UnequipItemForItemID(pCharInfo, kData->dwGoodsID);
		}
	}
}

DWORD KGCItemManager::GetItemGameModeFlag(DWORD gameMode)
{
	switch (gameMode)
	{
	case GC_GM_TEAM_MODE:
	case GC_GM_TAG_TEAM:
	case GC_GM_DEATH_TEAM:
	case GC_GM_ANGELS_EGG:
	case GC_GM_CAPTAIN:
	case GC_GM_DOTA:
	case GC_GM_AUTOMATCH_TEAM:
	case GC_GM_FATAL_DEATH_TEAM:
		return GCGM_TEAM;

	case GC_GM_SURVIVAL_MODE:
	case GC_GM_TAG_SURVIVAL:
	case GC_GM_DEATH_SURVIVAL:
	case GC_GM_FATAL_DEATH_SURVIVAL:
		return GCGM_SURVIVAL;

	case GC_GM_GUILD_BATTLE:
		return GCGM_GUILD_BATTLE;

	case GC_GM_QUEST0:
		return GCGM_QUEST_1;

	case GC_GM_QUEST1:
		return GCGM_QUEST_1;

	case GC_GM_QUEST2:
		return GCGM_QUEST_2;

	case GC_GM_QUEST3:
		return GCGM_QUEST_3;

	case GC_GM_QUEST4:
		return GCGM_QUEST_4;

	case GC_GM_QUEST5:
		return GCGM_QUEST_5;

	case GC_GM_QUEST6:
		return GCGM_QUEST_6;

	case GC_GM_QUEST7:
		return GCGM_QUEST_7;

	case GC_GM_QUEST8:
		return GCGM_QUEST_8;

	case GC_GM_QUEST9:
		return GCGM_QUEST_9;

	case GC_GM_QUEST10:
		return GCGM_QUEST_10;

	case GC_GM_QUEST11:
		return GCGM_QUEST_11;

	case GC_GM_QUEST12:
		return GCGM_QUEST_12;

	case GC_GM_QUEST13:
		return GCGM_QUEST_13;

	case GC_GM_QUEST14:
		return GCGM_QUEST_14;

	case GC_GM_QUEST15:
		return GCGM_QUEST_15;

	case GC_GM_QUEST16:
		return GCGM_QUEST_16;

	case GC_GM_QUEST17:
		return GCGM_QUEST_17;

	case GC_GM_QUEST18:
		return GCGM_QUEST_18;

	case GC_GM_QUEST19:
		return GCGM_QUEST_19;

	case GC_GM_QUEST20:
		return GCGM_QUEST_20;

	case GC_GM_QUEST21:
		return GCGM_QUEST_21;

	case GC_GM_QUEST22:
	case GC_GM_QUEST23:
	case GC_GM_QUEST24:
	case GC_GM_QUEST25:
	case GC_GM_QUEST26:
	case GC_GM_QUEST27:
	case GC_GM_QUEST28:
	case GC_GM_QUEST29:
	case GC_GM_QUEST30:
	case GC_GM_QUEST31:
	case GC_GM_QUEST32:
	case GC_GM_QUEST33:
	case GC_GM_QUEST34:
	case GC_GM_QUEST35:
	case GC_GM_QUEST36:
	case GC_GM_QUEST37:
	case GC_GM_QUEST38:
	case GC_GM_QUEST39:
	case GC_GM_QUEST40:
	case GC_GM_QUEST41:
	case GC_GM_QUEST42:
	case GC_GM_QUEST50:
		return GCGM_QUEST_22;

	case GC_GM_MONSTER_CRUSADER:
		return GCGM_MONSTER_CRUSADERS;

	case GC_GM_MONSTER_HUNT:
		return GCGM_MONSTER_HUNT;
	}
	return 0;
}

void KGCItemManager::LoadSetItemList(void)
{

	KLuaManager luaMgr;

	//	Enum°ªÀ» »ç¿ëÇÏ±â À§ÇØ ·Îµå
	if (GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg") == false)
		return;

	// ¹öÇÁµµ »ç¿ëÇÒ ¼ö ÀÖ°Ô ÇÏÀÚ
	if (GCFUNC::LoadLuaScript(luaMgr, "BuffEnum.stg") == false)
		return;

	if (GCFUNC::LoadLuaScript(luaMgr, "SetItem.stg") == false)
		return;

	LUA_BEGIN_TABLE("SET_ITEM", return);
	{
		for (int i = 1; ; i++)
		{
			SetTemplet setTemplet;
			LUA_BEGIN_TABLE(i, break)
			{
				LUA_GET_VALUE("NAME", setTemplet.iSetNum, return);
				LUA_BEGIN_TABLE("PARTS", return)
				{
					int iTemp = 0;
					for (int j = 1; ; j++)
					{
						LUA_GET_VALUE_NOASSERT(j, iTemp, break)
							setTemplet.vecParts.push_back(iTemp);
					}
				}
				LUA_END_TABLE(return);

				if (SUCCEEDED(luaMgr.BeginTable("ATTRIBUTE_LIST")))
				{
					for (int k = 1; ; ++k)
					{
						LUA_BEGIN_TABLE(k, break);
						{
							SetItemAttributeList stAttributeList;
							LUA_GET_VALUE_DEF("EQUIP_COUNT", stAttributeList.iEquipCount, setTemplet.vecParts.size());
							if (SUCCEEDED(luaMgr.BeginTable("ATTRIBUTE")))
							{
								for (int p = 1; ; ++p)
								{
									LUA_BEGIN_TABLE(p, break);
									{
										SetItemAttribute stSetItemAttribute;
										int iAttributeType = 0;
										LUA_GET_VALUE_DEF("ATTRIBUTE_TYPE", iAttributeType, 0);
										stSetItemAttribute.eAttributeType = static_cast<EAttributeType>(iAttributeType);
										LUA_GET_VALUE_DEF("ABILITY", stSetItemAttribute.fAbility, 0.0f);
										stAttributeList.vecSetItemAttribute.push_back(stSetItemAttribute);
									}
									LUA_END_TABLE(return);
								}
								LUA_END_TABLE(return);
							}
							setTemplet.vecSetItemAtrributeList.push_back(stAttributeList);
						}
						LUA_END_TABLE(return);
					}
					LUA_END_TABLE(return);
				}

				// 				LUA_GET_VALUE_DEF("EFF_ENUM", setTemplet.iItemEffEnum, -1 );
				// 
				if (luaMgr.BeginTable("EFF_ENUM") == S_OK)	// ¼¯¾î½á¼­ ¹Ì¾È¿ä..
				{
					int iItemEffEnum = -1;
					for (int iIndex = 1; luaMgr.GetValue(iIndex, iItemEffEnum) == S_OK; ++iIndex)
					{
						setTemplet.vecItemEffEnum.push_back(iItemEffEnum);
					}
					luaMgr.EndTable();
				}
				//LUA_GET_VALUE_DEF("EFF_ENUM", setTemplet.iItemEffEnum, -1 );

				//°¡Ã­ ¼¼Æ® ¾ÆÀÌÅÛ ÀÛ¾÷
				bool bGachaSetItem = true;
				LUA_GET_VALUE_DEF("GACHASETITEM", bGachaSetItem, false);
				if (bGachaSetItem)
				{
					setTemplet.bGachaSetItem = bGachaSetItem;
					LUA_BEGIN_TABLE("ACCESSORYS", return)
					{
						int iTemp = 0;
						for (int j = 1; ; j++)
						{
							LUA_GET_VALUE_NOASSERT(j, iTemp, break)
								setTemplet.vecAccessorys.push_back(iTemp);
						}
					}
					LUA_END_TABLE(return);
					LUA_BEGIN_TABLE("WEAPON", return)
					{
						int iTemp = 0;
						for (int j = 1; ; j++)
						{
							LUA_GET_VALUE_NOASSERT(j, iTemp, break)
								setTemplet.vecWeaponNum.push_back(iTemp);
						}
					}
					LUA_END_TABLE(return);
					LUA_BEGIN_TABLE("BUFF", return)
					{
						int iTemp = 0;
						for (int j = 1; ; j++)
						{
							LUA_GET_VALUE_NOASSERT(j, iTemp, break)
								setTemplet.vecBuffNum.push_back(iTemp);
						}
					}
					LUA_END_TABLE(return);
				}
				//------------------------------------------------------------------------
				// 20091023 [±¤Àå ¼¼Æ® ¾ÆÀÌÅÛ ¸ð¼Ç ÀÛ¾÷] : ÀÌµ¿¼®
				bool bSquareMotion = true;
				LUA_BEGIN_TABLE("SQUAREMOTION", bSquareMotion = false)
					if (bSquareMotion)
					{
						int	iID = setTemplet.iSetNum;
						int	iTest = -1;
						setTemplet.m_bSquareMotion = bSquareMotion;

						LUA_GET_VALUE_DEF("WALK", iTest, -1);
						GCSquarePeopleStateFunctions::ms_mapSquareWalkCharMotion[iID] = iTest;
						SETSQUAREFUNC(MOTIONID(iTest), GCSquarePeopleElesis::OnWalk);
						SETSQUAREFUNCREMOTE(MOTIONID(iTest), GCSquarePeopleCommon::OnWalkRemote);

						LUA_GET_VALUE_DEF("WAIT", iTest, -1);
						GCSquarePeopleStateFunctions::ms_mapSquareWaitCharMotion[iID] = iTest;
						SETSQUAREFUNC(MOTIONID(iTest), GCSquarePeopleElesis::OnWait);
						SETSQUAREFUNCREMOTE(MOTIONID(iTest), GCSquarePeopleCommon::OnWaitRemote);

						LUA_GET_VALUE_DEF("JUMP_READY", iTest, -1);
						GCSquarePeopleStateFunctions::ms_mapSquareJumpReadyCharMotion[iID] = iTest;
						SETSQUAREFUNC(MOTIONID(iTest), GCSquarePeopleElesis::OnJumpReady);
						SETSQUAREFUNCREMOTE(MOTIONID(iTest), GCSquarePeopleCommon::OnJumpReadyRemote);

						LUA_GET_VALUE_DEF("JUMP", iTest, -1);
						GCSquarePeopleStateFunctions::ms_mapSquareJumpCharMotion[iID] = iTest;
						SETSQUAREFUNC(MOTIONID(iTest), GCSquarePeopleCommon::OnJump);
						SETSQUAREFUNCREMOTE(MOTIONID(iTest), GCSquarePeopleCommon::OnJumpRemote);

						LUA_GET_VALUE_DEF("DASH", iTest, -1);
						GCSquarePeopleStateFunctions::ms_mapSquareDashCharMotion[iID] = iTest;
						SETSQUAREFUNC(MOTIONID(iTest), GCSquarePeopleElesis::OnDash);
						SETSQUAREFUNCREMOTE(MOTIONID(iTest), GCSquarePeopleElesis::OnDashRemote);

						LUA_END_TABLE(return);
					}
					else
					{
						setTemplet.m_bSquareMotion = false;
					}

				LUA_GET_VALUE_DEF("CHEERUPMOTION", setTemplet.m_bCheerUpMotion, false);
				//========================================================================

			}
			m_SetList.push_back(setTemplet);
			LUA_END_TABLE(return)

				//setTempley.vecSquareMotion À» ¸¸µé¾î¼­ Ãß°¡ÇÒ±î Çß´Âµ¥, º¯¼ö Á¢±ÙÀÌ ¾î·Á¿ö¼­ ÇÏÁö ¾ÊÀ½.
		}
	}
	LUA_END_TABLE(return)
}

void KGCItemManager::LoadCoordiSeasonList(void)
{
	if (!GC_GLOBAL_DEFINE::bReformCoordi)
		return;

	KLuaManager luaMgr;


	if (GCFUNC::LoadLuaScript(luaMgr, "CoordiSeasonSetItem.stg") == false)
		return;

	LUA_BEGIN_TABLE("COORDI_SEASON_ITEM", return);
	{
		for (int i = 1; ; i++)
		{
			CoordiSeasonTemplet seasonTemplet;
			LUA_BEGIN_TABLE(i, break)
			{
				LUA_GET_VALUE("SEASON_NUM", seasonTemplet.iSeasonNum, return);
				LUA_GET_VALUE("SEASON_TEXT_NUM", seasonTemplet.iSeasonText, return);
				LUA_GET_VALUE("SEASON_ITEM_GRADE", seasonTemplet.iSeasonGrade, return);

				LUA_BEGIN_TABLE("PARTS_LIST", return)
				{
					int iTemp = 0;
					for (int j = 1; ; j++)
					{
						LUA_GET_VALUE_NOASSERT(j, iTemp, break)
							seasonTemplet.setPartsList.insert(iTemp);
					}
				}
				LUA_END_TABLE(return);


				LUA_BEGIN_TABLE("ITEM_LIST", return)
				{
					int iTemp = 0;
					for (int j = 1; ; j++)
					{
						LUA_GET_VALUE_NOASSERT(j, iTemp, break)
							seasonTemplet.setItemList.insert(iTemp);
					}
				}
				LUA_END_TABLE(return);

				if (SUCCEEDED(luaMgr.BeginTable("ATTRIBUTE_LIST")))
				{
					for (int k = 1; ; ++k)
					{
						LUA_BEGIN_TABLE(k, break);
						{
							SetItemAttributeList stAttributeList;
							LUA_GET_VALUE_DEF("EQUIP_COUNT", stAttributeList.iEquipCount, seasonTemplet.setPartsList.size());
							if (SUCCEEDED(luaMgr.BeginTable("ATTRIBUTE")))
							{
								for (int p = 1; ; ++p)
								{
									LUA_BEGIN_TABLE(p, break);
									{
										SetItemAttribute stSetItemAttribute;
										int iAttributeType = 0;
										LUA_GET_VALUE_DEF("ATTRIBUTE_TYPE", iAttributeType, 0);
										stSetItemAttribute.eAttributeType = static_cast<EAttributeType>(iAttributeType);
										LUA_GET_VALUE_DEF("ABILITY", stSetItemAttribute.fAbility, 0.0f);
										stAttributeList.vecSetItemAttribute.push_back(stSetItemAttribute);
									}
									LUA_END_TABLE(return);
								}
								LUA_END_TABLE(return);
							}
							seasonTemplet.vecSetItemAtrributeList.push_back(stAttributeList);
						}
						LUA_END_TABLE(return);
					}
					LUA_END_TABLE(return);
				}
			}
			m_vecCoordiSensonList.push_back(seasonTemplet);
			LUA_END_TABLE(return)
		}
	}
	LUA_END_TABLE(return)
}

void KGCItemManager::LoadCoordiSetItemList(void)
{
	m_vecCoordiSetList.clear();
	KLuaManager luaMgr;

	//	Enum°ªÀ» »ç¿ëÇÏ±â À§ÇØ ·Îµå
	if (GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg") == false)
		return;

	//// ¹öÇÁµµ »ç¿ëÇÒ ¼ö ÀÖ°Ô ÇÏÀÚ
	//if( GCFUNC::LoadLuaScript( luaMgr , "BuffEnum.stg" ) == false )
	//    return;

	if (GCFUNC::LoadLuaScript(luaMgr, "CoordiSetItem.stg") == false)
		return;

	LUA_BEGIN_TABLE("COORDI_SET_ITEM", return);
	{
		for (int i = 1; ; i++)
		{
			KCoordiSetTemplet kCoordiSetTemplet;
			LUA_BEGIN_TABLE(i, break)
			{
				LUA_GET_VALUE("NAME", kCoordiSetTemplet.iSetNum, return);
				LUA_BEGIN_TABLE("PARTS", return)
				{
					int iTemp = 0;
					for (int j = 1; ; j++)
					{
						LUA_GET_VALUE_NOASSERT(j, iTemp, break)
							kCoordiSetTemplet.vecParts.push_back(iTemp);
					}
				}
				LUA_END_TABLE(return);
				LUA_GET_VALUE_DEF("SEQUENCE", kCoordiSetTemplet.strSequence, "");
				LUA_GET_VALUE_DEF("ADD_EFFECT", kCoordiSetTemplet.bAddEffect, "");
				LUA_GET_VALUE_DEF("MOTION_ID", kCoordiSetTemplet.iMotionID, -1);
			}
			m_vecCoordiSetList.push_back(kCoordiSetTemplet);
			LUA_END_TABLE(return)
		}
	}
	LUA_END_TABLE(return)
}

//------------------------------------------------------------------------
//	20091102 : ¿À´Ã °ø»ç±¸¿ª ½ÃÀÛ
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//	NAME : IsSetItemMotion()
//	DESC : ¼¼Æ® ¾ÆÀÌÅÛ ¾ÆÀÌµð·Î ±¤Àå ¸ð¼ÇÀÌ ÀÖ´ÂÁö È®ÀÎ
//------------------------------------------------------------------------
int KGCItemManager::IsSetItemMotion(const int iSetItemID_)
{
	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		if (iSetItemID_ == m_SetList[i].iSetNum)
		{
			return m_SetList[i].m_bSquareMotion;
		}
	}
	return false;
}


//------------------------------------------------------------------------
//	NAME : GetEquipSetItemTemplet()
//	DESC : ±âÁ¸ÀÇ Áßº¹ ÄÚµå¸¦ µû·Î ÇÔ¼ö·Î »©µÐ°Í
//	DATE : 20001102 ÀÌµ¿¼®
//------------------------------------------------------------------------
#define MATCH_FOR_FOR( v, b )							\
	match = 0;											\
	for( int k = 0; k < (int)v.size(); k++ )			\
	{													\
		for( size_t j = 0; j < vecSetItemSize; j++ )	\
		{												\
			if( vecSetItem[j] == v[k] )					\
				match++;								\
		}												\
	}													\
	if( match == (int)v.size() )						\
		b	=	true;									


#define MATCH_FOR_FOR_WEAPON( v, b )					\
	match = 0;											\
	for( int k = 0; k < (int)v.size(); k++ )			\
	{													\
		for( size_t j = 0; j < vecSetItemSize; j++ )	\
		{												\
			if( vecSetItem[j] == v[k] )					\
			match++;									\
		}												\
		if( match )										\
		{												\
			b = true;									\
			break;										\
		}												\
	}													

bool KGCItemManager::GetEquipCoordiSetItem(IN KCoordiSetTemplet& kCoordiSetTemplet, PLAYER* pPlayer)
{
	kCoordiSetTemplet.Init();

	if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
		return false;

	SCharInfo* pCharInfo = &(pPlayer->GetCurrentChar());
	if (pCharInfo == NULL)
		return false;

	std::vector< int > vecSetItem;
	const size_t vecSetItemSize = pCharInfo->vecLookInfo.size();
	for (size_t i = 0; i < vecSetItemSize; ++i)
		vecSetItem.push_back(pCharInfo->vecLookInfo[i].iItemID);

	for (int i = 0; i < (int)m_vecCoordiSetList.size(); i++)
	{
		if (pPlayer->m_iPlayerIndex > MAX_PLAYER_NUM)
			continue;

		int match = 0;
		bool bPartsMatch = false;

		MATCH_FOR_FOR(m_vecCoordiSetList[i].vecParts, bPartsMatch)

			if (bPartsMatch)
			{
				kCoordiSetTemplet = m_vecCoordiSetList[i];
				return true;
			}
	}

	return false;
}

bool KGCItemManager::GetEquipCoordiSetItem(IN KCoordiSetTemplet& kCoordiSetTemplet, KSquareUserInfo kPlayer)
{
	kCoordiSetTemplet.Init();

	if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
		return false;

	std::vector<int> vecSetItem;
	const size_t vecSetItemSize = kPlayer.m_vecEquips.size();
	for (size_t i = 0; i < vecSetItemSize; i++)
	{
		vecSetItem.push_back(kPlayer.m_vecEquips[i] / 10);
	}

	for (int i = 0; i < (int)m_vecCoordiSetList.size(); i++)
	{
		int match = 0;
		bool bPartsMatch = false;

		MATCH_FOR_FOR(m_vecCoordiSetList[i].vecParts, bPartsMatch)

			if (bPartsMatch)
			{
				kCoordiSetTemplet = m_vecCoordiSetList[i];
				return true;
			}
	}

	return false;
}

KGCItemManager::SetTemplet* KGCItemManager::GetEquipSetItemTemplet(std::vector<int>& vecSetItem, bool bDevide10)
{
	if (bDevide10)
	{
		size_t iLimit = vecSetItem.size();
		for (size_t i = 0; i < iLimit; ++i)
			vecSetItem[i] /= 10;
	}

	const size_t vecSetItemSize = vecSetItem.size();

	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		bool	bPartsMatch = false;
		bool	bAccessorysMatch = false;
		bool	bWeaponMatch = false;
		int		match = 0;

		MATCH_FOR_FOR(m_SetList[i].vecParts, bPartsMatch)

			if (m_SetList[i].vecAccessorys.size())
			{
				MATCH_FOR_FOR(m_SetList[i].vecAccessorys, bAccessorysMatch)
			}

		if (m_SetList[i].vecWeaponNum.size())
		{
			MATCH_FOR_FOR_WEAPON(m_SetList[i].vecWeaponNum, bWeaponMatch)
				if (bPartsMatch && bWeaponMatch && bAccessorysMatch)
				{
					return &m_SetList[i];	//	±âÁ¸ ÄÚµå	
				}
		}
		else
		{
			if (bPartsMatch || bAccessorysMatch)
			{
				return &m_SetList[i];	//	±âÁ¸ ÄÚµå	
			}
		}

	}
	return NULL;
}

void KGCItemManager::GetGaChaSetItemAbility(PLAYER* pPlayer)
{
	if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO)) return;
	SCharInfo* pCharInfo = &(pPlayer->GetCurrentChar());
	if (pCharInfo == NULL)
		return;

	std::vector< int > vecSetItem;
	const size_t vecSetItemSize = pCharInfo->vecItemInfo.size();
	for (size_t i = 0; i < vecSetItemSize; ++i)
		vecSetItem.push_back(pCharInfo->vecItemInfo[i].iItemID);


	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		if (!m_SetList[i].bGachaSetItem)
			continue;

		if (m_SetList[i].vecBuffNum.empty())
			continue;

		if (pPlayer->m_iPlayerIndex > MAX_PLAYER_NUM)
			continue;

		int match = 0;
		bool bPartsMatch = false;
		bool bAccessorysMatch = false;
		bool bWeaponMatch = false;

		MATCH_FOR_FOR(m_SetList[i].vecParts, bPartsMatch)

			if (bPartsMatch)
				g_pMagicEffect->SetMagicEffect(pPlayer->m_iPlayerIndex, m_SetList[i].vecBuffNum[0], 99999.0f);

		if (!m_SetList[i].vecAccessorys.empty())
		{
			MATCH_FOR_FOR(m_SetList[i].vecAccessorys, bAccessorysMatch)
		}

		if (bAccessorysMatch)
			g_pMagicEffect->SetMagicEffect(pPlayer->m_iPlayerIndex, m_SetList[i].vecBuffNum[1], 99999.0f);

		if (!m_SetList[i].vecWeaponNum.empty())
		{
			MATCH_FOR_FOR_WEAPON(m_SetList[i].vecWeaponNum, bWeaponMatch)
				if (bPartsMatch && bWeaponMatch && bAccessorysMatch)
					g_pMagicEffect->SetMagicEffect(pPlayer->m_iPlayerIndex, m_SetList[i].vecBuffNum[2], 99999.0f);
		}
	}
}

void KGCItemManager::GetGaChaSetItemAbility(PLAYER* pPlayer, TagCharacterInfo* pTagInfo)
{
	if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO)) return;
	SCharInfo* pCharInfo = &pPlayer->m_kUserInfo.GetCurrentChar(pTagInfo->m_iCharType);
	if (pCharInfo == NULL)
		g_kGlobalValue.m_strNullFunc = __FUNCTION__;

	std::vector< int > vecSetItem;
	const size_t vecSetItemSize = pCharInfo->vecItemInfo.size();
	for (size_t i = 0; i < vecSetItemSize; ++i)
		vecSetItem.push_back(pCharInfo->vecItemInfo[i].iItemID);

	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		if (!m_SetList[i].bGachaSetItem)
			continue;

		if (m_SetList[i].vecBuffNum.empty())
			continue;

		if (g_MyD3D->Get_MyPlayer() > MAX_PLAYER_NUM)
			continue;

		int match = 0;
		bool bPartsMatch = false;
		bool bAccessorysMatch = false;
		bool bWeaponMatch = false;

		MATCH_FOR_FOR(m_SetList[i].vecParts, bPartsMatch)

			if (bPartsMatch)
				g_pMagicEffect->SetMagicEffect(g_MyD3D->Get_MyPlayer(), m_SetList[i].vecBuffNum[0], 99999.0f);

		if (m_SetList[i].vecAccessorys.size())
		{
			MATCH_FOR_FOR(m_SetList[i].vecAccessorys, bAccessorysMatch)
		}

		if (bAccessorysMatch)
			g_pMagicEffect->SetMagicEffect(g_MyD3D->Get_MyPlayer(), m_SetList[i].vecBuffNum[1], 99999.0f);

		if (m_SetList[i].vecWeaponNum.size())
		{
			MATCH_FOR_FOR_WEAPON(m_SetList[i].vecWeaponNum, bWeaponMatch)
				if (bPartsMatch && bWeaponMatch && bAccessorysMatch)
					g_pMagicEffect->SetMagicEffect(g_MyD3D->Get_MyPlayer(), m_SetList[i].vecBuffNum[2], 99999.0f);
		}
	}
}

//------------------------------------------------------------------------
//	NAME : GetSet~()
//	DESC : ¼¼Æ® ¾ÆÀÌÅÛ ³Ñ¹ö È¤Àº ÀÌÆåÆ® ³Ñ¹ö¸¦ ±¸ÇÏ´Â ´ÙÃ¤·Î¿î ÇÔ¼öµé...
//			1Â÷ ¸®ÆÑÅä¸µ
//	DATE : 20091102 ÀÌµ¿¼®
//------------------------------------------------------------------------
int KGCItemManager::GetSetItemNumberByEquipVector(std::vector<DWORD>& rVecEquip)
{
	size_t iLimit = rVecEquip.size();
	std::vector< int > vecSetItem;
	for (size_t i = 0; i < iLimit; ++i)
		vecSetItem.push_back(static_cast<int>(rVecEquip[i]));

	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecSetItem, true);
	return (pSetTemplet) ? pSetTemplet->iSetNum : -1;
}

int KGCItemManager::GetEquipSetItemName(std::vector<int> vecItemInfo)
{
	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecItemInfo);
	return (pSetTemplet) ? pSetTemplet->iSetNum : -1;
}

int KGCItemManager::GetSetItemNumber(SCharInfo* pCharInfo)
{
	std::vector< int > vecSetItem;
	size_t	iLimit = pCharInfo->vecItemInfo.size();
	for (size_t i = 0; i < iLimit; ++i)
		vecSetItem.push_back(pCharInfo->vecItemInfo[i].iItemID);

	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecSetItem);
	return (pSetTemplet) ? pSetTemplet->iSetNum : -1;
}

int KGCItemManager::GetEquipSetItemName(PLAYER* pPlayer, bool bCoordi /*= false*/)
{
	SCharInfo* pCharInfo = &pPlayer->GetCurrentChar();
	std::vector< int > vecSetItem;

	if (bCoordi)
	{
		KItemIDVector vecItemInfo = pCharInfo->GetEquipmentForLoadModel();
		size_t	iLimit = vecItemInfo.size();
		for (size_t i = 0; i < iLimit; ++i)
			vecSetItem.push_back(vecItemInfo[i]);
	}
	else
	{
		KUserItemVector vecItemInfo = pCharInfo->vecItemInfo;
		size_t	iLimit = vecItemInfo.size();
		for (size_t i = 0; i < iLimit; ++i)
			vecSetItem.push_back(vecItemInfo[i].iItemID);
	}

	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecSetItem);
	return (pSetTemplet) ? pSetTemplet->iSetNum : -1;
}

int KGCItemManager::GetEquipSetItemName(PLAYER* pPlayer, TagCharacterInfo* pTagInfo, bool bCoordi /*= false*/)
{
	SCharInfo* pCharInfo = &pPlayer->m_kUserInfo.GetCurrentChar(pTagInfo->m_iCharType);
	std::vector< int > vecSetItem;


	if (bCoordi)
	{
		KItemIDVector vecItemInfo = pCharInfo->GetEquipmentForLoadModel();
		size_t	iLimit = vecItemInfo.size();
		for (size_t i = 0; i < iLimit; ++i)
			vecSetItem.push_back(vecItemInfo[i]);
	}
	else
	{
		KUserItemVector vecItemInfo = pCharInfo->vecItemInfo;
		size_t	iLimit = vecItemInfo.size();
		for (size_t i = 0; i < iLimit; ++i)
			vecSetItem.push_back(vecItemInfo[i].iItemID);
	}

	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecSetItem);
	return (pSetTemplet) ? pSetTemplet->iSetNum : -1;
}

std::vector<int> KGCItemManager::GetSetItemAbility(PLAYER* pPlayer)
{
	SCharInfo* pCharInfo = &pPlayer->GetCurrentChar();
	std::vector< int > vecSetItem;
	size_t	iLimit = pCharInfo->vecItemInfo.size();
	for (size_t i = 0; i < iLimit; ++i)
		vecSetItem.push_back(pCharInfo->vecItemInfo[i].iItemID);

	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecSetItem);
	std::vector<int> vecItemEff;
	if (pSetTemplet)
		vecItemEff = pSetTemplet->vecItemEffEnum;
	return vecItemEff;
}

std::vector<int> KGCItemManager::GetSetItemAbility(PLAYER* pPlayer, TagCharacterInfo* pTagInfo)
{
	SCharInfo* pCharInfo = &pPlayer->m_kUserInfo.GetCurrentChar(pTagInfo->m_iCharType);
	std::vector< int > vecSetItem;
	size_t	iLimit = pCharInfo->vecItemInfo.size();
	for (size_t i = 0; i < iLimit; ++i)
		vecSetItem.push_back(pCharInfo->vecItemInfo[i].iItemID);

	KGCItemManager::SetTemplet* pSetTemplet = GetEquipSetItemTemplet(vecSetItem);
	std::vector<int> vecItemEff;
	if (pSetTemplet)
		vecItemEff = pSetTemplet->vecItemEffEnum;
	return vecItemEff;
}
//------------------------------------------------------------------------
//	20091102 : ¿À´Ã °ø»ç±¸¿ª ³¡
//------------------------------------------------------------------------



std::set<int> KGCItemManager::GetSetEffectItemName(GCITEMID ItemID_, std::map< int, std::vector< int > >* pMapPartsList_)
{
	std::set<int> setSetNum;
	std::vector<SetTemplet>::iterator vitSetTemplate = m_SetList.begin();

	for (; vitSetTemplate != m_SetList.end(); ++vitSetTemplate)
	{
		std::vector< int >::iterator vitParts = std::find((*vitSetTemplate).vecParts.begin(), (*vitSetTemplate).vecParts.end(), ItemID_);
		if ((*vitSetTemplate).vecParts.end() != vitParts)
		{
			if (pMapPartsList_)
			{
				std::map< int, std::vector< int > >::iterator mit = pMapPartsList_->find((*vitSetTemplate).iSetNum);
				if (pMapPartsList_->end() == mit)
				{
					pMapPartsList_->insert(std::map< int, std::vector< int > >::value_type((*vitSetTemplate).iSetNum, (*vitSetTemplate).vecParts));
				}
				else
				{
					mit->second = (*vitSetTemplate).vecParts;
				}
			}

			setSetNum.insert((*vitSetTemplate).iSetNum);
		}
	}
	return setSetNum;
}

bool KGCItemManager::GetEquipSetItemAttributeBySetName(int iSetItemName_, OUT std::vector< SetItemAttributeList >& vecSetAttribute_)
{
	std::vector<SetTemplet>::iterator vitSetTemplate = std::find_if(m_SetList.begin(), m_SetList.end(), boost::bind(&SetTemplet::iSetNum, _1) == iSetItemName_);

	if (m_SetList.end() == vitSetTemplate)
		return false;

	vecSetAttribute_ = (*vitSetTemplate).vecSetItemAtrributeList;
	return true;
}

bool KGCItemManager::GetCoordiSeasonAttribute(int iSeasonNum_, OUT std::vector< SetItemAttributeList >& vecSetAttribute_)
{
	std::vector<CoordiSeasonTemplet>::iterator vitSetTemplate = std::find_if(m_vecCoordiSensonList.begin(), m_vecCoordiSensonList.end(),
		boost::bind(&CoordiSeasonTemplet::iSeasonNum, _1) == iSeasonNum_);

	if (m_vecCoordiSensonList.end() == vitSetTemplate)
		return false;

	vecSetAttribute_ = (*vitSetTemplate).vecSetItemAtrributeList;
	return true;
}

bool KGCItemManager::GetShopItem(int iShopItem, GCItem*& pItem)
{
	if (iShopItem >= (int)m_vecShopItemList.size() || iShopItem < 0)
	{
		return false;
	}
	pItem = m_mapDB[m_vecShopItemList[iShopItem]];

	g_pItemMgr->CheckAndChangeEventItem(pItem); //ÀÌº¥Æ® ¾ÆÀÌÅÛÀÌ¶ó¸é ¹Ù²Û´Ù.

	return true;
}


void KGCItemManager::UpdateUserItem(SUserInfo* pUserInfo)
{
	if (!pUserInfo) {
		return;
	}

	KEquipUser kEquip;
	kEquip.m_strLogin = g_kGlobalValue.m_kUserInfo.strLogin;

	// 	for( int iChar = 0; iChar < pUserInfo->GetCharSize(); iChar++)
	// 	{
	// 		SCharInfo& kCharInfo = pUserInfo->vecCharInfo[iChar];
	SCharInfo& kCharInfo = pUserInfo->GetCurrentChar();
	KChangeEquipment kChangeEquipment;
	// ÆêÁ¤º¸ ´Ù ³Ö¾îÁà¾ßÇÑ´Ù
	kChangeEquipment.m_equipPetInfo = kCharInfo.kPetInfo;
	kChangeEquipment.m_equipPetInfo.m_dwID = kCharInfo.kPetInfo.m_dwID * 10;
	kChangeEquipment.m_equipPetInfo.m_dwUID = kCharInfo.kPetInfo.m_dwUID;

	//±â°£ÀÌ ´Ù µÇ¾ú´Ù¸é ±»¹ÙÀÌ~
	if (!m_kInventory.FindItemByItemUID(kCharInfo.kPetInfo.m_kCostume.m_dwUID))
	{
		kChangeEquipment.m_equipPetInfo.m_kCostume.m_dwID = 0;
		kChangeEquipment.m_equipPetInfo.m_kCostume.m_dwUID = 0;
	}
	else
	{
		kChangeEquipment.m_equipPetInfo.m_kCostume.m_dwID = kCharInfo.kPetInfo.m_kCostume.m_dwID * 10;
		kChangeEquipment.m_equipPetInfo.m_kCostume.m_dwUID = kCharInfo.kPetInfo.m_kCostume.m_dwUID;
	}
	// Æê ¾ÆÀÌÅÛµµ ³¢¾Æ Áà¾ßÁö~
	std::vector<KSimpleItem> KSimplePetItem;
	KSimplePetItem = kCharInfo.kPetInfo.m_vecEquipItem;
	std::vector<KSimpleItem>::iterator vitTmp = KSimplePetItem.begin();
	for (; vitTmp != KSimplePetItem.end(); ++vitTmp)
	{
		vitTmp->m_dwID *= 10;
		vitTmp->m_dwUID = vitTmp->m_dwUID;
	}

	kChangeEquipment.m_equipPetInfo.m_vecEquipItem = KSimplePetItem;
	// ÄÁÅ×ÀÌ³Ê´Â ´Ù »ý¼ºÇØÁà¾ßÇÑ´Ù
	kEquip.m_mapEquipInfo.insert(std::make_pair(kCharInfo.iCharType, kChangeEquipment));

	KUserItemVector::iterator vitEquipitem = kCharInfo.vecItemInfo.begin();
	while (vitEquipitem != kCharInfo.vecItemInfo.end())
	{
		KItem* kInvenItem = m_kInventory.FindItemByItemUID(vitEquipitem->dwItemUID);

		if (!IsBaseItem(vitEquipitem->iItemID) && kInvenItem == NULL)
		{
			{ // ¾Æ·¡ ºê·¹ÀÌ½º¿¡ ¼±¾ðÇÑ º¯¼ö¸¦ ¿ÜºÎ¿¡¼­ »ç¿ë ÇÏÁö ¸øÇÏ°Ô ÇÏ±â À§ÇØ.
				KEquipItemInfo kReleaseItem;
				vitEquipitem->Copy(kReleaseItem);
				kEquip.m_mapEquipInfo[kCharInfo.iCharType].m_vecReleaseItems.push_back(kReleaseItem);
			}

			if (IsRallyTitleItemID(vitEquipitem->iItemID))
				g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(-1);
			vitEquipitem = kCharInfo.vecItemInfo.erase(vitEquipitem);
		}
		else
		{
			++vitEquipitem;
		}
	} // end of while
	EquipBaseItem(&kCharInfo);

#ifdef WEAPONCHANGE
	std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit = g_MyD3D->m_TempPlayer.m_mapSwapWeapon.find(kCharInfo.iCharType);
	if (g_MyD3D->m_TempPlayer.m_mapSwapWeapon.end() != mit)
	{
		KItem* pKItem = NULL;
		if (pKItem = g_pItemMgr->m_kInventory.FindItemByItemUID((*mit).second.second.second))
		{
			kEquip.m_mapEquipInfo[kCharInfo.iCharType].m_kChangeWeaponItem = g_kGlobalValue.ConvertKItemtoKEquipItemInfo(pKItem);
		}
	}
#endif
	//	}
	kEquip.m_cCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
	KP2P::GetInstance()->Send_EquipItemReq(kEquip);
	//g_MyD3D->SendEquipItemList();
	//ÀÎº¥Åä¸®¿¡ ¾÷µ¥ÀÌÆ®µÈ ¾ÆÀÌÅÛÀ» Àû¿ëÇÑ´Ù.
}

void KGCItemManager::UpdateShopItemList()
{
	if (m_shopListLastLog.CharType == GC_CHAR_INVALID)
		return;

	MakeShopItemList(m_shopListLastLog.CharType
		, m_shopListLastLog.ItemType
		, m_shopListLastLog.iCategory
		, m_shopListLastLog.iSortType
		, m_shopListLastLog.iMoneyType
		, m_shopListLastLog.strName.c_str());

}

void KGCItemManager::MakeShopItemList(EGCCharType CharType, int ItemType,
	int iCategory, int iSortType, int iMoneyType,
	const WCHAR* strName_)
{
	m_shopListLastLog.CharType = CharType;
	m_shopListLastLog.ItemType = ItemType;
	m_shopListLastLog.iCategory = iCategory;
	m_shopListLastLog.iSortType = iSortType;
	m_shopListLastLog.iMoneyType = iMoneyType;
	if (strName_)
		m_shopListLastLog.strName = strName_;
	else
		m_shopListLastLog.strName.clear();



	//ÀüÃ¼ ¾ÆÀÌÅÛ Áß¿¡ À§ Á¶°Ç¿¡ ¸Â´Â°ÍÀÇ ÀÎµ¦½º¸¦ º¤ÅÍ¿¡ ´ã´Â´Ù.
	DWORD dwCategory = 0;

	switch (ItemType)
	{
	case SIT_ARMOR:
	{
		dwCategory = ESP_HELMET // ¸Ó¸®
			| ESP_JACKET // »óÀÇ
			| ESP_PANTS // ÇÏÀÇ
			| ESP_GLOVES // Àå°©
			| ESP_SHOES // ½Å¹ß
			| ESP_MANTLE; // ¸ÁÅä
		break;
	}
	case SIT_SET:
	{
		dwCategory = ESP_SET_ITEM; // ¼¼Æ®
		break;
	}
	case SIT_WEAPON:
	{
		dwCategory = ESP_WEAPON; // ¹«±â
		break;
	}
	case SIT_ACCESSORY:
	case SIT_ACCESSORY2:
	{
		dwCategory = ESP_A_UPPER_HELMET
			| ESP_A_DOWN_HELMET
			| ESP_A_NECKLET
			| ESP_A_JACKET
			| ESP_A_PANTS
			| ESP_A_ARM
			| ESP_A_FINGER
			| ESP_A_SHOES;
		break;
	}
	case SIT_QUEST:
	{
		dwCategory = ESP_SKC_WHITE_ONE
			| ESP_SKC_WHITE_TWO
			| ESP_SKC_WHITE_THREE
			| ESP_SKC_DARK_ONE
			| ESP_SKC_DARK_TWO
			| ESP_SKC_DARK_THREE
			| ESP_SKC_WHITE_CHANGEJOB_ONE
			| ESP_SKC_WHITE_CHANGEJOB_TWO
			| ESP_SKC_WHITE_CHANGEJOB_THREE
			| ESP_SKC_DARK_CHANGEJOB_ONE
			| ESP_SKC_DARK_CHANGEJOB_TWO
			| ESP_SKC_DARK_CHANGEJOB_THREE
			| ESP_SKC_WHITE_CHANGEJOB2_ONE
			| ESP_SKC_WHITE_CHANGEJOB2_TWO
			| ESP_SKC_WHITE_CHANGEJOB2_THREE
			| ESP_SKC_DARK_CHANGEJOB2_ONE
			| ESP_SKC_DARK_CHANGEJOB2_TWO
			| ESP_SKC_DARK_CHANGEJOB2_THREE
			| ESP_SLOT_ITEM
			| ESP_SLOT_EMOT;
		break;
	}
	case SIT_PET:
	{
		dwCategory = ESP_PET;
		break;
	}
#if defined( COUPLE_SYSTEM )
	case SIT_COUPLE_ITEM:
	{
		dwCategory = ESP_HELMET // ¸Ó¸®
			| ESP_JACKET // »óÀÇ
			| ESP_PANTS // ÇÏÀÇ
			| ESP_SHOES; // ½Å¹ß
		break;
	}
#else
	case SIT_PETITEM:
	{
		dwCategory = EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM
			| EPET_EQUIP_ITEM_SLOT_SKILL;
		break;
	}
#endif
	case SIT_POINTITEM:
	{
		break;
	}
	}

	DWORD charFlag = GetCharFlag(CharType);

	m_vecShopItemList.clear();
	std::wstring strName;
	if (strName_ != NULL)
		strName = strName_;

	std::set<GCITEMID> setTempExclusionItem;
	for (std::map<int, GCITEMID>::iterator mit = g_kGlobalValue.m_mapCharCashGoods.begin(); mit != g_kGlobalValue.m_mapCharCashGoods.end(); ++mit) {
		setTempExclusionItem.insert(mit->second);
	}

	//Ä³¸¯ÅÍ ½½·Ô È®Àå ¾ÆÀÌÅÛ Ã³¸®
	setTempExclusionItem.insert(ITEM_ADD_SLOT_ITEM);

#ifdef ENABLE_POINT_SYSTEM // Kawan>
	stdext::hash_map< GCITEMID, GCItem* >::iterator mit;
	if (ItemType == SIT_POINTITEM)
	{
		MakePointShopItemList();

		for (int i = 0; i < (int)GetPointShopSize(); ++i)
		{
			DWORD ItemKey_ = m_vecPointShop[i].iGoodsID;
			mit = m_mapDB.find(ItemKey_);
			if (mit == m_mapDB.end())
				continue;
			CONTINUE_NIL(mit->second);

			if (!(mit->second->dwCharType & charFlag))
				continue;

			m_vecShopItemList.push_back(ItemKey_);
		}
		return;
	}
#endif

	EGCWorldMap eCurrentType = SiKGCWorldMapManager()->GetCurrentWorldMap();
	for (int i = 0; i < (int)m_dwNumItem; ++i)
	{
		DWORD ItemKey_ = i;
		mit = m_mapDB.find(ItemKey_);

		if (mit == m_mapDB.end())
			continue;

		GCItem* kItem = mit->second;
		CONTINUE_NIL(kItem);

#if defined( EVENT_VIEW_ITEM )
		if (g_kGlobalValue.IsAdmin() == false)
		{
			//이벤트 아이템 뷰 리스트에 추가해야하는 항목인지 검사한다. 추가되는 항목은 m_vecPointShop리스트에서 제외시킨다.
			if (AddEventTimeViewItemList(kItem->dwGoodsID))
				continue;
		}
#endif

		if (kItem->bNotBuyItem)
			continue;

		if (!(kItem->dwCharType & charFlag))
			continue;

		if (iMoneyType != -1 && iMoneyType != 0 && kItem->eMoneyType != (iMoneyType - 1))
			continue;

		// Kawan>
#ifndef MERGE_COORDI_SHOP
		if (kItem->eItemKind == GIK_COORDI_ITEM || kItem->eItemKind == GIK_COORDI_PACKAGE7 || kItem->eItemKind == GIK_COORDIBOX)
			continue;
#endif

#if defined( USE_AGIT_SYSTEM )
		if (true == SiKGCAgitGameManager()->IsAgitObject(kItem->dwGoodsID)) {
			continue;
		}
#endif
		//  Æ¯Á¤ Ã¤³ÎÀ¯Àú¸¸ »ì¼ö ÀÖ´Â ¾ÆÀÌÅÛ Ã¼Å©
		if (IsChannelItemList(kItem->dwGoodsID, g_kGlobalValue.m_dwChannelType))
			continue;

		if (setTempExclusionItem.find(kItem->dwGoodsID) != setTempExclusionItem.end())
			continue;


		if (ItemType == SIT_QUEST)
		{
			if (kItem->eItemKind == GIK_CERTIFICATE ||
				kItem->eItemKind == GIK_RECIPE || //·¹½ÃÇÇµµ ÆÈ°Ô ÇØ´Þ¶óÇØ¼­ ÀÏ´Ü Ãß°¡ÇÕ´Ï´Ù-0-;
				kItem->eItemKind == GIK_MAGIC_PAPER ||
				kItem->eItemKind == GIK_MISSION_SCROLL ||
				kItem->eItemKind == GIK_OPEN_UP_ITEM ||
				kItem->eItemKind == GIK_TREASUREBOX_ITEM ||
				kItem->eItemKind == GIK_COORDIBOX || kItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM ||  // Ä«¿À½ººÎÅÍ ³°Àº ·éÀ» »óÁ¡¿¡¼­ ÆÇ¸ÅÇÕ´Ï´Ù. 
				(kItem->eItemKind == GIK_NORMAL && kItem->dwSlotPosition == 0) ||
				(kItem->dwGoodsID == GEM_ITEM_ID) || // ÁªÀÌ ÆêÇªµå·Î ¾ÆÀÌÅÛ Ä«ÀÎµå°¡ º¯°æµÇ¾î ¿ø·¡ÀÖ´ø ÈÆ·Ã¼Ò¿¡ º¸ÀÌ°Ô ÇØÁÖ±â À§ÇØ=¤µ=;;
				kItem->dwSkillPosition != ESP_SKC_INVALID)
			{
				// Æê°ü·Ã »óÇ°ÀÌ´Ï Æê¼¥¿¡ º¸ÀÌ°Ô ÇÏÀÚ
				if (IsPetTabItem(kItem->dwGoodsID))
					continue;

				if (strName_ != NULL)
				{
					wstring strLowerName = strName;
					wstring strLowerItemName = kItem->strItemName;

					GCUTIL_STR::ToLower(strLowerName);
					GCUTIL_STR::ToLower(strLowerItemName);

					if (strLowerItemName.find(strLowerName) == std::wstring::npos)
						continue;
				}
#ifndef DISABLE_MAP_SHOP // Kawan>
				if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
				{
					if (kItem->eMoneyType == EMT_GAMEPOINT)
					{
						if (CheckItemContinentType(kItem, eCurrentType))
							if (!IsCustomizationItem(kItem->dwGoodsID * 10))
								m_vecShopItemList.push_back(kItem->dwGoodsID);
					}
				}
#endif
				else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
				{
#ifndef DISABLE_MAP_SHOP // Kawan>
					if (kItem->eMoneyType != EMT_GAMEPOINT)
#endif
						if (!IsCustomizationItem(kItem->dwGoodsID * 10))
							m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}
		}
#if defined( COUPLE_SYSTEM )
		else if (ItemType == SIT_PET)
		{
			dwCategory = ESP_PET;
			if ((kItem->dwSlotPosition & dwCategory || IsPetTabItem(kItem->dwGoodsID)))
			{
				if (strName_ != NULL)
				{
					wstring strLowerName = strName;
					wstring strLowerItemName = kItem->strItemName;

					GCUTIL_STR::ToLower(strLowerName);
					GCUTIL_STR::ToLower(strLowerItemName);

					if (strLowerItemName.find(strLowerName) == std::wstring::npos)
						continue;
				}
#ifndef DISABLE_MAP_SHOP // Kawan>
				if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
				{
					if (kItem->eMoneyType == EMT_GAMEPOINT)
					{
						if (CheckItemContinentType(kItem, eCurrentType))
							if (!IsCustomizationItem(kItem->dwGoodsID * 10))
								m_vecShopItemList.push_back(kItem->dwGoodsID);
					}
				}
#endif
				else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
				{
#ifndef DISABLE_MAP_SHOP // Kawan>
					if (kItem->eMoneyType != EMT_GAMEPOINT)
#endif
						if (!IsCustomizationItem(kItem->dwGoodsID * 10))
							m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}

			dwCategory = EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM
				| EPET_EQUIP_ITEM_SLOT_SKILL;
			if ((kItem->dwPetEquipItemSlot & dwCategory
				|| (kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_FOOD)))
			{
				if (strName_ != NULL)
				{
					wstring strLowerName = strName;
					wstring strLowerItemName = kItem->strItemName;

					GCUTIL_STR::ToLower(strLowerName);
					GCUTIL_STR::ToLower(strLowerItemName);

					if (strLowerItemName.find(strLowerName) == std::wstring::npos)
						continue;
				}
#ifndef DISABLE_MAP_SHOP // Kawan>
				if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
				{
					if (kItem->eMoneyType == EMT_GAMEPOINT)
					{
						if (CheckItemContinentType(kItem, eCurrentType))
							if (!IsCustomizationItem(kItem->dwGoodsID * 10))
								m_vecShopItemList.push_back(kItem->dwGoodsID);
					}
				}
#endif
				else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
				{
#ifndef DISABLE_MAP_SHOP // Kawan>
					if (kItem->eMoneyType != EMT_GAMEPOINT)
#endif
						if (!IsCustomizationItem(kItem->dwGoodsID * 10))
							m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}
		}
		else if (ItemType == SIT_COUPLE_ITEM)
		{
			if (kItem->eItemKind == GIK_COUPLE_ITEM)
			{
				if (strName_ != NULL)
				{
					wstring strLowerName = strName;
					wstring strLowerItemName = kItem->strItemName;

					GCUTIL_STR::ToLower(strLowerName);
					GCUTIL_STR::ToLower(strLowerItemName);

					if (strLowerItemName.find(strLowerName) == std::wstring::npos)
						continue;
				}
#ifndef DISABLE_MAP_SHOP // Kawan>
				if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
				{
					if (kItem->eMoneyType == EMT_GAMEPOINT)
					{
						if (CheckItemContinentType(kItem, eCurrentType))
							if (!IsCustomizationItem(kItem->dwGoodsID * 10))
								m_vecShopItemList.push_back(kItem->dwGoodsID);
					}
				}
#endif
				else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
				{
#ifndef DISABLE_MAP_SHOP // Kawan>
					if (kItem->eMoneyType != EMT_GAMEPOINT)
#endif
						if (!IsCustomizationItem(kItem->dwGoodsID * 10))
							m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}
		}
#else
		else if (ItemType == SIT_PET)
		{
			dwCategory = ESP_PET;
			if ((kItem->dwSlotPosition & dwCategory || IsPetTabItem(kItem->dwGoodsID)))
			{
				if (strName_ != NULL)
				{
					wstring strLowerName = strName;
					wstring strLowerItemName = kItem->strItemName;

					GCUTIL_STR::ToLower(strLowerName);
					GCUTIL_STR::ToLower(strLowerItemName);

					if (strLowerItemName.find(strLowerName) == std::wstring::npos)
						continue;
				}
				if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
				{
					if (kItem->eMoneyType != EMT_CASH)
					{
						if (CheckItemContinentType(kItem, eCurrentType))
							m_vecShopItemList.push_back(kItem->dwGoodsID);
					}
				}
				else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
				{
					if (kItem->eMoneyType != EMT_GAMEPOINT)
						m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}

			dwCategory = EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM
				| EPET_EQUIP_ITEM_SLOT_SKILL;
			if ((kItem->dwPetEquipItemSlot & dwCategory
				|| (kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_FOOD)))
			{
				if (strName_ != NULL)
				{
					if (kItem->strItemName.find(strName) == std::wstring::npos)
						continue;
				}
				if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
				{
					if (kItem->eMoneyType == EMT_GAMEPOINT)
					{
						if (CheckItemContinentType(kItem, eCurrentType))
							m_vecShopItemList.push_back(kItem->dwGoodsID);
					}
				}
				else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
				{
					if (kItem->eMoneyType != EMT_GAMEPOINT)
						m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}
		}
#endif
		else if (kItem->dwSlotPosition & dwCategory)
		{
			if (SIT_SET != ItemType && (kItem->dwSlotPosition & ESP_SET_ITEM))
				continue;
			if (kItem->eItemKind == GIK_COUPLE_ITEM)
				continue;

			if (strName_ != NULL)
			{
				wstring strLowerName = strName;
				wstring strLowerItemName = kItem->strItemName;

				GCUTIL_STR::ToLower(strLowerName);
				GCUTIL_STR::ToLower(strLowerItemName);

				if (strLowerItemName.find(strLowerName) == std::wstring::npos)
					continue;
			}
#ifndef DISABLE_MAP_SHOP // Kawan>
			if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP)
			{
				if (kItem->eMoneyType == EMT_GAMEPOINT)
				{
					if (CheckItemContinentType(kItem, eCurrentType))
						m_vecShopItemList.push_back(kItem->dwGoodsID);
				}
			}
#endif
			else if (g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH)
			{
#ifndef DISABLE_MAP_SHOP // Kawan>
				if (kItem->eMoneyType != EMT_GAMEPOINT)
#endif
					m_vecShopItemList.push_back(kItem->dwGoodsID);
			}
		}
	}

	// ¼¥¿¡¼­ ÀÌ³ðÀ» Áö¿î´Ù
#if defined( NOT_SELL_SKILL_LOCK_KEY_IN_SHOP )
	m_vecShopItemList.erase(std::remove_if(m_vecShopItemList.begin(), m_vecShopItemList.end(),
		std::bind2nd(std::equal_to<DWORD>(), ITEM_ID_OPEN_BLOCKSKILLGROUP)), m_vecShopItemList.end());
#endif

	CategoryGrouping(m_vecShopItemList, ItemType, iCategory);
	SortTypeGrouping(m_vecShopItemList, iSortType);
}


bool KGCItemManager::CheckItemContinentType(GCItem* gcItem, int  iCurrentMapType)
{
	int iContinentType = gcItem->shContinentType;
	std::vector<EGCWorldMap> vecContinentType;

	if (iContinentType < 0)
		return false;
	else if (iContinentType == 0)
		return true;

	if (iContinentType & KManufactureItem::ECF_BERMESIA_SERDIN)
		vecContinentType.push_back(WORLDMAP_BERMESIA_SERDIN);

	if (iContinentType & KManufactureItem::ECF_BERMESIA_CANAVAN)
		vecContinentType.push_back(WORLDMAP_BERMESIA_CANAVAN);

	if (iContinentType & KManufactureItem::ECF_SILVERLAND)
		vecContinentType.push_back(WORLDMAP_SILVERLAND);

	if (iContinentType & KManufactureItem::ECF_ELIA)
		vecContinentType.push_back(WORLDMAP_ELIA);

	if (iContinentType & KManufactureItem::ECF_XENIA)
		vecContinentType.push_back(WORLDMAP_XENEA);

	if (iContinentType & KManufactureItem::ECF_ATUM)
		vecContinentType.push_back(WORLDMAP_ATUM);

	if (iContinentType & KManufactureItem::ECF_ACHAEMEDIA)
		vecContinentType.push_back(WORLDMAP_ACHAEMEDIA);

	if (iContinentType & KManufactureItem::ECF_FLOSLAND)
		vecContinentType.push_back(WORLDMAP_FLOSLAND);

	if (iContinentType & KManufactureItem::ECF_DEVILDOM)
		vecContinentType.push_back(WORLDMAP_DEVILDOM);

	if (iContinentType & KManufactureItem::ECF_ELYOS)
		vecContinentType.push_back(WORLDMAP_ELYOS);

	//	if (iContinentType & KManufactureItem::ECF_ORIGIN)
	//		vecContinentType.push_back(WORLDMAP_ORIGIN);

	for (std::vector<EGCWorldMap>::iterator vitContinentType = vecContinentType.begin(); vitContinentType != vecContinentType.end(); ++vitContinentType)
	{
		//Á¦ÀÛ ¾ÆÀÌÅÛ ´ë·ú Å¸ÀÔÀÌ ÇöÀç À§Ä¡ÇÑ ´ë·ú°ú µ¿ÀÏÇÑ °æ¿ì
		if ((*vitContinentType) == SiKGCWorldMapManager()->GetCurrentWorldMap())
			return true;
		//´ë·úº°·Î ¸ðµÎ º¸¿©¾ßÇÒ °ø¿ë Á¦ÀÛ ¾ÆÀÌÅÛÀÎ °æ¿ì
		else
			continue;
	}
	//ÇöÀç À§Ä¡ÇÑ ´ë·ú¿¡¼­ º¸ÀÌÁö ¸»¾Æ¾ßÇÒ °æ¿ì
	return false;
}

bool KGCItemManager::IsEventTimeViewItem(DWORD _dwItemID)
{
	for (std::vector< EVENTITEMVIEW >::iterator itor = m_vecEventItemView.begin(); itor != m_vecEventItemView.end(); ++itor) {
		if (itor->dwViewItem == _dwItemID) {
			CTime timeStart(itor->timeStart);
			CTime timeEnd(itor->timeEnd);
			CTime timeCur(g_kGlobalValue.m_tmServerTime);
			if (timeStart < timeCur && timeCur < timeEnd) {
				return true;
			}

			if (timeStart > timeCur) {
				return true;
			}
		}
	}
	return false;
}



bool KGCItemManager::AddEventTimeViewItemList(DWORD dwGoodID)
{
	//return true : »óÁ¡¿¡ µî·Ï ¾ÈµÈ´Ù..
	//return false : »óÁ¡¿¡ µî·ÏµÈ´Ù.

	bool bReturn = false;
	for (std::vector< EVENTITEMVIEW >::iterator itor = m_vecEventItemView.begin(); itor != m_vecEventItemView.end(); ++itor) {
		if (itor->dwViewItem == dwGoodID) {
			CTime timeStart(itor->timeStart);
			CTime timeEnd(itor->timeEnd);
			CTime timeCur(g_kGlobalValue.m_tmServerTime);
			if (timeStart < timeCur && timeCur < timeEnd) {               //º¸¿©Áö´Â ½Ã°£Áß¿¡ ÀÖ´Ù¸é ¾ÆÀÌÅÛ»õ·Î°íÄ¥ ½Ã°£ ¸ñ·Ï¿¡ Ãß°¡
				m_setApplyRefreshEventTimeView.insert(itor->timeEnd);
				return false;
			}

			if (timeStart > timeCur) {                                    //¾ÕÀ¸·Î º¸¿©Áú ½Ã°£ÀÌ¶ó¸é ¾ÆÀÌÅÛ»õ·Î°íÄ¥ ½Ã°£ ¸ñ·Ï¿¡ Ãß°¡.
				m_setApplyRefreshEventTimeView.insert(itor->timeStart);     //»õ·Î°íÄ¥½Ã°£¿¡ µî·ÏÀº ÇÏÁö¸¸ »óÁ¡¿¡´Â º¸ÀÌ°Ô ÇÑ´Ù.
			}
			bReturn = true; //ÀÌº¥Æ® ½Ã°£ÀÌ ³¡³µ´Ù¸é µî·Ï ¾ÈµÈ´Ù.
		}
	}
	return bReturn;
}
void KGCItemManager::CategoryGrouping(std::vector<DWORD>& vecItemList, int ItemType, int iCategory)
{
	if ((iCategory == -1) || (iCategory == 0))
		return;

	DWORD dwCategory = 0;

	if (ItemType == SIT_WEAPON)
	{
		for (int i = (int)m_vecShopItemList.size() - 1; i >= 0; i--)
		{
			if (m_mapDB[vecItemList[i]]->iNeedChangeJobLevelForEquippingItem != (iCategory - 1))
				vecItemList.erase(vecItemList.begin() + i);
		}
		return;
	}
	else if (ItemType == SIT_ARMOR)
	{
		switch (iCategory) {
		case 1:
			dwCategory = ESP_HELMET;
			break;
		case 2:
			dwCategory = ESP_JACKET;
			break;
		case 3:
			dwCategory = ESP_PANTS;
			break;
		case 4:
			dwCategory = ESP_GLOVES;
			break;
		case 5:
			dwCategory = ESP_SHOES;
			break;
		case 6:
			dwCategory = ESP_MANTLE;
			break;
		default:
			break;
		}
	}
	else if (ItemType == SIT_ACCESSORY)
	{
		switch (iCategory) {
		case 1:
			dwCategory = ESP_A_UPPER_HELMET;
			break;
		case 2:
			dwCategory = ESP_A_DOWN_HELMET;
			break;
		case 3:
			dwCategory = ESP_A_NECKLET;
			break;
		case 4:
			dwCategory = ESP_A_JACKET;
			break;
		case 5:
			dwCategory = ESP_A_PANTS;
			break;
		case 6:
			dwCategory = ESP_A_ARM;
			break;
		case 7:
			dwCategory = ESP_A_FINGER;
			break;
		case 8:
			dwCategory = ESP_A_SHOES;
			break;
		case 9:
			dwCategory = ESP_A_EARRING;
			break;
		default:
			break;
		}
	}
#if defined( COUPLE_SYSTEM )
	else if (ItemType == SIT_COUPLE_ITEM)
	{
		switch (iCategory) {
		case 1:
			dwCategory = ESP_A_FINGER;
			break;
		case 2:
			dwCategory = ESP_HELMET;
			break;
		case 3:
			dwCategory = ESP_JACKET;
			break;
		case 4:
			dwCategory = ESP_PANTS;
			break;
		case 5:
			dwCategory = ESP_SHOES;
		}
	}
#endif

	for (int i = (int)vecItemList.size() - 1; i >= 0; i--)
	{
		if (dwCategory != m_mapDB[vecItemList[i]]->dwSlotPosition)
		{
			vecItemList.erase(vecItemList.begin() + i);
		}
	}
	return;
}

void KGCItemManager::SortTypeGrouping(std::vector<DWORD>& vecItemList, int iSortType)
{
	switch (iSortType)
	{
	case SIS_NEW:
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortNewItem());
		break;
	}
	case SIS_HIGH_LV: //·¹º§ ³ôÀº¼ø
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortLevelHigh());
		break;
	}
	case SIS_LOW_LV: //·¹º§ ³·Àº¼ø
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortLevelLow());
		break;
	}
	break;
	case SIS_NAME: //ÀÌ¸§¼ø
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortName());
		break;
	}
	case SIS_DEFAULT:
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortLowLevelLowItemIDHighRecom());
		break;
	}
	case SIS_RECOM:
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortRecom());
		break;
	}
	case SIS_KIND:
	{
		std::sort(vecItemList.begin(), vecItemList.end(), KItemSortKind());
		break;
	}
	default:
		return;
	}
}

void KGCItemManager::MakeInventoryItemList(USER_EQUIP_CATEGORY eCategory, int iGroupType, bool bEtcItem, std::vector<GCITEMUID>* pvecInventory, bool bSortByLevel, bool bEquipAllChar)
{
	SCharInfo* pCharInfo = NULL;
	if (bEtcItem == false)
		pCharInfo = &g_MyD3D->m_TempPlayer.GetCurrentChar();

	if (pvecInventory == NULL) {
		pvecInventory = &m_vecInventory;
	}

	if (iGroupType == 0)
	{
		_MakeInventoryItemList((*pvecInventory), pCharInfo, 0, 0, true, iGroupType, eCategory, bSortByLevel, false, bEquipAllChar);
	}
	else
	{
		if (eCategory < UEC_PET)
		{
			_MakeInventoryItemList((*pvecInventory), pCharInfo, m_dwItemCategoryFlag[eCategory], 0, false, iGroupType, eCategory, bSortByLevel);
		}
		else if (eCategory == UEC_PET)
		{
			_MakeInventoryItemList((*pvecInventory), pCharInfo, 0, 0, false, iGroupType, eCategory, bSortByLevel);
		}
		else
		{
			_MakeInventoryItemList((*pvecInventory), pCharInfo, 0, m_dwItemCategoryFlag[eCategory], false, iGroupType, eCategory, bSortByLevel);
		}
	}
}

bool KGCItemManager::IsCustomizationItem(GCITEMID iItem)
{
	for (auto FontSkin : g_pFontSkinMgr->GetFontSkinList())
		if (FontSkin.m_iItemID == iItem)
			return true;

	for (auto CutIn : g_pCutInSkinMgr->GetCutInSkinList())
		if (CutIn.m_iItemID == iItem)
			return true;

	for (auto RoomAnimation : g_pRoomAnimationMgr->GetRoomAnimationList())
		if (RoomAnimation.m_iItemID == iItem)
			return true;

	return false;
}

void KGCItemManager::_MakeInventoryItemList(std::vector<GCITEMUID>& vecInventory_, SCharInfo* pCharInfo, DWORD ItemCategory, DWORD SkillCategory, bool bEtc, int iGroupType, USER_EQUIP_CATEGORY invencategory, bool bSortByLevel, bool bNoClear, bool bEquipAllChar)
{
	if (!bNoClear)
		vecInventory_.clear();

	DWORD dFlag = pCharInfo ? GetCharFlag(pCharInfo->iCharType) : 0;

	if (!pCharInfo)
	{
		for (int i = 0; i < GC_CHAR_NUM; ++i)
			dFlag |= GetCharFlag(i);

		dFlag &= ~GetCharFlag(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType);
	}

	std::map<int, std::vector<GCITEMUID>> mapDisplayList;

	const KItemContainer::ItemList& vecInventory = m_kInventory.GetFullItemList();

	for (const auto& item : vecInventory)
	{
		const KItem& kItem = item.second;
		const auto mit = m_mapDB.find(kItem.m_ItemID);
		if (mit == m_mapDB.end())
			continue;

		const GCItem* ItemInfo = mit->second;
		if (pCharInfo)
		{
			if ((ItemInfo->dwCharType & dFlag) == 0)
				continue;
		}
		else
		{
			if ((ItemInfo->dwCharType & dFlag) == 0 || ItemInfo->dwCharType == ((1 << GC_CHAR_NUM) - 1))
				continue;
		}

		if (IsCustomizationItem(ItemInfo->dwGoodsID * 10))
			continue;

		if (ItemInfo->ePetItemKind == EPET_KIND_PET &&
			g_MyD3D->IsExistPetInfo(kItem.m_ItemUID) &&
			!IsSelectCharacterPet(static_cast<char>(pCharInfo->iCharType), kItem.m_ItemUID))
		{
			continue;
		}

		if (invencategory == UEC_PET)
		{
			if (!IsPetTabItem(ItemInfo->dwGoodsID))
			{
				if (ItemInfo->eItemKind != GIK_PET && ItemInfo->ePetItemKind == EPET_KIND_INVALID && !SiGCPetMagicStone()->IsPetGlyph(ItemInfo->dwGoodsID))
					continue;
			}
		}
		else if (invencategory == UEC_SKILL || invencategory == UEC_SKILL_WHITE || invencategory == UEC_SKILL_BLACK)
		{
			if (bEtc && (ItemInfo->dwSkillPosition & SkillCategory))
				continue;
			if (!bEtc && !(ItemInfo->dwSkillPosition & SkillCategory))
				continue;
		}
		else
		{
			if (bEtc && (ItemInfo->dwSlotPosition & ItemCategory))
				continue;
			if (!bEtc && !(ItemInfo->dwSlotPosition & ItemCategory))
				continue;
		}

		if (bEquipAllChar)
		{
			bool bEquip = false;
			for (auto& vit : g_kGlobalValue.m_kUserInfo.vecCharInfo)
			{
				bEquip = IsEquipedItem(&vit, kItem.m_ItemUID);
				if (bEquip)
					break;
			}
			if (bEquip)
				continue;
		}
		else
		{
			if (IsEquipedItem(pCharInfo, kItem.m_ItemUID))
				continue;
		}

		int iNowCategory = UEC_NUM + 1;

		for (int p = 0; p < UEC_NUM; ++p)
		{
			if (p < UEC_PET)
			{
				if (m_dwItemCategoryFlag[p] & ItemInfo->dwSlotPosition)
					iNowCategory = p + 1;
			}
			else if (p == UEC_PET)
			{
				if ((m_dwItemCategoryFlag[p] & ItemInfo->dwPetEquipItemSlot) ||
					IsPetTabItem(ItemInfo->dwGoodsID) || ItemInfo->eItemKind == GIK_PET || ItemInfo->ePetItemKind != EPET_KIND_INVALID)
				{
					iNowCategory = p + 1;
				}
			}
			else
			{
				if (m_dwItemCategoryFlag[p] & ItemInfo->dwSkillPosition)
					iNowCategory = p + 1;
			}
		}

		if (ItemInfo->eItemKind == GIK_TREASUREBOX_ITEM)
			iNowCategory = -1;

		if (iNowCategory - 1 == invencategory)
			iNowCategory = 0;

		mapDisplayList[iNowCategory].push_back(kItem.m_ItemUID);
	}

	std::wstring filterString_ = g_pkUIScene->m_pkMyInfoScene->m_strFilter;

	for (const auto& mapitor : mapDisplayList)
	{
		for (const auto& vecitor : mapitor.second)
		{
			KItem* pkItem = m_kInventory.FindItemByItemUID(vecitor);
			if (pkItem != nullptr)
			{
				GCItem* pFindItemData = this->GetItemData(pkItem->m_ItemID);
				if (pFindItemData != nullptr)
				{
					std::wstring strLowerItemName = pFindItemData->strItemName;
					GCUTIL_STR::ToLower(filterString_);
					GCUTIL_STR::ToLower(strLowerItemName);

					if (strLowerItemName.find(filterString_) != std::wstring::npos)
						vecInventory_.push_back(pkItem->m_ItemUID);
				}
			}
		}
	}

	_GroupingInventory(invencategory, iGroupType, vecInventory_);
}

std::vector<GCItem*> KGCItemManager::GetItemIDFromName(const WCHAR* strName_, int chartype)
{
	std::vector<GCItem*> vecItemNameList;
	stdext::hash_map< GCITEMID, GCItem* >::iterator mit;
	for (unsigned int i = 0; i < m_dwNumItem; ++i)
	{
		DWORD ItemKey_ = i;
		mit = m_mapDB.find(ItemKey_);

		if (mit == m_mapDB.end())
			continue;

		GCItem* kItem = mit->second;
		CONTINUE_NIL(kItem);

		wstring strLowerName = strName_;
		wstring strLowerItemName = kItem->strItemName;

		GCUTIL_STR::ToLower(strLowerName);
		GCUTIL_STR::ToLower(strLowerItemName);

		if (strLowerItemName.find(strLowerName) != std::wstring::npos)
		{
			if (chartype <= 0)
				vecItemNameList.push_back(kItem);

			else if (chartype > 0)
			{
				switch (kItem->dwCharType)
				{
				case ECT_ELESIS:if (chartype == 1)vecItemNameList.push_back(kItem); break;
				case ECT_LIRE:if (chartype == 2)vecItemNameList.push_back(kItem); break;
				case ECT_ARME:if (chartype == 3)vecItemNameList.push_back(kItem); break;
				case ECT_LAS:if (chartype == 4)vecItemNameList.push_back(kItem); break;
				case ECT_RYAN:if (chartype == 5)vecItemNameList.push_back(kItem); break;
				case ECT_RONAN:if (chartype == 6)vecItemNameList.push_back(kItem); break;
				case ECT_AMY:if (chartype == 7)vecItemNameList.push_back(kItem); break;
				case ECT_JIN:if (chartype == 8)vecItemNameList.push_back(kItem); break;
				case ECT_SIEG:if (chartype == 9)vecItemNameList.push_back(kItem); break;
				case ECT_MARI:if (chartype == 10)vecItemNameList.push_back(kItem); break;
				case ECT_DIO:if (chartype == 11)vecItemNameList.push_back(kItem); break;
				case ECT_ZERO:if (chartype == 12)vecItemNameList.push_back(kItem); break;
				case ECT_LEY:if (chartype == 13)vecItemNameList.push_back(kItem); break;
				case ECT_LUPUS:if (chartype == 14)vecItemNameList.push_back(kItem); break;
				case ECT_RIN:if (chartype == 15)vecItemNameList.push_back(kItem); break;
				case ECT_ASIN:if (chartype == 16)vecItemNameList.push_back(kItem); break;
				case ECT_LIME:if (chartype == 17)vecItemNameList.push_back(kItem); break;
#ifdef FEATURE_2021
				case ECT_EDEL:if (chartype == 18)vecItemNameList.push_back(kItem); break;
				case ECT_BEIGAS:if (chartype == 19)vecItemNameList.push_back(kItem); break;
				case ECT_UNO:if (chartype == 20)vecItemNameList.push_back(kItem); break;
#ifdef PERYTON
				case ECT_PERYTON:if (chartype == 21)vecItemNameList.push_back(kItem); break;
#endif
#ifdef ALICE
				case ECT_ALICE:if (chartype == 22)vecItemNameList.push_back(kItem); break;
#endif
#ifdef DECANEE
				case ECT_DECANEE:if (chartype == 23)vecItemNameList.push_back(kItem); break;
#endif
#endif
				case ECT_ALL:vecItemNameList.push_back(kItem); break;
				}
			}

		}
	}
	return vecItemNameList;
}
void KGCItemManager::AddItemInInventory(std::vector<GCItem*> vectoritemlist, std::vector<const WCHAR*> vecitemdata)
{
	std::vector<const WCHAR*> itemdata;//[4] = {0,};
	int j = 0;
	if (!vectoritemlist.empty())
	{
		unsigned int iLoop = 0;
		for (; iLoop < vectoritemlist.size(); ++iLoop)
		{
			itemdata.clear();
			for (int i = 0; i < 4; i++, j++)
				itemdata.push_back(vecitemdata[j]);


			KP2P::GetInstance()->Send_AddItem(vectoritemlist.at(iLoop)->dwGoodsID * 10,
				_wtoi(itemdata.at(0)), _wtoi(itemdata.at(1)), _wtoi(itemdata.at(2)), _wtoi(itemdata.at(3)));
		}
	}


}
void KGCItemManager::_GroupingInventory(USER_EQUIP_CATEGORY invencategory, int iGroupType, std::vector<GCITEMUID>& vecItemList)
{
	if ((iGroupType == -1) || (iGroupType == 1 && invencategory != UEC_PET) || (iGroupType == 0))
		return;

	DWORD dwCategory = 0;

	if (invencategory == UEC_EQUIP1)
	{
		switch (iGroupType)
		{
		case 2:
			dwCategory = ESP_HELMET;
			break;
		case 3:
			dwCategory = ESP_JACKET;
			break;
		case 4:
			dwCategory = ESP_PANTS;
			break;
		case 5:
			dwCategory = ESP_GLOVES;
			break;
		case 6:
			dwCategory = ESP_SHOES;
			break;
		case 7:
			dwCategory = ESP_MANTLE;
			break;
		case 8:
			dwCategory = ESP_WEAPON;
			break;
		case 9:
			dwCategory = ESP_PET;
			break;
		default:
			break;
		}
	}
	else if (invencategory == UEC_EQUIP2)
	{

		switch (iGroupType) {
		case 2:
			dwCategory = ESP_A_UPPER_HELMET;
			break;
		case 3:
			dwCategory = ESP_A_DOWN_HELMET;
			break;
		case 4:
			dwCategory = ESP_A_NECKLET;
			break;
		case 5:
			dwCategory = ESP_A_JACKET;
			break;
		case 6:
			dwCategory = ESP_A_PANTS;
			break;
		case 7:
			dwCategory = ESP_A_ARM;
			break;
		case 8:
			dwCategory = ESP_A_FINGER;
			break;
		case 9:
			dwCategory = ESP_A_SHOES;
			break;
		case 10:
			dwCategory = ESP_A_EARRING;
			break;
		default:
			break;
		}
	}
	else if (invencategory == UEC_SHORTCUT_SLOT)
	{

		switch (iGroupType) {
		case 2:
			dwCategory = ESP_SLOT_ITEM;
			break;
		case 3:
			dwCategory = ESP_SLOT_EMOT;
			break;
		default:
			break;
		}
	}
	else if (invencategory == UEC_PET)
	{
		switch (iGroupType)
		{
		case 1:
			dwCategory = EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM;
			break;
		case 2:
			dwCategory = 0;//EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM;
			return;
		case 3:
			dwCategory = EPET_EQUIP_ITEM_SLOT_SKILL;
			break;
		default:
			break;
		}
	}

	for (int i = (int)vecItemList.size() - 1; i >= 0; i--)
	{
		GCITEMUID itemKey = vecItemList[i];
		KItem* pkInvenItem = GetInventoryItem(itemKey);

		if (pkInvenItem == NULL) {
			ASSERT(L"Item is not exist in inventory!");
			continue;
		}

		GCItem* pkItem = GetItemData(pkInvenItem->m_ItemID);

		if (pkItem == NULL) {
			ASSERT(L"ItemKey is Invalid!!");
			continue;
		}

		if (invencategory == UEC_LOOK) {
			if (pkInvenItem->CheckItemType(KItem::TYPE_LOOK) == false) {
				vecItemList.erase(vecItemList.begin() + i);
			}
		}
		else if (invencategory < UEC_PET)
		{
			if (dwCategory != pkItem->dwSlotPosition)
			{
				vecItemList.erase(vecItemList.begin() + i);
			}
		}
		else if (invencategory == UEC_PET)
		{
			if (dwCategory != pkItem->dwPetEquipItemSlot &&
				ESP_PET != pkItem->dwSlotPosition)
			{
				vecItemList.erase(vecItemList.begin() + i);
			}
		}
		else
		{
			if (dwCategory != pkItem->dwSkillPosition)
			{
				vecItemList.erase(vecItemList.begin() + i);
			}
		}
	}
}

bool KGCItemManager::IsExistCrystalDiscountCard(void)
{
	return (GetInventoryItemDuration(ITEM_CRYSTAL_DISCOUNT_CARD) != 0);
}

int KGCItemManager::GetVipNum()
{
	return GetInventoryItemDuration(ITEM_NEW_PVP_COIN);
}

int KGCItemManager::GetGemNum()
{
	return GetInventoryItemDuration(GEM_ITEM_ID);
}

int KGCItemManager::GetCrystalNum()
{
	return GetInventoryItemDuration(CRYSTAL_ITEM_ID);
}

int KGCItemManager::GetHeroCoin()
{
	return GetInventoryItemDuration(ITEM_ID_HERO_COIN);
}


void KGCItemManager::InitNonCountItemSize()
{
	m_setNonCountItemList.clear();

#if !defined SERVER_ONLY_NON_COUNT_ITEM
	stdext::hash_map<DWORD, GCItem*>::iterator itor;
	for (itor = m_mapDB.begin(); itor != m_mapDB.end(); ++itor)
	{
		if (GIK_MONSTER_CARD == itor->second->eItemKind || itor->second->dwSlotPosition & ESP_SLOT_EMOT
			|| itor->second->dwSlotPosition & ESP_TITLE)
			m_setNonCountItemList.insert(itor->second->dwGoodsID);
	}

	for (std::map<DWORD, int>::iterator mIter = m_mapGachaList.begin(); mIter != m_mapGachaList.end(); mIter++)
	{
		m_setNonCountItemList.insert(mIter->first);
	}
#endif
}

std::vector<GCItem*> KGCItemManager::GetCashShopItem(DWORD dwGoodID_)
{
	if (m_mapCashShop.find(dwGoodID_) == m_mapCashShop.end())
	{
		std::vector<GCItem*> vecItem;
		return vecItem;
	}
	return m_mapCashShop[dwGoodID_];
}

GCItemAbility KGCItemManager::GetItemAbility(DWORD dwGoodsID_)
{
	GCItem* kItem = GetItemData(dwGoodsID_);
	return GetItemAbility(kItem);
}

GCItemAbility KGCItemManager::GetItemAbility(GCItem*& pItem_)
{
	GCItemAbility kItemAbility;

	EItemAbilityData itemAbility = GetItemAbilityData(pItem_, g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel);
	kItemAbility.iAtk = (int)itemAbility.fAtk;
	kItemAbility.iDef = (int)itemAbility.fDef;
	kItemAbility.iVit = (int)itemAbility.fHP;
	return kItemAbility;
}

GCItemAbility KGCItemManager::GetPetItemAbility(KPetInfo* pPetInfo_, int iCharacterLevel_)
{
	GCItemAbility kItemAbility;
	int iPetEvolutionLevel = pPetInfo_->m_cPromotion;
	// ÆêÀÏ °æ¿ì¿¡ Æê ÀÌ¸§ Ç¥½Ã & Æê ·¹º§ Ç¥½Ã
	int iLevel = GetPetLevel(pPetInfo_->m_dwEXP);

	if (iCharacterLevel_ < iLevel)
		iLevel = iCharacterLevel_;

	int iPetIndex = g_kGlobalValue.GetPetIDFromItemID(pPetInfo_->m_dwID);

	if (iPetIndex == -1)
		return kItemAbility;

	const SPetTemplate& Pet = g_kGlobalValue.GetPetTemplate(iPetIndex, pPetInfo_->m_cPromotion);

	kItemAbility.iAtk = (int)((Pet.fAtkDefault + iLevel * Pet.fAtkConstant * Pet.fEvolutionConst + 0.0005f) * 100);
	kItemAbility.iDef = (int)((Pet.fDefDefault + iLevel * Pet.fDefConstant * Pet.fEvolutionConst + 0.0005f) * 100);
	kItemAbility.iVit = (int)((Pet.fHPDefault + iLevel * Pet.fHPConstant * Pet.fEvolutionConst + 0.0005f) * 100);

	return kItemAbility;
}

std::vector<int> KGCItemManager::GetSetItemList(int SetID)
{
	std::map<int, std::vector<int>>::iterator itor;
	std::vector<int> vecEmpty;
	vecEmpty.clear();
	itor = m_mapSetItemList.find(SetID);
	if (itor == m_mapSetItemList.end())
		return vecEmpty;
	return itor->second;
}

//°ø¹æ»ý¸¸ ¾ò¾î¿Â´Ù.
EItemAbilityData KGCItemManager::GetItemAbilityData(GCItem* pItem_, int iCharacterLevel_, KItem* pkInvenItem_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, int iLevel_)
{
	EItemAbilityData stItemAbiliyData;
	GCItemAbility kItemAbility;

	// pItem_°¡ NULLÀÌ¸é ¹Ø¿¡¼­ Å©·¡½Ã³³´Ï´Ù.
	if (NULL == pItem_)
	{
		if (!(pkInvenItem_ && (pItem_ = GetItemData(pkInvenItem_->m_ItemID))))
			return stItemAbiliyData;
	}

	if (NULL == pPetInfo_ &&
		(pkInvenItem_ && GIK_PET == pItem_->eItemKind && pItem_->ePetItemKind == EPET_KIND_PET))
	{
		pPetInfo_ = SiKGCDepotManager()->GetPetInfo(pkInvenItem_);
	}

	int iItemSlotPosition;
	iItemSlotPosition = GetUniqueSlotPosition(pItem_->dwSlotPosition);
	//º¹Àå¼ÂÆ®´Â °è»ê ¾ÈÇÏ°í, °¢ ¾ÆÀÌÅÛ ´Ù ´õÇÑ´Ù.
	if (iItemSlotPosition == 0 && pItem_->iSetItemNumber != -1)
	{
		std::vector<int> vecSetItemList = GetSetItemList(pItem_->iSetItemNumber);

		if (vecSetItemList.empty() == false)
		{
			for (int i = 0; i < (int)vecSetItemList.size(); i++)
			{
				GCItem* pItemtemp = GetItemData(vecSetItemList[i]);
				//	/*EISP_HELMET      	        = 1
				//	EISP_JACKET                 = 2
				//	EISP_PANTS                  = 3
				//	EISP_GLOVES                 = 4
				//	EISP_SHOES	                = 5
				//	EISP_MANTLE                 = 6
				//	EISP_WEAPON	                = 7*/

				int iItemSlotPos = GetUniqueSlotPosition(pItemtemp->dwSlotPosition);

				// Åõ»óÇÏÀå½Å¸Á Æ÷ÇÔµÇµµ·Ï 
				if (iItemSlotPos < EISP_HELMET || iItemSlotPos > EISP_MANTLE)
					continue;

				stItemAbiliyData += GetItemAbilityData(pItemtemp, iCharacterLevel_, pkInvenItem_, pUserItem_, pPetInfo_);
			}
		}
		return stItemAbiliyData; //¼ÂÆ® ¾ÆÀÌÅÛÀº ´Ù ´õÇßÀ¸¸é ±×°É·Î ´É·ÂÄ¡ ¶¯.
	}


	if (GIK_ENCHANT_ENABLE_ITEM == pItem_->eItemKind)
	{
		char cEnchantLevel = 0;
		if (pkInvenItem_)
			cEnchantLevel = pkInvenItem_->m_cEnchantLevel;
		else if (pUserItem_)
			cEnchantLevel = pUserItem_->cEnchantLevel;

		kItemAbility = KSingleton<KEnchantStrong>::GetInstance()->GetAbility(pItem_, cEnchantLevel);
	}
	else if (pPetInfo_)
	{
		kItemAbility = GetPetItemAbility(pPetInfo_, iCharacterLevel_);
	}
	else
	{
		char cItemGrade = pItem_->cItemGrade;
		if (pkInvenItem_)
			cItemGrade = pkInvenItem_->m_cGradeID;
		else if (pUserItem_)
			cItemGrade = pUserItem_->cGradeID;

		GCItemAbility stItemCalcAbility = CalcItemAbility(pItem_, pkInvenItem_, cItemGrade, iLevel_, pUserItem_);
		kItemAbility.iAtk = stItemCalcAbility.iAtk;
		kItemAbility.iDef = stItemCalcAbility.iDef;
		kItemAbility.iVit = stItemCalcAbility.iVit;

		GCItemAbility kEnchantItemAbility;
		if (pkInvenItem_ && pkInvenItem_->m_EnchantEquipItemUID != 0) //½Å°­È­ ½ºÅÝ
		{
			kEnchantItemAbility = GetEnchantItemAbility(pItem_, pkInvenItem_);
		}
		else if (pUserItem_ && pUserItem_->cEnchantLevel != 0) //½Å°­È­ ½ºÅÝ
		{
			kEnchantItemAbility = GetEnchantItemAbility(pItem_, pUserItem_);
		}
		kItemAbility.iAtk += kEnchantItemAbility.iAtk;
		kItemAbility.iDef += kEnchantItemAbility.iDef;
		kItemAbility.iVit += kEnchantItemAbility.iVit;
	}

	int iLimitedLevelOfLevelRatioAbility;
	if (iLevel_ != -1)
		iLimitedLevelOfLevelRatioAbility = iLevel_;
	else
		iLimitedLevelOfLevelRatioAbility = GetlItemAbilityLimitLevel(pItem_, pkInvenItem_);

	if (iLimitedLevelOfLevelRatioAbility > 0)
	{
		int iLv = iCharacterLevel_;

		if (iLv > iLimitedLevelOfLevelRatioAbility)
			iLv = iLimitedLevelOfLevelRatioAbility;

		kItemAbility.iAtk += (int)round(iLv * pItem_->fLevelRatioAttackRate);
		kItemAbility.iDef += (int)round(iLv * pItem_->fLevelRatioDefenceRate);
		kItemAbility.iVit += (int)round(iLv * pItem_->fLevelRatioHealthRate);
	}

	stItemAbiliyData.fAtk = static_cast<float>(kItemAbility.iAtk);
	stItemAbiliyData.fDef = static_cast<float>(kItemAbility.iDef);
	stItemAbiliyData.fHP = static_cast<float>(kItemAbility.iVit);

	std::vector< KAttributeInfo > vecItemAttribute;
	std::vector< KSocketInfo > vecItemSocket;

	if (pkInvenItem_)
	{
		vecItemAttribute = pkInvenItem_->m_vecAttribute;
		if (pkInvenItem_->m_EnchantEquipItemUID != 0 && pItem_ && pItem_->eItemKind != GIK_ENCHANT_ENABLE_ITEM)
		{
			KItem* pkEnchantItem = g_pItemMgr->GetInventoryItem(pkInvenItem_->m_EnchantEquipItemUID);

			if (pkEnchantItem)
			{
				for (int i = 0; i < static_cast<int>(pkEnchantItem->m_vecAttribute.size()); i++)
					vecItemAttribute.push_back(pkEnchantItem->m_vecAttribute[i]);
			}
		}
		vecItemSocket = pkInvenItem_->m_vecSocket;
	}
	else if (pUserItem_)
	{
		vecItemAttribute = pUserItem_->vecItemAttribute;
		vecItemSocket = pUserItem_->vecItemSocket;
	}
	else
	{
		vecItemAttribute = pItem_->m_vecAttribute;
		vecItemSocket = pItem_->m_vecSocketInfo;
	}

	if ((pItem_->dwSlotPosition & ESP_TITLE) && vecItemAttribute.empty()) {
		KGCAttributeTable kItemAttributeTable;
		if (g_pItemMgr->GetAttributeTable(pItem_->iAttribute, pItem_->cItemGrade, kItemAttributeTable)) {
			const std::vector<KGCAttributeTable::SAttribute>& vecAttributelist = kItemAttributeTable.GetContainer();
			std::vector<KGCAttributeTable::SAttribute>::const_iterator vIter = vecAttributelist.begin();

			for (; vIter != vecAttributelist.end(); ++vIter) {
				if (vIter->m_iType == 0 || vIter->m_iType == 1 || vIter->m_iType == 2) {   // °ø¹æ»ý Á¦¿Ü 
					continue;
				}

				stItemAbiliyData.fItemAttribute[vIter->m_iType] += vIter->m_fValue;
			}
		}
	}

	std::vector< KAttributeInfo >::iterator vitAttr = vecItemAttribute.begin();
	for (; vitAttr != vecItemAttribute.end(); ++vitAttr)
	{
		if (KAttributeInfo::ATS_USING == (*vitAttr).m_cState)
		{
			stItemAbiliyData.fItemAttribute[(*vitAttr).m_cType] += (*vitAttr).m_fValue;
		}
	}
	std::vector< KSocketInfo >::iterator vitSocket = vecItemSocket.begin();
	for (; vitSocket != vecItemSocket.end(); ++vitSocket)
	{
		if (KSocketInfo::STS_USING == (*vitSocket).m_cState)
		{
			GCItem* pMonsterCardItem = GetItemData((*vitSocket).m_CardItemID / 10);

			if (NULL == pMonsterCardItem)
				continue;

			EItemAbilityData stMonsterCardAbilityData = GetItemAbilityData(pMonsterCardItem, iCharacterLevel_);
			stItemAbiliyData += stMonsterCardAbilityData;
		}
	}

	return stItemAbiliyData;
}

//°ø¹æ»ý¸¸ ¾ò¾î¿Â´Ù.
EItemAbilityData KGCItemManager::GetItemAttributeAbilityData(GCItem* pItem_, int iCharacterLevel_, KItem* pkInvenItem_, SUserItem* pUserItem_)
{
	EItemAbilityData stItemAbiliyData;
	GCItemAbility kItemAbility;

	// pItem_°¡ NULLÀÌ¸é ¹Ø¿¡¼­ Å©·¡½Ã³³´Ï´Ù.
	if (NULL == pItem_)
	{
		if (!(pkInvenItem_ && (pItem_ = GetItemData(pkInvenItem_->m_ItemID))))
			return stItemAbiliyData;
	}


	std::vector< KAttributeInfo > vecItemAttribute;

	if (pkInvenItem_)
	{
		vecItemAttribute = pkInvenItem_->m_vecAttribute;
	}
	else if (pUserItem_)
	{
		vecItemAttribute = pUserItem_->vecItemAttribute;
	}
	else
	{
		vecItemAttribute = pItem_->m_vecAttribute;
	}

	std::vector< KAttributeInfo >::iterator vitAttr = vecItemAttribute.begin();
	for (; vitAttr != vecItemAttribute.end(); ++vitAttr)
	{
		if (KAttributeInfo::ATS_USING == (*vitAttr).m_cState)
		{
			stItemAbiliyData.fItemAttribute[(*vitAttr).m_cType] += (*vitAttr).m_fValue;
		}
	}

	return stItemAbiliyData;
}

DWORD KGCItemManager::GetCheapItemPrice(DWORD dwGoodsID)
{
	return m_mapCheapItem[dwGoodsID];
}

bool KGCItemManager::IsPetTabItem(DWORD dwGoodsID)
{
	return dwGoodsID == KGCItemManager::ITEM_CHANGE_PET_NICKNAME_CARD ||
		dwGoodsID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH5000 ||
		dwGoodsID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH3000 ||
		dwGoodsID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_POINT ||
		dwGoodsID == KGCItemManager::ITEM_PET_EVOLUTION_ITEM_GP ||
		dwGoodsID == KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_POINT ||
		dwGoodsID == KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_GP ||
		dwGoodsID == KGCItemManager::ITEM_PET_HATCH_ITEM_GP ||
		dwGoodsID == KGCItemManager::ITEM_PETBOX ||
		dwGoodsID == KGCItemManager::ITEM_EVENT_PETBOX ||
		dwGoodsID == KGCItemManager::ITEM_PET_PACKEGE1 ||
		dwGoodsID == KGCItemManager::ITEM_PET_PACKEGE2 ||
		dwGoodsID == KGCItemManager::ITEM_PET_PACKEGE3;
}

#ifdef ENABLE_POINT_SYSTEM // Kawan>

void KGCItemManager::LoadPointItemList()
{
#if defined( ENABLE_POINT_SYSTEM )
	{
		PointItemList PointItem;
		KLuaManager luaMgr;

		if (GCFUNC::LoadLuaScript(luaMgr, "PointItem.stg") == false)
		{
			MessageBoxA(NULL, "PointItem Load Failed!", NULL, MB_OK);
			exit(0);
		}

		LUA_BEGIN_TABLE("POINT_ITEM", return);
		{
			for (int i = 1; ; ++i)
			{
				LUA_BEGIN_TABLE(i, break)
				{
					LUA_GET_VALUE("ITEM_GOODSID", PointItem.iGoodsID, return);
					LUA_GET_VALUE("POINT_PRICE", PointItem.iPointPrice, return);
					LUA_GET_VALUE("ITEM_TYPE_VALUE", PointItem.iItemTypeValue, return);
				}
				LUA_END_TABLE(return)

					m_vecPointItemList.push_back(PointItem);
			}
		}
		LUA_END_TABLE(return)
	}
#endif
}

void KGCItemManager::MakePointShopItemList()
{
	m_vecPointShop.clear();

	std::vector<SCharInfo> vecCharInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo;

	for (int i = 0; i < (int)m_vecPointItemList.size(); ++i)
	{
		// ¿ì¼± ÀüÃ¼ ¾ÆÀÌÅÛ ¸®½ºÆ®¿¡¼­ ¾ÆÀÌÅÛ Á¤º¸¸¦ ¾ò¾î¿Â´Ù
		GCItem* kItem = GetItemData(m_vecPointItemList[i].iGoodsID);
		CONTINUE_NIL(kItem);
		bool bValidItem = false;

		// ³»°¡ ÇöÀç °¡Áø Ä³¸¯ÅÍµéÀÇ ¾ÆÀÌÅÛ¸¸ °É·¯ÁØ´Ù
		for (int j = 0; j < (int)vecCharInfo.size(); ++j)
		{
			DWORD dwFlag = GetCharFlag(vecCharInfo[j].iCharType);

			if (kItem->dwCharType & dwFlag)
			{
				m_vecPointShop.push_back(m_vecPointItemList[i]);
				break;
			}
		}
	}
}
#endif

void KGCItemManager::MakePerfectMonthItemList()
{
	m_vecPerfectMonthBoxItem.clear();

	std::vector<SCharInfo> vecCharInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo;

	for (int i = 0; i < (int)m_vecPerfectMonthItemList.size(); ++i)
	{
		// ¿ì¼± ÀüÃ¼ ¾ÆÀÌÅÛ ¸®½ºÆ®¿¡¼­ ¾ÆÀÌÅÛ Á¤º¸¸¦ ¾ò¾î¿Â´Ù
		GCItem* kItem = GetItemData(m_vecPerfectMonthItemList[i].m_ItemID);
		CONTINUE_NIL(kItem);

		// ³»°¡ ÇöÀç °¡Áø Ä³¸¯ÅÍµéÀÇ ¾ÆÀÌÅÛ¸¸ °É·¯ÁØ´Ù
		for (int j = 0; j < (int)vecCharInfo.size(); ++j)
		{
			DWORD dwFlag = GetCharFlag(vecCharInfo[j].iCharType);

			if (kItem->dwCharType & dwFlag)
			{
				m_vecPerfectMonthBoxItem.push_back(m_vecPerfectMonthItemList[i]);
				break;
			}
		}
	}
}

#ifdef ENABLE_POINT_SYSTEM // Kawan>
bool KGCItemManager::IsPointShopItem(DWORD dwGoodsID)
{
	for (int i = 0; i < (int)m_vecPointItemList.size(); ++i)
	{
		if (m_vecPointItemList[i].iGoodsID == (int)dwGoodsID)
		{
			return true;
		}
	}
	return false;
}

int KGCItemManager::GetPointItemPrice(DWORD dwGoodsID)
{
	for (int i = 0; i < (int)m_vecPointItemList.size(); ++i)
	{
		if (m_vecPointItemList[i].iGoodsID == (int)dwGoodsID)
		{
			return m_vecPointItemList[i].iPointPrice;
		}
	}
	return 0;
}

int KGCItemManager::GetPointItemTypeValue(DWORD dwGoodsID)
{
	for (int i = 0; i < (int)m_vecPointItemList.size(); ++i)
	{
		if (m_vecPointItemList[i].iGoodsID == (int)dwGoodsID)
		{
			return m_vecPointItemList[i].iItemTypeValue;
		}
	}

	return 0;
}

void KGCItemManager::BuyPointItem(DWORD dwGoodsID, IN int nCount /*= 1*/, IN int nMoneyType /*= KBuySellItemReq::EMT_GCPOINT */)
{
	Result_Buy = INT_MAX;

	KP2P::GetInstance()->Send_BuyForGCPoint(dwGoodsID, nCount, nMoneyType);

	g_MyD3D->WaitForServerAck(Result_Buy, INT_MAX, 5000, TIME_OUT_VALUE);

	GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData(dwGoodsID);
	RETURN_NIL(kItem);
	switch (Result_Buy)
	{
	case 0: // ¼º°ø
	{
		g_KDSound.Play("78");
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetReplacedString(STR_ID_BUY_POINT_ITEM_SUCCESS, "l", kItem->strItemName), L"");
		break;
	}
	case -6: // Æ÷ÀÎÆ® ºÎÁ·
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetReplacedString(STR_ID_NOT_ENOUGH_POINT, "l", kItem->strItemName), L"");
		break;
	}
	case -9:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_GCPOINT_ITEM_ACK_COUNT_ERROR));
		break;
	}
	case TIME_OUT_VALUE: // Å¸ÀÓ¾Æ¿ô
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2));
		break;
	}
	case -98: // ÀÛ¾÷ÁßÀÎ ÆÐÅ¶
	{
		break;
	}
	default:
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_GCPOINT_ITEM_ACK, Result_Buy);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_BUY_POINT_ITEM_FAIL), strTemp);
		break;
	}
	}
}
#endif

KItem KGCItemManager::GetPerfectMonthItem(IN const GCITEMID iItemID_)
{
	for (int i = 0; i < (int)m_vecPerfectMonthBoxItem.size(); ++i)
	{
		if (m_vecPerfectMonthBoxItem[i].m_ItemID == iItemID_)
			return m_vecPerfectMonthBoxItem[i];
	}

	KItem kTempItem;
	kTempItem.m_ItemID = 0;
	return kTempItem;
}

bool KGCItemManager::GetMyCharEquipByItemPosition(IN DWORD dwItemPosition, OUT GCItem** ppItem)
{
	SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();
	ASSERT(pCharInfo);
	if (pCharInfo == NULL)
		return false;

	KUserItemVector::iterator vit;
	for (vit = pCharInfo->vecItemInfo.begin(); vit != pCharInfo->vecItemInfo.end(); ++vit)
	{
		if (FindInventory(vit->dwItemUID, ppItem) == false)
			continue;

		if ((*ppItem)->dwSlotPosition & dwItemPosition)
			return true;

		*ppItem = NULL;
	}

	return false;
}

bool KGCItemManager::GetMyCharEquipByItemPosition(IN DWORD dwItemPosition, OUT KItem** ppInvenItem_)
{
	SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();

	KUserItemVector::iterator vit;

	for (vit = pCharInfo->vecItemInfo.begin(); vit != pCharInfo->vecItemInfo.end(); ++vit)
	{
		GCItem* pkGCItem = NULL;
		if (FindInventory(vit->dwItemUID, ppInvenItem_, &pkGCItem) == false)
			continue;

		if (pkGCItem->dwSlotPosition & dwItemPosition)
			return true;

		*ppInvenItem_ = NULL;
	}

	return false;
}

bool KGCItemManager::GetMyCharEquipByItemPosition(IN DWORD dwItemPosition, OUT KItem** ppInvenItem_, OUT GCItem** pItem_)
{
	SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();

	KUserItemVector::iterator vit;
	for (vit = pCharInfo->vecItemInfo.begin(); vit != pCharInfo->vecItemInfo.end(); ++vit)
	{
		if (FindInventory(vit->dwItemUID, ppInvenItem_, pItem_) == false)
			continue;

		if ((*pItem_)->dwSlotPosition & dwItemPosition)
			return true;

		*pItem_ = NULL;
		*ppInvenItem_ = NULL;
	}

	return false;
}

bool KGCItemManager::CheckShopItemImage(GCItem* pItem_)
{
	char strItemID[MAX_PATH];
	bool bResult = true;

	int iItemID = GetSameImageIndex(pItem_->dwGoodsID);

	if (iItemID < (DWORD)1000)
		sprintf(strItemID, "Sbta%03d.dds", (int)iItemID);
	else
		sprintf(strItemID, "Sbta%d.dds", (int)iItemID);

	if (!g_pGCDeviceManager->GetMassFileManager()->IsIn(strItemID))
	{
		m_vecStrNullItemImage.push_back(strItemID);
		bResult = false;
	}

	return bResult;
}

bool KGCItemManager::CheckItemImage(GCItem* pItem_)
{
	const DWORD dwNoMeshItem = ESP_SET_ITEM | ESP_A_NECKLET | ESP_A_FINGER | ESP_A_SHOES | ESP_PET;
	bool bResult = true;


	if (pItem_->dwSlotPosition & ESP_SET_ITEM)
	{
		if (!CheckSetItemImage(pItem_))
		{
			bResult = false;
		}
	}
	else if (!CheckShopItemImage(pItem_))
	{
		bResult = false;
	}


	if (pItem_->dwSlotPosition != NULL && !(pItem_->dwSlotPosition & dwNoMeshItem) && !g_pkPetAI->IsOldPet(pItem_->dwGoodsID))
	{
		if (!CheckEquipItemMeshTex(pItem_))
		{
			bResult = false;
		}
	}

	return bResult;
}

bool KGCItemManager::CheckSetItemImage(GCItem* pItem_)
{
	ASSERT(pItem_->iSetItemNumber != -1 && "¼¼Æ®¾ÆÀÌÅÛÀÌ¶ó ÇØ³õ°í!!!!!");

	char strFileName[MAX_PATH];
	GCItem* pSetItem = GetSameImageIndexItemInfo(pItem_->dwGoodsID);
	INT iSetItemNum = pItem_->iSetItemNumber;
	if (pSetItem != NULL)
		iSetItemNum = pSetItem->iSetItemNumber;

	sprintf(strFileName, "Set%d.dds", iSetItemNum / 2);

	if (!g_pGCDeviceManager->GetMassFileManager()->IsIn(strFileName))
	{
		m_vecStrNullItemImage.push_back(strFileName);
		return false;
	}
	return true;
}

bool KGCItemManager::CheckEquipItemMeshTex(GCItem* pItem_)
{
	char strMeshFileName[MAX_PATH];
	char strTexFileName[MAX_PATH];
	char strTexAbtaName[MAX_PATH];
	char strMeshAbtaName[MAX_PATH];
	static char* strExt[] = { "dds", "dd0", "dd1" };
	bool bResult = true;

	int iItemID = GetSameImageIndex(pItem_->dwGoodsID);
	int iTextureID = GetSameTextureIndex(pItem_->dwGoodsID);

	if (iTextureID == pItem_->dwGoodsID)
		iTextureID = iItemID;

	if (iItemID < (DWORD)1000)
	{
		sprintf(strTexAbtaName, "Abta%03d", (int)iTextureID);
		sprintf(strMeshAbtaName, "Abta%03d", (int)iItemID);
	}
	else
	{
		sprintf(strTexAbtaName, "Abta%d", (int)iTextureID);
		sprintf(strMeshAbtaName, "Abta%d", (int)iItemID);
	}
	sprintf(strMeshFileName, "%s.p3m", strMeshAbtaName);
	if (!g_pGCDeviceManager->GetMassFileManager()->IsIn(strMeshFileName))
	{
		m_vecStrNullItemImage.push_back(strMeshFileName);
		bResult = false;
	}
	// ¶ó½º ÀüÁ÷Àº ¹«±â ¸Å½¬°¡ 2°³ Ä®, Ä®Áý.
	// ·ÐÇÑ2Â÷ ¹«±â Ä®°ú ¹æÆÐ!
	// Áø2Â÷ ¹«±â´Â ºÀÀÎµ¥,, ¾ç¼Õ¿¡ ÇÏ³ª¾¿ µé°íÀÖ´Ù~ ¾Ë¾Æ¼­ ÇÏ³ª´Â ²ô°í ¤¾ 
	if ((pItem_->dwCharType == ECT_LAS && pItem_->iNeedChangeJobLevelForEquippingItem == 3)
		|| (pItem_->dwCharType == ECT_RONAN && pItem_->iNeedChangeJobLevelForEquippingItem == 2)
		|| (pItem_->dwCharType == ECT_JIN && pItem_->iNeedChangeJobLevelForEquippingItem == 2))
	{
		sprintf(strMeshFileName, "%s_m.p3m", strMeshAbtaName);
		if (!g_pGCDeviceManager->GetMassFileManager()->IsIn(strMeshFileName))
		{
			m_vecStrNullItemImage.push_back(strMeshFileName);
			bResult = false;
		}
	}

	sprintf(strTexFileName, "%s.%s", strTexAbtaName, strExt[0]);
	if (!g_pGCDeviceManager->GetMassFileManager()->IsIn(strTexFileName))
	{
		m_vecStrNullItemImage.push_back(strTexFileName);
		bResult = false;
	}

	return bResult;
}

bool KGCItemManager::CheckItemImageFull()
{
	// ¿¹ÀÇ»ó Å¬¸®¾î.
	m_vecStrNullItemImage.clear();

	stdext::hash_map< DWORD, GCItem* >::iterator mit;
	for (mit = m_mapDB.begin(); mit != m_mapDB.end(); ++mit)
	{
		//¼¥¿¡ ÀÕ´Â ¾ÆÀÌÅÛÀÌ¸é 
		if (!mit->second->bNotBuyItem)
		{
			CheckItemImage(mit->second);
		}
	}

	if (m_vecStrNullItemImage.size() == 0)
	{
		return true;
	}
	return false;
}

bool KGCItemManager::IsMetamorphosisItem(DWORD dwItemID)
{
	switch (dwItemID)
	{
	case 3225:
	case 3227:
	case 3253:
	case 3953:
	case 3954:
	case 13713:
	case 13714:
	case 13715:
		// For_Thanatos
		return true;
	}

	return false;
}

bool KGCItemManager::CheckRare(const DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		if (pItem->cItemGrade)
			return true;
	}

	return false;
}

bool KGCItemManager::CheckItemKind(const DWORD dwItemID, const GCITEM_KIND eItemKind)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		if (eItemKind == pItem->eItemKind)
			return true;
	}

	return false;
}

void KGCItemManager::RemoveInventoryItemList(GCITEMUID dwUID)
{
	std::vector< GCITEMUID >::iterator vit = std::find(m_vecInventory.begin(), m_vecInventory.end(), dwUID);

	if (m_vecInventory.end() != vit)
		m_vecInventory.erase(vit);
}

KItem* KGCItemManager::GetInventoryItemIndex(int iIndexList)
{
	return GetInventoryItem(m_vecInventory[iIndexList]);
}

KItem* KGCItemManager::GetInventoryItem(GCITEMUID dwUID)
{
	return m_kInventory.FindItemByItemUID(dwUID);
}

KItem* KGCItemManager::GetInventoryItemFromID(GCITEMID dwItemID)
{
	return m_kInventory.FindItemByItemID(dwItemID);
}


int KGCItemManager::GetVolumeInventoryItem(int iItemID)
{
	int volume = 0;
	KItem* pkInvenItem = m_kInventory.FindItemByItemID(iItemID);

	if (pkInvenItem != NULL)
		volume = pkInvenItem->m_nCount;

	return volume;
}

DWORD KGCItemManager::GetPremiumType(int iPlayerIndex)
{
	if (iPlayerIndex < 0 || iPlayerIndex >= MAX_PLAYER_NUM)
		return KPremiumInfo::TYPE_NORMAL;

	return g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.dwPremium;
}

void KGCItemManager::GetNewReceiveItemNum(const std::vector<KItem>& vecItemInfo, std::vector<KItem>& newVecItemInfo)
{
	if (newVecItemInfo.empty() == true)
		return;

	std::vector<KItem>::iterator vit = newVecItemInfo.begin();

	for (int i = 0; i < (int)vecItemInfo.size(); ++i, ++vit)
	{
		if (vit == newVecItemInfo.end())
			break;

		const KItem& _ItemInfo = vecItemInfo[i];

		GCITEMUID itemUID = _ItemInfo.m_ItemUID;
		KItem* pkItem = m_kInventory.FindItemByItemUID(itemUID);
		if (pkItem == NULL) // ¿ø·¡ ¾ø¾ú´Ù¸é ÇöÀç DB¿¡¼­ ¹ÞÀº ¼ö·®ÀÌ ¹Ù·Î »õ·Î ¹ÞÀº ¼ö·®ÀÌ´Ù!
			continue;

		vit->m_nPeriod = _ItemInfo.m_nPeriod - pkItem->m_nPeriod;
	}
}

bool KGCItemManager::IsRallyTitleItemListIndex(const int iIndex)
{
	if (iIndex < 0 || iIndex >= (int)m_vtRally_Collection_List.size())
		return false;

	return true;
}

std::wstring KGCItemManager::DumpInventory(GCITEM_KIND eKind /*= GIK_ITEM_KIND_NUM */)
{
	std::wstringstream stmRet;
	stmRet << "Dump Inventory : ";

	const KItemContainer::ItemList& mapInventory = m_kInventory.GetFullItemList();
	KItemContainer::ItemList::const_iterator mit;
	for (mit = mapInventory.begin(); mit != mapInventory.end(); ++mit) {
		GCItem* pItem = GetItemData(mit->second.m_ItemID);
		if (eKind == GIK_ITEM_KIND_NUM || pItem->eItemKind == eKind)
		{
			stmRet << mit->second.m_ItemUID << ", ";
		}
	}

	return stmRet.str();
}

void KGCItemManager::UnEquipTitleItem(SCharInfo* pCharInfo)
{
	if (!pCharInfo)
		return;

	std::vector<SUserItem>::iterator itITEM = pCharInfo->vecItemInfo.begin();
	for (; itITEM != pCharInfo->vecItemInfo.end(); )
	{
		GCItem* pItemID = GetItemData(itITEM->iItemID);
		if (pItemID)
		{
			if (pItemID->eItemKind == GIK_TITLE_ITEM)
			{
				itITEM = pCharInfo->vecItemInfo.erase(itITEM);
				continue;
			}
			else if (IsRallyTitleItemID(pItemID->dwGoodsID))
			{
				itITEM = pCharInfo->vecItemInfo.erase(itITEM);
				EquipBaseItem(pCharInfo);
				g_pkUIScene->m_pkMyInfoScene->UpdateGameData(false, g_pkUIScene->m_pkMyInfoScene->GetEquipCategory());
				continue;
			}
		}
		++itITEM;
	}
}

bool KGCItemManager::IsEquipTitleItem(SCharInfo* pCharInfo, int iItemID)
{
	if (!pCharInfo)
		return false;

	GCItem* pItemID = GetItemData(iItemID);
	if (!pItemID)
		return false;

	if (pItemID->eItemKind != GIK_TITLE_ITEM
		&& false == IsRallyTitleItemID(pItemID->dwGoodsID))
		return false;

	std::vector<SUserItem>::iterator itITEM = pCharInfo->vecItemInfo.begin();
	for (; itITEM != pCharInfo->vecItemInfo.end(); ++itITEM)
	{
		if (iItemID == itITEM->iItemID)
		{
			return true;
		}
	}

	return false;
}

int KGCItemManager::GetEquipTitleItem(SCharInfo* pCharInfo)
{
	if (!pCharInfo)
		return false;

	std::vector<SUserItem>::iterator itITEM = pCharInfo->vecItemInfo.begin();
	for (; itITEM != pCharInfo->vecItemInfo.end(); ++itITEM)
	{
		GCItem* pItem = GetItemData(itITEM->iItemID);
		if (!pItem)
			continue;

		if (pItem->eItemKind == GIK_TITLE_ITEM
			|| IsRallyTitleItemID((int)pItem->dwGoodsID))
			return itITEM->iItemID;
	}

	return -1;
}

bool KGCItemManager::EquipTitleItem(SCharInfo* pCharInfo, int iItemID)
{
	KItem* pInventoryItem = NULL;
	if (FindInventoryForItemID(iItemID, &pInventoryItem))
	{
		// ¾ÆÀÌÅÛÀ» Ãß°¡·Î ´õ ÀåÂøÇÒ ¼ö ÀÖ´ÂÁö È®ÀÎ
		if (pCharInfo->vecItemInfo.size() < MAX_EQUIP_NUM)
		{
			UnEquipTitleItem(pCharInfo);
			SUserItem _Item;
			_Item.dwItemUID = pInventoryItem->m_ItemUID;
			_Item.iItemID = pInventoryItem->m_ItemID;
			pCharInfo->vecItemInfo.push_back(_Item);

			g_pkUIScene->m_pkMyInfoScene->SetTitleItemToCharWnd(pInventoryItem->m_ItemID);

			return true;
		}
		else
		{
			// ´õ ÀÌ»ó ÀåÂøÇÒ ¼ö ¾øÀ½.
			if (g_pkUIScene != NULL)
			{
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
					g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP1),
					g_pkStrLoader->GetString(STR_ID_NO_MORE_EQUIP2), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
			}
		}
	}

	return false;
}


float KGCItemManager::GetTitleDecreasePercentByItemID(const int iTitleID)
{
	return g_pkUIScene->m_pkTitleCollection->GetTitleDecreasePercentByItemID(iTitleID);
}

KItem* KGCItemManager::FindEquipedSkill(SCharInfo* pCharInfo, ESKILLPOSITION eSkillSlot)
{
	if (pCharInfo == NULL)
		return NULL;

	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); ++i)
	{
		if (IsBaseItem(pCharInfo->iCharType, pCharInfo->vecItemInfo[i].iItemID))
			continue;

		KItem* pInvenItem = GetInventoryItem(pCharInfo->vecItemInfo[i].dwItemUID);
		if (pInvenItem == NULL) continue;
		GCItem* pItem = GetItemData(pInvenItem->m_ItemID);
		if (pItem == NULL) continue;

		if (pItem->dwSkillPosition & eSkillSlot)
			return pInvenItem;
	}

	return NULL;
}

bool KGCItemManager::IsRallyTitleItemID(const int iItemID)
{
	for (int i = 0; i < (int)m_vtRally_Collection_List.size(); ++i)
	{
		if (iItemID == m_vtRally_Collection_List[i])
		{
			return true;
		}
	}

	return false;
}

void KGCItemManager::LoadNormalTitleItem(void)
{
	KLuaManager luaMgr;
	if (GCFUNC::LoadLuaScript(luaMgr, "collection_missionlist.lua") == false)
		return;

	m_vtRally_Collection_List.clear();
	m_vtRally_Collection_Hp.clear();
	m_vtRally_Collection_State.clear();
	m_vtRally_Collection_Arena.clear();

	LUA_BEGIN_TABLE("COLLECTION_MISSIONLIST", return)
	{
		char strTable[MAX_PATH] = { 0, };
		strcpy(strTable, "NORMAL_TITLE");
		LUA_BEGIN_TABLE(strTable, return)
		{
			for (int j = 1; ; ++j)
			{
				LUA_BEGIN_TABLE(j, break)
				{
					int iTitleItemID = -1;
					float fHpRecoveryIncrease = 0.0f;
					float fStateConfuseDecrease = 0.0f;
					float fArenaTimeIncrease = 0.0f;
					LUA_GET_VALUE_DEF("TitleID", iTitleItemID, -1);
					LUA_GET_VALUE_DEF("HpRecoveryIncrease", fHpRecoveryIncrease, 0.0f);
					LUA_GET_VALUE_DEF("StateConfuseDecrease", fStateConfuseDecrease, 0.0f);
					LUA_GET_VALUE_DEF("ArenaTimeIncrease", fArenaTimeIncrease, 0.0f);

					if (-1 != iTitleItemID)
					{
						m_vtRally_Collection_List.push_back((iTitleItemID / 10));
						m_vtRally_Collection_Hp.push_back(fHpRecoveryIncrease);
						m_vtRally_Collection_State.push_back(fStateConfuseDecrease);
						m_vtRally_Collection_Arena.push_back(fArenaTimeIncrease);
					}
				}
				LUA_END_TABLE(break)
			}
		}
		LUA_END_TABLE(return)
	}
	LUA_END_TABLE(return);
}

bool KGCItemManager::IsBetterItemAdd(GCItem*& pItem)
{
	return false;

	if (g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.empty())
		return false;

	SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();

	KUserItemVector::iterator vit;

	for (vit = pCharInfo->vecItemInfo.begin(); vit != pCharInfo->vecItemInfo.end(); ++vit)
	{
		GCItem* pEquipItem = NULL;
		pEquipItem = GetItemData(vit->iItemID);

		if (pItem->iNeedLevelForEquippingItem <= pCharInfo->iLevel &&
			(pItem->dwSlotPosition == pEquipItem->dwSlotPosition) &&
			(((pItem->iAttack * 2) + pItem->iDefence + pItem->iHealth) >
				((pEquipItem->iAttack * 2) + pEquipItem->iDefence + pEquipItem->iHealth)))
		{
			return true;
		}
	}
	return false;
}

std::vector<KItem> KGCItemManager::GetInventoryItemList(GCITEM_KIND eItemKind, int eMoneyType, DWORD eItemPosition, DWORD eCharType)
{
	std::vector<KItem> vecItemList;
	const auto& mapInventory = m_kInventory.GetFullItemList();
	vecItemList.reserve(m_kInventory.GetSize());

	for (const auto& itemPair : mapInventory) {
		const GCItem* pItem = GetItemData(itemPair.second.m_ItemID);

		if (pItem == nullptr)
			continue;

		if (pItem->eItemKind != eItemKind)
			continue;

		if (pItem->eMoneyType != eMoneyType && eMoneyType != -1)
			continue;

		if (!(pItem->dwSlotPosition & eItemPosition))
			continue;

		if (!(pItem->dwCharType & eCharType))
			continue;

		vecItemList.emplace_back(itemPair.second);
	}

	return vecItemList;
}

//¾ÆÀÌÅÛ °¹¼ö°¡ ¹ÌÄ£µíÀÌ ¸¹À»¶§
bool KGCItemManager::CheckItemCountLimit()
{
	return g_kGlobalValue.m_iInvenCapacity >= LIMIT_INVENTORY_SIZE;
}

bool KGCItemManager::CheckExtraInventoryExist()
{
	return GetTotalInventorySize() < g_kGlobalValue.m_iInvenCapacity;
}
bool KGCItemManager::CheckExtraCoordiExist()
{
	return GetCoordiInventorySize() < g_kGlobalValue.m_iCoordiCapacity;
}
bool KGCItemManager::CheckInventorySetItem(int iSetID)
{
	// ÀÎº¥Åä¸®¿¡ ÀÌ ¼¼Æ® ¾ÆÀÌÅÛÀ» °¡Áö°í ÀÖ´ÂÁö ¾Ë°í ½Í´Ù..
	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		if (m_SetList[i].iSetNum == iSetID)
		{
			int match = 0;

			for (int k = 0; k < (int)m_SetList[i].vecParts.size(); k++)
			{
				if (FindInventoryForItemID(m_SetList[i].vecParts[k]))
				{
					match++;
				}
			}

			if (match == m_SetList[i].vecParts.size())
			{
				return true;
			}
		}
	}

	return false;
}

void KGCItemManager::SendBuyForCashItem(GCItem* kItem, DWORD dwUseVirtualCash, bool bPresent, std::wstring strOwnName, std::wstring strComment)
{
	int iPrice = kItem->dwPrice;

	if (g_kGlobalValue.m_dwCash + dwUseVirtualCash < (unsigned long)iPrice)
	{
		g_pkUIScene->DestroyMsgBox();
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		return;
	}

	Result_CanBuyCashItem = INT_MAX;
	KP2P::GetInstance()->Send_CanBuyCashItem(kItem->dwGoodsID * 10);
	g_MyD3D->WaitForServerAck(Result_CanBuyCashItem, INT_MAX, 5000, TIME_OUT_VALUE);

	switch (Result_CanBuyCashItem)
	{
	case 0:
	case 1:
		g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_BUY_CASH_ITEM));
		break;
	case 2:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_LIMITED_BUY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;;
	case 3:
	case 4:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	case -98:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_BINGO_ERROR7), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	if (GCIT_PERIOD == kItem->eItemType)
		KP2P::GetInstance()->Send_BuyForCashItem(kItem->dwGoodsID * 10, KItem::UNLIMITED_ITEM, kItem->iItemTypeValue, dwUseVirtualCash, bPresent, strOwnName, strComment);
	else
		KP2P::GetInstance()->Send_BuyForCashItem(kItem->dwGoodsID * 10, kItem->iItemTypeValue, KItem::UNLIMITED_ITEM, dwUseVirtualCash, bPresent, strOwnName, strComment);

	g_KDSound.Play("30");
	Result_Buy_CashItem = INT_MAX;
	g_MyD3D->WaitForServerAck(Result_Buy_CashItem, INT_MAX, 10000, 4);
	g_KDSound.Play("78");

	switch (Result_Buy_CashItem)
	{
	case -6:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case -3:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_CASH_ITEM), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case -1:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_EXIST_NICK), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 0:
	{
		if (g_pkUIScene->m_pkCustomizationDlg->IsWndState(KD3DWnd::ED3DWndState::D3DWS_ENABLED))
			g_pkUIScene->m_pkCustomizationDlg->UpdateAllSubWindows();
		break;
	}
	case 1:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_NOT_EXIST_PRICE_INFO), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 3:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_LIMITED_BUY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 4:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_BUY_SUCCESS_BUT_INVENTORY_REFRESH_FAIL), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case -7:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUALCASH_MSG)
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 101:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_SYSTEM_ERROR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 103:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 110:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_REDUCTION_CASH), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	case 111:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PRICE_DIFFERENT_DB), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	default:
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_FOR_CASH_ACK, Result_Buy_CashItem);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ERROR_BUY_CASH_ITEM), strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
		break;
	}
	}
}

void KGCItemManager::SendBuyForVirtualCashItem(GCItem* kItem, int iAmmount)
{
	KItem* pkItem = m_kInventory.FindItemByItemID(kItem->dwGoodsID);

	Result_CanBuyCashItem = INT_MAX;
	KP2P::GetInstance()->Send_CanBuyCashItem(kItem->dwGoodsID * 10);
	g_MyD3D->WaitForServerAck(Result_CanBuyCashItem, INT_MAX, 5000, TIME_OUT_VALUE);

	switch (Result_CanBuyCashItem)
	{
	case 0:
	case 1:
		g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_BUY_CASH_ITEM));
		break;
	case 2:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_LIMITED_BUY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	case 3:
	case 4:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	case  5:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_AGIT_INVEN_FULL), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	case 6:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_BUY_AGIT_ITEM_FAIL8), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;

	case -98:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_BINGO_ERROR7), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		return;
	}

	if (GCIT_PERIOD == kItem->eItemType)
		KP2P::GetInstance()->Send_BuyItemForVirtualCashReq(kItem->dwGoodsID, 0, KItem::UNLIMITED_ITEM, kItem->iItemTypeValue * iAmmount);
	else
		KP2P::GetInstance()->Send_BuyItemForVirtualCashReq(kItem->dwGoodsID, 0, kItem->iItemTypeValue * iAmmount, KItem::UNLIMITED_ITEM);

	g_KDSound.Play("30");
	Result_Buy_VirtualCashItem = -1000;
	g_MyD3D->WaitForServerAck(Result_Buy_VirtualCashItem, -1000, 10000, 4);
	g_KDSound.Play("78");

	switch (Result_Buy_VirtualCashItem)
	{
	case 0:
	{
		if (g_pkUIScene->m_pkCustomizationDlg->IsWndState(KD3DWnd::ED3DWndState::D3DWS_ENABLED))
			g_pkUIScene->m_pkCustomizationDlg->UpdateAllSubWindows();
		break;
	}
	case 1:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_EXIST_PRICE_INFO), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	case 2:
	case 6:
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_VIRTUAL_CASH_ACK, Result_Buy_VirtualCashItem);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_CASH_POINT), strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;

	}
	case 3:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_EXIST_USER_INFO), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	case 4:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_GACHA_ERROR8), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	case 5:
	case 8:
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_VIRTUAL_CASH_ACK, Result_Buy_VirtualCashItem);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_VIRTUALCASH_MSG), strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true);
		break;
	}
	case 7:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_LIMITED_BUY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;

	}
	case -98:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PACKET_DELAY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;

	}
	default:
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_BUY_VIRTUAL_CASH_ACK, Result_Buy_VirtualCashItem);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_ERROR_BUY_CASH_ITEM), strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	}
}

void KGCItemManager::SetNotbuyInfoToAllItem(bool bNotbuy)
{
	stdext::hash_map< DWORD, GCItem* >::iterator mit;
	for (mit = m_mapDB.begin(); mit != m_mapDB.end(); ++mit)
	{
		if (mit->second->dwGoodsID != 0)
			mit->second->bNotBuyItem = bNotbuy;
	}
}

bool KGCItemManager::IsEquipmentItem(DWORD dwItemID_)
{
	GCItem* pItem = GetItemData(dwItemID_);

	if (NULL == pItem)
		return false;

	if (pItem->dwSlotPosition & ESP_HELMET ||
		pItem->dwSlotPosition & ESP_JACKET ||
		pItem->dwSlotPosition & ESP_PANTS ||
		pItem->dwSlotPosition & ESP_GLOVES ||
		pItem->dwSlotPosition & ESP_SHOES ||
		pItem->dwSlotPosition & ESP_MANTLE ||
		pItem->dwSlotPosition & ESP_WEAPON ||
		pItem->dwSlotPosition & ESP_A_UPPER_HELMET ||
		pItem->dwSlotPosition & ESP_A_DOWN_HELMET ||
		pItem->dwSlotPosition & ESP_A_NECKLET ||
		pItem->dwSlotPosition & ESP_A_JACKET ||
		pItem->dwSlotPosition & ESP_A_PANTS ||
		pItem->dwSlotPosition & ESP_A_ARM ||
		pItem->dwSlotPosition & ESP_A_FINGER ||
		pItem->dwSlotPosition & ESP_A_SHOES ||
		pItem->dwSlotPosition & ESP_HELMET_SOCKET ||
		pItem->dwSlotPosition & ESP_WEAPON_SOCKET ||
		pItem->dwSlotPosition & ESP_JACKET_SOCKET ||
		pItem->dwSlotPosition & ESP_A_HELMET_SOCKET ||
		pItem->dwSlotPosition & ESP_A_EARRING ||
		pItem->dwSlotPosition & ESP_PET ||
		pItem->dwSlotPosition & ESP_TITLE)
	{
		return true;
	}

	return false;
}

bool KGCItemManager::IsArmorEquipItem(DWORD dwItemID_)
{
	GCItem* pItem = GetItemData(dwItemID_);

	if (NULL == pItem)
		return false;

	if (pItem->eItemKind == GIK_MONSTER_CARD)
		return false;

	if (pItem->dwSlotPosition & ESP_HELMET ||
		pItem->dwSlotPosition & ESP_JACKET ||
		pItem->dwSlotPosition & ESP_PANTS ||
		pItem->dwSlotPosition & ESP_GLOVES ||
		pItem->dwSlotPosition & ESP_SHOES ||
		pItem->dwSlotPosition & ESP_MANTLE ||
		pItem->dwSlotPosition & ESP_A_UPPER_HELMET ||
		pItem->dwSlotPosition & ESP_A_DOWN_HELMET ||
		pItem->dwSlotPosition & ESP_A_NECKLET ||
		pItem->dwSlotPosition & ESP_A_JACKET ||
		pItem->dwSlotPosition & ESP_A_PANTS ||
		pItem->dwSlotPosition & ESP_A_ARM ||
		pItem->dwSlotPosition & ESP_A_FINGER ||
		pItem->dwSlotPosition & ESP_A_SHOES ||
		pItem->dwSlotPosition & ESP_A_EARRING)
	{
		return true;
	}

	return false;

}

bool KGCItemManager::IsClosetItem(GCItem* pItem)
{
	if (NULL == pItem)
		return false;

	if (pItem->eItemKind == GIK_MONSTER_CARD)
		return false;

	if (pItem->dwSlotPosition & ESP_A_UPPER_HELMET || pItem->dwSlotPosition & ESP_A_PANTS)
	{
		std::stringstream sModelTempName;
		sModelTempName.str("");
		sModelTempName << g_pGCDeviceManager2->GetAbtaName((pItem->dwGoodsID), "abta") << ".p3m";
		return g_pGCDeviceManager2->CheckFileExist(sModelTempName.str().c_str());
	}

	if (pItem->dwSlotPosition & ESP_HELMET ||
		pItem->dwSlotPosition & ESP_JACKET ||
		pItem->dwSlotPosition & ESP_PANTS ||
		pItem->dwSlotPosition & ESP_GLOVES ||
		pItem->dwSlotPosition & ESP_SHOES ||
		pItem->dwSlotPosition & ESP_MANTLE ||
		pItem->dwSlotPosition & ESP_WEAPON ||
		pItem->dwSlotPosition & ESP_A_UPPER_HELMET ||
		pItem->dwSlotPosition & ESP_A_DOWN_HELMET ||
		pItem->dwSlotPosition & ESP_A_JACKET ||
		pItem->dwSlotPosition & ESP_A_PANTS ||
		pItem->dwSlotPosition & ESP_A_ARM)
	{
		return true;
	}

	return false;
}

bool KGCItemManager::IsExistTradeItem(DWORD dwItemID_)
{
	std::map< std::pair<DWORD, int>, KDropItemInfo >::iterator mit;

	for (mit = m_mapTradeItem.begin(); mit != m_mapTradeItem.end(); ++mit)
	{
		if (mit->first.first / 10 == dwItemID_)
		{
			if (g_MyD3D->m_kItemMgr.GetInventoryItemDuration(dwItemID_) >= mit->first.second)
				return true;
			else
				return false;
		}
	}
	return false;
}

DWORD KGCItemManager::GetTradeableItemID()
{
	std::map< std::pair<DWORD, int>, KDropItemInfo >::iterator mit;
	DWORD dwItemID = 0;

	for (mit = m_mapTradeItem.begin(); mit != m_mapTradeItem.end(); ++mit)
	{
		dwItemID = (*mit).first.first / 10;
		if (g_MyD3D->m_kItemMgr.GetInventoryItemDuration(dwItemID) >= (*mit).first.second)
			return dwItemID;
	}

	return 0;
}

std::pair<DWORD, int> KGCItemManager::GetItemTradeInfo(DWORD dwItemID_)
{
	std::map< std::pair<DWORD, int>, KDropItemInfo >::iterator mit;

	for (mit = m_mapTradeItem.begin(); mit != m_mapTradeItem.end(); ++mit)
	{
		if (mit->first.first / 10 == dwItemID_)
		{
			return std::make_pair(mit->first.first, mit->first.second);
		}
	}

	return std::make_pair(0, 0);
}

void KGCItemManager::SetTradeItem(std::map< std::pair<DWORD, int>, KDropItemInfo>& mapTradeItem_, std::map<DWORD, KDropItemInfo>& mapAllTradeItem_)
{
	m_mapTradeItem = mapTradeItem_;
	m_mapAllTradeItem = mapAllTradeItem_;
}

void KGCItemManager::UpdateInventory(const std::vector< KItemUnit >& vecItemUnit_)
{
	std::vector< KItemUnit >::const_iterator vit = vecItemUnit_.begin();

	for (; vit != vecItemUnit_.end(); ++vit)
	{
		KItemUnit   kItemUnit = (*vit);
		int iItemUID = kItemUnit.m_iUID;

		if (0 == iItemUID)
			continue;

		int iSlotIndex = g_MyD3D->GetMyShortCutSlot()->FindItemIndex(kItemUnit.m_iID / 10);

		if (-1 == iSlotIndex)
		{
			if (0 <= kItemUnit.m_iDuration)
			{
				m_kInventory.RemoveItem(iItemUID);
				continue;
			}
		}
		else
		{
			int iCorrection = g_MyD3D->GetMyShortCutSlot()->FindItem(iSlotIndex)->GetQuantity();

			if (0 < iCorrection && iCorrection <= kItemUnit.m_iDuration)
				kItemUnit.m_iDuration -= iCorrection;
		}


		KItem* pInventoryItem = m_kInventory.FindItemByItemUID(iItemUID);
		if (pInventoryItem == NULL) {
			KItem kInvenItem;
			kInvenItem.m_ItemUID = iItemUID;
			m_kInventory.AddItem(kInvenItem);
			pInventoryItem = m_kInventory.FindItemByItemUID(iItemUID);
		}

		pInventoryItem->m_ItemID = kItemUnit.m_iID / 10;
		pInventoryItem->m_nCount = kItemUnit.m_iDuration;
		//pInventoryItem->m_nPeriod = kItemUnit.m_iPeriod;
		pInventoryItem->m_tEndDate = kItemUnit.m_tEndDate;
		pInventoryItem->m_cEnchantLevel = kItemUnit.m_cEnchantLevel;
	}


}

bool KGCItemManager::DecreaseDuration(GCITEMUID dwItemUID_, bool bSendServer_)
{
	KItem* pkInvenItem = m_kInventory.FindItemByItemUID(dwItemUID_);
	if (pkInvenItem == NULL)
		return false;

	if (pkInvenItem->m_nCount <= 0)
	{
		m_kInventory.RemoveItem(dwItemUID_);
		return false;
	}

	pkInvenItem->m_nCount--;

	if (pkInvenItem->m_nCount == 0)
	{
		m_kInventory.RemoveItem(dwItemUID_);
	}

	if (bSendServer_)
	{
		std::vector<GCITEMUID> vecDecItemUID;
		vecDecItemUID.push_back(dwItemUID_);
		KP2P::GetInstance()->Send_UseDurationItemReq(vecDecItemUID);
	}

	return true;
}

bool KGCItemManager::CheckEquipItemByItemUID(SCharInfo* pCharInfo, GCITEMUID dwItemUID_)
{
	KUserItemVector::iterator vitEquip = std::find_if(pCharInfo->vecItemInfo.begin(), pCharInfo->vecItemInfo.end(), boost::bind(&SUserItem::dwItemUID, _1) == dwItemUID_);
	KUserItemVector::iterator vitCoordi = std::find_if(pCharInfo->vecLookInfo.begin(), pCharInfo->vecLookInfo.end(), boost::bind(&SUserItem::dwItemUID, _1) == dwItemUID_);
	std::vector<KSimpleItem>::iterator vitPet = std::find_if(pCharInfo->kPetInfo.m_vecEquipItem.begin(), pCharInfo->kPetInfo.m_vecEquipItem.end(), boost::bind(&KSimpleItem::m_dwUID, _1) == dwItemUID_);
	bool bPetCoustum = pCharInfo->kPetInfo.m_kCostume.m_dwUID == dwItemUID_;

	if (pCharInfo->kPetInfo.m_dwUID == dwItemUID_) return true;
	return ((pCharInfo->vecItemInfo.end() != vitEquip) || (vitCoordi != pCharInfo->vecLookInfo.end()) || (vitPet != pCharInfo->kPetInfo.m_vecEquipItem.end()) || bPetCoustum);
}

bool KGCItemManager::GetEquipItemUIDByItemID(SCharInfo* pCharInfo, DWORD dwitemID_, OUT GCITEMUID& dwItemUID_)
{
	KUserItemVector::iterator vit = std::find_if(pCharInfo->vecItemInfo.begin(), pCharInfo->vecItemInfo.end(), boost::bind(&SUserItem::iItemID, _1) == dwitemID_);

	bool bResult = pCharInfo->vecItemInfo.end() != vit;

	if (bResult)
		dwItemUID_ = (*vit).dwItemUID;

	return bResult;
}

std::wstring KGCItemManager::GetItemTypeString(GCItem* pItem_)
{
	DWORD dwSlotPosition = pItem_->dwSlotPosition;
	DWORD dwSkillPosition = pItem_->dwSkillPosition;
	DWORD dwPetEquipItemSlot = pItem_->dwPetEquipItemSlot;
	GCITEM_KIND eItemKind = pItem_->eItemKind;

	if (GIK_MONSTER_CARD == eItemKind)
		dwSlotPosition = 0;

	int iStringNumber = 0;
	if (dwSlotPosition & ESP_HELMET)
	{
		if (GIK_COUPLE_ITEM == eItemKind)
			iStringNumber = STR_ID_COUPLE_ITEM1;
		else
			iStringNumber = STR_ID_ITEM_HEAD;
	}
	else if (dwSlotPosition & ESP_JACKET)
	{
		if (GIK_COUPLE_ITEM == eItemKind)
			iStringNumber = STR_ID_COUPLE_ITEM2;
		else
			iStringNumber = STR_ID_ITEM_BODY1;
	}
	else if (dwSlotPosition & ESP_PANTS)
	{
		if (GIK_COUPLE_ITEM == eItemKind)
			iStringNumber = STR_ID_COUPLE_ITEM3;
		else
			iStringNumber = STR_ID_ITEM_BODY2;
	}
	else if (dwSlotPosition & ESP_GLOVES)
	{
		iStringNumber = STR_ID_ITEM_HAND;
	}
	else if (dwSlotPosition & ESP_SHOES)
	{
		if (GIK_COUPLE_ITEM == eItemKind)
			iStringNumber = STR_ID_COUPLE_ITEM4;
		else
			iStringNumber = STR_ID_ITEM_SHOOSE;
	}
	else if (dwSlotPosition & ESP_MANTLE)
	{
		iStringNumber = STR_ID_ITEM_MANTLE;
	}
	else if (dwSlotPosition & ESP_WEAPON)
	{
		iStringNumber = STR_ID_ITEM_WEAPON;
	}
	else if (dwSlotPosition & ESP_A_UPPER_HELMET)
	{
		iStringNumber = STR_ID_ITEM_HEAD_UP;
	}
	else if (dwSlotPosition & ESP_A_DOWN_HELMET)
	{
		iStringNumber = STR_ID_ITEM_HEAD_DOWN;
	}
	else if (dwSlotPosition & ESP_A_NECKLET)
	{
		iStringNumber = STR_ID_ITEM_NECKLACE;
	}
	else if (dwSlotPosition & ESP_A_JACKET)
	{
		iStringNumber = STR_ID_ITEM_BODY1_ACC;
	}
	else if (dwSlotPosition & ESP_A_PANTS)
	{
		iStringNumber = STR_ID_ITEM_BODY2_ACC;
	}
	else if (dwSlotPosition & ESP_A_ARM)
	{
		iStringNumber = STR_ID_ITEM_ARM_ACC;
	}
	else if (dwSlotPosition & ESP_A_FINGER)
	{
		if (GIK_COUPLE_ITEM == eItemKind)
			iStringNumber = STR_ID_COUPLE_ITEM0;
		else
			iStringNumber = STR_ID_ITEM_RING;
	}
	else if (dwSlotPosition & ESP_A_SHOES)
	{
		iStringNumber = STR_ID_ITEM_SHOOSE_ACC;
	}
	else if (dwSlotPosition & ESP_HELMET_SOCKET)
	{
		iStringNumber = STR_ID_ITEM_HEAD;
	}
	else if (dwSlotPosition & ESP_WEAPON_SOCKET)
	{
		iStringNumber = STR_ID_ITEM_WEAPON;
	}
	else if (dwSlotPosition & ESP_JACKET_SOCKET)
	{
		iStringNumber = STR_ID_ITEM_BODY1;
	}
	else if (dwSlotPosition & ESP_A_HELMET_SOCKET)
	{
		iStringNumber = STR_ID_ITEM_HEAD_UP;
	}
	else if (dwSlotPosition & ESP_SLOT_ITEM)
	{
#if defined(NATION_THAILAND)
		iStringNumber = STR_ID_QUICK_SLOT_ITEM;
#else
		iStringNumber = STR_ID_QUICK_SLOT_POTION;
#endif
	}
	else if (dwSlotPosition & ESP_SLOT_EMOT)
	{
		iStringNumber = STR_ID_QUICK_SLOT_EMOTICON;
	}
	else if (dwSlotPosition & ESP_TITLE)
	{
		iStringNumber = STR_ID_SLOT_POSITION_TITLE;
	}
	else if (dwSlotPosition & ESP_PET)
	{
		iStringNumber = STR_ID_ITEM_PET;
	}
	else if (dwPetEquipItemSlot & EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM)
	{
		iStringNumber = STR_ID_ITEM_PETITEM;
	}
	else if (dwPetEquipItemSlot & EPET_EQUIP_ITEM_SLOT_SKILL)
	{
		iStringNumber = STR_ID_ITEM_PETSKILL;
	}
	else if (dwSkillPosition)
	{
		iStringNumber = STR_ID_ITEM_SPECIAL_SKILL;
	}
	else if (GIK_RECIPE == eItemKind)
	{
		iStringNumber = STR_ID_ITEM_KIND_RECIPE;
	}
	else if (GIK_MISSION_SCROLL == eItemKind)
	{
		iStringNumber = STR_ID_ITEM_KIND_MISSION_SCROLL;
	}
	else if (GIK_MONSTER_CARD == eItemKind)
	{
		iStringNumber = STR_ID_ITEM_KIND_MONSTERCARD;
	}
	else
	{
		iStringNumber = STR_ID_ITEM_KIND_EXTRA_ITEM;
	}


	return g_pkStrLoader->GetString(iStringNumber);
}

std::wstring KGCItemManager::GetCoordiSlotString(ESLOTPOSITION emSlotPosition)
{
	int iStringNumber = -1;

	switch (emSlotPosition)
	{
	case ESP_TITLE:
		iStringNumber = STR_ID_SLOT_POSITION_TITLE;
		break;
	case ESP_SET_ITEM:
		iStringNumber = STR_ID_COORDI_SETITEM;
		break;
	case ESP_HELMET:
		iStringNumber = STR_ID_COORDI_HELMET;
		break;
	case ESP_JACKET:
		iStringNumber = STR_ID_COORDI_JACKET;
		break;
	case ESP_PANTS:
		iStringNumber = STR_ID_COORDI_PANTS;
		break;
	case ESP_GLOVES:
		iStringNumber = STR_ID_COORDI_GLOVES;
		break;
	case ESP_SHOES:
		iStringNumber = STR_ID_COORDI_SHOES;
		break;
	case ESP_MANTLE:
		iStringNumber = STR_ID_COORDI_MANTLE;
		break;
	case ESP_WEAPON:
		iStringNumber = STR_ID_COORDI_WEAPON;
		break;
	case ESP_A_UPPER_HELMET:
		iStringNumber = STR_ID_COORDI_A_UPPER_HELMET;
		break;
	case ESP_A_DOWN_HELMET:
		iStringNumber = STR_ID_COORDI_A_DOWN_HELMET;
		break;
	case ESP_A_JACKET:
		iStringNumber = STR_ID_COORDI_A_JACKET;
		break;
	case ESP_A_PANTS:
		iStringNumber = STR_ID_COORDI_A_PANTS;
		break;
	case ESP_A_ARM:
		iStringNumber = STR_ID_COORDI_A_ARM;
		break;
	case ESP_A_NECKLET:
		iStringNumber = STR_ID_COORDI_NECKLET;
		break;
	case ESP_A_SHOES:
		iStringNumber = STR_ID_COORDI_A_SHOES;
		break;
	case ESP_NONE:
	case ESP_A_FINGER:
	case ESP_SLOT_ITEM:
	case ESP_HELMET_SOCKET:
	case ESP_WEAPON_SOCKET:
	case ESP_JACKET_SOCKET:
	case ESP_SLOT_EMOT:
	case ESP_A_HELMET_SOCKET:
	case ESP_A_EARRING:
		iStringNumber = -1;
		break;
	default:
		iStringNumber = -1;
	}

	return g_pkStrLoader->GetString(iStringNumber);
}

std::wstring KGCItemManager::GetItemGradeString(char cItemGrade_)
{
	KItem::ITEM_GRADE eItemGrade = static_cast<KItem::ITEM_GRADE>(cItemGrade_);

	switch (eItemGrade)
	{
	case KItem::GRADE_NORMAL:
		return g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_STRING1);
	case KItem::GRADE_RARE:
		return g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_STRING2);
	case KItem::GRADE_EPIC:
		return g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_STRING4);
	case KItem::GRADE_LEGEND:
		return g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_STRING5);
	case KItem::GRADE_MYSTIC:
		return g_pkStrLoader->GetString(STR_ID_ITEM_GRADE_STRING6);
	}
	return std::wstring();
}

std::wstring KGCItemManager::GetItemGradeColorKey(char cItemGrade_)
{
	KItem::ITEM_GRADE eItemGrade = static_cast<KItem::ITEM_GRADE>(cItemGrade_);

	switch (eItemGrade)
	{
	case KItem::GRADE_NORMAL:
		return L"#cffffff";
	case KItem::GRADE_RARE:
		return L"#c00ccff";
	case KItem::GRADE_EPIC:
		return L"#cffeb00";
	case KItem::GRADE_LEGEND:
		return L"#cff66ff";
	case KItem::GRADE_MYSTIC:
		return L"#cdd2443";
	}

	return L"#cffffff";
}

DWORD KGCItemManager::GetItemGradeColorRGB(char cItemGrade_)
{
	KItem::ITEM_GRADE eItemGrade = static_cast<KItem::ITEM_GRADE>(cItemGrade_);

	switch (eItemGrade)
	{
	case KItem::GRADE_NORMAL:
		return D3DCOLOR_RGBA(255, 255, 255, 255);
	case KItem::GRADE_RARE:
		return D3DCOLOR_RGBA(0, 136, 255, 255);
	case KItem::GRADE_EPIC:
		return D3DCOLOR_RGBA(255, 235, 0, 255);
	case KItem::GRADE_LEGEND:
		return D3DCOLOR_RGBA(255, 102, 255, 255);
	case KItem::GRADE_MYSTIC:
		return D3DCOLOR_RGBA(221, 36, 67, 255);
	}

	return D3DCOLOR_RGBA(255, 255, 255, 255);
}

std::wstring KGCItemManager::GetItemAtrributeString(GCItem* pItem_)
{
	wostringstream strm;
	return strm.str();
}

std::wstring KGCItemManager::GetItemLevelString(GCItem* pItem_)
{
	char cEquipWithBlacksmithPermit = 0;
	std::wstringstream stm;
	SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();

	std::wstring strColorKey;
	std::wstring strColorKeyEnd = L"#cX";

	if (g_pItemMgr->FindInventoryForItemID(ITEM_BLACKSMITH_PERMIT)) {
		cEquipWithBlacksmithPermit = 10;
	}
	else {
		cEquipWithBlacksmithPermit = 0;
	}

	if (pCharInfo->iLevel >= pItem_->iNeedLevelForEquippingItem - cEquipWithBlacksmithPermit)
	{
		if (cEquipWithBlacksmithPermit > 0)
			strColorKey = L"#c00ff00";
		else
			strColorKey = L"#cffffff";
	}
	else
	{
		strColorKey = L"#cff0000";
	}
	stm << strColorKey << g_pkStrLoader->GetString(STR_ID_INFO_NEED_LEVEL) << L" " << pItem_->iNeedLevelForEquippingItem << strColorKeyEnd;
	return stm.str();
}

std::vector< GCItem* > KGCItemManager::GetMonsterCardList()
{
	return m_kMonsterCardManager.GetMonsterCardList();
}

std::vector< GCItem* > KGCItemManager::GetOwnMonsterCardList()
{
	std::vector< GCItem* > vecOwnMonsterCardList;
	std::vector< GCItem* > vecMonsterCardList = m_kMonsterCardManager.GetMonsterCardList();

	std::vector< GCItem* >::iterator vitMonsterCard = vecMonsterCardList.begin();
	for (; vitMonsterCard != vecMonsterCardList.end(); ++vitMonsterCard)
	{
		if (NULL != m_kInventory.FindItemByItemID((*vitMonsterCard)->dwGoodsID))
			vecOwnMonsterCardList.push_back((*vitMonsterCard));
	}

	return vecOwnMonsterCardList;
}

void KGCItemManager::GetMonsterCardRewardList(const std::vector<GCITEMID>& _vecMaterial, std::vector<GCITEMID>& _vecReward)
{
	m_kMonsterCardManager.GetMonsterCardRewardList(_vecMaterial, _vecReward);
}

bool KGCItemManager::IsChannelItemList(GCITEMID _itemID, DWORD dwChannel)
{
	if (m_mapChannelItemList.empty())
		return false;

	std::map<GCITEMID, DWORD>::iterator pos = m_mapChannelItemList.find(_itemID);

	if (pos != m_mapChannelItemList.end())
	{
		if (pos->second != dwChannel)
			return true;
	}

	return false;
}

std::wstring KGCItemManager::GetAtrributeString( char cAtrributeType_, float fAbility_ )
{
	EAttributeType eAttrType = static_cast<EAttributeType>(cAtrributeType_);

	std::wstring strMark;
	if (fAbility_ < 0.0f) {
		strMark = L"-";
	}
	else {
		strMark = L"+";
	}

	fAbility_ = ASMfabs(fAbility_);

	switch (eAttrType)
	{
	case ATTRTYPE_ATK_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING0, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_DEF_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING1, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_HP_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING2, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_SPECIAL_ATK_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING3, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_SPECIAL_DEF_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING4, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_HP_DRAIN_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING5_S4, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_MP_DRAIN_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING6_S4, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_HP_RECOVERY_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING7, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_MP_RECOVERY_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING8, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING9, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING10, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING11, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_CRITICAL_ATTACK_RATE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING12, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_COUNTER_DEF:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING13, "d", static_cast<double>(fAbility_));
	case ATTRTYPE_ROLLING:
		return g_pkStrLoader->GetString(STR_ID_ITEM_ATTRIBUTE_STRING14);
	case ATTRTYPE_ATTACK_PERCENT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING15, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_DEFENCE_PERCENT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING16, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_HP_PERCENT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING17, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_FAST_HP_RECOVERY_DURING_LIMIT_TIME:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING18, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_IGNORE_HASTE:
		return g_pkStrLoader->GetString(STR_ID_ITEM_ATTRIBUTE_STRING19);
	case ATTRTYPE_AUTO_RECOVERY_HP_LIMIT_IN_DUNGEON:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING20, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_ABNORMAL_CONDITION_RESISTANCE:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING21, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_ARENA_ABSENCE_TIME:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING22, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_SKILL_POINT_EXP_BONUS:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING23, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_ALL_STAT_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING24, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_ALL_STAT_PERCENT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING25, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_DARK_DAMAGE_ATK_POINT:
		return g_pkStrLoader->GetReplacedString(STR_ID_ITEM_ATTRIBUTE_STRING26, "li", strMark, static_cast<int>(fAbility_));
	case ATTRTYPE_CRITICAL_DAMAGE_RESIST:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC02, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_CRITICAL_DAMAGE_UP:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_MARK_DESC07, "ld", strMark, static_cast<double>(fAbility_));
	case ATTRTYPE_CATCH_DAMAGE_RESIST:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC03, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_RECOVERY_UP_PET_MP:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC04, "d", static_cast<double>(fAbility_));
	case ATTRTYPE_MOVEMENT_SPEED_UP:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC05, "d", static_cast<double>(fAbility_));
	case ATTRTYPE_RECOVERY_UP_POTION:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC06, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_SUPER_ARMOR_BUFF:
		return g_pkStrLoader->GetString(STR_ID_ENCHANT_ATTRIBUTE_DESC09);
	case ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC10, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_ATTACK_UP_BUFF:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC11, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_SPEEP_UP_BUFF:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC12, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_RECOVERY_POTION_UP:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC13, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC14, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC08, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC01, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_ELYOS_RESIST:
		return g_pkStrLoader->GetReplacedString(STR_ID_ENCHANT_ATTRIBUTE_DESC15, "i", static_cast<int>(fAbility_));
	case ATTRTYPE_SPECIAL_ATTACK_PERCENT:
		return g_pkStrLoader->GetReplacedString(STR_SPATTACK_PER, "ld", strMark, static_cast<double>(fAbility_));
	default:
		return g_pkStrLoader->GetString(STR_ID_ITEM_UNKNOWN_STRING);
	}
}

std::wstring KGCItemManager::GetSocketStateString(char eItemSocketState_)
{
	switch (eItemSocketState_)
	{
	case KSocketInfo::STS_INIT:
		return g_pkStrLoader->GetString(STR_ID_ITEM_UNKNOWN_STRING);
	case KSocketInfo::STS_CLOSED:
		return g_pkStrLoader->GetString(STR_ID_ITEM_SOCKET_STATE_CLOSED);
	case KSocketInfo::STS_OPENED:
		return g_pkStrLoader->GetString(STR_ID_ITEM_SOCKET_STATE_OPENED);
	case KSocketInfo::STS_BREAK:
		return g_pkStrLoader->GetString(STR_ID_ITEM_SOCKET_STATE_UNUSABLE);
	default:
		return g_pkStrLoader->GetString(STR_ID_ITEM_UNKNOWN_STRING);
	}
}

std::wstring KGCItemManager::GetItemName(GCItem* pItem_, char cPetPromotion_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_)
{
	std::wstringstream strmItemName;

	if (NULL == pPetInfo_ &&
		(pInven_ && GIK_PET == pItem_->eItemKind && pItem_->ePetItemKind == EPET_KIND_PET))
	{
		pPetInfo_ = SiKGCDepotManager()->GetPetInfo(pInven_);
	}

	if (GIK_ENCHANT_ENABLE_ITEM == pItem_->eItemKind)
	{
		char cEnchantLevel = 0;
		if (pInven_)
			strmItemName << L"+" << static_cast<int>(pInven_->m_cEnchantLevel) << " " << pItem_->strItemName;
		else if (pUserItem_)
			strmItemName << L"+" << static_cast<int>(pUserItem_->cEnchantLevel) << " " << pItem_->strItemName;
		else
			strmItemName << pItem_->strItemName;
	}
	else if (pInven_ && pInven_->m_EnchantEquipItemUID != 0)
	{
		KItem* pkEnchantItem = GetInventoryItem(pInven_->m_EnchantEquipItemUID);
		if (pkEnchantItem)
			strmItemName << L"+" << static_cast<int>(pkEnchantItem->m_cEnchantLevel) << " " << pItem_->strItemName;
		else
			strmItemName << pItem_->strItemName;
	}
	else if (pUserItem_ && pUserItem_->cEnchantLevel != 0)
	{
		strmItemName << L"+" << static_cast<int>(pUserItem_->cEnchantLevel) << " " << pItem_->strItemName;
	}
	else if (pPetInfo_)
	{
		strmItemName << g_pkStrLoader->GetString(STR_ID_LEVEL) << " " << GetPetLevel(pPetInfo_->m_dwEXP) << " " << pPetInfo_->m_strName;
	}
	else
	{
		int iPetID = g_kGlobalValue.GetPetIDFromItemID(pItem_->dwGoodsID);
		int iPetItemNameID = -1;
		if (-1 != cPetPromotion_ && -1 != iPetID)
			iPetItemNameID = g_kGlobalValue.GetPetTemplate(iPetID, cPetPromotion_).iNameID;;

		if (-1 != iPetItemNameID)
			strmItemName << g_pkStrLoader->GetString(iPetItemNameID);
		else
			strmItemName << pItem_->strItemName;
	}

	return strmItemName.str();
}

std::wstring KGCItemManager::GetItemMoneyTypeString(GCItem* pItem_)
{
	switch (pItem_->eMoneyType)
	{
	case EMT_CASH:
		return g_pkStrLoader->GetString(STR_ID_MONEYTYPE_CASH);
	case EMT_GAMEPOINT:
		return g_pkStrLoader->GetString(STR_ID_MONEYTYPE_GP);
	case EMT_CRYSTAL:
		return g_pkStrLoader->GetString(STR_ID_MONEYTYPE_CRYSTAL);
	case EMT_GEM:
		return g_pkStrLoader->GetString(STR_ID_GEM_TEXT);
	}
#ifdef ENABLE_POINT_SYSTEM // Kawan>
	if (IsPointShopItem(pItem_->dwGoodsID))
		return g_pkStrLoader->GetString(STR_ID_MONEYTYPE_GCPOINT);
	else
#endif
		return std::wstring();
}

std::wstring KGCItemManager::GetItemNameDesc(GCItem* pItem_, char cPetPromotion_)
{
	std::wstringstream strmItemDesc;

	int iPetID = g_kGlobalValue.GetPetIDFromItemID(pItem_->dwGoodsID);
	if (-1 != cPetPromotion_ && -1 != iPetID)
	{
		int iPetItemDecsID = g_kGlobalValue.GetPetTemplate(iPetID, cPetPromotion_).iItemDescID;
		if (-1 != iPetItemDecsID)
		{
			strmItemDesc << g_pkStrLoader->GetString(iPetItemDecsID);
			return strmItemDesc.str();
		}
	}

	strmItemDesc << pItem_->strItemDesc;
	return strmItemDesc.str();
}

void KGCItemManager::GetItemAttribute(PLAYER* pPlayer, int iCharType_, bool CheckState, EItemAbilityData& stEquipItemAbility_)
{
	GetEquipItemAttribute(pPlayer, iCharType_, CheckState, stEquipItemAbility_);

	if (GC_GLOBAL_DEFINE::bReformCoordi)
	{
		GetCoordiItemAttribute(pPlayer, iCharType_, CheckState, stEquipItemAbility_);
	}

	///// ¼ºÀå °³Æí Å©¸®Æ¼ÄÃ °ü·Ã ¼öÄ¡ °è»ê////
	stEquipItemAbility_.fItemAttribute[ATTRTYPE_CRITICAL_ATTACK_RATE] =
		SiGCGrowingSkillManager()->CountSkillCriticalRate(pPlayer, stEquipItemAbility_.fItemAttribute[ATTRTYPE_CRITICAL_ATTACK_RATE]);
	stEquipItemAbility_.fItemAttribute[ATTRTYPE_CRITICAL_DAMAGE_UP] =
		SiGCGrowingSkillManager()->CountSkillCriticalDamage(pPlayer, stEquipItemAbility_.fItemAttribute[ATTRTYPE_CRITICAL_DAMAGE_UP]);
}

void KGCItemManager::GetEquipItemAttribute(PLAYER* pPlayer, int iCharType_, bool CheckState, EItemAbilityData& stEquipItemAbility_)
{
	// 펫 : 아이템 능력치 세팅
	// 펫일 경우에 레벨과 진화여부에 따라 능력치를 세팅해줘야 한다
	SCharInfo* pCharInfo = &(pPlayer->m_kUserInfo.GetCurrentChar(iCharType_));
	KPetInfo kPetinfo = pCharInfo->kPetInfo;
	if (kPetinfo.m_dwUID != 0)
	{
		GCItem* pPetItem = GetItemData(kPetinfo.m_dwID);
		EItemAbilityData kPetAbilityData = GetItemAbilityData(pPetItem, pCharInfo->iLevel, NULL, NULL, &kPetinfo);
		stEquipItemAbility_ += kPetAbilityData;
	}

	bool bDurationOper = false; // 수량 감소 처리 여부
	std::vector<GCITEMUID> vecDecItemUID; // 수량을 감소시킬 아이템의 상품아이디 리스트

	if (CheckState && pPlayer->IsLocalPlayer())
		bDurationOper = true;

	std::map< int, int > mapSetItemEquipCount;
	KUserItemVector::iterator vitUserItem = pCharInfo->vecItemInfo.begin();

	for (; vitUserItem != pCharInfo->vecItemInfo.end(); ++vitUserItem)
	{
		GCItem* kItem = NULL;
		KItem* kInvenItem = NULL;

		if (false == pPlayer->IsLocalPlayer())//아이템이 인벤토리에 없어도 됨
		{
			kItem = GetItemData((*vitUserItem).iItemID);
			if (NULL == kItem)
				continue;
		}
		else
		{
			if (0 == (*vitUserItem).dwItemUID || false == FindInventory((*vitUserItem).dwItemUID, &kInvenItem, &kItem) || NULL == kItem || NULL == kInvenItem)
			{
				kItem = GetItemData((*vitUserItem).iItemID);
				if (NULL == kItem)
					continue;
			}

			//수량이 다 떨어졌음.
			if (true == CheckState && kItem->eItemType == GCIT_COUNT && (NULL == kInvenItem || kInvenItem->m_nCount <= 0))
				continue;
		}

		EItemAbilityData stUserItemAbility = GetItemAbilityData(kItem, pCharInfo->iLevel, kInvenItem, &(*vitUserItem));
		stEquipItemAbility_ += stUserItemAbility;

		std::set<int> setSetItemName = GetSetEffectItemName((*vitUserItem).iItemID);

		//  셋트 아이템 체크
		BOOST_FOREACH(int iSetName, setSetItemName)
		{
			std::map< int, int >::iterator mit = mapSetItemEquipCount.find(iSetName);

			if (mapSetItemEquipCount.end() != mit)
				++(mit->second);
			else
				mapSetItemEquipCount.insert(std::map< int, int >::value_type(iSetName, 1));
		}

		//자동 수량 감소이고 아지트가 아닐면 깍아준다.
#if defined( USE_AGIT_SYSTEM )
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT) {
			continue;
		}
#endif

		// 자동 수량 감소이지만 던전에서만 깍아야 하는 경우( 영던 제외 )
#if defined( USE_EVENT_MON_SUMMON_RING )
		if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON ||
			SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST46 ||
			SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST47 ||
			SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST52) {
			continue;
		}
#endif

		if (bDurationOper)
		{
			if (kItem->bAutoDecrease && kItem->eItemType == GCIT_COUNT && !NotDecDurationItem(static_cast<GCITEMID>((*vitUserItem).iItemID)))
			{
				if (kItem->dwSkillPosition != ESP_SKC_INVALID)
				{
					if (kItem->iNeedChangeJobLevelForEquippingItem == pCharInfo->iPromotionLevel)
					{
						DecDurationItem((*vitUserItem).dwItemUID, &vecDecItemUID);
					}
				}
				else
				{
					DecDurationItem((*vitUserItem).dwItemUID, &vecDecItemUID);
				}
			}
		}
	}

	std::vector< SetItemAttribute > vecSetItemAttr;
	std::map< int, int >::iterator mitEquipSet = mapSetItemEquipCount.begin();
	for (; mitEquipSet != mapSetItemEquipCount.end(); ++mitEquipSet)
	{
		std::vector< SetItemAttributeList > vecSetItemAttrList;

		if (GetEquipSetItemAttributeBySetName(mitEquipSet->first, vecSetItemAttrList))
		{
			std::vector< SetItemAttributeList>::iterator vitSetItemAttrList = vecSetItemAttrList.begin();

			for (; vitSetItemAttrList != vecSetItemAttrList.end(); ++vitSetItemAttrList)
			{
				if (mitEquipSet->second >= (*vitSetItemAttrList).iEquipCount)
					std::copy((*vitSetItemAttrList).vecSetItemAttribute.begin(), (*vitSetItemAttrList).vecSetItemAttribute.end(), back_inserter(vecSetItemAttr));
			}
		}
	}

	std::vector< SetItemAttribute >::iterator vit = vecSetItemAttr.begin();

	for (; vit != vecSetItemAttr.end(); ++vit)
	{
		stEquipItemAbility_.fItemAttribute[(*vit).eAttributeType] += (*vit).fAbility;
	}

	if (vecDecItemUID.size() > 0)
	{
		//::Sleep( 100 );// 패킷 소실을 막기 위한 장치
		//// 게임 시작 패킷이 먼저 날아간 이후에 보내야 한다.

		KP2P::GetInstance()->Send_UseDurationItemReq(vecDecItemUID);
	}

#if defined( COUPLE_SYSTEM )
	KUserItemVector* pVecCoupleItem = NULL;
	if (SiKGCCoupleSystem()->GetCoupleItemVector(pPlayer->m_kUserInfo.dwUID, &pVecCoupleItem) || pVecCoupleItem)
	{
		KUserItemVector::iterator vitCouple = pVecCoupleItem->begin();

		for (; vitCouple != pVecCoupleItem->end(); ++vitCouple)
		{
			GCItem* pCoupleItem = GetItemData((*vitCouple).iItemID);
			EItemAbilityData stCoupleItemAbilityData = GetItemAbilityData(pCoupleItem, pCharInfo->iLevel, NULL, &(*vitCouple));
			stEquipItemAbility_ += stCoupleItemAbilityData;
		}
	}
#endif

	stEquipItemAbility_.fItemAttribute[ ATTRTYPE_ALL_STAT_PERCENT ] += g_kGlobalValue.GetGradeBonus( pPlayer );

	pPlayer->SetCollectionData();
	for ( auto& element : pPlayer->m_vecCharacterCollectionAttributeList )
		stEquipItemAbility_.fItemAttribute[ (EAttributeType)element.m_iAttribute ] += element.m_fAttributeValue;
	for ( auto& element2 : pPlayer->m_vecVIPCollectionAttributeList )
		stEquipItemAbility_.fItemAttribute[ (EAttributeType)element2.m_iAttribute ] += element2.m_fAttributeValue;
}

void KGCItemManager::GetCoordiItemAttribute(PLAYER* pPlayer, int iCharType_, bool CheckState, EItemAbilityData& stEquipItemAbility_)
{
	SCharInfo* pCharInfo = &(pPlayer->m_kUserInfo.GetCurrentChar(iCharType_));

	std::map< int, int > mapSetItemEquipCount;
	KUserItemVector::iterator vitUserItem = pCharInfo->vecLookInfo.begin();
	int nJobLevel = pPlayer->GetWeaponJobLevel();

	for (; vitUserItem != pCharInfo->vecLookInfo.end(); ++vitUserItem)
	{
		GCItem* kItem = NULL;
		KItem* kInvenItem = NULL;

		kItem = GetItemData((*vitUserItem).iItemID);
		if (!kItem)
			continue;

		if (kItem->eItemKind != GIK_COORDI_ITEM)
			continue;

		//  ¹«±â ÀÏ°æ¿ì ÇöÀç ÀåÂøÇÑ ¹«±â¿Í ÄÚµð ÀüÁ÷ Á¤º¸°¡ °°¾Æ¾ß ÇÑ´Ù.
		if (kItem->dwSlotPosition & ESP_WEAPON)
		{
			if (nJobLevel != kItem->iNeedChangeJobLevelForEquippingItem)
			{
				continue;
			}
		}

		EItemAbilityData stUserItemAbility = GetItemAttributeAbilityData(kItem, pCharInfo->iLevel, kInvenItem, &(*vitUserItem));
		stEquipItemAbility_ += stUserItemAbility;

		int nSetItemName = GetCoordiSeasonNum((*vitUserItem).iItemID, (*vitUserItem).cGradeID);
		if (nSetItemName == UINT_MAX)
			continue;

		std::map< int, int >::iterator mit = mapSetItemEquipCount.find(nSetItemName);

		if (mapSetItemEquipCount.end() != mit)
			//++(mit->second);
			mit->second += g_pItemMgr->GetSlotPostionCount(kItem);
		else
			//mapSetItemEquipCount.insert( std::map< int , int >::value_type( nSetItemName,1 ) );
			mapSetItemEquipCount.insert(std::map< int, int >::value_type(nSetItemName, g_pItemMgr->GetSlotPostionCount(kItem)));
	}


	//  ¼ÂÆ® ¾ÆÀÌÅÛ ´É·ÂÄ¡ °è»ê
	std::vector< SetItemAttribute > vecSetItemAttr;
	std::map< int, int >::iterator mitEquipSet = mapSetItemEquipCount.begin();
	for (; mitEquipSet != mapSetItemEquipCount.end(); ++mitEquipSet)
	{
		std::vector< SetItemAttributeList > vecSetItemAttrList;

		if (GetCoordiSeasonAttribute(mitEquipSet->first, vecSetItemAttrList))
		{
			std::vector< SetItemAttributeList>::iterator vitSetItemAttrList = vecSetItemAttrList.begin();

			for (; vitSetItemAttrList != vecSetItemAttrList.end(); ++vitSetItemAttrList)
			{
				if (mitEquipSet->second >= (*vitSetItemAttrList).iEquipCount)
					std::copy((*vitSetItemAttrList).vecSetItemAttribute.begin(), (*vitSetItemAttrList).vecSetItemAttribute.end(), back_inserter(vecSetItemAttr));
			}
		}
	}

	std::vector< SetItemAttribute >::iterator vit = vecSetItemAttr.begin();

	for (; vit != vecSetItemAttr.end(); ++vit)
	{
		stEquipItemAbility_.fItemAttribute[(*vit).eAttributeType] += (*vit).fAbility;
	}
}

void KGCItemManager::ConvertAttributeToAbility(OUT EAbilityData& stAbilityData_, IN const EItemAbilityData& stAttributeData_)
{
	stAbilityData_.fEquipItemAbility[ABILITY_ATK] = stAttributeData_.fAtk + stAttributeData_.fItemAttribute[ATTRTYPE_ATK_POINT] + stAttributeData_.fItemAttribute[ATTRTYPE_ALL_STAT_POINT];
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] = stAttributeData_.fDef + stAttributeData_.fItemAttribute[ATTRTYPE_DEF_POINT] + stAttributeData_.fItemAttribute[ATTRTYPE_ALL_STAT_POINT];
	stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= (1.0f + (0.01f * (stAttributeData_.fItemAttribute[ATTRTYPE_ATTACK_PERCENT] + stAttributeData_.fItemAttribute[ATTRTYPE_ALL_STAT_PERCENT])));
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= (1.0f + (0.01f * (stAttributeData_.fItemAttribute[ATTRTYPE_DEFENCE_PERCENT] + stAttributeData_.fItemAttribute[ATTRTYPE_ALL_STAT_PERCENT])));
	stAbilityData_.fEquipItemAbility[ABILITY_HP] = stAttributeData_.fHP + stAttributeData_.fItemAttribute[ATTRTYPE_HP_POINT] + stAttributeData_.fItemAttribute[ATTRTYPE_ALL_STAT_POINT];
	stAbilityData_.fEquipItemAbility[ABILITY_HP] *= (1.0f + (0.01f * (stAttributeData_.fItemAttribute[ATTRTYPE_HP_PERCENT] + stAttributeData_.fItemAttribute[ATTRTYPE_ALL_STAT_PERCENT])));
	stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] = stAttributeData_.fItemAttribute[ATTRTYPE_SPECIAL_ATK_POINT];
	stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= (1.0f + (0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_SPECIAL_ATTACK_PERCENT]));
	stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] = stAttributeData_.fItemAttribute[ATTRTYPE_SPECIAL_DEF_POINT];
	stAbilityData_.fEquipItemAbility[ABILITY_HP_DRAIN_RATE] = stAttributeData_.fItemAttribute[ATTRTYPE_HP_DRAIN_RATE];
	stAbilityData_.fEquipItemAbility[ABILITY_MP_DRAIN_RATE] = stAttributeData_.fItemAttribute[ATTRTYPE_MP_DRAIN_RATE];
	stAbilityData_.fEquipItemAbility[ABILITY_HP_RECOVERY_RATE] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_HP_RECOVERY_RATE];
	stAbilityData_.fEquipItemAbility[ABILITY_MP_RECOVERY_RATE] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_MP_RECOVERY_RATE];
	stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_CRITICAL_ATTACK_RATE];
	stAbilityData_.fEquipItemAbility[ABILITY_COUNTER_DEF] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_COUNTER_DEF];
	stAbilityData_.fEquipItemAbility[ABILITY_ROLLING] = stAttributeData_.fItemAttribute[ATTRTYPE_ROLLING];
	stAbilityData_.fEquipItemAbility[ABILITY_FAST_HP_RECOVERY_DURING_LIMIT_TIME] = stAttributeData_.fItemAttribute[ATTRTYPE_FAST_HP_RECOVERY_DURING_LIMIT_TIME];
	stAbilityData_.fEquipItemAbility[ABILITY_AUTO_RECOVERY_HP_LIMIT_IN_DUNGEON] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_AUTO_RECOVERY_HP_LIMIT_IN_DUNGEON];
	stAbilityData_.fEquipItemAbility[ABILITY_ABNORMAL_CONDITION_RESISTANCE] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_ABNORMAL_CONDITION_RESISTANCE];
	stAbilityData_.fEquipItemAbility[ABILITY_ARENA_ABSENCE_TIME] = 0.01f * stAttributeData_.fItemAttribute[ATTRTYPE_ARENA_ABSENCE_TIME];
	stAbilityData_.fEquipItemAbility[ABILITY_IGNORE_HASTE] = stAttributeData_.fItemAttribute[ATTRTYPE_IGNORE_HASTE];
	stAbilityData_.fEquipItemAbility[ABILITY_DARK_DAMAGE_ATK_POINT] = stAttributeData_.fItemAttribute[ATTRTYPE_DARK_DAMAGE_ATK_POINT];
	stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_RESIST] = stAttributeData_.fItemAttribute[ATTRTYPE_CRITICAL_DAMAGE_RESIST];
	stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_DAMAGE_UP] = stAttributeData_.fItemAttribute[ATTRTYPE_CRITICAL_DAMAGE_UP];
	stAbilityData_.fEquipItemAbility[ABILITY_CATCH_DAMAGE_RESIST] = stAttributeData_.fItemAttribute[ATTRTYPE_CATCH_DAMAGE_RESIST];
	stAbilityData_.fEquipItemAbility[ABILITY_RECOVERY_UP_PET_MP] = stAttributeData_.fItemAttribute[ATTRTYPE_RECOVERY_UP_PET_MP];
	stAbilityData_.fEquipItemAbility[ABILITY_MOVEMENT_SPEED_UP] = stAttributeData_.fItemAttribute[ATTRTYPE_MOVEMENT_SPEED_UP];
	stAbilityData_.fEquipItemAbility[ABILITY_RECOVERY_UP_POTION] = stAttributeData_.fItemAttribute[ATTRTYPE_RECOVERY_UP_POTION];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_SUPER_ARMOR_BUFF] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_SUPER_ARMOR_BUFF];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_RECOVERY_MP_UP_BUFF] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_ATTACK_UP_BUFF] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_ATTACK_UP_BUFF];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_SPEEP_UP_BUFF] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_SPEEP_UP_BUFF];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_RECOVERY_POTION_UP] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_RECOVERY_POTION_UP];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_CRITICAL_DAMAGE_UP_BUFF] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_ENCHANT_PROBABILITY_UP] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP];
	stAbilityData_.fEquipItemAbility[ABILITY_FATAL_ENCHANT_DESTORY_RESIST] = stAttributeData_.fItemAttribute[ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST];
	stAbilityData_.fEquipItemAbility[ABILITY_ELYOS_RESIST_BUFF] = stAttributeData_.fItemAttribute[ATTRTYPE_ELYOS_RESIST];
	stAbilityData_.fEquipItemAbility[ABILITY_MISSILE_DAMAGE_RESIST] = 0.f;
}

void KGCItemManager::MakeEquipAbilityUseful(IN EAbilityData& stAbilityData_, bool bStartGame_)
{
	// ¸ó½ºÅÍ¸¦ »ó´ëÇÒ¶§´Â ÀåºñÀÇ ´É·ÂÄ¡ ±×´ë·Î Àû¿ëÀÌ µÈ´Ù
	if (bStartGame_ && SiKGCRoomManager()->IsMonsterGameMode())
	{
#if !defined(LEVEL_DESIGN_STAT_CALC)
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 0.01f;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 0.01f;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 0.01f;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= 0.01f;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= 0.01f;
#endif
	}
	else
	{
#if defined(LEVEL_DESIGN_STAT_CALC)
		const float fAbilityRatio = 0.01f;
#else
		const float fAbilityRatio = 0.0001f;
#endif
#if defined(NATION_BRAZIL)
		if (SiKGCChannelManager()->IsPVPBallanceServer()) {
			stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= fAbilityRatio;
			stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= fAbilityRatio;
			stAbilityData_.fEquipItemAbility[ABILITY_HP] *= fAbilityRatio;
			stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= fAbilityRatio;
			stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= fAbilityRatio;
		}
		else {
#if defined(LEVEL_DESIGN_STAT_CALC)
			stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 0.52f;
			stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 0.04f;
			stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 1.f;
			stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= 0.04f;
			stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= 0.04f;
#else
			stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 0.0052f;
			stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 0.0004f;
			stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 0.01f;
			stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= 0.0004f;
			stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= 0.0004f;
#endif

		}
#else
#ifdef MATCH_BALANCE
#if defined(LEVEL_DESIGN_STAT_CALC)
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 0.52f;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 0.04f;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 1.f;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= 0.04f;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= 0.04f;
#else
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 0.0052f;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 0.0004f;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 0.01f;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= 0.0004f;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= 0.0004f;
#endif

#else
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= fAbilityRatio;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= fAbilityRatio;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] *= fAbilityRatio;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= fAbilityRatio;
		stAbilityData_.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= fAbilityRatio;
#endif

#endif
	}
}

void KGCItemManager::GetItemRealAbility(PLAYER* pPlayer, int iCharType_, bool CheckState, OUT EAbilityData& stRealAbility_)
{
	EItemAbilityData stItemAbilityData;
	GetItemAttribute(pPlayer, iCharType_, CheckState, stItemAbilityData);
	ConvertAttributeToAbility(stRealAbility_, stItemAbilityData);
}

bool KGCItemManager::IsEquippedSameItem(PLAYER* pPlayer_, GCItem* pStandardItem_, GCITEMUID ItemUID_)
{
	if (pStandardItem_->dwSlotPosition & ESP_PET)
	{
		return ItemUID_ == pPlayer_->m_kUserInfo.GetCurrentChar().kPetInfo.m_dwUID;
	}
	else
	{
		KUserItemVector* pVecUserItem = NULL;

		if (GIK_COUPLE_ITEM == pStandardItem_->eItemKind)
			SiKGCCoupleSystem()->GetCoupleItemVector(pPlayer_->m_kUserInfo.dwUID, &pVecUserItem);
		else
			pVecUserItem = &(pPlayer_->m_kUserInfo.GetCurrentChar().vecItemInfo);

		if (NULL == pVecUserItem)
			return false;

		KUserItemVector::iterator vit = std::find_if(pVecUserItem->begin(), pVecUserItem->end(), boost::bind(&SUserItem::dwItemUID, _1) == ItemUID_);
		return (pVecUserItem->end() != vit);
	}
}

bool KGCItemManager::GetEquipItemOfSamePosition(PLAYER* pPlayer_, GCItem* pStandardItem_, OUT GCItem** ppItem_, OUT SUserItem** ppUserItem_, OUT KPetInfo** ppPetInfo_)
{
	if (pStandardItem_->dwSlotPosition & ESP_PET)
	{
		KPetInfo* pEquipPetinfo = &(pPlayer_->m_kUserInfo.GetCurrentChar().kPetInfo);
		if (pEquipPetinfo->m_dwUID != 0)
		{
			if (ppItem_)
				*ppItem_ = GetItemData(pEquipPetinfo->m_dwID);

			if (ppPetInfo_)
				*ppPetInfo_ = pEquipPetinfo;

			return true;
		}
		return false;
	}
	else
	{
		KUserItemVector* pVecUserItem = &(pPlayer_->m_kUserInfo.GetCurrentChar().vecItemInfo);

		if (GIK_COUPLE_ITEM == pStandardItem_->eItemKind)
			SiKGCCoupleSystem()->GetCoupleItemVector(pPlayer_->m_kUserInfo.dwUID, &pVecUserItem);

		if (NULL == pVecUserItem)
			return false;

		KUserItemVector::iterator vit = pVecUserItem->begin();
		for (; vit != pVecUserItem->end(); ++vit)
		{
			GCItem* pEquipItem = GetItemData((*vit).iItemID);

			if (pEquipItem->dwSlotPosition & pStandardItem_->dwSlotPosition)
			{
				if (ppItem_)
					*ppItem_ = pEquipItem;

				if (ppUserItem_)
					*ppUserItem_ = &(*vit);

				return true;
			}
		}
		return false;
	}
}

bool KGCItemManager::IsSetItemCheerUpMotion(const int iCheerUpItemID_)
{
	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		if (iCheerUpItemID_ == m_SetList[i].iSetNum)
		{
			return m_SetList[i].m_bCheerUpMotion;
		}
	}
	return false;
}

std::wstring KGCItemManager::GetRemainTime(IN const KItem& kItem)
{
	if (SiKGCVirtualDepotManager()->IsVirtualDepotItem(kItem))
	{
		return g_pkStrLoader->GetReplacedString(STR_ID_PERIOD_DAY, "i", (int)kItem.m_nPeriod);
	}

	if (CTime(kItem.m_tEndDate + g_kGlobalValue.m_tmServerSyncTime) < g_kGlobalValue.m_tmServerTime)
	{
		return g_pkStrLoader->GetString(STR_ID_TIME_EXPIRED);
	}

	CTimeSpan cSpan = CTime(kItem.m_tEndDate + g_kGlobalValue.m_tmServerSyncTime) - g_kGlobalValue.m_tmServerTime;

	if (cSpan.GetDays() == 0)
	{
		if (cSpan.GetHours() == 0)
			return g_pkStrLoader->GetReplacedString(STR_ID_TIME_MINUIT, "i", (int)cSpan.GetMinutes());

		return g_pkStrLoader->GetReplacedString(STR_ID_TIME_HOUR_MINUIT, "ii", (int)cSpan.GetHours(), (int)cSpan.GetMinutes());

		// xºÐ

	}

	// xÀÏ x½Ã°£
	//CString strTime = cSpan.Format( "%DÀÏ %H½Ã°£" );
	return g_pkStrLoader->GetReplacedString(STR_ID_TIME_DAY_HOUR, "ii", (int)cSpan.GetDays(), (int)cSpan.GetHours());
}

void KGCItemManager::SetMonsterCardScriptInfoFromServer(KEVENT_MONSTER_CARD_MIX_INFO_ACK kPacket_)
{
	m_kMonsterCardManager.SetMonsterCardInfo(kPacket_);
}

void KGCItemManager::LoadMonsterCardManager()
{
	m_kMonsterCardManager.InitMonsterCard();
}

void KGCItemManager::LoadChannelItemList()
{
	KLuaManager luaMgr;
	if (GCFUNC::LoadLuaScript(luaMgr, "channel_item_list.lua") == false)
		return;

	m_mapChannelItemList.clear();

	LUA_BEGIN_TABLE("CHANNEL_ITEM_LIST", return);
	{
		for (int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE(i, break)
			{
				int iItemID = -1;
				int nChannel = 1;
				LUA_GET_VALUE_DEF("GOODSID", iItemID, -1);
				LUA_GET_VALUE_DEF("CHANNEL", nChannel, 1);

				GCItem* pItem = GetItemData(iItemID);
				if (pItem)
				{
					m_mapChannelItemList.insert(std::make_pair(iItemID, nChannel));
				}
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(return)

		return;
}

void KGCItemManager::SetSocketManagementCostTableInfo(std::map< DWORD, KSocketConsumeInfo > mapCashItemuseCount_, std::map< DWORD, KSocketConsumeInfo > mapConsumeGP_, GCITEMID SocketChangeItemID_)
{
	m_mapCashItemUseCount.swap(mapCashItemuseCount_);
	m_mapConsumeGP.swap(mapConsumeGP_);
	m_SocketChangeItemID = SocketChangeItemID_ / 10;
}

bool KGCItemManager::LoadGradeTable()
{
	return m_kGradeDecider.LoadScript();
}

bool KGCItemManager::LoadSocketTable()
{
	return m_kSocketDecider.LoadScript();
}

bool KGCItemManager::LoadAttributeTable()
{
	return m_kAttributeDecider.LoadScript();
}

bool KGCItemManager::LoadTableScript()
{
	KLuaManager luaMgr;
	KLuaManager luaMgr2;

	_JIF(GCFUNC::LoadLuaScript(luaMgr, "ItemTable.lua"), return false);
	_JIF(luaMgr.BeginTable("LoadTableList") == S_OK, return false);

	std::string strFileName;
	for (int i = 1; SUCCEEDED(luaMgr.GetValue(i, strFileName)); i++)
	{
		if (GCFUNC::LoadLuaScript(luaMgr2, strFileName) == false)
			continue;

		if (FAILED(luaMgr2.BeginTable("ItemTableList")))
			continue;

		int iIndex = 0;
		while (++iIndex)
		{
			if (FAILED(luaMgr2.BeginTable(iIndex)))
			{
				break;
			}

			DWORD dwItemID;
			_JIF(luaMgr2.GetValue(1, dwItemID) == S_OK, return false);
			dwItemID /= 10;

			int iGradeTableID = -1;
			int iAttributeTableID = -1;
			int iSocketTableID = -1;
			int iLevelDownTableID = -1;
			int iContinentTypeTableID = -1;

			_JIF(luaMgr2.GetValue(2, iGradeTableID) == S_OK, return false);
			_JIF(luaMgr2.GetValue(3, iAttributeTableID) == S_OK, return false);
			_JIF(luaMgr2.GetValue(4, iSocketTableID) == S_OK, return false);
			_JIF(luaMgr2.GetValue(6, iLevelDownTableID) == S_OK, return false);
			_JIF(luaMgr2.GetValue(7, iContinentTypeTableID) == S_OK, return false);

			SetGradeTableID(dwItemID, iGradeTableID);
			SetAttributeTableID(dwItemID, iAttributeTableID);
			SetSocketTableID(dwItemID, iSocketTableID);
			SetLevelDownTableID(dwItemID, iLevelDownTableID);
			SetContinentTypeTableID(dwItemID, iContinentTypeTableID);

			GCItem* pItem = GetItemData(dwItemID);
			if (pItem)
			{
				if (iGradeTableID != -1)
				{
					KGCGradeDecider::Table kGradeTable;
					m_kGradeDecider.GetTable(iGradeTableID, kGradeTable);

					if (kGradeTable.GetContainer().size() == 1)
					{
						pItem->cItemGrade = kGradeTable.GetGrade();
					}
				}

				if (m_kAttributeDecider.GetTableType(iAttributeTableID, pItem->cItemGrade) == KGCAttributeDecider::Table::ATT_FIXED)
				{
					std::list<std::pair<int, float>> listAttributes;
					m_kAttributeDecider.GetAttributeFullList(iAttributeTableID, pItem->cItemGrade, listAttributes);

					std::list<std::pair<int, float>>::iterator lit;
					int iSlot = 0;
					for (lit = listAttributes.begin(); lit != listAttributes.end(); ++lit, ++iSlot)
					{
						KAttributeInfo kAtt;
						kAtt.m_cType = lit->first;
						kAtt.m_fValue = lit->second;
						kAtt.m_cState = KAttributeInfo::ATS_USING;
						kAtt.m_cSlotID = iSlot;
						pItem->m_vecAttribute.push_back(kAtt);
					}
				}
			}
			luaMgr2.EndTable();
		}
		luaMgr2.EndTable();
	}
	luaMgr.EndTable();

	return true;
}

int KGCItemManager::_GetTableID(DWORD dwItemID, std::map<DWORD, int>& mapTable)
{
	std::map<DWORD, int>::iterator mit = mapTable.find(dwItemID);

	if (mit == mapTable.end()) {
		return -1;
	}

	return mit->second;
}

int KGCItemManager::GetGradeTableID(DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		return pItem->iGrade;
	}
	return -1;

}

int KGCItemManager::GetAttributeTableID(DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		return pItem->iAttribute;
	}
	return -1;

}

int KGCItemManager::GetSocketTableID(DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		return pItem->shSocket;
	}
	return -1;

}

int KGCItemManager::GetLevelDownTableID(DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		return pItem->shLevelDown;
	}
	return 0;

}

int KGCItemManager::GetContinentTypeTableID(DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		return pItem->shContinentType;
	}
	return 0;

}


void KGCItemManager::_SetTableID(DWORD dwItemID, int iTableID, std::map<DWORD, int>& mapTable)
{
	mapTable[dwItemID] = iTableID;
}

void KGCItemManager::SetGradeTableID(DWORD dwItemID, int iTableID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		pItem->iGrade = iTableID;
	}

}

void KGCItemManager::SetAttributeTableID(DWORD dwItemID, int iTableID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		pItem->iAttribute = iTableID;;
	}

}

void KGCItemManager::SetSocketTableID(DWORD dwItemID, int iTableID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		pItem->shSocket = iTableID;;
	}

}

void KGCItemManager::SetLevelDownTableID(DWORD dwItemID, int iTableID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		pItem->shLevelDown = iTableID;;
	}

}
void KGCItemManager::SetContinentTypeTableID(DWORD dwItemID, int iTableID)
{
	GCItem* pItem = GetItemData(dwItemID);
	if (pItem)
	{
		pItem->shContinentType = iTableID;;
	}

}

GCItemAbility KGCItemManager::CalcItemAbility(GCItem* pItem_, KItem* pInven_, char cItemGrade_, int iLevel_, SUserItem* pUserItem_)
{
	GCItemAbility stItemAbility;

	if (NULL == pItem_)
		return stItemAbility;

	stItemAbility.iAtk = pItem_->iAttack;
	stItemAbility.iDef = pItem_->iDefence;
	stItemAbility.iVit = pItem_->iHealth;

	if (m_setAbilityCalcExceptionList.end() != m_setAbilityCalcExceptionList.find(pItem_->dwGoodsID))
		return stItemAbility;


#define DEFAULT_ITEM_ABILITY_TOTAL 500
#define DEFAULT_ITEM_ABILITY_UP 50
#define PLUS_ITEM_ABILITY_UP 150
#define ITEM_ABILITY_INCREASE_PART_DEVIDE 3

	int iPlusLevel = 0;

	if (KItem::GRADE_RARE == cItemGrade_)
		iPlusLevel = 3;
	else if (KItem::GRADE_EPIC == cItemGrade_)
		iPlusLevel = 6;
	else if (KItem::GRADE_LEGEND == cItemGrade_)
		iPlusLevel = 9;
	else if (KItem::GRADE_MYSTIC == cItemGrade_)
		iPlusLevel = 12;

	int iLevel;
	if (iLevel_ != -1)
		iLevel = iLevel_ + iPlusLevel;
	else
		iLevel = GetlItemEquipLevel(pItem_, pInven_, pUserItem_) + iPlusLevel;

	int iTotalAbility = DEFAULT_ITEM_ABILITY_TOTAL + (DEFAULT_ITEM_ABILITY_UP * iLevel) + (PLUS_ITEM_ABILITY_UP * static_cast<int>(iLevel / ITEM_ABILITY_INCREASE_PART_DEVIDE));
	int iCharType = GetUniqueChartype(pItem_->dwCharType);
	int iItemSlotPosition = GetUniqueSlotPosition(pItem_->dwSlotPosition);

	std::map< int, ItemAbilityCalcRateByCharType >::iterator mitAbiltyRateByCharType = m_mapCalcRateByCharType.find(iItemSlotPosition);

	if (m_mapCalcRateByCharType.end() == mitAbiltyRateByCharType)
	{
		return stItemAbility;
	}

	std::map< int, ItemAbtilityPercentByCharType >::iterator mitAbilityPercentByCharType = m_mapAbilityPercentByCharType.find(iItemSlotPosition);

	if (m_mapAbilityPercentByCharType.end() == mitAbilityPercentByCharType)
		return stItemAbility;

	ItemAbilityPercent stItemAbilityPercent;
	std::map< int, ItemAbilityPercent >::iterator mitAbilityPercent;
	float fRealAbilityRate;
	std::map< int, float>::iterator mitRealAbilityRate;

	int iTempAtk = 0, iTempDef = 0, iTempVit = 0;

	stItemAbilityPercent = mitAbilityPercentByCharType->second.stDefaultItemAbtilityPercent;
	mitAbilityPercent = mitAbilityPercentByCharType->second.mapAbilityPercentByCharType.find(iCharType);

	if (mitAbilityPercentByCharType->second.mapAbilityPercentByCharType.end() != mitAbilityPercent)
		stItemAbilityPercent = mitAbilityPercent->second;

	fRealAbilityRate = mitAbiltyRateByCharType->second.fDefaultRate;
	mitRealAbilityRate = mitAbiltyRateByCharType->second.mapAbilityCalcRateByCharType.find(iCharType);

	if (mitAbiltyRateByCharType->second.mapAbilityCalcRateByCharType.end() != mitRealAbilityRate)
		fRealAbilityRate = mitRealAbilityRate->second;

	stItemAbility.iAtk = static_cast<int>(round(iTotalAbility * fRealAbilityRate * stItemAbilityPercent.fAtkCalcRate));
	stItemAbility.iDef = static_cast<int>(round(iTotalAbility * fRealAbilityRate * stItemAbilityPercent.fDefCalcRate));
	stItemAbility.iVit = static_cast<int>(round(iTotalAbility * fRealAbilityRate * stItemAbilityPercent.fVitCalcRate));

	return stItemAbility;
}

int KGCItemManager::GetUniqueChartype(DWORD  dwCharType_)
{
	for (int i = 0; i < GC_CHAR_NUM; ++i)
	{
		if ((1 << i) & dwCharType_)
			return i;
	}

	return 0;
}

int KGCItemManager::GetUniqueSlotPosition(DWORD dwSlotPosition_)
{
	for (int i = 0; i < EISP_NUM; ++i)
	{
		if ((1 << i) & dwSlotPosition_)
			return i;
	}

	return 0;
}

void KGCItemManager::LoadItemAbilityCalcTable()
{
	KLuaManager luaMgr;
	if (GCFUNC::LoadLuaScript(luaMgr, "item_ability_calc.lua") == false)
		return;

	m_mapAbilityPercentByCharType.clear();
	m_mapCalcRateByCharType.clear();
	LUA_BEGIN_TABLE("ITEM_ABILITY_CALC_INFO", return);
	{
		for (int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE(i, break)
			{
				int iSlotPosition = -1;
				ItemAbtilityPercentByCharType stAbilityPercentByCharType;
				ItemAbilityCalcRateByCharType stCalcAbilityRateByCharType;

				LUA_GET_VALUE_DEF("SLOT_POSITION", iSlotPosition, -1);

				LUA_GET_VALUE_DEF("DEFAULT_ATTACK_PERCENT", stAbilityPercentByCharType.stDefaultItemAbtilityPercent.fAtkCalcRate, 0.0f);
				LUA_GET_VALUE_DEF("DEFAULT_DEFENCE_PERCENT", stAbilityPercentByCharType.stDefaultItemAbtilityPercent.fDefCalcRate, 0.0f);
				LUA_GET_VALUE_DEF("DEFAULT_VITAL_PERCENT", stAbilityPercentByCharType.stDefaultItemAbtilityPercent.fVitCalcRate, 0.0f);

				if (SUCCEEDED(luaMgr.BeginTable("ABILITY_PERCENT_BY_CHARTYPE")))
				{
					for (int i = 1; ; ++i)
					{
						LUA_BEGIN_TABLE(i, break)
						{
							int iCharType = -1;
							ItemAbilityPercent stAbilityPercent;
							LUA_GET_VALUE_DEF("CHAR_TYPE", iCharType, -1);
							LUA_GET_VALUE_DEF("ATTACK_PERCENT", stAbilityPercent.fAtkCalcRate, 0.0f);
							LUA_GET_VALUE_DEF("DEFENCE_PERCENT", stAbilityPercent.fDefCalcRate, 0.0f);
							LUA_GET_VALUE_DEF("VITAL_PERCENT", stAbilityPercent.fVitCalcRate, 0.0f);

							stAbilityPercentByCharType.mapAbilityPercentByCharType.insert(std::map< int, ItemAbilityPercent>::value_type(iCharType, stAbilityPercent));
						}
						LUA_END_TABLE(break)
					}
					LUA_END_TABLE(continue)
				}

				m_mapAbilityPercentByCharType.insert(std::map< int, ItemAbtilityPercentByCharType >::value_type(iSlotPosition, stAbilityPercentByCharType));

				LUA_GET_VALUE_DEF("DEFAULT_TOTAL_ABILITY_PER_RATE", stCalcAbilityRateByCharType.fDefaultRate, 0.0f);

				if (SUCCEEDED(luaMgr.BeginTable("CALCRATE_BY_CHARTYPE")))
				{
					for (int i = 1; ; ++i)
					{
						LUA_BEGIN_TABLE(i, break)
						{
							int iCharType = -1;
							float fAbilityRate = 0.0f;
							LUA_GET_VALUE_DEF("CHAR_TYPE", iCharType, -1);
							LUA_GET_VALUE_DEF("TOTAL_ABILITY_PER_RATE", fAbilityRate, 0.0f);

							stCalcAbilityRateByCharType.mapAbilityCalcRateByCharType.insert(std::map< int, float>::value_type(iCharType, fAbilityRate));
						}
						LUA_END_TABLE(break)
					}
					LUA_END_TABLE(continue)
				}
				m_mapCalcRateByCharType.insert(std::map< int, ItemAbilityCalcRateByCharType >::value_type(iSlotPosition, stCalcAbilityRateByCharType));
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(return);
}

void KGCItemManager::LoadItemAbilitCalcExceptionList()
{
	m_setAbilityCalcExceptionList.clear();

	KLuaManager luaMgr;
	if (GCFUNC::LoadLuaScript(luaMgr, "item_ability_calc_exception_list.lua") == false)
		return;

	LUA_BEGIN_TABLE("ITEM_ABILITY_CALC_EXCEPTION_LIST", return);
	{
		for (int i = 1; ; ++i)
		{
			int iItemID = 0;
			LUA_GET_VALUE_NOASSERT(i, iItemID, break)
				m_setAbilityCalcExceptionList.insert(iItemID);
		}
	}
	LUA_END_TABLE(return);
}

void KGCItemManager::BindToLua()
{
	lua_State* L = KGCLuabinder::getInstance().GetLuaState();
	CHECK_STACK(L)
		lua_tinker::class_add<KGCItemManager>(L, "KGCItemManager");
	lua_tinker::class_def<KGCItemManager>(L, "GetInventoryItemDuration", &KGCItemManager::GetInventoryItemDuration);;
	lua_tinker::class_def<KGCItemManager>(L, "GetItemType", &KGCItemManager::GetItemType);;
	lua_tinker::class_def<KGCItemManager>(L, "IsItemToInvenByItemID", &KGCItemManager::IsItemToInvenByItemID);;
	lua_tinker::decl(L, "ItemMgr", this);
}

bool KGCItemManager::IsLimitItemID(GCITEMID ItemID_)
{
	return m_setBuyLimitedItemList.end() != m_setBuyLimitedItemList.find(ItemID_);
}

void KGCItemManager::LoadBuyLimitItem()
{
	m_setBuyLimitedItemList.clear();

	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "ExceptionItem.lua") == false)
		return;

	GCITEMID ItemID;
	LUA_BEGIN_TABLE("BUY_LIMITED_ITEM", return)
	{
		for (int i = 1; ; i++)
		{
			LUA_GET_VALUE_NOASSERT(i, ItemID, break)
				m_setBuyLimitedItemList.insert(ItemID);
		}
	}
	LUA_END_TABLE(return)
}

void KGCItemManager::LoadExceptionConuntItem()
{
	m_mapExceptionCountItem.clear();

	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "ExceptionItem.lua") == false)
		return;

	GCITEMID ItemID;
	int iMode;

	LUA_BEGIN_TABLE("EXCEPTION_COUNT_ITEM", return)
	{
		for (int i = 1; ; i++)
		{
			std::pair< GCITEMID, std::set<int> > mapbuf;
			LUA_BEGIN_TABLE(i, break)
			{
				LUA_GET_VALUE_NOASSERT("ITEMID", ItemID, break)
					mapbuf.first = ItemID;
				LUA_BEGIN_TABLE("MODE", break)
				{
					for (int j = 1;; j++)
					{
						LUA_GET_VALUE_NOASSERT(j, iMode, break)
							mapbuf.second.insert(iMode);
					}
				}
				LUA_END_TABLE(;)

			}
			LUA_END_TABLE(;)
				m_mapExceptionCountItem.insert(mapbuf);
		}
	}
	LUA_END_TABLE(return)

}

bool KGCItemManager::NotDecDurationItem(GCITEMID ItemID_)
{
	MAP_EXCEPTION_COUNT_ITEM::iterator mIter = m_mapExceptionCountItem.find(ItemID_);
	if (mIter == m_mapExceptionCountItem.end())
		return false;

	return mIter->second.find(SiKGCRoomManager()->GetGameMode()) == mIter->second.end();
}

GCItem* KGCItemManager::GetSameImageIndexItemInfo(GCITEMID iItemID_)
{
	map_same_image_index_type_type::iterator mIter = m_mapSameImageIndex.find(iItemID_);// /10ÇÑ°Å Ã£À½.
	if (mIter != m_mapSameImageIndex.end())
	{
		GCItem* pItem = NULL;
		pItem = g_MyD3D->m_kItemMgr.GetItemData(mIter->second);
		return pItem;
	}
	return NULL;
}

INT KGCItemManager::GetSameImageIndex(GCITEMID iItemID_)
{
	map_same_image_index_type_type::iterator mIter = m_mapSameImageIndex.find(iItemID_);// /10ÇÑ°Å Ã£À½.
	if (mIter != m_mapSameImageIndex.end())
	{
		return (INT)mIter->second;
	}
	return (int)iItemID_;
}

INT KGCItemManager::GetSameTextureIndex(GCITEMID iItemID_)
{
	map_same_image_index_type_type::iterator mIter = m_mapSameTextureIndex.find(iItemID_);// /10ÇÑ°Å Ã£À½.
	if (mIter != m_mapSameTextureIndex.end())
	{
		return (INT)mIter->second;
	}
	return (int)iItemID_;
}

GCDeviceTexture* KGCItemManager::CreateShopItemTexture(int iItemID)
{
	int dwItemGoodsID = iItemID;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreateShopItemTexture(dwItemGoodsID);
}

GCDeviceTexture* KGCItemManager::CreateItemTexture(int iItemID, int iCharType, int iCharPromotion)
{
	int dwItemGoodsID = iItemID;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreateItemTexture(dwItemGoodsID, iCharType, iCharPromotion);
}

GCDeviceTexture* KGCItemManager::CreateItemTextureCustom(int iItemID, int iCharType, int iCharPromotion)
{
	return g_pGCDeviceManager2->CreateItemTextureCustom(iItemID, iCharType, iCharPromotion);
}

GCDeviceTexture* KGCItemManager::CreateShopSetItemTexture(int iSetItemNum_, int iItemID)
{
	int dwItemGoodsID = iItemID;
	int iSetItemNum = iSetItemNum_;
	GCItem* pItem = g_pItemMgr->GetSameImageIndexItemInfo(dwItemGoodsID);
	if (pItem != NULL) {
		dwItemGoodsID = pItem->dwGoodsID;
		iSetItemNum = pItem->iSetItemNumber;
	}
	return g_pGCDeviceManager2->CreateShopSetItemTexture(iSetItemNum);
}

GCDeviceTexture* KGCItemManager::CreatePetItemTexture(int iPetGoodsID, int iEvolutionLevel)
{
	int dwItemGoodsID = iPetGoodsID;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreatePetItemTexture(dwItemGoodsID, iEvolutionLevel);
}

GCMeshObject* KGCItemManager::CreateLas4MergeAbtaModel(int iItemID_, int iCharType_, GCObject* pObj_, bool build)
{
	int dwItemGoodsID = iItemID_;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreateLas4MergeAbtaModel(dwItemGoodsID, iCharType_, pObj_, build);
}

GCMeshObject* KGCItemManager::CreateAbtaModel(int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, bool build, char cIgBoneIdx)
{
	int dwItemGoodsID = iItemID_;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreateAbtaModel(dwItemGoodsID, iCharType_, iPromotion, pObj_, build, cIgBoneIdx);
}

GCMeshObject* KGCItemManager::CreateRoomAnimationModel(std::string RoomAnimationModel, GCObject* pObj_, bool build, char cIgBoneIdx)
{
	return g_pGCDeviceManager2->CreateRoomAnimationModel(RoomAnimationModel, pObj_, build, cIgBoneIdx);
}

void KGCItemManager::CreateTmpModel(int iItemID_, int iCharType_, int iPromotion, char cIgBoneIdx/* = -1*/)
{
	int dwItemGoodsID = iItemID_;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreateTmpModel(dwItemGoodsID, iCharType_, iPromotion, cIgBoneIdx);
}

std::vector<GCObject*> KGCItemManager::CreateLiveModel(int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, D3DXVECTOR3 vOptionalPos, bool bOnlyTraceParentPos, char* szMotionFileName, int iBoneIdx, bool bUseBoneIDFromArg)
{
	int dwItemGoodsID = iItemID_;
	dwItemGoodsID = g_pItemMgr->GetSameImageIndex(dwItemGoodsID);
	return g_pGCDeviceManager2->CreateLiveModelList(dwItemGoodsID, iCharType_, iPromotion, pObj_, vOptionalPos, bOnlyTraceParentPos, szMotionFileName, iBoneIdx, bUseBoneIDFromArg);

}

void KGCItemManager::Draw_ShopImage(D3DXVECTOR2 vPos, float fWidth, float fHeight, GCDeviceTexture* tex, bool bSetItem, int iSetItemNumber /*= -1*/, D3DCOLOR col/*=0xffffffff*/, DWORD dwGoodsID_)
{
	int dwItemGoodsID = dwGoodsID_;
	int iSetItemNum = iSetItemNumber;
	GCItem* pItem = g_pItemMgr->GetSameImageIndexItemInfo(dwItemGoodsID);
	if (pItem != NULL) {
		dwItemGoodsID = pItem->dwGoodsID;
		iSetItemNum = pItem->iSetItemNumber;
	}

	if (bSetItem)
		dwItemGoodsID = iSetItemNum;

	return g_pGCDeviceManager2->Draw_ShopImage(vPos, fWidth, fHeight, tex, bSetItem, dwItemGoodsID, col);
}

void KGCItemManager::Draw_ShopImage_Origin(D3DXVECTOR2 vPos, float fWidth, float fHeight, GCDeviceTexture* tex, bool bSetItem, int iSetItemNumber /*= -1*/, D3DCOLOR col/*=0xffffffff*/, DWORD dwGoodsID_ /*=0 */)
{
	return g_pGCDeviceManager2->Draw_ShopImage(vPos, fWidth, fHeight, tex, bSetItem, dwGoodsID_, col);
}

DWORD KGCItemManager::GetBaseItemID(int iCharType, ESLOTPOSITION eSlot)
{
	DWORD aSlotPos[4] = { ESP_HELMET,ESP_JACKET,ESP_PANTS,ESP_WEAPON };

	for (int i = 0; i < 4; i++)
	{
		if (aSlotPos[i] == eSlot)
		{
			return m_vecBaseItem[iCharType * 4 + i];
		}
	}

	return UINT_MAX;
}

int KGCItemManager::GetItemType(DWORD dwItemGoodsID)
{
	GCItem* pItem = GetItemData(dwItemGoodsID);
	if (NULL == pItem) return -1;
	return int(pItem->eItemType);
}

bool KGCItemManager::IsItemToInvenByItemID(DWORD dwItemGoodsID)
{
	return m_kInventory.IsItemByItemID(dwItemGoodsID);
}

bool KGCItemManager::IsBreakUpItem(DWORD dwItemGoodsID)
{
	GCItem* pItem = GetItemData(dwItemGoodsID);
	KItem* pkItem = m_kInventory.FindItemByItemID(dwItemGoodsID);

	if (NULL == pItem || NULL == pkItem || false == IsEquipmentItem(pItem->dwGoodsID) || GIK_PET == pItem->eItemKind || GIK_COUPLE_ITEM == pItem->eItemKind || GIK_MONSTER_CARD == pItem->eItemKind || GIK_TITLE_ITEM == pItem->eItemKind)
		return false;

	if (KItem::UNLIMITED_ITEM != pkItem->m_nInitCount || KItem::UNLIMITED_ITEM != pkItem->m_nPeriod)
		return false;

	if (KGCItemManager::MAXIMUM_PCBANG_UID >= pkItem->m_ItemUID)
		return false;

	if (pItem->eItemKind == GIK_ENCHANT_ASIST_ITEM) {
		return false;
	}

	return true;
}

bool KGCItemManager::IsPutItemInDepot(GCItem* pItem, KItem* pkItem)
{
	if (!pItem)
		return false;

	if (GIK_TITLE_ITEM == pItem->eItemKind || GIK_AGIT_ITEM == pItem->eItemKind)
		return false;

	if (SiKGCAgitGameManager()->IsAgitObject(pItem->dwGoodsID)) {
		return false;
	}

	if ((pItem->iItemTypeValue != -1 && pItem->eItemType == GCIT_PERIOD))
		return false;

	// ¾Æ½Å ºñÀü¼­ ½ºÅ³ 
	if (IsDepotNonvisibleItem(pItem->dwGoodsID)) {
		return false;
	}


	if (pkItem)
	{
		if (pkItem == NULL)
			return false;

		if (KItem::UNLIMITED_ITEM != pkItem->m_nPeriod)
			return false;

		if (KGCItemManager::MAXIMUM_PCBANG_UID >= pkItem->m_ItemUID)
			return false;

		if (GIK_ENCHANT_ENABLE_ITEM == pItem->eItemKind && pkItem->m_EnchantEquipItemUID != 0)
			return false;
	}

	return true;
}

int KGCItemManager::GetItemCategory(GCITEMID itemID_)
{
	GCItem* kItem = GetItemData(itemID_);
	_RETURN_NIL(kItem, -1);
	//¸î ¹øÂ° Ä«Å×°í¸®¿¡ ÇØ´çÇÏ´ÂÁö returnÇÏ´Â ÇÔ¼öÀÌ´Ù.
	for (int i = 0; i < UEC_NUM; i++)
	{
		if (i < UEC_PET)
		{
			if (m_dwItemCategoryFlag[i] & kItem->dwSlotPosition)
			{
				return i;
			}
		}
		else if (i == UEC_PET)
		{
			if (m_dwItemCategoryFlag[i] & kItem->dwPetEquipItemSlot)
			{
				return i;
			}
		}
		else
		{
			if (m_dwItemCategoryFlag[i] & kItem->dwSkillPosition)
			{
				return i;
			}
		}
	}

	return -1;
}

bool KGCItemManager::IsEquipedItem(SCharInfo* pCharInfo, GCITEMUID itemUID_)
{
	DWORD dwCharFlag = 0;
	const KItem* kItem = GetInventoryItem(itemUID_);

	if (pCharInfo != nullptr && kItem != nullptr)
	{
		for (int j = 0; j < (int)pCharInfo->vecItemInfo.size(); j++)
		{
			if (kItem->m_ItemUID == pCharInfo->vecItemInfo[j].dwItemUID)
			{
				return true;
			}

			if (kItem->m_ItemUID == pCharInfo->kPetInfo.m_kCostume.m_dwUID)
			{
				return true;
			}
		}

		KPetInfo kPet = pCharInfo->kPetInfo;
		if (kPet.m_dwUID != 0)
		{
			// ÆêÀ» ÀåÂøÇÏ°í ÀÖ´Ù¸é
			if (kItem->m_ItemUID == kPet.m_dwUID)
			{
				return true;
			}

			// Æê¾ÆÀÌÅÛÀ» ÀåÂøÇÏ°í ÀÖ´Ù¸é
			for (int k = 0; k < (int)kPet.m_vecEquipItem.size(); k++)
			{
				if (kItem->m_ItemUID == kPet.m_vecEquipItem[k].m_dwUID)
				{
					return true;
				}
			}
		}

		for (int k = 0; k < 10; k++)
		{
			if (g_MyD3D->GetMyShortCutSlot()->GetEmotUID(k) == kItem->m_ItemUID)
			{
				return true;
			}
		}

		for (auto petGlyph : pCharInfo->vecPetGlyphInfo)
		{
			if (petGlyph.m_dwUID == kItem->m_ItemUID)
			{
				return true;
			}
		}
	}

	return false;
}

void KGCItemManager::FilterRemoveOtherChar(IN OUT std::vector<GCITEMUID>& vecItemList_, int iCharType)
{
	std::vector<GCITEMUID>::iterator vit;
	for (vit = vecItemList_.begin(); vit != vecItemList_.end(); ) {

		KItem* pInvenItem = GetInventoryItem(*vit);
		GCItem* fInvenItem = GetItemData(pInvenItem->m_ItemID);

		if (pInvenItem == NULL || !(fInvenItem->dwCharType & ItemCharTypeToItemFlag(iCharType))) {
			vit = vecItemList_.erase(vit);
		}
		else {
			++vit;
		}
	}
}

std::vector<GCITEMUID> KGCItemManager::GetInventoryItemUIDList(DWORD dwCharType_, DWORD dwItemPosition_, DWORD dwSkillPosition_)
{
	std::vector<GCITEMUID> vecItemList;

	const KItemContainer::ItemList& mapInventory = m_kInventory.GetFullItemList();
	KItemContainer::ItemList::const_iterator itor;
	vecItemList.reserve(m_kInventory.GetSize());

	for (itor = mapInventory.begin(); itor != mapInventory.end(); ++itor) {
		GCItem* pItem = GetItemData(itor->second.m_ItemID);

		if (pItem == NULL)
			continue;

		if (dwItemPosition_ != 0 && !(pItem->dwSlotPosition & dwItemPosition_))
			continue;

		if (dwSkillPosition_ != 0 && !(pItem->dwSkillPosition & dwSkillPosition_))
			continue;

		if (!(pItem->dwCharType & dwCharType_))
			continue;

		if (IsEquipDisableItem(GetUniqueChartype(dwCharType_), pItem->dwGoodsID)) {
			continue;
		}

		vecItemList.push_back(itor->second.m_ItemUID);
	}
	return vecItemList;
}

std::vector<GCITEMUID> KGCItemManager::GetInventoryItemUIDAllList(DWORD dwItemPosition_, DWORD dwSkillPosition_)
{
	std::vector<GCITEMUID> vecItemList;

	const KItemContainer::ItemList& mapInventory = m_kInventory.GetFullItemList();
	KItemContainer::ItemList::const_iterator itor;
	vecItemList.reserve(m_kInventory.GetSize());

	for (itor = mapInventory.begin(); itor != mapInventory.end(); ++itor) {
		GCItem* pItem = GetItemData(itor->second.m_ItemID);

		if (pItem == NULL)
			continue;

		if (dwItemPosition_ != 0 && !(pItem->dwSlotPosition & dwItemPosition_))
			continue;

		if (dwSkillPosition_ != 0 && !(pItem->dwSkillPosition & dwSkillPosition_))
			continue;

		vecItemList.push_back(itor->second.m_ItemUID);
	}
	return vecItemList;
}

std::vector<GCITEMUID> KGCItemManager::GetInventoryCoordiItemUIDList(DWORD dwCharType_, DWORD dwItemPosition_)
{
	std::vector<GCITEMUID> vecItemList;

	const KItemContainer::ItemList& mapInventory = m_kInventory.GetFullItemList();
	KItemContainer::ItemList::const_iterator itor;
	vecItemList.reserve(m_kInventory.GetSize());

	for (itor = mapInventory.begin(); itor != mapInventory.end(); ++itor) {
		if (itor->second.CheckItemType(KItem::TYPE_LOOK) == false)
			continue;

		GCItem* pItem = GetItemData(itor->second.m_ItemID);

		if (pItem == NULL)
			continue;

		if (dwItemPosition_ != 0 && !(pItem->dwSlotPosition & dwItemPosition_))
			continue;

		if (pItem != NULL && (!(pItem->dwCharType & g_pItemMgr->ItemCharTypeToItemFlag(-1)) && !(pItem->dwCharType & g_pItemMgr->ItemCharTypeToItemFlag(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType))))
			continue;

		std::wstring filterString_ = g_pkUIScene->m_pkMyInfoScene->m_strFilter;

		if (filterString_.size() > 0) {
			wstring strLowerItemName = pItem->strItemName;

			GCUTIL_STR::ToLower(filterString_);
			GCUTIL_STR::ToLower(strLowerItemName);

			if (strLowerItemName.find(filterString_) != std::wstring::npos)
				vecItemList.push_back(itor->second.m_ItemUID);
		}
		else
		{
			vecItemList.push_back(itor->second.m_ItemUID);
		}
	}
	return vecItemList;
}

std::vector<GCITEMUID> KGCItemManager::GetInventoryItemUIDListByKind(GCITEM_KIND eKind_)
{
	std::vector<GCITEMUID> vecItemList;

	const KItemContainer::ItemList& mapInventory = m_kInventory.GetFullItemList();
	KItemContainer::ItemList::const_iterator itor;
	vecItemList.reserve(m_kInventory.GetSize());

	for (itor = mapInventory.begin(); itor != mapInventory.end(); ++itor) {
		GCItem* pItem = GetItemData(itor->second.m_ItemID);

		if (pItem && pItem->eItemKind == eKind_)
		{
			vecItemList.push_back(itor->second.m_ItemUID);
		}
	}
	return vecItemList;
}

DWORD KGCItemManager::GetCharFlag(int iCharType_)
{
	DWORD charFlag = 1;
	charFlag <<= iCharType_;
	return charFlag;
}

void KGCItemManager::FilterBadPCBANGItemList(IN OUT std::vector<GCITEMUID>& vecItemList)
{
	vecItemList.erase(
		std::remove_if(
			vecItemList.begin(), vecItemList.end(),
			boost::bind(std::less_equal<GCITEMUID>(), _1, MAXIMUM_PCBANG_UID)),
		vecItemList.end()
	);

	std::vector<GCITEMUID>::iterator vit = vecItemList.begin();
	for (; vit != vecItemList.end(); ) {
		GCITEMUID itemUID = (*vit);
		KItem* pInvenItem = GetInventoryItem(itemUID);
		CONTINUE_NIL(pInvenItem);

		GCItem* pItemInfo = GetItemData(pInvenItem->m_ItemID);
		CONTINUE_NIL(pItemInfo);


		if (pItemInfo->eItemKind == GIK_MONSTER_CARD) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->eItemKind == GIK_COUPLE_ITEM) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->dwGoodsID == ITEM_ID_HEALBALL) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->dwGoodsID == ITEM_ID_HEALBALL2) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->dwGoodsID == ITEM_ID_HEALBALL_QUESTPACKAGE) {
			vit = vecItemList.erase(vit);
		}
		else {
			++vit;
		}
	}
}

void KGCItemManager::FilterBadCoordiItemList(std::vector<GCITEMUID>& vecItemList)
{
	vecItemList.erase(
		std::remove_if(
			vecItemList.begin(), vecItemList.end(),
			boost::bind(std::less_equal<GCITEMUID>(), _1, MAXIMUM_PCBANG_UID)),
		vecItemList.end()
	);

	std::vector<GCITEMUID>::iterator vit = vecItemList.begin();
	for (; vit != vecItemList.end(); ) {
		GCITEMUID itemUID = (*vit);
		KItem* pInvenItem = GetInventoryItem(itemUID);
		CONTINUE_NIL(pInvenItem);

		GCItem* pItemInfo = GetItemData(pInvenItem->m_ItemID);
		CONTINUE_NIL(pItemInfo);

		//  °­È­ ¾ÆÀÌÅÛÀº ÄÚµð·Î º¯È¯ ¸øÇÔ
		if (pInvenItem->m_EnchantEquipItemUID != 0) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->eItemKind == GIK_MONSTER_CARD) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->eItemKind == GIK_COUPLE_ITEM) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->dwGoodsID == ITEM_ID_HEALBALL) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->dwGoodsID == ITEM_ID_HEALBALL2) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->dwGoodsID == ITEM_ID_HEALBALL_QUESTPACKAGE) {
			vit = vecItemList.erase(vit);
		}
		else if (pItemInfo->eItemKind == GIK_ENCHANT_ASIST_ITEM) {
			vit = vecItemList.erase(vit);
		}
		else {
			++vit;
		}
	}
}

void KGCItemManager::FilterEquipedItem(const SCharInfo* pCharInfo_, std::vector<GCITEMUID>& vecItemList_, bool bCoordi_ /*= false*/)
{
	const KUserItemVector* pVecItem = NULL;
	if (bCoordi_) {
		pVecItem = &pCharInfo_->vecLookInfo;
	}
	else {
		pVecItem = &pCharInfo_->vecItemInfo;
	}

	for (DWORD i = 0; i < (*pVecItem).size(); ++i) {
		std::vector<GCITEMUID>::iterator vit = std::find(vecItemList_.begin(), vecItemList_.end(), (*pVecItem)[i].dwItemUID);
		if (vit == vecItemList_.end())
			continue;

		vecItemList_.erase(vit);
	}
}


std::pair<int, EMONEYTYPE> KGCItemManager::GetItemPrice(IN GCITEMUID itemUID_, IN int iDuration_ /*= 0*/)
{
	KItem* pkInvenItem = GetInventoryItem(itemUID_);
	if (pkInvenItem == NULL)
		return std::pair<int, EMONEYTYPE>(0, EMT_GAMEPOINT);

	GCItem* pkItem = GetItemData(pkInvenItem->m_ItemID);
	if (pkItem == NULL)
		return std::pair<int, EMONEYTYPE>(0, EMT_GAMEPOINT);

	// ÆÇ¸Å°¡ µÇÁö ¾Ê´Â ¾ÆÀÌÅÛµéÀº °¡°ÝÀÌ ¾ø´Â°É·Î ÇÕ´Ï´Ù.
	// ¼­¹ö¿¡¼­´Â °¡°ÝÀ» °è»êÇÏ´Ï±î ÆÈ¸®Áö ¾Êµµ·Ï Àß ÇØ¾ß ÇÕ´Ï´Ù.
	if (IsSellAvailableItem(itemUID_) == false)
		return std::pair<int, EMONEYTYPE>(0, EMT_GAMEPOINT);

	if (pkInvenItem->m_nPeriod == KItem::UNLIMITED_ITEM && pkInvenItem->m_nCount == KItem::UNLIMITED_ITEM) {
		switch (pkItem->eMoneyType) {
		case EMT_GEM:
			return std::pair<int, EMONEYTYPE>(pkItem->dwPrice / SELL_DIV_VALUE, EMT_GEM);
		case EMT_GAMEPOINT:
			return std::pair<int, EMONEYTYPE>(pkItem->dwPrice / SELL_GP_DIV_VALUE, EMT_GAMEPOINT);
		case EMT_CRYSTAL:
			return std::pair<int, EMONEYTYPE>(pkItem->dwPrice / SELL_DIV_VALUE, EMT_CRYSTAL);
		case EMT_CASH:
			return std::pair<int, EMONEYTYPE>(pkItem->dwPrice / GEM_PER_CASH_RATE, EMT_GEM);
		}
	}

	if (pkInvenItem->m_nCount != KItem::UNLIMITED_ITEM &&
		pkInvenItem->m_nCount != 0) {
		int iPrice = 0;
		EMONEYTYPE eMoneyType = EMT_GAMEPOINT;
		GetCountItemPrice(pkInvenItem->m_ItemID, iDuration_, iPrice, eMoneyType);
		return std::pair<int, EMONEYTYPE>(iPrice, eMoneyType);
	}

	return std::pair<int, EMONEYTYPE>(0, EMT_GAMEPOINT);
}

bool KGCItemManager::IsSellAvailableItem(IN GCITEMUID itemUID_)
{
	KItem* pkInvenItem = GetInventoryItem(itemUID_);
	if (pkInvenItem == NULL)
		return false;

	GCItem* pkItem = GetItemData(pkInvenItem->m_ItemID);
	if (pkItem == NULL)
		return false;

	if (pkInvenItem->m_ItemUID <= MAXIMUM_PCBANG_UID)
		return false;

	// ±â°£ÀÌ ´Ù µÈ ¾ÆÀÌÅÛÀº ÆÈ ¼ö ¾ø½À´Ï´Ù.
	if (pkInvenItem->IsExpired())
		return false;

	// ÆÇ¸Å°¡ µÇÁö ¾Ê´Â ¾ÆÀÌÅÛµéÀº °¡°ÝÀÌ ¾ø´Â°É·Î ÇÕ´Ï´Ù.
	// ¼­¹ö¿¡¼­´Â °¡°ÝÀ» °è»êÇÏ´Ï±î ÆÈ¸®Áö ¾Êµµ·Ï Àß ÇØ¾ß ÇÕ´Ï´Ù.
	if (pkItem->bNotSellItem)
		return false;

	if (CheckEquipItem(&g_kGlobalValue.m_kUserInfo, itemUID_))
		return false;

	// ±â°£Á¦·Î ¼ÂÆÃµÇ¾î ÀÖ´Âµ¥, ¼ö·®ÀÌ ÀÖ´Â ¾ÆÀÌÅÛÀº ÆÇ¸Å°¡ ¾ÈµË´Ï´Ù.
	if (pkItem->eItemType == GCIT_PERIOD && pkInvenItem->m_nCount != -1)
		return false;

	if (IsRemoveAvailableItem(itemUID_))
		return false;

	return true;
}

bool KGCItemManager::IsRemoveAvailableItem(IN GCITEMUID itemUID_)
{
	KItem* pkInvenItem = GetInventoryItem(itemUID_);
	if (pkInvenItem == NULL)
		return false;

	GCItem* pkItem = GetItemData(pkInvenItem->m_ItemID);
	if (pkItem == NULL)
		return false;

	if (pkInvenItem->m_ItemUID <= MAXIMUM_PCBANG_UID)
		return false;

#ifdef ENABLE_POINT_SYSTEM // Kawan>
	// Æ÷ÀÎÆ® ?¾ÆÀÌÅÛÀº ÆÇ¸Å µÇÁö ¾Ê½À´Ï´Ù.
	if (IsPointShopItem(pkInvenItem->m_ItemID))
		return true;
#endif

	// ±â°£Á¦ ¾ÆÀÌÅÛÀº ÆÇ¸ÅµÇÁö ¾Ê°í Á¦°Å µË´Ï´Ù.
	if (pkInvenItem->m_nPeriod != KItem::UNLIMITED_ITEM)
		return true;

	// ±â°£Á¦·Î ¼ÂÆÃµÇ¾î ÀÖ´Âµ¥, ¼ö·®ÀÌ ÀÖ´Â ¾ÆÀÌÅÛÀº Á¦°Å ÇØ¾ß µË´Ï´Ù.
	if (pkItem->eItemType == GCIT_PERIOD && pkInvenItem->m_nCount != -1)
		return true;

	if (CheckEquipItem(&g_kGlobalValue.m_kUserInfo, itemUID_))
		return false;

	if (pkInvenItem->m_ItemID == KGCItemManager::ITEM_HALLOWEEN_BOARD ||
		pkInvenItem->m_ItemID == KGCItemManager::ITEM_BINGO_BOARD)
		return false;

	return false;
}

void KGCItemManager::SellItem(IN const std::vector<KItem>& vecItemList)
{
	KEVENT_BUNDLE_SELL_ITEM_REQ kPacket;
	std::vector<KItem>::const_iterator vit = vecItemList.begin();
	for (; vit != vecItemList.end(); ++vit) {
		if (g_pItemMgr->IsRemoveAvailableItem((*vit).m_ItemUID)) {
			kPacket.m_vecEraseList.push_back((*vit).m_ItemUID);
		}
		else if (g_pItemMgr->IsSellAvailableItem((*vit).m_ItemUID)) {
			if ((*vit).m_nCount > 0) {
				kPacket.m_vecSellCountItem.push_back(std::pair<GCITEMUID, int>((*vit).m_ItemUID, (*vit).m_nCount));
			}
			else {
				kPacket.m_vecSellNormalItem.push_back((*vit).m_ItemUID);
			}
		}
	}
	g_kGlobalValue.isUndoCoordi = false;
	KP2P::GetInstance()->Send_BundleSellReq(kPacket);
}

void KGCItemManager::AddSellItemToList(IN const KItem& kItem_)
{
	for (DWORD i = 0; i < m_vecSellList.size(); ++i) {
		if (m_vecSellList[i].m_ItemUID == kItem_.m_ItemUID) {
			m_vecSellList[i] = kItem_;
			return;
		}
	}
	m_vecSellList.push_back(kItem_);
}

void KGCItemManager::RemoveSellItemList(IN GCITEMUID itemUID_)
{
	for (DWORD i = 0; i < m_vecSellList.size(); ++i) {
		if (m_vecSellList[i].m_ItemUID == itemUID_) {
			m_vecSellList.erase(m_vecSellList.begin() + i);
			break;
		}
	}
}

void KGCItemManager::SellItemFromList()
{
	SellItem(m_vecSellList);
	ClearSellItemList();
}

void KGCItemManager::AddSellPriceInfo(IN GCITEMID itemID_, IN const KSellInfo& kInfo_)
{
	m_mapSellPrice[itemID_ / 10] = kInfo_;
}

bool KGCItemManager::GetSellPriceInfo(IN GCITEMID itemID_, OUT KSellInfo& kInfo_)
{
	std::map<GCITEMID, KSellInfo>::iterator mit;
	mit = m_mapSellPrice.find(itemID_);

	if (mit == m_mapSellPrice.end())
		return false;

	kInfo_ = mit->second;
	return true;
}

bool KGCItemManager::GetCountItemPrice(IN GCITEMID itemID_, IN int iCount_, OUT int& iPrice_, OUT EMONEYTYPE& eMoneyType_)
{
	iPrice_ = 0;
	eMoneyType_ = EMT_GAMEPOINT;

	KSellInfo kSellInfo;
	if (GetSellPriceInfo(itemID_, kSellInfo) == false) {
		Result_SellItemInfo = INT_MAX;
		KP2P::GetInstance()->Send_GetSellPrice(itemID_);
		g_MyD3D->WaitForServerAck(Result_SellItemInfo, INT_MAX, 10000, TIME_OUT_VALUE);

		if (GetSellPriceInfo(itemID_, kSellInfo) == false)
			return false;
	}

	switch (kSellInfo.m_cSellMoneyType) {
	case EMTS_GAMEPOINT:
		eMoneyType_ = EMT_GAMEPOINT;
		break;
	case EMTS_GEM:
		eMoneyType_ = EMT_GEM;
		break;
	case EMTS_CRYSTAL:
		eMoneyType_ = EMT_CRYSTAL;
		break;
	case EMTS_CASH:
		eMoneyType_ = EMT_CASH;
		break;
	default:
		eMoneyType_ = EMT_GAMEPOINT;
		break;
	}
	//DB¶û ¿ÏÀüÇÑ µ¿±âÈ­ÀÓ µðºñ¿¡¼­ ÀÌ·¸°Ô °è»êÇÔ...!!!! 
	int iPrice = kSellInfo.m_nBuyPrice / kSellInfo.m_nDivideFactor; // ´Ü°¡

	int	iFactor = kSellInfo.m_nBuyCount;

	float fPrice = 0.0f;
	float fFactor = 0.0f;

	if (iFactor >= iPrice)
	{
		if (iFactor == 0 || iPrice == 0) {
			fPrice = 0.0f;
			fFactor = 0.0f;
		}
		else {
			fPrice = 1.0f;
			fFactor = static_cast<float>(static_cast<int>(iFactor / iPrice));
		}
	}
	else
	{
		if (iFactor == 0 || iPrice == 0) {
			fPrice = 0.0f;
			fFactor = 0.0f;
		}
		else {
			fPrice = static_cast<float>(static_cast<int>(iPrice / iFactor));
			fFactor = 1.0f;
		}
	}

	if (fFactor == 0.0f || fPrice == 0.0f || iCount_ == 0) {
		iPrice_ = 0;
	}
	else
	{
		iPrice_ = static_cast<int>(static_cast<float>(iCount_) * fPrice / fFactor);
	}

	return true;
}

bool KGCItemManager::IsContainSellItemList(IN GCITEMUID itemUID_)
{
	for (DWORD i = 0; i < m_vecSellList.size(); ++i) {
		if (m_vecSellList[i].m_ItemUID == itemUID_) {
			return true;
		}
	}
	return false;
}

std::wstring KGCItemManager::GetItemNameUID(GCITEMUID dwUID_)
{
	if (GetInventoryItem(dwUID_) == NULL) return L"";
	return GetItemData(GetInventoryItem(dwUID_)->m_ItemID)->strItemName;

}

void KGCItemManager::FilterRemoveCoordiItem(IN OUT std::vector<GCITEMUID>& vecItemList_)
{
	std::vector<GCITEMUID>::iterator vit;
	for (vit = vecItemList_.begin(); vit != vecItemList_.end(); ) {
		KItem* pInvenItem = GetInventoryItem(*vit);

		if (pInvenItem == NULL || pInvenItem->CheckItemType(KItem::TYPE_LOOK)) {
			vit = vecItemList_.erase(vit);
		}
		else {
			++vit;
		}
	}
}

void KGCItemManager::FilterRemainStuffOnly(IN OUT std::vector<GCITEMUID>& vecItemList_)
{
	std::vector<GCITEMUID>::iterator vit;
	for (vit = vecItemList_.begin(); vit != vecItemList_.end(); ) {
		KItem* pInvenItem = GetInventoryItem(*vit);
		if (pInvenItem == NULL) {
			vit = vecItemList_.erase(vit);
			continue;
		}

		GCItem* pItemInfo = GetItemData(pInvenItem->m_ItemID);
		if (pItemInfo == NULL) {
			vit = vecItemList_.erase(vit);
			continue;
		}

		if (pItemInfo->eItemKind == GIK_ENCHANT_ASIST_ITEM ||
			pItemInfo->eItemKind == GIK_ENCHANT_CORE ||
			pItemInfo->eItemKind == GIK_STUFF ||
			pItemInfo->eItemKind == GIK_MISSION_SCROLL ||
			pItemInfo->eItemKind == GIK_TREASUREBOX_ITEM || pItemInfo->eItemKind == GIK_COORDIBOX) {
			++vit;
		}
		else {
			vit = vecItemList_.erase(vit);
		}
	}
}

void KGCItemManager::addStatusBonus(EAbilityData& stAbilityData_, float bonus)
{
	stAbilityData_.fEquipItemAbility[ABILITY_HP] += bonus;
	stAbilityData_.fEquipItemAbility[ABILITY_ATK] += bonus;
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] += bonus;
}

void KGCItemManager::addCriticalBonus(EAbilityData& stAbilityData_, float bonus)
{
	stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] += bonus;
}

void KGCItemManager::SetPromotionStatus(EAbilityData& stAbilityData_, int iPromotion_, PLAYER* pPlayer_)
{
	float fStatusBonus = 0.0f;
	float fCriticalBonus = 0.0f;

	for (int i = 1; i <= iPromotion_; ++i)
	{
		switch (i)
		{
		case 1:
			fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
			break;

		case 2:
			fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
			fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
			break;

		case 3:
			fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
			fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
			break;

		default:
			break;
		}
	}

	if (pPlayer_ != NULL)
	{
		int charType = pPlayer_->GetCurrentChar().iCharType;
		int charLevel = pPlayer_->GetCurrentChar().iLevel;
		bool isLocalPlayer = pPlayer_->IsLocalPlayer();

		if (charType == GC_CHAR_ASIN)
		{
			if (isLocalPlayer)
			{
				if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_ABILITY_UP))
				{
					if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_3) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL3))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical);
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_2) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL2))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical);
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_1) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL1))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
					}
				}
				else
				{
					if (SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL3))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical);
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL2))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical);
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL1))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
					}
				}
			}
			else
			{
				if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_ABILITY_UP))
				{
					if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_SKILL_3) || pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL3))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical);
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_SKILL_2) || pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL2))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical);
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_SKILL_1) || pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL1))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
					}
				}
				else
				{
					if (pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL3))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical);
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL2))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
						addCriticalBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical);
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL1))
					{
						addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
					}
				}
			}
		}
		else if (charType == GC_CHAR_LIME)
		{
			if (isLocalPlayer && SiGCSKT()->IsLearnedSkill(SID_LIME_NORMAL_ABILITY_UP))
			{
				if (charLevel >= 60) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
				if (charLevel >= 40) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
				if (charLevel >= 20) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
			}
			else if (pPlayer_->IsTrainedSkill(SID_LIME_NORMAL_ABILITY_UP))
			{
				if (charLevel >= 60) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
				if (charLevel >= 40) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
				if (charLevel >= 20) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
			}
		}
		else if (charType == GC_CHAR_UNO)
		{
			if (isLocalPlayer && SiGCSKT()->IsLearnedSkill(SID_UNO_UP_STATS))
			{
				if (charLevel >= 60) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
				if (charLevel >= 40) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
				if (charLevel >= 20) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
			}
			else if (pPlayer_->IsTrainedSkill(SID_UNO_UP_STATS))
			{
				if (charLevel >= 60) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status);
				if (charLevel >= 40) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status);
				if (charLevel >= 20) addStatusBonus(stAbilityData_, g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status);
			}
		}
	}

	addStatusBonus(stAbilityData_, fStatusBonus);
	addCriticalBonus(stAbilityData_, fCriticalBonus);
}

/*void KGCItemManager::SetPromotionStatus(EAbilityData& stAbilityData_, int iPromotion_, PLAYER* pPlayer_)
{
	float fStatusBonus = 0.0f;
	float fCriticalBonus = 0.0f;

	for (int i = 0; i <= iPromotion_; ++i)
	{
		if (pPlayer_ != NULL) {
#ifdef DECANEE
			if ( pPlayer_->GetCurrentChar().iCharType == GC_CHAR_ASIN || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_LIME || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_DECANEE ) {
#else
			if (pPlayer_->GetCurrentChar().iCharType == GC_CHAR_ASIN || pPlayer_->GetCurrentChar().iCharType == GC_CHAR_LIME ) {
#endif
				break;
			}
		}

		switch (i)
		{
		case 1:
			fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
			break;

		case 2:
			fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
			fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
			break;

		case 3:
			fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
			fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
			break;

		default:
			break;
		}
	}

	// Á¤¸®ÇØº¸µµ·Ï ÇÏ°ÚÀ½...............
	if (pPlayer_ != NULL) {
		if (pPlayer_->GetCurrentChar().iCharType == GC_CHAR_ASIN) {
			if (pPlayer_->IsLocalPlayer()) {
				if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_ABILITY_UP)) {
					if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_3) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL3)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_2) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL2)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN_HIDDEN_SKILL_1) || SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL1)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
					}
				}
				else {
					if (SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL3)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL2)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
					}

					if (SiGCSKT()->IsLearnedSkill(SID_ASIN2_SPECIAL1)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
					}
				}
			}
			else {
				if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_ABILITY_UP)) {
					if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_SKILL_3) || pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL3)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_SKILL_2) || pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL2)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN_HIDDEN_SKILL_1) || pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL1)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
					}
				}
				else {
					if (pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL3)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL2)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
					}

					if (pPlayer_->IsTrainedSkill(SID_ASIN2_SPECIAL1)) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
					}
				}
			}
		}
		else if (pPlayer_->GetCurrentChar().iCharType == GC_CHAR_LIME) {
			if (pPlayer_->IsLocalPlayer()) {
				if (SiGCSKT()->IsLearnedSkill(SID_LIME_NORMAL_ABILITY_UP)) {
					if (pPlayer_->GetCurrentChar().iLevel >= 60) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
					}

					if (pPlayer_->GetCurrentChar().iLevel >= 40) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
					}

					if (pPlayer_->GetCurrentChar().iLevel >= 20) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
					}
				}
			}
			else {
				if (pPlayer_->IsTrainedSkill(SID_LIME_NORMAL_ABILITY_UP)) {
					if (pPlayer_->GetCurrentChar().iLevel >= 60) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion3Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion3Critical;
					}

					if (pPlayer_->GetCurrentChar().iLevel >= 40) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion2Status;
						fCriticalBonus += g_kGlobalValue.m_kPromotionBonus.m_fPromotion2Critical;
					}

					if (pPlayer_->GetCurrentChar().iLevel >= 20) {
						fStatusBonus += g_kGlobalValue.m_kPromotionBonus.m_iPromotion1Status;
					}
				}
			}
		}
	}

	stAbilityData_.fEquipItemAbility[ABILITY_HP] += fStatusBonus;
	stAbilityData_.fEquipItemAbility[ABILITY_ATK] += fStatusBonus;
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] += fStatusBonus;
	stAbilityData_.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] += fCriticalBonus;
}*/

void KGCItemManager::LoadEventChangeItemList()
{
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "EventChangeItemList.lua") == false)
	{
		MessageBoxA(NULL, "EventChangeItemList.lua Load Failed!", "", MB_OK);
		return;
	}

	m_mapEventItemChange.clear();

	luaMgr.BeginTable("EventChangeItemList");
	{
		int iItemIndex = 1;
		while (true)
		{
			if (FAILED(luaMgr.BeginTable(iItemIndex))) break;
			{
				DWORD dwOriginalItem;
				EVENTITEMCHANGE eventItemInfo;

				luaMgr.GetValue("originalItem", dwOriginalItem);
				luaMgr.GetValue("changeItem", eventItemInfo.dwChangeItem);


				int nTempSec = 0;

				if (SUCCEEDED(luaMgr.BeginTable("startTime")))
				{
					int nYear, nMonth, nDay, nHour, nMin;

					luaMgr.GetValue(1, nYear);
					luaMgr.GetValue(2, nMonth);
					luaMgr.GetValue(3, nDay);
					luaMgr.GetValue(4, nHour);
					luaMgr.GetValue(5, nMin);

					CTime time(nYear, nMonth, nDay, nHour, nMin, nTempSec);
					eventItemInfo.timeStart = time;

					luaMgr.EndTable(); // "startTime"
				}
				if (SUCCEEDED(luaMgr.BeginTable("endTime")))
				{
					int nYear, nMonth, nDay, nHour, nMin;

					luaMgr.GetValue(1, nYear);
					luaMgr.GetValue(2, nMonth);
					luaMgr.GetValue(3, nDay);
					luaMgr.GetValue(4, nHour);
					luaMgr.GetValue(5, nMin);

					CTime time(nYear, nMonth, nDay, nHour, nMin, nTempSec);
					eventItemInfo.timeEnd = time;

					luaMgr.EndTable(); // "endTime"
				}
				m_mapEventItemChange.insert(std::make_pair(dwOriginalItem, eventItemInfo));

			}
			luaMgr.EndTable();

			++iItemIndex;
		}
	}
	luaMgr.EndTable();
}

void KGCItemManager::LoadEventViewItemList()
{
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "EventViewItemList.lua") == false)
	{
		MessageBoxA(NULL, "EventViewItemList.lua Load Failed!", "", MB_OK);
		return;
	}

	m_vecEventItemView.clear();

	luaMgr.BeginTable("EventViewItemList");
	{
		int iItemIndex = 1;
		while (true)
		{
			if (FAILED(luaMgr.BeginTable(iItemIndex))) break;
			{
				EVENTITEMVIEW eventItemInfo;

				luaMgr.GetValue("viewItem", eventItemInfo.dwViewItem);


				const int nTempSec = 0;
				if (SUCCEEDED(luaMgr.BeginTable("startTime")))
				{
					int nYear, nMonth, nDay, nHour, nMin;

					luaMgr.GetValue(1, nYear);
					luaMgr.GetValue(2, nMonth);
					luaMgr.GetValue(3, nDay);
					luaMgr.GetValue(4, nHour);
					luaMgr.GetValue(5, nMin);

					CTime time(nYear, nMonth, nDay, nHour, nMin, nTempSec);
					eventItemInfo.timeStart = time;

					luaMgr.EndTable(); // "startTime"
				}
				if (SUCCEEDED(luaMgr.BeginTable("endTime")))
				{
					int nYear, nMonth, nDay, nHour, nMin;

					luaMgr.GetValue(1, nYear);
					luaMgr.GetValue(2, nMonth);
					luaMgr.GetValue(3, nDay);
					luaMgr.GetValue(4, nHour);
					luaMgr.GetValue(5, nMin);

					CTime time(nYear, nMonth, nDay, nHour, nMin, nTempSec);
					eventItemInfo.timeEnd = time;

					luaMgr.EndTable(); // "endTime"
				}
				m_vecEventItemView.push_back(eventItemInfo);

			}
			luaMgr.EndTable();

			++iItemIndex;
		}
	}
	luaMgr.EndTable();
}


bool KGCItemManager::isEventChangeItem(DWORD _dwItemID)
{
	std::map< DWORD, EVENTITEMCHANGE>::iterator itor;
	itor = m_mapEventItemChange.find(_dwItemID);
	if (itor == m_mapEventItemChange.end()) return false;
	return true;
}


bool KGCItemManager::CheckAndChangeEventItem(GCItem*& pItem)
{
	if (g_pItemMgr->isEventChangeItem(pItem->dwGoodsID)) {

		EVENTITEMCHANGE& changeItem = m_mapEventItemChange[pItem->dwGoodsID];

		CTime timeStart(changeItem.timeStart);
		CTime timeEnd(changeItem.timeEnd);
		CTime timeCur(g_kGlobalValue.m_tmServerTime);
		if (timeStart < timeCur && timeCur < timeEnd) {
			pItem = m_mapDB[changeItem.dwChangeItem];
			m_setApplyRefreshEventTimeChange.insert(changeItem.timeEnd);
		}

		if (timeStart > timeCur) {
			m_setApplyRefreshEventTimeChange.insert(changeItem.timeStart);
		}

		return true;
	}
	return false;
}

void KGCItemManager::LoadHeroItemSpecialAbility(void)
{
	m_vecHeroItemAbility.clear();
	m_mapHeroItemBuffGroup.clear();
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "SetHeroItemAbility.lua") == false)
		return;

	if (luaMgr.BeginTable("HeroItemBuffGroup") == S_OK)
	{
		for (int j = 1; ; j++)
		{
			LUA_BEGIN_TABLE(j, break)
			{
				int nBuffIndex = 0;
				int nBuffGroup = -1;
				LUA_GET_VALUE(1, nBuffIndex, return);
				LUA_GET_VALUE(2, nBuffGroup, return);

				m_mapHeroItemBuffGroup.insert(std::make_pair(nBuffIndex, nBuffGroup));
			}
			LUA_END_TABLE(return)
		}
		luaMgr.EndTable();
	}


	if (luaMgr.BeginTable("HeroItemSpecialAbility") == S_OK)
	{
		for (int i = 1; luaMgr.BeginTable(i) == S_OK; ++i)
		{
			KHeroItemSpecialAbility kHeroItemAbility;

			DWORD dwGoodsID = -1;
			if (luaMgr.GetValue("GoodsID", dwGoodsID) == S_OK)
				kHeroItemAbility.dwGoodsID = dwGoodsID;

			int iBuffIndex = -1;
			if (luaMgr.GetValue("BuffIndex", iBuffIndex) == S_OK)
				kHeroItemAbility.iBuffIndex = iBuffIndex;

			float fBuffTime = 0.f;
			if (luaMgr.GetValue("BuffTime", fBuffTime) == S_OK)
				kHeroItemAbility.fBuffTime = fBuffTime;

			int nPriority = 0;
			if (luaMgr.GetValue("Priority", nPriority) == S_OK)
				kHeroItemAbility.nPriority = nPriority;

			if (luaMgr.BeginTable("Conditions") == S_OK)
			{
				for (int j = 1; ; ++j)
				{
					float fCondition;
					if (luaMgr.GetValue(j, fCondition) == E_FAIL)
						break;
					kHeroItemAbility.vecConditions.push_back(fCondition);
				}
				luaMgr.EndTable();
			}

			m_vecHeroItemAbility.push_back(kHeroItemAbility);

			luaMgr.EndTable();
		}
		luaMgr.EndTable();
	}
}

bool KGCItemManager::GetHeroItemCondition(GCITEMID dwItemID_, std::vector<float>* pOutVector)
{
	if (!pOutVector)
		return false;

	pOutVector->clear();

	std::vector<KHeroItemSpecialAbility>::iterator vit = std::find_if(m_vecHeroItemAbility.begin(), m_vecHeroItemAbility.end(),
		boost::bind(&KHeroItemSpecialAbility::dwGoodsID, _1) == dwItemID_);

	if (vit == m_vecHeroItemAbility.end())
		return false;

	*pOutVector = vit->vecConditions;

	return true;
}

int  KGCItemManager::GetHeroItemBuffGroup(int nBuffIndex)
{
	std::map< int, int >::iterator pos = m_mapHeroItemBuffGroup.find(nBuffIndex);
	if (pos == m_mapHeroItemBuffGroup.end())
		return -1;
	else
		return pos->second;
}

void KGCItemManager::InitHeroItemSpecialAbility(int iPlayerIndex_)
{
	if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return;

	g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.clear();
}

void KGCItemManager::SettingHeroItemSpecialAbility(GCITEMID dwItemID_, int iPlayerIndex_)
{
	if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return;

	std::vector<KHeroItemSpecialAbility>::iterator vit = std::find_if(m_vecHeroItemAbility.begin(), m_vecHeroItemAbility.end(),
		boost::bind(&KHeroItemSpecialAbility::dwGoodsID, _1) == dwItemID_);

	if (vit != m_vecHeroItemAbility.end())
	{
		MAP_HEROITEM_ITER pos = g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.begin();
		MAP_HEROITEM_ITER pos_end = g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.end();

		bool bPriority = false;

		for (; pos != pos_end; ++pos)
		{
			GCITEMID dwFindID = pos->second;

			std::vector<KHeroItemSpecialAbility>::iterator vit2 = std::find_if(m_vecHeroItemAbility.begin(), m_vecHeroItemAbility.end(),
				boost::bind(&KHeroItemSpecialAbility::dwGoodsID, _1) == dwFindID);

			if (vit2 == m_vecHeroItemAbility.end())
				continue;

			int nBuffGroup1 = GetHeroItemBuffGroup(vit->iBuffIndex);
			int nBuffGroup2 = GetHeroItemBuffGroup(vit2->iBuffIndex);

			//  ¹öÇÁ°¡ °°°Å³ª ¹öÇÁ ±×·ìÀÌ °°À»¶§.... 
			if (vit->iBuffIndex == vit2->iBuffIndex || (nBuffGroup1 == nBuffGroup2 && nBuffGroup1 != -1))
			{
				//  ¿ì¼± ¼øÀ§°¡ ³·À¸¸é Áö¿î´Ù.
				if (vit->nPriority > vit2->nPriority)
				{
					g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, vit2->iBuffIndex);

					g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.erase(pos);
				}
				//  ³ôÀ¸¸é ³»¹ö·Á µÎ°í
				else
				{
					bPriority = true;
				}

				break;
			}
		}

		//  ¿ì¼± ¼øÀ§ ³ôÀº ¾ÆÀÌ°¡ ¾øÀ¸¸é ³Ö¾îÁØ´Ù.
		if (!bPriority)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_mapHeroItemCondition.insert(std::make_pair(vit->iBuffIndex, vit->dwGoodsID));
			g_pMagicEffect->SetMagicEffect(iPlayerIndex_, vit->iBuffIndex, vit->fBuffTime);
		}
	}
}

void KGCItemManager::Server_SetNonCountItem(std::set<DWORD>& setItemList)
{
	std::set<DWORD>::iterator sit;
	for (sit = setItemList.begin(); sit != setItemList.end(); ++sit)
		m_setNonCountItemList.insert(*sit / 10);
}

void KGCItemManager::Server_SetNonVisibleItem(std::set<DWORD>& setItemList)
{
	m_setNonVisibleItemList.clear();

	std::set<DWORD>::iterator sit;
	for (sit = setItemList.begin(); sit != setItemList.end(); ++sit)
		m_setNonVisibleItemList.insert(*sit / 10);
}

bool KGCItemManager::IsNonCountItem(DWORD dwItemID)
{
	std::set<DWORD>::iterator sit = m_setNonCountItemList.find(dwItemID);
	return (m_setNonCountItemList.end() != sit);
}

bool KGCItemManager::IsNonVisibleItem(DWORD dwItemID)
{
	std::set<DWORD>::iterator sit = m_setNonVisibleItemList.find(dwItemID);
	return (m_setNonVisibleItemList.end() != sit);
}

bool KGCItemManager::CheckRandomInitItemtoInventory(DWORD& dwItemID, int iGrade)
{
	if (m_mapRandomInitAttributeItemList.empty()) return false;

	MAP_DWORD_SET::iterator mit = m_mapRandomInitAttributeItemList.find(iGrade);
	if (mit == m_mapRandomInitAttributeItemList.end()) return false;


	SET_DWORD::iterator sit = mit->second.begin();

	for (; sit != mit->second.end(); sit++)
	{
		if (FindInventoryForItemID((*sit) / 10)) {
			dwItemID = (*sit) / 10;
			return true;
		}
	}
	return false;
}

bool KGCItemManager::IsCashItem(DWORD dwItemID)
{
	GCItem* kItem = GetItemData(dwItemID);

	if (kItem == NULL) return false;

	return kItem->eMoneyType == EMT_CASH;
}

bool KGCItemManager::GetAttributeTable(IN int iTableID, IN int iGrade, OUT KGCAttributeTable& kTable_)
{
	if (iTableID == -1)
		return false;

	return m_kAttributeDecider.GetTable(iTableID, iGrade, kTable_);
}

int KGCItemManager::GetlItemEquipLevel(GCItem* pItem_, KItem* pInven_, SUserItem* pUserItem_, bool bLevelDown)
{
	// bLevelDown = ·¹º§´Ù¿î ¼öÄ¡µµ °¡Á®¿ÀÀÚ.
	int nEquipLevelDown = 0;

	if (bLevelDown)
	{
		if (pInven_ && pInven_->m_sEquipLvDown)
		{
			nEquipLevelDown = pInven_->m_sEquipLvDown;
		}
	}

	int nEquipLevel = 0;

	if (pItem_->eItemKind == GIK_SELECT_LEVEL_ITEM && pInven_ != NULL)
	{
		nEquipLevel = static_cast<int>(pInven_->m_sEquipLevel);
	}
	else
	{
		if (pUserItem_ != NULL && pUserItem_->nEquipLevel > 0)
			nEquipLevel = pUserItem_->nEquipLevel;
		else
			nEquipLevel = pItem_->iNeedLevelForEquippingItem;
	}

	nEquipLevel -= nEquipLevelDown;

	if (nEquipLevel < 0)
		nEquipLevel = 0;

	return nEquipLevel;
}



int KGCItemManager::GetlItemAbilityLimitLevel(GCItem* pItem_, KItem* pInven_)
{
	if (pItem_->eItemKind == GIK_SELECT_LEVEL_ITEM && pInven_ != NULL)
	{
		return static_cast<int>(pInven_->m_sEquipLevel);
	}
	else
	{
		return pItem_->iLimitedLevelOfLevelRatioAbility;
	}
}

void KGCItemManager::LoadGachaList()
{
	m_mapGachaList.clear();
	KLuaManager     luaMgr;

	if (false == GCFUNC::LoadLuaScript(luaMgr, "GachaItemList.stg"))
		return;

	LUA_BEGIN_TABLE("GachaList", return)
	{
		for (int i = 1; ; ++i)
		{
			DWORD dwGoodsID = 0;
			int iCharID = 0;
			LUA_BEGIN_TABLE(i, break)
				LUA_GET_VALUE_NOASSERT(1, dwGoodsID, break)
				LUA_GET_VALUE_NOASSERT(2, iCharID, break)
				m_mapGachaList.insert(std::pair<DWORD, int>(dwGoodsID, iCharID));
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(;)
}

bool KGCItemManager::IsExpItemList(GCITEMID dwGoodsID)
{
	bool bResult = false;
	std::vector<GCITEMID>::iterator vecIter = std::find(m_vecExpItemList.begin(), m_vecExpItemList.end(), (dwGoodsID * 10));
	bResult = (vecIter != m_vecExpItemList.end()) ? true : false;
	return bResult;
}

void KGCItemManager::SetExpItemList(std::vector<GCITEMID>& vecList)
{
	m_vecExpItemList.clear();
	m_vecExpItemList = vecList;
}

#ifdef USE_COORDI_SHOP // Kawan>
void KGCItemManager::MakeCoordiShopItemList(EGCCharType eCharType_, int iItemType_, int iCategoly_, int iSortType_, const WCHAR* strName_)
{
	//ÀüÃ¼ ¾ÆÀÌÅÛ Áß¿¡ À§ Á¶°Ç¿¡ ¸Â´Â°ÍÀÇ ÀÎµ¦½º¸¦ º¤ÅÍ¿¡ ´ã´Â´Ù.
	DWORD dwCategory = 0;

	switch (iItemType_)
	{
	case SIT_ARMOR:
	{
		dwCategory = ESP_HELMET // ¸Ó¸®
			| ESP_JACKET // »óÀÇ
			| ESP_PANTS // ÇÏÀÇ
			| ESP_GLOVES // Àå°©
			| ESP_SHOES // ½Å¹ß
			| ESP_MANTLE; // ¸ÁÅä
		break;
	}
	case SIT_ACCESSORY:
	{
		dwCategory = ESP_A_UPPER_HELMET
			| ESP_A_DOWN_HELMET
			| ESP_A_NECKLET
			| ESP_A_JACKET
			| ESP_A_PANTS
			| ESP_A_ARM
			| ESP_A_FINGER
			| ESP_A_SHOES
			| ESP_SLOT_ITEM
			| ESP_SLOT_EMOT;
		break;
	}
	case SIT_QUEST: {            dwCategory = 0;            break;        }
	}

	DWORD charFlag = GetCharFlag(eCharType_);

	m_vecCoordishopItemList.clear();
	std::wstring strName;
	if (strName_ != NULL)
		strName = strName_;

	if (iItemType_ == SIT_QUEST) {
		std::map< GCITEMID, GCItem* >::iterator mit;
		for (mit = m_mapCoordishopRandomBox.begin(); mit != m_mapCoordishopRandomBox.end(); ++mit) {
			GCItem* pItem = mit->second;
			CONTINUE_NIL(pItem);

			if (!(pItem->dwCharType & charFlag))
				continue;

			if (pItem->bNotBuyItem)
				continue;

			m_vecCoordishopItemList.push_back(std::pair<GCITEMID, GCITEMID>(pItem->dwGoodsID, 0));
		}
	}
	else {
		std::map< int, KCoordiShopItem >::iterator mit;
		for (mit = m_mapCoordishopItem.begin(); mit != m_mapCoordishopItem.end(); ++mit) {
			int iSetNum = mit->first;
			GCItem* pItem = mit->second.pOriginal;
			if (pItem == NULL)
				pItem = mit->second.pPackage7;
			CONTINUE_NIL(pItem);

			if (!(pItem->dwCharType & charFlag))
				continue;
			if (SIT_SET != iItemType_ && (pItem->dwSlotPosition & ESP_SET_ITEM))
				continue;

			if (!(pItem->dwSlotPosition & dwCategory))
				continue;

			if (strName_ != NULL) {
				transform(strName.begin(), strName.end(), strName.begin(), tolower);

				std::wstring strItem = pItem->strItemName;

				transform(strItem.begin(), strItem.end(), strItem.begin(), tolower);
				if (strItem.find(strName) == std::wstring::npos)
					continue;
			}

			GCITEMID dwOriginal = 0;
			GCITEMID dwPackage7 = 0;
			if (mit->second.pOriginal) {
				if (!mit->second.pOriginal->bNotBuyItem)
					dwOriginal = mit->second.pOriginal->dwGoodsID;
			}

			if (mit->second.pPackage7) {
				if (!mit->second.pPackage7->bNotBuyItem)
					dwPackage7 = mit->second.pPackage7->dwGoodsID;
			}
			if (dwPackage7 != 0 || dwOriginal != 0)
				m_vecCoordishopItemList.push_back(std::pair<GCITEMID, GCITEMID>(dwOriginal, dwPackage7));
		}
	}
	std::vector< GCITEMID > vecItemList;

	// 7ÀÏ°ú ¿µ±¸¸¦ ÇÏ³ª·Î ¸¸µé¾î¼­ sortÇÑ´Ù.
	for (std::vector<std::pair<GCITEMID, GCITEMID>>::iterator vIter = m_vecCoordishopItemList.begin(); vIter != m_vecCoordishopItemList.end(); vIter++) {
		if (vIter->first != 0)
			vecItemList.push_back(vIter->first);

		if (vIter->second != 0)
			vecItemList.push_back(vIter->second);
	}

	CategoryGrouping(vecItemList, iItemType_, iCategoly_);
	SortTypeGrouping(vecItemList, iSortType_);


	std::vector<std::pair<GCITEMID, GCITEMID>> vecCoordishopItemList;

	// ¼ÒÆ®µÈ ¾ÆÀÌÅÛµéÀ» ´Ù½Ã 7ÀÏ°ú ¿µ±¸·Î ÇÕÄ£´Ù.
	while (!vecItemList.empty())
	{
		GCITEMID itemID = *vecItemList.begin();

		std::vector<std::pair<GCITEMID, GCITEMID>>::iterator pos = m_vecCoordishopItemList.begin();
		std::vector<std::pair<GCITEMID, GCITEMID>>::iterator end = m_vecCoordishopItemList.end();


		GCITEMID itemIDFirst = 0;
		GCITEMID itemIDSecond = 0;

		for (; pos != end; ++pos)
		{
			if (itemID == pos->first || itemID == pos->second)
			{
				itemIDFirst = pos->first;
				itemIDSecond = pos->second;

				break;
			}
		}

		//  ¸øÃ£À» °¡´É¼ºÀº Á¦·Î´Ù...ÀÌ·±°æ¿ì ÀÏ´Ü Á¾·áÇÑ´Ù.
		if (itemIDFirst == 0 && itemIDSecond == 0)
		{
			break;
		}

		vecCoordishopItemList.push_back(std::make_pair(itemIDFirst, itemIDSecond));

		if (itemIDFirst != 0)
		{
			std::vector< GCITEMID >::iterator pos_find = std::find(vecItemList.begin(), vecItemList.end(), itemIDFirst);
			if (pos_find != vecItemList.end())
				vecItemList.erase(pos_find);
		}

		if (itemIDSecond != 0)
		{
			std::vector< GCITEMID >::iterator pos_find = std::find(vecItemList.begin(), vecItemList.end(), itemIDSecond);
			if (pos_find != vecItemList.end())
				vecItemList.erase(pos_find);
		}
	}

	m_vecCoordishopItemList.swap(vecCoordishopItemList);

}

void KGCItemManager::MakeHotCoordiShopItemList(EGCCharType eCharType_, std::vector< KCoordiShopItem >& vecItemList_)
{
#if defined(NATION_USA) || defined(NATION_KOREA) || defined(NATION_TAIWAN) || defined (NATION_BRAZIL) || defined ( NATION_CHINA )
	std::vector< KCoordiShopItem > vectemp;

	for (std::map< int, KCoordiShopItem >::iterator mit = m_mapCoordishopItem.begin(); mit != m_mapCoordishopItem.end(); ++mit)
	{
		if (mit->second.pOriginal->GetCharType() == eCharType_)
		{
			KCoordiShopItem tempItem;
			if (mit->second.pOriginal && (mit->second.pOriginal->dwSlotPosition == 0x00000000 && !mit->second.pOriginal->bNotBuyItem))
				tempItem.pOriginal = mit->second.pOriginal;

			if (mit->second.pPackage7 && (mit->second.pPackage7->dwSlotPosition == 0x00000000 && !mit->second.pPackage7->bNotBuyItem))
				tempItem.pPackage7 = mit->second.pPackage7;

#if defined (NATION_BRAZIL) || defined( NATION_USA )
			if (tempItem.pOriginal || tempItem.pPackage7)
#else
			if (tempItem.pOriginal && tempItem.pPackage7)
#endif
				vectemp.push_back(tempItem);
		}
	}
	// GoodID°¡ Å«³ðÀÌ ÃÖ½Å - ÃÖ½Å ¼øÀ¸·Î ¸¸µé¾î ÁÝ´Ï´Ù 
	while (false == vectemp.empty()) {
		std::vector< KCoordiShopItem >::iterator bigID = vectemp.begin();

		for (std::vector< KCoordiShopItem >::iterator vit = vectemp.begin(); vit != vectemp.end(); ++vit)
		{
			if (vit->pOriginal && bigID->pOriginal)
			{

				if (vit->pOriginal->dwGoodsID > bigID->pOriginal->dwGoodsID) {
					bigID = vit;
				}
			}
			else if (vit->pPackage7 && bigID->pPackage7)
			{
				if (vit->pPackage7->dwGoodsID > bigID->pPackage7->dwGoodsID) {
					bigID = vit;
				}
			}
		}

		vecItemList_.push_back(*bigID);
		vectemp.erase(bigID);

	}
#else


	for (std::map< int, KCoordiShopItem >::iterator mit = m_mapCoordishopItem.begin(); mit != m_mapCoordishopItem.end(); ++mit)
	{
		if (mit->second.pOriginal->GetCharType() == eCharType_)
		{
			KCoordiShopItem tempItem;
			if (mit->second.pOriginal && (mit->second.pOriginal->dwSlotPosition == 0x00000000 && !mit->second.pOriginal->bNotBuyItem))
				tempItem.pOriginal = mit->second.pOriginal;

			if (mit->second.pPackage7 && (mit->second.pPackage7->dwSlotPosition == 0x00000000 && !mit->second.pPackage7->bNotBuyItem))
				tempItem.pPackage7 = mit->second.pPackage7;            if (tempItem.pOriginal || tempItem.pPackage7)
				vecItemList_.push_back(tempItem);
		}
	}
#endif
}

bool KGCItemManager::GetCoordiItem(ECoordiShopItemType eType, int iShopItem, GCItem*& pItem)
{
	if (iShopItem >= static_cast<int>(m_vecCoordishopItemList.size()) || iShopItem < 0)
		return false;

	GCITEMID idItemID = 0;
	if (eType == COORDI_SHOP_ORIGINAL) {
		idItemID = m_vecCoordishopItemList[iShopItem].first;
	}
	else {
		idItemID = m_vecCoordishopItemList[iShopItem].second;
	}

	if (idItemID == 0)
		return false;

	pItem = GetItemData(idItemID);
	return true;
}

bool KGCItemManager::GetCoordiItem(int iCoordiSetID, KCoordiShopItem& itemInfo)
{
	std::map<int, KCoordiShopItem>::iterator it = m_mapCoordishopItem.find(iCoordiSetID);

	if (it == m_mapCoordishopItem.end())
		return false;

	itemInfo = m_mapCoordishopItem[iCoordiSetID];
	return true;
}
#endif


bool KGCItemManager::IsBaseCoordiItem(GCITEMID itemID_)
{
	return m_setBaseCoordiItem.find(itemID_) != m_setBaseCoordiItem.end();
}

bool KGCItemManager::IsTransparentCoordiItem(GCITEMID itemID_)
{
	return m_setTransparentCoordiItem.find(itemID_) != m_setTransparentCoordiItem.end();
}

void KGCItemManager::LoadBaseCoordiItem(void)
{
	KLuaManager luaMgr;

	if (!GCFUNC::LoadLuaScript(luaMgr, "CoordiShopInfo.stg", 0))
		return;

	LUA_BEGIN_TABLE("BASE_COORDI_ITEM", return)
	{
		for (int i = 1; ; ++i)
		{
			DWORD dwGoodsID = 0;

			if (FAILED(luaMgr.GetValue(i, dwGoodsID)))
				break;

			m_setBaseCoordiItem.insert(dwGoodsID / 10);
		}
	}
	luaMgr.EndTable();  // BASE_COORDI_ITEM    

	LUA_BEGIN_TABLE("TRANSPARENT_COORDI_ITEM", return)
	{
		for (int i = 1; ; ++i)
		{
			DWORD dwGoodsID = 0;

			if (FAILED(luaMgr.GetValue(i, dwGoodsID)))
				break;

			m_setTransparentCoordiItem.insert(dwGoodsID / 10);
		}
	}
	luaMgr.EndTable();  // BASE_COORDI_ITEM    
}

void KGCItemManager::BuyGPItem(GCItem* kItem, int iItemID, int iPeriod)
{
	std::wostringstream strmText1;
	std::wostringstream strmText2;

#if defined ( _KOREAN ) || defined( _SPANISH )
	strmText1 << kItem->strItemName;
	strmText2 << g_pkStrLoader->GetReplacedString(STR_ID_BUY_PRICE, "i", (int)kItem->dwPrice);
#elif defined( _ENGLISH ) || defined(_THAI_LANG)
	strmText1 << g_pkStrLoader->GetReplacedString(STR_ID_BUY_PRICE, "li", kItem->strItemName, (int)kItem->dwPrice);
#else

	strmText1 << kItem->strItemName << g_pkStrLoader->GetString(STR_ID_OBJECT_POSTPOSITION);
	strmText2 << kItem->dwPrice << ' ' << g_pkStrLoader->GetString(STR_ID_BUY_PRICE);
#endif

#if defined ( NEW_GPITEM_BUY_PROCESS )
	strmText1.str(L"");
	strmText2.str(L"");
	strmText1 << kItem->strItemName << g_pkStrLoader->GetString(STR_ID_OBJECT_POSTPOSITION);
	if ((int)kItem->dwPrice > g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().GetCurrentGP())
	{
		strmText2 << g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_GP);

		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			strmText1.str(), strmText2.str(),
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
	}
	else
	{
		strmText2 << kItem->dwPrice << ' ' << g_pkStrLoader->GetString(STR_ID_BUY_PRICE);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
			strmText1.str(), strmText2.str(),
			KGCUIScene::GC_MBOX_USE_BUY, iItemID, iPeriod, false, true);
	}
#else
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
		strmText1.str(), strmText2.str(),
		KGCUIScene::GC_MBOX_USE_BUY, iItemID, iPeriod, false, true);
#endif
}



void KGCItemManager::BuyCashItemWithPoint(GCItem* kItem, int iPeriod)
{
	if (g_pkUIScene->m_pkCashShopScene->IsPointMode())
	{
		int iPrice = static_cast<int>(kItem->dwPrice * 10);

		if (g_MyD3D->m_kItemMgr.IsInVaildPointShopItem(kItem->dwGoodsID))
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_BUY_ATTEND_POINT),
				L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		}
		else if (iPrice <= g_kGlobalValue.m_iGCPoint)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
				g_pkStrLoader->GetReplacedString(STR_ID_BUY_POINT_ITEM, "li", kItem->strItemName, iPrice), L"",
				KGCUIScene::GC_MBOX_USE_BUY_POINT_ITEM, kItem->dwGoodsID, KBuySellItemReq::EMT_CASH, false, true);
		}
		else
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetReplacedString(STR_ID_NOT_ENOUGH_POINT, "l", kItem->strItemName), L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		}
	}
	else
	{
		g_MyD3D->m_kItemMgr.BuyItem(kItem->dwGoodsID, iPeriod);
	}
}

void KGCItemManager::LoadPointShopValidItemList(void)
{
	KLuaManager luaMgr;

	if (!GCFUNC::LoadLuaScript(luaMgr, "PointShopValidItemList.lua", 0))
		return;

	LUA_BEGIN_TABLE("POINTSHOP_VALID_ITEM_LIST", return)
	{
		for (int i = 1; ; ++i)
		{
			DWORD dwGoodsID = 0;

			if (FAILED(luaMgr.GetValue(i, dwGoodsID)))
				break;

			m_setPointShopValidItemList.insert(dwGoodsID / 10);
		}
	}
	luaMgr.EndTable();  // BASE_COORDI_ITEM    
}


BOOL KGCItemManager::IsInVaildPointShopItem(GCITEMID _goodsID)
{
	if (m_setPointShopValidItemList.end() != m_setPointShopValidItemList.find(_goodsID))
	{
		return false;
	}
	return true;
}

bool KGCItemManager::HaveEventItem()
{
	for (std::set<GCITEMID>::iterator vIt = g_kGlobalValue.m_setEventAlarmItemList.begin(); vIt != g_kGlobalValue.m_setEventAlarmItemList.end(); vIt++)
	{
		if (FindInventoryForItemID((*vIt) / 10))
		{
			return true;
		}
	}
	return false;
}

void KGCItemManager::MakeAgitCashItemList(std::vector<DWORD>& vecAgitCashItem)
{
	std::map< DWORD, KAgitObjectInfo > mapAgitObj = SiKGCAgitGameManager()->GetAgitObjectMap();
	std::set< GCITEMID > setExceptAgitObj = SiKGCAgitGameManager()->GetExeceptAgitObjectSet();

	for (std::map< DWORD, KAgitObjectInfo >::iterator mit = mapAgitObj.begin(); mit != mapAgitObj.end(); ++mit)
	{
		auto it = m_mapDB.find(mit->first / 10);

		if (it == m_mapDB.end())
			continue;

		if (it->second->bNotBuyItem)
			continue;

		if (EMT_CASH == it->second->eMoneyType)
			vecAgitCashItem.push_back(it->first);
	}


	for (std::set< GCITEMID >::iterator sit = setExceptAgitObj.begin(); sit != setExceptAgitObj.end(); ++sit)
	{
		auto it = m_mapDB.find(*sit / 10);

		if (it == m_mapDB.end())
			continue;

		if (it->second->bNotBuyItem)
			continue;

		if (EMT_CASH == it->second->eMoneyType)
			vecAgitCashItem.push_back(it->first);
	}
}

void KGCItemManager::InitEnchantItemStatTable()
{
	//ÇØÅ·ÀÇ À§Çè ¶§¹®¿¡ ¼öÄ¡µéÀº ÄÚµå¿¡ Á÷Á¢ ³ÖÀ½.
	//¼öÄ¡µéÀº ¼­¹ö·Î ¿Å±â´Â ÂÊÀº ¾î¶³Áö?
	m_mapEnchantEpicItemStatTable.clear();
	m_mapEnchantNormalItemStatTable.clear();

	std::map<int, vector<float>> mapStat;
	mapStat.clear();

	// °ø°Ý·Â Ç¥ÁØ ½ºÅÝ
	mapStat[0].push_back(0.f);
	mapStat[0].push_back(55.f);
	mapStat[0].push_back(74.f);

	// 34°­ºÎÅÍ´Â ¼öÄ¡ÀÇ º¯È­°¡ ¾ø¾î¼­ 34±îÁö ·çÇÁ¸¦ µ·´Ù.
	for (int i = 3; i <= 34; i++)
	{
		float fAttStat = mapStat[0][i - 1] + (mapStat[0][i - 1] - mapStat[0][i - 2]) * (1.45f - i * 0.029f);
		mapStat[0].push_back(fAttStat);
	}

	// ¹æ¾î·Â Ç¥ÁØ ½ºÅÝ
	for (int i = 0; i <= 34; i++)
	{
		if (i == 0)
		{
			mapStat[1].push_back(0.f);
			continue;
		}

		mapStat[1].push_back(mapStat[0][i] * 0.6f + 400.f);
	}

	// »ý¸í·Â Ç¥ÁØ ½ºÅÝ
	for (int i = 0; i <= 34; i++)
	{
		mapStat[2].push_back(mapStat[0][i] * 0.4f);
	}

	float rate;
	int iIndex;
	for (int i = 1; i <= 7; i++)
	{
		if (i == 1)
		{
			rate = 0.3f;
			iIndex = 1;
		}
		else if (i == 2 || i == 3)
		{
			rate = 0.35f;
			iIndex = 1;
		}
		else if (i == 4)
		{
			rate = 0.3f;
			iIndex = 0;
		}
		else if (i == 5 || i == 6)
		{
			rate = 0.5f;
			iIndex = 2;
		}
		else if (i == 7)
		{
			rate = 0.7f;
			iIndex = 0;
		}

		std::map<int, std::vector<float>>::iterator mit = mapStat.find(iIndex);

		if (mit == mapStat.end())
			continue;

		for (int j = 0; j < static_cast<int>(mit->second.size()); j++)
		{
			//int iStat = static_cast<int>(round(mit->second[j] * rate));
			int iStat = 0;
			if (iIndex == 0) {     // °ø°Ý·ÂÀÎ °æ¿ì ÃÖÁ¾ ½ºÅÈ¿¡ 30% Ãß°¡ 
				iStat = static_cast<int>(round(mit->second[j] * rate * 1.3f));
			}
			else {
				iStat = static_cast<int>(round(mit->second[j] * rate));
			}

			m_mapEnchantEpicItemStatTable[i].push_back(iStat);
		}

		for (int j = 0; j < static_cast<int>(mit->second.size()); j++)
		{
			//int iStat = static_cast<int>(round(mit->second[j] * rate * (0.7f + 0.005f * (float)j)));
			int iStat = 0;
			if (iIndex == 0) { // °ø°Ý·ÂÀÎ °æ¿ì ÃÖÁ¾ ½ºÅÈ¿¡ 30% Ãß°¡ 
				iStat = static_cast<int>(round(mit->second[j] * rate * (0.7f + 0.005f * (float)j) * 1.3f));
			}
			else {
				iStat = static_cast<int>(round(mit->second[j] * rate * (0.7f + 0.005f * (float)j)));
			}

			m_mapEnchantNormalItemStatTable[i].push_back(iStat);
		}
	}
}

int KGCItemManager::GetEnchantItemStat(int iItemType, int iEnchantItemType_, int iEnchantLv_)
{
	if (iEnchantLv_ < 0)
		return -1;

	if (iEnchantItemType_ == KItem::GRADE_NORMAL)
	{
		std::map<int, std::vector<int>>::iterator mit = m_mapEnchantNormalItemStatTable.find(iItemType);
		if (mit != m_mapEnchantNormalItemStatTable.end())
		{
			int iMaxSize = static_cast<int>(mit->second.size());
			if (iEnchantLv_ < iMaxSize)
				return mit->second[iEnchantLv_];
			else
				return mit->second[iMaxSize - 1];
		}
	}
	else if (iEnchantItemType_ == KItem::GRADE_EPIC)
	{
		std::map<int, std::vector<int>>::iterator mit = m_mapEnchantEpicItemStatTable.find(iItemType);
		if (mit != m_mapEnchantEpicItemStatTable.end())
		{
			int iMaxSize = static_cast<int>(mit->second.size());
			if (iEnchantLv_ < iMaxSize)
				return mit->second[iEnchantLv_];
			else
				return mit->second[iMaxSize - 1];
		}
	}

	return -1;
}

GCItemAbility KGCItemManager::GetEnchantItemAbility(GCItem* pItem_, KItem* pInvenItem_)
{
	GCItemAbility kAbility;
	if (pInvenItem_ == NULL || pItem_ == NULL)
		return kAbility;

	if (pItem_->dwGoodsID != pInvenItem_->m_ItemID)
		return kAbility;

	if (pInvenItem_->m_EnchantEquipItemUID == 0)
		return kAbility;

	KItem* kEnchantItem = GetInventoryItem(pInvenItem_->m_EnchantEquipItemUID);

	if (kEnchantItem == NULL)
		return kAbility;

	if (pItem_->dwSlotPosition & ESP_HELMET)
	{
		kAbility.iDef = GetEnchantItemStat(EISP_HELMET, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_JACKET)
	{
		kAbility.iDef = GetEnchantItemStat(EISP_JACKET, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_PANTS)
	{
		kAbility.iDef = GetEnchantItemStat(EISP_PANTS, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_GLOVES)
	{
		kAbility.iAtk = GetEnchantItemStat(EISP_GLOVES, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_SHOES)
	{
		kAbility.iVit = GetEnchantItemStat(EISP_SHOES, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_MANTLE)
	{
		kAbility.iVit = GetEnchantItemStat(EISP_MANTLE, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_WEAPON)
	{
		kAbility.iAtk = GetEnchantItemStat(EISP_WEAPON, kEnchantItem->m_cGradeID, kEnchantItem->m_cEnchantLevel);
	}

	return kAbility;
}

GCItemAbility KGCItemManager::GetEnchantItemAbility(GCItem* pItem_, SUserItem* pUserItem_)
{
	GCItemAbility kAbility;
	if (pItem_ == NULL || pUserItem_ == NULL)
		return kAbility;

	if (pItem_->dwGoodsID != pUserItem_->iItemID)
		return kAbility;

	if (pItem_->dwSlotPosition & ESP_HELMET)
	{
		kAbility.iDef = GetEnchantItemStat(EISP_HELMET, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_JACKET)
	{
		kAbility.iDef = GetEnchantItemStat(EISP_JACKET, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_PANTS)
	{
		kAbility.iDef = GetEnchantItemStat(EISP_PANTS, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_GLOVES)
	{
		kAbility.iAtk = GetEnchantItemStat(EISP_GLOVES, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_SHOES)
	{
		kAbility.iVit = GetEnchantItemStat(EISP_SHOES, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_MANTLE)
	{
		kAbility.iVit = GetEnchantItemStat(EISP_MANTLE, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}
	else if (pItem_->dwSlotPosition & ESP_WEAPON)
	{
		kAbility.iAtk = GetEnchantItemStat(EISP_WEAPON, pUserItem_->m_cEnchantEquipGradeID, pUserItem_->cEnchantLevel);
	}

	return kAbility;
}

std::pair<bool, int> KGCItemManager::GetEquipedEnchantWeaponInfo(SCharInfo* pCharInfo_)
{
	if (pCharInfo_ != NULL)
	{

		for (int i = 0; i < (int)pCharInfo_->vecItemInfo.size(); i++)
		{
			KItem* pkItem = GetInventoryItem(pCharInfo_->vecItemInfo[i].dwItemUID);
			if (pkItem)
			{
				GCItem* pkGCItem = GetItemData(pkItem->m_ItemID);

				if (pkGCItem)
				{
					if (pkGCItem->dwSlotPosition & ESP_WEAPON && pkItem->m_EnchantEquipItemUID != 0)
					{
						return std::make_pair(true, pkItem->m_cEnchantLevel);
					}
				}
			}
		}
	}
	return std::make_pair(false, 0);
}

bool ItemInvenBuffComp(KInvenBuffItem& lhs, KInvenBuffItem& rhs)
{
	if (lhs.m_usPriorityIndex < rhs.m_usPriorityIndex)
		return true;

	return false;
}

void KGCItemManager::SetInventBuffItemList(VEC_INVEN_BUFF& vecInvenBuff)
{
	VEC_INVEN_BUFF_ITER pos = vecInvenBuff.begin();
	VEC_INVEN_BUFF_ITER end = vecInvenBuff.end();

	for (; pos != end; ++pos)
	{
		KInvenBuffItem sItem = *pos;

		if (m_mapInvenBuffItem.find(sItem.m_nGroupID) != m_mapInvenBuffItem.end())
		{
			m_mapInvenBuffItem[sItem.m_nGroupID].push_back(sItem);
		}
		else
		{
			VEC_INVEN_BUFF vecInvenBuff;
			vecInvenBuff.push_back(sItem);

			m_mapInvenBuffItem.insert(std::make_pair(sItem.m_nGroupID, vecInvenBuff));
		}
	}

	/*
	MAP_INVEN_BUFF_GROUP_ITER pos_map = m_mapInvenBuffItem.begin();
	MAP_INVEN_BUFF_GROUP_ITER pos_map_end = m_mapInvenBuffItem.end();

	for (; pos_map != pos_map_end; ++pos_map)
	{
		VEC_INVEN_BUFF& vecInvenBuff = pos_map->second;
		//std::sort(vecInvenBuff.begin(), vecInvenBuff.end(), ItemInvenBuffComp);
	}*/
}

void KGCItemManager::UnEquipWeapon(SCharInfo* pCharInfo)
{
	for (KUserItemVector::iterator vit = pCharInfo->vecItemInfo.begin(); vit != pCharInfo->vecItemInfo.end(); ++vit) {
		GCItem* pItem = g_pItemMgr->GetItemData(vit->iItemID);
		if (NULL == pItem) {
			continue;
		}

		if (pItem->dwSlotPosition & ESP_WEAPON) {
			pCharInfo->vecItemInfo.erase(vit);
			break;
		}
	}
}

bool KGCItemManager::IsHotItem(GCITEMUID itemUID_, PLAYER* pPlayer_)
{
	if (NULL == pPlayer_) {
		return false;
	}

	GCItem* pNewItem = NULL;
	GCItem* pEquipItem = NULL;
	KItem* pNewInvenItem = NULL;
	KItem* pEquipInvenItem = NULL;

	FindInventory(itemUID_, &pNewInvenItem, &pNewItem);
	if (NULL == pNewItem || NULL == pNewInvenItem) {
		return false;
	}

	// ÀåÂø °¡´ÉÇÑ ºÎÀ§ÀÎ°¡?
	if (false == IsEquipmentItem(pNewItem->dwGoodsID)) {
		return false;
	}

	// ³» Ä³¸¯ÅÍ ¾ÆÀÌÅÛÀÎ°¡?
	if (pPlayer_->GetCurrentChar().iCharType != pNewItem->GetCharType()) {
		return false;
	}

	// ÀüÁ÷ ·¹º§ Ã¼Å© 
	if (pNewItem->iNeedChangeJobLevelForEquippingItem > pPlayer_->GetCurrentChar().iPromotionLevel) {
		return false;
	}

	// ÀåÂø ·¹º§ Ã¼Å© 
	if (false == CheckEquipLevel(pPlayer_->GetCurrentChar().iLevel, itemUID_)) {
		return false;
	}

	// ÇØ´ç ºÎÀ§¿¡ ³»°¡ ÀåÂøÇÑ ¾ÆÀÌÅÛ°ú ºñ±³ 
	if (false == GetMyCharEquipByItemPosition(pNewItem->dwSlotPosition, &pEquipInvenItem, &pEquipItem)) {
		return true;    // ÇØ´ç ºÎÀ§ ÀåÂø ¾ÆÀÌÅÛ ¾øÀ½ 
	}

	// ´É·ÂÄ¡ ¾ò¾î¿È 
	EItemAbilityData eNewItemData = GetItemAbilityData(pNewItem, pPlayer_->GetCurrentChar().iLevel, pNewInvenItem);
	EItemAbilityData eEquipItemData = GetItemAbilityData(pEquipItem, pPlayer_->GetCurrentChar().iLevel, pEquipInvenItem);

	// °ø¹æ»ý ºñ±³ 
	float fNewTotalAbility = eNewItemData.fAtk + eNewItemData.fDef + eEquipItemData.fHP
		+ eNewItemData.fItemAttribute[ATTRTYPE_ATK_POINT] + eNewItemData.fItemAttribute[ATTRTYPE_DEF_POINT] + eNewItemData.fItemAttribute[ATTRTYPE_HP_POINT];
	float fEquipTotalAbility = eEquipItemData.fAtk + eEquipItemData.fDef + eEquipItemData.fHP
		+ eEquipItemData.fItemAttribute[ATTRTYPE_ATK_POINT] + eEquipItemData.fItemAttribute[ATTRTYPE_DEF_POINT] + eEquipItemData.fItemAttribute[ATTRTYPE_HP_POINT];

	if (fEquipTotalAbility >= fNewTotalAbility) {
		return false;
	}

	return true;
}

void KGCItemManager::AddNewItem(GCITEMUID itemUID_)
{
	m_kInventory.AddNewItem(itemUID_);
}

void KGCItemManager::ClearNewItem(void)
{
	m_kInventory.ClearNewItem();
}

bool KGCItemManager::IsNewItem(GCITEMUID itemUID_)
{
	return m_kInventory.IsNewItem(itemUID_);
}

bool KGCItemManager::CheckEquipLevel(const int iCharacterLevel_, GCItem* pItem_, KItem* pInvenItem_)
{
	// ÀåÂø ·¹º§ Ã¼Å© 
	std::vector< KAttributeInfo > vecAttribute = pInvenItem_->m_vecAttribute;
	std::vector< KAttributeInfo >::iterator vitAttribute = vecAttribute.begin();

	char cEquipDecLvValue = 0;
	while (vecAttribute.end() != vitAttribute)
	{
		if ((*vitAttribute).m_cType == ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN)
		{
			cEquipDecLvValue = static_cast<char>((*vitAttribute).m_fValue);
		}
		++vitAttribute;
	}

	if (iCharacterLevel_ < g_pItemMgr->GetlItemEquipLevel(pItem_, pInvenItem_, NULL, true) - cEquipDecLvValue) {
		return false;
	}

	return true;
}

bool KGCItemManager::CheckEquipLevel(const int iCharacterLevel_, GCITEMUID itemUID_)
{
	GCItem* pItem = NULL;
	KItem* pInvenItem = NULL;

	FindInventory(itemUID_, &pInvenItem, &pItem);
	if (NULL == pItem || NULL == pInvenItem) {
		return false;
	}

	return CheckEquipLevel(iCharacterLevel_, pItem, pInvenItem);
}

bool KGCItemManager::GetItemUniqueSlotPosAndAbility(IN GCITEMUID itemUID_, IN SCharInfo& sCharInfo_, OUT int& iSlotPos_, OUT int& iAbility_)
{
	GCItem* pItem = NULL;
	KItem* pInvenItem = NULL;

	g_pItemMgr->FindInventory(itemUID_, &pInvenItem, &pItem);
	if (NULL == pItem || NULL == pInvenItem) {
		return false;
	}

	if (pItem->dwSlotPosition & ESP_PET) {  // ÆêÀº °É·¯ ÁÝ´Ï´Ù. 
		return false;
	}

	if (pItem->eItemKind == GIK_ENCHANT_ASIST_ITEM) {
		return false;
	}

	if (false == g_pItemMgr->IsEquipmentItem(pItem->dwGoodsID)) {   // ÀåÂø ÀÚ´ÉÇÑ ÀåºñÀÎ°¡?
		return false;
	}

	if (false == g_pItemMgr->CheckEquipLevel(sCharInfo_.iLevel, pItem, pInvenItem)) {   // ÀåÂø ·¹º§  Ã¼Å© 
		return false;
	}

	if (pItem->iNeedChangeJobLevelForEquippingItem > sCharInfo_.iPromotionLevel) {         // ÀüÁ÷ ·¹º§ Ã¼Å© 
		return false;
	}

	iSlotPos_ = g_pItemMgr->GetUniqueSlotPosition(pItem->dwSlotPosition);

	EItemAbilityData eItemAbility = GetItemAbilityData(pItem, sCharInfo_.iLevel, pInvenItem);
	float fItemStatus = eItemAbility.fAtk + eItemAbility.fDef + eItemAbility.fHP
		+ eItemAbility.fItemAttribute[ATTRTYPE_ATK_POINT] + eItemAbility.fItemAttribute[ATTRTYPE_DEF_POINT] + eItemAbility.fItemAttribute[ATTRTYPE_HP_POINT];

	iAbility_ = static_cast<int>(fItemStatus);

	return true;
}

void KGCItemManager::FilterUnuseCoordiItemList(IN OUT std::vector< GCITEMUID >& vecItemList_)
{
	DWORD dwCharType = 1;
	KCharInfoVector& vecCharInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo;
	//     for( KCharInfoVector::iterator vit = vecCharInfo.begin() ; vit != vecCharInfo.end() ; ++vit ) 
	//     {
	//         // º¸À¯´Â ÇßÁö¸¸ ºÀÀÎµÈ Ä³¸¯ÅÍ´Â ¹Ì»ç¿ë ÅÇ¿¡ º¸¿©Áöµµ·Ï
	//         std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find( vit->iCharType );
	//         if( mapSealedIter != g_kGlobalValue.m_mapSealedCharInfo.end() )
	//         {
	//             if( mapSealedIter->second.m_bIsSealed )
	//                 continue;
	//         }
	// 
	//         dwCharType |= GetCharFlag( vit->iCharType );
	//     }

	dwCharType = GetCharFlag(g_kGlobalValue.m_kUserInfo.GetCurrentCharType());

	for (std::vector< GCITEMUID >::iterator vit = vecItemList_.begin(); vit != vecItemList_.end(); ) {
		GCItem* pItem = NULL;
		g_pItemMgr->FindInventory(*vit, &pItem);

		if (NULL == pItem) {
			++vit;
			continue;
		}

		if (pItem->dwCharType & dwCharType) {
			vecItemList_.erase(vit);
			continue;
		}

		++vit;
	}
}


EITEMSLOTPOSITION KGCItemManager::GetItemSlotPosion(COORDI_SLOT_POSION emPosition)
{
	switch (emPosition)
	{
	case COORDI_PARTS_HELMET:
		return EISP_HELMET;
	case COORDI_PARTS_JACKET:
		return EISP_JACKET;
	case COORDI_PARTS_PANTS:
		return EISP_PANTS;
	case COORDI_PARTS_GLOVES:
		return EISP_GLOVES;
	case COORDI_PARTS_SHOES:
		return EISP_SHOES;
	case COORDI_PARTS_MANTLE:
		return EISP_MANTLE;
	case COORDI_PARTS_A_UPPER_HELMET:
		return EISP_A_UPPER_HELMET;
	case COORDI_PARTS_A_DOWN_HELMET:
		return EISP_A_DOWN_HELMET;
	case COORDI_PARTS_A_JACKET:
		return EISP_A_JACKET;
	case COORDI_PARTS_A_PANTS:
		return EISP_A_PANTS;
	case COORDI_PARTS_A_ARM:
		return EISP_A_ARM;
	case COORDI_PARTS_WEAPON:
		return EISP_WEAPON;
	default:
		return EISP_NUM;
	}
}

ESLOTPOSITION KGCItemManager::GetSlotPosion(COORDI_SLOT_POSION emPosition)
{
	switch (emPosition)
	{
	case COORDI_PARTS_HELMET:
		return ESP_HELMET;
	case COORDI_PARTS_JACKET:
		return ESP_JACKET;
	case COORDI_PARTS_PANTS:
		return ESP_PANTS;
	case COORDI_PARTS_GLOVES:
		return ESP_GLOVES;
	case COORDI_PARTS_SHOES:
		return ESP_SHOES;
	case COORDI_PARTS_MANTLE:
		return ESP_MANTLE;
	case COORDI_PARTS_A_UPPER_HELMET:
		return ESP_A_UPPER_HELMET;
	case COORDI_PARTS_A_DOWN_HELMET:
		return ESP_A_DOWN_HELMET;
	case COORDI_PARTS_A_JACKET:
		return ESP_A_JACKET;
	case COORDI_PARTS_A_PANTS:
		return ESP_A_PANTS;
	case COORDI_PARTS_A_ARM:
		return ESP_A_ARM;
	case COORDI_PARTS_A_NECKLET:
		return ESP_A_NECKLET;
	case COORDI_PARTS_A_SHOES:
		return ESP_A_SHOES;
	case COORDI_PARTS_TITLE:
		return ESP_TITLE;
	case COORDI_PARTS_WEAPON:
		return ESP_WEAPON;
	default:
		return ESP_NONE;
	}
}

std::wstring KGCItemManager::GetCoordiSeasonName(int _iSeasonNum)
{
	std::wstring strName;
	for (int i = 0; i < (int)m_vecCoordiSensonList.size(); i++)
	{
		if (_iSeasonNum == m_vecCoordiSensonList[i].iSeasonNum)
		{
			return g_pkStrLoader->GetString(m_vecCoordiSensonList[i].iSeasonText);
		}
	}

	return strName;
}

KGCItemManager::CoordiSeasonTemplet* KGCItemManager::GetCoordiSeasonTemplet(GCITEMID _itemID, int nGrade, bool bDevide10)
{
	if (bDevide10)
	{
		_itemID /= 10;
	}

	for (int i = 0; i < (int)m_vecCoordiSensonList.size(); i++)
	{

		//  µî±Þ Ã¼Å©
		int iSeasonGrade = m_vecCoordiSensonList[i].iSeasonGrade;
		if (iSeasonGrade != -1 && iSeasonGrade != nGrade)
			continue;

		std::set<int>& setItemList = m_vecCoordiSensonList[i].setItemList;

		std::set<int>::iterator pos = setItemList.find(_itemID);
		if (pos != setItemList.end())
		{
			return &m_vecCoordiSensonList[i];
		}
	}

	return NULL;
}

int KGCItemManager::GetCoordiSeasonNum(GCITEMID ItemID_, int nGrade, std::set< int >* pVecPartsList_)
{
	int iSeasonNum = UINT_MAX;

	CoordiSeasonTemplet* pTemplet = GetCoordiSeasonTemplet(ItemID_, nGrade);
	if (pTemplet)
	{
		iSeasonNum = pTemplet->iSeasonNum;
		if (pVecPartsList_)
		{
			*pVecPartsList_ = pTemplet->setPartsList;
		}
	}

	return iSeasonNum;
}

int KGCItemManager::GetCoordiSlotSeasonNum(SCharInfo* pCharInfo, ESLOTPOSITION emPosition)
{
	pCharInfo->vecLookInfo;

	for (int i = 0; i < (int)pCharInfo->vecLookInfo.size(); ++i)
	{
		GCItem* pItem = GetItemData(pCharInfo->vecLookInfo[i].iItemID);
		if (!pItem)
			continue;

		if (!(pItem->dwSlotPosition & emPosition))
			continue;

		return GetCoordiSeasonNum(pItem->dwGoodsID, pCharInfo->vecLookInfo[i].cGradeID);
	}

	return UINT_MAX;
}

bool KGCItemManager::GetInventoryItemListFromItemID(IN GCITEMID itemID_, OUT std::vector< KItem* >& vecItemList)
{
	return m_kInventory.FindItemListByItemID(itemID_, vecItemList);
}

int KGCItemManager::GetLevelDownReqCoin(GCItem* pItem, KItem* pInvenItem)
{
	int nReqCoin = 0;

	if (!pInvenItem || !pItem)
		return 0;

	int nNeedLevel = GetlItemEquipLevel(pItem, pInvenItem);
	nReqCoin = (nNeedLevel / 10) + (pInvenItem->m_sEquipLvDown * pInvenItem->m_sEquipLvDown);

	return nReqCoin;
}

bool KGCItemManager::IsLevelDownItem(KItem* pInvenItem)
{
	if (!pInvenItem)
		return false;

	if (!IsEquipmentItem(pInvenItem->m_ItemID))
		return false;

	if (GetLevelDownTableID(pInvenItem->m_ItemID) <= 0)
		return false;

	return true;
}

std::pair<bool, int> KGCItemManager::GetOtherUserEquipedEnchantWeaponInfo(SCharInfo* pCharInfo_)
{
	if (pCharInfo_ != NULL)
	{
		for (int i = 0; i < (int)pCharInfo_->vecItemInfo.size(); i++)
		{
			GCItem* pItem = GetItemData(pCharInfo_->vecItemInfo[i].iItemID);
			if (pItem) {
				if (pItem->dwSlotPosition & ESP_WEAPON && pCharInfo_->vecItemInfo[i].cEnchantLevel != 0) {
					return std::make_pair(true, pCharInfo_->vecItemInfo[i].cEnchantLevel);
				}
			}
		}
	}

	return std::make_pair(false, 0);
}

int KGCItemManager::GetCoordiInventoryMaxSize()
{
	return m_iMaxCoordiInvenSize;
}

int KGCItemManager::SetCoordiInventoryMaxSize(int iMaxCoordiInvenSize)
{
	return m_iMaxCoordiInvenSize = iMaxCoordiInvenSize;
}

std::map<GCITEMID, std::vector<KDropItemInfo>>& KGCItemManager::GetPackageInfo()
{
	if (m_bIsPackagInfo == false)
	{
		Result_ReceivePackageInfo = INT_MAX;
		KP2P::GetInstance()->Send_PackageInfoReq();
		g_MyD3D->WaitForServerAck(Result_ReceivePackageInfo, INT_MAX, 5000, TIME_OUT_VALUE);

	}
	if (!m_bRecievePackageInfo)
	{
		for (int i = m_iPackageCurrendIndex; i <= m_iPackageIndexMax; i++)
		{
			Result_ReceivePackageDetailInfo = INT_MAX;
			KP2P::GetInstance()->Send_PackageInfoDetailReq(m_iPackageIndexMax, i);
			g_MyD3D->WaitForServerAck(Result_ReceivePackageDetailInfo, INT_MAX, 5000, TIME_OUT_VALUE);
		}
		return m_mapPackageInfo;
	}

	return m_mapPackageInfo;
}

void KGCItemManager::SetPackageInfo(std::map<GCITEMID, std::vector<KDropItemInfo>> _mapPackageInfo, bool bLastData)
{
	/*
	DB¿Í ¼­¹öÀÇ »çÁ¤À¸·Î ÇÏ³ªÀÇ ÆÐÅ°Áö¿¡ ¼ÓÇÑ ±¸¼ºÇ° Á¤º¸¸¦ ³ª´µ¾î Àü´Þ¹Þ´Â °æ¿ì°¡ »ý°å°í
	ÆÐÅ°Áö ÀÏºÎ ±¸¼ºÇ°¸¸ º¸ÀÌ´Â °æ¿ì·Î ÀÌ¾îÁö´Â ¹®Á¦°¡ ÀÖ¾î¼­
	Å¬¶óÀÌ¾ðÆ® ÄÚµå¿¡¼­ Á¤º¸¸¦ Àß Á¶ÇÕÇÏµµ·Ï Ä£ÀýÇÏ°Ô ¼öÁ¤

	±âÁ¸ ÄÚµå:
	m_mapPackageInfo.insert( _mapPackageInfo.begin(), _mapPackageInfo.end() );
	*/
	std::map< GCITEMID, std::vector< KDropItemInfo > >::iterator mitPackageInfo;
	for (mitPackageInfo = _mapPackageInfo.begin(); mitPackageInfo != _mapPackageInfo.end(); ++mitPackageInfo) {
		m_mapPackageInfo[mitPackageInfo->first].insert(m_mapPackageInfo[mitPackageInfo->first].end(), mitPackageInfo->second.begin(), mitPackageInfo->second.end());
	}

	m_bRecievePackageInfo = bLastData;
}

void KGCItemManager::ClearPackageInfo()
{
	m_bRecievePackageInfo = false;
	m_mapPackageInfo.clear();
}

bool KGCItemManager::IsPackageItemID(IN GCITEMID dwItemID_)
{
	std::map<GCITEMID, std::vector<KDropItemInfo>>& mapPackageInfo = GetPackageInfo();

	std::map<GCITEMID, std::vector<KDropItemInfo>>::iterator mapIter = mapPackageInfo.find(dwItemID_);
	if (mapIter != mapPackageInfo.end() && (mapIter->second.size() >= 1)) {
		// Ã£À¸¸é ÆÐÅ°Áö¿©~
		return true;
	}
	return false;
}

void KGCItemManager::GetPackageElements(IN GCITEMID dwItemID_, OUT std::vector< KDropItemInfo >& vecItems_)
{
	// ÁÖ¾îÁø ItemID°¡ ÆÐÅ°Áö ¾ÆÀÌÅÛÀÇ ID°¡ ¾Æ´Ï¸é vecItems_ ¿¡ ºó º¤ÅÍ¸¦ ³Ö¾îÁØ´Ù
	vecItems_.clear();

	std::map<GCITEMID, std::vector<KDropItemInfo>>& mapPackageInfo = GetPackageInfo();

	std::map<GCITEMID, std::vector<KDropItemInfo>>::iterator mapIter = mapPackageInfo.find(dwItemID_);
	if (mapIter != mapPackageInfo.end() && (mapIter->second.size() > 1)) {     // ÆÐÅ°Áö´Ï±î Ç×»ó 2°³ ÀÌ»óÀÌ°ÚÁö¿ë
		vecItems_ = mapIter->second;
	}

}

void KGCItemManager::LoadEquipDisableItemList(void)
{
	m_mapEquipDisableItem.clear();
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "EquipDisableItemList.stg") == false) {
		return;
	}

	if (SUCCEEDED(luaMgr.BeginTable("EquipDisableItemList"))) {
		int iIndex = 1;
		while (true) {
			if (FAILED(luaMgr.BeginTable(iIndex++))) {
				break;
			}

			int iCharType = -1;
			std::vector< GCITEMID > vecItemList;
			LUA_GET_VALUE_DEF("CHAR_TYPE", iCharType, -1);

			if (SUCCEEDED(luaMgr.BeginTable("ITEM_LIST"))) {
				for (int i = 1; ; ++i) {
					GCITEMID itemID = 0;

					if (FAILED(luaMgr.GetValue(i, itemID))) {
						break;
					}

					if (itemID != 0) {
						vecItemList.push_back(itemID);
					}
				}
			}
			luaMgr.EndTable();  // ITEM_LIST

			if (iCharType != -1) {
				m_mapEquipDisableItem.insert(std::make_pair(iCharType, vecItemList));
			}
			luaMgr.EndTable(); // °¢ Ä³¸¯ÅÍ 
		}
	}
	luaMgr.EndTable();  // EquipDisableItemList
}

bool KGCItemManager::IsEquipDisableItem(const IN int iCharType_, const IN GCITEMID itemID_)
{
	if (m_mapEquipDisableItem.empty()) {
		return false;
	}

	std::map< int, std::vector< GCITEMID > >::const_iterator mit = m_mapEquipDisableItem.find(iCharType_);
	if (mit == m_mapEquipDisableItem.end()) {
		return false;
	}

	if (mit->second.empty()) {
		return false;
	}

	std::vector< GCITEMID >::const_iterator vit = mit->second.begin();
	for (; vit != mit->second.end(); ++vit) {
		if (*vit == itemID_) {
			return true;
		}
	}

	return false;
}

bool KGCItemManager::IsPaintItem()
{
	if (g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB ||
		g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG ||
		g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCPREMIUM ||
		g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH) ||
		g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_ROOM_NAME_COLORING_GP) ||
		g_kGlobalValue.m_kUserInfo.cIsNetmarblePCRoom == 1)
		return true;

	return false;
}

bool KGCItemManager::IsCashPaintItem()
{
	if (!g_pItemMgr->FindInventoryForItemID(KGCItemManager::ITEM_ROOM_NAME_COLORING_CASH) &&
		!(g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB ||
			g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG ||
			g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCPREMIUM ||
			g_kGlobalValue.m_kUserInfo.cIsNetmarblePCRoom == 1))
		return false;

	return true;
}

bool KGCItemManager::IsFreePaintItem()
{
	if (g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB ||
		g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG ||
		g_kGlobalValue.m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCPREMIUM ||
		g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN) {
		return true;
	}
	return false;
}

bool KGCItemManager::IsCurrentCharacterItem(GCITEMID giItemID)
{
	GCItem* pkItem = GetItemData(giItemID);

	if (pkItem == NULL) return false;

	if (g_MyD3D->m_TempPlayer.GetCharTypeDword(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) & pkItem->dwCharType) {
		return true;
	}

	return false;
}


void KGCItemManager::SetCharacterCardInfo(KEVENT_NEW_CHAR_CARD_INFO_NOT& mapInfo)
{
	m_mapCharCardInfo = mapInfo;
}

bool KGCItemManager::GetCharacterCardInfo(int iChar, OUT KCharCardInfo& kInfo)
{
	for (KEVENT_NEW_CHAR_CARD_INFO_NOT::iterator mit = m_mapCharCardInfo.begin(); mit != m_mapCharCardInfo.end(); ++mit) {
		if (iChar != mit->second.m_cCharType) continue;

		kInfo = mit->second;
		return true;
	}

	return false;
}

bool KGCItemManager::IsCharCard(GCITEMID ItemIDCharCard)
{
	return m_mapCharCardInfo.find(ItemIDCharCard * 10) != m_mapCharCardInfo.end();
}

bool KGCItemManager::GetCharacterCardInfo(int iChar, OUT GCITEMID& ItemIDCharCard, OUT GCITEMUID& ItemUIDCharCard)
{
	KCharCardInfo kTemp = KCharCardInfo();
	if (GetCharacterCardInfo(iChar, kTemp) == false)
		return false;

	ItemIDCharCard = kTemp.m_ItmeID / 10;

	KItem* pItem = GetInventoryItemFromID(ItemIDCharCard);

	if (pItem == NULL)
		return false;

	ItemUIDCharCard = pItem->m_ItemUID;

	return true;
}

void KGCItemManager::LoadDepotNonvisibelItem(void)
{
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "ExceptionItem.lua") == false) {
		return;
	}

	m_setDpotNonvisibleItem.clear();
	GCITEMID itemID;
	LUA_BEGIN_TABLE("DEPOT_NONVISIBLE_ITEM", return)
	{
		for (int i = 1; ; ++i) {
			LUA_GET_VALUE_NOASSERT(i, itemID, break)
				m_setDpotNonvisibleItem.insert(itemID);
		}
	}
	LUA_END_TABLE(return)
}

bool KGCItemManager::IsDepotNonvisibleItem(const IN GCITEMID itemID_)
{
	std::set< GCITEMID >::const_iterator sit = m_setDpotNonvisibleItem.find(itemID_);
	return (sit != m_setDpotNonvisibleItem.end());
}


std::map<int, std::pair<GCITEMID, GCITEMUID>> KGCItemManager::GetUserCharCardList()
{
	std::map<int, std::pair<GCITEMID, GCITEMUID>> mapItemList;

	for (int i = 0; i < GC_CHAR_NUM; ++i) {
		if (g_kGlobalValue.IsOpenedChar(i) == false) continue;
		if (g_kGlobalValue.m_kUserInfo.IsCharExist(i) == true) continue;

		GCITEMID itItemID = 0;
		GCITEMUID itItemUID = 0;

		if (GetCharacterCardInfo(i, itItemID, itItemUID) == false) continue;

		mapItemList.insert(std::make_pair(i, std::make_pair(itItemID, itItemUID)));

	}

	return mapItemList;
}

int KGCItemManager::GetSlotPostionCount(const GCItem* pItemInfo_)
{
	// ½½·Ô Æ÷Áö¼ÇÀÌ ¿©·¯ÁØµ¥ °ãÄ¡´Â °æ¿ìµµ ÀÖ¾î 
	// Æ÷Áö¼Ç ¸î±ºµ¥¸¦ Â÷ÁöÇÏ´ÂÁö Ã¼Å©ÇÏ±â À§ÇØ 
	int iCount = 0;
	if (pItemInfo_ == NULL) {
		return iCount;
	}

	if (pItemInfo_->dwSlotPosition & ESP_HELMET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_JACKET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_PANTS) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_GLOVES) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_SHOES) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_MANTLE) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_WEAPON) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_UPPER_HELMET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_DOWN_HELMET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_NECKLET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_JACKET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_PANTS) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_ARM) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_FINGER) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_SHOES) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_PET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_SLOT_ITEM) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_HELMET_SOCKET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_WEAPON_SOCKET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_JACKET_SOCKET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_SLOT_EMOT) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_HELMET_SOCKET) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_TITLE) { ++iCount; }
	if (pItemInfo_->dwSlotPosition & ESP_A_EARRING) { ++iCount; }

	return iCount;
}

void KGCItemManager::SetGPAttributeRandomItemList(MAP_DWORD_SET mapList_)
{
	m_mapGPAttributeRandomItem.clear();
	m_mapGPAttributeRandomItem = mapList_;
}

void KGCItemManager::SetGPAttributeInitItemList(MAP_DWORD_SET mapList_)
{
	m_mapGPAttributeInitItem.clear();
	m_mapGPAttributeInitItem = mapList_;
}

void KGCItemManager::SetAttributeSingleRandomItemList(MAP_DWORD_SET mapList_)
{
	m_mapAttributeSingleRandomItem.clear();
	m_mapAttributeSingleRandomItem = mapList_;
}

bool KGCItemManager::CheckGPAttributeRandomItemFromInventory(OUT DWORD& dwItemID, int iGrade)
{
	if (m_mapGPAttributeRandomItem.empty()) {
		return false;
	}

	MAP_DWORD_SET::const_iterator mit = m_mapGPAttributeRandomItem.find(iGrade);
	if (mit == m_mapGPAttributeRandomItem.end()) {
		return false;
	}

	SET_DWORD::const_iterator sit = mit->second.begin();
	for (; sit != mit->second.end(); ++sit) {
		if (FindInventoryForItemID(*sit / 10)) {
			dwItemID = *sit / 10;
			return true;
		}
	}

	return false;
}

bool KGCItemManager::CheckGPAttributeInitItemFromInventory(OUT DWORD& dwItemID, int iGrade)
{
	if (m_mapGPAttributeInitItem.empty()) {
		return false;
	}

	MAP_DWORD_SET::const_iterator mit = m_mapGPAttributeInitItem.find(iGrade);
	if (mit == m_mapGPAttributeInitItem.end()) {
		return false;
	}

	SET_DWORD::const_iterator sit = mit->second.begin();
	for (; sit != mit->second.end(); ++sit) {
		if (FindInventoryForItemID(*sit / 10)) {
			dwItemID = *sit / 10;
			return true;
		}
	}

	return false;
}

bool KGCItemManager::CheckAttributeSingleRandomItemFromInventory(OUT DWORD& dwItemID, int iLevel)
{
	if (m_mapAttributeSingleRandomItem.empty()) {
		return false;
	}

	MAP_DWORD_SET::iterator mit = m_mapAttributeSingleRandomItem.begin();
	MAP_DWORD_SET::iterator mit_end = m_mapAttributeSingleRandomItem.end();
	for (; mit != mit_end; ++mit)
	{
		if (mit->first >= (DWORD)iLevel)
		{
			SET_DWORD::iterator sit = mit->second.begin();
			for (; sit != mit->second.end(); ++sit) {
				if (FindInventoryForItemID(*sit / 10)) {
					dwItemID = *sit / 10;
					return true;
				}
			}
		}
	}

	return false;

}

EItemElemental KGCItemManager::GetElementalType(EITEMATTRIBUTE eAttr)
{
	switch (eAttr)
	{
	case EIATTR_NONE:               return ITEM_ELEMENTAL_NONE;
	case EIATTR_FIRE:               return ITEM_ELEMENTAL_FIRE;
	case EIATTR_ICE:                return ITEM_ELEMENTAL_ICE;
	case EIATTR_LIGHTNING:          return ITEM_ELEMENTAL_LIGHTNING;
	case EIATTR_DARKNESS:           return ITEM_ELEMENTAL_DARK;
	case EIATTR_WIND:               return ITEM_ELEMENTAL_WIND;
	case EIATTR_CASH_SHOES_SHIELD:  return ITEM_ELEMENTAL_CASH_SHIELD;
	case EIATTR_GP_SHOES_SHIELD:    return ITEM_ELEMENTAL_GP_SHIELD;
	case EIATTR_SHINING:            return ITEM_ELEMENTAL_SHINING;
	case EIATTR_LOVE:               return ITEM_ELEMENTAL_LOVE;
	case EIATTR_ROSE:               return ITEM_ELEMENTAL_ROSE;
	case EIATTR_DEATH:              return ITEM_ELEMENTAL_DEATH;
	case EIATTR_TIME:               return ITEM_ELEMENTAL_TIME;
	case EIATTR_LIFE:               return ITEM_ELEMENTAL_LIFE;
	case EIATTR_DARK_DRAGON_RED:    return ITEM_ELEMENTAL_DARK_DRAGON_RED;
	case EIATTR_DARK_DRAGON_BLUE:   return ITEM_ELEMENTAL_DARK_DRAGON_BLUE;
	case EIATTR_FOOLS_DAY:          return ITEM_ELEMENTAL_FOOLS_DAY;
	case EIATTR_ARIES_ANKLE:        return ITEM_ELEMENTAL_ARIES_ANKLE;
	case EIATTR_TAURUS_ANKLE:       return ITEM_ELEMENTAL_TAURUS_ANKLE;
	case EIATTR_ZORRO_ANKLE:        return ITEM_ELEMENTAL_ZORRO_ANKLE;
	case EIATTR_GEMINI_ANKLE:       return ITEM_ELEMENTAL_GEMINI_ANKLE;
	case EIATTR_CANCER_ANKLE:       return ITEM_ELEMENTAL_CANCER_ANKLE;
	case EIATTR_LEO_ANKLE:          return ITEM_ELEMENTAL_LEO_ANKLE;
	case EIATTR_VIRGO_ANKLE:        return ITEM_ELEMENTAL_VIRGO_ANKLE;
	case EIATTR_LIBRA_ANKLE:        return ITEM_ELEMENTAL_LIBRA_ANKLE;
	case EIATTR_SCORPIO_ANKLE:      return ITEM_ELEMENTAL_SCORPIO_ANKLE;
	case EIATTR_SAGITTARIUS_ANKLE:  return ITEM_ELEMENTAL_SAGITTARIUS_ANKLE;
	case EIATTR_CAPRICORN_ANKLE:    return ITEM_ELEMENTAL_CAPRICORN_ANKLE;
	case EIATTR_AQUARIUS_ANKLE:     return ITEM_ELEMENTAL_AQUARIUS_ANKLE;
	case EIATTR_PISCES_ANKLE:       return ITEM_ELEMENTAL_PISCES_ANKLE;
	case EIATTR_HALLOWEEN:          return ITEM_ELEMENTAL_HALLOWEEN;
	case EIATTR_ELIA:               return ITEM_ELEMENTAL_ELIA;
	case EIATTR_ELIA_ANKLE:         return ITEM_ELEMENTAL_ELIA_ANKLE;
	case EIATTR_ARIES:              return ITEM_ELEMENTAL_ARIES;
	case EIATTR_TAURUS:             return ITEM_ELEMENTAL_TAURUS;
	case EIATTR_GEMINI:             return ITEM_ELEMENTAL_GEMINI;
	case EIATTR_CANCER:             return ITEM_ELEMENTAL_CANCER;
	case EIATTR_LEO:                return ITEM_ELEMENTAL_LEO;
	case EIATTR_VIRGO:              return ITEM_ELEMENTAL_VIRGO;
	case EIATTR_LIBRA:              return ITEM_ELEMENTAL_LIBRA;
	case EIATTR_SCORPIO:            return ITEM_ELEMENTAL_SCORPIO;
	case EIATTR_SAGITTARIUS:        return ITEM_ELEMENTAL_SAGITTARIUS;
	case EIATTR_CAPRICORN:          return ITEM_ELEMENTAL_CAPRICORN;
	case EIATTR_AQUARIUS:           return ITEM_ELEMENTAL_AQUARIUS;
	case EIATTR_PISCES:             return ITEM_ELEMENTAL_PISCES;
	case EIATTR_10TH:               return ITEM_ELEMENTAL_10TH;
	case EIATTR_DIMENSION:          return ITEM_ELEMENTAL_DIMENSION;
	case EIATTR_PEGASUS_ANKLE:      return ITEM_ELEMENTAL_PEGASUS_ANKLE;
	case EIATTR_PEGASUS_NECK:       return ITEM_ELEMENTAL_PEGASUS_NECK;
	case EIATTR_AUTUMN_NECK:       return ITEM_ELEMENTAL_AUTUMN;
	case EIATTR_CELESTIAL_MASTER_NECK:       return ITEM_ELEMENTAL_CELESTIAL_MASTER;
	case EIATTR_COLISEU_NECK:       return ITEM_ELEMENTAL_COLISEU;
	case EIATTR_FAIRY_NECK:       return ITEM_ELEMENTAL_FAIRY;
	default:                        return ITEM_ELEMENTAL_NONE;
	}

	return ITEM_ELEMENTAL_NONE;
}

std::string KGCItemManager::EnumElementalTypetoStr(EItemElemental eElementalType)
{
	switch (eElementalType)
	{
		CaseToString(ITEM_ELEMENTAL_NONE);
		CaseToString(ITEM_ELEMENTAL_FIRE);
		CaseToString(ITEM_ELEMENTAL_ICE);
		CaseToString(ITEM_ELEMENTAL_LIGHTNING);
		CaseToString(ITEM_ELEMENTAL_DARK);
		CaseToString(ITEM_ELEMENTAL_WIND);
		CaseToString(ITEM_ELEMENTAL_CASH_SHIELD);
		CaseToString(ITEM_ELEMENTAL_GP_SHIELD);
		CaseToString(ITEM_ELEMENTAL_SHINING);
		CaseToString(ITEM_ELEMENTAL_LOVE);
		CaseToString(ITEM_ELEMENTAL_ROSE);
		CaseToString(ITEM_ELEMENTAL_DEATH);
		CaseToString(ITEM_ELEMENTAL_TIME);
		CaseToString(ITEM_ELEMENTAL_LIFE);
		CaseToString(ITEM_ELEMENTAL_DARK_DRAGON_RED);
		CaseToString(ITEM_ELEMENTAL_DARK_DRAGON_BLUE);
		CaseToString(ITEM_ELEMENTAL_FOOLS_DAY);
		CaseToString(ITEM_ELEMENTAL_ARIES_ANKLE);
		CaseToString(ITEM_ELEMENTAL_TAURUS_ANKLE);
		CaseToString(ITEM_ELEMENTAL_ZORRO_ANKLE);
		CaseToString(ITEM_ELEMENTAL_GEMINI_ANKLE);
		CaseToString(ITEM_ELEMENTAL_CANCER_ANKLE);
		CaseToString(ITEM_ELEMENTAL_LEO_ANKLE);
		CaseToString(ITEM_ELEMENTAL_VIRGO_ANKLE);
		CaseToString(ITEM_ELEMENTAL_LIBRA_ANKLE);
		CaseToString(ITEM_ELEMENTAL_SCORPIO_ANKLE);
		CaseToString(ITEM_ELEMENTAL_SAGITTARIUS_ANKLE);
		CaseToString(ITEM_ELEMENTAL_CAPRICORN_ANKLE);
		CaseToString(ITEM_ELEMENTAL_AQUARIUS_ANKLE);
		CaseToString(ITEM_ELEMENTAL_PISCES_ANKLE);
		CaseToString(ITEM_ELEMENTAL_HALLOWEEN);
		CaseToString(ITEM_ELEMENTAL_ELIA);
		CaseToString(ITEM_ELEMENTAL_ELIA_ANKLE);
		CaseToString(ITEM_ELEMENTAL_ARIES);
		CaseToString(ITEM_ELEMENTAL_TAURUS);
		CaseToString(ITEM_ELEMENTAL_GEMINI);
		CaseToString(ITEM_ELEMENTAL_CANCER);
		CaseToString(ITEM_ELEMENTAL_LEO);
		CaseToString(ITEM_ELEMENTAL_VIRGO);
		CaseToString(ITEM_ELEMENTAL_LIBRA);
		CaseToString(ITEM_ELEMENTAL_SCORPIO);
		CaseToString(ITEM_ELEMENTAL_SAGITTARIUS);
		CaseToString(ITEM_ELEMENTAL_CAPRICORN);
		CaseToString(ITEM_ELEMENTAL_AQUARIUS);
		CaseToString(ITEM_ELEMENTAL_PISCES);
		CaseToString(ITEM_ELEMENTAL_10TH);
		CaseToString(ITEM_ELEMENTAL_DIMENSION);
		CaseToString(ITEM_ELEMENTAL_PEGASUS_ANKLE);
		CaseToString(ITEM_ELEMENTAL_PEGASUS_NECK);
		CaseToString(ITEM_ELEMENTAL_AUTUMN);
		CaseToString(ITEM_ELEMENTAL_CELESTIAL_MASTER);
		CaseToString(ITEM_ELEMENTAL_COLISEU);
	default: ITEM_ELEMENTAL_NONE;
	}

	return EnumToString(ITEM_ELEMENTAL_NONE);
}

void KGCItemManager::SetItemBuyInfoData(KItemBuyInfo& kItemBuyInfoData_)
{
	SetNotbuyInfoToAllItem(kItemBuyInfoData_.m_bBuyEnable);

	std::vector<std::pair<DWORD, DWORD> >::iterator vit;
	for (vit = kItemBuyInfoData_.m_vecItemRange.begin(); vit != kItemBuyInfoData_.m_vecItemRange.end(); ++vit)
	{
		std::pair<DWORD, DWORD>& rng = (*vit);
		for (DWORD itemID = rng.first; itemID <= rng.second; itemID = itemID + 10)
		{
			GCItem* pItem = GetItemData(itemID / 10);
			if (pItem)
				pItem->bNotBuyItem = !kItemBuyInfoData_.m_bBuyEnable;
		}
	}
}

void KGCItemManager::LoadRemovePartsItem(void)
{
	m_vecNoFaceItem.clear();

	KLuaManager luaMgr;
	if (!GCFUNC::LoadLuaScript(luaMgr, "CoordiShopInfo.stg", 0))
		return;

	LUA_BEGIN_TABLE("REMOVE_PARTS_COORDI_ITEM", return)
	{
		for (int i = 1; ; i++)
		{
			kRemovePartsCoordiItem kRemovePartsCoordiItemTemp;
			LUA_BEGIN_TABLE(i, break)
			{

				LUA_GET_VALUE("REMOVE_PARTS", kRemovePartsCoordiItemTemp.iRemoveParts, return);
				LUA_BEGIN_TABLE("GOODSID", return)
				{
					int iTemp = 0;
					for (int j = 1; ; j++)
					{
						LUA_GET_VALUE_NOASSERT(j, iTemp, break)
							kRemovePartsCoordiItemTemp.vecItemID.push_back(iTemp);
					}
				}
				LUA_END_TABLE(return);
			}
			m_vecNoFaceItem.push_back(kRemovePartsCoordiItemTemp);
			LUA_END_TABLE(return)
		}
	}
	LUA_END_TABLE(return) // REMOVE_PARTS_COORDI_ITEM
}

bool KGCItemManager::IsRemovePartsItem(GCITEMID itemID_, int iRemoveParts)
{
	std::vector<kRemovePartsCoordiItem>::iterator vit = m_vecNoFaceItem.begin();
	for (; vit != m_vecNoFaceItem.end(); vit++)
	{
		if (vit->iRemoveParts == iRemoveParts) {
			std::vector<GCITEMID>::iterator vit2 = find(vit->vecItemID.begin(), vit->vecItemID.end(), itemID_ * 10);
			if (vit2 != vit->vecItemID.end())
				return true;
		}
	}
	return false;
}

bool KGCItemManager::IsEquipMonsterCard(const SCharInfo& charInfo_, const GCITEMID cardID_)
{
	bool bEquip = false;

	KUserItemVector::const_iterator itemIter = charInfo_.vecItemInfo.begin();
	for (; itemIter != charInfo_.vecItemInfo.end(); ++itemIter) {
		std::vector< KSocketInfo >::const_iterator socketIter = itemIter->vecItemSocket.begin();
		for (; socketIter != itemIter->vecItemSocket.end(); ++socketIter) {
			if (socketIter->m_CardItemID == cardID_ * 10) {
				bEquip = true;
				break;
			}
		}
	}

	return bEquip;
}

bool KGCItemManager::IsSelectCharacterPet(const char cCharType_, GCITEMUID itemUID_)
{
	std::map< GCITEMUID, KPetInfo >::iterator mit = g_MyD3D->m_mapPetInfo.find(itemUID_);
	if (mit == g_MyD3D->m_mapPetInfo.end()) {
		return false;
	}

	if (mit->second.m_cCharType == -1) { // °ø¿ë ÆêÀÓ
		return true;
	}
	else {
		return mit->second.m_cCharType == cCharType_;
	}
}

// Ä³½¬ or °¡»óÄ³½¬·Î ±¸¸Å
void KGCItemManager::BuyCashItemS6(GCITEMID dwItemID_, bool bMsgBox)
{
	// ÇöÀç »óÇ° Á¤º¸¸¦ ±â¾ï
	m_dwCurrentGoodsID = dwItemID_;

	if (bMsgBox)
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false);
	else
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}

void KGCItemManager::BuyCashItemQuantity(GCITEMID dwItemID_, bool bForce)
{
	m_dwCurrentGoodsID = dwItemID_;
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NEW_COMBO_BOX_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwCurrentGoodsID, KBuySellItemReq::EMT_VIRTUALCASH, true, bForce);
}

void KGCItemManager::VerifyCharCardInfo(void)
{
	std::map<int, std::pair<GCITEMID, GCITEMUID>> mapCharItemList = GetUserCharCardList();

	for (std::map<int, std::pair<GCITEMID, GCITEMUID>>::iterator mit = mapCharItemList.begin(); mit != mapCharItemList.end(); ++mit) {
		Result_New_Character = INT_MAX;
		KP2P::GetInstance()->Send_NewCharacterReq(mit->second.first, mit->second.second);
		if (false == g_MyD3D->WaitForServerAck(Result_New_Character, INT_MAX, 3000, INT_MAX))
		{
			// Å¸ÀÓ¾Æ¿ô Ã³¸®
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"");
		}
	}
}

int KGCItemManager::GetEquipLevelFromMonsterCard(DWORD dwItemID)
{
	GCItem* pItem = GetItemData(dwItemID);

	if (!pItem)
		return -1;
	if (pItem->eItemKind != GIK_MONSTER_CARD)
		return -1;

	return pItem->iNeedLevelForEquippingItem;
}

bool KGCItemManager::IsCoordiSlot(DWORD dwSlotPosition)
{
	// ½½·Ô Áßº¹ Ã¼Å© ¸øÇÑ´Ù.
	switch (dwSlotPosition)
	{
	case ESP_SET_ITEM:
	case ESP_HELMET:
	case ESP_JACKET:
	case ESP_PANTS:
	case ESP_GLOVES:
	case ESP_SHOES:
	case ESP_MANTLE:
	case ESP_WEAPON:
	case ESP_A_UPPER_HELMET:
	case ESP_A_DOWN_HELMET:
	case ESP_A_JACKET:
	case ESP_A_PANTS:
	case ESP_A_ARM:
	case ESP_A_SHOES:
	case ESP_A_NECKLET:
	case ESP_TITLE:
		return true;

	case ESP_NONE:
	case ESP_A_FINGER:
	case ESP_SLOT_ITEM:
	case ESP_HELMET_SOCKET:
	case ESP_WEAPON_SOCKET:
	case ESP_JACKET_SOCKET:
	case ESP_SLOT_EMOT:
	case ESP_A_HELMET_SOCKET:
	case ESP_A_EARRING:
		return false;
	default:
		return false;
	}

	return false;
}

int KGCItemManager::EnableAbilityCoordi(GCITEMUID _itemUID)
{
	KItem* kItem = GetInventoryItem(_itemUID);
	if (!kItem)
		return EMCOORDI_COMPOSE_ERR0;

	GCItem* pItem = GetItemData(kItem->m_ItemID);
	if (!pItem)
		return EMCOORDI_COMPOSE_ERR0;

	if (!kItem->CheckItemType(KItem::TYPE_LOOK))
		return EMCOORDI_COMPOSE_ERR1;

	// ÄÚµð°¡ ¾Æ´Ô.
	if (pItem->eItemKind != GIK_COORDI_ITEM)
		return EMCOORDI_COMPOSE_ERR1;

	// ´É·ÂÄ¡°¡ ¾øÀ½.
	if (kItem->m_vecAttribute.empty())
		return EMCOORDI_COMPOSE_ERR2;

	// ±â°£Á¦ÀÓ.
	if (!kItem->IsUnlimited())
		return EMCOORDI_COMPOSE_ERR3;

#ifdef COMMON_MANUAL
	DWORD dwCommonCharType = ECT_ALL;
#else
	DWORD dwCommonCharType = 0;
#
	for (int i = 0; i < GC_CHAR_NUM; ++i) {
		dwCommonCharType = dwCommonCharType | (1 << i);
	}
#endif

	// °ø¿ëÀº ¾ÈµÊ.
	if (pItem->dwCharType == dwCommonCharType)
		return EMCOORDI_COMPOSE_ERR4;

	// ÀåÂøÁßÀÎÁö Ã¼Å©
	if (CheckEquipItem(_itemUID))
		return EMCOORDI_COMPOSE_ERR5;

	return EMCOORDI_COMPOSE_OK;
}

int KGCItemManager::EnableDesignCoordi(GCITEMUID _itemUID)
{
	KItem* kItem = GetInventoryItem(_itemUID);
	if (!kItem)
		return EMCOORDI_COMPOSE_ERR0;

	GCItem* pItem = GetItemData(kItem->m_ItemID);
	if (!pItem)
		return EMCOORDI_COMPOSE_ERR0;

	// ÄÚµð°¡ ¾Æ´Ô
	if (!kItem->CheckItemType(KItem::TYPE_LOOK))
		return EMCOORDI_COMPOSE_ERR1;

	// ±â°£Á¦ÀÓ.
	if (!kItem->IsUnlimited())
		return EMCOORDI_COMPOSE_ERR3;

	return EMCOORDI_COMPOSE_OK;
}

int KGCItemManager::DoCoordiCompose(GCITEMUID _AbilityItemUID, GCITEMUID _DesignItemUID)
{
	int emResult = EMCOORDI_COMPOSE_OK;

	if (_AbilityItemUID == 0 || _DesignItemUID == 0)
		return EMCOORDI_COMPOSE_ERR0;

	emResult = EnableAbilityCoordi(_AbilityItemUID);
	if (emResult != EMCOORDI_COMPOSE_OK)
		return emResult;

	emResult = EnableDesignCoordi(_DesignItemUID);
	if (emResult != EMCOORDI_COMPOSE_OK)
		return emResult;

	if (_AbilityItemUID == _DesignItemUID)
		return EMCOORDI_COMPOSE_ERR0;

	KItem* kItemAblility = GetInventoryItem(_AbilityItemUID);
	if (!kItemAblility)
		return EMCOORDI_COMPOSE_ERR0;

	GCItem* pItemAbility = GetItemData(kItemAblility->m_ItemID);
	if (!pItemAbility)
		return EMCOORDI_COMPOSE_ERR0;

	KItem* kItemDesign = GetInventoryItem(_DesignItemUID);
	if (!kItemAblility)
		return EMCOORDI_COMPOSE_ERR0;

	GCItem* pItemDesign = GetItemData(kItemDesign->m_ItemID);
	if (!pItemDesign)
		return EMCOORDI_COMPOSE_ERR0;

	// ½½·ÔÀÌ °°ÀºÁö Ã¼Å©
	if (pItemAbility->dwSlotPosition != pItemDesign->dwSlotPosition)
		return EMCOORDI_COMPOSE_ERR6;

	// ÀüÁ÷ Â÷¼ö Ã¼Å©
	if (pItemAbility->iNeedChangeJobLevelForEquippingItem != pItemDesign->iNeedChangeJobLevelForEquippingItem)
		return EMCOORDI_COMPOSE_ERR7;

	// ¼ö·® Ã¼Å©
	if (GetInventoryItemDuration(ITEM_COORDI_COMPOSE_COUPON) <= 0)
		return EMCOORDI_COMPOSE_ERR8;


	SiKP2P()->Send_CoordiComposeReq(_AbilityItemUID, _DesignItemUID);

	return EMCOORDI_COMPOSE_OK;
}

std::vector<GCITEMUID> KGCItemManager::GetEnableCoordiComposeList(bool bDesignCoordi, GCITEMUID _AbilityItemUID, GCITEMUID _DesignItemUID)
{
	std::vector<GCITEMUID> vecOriginList;
	std::vector<GCITEMUID> vecFilterList;

	vecOriginList = g_pItemMgr->GetInventoryCoordiItemUIDList(g_pItemMgr->GetCharFlag(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType));
	g_pItemMgr->FilterEquipedItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), vecOriginList, true);

	if (bDesignCoordi)
	{
		for (int i = 0; i < (int)vecOriginList.size(); ++i)
		{
			if (EnableDesignCoordi(vecOriginList[i]) != EMCOORDI_COMPOSE_OK)
				continue;

			if (_AbilityItemUID == vecOriginList[i])
				continue;

			// ´É·ÂÄ¡ ÄÚµð°¡ ¼³Á¤µÇ¾î ÀÖÀ¸¸é °Å±â¿¡ ¸Â´Â¾ÆÀÌÅÛ¸¸ ³ª¿Â´Ù.
			if (_AbilityItemUID != 0)
			{
				KItem* kItemAblility = GetInventoryItem(_AbilityItemUID);
				if (!kItemAblility)
					continue;

				GCItem* pItemAbility = GetItemData(kItemAblility->m_ItemID);
				if (!pItemAbility)
					continue;

				KItem* kItemDesign = GetInventoryItem(vecOriginList[i]);
				if (!kItemAblility)
					continue;

				GCItem* pItemDesign = GetItemData(kItemDesign->m_ItemID);
				if (!pItemDesign)
					continue;

				// ½½·ÔÀÌ °°ÀºÁö Ã¼Å©
				if (pItemAbility->dwSlotPosition != pItemDesign->dwSlotPosition)
					continue;

				// ÀüÁ÷ Â÷¼ö Ã¼Å©
				if (pItemAbility->iNeedChangeJobLevelForEquippingItem != pItemDesign->iNeedChangeJobLevelForEquippingItem)
					continue;

			}

			vecFilterList.push_back(vecOriginList[i]);

		}
	}
	else
	{
		for (int i = 0; i < (int)vecOriginList.size(); ++i)
		{
			if (EnableAbilityCoordi(vecOriginList[i]) != EMCOORDI_COMPOSE_OK)
				continue;

			if (_DesignItemUID == vecOriginList[i])
				continue;

			// ´É·ÂÄ¡ ÄÚµð°¡ ¼³Á¤µÇ¾î ÀÖÀ¸¸é °Å±â¿¡ ¸Â´Â¾ÆÀÌÅÛ¸¸ ³ª¿Â´Ù.
			if (_DesignItemUID != 0)
			{
				KItem* kItemAblility = GetInventoryItem(vecOriginList[i]);
				if (!kItemAblility)
					continue;

				GCItem* pItemAbility = GetItemData(kItemAblility->m_ItemID);
				if (!pItemAbility)
					continue;

				KItem* kItemDesign = GetInventoryItem(_DesignItemUID);
				if (!kItemAblility)
					continue;

				GCItem* pItemDesign = GetItemData(kItemDesign->m_ItemID);
				if (!pItemDesign)
					continue;

				// ½½·ÔÀÌ °°ÀºÁö Ã¼Å©
				if (pItemAbility->dwSlotPosition != pItemDesign->dwSlotPosition)
					continue;

				// ÀüÁ÷ Â÷¼ö Ã¼Å©
				if (pItemAbility->iNeedChangeJobLevelForEquippingItem != pItemDesign->iNeedChangeJobLevelForEquippingItem)
					continue;

			}

			vecFilterList.push_back(vecOriginList[i]);
		}
	}

	return vecFilterList;
}

void KGCItemManager::CoordiGradeUpgradeInfoReq()
{
	if (m_sCoordiUpgradeInfo.m_bGetInfo)
		return;

	// ¼­¹ö¿¡ ÀÌº¥Æ® Á¤º¸ ¿äÃ»
	Result_ReceiveCoordiUpgradeInfo = INT_MAX;
	KP2P::GetInstance()->Send_CoordiGradeUpgradeInfoReq();
	g_MyD3D->WaitForServerAck(Result_ReceiveCoordiUpgradeInfo, INT_MAX, 3000, TIME_OUT_VALUE);

	return;

}

void KGCItemManager::SetCoordiUpgradeInfo(KEVENT_COORDI_GRADE_UPGRADE_INFO_ACK& kRecv)
{
	//     GCITEMID                m_GradeUpgradeItemID; // µî±Þ ¾÷±×·¹ÀÌµå ¾ÆÀÌÅÛ.
	//     std::map<char,int>      m_mapGradeUseCount;   // µî±Þ ¾÷±×·¹ÀÌµå½Ã ¾ÆÀÌÅÛ »ç¿ë °¹¼ö
	//     int                     m_nEnableItemType;    // Çã¿ë ItemType.
	//     std::set<GCITEMID>      m_setUpgradeBanItem;  // ¾÷±×·¹ÀÌµå ºÒ°¡ ¾ÆÀÌÅÛ.


	m_sCoordiUpgradeInfo.m_GradeUpgradeItemID = kRecv.m_GradeUpgradeItemID;
	m_sCoordiUpgradeInfo.m_mapGradeUseCount = kRecv.m_mapGradeUseCount;
	m_sCoordiUpgradeInfo.m_nEnableItemType = kRecv.m_nEnableItemType;
	m_sCoordiUpgradeInfo.m_setUpgradeBanItem = kRecv.m_setUpgradeBanItem;
	m_sCoordiUpgradeInfo.m_bGetInfo = true;
}

int KGCItemManager::EnableCoordiUpgrade(GCITEMUID _itemUID)
{
	KItem* kItem = GetInventoryItem(_itemUID);
	if (!kItem)
		return EMCOORDI_UPGRADE_ERR0;

	GCItem* pItem = GetItemData(kItem->m_ItemID);
	if (!pItem)
		return EMCOORDI_UPGRADE_ERR0;

	if (!kItem->CheckItemType(KItem::TYPE_LOOK))
		return EMCOORDI_UPGRADE_ERR1;

	// ÄÚµð°¡ ¾Æ´Ô.
	if (pItem->eItemKind != GIK_COORDI_ITEM)
		return EMCOORDI_UPGRADE_ERR1;

	// ´É·ÂÄ¡°¡ ¾øÀ½.
	if (kItem->m_vecAttribute.empty())
		return EMCOORDI_UPGRADE_ERR2;

	// ÀåÂøÁßÀÎÁö Ã¼Å©
	if (CheckEquipItem(_itemUID))
		return EMCOORDI_UPGRADE_ERR3;

	if (kItem->m_cGradeID == KItem::GRADE_MYSTIC)
		return EMCOORDI_UPGRADE_ERR4;

	if (m_sCoordiUpgradeInfo.m_setUpgradeBanItem.find(pItem->dwGoodsID * 10) !=
		m_sCoordiUpgradeInfo.m_setUpgradeBanItem.end())
		return EMCOORDI_UPGRADE_ERR5;

	return EMCOORDI_UPGRADE_OK;
}

int KGCItemManager::DoCoordiUpgrade(GCITEMUID _itemUID)
{
	int emResult = EMCOORDI_UPGRADE_OK;

	emResult = EnableCoordiUpgrade(_itemUID);
	if (emResult != EMCOORDI_UPGRADE_OK)
		return emResult;

	KItem* kItem = GetInventoryItem(_itemUID);
	if (!kItem)
		return EMCOORDI_UPGRADE_ERR0;

	// ¼ö·® Ã¼Å©
	if (GetInventoryItemDuration((m_sCoordiUpgradeInfo.m_GradeUpgradeItemID / 10)) <= 0)
		return EMCOORDI_UPGRADE_ERR6;

	SiKP2P()->Send_CoordiGradeUpgradeReq(_itemUID, m_sCoordiUpgradeInfo.m_GradeUpgradeItemID, kItem->m_cGradeID + 1);

	return EMCOORDI_UPGRADE_OK;
}

void KGCItemManager::GetEnableCoordiUpgradeList(std::vector<GCITEMUID>& vecItemList)
{
	std::vector<GCITEMUID> vecOriginList;

	vecItemList.clear();

	vecOriginList = g_pItemMgr->GetInventoryCoordiItemUIDList(g_pItemMgr->GetCharFlag(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType));
	g_pItemMgr->FilterEquipedItem(&g_MyD3D->m_TempPlayer.GetCurrentChar(), vecOriginList, true);

	for (int i = 0; i < (int)vecOriginList.size(); ++i)
	{
		if (EnableCoordiUpgrade(vecOriginList[i]) != EMCOORDI_UPGRADE_OK)
			continue;

		vecItemList.push_back(vecOriginList[i]);
	}

	return;
}

bool KGCItemManager::IsChangeMeshCoordi(const int iSetItemID_)
{
	std::vector<int> vecParts;

	//µî·ÏµÈ ¼ÂÆ® ¾ÆÀÌÅÛÁß ³»°¡ Ã£´Â ¾ÆÀÌÅÛÀÌ ÀÖ´Ù¸é vecParts¿¡ ÀúÀå.
	for (int i = 0; i < (int)m_SetList.size(); i++)
	{
		if (iSetItemID_ == m_SetList[i].iSetNum)
			vecParts = m_SetList[i].vecParts;
	}

	if (vecParts.empty())
		return false;

	std::vector<int>::iterator itrParts = vecParts.begin();

	//³» Ä³¸¯ÅÍ°¡ ¸ðµç ÆÄÃ÷¸¦ Âø¿ëÇÏ°í ÀÖ´Ù¸é TRUE¸¦ ¸®ÅÏ, ¾Æ´Ï¸é FALSE¸¦ ¸®ÅÏ
	for (; itrParts != vecParts.end(); ++itrParts)
	{
		if (!CheckEquipItemForItemID(&g_MyD3D->m_TempPlayer.GetCurrentChar(), (*itrParts), true))
			return false;
	}

	return true;
}

void KGCItemManager::LoadSkillScroolList()
{
	m_mapSkillScrool.clear();
	KLuaManager     luaMgr;

	if (false == GCFUNC::LoadLuaScript(luaMgr, "SkillScroolList.lua"))
		return;

	LUA_BEGIN_TABLE("SkillScroolList", return)
	{
		for (int i = 1; ; ++i)
		{
			GCITEMID dwGoodsID = 0;
			int iSkillID = 0;
			LUA_BEGIN_TABLE(i, break)
				LUA_GET_VALUE_NOASSERT(1, dwGoodsID, break)
				LUA_GET_VALUE_NOASSERT(2, iSkillID, break)
				m_mapSkillScrool.insert(std::pair<GCITEMID, int>(dwGoodsID, iSkillID));
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(;)
}

int KGCItemManager::GetSkillID(GCITEMID dwGoodsID)
{
	std::map< GCITEMID, int>::iterator mit = m_mapSkillScrool.find(dwGoodsID);

	if (mit == m_mapSkillScrool.end())
		return -1;

	return mit->second;
}

bool KGCItemManager::IsSkillScroolList(DWORD dwGoodsID)
{
	return m_mapSkillScrool.end() != m_mapSkillScrool.find(dwGoodsID);
}

// Ä³½¬ or °¡»óÄ³½¬·Î ¼±¹°
void KGCItemManager::PresentCashItemS6(GCITEMID dwItemID_, bool bMsgBox)
{
	// ÇöÀç »óÇ° Á¤º¸¸¦ ±â¾ï
	m_dwCurrentGoodsID = dwItemID_;

	if (bMsgBox)
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false);
	else
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
	/*if( bMsgBox )
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PRESENT_CASH_ITEM_WND, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
	else
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PRESENT_CASH_ITEM_WND, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );*/
}

void KGCItemManager::SetLookExtendItemInfo(KEVENT_LOOK_INVENTORY_EXTEND_INFO_NOT& kRecv)
{
	m_mapLookExtendItemInfo.clear();
	m_mapLookExtendItemInfo.swap(kRecv.m_mapInventoryExtendInfo);
	m_nMaxLookInventorySlot = kRecv.m_nMaxSlot;
}

bool KGCItemManager::IsLookExtendItem(GCITEMID dwGoodsID)
{
	std::map<GCITEMID, int>::iterator mit = m_mapLookExtendItemInfo.find(dwGoodsID * 10);

	if (mit != m_mapLookExtendItemInfo.end())
	{
		return true;
	}

	return false;
}

int KGCItemManager::GetLookExtendSize(GCITEMID dwGoodsID)
{
	std::map<GCITEMID, int>::iterator mit = m_mapLookExtendItemInfo.find(dwGoodsID * 10);

	if (mit != m_mapLookExtendItemInfo.end())
	{
		return mit->second;
	}

	return 0;
}

void KGCItemManager::GetInventoryPetGlyph(SCharInfo* pCharInfo, OUT std::vector<GCITEMUID>& vecGlyphs)
{
	auto glyphMap = SiGCPetMagicStone()->GetPetGlyphMap();
	for (auto it : glyphMap)
	{
		auto PetGlyphItem = GetInventoryItemFromID(it.first / 10);
		if (PetGlyphItem != nullptr)
		{
			if (IsEquipedItem(pCharInfo, PetGlyphItem->m_ItemUID))
			{
				continue;
			}

			vecGlyphs.push_back(PetGlyphItem->m_ItemUID);
		}
	}
}

bool KGCItemManager::IsGPCoupon(GCITEMID dwGoodsID)
{
	switch (dwGoodsID)
	{
	case ITEM_10000_GP_COUPON:
	case ITEM_500_GP_COUPON:
	case ITEM_20000_GP_COUPON:
	case ITEM_1000_GP_COUPON:
	case ITEM_2000_GP_COUPON:
	case ITEM_NEW_YEAR_MONEY_1000GP:
	case ITEM_NEW_YEAR_MONEY_5000GP:
	case ITEM_NEW_YEAR_MONEY_10000GP:
	case ITEM_NEW_YEAR_MONEY_50000GP:
	case ITEM_5000_GP_COUPON:
	case ITEM_1000000_GP_COUPON:
	case ITEM_1000000_GP_COUPON2:
	case ITEM_EASTER_EVENT_BROKEN_EGG1:
	case ITEM_EASTER_EVENT_BROKEN_EGG2:
	case ITEM_EASTER_EVENT_BROKEN_EGG3:
	case ITEM_EASTER_EVENT_BROKEN_EGG4:
	case ITEM_100_GP_COUPON:
	case ITEM_1000_GP_COUPON2:
	case ITEM_10000_GP_COUPON2:
	case ITEM_100000_GP_COUPON:
		return true;
	default:
		return false;
	}
	return false;
}

bool KGCItemManager::IsVPCoupon(GCITEMID dwGoodsID)
{
	switch (dwGoodsID)
	{
	case ITEM_VP1_COUPON:
	case ITEM_VP10_COUPON:
	case ITEM_VP100_COUPON:
	case ITEM_VP500_COUPON:
	case ITEM_VP1000_COUPON:
	case ITEM_VP10000_COUPON:
	case ITEM_VP1000000_COUPON:
	case ITEM_WATER_FESTIVAL_VP1000:
		return true;
	default:
		return false;
	}
	return false;
}