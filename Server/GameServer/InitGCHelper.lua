FALSE = 0
TRUE = 1

-- ���ʽ� ����..
MaxBonusData = 99
BonusItemList = { 70200, 71440 }
BonusData =
{
    { 0, 100, 3 },

}

-- ������ ����.

GroupItem =
{
    { 58710, 58720, 58730, 109810, 114800,141750 }, -- HP
    { 58740, 58750, 58760, 109820, 114810, 206880,141760 }, -- MP
    { 12710, 17510, 19360, 28850, 28860, 51300, 35030, }, -- ����
    { 51310, 17470, }, -- ������
    { 138410, 138420, 138430, 144030, 144040, 144050, 141090, 141100, 141110, 283480, }, -- ����
    { 138440, 114150, 114160, 114170, 177560, 283470, 367430, 305880 }, -- ȸ�� ���ݷ�
    { 144060, 114180 }, -- �����
}

NonGroupItem = { }

EmoticonItem = { 71790, 71800, 71810, 71820, 71830, 71840, 71850, 71860, 71870, 71880, 71890,
                 71900, 71910, 71920, 71930, 71940, 71950, 71960, 71970, 71980, 71990, 72000,
                 72010, 72020, 72030, 72040, 72050, 72060, 72070, 72080, 72090, 72100, 72110,
                 72120, 72130, 72140, 72150, 72160, 72170, 72180, 72190, 72200, 72210, 72220,
                 72230, 72240, 72250, 72260, 72270, 72280, 80420, 123360, 123370, 138820, 106320, }

EmoticonMixRatio =
{
{	71790	,	2.97	},
{	71800	,	2.97	},
{	71810	,	2.97	},
{	71830	,	2.97	},
{	71850	,	2.97	},
{	71860	,	2.97	},
{	71870	,	2.97	},
{	71890	,	2.97	},
{	71910	,	2.97	},
{	71920	,	2.97	},
{	71930	,	2.97	},
{	71940	,	2.97	},
{	71960	,	2.97	},
{	71970	,	2.97	},
{	71990	,	2.97	},
{	72020	,	2.97	},
{	72030	,	2.97	},
{	72040	,	2.97	},
{	72050	,	2.97	},
{	72090	,	2.97	},
{	72100	,	2.9	},
{	72110	,	2.9	},
{	72130	,	2.9	},
{	72170	,	2.9	},
{	72190	,	2.9	},
{	72200	,	2.9	},
{	72210	,	2.9	},
{	72220	,	2.9	},
{	72230	,	2.9	},
{	106320	,	2.9	},
{	123360	,	2.9	},
{	123370	,	2.9	},
{	138820	,	2.9	},
{	72260	,	2.9	},
{	80420	,	0	},
{	71820	,	0	},
{	71840	,	0	},
{	71880	,	0	},
{	71900	,	0	},
{	71950	,	0	},
{	71980	,	0	},
{	72000	,	0	},
{	72010	,	0	},
{	72060	,	0	},
{	72070	,	0	},
{	72080	,	0	},
{	72120	,	0	},
{	72140	,	0	},
{	72150	,	0	},
{	72160	,	0	},
{	72180	,	0	},
{	72240	,	0	},
{	72250	,	0	},
{	72270	,	0	},
{	72280	,	0	},


}

-- �κ� Ȯ�� ������ ����
IET_FIVE    = 5
IET_TEN     = 10
IET_THIRTY  = 30
IET_SIXTY   = 60
InvenInfo =
{

    MaxInvenCapacity    = 700,   -- �κ� �ִ� ũ��.
    InvenOverflowEnable = FALSE, -- �κ��丮 �ʰ����� �������� ����

	InvenExternItem =
	{
	    {
	        InvenCapacity = IET_FIVE,
	        ItemID = { 161290, },
	    },
	    {
	        InvenCapacity = IET_TEN,
	        ItemID = { 59780, 76820, 98770, },
	    },
	    {
	        InvenCapacity = IET_THIRTY,
	        ItemID = { 43500, },
	    },
	    {
	        InvenCapacity = IET_SIXTY,
	        ItemID = { 42220, },
	    },
	}
}

-- ����.
CouponInfo = {
    { 7430, 20000 },		-- 20000 GP
	{ 100950, 10000 },		-- 10000 GP
	{ 133670, 1000 },
	{ 133680, 5000 },
	{ 133690, 10000 },
	{ 133700, 50000 },
	{ 158980, 5000 },
}

-- ĳ���� ī��

CharacterCard =
{
	{ -- ��
		ItemID = 23800,
		CharType = 3,
		CompleteMissionInfo = { },
	},
}

