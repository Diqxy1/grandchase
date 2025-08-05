FALSE = 0
TRUE = 1

-- CharEnum
CHAR_ELESIS = 0
CHAR_LIRE = 1
CHAR_ARME = 2
CHAR_LAS  = 3
CHAR_RYAN = 4
CHAR_RONAN = 5
CHAR_AMY = 6
CHAR_JIN = 7
CHAR_SIEG = 8
CHAR_MARI = 9
CHAR_DIO = 10
CHAR_ZERO = 11
CHAR_LEY = 12
CHAR_LUPUS = 13
CHAR_RIN = 14
CHAR_ASIN = 15

SUCCESS = 0 -- ����
FAILED = 1  -- ��

-- �̺�Ʈ �Ⱓ ����
BeginTime   = { 2012, 08, 28, 09 }   -- Y, M, D, H
EndTime     = { 2015, 10, 06, 10 }   -- Y, M, D, H

-- ��í ���� ����
GachaLotteryVersion = 1
-- ��í ���� ItemID
GachaLotteryItemID = 1000540
-- ��í ���� ���� ����
GachaNoticeEnable = TRUE
-- ������� ġȯ String
GradeStringDesc = 
{
	{ Grade = 1, msgID = "str_12" },
	{ Grade = 2, msgID = "str_13" },
	{ Grade = 3, msgID = "str_14" },	
}
-- ��í ���� �ý���
InitGachaLottery = 
{
	{
		CharType = CHAR_ELESIS,
		GradeInfo = 
		{
			{ Grade = 1, ItemID = 1000020 },
			{ Grade = 2, ItemID = 795340 },
			{ Grade = 3, ItemID = 1000190 },
		},
		RewardType =
		{ -- { Type, Ratio }
			{ SUCCESS, 50 },
			{ FAILED, 50 },
		},
		SuccessRewardList = 		
		{   -- ������ ���� ������� ��� ���÷���. (1�� ���� ������ ���� �ϼ�)
			-- { ItemID, Duration, Period, EquipLevel, Ratio }
			{1000020,-1,-1,80,7.14}, -- ��í Ǯ��Ʈ
			{795340,-1,-1,0,7.14}, --��í ��			
			{1000190,-1,-1,80,7.14}, --��í ���弼Ʈ
			{782390,-1,-1,80,7.14}, --��í ����
			{782400,-1,-1,80,7.14}, --��í ����
			{782410,-1,-1,80,7.14}, --��í ����
			{782420,-1,-1,80,7.14}, --��í �尩
			{782430,-1,-1,80,7.14}, --��í �Ź�
			{782440,-1,-1,80,7.14}, --��í ����			
			{783880,-1,-1,80,7.14}, --��í ��Ŭ��
			{783890,-1,-1,80,7.15}, --��í ����
			{783900,-1,-1,80,7.15}, --��í �������
			{783910,-1,-1,80,7.15}, --��í ����
			{784480,-1,-1,80,7.15}, --��í ����ũ	
			
		},
		FailRewardList = 
		{ -- { ItemID, Duration, Period, Ratio }
			{1000360,1,-1,0,100}, --��� ��
		},
	},
}
