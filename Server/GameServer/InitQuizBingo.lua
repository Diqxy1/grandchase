TickInterval    = 1000 * 60             -- 정보 갱신 체크
BoardSize       = 10

BingoBoardItemID    = 206760        -- 빙고 판 아이템 아이디.
ResurrectionItem    = 206780        -- 부활 아이템 ID ( 수량 )
BingoCoinID         = 206770        -- 빙고 코인 ID ( 수량 )
DailyMaxCoin        = 5             -- 일일 지급 코인 최대 개수
CoinSupplyInterval  = 60            -- 코인 지급 간격 (분)
CoinSupplyCount     = 2             -- 코인 몇 개씩 지급 할지

BingoReward =
{
    { Count = 0, Reward = { 8880, 1, -1, 12490, 1, -1, }, }, -- 빙고 이지만 아래 리스트에 없는 경우
                                     
    { Count = 1, Reward = { 8880, 2, -1, 12490, 2, -1, }, },
    { Count = 2, Reward = { 8880, 3, -1, 12490, 3, -1, }, },
    { Count = 3, Reward = { 8880, 4, -1, 12490, 4, -1, }, },
    { Count = 4, Reward = { 8880, 5, -1, 12490, 5, -1, }, },
    { Count = 5, Reward = { 8880, 6, -1, 12490, 6, -1, }, },
    { Count = 6, Reward = { 8880, 7, -1, 12490, 7, -1, }, },
}

CorrectQuizReward = { 8880, 1, -1, 12490, 1, -1, }          -- 정답을 맞출때 마다 주는 보상.