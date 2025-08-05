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
	--늑대 변신 지속
	{
        ID 	=	SID_RYAN1_WOLF_TIME_EXTENSION,
        GroupID 	=	 SG_RYAN1_COMMON_CHANGE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage430.dds",
        TitleID 	=	 571,
        DescID 	=	 618,
        ControlDesc	=	665,
        		
        UI_Pos_x 	=	329,
        UI_Pos_y 	=	175,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--챠징 엑스
	{
        ID 	=	SID_RYAN1_CHARGING_ATK,
        GroupID 	=	 SG_RYAN1_COMMON_CHARGE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 20,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon189.dds",
        TitleID 	=	 572,
        DescID 	=	 619,
        ControlDesc	=	666,
        		
        UI_Pos_x 	=	329,
        UI_Pos_y 	=	375,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--끈질긴 생명력 Lv1
	{
        ID 	=	SID_RYAN1_STRONGLY_LIFE_FORCE_LV1,
        GroupID 	=	 SG_RYAN1_COMMON_RESERECTION,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_STRONGLY_LIFE_FORCE_LV2, },
        CharLv 	=	 30,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon184.dds",
        TitleID 	=	 573,
        DescID 	=	 620,
        ControlDesc	=	667,
        		
        UI_Pos_x 	=	463,
        UI_Pos_y 	=	275,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--끈질긴 생명력 Lv2
	{
        ID 	=	SID_RYAN1_STRONGLY_LIFE_FORCE_LV2,
        GroupID 	=	 SG_RYAN1_COMMON_RESERECTION,
        PreList 	=	 { SID_RYAN1_STRONGLY_LIFE_FORCE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon185.dds",
        TitleID 	=	 574,
        DescID 	=	 621,
        ControlDesc	=	668,
        		
        UI_Pos_x 	=	526,
        UI_Pos_y 	=	275,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--야성의 포효
	{
        ID 	=	SID_RYAN1_WILD_HOWLING,
        GroupID 	=	 SG_RYAN1_COMMON_HOWLING,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon187.dds",
        TitleID 	=	 575,
        DescID 	=	 622,
        ControlDesc	=	669,
        		
        UI_Pos_x 	=	463,
        UI_Pos_y 	=	475,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	--도끼 내려찍기 Lv1
	{
        ID 	=	SID_RYAN1_DOWNWARD_CHOP_LV1,
        GroupID 	=	 SG_RYAN1_JUMP_ATK_1,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_DOWNWARD_CHOP_LV2, },
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon200.dds",
        TitleID 	=	 576,
        DescID 	=	 623,
        ControlDesc	=	670,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--도끼 내려찍기 Lv2
	{
        ID 	=	SID_RYAN1_DOWNWARD_CHOP_LV2,
        GroupID 	=	 SG_RYAN1_JUMP_ATK_1,
        PreList 	=	 { SID_RYAN1_DOWNWARD_CHOP_LV1, },
        NextList 	=	 { SID_RYAN1_DOWNWARD_CHOP_LV3, },
        CharLv 	=	 14,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon201.dds",
        TitleID 	=	 577,
        DescID 	=	 624,
        ControlDesc	=	671,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--도끼 내려찍기 Lv3
	{
        ID 	=	SID_RYAN1_DOWNWARD_CHOP_LV3,
        GroupID 	=	 SG_RYAN1_JUMP_ATK_1,
        PreList 	=	 { SID_RYAN1_DOWNWARD_CHOP_LV2, },
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon202.dds",
        TitleID 	=	 578,
        DescID 	=	 625,
        ControlDesc	=	672,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--점프 공격 Lv1
	{
        ID 	=	SID_RYAN1_JUMP_ATTACK_LV1,
        GroupID 	=	 SG_RYAN1_JUMP_ATK_2,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_JUMP_ATTACK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon195.dds",
        TitleID 	=	 579,
        DescID 	=	 626,
        ControlDesc	=	673,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--점프 공격 Lv2
	{
        ID 	=	SID_RYAN1_JUMP_ATTACK_LV2,
        GroupID 	=	 SG_RYAN1_JUMP_ATK_2,
        PreList 	=	 { SID_RYAN1_JUMP_ATTACK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon196.dds",
        TitleID 	=	 580,
        DescID 	=	 627,
        ControlDesc	=	674,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--숏 대쉬
	{
        ID 	=	SID_RYAN1_JUMP_DASH,
        GroupID 	=	 SG_RYAN1_MOVE,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_LEOPARD_IMAGE, },
        CharLv 	=	 5,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon203.dds",
        TitleID 	=	 581,
        DescID 	=	 628,
        ControlDesc	=	675,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--표범의 질주
	{
        ID 	=	SID_RYAN1_LEOPARD_IMAGE,
        GroupID 	=	 SG_RYAN1_MOVE,
        PreList 	=	 { SID_RYAN1_JUMP_DASH, },
        NextList 	=	 {},
        CharLv 	=	 18,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon190.dds",
        TitleID 	=	 582,
        DescID 	=	 629,
        ControlDesc	=	676,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    
--[[
		--대쉬 캔슬
	{
        ID 	=	SID_RYAN1_DASHATK_CANCEL,
        GroupID 	=	 SG_RYAN1_MOVE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 0,
		
        Img 	=	 "skillicon183.dds",
        TitleID 	=	 583,
        DescID 	=	 630,
        ControlDesc	=	677,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    }, ]]
	--대쉬 공격 Lv1
	{
        ID 	=	SID_RYAN1_DASH_ATK_LV1,
        GroupID 	=	 SG_RYAN1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_DASH_ATK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon197.dds",
        TitleID 	=	 583,
        DescID 	=	 630,
        ControlDesc	=	677,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--대쉬 공격 Lv2
	{
        ID 	=	SID_RYAN1_DASH_ATK_LV2,
        GroupID 	=	 SG_RYAN1_DASH_ATK,
        PreList 	=	 { SID_RYAN1_DASH_ATK_LV1, },
        NextList 	=	 { SID_RYAN1_DASH_ATK_LV3, },
        CharLv 	=	 17,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon198.dds",
        TitleID 	=	 584,
        DescID 	=	 631,
        ControlDesc	=	678,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--대쉬 공격 Lv3
	{
        ID 	=	SID_RYAN1_DASH_ATK_LV3,
        GroupID 	=	 SG_RYAN1_DASH_ATK,
        PreList 	=	 { SID_RYAN1_DASH_ATK_LV2, },
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon199.dds",
        TitleID 	=	 585,
        DescID 	=	 632,
        ControlDesc	=	679,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--크리티컬 Lv1
	{
        ID 	=	SID_RYAN1_CRITICAL_LV1,
        GroupID 	=	 SG_RYAN1_CRITICAL_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_CRITICAL, },
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon206.dds",
        TitleID 	=	 586,
        DescID 	=	 633,
        ControlDesc	=	680,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--크리티컬 Lv2
	{
        ID 	=	SID_RYAN1_CRITICAL,
        GroupID 	=	 SG_RYAN1_CRITICAL_ATK,
        PreList 	=	 { SID_RYAN1_CRITICAL_LV1, },
        NextList 	=	 {},
        CharLv 	=	 9,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon207.dds",
        TitleID 	=	 587,
        DescID 	=	 634,
        ControlDesc	=	681,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--자연의 방패
	{
        ID 	=	SID_RYAN1_RYAN_NATURESHIELD,
        GroupID 	=	 SG_RYAN1_SURVIVAL,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon188.dds",
        TitleID 	=	 588,
        DescID 	=	 635,
        ControlDesc	=	682,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--방어 강화 토템폴
	{
        ID 	=	SID_RYAN1_DP_TOTEM,
        GroupID 	=	 SG_RYAN1_TOTEMPOLE,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_HP_TOTEM, },
        CharLv 	=	 12,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon191.dds",
        TitleID 	=	 589,
        DescID 	=	 636,
        ControlDesc	=	683,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--체력 강화 토템폴
	{
        ID 	=	SID_RYAN1_HP_TOTEM,
        GroupID 	=	 SG_RYAN1_TOTEMPOLE,
        PreList 	=	 { SID_RYAN1_DP_TOTEM, },
        NextList 	=	 { SID_RYAN1_MP_TOTEM, },
        CharLv 	=	 26,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon192.dds",
        TitleID 	=	 590,
        DescID 	=	 637,
        ControlDesc	=	684,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--정신 강화 토템폴
	{
        ID 	=	SID_RYAN1_MP_TOTEM,
        GroupID 	=	 SG_RYAN1_TOTEMPOLE,
        PreList 	=	 { SID_RYAN1_HP_TOTEM, },
        NextList 	=	 {},
        CharLv 	=	 42,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon193.dds",
        TitleID 	=	 591,
        DescID 	=	 638,
        ControlDesc	=	685,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--돌벽 세우기
	{
        ID 	=	SID_RYAN1_STONE_WALL,
        GroupID 	=	 SG_RYAN1_DEFENCE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 15,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon186.dds",
        TitleID 	=	 592,
        DescID 	=	 639,
        ControlDesc	=	686,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--콤보 공격 Lv1
	{
        ID 	=	SID_RYAN1_COMBO_LV1,
        GroupID 	=	 SG_RYAN1_COMBO_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_COMBO_LV2, },
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon204.dds",
        TitleID 	=	 593,
        DescID 	=	 640,
        ControlDesc	=	687,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--콤보 공격 Lv2
	{
        ID 	=	SID_RYAN1_COMBO_LV2,
        GroupID 	=	 SG_RYAN1_COMBO_ATK,
        PreList 	=	 { SID_RYAN1_COMBO_LV1, },
        NextList 	=	 {},
        CharLv 	=	 13,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon205.dds",
        TitleID 	=	 594,
        DescID 	=	 641,
        ControlDesc	=	688,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--공중 콤보
	{
        ID 	=	SID_RYAN1_COMBO_UP,
        GroupID 	=	 SG_RYAN1_COMBO_PLUS,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_COMBO_DOWN, },
        CharLv 	=	 7,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon194.dds",
        TitleID 	=	 595,
        DescID 	=	 642,
        ControlDesc	=	689,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--하단 콤보
	{
        ID 	=	SID_RYAN1_COMBO_DOWN,
        GroupID 	=	 SG_RYAN1_COMBO_PLUS,
        PreList 	=	 { SID_RYAN1_COMBO_UP, },
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon208.dds",
        TitleID 	=	 596,
        DescID 	=	 643,
        ControlDesc	=	690,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	--트윈 블레이드 Lv1
	{
        ID 	=	SID_RYAN1_SPECIAL1,
        GroupID 	=	 SG_RYAN1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_SPECIAL1_LV2, SID_RYAN1_SPECIAL1_LV3, SID_RYAN1_CRASH_AXE,SID_RYAN1_FREEZING_ATK,},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage422.dds",
        TitleID 	=	 597,
        DescID 	=	 644,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RYAN_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--트윈 블레이드 Lv2
	{
        ID 	=	SID_RYAN1_SPECIAL1_LV2,
        GroupID 	=	 SG_RYAN1_SPECIAL1,
        PreList 	=	 { SID_RYAN1_SPECIAL1, },
        NextList 	=	 { SID_RYAN1_SPECIAL1_LV3, },
        CharLv 	=	 10,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon220.dds",
        TitleID 	=	 598,
        DescID 	=	 645,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_RYAN_EXT_SPECIAL1_1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--트윈 블레이드 Lv3
	{
        ID 	=	SID_RYAN1_SPECIAL1_LV3,
        GroupID 	=	 SG_RYAN1_SPECIAL1,
        PreList 	=	 { SID_RYAN1_SPECIAL1_LV2, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,		
        Img 	=	 "skillicon221.dds",
        TitleID 	=	 599,
        DescID 	=	 646,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 1,        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_RYAN_SPECIAL1_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    
    --크래쉬 엑스
	{
        ID 	=	SID_RYAN1_CRASH_AXE,
        GroupID 	=	 SG_RYAN1_SPECIAL1,
        PreList 	=	 { SID_RYAN1_SPECIAL1, },
        NextList 	=	 { SID_RYAN1_POISON_SPORE, },
        CharLv 	=	 6,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon217.dds",
        TitleID 	=	 600,
        DescID 	=	 647,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_RYAN1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_RYAN_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
   	
	--포이즌 스포어
	{
        ID 	=	SID_RYAN1_POISON_SPORE,
        GroupID 	=	 SG_RYAN1_SPECIAL1,
        PreList 	=	 { SID_RYAN1_CRASH_AXE, },
        NextList 	=	 {},
        CharLv 	=	 19,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon222.dds",
        TitleID 	=	 601,
        DescID 	=	 648,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_RYAN1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_RYAN_POISON_SPORE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--프리징 어택
	{
        ID 	=	SID_RYAN1_FREEZING_ATK,
        GroupID 	=	 SG_RYAN1_SPECIAL1,
        PreList 	=	 {SID_RYAN1_SPECIAL1,},
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon223.dds",
        TitleID 	=	 602,
        DescID 	=	 649,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_RYAN1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_RYAN_FREEZING_ATK,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--라이칸 롤링 어택(늑대 1필)
	{
        ID 	=	SID_RYAN1_WOLF_SPECIAL1,
        GroupID 	=	 SG_RYAN1_WOLF_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 8,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon212.dds",
        TitleID 	=	 603,
        DescID 	=	 650,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SKILL_RYAN_WOLF_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	--타이탄 엑스 Lv1
	{
        ID 	=	SID_RYAN1_SPECIAL2,
        GroupID 	=	 SG_RYAN1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_SPECIAL2_LV2, SID_RYAN1_SPECIAL2_LV3, SID_RYAN1_SUMMON_WOLVES, SID_RYAN1_SPECIAL2_GREATDEVIDE, SID_RYAN1_THUNDER_TRUST,},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage423.dds",
        TitleID 	=	 604,
        DescID 	=	 651,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RYAN_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--타이탄 엑스 Lv2
	{
        ID 	=	SID_RYAN1_SPECIAL2_LV3,
        GroupID 	=	 SG_RYAN1_SPECIAL2,
        PreList 	=	 { SID_RYAN1_SPECIAL2, },
        NextList 	=	 { SID_RYAN1_SPECIAL2_LV2, },
        CharLv 	=	 15,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon218.dds",
        TitleID 	=	 605,
        DescID 	=	 653,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_RYAN_SPECIAL2_LV3_1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--타이탄 엑스 Lv3
	{
        ID 	=	SID_RYAN1_SPECIAL2_LV2,
        GroupID 	=	 SG_RYAN1_SPECIAL2,
        PreList 	=	 { SID_RYAN1_SPECIAL2_LV3, },
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon219.dds",
        TitleID 	=	 606,
        DescID 	=	 652,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_RYAN_SPECIAL2_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--그레이트 디바이드
	{
        ID 	=	SID_RYAN1_SPECIAL2_GREATDEVIDE,
        GroupID 	=	 SG_RYAN1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_GLORIOUS, },
        CharLv 	=	 8,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon209.dds",
        TitleID 	=	 607,
        DescID 	=	 654,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	294,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_RYAN1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_RYAN_GREAT_DEVIDE_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--글로리어스 스탬핑
	{
        ID 	=	SID_RYAN1_GLORIOUS,
        GroupID 	=	 SG_RYAN1_SPECIAL2,
        PreList 	=	 { SID_RYAN1_SPECIAL2_GREATDEVIDE, },
        NextList 	=	 {},
        CharLv 	=	 22,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon210.dds",
        TitleID 	=	 608,
        DescID 	=	 655,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_RYAN1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_RYAN_GLORIOUS_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--썬더 트러스트
	{
        ID 	=	SID_RYAN1_THUNDER_TRUST,
        GroupID 	=	 SG_RYAN1_SPECIAL2,
        PreList 	=	 {SID_RYAN1_SPECIAL2, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon216.dds",
        TitleID 	=	 609,
        DescID 	=	 656,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_RYAN1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_RYAN_THUNDER,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--버서크 울버린
	{
        ID 	=	SID_RYAN1_WOLF_SPECIAL2,
        GroupID 	=	 SG_RYAN1_WOLF_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 16,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon214.dds",
        TitleID 	=	 610,
        DescID 	=	 657,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SKILL_RYAN_WOLF_SPECIAL2_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--리저렉션 Lv1
	{
        ID 	=	SID_RYAN1_RESERECTION_LV1,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_RYAN1_RESERECTION_LV2,SID_RYAN1_SUMMON_WOLVES, SID_RYAN1_FRIGHT_PUNISHER, },
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage424.dds",
        TitleID 	=	 611,
        DescID 	=	 658,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	100,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RYAN_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--리저렉션 Lv2
	{
        ID 	=	SID_RYAN1_RESERECTION_LV2,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 { SID_RYAN1_RESERECTION_LV1, },
        NextList 	=	 { SID_RYAN1_HAMMERING_CRUSHER, },
        CharLv 	=	 20,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon213.dds",
        TitleID 	=	 612,
        DescID 	=	 659,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_RYAN_RESURECTION,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--해머링 크러셔
	{
        ID 	=	SID_RYAN1_HAMMERING_CRUSHER,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 { SID_RYAN1_RESERECTION_LV2, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon225.dds",
        TitleID 	=	 613,
        DescID 	=	 660,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_RYAN_HAMMERING,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--프라이트 퍼니셔
	{
        ID 	=	SID_RYAN1_FRIGHT_PUNISHER,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 {SID_RYAN1_RESERECTION_LV1,},
        NextList 	=	 { SID_RYAN1_BLOODY, },
        CharLv 	=	 10,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon226.dds",
        TitleID 	=	 614,
        DescID 	=	 661,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_RYAN1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_RYAN_PUNISHER,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--블러디 퓨너럴
	{
        ID 	=	SID_RYAN1_BLOODY,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 { SID_RYAN1_FRIGHT_PUNISHER, },
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon215.dds",
        TitleID 	=	 615,
        DescID 	=	 662,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_RYAN1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_RYAN_BLOODY,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--네이쳐 커맨더
	{
        ID 	=	SID_RYAN1_SUMMON_WOLVES,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 {SID_RYAN1_SPECIAL2,},
        NextList 	=	 { SID_RYAN1_PHOENIX_ASSULT, },
        CharLv 	=	 24,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon211.dds",
        TitleID 	=	 616,
        DescID 	=	 663,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_RYAN1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_RYAN_SUMMON_WOLF,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--피닉스 어썰트
	{
        ID 	=	SID_RYAN1_PHOENIX_ASSULT,
        GroupID 	=	 SG_RYAN1_SPECIAL3,
        PreList 	=	 { SID_RYAN1_SUMMON_WOLVES, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon224.dds",
        TitleID 	=	 617,
        DescID 	=	 664,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_RYAN1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SKILL_RYAN_PHOENIX_ASSULT,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	--늑대변신(기본 늑대)
    {		
        ID 	=	 SID_RYAN1_CHANGE_WOLF,
        GroupID 	=	 SG_RYAN1_CASH_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 3225, },
        Base 	=	 FALSE,
        Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage430.dds",
        TitleID 	=	 704,
        DescID 	=	 707,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	660,
        UI_Pos_y 	=	260,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_WOLF_FORM_START,			
			CoolTime = 18,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--달빛 늑대 변신(신필)
    {		
        ID 	=	 SID_RYAN1_CHANGE_WOLF_MOONLIGHT,
        GroupID 	=	 SG_RYAN1_CASH_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 3227, 13713, },
        Base 	=	 FALSE,
        Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon228.dds",
        TitleID 	=	 705,
        DescID 	=	 708,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	660,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_WOLF_FORM_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	--암흑 늑대 변신(신필)
    {		
        ID 	=	 SID_RYAN1_CHANGE_WOLF_DARK,
        GroupID 	=	 SG_RYAN1_CASH_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 3253, 13714, },
        Base 	=	 FALSE,
        Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon227.dds",
        TitleID 	=	 706,
        DescID 	=	 709,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	660,
        UI_Pos_y 	=	400,
        UI_LineType 	=	 2,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_WOLF_FORM_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	-- 라이언 기본 마나 4칸 확장
    {		
		ID 			=	SID_RYAN1_HIDDEN_MP4,
		GroupID 	=	SG_RYAN1_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	4,
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

    -- 라이언 기본 4단필살기
    {		
		ID 			=	SID_RYAN1_SPECIAL_4,
		GroupID 	=	SG_RYAN1_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	4,
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
			Motion = MID_RYAN1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			ChargingSkillLV = 4,
		},
    },
}