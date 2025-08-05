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
	-- �׶� ���� LV1
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

	-- �׶� ���� LV2
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

	-- �׶� ���� LV3
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

	-- �׶� ���� ���� �ð�
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

	-- �׶� ���� ���� ����
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

	-- ����Ʈ ���� LV1	
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

	-- ����Ʈ ���� LV2	
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

	-- ����Ʈ ���� LV3	
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

	-- �극��ũ ���� LV1	
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

	-- �극��ũ ���� LV2	
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

	-- �극��ũ ���� LV3	
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

	-- ��� LV1	
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

	-- ��� LV2	
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

	-- ��� LV3	
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

	-- ��Ű�� LV1	
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

	-- ��Ű�� LV2	
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

	-- ��Ű�� LV3	
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

	-- ��Ű�� AP ���	
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

	-- ��Ű�� HP ���	
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

	-- ��Ű�� ���� �� ���� LV1
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

	-- ��Ű�� ���� �� ���� LV2
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

	-- ��Ű�� ���� �� ���� LV3
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

	-- ���� HP / AP ����� LV1
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

	-- ���� HP / AP ����� LV2
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

	-- ��̵� (����)	
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

	-- �� ���� LV1
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
	
	-- �� ���� LV2
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

	-- �� ���� LV3
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
	
	-- ��ũ���� (����)	
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

	-- ��ũ���� �ð� ���� LV1	
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

	-- ��ũ���� �ð� ���� LV2	
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

	-- ��ũ���� �ð� ���� LV3	
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

	-- ��ũ���� SS ���� LV1
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

	-- ��ũ���� SS ���� LV2
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

	-- ��ũ���� DS ���� LV1
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

	-- ��ũ���� DS ���� LV2
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

	-- ��ũ���� OS ���� LV1
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

	-- ��ũ���� OS ���� LV2
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

	-- DS �� ���� LV4
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
	
	-- DS �� ���� LV5
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

	-- DS �ݻ� LV1
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

	-- DS �ݻ� LV2
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

	-- DS ��� �޺�
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

	-- DS �ϴ� �޺�
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

	-- OS ���
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
	
	-- OS �뽬
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

	-- OS ũ��/����
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
	
	-- OS ���� LV1
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
	
	-- OS ���� LV2
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
}