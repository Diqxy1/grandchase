-- ���Ӹ��
GC_GM_INVALID           = -1

GC_GM_TUTORIAL          = 0
GC_GM_TEAM_MODE         = 1
GC_GM_SURVIVAL_MODE     = 2
GC_GM_TAG_TEAM          = 3
GC_GM_TAG_SURVIVAL      = 4
GC_GM_GUILD_BATTLE      = 5
GC_GM_INDIGO_TEAM       = 6
GC_GM_QUEST0            = 7     -- �÷��� ��
GC_GM_QUEST1            = 8     -- �÷��� ž
GC_GM_QUEST2            = 9     -- ������ܰ�
GC_GM_QUEST3            = 10    -- ĳ����ġ
GC_GM_QUEST4            = 11    -- ��ũ���
GC_GM_QUEST5            = 12    -- ������ ����
GC_GM_QUEST6            = 13    -- ������ ��
GC_GM_QUEST7            = 14    -- �ͼ��� ���
GC_GM_QUEST8            = 15    -- ������ ��
GC_GM_QUEST9            = 16    -- ������ ����
GC_GM_QUEST10           = 17    -- ������ ����
GC_GM_QUEST11           = 18    -- ���������� ��
GC_GM_QUEST12           = 19    -- ���������� �ٴ�
GC_GM_QUEST13           = 20    -- �����ƴ��
GC_GM_QUEST14           = 21    -- ���� ���
GC_GM_QUEST15           = 22    -- ��긴��
GC_GM_QUEST16           = 23    -- ī�������� ��
GC_GM_QUEST17           = 24    -- ī���� ������ ����
GC_GM_QUEST18           = 25    -- ī���� ������ ����
GC_GM_QUEST19           = 26    -- �����޽þ��� ����
GC_GM_QUEST20           = 27    -- ���Ͼ�������
GC_GM_QUEST21           = 30    -- ��ȯ�ǽ���
GC_GM_QUEST22           = 36    -- �߱� ���� ����
GC_GM_ANGELS_EGG        = 37    -- õ�� ��
GC_GM_CAPTAIN           = 38    -- ������
GC_GM_QUEST23           = 39    -- ���� ��Ʈ�� ��
GC_GM_QUEST24           = 40    -- �ǹ��������� ����
GC_GM_QUEST25           = 41    -- �̸��� ȣ��
GC_GM_QUEST26           = 42    -- �̱״Ͻ��� ���
GC_GM_QUEST27           = 43    -- �˴���
GC_GM_QUEST28           = 44    -- �ǹ����� ������ ��
GC_GM_QUEST29           = 45    -- �ǹ����� �������
GC_GM_QUEST30           = 46    -- �ǹ����� ������ ���
GC_GM_QUEST31           = 47    -- ���Ͼ� ���� ������ ����
GC_GM_QUEST32           = 48    -- ���Ͼ� ����
GC_GM_QUEST33           = 49    -- ���Ͼ� ������
GC_GM_QUEST34           = 50    -- ���Ͼ� ������ ��
GC_GM_QUEST35           = 51    -- �����޽þ� ���� ����
GC_GM_QUEST36           = 52    -- �ǹ����� ���� ����
GC_GM_QUEST37           = 53    -- ������ ���� ����
GC_GM_QUEST38           = 54    -- ���Ͼ� ���� ����
GC_GM_QUEST39           = 55    -- ��� �뿪�� (GoblinLaborCamp)
GC_GM_QUEST40           = 56    -- ���Ͼ��� �ͳ� (UndergroundTunnel)
GC_GM_QUEST41           = 57    -- ���� ���� (KungjiVillage)
GC_GM_QUEST42           = 58    -- ����� ���ʱ��� (DwarvenBase)
GC_GM_QUEST43           = 59    -- 
GC_GM_QUEST44           = 60    -- 



-- Ư���ɷ� �Ӽ�
CP_ATTACK           = 1

CP_DEFENCE          = 2
CP_HP_RECOVERY      = 4

