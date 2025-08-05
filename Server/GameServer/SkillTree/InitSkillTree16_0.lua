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
		ID 			=	SID_LIME_SPECIAL_1,
		GroupID 	=	SG_LIME_SPECIAL_1,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon983.dds",
		TitleID 	=	2189,
		DescID 		=	2230,
		ControlDesc	=	127,

		UI_Pos_x 	=	250,
		UI_Pos_y 	=	150,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.9,
			Motion = MID_LIME1_SPECIAL1,			
			CoolTime = 10,
			
			MonsterModeMana = 0.9,
			MonsterModeCoolTime = 10,
			
			ChargingSkillLV = 1,
			IsSpecialSkill = TRUE,
		},
    },

	-- 2단 필살기
    {		
		ID 			=	SID_LIME_SPECIAL_2,
		GroupID 	=	SG_LIME_SPECIAL_2,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon984.dds",
		TitleID 	=	2190,
		DescID 		=	2231,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	150,
		UI_LineType =	-1,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 1.9,
			Motion = MID_LIME1_SPECIAL2,			
			CoolTime = 20,
			
			MonsterModeMana = 1.9,
			MonsterModeCoolTime = 18,
			
			ChargingSkillLV = 2,
			IsSpecialSkill = TRUE,
		},
    },
	
	-- 3단 필살기
    {		
		ID 			=	SID_LIME_SPECIAL_3,
		GroupID 	=	SG_LIME_SPECIAL_3,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	0,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon985.dds",
		TitleID 	=	2191,
		DescID 		=	2232,
		ControlDesc	=	127,

		UI_Pos_x 	=	450,
		UI_Pos_y 	=	150,
		UI_LineType =	-1,

		LockGroup = -1,
		Overlapped  = FALSE,
		
		SlotSkill = 
		{
			Mana = 2.9,
			Motion = MID_LIME1_SPECIAL3,
			CoolTime = 30,
			
			MonsterModeMana = 2.9,
			MonsterModeCoolTime = 26,
			
			ChargingSkillLV = 3,
			IsSpecialSkill = TRUE,
		},
    },
	
	-- 콤보 중 업 커맨드
    {		
		ID 			=	SID_LIME_NORMAL_COMBO_UP,
		GroupID 	=	SG_LIME_NORMAL_COMBO_UP,
		PreList 	=	{ },
		NextList 	=	{SID_LIME_NORMAL_COMBO_BACK, },
		CharLv 		=	4,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon986.dds",
		TitleID 	=	2192,
		DescID 		=	2233,
		ControlDesc	=	216,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
		
    },
	
	-- 콤보 중 뒤 커맨드
    {		
		ID 			=	SID_LIME_NORMAL_COMBO_BACK,
		GroupID 	=	SG_LIME_NORMAL_COMBO_BACK,
		PreList 	=	{SID_LIME_NORMAL_COMBO_UP, },
		NextList 	=	{SID_LIME_NORMAL_COMBO_PLUS, },
		CharLv 		=	8,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon987.dds",
		TitleID 	=	2193,
		DescID 		=	2234,
		ControlDesc	=	2186,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	220,
		UI_LineType =	19,	

		LockGroup = -1,
		
    },

	-- 대쉬 강화 (대쉬에서 런으로 연결)
    {		
		ID 			=	SID_LIME_NORMAL_COMBO_PLUS,
		GroupID 	=	SG_LIME_NORMAL_COMBO_PLUS,
		PreList 	=	{SID_LIME_NORMAL_COMBO_BACK, },
		NextList 	=	{SID_LIME_NORMAL_DASH_PLUS, },
		CharLv 		=	12,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon988.dds",
		TitleID 	=	2194,
		DescID 		=	2235,
		ControlDesc	=	2271,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
		
    },

	-- 대쉬 콤보 플러스
    {		
		ID 			=	SID_LIME_NORMAL_DASH_PLUS,
		GroupID 	=	SG_LIME_NORMAL_DASH_PLUS,
		PreList 	=	{SID_LIME_NORMAL_COMBO_PLUS, },
		NextList 	=	{SID_LIME_NORMAL_ABILITY_UP, SID_LIME_NORMAL_DASH_PLUS_COUNT, SID_LIME_NORMAL_DASH_PLUS_MP_DOWN, SID_LIME_NORMAL_DASH_PLUS_JUMP, },
		CharLv 		=	16,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon989.dds",
		TitleID 	=	2195,
		DescID 		=	2236,
		ControlDesc	=	2272,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
		
    },

	-- 능력치 증가
    {		
		ID 			=	SID_LIME_NORMAL_ABILITY_UP,
		GroupID 	=	SG_LIME_NORMAL_ABILITY_UP,
		PreList 	=	{SID_LIME_NORMAL_DASH_PLUS, },
		NextList 	=	{SID_LIME_NORMAL_COMBO_SPEED, },
		CharLv 		=	20,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon990.dds",
		TitleID 	=	2196,
		DescID 		=	2237,
		ControlDesc	=	397,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	220,
		UI_LineType =	26,

		LockGroup = -1,
		
		Passive = TRUE,
    },

	-- 콤보 공속 증가
    {		
		ID 			=	SID_LIME_NORMAL_COMBO_SPEED,
		GroupID 	=	SG_LIME_NORMAL_COMBO_SPEED,
		PreList 	=	{SID_LIME_NORMAL_ABILITY_UP, },
		NextList 	=	{SID_LIME_NORMAL_MP_RECOVERY_1, },
		CharLv 		=	24,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon996.dds",
		TitleID 	=	2202,
		DescID 		=	2243,
		ControlDesc	=	397,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	315,
		UI_LineType =	-1,	

		LockGroup = -1,
		
    },	
	
	-- 잡기 데미지 감소
    --{		
	--	ID 			=	SID_LIME_NORMAL_CATCH_DEFENSE,
	--	GroupID 	=	SG_LIME_NORMAL_CATCH_DEFENSE,
	--	PreList 	=	{SID_LIME_NORMAL_ABILITY_UP, },
	--	NextList 	=	{SID_LIME_NORMAL_MP_RECOVERY_1, },
	--	CharLv 		=	24,
	--	CharType 	=	16,
	--	Promotion 	=	-1,
	--	Enable  	=	TRUE,
	--	GoodsID 	=	{},
	--	Base 		=	FALSE,
	--	LearnSP 	=	8,
    --
	--	Img 		=	"skillicon991.dds",
	--	TitleID 	=	2197,
	--	DescID 		=	2238,
	--	ControlDesc	=	397,
    --
	--	UI_Pos_x 	=	590,
	--	UI_Pos_y 	=	315,
	--	UI_LineType =	-1,	
    --
	--	LockGroup = -1,
	--	
    --},
	
	-- 기본 MP 회복 속도 증가1
    {		
		ID 			=	SID_LIME_NORMAL_MP_RECOVERY_1,
		GroupID 	=	SG_LIME_NORMAL_MP_RECOVERY,
		PreList 	=	{SID_LIME_NORMAL_COMBO_SPEED, },
		NextList 	=	{SID_LIME_NORMAL_AURA_DEFENSE, },
		CharLv 		=	28,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon992.dds",
		TitleID 	=	2198,
		DescID 		=	2239,
		ControlDesc	=	397,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	315,
		UI_LineType =	19,

		LockGroup = -1,
		
    },
	
	-- 오오라 - 방어
    {		
		ID 			=	SID_LIME_NORMAL_AURA_DEFENSE,
		GroupID 	=	SG_LIME_NORMAL_AURA_DEFENSE,
		PreList 	=	{SID_LIME_NORMAL_MP_RECOVERY_1, },
		NextList 	=	{SID_LIME_NORMAL_JUMP_DASH, SID_LIME_NORMAL_AURA_DEFENSE_UP, SID_LIME_NORMAL_AURA_DEFENSE_HP_RECOVERY, SID_LIME_NORMAL_AURA_DEFENSE_SHEILD, },
		CharLv 		=	32,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon993.dds",
		TitleID 	=	2199,
		DescID 		=	2240,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	315,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.0,
			Motion = MID_LIME1_AURA_DEFENSE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.0,
			MonsterModeCoolTime = 10,
			
			--AuraSizeWidht = 0.4,
			--AuraSizeHeight = 0.4,
			--AuraDeBuff = { EGC_EFFECT_SKILL_FEAR_LV1, },
		},
    },
	
	-- 점프 대쉬
    {		
		ID 			=	SID_LIME_NORMAL_JUMP_DASH,
		GroupID 	=	SG_LIME_NORMAL_JUMP_DASH,
		PreList 	=	{SID_LIME_NORMAL_AURA_DEFENSE, },
		NextList 	=	{SID_LIME_NORMAL_ARMOR, },
		CharLv 		=	36,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon994.dds",
		TitleID 	=	2200,
		DescID 		=	2241,
		ControlDesc	=	402,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	315,
		UI_LineType =	19,	

		LockGroup = -1,
		
    },
	
	-- 하이퍼 아머
    {		
		ID 			=	SID_LIME_NORMAL_ARMOR,
		GroupID 	=	SG_LIME_NORMAL_ARMOR,
		PreList 	=	{SID_LIME_NORMAL_JUMP_DASH, },
		NextList 	=	{SID_LIME_NORMAL_CATCH_DEFENSE, },
		CharLv 		=	40,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon995.dds",
		TitleID 	=	2201,
		DescID 		=	2242,
		ControlDesc	=	127,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	315,
		UI_LineType =	24,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.5,
			Motion = -1,			
			CoolTime = 15,
			
			MonsterModeMana = 0.5,
			MonsterModeCoolTime = 15,
			
			BuffIndex = EGC_EFFECT_LIME_HYPER_ARMOR,
			BuffTime = 5.0
		},
    },

	-- 잡기 데미지 감소
    {		
		ID 			=	SID_LIME_NORMAL_CATCH_DEFENSE,
		GroupID 	=	SG_LIME_NORMAL_CATCH_DEFENSE,
		PreList 	=	{SID_LIME_NORMAL_ARMOR, },
		NextList 	=	{SID_LIME_NORMAL_COUNT_COOL_DOWN_1, },
		CharLv 		=	44,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon991.dds",
		TitleID 	=	2197,
		DescID 		=	2238,
		ControlDesc	=	397,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	410,
		UI_LineType =	19,	

		LockGroup = -1,
		
    },
	
	-- 콤보 공속 증가
    --{		
	--	ID 			=	SID_LIME_NORMAL_COMBO_SPEED,
	--	GroupID 	=	SG_LIME_NORMAL_COMBO_SPEED,
	--	PreList 	=	{SID_LIME_NORMAL_ARMOR, },
	--	NextList 	=	{SID_LIME_NORMAL_COUNT_COOL_DOWN_1, },
	--	CharLv 		=	44,
	--	CharType 	=	16,
	--	Promotion 	=	-1,
	--	Enable  	=	TRUE,
	--	GoodsID 	=	{},
	--	Base 		=	FALSE,
	--	LearnSP 	=	8,
    --
	--	Img 		=	"skillicon996.dds",
	--	TitleID 	=	2202,
	--	DescID 		=	2243,
	--	ControlDesc	=	397,
    --
	--	UI_Pos_x 	=	110,
	--	UI_Pos_y 	=	410,
	--	UI_LineType =	19,	
    --
	--	LockGroup = -1,
	--	
    --},
	
	-- 카운터 쿨 타임 감소 1
    {		
		ID 			=	SID_LIME_NORMAL_COUNT_COOL_DOWN_1,
		GroupID 	=	SG_LIME_NORMAL_COUNT_COOL_DOWN,
		PreList 	=	{SID_LIME_NORMAL_CATCH_DEFENSE, },
		NextList 	=	{SID_LIME_NORMAL_AURA_ASSISTANT, },
		CharLv 		=	48,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon997.dds",
		TitleID 	=	2203,
		DescID 		=	2244,
		ControlDesc	=	397,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	410,
		UI_LineType =	19,

		LockGroup = -1,
		
    },	
	
	-- 오오라 - 보조
    {		
		ID 			=	SID_LIME_NORMAL_AURA_ASSISTANT,
		GroupID 	=	SG_LIME_NORMAL_AURA_ASSISTANT,
		PreList 	=	{SID_LIME_NORMAL_COUNT_COOL_DOWN_1, },
		NextList 	=	{SID_LIME_NORMAL_MP_RECOVERY_2, SID_LIME_NORMAL_AURA_ASSISTANT_MP_RECORVERY_UP, SID_LIME_NORMAL_AURA_ASSISTANT_SP_MANA_DOWN, SID_LIME_NORMAL_AURA_ASSISTANT_ALL_MANA_DOWN, },
		CharLv 		=	52,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon998.dds",
		TitleID 	=	2204,
		DescID 		=	2245,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	410,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.0,
			Motion = MID_LIME1_AURA_ASSISTANCE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.0,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 기본 MP 회복 속도 증가 2
    {		
		ID 			=	SID_LIME_NORMAL_MP_RECOVERY_2,
		GroupID 	=	SG_LIME_NORMAL_MP_RECOVERY,
		PreList 	=	{SID_LIME_NORMAL_AURA_ASSISTANT, },
		NextList 	=	{SID_LIME_NORMAL_AGGRESSIVE, },
		CharLv 		=	56,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon999.dds",
		TitleID 	=	2205,
		DescID 		=	2246,
		ControlDesc	=	397,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	410,
		UI_LineType =	19,	

		LockGroup = -1,
		
    },
	
	-- 도발 - 어그로
    {		
		ID 			=	SID_LIME_NORMAL_AGGRESSIVE,
		GroupID 	=	SG_LIME_NORMAL_AGGRESSIVE,
		PreList 	=	{SID_LIME_NORMAL_MP_RECOVERY_2, },
		NextList 	=	{SID_LIME_NORMAL_FATAL_SUPER_ARMOR, SID_LIME_NORMAL_AGGRESSIVE_TIME, SID_LIME_NORMAL_AGGRESSIVE_SIZE, SID_LIME_NORMAL_AGGRESSIVE_DAMAGE, },
		CharLv 		=	60,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon1000.dds",
		TitleID 	=	2206,
		DescID 		=	2247,
		ControlDesc	=	127,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	410,
		UI_LineType =	26,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.5,
			Motion = MID_LIME1_AGGRO,			
			CoolTime = 20,
			
			MonsterModeMana = 0.5,
			MonsterModeCoolTime = 20,
		},
    },
	
	-- 페이탈시 슈퍼아머
    {		
		ID 			=	SID_LIME_NORMAL_FATAL_SUPER_ARMOR,
		GroupID 	=	SG_LIME_NORMAL_FATAL_SUPER_ARMOR,
		PreList 	=	{SID_LIME_NORMAL_AGGRESSIVE, },
		NextList 	=	{SID_LIME_NORMAL_AURA_OFFENCE, },
		CharLv 		=	64,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon1001.dds",
		TitleID 	=	2207,
		DescID 		=	2248,
		ControlDesc	=	397,

		UI_Pos_x 	=	590,
		UI_Pos_y 	=	505,
		UI_LineType =	-1,	

		LockGroup = -1,
		
    },
	
	-- 오오라 - 공격
    {		
		ID 			=	SID_LIME_NORMAL_AURA_OFFENCE,
		GroupID 	=	SG_LIME_NORMAL_AURA_OFFENCE,
		PreList 	=	{SID_LIME_NORMAL_FATAL_SUPER_ARMOR, },
		NextList 	=	{SID_LIME_NORMAL_KILL_BUFF, SID_LIME_NORMAL_AURA_OFFENCE_UP, SID_LIME_NORMAL_AURA_OFFENCE_CONTINUE, SID_LIME_NORMAL_AURA_OFFENCE_CRITICAL_UP, },
		CharLv 		=	68,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon1002.dds",
		TitleID 	=	2208,
		DescID 		=	2249,
		ControlDesc	=	127,

		UI_Pos_x 	=	470,
		UI_Pos_y 	=	505,
		UI_LineType =	19,		

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 0.0,
			Motion = MID_LIME1_AURA_OFFENSE,			
			CoolTime = 10,
			
			MonsterModeMana = 0.0,
			MonsterModeCoolTime = 10,
		},
    },
	
	-- 적을 죽이면 방어력 증가
    {		
		ID 			=	SID_LIME_NORMAL_KILL_BUFF,
		GroupID 	=	SG_LIME_NORMAL_KILL_BUFF,
		PreList 	=	{SID_LIME_NORMAL_AURA_OFFENCE, },
		NextList 	=	{SID_LIME_NORMAL_COUNT_COOL_DOWN_2, },
		CharLv 		=	72,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon1003.dds",
		TitleID 	=	2209,
		DescID 		=	2250,
		ControlDesc	=	397,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,
		
		Passive = TRUE,
		
    },
	
	-- 카운터 쿨 타임 감소 2
    {		
		ID 			=	SID_LIME_NORMAL_COUNT_COOL_DOWN_2,
		GroupID 	=	SG_LIME_NORMAL_COUNT_COOL_DOWN,
		PreList 	=	{SID_LIME_NORMAL_KILL_BUFF, },
		NextList 	=	{SID_LIME_NORMAL_PARTY_BUFF, },
		CharLv 		=	76,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon1004.dds",
		TitleID 	=	2210,
		DescID 		=	2251,
		ControlDesc	=	397,

		UI_Pos_x 	=	230,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,
		
    },
	
	-- 파티원 수에 따라 능력치 변경
    {		
		ID 			=	SID_LIME_NORMAL_PARTY_BUFF,
		GroupID 	=	SG_LIME_NORMAL_PARTY_BUFF,
		PreList 	=	{SID_LIME_NORMAL_COUNT_COOL_DOWN_2, },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	8,

		Img 		=	"skillicon1005.dds",
		TitleID 	=	2211,
		DescID 		=	2252,
		ControlDesc	=	397,

		UI_Pos_x 	=	110,
		UI_Pos_y 	=	505,
		UI_LineType =	19,	

		LockGroup = -1,
		
		Passive = TRUE,
		
    },
	
	-- 오오라 - 공격력 증가
    {		
		ID 			=	SID_LIME_NORMAL_AURA_OFFENCE_UP,
		GroupID 	=	SG_LIME_NORMAL_AURA_OFFENCE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_OFFENCE, },
		NextList 	=	{ },
		CharLv 		=	71,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1006.dds",
		TitleID 	=	2212,
		DescID 		=	2253,
		ControlDesc	=	397,

		UI_Pos_x 	=	157,
		UI_Pos_y 	=	250,
		UI_LineType =	2,

		LockGroup = -1,
    },
	
	-- 오오라 - 지속 피해
    {		
		ID 			=	SID_LIME_NORMAL_AURA_OFFENCE_CONTINUE,
		GroupID 	=	SG_LIME_NORMAL_AURA_OFFENCE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_OFFENCE, },
		NextList 	=	{ },
		CharLv 		=	74,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1007.dds",
		TitleID 	=	2213,
		DescID 		=	2254,
		ControlDesc	=	397,

		UI_Pos_x 	=	157,
		UI_Pos_y 	=	320,
		UI_LineType =	2,

		LockGroup = -1,
    },
	
	-- 오오라 - 크리티컬 확률 상승
    {		
		ID 			=	SID_LIME_NORMAL_AURA_OFFENCE_CRITICAL_UP,
		GroupID 	=	SG_LIME_NORMAL_AURA_OFFENCE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_OFFENCE, },
		NextList 	=	{ },
		CharLv 		=	77,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1008.dds",
		TitleID 	=	2214,
		DescID 		=	2255,
		ControlDesc	=	397,

		UI_Pos_x 	=	157,
		UI_Pos_y 	=	390,
		UI_LineType =	2,

		LockGroup = -1,
    },
	
	-- 오오라 - 방어력 증가
    {		
		ID 			=	SID_LIME_NORMAL_AURA_DEFENSE_UP,
		GroupID 	=	SG_LIME_NORMAL_AURA_DEFENSE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_DEFENSE, },
		NextList 	=	{ },
		CharLv 		=	35,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1009.dds",
		TitleID 	=	2215,
		DescID 		=	2256,
		ControlDesc	=	397,

		UI_Pos_x 	=	257,
		UI_Pos_y 	=	250,
		UI_LineType =	2,

		LockGroup = -1,
    },
	
	-- 오오라 - HP 회복 증가
    {		
		ID 			=	SID_LIME_NORMAL_AURA_DEFENSE_HP_RECOVERY,
		GroupID 	=	SG_LIME_NORMAL_AURA_DEFENSE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_DEFENSE, },
		NextList 	=	{ },
		CharLv 		=	38,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1010.dds",
		TitleID 	=	2216,
		DescID 		=	2257,
		ControlDesc	=	397,

		UI_Pos_x 	=	257,
		UI_Pos_y 	=	320,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 오오라 - 일시적인 강력 보호막
    {		
		ID 			=	SID_LIME_NORMAL_AURA_DEFENSE_SHEILD,
		GroupID 	=	SG_LIME_NORMAL_AURA_DEFENSE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_DEFENSE, },
		NextList 	=	{ },
		CharLv 		=	41,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1011.dds",
		TitleID 	=	2217,
		DescID 		=	2258,
		ControlDesc	=	397,

		UI_Pos_x 	=	257,
		UI_Pos_y 	=	390,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 오오라 - MP 회복량 증가
    {		
		ID 			=	SID_LIME_NORMAL_AURA_ASSISTANT_MP_RECORVERY_UP,
		GroupID 	=	SG_LIME_NORMAL_AURA_ASSISTANT_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_ASSISTANT, },
		NextList 	=	{ },
		CharLv 		=	55,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1012.dds",
		TitleID 	=	2218,
		DescID 		=	2259,
		ControlDesc	=	397,

		UI_Pos_x 	=	357,
		UI_Pos_y 	=	250,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 오오라 - 필살기 MP 소모량 감소
    {		
		ID 			=	SID_LIME_NORMAL_AURA_ASSISTANT_SP_MANA_DOWN,
		GroupID 	=	SG_LIME_NORMAL_AURA_ASSISTANT_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_ASSISTANT, },
		NextList 	=	{ },
		CharLv 		=	58,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1013.dds",
		TitleID 	=	2219,
		DescID 		=	2260,
		ControlDesc	=	397,

		UI_Pos_x 	=	357,
		UI_Pos_y 	=	320,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 오오라 - 모든 MP 소모량 감소
    {		
		ID 			=	SID_LIME_NORMAL_AURA_ASSISTANT_ALL_MANA_DOWN,
		GroupID 	=	SG_LIME_NORMAL_AURA_ASSISTANT_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AURA_ASSISTANT, },
		NextList 	=	{ },
		CharLv 		=	61,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1014.dds",
		TitleID 	=	2220,
		DescID 		=	2261,
		ControlDesc	=	397,

		UI_Pos_x 	=	357,
		UI_Pos_y 	=	390,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 휠윈드 - 회전 수 증가
    {		
		ID 			=	SID_LIME_NORMAL_DASH_PLUS_COUNT,
		GroupID 	=	SG_LIME_NORMAL_DASH_PLUS_SELECT,
		PreList 	=	{SID_LIME_NORMAL_DASH_PLUS, },
		NextList 	=	{ },
		CharLv 		=	19,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1015.dds",
		TitleID 	=	2221,
		DescID 		=	2262,
		ControlDesc	=	397,

		UI_Pos_x 	=	457,
		UI_Pos_y 	=	250,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 휠윈드 - 마나 소모량 감소
    {		
		ID 			=	SID_LIME_NORMAL_DASH_PLUS_MP_DOWN,
		GroupID 	=	SG_LIME_NORMAL_DASH_PLUS_SELECT,
		PreList 	=	{SID_LIME_NORMAL_DASH_PLUS, },
		NextList 	=	{ },
		CharLv 		=	22,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1016.dds",
		TitleID 	=	2222,
		DescID 		=	2263,
		ControlDesc	=	397,

		UI_Pos_x 	=	457,
		UI_Pos_y 	=	320,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 휠윈드 - 점프 가능
    {		
		ID 			=	SID_LIME_NORMAL_DASH_PLUS_JUMP,
		GroupID 	=	SG_LIME_NORMAL_DASH_PLUS_SELECT,
		PreList 	=	{SID_LIME_NORMAL_DASH_PLUS, },
		NextList 	=	{ },
		CharLv 		=	25,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1017.dds",
		TitleID 	=	2223,
		DescID 		=	2264,
		ControlDesc	=	2273,

		UI_Pos_x 	=	457,
		UI_Pos_y 	=	390,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 도발 - 지속 시간 증가
    {		
		ID 			=	SID_LIME_NORMAL_AGGRESSIVE_TIME,
		GroupID 	=	SG_LIME_NORMAL_AGGRESSIVE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AGGRESSIVE, },
		NextList 	=	{ },
		CharLv 		=	63,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1018.dds",
		TitleID 	=	2224,
		DescID 		=	2265,
		ControlDesc	=	397,

		UI_Pos_x 	=	557,
		UI_Pos_y 	=	250,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 도발 - 범위 증가
    {		
		ID 			=	SID_LIME_NORMAL_AGGRESSIVE_SIZE,
		GroupID 	=	SG_LIME_NORMAL_AGGRESSIVE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AGGRESSIVE, },
		NextList 	=	{ },
		CharLv 		=	66,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1019.dds",
		TitleID 	=	2225,
		DescID 		=	2266,
		ControlDesc	=	397,

		UI_Pos_x 	=	557,
		UI_Pos_y 	=	320,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
	-- 도발 - 데미지 추가
    {		
		ID 			=	SID_LIME_NORMAL_AGGRESSIVE_DAMAGE,
		GroupID 	=	SG_LIME_NORMAL_AGGRESSIVE_SELECT,
		PreList 	=	{SID_LIME_NORMAL_AGGRESSIVE, },
		NextList 	=	{ },
		CharLv 		=	69,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1020.dds",
		TitleID 	=	2226,
		DescID 		=	2267,
		ControlDesc	=	397,

		UI_Pos_x 	=	557,
		UI_Pos_y 	=	390,
		UI_LineType =	2,	

		LockGroup = -1,
    },
	
    -- 최종 봉인기 (MP 게이지 4칸)
    {		
		ID 			=	SID_LIME_HIDDEN_MP4,
		GroupID 	=	SG_LIME_HIDDEN_MP4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	50,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon980.dds",
		TitleID 	=	2227,
		DescID 		=	2268,
		ControlDesc	=	430,

		UI_Pos_x 	=	190,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,
		Overlapped  = FALSE,

		LockGroup = -1,
		Passive = TRUE,
    },

    -- 최종 봉인기 (오오라)
    {		
		ID 			=	SID_LIME_HIDDEN_AURA,
		GroupID 	=	SG_LIME_HIDDEN_AURA,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	70,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1022.dds",
		TitleID 	=	2228,
		DescID 		=	2269,
		ControlDesc	=	127,

		UI_Pos_x 	=	350,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,
		Overlapped  = FALSE,

		LockGroup = -1,
		Passive = TRUE,				
    },

    -- 최종 봉인기 (4단 필살기)
    {		
		ID 			=	SID_LIME_HIDDEN_SPECIAL_4,
		GroupID 	=	SG_LIME_HIDDEN_SPECIAL_4,
		PreList 	=	{ },
		NextList 	=	{ },
		CharLv 		=	80,
		CharType 	=	16,
		Promotion 	=	-1,
		Enable  	=	TRUE,
		GoodsID 	=	{},
		Base 		=	FALSE,
		LearnSP 	=	0,

		Img 		=	"skillicon1023.dds",
		TitleID 	=	2229,
		DescID 		=	2270,
		ControlDesc	=	127,

		UI_Pos_x 	=	510,
		UI_Pos_y 	=	500,
		UI_LineType =	-1,

		LockGroup = -1,						
		Overlapped  = FALSE,		
		
		SlotSkill = 
		{
			Mana = 3.9,
			Motion = MID_LIME1_SPECIAL4,			
			CoolTime = 45,
			
			MonsterModeMana = 3.9,
			MonsterModeCoolTime = 45,
		},
    },
}