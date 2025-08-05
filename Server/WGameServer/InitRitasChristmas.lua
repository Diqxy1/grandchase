--[[
2012년 크리스마스 이벤트용으로 제작된 미니게임

작업자: 유성철, 이환수, 임지은, 정기섭, 곽재영
--]]

-- 이벤트 버전
Version = 1

-- 이벤트 기간 설정
BeginTime   = { 2012, 11, 27, 00 }   -- 이벤트 시작 시간(Y, M, D, H)
EndTime     = { 2013, 01, 03, 10 }   -- 이벤트 종료 시간(Y, M, D, H)
RankUpdateTime = { 01, 00 }   -- 랭킹 데이터 새로 가져오는 시간(H, m)

-- 게임 시작관련
COIN_PER_GAME = 1   -- 게임 시작시 소모되는 코인 수
COIN_SUPPLY_INTERVAL = 4 * 60   -- 코인이 충전되는 시간 간격(초)
COIN_SUPPLY_COUNT = 1   -- 한번에 충전되는 코인 수
COIN_MAX_QUANTITY = 5   -- 최대 코인 충전 수
COIN_DOUBLE_GAIN_ITEMID = 1046790   -- 일명 코인 부스터. 갖고 있으면 코인이 두배로 충전되는 아이템ID

CONTINUE_COIN_EXCHANGE_ITEMID = 1046730     -- 재도전 코인 충전 아이템ID(수량 아이템)
CONTINUE_COIN_EXCHANGE_CONSUME_COUNT = 1    -- 충전시 소모되는 재도전 코인 아이템 수
CONTINUE_COIN_EXCHANGE_RESULT_COUNT = 1     -- 충전 아이템 하나당 충전되는 재도전 코인 수

-- 플레이정보
PlayInfo = {
   SlotNum = 12,        -- 오브젝트가 생성될 수 있는 가로 개수
   BasketSize = 10,     -- 바구니 사이즈
   BasketWidth = 0.1,    -- 바구니의 넓이 (Map 넓이의 상대치 값임 %로 값으로 주면 됨)
   PlayerSpeed = 6.3,    -- 플레이어(바구니) 스피드
   CatchZoneTop = 0.83,   -- 수정시 프로그래머(환수)와 의논할것
   CatchZoneBottom = 0.93,  -- 수정시 프로그래머(환수)와 의논할것
}

-- 오브젝트 정보
OT_NORMAL = 0
OT_FEVER = 1

ObjectInfo =
{
    {
        ObjectID = 0,   -- 오브젝트ID는 항상 0부터 시작해야한다
        ObjectType = OT_NORMAL,
        DownSpeed = 120.1,  -- 속도
        Point = 50,    -- 획득 점수
        DecreaseTime = 1,   -- 타이머 감소량(초)
    },
    {
        ObjectID = 1,
        ObjectType = OT_NORMAL,
        DownSpeed = 130.1,  -- 속도
        Point = 100,    -- 획득 점수
        DecreaseTime = 1,   -- 타이머 감소량(초)
    },
    {
        ObjectID = 2,
        ObjectType = OT_NORMAL,
        DownSpeed = 150.1,  -- 속도
        Point = 150,    -- 획득 점수
        DecreaseTime = 1,   -- 타이머 감소량(초)
    },
    {
        ObjectID = 3,
        ObjectType = OT_FEVER,  -- 피버 오브젝트는 하나만 존재하도록 구현되어 있음
        DownSpeed = 300.1,  -- 속도
        Point = 700,    -- 획득 점수
        DecreaseTime = 0,   -- 타이머 감소량(초)
    },
}

BASKET_EMPTY_POINT_LIMIT = 150 * 10   -- 바구니 비우기 한번에 얻을 수 있는 최대 점수(해킹체크용)
FEVER_POINT_LIMIT = 70000   -- 피버 오브젝트로 스테이지당 얻을 수 있는 최대 점수(해킹체크용)

-- 스테이지 정보
StageInfo =
{
    {
        StageNum = 1,   -- 스테이지 번호
        TimeLimit = 120.1, -- 스테이지 제한시간(초)
        GoalCount = 25,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 0.8,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 1,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.5,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 3.1,  -- 피버타임 지속시간(초)
        ComboLimit = 3, -- 최대 목표 콤보 수
    },
    {
        StageNum = 2,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 40,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 0.9,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 1,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.3,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 3, -- 최대 목표 콤보 수
    },
    {
        StageNum = 3,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 50,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.2,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 1,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.2,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 4,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 55,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.3,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 2,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.2,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 5,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 60,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.4,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 2,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.2,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 6,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 65,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.5,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 2,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.1,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 7,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 70,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.6,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 3,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.1,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 8,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 75,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.7,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 1,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 3,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.0,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 9,   -- 스테이지 번호
        TimeLimit = 90.1, -- 스테이지 제한시간(초)
        GoalCount = 80,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 1.8,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 4,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 1.0,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
    {
        StageNum = 10,   -- 스테이지 번호
        TimeLimit = 5.1, -- 스테이지 제한시간(초)
        GoalCount = 100,    -- 획득해야 하는 오브젝트 수
        DownSpeedIncRatio = 2.8,   -- 오브젝트 낙하속도 증가비율(1.0: 기본 속도)
        ClearRewardCount = 4,   -- 클리어시 얻는 보상 수
        ContinueCoinCount = 1,  -- 이어하기시 소모되는 코인 수
        RemainTimeBonusPointPerSec = 100,   -- 남은시간 보너스 점수(초당 점수)
        GenerateSec = 0.2,      -- 일반 오브젝트를 생성시키는 텀(초)
        GenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 일반 오브젝트의 개수
        FeverGenerateSec = 0.1,      -- 피버 오브젝트를 생성시키는 텀(초)
        FeverGenerateNum = 1,      -- GenerateTime이 되었을시에 생성시킬 피버 오브젝트의 개수
        FeverTimeDuration = 4.1,  -- 피버타임 지속시간(초)
        ComboLimit = 4, -- 최대 목표 콤보 수
    },
}

-- 보상 정보
SpecialRewardInfo =
{
    { 1046700, -1, -1, 0.001 }, -- ItemID, 수량, 기간, 확률
	{ 1046740, 1, -1, 2.5 },
	{ 1056970, 1, -1, 1.499 },
	{ 1046980, 1, -1, 7 },
	{ 1047350, 1, -1, 7 },
	{ 1046780, 1, -1, 7 },
	{ 133670, 1, -1, 25 },
	{ 133680, 1, -1, 25 },
	{ 133690, 1, -1, 25 },
}

PREMIUM_KEY_ITEMID = 1046740    -- 프리미엄 상자를 열 때 필요한 열쇠 아이템(상자당 1개씩 사용됨)
PREMIUM_PER_FEVER_OBJECT_COUNT = 10   -- 프리미엄 상자 1개를 얻는데 필요한 피버타임 오브젝트 수
PremiumRewardInfo =
{
    { 1046700, -1, -1, 3 },    -- ItemID, 수량, 기간, 확률
    { 1046970, 1, -1, 7 },
	{ 1046980, 1, -1, 18 },
	{ 1047340, 1, -1, 12 },
	{ 1047350, 1, -1, 18 },
	{ 1046740, 1, -1, 12 },
	{ 1056970, 1, -1, 30 },
}

-- 클라 UI의 도움말에서 보여지는 안내용 아이템 리스트 설정
RewardViewListInfo =
{
    1046700, 1046970, 1046980, 1047340, 1047350, 1046740, 1056970,  -- 입력한 순서대로 보여진다
}
