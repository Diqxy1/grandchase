#include "stdafx.h"
#include "GCDepotDlg.h"
#include "GCDepotChar.h"
#include "./KGCDepotManager.h"
#include "./DragManager.h"

IMPLEMENT_CLASSNAME(KGCDepotDlg);
IMPLEMENT_WND_FACTORY(KGCDepotDlg);
IMPLEMENT_WND_FACTORY_NAME(KGCDepotDlg, "gc_depot_dlg");

#define DEPOT_SLOT_BACK_SIZE 66.0f

KGCDepotDlg::KGCDepotDlg(void)
	: m_pkCharacterInfo(NULL)
	, m_pkBottomCloseBtn(NULL)
	, m_pkCloseBtn(NULL)
	, m_pkDescStatic(NULL)
	, m_pkClipRange(NULL)
	, m_pkScrollbar(NULL)
	, m_iScrollpos(0)
	, m_pkSafeClipRange(NULL)
	, m_pkCurrentDragImage(NULL)
	, m_pkCurrentDragItem(NULL)
	, m_pkInven_Back(NULL)
	, m_pkBtnSafeAdd(NULL)
	, m_pkBtnSlotAdd(NULL)
	, m_pkNpcFailFace(NULL)
	, m_iFaceTime(0)
	, m_pkNpcStatic(NULL)
	, m_iMentTime(0)
	, m_iCurrentMent(0)
	, isDepotSearchActive(false)
{
	LINK_CONTROL("end_bottom_btn", m_pkBottomCloseBtn);

	LINK_CONTROL("end_btn", m_pkCloseBtn);
	LINK_CONTROL("depot_desc_static", m_pkDescStatic);
	LINK_CONTROL("clip_range", m_pkClipRange);
	LINK_CONTROL("scroll", m_pkScrollbar);
	LINK_CONTROL("safe_clip_range", m_pkSafeClipRange);
	LINK_CONTROL("inven_slot_background", m_pkInven_Back);
	LINK_CONTROL("safe_btn", m_pkBtnSafeAdd);
	LINK_CONTROL("slot_btn", m_pkBtnSlotAdd);
	LINK_CONTROL("npc_fail", m_pkNpcFailFace);
	LINK_CONTROL("npc_static", m_pkNpcStatic);
}

KGCDepotDlg::~KGCDepotDlg(void)
{
	isDepotSearchActive = false;
}

void KGCDepotDlg::ActionPerformed(const KActionEvent& event)
{
	if (event.m_dwCode == D3DWE_RBUTTON_CLICK) {
		for (std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>>::iterator mIt = m_vecInvenItemImgList.begin(); mIt != m_vecInvenItemImgList.end(); mIt++) {
			if (mIt->second == event.m_pWnd) {
				ActionDepotAddItemRbtnClick(mIt->first);
				return;
			}
		}
		for (std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>>::iterator mIt = m_vecDepotItemImgList.begin(); mIt != m_vecDepotItemImgList.end(); mIt++) {
			if (mIt->second == event.m_pWnd) {
				ActionDepotDelItemRbtnClick(mIt->first);
				return;
			}
		}
	}

	if (event.m_dwCode == D3DWE_BUTTON_CLICK) {
		OnClickSafe(event);
		return;
	}

	if (event.m_dwCode == D3DWE_BUTTON_DOWN) {
		for (std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>>::iterator mIt = m_vecInvenItemImgList.begin(); mIt != m_vecInvenItemImgList.end(); mIt++) {
			if (mIt->second == event.m_pWnd) {
				ActionInvenDrag(mIt->first, mIt->second);
				return;
			}
		}
		for (std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>>::iterator mIt = m_vecDepotItemImgList.begin(); mIt != m_vecDepotItemImgList.end(); mIt++) {
			if (mIt->second == event.m_pWnd) {
				ActionDepotDrag(mIt->first, mIt->second);
				return;
			}
		}
	}

	GCWND_MAP(m_pkScrollbar, OnScrollPos);
}

