#ifndef _GCSTATEGAME_H_
#define _GCSTATEGAME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"
//#include "define.h"
#include "KGCGameModeInterface.h"
//#include "GCEnum.h"

class KGCStateGame : public KGCState
{
public:
	KGCStateGame(void);
	virtual ~KGCStateGame(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);

    virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void Process_Admin_Command( AdminCommandType _ACT );
	virtual void OnSetupFSM();

    KGCGameModeInterface* GetCurGameModePtr();
    KGCGameModeInterface* GetGameModePtr( EGCGameModeCategory eGameMode );

	//���� �帧�� ���Ѱ� ���⿡ ������
	void ClearGameResource();
	void ClearStageResource( bool bLoad = false );
	void EndGame();
	void OnExitGame();
	void OnStartGame();
	void OnEnterStage();

	void OnExitReplay();
    void OnExitTutorial();
	void OnExitMinigame();

	//���� ����
	void MonsterRisen();

	void RemoveEmptyItem();		//�پ� ������ �������� �����ϰ� �ٲ��ٰ� ��ü���� �˸�

	void SendEndGamePacket();

	void CountStatPing();

    void CheckTCPRelayServer();

    void DungeonRewardExp( std::map<DWORD,KCharExpReward>& mapRewardExp );
    void DungeonRewardItem( DWORD dwRecvUserUID, std::vector<KItem>& vecDropItem, std::vector<std::pair<DWORD,std::vector<KItem>>>& vecPartyDropItem, std::map< DWORD, int >& mapUserDice );

public:
	std::vector<KStatPingInfo> m_vecStatPingInfo;

private:
	KSafeArray<KGCGameModeInterface*,GC_GMC_COUNT> m_ppGameMode;
	EGCGameModeCategory m_eCurrGameModeCategory;
    unsigned int m_uiBadPingCnt;
public:
    bool m_bOnExit;
};

extern KGCStateGame* g_pStateGame;

#endif //_GCSTATEGAME_H_
