--게임서버 매칭 로직 스크립트
TRUE = 1
FALSE = 0

-- 파티 매칭
-- 파티장 포함해서 3명만 파티를 할 수 있다.
MatchPartyMaxNum = 3
-- 매칭서버에서 사용되는 부분인데 게임서버에서는 클라이언트에 전달을 위해서 사용.
-- 해당 값은 대기시간 계산용이며, InitMatchInfo.lua에 값이 수정될때 함께 수정되어야 함.
bLimitMatchEnable = TRUE -- 최소 설정값을 제어할 수 있도록 ON/OFF 기능 추가.
LimitMatchGroupNum = 15 -- 최소 설정값 이상의 그룹이 되어야 매칭검색을 시작한다.
SeasonYear = 2012
SeasonVersion = 0

-- ELO Rating 상수 K Table
MaxConstantK = 24 -- 상수 K 최대값.
MaxReceiveRP = 64 -- 획득할 수 있는 최대 RP값.
ConstantKTable = 
{
	-- RA 범위( 시작, 끝 ), 상수 K값
	{ RaStartValue = 0, RaEndValue = 2000, ConstantK = 32 },
	{ RaStartValue = 2001, RaEndValue = 10000, ConstantK = 24 },
}

-- 추가 EXP, GP 배율 설정
-- 추가 EXP, GP 획득은 게임중에 획득한 총 EXP, GP에서 배율값을 곱해서 나온값.
-- 1로 설정하면 100% 추가 보상 배율임.
ELOAddExpRatio = 0.5
ELOAddGpRatio = 0.5

-- 배치고사 진입 RP Table
MaxStartRPValue = 1851 -- 시작 RP 최대값.( 실수에 대한 보정을 위해서 구분 )
PlacementTestRPTable = 
{
	-- 캐릭터 레벨 범위, 시작 Rating
	{ StartLevel = 0, EndLevel = 11, StartRPValue = 300 },
	{ StartLevel = 12, EndLevel = 23, StartRPValue = 500 },
	{ StartLevel = 24, EndLevel = 35, StartRPValue = 650 },
	{ StartLevel = 36, EndLevel = 44, StartRPValue = 750 },
	{ StartLevel = 45, EndLevel = 53, StartRPValue = 850 },
	{ StartLevel = 54, EndLevel = 62, StartRPValue = 950 },
	{ StartLevel = 63, EndLevel = 71, StartRPValue = 1250 },
	{ StartLevel = 72, EndLevel = 80, StartRPValue = 1550 },
	{ StartLevel = 81, EndLevel = 85, StartRPValue = 1851 },
}
PlacementTestPlayCount = 12 -- 배치고사 판수 설정.
PlacementTestFirstConstantK = 300 -- 배치고사 초기 상수 K값.
PlacementTestRatio = 0.7 -- 배치고사 전판과 결과 다를 경우 배율값.
PlacementTestMaxReceiveRP = 600 -- 배치고사때 획득할 수 있는 최대 RP값.

-- 거절시 패널티 적용 정보.
PenaltyEnable = FALSE -- 패널티 사용여부.
PenaltyRefuseCount = 1 -- 패널티 적용 거절 횟수.
PenaltyTime = 1000 * 60 * 5 -- 패널티 적용 시간( 현재 스크립트상의 적용값은 5분 )

-- RP단계 테이블 추가.
MaxGrade = 11
RPMarkGradeTable = 
{
	{ StartRP = 0, EndRP = 499, Grade = 1 },
	{ StartRP = 500, EndRP = 699, Grade = 2 },
	{ StartRP = 700, EndRP = 899, Grade = 3 },
	{ StartRP = 900, EndRP = 1099, Grade = 4 },
	{ StartRP = 1100, EndRP = 1299, Grade = 5 },
	{ StartRP = 1300, EndRP = 1499, Grade = 6 },
	{ StartRP = 1500, EndRP = 1699, Grade = 7 },
	{ StartRP = 1700, EndRP = 1899, Grade = 8 },
	{ StartRP = 1900, EndRP = 2099, Grade = 9 },
	{ StartRP = 2100, EndRP = 2299, Grade = 10 },
}

-- RP 하락 기준치 테이블.
RPMarkDecreaseGradeTable = 
{
	{ StartRP = 0, EndRP = 399, Grade = 1 },
	{ StartRP = 400, EndRP = 599, Grade = 2 },
	{ StartRP = 600, EndRP = 799, Grade = 3 },
	{ StartRP = 800, EndRP = 999, Grade = 4 },
	{ StartRP = 1000, EndRP = 1199, Grade = 5 },
	{ StartRP = 1200, EndRP = 1399, Grade = 6 },
	{ StartRP = 1400, EndRP = 1599, Grade = 7 },
	{ StartRP = 1600, EndRP = 1799, Grade = 8 },
	{ StartRP = 1800, EndRP = 1999, Grade = 9 },
	{ StartRP = 2000, EndRP = 2199, Grade = 10 },
}