void KGCDepotDlg::OnCreate(void)
{
	SiKGCDepotManager()->ClearRecvItemList();
	ClearDepotItemImageList();

	m_pkCloseBtn->InitState(true, true, this);
	m_pkBottomCloseBtn->InitState(true, true, this);
	m_pkClipRange->InitState(true, true, this);
	m_pkSafeClipRange->InitState(true, true, this);
	m_pkInven_Back->InitState(true, false, NULL);
	m_pkBtnSafeAdd->InitState(true, true, this);
	m_pkBtnSlotAdd->InitState(true, true, this);
	m_pkNpcFailFace->InitState(false, false, NULL);

	m_pkScrollbar->InitState(true, true, this);
	m_pkScrollbar->SetScrollPos(0);
	m_pkScrollbar->SetScrollRangeMin(0);
	m_pkScrollbar->SetScrollRangeMax(1);
	m_pkScrollbar->SetScrollPageSize(2);

	g_pkUIMgr->DisconnectAllEventProcedure(GetUID());
	m_pkCloseBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDepotDlg::OnClickClose);
	m_pkBottomCloseBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDepotDlg::OnClickClose);
	m_pkBtnSafeAdd->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDepotDlg::OnClickSafeAdd);
	m_pkBtnSlotAdd->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCDepotDlg::OnClickSlotAdd);

	g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_DRAG_ITEM_END, this, &KGCDepotDlg::EndlDrag);

	m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
	m_pkBottomCloseBtn->SetHotKey(DIK_ESCAPE);

	m_pkDescStatic->InitState(true, false, NULL);
	m_pkDescStatic->SetText(g_pkStrLoader->GetString(STR_ID_DEPOT_CAN_EXTEND));
	m_pkDescStatic->SetFontColor(D3DCOLOR_ARGB(255, 212, 209, 172));
	m_pkDescStatic->SetAlign(DT_CENTER);

	m_pkNpcStatic->InitState(true, false, NULL);
	m_pkNpcStatic->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_DEPOT_NPC));
	m_pkNpcStatic->SetFontColor(D3DCOLOR_ARGB(255, 0, 0, 0));

	m_iMentTime = 300;
	m_iCurrentMent = 0;

	isDepotSearchActive = true;

	GetCurrentGameState = g_MyD3D->m_pStateMachine->GetState();
}

void KGCDepotDlg::UpdateItemSearch(std::wstring SearchName) {
	DepotSearch = SearchName;
	SetInvenItemList();
}

void KGCDepotDlg::OnClickSafeAdd(void)
{
	GCItem* gcItem = g_pItemMgr->GetItemData(SiKGCDepotManager()->GetSafeExtendItemID());

	if (gcItem == NULL) return;

	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
		g_pkStrLoader->GetReplacedString(STR_ID_DEPOT_ADD_SAFE_EX, "li", gcItem->strItemName, (int)1), L"",
		KGCUIScene::GC_MBOX_USE_DEPOT_EXTEND, KDepotExtend::TAB_EXTEND, 0, false, true);
}

void KGCDepotDlg::OnClickSlotAdd(void)
{
	GCItem* gcItem = g_pItemMgr->GetItemData(SiKGCDepotManager()->GetSlotExtendItemID());

	if (gcItem == NULL) return;

	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO,
		g_pkStrLoader->GetReplacedString(STR_ID_DEPOT_ADD_SLOT_EX, "li", gcItem->strItemName, (int)1), L"",
		KGCUIScene::GC_MBOX_USE_DEPOT_EXTEND, KDepotExtend::SLOT_EXTEND, 0, false, true);
}

void KGCDepotDlg::OnCreateComplete(void)
{
	g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
	SiKGCDepotManager()->SetAllItemInventoryList();
	SiKGCDepotManager()->SetCurrentSafe(0);
	UpdateCharacterSelect();
}

void KGCDepotDlg::FrameMoveInEnabledState(void)
{
	if (GetCurrentGameState != g_MyD3D->m_pStateMachine->GetState())
	{
		OnClickClose();
	}
	if (m_iFaceTime <= 0 && m_pkNpcFailFace->IsRenderOn()) {
		m_pkNpcFailFace->ToggleRender(false);
	}
	if (m_iFaceTime > 0) {
		--m_iFaceTime;
	}

	if (m_iMentTime == 0) {
		++m_iCurrentMent;
		if (m_iCurrentMent >= static_cast<int>(m_vecMentList.size())) {
			m_iCurrentMent = 0;
		}

		m_pkNpcStatic->SetTextAutoMultiline(m_vecMentList[m_iCurrentMent]);
		m_iMentTime = 300;
	}
	else {
		--m_iMentTime;
	}

	if (SiKGCDragManager()->GetDragState() != KGCDragManager::KGDM_DRAG_PROGRESS) {
		CheckDrowItemInfo();
	}
	else {
		g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
	}
}

