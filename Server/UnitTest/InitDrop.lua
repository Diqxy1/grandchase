--[[
-- 예제
DropMode = KDropMode()

DropMode:AddDropInfo{ NumItem = 1, Probability = 0.35, ItemList = { 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490 } }
DropMode:AddDropInfo{ NumItem = 1, Probability = 0.10, ItemList = {  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880 } }
DropMode:AddDropInfo{ NumItem = 1, Probability = 0.55, ItemList = { 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530 } }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 1, 0, DropMode, KDropSystem.DM_ALL )

--DropSystem:AddMode( int iMainKey, int iSubKey, int iDifficult, int ireserve, DropMode, KDropSystem.DM_ALL )

--iMainKey	= 메인 키(게임 모드)
--iSubKey		= 서브키( 미궁 퀘스트 에서 사용 )
--iDifficult	= 난이도 키(퀘스트 에서 사용)
--ireserve	= 예약 키(현재 사용하지 않음 0으로 셋팅해야함)
--각각의 키는 char로 케스팅됨.(되도록 음수 사용은 금지.


--]]

DropSystem  = GetDropSystem()
--===========================================
--060321. jseop. 기술은 안 떨어뜨린다.
--DropMode    = KDropMode()

--신기술과 기술 아이템 총 10%의 확률로 떨어진다.
--[[
21590	버서크 러시
21600   토네이도 블레이드
21610   이글사이트
21620   익스플로젼 애로우
21630   다크 클라우드
21640	컨퓨젼
21650 	라바 플로우
21660	아테네 소드
21670	부스트 힐

DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21590, 21590, 21590, 21590, 21590, 21590, 21590, 21590 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21600, 21600, 21600, 21600, 21600, 21600, 21600, 21600 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21610, 21610, 21610, 21610, 21610, 21610, 21610, 21610 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21620, 21620, 21620, 21620, 21620, 21620, 21620, 21620 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21630, 21630, 21630, 21630, 21630, 21630, 21630, 21630 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21640, 21640, 21640, 21640, 21640, 21640, 21640, 21640 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21650, 21650, 21650, 21650, 21650, 21650, 21650, 21650 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21660, 21660, 21660, 21660, 21660, 21660, 21660, 21660 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 21670, 21670, 21670, 21670, 21670, 21670, 21670, 21670 } }

DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 19860, 19860, 19860, 19860, 19860, 19860, 19860, 19860 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 19870, 19870, 19870, 19870, 19870, 19870, 19870, 19870 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 19880, 19880, 19880, 19880, 19880, 19880, 19880, 19880 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 19890, 19890, 19890, 19890, 19890, 19890, 19890, 19890 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 19900, 19900, 19900, 19900, 19890, 19900, 19900, 19900 } }
DropMode:AddDropInfo{ NumItem = 10, Probability = 0.0067, DropType = KDropSystem.DT_ALLWAYS, ItemList = { 19910, 19910, 19910, 19910, 19910, 19910, 19910, 19910 } }

DropSystem:AddMode( KDropSystem.GC_GM_TEAM_MODE, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_SURVIVAL_MODE, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_GUILD_BATTLE, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_ANGEL_RESCUE, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_ARCHER_CONTEST, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_GADOSEN_DEFEAT, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_ARENA_TEAM, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
DropSystem:AddMode( KDropSystem.GC_GM_ARENA_SURVIVAL, 0, 0, 0, DropMode, KDropSystem.DM_ALL )

--XMAS_EVENT_ITEM = { TestGroup = 100, NumItem = -1, Probability = 1.0, ItemList = { -2005, -2005, -2005, -2005, -2005, -2005, -2005, -2005 }}
--DropMode:AddDropInfo( XMAS_EVENT_ITEM )

DropSystem:AddMode( KDropSystem.GC_GM_MONSTER_CRUSADERS, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
--]]
--===========================================
ITEM_LIST_KAZEAZE_MAP_1 =        { 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530 } -- 지도 조각1
ITEM_LIST_KAZEAZE_MAP_2 =        { 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690 } -- 지도 조각2
ITEM_LIST_KAZEAZE_MAP_3 =        { 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130 } -- 지도 조각3
ITEM_LIST_KAZEAZE_MAP_4 =        { 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170 } -- 지도 조각4
ITEM_LIST_KAZEAZE_MAP_ORIGINAL = { 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490 } -- 지도 조각 원본
ITEM_LIST_DIM_CRYSTAL =          { 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520 } -- 빛을 잃은 크리스탈
ITEM_LIST_CRYSTAL =              { 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490 } -- 크리스탈
ITEM_LIST_GEM =                  {  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880 } -- 젬
ITEM_LIST_SEALED_CARD =          { 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500 } -- 봉인된 카드
ITEM_LIST_EMBLEM = 				 { 21920, 21920, 21920,     0, 21920, 21920, 21920,     0, 21920, 21920, 21920,     0 } -- 앰블램 
--===========================================

DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.10, ItemList = ITEM_LIST_DIM_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST_ZERO_2, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_DIM_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST_ZERO_2, 0, 1, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.35, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.05, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST_ZERO_2, 0, 2, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST_ZERO_2, 0, 3, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 2, Probability = 0.05, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST_ZERO_2, 0, 4, 0, DropMode, KDropSystem.DM_ALL )

--===========================================

DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.10, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.05, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 1, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_KAZEAZE_MAP_1 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 2, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_KAZEAZE_MAP_1 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.60, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 3, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.60, ItemList = ITEM_LIST_KAZEAZE_MAP_1 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 4, 0, DropMode, KDropSystem.DM_ALL )

--===========================================

DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST2, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST2, 0, 1, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_KAZEAZE_MAP_2 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST2, 0, 2, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_KAZEAZE_MAP_2 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.90, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST2, 0, 3, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_KAZEAZE_MAP_2 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.80, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.20, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST2, 0, 4, 0, DropMode, KDropSystem.DM_ALL )

--===========================================

DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST3, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_DIM_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST3, 0, 1, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_KAZEAZE_MAP_3 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.90, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST3, 0, 2, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.60, ItemList = ITEM_LIST_KAZEAZE_MAP_3 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.80, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.20, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST3, 0, 3, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.80, ItemList = ITEM_LIST_KAZEAZE_MAP_3 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 2, Probability = 0.10, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST3, 0, 4, 0, DropMode, KDropSystem.DM_ALL )

--===========================================

DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST4, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST4, 0, 1, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.60, ItemList = ITEM_LIST_KAZEAZE_MAP_4 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST4, 0, 2, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.80, ItemList = ITEM_LIST_KAZEAZE_MAP_4 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.50, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 2, Probability = 0.20, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST4, 0, 3, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 1.00, ItemList = ITEM_LIST_KAZEAZE_MAP_4 }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 3, Probability = 0.20, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 2, Probability = 0.50, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST4, 0, 4, 0, DropMode, KDropSystem.DM_ALL )

--===========================================

DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.70, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST6, 0, 0, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, NumItem = 1, Probability = 0.60, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 0, NumItem = 2, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.20, ItemList = ITEM_LIST_GEM }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST6, 0, 1, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, Receiver = KDropSystem.DR_HOST, NumItem = -1, Probability = 0.30, ItemList = ITEM_LIST_KAZEAZE_MAP_ORIGINAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 3, Probability = 0.30, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.50, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 2, Probability = 0.50, ItemList = ITEM_LIST_GEM }
--DropMode:AddDropInfo{ TestGroup = 3, NumItem = 1, Probability = 1.00, ItemList = ITEM_LIST_SEALED_CARD }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST6, 0, 2, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, Receiver = KDropSystem.DR_HOST, NumItem = -1, Probability = 0.60, ItemList = ITEM_LIST_KAZEAZE_MAP_ORIGINAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.20, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 3, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.40, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 3, Probability = 0.30, ItemList = ITEM_LIST_GEM }
--DropMode:AddDropInfo{ TestGroup = 3, NumItem = 1, Probability = 1.00, ItemList = ITEM_LIST_SEALED_CARD }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST6, 0, 3, 0, DropMode, KDropSystem.DM_ALL )
---------------------------------------------
DropMode = KDropMode()

DropMode:AddDropInfo{ TestGroup = 0, Receiver = KDropSystem.DR_HOST, NumItem = -1, Probability = 0.90, ItemList = ITEM_LIST_KAZEAZE_MAP_ORIGINAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 1, Probability = 0.10, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 2, Probability = 0.40, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 1, NumItem = 3, Probability = 0.50, ItemList = ITEM_LIST_CRYSTAL }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 1, Probability = 0.30, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 2, Probability = 0.30, ItemList = ITEM_LIST_GEM }
DropMode:AddDropInfo{ TestGroup = 2, NumItem = 3, Probability = 0.40, ItemList = ITEM_LIST_GEM }
--DropMode:AddDropInfo{ TestGroup = 3, NumItem = 1, Probability = 1.00, ItemList = ITEM_LIST_SEALED_CARD }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST6, 0, 4, 0, DropMode, KDropSystem.DM_ALL )

-- KDropSystem::EnableAdminEvent( bool bOnOff, int iItemID, int iNumItem )
DropSystem:EnableAdminEvent( false, 17010, 100 )
