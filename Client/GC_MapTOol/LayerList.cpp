// LayerList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "LayerList.h"
#include ".\layerlist.h"
#include "LayerManager.h"
#include "MoveObjectDlg.h"
#include "LayerResizing.h"
#include "ChangeLayerDlg.h"
#include "MoveProp.h"
#include "EffectProp.h"

#include "Command/LayerChangeCmd.h"
#include "Command/LayerResizeCmd.h"
#include "Command/LayerDelCmd.h"
#include "Command/LayerPartDelCmd.h"
#include "Command/PasteObejctCmd.h"
#include "Command/InverseLeftRightCmd.h"
#include "Command/LayerPartMoveCmd.h"
#include "Command/LayerZOrderCmd.h"
#include "Command/CommandRecv.h"
#include "Command/AllLayerResizeCmd.h"

#undef LAYERINDEXCONDITION
#define LAYERINDEXCONDITION if( g_AppFormView->GetLayerIndex() < SiCLayerManager()->GetMaxLayerNum() && !vecLayer.empty() )

// CLayerList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLayerList, CDialog)
CLayerList::CLayerList(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerList::IDD, pParent), m_pCopyObject(NULL)
{
}

CLayerList::~CLayerList()
{
	SAFE_DELETE(m_pCopyObject);
}

void CLayerList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYER_LIST, m_lcLayerList);
	DDX_Control(pDX, IDC_OBJECT_LIST, m_lcObjectList);
	DDX_Control(pDX, IDC_LAYER_RESIZE, m_btnResize);
	DDX_Control(pDX, IDC_CHANGE_LAYER, m_btnLayerChange);
	DDX_Control(pDX, IDC_LAYER_ADD, m_btnLayerAdd);
	DDX_Control(pDX, IDC_LAYER_DEL, m_btnLayerDel);
	DDX_Control(pDX, IDC_OBJECT_DEL, m_btnObjectDel);
	DDX_Control(pDX, IDC_OBJECT_CLONE, m_btnObjectClone);
	DDX_Control(pDX, IDC_REVERSE_LEFTRIGHT, m_btnObjectInverse1);
	DDX_Control(pDX, IDC_REVERSE_UPDOWN, m_btnObjectInverse2);
	DDX_Control(pDX, IDC_OBJECT_MOVE, m_btnObjectMove);
	DDX_Control(pDX, IDC_Z_ORDER_UP, m_btnZUp);
	DDX_Control(pDX, IDC_Z_ORDER_DOWN, m_btnZDown);
}

BEGIN_MESSAGE_MAP(CLayerList, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LAYER_LIST, OnNMClickLayerList)
	ON_BN_CLICKED(IDC_OBJECT_MOVE, OnBnClickedObjectMove)
	ON_BN_CLICKED(IDC_OBJECT_DEL, OnBnClickedObjectDel)
	ON_BN_CLICKED(IDC_LAYER_ADD, OnBnClickedLayerAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LAYER_LIST, OnLvnItemchangedLayerList)
	ON_BN_CLICKED(IDC_LAYER_DEL, OnBnClickedLayerDel)
	ON_NOTIFY(NM_CLICK, IDC_OBJECT_LIST, OnNMClickObjectList)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECT_LIST, OnNMDblclkObjectList)
	ON_BN_CLICKED(IDC_LAYER_RESIZE, OnBnClickedLayerResize)
	ON_BN_CLICKED(IDC_CHANGE_LAYER, OnBnClickedChangeLayer)
	ON_BN_CLICKED(IDC_Z_ORDER_UP, OnBnClickedZOrderUp)
	ON_BN_CLICKED(IDC_Z_ORDER_DOWN, OnBnClickedZOrderDown)
	ON_BN_CLICKED(IDC_OBJECT_CLONE, OnBnClickedObjectClone)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJECT_LIST, OnLvnItemchangedObjectList)
	ON_BN_CLICKED(IDC_REVERSE_LEFTRIGHT, OnBnClickedReverseLeftright)
	ON_BN_CLICKED(IDC_REVERSE_UPDOWN, OnBnClickedReverseUpdown)
END_MESSAGE_MAP()

// CLayerList 메시지 처리기입니다.

void CLayerList::OnBnClickedOk()
{
	return;
	OnOK();
}

