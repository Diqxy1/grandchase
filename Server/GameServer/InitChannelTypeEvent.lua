TOTAL      = 0 	-- ��ü���� ���
PUBLISHER1 = 1	-- ä��Ÿ�� 1 ( �ݸ���,  �ݹڽ� )
PUBLISHER2 = 2  -- ä��Ÿ�� 2 ( ���Ϸ���, �÷�����ũ )
PUBLISHER3 = 3  -- ä��Ÿ�� 3 ( ����Ʈ.., )

-- ä�θ� Ÿ�Կ� ���� ���� �̺�Ʈ

-- �̺�Ʈ �Ⱓ
BeginTime   = { 2010, 12, 09, 10 }   -- Y, M, D, H
EndTime     = { 2011, 01, 07, 01 }   -- Y, M, D, H

-- ���� ���ӽ� �޴� ������
DailyConnectionReward = 
{
	-- Reward = { ItemID, Duration, Period }
	--{ ChannelType = 2, Reward = { 366530, 1, -1, 366650, 1, -1 }, },

}

-- �̺�Ʈ �Ⱓ���� �ѹ��� �޴� ������.
-- DB�� �̺�Ʈ�� ���۳�¥�� �������� �޾Ҵ��� �ȹ޾Ҵ����� üũ�Ѵ�.
OneConnectionReward = 
{
	-- Reward = { ItemID, Duration, Period }
	{ ChannelType = 2, Reward = { 42410, -1, 30, 19360, 10, -1, 58710, 10, -1, 58740, 10, -1, }, },
}

-- �ű� ���� üũ �ð�.
NewBeginTime = { 2010, 10, 07, 10 }   -- Y, M, D, H
NewConnectionReward =  -- �ű� �������Ը� �����ϴ� ������.
{
	-- Reward = { ItemID, Duration, Period }
	-- { ChannelType = 0, Reward = { 378290, 1, -1, 378300, 1, -1, 378310, 1, -1, 378320, 1, -1, }, },
}

-- ChannelType�� �߰� ����ġ, SPX ����.
-- { ChaneelType, Ratio }
ChannelTypeRatio = {
--	{
--		ChannelType = TOTAL,
--		Ratio = 
--		{
--			Exp         = 0.3,      -- �߰� Exp
--			Gp          = 0.0,      -- �߰� Gp
--		}
--	}
}

-- Pet ����( Pet�������� ��� PC�� ������ó�� ������� �ʵ��� �۾��Ѵ�. )
-- 2147484000���� ����.
PetConnectionReward = 
{
    {
        ChannelType = TOTAL,
        Default = {
            {
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} ����������( ��ų ����. )
            },
            {
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} ����������( ��ų ����. )
            },
        },
        Event = {
            {
                Date = { 12, 29 }, -- ��,��
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} ����������( ��ų ����. )
            },
            {
                Date = { 1, 1 }, -- ��,��
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} ����������( ��ų ����. )
            },
        },
    },
}
