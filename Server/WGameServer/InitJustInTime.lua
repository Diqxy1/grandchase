ALL_USER = 0 -- ��� ����
PCCAFE_USER = 1 -- PC�� ����
EX_PCCAFE_USER = 2 -- PC�� ���� ����


TickGap = 1000 * 60  -- ms

EventList =
{
    --[[ ��ũ��Ʈ ���� ����
    {
        UserType = ALL_USER, -- �̺�Ʈ ��� ����
        Event = {
            {
                Time = { YYYY, MM, DD, HH, MM, }, -- �⵵, ��, ��, �ð�, ��
                RewardList = { GoodsID, Count, Period, }, -- GoodsID, ����, �Ⱓ
            },
        },
    },
    --]]

    -- ��� ����
    {
        UserType = ALL_USER,
        Event = {
            {
                Time = { 2012, 01, 20, 14, 00, },
                RewardList = { 799310, 1, -1, 801870, 2, -1, 799360, 3, -1},
            },
            {
                Time = { 2012, 01, 20, 14, 10, },
                RewardList = { 799310, 1, -1, 801870, 2, -1, 799360, 3, -1},
            },
        },
    },

    -- PC�� ����
    {
        UserType = PCCAFE_USER,
        Event = {
            {
                Time = { 2012, 01, 20, 14, 00, },
                RewardList = { 799310, 1, -1, 801870, 2, -1, 799360, 3, -1},
            },
            {
                Time = { 2012, 01, 20, 14, 10, },
                RewardList = { 799310, 1, -1, 801870, 2, -1, 799360, 3, -1},
            },
        },
    },

    -- PC�� ���� ����
    {
        UserType = EX_PCCAFE_USER,
        Event = {
            {
                Time = { 2012, 01, 20, 14, 00, },
                RewardList = { 799310, 1, -1, 801870, 2, -1, 799360, 3, -1},
            },
            {
                Time = { 2012, 01, 20, 14, 10, },
                RewardList = { 799310, 1, -1, 801870, 2, -1, 799360, 3, -1},
            },
        },
    },
}