CP_SUPER_ARMOR      = 8
CP_FAST_STAND       = 16
-- �߰��� è�Ǿ� �Ӽ�
CP_ARROW_DEFENSE    = 256  -- ȭ�� ���
CP_SHOCKWAVE        = 512  -- �����

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
        -- ������ܰ� ���̵� 1
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ܰ� ���̵� 2
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ܰ� ���̵� 3
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ������ܰ� ���̵� Hell
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ������ܰ� ���̵� Hell
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ������ܰ� ���̵� Hell
        DungeonID = GC_GM_QUEST2,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ĳ����ġ ���̵� 1
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ĳ����ġ ���̵� 2
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ĳ����ġ ���̵� 3
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ĳ����ġ ���̵� Hell
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ĳ����ġ ���̵� Hell
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ĳ����ġ ���̵� Hell
        DungeonID = GC_GM_QUEST3,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ��ũ��� ���̵� 1
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ��ũ��� ���̵� 2
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ��ũ��� ���̵� 3
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ��ũ��� ���̵� Hell
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ��ũ��� ���̵� Hell
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ��ũ��� ���̵� Hell
        DungeonID = GC_GM_QUEST4,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- �������� ���̵� 1
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �������� ���̵� 2
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �������� ���̵� 3
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �������� ���̵� Hell
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �������� ���̵� Hell
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �������� ���̵� Hell
        DungeonID = GC_GM_QUEST5,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ������ �� ���̵� 1
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ �� ���̵� 2
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ �� ���̵� 3
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ������ �� ���̵� Hell
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ������ �� ���̵� Hell
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ������ �� ���̵� Hell
        DungeonID = GC_GM_QUEST6,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- �ͼ��� ��� ���̵� 1
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ͼ��� ��� ���̵� 2
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ͼ��� ��� ���̵� 3
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �ͼ��� ��� ���̵� Hell
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ͼ��� ��� ���̵� Hell
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �ͼ��� ��� ���̵� Hell
        DungeonID = GC_GM_QUEST7,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ������ �� ���̵� 1
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ �� ���̵� 2
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ �� ���̵� 3
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ������ �� ���̵� Hell
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ������ �� ���̵� Hell
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ������ �� ���̵� Hell
        DungeonID = GC_GM_QUEST8,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ������ ���� ���̵� 1
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ ���� ���̵� 2
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ ���� ���̵� 3
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ������ ���� ���̵� Hell
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ������ ���� ���̵� Hell
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ������ ���� ���̵� Hell
        DungeonID = GC_GM_QUEST9,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ������ ���� ���̵� 1
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ ���� ���̵� 2
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ������ ���� ���̵� 3
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ������ ���� ���̵� Hell
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ������ ���� ���̵� Hell
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ������ ���� ���̵� Hell
        DungeonID = GC_GM_QUEST10,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ���������� �� ���̵� 1
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���������� �� ���̵� 2
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���������� �� ���̵� 3
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���������� �� ���̵� Hell
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���������� �� ���̵� Hell
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ���������� �� ���̵� Hell
        DungeonID = GC_GM_QUEST11,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ���������� �ٴ� ���̵� 1
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���������� �ٴ� ���̵� 2
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���������� �ٴ� ���̵� 3
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���������� �ٴ� ���̵� Hell
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���������� �ٴ� ���̵� Hell
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ���������� �ٴ� ���̵� Hell
        DungeonID = GC_GM_QUEST12,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- �����ƴ�� ���̵� 1
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �����ƴ�� ���̵� 2
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �����ƴ�� ���̵� 3
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �����ƴ�� ���̵� Hell
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �����ƴ�� ���̵� Hell
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �����ƴ�� ���̵� Hell
        DungeonID = GC_GM_QUEST13,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ���� ��� ���̵� 1
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���� ��� ���̵� 2
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���� ��� ���̵� 3
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --���� ��� ���̵� Hell
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���� ��� ���̵� Hell
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ���� ��� ���̵� Hell
        DungeonID = GC_GM_QUEST14,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- �� �긴�� ���̵� 1
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� �긴�� ���̵� 2
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� �긴�� ���̵� 3
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --�� �긴�� ���̵� Hell
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �� �긴�� ���̵� Hell
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �� �긴�� ���̵� Hell
        DungeonID = GC_GM_QUEST15,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ī�������� �� ���̵� 1
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ī�������� �� ���̵� 2
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ī�������� �� ���̵� 3
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --ī�������� �� ���̵� Hell
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ī�������� �� ���̵� Hell
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ī�������� �� ���̵� Hell
        DungeonID = GC_GM_QUEST16,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ī���������� ���� ���̵� 1
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ī���������� ���� ���̵� 2
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ī���������� ���� ���̵� 3
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --ī���������� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ī���������� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ī���������� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST17,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- ī���������� ���� ���̵� 1
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ī���������� ���� ���̵� 2
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ī���������� ���� ���̵� 3
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --ī���������� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ī���������� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ī���������� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST18,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    
    
    {
        -- �����޽þ��� ���� ���̵� 1
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 30}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 20} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �����޽þ��� ���� ���̵� 2
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 30}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 20} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �����޽þ��� ���� ���̵� 3
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 30}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 20} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --�����޽þ��� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �����޽þ��� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �����޽þ��� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST19,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
        
    
    {
        -- ���Ͼ������� ���̵� 1
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ������� ���̵� 2
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ������� ���̵� 3
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --���Ͼ������� ���̵� Hell
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���Ͼ������� ���̵� Hell
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ���Ͼ������� ���̵� Hell
        DungeonID = GC_GM_QUEST20,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
    

    {
        -- ��ȯ�ǽ��� ���̵� 1
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ��ȯ�ǽ��� ���̵� 2
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ��ȯ�ǽ��� ���̵� 3
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --��ȯ�ǽ��� ���̵� Hell
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ��ȯ�ǽ��� ���̵� Hell
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ��ȯ�ǽ��� ���̵� Hell
        DungeonID = GC_GM_QUEST21,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },

    {
        -- �ҽ��� ���� ���̵� 1
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ҽ��� ���� ���̵� 2
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ҽ��� ���� ���̵� 3
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --�ҽ��� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ҽ��� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �ҽ��� ���� ���̵� Hell
        DungeonID = GC_GM_QUEST22,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- ���� ��Ʈ�� �� 1
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���� ��Ʈ�� �� 2
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���� ��Ʈ�� �� 3
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        --���� ��Ʈ�� �� Hell
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���� ��Ʈ�� �� Hell
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- ���� ��Ʈ�� �� Hell
        DungeonID = GC_GM_QUEST23,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �ǹ��������� ���� 1
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ǹ��������� ���� 2
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ǹ��������� ���� 3
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �ǹ��������� ���� Hell
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ǹ��������� ���� Hell
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �ǹ��������� ���� Hell
        DungeonID = GC_GM_QUEST24,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �� ���� 1
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� ���� 2
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� ���� 3
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST25,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �� ���� 1
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� ���� 2
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� ���� 3
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST26,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �� ���� 1
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� ���� 2
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �� ���� 3
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50}, },
        },
    },
	
	{
        -- �� ���� Hell
        DungeonID = GC_GM_QUEST27,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �ǹ����� ������ �� 1
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- �ǹ����� ������ �� 2
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ǹ����� ������ �� 3
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �ǹ����� ������ �� Hell
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ǹ����� ������ �� Hell
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- �ǹ����� ������ �� Hell
        DungeonID = GC_GM_QUEST28,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �ǹ����� ������� 1
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- �ǹ����� ������� 2
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ǹ����� ������� 3
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �ǹ����� ������� Hell
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ǹ����� ������� Hell
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, {CP_SHOCKWAVE, 40} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- �ǹ����� ������� Hell
        DungeonID = GC_GM_QUEST29,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- �ǹ����� ������ ��� 1
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- �ǹ����� ������ ��� 2
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ǹ����� ������ ��� 3
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �ǹ����� ������ ��� Hell
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ǹ����� ������ ��� Hell
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- �ǹ����� ������ ��� Hell
        DungeonID = GC_GM_QUEST30,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_STONE, 50} },
        },
    },
    
