-- 2011-01-24 19:49:10
TRUE = 1
FALSE = 0
EquipmentType = 0 -- ��� Type
AccessoriesType = 1 -- �Ǽ��縮 Type
EquipmentRewardType = 0 -- ��� ����
TotalRewardType = 1 -- ��ü ����

GachaVersion = 11
GachaMaxReelPoint = 300
InitGacha = 
{
	ReelPointEnable = TRUE,
	EquipmentEnable = TRUE,
        AccessoriesEnable = TRUE,
	{
		GachaItem = 101840,
		KeyItemID = 102030,
		NeedKeyItemCount =
		{
			{ 10, 1 }, -- { Lv, NeedKeyCount }
			{ 20, 1 },
			{ 30, 2 },
			{ 40, 2 },
			{ 50, 3 },
			{ 60, 3 },
			{ 70, 4 },
		},
		GachaSetRewardList =
		{
			{
				SetRewardType = EquipmentRewardType,
				RewardList = 
				{
					{	545500	,-1,-1},
					{	545510	,-1,-1},
					{	545520	,-1,-1},
					{	545530	,-1,-1},

				},				
			},
			{
				SetRewardType = TotalRewardType,
				RewardList = 
				{
					{238060, -1, -1}, -- �����ҿ���
				},
			},
		},
		CollectItemList =
		{
			{
				CollectItemType = EquipmentType,
				CollectItemList = 
				{
					{	322500	,-1,-1, 0},
					{	322510	,-1,-1, 0},
					{	322520	,-1,-1, 0},
					{	322530	,-1,-1, 0},
					{	322540	,-1,-1, 0},
					{	322550	,-1,-1, 0},

				},
			},
			{
				CollectItemType = AccessoriesType,
				CollectItemList = 
				{
					{	323300	,-1,-1, 0},
					{	323310	,-1,-1, 0},
					{	323320	,-1,-1, 0},
					{	323330	,-1,-1, 0},
					{	323800	,-1,-1, 0},
2

				}
			},
		},
		FailItemList =
		{			
			{
				ItemLevel = 10,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 20,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 30,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 40,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 50,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 60,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 70,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
		},
	},
	{
		GachaItem = 101870,
		KeyItemID = 102030,
		NeedKeyItemCount =
		{
			{ 10, 1 }, -- { Lv, NeedKeyCount }
			{ 20, 1 },
			{ 30, 2 },
			{ 40, 2 },
			{ 50, 3 },
			{ 60, 3 },
			{ 70, 4 },
		},
		GachaSetRewardList =
		{
			{
				SetRewardType = EquipmentRewardType,
				RewardList = 
				{
					{	545670	,-1,-1},
					{	545680	,-1,-1},
					{	545690	,-1,-1},
					{	545700	,-1,-1},

				},				
			},
			{
				SetRewardType = TotalRewardType,
				RewardList = 
				{
					{238060, -1, -1}, -- �����ҿ���
				},
			},
		},
		CollectItemList =
		{
			{
				CollectItemType = EquipmentType,
				CollectItemList = 
				{
					{	545610	,-1,-1, 0},
					{	545620	,-1,-1, 0},
					{	545630	,-1,-1, 0},
					{	545640	,-1,-1, 0},
					{	545650	,-1,-1, 0},
					{	545660	,-1,-1, 0},

				},
			},
			{
				CollectItemType = AccessoriesType,
				CollectItemList = 
				{
					{	545710	,-1,-1, 0},
					{	545720	,-1,-1, 0},
					{	545730	,-1,-1, 0},
					{	545740	,-1,-1, 0},
					{	545750	,-1,-1, 0},
					{	545760	,-1,-1, 0},

				}
			},
		},
		FailItemList =
		{			
			{
				ItemLevel = 10,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 20,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 30,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 40,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 50,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 60,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 70,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
		},
	},
	{
		GachaItem = 101900,
		KeyItemID = 102030,
		NeedKeyItemCount =
		{
			{ 10, 1 }, -- { Lv, NeedKeyCount }
			{ 20, 1 },
			{ 30, 2 },
			{ 40, 2 },
			{ 50, 3 },
			{ 60, 3 },
			{ 70, 4 },
		},
		GachaSetRewardList =
		{
			{
				SetRewardType = EquipmentRewardType,
				RewardList = 
				{
					{	545840	,-1,-1},
					{	545850	,-1,-1},
					{	545860	,-1,-1},
					{	545870	,-1,-1},


				},				
			},
			{
				SetRewardType = TotalRewardType,
				RewardList = 
				{
					{238060, -1, -1}, -- �����ҿ���
				},
			},
		},
		CollectItemList =
		{
			{
				CollectItemType = EquipmentType,
				CollectItemList = 
				{
					{	545780	,-1,-1, 0},
					{	545790	,-1,-1, 0},
					{	545800	,-1,-1, 0},
					{	545810	,-1,-1, 0},
					{	545820	,-1,-1, 0},
					{	545830	,-1,-1, 0},

				},
			},
			{
				CollectItemType = AccessoriesType,
				CollectItemList = 
				{
					{	545880	,-1,-1, 0},
					{	545890	,-1,-1, 0},
					{	545900	,-1,-1, 0},
					{	545910	,-1,-1, 0},
					{	545920	,-1,-1, 0},
					{	545930	,-1,-1, 0},

				}
			},
		},
		FailItemList =
		{			
			{
				ItemLevel = 10,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 20,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 30,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 40,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 50,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 60,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
			{
				ItemLevel = 70,
				LevelRatioReward = {
					{ 42340, -1, 1, 49, }, -- GC Ŭ��
					{ 71780, 1, -1, 21, }, -- ��ȭ�� ��� ������(5�ۼ�Ʈ UP)
					{ 287060, 3, -1, 30, }, -- ���� ���
				},
			},
		},
	},
}
-- Gacha Real Point ��밡���� Item�� ��ް� Div�� ���.
GachaRealPointInfo = 
{
	ItemList = {
		-- ItemID, Grade, Div -- Comment
		{ 101840,6,2.7},
		{ 101870,6,2.7},
		{ 101900,6,2.7},
	}
}
