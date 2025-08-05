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
	-- 로난 기본 마나 4칸 확장
    {		
		ID 			=	SID_RONAN1_HIDDEN_MP4,
		GroupID 	=	SG_RONAN1_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	5,
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

    -- 로난 기본 4단필살기
    {		
		ID 			=	SID_RONAN1_SPECIAL_4,
		GroupID 	=	SG_RONAN1_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	5,
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
			Motion = MID_RONAN1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			ChargingSkillLV = 4,
		},
    },
	--룬 스파이럴 LV1
	{		
        ID 	=	SID_RONAN1_BLACK1_RUNESPIRAL_LV1,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_BLACK1_RUNESPIRAL_LV2,SID_RONAN1_BLACK1_HOMINGBUSTER,SID_RONAN1_BLACK1_BLAZESLASH_LV1, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage752.dds",
        TitleID 	=	 720,
        DescID 	=	 779,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	100,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SWORD_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--블레이즈 슬래쉬 LV1
	{		
        ID 	=	SID_RONAN1_BLACK1_BLAZESLASH_LV1,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL1,
        PreList 	=	 { SID_RONAN1_BLACK1_RUNESPIRAL_LV1, },
        NextList 	=	 { SID_RONAN1_BLACK1_BLAZESLASH_LV2, },
        CharLv 	=	 6,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon269.dds",
        TitleID 	=	 717,
        DescID 	=	 776,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	462,
        UI_Pos_y 	=	228,
        UI_LineType 	=	 7,
        
        LockGroup = BGID_RONAN1_BLACK_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL1_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--블레이즈 슬래쉬 LV2
	{		
        ID 	=	SID_RONAN1_BLACK1_BLAZESLASH_LV2,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL1,
        PreList 	=	 { SID_RONAN1_BLACK1_BLAZESLASH_LV1, },
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon270.dds",
        TitleID 	=	 718,
        DescID 	=	 777,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	524,
        UI_Pos_y 	=	228,
        UI_LineType 	=	 1,
        
        LockGroup = BGID_RONAN1_BLACK_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL1_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--호밍 버스터
	{		
        ID 	=	SID_RONAN1_BLACK1_HOMINGBUSTER,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL1,
        PreList 	=	 { SID_RONAN1_BLACK1_RUNESPIRAL_LV1, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon271.dds",
        TitleID 	=	 719,
        DescID 	=	 778,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	462,
        UI_Pos_y 	=	161,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_RONAN1_BLACK_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL1_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--룬 스파이럴 LV2
	{		
        ID 	=	SID_RONAN1_BLACK1_RUNESPIRAL_LV2,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL1,
        PreList 	=	 { SID_RONAN1_BLACK1_RUNESPIRAL_LV1, },
        NextList 	=	 { SID_RONAN1_BLACK1_RUNESPIRAL_LV3, },
        CharLv 	=	 19,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon273.dds",
        TitleID 	=	 721,
        DescID 	=	 780,
        ControlDesc	=	125,
        	
        UI_Pos_x 	=	313,
        UI_Pos_y 	=	191,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SWORD_SPECIAL1_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--룬 스파이럴 LV3
	{		
        ID 	=	SID_RONAN1_BLACK1_RUNESPIRAL_LV3,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL1,
        PreList 	=	 { SID_RONAN1_BLACK1_RUNESPIRAL_LV2, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon274.dds",
        TitleID 	=	 722,
        DescID 	=	 781,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	250,
        UI_Pos_y 	=	191,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SWORD_SPECIAL1_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--스피닝 서클 LV1
	{		
        ID 	=	SID_RONAN1_BLACK2_SPINNINGCIRCLE_LV1,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL2,
        PreList 	=	 { SID_RONAN1_BLACK2_GIGACRASH_LV1, },
        NextList 	=	 { SID_RONAN1_BLACK2_SPINNINGCIRCLE_LV2, },
        CharLv 	=	 8,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon275.dds",
        TitleID 	=	 723,
        DescID 	=	 782,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	462,
        UI_Pos_y 	=	365,
        UI_LineType 	=	 7,
        
        LockGroup = BGID_RONAN1_BLACK_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL2_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--스피닝 서클 LV2
	{		
        ID 	=	SID_RONAN1_BLACK2_SPINNINGCIRCLE_LV2,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL2,
        PreList 	=	 { SID_RONAN1_BLACK2_SPINNINGCIRCLE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 45,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon276.dds",
        TitleID 	=	 724,
        DescID 	=	 783,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	524,
        UI_Pos_y 	=	365,
        UI_LineType 	=	 1,
                
        LockGroup = BGID_RONAN1_BLACK_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL2_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--레이 프리즈
	{		
        ID 	=	SID_RONAN1_BLACK2_RAYFREEZE,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL2,
        PreList 	=	 { SID_RONAN1_BLACK2_GIGACRASH_LV1, },
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon277.dds",
        TitleID 	=	 725,
        DescID 	=	 784,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	462,
        UI_Pos_y 	=	298,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_RONAN1_BLACK_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL2_A,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--기가 크래쉬 LV1
	{		
        ID 	=	SID_RONAN1_BLACK2_GIGACRASH_LV1,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_BLACK2_RAYFREEZE,SID_RONAN1_BLACK2_SPINNINGCIRCLE_LV1,SID_RONAN1_BLACK2_GIGACRASH_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage753.dds",
        TitleID 	=	 726,
        DescID 	=	 785,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SWORD_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--기가 크래쉬 LV2
	{		
        ID 	=	SID_RONAN1_BLACK2_GIGACRASH_LV2,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL2,
        PreList 	=	 { SID_RONAN1_BLACK2_GIGACRASH_LV1, },
        NextList 	=	 { SID_RONAN1_BLACK2_GIGACRASH_LV3, },
        CharLv 	=	 21,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon279.dds",
        TitleID 	=	 727,
        DescID 	=	 786,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	313,
        UI_Pos_y 	=	331,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SWORD_SPECIAL2_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--기가 크래쉬 LV3
	{		
        ID 	=	SID_RONAN1_BLACK2_GIGACRASH_LV3,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL2,
        PreList 	=	 { SID_RONAN1_BLACK2_GIGACRASH_LV2, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon280.dds",
        TitleID 	=	 728,
        DescID 	=	 787,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	250,
        UI_Pos_y 	=	331,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SWORD_SPECIAL2_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--아스트랄 바인
	{		
        ID 	=	SID_RONAN1_BLACK3_ASTRALVINE,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL3,
        PreList 	=	 { SID_RONAN1_BLACK3_SHADOWSLICER, },
        NextList 	=	 {},
        CharLv 	=	 49,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon281.dds",
        TitleID 	=	 729,
        DescID 	=	 788,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	524,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 1,
        LockGroup = BGID_RONAN1_BLACK_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SWORD_SPECIAL3_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--호버링 크러셔
	{		
        ID 	=	SID_RONAN1_BLACK3_HOVERINGCRUSHER,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL3,
        PreList 	=	 { SID_RONAN1_BLACK3_CANAVANSTRIKE, },
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon282.dds",
        TitleID 	=	 730,
        DescID 	=	 789,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	462,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_RONAN1_BLACK_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SWORD_SPECIAL3_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--섀도우 슬라이서
	{		
        ID 	=	SID_RONAN1_BLACK3_SHADOWSLICER,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL3,
        PreList 	=	 { SID_RONAN1_BLACK3_CANAVANSTRIKE, },
        NextList 	=	 { SID_RONAN1_BLACK3_ASTRALVINE, },
        CharLv 	=	 10,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon283.dds",
        TitleID 	=	 731,
        DescID 	=	 790,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	462,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 7,
        LockGroup = BGID_RONAN1_BLACK_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL3_A,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--카나반 스트라이크
	{		
        ID 	=	SID_RONAN1_BLACK3_CANAVANSTRIKE,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_BLACK3_HOVERINGCRUSHER,SID_RONAN1_BLACK3_SHADOWSLICER,SID_RONAN1_BLACK3_FANTACLESTRIKE_LV1, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage754.dds",
        TitleID 	=	 732,
        DescID 	=	 791,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SWORD_SPECIAL3_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--펜타클 스트라이크 LV1
	{		
        ID 	=	SID_RONAN1_BLACK3_FANTACLESTRIKE_LV1,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL3,
        PreList 	=	 { SID_RONAN1_BLACK3_CANAVANSTRIKE, },
        NextList 	=	 { SID_RONAN1_BLACK3_FANTACLESTRIKE_LV2, },
        CharLv 	=	 23,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon285.dds",
        TitleID 	=	 733,
        DescID 	=	 792,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	313,
        UI_Pos_y 	=	475,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL3_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--펜타클 스트라이크 LV2
	{		
        ID 	=	SID_RONAN1_BLACK3_FANTACLESTRIKE_LV2,
        GroupID 	=	 SG_RONAN1_BLACK_SPECIAL3,
        PreList 	=	 { SID_RONAN1_BLACK3_FANTACLESTRIKE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon286.dds",
        TitleID 	=	 734,
        DescID 	=	 793,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	250,
        UI_Pos_y 	=	475,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SKILL_SWORD_SPECIAL3_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--다크 미스트
	{		
        ID 	=	SID_RONAN1_WHITE1_DARKMIST,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL1,
        PreList 	=	 { SID_RONAN1_WHITE1_HOLYBLESS_LV1, },
        NextList 	=	 { SID_RONAN1_WHITE1_PARALYSE, },
        CharLv 	=	 13,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon287.dds",
        TitleID 	=	 735,
        DescID 	=	 794,
        ControlDesc	=	125,
        		
        
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 5,
        LockGroup = BGID_RONAN1_WHITE_SPECIAL1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL1_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--패럴라이즈
	{		
        ID 	=	SID_RONAN1_WHITE1_PARALYSE,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL1,
        PreList 	=	 { SID_RONAN1_WHITE1_DARKMIST, },
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon288.dds",
        TitleID 	=	 736,
        DescID 	=	 795,
        ControlDesc	=	125,
        			
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	227,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_RONAN1_WHITE_SPECIAL1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL1_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--윈드 피스트
	{		
        ID 	=	SID_RONAN1_WHITE1_WINDFIST,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL1,
        PreList 	=	 { SID_RONAN1_WHITE1_HOLYBLESS_LV1, },
        NextList 	=	 {},
        CharLv 	=	 43,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon289.dds",
        TitleID 	=	 737,
        DescID 	=	 796,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	161,
        UI_LineType 	=	 4,
        LockGroup = BGID_RONAN1_WHITE_SPECIAL1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL1_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--홀리 블레스 LV1
	{		
        ID 	=	SID_RONAN1_WHITE1_HOLYBLESS_LV1,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_WHITE1_WINDFIST,SID_RONAN1_WHITE1_DARKMIST,SID_RONAN1_WHITE1_HOLYBLESS_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage755.dds",
        TitleID 	=	 738,
        DescID 	=	 797,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_MAGIC_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--홀리 블레스 LV2
	{		
        ID 	=	SID_RONAN1_WHITE1_HOLYBLESS_LV2,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL1,
        PreList 	=	 { SID_RONAN1_WHITE1_HOLYBLESS_LV1, },
        NextList 	=	 { SID_RONAN1_WHITE1_HOLYBLESS_LV3, },
        CharLv 	=	 12,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon291.dds",
        TitleID 	=	 739,
        DescID 	=	 798,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	193,
        UI_LineType 	=	 1,
        
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_MAGIC_SPECIAL1_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--홀리 블레스 LV3
	{		
        ID 	=	SID_RONAN1_WHITE1_HOLYBLESS_LV3,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL1,
        PreList 	=	 { SID_RONAN1_WHITE1_HOLYBLESS_LV2, },
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon292.dds",
        TitleID 	=	 740,
        DescID 	=	 799,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	516,
        UI_Pos_y 	=	193,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RONAN_MAGIC_SPECIAL1_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
		--템페스트 배리어
	{		
        ID 	=	SID_RONAN1_WHITE2_TEMPESTBARRIER,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL2,
        PreList 	=	 { SID_RONAN1_WHITE2_BLASTBOMB_LV1, },
        NextList 	=	 { SID_RONAN1_WHITE2_WINDYSHIELD, },
        CharLv 	=	 15,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon293.dds",
        TitleID 	=	 741,
        DescID 	=	 800,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_RONAN1_WHITE_SPECIAL2,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL2_A,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--윈디 쉴드
	{		
        ID 	=	SID_RONAN1_WHITE2_WINDYSHIELD,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL2,
        PreList 	=	 { SID_RONAN1_WHITE2_TEMPESTBARRIER, },
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon294.dds",
        TitleID 	=	 742,
        DescID 	=	 801,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	366,
        UI_LineType 	=	 0,
        LockGroup = BGID_RONAN1_WHITE_SPECIAL2,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL2_B,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--에너지 서클
	{		
        ID 	=	SID_RONAN1_WHITE2_ENERGYCIRCLE,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL2,
        PreList 	=	 { SID_RONAN1_WHITE2_BLASTBOMB_LV1, },
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon295.dds",
        TitleID 	=	 743,
        DescID 	=	 802,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	300,
        UI_LineType 	=	 4,
        
        
        LockGroup = BGID_RONAN1_WHITE_SPECIAL2,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL2_C,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--블라스트 봄 LV1
	{		
        ID 	=	SID_RONAN1_WHITE2_BLASTBOMB_LV1,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_WHITE2_BLASTBOMB_LV2,SID_RONAN1_WHITE2_TEMPESTBARRIER,SID_RONAN1_WHITE2_ENERGYCIRCLE },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage756.dds",
        TitleID 	=	 744,
        DescID 	=	 803,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_MAGIC_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--블라스트 봄 LV2
	{		
        ID 	=	SID_RONAN1_WHITE2_BLASTBOMB_LV2,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL2,
        PreList 	=	 { SID_RONAN1_WHITE2_BLASTBOMB_LV1, },
        NextList 	=	 { SID_RONAN1_WHITE2_BLASTBOMB_LV3, },
        CharLv 	=	 14,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon297.dds",
        TitleID 	=	 745,
        DescID 	=	 804,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_MAGIC_SPECIAL2_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--블라스트 봄 LV3
	{		
        ID 	=	SID_RONAN1_WHITE2_BLASTBOMB_LV3,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL2,
        PreList 	=	 { SID_RONAN1_WHITE2_BLASTBOMB_LV2, },
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon298.dds",
        TitleID 	=	 746,
        DescID 	=	 805,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	516,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RONAN_MAGIC_SPECIAL2_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
		--브람 가슈
	{		
        ID 	=	SID_RONAN1_WHITE3_BRAMGUSH,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL3,
        PreList 	=	 { SID_RONAN1_WHITE3_LUNATICFORCE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 17,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon299.dds",
        TitleID 	=	 747,
        DescID 	=	 806,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	302,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 4,
        
        
        LockGroup = BGID_RONAN1_WHITE_SPECIAL3,
        
        SpecialTypeB = TRUE,
        
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL3_LV1,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--일루미네이션 소드
	{		
        ID 	=	SID_RONAN1_WHITE3_ILLUMINATIONSWORD,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL3,
        PreList 	=	 { SID_RONAN1_WHITE3_FLAMETHROW, },
        NextList 	=	 {},
        CharLv 	=	 50,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon300.dds",
        TitleID 	=	 748,
        DescID 	=	 807,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	239,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        
        LockGroup = BGID_RONAN1_WHITE_SPECIAL3,
        
        SpecialTypeB = TRUE,
        
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL3_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--플레임 드로우
	{		
        ID 	=	SID_RONAN1_WHITE3_FLAMETHROW,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL3,
        PreList 	=	 { SID_RONAN1_WHITE3_LUNATICFORCE_LV1, },
        NextList 	=	 { SID_RONAN1_WHITE3_ILLUMINATIONSWORD, },
        CharLv 	=	 30,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon301.dds",
        TitleID 	=	 749,
        DescID 	=	 808,
        ControlDesc	=	127,
        
		UI_Pos_x 	=	302,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,		
        
        
        LockGroup = BGID_RONAN1_WHITE_SPECIAL3,
        
        SpecialTypeB = TRUE,
        
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_SKILL_MAGIC_SPECIAL3_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--루나틱 포스 LV1
	{		
        ID 	=	SID_RONAN1_WHITE3_LUNATICFORCE_LV1,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_WHITE3_BRAMGUSH,SID_RONAN1_WHITE3_FLAMETHROW,SID_RONAN1_WHITE3_LUNATICFORCE_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage757.dds",
        TitleID 	=	 750,
        DescID 	=	 809,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_MAGIC_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--루나틱 포스 LV2
	{		
        ID 	=	SID_RONAN1_WHITE3_LUNATICFORCE_LV2,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL3,
        PreList 	=	 { SID_RONAN1_WHITE3_LUNATICFORCE_LV1, },
        NextList 	=	 { SID_RONAN1_WHITE3_LUNATICFORCE_LV3, },
        CharLv 	=	 16,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon303.dds",
        TitleID 	=	 751,
        DescID 	=	 810,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_MAGIC_SPECIAL3_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--루나틱 포스 LV3
	{		
        ID 	=	SID_RONAN1_WHITE3_LUNATICFORCE_LV3,
        GroupID 	=	 SG_RONAN1_WHITE_SPECIAL3,
        PreList 	=	 { SID_RONAN1_WHITE3_LUNATICFORCE_LV2, },
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon304.dds",
        TitleID 	=	 752,
        DescID 	=	 811,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	516,
        UI_Pos_y 	=	473,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RONAN_MAGIC_SPECIAL3_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
		--마검의 눈 LV1
	{		
        ID 	=	SID_RONAN1_SWORD_EYE_LV1,
        GroupID 	=	 SG_RONAN1_COMMON_UNIQUE_SKILL,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_SWORD_EYE_LV2, },
        CharLv 	=	 5,
        CharType 	=	 5,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon305.dds",
        TitleID 	=	 753,
        DescID 	=	 812,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	305,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--마검의 눈 LV2
	{		
        ID 	=	SID_RONAN1_SWORD_EYE_LV2,
        GroupID 	=	 SG_RONAN1_COMMON_UNIQUE_SKILL,
        PreList 	=	 { SID_RONAN1_SWORD_EYE_LV1, },
        NextList 	=	 { SID_RONAN1_SWORD_EYE_LV3, },
        CharLv 	=	 20,
        CharType 	=	 5,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon306.dds",
        TitleID 	=	 754,
        DescID 	=	 813,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	536,
        UI_Pos_y 	=	305,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--마검의 눈 LV3
	{		
        ID 	=	SID_RONAN1_SWORD_EYE_LV3,
        GroupID 	=	 SG_RONAN1_COMMON_UNIQUE_SKILL,
        PreList 	=	 { SID_RONAN1_SWORD_EYE_LV2, },
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 5,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "SkillIcon307.dds",
        TitleID 	=	 755,
        DescID 	=	 814,
        ControlDesc	=	214,
        
        UI_Pos_x 	=	599,
        UI_Pos_y 	=	305,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--띄우기 LV1
	{		
        ID 	=	SID_RONAN1_COMBO_FLYING_LV1,
        GroupID 	=	 SG_RONAN1_COMBO_PLUS,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_COMBO_FLYING_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "SkillIcon308.dds",
        TitleID 	=	 756,
        DescID 	=	 815,
        ControlDesc	=	689,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--띄우기 LV2
	{		
        ID 	=	SID_RONAN1_COMBO_FLYING_LV2,
        GroupID 	=	 SG_RONAN1_COMBO_PLUS,
        PreList 	=	 { SID_RONAN1_COMBO_FLYING_LV1, },
        NextList 	=	 {},
        CharLv 	=	 18,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon309.dds",
        TitleID 	=	 757,
        DescID 	=	 816,
        ControlDesc	=	770,
        		
        UI_Pos_x 	=	328,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
		--라그나볼트 LV1
	{		
        ID 	=	SID_RONAN1_RAGNABOLT_LV1,
        GroupID 	=	 SG_RONAN1_MAGIC_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_RAGNABOLT_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "SkillIcon310.dds",
        TitleID 	=	 758,
        DescID 	=	 817,
        ControlDesc	=	771,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--라그나볼트 LV2
	{		
        ID 	=	SID_RONAN1_RAGNABOLT_LV2,
        GroupID 	=	 SG_RONAN1_MAGIC_ATK,
        PreList 	=	 { SID_RONAN1_RAGNABOLT_LV1, },
        NextList 	=	 {},
        CharLv 	=	 7,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon311.dds",
        TitleID 	=	 759,
        DescID 	=	 818,
        ControlDesc	=	772,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--대쉬공격 LV1
	{		
        ID 	=	SID_RONAN1_DASH_ATK_LV1,
        GroupID 	=	 SG_RONAN1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_DASH_ATK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "SkillIcon312.dds",
        TitleID 	=	 760,
        DescID 	=	 819,
        ControlDesc	=	773,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--대쉬공격 LV2
	{		
        ID 	=	SID_RONAN1_DASH_ATK_LV2,
        GroupID 	=	 SG_RONAN1_DASH_ATK,
        PreList 	=	 { SID_RONAN1_DASH_ATK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 22,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon313.dds",
        TitleID 	=	 761,
        DescID 	=	 820,
        ControlDesc	=	773,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--대쉬 공중 콤보
	{		
        ID 	=	SID_RONAN1_DASH_ATK_PLUS_COMBO,
        GroupID 	=	 SG_RONAN1_DASH_ATK_PLUS,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 9,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillIcon314.dds",
        TitleID 	=	 762,
        DescID 	=	 821,
        ControlDesc	=	774,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        LockGroup = -1,
    },
		--룬 쉴드
	{		
        ID 	=	SID_RONAN1_RUNESHIELD,
        GroupID 	=	 SG_RONAN1_DIFFENCE_MAGIC,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon315.dds",
        TitleID 	=	 763,
        DescID 	=	 822,
        ControlDesc	=	775,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    
    
		--커스 인챈트
	{		
        ID 	=	SID_RONAN1_ENCHANT_CURSE,
        GroupID 	=	 SG_RONAN1_ENCHANT,
        PreList 	=	 { SID_RONAN1_ENCHANT_LIGHTNING,},
        NextList 	=	 { SID_RONAN1_ENCHANT_BLOODY, },
        CharLv 	=	 30,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon316.dds",
        TitleID 	=	 764,
        DescID 	=	 823,
        ControlDesc	=	682,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--라이트닝 인챈트
	{		
        ID 	=	SID_RONAN1_ENCHANT_LIGHTNING,
        GroupID 	=	 SG_RONAN1_ENCHANT,
        PreList 	=	 { SID_RONAN1_ENCHANT_FIRELY, },
        NextList 	=	 { SID_RONAN1_ENCHANT_CURSE, },
        CharLv 	=	 24,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon317.dds",
        TitleID 	=	 765,
        DescID 	=	 824,
        ControlDesc	=	682,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        LockGroup = -1,
    },
		--파이어리 인챈트
	{		
        ID 	=	SID_RONAN1_ENCHANT_FIRELY,
        GroupID 	=	 SG_RONAN1_ENCHANT,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_ENCHANT_LIGHTNING, },
        CharLv 	=	 11,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon318.dds",
        TitleID 	=	 766,
        DescID 	=	 825,
        ControlDesc	=	682,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--블러디 인챈트
	{		
        ID 	=	SID_RONAN1_ENCHANT_BLOODY,
        GroupID 	=	 SG_RONAN1_ENCHANT,
        PreList 	=	 { SID_RONAN1_ENCHANT_CURSE, },
        NextList 	=	 {},
        CharLv 	=	 57,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon319.dds",
        TitleID 	=	 767,
        DescID 	=	 826,
        ControlDesc	=	682,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
		--콤보공격 LV1
	{		
        ID 	=	SID_RONAN1_COMBO_LV1,
        GroupID 	=	 SG_RONAN1_COMBO,
        PreList 	=	 {},
        NextList 	=	 { SID_RONAN1_COMBO_LV2, },
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "SkillIcon320.dds",
        TitleID 	=	 768,
        DescID 	=	 827,
        ControlDesc	=	493,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
		--콤보공격 LV2
	{		
        ID 	=	SID_RONAN1_COMBO_LV2,
        GroupID 	=	 SG_RONAN1_COMBO,
        PreList 	=	 { SID_RONAN1_COMBO_LV1, },
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 5,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillIcon321.dds",
        TitleID 	=	 769,
        DescID 	=	 828,
        ControlDesc	=	687,
        		
        UI_Pos_x 	=	290,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
}