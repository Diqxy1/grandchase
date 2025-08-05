-- Survey Types 설문 팝업 상황
GAME_CONNECT = 0 -- 게임접속시
GAME_CLEAR   = 1 -- 게임클리어시
CAHR_LEVELUP = 2 -- 레벨업시

-- 채널링 타입
PUBLISHER1 = 1	-- 채널타입 1 ( 넷마블,  펀박스 )
PUBLISHER2 = 2  -- 채널타입 2 ( 투니랜드, 플레이파크 )
PUBLISHER3 = 3  -- 채널타입 3 ( 네이트.., )

-- Target User Types 설문 대상
NEWBIE   = 1	-- 신규 유저
SAVEUSER = 2    -- 기존 유저
RETURNER = 3	-- 복귀 유저

-- 국가 코드 설정
EU_EN = 0 -- 유럽 영어
EU_GE = 1 -- 독일어
KR_KO = 2 -- 한국어
TW_TW = 3 -- 대만어
BR_PT = 4 -- 포루트뇰
US_EN = 5 -- 미국 영어
PH_EN = 6 -- 필리핀 영어
TH_TH = 7 -- 태국어
ID_ID = 8 -- 인도네시아어
LA_ES = 9 -- 에스파뇰


BeginTime = { 2011, 9, 1, 00 }	-- Y, M, D, H 이벤트 시작 기간.
EndTime = { 2012, 9, 30, 00 }	-- Y, M, D, H 이벤트 종료 기간.
ReturnTime = { 2011, 9, 30, 00 } -- Y, M, D, H 복귀 날짜.
TickGap = 1000 * 60			-- ms
Version = 1

LanguageInfo = {
	KR_KO,
}

GameConnectInfo = {
	Type = GAME_CONNECT,
	TypeInfo = {
		{ ChannelType = PUBLISHER1, UserType = NEWBIE, SurveyIDList = {}, },
		{ ChannelType = PUBLISHER1, UserType = SAVEUSER, SurveyIDList = {}, },
		{ ChannelType = PUBLISHER1, UserType = RETURNER, SurveyIDList = {}, },
	}
}

DungeonClearInfo = {
	Type = GAME_CLEAR,
	DungeonInfo = {
		{ DungeonID = 18, Difficult = 0, SurveyIDList = { 1,2,3, }, },
		{ DungeonID = 26, Difficult = 0, SurveyIDList = { 1,2,3, }, },
		{ DungeonID = 46, Difficult = 0, SurveyIDList = { 1,2,3, }, },
		{ DungeonID = 50, Difficult = 0, SurveyIDList = { 1,2,3, }, },
	},
}

LevelUpInfo = {
	Type = CAHR_LEVELUP,
	-- { LevelRange, 
	LevelInfo = {
		{ CharType = 0, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 1, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 2, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 3, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 4, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 5, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 6, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 7, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 8, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 9, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 10, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 11, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 20, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 40, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 60, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 12, CharLevel = 80, SurveyIDList = { 4,5,6,7, }, },
		{ CharType = 0, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 0, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 0, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 1, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 1, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 1, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 2, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 2, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 2, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 3, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 3, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 3, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 4, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 4, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 4, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 5, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 5, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 5, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 6, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 6, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 6, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 7, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 7, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 7, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 8, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 8, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 8, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 9, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 9, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 9, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 10, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 10, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 10, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 11, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 11, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 11, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
		{ CharType = 12, CharLevel = 30, SurveyIDList = { 8,9,10, }, },
		{ CharType = 12, CharLevel = 50, SurveyIDList = { 8,9,10, }, },
		{ CharType = 12, CharLevel = 70, SurveyIDList = { 8,9,10, }, },
	}
}

SurveyRewardInfo = {
	-- { RewardType, Reward{ ItemID, Duration, Period.., } }
	{ RewardType = GAME_CONNECT, RewardInfo = { 114150, 3, -1, 71440, 3, -1}, },
	{ RewardType = GAME_CLEAR, RewardInfo = { 114150, 3, -1, 71440, 3, -1}, },
	{ RewardType = CAHR_LEVELUP, RewardInfo = { 114150, 3, -1, 71440, 3, -1, 149150, -1, 1 }, },
}
