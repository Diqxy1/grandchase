#pragma once


#include "GCRankingList.h"
#include "KGCGuildCreateDlg.h"
#include "CommonPacket.h"
#include "GCRanking.h"


class KGCRankingReward;
class KGCAutoMatchRankingReward;

class KGCRankingTab
{
public:
	KGCRankingTab();
	void Active( bool bActive_ );
	void ToggleRender( const bool bRender_ );
	void Update();

public:
	KD3DWnd*	m_pkActiveTab;
	KD3DWnd*	m_pkTab;

private:
	bool	m_bActive;
	bool	m_bRender;
};


class KGCRankingDlg:	public KD3DDialog,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRankingDlg );
	/// Default constructor
	KGCRankingDlg( void );
	/// Default destructor
	virtual ~KGCRankingDlg( void );
	/// Copy constructor
	KGCRankingDlg( const KGCRankingDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRankingDlg& operator=( const KGCRankingDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum GCRankingTab
	{
		RT_Daily	=	0,		RT_Weekly	=	1,
		RT_Monthly	=	2,		RT_Exp		=	3,
		RT_Reward	=	4,		RT_Total	=	5,
        RT_AutoMatch = 6,       RT_AutoMatch_old = 7,
		
		RT_NUM,
	};

	enum GCRankingModeTab
	{
		RMT_CHARACTER		=	0,
		RMT_GUILD			=	1,
		RMT_GUILDBATTLE		=	2,

		RMT_NUM,
	};

protected:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event);

public:
	void ActiveTab( GCRankingTab tab );
	void Update();
	void FindErrorMsg( int iStringID );

	template< typename T >
	void SetFindedData( const T& data )
	{
		m_pkRankingList->SetFindedData( data );
	}

    bool EnableAutoMatchRanking() { return GC_GLOBAL_DEFINE::bAutoMatchRanking ? true : false; }
    bool IsGuildMode() { return ( m_emRankingMode == EM_RANKING_MODE_GUILD || m_emRankingMode == EM_RANKING_MODE_GUILD_BATTLE ); }
    bool IsGuildBattleMode() { return ( m_emRankingMode == EM_RANKING_MODE_GUILD_BATTLE ); }
    bool IsAutoMatchMode() { return ( m_emRankingMode == EM_RANKING_MODE_AUTO_MATCH ); }
    
    
    bool IsUserInGuild();
	bool IsHaveGuild();
	void RankingTabToogleRender();	//길드용으로 UI전환하자.
	void RankingModeTabToggleRender();

	void SetGuildRankingInfo(
		KNGuildRankInfo::RANK_TYPE		eType_,					//	정보 타입	
		std::vector< KNGuildRankInfo >&	vecRefKNGuildRankInfo_ );//	랭킹 벡터
	
	void GuildRankReq( KNGuildRankInfo::RANK_TYPE eType );

	void GuildRankReqAll();

private:
	void DlgModeTabActive( GCRankingModeTab eRankingMode_ );


protected:
	KGCRankingList*		        m_pkRankingList;
	KGCRankingReward*	        m_pkRankingReward;
    KGCAutoMatchRankingReward*  m_pkAutoMatchRankingReward;
	KD3DWnd*			m_pkClose;

	KSafeArray<KGCRankingTab, RT_NUM >	m_akTab;
	KSafeArray<KGCRankingTab, RMT_NUM>	m_akRankingModeTab;

	//	길드 모드일때만 렌더되야 하는 UI들
	KD3DWnd*	m_pkGraph;					//	그래프( 단순 장식 문자 )

	KD3DWnd*	m_pkGuildTitleDaily;		//	길드 일일 랭킹
	KD3DWnd*	m_pkGuildTitleWeekly;		//	길드 주간 랭킹
	KD3DWnd*	m_pkGuildTitleMonthly;		//	길드 월간 랭킹
	KD3DWnd*	m_pkGuildTitleGuildBattle;	//	길드전 랭킹

	//#	내부 상태 저장

    int                 m_emRankingMode;
	GCRankingTab		m_eActiveRankingTab;
	GCRankingModeTab	m_eActiveRankingModeTab;

public:	//	길드로부터 랭킹 데이타를 받을 맵 벡터
	std::map< KNGuildRankInfo::RANK_TYPE, std::vector< KNGuildRankInfo > >	m_mapGuildRankInfo;
};


DEFINE_WND_FACTORY( KGCRankingDlg );
DECLARE_WND_FACTORY( KGCRankingDlg );
DECLARE_WND_FACTORY_NAME( KGCRankingDlg );
