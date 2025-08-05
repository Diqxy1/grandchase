CHAR_ELESIS = 0,     -- 엘리시스
CHAR_LIRE = 1,       -- 리르
CHAR_ARME = 2,       -- 아르메
CHAR_LAS = 3,        -- 라스
CHAR_RYAN = 4,       -- 라이언
CHAR_RONAN = 5,      -- 로난
CHAR_AMY = 6,        -- 에이미
CHAR_JIN = 7,        -- 진
CHAR_SIEG,           -- 지크하트
CHAR_MARI,           -- 마리
CHAR_DIO,            -- 디오
CHAR_ZERO,           -- 제로
CHAR_LEY,            -- 레이

Equipment = 0 		 -- 장비
Goal	  = 1		 -- 목표
Normal	  = 2		 -- 일반
Fail	  = 3		 -- 꽝

BeginTime   = { 2011, 04, 13, 15 }   -- Y, M, D, H
EndTime     = { 2011, 09, 14, 10 }   -- Y, M, D, H
TornadoVersion = 0				     -- 해당 이벤트 재사용시 해당 버전을 1씩 높이는 작업 필요.
TornadoItemID = 8880				 -- 토네이도 말판 ItemID
TornadoCoinItemID = 11110			 -- 토네이도 코인 ItemID
TornadoFailCoinItemID = 22220		 -- 토네이도 Fail에 대한 ItemID
TorandoFailCoinCount = 5			 -- 토네이도 Fail ItemCount
ExposeItemToUser = 657800			 -- 처음 미니게임 실행 시 유저에게 노출 할 아이템 

InitTornado = 
{
	{
		CharType = CHAR_ELESIS,
		RewardItemList = { -- 전체 아이템 리스트.
			{ 189520, 1, -1, 0.5, }, -- Jr.타나토스 펫 제작서
			{ 99600, 1, -1, 10, }, -- 스킬 오픈 열쇠
			{ 42340, -1, 3, 6, }, -- GC클럽
			{ 42410, -1, 3, 6, }, -- 여왕의 보석
		},	
		NonFailRewardItemList = {
			{ 189520, 1, -1, 0.5, }, -- Jr.타나토스 펫 제작서
			{ 99600, 1, -1, 10, }, -- 스킬 오픈 열쇠
			{ 42340, -1, 3, 6, }, -- GC클럽
			{ 42410, -1, 3, 6, }, -- 여왕의 보석
		},
		FailRewardItemList = {		
			{ 42340, -1, 3, 6, }, -- GC클럽
			{ 42410, -1, 3, 6, }, -- 여왕의 보석
		},		
		TypeItemList = {
			{ Type = Equipment,  ItemList = { }, },
			{ Type = Goal,  ItemList = { }, },
			{ Type = Normal,  ItemList = { }, },
			{ Type = Fail,  ItemList = { }, },
		},
		FailItemList = {
			{ 189520, 1, -1 }, -- Jr.타나토스 펫 제작서
			{ 99600, 1, -1 }, -- 스킬 오픈 열쇠
		},
	},	
}