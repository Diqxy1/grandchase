--[[
ĳ���� ���� ���� ������ ���� ��ũ��Ʈ�Դϴ�
CharType: ������ ĳ���� ��ȣ�� �����մϴ�
Conditions: �ش� ĳ���Ͱ� �����ϱ� ���� Ŭ�����ؾ� �� ���� �� ���̵��� �����մϴ�
Rewards: ������ ������ �����۵��� �����մϴ�
ClearModes: ������ �ڵ����� Ŭ���� ó��(���̵� 0��) ���� ������ �����մϴ�
������: ������, �̼���, ���翵
--]]

CHAR_ELESIS     = 0
CHAR_LIRE       = 1
CHAR_ARME       = 2
CHAR_LAS        = 3
CHAR_RYAN       = 4
CHAR_RONAN      = 5
CHAR_AMY        = 6
CHAR_JIN        = 7
CHAR_SIEG       = 8
CHAR_MARI       = 9
CHAR_DIO        = 10
CHAR_ZERO       = 11
CHAR_LEY        = 12
CHAR_LUPUS      = 13
CHAR_RIN        = 14
CHAR_ASIN       = 15
CHAR_LIME       = 16
-------------------------- �׽�Ʈ ������.. ȥ������ ����!!!!!!!!!!!!!!
Graduation =
{
    {
        CharType = CHAR_RIN,
        Condition =
        {
            { ModeID = GC_GM_QUEST42, Difficulty = { 0, 1, 2, }, },
            --[[
            { ModeID = GC_GM_QUEST44, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST45, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST48, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST55, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST56, Difficulty = { 0, 1, 2, }, },
            { ModeID = GC_GM_QUEST62, Difficulty = { 0, 1, 2, }, },
            --]]
        },
        ClearModes =
        {
            GC_GM_QUEST0, GC_GM_QUEST1, GC_GM_QUEST2, GC_GM_QUEST3, GC_GM_QUEST4, GC_GM_QUEST5,
            GC_GM_QUEST6, GC_GM_QUEST7, GC_GM_QUEST8, GC_GM_QUEST9, --GC_GM_QUEST10, GC_GM_QUEST11,
        },
        Rewards =
        {-- { itemid, grade, period, count, }, ...
            { 833050, 3, -1, -1, },
        },
    },
}
