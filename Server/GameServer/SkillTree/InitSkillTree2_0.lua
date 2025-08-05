TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ 기사 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- 아르메 기본 마나 4칸 확장
    {		
		ID 			=	SID_ARME1_HIDDEN_MP4,
		GroupID 	=	SG_ARME1_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	2,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon980.dds",
		TitleID 	=	2227,
		DescID 		=	2268,
		ControlDesc	=	430,

		UI_Pos_x 	=	300,
		UI_Pos_y 	=	70,
		UI_LineType =	-1,
		Overlapped  = FALSE,

		LockGroup = -1,
		Passive = TRUE,
    },

    -- 아르메 기본 4단필살기
    {		
		ID 			=	SID_ARME1_SPECIAL_4,
		GroupID 	=	SG_ARME1_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	2,
		Promotion 	=	0,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1023.dds",
		TitleID 	=	2229,
		DescID 		=	2270,
		ControlDesc	=	127,

		UI_Pos_x 	=	385,
		UI_Pos_y 	=	70,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 3.9,
			Motion = MID_ARME1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			ChargingSkillLV = 4,
		},
    },
	-- 생명전환
    {		
        ID 	=	 SID_ARME1_CHANGE_HP_TO_MP ,
        GroupID 	=	 SG_ARME_COMMON_UNIQUE_SKILL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon1396.dds",
        TitleID 	=	 194,
        DescID 	=	 265,
        ControlDesc	=	213,
        		
        UI_Pos_x 	=	339,
        UI_Pos_y 	=	409,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
    -- 기본 텔레포트
    {		
        ID 	=	 SID_ARME1_BASE_TELEPORT ,
        GroupID 	=	 SG_ARME_COMMON_DASH,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_TELEPORT_SPEED_UP},
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon54.dds",
        TitleID 	=	 191,
        DescID 	=	 262,
        ControlDesc	=	212,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    -- 텔레포트 스피드 업
    {		
        ID 	=	 SID_ARME1_TELEPORT_SPEED_UP ,
        GroupID 	=	 SG_ARME_COMMON_DASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 49,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1397.dds",
        TitleID 	=	 190,
        DescID 	=	 261,
        ControlDesc	=	210,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	162,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },	
    -- 텔레포트 수직이동
    {		
        ID 	=	 SID_ARME1_TELEPORT_VERTICAL ,
        GroupID 	=	 SG_ARME_COMMON_DASH_PLUS1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1398.dds",
        TitleID 	=	 192,
        DescID 	=	 263,
        ControlDesc	=	211,
        		
        UI_Pos_x 	=	339,
        UI_Pos_y 	=	245,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
    -- 텔레포트 백이동
    {		
        ID 	=	 SID_ARME1_TELEPORT_BACK_MOVE ,
        GroupID 	=	 SG_ARME_COMMON_DASH_PLUS2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1399.dds",
        TitleID 	=	 193,
        DescID 	=	 264,
        ControlDesc	=	212,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	326,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },	
    -- 기상공격
    {		
        ID 	=	 SID_ARME1_STAND_UP_ATK ,
        GroupID 	=	 SG_ARME_STAND_UP_ATK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 6,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1400.dds",
        TitleID 	=	 200,
        DescID 	=	 271,
        ControlDesc	=	217,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
    -- 정신회복 1
    {		
        ID 	=	 SID_ARME1_SPIRIT_RECOVERY_LV1 ,
        GroupID 	=	 SG_ARME_COMMON_UNIQUE_SKILL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_SPIRIT_RECOVERY_LV2,},
        CharLv 	=	 20,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1408_1.dds",
        TitleID 	=	 195,
        DescID 	=	 266,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	490,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    -- 정신회복 2
    {		
        ID 	=	 SID_ARME1_SPIRIT_RECOVERY_LV2 ,
        GroupID 	=	 SG_ARME_COMMON_UNIQUE_SKILL2,
        PreList 	=	 {SID_ARME1_SPIRIT_RECOVERY_LV1,},
        NextList 	=	 {SID_ARME1_SPIRIT_RECOVERY_LV3,},
        CharLv 	=	 42,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1408_2.dds",
        TitleID 	=	 196,
        DescID 	=	 267,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	536,
        UI_Pos_y 	=	490,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    -- 정신회복 3
    {		
        ID 	=	 SID_ARME1_SPIRIT_RECOVERY_LV3 ,
        GroupID 	=	 SG_ARME_COMMON_UNIQUE_SKILL2,
        PreList 	=	 {SID_ARME1_SPIRIT_RECOVERY_LV2,},
        NextList 	=	 {},
        CharLv 	=	 57,
        CharType 	=	 2,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon1408_3.dds",
        TitleID 	=	 197,
	DescID 	=	 268,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	599,
        UI_Pos_y 	=	490,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    -- 신 콤보
    {		
        ID 	=	 SID_ARME1_NEWCOMBO ,
        GroupID 	=	 SG_ARME_COMBO,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 4,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1441.dds",
        TitleID 	=	 206,
        DescID 	=	 277,
        ControlDesc	=	221,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    -- 신 크리티컬 어택
    {		
        ID 	=	 SID_ARME1_NEW_CRITICAL_ATK ,
        GroupID 	=	 SG_ARME_CRITICAL_ATK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 8,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1445.dds",
        TitleID 	=	 202,
        DescID 	=	 273,
        ControlDesc	=	218,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    -- 신 더블 어택
    {		
        ID 	=	 SID_ARME1_NEW_DOUBLE_ATK ,
        GroupID 	=	 SG_ARME_DOUBLE_ATK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 8,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1446.dds",
        TitleID 	=	 204,
        DescID 	=	 275,
        ControlDesc	=	219,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	-- 마법사 인형
    {		
        ID 	=	 SID_ARME1_WIZARD_DOLL ,
        GroupID 	=	 SG_ARME_COMBO_PLUS1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1447.dds",
        TitleID 	=	 198,
        DescID 	=	 269,
        ControlDesc	=	215,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    -- 어스웨이브(기본 콤보 응용)
    {		
        ID 	=	 SID_ARME1_BASE_EARTHWAVE,
        GroupID 	=	 SG_ARME_COMBO_PLUS2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "SID_ARME1_BASE_EARTHWAVE.dds",
        TitleID 	=	 199,
        DescID 	=	 270,
        ControlDesc	=	216,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    -- 백마법 1단 월오브아이스
    {		
        ID 	=	 SID_ARME1_WHITE1_WALLOFICE ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1410.dds",
        TitleID 	=	 170,
        DescID 	=	 241,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	161,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_W1_WALLOFICE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 1단 프로스트 링 레벨1
    {		
        ID 	=	 SID_ARME1_WHITE1_FROSTRING_LV1 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_WHITE1_FROSTRING_LV2,},
        CharLv 	=	 12,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1411.dds",
        TitleID 	=	 171,
        DescID 	=	 242,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_W1_FROSTRING_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 1단 프로스트 링 레벨2
    {		
        ID 	=	 SID_ARME1_WHITE1_FROSTRING_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {SID_ARME1_WHITE1_FROSTRING_LV1,},
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1412.dds",
        TitleID 	=	 172,
        DescID 	=	 243,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_W1_FROSTRING_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 1단 매직쉴드 레벨2
    {		
        ID 	=	 SID_ARME1_WHITE1_BASE_MGSHIELD_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 22,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1413.dds",
        TitleID 	=	 174,
        DescID 	=	 245,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	161,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_W1_BASE_MGSHIELD_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 1단 프로텍션 프롬 파이어 레벨1
    {		
        ID 	=	 SID_ARME1_WHITE1_PROTECTIONFIRE_LV1 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_ARME1_WHITE1_PROTECTIONFIRE_LV2, },
        CharLv 	=	 6,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
		Img 	=	 "skillicon1414.dds",
        TitleID 	=	 175,
        DescID 	=	 246,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 7,  
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_W1_PROTECTIONFIRE_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
     -- 백마법 1단 프로텍션 프롬 파이어 레벨2
    {		
        ID 	=	 SID_ARME1_WHITE1_PROTECTIONFIRE_LV2 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 { SID_ARME1_WHITE1_PROTECTIONFIRE_LV1,},
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
		Img 	=	 "skillicon1415.dds",
        TitleID 	=	 176,
        DescID 	=	 247,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	527,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 1,   
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_W1_PROTECTIONFIRE_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 2단
    {		
        ID 	=	 SID_ARME1_WHITE2_ICESTRIKE ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1401.dds",
        TitleID 	=	 177,
        DescID 	=	 248,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	300,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL2,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_WHITE2_ICESTRIKE,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 2단
    {		
        ID 	=	 SID_ARME1_WHITE2_ICECIRCULAR_LV1 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_WHITE2_ICECIRCULAR_LV2},
        CharLv 	=	 14,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1402.dds",
        TitleID 	=	 178,
        DescID 	=	 249,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL2,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_WHITE2_ICECIRCULAR_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 2단
    {		
        ID 	=	 SID_ARME1_WHITE2_ICECIRCULAR_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2,
        PreList 	=	 {SID_ARME1_WHITE2_ICECIRCULAR_LV1},
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1403.dds",
        TitleID 	=	 179,
        DescID 	=	 250,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL2,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_WHITE2_ICECIRCULAR_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 2단
    {		
        ID 	=	 SID_ARME1_WHITE2_BASE_SPECIAL_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 24,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1404.dds",
        TitleID 	=	 181,
        DescID 	=	 252,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	299,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_W2_BASE_SPECIAL_LV2_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 2단
    {		
        ID 	=	 SID_ARME1_WHITE2_ICETRAP_LV1 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 7,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1405.dds",
        TitleID 	=	 182,
        DescID 	=	 253,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_WHITE2_ICETRAP_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 3단 페이스 오브 갓
    {		
        ID 	=	 SID_ARME1_WHITE3_FACEOFGOD ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1416.dds",
        TitleID 	=	 183,
        DescID 	=	 254,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL3,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_W3_FACEOFGOD_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 3단 홀리스트라이크 레벨1
    {		
        ID 	=	 SID_ARME1_WHITE3_HOLYSTRIKE_LV1 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_WHITE3_HOLYSTRIKE_LV2},
        CharLv 	=	 17,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1417.dds",
        TitleID 	=	 184,
        DescID 	=	 255,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_W3_HOLYSTRIKE_LV1_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 3단 홀리스트라이크 레벨2
    {		
        ID 	=	 SID_ARME1_WHITE3_HOLYSTRIKE_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {SID_ARME1_WHITE3_HOLYSTRIKE_LV1},
        NextList 	=	 {},
        CharLv 	=	 55,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1418.dds",
        TitleID 	=	 185,
        DescID 	=	 256,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	527,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_W3_HOLYSTRIKE_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 3단 큐어 레벨2
    {		
        ID 	=	 SID_ARME1_WHITE3_BASE_CURE_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1419.dds",
        TitleID 	=	 187,
        DescID 	=	 258,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_W3_BASE_CURE_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 3단 고르고스플레임 레벨 1
    {		
        ID 	=	 SID_ARME1_WHITE3_GORGOSFLAME_LV1 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_WHITE3_GORGOSFLAME_LV2, },
        CharLv 	=	 16,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1420.dds",
        TitleID 	=	 188,
        DescID 	=	 259,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL3,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_W3_GORGOSFLAME_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 백마법 3단 고르고스플레임 레벨 2
    {		
        ID 	=	 SID_ARME1_WHITE3_GORGOSFLAME_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {SID_ARME1_WHITE3_GORGOSFLAME_LV1, },
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1421.dds",
        TitleID 	=	 189,
        DescID 	=	 260,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ARME1_WHITE_SPECIAL3,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_W3_GORGOSFLAME_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 1단 블러드페스트
    {		
        ID 	=	 SID_ARME1_BLACK1_BLOODPEST ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1422.dds",
        TitleID 	=	 151,
        DescID 	=	 222,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	161,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_B1_BLOODPEST,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 1단 피어 레벨1
    {		
        ID 	=	 SID_ARME1_BLACK1_FEAR_LV1 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK1_FEAR_LV2},
        CharLv 	=	 12,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1423.dds",
        TitleID 	=	 152,
        DescID 	=	 223,
        ControlDesc	=	207,   
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 7,   
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_B1_FEAR_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 1단 피어 레벨2
    {		
        ID 	=	 SID_ARME1_BLACK1_FEAR_LV2 ,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {SID_ARME1_BLACK1_FEAR_LV1},
        NextList 	=	 {},
        CharLv 	=	 43,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1424.dds",
        TitleID 	=	 153,
        DescID 	=	 224,
        ControlDesc	=	207, 
        		
        UI_Pos_x 	=	527,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 1,    
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_B1_FEAR_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 1단 스톤커스 레벨2
    {		
        ID 	=	 SID_ARME1_BLACK1_BASE_STONECURSE_LV2 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 18,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1425.dds",
        TitleID 	=	 155,
        DescID 	=	 226,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	161,
        UI_LineType 	=	 4,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_B1_BASE_STONE_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 1단 피어 레벨1
    {		
        ID 	=	 SID_ARME1_BLACK1_HOLDBEAM_LV1 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_ARME1_BLACK1_HOLDBEAM_LV2, },
        CharLv 	=	 13,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1426.dds",
        TitleID 	=	 156,
        DescID 	=	 227,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 5,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_B1_HOLDBEAM_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 1단 피어 레벨2
    {		
        ID 	=	 SID_ARME1_BLACK1_HOLDBEAM_LV2 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 { SID_ARME1_BLACK1_HOLDBEAM_LV1, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1427.dds",
        TitleID 	=	 157,
        DescID 	=	 228,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_ARME1_B1_HOLDBEAM_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },    
    -- 흑마법 2단 데스 클라우드
    {		
        ID 	=	 SID_ARME1_BLACK2_DEATHCLOUD ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1428.dds",
        TitleID 	=	 158,
        DescID 	=	 229,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	299,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_B2_DEATHCLOUD,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 2단 에너지볼 레벨1
    {		
        ID 	=	 SID_ARME1_BLACK2_ENERGYBALL_LV1 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK2_ENERGYBALL_LV2,},
        CharLv 	=	 8,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1429.dds",
        TitleID 	=	 159,
        DescID 	=	 230,
        ControlDesc	=	208, 
        		
        UI_Pos_x 	=	314,
        UI_Pos_y 	=	332,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_B2_ENERGYBALL_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 2단 에너지볼 레벨2 
    {		
        ID 	=	 SID_ARME1_BLACK2_ENERGYBALL_LV2 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2,
        PreList 	=	 {SID_ARME1_BLACK2_ENERGYBALL_LV1,},
        NextList 	=	 {},
        CharLv 	=	 45,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1430.dds",
        TitleID 	=	 160,
        DescID 	=	 231,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	251,
        UI_Pos_y 	=	332,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_B2_ENERGYBALL_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 2단 쇼크스턴 레벨1
    {		
        ID 	=	 SID_ARME1_BLACK2_SHOCKSTURN_LV1 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK2_SHOCKSTURN_LV2,},
        CharLv 	=	 8,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1432.dds",
        TitleID 	=	 162,
        DescID 	=	 233,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 7,
       
        LockGroup = BGID_ARME1_BLACK_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_B2_SHOCKSTURN_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 2단 쇼크스턴 레벨2 
    {		
        ID 	=	 SID_ARME1_BLACK2_SHOCKSTURN_LV2 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2,
        PreList 	=	 {SID_ARME1_BLACK2_SHOCKSTURN_LV1,},
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1433.dds",
        TitleID 	=	 163,
        DescID 	=	 234,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	527,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 1,
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_ARME1_B2_SHOCKSTURN_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 3단 세인트(팬덤) 브리딩
    {		
        ID 	=	 SID_ARME1_BLACK3_SAINTBREEDING ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 37,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1434.dds",
        TitleID 	=	 164,
        DescID 	=	 235,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_B3_SAINTBREEDING,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 3단 피니쉬 터치 레벨1
    {		
        ID 	=	 SID_ARME1_BLACK3_FINISHTOUCH_LV1 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK3_FINISHTOUCH_LV2,},
        CharLv 	=	 10,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1435.dds",
        TitleID 	=	 165,
        DescID 	=	 236,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 7,
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_B3_FINISHTOUCH_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 3단 피니쉬 터치 레벨2
    {		
        ID 	=	 SID_ARME1_BLACK3_FINISHTOUCH_LV2,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3,
        PreList 	=	 {SID_ARME1_BLACK3_FINISHTOUCH_LV1,},
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1454.dds",
        TitleID 	=	 166,
        DescID 	=	 237,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	527,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 1,
        
        LockGroup = BGID_ARME1_BLACK_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_B3_FINISHTOUCH_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 3단 새틀라이트 밤 레벨1
    {		
        ID 	=	 SID_ARME1_BLACK3_SATELLITEBOOM_LV1 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK3_SATELLITEBOOM_LV2,},
        CharLv 	=	 17,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1431.dds",
        TitleID 	=	 168,
        DescID 	=	 239,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	314,
        UI_Pos_y 	=	471,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_B3_SATELLITE_LV1_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 흑마법 3단 새틀라이트 밤 레벨2
    {		
        ID 	=	 SID_ARME1_BLACK3_SATELLITEBOOM_LV2 ,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3,
        PreList 	=	 {SID_ARME1_BLACK3_SATELLITEBOOM_LV1,},
        NextList 	=	 {},
        CharLv 	=	 37,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1437.dds",
        TitleID 	=	 169,
        DescID 	=	 240,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	251,
        UI_Pos_y 	=	471,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_ARME1_B3_SATELLITE_LV2_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },    
    -- 기본 콤보
    {		
        ID 	=	 SID_ARME1_BASE_COMBO,
        GroupID 	=	 SG_ARME_COMBO,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "SID_ARME1_BASE_COMBO.dds",
        TitleID 	=	 205,
        DescID 	=	 276,
        ControlDesc	=	220,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },    
    -- 기본 크리티컬 어택
    {		
        ID 	=	 SID_ARME1_BASE_CRITICAL_ATK,
        GroupID 	=	 SG_ARME_CRITICAL_ATK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon66.dds",
        TitleID 	=	 201,
        DescID 	=	 272,
        ControlDesc	=	218,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },    
	-- 기본 더블 어택
    {		
        ID 	=	 SID_ARME1_BASE_DOUBLE_ATK,
        GroupID 	=	 SG_ARME_DOUBLE_ATK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon66.dds",
        TitleID 	=	 204,
        DescID 	=	 274,
        ControlDesc	=	219,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },        
    -- 기본 백마법 1단
    {		
        ID 	=	 SID_ARME1_BASE_WHITE_SPECIAL1,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_WHITE1_WALLOFICE, SID_ARME1_WHITE1_FROSTRING_LV1, SID_ARME1_WHITE1_BASE_MGSHIELD_LV2, SID_ARME1_BLACK1_FEAR_LV1, },
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage62.dds",
        TitleID 	=	 173,
        DescID 	=	 244,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_ARME_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },        
    -- 기본 백마법 2단
    {		
        ID 	=	 SID_ARME1_BASE_WHITE_SPECIAL2,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_WHITE2_ICESTRIKE, SID_ARME1_WHITE2_ICECIRCULAR_LV1, SID_ARME1_WHITE2_BASE_SPECIAL_LV2, SID_ARME1_WHITE2_ICETRAP_LV1, },
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage63.dds",
        TitleID 	=	 180,
        DescID 	=	 251,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ARME_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },    
    -- 기본 백마법 3단
    {		
        ID 	=	 SID_ARME1_BASE_WHITE_SPECIAL3,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_ARME1_WHITE3_FACEOFGOD, SID_ARME1_WHITE3_HOLYSTRIKE_LV1, SID_ARME1_WHITE3_BASE_CURE_LV2, SID_ARME1_WHITE3_GORGOSFLAME_LV1, },
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage65.dds",
        TitleID 	=	 186,
        DescID 	=	 257,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_ARME_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},				
    },  
    -- 기본 흑마법 1단
    {		
        ID 	=	 SID_ARME1_BASE_BLACK_SPECIAL1,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK1_BLOODPEST, SID_ARME1_WHITE1_PROTECTIONFIRE_LV1, SID_ARME1_BLACK1_BASE_STONECURSE_LV2, SID_ARME1_BLACK1_HOLDBEAM_LV1, },
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage58.dds",
        TitleID 	=	 154,
        DescID 	=	 225,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	307,
        UI_Pos_y 	=	467,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_ARME_SPECIAL1_BLACK,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },                  
    -- 기본 흑마법 2단
    {		
        ID 	=	 SID_ARME1_BASE_BLACK_SPECIAL2,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK2_DEATHCLOUD, SID_ARME1_BLACK2_ENERGYBALL_LV1, SID_ARME1_BLACK2_SHOCKSTURN_LV1,  },
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage60.dds",
        TitleID 	=	 161,
        DescID 	=	 232,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ARME_SPECIAL2_BLACK,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },                
    -- 기본 흑마법 3단
    {		
        ID 	=	 SID_ARME1_BASE_BLACK_SPECIAL3,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ARME1_BLACK3_SAINTBREEDING, SID_ARME1_BLACK3_FINISHTOUCH_LV1, SID_ARME1_BLACK3_SATELLITEBOOM_LV1,  },
        CharLv 	=	 0,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage61.dds",
        TitleID 	=	 167,
        DescID 	=	 238,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	320,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_ARME_SPECIAL3_BLACK,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },  
    -- 기본 캐쉬 백마법 1단 
    {		
        ID 	=	 SID_ARME1_BASE_WHITE_SPECIAL1_CASH,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL1_CASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,

        GoodsID 	=	 {1988, 4145, 7633, 7651, 10217 },
        Base 	=	 FALSE,
        LearnSP = 0,
        Cash    =   TRUE,
		
        Img 	=	 "skillimage227.dds",
        TitleID 	=	 287,
        DescID 	=	 293,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	193,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_ARME_SPECIAL_SHINING_ARROW,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },                      
    -- 기본 캐쉬 백마법 2단 
    {		
        ID 	=	 SID_ARME1_BASE_WHITE_SPECIAL2_CASH,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL2_CASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {2166, 4154, 7644, 7662,},
        Base 	=	 FALSE,
        LearnSP = 0,
        Cash    =   TRUE,
		
        Img 	=	 "skillimage275.dds",
        TitleID 	=	 288,
        DescID 	=	 294,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,

		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ARME_ATHENS_SWORD,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },                      
    -- 기본 캐쉬 백마법 3단 
    {		
        ID 	=	 SID_ARME1_BASE_WHITE_SPECIAL3_CASH,
        GroupID 	=	 SG_ARME_WHITE_SPECIAL3_CASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {2167, 7645, 7663, },
        Base 	=	 FALSE,
        LearnSP = 0,
        Cash    =   TRUE,
		
        Img 	=	 "skillimage272.dds",
        TitleID 	=	 289,
        DescID 	=	 295,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,

		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_ARME_BOOST_HEAL,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },  
    -- 기본 캐쉬 흑마법 1단 
    {		
        ID 	=	 SID_ARME1_BASE_BLACK_SPECIAL1_CASH,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL1_CASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {2163, 7641, 7659, 10222,},
        Base 	=	 FALSE,
        LearnSP = 0,
        Cash    =   TRUE,
		
        Img 	=	 "skillimage273.dds",
        TitleID 	=	 290,
        DescID 	=	 296,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	193,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_ARME_DARK_SWORM,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },                                          
    -- 기본 캐쉬 흑마법 2단 
    {		
        ID 	=	 SID_ARME1_BASE_BLACK_SPECIAL2_CASH,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL2_CASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {2164, 7642, 7660, },
        Base 	=	 FALSE,
        LearnSP = 0,
        Cash    =   TRUE,
		
        Img 	=	 "skillimage270.dds",
        TitleID 	=	 291,
        DescID 	=	 297,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ARME_SPECIAL_CONFUSION,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },  
    -- 기본 캐쉬 흑마법 3단 
    {		
        ID 	=	 SID_ARME1_BASE_BLACK_SPECIAL3_CASH,
        GroupID 	=	 SG_ARME_BLACK_SPECIAL3_CASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 2,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {2165, 4153, 7643, 7661,},
        Base 	=	 FALSE,
        LearnSP = 0,
        Cash    =   TRUE,
		
        Img 	=	 "skillimage271.dds",
        TitleID 	=	 292,
        DescID 	=	 298,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_ARME_LAVA_FLOW,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },     
}