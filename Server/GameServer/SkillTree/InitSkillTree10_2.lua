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
	--타임 마블 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_TIME_MARBLE,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV3, SID_DIO2_CHAOTICSPARK_SPECIAL_LV5, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon908.dds",
		TitleID 	=	2012,
		DescID 		=	2072,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	3,

		LockGroup = -1,		
    },		

    --타임 마블 지속 시간 LV1
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_TIME_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon909.dds",
		TitleID 	=	2013,
		DescID 		=	2073,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	12,

		LockGroup = -1,		
    },		

    --타임 마블 지속 시간 LV2
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_TIME_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV1, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon910.dds",
		TitleID 	=	2014,
		DescID 		=	2074,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

    --타임 마블 지속 시간 LV3
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_TIME_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV2, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon911.dds",
		TitleID 	=	2015,
		DescID 		=	2075,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

    --타임 마블 강화 LV1
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_UPGRADE_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV3, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon912.dds",
		TitleID 	=	2016,
		DescID 		=	2076,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	17,

		LockGroup = -1,		
    },

    --타임 마블 강화 LV2
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_UPGRADE_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV1, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon913.dds",
		TitleID 	=	2017,
		DescID 		=	2077,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --타임 마블 강화 LV3
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_UPGRADE_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV2, },
		NextList 	=	{SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon914.dds",
		TitleID 	=	2018,
		DescID 		=	2078,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
	-- 터네이셔스 홀드 LV1
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO2_BLOCKER_TIMEUP_LV2, SID_DIO3_TIME_MARBLE_UPGRADE_LV3, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon915.dds",
		TitleID 	=	2019,
		DescID 		=	2079,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 터네이셔스 홀드 LV2
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon916.dds",
		TitleID 	=	2020,
		DescID 		=	2080,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 터네이셔스 홀드 LV3
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV2, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon917.dds",
		TitleID 	=	2021,
		DescID 		=	2081,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 터네이셔스 홀드 LV4
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV3, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon918.dds",
		TitleID 	=	2022,
		DescID 		=	2082,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 터네이셔스 홀드 LV5
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV4, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon919.dds",
		TitleID 	=	2023,
		DescID 		=	2083,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 프라우드 스윙 LV1
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO2_CHARGING_DS_W_LV5, SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV5, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon920.dds",
		TitleID 	=	2024,
		DescID 		=	2084,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV2
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV1, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon921.dds",
		TitleID 	=	2025,
		DescID 		=	2085,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV3
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV2, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon922.dds",
		TitleID 	=	2026,
		DescID 		=	2086,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV4
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV3, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon923.dds",
		TitleID 	=	2027,
		DescID 		=	2087,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV5
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV4, },
		NextList 	=	{ SID_DIO3_SPELL_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon924.dds",
		TitleID 	=	2028,
		DescID 		=	2088,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

    --스펠 마블 (AP 소모량 감소)
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_PROUD_SWING_SPECIAL_LV5, SID_DIO3_COUNT_COOLTIME_LV5, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon925.dds",
		TitleID 	=	2029,
		DescID 		=	2089,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	3,

		LockGroup = -1,
    },

    --스펠 마블 지속 시간 LV1
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_TIME_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon926.dds",
		TitleID 	=	2030,
		DescID 		=	2090,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
    },	

    --스펠 마블 지속 시간 LV2
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_TIME_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV1, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon927.dds",
		TitleID 	=	2031,
		DescID 		=	2091,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --스펠 마블 지속 시간 LV3
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_TIME_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV2, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon928.dds",
		TitleID 	=	2032,
		DescID 		=	2092,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --스펠 마블 강화 LV1
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_UPGRADE_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV3, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon929.dds",
		TitleID 	=	2033,
		DescID 		=	2093,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
    },

    --스펠 마블 강화 LV2
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_UPGRADE_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV1, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon930.dds",
		TitleID 	=	2034,
		DescID 		=	2094,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --스펠 마블 강화 LV3
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_UPGRADE_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV2, },
		NextList 	=	{SID_DIO3_STICK_LANCE_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon931.dds",
		TitleID 	=	2035,
		DescID 		=	2095,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- 스틱 랜스 LV1
    {		
		ID 			=	SID_DIO3_STICK_LANCE_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_STICK_LANCE_SPECIAL,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV3, },
		NextList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	2036,
		DescID 		=	2096,
		ControlDesc	=	127,

		Img 		=	"SkillIcon932.dds",
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_STICK_LANCE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 스틱 랜스 LV2 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_STICK_LANCE_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_STICK_LANCE_SPECIAL,
		PreList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV1, },
		NextList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	2037,
		DescID 		=	2097,
		ControlDesc	=	127,

		Img 		=	"SkillIcon933.dds",
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_STICK_LANCE,			
			CoolTime = 17,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 15,
		},
    },

	-- 스틱 랜스 LV3 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_STICK_LANCE_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_STICK_LANCE_SPECIAL,
		PreList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon934.dds",
		TitleID 	=	2038,
		DescID 		=	2098,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_STICK_LANCE,			
			CoolTime = 14,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 13,
		},
    },

    -- 콤보 스피드업 LV1
    {		
		ID 			=	SID_DIO3_COMBO_SPEED_UP_LV1,
		GroupID 	=	SG_DIO3_COMBO_SPEED,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV5, SID_DIO3_COMBO_ATK_DMG_LV2, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon900.dds",
		TitleID 	=	2006,
		DescID 		=	2066,
		ControlDesc	=	1169,

		UI_Pos_x 	=	220,--dio2
		UI_Pos_y 	=	186,--dio2
		UI_LineType =	3,

		LockGroup = -1,
    },

	-- 그립 LV1
    {		
		ID 			=	SID_DIO3_GRIP_NORMAL_LV1,
		GroupID 	=	SG_DIO3_GRIP_NORMAL,
		PreList 	=	{SID_DIO3_COMBO_SPEED_UP_LV1, SID_DIO3_DASH_ATK_DMG_LV2, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon892.dds",
		TitleID 	=	1998,
		DescID 		=	2058,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO3_GRIP,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },

	-- 그립 LV2 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_GRIP_NORMAL_LV2,
		GroupID 	=	SG_DIO3_GRIP_NORMAL,
		PreList 	=	{SID_DIO3_GRIP_NORMAL_LV1, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon893.dds",
		TitleID 	=	1999,
		DescID 		=	2059,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO3_GRIP,			
			CoolTime = 4,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 4,
		},
    },

	-- 그립 LV3 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_GRIP_NORMAL_LV3,
		GroupID 	=	SG_DIO3_GRIP_NORMAL,
		PreList 	=	{SID_DIO3_GRIP_NORMAL_LV2, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon894.dds",
		TitleID 	=	2000,
		DescID 		=	2060,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO3_GRIP,			
			CoolTime = 3,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 3,
		},
    },

	-- 쿨 스파이럴 LV1 (1단 필살기)
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_GRIP_NORMAL_LV3, SID_DIO3_CRI_DOUBLE_DMG_LV2, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon895.dds",
		TitleID 	=	2001,
		DescID 		=	2061,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV2
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV1, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,


		Img 		=	"SkillIcon896.dds",
		TitleID 	=	2002,
		DescID 		=	2062,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV3
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV2, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon897.dds",
		TitleID 	=	2003,
		DescID 		=	2063,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV4
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV3, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon898.dds",		
		TitleID 	=	2004,
		DescID 		=	2064,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV5
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV4, },
		NextList 	=	{SID_DIO3_POWER_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon899.dds",
		TitleID 	=	2005,
		DescID 		=	2065,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
	--파워 마블 (공격력 증가)
	{		
		ID 			=	SID_DIO3_POWER_MARBLE,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV5, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon885.dds",
		TitleID 	=	1991,
		DescID 		=	2051,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	220,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	3,

		LockGroup = -1,
    },

    --파워 마블 지속 시간 LV1
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_TIME_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE, SID_DIO1_AP_SPEED_LV2, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon886.dds",
		TitleID 	=	1992,
		DescID 		=	2052,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	143,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
    },	

    --파워 마블 지속 시간 LV2
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_TIME_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV1, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon887.dds",
		TitleID 	=	1993,
		DescID 		=	2053,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	143,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --파워 마블 지속 시간 LV3
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_TIME_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV2, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon888.dds",
		TitleID 	=	1994,
		DescID 		=	2054,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	143,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --파워 마블 강화 LV1
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_UPGRADE_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV3, SID_DIO1_AP_START_LV3, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon889.dds",
		TitleID 	=	1995,
		DescID 		=	2055,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	298,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
    },

    --파워 마블 강화 LV2
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_UPGRADE_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV1, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon890.dds",
		TitleID 	=	1996,
		DescID 		=	2056,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	298,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --파워 마블 강화 LV3
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_UPGRADE_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV2, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon891.dds",
		TitleID 	=	1997,
		DescID 		=	2057,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	298,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- 이블 에디 LV1
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV2, SID_DIO3_POWER_MARBLE_UPGRADE_LV3, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon903.dds",
		TitleID 	=	2007,
		DescID 		=	2067,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV2
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV1, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon904.dds",
		TitleID 	=	2008,
		DescID 		=	2068,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV3
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV2, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon905.dds",
		TitleID 	=	2009,
		DescID 		=	2069,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV4
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV3, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon906.dds",
		TitleID 	=	2010,
		DescID 		=	2070,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV5
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV4, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon907.dds",
		TitleID 	=	2011,
		DescID 		=	2071,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
    -- 2차전직 콤보
    {		
		ID 			=	SID_DIO3_COMBO_ATK,
		GroupID 	=	SG_DIO3_COMBO_ATK,
		PreList 	=	{SID_DIO2_COMBO_ATK_LV2, },
		NextList 	=	{SID_DIO3_COMBO_ATK_DMG_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon935.dds",
		TitleID 	=	2039,
		DescID 		=	2099,
		ControlDesc	=	221,

		UI_Pos_x 	=	205,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
		
    },
	
    -- 2차전직 콤보 데미지 증가 LV1
    {		
		ID 			=	SID_DIO3_COMBO_ATK_DMG_LV1,
		GroupID 	=	SG_DIO3_COMBO_ATK,
		PreList 	=	{SID_DIO3_COMBO_ATK, },
		NextList 	=	{SID_DIO3_COMBO_ATK_DMG_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon936.dds",
		
		TitleID 	=	2040,
		DescID 		=	2100,
		ControlDesc	=	221,

		UI_Pos_x 	=	205,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },	
	
    -- 2차전직 콤보 데미지 증가 LV2
    {		
		ID 			=	SID_DIO3_COMBO_ATK_DMG_LV2,
		GroupID 	=	SG_DIO3_COMBO_ATK,
		PreList 	=	{SID_DIO3_COMBO_ATK_DMG_LV1, },
		NextList 	=	{SID_DIO3_COMBO_SPEED_UP_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon937.dds",
		
		TitleID 	=	2041,
		DescID 		=	2101,
		ControlDesc	=	221,

		UI_Pos_x 	=	205,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
    },
	
    -- 2차전직 대시 어택
    {		
		ID 			=	SID_DIO3_DASH_ATK,
		GroupID 	=	SG_DIO3_DASH_ATK,
		PreList 	=	{SID_DIO2_JUMP_ATK_LV2, SID_DIO2_JUMP_PLUS_LV3, },
		NextList 	=	{SID_DIO3_DASH_ATK_DMG_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon938.dds",
		TitleID 	=	2042,
		DescID 		=	2102,
		ControlDesc	=	773,

		UI_Pos_x 	=	305,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		
		
    },	
	
    -- 2차전직 대시 어택 데미지 증가 LV1
    {		
		ID 			=	SID_DIO3_DASH_ATK_DMG_LV1,
		GroupID 	=	SG_DIO3_DASH_ATK,
		PreList 	=	{SID_DIO3_DASH_ATK, },
		NextList 	=	{SID_DIO3_DASH_ATK_DMG_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon939.dds",
		
		TitleID 	=	2043,
		DescID 		=	2103,
		ControlDesc	=	773,

		UI_Pos_x 	=	305,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,	
		
    },	
	
    -- 2차전직 대시 어택 데미지 증가 LV2
    {		
		ID 			=	SID_DIO3_DASH_ATK_DMG_LV2,
		GroupID 	=	SG_DIO3_DASH_ATK,
		PreList 	=	{SID_DIO3_DASH_ATK_DMG_LV1, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon940.dds",
		TitleID 	=	2044,
		DescID 		=	2104,
		ControlDesc	=	773,

		UI_Pos_x 	=	305,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
    },		
	
    -- 2차전직 크리&더블 어택
    {		
		ID 			=	SID_DIO3_CRI_DOUBLE,
		GroupID 	=	SG_DIO3_CRI_DOUBLE,
		PreList 	=	{SID_DIO2_COMBO_PLUS_LV3, },
		NextList 	=	{SID_DIO3_CRI_DOUBLE_DMG_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon941.dds",
		
		TitleID 	=	2045,
		DescID 		=	2105,
		ControlDesc	=	130,

		UI_Pos_x 	=	405,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,			
		
    },	
	
    -- 2차전직 크리&더블 어택 데미지 증가 LV1
    {		
		ID 			=	SID_DIO3_CRI_DOUBLE_DMG_LV1,
		GroupID 	=	SG_DIO3_CRI_DOUBLE,
		PreList 	=	{SID_DIO3_CRI_DOUBLE, },
		NextList 	=	{SID_DIO3_CRI_DOUBLE_DMG_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon942.dds",
		
		TitleID 	=	2046,
		DescID 		=	2106,
		ControlDesc	=	130,

		UI_Pos_x 	=	405,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
	
    },		
	
    -- 2차전직 크리&더블 어택 데미지 증가 LV2
    {		
		ID 			=	SID_DIO3_CRI_DOUBLE_DMG_LV2,
		GroupID 	=	SG_DIO3_CRI_DOUBLE,
		PreList 	=	{SID_DIO3_CRI_DOUBLE_DMG_LV1, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon943.dds",
		
		TitleID 	=	2047,
		DescID 		=	2107,
		ControlDesc	=	130,

		UI_Pos_x 	=	405,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
    },
	
	--카운터 쿨 타임 감소 LV3
	{		
		ID 			=	SID_DIO3_COUNT_COOLTIME_LV3,
		GroupID 	=	SG_DIO3_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV2, },
		NextList 	=	{SID_DIO3_COUNT_COOLTIME_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon944.dds",
		TitleID 	=	2048,
		DescID 		=	2108,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		
    },

	--카운터 쿨 타임 감소 LV4
	{		
		ID 			=	SID_DIO3_COUNT_COOLTIME_LV4,
		GroupID 	=	SG_DIO3_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV3, },
		NextList 	=	{SID_DIO3_COUNT_COOLTIME_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon945.dds",
		TitleID 	=	2049,
		DescID 		=	2109,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		
    },

	--카운터 쿨 타임 감소 LV5
	{		
		ID 			=	SID_DIO3_COUNT_COOLTIME_LV5,
		GroupID 	=	SG_DIO3_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV4, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon946.dds",
		TitleID 	=	2050,
		DescID 		=	2110,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		
    },
}