void CLayerList::OnBnClickedCancel()
{
	return;
	OnCancel();
}

void CLayerList::OnNMClickLayerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION CurSel = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurSel == NULL)
	{
		*pResult = 0;

		m_btnResize.EnableWindow(FALSE);
		m_btnLayerAdd.EnableWindow(FALSE);
		m_btnLayerDel.EnableWindow(FALSE);
		m_btnLayerChange.EnableWindow(FALSE);

		return;
	}
	else
	{
		m_btnResize.EnableWindow();
		m_btnLayerAdd.EnableWindow();
		m_btnLayerDel.EnableWindow();
		m_btnLayerChange.EnableWindow();
	}

	ReloadObjectList(CurSel);

	*pResult = 0;
}

void CLayerList::ReloadObjectList(POSITION CurSel)
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	m_lcObjectList.DeleteAllItems();

	if (CurSel == NULL)
		return;

	g_AppFormView->SetLayerIndex(m_lcLayerList.GetItemData(m_lcLayerList.GetNextSelectedItem(CurSel)));

	LAYERINDEXCONDITION
	{
		std::vector< CLayerPart* >& PartVector = vecLayer[g_AppFormView->GetLayerIndex()]->GetPartVector();

		for (int i = 0; i < (int)PartVector.size(); ++i)
		{
			int nItem = m_lcObjectList.GetItemCount();
			m_lcObjectList.InsertItem(nItem, GetPartName(PartVector[i]).c_str());
			m_lcObjectList.SetItemData(nItem, (DWORD_PTR)PartVector[i]);
			m_lcObjectList.SetCheck(nItem, PartVector[i]->GetShow());
		}
	}
}

void CLayerList::ReloadObjectList(int iIndex)
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	m_lcObjectList.DeleteAllItems();

	if (iIndex == -1)
		return;

	g_AppFormView->SetLayerIndex(m_lcLayerList.GetItemData(iIndex));

	LAYERINDEXCONDITION
	{
		std::vector< CLayerPart* >& PartVector = vecLayer[g_AppFormView->GetLayerIndex()]->GetPartVector();

		for (int i = 0; i < (int)PartVector.size(); ++i)
		{
			int nItem = m_lcObjectList.GetItemCount();
			m_lcObjectList.InsertItem(nItem, GetPartName(PartVector[i]).c_str());
			m_lcObjectList.SetItemData(nItem, (DWORD_PTR)PartVector[i]);
			m_lcObjectList.SetCheck(nItem, PartVector[i]->GetShow());
		}
	}
}

std::string CLayerList::GetPartName(CLayerPart* pPart)
{
	std::stringstream str;
	if (pPart->GetObjectName() == "")
		str << pPart->GetFileName() << " : " << pPart->GetPartName() << " (" << pPart->GetZOrder() << ")";
	else
		str << pPart->GetFileName() << " : " << pPart->GetObjectName() << " (" << pPart->GetZOrder() << ")";

	return str.str();
}

