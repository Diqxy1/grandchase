Category = KGameModeCategory()
GameStage ={
	 { 0 , 6 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 6 },	--GC_GS_FLYING_SHIP      
	 { 3 , 6 },	--GC_GS_VALLEY_OF_OATH   
	 { 4 , 10 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 21 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 9 , 8 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 13 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 8 , 14 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 8, 6, false, GameStage )	--GC_GM_ARENA_TEAM
GameStage ={
	 { 0 , 6 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 6 },	--GC_GS_FLYING_SHIP      
	 { 3 , 6 },	--GC_GS_VALLEY_OF_OATH   
	 { 4 , 10 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 21 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 9 , 8 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 13 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 8 , 14 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 9, 6, false, GameStage )	--GC_GM_ARENA_SURVIVAL
Server:AddGameModeCategory( 6, 6, Category )	-- GC_GMC_ARENABATTLE

Category = KGameModeCategory()
GameStage ={
	 { 0 , 0 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 3 },	--GC_GS_FLYING_SHIP      
	 { 3 , 6 },	--GC_GS_VALLEY_OF_OATH   
	 { 4 , 10 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 21 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 9 , 8 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 13 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 8 , 14 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 0, 0, false, GameStage )	--GC_GM_TEAM_MODE
GameStage ={
	 { 0 , 0 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 3 },	--GC_GS_FLYING_SHIP      
	 { 3 , 6 },	--GC_GS_VALLEY_OF_OATH   
	 { 4 , 10 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 21 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 9 , 8 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 13 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 8 , 14 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 1, 0, false, GameStage )	--GC_GM_SURVIVAL_MODE
Server:AddGameModeCategory( 0, 0, Category )	-- GC_GMC_MATCH

Category = KGameModeCategory()
GameStage ={
	 { 0 , 4 },	--GC_GS_FOREST_OF_ELF    
	 { 3 , 10 },	--GC_GS_VALLEY_OF_OATH   
	 { 4 , 10 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 21 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 9 , 10 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 21 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 8 , 21 },	--GC_GS_HELL_BRIDGE      
	 { 2 , 4 },	--GC_GS_FLYING_SHIP      
 }
Server:AddKGameModeInfo( Category, 6, 4, false, GameStage )	--GC_GM_MONSTER_CRUSADERS
Server:AddGameModeCategory( 3, 4, Category )	-- GC_GMC_MONSTER_CRUSADERS

Category = KGameModeCategory()
GameStage ={ }
Server:AddKGameModeInfo( Category, 26, 0, false, GameStage )	--GC_GM_QUEST_ZERO
GameStage ={ }
Server:AddKGameModeInfo( Category, 27, 0, false, GameStage )	--GC_GM_QUEST_ZERO_2
GameStage ={ }
Server:AddKGameModeInfo( Category, 10, 5, false, GameStage )	--GC_GM_QUEST1
GameStage ={ }
Server:AddKGameModeInfo( Category, 11, 9, false, GameStage )	--GC_GM_QUEST2
GameStage ={ }
Server:AddKGameModeInfo( Category, 12, 13, false, GameStage )	--GC_GM_QUEST3
Server:AddGameModeCategory( 5, 0, Category )	-- GC_GMC_QUEST

Category = KGameModeCategory()
GameStage ={ }
Server:AddKGameModeInfo( Category, 7, 0, false, GameStage )	--GC_GM_MONSTER_HUNT
Server:AddGameModeCategory( 4, 0, Category )	-- GC_GMC_MONSTER_HUNT
