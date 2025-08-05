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
    {58730, 58720, 58710, 109810, 114800, 175710, 162730, 149880, 308460, 308520, 367150, 547570, 436300, 746070, 946730, 946750, }, -- HP
    {58760, 58750, 58740, 20688, 109820, 114810, 175700, 162720, 149890, 308530, 547580, 436290, 746080, 946740, }, -- MP
    {12710, 51300, 35030, 17510, 19360, 28850, 28860, 745470, }, -- 수반PROTECT
    { 51310, 17470, }, -- 힐링볼
    {138410, 138420, 138430, 144030, 144040, 144050, 141090, 141100, 141110, 149180, 177230, 177240, 177250, 283480, 162740, 162750, 308040, 308540, 302070, 308550, 378420, 547600, 547610, 547620, 746090, 745490, 750490, 779570, 911880, 911890, 911900,}, -- 버프
	{ 558980, 558990, 559000, 559010, 559020, 559030, 559040, 559050, 559060, 559070, 729320, 790340, }, -- 이벤트포션
    {102130, 106310, 114150, 114160, 114170, 138440, 177560, 283470, 367160, 378410, 547590, 436280, 736310, 728180, 788410, 946760, }, -- 특수포션REJUVENATION_POTION, HP&MP
	
    {114180, 144060, 745460, }, -- 디버프CANCELATION_POTION
	{177210, }, --SKILL_POTTION2
	{177220, }, --SKILL_POTTION1
	{992660, }, --4단 필살기 스크롤
	
}

NonGroupItem = { }

EmoticonItem = { 71790, 71800, 71810, 71820, 71830, 71840, 71850, 71860, 71870, 71880, 71890,
                 71900, 71910, 71920, 71930, 71940, 71950, 71960, 71970, 71980, 71990, 72000,
                 72010, 72020, 72030, 72040, 72050, 72060, 72070, 72080, 72090, 72100, 72110,
                 72120, 72130, 72140, 72150, 72160, 72170, 72180, 72190, 72200, 72210, 72220,
                 72230, 72240, 72250, 72260, 72270, 72280, 80420, 123360, 123370, 138820, 998950 }

EmoticonMixRatio =
{
    {71790,3.4},
    {71800,3.4},
    {71810,3.4},
    {71830,3.4},
    {71850,3.4},
    {71860,3.4},
    {71870,3.4},
    {71890,3.4},
    {71910,3.4},
    {71920,3.4},
    {71930,3.3},
    {71940,3.3},
    {71960,3.3},
    {71970,3.3},
    {71990,3.3},
    {72020,3.3},
    {72030,3.3},
    {72040,3.3},
    {72050,3.3},
    {72090,3.3},
    {72100,3.3},
    {72110,3.3},
    {72130,3.3},
    {72170,3.3},
    {72190,3.3},
    {72200,3.3},
    {72210,3.3},
    {72220,3.3},
    {72230,3.3},
    {72260,3.3},
    {71820,0},
    {71840,0},
    {71880,0},
    {71900,0},
    {71950,0},
    {71980,0},
    {72000,0},
    {72010,0},
    {72060,0},
    {72070,0},
    {72080,0},
    {72120,0},
    {72140,0},
    {72150,0},
    {72160,0},
    {72180,0},
    {72240,0},
    {72250,0},
    {72270,0},
    {72280,0},

}

-- 인벤 확장 아이템 정보
IET_FIVE    = 5
IET_TEN     = 10
IET_THIRTY  = 30
IET_SIXTY   = 60
InvenInfo =
{

    MaxInvenCapacity    = 900,   -- 인벤 최대 크기.
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
	{ 378370, 1000000 },	-- 100만 GP
}