void KGCDepotDlg::PostChildDraw()
{
}

void KGCDepotDlg::DrawItemInfo(KItem* pItem, D3DXVECTOR2 vPos)
{
	if (pItem != NULL)
	{
		vPos.x += 49.f * GC_SCREEN_DIV_WIDTH;
		vPos.y += 20.f * GC_SCREEN_DIV_WIDTH;
		vPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
		vPos.y *= g_pGCDeviceManager2->GetWindowScaleY();
		GCItem* pGCItem = g_pItemMgr->GetItemData(pItem->m_ItemID);

		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pGCItem, -1, pItem);
		g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
		g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vPos);
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
	}
	else
	{
		g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
	}
}

void KGCDepotDlg::CheckDrowItemInfo()
{
	for (int i = 0; i < (int)m_vecDepotItemImgList.size(); ++i)
	{
		m_vecDepotItemImgList[i].second->SelectedItem(false);
	}

	for (int i = 0; i < (int)m_vecInvenItemImgList.size(); ++i)
	{
		m_vecInvenItemImgList[i].second->SelectedItem(false);
	}

	for (int i = 0; i < SiKGCDepotManager()->GetSlotOpenSize(); ++i) {
		if (m_vecDepotItemImgList[i].first != 0) {
			if (m_vecDepotBackBox[i]->CheckMousePosInWindowBound())
			{
				DrawItemInfo(SiKGCDepotManager()->GetDepotItemInfo(m_vecDepotItemImgList[i].first)
					, m_vecDepotBackBox[i]->GetCurrentWindowPos());

				m_vecDepotItemImgList[i].second->SelectedItem(true);
				return;
			}
		}
	}

	for (int i = 0; i < SiKGCDepotManager()->GetInvenMaxSlot(); ++i) {
		if (m_vecInvenItemImgList[i].first != 0) {
			if (m_vecInvenBackBox[i]->CheckMousePosInWindowBound())
			{
				DrawItemInfo(g_pItemMgr->GetInventoryItem(m_vecInvenItemImgList[i].first)
					, m_vecInvenBackBox[i]->GetCurrentWindowPos());

				m_vecInvenItemImgList[i].second->SelectedItem(true);
				return;
			}
		}
	}

	g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
}

void KGCDepotDlg::ActionInvenDrag(GCITEMUID dwItemUID, KGCCommonItemWnd* pImage)
{
	if (dwItemUID == 0) return;

	KItem *pItem = g_pItemMgr->GetInventoryItem(dwItemUID);
	if (pItem == NULL) return;

	m_pkCurrentDragImage = pImage;
	m_pkCurrentDragImage->SetColor(0x991e1e1e);
	m_pkCurrentDragItem = pItem;

	SiKGCDragManager()->StartDrag(pItem, 49.f * GC_SCREEN_DIV_WIDTH, 49.f * GC_SCREEN_DIV_WIDTH, boost::bind(&KGCDepotDlg::ProgressStartInvenDrag, this, _1), SiKGCDepotManager()->GetPetInfo(pItem));
}

void KGCDepotDlg::ActionDepotDrag(GCITEMUID dwItemUID, KGCCommonItemWnd* pImage)
{
	if (dwItemUID == 0) return;

	KItem *pItem = SiKGCDepotManager()->GetDepotItemInfo(dwItemUID);
	if (pItem == NULL) return;

	if (pItem->CheckItemType(KItem::TYPE_LOOK) == true && g_pItemMgr->GetCoordiInventorySize() >= g_pItemMgr->GetCoordiInventoryMaxSize())
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COORDI_INVENTORY_FULL_ERROR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, false);
		return;
	}

	m_pkCurrentDragImage = pImage;
	m_pkCurrentDragImage->SetColor(0x991e1e1e);
	m_pkCurrentDragItem = pItem;

	SiKGCDragManager()->StartDrag(pItem, 49.f * GC_SCREEN_DIV_WIDTH, 49.f * GC_SCREEN_DIV_WIDTH, boost::bind(&KGCDepotDlg::ProgressStartDepotDrag, this, _1), SiKGCDepotManager()->GetPetInfo(pItem));
}

