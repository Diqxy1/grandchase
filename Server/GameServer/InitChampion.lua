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
GC_GM_QUEST43           = 59    -- 
GC_GM_QUEST44           = 60    -- 



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
    {
        -- 세르딘외곽 난이도 1
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 세르딘외곽 난이도 2
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 세르딘외곽 난이도 3
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 세르딘외곽 난이도 Hell
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 세르딘외곽 난이도 Hell
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 세르딘외곽 난이도 Hell
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 캐리비치 난이도 1
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 캐리비치 난이도 2
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 캐리비치 난이도 3
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 캐리비치 난이도 Hell
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 캐리비치 난이도 Hell
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 캐리비치 난이도 Hell
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 오크사원 난이도 1
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 오크사원 난이도 2
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 오크사원 난이도 3
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 오크사원 난이도 Hell
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 오크사원 난이도 Hell
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 오크사원 난이도 Hell
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 고르고스던전 난이도 1
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 고르고스던전 난이도 2
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 고르고스던전 난이도 3
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 고르고스던전 난이도 Hell
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 고르고스던전 난이도 Hell
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 고르고스던전 난이도 Hell
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 엘프의 숲 난이도 1
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 엘프의 숲 난이도 2
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 엘프의 숲 난이도 3
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 엘프의 숲 난이도 Hell
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 엘프의 숲 난이도 Hell
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 엘프의 숲 난이도 Hell
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 맹세의 계곡 난이도 1
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 맹세의 계곡 난이도 2
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 맹세의 계곡 난이도 3
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 맹세의 계곡 난이도 Hell
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 맹세의 계곡 난이도 Hell
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 맹세의 계곡 난이도 Hell
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 망각의 늪 난이도 1
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 망각의 늪 난이도 2
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 망각의 늪 난이도 3
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 망각의 늪 난이도 Hell
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 망각의 늪 난이도 Hell
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 망각의 늪 난이도 Hell
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 망자의 묘지 난이도 1
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 망자의 묘지 난이도 2
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 망자의 묘지 난이도 3
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 망자의 묘지 난이도 Hell
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 망자의 묘지 난이도 Hell
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 망자의 묘지 난이도 Hell
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 잊혀진 도시 난이도 1
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 잊혀진 도시 난이도 2
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 잊혀진 도시 난이도 3
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 잊혀진 도시 난이도 Hell
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 잊혀진 도시 난이도 Hell
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 잊혀진 도시 난이도 Hell
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 가이코즈의 성 난이도 1
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 가이코즈의 성 난이도 2
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 가이코즈의 성 난이도 3
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 가이코즈의 성 난이도 Hell
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 가이코즈의 성 난이도 Hell
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 가이코즈의 성 난이도 Hell
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 파투세이의 바다 난이도 1
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 파투세이의 바다 난이도 2
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 파투세이의 바다 난이도 3
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 파투세이의 바다 난이도 Hell
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 파투세이의 바다 난이도 Hell
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 파투세이의 바다 난이도 Hell
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 엘리아대륙 난이도 1
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 엘리아대륙 난이도 2
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 엘리아대륙 난이도 3
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 엘리아대륙 난이도 Hell
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 엘리아대륙 난이도 Hell
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 엘리아대륙 난이도 Hell
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 불의 사원 난이도 1
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 불의 사원 난이도 2
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 불의 사원 난이도 3
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --불의 사원 난이도 Hell
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 불의 사원 난이도 Hell
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 불의 사원 난이도 Hell
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 헬 브릿지 난이도 1
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 헬 브릿지 난이도 2
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 헬 브릿지 난이도 3
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --헬 브릿지 난이도 Hell
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 헬 브릿지 난이도 Hell
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 헬 브릿지 난이도 Hell
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 카제아제의 성 난이도 1
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 카제아제의 성 난이도 2
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 카제아제의 성 난이도 3
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --카제아제의 성 난이도 Hell
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 카제아제의 성 난이도 Hell
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 카제아제의 성 난이도 Hell
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 카스툴유적지 하층 난이도 1
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 카스툴유적지 하층 난이도 2
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 카스툴유적지 하층 난이도 3
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --카스툴유적지 하층 난이도 Hell
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 카스툴유적지 하층 난이도 Hell
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 카스툴유적지 하층 난이도 Hell
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 카스툴유적지 상층 난이도 1
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 카스툴유적지 상층 난이도 2
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 카스툴유적지 상층 난이도 3
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --카스툴유적지 상층 난이도 Hell
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 카스툴유적지 상층 난이도 Hell
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 카스툴유적지 상층 난이도 Hell
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- 베르메시아의 최후 난이도 1
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 30}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 20} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 베르메시아의 최후 난이도 2
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 30}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 20} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 베르메시아의 최후 난이도 3
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 30}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 20} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --베르메시아의 최후 난이도 Hell
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 베르메시아의 최후 난이도 Hell
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 베르메시아의 최후 난이도 Hell
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
        
    
    {
        -- 제니아접경지 난이도 1
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 제니아접경지 난이도 2
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 제니아접경지 난이도 3
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --제니아접경지 난이도 Hell
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 제니아접경지 난이도 Hell
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 제니아접경지 난이도 Hell
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    

    {
        -- 순환의신전 난이도 1
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 순환의신전 난이도 2
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 순환의신전 난이도 3
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --순환의신전 난이도 Hell
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 순환의신전 난이도 Hell
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 순환의신전 난이도 Hell
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },

    {
        -- 소실의 협곡 난이도 1
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 소실의 협곡 난이도 2
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 소실의 협곡 난이도 3
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --소실의 협곡 난이도 Hell
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 소실의 협곡 난이도 Hell
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 소실의 협곡 난이도 Hell
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
-----------
    {
        -- 성난 엔트의 숲 1
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 성난 엔트의 숲 2
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 성난 엔트의 숲 3
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --성난 엔트의 숲 Hell
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 성난 엔트의 숲 Hell
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 성난 엔트의 숲 Hell
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
-----------
    {
        -- 실버나이츠의 폐허 1
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 실버나이츠의 폐허 2
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 실버나이츠의 폐허 3
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 실버나이츠의 폐허 Hell
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 실버나이츠의 폐허 Hell
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 실버나이츠의 폐허 Hell
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },

-----------
    {
        -- 물 던전 1
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 물 던전 2
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 물 던전 3
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 물 던전 Hell
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 물 던전 Hell
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 물 던전 Hell
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
-----------
    {
        -- 불 던전 1
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 불 던전 2
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 불 던전 3
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 불 던전 Hell
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 불 던전 Hell
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 불 던전 Hell
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
-----------
    {
        -- 늪 던전 1
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 늪 던전 2
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 늪 던전 3
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 늪 던전 Hell
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 늪 던전 Hell
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- 늪 던전 Hell
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
---------------
	{
        -- 실버랜드 원시의 섬 1
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 실버랜드 원시의 섬 2
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 실버랜드 원시의 섬 3
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 실버랜드 원시의 섬 Hell
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 실버랜드 원시의 섬 Hell
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 실버랜드 원시의 섬 Hell
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
---------------
	{
        -- 실버랜드 용암지대 1
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 실버랜드 용암지대 2
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 실버랜드 용암지대 3
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 실버랜드 용암지대 Hell
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 실버랜드 용암지대 Hell
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 실버랜드 용암지대 Hell
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },

---------------
	{
        -- 실버랜드 빅터의 요새 1
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 실버랜드 빅터의 요새 2
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 실버랜드 빅터의 요새 3
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 실버랜드 빅터의 요새 Hell
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 실버랜드 빅터의 요새 Hell
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 실버랜드 빅터의 요새 Hell
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50} },
        },
    },
    
---------------
	{
        -- 제니아 빙하 조율의 제단 1
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 제니아 빙하 조율의 제단 2
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 제니아 빙하 조율의 제단 3
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 제니아 빙하 조율의 제단 Hell
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 제니아 빙하 조율의 제단 Hell
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 제니아 빙하 조율의 제단 Hell
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 제니아 돌산 1
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 제니아 돌산 2
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 제니아 돌산 3
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 제니아 돌산 Hell
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 제니아 돌산 Hell
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100}  },
        },
    },
	
	{
        -- 제니아 돌산 Hell
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 제니아 숲지역 1
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 제니아 숲지역 2
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 제니아 숲지역 3
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 제니아 숲지역 Hell
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 제니아 숲지역 Hell
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 제니아 숲지역 Hell
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 제니아 지배의 성 1
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 제니아 지배의 성 2
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 제니아 지배의 성 3
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 제니아 지배의 성 Hell
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 제니아 지배의 성 Hell
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 제니아 지배의 성 Hell
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 3,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

--//-------------------------------------------------------------------------------------------------

--//-------------------------------------------------------------------------------------------------
	{
        -- 고블린 노역소 1
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 고블린 노역소 2
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 고블린 노역소 3
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 고블린 노역소 Hell
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 고블린 노역소 Hell
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 고블린 노역소 Hell
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 지하암흑 터널 1
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 지하암흑 터널 2
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 지하암흑 터널 3
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 지하암흑 터널 Hell
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 지하암흑 터널 Hell
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 지하암흑 터널 Hell
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_ICE, 50} },
        },
    },

---------------
	{
        -- 쿵지 쿵지 1
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 쿵지 쿵지 2
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 쿵지 쿵지 3
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 쿵지 쿵지 Hell
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 쿵지 쿵지 Hell
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 쿵지 쿵지 Hell
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 드워프 전초기지 1
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 드워프 전초기지 2
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 드워프 전초기지 3
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 드워프 전초기지 Hell
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 드워프 전초기지 Hell
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 3,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 드워프 전초기지 Hell
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 3,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 신기루 사막 1
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 신기루 사막 2
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 신기루 사막 3
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 신기루 사막 Hell
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 신기루 사막 Hell
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 신기루 사막 Hell
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- 드워프 보급로 1
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- 드워프 보급로 2
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- 드워프 보급로 3
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- 드워프 보급로 Hell
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- 드워프 보급로 Hell
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 3,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- 드워프 보급로 Hell
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 3,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- 챔피언 몇마리 나올래?
        PropertyCount = { { 3,   100 }, },   -- 속성 몇개 가질래?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },	
}

