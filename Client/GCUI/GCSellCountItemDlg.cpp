#include "stdafx.h"
#include "GCSellCountItemDlg.h"
#include "./KGCVirtualDepotManager.h"

IMPLEMENT_CLASSNAME(KGCSellCountItemDlg);
IMPLEMENT_WND_FACTORY(KGCSellCountItemDlg);
IMPLEMENT_WND_FACTORY_NAME(KGCSellCountItemDlg, "gc_sellcountitemdlg");

KGCSellCountItemDlg::KGCSellCountItemDlg(void)
{
	for (int i = 0; i < NUM_PAD; ++i) {
		m_pkBtnNums[i] = NULL;
	}

	m_emModeState = EM_MODE_SELL_LIST;
	m_ItemUID = KItem::INVALID_ITEM;
	m_iMaxNum = 0;
	m_bScrollStart = false;

	m_pkBtnOK = NULL;
	m_pkBtnCancle = NULL;
	m_pkBtnClose = NULL;
	m_pkIncrease1 = NULL;
	m_pkDecrease1 = NULL;
	m_pkIncrease10 = NULL;
	m_pkDecrease10 = NULL;

	m_pkItemImg = NULL;

	m_pkItemName = NULL;
	m_pkDesc = NULL;

	m_pkBtnMax = NULL;
	m_pkBtnMin = NULL;
	m_pkBtnBackspace = NULL;
	m_pkBtnClear = NULL;

	m_pkOwnCount = NULL;
	m_pkSellCount = NULL;
	m_pkReward = NULL;
	m_pkMoneyType = NULL;

	m_pkScrollLine = NULL;
	m_pkBtnScroll = NULL;

	m_pkSellTitle = NULL;
	m_pkInsertDepotTitle = NULL;
	m_pkDeleteDepotTitle = NULL;
	m_pkSellCountTitle = NULL;
	m_pkInsertDepotCountTitle = NULL;
	m_pkDeleteDepotCountTitle = NULL;

	for (int i = 0; i < NUM_PAD; ++i) {
		LINK_CONTROL_STM("btn_num_" << i, m_pkBtnNums[i]);
	}
	LINK_CONTROL("btn_ok", m_pkBtnOK);
	LINK_CONTROL("btn_cancle", m_pkBtnCancle);
	LINK_CONTROL("btn_close", m_pkBtnClose);
	LINK_CONTROL("btn_increase10", m_pkIncrease10);
	LINK_CONTROL("btn_decrease10", m_pkDecrease10);
	LINK_CONTROL("btn_increase1", m_pkIncrease1);
	LINK_CONTROL("btn_decrease1", m_pkDecrease1);
	LINK_CONTROL("btn_num_max", m_pkBtnMax);
	LINK_CONTROL("btn_num_min", m_pkBtnMin);
	LINK_CONTROL("btn_num_bs", m_pkBtnBackspace);
	LINK_CONTROL("btn_num_ac", m_pkBtnClear);

	LINK_CONTROL("static_itemname", m_pkItemName);
	LINK_CONTROL("static_desc", m_pkDesc);
	LINK_CONTROL("item_img", m_pkItemImg);

	LINK_CONTROL("number_owncount", m_pkOwnCount);
	LINK_CONTROL("number_sellcount", m_pkSellCount);
	LINK_CONTROL("number_reward", m_pkReward);
	LINK_CONTROL("money_type", m_pkMoneyType);

	LINK_CONTROL("scroll_line", m_pkScrollLine);
	LINK_CONTROL("btn_scroll", m_pkBtnScroll);

	LINK_CONTROL("text_title", m_pkSellTitle);
	LINK_CONTROL("text_title_insert_depot", m_pkInsertDepotTitle);
	LINK_CONTROL("text_title_delete_depot", m_pkDeleteDepotTitle);
	LINK_CONTROL("text_sellcount", m_pkSellCountTitle);
	LINK_CONTROL("text_insert_depot_count", m_pkInsertDepotCountTitle);
	LINK_CONTROL("text_delete_depot_count", m_pkDeleteDepotCountTitle);
	LINK_CONTROL("text_titlle_virtual_depot", m_pkVirtualDepotTitle);
}

