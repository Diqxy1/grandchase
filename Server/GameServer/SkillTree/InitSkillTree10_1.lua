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
	-- 리바운드
    {		
		ID 			=	SID_DIO2_REBOUND_NORMAL_LV1,
		GroupID 	=	SG_DIO2_REBOUND_NORMAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO2_REBOUND_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1207,
		DescID 		=	1265,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_REBOUND,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_REBOUND_NORMAL_LV2,
		GroupID 	=	SG_DIO2_REBOUND_NORMAL,
		PreList 	=	{SID_DIO2_REBOUND_NORMAL_LV1, },
		NextList 	=	{SID_DIO2_REBOUND_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1208,
		DescID 		=	1266,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_REBOUND,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_REBOUND_NORMAL_LV3,
		GroupID 	=	SG_DIO2_REBOUND_NORMAL,
		PreList 	=	{SID_DIO2_REBOUND_NORMAL_LV2, },
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV1, SID_DIO2_REBOUND_LENGTH_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1209,
		DescID 		=	1267,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_REBOUND,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
-- 버티컬 블링크
	{		
		ID 			=	SID_DIO2_VERTICALBLINK_NORMAL_LV1,
		GroupID 	=	SG_DIO2_VERTICALBLINK_NORMAL,
		PreList 	=	{SID_DIO1_MOVE_LV1, SID_DIO1_BLINK_NORMAL_LV2, },
		NextList 	=	{SID_DIO2_VERTICALBLINK_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1210,
		DescID 		=	1268,
		ControlDesc	=	1169,

		UI_Pos_x 	=	274,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_VERTICALBLINK,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_VERTICALBLINK_NORMAL_LV2,
		GroupID 	=	SG_DIO2_VERTICALBLINK_NORMAL,
		PreList 	=	{SID_DIO2_VERTICALBLINK_NORMAL_LV1, },
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1211,
		DescID 		=	1269,
		ControlDesc	=	1169,

		UI_Pos_x 	=	274,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_VERTICALBLINK,			
			CoolTime = 3,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 3,
		},
    },
	
-- 이블 쇼크
    {		
		ID 			=	SID_DIO2_EVILSHOCK_SPECIAL_LV1,
		GroupID 	=	SG_DIO2_EVILSHOCK_SPECIAL,
		PreList 	=	{SID_DIO2_REBOUND_NORMAL_LV3, SID_DIO2_EATOFF_APDOWN_NORMAL_LV1, SID_DIO2_VERTICALBLINK_NORMAL_LV2,},
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1213,
		DescID 		=	1271,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 16,17 },

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_EVILSHOCK_INSTALL,			
			CoolTime = 1,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 1,
		},
    },
	
    {		
		ID 			=	SID_DIO2_EVILSHOCK_SPECIAL_LV2,
		GroupID 	=	SG_DIO2_EVILSHOCK_SPECIAL,
		PreList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV1, },
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1214,
		DescID 		=	1272,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 16,17 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_EVILSHOCK_INSTALL,					
			CoolTime = 1,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 1,
		},
    },
	
    {		
		ID 			=	SID_DIO2_EVILSHOCK_SPECIAL_LV3,
		GroupID 	=	SG_DIO2_EVILSHOCK_SPECIAL,
		PreList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV2, },
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1215,
		DescID 		=	1273,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 16,17 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_EVILSHOCK_INSTALL,					
			CoolTime = 1,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 1,
		},
    },
	
    {		
		ID 			=	SID_DIO2_EVILSHOCK_SPECIAL_LV4,
		GroupID 	=	SG_DIO2_EVILSHOCK_SPECIAL,
		PreList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV3, },
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1329,
		DescID 		=	1331,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 16,17 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_EVILSHOCK_INSTALL,					
			CoolTime = 1,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 1,
		},
    },
	
    {		
		ID 			=	SID_DIO2_EVILSHOCK_SPECIAL_LV5,
		GroupID 	=	SG_DIO2_EVILSHOCK_SPECIAL,
		PreList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV4, },
		NextList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1330,
		DescID 		=	1332,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 16,17 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_EVILSHOCK_INSTALL,					
			CoolTime = 1,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 1,
		},
    },
	