void CLayerList::OnBnClickedObjectMove()
{
	POSITION CurObjPos = m_lcObjectList.GetFirstSelectedItemPosition();
	POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();

	if (CurLayerPos == NULL || CurObjPos == NULL)
		return;

	CMoveObjectDlg dlg;

	INT_PTR nRet = -1;
	nRet = dlg.DoModal();

	switch (nRet)
	{
	case -1:
		AfxMessageBox("CMoveObjectDlg 를 생성하는데 실패했습니다.\n");
		break;
	case IDOK:
	{
		// 백업
		int iCurSel = m_lcObjectList.GetNextSelectedItem(CurObjPos);
		CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(iCurSel);
		if (pPart == NULL)
			return;
		CLayerPartMoveCmd* pCmd = new CLayerPartMoveCmd();
		KMoveData kData;
		kData.m_iCurLayerIndex = pPart->GetLayer();
		kData.m_iCurZOrder = pPart->GetZOrder();

		// [8/21/2007 breadceo] 지우지만 백업해둔다.
		std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
		vecLayer[kData.m_iCurLayerIndex]->EraseLayerObject(pPart);
		vecLayer[kData.m_iCurLayerIndex]->ResetZOrder(pPart->GetZOrder());
		m_lcObjectList.DeleteItem(iCurSel);
		vecLayer[kData.m_iCurLayerIndex]->SortByZOrder();

		g_AppFormView->SetLayerIndex(g_AppFormView->GetChangeLayerIndex());
		int iLayer = g_AppFormView->GetLayerIndex();
		pPart->SetLayer(iLayer);

		// 옮기기
		m_lcLayerList.SetItemState(iLayer, LVIS_SELECTED, LVIS_SELECTED);
		POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();
		// Z - order 구하기 위해
		ReloadObjectList(CurLayerPos);
		LAYERINDEXCONDITION
		{
			pPart->SetZOrder(m_lcObjectList.GetItemCount());
			vecLayer[iLayer]->AddLayerPart(pPart);
			ReloadLayerList();
			ReloadObjectList(kData.m_iCurLayerIndex);
			ReloadObjectList(iLayer);
		}

		kData.m_iAfterLayerIndex = pPart->GetLayer();
		kData.m_iAfterZOrder = pPart->GetZOrder();

		pCmd->SetMoveData(kData, pPart);
		g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
		g_AppFormView->AddLog(pCmd->GetLogStr());

		// 표시해주기
		m_lcLayerList.SetItemState(iLayer, LVIS_SELECTED, LVIS_SELECTED);
		m_lcObjectList.SetItemState(m_lcObjectList.GetItemCount() - 1, LVIS_SELECTED, LVIS_SELECTED);
		m_lcObjectList.EnsureVisible(m_lcObjectList.GetItemCount() - 1, FALSE);
	}
	break;
	}
}

void CLayerList::OnBnClickedObjectDel()
{
	POSITION CurSel = m_lcObjectList.GetFirstSelectedItemPosition();
	POSITION ListCurSel = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurSel == NULL)
		return;

	CLayerPartDelCmd* pCmd = new CLayerPartDelCmd();

	int iLayerIndex = g_AppFormView->GetLayerIndex();
	int iCurSel = m_lcObjectList.GetNextSelectedItem(CurSel);

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(iCurSel);

	int DeletedZOrder = pPart->GetZOrder();
	vecLayer[iLayerIndex]->EraseLayerObject(pPart);
	vecLayer[iLayerIndex]->ResetZOrder(DeletedZOrder);

	m_lcObjectList.DeleteItem(iCurSel);
	vecLayer[iLayerIndex]->SortByZOrder();

	pCmd->SetDeletePart(iLayerIndex, DeletedZOrder, pPart);
	g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
	g_AppFormView->AddLog(pCmd->GetLogStr());

	m_lcObjectList.SetItemState(iCurSel, LVIS_SELECTED, LVIS_SELECTED);

	if (ListCurSel == NULL)
		return;

	ReloadLayerList();
	ReloadObjectList(ListCurSel);
	SiCLayerManager()->UpdateCategoryUse();

	g_AppFormView->GetRenderView()->ClearCursor();
	g_AppFormView->GetRenderView()->ClearDragPart();
}

void CLayerList::ReloadLayerList()
{
	g_AppFormView->SetListLoaded(false);

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	m_lcLayerList.DeleteAllItems();
	int PlayerLayer = SiCLayerManager()->GetPlayerLayer();
	std::stringstream stm;
	for (int i = 0; i < SiCLayerManager()->GetMaxLayerNum(); ++i)
	{
		std::vector< CLayerPart* >& PartVector = vecLayer[i]->GetPartVector();
		int nItem = m_lcLayerList.GetItemCount();
		stm.str("");
		stm << "Layer " << nItem << " ( " << (int)PartVector.size() << "개 )" << " { " << vecLayer[i]->GetLayerXSize() << ", " << vecLayer[i]->GetLayerYSize() << " }";

		if (i == PlayerLayer)
			stm << " - Player Layer";
		else if (i == 6)
			stm << " - NPC/MONSTER Layer";
		else if (i == 7)
			stm << " - Extra Unit Layer";
		else if (i == PlayerLayer + PLAYER_FOOTHOLD_LAYER_TERM)
			stm << " - FootHold Layer";
		else if (i == LAST_LAYER - 1 || i == LAST_LAYER - 2 || i == LAST_LAYER - 3)
			stm << " - Scrolled View";
		else if (i == PlayerLayer - 1)
			stm << " - 플레이어 앞";

		m_lcLayerList.InsertItem(nItem, stm.str().c_str());
		m_lcLayerList.SetItemData(nItem, i);
		m_lcLayerList.SetCheck(nItem, vecLayer[i]->GetShow());
	}

	m_lcLayerList.EnsureVisible(g_AppFormView->GetLayerIndex(), FALSE);

	g_AppFormView->SetListLoaded(true);
}