-- ���� ���� ����
SnowCoin = {
    BeginDate = { 2009, 04, 30, 00 },    -- Y,M,D,H
    EndDate   = { 2009, 05, 29, 0, },    -- Y,M,D,H
    CoinID    = 158310,
    CoinUID   = 0,
    TickGap   = 1000 * 60,
}

HellModeTicket =
{
    DungeonFreePass     = 62620,    -- ��� ���� ���� ���, ���� ����.
    DungeonFreePassID   = 62620,    -- ��� ���� ���� ���, ���� ����.
    CheckEnable         = TRUE,     -- ����� üũ����
    TicketID            = 82050,    -- ����� ItemID

    MaxTicketCount      = 4,        -- ������ �ִ� �ִ� ����(-1:�˻����)
    ConnectGift         = 0,        -- ���ӽ� �ִ� �����
    DayAttendGift       = 3,        -- �����⼮�� �ִ� �����
    WeekAttendGift      = 0,        -- �ְ��⼮�� �ִ� �����
    MonthAttendGift     = 0,        -- �����⼮�� �޴� �����
    EndGameTicketDec    = FALSE,    -- ���������� ���� Ƽ�� �������� ����
}

-- GCŬ�� Ȯ����
GCClubExtension =
{
    ExtensionEnable  = FALSE,   -- Ȯ���� ��뿩��
    GCClubItemID     =
    {
	    42340,   -- GCŬ�� ItemID
    },

    -- ���ӽ� ���ʽ� ���� ������ { ItemID, Factor }
    ConnectBonusItem =
    {
        { 153200, 3, -1 }, -- GCŬ�� ����(��)
        { 153210, 3, -1 }, -- GCŬ�� ����(��)
        { 153220, 3, -1 }, -- GCŬ�� ����(��)
    },
    -- ���� �⼮�� ���ʽ� ���� ������ { ItemID, Factor }
    DayAttendBonusItem =
    {
        { 151390, 1, -1 }, -- GCŬ�� ���ʽ� ����
    },
}

-- ����.
VirtualCashCouponInfo = {
	{ 131190, 2 },		-- 2 Cash
	{ 131200, 3 },		-- 3 Cash

}

OnlyOneMission =
{
    CheckEnable = TRUE,     -- ����� üũ����
    TicketID    = {},    -- ����� ItemID

    MaxTicketCount  = 1,    -- ������ �ִ� �ִ� ����(-1:�˻����)
    ConnectGift     = 0,    -- ���ӽ� �ִ� �����
    DayAttendGift   = 0,    -- �����⼮�� �ִ� �����
    WeekAttendGift  = 0,    -- �ְ��⼮�� �ִ� �����
    MonthAttendGift = 0,    -- �����⼮�� �޴� �����
    EndGameTicketDec = FALSE, -- ���������� ���� Ƽ�� �������� ����
}

-- ����� ��� ���� ����..
Drop_Limit =
{
    -- ��� ���� ������ ID, �����丮�� �ִ� ������..
    -- 0 �� normal, 1 �� ����.
    --{ ItemID = , Inven = { ItemID, Type, ItemID, Type.... } },
    { ItemID = 189490, Inven = { 189490, 1, 189570, 0 } },  -- ��������
    { ItemID = 189500, Inven = { 189500, 1, 189600, 0 } },  -- ũ����
    { ItemID = 189510, Inven = { 189510, 1, 189630, 0 } },  -- ī������
    { ItemID = 189520, Inven = { 189520, 1, 189660, 0 } },  -- Ÿ���佺
}

-- ��õ�� �ݸ��� ĳ�� �̺�Ʈ
CashCouponInfo = 
{
	ItemID = 196800,
}

-- �븸 ��ȸ�����¸��� ���� ���
TourDonation = {
    BeginDate = { 2009, 11, 19, 10 },    -- Y,M,D,H
    EndDate   = { 2010, 11, 25, 10, },   -- Y,M,D,H
    BuyCheckItem = { 301930, 301940, 301950, 301960, }		 -- �� ������ �������ѿ� �� ������ ����Ʈ.
}

-- �̺�Ʈ ���� ��� ���� Ȯ��
EventInfo = 
{
	CheckEnable = FALSE,
	StringID = { 5024, 5025, },
}

-- �̱� NTreev ��ȸ���� �ð� ����.
TournamentTime = 
{
    BeginDate = { 2009, 09, 21, 21, },    -- Y,M,D,H
    EndDate   = { 2009, 09, 22, 23, },    -- Y,M,D,H
    TickGap   = 1000 * 60,
    TournamentTicketID = 565940,
	
	-- �߰� ��ȸ���� �ɼ�
	TimeCheck = {
		CheckEnable = TRUE,
		StartHour = 10,
		EndHour = 22,
	}
}

