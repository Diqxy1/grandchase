-- 던전 매니저
TRUE = 1
FALSE = 0

--[[ 게임모드,ID,던전 이름
    GC_GM_QUEST0            = 7,   // 시련의 숲
    GC_GM_QUEST1            = 8,   // 시련의 탑
    GC_GM_QUEST2            = 9,   // 세르딘성 외곽
    GC_GM_QUEST3            = 10,  // 캐리비치
    GC_GM_QUEST4            = 11,  // 오크사원
    GC_GM_QUEST5            = 12,  // 고르고스 던전
    GC_GM_QUEST6            = 13,  // 엘프의 숲
    GC_GM_QUEST7            = 14,  // 맹세의 계곡
    GC_GM_QUEST8            = 15,  // 망각의 늪
    GC_GM_QUEST9            = 16,  // 망자의 묘지
    GC_GM_QUEST10           = 17,  // 잊혀진 도시
    GC_GM_QUEST11           = 18,  // 가이코즈의 성
    GC_GM_QUEST12           = 19,  // 파투세이의 바다
    GC_GM_QUEST13           = 20,  // 엘리아 대륙
    GC_GM_QUEST14           = 21,  // 불의 사원
    GC_GM_QUEST15           = 22,  // 헬 브릿지
    GC_GM_QUEST16           = 23,  // 카제아제의 성
    GC_GM_QUEST17           = 24,  // 카스툴 유적지 하층
    GC_GM_QUEST18           = 25,  // 카스툴 유적지 상층
    GC_GM_QUEST19           = 26,  // 베르메시아의 최후
    GC_GM_QUEST20           = 27,  // 제니아 접경지
    GC_GM_MONSTER_CRUSADER  = 28,  // 몬스터 원정대
    GC_GM_MONSTER_HUNT      = 29,  // 몬스터 대전(시즌1.미사용)
    GC_GM_QUEST21           = 30,  // 신신신 퀘스트 
    GC_GM_DEATH_TEAM        = 31,  // 데스매치 팀
    GC_GM_DEATH_SURVIVAL    = 32,  // 데스매치 서바이벌
    GC_GM_MINIGAME_TREEDROP    = 33,   // 미니게임 통나무 떨어지기
    GC_GM_MINIGAME_BALLOON    = 34,   // 미니게임 풍선 터뜨리기
    GC_GM_MINIGAME_BUTTERFRY    = 35,   // 미니게임 나비 잡기
    GC_GM_QUEST22           = 36,  // 소실의 협곡.... (가제)
    GC_GM_ANGELS_EGG        = 37,   //천사의 알
    GC_GM_CAPTAIN           = 38,  // 대장전
    GC_GM_QUEST23           = 39,  // 실버랜드 성난 엔트의 숲
    GC_GM_QUEST24           = 40,  // 실버랜드 실버나이츠
    GC_GM_QUEST25           = 41,   // 실버랜드 미명의 호수
    GC_GM_QUEST26           = 42,   // 제니아 이그니스의 산맥
    GC_GM_QUEST27           = 43,   // 싫버랜드 수룡의 늪
	GC_GM_QUEST28           = 44,   // 실버랜드 원시의 섬
	GC_GM_QUEST29           = 45,   // 실버랜드 드레이크 웜의 영지
	GC_GM_QUEST30           = 46,   // 실버랜드 빅터의 요새
	GC_GM_QUEST31           = 47,   // 제니아 빙하 조율의 제단
	GC_GM_QUEST32           = 48,   // 제니아 돌산
	GC_GM_QUEST33           = 49,   // 제니아 숲지역
	GC_GM_QUEST34           = 50,   // 제니아 지배의 성
	GC_GM_QUEST35           = 51,   // 베르메시아 보스 던전
	GC_GM_QUEST36           = 52,   // 실버랜드 보스 던전
	GC_GM_QUEST37           = 53,   // 엘리아 보스 던전
	GC_GM_QUEST38           = 54,   // 제니아 보스 던전
    GC_GM_QUEST39           = 55,   // 아툼 고블린 노역소
    GC_GM_QUEST40           = 56,   // 아툼 지하암흑 터널
    GC_GM_QUEST41           = 57,   // 아툼 쿵지쿵지
    GC_GM_QUEST42           = 58,   // 아케메디아 드워프 전초기지
    GC_GM_QUEST43           = 59,   // 아툼 신기루 사막
	GC_GM_QUEST44           = 60,   // 아케메디아 드워프 보급로
    GC_GM_QUEST45           = 61,   // 아케메디아 썬더 해머
    GC_GM_QUEST46           = 62,   // 영웅던전 ( 파멸의 성소 )
    GC_GM_QUEST47           = 63,   // 영웅던전 ( 지옥의 용광로 )
    GC_GM_QUEST48           = 64,   // 고대 왕국의 잔해
    GC_GM_DOTA              = 65,   // 점령전
    GC_GM_AGIT				= 66,   // 아지트
    GC_GM_QUEST49       	= 67,   // 이벤트 던전( 카니발 )
    GC_GM_QUEST50       	= 68,   // 이벤트 릴레이 던전
    GC_GM_QUEST51       	= 69,   // 이벤트 영웅 던전
    GC_GM_QUEST52           = 70,   // 릴레이 영웅 던전( 슈퍼 몬스터 ) 1단계 
    GC_GM_QUEST53       	= 71,	 // 명계열차 301 (GhostTrain)
    GC_GM_QUEST54       	= 72,	 // 배고픈 펫들의 반란
    GC_GM_QUEST55           = 73,   // 카루엘 접경지(KaruelBorder)
    GC_GM_QUEST56           = 74,   // 카루엘(Karuel)
    GC_GM_QUEST57           = 75,   // 이벤트던전(야시장)
    GC_GM_QUEST58           = 76,   // 레이드던전
--]]

