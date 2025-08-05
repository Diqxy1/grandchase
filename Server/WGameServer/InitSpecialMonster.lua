-- AutoLevel�� TRUE�� ���, ���Ͱ� ���� ������ �ּ� ���������� �����˴ϴ�.
-- AutoLevel�� TRUE�� ���, MonsterLevel������ ������ ������ �߰��˴ϴ�.

--[[ ���Ӹ��, ID, ���� �̸�
GC_GM_QUEST0   = 7,     // �÷��� ��
GC_GM_QUEST1   = 8,     // �÷��� ž
GC_GM_QUEST2   = 9,     // ������ �ܰ�
GC_GM_QUEST3   = 10,    // ĳ����ġ
GC_GM_QUEST4   = 11,    // ��ũ���
GC_GM_QUEST5   = 12,    // ���� ����
GC_GM_QUEST6   = 13,    // ������ ��
GC_GM_QUEST7   = 14,    // �ͼ��� ���
GC_GM_QUEST8   = 15,    // ������ ��
GC_GM_QUEST9   = 16,    // ������ ����
GC_GM_QUEST10  = 17,    // ������ ����
GC_GM_QUEST11  = 18,    // ���������� ��

GC_GM_QUEST12  = 19,    // ���������� �ٴ�
GC_GM_QUEST13  = 20,    // ī��Ű ��ä
GC_GM_QUEST14  = 21,    // ���� ���
GC_GM_QUEST15  = 22,    // �� �긴��
GC_GM_QUEST16  = 23,    // ī�������� ��
GC_GM_QUEST17  = 24,    // ī���� ������ ����
GC_GM_QUEST18  = 25,    // ī���� ������ ����
GC_GM_QUEST19  = 26,    // �����޽þ��� ����

GC_GM_QUEST20  = 27,    // ���Ͼ� ������
GC_GM_QUEST21  = 30,    // ��ȯ�� ����
GC_GM_QUEST22  = 36,    // �ҽ��� ����
GC_GM_QUEST26  = 42,    // �̱״Ͻ� ���
GC_GM_QUEST31  = 47,    // ������ ����
GC_GM_QUEST32  = 48,    // �ı��� ����
GC_GM_QUEST33  = 49,    // ������ ��
GC_GM_QUEST34  = 50,    // ������ ��

GC_GM_QUEST23  = 39,    // ���� ��Ʈ�� ��
GC_GM_QUEST24  = 40,    // �ǹ�������
GC_GM_QUEST25  = 41,    // �̸��� ȣ��
GC_GM_QUEST27  = 43,    // ������ ��
GC_GM_QUEST28  = 44,    // ������ ��
GC_GM_QUEST29  = 45,    // �巹��ũ ���� ����
GC_GM_QUEST30  = 46,    // ������ ���

GC_GM_QUEST39  = 55,    // ��� �뿪�� (GoblinLaborCamp)
GC_GM_QUEST40  = 56,    // ���Ͼ��� �ͳ� (UndergroundTunnel)
GC_GM_QUEST41  = 57,    // ���� ���� (KungjiVillage)
GC_GM_QUEST43  = 59,    // �ű�� �縷 (MirageDesert)

GC_GM_QUEST42  = 58,    // ����� ���ʱ��� (DwarvenBase)
GC_GM_QUEST44  = 60,    // ����� ���޷� (DwarfSupplyRoute)
GC_GM_QUEST45  = 61,    // ��� �ظ� (ThunderHammer)
GC_GM_QUEST49  = 67,    // �Ǹ��� ��Ŀ��
GC_GM_QUEST50  = 68,    // �̺�Ʈ �����̴���

--]]

TRUE = 1
FALSE = 0

-- Ư���ɷ� �Ӽ�
CP_ATTACK           = 1

CP_DEFENCE          = 2
CP_HP_RECOVERY      = 4

