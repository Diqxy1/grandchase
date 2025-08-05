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
	-- 라이언 3차 마나 4칸 확장
    {		
		ID 			=	SID_RYAN4_HIDDEN_MP4,
		GroupID 	=	SG_RYAN4_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	4,
		Promotion 	=	3,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon985.dds",
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

    -- 라이언 3차 4단필살기
    {		
		ID 			=	SID_RYAN4_SPECIAL_4,
		GroupID 	=	SG_RYAN4_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	4,
		Promotion 	=	3,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon986.dds",
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
			Motion = MID_RYAN4_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			
			ChargingSkillLV = 4,
		},
    },  
	-- 라이언 제노사이더 기본 1필
    {		
        ID 	=	 SID_RYAN4_BASE_SEPCIAL1,
        GroupID 	=	 SG_RYAN4_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage926.dds",
        TitleID 	=	 72,
        DescID 	=	 73,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RYAN4_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },		
    
    -- 라이언 제노사이더 기본 한손 1필
    {		
        ID 	=	 SID_RYAN4_BASE_SEPCIAL1_ONEHAND,
        GroupID 	=	 SG_RYAN4_SPECIAL1_ONEHAND,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage960.dds",
        TitleID 	=	 72,
        DescID 	=	 73,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RYAN4_ONE_HAND_SPECIAL1_1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },		
	-- 라이언 제노사이더 기본 2필
    {		
        ID 	=	 SID_RYAN4_BASE_SEPCIAL2,
        GroupID 	=	 SG_RYAN4_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage927.dds",
        TitleID 	=	 74,
        DescID 	=	 75,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RYAN4_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },		
    
    -- 라이언 제노사이더 기본 한손 2필
    {		
        ID 	=	 SID_RYAN4_BASE_SEPCIAL2_ONEHAND,
        GroupID 	=	 SG_RYAN4_SPECIAL2_ONEHAND,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage963.dds",
        TitleID 	=	 74,
        DescID 	=	 75,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RYAN4_ONE_HAND_SPECIAL2_1,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },		
	-- 라이언 제노사이더 기본 3필
    {		
        ID 	=	 SID_RYAN4_BASE_SEPCIAL3,
        GroupID 	=	 SG_RYAN4_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage968.dds",
        TitleID 	=	 76,
        DescID 	=	 77,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,        
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RYAN4_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },	
    -- 라이언 제노사이더 기본 한손 3필
    {		
        ID 	=	 SID_RYAN4_BASE_SEPCIAL3_ONEHAND,
        GroupID 	=	 SG_RYAN4_SPECIAL3_ONEHAND,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage969.dds",
        TitleID 	=	 76,
        DescID 	=	 77,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        SpecialTypeB = TRUE,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RYAN4_ONE_HAND_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },	
    --네피림 변신
    {		
        ID 	=	 SID_RYAN_NEPHILLIM_FORM,
        GroupID 	=	 SG_RYAN1_CASH_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 { 3953, },
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
			Mana = 2.9,
			Motion = MID_NEPHILLIM_FORM_START,			
			CoolTime = 26,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
    -- 라이언 제노사이더 매그너스단 변신
    {		
        ID 	=	 SID_RYAN_MEGNUS_DAN_FORM,
        GroupID 	=	 SG_RYAN_MEGNUS_DAN_FORM,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage924.dds",
        TitleID 	=	 74,
        DescID 	=	 75,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RYAN4_FORM_START,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
    
    -- 네피림 1단필
    {		
        ID 	=	 SID_RYAN_NEPHILLIM_SPECIAL1,
        GroupID 	=	 SG_RYAN_NEPHILLIM_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 -1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage960.dds",
        TitleID 	=	 72,
        DescID 	=	 73,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_NEPHILLIM_SPECIAL,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },				
    
    -- 매그너스단 1필
    {		
        ID 	=	 SID_RYAN_MEGNUS_DAN_SPECIAL1,
        GroupID 	=	 SG_RYAN_MEGNUS_DAN_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1002.dds",
        TitleID 	=	 72,
        DescID 	=	 73,
        ControlDesc	=	125,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_RYAN4_MDAN_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },		
    
	-- 매그너스단 2필
    {		
        ID 	=	 SID_RYAN_MEGNUS_DAN_SPECIAL2,
        GroupID 	=	 SG_RYAN_MEGNUS_DAN_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1003.dds",
        TitleID 	=	 74,
        DescID 	=	 75,
        ControlDesc	=	126,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_RYAN4_MDAN_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },		
    
    -- 매그너스단 3필
    {		
        ID 	=	 SID_RYAN_MEGNUS_DAN_SPECIAL3,
        GroupID 	=	 SG_RYAN_MEGNUS_DAN_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 4,
        Promotion 	=	 3,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage1004.dds",
        TitleID 	=	 76,
        DescID 	=	 77,
        ControlDesc	=	127,
        		
        UI_Pos_x 	=	 100,
        UI_Pos_y 	=	 100,
        UI_LineType 	=	 1,
        
        LockGroup = -1,
        
        SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_RYAN4_MDAN_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },	
}