KGCSellCountItemDlg::~KGCSellCountItemDlg(void)
{
}

void KGCSellCountItemDlg::OnCreate(void)
{
	m_pkBtnOK->InitState(true, true);
	m_pkBtnOK->DisconnectAllEventProcedure();
	m_pkBtnOK->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickOK);
	m_pkBtnOK->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClose);
	m_pkBtnOK->SetHotKey(DIK_RETURN);

	m_pkBtnCancle->InitState(true, true);
	m_pkBtnCancle->DisconnectAllEventProcedure();
	m_pkBtnCancle->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClose);

	m_pkBtnClose->InitState(true, true);
	m_pkBtnClose->DisconnectAllEventProcedure();
	m_pkBtnClose->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClose);
	m_pkBtnClose->SetHotKey(DIK_ESCAPE);

	m_pkIncrease1->InitState(true, true);
	m_pkIncrease1->DisconnectAllEventProcedure();
	m_pkIncrease1->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickIncresase1);
	m_pkDecrease1->InitState(true, true);
	m_pkDecrease1->DisconnectAllEventProcedure();
	m_pkDecrease1->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickDecresase1);
	m_pkIncrease10->InitState(true, true);
	m_pkIncrease10->DisconnectAllEventProcedure();
	m_pkIncrease10->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickIncresase10);
	m_pkDecrease10->InitState(true, true);
	m_pkDecrease10->DisconnectAllEventProcedure();
	m_pkDecrease10->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickDecresase10);

	m_pkBtnMax->InitState(true, true);
	m_pkBtnMax->DisconnectAllEventProcedure();
	m_pkBtnMax->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickMax);
	m_pkBtnMin->InitState(true, true);
	m_pkBtnMin->DisconnectAllEventProcedure();
	m_pkBtnMin->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickMin);
	m_pkBtnBackspace->InitState(true, true);
	m_pkBtnBackspace->DisconnectAllEventProcedure();
	m_pkBtnBackspace->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickBackspace);
	m_pkBtnBackspace->SetHotKey(DIK_BACKSPACE);

	m_pkBtnClear->InitState(true, true);
	m_pkBtnClear->DisconnectAllEventProcedure();
	m_pkBtnClear->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickClear);

	m_mapNumbers.clear();
	for (int i = 0; i < NUM_PAD; ++i) {
		m_mapNumbers[m_pkBtnNums[i]->GetUID()] = i;
		m_pkBtnNums[i]->InitState(true, true);
		m_pkBtnNums[i]->DisconnectAllEventProcedure();
		m_pkBtnNums[i]->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCSellCountItemDlg::OnClickNumber);
	}
	m_pkBtnNums[0]->SetHotKey(DIK_0);
	m_pkBtnNums[0]->SetHotKey(DIK_NUMPAD0);
	m_pkBtnNums[1]->SetHotKey(DIK_1);
	m_pkBtnNums[1]->SetHotKey(DIK_NUMPAD1);
	m_pkBtnNums[2]->SetHotKey(DIK_2);
	m_pkBtnNums[2]->SetHotKey(DIK_NUMPAD2);
	m_pkBtnNums[3]->SetHotKey(DIK_3);
	m_pkBtnNums[3]->SetHotKey(DIK_NUMPAD3);
	m_pkBtnNums[4]->SetHotKey(DIK_4);
	m_pkBtnNums[4]->SetHotKey(DIK_NUMPAD4);
	m_pkBtnNums[5]->SetHotKey(DIK_5);
	m_pkBtnNums[5]->SetHotKey(DIK_NUMPAD5);
	m_pkBtnNums[6]->SetHotKey(DIK_6);
	m_pkBtnNums[6]->SetHotKey(DIK_NUMPAD6);
	m_pkBtnNums[7]->SetHotKey(DIK_7);
	m_pkBtnNums[7]->SetHotKey(DIK_NUMPAD7);
	m_pkBtnNums[8]->SetHotKey(DIK_8);
	m_pkBtnNums[8]->SetHotKey(DIK_NUMPAD8);
	m_pkBtnNums[9]->SetHotKey(DIK_9);
	m_pkBtnNums[9]->SetHotKey(DIK_NUMPAD9);

	m_pkItemImg->InitState(true);
	m_pkItemName->InitState(true);
	m_pkDesc->InitState(true);
	m_pkBtnScroll->InitState(true, true);

	m_pkDesc->SetMultiLine(true);

	m_pkSellTitle->InitState(true);
	m_pkInsertDepotTitle->InitState(false);
	m_pkDeleteDepotTitle->InitState(false);
	m_pkSellCountTitle->InitState(true);
	m_pkInsertDepotCountTitle->InitState(false);
	m_pkDeleteDepotCountTitle->InitState(false);
	m_pkVirtualDepotTitle->InitState(false);

