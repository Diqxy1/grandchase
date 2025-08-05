-- 게임모드
GC_GM_INVALID           = -1

GC_GM_TUTORIAL          = 0
GC_GM_TEAM_MODE         = 1
GC_GM_SURVIVAL_MODE     = 2
GC_GM_TAG_TEAM          = 3
GC_GM_TAG_SURVIVAL      = 4
GC_GM_GUILD_BATTLE      = 5
GC_GM_INDIGO_TEAM       = 6
GC_GM_QUEST0            = 7     -- 시련의 숲
GC_GM_QUEST1            = 8     -- 시련의 탑
GC_GM_QUEST2            = 9     -- 세르딘외곽
GC_GM_QUEST3            = 10    -- 캐리비치
GC_GM_QUEST4            = 11    -- 오크사원
GC_GM_QUEST5            = 12    -- 고르고스의 던전
GC_GM_QUEST6            = 13    -- 엘프의 숲
GC_GM_QUEST7            = 14    -- 맹세의 계곡
GC_GM_QUEST8            = 15    -- 망각의 늪
GC_GM_QUEST9            = 16    -- 망자의 묘지
GC_GM_QUEST10           = 17    -- 잊혀진 도시
GC_GM_QUEST11           = 18    -- 가이코즈의 성
GC_GM_QUEST12           = 19    -- 파투세이의 바다
GC_GM_QUEST13           = 20    -- 엘리아대륙
GC_GM_QUEST14           = 21    -- 불의 사원
GC_GM_QUEST15           = 22    -- 헬브릿지
GC_GM_QUEST16           = 23    -- 카제아제의 성
GC_GM_QUEST17           = 24    -- 카스툴 유적지 하층
GC_GM_QUEST18           = 25    -- 카스툴 유적지 상층
GC_GM_QUEST19           = 26    -- 베르메시아의 최후
GC_GM_QUEST20           = 27    -- 제니아접경지
GC_GM_QUEST21           = 30    -- 순환의신전
GC_GM_QUEST22           = 36    -- 야광 바위 협곡
GC_GM_ANGELS_EGG        = 37    -- 천사 알
GC_GM_CAPTAIN           = 38    -- 대장전
GC_GM_QUEST23           = 39    -- 성난 엔트의 숲
GC_GM_QUEST24           = 40    -- 실버나이츠의 폐허
GC_GM_QUEST25           = 41    -- 미명의 호수
GC_GM_QUEST26           = 42    -- 이그니스의 산맥
GC_GM_QUEST27           = 43    -- 늪던전
GC_GM_QUEST28           = 44    -- 실버랜드 원시의 섬
GC_GM_QUEST29           = 45    -- 실버랜드 용암지대
GC_GM_QUEST30           = 46    -- 실버랜드 빅터의 요새
GC_GM_QUEST31           = 47    -- 제니아 빙하 조율의 제단
GC_GM_QUEST32           = 48    -- 제니아 돌산
GC_GM_QUEST33           = 49    -- 제니아 숲지역
GC_GM_QUEST34           = 50    -- 제니아 지배의 성
GC_GM_QUEST35           = 51    -- 베르메시아 보스 던전
GC_GM_QUEST36           = 52    -- 실버랜드 보스 던전
GC_GM_QUEST37           = 53    -- 엘리아 보스 던전
GC_GM_QUEST38           = 54    -- 제니아 보스 던전
GC_GM_QUEST39           = 55    -- 고블린 노역소 (GoblinLaborCamp)
GC_GM_QUEST40           = 56    -- 지하암흑 터널 (UndergroundTunnel)
GC_GM_QUEST41           = 57    -- 쿵지 쿵지 (KungjiVillage)
GC_GM_QUEST42           = 58    -- 드워프 전초기지 (DwarvenBase)
GC_GM_QUEST43           = 59    -- 신기루 사막 (MirageDesert)
GC_GM_QUEST44           = 60    -- 드워프 보급로 (DwarfSupplyRoute)
GC_GM_QUEST45           = 61    -- 썬더 해머 (ThunderHammer)



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


