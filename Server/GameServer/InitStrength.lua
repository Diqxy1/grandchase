TRUE = 1
FALSE = 0

ENCHANT = 0 -- 강화
BREAKUP = 1 -- 해체
COMPOSE = 2 -- 합성

-- 합성 테이블 정의 변수.
ComposeResultTypeA = 0 -- 합성 결과물 Type A
ComposeResultTypeB = 1 -- 합성 결과물 Type B
ComposeResultTypeC = 2 -- 합성 결과물 Type C
ComposeResultTypeD = 3 -- 합성 결과물 Type D
ComposeResultTypeE = 4 -- 합성 결과물 Type E
ComposeResultTypeF = 5 -- 합성 결과물 Type F

NORMALNORMAL = 0 -- 일반강화장비 + 일반강화장비
NORMALEPIC = 1   -- 일반강화장비 + 에픽강화장비
EPICEPIC = 2     -- 에픽강화장비 + 에픽강화장비

-- 강화 결과 확률
SUCCESS      = 1 -- 성공
WEAKNESS     = 2 -- 하락
BREAK        = 3 -- 깨짐
NONE         = 4 -- 제자리
DROP         = 5 -- 초기화
UPGRADE		 = 6 -- 에픽 강화석으로 업그레이드

GRADE_NORMAL   = 0 -- 일반 강화석
GRADE_RARE     = 1
GRADE_EPIC     = 2 -- 에픽 강화석
GRADE_LEGEND   = 3

-- 강화 DB 마이그레이션 여부
MigrationEnable = FALSE

-- 공지 출력 시간 설정
NoticeTimeGap = 15000 -- 15초

-- 강화석의 스킬 갯수 설정( 추가 확장성을 고려하여 등급별 스킬 갯수를 지정하게 수정 )
StrengthGradeSkillInfo = {
	{ GRADE_NORMAL, 0 },
	{ GRADE_EPIC, 1 },
}

