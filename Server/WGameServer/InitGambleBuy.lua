TRUE = 1
FALSE = 0

GambleBuy =
{
    CostRate = 2.0, -- ���ź��

    -- �׺� Ȯ��
    -- = MAX( MinGambleRatio, MaxGambleRatio - int(ITEM_LV / LevelModNum) )
    MinGambleRatio = 10,
    MaxGambleRatio = 20,
    LevelModNum = 8,

    -- ������ ��޺� ���� Ȯ��(1.0 = 100%)
    RareRatio = 0.98,
    EpicRatio = 0.02,
    LegendRatio = 0,
}