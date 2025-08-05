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
	-- 셔브 LV1
    {		
		ID 			=	SID_LEY02_SHOVE_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_SHOVE_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_SHOVE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1822,
		DescID 		=	1860,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_SHOVE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 셔브 LV2
    {		
		ID 			=	SID_LEY02_SHOVE_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_SHOVE_SPECIAL,
		PreList 	=	{SID_LEY02_SHOVE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_SHOVE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1823,
		DescID 		=	1861,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_SHOVE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,
    },

	-- 셔브 LV3
    {		
		ID 			=	SID_LEY02_SHOVE_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_SHOVE_SPECIAL,
		PreList 	=	{SID_LEY02_SHOVE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1824,
		DescID 		=	1862,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_SHOVE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,
    },

	-- 바스티언 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPECIAL,
		PreList 	=	{SID_LEY02_SHOVE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1825,
		DescID 		=	1863,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_MERRY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 바스티언 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPECIAL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_BASTION_SPEED_LV1, SID_LEY02_BASTION_HP_LV1, SID_LEY02_BASTION_HOWLING_LV1, SID_LEY02_BASTION_SPINDASH_LV1, SID_LEY02_BASTION_SPINDROP_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1826,
		DescID 		=	1864,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_MERRY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		Overlapped  = TRUE,
    },
	
	-- 바스티언 이동 속도 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPEED_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPEED_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPEED_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Passive = TRUE,
		TitleID 	=	1827,
		DescID 		=	1865,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =   -1,

		LockGroup = -1,	
    },

	-- 바스티언 이동 속도 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPEED_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPEED_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_SPEED_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1828,
		DescID 		=	1866,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =   -1,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },
	
	-- 바스티언 체력 LV1
    {		
		ID 			=	SID_LEY02_BASTION_HP_LV1,
		GroupID 	=	SG_LEY02_BASTION_HP_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_HP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1829,
		DescID 		=	1867,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 바스티언 체력 LV2
    {		
		ID 			=	SID_LEY02_BASTION_HP_LV2,
		GroupID 	=	SG_LEY02_BASTION_HP_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_HP_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1830,
		DescID 		=	1868,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,		
		Overlapped  = TRUE,		
    },
	
	-- 바스티언 하울링 LV1
    {		
		ID 			=	SID_LEY02_BASTION_HOWLING_LV1,
		GroupID 	=	SG_LEY02_BASTION_HOWLING_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_HOWLING_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1831,
		DescID 		=	1869,
		ControlDesc	=	1903,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 바스티언 하울링 LV2
    {		
		ID 			=	SID_LEY02_BASTION_HOWLING_LV2,
		GroupID 	=	SG_LEY02_BASTION_HOWLING_SKILL,
		PreList 	=	{SID_LEY02_BASTION_HOWLING_LV1, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1832,
		DescID 		=	1870,
		ControlDesc	=	1903,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 대쉬 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPINDASH_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPINDASH_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPINDASH_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1833,
		DescID 		=	1871,
		ControlDesc	=	1902,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,						
    },

	-- 바스티언 스핀 대쉬 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPINDASH_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPINDASH_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDASH_LV1, },
		NextList 	=	{SID_LEY02_BASTION_SPINDASH_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1834,
		DescID 		=	1872,
		ControlDesc	=	1902,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 대쉬 LV3
    {		
		ID 			=	SID_LEY02_BASTION_SPINDASH_LV3,
		GroupID 	=	SG_LEY02_BASTION_SPINDASH_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDASH_LV2, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1835,
		DescID 		=	1873,
		ControlDesc	=	1902,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 드랍 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPINDROP_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPINDROP_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPINDROP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1836,
		DescID 		=	1874,
		ControlDesc	=	401,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 바스티언 스핀 드랍 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPINDROP_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPINDROP_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDROP_LV1, },
		NextList 	=	{SID_LEY02_BASTION_SPINDROP_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1837,
		DescID 		=	1875,
		ControlDesc	=	401,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 드랍 LV3
    {		
		ID 			=	SID_LEY02_BASTION_SPINDROP_LV3,
		GroupID 	=	SG_LEY02_BASTION_SPINDROP_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDROP_LV2, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1838,
		DescID 		=	1876,
		ControlDesc	=	401,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

	-- 그림 핸드 LV1
    {		
		ID 			=	SID_LEY02_GRIMHAND_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_GRIMHAND_SPECIAL,
		PreList 	=	{SID_LEY02_BASTION_HOWLING_LV2, SID_LEY02_BASTION_SPINDASH_LV3, SID_LEY02_BASTION_SPINDROP_LV3, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1839,
		DescID 		=	1877,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_GRIM_HAND,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 그림 핸드 LV2
    {		
		ID 			=	SID_LEY02_GRIMHAND_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_GRIMHAND_SPECIAL,
		PreList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1840,
		DescID 		=	1878,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_GRIM_HAND,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
		Overlapped  = TRUE,		
    },

	-- 그림 핸드 LV3
    {		
		ID 			=	SID_LEY02_GRIMHAND_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_GRIMHAND_SPECIAL,
		PreList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1841,
		DescID 		=	1879,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_GRIM_HAND,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
		Overlapped  = TRUE,		
    },

	-- 언설튼 에어 LV1
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1842,
		DescID 		=	1880,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_UNCERTAIN_AIR,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 언설튼 에어 LV2
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_SPECIAL,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1843,
		DescID 		=	1881,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_UNCERTAIN_AIR,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,		
    },

	-- 언설튼 에어 LV3
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_SPECIAL,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_UNCERTAINAIR_NUM, SID_LEY02_TEARDROP_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1844,
		DescID 		=	1882,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_UNCERTAIN_AIR,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,		
    },

	-- 언설튼 에어 개수 증가
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_NUM,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_NUM_PASSIVE,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV3, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1845,
		DescID 		=	1883,
		ControlDesc	=	127,

		UI_Pos_x 	=	500 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,	
		Passive = TRUE,
    },

	-- 티어 드랍 LV1
    {		
		ID 			=	SID_LEY02_TEARDROP_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_TEARDROP_SPECIAL,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV3, },
		NextList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1846,
		DescID 		=	1884,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_TEARDROP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
	-- 티어 드랍 LV2
    {		
		ID 			=	SID_LEY02_TEARDROP_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_TEARDROP_SPECIAL,
		PreList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1847,
		DescID 		=	1885,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_TEARDROP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		Overlapped  = TRUE,
    },
	
	-- 티어 드랍 LV3
    {		
		ID 			=	SID_LEY02_TEARDROP_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_TEARDROP_SPECIAL,
		PreList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BARRAGE_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1848,
		DescID 		=	1886,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_TEARDROP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		Overlapped  = TRUE,
    },
	
	-- 버라지 LV1
    {		
		ID 			=	SID_LEY02_BARRAGE_NORMAL_LV1,
		GroupID 	=	SG_LEY02_BARRAGE_NORMAL,
		PreList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV3, SID_LEY01_SECRETPASSAGE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1849,
		DescID 		=	1887,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.5,
			Motion = MID_LEY2_BARRAGE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.5,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 버라지 LV2
    {		
		ID 			=	SID_LEY02_BARRAGE_NORMAL_LV2,
		GroupID 	=	SG_LEY02_BARRAGE_NORMAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV1, },
		NextList 	=	{SID_LEY02_BARRAGE_DARKSIGHT_LV1, SID_LEY02_BARRAGE_BIGEYE, SID_LEY02_COG_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1850,
		DescID 		=	1888,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.5,
			Motion = MID_LEY2_BARRAGE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.5,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,
    },
	
	-- 버라지 다크 사이트 LV1
    {		
		ID 			=	SID_LEY02_BARRAGE_DARKSIGHT_LV1,
		GroupID 	=	SG_LEY02_BARRAGE_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, },
		NextList 	=	{SID_LEY02_BARRAGE_DARKSIGHT_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1851,
		DescID 		=	1889,
		ControlDesc	=	1169,

		UI_Pos_x 	=	500 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
    },
	
	-- 버라지 다크 사이트 LV2
    {		
		ID 			=	SID_LEY02_BARRAGE_DARKSIGHT_LV2,
		GroupID 	=	SG_LEY02_BARRAGE_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_DARKSIGHT_LV1, },
		NextList 	=	{SID_LEY02_COG_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1852,
		DescID 		=	1890,
		ControlDesc	=	1169,

		UI_Pos_x 	=	500 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,
    },
	
	-- 버라지 빅아이 익스플로전
    {		
		ID 			=	SID_LEY02_BARRAGE_BIGEYE,
		GroupID 	=	SG_LEY02_BARRAGE_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, },
		NextList 	=	{SID_LEY02_COG_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1853,
		DescID 		=	1891,
		ControlDesc	=	1169,

		UI_Pos_x 	=	500 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
    },
	
	-- 센터 오브 그라비티
    {		
		ID 			=	SID_LEY02_COG_SPECIAL,
		GroupID 	=	SG_LEY02_COG_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, SID_LEY02_BARRAGE_DARKSIGHT_LV2, SID_LEY02_BARRAGE_BIGEYE, },
		NextList 	=	{SID_LEY02_COGFATAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1854,
		DescID 		=	1892,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_THE_CENTER_OF_GRAVITY_READY,			
			CoolTime = 35,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 30,
		},
    },

	-- 센터 오브 그라비티 FATAL 확률 LV1
    {		
		ID 			=	SID_LEY02_COGFATAL_LV1,
		GroupID 	=	SG_LEY02_COG_SPECIAL,
		PreList 	=	{SID_LEY02_COG_SPECIAL, },
		NextList 	=	{SID_LEY02_COGFATAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1855,
		DescID 		=	1893,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_THE_CENTER_OF_GRAVITY_READY,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 27,
		},
		
    },

	-- 센터 오브 그라비티 FATAL 확률 LV2
    {		
		ID 			=	SID_LEY02_COGFATAL_LV2,
		GroupID 	=	SG_LEY02_COG_SPECIAL,
		PreList 	=	{SID_LEY02_COGFATAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1856,
		DescID 		=	1894,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_THE_CENTER_OF_GRAVITY_READY,			
			CoolTime = 25,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 24,
		},
    },

	-- 점프 어택 LV1
    {		
		ID 			=	SID_LEY02_JUMPATK_LV1,
		GroupID 	=	SG_LEY02_JUMPATK,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_JUMPATK_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	TRUE,
		LearnSP 	=	1,

		TitleID 	=	1252,
		DescID 		=	1900,
		ControlDesc	=	403,

		UI_Pos_x 	=	186,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 점프 어택 LV2
    {		
		ID 			=	SID_LEY02_JUMPATK_LV2,
		GroupID 	=	SG_LEY02_JUMPATK,
		PreList 	=	{SID_LEY02_JUMPATK_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1253,
		DescID 		=	1895,
		ControlDesc	=	403,

		UI_Pos_x 	=	186,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 대쉬 어택 LV1
    {		
		ID 			=	SID_LEY02_DASHATK_LV1,
		GroupID 	=	SG_LEY02_DASHATK,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_DASHATK_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	TRUE,
		LearnSP 	=	1,

		TitleID 	=	1857,
		DescID 		=	1901,
		ControlDesc	=	138,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 대쉬 어택 LV2
    {		
		ID 			=	SID_LEY02_DASHATK_LV2,
		GroupID 	=	SG_LEY02_DASHATK,
		PreList 	=	{SID_LEY02_DASHATK_LV1, },
		NextList 	=	{SID_LEY02_DASHATK_SIZE, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1858,
		DescID 		=	1896,
		ControlDesc	=	1904,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 대쉬 어택 거리 증가
    {		
		ID 			=	SID_LEY02_DASHATK_SIZE,
		GroupID 	=	SG_LEY02_DASHATK,
		PreList 	=	{SID_LEY02_DASHATK_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1859,
		DescID 		=	1897,
		ControlDesc	=	430,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- AP 회복 속도 증가 LV1
    {		
		ID 			=	SID_LEY02_AP_UP_LV1,
		GroupID 	=	SG_LEY02_AP_UP,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_AP_UP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1106,
		DescID 		=	1156,
		ControlDesc	=	430,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- AP 회복 속도 증가 LV2
    {		
		ID 			=	SID_LEY02_AP_UP_LV2,
		GroupID 	=	SG_LEY02_AP_UP,
		PreList 	=	{SID_LEY02_AP_UP_LV1, },
		NextList 	=	{SID_LEY02_AP_UP_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1107,
		DescID 		=	1157,
		ControlDesc	=	430,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- AP 회복 속도 증가 LV3
    {		
		ID 			=	SID_LEY02_AP_UP_LV3,
		GroupID 	=	SG_LEY02_AP_UP,
		PreList 	=	{SID_LEY02_AP_UP_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1257,
		DescID 		=	1315,
		ControlDesc	=	430,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- 시작 시 AP 보유 LV4
    {		
		ID 			=	SID_LEY01_START_AP_LV4,
		GroupID 	=	SG_LEY02_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV3, },
		NextList 	=	{SID_LEY01_START_AP_LV5, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1260,
		DescID 		=	1318,
		ControlDesc	=	430,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 시작 시 AP 보유 LV5
    {		
		ID 			=	SID_LEY01_START_AP_LV5,
		GroupID 	=	SG_LEY02_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV4, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1261,
		DescID 		=	1319,
		ControlDesc	=	430,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
	-- 크리티컬 증가 LV4
    {		
		ID 			=	SID_LEY01_CRITICAL_LV4,
		GroupID 	=	SG_LEY02_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV3, },
		NextList 	=	{SID_LEY01_CRITICAL_LV5, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1263,
		DescID 		=	1898,
		ControlDesc	=	430,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	

	-- 크리티컬 증가 LV5
    {		
		ID 			=	SID_LEY01_CRITICAL_LV5,
		GroupID 	=	SG_LEY02_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV4, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1264,
		DescID 		=	1899,
		ControlDesc	=	430,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },
}