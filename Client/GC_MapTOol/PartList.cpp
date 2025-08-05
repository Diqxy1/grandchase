// PartList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "PartList.h"
#include ".\partlist.h"
#include "LayerManager.h"
#include "staticrenderview.h"
#include <dbg/dbg.hpp>

// CPartList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPartList, CDialog)
CPartList::CPartList(CWnd* pParent /*=NULL*/)
	: CDialog(CPartList::IDD, pParent)
{
}

CPartList::~CPartList()
{
}

void CPartList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CATEGORY, m_lbCategory);
	DDX_Control(pDX, IDC_PARTS, m_lbParts);
}

BEGIN_MESSAGE_MAP(CPartList, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_CATEGORY, OnLbnSelchangeCategory)
	ON_LBN_SELCHANGE(IDC_PARTS, OnLbnSelchangeParts)
END_MESSAGE_MAP()

// CPartList 메시지 처리기입니다.

void CPartList::OnBnClickedOk()
{
	// 항상 띄우고 싶어혀
	return;
	OnOK();
}

void CPartList::OnBnClickedCancel()
{
	// 항상 띄우고 싶어혀
	return;
	OnCancel();
}

void CPartList::OnLbnSelchangeCategory()
{
	JIF(m_lbCategory.GetCurSel() != -1);

	std::vector< std::string > CategoryList = SiCLayerManager()->GetCategoryList();
	SetCategoryMember(CategoryList[m_lbCategory.GetCurSel()]);
}

void CPartList::SetCategoryMember(std::string CategoryName)
{
	m_lbParts.ResetContent();
	assert(m_lbParts.GetCount() == 0);

	std::map< std::string, CLayerPart* >& PartPool = SiCLayerManager()->GetPartPool();
	std::map< std::string, CLayerPart* >::iterator iter;
	int CategoryNum = 0;
	for (iter = PartPool.begin(); iter != PartPool.end(); ++iter)
	{
		if (iter->second != NULL && iter->second->GetPartCategory() == CategoryName)
		{
			m_lbParts.AddString(iter->second->GetPartName().c_str());
			m_lbParts.SetItemData(CategoryNum, (DWORD_PTR)PartPool[iter->second->GetPartName()]);
			CategoryNum++;
		}
	}
}

void CPartList::OnLbnSelchangeParts()
{
	JIF(m_lbParts.GetCurSel() != -1);

	CLayerPart* pPart = (CLayerPart*)m_lbParts.GetItemData(m_lbParts.GetCurSel());

	if (pPart != NULL)
		g_AppFormView->GetRenderView()->CreateCursorPart(pPart->GetPartName());
}

void CPartList::ResetContent()
{
	m_lbCategory.ResetContent();
	m_lbParts.ResetContent();
}

void CPartList::UpdateCategoryList()
{
	ResetContent();

	std::vector< std::string > CategoryList = SiCLayerManager()->GetCategoryList();
	for (int i = 0; i < (int)CategoryList.size(); ++i)
	{
		m_lbCategory.AddString(CategoryList[i].c_str());
		m_lbCategory.SetItemData(i, i);
	}
}

void CPartList::EnableWindow(BOOL bEnable/* = TRUE*/)
{
	m_lbCategory.EnableWindow(bEnable);
	m_lbParts.EnableWindow(bEnable);
}