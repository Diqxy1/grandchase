TRUE 	=	 1;
FALSE 	=	 0;

X_GAP = 63;
Y_GAP = 63;
X_GAP_QUICKSLOT = 63;
Y_GAP_QUICKSLOT = 70;

X_GAP_LONG = 77;

--------------------------------------------------------------------------------------------------------------------------
------------ ���۽� 
--------------------------------------------------------------------------------------------------------------------------
SkillTree 	=
{
	-- �޹�������
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

		Img 		=	"skillicon842.dds",
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

	-- Ʈ�� ����
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

		Img 		=	"skillicon843.dds",
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
	
	-- ����ũ �� ����
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

		Img 		=	"skillicon844.dds",
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
	
	-- ����ũ
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

		Img 		=	"skillicon845.dds",
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
	
	-- �̼� ���� 1
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

		Img 		=	"skillicon846.dds",
		TitleID 	=	1909,
		DescID 		=	1952,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	290,
		UI_LineType =	19,

		LockGroup = -1,
		Passive = TRUE,
    },
	
	-- ��� ȸ��
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

		Img 		=	"skillicon847.dds",
		TitleID 	=	1910,
		DescID 		=	1953,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	290,
		UI_LineType =	20,

		LockGroup = -1,
    },
	
	-- ���Ŀ�
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

		Img 		=	"skillicon848.dds",
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
	
	-- ���Ŀ� ������ ����
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
		
		Img 		=	"skillicon849.dds",
		TitleID 	=	1912,
		DescID 		=	1955,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	380,
		UI_LineType =	19,		

		LockGroup = -1,
    },
	
	-- Ʈ����
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

		Img 		=	"skillicon850.dds",
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
	
	-- �������� ������ ����
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

		Img 		=	"skillicon851.dds",
		TitleID 	=	1914,
		DescID 		=	1957,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },

	-- MP ȸ�� ���� 1
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
	
		Img 		=	"skillicon852.dds",
		TitleID 	=	1915,
		DescID 		=	1958,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- �� Ÿ��
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

		Img 		=	"skillicon853.dds",
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
	
	-- �� Ÿ�� ������ ����
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
		
		Img 		=	"skillicon854.dds",
		TitleID 	=	1917,
		DescID 		=	1960,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
	-- �״� ���̵�
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

		Img 		=	"skillicon855.dds",
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
	
	-- �̼� ���� 2
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

		Img 		=	"skillicon856.dds",
		TitleID 	=	1919,
		DescID 		=	1962,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- ���� ����
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

		Img 		=	"skillicon857.dds",
		TitleID 	=	1920,
		DescID 		=	1963,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	22,		

		LockGroup = -1,
    },

	-- �ҿ� ��Ŭ��
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

		Img 		=	"skillicon858.dds",
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
	
	-- �ҿ� ��Ŭ�� ���� �ð� ����
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

		Img 		=	"skillicon859.dds",
		TitleID 	=	1922,
		DescID 		=	1965,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,		

		LockGroup = -1,
    },
	
	-- �״� ���̵� 2
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

		Img 		=	"skillicon860.dds",
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
	
	-- �״� ���̵� ������ ����
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

		Img 		=	"skillicon861.dds",
		TitleID 	=	1924,
		DescID 		=	1967,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- MP ȸ�� ���� 2
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

		Img 		=	"skillicon862.dds",
		TitleID 	=	1925,
		DescID 		=	1968,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- ũ����Ʈ ����
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

		Img 		=	"skillicon863.dds",
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
	
	-- ũ����Ʈ ���� ������ ����
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

		Img 		=	"skillicon864.dds",
		TitleID 	=	1927,
		DescID 		=	1970,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
	-- ����
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

		Img 		=	"skillicon865.dds",
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
	
	-- �̼� ���� 3
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

		Img 		=	"skillicon866.dds",
		TitleID 	=	1929,
		DescID 		=	1972,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- ���� ��� Ƚ�� ����
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

		Img 		=	"skillicon867.dds",
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
			
			-- SID / Ű �Է� ��� ���� FRM / Ű �Է� ��� �� ������ / ��� Change ������
			ComboSkill = {  {SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 40, 35, },
							{SID_LUPUS_NORMAL_RUPTURE, 0, 0, 0, },
							},
		},
    },
	
	-- �Ͽ︵ ����
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

		Img 		=	"skillicon868.dds",
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
	
	-- �Ͽ︵ ���� ������ ����
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

		Img 		=	"skillicon869.dds",
		TitleID 	=	1932,
		DescID 		=	1975,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- ������
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

		Img 		=	"skillicon870.dds",
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
	
	-- ���� ������ ����
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

		Img 		=	"skillicon871.dds",
		TitleID 	=	1934,
		DescID 		=	1977,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- MP ȸ�� ���� 3
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

		Img 		=	"skillicon872.dds",
		TitleID 	=	1935,
		DescID 		=	1978,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- ���� �Ž�Ʈ
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

		Img 		=	"skillicon873.dds",
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
	
	-- ���� �Ž�Ʈ ������ ����
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

		Img 		=	"skillicon874.dds",
		TitleID 	=	1937,
		DescID 		=	1980,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
	
	-- �ҿ� �ƺ���
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

		Img 		=	"skillicon875.dds",
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
	
	-- �̼� ���� 4
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

		Img 		=	"skillicon876.dds",
		TitleID 	=	1939,
		DescID 		=	1982,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- �ҿ� �ƺ��� ��� Ƚ�� ����
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

		Img 		=	"skillicon877.dds",
		TitleID 	=	1940,
		DescID 		=	1983,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120 + 120,
		UI_Pos_y 	=	220,
		UI_LineType =	22,

		LockGroup = -1,
    },
	
	-- ����
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

		Img 		=	"skillicon878.dds",
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
	
	-- ���� ������ ����
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

		Img 		=	"skillicon879.dds",
		TitleID 	=	1942,
		DescID 		=	1985,
		ControlDesc	=	430,

		UI_Pos_x 	=	230 + 120,
		UI_Pos_y 	=	220 + Y_GAP + Y_GAP,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- ������
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

		Img 		=	"skillicon880.dds",
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
	
	-- �ҿ� �ƺ��� ������ ����
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

		Img 		=	"skillicon881.dds",
		TitleID 	=	1944,
		DescID 		=	1987,
		ControlDesc	=	430,

		UI_Pos_x 	=	170,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- MP ȸ�� ���� 4
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

		Img 		=	"skillicon882.dds",
		TitleID 	=	1945,
		DescID 		=	1988,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	19,

		LockGroup = -1,
    },
	
	-- �ͽ�����
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

		Img 		=	"skillicon883.dds",
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
	
	-- �ͽ����� ������ ����
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

		Img 		=	"skillicon884.dds",
		TitleID 	=	1947,
		DescID 		=	1990,
		ControlDesc	=	430,

		UI_Pos_x 	=	170 + 120 + 120 + 120,
		UI_Pos_y 	=	470,
		UI_LineType =	-1,

		LockGroup = -1,
    },
}