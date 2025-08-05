TRUE = 1
FALSE = 0

-- 카드 등급
GRADE_NORMAL   = 0
GRADE_RARE     = 1
GRADE_EPIC     = 2
GRADE_LEGEND   = 3



-- 몬스터카드 조합 사용여부
MonsterCardMixEnable = FALSE

MinMaterialCount = 2 -- 최소 재료수
MaxMaterialCount = 7 -- 최대 재료수

-- 카드 등급별 포인트
Point = {
	-- 카드등급, 점수
	{ GRADE_NORMAL, 1 },
	{ GRADE_RARE, 2 },
	{ GRADE_EPIC, 3 },
	{ GRADE_LEGEND, 5 },
}

--획득 카드 그룹 확률
PointRewardRatio = {
	-- pointRange = {민값, 맥스값}, RewardGrop = {그룹1,확률},{그룹2,확률}
	{ PointRange = { 2, 10 }, RewardGroup = {{ 1, 50 }, { 2, 50 },}, },
	{ PointRange = { 11, 20 }, RewardGroup = {{ 1, 30 }, { 2, 40}, { 3, 30 },}, },
	{ PointRange = { 21, 30 }, RewardGroup = {{ 2, 30 }, { 3, 40}, { 4, 30 },}, },
}

--그룹별 카드 구성 ( GroupNum = 그룹넘버 , { 카드아이디, 확률, 카드아이디, 확률 }
RewardGroup = {
	{ GroupNum = 1, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 2, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 3, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 4, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 5, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
}
