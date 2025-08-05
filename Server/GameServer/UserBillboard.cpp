#include "CnConnector.h" // winsock
#include "UserBillboard.h"
#include "userevent.h"
#include "UserPacket.h"
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "Lua/KLuaManager.h"

ImplementSingleton( KUserBillboardManager );
ImplOstmOperatorW2A( KUserBillboardManager );
NiImplementRootRTTI( KUserBillboardManager );

KUserBillboardManager::KUserBillboardManager() 
: m_bActive(false)
, m_tmBegin(1)
, m_tmEnd(1)
, m_dwBillboardItemID(0)
, m_dwTickGap(20)
{
	LoadScript();
}

KUserBillboardManager::~KUserBillboardManager()
{
	//empty
}

ImplToStringW( KUserBillboardManager )
{
	KLocker lock(m_csBillboardData);
	START_TOSTRINGW
		<< TOSTRINGWt( CTime(m_tmBegin) )
		<< TOSTRINGWt( CTime(m_tmEnd) )
		<< TOSTRINGWb( m_bActive )
		<< TOSTRINGWb( m_bTimedEvent )
		<< TOSTRINGW( m_dwTickGap )
		<< TOSTRINGW( m_dwBillboardItemID );

	return stm_;
}

bool KUserBillboardManager::LoadScript()
{
	return _LoadScript(std::string());
}

bool KUserBillboardManager::_LoadScript( OUT std::string& strScript )
{
	time_t tmBegin;
	time_t tmEnd;
	DWORD dwTickGap;
	GCITEMID dwBillboardItemID;
	bool bActive;
	bool bTimedEvent;

	KLuaManager kLuaMng;
	const std::string strScriptName = "InitBillboardInfo.lua";
	strScript = strScriptName;

	_JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "BillboardMessageTime", dwTickGap ) == S_OK, return false );
	_JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
	_JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
	_JIF( kLuaMng.GetValue( "ActiveEvent", bActive ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "UseTimedEvent", bTimedEvent ) == S_OK, return false );
	_JIF( kLuaMng.GetValue( "BillboardItemID", dwBillboardItemID ) == S_OK, return false );

	START_LOG(clog, "Billboard event info loaded with success") << END_LOG;
	{
		m_dwTickGap = dwTickGap;
		m_dwBillboardItemID = dwBillboardItemID;
		m_tmBegin = tmBegin;
		m_tmEnd = tmEnd;
		m_bActive = bActive;
		m_bTimedEvent = bTimedEvent;
		m_dwLastTick = ::GetTickCount();
	}

	return true;
}

bool KUserBillboardManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
	int nYear, nMonth, nDay, nHour, nMinute;
	_JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
	_JIF( kLuaMng_.GetValue( 1, nHour ) == S_OK, return false );
	_JIF( kLuaMng_.GetValue( 2, nMinute ) == S_OK, return false );
	_JIF( kLuaMng_.GetValue( 3, nMonth ) == S_OK, return false );
	_JIF( kLuaMng_.GetValue( 4, nDay ) == S_OK, return false );
	_JIF( kLuaMng_.GetValue( 5, nYear ) == S_OK, return false );
	
	_JIF( kLuaMng_.EndTable() == S_OK, return false );

	tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, nMinute, 0 ) );
	return true;
}

void KUserBillboardManager::Tick()
{
	if(m_bTimedEvent)
	{
		time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

		if( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent )
			m_bActive = true;
		else
			m_bActive = false;
	}

	if(!m_bActive)
	{
		return;
	}

	if(::GetTickCount() - m_dwLastTick < (m_dwTickGap * 1000))
	{
		return;
	}

	
	KLocker lock(m_csBillboardData);
	if(m_vecBillboardData.empty())
	{
		return;
	}

	SendBillboardToCenter();
	m_dwLastTick = ::GetTickCount();
}

void KUserBillboardManager::SendBillboardToCenter()
{
	if(m_vecBillboardData.empty()) 
	{
		return;
	}

	KBillboardChat data = m_vecBillboardData.at(0);
	m_vecBillboardData.pop_front();
	LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_BILLBOARD_DATA_BROAD_REQ, data) );
}

void KUserBillboardManager::InsertBillboardData(std::wstring& message, std::wstring& nickname, DWORD color)
{
	KBillboardChat newBillboardChat;
	newBillboardChat.Nickname = nickname;
	newBillboardChat.MessageColor = color;
	newBillboardChat.Message = message;

	m_vecBillboardData.push_back(newBillboardChat);
}