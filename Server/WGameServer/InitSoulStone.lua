FALSE = 0
TRUE = 1

-- 소울스톤

-- 각 스톤 ItemID가지고 있기.
-- 1단 속성별 스톤 소모 갯수 지정.
-- 1,2,3단별 속성별 확률 지정.

-- 소울스톤 속성
FIRE = 0
ICE = 1
EARTH = 2
WIND = 3

SoulStoneUseEnable = TRUE

-- 소울스톤 ItemID
-- 속성 추가시에 코드 작업 하지 않기 위해서 서버코드에서 확장성있게 테이블 가지고 있음.
SoulStoneItemInfo = 
{
	{ SoulStoneType = FIRE, ItemID = 565660 },
	{ SoulStoneType = ICE,  ItemID = 565670 },
	{ SoulStoneType = EARTH,  ItemID = 565680 },
	{ SoulStoneType = WIND,  ItemID = 565690 },	
}

SoulStoneUseCountInfo = 
{
	{ SoulStoneType = FIRE, Grade = 1, UseCount = 1 },
	{ SoulStoneType = FIRE, Grade = 2, UseCount = 2 },
	{ SoulStoneType = FIRE, Grade = 3, UseCount = 3 },
	{ SoulStoneType = FIRE, Grade = 4, UseCount = 4 },

	{ SoulStoneType = ICE, Grade = 1, UseCount = 1 },
	{ SoulStoneType = ICE, Grade = 2, UseCount = 2 },
	{ SoulStoneType = ICE, Grade = 3, UseCount = 3 },
	{ SoulStoneType = ICE, Grade = 4, UseCount = 4 },
	
	{ SoulStoneType = EARTH, Grade = 1, UseCount = 1 },
	{ SoulStoneType = EARTH, Grade = 2, UseCount = 2 },
	{ SoulStoneType = EARTH, Grade = 3, UseCount = 3 },
	{ SoulStoneType = EARTH, Grade = 4, UseCount = 4 },

	{ SoulStoneType = WIND, Grade = 1, UseCount = 1 },
	{ SoulStoneType = WIND, Grade = 2, UseCount = 2 },
	{ SoulStoneType = WIND, Grade = 3, UseCount = 3 },
	{ SoulStoneType = WIND, Grade = 4, UseCount = 4 },
}

SoulStoneRatio = 
{
	-- Grade, Ratio[ {Damage, Ratio} ] 확률은 항상 100%를 맞추어야 한다.
	{ SoulStoneType = FIRE, Grade = 1, GradeRatio = { {10, 20}, {20, 20}, {30, 20}, {40, 20}, {50, 20}, }, },
	{ SoulStoneType = FIRE, Grade = 2, GradeRatio = { {20, 20}, {30, 20}, {40, 20}, {50, 20}, {60, 20}, }, },
	{ SoulStoneType = FIRE, Grade = 3, GradeRatio = { {30, 20}, {40, 20}, {50, 20}, {60, 20}, {70, 20}, }, },
	{ SoulStoneType = FIRE, Grade = 4, GradeRatio = { {40, 20}, {50, 20}, {60, 20}, {70, 20}, {80, 20}, }, },
		
	-- Grade, Ratio[ {Damage, Ratio} ] 확률은 항상 100%를 맞추어야 한다.
	{ SoulStoneType = ICE, Grade = 1, GradeRatio = { {10, 20}, {20, 20}, {30, 20}, {40, 20}, {50, 20}, }, },
	{ SoulStoneType = ICE, Grade = 2, GradeRatio = { {20, 20}, {30, 20}, {40, 20}, {50, 20}, {60, 20}, }, },
	{ SoulStoneType = ICE, Grade = 3, GradeRatio = { {30, 20}, {40, 20}, {50, 20}, {60, 20}, {70, 20}, }, },
	{ SoulStoneType = ICE, Grade = 4, GradeRatio = { {40, 20}, {50, 20}, {60, 20}, {70, 20}, {80, 20}, }, },
	
	{ SoulStoneType = EARTH, Grade = 1, GradeRatio = { {10, 20}, {20, 20}, {30, 20}, {40, 20}, {50, 20}, }, },
	{ SoulStoneType = EARTH, Grade = 2, GradeRatio = { {20, 20}, {30, 20}, {40, 20}, {50, 20}, {60, 20}, }, },
	{ SoulStoneType = EARTH, Grade = 3, GradeRatio = { {30, 20}, {40, 20}, {50, 20}, {60, 20}, {70, 20}, }, },
	{ SoulStoneType = EARTH, Grade = 4, GradeRatio = { {40, 20}, {50, 20}, {60, 20}, {70, 20}, {80, 20}, }, },

	{ SoulStoneType = WIND, Grade = 1, GradeRatio = { {10, 20}, {20, 20}, {30, 20}, {40, 20}, {50, 20}, }, },
	{ SoulStoneType = WIND, Grade = 2, GradeRatio = { {20, 20}, {30, 20}, {40, 20}, {50, 20}, {60, 20}, }, },
	{ SoulStoneType = WIND, Grade = 3, GradeRatio = { {30, 20}, {40, 20}, {50, 20}, {60, 20}, {70, 20}, }, },
	{ SoulStoneType = WIND, Grade = 4, GradeRatio = { {40, 20}, {50, 20}, {60, 20}, {70, 20}, {80, 20}, }, },
}