void KGCDepotDlg::EndlDrag(void)
{
	for (int i = 0; i < SiKGCDepotManager()->GetSlotOpenSize(); ++i) {
		m_vecDepotBackBox[i]->Lock(false);
	}

	if (m_pkCurrentDragImage != NULL)
		m_pkCurrentDragImage->SetColor(0xffffffff);
	m_pkCurrentDragImage = NULL;
	m_pkCurrentDragItem = NULL;
}

void KGCDepotDlg::ProgressStartHoverBox()
{
	for (int i = 0; i < SiKGCDepotManager()->GetSlotOpenSize(); ++i) {
		m_vecDepotBackBox[i]->Lock(false);
		if (m_vecDepotItemImgList[i].first == 0) {
			if (m_vecDepotBackBox[i]->CheckMousePosInWindowBound()) {
				m_vecDepotBackBox[i]->Lock(true);
			}
		}
	}
}

int KGCDepotDlg::ProgressStartDepotDrag(bool btnUp)
{
	if (btnUp) {
		if (m_pkInven_Back->CheckMousePosInWindowBound())
		{
			if (m_pkCurrentDragItem == 0) {
				return KGCDragManager::KGDM_DRAG_END;
			}

			GCItem* pItemData = g_pItemMgr->GetItemData(m_pkCurrentDragItem->m_ItemID);
			if (NULL == pItemData) {
				return KGCDragManager::KGDM_DRAG_END;
			}

			if (m_pkCurrentDragItem->m_nCount > 0)
			{
				SiKGCDepotManager()->OnQuantityDepotMove(m_pkCurrentDragItem->m_ItemUID, 4);
				//g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_pkCurrentDragItem->m_ItemUID, KGCSellCountItemDlg::EM_MODE_DELETE_DEPOT, true, true);
			}
			else {
				SiKGCDepotManager()->SendDelDepotItem(m_pkCurrentDragItem);
			}
			return KGCDragManager::KGDM_DRAG_END;
		}
		for (int i = 0; i < SiKGCDepotManager()->GetSlotOpenSize(); ++i) {
			if (m_vecDepotItemImgList[i].first == 0) {
				if (m_vecDepotBackBox[i]->CheckMousePosInWindowBound())
				{
					SiKGCDepotManager()->SendMoveDepotItem(m_pkCurrentDragItem, i);
					break;
				}
			}
		}
		return KGCDragManager::KGDM_DRAG_END;
	}
	ProgressStartHoverBox();

	return KGCDragManager::KGDM_DRAG_PROGRESS;
}

int KGCDepotDlg::ProgressStartInvenDrag(bool btnUp)
{
	if (btnUp) {
		for (int i = 0; i < SiKGCDepotManager()->GetSlotOpenSize(); ++i) {
			//  창고에 넣을 수 있는 템인가
			bool bEnableInsertDepot = (m_vecDepotItemImgList[i].first == 0);
			int  iExistSlot = -1;
			if (false == bEnableInsertDepot) {
				KItem* pDepotItem = SiKGCDepotManager()->GetDepotItemInfo(m_vecDepotItemImgList[i].first);
				GCItem* pDepotItemData = g_pItemMgr->GetItemData(pDepotItem->m_ItemID);
				if (pDepotItem && pDepotItemData && (pDepotItem->m_ItemID == m_pkCurrentDragItem->m_ItemID) && (pDepotItem->m_nCount > 0)) {
					bEnableInsertDepot = true;
				}
			}

			if (bEnableInsertDepot) {
				if (m_vecDepotBackBox[i]->CheckMousePosInWindowBound())
				{
					GCItem* pItemData = g_pItemMgr->GetItemData(m_pkCurrentDragItem->m_ItemID);
					if (NULL == pItemData) {
						continue;
					}

					if (pItemData->eItemKind == GIK_SHORTCUT_SLOT && m_pkCurrentDragItem->m_nCount == 0)
					{
						g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_00),
							L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
						break;
					}

					if (m_pkCurrentDragItem->m_nCount > 0)
					{
						SiKGCDepotManager()->SetCurrentSelectSlot(i);
						SiKGCDepotManager()->OnQuantityDepotMove(m_pkCurrentDragItem->m_ItemUID, 3);
						//g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_pkCurrentDragItem->m_ItemUID, KGCSellCountItemDlg::EM_MODE_INSERT_DEPOT_FROM_DEPOT_UI, true, true);
					}
					else {
						SiKGCDepotManager()->SendAddDepotItem(m_pkCurrentDragItem, i);
					}
					break;
				}
			}
		}
		return KGCDragManager::KGDM_DRAG_END;
	}

	ProgressStartHoverBox();

	return KGCDragManager::KGDM_DRAG_PROGRESS;
}

