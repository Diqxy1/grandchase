TOTAL      = 0 	-- 전체유저 대상
PUBLISHER1 = 1	-- 채널타입 1 ( 넷마블,  펀박스 )
PUBLISHER2 = 2  -- 채널타입 2 ( 투니랜드, 플레이파크 )
PUBLISHER3 = 3  -- 채널타입 3 ( 네이트.., )

-- 채널링 타입에 따른 유저 이벤트

-- 이벤트 기간
BeginTime   = { 2010, 12, 09, 10 }   -- Y, M, D, H
EndTime     = { 2011, 01, 07, 01 }   -- Y, M, D, H

-- 매일 접속시 받는 아이템
DailyConnectionReward = 
{
	-- Reward = { ItemID, Duration, Period }
	--{ ChannelType = 2, Reward = { 366530, 1, -1, 366650, 1, -1 }, },

}

-- 이벤트 기간동안 한번만 받는 아이템.
-- DB에 이벤트의 시작날짜를 기준으로 받았는지 안받았는지를 체크한다.
OneConnectionReward = 
{
	-- Reward = { ItemID, Duration, Period }
	{ ChannelType = 2, Reward = { 42410, -1, 30, 19360, 10, -1, 58710, 10, -1, 58740, 10, -1, }, },
}

-- 신규 유저 체크 시간.
NewBeginTime = { 2010, 10, 07, 10 }   -- Y, M, D, H
NewConnectionReward =  -- 신규 유저에게만 지급하는 아이템.
{
	-- Reward = { ItemID, Duration, Period }
	-- { ChannelType = 0, Reward = { 378290, 1, -1, 378300, 1, -1, 378310, 1, -1, 378320, 1, -1, }, },
}

-- ChannelType별 추가 경험치, SPX 배율.
-- { ChaneelType, Ratio }
ChannelTypeRatio = {
--	{
--		ChannelType = TOTAL,
--		Ratio = 
--		{
--			Exp         = 0.3,      -- 추가 Exp
--			Gp          = 0.0,      -- 추가 Gp
--		}
--	}
}

-- Pet 지급( Pet아이템의 경우 PC방 아이템처럼 저장되지 않도록 작업한다. )
-- 2147484000부터 생성.
PetConnectionReward = 
{
    {
        ChannelType = TOTAL,
        Default = {
            {
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} 장착아이템( 스킬 정보. )
            },
            {
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} 장착아이템( 스킬 정보. )
            },
        },
        Event = {
            {
                Date = { 12, 29 }, -- 월,일
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} 장착아이템( 스킬 정보. )
            },
            {
                Date = { 1, 1 }, -- 월,일
                PetItemID = 218800,
                PetItemUID = 2147484000,
                PetStrName = "str_10",
                PetEquipItem = { 218810, 127, 2147484001 }, -- { ItemID, Factor, ItemUID} 장착아이템( 스킬 정보. )
            },
        },
    },
}
