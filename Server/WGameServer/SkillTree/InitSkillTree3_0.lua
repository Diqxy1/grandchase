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
	-- 라스 기본 마나 4칸 확장
    {		
		ID 			=	SID_LAS1_HIDDEN_MP4,
		GroupID 	=	SG_LAS1_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	3,
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

    -- 라스 기본 4단필살기
    {		
		ID 			=	SID_LAS1_SPECIAL_4,
		GroupID 	=	SG_LAS1_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	3,
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
			Motion = MID_LAS1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			ChargingSkillLV = 4,
		},
    },
	--그림자 숨기
	{		
        ID 	=	SID_LAS1_SHADOW,
        GroupID 	=	 SG_LAS1_SHADOW,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 3,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon163.dds",
        TitleID 	=	 435,
        DescID 	=	 516,
        ControlDesc	=	486,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	186,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
		--슈퍼 점프
	{		
        ID 	=	SID_LAS1_SUPER_JUMP,
        GroupID 	=	 SG_LAS1_MOVE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 20,
        CharType 	=	 3,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon168.dds",
        TitleID 	=	 436,
        DescID 	=	 517,
        ControlDesc	=	487,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
		--질주
	{		
        ID 	=	SID_LAS1_SUPER_SPEED,
        GroupID 	=	 SG_LAS1_BUFF,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 3,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon170.dds",
        TitleID 	=	 437,
        DescID 	=	 518,
        ControlDesc	=	488,
        		
        UI_Pos_x 	=	463,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--연속 베기
	{		
        ID 	=	SID_LAS1_RAPID_KNIFE,
        GroupID 	=	 SG_LAS1_SPECIAL_ATK3,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_SHOOTING_SURIKEN, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon182.dds",
        TitleID 	=	 442,
        DescID 	=	 523,
        ControlDesc	=	493,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--표창 난사
	{		
        ID 	=	SID_LAS1_SHOOTING_SURIKEN,
        GroupID 	=	 SG_LAS1_SPECIAL_ATK3,
        PreList 	=	 { SID_LAS1_RAPID_KNIFE, },
        NextList 	=	 { SID_LAS1_DOUBLE_SURIKEN, },
        CharLv 	=	 15,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon177.dds",
        TitleID 	=	 443,
        DescID 	=	 524,
        ControlDesc	=	494,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--표창 던지기 Lv1
	{		
        ID 	=	SID_LAS1_SURIKEN_LV1,
        GroupID 	=	 SG_LAS1_JUMP_ATK1,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_SURIKEN_LV2, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon178.dds",
        TitleID 	=	 444,
        DescID 	=	 525,
        ControlDesc	=	495,
        		
        UI_Pos_x 	=	274,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--표창 던지기 Lv2
	{		
        ID 	=	SID_LAS1_SURIKEN_LV2,
        GroupID 	=	 SG_LAS1_JUMP_ATK1,
        PreList 	=	 { SID_LAS1_SURIKEN_LV1, },
        NextList 	=	 {},
        CharLv 	=	 21,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon179.dds",
        TitleID 	=	 445,
        DescID 	=	 526,
        ControlDesc	=	496,
        		
        UI_Pos_x 	=	274,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--강하
	{		
        ID 	=	SID_LAS1_DROPPING,
        GroupID 	=	 SG_LAS1_JUMP_ATK2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon160.dds",
        TitleID 	=	 446,
        DescID 	=	 527,
        ControlDesc	=	497,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--대쉬어택 LV1
	{		
        ID 	=	SID_LAS1_DASHATK_LV1,
        GroupID 	=	 SG_LAS1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_DASHATK_LV2,},
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon164.dds",
        TitleID 	=	 447,
        DescID 	=	 528,
        ControlDesc	=	498,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--대쉬어택 LV2
	{		
        ID 	=	SID_LAS1_DASHATK_LV2,
        GroupID 	=	 SG_LAS1_DASH_ATK,
        PreList 	=	 { SID_LAS1_DASHATK_LV1, },
        NextList 	=	 { SID_LAS1_DASHATK_LV3, },
        CharLv 	=	 17,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon165.dds",
        TitleID 	=	 448,
        DescID 	=	 529,
        ControlDesc	=	499,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--대쉬어택 LV3
	{		
        ID 	=	SID_LAS1_DASHATK_LV3,
        GroupID 	=	 SG_LAS1_DASH_ATK,
        PreList 	=	 { SID_LAS1_DASHATK_LV2, },
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon166.dds",
        TitleID 	=	 449,
        DescID 	=	 530,
        ControlDesc	=	500,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--콤보 공격 LV1
	{		
        ID 	=	SID_LAS1_COMBO_LV1,
        GroupID 	=	 SG_LAS1_COMBO_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_COMBO_LV2, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon172.dds",
        TitleID 	=	 450,
        DescID 	=	 531,
        ControlDesc	=	501,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	= 2,
        
        LockGroup = -1,
    },
		--콤보 공격 LV2
	{		
        ID 	=	SID_LAS1_COMBO_LV2,
        GroupID 	=	 SG_LAS1_COMBO_ATK,
        PreList 	=	 { SID_LAS1_COMBO_LV1, },
        NextList 	=	 { SID_LAS1_COMBO_LV3, },
        CharLv 	=	 13,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon173.dds",
        TitleID 	=	 451,
        DescID 	=	 532,
        ControlDesc	=	502,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--콤보 공격 LV3
	{		
        ID 	=	SID_LAS1_COMBO_LV3,
        GroupID 	=	 SG_LAS1_COMBO_ATK,
        PreList 	=	 { SID_LAS1_COMBO_LV2, },
        NextList 	=	 {},
        CharLv 	=	 37,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon174.dds",
        TitleID 	=	 452,
        DescID 	=	 533,
        ControlDesc	=	503,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--공중 콤보 LV1
	{		
        ID 	=	SID_LAS1_FLY_COMBO_LV1,
        GroupID 	=	 SG_LAS1_COMBO_UP,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_FLY_COMBO_LV2, },
        CharLv 	=	 14,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon175.dds",
        TitleID 	=	 453,
        DescID 	=	 534,
        ControlDesc	=	504,
        		
        UI_Pos_x 	=	158,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--공중 콤보 LV2
	{		
        ID 	=	SID_LAS1_FLY_COMBO_LV2,
        GroupID 	=	 SG_LAS1_COMBO_UP,
        PreList 	=	 { SID_LAS1_FLY_COMBO_LV1, },
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon176.dds",
        TitleID 	=	 454,
        DescID 	=	 535,
        ControlDesc	=	505,
        		
        UI_Pos_x 	=	158,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--회피 콤보 LV1
	{		
        ID 	=	SID_LAS1_DODGE_COMBO_LV1,
        GroupID 	=	 SG_LAS1_COMBO_DODGE,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_DODGE_COMBO_LV2, },
        CharLv 	=	 5,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon180.dds",
        TitleID 	=	 455,
        DescID 	=	 536,
        ControlDesc	=	506,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--회피 콤보 LV2
	{		
        ID 	=	SID_LAS1_DODGE_COMBO_LV2,
        GroupID 	=	 SG_LAS1_COMBO_DODGE,
        PreList 	=	 { SID_LAS1_DODGE_COMBO_LV1, },
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon181.dds",
        TitleID 	=	 456,
        DescID 	=	 537,
        ControlDesc	=	507,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--크리티컬 LV1
	{		
        ID 	=	SID_LAS1_CRITICAL_LV1,
        GroupID 	=	 SG_LAS1_CRITICAL,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_CRITICAL_LV2, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 1,
		
        Img 	=	 "skillicon161.dds",
        TitleID 	=	 457,
        DescID 	=	 538,
        ControlDesc	=	508,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--크리티컬 LV2
	{		
        ID 	=	SID_LAS1_CRITICAL_LV2,
        GroupID 	=	 SG_LAS1_CRITICAL,
        PreList 	=	 { SID_LAS1_CRITICAL_LV1, },
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon162.dds",
        TitleID 	=	 458,
        DescID 	=	 539,
        ControlDesc	=	509,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--스파이크 트랩
	{		
        ID 	=	SID_LAS1_SPIKE_TRAP,
        GroupID 	=	 SG_LAS1_TRAP,
        PreList 	=	 { SID_LAS1_CANON_TRAP, },
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon169.dds",
        TitleID 	=	 459,
        DescID 	=	 540,
        ControlDesc	=	510,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--캐논 트랩
	{		
        ID 	=	SID_LAS1_CANON_TRAP,
        GroupID 	=	 SG_LAS1_TRAP,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_SPIKE_TRAP, },
        CharLv 	=	 28,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon171.dds",
        TitleID 	=	 460,
        DescID 	=	 541,
        ControlDesc	=	511,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
    },
		--임팩트 슬래쉬 LV1
	{		
        ID 	=	SID_LAS1_IMPACT_SLASH_LV1,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_IMPACT_SLASH_LV2, SID_LAS1_SPINNING_SLASH_LV1, SID_LAS1_AST_HAMMER, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage209.dds",
        TitleID 	=	 462,
        DescID 	=	 543,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_LAS_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--임팩트 슬래쉬 LV2
	{		
        ID 	=	SID_LAS1_IMPACT_SLASH_LV2,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 { SID_LAS1_IMPACT_SLASH_LV1, },
        NextList 	=	 { SID_LAS1_IMPACT_SLASH_LV3, },
        CharLv 	=	 10,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon155.dds",
        TitleID 	=	 463,
        DescID 	=	 544,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LAS_IMPACTSLASH_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--임팩트 슬래쉬 LV3
	{		
        ID 	=	SID_LAS1_IMPACT_SLASH_LV3,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 { SID_LAS1_IMPACT_SLASH_LV2, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon156.dds",
        TitleID 	=	 464,
        DescID 	=	 545,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LAS_IMPACTSLASH_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--스피닝 슬래쉬 LV1
	{		
        ID 	=	SID_LAS1_SPINNING_SLASH_LV1,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 { SID_LAS1_IMPACT_SLASH_LV1,},
        NextList 	=	 { SID_LAS1_SPINNING_SLASH_LV2, },
        CharLv 	=	 6,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon148.dds",
        TitleID 	=	 465,
        DescID 	=	 546,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_LAS1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LAS_SPININGSLASH_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--스피닝 슬래쉬 LV2
	{		
        ID 	=	SID_LAS1_SPINNING_SLASH_LV2,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 { SID_LAS1_SPINNING_SLASH_LV1, },
        NextList 	=	 {},
        CharLv 	=	 19,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon149.dds",
        TitleID 	=	 466,
        DescID 	=	 547,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LAS1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LAS_SPININGSLASH_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--어스트 해머
	{		
        ID 	=	SID_LAS1_AST_HAMMER,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 { SID_LAS1_IMPACT_SLASH_LV1, },
        NextList 	=	 { SID_LAS1_INDURE, },
        CharLv 	=	 31,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon153.dds",
        TitleID 	=	 468,
        DescID 	=	 549,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_LAS1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LAS_AST_HAMMER_START,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--카게무샤
	{		
        ID 	=	SID_LAS1_INDURE,
        GroupID 	=	 SG_LAS1_SPECIAL1,
        PreList 	=	 { SID_LAS1_AST_HAMMER, },
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon154.dds",
        TitleID 	=	 467,
        DescID 	=	 548,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LAS1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_LAS_CLONE_ATK,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--페이탈 퓨리
	{		
        ID 	=	SID_LAS1_FATAL_FURY,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_TWOHAND_QUICKEN, SID_LAS1_SHOADOW_STRANGER_LV1, SID_LAS1_TRAD_WARM_LV1, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage210.dds",
        TitleID 	=	 469,
        DescID 	=	 550,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	409,
        UI_Pos_y 	=	339,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_LAS_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--무기 훔치기
	{		
        ID 	=	SID_LAS1_TWOHAND_QUICKEN,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 { SID_LAS1_FATAL_FURY, },
        NextList 	=	 { SID_LAS1_HORNET_STING, },
        CharLv 	=	 15,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage1758.dds",
        TitleID 	=	 470,
        DescID 	=	 551,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LAS_TWOHAND_QUICKEN,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--호넷 스팅
	{		
        ID 	=	SID_LAS1_HORNET_STING,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 { SID_LAS1_TWOHAND_QUICKEN, },
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon159.dds",
        TitleID 	=	 471,
        DescID 	=	 552,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LAS_HONET_STING_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--섀도우 스트레인저 LV1
	{		
        ID 	=	SID_LAS1_SHOADOW_STRANGER_LV1,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 { SID_LAS1_FATAL_FURY, },
        NextList 	=	 { SID_LAS1_SHOADOW_STRANGER_LV2, },
        CharLv 	=	 8,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon146.dds",
        TitleID 	=	 472,
        DescID 	=	 553,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_LAS1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LAS_SPEEDY_MOVE_ATK,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--섀도우 스트레인저 LV2
	{		
        ID 	=	SID_LAS1_SHOADOW_STRANGER_LV2,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 { SID_LAS1_SHOADOW_STRANGER_LV1, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon147.dds",
        TitleID 	=	 473,
        DescID 	=	 554,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LAS1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LAS_SPEEDY_MOVE_ATK_LV2_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--트래드 웜 LV1
	{		
        ID 	=	SID_LAS1_TRAD_WARM_LV1,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 { SID_LAS1_FATAL_FURY, },
        NextList 	=	 { SID_LAS1_TRAD_WARM_LV2, },
        CharLv 	=	 22,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon157.dds",
        TitleID 	=	 474,
        DescID 	=	 555,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_LAS1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LAS_TRADWARM_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--트래드 웜 LV2
	{		
        ID 	=	SID_LAS1_TRAD_WARM_LV2,
        GroupID 	=	 SG_LAS1_SPECIAL2,
        PreList 	=	 { SID_LAS1_TRAD_WARM_LV1, },
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon158.dds",
        TitleID 	=	 475,
        DescID 	=	 556,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_LAS1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_LAS_TRADWARM_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    
	--파이날 스트라이크
	{		
        ID 	=	SID_LAS1_FINAL_STRIKE,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_LAS1_THUNDER_SLASH, SID_LAS1_BLIND_SLASH, SID_LAS1_SUMMON_KAZEAZE_LV1, },
        CharLv 	=	 0,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage211.dds",
        TitleID 	=	 476,
        DescID 	=	 557,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	693,
        UI_Pos_y 	=	490,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_LAS_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--썬더 슬래쉬
	{		
        ID 	=	SID_LAS1_THUNDER_SLASH,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 { SID_LAS1_FINAL_STRIKE, },
        NextList 	=	 { SID_LAS1_RAVEN_WALKER, },
        CharLv 	=	 20,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon150.dds",
        TitleID 	=	 477,
        DescID 	=	 558,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LAS_THUNDER_SLASH_START,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--레이븐 워커
	{		
        ID 	=	SID_LAS1_RAVEN_WALKER,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 { SID_LAS1_THUNDER_SLASH, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon144.dds",
        TitleID 	=	 478,
        DescID 	=	 559,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LAS_RAVEN_WALKER_START,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--귀폭수리검
	{		
        ID 	=	SID_LAS1_BLIND_SLASH,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 { SID_LAS1_FINAL_STRIKE, },
        NextList 	=	 { SID_LAS1_VENOM_SLICE, },
        CharLv 	=	 10,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon143.dds",
        TitleID 	=	 479,
        DescID 	=	 560,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 4,
        
		LockGroup = BGID_LAS1_SPECIAL3,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LAS_BOMB_DART_START,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--베놈 슬라이스
	{		
        ID 	=	SID_LAS1_VENOM_SLICE,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 { SID_LAS1_BLIND_SLASH, },
        NextList 	=	 {},
        CharLv 	=	 10,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon145.dds",
        TitleID 	=	 480,
        DescID 	=	 561,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 0,
        
		LockGroup = BGID_LAS1_SPECIAL3,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LAS_VENOM_SLICE_START,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--어두운 기억 LV1
	{		
        ID 	=	SID_LAS1_SUMMON_KAZEAZE_LV1,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 { SID_LAS1_FINAL_STRIKE, },
        NextList 	=	 { SID_LAS1_SUMMON_KAZEAZE_LV2, },
        CharLv 	=	 35,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon151.dds",
        TitleID 	=	 481,
        DescID 	=	 562,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,
        
		LockGroup = BGID_LAS1_SPECIAL3,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LAS_SUMMON_KAZEAZE_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--어두운 기억 LV2
	{		
        ID 	=	SID_LAS1_SUMMON_KAZEAZE_LV2,
        GroupID 	=	 SG_LAS1_SPECIAL3,
        PreList 	=	 { SID_LAS1_SUMMON_KAZEAZE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 42,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon152.dds",
        TitleID 	=	 482,
        DescID 	=	 563,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
		LockGroup = BGID_LAS1_SPECIAL3,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_LAS_SUMMON_KAZEAZE_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--더블 슬래쉬(신필 1단)
	{		
        ID 	=	SID_LAS1_DOUBLE_SLASH,
        GroupID 	=	 SG_LAS1_CASH_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 2418, 4155, 7646, 7664, 13731, },
        Base 	=	FALSE,
		Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage276.dds",
        TitleID 	=	 483,
        DescID 	=	 564,
        ControlDesc	=	513,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	186,
        UI_LineType 	=	 0,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_LAS_DOUBLE_SLASH,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--블러드 배쉬(신필 2단)
	{		
        ID 	=	SID_LAS1_BLOOD_BASH,
        GroupID 	=	 SG_LAS1_CASH_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 2419, 4156, 7647, 7665, 13732, },
        Base 	=	FALSE,
		Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage277.dds",
        TitleID 	=	 484,
        DescID 	=	 565,
        ControlDesc	=	514,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_LAS_BLOOD_BASH,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--가디언 스트라이크(신필 3단)
	{		
        ID 	=	SID_LAS1_GUARDIAN_STRIKE,
        GroupID 	=	 SG_LAS1_CASH_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 2420, 4157, 7648, 7666, 13733, },
        Base 	=	FALSE,
		Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage278.dds",
        TitleID 	=	 485,
        DescID 	=	 566,
        ControlDesc	=	515,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	472,
        UI_LineType 	=	 0,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_LAS_GUARDIAN_BALL,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    --더블 수리켄
	{		
        ID 	=	SID_LAS1_DOUBLE_SURIKEN,
        GroupID 	=	 SG_LAS1_SPECIAL_ATK3,
        PreList 	=	 { SID_LAS1_SHOOTING_SURIKEN, },
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon167.dds",
        TitleID 	=	 440,
        DescID 	=	 521,
        ControlDesc	=	491,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    --마비 표창
	{		
        ID 	=	SID_LAS1_STUN_SURIKEN,
        GroupID 	=	 SG_LAS1_SPECIAL_ATK2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 3,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon183.dds",
        TitleID 	=	 441,
        DescID 	=	 522,
        ControlDesc	=	492,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
}