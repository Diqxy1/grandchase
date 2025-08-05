#include "stdafx.h"
//
#include ".\gcranking.h"
#include "MyD3D.h"
#include "Procedure.h"

//
//

KGCRanking::KGCRanking(void)
	: m_bRefresh(false)
{
}

KGCRanking::~KGCRanking(void)
{
}

void KGCRanking::SetRankingPage(int iRankingType, int iChartype, int iPage, const std::vector< KWinRateRank >& vecRanking)
{
	m_mapWinrateRanking[(ERankType)iRankingType][iChartype][iPage] = vecRanking;
}

void KGCRanking::SetRankingPage(int iRankingType, int iChartype, int iPage, const std::vector< KExpRank >& vecRanking)
{
	m_mapExpRanking[iChartype][iPage] = vecRanking;
}

const std::vector< KWinRateRank >& KGCRanking::GetWinrateRanking(int iRankingType, int iChartype, int iPage)
{
	if (m_mapWinrateRanking.count((ERankType)iRankingType) == 0)
	{
		SiKP2P()->Send_RankPageReq(iRankingType, iChartype, iPage);

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}
	else if (m_mapWinrateRanking[(ERankType)iRankingType].count(iChartype) == 0)
	{
		SiKP2P()->Send_RankPageReq(iRankingType, iChartype, iPage);

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}
	else if (m_mapWinrateRanking[(ERankType)iRankingType][iChartype].count(iPage) == 0)
	{
		SiKP2P()->Send_RankPageReq(iRankingType, iChartype, iPage);

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}

	return m_mapWinrateRanking[(ERankType)iRankingType][iChartype][iPage];
}

const std::vector< KExpRank >& KGCRanking::GetExpRanking(int iChartype, int iPage)
{
	if (m_mapExpRanking.count(iChartype) == 0)
	{
		if (GC_GLOBAL_DEFINE::bAutoMatchRanking)
		{
			SiKP2P()->Send_Match_Rank_Page_Req(KMatchRank::MRT_EXP, iChartype, iPage);
		}
		else
		{
			SiKP2P()->Send_RankPageReq(RT_EXP, iChartype, iPage);
		}

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}
	else if (m_mapExpRanking[iChartype].count(iPage) == 0)
	{
		if (GC_GLOBAL_DEFINE::bAutoMatchRanking)
		{
			SiKP2P()->Send_Match_Rank_Page_Req(KMatchRank::MRT_EXP, iChartype, iPage);
		}
		else
		{
			SiKP2P()->Send_RankPageReq(RT_EXP, iChartype, iPage);
		}

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}

	return m_mapExpRanking[iChartype][iPage];
}

void KGCRanking::SetMyRanking(int iRankingType, const std::map< char, KWinRateRank >& mapMyRanking)
{
	m_mapMyWinrateRanking[(ERankType)iRankingType] = mapMyRanking;
}

void KGCRanking::SetMyRanking(const std::map< char, KExpRank >& mapMyRanking)
{
	m_mapMyExpRanking = mapMyRanking;
}

const KWinRateRank* KGCRanking::GetMyWinrateRanking(int iRankingType, int iChartype /*= -1 */)
{
	if (m_mapMyWinrateRanking.count((ERankType)iRankingType) == 0)
		return NULL;

	if (m_mapMyWinrateRanking[(ERankType)iRankingType].count(iChartype) == 0)
		return NULL;

	return &m_mapMyWinrateRanking[(ERankType)iRankingType][iChartype];
}

const KExpRank* KGCRanking::GetMyExpRanking(int iChartype /*= -1 */)
{
	if (m_mapMyExpRanking.find(iChartype) == m_mapMyExpRanking.end())
		return NULL;

	return &m_mapMyExpRanking[iChartype];
}

void KGCRanking::FindRankingInfo(int iRankingType, int iChartype, const std::wstring& strFind)
{
	SiKP2P()->Send_RankSearchReq(iRankingType, iChartype, strFind);
}

