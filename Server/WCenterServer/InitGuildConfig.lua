TRUE = 1
FALSE = 0

IT_PERIOD   = 0 -- �Ⱓ��/����
IT_QUANTITY = 1 -- ������ ������


MarkServerURL = "http://200.229.51.34/cp/GrandChase/"
MarkImageExt = { "jpg", "jpeg", "png", }
MarkDefaultName = "defaultmark.dds"
ImageBuffMinSize = 100
ImageBuffMaxSize = 1024 * 4

-- ��帶ũ ���ε� ������(�°ݽ� ���޼���)
GuildMarkUploadItemID = 206680
GuildMarkUploadItemFactor = 30
GuildMarkUploadItemType = IT_PERIOD
-- ���� ���� ������
GuildChangeNameItemID = 206670
GuildMarkUploadCashItemID = 0
GuildPageSize = 8
GuildNameMinLength = 2
GuildNameMaxLength = 12
GuildMasterMinLv = 30
GuildURLMinLength = 1
GuildURLMaxLength = 100
GuildCreateGP = 200000

OfficailGuildMemberNum = 5 -- ���� ��尡 �Ǳ����� �ɹ���
NewbieGuildRemoveTerm = 30 -- �Ż���� ������ ���ŵǱ� ���� ��¥��
KeywordMinLength = 2 -- �ּ� �˻��� ����
JoinerRemoveTerm = 15 -- ���δ��Ⱓ (������ ���Խ�û���ó��)

-- http://www.unicode.org/charts/ �� ����Ʈ ���� �ϼ���.
AllowUnicode = TRUE
Unicode = {
    { tonumber( '0030', 16 ), tonumber( '0039', 16 ), }, -- ����
    { tonumber( '0041', 16 ), tonumber( '005A', 16 ), }, -- ���ĺ� �빮��
    { tonumber( '0061', 16 ), tonumber( '007A', 16 ), }, -- ������ �ҹ���
    { tonumber( '00C0', 16 ), tonumber( '00FF', 16 ), }, -- ��ƾ-1
}

GuildInfoClearTerm = 1000 * 60 * 30 -- ������� ���� �ֱ�
MemberListClearTerm = 1000 * 60 * 30 -- �ɹ�����Ʈ ���� �ֱ�
MaxPageSize = 7 -- �ִ� ���Page ����
UpdatePointTerm = 1000 * 60 * 10

-- ��巹��/�ʿ�GPoint
GuildLvPoint =
{
    { 0,    0     },
    { 1,    1000  },
    { 2,    4000  },
    { 3,    10000 },
    { 4,    23000 },
    { 5,    42000 },
    { 6,    68000 },
    { 7,    110000 },
    { 8,    160000 },
    { 9,    250000 },
    { 10,   410000 },
}

-- ����
NONE                    = 0 -- ����
EXP_BONUS               = 1 -- ����ġ ���ʽ�
STORE_TAB               = 2 -- ���� �� �̿�
RUNE_ENCHANT_BONUS      = 3 -- �鰭ȭ ���ʽ�
ACCESSORY_ENCHANT_BONUS = 4 -- ����̰�ȭ ���ʽ�
MORE_EQUIP_POTION       = 5 -- �������� ����

-- ������ �� index
GUILD_TAB_1 = 0
GUILD_TAB_2 = 1
GUILD_TAB_3 = 2

-- ��巹��/��������/��������
GuildLvAdvantage = {
    { 0,    NONE,                    0 },  -- ����
    { 1,    EXP_BONUS,               0.05 },  -- EXP +5%
    { 2,    STORE_TAB,               GUILD_TAB_1 },  -- ȸ������(TabID:1) �����̿�
    { 3,    RUNE_ENCHANT_BONUS,      0.05 },  -- �鰭ȭ +5%
    { 4,    EXP_BONUS,               0.05 },  -- EXP +5% (����+10%)
    { 5,    ACCESSORY_ENCHANT_BONUS, 0.05 },  -- ����̰�ȭ +5%
    { 6,    STORE_TAB,               GUILD_TAB_2 },  -- ��������(TabID:2) �����̿�
    { 7,    MORE_EQUIP_POTION,       2 },  -- �������� +2��
    { 8,    RUNE_ENCHANT_BONUS,      0.05 },  -- �鰭ȭ +5% (����+10%)
    { 9,    ACCESSORY_ENCHANT_BONUS, 0.05 },  -- ����̰�ȭ +5% (����+10%)
    { 10,   EXP_BONUS,               0.05 },  -- EXP +5% (����+15%)
}

-- ���⿩��/����Ǵ� ��巹�� ����
ContributePointAdventageLv = {
    { 0,     0 },
    { 200,   1 },
    { 600,   2 },
    { 1200,  3 },
    { 2000,  4 },
    { 3000,  5 },
    { 5000,  6 },
    { 7000,  7 },
    { 9000,  8 },
    { 12000, 9 },
    { 15000, 10 },
}

ChannelGrade = {
        { ChannelType = 20, BeginPoint = 0, EndPoint = 1199 },
	{ ChannelType  = 18, BeginPoint = 1200, EndPoint = 1349 },
	{ ChannelType = 14, BeginPoint = 1350, EndPoint = 1499 },
	{ ChannelType = 10, BeginPoint = 1500, EndPoint = 9999 },
}

DUNGEON = 1 -- ����
PVP = 2 -- ����
GUILDBATTLE = 3 -- �����
CONNECT = 4 -- ����
ATTEND = 5 -- �⼮

GuildPointReward =
{
	{ Type = DUNGEON,  Win = 5, Lose = 1 },
	{ Type = PVP , Win = 3, Lose = 1 },
	{ Type = GUILDBATTLE, Win = 5, Lose = 1 },
	{ Type = CONNECT, Win = 5, Lose = 5 },
	{ Type = ATTEND, Win = 50, Lose = 50 },
}


-- �������Ʈ ���� ��ġ
GuildUpdateCount = 50 -- �������Ʈ ���� ��� ���� ��� �������� Max ����
ChangeGuildTickGap = 60 * 1000 -- ���� ���� �ð��뿡 �󸶸��� �ѹ��� ����� ��� ����Ʈ ��� ���� �������� TickGap ����
