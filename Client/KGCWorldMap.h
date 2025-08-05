enum EGCGameMode;

enum EM_DUNGEON_TYPE
{
    DUNGEON_TYPE_INVALID = -1,
    DUNGEON_TYPE_NORMAL = 0,
    DUNGEON_TYPE_HERO = 1,
    DUNGEON_TYPE_EVENT = 2,
    DUNGEON_TYPE_RELAY_HERO = 3,
    DUNGEON_TYPE_RAID_HERO = 4,
    DUNGEON_TYPE_INFINITY = 5,
    DUNGEON_TYPE_SPECIAL_STAGE = 6,
    DUNGEON_TYPE_RAID_INFINITY = 7,
};

enum EGCWorldMap
{
    WORLDMAP_BERMESIA_SERDIN    =  0,
    WORLDMAP_ELIA               =  1,
    WORLDMAP_XENEA              =  2,
    WORLDMAP_UPPERMAP           =  3,
    WORLDMAP_SILVERLAND	        =  4,
    WORLDMAP_ATUM	            =  5,
    WORLDMAP_ACHAEMEDIA	        =  6,
    WORLDMAP_FLOSLAND           =  7,
    WORLDMAP_DEVILDOM           =  8,
    WORLDMAP_BERMESIA_CANAVAN   =  9,
    WORLDMAP_ELYOS              = 10,
    WORLDMAP_NEWMAP             = 11,

    NUM_WORLDMAP,
};

struct SAbilityRevision
{
    bool bLoadTable;        // 보정테이블을 로드했는가?
    int iStdLevel_AP;       // 공격력 보정레벨
    int iStdLevel_DP;       // 방어력 보정레벨
    int iStdLevel_HP;       // 생명력 보정레벨
    float fSpecialAP;       // 필살기 공격력 적용비율
    float fDrainHP;         // HP흡수율 적용비율
    float fDrainMP;         // MP흡수율 적용비율
    float fRecoveryHP;      // HP회복율 적용비율
    float fRecoveryMP;      // MP회복율 적용비율
    float fCriticalRate;   // 크리티컬확률 적용비율
    float fCounterDEF;      // 카운터시 MP소모량 감소% 적용비율
    float fDarkATK;         // 암흑검 데미지 적용비율
    float fCriticalATK;     // 크리티컬 공격력 적용비율
    float fPetMPRecovery;   // 펫 MP 회복 증가 적용비율
    float fPortionUP;       // 포션성능증가 적용비율
    bool  bNoItemAbility;   // 아이템 능력치를 적용할것인지, 말것인지,

    SAbilityRevision()
    {
        init();
    }

    void init()
    {
        bLoadTable = false;
        iStdLevel_AP = 0;
        iStdLevel_DP = 0;
        iStdLevel_HP = 0;
        fSpecialAP = 1.0f;
        fDrainHP = 1.0f;
        fDrainMP = 1.0f;
        fRecoveryHP = 1.0f;
        fRecoveryMP = 1.0f;
        fCriticalRate = 1.0f;
        fCounterDEF = 1.0f;
        fDarkATK = 1.0f;
        fCriticalATK = 1.0f;
        fPetMPRecovery = 1.0f;
        fPortionUP = 1.0f;
        bNoItemAbility = false;
    }
};

struct SCameraZoom
{
    int m_iPosUI_X;
    int m_iPosUI_Y;
    bool m_bNotJumpZoom;
    bool m_bNotDashZoom;
    bool m_bNotSpecialZoom;
    bool m_bNotBossDieEffect;
    float m_fFristZoom;
    bool m_bShowDesc;
    std::vector<float> m_vecZoomStep;

    SCameraZoom()
    {
        init();
    }

    void init()
    {
        m_iPosUI_X = 0;
        m_iPosUI_Y = 0;
        m_bNotJumpZoom = false;
        m_bNotDashZoom = false;
        m_bNotSpecialZoom = false;
        m_bNotBossDieEffect = false;
        m_bShowDesc = true;
        m_fFristZoom = 0.0f;
        m_vecZoomStep.clear();
    }
};

struct SMiniMapTexInfo
{
    std::string m_strFileName;
    D3DXVECTOR4 m_vUV;
    DWORD m_dwColor;

    SMiniMapTexInfo()
    {
        m_strFileName.clear();
        m_vUV = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
        m_dwColor = 0xffffffff;
    }

    void Init()
    {
        m_strFileName.clear();
        m_vUV = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
        m_dwColor = 0xffffffff;
    }
};

