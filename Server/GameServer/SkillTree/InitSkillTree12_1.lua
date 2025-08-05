TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ ���̾� 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- �ź� LV1
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

	-- �ź� LV2
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

	-- �ź� LV3
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

	-- �ٽ�Ƽ�� LV1
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

	-- �ٽ�Ƽ�� LV2
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
	
	-- �ٽ�Ƽ�� �̵� �ӵ� LV1
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

	-- �ٽ�Ƽ�� �̵� �ӵ� LV2
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
	
	-- �ٽ�Ƽ�� ü�� LV1
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

	-- �ٽ�Ƽ�� ü�� LV2
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
	
	-- �ٽ�Ƽ�� �Ͽ︵ LV1
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

	-- �ٽ�Ƽ�� �Ͽ︵ LV2
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

	-- �ٽ�Ƽ�� ���� �뽬 LV1
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

	-- �ٽ�Ƽ�� ���� �뽬 LV2
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

	-- �ٽ�Ƽ�� ���� �뽬 LV3
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

	-- �ٽ�Ƽ�� ���� ��� LV1
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

	-- �ٽ�Ƽ�� ���� ��� LV2
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

	-- �ٽ�Ƽ�� ���� ��� LV3
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

	-- �׸� �ڵ� LV1
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

	-- �׸� �ڵ� LV2
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

	-- �׸� �ڵ� LV3
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

	-- ��ư ���� LV1
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

	-- ��ư ���� LV2
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

	-- ��ư ���� LV3
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

	-- ��ư ���� ���� ����
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

	-- Ƽ�� ��� LV1
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
	
	-- Ƽ�� ��� LV2
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
	
	-- Ƽ�� ��� LV3
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
	
	-- ������ LV1
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
	
	-- ������ LV2
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
	
	-- ������ ��ũ ����Ʈ LV1
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
	
	-- ������ ��ũ ����Ʈ LV2
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
	
	-- ������ ����� �ͽ��÷���
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
	
	-- ���� ���� �׶��Ƽ
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

	-- ���� ���� �׶��Ƽ FATAL Ȯ�� LV1
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

	-- ���� ���� �׶��Ƽ FATAL Ȯ�� LV2
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

	-- ���� ���� LV1
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

	-- ���� ���� LV2
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

	-- �뽬 ���� LV1
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

	-- �뽬 ���� LV2
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

	-- �뽬 ���� �Ÿ� ����
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

	-- AP ȸ�� �ӵ� ���� LV1
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

	-- AP ȸ�� �ӵ� ���� LV2
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

	-- AP ȸ�� �ӵ� ���� LV3
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

	-- ���� �� AP ���� LV4
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

	-- ���� �� AP ���� LV5
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
	
	-- ũ��Ƽ�� ���� LV4
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

	-- ũ��Ƽ�� ���� LV5
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