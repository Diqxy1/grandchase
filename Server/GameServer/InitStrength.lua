TRUE = 1
FALSE = 0

ENCHANT = 0 -- ��ȭ
BREAKUP = 1 -- ��ü
COMPOSE = 2 -- �ռ�

-- �ռ� ���̺� ���� ����.
ComposeResultTypeA = 0 -- �ռ� ����� Type A
ComposeResultTypeB = 1 -- �ռ� ����� Type B
ComposeResultTypeC = 2 -- �ռ� ����� Type C
ComposeResultTypeD = 3 -- �ռ� ����� Type D
ComposeResultTypeE = 4 -- �ռ� ����� Type E
ComposeResultTypeF = 5 -- �ռ� ����� Type F

NORMALNORMAL = 0 -- �Ϲݰ�ȭ��� + �Ϲݰ�ȭ���
NORMALEPIC = 1   -- �Ϲݰ�ȭ��� + ���Ȱ�ȭ���
EPICEPIC = 2     -- ���Ȱ�ȭ��� + ���Ȱ�ȭ���

-- ��ȭ ��� Ȯ��
SUCCESS      = 1 -- ����
WEAKNESS     = 2 -- �϶�
BREAK        = 3 -- ����
NONE         = 4 -- ���ڸ�
DROP         = 5 -- �ʱ�ȭ
UPGRADE		 = 6 -- ���� ��ȭ������ ���׷��̵�

GRADE_NORMAL   = 0 -- �Ϲ� ��ȭ��
GRADE_RARE     = 1
GRADE_EPIC     = 2 -- ���� ��ȭ��
GRADE_LEGEND   = 3

-- ��ȭ DB ���̱׷��̼� ����
MigrationEnable = FALSE

-- ���� ��� �ð� ����
NoticeTimeGap = 15000 -- 15��

-- ��ȭ���� ��ų ���� ����( �߰� Ȯ�强�� ����Ͽ� ��޺� ��ų ������ �����ϰ� ���� )
StrengthGradeSkillInfo = {
	{ GRADE_NORMAL, 0 },
	{ GRADE_EPIC, 1 },
}

