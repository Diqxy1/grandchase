require( "Enum.stg" )

TRUE = 1
FALSE = 0

ItemList = {
	--[[ Sample
	{
		ItemID = 8880,
		BuffID = -1, -- 적용될 버프ID (클라이언트 사용)
		GroupID = -1, -- 아이템 그룹ID
		PriorityIndex = 0, -- 그룹내 적용우선순위 (0이 가장 높음)
		All = false, -- true : 파티적용, false : 혼자적용
		ModeCategory = { GC_GMC_DUNGEON, GC_GMC_MATCH, GC_GMC_GUILD_BATTLE, }, -- 버프 적용되는 게임카테고리
	},
	]]--

	{
		ItemID = 558860, -- 참전용사의 은빛 증표
		BuffID = -1,
		GroupID = -1,
		PriorityIndex = 2,
		All = FALSE,
		ModeCategory = { GC_GMC_DUNGEON, },
	},
	{
		ItemID = 558870, -- 참전용사의 금빛 증표
		BuffID = -1,
		GroupID = -1,
		PriorityIndex = 1,
		All = FALSE,
		ModeCategory = { GC_GMC_DUNGEON, },
	},
	{
		ItemID = 558880, -- 참전용사의 무지개빛 증표
		BuffID = -1,
		GroupID = -1,
		PriorityIndex = 0,
		All = FALSE,
		ModeCategory = { GC_GMC_DUNGEON, },
	},
}