#if defined(NATION_THAILAND)
	m_pkDesc->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_COUNTITEM_DLG_TIP2));
#else
	m_pkDesc->SetTextAutoMultiline(g_pkStrLoader->GetString((rand() % 2 == 0) ? STR_ID_COUNTITEM_DLG_TIP1 : STR_ID_COUNTITEM_DLG_TIP2));
#endif

	SetScrollPos(0);

	UpdateModeState();
}

void KGCSellCountItemDlg::OnClickClose()
{
	if (m_iSellCount != 0)
	{
		SetSellCount(0);
		return;
	}
	else {
		DisconnectGlobalEventProcedure(KD3DWnd::D3DWE_KEY_DOWN);
		SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
	}
}

void KGCSellCountItemDlg::OnClickNumber(const KActionEvent& event)
{
	std::map<KD3DWnd::uid_type, int>::iterator mit;
	mit = m_mapNumbers.find(event.m_pWnd->GetUID());

	if (mit == m_mapNumbers.end())
		return;

	int iCurrent = m_iSellCount * 10 + mit->second;
	if (iCurrent > m_iMaxNum) {
		iCurrent = m_iMaxNum;
	}
	SetSellCount(iCurrent);
}

bool KGCSellCountItemDlg::CheckCondition(IN const KDialogInfo& kInfo_)
{
	m_ItemUID = kInfo_.m_lParam;
	m_emModeState = (int)kInfo_.m_lParam2;

	KItem* pItem = NULL;

	switch (m_emModeState)
	{
	case EM_MODE_DELETE_DEPOT:
		pItem = SiKGCDepotManager()->GetDepotItemInfo(m_ItemUID);
		break;
	case EM_MODE_MOVE_VDEPOT_TO_INVEN:
		if (SiKGCVirtualDepotManager()->GetCurrentTab() == KGCVirtualDepotDlg::EVDEPOT_COMMONTAB)
			pItem = SiKGCVirtualDepotManager()->CommonFindGetItem(m_ItemUID);
		else
			pItem = SiKGCVirtualDepotManager()->ExclusiveFindGetItem(m_ItemUID);
		break;
	case EM_MODE_MOVE_INVEN_TO_VDEPOT:
		pItem = SiKGCVirtualDepotManager()->InvenFindGetItem(m_ItemUID);
		break;
	default:
		pItem = g_pItemMgr->GetInventoryItem(m_ItemUID);
		break;
	}

	if (pItem == NULL) {
		ASSERT(pItem);
		return false;
	}

	GCItem* pkItemInfo = g_pItemMgr->GetItemData(pItem->m_ItemID);
	if (pkItemInfo == NULL) {
		ASSERT(pkItemInfo);
		return false;
	}
	return true;
}

