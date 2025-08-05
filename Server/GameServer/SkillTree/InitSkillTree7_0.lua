TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ 진
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- 마나 4칸 확장
    {		
		ID 			=	SID_JIN1_HIDDEN_MP4,
		GroupID 	=	SG_JIN1_HIDDEN_MP4,
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
		ID 			=	SID_JIN1_SPECIAL_4,
		GroupID 	=	SG_JIN1_SPECIAL_4,
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
	-- 1단 필살기 [연격 철산고]
    {		
        ID 	=	 SID_JIN1_CASH_TYPEA_SPECIAL1_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL1,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL1_LV1},
        NextList 	=	 {},
        CharLv 	=	 10,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2995.dds",
        TitleID 	=	 1367,
        DescID 	=	 1425,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_JIN1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_JIN1_SPECIAL1_SHOULDERCHARGE_A,
			AddMotion = MID_JIN1_SPECIAL1_SHOULDERCHARGE_A_BURNING,		
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	-- 1단 필살기 [통배권]
    {		
        ID 	=	 SID_JIN1_CASH_TYPEB_SPECIAL1_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL1,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL1_LV1},
        NextList 	=	 {},
        CharLv 	=	 20,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2998.dds",
        TitleID 	=	 1368,
        DescID 	=	 1426,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153 + Y_GAP,
        UI_LineType 	=	 7,
        
        LockGroup = BGID_JIN1_SPECIAL1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_JIN1_SPECIAL1_QUIVERINGPALM,
			AddMotion = MID_JIN1_SPECIAL1_QUIVERINGPALM_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	-- 1단 필살기 [마룡타 LV1]
    {		
        ID 	=	 SID_JIN1_BASE_SPECIAL1_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_BASE_SPECIAL1_LV2, SID_JIN1_CASH_TYPEA_SPECIAL1_LV1, SID_JIN1_CASH_TYPEB_SPECIAL1_LV1, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1575.dds",
        TitleID 	=	 1369,
        DescID 	=	 1427,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	186,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_JIN_SPECIAL1,
			AddMotion = MID_JIN_SPECIAL1_FORCED,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	-- 1단 필살기 [마룡타 LV2]
    {		
        ID 	=	 SID_JIN1_BASE_SPECIAL1_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL1,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL1_LV1},
        NextList 	=	 {},
        CharLv 	=	 15,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage3002.dds",
        TitleID 	=	 1370,
        DescID 	=	 1428,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	186,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_JIN1_SPECIAL1_LV2,			
			AddMotion = MID_JIN1_SPECIAL1_LV2_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
	-- 2단 필살기 [용포]
    {		
        ID 	=	 SID_JIN1_CASH_TYPEA_SPECIAL2_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL2,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL2_LV1, },
        NextList 	=	 {},
        CharLv 	=	 13,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage3003.dds",
        TitleID 	=	 1371,
        DescID 	=	 1429,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_JIN1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_JIN1_SPECIAL2_DRAGONSTRIKE,			
			AddMotion = MID_JIN1_SPECIAL2_DRAGONSTRIKE_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	-- 2단 필살기 [고산연격]
    {		
        ID 	=	 SID_JIN1_CASH_TYPEB_SPECIAL2_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL2,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL2_LV1, },
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage3028.dds",
        TitleID 	=	 1372,
        DescID 	=	 1430,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296 + Y_GAP,
        UI_LineType 	=	 7,
        LockGroup = BGID_JIN1_SPECIAL2,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_JIN1_SPECIAL2_FURRYOFBLOWS,			
			AddMotion = MID_JIN1_SPECIAL2_FURRYOFBLOWS_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	-- 2단 필살기 [백화난무 LV1]
    {		
        ID 	=	 SID_JIN1_BASE_SPECIAL2_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_BASE_SPECIAL2_LV2, SID_JIN1_CASH_TYPEA_SPECIAL2_LV1, SID_JIN1_CASH_TYPEB_SPECIAL2_LV1, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1577.dds",
        TitleID 	=	 1373,
        DescID 	=	 1431,
        ControlDesc	=	126,        		
        
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_JIN_SPECIAL2,			
			AddMotion = MID_JIN_SPECIAL2_FORCED,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	-- 2단 필살기 [백화난무 LV2]
    {		
        ID 	=	 SID_JIN1_BASE_SPECIAL2_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL2,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL2_LV1, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage3005.dds",
        TitleID 	=	 1374,
        DescID 	=	 1432,
        ControlDesc	=	126,        		
        
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_JIN1_SPECIAL2_LV2,			
			AddMotion = MID_JIN1_SPECIAL2_LV2_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	-- 3단 필살기 [귀산마]
    {		
        ID 	=	 SID_JIN1_CASH_TYPEA_SPECIAL3_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL3,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL3_LV1, },
        NextList 	=	 {},
        CharLv 	=	 16,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage3006.dds",
        TitleID 	=	 1375,
        DescID 	=	 1433,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = BGID_JIN1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_JIN1_SPECIAL3_EMPTYBODY,			
			AddMotion = MID_JIN1_SPECIAL3_EMPTYBODY_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	-- 3단 필살기 [섬광열권]
    {		
        ID 	=	 SID_JIN1_CASH_TYPEB_SPECIAL3_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL3,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL3_LV1, },
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage2999.dds",
        TitleID 	=	 1376,
        DescID 	=	 1434,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439 + Y_GAP,
        UI_LineType 	=	 7,
        
        LockGroup = BGID_JIN1_SPECIAL3,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_JIN1_SPECIAL3_EARTHBREAK,			
			AddMotion = MID_JIN1_SPECIAL3_EARTHBREAK_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	-- 3단 필살기 [천공포 LV1]
    {		
        ID 	=	 SID_JIN1_BASE_SPECIAL3_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_CASH_TYPEA_SPECIAL3_LV1, SID_JIN1_CASH_TYPEB_SPECIAL3_LV1, SID_JIN1_BASE_SPECIAL3_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1579.dds",
        TitleID 	=	 1377,
        DescID 	=	 1435,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	472,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_JIN_SPECIAL3,			
			AddMotion = MID_JIN_SPECIAL3_FORCED,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	-- 3단 필살기 [천공포 LV2]
    {		
        ID 	=	 SID_JIN1_BASE_SPECIAL3_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL3,
        PreList 	=	 {SID_JIN1_BASE_SPECIAL3_LV1, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillimage3007.dds",
        TitleID 	=	 1378,
        DescID 	=	 1436,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	472,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_JIN1_SPECIAL3_LV2,			
			AddMotion = MID_JIN1_SPECIAL3_LV2_BURNING,
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	-- 버닝 감소 시간 LV1
    {		
        ID 	=	 SID_JIN1_BUNNING_DROP_TIME_LV1,
        GroupID 	=	 SG_JIN1_BUNNING_DROP_TIME,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_BUNNING_DROP_TIME_LV2, },
        CharLv 	=	 6,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1379,
        DescID 	=	 1437,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	329,
        UI_Pos_y 	=	175,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	-- 버닝 감소 시간 LV2
    {		
        ID 	=	 SID_JIN1_BUNNING_DROP_TIME_LV2,
        GroupID 	=	 SG_JIN1_BUNNING_DROP_TIME,
        PreList 	=	 {SID_JIN1_BUNNING_DROP_TIME_LV1, },
        NextList 	=	 {SID_JIN1_BUNNING_DROP_TIME_LV3, },
        CharLv 	=	 17,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1380,
        DescID 	=	 1438,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	329 - X_GAP,
        UI_Pos_y 	=	175,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	-- 버닝 감소 시간 LV3
    {		
        ID 	=	 SID_JIN1_BUNNING_DROP_TIME_LV3,
        GroupID 	=	 SG_JIN1_BUNNING_DROP_TIME,
        PreList 	=	 {SID_JIN1_BUNNING_DROP_TIME_LV2, },
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	 1381,
        DescID 	=	 1439,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	329 - X_GAP - X_GAP,
        UI_Pos_y 	=	175,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	-- 버닝 유지 시간 LV1
    {		
        ID 	=	 SID_JIN1_BUNNING_KEEP_TIME_LV1,
        GroupID 	=	 SG_JIN1_BUNNING_KEEP_TIME,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_BUNNING_KEEP_TIME_LV2, },
        CharLv 	=	 8,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1382,
        DescID 	=	 1440,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	275,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 버닝 유지 시간 LV2
    {		
        ID 	=	 SID_JIN1_BUNNING_KEEP_TIME_LV2,
        GroupID 	=	 SG_JIN1_BUNNING_KEEP_TIME,
        PreList 	=	 {SID_JIN1_BUNNING_KEEP_TIME_LV1, },
        NextList 	=	 {SID_JIN1_BUNNING_KEEP_TIME_LV3, },
        CharLv 	=	 13,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1383,
        DescID 	=	 1441,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	464 + X_GAP,
        UI_Pos_y 	=	275,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 버닝 유지 시간 LV3
    {		
        ID 	=	 SID_JIN1_BUNNING_KEEP_TIME_LV3,
        GroupID 	=	 SG_JIN1_BUNNING_KEEP_TIME,
        PreList 	=	 {SID_JIN1_BUNNING_KEEP_TIME_LV2, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	 1384,
        DescID 	=	 1442,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	464 + X_GAP + X_GAP,
        UI_Pos_y 	=	275,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 버닝 필살기 LV1
    {		
        ID 	=	 SID_JIN1_BUNNING_USE_LV1,
        GroupID 	=	 SG_JIN1_BUNNING_USE,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_BUNNING_USE_LV2, },
        CharLv 	=	 11,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1385,
        DescID 	=	 1443,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	329,
        UI_Pos_y 	=	375,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	-- 버닝 필살기 LV2
    {		
        ID 	=	 SID_JIN1_BUNNING_USE_LV2,
        GroupID 	=	 SG_JIN1_BUNNING_USE,
        PreList 	=	 {SID_JIN1_BUNNING_USE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1386,
        DescID 	=	 1444,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	329 - X_GAP,
        UI_Pos_y 	=	375,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
	-- 능공허도 LV1
    {		
        ID 	=	 SID_JIN1_COMMON_JUMP_LV1,
        GroupID 	=	 SG_JIN1_COMMON_JUMP,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_COMMON_JUMP_LV2, },
        CharLv 	=	 21,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1387,
        DescID 	=	 1445,
        ControlDesc	=	1420,
        		
        UI_Pos_x 	=	464,
        UI_Pos_y 	=	475,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 능공허도 LV2
    {		
        ID 	=	 SID_JIN1_COMMON_JUMP_LV2,
        GroupID 	=	 SG_JIN1_COMMON_JUMP,
        PreList 	=	 {SID_JIN1_COMMON_JUMP_LV1, },
        NextList 	=	 {SID_JIN1_COMMON_JUMP_LV3, },
        CharLv 	=	 31,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1388,
        DescID 	=	 1446,
        ControlDesc	=	1420,
        		
        UI_Pos_x 	=	464 + X_GAP,
        UI_Pos_y 	=	475,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 능공허도 LV3
    {		
        ID 	=	 SID_JIN1_COMMON_JUMP_LV3,
        GroupID 	=	 SG_JIN1_COMMON_JUMP,
        PreList 	=	 {SID_JIN1_COMMON_JUMP_LV2, },
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 7,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	 1389,
        DescID 	=	 1447,
        ControlDesc	=	1420,
        		
        UI_Pos_x 	=	464 + X_GAP + X_GAP,
        UI_Pos_y 	=	475,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 콤보 어택 LV1
    {		
        ID 	=	 SID_JIN1_COMBO_ATK_LV1,
        GroupID 	=	 SG_JIN1_COMBO_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_COMBO_ATK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 450,
        DescID 	=	 1448,
        ControlDesc	=	221,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 콤보 어택 LV2
    {		
        ID 	=	 SID_JIN1_COMBO_ATK_LV2,
        GroupID 	=	 SG_JIN1_COMBO_ATK,
        PreList 	=	 {SID_JIN1_COMBO_ATK_LV1},
        NextList 	=	 {},
        CharLv 	=	 18,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 451,
        DescID 	=	 1449,
        ControlDesc	=	220,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 더블 어택 LV1
    {		
        ID 	=	 SID_JIN1_DOUBLE_ATK_LV1,
        GroupID 	=	 SG_JIN1_DOUBLE_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_DOUBLE_ATK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 64,
        DescID 	=	 1450,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 더블 어택 LV2
    {		
        ID 	=	 SID_JIN1_DOUBLE_ATK_LV2,
        GroupID 	=	 SG_JIN1_DOUBLE_ATK,
        PreList 	=	 {SID_JIN1_DOUBLE_ATK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 12,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 150,
        DescID 	=	 1451,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 콤보 응용
    {		
        ID 	=	 SID_JIN1_COMBO_PLUS,
        GroupID 	=	 SG_JIN1_COMBO_PLUS,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 5,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1390,
        DescID 	=	 1452,
        ControlDesc	=	504,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 점프 어택 LV1
    {		
        ID 	=	 SID_JIN1_JUMP_ATK_LV1,
        GroupID 	=	 SG_JIN1_JUMP_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_JUMP_ATK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 996,
        DescID 	=	 1453,
        ControlDesc	=	403,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 점프 어택 LV2
    {		
        ID 	=	 SID_JIN1_JUMP_ATK_LV2,
        GroupID 	=	 SG_JIN1_JUMP_ATK,
        PreList 	=	 {SID_JIN1_JUMP_ATK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 22,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 997,
        DescID 	=	 1454,
        ControlDesc	=	403,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 점프 응용
    {		
        ID 	=	 SID_JIN1_JUMP_PLUS,
        GroupID 	=	 SG_JIN1_JUMP_PLUS,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1391,
        DescID 	=	 1455,
        ControlDesc	=	1421,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 대쉬 콤보 LV1
    {		
        ID 	=	 SID_JIN1_DASH_COMBO_LV1,
        GroupID 	=	 SG_JIN1_DASH_COMBO,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_DASH_COMBO_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 1392,
        DescID 	=	 1456,
        ControlDesc	=	774,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 대쉬 콤보 LV2
    {		
        ID 	=	 SID_JIN1_DASH_COMBO_LV2,
        GroupID 	=	 SG_JIN1_DASH_COMBO,
        PreList 	=	 {SID_JIN1_DASH_COMBO_LV1, },
        NextList 	=	 {SID_JIN1_DASH_COMBO_LV3, },
        CharLv 	=	 16,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1393,
        DescID 	=	 1457,
        ControlDesc	=	774,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 대쉬 콤보 LV3
    {		
        ID 	=	 SID_JIN1_DASH_COMBO_LV3,
        GroupID 	=	 SG_JIN1_DASH_COMBO,
        PreList 	=	 {SID_JIN1_DASH_COMBO_LV2, },
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1394,
        DescID 	=	 1458,
        ControlDesc	=	774,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 대쉬 어택 LV1
    {		
        ID 	=	 SID_JIN1_DASH_ATK_LV1,
        GroupID 	=	 SG_JIN1_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_DASH_ATK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 447,
        DescID 	=	 1459,
        ControlDesc	=	773,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 대쉬 어택 LV2
    {		
        ID 	=	 SID_JIN1_DASH_ATK_LV2,
        GroupID 	=	 SG_JIN1_DASH_ATK,
        PreList 	=	 {SID_JIN1_DASH_ATK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 448,
        DescID 	=	 1460,
        ControlDesc	=	773,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 금강권 LV1
    {		
        ID 	=	 SID_JIN1_GUMGANG_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_GUMGANG_LV2, },
        CharLv 	=	 9,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1395,
        DescID 	=	 1461,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,
        LockGroup = -1,
    },
	-- 금강권 LV2
    {		
        ID 	=	 SID_JIN1_GUMGANG_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {SID_JIN1_GUMGANG_LV1, },
        NextList 	=	 {SID_JIN1_GUMGANG_LV3, },
        CharLv 	=	 19,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1396,
        DescID 	=	 1462,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	516,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
	-- 금강권 LV3
    {		
        ID 	=	 SID_JIN1_GUMGANG_LV3,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {SID_JIN1_GUMGANG_LV2, },
        NextList 	=	 {SID_JIN1_GUMGANG_LV4, },
        CharLv 	=	 29,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	 1397,
        DescID 	=	 1463,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	579,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
	-- 금강권 LV4
    {		
        ID 	=	 SID_JIN1_GUMGANG_LV4,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {SID_JIN1_GUMGANG_LV3, },
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 4,
		
        
        TitleID 	=	 1398,
        DescID 	=	 1464,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	579 + 63,
        UI_Pos_y 	=	500,
        UI_LineType 	=	1,        
        
        LockGroup = -1,
    },
	-- 규화 LV1
    {		
        ID 	=	 SID_JIN1_GYUHWA_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_GYUHWA_LV2, },
        CharLv 	=	 9,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1399,
        DescID 	=	 1465,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	318,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
	-- 규화 LV2
    {		
        ID 	=	 SID_JIN1_GYUHWA_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {SID_JIN1_GYUHWA_LV1, },
        NextList 	=	 {SID_JIN1_GYUHWA_LV3, },
        CharLv 	=	 19,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	 1400,
        DescID 	=	 1466,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	318 - 63,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
	-- 규화 LV3
    {		
        ID 	=	 SID_JIN1_GYUHWA_LV3,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {SID_JIN1_GYUHWA_LV2, },
        NextList 	=	 {SID_JIN1_GYUHWA_LV4, },
        CharLv 	=	 29,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	 1401,
        DescID 	=	 1467,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	318 - 63 - 63,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
	-- 규화 LV4
    {		
        ID 	=	 SID_JIN1_GYUHWA_LV4,
        GroupID 	=	 SG_JIN1_SPECIAL_BURNING,
        PreList 	=	 {SID_JIN1_GYUHWA_LV3, },
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 4,
		
        
        TitleID 	=	 1402,
        DescID 	=	 1468,
        ControlDesc	=	1423,
        		
        UI_Pos_x 	=	318 - 63 - 63 - 63,
        UI_Pos_y 	=	500,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
	-- 질풍
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_1_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_1,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_SPECIAL_COMMAND_1_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 1403,
        DescID 	=	 1469,
        ControlDesc	=	1184,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 풍신권
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_1_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_1,
        PreList 	=	 {SID_JIN1_SPECIAL_COMMAND_1_LV1, },
        NextList 	=	 {},
        CharLv 	=	 24,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1404,
        DescID 	=	 1470,
        ControlDesc	=	1184,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 나선풍
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_2_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_2,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_SPECIAL_COMMAND_2_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 1405,
        DescID 	=	 1471,
        ControlDesc	=	771,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 전신연포
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_2_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_2,
        PreList 	=	 {SID_JIN1_SPECIAL_COMMAND_2_LV1, },
        NextList 	=	 {},
        CharLv 	=	 7,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1406,
        DescID 	=	 1472,
        ControlDesc	=	771,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 천류불식
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_LINK,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_LINK,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1407,
        DescID 	=	 1473,
        ControlDesc	=	397,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 기공파
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_3_LV1,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_3,
        PreList 	=	 {},
        NextList 	=	 {SID_JIN1_SPECIAL_COMMAND_3_LV2, },
        CharLv 	=	 0,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	 1408,
        DescID 	=	 1474,
        ControlDesc	=	492,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 십자경
    {		
        ID 	=	 SID_JIN1_SPECIAL_COMMAND_3_LV2,
        GroupID 	=	 SG_JIN1_SPECIAL_COMMAND_3,
        PreList 	=	 {SID_JIN1_SPECIAL_COMMAND_3_LV1, },
        NextList 	=	 {},
        CharLv 	=	 14,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1409,
        DescID 	=	 1475,
        ControlDesc	=	492,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
	-- 연속 회피
    {		
        ID 	=	 SID_JIN1_EVASION,
        GroupID 	=	 SG_JIN1_EVASION,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 7,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	 1410,
        DescID 	=	 1476,
        ControlDesc	=	1424,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },
}