ALL_USER = 0 -- 모든 유저
PCCAFE_USER = 1 -- PC방 유저
EX_PCCAFE_USER = 2 -- PC방 유저 제외


TickGap = 1000 * 60  -- ms

EventList =
{
    --[[ 스크립트 설정 샘플
    {
        UserType = ALL_USER, -- 이벤트 대상 유저
        Event = {
            {
                Time = { YYYY, MM, DD, HH, MM, }, -- 년도, 월, 일, 시간, 분
                RewardList = { GoodsID, Count, Period, }, -- GoodsID, 수량, 기간
            },
        },
    },
    --]]

    -- 모든 유저
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

    -- PC방 유저
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

    -- PC방 유저 제외
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
