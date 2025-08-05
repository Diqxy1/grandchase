FHP_JUST_FOOTHOLD	=0	-- �׷��� ����. �ö󰡴� ������
FHP_NO_JUMP_DOWN	=1	-- �� Ű�� ������ ����������.
FHP_NO_JUMP_UP		=2	-- �� Ű�� ������ ����������.
FHP_NO_COME_RIGHT	=4	-- �� Ű�� ������ ����.
FHP_NO_COME_LEFT	=8	-- �� Ű�� ������ ����.


LayerManager:AddLayerParts{
	CATEGORY = "Dungeon_Quest0.lua",
	PARTNAME = "����_����1",
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
	PARTNAME = "����_����2",
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
	PARTNAME = "����_���",
	TEX_NAME = "SKY0.DDS",
	TEX_X = 0,
	TEX_Y = 0,
	TEX_WIDTH = 512,
	TEX_HEIGHT = 512, 
	PARTS_WIDTH = 800,
	PARTS_HEIGHT = 600,
}