-- 라이프 킵	
    {		
		ID 			=	SID_DIO2_LIFEKEEP_NORMAL_LV1,
		GroupID 	=	SG_DIO2_LIFEKEEP_NORMAL,
		PreList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV5, },
		NextList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1216,
		DescID 		=	1274,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	{ 2 },

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO2_LIFEKEEP,			
			CoolTime = 99,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 99,
		},
    },
	
    {		
		ID 			=	SID_DIO2_LIFEKEEP_NORMAL_LV2,
		GroupID 	=	SG_DIO2_LIFEKEEP_NORMAL,
		PreList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV1, },
		NextList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1217,
		DescID 		=	1275,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	{ 2 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO2_LIFEKEEP,			
			CoolTime = 99,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 99,
		},
    },
	
    {		
		ID 			=	SID_DIO2_LIFEKEEP_NORMAL_LV3,
		GroupID 	=	SG_DIO2_LIFEKEEP_NORMAL,
		PreList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV2, },
		NextList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1218,
		DescID 		=	1276,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	{ 2 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO2_LIFEKEEP,			
			CoolTime = 99,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 99,
		},
    },	
	
    {		
		ID 			=	SID_DIO2_LIFEKEEP_NORMAL_LV4,
		GroupID 	=	SG_DIO2_LIFEKEEP_NORMAL,
		PreList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV3, },
		NextList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1333,
		DescID 		=	1335,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	{ 2 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO2_LIFEKEEP,			
			CoolTime = 99,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 99,
		},
    },
	
    {		
		ID 			=	SID_DIO2_LIFEKEEP_NORMAL_LV5,
		GroupID 	=	SG_DIO2_LIFEKEEP_NORMAL,
		PreList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV4, },
		NextList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1334,
		DescID 		=	1336,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	{ 2 },
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO2_LIFEKEEP,			
			CoolTime = 99,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 99,
		},
    },
	
