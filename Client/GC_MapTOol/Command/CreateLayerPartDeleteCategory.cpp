#include ".\createlayerpartdeletecategory.h"
#include "../KCreateLayerPartDlg.h"

CCreateLayerPartDeleteCategory::CCreateLayerPartDeleteCategory(void)
	: m_strCategory("")
{
}

CCreateLayerPartDeleteCategory::~CCreateLayerPartDeleteCategory(void)
{
}

void CCreateLayerPartDeleteCategory::Undo()
{
	g_LayerToolDlg->AddNewCategory(m_strCategory);

	std::stringstream stm;
	stm << "Undo] Restore Category [ " << m_strCategory << " ]";
	m_strLog = stm.str();
}

void CCreateLayerPartDeleteCategory::Redo()
{
	g_LayerToolDlg->DeleteNewCategory(m_strCategory);

	std::stringstream stm;
	stm << "Redo] Delete Category [ " << m_strCategory << " ]";
	m_strLog = stm.str();
}

void CCreateLayerPartDeleteCategory::SetCategoryName(const std::string& strCategory)
{
	m_strCategory = strCategory;

	std::stringstream stm;
	stm << "Delete Category [ " << m_strCategory << " ]";
	m_strLog = stm.str();
}