void KGCSellCountItemDlg::OnClickOK()
{
	KItem* pItem = NULL;

	switch (m_emModeState)
	{
	case EM_MODE_DELETE_DEPOT:
		pItem = SiKGCDepotManager()->GetDepotItemInfo(m_ItemUID);
		break;
	case EM_MODE_MOVE_VDEPOT_TO_INVEN:
		if (SiKGCVirtualDepotManager()->GetCurrentTab() == KGCVirtualDepotDlg::EVDEPOT_COMMONTAB)
			pItem = SiKGCVirtualDepotManager()->CommonFindGetItem(m_ItemUID);
		else
			pItem = SiKGCVirtualDepotManager()->ExclusiveFindGetItem(m_ItemUID);
		break;
	case EM_MODE_MOVE_INVEN_TO_VDEPOT:
		pItem = SiKGCVirtualDepotManager()->InvenFindGetItem(m_ItemUID);
		break;
	default:
		pItem = g_pItemMgr->GetInventoryItem(m_ItemUID);
		break;
	}

	if (NULL == pItem) {
		return;
	}

	if (m_iSellCount == 0) {
		OnClose();
		return;
	}

	//수량을 조절하기 위해서 복사
	KItem kSellItem = *pItem;
	kSellItem.m_nCount = m_iSellCount;

	switch (m_emModeState)
	{
	case EM_MODE_SELL_LIST:
	{
		g_pItemMgr->AddSellItemToList(kSellItem);
		g_pkUIMgr->SendEvent(KActionEvent(NULL, KGCUIScene::D3DWE_SELL_ADD, m_ItemUID));
	}
	break;
	case EM_MODE_SELL_INVEN:
	{
		g_pkUIScene->m_pkMyInfoScene->SellInvenItem(kSellItem);
	}
	break;
	case EM_MODE_INSERT_DEPOT_FROM_INVENTORY:
	{
		//  창고에 넣을수 있는 아이템인지 확인
		GCItem* pItemData = g_pItemMgr->GetItemData(pItem->m_ItemID);
		if (!SiKGCDepotManager()->IsInsertDepotItem(pItemData, pItem))
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15), L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
			this->OnClose();
		}

		bool bOK = SiKGCDepotManager()->InsertDepotItem(pItem, m_iSellCount);

		//  창고가 가득 찼습니다.
		if (!bOK)
		{
			if (SiKGCDepotManager()->IsRestrictItem(pItem->m_ItemID)) {
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15), L"",
					KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
			}
			else {
				g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_16), L"",
					KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
			}
			this->OnClose();
		}

		//  갱신
		Result_DepotUpdateItemReq = INT_MAX;
		KP2P::GetInstance()->Send_DepotDBUpdateReq();
		g_MyD3D->WaitForServerAck(Result_DepotUpdateItemReq, INT_MAX, 5000, TIME_OUT_VALUE);
		g_pkUIScene->EnableMessageBox(true);
	}
	break;
	case EM_MODE_INSERT_DEPOT_FROM_DEPOT_UI:
	{
		//  창고에 넣을수 있는 아이템인지 확인
		GCItem* pItemData = g_pItemMgr->GetItemData(pItem->m_ItemID);
		if (!SiKGCDepotManager()->IsInsertDepotItem(pItemData, pItem))
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_15), L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
			this->OnClose();
		}

		bool bOK = SiKGCDepotManager()->InsertDepotItemSetectSlot(pItem, m_iSellCount, SiKGCDepotManager()->GetCurrentSelectSlot());

		//  창고가 가득 찼습니다.
		if (!bOK)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_DEPOT_ERR_DEPOT_02), L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false, true);
			this->OnClose();
		}
	}
	break;
	case EM_MODE_DELETE_DEPOT:
	{
		SiKGCDepotManager()->SendDelDepotItem(pItem, m_iSellCount);
		this->OnClose();
	}
	break;
	case EM_MODE_MOVE_VDEPOT_TO_INVEN:
	{
		SiKGCVirtualDepotManager()->MoveDepotToInvenItem(*pItem, m_iSellCount, true);
		this->OnClose();
	}
	break;
	case EM_MODE_MOVE_INVEN_TO_VDEPOT:
	{
		if (SiKGCVirtualDepotManager()->GetCurrentTab() == KGCVirtualDepotDlg::EVDEPOT_COMMONTAB)
		{
			SiKGCVirtualDepotManager()->MoveInvenToDepotCommon(*pItem, m_iSellCount, true);
		}
		else
			SiKGCVirtualDepotManager()->MoveInvenToDepotCharItem(*pItem, m_iSellCount, true);

		this->OnClose();
	}
	break;
	}
}

