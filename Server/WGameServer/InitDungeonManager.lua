-- ���� �Ŵ���
TRUE = 1
FALSE = 0

--[[ ���Ӹ��,ID,���� �̸�
    GC_GM_QUEST0            = 7,   // �÷��� ��
    GC_GM_QUEST1            = 8,   // �÷��� ž
    GC_GM_QUEST2            = 9,   // ������ �ܰ�
    GC_GM_QUEST3            = 10,  // ĳ����ġ
    GC_GM_QUEST4            = 11,  // ��ũ���
    GC_GM_QUEST5            = 12,  // ���� ����
    GC_GM_QUEST6            = 13,  // ������ ��
    GC_GM_QUEST7            = 14,  // �ͼ��� ���
    GC_GM_QUEST8            = 15,  // ������ ��
    GC_GM_QUEST9            = 16,  // ������ ����
    GC_GM_QUEST10           = 17,  // ������ ����
    GC_GM_QUEST11           = 18,  // ���������� ��
    GC_GM_QUEST12           = 19,  // ���������� �ٴ�
    GC_GM_QUEST13           = 20,  // ������ ���
    GC_GM_QUEST14           = 21,  // ���� ���
    GC_GM_QUEST15           = 22,  // �� �긴��
    GC_GM_QUEST16           = 23,  // ī�������� ��
    GC_GM_QUEST17           = 24,  // ī���� ������ ����
    GC_GM_QUEST18           = 25,  // ī���� ������ ����
    GC_GM_QUEST19           = 26,  // �����޽þ��� ����
    GC_GM_QUEST20           = 27,  // ���Ͼ� ������
    GC_GM_MONSTER_CRUSADER  = 28,  // ���� ������
    GC_GM_MONSTER_HUNT      = 29,  // ���� ����(����1.�̻��)
    GC_GM_QUEST21           = 30,  // �ŽŽ� ����Ʈ 
    GC_GM_DEATH_TEAM        = 31,  // ������ġ ��
    GC_GM_DEATH_SURVIVAL    = 32,  // ������ġ �����̹�
    GC_GM_MINIGAME_TREEDROP    = 33,   // �̴ϰ��� �볪�� ��������
    GC_GM_MINIGAME_BALLOON    = 34,   // �̴ϰ��� ǳ�� �Ͷ߸���
    GC_GM_MINIGAME_BUTTERFRY    = 35,   // �̴ϰ��� ���� ���
    GC_GM_QUEST22           = 36,  // �ҽ��� ����.... (����)
    GC_GM_ANGELS_EGG        = 37,   //õ���� ��
    GC_GM_CAPTAIN           = 38,  // ������
    GC_GM_QUEST23           = 39,  // �ǹ����� ���� ��Ʈ�� ��
    GC_GM_QUEST24           = 40,  // �ǹ����� �ǹ�������
    GC_GM_QUEST25           = 41,   // �ǹ����� �̸��� ȣ��
    GC_GM_QUEST26           = 42,   // ���Ͼ� �̱״Ͻ��� ���
    GC_GM_QUEST27           = 43,   // �ȹ����� ������ ��
	GC_GM_QUEST28           = 44,   // �ǹ����� ������ ��
	GC_GM_QUEST29           = 45,   // �ǹ����� �巹��ũ ���� ����
	GC_GM_QUEST30           = 46,   // �ǹ����� ������ ���
	GC_GM_QUEST31           = 47,   // ���Ͼ� ���� ������ ����
	GC_GM_QUEST32           = 48,   // ���Ͼ� ����
	GC_GM_QUEST33           = 49,   // ���Ͼ� ������
	GC_GM_QUEST34           = 50,   // ���Ͼ� ������ ��
	GC_GM_QUEST35           = 51,   // �����޽þ� ���� ����
	GC_GM_QUEST36           = 52,   // �ǹ����� ���� ����
	GC_GM_QUEST37           = 53,   // ������ ���� ����
	GC_GM_QUEST38           = 54,   // ���Ͼ� ���� ����
    GC_GM_QUEST39           = 55,   // ���� ��� �뿪��
    GC_GM_QUEST40           = 56,   // ���� ���Ͼ��� �ͳ�
    GC_GM_QUEST41           = 57,   // ���� ��������
    GC_GM_QUEST42           = 58,   // ���ɸ޵�� ����� ���ʱ���
    GC_GM_QUEST43           = 59,   // ���� �ű�� �縷
	GC_GM_QUEST44           = 60,   // ���ɸ޵�� ����� ���޷�
    GC_GM_QUEST45           = 61,   // ���ɸ޵�� ��� �ظ�
    GC_GM_QUEST46           = 62,   // �������� ( �ĸ��� ���� )
    GC_GM_QUEST47           = 63,   // �������� ( ������ �뱤�� )
    GC_GM_QUEST48           = 64,   // ��� �ձ��� ����
    GC_GM_DOTA              = 65,   // ������
    GC_GM_AGIT				= 66,   // ����Ʈ
    GC_GM_QUEST49       	= 67,   // �̺�Ʈ ����( ī�Ϲ� )
    GC_GM_QUEST50       	= 68,   // �̺�Ʈ ������ ����
    GC_GM_QUEST51       	= 69,   // �̺�Ʈ ���� ����
    GC_GM_QUEST52           = 70,   // ������ ���� ����( ���� ���� ) 1�ܰ� 
    GC_GM_QUEST53       	= 71,	 // ��迭�� 301 (GhostTrain)
    GC_GM_QUEST54       	= 72,	 // ����� ����� �ݶ�
    GC_GM_QUEST55           = 73,   // ī�翤 ������(KaruelBorder)
    GC_GM_QUEST56           = 74,   // ī�翤(Karuel)
    GC_GM_QUEST57           = 75,   // �̺�Ʈ����(�߽���)
    GC_GM_QUEST58           = 76,   // ���̵����
--]]

