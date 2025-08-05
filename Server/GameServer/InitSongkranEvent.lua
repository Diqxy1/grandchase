TRUE = 1
FALSE = 0

BeginTime = { 2012, 03, 13, 00 }-- Y, M, D, H 이벤트 시작 기간. Y, M, D 중 하나에 0값을 넣으면 크래쉬 납니다. 체크는 하지만 주의...
EndTime = { 2012, 05, 01, 00 }	-- Y, M, D, H 이벤트 종료 기간.
Version = 1						-- 이벤트 버전
MaxGrade = 6			 		-- 최대 등급 한계치
OneTimeMaxScore = 30			-- 물폭탄 한번에 획득할 수 있는 최대 스코어 ( 해킹 방지용 )
PlayerMoveSpeed = 3.0			-- 유저 캐릭터 이동 속도
WaterBombItem = 8880			-- 물폭탄 ItemID
WaterBombFylingSpeed = 10.0		-- 물폭탄 날아가는 속도 ( 초 단위 )
WaterBombRange = 20.0			-- 물폭탄 폭발 범위
WaterBombSupplyInterval = 1 	-- 물폭탄 지급 시간 텀 ( 분 단위 )
WaterBombSupplyDuration = 5		-- 물폭탄 지급 갯수
DailyMaxSupplyCount = 30		-- 물폭탄 하루 최대 지급 가능 수량 제한

-- 타겟 타입
--	CHAR_ELESIS = 0,
--	CHAR_ZERO	= 1,
--	CHAR_LAS	= 2,
--	CHAR_ARME	= 3,
--	CHAR_REY	= 4,
--	CHAR_SIZE	= 5,

TargetInfoList = 
{
	-- { 타겟 타입, 타겟 이동 속도, 타겟 점수, 텔레포트 주기, 폭탄 맞고 리셋 되는 시간 },
	{ CharType = 0, MoveSpeed = 2.0, Score = 1, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 1, MoveSpeed = 1.0, Score = 2, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 2, MoveSpeed = 4.0, Score = 3, TelpoPeriod = 1.0, ResetTime = 1.0 },
	{ CharType = 3, MoveSpeed = 1.0, Score = 4, TelpoPeriod = 3.0, ResetTime = 1.0 },
	{ CharType = 4, MoveSpeed = 1.0, Score = 5, TelpoPeriod = 3.0, ResetTime = 1.0 },

}

GradeRewardList = 
{
	-- { 등급, 등급의EndPoint, SpecialReward, Reward = { ItemID, Duration, Period, .., }
	{ Grade = 1, GradeEndScore = 40, SpecialReward = FALSE, Reward = { 395110, -1, -1, }, },
	{ Grade = 2, GradeEndScore = 70, SpecialReward = FALSE, Reward = { 395100, -1, -1, }, },
	{ Grade = 3, GradeEndScore = 100, SpecialReward = TRUE, Reward = { 395090, -1, -1, }, },
	{ Grade = 4, GradeEndScore = 150, SpecialReward = FALSE, Reward = { 395080, -1, -1, }, },
	{ Grade = 5, GradeEndScore = 200, SpecialReward = FALSE, Reward = { 395070, -1, -1, }, },
	{ Grade = 6, GradeEndScore = 270, SpecialReward = TRUE, Reward = { 394350, -1, -1, }, }, 
}
