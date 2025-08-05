CHAR_ELESIS = 0,     -- �����ý�
CHAR_LIRE = 1,       -- ����
CHAR_ARME = 2,       -- �Ƹ���
CHAR_LAS = 3,        -- ��
CHAR_RYAN = 4,       -- ���̾�
CHAR_RONAN = 5,      -- �γ�
CHAR_AMY = 6,        -- ���̹�
CHAR_JIN = 7,        -- ��
CHAR_SIEG,           -- ��ũ��Ʈ
CHAR_MARI,           -- ����
CHAR_DIO,            -- ���
CHAR_ZERO,           -- ����
CHAR_LEY,            -- ����

Equipment = 0 		 -- ���
Goal	  = 1		 -- ��ǥ
Normal	  = 2		 -- �Ϲ�
Fail	  = 3		 -- ��

BeginTime   = { 2011, 04, 13, 15 }   -- Y, M, D, H
EndTime     = { 2011, 09, 14, 10 }   -- Y, M, D, H
TornadoVersion = 0				     -- �ش� �̺�Ʈ ����� �ش� ������ 1�� ���̴� �۾� �ʿ�.
TornadoItemID = 8880				 -- ����̵� ���� ItemID
TornadoCoinItemID = 11110			 -- ����̵� ���� ItemID
TornadoFailCoinItemID = 22220		 -- ����̵� Fail�� ���� ItemID
TorandoFailCoinCount = 5			 -- ����̵� Fail ItemCount
ExposeItemToUser = 657800			 -- ó�� �̴ϰ��� ���� �� �������� ���� �� ������ 

InitTornado = 
{
	{
		CharType = CHAR_ELESIS,
		RewardItemList = { -- ��ü ������ ����Ʈ.
			{ 189520, 1, -1, 0.5, }, -- Jr.Ÿ���佺 �� ���ۼ�
			{ 99600, 1, -1, 10, }, -- ��ų ���� ����
			{ 42340, -1, 3, 6, }, -- GCŬ��
			{ 42410, -1, 3, 6, }, -- ������ ����
		},	
		NonFailRewardItemList = {
			{ 189520, 1, -1, 0.5, }, -- Jr.Ÿ���佺 �� ���ۼ�
			{ 99600, 1, -1, 10, }, -- ��ų ���� ����
			{ 42340, -1, 3, 6, }, -- GCŬ��
			{ 42410, -1, 3, 6, }, -- ������ ����
		},
		FailRewardItemList = {		
			{ 42340, -1, 3, 6, }, -- GCŬ��
			{ 42410, -1, 3, 6, }, -- ������ ����
		},		
		TypeItemList = {
			{ Type = Equipment,  ItemList = { }, },
			{ Type = Goal,  ItemList = { }, },
			{ Type = Normal,  ItemList = { }, },
			{ Type = Fail,  ItemList = { }, },
		},
		FailItemList = {
			{ 189520, 1, -1 }, -- Jr.Ÿ���佺 �� ���ۼ�
			{ 99600, 1, -1 }, -- ��ų ���� ����
		},
	},	
}