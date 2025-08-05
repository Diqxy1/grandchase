TRUE = 1
FALSE = 0

GambleBuy =
{
    CostRate = 1.0, -- 구매비용

    -- 겜블 확률
    -- = MAX( MinGambleRatio, MaxGambleRatio - int(ITEM_LV / LevelModNum) )
    MinGambleRatio = 5,
    MaxGambleRatio = 15,
    LevelModNum = 5,

    -- 아이템 등급별 출현 확률(1.0 = 100%)
    RareRatio = 0.9,
    EpicRatio = 0.1,
    LegendRatio = 0,
}

