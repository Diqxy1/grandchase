-- ���� Type
BERMESIAH = 1   -- �����޽þ�
SILVER_LAND = 2 -- �ǹ�����
ELLIA = 3       -- ������ 
ATUM = 4        -- ����

-- ���� ����
DIE = 0         -- ���� ���� ����
NORMAL_TREE = 1 -- �Ϲ� ���� ����
GOLD_TREE = 2   -- Ȳ�� ���� ����

-- ���� Type
NORMAL_FRUIT = 0 -- �Ϲ� ���� ����
GOLD_FRUIT = 1   -- Ȳ�� ���� ����
CONTINENT_REWARD = 2 -- ��� ����

PLANTTREE_VERSION = 0   	-- ���� ������ ������ ������ 1�� �÷��� �Ѵ�. ������ �ٲٰ� ��ġ�ϴ� ��쿡�� +1 �ؾ��Ѵ�.
PLANTTREE_ITEM = 44440		-- �����ɱ��� ItemID
GOLDTREE_ITEM = 12490  		-- Ȳ�� ���� ItemID
WATER_ITEM = 8880     		-- ���ֱ� ItemID
MANURE_ITEM = 58630 		-- �Ÿ��ֱ� ItemID
MAX_CONTINENT = 4			-- ������� �ְ�

BeginTime   = { 2011, 1, 3, 21 }   -- Y, M, D, H
EndTime     = { 2011, 3, 20, 01 }   -- Y, M, D, H

-- �������� �̺�Ʈ ����.
PLANTTREE_INFO = 
{
	{
		CONTINENT_TYPE = BERMESIAH,	 -- ��� Type.
		TOTAL_TREE_COUNT = 5,		 -- ��� ��ü ���� ����.
		NEED_WATER_COUNT = 4,		 -- ���� �α���� ���ֱ� Count.
		REWARD_LIST = {				 -- ���� ������ ����Ʈ.
			{ TYPE = NORMAL_FRUIT, REWARD = { 8880, 1, -1, }, }, -- �Ϲ� ���� ���� ���� { ItemID, Count, Period, }
			{ TYPE = GOLD_FRUIT, REWARD = { 58720, 1, -1, }, },   -- Ȳ�� ���� ���� ����
			{ TYPE = CONTINENT_REWARD, REWARD = { 352880, 1, -1, }, }, -- ��� ����
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

-- �Ÿ� Ȯ�� ����
MANURE_RATIO = 
{
	{ DIE, 30 },         -- ���� ���� ����
	{ NORMAL_TREE, 40 }, -- �Ϲ� ���� ����
	{ GOLD_TREE, 30 },   -- Ȳ�� ���� ����
}

-- ���� ���°� List
PLANTTREE_STATUS = { 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 100, 200, 300, 400, 500, }