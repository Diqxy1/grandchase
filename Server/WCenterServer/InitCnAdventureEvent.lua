--[[
�̺�Ʈ �Ⱓ �� ON/OFF ������ DB���� �մϴ�
������: ����ö, �迵ȣ, ������, ���翵, ������
--]]

-- /////// ENUM ���� ///////

-- EGCContinent
GC_CONTINENT_INVALID       = -1
GC_CONTINENT_BERMESIAH     = 0
GC_CONTINENT_SILVERLAND    = 1
GC_CONTINENT_ELIA          = 2
GC_CONTINENT_XENIA         = 3
GC_CONTINENT_ACHAEMEDIA    = 4
GC_CONTINENT_ATUM          = 5

-- EGCCharType
GC_CHAR_ALL              = -1   -- ������ �����
GC_CHAR_ELESIS           = 0
GC_CHAR_LIRE             = 1
GC_CHAR_ARME             = 2
GC_CHAR_LAS              = 3
GC_CHAR_RYAN             = 4
GC_CHAR_RONAN            = 5
GC_CHAR_AMY              = 6
GC_CHAR_JIN              = 7
GC_CHAR_SIEG             = 8
GC_CHAR_MARI             = 9
GC_CHAR_DIO              = 10
GC_CHAR_ZERO             = 11
GC_CHAR_LEY              = 12
GC_CHAR_LUPUS            = 13
GC_CHAR_RIN              = 14
GC_CHAR_ASIN             = 15

-- EGCGameMode
GC_GM_QUEST0            = 7    --// �����޽þ� �÷��� ��
GC_GM_QUEST1            = 8    --// �����޽þ� �÷��� ž
GC_GM_QUEST2            = 9    --// �����޽þ� ������ �ܰ�
GC_GM_QUEST3            = 10   --// �����޽þ� ĳ����ġ
GC_GM_QUEST4            = 11   --// �����޽þ� ��ũ ���
GC_GM_QUEST5            = 12   --// �����޽þ� ���� ����
GC_GM_QUEST6            = 13   --// �����޽þ� ������ ��
GC_GM_QUEST7            = 14   --// �����޽þ� �ͼ��� ���
GC_GM_QUEST8            = 15   --// �����޽þ� ������ ��
GC_GM_QUEST9            = 16   --// �����޽þ� ������ ����
GC_GM_QUEST10           = 17   --// �����޽þ� ������ ����
GC_GM_QUEST11           = 18   --// �����޽þ� ���������� ��
GC_GM_QUEST12           = 19   --// ������ ���������� �ٴ�
GC_GM_QUEST13           = 20   --// ������ ī��Ű ��ä
GC_GM_QUEST14           = 21   --// ������ ���� ���
GC_GM_QUEST15           = 22   --// ������ �� �긴��
GC_GM_QUEST16           = 23   --// ������ ī�������� ��
GC_GM_QUEST17           = 24   --// ������ ī���� ������ ����
GC_GM_QUEST18           = 25   --// ������ ī���� ������ ����
GC_GM_QUEST19           = 26   --// ������ �����޽þ��� ����
GC_GM_QUEST20           = 27   --// ���Ͼ� ���Ͼ� ������
GC_GM_QUEST21           = 30   --// ���Ͼ� ��ȯ�� ���� (�ŽŽ� ����Ʈ ��)
GC_GM_QUEST22           = 36   --// ���Ͼ� �ҽ��� ����
GC_GM_QUEST23           = 39    --// �ǹ� ���� ��Ʈ�� ��
GC_GM_QUEST24           = 40    --// �ǹ� �ǹ������� ����
GC_GM_QUEST25           = 41    --// �ǹ� �̸��� ȣ��
GC_GM_QUEST26           = 42    --// ���Ͼ� �̱״Ͻ��� ���
GC_GM_QUEST27           = 43    --// �ǹ� ������ ��
GC_GM_QUEST28           = 44    --// �ǹ� ������ ��
GC_GM_QUEST29           = 45    --// �ǹ� �巹��ũ ���� ����
GC_GM_QUEST30           = 46    --// �ǹ� ������ ���
GC_GM_QUEST31           = 47    --// ���Ͼ� ������ ����
GC_GM_QUEST32           = 48    --// ���Ͼ� �ı��� ���� (����)
GC_GM_QUEST33           = 49    --// ���Ͼ� ������ ��
GC_GM_QUEST34           = 50    --// ���Ͼ� ������ ��
GC_GM_QUEST35           = 51    --// �����޽þ� ���� ����
GC_GM_QUEST36           = 52    --// �ǹ����� ���� ����
GC_GM_QUEST37           = 53    --// ������ ���� ����
GC_GM_QUEST38           = 54    --// ���Ͼ� ���� ����
GC_GM_QUEST39           = 55    --// ���� ��� �뿪��
GC_GM_QUEST40           = 56    --// ���� ���Ͼ��� �ͳ�
GC_GM_QUEST41           = 57    --// ���� ��������
GC_GM_QUEST42           = 58    --// ���ɸ޵�� ����� ���ʱ���
GC_GM_QUEST43           = 59    --// ���� �ű�� �縷
GC_GM_QUEST44           = 60    --// ���ɸ޵�� ����� ���޷�
GC_GM_QUEST45           = 61    --// ���ɸ޵�� ��� �ظ�
GC_GM_QUEST46           = 62    --// �������� ( �ĸ��� ���� )
GC_GM_QUEST47           = 63    --// �������� ( ������ �뱤�� )
GC_GM_QUEST48           = 64    --// ���ɸ޵�� ��� �ձ��� ����
GC_GM_QUEST49           = 67    --// Event Carnival
GC_GM_QUEST50           = 68    --// Event RelayDungeon
GC_GM_QUEST51           = 69    --// �̺�Ʈ �������� (��Ŀ��)
GC_GM_QUEST52           = 70    --// ������ ��������
GC_GM_QUEST53           = 71    --// Monster Train