CP_SUPER_ARMOR      = 8
CP_FAST_STAND       = 16
-- �߰��� è�Ǿ� �Ӽ�
CP_ARROW_DEFENSE    = 256  -- ȭ�� ���
CP_SHOCKWAVE        = 512  -- �����

CP_ATTACK_CURES     = 32
CP_ATTACK_STONE     = 64
CP_ATTACK_ICE       = 128

-- GameMode, Difficult���� Key���̴�.
SpecialMonster =
{
	ActiveEvent = TRUE,                    -- TRUE, TRUE/ ��ü �̺�Ʈ ON/OFF
    {	 --	������ �ܰ�			
		GameMode =	9	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	9	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	ĳ����ġ			
		GameMode =	10	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	11	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	��ũ���			
		GameMode =	11	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	13	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	���� ����			
		GameMode =	12	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	15	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ��			
		GameMode =	13	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	17	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�ͼ��� ���			
		GameMode =	14	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	19	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ��			
		GameMode =	15	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	21	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ����			
		GameMode =	16	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	23	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ����			
		GameMode =	17	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	25	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	���������� ��			
		GameMode =	18	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	548	,	
		MonsterLevel =	27	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 2, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	���������� �ٴ�			
		GameMode =	19	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	43	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	ī��Ű ��ä			
		GameMode =	20	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	45	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	���� ���			
		GameMode =	21	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	47	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�� �긴��			
		GameMode =	22	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	49	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	ī�������� ��			
		GameMode =	23	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	51	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	ī���� ������ ����			
		GameMode =	24	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	53	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	ī���� ������ ����			
		GameMode =	25	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	55	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�����޽þ��� ����			
		GameMode =	26	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	550	,	
		MonsterLevel =	57	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	���Ͼ� ������			
		GameMode =	27	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	59	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	��ȯ�� ����			
		GameMode =	30	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	61	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�ҽ��� ����			
		GameMode =	36	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	63	,
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������Ʈ�� ��			
		GameMode =	39	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	29	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�ǹ��������� ����			
		GameMode =	40	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	31	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�̸��� ȣ��			
		GameMode =	41	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	33	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�̱״Ͻ��� ���			
		GameMode =	42	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	65	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ��			
		GameMode =	43	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	35	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ��			
		GameMode =	44	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	37	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�巹��ũ ���� ����			
		GameMode =	45	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	39	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ���			
		GameMode =	46	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	549	,	
		MonsterLevel =	41	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ����			
		GameMode =	47	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	67	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�ı��� ����			
		GameMode =	48	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	69	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ��			
		GameMode =	49	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	71	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	������ ��			
		GameMode =	50	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	551	,	
		MonsterLevel =	73	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 3, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	��� �뿪��			
		GameMode =	55	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	553	,	
		MonsterLevel =	75	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	���Ͼ��� �ͳ�			
		GameMode =	56	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	553	,	
		MonsterLevel =	77	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	��������			
		GameMode =	57	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	553	,	
		MonsterLevel =	79	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	����� ���ʱ���			
		GameMode =	58	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	552	,	
		MonsterLevel =	75	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	�ű�� �縷			
		GameMode =	59	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	553	,	
		MonsterLevel =	81	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	����� ���޷�			
		GameMode =	60	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	552	,	
		MonsterLevel =	77	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	��� �ظ�			
		GameMode =	61	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	552	,	
		MonsterLevel =	79	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
    {	 --	��� �ձ��� ����			
		GameMode =	64	,	
		Difficult = 255, -- ���̵�			
		MonsterID =	552	,	
		MonsterLevel =	81	,	
		PropertyInfo = { -- ���� �Ӽ� ����.			
			bEnable = TRUE,		
			PropertyCount = { { 4, 100 }, },    -- �Ӽ� ���� Ȯ���� ����.		
			Property = {		
				{ {CP_ATTACK, 100} },	
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 50} },	
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },	
				{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },	
			}		
		},			
    },					
}