void KGCRanking::FindMatchRankingInfo(char cRankType, char cCharType, std::wstring strFind)
{
	SiKP2P()->Send_Match_Rank_Search_Req(cRankType, cCharType, strFind);
}

int KGCRanking::GetBestExpRankerLevel(int iChartype)
{
	if (m_mapExpRanking.find(iChartype) == m_mapExpRanking.end())
		return g_kGlobalValue.GetMaxLevel();

	if (m_mapExpRanking[iChartype].find(1) == m_mapExpRanking[iChartype].end())
		return g_kGlobalValue.GetMaxLevel();

	if (m_mapExpRanking[iChartype][1].empty())
		return g_kGlobalValue.GetMaxLevel();

	return Exp_2_Level(m_mapExpRanking[iChartype][0][0].m_biExp);
}

int KGCRanking::GetRankingFromNick(int iRankingtype, int iChartype, DWORD dwUserUID)
{
	if (m_mapOtherUserMonthlyRank.find(dwUserUID) == m_mapOtherUserMonthlyRank.end())
		return -1;

	if (m_mapOtherUserMonthlyRank[dwUserUID].find(iChartype) == m_mapOtherUserMonthlyRank[dwUserUID].end())
		return -1;

	if (m_mapOtherUserMonthlyRank[dwUserUID][iChartype] > 100)
		return -1;

	return m_mapOtherUserMonthlyRank[dwUserUID][iChartype];
}

int KGCRanking::AddUserRankingInfo(DWORD dwUserUID, const std::map< char, KWinRateRank >& m_mapRankInfo)
{
	std::map< char, KWinRateRank >::const_iterator mit;

	for (mit = m_mapRankInfo.begin(); mit != m_mapRankInfo.end(); ++mit)
	{
		m_mapOtherUserMonthlyRank[dwUserUID][mit->first] = mit->second.m_nRank;
	}

	return 0;
}

int KGCRanking::AddUserRankingInfo(DWORD dwUserUID, const std::map< char, int >& m_mapRankInfo)
{
	m_mapOtherUserMonthlyRank[dwUserUID] = m_mapRankInfo;
	return 0;
}
int KGCRanking::ClearOtherUserRank()
{
	m_mapOtherUserMonthlyRank.clear();
	return 0;
}

std::vector<DWORD> KGCRanking::GetRankerReward(int iRankingtype, int iCharType, int iRanking)
{
	if (m_mapRankerReward.find((ERankType)iRankingtype) == m_mapRankerReward.end())
		return std::vector<DWORD>();

	if (m_mapRankerReward[(ERankType)iRankingtype].find(iCharType) == m_mapRankerReward[(ERankType)iRankingtype].end())
		return std::vector<DWORD>();

	if (m_mapRankerReward[(ERankType)iRankingtype][iCharType].find(iRanking) == m_mapRankerReward[(ERankType)iRankingtype][iCharType].end())
		return std::vector<DWORD>();

	return m_mapRankerReward[(ERankType)iRankingtype][iCharType][iRanking];
}

void KGCRanking::SetAutoMatchRanking(char cRankType, char cCharType, char cPageNum, const std::vector< KMatchRank >& vecRanking)
{
	m_mapMatchRanking[cRankType][cCharType][cPageNum] = vecRanking;
}

const std::vector< KMatchRank >& KGCRanking::GetAutoMatchRanking(char cRankType, char cCharType, char cPageNum)
{
	if (m_mapMatchRanking.count(cRankType) == 0)
	{
		SiKP2P()->Send_Match_Rank_Page_Req(cRankType, cCharType, cPageNum);

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}
	else if (m_mapMatchRanking[cRankType].count(cCharType) == 0)
	{
		SiKP2P()->Send_Match_Rank_Page_Req(cRankType, cCharType, cPageNum);

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}
	else if (m_mapMatchRanking[cRankType][cCharType].count(cPageNum) == 0)
	{
		SiKP2P()->Send_Match_Rank_Page_Req(cRankType, cCharType, cPageNum);

		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck(Result_GuildRankingInfo_Req, INT_MAX, 3000, TIME_OUT_VALUE);
	}

	return m_mapMatchRanking[cRankType][cCharType][cPageNum];
}

