TRUE = 1
FALSE = 0

IT_PERIOD   = 0 -- �Ⱓ��/����
IT_QUANTITY = 1 -- ������ ������
MarkServerURL = "http://fileboard.netmarble.net:80/BBS/Chase/GuildMark/"
MarkImageExt = { "jpg", "jpeg", "png", }
MarkDefaultName = "defaultmark.jpg"
ImageBuffMinSize = 100
ImageBuffMaxSize = 1024 * 4
-- ��帶ũ ���ε� ������(�°ݽ� ���޼���)
GuildMarkUploadItemID = 206680
GuildMarkUploadItemFactor = 30
GuildMarkUploadItemType = IT_PERIOD
-- ���� ���� ������
GuildChangeNameItemID = 8880
GuildMarkUploadCashItemID = 0

GuildPageSize = 8 -- ���1�������� ����
GuildNameMinLength = 1 -- ����̸��ּұ���
GuildNameMaxLength = 6 -- ����̸��ִ����
GuildMasterMinLv = 30 -- ��帶���� �ּҷ��� ����
GuildURLMinLength = 1 -- URL �ּ� �Է±���
GuildURLMaxLength = 100 -- URL �ִ� �Է±���
GuildCreateGP = 10000 -- �������� �Ҹ�GP

OfficailGuildMemberNum = 5  -- ���� ��尡 �Ǳ����� �ɹ���
NewbieGuildRemoveTerm = 30  -- �Ż���� ������ ���ŵǱ� ���� ��¥��
KeywordMinLength = 2        -- �ּ� �˻��� ����
JoinerRemoveTerm = 15       -- ��尡���� ���� �����Ⱓ. ������ �ڵ� ������

-- http://www.unicode.org/charts/ �� ����Ʈ ���� �ϼ���.
AllowUnicode = TRUE
Unicode = {
    { tonumber( '0030', 16 ), tonumber( '0039', 16 ), }, -- ����
    { tonumber( '0041', 16 ), tonumber( '005A', 16 ), }, -- ���ĺ� �빮��
    { tonumber( '0061', 16 ), tonumber( '007A', 16 ), }, -- ������ �ҹ���
    { tonumber( '00C0', 16 ), tonumber( '00FF', 16 ), }, -- ��ƾ-1
-- �ѱ� Jamo
    { tonumber( 'FFA1', 16 ), tonumber( 'FFBE', 16 ), }, -- �ѱ�
    { tonumber( 'FFC2', 16 ), tonumber( 'FFC7', 16 ), }, -- �ѱ�
    { tonumber( 'FFCA', 16 ), tonumber( 'FFCF', 16 ), }, -- �ѱ�
    { tonumber( 'FFD2', 16 ), tonumber( 'FFD7', 16 ), }, -- �ѱ�
    { tonumber( 'FFDA', 16 ), tonumber( 'FFDC', 16 ), }, -- �ѱ�
    { tonumber( 'FFDA', 16 ), tonumber( 'FFDC', 16 ), }, -- �ѱ�
-- �ѱ� �ϼ���
    { tonumber( 'AC00', 16 ), tonumber( 'D7A3', 16 ), }, -- �ѱ�
}
GuildInfoClearTerm = 1200000 -- ������� ���� �ֱ� (20��)
MemberListClearTerm = 600000 -- �ɹ�����Ʈ ���� �ֱ� (10��)
MaxPageSize = 15 -- �ִ� ���Page ����


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
GUILD_TAB_1 = 1
GUILD_TAB_2 = 2
GUILD_TAB_3 = 3

-- ��巹��/��������/��������
GuildLvAdvantage = {
    { 0,    NONE,                    0 },  -- ����
    { 1,    EXP_BONUS,               5 },  -- EXP +5%
    { 2,    STORE_TAB,               GUILD_TAB_1 },  -- ȸ������(TabID:1) �����̿�
    { 3,    RUNE_ENCHANT_BONUS,      5 },  -- �鰭ȭ +5%
    { 4,    EXP_BONUS,               5 },  -- EXP +5% (����+10%)
    { 5,    ACCESSORY_ENCHANT_BONUS, 5 },  -- ����̰�ȭ +5%
    { 6,    STORE_TAB,               GUILD_TAB_2 },  -- ��������(TabID:2) �����̿�
    { 7,    MORE_EQUIP_POTION,       2 },  -- �������� +2��
    { 8,    RUNE_ENCHANT_BONUS,      5 },  -- �鰭ȭ +5% (����+10%)
    { 9,    ACCESSORY_ENCHANT_BONUS, 5 },  -- ����̰�ȭ +5% (����+10%)
    { 10,   EXP_BONUS,               5 },  -- EXP +5% (����+15%)
}

-- ���⿩��/����ȴ� ��巹�� ����
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

-- �������Ʈ ���� ��ġ
GuildUpdateCount = 50 -- �������Ʈ ���� ��� ���� ��� �������� Max ����
ChangeGuildTickGap = 60 * 1000 -- ���� ���� �ð��뿡 �󸶸��� �ѹ��� ����� ��� ����Ʈ ��� ���� �������� TickGap ����
