-- 올림픽 이벤트 스크립트.
OLYMPIC_GAME_TREEDROP = 1 -- 통나무 깨기
OLYMPIC_GAME_BALLOON = 2 -- 풍선터뜨리기
OLYMPIC_GAME_WATERBOMB = 3 -- 물풍선 던지기
OLYMPIC_GAME_GAWIBAWIBO = 4 -- 가위바위보
OLYMPIC_GAME_DICEPLAY = 5 -- 주사위 굴리기
OLYMPIC_GAME_BOSSGATE = 6 -- 보스게이트

SOLO = 1 -- 개인전
TEAM = 2 -- 단체전

--*****************************
-- 국가 정보 추가 필요
NATION_KR = 1
NATION_TW = 2
NATION_BR = 3
NATION_US = 4
NATION_PH = 5
NATION_TH = 6
NATION_ID = 7
NATION_LA = 8

CountryCode = NATION_KR

-- 이벤트 기간.
BeginTime   = { 2012, 07, 18, 05 }   -- Y, M, D, H
EndTime     = { 2012, 08, 16, 21 }   -- Y, M, D, H

-- 게임 중복설정은 되지 않는다.
-- 시작날짜 ~ 종료날짜별로 데이터를 가지고 있다.
-- 날짜 설정 시간정보에 24로 설정하면 안됨.
-- 시간정보는 예를 들어서 0으로 세팅해 두면 전달 11시 59분까지만 플레이 가능.
-- 스케줄 정보를 세팅할때는 항상 시간순으로 세팅한다.
OlympicSchedule = 
{
	-- Y, M, D, H
	{ RegDate = { 2012, 07, 18, 00 }, GameType = OLYMPIC_GAME_DICEPLAY, TeamType = SOLO, },
	{ RegDate = { 2012, 07, 19, 00 }, GameType = OLYMPIC_GAME_DICEPLAY, TeamType = TEAM, },
	{ RegDate = { 2012, 07, 20, 00 }, GameType = OLYMPIC_GAME_TREEDROP, TeamType = SOLO, },
	{ RegDate = { 2012, 07, 21, 00 }, GameType = OLYMPIC_GAME_TREEDROP, TeamType = TEAM, },
	{ RegDate = { 2012, 07, 22, 00 }, GameType = OLYMPIC_GAME_GAWIBAWIBO, TeamType = SOLO, },
	{ RegDate = { 2012, 07, 23, 00 }, GameType = OLYMPIC_GAME_GAWIBAWIBO, TeamType = TEAM, },
	{ RegDate = { 2012, 07, 24, 00 }, GameType = OLYMPIC_GAME_BOSSGATE, TeamType = SOLO, },
	{ RegDate = { 2012, 07, 25, 00 }, GameType = OLYMPIC_GAME_BOSSGATE, TeamType = TEAM, },
	{ RegDate = { 2012, 07, 26, 00 }, GameType = OLYMPIC_GAME_WATERBOMB, TeamType = SOLO, },
	{ RegDate = { 2012, 07, 27, 00 }, GameType = OLYMPIC_GAME_WATERBOMB, TeamType = TEAM, },
	{ RegDate = { 2012, 07, 28, 00 }, GameType = OLYMPIC_GAME_BALLOON, TeamType = SOLO, },
	{ RegDate = { 2012, 07, 29, 00 }, GameType = OLYMPIC_GAME_BALLOON, TeamType = TEAM, },	
}