-- Mob Grade
NORMAL_MOB      = 0
BOSS            = 1
MIDDLE_BOSS     = 2


ChampionInfo =
{
--// --------------------------------------------------------------------------------
	{
        -- 릴레이 던전 (베르메시아)
        DungeonID = GC_GM_QUEST35,
        DungeonLv = 0,
        MobGrade  = 0,
        ChampionCount = { { 1,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
        },
    },

	{
        -- 릴레이 던전 (베르메시아)
        DungeonID = GC_GM_QUEST35,
        DungeonLv = 0,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (베르메시아)
        DungeonID = GC_GM_QUEST35,
        DungeonLv = 0,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (엘리아)
        DungeonID = GC_GM_QUEST37,
        DungeonLv = 0,
        MobGrade  = 0,
        ChampionCount = { { 1,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
        },
    },

	{
        -- 릴레이 던전 (엘리아)
        DungeonID = GC_GM_QUEST37,
        DungeonLv = 0,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (엘리아)
        DungeonID = GC_GM_QUEST37,
        DungeonLv = 0,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (제니아)
        DungeonID = GC_GM_QUEST38,
        DungeonLv = 0,
        MobGrade  = 0,
        ChampionCount = { { 1,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 30}, {CP_DEFENCE, 30}, {CP_ARROW_DEFENSE, 20}, {CP_ATTACK_CURES, 20} },
        },
    },

	{
        -- 릴레이 던전 (제니아)
        DungeonID = GC_GM_QUEST38,
        DungeonLv = 0,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 30}, {CP_DEFENCE, 30}, {CP_ARROW_DEFENSE, 20}, {CP_ATTACK_CURES, 20} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (제니아)
        DungeonID = GC_GM_QUEST38,
        DungeonLv = 0,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 30}, {CP_DEFENCE, 30}, {CP_ARROW_DEFENSE, 20}, {CP_ATTACK_CURES, 20} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (실버랜드)
        DungeonID = GC_GM_QUEST36,
        DungeonLv = 0,
        MobGrade  = 0,
        ChampionCount = { { 1,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
        },
    },

	{
        -- 릴레이 던전 (실버랜드)
        DungeonID = GC_GM_QUEST36,
        DungeonLv = 0,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

	{
        -- 릴레이 던전 (실버랜드)
        DungeonID = GC_GM_QUEST36,
        DungeonLv = 0,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 40}, {CP_DEFENCE, 40}, {CP_ARROW_DEFENSE, 20} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 25}, {CP_SHOCKWAVE, 25}, {CP_ATTACK_CURES, 25} },
            --{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

--// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++			
--// --------------------------------------------------------------------------------			
	{		
		-- 세르딘 외곽 난이도 1	
		DungeonID = GC_GM_QUEST2,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_FAST_STAND, 100} },
		},	
	},		
	{		
		-- 세르딘 외곽 난이도 Hell	
		DungeonID = GC_GM_QUEST2,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 세르딘 외곽 난이도 Hell	
		DungeonID = GC_GM_QUEST2,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 세르딘 외곽 난이도 Hell	
		DungeonID = GC_GM_QUEST2,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 캐리 비치 난이도 1	
		DungeonID = GC_GM_QUEST3,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_FAST_STAND, 100} },
		},	
	},		
	{		
		-- 캐리 비치 난이도 Hell	
		DungeonID = GC_GM_QUEST3,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 캐리 비치 난이도 Hell	
		DungeonID = GC_GM_QUEST3,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 캐리 비치 난이도 Hell	
		DungeonID = GC_GM_QUEST3,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 오크 사원 난이도 1	
		DungeonID = GC_GM_QUEST4,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 오크 사원 난이도 Hell	
		DungeonID = GC_GM_QUEST4,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 오크 사원 난이도 Hell	
		DungeonID = GC_GM_QUEST4,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 오크 사원 난이도 Hell	
		DungeonID = GC_GM_QUEST4,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 고르고스 던전 난이도 1	
		DungeonID = GC_GM_QUEST5,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 고르고스 던전 난이도 Hell	
		DungeonID = GC_GM_QUEST5,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 고르고스 던전 난이도 Hell	
		DungeonID = GC_GM_QUEST5,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 고르고스 던전 난이도 Hell	
		DungeonID = GC_GM_QUEST5,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
    
--// --------------------------------------------------------------------------------			
	{		
		-- 엘프의 숲 난이도 1	
		DungeonID = GC_GM_QUEST6,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 엘프의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST6,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 엘프의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST6,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 엘프의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST6,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
    
--// --------------------------------------------------------------------------------			
	{		
		-- 맹세의 계곡 난이도 1	
		DungeonID = GC_GM_QUEST7,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 맹세의 계곡 난이도 Hell	
		DungeonID = GC_GM_QUEST7,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 맹세의 계곡 난이도 Hell	
		DungeonID = GC_GM_QUEST7,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 맹세의 계곡 난이도 Hell	
		DungeonID = GC_GM_QUEST7,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
    
--// --------------------------------------------------------------------------------			
	{		
		-- 망각의 늪 난이도 1	
		DungeonID = GC_GM_QUEST8,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 망각의 늪 난이도 Hell	
		DungeonID = GC_GM_QUEST8,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 망각의 늪 난이도 Hell	
		DungeonID = GC_GM_QUEST8,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 망각의 늪 난이도 Hell	
		DungeonID = GC_GM_QUEST8,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
    
--// --------------------------------------------------------------------------------			
	{		
		-- 망자의 묘지 난이도 1	
		DungeonID = GC_GM_QUEST9,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 망자의 묘지 난이도 Hell	
		DungeonID = GC_GM_QUEST9,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 망자의 묘지 난이도 Hell	
		DungeonID = GC_GM_QUEST9,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 망자의 묘지 난이도 Hell	
		DungeonID = GC_GM_QUEST9,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 잊혀진 도시 난이도 1	
		DungeonID = GC_GM_QUEST10,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 잊혀진 도시 난이도 Hell	
		DungeonID = GC_GM_QUEST10,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 잊혀진 도시 난이도 Hell	
		DungeonID = GC_GM_QUEST10,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 잊혀진 도시 난이도 Hell	
		DungeonID = GC_GM_QUEST10,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 가이코즈의 성 난이도 1	
		DungeonID = GC_GM_QUEST11,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 가이코즈의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST11,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 가이코즈의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST11,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 가이코즈의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST11,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 성난엔트의 숲 난이도 1	
		DungeonID = GC_GM_QUEST23,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 성난엔트의 숲 난이도 2	
		DungeonID = GC_GM_QUEST23,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 성난엔트의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST23,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 성난엔트의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST23,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 성난엔트의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST23,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 실버나이츠 폐허 난이도 1	
		DungeonID = GC_GM_QUEST24,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 실버나이츠 폐허 난이도 2	
		DungeonID = GC_GM_QUEST24,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 실버나이츠 폐허 난이도 Hell	
		DungeonID = GC_GM_QUEST24,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 실버나이츠 폐허 난이도 Hell	
		DungeonID = GC_GM_QUEST24,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 실버나이츠 폐허 난이도 Hell	
		DungeonID = GC_GM_QUEST24,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 미명의 호수 난이도 1	
		DungeonID = GC_GM_QUEST25,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 미명의 호수 난이도 2	
		DungeonID = GC_GM_QUEST25,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 미명의 호수 난이도 Hell	
		DungeonID = GC_GM_QUEST25,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 미명의 호수 난이도 Hell	
		DungeonID = GC_GM_QUEST25,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 미명의 호수 난이도 Hell	
		DungeonID = GC_GM_QUEST25,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 수룡의 늪 난이도 1	
		DungeonID = GC_GM_QUEST27,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 수룡의 늪 난이도 2	
		DungeonID = GC_GM_QUEST27,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 수룡의 늪 난이도 Hell	
		DungeonID = GC_GM_QUEST27,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 수룡의 늪 난이도 Hell	
		DungeonID = GC_GM_QUEST27,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 수룡의 늪 난이도 Hell	
		DungeonID = GC_GM_QUEST27,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 원시의 섬 난이도 1	
		DungeonID = GC_GM_QUEST28,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 원시의 섬 난이도 2	
		DungeonID = GC_GM_QUEST28,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 원시의 섬 난이도 Hell	
		DungeonID = GC_GM_QUEST28,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 원시의 섬 난이도 Hell	
		DungeonID = GC_GM_QUEST28,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 원시의 섬 난이도 Hell	
		DungeonID = GC_GM_QUEST28,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 드레이크 웜의 영지 난이도 1	
		DungeonID = GC_GM_QUEST29,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 드레이크 웜의 영지 난이도 2	
		DungeonID = GC_GM_QUEST29,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 드레이크 웜의 영지 난이도 Hell	
		DungeonID = GC_GM_QUEST29,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 드레이크 웜의 영지 난이도 Hell	
		DungeonID = GC_GM_QUEST29,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 드레이크 웜의 영지 난이도 Hell	
		DungeonID = GC_GM_QUEST29,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 빅터의 요새 난이도 1	
		DungeonID = GC_GM_QUEST30,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 빅터의 요새 난이도 2	
		DungeonID = GC_GM_QUEST30,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 빅터의 요새 난이도 Hell	
		DungeonID = GC_GM_QUEST30,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 빅터의 요새 난이도 Hell	
		DungeonID = GC_GM_QUEST30,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		
	{		
		-- 빅터의 요새 난이도 Hell	
		DungeonID = GC_GM_QUEST30,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 100} },
			{ {CP_FAST_STAND, 100} },
			{ {CP_SUPER_ARMOR, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 파투세이의 바다 난이도 1	
		DungeonID = GC_GM_QUEST12,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 파투세이의 바다 난이도 2	
		DungeonID = GC_GM_QUEST12,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 파투세이의 바다 난이도 3	
		DungeonID = GC_GM_QUEST12,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 파투세이의 바다 난이도 Hell	
		DungeonID = GC_GM_QUEST12,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 파투세이의 바다 난이도 Hell	
		DungeonID = GC_GM_QUEST12,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 파투세이의 바다 난이도 Hell	
		DungeonID = GC_GM_QUEST12,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 카미키 성채 난이도 1	
		DungeonID = GC_GM_QUEST13,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카미키 성채 난이도 2	
		DungeonID = GC_GM_QUEST13,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카미키 성채 난이도 3	
		DungeonID = GC_GM_QUEST13,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카미키 성채 난이도 Hell	
		DungeonID = GC_GM_QUEST13,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카미키 성채 난이도 Hell	
		DungeonID = GC_GM_QUEST13,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 카미키 성채 난이도 Hell	
		DungeonID = GC_GM_QUEST13,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 불의 사원 난이도 1	
		DungeonID = GC_GM_QUEST14,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 불의 사원 난이도 2	
		DungeonID = GC_GM_QUEST14,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 불의 사원 난이도 3	
		DungeonID = GC_GM_QUEST14,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 불의 사원 난이도 Hell	
		DungeonID = GC_GM_QUEST14,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 불의 사원 난이도 Hell	
		DungeonID = GC_GM_QUEST14,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 불의 사원 난이도 Hell	
		DungeonID = GC_GM_QUEST14,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 헬 브릿지 난이도 1	
		DungeonID = GC_GM_QUEST15,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 헬 브릿지 난이도 2	
		DungeonID = GC_GM_QUEST15,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 헬 브릿지 난이도 3	
		DungeonID = GC_GM_QUEST15,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 헬 브릿지 난이도 Hell	
		DungeonID = GC_GM_QUEST15,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 헬 브릿지 난이도 Hell	
		DungeonID = GC_GM_QUEST15,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 헬 브릿지 난이도 Hell	
		DungeonID = GC_GM_QUEST15,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 카제아제의 성 난이도 1	
		DungeonID = GC_GM_QUEST16,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카제아제의 성 난이도 2	
		DungeonID = GC_GM_QUEST16,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카제아제의 성 난이도 3	
		DungeonID = GC_GM_QUEST16,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카제아제의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST16,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카제아제의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST16,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 카제아제의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST16,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 카스툴 유적지 하층 난이도 1	
		DungeonID = GC_GM_QUEST17,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 하층 난이도 2	
		DungeonID = GC_GM_QUEST17,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 하층 난이도 3	
		DungeonID = GC_GM_QUEST17,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 하층 난이도 Hell	
		DungeonID = GC_GM_QUEST17,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 하층 난이도 Hell	
		DungeonID = GC_GM_QUEST17,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 하층 난이도 Hell	
		DungeonID = GC_GM_QUEST17,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 카스툴 유적지 상층 난이도 1	
		DungeonID = GC_GM_QUEST18,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 상층 난이도 2	
		DungeonID = GC_GM_QUEST18,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 상층 난이도 3	
		DungeonID = GC_GM_QUEST18,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 상층 난이도 Hell	
		DungeonID = GC_GM_QUEST18,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 상층 난이도 Hell	
		DungeonID = GC_GM_QUEST18,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 카스툴 유적지 상층 난이도 Hell	
		DungeonID = GC_GM_QUEST18,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 베르메시아의 최후 난이도 1	
		DungeonID = GC_GM_QUEST19,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 베르메시아의 최후 난이도 2	
		DungeonID = GC_GM_QUEST19,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 베르메시아의 최후 난이도 3	
		DungeonID = GC_GM_QUEST19,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 베르메시아의 최후 난이도 Hell	
		DungeonID = GC_GM_QUEST19,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
		},	
	},		
	{		
		-- 베르메시아의 최후 난이도 Hell	
		DungeonID = GC_GM_QUEST19,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 베르메시아의 최후 난이도 Hell	
		DungeonID = GC_GM_QUEST19,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 제니아 접경지 난이도 1	
		DungeonID = GC_GM_QUEST20,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 제니아 접경지 난이도 2	
		DungeonID = GC_GM_QUEST20,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 제니아 접경지 난이도 3	
		DungeonID = GC_GM_QUEST20,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 제니아 접경지 난이도 Hell	
		DungeonID = GC_GM_QUEST20,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 제니아 접경지 난이도 Hell	
		DungeonID = GC_GM_QUEST20,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 제니아 접경지 난이도 Hell	
		DungeonID = GC_GM_QUEST20,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 순환의 신전 난이도 1	
		DungeonID = GC_GM_QUEST21,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 순환의 신전 난이도 2	
		DungeonID = GC_GM_QUEST21,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 순환의 신전 난이도 3	
		DungeonID = GC_GM_QUEST21,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 순환의 신전 난이도 Hell	
		DungeonID = GC_GM_QUEST21,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 순환의 신전 난이도 Hell	
		DungeonID = GC_GM_QUEST21,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 순환의 신전 난이도 Hell	
		DungeonID = GC_GM_QUEST21,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 소실의 협곡 난이도 1	
		DungeonID = GC_GM_QUEST22,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 소실의 협곡 난이도 2	
		DungeonID = GC_GM_QUEST22,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 소실의 협곡 난이도 3	
		DungeonID = GC_GM_QUEST22,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 소실의 협곡 난이도 Hell	
		DungeonID = GC_GM_QUEST22,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 소실의 협곡 난이도 Hell	
		DungeonID = GC_GM_QUEST22,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 소실의 협곡 난이도 Hell	
		DungeonID = GC_GM_QUEST22,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 이그니스의 산맥 난이도 1	
		DungeonID = GC_GM_QUEST26,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 이그니스의 산맥 난이도 2	
		DungeonID = GC_GM_QUEST26,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 이그니스의 산맥 난이도 3	
		DungeonID = GC_GM_QUEST26,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 이그니스의 산맥 난이도 Hell	
		DungeonID = GC_GM_QUEST26,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 이그니스의 산맥 난이도 Hell	
		DungeonID = GC_GM_QUEST26,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 이그니스의 산맥 난이도 Hell	
		DungeonID = GC_GM_QUEST26,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 조율의 제단 난이도 1	
		DungeonID = GC_GM_QUEST31,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 조율의 제단 난이도 2	
		DungeonID = GC_GM_QUEST31,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 조율의 제단 난이도 3	
		DungeonID = GC_GM_QUEST31,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 조율의 제단 난이도 Hell	
		DungeonID = GC_GM_QUEST31,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 조율의 제단 난이도 Hell	
		DungeonID = GC_GM_QUEST31,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 조율의 제단 난이도 Hell	
		DungeonID = GC_GM_QUEST31,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 파괴의 신전 난이도 1	
		DungeonID = GC_GM_QUEST32,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 파괴의 신전 난이도 2	
		DungeonID = GC_GM_QUEST32,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 파괴의 신전 난이도 3	
		DungeonID = GC_GM_QUEST32,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 파괴의 신전 난이도 Hell	
		DungeonID = GC_GM_QUEST32,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 파괴의 신전 난이도 Hell	
		DungeonID = GC_GM_QUEST32,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 파괴의 신전 난이도 Hell	
		DungeonID = GC_GM_QUEST32,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 생명의 숲 난이도 1	
		DungeonID = GC_GM_QUEST33,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 생명의 숲 난이도 2	
		DungeonID = GC_GM_QUEST33,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 생명의 숲 난이도 3	
		DungeonID = GC_GM_QUEST33,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 생명의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST33,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 생명의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST33,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 생명의 숲 난이도 Hell	
		DungeonID = GC_GM_QUEST33,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 지배의 성 난이도 1	
		DungeonID = GC_GM_QUEST34,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지배의 성 난이도 2	
		DungeonID = GC_GM_QUEST34,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지배의 성 난이도 3	
		DungeonID = GC_GM_QUEST34,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지배의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST34,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지배의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST34,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 지배의 성 난이도 Hell	
		DungeonID = GC_GM_QUEST34,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 드워프 전초기지 난이도 1	
		DungeonID = GC_GM_QUEST42,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 전초기지 난이도 2	
		DungeonID = GC_GM_QUEST42,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 전초기지 난이도 3	
		DungeonID = GC_GM_QUEST42,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 전초기지 난이도 Hell	
		DungeonID = GC_GM_QUEST42,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 전초기지 난이도 Hell	
		DungeonID = GC_GM_QUEST42,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 드워프 전초기지 난이도 Hell	
		DungeonID = GC_GM_QUEST42,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 드워프 보급로 난이도 1	
		DungeonID = GC_GM_QUEST44,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 보급로 난이도 2	
		DungeonID = GC_GM_QUEST44,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 보급로 난이도 3	
		DungeonID = GC_GM_QUEST44,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 보급로 난이도 Hell	
		DungeonID = GC_GM_QUEST44,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 드워프 보급로 난이도 Hell	
		DungeonID = GC_GM_QUEST44,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 드워프 보급로 난이도 Hell	
		DungeonID = GC_GM_QUEST44,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 썬더 해머 난이도 1	
		DungeonID = GC_GM_QUEST45,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 75}, {CP_ARROW_DEFENSE, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 썬더 해머 난이도 2	
		DungeonID = GC_GM_QUEST45,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 75}, {CP_ARROW_DEFENSE, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 썬더 해머 난이도 3	
		DungeonID = GC_GM_QUEST45,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 75}, {CP_ARROW_DEFENSE, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 썬더 해머 난이도 Hell	
		DungeonID = GC_GM_QUEST45,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 75}, {CP_ARROW_DEFENSE, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 썬더 해머 난이도 Hell	
		DungeonID = GC_GM_QUEST45,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 2, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
		},	
	},		
	{		
		-- 썬더 해머 난이도 Hell	
		DungeonID = GC_GM_QUEST45,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 고블린 노역소 난이도 1	
		DungeonID = GC_GM_QUEST39,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 25 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 고블린 노역소 난이도 2	
		DungeonID = GC_GM_QUEST39,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 고블린 노역소 난이도 3	
		DungeonID = GC_GM_QUEST39,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 고블린 노역소 난이도 Hell	
		DungeonID = GC_GM_QUEST39,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 고블린 노역소 난이도 Hell	
		DungeonID = GC_GM_QUEST39,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 고블린 노역소 난이도 Hell	
		DungeonID = GC_GM_QUEST39,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 지하암흑 터널 난이도 1	
		DungeonID = GC_GM_QUEST40,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 30 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지하암흑 터널 난이도 2	
		DungeonID = GC_GM_QUEST40,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지하암흑 터널 난이도 3	
		DungeonID = GC_GM_QUEST40,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지하암흑 터널 난이도 Hell	
		DungeonID = GC_GM_QUEST40,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 지하암흑 터널 난이도 Hell	
		DungeonID = GC_GM_QUEST40,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 지하암흑 터널 난이도 Hell	
		DungeonID = GC_GM_QUEST40,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 쿵지 쿵지 난이도 1	
		DungeonID = GC_GM_QUEST41,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 쿵지 쿵지 난이도 2	
		DungeonID = GC_GM_QUEST41,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 쿵지 쿵지 난이도 3	
		DungeonID = GC_GM_QUEST41,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 쿵지 쿵지 난이도 Hell	
		DungeonID = GC_GM_QUEST41,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 쿵지 쿵지 난이도 Hell	
		DungeonID = GC_GM_QUEST41,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 쿵지 쿵지 난이도 Hell	
		DungeonID = GC_GM_QUEST41,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// --------------------------------------------------------------------------------			
	{		
		-- 신기루 사막 난이도 1	
		DungeonID = GC_GM_QUEST43,	
		DungeonLv = 0,	
		MobGrade = 0,	
		ChampionCount = { { 1, 35 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 신기루 사막 난이도 2	
		DungeonID = GC_GM_QUEST43,	
		DungeonLv = 1,	
		MobGrade = 0,	
		ChampionCount = { { 2, 40 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 40 }, { 2, 60 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 신기루 사막 난이도 3	
		DungeonID = GC_GM_QUEST43,	
		DungeonLv = 2,	
		MobGrade = 0,	
		ChampionCount = { { 3, 45 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 1, 30 }, { 2, 40 }, { 3, 30 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 신기루 사막 난이도 Hell	
		DungeonID = GC_GM_QUEST43,	
		DungeonLv = 3,	
		MobGrade = 0,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 50}, {CP_ARROW_DEFENSE, 25}, {CP_HP_RECOVERY, 25} },
			{ {CP_SUPER_ARMOR, 40}, {CP_FAST_STAND, 30}, {CP_SHOCKWAVE, 30} },
			{ {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
		},	
	},		
	{		
		-- 신기루 사막 난이도 Hell	
		DungeonID = GC_GM_QUEST43,	
		DungeonLv = 3,	
		MobGrade = 1,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		
	{		
		-- 신기루 사막 난이도 Hell	
		DungeonID = GC_GM_QUEST43,	
		DungeonLv = 3,	
		MobGrade = 2,	
		ChampionCount = { { 100, 100 }, },    -- 챔피언 몇마리 나올래?	
		PropertyCount = { { 3, 100 }, },    -- 속성 몇개 가질래?	
		Property =	
		{	
			{ {CP_ATTACK, 100} },
			{ {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
			{ {CP_ATTACK_CURES, 70}, {CP_SUPER_ARMOR, 30} },
		},	
	},		

--// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++			

}