---------------
	{
        -- ���Ͼ� ���� ������ ���� 1
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ���Ͼ� ���� ������ ���� 2
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ� ���� ������ ���� 3
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���Ͼ� ���� ������ ���� Hell
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���Ͼ� ���� ������ ���� Hell
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 100}, },
            { {CP_FAST_STAND, 30}, {CP_ARROW_DEFENSE, 30}, },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ���Ͼ� ���� ������ ���� Hell
        DungeonID = GC_GM_QUEST31,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- ���Ͼ� ���� 1
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ���Ͼ� ���� 2
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ� ���� 3
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���Ͼ� ���� Hell
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���Ͼ� ���� Hell
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100}  },
        },
    },
	
	{
        -- ���Ͼ� ���� Hell
        DungeonID = GC_GM_QUEST32,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- ���Ͼ� ������ 1
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ���Ͼ� ������ 2
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ� ������ 3
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���Ͼ� ������ Hell
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���Ͼ� ������ Hell
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ���Ͼ� ������ Hell
        DungeonID = GC_GM_QUEST33,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- ���Ͼ� ������ �� 1
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ���Ͼ� ������ �� 2
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ� ������ �� 3
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���Ͼ� ������ �� Hell
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���Ͼ� ������ �� Hell
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ���Ͼ� ������ �� Hell
        DungeonID = GC_GM_QUEST34,
        DungeonLv = 3,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
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
        -- ��� �뿪�� 1
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ��� �뿪�� 2
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ��� �뿪�� 3
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ��� �뿪�� Hell
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ��� �뿪�� Hell
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ��� �뿪�� Hell
        DungeonID = GC_GM_QUEST39,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- ���Ͼ��� �ͳ� 1
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ���Ͼ��� �ͳ� 2
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���Ͼ��� �ͳ� 3
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���Ͼ��� �ͳ� Hell
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���Ͼ��� �ͳ� Hell
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ���Ͼ��� �ͳ� Hell
        DungeonID = GC_GM_QUEST40,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 50}, {CP_ATTACK_ICE, 50} },
        },
    },