struct SMiniMapInfo
{
    int m_iPosUI_X;
    int m_iPosUI_Y;
    int m_iWidthUI;
    int m_iHeightUI;
    int m_iRenewFrame;
    bool m_bIsShow;

    float m_fScale_FootHold;
    float m_fScale_Enemy;
    float m_fScale_Ally;
    float m_fScale_My;

    SMiniMapTexInfo m_kTexInfo_Enemy;
    SMiniMapTexInfo m_kTexInfo_Ally;
    SMiniMapTexInfo m_kTexInfo_My;

    D3DCOLOR m_dxColor_FootHold;
    D3DCOLOR m_dxColor_Enemy;
    D3DCOLOR m_dxColor_Ally;
    D3DCOLOR m_dxColor_My;
    std::map<std::pair<int, DWORD>, D3DCOLOR>    m_mapETCColor;   // <기타오브젝트ID, 오브젝트ID>, 색상

    SMiniMapInfo()
    {
        Init();
    }

    const SMiniMapInfo& operator = ( const SMiniMapInfo& kSrc_ )
    {
        m_iPosUI_X = kSrc_.m_iPosUI_X;
        m_iPosUI_Y = kSrc_.m_iPosUI_Y;
        m_iWidthUI = kSrc_.m_iWidthUI;
        m_iHeightUI = kSrc_.m_iHeightUI;
        m_iRenewFrame = kSrc_.m_iRenewFrame;
        m_bIsShow = kSrc_.m_bIsShow;
        m_fScale_FootHold = kSrc_.m_fScale_FootHold;
        m_fScale_Enemy = kSrc_.m_fScale_Enemy;
        m_fScale_Ally = kSrc_.m_fScale_Ally;
        m_fScale_My = kSrc_.m_fScale_My;
        m_dxColor_FootHold = kSrc_.m_dxColor_FootHold;
        m_dxColor_Enemy = kSrc_.m_dxColor_Enemy;
        m_dxColor_Ally = kSrc_.m_dxColor_Ally;
        m_dxColor_My = kSrc_.m_dxColor_My;
        m_mapETCColor = kSrc_.m_mapETCColor;

        m_kTexInfo_Enemy = kSrc_.m_kTexInfo_Enemy;
        m_kTexInfo_Ally = kSrc_.m_kTexInfo_Ally;
        m_kTexInfo_My = kSrc_.m_kTexInfo_My;

        return (*this);
    }

    void Init()
    {
        m_iPosUI_X = 0;
        m_iPosUI_Y = 0;
        m_iWidthUI = 0;
        m_iHeightUI = 0;
        m_iRenewFrame = 1;
        m_bIsShow = false;
        m_fScale_FootHold = 1.0f;
        m_fScale_Enemy = 1.0f;
        m_fScale_Ally = 1.0f;
        m_fScale_My = 1.0f;

        m_kTexInfo_Enemy.Init();
        m_kTexInfo_Ally.Init();
        m_kTexInfo_My.Init();

        m_dxColor_FootHold = D3DXCOLOR(0,0,0,0);
        m_dxColor_Enemy = D3DXCOLOR(0,0,0,0);
        m_dxColor_Ally = D3DXCOLOR(0,0,0,0);
        m_dxColor_My = D3DXCOLOR(0,0,0,0);

        m_mapETCColor.clear();
    }
};

struct SAlarmInfo
{
    int iMitMin;
    int iMitMax;
    int iSecMin;
    int iSecMax;
    std::string strSound;
};

struct SLimitedTimeInfo
{
    std::vector<SAlarmInfo> m_vecAlarmInfo;

    SLimitedTimeInfo()
    {
        Init();
    }

    const SLimitedTimeInfo& operator = ( const SLimitedTimeInfo& kSrc_ )
    {
        m_vecAlarmInfo = kSrc_.m_vecAlarmInfo;
        return (*this);
    }

    void Init()
    {
        m_vecAlarmInfo.clear();
    }
};

struct SWorldMapParticle
{
    std::string m_strName;
    D3DXVECTOR2 m_vPos;

    SWorldMapParticle()
    {
        m_strName.clear();
        m_vPos = D3DXVECTOR2(0.0f, 0.0f);
    }

    SWorldMapParticle( std::string strName, float fX, float fY )
    {
        m_strName = strName;
        m_vPos = D3DXVECTOR2(fX, fY);
    }

    const SWorldMapParticle& operator = ( const SWorldMapParticle& kSrc_ )
    {
        m_strName = kSrc_.m_strName;
        m_vPos = kSrc_.m_vPos;
        return (*this);
    }
};

