#include ".\createlayerpartcreatecaterogycmd.h"

CCreateLayerPartCreateCaterogyCmd::CCreateLayerPartCreateCaterogyCmd(void)
	: m_strCategory("")
{
}

CCreateLayerPartCreateCaterogyCmd::~CCreateLayerPartCreateCaterogyCmd(void)
{
}

void CCreateLayerPartCreateCaterogyCmd::Undo()
{
	g_LayerToolDlg->DeleteNewCategory(m_strCategory);

	std::stringstream stm;
	stm << "Undo] Delete Category [ " << m_strCategory << " ]";
	m_strLog = stm.str();
}

void CCreateLayerPartCreateCaterogyCmd::Redo()
{
	g_LayerToolDlg->AddNewCategory(m_strCategory);

	std::stringstream stm;
	stm << "Redo] New Category [ " << m_strCategory << " ]";
	m_strLog = stm.str();
}

void CCreateLayerPartCreateCaterogyCmd::SetCategoryname(const std::string& strCategory)
{
	m_strCategory = strCategory;

	std::stringstream stm;
	stm << "New Category [ " << m_strCategory << " ]";
	m_strLog = stm.str();
}