//"수량 캐시 아이템 판매시 초기 화폐 표시가 올바르지 않음"문제로 서버에서 화폐정보패킷을 받았을때 업데이트 하는 용도로 만듬
void KGCSellCountItemDlg::SetUpdateUI()
{
	std::pair<int, EMONEYTYPE> pairPrice = g_pItemMgr->GetItemPrice(m_ItemUID, m_iSellCount);
	m_pkMoneyType->SetMoneyType(pairPrice.second);
	m_pkReward->SetNumber(pairPrice.first);
}

void KGCSellCountItemDlg::SetSellCount(int iCount_, bool bUpdateScroll_ /*= true */)
{
	if (iCount_ > m_iMaxNum)
		iCount_ = m_iMaxNum;

	if (iCount_ < 0)
		iCount_ = 0;

	m_iSellCount = iCount_;
	m_pkSellCount->SetNumber(m_iSellCount);

	std::pair<int, EMONEYTYPE> pairPrice = g_pItemMgr->GetItemPrice(m_ItemUID, m_iSellCount);
	m_pkMoneyType->SetMoneyType(pairPrice.second);
	m_pkReward->SetNumber(pairPrice.first);

	SetScrollPos(static_cast<float>(m_iSellCount) / static_cast<float>(m_iMaxNum));
}

void KGCSellCountItemDlg::OnClickMax()
{
	SetSellCount(m_iMaxNum);
}

void KGCSellCountItemDlg::OnClickMin()
{
	SetSellCount(1);
}

void KGCSellCountItemDlg::OnClickBackspace()
{
	SetSellCount(m_iSellCount / 10);
}

void KGCSellCountItemDlg::OnClickClear()
{
	SetSellCount(0);
}

void KGCSellCountItemDlg::FrameMoveInEnabledState(void)
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);

	if (g_pkInput->BtnDown(KInput::MBLEFT) && m_pkBtnScroll->CheckPosInWindowBound(vMousePos))
		m_bScrollStart = true;

	if (g_pkInput->BtnUp(KInput::MBLEFT))
		m_bScrollStart = false;

	if (m_bScrollStart) {
		float fMouseX = g_pkInput->GetMousePos().x - m_pkScrollLine->GetRelocatedWindowPos().x - ((m_pkBtnScroll->GetWidth() / 2)*m_fWindowScaleX);

		float fScrollWidth = static_cast<float>(m_pkScrollLine->GetWidth() - m_pkBtnScroll->GetWidth());
		float fPercent = fMouseX / (fScrollWidth*m_fWindowScaleX);
		int iCurrent = static_cast<int>(m_iMaxNum * fPercent);

		SetScrollPos(fPercent);
		SetSellCount(iCurrent, false);
	}
}

void KGCSellCountItemDlg::SetScrollPos(float fPercent_)
{
	if (fPercent_ < 0.f)
		fPercent_ = 0.f;

	if (fPercent_ > 1.f)
		fPercent_ = 1.f;

	D3DXVECTOR2 vBtnPos = m_pkBtnScroll->GetFixedWindowLocalPos();
	D3DXVECTOR2 vLinePos = m_pkScrollLine->GetFixedWindowLocalPos();
	float fScrollWidth = static_cast<float>(m_pkScrollLine->GetWidth() - m_pkBtnScroll->GetWidth());

	vBtnPos.x = vLinePos.x + (fScrollWidth * fPercent_);

	m_pkBtnScroll->SetWindowPos(vBtnPos);
}