void CLayerList::OnBnClickedLayerAdd()
{
	AfxMessageBox("레이어 추가는\n아직 지원되지 않는 기능입니다.\n일정에 쫓기다 보니 그만 ㅜ.ㅜ");
}

BOOL CLayerList::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lcLayerList.InsertColumn(0, "List #", LVCFMT_LEFT, 490);
	m_lcLayerList.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lcObjectList.InsertColumn(0, "Parts Name", LVCFMT_LEFT, 490);
	m_lcObjectList.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLayerList::OnLvnItemchangedLayerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	g_AppFormView->DisableAllProperty(TRUE);

	// 추가 되는 경우가 아니면
	if (pNMLV->uOldState != 0)
	{
		if (!(pNMLV->uNewState & LVIS_SELECTED))
		{
			if (m_lcLayerList.GetItemCount() > pNMLV->iItem && pNMLV->iItem > -1)
			{
				if (((BOOL)(pNMLV->uNewState >> 12) - 1) != ((BOOL)(pNMLV->uOldState >> 12) - 1))
				{
					std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
					if (!vecLayer.empty() && g_AppFormView->GetScriptLoaded())
						vecLayer[pNMLV->iItem]->SetShow(ListView_GetCheckState(m_lcLayerList.m_hWnd, pNMLV->iItem));
				}
			}
		}
	}
	*pResult = 0;
}

