-- ��Ī���� ������ ��ũ��Ʈ.
TRUE = 1
FALSE = 0

-- ��� ����.
PVP1vs1 = 0
PVP2vs2 = 1
PVP3vs3 = 2

StartSearchScope = 50 -- ���� Rating ������.
MaxIncreaseScope = 400 -- �ִ� �����ϴ� RP ������.
IncreaseScope = 50 -- �����ϴ� RP ������.
-- ���Ӽ��� InitMatchHelper.lua���� ���Ǳ� ������ �Ʒ��� 2������ �����ɶ����� ���Ӽ����� ���� ���� �ʿ�.

--ModeID ��Ī ��� ����.
--bLimitMatchEnable �ּ� �������� ������ �� �ֵ��� ON/OFF ��� �߰�.
--LimitMatchGroupNum �ּ� ������ �̻��� �׷��� �Ǿ�� ��Ī�˻��� �����Ѵ�.
LimitMatchTable = 
{
	{ ModeID = PVP1vs1, LimitMatchGroupNum = 15, bLimitMatchEnable = FALSE },
	{ ModeID = PVP2vs2, LimitMatchGroupNum = 7, bLimitMatchEnable = TRUE },
	{ ModeID = PVP3vs3, LimitMatchGroupNum = 15, bLimitMatchEnable = FALSE },
}