-- 서버에서 사용하는 테이블( 손대면 안댐 )
RPGradeTable = 
{
	{ StartRP = 0, EndRP = 150, DefineRP = 150 },
--	{ StartRP = 151, EndRP = 300, DefineRP = 300 },
--	{ StartRP = 301, EndRP = 450, DefineRP = 450 },
--	{ StartRP = 451, EndRP = 600, DefineRP = 600 },
--	{ StartRP = 601, EndRP = 750, DefineRP = 750 },
--	{ StartRP = 751, EndRP = 900, DefineRP = 900 },
--	{ StartRP = 901, EndRP = 1050, DefineRP = 1050 },
--	{ StartRP = 1051, EndRP = 1200, DefineRP = 1200 },
--	{ StartRP = 1201, EndRP = 1350, DefineRP = 1350 },
--	{ StartRP = 1351, EndRP = 1500, DefineRP = 1500 },
--	{ StartRP = 1501, EndRP = 1650, DefineRP = 1650 },
--	{ StartRP = 1651, EndRP = 1800, DefineRP = 1800 },
--	{ StartRP = 1801, EndRP = 1950, DefineRP = 1950 },
--	{ StartRP = 1951, EndRP = 2100, DefineRP = 2100 },
--	{ StartRP = 2101, EndRP = 2250, DefineRP = 2250 },
--	{ StartRP = 2251, EndRP = 2400, DefineRP = 2400 },
--	{ StartRP = 2401, EndRP = 2550, DefineRP = 2550 },
--	{ StartRP = 2551, EndRP = 2700, DefineRP = 2700 },
--	{ StartRP = 2701, EndRP = 2850, DefineRP = 2850 },
--	{ StartRP = 2851, EndRP = 3000, DefineRP = 3000 },
}
--[[
RPGradeTable = 
{
	{ StartRP = 0, EndRP = 100, DefineRP = 100 },
	{ StartRP = 101, EndRP = 200, DefineRP = 200 },
	{ StartRP = 201, EndRP = 300, DefineRP = 300 },
	{ StartRP = 301, EndRP = 400, DefineRP = 400 },
	{ StartRP = 401, EndRP = 500, DefineRP = 500 },
	{ StartRP = 501, EndRP = 600, DefineRP = 600 },
	{ StartRP = 601, EndRP = 700, DefineRP = 700 },
	{ StartRP = 701, EndRP = 800, DefineRP = 800 },
	{ StartRP = 801, EndRP = 900, DefineRP = 900 },
	{ StartRP = 901, EndRP = 1000, DefineRP = 1000 },
	{ StartRP = 1001, EndRP = 1100, DefineRP = 1100 },
	{ StartRP = 1101, EndRP = 1200, DefineRP = 1200 },
	{ StartRP = 1201, EndRP = 1300, DefineRP = 1300 },
	{ StartRP = 1301, EndRP = 1400, DefineRP = 1400 },
	{ StartRP = 1401, EndRP = 1500, DefineRP = 1500 },
	{ StartRP = 1501, EndRP = 1600, DefineRP = 1600 },
	{ StartRP = 1601, EndRP = 1700, DefineRP = 1700 },
	{ StartRP = 1701, EndRP = 1800, DefineRP = 1800 },
	{ StartRP = 1801, EndRP = 1900, DefineRP = 1900 },
	{ StartRP = 1901, EndRP = 2000, DefineRP = 2000 },
	{ StartRP = 2001, EndRP = 2100, DefineRP = 2100 },
	{ StartRP = 2101, EndRP = 2200, DefineRP = 2200 },
	{ StartRP = 2201, EndRP = 2300, DefineRP = 2300 },
	{ StartRP = 2301, EndRP = 2400, DefineRP = 2400 },
	{ StartRP = 2401, EndRP = 2500, DefineRP = 2500 },
	{ StartRP = 2501, EndRP = 2600, DefineRP = 2600 },
	{ StartRP = 2601, EndRP = 2700, DefineRP = 2700 },
	{ StartRP = 2701, EndRP = 2800, DefineRP = 2800 },
	{ StartRP = 2801, EndRP = 2900, DefineRP = 2900 },
	{ StartRP = 2901, EndRP = 3000, DefineRP = 3000 },	
}
--]]

-- 오픈 시간 설정
ELOTimeTable = 
{
	bEnable = TRUE,
	ELOSetOpenTime = 
	{
		-- Time = { Hour, Min }
		{ DayIndex = 0, BeginTime = { 09, 00 }, EndTime = { 23, 00 }, }, -- 일요일
		{ DayIndex = 1, BeginTime = { 15, 20 }, EndTime = { 23, 00 }, }, -- 월요일
		{ DayIndex = 2, BeginTime = { 15, 00 }, EndTime = { 23, 00 }, }, -- 화요일
		{ DayIndex = 3, BeginTime = { 15, 00 }, EndTime = { 23, 50 }, }, -- 수요일
		{ DayIndex = 4, BeginTime = { 09, 00 }, EndTime = { 23, 00 }, }, -- 목요일
		{ DayIndex = 5, BeginTime = { 15, 00 }, EndTime = { 23, 00 }, }, -- 금요일
		{ DayIndex = 6, BeginTime = { 22, 00 }, EndTime = { 23, 00 }, }, -- 토요일
	},
}

-- 모드별 최대값, 최소값 설정
-- 0 : 1vs1
-- 1 : 2vs2
-- 2 : 3vs3
ModeMaxMinInfo = 
{
	{ ModeInfo = 0, MinCount = 2, MaxCount = 8 },
	{ ModeInfo = 1, MinCount = 2, MaxCount = 8 },
	{ ModeInfo = 2, MinCount = 2, MaxCount = 8 },
}

-- 모드별 on/off 기능
ModeOpenEnable = 
{
	{ ModeInfo = 0, OpenEnable = TRUE },
	{ ModeInfo = 1, OpenEnable = TRUE },	
	{ ModeInfo = 2, OpenEnable = TRUE },
}

-- 알람 기능.
-- 현재 영웅던전의 알람 기능과 중복되기 때문에 5분전에 한번만 사용하도록 설정하는 것이 좋을듯 함.
-- 현재 기능은 몇분전 세팅 가능하도록 확장성 있는 구조로 만듬.
MatchOpenAlarm = { 15, 10, 5 }
MatchCloseAlarm = { 10, 5 }