--[[ ���Ӹ��, ID, ���� �̸�
GC_GM_QUEST0   = 7,    // �÷��� ��
GC_GM_QUEST1   = 8,    // �÷��� ž
GC_GM_QUEST2   = 9,    // ������ �ܰ�
GC_GM_QUEST3   = 10,   // ĳ����ġ
GC_GM_QUEST4   = 11,   // ��ũ���
GC_GM_QUEST5   = 12,   // ���� ����
GC_GM_QUEST6   = 13,   // ������ ��
GC_GM_QUEST7   = 14,   // �ͼ��� ���
GC_GM_QUEST8   = 15,   // ������ ��
GC_GM_QUEST9   = 16,   // ������ ����
GC_GM_QUEST10  = 17,   // ������ ����
GC_GM_QUEST11  = 18,   // ���������� ��
GC_GM_QUEST12  = 19,   // ���������� �ٴ�
GC_GM_QUEST13  = 20,   // ������ ���
GC_GM_QUEST14  = 21,   // ���� ���
GC_GM_QUEST15  = 22,   // �� �긴��
GC_GM_QUEST16  = 23,   // ī�������� ��
GC_GM_QUEST17  = 24,   // ī���� ������ ����
GC_GM_QUEST18  = 25,   // ī���� ������ ����
GC_GM_QUEST19  = 26,   // �����޽þ��� ����
GC_GM_QUEST20  = 27,   // ���Ͼ� ������
GC_GM_QUEST21  = 30,   // �ŽŽ� ����Ʈ ��
GC_GM_QUEST22  = 36,   // �ҽ��� ����.... (����)
GC_GM_QUEST23  = 39,   // ���� ��Ʈ�� ��
GC_GM_QUEST24  = 40,   // �ǹ�������
GC_GM_QUEST25  = 41,    // �̸��� ȣ��
GC_GM_QUEST26  = 42,    // �̱״Ͻ��� ���
GC_GM_QUEST27  = 43,    // ���� ����
GC_GM_QUEST28  = 44,    // �ǹ����� ������ ��
GC_GM_QUEST29  = 45,    // �ǹ����� �������
GC_GM_QUEST30  = 46,    // �ǹ����� ������ ���
GC_GM_QUEST31  = 47,    // ���Ͼ� ���� ������ ����
GC_GM_QUEST32  = 48,    // ���Ͼ� ����
GC_GM_QUEST33  = 49,    // ���Ͼ� ������
GC_GM_QUEST34  = 50,    // ���Ͼ� ������ ��
GC_GM_QUEST35  = 51,    // �����޽þ� ���� ����
GC_GM_QUEST36  = 52,    // �ǹ����� ���� ����
GC_GM_QUEST37  = 53,    // ������ ���� ����
GC_GM_QUEST38  = 54,    // ���Ͼ� ���� ����
GC_GM_QUEST39  = 55,    // GoblinLaborCamp
GC_GM_QUEST40  = 56,    // UndergroundTunnel
GC_GM_QUEST41  = 57,    // KungjiVillage
GC_GM_QUEST42  = 58,    // DwarvenBase
GC_GM_QUEST43  = 59,    // �ű�� �縷
GC_GM_QUEST44  = 60,    // ����� ���޷�
GC_GM_QUEST45  = 61,    // ��� �ظ�
GC_GM_QUEST46  = 62,    // ��������(�����޽þ�)
GC_GM_QUEST47  = 63,    // ��������
GC_GM_QUEST48  = 64,    // ��� �ձ��� ����
GC_GM_QUEST49  = 67,    // Event Carnival
GC_GM_QUEST50  = 68,    // Event RelayDungeon
GC_GM_QUEST51  = 69,    // Event Hero
GC_GM_QUEST52  = 70,    // ������ ��������
GC_GM_QUEST53  = 71,    // Monster Train
--]]

TRUE = 1
FALSE = 0

OpenAlarm = { 5, 3, 1, 0 }
CloseAlarm = { 5, 3, 1 }

MonsterExpRatio = 40      -- ���� ����ġ ������ ����
DungeonWinClearExpRatio = 40  -- ���� Ŭ���� ���� ����ġ �������
DungeonLoseClearExpRatio = 10  -- ���� Ŭ���� ���� ����ġ �������

HeroDungeon = {
    ActiveMode = TRUE,          -- ��ü ���ۿ���

    HeroPointItem = { 12490, 1, -1 },   -- ��������Ʈ ������ {ItemID,Count,Period}
    ResetTime = { 00, 00 },     -- �����ʱ�ȭ �ð� (00�� 00��)

	
	{
        ModeID = 62,           -- ���Ӹ��ID
        HeroPoint = 1,         -- ��������Ʈ ������ ���󰳼�(Count)
        MinLv = 40,            -- �ּ� ���尡�� ĳ���ͷ���
        MaxClearCount = 2,     -- ���� �ִ�Ŭ���� ���ɼ�
		MonsterLv = 0,         -- 1ȸ�̻� Ŭ������, ����Ǵ� ���� ���� (0�̸� ��������)
		ResetDifficulty = FALSE,-- �ִ볭�̵� Ŭ�����, ���̵� �ʱ�ȭ �Ǵ��� ����
		DifficultyCheck = FALSE,-- ���̵� ���� üũ�ϴ��� ����

		RewardCount = 0,       -- Ŭ����� ���� ȹ�氡���� ���� ����
		ClearReward = {
			-- { ItemID = 776750, Count = -1, Period = 7, IsLook = TRUE, },
		},

        -- ���½ð���
        OpenTime = {
			-- �ݵ�� 00�ú��� �ð� ������� ������ ��
			{ 00,00, 00,20 },
            { 01,00, 01,20 },
            { 02,00, 02,20 },
            { 03,00, 03,20 },
            { 04,00, 04,20 },
            { 05,00, 05,20 },
            { 06,00, 06,20 },
            { 07,00, 07,20 },
            { 08,00, 08,20 },
            { 09,00, 09,20 },
            { 10,00, 10,20 },
            { 11,00, 11,20 },
            { 12,00, 12,20 },
            { 13,00, 13,20 },
            { 14,00, 14,20 },
            { 15,00, 15,20 },
            { 16,00, 16,20 },
            { 17,00, 17,20 },
            { 18,00, 18,20 },
            { 19,00, 19,20 },
            { 20,00, 20,20 },
            { 21,00, 21,20 },
            { 22,00, 22,20 },
            { 23,00, 23,20 },
        },
    },
}