void CLayerList::OnBnClickedLayerDel()
{
	int nRet = AfxMessageBox("이 레이어의 모든 내용이 사라집니다. 계속하시겠습니까?", MB_YESNO);
	if (nRet == IDYES)
	{
		POSITION CurLayer = m_lcLayerList.GetFirstSelectedItemPosition();
		if (CurLayer == NULL)
		{
			AfxMessageBox("레이어를 선택해 주세요\n");
			return;
		}

		CLayerDelCmd* pCmd = new CLayerDelCmd();

		std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
		std::vector< CLayerPart* >& vecPart = vecLayer[g_AppFormView->GetLayerIndex()]->GetPartVector();

		pCmd->SetDeleteVector(g_AppFormView->GetLayerIndex(), vecPart);
		vecPart.clear();

		g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
		g_AppFormView->AddLog(pCmd->GetLogStr());

		ReloadLayerList();
		ReloadObjectList(CurLayer);
		SiCLayerManager()->UpdateCategoryUse();

		m_lcLayerList.SetItemState(g_AppFormView->GetLayerIndex(), LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CLayerList::OnBnClickedLayerResize()
{
	CLayerResizing dlg;
	INT_PTR nRet = -1;

	int iLayerIndex = g_AppFormView->GetLayerIndex();

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	LAYERINDEXCONDITION
	{
		float LayerWidth = vecLayer[iLayerIndex]->GetLayerXSize();
		float LayerHeight = vecLayer[iLayerIndex]->GetLayerYSize();

		dlg.SetSize(static_cast<int>(LayerWidth), static_cast<int>(LayerHeight));
	}

	nRet = dlg.DoModal();

	switch (nRet)
	{
	case -1:
		AfxMessageBox("Create LayerResizing Dlg Failed!");
		break;
	case IDOK:
	{
		POSITION CurPos = m_lcLayerList.GetFirstSelectedItemPosition();

		std::stringstream str;
		float Width, Height;
		str << dlg.GetWidth().GetBuffer();
		str >> Width;
		str.clear();
		str << dlg.GetHeight().GetBuffer();
		str >> Height;

		if (dlg.GetCheckAll() == FALSE)
		{
			CLayerResizeCmd* pCmd = new CLayerResizeCmd();
			LayerResizeValue kValue;
			kValue.m_iLayerIndex = iLayerIndex;
			kValue.m_fOldWidth = vecLayer[iLayerIndex]->GetLayerXSize();
			kValue.m_fOldHeight = vecLayer[iLayerIndex]->GetLayerYSize();
			kValue.m_fNewWidth = Width;
			kValue.m_fNewHeight = Height;

			vecLayer[iLayerIndex]->SetLayerXSize(Width);
			vecLayer[iLayerIndex]->SetLayerYSize(Height);

			int iCurPos = m_lcLayerList.GetNextSelectedItem(CurPos);
			kValue.m_iCurPos = iCurPos;
			ReloadLayerList();
			m_lcLayerList.SetItemState(iCurPos, LVIS_SELECTED, LVIS_SELECTED);
			ReloadObjectList(iCurPos);

			pCmd->SetReseizeValue(kValue);
			g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
			g_AppFormView->AddLog(pCmd->GetLogStr());
		}
		else
		{
			CAllLayerResizeCmd* pCmd = new CAllLayerResizeCmd();

			for (int i = 0; i < (int)vecLayer.size(); ++i)
			{
				AllLayerResizeValue kValue;
				kValue.m_fOldWidth = vecLayer[i]->GetLayerXSize();
				kValue.m_fOldHeight = vecLayer[i]->GetLayerYSize();
				kValue.m_fNewWidth = Width;
				kValue.m_fNewHeight = Height;

				pCmd->push_back(kValue);

				vecLayer[i]->SetLayerXSize(Width);
				vecLayer[i]->SetLayerYSize(Height);
			}

			int iCurPos = m_lcLayerList.GetNextSelectedItem(CurPos);
			pCmd->SetCurPos(iCurPos);
			ReloadLayerList();
			m_lcLayerList.SetItemState(iCurPos, LVIS_SELECTED, LVIS_SELECTED);
			ReloadObjectList(iCurPos);

			pCmd->SetReseizeValue();
			g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
			g_AppFormView->AddLog(pCmd->GetLogStr());
		}
	}
	break;
	case IDCANCEL:
		break;
	}
}

void CLayerList::DeleteAllItems()
{
	m_lcLayerList.DeleteAllItems();
	m_lcObjectList.DeleteAllItems();
}

bool CLayerList::GetCurLayer(int& iLayer)
{
	POSITION CurSel = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurSel == NULL)
	{
		AfxMessageBox("Layer를 선택하세요!");
		return false;
	}

	iLayer = m_lcLayerList.GetItemData(m_lcLayerList.GetNextSelectedItem(CurSel));

	return true;
}

void CLayerList::OnNMClickObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION CurSel = m_lcObjectList.GetFirstSelectedItemPosition();
	if (CurSel == NULL)
	{
		*pResult = 0;
		g_AppFormView->DisableAllProperty(TRUE);
		return;
	}
	else
	{
		g_AppFormView->DisableAllProperty(FALSE);
	}

	m_lcObjectList.GetNextSelectedItem(CurSel);

	*pResult = 0;
}

void CLayerList::DisableAllProperty(BOOL bDisable)
{
	m_btnZUp.EnableWindow(!bDisable);
	m_btnZDown.EnableWindow(!bDisable);
	m_btnObjectDel.EnableWindow(!bDisable);
	m_btnObjectClone.EnableWindow(!bDisable);
	//m_btnObjectInverse1.EnableWindow( !bDisable );
	//m_btnObjectInverse2.EnableWindow( !bDisable );
	m_btnObjectMove.EnableWindow(!bDisable);
}

void CLayerList::OnNMDblclkObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION CurSel = m_lcObjectList.GetFirstSelectedItemPosition();
	if (CurSel != NULL)
	{
		CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(m_lcObjectList.GetNextSelectedItem(CurSel));

		KLayerRect<float> kRect;
		pPart->GetLayerRect(kRect);

		if (kRect.m_X < 400.0f)
			kRect.m_X = 400.0f;
		else if (kRect.m_X + 400.0f > SiKGCMap()->GetPixelWidth())
			kRect.m_X = SiKGCMap()->GetPixelWidth() - 400.0f;

		if (kRect.m_Y < 300.0f)
			kRect.m_Y = 300.0f;
		else if (kRect.m_Y + 300.0f > SiKGCMap()->GetPixelHeight())
			kRect.m_Y = SiKGCMap()->GetPixelHeight() - 300.0f;

		D3DXMATRIX matView;
		D3DXVECTOR3 vFromPt = D3DXVECTOR3(static_cast<FLOAT>(kRect.m_X), static_cast<FLOAT>(kRect.m_Y), 0.0f);
		D3DXVECTOR3 vLookatPt = D3DXVECTOR3(static_cast<FLOAT>(kRect.m_X), static_cast<FLOAT>(kRect.m_Y), 1.0f);
		D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		g_AppFormView->GetRenderView()->SetEyeX(kRect.m_X);
		g_AppFormView->GetRenderView()->SetEyeY(kRect.m_Y);
	}
	*pResult = 0;
}

