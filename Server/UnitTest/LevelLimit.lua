Category = KGameModeCategory()
GameStage ={
	 { 0 , 10 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 10 },	--GC_GS_FLYING_SHIP      
	 { 43 , 10 },	--GC_GS_FOREST_OF_TRIAL  
	 { 3 , 15 },	--GC_GS_VALLEY_OF_OATH   
	 { 11 , 15 },	--GC_GS_CARRIBEACH
	 { 4 , 15 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 20 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 10 , 20 },	--GC_GS_GUILD
	 { 7 , 20 },	--GC_GS_SHOOTING_FIELD   
	 { 9 , 25 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 25 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 5 , 28 },	--GC_GS_BABEL_OF_X_MAS   
	 { 8 , 28 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 8, 10, false, GameStage )	--GC_GM_ARENA_TEAM
GameStage ={
	 { 0 , 10 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 10 },	--GC_GS_FLYING_SHIP      
	 { 43 , 10 },	--GC_GS_FOREST_OF_TRIAL  
	 { 3 , 15 },	--GC_GS_VALLEY_OF_OATH   
	 { 11 , 15 },	--GC_GS_CARRIBEACH
	 { 4 , 15 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 20 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 10 , 20 },	--GC_GS_GUILD
	 { 7 , 20 },	--GC_GS_SHOOTING_FIELD   
	 { 9 , 25 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 25 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 5 , 28 },	--GC_GS_BABEL_OF_X_MAS   
	 { 8 , 28 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 9, 10, false, GameStage )	--GC_GM_ARENA_SURVIVAL
Server:AddGameModeCategory( 6, 10, Category )	-- GC_GMC_ARENABATTLE

Category = KGameModeCategory()
GameStage ={ }
Server:AddKGameModeInfo( Category, 7, 1, false, GameStage )	--GC_GM_MONSTER_HUNT
Server:AddGameModeCategory( 4, 1, Category )	-- GC_GMC_MONSTER_HUNT

Category = KGameModeCategory()
GameStage ={
	 { 0 , 1 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 1 },	--GC_GS_FLYING_SHIP      
	 { 43 , 1 },	--GC_GS_FOREST_OF_TRIAL  
	 { 3 , 15 },	--GC_GS_VALLEY_OF_OATH   
	 { 11 , 15 },	--GC_GS_CARRIBEACH
	 { 4 , 15 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 20 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 10 , 20 },	--GC_GS_GUILD
	 { 7 , 20 },	--GC_GS_SHOOTING_FIELD   
	 { 9 , 25 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 25 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 5 , 28 },	--GC_GS_BABEL_OF_X_MAS   
	 { 8 , 28 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 0, 1, false, GameStage )	--GC_GM_TEAM_MODE
GameStage ={
	 { 0 , 10 },	--GC_GS_FOREST_OF_ELF    
	 { 2 , 10 },	--GC_GS_FLYING_SHIP      
	 { 43 , 10 },	--GC_GS_FOREST_OF_TRIAL  
	 { 3 , 15 },	--GC_GS_VALLEY_OF_OATH   
	 { 11 , 15 },	--GC_GS_CARRIBEACH
	 { 4 , 15 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 20 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 10 , 20 },	--GC_GS_GUILD
	 { 7 , 20 },	--GC_GS_SHOOTING_FIELD   
	 { 9 , 25 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 25 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 5 , 28 },	--GC_GS_BABEL_OF_X_MAS   
	 { 8 , 28 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 1, 1, false, GameStage )	--GC_GM_SURVIVAL_MODE
Server:AddGameModeCategory( 0, 1, Category )	-- GC_GMC_MATCH

Category = KGameModeCategory()
GameStage ={
	 { 0 , 5 },	--GC_GS_FOREST_OF_ELF    
	 { 43 , 5 },	--GC_GS_FOREST_OF_TRIAL  
	 { 3 , 5 },	--GC_GS_VALLEY_OF_OATH   
	 { 11 , 15 },	--GC_GS_CARRIBEACH
	 { 4 , 15 },	--GC_GS_FOGOTTEN_CITY    
	 { 1 , 15 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 10 , 20 },	--GC_GS_GUILD
	 { 7 , 20 },	--GC_GS_SHOOTING_FIELD   
	 { 9 , 20 },	--GC_GS_TEMPLE_OF_ORC    
	 { 6 , 25 },	--GC_GS_TEMPLE_OF_FIRE   
	 { 8 , 28 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 6, 5, false, GameStage )	--GC_GM_MONSTER_CRUSADERS
Server:AddGameModeCategory( 3, 5, Category )	-- GC_GMC_MONSTER_CRUSADERS

Category = KGameModeCategory()
GameStage ={ }
Server:AddKGameModeInfo( Category, 26, 0, false, GameStage )	--GC_GM_QUEST_ZERO
GameStage ={ }
Server:AddKGameModeInfo( Category, 27, 0, false, GameStage )	--GC_GM_QUEST_ZERO_2
GameStage ={ }
Server:AddKGameModeInfo( Category, 10, 1, false, GameStage )	--GC_GM_QUEST1
GameStage ={ }
Server:AddKGameModeInfo( Category, 11, 10, false, GameStage )	--GC_GM_QUEST2
GameStage ={ }
Server:AddKGameModeInfo( Category, 12, 15, false, GameStage )	--GC_GM_QUEST3
GameStage ={ }
Server:AddKGameModeInfo( Category, 13, 20, false, GameStage )	--GC_GM_QUEST4
GameStage ={ }
Server:AddKGameModeInfo( Category, 14, 25, false, GameStage )	--GC_GM_QUEST5
GameStage ={ }
Server:AddKGameModeInfo( Category, 15, 28, false, GameStage )	--GC_GM_QUEST6
Server:AddGameModeCategory( 5, 0, Category )	-- GC_GMC_QUEST

Category = KGameModeCategory()
GameStage ={
	 { 7 , 10 },	--GC_GS_SHOOTING_FIELD   
 }
Server:AddKGameModeInfo( Category, 4, 10, false, GameStage )	--GC_GM_ARCHER_CONTEST
GameStage ={
	 { 8 , 10 },	--GC_GS_HELL_BRIDGE      
 }
Server:AddKGameModeInfo( Category, 5, 10, false, GameStage )	--GC_GM_GADOSEN_DEFEAT
GameStage ={
	 { 0 , 10 },	--GC_GS_FOREST_OF_ELF    
	 { 1 , 25 },	--GC_GS_SWAMP_OF_OBLIVION
	 { 2 , 10 },	--GC_GS_FLYING_SHIP      
	 { 3 , 15 },	--GC_GS_VALLEY_OF_OATH   
	 { 4 , 15 },	--GC_GS_FOGOTTEN_CITY    
	 { 7 , 20 },	--GC_GS_SHOOTING_FIELD   
	 { 8 , 28 },	--GC_GS_HELL_BRIDGE      
	 { 9 , 25 },	--GC_GS_TEMPLE_OF_ORC    
	 { 10 , 20 },	--GC_GS_GUILD
	 { 43 , 10 },	--GC_GS_FOREST_OF_TRIAL  
 }
Server:AddKGameModeInfo( Category, 3, 10, false, GameStage )	--GC_GM_ANGEL_RESCUE
Server:AddGameModeCategory( 2, 10, Category )	-- GC_GMC_PROMOTION_MISSION

