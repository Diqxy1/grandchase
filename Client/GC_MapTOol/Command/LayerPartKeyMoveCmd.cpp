#include ".\layerpartkeymovecmd.h"

CLayerPartKeyMoveCmd::CLayerPartKeyMoveCmd(void)
	: m_pPart(NULL)
{
}

CLayerPartKeyMoveCmd::~CLayerPartKeyMoveCmd(void)
{
}

void CLayerPartKeyMoveCmd::Undo()
{
	switch (m_eMoveType)
	{
	case LEFT:
		m_pPart->MoveRight();
		break;
	case RIGHT:
		m_pPart->MoveLeft();
		break;
	case UP:
		m_pPart->MoveDown();
		break;
	case DOWN:
		m_pPart->MoveUp();
		break;
	}
	m_pPart->SetVertex();
	g_AppFormView->SetControlValue(m_pPart);

	std::stringstream stm;
	stm << "Undo] " << GetMoveTypeStr(m_eMoveType) << " " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CLayerPartKeyMoveCmd::Redo()
{
	switch (m_eMoveType)
	{
	case LEFT:
		m_pPart->MoveLeft();
		break;
	case RIGHT:
		m_pPart->MoveRight();
		break;
	case UP:
		m_pPart->MoveUp();
		break;
	case DOWN:
		m_pPart->MoveDown();
		break;
	}
	m_pPart->SetVertex();
	g_AppFormView->SetControlValue(m_pPart);

	std::stringstream stm;
	stm << "Redo] " << GetMoveTypeStr(m_eMoveType) << " " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

void CLayerPartKeyMoveCmd::SetMove(MOVETYPE eType, CLayerPart* pPart)
{
	m_eMoveType = eType;
	m_pPart = pPart;

	std::stringstream stm;
	stm << GetMoveTypeStr(m_eMoveType) << " " << m_pPart->GetLayer() << " Layer : " << m_pPart->GetObjectName()
		<< "( " << m_pPart->GetZOrder() << " )";
	m_strLog = stm.str();
}

std::string CLayerPartKeyMoveCmd::GetMoveTypeStr(MOVETYPE eType)
{
	switch (eType)
	{
	case LEFT:
		return "Left Move";
		break;
	case RIGHT:
		return "Right Move";
		break;
	case UP:
		return "Up Move";
		break;
	case DOWN:
		return "Down Move";
		break;
	}
	return "Invalid Type Move";
}