InitStrength = { -- { ��ȭType, ����, Ȯ��{ ����, �϶�, ����, ���ڸ�, �ʱ�ȭ, ���Ⱦ��׷��̵� }
	StrengthItem = 627270, -- �Ϲ� ��ȭ�� ItemID
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

--��ȭ ��� ������ ����.
StrengthMaterialItem = {
	DefaultStrengthMaterial = 8880, -- �⺻ ��ȭ ���( GEM )
	DefaultStrengthValue = 2, -- �⺻ ��ȭ ��� ���� ����( ��ȭ Gem ��� = ��ȭ �ܰ� * DefaultStrengthValue )
	NormalStrengthMinLevel = 20, -- �Ϲ� ��ȭ�� ��ȭ�� ��� �ּ� ����.
	EpicStrengthMinLevel = 50,   -- ���� ��ȭ�� ��ȭ�� ��� �ּ� ����.
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

-- ��ȭ ���� ������ ����Ʈ
StrengthUpgradeAssist = { -- ��ȭ�� ��� ������ { ItemID, ����Ȯ�� }
	{ 627290, 5 },
	{ 627300, 10 },
}

StrengthProtection = {  -- ��ȭ ��ȣ �ֹ���
	-- { ItemID, { StartLv, EndLv }
	{ ItemID = 627310, LevelRange = { 1, 10 }, },
}
SpecificLevelStrength = { -- Ư������ ��ȭ ��ȭ �ֹ��� { ItemID, Ư����ȭ���� }
	{ 627360, 7 },
	{ 627370, 8 },
	{ 627380, 9 },
	{ 627390, 10 },
}

RandomLevelStrength = { -- ���� ���� ��ȭ ����( ������ )
	{
		ItemID = 627400,  -- Ư�� Level ItemID
		LevelRatio = {  -- { Level, Ratio }
			{ 3, 20 },
			{ 4, 20 },
			{ 5, 30 },
			{ 6, 30 },
		},
	},
}

-- ��ü ��� ������ ����Ʈ.
BreakUpItem = { -- { ItemID, ���� Ȯ��, ��ȭ�� �ı����� }
	{ ItemID = 627320, BreakUpRatio = { { SUCCESS, 30 }, { BREAK, 70 } } }, -- �Ϲ� ��ü �ֹ���
	{ ItemID = 627330, BreakUpRatio = { { SUCCESS, 30 }, { NONE, 70 } } }, -- Ư�� ��ü �ֹ���
}

-- �ռ� ������ ����Ʈ.
-- ����� Type�� ���� ����
-- �ش� ��� ������ ���� �ڵ� Level�� ���ǵǾ� �ִ�. ��ũ��Ʈ�� ���ǵ� ������ �ڵ� Level�� ������ �κ��� ����
-- ComposeResultTypeA = { ���� : �Ϲ� ��ȭ��, ��ȭ : ( A��� ��ȭ ���� + B��� ��ȭ ���� )/ 1.5 )
-- ComposeResultTypeB = { ���� : �Ϲ� ��ȭ��, ��ȭ : ( A��� ��ȭ ���� + B��� ��ȭ ���� )/ 2.0 )
-- ComposeResultTypeC = { ���� : �Ϲ� ��ȭ��, ��ȭ : ( A��� ��ȭ ���� + B��� ��ȭ ���� )/ 3.0 )
-- ComposeResultTypeD = { ���� : ���� ��ȭ��, ��ȭ : ( A��� ��ȭ ���� + B��� ��ȭ ���� )/ 1.5 )
-- ComposeResultTypeE = { ���� : ���� ��ȭ��, ��ȭ : ( A��� ��ȭ ���� + B��� ��ȭ ���� )/ 2.0 )
-- ComposeResultTypeF = { ���� : ���� ��ȭ��, ��ȭ : ( A��� ��ȭ ���� + B��� ��ȭ ���� )/ 3.0 )
ComposeTypeDefine = {
	{ ItemCompose = { GRADE_NORMAL, GRADE_NORMAL }, ComposeType = NORMALNORMAL },
	{ ItemCompose = { GRADE_NORMAL, GRADE_EPIC }, ComposeType = NORMALEPIC },
	{ ItemCompose = { GRADE_EPIC, GRADE_EPIC }, ComposeType = EPICEPIC },
	{ ItemCompose = { GRADE_EPIC, GRADE_NORMAL }, ComposeType = NORMALEPIC },
}

ComposeResultRatio = { -- �ռ���� Type�� ���� ��� { ResultType, { ��ȭ�� ����, Ratio } }
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
		ComposeItemID = 627340, -- �Ϲ� �ռ� �ֹ���.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeC, 40 },
			{ ComposeResultTypeB, 50 },
			{ ComposeResultTypeE, 10 },
		},
	},
	{
		ComposeType = NORMALEPIC,
		ComposeItemID = 627340, -- �Ϲ� �ռ� �ֹ���.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeC, 20 },
			{ ComposeResultTypeB, 50 },
			{ ComposeResultTypeF, 10 },
			{ ComposeResultTypeE, 20 },
		},
	},
	{
		ComposeType = EPICEPIC,
		ComposeItemID = 627340, -- �Ϲ� �ռ� �ֹ���.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeF, 40 },
			{ ComposeResultTypeE, 50 },
			{ ComposeResultTypeD, 10 },
		},
	},
	{
		ComposeType = NORMALNORMAL,
		ComposeItemID = 627350, -- ���� �ռ� �ֹ���.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeA, 40 },
			{ ComposeResultTypeC, 50 },
			{ ComposeResultTypeF, 10 },
		},
	},
	{
		ComposeType = NORMALEPIC,
		ComposeItemID = 627350, -- ���� �ռ� �ֹ���.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeA, 20 },
			{ ComposeResultTypeC, 50 },
			{ ComposeResultTypeD, 10 },
			{ ComposeResultTypeF, 20 },
		},
	},
	{
		ComposeType = EPICEPIC,
		ComposeItemID = 627350, -- ���� �ռ� �ֹ���.
		ComposeRatio = {  -- { ResultType, Ratio }
			{ ComposeResultTypeF, 45 },
			{ ComposeResultTypeE, 40 },
			{ ComposeResultTypeD, 15 },
		},
	},
}

