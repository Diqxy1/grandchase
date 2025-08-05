KR = 1  -- 한국
TW = 3  -- 대만
BR = 4  -- 브라질
US = 8  -- 미국
PH = 9  -- 필리핀
TH = 11 -- 태국
ID = 12 -- 인도네시아
LA = 13 -- 남미

BeginTime = { 2011, 9, 1, 00 }	-- Y, M, D, H 이벤트 시작 기간.
EndTime = { 2011, 9, 30, 00 }	-- Y, M, D, H 이벤트 종료 기간.
Version = 1
TickGap = 1000 * 60        		-- ms
CountryID = KR
CountryStringID = "str_14"		-- 국가 StringID.., 실제 String 설정은 config_center.ini파일에 설정.

-- 등급에 따른 보상
GradeRewardInfo = {
	{
		GradeID = 1,
		ExpReward = 0.0,
		GPReward = 0.0,
	},
	{
		GradeID = 2,
		ExpReward = 0.0,
		GPReward = 0.0,
	},
}

-- 순위에 따른 보상
RankingRewardInfo = {
	{
		RankingNum = 1,
		RewardInfo = { }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 2,
		RewardInfo = { }, -- { ItemID, Duration, Period, .., }
	},
}

-- 점수에 따른 등급
GradeDesc = {
	{ GradeID = 1, StratPoint = 0, EndPoint = 50000	},
	{ GradeID = 2, StratPoint = 50000, EndPoint = 100000 },
}
