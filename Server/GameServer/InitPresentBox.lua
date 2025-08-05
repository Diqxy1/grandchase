-- 선물 상자 이벤트
BeginTime   = { 2009, 10, 06, 09 }   -- Y, M, D, H
EndTime     = { 2010, 11, 03, 21 }   -- Y, M, D, H
PresentBoxVersion = 1	-- 해당 이벤트 재사용시 해당 버전을 1씩 높이는 작업 필요.
PresentBoxItemID = 301740 -- 선물상자 UI ItemID
PresentBoxItemList = { 301750, 301760, 301770, 301780, 301790, 301800, 301810, 301820, 301830, } -- 조각 ItemList

-- 조각 아이템 교환 정보.
TradeItemList = 
{
	{ ItemID = 301750, TradeItem = { 141110, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301760, TradeItem = { 141090, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301770, TradeItem = { 141100, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301780, TradeItem = { 114180, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301790, TradeItem = { 114150, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301800, TradeItem = { 114160, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301810, TradeItem = { 302070, 1, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301820, TradeItem = { 175940, 1, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301830, TradeItem = { 71440, 1, -1, }, },   -- { ItmeID, Factor }
	
}

-- 
RewardItemList = 
{
	{ 302070, 5, -1, 20, }, -- 2주년 버프포션
	{ 302080, 1, -1, 20, }, -- 일반 몬스터 인형 상자
	{ 147930, 7, -1, 20, }, -- 실버나이츠 날개
	{ 149600, 7, -1, 15, }, -- 전장의 영웅 날개
	{ 302090, 1, -1, 10, }, -- 고급 몬스터 인형 상자
	{ 302100, 1, -1, 10, }, -- GC클럽 액세서리 상자
	{ 149600, -1, -1, 3.7, }, -- 전장의 영웅 날개
	{ 197370, -1, -1, 1.3, }, -- Pet Knight Master
}