-- 게임Type별 세부사항
OlympicGameInfo = 
{
    {
		GameType = OLYMPIC_GAME_TREEDROP,
		DefaultCount = 0, -- 기본 횟수( 필요없는 모드는 0으로 세팅 )
		RewardCount = 0, -- 보상 Count( 필요없는 모드는 0으로 세팅 )
		OnePointMaxPoint = 10000, -- 한판 보상 수치( 필요없는 모드는 0으로 세팅 )
		TotalMaxPoint = 999999,-- 전체 보상 수치( 필요없는 모드는 0으로 세팅 )
		RewardInfo = { 977510, 1, -1  },-- RewardInfo = { ItemID, Duration, Period }
    },
	{
		GameType = OLYMPIC_GAME_BALLOON,
		DefaultCount = 0,
		RewardCount = 0,
		OnePointMaxPoint = 10000, -- 한판 보상 수치( 필요없는 모드는 0으로 세팅 )
		TotalMaxPoint = 999999,-- 전체 보상 수치( 필요없는 모드는 0으로 세팅 )
		RewardInfo = { 977510, 1, -1  },
	},
    {
		GameType = OLYMPIC_GAME_WATERBOMB,
		DefaultCount = 10,
		RewardCount = 0,
		OnePointMaxPoint = 500, -- 한판 보상 수치( 필요없는 모드는 0으로 세팅 )
		TotalMaxPoint = 99999,-- 전체 보상 수치( 필요없는 모드는 0으로 세팅 )
		RewardInfo = { 977510, 1, -1  },
	},
    {
		GameType = OLYMPIC_GAME_GAWIBAWIBO,
		DefaultCount = 2,
		RewardCount = 0,
		OnePointMaxPoint = 500, -- 한판 보상 수치( 필요없는 모드는 0으로 세팅 )
		TotalMaxPoint = 99999,-- 전체 보상 수치( 필요없는 모드는 0으로 세팅 )
		RewardInfo = { 977510, 1, -1  },
	},
    {
		GameType = OLYMPIC_GAME_DICEPLAY,
		DefaultCount = 3,
		RewardCount = 0,
		OnePointMaxPoint = 100, -- 한판 보상 수치( 필요없는 모드는 0으로 세팅 )
		TotalMaxPoint = 99999,-- 전체 보상 수치( 필요없는 모드는 0으로 세팅 )
		RewardInfo = { 977510, 1, -1  },
    },
    {
		GameType = OLYMPIC_GAME_BOSSGATE,
		DefaultCount = 0,
		RewardCount = 1,
		OnePointMaxPoint = 100, -- 한판 보상 수치( 필요없는 모드는 0으로 세팅 )
		TotalMaxPoint = 99999,-- 전체 보상 수치( 필요없는 모드는 0으로 세팅 )
		RewardInfo = { 977520, 1, -1 },
    },
}

-- 등급 보상에 대한 정보
GradeRewardInfo = 
{
	{ Grade = 1, RewardInfo = { 506040, -1, 1 }, }, -- [Grade, RewardInfo = { ItemID, Duration, Period } ]
	{ Grade = 2, RewardInfo = { 149150, -1, 3 }, },
	{ Grade = 3, RewardInfo = { 197370, -1, -1 }, },
	{ Grade = 4, RewardInfo = { 779620, 1, -1 }, },
	{ Grade = 5, RewardInfo = { 42340, -1, 3 }, },
	{ Grade = 6, RewardInfo = { 627300, 1, -1 }, },
	{ Grade = 7, RewardInfo = { 224930, 1, -1 }, },
}

-- 등급별 보상 디자인.
GradeRewardDesc = 
{
	MaxGrade = 7,
	MaxPoint = 210,
	{ GradeID = 1, StratPoint = 0,      EndPoint = 199 },
	{ GradeID = 2, StratPoint = 200,    EndPoint = 399 },
	{ GradeID = 3, StratPoint = 400,    EndPoint = 599 },
	{ GradeID = 4, StratPoint = 600,    EndPoint = 799 },
	{ GradeID = 5, StratPoint = 800,    EndPoint = 999 },
	{ GradeID = 6, StratPoint = 1000,	EndPoint = 1199 },
	{ GradeID = 7, StratPoint = 1200,	EndPoint = 2400 },	
}

-- 물폭탄 정보

-- 가위바위보 승률 정보
GawibawiboInfo =
{
    { RemainTryPoint = 2, Win = 33, Lose = 33, }, -- 도전 포인트가 2일 경우 가위바위보 승률. 나머지는 비김
    { RemainTryPoint = 1, Win = 33, Lose = 33, }, -- 도전 포인트가 1일 경우 가위바위보 승률. 나머지는 비김
}

