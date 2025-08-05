// KCChooseCategoryDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "KCChooseCategoryDlg.h"
#include ".\kcchoosecategorydlg.h"
#include "LayerManager.h"

//#include <vector>
//#include <string>

// KCChooseCategoryDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCChooseCategoryDlg, CDialog)
KCChooseCategoryDlg::KCChooseCategoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KCChooseCategoryDlg::IDD, pParent)
{
}

KCChooseCategoryDlg::~KCChooseCategoryDlg()
{
}

void KCChooseCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CATEGORY_LIST, m_listCategory);
}

BEGIN_MESSAGE_MAP(KCChooseCategoryDlg, CDialog)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickCheckCategoryList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CHECK_CATEGORY_LIST, OnLvnItemActivateCheckCategoryList)
END_MESSAGE_MAP()

// KCChooseCategoryDlg 메시지 처리기입니다.

BOOL KCChooseCategoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listCategory.InsertColumn(0, "Category Name", LVCFMT_LEFT, 490);
	m_listCategory.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	std::vector< std::string > WholeCategory = SiCLayerManager()->GetCategoryList();
	std::vector< std::string > UsingCategory = SiCLayerManager()->GetUsingCategoryList();

	for (int i = 0; i < (int)WholeCategory.size(); ++i)
	{
		int nItem = m_listCategory.GetItemCount();
		m_listCategory.InsertItem(nItem, WholeCategory[i].c_str());

		for (int j = 0; j < (int)UsingCategory.size(); ++j)
		{
			if (WholeCategory[i] == UsingCategory[j])
				m_listCategory.SetCheck(nItem, TRUE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCChooseCategoryDlg::OnHdnItemclickCheckCategoryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;
}

void KCChooseCategoryDlg::OnLvnItemActivateCheckCategoryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
}