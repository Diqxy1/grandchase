FALSE = 0
TRUE = 1

-- CharEnum
CHAR_ELESIS = 0
CHAR_LIRE = 1
CHAR_ARME = 2
CHAR_LAS  = 3
CHAR_RYAN = 4
CHAR_RONAN = 5
CHAR_AMY = 6
CHAR_JIN = 7
CHAR_SIEG = 8
CHAR_MARI = 9
CHAR_DIO = 10
CHAR_ZERO = 11
CHAR_LEY = 12
CHAR_LUPUS = 13
CHAR_RIN = 14
CHAR_ASIN = 15

SUCCESS = 0 -- 성공
FAILED = 1  -- 꽝

-- 이벤트 기간 설정
BeginTime   = { 2012, 08, 28, 09 }   -- Y, M, D, H
EndTime     = { 2015, 10, 06, 10 }   -- Y, M, D, H

-- 가챠 복권 버전
GachaLotteryVersion = 1
-- 가챠 복권 ItemID
GachaLotteryItemID = 1000540
-- 가챠 복권 공지 여부
GachaNoticeEnable = TRUE
-- 등급정보 치환 String
GradeStringDesc = 
{
	{ Grade = 1, msgID = "str_12" },
	{ Grade = 2, msgID = "str_13" },
	{ Grade = 3, msgID = "str_14" },	
}
-- 가챠 복권 시스템
InitGachaLottery = 
{
	{
		CharType = CHAR_ELESIS,
		GradeInfo = 
		{
			{ Grade = 1, ItemID = 1000020 },
			{ Grade = 2, ItemID = 795340 },
			{ Grade = 3, ItemID = 1000190 },
		},
		RewardType =
		{ -- { Type, Ratio }
			{ SUCCESS, 50 },
			{ FAILED, 50 },
		},
		SuccessRewardList = 		
		{   -- 설정한 정보 순서대로 등급 디스플레이. (1등 부터 위에서 설정 하셈)
			-- { ItemID, Duration, Period, EquipLevel, Ratio }
			{1000020,-1,-1,80,7.14}, -- 가챠 풀세트
			{795340,-1,-1,0,7.14}, --가챠 펫			
			{1000190,-1,-1,80,7.14}, --가챠 복장세트
			{782390,-1,-1,80,7.14}, --가챠 투구
			{782400,-1,-1,80,7.14}, --가챠 상의
			{782410,-1,-1,80,7.14}, --가챠 하의
			{782420,-1,-1,80,7.14}, --가챠 장갑
			{782430,-1,-1,80,7.14}, --가챠 신발
			{782440,-1,-1,80,7.14}, --가챠 망토			
			{783880,-1,-1,80,7.14}, --가챠 써클릿
			{783890,-1,-1,80,7.15}, --가챠 날개
			{783900,-1,-1,80,7.15}, --가챠 슈즈나이프
			{783910,-1,-1,80,7.15}, --가챠 방패
			{784480,-1,-1,80,7.15}, --가챠 마스크	
			
		},
		FailRewardList = 
		{ -- { ItemID, Duration, Period, Ratio }
			{1000360,1,-1,0,100}, --헛된 꿈
		},
	},
}
