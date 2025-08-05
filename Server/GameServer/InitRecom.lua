
BeginTime   = { 2009, 12, 09, 10 }   -- Y, M, D, H
EndTime     = { 2009, 12, 23, 10 }   -- Y, M, D, H
ReturnTime  = { 2009, 11, 09, 10 }   -- Y, M, D, H
NewBeginTime = { 2009, 12, 09, 10 }  -- Y, M, D, H, �ű��������� �ƴ��� üũ�ð�.
AdjustPeriod = 45
Version = 1

AttendanceUserGap = 3
PartyPlayEventID = 1
PartyPlayBonusExp = 0.3
PartyPlayBonusGp = 0.3

-- ù �⼮�� �޴� ������.
FirstAttendance =
{
    New = { 19360, 30, -1, 42410, 3, -1, 42370, 1, -1, },
    Old = { 58720, 10, -1, 58750, 10, -1, 76760, 1, -1, 76740, 1, -1, 76720, 1, -1, 58700, 1, -1, },
    Recommendee = { 19360, 10, -1, 58630, 10, -1, 8880, 10, -1, 58700, 1, -1, },
}

-- ���� �⼮�� �޴� ������
DailyAttendance =
{
    New = { 8880, 1, -1 },
    Old = { 8880, 1, -1 },
}

RecommendReward =  -- ��õ ���� ����� ù �⼮ ��� �ߴ����� ����.. ����..
{
    { 59780, 1, -1, 81610, 1, -1, }, -- 3��
    { 59780, 1, -1, 58700, 1, -1, 81610, 1, -1, }, -- 6��
    { 59780, 1, -1, 42410, 5, -1, 58700, 2, -1, 58630, 10, -1, 84990, 1, -1, }, -- 9��
    { 79060, -1, -1, 38970, 1, -1, 59780, 1, -1, 42410, 5, -1, 58700, 3, -1, 84990, 1, -1, }, -- 12��
}

-- �ű����� Ư������ �޼��� �޴� ������
-- �� ������ �ѹ��� �����Ѵ�.
LevelUpReward = 
{
    --[[
    {
        Level = �ű������� ����,
        RewardItem = { ItemID, Duration, Period, }
    },
    --]]
    {
        Level = 5,
        RewardItem = { 8880, 1, -1, },
    },
    {
        Level = 10,
        RewardItem = { 189490, 1, -1, 189530, 10, -1, },
    },
	{
        Level = 3,
        RewardItem = { 8880, 3, -1, },
    },
	{
        Level = 4,
        RewardItem = { 8880, 4, -1, },
    },
	{
        Level = 5,
        RewardItem = { 8880, 5, -1, },
    },
}

-- ��õ���� ���� ���� �޼��� ���� ������.
RecommendLevelUp = 
{
    --[[
    {
        Level = �� ��õ�� ������ ����,
        RewardItem = { ItemID, Duration, Period, },
    },
    --]]
    {
        Level = 1,
        RewardItem = { 8880, 1, -1, },
    },
    {
        Level = 2,
        RewardItem = { 8880, 2, -1, },
    },
	{
        Level = 3,
        RewardItem = { 8880, 3, -1, },
    },
	{
        Level = 4,
        RewardItem = { 8880, 4, -1, },
    },
	{
        Level = 5,
        RewardItem = { 8880, 5, -1, },
    },
    {
        Level = 10,
        RewardItem = { 19360, 10, -1, 58630, 10, -1, 8880, 10, -1, 71770, 1, -1, 189530, 5, -1, 189540, 5, -1, 189550, 5, -1, 189560, 5, -1, },
    },
}

ConnectMsg = "str_11"