---------------
	{
        -- ���� ���� 1
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ���� ���� 2
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ���� ���� 3
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ���� ���� Hell
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ���� ���� Hell
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ���� ���� Hell
        DungeonID = GC_GM_QUEST41,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- ����� ���ʱ��� 1
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ����� ���ʱ��� 2
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ����� ���ʱ��� 3
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ����� ���ʱ��� Hell
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ����� ���ʱ��� Hell
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 3,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ����� ���ʱ��� Hell
        DungeonID = GC_GM_QUEST42,
        DungeonLv = 3,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- �ű�� �縷 1
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- �ű�� �縷 2
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- �ű�� �縷 3
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- �ű�� �縷 Hell
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- �ű�� �縷 Hell
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 3,
        MobGrade  = 1,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- �ű�� �縷 Hell
        DungeonID = GC_GM_QUEST43,
        DungeonLv = 3,
        MobGrade  = 2,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_ARROW_DEFENSE, 30} },
            { {CP_ATTACK_CURES, 100} },
        },
    },

---------------
	{
        -- ����� ���޷� 1
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 0,
        MobGrade  = 0,        
        ChampionCount = { { 1,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  80 }, { 2,  20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },

    {
        -- ����� ���޷� 2
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 1,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  60 }, { 2,  30 }, { 3,   10 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
    
    {
        -- ����� ���޷� 3
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 2,
        MobGrade  = 0,        
        ChampionCount = { { 1,  40 }, { 2,  30 }, { 3,   10 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 1,  40 }, { 2,  40 }, { 3,   20 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 25}, {CP_FAST_STAND, 20}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 30}, {CP_ATTACK_ICE, 30} },
        },
    },
	
	{
        -- ����� ���޷� Hell
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 3,
        MobGrade  = 0,        
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 70}, {CP_HP_RECOVERY, 30} },
            { {CP_SUPER_ARMOR, 20}, {CP_FAST_STAND, 25}, {CP_ARROW_DEFENSE, 25}, {CP_SHOCKWAVE, 30} },
            { {CP_ATTACK_CURES, 40}, {CP_ATTACK_STONE, 40}, {CP_ATTACK_ICE, 20} },
        },
    },
	
	{
        -- ����� ���޷� Hell
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 3,
        MobGrade  = 1,
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },
	
	{
        -- ����� ���޷� Hell
        DungeonID = GC_GM_QUEST44,
        DungeonLv = 3,
        MobGrade  = 2,
        ChampionCount = { { 100,  100 }, }, -- è�Ǿ� ��� ���÷�?
        PropertyCount = { { 3,   100 }, },   -- �Ӽ� � ������?
        Property =
        {
            { {CP_ATTACK, 100} },
            { {CP_DEFENCE, 80}, {CP_ARROW_DEFENSE, 20} },
            { {CP_ATTACK_CURES, 100} },
        },
    },	
}

