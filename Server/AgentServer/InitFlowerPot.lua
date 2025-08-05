-- 씨앗
InvenLimitSeedCount = 20 -- 인벤토리 최대 보유할 수 있는 씨앗의 개수
DailyMaxSeedCount = 120 -- 일일 획득할 수 있는 최대 씨앗의 개수
DailyInitSeedCount = 20 -- 일일 초기화되는 씨앗 개수 (최소수량 보장)
SeedDropTime = 1 -- 남의 아지트에서 씨앗을 받을때까지 걸리는 체류 시간(분)
SeedDropCount = 5 -- 남의 아지트에서 체류시간에 따른 씨앗 획득 개수

AgitCashPotCount = 2 -- 아지트 배치가능한 캐쉬화분 개수
InvenLimitFlowerCount = 99999 -- 인벤토리 최대 보유할 수 있는 꽃의 개수

-- SEED_TYPE
ST_NONE = -1 -- 안심어진 상태
ST_FAST = 0 -- 예쁘게
ST_NORMAL = 1 -- 정성스럽게
ST_BEST = 2 -- 사랑듬뿍
-- 씨앗심기 설정
FlowerPot = {
	{
		SeedType = ST_FAST, -- 씨앗심는 타입
		NeedSeedCount = 1, -- 소비씨앗
		GrowTime = 1, -- 성장시간(분)
		FlowerCount = 1, -- 성장완료시 수확되는 꽃의 개수
		BonusCount = 1, -- 수확시 획득가능한 보너스 꽃의 개수
	},
	{
		SeedType = ST_NORMAL, -- 씨앗심는 타입
		NeedSeedCount = 1, -- 소비씨앗
		GrowTime = 2, -- 성장시간(분)
		FlowerCount = 5, -- 성장완료시 수확되는 꽃의 개수
		BonusCount = 2, -- 수확시 획득가능한 보너스 꽃의 개수
	},
	{
		SeedType = ST_BEST, -- 씨앗심는 타입
		NeedSeedCount = 1, -- 소비씨앗
		GrowTime = 3, -- 성장시간(분)
		FlowerCount = 10, -- 성장완료시 수확되는 꽃의 개수
		BonusCount = 3, -- 수확시 획득가능한 보너스 꽃의 개수
	},
}

-- ItemID
SeedItemID = 605260 -- 씨앗 ItemID
FlowerItemID = 605270 -- 꽃 ItemID
NormalPotItemID = { 605280, } -- 일반화분 ItemID
CashPotItemID = { 605290, } -- 캐쉬 화분 ItemID
