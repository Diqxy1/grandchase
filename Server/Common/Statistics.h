#pragma once
#include <WTypes.h>
#include <Thread/Locker.h>
#include "KncSmartPtr.h"
#include <ToString.h>

class KStatistics
{
public:
	KStatistics(void);
	virtual~KStatistics(void);

	virtual void Tick() = 0;
	virtual void Dump(std::wostream& stm);

	virtual void Init(int nID, int nFlushGap, bool bWriteToDB, int nRowCount);
	DWORD GetElapsed();
	void ResetTime();
	void SetDesc(const std::wstring& strTile, const std::wstring& strDesc);

protected:
	int                 m_nSIID;
	DWORD               m_dwFlushGap; // 시간 단위..
	DWORD               m_dwRowCount;
	bool                m_bWriteToDB;
	DWORD               m_dwStartTime;
	std::wstring        m_strTitle;
	std::wstring        m_strDesc;

	mutable KncCriticalSection  m_csStat;
};
