TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ 라이언 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- 그라비티 볼
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_SPECIAL,
		GroupID 	=	SG_LEY01_GRAVITYBALL_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon703.dds",
		TitleID 	=	1638,
		DescID 		=	1678,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_GRAVITY_BALL,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 그라비티 볼 개수 증가 LV1
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GRAVITYBALL_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_SPECIAL, },
		NextList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon704.dds",
		Passive = TRUE,
		TitleID 	=	1639,
		DescID 		=	1679,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,
    },

	-- 그라비티 볼 개수 증가 LV2
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GRAVITYBALL_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon705.dds",
		Passive = TRUE,
		TitleID 	=	1640,
		DescID 		=	1680,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
	-- 그라비티 볼 개수 증가 LV3
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_GRAVITYBALL_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon706.dds",
		Passive = TRUE,
		TitleID 	=	1641,
		DescID 		=	1681,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,
    },

	-- 플레임 롤 LV1
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_FLAMEROLL_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon707.dds",
		TitleID 	=	1642,
		DescID 		=	1682,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_FLAME_ROLL,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 플레임 롤 LV2
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_FLAMEROLL_SPECIAL,
		PreList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon708.dds",
		TitleID 	=	1643,
		DescID 		=	1683,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,	
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_FLAME_ROLL,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 플레임 롤 LV3
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_FLAMEROLL_SPECIAL,
		PreList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SIZE_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon709.dds",
		TitleID 	=	1644,
		DescID 		=	1684,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_FLAME_ROLL,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 플레임 롤 공격 범위
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SIZE_SPECIAL,
		GroupID 	=	SG_LEY01_FLAMEROLL_SIZE_SPECIAL,
		PreList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_ETHER, SID_LEY01_INSPIRE_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon710.dds",
		Passive = TRUE,
		TitleID 	=	1645,
		DescID 		=	1685,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },	
	
	-- 에테르
    {		
		ID 			=	SID_LEY01_ETHER,
		GroupID 	=	SG_LEY01_ETHER,
		PreList 	=	{SID_LEY01_FLAMEROLL_SIZE_SPECIAL, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon711.dds",
		TitleID 	=	1646,
		DescID 		=	1686,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,	
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LEY1_ETHER,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

	-- 인스파이어 LV1
    {		
		ID 			=	SID_LEY01_INSPIRE_LV1,
		GroupID 	=	SG_LEY01_INSPIRE,
		PreList 	=	{SID_LEY01_FLAMEROLL_SIZE_SPECIAL, },
		NextList 	=	{SID_LEY01_INSPIRE_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon712.dds",
		TitleID 	=	1647,
		DescID 		=	1687,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LEY1_INSPIRE,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },

	-- 인스파이어 LV2
    {		
		ID 			=	SID_LEY01_INSPIRE_LV2,
		GroupID 	=	SG_LEY01_INSPIRE,
		PreList 	=	{SID_LEY01_INSPIRE_LV1, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon713.dds",
		TitleID 	=	1648,
		DescID 		=	1688,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,				
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LEY1_INSPIRE,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },	

	-- 시크릿 패시지 LV1
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_SPECIAL,
		PreList 	=	{SID_LEY01_ETHER, SID_LEY01_INSPIRE_LV2, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon714.dds",
		TitleID 	=	1649,
		DescID 		=	1689,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_SECRET_PASSAGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 시크릿 패시지 LV2
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon715.dds",
		TitleID 	=	1650,
		DescID 		=	1690,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,		

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_SECRET_PASSAGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 시크릿 패시지 LV3
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV1, SID_LEY02_BARRAGE_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon716.dds",
		TitleID 	=	1651,
		DescID 		=	1691,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,	

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_SECRET_PASSAGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

	-- 시크릿 패시지 개수 증가 LV1
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon717.dds",
		Passive = TRUE,
		TitleID 	=	1652,
		DescID 		=	1692,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 시크릿 패시지 개수 증가 LV2
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon718.dds",
		Passive = TRUE,
		TitleID 	=	1653,
		DescID 		=	1693,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },	

	-- 시크릿 패시지 개수 증가 LV3
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon719.dds",
		Passive = TRUE,
		TitleID 	=	1654,
		DescID 		=	1694,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 지브스 에티켓 LV1
    {		
		ID 			=	SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_JEEVESETIQUETTE_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon720.dds",
		TitleID 	=	1655,
		DescID 		=	1695,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_JEEVES_ETIQUETTE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 지브스 에티켓 LV2
    {		
		ID 			=	SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_JEEVESETIQUETTE_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon721.dds",
		TitleID 	=	1656,
		DescID 		=	1696,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_JEEVES_ETIQUETTE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 지브스 에티켓 LV3
    {		
		ID 			=	SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_JEEVESETIQUETTE_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon722.dds",
		TitleID 	=	1657,
		DescID 		=	1697,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_JEEVES_ETIQUETTE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 가고울리 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon723.dds",
		TitleID 	=	1658,
		DescID 		=	1698,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_GAROUILLE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 가고울리 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV1, SID_LEY01_GAROUILLE_HP_SPECIAL_LV1, SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1,
						SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV1, SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV1, SID_LEY02_SHOVE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon724.dds",
		TitleID 	=	1659,
		DescID 		=	1699,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_GAROUILLE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 가고울리 이동 속도 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SPEED_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon725.dds",
		Passive = TRUE,
		TitleID 	=	1660,
		DescID 		=	1700,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =   -1,

		LockGroup = -1,						
    },

	-- 가고울리 이동 속도 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SPEED_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon726.dds",
		Passive = TRUE,
		TitleID 	=	1661,
		DescID 		=	1701,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 가고울리 체력 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_HP_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_HP_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_HP_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon727.dds",
		Passive = TRUE,
		TitleID 	=	1662,
		DescID 		=	1702,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },
	
	-- 가고울리 체력 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_HP_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_HP_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_HP_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon728.dds",
		Passive = TRUE,
		TitleID 	=	1663,
		DescID 		=	1703,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 가고울리 브레이크 쓰루 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_A_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon729.dds",
		TitleID 	=	1664,
		DescID 		=	1704,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },	
	
	-- 가고울리 브레이크 쓰루 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_A_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon730.dds",
		TitleID 	=	1665,
		DescID 		=	1705,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },	

	-- 가고울리 브레스 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon731.dds",
		TitleID 	=	1666,
		DescID 		=	1706,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 가고울리 브레스 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_GAROUILLE_SKILL_B_FIRE_SPECIAL, SID_LEY01_GAROUILLE_SKILL_B_STONE_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon732.dds",
		TitleID 	=	1667,
		DescID 		=	1707,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 가고울리 브레스 - 파이어
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_FIRE_SPECIAL,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon733.dds",
		TitleID 	=	1668,
		DescID 		=	1708,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },	

	-- 가고울리 브레스 - 스톤
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_STONE_SPECIAL,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon734.dds",
		TitleID 	=	1669,
		DescID 		=	1709,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },		

	-- 지브스 파워 밤 LV1
    {		
		ID 			=	SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_JEEVESPOWERBOMB_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV2, SID_LEY01_GAROUILLE_HP_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon735.dds",
		TitleID 	=	1670,
		DescID 		=	1710,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_JEEVES_POWER_BOMB,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 지브스 파워 밤 LV2
    {		
		ID 			=	SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_JEEVESPOWERBOMB_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon736.dds",
		TitleID 	=	1671,
		DescID 		=	1711,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_JEEVES_POWER_BOMB,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 지브스 파워 밤 LV3
    {		
		ID 			=	SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_JEEVESPOWERBOMB_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon737.dds",
		TitleID 	=	1672,
		DescID 		=	1712,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_JEEVES_POWER_BOMB,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 챠징 어택
    {		
		ID 			=	SID_LEY01_CHARGEATK,
		GroupID 	=	SG_LEY01_CHARGEATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon738.dds",
		TitleID 	=	1673,
		DescID 		=	1713,
		ControlDesc	=	1677,

		UI_Pos_x 	=	186,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	

	-- 플롯
    {		
		ID 			=	SID_LEY01_FLOAT,
		GroupID 	=	SG_LEY01_FLOAT,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon739.dds",
		TitleID 	=	1674,
		DescID 		=	1714,
		ControlDesc	=	1676,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 카운터 시 쿨 타임 감소 LV1
    {		
		ID 			=	SID_LEY01_COUNT_COOLTIME_LV1,
		GroupID 	=	SG_LEY01_COUNT_COOLTIME,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_COUNT_COOLTIME_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon740.dds",
		TitleID 	=	1112,
		DescID 		=	1162,
		ControlDesc	=	1169,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	
	
	-- 카운터 시 쿨 타임 감소 LV2
    {		
		ID 			=	SID_LEY01_COUNT_COOLTIME_LV2,
		GroupID 	=	SG_LEY01_COUNT_COOLTIME,
		PreList 	=	{SID_LEY01_COUNT_COOLTIME_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon741.dds",
		TitleID 	=	1113,
		DescID 		=	1163,
		ControlDesc	=	1169,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },	

	-- 시작 시 AP 보유 LV1
    {		
		ID 			=	SID_LEY01_START_AP_LV1,
		GroupID 	=	SG_LEY01_START_AP,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_START_AP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon742.dds",		
		TitleID 	=	1108,
		DescID 		=	1158,
		ControlDesc	=	1169,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 시작 시 AP 보유 LV2
    {		
		ID 			=	SID_LEY01_START_AP_LV2,
		GroupID 	=	SG_LEY01_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV1, },
		NextList 	=	{SID_LEY01_START_AP_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon743.dds",
		TitleID 	=	1109,
		DescID 		=	1159,
		ControlDesc	=	1169,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

	-- 시작 시 AP 보유 LV3
    {		
		ID 			=	SID_LEY01_START_AP_LV3,
		GroupID 	=	SG_LEY01_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV2, },
		NextList 	=	{SID_LEY01_START_AP_LV4, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon744.dds",
		TitleID 	=	1110,
		DescID 		=	1160,
		ControlDesc	=	1169,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

	-- 크리티컬 증가 LV1
    {		
		ID 			=	SID_LEY01_CRITICAL_LV1,
		GroupID 	=	SG_LEY01_CRITICAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_CRITICAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon745.dds",
		TitleID 	=	1114,
		DescID 		=	1715,
		ControlDesc	=	1169,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	

	-- 크리티컬 증가 LV2
    {		
		ID 			=	SID_LEY01_CRITICAL_LV2,
		GroupID 	=	SG_LEY01_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV1, },
		NextList 	=	{SID_LEY01_CRITICAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon746.dds",
		TitleID 	=	1115,
		DescID 		=	1716,
		ControlDesc	=	1169,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },	

	-- 크리티컬 증가 LV3
    {		
		ID 			=	SID_LEY01_CRITICAL_LV3,
		GroupID 	=	SG_LEY01_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV2, },
		NextList 	=	{SID_LEY01_CRITICAL_LV4, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon747.dds",
		TitleID 	=	1262,
		DescID 		=	1717,
		ControlDesc	=	1169,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },	
}