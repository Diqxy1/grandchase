KR = 1  -- �ѱ�
TW = 3  -- �븸
BR = 4  -- �����
US = 8  -- �̱�
PH = 9  -- �ʸ���
TH = 11 -- �±�
ID = 12 -- �ε��׽þ�
LA = 13 -- ����

BeginTime = { 2011, 9, 1, 00 }	-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
EndTime = { 2011, 9, 30, 00 }	-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
Version = 1
TickGap = 1000 * 60        		-- ms
CountryID = KR
CountryStringID = "str_14"		-- ���� StringID.., ���� String ������ config_center.ini���Ͽ� ����.

-- ��޿� ���� ����
GradeRewardInfo = {
	{
		GradeID = 1,
		ExpReward = 0.0,
		GPReward = 0.0,
	},
	{
		GradeID = 2,
		ExpReward = 0.0,
		GPReward = 0.0,
	},
}

-- ������ ���� ����
RankingRewardInfo = {
	{
		RankingNum = 1,
		RewardInfo = { }, -- { ItemID, Duration, Period, .., }
	},
	{
		RankingNum = 2,
		RewardInfo = { }, -- { ItemID, Duration, Period, .., }
	},
}

-- ������ ���� ���
GradeDesc = {
	{ GradeID = 1, StratPoint = 0, EndPoint = 50000	},
	{ GradeID = 2, StratPoint = 50000, EndPoint = 100000 },
}
