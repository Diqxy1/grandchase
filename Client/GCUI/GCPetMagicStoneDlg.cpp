#include "stdafx.h"
#include "GCPetMagicStoneItemSlot.h"
#include "GCPetMagicStoneDlg.h"
#include "GCPetMagicStone.h"
#include <Procedure.h>

IMPLEMENT_CLASSNAME(KGCPetMagicStoneDlg);
IMPLEMENT_WND_FACTORY(KGCPetMagicStoneDlg);
IMPLEMENT_WND_FACTORY_NAME(KGCPetMagicStoneDlg, "gc_petmagicstonedlg" );

KGCPetMagicStoneDlg::KGCPetMagicStoneDlg(void)
: m_pkScrollBar(NULL)
, m_pkNoStone(NULL)
, m_pkBtnClose(NULL)
, m_iActiveTab(MAX_STONE_ITEM_TAB - 1)
, m_pCompleteEffect(NULL)
, m_iTopIndex(0)
{
	LINK_CONTROL("btn_close", m_pkBtnClose);
	LINK_CONTROL("img_empty", m_pkNoStone);
	LINK_CONTROL("scroll", m_pkScrollBar);

	for (int i = 0; i < MAX_STONE_EQUIP_SLOT; i++)
	{
		m_arrEquipSlot[i] = NULL;
		LINK_CONTROL_STM("equip_slot" << i, m_arrEquipSlot[i]);
	}

	for (int i = 0; i < MAX_STONE_ITEM_TAB; i++)
	{
		m_arrTabOn[i] = NULL;
		m_arrTabOff[i] = NULL;
		
		LINK_CONTROL_STM("btn_tab" << i, m_arrTabOff[i]);
		LINK_CONTROL_STM("btn_tab_on" << i, m_arrTabOn[i]);
	}

	for (int i = 0; i < MAX_STONE_ITEM_SLOT; i++)
	{
		m_arrItemSlot[i] = NULL;
		LINK_CONTROL_STM("itemslot0" << i, m_arrItemSlot[i]);
	}
}

KGCPetMagicStoneDlg::~KGCPetMagicStoneDlg(void)
{
}

void KGCPetMagicStoneDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP(m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
	GCWND_MAP(m_pkScrollBar, OnScrollPos);

	if (event.m_dwCode == KGCUIScene::D3DWE_PET_MAGIC_STONE_EQUIP_CLICK)
	{
		int iEffect = static_cast<int>(event.m_dwlParam);
		GCITEMUID dwUID = static_cast<GCITEMUID>(event.m_dwlParam2);
		GCITEMUID equipSlotUID = m_arrEquipSlot[iEffect]->GetItemUID();

		if (equipSlotUID != 0 && equipSlotUID != dwUID && iEffect < MAX_STONE_TYPE)
		{
			RemovePetGlyph(iEffect, equipSlotUID);
		}

		OnEquipPetGlyph(iEffect, dwUID, true);
	}
}

void KGCPetMagicStoneDlg::PostChildDraw()
{
	g_ParticleManager->Render(GC_LAYER_UPPER_SQUARE_UI_0);
}

void KGCPetMagicStoneDlg::UpdatePetGlyphData(bool bAllEffect)
{
	FilterGlyph(m_iActiveTab);

	if (bAllEffect == false)
	{
		if (m_pCompleteEffect)
		{
			g_ParticleManager->DeleteSequence(m_pCompleteEffect);
		}
		return;
	}
	else
	{
		if (m_pCompleteEffect)
		{
			g_ParticleManager->DeleteSequence(m_pCompleteEffect);
			delete m_pCompleteEffect;
			m_pCompleteEffect = NULL;
		}
		
		m_pCompleteEffect = g_ParticleManager->CreateSequence("UI_PetCircle_11", 0.f, 0.f, 0.5f);
		g_ParticleManager->SetLayer(m_pCompleteEffect, GC_LAYER_UPPER_SQUARE_UI_0);
	}
}