const KMatchRank* KGCRanking::GetMyAutoMatchRanking(char cRankType, char cCharTypee)
{
	//  갱신되었으면 일단 다시 요청 하고 다음에 갱신한다.
	if (m_bRefresh)
	{
		SiKP2P()->Send_My_Match_Rank_Info_Req();
		m_bRefresh = false;
	}

	if (m_mapMyMatchRanking.count(cRankType) == 0)
		return NULL;

	if (m_mapMyMatchRanking[cRankType].count(cCharTypee) == 0)
		return NULL;

	return &m_mapMyMatchRanking[cRankType][cCharTypee];
}

void KGCRanking::SetMyAutoMatchRanking(const std::map<char, KMatchRank>& mapRPRanking, const std::map<char, KMatchRank>& mapWinRanking)
{
	m_mapMyMatchRanking[KMatchRank::MRT_RP] = mapRPRanking;
	m_mapMyMatchRanking[KMatchRank::MRT_WINRECORD] = mapWinRanking;
}

// std::vector<DWORD> KGCRanking::GetAutoMatchReward(  char cCharType, int iRanking )
// {
// m_mapRankerReward[iRankingtype][cCharType][iRanking];
// }

void KGCRanking::LoadRankgerReward()
{
	KGCLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "rankerreward.stg") == false)
		return;

	m_mapRankerReward.clear();
	LUA_BEGIN_TABLE("RankerReward", return);
	{
		for (int i = 1;; ++i)
		{
			int iCharType = -1;
			int iRankingType = -1;
			int iItemID = -1;
			int iRanking = 1;
			LUA_BEGIN_TABLE(i, break;)
			{
				LUA_GET_VALUE("CharType", iCharType, break);
				LUA_GET_VALUE("RankingType", iRankingType, break);
				LUA_GET_VALUE("ItemID", iItemID, break);
				LUA_GET_VALUE_DEF("Ranking", iRanking, 1);
			}
			LUA_END_TABLE(break; )

				m_mapRankerReward[(ERankType)iRankingType][iCharType][iRanking].push_back(iItemID);
		}
	}
	LUA_END_TABLE(return);
}

void KGCRanking::RefreshAutoMatchRanking()
{
	m_bRefresh = true;

	//  랭킹 게시판 정보 지운다. // 그럼 다시 불러올때 새로 로드 한다.
	m_mapMatchRanking.clear();
}

void KGCRanking::SetAutoMatchReward(std::map<char, std::vector<KRankRewardInfo>> mapRPRewardInfo,
	std::map<char, std::vector<KRankRewardInfo>> mapWinRecordRewardInfo)
{
	m_mapRPRewardInfo = mapRPRewardInfo;
	m_mapWinRecordRewardInfo = mapWinRecordRewardInfo;
}

std::vector<DWORD> KGCRanking::GetAutoMatchReward(char cRankType, char cCharType, int iRanking)
{
	std::vector<DWORD> vecTemp;

	if (cRankType == KRankRewardInfo::RT_RP)
	{
		if (m_mapRPRewardInfo.empty())
			return vecTemp;

		if (m_mapRPRewardInfo.find(cCharType) == m_mapRPRewardInfo.end())
			return vecTemp;

		if ((int)m_mapRPRewardInfo[cCharType].size() <= iRanking)
			return vecTemp;

		return m_mapRPRewardInfo[cCharType][iRanking].m_vecRewardInfo;
	}
	else
	{
		if (m_mapWinRecordRewardInfo.empty())
			return vecTemp;

		if (m_mapWinRecordRewardInfo.find(cCharType) == m_mapRPRewardInfo.end())
			return vecTemp;

		if ((int)m_mapWinRecordRewardInfo[cCharType].size() <= iRanking)
			return vecTemp;

		return m_mapWinRecordRewardInfo[cCharType][iRanking].m_vecRewardInfo;
	}

	return vecTemp;
}