#pragma once
//#include <string>
//#include <vector>
//#include <set>

enum
{
	EM_RANKING_MODE_CHAR = 0,
	EM_RANKING_MODE_GUILD = 1,
	EM_RANKING_MODE_GUILD_BATTLE = 2,
	EM_RANKING_MODE_AUTO_MATCH = 3,
};

class KGCRanking
{
public:
	KGCRanking(void);
	~KGCRanking(void);

	enum ERankType
	{
		RT_DAILY,
		RT_WEEKLY,
		RT_MONTHLY,
		RT_EXP,
	};

	const std::vector< KWinRateRank >& GetWinrateRanking(int iRankingType, int iChartype, int iPage);
	const std::vector< KExpRank >& GetExpRanking(int iChartype, int iPage);

	void SetRankingPage(int iRankingType, int iChartype, int iPage, const std::vector< KWinRateRank >& vecRanking);
	void SetRankingPage(int iRankingType, int iChartype, int iPage, const std::vector< KExpRank >& vecRanking);

	void SetMyRanking(int iRankingType, const std::map< char, KWinRateRank >& mapMyRanking);
	void SetMyRanking(const std::map< char, KExpRank >& mapMyRanking);

	const KWinRateRank*	GetMyWinrateRanking(int iRankingType, int iChartype = -1);
	const KExpRank*	GetMyExpRanking(int iChartype = -1);

	void FindRankingInfo(int iRankingType, int iChartype, const std::wstring& strFind);
	void FindMatchRankingInfo(char cRankType, char cCharType, std::wstring strFind);
	int	GetBestExpRankerLevel(int iChartype = -1);

	int GetRankingFromNick(int iRankingtype, int iChartype, DWORD dwUserUID);
	int AddUserRankingInfo(DWORD dwUserUID, const std::map< char, KWinRateRank >& m_mapRankInfo);
	int AddUserRankingInfo(DWORD dwUserUID, const std::map< char, int >& m_mapRankInfo);
	int ClearOtherUserRank();

	void LoadRankgerReward();
	std::vector<DWORD> GetRankerReward(int iRankingtype, int iCharType, int iRanking);

	void SetAutoMatchRanking(char cRankType, char cCharType, char cPageNum, const std::vector< KMatchRank >& vecRanking);
	const std::vector< KMatchRank >& GetAutoMatchRanking(char cRankType, char cCharType, char cPageNum);

	void RefreshAutoMatchRanking();

	const KMatchRank* GetMyAutoMatchRanking(char cRankType, char cCharTypee = -1);
	void SetMyAutoMatchRanking(const std::map<char, KMatchRank>& vecRPRanking, const std::map<char, KMatchRank>& vecWinRanking);

	void SetAutoMatchReward(std::map<char, std::vector<KRankRewardInfo>> mapRPRewardInfo, std::map<char, std::vector<KRankRewardInfo>> mapWinRecordRewardInfo);
	std::vector<GCITEMID> GetAutoMatchReward(char cRankType, char cCharType, int iRanking);

private:
	std::map< ERankType, std::map< int, std::map< int, std::vector< KWinRateRank > > > > m_mapWinrateRanking;
	std::map< int, std::map< int, std::vector< KExpRank > > >	m_mapExpRanking;

	//  RankType, Char, Page, μ￥AIAI
	std::map< char, std::map< char, std::map< char, std::vector< KMatchRank > > > > m_mapMatchRanking;
	std::map< char, std::map< char, KMatchRank > > m_mapMyMatchRanking;

	std::map<char, std::vector<KRankRewardInfo>> m_mapRPRewardInfo;
	std::map<char, std::vector<KRankRewardInfo>> m_mapWinRecordRewardInfo;

	std::map< ERankType, std::map< char, KWinRateRank > > m_mapMyWinrateRanking;
	std::map< char, KExpRank > m_mapMyExpRanking;

	std::map< DWORD, std::map< char, int > > m_mapOtherUserMonthlyRank;

	std::map< ERankType, std::map< int, std::map< int, std::vector<DWORD> > > >		m_mapRankerReward;

	bool    m_bRefresh;
};