void KGCPetMagicStoneDlg::RemovePetGlyph(int iIndex, GCITEMUID dwUID, bool bUpdateTab)
{
	SiKP2P()->Send_PetMagicStoneChangeEquipReq(dwUID, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType, false);
	Result_ReceivePetMagicStoneChangeEquipReq = INT_MAX;
	g_MyD3D->WaitForServerAck(Result_ReceivePetMagicStoneChangeEquipReq, INT_MAX, 5000, TIME_OUT_VALUE);

	if (Result_ReceivePetMagicStoneChangeEquipReq == 0)
	{
		m_arrEquipSlot[iIndex]->ResetItem();

		KItem* pItem = g_pItemMgr->GetInventoryItem(dwUID);
		m_vecInvGlyph.insert(m_vecInvGlyph.begin(), dwUID);
		g_pkUIScene->m_pkMyInfoScene->UpdateGameData();

		auto it = m_mapEffect.find(iIndex);
		if (it != m_mapEffect.end())
		{
			for (auto effect : it->second)
			{
				g_ParticleManager->DeleteSequence(effect);
			}

			m_mapEffect.erase(it);
		}

		FilterGlyph(m_iActiveTab);
	}
}

void KGCPetMagicStoneDlg::OnCreate()
{
	if (m_pkBtnClose)
	{
		m_pkBtnClose->InitState(true, true, this);
		m_pkBtnClose->SetHotKey(DIK_ESCAPE);
	}

	if (m_pkNoStone)
	{
		m_pkNoStone->InitState(true, true, this);
	}

	if (m_pkScrollBar)
	{
		m_pkScrollBar->InitState(true, true, this);
		m_pkScrollBar->InitScroll(0, 1, 0, MAX_STONE_ITEM_SLOT, 0, 0, 1, true);
	}

	for (int i = 0; i < MAX_STONE_EQUIP_SLOT; i++)
	{
		if (m_arrEquipSlot[i])
		{
			m_arrEquipSlot[i]->InitState(true, true, this);
			m_arrEquipSlot[i]->ConnectEventProcedure<KGCPetMagicStoneDlg, void>(KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind(&KGCPetMagicStoneDlg::OnClickGlyphSlot, _1, i));
		}
	}

	for (int i = 0; i < MAX_STONE_ITEM_TAB; i++)
	{
		if (m_arrTabOn[i])
		{
			m_arrTabOn[i]->InitState(false, true, this);
			m_arrTabOn[i]->ConnectEventProcedure<KGCPetMagicStoneDlg, void>(KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind(&KGCPetMagicStoneDlg::OnClickTab, _1, i));
		}

		if (m_arrTabOff[i])
		{
			m_arrTabOff[i]->InitState(true, true, this);
			m_arrTabOff[i]->ConnectEventProcedure<KGCPetMagicStoneDlg, void>(KD3DWnd::D3DWE_BUTTON_CLICK, this, boost::bind(&KGCPetMagicStoneDlg::OnClickTab, _1, i));
		}
	}

	for (int i = 0; i < MAX_STONE_ITEM_SLOT; i++)
	{
		if (m_arrItemSlot[i])
		{
			m_arrItemSlot[i]->InitState(false, true, this);
		}
	}
}

bool KGCPetMagicStoneDlg::InitDialog(IN const KDialogInfo& kInfo_)
{
	m_vecInvGlyph.clear();
	g_pItemMgr->GetInventoryPetGlyph(&g_kGlobalValue.m_kUserInfo.GetCurrentChar(), m_vecInvGlyph);

	if (kInfo_.m_lParam != 0)
	{
		GCITEMID dwID = static_cast<GCITEMID>(kInfo_.m_lParam) * 10;
		auto petGlyph = SiGCPetMagicStone()->GetPetGlyph(dwID);
		m_iActiveTab = petGlyph.m_iGlyphEffect;
	}

	InitStoneInfo();
	return true;
}

