TRUE = 1
FALSE = 0

BeginTime = { 2011, 10, 25, 00 }	-- Y, M, D, H 이벤트 시작 기간.
EndTime = { 2011, 12, 14, 18 }		-- Y, M, D, H 이벤트 종료 기간.
Version = 1
SledgeItem = 8880 -- 썰매 ItemID
LoopEnable = TRUE -- 계속 할지 여부에 대해서 확인.
MaxGrade = 7
MaxPoint = 7000

-- 선물
PresentItemInfo = 
{
	-- { {ItemID,Duration, Period },{ 썰매갯수, 포인트 점수 }  }
	{ ItemInfo = {8880, 1, 1} , Config = { SledgeUseCnt = 5, BonusPoint = 10 }, },
}

-- 기부
DonationInfo =
{
	-- 썰매갯수, 포인트 점수
	{ SledgeUseCnt = 10, BonusPoint = 10 },
	{ SledgeUseCnt = 20, BonusPoint = 20 },
}

-- 등급별 보상
GradeRewardList = 
{
	-- { Grade, Reward = { ItemID, Duration, Period, .., }
	{ Grade = 1, Reward = { }, },
	{ Grade = 2, Reward = { }, },
}

-- 보너스 포인트별 등급
GradeDesc = {
	{ GradeID = 1, StratPoint = 0, EndPoint = 19000 },
	{ GradeID = 2, StratPoint = 19001,	EndPoint = 38000 },
	
}