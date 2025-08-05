-- ������ ������ ������ �ش� �������� ���ð����ϴ�.
-- ��ǥ������ �ִµ� ��ǥ ������ �Ѿ�� ��ǥ���� �� ���� Goal�� �������� ��������� �����Ѵ�.
-- BoardType Enum
P_NORMAL            = 0            -- �Ϲ�ĭ
P_ACCESSARIES       = 1            -- �Ǽ��縮 ����ĭ.
P_POSITION          = 2            -- ���� ����ĭ
P_GEM               = 3            -- GEM ĭ
P_AGINGCORE         = 4            -- ���� ĭ
P_CHAMPION          = 5            -- è�Ǿ� ����� ĭ
P_CONTINENT1        = 6            -- ����� ����ĭ1
P_CONTINENT2        = 7            -- ����� ����ĭ2
P_CONTINENT3        = 8            -- ����� ����ĭ3
P_CONTINENT4        = 9            --����� ����ĭ4
P_SPECIALMISSION    = 10           -- Ư�� �̼�ĭ

BeginTime   = { 2009, 10, 06, 09 }   -- Y, M, D, H
EndTime     = { 2009, 10, 28, 09 }   -- Y, M, D, H
TICK_SYNC_GAP = 1000 * 60 * 5
TOTAL_MAP_COUNT = 24			-- ��ü ����������� ĭ ��( ������ ĭ�� Num���� �������� 28�� 27�� ǥ�� )
MAX_GOAX = 30				-- �ִ� ��ǥ ������
DAILY_MAX = 8				-- �Ϸ翡 ���� �� �ִ� �ִ� �ֻ��� ������ Ƚ��.
DAILY_FREE = 5				-- �Ϸ翡 ����� ������ �ֻ��� ����.
FREE_TICK_GAP = 60 * 60 		-- 1�ð�.
UPDATE_HOUR = 6				-- �ʱ�ȭ�� ������ �ð�.
DICE_ITEMID = 203210			-- �ֻ��� ItemID
BOARD_ITEMID = 203200       		-- ���� ������.
BOARD_VERSION = 6			-- �ֻ��� �̺�Ʈ ����.( �̺�Ʈ ����ɶ����� ������ 1�� �÷��� �Ѵ� )

SpecialMission = { 204160, 204170, 204180, }		-- ������ �̼� ID( 1�� �̻��� �̼��� �����ؾ� �Ѵ�. )

InitPositionValue = 
{
	{
		TYPE = P_NORMAL,
		ItemList = { },
	},
	{
		TYPE = P_ACCESSARIES,
		ItemList = { 111080, 1, -1, },
	},
	{
		TYPE = P_POSITION,
		ItemList = { 177560, 2, -1, },
	},	
	{
		TYPE = P_GEM,
		ItemList = { 203220, 1, -1, },
	},
	{
		TYPE = P_AGINGCORE,
		ItemList = { 203230, 1, -1, },
	},	
	{
		TYPE = P_CHAMPION,
		ItemList = { 82050, 1, -1, },
	},	
	{
		TYPE = P_CONTINENT1,
		ItemList = { 77710, 1, -1, },
	},	
	{
		TYPE = P_CONTINENT2,
		ItemList = { },
	},	
	{
		TYPE = P_CONTINENT3,
		ItemList = { 77720, 1, -1, },
	},	
	{
		TYPE = P_CONTINENT4,
		ItemList = { 77730, 1, -1, },
	},	
}

InitBoardGameInfo = 
{
	{
		MapID = 0,
		Properties = 0,
	},
	{
		MapID = 1,
		Properties = 1,
	},
	{
		MapID = 2,
		Properties = 4,
	},
	{
		MapID = 3,
		Properties = 2,
	},	
	{
		MapID = 4,
		Properties = 5,
	},	
	{
		MapID = 5,
		Properties = 2,
	},	
	{
		MapID = 6,
		Properties = 10,
	},	
	{
		MapID = 7,
		Properties = 1,
	},	
	{
		MapID = 8,
		Properties = 5,
	},	
	{
		MapID = 9,
		Properties = 6,
	},	
	{
		MapID = 10,
		Properties = 2,
	},	
	{
		MapID = 11,
		Properties = 4,
	},	
	{
		MapID = 12,
		Properties = 10,
	},	
	{
		MapID = 13,
		Properties = 5,
	},	
	{
		MapID = 14,
		Properties = 2,
	},	
	{
		MapID = 15,
		Properties = 7,
	},	
	{
		MapID = 16,
		Properties = 1,
	},	
	{
		MapID = 17,
		Properties = 4,
	},	
	{
		MapID = 18,
		Properties = 10,
	},	
	{
		MapID = 19,
		Properties = 4,
	},	
	{
		MapID = 20,
		Properties = 1,
	},	
	{
		MapID = 21,
		Properties = 8,
	},	
	{
		MapID = 22,
		Properties = 2,
	},	
	{
		MapID = 23,
		Properties = 5,
	},
}


InitGoalReward = 
{
	{
		GOAL = 1,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 2,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 3,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 4,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 5,
		ItemList = { 102030, 1, -1, 42340, 3, -1, },
	},
	{
		GOAL = 6,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 7,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 8,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 9,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 10,
		ItemList = { 102030, 1, -1, 42340, 3, -1,},
	},
	{
		GOAL = 11,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 12,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 13,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 14,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 15,
		ItemList = { 102030, 1, -1, 42340, 3, -1,},
	},
	{
		GOAL = 16,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 17,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 18,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 19,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 20,
		ItemList = { 102030, 1, -1, 42340, 3, -1,},
	},
	{
		GOAL = 21,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 22,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 23,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 24,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 25,
		ItemList = { 102030, 1, -1, 42340, 3, -1,},
	},
	{
		GOAL = 26,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 27,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 28,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 29,
		ItemList = { 102030, 1, -1, },
	},
	{
		GOAL = 30,
		ItemList = { 100030, 10, -1, 102030, 1, -1, 42340, 3, -1, },
	},
	{
		GOAL = 31,
		ItemList = { 102030, 1, -1, },
	},
}
