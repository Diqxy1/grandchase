EventList =
{
    -- Sample
--[[
    {
        -- std::pair<EventID,CharType> prKey
        EventID = 49,
        CharType = -1,
        
        -- Custum Data(can't set in D-bot)
        ItemIDA = {
            {
                charType = 0,
                Equips = { 6030,6040,6050,6060,6070,6080, },
            }
        },

        ItemIDB = {
            {
                charType = 1,
                Equips = { 6090,6100,6110,6120,6130,6140, },
            }
        },
        SetModeList = {},
        PartyNum = -1,
        ActiveRate = 1.0,
    },
]]--
	-- 진과 함께 하면..
	{
    	EventID = 72,     -- Dbot과 같이 설정(식별키)
	    CharType = 7,   -- 이 캐릭터(7:진)와 같이 플레이한 파티원은 보너스를 받는다. Dbot과 같이 설정(식별키)
	    ItemIDA = {},
	    ItemIDB = {},
	    SetModeList = {},
	    SetMapList = {},
	    PartyNum = -1,
	    -- 파티원(진 제외)은 50프로의 Exp/Gp보너스를 받는다.
	    -- 진은 Dbot에서 설정한 수치의 보너스를 받는다.(=진과 파티원의 보너스를 다르게 설정 가능하다)
	    PartyExpBonus = 0.5,
	    PartyGpBonus = 0.5,
	    ActiveRate = 1.0,
	},
    
}

