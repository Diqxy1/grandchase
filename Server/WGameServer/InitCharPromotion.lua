CharUpdatePromotion = 
{
	{
		ItemID = 366680, -- 전직 마법서 ItemID
		EnablePromotion = 1, -- 특정 전직만 가능, 1차 전직 가능.
		CharMinLevel = 
		{
			{ CharType = 0, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 1, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 2, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 3, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 4, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 5, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 6, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 7, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 8, MinLevel = 20 }, -- CharType, MinLevel
			{ CharType = 9, MinLevel = 20 }, -- CharType, MinLevel
		},
		MissionIDList = -- 해당 전직 수행후 DB에 등록해 줘야 하는 MissionID
		{
			{ CharType = 0,MissionID = 334610}, --CharType,MissionID
			{ CharType = 1,MissionID = 334670}, --CharType,MissionID
			{ CharType = 2,MissionID = 334730}, --CharType,MissionID
			{ CharType = 3,MissionID = 334790}, --CharType,MissionID
			{ CharType = 4,MissionID = 334850}, --CharType,MissionID
			{ CharType = 5,MissionID = 334910}, --CharType,MissionID
			{ CharType = 6,MissionID = 334970}, --CharType,MissionID
			{ CharType = 7,MissionID = 335030}, --CharType,MissionID
			{ CharType = 8,MissionID = 335090}, --CharType,MissionID
			{ CharType = 9,MissionID = 335150}, --CharType,MissionID
		},
		CharReward = 
		{
			{ CharType = 0, Reward = {336420,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 1, Reward = {336430,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 2, Reward = {336440,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 3, Reward = {336450,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 4, Reward = {336470,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 5, Reward = {336480,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 6, Reward = {336490,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 7, Reward = {336510,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 8, Reward = {336530,-1,-1,  }, }, -- Reward={ItemID, Count, Period}
			{ CharType = 9, Reward = {336550,-1,-1,  }, }, -- Reward={ItemID, Count, Period}

		},
	},
}