-- ///// Enum �� //////


TickGap = 1000 * 60     -- �̺�Ʈ �������� Ȯ���� �ֱ�(�⺻ 1�� ���� Ȯ��)


AdventureEventInfo =
{
    -- /////////////////////////////////// ���� ���� /////////////////////////////////
    MonsterInfo =
    {
        -- // 7�� ��������(è�Ǿ� ��常 �����Ѵ�) 1�� ���� ������ 30% Ȯ���� ����ǰ 1�� ȹ�� ����.. BiteMeString�� ���� ������� �ʴ´�
        {
            DungeonID = GC_GM_QUEST0,
            Monsters =
            {
                -- �� ������ ��� ���Ͱ�, ��� ����� ��� ����ǰ��, ���� Ȯ����, �� ���� �ִ��� ����
                { MonsterID = 1, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 2, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 3, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
            },
        },
        {
            DungeonID = GC_GM_QUEST1,
            Monsters =
            {
                { MonsterID = 4, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 5, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 6, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
            },
        },
    },
    
    -- ////////////////////////////////// ��� ���� ���� //////////////////////////////////
    ContinentRewardInfo =
    {
        {
            ContinentID = GC_CONTINENT_BERMESIAH,
            CollectItems =
            {   -- ����ǰ ������ �ִ� 9�� ������ �����մϴ�
                { CollectID = 0, DisplayItemID = 565660, MaxCount = 311 },
                { CollectID = 1, DisplayItemID = 565660, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 627290, MaxCount = 33 },
                { CollectID = 3, DisplayItemID = 8880, MaxCount = 33 },
                { CollectID = 4, DisplayItemID = 627300, MaxCount = 12 },
                { CollectID = 5, DisplayItemID = 287060, MaxCount = 77 },
                { CollectID = 6, DisplayItemID = 565670, MaxCount = 3 },
                { CollectID = 7, DisplayItemID = 102030, MaxCount = 22 },
                { CollectID = 8, DisplayItemID = 102030, MaxCount = 1 },
            },
            Rewards =
            {
                { 8880, -1, -1, 100, -1 },
                -- ItemID, Grade, Period, Duration, ItemLevel... �� ������ ����
                -- ���� ItemLevel�� �������� ����.. -1�� �־��ּ���
            },
        },
        {
            ContinentID = GC_CONTINENT_SILVERLAND,
            CollectItems =
            {   -- ����ǰ ������ �ִ� 9�� ������ �����մϴ�
                { CollectID = 0, DisplayItemID = 802340, MaxCount = 1 },
                { CollectID = 1, DisplayItemID = 791590, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 802340, MaxCount = 3 },
                { CollectID = 3, DisplayItemID = 791590, MaxCount = 5 },
                { CollectID = 4, DisplayItemID = 802340, MaxCount = 22 },
                { CollectID = 5, DisplayItemID = 802340, MaxCount = 2 },
                { CollectID = 6, DisplayItemID = 8880, MaxCount = 11 },
            },
            Rewards =
            {
                { 8880, -1, -1, 200, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_ELIA,
            CollectItems =
            {   -- ����ǰ ������ �ִ� 9�� ������ �����մϴ�
                { CollectID = 0, DisplayItemID = 565680, MaxCount = 1 },
                { CollectID = 1, DisplayItemID = 762910, MaxCount = 2 },
                { CollectID = 2, DisplayItemID = 646990, MaxCount = 3 },
                { CollectID = 3, DisplayItemID = 645690, MaxCount = 4 },
                { CollectID = 4, DisplayItemID = 506030, MaxCount = 5 },
                { CollectID = 5, DisplayItemID = 504550, MaxCount = 6 },
                { CollectID = 6, DisplayItemID = 503040, MaxCount = 7 },
                { CollectID = 7, DisplayItemID = 504070, MaxCount = 8 },
                { CollectID = 8, DisplayItemID = 506090, MaxCount = 9 },
            },
            Rewards =
            {
                { 8880, -1, -1, 300, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_XENIA,
            CollectItems =
            {   -- ����ǰ ������ �ִ� 9�� ������ �����մϴ�
                { CollectID = 0, DisplayItemID = 8880, MaxCount = 22 },
                { CollectID = 1, DisplayItemID = 8880, MaxCount = 7 },
                { CollectID = 2, DisplayItemID = 8880, MaxCount = 22 },
                { CollectID = 3, DisplayItemID = 8880, MaxCount = 9 },
            },
            Rewards =
            {
                { 8880, -1, -1, 400, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_ACHAEMEDIA,
            CollectItems =
            {   -- ����ǰ ������ �ִ� 9�� ������ �����մϴ�
                { CollectID = 0, DisplayItemID = 620500, MaxCount = 22 },
                { CollectID = 1, DisplayItemID = 620510, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 620520, MaxCount = 22 },
                { CollectID = 3, DisplayItemID = 620530, MaxCount = 22 },
                { CollectID = 4, DisplayItemID = 621140, MaxCount = 22 },
                { CollectID = 5, DisplayItemID = 604000, MaxCount = 22 },
                { CollectID = 6, DisplayItemID = 585030, MaxCount = 22 },
                { CollectID = 7, DisplayItemID = 603310, MaxCount = 22 },
                { CollectID = 8, DisplayItemID = 603320, MaxCount = 22 },
            },
            Rewards =
            {
                { 8880, -1, -1, 500, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_ATUM,
            CollectItems =
            {   -- ����ǰ ������ �ִ� 9�� ������ �����մϴ�
                { CollectID = 0, DisplayItemID = 395110, MaxCount = 3 },
                { CollectID = 1, DisplayItemID = 395100, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 395090, MaxCount = 9 },
                { CollectID = 3, DisplayItemID = 395080, MaxCount = 2 },
                { CollectID = 4, DisplayItemID = 395070, MaxCount = 4 },
                { CollectID = 5, DisplayItemID = 395120, MaxCount = 22 },
                { CollectID = 6, DisplayItemID = 394300, MaxCount = 5 },
                { CollectID = 7, DisplayItemID = 394310, MaxCount = 8 },
                { CollectID = 8, DisplayItemID = 394320, MaxCount = 7 },
            },
            Rewards =
            {
                { 8880, -1, -1, 600, -1 },
            },
        },
    },
    
    -- ////////////////////////////////// ���� ���� ���� //////////////////////////////////
    FinalRewardInfo =
    {
        -- ���� ������ �� ���� �����۸� �����ϴ�
        102030, -1, -1, 1000, -1,
        -- ItemID, Grade, Period, Duration, ItemLevel... �� ������ ����
        -- ���� ItemLevel�� �������� ����.. -1�� �־��ּ���
    },
}
