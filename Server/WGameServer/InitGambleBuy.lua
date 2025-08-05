TRUE = 1
FALSE = 0

GambleBuy =
{
    CostRate = 2.0, -- 구매비용

    -- 겜블 확률
    -- = MAX( MinGambleRatio, MaxGambleRatio - int(ITEM_LV / LevelModNum) )
    MinGambleRatio = 10,
    MaxGambleRatio = 20,
    LevelModNum = 8,

    -- 아이템 등급별 출현 확률(1.0 = 100%)
    RareRatio = 0.98,
    EpicRatio = 0.02,
    LegendRatio = 0,
}