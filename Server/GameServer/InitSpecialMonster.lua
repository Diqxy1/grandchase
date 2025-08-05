-- AutoLevel�� TRUE�� ���, ���Ͱ� ���� ������ �ּ� ���������� �����˴ϴ�.
-- AutoLevel�� FALSE�� ���, MonsterLevel������ ������ ������ �߰��˴ϴ�.

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
	ActiveEvent = TRUE,                    -- TRUE, FALSE/ ��ü �̺�Ʈ ON/OFF
    {
		GameMode = 9,
		Difficult = 255, -- ���̵�
        MonsterID = 278,
		MonsterLevel = 50,
		PropertyInfo = { -- ���� �Ӽ� ����.
			bEnable = FALSE,
			PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- �Ӽ� ���� Ȯ���� ����.
			Property = {
				{ {CP_ATTACK, 100} },
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			}
		},
    },
}