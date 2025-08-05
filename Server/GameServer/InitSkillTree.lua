TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
--------------------------------------------------------------------------------------------------------------------------
------------ 기사 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
    {		
        ID 	=	 SID_ELESIS1_JUMP_ATK1,
        GroupID 	=	 SG_ELESIS1_JUMPATK,
        PreList 	=	 {SID_ELESIS1_BASE_JUMPATK,},
        NextList 	=	 {},
		
        CharLv 	=	 48,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon0.dds",
        TitleID 	=	 0,
        DescID 	=	 1,
        ControlDesc	=	131,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },   		
    {		
        ID 	=	 SID_ELESIS1_SUPER_DASH,
        GroupID 	=	 SG_ELESIS1_DASH,
        PreList 	=	 {SID_ELESIS1_BASE_DASH,},
        NextList 	=	 {},
        CharLv 	=	 42,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon1.dds",
        TitleID 	=	 2,
        DescID 	=	 3,
        ControlDesc	=	140,
        		
        UI_Pos_x 	=	633,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_DASH_ATK1,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 { SID_ELESIS1_BASE_DASHATK, },
        NextList 	=	 { SID_ELESIS1_DASH_ATK2, },
        CharLv 	=	 32,
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
    {		
        ID 	=	 SID_ELESIS1_COMBO_A,
        GroupID 	=	 SG_ELESIS1_COMBO,
        PreList 	=	 {SID_ELESIS1_BASE_COMBO, },
        NextList 	=	 {},
        CharLv 	=	 13,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon3.dds",
        TitleID 	=	 6,
        DescID 	=	 7,
        ControlDesc	=	144,
        		
        UI_Pos_x 	=	290,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_CRITICAL_ATK,
        GroupID 	=	 SG_ELESIS1_CRITICAL,
        PreList 	=	 {SID_ELESIS1_BASE_CRITICAL,},
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon4.dds",
        TitleID 	=	 8,
        DescID 	=	 9,
        ControlDesc	=	137,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_DASH_ATK2,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 { SID_ELESIS1_DASH_ATK1, },
        NextList 	=	 { SID_ELESIS1_DASH_ATK3, },
        CharLv 	=	 36,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    {		
        ID 	=	 SID_ELESIS1_DASH_ATK3,
        GroupID 	=	 SG_ELESIS1_DASHATK,
        PreList 	=	 { SID_ELESIS1_DASH_ATK2, },
        NextList 	=	 {},
        CharLv 	=	 45,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon6.dds",
        TitleID 	=	 14,
        DescID 	=	 15,
        ControlDesc	=	139,
        		
        UI_Pos_x 	=	433,
        UI_Pos_y 	=	20,
        UI_LineType 	=	3,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_FAST_ATK,
        GroupID 	=	 SG_ELESIS_MAGIC_DEF,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 20,
        CharType 	=	 0,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon7.dds",
        TitleID 	=	 10,
        DescID 	=	 11,
        ControlDesc	=	149,
        		
        UI_Pos_x 	=	345,
        UI_Pos_y 	=	186,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_ATK1,
        GroupID 	=	 SG_ELESIS1_COMMAND1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 28,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon8.dds",
        TitleID 	=	 16,
        DescID 	=	 17,
        ControlDesc	=	147,
        		
        UI_Pos_x 	=	495,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_SPECIAL_ATK2,
        GroupID 	=	 SG_ELESIS1_COMMAND2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 39,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    {		
        ID 	=	 SID_ELESIS1_MEGASLASH_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_MEGASLASH_LV3, },
        CharLv 	=	 10,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon10.dds",
        TitleID 	=	 20,
        DescID 	=	 21,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_MEGASLASH_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_MEGASLASH_LV2, },
        NextList 	=	 {},
        CharLv 	=	 25,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon11.dds",
        TitleID 	=	 22,
        DescID 	=	 23,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	153,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
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
        LearnSP = 2,
		
        Img 	=	 "skillicon12.dds",
        TitleID 	=	 24,
        DescID 	=	 25,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_SWORDFIRE_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 { SID_ELESIS1_SWORDFIRE_LV2, },
        NextList 	=	 {},
        CharLv 	=	 30,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon13.dds",
        TitleID 	=	 26,
        DescID 	=	 27,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	296,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_CRITICALX_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_CRITICALX_LV3, },
        CharLv 	=	 20,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon14.dds",
        TitleID 	=	 28,
        DescID 	=	 29,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	494,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 6,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_CRITICALX_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_CRITICALX_LV2, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon15.dds",
        TitleID 	=	 30,
        DescID 	=	 31,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	557,
        UI_Pos_y 	=	439,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
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
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_SPECIAL1_LV1, },
        NextList 	=	 { SID_ELESIS1_SPECIAL1_LV3, },
        CharLv 	=	 19,
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
    {		
        ID 	=	 SID_ELESIS1_SPECIAL1_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_SPECIAL1_LV2, },
        NextList 	=	 {},
        CharLv 	=	 31,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 { SID_ELESIS1_SPECIAL2_LV1, },
        NextList 	=	 { SID_ELESIS1_SPECIAL2_LV3, },
        CharLv 	=	 22,
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
    {		
        ID 	=	 SID_ELESIS1_SPECIAL2_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 { SID_ELESIS1_SPECIAL2_LV2, },
        NextList 	=	 {},
        CharLv 	=	 33,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon21.dds",
        TitleID 	=	 42,
        DescID 	=	 43,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	206,
        UI_Pos_y 	=	362,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL2,
    },		
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_LV1,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 { SID_ELESIS1_SPECIAL3_LV2, },
        CharLv 	=	 10,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon22.dds",
        TitleID 	=	 44,
        DescID 	=	 45,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	332,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 5,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_LV2,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_SPECIAL3_LV1, },
        NextList 	=	 { SID_ELESIS1_SPECIAL3_LV3, },
        CharLv 	=	 24,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 2,
		
        Img 	=	 "skillicon23.dds",
        TitleID 	=	 46,
        DescID 	=	 47,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	269,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
    {		
        ID 	=	 SID_ELESIS1_SPECIAL3_LV3,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 { SID_ELESIS1_SPECIAL3_LV2, },
        NextList 	=	 {},
        CharLv 	=	 35,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 3,
		
        Img 	=	 "skillicon24.dds",
        TitleID 	=	 48,
        DescID 	=	 49,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	206,
        UI_Pos_y 	=	505,
        UI_LineType 	=	 0,
        
        LockGroup = BGID_ELESIS1_SPECIAL3,
    },		
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL1_A,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 38,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    --[[		
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL1_B,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 { SID_ELESIS1_NEWSPECIAL1_A, },
        NextList 	=	 {},
        CharLv 	=	 10,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    --]]	
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL2_A,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 41,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    {		
        ID 	=	 SID_ELESIS1_NEWSPECIAL3_A,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 44,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
        ControlDesc	=	130,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
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
    {		
        ID 	=	 SID_ELESIS1_BASE_SPECIAL1,
        GroupID 	=	 SG_ELESIS1_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SPECIAL1_LV1,SID_ELESIS1_MEGASLASH_LV2, SID_ELESIS1_NEWSPECIAL1_A, },
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
    {		
        ID 	=	 SID_ELESIS1_BASE_SPECIAL2,
        GroupID 	=	 SG_ELESIS1_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SWORDFIRE_LV2,SID_ELESIS1_SPECIAL2_LV1, SID_ELESIS1_NEWSPECIAL2_A, },
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
    {		
        ID 	=	 SID_ELESIS1_BASE_SPECIAL3,
        GroupID 	=	 SG_ELESIS1_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_CRITICALX_LV2, SID_ELESIS1_SPECIAL3_LV1, SID_ELESIS1_NEWSPECIAL3_A, },
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
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_UP ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {SID_ELESIS1_BASE_COMBO_PLUS,},
        NextList 	=	 {SID_ELESIS1_COMBO_A_UP2, },
        CharLv 	=	 14,
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
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_DOWN  ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS2,
        PreList 	=	 {SID_ELESIS1_BASE_COMBO_DOWN,},
        NextList 	=	 {SID_ELESIS1_BASE_COMBO_DOWN2, },
        CharLv 	=	 15,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon43.dds",
        TitleID 	=	 86,
        DescID 	=	 87,
        ControlDesc	=	146,
        		
        UI_Pos_x 	=	290,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 8,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_UP2 ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {SID_ELESIS1_COMBO_A_UP },
        NextList 	=	 {SID_ELESIS1_BASE_COMBO_UP2,},
        CharLv 	=	 17,
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
    {		
        ID 	=	 SID_ELESIS1_COMBO_A_DOWN2 ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS2,
        PreList 	=	 {SID_ELESIS1_BASE_COMBO_DOWN2, },
        NextList 	=	 {},
        CharLv 	=	 26,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon45.dds",
        TitleID 	=	 90,
        DescID 	=	 91,
        ControlDesc	=	146,
        		
        UI_Pos_x 	=	352,
        UI_Pos_y 	=	20,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		 
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO_DOWN,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS2,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_COMBO_A_DOWN, },
        CharLv 	=	 5,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO_UP2,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS1,
        PreList 	=	 {SID_ELESIS1_COMBO_A_UP2,},
        NextList 	=	 {},
        CharLv 	=	 17,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon47.dds",
        TitleID 	=	 96,
        DescID 	=	 97,
        ControlDesc	=	134,
        		
        UI_Pos_x 	=	152,
        UI_Pos_y 	=	20,
        UI_LineType 	=	0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_BASE_COMBO_DOWN2  ,
        GroupID 	=	 SG_ELESIS1_COMBO_PLUS2,
        PreList 	=	 {SID_ELESIS1_COMBO_A_DOWN, },
        NextList 	=	 {SID_ELESIS1_COMBO_A_DOWN2, },
        CharLv 	=	 22,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
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
        UI_Pos_y 	=	330,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_SUPER_BERSERK_LV1  ,
        GroupID 	=	 SG_ELESIS_BERSERK,
        PreList 	=	 {},
        NextList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV2, },
        CharLv 	=	 30,
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
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_SUPER_BERSERK_LV2  ,
        GroupID 	=	 SG_ELESIS_BERSERK,
        PreList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV1, },
        NextList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV3,},
        CharLv 	=	 47,
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
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },		
    {		
        ID 	=	 SID_ELESIS1_SUPER_BERSERK_LV3  ,
        GroupID 	=	 SG_ELESIS_BERSERK,
        PreList 	=	 {SID_ELESIS1_SUPER_BERSERK_LV2,},
        NextList 	=	 {},
        CharLv 	=	 57,
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
        UI_Pos_y 	=	473,
        UI_LineType 	=	 0,
        
        LockGroup = -1,
    },	
    {		
        ID 	=	 SID_ELESIS1_DOUBLE_ATK ,
        GroupID 	=	 SG_ELESIS1_DOUBLE,
        PreList 	=	 {SID_ELESIS1_BASE_DOUBLE},
        NextList 	=	 {},
        CharLv 	=	 29,
        CharType 	=	 0,
        Promotion 	=	 0,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 FALSE,
        LearnSP = 1,
		
        Img 	=	 "skillicon53.dds",
        TitleID 	=	 150,
        DescID 	=	 9,
        ControlDesc	=	130,
        		
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
    
 
}