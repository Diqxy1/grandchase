FALSE = 0
TRUE = 1

-- 보너스 관련..
MaxBonusData = 99
BonusItemList = { 70200, 71440 }
BonusData =
{
    { 0, 100, 3 },

}

-- 퀵슬롯 관련.

GroupItem =
{
    { 58710, 58720, 58730, 109810, 114800,141750 }, -- HP
    { 58740, 58750, 58760, 109820, 114810, 206880,141760 }, -- MP
    { 12710, 17510, 19360, 28850, 28860, 51300, 35030, }, -- 수반
    { 51310, 17470, }, -- 힐링볼
    { 138410, 138420, 138430, 144030, 144040, 144050, 141090, 141100, 141110, 283480, }, -- 버프
    { 138440, 114150, 114160, 114170, 177560, 283470, 367430, 305880 }, -- 회복 초콜렛
    { 144060, 114180 }, -- 디버프
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

-- 인벤 확장 아이템 정보
IET_FIVE    = 5
IET_TEN     = 10
IET_THIRTY  = 30
IET_SIXTY   = 60
InvenInfo =
{

    MaxInvenCapacity    = 700,   -- 인벤 최대 크기.
    InvenOverflowEnable = FALSE, -- 인벤토리 초과증설 가능한지 여부

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

-- 쿠폰.
CouponInfo = {
    { 7430, 20000 },		-- 20000 GP
	{ 100950, 10000 },		-- 10000 GP
	{ 133670, 1000 },
	{ 133680, 5000 },
	{ 133690, 10000 },
	{ 133700, 50000 },
	{ 158980, 5000 },
}

-- 캐릭터 카드

CharacterCard =
{
	{ -- 라스
		ItemID = 23800,
		CharType = 3,
		CompleteMissionInfo = { },
	},
}

-- 눈꽃 코인 관련
SnowCoin = {
    BeginDate = { 2009, 04, 30, 00 },    -- Y,M,D,H
    EndDate   = { 2009, 05, 29, 0, },    -- Y,M,D,H
    CoinID    = 158310,
    CoinUID   = 0,
    TickGap   = 1000 * 60,
}

HellModeTicket =
{
    DungeonFreePass     = 62620,    -- 모든 던전 입장 허용, 헬모드 포함.
    DungeonFreePassID   = 62620,    -- 모든 던전 입장 허용, 헬모드 포함.
    CheckEnable         = TRUE,     -- 입장권 체크여부
    TicketID            = 82050,    -- 입장권 ItemID

    MaxTicketCount      = 4,        -- 가질수 있는 최대 수량(-1:검사안함)
    ConnectGift         = 0,        -- 접속시 주는 입장권
    DayAttendGift       = 3,        -- 일일출석시 주는 입장권
    WeekAttendGift      = 0,        -- 주간출석시 주는 입장권
    MonthAttendGift     = 0,        -- 월간출석시 받는 입장권
    EndGameTicketDec    = FALSE,    -- 게임종료후 헬모드 티켓 감소할지 여부
}

-- GC클럽 확장기능
GCClubExtension =
{
    ExtensionEnable  = FALSE,   -- 확장기능 사용여부
    GCClubItemID     =
    {
	    42340,   -- GC클럽 ItemID
    },

    -- 접속시 보너스 지급 아이템 { ItemID, Factor }
    ConnectBonusItem =
    {
        { 153200, 3, -1 }, -- GC클럽 포션(소)
        { 153210, 3, -1 }, -- GC클럽 포션(중)
        { 153220, 3, -1 }, -- GC클럽 포션(대)
    },
    -- 일일 출석시 보너스 지급 아이템 { ItemID, Factor }
    DayAttendBonusItem =
    {
        { 151390, 1, -1 }, -- GC클럽 보너스 상자
    },
}

-- 쿠폰.
VirtualCashCouponInfo = {
	{ 131190, 2 },		-- 2 Cash
	{ 131200, 3 },		-- 3 Cash

}

OnlyOneMission =
{
    CheckEnable = TRUE,     -- 입장권 체크여부
    TicketID    = {},    -- 입장권 ItemID

    MaxTicketCount  = 1,    -- 가질수 있는 최대 수량(-1:검사안함)
    ConnectGift     = 0,    -- 접속시 주는 입장권
    DayAttendGift   = 0,    -- 일일출석시 주는 입장권
    WeekAttendGift  = 0,    -- 주간출석시 주는 입장권
    MonthAttendGift = 0,    -- 월간출석시 받는 입장권
    EndGameTicketDec = FALSE, -- 게임종료후 헬모드 티켓 감소할지 여부
}

-- 대륙펫 드랍 제한 관련..
Drop_Limit =
{
    -- 드랍 받을 아이템 ID, 인젠토리에 있는 아이템..
    -- 0 은 normal, 1 은 수량.
    --{ ItemID = , Inven = { ItemID, Type, ItemID, Type.... } },
    { ItemID = 189490, Inven = { 189490, 1, 189570, 0 } },  -- 가이코즈
    { ItemID = 189500, Inven = { 189500, 1, 189600, 0 } },  -- 크라코
    { ItemID = 189510, Inven = { 189510, 1, 189630, 0 } },  -- 카제아제
    { ItemID = 189520, Inven = { 189520, 1, 189660, 0 } },  -- 타나토스
}

-- 파천왕 넷마블 캐쉬 이벤트
CashCouponInfo = 
{
	ItemID = 196800,
}

-- 대만 대회참가승리팀 예상 기부
TourDonation = {
    BeginDate = { 2009, 11, 19, 10 },    -- Y,M,D,H
    EndDate   = { 2010, 11, 25, 10, },   -- Y,M,D,H
    BuyCheckItem = { 301930, 301940, 301950, 301960, }		 -- 웹 아이템 구매제한에 들어갈 아이템 리스트.
}

-- 이벤트 내용 출력 여부 확인
EventInfo = 
{
	CheckEnable = FALSE,
	StringID = { 5024, 5025, },
}

-- 미국 NTreev 대회서버 시간 세팅.
TournamentTime = 
{
    BeginDate = { 2009, 09, 21, 21, },    -- Y,M,D,H
    EndDate   = { 2009, 09, 22, 23, },    -- Y,M,D,H
    TickGap   = 1000 * 60,
    TournamentTicketID = 565940,
	
	-- 추가 대회서버 옵션
	TimeCheck = {
		CheckEnable = TRUE,
		StartHour = 10,
		EndHour = 22,
	}
}

-- 채널링 신규 유저 이벤트, 기간 내 아이템 1회 지급
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

-- 미국 유저 인증( 남미 서버에 들어오는 유저 체크 )
UserAuthIfno = 
{
    BeginDate = { 2010, 05, 04, 01 },    -- Y,M,D,H
    AuthEndDate   = { 2010, 07, 11, 10, },   -- Y,M,D,H
	PurchaseEndDate   = { 2010, 07, 20, 10, },   -- Y,M,D,H
	UserReward = { 
	},
}


-- 남미 국가별 혜택.
NationRewardInfo = 
{
    BeginDate = { 2011, 06, 04, 01, },    -- Y,M,D,H
    EndDate   = { 2011, 12, 11, 10, },   -- Y,M,D,H
	ExpReward = 0.05,
	GPReward = 0.0,
}


CANIVAL_TICKET = 663100 -- 더 카니발 입장권 
GOLDSTAGE_TICKET = 663110 -- 골드 스테이지 이용권
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

-- Kairo 상자 특정 ItemID 공지 출력 시스템
KairoNoticeItem = 
{
	{ BoxItemID = 727910, PostItemList = { 346330, 150490, 169520, }, },
}

-- 경험치 포션
ExpPotionInfo =
{
    {
	    PercentageGrow = FALSE, -- 퍼센트로 증가 유무 표시
	    ItemID = 807690,		-- 증가 경험치 포션 ID
	    GrowRate = 1,			-- 증가율 수치(1은 100%)
	    GrowValue = 30000,		-- 절대값 수치(PercentageGrow가 FALSE 일 때 사용)
	    ExceedEXP = TRUE,		-- 레벨을 넘어섰을 때 적용 여부
	    ValidMinLev = 20,       -- 사용 가능 최소 레벨
	    ValidMaxLev = 80,       -- 사용 가능 최대 레벨
	},
	{
	    PercentageGrow = FALSE, -- 퍼센트로 증가 유무 표시
	    ItemID = 807700,		-- 증가 경험치 포션 ID
	    GrowRate = 1,			-- 증가율 수치(1은 100%)
	    GrowValue = 300000,		-- 절대값 수치(PercentageGrow가 FALSE 일 때 사용)
	    ExceedEXP = TRUE,		-- 레벨을 넘어섰을 때 적용 여부
	    ValidMinLev = 40,       -- 사용 가능 최소 레벨
	    ValidMaxLev = 80,       -- 사용 가능 최대 레벨
	},
	{
	    PercentageGrow = FALSE, -- 퍼센트로 증가 유무 표시
	    ItemID = 807710,		-- 증가 경험치 포션 ID
	    GrowRate = 1,			-- 증가율 수치(1은 100%)
	    GrowValue = 3000000,	-- 절대값 수치(PercentageGrow가 FALSE 일 때 사용)
	    ExceedEXP = TRUE,		-- 레벨을 넘어섰을 때 적용 여부
	    ValidMinLev = 60,       -- 사용 가능 최소 레벨
	    ValidMaxLev = 80,       -- 사용 가능 최대 레벨
	},
}

-- 선물하기 기능 제한 아이템 추가
PresentLimitItem = 
{
	LimitUseEnable = FALSE,
	LimitItemList = { 301930, }		 -- 웹 아이템 구매제한에 들어갈 아이템 리스트.	
}

--PVP 서버에서 닫을 캐릭터
PVP_Character_Disable = { 10, 11, 12, 13, }
