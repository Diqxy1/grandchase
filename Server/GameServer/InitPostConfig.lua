TRUE = 1
FALSE = 0

-- ������(��ü) ������� ����
PostEnable = FALSE

-- �������
SantaPost = FALSE -- ��Ÿ�ý��� �������� ����ϴ°�?
NewPostAlarm = TRUE -- ���� ������, �ǽð����� �˷��� ���ΰ�?
UserSendPost = FALSE -- ���� �������� ��� ����� ���ΰ�?
UserPostItem = FALSE -- ���� ��������� ������ ÷�� ����� ���ΰ�?
UserPostGP = FALSE -- ���� ��������� GP ÷�� ����� ���ΰ�?

-- �ý��ۼ���
CheckNewPostTickGap = 1000 * 60 * 3 -- �������� ������ �� ���� DB���� �޾ƿ��� �ֱ�(ms����)
OnePageLetterCount = 5 -- 1�������� �����ִ� ���� ����
NewLetterTitleCount = 5 -- �ѹ��� �����ִ� �űԿ��� ����

-- ������
TitleLength = 20 -- ���� ���� ��������
MessageLength = 200 -- ���� ���� ��������
LetterCost = 1000 -- ���� �⺻ ������
fGPCostRatio = 0.05 -- GP÷�ν� ÷��GP�� ���� ������ ����
fItemCostRatio = 0.05 -- ������÷�ν� ÷�ξ����ۿ� ���� ������ ����
SystemPostStorageTime = 30 -- �ý��ۿ��� ������� (�ϴ���,-1:��������)
UserPostStorageTime = -1 -- �������� ������� (�ϴ���,-1:��������)

-- ÷���� �� ���� ItemID ����Ʈ (PC�� �������� �ڵ�󿡼� ÷������)
NotPostItemID = {
	--[[ (����)
	605280, -- �Ϲ�ȭ��
	605260, -- ����
	--]]
}
