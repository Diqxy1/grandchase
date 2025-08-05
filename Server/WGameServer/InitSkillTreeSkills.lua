TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;
X_GAP_LONG = 77;

SkillTreeSkills 	=
{
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
	-- 엘리시스 기사
	{		
		ID 			=	SID_CLASS_ELESIS1_SPECIAL1_CS,
		GroupID 	=	SG_CLASS_ELESIS1_SPECIAL1_CS,
		PreList 	=	{ },
		NextList 	=	{ SID_CLASS_ELESIS1_SPECIAL2_CS, },
		CharLv 		=	1,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage15.dds",
		TitleID 	=	20,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.0,
			Motion = MID_ELESIS_SPECIAL1,			
			CoolTime = 5,
			
			MonsterModeMana = 1.0,
			MonsterModeCoolTime = 5,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS1_SPECIAL2_CS,
		GroupID 	=	SG_CLASS_ELESIS1_SPECIAL2_CS,
		PreList 	=	{ SG_CLASS_ELESIS1_SPECIAL1_CS, },
		NextList 	=	{ SID_CLASS_ELESIS1_SPECIAL3_CS, },
		CharLv 		=	2,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage16.dds",
		TitleID 	=	24,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ELESIS_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS1_SPECIAL3_CS,
		GroupID 	=	SG_CLASS_ELESIS1_SPECIAL3_CS,
		PreList 	=	{ SID_CLASS_ELESIS1_SPECIAL2_CS, },
		NextList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP1, },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage17.dds",
		TitleID 	=	28,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.7,
			Motion = MID_ELESIS_SPECIAL3,			
			CoolTime = 15,
			
			MonsterModeMana = 2.7,
			MonsterModeCoolTime = 15,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	{		
		ID 			=	SID_CHAR_ELESIS_CLASS_SKILL_TEMP1,
		GroupID 	=	SG_CLASS_ELESIS1_UNIQUE_ATK,
		PreList 	=	{ SID_CLASS_ELESIS1_SPECIAL3_CS, },
		NextList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP2, },
		CharLv 		=	4,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon47.dds",
		TitleID 	=	0,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,	


    },
	{		
		ID 			=	SID_CHAR_ELESIS_CLASS_SKILL_TEMP2,
		GroupID 	=	SG_CLASS_ELESIS1_UNIQUE_ATK,
		PreList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP1, },
		NextList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP3, },
		CharLv 		=	5,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon48.dds",
		TitleID 	=	2,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,


    },
	{		
		ID 			=	SID_CHAR_ELESIS_CLASS_SKILL_TEMP3,
		GroupID 	=	SG_CLASS_ELESIS1_UNIQUE_ATK,
		PreList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP2, },
		NextList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP4, },
		CharLv 		=	6,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon49.dds",
		TitleID 	=	4,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.7,
			Motion = MID_ELESIS_SPECIAL3,			
			CoolTime = 15,
			
			MonsterModeMana = 2.7,
			MonsterModeCoolTime = 15,
		},
    },
	{		
		ID 			=	SID_CHAR_ELESIS_CLASS_SKILL_TEMP4,
		GroupID 	=	SG_CLASS_ELESIS1_UNIQUE_ATK,
		PreList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP3, },
		NextList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP5, },
		CharLv 		=	7,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon50.dds",
		TitleID 	=	10,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		

    },
	{		
		ID 			=	SID_CHAR_ELESIS_CLASS_SKILL_TEMP5,
		GroupID 	=	SG_CLASS_ELESIS1_UNIQUE_ATK,
		PreList 	=	{ SID_CHAR_ELESIS_CLASS_SKILL_TEMP4, },
		NextList 	=	{ },
		CharLv 		=	8,
		CharType 	=	0,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon51.dds",
		TitleID 	=	16,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,	

		SlotSkill = 
		{
			Mana = 2.7,
			Motion = MID_ELESIS_SPECIAL3,			
			CoolTime = 15,
			
			MonsterModeMana = 2.7,
			MonsterModeCoolTime = 15,
		},
		
    },
    -- 엘리시스 창병
    {		
		ID 			=	SID_CLASS_ELESIS2_SPECIAL1_CS,
		GroupID 	=	SG_CLASS_ELESIS2_SPECIAL1_CS,
		PreList 	=	{ },
		NextList 	=	{ SID_CLASS_ELESIS2_SPECIAL2_CS, },
		CharLv 		=	1,
		CharType 	=	0,
		Promotion 	=	1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage109.dds",
		TitleID 	=	20,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.0,
			Motion = MID_ELESIS2_SPECIAL1,			
			CoolTime = 5,
			
			MonsterModeMana = 1.0,
			MonsterModeCoolTime = 5,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS2_SPECIAL2_CS,
		GroupID 	=	SG_CLASS_ELESIS2_SPECIAL2_CS,
		PreList 	=	{ SID_CLASS_ELESIS2_SPECIAL1_CS, },
		NextList 	=	{ SID_CLASS_ELESIS2_SPECIAL3_CS, },
		CharLv 		=	2,
		CharType 	=	0,
		Promotion 	=	1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage110.dds",
		TitleID 	=	24,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ELESIS2_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS2_SPECIAL3_CS,
		GroupID 	=	SG_CLASS_ELESIS2_SPECIAL3_CS,
		PreList 	=	{ SID_CLASS_ELESIS2_SPECIAL2_CS, },
		NextList 	=	{ },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage111.dds",
		TitleID 	=	28,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.7,
			Motion = MID_ELESIS2_SPECIAL3,			
			CoolTime = 15,
			
			MonsterModeMana = 2.7,
			MonsterModeCoolTime = 15,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 엘리시스 소드마스터
    {		
		ID 			=	SID_CLASS_ELESIS3_SPECIAL1_CS,
		GroupID 	=	SG_CLASS_ELESIS3_SPECIAL1_CS,
		PreList 	=	{ },
		NextList 	=	{ SID_CLASS_ELESIS3_SPECIAL2_CS, },
		CharLv 		=	1,
		CharType 	=	0,
		Promotion 	=	2,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage300.dds",
		TitleID 	=	20,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.0,
			Motion = MID_ELESIS3_SPECIAL1,			
			CoolTime = 5,
			
			MonsterModeMana = 1.0,
			MonsterModeCoolTime = 5,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS3_SPECIAL2_CS,
		GroupID 	=	SG_CLASS_ELESIS3_SPECIAL2_CS,
		PreList 	=	{ SID_CLASS_ELESIS3_SPECIAL1_CS, },
		NextList 	=	{ SID_CLASS_ELESIS3_SPECIAL3_CS, },
		CharLv 		=	2,
		CharType 	=	0,
		Promotion 	=	2,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage301.dds",
		TitleID 	=	24,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ELESIS3_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS3_SPECIAL3_CS,
		GroupID 	=	SG_CLASS_ELESIS3_SPECIAL3_CS,
		PreList 	=	{ SID_CLASS_ELESIS3_SPECIAL2_CS, },
		NextList 	=	{ },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	2,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage302.dds",
		TitleID 	=	28,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.7,
			Motion = MID_ELESIS3_SPECIAL3,			
			CoolTime = 15,
			
			MonsterModeMana = 2.7,
			MonsterModeCoolTime = 15,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 엘리시스 세이버
    {		
		ID 			=	SID_CLASS_ELESIS4_SPECIAL1_CS,
		GroupID 	=	SG_CLASS_ELESIS4_SPECIAL1_CS,
		PreList 	=	{ },
		NextList 	=	{ SID_CLASS_ELESIS4_SPECIAL2_CS, },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	3,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage595.dds",
		TitleID 	=	20,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.0,
			Motion = MID_ELESIS4_SPECIAL1,			
			CoolTime = 5,
			
			MonsterModeMana = 1.0,
			MonsterModeCoolTime = 5,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS4_SPECIAL2_CS,
		GroupID 	=	SG_CLASS_ELESIS4_SPECIAL2_CS,
		PreList 	=	{ SID_CLASS_ELESIS4_SPECIAL1_CS, },
		NextList 	=	{ SID_CLASS_ELESIS4_SPECIAL3_CS, },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	3,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage596.dds",
		TitleID 	=	24,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ELESIS4_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    {		
		ID 			=	SID_CLASS_ELESIS4_SPECIAL3_CS,
		GroupID 	=	SG_CLASS_ELESIS4_SPECIAL3_CS,
		PreList 	=	{ SID_CLASS_ELESIS4_SPECIAL2_CS, },
		NextList 	=	{ },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	3,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage597.dds",
		TitleID 	=	28,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.7,
			Motion = MID_ELESIS4_SPECIAL3,			
			CoolTime = 15,
			
			MonsterModeMana = 2.7,
			MonsterModeCoolTime = 15,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 엘리시스 캐릭터 스킬
    {		
		ID 			=	SID_CHAR_ELESIS_MAGIC_DEFENSE,
		GroupID 	=	SG_CHAR_ELESIS_MAGIC_DEFENSE,
		PreList 	=	{ },
		NextList 	=	{ SID_CHAR_ELESIS_EVASION },
		CharLv 		=	1,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon49.dds",
		TitleID 	=	106,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,	

		Passive = TRUE,
	},		
	{		
		ID 			=	SID_CHAR_ELESIS_EVASION,
		GroupID 	=	SG_CHAR_ELESIS_EVASION,
		PreList 	=	{ SID_CHAR_ELESIS_MAGIC_DEFENSE, },
		NextList 	=	{ SID_CHAR_ELESIS_BERSERK_LV1, },
		CharLv 		=	2,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon43.dds",
		TitleID 	=	107,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,	

		Passive = TRUE,		
	},		
	{		
		ID 			=	SID_CHAR_ELESIS_BERSERK_LV1,
		GroupID 	=	SG_CHAR_ELESIS_BERSERK,
		PreList 	=	{ SID_CHAR_ELESIS_EVASION, },
		NextList 	=	{ SID_CHAR_ELESIS_BERSERK_LV2, },
		CharLv 		=	3,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon50.dds",
		TitleID 	=	100,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,

        SkillLv     = 1,
		MaxSkillLv  = 3,	

		Passive = TRUE,
	},		
	{		
		ID 			=	SID_CHAR_ELESIS_BERSERK_LV2,
		GroupID 	=	SG_CHAR_ELESIS_BERSERK,
		PreList 	=	{ SID_CHAR_ELESIS_BERSERK_LV1, },
		NextList 	=	{ SID_CHAR_ELESIS_BERSERK_LV3, },
		CharLv 		=	4,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon51.dds",
		TitleID 	=	100,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,

        SkillLv     = 2,
		MaxSkillLv  = 3,

		Passive = TRUE,
	},		
	{		
		ID 			=	SID_CHAR_ELESIS_BERSERK_LV3,
		GroupID 	=	SG_CHAR_ELESIS_BERSERK,
		PreList 	=	{ SID_CHAR_ELESIS_BERSERK_LV2, },
		NextList 	=	{ SID_CHAR_ELESIS_HYPER_ARMOR_LV1, },
		CharLv 		=	5,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon52.dds",
		TitleID 	=	100,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,

        SkillLv     = 3,
		MaxSkillLv  = 3,		
	},
	{		
		ID 			=	SID_CHAR_ELESIS_HYPER_ARMOR_LV1,
		GroupID 	=	SG_CHAR_ELESIS_HYPER_ARMOR,
		PreList 	=	{ SID_CHAR_ELESIS_BERSERK_LV3, },
		NextList 	=	{ SID_CHAR_ELESIS_HYPER_ARMOR_LV2, },
		CharLv 		=	6,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon481.dds",
		TitleID 	=	1177,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,
		SkillLv     = 1,
		MaxSkillLv  = 2,
		
		Passive = TRUE,
	},
	{		
		ID 			=	SID_CHAR_ELESIS_HYPER_ARMOR_LV2,
		GroupID 	=	SG_CHAR_ELESIS_HYPER_ARMOR,
		PreList 	=	{ SID_CHAR_ELESIS_HYPER_ARMOR_LV1, },
		NextList 	=	{ SID_CHAR_ELESIS_CHAR_SKILL_TEMP1, },
		CharLv 		=	7,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon482.dds",
		TitleID 	=	1178,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,

        SkillLv     = 2,
		MaxSkillLv  = 2,

		Passive = TRUE,
	},
	
	{		
		ID 			=	SID_CHAR_ELESIS_CHAR_SKILL_TEMP1,
		GroupID 	=	SG_CHAR_ELESIS_TEMP1,
		PreList 	=	{ SID_CHAR_ELESIS_HYPER_ARMOR_LV2, },
		NextList 	=	{ SID_CHAR_ELESIS_CHAR_SKILL_TEMP2, },
		CharLv 		=	8,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon26.dds",
		TitleID 	=	1179,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,	
		
		Passive = TRUE,
	},
	
	{		
		ID 			=	SID_CHAR_ELESIS_CHAR_SKILL_TEMP2,
		GroupID 	=	SG_CHAR_ELESIS_TEMP2,
		PreList 	=	{ SID_CHAR_ELESIS_CHAR_SKILL_TEMP1, },
		NextList 	=	{ SID_CHAR_ELESIS_CHAR_SKILL_TEMP3, },
		CharLv 		=	9,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon27.dds",
		TitleID 	=	1180,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,			
	},
	
	{		
		ID 			=	SID_CHAR_ELESIS_CHAR_SKILL_TEMP3,
		GroupID 	=	SG_CHAR_ELESIS_TEMP3,
		PreList 	=	{ SID_CHAR_ELESIS_CHAR_SKILL_TEMP2, },
		NextList 	=	{ },
		CharLv 		=	10,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon28.dds",
		TitleID 	=	1181,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,
		
		Passive = TRUE,
			
	},	
	
	-- 각성 스킬
	{		
		ID 			=	SID_ELESIS_AWAKENING_LV1,
		GroupID 	=	SG_ELESIS_AWAKENING_SKILL,
		PreList 	=	{ },
		NextList 	=	{ SID_ELESIS_AWAKENING_LV2, },
		CharLv 		=	20,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon28.dds",
		TitleID 	=	1181,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,
		
		AwakeningLv = 1,
	},	
	
	{		
		ID 			=	SID_ELESIS_AWAKENING_LV2,
		GroupID 	=	SG_ELESIS_AWAKENING_SKILL,
		PreList 	=	{ SID_ELESIS_AWAKENING_LV1, },
		NextList 	=	{ SID_ELESIS_AWAKENING_LV3, },
		CharLv 		=	40,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage15.dds",
		TitleID 	=	1181,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,		

		AwakeningLv = 2,
	},	
	
	{		
		ID 			=	SID_ELESIS_AWAKENING_LV3,
		GroupID 	=	SG_ELESIS_AWAKENING_SKILL,
		PreList 	=	{ SID_ELESIS_AWAKENING_LV2, },
		NextList 	=	{ SID_ELESIS_AWAKENING_LV4, },
		CharLv 		=	60,
		CharType 	=	0,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillimage16.dds",
		TitleID 	=	1181,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,
			
		AwakeningLv = 3,
	},	
}
