#include "stdafx.h"
#include "GCSellInfo.h"

IMPLEMENT_CLASSNAME(KGCSellInfo);
IMPLEMENT_WND_FACTORY(KGCSellInfo);
IMPLEMENT_WND_FACTORY_NAME(KGCSellInfo, "gc_sellinfo");

KGCSellInfo::KGCSellInfo(void)
{
	m_pkNumSellItem = NULL;
	LINK_CONTROL("num_sellitem", m_pkNumSellItem);
	m_pkNumRewardGP = NULL;
	LINK_CONTROL("num_gp", m_pkNumRewardGP);
	m_pkNumRewardGem = NULL;
	LINK_CONTROL("num_gem", m_pkNumRewardGem);
	m_pkNumRewardCrystal = NULL;
	LINK_CONTROL("num_cristal", m_pkNumRewardCrystal);

	m_pkBtnSell = NULL;
	LINK_CONTROL("btn_sell", m_pkBtnSell);
	m_pkBtnExit = NULL;
	LINK_CONTROL("btn_exit", m_pkBtnExit);

	m_pkOwnGP = NULL;
	LINK_CONTROL("gp_static", m_pkOwnGP);
	m_pkOwnGem = NULL;
	LINK_CONTROL("gem_static", m_pkOwnGem);
	m_pkOwnPoint = NULL;
	LINK_CONTROL("point_static", m_pkOwnPoint);

	m_pkOwnCrystal = NULL;
	LINK_CONTROL("cristal_static", m_pkOwnCrystal);

	m_pkHeroCoinText = NULL;
	LINK_CONTROL("herocoin_static", m_pkHeroCoinText);
}

KGCSellInfo::~KGCSellInfo(void)
{
}

void KGCSellInfo::OnCreate(void)
{
	m_pkNumSellItem->InitState(true);
	m_pkNumRewardGP->InitState(true);
	m_pkNumRewardGem->InitState(true);
	m_pkNumRewardCrystal->InitState(false);

	m_pkBtnSell->InitState(true, true);
	m_pkBtnSell->DisconnectAllEventProcedure();
	m_pkBtnSell->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSellInfo::OnClickSell);
	m_pkBtnExit->InitState(true, true);
	m_pkBtnExit->DisconnectAllEventProcedure();
	m_pkBtnExit->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSellInfo::OnClickExit);

	m_pkOwnGP->InitState(true);
	m_pkOwnGem->InitState(true);
	m_pkOwnPoint->InitState(true);
	m_pkOwnCrystal->InitState(true);

	if (m_pkHeroCoinText)
	{
		m_pkHeroCoinText->InitState(true);
		m_pkOwnCrystal->InitState(false);
	}
}

void KGCSellInfo::OnInitialize()
{
	InitNumbers();
	Update();
}

void KGCSellInfo::OnClickSell()
{
	SpeakToActionListener(KActionEvent(this, KGCUIScene::D3DWE_SELL_ITEM));
}

void KGCSellInfo::OnClickExit()
{
	SpeakToActionListener(KActionEvent(this, KGCUIScene::D3DWE_SELL_EXIT));
}

void KGCSellInfo::Update()
{
	InitNumbers();

	m_pkOwnGP->SetText(g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().GetCurrentGP());
	m_pkOwnGem->SetText(g_MyD3D->m_kItemMgr.GetVipNum());
	m_pkOwnPoint->SetText(g_kGlobalValue.m_iGCPoint);
	m_pkOwnCrystal->SetText(g_MyD3D->m_kItemMgr.GetCrystalNum());

	if (m_pkHeroCoinText)
		m_pkHeroCoinText->SetText(g_MyD3D->m_kItemMgr.GetHeroCoin());

	m_pkBtnSell->Lock(m_mapPriceInfo.empty());

	std::map< GCITEMUID, std::pair<int, int> >::iterator mit = m_mapPriceInfo.begin();
	for (; mit != m_mapPriceInfo.end(); ++mit) {
		switch (mit->second.second) {
		case EMT_GAMEPOINT:
			SetRewardGP(GetRewardGP() + mit->second.first);
			break;
		case EMT_GEM:
			SetRewardGP(GetRewardGP() + mit->second.first);
			break;
		case EMT_CRYSTAL:
			SetRewardCrystal(GetRewardCrystal() + mit->second.first);
			break;
		default:
			ASSERT(!"it can't be rewards");
			return;
		}
	}
	SetSellItemNum(static_cast<int>(m_mapPriceInfo.size()));
}

void KGCSellInfo::SetSellItemNum(int iNum)
{
	if (iNum < 0)
		iNum = 0;

	m_pkNumSellItem->SetNumber(iNum);
}

void KGCSellInfo::SetRewardGP(int iGP_)
{
	if (iGP_ < 0)
		iGP_ = 0;

	m_pkNumRewardGP->SetNumber(iGP_);
}

void KGCSellInfo::SetRewardGem(int iGem_)
{
	if (iGem_ < 0)
		iGem_ = 0;

	m_pkNumRewardGem->SetNumber(iGem_);
}

void KGCSellInfo::SetRewardCrystal(int iCrystal_)
{
	if (iCrystal_ < 0)
		iCrystal_ = 0;

	m_pkNumRewardCrystal->SetNumber(iCrystal_);
}

void KGCSellInfo::InitNumbers()
{
	SetSellItemNum(0);
	SetRewardGP(0);
	SetRewardGem(0);
	SetRewardCrystal(0);
}

void KGCSellInfo::AddReward(GCITEMUID itemUID_, int iPrice_, int iType_)
{
	m_mapPriceInfo[itemUID_] = std::pair<int, int>(iPrice_, iType_);
	Update();
}

void KGCSellInfo::RemoveReward(GCITEMUID itemUID_)
{
	std::map< GCITEMUID, std::pair<int, int> >::iterator mit;
	mit = m_mapPriceInfo.find(itemUID_);

	if (mit != m_mapPriceInfo.end()) {
		m_mapPriceInfo.erase(mit);
	}
	Update();
}

void KGCSellInfo::InitPriceList()
{
	m_mapPriceInfo.clear();
	Update();
}