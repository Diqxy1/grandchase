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
	-- 점프 더블 공격
    {		
        ID 	=	 SID_ELESIS1_JUMP_ATK1,
        GroupID 	=	 SG_ELESIS1_JUMPATK,
        PreList 	=	 {SID_ELESIS1_BASE_JUMPATK,},
        NextList 	=	 {},
		
        CharLv 	=	 47,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon0.dds",
        TitleID 	=	 0,
        DescID 	=	 1,
        ControlDesc	=	143,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },   		
	-- 도약 대쉬
    {		
        ID 	=	 SID_ELESIS1_SUPER_DASH,
        GroupID 	=	 SG_ELESIS1_DASH,
        PreList 	=	 {SID_ELESIS1_BASE_DASH,},
        NextList 	=	 {},
        CharLv 	=	 43,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon1.dds",
        TitleID 	=	 2,
        DescID 	=	 3,
        ControlDesc	=	140,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 대쉬 어택 LV2
    {		
        ID 	=	 SID_ELESIS1_DASH_ATK1,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 { SID_ELESIS1_BASE_DASHATK, },
        NextList 	=	 { SID_ELESIS1_DASH_ATK3, },
        CharLv 	=	 13,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon2.dds",
        TitleID 	=	 4,
        DescID 	=	 5,
        ControlDesc	=	129,
        		
        UI_Pos_x 	=	495,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 4단 콤보
    {		
        ID 	=	 SID_ELESIS1_COMBO_A,
        GroupID 	=	 SG_ELESIS1_COMBO,
        PreList 	=	 {SID_ELESIS1_BASE_COMBO, },
        NextList 	=	 {},
        CharLv 	=	 19,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon3.dds",
        TitleID 	=	 6,
        DescID 	=	 7,
        ControlDesc	=	144,
        		
        UI_Pos_x 	=	290,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 크리티컬 LV2
    {		
        ID 	=	 SID_ELESIS1_CRITICAL_ATK,
        GroupID 	=	 SG_ELESIS1_CRITICAL,
        PreList 	=	 {SID_ELESIS1_BASE_CRITICAL,},
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon4.dds",
        TitleID 	=	 202,
        DescID 	=	 1197,
        ControlDesc	=	130,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 제거 되어야 합니다. (구 대쉬 어택 LV3)
	
    {		
        ID 	=	 SID_ELESIS1_DASH_ATK2,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 { SID_ELESIS1_DASH_ATK1, },
        NextList 	=	 { SID_ELESIS1_DASH_ATK3, },
        CharLv 	=	 36,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon5.dds",
        TitleID 	=	 12,
        DescID 	=	 13,
        ControlDesc	=	138,
        		
        UI_Pos_x 	=	433,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 대쉬 어택 LV3 (위치는 대쉬 어택 LV2 로 수정됨)
    {		
        ID 	=	 SID_ELESIS1_DASH_ATK3,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 { SID_ELESIS1_DASH_ATK1, },
        NextList 	=	 {},
        CharLv 	=	 53,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon6.dds",
        TitleID 	=	 12,
        DescID 	=	 15,
        ControlDesc	=	129,
        		
        UI_Pos_x 	=	433,
        UI_Pos_y 	=	20,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },		
	-- 제거 되어야 합니다. (디펜시브 스윙)
	
    {		
        ID 	=	 SID_ELESIS1_FAST_ATK,
        GroupID 	=	 SG_ELESIS_MAGIC_DEF,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 20,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon7.dds",
        TitleID 	=	 10,
        DescID 	=	 11,
        ControlDesc	=	149,
        		
        UI_Pos_x 	=	55345,
        UI_Pos_y 	=	166,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	
	-- 새로운 디펜시브 스윙 (매직 디펜스)
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_DEFFENCE,
        GroupID 	=	 SG_ELESIS_MAGIC_DEF,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon480.dds",
        TitleID 	=	 1176,
        DescID 	=	 1191,
        ControlDesc	=	1183,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	166,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },			
	-- 풍차 베기
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_ATK1,
        GroupID 	=	 SG_ELESIS1_COMMAND1,
        PreList 	=	 {SID_ELESIS1_SPECIAL_LUNGE_ATK,},
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon8.dds",
        TitleID 	=	 16,
        DescID 	=	 1195,
        ControlDesc	=	1184,
        		
        UI_Pos_x 	=	495,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 제거 되어야 합니다. (체인 콤보)
	
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_ATK2,
        GroupID 	=	 SG_ELESIS1_COMMAND2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon9.dds",
        TitleID 	=	 18,
        DescID 	=	 19,
        ControlDesc	=	148,
        		
        UI_Pos_x 	=	495,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },			
	
	-- 1단 필살기 (메가 슬래쉬 LV2)
    {		
        ID 	=	 SID_ELESIS1_MEGASLASH_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_MEGASLASH_LV3, },
        CharLv 	=	 9,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon10.dds",
        TitleID 	=	 20,
        DescID 	=	 21,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },		
	-- 1단 필살기 (메가 슬래쉬 LV3)
    {		
        ID 	=	 SID_ELESIS1_MEGASLASH_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_MEGASLASH_LV2, },
        NextList 	=	 {},
        CharLv 	=	 27,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon11.dds",
        TitleID 	=	 22,
        DescID 	=	 23,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 2단 필살기 (소드 파이어 LV2)
    {		
        ID 	=	 SID_ELESIS1_SWORDFIRE_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SWORDFIRE_LV3, },
        CharLv 	=	 15,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon12.dds",
        TitleID 	=	 24,
        DescID 	=	 25,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },		
	-- 2단 필살기 (소드 파이어 LV3)
    {		
        ID 	=	 SID_ELESIS1_SWORDFIRE_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 { SID_ELESIS1_SWORDFIRE_LV2, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon13.dds",
        TitleID 	=	 26,
        DescID 	=	 27,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 3단 필살기 (크리티컬 X LV2)
    {		
        ID 	=	 SID_ELESIS1_CRITICALX_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_CRITICALX_LV3, },
        CharLv 	=	 21,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon14.dds",
        TitleID 	=	 28,
        DescID 	=	 29,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },		
	-- 3단 필살기 (크리티컬 X LV3)
    {		
        ID 	=	 SID_ELESIS1_CRITICALX_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_CRITICALX_LV2, },
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon15.dds",
        TitleID 	=	 30,
        DescID 	=	 31,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- CASH 1단 필살기 러프 디바이드 LV1
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SPECIAL1_LV2, },
        CharLv 	=	 6,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon16.dds",
        TitleID 	=	 32,
        DescID 	=	 33,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },		
	-- CASH 1단 필살기 러프 디바이드 LV2
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_SPECIAL1_LV1, },
        NextList 	=	 { },
        CharLv 	=	 26,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon17.dds",
        TitleID 	=	 34,
        DescID 	=	 35,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },		
	-- CASH 1단 필살기 와인딩 플래쉬 LV1
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_WINDINGFLASH_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SPECIAL1_WINDINGFLASH_LV2, },
        CharLv 	=	 16,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillImage1297.dds",
        TitleID 	=	 1170,
        DescID 	=	 1185,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },		
	-- CASH 1단 필살기 와인딩 플래쉬 LV2
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_WINDINGFLASH_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_SPECIAL1_WINDINGFLASH_LV1, },
        NextList 	=	 { },
        CharLv 	=	 36,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillImage1298.dds",
        TitleID 	=	 1171,
        DescID 	=	 1186,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },			
	-- 제거 되어야 합니다. (러프 디바이드 LV3)
	
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_SPECIAL1_LV2, },
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon18.dds",
        TitleID 	=	 36,
        DescID 	=	 37,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	206,
        UI_Pos_y 	=	219,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },		
	-- CASH 2단 필살기 라운드 스트림 LV1
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SPECIAL2_LV2, },
        CharLv 	=	 8,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon19.dds",
        TitleID 	=	 38,
        DescID 	=	 39,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },		
	-- CASH 2단 필살기 라운드 스트림 LV2
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 { SID_ELESIS1_SPECIAL2_LV1, },
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon20.dds",
        TitleID 	=	 40,
        DescID 	=	 41,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },		
	-- CASH 2단 크로스 슬래쉬 LV1
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_CROSSSLASH_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SPECIAL2_CROSSSLASH_LV2, },
        CharLv 	=	 18,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillImage1304.dds",
        TitleID 	=	 1172,
        DescID 	=	 1187,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },		
	-- CASH 2단 필살기 크로스 슬래쉬 LV2
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_CROSSSLASH_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 { SID_ELESIS1_SPECIAL2_CROSSSLASH_LV1, },
        NextList 	=	 { },
        CharLv 	=	 38,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillImage1320.dds",
        TitleID 	=	 1173,
        DescID 	=	 1188,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },			
	-- 제거 되어야 합니다. (라운드 스트림 LV3)
	
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon21.dds",
        TitleID 	=	 42,
        DescID 	=	 43,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	22206,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },		
	-- 제거 되어야 합니다. (메테오 슬래쉬 LV1)
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_SPECIAL3_LV3,},
        NextList 	=	 {},
        CharLv 	=	 10,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon22.dds",
        TitleID 	=	 44,
        DescID 	=	 45,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	55505,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
    -- CASH 3단 필살기 위치 이동 (메테오 슬래쉬 LV1 위치로 자리 변경)
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { },
        NextList 	=	 { SID_ELESIS1_SPECIAL3_LV3, },
        CharLv 	=	 10,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon22.dds",
        TitleID 	=	 44,
        DescID 	=	 47,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
	-- CASH 3단 필살기 위치 이동 (메테오 슬래쉬 LV2 위치로 자리 변경)
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_SPECIAL3_LV2, },
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon23.dds",
        TitleID 	=	 46,
        DescID 	=	 49,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
	-- CASH 3단 필살기 매그넘 브레이크 LV1
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_MAGNUMBREAK_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { },
        NextList 	=	 { SID_ELESIS1_SPECIAL3_MAGNUMBREAK_LV2, },
        CharLv 	=	 20,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "SkillImage1322.dds",
        TitleID 	=	 1174,
        DescID 	=	 1189,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 4,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
	-- CASH 3단 필살기 매그넘 브레이크 LV2
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_MAGNUMBREAK_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_SPECIAL3_MAGNUMBREAK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "SkillImage1323.dds",
        TitleID 	=	 1175,
        DescID 	=	 1190,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },			
	
	-- 제거 되어야 합니다. (구 와인딩 플래쉬 LV1)
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL1_A,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon25.dds",
        TitleID 	=	 50,
        DescID 	=	 51,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	153,
        UI_LineType 	=	4,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },
    -- 제거 되어야 합니다. (구 와인딩 플래쉬 LV2 - 사용하지도 않음)
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL1_B,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_NEWSPECIAL1_A, },
        NextList 	=	 {},
        CharLv 	=	 10,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon26.dds",
        TitleID 	=	 52,
        DescID 	=	 53,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	556,
        UI_Pos_y 	=	153,
        UI_LineType 	=	1,
        
        LockGroup = BGID_ELESIS1_SPECIAL1,
    },	
	-- 제거 되어야 합니다. (구 크로스 슬래쉬 LV1)
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL2_A,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon27.dds",
        TitleID 	=	 54,
        DescID 	=	 55,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	296,
        UI_LineType 	=	4,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },		
	-- 제거 되어야 합니다. (구 매그넘 브레이크 LV1)
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL3_A,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 44,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon28.dds",
        TitleID 	=	 56,
        DescID 	=	 57,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	439,
        UI_LineType 	=	4,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
	
	-- 5단 콤보
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO,
        GroupID 	=	 SG_ELESIS1_COMBO,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_COMBO_A,},
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon29.dds",
        TitleID 	=	 58,
        DescID 	=	 59,
        ControlDesc	=	132,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
	-- 공중 콤보 LV1
    {
        ID 	=	 SID_ELESIS1_BASE_COMBO_PLUS,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_COMBO_A_UP, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon30.dds",
        TitleID 	=	 60,
        DescID 	=	 61,
        ControlDesc	=	133,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 크리티컬 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_CRITICAL,
        GroupID 	=	 SG_ELESIS1_CRITICAL,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_CRITICAL_ATK, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon31.dds",
        TitleID 	=	 62,
        DescID 	=	 63,
        ControlDesc	=	130,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 더블어택 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_DOUBLE,
        GroupID 	=	 SG_ELESIS1_DOUBLE,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_DOUBLE_ATK,},
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon31.dds",
        TitleID 	=	 64,
        DescID 	=	 65,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 대쉬 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_DASH,
        GroupID 	=	 SG_ELESIS1_DASH,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SUPER_DASH, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon33.dds",
        TitleID 	=	 66,
        DescID 	=	 67,
        ControlDesc	=	128,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 대쉬 어택 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_DASHATK,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_DASH_ATK1, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon34.dds",
        TitleID 	=	 68,
        DescID 	=	 69,
        ControlDesc	=	129,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 점프 공격 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_JUMPATK,
        GroupID 	=	 SG_ELESIS1_JUMPATK,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_JUMP_ATK1 },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillicon35.dds",
        TitleID 	=	 70,
        DescID 	=	 71,
        ControlDesc	=	131,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 메가 슬래쉬 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_SPECIAL1,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SPECIAL1_LV1,SID_ELESIS1_MEGASLASH_LV2, SID_ELESIS1_SPECIAL1_WINDINGFLASH_LV1, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage15.dds",
        TitleID 	=	 72,
        DescID 	=	 73,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 소드 파이어 LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_SPECIAL2,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SWORDFIRE_LV2,SID_ELESIS1_SPECIAL2_LV1, SID_ELESIS1_SPECIAL2_CROSSSLASH_LV1, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage16.dds",
        TitleID 	=	 74,
        DescID 	=	 75,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 크리티컬 X LV1
    {		
        ID 	=	 SID_ELESIS1_BASE_SPECIAL3,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_CRITICALX_LV2, SID_ELESIS1_SPECIAL3_MAGNUMBREAK_LV1,SID_ELESIS1_SPECIAL3_LV2, },
        CharLv 	=	 0,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage17.dds",
        TitleID 	=	 76,
        DescID 	=	 77,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 소모용 캐쉬 1단 필살기 (라이징 슬래쉬)
    {		
        ID 	=	 SID_ELESIS1_CASH_SPECIAL1,
        GroupID 	=	 SG_ELESIS1_CASH_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 7631, 7649, 4143, 1986, },
        Base 	=	 FALSE,
        Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage226.dds",
        TitleID 	=	 78,
        DescID 	=	 79,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	186,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 소모용 캐쉬 2단 필살기 (버서크러쉬)
    {		
        ID 	=	 SID_ELESIS1_CASH_SPECIAL2,
        GroupID 	=	 SG_ELESIS1_CASH_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 7637, 7655, 4149, 2159, },
        Base 	=	 FALSE,
        Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage266.dds",
        TitleID 	=	 80,
        DescID 	=	 81,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 소모용 캐쉬 3단 필살기 (토네이도블레이드)
    {		
        ID 	=	 SID_ELESIS1_CASH_SPECIAL3,
        GroupID 	=	 SG_ELESIS1_CASH_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 7638, 7656, 4150, 2160, },
        Base 	=	 FALSE,
        Cash    =   TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage267.dds",
        TitleID 	=	 82,
        DescID 	=	 83,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	625,
        UI_Pos_y 	=	472,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 전진 콤보
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_UP ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {SID_ELESIS1_BASE_COMBO_PLUS,},
        NextList 	=	 {SID_ELESIS1_COMBO_A_UP2, },
        CharLv 	=	 5,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon42.dds",
        TitleID 	=	 84,
        DescID 	=	 85,
        ControlDesc	=	133,
        		
        UI_Pos_x 	=	290,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 8,
        
        LockGroup = -1,
    },		
	-- 공용으로 위치 변경 (구르기 LV1)
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_DOWN  ,
        GroupID 	=	 SG_ELESIS1_AVOID,
        PreList 	=	 { },
        NextList 	=	 {SID_ELESIS1_COMBO_A_DOWN2, },
        CharLv 	=	 17,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon43.dds",
        TitleID 	=	 86,
        DescID 	=	 87,
        ControlDesc	=	146,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 공중 더블 콤보
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_UP2 ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {SID_ELESIS1_COMBO_A_UP },
        NextList 	=	 {SID_ELESIS1_BASE_COMBO_UP2,},
        CharLv 	=	 14,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon44.dds",
        TitleID 	=	 88,
        DescID 	=	 89,
        ControlDesc	=	145,
        		
        UI_Pos_x 	=	352,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 공용으로 위치 변경 (구르기 LV2)
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_DOWN2 ,
        GroupID 	=	 SG_ELESIS1_AVOID,
        PreList 	=	 {SID_ELESIS1_COMBO_A_DOWN, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon45.dds",
        TitleID 	=	 90,
        DescID 	=	 91,
        ControlDesc	=	146,
        		
        UI_Pos_x 	=	282,
        UI_Pos_y 	=	330,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		 
	-- 제거 되어야 합니다. (하단 콤보 LV1)
	
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO_DOWN,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS2,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_COMBO_A_DOWN, },
        CharLv 	=	 5,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon46.dds",
        TitleID 	=	 92,
        DescID 	=	 93,
        ControlDesc	=	135,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	
	-- 공중 콤보 LV2
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO_UP2,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {SID_ELESIS1_COMBO_A_UP2,},
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon47.dds",
        TitleID 	=	 454,
        DescID 	=	 97,
        ControlDesc	=	134,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },		
	-- 제거 되어야 합니다. (하단 콤보 LV2)
	
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO_DOWN2  ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS2,
        PreList 	=	 {SID_ELESIS1_COMBO_A_DOWN, },
        NextList 	=	 { },
        CharLv 	=	 22,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 FALSE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon48.dds",
        TitleID 	=	 94,
        DescID 	=	 95,
        ControlDesc	=	136,
        		
        UI_Pos_x 	=	90,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	
	-- 파이어볼 베기
    {		
        ID 	=	 SID_ELESIS1_MAGIC_DEFENSE,
        GroupID 	=	 SG_ELESIS_DEFENCE,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 11,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon49.dds",
        TitleID 	=	 98,
        DescID 	=	 99,
        ControlDesc	=	141,
        		
        UI_Pos_x 	=	481,
        UI_Pos_y 	=	247,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
	-- 슈퍼 버서커 LV1
    {		
        ID 	=	 SID_ELESIS1_SUPER_BERSERK_LV1  ,
        GroupID 	=	 SG_ELESIS_BERSERK,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV2, },
        CharLv 	=	 41,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon50.dds",
        TitleID 	=	 100,
        DescID 	=	 101,
        ControlDesc	=	142,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	413,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 슈퍼 버서커 LV2
    {		
        ID 	=	 SID_ELESIS1_SUPER_BERSERK_LV2  ,
        GroupID 	=	 SG_ELESIS_BERSERK,
        PreList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV1, },
        NextList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV3,},
        CharLv 	=	 45,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon51.dds",
        TitleID 	=	 102,
        DescID 	=	 103,
        ControlDesc	=	142,
        		
        UI_Pos_x 	=	282,
        UI_Pos_y 	=	413,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
	-- 슈퍼 버서커 LV3
    {		
        ID 	=	 SID_ELESIS1_SUPER_BERSERK_LV3  ,
        GroupID 	=	 SG_ELESIS_BERSERK,
        PreList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV2,},
        NextList 	=	 {},
        CharLv 	=	 49,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon52.dds",
        TitleID 	=	 104,
        DescID 	=	 105,
        ControlDesc	=	142,
        		
        UI_Pos_x 	=	219,
        UI_Pos_y 	=	413,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
	-- 하이퍼 아머 LV1
    {		
        ID 	=	 SID_ELESIS1_HYPER_ARMOR_LV1  ,
        GroupID 	=	 SG_ELESIS1_HYPER_ARMOR,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_HYPER_ARMOR_LV2, },
        CharLv 	=	 35,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon481.dds",
        TitleID 	=	 1177,
        DescID 	=	 1192,
        ControlDesc	=	947,
        		
        
        UI_Pos_x 	=	481,
        UI_Pos_y 	=	495,
        UI_LineType 	=	 1,
                
        LockGroup = -1,
    },		
	-- 하이퍼 아머 LV2
    {		
        ID 	=	 SID_ELESIS1_HYPER_ARMOR_LV2  ,
        GroupID 	=	 SG_ELESIS1_HYPER_ARMOR,
        PreList 	=	 {SID_ELESIS1_HYPER_ARMOR_LV1, },
        NextList 	=	 { },
        CharLv 	=	 55,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon482.dds",
        TitleID 	=	 1178,
        DescID 	=	 1193,
        ControlDesc	=	947,
        		
		UI_Pos_x 	=	541,
        UI_Pos_y 	=	495,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },			
	-- 더블어택 LV2
    {		
        ID 	=	 SID_ELESIS1_DOUBLE_ATK ,
        GroupID 	=	 SG_ELESIS1_DOUBLE,
        PreList 	=	 {SID_ELESIS1_BASE_DOUBLE},
        NextList 	=	 {},
        CharLv 	=	 22,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon53.dds",
        TitleID 	=	 150,
        DescID 	=	 1196,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
	-- 회피 베기
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_ATK3 ,
        GroupID 	=	 SG_ELESIS1_COMMAND2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 47,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon25.dds",
        TitleID 	=	 1180,
        DescID 	=	 1198,
        ControlDesc	=	1013,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
	-- 프로보크
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_ATK4 ,
        GroupID 	=	 SG_ELESIS1_COMMAND3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 51,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon484.dds",
        TitleID 	=	 1181,
        DescID 	=	 1199,
        ControlDesc	=	1200,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	    
    --돌격 찌르기
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_LUNGE_ATK ,
        GroupID 	=	 SG_ELESIS1_COMMAND1,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SPECIAL_ATK1,},
        CharLv 	=	 7,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon491.dds",
        TitleID 	=	 1179,
        DescID 	=	 1194,
        ControlDesc	=	1184,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },
    