struct SMacroCommunity
{
    std::string m_strTexture;
    int m_iStringID;
    int m_iStringColorR;
    int m_iStringColorG;
    int m_iStringColorB;
    int m_iReEntryCool;
    int m_iSoundID;

    SMacroCommunity()
    {
        m_strTexture.clear();;
        m_iStringID = -1;
        m_iStringColorR = 255;
        m_iStringColorG = 255;
        m_iStringColorB = 255;
        m_iReEntryCool = 0;
        m_iSoundID = -1;
    }

    const SMacroCommunity& operator = ( const SMacroCommunity& kSrc_ )
    {
        m_strTexture = kSrc_.m_strTexture;
        m_iStringID = kSrc_.m_iStringID;
        m_iStringColorR = kSrc_.m_iStringColorR;
        m_iStringColorG = kSrc_.m_iStringColorG;
        m_iStringColorB = kSrc_.m_iStringColorB;
        m_iReEntryCool = kSrc_.m_iReEntryCool;
        m_iSoundID = kSrc_.m_iSoundID;
        return (*this);
    }
};

struct SDungeonInfo
{
    EGCGameMode         m_emGameMode;       //  게임모드 ( 던전 Flag ) 
    std::string         m_strLoadingImg;    // 로딩 파일 이름
    std::string         m_strLoadingBreakImg;    // 로딩 파일 이름
    int                 m_nLimitLevel;     //  던전 난이도 ( 노멀 1, 어려움 2, 매우어려움 3 ) 
    bool                m_bHellMode;        //  헬모드 여부
    bool                m_bBreakMode;        //  헬모드 여부
    EM_DUNGEON_TYPE     m_emDungeonType;    //  던전 타입
    int                 m_nDungeonName;     //  던전 이름 스트링 넘버
    int                 m_nWorldMapName;    //  월드맵 이름 스트링 넘버
    bool                m_bShowPlayTime;  // 플레이 타임 출력여부
    bool                m_bShowRank;  // 랭크UI 출력여부
    bool                m_bShowTodayTip;  // 오늘의 팁 출력여부
    bool                m_bComboEffect;  // 콤보이펙트 출력여부
    bool                m_bUseGameItem;    // 게임아이템 사용여부
    bool                m_bUsePetSkill;    // 펫 필살기 사용여부
    bool                m_bNoRecoveryMode; // MP자동회복 금지모드
    int                 m_nMaxPlayerNum;    // 던전 플레이 인원
    int                 m_iCharLife;    // 던전 기본 Life

    std::map<EGCWorldMap,std::pair<int, int>>   m_mapWorldMapInfo;   //  월드맵 등록 정보 <아이콘, Path>
    std::vector<int>    m_vecNotUseShortCutItem;    // 숏컷아이템사용제한할 난이도
    std::vector<int>    m_vecNotUseRebirth;    // 부활사용제한할 난이도

    SCameraZoom         m_sCameraZoom;
    SMiniMapInfo        m_sMiniMapInfo;
    SLimitedTimeInfo    m_sLimitedTimeInfo;
    SAbilityRevision    m_sAbilityRevision;
    std::map<int, SMacroCommunity>     m_mapMacroCommunity;
    std::map<int, std::vector<SWorldMapParticle>>   m_mapMapParticle;

    SDungeonInfo()
    {
        m_emGameMode = GC_GM_INVALID;
        m_strLoadingImg = std::string();
        m_nLimitLevel = 1;
        m_bHellMode = false;
        m_bBreakMode = false;
        m_emDungeonType = DUNGEON_TYPE_NORMAL;
        m_nDungeonName = 0;
        m_nWorldMapName = 0;
        m_bShowPlayTime = true;
        m_bShowRank = true;
        m_bShowTodayTip = true;
        m_bComboEffect = true;
        m_bUseGameItem =true;
        m_bUsePetSkill = true;
        m_bNoRecoveryMode = false;
        m_nMaxPlayerNum = OLD_MAX_DUNGEON_PLAYER_NUM;
        m_iCharLife = -1;

        m_vecNotUseShortCutItem.clear();
        m_vecNotUseRebirth.clear();

        m_mapWorldMapInfo.clear();
        m_sCameraZoom.init();
        m_sAbilityRevision.init();
        m_mapMapParticle.clear();
    }
};

typedef std::map<EGCGameMode, SDungeonInfo> MAP_DUNGEON_INFO;
typedef MAP_DUNGEON_INFO::iterator          MAP_DUNGEON_INFO_ITER;
