MaxCashingAgitInfo = 2000 -- �ִ� ĳ���� �� �ִ� ����Ʈ ������

MaxAgitNameLength = 12 -- ����Ʈ�� �ִ����(���� ����Ʈ)
MaxProfileLength = 50 -- �ִ� ������ ����
MaxUserCount = 20 -- ����Ʈ ���尡���� �ִ� �ο�
MaxPasswordLength = 8 -- �ִ� ��ȣ ����
MaxInvenSize = 100 -- ����Ʈ������Ʈ �κ��丮(â��� �ٸ�)
SellRewardFlowerCount = 1 -- ������Ʈ ������ �ǸŽ�, ���� �� ����
MaxFavoriteCount = 20 -- �ִ� ���ã�� ��������
ReserveRoomID = 500 -- ���ȣ ���� ����(�ʱⰪ,����ġ)
BuildPageTick = 1000 * 3 -- ������ ���� ��(ms)
AgitCountOnePage = 11 -- 1�������� ���÷��̵Ǵ� ����Ʈ ����

BrownieID = { 605570, } -- �����ID

-- Ʃ�丮��� ������ ����(1ȸ)
TutorialItem = {
	-- ItemID, Count, Period
	{ 605570, -1, -1 }, -- �÷ζ�(�����)
	-- { 605260, 5, -1 }, --����
	{ 605630, -1, -1 }, -- ���� ��Ź
	{ 605650, -1, -1 }, -- ���� ����
}

RankDBUpdateHour = 8 -- DB���� ���� �ۿ��� �ð�(��)
MaxGuestbookMsgLenth = 60 -- ������ �ִ� �޼��� ����
GuestbookWriteTerm = 1000 * 10 -- ������ �۾��� �ֱ� (ms)
GuestBookPageSize = 100 -- Ŭ���̾�Ʈ�� ���޵Ǵ� ������ �� ����
GuestbookWritePerDay = 3 -- ���� ������ ���ۼ� ����

TrainingItemID = 605300 -- �Ʒü� ItemID
TrainingMaterialItemID = 605320 -- ���� ItemID
TrainingExpRatio = 3 -- �Ʒü� ����1���� ����ġ ����
TrainingMaterialRegenTerm = 5 -- ���� �߰��Ǵ� �ð� (��)
TrainingMaterialLimit = 10 -- �ִ� ���̴� ���۰���

-- ĳ�� �������̳�, ����Ʈ�������� ������ �� ���� ��ġ ������Ʈ�� ���⿡ ���.
ObjectItemID = {
	605300, -- �Ʒü�
	605310, -- ������
}

CharacterOpenCost = 1 -- ĳ���͹�ġ ���½� �ʿ��� ���� ����
CharacterSpeechLength = 30 -- ��ġĳ���� ��� ��������

-- ���ȱ� �ȵǴ� ������Ʈ ItemID
NotResellObjectItemID = {
    605280, -- �Ϲ�ȭ��
	605260, -- ����
	605270, -- ��
}

-- �κ��丮 ������ ����
ObjectItemInvenLimit = {
	-- ItemID, �ִ뺸����(1�̻�)
    { 605290, 2 }, -- ������ ����� ȭ�� 2�� 
    { 606260, 5 }, -- �Ͼ� ���� ���� 5�� 
    { 606270, 5 }, -- �Ͼ� �߰� ���� 5��
    { 667900, 1 }, -- ��� 1��
    { 667910, 1 }, -- �ֻ��� 1��
    { 605320, 900 }, -- �����丷 900��
}
