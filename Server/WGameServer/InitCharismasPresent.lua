TRUE = 1
FALSE = 0

BeginTime = { 2011, 10, 25, 00 }	-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
EndTime = { 2011, 12, 14, 18 }		-- Y, M, D, H �̺�Ʈ ���� �Ⱓ.
Version = 1
SledgeItem = 8880 -- ��� ItemID
LoopEnable = TRUE -- ��� ���� ���ο� ���ؼ� Ȯ��.
MaxGrade = 7
MaxPoint = 7000

-- ����
PresentItemInfo = 
{
	-- { {ItemID,Duration, Period },{ ��Ű���, ����Ʈ ���� }  }
	{ ItemInfo = {8880, 1, 1} , Config = { SledgeUseCnt = 5, BonusPoint = 10 }, },
}

-- ���
DonationInfo =
{
	-- ��Ű���, ����Ʈ ����
	{ SledgeUseCnt = 10, BonusPoint = 10 },
	{ SledgeUseCnt = 20, BonusPoint = 20 },
}

-- ��޺� ����
GradeRewardList = 
{
	-- { Grade, Reward = { ItemID, Duration, Period, .., }
	{ Grade = 1, Reward = { }, },
	{ Grade = 2, Reward = { }, },
}

-- ���ʽ� ����Ʈ�� ���
GradeDesc = {
	{ GradeID = 1, StratPoint = 0, EndPoint = 19000 },
	{ GradeID = 2, StratPoint = 19001,	EndPoint = 38000 },
	
}