SELECT_REWARD = 1 -- ���� ���� Type
RESULT_REWARD = 2 -- ��� ���� Type
NONE_REWARD   = 3 -- ���� ���� Type

-- ���� �ִ� �ο� ����.
DungeonMaxUserInfo =
{
	-- ModeID,MaxUser
	{ ModeID = 76, MaxUser = 6,},
	{ ModeID = 83, MaxUser = 2,},
	{ ModeID = 84, MaxUser = 1,},
	{ ModeID = 85, MaxUser = 1,},
	{ ModeID = 86, MaxUser = 1,},
	{ ModeID = 87, MaxUser = 1,},
	{ ModeID = 88, MaxUser = 1,},
	{ ModeID = 89, MaxUser = 1,},
	
	{ ModeID = 90, MaxUser = 1,},
	{ ModeID = 91, MaxUser = 1,},
	{ ModeID = 92, MaxUser = 1,},
	{ ModeID = 93, MaxUser = 1,},
	{ ModeID = 94, MaxUser = 1,},
	{ ModeID = 95, MaxUser = 1,},
	
}

-- ���� ��� ���� �ȵǴ� ��������.
-- �⺻ Default�� TRUE
DungeonModeChangeDisableInfo = { 76, 83, }

-- ���� ���� TYPE ����.
DungeonRewardTypeInfo = 
{
	{ 7, SELECT_REWARD },
	{ 8, SELECT_REWARD },
	{ 9, SELECT_REWARD },
	{ 10, SELECT_REWARD },
	{ 11, SELECT_REWARD },
	{ 12, SELECT_REWARD },
	{ 13, SELECT_REWARD },
	{ 14, SELECT_REWARD },
	{ 15, SELECT_REWARD },
	{ 16, SELECT_REWARD },
	{ 17, SELECT_REWARD },
	{ 18, SELECT_REWARD },
	{ 19, SELECT_REWARD },
	{ 20, SELECT_REWARD },
	{ 21, SELECT_REWARD },
	{ 22, SELECT_REWARD },
	{ 23, SELECT_REWARD },
	{ 24, SELECT_REWARD },
	{ 25, SELECT_REWARD },
	{ 26, SELECT_REWARD },
	{ 27, SELECT_REWARD },
	{ 28, SELECT_REWARD },
	{ 29, SELECT_REWARD },
	{ 30, SELECT_REWARD },
	{ 31, SELECT_REWARD },
	{ 32, SELECT_REWARD },
	{ 33, SELECT_REWARD },
	{ 34, SELECT_REWARD },
	{ 35, SELECT_REWARD },
	{ 36, SELECT_REWARD },
	{ 37, SELECT_REWARD },
	{ 38, SELECT_REWARD },
	{ 39, SELECT_REWARD },
	{ 40, SELECT_REWARD },
	{ 41, SELECT_REWARD },
	{ 42, SELECT_REWARD },
	{ 43, SELECT_REWARD },
	{ 44, SELECT_REWARD },
	{ 45, SELECT_REWARD },
	{ 46, SELECT_REWARD },
	{ 47, SELECT_REWARD },
	{ 48, SELECT_REWARD },
	{ 49, SELECT_REWARD },
	{ 50, SELECT_REWARD },
	{ 51, SELECT_REWARD },
	{ 52, SELECT_REWARD },
	{ 53, SELECT_REWARD },
	{ 54, SELECT_REWARD },
	{ 55, SELECT_REWARD },
	{ 56, SELECT_REWARD },
	{ 57, SELECT_REWARD },
	{ 58, SELECT_REWARD },
	{ 59, SELECT_REWARD },
	{ 60, SELECT_REWARD },
	{ 61, SELECT_REWARD },
	{ 62, RESULT_REWARD },
	{ 63, RESULT_REWARD },
	{ 64, SELECT_REWARD },
	{ 65, SELECT_REWARD },
	{ 66, SELECT_REWARD },
	{ 67, SELECT_REWARD },
	{ 68, SELECT_REWARD },
	{ 69, SELECT_REWARD },
	{ 70, RESULT_REWARD },
	{ 71, SELECT_REWARD },
	{ 72, SELECT_REWARD },
	{ 73, SELECT_REWARD },
	{ 74, SELECT_REWARD },
	{ 75, SELECT_REWARD },
	{ 76, RESULT_REWARD },
	{ 78, SELECT_REWARD },
	{ 79, SELECT_REWARD },
	{ 80, SELECT_REWARD },
	{ 81, RESULT_REWARD },
	{ 82, RESULT_REWARD },
	{ 83, SELECT_REWARD },
	{ 84, SELECT_REWARD },
	{ 85, SELECT_REWARD },
	{ 86, SELECT_REWARD },
	{ 87, SELECT_REWARD },
	{ 88, SELECT_REWARD },
	{ 89, RESULT_REWARD },
	{ 90, SELECT_REWARD },
	{ 91, SELECT_REWARD },
	{ 92, SELECT_REWARD },
	{ 93, SELECT_REWARD },
	{ 94, SELECT_REWARD },
	{ 95, SELECT_REWARD },
	{ 98, NONE_REWARD },
	{ 99, NONE_REWARD },
	{ 100, NONE_REWARD },
	{ 101, NONE_REWARD },
}