-- 킬 존	
    {		
		ID 			=	SID_DIO2_KILLZONE_SPECIAL_LV1,
		GroupID 	=	SG_DIO2_KILLZONE_SPECIAL,
		PreList 	=	{SID_DIO2_LIFEKEEP_NORMAL_LV5, },
		NextList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1219,
		DescID 		=	1277,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	2,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_KILLZONE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
    {		
		ID 			=	SID_DIO2_KILLZONE_SPECIAL_LV2,
		GroupID 	=	SG_DIO2_KILLZONE_SPECIAL,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV1, },
		NextList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1220,
		DescID 		=	1278,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_KILLZONE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
    {		
		ID 			=	SID_DIO2_KILLZONE_SPECIAL_LV3,
		GroupID 	=	SG_DIO2_KILLZONE_SPECIAL,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV2, },
		NextList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1221,
		DescID 		=	1279,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_KILLZONE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		
	
    {		
		ID 			=	SID_DIO2_KILLZONE_SPECIAL_LV4,
		GroupID 	=	SG_DIO2_KILLZONE_SPECIAL,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV3, },
		NextList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1337,
		DescID 		=	1339,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_KILLZONE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
    {		
		ID 			=	SID_DIO2_KILLZONE_SPECIAL_LV5,
		GroupID 	=	SG_DIO2_KILLZONE_SPECIAL,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV4, },
		NextList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV1, SID_DIO3_COMBO_SPEED_UP_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1338,
		DescID 		=	1340,
		ControlDesc	=	1169,

		UI_Pos_x 	=	200,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_KILLZONE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
-- 킬 존 지속 시간 증가	
    {		
		ID 			=	SID_DIO2_KILLZONE_TIME_NORMAL_LV1,
		GroupID 	=	SG_DIO2_KILLZONE_TIMEUP,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV5, SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV2},
		NextList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1222,
		DescID 		=	1280,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	15,
		LockGroup = -1,				
		Passive = TRUE,
    },
	
    {		
		ID 			=	SID_DIO2_KILLZONE_TIME_NORMAL_LV2,
		GroupID 	=	SG_DIO2_KILLZONE_TIMEUP,
		PreList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV1, },
		NextList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1223,
		DescID 		=	1281,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	15,
		Overlapped  = TRUE,

		LockGroup = -1,				
		Passive = TRUE,
    },
	
    {		
		ID 			=	SID_DIO2_KILLZONE_TIME_NORMAL_LV3,
		GroupID 	=	SG_DIO2_KILLZONE_TIMEUP,
		PreList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV2, },
		NextList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1224,
		DescID 		=	1282,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	15,
		Overlapped  = TRUE,

		LockGroup = -1,				
		Passive = TRUE,
    },	
	
    {		
		ID 			=	SID_DIO2_KILLZONE_TIME_NORMAL_LV4,
		GroupID 	=	SG_DIO2_KILLZONE_TIMEUP,
		PreList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV3, },
		NextList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1341,
		DescID 		=	1343,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	15,
		Overlapped  = TRUE,

		LockGroup = -1,				
		Passive = TRUE,
    },	
	
    {		
		ID 			=	SID_DIO2_KILLZONE_TIME_NORMAL_LV5,
		GroupID 	=	SG_DIO2_KILLZONE_TIMEUP,
		PreList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV4, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1342,
		DescID 		=	1344,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	15,
		Overlapped  = TRUE,

		LockGroup = -1,				
		Passive = TRUE,
    },	
	