-- 캐릭터 카드
CharacterCard = { 
	{
	-- 엘리시스
		ItemID = 990690,
		CharType = 0,
		CompleteMissionInfo = { },
	},
	{ -- 리르
		ItemID = 990700,
		CharType = 1,
		CompleteMissionInfo = { },
	},
	{ -- 아르메
		ItemID = 990710,
		CharType = 2,
		CompleteMissionInfo = { },
	},
	{ -- 라스
		ItemID = 990720,
		CharType = 3,
		CompleteMissionInfo = { 508680, 508670, },
	},
	{ -- 라이언
		ItemID = 990730,
		CharType = 4,
		CompleteMissionInfo = { 508690, 508700, 508710, },
	},
	{ -- 로난
		ItemID = 990740,
		CharType = 5,
		CompleteMissionInfo = { 508660, 508650,  },
	},
	{ -- 에이미
		ItemID = 990750,
		CharType = 6,
		CompleteMissionInfo = { 508790, 508800, 508810, },
	},
	{ -- 진
		ItemID = 990760,
		CharType = 7,
		CompleteMissionInfo = { 508740, 508750, 508760, },
	},
	{ -- 지크하트
		ItemID = 990770,
		CharType = 8,
		CompleteMissionInfo = { },
	},
	{ -- 마리
		ItemID = 990780,
		CharType = 9,
		CompleteMissionInfo = { 508890, 508900, 508910, },
	},
	{ -- 디오
		ItemID = 990790,
		CharType = 10,
		CompleteMissionInfo = { 508940, 508950, 508960, },
	},
	{ -- 제로
		ItemID = 990800,
		CharType = 11,
		CompleteMissionInfo = { 508990, 509000, 509010, },
	},
	{ -- 레이
		ItemID = 990810,
		CharType = 12,
		CompleteMissionInfo = { 525440, 525450, 525460, },
	},
	{ -- 루퍼스
		ItemID = 990820,
		CharType = 13,
		CompleteMissionInfo = { },
	},
	{ -- 린
		ItemID = 990830,
		CharType = 14,
		CompleteMissionInfo = { },
	},
	{ -- 아신
		ItemID = 990840,
		CharType = 15,
		CompleteMissionInfo = { },
	},
	{ -- 라임
		ItemID = 990850,
		CharType = 16,
		CompleteMissionInfo = { },
	},
}

-- 눈꽃 코인 관련
--6월 28일에 점령전 코인 관련해서 세팅을 합니다. SP전달을 위한 작업입니다. 한국 서비스 기획담당자 잘 기억해두세요.
--시작과 끝은 점령전 클로즈베타를 기준으로 2주로 넉넉하게 잡아두었습니다. 
SnowCoin = {
    BeginDate = { 2012, 04, 28, 07 },    -- Y,M,D,H 테스트를 위한 목요일 오픈 시간 이전으로 세팅
    EndDate   = { 2012, 05, 02, 10, },    -- Y,M,D,H 2주 뒤 서버점검을 기준으로 세팅
    CoinID    = 901320, --점령전 획득 포인트 아이템
    CoinUID   = 0,
    TickGap   = 1000 * 60,
}

HellModeTicket =
{
    DungeonFreePass     = 62620,    -- 모든 던전 입장 허용, 헬모드 포함.
    CheckEnable         = TRUE,     -- 입장권 체크여부
    TicketID            = 82050,    -- 입장권 ItemID

    MaxTicketCount      = 4,        -- 가질수 있는 최대 수량(-1:검사안함)
    ConnectGift         = 0,        -- 접속시 주는 입장권
    DayAttendGift       = 3,        -- 일일출석시 주는 입장권
    WeekAttendGift      = 0,        -- 주간출석시 주는 입장권
    MonthAttendGift     = 0,        -- 월간출석시 받는 입장권
    EndGameTicketDec    = TRUE,    -- 게임종료후 헬모드 티켓 감소할지 여부
    DungeonFreePass     = FALSE,     -- 던전 프리패스 아이템 소유시 챔피언 모드 입장 가능 
    DungeonFreePassID = 62620,
}

