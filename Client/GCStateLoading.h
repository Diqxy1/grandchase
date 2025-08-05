#ifndef _GCSTATELOADING_H_
#define _GCSTATELOADING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <thread/Locker.h>
#include "KGCState.h"
//#include <map>

#define LOADCOUNT 17

class GCDeviceTexture;

class KGCStateLoading : public KGCState
{
public:
	KGCStateLoading(void);
	virtual ~KGCStateLoading(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();    
	virtual void OnDestroy();
    virtual bool InitAtLoading();
	virtual void OnSetupFSM();

    void SetLoadComplete( DWORD dwUID_ );
    void RemoveFromLoadList( DWORD dwUID_ );    
    bool IsLoadComplete();
    bool IsLoadComplete( DWORD dwUID_ );
    void SetConnectComplete( DWORD dwUID );
	bool IsConnectComplete(void);

    void AddP2PConnectList( DWORD dwUID );
    void RemoveP2PConnectList( DWORD dwUID );    

	std::set<DWORD>	GetWaittingUserList();
    void SendLoadingComplete( int iP2PCount, int iRelayCount );

    void ClearLoadList();
    void InsertLoadList( DWORD dwUID );

    void SendTCPP2PDifference();
	bool IsStartingMember( DWORD dwUID );
	std::vector<DWORD> GetGameStartingMembers();
    int GetLoadStartingMembers();
	void ClearUserConnectState()
	{
		m_mapUserConnectState.clear();
	}

    void SetLoadCompleteInServer(bool bComplete_ ) { m_bLoadingComplete = bComplete_; }

	bool m_bConnectComplete;


protected:
	void (KGCStateLoading::*LoadFunc[LOADCOUNT])();

	enum
	{
		NUM_MAX_PLAYER = 6,
	};

private:
	void AdjustPlayerPosition();
    void SetPlayerPosition();

	void CheckValentineEvent();
	void ReloadScript();
	void PetRender();

	//로딩할 목록
	void InitData();
	void LoadEvent();
	void StageLoading();
	void InitGameMode();
	void LoadItemEffect();

    void PrevInitPlayer();
    void InitPlayer();
    void AfterInitPlayer();

    void SparkLoading();
    void DamageLoading();
    void SoundLoading();

	void SendStatisticsInfo();

    bool CheckLoadingTime();

    
private:
	GCDeviceTexture*	            m_pTexLoad;

    bool                            m_bSendP2PConnectionReport;
    int                             m_iLoadCount;
    int                             m_iPlayerInitCount;
    int                             m_iDamageLoadCount;
    int                             m_iSoundLoadCount;
    int                             m_iStartingMember;

    bool                            m_bSendLoadingTimeReport;
    DWORD                           m_dwLoadingTime;

	std::map< DWORD, bool >			m_mapUserConnectState;
	std::map< DWORD, bool >			m_mapLoadComplete;
    KncCriticalSection m_csCompleteCon;
    KncCriticalSection m_csWaitToLoad;
    int m_iSendLoadingCompleteCnt;

    KncStopWatch m_kStopWatch;

    KncStopWatch m_kLoadingTimeStopWatch;

    bool m_bOverLoadingTime;

    bool m_bLoadingComplete;

    FILE *m_pLoadingTimeFile;
    DWORD m_dwTotalTime;
    float m_fWaitTime;
};

#endif //_GCSTATELOADING_H_
