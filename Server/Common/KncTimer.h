#pragma once
#include <Windows.h>

class KncTimer
{
	mutable KncCriticalSection m_csTime;
	DWORD   m_dwInterval;
	DWORD   m_dwLastTick;
	bool    m_bAuto;
public:
	KncTimer(DWORD dwInterval, bool bAuto = true)
		:m_dwInterval(dwInterval)
		, m_bAuto(bAuto)
	{
		Reset();
	}

	DWORD GetInterval() const { KLocker lock(m_csTime); return m_dwInterval; }
	DWORD GetLastTick() const { KLocker lock(m_csTime); return m_dwLastTick; }
	bool IsAutoReset() const { KLocker lock(m_csTime); return m_bAuto; }
	void SetAutoReset(bool bAuto) { KLocker lock(m_csTime); m_bAuto = bAuto; }
	void SetInterval(DWORD dwInterval) { KLocker lock(m_csTime); m_dwInterval = dwInterval; }

	void Reset()
	{
		KLocker lock(m_csTime);
		m_dwLastTick = ::GetTickCount();
	}

	bool CheckTime()
	{
		{ // Locking Scope
			KLocker lock(m_csTime);
			if (::GetTickCount() - m_dwLastTick < m_dwInterval) {
				return false;
			}
		}

		if (m_bAuto) {
			Reset();
		}
		return true;
	}
};