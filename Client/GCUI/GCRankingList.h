#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DEdit.h"
//#include "../uifx/D3DStaticLink.h"
#include "gcui/GCRankingElement.h"
#include "CommonPacket.h"
#include "GCRanking.h"

//#include <map>

class KGCRankingList:	public KD3DWnd,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRankingList );
	/// Default constructor
	KGCRankingList( void );
	/// Default destructor
	virtual ~KGCRankingList( void );
	/// Copy constructor
	KGCRankingList( const KGCRankingList& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRankingList& operator=( const KGCRankingList& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		GC_RL_RANKING_NUM = 10,
		GC_RL_LINE_NUM = 6,
		GC_RL_TAB_NUM = 8,
	};

	enum RankingListTitle {
		GC_RLT_ORDER	=	0,
		GC_RLT_GUILD	=	1,
		GC_RLT_CHAR		=	2,
		GC_RLT_NICKNAME	=	3,
		GC_RLT_WIN		=	4,
		GC_RLT_LOSE		=	5,
		GC_RLT_WINRATE	=	6,
		GC_RLT_EXP		=	7,
		GC_RLT_BPOINT	=	8,
		GC_RLT_NUM,
	};

    enum 
    {
        EM_AUTOMATCH_CURRENT_SEASON = 0,
        EM_AUTOMATCH_OLD_SEASON = 1,
        EM_AUTOMATCH_SEASON_NUM = 2,
    };

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	virtual void ActionPerformed( const KActionEvent& event);

	void UpdateExpRanking();
	void UpdateWinreateRanking();
    void UpdateAutoMatchWinreateRanking( bool bWinCount, bool bOldSeason );


    void ChangeAutoMatchRanking();


	void OnFindBtn( DWORD code );

public:
	void ExpRankingMode();			//	일반 경험치 탭
	void WinrateRankingMode();		//	일반 승, 패, 승률 탭
	void TotalRankingMode();		//	전체 랭킹 탭

	void SetTabInfo( int tab );
	void UpdateRanking();
	void SetMyRankingInfo();
	void SetFindErrorMsg( int iStringID );

	template< typename T >
	void SetFindedData( const T& data )
	{
		m_pkTempRanking->SetRankingInfo( &data );
		m_pkTempRanking->ToggleRender( true );
		m_pkMessage->ToggleRender( false );
	}

	//------------------------------------------------------------------------
	//#	길드 함수들
	void UpdateGuildWinreateRanking( KNGuildRankInfo::RANK_TYPE eType );
	void UpdateGuildTotalRanking();
	void ToggleUseForGuild( int emRankingMode );

    void OpenCharacterSelcectUI( void );
    void UpdateCharacterSelect( void );
    void SendChangeChange(int iChar);

protected:
	KSafeArray<KGCRankingElement*,GC_RL_RANKING_NUM> m_apkRanking;
	KGCRankingElement*	m_pkTempRanking;

	KSafeArray<KD3DWnd*,GC_RLT_NUM> m_apkListTitle;
	KSafeArray<KD3DWnd*,GC_RL_LINE_NUM> m_apkLine;

    KD3DWnd*    m_pkBtnSelectCharacter;
    KD3DWnd*    m_pkBtnAllCharacter;

	KSafeArray<KD3DWnd*,GC_RL_TAB_NUM> m_apkDescTitle;
	KD3DWnd*	m_pkFindbtn;
	KD3DEdit*	m_pkFindtxt;
	KD3DWnd*	m_pkClosebtn;
	KD3DStatic*	m_pkDesc;

	KD3DWnd*	m_pkFindTabActive;
	KD3DWnd*	m_pkFindTab;
	KD3DWnd*	m_pkMyRankTabActive;
	KD3DWnd*	m_pkMyRankTab;

	KD3DStaticLink*	m_pkStaticLink;

	KD3DStatic*	m_pkMessage;

	std::map< int, DWORD >	m_mapTabDesc;
	std::map< int, DWORD >	m_mapTabGuildDesc;

    KD3DComboBox* m_pkAutoMatchCombo;
	
	//#	길드 토글 해야 하는 UI들
	KD3DWnd*	m_pkGuildDescFrame;
	KD3DWnd*	m_pkGuildName;
	KD3DWnd*	m_pkG_Point;
	KD3DWnd*	m_guild_rank_tab;
	KD3DWnd*	m_guild_rank_tab_active;
	KD3DWnd*	m_line2_2;					//	길드 랭킹 표시할때 렌더

    bool EnableAutoMatchRanking() { return GC_GLOBAL_DEFINE::bAutoMatchRanking ? true : false; }
    bool IsGuildMode() { return ( m_emRankingMode == EM_RANKING_MODE_GUILD || m_emRankingMode == EM_RANKING_MODE_GUILD_BATTLE ); }
    bool IsGuildBattleMode() { return ( m_emRankingMode == EM_RANKING_MODE_GUILD_BATTLE ); }
    bool IsAutoMatchMode() { return ( m_emRankingMode == EM_RANKING_MODE_AUTO_MATCH ); }
    char GetMatchRankType( bool bWinCount, bool bOldSeason );

	//#	내부 정보
    int         m_emRankingMode;

	int	m_iRankingType;
	int m_iCharType;
	int m_iPage;
};

DEFINE_WND_FACTORY( KGCRankingList );
DECLARE_WND_FACTORY( KGCRankingList );
DECLARE_WND_FACTORY_NAME( KGCRankingList );
