#pragma once
#include "KNCSingleton.h"

class KGCMatchManager
{
    DeclareSingleton( KGCMatchManager );
public:
	enum {
		MSTATE_NONE = -1, //현재 아무 상태도 아님 - 룸리스트에 있을때
		MSTATE_READY_STATE = 0, //매칭 준비 상태
		MSTATE_WAIT_STATE = 1, //매칭 대기 상태
		MSTATE_FIND_USER_STATE = 2, //매칭 상대 찾은 상태
		MSTATE_COMPLETE_STATE = 3, //매칭 완료 상태
		MSTATE_ROOM_STATE = 4, //대전 매칭 방입장 상태.
	};

	enum {
		MUSERSTATE_NONE = 0, // 파티가 아닌 상태
		MUSERSTATE_PARTY_USER = 1, //파티원인 상태
		MUSERSTATE_PARTY_MASTER = 2, //파티장인 상태
	};

	enum {
		MMATCHMODE_1VS1 = 0,
		MMATCHMODE_2VS2 = 1,
		MMATCHMODE_3VS3 = 2,
		MMATCHMODE_MAX,	
	};

	enum {
		MMATCHMODE_AUTO_MATCH = 0,
		MMATCHMODE_CUSTOM_MODE = 1,
	};

public:
    KGCMatchManager( void );
    ~KGCMatchManager( void );
	
	void SetCurrentState(int iState) { m_iCurrentState = iState; }
	int GetCurrentState( void ) { return m_iCurrentState; }

	void SetCurrentMode(int iMode) { m_iCurrentMode = iMode; }
	int GetCurrentMode( void );

	int GetCurrentUserState( void );
	bool IsModePlayable(int iMode);
	time_t GetWaitMachingTime( void );
	void SetWaitMatchingTime( int iMatchUser, int iMinMatchUser, bool bSystemOn );
	void InitMatchMapName( void );
	int GetMatchMapNameID( int iMapID );

	void SetCurrentExceptMapID( int iMapID );
	int GetCurrentExceptMapID( void );

	void InitMatchInfo();

	void SetMatchCompleteUser(std::map<int, std::vector<KPartyMember> > &mapUser) {m_mapCompleteUser = mapUser;}
	std::map<int, std::vector<KPartyMember> >  GetMatchCompleteUser() {return m_mapCompleteUser;}

	void SetRoomUser();
	int GetRoomUserCnt(){ return static_cast<int>( m_vecRoomUser.size() );}
	int GetStartAutoGameMapID();
	void SetWaitMachingTime( void );

	void SetStartOption( bool bOK ){m_bStartOption = bOK;}
	bool GetStartOption( void ){return m_bStartOption;}

	void SetMatchModeState( KEVENT_MATCH_MODE_STATE_ACK mapData ){m_mapMatchModeState = mapData;}
	bool GetMatchModeState( int iMode, int &iType );

	void SetMatchOpenTime( KEVENT_MATCH_TIME_TABLE_NOT kData ){m_kMatchOpenTime = kData;}
	bool IsAutoMatchOpenTime();
	bool GetCurrentOpenTime( KMatchOpenTime &kOpenTime );
private:
	int m_iCurrentState;
	int m_iCurrentMode;

	int m_iCurrentExceptMapID;

	time_t m_tmWaitTime;
	time_t m_tmSendWaitTime;
	std::map<int, int> m_mapMatchMapName;
	std::map<int, std::vector<KPartyMember> > m_mapCompleteUser;
	std::vector<KPartyMember> m_vecRoomUser;

	bool m_bStartOption;

	KEVENT_MATCH_MODE_STATE_ACK m_mapMatchModeState;
	KEVENT_MATCH_TIME_TABLE_NOT m_kMatchOpenTime;
};

DefSingletonInline( KGCMatchManager );