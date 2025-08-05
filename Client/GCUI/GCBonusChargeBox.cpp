#include "stdafx.h"
#include "GCBonusChargeBox.h"
//
//

#include "KGCElvis.h"
//
//
#include "GCItemManager.h"

IMPLEMENT_CLASSNAME(KGCBonusChargeBox);
IMPLEMENT_WND_FACTORY(KGCBonusChargeBox);
IMPLEMENT_WND_FACTORY_NAME(KGCBonusChargeBox, "gc_bonus_charge_box");

KGCBonusChargeBox::KGCBonusChargeBox(void)
{
	m_pkChargeTitle = NULL;
	m_pkChargeError = NULL;
	m_pkYes = NULL;
	m_pkNo = NULL;
	m_pkBonusNumUp = NULL;
	m_pkBonusNumDown = NULL;
	m_pkChargeNum = NULL;
	m_nChargeFactor = 0;
	m_iPressCount = 0;
	m_nMaxChargeNum = MAX_BONUS_NUM;
	m_dwUID = 0;
	m_pkCurrentNum = NULL;

	LINK_CONTROL("bonus_num", m_pkChargeNum);
	LINK_CONTROL("bonus_box_yes_button", m_pkYes);
	LINK_CONTROL("bonus_box_no_button", m_pkNo);
	LINK_CONTROL("bonus_num_up", m_pkBonusNumUp);
	LINK_CONTROL("bonus_num_down", m_pkBonusNumDown);
	LINK_CONTROL("bonus_charge_string", m_pkChargeTitle);
	LINK_CONTROL("bonus_charge_error", m_pkChargeError);
	LINK_CONTROL("current_num", m_pkCurrentNum);
}

KGCBonusChargeBox::~KGCBonusChargeBox(void)
{
	// empty
}

void KGCBonusChargeBox::OnCreate(void)
{
	m_pkChargeTitle->SetText(g_pkStrLoader->GetString(STR_ID_BONUS_QUERY));
	m_pkChargeTitle->InitState(true);

	m_pkYes->InitState(true, true, this);
	m_pkYes->SetHotKey(DIK_RETURN);
	m_pkYes->SetHotKey(DIK_NUMPADENTER);
	m_pkNo->InitState(true, true, this);
	m_pkNo->SetHotKey(DIK_ESCAPE);

	m_nMaxChargeNum = SiKGCElvis()->GetMaxChargePoint();

	m_pkChargeError->SetText(g_pkStrLoader->GetReplacedString(STR_ID_BONUS_CHARGING_MAXIMUM, "i", m_nMaxChargeNum));
	m_pkChargeError->InitState(true);
	m_pkChargeError->SetFontColor(D3DCOLOR_ARGB(255, 255, 0, 0));

	m_pkBonusNumUp->InitState(true, true, this);
	m_pkBonusNumUp->SetHotKey(DIK_UP);
	m_pkBonusNumDown->InitState(true, true, this);
	m_pkBonusNumDown->SetHotKey(DIK_DOWN);

	SetChargeFactor(0);

	m_pkCurrentNum->SetText(L"0");
}

void KGCBonusChargeBox::ActionPerformed(const KActionEvent& event)
{
	KItem* pInvenItem = NULL;

	g_pItemMgr->FindInventory(m_dwUID, &pInvenItem);

	switch (event.m_dwCode)
	{
	case KD3DWnd::D3DWE_CURSOR_ENTER:
	{
		g_KDSound.Play("30");
		break;
	}
	case KD3DWnd::D3DWE_BUTTON_DOWN:
	{
		g_KDSound.Play("31");
		break;
	}
	case KD3DWnd::D3DWE_BUTTON_PRESS:
	{
		if (++m_iPressCount > 10)
		{
			if (event.m_pWnd == m_pkBonusNumUp)
			{
				if (pInvenItem && m_nChargeFactor < m_nMaxChargeNum && m_nChargeFactor < (UINT)pInvenItem->m_nCount)
				{
					SetChargeFactor(++m_nChargeFactor);
				}
			}
			else if (event.m_pWnd == m_pkBonusNumDown)
			{
				if (m_nChargeFactor > 0)
				{
					SetChargeFactor(--m_nChargeFactor);
				}
			}
		}
		break;
	}
	case KD3DWnd::D3DWE_BUTTON_CLICK:
	{
		m_iPressCount = 0;

		if (event.m_pWnd == m_pkYes)
		{
			SpeakToActionListener(KActionEvent(this, GetOKCode()));
		}
		else if (event.m_pWnd == m_pkNo)
		{
			SpeakToActionListener(KActionEvent(this, BCB_NO));
		}
		else if (event.m_pWnd == m_pkBonusNumUp)
		{
			if (pInvenItem && m_nChargeFactor < m_nMaxChargeNum && m_nChargeFactor < (UINT)pInvenItem->m_nCount)
			{
				SetChargeFactor(++m_nChargeFactor);
			}
		}
		else if (event.m_pWnd == m_pkBonusNumDown)
		{
			if (m_nChargeFactor > 0)
			{
				SetChargeFactor(--m_nChargeFactor);
			}
		}
		break;
	}
	}
}

void KGCBonusChargeBox::OnCreateComplete(void)
{
	SpeakToActionListener(KActionEvent(this, BCB_CREATED));
}

void KGCBonusChargeBox::OnDestroyComplete(void)
{
	SpeakToActionListener(KActionEvent(this, BCB_DESTROYED));
}

void KGCBonusChargeBox::SetChargeFactor(UINT nChargeFactor)
{
	m_nChargeFactor = nChargeFactor;

	std::wstringstream stm;
	stm << m_nChargeFactor;
	m_pkChargeNum->SetText(stm.str());
}

DWORD KGCBonusChargeBox::GetOKCode() const
{
	KItem* pInvenItem = NULL;

	if (g_pItemMgr->FindInventory(m_dwUID, &pInvenItem) == false)
		return BCB_NO;

	if (m_nChargeFactor > (UINT)pInvenItem->m_nCount)
		return BCB_NO;

	if (m_nChargeFactor <= 0)
		return BCB_NO;

	return BCB_YES;
}

void KGCBonusChargeBox::SetChargingItem(GCITEMUID dwUID_)
{
	m_dwUID = dwUID_;

	std::wstringstream stm;
	KItem* pInvenItem = NULL;

	if (g_pItemMgr->FindInventory(m_dwUID, &pInvenItem))
		stm << g_pkStrLoader->GetReplacedString(STR_ID_BONUS_ITEM_CURRENT_NUM, "i", (int)pInvenItem->m_nCount);

	m_pkCurrentNum->SetText(stm.str());
}

bool KGCBonusChargeBox::InitDialog(IN const KDialogInfo& kInfo_)
{
	this->SetChargingItem(kInfo_.m_lParam);
	return true;
}