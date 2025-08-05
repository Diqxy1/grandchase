#pragma once

//#include <string>

class ICommand
{
public:
	ICommand() : m_strLog("Default Command") {}
	virtual ~ICommand() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual std::string& GetLogStr() { return m_strLog; }

protected:
	std::string m_strLog;
};
