#include "stdafx.h"
#include ".\commandrecv.h"
#include "ICommand.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

CCommandRecv::CCommandRecv(void) : m_strLastLog("")
{
}

CCommandRecv::~CCommandRecv(void)
{
	Init();
}

bool CCommandRecv::Undo()
{
	if (m_UndoStack.empty())
		return false;

	ICommand* pCmd = m_UndoStack.top();
	pCmd->Undo();
	m_RedoStack.push(pCmd);
	m_UndoStack.pop();
	m_strLastLog = pCmd->GetLogStr();

	return true;
}

bool CCommandRecv::Redo()
{
	if (m_RedoStack.empty())
		return false;

	ICommand* pCmd = m_RedoStack.top();
	pCmd->Redo();
	m_UndoStack.push(pCmd);
	m_RedoStack.pop();
	m_strLastLog = pCmd->GetLogStr();

	return true;
}

void CCommandRecv::AddCommand(ICommand* pCmd)
{
	m_UndoStack.push(pCmd);
	ClearStack(m_RedoStack);
}

void CCommandRecv::ClearStack(std::stack< ICommand* >& kStack)
{
	ICommand* pCmd = NULL;
	while (!kStack.empty())
	{
		pCmd = kStack.top();
		kStack.pop();
		SAFE_DELETE(pCmd);
	}
}
void CCommandRecv::Init()
{
	ClearStack(m_UndoStack);
	ClearStack(m_RedoStack);
}