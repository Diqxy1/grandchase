#pragma once

#include <atltime.h>
#include <Thread/Locker.h>
#include <KncSmartPtr.h>

#include "Thread/Thread.h"

SmartPointer(IAccepter);

class IAccepter : public KThread
{
protected:
	IAccepter();
public:
	virtual ~IAccepter() {}

	void DumpMaxTimeInfo(OUT std::wostream& stm);
	void ResetMaxTimeInfo();
	virtual bool GetNagleOpt();

protected:
	struct KMaxTimeInfo {
		KncCriticalSection  m_cs;
		std::wstring        m_strIP;
		DWORD               m_dwTerm;
		CTime               m_cTime;
	} m_kMaxTimeInfo;

	std::wstring            m_strClassName;
};