bool KGCSellCountItemDlg::InitDialog(IN const KDialogInfo& kInfo_)
{
	KItem* pItem = NULL;

	switch (m_emModeState)
	{
	case EM_MODE_DELETE_DEPOT:
		pItem = SiKGCDepotManager()->GetDepotItemInfo(m_ItemUID);
		break;
	case EM_MODE_MOVE_VDEPOT_TO_INVEN:
		if (SiKGCVirtualDepotManager()->GetCurrentTab() == KGCVirtualDepotDlg::EVDEPOT_COMMONTAB)
			pItem = SiKGCVirtualDepotManager()->CommonFindGetItem(m_ItemUID);
		else
			pItem = SiKGCVirtualDepotManager()->ExclusiveFindGetItem(m_ItemUID);
		break;
	case EM_MODE_MOVE_INVEN_TO_VDEPOT:
		pItem = SiKGCVirtualDepotManager()->InvenFindGetItem(m_ItemUID);
		break;
	default:
		pItem = g_pItemMgr->GetInventoryItem(m_ItemUID);
		break;
	}

	if (NULL == pItem) {
		ASSERT(pItem);
		return false;
	}

	GCItem* pkItemInfo = g_pItemMgr->GetItemData(pItem->m_ItemID);
	if (pkItemInfo == NULL) {
		ASSERT(pkItemInfo);
		return false;
	}

	m_pkItemImg->SetItemInfo(pItem->m_ItemID);
	m_pkItemName->SetText(pkItemInfo->strItemName);
	m_pkItemName->SetAutoScroll(true);
	m_iMaxNum = pItem->m_nCount;
	m_pkOwnCount->SetNumber(pItem->m_nCount);
	SetSellCount(m_iMaxNum);
	return true;
}

void KGCSellCountItemDlg::UpdateModeState()
{
	switch (m_emModeState)
	{
	case EM_MODE_SELL_LIST:
	case EM_MODE_SELL_INVEN:
	{
		m_pkSellTitle->ToggleRender(true);
		m_pkSellCountTitle->ToggleRender(true);

		m_pkInsertDepotTitle->ToggleRender(false);
		m_pkDeleteDepotCountTitle->ToggleRender(false);
		m_pkDeleteDepotTitle->ToggleRender(false);
		m_pkDeleteDepotCountTitle->ToggleRender(false);

		m_pkMoneyType->ToggleRender(true);
		m_pkReward->ToggleRender(true);
		m_pkVirtualDepotTitle->ToggleRender(false);
	}
	break;
	case EM_MODE_INSERT_DEPOT_FROM_INVENTORY:
	case EM_MODE_INSERT_DEPOT_FROM_DEPOT_UI:
	{
		m_pkSellTitle->ToggleRender(false);
		m_pkSellCountTitle->ToggleRender(false);

		m_pkInsertDepotTitle->ToggleRender(true);
		m_pkDeleteDepotCountTitle->ToggleRender(true);
		m_pkDeleteDepotTitle->ToggleRender(false);
		m_pkDeleteDepotCountTitle->ToggleRender(false);

		m_pkMoneyType->ToggleRender(false);
		m_pkReward->ToggleRender(false);
		m_pkVirtualDepotTitle->ToggleRender(false);
	}
	break;
	case EM_MODE_DELETE_DEPOT:
	{
		m_pkSellTitle->ToggleRender(false);
		m_pkSellCountTitle->ToggleRender(false);

		m_pkInsertDepotTitle->ToggleRender(false);
		m_pkDeleteDepotCountTitle->ToggleRender(false);
		m_pkDeleteDepotTitle->ToggleRender(true);
		m_pkDeleteDepotCountTitle->ToggleRender(true);

		m_pkMoneyType->ToggleRender(false);
		m_pkReward->ToggleRender(false);
	}
	break;
	case EM_MODE_MOVE_VDEPOT_TO_INVEN:
	case EM_MODE_MOVE_INVEN_TO_VDEPOT:
	{
		m_pkSellTitle->ToggleRender(false);
		m_pkSellCountTitle->ToggleRender(false);

		m_pkInsertDepotTitle->ToggleRender(false);
		m_pkDeleteDepotCountTitle->ToggleRender(false);
		m_pkDeleteDepotTitle->ToggleRender(false);
		m_pkDeleteDepotCountTitle->ToggleRender(false);

		m_pkMoneyType->ToggleRender(false);
		m_pkReward->ToggleRender(false);
		m_pkVirtualDepotTitle->ToggleRender(true);
	}
	break;
	}
}

void KGCSellCountItemDlg::OnClose()
{
	DisconnectGlobalEventProcedure(KD3DWnd::D3DWE_KEY_DOWN);
	SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
}