-- ����
InvenLimitSeedCount = 20 -- �κ��丮 �ִ� ������ �� �ִ� ������ ����
DailyMaxSeedCount = 120 -- ���� ȹ���� �� �ִ� �ִ� ������ ����
DailyInitSeedCount = 20 -- ���� �ʱ�ȭ�Ǵ� ���� ���� (�ּҼ��� ����)
SeedDropTime = 1 -- ���� ����Ʈ���� ������ ���������� �ɸ��� ü�� �ð�(��)
SeedDropCount = 5 -- ���� ����Ʈ���� ü���ð��� ���� ���� ȹ�� ����

AgitCashPotCount = 2 -- ����Ʈ ��ġ������ ĳ��ȭ�� ����
InvenLimitFlowerCount = 99999 -- �κ��丮 �ִ� ������ �� �ִ� ���� ����

-- SEED_TYPE
ST_NONE = -1 -- �Ƚɾ��� ����
ST_FAST = 0 -- ���ڰ�
ST_NORMAL = 1 -- ����������
ST_BEST = 2 -- ������
-- ���ѽɱ� ����
FlowerPot = {
	{
		SeedType = ST_FAST, -- ���ѽɴ� Ÿ��
		NeedSeedCount = 1, -- �Һ񾾾�
		GrowTime = 1, -- ����ð�(��)
		FlowerCount = 1, -- ����Ϸ�� ��Ȯ�Ǵ� ���� ����
		BonusCount = 1, -- ��Ȯ�� ȹ�氡���� ���ʽ� ���� ����
	},
	{
		SeedType = ST_NORMAL, -- ���ѽɴ� Ÿ��
		NeedSeedCount = 1, -- �Һ񾾾�
		GrowTime = 2, -- ����ð�(��)
		FlowerCount = 5, -- ����Ϸ�� ��Ȯ�Ǵ� ���� ����
		BonusCount = 2, -- ��Ȯ�� ȹ�氡���� ���ʽ� ���� ����
	},
	{
		SeedType = ST_BEST, -- ���ѽɴ� Ÿ��
		NeedSeedCount = 1, -- �Һ񾾾�
		GrowTime = 3, -- ����ð�(��)
		FlowerCount = 10, -- ����Ϸ�� ��Ȯ�Ǵ� ���� ����
		BonusCount = 3, -- ��Ȯ�� ȹ�氡���� ���ʽ� ���� ����
	},
}

-- ItemID
SeedItemID = 605260 -- ���� ItemID
FlowerItemID = 605270 -- �� ItemID
NormalPotItemID = { 605280, } -- �Ϲ�ȭ�� ItemID
CashPotItemID = { 605290, } -- ĳ�� ȭ�� ItemID
