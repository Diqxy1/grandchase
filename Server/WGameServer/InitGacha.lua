-- 2012-12-14 10:35:20
TRUE = 1
FALSE = 0
EquipmentType = 0 -- 장비 Type
AccessoriesType = 1 -- 악세사리 Type
EquipmentRewardType = 0 -- 장비 보상
TotalRewardType = 1 -- 전체 보상

UseVersions =
{
    -- 버전, 스트링 번호
    900,    1051720,    -- 오르도 코디
    24,     890700,    -- 천랑 코디
    700,    373960,     -- 셀리온 코디
}

DefaultRealPointInfo =
{
    -- 기본 릴 포인트 등급과 divider를 설정함(설정이 누락되는 경우를 대비)
    6, 2.7
}

InitGacha =
{
    {
        GachaVersion = 900,
        GachaMaxReelPoint = 945,
        GachaDecreaseEnable = FALSE, -- 최종보상 획득시 다음 가챠 아이템 봉인해제서 갯수 감소시킬지의 여부 결정.
        DecreaseCount = 1, -- 감소시킬 봉인 해제서 갯수

        ReelPointEnable = TRUE,
        EquipmentEnable = TRUE,
        AccessoriesEnable = TRUE,

        RealPointInfo =
        {
            { 565660, 6, 2.7 },
            { 565670, 6, 2.7 },
            { 565680, 6, 2.7 },
            { 565690, 6, 2.7 },
            { 565700, 6, 2.7 },
            { 565710, 6, 2.7 },
            { 565720, 6, 2.7 },
            { 565730, 6, 2.7 },
            { 565740, 6, 2.7 },
            { 565750, 6, 2.7 },
            { 565760, 6, 2.7 },
            { 565770, 6, 2.7 },
            { 565780, 6, 2.7 },
            { 779990, 6, 2.7 },
            { 803840, 6, 2.7 },
            { 904860, 6, 2.7 },
            { 966870, 6, 2.7 },
        },
        CharacterItemInfo =
        {
            {
                GachaItem = 565660,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1052940, -1, -1 }, -- 오르도 소대 코디 검
                            { 1052950, -1, -1 }, -- 오르도 소대 코디 창
                            { 1052960, -1, -1 }, -- 오르도 소대 코디 대검
                            { 1052970, -1, -1 }, -- 오르도 소대 코디 듀얼소드
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1051760, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1051770, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1051780, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1051790, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1051800, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1051810, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053410, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053420, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053430, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053440, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054090, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565670,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1052980, -1, -1 }, -- 오르도 소대 코디 활
                            { 1052990, -1, -1 }, -- 오르도 소대 코디 석궁
                            { 1053000, -1, -1 }, -- 오르도 소대 코디 대궁
                            { 1053010, -1, -1 }, -- 오르도 소대 코디 컴포짓보우
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1051830, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1051840, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1051850, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1051860, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1051870, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1051880, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053450, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053460, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053470, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053480, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054100, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565680,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053020, -1, -1 }, -- 오르도 소대 코디 지팡이
                            { 1053030, -1, -1 }, -- 오르도 소대 코디 항아리
                            { 1053040, -1, -1 }, -- 오르도 소대 코디 램프
                            { 1053050, -1, -1 }, -- 오르도 소대 코디 롱 스테프
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1051900, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1051910, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1051920, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1051930, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1051940, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1051950, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053490, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053500, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053510, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053520, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054110, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565690,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053060, -1, -1 }, -- 오르도 소대 코디 단검
                            { 1053070, -1, -1 }, -- 오르도 소대 코디 시미터
                            { 1053080, -1, -1 }, -- 오르도 소대 코디 클로
                            { 1053090, -1, -1 }, -- 오르도 소대 코디 패도
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1051970, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1051980, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1051990, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052000, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052010, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052020, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053530, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053540, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053550, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053560, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054120, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565700,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053100, -1, -1 }, -- 오르도 소대 코디 도끼
                            { 1053110, -1, -1 }, -- 오르도 소대 코디 크리센트
                            { 1053120, -1, -1 }, -- 오르도 소대 코디 사이저
                            { 1053130, -1, -1 }, -- 오르도 소대 코디 파이크
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052040, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052050, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052060, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052070, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052080, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052090, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053570, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053580, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053590, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053600, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054130, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565710,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053140, -1, -1 }, -- 오르도 소대 코디 마검
                            { 1053150, -1, -1 }, -- 오르도 소대 코디 그레이브
                            { 1053160, -1, -1 }, -- 오르도 소대 코디 소드쉴드
                            { 1053170, -1, -1 }, -- 오르도 소대 코디 티르빙
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052110, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052120, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052130, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052140, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052150, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052160, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053610, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053620, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053630, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053640, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054140, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565720,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053180, -1, -1 }, -- 오르도 소대 코디 차크람
                            { 1053190, -1, -1 }, -- 오르도 소대 코디 바이엘
                            { 1053200, -1, -1 }, -- 오르도 소대 코디 클레오
                            { 1053210, -1, -1 }, -- 오르도 소대 코디 판도라
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052180, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052190, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052200, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052210, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052220, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052230, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053650, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053660, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053670, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053680, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054150, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565730,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053220, -1, -1 }, -- 오르도 소대 코디 건틀릿
                            { 1053230, -1, -1 }, -- 오르도 소대 코디 톤파
                            { 1053240, -1, -1 }, -- 오르도 소대 코디 참마곤
                            { 1053250, -1, -1 }, -- 오르도 소대 코디 금강저
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052250, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052260, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052270, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052280, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052290, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052300, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053690, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053700, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053710, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053720, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054160, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565740,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053260, -1, -1 }, -- 오르도 소대 코디 블레이드
                            { 1053270, -1, -1 }, -- 오르도 소대 코디 창검
                            { 1053280, -1, -1 }, -- 오르도 소대 코디 에페
                            { 1053290, -1, -1 }, -- 오르도 소대 코디 솔루나
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052320, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052330, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052340, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052350, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052360, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052370, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053730, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053740, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053750, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053760, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054170, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565750,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053300, -1, -1 }, -- 오르도 소대 코디 매뉴얼북
                            { 1053310, -1, -1 }, -- 오르도 소대 코디 버스터건
                            { 1053320, -1, -1 }, -- 오르도 소대 코디 매직맬릿
                            { 1053330, -1, -1 }, -- 오르도 소대 코디 소울테이커
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052390, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052400, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052410, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052420, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052430, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052440, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053770, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053780, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053790, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053800, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054180, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565760,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053340, -1, -1 }, -- 오르도 소대 코디 데스스타
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052460, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052470, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052480, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052490, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052500, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052510, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053810, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053820, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053830, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053840, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054190, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565770,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053350, -1, -1 }, -- 오르도 소대 코디 그란다르크
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052530, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052540, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052550, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052560, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052570, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052580, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053850, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053860, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053870, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053880, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054200, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565780,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053360, -1, -1 }, -- 오르도 소대 코디 엔들레스
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052600, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052610, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052620, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052630, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052640, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052650, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053890, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053900, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053910, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053920, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054210, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 779990,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053370, -1, -1 }, -- 오르도 소대 코디 아이투스
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052670, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052680, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052690, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052700, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052710, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052720, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053930, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053940, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053950, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1053960, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054220, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 803840,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053380, -1, -1 }, -- 오르도 소대 코디 부채
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052740, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052750, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052760, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052770, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052780, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052790, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1053970, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1053980, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1053990, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1054000, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054230, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 904860,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053390, -1, -1 }, -- 오르도 소대 코디 암가드
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052810, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052820, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052830, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052840, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052850, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052860, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1054010, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1054020, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1054030, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1054040, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054240, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 966870,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1053400, -1, -1 }, -- 오르도 소대 코디 블레싱 해머
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 1051720, -1, -1 }, -- 오르도 소대 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1052880, -1, -1, 0 }, -- 오르도 소대 코디 투구
                            { 1052890, -1, -1, 0 }, -- 오르도 소대 코디 상의
                            { 1052900, -1, -1, 0 }, -- 오르도 소대 코디 하의
                            { 1052910, -1, -1, 0 }, -- 오르도 소대 코디 장갑
                            { 1052920, -1, -1, 0 }, -- 오르도 소대 코디 신발
                            { 1052930, -1, -1, 0 }, -- 오르도 소대 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1054050, -1, -1, 0 }, -- 오르도 소대 코디 써클릿
                            { 1054060, -1, -1, 0 }, -- 오르도 소대 코디 날개
                            { 1054070, -1, -1, 0 }, -- 오르도 소대 코디 슈즈나이프
                            { 1054080, -1, -1, 0 }, -- 오르도 소대 코디 방패
                            { 1054250, -1, -1, 0 }, -- 오르도 소대 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
        }, -- end of CharacterItemInfo
    },  -- end of version 900
    {
        GachaVersion = 24,
        GachaMaxReelPoint = 945,
        GachaDecreaseEnable = FALSE, -- 최종보상 획득시 다음 가챠 아이템 봉인해제서 갯수 감소시킬지의 여부 결정.
        DecreaseCount = 1, -- 감소시킬 봉인 해제서 갯수

        ReelPointEnable = TRUE,
        EquipmentEnable = TRUE,
        AccessoriesEnable = TRUE,

        RealPointInfo =
        {
            { 565660, 6, 2.7 },
            { 565670, 6, 2.7 },
            { 565680, 6, 2.7 },
            { 565690, 6, 2.7 },
            { 565700, 6, 2.7 },
            { 565710, 6, 2.7 },
            { 565720, 6, 2.7 },
            { 565730, 6, 2.7 },
            { 565740, 6, 2.7 },
            { 565750, 6, 2.7 },
            { 565760, 6, 2.7 },
            { 565770, 6, 2.7 },
            { 565780, 6, 2.7 },
            { 779990, 6, 2.7 },
            { 803840, 6, 2.7 },
            { 904860, 6, 2.7 },
            { 966870, 6, 2.7 },
        },
        CharacterItemInfo =
        {
            {
                GachaItem = 565660,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889500, -1, -1 }, -- 천랑 코디 검
                            { 889510, -1, -1 }, -- 천랑 코디 창
                            { 889520, -1, -1 }, -- 천랑 코디 대검
                            { 889530, -1, -1 }, -- 천랑 코디 듀얼소드
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888390, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888400, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888410, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888420, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888430, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888440, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 889950, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 889960, -1, -1, 0 }, -- 천랑 코디 날개
                            { 889970, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 889980, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890550, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565670,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889540, -1, -1 }, -- 천랑 코디 활
                            { 889550, -1, -1 }, -- 천랑 코디 석궁
                            { 889560, -1, -1 }, -- 천랑 코디 대궁
                            { 889570, -1, -1 }, -- 천랑 코디 컴포짓보우
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888460, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888470, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888480, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888490, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888500, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888510, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 889990, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890000, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890010, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890020, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890560, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565680,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889580, -1, -1 }, -- 천랑 코디 지팡이
                            { 889590, -1, -1 }, -- 천랑 코디 항아리
                            { 889600, -1, -1 }, -- 천랑 코디 램프
                            { 889610, -1, -1 }, -- 천랑 코디 롱 스테프
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888530, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888540, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888550, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888560, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888570, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888580, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890030, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890040, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890050, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890060, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890570, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565690,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889620, -1, -1 }, -- 천랑 코디 단검
                            { 889630, -1, -1 }, -- 천랑 코디 시미터
                            { 889640, -1, -1 }, -- 천랑 코디 클로
                            { 889650, -1, -1 }, -- 천랑 코디 패도
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888600, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888610, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888620, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888630, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888640, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888650, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890070, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890080, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890090, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890100, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890580, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565700,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889660, -1, -1 }, -- 천랑 코디 도끼
                            { 889670, -1, -1 }, -- 천랑 코디 크리센트
                            { 889680, -1, -1 }, -- 천랑 코디 사이저
                            { 889690, -1, -1 }, -- 천랑 코디 파이크
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888670, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888680, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888690, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888700, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888710, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888720, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890110, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890120, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890130, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890140, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890590, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565710,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889700, -1, -1 }, -- 천랑 코디 마검
                            { 889710, -1, -1 }, -- 천랑 코디 그레이브
                            { 889720, -1, -1 }, -- 천랑 코디 소드쉴드
                            { 889730, -1, -1 }, -- 천랑 코디 티르빙
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888740, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888750, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888760, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888770, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888780, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888790, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890150, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890160, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890170, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890180, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890600, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565720,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889740, -1, -1 }, -- 천랑 코디 차크람
                            { 889750, -1, -1 }, -- 천랑 코디 바이엘
                            { 889760, -1, -1 }, -- 천랑 코디 클레오
                            { 889770, -1, -1 }, -- 천랑 코디 판도라
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888810, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888820, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888830, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888840, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888850, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888860, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890190, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890200, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890210, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890220, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890610, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565730,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889780, -1, -1 }, -- 천랑 코디 건틀릿
                            { 889790, -1, -1 }, -- 천랑 코디 톤파
                            { 889800, -1, -1 }, -- 천랑 코디 참마곤
                            { 889810, -1, -1 }, -- 천랑 코디 금강저
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888880, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888890, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888900, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888910, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888920, -1, -1, 0 }, -- 천랑 코디 신발
                            { 888930, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890230, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890240, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890250, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890260, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890620, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565740,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889820, -1, -1 }, -- 천랑 코디 블레이드
                            { 889830, -1, -1 }, -- 천랑 코디 창검
                            { 889840, -1, -1 }, -- 천랑 코디 에페
                            { 889850, -1, -1 }, -- 천랑 코디 솔루나
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 888950, -1, -1, 0 }, -- 천랑 코디 투구
                            { 888960, -1, -1, 0 }, -- 천랑 코디 상의
                            { 888970, -1, -1, 0 }, -- 천랑 코디 하의
                            { 888980, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 888990, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889000, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890270, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890280, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890290, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890300, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890630, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565750,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889860, -1, -1 }, -- 천랑 코디 매뉴얼북
                            { 889870, -1, -1 }, -- 천랑 코디 버스터건
                            { 889880, -1, -1 }, -- 천랑 코디 매직맬릿
                            { 889890, -1, -1 }, -- 천랑 코디 소울테이커
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889020, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889030, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889040, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889050, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889060, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889070, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890310, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890320, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890330, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890340, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890640, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565760,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889900, -1, -1 }, -- 천랑 코디 데스스타
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889090, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889100, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889110, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889120, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889130, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889140, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890350, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890360, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890370, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890380, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890650, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565770,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889910, -1, -1 }, -- 천랑 코디 그란다르크
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889160, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889170, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889180, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889190, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889200, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889210, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890390, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890400, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890410, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890420, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890660, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565780,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889920, -1, -1 }, -- 천랑 코디 엔들레스
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889230, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889240, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889250, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889260, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889270, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889280, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890430, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890440, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890450, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890460, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890670, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 779990,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889930, -1, -1 }, -- 천랑 코디 아이투스
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889300, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889310, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889320, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889330, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889340, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889350, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890470, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890480, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890490, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890500, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890680, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 803840,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 889940, -1, -1 }, -- 천랑 코디 린 무기
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889370, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889380, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889390, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889400, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889410, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889420, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 890510, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 890520, -1, -1, 0 }, -- 천랑 코디 날개
                            { 890530, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 890540, -1, -1, 0 }, -- 천랑 코디 방패
                            { 890690, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 904860,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 904560, -1, -1 }, -- 천랑 아신 암가드
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 889440, -1, -1, 0 }, -- 천랑 코디 투구
                            { 889450, -1, -1, 0 }, -- 천랑 코디 상의
                            { 889460, -1, -1, 0 }, -- 천랑 코디 하의
                            { 889470, -1, -1, 0 }, -- 천랑 코디 장갑
                            { 889480, -1, -1, 0 }, -- 천랑 코디 신발
                            { 889490, -1, -1, 0 }, -- 천랑 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 904510, -1, -1, 0 }, -- 천랑 코디 써클릿
                            { 904520, -1, -1, 0 }, -- 천랑 코디 날개
                            { 904530, -1, -1, 0 }, -- 천랑 코디 슈즈나이프
                            { 904540, -1, -1, 0 }, -- 천랑 코디 방패
                            { 904550, -1, -1, 0 }, -- 천랑 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 966870,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1004870, -1, -1 }, -- 천랑 블레싱 해머
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 890700, -1, -1 }, -- 천랑 펫 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1004810, -1, -1, 0 }, -- 천랑 투구
                            { 1004820, -1, -1, 0 }, -- 천랑 상의
                            { 1004830, -1, -1, 0 }, -- 천랑 하의
                            { 1004840, -1, -1, 0 }, -- 천랑 장갑
                            { 1004850, -1, -1, 0 }, -- 천랑 신발
                            { 1004860, -1, -1, 0 }, -- 천랑 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1004880, -1, -1, 0 }, -- 천랑 써클릿
                            { 1004890, -1, -1, 0 }, -- 천랑 날개
                            { 1004900, -1, -1, 0 }, -- 천랑 슈즈나이프
                            { 1004910, -1, -1, 0 }, -- 천랑 방패
                            { 1004920, -1, -1, 0 }, -- 천랑 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
        }, -- end of CharacterItemInfo
    },  -- end of version 24
    {
        GachaVersion = 700,
        GachaMaxReelPoint = 945,
        GachaDecreaseEnable = FALSE, -- 최종보상 획득시 다음 가챠 아이템 봉인해제서 갯수 감소시킬지의 여부 결정.
        DecreaseCount = 1, -- 감소시킬 봉인 해제서 갯수

        ReelPointEnable = TRUE,
        EquipmentEnable = TRUE,
        AccessoriesEnable = TRUE,

        RealPointInfo =
        {
            { 565660, 6, 2.7 },
            { 565670, 6, 2.7 },
            { 565680, 6, 2.7 },
            { 565690, 6, 2.7 },
            { 565700, 6, 2.7 },
            { 565710, 6, 2.7 },
            { 565720, 6, 2.7 },
            { 565730, 6, 2.7 },
            { 565740, 6, 2.7 },
            { 565750, 6, 2.7 },
            { 565760, 6, 2.7 },
            { 565770, 6, 2.7 },
            { 565780, 6, 2.7 },
            { 779990, 6, 2.7 },
            { 803840, 6, 2.7 },
            { 904860, 6, 2.7 },
            { 966870, 6, 2.7 },
        },
        CharacterItemInfo =
        {
            {
                GachaItem = 565660,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037560, -1, -1 }, -- 다크 볼케이노 코디 검
                            { 1037570, -1, -1 }, -- 다크 볼케이노 코디 창
                            { 1037580, -1, -1 }, -- 다크 볼케이노 코디 대검
                            { 1037590, -1, -1 }, -- 다크 볼케이노 코디 듀얼소드
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036380, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036390, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036400, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036410, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036420, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036430, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038030, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038040, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038050, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038060, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038710, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565670,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037600, -1, -1 }, -- 다크 볼케이노 코디 활
                            { 1037610, -1, -1 }, -- 다크 볼케이노 코디 석궁
                            { 1037620, -1, -1 }, -- 다크 볼케이노 코디 대궁
                            { 1037630, -1, -1 }, -- 다크 볼케이노 코디 컴포짓보우
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036450, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036460, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036470, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036480, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036490, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036500, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038070, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038080, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038090, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038100, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038720, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565680,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037640, -1, -1 }, -- 다크 볼케이노 코디 지팡이
                            { 1037650, -1, -1 }, -- 다크 볼케이노 코디 항아리
                            { 1037660, -1, -1 }, -- 다크 볼케이노 코디 램프
                            { 1037670, -1, -1 }, -- 다크 볼케이노 코디 롱 스테프
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036520, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036530, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036540, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036550, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036560, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036570, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038110, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038120, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038130, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038140, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038730, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565690,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037680, -1, -1 }, -- 다크 볼케이노 코디 단검
                            { 1037690, -1, -1 }, -- 다크 볼케이노 코디 시미터
                            { 1037700, -1, -1 }, -- 다크 볼케이노 코디 클로
                            { 1037710, -1, -1 }, -- 다크 볼케이노 코디 패도
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036590, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036600, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036610, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036620, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036630, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036640, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038150, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038160, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038170, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038180, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038740, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565700,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037720, -1, -1 }, -- 다크 볼케이노 코디 도끼
                            { 1037730, -1, -1 }, -- 다크 볼케이노 코디 크리센트
                            { 1037740, -1, -1 }, -- 다크 볼케이노 코디 사이저
                            { 1037750, -1, -1 }, -- 다크 볼케이노 코디 파이크
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036660, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036670, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036680, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036690, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036700, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036710, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038190, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038200, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038210, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038220, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038750, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565710,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037760, -1, -1 }, -- 다크 볼케이노 코디 마검
                            { 1037770, -1, -1 }, -- 다크 볼케이노 코디 그레이브
                            { 1037780, -1, -1 }, -- 다크 볼케이노 코디 소드쉴드
                            { 1037790, -1, -1 }, -- 다크 볼케이노 코디 티르빙
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036730, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036740, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036750, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036760, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036770, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036780, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038230, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038240, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038250, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038260, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038760, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565720,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037800, -1, -1 }, -- 다크 볼케이노 코디 차크람
                            { 1037810, -1, -1 }, -- 다크 볼케이노 코디 바이엘
                            { 1037820, -1, -1 }, -- 다크 볼케이노 코디 클레오
                            { 1037830, -1, -1 }, -- 다크 볼케이노 코디 판도라
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036800, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036810, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036820, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036830, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036840, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036850, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038270, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038280, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038290, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038300, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038770, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565730,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037840, -1, -1 }, -- 다크 볼케이노 코디 건틀릿
                            { 1037850, -1, -1 }, -- 다크 볼케이노 코디 톤파
                            { 1037860, -1, -1 }, -- 다크 볼케이노 코디 참마곤
                            { 1037870, -1, -1 }, -- 다크 볼케이노 코디 금강저
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036870, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036880, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036890, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036900, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036910, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036920, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038310, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038320, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038330, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038340, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038780, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565740,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037880, -1, -1 }, -- 다크 볼케이노 코디 블레이드
                            { 1037890, -1, -1 }, -- 다크 볼케이노 코디 창검
                            { 1037900, -1, -1 }, -- 다크 볼케이노 코디 에페
                            { 1037910, -1, -1 }, -- 다크 볼케이노 코디 솔루나
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1036940, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1036950, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1036960, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1036970, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1036980, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1036990, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038350, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038360, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038370, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038380, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038790, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565750,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037920, -1, -1 }, -- 다크 볼케이노 코디 매뉴얼북
                            { 1037930, -1, -1 }, -- 다크 볼케이노 코디 버스터건
                            { 1037940, -1, -1 }, -- 다크 볼케이노 코디 매직맬릿
                            { 1037950, -1, -1 }, -- 다크 볼케이노 코디 소울테이커
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037010, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037020, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037030, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037040, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037050, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037060, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038390, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038400, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038410, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038420, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038800, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565760,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037960, -1, -1 }, -- 다크 볼케이노 코디 데스스타
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037080, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037090, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037100, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037110, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037120, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037130, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038430, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038440, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038450, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038460, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038810, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565770,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037970, -1, -1 }, -- 다크 볼케이노 코디 그란다르크
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037150, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037160, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037170, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037180, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037190, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037200, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038470, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038480, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038490, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038500, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038820, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 565780,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037980, -1, -1 }, -- 다크 볼케이노 코디 엔들레스
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037220, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037230, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037240, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037250, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037260, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037270, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038510, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038520, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038530, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038540, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038830, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 779990,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1037990, -1, -1 }, -- 다크 볼케이노 코디 아이투스
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037290, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037300, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037310, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037320, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037330, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037340, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038550, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038560, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038570, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038580, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038840, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 803840,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1038000, -1, -1 }, -- 다크 볼케이노 코디 부채
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037360, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037370, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037380, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037390, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037400, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037410, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038590, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038600, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038610, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038620, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038850, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 904860,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1038010, -1, -1 }, -- 다크 볼케이노 코디 암가드
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037430, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037440, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037450, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037460, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037470, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037480, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038630, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038640, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038650, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038660, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038860, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
            {
                GachaItem = 966870,
                KeyItemID = 102030,
                NeedKeyItemCount = 
                {
                    { 0, 3 }, -- { Lv, NeedKeyCount }
                },
                GachaSetRewardList =
                {
                    {
                        SetRewardType = EquipmentRewardType,
                        RewardList =
                        {
                            { 1038020, -1, -1 }, -- 다크 볼케이노 코디 블레싱 해머
                        },
                    },
                    {
                        SetRewardType = TotalRewardType,
                        RewardList =
                        {
                            { 373960, -1, -1 }, -- 셀리온 봉인카드
                        },
                    },
                },
                CollectItemList = 
                {
                    {
                        CollectItemType = EquipmentType,
                        CollectItemList =
                        {
                            { 1037500, -1, -1, 0 }, -- 다크 볼케이노 코디 투구
                            { 1037510, -1, -1, 0 }, -- 다크 볼케이노 코디 상의
                            { 1037520, -1, -1, 0 }, -- 다크 볼케이노 코디 하의
                            { 1037530, -1, -1, 0 }, -- 다크 볼케이노 코디 장갑
                            { 1037540, -1, -1, 0 }, -- 다크 볼케이노 코디 신발
                            { 1037550, -1, -1, 0 }, -- 다크 볼케이노 코디 망토
                        },
                    },
                    {
                        CollectItemType = AccessoriesType,
                        CollectItemList =
                        {
                            { 1038670, -1, -1, 0 }, -- 다크 볼케이노 코디 써클릿
                            { 1038680, -1, -1, 0 }, -- 다크 볼케이노 코디 날개
                            { 1038690, -1, -1, 0 }, -- 다크 볼케이노 코디 슈즈나이프
                            { 1038700, -1, -1, 0 }, -- 다크 볼케이노 코디 방패
                            { 1038870, -1, -1, 0 }, -- 다크 볼케이노 코디 마스크
                        }
                    },
                },
                FailItemList =
                {
                    {
                        ItemLevel = 0,
                        LevelRatioReward = {
                            { 42340, -1, 3, 37.5, }, -- GC 클럽
                            { 627300, 1, -1, 25.25, }, -- 특수 강화 성공률 상승 주문서
                            { 287060, 9, -1, 27.25, }, -- 소켓 쥬얼리
                            { 506040, -1, 2, 10, }, -- 쑥쑥 천사의 반지
                        },
                    },
                },
            },
        }, -- end of CharacterItemInfo
    },  -- end of version 700
}
