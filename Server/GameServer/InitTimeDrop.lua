BeginTime       = { 2010, 2, 2, 09 }   -- Y, M, D, H
EndTime         = { 2010, 2, 16, 09 }   -- Y, M, D, H
TickGap         = 1000 * 60 -- ms
DropInterval    = 60 -- ��
DailyMaxDrop    = 5

-- Receiver Type
ALL     = 0 -- ��ü
PC_BANG = 1 -- PC�� ����
NORMAL  = 2 -- �Ϲ� ����

DropItem = {
    -- DropInterval ������ �ִ� DailyMaxDrop ��ŭ ����Ѵ�.
    -- Count = nȸ��, Item = { ���� ������ ����Ʈ{�������,ItemID,����,�Ⱓ}, }
    {   Count = 1,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,},   },
    {   Count = 2,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,}, {NORMAL,283450,3,-1,},  },
    {   Count = 3,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,},   },
    {   Count = 4,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,}, {NORMAL,283450,3,-1,},  },
    {   Count = 5,  Item = { {ALL,283450,3,-1,}, {PC_BANG,283450,3,-1,},   },

}
