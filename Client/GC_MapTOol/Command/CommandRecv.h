#pragma once

#include <stack>
//#include <deque>

class ICommand;

class CCommandRecv
{
public:
	CCommandRecv(void);
	virtual ~CCommandRecv(void);

	bool Undo();
	bool Redo();
	std::string& GetLastLog() { return m_strLastLog; }
	void ClearStack(std::stack< ICommand* >& kStack);
	void AddCommand(ICommand* pCmd);
	void Init();

private:
	std::stack< ICommand* > m_UndoStack;
	std::stack< ICommand* > m_RedoStack;

	std::string m_strLastLog;
};
