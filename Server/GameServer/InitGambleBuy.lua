TRUE = 1
FALSE = 0

GambleBuy =
{
    CostRate = 1.0, -- ���ź��

    -- �׺� Ȯ��
    -- = MAX( MinGambleRatio, MaxGambleRatio - int(ITEM_LV / LevelModNum) )
    MinGambleRatio = 5,
    MaxGambleRatio = 15,
    LevelModNum = 5,

    -- ������ ��޺� ���� Ȯ��(1.0 = 100%)
    RareRatio = 0.9,
    EpicRatio = 0.1,
    LegendRatio = 0,
}