void CLayerList::ReloadCurLayerListObjectList()
{
	POSITION CurPos = m_lcLayerList.GetFirstSelectedItemPosition();
	ReloadLayerList();
	ReloadObjectList(CurPos);
}

void CLayerList::CopyObject()
{
	SAFE_DELETE(m_pCopyObject);
	POSITION CurPos = m_lcObjectList.GetFirstSelectedItemPosition();
	if (CurPos != NULL)
	{
		CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(m_lcObjectList.GetNextSelectedItem(CurPos));
		m_pCopyObject = pPart->Clone();
	}
}

void CLayerList::PasteObject()
{
	POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurLayerPos != NULL)
	{
		if (m_pCopyObject != NULL)
		{
			std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
			LAYERINDEXCONDITION
			{
				CPasteObejctCmd* pCmd = new CPasteObejctCmd();

				CLayerPart* pInsertPart = m_pCopyObject->Clone();
				pInsertPart->SetZOrder(m_lcObjectList.GetItemCount());
				vecLayer[g_AppFormView->GetLayerIndex()]->AddLayerPart(pInsertPart);
				ReloadLayerList();
				ReloadObjectList(CurLayerPos);
				m_lcObjectList.EnsureVisible(m_lcObjectList.GetItemCount(), FALSE);

				pCmd->SetPasteData(g_AppFormView->GetLayerIndex(), pInsertPart->GetZOrder(), pInsertPart);
				g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
				g_AppFormView->AddLog(pCmd->GetLogStr());
			}
		}
	}
}

void CLayerList::OnBnClickedChangeLayer()
{
	CChangeLayerDlg dlg;

	INT_PTR nRet = -1;
	nRet = dlg.DoModal();

	switch (nRet)
	{
	case -1:
		break;
	case IDOK:
	{
		CLayerChangeCmd* pCmd = new CLayerChangeCmd();

		int iChangeLayer = g_AppFormView->GetChangeLayerIndex();
		int iLayer = g_AppFormView->GetLayerIndex();
		ChangeLayer(iLayer, iChangeLayer);
		pCmd->ChangeLayer(iLayer, iChangeLayer);

		g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
		g_AppFormView->AddLog(pCmd->GetLogStr());
	}
	break;
	}
}

void CLayerList::ChangeLayer(int iFirst, int iSecond)
{
	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();

	LAYERINDEXCONDITION
	{
		if (iSecond < SiCLayerManager()->GetMaxLayerNum() && !vecLayer.empty())
		{
			// 백업
			std::vector< CLayerPart* >& PartVector1 = vecLayer[iFirst]->GetPartVector();
			std::vector< CLayerPart* >& PartVector2 = vecLayer[iSecond]->GetPartVector();
			std::vector< CLayerPart* > pPartBack1, pPartBack2;
			for (int i = 0; i < (int)PartVector1.size(); ++i)
				pPartBack1.push_back(PartVector1[i]->Clone());
			for (int i = 0; i < (int)PartVector2.size(); ++i)
				pPartBack2.push_back(PartVector2[i]->Clone());

			// 지우고
			vecLayer[iFirst]->clear();
			vecLayer[iSecond]->clear();

			// 넣어주기
			for (int i = 0; i < (int)pPartBack2.size(); ++i)
			{
				pPartBack2[i]->SetLayer(iFirst);
				vecLayer[iFirst]->AddLayerPart(pPartBack2[i]);
			}
			for (int i = 0; i < (int)pPartBack1.size(); ++i)
			{
				pPartBack1[i]->SetLayer(iSecond);
				vecLayer[iSecond]->AddLayerPart(pPartBack1[i]);
			}

			ReloadLayerList();
			m_lcObjectList.DeleteAllItems();
		}
	}
}