void KGCDepotDlg::ActionDepotAddItemRbtnClick(GCITEMUID dwItemUID)
{
	if (dwItemUID == 0) return;

	KItem* pInvenItem = g_pItemMgr->GetInventoryItem(dwItemUID);
	if (NULL == pInvenItem) {
		return;
	}

	for (int i = 0; i < SiKGCDepotManager()->GetSlotOpenSize(); ++i)
	{
		bool bEnableInsertDepot = (m_vecDepotItemImgList[i].first == 0);
		if (false == bEnableInsertDepot) {
			KItem* pDepotItem = SiKGCDepotManager()->GetDepotItemInfo(m_vecDepotItemImgList[i].first);
			GCItem* pDepotItemData = g_pItemMgr->GetItemData(pDepotItem->m_ItemID);
			if (pDepotItem && pDepotItemData && (pDepotItem->m_ItemID == pInvenItem->m_ItemID) && (pDepotItem->m_nCount > 0)) {
				bEnableInsertDepot = true;
			}
		}

		if (bEnableInsertDepot) {
			GCItem* pInvenItemData = g_pItemMgr->GetItemData(pInvenItem->m_ItemID);
			if (NULL == pInvenItemData) {
				continue;
			}

			if (pInvenItemData->eItemKind == GIK_SHORTCUT_SLOT && pInvenItem->m_nCount == 0)
			{
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_00),
					L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
				break;
			}

			if (pInvenItem->m_nCount > 0)
			{
				SiKGCDepotManager()->SetCurrentSelectSlot(i);
				SiKGCDepotManager()->OnQuantityDepotMove(dwItemUID, 3);
				//g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, dwItemUID, KGCSellCountItemDlg::EM_MODE_INSERT_DEPOT_FROM_DEPOT_UI, true, true);
			}
			else {
				SiKGCDepotManager()->SendAddDepotItem(pInvenItem, i);
			}
			break;
		}
	}
}

void KGCDepotDlg::ActionDepotDelItemRbtnClick(GCITEMUID dwItemUID)
{
	if (dwItemUID == 0) return;

	KItem *pItem = SiKGCDepotManager()->GetDepotItemInfo(dwItemUID);
	if (pItem == NULL) return;

	if (pItem->CheckItemType(KItem::TYPE_LOOK) == true && g_pItemMgr->GetCoordiInventorySize() >= g_pItemMgr->GetCoordiInventoryMaxSize())
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COORDI_INVENTORY_FULL_ERROR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, false);
		return;
	}

	GCItem* pItemData = g_pItemMgr->GetItemData(pItem->m_ItemID);
	if (NULL == pItemData) {
		return;
	}

	if (pItem->m_nCount > 0)
	{
		SiKGCDepotManager()->OnQuantityDepotMove(dwItemUID, 4);
		//g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_SELL_COUNT_ITEM, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, dwItemUID, KGCSellCountItemDlg::EM_MODE_DELETE_DEPOT, true, true);
	}
	else {
		SiKGCDepotManager()->SendDelDepotItem(pItem);
	}
}

void KGCDepotDlg::OnScrollPos(void)
{
	int iPos = m_pkScrollbar->GetScrollPos();

	if (m_iScrollpos != iPos)
	{
		m_iScrollpos = iPos;
		g_KDSound.Play("73");
		SetInvenItemList();
	}
}