-- TimeIndex 
-- 해당 스크립트는 손대지 마세요.
-- 수정가능한 구조지만 현재 올림픽 이벤트시에는 수정 필요없습니다.
TimeIndexDesc = 
{
	GameAlarmNotice = { 0, 30 }, --Min
	-- index, [Hour,Min], [Hour,Min]
	{ Index = 0, StartTime = { 00,00 }, EndTime = { 00,29 } },
	{ Index = 1, StartTime = { 00,30 }, EndTime = { 00,59 } },
	{ Index = 2, StartTime = { 01,00 }, EndTime = { 01,29 } },
	{ Index = 3, StartTime = { 01,30 }, EndTime = { 01,59 } },
	{ Index = 4, StartTime = { 02,00 }, EndTime = { 02,29 } },
	{ Index = 5, StartTime = { 02,30 }, EndTime = { 02,59 } },
	{ Index = 6, StartTime = { 03,00 }, EndTime = { 03,29 } },
	{ Index = 7, StartTime = { 03,30 }, EndTime = { 03,59 } },
	{ Index = 8, StartTime = { 04,00 }, EndTime = { 04,29 } },
	{ Index = 9, StartTime = { 04,30 }, EndTime = { 04,59 } },
	{ Index = 10, StartTime = { 05,00 }, EndTime = { 05,29 } },
	{ Index = 11, StartTime = { 05,30 }, EndTime = { 05,59 } },
	{ Index = 12, StartTime = { 06,00 }, EndTime = { 06,29 } },
	{ Index = 13, StartTime = { 06,30 }, EndTime = { 06,59 } },
	{ Index = 14, StartTime = { 07,00 }, EndTime = { 07,29 } },
	{ Index = 15, StartTime = { 07,30 }, EndTime = { 07,59 } },
	{ Index = 16, StartTime = { 08,00 }, EndTime = { 08,29 } },
	{ Index = 17, StartTime = { 08,30 }, EndTime = { 08,59 } },
	{ Index = 18, StartTime = { 09,00 }, EndTime = { 09,29 } },
	{ Index = 19, StartTime = { 09,30 }, EndTime = { 09,59 } },
	{ Index = 20, StartTime = { 10,00 }, EndTime = { 10,29 } },
	{ Index = 21, StartTime = { 10,30 }, EndTime = { 10,59 } },
	{ Index = 22, StartTime = { 11,00 }, EndTime = { 11,29 } },
	{ Index = 23, StartTime = { 11,30 }, EndTime = { 11,59 } },
	{ Index = 24, StartTime = { 12,00 }, EndTime = { 12,29 } },
	{ Index = 25, StartTime = { 12,30 }, EndTime = { 12,59 } },
	{ Index = 26, StartTime = { 13,00 }, EndTime = { 13,29 } },
	{ Index = 27, StartTime = { 13,30 }, EndTime = { 13,59 } },
	{ Index = 28, StartTime = { 14,00 }, EndTime = { 14,29 } },
	{ Index = 29, StartTime = { 14,30 }, EndTime = { 14,59 } },
	{ Index = 30, StartTime = { 15,00 }, EndTime = { 15,29 } },
	{ Index = 31, StartTime = { 15,30 }, EndTime = { 15,59 } },
	{ Index = 32, StartTime = { 16,00 }, EndTime = { 16,29 } },
	{ Index = 33, StartTime = { 16,30 }, EndTime = { 16,59 } },
	{ Index = 34, StartTime = { 17,00 }, EndTime = { 17,29 } },
	{ Index = 35, StartTime = { 17,30 }, EndTime = { 17,59 } },
	{ Index = 36, StartTime = { 18,00 }, EndTime = { 18,29 } },
	{ Index = 37, StartTime = { 18,30 }, EndTime = { 18,59 } },
	{ Index = 38, StartTime = { 19,00 }, EndTime = { 19,29 } },
	{ Index = 39, StartTime = { 19,30 }, EndTime = { 19,59 } },
	{ Index = 40, StartTime = { 20,00 }, EndTime = { 20,29 } },
	{ Index = 41, StartTime = { 20,30 }, EndTime = { 20,59 } },
	{ Index = 42, StartTime = { 21,00 }, EndTime = { 21,29 } },
	{ Index = 43, StartTime = { 21,30 }, EndTime = { 21,59 } },
	{ Index = 44, StartTime = { 22,00 }, EndTime = { 22,29 } },
	{ Index = 45, StartTime = { 22,30 }, EndTime = { 22,59 } },
	{ Index = 46, StartTime = { 23,00 }, EndTime = { 23,29 } },
	{ Index = 47, StartTime = { 23,30 }, EndTime = { 23,59 } },
}