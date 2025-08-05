--게임서버 매칭 로직 스크립트

-- ELO Rating 상수 K Table
ConstantKTable = 
{
	-- RA 범위( 시작, 끝 ), 상수 K값
	{ RaStartValue = 0, RaEndValue = 2000, ConstantK = 32 },
	{ RaStartValue = 2001, RaEndValue = 10000, ConstantK = 24 },
}

-- 배치고사 진입 RP Table
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