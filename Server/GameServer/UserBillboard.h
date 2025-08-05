#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <deque>
#include "PrimitiveTypedef.h"
#include "UserPacket.h"

class KLuaManager;

class KUserBillboardManager
{
	DeclareSingleton( KUserBillboardManager );
	NiDeclareRootRTTI( KUserBillboardManager );
	DeclToStringW;

public:

	KUserBillboardManager(void);
	~KUserBillboardManager(void);

	bool LoadScript();
	bool _LoadScript( OUT std::string& strScript );
	bool IsEnableBillboard() { return m_bActive; };
	GCITEMID GetBillboardItem(){ return m_dwBillboardItemID; };
	int WaitingMessages() { return m_vecBillboardData.size(); };

	void Tick();

	void InsertBillboardData(std::wstring& message, std::wstring& nickname, DWORD color);

protected:
	bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
	void SendBillboardToCenter();

protected:

	mutable KncCriticalSection  m_csBillboardData;
	time_t						m_tmBegin;
	time_t						m_tmEnd;
	DWORD						m_dwTickGap;
	DWORD						m_dwLastTick;

	GCITEMID					m_dwBillboardItemID;
	bool						m_bActive;
	bool						m_bTimedEvent;
	
	std::deque<KBillboardChat>	m_vecBillboardData;

};

DefSingletonInline( KUserBillboardManager );
DeclOstmOperatorA( KUserBillboardManager );