// KCNewCategory.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOGMapTOol.h"
#include "KCNewCategory.h"
#include "KCreateLayerPartDlg.h"
#include ".\kcnewcategory.h"
#include "resource.h"
#include "LayerManager.h"
//#include <vector>

// KCNewCategory 대화 상자입니다.

IMPLEMENT_DYNCREATE(KCNewCategory, CDialog)

KCNewCategory::KCNewCategory(CWnd* pParent /*=NULL*/)
	: CDialog(KCNewCategory::IDD, pParent)
	, m_strNewCategoryName(_T(""))
{
}

KCNewCategory::~KCNewCategory()
{
}

void KCNewCategory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEW_CATEGORYNAME, m_strNewCategoryName);
	DDX_Control(pDX, IDOK, m_btnOK);
}

BOOL KCNewCategory::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_btnOK.EnableWindow(FALSE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(KCNewCategory, CDialog)
	ON_BN_CLICKED(IDC_NEW_NAME_VERIFY, OnBnClickedNewNameVerify)
	ON_BN_CLICKED(IDOK, OnButtonOK)
	ON_BN_CLICKED(IDCANCEL, OnButtonCancel)
	ON_EN_CHANGE(IDC_NEW_CATEGORYNAME, OnEnChangeNewCategoryname)
END_MESSAGE_MAP()

// KCNewCategory 메시지 처리기입니다.

void KCNewCategory::OnButtonOK()
{
	UpdateData();
	std::string NewName = m_strNewCategoryName.GetBuffer();

	if (g_LayerToolDlg != NULL)
		g_LayerToolDlg->SetNewCategoryName(NewName);

	CDialog::OnOK();
}

void KCNewCategory::OnButtonCancel()
{
	CDialog::OnCancel();
}

void KCNewCategory::OnBnClickedNewNameVerify()
{
	UpdateData();

	std::string CategoryName = m_strNewCategoryName.GetBuffer();

	if (CategoryName == "")
	{
		AfxMessageBox("이름을 입력하세요");
		return;
	}

	std::vector< std::string > CategoryList = SiCLayerManager()->GetCategoryList();
	int i = 0;
	for (; i < (int)CategoryList.size(); ++i)
	{
		if (CategoryName == CategoryList[i])
		{
			AfxMessageBox("중복되는 이름이 있습니다.");
			m_btnOK.EnableWindow(FALSE);
			break;
		}
	}

	// 끝까지 같은 것을 못찾았다면.
	if (i == (int)CategoryList.size())
	{
		AfxMessageBox("사용할 수 있는 이름입니다.");
		m_btnOK.EnableWindow();
	}
}

void KCNewCategory::OnEnChangeNewCategoryname()
{
	m_btnOK.EnableWindow(FALSE);
}