// 귀차니즘 + 가독성의 산실
void CLayerList::OnBnClickedZOrderUp()
{
	POSITION CurSel = m_lcObjectList.GetFirstSelectedItemPosition();
	if (CurSel == NULL)
		return;

	int iCurSel = m_lcObjectList.GetNextSelectedItem(CurSel);

	// 바운드 체크
	if (iCurSel == 0)
		return;

	// 올릴 데이터 백업
	CLayerPart* pPartSrc = (CLayerPart*)m_lcObjectList.GetItemData(iCurSel);
	CLayerPart* pPartDest = (CLayerPart*)m_lcObjectList.GetItemData(iCurSel - 1);

	CLayerZOrderCmd* pCmd = new CLayerZOrderCmd();

	// Z오더를 바꾼다.
	int tempZOrder = 0;
	tempZOrder = pPartDest->GetZOrder();
	pCmd->SetZOrder(tempZOrder, pPartSrc);

	pPartDest->SetZOrder(pPartSrc->GetZOrder());
	pPartSrc->SetZOrder(tempZOrder);

	std::stringstream str;
	str << pPartSrc->GetPartName() << " (" << pPartSrc->GetZOrder() << ")";
	str.str(std::string());
	m_lcObjectList.SetItemData(iCurSel - 1, (DWORD_PTR)pPartSrc);
	m_lcObjectList.SetItemText(iCurSel - 1, 0, GetPartName(pPartSrc).c_str());
	str << pPartDest->GetPartName() << " (" << pPartDest->GetZOrder() << ")";
	m_lcObjectList.SetItemData(iCurSel, (DWORD_PTR)pPartDest);
	m_lcObjectList.SetItemText(iCurSel, 0, GetPartName(pPartDest).c_str());

	m_lcObjectList.SetItemState(iCurSel - 1, LVIS_SELECTED, LVIS_SELECTED);

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[g_AppFormView->GetLayerIndex()]->SortByZOrder();

	g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
	g_AppFormView->AddLog(pCmd->GetLogStr());
}

void CLayerList::OnBnClickedZOrderDown()
{
	POSITION CurSel = m_lcObjectList.GetFirstSelectedItemPosition();
	if (CurSel == NULL)
		return;

	int iCurSel = m_lcObjectList.GetNextSelectedItem(CurSel);

	// 젤 위에 인덱스면 무시
	if (iCurSel == m_lcObjectList.GetItemCount() - 1)
		return;

	// 올릴 데이터 백업
	CLayerPart* pPartSrc = (CLayerPart*)m_lcObjectList.GetItemData(iCurSel);
	CLayerPart* pPartDest = (CLayerPart*)m_lcObjectList.GetItemData(iCurSel + 1);

	CLayerZOrderCmd* pCmd = new CLayerZOrderCmd();

	// Z오더를 바꾼다.
	int tempZOrder = 0;
	tempZOrder = pPartDest->GetZOrder();
	pCmd->SetZOrder(tempZOrder, pPartSrc);

	pPartDest->SetZOrder(pPartSrc->GetZOrder());
	pPartSrc->SetZOrder(tempZOrder);

	// 그냥 for 돌려도 될건데 --;
	std::stringstream str;
	str << pPartSrc->GetPartName() << " (" << pPartSrc->GetZOrder() << ")";
	m_lcObjectList.SetItemData(iCurSel + 1, (DWORD_PTR)pPartSrc);
	m_lcObjectList.SetItemText(iCurSel + 1, 0, GetPartName(pPartSrc).c_str());
	str.str(std::string());
	str << pPartDest->GetPartName() << " (" << pPartDest->GetZOrder() << ")";
	m_lcObjectList.SetItemData(iCurSel, (DWORD_PTR)pPartDest);
	m_lcObjectList.SetItemText(iCurSel, 0, GetPartName(pPartDest).c_str());

	m_lcObjectList.SetItemState(iCurSel + 1, LVIS_SELECTED, LVIS_SELECTED);

	std::vector< CLayer* >& vecLayer = SiCLayerManager()->GetLayer();
	vecLayer[g_AppFormView->GetLayerIndex()]->SortByZOrder();

	g_AppFormView->GetCommandRecv()->AddCommand(static_cast<ICommand*>(pCmd));
	g_AppFormView->AddLog(pCmd->GetLogStr());
}

