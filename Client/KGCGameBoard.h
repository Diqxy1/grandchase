#ifndef _KGCGAMEBOARD_H_
#define _KGCGAMEBOARD_H_

//#include "GCEnum.h"
//#include <vector>
//#include <map>
#include "ProtectedType.h"

struct Player_Work
{
	
	bool                        IsLive;	
	bool                        IsLevelUp;
	int							IsSPLevelUp;
	float                       fGuildPointPercent;
    std::map< char, GCEXPTYPE > m_mapStartExp;
	DWORD                       Start_Score;
	KProtectedType<int>         How_Kill;
	int 	                    How_Die;
	bool                        IsWin;
	int		                    m_iChar;
    DWORD                       dwUID;
    float                       fTotalDamage;
    float                       fComboDamage;
    float                       fMaxComboDamage;

	int GetKillDieScore()
	{
		return ( How_Kill * 1 ) + ( How_Die * (-10) );
	}

    void clear()
    {

        IsLive = false;
        IsLevelUp = false;
		IsSPLevelUp = 0;
        fGuildPointPercent = 0;
        m_mapStartExp.clear();
        Start_Score = 0;
        How_Kill = 0;
        How_Die = 0;
        IsWin = false;
        m_iChar = 0;
        dwUID = 0;
        fTotalDamage = 0.f;
    }
};

extern KProtectedType<int>     g_aiArrowScore[MAX_PLAYER_NUM][3];

class KGCGameBoard
{
public:
	KGCGameBoard(void);
	~KGCGameBoard(void);

public:
	void Init();

	// 몬스터 개수 관련
	void AddMonsterNum( int iMon, int iMonNum );
    void AddMonKillNum( int iMon, int iMonNum, int iTGID );

	std::map<int, unsigned short> GetMonsterNum(){ return m_mapMonNum; }
	std::map<int, unsigned short> GetMonKillNum(){ return m_mapMonKillNum; }
    std::vector<int> GetMonKillTGID() { return m_vtMonKillTGID; }

	void SetMonsterNum( std::map<int, unsigned short> mapMonNum ){ m_mapMonNum = mapMonNum; }
	void SetMonKillNum( std::map<int, unsigned short> mapMonKillNum ){ m_mapMonKillNum = mapMonKillNum; }
    void SetMonKillTGID( std::vector<int> vtMonKillTGID ){ m_vtMonKillTGID = vtMonKillTGID; }

	unsigned short GetMonsterNum( int iMon );
	unsigned short GetMonKillNum( int iMon );

	int GetGameScore(){ return m_iGameScore; }
    int* GetGameScorePtr(){ return &m_iGameScore; }
	void SetGameScore( int iPoint );
    int GetScoreMax(){  return m_iScoreMax; }
    int GetScoreSuccess(){  return m_iScoreSuccess; }
    void SetScoreMax( int iScoreMax ) { m_iScoreMax = iScoreMax; }
    void SetScoreSuccess( int iScoreSuccess ) { m_iScoreSuccess = iScoreSuccess; }
    void SetTotalTime( float fTime ){  m_fTotalTime = fTime; }
    void SetGameTime( float fTime ){   m_fGameTime = fTime; }
    
    DWORD GetTotalTime(){ return (DWORD)m_fTotalTime; }
    DWORD GetGameTime(){ return (DWORD)m_fGameTime; }
	DWORD GetPlayTime();

	int GetArrowTotalScore(int iIndex)
	{
		int iScore = 0;
		for(int i = 0; i < 3; i++)
			iScore += g_aiArrowScore[iIndex][i];
		return iScore;
	}
	// 게임 시작전 처리
	void OnStartGame();

    void OnEnterPlayer(int iPlayerIndex);
    void OnLeavePlayer(int iPlayerIndex);

	// StartingMember 정보 접근	
	int GetHowKill( int iPlayerIndex );
	int GetHowDie( int iPlayerIndex );
	bool IsLevelUp( int iPlayerIndex );
	int IsSPLevelUp( int iPlayerIndex );

	int	GetRank( int iPlayerIndex );
	bool IsWin( int iPlayerIndex );
	bool IsWin();
	GCEXPTYPE GetChangedExp( int iPlayerIndex );
	GCEXPTYPE GetChangedSPExp( int iPlayerIndex );
	int	GetChangeGp( int iPlayerIndex );

	void InitPlayTime(){m_dwPlayTime=0;}
	void AdvancePlayTime(){++m_dwPlayTime;}
	DWORD GetPlayTimeInFrame(){return (m_dwPlayTime);}
	int	GetStartingMemberCount();

private:
	// 몬스터 정보
    std::vector< int >              m_vtMonKillTGID;
	std::map< int, unsigned short > m_mapMonKillNum;
	std::map< int, unsigned short > m_mapMonNum;

	// 몬스터 대전 현재 포인트
	int m_iGameScore;
	DWORD m_dwStartTime;
    int m_iScoreSuccess;
    int m_iScoreMax;
    float m_fTotalTime;
    float m_fGameTime;

	DWORD m_dwPlayTime;

    KSafeArray<KProtectedType<int>,TEAM_NUM> m_aiTeamScore;

    int m_iAttackCount;

public:	
	// 플레이어 시작정보
	KSafeArray<Player_Work,MAX_PLAYER_NUM> m_pStartingMember;

	// 플레이어 업데이트 정보
	std::map< int, std::map< char, GCEXPTYPE > >  Change_How_Exp;
	std::map< int, std::map< char, GCEXPTYPE > >  Change_How_SPExp;
	KSafeArray<int,MAX_PLAYER_NUM> Change_How_Score;

	std::map< int, std::map< GCITEMUID, int > > Change_How_Pet_Exp;
	KSafeArray<bool,MAX_PLAYER_NUM> m_bIsPetLevelUp;

    //길드 대전 길드 포인트 정보 (팀간)
    std::vector <int> m_vecPlayerDiffGuildPoint;
    //던전 포인트 정보
    DWORD m_dwContributionPoint;
	bool m_bComboBreak;
	
	void ClearMonKillNum()
	{
		m_mapMonKillNum.clear();
	}

    void ClearMonKillTGID()
    {
        m_vtMonKillTGID.clear();
    }

    int GetTeamScore( unsigned int iTeam_ );
    void SetTeamScore( unsigned int iTeam_ ,int iScore_ );
 
    void AddAttackCount();

public:
    void BindLua();
    float GetTotalDamage( int iPlayerIndex );
    float GetComboDamage( int iPlayerIndex );
    void SetComboBreak(bool bBreak);
    bool GetComboBreak() { return m_bComboBreak;}
};

extern KGCGameBoard* g_pGameBoard;

#endif //_KGCGAMEBOARD_H_
