-- AutoLevel이 TRUE인 경우, 몬스터가 등장 던전의 최소 적정레벨로 설정됩니다.
-- AutoLevel이 FALSE인 경우, MonsterLevel변수에 설정된 값으로 추가됩니다.

--[[ 게임모드, ID, 던전 이름
GC_GM_QUEST0   = 7,     // 시련의 숲
GC_GM_QUEST1   = 8,     // 시련의 탑
GC_GM_QUEST2   = 9,     // 세르딘성 외곽
GC_GM_QUEST3   = 10,    // 캐리비치
GC_GM_QUEST4   = 11,    // 오크사원
GC_GM_QUEST5   = 12,    // 고르고스 던전
GC_GM_QUEST6   = 13,    // 엘프의 숲
GC_GM_QUEST7   = 14,    // 맹세의 계곡
GC_GM_QUEST8   = 15,    // 망각의 늪
GC_GM_QUEST9   = 16,    // 망자의 묘지
GC_GM_QUEST10  = 17,    // 잊혀진 도시
GC_GM_QUEST11  = 18,    // 가이코즈의 성

GC_GM_QUEST12  = 19,    // 파투세이의 바다
GC_GM_QUEST13  = 20,    // 카미키 성채
GC_GM_QUEST14  = 21,    // 불의 사원
GC_GM_QUEST15  = 22,    // 헬 브릿지
GC_GM_QUEST16  = 23,    // 카제아제의 성
GC_GM_QUEST17  = 24,    // 카스툴 유적지 하층
GC_GM_QUEST18  = 25,    // 카스툴 유적지 상층
GC_GM_QUEST19  = 26,    // 베르메시아의 최후

GC_GM_QUEST20  = 27,    // 제니아 접경지
GC_GM_QUEST21  = 30,    // 순환의 신전
GC_GM_QUEST22  = 36,    // 소실의 협곡
GC_GM_QUEST26  = 42,    // 이그니스 산맥
GC_GM_QUEST31  = 47,    // 조율의 제단
GC_GM_QUEST32  = 48,    // 파괴의 신전
GC_GM_QUEST33  = 49,    // 생명의 숲
GC_GM_QUEST34  = 50,    // 지배의 성

GC_GM_QUEST23  = 39,    // 성난 엔트의 숲
GC_GM_QUEST24  = 40,    // 실버나이츠
GC_GM_QUEST25  = 41,    // 미명의 호수
GC_GM_QUEST27  = 43,    // 수룡의 늪
GC_GM_QUEST28  = 44,    // 원시의 섬
GC_GM_QUEST29  = 45,    // 드레이크 웜의 영지
GC_GM_QUEST30  = 46,    // 빅터의 요새

GC_GM_QUEST39  = 55,    // 고블린 노역소 (GoblinLaborCamp)
GC_GM_QUEST40  = 56,    // 지하암흑 터널 (UndergroundTunnel)
GC_GM_QUEST41  = 57,    // 쿵지 쿵지 (KungjiVillage)
GC_GM_QUEST43  = 59,    // 신기루 사막 (MirageDesert)

GC_GM_QUEST42  = 58,    // 드워프 전초기지 (DwarvenBase)
GC_GM_QUEST44  = 60,    // 드워프 보급로 (DwarfSupplyRoute)
GC_GM_QUEST45  = 61,    // 썬더 해머 (ThunderHammer)
GC_GM_QUEST49  = 67,    // 악몽의 써커스
GC_GM_QUEST50  = 68,    // 이벤트 릴레이던전

--]]

TRUE = 1
FALSE = 0

-- 특수능력 속성
CP_ATTACK           = 1

CP_DEFENCE          = 2
CP_HP_RECOVERY      = 4

CP_SUPER_ARMOR      = 8
CP_FAST_STAND       = 16
-- 추가된 챔피언 속성
CP_ARROW_DEFENSE    = 256  -- 화살 방어
CP_SHOCKWAVE        = 512  -- 충격파

CP_ATTACK_CURES     = 32
CP_ATTACK_STONE     = 64
CP_ATTACK_ICE       = 128

-- GameMode, Difficult값이 Key값이다.
SpecialMonster =
{
	ActiveEvent = TRUE,                    -- TRUE, FALSE/ 전체 이벤트 ON/OFF
    {
		GameMode = 9,
		Difficult = 255, -- 난이도
        MonsterID = 278,
		MonsterLevel = 50,
		PropertyInfo = { -- 몬스터 속성 설정.
			bEnable = FALSE,
			PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 갯수 확률로 설정.
			Property = {
				{ {CP_ATTACK, 100} },
				{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
				{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			}
		},
    },
}