void KGCPetMagicStoneDlg::InitStoneInfo()
{
	if(m_arrTabOn[m_iActiveTab])
		m_arrTabOn[m_iActiveTab]->ToggleRender(true);

	auto vecGlyph = g_kGlobalValue.m_kUserInfo.GetCurrentChar().vecPetGlyphInfo;

	for (auto glyphInfo : vecGlyph)
	{
		OnEquipPetGlyph(glyphInfo.m_cItemType, glyphInfo.m_dwUID);
	}

	if (vecGlyph.size() >= MAX_STONE_TYPE)
	{
		m_pCompleteEffect = g_ParticleManager->CreateSequence("UI_PetCircle_11", 0.f, 0.f, 0.5f);
		g_ParticleManager->SetLayer(m_pCompleteEffect, GC_LAYER_UPPER_SQUARE_UI_0);
	}

	FilterGlyph(m_iActiveTab);
}

void KGCPetMagicStoneDlg::UpdateStoneList()
{
	if (m_iActiveTab != MAX_STONE_ITEM_TAB - 1)
	{
		return;
	}

	for (int i = 0; i < MAX_STONE_ITEM_SLOT; i++)
	{
		m_arrItemSlot[i]->ToggleRender(false);
	}

	m_pkScrollBar->SetScrollRangeMax(m_vecInvGlyph.size());

	for (int i = 0; i < MAX_STONE_ITEM_SLOT; i++)
	{
		if (m_iTopIndex + i < (int)m_vecInvGlyph.size())
		{
			GCITEMUID dwUID = m_vecInvGlyph[m_iTopIndex + i];
			KItem* PetGlyphItem = g_pItemMgr->GetInventoryItem(dwUID);

			if (PetGlyphItem != NULL)
			{
				KPetGlyphData petGlyph = SiGCPetMagicStone()->GetPetGlyph(PetGlyphItem->m_ItemID * 10);
				if (m_iActiveTab != MAX_STONE_ITEM_TAB - 1 && petGlyph.m_iGlyphEffect != m_iActiveTab)
				{
					continue;
				}

				m_arrItemSlot[i]->AddItem(dwUID);
				m_arrItemSlot[i]->ToggleRender(true);
				m_arrItemSlot[i]->SetIndex(petGlyph.m_iGlyphEffect);
			}
		}
		else
		{
			m_arrItemSlot[i]->ToggleRender(false);
		}
	}
}

void KGCPetMagicStoneDlg::FilterGlyph(int iGlyphEffect)
{
	for (int i = 0; i < MAX_STONE_ITEM_SLOT; i++)
	{
		m_arrItemSlot[i]->ToggleRender(false);
	}

	int iSlotIndex = 0;
	bool bFound = false;

	for (auto it : m_vecInvGlyph)
	{
		KItem* PetGlyphItem = g_pItemMgr->GetInventoryItem(it);

		if (PetGlyphItem != NULL)
		{
			auto petGlyph = SiGCPetMagicStone()->GetPetGlyph(PetGlyphItem->m_ItemID * 10);
			if (iGlyphEffect != MAX_STONE_ITEM_TAB - 1 && petGlyph.m_iGlyphEffect != iGlyphEffect)
			{
				continue;
			}

			bFound = true;
			m_arrItemSlot[iSlotIndex]->AddItem(PetGlyphItem->m_ItemUID);
			m_arrItemSlot[iSlotIndex]->ToggleRender(true);
			m_arrItemSlot[iSlotIndex]->SetIndex(petGlyph.m_iGlyphEffect);

			if (++iSlotIndex == MAX_STONE_ITEM_SLOT)
			{
				break;
			}
		}
	}

	m_pkNoStone->ToggleRender(!bFound);
}

