#include ".\modifyfhvaluecmd.h"

CModifyFHValueCmd::CModifyFHValueCmd(void)
{
}

CModifyFHValueCmd::~CModifyFHValueCmd(void)
{
}

class CFunctorSetValue : public std::unary_function< VecFootHoldValue, void >
{
public:
	CFunctorSetValue() {}
	~CFunctorSetValue() {}

	void operator()(const PairFootHoldValue& _Left)
	{
		std::pair<int, int> indexPair = _Left.first;
		g_AppFormView->m_ppTObj[indexPair.first][indexPair.second].SetValue(_Left.second);
	}
};

void CModifyFHValueCmd::Undo()
{
	if (m_vecUndo.empty())
		return;

	std::for_each(m_vecUndo.begin(), m_vecUndo.end(), CFunctorSetValue());
	std::stringstream stm;
	if (m_vecUndo[0].second != "")
		stm << "Undo] " << m_vecUndo.size() << " FootHolds Value to " << m_vecUndo[0].second;
	else
		stm << "Undo] " << m_vecUndo.size() << " FootHolds Value to (empty)";
	m_strLog = stm.str();
}

void CModifyFHValueCmd::Redo()
{
	if (m_vecUndo.empty())
		return;

	std::for_each(m_vecRedo.begin(), m_vecRedo.end(), CFunctorSetValue());
	std::stringstream stm;
	if (m_vecRedo[0].second != "")
		stm << "Redo] " << m_vecRedo.size() << " FootHolds Value to " << m_vecRedo[0].second;
	else
		stm << "Redo] " << m_vecRedo.size() << " FootHolds Value to (empty)";
	m_strLog = stm.str();
}

void CModifyFHValueCmd::SetRedoData(const VecFootHoldValue& vecRedo)
{
	m_vecRedo = vecRedo;

	if (m_vecRedo.empty())
		return;

	std::stringstream stm;
	if (m_vecRedo[0].second != "")
		stm << m_vecRedo.size() << " FootHolds Value to " << m_vecRedo[0].second;
	else
		stm << m_vecRedo.size() << " FootHolds Value to (empty)";
	m_strLog = stm.str();
}