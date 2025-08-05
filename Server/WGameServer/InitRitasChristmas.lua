--[[
2012�� ũ�������� �̺�Ʈ������ ���۵� �̴ϰ���

�۾���: ����ö, ��ȯ��, ������, ���⼷, ���翵
--]]

-- �̺�Ʈ ����
Version = 1

-- �̺�Ʈ �Ⱓ ����
BeginTime   = { 2012, 11, 27, 00 }   -- �̺�Ʈ ���� �ð�(Y, M, D, H)
EndTime     = { 2013, 01, 03, 10 }   -- �̺�Ʈ ���� �ð�(Y, M, D, H)
RankUpdateTime = { 01, 00 }   -- ��ŷ ������ ���� �������� �ð�(H, m)

-- ���� ���۰���
COIN_PER_GAME = 1   -- ���� ���۽� �Ҹ�Ǵ� ���� ��
COIN_SUPPLY_INTERVAL = 4 * 60   -- ������ �����Ǵ� �ð� ����(��)
COIN_SUPPLY_COUNT = 1   -- �ѹ��� �����Ǵ� ���� ��
COIN_MAX_QUANTITY = 5   -- �ִ� ���� ���� ��
COIN_DOUBLE_GAIN_ITEMID = 1046790   -- �ϸ� ���� �ν���. ���� ������ ������ �ι�� �����Ǵ� ������ID

CONTINUE_COIN_EXCHANGE_ITEMID = 1046730     -- �絵�� ���� ���� ������ID(���� ������)
CONTINUE_COIN_EXCHANGE_CONSUME_COUNT = 1    -- ������ �Ҹ�Ǵ� �絵�� ���� ������ ��
CONTINUE_COIN_EXCHANGE_RESULT_COUNT = 1     -- ���� ������ �ϳ��� �����Ǵ� �絵�� ���� ��

-- �÷�������
PlayInfo = {
   SlotNum = 12,        -- ������Ʈ�� ������ �� �ִ� ���� ����
   BasketSize = 10,     -- �ٱ��� ������
   BasketWidth = 0.1,    -- �ٱ����� ���� (Map ������ ���ġ ���� %�� ������ �ָ� ��)
   PlayerSpeed = 6.3,    -- �÷��̾�(�ٱ���) ���ǵ�
   CatchZoneTop = 0.83,   -- ������ ���α׷���(ȯ��)�� �ǳ��Ұ�
   CatchZoneBottom = 0.93,  -- ������ ���α׷���(ȯ��)�� �ǳ��Ұ�
}

-- ������Ʈ ����
OT_NORMAL = 0
OT_FEVER = 1

ObjectInfo =
{
    {
        ObjectID = 0,   -- ������ƮID�� �׻� 0���� �����ؾ��Ѵ�
        ObjectType = OT_NORMAL,
        DownSpeed = 120.1,  -- �ӵ�
        Point = 50,    -- ȹ�� ����
        DecreaseTime = 1,   -- Ÿ�̸� ���ҷ�(��)
    },
    {
        ObjectID = 1,
        ObjectType = OT_NORMAL,
        DownSpeed = 130.1,  -- �ӵ�
        Point = 100,    -- ȹ�� ����
        DecreaseTime = 1,   -- Ÿ�̸� ���ҷ�(��)
    },
    {
        ObjectID = 2,
        ObjectType = OT_NORMAL,
        DownSpeed = 150.1,  -- �ӵ�
        Point = 150,    -- ȹ�� ����
        DecreaseTime = 1,   -- Ÿ�̸� ���ҷ�(��)
    },
    {
        ObjectID = 3,
        ObjectType = OT_FEVER,  -- �ǹ� ������Ʈ�� �ϳ��� �����ϵ��� �����Ǿ� ����
        DownSpeed = 300.1,  -- �ӵ�
        Point = 700,    -- ȹ�� ����
        DecreaseTime = 0,   -- Ÿ�̸� ���ҷ�(��)
    },
}