void KGCDepotDlg::OnClickClose(void)
{
	Result_DepotUpdateItemReq = INT_MAX;
	KP2P::GetInstance()->Send_DepotDBUpdateReq();
	g_MyD3D->WaitForServerAck(Result_DepotUpdateItemReq, INT_MAX, 5000, TIME_OUT_VALUE);
	isDepotSearchActive = false;
	SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
};

void KGCDepotDlg::SetBoxPosition(std::vector<KD3DWnd*> &vecBox, int With, D3DXVECTOR2 pos_)
{
	if (With == 0)
		return;

	for (int i = 0; i < (int)vecBox.size(); ++i)
	{
		D3DXVECTOR2 pos = pos_;
		pos.x += DEPOT_SLOT_BACK_SIZE * (i % With);
		pos.y += DEPOT_SLOT_BACK_SIZE * (i / With);
		vecBox[i]->SetWindowPosDirect(pos);
		vecBox[i]->InitState(true, true, this);
	}
}

void KGCDepotDlg::SetSafeBoxPosition(std::vector<KD3DWnd*> &vecBox, int SafeType, bool bIsBtn, KActionListener* pListerner)
{
	const float fCenterPos = 132.f * GC_SCREEN_DIV_WIDTH;
	const float fSafeHalfWidth = 22.f * GC_SCREEN_DIV_WIDTH;

	int iSafeBoxNum = SiKGCDepotManager()->GetDepotMaxSafe();

	float fStartPos = fCenterPos - (fSafeHalfWidth * (iSafeBoxNum - SafeType));

	int i = 0;
	for (std::vector<KD3DWnd*>::iterator vit = vecBox.begin(); vit != vecBox.end(); vit++)
	{
		D3DXVECTOR2 pos;
		pos.x = fStartPos + ((fSafeHalfWidth * 2) * i);
		pos.y = 0.f;
		(*vit)->SetWindowPos(pos);

		(*vit)->InitState(true, bIsBtn, pListerner);
		i++;
	}
}

void KGCDepotDlg::OnClickSafe(const KActionEvent& event)
{
	for (int i = 0; i < static_cast<int>(m_vecDepotSafeBtn.size()); ++i)
	{
		if (m_vecDepotSafeBtn[i] == event.m_pWnd) {
			SetSafeList(i);
		}
	}
}

void KGCDepotDlg::CreateUI(void)
{
	if (m_vecDepotBackBox.empty()) {
		m_vecDepotBackBox = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRange, "ui_depot.stg", "",
			"depot_dlg\\depot_slot_box", true, SiKGCDepotManager()->GetDepotMaxSlot());
		SetBoxPosition(m_vecDepotBackBox, SiKGCDepotManager()->GetDepotWidth(), D3DXVECTOR2(4.0f, 4.0f));
	}

	if (m_vecDepotLockBackBox.empty()) {
		m_vecDepotLockBackBox = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRange, "ui_depot.stg", "",
			"depot_dlg\\depot_slot_lock_box", true, SiKGCDepotManager()->GetDepotMaxSlot());
		SetBoxPosition(m_vecDepotLockBackBox, SiKGCDepotManager()->GetDepotWidth(), D3DXVECTOR2(4.0f, 4.0f));
	}

	if (m_vecInvenBackBox.empty()) {
		m_vecInvenBackBox = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRange, "ui_depot.stg", "",
			"depot_dlg\\inven_slot_box", true, SiKGCDepotManager()->GetInvenMaxSlot());
		SetBoxPosition(m_vecInvenBackBox, SiKGCDepotManager()->GetInvenWidth(), D3DXVECTOR2(387.0f, 4.0f));
	}

	if (m_vecDepotItemImgList.empty()) {
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRange, "ui_depot.stg", "",
			"depot_dlg\\box_item", true, SiKGCDepotManager()->GetDepotMaxSlot());

		SetBoxPosition(vecTemp, SiKGCDepotManager()->GetDepotWidth(), D3DXVECTOR2(5.0f, 5.0f));

		for (std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end(); vIt++)
		{
			m_vecDepotItemImgList.push_back(std::pair<GCITEMUID, KGCCommonItemWnd*>(0, static_cast<KGCCommonItemWnd*>(*vIt)));
		}
	}

	if (m_vecInvenItemImgList.empty())
	{
		std::vector<KD3DWnd*> vecTemp = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkClipRange, "ui_depot.stg", "",
			"depot_dlg\\box_item", true, SiKGCDepotManager()->GetInvenMaxImage());

		SetBoxPosition(vecTemp, SiKGCDepotManager()->GetInvenWidth(), D3DXVECTOR2(388.0f, 5.0f));

		for (std::vector<KD3DWnd*>::iterator vIt = vecTemp.begin(); vIt != vecTemp.end(); vIt++)
		{
			m_vecInvenItemImgList.push_back(std::pair<GCITEMUID, KGCCommonItemWnd*>(0, static_cast<KGCCommonItemWnd*>(*vIt)));
		}
	}

	if (m_vecDepotSafeBtn.empty()) {
		m_vecDepotSafeBtn = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkSafeClipRange, "ui_depot.stg", "",
			"depot_dlg\\sel_safe_btn", true, SiKGCDepotManager()->GetDepotMaxSafe());
		SetSafeBoxPosition(m_vecDepotSafeBtn, 0, true, this);
	}

	if (m_vecDepotSafeLock.empty()) {
		m_vecDepotSafeLock = g_pkUIMgr->CreateTemplateUIListByFindName(m_pkSafeClipRange, "ui_depot.stg", "",
			"depot_dlg\\lock_safe", true, SiKGCDepotManager()->GetDepotMaxSafe());
		SetSafeBoxPosition(m_vecDepotSafeLock, 0, false, NULL);
	}

	if (m_vecMentList.empty()) {
		m_vecMentList.push_back(g_pkStrLoader->GetString(STR_ID_DEPOT_NPC));
		m_vecMentList.push_back(g_pkStrLoader->GetString(STR_ID_DEPOT_NPC_1));
	}
}