InitStrength = { -- { 강화Type, 레벨, 확률{ 성공, 하락, 깨짐, 제자리, 초기화, 에픽업그레이드 }
	StrengthItem = 627270, -- 일반 강화석 ItemID
	MaxStrengthLevel = 35,
	-- Normal
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 1, StrengthRatio = { { SUCCESS, 100 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 0 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 2, StrengthRatio = { { SUCCESS, 100 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 0 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 3, StrengthRatio = { { SUCCESS, 100 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 0 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 4, StrengthRatio = { { SUCCESS, 50 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 50 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 5, StrengthRatio = { { SUCCESS, 50 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 50 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 6, StrengthRatio = { { SUCCESS, 40 },{ WEAKNESS, 31 }, { BREAK, 0 }, { NONE, 29 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 7, StrengthRatio = { { SUCCESS, 40 },{ WEAKNESS, 31 }, { BREAK, 0 }, { NONE, 29 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 8, StrengthRatio = { { SUCCESS, 30 },{ WEAKNESS, 21 }, { BREAK, 0 }, { NONE, 19 }, { DROP, 20 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 9, StrengthRatio = { { SUCCESS, 30 },{ WEAKNESS, 31 }, { BREAK, 0 }, { NONE, 19 }, { DROP, 20 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 10, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 11, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 12, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 13, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 14, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 15, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 16, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 17, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 18, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 19, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 20, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 21, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 22, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 23, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 24, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 25, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 26, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 27, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 28, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 29, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 30, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 31, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 32, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 33, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 34, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_NORMAL, StrengthLevel = 35, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	-- Epic
	{ StrengthType = GRADE_EPIC, StrengthLevel = 1, StrengthRatio = { { SUCCESS, 100 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 0 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 2, StrengthRatio = { { SUCCESS, 100 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 0 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 3, StrengthRatio = { { SUCCESS, 100 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 0 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 4, StrengthRatio = { { SUCCESS, 50 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 50 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 5, StrengthRatio = { { SUCCESS, 50 },{ WEAKNESS, 0 }, { BREAK, 0 }, { NONE, 50 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 6, StrengthRatio = { { SUCCESS, 40 },{ WEAKNESS, 31 }, { BREAK, 0 }, { NONE, 29 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 7, StrengthRatio = { { SUCCESS, 40 },{ WEAKNESS, 31 }, { BREAK, 0 }, { NONE, 29 }, { DROP, 0 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 8, StrengthRatio = { { SUCCESS, 30 },{ WEAKNESS, 21 }, { BREAK, 0 }, { NONE, 19 }, { DROP, 20 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 9, StrengthRatio = { { SUCCESS, 30 },{ WEAKNESS, 31 }, { BREAK, 0 }, { NONE, 19 }, { DROP, 20 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 10, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 11, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 12, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 13, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 14, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 15, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 16, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 17, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 18, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 19, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 20, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 21, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 22, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 23, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 24, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 25, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 26, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 27, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 28, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 29, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 30, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 31, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 32, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 33, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 34, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
	{ StrengthType = GRADE_EPIC, StrengthLevel = 35, StrengthRatio = { { SUCCESS, 20 },{ WEAKNESS, 30 }, { BREAK, 25 }, { NONE, 0 }, { DROP, 25 }, { UPGRADE, 0 } } },
}

--강화 재료 아이템 정보.
StrengthMaterialItem = {
	DefaultStrengthMaterial = 8880, -- 기본 강화 재료( GEM )
	DefaultStrengthValue = 2, -- 기본 강화 재료 갯수 배율( 강화 Gem 비용 = 강화 단계 * DefaultStrengthValue )
	NormalStrengthMinLevel = 20, -- 일반 강화석 강화시 재료 최소 레벨.
	EpicStrengthMinLevel = 50,   -- 에픽 강화석 강화시 재료 최소 레벨.
	{
		GRADE = GRADE_NORMAL,
		LevelInfo = {
			{ ItemLevelRange = { 1, 20 }, AddSuccessRatio = 0 },
			{ ItemLevelRange = { 21, 40 }, AddSuccessRatio = 0 },
			{ ItemLevelRange = { 41, 60 }, AddSuccessRatio = 0 },
			{ ItemLevelRange = { 61, 80 }, AddSuccessRatio = 0 },
		},
	},
	{ 
		GRADE = GRADE_RARE, 
		LevelInfo = {
			{ ItemLevelRange = { 1, 20 }, AddSuccessRatio = 0 },
			{ ItemLevelRange = { 21, 40 }, AddSuccessRatio = 0.5 },
			{ ItemLevelRange = { 41, 60 }, AddSuccessRatio = 1 },
			{ ItemLevelRange = { 61, 80 }, AddSuccessRatio = 1.5 },
		},
	},
	{
		GRADE = GRADE_EPIC,
		LevelInfo = {
			{ ItemLevelRange = { 1, 20 }, AddSuccessRatio = 0 },
			{ ItemLevelRange = { 21, 40 }, AddSuccessRatio = 1 },
			{ ItemLevelRange = { 41, 60 }, AddSuccessRatio = 2 },
			{ ItemLevelRange = { 61, 80 }, AddSuccessRatio = 3 },
		},
	},
	{
		GRADE = GRADE_LEGEND,
		LevelInfo = {
			{ ItemLevelRange = { 1, 20 }, AddSuccessRatio = 0 },
			{ ItemLevelRange = { 21, 40 }, AddSuccessRatio = 1.5 },
			{ ItemLevelRange = { 41, 60 }, AddSuccessRatio = 3 },
			{ ItemLevelRange = { 61, 80 }, AddSuccessRatio = 4.5 },
		},
	},
}

-- 강화 보조 아이템 리스트
StrengthUpgradeAssist = { -- 강화률 상승 아이템 { ItemID, 성공확률 }
	{ 627290, 5 },
	{ 627300, 10 },
}

StrengthProtection = {  -- 강화 보호 주문서
	-- { ItemID, { StartLv, EndLv }
	{ ItemID = 627310, LevelRange = { 1, 10 }, },
}
SpecificLevelStrength = { -- 특정레벨 강화 변화 주문서 { ItemID, 특정강화레벨 }
	{ 627360, 7 },
	{ 627370, 8 },
	{ 627380, 9 },
	{ 627390, 10 },
}

RandomLevelStrength = { -- 랜덤 레벨 강화 부적( 사용안함 )
	{
		ItemID = 627400,  -- 특정 Level ItemID
		LevelRatio = {  -- { Level, Ratio }
			{ 3, 20 },
			{ 4, 20 },
			{ 5, 30 },
			{ 6, 30 },
		},
	},
}

-- 해체 재료 아이템 리스트.
BreakUpItem = { -- { ItemID, 성공 확률, 강화석 파괴여부 }
	{ ItemID = 627320, BreakUpRatio = { { SUCCESS, 30 }, { BREAK, 70 } } }, -- 일반 해체 주문서
	{ ItemID = 627330, BreakUpRatio = { { SUCCESS, 30 }, { NONE, 70 } } }, -- 특수 해체 주문서
}

-- 합성 아이템 리스트.
-- 결과물 Type에 대한 정의
-- 해당 계산 공식은 서버 코드 Level에 정의되어 있다. 스크립트에 정의된 내용은 코드 Level에 구현된 부분의 정의
-- ComposeResultTypeA = { 종류 : 일반 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 1.5 )
-- ComposeResultTypeB = { 종류 : 일반 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 2.0 )
-- ComposeResultTypeC = { 종류 : 일반 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 3.0 )
-- ComposeResultTypeD = { 종류 : 에픽 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 1.5 )
-- ComposeResultTypeE = { 종류 : 에픽 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 2.0 )
-- ComposeResultTypeF = { 종류 : 에픽 강화석, 강화 : ( A장비 강화 레벨 + B장비 강화 레벨 )/ 3.0 )
ComposeTypeDefine = {
	{ ItemCompose = { GRADE_NORMAL, GRADE_NORMAL }, ComposeType = NORMALNORMAL },
	{ ItemCompose = { GRADE_NORMAL, GRADE_EPIC }, ComposeType = NORMALEPIC },
	{ ItemCompose = { GRADE_EPIC, GRADE_EPIC }, ComposeType = EPICEPIC },
	{ ItemCompose = { GRADE_EPIC, GRADE_NORMAL }, ComposeType = NORMALEPIC },
}

ComposeResultRatio = { -- 합성결과 Type에 따른 결과 { ResultType, { 강화석 종류, Ratio } }
	{ ResultType = ComposeResultTypeA, ResultRatio = { GRADE_NORMAL, 1.4 }, },
	{ ResultType = ComposeResultTypeB, ResultRatio = { GRADE_NORMAL, 2.4 }, },
	{ ResultType = ComposeResultTypeC, ResultRatio = { GRADE_NORMAL, 1.7 }, },
	{ ResultType = ComposeResultTypeD, ResultRatio = { GRADE_EPIC, 1.4 }, },
	{ ResultType = ComposeResultTypeE, ResultRatio = { GRADE_EPIC, 2.4 }, },
	{ ResultType = ComposeResultTypeF, ResultRatio = { GRADE_EPIC, 1.7 }, },
}

ComposeItem = {
	{
		ComposeType = NORMALNORMAL, -- Key{ ComposeType, ComposeItemID }
		ComposeItemID = 627340, -- 일반 합성 주문서.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeC, 40 },
			{ ComposeResultTypeB, 50 },
			{ ComposeResultTypeE, 10 },
		},
	},
	{
		ComposeType = NORMALEPIC,
		ComposeItemID = 627340, -- 일반 합성 주문서.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeC, 20 },
			{ ComposeResultTypeB, 50 },
			{ ComposeResultTypeF, 10 },
			{ ComposeResultTypeE, 20 },
		},
	},
	{
		ComposeType = EPICEPIC,
		ComposeItemID = 627340, -- 일반 합성 주문서.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeF, 40 },
			{ ComposeResultTypeE, 50 },
			{ ComposeResultTypeD, 10 },
		},
	},
	{
		ComposeType = NORMALNORMAL,
		ComposeItemID = 627350, -- 에픽 합성 주문서.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeA, 40 },
			{ ComposeResultTypeC, 50 },
			{ ComposeResultTypeF, 10 },
		},
	},
	{
		ComposeType = NORMALEPIC,
		ComposeItemID = 627350, -- 에픽 합성 주문서.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeA, 20 },
			{ ComposeResultTypeC, 50 },
			{ ComposeResultTypeD, 10 },
			{ ComposeResultTypeF, 20 },
		},
	},
	{
		ComposeType = EPICEPIC,
		ComposeItemID = 627350, -- 에픽 합성 주문서.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeF, 45 },
			{ ComposeResultTypeE, 40 },
			{ ComposeResultTypeD, 15 },
		},
	},
}

-- 아이템 장착 Type
EISP_HELMET = 1 -- 헬멧
EISP_JACKET = 2 -- 자켓
EISP_PANTS  = 3 -- 바지
EISP_GLOVES = 4 -- 장갑
EISP_SHOES	= 5 -- 신발
EISP_MANTLE = 6 -- 망토
EISP_WEAPON	= 7 -- 무기

-- 강화 속성 Type
ATTRTYPE_CRITICAL_DAMAGE_RESIST = 27 -- 크리티컬데미지 저항력 - 상대에게서 크리티컬 데미지를 받을 때 효과만큼 데미지를 감소시킨다
ATTRTYPE_CRITICAL_DAMAGE_UP = 28     -- 크리티컬데미지 증가 
ATTRTYPE_CATCH_DAMAGE_RESIST =29	 -- 잡기 데미지 저항력 
ATTRTYPE_RECOVERY_UP_PET_MP = 30	 -- 펫 MP회복속도 증가 
ATTRTYPE_MOVEMENT_SPEED_UP = 31		 -- 이동속도 증가( 달리기, 걷기 )
ATTRTYPE_RECOVERY_UP_POTION = 32	 -- 회복포션 효과 증가
ATTRTYPE_FATAL_SUPER_ARMOR_BUFF = 33 -- Fatal시 슈퍼아머발동 버프
ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF = 34 -- Fatat시 MP회복 속도 상승 버프
ATTRTYPE_FATAL_ATTACK_UP_BUFF = 35		-- Fatat시 공격력 상승 버프
ATTRTYPE_FATAL_SPEEP_UP_BUFF = 36		-- Fatat시 이동속도 상승 버프
ATTRTYPE_FATAL_RECOVERY_POTION_UP = 37	-- Fatat시 회복포션 효과 상승
ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF = 38 -- Fatat시 크리티컬 데미지 상승 버프
ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP = 39 -- 강화확률 상승
ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST = 40 -- 강화파괴 방지

-- 강화석 속성.
StrengthAttributeTable = {
	{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_HELMET,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP, 20,5 },
			{ ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP, 10,45},
			{ ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST, 20,5 },
			{ ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST, 10,40 },
		},
	},
		{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_JACKET,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_CRITICAL_DAMAGE_RESIST, 20,5 },
			{ ATTRTYPE_CRITICAL_DAMAGE_RESIST, 15,35},
			{ ATTRTYPE_FATAL_SUPER_ARMOR_BUFF, 20,60 },
		},
	},
		{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_PANTS,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_CATCH_DAMAGE_RESIST, 20,5 },
			{ ATTRTYPE_CATCH_DAMAGE_RESIST, 15,35},
			{ ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF, 20,30 },
			{ ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF, 15,30 },
		},
	},
		{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_GLOVES,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_RECOVERY_UP_PET_MP, 10,5 },
			{ ATTRTYPE_RECOVERY_UP_PET_MP, 7,35},
			{ ATTRTYPE_FATAL_ATTACK_UP_BUFF, 20,30 },
			{ ATTRTYPE_FATAL_ATTACK_UP_BUFF, 15,30 },
		},
	},
		{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_SHOES,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_MOVEMENT_SPEED_UP, 10,5 },
			{ ATTRTYPE_MOVEMENT_SPEED_UP, 7,35},
			{ ATTRTYPE_FATAL_SPEEP_UP_BUFF, 15,30 },
			{ ATTRTYPE_FATAL_SPEEP_UP_BUFF, 10,30 },
		},
	},
		{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_MANTLE,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_RECOVERY_UP_POTION, 20,5 },
			{ ATTRTYPE_RECOVERY_UP_POTION, 10,35},
			{ ATTRTYPE_FATAL_RECOVERY_POTION_UP, 100,30 },
			{ ATTRTYPE_FATAL_RECOVERY_POTION_UP, 50,30 },
		},
	},
		{
		StrengthGrade = GRADE_EPIC,
		EquipItemType = EISP_WEAPON,
		RandomCount = 1,
		AttributeInfo = {	
			{ ATTRTYPE_CRITICAL_DAMAGE_UP, 10,5 },
			{ ATTRTYPE_CRITICAL_DAMAGE_UP, 7,35},
			{ ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF, 20,30 },
			{ ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF, 15,30 },
		},
	},
}

-- 강화 관련 판매 아이템 리스트
StrengthShopItemList = { 642630, 642650, 642670, 627350, }

-- 강화 공지 출력 여부
StrengthNotice = {
	NoticeEnable = TRUE,
	NoticeMinStrengthLevel = 10,
}

-- 강화석 장착 못하는 아이템 리스트.
NonStrengthItemList = { } -- { ItemID, }

StrengthUpgradeProtectionAssist = { -- ( 강화률 상승 + 하락 방지 ) 주문서
	{
		ItemID = 762780,		-- ItemID
		fRatio = 100,           -- 강화 성공 증가 확률
		LevelRange = { 15, 17 }, -- 사용가능 레벨
	},
}

-- 강화에 필요한 GP가격
StrengthGPCost = {
	-- { Level, 무기GP, 방어구GP },
	{ 1, 1000, 500 },
	{ 2, 1000, 500 },
	{ 3, 2000, 1000 },
	{ 4, 2000, 1000 },
	{ 5, 2000, 1000 },
	{ 6, 4000, 2000 },
	{ 7, 5000, 2500 },
}