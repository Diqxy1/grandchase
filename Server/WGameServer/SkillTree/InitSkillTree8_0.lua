TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ 검투사 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- 마나 4칸 확장
    {		
		ID 			=	SID_SIEG1_HIDDEN_MP4,
		GroupID 	=	SG_SIEG1_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	6,
		Promotion 	=	1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon981.dds",
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

    -- 4단필살기
    {		
		ID 			=	SID_SIEG1_SPECIAL_4,
		GroupID 	=	SG_SIEG1_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	6,
		Promotion 	=	1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon982.dds",
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
			Motion = -1,
			--Motion = MID_JIN1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			
			ChargingSkillLV = 4,
		},
    },
	{		
        ID 	=	SID_SIEG1_BERSERK_LV1,
        GroupID 	=	 SG_SIEG1_COMMON_BERSERK,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_BERSERK_LV2},
        CharLv 	=	 5,
        CharType 	=	 8,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "sieg1skill_Berserk_Lv1.dds",
        TitleID 	=	990,
        DescID 	=	 1043,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	480,
        UI_Pos_y 	=	405,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_BERSERK_LV2,
        GroupID 	=	 SG_SIEG1_COMMON_BERSERK,
        PreList 	=	 {SID_SIEG1_BERSERK_LV1},
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 8,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "sieg1skill_Berserk_Lv2.dds",
        TitleID 	=	991,
        DescID 	=	 1044,
        ControlDesc	=	214,
        		
        UI_Pos_x 	=	543,
        UI_Pos_y 	=	405,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	 SID_SIEG1_DEFENCE_LV1,
        GroupID 	=	 SG_SIEG1_COMMON_DEFENCE,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_DEFENCE_LV2},
        CharLv 	=	 8,
        CharType 	=	 8,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "sieg1skill_Defence_Lv1.dds",
        TitleID 	=	 992,
        DescID 	=	 1045,
        ControlDesc	=	1010,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	245,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_SIEG1_DEFENCE_LV2,
        GroupID 	=	 SG_SIEG1_COMMON_DEFENCE,
        PreList 	=	 {SID_SIEG1_DEFENCE_LV1},
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 8,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "sieg1skill_Defence_Lv2.dds",
        TitleID 	=	 993,
        DescID 	=	 1046,
        ControlDesc	=	1011,
        		
        UI_Pos_x 	=	282,
        UI_Pos_y 	=	245,
        UI_LineType 	=	 0,
        
        LockGroup = -1,        
    },
    --유유자적
    {		
        ID 	=	SID_SIEG1_LIVING_FREE,
        GroupID 	=	 SG_SIEG1_UNIQUE_SKILL,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 42,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img     =   "sieg1skill_LivingFree.dds",
        TitleID 	=	994,
        DescID 	=	 635,
        ControlDesc	=	1012,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },    		
    {		
        ID 	=	 SID_SIEG1_DOUBLE_DASH,
        GroupID 	=	 SG_SIEG1_DASH,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_DoubleDash.dds",
        TitleID 	=	1001,
        DescID 	=	 1055,
        ControlDesc	=	1015,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_EVASION,
        GroupID 	=	 SG_SIEG1_EVASION,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 20,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_Evasion.dds",
        TitleID 	=	995,
        DescID 	=	 1047,
        ControlDesc	=	1013,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },    
    -- 대쉬공격LV2
    {		
        ID 	=	SID_SIEG1_DASH_ATK_COMBO,
        GroupID 	=	 SG_SIEG1_DASH_ATK,
        PreList 	=	 {SID_SIEG1_BASE_DASH_ATK},
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_DashAtk_Lv2.dds",
        TitleID 	=	761,
        DescID 	=	 1054,
        ControlDesc	=	137,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_BASE_DASH_ATK,
        GroupID 	=	 SG_SIEG1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_DASH_ATK_COMBO},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img     =   "sieg1skill_DashAtk.dds",
        TitleID 	=	760,
        DescID 	=	 1053,
        ControlDesc	=	137,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    -- 더블어택LV2
    {		
        ID 	=	SID_SIEG1_NEW_DOUBLE_ATK,
        GroupID 	=	 SG_SIEG1_DOUBLE_ATK,
        PreList 	=	 {SID_SIEG1_BASE_DOUBLE_ATK},
        NextList 	=	 {},
        CharLv 	=	 17,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_DoubleAtk_Lv2.dds",
        TitleID 	=	204,
        DescID 	=	 1059,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_BASE_DOUBLE_ATK,
        GroupID 	=	 SG_SIEG1_DOUBLE_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_DOUBLE_ATK},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img     =   "sieg1skill_DoubleAtk.dds",
        TitleID 	=	203,
        DescID 	=	 1058,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    -- 점프콤보LV2
    {		
        ID 	=	SID_SIEG1_NEW_JUMPATK_COMBO,
        GroupID 	=	 SG_SIEG1_JUMP_ATK,
        PreList 	=	 {SID_SIEG1_BASE_JUMPATK_COMBO},
        NextList 	=	 {SID_SIEG1_UNIQUE_JUMP},
        CharLv 	=	 14,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_JumpAtk_Lv2.dds",
        TitleID 	=	997,
        DescID 	=	 1049,
        ControlDesc	=	1014,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_BASE_JUMPATK_COMBO,
        GroupID 	=	 SG_SIEG1_JUMP_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_JUMPATK_COMBO},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img     =   "sieg1skill_JumpAtk.dds",
        TitleID 	=	996,
        DescID 	=	 1048,
        ControlDesc	=	1014,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_UNIQUE_JUMP,
        GroupID 	=	 SG_SIEG1_JUMP_ATK,
        PreList 	=	 {SID_SIEG1_NEW_JUMPATK_COMBO},
        NextList 	=	 {},
        CharLv 	=	 24,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img     =   "sieg1skill_SuperJump.dds",
        TitleID 	=	998,
        DescID 	=	 1050,
        ControlDesc	=	405,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    -- 콤보파생기LV2
    {		
        ID 	=	SID_SIEG1_NEW_COMBO_PLUS,
        GroupID 	=	 SG_SIEG1_COMBO_PLUS,
        PreList 	=	 {SID_SIEG1_BASE_COMBO_PLUS},
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_ComboPlus_Lv2.dds",
        TitleID 	=	1000,
        DescID 	=	 1052,
        ControlDesc	=	216,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_BASE_COMBO_PLUS,
        GroupID 	=	 SG_SIEG1_COMBO_PLUS,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_COMBO_PLUS},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img     =   "sieg1skill_ComboPlus.dds",
        TitleID 	=	999,
        DescID 	=	 1051,
        ControlDesc	=	216,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    -- 크리티컬어택LV2
    {		
        ID 	=	SID_SIEG1_NEW_CRITICAL_ATK,
        GroupID 	=	 SG_SIEG1_CRITICAL_ATK,
        PreList 	=	 {SID_SIEG1_BASE_CRITICAL_ATK},
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img     =   "sieg1skill_CriticalAtk_Lv1.dds",
        TitleID 	=	202,
        DescID 	=	 1057,
        ControlDesc	=	130,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    {		
        ID 	=	SID_SIEG1_BASE_CRITICAL_ATK,
        GroupID 	=	 SG_SIEG1_CRITICAL_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_CRITICAL_ATK},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img     =   "sieg1skill_CriticalAtk.dds",
        TitleID 	=	201,
        DescID 	=	 1056,
        ControlDesc	=	130,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    --분노모드 타입A LV1
    {		
        ID 	=	SID_SIEG1_FURY_TYPE_A_LV1,
        GroupID 	=	 SG_SIEG1_FURY,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_FURY_TYPE_A_LV2,},
        CharLv 	=	 11,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_furyA_Lv1.dds",
        TitleID 	=	1002,
        DescID 	=	 1060,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    --분노모드 타입A LV2
    {		
        ID 	=	SID_SIEG1_FURY_TYPE_A_LV2,
        GroupID 	=	 SG_SIEG1_FURY,
        PreList 	=	 {SID_SIEG1_FURY_TYPE_A_LV1},
        NextList 	=	 {SID_SIEG1_FURY_TYPE_A_LV3,},
        CharLv 	=	 20,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img     =   "sieg1skill_furyA_Lv2.dds",
        TitleID 	=	1003,
        DescID 	=	 1061,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	516,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    --분노모드 타입A LV3
    {		
        ID 	=	SID_SIEG1_FURY_TYPE_A_LV3,
        GroupID 	=	 SG_SIEG1_FURY,
        PreList 	=	 {SID_SIEG1_FURY_TYPE_A_LV2},
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img     =   "sieg1skill_furyA_Lv3.dds",
        TitleID 	=	1004,
        DescID 	=	 1062,
        ControlDesc	=	1018,
        		
        UI_Pos_x 	=	579,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    --분노모드 타입B LV1
    {		
        ID 	=	SID_SIEG1_FURY_TYPE_B_LV1,
        GroupID 	=	 SG_SIEG1_FURY,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_FURY_TYPE_B_LV2},
        CharLv 	=	 11,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img     =   "sieg1skill_furyB_Lv1.dds",
        TitleID 	=	1005,
        DescID 	=	 1063,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	318,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
    --분노모드 타입B LV2
    {		
        ID 	=	SID_SIEG1_FURY_TYPE_B_LV2,
        GroupID 	=	 SG_SIEG1_FURY,
        PreList 	=	 {SID_SIEG1_FURY_TYPE_B_LV1},
        NextList 	=	 {SID_SIEG1_FURY_TYPE_B_LV3},
        CharLv 	=	 21,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img     =   "sieg1skill_furyB_Lv2.dds",
        TitleID 	=	1006,
        DescID 	=	 1064,
        ControlDesc	=	1016,
        		
        UI_Pos_x 	=	255,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
    --분노모드 타입B LV3
    {		
        ID 	=	SID_SIEG1_FURY_TYPE_B_LV3,
        GroupID 	=	 SG_SIEG1_FURY,
        PreList 	=	 {SID_SIEG1_FURY_TYPE_B_LV2},
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img     =   "sieg1skill_furyB_Lv3.dds",
        TitleID 	=	1007,
        DescID 	=	 1065,
        ControlDesc	=	1017,
        		
        UI_Pos_x 	=	192,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },        
    --아이언크러셔 LV2
    {		
        ID 	=	 SID_SIEG1_BASE_SPECIAL1_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 16,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2813.dds",
        TitleID 	=	 971,
        DescID 	=	 1024,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_BASE_SPECIAL1_LV2,			
			AddMotion = MID_SIEG1_SKILL_BASE_SPECIAL1_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --소드파이어 LV2
    {		
        ID 	=	 SID_SIEG1_BASE_SPECIAL2_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 19,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2817.dds",
        TitleID 	=	 979,
        DescID 	=	 1032,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
		UI_Pos_y 	=	297,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_BASE_SPECIAL2_LV2,			
			AddMotion = MID_SIEG1_SKILL_BASE_SPECIAL2_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --그라인딩 퍼니셔 LV2
    {		
        ID 	=	 SID_SIEG1_BASE_SPECIAL3_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 13,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2821.dds",
        TitleID 	=	 987,
        DescID 	=	 1040,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
		UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_BASE_SPECIAL3_LV2,			
			AddMotion = MID_SIEG1_SKILL_BASE_SPECIAL3_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    --아머크래셔 LV1
    {		
        ID 	=	 SID_SIEG1_NEW_SPECIAL1_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_SPECIAL1_LV2,},
        CharLv 	=	 7,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2837.dds",
        TitleID 	=	 972,
        DescID 	=	 1025,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	220,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_NEW_SPECIAL1_LV1,			
			AddMotion = MID_SIEG1_SKILL_NEW_SPECIAL1_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --아머크래셔 LV2
    {		
        ID 	=	 SID_SIEG1_NEW_SPECIAL1_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {SID_SIEG1_NEW_SPECIAL1_LV1,},
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2840.dds",
        TitleID 	=	 973,
        DescID 	=	 1026,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	220,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_NEW_SPECIAL1_LV2,			
			AddMotion = MID_SIEG1_SKILL_NEW_SPECIAL1_LV2_FURY,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --얼티메이트스톰 LV1
    {		
        ID 	=	 SID_SIEG1_NEW_SPECIAL2_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_SPECIAL2_LV2,},
        CharLv 	=	 10,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2825.dds",
        TitleID 	=	 980,
        DescID 	=	 1033,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	364,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_NEW_SPECIAL2_LV1,
			AddMotion = MID_SIEG1_SKILL_NEW_SPECIAL2_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --얼티메이트스톰 LV2
    {		
        ID 	=	 SID_SIEG1_NEW_SPECIAL2_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {SID_SIEG1_NEW_SPECIAL2_LV1,},
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2828.dds",
        TitleID 	=	 981,
        DescID 	=	 1034,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	364,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_NEW_SPECIAL2_LV2,
			AddMotion = MID_SIEG1_SKILL_NEW_SPECIAL2_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --트리플슬래쉬 LV1
    {		
        ID 	=	 SID_SIEG1_NEW_SPECIAL3_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_NEW_SPECIAL3_LV2,},
        CharLv 	=	 23,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2831.dds",
        TitleID 	=	 988,
        DescID 	=	 1041,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 7,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_NEW_SPECIAL3_LV1,
			AddMotion = MID_SIEG1_SKILL_NEW_SPECIAL3_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    --트리플슬래쉬 LV2
    {		
        ID 	=	 SID_SIEG1_NEW_SPECIAL3_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {SID_SIEG1_NEW_SPECIAL3_LV1,},
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2834.dds",
        TitleID 	=	 989,
        DescID 	=	 1042,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_NEW_SPECIAL3_LV2,
			AddMotion = MID_SIEG1_SKILL_NEW_SPECIAL3_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },        
    --소드부메랑 LV1
    {		
        ID 	=	 SID_SIEG1_CASH_MATCH_SPECIAL1_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_CASH_MATCH_SPECIAL1_LV2,},
        CharLv 	=	 15,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2848.dds",
        TitleID 	=	 966,
        DescID 	=	 1019,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	220,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_SIEG1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL1_LV1,
			AddMotion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL1_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --소드부메랑 LV2
    {		
        ID 	=	 SID_SIEG1_CASH_MATCH_SPECIAL1_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {SID_SIEG1_CASH_MATCH_SPECIAL1_LV1,},
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2851.dds",
        TitleID 	=	 967,
        DescID 	=	 1020,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	220,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_SIEG1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL1_LV2,
			AddMotion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL1_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --마하스트라이크 LV1
    {		
        ID 	=	 SID_SIEG1_CASH_TYPEB_SPECIAL1_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_CASH_TYPEB_SPECIAL1_LV2,},
        CharLv 	=	 6,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2876.dds",
        TitleID 	=	 968,
        DescID 	=	 1021,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	154,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_SIEG1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL1_LV1,
			AddMotion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL1_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --마하스트라이크 LV2
    {		
        ID 	=	 SID_SIEG1_CASH_TYPEB_SPECIAL1_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {SID_SIEG1_CASH_TYPEB_SPECIAL1_LV1,},
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2879.dds",
        TitleID 	=	 969,
        DescID 	=	 1022,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	154,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_SIEG1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL1_LV2,
			AddMotion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL1_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    --팬텀러쉬 LV1
    {		
        ID 	=	 SID_SIEG1_CASH_MATCH_SPECIAL2_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_CASH_MATCH_SPECIAL2_LV2,},
        CharLv 	=	 9,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2854.dds",
        TitleID 	=	 974,
        DescID 	=	 1027,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	363,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_SIEG1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL2_LV1,
			AddMotion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL2_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --팬텀러쉬 LV2
    {		
        ID 	=	 SID_SIEG1_CASH_MATCH_SPECIAL2_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {SID_SIEG1_CASH_MATCH_SPECIAL2_LV1,},
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2856.dds",
        TitleID 	=	 975,
        DescID 	=	 1028,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	363,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_SIEG1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL2_LV2,
			AddMotion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL2_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --데스벨 LV1
    {		
        ID 	=	 SID_SIEG1_CASH_TYPEB_SPECIAL2_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_CASH_TYPEB_SPECIAL2_LV2,},
        CharLv 	=	 18,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2866.dds",
        TitleID 	=	 976,
        DescID 	=	 1029,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	297,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_SIEG1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL2_LV1,
			AddMotion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL2_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --데스벨 LV2
    {		
        ID 	=	 SID_SIEG1_CASH_TYPEB_SPECIAL2_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {SID_SIEG1_CASH_TYPEB_SPECIAL2_LV1,},
        NextList 	=	 {},
        CharLv 	=	 37,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2869.dds",
        TitleID 	=	 977,
        DescID 	=	 1030,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	297,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_SIEG1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL2_LV2,
			AddMotion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL2_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    --어노잉제스쳐 LV1
    {		
        ID 	=	 SID_SIEG1_CASH_MATCH_SPECIAL3_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_CASH_MATCH_SPECIAL3_LV2,},
        CharLv 	=	 22,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2861.dds",
        TitleID 	=	 982,
        DescID 	=	 1035,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_SIEG1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV1,
			AddMotion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    --어노잉제스쳐 LV2
    {		
        ID 	=	 SID_SIEG1_CASH_MATCH_SPECIAL3_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {SID_SIEG1_CASH_MATCH_SPECIAL3_LV1,},
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2863.dds",
        TitleID 	=	 983,
        DescID 	=	 1036,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	506,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_SIEG1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV2,
			AddMotion = MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    --모탈임펙트 LV1
    {		
        ID 	=	 SID_SIEG1_CASH_TYPEB_SPECIAL3_LV1,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_CASH_TYPEB_SPECIAL3_LV2,},
        CharLv 	=	 12,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2872.dds",
        TitleID 	=	 984,
        DescID 	=	 1037,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	440,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_SIEG1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL3_LV1,
			AddMotion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL3_LV1_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    --모탈임펙트 LV2
    {		
        ID 	=	 SID_SIEG1_CASH_TYPEB_SPECIAL3_LV2,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {SID_SIEG1_CASH_TYPEB_SPECIAL3_LV1,},
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillimage2874.dds",
        TitleID 	=	 985,
        DescID 	=	 1038,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	440,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_SIEG1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL3_LV2,
			AddMotion = MID_SIEG1_SKILL_CASH_TYPEB_SPECIAL3_LV2_FURY,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    {		
        ID 	=	 SID_SIEG1_BASE_SPECIAL1,
        GroupID 	=	 SG_SIEG1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_BASE_SPECIAL1_LV2,SID_SIEG1_NEW_SPECIAL1_LV1, SID_SIEG1_CASH_MATCH_SPECIAL1_LV1, SID_SIEG1_CASH_TYPEB_SPECIAL1_LV1,},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1826.dds",
        TitleID 	=	 970,
        DescID 	=	 1023,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_SIEG1_SPECIAL1,
			AddMotion = MID_SIEG1_SPECIAL1_F,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },		
    {		
        ID 	=	 SID_SIEG1_BASE_SPECIAL2,
        GroupID 	=	 SG_SIEG1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_BASE_SPECIAL2_LV2,SID_SIEG1_NEW_SPECIAL2_LV1, SID_SIEG1_CASH_MATCH_SPECIAL2_LV1, SID_SIEG1_CASH_TYPEB_SPECIAL2_LV1},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1827.dds",
        TitleID 	=	 978,
        DescID 	=	 1031,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_SIEG1_SPECIAL2,
			AddMotion = MID_SIEG1_SPECIAL2_F,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    {		
        ID 	=	 SID_SIEG1_BASE_SPECIAL3,
        GroupID 	=	 SG_SIEG1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_SIEG1_BASE_SPECIAL3_LV2,SID_SIEG1_NEW_SPECIAL3_LV1, SID_SIEG1_CASH_MATCH_SPECIAL3_LV1, SID_SIEG1_CASH_TYPEB_SPECIAL3_LV1},
        CharLv 	=	 0,
        CharType 	=	 8,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1828.dds",
        TitleID 	=	 986,
        DescID 	=	 1039,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_SIEG1_SPECIAL3,
			AddMotion = MID_SIEG1_SPECIAL3_F,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
}