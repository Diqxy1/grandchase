BeginTime       = { 2010, 2, 2, 09 }   -- Y, M, D, H
EndTime         = { 2010, 2, 16, 09 }   -- Y, M, D, H
TickGap         = 1000 * 60 -- ms
DropInterval    = 60 -- 분
DailyMaxDrop    = 5

-- Receiver Type
ALL     = 0 -- 전체
PC_BANG = 1 -- PC방 유저
NORMAL  = 2 -- 일반 유저

DropItem = {
    -- DropInterval 단위로 최대 DailyMaxDrop 만큼 드랍한다.
    -- Count = n회차, Item = { 받을 아이템 리스트{대상유저,ItemID,수량,기간}, }
    {   Count = 1,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,},   },
    {   Count = 2,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,}, {NORMAL,283450,3,-1,},  },
    {   Count = 3,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,},   },
    {   Count = 4,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,}, {NORMAL,283450,3,-1,},  },
    {   Count = 5,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,},   },

}
