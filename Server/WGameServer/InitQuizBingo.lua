TickInterval    = 1000 * 60             -- ���� ���� üũ
BoardSize       = 10

BingoBoardItemID    = 206760        -- ���� �� ������ ���̵�.
ResurrectionItem    = 206780        -- ��Ȱ ������ ID ( ���� )
BingoCoinID         = 206770        -- ���� ���� ID ( ���� )
DailyMaxCoin        = 5             -- ���� ���� ���� �ִ� ����
CoinSupplyInterval  = 60            -- ���� ���� ���� (��)
CoinSupplyCount     = 2             -- ���� �� ���� ���� ����

BingoReward =
{
    { Count = 0, Reward = { 8880, 1, -1, 12490, 1, -1, }, }, -- ���� ������ �Ʒ� ����Ʈ�� ���� ���
                                     
    { Count = 1, Reward = { 8880, 2, -1, 12490, 2, -1, }, },
    { Count = 2, Reward = { 8880, 3, -1, 12490, 3, -1, }, },
    { Count = 3, Reward = { 8880, 4, -1, 12490, 4, -1, }, },
    { Count = 4, Reward = { 8880, 5, -1, 12490, 5, -1, }, },
    { Count = 5, Reward = { 8880, 6, -1, 12490, 6, -1, }, },
    { Count = 6, Reward = { 8880, 7, -1, 12490, 7, -1, }, },
}

CorrectQuizReward = { 8880, 1, -1, 12490, 1, -1, }          -- ������ ���⶧ ���� �ִ� ����.