-- GC클럽 확장기능
GT_GCCLUB = 1
GT_GCBLOG = 2
GCClubExtension =
{
    ExtensionEnable  = FALSE,   -- 확장기능 사용여부
    GCClubItemID     =
    {
		{ 42340, GT_GCCLUB },
        { 328080, GT_GCBLOG },
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
    BuyCheckItem = { 301750, 301780, 301810, 301930, 301940, 301950, 301960, 565650, },	-- 웹 아이템 구매제한에 들어갈 아이템 리스트.
}

-- 이벤트 내용 출력 여부 확인
EventInfo = 
{
	CheckEnable = TRUE,
	StringID = { 6018, },
}

-- 미국 NTreev 대회서버 시간 세팅.
TournamentTime = 
{
    BeginDate = { 2010, 06, 21, 00, },    -- Y,M,D,H
    EndDate   = { 2010, 06, 30, 22, },    -- Y,M,D,H
    TickGap   = 1000 * 60,
    TournamentTicketID = 565940,
	
	-- 추가 대회서버 옵션
	TimeCheck = {
		CheckEnable = FALSE,
		StartHour = 12, -- 12:00 ~ 17:59 event on
		EndHour = 17,
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
    { BOSS, 0.0, },
    { MIDDLE_BOSS, 0.0, },
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
    BeginDate = { 2010, 06, 04, 01 },    -- Y,M,D,H
    EndDate   = { 2010, 12, 11, 10, },   -- Y,M,D,H
	ExpReward = 0.05,
	GPReward = 0.0,
}

-- 던전 입장권 체크
CANIVAL_TICKET = 663100 -- 더 카니발 입장권 
GOLDSTAGE_TICKET = 663110 -- 골드 스테이지 이용권
ANGRY_TICKET = 746060 -- 앵그리 보스 입장권
TRAIN_TICKET = 807640	-- 철도 승차권
GOLDTRAIN_TICKET = 808220	-- 명계열차 골든 티켓
HUNGRYPET_TICKET = 881320 -- 배고픈 펫들의 반란 티켓

DungeonTicket =
{
    --[[ Sample
    {
        ModeID = 8,
        EnterTicket = { CANIVAL_TICKET, 1, }, -- ItemID, NeedCount
        BossTicket = { GOLDSTAGE_TICKET, 1, }, -- ItemID, NeedCount
    },
    --]]

--	{ ModeID = 8, EnterTicket = { CANIVAL_TICKET, 1, }, BossTicket = { GOLDSTAGE_TICKET, 1, }, },
    { ModeID = 67, EnterTicket = { CANIVAL_TICKET, 1, }, BossTicket = { GOLDSTAGE_TICKET, 1, }, },
	{ ModeID = 68, EnterTicket = { ANGRY_TICKET, 1, }, BossTicket = {}, },
	{ ModeID = 69, EnterTicket = { CANIVAL_TICKET, 1, }, BossTicket = { GOLDSTAGE_TICKET, 1, }, },
	{ ModeID = 71, EnterTicket = { TRAIN_TICKET, 1, }, BossTicket = { GOLDTRAIN_TICKET, 1, }, },
	{ ModeID = 72, EnterTicket = { HUNGRYPET_TICKET, 1, }, BossTicket = {}, },
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
	    GrowValue = 3800,		-- 절대값 수치(PercentageGrow가 FALSE 일 때 사용)
	    ExceedEXP = TRUE,		-- 레벨을 넘어섰을 때 적용 여부
	    ValidMinLev = 20,       -- 사용 가능 최소 레벨
	    ValidMaxLev = 80,       -- 사용 가능 최대 레벨
	},
	{
	    PercentageGrow = FALSE, -- 퍼센트로 증가 유무 표시
	    ItemID = 807700,		-- 증가 경험치 포션 ID
	    GrowRate = 1,			-- 증가율 수치(1은 100%)
	    GrowValue = 35000,		-- 절대값 수치(PercentageGrow가 FALSE 일 때 사용)
	    ExceedEXP = TRUE,		-- 레벨을 넘어섰을 때 적용 여부
	    ValidMinLev = 40,       -- 사용 가능 최소 레벨
	    ValidMaxLev = 80,       -- 사용 가능 최대 레벨
	},
	{
	    PercentageGrow = FALSE, -- 퍼센트로 증가 유무 표시
	    ItemID = 807710,		-- 증가 경험치 포션 ID
	    GrowRate = 1,			-- 증가율 수치(1은 100%)
	    GrowValue = 350000,	-- 절대값 수치(PercentageGrow가 FALSE 일 때 사용)
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

-- 신캐릭터 지급 팝업 알림 for 아신.
NewCharEvent = 
{
    BeginTime = { 2012, 6, 30, 01 },    -- Y,M,D,H
    EndTime   = { 2012, 7, 19, 06, },   -- Y,M,D,H
    NewCharID = 16, -- LIME
}

AttendGetChar = 
{
	BeginDate = { 2012, 5, 8, 01, }, -- Y,M,D,H
	EndDate = { 2012, 5, 25, 00, }, -- Y,M,D,H
	Reward = { {909140, 1, -1}, },
	AttendCount = 7,
} 

-- 특정 레벨 캐릭터 아이템 지급.
CharLevelEvent = 
{
	Version = 0,
    BeginTime = { 2012, 9, 10, 01 },    -- Y,M,D,H
    EndTime   = { 2012, 9, 27, 10, },   -- Y,M,D,H
	RewardItem = {
		-- CharType, ItemInfo{ ItemID, Duration, Period, GradeID.., }
	{ CharType = 0, CharLevel = 78, ItemInfo = { 395840, -1, -1, 3, 395850, -1, -1, 3, 395860, -1, -1, 3, 395870, -1, -1, 4, 395880, -1, -1, 3, 395890, -1, -1, 3, 1004960, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 1, CharLevel = 78, ItemInfo = { 395910, -1, -1, 3, 395920, -1, -1, 3, 395930, -1, -1, 3, 395940, -1, -1, 4, 395950, -1, -1, 3, 395960, -1, -1, 3, 1004970, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 2, CharLevel = 78, ItemInfo = { 395980, -1, -1, 3, 395990, -1, -1, 3, 396000, -1, -1, 3, 396010, -1, -1, 4, 396020, -1, -1, 3, 396030, -1, -1, 3, 1004980, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 3, CharLevel = 78, ItemInfo = { 396050, -1, -1, 3, 396060, -1, -1, 3, 396070, -1, -1, 3, 396080, -1, -1, 4, 396090, -1, -1, 3, 396100, -1, -1, 3, 1004990, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 4, CharLevel = 78, ItemInfo = { 396120, -1, -1, 3, 396130, -1, -1, 3, 396140, -1, -1, 3, 396150, -1, -1, 4, 396160, -1, -1, 3, 396170, -1, -1, 3, 1005000, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 5, CharLevel = 78, ItemInfo = { 396190, -1, -1, 3, 396200, -1, -1, 3, 396210, -1, -1, 3, 396220, -1, -1, 4, 396230, -1, -1, 3, 396240, -1, -1, 3, 1005010, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 6, CharLevel = 78, ItemInfo = { 396260, -1, -1, 3, 396270, -1, -1, 3, 396280, -1, -1, 3, 396290, -1, -1, 4, 396300, -1, -1, 3, 396310, -1, -1, 3, 1005020, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 7, CharLevel = 78, ItemInfo = { 396330, -1, -1, 3, 396340, -1, -1, 3, 396350, -1, -1, 3, 396360, -1, -1, 4, 396370, -1, -1, 3, 396380, -1, -1, 3, 1005030, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 8, CharLevel = 78, ItemInfo = { 396400, -1, -1, 3, 396410, -1, -1, 3, 396420, -1, -1, 3, 396430, -1, -1, 4, 396440, -1, -1, 3, 396450, -1, -1, 3, 1005040, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 9, CharLevel = 78, ItemInfo = { 396470, -1, -1, 3, 396480, -1, -1, 3, 396490, -1, -1, 3, 396500, -1, -1, 4, 396510, -1, -1, 3, 396520, -1, -1, 3, 1005050, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 10, CharLevel = 78, ItemInfo = { 396540, -1, -1, 3, 396550, -1, -1, 3, 396560, -1, -1, 3, 396570, -1, -1, 4, 396580, -1, -1, 3, 396590, -1, -1, 3, 1005060, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 11, CharLevel = 78, ItemInfo = { 454280, -1, -1, 3, 454290, -1, -1, 3, 454300, -1, -1, 3, 454310, -1, -1, 4, 454320, -1, -1, 3, 454330, -1, -1, 3, 1005070, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 12, CharLevel = 78, ItemInfo = { 520330, -1, -1, 3, 520340, -1, -1, 3, 520350, -1, -1, 3, 520360, -1, -1, 4, 520370, -1, -1, 3, 520380, -1, -1, 3, 1005080, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 13, CharLevel = 78, ItemInfo = { 692400, -1, -1, 3, 692410, -1, -1, 3, 692420, -1, -1, 3, 692430, -1, -1, 4, 692440, -1, -1, 3, 692450, -1, -1, 3, 1005090, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 14, CharLevel = 78, ItemInfo = { 706790, -1, -1, 3, 706800, -1, -1, 3, 706810, -1, -1, 3, 706820, -1, -1, 4, 706830, -1, -1, 3, 706840, -1, -1, 3, 1005100, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 15, CharLevel = 78, ItemInfo = { 861470, -1, -1, 3, 861480, -1, -1, 3, 861490, -1, -1, 3, 861500, -1, -1, 4, 861510, -1, -1, 3, 861520, -1, -1, 3, 1005110, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	{ CharType = 16, CharLevel = 78, ItemInfo = { 932650, -1, -1, 3, 932660, -1, -1, 3, 932670, -1, -1, 3, 932680, -1, -1, 4, 932690, -1, -1, 3, 932700, -1, -1, 3, 1005120, 1, -1, 4, 537390, 1, -1, 2, 627390, 1, -1, 4, 650320, 1, -1, 4, }, },
	}
}

--PVP 서버에서 닫을 캐릭터
PVP_Character_Disable = { } --{ 10, 11, 12, 13, }

CharLevelEvent = {
	Version = 0,
        BeginTime = { 2012, 5, 8, 01, },    -- Y,M,D,H
        EndTime   = { 2012, 5, 25, 00, },   -- Y,M,D,H
	RewardItem = {},
}
