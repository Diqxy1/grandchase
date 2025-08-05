--[[
-- ����
DropMode = KDropMode()

DropMode:AddDropInfo{ NumItem = 1, Probability = 0.35, ItemList = { 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490 } }
DropMode:AddDropInfo{ NumItem = 1, Probability = 0.10, ItemList = {  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880 } }
DropMode:AddDropInfo{ NumItem = 1, Probability = 0.55, ItemList = { 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530 } }

DropSystem:AddMode( KDropSystem.GC_GM_QUEST1, 0, 1, 0, DropMode, KDropSystem.DM_ALL )

--DropSystem:AddMode( int iMainKey, int iSubKey, int iDifficult, int ireserve, DropMode, KDropSystem.DM_ALL )

--iMainKey	= ���� Ű(���� ���)
--iSubKey		= ����Ű( �̱� ����Ʈ ���� ��� )
--iDifficult	= ���̵� Ű(����Ʈ ���� ���)
--ireserve	= ���� Ű(���� ������� ���� 0���� �����ؾ���)
--������ Ű�� char�� �ɽ��õ�.(�ǵ��� ���� ����� ����.


--]]

DropSystem  = GetDropSystem()
--===========================================
--060321. jseop. ����� �� ����߸���.
--DropMode    = KDropMode()

--�ű���� ��� ������ �� 10%�� Ȯ���� ��������.
--[[
21590	����ũ ����
21600   ����̵� ���̵�
21610   �̱ۻ���Ʈ
21620   �ͽ��÷��� �ַο�
21630   ��ũ Ŭ����
21640	��ǻ��
21650 	��� �÷ο�
21660	���׳� �ҵ�
21670	�ν�Ʈ ��

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
ITEM_LIST_KAZEAZE_MAP_1 =        { 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530, 12530 } -- ���� ����1
ITEM_LIST_KAZEAZE_MAP_2 =        { 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690, 13690 } -- ���� ����2
ITEM_LIST_KAZEAZE_MAP_3 =        { 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130, 14130 } -- ���� ����3
ITEM_LIST_KAZEAZE_MAP_4 =        { 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170, 15170 } -- ���� ����4
ITEM_LIST_KAZEAZE_MAP_ORIGINAL = { 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490, 17490 } -- ���� ���� ����
ITEM_LIST_DIM_CRYSTAL =          { 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520, 12520 } -- ���� ���� ũ����Ż
ITEM_LIST_CRYSTAL =              { 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490, 12490 } -- ũ����Ż
ITEM_LIST_GEM =                  {  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880,  8880 } -- ��
ITEM_LIST_SEALED_CARD =          { 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500, 17500 } -- ���ε� ī��
ITEM_LIST_EMBLEM = 				 { 21920, 21920, 21920,     0, 21920, 21920, 21920,     0, 21920, 21920, 21920,     0 } -- �ں� 
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
