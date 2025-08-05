#include "stdafx.h"
#include "GCGemBuyBox.h"
//
//

//
#include "../MyD3D.h"

//

IMPLEMENT_CLASSNAME(KGCGemBuyBox);
IMPLEMENT_WND_FACTORY(KGCGemBuyBox);
IMPLEMENT_WND_FACTORY_NAME(KGCGemBuyBox, "gc_gem_buy_box");

KGCGemBuyBox::KGCGemBuyBox(void)
{
	m_pkTxt = NULL;
	m_pkYes = NULL;
	m_pkNo = NULL;
	m_pkGemNumUp = NULL;
	m_pkGemNumDown = NULL;
	m_pkGemNum = NULL;
	m_pkGPPrice = NULL;

	m_iGemNum = 0;
	m_iPressCount = 0;

	LINK_CONTROL("gem_box_text", m_pkTxt);
	LINK_CONTROL("gem_box_yes_button", m_pkYes);
	LINK_CONTROL("gem_box_no_button", m_pkNo);
	LINK_CONTROL("gem_num_up", m_pkGemNumUp);
	LINK_CONTROL("gem_num_down", m_pkGemNumDown);
	LINK_CONTROL("gem_num", m_pkGemNum);
	LINK_CONTROL("gp_price", m_pkGPPrice);
}

KGCGemBuyBox::~KGCGemBuyBox(void)
{
	// empty
}

void KGCGemBuyBox::OnCreate(void)
{
	m_iMaxGemNum = MAX_GP_VALUE / g_MyD3D->m_kItemMgr.GetItemData(GEM_ITEM_ID)->dwPrice;

	m_pkTxt->SetSelfInputCheck(false);
	m_pkTxt->SetText(g_pkStrLoader->GetString(STR_ID_BUY_GEM_QUERY));

	m_pkGemNum->SetSelfInputCheck(false);
	m_pkGPPrice->SetSelfInputCheck(false);
	m_pkGemNum->SetOffset(D3DXVECTOR2(5.0f, 4.0f));
	m_pkGPPrice->SetOffset(D3DXVECTOR2(5.0f, 4.0f));

	m_pkYes->SetSelfInputCheck(true);
	m_pkNo->SetSelfInputCheck(true);
	m_pkGemNumUp->SetSelfInputCheck(true);
	m_pkGemNumDown->SetSelfInputCheck(true);

	m_pkYes->AddActionListener(this);
	m_pkNo->AddActionListener(this);
	m_pkGemNumUp->AddActionListener(this);
	m_pkGemNumDown->AddActionListener(this);

	SetGemNum(m_iGemNum = 0);
}

void KGCGemBuyBox::OnCreateComplete(void)
{
	SpeakToActionListener(KActionEvent(this, GBB_CREATED));
}

void KGCGemBuyBox::OnDestroyComplete(void)
{
	SpeakToActionListener(KActionEvent(this, GBB_DESTROYED));
}

void KGCGemBuyBox::FrameMoveInEnabledState(void)
{
	if (g_pkInput->IsDown(DIK_RETURN))
	{
		SpeakToActionListener(KActionEvent(this, ((m_iGemNum == 0) ? GBB_NO : GBB_YES)));
		ReleaseCapture();
	}
	else if (g_pkInput->IsDown(DIK_ESCAPE))
	{
		SpeakToActionListener(KActionEvent(this, GBB_NO));
		ReleaseCapture();
		g_pkInput->IgnoreDown(DIK_ESCAPE, 30);
	}
}

void KGCGemBuyBox::ActionPerformed(const KActionEvent& event)
{
	switch (event.m_dwCode)
	{
	case KD3DWnd::D3DWE_CURSOR_ENTER:
	{
		g_KDSound.Play("30");
		break;
	}
	case KD3DWnd::D3DWE_BUTTON_PRESS:
	{
		if (++m_iPressCount > 10)
		{
			if (event.m_pWnd == m_pkGemNumUp)
			{
				int nMaxCount = GetAvilableNum();
				if (m_iGemNum < nMaxCount)
				{
					int nPlusCount = (m_iPressCount / GC_FPS_LIMIT) * 10;
					nPlusCount = nPlusCount == 0 ? 1 : nPlusCount;

					if (m_iGemNum + nPlusCount > nMaxCount)
						nPlusCount = nMaxCount - m_iGemNum;

					if (CheckAvailableNum(m_iGemNum + nPlusCount))
					{
						m_iGemNum += nPlusCount;
						SetGemNum(m_iGemNum);
					}
				}
			}
			else if (event.m_pWnd == m_pkGemNumDown)
			{
				if (m_iGemNum > 0)
				{
					int nPlusCount = (m_iPressCount / GC_FPS_LIMIT) * 10;
					nPlusCount = nPlusCount == 0 ? 1 : nPlusCount;

					if (m_iGemNum - nPlusCount < 0)
						nPlusCount = m_iGemNum;

					m_iGemNum -= nPlusCount;
					SetGemNum(m_iGemNum);
				}
			}
		}
		break;
	}
	case KD3DWnd::D3DWE_BUTTON_DOWN:
	{
		g_KDSound.Play("31");
		break;
	}
	case KD3DWnd::D3DWE_BUTTON_CLICK:
	{
		m_iPressCount = 0;

		if (event.m_pWnd == m_pkYes)
		{
			SpeakToActionListener(KActionEvent(this, ((m_iGemNum == 0) ? GBB_NO : GBB_YES)));
		}
		else if (event.m_pWnd == m_pkNo)
		{
			SpeakToActionListener(KActionEvent(this, GBB_NO));
		}
		else if (event.m_pWnd == m_pkGemNumUp)
		{
			if (m_iGemNum < m_iMaxGemNum && CheckAvailableNum(m_iGemNum + 1))
				SetGemNum(++m_iGemNum);
		}
		else if (event.m_pWnd == m_pkGemNumDown)
		{
			if (m_iGemNum > 0)
				SetGemNum(--m_iGemNum);
		}
		break;
	}
	}
}

void KGCGemBuyBox::SetGemNum(int iGemNum)
{
	std::wostringstream strmGemNum;
	std::wostringstream strmGPPrice;

	strmGemNum << iGemNum;
	strmGPPrice << iGemNum * g_MyD3D->m_kItemMgr.GetItemData(GEM_ITEM_ID)->dwPrice;

	m_pkGemNum->SetText(strmGemNum.str());
	m_pkGPPrice->SetText(strmGPPrice.str());
}

int KGCGemBuyBox::GetAvilableNum()
{
	int dwPrice = g_MyD3D->m_kItemMgr.GetItemData(GEM_ITEM_ID)->dwPrice;
	int nCount = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() / dwPrice;

	if (nCount > m_iMaxGemNum)
		return m_iMaxGemNum;
	else
		return nCount;
}

bool KGCGemBuyBox::CheckAvailableNum(int iNum)
{
	int iGemPrice = iNum * g_MyD3D->m_kItemMgr.GetItemData(GEM_ITEM_ID)->dwPrice;

	if (g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetCurrentGP() < iGemPrice)
		return false;
	else
		return true;
}