-- ä�θ� �ű� ���� �̺�Ʈ, �Ⱓ �� ������ 1ȸ ����
ChannelingInfo =
{
    BeginDate = { 2010, 11, 09, 10, },    -- Y,M,D,H
    EndDate   = { 2010, 12, 03, 10, },    -- Y,M,D,H
    FirstConnectionReward = 
    { 
	--{ ChannelType = 3, ItemReward = { 118600, -1, 15, 149150, -1, 15,}, }, -- { ItemID, Count, Period, }
    }
}

-- Mob Grade
NORMAL_MOB      = 0
BOSS            = 1
MIDDLE_BOSS     = 2

AdditionalDropByMobGrade = {
    { NORMAL_MOB, 0.0, },
    { BOSS, 10.0, },
    { MIDDLE_BOSS, 3.0, },
}

-- �̱� ���� ����( ���� ������ ������ ���� üũ )
UserAuthIfno = 
{
    BeginDate = { 2010, 05, 04, 01 },    -- Y,M,D,H
    AuthEndDate   = { 2010, 07, 11, 10, },   -- Y,M,D,H
	PurchaseEndDate   = { 2010, 07, 20, 10, },   -- Y,M,D,H
	UserReward = { 
	},
}


-- ���� ������ ����.
NationRewardInfo = 
{
    BeginDate = { 2011, 06, 04, 01, },    -- Y,M,D,H
    EndDate   = { 2011, 12, 11, 10, },   -- Y,M,D,H
	ExpReward = 0.05,
	GPReward = 0.0,
}


CANIVAL_TICKET = 663100 -- �� ī�Ϲ� ����� 
GOLDSTAGE_TICKET = 663110 -- ��� �������� �̿��
DungeonTicket =
{
    --[[ Sample
    {
        ModeID = 8,
        EnterTicket = { CANIVAL_TICKET, 1, }, -- ItemID, NeedCount
        BossTicket = { GOLDSTAGE_TICKET, 1, }, -- ItemID, NeedCount
    },
    --]]

    { ModeID = 8, EnterTicket = { CANIVAL_TICKET, 1, }, BossTicket = { GOLDSTAGE_TICKET, 1, }, },
}

-- Kairo ���� Ư�� ItemID ���� ��� �ý���
KairoNoticeItem = 
{
	{ BoxItemID = 727910, PostItemList = { 346330, 150490, 169520, }, },
}

-- ����ġ ����
ExpPotionInfo =
{
    {
	    PercentageGrow = FALSE, -- �ۼ�Ʈ�� ���� ���� ǥ��
	    ItemID = 807690,		-- ���� ����ġ ���� ID
	    GrowRate = 1,			-- ������ ��ġ(1�� 100%)
	    GrowValue = 30000,		-- ���밪 ��ġ(PercentageGrow�� FALSE �� �� ���)
	    ExceedEXP = TRUE,		-- ������ �Ѿ�� �� ���� ����
	    ValidMinLev = 20,       -- ��� ���� �ּ� ����
	    ValidMaxLev = 80,       -- ��� ���� �ִ� ����
	},
	{
	    PercentageGrow = FALSE, -- �ۼ�Ʈ�� ���� ���� ǥ��
	    ItemID = 807700,		-- ���� ����ġ ���� ID
	    GrowRate = 1,			-- ������ ��ġ(1�� 100%)
	    GrowValue = 300000,		-- ���밪 ��ġ(PercentageGrow�� FALSE �� �� ���)
	    ExceedEXP = TRUE,		-- ������ �Ѿ�� �� ���� ����
	    ValidMinLev = 40,       -- ��� ���� �ּ� ����
	    ValidMaxLev = 80,       -- ��� ���� �ִ� ����
	},
	{
	    PercentageGrow = FALSE, -- �ۼ�Ʈ�� ���� ���� ǥ��
	    ItemID = 807710,		-- ���� ����ġ ���� ID
	    GrowRate = 1,			-- ������ ��ġ(1�� 100%)
	    GrowValue = 3000000,	-- ���밪 ��ġ(PercentageGrow�� FALSE �� �� ���)
	    ExceedEXP = TRUE,		-- ������ �Ѿ�� �� ���� ����
	    ValidMinLev = 60,       -- ��� ���� �ּ� ����
	    ValidMaxLev = 80,       -- ��� ���� �ִ� ����
	},
}

-- �����ϱ� ��� ���� ������ �߰�
PresentLimitItem = 
{
	LimitUseEnable = FALSE,
	LimitItemList = { 301930, }		 -- �� ������ �������ѿ� �� ������ ����Ʈ.	
}

--PVP �������� ���� ĳ����
PVP_Character_Disable = { 10, 11, 12, 13, }
