TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ ���̾� 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- �γ� 1�� ���� 4ĭ Ȯ��
    {		
		ID 			=	SID_RONAN2_HIDDEN_MP4,
		GroupID 	=	SG_RONAN2_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	5,
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

    -- �γ� 1�� 4���ʻ��
    {		
		ID 			=	SID_RONAN2_SPECIAL_4,
		GroupID 	=	SG_RONAN2_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	5,
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
			Motion = MID_RONAN2_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
			
			ChargingSkillLV = 4,
		},
    },
	-- �γ� �巡�ﳪ��Ʈ �⺻ �� 1��
    {		
        ID 	=	 SID_RONAN2_BLACK_SEPCIAL1,
        GroupID 	=	 SG_RONAN2_BLACK_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage881.dds",
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
			Motion = MID_RONAN2_SWORD_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },
    
	-- �γ� �巡�ﳪ��Ʈ �⺻ �� 2��
    {		
        ID 	=	 SID_RONAN2_BLACK_SEPCIAL2,
        GroupID 	=	 SG_RONAN2_BLACK_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage882.dds",
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
			Motion = MID_RONAN2_SWORD_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },		
	-- �γ� �巡�ﳪ��Ʈ �⺻ �� 3��
    {		
        ID 	=	 SID_RONAN2_BLACK_SEPCIAL3,
        GroupID 	=	 SG_RONAN2_BLACK_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage883.dds",
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
			Motion = MID_RONAN2_SWORD_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },	
    	
	-- �γ� �巡�ﳪ��Ʈ �⺻ �� 1��
    {		
        ID 	=	 SID_RONAN2_WHITE_SEPCIAL1,
        GroupID 	=	 SG_RONAN2_WHITE_SPECIAL1,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage884.dds",
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
			Motion = MID_RONAN2_MAGIC_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },		
	-- �γ� �巡�ﳪ��Ʈ �⺻ �� 2��
    {		
        ID 	=	 SID_RONAN2_WHITE_SEPCIAL2,
        GroupID 	=	 SG_RONAN2_WHITE_SPECIAL2,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage885.dds",
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
			Motion = MID_RONAN2_MAGIC_SPECIAL2,			
			CoolTime = 10,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },		
	-- �γ� �巡�ﳪ��Ʈ �⺻ �� 3��
    {		
        ID 	=	 SID_RONAN2_WHITE_SEPCIAL3,
        GroupID 	=	 SG_RONAN2_WHITE_SPECIAL3,
        PreList 	=	 {},
        NextList 	=	 {},
        CharLv 	=	 0,
        CharType 	=	 5,
        Promotion 	=	 1,
        Enable  	=	 TRUE,
        GoodsID 	=	 {},
        Base 	=	 TRUE,
        LearnSP = 0,
		
        Img 	=	 "skillimage886.dds",
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
			Motion = MID_RONAN2_MAGIC_SPECIAL3,			
			CoolTime = 10,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },	
}