-- ������ ���� Type
EISP_HELMET = 1 -- ���
EISP_JACKET = 2 -- ����
EISP_PANTS  = 3 -- ����
EISP_GLOVES = 4 -- �尩
EISP_SHOES	= 5 -- �Ź�
EISP_MANTLE = 6 -- ����
EISP_WEAPON	= 7 -- ����

-- ��ȭ �Ӽ� Type
ATTRTYPE_CRITICAL_DAMAGE_RESIST = 27 -- ũ��Ƽ�õ����� ���׷� - ��뿡�Լ� ũ��Ƽ�� �������� ���� �� ȿ����ŭ �������� ���ҽ�Ų��
ATTRTYPE_CRITICAL_DAMAGE_UP = 28     -- ũ��Ƽ�õ����� ���� 
ATTRTYPE_CATCH_DAMAGE_RESIST =29	 -- ��� ������ ���׷� 
ATTRTYPE_RECOVERY_UP_PET_MP = 30	 -- �� MPȸ���ӵ� ���� 
ATTRTYPE_MOVEMENT_SPEED_UP = 31		 -- �̵��ӵ� ����( �޸���, �ȱ� )
ATTRTYPE_RECOVERY_UP_POTION = 32	 -- ȸ������ ȿ�� ����
ATTRTYPE_FATAL_SUPER_ARMOR_BUFF = 33 -- Fatal�� ���۾Ƹӹߵ� ����
ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF = 34 -- Fatat�� MPȸ�� �ӵ� ��� ����
ATTRTYPE_FATAL_ATTACK_UP_BUFF = 35		-- Fatat�� ���ݷ� ��� ����
ATTRTYPE_FATAL_SPEEP_UP_BUFF = 36		-- Fatat�� �̵��ӵ� ��� ����
ATTRTYPE_FATAL_RECOVERY_POTION_UP = 37	-- Fatat�� ȸ������ ȿ�� ���
ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF = 38 -- Fatat�� ũ��Ƽ�� ������ ��� ����
ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP = 39 -- ��ȭȮ�� ���
ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST = 40 -- ��ȭ�ı� ����

-- ��ȭ�� �Ӽ�.
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

-- ��ȭ ���� �Ǹ� ������ ����Ʈ
StrengthShopItemList = { 642630, 642650, 642670, 627350, }

-- ��ȭ ���� ��� ����
StrengthNotice = {
	NoticeEnable = TRUE,
	NoticeMinStrengthLevel = 10,
}

-- ��ȭ�� ���� ���ϴ� ������ ����Ʈ.
NonStrengthItemList = { } -- { ItemID, }

StrengthUpgradeProtectionAssist = { -- ( ��ȭ�� ��� + �϶� ���� ) �ֹ���
	{
		ItemID = 762780,		-- ItemID
		fRatio = 100,           -- ��ȭ ���� ���� Ȯ��
		LevelRange = { 15, 17 }, -- ��밡�� ����
	},
}

-- ��ȭ�� �ʿ��� GP����
StrengthGPCost = {
	-- { Level, ����GP, ��GP },
	{ 1, 1000, 500 },
	{ 2, 1000, 500 },
	{ 3, 2000, 1000 },
	{ 4, 2000, 1000 },
	{ 5, 2000, 1000 },
	{ 6, 4000, 2000 },
	{ 7, 5000, 2500 },
}