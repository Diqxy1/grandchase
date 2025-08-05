require( "Enum.stg" )

TRUE = 1
FALSE = 0

ItemList = {
	--[[ Sample
	{
		ItemID = 8880,
		BuffID = -1, -- ����� ����ID (Ŭ���̾�Ʈ ���)
		GroupID = -1, -- ������ �׷�ID
		PriorityIndex = 0, -- �׷쳻 ����켱���� (0�� ���� ����)
		All = false, -- true : ��Ƽ����, false : ȥ������
		ModeCategory = { GC_GMC_DUNGEON, GC_GMC_MATCH, GC_GMC_GUILD_BATTLE, }, -- ���� ����Ǵ� ����ī�װ�
	},
	]]--

	{
		ItemID = 558860, -- ��������� ���� ��ǥ
		BuffID = -1,
		GroupID = -1,
		PriorityIndex = 2,
		All = FALSE,
		ModeCategory = { GC_GMC_DUNGEON, },
	},
	{
		ItemID = 558870, -- ��������� �ݺ� ��ǥ
		BuffID = -1,
		GroupID = -1,
		PriorityIndex = 1,
		All = FALSE,
		ModeCategory = { GC_GMC_DUNGEON, },
	},
	{
		ItemID = 558880, -- ��������� �������� ��ǥ
		BuffID = -1,
		GroupID = -1,
		PriorityIndex = 0,
		All = FALSE,
		ModeCategory = { GC_GMC_DUNGEON, },
	},
}

