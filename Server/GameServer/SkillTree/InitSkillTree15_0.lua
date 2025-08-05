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
	-- 1단 필살기
	{		
		ID 			=	SID_ASIN_SPECIAL_1,
		GroupID 	=	SG_ASIN_SPECIAL_1,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon947.dds",
		TitleID 	=	2111,
		DescID 		=	2147,
		ControlDesc	=	127,

		UI_Pos_x 	=	250,
		UI_Pos_y 	=	150,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_ASIN1_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },

	-- 2단 필살기
    {		
		ID 			=	SID_ASIN_SPECIAL_2,
		GroupID 	=	SG_ASIN_SPECIAL_2,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon948.dds",
		TitleID 	=	2112,
		DescID 		=	2148,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	150,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_ASIN1_SPECIAL2,			
			CoolTime = 20,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	
	-- 3단 필살기
    {		
		ID 			=	SID_ASIN_SPECIAL_3,
		GroupID 	=	SG_ASIN_SPECIAL_3,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon949.dds",
		TitleID 	=	2113,
		DescID 		=	2149,
		ControlDesc	=	127,

		UI_Pos_x 	=	450,
		UI_Pos_y 	=	150,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = FALSE,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_ASIN1_SPECIAL3,
			CoolTime = 30,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },

	-- 콤보 중 업 커맨드
    {		
		ID 			=	SID_ASIN_NORMAL_COMBO_UP,
		GroupID 	=	SG_ASIN_NORMAL_COMBO_UP,
		PreList 	=	{ },
		NextList 	=	{SID_ASIN_NORMAL_COMBO_PLUS, },
		CharLv 		=	4,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon951.dds",
		TitleID 	=	2115,
		DescID 		=	2151,
		ControlDesc	=	2187,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },

	-- 콤보 플러스
    {		
		ID 			=	SID_ASIN_NORMAL_COMBO_PLUS,
		GroupID 	=	SG_ASIN_NORMAL_COMBO_PLUS,
		PreList 	=	{SID_ASIN_NORMAL_COMBO_UP, },
		NextList 	=	{SID_ASIN_NORMAL_SPEED_BUFF, },
		CharLv 		=	8,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon958.dds",
		TitleID 	=	2122,
		DescID 		=	2158,
		ControlDesc	=	220,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220,
		UI_LineType =	19,	

		LockGroup = -1,
    },

	-- 이속 증가 버프
    {		
		ID 			=	SID_ASIN_NORMAL_SPEED_BUFF,
		GroupID 	=	SG_ASIN_NORMAL_SPEED_BUFF,
		PreList 	=	{SID_ASIN_NORMAL_COMBO_PLUS, },
		NextList 	=	{SID_ASIN_NORMAL_MP_RECOVERY_1, },
		CharLv 		=	12,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon975.dds",
		TitleID 	=	2139,
		DescID 		=	2175,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_SPEED_BUFF,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },

	-- MP 회복 속도 증가 1
    {		
		ID 			=	SID_ASIN_NORMAL_MP_RECOVERY_1,
		GroupID 	=	SG_ASIN_NORMAL_MP_RECOVERY,
		PreList 	=	{SID_ASIN_NORMAL_SPEED_BUFF, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_1, },
		CharLv 		=	16,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon966.dds",
		TitleID 	=	2130,
		DescID 		=	2166,
		ControlDesc	=	430,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },

	-- 1차 오의 개방
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_1,
		GroupID 	=	SG_ASIN_HIDDEN_SKILL,
		PreList 	=	{SID_ASIN_NORMAL_MP_RECOVERY_1, },
		NextList 	=	{SID_ASIN_HIDDEN_ABILITY_UP, SID_ASIN_NORMAL_DEFFENCE_BUFF,},
		CharLv 		=	20,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon954.dds",
		TitleID 	=	2118,
		DescID 		=	2154,
		ControlDesc	=	430,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	220,
		UI_LineType =	26,

		LockGroup = -1,
		Passive = TRUE,
    },	

	-- 방어력 증가 버프
    {		
		ID 			=	SID_ASIN_NORMAL_DEFFENCE_BUFF,
		GroupID 	=	SG_ASIN_NORMAL_DEFFENCE_BUFF,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_1, },
		NextList 	=	{SID_ASIN_NORMAL_DASH_COMBO_PLUS, },
		CharLv 		=	24,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon956.dds",
		TitleID 	=	2120,
		DescID 		=	2156,
		ControlDesc	=	127,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	315,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_DEFFENCE_BUFF,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },

	---- 대쉬 콤보 플러스
    {		
		ID 			=	SID_ASIN_NORMAL_DASH_COMBO_PLUS,
		GroupID 	=	SG_ASIN_NORMAL_DASH_COMBO_PLUS,
		PreList 	=	{SID_ASIN_NORMAL_DEFFENCE_BUFF, },
		NextList 	=	{SID_ASIN_NORMAL_COUNT_COOL_DOWN_1, },
		CharLv 		=	28,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon953.dds",
		TitleID 	=	2117,
		DescID 		=	2153,
		ControlDesc	=	2183,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	315,
		UI_LineType =	19,

		LockGroup = -1,
    },

	-- 카운터 쿨 타임 감소
    {		
		ID 			=	SID_ASIN_NORMAL_COUNT_COOL_DOWN_1,
		GroupID 	=	SG_ASIN_NORMAL_COUNT_COOL_DOWN,
		PreList 	=	{SID_ASIN_NORMAL_DASH_COMBO_PLUS, },
		NextList 	=	{SID_ASIN_NORMAL_COMBO_BACK, },
		CharLv 		=	32,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon967.dds",
		TitleID 	=	2131,
		DescID 		=	2167,
		ControlDesc	=	430,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	315,
		UI_LineType =	19,	

		LockGroup = -1,
    },

	-- 콤보 중 후방 커맨드
    {		
		ID 			=	SID_ASIN_NORMAL_COMBO_BACK,
		GroupID 	=	SG_ASIN_NORMAL_COMBO_BACK,
		PreList 	=	{SID_ASIN_NORMAL_COUNT_COOL_DOWN_1, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_2, },
		CharLv 		=	36,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon950.dds",
		TitleID 	=	2114,
		DescID 		=	2150,
		ControlDesc	=	2186,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	315,
		UI_LineType =	19,	

		LockGroup = -1,
    },

	-- 2차 오의 개방
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_2,
		GroupID 	=	SG_ASIN_HIDDEN_SKILL,
		PreList 	=	{SID_ASIN_NORMAL_COMBO_BACK, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_THRUST_1, SID_ASIN_HIDDEN_SKILL_BUFF_TIME, SID_ASIN_HIDDEN_SKILL_SPECIAL_MP_DOWN, SID_ASIN_NORMAL_JUMP_COMBO_PLUS, },
		CharLv 		=	40,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon960.dds",
		TitleID 	=	2124,
		DescID 		=	2160,
		ControlDesc	=	430,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	315,
		UI_LineType =	24,	

		LockGroup = -1,
		Passive = TRUE,
    },				

	-- 점프 콤보 플러스
    {		
		ID 			=	SID_ASIN_NORMAL_JUMP_COMBO_PLUS,
		GroupID 	=	SG_ASIN_NORMAL_JUMP_COMBO_PLUS,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_2, },
		NextList 	=	{SID_ASIN_NORMAL_MP_RECOVERY_2, },
		CharLv 		=	44,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon964.dds",
		TitleID 	=	2128,
		DescID 		=	2164,
		ControlDesc	=	2185,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	410,
		UI_LineType =	19,	

		LockGroup = -1,
    },

	-- MP 회복 속도 증가 2
    {		
		ID 			=	SID_ASIN_NORMAL_MP_RECOVERY_2,
		GroupID 	=	SG_ASIN_NORMAL_MP_RECOVERY,
		PreList 	=	{SID_ASIN_NORMAL_JUMP_COMBO_PLUS, },
		NextList 	=	{SID_ASIN_NORMAL_ARROW_REFLECT, },
		CharLv 		=	48,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon973.dds",
		TitleID 	=	2137,
		DescID 		=	2173,
		ControlDesc	=	430,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	410,
		UI_LineType =	19,

		LockGroup = -1,
    },

	-- 화살 반사
    {		
		ID 			=	SID_ASIN_NORMAL_ARROW_REFLECT,
		GroupID 	=	SG_ASIN_NORMAL_ARROW_REFLECT,
		PreList 	=	{SID_ASIN_NORMAL_MP_RECOVERY_2, },
		NextList 	=	{SID_ASIN_NORMAL_DOWN_MOVE, },
		CharLv 		=	52,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon952.dds",
		TitleID 	=	2116,
		DescID 		=	2152,
		ControlDesc	=	2188,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	410,
		UI_LineType =	19,	

		LockGroup = -1,
    },

	-- 다운 중 회피
    {		
		ID 			=	SID_ASIN_NORMAL_DOWN_MOVE,
		GroupID 	=	SG_ASIN_NORMAL_DOWN_MOVE,
		PreList 	=	{SID_ASIN_NORMAL_ARROW_REFLECT, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_3, },
		CharLv 		=	56,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon957.dds",
		TitleID 	=	2121,
		DescID 		=	2157,
		ControlDesc	=	2184,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	410,
		UI_LineType =	19,	

		LockGroup = -1,
    },
	
	-- 3차 오의 개방
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_3,
		GroupID 	=	SG_ASIN_HIDDEN_SKILL,
		PreList 	=	{SID_ASIN_NORMAL_DOWN_MOVE, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_THRUST_2, SID_ASIN_HIDDEN_SKILL_BUFF_STRONG, SID_ASIN_HIDDEN_SKILL_SPECIAL_POWER_UP, SID_ASIN_NORMAL_COUNT_COOL_DOWN_2, },
		CharLv 		=	60,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon968.dds",
		TitleID 	=	2132,
		DescID 		=	2168,
		ControlDesc	=	430,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	410,
		UI_LineType =	26,	

		LockGroup = -1,
		Passive = TRUE,
    },	
	
	-- 카운터 쿨 타임 감소 2
    {		
		ID 			=	SID_ASIN_NORMAL_COUNT_COOL_DOWN_2,
		GroupID 	=	SG_ASIN_NORMAL_COUNT_COOL_DOWN,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_3, },
		NextList 	=	{SID_ASIN_NORMAL_OFFENCE_BUFF, },
		CharLv 		=	64,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon974.dds",
		TitleID 	=	2138,
		DescID 		=	2174,
		ControlDesc	=	430,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	505,
		UI_LineType =	-1,	

		LockGroup = -1,
    },

	-- 공격력 증가 버프
    {		
		ID 			=	SID_ASIN_NORMAL_OFFENCE_BUFF,
		GroupID 	=	SG_ASIN_NORMAL_OFFENCE_BUFF,
		PreList 	=	{SID_ASIN_NORMAL_COUNT_COOL_DOWN_2, },
		NextList 	=	{SID_ASIN_NORMAL_UNBEATABLE, },
		CharLv 		=	68,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon972.dds",
		TitleID 	=	2136,
		DescID 		=	2172,
		ControlDesc	=	127,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_OFFENCE_BUFF,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },

	-- 참선
    {		
		ID 			=	SID_ASIN_NORMAL_UNBEATABLE,
		GroupID 	=	SG_ASIN_NORMAL_UNBEATABLE,
		PreList 	=	{SID_ASIN_NORMAL_OFFENCE_BUFF, },
		NextList 	=	{SID_ASIN_NORMAL_FLOW, },
		CharLv 		=	72,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon965.dds",
		TitleID 	=	2129,
		DescID 		=	2165,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_NORMAL_UNBEATABLE,			
			CoolTime = 40,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 40,
		},
    },
	
	-- 흘리기
    {		
		ID 			=	SID_ASIN_NORMAL_FLOW,
		GroupID 	=	SG_ASIN_NORMAL_FLOW,
		PreList 	=	{SID_ASIN_NORMAL_UNBEATABLE, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_4, },
		CharLv 		=	76,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,
		
		Img 		=	"skillicon959.dds",
		TitleID 	=	2123,
		DescID 		=	2159,
		ControlDesc	=	127,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_SEND_READY,			
			CoolTime = 10,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 10,
			
			ForceUseSkill = TRUE,
		},
    },

	-- 4차 오의 개방
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_4,
		GroupID 	=	SG_ASIN_HIDDEN_SKILL,
		PreList 	=	{SID_ASIN_NORMAL_FLOW, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_THRUST_3, SID_ASIN_HIDDEN_SKILL_BUFF_BONUS, SID_ASIN_HIDDEN_SKILL_SPECIAL_CHAIN, },
		CharLv 		=	80,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon976.dds",
		TitleID 	=	2140,
		DescID 		=	2176,
		ControlDesc	=	430,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,
		Passive = TRUE,
    },			

	-- 1차 오의 개방 (능력치 증가)
    {		
		ID 			=	SID_ASIN_HIDDEN_ABILITY_UP,
		GroupID 	=	SG_ASIN_HIDDEN_ABILITY_UP,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_1, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_THRUST_1, SID_ASIN_HIDDEN_SKILL_BUFF_TIME, SID_ASIN_HIDDEN_SKILL_SPECIAL_MP_DOWN, },
		CharLv 		=	20,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon955.dds",
		TitleID 	=	2119,
		DescID 		=	2155,
		ControlDesc	=	430,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	150,
		UI_LineType =	28,

		LockGroup = -1,
		Passive = TRUE,
    },	
    
    -- 점혈 1
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_THRUST_1,
		GroupID 	=	SG_ASIN_HIDDEN_THRUST,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_2, SID_ASIN_HIDDEN_ABILITY_UP, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_THRUST_2, },
		CharLv 		=	40,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon961.dds",
		TitleID 	=	2125,
		DescID 		=	2161,
		ControlDesc	=	127,

		UI_Pos_x 	=	190,
		UI_Pos_y 	=	237,
		UI_LineType =	27,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_DEBUFF_ATK1,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
    
    -- 점혈 2
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_THRUST_2,
		GroupID 	=	SG_ASIN_HIDDEN_THRUST,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_3, SID_ASIN_HIDDEN_SKILL_THRUST_1, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_THRUST_3, },
		CharLv 		=	60,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon969.dds",
		TitleID 	=	2133,
		DescID 		=	2169,
		ControlDesc	=	127,

		UI_Pos_x 	=	190,
		UI_Pos_y 	=	317,
		UI_LineType =	27,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_DEBUFF_ATK2,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },
    
    -- 점혈 3
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_THRUST_3,
		GroupID 	=	SG_ASIN_HIDDEN_THRUST,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_4, SID_ASIN_HIDDEN_SKILL_THRUST_2, },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,
		
		Img 		=	"skillicon977.dds",
		TitleID 	=	2141,
		DescID 		=	2177,
		ControlDesc	=	127,

		UI_Pos_x 	=	190,
		UI_Pos_y 	=	397,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_DEBUFF_ATK3,			
			CoolTime = 5,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 5,
		},
    },		
    
    -- 버프 지속 시간
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_BUFF_TIME,
		GroupID 	=	SG_ASIN_HIDDEN_BUFF,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_2, SID_ASIN_HIDDEN_ABILITY_UP, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_BUFF_STRONG, },
		CharLv 		=	40,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon962.dds",
		TitleID 	=	2126,
		DescID 		=	2162,
		ControlDesc	=	430,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	237,
		UI_LineType =	27,

		LockGroup = -1,
    },
    
    -- 버프 강화
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_BUFF_STRONG,
		GroupID 	=	SG_ASIN_HIDDEN_BUFF,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_3, SID_ASIN_HIDDEN_SKILL_BUFF_TIME, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_BUFF_BONUS, },
		CharLv 		=	60,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon970.dds",
		TitleID 	=	2134,
		DescID 		=	2170,
		ControlDesc	=	430,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	317,
		UI_LineType =	27,

		LockGroup = -1,
    },	
    
    -- 버프 보너스
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_BUFF_BONUS,
		GroupID 	=	SG_ASIN_HIDDEN_BUFF,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_4, SID_ASIN_HIDDEN_SKILL_BUFF_STRONG, },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon978.dds",
		TitleID 	=	2142,
		DescID 		=	2178,
		ControlDesc	=	430,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	397,
		UI_LineType =	-1,

		LockGroup = -1,
    },	
    
    -- 필살기 마나 감소
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_SPECIAL_MP_DOWN,
		GroupID 	=	SG_ASIN_HIDDEN_SPECIAL,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_2, SID_ASIN_HIDDEN_ABILITY_UP, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_SPECIAL_POWER_UP, },
		CharLv 		=	40,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon963.dds",
		TitleID 	=	2127,
		DescID 		=	2163,
		ControlDesc	=	430,

		UI_Pos_x 	=	510,
		UI_Pos_y 	=	237,
		UI_LineType =	27,

		LockGroup = -1,
    },	
    
    -- 필살기 공격력 증가
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_SPECIAL_POWER_UP,
		GroupID 	=	SG_ASIN_HIDDEN_SPECIAL,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_3, SID_ASIN_HIDDEN_SKILL_SPECIAL_MP_DOWN, },
		NextList 	=	{SID_ASIN_HIDDEN_SKILL_SPECIAL_CHAIN, },
		CharLv 		=	60,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon971.dds",
		TitleID 	=	2135,
		DescID 		=	2171,
		ControlDesc	=	430,

		UI_Pos_x 	=	510,
		UI_Pos_y 	=	317,
		UI_LineType =	27,

		LockGroup = -1,
    },
    
    -- 필살기 연계
    {		
		ID 			=	SID_ASIN_HIDDEN_SKILL_SPECIAL_CHAIN,
		GroupID 	=	SG_ASIN_HIDDEN_SPECIAL,
		PreList 	=	{SID_ASIN_HIDDEN_SKILL_4, SID_ASIN_HIDDEN_SKILL_SPECIAL_POWER_UP, },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon979.dds",
		TitleID 	=	2143,
		DescID 		=	2179,
		ControlDesc	=	430,

		UI_Pos_x 	=	510,
		UI_Pos_y 	=	397,
		UI_LineType =	-1,

		LockGroup = -1,
    },

    -- 최종 봉인기 (MP 게이지 4칸)
    {		
		ID 			=	SID_ASIN_SUPER_HIDDEN_MP4,
		GroupID 	=	SG_ASIN_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon980.dds",
		TitleID 	=	2144,
		DescID 		=	2180,
		ControlDesc	=	430,

		UI_Pos_x 	=	190,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,
		Overlapped  = FALSE,

		LockGroup = -1,
		Passive = TRUE,
    },

    -- 최종 봉인기 (무적 버프)
    {		
		ID 			=	SID_ASIN_SUPER_HIDDEN_UNBEATABLE_BUFF,
		GroupID 	=	SG_ASIN_SUPER_HIDDEN_UNBEATABLE_BUFF,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	70,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon981.dds",
		TitleID 	=	2145,
		DescID 		=	2181,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.2,
			Motion = MID_ASIN1_ULTIMATE_BUFF,			
			CoolTime = 30,
			
			MonsterModeMana = 0.2,
			MonsterModeCoolTime = 30,
		},
    },
	
    -- 최종 봉인기 (4단 필살기)
    {		
		ID 			=	SID_ASIN_SUPER_HIDDEN_SPECIAL_4,
		GroupID 	=	SG_ASIN_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	15,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon982.dds",
		TitleID 	=	2146,
		DescID 		=	2182,
		ControlDesc	=	127,

		UI_Pos_x 	=	510,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 3.9,
			Motion = MID_ASIN1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
		},
    },
}