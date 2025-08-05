KR = 1  -- 한국
TW = 3  -- 대만
BR = 4  -- 브라질
US = 8  -- 미국
PH = 9  -- 필리핀
TH = 11 -- 태국
ID = 12 -- 인도네시아
LA = 13 -- 남미

BeginTime = { 2011, 10, 25, 00 }	-- Y, M, D, H 이벤트 시작 기간.
EndTime = { 2011, 11, 15, 00 }	-- Y, M, D, H 이벤트 종료 기간.
Version = 1
TickGap = 1000 * 60        		-- ms
CountryID = TW
CountryStringID = "str_14"		-- 국가 StringID.., 실제 String 설정은 config_center.ini파일에 설정.

-- 등급에 따른 보상
GradeRewardInfo = {
	{ GradeID = 1,	ExpReward = 0.0,	GPReward = 0.0,	},
	{ GradeID = 2,	ExpReward = 0.05,	GPReward = 0.05,},
	{ GradeID = 3,	ExpReward = 0.1, 	GPReward = 0.1,},
	{ GradeID = 4,	ExpReward = 0.15, 	GPReward = 0.15,},
	{ GradeID = 5,	ExpReward = 0.2, 	GPReward = 0.2, },
	{ GradeID = 6,	ExpReward = 0.25, 	GPReward = 0.25, },
	{ GradeID = 7,	ExpReward = 0.3, 	GPReward = 0.3, },
	{ GradeID = 8,	ExpReward = 0.35,	GPReward = 0.35, },
	{ GradeID = 9,	ExpReward = 0.4,	GPReward = 0.4, },
	{ GradeID = 10, ExpReward = 0.5,	GPReward = 0.5, },
	{ GradeID = 11, ExpReward = 0.6,	GPReward = 0.6, },
	{ GradeID = 12, ExpReward = 0.7,	GPReward = 0.7, },
	{ GradeID = 13, ExpReward = 0.8,	GPReward = 0.8, },
	{ GradeID = 14, ExpReward = 0.9,	GPReward = 0.9, },
	{ GradeID = 15, ExpReward = 1.0,	GPReward = 1.0, },
	
}

-- 순위에 따른 보상
RankingRewardInfo = {
	{
		RankingNum = 1,
		RewardInfo = {728180, 10, -1, 35030, 5, -1, 149150, -1, 3}, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 2,
		RewardInfo = {728180, 8, -1, 35030, 3, -1, 149150, -1, 2 }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 3,
		RewardInfo = {728180, 6, -1, 35030, 2, -1, 149150, -1, 1 }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 4,
		RewardInfo = {728180, 4, -1, 35030, 1, -1, 149150, -1, 1 }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 5,
		RewardInfo = {728180, 3, -1 }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 6,
		RewardInfo = {728180, 2, -1 }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 7,
		RewardInfo = {728180, 1, -1 }, -- { ItemID, Duration, Period, .., }
	},
	
}

-- 점수에 따른 등급
GradeDesc = {
	{ GradeID = 1, StratPoint = 0,			EndPoint = 190000000 },
	{ GradeID = 2, StratPoint = 190000001,	EndPoint = 380000000 },
	{ GradeID = 3, StratPoint = 380000001,	EndPoint = 570000000 },
	{ GradeID = 4, StratPoint = 570000001,	EndPoint = 760000000 },
	{ GradeID = 5, StratPoint = 760000001,	EndPoint = 950000000 },
	{ GradeID = 6, StratPoint = 950000001,	EndPoint = 1140000000},
	{ GradeID = 7, StratPoint = 1140000001,	EndPoint = 1330000000},
	{ GradeID = 8, StratPoint = 1330000001, EndPoint = 1520000000},
	{ GradeID = 9, StratPoint = 1520000001, EndPoint = 1710000000},
	{ GradeID = 10, StratPoint = 1710000001, EndPoint = 1900000000},
	{ GradeID = 11, StratPoint = 1900000001, EndPoint = 2090000000},
	{ GradeID = 12, StratPoint = 2090000001, EndPoint = 2280000000},
	{ GradeID = 13, StratPoint = 2280000001, EndPoint = 2470000000},
	{ GradeID = 14, StratPoint = 2470000001, EndPoint = 2660000000},
	{ GradeID = 15, StratPoint = 2660000001, EndPoint = 2850000000},
	
}
