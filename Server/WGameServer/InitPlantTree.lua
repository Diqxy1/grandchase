-- 던전 Type
BERMESIAH = 1   -- 베르메시아
SILVER_LAND = 2 -- 실버랜드
ELLIA = 3       -- 엘리아 
ATUM = 4        -- 아툼

-- 나무 상태
DIE = 0         -- 죽은 나무 상태
NORMAL_TREE = 1 -- 일반 나무 상태
GOLD_TREE = 2   -- 황금 나무 상태

-- 보상 Type
NORMAL_FRUIT = 0 -- 일반 나무 열매
GOLD_FRUIT = 1   -- 황금 나무 열매
CONTINENT_REWARD = 2 -- 대륙 보상

PLANTTREE_VERSION = 0   	-- 새로 진행할 때마다 버전을 1씩 올려야 한다. 보상을 바꾸고 패치하는 경우에도 +1 해야한다.
PLANTTREE_ITEM = 44440		-- 나무심기판 ItemID
GOLDTREE_ITEM = 12490  		-- 황금 나무 ItemID
WATER_ITEM = 8880     		-- 물주기 ItemID
MANURE_ITEM = 58630 		-- 거름주기 ItemID
MAX_CONTINENT = 4			-- 대륙정보 최고값

BeginTime   = { 2011, 1, 3, 21 }   -- Y, M, D, H
EndTime     = { 2011, 3, 20, 01 }   -- Y, M, D, H

-- 나무나무 이벤트 정보.
PLANTTREE_INFO = 
{
	{
		CONTINENT_TYPE = BERMESIAH,	 -- 대륙 Type.
		TOTAL_TREE_COUNT = 5,		 -- 대륙 전체 나무 갯수.
		NEED_WATER_COUNT = 4,		 -- 열매 맺기까지 물주기 Count.
		REWARD_LIST = {				 -- 보상 아이템 리스트.
			{ TYPE = NORMAL_FRUIT, REWARD = { 8880, 1, -1, }, }, -- 일반 나무 열매 보상 { ItemID, Count, Period, }
			{ TYPE = GOLD_FRUIT, REWARD = { 58720, 1, -1, }, },   -- 황금 나무 열매 보상
			{ TYPE = CONTINENT_REWARD, REWARD = { 352880, 1, -1, }, }, -- 대륙 보상
		},
	},
	{
		CONTINENT_TYPE = SILVER_LAND,
		TOTAL_TREE_COUNT = 10,
		NEED_WATER_COUNT = 5,
		REWARD_LIST = {
			TYPE = NORMAL_FRUIT, REWARD = { 8880, 2, -1, },
			TYPE = GOLD_FRUIT, REWARD = { 58720, 2, -1, },
			TYPE = CONTINENT_REWARD, REWARD = { 352880, 2, -1, },
		},
	},
	{
		CONTINENT_TYPE = ELLIA,
		TOTAL_TREE_COUNT = 15,
		NEED_WATER_COUNT = 7,
			REWARD_LIST = {
			TYPE = NORMAL_FRUIT, REWARD = { 8880, 3, -1,},
			TYPE = GOLD_FRUIT, REWARD = { 58720, 3, -1, },
			TYPE = CONTINENT_REWARD, REWARD = { 352880, 3, -1, },
		},
	},
	{
		CONTINENT_TYPE = ATUM, 
		TOTAL_TREE_COUNT = 20,		
		NEED_WATER_COUNT = 10,
		REWARD_LIST = {				 
			TYPE = NORMAL_FRUIT, REWARD = { 8880, 4, -1, }, 
			TYPE = GOLD_FRUIT, REWARD = { 58720, 4, -1, },
			TYPE = CONTINENT_REWARD, REWARD = { 352880, 4, -1, },
		},
	},
}

-- 거름 확률 설정
MANURE_RATIO = 
{
	{ DIE, 30 },         -- 죽은 나무 상태
	{ NORMAL_TREE, 40 }, -- 일반 나무 상태
	{ GOLD_TREE, 30 },   -- 황금 나무 상태
}

-- 나무 상태값 List
PLANTTREE_STATUS = { 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 100, 200, 300, 400, 500, }