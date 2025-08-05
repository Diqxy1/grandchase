#pragma once
////#include <map>
//#include <Windows.h>

struct SConditionInfo
{
    DWORD   m_dwID;         //이 조건의 아이디;
    int     m_iMonsterID;
    int     m_iGameMode;
    int     m_iStage;
    std::pair< int, int >   m_pairDifficultRange;
    int     m_iMapID;
    std::pair< int, int >   m_pairScoreRange;
    bool    m_bWin;
    int     m_iTime;
    int     m_iKill;
    int     m_iDie;
    int     m_iExLife;
	bool	m_bSolo;
    float   m_fConditionRatio;  //이 조건이 맞을 확률;
    SConditionInfo()
    {
        m_dwID                      = -1;
        m_iMonsterID                = -1;
        m_iGameMode                 = -1;
        m_iStage                    = -1;
        m_pairDifficultRange        = std::make_pair( -1, -1 );
        m_iMapID                    = -1;
        m_pairScoreRange               = std::make_pair( -1, -1 );
        m_bWin                      = true;
        m_iTime                     = -1;
        m_iKill                     = -1;
        m_iDie                      = -1;
        m_iExLife                   = -1;
		m_bSolo						= false;
        m_fConditionRatio           = 1.0f;    
    }

    bool operator==( const SConditionInfo& arg )
    {
        return (m_iMonsterID         == arg.m_iMonsterID        )&&
        (m_iGameMode                 == arg.m_iGameMode         )&&
        (m_iStage                    == arg.m_iStage            )&&
        (m_pairDifficultRange        == arg.m_pairDifficultRange)&&
        (m_iMapID                    == arg.m_iMapID            )&&
        (m_pairScoreRange            == arg.m_pairScoreRange    )&&
        (m_bWin                      == arg.m_bWin              )&&
        (m_iTime                     == arg.m_iTime             )&&
        (m_iKill                     == arg.m_iKill             )&&
        (m_iDie                      == arg.m_iDie              )&&
        (m_iExLife                   == arg.m_iExLife           )&&
		(m_bSolo					 == arg.m_bSolo				)&&
        (m_fConditionRatio           == arg.m_fConditionRatio   );        
    }
};
class KGCCondition
{
private:
    SConditionInfo m_sCondition;
public:
    KGCCondition(void);
    ~KGCCondition(void);

    void SetConditionInfo( SConditionInfo sConditionInfo );
    const SConditionInfo& GetConditionInfo() { return m_sCondition; }
    void SetConditionID( DWORD dwID )   { m_sCondition.m_dwID = dwID; }
    void MonsterKill( int iMonsterID ); //몬스터를 죽여야 하는 조건을 건다.
    void GameModeClear( int iGameMode, bool bWin = true, std::pair<int, int> pairDifficultRange = std::make_pair( -1, -1 ) );
    void ArriveSpecialStage( int iGameMode, int iStage, std::pair<int, int> pairDifficultRange = std::make_pair( -1, -1 ) );
    void SetTimeLimit( int iTime );
    void SetKillLimit( int iKill );
    void SetDieLimit( int iDie );
    void SetExLifeLimit( int iExLife );
    void SetConditionRatio( float fRatio );

#define GC_CONDITION_SATISFIED( value, initValue, argValue ) \
	if( value != initValue && value != argValue )\
		return false;
	bool IsSatisfied( const SConditionInfo& arg )
	{
		GC_CONDITION_SATISFIED( m_sCondition.m_iMonsterID,	-1, arg.m_iMonsterID );
		GC_CONDITION_SATISFIED( m_sCondition.m_iGameMode,	-1, arg.m_iGameMode );
		GC_CONDITION_SATISFIED( m_sCondition.m_iStage,		-1, arg.m_iStage );
		GC_CONDITION_SATISFIED( m_sCondition.m_iMapID,		-1, arg.m_iMapID );
		GC_CONDITION_SATISFIED( m_sCondition.m_iTime,		-1, arg.m_iTime );
		GC_CONDITION_SATISFIED( m_sCondition.m_iKill,		-1, arg.m_iKill );
		GC_CONDITION_SATISFIED( m_sCondition.m_iDie,		-1, arg.m_iDie );
		GC_CONDITION_SATISFIED( m_sCondition.m_iExLife,		-1, arg.m_iExLife );
		GC_CONDITION_SATISFIED( m_sCondition.m_pairDifficultRange,	std::make_pair( -1, -1 ), arg.m_pairDifficultRange );
		GC_CONDITION_SATISFIED( m_sCondition.m_pairScoreRange,		std::make_pair( -1, -1 ), arg.m_pairScoreRange );
		if( m_sCondition.m_bWin != arg.m_bWin )
			return false;
		if( m_sCondition.m_bSolo != arg.m_bSolo )
			return false;
		return true;

	}

    bool operator== ( const SConditionInfo& arg )
    {
        return m_sCondition==arg;
    }
};
