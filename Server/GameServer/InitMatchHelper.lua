--���Ӽ��� ��Ī ���� ��ũ��Ʈ

-- ELO Rating ��� K Table
ConstantKTable = 
{
	-- RA ����( ����, �� ), ��� K��
	{ RaStartValue = 0, RaEndValue = 2000, ConstantK = 32 },
	{ RaStartValue = 2001, RaEndValue = 10000, ConstantK = 24 },
}

-- ��ġ��� ���� RP Table
PlacementTestRPTable = 
{
	-- ĳ���� ���� ����, ���� Rating
	{ StartLevel = 0, EndLevel = 11, StartRPValue = 300 },
	{ StartLevel = 12, EndLevel = 23, StartRPValue = 500 },
	{ StartLevel = 24, EndLevel = 35, StartRPValue = 650 },
	{ StartLevel = 36, EndLevel = 44, StartRPValue = 750 },
	{ StartLevel = 45, EndLevel = 53, StartRPValue = 850 },
	{ StartLevel = 54, EndLevel = 62, StartRPValue = 950 },
	{ StartLevel = 63, EndLevel = 71, StartRPValue = 1250 },
	{ StartLevel = 72, EndLevel = 80, StartRPValue = 1550 },
	{ StartLevel = 81, EndLevel = 85, StartRPValue = 1851 },
}
PlacementTestPlayCount = 12 -- ��ġ��� �Ǽ� ����.
PlacementTestFirstConstantK = 300 -- ��ġ��� �ʱ� ��� K��.
PlacementTestRatio = 0.7 -- ��ġ��� ���ǰ� ��� �ٸ� ��� ������.