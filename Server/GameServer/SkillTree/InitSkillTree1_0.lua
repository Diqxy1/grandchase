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
	-- 리르 기본 마나 4칸 확장
    {		
		ID 			=	SID_LIRE1_HIDDEN_MP4,
		GroupID 	=	SG_LIRE1_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	1,
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

    -- 리르 기본 4단필살기
    {		
		ID 			=	SID_LIRE1_SPECIAL_4,
		GroupID 	=	SG_LIRE1_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	1,
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
			Motion = MID_LIRE1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			ChargingSkillLV = 4,
		},
    },
	-- 마탄의 사수
    {		
        ID 	=	 SID_LIRE1_HUNGRY_ARROW,
        GroupID 	=	 SG_LIRE1_SHOT_DOWN,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 1,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon114.dds",
        TitleID 	=	 299,
        DescID 	=	 348,
        ControlDesc	=	430,
        		
        UI_Pos_x 	=	330,
        UI_Pos_y 	=	255,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
	-- 날쌘 엘프
    {		
        ID 	=	 SID_LIRE1_MOVE_SPEEDUP,
        GroupID 	=	 SG_LIRE1_IMAGE,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_MP_REGEN_SPEEDUP, },
        CharLv 	=	 20,
        CharType 	=	 1,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon107.dds",
        TitleID 	=	 300,
        DescID 	=	 349,
        ControlDesc	=	398,
        		
        UI_Pos_x 	=	461,
        UI_Pos_y 	=	385,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--정신력 향상
    {		
        ID 	=	 SID_LIRE1_MP_REGEN_SPEEDUP,
        GroupID 	=	 SG_LIRE1_IMAGE,
        PreList 	=	 { SID_LIRE1_MOVE_SPEEDUP, },
        NextList 	=	 { SID_LIRE1_SHOT_SPEEDUP, },
        CharLv 	=	 30,
        CharType 	=	 1,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon131.dds",
        TitleID 	=	 301,
        DescID 	=	 350,
        ControlDesc	=	399,
        		
        UI_Pos_x 	=	524,
        UI_Pos_y 	=	385,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    --속사의 명인
	{		
        ID 	=	 SID_LIRE1_SHOT_SPEEDUP,
        GroupID 	=	 SG_LIRE1_IMAGE,
        PreList 	=	 {SID_LIRE1_MP_REGEN_SPEEDUP},
        NextList 	=	 {},
        CharLv 	=	 46,
        CharType 	=	 1,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon122.dds",
        TitleID 	=	 302,
        DescID 	=	 351,
        ControlDesc	=	400,
        		
        UI_Pos_x 	=	587,
        UI_Pos_y 	=	385,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--포격 모드
	{		
        ID 	=	 SID_LIRE1_SIEGE,
        GroupID 	=	 SG_LIRE1_SHOOTING_SKILL,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_DOUBLE_SIEGE_SHOT, },
        CharLv 	=	 16,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon135.dds",
        TitleID 	=	 303,
        DescID 	=	 352,
        ControlDesc	=	401,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--대쉬
	{		
        ID 	=	 SID_LIRE1_BASE_DASH,
        GroupID 	=	 SG_LIRE1_DASH,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_ON_JUMP_DASH, },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon130.dds",
        TitleID 	=	431,
        DescID 	=	 432,
        ControlDesc	=	433,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--점프 대쉬
	{		
        ID 	=	 SID_LIRE1_ON_JUMP_DASH,
        GroupID 	=	 SG_LIRE1_DASH,
        PreList 	=	 { SID_LIRE1_BASE_DASH, },
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon130.dds",
        TitleID 	=	 304,
        DescID 	=	 353,
        ControlDesc	=	402,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--점핑 샷
	{		
        ID 	=	 SID_LIRE1_BASE_JUMPING_SHOT,
        GroupID 	=	 SG_LIRE1_JUMP_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_JUMPING_DOUBLE_SHOT, },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage141.dds",
        TitleID 	=	 305,
        DescID 	=	 354,
        ControlDesc	=	403,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--점핑 더블 샷
	{		
        ID 	=	 SID_LIRE1_JUMPING_DOUBLE_SHOT,
        GroupID 	=	 SG_LIRE1_JUMP_ATK,
        PreList 	=	 { SID_LIRE1_BASE_JUMPING_SHOT, },
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon129.dds",
        TitleID 	=	 306,
        DescID 	=	 355,
        ControlDesc	=	404,
        		
        UI_Pos_x 	=	200,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--데미지 증폭기 LV1
	{		
        ID 	=	 SID_LIRE1_DAMAGE_AMPLIFIER_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_DAMAGE_AMPLIFIER_LV2, },
        CharLv 	=	 12,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon112.dds",
        TitleID 	=	 307,
        DescID 	=	 356,
        ControlDesc	=	405,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--데미지 증폭기 LV2
	{		
        ID 	=	 SID_LIRE1_DAMAGE_AMPLIFIER_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL_ATK,
        PreList 	=	 { SID_LIRE1_DAMAGE_AMPLIFIER_LV1, },
        NextList 	=	 { SID_LIRE1_DAMAGE_AMPLIFIER_LV3, },
        CharLv 	=	 26,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon110.dds",
        TitleID 	=	 308,
        DescID 	=	 357,
        ControlDesc	=	406,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--데미지 증폭기 LV3
	{		
        ID 	=	 SID_LIRE1_DAMAGE_AMPLIFIER_LV3,
        GroupID 	=	 SG_LIRE1_SPECIAL_ATK,
        PreList 	=	 { SID_LIRE1_DAMAGE_AMPLIFIER_LV2, },
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon111.dds",
        TitleID 	=	 309,
        DescID 	=	 358,
        ControlDesc	=	407,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--대쉬 샷
	{		
        ID 	=	 SID_LIRE1_BASE_DASH_SHOT,
        GroupID 	=	 SG_LIRE1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_DASH_DOUBLE_ATK, },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage140.dds",
        TitleID 	=	 311,
        DescID 	=	 360,
        ControlDesc	=	409,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--대쉬 더블 샷
	{		
        ID 	=	 SID_LIRE1_DASH_DOUBLE_ATK,
        GroupID 	=	 SG_LIRE1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon109.dds",
        TitleID 	=	 312,
        DescID 	=	 361,
        ControlDesc	=	410,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--엘프의 몸놀림
	{		
        ID 	=	 SID_LIRE1_DODGE,
        GroupID 	=	 SG_LIRE1_DODGE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon124.dds",
        TitleID 	=	 310,
        DescID 	=	 359,
        ControlDesc	=	408,
        		
        UI_Pos_x 	=	339,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--연발 사격
	{		
        ID 	=	 SID_LIRE1_CONTINUOUS_COMBO_SHOT,
        GroupID 	=	 SG_LIRE1_COMBO_PLUS2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 42,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon125.dds",
        TitleID 	=	 313,
        DescID 	=	 362,
        ControlDesc	=	411,
        		
        UI_Pos_x 	=	290,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--근거리 반격 LV1
	{		
        ID 	=	 SID_LIRE1_MELEE_COMBO_LV1,
        GroupID 	=	 SG_LIRE1_COMBO_PLUS1,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_MELEE_COMBO_LV2, },
        CharLv 	=	 14,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon105.dds",
        TitleID 	=	 314,
        DescID 	=	 363,
        ControlDesc	=	412,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--근거리 반격 LV2
	{		
        ID 	=	 SID_LIRE1_MELEE_COMBO_LV2,
        GroupID 	=	 SG_LIRE1_COMBO_PLUS1,
        PreList 	=	 { SID_LIRE1_MELEE_COMBO_LV1, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon106.dds",
        TitleID 	=	 315,
        DescID 	=	 364,
        ControlDesc	=	413,
        		
        UI_Pos_x 	=	158,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--콤보 사격 LV1
	{		
        ID 	=	 SID_LIRE1_COMBO_A,
        GroupID 	=	 SG_LIRE1_COMBO,
        PreList 	=	 {},
        NextList 	=	 { SG_LIRE1_COMBO_B, },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon132.dds",
        TitleID 	=	 316,
        DescID 	=	 365,
        ControlDesc	=	414,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--콤보 사격 LV2
	{		
        ID 	=	 SID_LIRE1_COMBO_B,
        GroupID 	=	 SG_LIRE1_COMBO,
        PreList 	=	 { SG_LIRE1_COMBO_A, },
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon132.dds",
        TitleID 	=	 317,
        DescID 	=	 366,
        ControlDesc	=	415,
        		
        UI_Pos_x 	=	162,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--연쇄 포격
	{		
        ID 	=	 SID_LIRE1_DOUBLE_SIEGE_SHOT,
        GroupID 	=	 SG_LIRE1_SHOOTING_SKILL,
        PreList 	=	 { SID_LIRE1_SIEGE, },
        NextList 	=	 {},
        CharLv 	=	 18,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon128.dds",
        TitleID 	=	 318,
        DescID 	=	 367,
        ControlDesc	=	416,
        		
        UI_Pos_x 	=	339,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--메탈 샷 LV1
	{		
        ID 	=	 SID_LIRE1_METALSHOT_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_METALSHOT_LV2, SID_LIRE1_CONTINUOUS_ATK_LV1, SID_LIRE1_FIRE_ARROW_LV1, SID_LIRE1_FROZEN_ARROW,  },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage37.dds",
        TitleID 	=	 319,
        DescID 	=	 368,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_LIRE_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--메탈 샷 LV2
	{		
        ID 	=	 SID_LIRE1_METALSHOT_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_METALSHOT_LV1, },
        NextList 	=	 { SID_LIRE1_METALSHOT_LV3, },
        CharLv 	=	 6,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon116.dds",
        TitleID 	=	 320,
        DescID 	=	 369,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_METALSHOT_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--메탈 샷 LV3
	{		
        ID 	=	 SID_LIRE1_METALSHOT_LV3,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_METALSHOT_LV2, },
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon117.dds",
        TitleID 	=	 321,
        DescID 	=	 370,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_METALSHOT_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--래피드 샷 LV1
	{		
        ID 	=	 SID_LIRE1_CONTINUOUS_ATK_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_METALSHOT_LV1, },
        NextList 	=	 { SID_LIRE1_CONTINUOUS_ATK_LV2, },
        CharLv 	=	 19,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon127.dds",
        TitleID 	=	 322,
        DescID 	=	 371,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_CONTINUOUS_ATK_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--래피드 샷 LV2
	{		
        ID 	=	 SID_LIRE1_CONTINUOUS_ATK_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_CONTINUOUS_ATK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon126.dds",
        TitleID 	=	 323,
        DescID 	=	 372,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_CONTINUOUS_ATK_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--파이어 애로우 LV1
	{		
        ID 	=	 SID_LIRE1_FIRE_ARROW_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_METALSHOT_LV1, },
        NextList 	=	 { SID_LIRE1_FIRE_ARROW_LV2, },
        CharLv 	=	 6,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon134.dds",
        TitleID 	=	 324,
        DescID 	=	 373,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_LIRE1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_FIRE_ARROW_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--파이어 애로우 LV2
	{		
        ID 	=	 SID_LIRE1_FIRE_ARROW_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_FIRE_ARROW_LV1, },
        NextList 	=	 {},
        CharLv 	=	 19,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon133.dds",
        TitleID 	=	 325,
        DescID 	=	 374,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_FIRE_ARROW_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--프리징 애로우
	{		
        ID 	=	SID_LIRE1_FROZEN_ARROW,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_METALSHOT_LV1, },
        NextList 	=	 { SID_LIRE1_POISON_ARROW, },
        CharLv 	=	 31,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon120.dds",
        TitleID 	=	 326,
        DescID 	=	 375,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_LIRE1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_FROZEN_ARROW,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--포이즌 애로우
	{		
        ID 	=	SID_LIRE1_POISON_ARROW,
        GroupID 	=	 SG_LIRE1_SPECIAL1,
        PreList 	=	 { SID_LIRE1_FROZEN_ARROW, },
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon113.dds",
        TitleID 	=	 327,
        DescID 	=	 376,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LIRE_POISON_ARROW,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--레인 애로우
	{		
        ID 	=	SID_LIRE1_RAINARROW,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_HEADSHOT_LV1, SID_LIRE1_PIERCESHOT_LV1, SID_LIRE1_DARK_SHOT, SID_LIRE1_THORN_TRAP, },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage35.dds",
        TitleID 	=	 328,
        DescID 	=	 377,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	339,
        UI_Pos_y 	=	409,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_LIRE_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--헤드 샷 LV1
	{		
        ID 	=	SID_LIRE1_HEADSHOT_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_RAINARROW, },
        NextList 	=	 { SID_LIRE1_HEADSHOT_LV2, },
        CharLv 	=	 22,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon137.dds",
        TitleID 	=	 329,
        DescID 	=	 378,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_HEADSHOT_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--헤드 샷 LV2
	{		
        ID 	=	SID_LIRE1_HEADSHOT_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_HEADSHOT_LV1, },
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon136.dds",
        TitleID 	=	 330,
        DescID 	=	 379,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_HEADSHOT_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--샤프 슛 LV1
	{		
        ID 	=	SID_LIRE1_PIERCESHOT_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_RAINARROW, },
        NextList 	=	 { SID_LIRE1_PIERCESHOT_LV2, },
        CharLv 	=	 8,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon102.dds",
        TitleID 	=	 331,
        DescID 	=	 380,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	363,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_PIERCESHOT_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--샤프 슛 LV2
	{		
        ID 	=	SID_LIRE1_PIERCESHOT_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_PIERCESHOT_LV1, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon101.dds",
        TitleID 	=	 332,
        DescID 	=	 381,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	363,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_PIERCESHOT_L2V,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
        
    },
	--슈팅 트랩 LV1 (가시트랩)
	{		
        ID 	=	SID_LIRE1_THORN_TRAP,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_RAINARROW, },
        NextList 	=	 { SID_LIRE1_FIRE_TRAP, },
        CharLv 	=	 8,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon100.dds",
        TitleID 	=	 333,
        DescID 	=	 382,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_LIRE1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_THORNTRAP_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--슈팅 트랩 LV2(화염트랩)
	{		
        ID 	=	SID_LIRE1_FIRE_TRAP,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_THORN_TRAP, },
        NextList 	=	 { SID_LIRE1_HOLY_TRAP, },
        CharLv 	=	 22,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon139.dds",
        TitleID 	=	 334,
        DescID 	=	 383,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_FIRETRAP_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--슈팅 트랩 LV3(신성트랩)
	{		
        ID 	=	SID_LIRE1_HOLY_TRAP,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_FIRE_TRAP, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon138.dds",
        TitleID 	=	 335,
        DescID 	=	 384,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	206,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_HOLYTRAP_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--다크 샷
	{		
        ID 	=	SID_LIRE1_DARK_SHOT,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_RAINARROW, },
        NextList 	=	 { SID_LIRE1_SHOT_MAGIC_CIRCLE,},
        CharLv 	=	 41,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon108.dds",
        TitleID 	=	 336,
        DescID 	=	 385,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_LIRE1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_DARKSHOT_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--스캐터 샷 (산탄 마법진)
	{		
        ID 	=	SID_LIRE1_SHOT_MAGIC_CIRCLE,
        GroupID 	=	 SG_LIRE1_SPECIAL2,
        PreList 	=	 { SID_LIRE1_DARK_SHOT, },
        NextList 	=	 {},
        CharLv 	=	 48,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon121.dds",
        TitleID 	=	 337,
        DescID 	=	 386,
        ControlDesc	=	208,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--스톰 애로우
	{		
        ID 	=	SID_LIRE1_STORMARROW,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_LIRE1_METEO_STRIKE_LV1, SID_LIRE1_DEMI_SHOCK, SID_LIRE1_SUMMON_ARCHER_LV1, },
        CharLv 	=	 0,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage36.dds",
        TitleID 	=	 338,
        DescID 	=	 387,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	339,
        UI_Pos_y 	=	409,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_LIRE_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--메테오 스트라이크
	{		
        ID 	=	SID_LIRE1_METEO_STRIKE_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 { SID_LIRE1_STORMARROW, },
        NextList 	=	 { SID_LIRE1_STARDUST_RAIN, },
        CharLv 	=	 10,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon119.dds",
        TitleID 	=	 339,
        DescID 	=	 388,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LIRE_METEO_STRIKE_LV1_JUMP,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--스타더스트 레인
	{		
        ID 	=	SID_LIRE1_STARDUST_RAIN,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 { SID_LIRE1_METEO_STRIKE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon123.dds",
        TitleID 	=	 340,
        DescID 	=	 389,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LIRE_STARDUST_RAIN,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--바이올런트 어택 LV1 (엘프궁수대 소환)
	{		
        ID 	=	SID_LIRE1_SUMMON_ARCHER_LV1,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 { SID_LIRE1_STORMARROW, },
        NextList 	=	 { SID_LIRE1_SUMMON_ARCHER_LV2, },
        CharLv 	=	 24,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon104.dds",
        TitleID 	=	 341,
        DescID 	=	 390,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_LIRE1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LIRE_SUMMON_ARCHER_LV1_START,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--바이올런트 어택 LV2 (엘프궁수대 소환)
	{
		ID 	=	SID_LIRE1_SUMMON_ARCHER_LV2,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 { SID_LIRE1_SUMMON_ARCHER_LV1, },
        NextList 	=	 { },
        CharLv 	=	 35,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon103.dds",
        TitleID 	=	 342,
        DescID 	=	 391,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LIRE_SUMMON_ARCHER_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--데미 쇼크 (어나더데이)
	{		
        ID 	=	SID_LIRE1_DEMI_SHOCK,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 { SID_LIRE1_STORMARROW, },
        NextList 	=	 { SID_LIRE1_MAGIC_MISSILIE, },
        CharLv 	=	 10,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage142.dds",
        TitleID 	=	 343,
        DescID 	=	 392,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_LIRE1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LIRE_DEMI_SHOCK_START,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--매직 미사일
	{		
        ID 	=	SID_LIRE1_MAGIC_MISSILIE,
        GroupID 	=	 SG_LIRE1_SPECIAL3,
        PreList 	=	 { SID_LIRE1_DEMI_SHOCK, },
        NextList 	=	 {},
        CharLv 	=	 44,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon115.dds",
        TitleID 	=	 344,
        DescID 	=	 393,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LIRE1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LIRE_MAGIC_MISSILE,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--윈드 워드
	{		
        ID 	=	SID_LIRE1_WINDWARD,
        GroupID 	=	 SG_LIRE1_CASH_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 4144, 7632, 7650, 10216, 13717, 1987, },
        Base 	=	 FALSE,
		Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage230.dds",
        TitleID 	=	 345,
        DescID 	=	 394,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	693,
        UI_Pos_y 	=	260,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_LIRE_WIND_WARD,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--이글 사이트
	{		
        ID 	=	SID_LIRE1_EAGLESIGHT,
        GroupID 	=	 SG_LIRE1_CASH_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 2161, 4151, 7639, 10221, 13724, },
        Base 	=	 FALSE,
		Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage268.dds",
        TitleID 	=	 346,
        DescID 	=	 395,
        ControlDesc	=	207,
        		
        UI_Pos_x 	=	693,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_LIRE_EAGLE_SITE,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--익스플로전 애로우
	{		
        ID 	=	SID_LIRE1_EXPLSIONARROW,
        GroupID 	=	 SG_LIRE1_CASH_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 1,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 2162, 4152, 7640, 13725, 4152, 7658, },
        Base 	=	 FALSE,
		Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage269.dds",
        TitleID 	=	 347,
        DescID 	=	 396,
        ControlDesc	=	209,
        		
        UI_Pos_x 	=	693,
        UI_Pos_y 	=	490,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_LIRE_EXPLOSION_ARROW,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
}