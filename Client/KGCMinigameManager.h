#pragma once

//#include "GCEnum.h"
//#include <list>
#include <KNCSingleton.h>
#include "GCCollisionRect.h"
#include "Procedure.h"
#include "ProtectedType.h"
#include "GCStateMachine.h"
#include "GCOlympicManager.h"


class KGCContectInfo;
class KGC2DObject;

class KGCMinigameManager
{
	DeclareSingleton(KGCMinigameManager);
public:
	KGCMinigameManager(void);
	~KGCMinigameManager(void);

private:
	bool LoadMinigameAttribute( EGCGameMode eGameMode );

public:

	bool RestartMinigame();
	int GetCurrentScore();
	int GetCurrentLevel();
	/************************************************************************/
	/* 미니게임을 시작하기 전 초기화를 위한 함수들
	/************************************************************************/
	bool BindLua();
	bool SetMiniGameModeID( EGCGameMode eGameMode );
    EGCGameMode GetMinigameModeID( void ) const { return m_eMiniGameMode; }
	bool LoadStage();
	void InitValue();
	bool InitCamara();
	void InitPlayer();
	void InitRoomInfo();
	void InitLua();
	void SetPlayerPosition();

	bool StartMiniGame();
	bool EndMiniGame();

	bool IsEndReadyTime()	{ return m_bTutorialEnd; };
    bool GetExitGame( void ) const { return m_bExitGame; }
    void SetExitGame( bool bExitGame )  { m_bExitGame = bExitGame; }

    int GetMinigameChar( void ) { return m_iMiniGameChar; }

	/************************************************************************/
	/* 미니게임을 진행하기 위한 함수들
	/************************************************************************/
	void FrameMove();
	void Render();
	void ShowText( const char* str );

	std::pair< bool, GCCollisionRect<float> > HitObject( GCCollisionRect<float>* rtWeapon, bool bOneHit, int iDamageType = -1 );
	void PushCheck( bool bTeleport = true );
	bool CheckContact( KGCContectInfo& kInfo );

    // 올림픽 관련 추가 함수 2012-07-17
    void GoPrevState();
    void SetPrevState();
    void SendResult();
    void StartGame( EGCGameMode eGameMode );
    void QuitCurrentGame();
    void SetAccumulatedScore( IN const int nScore );
    void SetUserInfo( SUserInfo& kUserInfo ){ m_kUserInfo = kUserInfo; }
    SUserInfo& GetUserInfo(){ return m_kUserInfo; }
    SUserInfo m_kUserInfo;

    /************************************************************************/
    /* 쫌 그런 함수들..랭킹 관리
    /************************************************************************/
    void SetRankingInfo( int iMode, std::vector<KMiniGameRankInfo>& vecRankinfo );
    void GetRankingInfo( IN int iMode, OUT std::vector<KMiniGameRankInfo>& vecRankInfo );
    void SetMyRankingInfo( std::vector<KMiniGameRankInfo>& vecRankinfo ) { m_vecMyRankInfo = vecRankinfo; }
    std::vector<KMiniGameRankInfo>& GetMyRankingInfo( void ) { return m_vecMyRankInfo; }
    int GetMyHighScore( int iMode );
    int GetMyRanking( int iMode );
    void SetMyHighScore( int iMode, DWORD dwScore );
    void SetMyRanking( int iMode, DWORD dwRank );
    std::wstring GetRankerName( int iMode, int iRank );
    bool IsInRanker( std::wstring strNick );

private:
	/************************************************************************/
	/* 루아에 사용되는 함수들
	/************************************************************************/
	void			SetScore( int iScore );
	void			AddScore() { SetScore( ++m_iScore ); };
	KGC2DObject*	CreateObject();
	void			RemoveObject( KGC2DObject* obj );
	void			ClearObject();
	void			ParticleEffect( const char* strEffectName, float fx, float fy );
	float			GetMapWidth();
	float			GetRandomValue( float begin, float end );

private:
	bool		m_bTutorialEnd;
	bool		m_bPushCheck;
	bool		m_bContact;
	EGCGameMode	m_eMiniGameMode;
	int			m_iMiniGameChar;
	KProtectedType<int> m_iScore;
	int			m_iLevel;
	float		m_fCharPosX;
	float		m_fCharPosY;
	std::string	m_strScript;
	std::string	m_strMap;
    bool        m_bExitGame;
    std::vector<EGCSkillTree> m_vecTempSkill; 
	std::list< KGC2DObject* > m_listObjects;
    GAME_STATE         m_ePrevState;
    int         m_iAccumulatedScore;

    // 랭크 정보를 들고 있을까? -_-;;
    std::map< int, std::vector<KMiniGameRankInfo> > m_mapRankInfo;
    std::vector<KMiniGameRankInfo>                  m_vecMyRankInfo;
};

DefSingletonInline(KGCMinigameManager);
