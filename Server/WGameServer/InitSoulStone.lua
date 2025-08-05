FALSE = 0
TRUE = 1

-- �ҿｺ��

-- �� ���� ItemID������ �ֱ�.
-- 1�� �Ӽ��� ���� �Ҹ� ���� ����.
-- 1,2,3�ܺ� �Ӽ��� Ȯ�� ����.

-- �ҿｺ�� �Ӽ�
FIRE = 0
ICE = 1
EARTH = 2
WIND = 3

SoulStoneUseEnable = TRUE

-- �ҿｺ�� ItemID
-- �Ӽ� �߰��ÿ� �ڵ� �۾� ���� �ʱ� ���ؼ� �����ڵ忡�� Ȯ�强�ְ� ���̺� ������ ����.
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
	-- Grade, Ratio[ {Damage, Ratio} ] Ȯ���� �׻� 100%�� ���߾�� �Ѵ�.
	{ SoulStoneType = FIRE, Grade = 1, GradeRatio = { {10, 20}, {20, 20}, {30, 20}, {40, 20}, {50, 20}, }, },
	{ SoulStoneType = FIRE, Grade = 2, GradeRatio = { {20, 20}, {30, 20}, {40, 20}, {50, 20}, {60, 20}, }, },
	{ SoulStoneType = FIRE, Grade = 3, GradeRatio = { {30, 20}, {40, 20}, {50, 20}, {60, 20}, {70, 20}, }, },
	{ SoulStoneType = FIRE, Grade = 4, GradeRatio = { {40, 20}, {50, 20}, {60, 20}, {70, 20}, {80, 20}, }, },
		
	-- Grade, Ratio[ {Damage, Ratio} ] Ȯ���� �׻� 100%�� ���߾�� �Ѵ�.
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