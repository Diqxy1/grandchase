TRUE = 1
FALSE = 0

-- ī�� ���
GRADE_NORMAL   = 0
GRADE_RARE     = 1
GRADE_EPIC     = 2
GRADE_LEGEND   = 3



-- ����ī�� ���� ��뿩��
MonsterCardMixEnable = FALSE

MinMaterialCount = 2 -- �ּ� ����
MaxMaterialCount = 7 -- �ִ� ����

-- ī�� ��޺� ����Ʈ
Point = {
	-- ī����, ����
	{ GRADE_NORMAL, 1 },
	{ GRADE_RARE, 2 },
	{ GRADE_EPIC, 3 },
	{ GRADE_LEGEND, 5 },
}

--ȹ�� ī�� �׷� Ȯ��
PointRewardRatio = {
	-- pointRange = {�ΰ�, �ƽ���}, RewardGrop = {�׷�1,Ȯ��},{�׷�2,Ȯ��}
	{ PointRange = { 2, 10 }, RewardGroup = {{ 1, 50 }, { 2, 50 },}, },
	{ PointRange = { 11, 20 }, RewardGroup = {{ 1, 30 }, { 2, 40}, { 3, 30 },}, },
	{ PointRange = { 21, 30 }, RewardGroup = {{ 2, 30 }, { 3, 40}, { 4, 30 },}, },
}

--�׷캰 ī�� ���� ( GroupNum = �׷�ѹ� , { ī����̵�, Ȯ��, ī����̵�, Ȯ�� }
RewardGroup = {
	{ GroupNum = 1, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 2, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 3, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 4, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
	{ GroupNum = 5, RewardRatio = { 303030, 30, 303040, 30, 303050, 30, 303060, 10, }, },
}
