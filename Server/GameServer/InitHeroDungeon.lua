--[[ 게임모드, ID, 던전 이름
GC_GM_QUEST0   = 7,    // 시련의 숲
GC_GM_QUEST1   = 8,    // 시련의 탑
GC_GM_QUEST2   = 9,    // 세르딘성 외곽
GC_GM_QUEST3   = 10,   // 캐리비치
GC_GM_QUEST4   = 11,   // 오크사원
GC_GM_QUEST5   = 12,   // 고르고스 던전
GC_GM_QUEST6   = 13,   // 엘프의 숲
GC_GM_QUEST7   = 14,   // 맹세의 계곡
GC_GM_QUEST8   = 15,   // 망각의 늪
GC_GM_QUEST9   = 16,   // 망자의 묘지
GC_GM_QUEST10  = 17,   // 잊혀진 도시
GC_GM_QUEST11  = 18,   // 가이코즈의 성
GC_GM_QUEST12  = 19,   // 파투세이의 바다
GC_GM_QUEST13  = 20,   // 엘리아 대륙
GC_GM_QUEST14  = 21,   // 불의 사원
GC_GM_QUEST15  = 22,   // 헬 브릿지
GC_GM_QUEST16  = 23,   // 카제아제의 성
GC_GM_QUEST17  = 24,   // 카스툴 유적지 하층
GC_GM_QUEST18  = 25,   // 카스툴 유적지 상층
GC_GM_QUEST19  = 26,   // 베르메시아의 최후
GC_GM_QUEST20  = 27,   // 제니아 접경지
GC_GM_QUEST21  = 30,   // 신신신 퀘스트 ㅋ
GC_GM_QUEST22  = 36,   // 소실의 협곡.... (가제)
GC_GM_QUEST23  = 39,   // 성난 엔트의 숲
GC_GM_QUEST24  = 40,   // 실버나이츠
GC_GM_QUEST25  = 41,    // 미명의 호수
GC_GM_QUEST26  = 42,    // 이그니스의 산맥
GC_GM_QUEST27  = 43,    // 요정 습지
GC_GM_QUEST28  = 44,    // 실버랜드 원시의 섬
GC_GM_QUEST29  = 45,    // 실버랜드 용암지대
GC_GM_QUEST30  = 46,    // 실버랜드 빅터의 요새
GC_GM_QUEST31  = 47,    // 제니아 빙하 조율의 제단
GC_GM_QUEST32  = 48,    // 제니아 돌산
GC_GM_QUEST33  = 49,    // 제니아 숲지역
GC_GM_QUEST34  = 50,    // 제니아 지배의 성
GC_GM_QUEST35  = 51,    // 베르메시아 보스 던전
GC_GM_QUEST36  = 52,    // 실버랜드 보스 던전
GC_GM_QUEST37  = 53,    // 엘리아 보스 던전
GC_GM_QUEST38  = 54,    // 제니아 보스 던전
GC_GM_QUEST39  = 55,    // GoblinLaborCamp
GC_GM_QUEST40  = 56,    // UndergroundTunnel
GC_GM_QUEST41  = 57,    // KungjiVillage
GC_GM_QUEST42  = 58,    // DwarvenBase
GC_GM_QUEST43  = 59,    // 신기루 사막
GC_GM_QUEST44  = 60,    // 드워프 보급로
GC_GM_QUEST45  = 61,    // 썬더 해머
GC_GM_QUEST46  = 62,    // 영웅던전(베르메시아)
GC_GM_QUEST47  = 63,    // 영웅던전
GC_GM_QUEST48  = 64,    // 고대 왕국의 잔해
GC_GM_QUEST49  = 67,    // Event Carnival
GC_GM_QUEST50  = 68,    // Event RelayDungeon
GC_GM_QUEST51  = 69,    // Event Hero
GC_GM_QUEST52  = 70,    // 릴레이 영웅던전
GC_GM_QUEST53  = 71,    // Monster Train
--]]

TRUE = 1
FALSE = 0

OpenAlarm = { 5, 3, 1, 0 }
CloseAlarm = { 5, 3, 1 }

MonsterExpRatio = 40      -- 몬스터 경험치 나누기 배율
DungeonWinClearExpRatio = 40  -- 던전 클리어 성공 경험치 연산배율
DungeonLoseClearExpRatio = 10  -- 던전 클리어 성공 경험치 연산배율

HeroDungeon = {
    ActiveMode = TRUE,          -- 전체 동작여부

    HeroPointItem = { 12490, 1, -1 },   -- 영웅포인트 아이템 {ItemID,Count,Period}
    ResetTime = { 00, 00 },     -- 입장초기화 시간 (00시 00분)

	
	{
        ModeID = 62,           -- 게임모드ID
        HeroPoint = 1,         -- 영웅포인트 아이템 보상개수(Count)
        MinLv = 40,            -- 최소 입장가능 캐릭터레벨
        MaxClearCount = 2,     -- 일일 최대클리어 가능수
		MonsterLv = 0,         -- 1회이상 클리어후, 적용되는 몬스터 레벨 (0이면 설정안함)
		ResetDifficulty = FALSE,-- 최대난이도 클리어시, 난이도 초기화 되는지 여부
		DifficultyCheck = FALSE,-- 난이도 조건 체크하는지 여부

		RewardCount = 0,       -- 클리어시 선택 획득가능한 보상 개수
		ClearReward = {
			-- { ItemID = 776750, Count = -1, Period = 7, IsLook = TRUE, },
		},

        -- 오픈시간대
        OpenTime = {
			-- 반드시 00시부터 시간 순서대로 설정할 것
			{ 00,00, 00,20 },
            { 01,00, 01,20 },
            { 02,00, 02,20 },
            { 03,00, 03,20 },
            { 04,00, 04,20 },
            { 05,00, 05,20 },
            { 06,00, 06,20 },
            { 07,00, 07,20 },
            { 08,00, 08,20 },
            { 09,00, 09,20 },
            { 10,00, 10,20 },
            { 11,00, 11,20 },
            { 12,00, 12,20 },
            { 13,00, 13,20 },
            { 14,00, 14,20 },
            { 15,00, 15,20 },
            { 16,00, 16,20 },
            { 17,00, 17,20 },
            { 18,00, 18,20 },
            { 19,00, 19,20 },
            { 20,00, 20,20 },
            { 21,00, 21,20 },
            { 22,00, 22,20 },
            { 23,00, 23,20 },
        },
    },
}
