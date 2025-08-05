--[[
이벤트 기간 및 ON/OFF 설정은 DB에서 합니다
개발자: 유성철, 김영호, 박현민, 곽재영, 구동현
--]]

-- /////// ENUM 설정 ///////

-- EGCContinent
GC_CONTINENT_INVALID       = -1
GC_CONTINENT_BERMESIAH     = 0
GC_CONTINENT_SILVERLAND    = 1
GC_CONTINENT_ELIA          = 2
GC_CONTINENT_XENIA         = 3
GC_CONTINENT_ACHAEMEDIA    = 4
GC_CONTINENT_ATUM          = 5

-- EGCCharType
GC_CHAR_ALL              = -1   -- 공용템 보상용
GC_CHAR_ELESIS           = 0
GC_CHAR_LIRE             = 1
GC_CHAR_ARME             = 2
GC_CHAR_LAS              = 3
GC_CHAR_RYAN             = 4
GC_CHAR_RONAN            = 5
GC_CHAR_AMY              = 6
GC_CHAR_JIN              = 7
GC_CHAR_SIEG             = 8
GC_CHAR_MARI             = 9
GC_CHAR_DIO              = 10
GC_CHAR_ZERO             = 11
GC_CHAR_LEY              = 12
GC_CHAR_LUPUS            = 13
GC_CHAR_RIN              = 14
GC_CHAR_ASIN             = 15

-- EGCGameMode
GC_GM_QUEST0            = 7    --// 베르메시아 시련의 숲
GC_GM_QUEST1            = 8    --// 베르메시아 시련의 탑
GC_GM_QUEST2            = 9    --// 베르메시아 세르딘 외곽
GC_GM_QUEST3            = 10   --// 베르메시아 캐리비치
GC_GM_QUEST4            = 11   --// 베르메시아 오크 사원
GC_GM_QUEST5            = 12   --// 베르메시아 고르고스 던전
GC_GM_QUEST6            = 13   --// 베르메시아 엘프의 숲
GC_GM_QUEST7            = 14   --// 베르메시아 맹세의 계곡
GC_GM_QUEST8            = 15   --// 베르메시아 망각의 늪
GC_GM_QUEST9            = 16   --// 베르메시아 망자의 묘지
GC_GM_QUEST10           = 17   --// 베르메시아 잊혀진 도시
GC_GM_QUEST11           = 18   --// 베르메시아 가이코즈의 성
GC_GM_QUEST12           = 19   --// 엘리아 파투세이의 바다
GC_GM_QUEST13           = 20   --// 엘리아 카미키 성채
GC_GM_QUEST14           = 21   --// 엘리아 불의 사원
GC_GM_QUEST15           = 22   --// 엘리아 헬 브릿지
GC_GM_QUEST16           = 23   --// 엘리아 카제아제의 성
GC_GM_QUEST17           = 24   --// 엘리아 카스툴 유적지 하층
GC_GM_QUEST18           = 25   --// 엘리아 카스툴 유적지 상층
GC_GM_QUEST19           = 26   --// 엘리아 베르메시아의 최후
GC_GM_QUEST20           = 27   --// 제니아 제니아 접경지
GC_GM_QUEST21           = 30   --// 제니아 순환의 신전 (신신신 퀘스트 ㅋ)
GC_GM_QUEST22           = 36   --// 제니아 소실의 협곡
GC_GM_QUEST23           = 39    --// 실버 성난 엔트의 숲
GC_GM_QUEST24           = 40    --// 실버 실버나이츠 폐혀
GC_GM_QUEST25           = 41    --// 실버 미명의 호수
GC_GM_QUEST26           = 42    --// 제니아 이그니스의 산맥
GC_GM_QUEST27           = 43    --// 실버 수룡의 늪
GC_GM_QUEST28           = 44    --// 실버 원시의 섬
GC_GM_QUEST29           = 45    --// 실버 드레이크 웜의 영지
GC_GM_QUEST30           = 46    --// 실버 빅터의 요새
GC_GM_QUEST31           = 47    --// 제니아 조율의 제단
GC_GM_QUEST32           = 48    --// 제니아 파괴의 신전 (돌산)
GC_GM_QUEST33           = 49    --// 제니아 생명의 숲
GC_GM_QUEST34           = 50    --// 제니아 지배의 성
GC_GM_QUEST35           = 51    --// 베르메시아 보스 던전
GC_GM_QUEST36           = 52    --// 실버랜드 보스 던전
GC_GM_QUEST37           = 53    --// 엘리아 보스 던전
GC_GM_QUEST38           = 54    --// 제니아 보스 던전
GC_GM_QUEST39           = 55    --// 아툼 고블린 노역소
GC_GM_QUEST40           = 56    --// 아툼 지하암흑 터널
GC_GM_QUEST41           = 57    --// 아툼 쿵지쿵지
GC_GM_QUEST42           = 58    --// 아케메디아 드워프 전초기지
GC_GM_QUEST43           = 59    --// 아툼 신기루 사막
GC_GM_QUEST44           = 60    --// 아케메디아 드워프 보급로
GC_GM_QUEST45           = 61    --// 아케메디아 썬더 해머
GC_GM_QUEST46           = 62    --// 영웅던전 ( 파멸의 성소 )
GC_GM_QUEST47           = 63    --// 영웅던전 ( 지옥의 용광로 )
GC_GM_QUEST48           = 64    --// 아케메디아 고대 왕국의 흔적
GC_GM_QUEST49           = 67    --// Event Carnival
GC_GM_QUEST50           = 68    --// Event RelayDungeon
GC_GM_QUEST51           = 69    --// 이벤트 영웅던전 (서커스)
GC_GM_QUEST52           = 70    --// 릴레이 영웅던전
GC_GM_QUEST53           = 71    --// Monster Train

