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
	-- 클래시컬 포스 기본
    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1477,
		DescID 		=	1553,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1478,
		DescID 		=	1554,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,


		LockGroup = -1,				
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_HIT_SPECIAL_LV1, SID_ZERO1_ICEFORCE_SPECIAL_LV1, SID_ZERO1_FIREFORCE_SPECIAL_LV1, SID_ZERO1_LOSS_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1479,
		DescID 		=	1555,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,


		LockGroup = -1,				
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	-- 클래시컬 포스 기본 히트 수 증가
    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_HIT_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_HIT_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1482,
		DescID 		=	1558,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		Passive = TRUE,
		LockGroup = -1,				
		
    },

	-- 클래시컬 포스 기본 공격 거리 증가
    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_HIT_SPECIAL_LV1, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1485,
		DescID 		=	1561,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		LockGroup = -1,				
		
		
    },	

	-- 클래시컬 포스 얼음
    {		
		ID 			=	SID_ZERO1_ICEFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV2, SID_ZERO2_POSIONFORCE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1487,
		DescID 		=	1563,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	0,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_ICEFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1488,
		DescID 		=	1564,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	0,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_ICEFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_ICEFORCE_TIME_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1489,
		DescID 		=	1565,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	0,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },		

	-- 클래시컬 포스 얼음 지속 시간 증가
    {		
		ID 			=	SID_ZERO1_ICEFORCE_TIME_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_ICEFORCE_TIME_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_ICEFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1492,
		DescID 		=	1568,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	

		LockGroup = -1,				
		
		
    },	

	
	-- 클래시컬 포스 얼음 공격 거리 증가
    {		
		ID 			=	SID_ZERO1_ICEFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_ICEFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_TIME_SPECIAL_LV1, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1495,
		DescID 		=	1571,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT+ Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		

		LockGroup = -1,				
    },	

	-- 클래시컬 포스 불
    {		
		ID 			=	SID_ZERO1_FIREFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV2, SID_ZERO2_POSIONFORCE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1497,
		DescID 		=	1573,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	1,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_FIREFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1498,
		DescID 		=	1574,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	1,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	
    {		
		ID 			=	SID_ZERO1_FIREFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_FIREFORCE_TIME_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1499,
		DescID 		=	1575,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	1,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	
	-- 클래시컬 포스 불 지속 시간 증가
    {		
		ID 			=	SID_ZERO1_FIREFORCE_TIME_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_FIREFORCE_TIME_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_FIREFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1502,
		DescID 		=	1578,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		
		LockGroup = -1,				
		
		
    },	
	
	-- 클래시컬 포스 불 공격 거리 증가
    {		
		ID 			=	SID_ZERO1_FIREFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_FIREFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_TIME_SPECIAL_LV1, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1505,
		DescID 		=	1581,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		
		LockGroup = -1,				
		
		
    },

	-- 트리플 베쉬
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1507,
		DescID 		=	1583,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 ,
		UI_LineType =	-1,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_TRIPLEBASH,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1508,
		DescID 		=	1584,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 ,
		UI_LineType =	-1,				
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_TRIPLEBASH_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1509,
		DescID 		=	1585,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 ,
		UI_LineType =	-1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_TRIPLEBASH_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 트리플 베쉬 공격력 증가
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1510,
		DescID 		=	1586,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 ,
		UI_LineType =	1,		

		LockGroup = -1,				
    },
	
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_GRANX_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1511,
		DescID 		=	1587,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 ,
		UI_LineType =	1,		
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		
    },	
	
	-- 그란엑스
    {		
		ID 			=	SID_ZERO1_GRANX_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_GRANX_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1513,
		DescID 		=	1589,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO1_GRANX_A,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		
	
    {		
		ID 			=	SID_ZERO1_GRANX_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL,
		PreList 	=	{SID_ZERO1_GRANX_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1514,
		DescID 		=	1590,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO1_GRANX_A,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	
	
	-- 그란엑스 공격력 증가
	{		
		ID 			=	SID_ZERO1_GRANX_DMG_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_GRANX_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1515,
		DescID 		=	1591,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	1,		

		LockGroup = -1,				
		
		
    },
	
	{		
		ID 			=	SID_ZERO1_GRANX_DMG_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1516,
		DescID 		=	1592,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	1,		
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		
    },	
	
	{		
		ID 			=	SID_ZERO1_GRANX_DMG_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1517,
		DescID 		=	1593,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
    },
	
	-- 프리클 가쉬
	{		
		ID 			=	SID_ZERO1_PRICKLEGUSH_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_PRICKLEGUSH_SPECIAL,
		PreList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1518,
		DescID 		=	1594,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,				

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO1_PRICKLEGUSH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_PRICKLEGUSH_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_PRICKLEGUSH_SPECIAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1519,
		DescID 		=	1595,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO1_PRICKLEGUSH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_PRICKLEGUSH_SPECIAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV1, SID_ZERO2_BREAKTH_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1520,
		DescID 		=	1596,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO1_PRICKLEGUSH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 로스
	{		
		ID 			=	SID_ZERO1_LOSS_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_LOSS_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1523,
		DescID 		=	1599,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 ,
		UI_Pos_y 	=	430 ,
		UI_LineType =	-1,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_LOSS_NORMAL_LV2,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV1, },
		NextList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1524,
		DescID 		=	1600,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 ,
		UI_Pos_y 	=	430 ,
		UI_LineType =	-1,	
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		
	
	{		
		ID 			=	SID_ZERO1_LOSS_NORMAL_LV3,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV2, },
		NextList 	=	{SID_ZERO1_LOSS_HP_NORMAL_LV1, SID_ZERO1_LOSS_AP_NORMAL_LV1, SID_ZERO2_ORCHID_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1525,
		DescID 		=	1601,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 ,
		UI_Pos_y 	=	430 ,
		UI_LineType =	MID_ZERO1_LOSS,	
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

	-- 로스 HP 흡수
	{		
		ID 			=	SID_ZERO1_LOSS_HP_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1528,
		DescID 		=	1604,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	430 ,
		UI_LineType =	1,	
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

	-- 로스 AP 흡수
	{		
		ID 			=	SID_ZERO1_LOSS_AP_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1533,
		DescID 		=	1609,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	430 + Y_GAP_QUICKSLOT,
		UI_LineType =	18,	
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	

	-- 루드 프레셔
	{		
		ID 			=	SID_ZERO1_RUDEPRESSURE_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_RUDEPRESSURE_NORMAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1538,
		DescID 		=	1614,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_RUDPRESSURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		
	
	{		
		ID 			=	SID_ZERO1_RUDEPRESSURE_NORMAL_LV2,
		GroupID 	=	SG_ZERO1_RUDEPRESSURE_NORMAL,
		PreList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV1, },
		NextList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1539,
		DescID 		=	1615,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	
		Overlapped  = TRUE,


		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_RUDPRESSURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_RUDEPRESSURE_NORMAL_LV3,
		GroupID 	=	SG_ZERO1_RUDEPRESSURE_NORMAL,
		PreList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV2, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1540,
		DescID 		=	1616,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_RUDPRESSURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	

	-- 오리지널 스텐스 대공
    {		
		ID 			=	SID_ZERO1_OS_UPATK_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_UPATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1543,
		DescID 		=	1619,
		ControlDesc	=	1629,

		UI_Pos_x 	=	394,
		UI_Pos_y 	=	249,
		UI_LineType =	2,
		LockGroup = -1,
    },	
	
	-- 오리지널 스텐스 전방
    {		
		ID 			=	SID_ZERO1_OS_FRONTATK_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_FRONTATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1544,
		DescID 		=	1620,
		ControlDesc	=	1630,

		UI_Pos_x 	=	294,
		UI_Pos_y 	=	249,
		UI_LineType =	2,

		LockGroup = -1,
    },	
	
	-- 오리지널 스텐스 후방
    {		
		ID 			=	SID_ZERO1_OS_BACKATK_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_BACKATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1545,
		DescID 		=	1621,
		ControlDesc	=	1631,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	249,
		UI_LineType =	2,

		LockGroup = -1,
    },	
	
	-- 오리지널 스텐스 이동
    {		
		ID 			=	SID_ZERO1_OS_MOVE_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_MOVE,
		PreList 	=	{},
		NextList 	=	{SID_ZERO2_OS_COMBO_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1546,
		DescID 		=	1622,
		ControlDesc	=	1169,

		UI_Pos_x 	=	494,
		UI_Pos_y 	=	249,
		UI_LineType =	2,

		LockGroup = -1,
    },		
	
	-- 디펜스 스텐스
    {		
		ID 			=	SID_ZERO1_DS_LV1,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE,
		PreList 	=	{},
		NextList 	=	{SID_ZERO1_DS_LV2, SID_ZERO2_INCREASE_DS_NORMAL_LV1, SID_ZERO1_DS_BACKATK_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1547,
		DescID 		=	1623,
		ControlDesc	=	1632,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	419,
		UI_LineType =	2,

		LockGroup = -1,
    },	
	
    {		
		ID 			=	SID_ZERO1_DS_LV2,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE,
		PreList 	=	{SID_ZERO1_DS_LV1, },
		NextList 	=	{SID_ZERO1_DS_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1548,
		DescID 		=	1624,
		ControlDesc	=	1632,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	419,
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,
    },		
	
    {		
		ID 			=	SID_ZERO1_DS_LV3,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE,
		PreList 	=	{SID_ZERO1_DS_LV2, },
		NextList 	=	{SID_ZERO1_DS_LV4, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1549,
		DescID 		=	1625,
		ControlDesc	=	1632,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	419,
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,
    },		

	-- 디펜스 스텐스 후방
    {		
		ID 			=	SID_ZERO1_DS_BACKATK_LV1,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE_BACKATK,
		PreList 	=	{SID_ZERO1_DS_LV1, },
		NextList 	=	{SID_ZERO2_DS_UP_ATK_LV1, SID_ZERO2_DS_DOWN_ATK_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1552,
		DescID 		=	1628,
		ControlDesc	=	1633,

		UI_Pos_x 	=	294,
		UI_Pos_y 	=	419,
		UI_LineType =	2,		

		LockGroup = -1,
    },		
}