void KGCPetMagicStoneDlg::OnClickTab(int iTab)
{
	if (iTab < 0 || iTab >= MAX_STONE_ITEM_TAB)
		return;

	g_KDSound.Play("73");
	m_arrTabOn[m_iActiveTab]->ToggleRender(false);
	m_arrTabOff[m_iActiveTab]->ToggleRender(true);

	m_iActiveTab = iTab;
	FilterGlyph(iTab);

	m_arrTabOff[m_iActiveTab]->ToggleRender(false);
	m_arrTabOn[m_iActiveTab]->ToggleRender(true);
}

void KGCPetMagicStoneDlg::OnClickGlyphSlot(int iSlot)
{
	if (iSlot < 0 || iSlot >= MAX_STONE_ITEM_SLOT)
		return;

	if (m_arrEquipSlot[iSlot]->GetItemUID() == 0)
	{
		return;
	}

	g_KDSound.Play("73");
	RemovePetGlyph(iSlot, m_arrEquipSlot[iSlot]->GetItemUID(), true);
}

void KGCPetMagicStoneDlg::OnScrollPos(void)
{
	int iPos = m_pkScrollBar->GetScrollPos();

	if (m_iTopIndex != iPos)
	{
		g_KDSound.Play("73");
		m_iTopIndex = iPos;
		UpdateStoneList();
	}
}

void KGCPetMagicStoneDlg::OnClose()
{
	m_iActiveTab = MAX_STONE_ITEM_TAB - 1;
	g_ParticleManager->DeleteAllInstance();
	g_KDSound.Play("31");
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void KGCPetMagicStoneDlg::OnEquipPetGlyph(int iIndex, GCITEMUID dwUID, bool bSendPkt)
{
	KItem* pItem = g_pItemMgr->GetInventoryItem(dwUID);

	if (pItem == NULL)
	{
		return;
	}

	auto petData = SiGCPetMagicStone()->GetPetGlyph(pItem->m_ItemID * 10);

	if (bSendPkt)
	{
		SiKP2P()->Send_PetMagicStoneChangeEquipReq(dwUID, g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType, true);
		Result_ReceivePetMagicStoneChangeEquipReq = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_ReceivePetMagicStoneChangeEquipReq, INT_MAX, 5000, TIME_OUT_VALUE);
	}
	else
	{
		Result_ReceivePetMagicStoneChangeEquipReq = 0;
	}

	if (Result_ReceivePetMagicStoneChangeEquipReq == 0)
	{
		g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
		m_arrEquipSlot[iIndex]->SetItem(pItem);
		EnableGlyphEffect(iIndex);

		for (auto it = m_vecInvGlyph.begin(); it != m_vecInvGlyph.end(); ++it)
		{
			if (*it == dwUID)
			{
				m_vecInvGlyph.erase(it);
				break;
			}
		}

		FilterGlyph(m_iActiveTab);
	}
}

void KGCPetMagicStoneDlg::EnableGlyphEffect(int iIndex)
{
	int iParticleSeq = iIndex * 2 + 1;

	std::string particleStr = "UI_PetCircle_";

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << iParticleSeq;

	std::string strEffect1 = particleStr + ss.str();
	CParticleEventSeqPTR effect1 = g_ParticleManager->CreateSequence(strEffect1, 0.f, 0.f, 0.5f);

	std::stringstream ss2;
	iParticleSeq++;
	ss2 << std::setw(2) << std::setfill('0') << iParticleSeq;
	std::string strEffect2 = particleStr + ss2.str();

	CParticleEventSeqPTR effect2 = g_ParticleManager->CreateSequence(strEffect2, 0.f, 0.f, 0.5f);

	std::vector<CParticleEventSeqPTR> vecEffect = { effect1, effect2 };

	auto it = m_mapEffect.find(iIndex);
	if (it != m_mapEffect.end())
	{
		it->second.clear();
		it->second.swap(vecEffect);
	}
	else
	{
		m_mapEffect.insert(std::make_pair(iIndex, vecEffect));
	}

	g_ParticleManager->SetLayer(effect1, GC_LAYER_UPPER_SQUARE_UI_0);
	g_ParticleManager->SetLayer(effect2, GC_LAYER_UPPER_SQUARE_UI_0);
}