SELECT_REWARD = 1 -- 선택 보상 Type
RESULT_REWARD = 2 -- 결과 보상 Type
NONE_REWARD   = 3 -- 보상 없음 Type

-- 던전 최대 인원 세팅.
DungeonMaxUserInfo =
{
	-- ModeID,MaxUser
	{ ModeID = 76, MaxUser = 6,},
	{ ModeID = 83, MaxUser = 2,},
	{ ModeID = 84, MaxUser = 1,},
	{ ModeID = 85, MaxUser = 1,},
	{ ModeID = 86, MaxUser = 1,},
	{ ModeID = 87, MaxUser = 1,},
	{ ModeID = 88, MaxUser = 1,},
	{ ModeID = 89, MaxUser = 1,},
	
	{ ModeID = 90, MaxUser = 1,},
	{ ModeID = 91, MaxUser = 1,},
	{ ModeID = 92, MaxUser = 1,},
	{ ModeID = 93, MaxUser = 1,},
	{ ModeID = 94, MaxUser = 1,},
	{ ModeID = 95, MaxUser = 1,},
	
}

-- 던전 모드 변경 안되는 던전정보.
-- 기본 Default는 TRUE
DungeonModeChangeDisableInfo = { 76, 83, }

-- 던전 보상 TYPE 세팅.
DungeonRewardTypeInfo = 
{
	{ 7, SELECT_REWARD },
	{ 8, SELECT_REWARD },
	{ 9, SELECT_REWARD },
	{ 10, SELECT_REWARD },
	{ 11, SELECT_REWARD },
	{ 12, SELECT_REWARD },
	{ 13, SELECT_REWARD },
	{ 14, SELECT_REWARD },
	{ 15, SELECT_REWARD },
	{ 16, SELECT_REWARD },
	{ 17, SELECT_REWARD },
	{ 18, SELECT_REWARD },
	{ 19, SELECT_REWARD },
	{ 20, SELECT_REWARD },
	{ 21, SELECT_REWARD },
	{ 22, SELECT_REWARD },
	{ 23, SELECT_REWARD },
	{ 24, SELECT_REWARD },
	{ 25, SELECT_REWARD },
	{ 26, SELECT_REWARD },
	{ 27, SELECT_REWARD },
	{ 28, SELECT_REWARD },
	{ 29, SELECT_REWARD },
	{ 30, SELECT_REWARD },
	{ 31, SELECT_REWARD },
	{ 32, SELECT_REWARD },
	{ 33, SELECT_REWARD },
	{ 34, SELECT_REWARD },
	{ 35, SELECT_REWARD },
	{ 36, SELECT_REWARD },
	{ 37, SELECT_REWARD },
	{ 38, SELECT_REWARD },
	{ 39, SELECT_REWARD },
	{ 40, SELECT_REWARD },
	{ 41, SELECT_REWARD },
	{ 42, SELECT_REWARD },
	{ 43, SELECT_REWARD },
	{ 44, SELECT_REWARD },
	{ 45, SELECT_REWARD },
	{ 46, SELECT_REWARD },
	{ 47, SELECT_REWARD },
	{ 48, SELECT_REWARD },
	{ 49, SELECT_REWARD },
	{ 50, SELECT_REWARD },
	{ 51, SELECT_REWARD },
	{ 52, SELECT_REWARD },
	{ 53, SELECT_REWARD },
	{ 54, SELECT_REWARD },
	{ 55, SELECT_REWARD },
	{ 56, SELECT_REWARD },
	{ 57, SELECT_REWARD },
	{ 58, SELECT_REWARD },
	{ 59, SELECT_REWARD },
	{ 60, SELECT_REWARD },
	{ 61, SELECT_REWARD },
	{ 62, RESULT_REWARD },
	{ 63, RESULT_REWARD },
	{ 64, SELECT_REWARD },
	{ 65, SELECT_REWARD },
	{ 66, SELECT_REWARD },
	{ 67, SELECT_REWARD },
	{ 68, SELECT_REWARD },
	{ 69, SELECT_REWARD },
	{ 70, RESULT_REWARD },
	{ 71, SELECT_REWARD },
	{ 72, SELECT_REWARD },
	{ 73, SELECT_REWARD },
	{ 74, SELECT_REWARD },
	{ 75, SELECT_REWARD },
	{ 76, RESULT_REWARD },
	{ 78, SELECT_REWARD },
	{ 79, SELECT_REWARD },
	{ 80, SELECT_REWARD },
	{ 81, RESULT_REWARD },
	{ 82, RESULT_REWARD },
	{ 83, SELECT_REWARD },
	{ 84, SELECT_REWARD },
	{ 85, SELECT_REWARD },
	{ 86, SELECT_REWARD },
	{ 87, SELECT_REWARD },
	{ 88, SELECT_REWARD },
	{ 89, RESULT_REWARD },
	{ 90, SELECT_REWARD },
	{ 91, SELECT_REWARD },
	{ 92, SELECT_REWARD },
	{ 93, SELECT_REWARD },
	{ 94, SELECT_REWARD },
	{ 95, SELECT_REWARD },
	{ 98, NONE_REWARD },
	{ 99, NONE_REWARD },
	{ 100, NONE_REWARD },
	{ 101, NONE_REWARD },
}
