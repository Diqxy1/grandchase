#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../GCDeviceManager2.h"
//#include <string>

struct SRankingRewardItem
{
	DWORD dwGoodsID;
	int	iSetNumber;
	GCDeviceTexture*	ItemTexture;
	SRankingRewardItem()
	{
		dwGoodsID = 0;
		iSetNumber = -1;
		ItemTexture = NULL;
	};
};

class KGCRankingReward:	public KD3DWnd,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRankingReward );
	/// Default constructor
	KGCRankingReward( void );
	/// Default destructor
	virtual ~KGCRankingReward( void );
	/// Copy constructor
	KGCRankingReward( const KGCRankingReward& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRankingReward& operator=( const KGCRankingReward& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	virtual void PostChildDraw( void );
	//virtual void OnDestroyComplete( void );

	GCDeviceTexture* GetItemTexture( int iGoodsID );

	//------------------------------------------------------------------------
	//	[ 길드 광장 랭킹 UI 추가 작업 ] 20091117 : 이동석
public:
	void UpdateRankerReward();	//	이 함수가 왜 프로텍트의 접근을 갖고 있었는지 의문.
	void ToggleUseForGuild( bool bUseForGuild );
	//------------------------------------------------------------------------

    void OpenCharacterSelcectUI( void );
    void UpdateCharacterSelect( void );
    void SendChangeChange(int iChar);

protected:
	KD3DStatic*	m_pkDesc;

	KD3DStatic* m_pkWeeklyItemName;
	KD3DStatic* m_pkWeeklyItemDesc;

	KSafeArray<KD3DStatic*,2> m_pkMonthlyItemName;
	KSafeArray<KD3DStatic*,2> m_pkMonthlyItemDesc;

	KD3DWnd*	m_pkWeeklyItemBox;
	KSafeArray<KD3DWnd*,2> m_pkMonthlyItemBox;

	SRankingRewardItem	m_pkGuildTotalItem;
	SRankingRewardItem	m_pkGuildMonthlyItem;
	SRankingRewardItem	m_pkGuildWeeklyItem;

	SRankingRewardItem	m_WeeklyItem;
	KSafeArray<SRankingRewardItem,2> m_MonthlyItem;

	KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_apkChar;
    KD3DWnd* m_pkBtnSelectCharacter;
	KSafeArray<KD3DWnd*,3> m_apkRank;

	KD3DWnd*	m_pkClosebtn;


	//------------------------------------------------------------------------
	//	[ 길드 광장 랭킹 UI 추가 작업 ] 20091117 : 이동석
	enum
	{
		GUILD_WHOLE_RANK_FRAME,
		GUILD_WHOLE_ITEM_NAME,
		GUILD_WHOLE_ITEM_DESC,
		GUILD_WHOLE_ITEM_BOX,
		GUILD_WHOLE_RANKER_TITLE,
		GUILD_WHOLE_DESC_FRAME1,
		GUILD_WHOLE_DESC_FRAME2,
		GUILD_WHOLE_1,
		GUILD_WHOLE_2,
		GUILD_WHOLE_3,

		GUILD_MONTHLY_RANK_FRAME,
		GUILD_MONTHLY_ITEM_NAME,
		GUILD_MONTHLY_ITEM_DESC,
		GUILD_MONTHLY_ITEM_BOX,
		GUILD_MONTHLY_RANKER_TITLE,
		GUILD_MONTHLY_DESC_FRAME1,
		GUILD_MONTHLY_DESC_FRAME2,
		GUILD_MONTHLY_1,
		GUILD_MONTHLY_2,
		GUILD_MONTHLY_3,

		GUILD_WEEKLY_RANK_FRAME,
		GUILD_WEEKLY_ITEM_NAME,
		GUILD_WEEKLY_ITEM_DESC,
		GUILD_WEEKLY_ITEM_BOX,
		GUILD_WEEKLY_RANKER_TITLE,
		GUILD_WEEKLY_DESC_FRAME1,
		GUILD_WEEKLY_DESC_FRAME2,
		GUILD_WEEKLY_1,
		GUILD_WEEKLY_2,
		GUILD_WEEKLY_3,

		GUILD_RANKING_REWARD_FRAME,

		GUILD_RANKING_WND_NUM_END,
	};
	KSafeArray< KD3DWnd*, GUILD_RANKING_WND_NUM_END > m_apkGuildUIWnd;

	//#	reinterpret_cast 로 enum에 등록된 컨트롤과 맞춰야 한다.
	KD3DStatic*		m_pkGuildWholeItemName;
	KD3DStatic*		m_pkGuildWeeklyItemName;
	KD3DStatic*		m_pkGuildMonthlyItemName;

	KD3DStatic*		m_pkGuildWholeItemDesc;
	KD3DStatic*		m_pkGuildWeeklyItemDesc;
	KD3DStatic*		m_pkGuildMonthlyItemDesc;

	bool	m_bUseForGuild;
	//------------------------------------------------------------------------

	int m_iCharType;
	int m_iMonthlyRank;


	int m_iGuildTotalRank;
	int m_iGuildMonthlyRank;
	int m_iGuildWeeklyRank;
};

DEFINE_WND_FACTORY( KGCRankingReward );
DECLARE_WND_FACTORY( KGCRankingReward );
DECLARE_WND_FACTORY_NAME( KGCRankingReward );