--------------------------------------------------------------------------------------------------------------------------
------------ 법사 
--------------------------------------------------------------------------------------------------------------------------	
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
        TitleID 	=	 203,
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
    },     
--------------------------------------------------------------------------------------------------------------------------
------------ 궁수
--------------------------------------------------------------------------------------------------------------------------
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
    },
--------------------------------------------------------------------------------------------------------------------------
------------ 도적
--------------------------------------------------------------------------------------------------------------------------
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
--------------------------------------------------------------------------------------------------------------------------
------------ 드루이드
--------------------------------------------------------------------------------------------------------------------------
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
    }, --]]
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
    },
	

--------------------------------------------------------------------------------------------------------------------------
------------마검사
--------------------------------------------------------------------------------------------------------------------------
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
--------------------------------------------------------------------------------------------------------------------------
------------무희
--------------------------------------------------------------------------------------------------------------------------


         -- 디바인 메세지 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE1_DIVINEMESSAGE_LV1,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE1_DIVINEMESSAGE_LV2, },
        CharLv 	=	 6,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	835,
        DescID 	=	 886,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	300,
        UI_Pos_y 	=	193 + 31,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL1,
    },

		-- 디바인 메세지 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE1_DIVINEMESSAGE_LV2,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE1_DIVINEMESSAGE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	836,
        DescID 	=	 887,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	300 - X_GAP,
        UI_Pos_y 	=	193 + 31,
        UI_LineType 	=	0,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL1,
    },	
		-- 포토 샷
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE1_PHOTOSHOT,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV1, },
        NextList 	=	 { },
        CharLv 	=	 25,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	837,
        DescID 	=	 888,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	300,
        UI_Pos_y 	=	193 - 31 ,
        UI_LineType 	=	 4,
        
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL1,
    },	
		-- 샤이샤이 펀치 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV1,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV2, SID_AMY1_SPECIAL_DANCE1_PHOTOSHOT, SID_AMY1_SPECIAL_DANCE1_DIVINEMESSAGE_LV1, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	838,
        DescID 	=	 889,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	0,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
		-- 샤이샤이 펀치 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV2,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV3, },
        CharLv 	=	 19,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	839,
        DescID 	=	 890,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	193,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },	
		-- 샤이샤이 펀치 Lv3
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV3,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE1_SHYSHYPUNCH_LV2, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	840,
        DescID 	=	 891,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	454 + X_GAP,
        UI_Pos_y 	=	193,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
		-- 스티그마 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_STIGMA_LV1,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE2_STIGMA_LV2, },
        CharLv 	=	 27,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	841,
        DescID 	=	 892,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	300,
        UI_Pos_y 	=	330 - 31,
        UI_LineType 	=	4,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL2,
    },	
		-- 스티그마 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_STIGMA_LV2,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE2_STIGMA_LV1, },
        NextList 	=	 { },
        CharLv 	=	 41,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	842,
        DescID 	=	 893,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	300 - X_GAP,
        UI_Pos_y 	=	330 - 31,
        UI_LineType 	=	0,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL2,
    },	
		-- 홀리 댄스 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_HOLYDANCE_LV1,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE2_HOLYDANCE_LV2, },
        CharLv 	=	 8,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	843,
        DescID 	=	 894,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	300,
        UI_Pos_y 	=	330 + 31,
        UI_LineType 	=	5,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL2,
    },	
		-- 홀리 댄스 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_HOLYDANCE_LV2,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE2_HOLYDANCE_LV1, },
        NextList 	=	 {},
        CharLv 	=	 43,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	844,
        DescID 	=	 895,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	300 - X_GAP,
        UI_Pos_y 	=	330 + 31,
        UI_LineType 	=	0,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL2,
    },	
    
    
		-- 피버타임 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV1,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV2, SID_AMY1_SPECIAL_DANCE2_STIGMA_LV1, SID_AMY1_SPECIAL_DANCE2_HOLYDANCE_LV1, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	845,
        DescID 	=	 896,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	0,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
		-- 피버타임 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV2,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV3, },
        CharLv 	=	 21,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	846,
        DescID 	=	 897,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	330,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
    --요 밑에 문제
    
		-- 피버타임 Lv3
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV3,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV2, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	847,
        DescID 	=	 898,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	454 + X_GAP,
        UI_Pos_y 	=	330,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
		-- 에이미 댄스
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE3_AMYDANCE,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV1, },
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	848,
        DescID 	=	 899,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	300,
        UI_Pos_y 	=	472 - 31,
        UI_LineType 	=	4,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL3,
    },	
    
		-- 팬 서비스 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE3_FANSERVICE,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE3_FANSERVICE_LV2, },
        CharLv 	=	 10,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	849,
        DescID 	=	 900,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	300,
        UI_Pos_y 	=	472 + 31,
        UI_LineType 	=	5,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL3,
    },	
		-- 팬 서비스 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE3_FANSERVICE_LV2,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE3_FANSERVICE, },
        NextList 	=	 {},
        CharLv 	=	 43,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	850,
        DescID 	=	 901,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	300 - X_GAP,
        UI_Pos_y 	=	472 + 31,
        UI_LineType 	=	0,
        
        LockGroup = BGID_AMY1_DANCE_SPECIAL3,
    },	
    
    --요 밑은 안전
		-- 러빙 유 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV1,
        GroupID 	=	 SG_AMY1_DANCE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV2, SID_AMY1_SPECIAL_DANCE3_AMYDANCE, SID_AMY1_SPECIAL_DANCE3_FANSERVICE, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	851,
        DescID 	=	 902,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	0,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
		-- 러빙 유 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV2,
        GroupID 	=	SG_AMY1_DANCE_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV1, },
        NextList 	=	 {},
        CharLv 	=	 23,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	852,
        DescID 	=	 903,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	454,
        UI_Pos_y 	=	472,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
    
    --요 밑은 안전
		-- 트리플 악셀 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT1_TRIPLEAXEL_LV1,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT1_SPINNINGCOUNTER_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT1_TRIPLEAXEL_LV2, },
        CharLv 	=	 13,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	853,
        DescID 	=	 904,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	460,
        UI_Pos_y 	=	195 + 31, 
        UI_LineType 	=	7,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL1,
    },	
		-- 트리플 악셀 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT1_TRIPLEAXEL_LV2,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT1_TRIPLEAXEL_LV1, },
        NextList 	=	 {},
        CharLv 	=	 42,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	854,
        DescID 	=	 905,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	460 + X_GAP,
        UI_Pos_y 	=	195 + 31, 
        UI_LineType 	=	1,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL1,
    },	
		-- 리버스 턴
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT1_REVERSETURN,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT1_SPINNINGCOUNTER_LV1, },
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	855,
        DescID 	=	 906,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	460,
        UI_Pos_y 	=	195 - 31, 
        UI_LineType 	=	6,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL1,
    },	
		-- 스피닝 카운터 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT1_SPINNINGCOUNTER_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT1_SPINNINGCOUNTER_LV2, SID_AMY1_SPECIAL_FIGHT1_TRIPLEAXEL_LV1, SID_AMY1_SPECIAL_FIGHT1_REVERSETURN, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	856,
        DescID 	=	 907,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	195,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },	
		-- 스피닝 카운터 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT1_SPINNINGCOUNTER_LV2,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL1,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT1_SPINNINGCOUNTER_LV1, },
        NextList 	=	 {},
        CharLv 	=	 12,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	857,
        DescID 	=	 908,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	312,
        UI_Pos_y 	=	195,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
			-- 스핀 컴비네이션 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT2_SPINCOMBINATION_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT2_SPINCOMBINATION_LV2, },
        CharLv 	=	 15,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	858,
        DescID 	=	 909,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	460,
        UI_Pos_y 	=	332 + 31,
        UI_LineType 	=	7,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL2,
    },
    
    
    --요 밑은 안전
			-- 스핀 컴비네이션 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT2_SPINCOMBINATION_LV2,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT2_SPINCOMBINATION_LV1, },
        NextList 	=	 {},
        CharLv 	=	 44,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	859,
        DescID 	=	 910,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	460 + X_GAP,
        UI_Pos_y 	=	332 + 31,
        UI_LineType 	=	1,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL2,
    },
    
			-- 플래쉬 무브
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT2_EVATION,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV1, },
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	860,
        DescID 	=	 911,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	460,
        UI_Pos_y 	=	332 - 31,
        UI_LineType 	=	6,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL2,
    },
			-- 아이돌 러쉬 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV2, SID_AMY1_SPECIAL_FIGHT2_SPINCOMBINATION_LV1, SID_AMY1_SPECIAL_FIGHT2_EVATION, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	861,
        DescID 	=	 912,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	0,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 아이돌 러쉬 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV2,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV3, },
        CharLv 	=	 14,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	862,
        DescID 	=	 913,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	312,
        UI_Pos_y 	=	332,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
			-- 아이돌 러쉬 Lv3
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV3,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL2,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT2_IDOLRUSH_LV2, },
        NextList 	=	 {},
        CharLv 	=	 32,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	863,
        DescID 	=	 914,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	312 - X_GAP,
        UI_Pos_y 	=	332,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
			-- 싸이코 크래쉬 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT3_PSYCHOCRASH_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT3_HECTOPASCALKICK_LV1, },
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT3_PSYCHOCRASH_LV2, },
        CharLv 	=	 17,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	864,
        DescID 	=	 915,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	460,
        UI_Pos_y 	=	472 + 31,
        UI_LineType 	=	7,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL3,
    },
			-- 싸이코 크래쉬 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT3_PSYCHOCRASH_LV2,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT3_PSYCHOCRASH_LV1, },
        NextList 	=	 {},
        CharLv 	=	 46,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	865,
        DescID 	=	 916,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	460 + X_GAP,
        UI_Pos_y 	=	472 + 31,
        UI_LineType 	=	1,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL3,
    },
			-- 뷰티 사인
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT3_BEAUTYSIGN,
        GroupID 	=	 SG_AMY1_FIGHT_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT3_HECTOPASCALKICK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	866,
        DescID 	=	 917,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	460,
        UI_Pos_y 	=	472 - 31,
        UI_LineType 	=	6,
        
        LockGroup = BGID_AMY1_FIGHT_SPECIAL3,
    },
    
        
			-- 헥토파스칼 킥 Lv1
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT3_HECTOPASCALKICK_LV1,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_SPECIAL_FIGHT3_HECTOPASCALKICK_LV2, SID_AMY1_SPECIAL_FIGHT3_PSYCHOCRASH_LV1, SID_AMY1_SPECIAL_FIGHT3_BEAUTYSIGN, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	867,
        DescID 	=	 918,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	0,
        UI_Pos_y 	=	0,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 헥토파스칼 킥 Lv2
	{		
        ID 	=	SID_AMY1_SPECIAL_FIGHT3_HECTOPASCALKICK_LV2,
        GroupID 	=	SG_AMY1_FIGHT_SPECIAL3,
        PreList 	=	 { SID_AMY1_SPECIAL_FIGHT3_HECTOPASCALKICK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 16,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	868,
        DescID 	=	 919,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	312,
        UI_Pos_y 	=	472,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },
    --요기까지 ㅇㅋ
    
			-- 방어 증가 Lv1
	{		
        ID 	=	SID_AMY1_MODECHANGE_LV1,
        GroupID 	=	SG_AMY1_COMMON_UNIQUE_SKILL,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_MODECHANGE_LV2, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	869,
        DescID 	=	 920,
        ControlDesc	=	948,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	305,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
			-- 방어 증가 Lv2
	{		
        ID 	=	SID_AMY1_MODECHANGE_LV2,
        GroupID 	=	 SG_AMY1_COMMON_UNIQUE_SKILL,
        PreList 	=	 { SID_AMY1_MODECHANGE_LV1, },
        NextList 	=	 { SID_AMY1_MODECHANGE_LV3, },
        CharLv 	=	 20,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	870,
        DescID 	=	 921,
        ControlDesc	=	948,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	305,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
			-- 방어 증가 Lv3
	{		
        ID 	=	SID_AMY1_MODECHANGE_LV3,
        GroupID 	=	 SG_AMY1_COMMON_UNIQUE_SKILL,
        PreList 	=	 { SID_AMY1_MODECHANGE_LV2, },
        NextList 	=	 {},
        CharLv 	=	 47,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	871,
        DescID 	=	 922,
        ControlDesc	=	948,
        		
        UI_Pos_x 	=	536,
        UI_Pos_y 	=	305,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
    
    
			-- 버프 시간 Lv1
	{		
        ID 	=	SID_AMY1_BUFFTIME_LV1,
        GroupID 	=	 SG_AMY1_COMMON_UNIQUE_SKILL2,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_BUFFTIME_LV2, },
        CharLv 	=	 30,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	872,
        DescID 	=	 923,
        ControlDesc	=	948,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	205,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 버프 시간 Lv2
	{		
        ID 	=	SID_AMY1_BUFFTIME_LV2,
        GroupID 	=	SG_AMY1_COMMON_UNIQUE_SKILL2,
        PreList 	=	 { SID_AMY1_BUFFTIME_LV1, },
        NextList 	=	 {},
        CharLv 	=	 36,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	873,
        DescID 	=	 924,
        ControlDesc	=	948,
        		
        UI_Pos_x 	=	536,
        UI_Pos_y 	=	205,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },
			-- 블레싱 Lv1
	{		
        ID 	=	SID_AMY1_BLESSING_LV1,
        GroupID 	=	 SG_AMY1_COMMON_UNIQUE_SKILL3,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_BLESSING_LV2, },
        CharLv 	=	 11,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	874,
        DescID 	=	 925,
        ControlDesc	=	947,
        		
        UI_Pos_x 	=	473,
        UI_Pos_y 	=	405,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 블레싱 Lv2
	{		
        ID 	=	SID_AMY1_BLESSING_LV2,
        GroupID 	=	 SG_AMY1_COMMON_UNIQUE_SKILL3,
        PreList 	=	 { SID_AMY1_BLESSING_LV1, },
        NextList 	=	 { SID_AMY1_BLESSING_LV3, },
        CharLv 	=	 24,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        
        TitleID 	=	875,
        DescID 	=	 926,
        ControlDesc	=	947,
        		
        UI_Pos_x 	=	536,
        UI_Pos_y 	=	405,
        UI_LineType 	=	1,
        
        
        LockGroup = -1,
    },
    
			-- 블레싱 Lv3
	{		
        ID 	=	SID_AMY1_BLESSING_LV3,
        GroupID 	=	SG_AMY1_COMMON_UNIQUE_SKILL3,
        PreList 	=	 { SID_AMY1_BLESSING_LV2, },
        NextList 	=	 {},
        CharLv 	=	45,
        CharType 	=	 6,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        
        TitleID 	=	876,
        DescID 	=	 927,
        ControlDesc	=	947,
        		
        UI_Pos_x 	=	599,
        UI_Pos_y 	=	405,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    
    
    
			-- 댄싱 콤보 공격 Lv1
	{		
        ID 	=	SID_AMY1_DANCE_COMBOATTACK_LV1,
        GroupID 	=	SG_AMY1_DANCE_COMBO,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_DANCE_COMBOATTACK_LV2, },
        CharLv 	=	0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	877,
        DescID 	=	 928,
        ControlDesc	=	688,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 댄싱 콤보 공격 Lv2
	{		
        ID 	=	SID_AMY1_DANCE_COMBOATTACK_LV2,
        GroupID 	=	SG_AMY1_DANCE_COMBO,
        PreList 	=	 { SID_AMY1_DANCE_COMBOATTACK_LV1, },
        NextList 	=	 {},
        CharLv 	=	5,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	878,
        DescID 	=	 929,
        ControlDesc	=	221,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 댄싱 대쉬 공격 Lv1
	{		
        ID 	=	SID_AMY1_DANCE_DASHATTACK_LV1,
        GroupID 	=	SG_AMY1_DANCE_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_DANCE_DASHATTACK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	880,
        DescID 	=	 930,
        ControlDesc	=	944,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 댄싱 대쉬 공격 Lv2
	{		
        ID 	=	SID_AMY1_DANCE_DASHATTACK_LV2,
        GroupID 	=	 SG_AMY1_DANCE_DASH_ATK,
        PreList 	=	 { SID_AMY1_DANCE_DASHATTACK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 18,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	881,
        DescID 	=	 931,
        ControlDesc	=	944,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 댄싱 크리티컬 Lv1
	{		
        ID 	=	SID_AMY1_DANCE_CRITICAL_LV1,
        GroupID 	=	 SG_AMY1_DANCE_CRITICAL,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_DANCE_CRITICAL_LV2, },
        CharLv 	=	0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	883,
        DescID 	=	 932,
        ControlDesc	=	508,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 댄싱 크리티컬 Lv2
	{		
        ID 	=	SID_AMY1_DANCE_CRITICAL_LV2,
        GroupID 	=	 SG_AMY1_DANCE_CRITICAL,
        PreList 	=	 { SID_AMY1_DANCE_CRITICAL_LV1, },
        NextList 	=	 {},
        CharLv 	=	 9,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	884,
        DescID 	=	 933,
        ControlDesc	=	508,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 파이팅 콤보 공격 Lv1
	{		
        ID 	=	SID_AMY1_FIGHT_COMBOATTACK_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_COMBO,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_FIGHT_COMBOATTACK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	951,
        DescID 	=	 934,
        ControlDesc	=	943,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 파이팅 콤보 공격 Lv2
	{		
        ID 	=	SID_AMY1_FIGHT_COMBOATTACK_LV2,
        GroupID 	=	SG_AMY1_FIGHT_COMBO,
        PreList 	=	 { SID_AMY1_FIGHT_COMBOATTACK_LV1, },
        NextList 	=	 { SID_AMY1_FIGHT_COMBOATTACK_LV3, },
        CharLv 	=	7,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	952,
        DescID 	=	 935,
        ControlDesc	=	220,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 파이팅 콤보 공격 Lv3
	{		
        ID 	=	SID_AMY1_FIGHT_COMBOATTACK_LV3,
        GroupID 	=	 SG_AMY1_FIGHT_COMBO,
        PreList 	=	 { SID_AMY1_FIGHT_COMBOATTACK_LV2, },
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	879,
        DescID 	=	 936,
        ControlDesc	=	221,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    
			-- 파이팅 대쉬 공격 Lv1
	{		
        ID 	=	SID_AMY1_FIGHT_DASHATTACK_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_DASH_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_FIGHT_DASHATTACK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	953,
        DescID 	=	 937,
        ControlDesc	=	944,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 파이팅 대쉬 공격 Lv2
	{		
        ID 	=	SID_AMY1_FIGHT_DASHATTACK_LV2,
        GroupID 	=	 SG_AMY1_FIGHT_DASH_ATK,
        PreList 	=	 { SID_AMY1_FIGHT_DASHATTACK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 22,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	954,
        DescID 	=	 938,
        ControlDesc	=	944,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 파이팅 콤보 응용
	{		
        ID 	=	SID_AMY1_FIGHT_COMBOAPPLICATION,
        GroupID 	=	 SG_AMY1_FIGHT_COMBO_PLUS,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 40,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	882,
        DescID 	=	 939,
        ControlDesc	=	504,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    
			-- 파이팅 크리티컬 Lv1
	{		
        ID 	=	SID_AMY1_FIGHT_CRITICAL_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_CRITICAL,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_FIGHT_CRITICAL_LV2, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	955,
        DescID 	=	 940,
        ControlDesc	=	508,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    
    
			-- 파이팅 크리티컬 Lv2
	{		
        ID 	=	SID_AMY1_FIGHT_CRITICAL_LV2,
        GroupID 	=	 SG_AMY1_FIGHT_CRITICAL,
        PreList 	=	 { SID_AMY1_FIGHT_CRITICAL_LV1, },
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	956,
        DescID 	=	 941,
        ControlDesc	=	508,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
			-- 파이팅 더블 어택 Lv1
	{		
        ID 	=	SID_AMY1_FIGHT_DOUBLEATTACK_LV1,
        GroupID 	=	 SG_AMY1_FIGHT_DOUBLE_ATK,
        PreList 	=	 {},
        NextList 	=	 { SID_AMY1_FIGHT_DOUBLEATTACK_LV2, },
        CharLv 	=	 0,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        
        TitleID 	=	949,
        DescID 	=	 940,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
    
			-- 파이팅 더블 어택 Lv2
	{		
        ID 	=	SID_AMY1_FIGHT_DOUBLEATTACK_LV2,
        GroupID 	=	 SG_AMY1_FIGHT_DOUBLE_ATK,
        PreList 	=	 { SID_AMY1_FIGHT_DOUBLEATTACK_LV1, },
        NextList 	=	 {},
        CharLv 	=	 34,
        CharType 	=	 6,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        
        TitleID 	=	885,
        DescID 	=	 942,
        ControlDesc	=	946,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	1,
        
        LockGroup = -1,
    },
--------------------------------------------------------------------------------------------------------------------------
------------ 검투사
--------------------------------------------------------------------------------------------------------------------------        
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
    },
--------------------------------------------------------------------------------------------------------------------------
------------ 클리퍼 - 디오 -
-------------------------------------------------------------------------------------------------------------------------- 

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
	
	
--------------------------------------------------------------------------------------------------------------------------
------------ 드라카 - 디오 1차 -
--------------------------------------------------------------------------------------------------------------------------
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


--------------------------------------------------------------------------------------------------------------------------
------------ 진 [격투가]
--------------------------------------------------------------------------------------------------------------------------
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
	
--------------------------------------------------------------------------------------------------------------------------
------------ 완더러 - 제로 -
--------------------------------------------------------------------------------------------------------------------------	

	-- 클래시컬 포스 기본
    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1477,
		DescID 		=	1553,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1478,
		DescID 		=	1554,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,


		LockGroup = -1,				
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_HIT_SPECIAL_LV1, SID_ZERO1_ICEFORCE_SPECIAL_LV1, SID_ZERO1_FIREFORCE_SPECIAL_LV1, SID_ZERO1_LOSS_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1479,
		DescID 		=	1555,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,


		LockGroup = -1,				
		Overlapped  = TRUE,
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	-- 클래시컬 포스 기본 히트 수 증가
    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_HIT_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_HIT_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_CLASSICALFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1482,
		DescID 		=	1558,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		Passive = TRUE,
		LockGroup = -1,				
		
    },

	-- 클래시컬 포스 기본 공격 거리 증가
    {		
		ID 			=	SID_ZERO1_CLASSICALFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_HIT_SPECIAL_LV1, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1485,
		DescID 		=	1561,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		LockGroup = -1,				
		
		
    },	

	-- 클래시컬 포스 얼음
    {		
		ID 			=	SID_ZERO1_ICEFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV2, SID_ZERO2_POSIONFORCE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1487,
		DescID 		=	1563,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	0,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_ICEFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1488,
		DescID 		=	1564,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	0,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_ICEFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_ICEFORCE_TIME_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1489,
		DescID 		=	1565,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	0,
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },		

	-- 클래시컬 포스 얼음 지속 시간 증가
    {		
		ID 			=	SID_ZERO1_ICEFORCE_TIME_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_ICEFORCE_TIME_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_ICEFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1492,
		DescID 		=	1568,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	

		LockGroup = -1,				
		
		
    },	

	
	-- 클래시컬 포스 얼음 공격 거리 증가
    {		
		ID 			=	SID_ZERO1_ICEFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_ICEFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_TIME_SPECIAL_LV1, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1495,
		DescID 		=	1571,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 - X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT+ Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		

		LockGroup = -1,				
    },	

	-- 클래시컬 포스 불
    {		
		ID 			=	SID_ZERO1_FIREFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV2, SID_ZERO2_POSIONFORCE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1497,
		DescID 		=	1573,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	1,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

    {		
		ID 			=	SID_ZERO1_FIREFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1498,
		DescID 		=	1574,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	1,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	
    {		
		ID 			=	SID_ZERO1_FIREFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_FIREFORCE_TIME_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1499,
		DescID 		=	1575,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200,
		UI_LineType =	1,
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	
	
	-- 클래시컬 포스 불 지속 시간 증가
    {		
		ID 			=	SID_ZERO1_FIREFORCE_TIME_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_FIREFORCE_TIME_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_FIREFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1502,
		DescID 		=	1578,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		
		LockGroup = -1,				
		
		
    },	
	
	-- 클래시컬 포스 불 공격 거리 증가
    {		
		ID 			=	SID_ZERO1_FIREFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_FIREFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO1_FIREFORCE_TIME_SPECIAL_LV1, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1505,
		DescID 		=	1581,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,
		
		LockGroup = -1,				
		
		
    },

	-- 트리플 베쉬
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1507,
		DescID 		=	1583,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 ,
		UI_LineType =	-1,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_TRIPLEBASH,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1508,
		DescID 		=	1584,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 ,
		UI_LineType =	-1,				
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_TRIPLEBASH_LV2,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1509,
		DescID 		=	1585,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 ,
		UI_LineType =	-1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_TRIPLEBASH_LV3,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 트리플 베쉬 공격력 증가
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1510,
		DescID 		=	1586,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 ,
		UI_LineType =	1,		

		LockGroup = -1,				
    },
	
    {		
		ID 			=	SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_TRIPLEBASH_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_GRANX_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1511,
		DescID 		=	1587,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 ,
		UI_LineType =	1,		
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		
    },	
	
	-- 그란엑스
    {		
		ID 			=	SID_ZERO1_GRANX_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL,
		PreList 	=	{SID_ZERO1_TRIPLEBASH_DMG_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_GRANX_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1513,
		DescID 		=	1589,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO1_GRANX_A,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },		
	
    {		
		ID 			=	SID_ZERO1_GRANX_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL,
		PreList 	=	{SID_ZERO1_GRANX_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1514,
		DescID 		=	1590,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO1_GRANX_A,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	
	
	-- 그란엑스 공격력 증가
	{		
		ID 			=	SID_ZERO1_GRANX_DMG_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_GRANX_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1515,
		DescID 		=	1591,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	1,		

		LockGroup = -1,				
		
		
    },
	
	{		
		ID 			=	SID_ZERO1_GRANX_DMG_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1516,
		DescID 		=	1592,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	1,		
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		
    },	
	
	{		
		ID 			=	SID_ZERO1_GRANX_DMG_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_GRANX_SPECIAL_DMG,
		PreList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1517,
		DescID 		=	1593,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
    },
	
	-- 프리클 가쉬
	{		
		ID 			=	SID_ZERO1_PRICKLEGUSH_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_PRICKLEGUSH_SPECIAL,
		PreList 	=	{SID_ZERO1_GRANX_DMG_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1518,
		DescID 		=	1594,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,				

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO1_PRICKLEGUSH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_PRICKLEGUSH_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_PRICKLEGUSH_SPECIAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1519,
		DescID 		=	1595,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO1_PRICKLEGUSH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_PRICKLEGUSH_SPECIAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV1, SID_ZERO2_BREAKTH_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1520,
		DescID 		=	1596,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,		
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO1_PRICKLEGUSH,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 로스
	{		
		ID 			=	SID_ZERO1_LOSS_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_CLASSICALFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_LOSS_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1523,
		DescID 		=	1599,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 ,
		UI_Pos_y 	=	430 ,
		UI_LineType =	-1,		

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_LOSS_NORMAL_LV2,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV1, },
		NextList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1524,
		DescID 		=	1600,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 ,
		UI_Pos_y 	=	430 ,
		UI_LineType =	-1,	
		Overlapped  = TRUE,

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		
	
	{		
		ID 			=	SID_ZERO1_LOSS_NORMAL_LV3,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV2, },
		NextList 	=	{SID_ZERO1_LOSS_HP_NORMAL_LV1, SID_ZERO1_LOSS_AP_NORMAL_LV1, SID_ZERO2_ORCHID_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1525,
		DescID 		=	1601,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 ,
		UI_Pos_y 	=	430 ,
		UI_LineType =	MID_ZERO1_LOSS,	
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

	-- 로스 HP 흡수
	{		
		ID 			=	SID_ZERO1_LOSS_HP_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1528,
		DescID 		=	1604,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	430 ,
		UI_LineType =	1,	
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

	-- 로스 AP 흡수
	{		
		ID 			=	SID_ZERO1_LOSS_AP_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_LOSS_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1533,
		DescID 		=	1609,
		ControlDesc	=	1169,

		UI_Pos_x 	=	195 + X_GAP_QUICKSLOT,
		UI_Pos_y 	=	430 + Y_GAP_QUICKSLOT,
		UI_LineType =	18,	
		
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_LOSS,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	

	-- 루드 프레셔
	{		
		ID 			=	SID_ZERO1_RUDEPRESSURE_NORMAL_LV1,
		GroupID 	=	SG_ZERO1_RUDEPRESSURE_NORMAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1538,
		DescID 		=	1614,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	

		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_RUDPRESSURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		
	
	{		
		ID 			=	SID_ZERO1_RUDEPRESSURE_NORMAL_LV2,
		GroupID 	=	SG_ZERO1_RUDEPRESSURE_NORMAL,
		PreList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV1, },
		NextList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1539,
		DescID 		=	1615,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	
		Overlapped  = TRUE,


		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_RUDPRESSURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	
	
	{		
		ID 			=	SID_ZERO1_RUDEPRESSURE_NORMAL_LV3,
		GroupID 	=	SG_ZERO1_RUDEPRESSURE_NORMAL,
		PreList 	=	{SID_ZERO1_RUDEPRESSURE_NORMAL_LV2, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1540,
		DescID 		=	1616,
		ControlDesc	=	1169,

		UI_Pos_x 	=	395 ,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,	
		Overlapped  = TRUE,
		LockGroup = -1,				
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO1_RUDPRESSURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },	

	-- 오리지널 스텐스 대공
    {		
		ID 			=	SID_ZERO1_OS_UPATK_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_UPATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1543,
		DescID 		=	1619,
		ControlDesc	=	1629,

		UI_Pos_x 	=	394,
		UI_Pos_y 	=	249,
		UI_LineType =	2,
		LockGroup = -1,
    },	
	
	-- 오리지널 스텐스 전방
    {		
		ID 			=	SID_ZERO1_OS_FRONTATK_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_FRONTATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1544,
		DescID 		=	1620,
		ControlDesc	=	1630,

		UI_Pos_x 	=	294,
		UI_Pos_y 	=	249,
		UI_LineType =	2,

		LockGroup = -1,
    },	
	
	-- 오리지널 스텐스 후방
    {		
		ID 			=	SID_ZERO1_OS_BACKATK_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_BACKATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1545,
		DescID 		=	1621,
		ControlDesc	=	1631,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	249,
		UI_LineType =	2,

		LockGroup = -1,
    },	
	
	-- 오리지널 스텐스 이동
    {		
		ID 			=	SID_ZERO1_OS_MOVE_LV1,
		GroupID 	=	SG_ZERO1_ORIGINAL_STANCE_MOVE,
		PreList 	=	{},
		NextList 	=	{SID_ZERO2_OS_COMBO_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1546,
		DescID 		=	1622,
		ControlDesc	=	1169,

		UI_Pos_x 	=	494,
		UI_Pos_y 	=	249,
		UI_LineType =	2,

		LockGroup = -1,
    },		
	
	-- 디펜스 스텐스
    {		
		ID 			=	SID_ZERO1_DS_LV1,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE,
		PreList 	=	{},
		NextList 	=	{SID_ZERO1_DS_LV2, SID_ZERO2_INCREASE_DS_NORMAL_LV1, SID_ZERO1_DS_BACKATK_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1547,
		DescID 		=	1623,
		ControlDesc	=	1632,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	419,
		UI_LineType =	2,

		LockGroup = -1,
    },	
	
    {		
		ID 			=	SID_ZERO1_DS_LV2,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE,
		PreList 	=	{SID_ZERO1_DS_LV1, },
		NextList 	=	{SID_ZERO1_DS_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1548,
		DescID 		=	1624,
		ControlDesc	=	1632,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	419,
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,
    },		
	
    {		
		ID 			=	SID_ZERO1_DS_LV3,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE,
		PreList 	=	{SID_ZERO1_DS_LV2, },
		NextList 	=	{SID_ZERO1_DS_LV4, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1549,
		DescID 		=	1625,
		ControlDesc	=	1632,

		UI_Pos_x 	=	194,
		UI_Pos_y 	=	419,
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,
    },		

	-- 디펜스 스텐스 후방
    {		
		ID 			=	SID_ZERO1_DS_BACKATK_LV1,
		GroupID 	=	SG_ZERO1_DEFENCE_STANCE_BACKATK,
		PreList 	=	{SID_ZERO1_DS_LV1, },
		NextList 	=	{SID_ZERO2_DS_UP_ATK_LV1, SID_ZERO2_DS_DOWN_ATK_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1552,
		DescID 		=	1628,
		ControlDesc	=	1633,

		UI_Pos_x 	=	294,
		UI_Pos_y 	=	419,
		UI_LineType =	2,		

		LockGroup = -1,
    },		

--------------------------------------------------------------------------------------------------------------------------
------------ 소환사 - 레이 -
--------------------------------------------------------------------------------------------------------------------------	

	-- 그라비티 볼
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_SPECIAL,
		GroupID 	=	SG_LEY01_GRAVITYBALL_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon703.dds",
		TitleID 	=	1638,
		DescID 		=	1678,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_GRAVITY_BALL,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 그라비티 볼 개수 증가 LV1
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GRAVITYBALL_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_SPECIAL, },
		NextList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon704.dds",
		Passive = TRUE,
		TitleID 	=	1639,
		DescID 		=	1679,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,
    },

	-- 그라비티 볼 개수 증가 LV2
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GRAVITYBALL_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon705.dds",
		Passive = TRUE,
		TitleID 	=	1640,
		DescID 		=	1680,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
	-- 그라비티 볼 개수 증가 LV3
    {		
		ID 			=	SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_GRAVITYBALL_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon706.dds",
		Passive = TRUE,
		TitleID 	=	1641,
		DescID 		=	1681,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,
    },

	-- 플레임 롤 LV1
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_FLAMEROLL_SPECIAL,
		PreList 	=	{SID_LEY01_GRAVITYBALL_NUM_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon707.dds",
		TitleID 	=	1642,
		DescID 		=	1682,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_FLAME_ROLL,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 플레임 롤 LV2
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_FLAMEROLL_SPECIAL,
		PreList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon708.dds",
		TitleID 	=	1643,
		DescID 		=	1683,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,	
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_FLAME_ROLL,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 플레임 롤 LV3
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_FLAMEROLL_SPECIAL,
		PreList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_FLAMEROLL_SIZE_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon709.dds",
		TitleID 	=	1644,
		DescID 		=	1684,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_FLAME_ROLL,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 플레임 롤 공격 범위
    {		
		ID 			=	SID_LEY01_FLAMEROLL_SIZE_SPECIAL,
		GroupID 	=	SG_LEY01_FLAMEROLL_SIZE_SPECIAL,
		PreList 	=	{SID_LEY01_FLAMEROLL_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_ETHER, SID_LEY01_INSPIRE_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon710.dds",
		Passive = TRUE,
		TitleID 	=	1645,
		DescID 		=	1685,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },	
	
	-- 에테르
    {		
		ID 			=	SID_LEY01_ETHER,
		GroupID 	=	SG_LEY01_ETHER,
		PreList 	=	{SID_LEY01_FLAMEROLL_SIZE_SPECIAL, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon711.dds",
		TitleID 	=	1646,
		DescID 		=	1686,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12},

		LockGroup = -1,	
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LEY1_ETHER,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		

	-- 인스파이어 LV1
    {		
		ID 			=	SID_LEY01_INSPIRE_LV1,
		GroupID 	=	SG_LEY01_INSPIRE,
		PreList 	=	{SID_LEY01_FLAMEROLL_SIZE_SPECIAL, },
		NextList 	=	{SID_LEY01_INSPIRE_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon712.dds",
		TitleID 	=	1647,
		DescID 		=	1687,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LEY1_INSPIRE,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },

	-- 인스파이어 LV2
    {		
		ID 			=	SID_LEY01_INSPIRE_LV2,
		GroupID 	=	SG_LEY01_INSPIRE,
		PreList 	=	{SID_LEY01_INSPIRE_LV1, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon713.dds",
		TitleID 	=	1648,
		DescID 		=	1688,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,				
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LEY1_INSPIRE,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },	

	-- 시크릿 패시지 LV1
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_SPECIAL,
		PreList 	=	{SID_LEY01_ETHER, SID_LEY01_INSPIRE_LV2, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon714.dds",
		TitleID 	=	1649,
		DescID 		=	1689,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_SECRET_PASSAGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 시크릿 패시지 LV2
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon715.dds",
		TitleID 	=	1650,
		DescID 		=	1690,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,		

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_SECRET_PASSAGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 시크릿 패시지 LV3
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV1, SID_LEY02_BARRAGE_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon716.dds",
		TitleID 	=	1651,
		DescID 		=	1691,
		ControlDesc	=	127,

		UI_Pos_x 	=	440,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,	

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_SECRET_PASSAGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },		

	-- 시크릿 패시지 개수 증가 LV1
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon717.dds",
		Passive = TRUE,
		TitleID 	=	1652,
		DescID 		=	1692,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 시크릿 패시지 개수 증가 LV2
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon718.dds",
		Passive = TRUE,
		TitleID 	=	1653,
		DescID 		=	1693,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },	

	-- 시크릿 패시지 개수 증가 LV3
    {		
		ID 			=	SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_SECRETPASSAGE_NUM_SPECIAL,
		PreList 	=	{SID_LEY01_SECRETPASSAGE_NUM_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon719.dds",
		Passive = TRUE,
		TitleID 	=	1654,
		DescID 		=	1694,
		ControlDesc	=	127,

		UI_Pos_x 	=	440 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 지브스 에티켓 LV1
    {		
		ID 			=	SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_JEEVESETIQUETTE_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon720.dds",
		TitleID 	=	1655,
		DescID 		=	1695,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_JEEVES_ETIQUETTE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 지브스 에티켓 LV2
    {		
		ID 			=	SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_JEEVESETIQUETTE_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon721.dds",
		TitleID 	=	1656,
		DescID 		=	1696,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_JEEVES_ETIQUETTE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 지브스 에티켓 LV3
    {		
		ID 			=	SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_JEEVESETIQUETTE_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon722.dds",
		TitleID 	=	1657,
		DescID 		=	1697,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,				
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY1_JEEVES_ETIQUETTE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 가고울리 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESETIQUETTE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon723.dds",
		TitleID 	=	1658,
		DescID 		=	1698,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_GAROUILLE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 가고울리 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV1, SID_LEY01_GAROUILLE_HP_SPECIAL_LV1, SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1,
						SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV1, SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV1, SID_LEY02_SHOVE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon724.dds",
		TitleID 	=	1659,
		DescID 		=	1699,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY1_GAROUILLE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 가고울리 이동 속도 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SPEED_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon725.dds",
		Passive = TRUE,
		TitleID 	=	1660,
		DescID 		=	1700,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =   -1,

		LockGroup = -1,						
    },

	-- 가고울리 이동 속도 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SPEED_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon726.dds",
		Passive = TRUE,
		TitleID 	=	1661,
		DescID 		=	1701,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 가고울리 체력 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_HP_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_HP_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_HP_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon727.dds",
		Passive = TRUE,
		TitleID 	=	1662,
		DescID 		=	1702,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },
	
	-- 가고울리 체력 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_HP_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_HP_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_HP_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon728.dds",
		Passive = TRUE,
		TitleID 	=	1663,
		DescID 		=	1703,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 가고울리 브레이크 쓰루 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_A_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon729.dds",
		TitleID 	=	1664,
		DescID 		=	1704,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },	
	
	-- 가고울리 브레이크 쓰루 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_A_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_A_SPECIAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon730.dds",
		TitleID 	=	1665,
		DescID 		=	1705,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },	

	-- 가고울리 브레스 LV1
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon731.dds",
		TitleID 	=	1666,
		DescID 		=	1706,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 가고울리 브레스 LV2
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_GAROUILLE_SKILL_B_FIRE_SPECIAL, SID_LEY01_GAROUILLE_SKILL_B_STONE_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon732.dds",
		TitleID 	=	1667,
		DescID 		=	1707,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
    },

	-- 가고울리 브레스 - 파이어
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_FIRE_SPECIAL,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon733.dds",
		TitleID 	=	1668,
		DescID 		=	1708,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },	

	-- 가고울리 브레스 - 스톤
    {		
		ID 			=	SID_LEY01_GAROUILLE_SKILL_B_STONE_SPECIAL,
		GroupID 	=	SG_LEY01_GAROUILLE_SKILL_B_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SKILL_B_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon734.dds",
		TitleID 	=	1669,
		DescID 		=	1709,
		ControlDesc	=	1821,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },		

	-- 지브스 파워 밤 LV1
    {		
		ID 			=	SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1,
		GroupID 	=	SG_LEY01_JEEVESPOWERBOMB_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, SID_LEY01_GAROUILLE_SPEED_SPECIAL_LV2, SID_LEY01_GAROUILLE_HP_SPECIAL_LV2, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon735.dds",
		TitleID 	=	1670,
		DescID 		=	1710,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_JEEVES_POWER_BOMB,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 지브스 파워 밤 LV2
    {		
		ID 			=	SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV2,
		GroupID 	=	SG_LEY01_JEEVESPOWERBOMB_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV1, },
		NextList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon736.dds",
		TitleID 	=	1671,
		DescID 		=	1711,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_JEEVES_POWER_BOMB,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 지브스 파워 밤 LV3
    {		
		ID 			=	SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV3,
		GroupID 	=	SG_LEY01_JEEVESPOWERBOMB_SPECIAL,
		PreList 	=	{SID_LEY01_JEEVESPOWERBOMB_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon737.dds",
		TitleID 	=	1672,
		DescID 		=	1712,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = TRUE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY1_JEEVES_POWER_BOMB,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 챠징 어택
    {		
		ID 			=	SID_LEY01_CHARGEATK,
		GroupID 	=	SG_LEY01_CHARGEATK,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon738.dds",
		TitleID 	=	1673,
		DescID 		=	1713,
		ControlDesc	=	1677,

		UI_Pos_x 	=	186,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	

	-- 플롯
    {		
		ID 			=	SID_LEY01_FLOAT,
		GroupID 	=	SG_LEY01_FLOAT,
		PreList 	=	{},
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon739.dds",
		TitleID 	=	1674,
		DescID 		=	1714,
		ControlDesc	=	1676,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 카운터 시 쿨 타임 감소 LV1
    {		
		ID 			=	SID_LEY01_COUNT_COOLTIME_LV1,
		GroupID 	=	SG_LEY01_COUNT_COOLTIME,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_COUNT_COOLTIME_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon740.dds",
		TitleID 	=	1112,
		DescID 		=	1162,
		ControlDesc	=	1169,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	
	
	-- 카운터 시 쿨 타임 감소 LV2
    {		
		ID 			=	SID_LEY01_COUNT_COOLTIME_LV2,
		GroupID 	=	SG_LEY01_COUNT_COOLTIME,
		PreList 	=	{SID_LEY01_COUNT_COOLTIME_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon741.dds",
		TitleID 	=	1113,
		DescID 		=	1163,
		ControlDesc	=	1169,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },	

	-- 시작 시 AP 보유 LV1
    {		
		ID 			=	SID_LEY01_START_AP_LV1,
		GroupID 	=	SG_LEY01_START_AP,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_START_AP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon742.dds",		
		TitleID 	=	1108,
		DescID 		=	1158,
		ControlDesc	=	1169,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 시작 시 AP 보유 LV2
    {		
		ID 			=	SID_LEY01_START_AP_LV2,
		GroupID 	=	SG_LEY01_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV1, },
		NextList 	=	{SID_LEY01_START_AP_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon743.dds",
		TitleID 	=	1109,
		DescID 		=	1159,
		ControlDesc	=	1169,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

	-- 시작 시 AP 보유 LV3
    {		
		ID 			=	SID_LEY01_START_AP_LV3,
		GroupID 	=	SG_LEY01_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV2, },
		NextList 	=	{SID_LEY01_START_AP_LV4, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon744.dds",
		TitleID 	=	1110,
		DescID 		=	1160,
		ControlDesc	=	1169,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

	-- 크리티컬 증가 LV1
    {		
		ID 			=	SID_LEY01_CRITICAL_LV1,
		GroupID 	=	SG_LEY01_CRITICAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY01_CRITICAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon745.dds",
		TitleID 	=	1114,
		DescID 		=	1715,
		ControlDesc	=	1169,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	

	-- 크리티컬 증가 LV2
    {		
		ID 			=	SID_LEY01_CRITICAL_LV2,
		GroupID 	=	SG_LEY01_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV1, },
		NextList 	=	{SID_LEY01_CRITICAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon746.dds",
		TitleID 	=	1115,
		DescID 		=	1716,
		ControlDesc	=	1169,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },	

	-- 크리티컬 증가 LV3
    {		
		ID 			=	SID_LEY01_CRITICAL_LV3,
		GroupID 	=	SG_LEY01_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV2, },
		NextList 	=	{SID_LEY01_CRITICAL_LV4, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Img 	=	 "skillicon747.dds",
		TitleID 	=	1262,
		DescID 		=	1717,
		ControlDesc	=	1169,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,		
    },	

--------------------------------------------------------------------------------------------------------------------------
------------ 크레이버 - 제로1차 -
--------------------------------------------------------------------------------------------------------------------------	

	-- 그란 포스 LV1
    {		
		ID 			=	SID_ZERO2_POSIONFORCE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO1_ICEFORCE_SPECIAL_LV1, SID_ZERO1_FIREFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO2_POSIONFORCE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1729,
		DescID 		=	1776,
		ControlDesc	=	127,

		UI_Pos_x 	=	450 + X_GAP_LONG,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 그란 포스 LV2
    {		
		ID 			=	SID_ZERO2_POSIONFORCE_SPECIAL_LV2,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO2_POSIONFORCE_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO2_POSIONFORCE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1730,
		DescID 		=	1777,
		ControlDesc	=	127,

		UI_Pos_x 	=	450 + X_GAP_LONG,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 그란 포스 LV3
    {		
		ID 			=	SID_ZERO2_POSIONFORCE_SPECIAL_LV3,
		GroupID 	=	SG_ZERO1_CLASSICALFORCE_SPECIAL,
		PreList 	=	{SID_ZERO2_POSIONFORCE_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO2_POSIONFORCE_TIME_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1731,
		DescID 		=	1778,
		ControlDesc	=	127,

		UI_Pos_x 	=	450 + X_GAP_LONG,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,

		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO1_CLASSICALFORCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 그란 포스 지속 시간
    {		
		ID 			=	SID_ZERO2_POSIONFORCE_TIME_SPECIAL_LV1,
		GroupID 	=	SG_ZERO2_POSIONFORCE_TIME_SPECIAL,
		PreList 	=	{SID_ZERO2_POSIONFORCE_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO2_POSIONFORCE_SIZE_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1732,
		DescID 		=	1779,
		ControlDesc	=	127,

		UI_Pos_x 	=	450 + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Passive = TRUE,
    },	

	-- 그란 포스 공격 범위
    {		
		ID 			=	SID_ZERO2_POSIONFORCE_SIZE_SPECIAL_LV1,
		GroupID 	=	SG_ZERO2_POSIONFORCE_SIZE_SPECIAL,
		PreList 	=	{SID_ZERO2_POSIONFORCE_TIME_SPECIAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1733,
		DescID 		=	1780,
		ControlDesc	=	127,

		UI_Pos_x 	=	450 + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Passive = TRUE,
    },

	-- 바이트 오프 LV1	
    {		
		ID 			=	SID_ZERO2_BITEOFF_SPECIAL_LV1,
		GroupID 	=	SG_ZERO2_BITEOFF_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_ZERO2_BITEOFF_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1720,
		DescID 		=	1767,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO2_BITE_OFF,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 바이트 오프 LV2	
    {		
		ID 			=	SID_ZERO2_BITEOFF_SPECIAL_LV2,
		GroupID 	=	SG_ZERO2_BITEOFF_SPECIAL,
		PreList 	=	{SID_ZERO2_BITEOFF_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO2_BITEOFF_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1721,
		DescID 		=	1768,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO2_BITE_OFF,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 바이트 오프 LV3	
    {		
		ID 			=	SID_ZERO2_BITEOFF_SPECIAL_LV3,
		GroupID 	=	SG_ZERO2_BITEOFF_SPECIAL,
		PreList 	=	{SID_ZERO2_BITEOFF_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO2_BREAKTH_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1722,
		DescID 		=	1769,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_ZERO2_BITE_OFF,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 브레이크 쓰루 LV1	
    {		
		ID 			=	SID_ZERO2_BREAKTH_SPECIAL_LV1,
		GroupID 	=	SG_ZERO2_BREAKTH_SPECIAL,
		PreList 	=	{SID_ZERO1_PRICKLEGUSH_SPECIAL_LV3, SID_ZERO2_BITEOFF_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO2_BREAKTH_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1723,
		DescID 		=	1770,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO2_BREAK_THROUGH,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 브레이크 쓰루 LV2	
    {		
		ID 			=	SID_ZERO2_BREAKTH_SPECIAL_LV2,
		GroupID 	=	SG_ZERO2_BREAKTH_SPECIAL,
		PreList 	=	{SID_ZERO2_BREAKTH_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO2_BREAKTH_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1724,
		DescID 		=	1771,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO2_BREAK_THROUGH,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 브레이크 쓰루 LV3	
    {		
		ID 			=	SID_ZERO2_BREAKTH_SPECIAL_LV3,
		GroupID 	=	SG_ZERO2_BREAKTH_SPECIAL,
		PreList 	=	{SID_ZERO2_BREAKTH_SPECIAL_LV2, },
		NextList 	=	{SID_ZERO2_ABYSS_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1725,
		DescID 		=	1772,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO2_BREAK_THROUGH,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 어비스 LV1	
    {		
		ID 			=	SID_ZERO2_ABYSS_SPECIAL_LV1,
		GroupID 	=	SG_ZERO2_ABYSS_SPECIAL,
		PreList 	=	{SID_ZERO2_BREAKTH_SPECIAL_LV3, },
		NextList 	=	{SID_ZERO2_ABYSS_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1726,
		DescID 		=	1773,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO2_ABYSS,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 어비스 LV2	
    {		
		ID 			=	SID_ZERO2_ABYSS_SPECIAL_LV2,
		GroupID 	=	SG_ZERO2_ABYSS_SPECIAL,
		PreList 	=	{SID_ZERO2_ABYSS_SPECIAL_LV1, },
		NextList 	=	{SID_ZERO2_ABYSS_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1727,
		DescID 		=	1774,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO2_ABYSS,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },	

	-- 어비스 LV3	
    {		
		ID 			=	SID_ZERO2_ABYSS_SPECIAL_LV3,
		GroupID 	=	SG_ZERO2_ABYSS_SPECIAL,
		PreList 	=	{SID_ZERO2_ABYSS_SPECIAL_LV2, },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1728,
		DescID 		=	1775,
		ControlDesc	=	127,

		UI_Pos_x 	=	425,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_ZERO2_ABYSS,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 올키드 LV1	
    {		
		ID 			=	SID_ZERO2_ORCHID_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_ORCHID_NORMAL,
		PreList 	=	{SID_ZERO1_LOSS_NORMAL_LV3, },
		NextList 	=	{SID_ZERO2_ORCHID_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1734,
		DescID 		=	1781,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_ORCHID,			
			CoolTime = 10,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 올키드 LV2	
    {		
		ID 			=	SID_ZERO2_ORCHID_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_ORCHID_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_ORCHID_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1735,
		DescID 		=	1782,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_ORCHID,			
			CoolTime = 10,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 10,
		},
    },

	-- 올키드 LV3	
    {		
		ID 			=	SID_ZERO2_ORCHID_NORMAL_LV3,
		GroupID 	=	SG_ZERO2_ORCHID_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NORMAL_LV2, },
		NextList 	=	{SID_ZERO2_ORCHID_AP_NORMAL_LV1, SID_ZERO2_ORCHID_HP_NORMAL_LV1, SID_ZERO2_ORCHID_NUMBER_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1736,
		DescID 		=	1783,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_ORCHID,			
			CoolTime = 10,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 10,
		},
    },

	-- 올키드 AP 흡수	
    {		
		ID 			=	SID_ZERO2_ORCHID_AP_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_ORCHID_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NORMAL_LV3, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1737,
		DescID 		=	1784,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 - X_GAP_LONG,
		UI_Pos_y 	=	200,
        UI_LineType =	12,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_ORCHID,			
			CoolTime = 10,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 10,
		},
    },

	-- 올키드 HP 흡수	
    {		
		ID 			=	SID_ZERO2_ORCHID_HP_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_ORCHID_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NORMAL_LV3, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1738,
		DescID 		=	1785,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 + X_GAP_LONG,
		UI_Pos_y 	=	200,
        UI_LineType =	17,

		LockGroup = -1,
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_ORCHID,			
			CoolTime = 10,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 10,
		},
    },

	-- 올키드 가시 수 증가 LV1
    {		
		ID 			=	SID_ZERO2_ORCHID_NUMBER_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_ORCHID_NUMBER_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NORMAL_LV3, },
		NextList 	=	{SID_ZERO2_ORCHID_NUMBER_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1739,
		DescID 		=	1786,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
        UI_LineType =	2,

		LockGroup = -1,
		Passive = TRUE,
    },

	-- 올키드 가시 수 증가 LV2
    {		
		ID 			=	SID_ZERO2_ORCHID_NUMBER_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_ORCHID_NUMBER_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NUMBER_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_ORCHID_NUMBER_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1740,
		DescID 		=	1787,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },

	-- 올키드 가시 수 증가 LV3
    {		
		ID 			=	SID_ZERO2_ORCHID_NUMBER_NORMAL_LV3,
		GroupID 	=	SG_ZERO2_ORCHID_NUMBER_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NUMBER_NORMAL_LV2, },
		NextList 	=	{SID_ZERO2_ABSORB_LV1, SID_ZERO2_AMID_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1741,
		DescID 		=	1788,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },

	-- 가시 HP / AP 흡수량 LV1
    {		
		ID 			=	SID_ZERO2_ABSORB_LV1,
		GroupID 	=	SG_ZERO2_ABSORB,
		PreList 	=	{SID_ZERO2_ORCHID_NUMBER_NORMAL_LV3, },
		NextList 	=	{SID_ZERO2_ABSORB_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1742,
		DescID 		=	1789,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 - X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Passive = TRUE,
    },

	-- 가시 HP / AP 흡수량 LV2
    {		
		ID 			=	SID_ZERO2_ABSORB_LV2,
		GroupID 	=	SG_ZERO2_ABSORB,
		PreList 	=	{SID_ZERO2_ABSORB_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1743,
		DescID 		=	1790,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 - X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },

	-- 어미드 (무적)	
    {		
		ID 			=	SID_ZERO2_AMID_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_AMID_NORMAL,
		PreList 	=	{SID_ZERO2_ORCHID_NUMBER_NORMAL_LV3, },
		NextList 	=	{SID_ZERO2_INCREASE_NORMAL_LV1, SID_ZERO2_WHIRLWIND_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1744,
		DescID 		=	1791,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_ZERO2_AMID,			
			CoolTime = 60,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 60,
		},
    },	

	-- 휠 윈드 LV1
    {		
		ID 			=	SID_ZERO2_WHIRLWIND_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_WHIRLWIND_NORMAL,
		PreList 	=	{SID_ZERO2_AMID_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_WHIRLWIND_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1755,
		DescID 		=	1802,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 - X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_WHIRLWIND,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
	-- 휠 윈드 LV2
    {		
		ID 			=	SID_ZERO2_WHIRLWIND_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_WHIRLWIND_NORMAL,
		PreList 	=	{SID_ZERO2_WHIRLWIND_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_WHIRLWIND_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1756,
		DescID 		=	1803,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 - X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_WHIRLWIND,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },

	-- 휠 윈드 LV3
    {		
		ID 			=	SID_ZERO2_WHIRLWIND_NORMAL_LV3,
		GroupID 	=	SG_ZERO2_WHIRLWIND_NORMAL,
		PreList 	=	{SID_ZERO2_WHIRLWIND_NORMAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1757,
		DescID 		=	1804,
		ControlDesc	=	127,

		UI_Pos_x 	=	210 - X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_WHIRLWIND,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
	-- 인크리즈 (버프)	
    {		
		ID 			=	SID_ZERO2_INCREASE_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_INCREASE_NORMAL,
		PreList 	=	{SID_ZERO2_AMID_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_INCREASE_TIME_NORMAL_LV1, SID_ZERO2_INCREASE_SS_NORMAL_LV1,
						SID_ZERO2_INCREASE_DS_NORMAL_LV1, SID_ZERO2_INCREASE_OS_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1745,
		DescID 		=	1792,
		ControlDesc	=	127,

		UI_Pos_x 	=	208 + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	13,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ZERO2_INCREASE,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },	

	-- 인크리즈 시간 증가 LV1	
    {		
		ID 			=	SID_ZERO2_INCREASE_TIME_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_INCREASE_TIME_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_INCREASE_TIME_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1746,
		DescID 		=	1793,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Passive = TRUE,
    },	

	-- 인크리즈 시간 증가 LV2	
    {		
		ID 			=	SID_ZERO2_INCREASE_TIME_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_INCREASE_TIME_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_TIME_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_INCREASE_TIME_NORMAL_LV3, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1747,
		DescID 		=	1794,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },	

	-- 인크리즈 시간 증가 LV3	
    {		
		ID 			=	SID_ZERO2_INCREASE_TIME_NORMAL_LV3,
		GroupID 	=	SG_ZERO2_INCREASE_TIME_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_TIME_NORMAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1748,
		DescID 		=	1795,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
        Overlapped  = TRUE,
		Passive = TRUE,
    },		

	-- 인크리즈 SS 버프 LV1
    {		
		ID 			=	SID_ZERO2_INCREASE_SS_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_INCREASE_SS_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_NORMAL_LV1, },
		NextList 	=	{SID_ZERO2_INCREASE_SS_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1749,
		DescID 		=	1796,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
		Passive = TRUE,
    },

	-- 인크리즈 SS 버프 LV2
    {		
		ID 			=	SID_ZERO2_INCREASE_SS_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_INCREASE_SS_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_SS_NORMAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1750,
		DescID 		=	1797,
		ControlDesc	=	127,

		UI_Pos_x 	=	210,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	12,

		LockGroup = -1,
        Overlapped  = TRUE,
		Passive = TRUE,
    },

	-- 인크리즈 DS 버프 LV1
    {		
		ID 			=	SID_ZERO2_INCREASE_DS_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_INCREASE_DS_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_NORMAL_LV1, SID_ZERO1_DS_LV1, },
		NextList 	=	{SID_ZERO2_INCREASE_DS_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1751,
		DescID 		=	1798,
		ControlDesc	=	127,

		UI_Pos_x 	=	208 + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
		Passive = TRUE,
    },

	-- 인크리즈 DS 버프 LV2
    {		
		ID 			=	SID_ZERO2_INCREASE_DS_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_INCREASE_DS_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_DS_NORMAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1752,
		DescID 		=	1799,
		ControlDesc	=	127,

		UI_Pos_x 	=	208 + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,

		LockGroup = -1,
        Overlapped  = TRUE,
		Passive = TRUE,
    },

	-- 인크리즈 OS 버프 LV1
    {		
		ID 			=	SID_ZERO2_INCREASE_OS_NORMAL_LV1,
		GroupID 	=	SG_ZERO2_INCREASE_OS_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_NORMAL_LV1, SID_ZERO2_OS_COMBO_LV1, },
		NextList 	=	{SID_ZERO2_INCREASE_OS_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1753,
		DescID 		=	1800,
		ControlDesc	=	127,

		UI_Pos_x 	=	208 + X_GAP_LONG + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	17,

		LockGroup = -1,
		Passive = TRUE,
    },

	-- 인크리즈 OS 버프 LV2
    {		
		ID 			=	SID_ZERO2_INCREASE_OS_NORMAL_LV2,
		GroupID 	=	SG_ZERO2_INCREASE_OS_NORMAL,
		PreList 	=	{SID_ZERO2_INCREASE_OS_NORMAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1754,
		DescID 		=	1801,
		ControlDesc	=	127,

		UI_Pos_x 	=	208 + X_GAP_LONG + X_GAP_LONG,
		UI_Pos_y 	=	200 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
		Passive = TRUE,
    },

	-- DS 방어량 증가 LV4
    {		
		ID 			=	SID_ZERO1_DS_LV4,
		GroupID 	=	SG_ZERO2_DEFENCE_STANCE,
		PreList 	=	{SID_ZERO1_DS_LV3, },
		NextList 	=	{SID_ZERO1_DS_LV5, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1550,
		DescID 		=	1805,
		ControlDesc	=	1632,

		UI_Pos_x 	=	215,
		UI_Pos_y 	=	280,
		UI_LineType =	2,

		LockGroup = -1,

    },	
	
	-- DS 방어량 증가 LV5
    {		
		ID 			=	SID_ZERO1_DS_LV5,
		GroupID 	=	SG_ZERO2_DEFENCE_STANCE,
		PreList 	=	{SID_ZERO1_DS_LV4, },
		NextList 	=	{SID_ZERO2_DS_REFLECTION_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1551,
		DescID 		=	1806,
		ControlDesc	=	1632,

		UI_Pos_x 	=	215,
		UI_Pos_y 	=	280,
		UI_LineType =	2,
		Overlapped  = TRUE,

		LockGroup = -1,

    },	

	-- DS 반사 LV1
    {		
		ID 			=	SID_ZERO2_DS_REFLECTION_LV1,
		GroupID 	=	SG_ZERO2_DEFENCE_STANCE_REFLECTION,
		PreList 	=	{SID_ZERO1_DS_LV5, },
		NextList 	=	{SID_ZERO2_DS_REFLECTION_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1758,
		DescID 		=	1807,
		ControlDesc	=	1632,

		UI_Pos_x 	=	335,
		UI_Pos_y 	=	280,
		UI_LineType =	2,

		LockGroup = -1,

    },

	-- DS 반사 LV2
    {		
		ID 			=	SID_ZERO2_DS_REFLECTION_LV2,
		GroupID 	=	SG_ZERO2_DEFENCE_STANCE_REFLECTION,
		PreList 	=	{SID_ZERO2_DS_REFLECTION_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1759,
		DescID 		=	1808,
		ControlDesc	=	1632,

		UI_Pos_x 	=	335,
		UI_Pos_y 	=	280,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,

    },

	-- DS 상단 콤보
    {		
		ID 			=	SID_ZERO2_DS_UP_ATK_LV1,
		GroupID 	=	SG_ZERO2_DEFENCE_STANCE_UP_ATK,
		PreList 	=	{SID_ZERO1_DS_BACKATK_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1760,
		DescID 		=	1809,
		ControlDesc	=	1816,

		UI_Pos_x 	=	455,
		UI_Pos_y 	=	280,
		UI_LineType =	2,

		LockGroup = -1,

    },

	-- DS 하단 콤보
    {		
		ID 			=	SID_ZERO2_DS_DOWN_ATK_LV1,
		GroupID 	=	SG_ZERO2_DEFENCE_STANCE_DOWN_ATK,
		PreList 	=	{SID_ZERO1_DS_BACKATK_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1761,
		DescID 		=	1810,
		ControlDesc	=	1817,

		UI_Pos_x 	=	575,
		UI_Pos_y 	=	280,
		UI_LineType =	2,

		LockGroup = -1,

    },

	-- OS 사용
    {		
		ID 			=	SID_ZERO2_OS_COMBO_LV1,
		GroupID 	=	SG_ZERO2_OFFENCE_STANCE_COMBO,
		PreList 	=	{SID_ZERO1_OS_MOVE_LV1, },
		NextList 	=	{SID_ZERO2_OS_DASH_LV1, SID_ZERO2_OS_CRITICAL_LV1, SID_ZERO2_OS_TRACE_LV1, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1762,
		DescID 		=	1811,
		ControlDesc	=	1818,

		UI_Pos_x 	=	215,
		UI_Pos_y 	=	450,
		UI_LineType =	2,

		LockGroup = -1,

    },	
	
	-- OS 대쉬
    {		
		ID 			=	SID_ZERO2_OS_DASH_LV1,
		GroupID 	=	SG_ZERO2_OFFENCE_STANCE_DASH,
		PreList 	=	{SID_ZERO2_OS_COMBO_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1763,
		DescID 		=	1812,
		ControlDesc	=	1819,

		UI_Pos_x 	=	335,
		UI_Pos_y 	=	450,
		UI_LineType =	2,

		LockGroup = -1,

    },	

	-- OS 크리/더블
    {		
		ID 			=	SID_ZERO2_OS_CRITICAL_LV1,
		GroupID 	=	SG_ZERO2_OFFENCE_STANCE_CRITICAL,
		PreList 	=	{SID_ZERO2_OS_COMBO_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1764,
		DescID 		=	1813,
		ControlDesc	=	1820,

		UI_Pos_x 	=	455,
		UI_Pos_y 	=	450,
		UI_LineType =	2,

		LockGroup = -1,

    },	
	
	-- OS 검흔 LV1
    {		
		ID 			=	SID_ZERO2_OS_TRACE_LV1,
		GroupID 	=	SG_ZERO2_OFFENCE_STANCE_TRACE,
		PreList 	=	{SID_ZERO2_OS_COMBO_LV1, },
		NextList 	=	{SID_ZERO2_OS_TRACE_LV2, },
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1765,
		DescID 		=	1814,
		ControlDesc	=	1169,

		UI_Pos_x 	=	575,
		UI_Pos_y 	=	450,
		UI_LineType =	2,

		LockGroup = -1,

    },		
	
	-- OS 검흔 LV2
    {		
		ID 			=	SID_ZERO2_OS_TRACE_LV2,
		GroupID 	=	SG_ZERO2_OFFENCE_STANCE_TRACE,
		PreList 	=	{SID_ZERO2_OS_TRACE_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	11,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		
		TitleID 	=	1766,
		DescID 		=	1815,
		ControlDesc	=	1169,

		UI_Pos_x 	=	575,
		UI_Pos_y 	=	450,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
    },	

--------------------------------------------------------------------------------------------------------------------------
------------ 다크브링어 - 레이1차 -
--------------------------------------------------------------------------------------------------------------------------

	-- 셔브 LV1
    {		
		ID 			=	SID_LEY02_SHOVE_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_SHOVE_SPECIAL,
		PreList 	=	{SID_LEY01_GAROUILLE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_SHOVE_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1822,
		DescID 		=	1860,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_SHOVE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },	

	-- 셔브 LV2
    {		
		ID 			=	SID_LEY02_SHOVE_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_SHOVE_SPECIAL,
		PreList 	=	{SID_LEY02_SHOVE_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_SHOVE_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1823,
		DescID 		=	1861,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_SHOVE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,
    },

	-- 셔브 LV3
    {		
		ID 			=	SID_LEY02_SHOVE_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_SHOVE_SPECIAL,
		PreList 	=	{SID_LEY02_SHOVE_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1824,
		DescID 		=	1862,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_SHOVE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,
    },

	-- 바스티언 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPECIAL,
		PreList 	=	{SID_LEY02_SHOVE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1825,
		DescID 		=	1863,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_MERRY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 바스티언 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPECIAL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_BASTION_SPEED_LV1, SID_LEY02_BASTION_HP_LV1, SID_LEY02_BASTION_HOWLING_LV1, SID_LEY02_BASTION_SPINDASH_LV1, SID_LEY02_BASTION_SPINDROP_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1826,
		DescID 		=	1864,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	{3, 12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_MERRY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		Overlapped  = TRUE,
    },
	
	-- 바스티언 이동 속도 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPEED_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPEED_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPEED_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

        Passive = TRUE,
		TitleID 	=	1827,
		DescID 		=	1865,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =   -1,

		LockGroup = -1,	
    },

	-- 바스티언 이동 속도 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPEED_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPEED_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_SPEED_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1828,
		DescID 		=	1866,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =   -1,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },
	
	-- 바스티언 체력 LV1
    {		
		ID 			=	SID_LEY02_BASTION_HP_LV1,
		GroupID 	=	SG_LEY02_BASTION_HP_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_HP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1829,
		DescID 		=	1867,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 바스티언 체력 LV2
    {		
		ID 			=	SID_LEY02_BASTION_HP_LV2,
		GroupID 	=	SG_LEY02_BASTION_HP_PASSIVE,
		PreList 	=	{SID_LEY02_BASTION_HP_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Passive = TRUE,
		TitleID 	=	1830,
		DescID 		=	1868,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,		
		Overlapped  = TRUE,		
    },
	
	-- 바스티언 하울링 LV1
    {		
		ID 			=	SID_LEY02_BASTION_HOWLING_LV1,
		GroupID 	=	SG_LEY02_BASTION_HOWLING_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_HOWLING_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1831,
		DescID 		=	1869,
		ControlDesc	=	1903,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 바스티언 하울링 LV2
    {		
		ID 			=	SID_LEY02_BASTION_HOWLING_LV2,
		GroupID 	=	SG_LEY02_BASTION_HOWLING_SKILL,
		PreList 	=	{SID_LEY02_BASTION_HOWLING_LV1, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1832,
		DescID 		=	1870,
		ControlDesc	=	1903,

		UI_Pos_x 	=	230 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 대쉬 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPINDASH_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPINDASH_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPINDASH_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1833,
		DescID 		=	1871,
		ControlDesc	=	1902,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,						
    },

	-- 바스티언 스핀 대쉬 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPINDASH_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPINDASH_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDASH_LV1, },
		NextList 	=	{SID_LEY02_BASTION_SPINDASH_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1834,
		DescID 		=	1872,
		ControlDesc	=	1902,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 대쉬 LV3
    {		
		ID 			=	SID_LEY02_BASTION_SPINDASH_LV3,
		GroupID 	=	SG_LEY02_BASTION_SPINDASH_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDASH_LV2, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1835,
		DescID 		=	1873,
		ControlDesc	=	1902,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 드랍 LV1
    {		
		ID 			=	SID_LEY02_BASTION_SPINDROP_LV1,
		GroupID 	=	SG_LEY02_BASTION_SPINDROP_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BASTION_SPINDROP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1836,
		DescID 		=	1874,
		ControlDesc	=	401,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,						
    },

	-- 바스티언 스핀 드랍 LV2
    {		
		ID 			=	SID_LEY02_BASTION_SPINDROP_LV2,
		GroupID 	=	SG_LEY02_BASTION_SPINDROP_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDROP_LV1, },
		NextList 	=	{SID_LEY02_BASTION_SPINDROP_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1837,
		DescID 		=	1875,
		ControlDesc	=	401,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },

	-- 바스티언 스핀 드랍 LV3
    {		
		ID 			=	SID_LEY02_BASTION_SPINDROP_LV3,
		GroupID 	=	SG_LEY02_BASTION_SPINDROP_SKILL,
		PreList 	=	{SID_LEY02_BASTION_SPINDROP_LV2, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		TitleID 	=	1838,
		DescID 		=	1876,
		ControlDesc	=	401,

		UI_Pos_x 	=	230 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

	-- 그림 핸드 LV1
    {		
		ID 			=	SID_LEY02_GRIMHAND_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_GRIMHAND_SPECIAL,
		PreList 	=	{SID_LEY02_BASTION_HOWLING_LV2, SID_LEY02_BASTION_SPINDASH_LV3, SID_LEY02_BASTION_SPINDROP_LV3, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1839,
		DescID 		=	1877,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_GRIM_HAND,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },

	-- 그림 핸드 LV2
    {		
		ID 			=	SID_LEY02_GRIMHAND_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_GRIMHAND_SPECIAL,
		PreList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1840,
		DescID 		=	1878,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_GRIM_HAND,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
		Overlapped  = TRUE,		
    },

	-- 그림 핸드 LV3
    {		
		ID 			=	SID_LEY02_GRIMHAND_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_GRIMHAND_SPECIAL,
		PreList 	=	{SID_LEY02_GRIMHAND_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1841,
		DescID 		=	1879,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_GRIM_HAND,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
		Overlapped  = TRUE,		
    },

	-- 언설튼 에어 LV1
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_SPECIAL,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1842,
		DescID 		=	1880,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_UNCERTAIN_AIR,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 언설튼 에어 LV2
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_SPECIAL,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1843,
		DescID 		=	1881,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_UNCERTAIN_AIR,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,		
    },

	-- 언설튼 에어 LV3
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_SPECIAL,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_UNCERTAINAIR_NUM, SID_LEY02_TEARDROP_SPECIAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1844,
		DescID 		=	1882,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240,
		UI_LineType =	{3, 12},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LEY2_UNCERTAIN_AIR,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,		
    },

	-- 언설튼 에어 개수 증가
    {		
		ID 			=	SID_LEY02_UNCERTAINAIR_NUM,
		GroupID 	=	SG_LEY02_UNCERTAINAIR_NUM_PASSIVE,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV3, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1845,
		DescID 		=	1883,
		ControlDesc	=	127,

		UI_Pos_x 	=	500 + X_GAP_LONG,
		UI_Pos_y 	=	240,
		UI_LineType =	-1,

		LockGroup = -1,	
		Passive = TRUE,
    },

	-- 티어 드랍 LV1
    {		
		ID 			=	SID_LEY02_TEARDROP_SPECIAL_LV1,
		GroupID 	=	SG_LEY02_TEARDROP_SPECIAL,
		PreList 	=	{SID_LEY02_UNCERTAINAIR_SPECIAL_LV3, },
		NextList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1846,
		DescID 		=	1884,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_TEARDROP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
	-- 티어 드랍 LV2
    {		
		ID 			=	SID_LEY02_TEARDROP_SPECIAL_LV2,
		GroupID 	=	SG_LEY02_TEARDROP_SPECIAL,
		PreList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV1, },
		NextList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1847,
		DescID 		=	1885,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_TEARDROP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		Overlapped  = TRUE,
    },
	
	-- 티어 드랍 LV3
    {		
		ID 			=	SID_LEY02_TEARDROP_SPECIAL_LV3,
		GroupID 	=	SG_LEY02_TEARDROP_SPECIAL,
		PreList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV2, },
		NextList 	=	{SID_LEY02_BARRAGE_NORMAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1848,
		DescID 		=	1886,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT,
		UI_LineType =	3,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LEY2_TEARDROP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
		Overlapped  = TRUE,
    },
	
	-- 버라지 LV1
    {		
		ID 			=	SID_LEY02_BARRAGE_NORMAL_LV1,
		GroupID 	=	SG_LEY02_BARRAGE_NORMAL,
		PreList 	=	{SID_LEY02_TEARDROP_SPECIAL_LV3, SID_LEY01_SECRETPASSAGE_SPECIAL_LV3, },
		NextList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1849,
		DescID 		=	1887,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.5,
			Motion = MID_LEY2_BARRAGE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.5,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 버라지 LV2
    {		
		ID 			=	SID_LEY02_BARRAGE_NORMAL_LV2,
		GroupID 	=	SG_LEY02_BARRAGE_NORMAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV1, },
		NextList 	=	{SID_LEY02_BARRAGE_DARKSIGHT_LV1, SID_LEY02_BARRAGE_BIGEYE, SID_LEY02_COG_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1850,
		DescID 		=	1888,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	{12, 17},

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 0.5,
			Motion = MID_LEY2_BARRAGE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.5,
			MonsterModeCoolTime = 10,
		},
		Overlapped  = TRUE,
    },
	
	-- 버라지 다크 사이트 LV1
    {		
		ID 			=	SID_LEY02_BARRAGE_DARKSIGHT_LV1,
		GroupID 	=	SG_LEY02_BARRAGE_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, },
		NextList 	=	{SID_LEY02_BARRAGE_DARKSIGHT_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1851,
		DescID 		=	1889,
		ControlDesc	=	1169,

		UI_Pos_x 	=	500 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
    },
	
	-- 버라지 다크 사이트 LV2
    {		
		ID 			=	SID_LEY02_BARRAGE_DARKSIGHT_LV2,
		GroupID 	=	SG_LEY02_BARRAGE_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_DARKSIGHT_LV1, },
		NextList 	=	{SID_LEY02_COG_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1852,
		DescID 		=	1890,
		ControlDesc	=	1169,

		UI_Pos_x 	=	500 + X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,
    },
	
	-- 버라지 빅아이 익스플로전
    {		
		ID 			=	SID_LEY02_BARRAGE_BIGEYE,
		GroupID 	=	SG_LEY02_BARRAGE_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, },
		NextList 	=	{SID_LEY02_COG_SPECIAL, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1853,
		DescID 		=	1891,
		ControlDesc	=	1169,

		UI_Pos_x 	=	500 - X_GAP_LONG,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
    },
	
	-- 센터 오브 그라비티
    {		
		ID 			=	SID_LEY02_COG_SPECIAL,
		GroupID 	=	SG_LEY02_COG_SPECIAL,
		PreList 	=	{SID_LEY02_BARRAGE_NORMAL_LV2, SID_LEY02_BARRAGE_DARKSIGHT_LV2, SID_LEY02_BARRAGE_BIGEYE, },
		NextList 	=	{SID_LEY02_COGFATAL_LV1, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1854,
		DescID 		=	1892,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_THE_CENTER_OF_GRAVITY_READY,			
			CoolTime = 35,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 30,
		},
    },

	-- 센터 오브 그라비티 FATAL 확률 LV1
    {		
		ID 			=	SID_LEY02_COGFATAL_LV1,
		GroupID 	=	SG_LEY02_COG_SPECIAL,
		PreList 	=	{SID_LEY02_COG_SPECIAL, },
		NextList 	=	{SID_LEY02_COGFATAL_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1855,
		DescID 		=	1893,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,

		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_THE_CENTER_OF_GRAVITY_READY,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 27,
		},
		
    },

	-- 센터 오브 그라비티 FATAL 확률 LV2
    {		
		ID 			=	SID_LEY02_COGFATAL_LV2,
		GroupID 	=	SG_LEY02_COG_SPECIAL,
		PreList 	=	{SID_LEY02_COGFATAL_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1856,
		DescID 		=	1894,
		ControlDesc	=	127,

		UI_Pos_x 	=	500,
		UI_Pos_y 	=	240 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	-1,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LEY2_THE_CENTER_OF_GRAVITY_READY,			
			CoolTime = 25,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 24,
		},
    },

	-- 점프 어택 LV1
    {		
		ID 			=	SID_LEY02_JUMPATK_LV1,
		GroupID 	=	SG_LEY02_JUMPATK,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_JUMPATK_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	TRUE,
		LearnSP 	=	1,

		TitleID 	=	1252,
		DescID 		=	1900,
		ControlDesc	=	403,

		UI_Pos_x 	=	186,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 점프 어택 LV2
    {		
		ID 			=	SID_LEY02_JUMPATK_LV2,
		GroupID 	=	SG_LEY02_JUMPATK,
		PreList 	=	{SID_LEY02_JUMPATK_LV1, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1253,
		DescID 		=	1895,
		ControlDesc	=	403,

		UI_Pos_x 	=	186,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 대쉬 어택 LV1
    {		
		ID 			=	SID_LEY02_DASHATK_LV1,
		GroupID 	=	SG_LEY02_DASHATK,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_DASHATK_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	TRUE,
		LearnSP 	=	1,

		TitleID 	=	1857,
		DescID 		=	1901,
		ControlDesc	=	138,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 대쉬 어택 LV2
    {		
		ID 			=	SID_LEY02_DASHATK_LV2,
		GroupID 	=	SG_LEY02_DASHATK,
		PreList 	=	{SID_LEY02_DASHATK_LV1, },
		NextList 	=	{SID_LEY02_DASHATK_SIZE, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1858,
		DescID 		=	1896,
		ControlDesc	=	1904,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 대쉬 어택 거리 증가
    {		
		ID 			=	SID_LEY02_DASHATK_SIZE,
		GroupID 	=	SG_LEY02_DASHATK,
		PreList 	=	{SID_LEY02_DASHATK_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1859,
		DescID 		=	1897,
		ControlDesc	=	430,

		UI_Pos_x 	=	286,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- AP 회복 속도 증가 LV1
    {		
		ID 			=	SID_LEY02_AP_UP_LV1,
		GroupID 	=	SG_LEY02_AP_UP,
		PreList 	=	{},
		NextList 	=	{SID_LEY02_AP_UP_LV2, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1106,
		DescID 		=	1156,
		ControlDesc	=	430,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- AP 회복 속도 증가 LV2
    {		
		ID 			=	SID_LEY02_AP_UP_LV2,
		GroupID 	=	SG_LEY02_AP_UP,
		PreList 	=	{SID_LEY02_AP_UP_LV1, },
		NextList 	=	{SID_LEY02_AP_UP_LV3, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1107,
		DescID 		=	1157,
		ControlDesc	=	430,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- AP 회복 속도 증가 LV3
    {		
		ID 			=	SID_LEY02_AP_UP_LV3,
		GroupID 	=	SG_LEY02_AP_UP,
		PreList 	=	{SID_LEY02_AP_UP_LV2, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1257,
		DescID 		=	1315,
		ControlDesc	=	430,

		UI_Pos_x 	=	386,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- 시작 시 AP 보유 LV4
    {		
		ID 			=	SID_LEY01_START_AP_LV4,
		GroupID 	=	SG_LEY02_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV3, },
		NextList 	=	{SID_LEY01_START_AP_LV5, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1260,
		DescID 		=	1318,
		ControlDesc	=	430,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },

	-- 시작 시 AP 보유 LV5
    {		
		ID 			=	SID_LEY01_START_AP_LV5,
		GroupID 	=	SG_LEY02_START_AP,
		PreList 	=	{SID_LEY01_START_AP_LV4, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1261,
		DescID 		=	1319,
		ControlDesc	=	430,

		UI_Pos_x 	=	486,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
	-- 크리티컬 증가 LV4
    {		
		ID 			=	SID_LEY01_CRITICAL_LV4,
		GroupID 	=	SG_LEY02_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV3, },
		NextList 	=	{SID_LEY01_CRITICAL_LV5, },
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1263,
		DescID 		=	1898,
		ControlDesc	=	430,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
    },	

	-- 크리티컬 증가 LV5
    {		
		ID 			=	SID_LEY01_CRITICAL_LV5,
		GroupID 	=	SG_LEY02_CRITICAL,
		PreList 	=	{SID_LEY01_CRITICAL_LV4, },
		NextList 	=	{},
		CharLv 		=	0,
		CharType 	=	12,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	1264,
		DescID 		=	1899,
		ControlDesc	=	430,

		UI_Pos_x 	=	586,
		UI_Pos_y 	=	249 + Y_GAP_QUICKSLOT,
		UI_LineType =	2,		

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
--------------------------------------------------------------------------------------------------------------------------
------------ 루퍼스 [바운티헌터]
--------------------------------------------------------------------------------------------------------------------------	
    -- 펄버라이즈
	{		
		ID 			=	SID_LUPUS_SPECIAL_PULVERIZE,
		GroupID 	=	SG_LUPUS_SPECIAL_PULVERIZE,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		TitleID 	=	1905,
		DescID 		=	1948,
		ControlDesc	=	127,

		UI_Pos_x 	=	250,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LUPUS1_SPECIAL1_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 트윈 투쓰
    {		
		ID 			=	SID_LUPUS_SPECIAL_TWINTOOTH,
		GroupID 	=	SG_LUPUS_SPECIAL_TWINTOOTH,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		TitleID 	=	1906,
		DescID 		=	1949,
		ControlDesc	=	127,

		UI_Pos_x 	=	250 + 100,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LUPUS1_SPECIAL2,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
	-- 메이크 잇 레인
    {		
		ID 			=	SID_LUPUS_SPECIAL_MAKEITRAIN,
		GroupID 	=	SG_LUPUS_SPECIAL_MAKEITRAIN,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		TitleID 	=	1907,
		DescID 		=	1950,
		ControlDesc	=	127,

		UI_Pos_x 	=	250 + 100 + 100,
		UI_Pos_y 	=	200,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = FALSE,
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LUPUS1_SPECIAL3,
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
	-- 레이크
    {		
		ID 			=	SID_LUPUS_NORMAL_RAKE,
		GroupID 	=	SG_LUPUS_NORMAL_RAKE,
		PreList 	=	{ },
		NextList 	=	{ SID_LUPUS_SPEED_1, },
		CharLv 		=	2,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1908,
		DescID 		=	1951,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	290,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_RAKE,
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
	-- 이속 증가 1
    {		
		ID 			=	SID_LUPUS_SPEED_1,
		GroupID 	=	SG_LUPUS_SPEED_1,
		PreList 	=	{ SID_LUPUS_NORMAL_RAKE, },
		NextList 	=	{ SID_LUPUS_STAND_UP_MOVE, },
		CharLv 		=	4,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1909,
		DescID 		=	1952,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	290,
		UI_LineType =	19,

		LockGroup = -1,
		Passive = TRUE,
    },
	
	-- 기상 회피
    {		
		ID 			=	SID_LUPUS_STAND_UP_MOVE,
		GroupID 	=	SG_LUPUS_STAND_UP_MOVE,
		PreList 	=	{ SID_LUPUS_SPEED_1, },
		NextList 	=	{ SID_LUPUS_SPECIAL_EMPOWER, },
		CharLv 		=	6,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1910,
		DescID 		=	1953,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	290,
		UI_LineType =	20,

		LockGroup = -1,
    },
	
	-- 임파워
    {		
		ID 			=	SID_LUPUS_SPECIAL_EMPOWER,
		GroupID 	=	SG_LUPUS_SPECIAL_EMPOWER,
		PreList 	=	{ SID_LUPUS_STAND_UP_MOVE, },
		NextList 	=	{ SID_LUPUS_SPECIAL_EMPOWER_DMG, },
		CharLv 		=	8,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1911,
		DescID 		=	1954,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	380,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LUPUS1_EMPOWER,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 임파워 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_EMPOWER_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_EMPOWER_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_EMPOWER, },
		NextList 	=	{ SID_LUPUS_NORMAL_TRAMPLE, },
		CharLv 		=	10,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1912,
		DescID 		=	1955,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	380,
		UI_LineType =	19,		

		LockGroup = -1,
    },
	
	-- 트램플
    {		
		ID 			=	SID_LUPUS_NORMAL_TRAMPLE,
		GroupID 	=	SG_LUPUS_NORMAL_TRAMPLE,
		PreList 	=	{ SID_LUPUS_SPECIAL_EMPOWER_DMG, },
		NextList 	=	{ SID_LUPUS_EYETOOTH_DMG, },
		CharLv 		=	12,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1913,
		DescID 		=	1956,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	380,
		UI_LineType =	21,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_TRAMPLE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
	-- 아이투스 데미지 증가
    {		
		ID 			=	SID_LUPUS_EYETOOTH_DMG,
		GroupID 	=	SG_LUPUS_EYETOOTH_DMG,
		PreList 	=	{ SID_LUPUS_NORMAL_TRAMPLE, },
		NextList 	=	{ SID_LUPUS_MP_1, },
		CharLv 		=	14,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1914,
		DescID 		=	1957,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },

	-- MP 회복 증가 1
    {		
		ID 			=	SID_LUPUS_MP_1,
		GroupID 	=	SG_LUPUS_MP_1,
		PreList 	=	{ SID_LUPUS_EYETOOTH_DMG, },
		NextList 	=	{ SID_LUPUS_SPECIAL_BULLETTIME, },
		CharLv 		=	16,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1915,
		DescID 		=	1958,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 블렛 타임
    {		
		ID 			=	SID_LUPUS_SPECIAL_BULLETTIME,
		GroupID 	=	SG_LUPUS_SPECIAL_BULLETTIME,
		PreList 	=	{ SID_LUPUS_MP_1, },
		NextList 	=	{ SID_LUPUS_SPECIAL_BULLETTIME_DMG, },
		CharLv 		=	18,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1916,
		DescID 		=	1959,
		ControlDesc	=	127,

		UI_Pos_x 	=	170 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LUPUS1_BULLE_TIME,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
	-- 블렛 타임 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_BULLETTIME_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_BULLETTIME_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_BULLETTIME, },
		NextList 	=	{ SID_LUPUS_NORMAL_NETHERBLADE_1, },
		CharLv 		=	20,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1917,
		DescID 		=	1960,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
	-- 네더 블레이드
    {		
		ID 			=	SID_LUPUS_NORMAL_NETHERBLADE_1,
		GroupID 	=	SG_LUPUS_NORMAL_NETHERBLADE_1,
		PreList 	=	{ SID_LUPUS_SPECIAL_BULLETTIME_DMG, },
		NextList 	=	{ SID_LUPUS_SPEED_2, },
		CharLv 		=	22,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1918,
		DescID 		=	1961,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_NETHER_BLADE_A,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
	-- 이속 증가 2
    {		
		ID 			=	SID_LUPUS_SPEED_2,
		GroupID 	=	SG_LUPUS_SPEED_2,
		PreList 	=	{ SID_LUPUS_NORMAL_NETHERBLADE_1, },
		NextList 	=	{ SID_LUPUS_JUMP, },
		CharLv 		=	24,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1919,
		DescID 		=	1962,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 점프 증가
    {		
		ID 			=	SID_LUPUS_JUMP,
		GroupID 	=	SG_LUPUS_JUMP,
		PreList 	=	{ SID_LUPUS_SPEED_2, },
		NextList 	=	{ SID_LUPUS_SPECIAL_SOULSHACKLES, },
		CharLv 		=	26,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1920,
		DescID 		=	1963,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	22,		

		LockGroup = -1,
    },

	-- 소울 섀클즈
    {		
		ID 			=	SID_LUPUS_SPECIAL_SOULSHACKLES,
		GroupID 	=	SG_LUPUS_SPECIAL_SOULSHACKLES,
		PreList 	=	{ SID_LUPUS_JUMP, },
		NextList 	=	{ SID_LUPUS_SPECIAL_SOULSHACKLES_TIME, },
		CharLv 		=	28,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1921,
		DescID 		=	1964,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LUPUS1_SOUL_SHACKLES,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 소울 섀클즈 지속 시간 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_SOULSHACKLES_TIME,
		GroupID 	=	SG_LUPUS_SPECIAL_SOULSHACKLES_TIME,
		PreList 	=	{ SID_LUPUS_SPECIAL_SOULSHACKLES, },
		NextList 	=	{ SID_LUPUS_NORMAL_NETHERBLADE_2, },
		CharLv 		=	30,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1922,
		DescID 		=	1965,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,		

		LockGroup = -1,
    },
	
	-- 네더 블레이드 2
    {		
		ID 			=	SID_LUPUS_NORMAL_NETHERBLADE_2,
		GroupID 	=	SG_LUPUS_NORMAL_NETHERBLADE_1,
		PreList 	=	{ SID_LUPUS_SPECIAL_SOULSHACKLES_TIME, },
		NextList 	=	{ SID_LUPUS_NORMAL_NETHERBLADE_DMG, },
		CharLv 		=	32,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1923,
		DescID 		=	1966,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	23,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_NETHER_BLADE_B,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
			
			ComboSkill = {  {SID_LUPUS_NORMAL_NETHERBLADE_1, 0, 45, 40, },
							{SID_LUPUS_NORMAL_NETHERBLADE_2, 0, 0, 0, }, },
		},
    },
	
	-- 네더 블레이드 데미지 증가
    {		
		ID 			=	SID_LUPUS_NORMAL_NETHERBLADE_DMG,
		GroupID 	=	SG_LUPUS_NORMAL_NETHERBLADE_DMG,
		PreList 	=	{ SID_LUPUS_NORMAL_NETHERBLADE_2, },
		NextList 	=	{ SID_LUPUS_MP_2, },
		CharLv 		=	34,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1924,
		DescID 		=	1967,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- MP 회복 증가 2
    {		
		ID 			=	SID_LUPUS_MP_2,
		GroupID 	=	SG_LUPUS_MP_2,
		PreList 	=	{ SID_LUPUS_NORMAL_NETHERBLADE_DMG, },
		NextList 	=	{ SID_LUPUS_SPECIAL_CRESCENTSWEEP, },
		CharLv 		=	36,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1925,
		DescID 		=	1968,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 크리센트 스윕
    {		
		ID 			=	SID_LUPUS_SPECIAL_CRESCENTSWEEP,
		GroupID 	=	SG_LUPUS_SPECIAL_CRESCENTSWEEP,
		PreList 	=	{ SID_LUPUS_MP_2, },
		NextList 	=	{ SID_LUPUS_SPECIAL_CRESCENTSWEEP_DMG, },
		CharLv 		=	38,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1926,
		DescID 		=	1969,
		ControlDesc	=	127,

		UI_Pos_x 	=	170 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LUPUS1_CRESCENT_SWEEP,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
	-- 크리센트 스윕 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_CRESCENTSWEEP_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_CRESCENTSWEEP_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_CRESCENTSWEEP, },
		NextList 	=	{ SID_LUPUS_NORMAL_RUPTURE, },
		CharLv 		=	40,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1927,
		DescID 		=	1970,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
	-- 럽쳐
    {		
		ID 			=	SID_LUPUS_NORMAL_RUPTURE,
		GroupID 	=	SG_LUPUS_NORMAL_RUPTURE,
		PreList 	=	{ SID_LUPUS_SPECIAL_CRESCENTSWEEP_DMG, },
		NextList 	=	{ SID_LUPUS_SPEED_3, },
		CharLv 		=	42,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1928,
		DescID 		=	1971,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_RUPTURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
			
			ComboSkill = {  {SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 0, 0, }
							},							
		},
    },
	
	-- 이속 증가 3
    {		
		ID 			=	SID_LUPUS_SPEED_3,
		GroupID 	=	SG_LUPUS_SPEED_3,
		PreList 	=	{ SID_LUPUS_NORMAL_RUPTURE, },
		NextList 	=	{ SID_LUPUS_NORMAL_RUPTURE_TIME, },
		CharLv 		=	44,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1929,
		DescID 		=	1972,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 럽쳐 사용 횟수 증가
    {		
		ID 			=	SID_LUPUS_NORMAL_RUPTURE_TIME,
		GroupID 	=	SG_LUPUS_NORMAL_RUPTURE,
		PreList 	=	{ SID_LUPUS_SPEED_3, },
		NextList 	=	{ SID_LUPUS_SPECIAL_HOWLINGGALE, },
		CharLv 		=	46,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1930,
		DescID 		=	1973,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	22,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_RUPTURE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
			
			-- SID / 키 입력 허용 시작 FRM / 키 입력 허용 끝 프레임 / 모션 Change 프레임
			ComboSkill = {  {SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 0, 0, },
							},
		},
    },
	
	-- 하울링 게일
    {		
		ID 			=	SID_LUPUS_SPECIAL_HOWLINGGALE,
		GroupID 	=	SG_LUPUS_SPECIAL_HOWLINGGALE,
		PreList 	=	{ SID_LUPUS_NORMAL_RUPTURE_TIME, },
		NextList 	=	{ SID_LUPUS_SPECIAL_HOWLINGGALE_DMG, },
		CharLv 		=	48,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1931,
		DescID 		=	1974,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_LUPUS1_HOWLING_GALE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
	-- 하울링 게일 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_HOWLINGGALE_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_HOWLINGGALE_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_HOWLINGGALE, },
		NextList 	=	{ SID_LUPUS_NORMAL_IMPALE, },
		CharLv 		=	50,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1932,
		DescID 		=	1975,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 임페일
    {		
		ID 			=	SID_LUPUS_NORMAL_IMPALE,
		GroupID 	=	SG_LUPUS_NORMAL_IMPALE,
		PreList 	=	{ SID_LUPUS_SPECIAL_HOWLINGGALE_DMG, },
		NextList 	=	{ SID_LUPUS_NORMAL_RUPTURE_DMG, },
		CharLv 		=	52,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1933,
		DescID 		=	1976,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	23,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_IMPALE,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
	
	-- 럽쳐 데미지 증가
    {		
		ID 			=	SID_LUPUS_NORMAL_RUPTURE_DMG,
		GroupID 	=	SG_LUPUS_NORMAL_RUPTURE_DMG,
		PreList 	=	{ SID_LUPUS_NORMAL_IMPALE, },
		NextList 	=	{ SID_LUPUS_MP_3, },
		CharLv 		=	54,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1934,
		DescID 		=	1977,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- MP 회복 증가 3
    {		
		ID 			=	SID_LUPUS_MP_3,
		GroupID 	=	SG_LUPUS_MP_3,
		PreList 	=	{ SID_LUPUS_NORMAL_RUPTURE_DMG, },
		NextList 	=	{ SID_LUPUS_SPECIAL_BLOODYGUST, },
		CharLv 		=	56,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1935,
		DescID 		=	1978,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 블러디 거스트
    {		
		ID 			=	SID_LUPUS_SPECIAL_BLOODYGUST,
		GroupID 	=	SG_LUPUS_SPECIAL_BLOODYGUST,
		PreList 	=	{ SID_LUPUS_MP_3, },
		NextList 	=	{ SID_LUPUS_SPECIAL_BLOODYGUST_DMG, },
		CharLv 		=	58,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1936,
		DescID 		=	1979,
		ControlDesc	=	127,

		UI_Pos_x 	=	170 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LUPUS1_BLOODY_GUST,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
	-- 블러디 거스트 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_BLOODYGUST_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_BLOODYGUST_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_BLOODYGUST, },
		NextList 	=	{ SID_LUPUS_NORMAL_SOULARBITER, },
		CharLv 		=	60,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1937,
		DescID 		=	1980,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
	-- 소울 아비터
    {		
		ID 			=	SID_LUPUS_NORMAL_SOULARBITER,
		GroupID 	=	SG_LUPUS_NORMAL_SOULARBITER,
		PreList 	=	{ SID_LUPUS_SPECIAL_BLOODYGUST_DMG, },
		NextList 	=	{ SID_LUPUS_SPEED_4, },
		CharLv 		=	62,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1938,
		DescID 		=	1981,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_SOUL_ARBITER,			
			CoolTime = 15,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 15,
		},
    },
	
	-- 이속 증가 4
    {		
		ID 			=	SID_LUPUS_SPEED_4,
		GroupID 	=	SG_LUPUS_SPEED_4,
		PreList 	=	{ SID_LUPUS_NORMAL_SOULARBITER, },
		NextList 	=	{ SID_LUPUS_NORMAL_SOULARBITER_TIME, },
		CharLv 		=	64,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1939,
		DescID 		=	1982,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 소울 아비터 사용 횟수 증가
    {		
		ID 			=	SID_LUPUS_NORMAL_SOULARBITER_TIME,
		GroupID 	=	SG_LUPUS_NORMAL_SOULARBITER_TIME,
		PreList 	=	{ SID_LUPUS_SPEED_4, },
		NextList 	=	{ SID_LUPUS_SPECIAL_SHATTER, },
		CharLv 		=	66,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1940,
		DescID 		=	1983,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	22,

		LockGroup = -1,
    },
	
	-- 섀터
    {		
		ID 			=	SID_LUPUS_SPECIAL_SHATTER,
		GroupID 	=	SG_LUPUS_SPECIAL_SHATTER,
		PreList 	=	{ SID_LUPUS_NORMAL_SOULARBITER_TIME, },
		NextList 	=	{ SID_LUPUS_SPECIAL_SHATTER_DMG, },
		CharLv 		=	68,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1941,
		DescID 		=	1984,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_LUPUS1_SHATTER,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 섀터 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_SHATTER_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_SHATTER_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_SHATTER, },
		NextList 	=	{ SID_LUPUS_NORMAL_RAVAGE, },
		CharLv 		=	70,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1942,
		DescID 		=	1985,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 래비지
    {		
		ID 			=	SID_LUPUS_NORMAL_RAVAGE,
		GroupID 	=	SG_LUPUS_NORMAL_RAVAGE,
		PreList 	=	{ SID_LUPUS_SPECIAL_SHATTER_DMG, },
		NextList 	=	{ SID_LUPUS_NORMAL_SOULARBITER_DMG, },
		CharLv 		=	72,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1943,
		DescID 		=	1986,
		ControlDesc	=	127,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	23,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_LUPUS1_RAVAGE,			
			CoolTime = 15,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 15,
		},
    },
	
	-- 소울 아비터 데미지 증가
    {		
		ID 			=	SID_LUPUS_NORMAL_SOULARBITER_DMG,
		GroupID 	=	SG_LUPUS_NORMAL_SOULARBITER_DMG,
		PreList 	=	{ SID_LUPUS_NORMAL_RAVAGE, },
		NextList 	=	{ SID_LUPUS_MP_4, },
		CharLv 		=	74,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1944,
		DescID 		=	1987,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- MP 회복 증가 4
    {		
		ID 			=	SID_LUPUS_MP_4,
		GroupID 	=	SG_LUPUS_MP_4,
		PreList 	=	{ SID_LUPUS_NORMAL_SOULARBITER_DMG, },
		NextList 	=	{ SID_LUPUS_SPECIAL_EXPUNGE, },
		CharLv 		=	76,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1945,
		DescID 		=	1988,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- 익스펀지
    {		
		ID 			=	SID_LUPUS_SPECIAL_EXPUNGE,
		GroupID 	=	SG_LUPUS_SPECIAL_EXPUNGE,
		PreList 	=	{ SID_LUPUS_MP_4, },
		NextList 	=	{ SID_LUPUS_SPECIAL_EXPUNGE_DMG, },
		CharLv 		=	78,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1946,
		DescID 		=	1989,
		ControlDesc	=	127,

		UI_Pos_x 	=	170 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 2.5,
			Motion = MID_LUPUS1_EXPUNGE,			
			CoolTime = 30,
			
			MonsterModeMana = 2.3,
			MonsterModeCoolTime = 26,
		},
    },
	
	-- 익스펀지 데미지 증가
    {		
		ID 			=	SID_LUPUS_SPECIAL_EXPUNGE_DMG,
		GroupID 	=	SG_LUPUS_SPECIAL_EXPUNGE_DMG,
		PreList 	=	{ SID_LUPUS_SPECIAL_EXPUNGE, },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	13,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	4,

		TitleID 	=	1947,
		DescID 		=	1990,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
--------------------------------------------------------------------------------------------------------------------------
------------ 레비아탄 - 디오2차
--------------------------------------------------------------------------------------------------------------------------
    --타임 마블 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_TIME_MARBLE,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO1_DEATHSTAR_SPECIAL_LV3, SID_DIO2_CHAOTICSPARK_SPECIAL_LV5, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon908.dds",
		TitleID 	=	2012,
		DescID 		=	2072,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	3,

		LockGroup = -1,		
    },		

    --타임 마블 지속 시간 LV1
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_TIME_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon909.dds",
		TitleID 	=	2013,
		DescID 		=	2073,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	12,

		LockGroup = -1,		
    },		

    --타임 마블 지속 시간 LV2
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_TIME_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV1, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon910.dds",
		TitleID 	=	2014,
		DescID 		=	2074,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

    --타임 마블 지속 시간 LV3
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_TIME_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV2, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon911.dds",
		TitleID 	=	2015,
		DescID 		=	2075,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,		
    },

    --타임 마블 강화 LV1
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_UPGRADE_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_TIME_LV3, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon912.dds",
		TitleID 	=	2016,
		DescID 		=	2076,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	17,

		LockGroup = -1,		
    },

    --타임 마블 강화 LV2
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_UPGRADE_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV1, },
		NextList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon913.dds",
		TitleID 	=	2017,
		DescID 		=	2077,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --타임 마블 강화 LV3
    {		
		ID 			=	SID_DIO3_TIME_MARBLE_UPGRADE_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_TIME_MARBLE_UPGRADE_LV2, },
		NextList 	=	{SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon914.dds",
		TitleID 	=	2018,
		DescID 		=	2078,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	186,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },
	
	-- 터네이셔스 홀드 LV1
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO2_BLOCKER_TIMEUP_LV2, SID_DIO3_TIME_MARBLE_UPGRADE_LV3, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon915.dds",
		TitleID 	=	2019,
		DescID 		=	2079,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 터네이셔스 홀드 LV2
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV1, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon916.dds",
		TitleID 	=	2020,
		DescID 		=	2080,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 터네이셔스 홀드 LV3
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV2, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon917.dds",
		TitleID 	=	2021,
		DescID 		=	2081,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 터네이셔스 홀드 LV4
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV3, },
		NextList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon918.dds",
		TitleID 	=	2022,
		DescID 		=	2082,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 터네이셔스 홀드 LV5
    {		
		ID 			=	SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_TENACIOUS_HOLD_SPECIAL,
		PreList 	=	{ SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV4, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon919.dds",
		TitleID 	=	2023,
		DescID 		=	2083,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	260,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_TENACIOUS_HOLD,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 프라우드 스윙 LV1
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO2_CHARGING_DS_W_LV5, SID_DIO3_TENACIOUS_HOLD_SPECIAL_LV5, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon920.dds",
		TitleID 	=	2024,
		DescID 		=	2084,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV2
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV1, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon921.dds",
		TitleID 	=	2025,
		DescID 		=	2085,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV3
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV2, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon922.dds",
		TitleID 	=	2026,
		DescID 		=	2086,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV4
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV3, },
		NextList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon923.dds",
		TitleID 	=	2027,
		DescID 		=	2087,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 프라우드 스윙 LV5
    {		
		ID 			=	SID_DIO3_PROUD_SWING_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_PROUD_SWING_SPECIAL,
		PreList 	=	{ SID_DIO3_PROUD_SWING_SPECIAL_LV4, },
		NextList 	=	{ SID_DIO3_SPELL_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon924.dds",
		TitleID 	=	2028,
		DescID 		=	2088,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	334,
		UI_LineType =	3,
		
		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_PROUD_SWING,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

    --스펠 마블 (AP 소모량 감소)
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_PROUD_SWING_SPECIAL_LV5, SID_DIO3_COUNT_COOLTIME_LV5, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon925.dds",
		TitleID 	=	2029,
		DescID 		=	2089,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	3,

		LockGroup = -1,
    },

    --스펠 마블 지속 시간 LV1
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_TIME_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon926.dds",
		TitleID 	=	2030,
		DescID 		=	2090,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
    },	

    --스펠 마블 지속 시간 LV2
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_TIME_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV1, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon927.dds",
		TitleID 	=	2031,
		DescID 		=	2091,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --스펠 마블 지속 시간 LV3
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_TIME_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV2, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon928.dds",
		TitleID 	=	2032,
		DescID 		=	2092,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	418,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --스펠 마블 강화 LV1
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_UPGRADE_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_TIME_LV3, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon929.dds",
		TitleID 	=	2033,
		DescID 		=	2093,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
    },

    --스펠 마블 강화 LV2
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_UPGRADE_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV1, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon930.dds",
		TitleID 	=	2034,
		DescID 		=	2094,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --스펠 마블 강화 LV3
    {		
		ID 			=	SID_DIO3_SPELL_MARBLE_UPGRADE_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV2, },
		NextList 	=	{SID_DIO3_STICK_LANCE_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon931.dds",
		TitleID 	=	2035,
		DescID 		=	2095,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	573,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- 스틱 랜스 LV1
    {		
		ID 			=	SID_DIO3_STICK_LANCE_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_STICK_LANCE_SPECIAL,
		PreList 	=	{SID_DIO3_SPELL_MARBLE_UPGRADE_LV3, },
		NextList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	2036,
		DescID 		=	2096,
		ControlDesc	=	127,

		Img 		=	"SkillIcon932.dds",
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_STICK_LANCE,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },	

	-- 스틱 랜스 LV2 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_STICK_LANCE_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_STICK_LANCE_SPECIAL,
		PreList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV1, },
		NextList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		TitleID 	=	2037,
		DescID 		=	2097,
		ControlDesc	=	127,

		Img 		=	"SkillIcon933.dds",
		UI_Pos_x 	=	495,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_STICK_LANCE,			
			CoolTime = 17,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 15,
		},
    },

	-- 스틱 랜스 LV3 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_STICK_LANCE_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_STICK_LANCE_SPECIAL,
		PreList 	=	{ SID_DIO3_STICK_LANCE_SPECIAL_LV2, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon934.dds",
		TitleID 	=	2038,
		DescID 		=	2098,
		ControlDesc	=	127,

		UI_Pos_x 	=	495,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_STICK_LANCE,			
			CoolTime = 14,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 13,
		},
    },

    -- 콤보 스피드업 LV1
    {		
		ID 			=	SID_DIO3_COMBO_SPEED_UP_LV1,
		GroupID 	=	SG_DIO3_COMBO_SPEED,
		PreList 	=	{SID_DIO2_KILLZONE_SPECIAL_LV5, SID_DIO3_COMBO_ATK_DMG_LV2, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV1,},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon900.dds",
		TitleID 	=	2006,
		DescID 		=	2066,
		ControlDesc	=	1169,

		UI_Pos_x 	=	220,--dio2
		UI_Pos_y 	=	186,--dio2
		UI_LineType =	3,

		LockGroup = -1,
    },

	-- 그립 LV1
    {		
		ID 			=	SID_DIO3_GRIP_NORMAL_LV1,
		GroupID 	=	SG_DIO3_GRIP_NORMAL,
		PreList 	=	{SID_DIO3_COMBO_SPEED_UP_LV1, SID_DIO3_DASH_ATK_DMG_LV2, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon892.dds",
		TitleID 	=	1998,
		DescID 		=	2058,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO3_GRIP,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },

	-- 그립 LV2 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_GRIP_NORMAL_LV2,
		GroupID 	=	SG_DIO3_GRIP_NORMAL,
		PreList 	=	{SID_DIO3_GRIP_NORMAL_LV1, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon893.dds",
		TitleID 	=	1999,
		DescID 		=	2059,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO3_GRIP,			
			CoolTime = 4,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 4,
		},
    },

	-- 그립 LV3 (쿨 타임 감소)
    {		
		ID 			=	SID_DIO3_GRIP_NORMAL_LV3,
		GroupID 	=	SG_DIO3_GRIP_NORMAL,
		PreList 	=	{SID_DIO3_GRIP_NORMAL_LV2, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon894.dds",
		TitleID 	=	2000,
		DescID 		=	2060,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	260,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_DIO3_GRIP,			
			CoolTime = 3,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 3,
		},
    },

	-- 쿨 스파이럴 LV1 (1단 필살기)
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_GRIP_NORMAL_LV3, SID_DIO3_CRI_DOUBLE_DMG_LV2, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon895.dds",
		TitleID 	=	2001,
		DescID 		=	2061,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV2
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV1, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,


		Img 		=	"SkillIcon896.dds",
		TitleID 	=	2002,
		DescID 		=	2062,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV3
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV2, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon897.dds",
		TitleID 	=	2003,
		DescID 		=	2063,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV4
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV3, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon898.dds",		
		TitleID 	=	2004,
		DescID 		=	2064,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },

	-- 쿨 스파이럴 LV5
    {		
		ID 			=	SID_DIO3_COOL_SPIRAL_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_COOL_SPIRAL_SPECIAL,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV4, },
		NextList 	=	{SID_DIO3_POWER_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon899.dds",
		TitleID 	=	2005,
		DescID 		=	2065,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	334,
		UI_LineType =	3,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 0.8,
			Motion = MID_DIO3_COOL_SPIRAL_A,			
			CoolTime = 10,
			
			MonsterModeMana = 0.7,
			MonsterModeCoolTime = 10,
		},
    },
	
	--파워 마블 (공격력 증가)
	{		
		ID 			=	SID_DIO3_POWER_MARBLE,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV5, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon885.dds",
		TitleID 	=	1991,
		DescID 		=	2051,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	220,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	3,

		LockGroup = -1,
    },

    --파워 마블 지속 시간 LV1
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_TIME_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE, SID_DIO1_AP_SPEED_LV2, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon886.dds",
		TitleID 	=	1992,
		DescID 		=	2052,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	143,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
    },	

    --파워 마블 지속 시간 LV2
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_TIME_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV1, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon887.dds",
		TitleID 	=	1993,
		DescID 		=	2053,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	143,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --파워 마블 지속 시간 LV3
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_TIME_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV2, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon888.dds",
		TitleID 	=	1994,
		DescID 		=	2054,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	143,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	12,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --파워 마블 강화 LV1
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_UPGRADE_LV1,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_TIME_LV3, SID_DIO1_AP_START_LV3, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon889.dds",
		TitleID 	=	1995,
		DescID 		=	2055,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	298,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
    },

    --파워 마블 강화 LV2
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_UPGRADE_LV2,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV1, },
		NextList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon890.dds",
		TitleID 	=	1996,
		DescID 		=	2056,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	298,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

    --파워 마블 강화 LV3
    {		
		ID 			=	SID_DIO3_POWER_MARBLE_UPGRADE_LV3,
		GroupID 	=	SG_DIO3_MARBLE,
		PreList 	=	{SID_DIO3_POWER_MARBLE_UPGRADE_LV2, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon891.dds",
		TitleID 	=	1997,
		DescID 		=	2057,
		ControlDesc	=	1169,

		
		UI_Pos_x 	=	298,
		UI_Pos_y 	=	408,--dio
		UI_LineType =	17,

		LockGroup = -1,
		Overlapped  = TRUE,
    },

	-- 이블 에디 LV1
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV1,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO2_BLACKSPACE_APDOWN_NORMAL_LV2, SID_DIO3_POWER_MARBLE_UPGRADE_LV3, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon903.dds",
		TitleID 	=	2007,
		DescID 		=	2067,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV2
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV2,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV1, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV3, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon904.dds",
		TitleID 	=	2008,
		DescID 		=	2068,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV3
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV3,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV2, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon905.dds",
		TitleID 	=	2009,
		DescID 		=	2069,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV4
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV4,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV3, },
		NextList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon906.dds",
		TitleID 	=	2010,
		DescID 		=	2070,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },

	-- 이블 에디 LV5
    {		
		ID 			=	SID_DIO3_EVIL_EDDY_SPECIAL_LV5,
		GroupID 	=	SG_DIO3_EVIL_EDDY_SPECIAL,
		PreList 	=	{SID_DIO3_EVIL_EDDY_SPECIAL_LV4, },
		NextList 	=	{},
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon907.dds",
		TitleID 	=	2011,
		DescID 		=	2071,
		ControlDesc	=	127,

		UI_Pos_x 	=	220,
		UI_Pos_y 	=	482,
		UI_LineType =	-1,

		Overlapped  = TRUE,
		LockGroup = -1,						
		
		SlotSkill = 
		{
			Mana = 1.6,
			Motion = MID_DIO3_EVIL_EDDY_READY,			
			CoolTime = 20,
			
			MonsterModeMana = 1.4,
			MonsterModeCoolTime = 17,
		},
    },
	
    -- 2차전직 콤보
    {		
		ID 			=	SID_DIO3_COMBO_ATK,
		GroupID 	=	SG_DIO3_COMBO_ATK,
		PreList 	=	{SID_DIO2_COMBO_ATK_LV2, },
		NextList 	=	{SID_DIO3_COMBO_ATK_DMG_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon935.dds",
		TitleID 	=	2039,
		DescID 		=	2099,
		ControlDesc	=	221,

		UI_Pos_x 	=	205,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,
		
    },
	
    -- 2차전직 콤보 데미지 증가 LV1
    {		
		ID 			=	SID_DIO3_COMBO_ATK_DMG_LV1,
		GroupID 	=	SG_DIO3_COMBO_ATK,
		PreList 	=	{SID_DIO3_COMBO_ATK, },
		NextList 	=	{SID_DIO3_COMBO_ATK_DMG_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon936.dds",
		
		TitleID 	=	2040,
		DescID 		=	2100,
		ControlDesc	=	221,

		UI_Pos_x 	=	205,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,		
    },	
	
    -- 2차전직 콤보 데미지 증가 LV2
    {		
		ID 			=	SID_DIO3_COMBO_ATK_DMG_LV2,
		GroupID 	=	SG_DIO3_COMBO_ATK,
		PreList 	=	{SID_DIO3_COMBO_ATK_DMG_LV1, },
		NextList 	=	{SID_DIO3_COMBO_SPEED_UP_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon937.dds",
		
		TitleID 	=	2041,
		DescID 		=	2101,
		ControlDesc	=	221,

		UI_Pos_x 	=	205,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
    },
	
    -- 2차전직 대시 어택
    {		
		ID 			=	SID_DIO3_DASH_ATK,
		GroupID 	=	SG_DIO3_DASH_ATK,
		PreList 	=	{SID_DIO2_JUMP_ATK_LV2, SID_DIO2_JUMP_PLUS_LV3, },
		NextList 	=	{SID_DIO3_DASH_ATK_DMG_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon938.dds",
		TitleID 	=	2042,
		DescID 		=	2102,
		ControlDesc	=	773,

		UI_Pos_x 	=	305,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		
		
    },	
	
    -- 2차전직 대시 어택 데미지 증가 LV1
    {		
		ID 			=	SID_DIO3_DASH_ATK_DMG_LV1,
		GroupID 	=	SG_DIO3_DASH_ATK,
		PreList 	=	{SID_DIO3_DASH_ATK, },
		NextList 	=	{SID_DIO3_DASH_ATK_DMG_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon939.dds",
		
		TitleID 	=	2043,
		DescID 		=	2103,
		ControlDesc	=	773,

		UI_Pos_x 	=	305,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,	
		
    },	
	
    -- 2차전직 대시 어택 데미지 증가 LV2
    {		
		ID 			=	SID_DIO3_DASH_ATK_DMG_LV2,
		GroupID 	=	SG_DIO3_DASH_ATK,
		PreList 	=	{SID_DIO3_DASH_ATK_DMG_LV1, },
		NextList 	=	{SID_DIO3_GRIP_NORMAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon940.dds",
		TitleID 	=	2044,
		DescID 		=	2104,
		ControlDesc	=	773,

		UI_Pos_x 	=	305,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
    },		
	
    -- 2차전직 크리&더블 어택
    {		
		ID 			=	SID_DIO3_CRI_DOUBLE,
		GroupID 	=	SG_DIO3_CRI_DOUBLE,
		PreList 	=	{SID_DIO2_COMBO_PLUS_LV3, },
		NextList 	=	{SID_DIO3_CRI_DOUBLE_DMG_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon941.dds",
		
		TitleID 	=	2045,
		DescID 		=	2105,
		ControlDesc	=	130,

		UI_Pos_x 	=	405,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,			
		
    },	
	
    -- 2차전직 크리&더블 어택 데미지 증가 LV1
    {		
		ID 			=	SID_DIO3_CRI_DOUBLE_DMG_LV1,
		GroupID 	=	SG_DIO3_CRI_DOUBLE,
		PreList 	=	{SID_DIO3_CRI_DOUBLE, },
		NextList 	=	{SID_DIO3_CRI_DOUBLE_DMG_LV2, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon942.dds",
		
		TitleID 	=	2046,
		DescID 		=	2106,
		ControlDesc	=	130,

		UI_Pos_x 	=	405,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
	
    },		
	
    -- 2차전직 크리&더블 어택 데미지 증가 LV2
    {		
		ID 			=	SID_DIO3_CRI_DOUBLE_DMG_LV2,
		GroupID 	=	SG_DIO3_CRI_DOUBLE,
		PreList 	=	{SID_DIO3_CRI_DOUBLE_DMG_LV1, },
		NextList 	=	{SID_DIO3_COOL_SPIRAL_SPECIAL_LV1, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,
		
		Img 		=	"SkillIcon943.dds",
		
		TitleID 	=	2047,
		DescID 		=	2107,
		ControlDesc	=	130,

		UI_Pos_x 	=	405,--dio2
		UI_Pos_y 	=	318,--dio2
		UI_LineType =	2,

		LockGroup = -1,	
		Overlapped  = TRUE,
		
    },
	
	--카운터 쿨 타임 감소 LV3
	{		
		ID 			=	SID_DIO3_COUNT_COOLTIME_LV3,
		GroupID 	=	SG_DIO3_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV2, },
		NextList 	=	{SID_DIO3_COUNT_COOLTIME_LV4, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon944.dds",
		TitleID 	=	2048,
		DescID 		=	2108,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		
    },

	--카운터 쿨 타임 감소 LV4
	{		
		ID 			=	SID_DIO3_COUNT_COOLTIME_LV4,
		GroupID 	=	SG_DIO3_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV3, },
		NextList 	=	{SID_DIO3_COUNT_COOLTIME_LV5, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon945.dds",
		TitleID 	=	2049,
		DescID 		=	2109,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		
    },

	--카운터 쿨 타임 감소 LV5
	{		
		ID 			=	SID_DIO3_COUNT_COOLTIME_LV5,
		GroupID 	=	SG_DIO3_COUNT_COOLTIME,
		PreList 	=	{SID_DIO1_COUNT_COOLTIME_LV4, },
		NextList 	=	{SID_DIO3_SPELL_MARBLE, },
		CharLv 		=	1,
		CharType 	=	10,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	1,

		Img 		=	"SkillIcon946.dds",
		TitleID 	=	2050,
		DescID 		=	2110,
		ControlDesc	=	127,

		
		UI_Pos_x 	=	505,
		UI_Pos_y 	=	318,
		UI_LineType =	2,

		LockGroup = -1,
		Overlapped  = TRUE,
		
    },
}