void KGCDepotDlg::SetShowInvenItemList()
{
	int size = m_vecShowInvenItemList.size();
	std::vector<GCITEMUID> vecRealShowList;

	if (size < SiKGCDepotManager()->GetInvenMaxSlot()) {
		vecRealShowList = m_vecShowInvenItemList;
	}
	else {
		std::copy(m_vecShowInvenItemList.begin() + (m_iScrollpos*SiKGCDepotManager()->GetInvenWidth()),
			m_vecShowInvenItemList.end(), std::back_inserter(vecRealShowList));
	}

	std::vector<GCITEMUID>::iterator vitTemp = vecRealShowList.begin();

	int showSize = m_vecInvenItemImgList.size();
	for (int i = 0; i < showSize; ++i) {
		if (vitTemp == vecRealShowList.end()) {
			m_vecInvenItemImgList[i].first = 0;
			m_vecInvenItemImgList[i].second->ResetItem();
			continue;
		}
		KItem* pItem = g_pItemMgr->GetInventoryItem(*vitTemp);
		if (pItem != NULL) {
			m_vecInvenItemImgList[i].first = *vitTemp;
			SetItemImg(pItem, m_vecInvenItemImgList[i].second);
		}
		vitTemp++;
	}
}

void KGCDepotDlg::SetItemImg(KItem* pInventoryItem, KGCCommonItemWnd* pkItemImg)
{
	GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData(pInventoryItem->m_ItemID);

	KPetInfo *pPet = SiKGCDepotManager()->GetPetInfo(pInventoryItem);
	pkItemImg->SetItem(pInventoryItem, pPet);
}

void KGCDepotDlg::SetInvenItemList()
{
	m_vecShowInvenItemList.clear();
	m_vecTempItemList.clear();
	m_vecShowInvenItemList = SiKGCDepotManager()->GetShowItemInventoryList();

	if (DepotSearch.size() > 0)
	{
		for (auto& element : m_vecShowInvenItemList) {
			std::wstring strLowerName = DepotSearch;
			std::wstring strLowerItemName = g_pItemMgr->GetItemNameUID(element);
			GCUTIL_STR::ToLower(strLowerName);
			GCUTIL_STR::ToLower(strLowerItemName);
			if (strLowerItemName.find(strLowerName) != std::wstring::npos) {
				m_vecTempItemList.push_back(element);
			}
		}
		m_vecShowInvenItemList.clear();
		m_vecShowInvenItemList = m_vecTempItemList;
	}

	int scrollMaxSize = ((m_vecShowInvenItemList.size() - 1) / SiKGCDepotManager()->GetInvenWidth()) - 4;

	if (scrollMaxSize < 0 || m_vecShowInvenItemList.empty()) {
		scrollMaxSize = 1;
	}
	if (m_iScrollpos > scrollMaxSize) {
		m_iScrollpos = scrollMaxSize;
	}

	m_pkScrollbar->SetScrollRangeMax(scrollMaxSize);

	SetShowInvenItemList();
}