-- 잇 오프 히트수 증가	
    {		
		ID 			=	SID_DIO2_EATOFF_APDOWN_NORMAL_LV1,
		GroupID 	=	SG_DIO2_EATOFF_APDOWN,
		PreList 	=	{SID_DIO1_EATOFF_SPECIAL_LV3, },
		NextList 	=	{SID_DIO2_EVILSHOCK_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1225,
		DescID 		=	1283,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	-1,

		LockGroup = -1,				
		Passive = TRUE,
		
    },	
	
-- 블랙 스페이스 데미지 증가	
    {		
		ID 			=	SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV1,
		GroupID 	=	SG_DIO2_BLACKSPACE_APDOWN,
		PreList 	=	{SID_DIO1_BLACKSPACE_LV1, },
		NextList 	=	{SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1226,
		DescID 		=	1284,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	-1,

		LockGroup = -1,				
		
		Passive = TRUE,
    },		
	
    {		
		ID 			=	SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV2,
		GroupID 	=	SG_DIO2_BLACKSPACE_APDOWN,
		PreList 	=	{SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV1, },
		NextList 	=	{SID_DIO2_KILLZONE_TIME_NORMAL_LV1, SID_DIO3_EVIL_EDDY_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1345,
		DescID 		=	1346,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,		
		LockGroup = -1,				
		Passive = TRUE,
    },		
	
-- 챠징 데스스타 TYPE - 1
    {		
		ID 			=	SID_DIO2_CHARGING_DS_W_LV1,
		GroupID 	=	SG_DIO2_CHARGING_DS_W_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_H_LV5, },
		NextList 	=	{SID_DIO2_CHARGING_DS_W_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1230,
		DescID 		=	1285,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_W,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_W_LV2,
		GroupID 	=	SG_DIO2_CHARGING_DS_W_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_W_LV1, },
		NextList 	=	{SID_DIO2_CHARGING_DS_W_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1231,
		DescID 		=	1286,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,
		Overlapped  = TRUE,		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_W,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_W_LV3,
		GroupID 	=	SG_DIO2_CHARGING_DS_W_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_W_LV2, },
		NextList 	=	{SID_DIO2_CHARGING_DS_W_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1232,
		DescID 		=	1287,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_W,	
			CoolTime = 5,

			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,			
		},
    },	
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_W_LV4,
		GroupID 	=	SG_DIO2_CHARGING_DS_W_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_W_LV3, },
		NextList 	=	{SID_DIO2_CHARGING_DS_W_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1349,
		DescID 		=	1351,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_W,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_W_LV5,
		GroupID 	=	SG_DIO2_CHARGING_DS_W_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_W_LV4, },
		NextList 	=	{SID_DIO3_PROUD_SWING_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1350,
		DescID 		=	1352,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_W,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
-- 챠징 데스스타 - 2	
	{		
		ID 			=	SID_DIO2_CHARGING_DS_H_LV1,
		GroupID 	=	SG_DIO2_CHARGING_DS_H_NORMAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO2_CHARGING_DS_H_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1227,
		DescID 		=	1288,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_H,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_H_LV2,
		GroupID 	=	SG_DIO2_CHARGING_DS_H_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_H_LV1, },
		NextList 	=	{SID_DIO2_CHARGING_DS_H_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1228,
		DescID 		=	1289,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_H,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_H_LV3,
		GroupID 	=	SG_DIO2_CHARGING_DS_H_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_H_LV2, },
		NextList 	=	{SID_DIO2_CHARGING_DS_H_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1229,
		DescID 		=	1290,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_H,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_H_LV4,
		GroupID 	=	SG_DIO2_CHARGING_DS_H_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_H_LV3, },
		NextList 	=	{SID_DIO2_CHARGING_DS_H_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1347,
		DescID 		=	1353,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_H,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
    {		
		ID 			=	SID_DIO2_CHARGING_DS_H_LV5,
		GroupID 	=	SG_DIO2_CHARGING_DS_H_NORMAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_H_LV4, },
		NextList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV1, SID_DIO2_CHARGING_DS_W_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1348,
		DescID 		=	1354,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	-1,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_CHARGING_DS_H,	
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
-- 블로커
	{		
		ID 			=	SID_DIO2_BLOCKER_SPECIAL_LV1,
		GroupID 	=	SG_DIO2_BLOCKER_SPECIAL,
		PreList 	=	{SID_DIO2_CHARGING_DS_H_LV5, SID_DIO2_HAUNTINGSHOCK_APDOWN_LV1, },
		NextList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1233,
		DescID 		=	1291,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 2, 12,	},

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1,
			Motion = MID_DIO2_BLOCKER,			
			CoolTime = 5,
			
			MonsterModeMana = 1,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_BLOCKER_SPECIAL_LV2,
		GroupID 	=	SG_DIO2_BLOCKER_SPECIAL,
		PreList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV1, },
		NextList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1234,
		DescID 		=	1292,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1,
			Motion = MID_DIO2_BLOCKER,			
			CoolTime = 5,
			
			MonsterModeMana = 1,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_BLOCKER_SPECIAL_LV3,
		GroupID 	=	SG_DIO2_BLOCKER_SPECIAL,
		PreList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV2, },
		NextList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1235,
		DescID 		=	1293,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1,
			Motion = MID_DIO2_BLOCKER,			
			CoolTime = 5,
			
			MonsterModeMana = 1,
			MonsterModeCoolTime = 5,
		},
    },	
	
    {		
		ID 			=	SID_DIO2_BLOCKER_SPECIAL_LV4,
		GroupID 	=	SG_DIO2_BLOCKER_SPECIAL,
		PreList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV3, },
		NextList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1355,
		DescID 		=	1357,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1,
			Motion = MID_DIO2_BLOCKER,			
			CoolTime = 5,
			
			MonsterModeMana = 1,
			MonsterModeCoolTime = 5,
		},
    },	
	
    {		
		ID 			=	SID_DIO2_BLOCKER_SPECIAL_LV5,
		GroupID 	=	SG_DIO2_BLOCKER_SPECIAL,
		PreList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV4, },
		NextList 	=	{SID_DIO2_BLOCKER_TIMEUP_LV1, SID_DIO2_FARAWAW_NORMAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1356,
		DescID 		=	1358,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1,
			Motion = MID_DIO2_BLOCKER,			
			CoolTime = 5,
			
			MonsterModeMana = 1,
			MonsterModeCoolTime = 5,
		},
    },	
	