BASKET_EMPTY_POINT_LIMIT = 150 * 10   -- �ٱ��� ���� �ѹ��� ���� �� �ִ� �ִ� ����(��ŷüũ��)
FEVER_POINT_LIMIT = 70000   -- �ǹ� ������Ʈ�� ���������� ���� �� �ִ� �ִ� ����(��ŷüũ��)

-- �������� ����
StageInfo =
{
    {
        StageNum = 1,   -- �������� ��ȣ
        TimeLimit = 120.1, -- �������� ���ѽð�(��)
        GoalCount = 25,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 0.8,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 1,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.5,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 3.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 3, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 2,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 40,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 0.9,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 1,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.3,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 3, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 3,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 50,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.2,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 1,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.2,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 4,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 55,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.3,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 2,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.2,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 5,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 60,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.4,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 2,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.2,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 6,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 65,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.5,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 2,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.1,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 7,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 70,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.6,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 3,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.1,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 8,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 75,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.7,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 1,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 3,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.0,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 9,   -- �������� ��ȣ
        TimeLimit = 90.1, -- �������� ���ѽð�(��)
        GoalCount = 80,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 1.8,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 4,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 1.0,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
    {
        StageNum = 10,   -- �������� ��ȣ
        TimeLimit = 5.1, -- �������� ���ѽð�(��)
        GoalCount = 100,    -- ȹ���ؾ� �ϴ� ������Ʈ ��
        DownSpeedIncRatio = 2.8,   -- ������Ʈ ���ϼӵ� ��������(1.0: �⺻ �ӵ�)
        ClearRewardCount = 4,   -- Ŭ����� ��� ���� ��
        ContinueCoinCount = 1,  -- �̾��ϱ�� �Ҹ�Ǵ� ���� ��
        RemainTimeBonusPointPerSec = 100,   -- �����ð� ���ʽ� ����(�ʴ� ����)
        GenerateSec = 0.2,      -- �Ϲ� ������Ʈ�� ������Ű�� ��(��)
        GenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �Ϲ� ������Ʈ�� ����
        FeverGenerateSec = 0.1,      -- �ǹ� ������Ʈ�� ������Ű�� ��(��)
        FeverGenerateNum = 1,      -- GenerateTime�� �Ǿ����ÿ� ������ų �ǹ� ������Ʈ�� ����
        FeverTimeDuration = 4.1,  -- �ǹ�Ÿ�� ���ӽð�(��)
        ComboLimit = 4, -- �ִ� ��ǥ �޺� ��
    },
}

-- ���� ����
SpecialRewardInfo =
{
    { 1046700, -1, -1, 0.001 }, -- ItemID, ����, �Ⱓ, Ȯ��
	{ 1046740, 1, -1, 2.5 },
	{ 1056970, 1, -1, 1.499 },
	{ 1046980, 1, -1, 7 },
	{ 1047350, 1, -1, 7 },
	{ 1046780, 1, -1, 7 },
	{ 133670, 1, -1, 25 },
	{ 133680, 1, -1, 25 },
	{ 133690, 1, -1, 25 },
}

PREMIUM_KEY_ITEMID = 1046740    -- �����̾� ���ڸ� �� �� �ʿ��� ���� ������(���ڴ� 1���� ����)
PREMIUM_PER_FEVER_OBJECT_COUNT = 10   -- �����̾� ���� 1���� ��µ� �ʿ��� �ǹ�Ÿ�� ������Ʈ ��
PremiumRewardInfo =
{
    { 1046700, -1, -1, 3 },    -- ItemID, ����, �Ⱓ, Ȯ��
    { 1046970, 1, -1, 7 },
	{ 1046980, 1, -1, 18 },
	{ 1047340, 1, -1, 12 },
	{ 1047350, 1, -1, 18 },
	{ 1046740, 1, -1, 12 },
	{ 1056970, 1, -1, 30 },
}

-- Ŭ�� UI�� ���򸻿��� �������� �ȳ��� ������ ����Ʈ ����
RewardViewListInfo =
{
    1046700, 1046970, 1046980, 1047340, 1047350, 1046740, 1056970,  -- �Է��� ������� ��������
}