-- ///// Enum 끝 //////


TickGap = 1000 * 60     -- 이벤트 변동사항 확인할 주기(기본 1분 마다 확인)


AdventureEventInfo =
{
    -- /////////////////////////////////// 몬스터 정보 /////////////////////////////////
    MonsterInfo =
    {
        -- // 7번 던전에서(챔피언 모드만 적용한다) 1번 몹을 잡으면 30% 확률로 전리품 1개 획득 가능.. BiteMeString은 현재 사용하지 않는다
        {
            DungeonID = GC_GM_QUEST0,
            Monsters =
            {
                -- 이 던전의 어느 몬스터가, 어느 대륙의 어느 전리품을, 얼마의 확률로, 몇 개나 주는지 설정
                { MonsterID = 1, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 2, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 3, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
            },
        },
        {
            DungeonID = GC_GM_QUEST1,
            Monsters =
            {
                { MonsterID = 4, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 5, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
                { MonsterID = 6, DropItemInfo = { GC_CONTINENT_BERMESIAH, 0 }, DropChance = 1.0, DropCount = 1, BiteMeString = {}, },
            },
        },
    },
    
    -- ////////////////////////////////// 대륙 보상 정보 //////////////////////////////////
    ContinentRewardInfo =
    {
        {
            ContinentID = GC_CONTINENT_BERMESIAH,
            CollectItems =
            {   -- 전리품 종류는 최대 9개 까지만 가능합니다
                { CollectID = 0, DisplayItemID = 565660, MaxCount = 311 },
                { CollectID = 1, DisplayItemID = 565660, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 627290, MaxCount = 33 },
                { CollectID = 3, DisplayItemID = 8880, MaxCount = 33 },
                { CollectID = 4, DisplayItemID = 627300, MaxCount = 12 },
                { CollectID = 5, DisplayItemID = 287060, MaxCount = 77 },
                { CollectID = 6, DisplayItemID = 565670, MaxCount = 3 },
                { CollectID = 7, DisplayItemID = 102030, MaxCount = 22 },
                { CollectID = 8, DisplayItemID = 102030, MaxCount = 1 },
            },
            Rewards =
            {
                { 8880, -1, -1, 100, -1 },
                -- ItemID, Grade, Period, Duration, ItemLevel... 디봇 순서에 맞춤
                -- 현재 ItemLevel은 동작하지 않음.. -1로 넣어주세요
            },
        },
        {
            ContinentID = GC_CONTINENT_SILVERLAND,
            CollectItems =
            {   -- 전리품 종류는 최대 9개 까지만 가능합니다
                { CollectID = 0, DisplayItemID = 802340, MaxCount = 1 },
                { CollectID = 1, DisplayItemID = 791590, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 802340, MaxCount = 3 },
                { CollectID = 3, DisplayItemID = 791590, MaxCount = 5 },
                { CollectID = 4, DisplayItemID = 802340, MaxCount = 22 },
                { CollectID = 5, DisplayItemID = 802340, MaxCount = 2 },
                { CollectID = 6, DisplayItemID = 8880, MaxCount = 11 },
            },
            Rewards =
            {
                { 8880, -1, -1, 200, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_ELIA,
            CollectItems =
            {   -- 전리품 종류는 최대 9개 까지만 가능합니다
                { CollectID = 0, DisplayItemID = 565680, MaxCount = 1 },
                { CollectID = 1, DisplayItemID = 762910, MaxCount = 2 },
                { CollectID = 2, DisplayItemID = 646990, MaxCount = 3 },
                { CollectID = 3, DisplayItemID = 645690, MaxCount = 4 },
                { CollectID = 4, DisplayItemID = 506030, MaxCount = 5 },
                { CollectID = 5, DisplayItemID = 504550, MaxCount = 6 },
                { CollectID = 6, DisplayItemID = 503040, MaxCount = 7 },
                { CollectID = 7, DisplayItemID = 504070, MaxCount = 8 },
                { CollectID = 8, DisplayItemID = 506090, MaxCount = 9 },
            },
            Rewards =
            {
                { 8880, -1, -1, 300, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_XENIA,
            CollectItems =
            {   -- 전리품 종류는 최대 9개 까지만 가능합니다
                { CollectID = 0, DisplayItemID = 8880, MaxCount = 22 },
                { CollectID = 1, DisplayItemID = 8880, MaxCount = 7 },
                { CollectID = 2, DisplayItemID = 8880, MaxCount = 22 },
                { CollectID = 3, DisplayItemID = 8880, MaxCount = 9 },
            },
            Rewards =
            {
                { 8880, -1, -1, 400, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_ACHAEMEDIA,
            CollectItems =
            {   -- 전리품 종류는 최대 9개 까지만 가능합니다
                { CollectID = 0, DisplayItemID = 620500, MaxCount = 22 },
                { CollectID = 1, DisplayItemID = 620510, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 620520, MaxCount = 22 },
                { CollectID = 3, DisplayItemID = 620530, MaxCount = 22 },
                { CollectID = 4, DisplayItemID = 621140, MaxCount = 22 },
                { CollectID = 5, DisplayItemID = 604000, MaxCount = 22 },
                { CollectID = 6, DisplayItemID = 585030, MaxCount = 22 },
                { CollectID = 7, DisplayItemID = 603310, MaxCount = 22 },
                { CollectID = 8, DisplayItemID = 603320, MaxCount = 22 },
            },
            Rewards =
            {
                { 8880, -1, -1, 500, -1 },
            },
        },
        {
            ContinentID = GC_CONTINENT_ATUM,
            CollectItems =
            {   -- 전리품 종류는 최대 9개 까지만 가능합니다
                { CollectID = 0, DisplayItemID = 395110, MaxCount = 3 },
                { CollectID = 1, DisplayItemID = 395100, MaxCount = 22 },
                { CollectID = 2, DisplayItemID = 395090, MaxCount = 9 },
                { CollectID = 3, DisplayItemID = 395080, MaxCount = 2 },
                { CollectID = 4, DisplayItemID = 395070, MaxCount = 4 },
                { CollectID = 5, DisplayItemID = 395120, MaxCount = 22 },
                { CollectID = 6, DisplayItemID = 394300, MaxCount = 5 },
                { CollectID = 7, DisplayItemID = 394310, MaxCount = 8 },
                { CollectID = 8, DisplayItemID = 394320, MaxCount = 7 },
            },
            Rewards =
            {
                { 8880, -1, -1, 600, -1 },
            },
        },
    },
    
    -- ////////////////////////////////// 최종 보상 정보 //////////////////////////////////
    FinalRewardInfo =
    {
        -- 최종 보상은 한 가지 아이템만 가능하다
        102030, -1, -1, 1000, -1,
        -- ItemID, Grade, Period, Duration, ItemLevel... 디봇 순서에 맞춤
        -- 현재 ItemLevel은 동작하지 않음.. -1로 넣어주세요
    },
}