void KGCDepotDlg::ClearDepotItemImageList()
{
	for (std::vector<std::pair<GCITEMUID, KGCCommonItemWnd*>>::iterator vecIter = m_vecDepotItemImgList.begin(); vecIter != m_vecDepotItemImgList.end(); vecIter++) {
		vecIter->first = 0;
		vecIter->second->ResetItem();
	}
}
void KGCDepotDlg::SetDepotItemList()
{
	ClearDepotItemImageList();
	std::vector<KItem> &vecItem = SiKGCDepotManager()->GetDepotItemList();

	for (std::vector<KItem>::iterator vecIter = vecItem.begin(); vecIter != vecItem.end(); vecIter++) {
		int iSlotID = vecIter->m_cSlotID;
		KItem* pItem = SiKGCDepotManager()->GetDepotItemInfo(vecIter->m_ItemUID);
		if (pItem != NULL) {
			m_vecDepotItemImgList[iSlotID].first = vecIter->m_ItemUID;

			SetItemImg(pItem, m_vecDepotItemImgList[iSlotID].second);
		}
	}
}

void KGCDepotDlg::SetSafeList()
{
	int iSafeOpenSize = SiKGCDepotManager()->GetSafeOpenSize();

	for (int i = 0; i < SiKGCDepotManager()->GetDepotMaxSafe(); i++)
	{
		m_vecDepotSafeBtn[i]->ToggleRender(i < iSafeOpenSize);
		m_vecDepotSafeBtn[i]->Lock(false);
		m_vecDepotSafeLock[i]->ToggleRender(i >= iSafeOpenSize);
	}
	SetSafeList(SiKGCDepotManager()->GetCurrentSafe());
}

void KGCDepotDlg::SetSafeList(int iSelSafe)
{
	int iSafeOpenSize = SiKGCDepotManager()->GetSafeOpenSize();

	if (iSafeOpenSize <= iSelSafe) return;

	m_vecDepotSafeBtn[SiKGCDepotManager()->GetCurrentSafe()]->Lock(false);
	m_vecDepotSafeBtn[iSelSafe]->Lock(true);
	SiKGCDepotManager()->SetCurrentSafe(iSelSafe);
	SiKGCDepotManager()->Send_DepotInfoByCharTabReq();
	SetDepotBackBox();
	SetTargetBoxList();
	SetDepotItemList();
}

void KGCDepotDlg::SetDepotBackBox(void)
{
	int iSlotOpenSize = SiKGCDepotManager()->GetSlotOpenSize();

	for (int i = 0; i < SiKGCDepotManager()->GetDepotMaxSlot(); i++)
	{
		m_vecDepotBackBox[i]->ToggleRender(i < iSlotOpenSize);
		m_vecDepotLockBackBox[i]->ToggleRender(i >= iSlotOpenSize);
	}
}

void KGCDepotDlg::SetTargetBoxList(void)
{
	m_vecDepotTargetBox.clear();
	m_vecDepotTargetBox = m_vecInvenBackBox;
	for (std::vector<KD3DWnd*>::iterator vIt = m_vecDepotBackBox.begin(); vIt != m_vecDepotBackBox.end(); vIt++)
	{
		if ((*vIt)->IsRenderOn() == false) break;
		m_vecDepotTargetBox.push_back(*vIt);
	}
}

void KGCDepotDlg::UpdateDepotData(void)
{
	SetInvenItemList();
	OnScrollPos();
	SetSafeList();
	SetDepotItemList();
}

void KGCDepotDlg::UpdateCharacterSelect(void)
{
	SiKGCDepotManager()->SetCurrentSafe(0);
	SiKGCDepotManager()->Send_DepotInfoByCharTabReq();
	UpdateDepotData();
}