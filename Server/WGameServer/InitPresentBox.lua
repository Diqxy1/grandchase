-- 선물 상자 이벤트
BeginTime   = { 2010, 09, 14, 09 }   -- Y, M, D, H
EndTime     = { 2010, 10, 14, 10 }   -- Y, M, D, H
PresentBoxVersion = 1	-- 해당 이벤트 재사용시 해당 버전을 1씩 높이는 작업 필요.
PresentBoxItemID = 301740 -- 선물상자 UI ItemID
PresentBoxItemList = { 301750, 301760, 301770, 301780, 301790, 301800, 301810, 301820, 301830, } -- 조각 ItemList

-- 조각 아이템 교환 정보.
TradeItemList = 
{
	{ ItemID = 301750, TradeItem = { 58710, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301760, TradeItem = { 302870, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301770, TradeItem = { 58740, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301780, TradeItem = { 19360, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301790, TradeItem = { 189540, 1, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301800, TradeItem = { 114150, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301810, TradeItem = { 114160, 2, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301820, TradeItem = { 189550, 1, -1, }, },  -- { ItmeID, Factor }
	{ ItemID = 301830, TradeItem = { 70200, 2, -1, }, },   -- { ItmeID, Factor }
	
}

-- 
RewardItemList = 
{
	{ 189520, 1, -1, 1, }, -- Jr.타나토스 펫 제작서
	{ 99600, 1, -1, 1, }, -- 스킬 오픈 열쇠
	{ 42340, -1, 3, 6, }, -- GC클럽
	{ 42410, -1, 3, 6, }, -- 여왕의 보석
	{ 147930, -1, 7, 5, }, -- 실버나이츠 날개
	{ 189530, 5, -1, 7, }, -- 베르메시아의 보주
	{ 189560, 5, -1, 7, }, -- 실버랜드의 보주
	{ 189540, 5, -1, 7, }, -- 엘리아의 보주
	{ 189550, 5, -1, 7, }, -- 제니아의 보주
	{ 82050, 3, -1, 30, }, -- 챔피언 모드 입장권
	{ 189490, 1, -1, 1, }, -- Jr.가이코즈 펫 제작서
	{ 189500, 1, -1, 1, }, -- Jr.크라코 펫 제작서
	{ 189510, 1, -1, 1, }, -- Jr.카제아제 펫 제작서
	{ 301720, 30, -1, 10, }, -- 숙련 장인의 의지
	{ 302870, 15, -1, 10, }, -- 괴수의 마석
}