void CLayerList::PasteCurObject()
{
	POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();
	PasteObject();
	if (CurLayerPos != NULL)
		m_lcLayerList.SetItemState(m_lcLayerList.GetNextSelectedItem(CurLayerPos), LVIS_SELECTED, LVIS_SELECTED);
}

// 아 이런거 싫은데..
CLayerPart* CLayerList::GetCurItemData(OUT int* pIRet/* = NULL*/)
{
	POSITION CurSel = m_lcObjectList.GetFirstSelectedItemPosition();
	if (CurSel != NULL)
	{
		int iCurSel = m_lcLayerList.GetNextSelectedItem(CurSel);
		if (pIRet != NULL)
			*pIRet = iCurSel;

		return (CLayerPart*)m_lcObjectList.GetItemData(iCurSel);
	}
	else
	{
		return NULL;
	}
}

void CLayerList::OnBnClickedObjectClone()
{
	POSITION CurPos = m_lcObjectList.GetFirstSelectedItemPosition();
	POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurPos == NULL || CurLayerPos == NULL)
		return;

	int iPos = m_lcObjectList.GetNextSelectedItem(CurPos);
	CopyObject();
	PasteObject();

	m_lcObjectList.SetItemState(iPos, LVIS_SELECTED, LVIS_SELECTED);
	m_lcLayerList.SetItemState(m_lcLayerList.GetNextSelectedItem(CurLayerPos), LVIS_SELECTED, LVIS_SELECTED);
}

void CLayerList::Reset()
{
	m_lcObjectList.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
	m_lcLayerList.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
}

void CLayerList::OnLvnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	g_AppFormView->DisableAllProperty(TRUE);

	if (pNMLV->uNewState & LVIS_SELECTED)
	{
		CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(pNMLV->iItem);
		if (pPart != NULL)
		{
			g_AppFormView->SetBoxObj(pPart);
			g_AppFormView->DisableAllProperty(FALSE);
			g_AppFormView->SetControlValue(pPart);

			if (pPart->GetMoveable())
			{
				g_AppFormView->GetMoveProp()->Enable(TRUE);
			}
		}
	}
	else
	{
		if (m_lcObjectList.GetItemCount() > pNMLV->iItem && pNMLV->iItem > -1)
		{
			if (((BOOL)(pNMLV->uNewState >> 12) - 1) != ((BOOL)(pNMLV->uOldState >> 12) - 1))
			{
				CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(pNMLV->iItem);
				if (pPart != NULL)
				{
					pPart->SetShow(ListView_GetCheckState(m_lcObjectList.m_hWnd, pNMLV->iItem));
				}
			}
		}
	}
	*pResult = 0;
}

void CLayerList::OnBnClickedReverseLeftright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION CurPos = m_lcObjectList.GetFirstSelectedItemPosition();
	POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurPos == NULL || CurLayerPos == NULL)
		return;

	int iPos = m_lcObjectList.GetNextSelectedItem(CurPos);
	CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(iPos);
	if (pPart == NULL)
		return;

	pPart->Inverse(true);
	g_AppFormView->SetControlValue(pPart);

	m_lcLayerList.SetItemState(m_lcLayerList.GetNextSelectedItem(CurLayerPos), LVIS_SELECTED, LVIS_SELECTED);
	m_lcObjectList.SetItemState(iPos, LVIS_SELECTED, LVIS_SELECTED);
}

void CLayerList::OnBnClickedReverseUpdown()
{
	POSITION CurPos = m_lcObjectList.GetFirstSelectedItemPosition();
	POSITION CurLayerPos = m_lcLayerList.GetFirstSelectedItemPosition();
	if (CurPos == NULL || CurLayerPos == NULL)
		return;

	int iPos = m_lcObjectList.GetNextSelectedItem(CurPos);
	CLayerPart* pPart = (CLayerPart*)m_lcObjectList.GetItemData(iPos);
	if (pPart == NULL)
		return;

	pPart->Inverse(false);
	g_AppFormView->SetControlValue(pPart);

	m_lcLayerList.SetItemState(m_lcLayerList.GetNextSelectedItem(CurLayerPos), LVIS_SELECTED, LVIS_SELECTED);
	m_lcObjectList.SetItemState(iPos, LVIS_SELECTED, LVIS_SELECTED);
}