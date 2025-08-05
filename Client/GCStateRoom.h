#ifndef _GCSTATEROOM_H_
#define _GCSTATEROOM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateRoom : public KGCState
{
public:
	KGCStateRoom(void);
	virtual ~KGCStateRoom(void);

public:
    void SetRoomInfo( int iRoomNum_ , const std::wstring& strRoomName, const std::wstring& strRoomPassword );
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void Process_Admin_Command( AdminCommandType _ACT );
	virtual void OnSetupFSM();
	void OnLeaveState();

	bool IsEnableCurrentGameMode();
    void UpdateRoomOption();

protected:
	int m_iCurrRoomNum;
	int m_iRoomNum;
	std::wstring m_strRoomName;
    std::wstring m_strRoomPassword;
    bool m_bPrevUdpRelayConnect;
	time_t m_tmStartGameTimer;
	time_t m_tmExitRoomTimer;
public:
	void			SetRoomName( const std::wstring& strName_ ) { m_strRoomName = strName_; }
    void			SetRoomPassword( const std::wstring& strPass ) { m_strRoomPassword = strPass; }
	std::wstring	GetRoomName() {return m_strRoomName; }
    std::wstring	GetRoomPassword() {return m_strRoomPassword; }
	void OnGameStart();
	void OnExitRoom();
};

extern KGCStateRoom* g_pStateRoom;

#endif // _GCSTATEROOM_H_
