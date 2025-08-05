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
	-- 데스 사이저
    {		
		ID 			=	SID_DIO1_DEATHSCYTHE_SPECIAL_LV1,
		GroupID 	=	SG_DIO1_DEATHSCYTHE_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_DEATHSCYTHE_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup156.dds",
		TitleID 	=	1066,		-- 스킬 이름
		DescID 		=	1116,		-- 스킬 설명
		ControlDesc	=	127, 		-- 사용 안함 (수치는 필요)
                           
		UI_Pos_x 	=	120,
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO1_NAT_COMBO_SPECIAL,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
    {		
		ID 			=	SID_DIO1_DEATHSCYTHE_SPECIAL_LV2,
		GroupID 	=	SG_DIO1_DEATHSCYTHE_SPECIAL,
		PreList 	=	{SID_DIO1_DEATHSCYTHE_SPECIAL_LV1,},
		NextList 	=	{SID_DIO1_DEATHSCYTHE_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup156_lv2.dds",
		TitleID 	=	1067,
		DescID 		=	1117,
		ControlDesc	=	127,

		UI_Pos_x 	=	120,
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO1_NAT_COMBO_SPECIAL,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	
    {		
		ID 			=	SID_DIO1_DEATHSCYTHE_SPECIAL_LV3,
		GroupID 	=	SG_DIO1_DEATHSCYTHE_SPECIAL,
		PreList 	=	{SID_DIO1_DEATHSCYTHE_SPECIAL_LV2,},
		NextList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup156_lv3.dds",
		TitleID 	=	1068,
		DescID 		=	1118,
		ControlDesc	=	127,

		UI_Pos_x 	=	120,
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO1_NAT_COMBO_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },		

-- 데스 스타	

    {		
		ID 			=	SID_DIO1_DEATHSTAR_SPECIAL_LV1,
		GroupID 	=	SG_DIO1_DEATHSTAR_SPECIAL,
		PreList 	=	{SID_DIO1_DEATHSCYTHE_SPECIAL_LV3, SID_DIO1_UPPERCUT_NORMAL_LV3,},
		NextList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup157.dds",
		TitleID 	=	1069,
		DescID 		=	1119,
		ControlDesc	=	127,

		UI_Pos_x 	=	120,
		UI_Pos_y 	=	290,--dio
		UI_LineType =	11,

		LockGroup = -1,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_NAT_THROW,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
    {		
		ID 			=	SID_DIO1_DEATHSTAR_SPECIAL_LV2,
		GroupID 	=	SG_DIO1_DEATHSTAR_SPECIAL,
		PreList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV1,},
		NextList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup157_lv2.dds",
		TitleID 	=	1070,
		DescID 		=	1120,
		ControlDesc	=	127,

		UI_Pos_x 	=	120,
		UI_Pos_y 	=	290,--dio
		UI_LineType =	11,
		
		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_NAT_THROW,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
    {		
		ID 			=	SID_DIO1_DEATHSTAR_SPECIAL_LV3,
		GroupID 	=	SG_DIO1_DEATHSTAR_SPECIAL,
		PreList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV2,},
		NextList 	=	{SID_DIO1_EATOFF_SPECIAL_LV1, SID_DIO3_TIME_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup157_lv3.dds",
		TitleID 	=	1071,
		DescID 		=	1121,
		ControlDesc	=	127,

		UI_Pos_x 	=	120,
		UI_Pos_y 	=	290,--dio
		UI_LineType =	11,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_NAT_THROW,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		

-- 데스 스타 어퍼컷	
	
    {		
		ID 			=	SID_DIO1_UPPERCUT_NORMAL_LV1,
		GroupID 	=	SG_DIO1_UPPERCUT_NORMAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_UPPERCUT_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup158.dds",
		TitleID 	=	1072,
		DescID 		=	1122,
		ControlDesc	=	127,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_NAT_UPPER,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
		
    },		

    {		
		ID 			=	SID_DIO1_UPPERCUT_NORMAL_LV2,
		GroupID 	=	SG_DIO1_UPPERCUT_NORMAL,
		PreList 	=	{SID_DIO1_UPPERCUT_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_UPPERCUT_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup158_lv2.dds",
		TitleID 	=	1073,
		DescID 		=	1123,
		ControlDesc	=	127,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,
		
		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_NAT_UPPER,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

    {		
		ID 			=	SID_DIO1_UPPERCUT_NORMAL_LV3,
		GroupID 	=	SG_DIO1_UPPERCUT_NORMAL,
		PreList 	=	{SID_DIO1_UPPERCUT_NORMAL_LV2,},
		NextList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup158_lv3.dds",
		TitleID 	=	1074,
		DescID 		=	1124,
		ControlDesc	=	127,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_NAT_UPPER,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

-- 드로 핸드	
	
    {		
		ID 			=	SID_DIO1_DRAWHAND_NORMAL_LV1,
		GroupID 	=	SG_DIO1_DRAWHAND_NORMAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_DRAWHAND_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup162.dds",
		TitleID 	=	1075,
		DescID 		=	1125,
		ControlDesc	=	127,

		UI_Pos_x 	=	270,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-3,

		LockGroup = -1,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_ARM_SCRATCH,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
		
    },		

    {		
		ID 			=	SID_DIO1_DRAWHAND_NORMAL_LV2,
		GroupID 	=	SG_DIO1_DRAWHAND_NORMAL,
		PreList 	=	{SID_DIO1_DRAWHAND_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_DRAWHAND_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup162_lv2.dds",
		TitleID 	=	1076,
		DescID 		=	1126,
		ControlDesc	=	127,

		UI_Pos_x 	=	270,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-3,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_ARM_SCRATCH,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

    {		
		ID 			=	SID_DIO1_DRAWHAND_NORMAL_LV3,
		GroupID 	=	SG_DIO1_DRAWHAND_NORMAL,
		PreList 	=	{SID_DIO1_DRAWHAND_NORMAL_LV2,},
		NextList 	=	{SID_DIO1_BIGHIT_NORMAL_LV1, SID_DIO1_DRAWHAND_SIZE_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup162_lv3.dds",
		TitleID 	=	1077,
		DescID 		=	1127,
		ControlDesc	=	127,

		UI_Pos_x 	=	270,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-3,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_ARM_SCRATCH,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

-- 빅 히트	
	
    {		
		ID 			=	SID_DIO1_BIGHIT_NORMAL_LV1,
		GroupID 	=	SG_DIO1_BIGHIT_NORMAL,
		PreList 	=	{SID_DIO1_DRAWHAND_NORMAL_LV3,},
		NextList 	=	{SID_DIO1_RAKEHAND_SPECIAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup160.dds",
		TitleID 	=	1078,
		DescID 		=	1128,
		ControlDesc	=	127,

		UI_Pos_x 	=	270,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	2,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_ARM_EARTHQUAKE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

-- 드로 핸드 거리 증가	
	
    {		
		ID 			=	SID_DIO1_DRAWHAND_SIZE_NORMAL_LV1,
		GroupID 	=	SG_DIO1_DRAWHAND_SIZE_NORMAL,
		PreList 	=	{SID_DIO1_DRAWHAND_NORMAL_LV3,},
		NextList 	=	{SID_DIO1_RAKEHAND_SPECIAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup161.dds",
		TitleID 	=	1079,
		DescID 		=	1129,
		ControlDesc	=	127,

		UI_Pos_x 	=	345,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	13,

		LockGroup = -1,		
		Passive = TRUE,
    },		

-- 레이크 핸드	
	
    {		
		ID 			=	SID_DIO1_RAKEHAND_SPECIAL_LV1,
		GroupID 	=	SG_DIO1_RAKEHAND_SPECIAL,
		PreList 	=	{SID_DIO1_BIGHIT_NORMAL_LV1, SID_DIO1_DRAWHAND_SIZE_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_RAKEHAND_SPECIAL_LV2},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup159.dds",
		TitleID 	=	1080,
		DescID 		=	1130,
		ControlDesc	=	127,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	360,--dio
		UI_LineType =	14,


		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_ARM_UPPER,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		
    },		

    {		
		ID 			=	SID_DIO1_RAKEHAND_SPECIAL_LV2,
		GroupID 	=	SG_DIO1_RAKEHAND_SPECIAL,
		PreList 	=	{SID_DIO1_RAKEHAND_SPECIAL_LV1,},
		NextList 	=	{SID_DIO1_RAKEHAND_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup159_lv2.dds",
		TitleID 	=	1081,
		DescID 		=	1131,
		ControlDesc	=	127,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	360,--dio
		UI_LineType =	14,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_ARM_UPPER,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		

    {		
		ID 			=	SID_DIO1_RAKEHAND_SPECIAL_LV3,
		GroupID 	=	SG_DIO1_RAKEHAND_SPECIAL,
		PreList 	=	{SID_DIO1_RAKEHAND_SPECIAL_LV2,},
		NextList 	=	{SID_DIO1_EATOFF_SPECIAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup159_lv3.dds",
		TitleID 	=	1082,
		DescID 		=	1132,
		ControlDesc	=	127,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	360,--dio
		UI_LineType =	14,
		
		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_ARM_UPPER,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		

-- 잇 오프	
	
    {		
		ID 			=	SID_DIO1_EATOFF_SPECIAL_LV1,
		GroupID 	=	SG_DIO1_EATOFF_SPECIAL,
		PreList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV3, SID_DIO1_RAKEHAND_SPECIAL_LV3,},
		NextList 	=	{SID_DIO1_EATOFF_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup163.dds",
		TitleID 	=	1083,
		DescID 		=	1133,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	120,
		UI_Pos_y 	=	360,--dio
		UI_LineType =	15,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO1_SUMMON_MOUTH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

    {		
		ID 			=	SID_DIO1_EATOFF_SPECIAL_LV2,
		GroupID 	=	SG_DIO1_EATOFF_SPECIAL,
		PreList 	=	{SID_DIO1_EATOFF_SPECIAL_LV1,},
		NextList 	=	{SID_DIO1_EATOFF_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup163_lv2.dds",
		TitleID 	=	1084,
		DescID 		=	1134,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	120,
		UI_Pos_y 	=	360,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO1_SUMMON_MOUTH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

    {		
		ID 			=	SID_DIO1_EATOFF_SPECIAL_LV3,
		GroupID 	=	SG_DIO1_EATOFF_SPECIAL,
		PreList 	=	{SID_DIO1_EATOFF_SPECIAL_LV2,},
		NextList 	=	{SID_DIO1_BLACKSPACE_LV1,SID_DIO2_EATOFF_APDOWN_NORMAL_LV1},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup163_lv3.dds",
		TitleID 	=	1085,
		DescID 		=	1135,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	120,
		UI_Pos_y 	=	360,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO1_SUMMON_MOUTH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

-- 리벤지 스워드	
	
    {		
		ID 			=	SID_DIO1_REVENGESWORD_SPECIAL_LV1,
		GroupID 	=	SG_DIO1_REVENGESWORD_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_REVENGESWORD_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup164.dds",
		TitleID 	=	1086,
		DescID 		=	1136,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-3,

		LockGroup = -1,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_SWORD_UP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		

    {		
		ID 			=	SID_DIO1_REVENGESWORD_SPECIAL_LV2,
		GroupID 	=	SG_DIO1_REVENGESWORD_SPECIAL,
		PreList 	=	{SID_DIO1_REVENGESWORD_SPECIAL_LV1,},
		NextList 	=	{SID_DIO1_REVENGESWORD_SPECIAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup164_lv2.dds",
		TitleID 	=	1087,
		DescID 		=	1137,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-3,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_SWORD_UP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		

    {		
		ID 			=	SID_DIO1_REVENGESWORD_SPECIAL_LV3,
		GroupID 	=	SG_DIO1_REVENGESWORD_SPECIAL,
		PreList 	=	{SID_DIO1_REVENGESWORD_SPECIAL_LV2,},
		NextList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup164_lv3.dds",
		TitleID 	=	1088,
		DescID 		=	1138,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-3,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_SWORD_UP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		

-- 스페이스 테어	
	
    {		
		ID 			=	SID_DIO1_SPACETEAR_NORMAL_LV1,
		GroupID 	=	SG_DIO1_SPACETEAR_NORMAL,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_SPACETEAR_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup165.dds",
		TitleID 	=	1089,
		DescID 		=	1139,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_SWORD_WARP,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
		
    },		

    {		
		ID 			=	SID_DIO1_SPACETEAR_NORMAL_LV2,
		GroupID 	=	SG_DIO1_SPACETEAR_NORMAL,
		PreList 	=	{SID_DIO1_SPACETEAR_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_SPACETEAR_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup165_lv2.dds",
		TitleID 	=	1090,
		DescID 		=	1140,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_SWORD_WARP,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

    {		
		ID 			=	SID_DIO1_SPACETEAR_NORMAL_LV3,
		GroupID 	=	SG_DIO1_SPACETEAR_NORMAL,
		PreList 	=	{SID_DIO1_SPACETEAR_NORMAL_LV2,},
		NextList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup165_lv3.dds",
		TitleID 	=	1091,
		DescID 		=	1141,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	220,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_SWORD_WARP,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

-- 플래쉬 스워드	
	
    {		
		ID 			=	SID_DIO1_FLASHSWORD_NORMAL_LV1,
		GroupID 	=	SG_DIO1_FLASHSWORD_NORMAL,
		PreList 	=	{SID_DIO1_REVENGESWORD_SPECIAL_LV3, SID_DIO1_SPACETEAR_NORMAL_LV3,},
		NextList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup166.dds",
		TitleID 	=	1092,
		DescID 		=	1142,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	2,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_SWORD_STRAIGHT,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

    {		
		ID 			=	SID_DIO1_FLASHSWORD_NORMAL_LV2,
		GroupID 	=	SG_DIO1_FLASHSWORD_NORMAL,
		PreList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV3, SID_DIO1_FLASHSWORD_HIT_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup166_lv2.dds",
		TitleID 	=	1093,
		DescID 		=	1143,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_SWORD_STRAIGHT,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

    {		
		ID 			=	SID_DIO1_FLASHSWORD_NORMAL_LV3,
		GroupID 	=	SG_DIO1_FLASHSWORD_NORMAL,
		PreList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV2,},
		NextList 	=	{SID_DIO1_BULL_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup166_lv3.dds",
		TitleID 	=	1094,
		DescID 		=	1144,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_SWORD_STRAIGHT,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

-- 플래쉬 스워드 타격 증가	
	
    {		
		ID 			=	SID_DIO1_FLASHSWORD_HIT_NORMAL_LV1,
		GroupID 	=	SG_DIO1_FLASHSWORD_HIT_NORMAL,
		PreList 	=	{SID_DIO1_FLASHSWORD_NORMAL_LV2,},
		NextList 	=	{SID_DIO1_FLASHSWORD_HIT_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup167.dds",
		TitleID 	=	1095,
		DescID 		=	1145,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Passive = TRUE,
    },		

    {		
		ID 			=	SID_DIO1_FLASHSWORD_HIT_NORMAL_LV2,
		GroupID 	=	SG_DIO1_FLASHSWORD_HIT_NORMAL,
		PreList 	=	{SID_DIO1_FLASHSWORD_HIT_NORMAL_LV1,},
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup167_lv2.dds",
		TitleID 	=	1096,
		DescID 		=	1146,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },		

-- 블링크	
	
    {		
		ID 			=	SID_DIO1_BLINK_NORMAL_LV1,
		GroupID 	=	SG_DIO1_BLINK_NORMAL,
		PreList 	=	{SID_DIO1_MOVE_LV1,},
		NextList 	=	{SID_DIO1_BLINK_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup168.dds",
		TitleID 	=	1097,
		DescID 		=	1147,
		ControlDesc	=	127,


		UI_Pos_x 	=	570,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_BLINK,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

    {		
		ID 			=	SID_DIO1_BLINK_NORMAL_LV2,
		GroupID 	=	SG_DIO1_BLINK_NORMAL,
		PreList 	=	{SID_DIO1_BLINK_NORMAL_LV1, },
		NextList 	=	{SID_DIO1_BULL_NORMAL_LV1, SID_DIO2_VERTICALBLINK_NORMAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup168_lv2.dds",
		TitleID 	=	1098,
		DescID 		=	1148,
		ControlDesc	=	127,

		UI_Pos_x 	=	570,--dio
		UI_Pos_y 	=	290,--dio
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO1_BLINK,			
			CoolTime = 3,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 3,
		},
    },		

-- 불	
	
    {			
		ID 			=	SID_DIO1_BULL_NORMAL_LV1,
		GroupID 	=	SG_DIO1_BULL_NORMAL,
		PreList 	=	{SID_DIO1_BLINK_NORMAL_LV2, SID_DIO1_FLASHSWORD_NORMAL_LV3,},
		NextList 	=	{SID_DIO1_BULL_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup169.dds",
		TitleID 	=	1099,
		DescID 		=	1149,
		ControlDesc	=	127,
		

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	360,--dio
		UI_LineType =	16,

		LockGroup = -1,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO1_LANCE_STRAIGHT,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },		

    {		
		ID 			=	SID_DIO1_BULL_NORMAL_LV2,
		GroupID 	=	SG_DIO1_BULL_NORMAL,
		PreList 	=	{SID_DIO1_BULL_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_BULL_NORMAL_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup169_lv2.dds",
		TitleID 	=	1100,
		DescID 		=	1150,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	360,--dio
		UI_LineType =	16,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO1_LANCE_STRAIGHT,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },		

    {		
		ID 			=	SID_DIO1_BULL_NORMAL_LV3,
		GroupID 	=	SG_DIO1_BULL_NORMAL,
		PreList 	=	{SID_DIO1_BULL_NORMAL_LV2,},
		NextList 	=	{SID_DIO1_HAUNTINGSHOCK_LV1, SID_DIO1_BULL_HIT_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup169_lv3.dds",
		TitleID 	=	1101,
		DescID 		=	1151,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	360,--dio
		UI_LineType =	16,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO1_LANCE_STRAIGHT,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },		

-- 불 타격 증가	
	
    {		
		ID 			=	SID_DIO1_BULL_HIT_NORMAL_LV1,
		GroupID 	=	SG_DIO1_BULL_HIT_NORMAL,
		PreList 	=	{SID_DIO1_BULL_NORMAL_LV3,},
		NextList 	=	{SID_DIO1_BULL_HIT_NORMAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup170.dds",
		TitleID 	=	1102,
		DescID 		=	1152,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	360,--dio
		UI_LineType =	12,


		LockGroup = -1,
		Passive = TRUE,
    },		

    {		
		ID 			=	SID_DIO1_BULL_HIT_NORMAL_LV2,
		GroupID 	=	SG_DIO1_BULL_HIT_NORMAL,
		PreList 	=	{SID_DIO1_BULL_HIT_NORMAL_LV1,},
		NextList 	=	{SID_DIO1_HAUNTINGSHOCK_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup170_lv2.dds",
		TitleID 	=	1103,
		DescID 		=	1153,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	360,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },		

-- 온러쉬	
	
    {		
		ID 			=	SID_DIO1_ONRUSH_SPECIAL_LV1,
		GroupID 	=	SG_DIO1_ONRUSH_SPECIAL,
		PreList 	=	{SID_DIO1_HAUNTINGSHOCK_LV1,},
		NextList 	=	{SID_DIO1_ONRUSH_SPECIAL_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup171.dds",
		TitleID 	=	1104,
		DescID 		=	1154,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	430,--dio
		UI_LineType =	12,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO1_LANCE_CHARGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

    {		
		ID 			=	SID_DIO1_ONRUSH_SPECIAL_LV2,
		GroupID 	=	SG_DIO1_ONRUSH_SPECIAL,
		PreList 	=	{SID_DIO1_ONRUSH_SPECIAL_LV1,},
		NextList 	=	{SID_DIO2_ONRUSH_APDOWN_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup171_lv2.dds",
		TitleID 	=	1105,
		DescID 		=	1155,
		ControlDesc	=	127,

		UI_Pos_x 	=	420,--dio
		UI_Pos_y 	=	430,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO1_LANCE_CHARGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

-- AP 회복 속도 증가	
	
    {		
		ID 			=	SID_DIO1_AP_SPEED_LV1,
		GroupID 	=	SG_DIO1_AP,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_AP_SPEED_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup172_2.dds",
		TitleID 	=	1106,
		DescID 		=	1156,
		ControlDesc	=	1169,

		UI_Pos_x 	=	205,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
    },		

    {		
		ID 			=	SID_DIO1_AP_SPEED_LV2,
		GroupID 	=	SG_DIO1_AP,
		PreList 	=	{SID_DIO1_AP_SPEED_LV1,},
		NextList 	=	{SID_DIO1_AP_START_LV1, SID_DIO2_AP_SPEED_LV3, SID_DIO3_POWER_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup172_2_lv2.dds",
		TitleID 	=	1107,
		DescID 		=	1157,
		ControlDesc	=	1169,

		UI_Pos_x 	=	205,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },		

-- 시작 시 AP 보유	
	
    {		
		ID 			=	SID_DIO1_AP_START_LV1,
		GroupID 	=	SG_DIO1_AP,
		PreList 	=	{SID_DIO1_AP_SPEED_LV2,},
		NextList 	=	{SID_DIO1_AP_START_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup172.dds",
		TitleID 	=	1108,
		DescID 		=	1158,
		ControlDesc	=	1169,

		UI_Pos_x 	=	205,
		UI_Pos_y 	=	388,
		UI_LineType =	2,

		LockGroup = -1,
    },		

    {		
		ID 			=	SID_DIO1_AP_START_LV2,
		GroupID 	=	SG_DIO1_AP,
		PreList 	=	{SID_DIO1_AP_START_LV1,},
		NextList 	=	{SID_DIO1_AP_START_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup172_lv2.dds",
		TitleID 	=	1109,
		DescID 		=	1159,
		ControlDesc	=	1169,

		UI_Pos_x 	=	205,
		UI_Pos_y 	=	388,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },		

    {		
		ID 			=	SID_DIO1_AP_START_LV3,
		GroupID 	=	SG_DIO1_AP,
		PreList 	=	{SID_DIO1_AP_START_LV2,},
		NextList 	=	{SID_DIO2_AP_START_LV4, SID_DIO3_POWER_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup172_lv3.dds",
		TitleID 	=	1110,
		DescID 		=	1160,
		ControlDesc	=	1169,

		UI_Pos_x 	=	205,
		UI_Pos_y 	=	388,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },		

-- 이동 속도 증가	
	
    {		
		ID 			=	SID_DIO1_MOVE_LV1,
		GroupID 	=	SG_DIO1_SPEED,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_BLINK_NORMAL_LV1, SID_DIO2_VERTICALBLINK_NORMAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup173.dds",
		TitleID 	=	1111,
		DescID 		=	1161,
		ControlDesc	=	1169,

		UI_Pos_x 	=	305,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
    },		

-- 카운터 쿨 타임 감소	
	
    {		
		ID 			=	SID_DIO1_COUNT_COOLTIME_LV1,
		GroupID 	=	SG_DIO1_COUNT_COOLTIME,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_COUNT_COOLTIME_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup174.dds",
		TitleID 	=	1112,
		DescID 		=	1162,
		ControlDesc	=	1169,

		UI_Pos_x 	=	405,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
    },		

    {		
		ID 			=	SID_DIO1_COUNT_COOLTIME_LV2,
		GroupID 	=	SG_DIO1_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV1,},
		NextList 	=	{SID_DIO3_COUNT_COOLTIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup174_lv2.dds",
		TitleID 	=	1113,
		DescID 		=	1163,
		ControlDesc	=	1169,

		UI_Pos_x 	=	405,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },		

-- 크리티컬 상승	
	
    {		
		ID 			=	SID_DIO1_CRITICAL_UP_LV1,
		GroupID 	=	SG_DIO1_CRITICAL_UP,
		PreList 	=	{},
		NextList 	=	{SID_DIO1_CRITICAL_UP_LV2,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup175.dds",
		TitleID 	=	1114,
		DescID 		=	1164,
		ControlDesc	=	1169,

		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
    },		

    {		
		ID 			=	SID_DIO1_CRITICAL_UP_LV2,
		GroupID 	=	SG_DIO1_CRITICAL_UP,
		PreList 	=	{SID_DIO1_CRITICAL_UP_LV1,},
		NextList 	=	{SID_DIO2_CRITICAL_UP_LV3,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup175_lv2.dds",
		TitleID 	=	1115,
		DescID 		=	1165,
		ControlDesc	=	1169,

		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },	

-- 블랙 스페이스
    {		
		ID 			=	SID_DIO1_BLACKSPACE_LV1,
		GroupID 	=	SG_DIO1_BLACKSPACE_SPECIAL,
		PreList 	=	{SID_DIO1_EATOFF_SPECIAL_LV3, },
		NextList 	=	{SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup182.dds",
		TitleID 	=	1202,
		DescID 		=	1204,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	120,
		UI_Pos_y 	=	430,--dio
		UI_LineType =	2,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_DIO1_BLACK_SPACE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },			

-- 헌팅 쇼크
    {		
		ID 			=	SID_DIO1_HAUNTINGSHOCK_LV1,
		GroupID 	=	SG_DIO1_HAUNTINGSHOCK_SPECIAL,
		PreList 	=	{SID_DIO1_BULL_NORMAL_LV3, SID_DIO1_BULL_HIT_NORMAL_LV2, },
		NextList 	=	{SID_DIO1_ONRUSH_SPECIAL_LV1,SID_DIO2_HAUNTINGSHOCK_APDOWN_LV1},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"skillGroup183.dds",
		TitleID 	=	1203,
		DescID 		=	1205,
		ControlDesc	=	1169,

		UI_Pos_x 	=	495,--dio
		UI_Pos_y 	=	430,--dio
		UI_LineType =	2,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO1_HAUNTINGSHOCK,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },				
    
    
    {		
		ID 			=	SID_DIO1_EMPTY_SLOT,
		GroupID 	=	SG_DIO1_EMPTY_SLOT,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"special75.dds",
		TitleID 	=	986,
		DescID 		=	1039,
		ControlDesc	=	1169,

		UI_Pos_x 	=	0,
		UI_Pos_y 	=	0,
		UI_LineType =	0,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 50000,
			Motion = -1,			
			CoolTime = 0,
		},
    },			
}