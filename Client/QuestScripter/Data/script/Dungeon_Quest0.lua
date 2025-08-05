FHP_JUST_FOOTHOLD	=0	-- 그런거 없다. 올라가는 발판임
FHP_NO_JUMP_DOWN	=1	-- ↓ 키를 눌러서 못내려간다.
FHP_NO_JUMP_UP		=2	-- ↑ 키를 눌러서 못내려간다.
FHP_NO_COME_RIGHT	=4	-- ← 키를 눌러서 못감.
FHP_NO_COME_LEFT	=8	-- → 키를 눌러서 못감.


LayerManager:AddLayerParts{
	CATEGORY = "Dungeon_Quest0.lua",
	PARTNAME = "엘숲_발판1",
	TEX_NAME = "sky27.DDS",
	TEX_X = 0,
	TEX_Y = 0,
	TEX_WIDTH = 512,
	TEX_HEIGHT = 512, 
	PARTS_WIDTH = 600,
	PARTS_HEIGHT = 600,
	FOOTHOLD_WIDTH = 1,
	FOOTHOLD_HEIGHT = 1,
	FOOTHOLD_DATA = 
	{
		{ 0, 0, FHP_NO_JUMP_DOWN + FHP_JUST_FOOTHOLD, "", },
	},
}

LayerManager:AddLayerParts{
	CATEGORY = "Dungeon_Quest0.lua",
	PARTNAME = "엘숲_발판2",
	TEX_NAME = "0_2.DDS",
	TEX_X = 0,
	TEX_Y = 0,
	TEX_WIDTH = 512,
	TEX_HEIGHT = 512, 
	PARTS_WIDTH = 600,
	PARTS_HEIGHT = 600,
}

LayerManager:AddLayerParts{
	CATEGORY = "Dungeon_Quest0.lua",
	PARTNAME = "엘숲_배경",
	TEX_NAME = "SKY0.DDS",
	TEX_X = 0,
	TEX_Y = 0,
	TEX_WIDTH = 512,
	TEX_HEIGHT = 512, 
	PARTS_WIDTH = 800,
	PARTS_HEIGHT = 600,
}