-- 블로커 지속 시간 증가
	{		
		ID 			=	SID_DIO2_BLOCKER_TIMEUP_LV1,
		GroupID 	=	SG_DIO2_BLOCKER_TIMEUP,
		PreList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV5, },
		NextList 	=	{SID_DIO2_BLOCKER_TIMEUP_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1236,
		DescID 		=	1294,
		ControlDesc	=	1169,
		
		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	12,

		LockGroup = -1,				
		Passive = TRUE,
    },
	
    {		
		ID 			=	SID_DIO2_BLOCKER_TIMEUP_LV2,
		GroupID 	=	SG_DIO2_BLOCKER_TIMEUP,
		PreList 	=	{ SID_DIO2_BLOCKER_TIMEUP_LV1, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1237,
		DescID 		=	1295,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	12,
		Overlapped  = TRUE,
		
		LockGroup = -1,				
		Passive = TRUE,
    },
	
    {		
		ID 			=	SID_DIO2_BLOCKER_TIMEUP_LV3,
		GroupID 	=	SG_DIO2_BLOCKER_TIMEUP,
		PreList 	=	{SID_DIO2_BLOCKER_TIMEUP_LV2, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1238,
		DescID 		=	1296,
		ControlDesc	=	1169,

		UI_Pos_x 	=	393,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	12,
		Overlapped  = TRUE,

		LockGroup = -1,				
		Passive = TRUE,
    },	
	
-- 파어웨이
	{		
		ID 			=	SID_DIO2_FARAWAW_NORMAL_LV1,
		GroupID 	=	SG_DIO2_FARAWAY_NORMAL,
		PreList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV5, },
		NextList 	=	{SID_DIO2_FARAWAW_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1239,
		DescID 		=	1297,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	2,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_FARAWAY,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_FARAWAW_NORMAL_LV2,
		GroupID 	=	SG_DIO2_FARAWAY_NORMAL,
		PreList 	=	{SID_DIO2_FARAWAW_NORMAL_LV1, },
		NextList 	=	{SID_DIO2_FARAWAW_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1240,
		DescID 		=	1298,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_FARAWAY,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_FARAWAW_NORMAL_LV3,
		GroupID 	=	SG_DIO2_FARAWAY_NORMAL,
		PreList 	=	{SID_DIO2_FARAWAW_NORMAL_LV2, },
		NextList 	=	{SID_DIO2_FARAWAW_NORMAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1241,
		DescID 		=	1299,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_FARAWAY,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_FARAWAW_NORMAL_LV4,
		GroupID 	=	SG_DIO2_FARAWAY_NORMAL,
		PreList 	=	{SID_DIO2_FARAWAW_NORMAL_LV3, },
		NextList 	=	{SID_DIO2_FARAWAW_NORMAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1359,
		DescID 		=	1361,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_FARAWAY,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
    {		
		ID 			=	SID_DIO2_FARAWAW_NORMAL_LV5,
		GroupID 	=	SG_DIO2_FARAWAY_NORMAL,
		PreList 	=	{SID_DIO2_FARAWAW_NORMAL_LV4, },
		NextList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1360,
		DescID 		=	1362,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	338,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO2_FARAWAY,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	

-- 카오틱 스파크
	{		
		ID 			=	SID_DIO2_CHAOTICSPARK_SPECIAL_LV1,
		GroupID 	=	SG_DIO2_CHAOTICSPARK_SPECIAL,
		PreList 	=	{SID_DIO2_FARAWAW_NORMAL_LV5, SID_DIO2_ONRUSH_APDOWN_LV1, },
		NextList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1242,
		DescID 		=	1300,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	{ 2, 12,	},

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_CHAOTICSPARK,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHAOTICSPARK_SPECIAL_LV2,
		GroupID 	=	SG_DIO2_CHAOTICSPARK_SPECIAL,
		PreList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV1, },
		NextList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1243,
		DescID 		=	1301,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_CHAOTICSPARK,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHAOTICSPARK_SPECIAL_LV3,
		GroupID 	=	SG_DIO2_CHAOTICSPARK_SPECIAL,
		PreList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV2, },
		NextList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1244,
		DescID 		=	1302,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_CHAOTICSPARK,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

    {		
		ID 			=	SID_DIO2_CHAOTICSPARK_SPECIAL_LV4,
		GroupID 	=	SG_DIO2_CHAOTICSPARK_SPECIAL,
		PreList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV3, },
		NextList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1363,
		DescID 		=	1365,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_CHAOTICSPARK,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
    {		
		ID 			=	SID_DIO2_CHAOTICSPARK_SPECIAL_LV5,
		GroupID 	=	SG_DIO2_CHAOTICSPARK_SPECIAL,
		PreList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV4, },
		NextList 	=	{SID_DIO3_TIME_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1364,
		DescID 		=	1366,
		ControlDesc	=	1169,

		UI_Pos_x 	=	470,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	{ 2, 12,	},
		Overlapped  = TRUE,
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO2_CHAOTICSPARK,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
-- 헌팅 쇼크 데미지 증가
    {		
		ID 			=	SID_DIO2_HAUNTINGSHOCK_APDOWN_LV1,
		GroupID 	=	SG_DIO2_HAUNTINGSHOCK_APDOWN,
		PreList 	=	{SID_DIO1_HAUNTINGSHOCK_LV1, },
		NextList 	=	{SID_DIO2_BLOCKER_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1245,
		DescID 		=	1303,
		ControlDesc	=	1169,

		UI_Pos_x 	=	547,--dio2
		UI_Pos_y 	=	269,--dio2
		UI_LineType =	-1,

		LockGroup = -1,				
		
		Passive = TRUE,
    },	
	
-- 온 러쉬 AP 감소	
    {		
		ID 			=	SID_DIO2_ONRUSH_APDOWN_LV1,
		GroupID 	=	SG_DIO2_ONRUSH_APDOWN,
		PreList 	=	{SID_DIO1_ONRUSH_SPECIAL_LV2, },
		NextList 	=	{SID_DIO2_CHAOTICSPARK_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1246,
		DescID 		=	1304,
		ControlDesc	=	1169,

		UI_Pos_x 	=	547,--dio2
		UI_Pos_y 	=	407,--dio2
		UI_LineType =	-1,

		LockGroup = -1,				
		Passive = TRUE,
    },
	
-- 콤보 어택
    {		
		ID 			=	SID_DIO2_COMBO_ATK_LV1,
		GroupID 	=	SG_DIO2_COMBO_ATK,
		PreList 	=	{},
		NextList 	=	{SID_DIO2_COMBO_ATK_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	TRUE,
		LearnSP 	=	1,

		
		TitleID 	=	1247,
		DescID 		=	1305,
		ControlDesc	=	220,

		UI_Pos_x 	=	100,--dio2
		UI_Pos_y 	=	330,--dio2
		UI_LineType =	2,

		LockGroup = -1,				
    },
	
    {		
		ID 			=	SID_DIO2_COMBO_ATK_LV2,
		GroupID 	=	SG_DIO2_COMBO_ATK,
		PreList 	=	{SID_DIO2_COMBO_ATK_LV1, },
		NextList 	=	{SID_DIO3_COMBO_ATK, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1248,
		DescID 		=	1306,
		ControlDesc	=	221,

		UI_Pos_x 	=	105,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,				
    },	
	
-- 콤보 응용	
	{		
		ID 			=	SID_DIO2_COMBO_PLUS_LV1,
		GroupID 	=	SG_DIO2_COMBO_PLUS,
		PreList 	=	{},
		NextList 	=	{SID_DIO2_COMBO_PLUS_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1249,
		DescID 		=	1307,
		ControlDesc	=	134,

		UI_Pos_x 	=	195,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_COMBO_PLUS_LV2,
		GroupID 	=	SG_DIO2_COMBO_PLUS,
		PreList 	=	{SID_DIO2_COMBO_PLUS_LV1, },
		NextList 	=	{SID_DIO2_COMBO_PLUS_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1250,
		DescID 		=	1308,
		ControlDesc	=	134,

		UI_Pos_x 	=	195,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_COMBO_PLUS_LV3,
		GroupID 	=	SG_DIO2_COMBO_PLUS,
		PreList 	=	{SID_DIO2_COMBO_PLUS_LV2, },
		NextList 	=	{SID_DIO3_CRI_DOUBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1251,
		DescID 		=	1309,
		ControlDesc	=	134,

		UI_Pos_x 	=	195,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,
		LockGroup = -1,
    },	
	
-- 점프 어택
    {		
		ID 			=	SID_DIO2_JUMP_ATK_LV1,
		GroupID 	=	SG_DIO2_JUMP_ATK,
		PreList 	=	{},
		NextList 	=	{SID_DIO2_JUMP_ATK_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		
		TitleID 	=	1252,
		DescID 		=	1310,
		ControlDesc	=	131,

		UI_Pos_x 	=	285,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_JUMP_ATK_LV2,
		GroupID 	=	SG_DIO2_JUMP_ATK,
		PreList 	=	{SID_DIO2_JUMP_ATK_LV1, },
		NextList 	=	{SID_DIO3_DASH_ATK, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1253,
		DescID 		=	1311,
		ControlDesc	=	143,

		UI_Pos_x 	=	285,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },	
	
-- 점프 응용	
	{		
		ID 			=	SID_DIO2_JUMP_PLUS_LV1,
		GroupID 	=	SG_DIO2_JUMP_PLUS,
		PreList 	=	{},
		NextList 	=	{SID_DIO2_JUMP_PLUS_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1254,
		DescID 		=	1312,
		ControlDesc	=	1323,

		UI_Pos_x 	=	375,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_JUMP_PLUS_LV2,
		GroupID 	=	SG_DIO2_JUMP_PLUS,
		PreList 	=	{SID_DIO2_JUMP_PLUS_LV1, },
		NextList 	=	{SID_DIO2_JUMP_PLUS_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1255,
		DescID 		=	1313,
		ControlDesc	=	1323,

		UI_Pos_x 	=	375,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,
		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_JUMP_PLUS_LV3,
		GroupID 	=	SG_DIO2_JUMP_PLUS,
		PreList 	=	{SID_DIO2_JUMP_PLUS_LV2, },
		NextList 	=	{SID_DIO3_DASH_ATK, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1256,
		DescID 		=	1314,
		ControlDesc	=	1323,

		UI_Pos_x 	=	375,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,
		LockGroup = -1,
    },		
	
-- AP 회복 속도 증가
	{		
		ID 			=	SID_DIO2_AP_SPEED_LV3,
		GroupID 	=	SG_DIO2_AP,
		PreList 	=	{SID_DIO1_AP_SPEED_LV2,},
		NextList 	=	{SID_DIO2_AP_SPEED_LV4,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1257,
		DescID 		=	1315,
		ControlDesc	=	1169,

		UI_Pos_x 	=	465,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_AP_SPEED_LV4,
		GroupID 	=	SG_DIO2_AP,
		PreList 	=	{SID_DIO2_AP_SPEED_LV3, },
		NextList 	=	{SID_DIO2_AP_SPEED_LV5,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1258,
		DescID 		=	1316,
		ControlDesc	=	1169,
		Overlapped  = TRUE,
		UI_Pos_x 	=	465,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_AP_SPEED_LV5,
		GroupID 	=	SG_DIO2_AP,
		PreList 	=	{SID_DIO2_AP_SPEED_LV4, },
		NextList 	=	{SID_DIO2_AP_START_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		Overlapped  = TRUE,
		
		TitleID 	=	1259,
		DescID 		=	1317,
		ControlDesc	=	1169,

		UI_Pos_x 	=	465,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
-- 시작 시 AP 보유
    {		
		ID 			=	SID_DIO2_AP_START_LV4,
		GroupID 	=	SG_DIO2_AP,
		PreList 	=	{SID_DIO2_AP_SPEED_LV5, SID_DIO1_AP_START_LV3},
		NextList 	=	{SID_DIO2_AP_START_LV5,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1260,
		DescID 		=	1318,
		ControlDesc	=	1169,

		UI_Pos_x 	=	465,--dio2
		UI_Pos_y 	=	388,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_AP_START_LV5,
		GroupID 	=	SG_DIO2_AP,
		PreList 	=	{SID_DIO2_AP_START_LV4, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1261,
		DescID 		=	1319,
		ControlDesc	=	1169,
		Overlapped  = TRUE,
		UI_Pos_x 	=	465,--dio2
		UI_Pos_y 	=	388,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
-- 크리티컬 2
	{		
		ID 			=	SID_DIO2_CRITICAL_UP_LV3,
		GroupID 	=	SG_DIO2_CRITICAL_UP_2,
		PreList 	=	{SID_DIO1_CRITICAL_UP_LV2, },
		NextList 	=	{SID_DIO2_CRITICAL_UP_LV4,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1262,
		DescID 		=	1320,
		ControlDesc	=	1169,

		UI_Pos_x 	=	555,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_CRITICAL_UP_LV4,
		GroupID 	=	SG_DIO2_CRITICAL_UP_2,
		PreList 	=	{SID_DIO2_CRITICAL_UP_LV3, },
		NextList 	=	{SID_DIO2_CRITICAL_UP_LV5,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1263,
		DescID 		=	1321,
		ControlDesc	=	1169,

		UI_Pos_x 	=	555,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,
		LockGroup = -1,
    },
	
    {		
		ID 			=	SID_DIO2_CRITICAL_UP_LV5,
		GroupID 	=	SG_DIO2_CRITICAL_UP_2,
		PreList 	=	{SID_DIO2_CRITICAL_UP_LV4, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1264,
		DescID 		=	1322,
		ControlDesc	=	1169,

		UI_Pos_x 	=	555,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,
		Overlapped  = TRUE,
		LockGroup = -1,
    },
    
    --리바운드 거리 조정
	{		
		ID 			=	SID_DIO2_REBOUND_LENGTH_LV1,
		GroupID 	=	SG_DIO2_REBOUND_LENGTH,
		PreList 	=	{SID_DIO2_REBOUND_NORMAL_LV3, },
		NextList 	=	{SID_DIO2_REBOUND_LENGTH_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1212,
		DescID 		=	1327,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,

		LockGroup = -1,
		
		Passive = TRUE,
    },
	
	{		
		ID 			=	SID_DIO2_REBOUND_LENGTH_LV2,
		GroupID 	=	SG_DIO2_REBOUND_LENGTH,
		PreList 	=	{SID_DIO2_REBOUND_LENGTH_LV1, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1328,
		DescID 		=	1327,
		ControlDesc	=	1169,

		UI_Pos_x 	=	124,--dio2
		UI_Pos_y 	=	200,--dio2
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },
}