#ifndef _KGCWORLDMAPMANAGER_H_
#define _KGCWORLDMAPMANAGER_H_

#include "KNCSingleton.h"
#include "KGCWorldMap.h"
#include "KGCPrivateDungeonManager.h"

class KGCWorldMapManager
{
    DeclareSingleton( KGCWorldMapManager );

public:
    KGCWorldMapManager();
    ~KGCWorldMapManager();

    enum
    {
        // UI에 마련된 월드맵 아이콘의 갯수..
        // 만약 한 대륙에 던전아이콘이 20개가 넘어간다면 이 수치도 변경해 줘야 한다
        // UI에서도 수정이 필요하고..
        MAX_WORLDMAP_ICON = 20,
        // 위와 마찬가지로.. UI에 마련된 최대 길의 갯수..
        MAX_WORLDMAP_PATH = 30,
    };

    enum EGCUpperWorldMapIcon
    {
        UPPERWORLDMAP_BERMESIA_SERDIN  =  0,
        UPPERWORLDMAP_BERMESIA_CANAVAN =  1,
        UPPERWORLDMAP_SILVERLAND       =  2,
        UPPERWORLDMAP_ELIA             =  3,
        UPPERWORLDMAP_XENEA            =  4,
        UPPERWORLDMAP_ATUM	           =  5,
        UPPERWORLDMAP_ACHAEMEDIA       =  6,
        UPPERWORLDMAP_FLOSLAND         =  7,
        UPPERWORLDMAP_DEVILDOM         =  8,
        UPPERWORLDMAP_ELYOS            =  9,
        UPPERWORLDMAP_NEWMAP           = 10,

        NUM_UPPERWORLDMAP,
    };

	enum WMM_Error
	{
		WMM_OK = 0,
		WMM_NOT_ICON,
		WMM_NOT_AVAILABLE,

		WMM_NUM,
	};
	
	enum GC_PLAY_TYPE {
		GRPT_NOT_INFO = -1, // 던전 정보가 없음.(레이드 OR 영웅던전이 아님)
		GRPT_ADMISSION = 0, // 입장가능
		GRPT_IMPOSSIBLE_ADMISSION = 1, // 입장불가능 (클리어 완료)
		GRPT_CONDITIONS_BELOW = 2, // 입장불가능 (조건 미달)
		GRPT_IMPOSSIBLE_TIME = 3, // 입장불가능 (입장가능한 시간이 아님)
	};

public:
    void Init();
    void InitWorldMap();
    bool LoadDungeonInfo();

    void UpdateWorldMap(); 
	void EnableObject();
	int GetLastDungeonIndex();

    void SelectIcon( int iIconIndex );
    EGCWorldMap GetCurrentWorldMap(){ return m_eCurrentWorldMap; }
	void SetCurrentWorldMap( int iCurrentWorldMap );
	int GetCurrentSelectDungeon(){ return m_iCurrentSelectDungeon; }
	void SetCurrentSelectDungeon( int iSelect );
    void SetCurrentSelectHeroDungeon( int iSelect );
	std::wstring GetCurrentDungeonName();
    std::wstring GetCurrentDungeonName( EGCGameMode eMode );
    EGCGameMode GetGameMode();
    EGCGameMode GetGameMode( int iIconIndex );

    void ShowDungeonMissionList( int iIconIndex );
    void HideDungeonMissionList( int iIconIndex );

    int GetDungeonIndex( EGCGameMode iGameMode );
	EGCWorldMap GetWorldMapEnumFromWorldMapIcon( int iWorldMapIconID );
    EGCUpperWorldMapIcon GetWorldMapIconFromEnum( int iWorldMapEnum );

	bool CheckContinentEnable( EGCWorldMap eWorldMap );
    bool _CheckContinentEnable( EGCWorldMap eWorldMap );
    EGCWorldMap GetPlayableWorldMap();

    int GetDungeonMaxPlayerNum( EGCGameMode eGameMode_ = GC_GM_INVALID );
    int GetDungeonCharBaseLife( EGCGameMode eGameMode_ = GC_GM_INVALID );

	//void UpdateRoomCount();

#if defined( _DEBUG ) || !defined( __PATH__ )
    bool m_bUnLock;
#endif

public:
    bool IsEventDungeon( EGCGameMode eGameMode_ = GC_GM_INVALID );
    bool IsHeroDungeon( EGCGameMode eGameMode_ = GC_GM_INVALID );
    bool IsRelayHeroDungeon( EGCGameMode eGameMode_ = GC_GM_INVALID );
    bool IsRaidHeroDungeon( EGCGameMode eGameMode_ = GC_GM_INVALID );
    bool IsPrivateDungeon( EGCGameMode eGameMode_ = GC_GM_INVALID );    //  캐릭터 전용 던전인가?
    bool IsInfinityDungeon( EGCGameMode eGameMode_ = GC_GM_INVALID );    //  사냥터 인가??
    bool IsInfinityRaid( EGCGameMode eGameMode_ = GC_GM_INVALID );

    bool IsExistEventDungeon( ); //현재 이벤트 던전이 오픈되어 있는지?
    //  영웅던전 및 릴레이 영웅던전 체크 한다.
    bool IsHeroDungeonMode( EGCGameMode eGameMode_ = GC_GM_INVALID );

    std::vector<SWorldMapParticle> GetMapParticleInfo( EGCWorldMap iWorldMap_ );
    std::vector<SWorldMapParticle> GetDungeonParticle( EGCGameMode eGameMode_, EGCWorldMap iWorldMap_ );
    EM_DUNGEON_TYPE GetDungeonType( EGCGameMode eGameMode_ );
    std::wstring GetContinentName( EGCGameMode eGameMode_ );
    SCameraZoom* GetCameraZoomInfo( EGCGameMode eGameMode_ );
    SAbilityRevision* GetAbilityRevisionInfo( EGCGameMode eGameMode_ );
    SMiniMapInfo* GetMiniMapInfo( EGCGameMode eGameMode_ );
    SLimitedTimeInfo* GetLimitedTimeInfo( EGCGameMode eGameMode_ );
    std::map<int, SMacroCommunity> GetMacroCommunity( EGCGameMode eGameMode_ );
    bool IsShowPlayTime( EGCGameMode eGameMode_ );
    bool IsShowRank( EGCGameMode eGameMode_ );
    bool IsShowTodayTip( EGCGameMode eGameMode_ );
    bool IsNotUseShortCutItem( EGCGameMode eGameMode_, int iLevel );
    bool IsNotRebirth( EGCGameMode eGameMode_, int iLevel );
    bool IsShowComboEffect( EGCGameMode eGameMode_ );
    bool IsUseGameItem( EGCGameMode eGameMode_ );
    bool IsUsePetSkill( EGCGameMode eGameMode_ );
    bool IsNoMPRecoveryMode( EGCGameMode eGameMode_ );

    //  헬모드 플레이가 가능한지체크
    bool IsOpenHellMode( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );
	bool IsOpenHellMode( );
    bool IsOpenBreakMode(EGCGameMode eGameMode_, std::map<int, KDungeonUserInfo>& mapStagePlayable_);
    bool IsOpenBreakMode();
    //  해당 던전에 헬모드가 있는지 체크
    bool IsHellMode( EGCGameMode eGameMode_ );
	bool IsHellMode( );
    bool IsBreakMode(EGCGameMode eGameMode_);
    bool IsBreakMode();

    // 해당 던전 플레이 가능한지 체크
    bool IsPlayableDungeon( EGCGameMode eCurGameMode, int iCharID );
    bool _IsPlayableDungeon( EGCGameMode eCurGameMode, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );

    //  해당 던전 플레이 기록이 있는지 체크
    bool IsPlayedDungeon( EGCGameMode eGameMode_, int nLevel, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );

    //  플레이 가능한 던전 레벨( 챔피언 모드 제외 버전 ) 
    int GetPlayEnableDungeonLevel( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );

    int GetPlayEnableTowerLevel(EGCGameMode eGameMode_, std::map<int, KDungeonUserInfo>& mapStagePlayable_);

    //  해당 난이도 플레이 가능한지 체크
    bool IsPlayEnableDungeonLevel( EGCGameMode eGameMode_, int nLevel, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );

    //  해당 게임모드에 마지막 난이도를 가져온다.
    int GetLastPlayDungeonLevel( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );

    int GetCorruptionRecomended( EGCGameMode eGameMode_ );

    //  해당 던전 모드에서 얻을수 있는 보상 갯수를 가져온다. 
    int GetDungeonRewardNum( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ );

    //  해당 던전 모드에서 얻을수 있는 보상 갯수를 설정한다.
    void SetDungeonRewardNum( EGCGameMode eGameMode_, USHORT _usLeftRewardCount );

    //  모든 난이도를 클리어한 던전의 몬스터레벨을 가져온다.
    bool GetClearMonsterLevel( int& iLevel );

    bool IsEnableJoinRoomCheck( EGCGameMode eGameMode_, int nLevel, bool bClearLastDifficult );


    //  파티로 던전이동이 가능한지?
    bool IsEnablePartyDungeon( EGCGameMode eGameMode_ );

	//	던전 시작시 던전 모션 스킵할것 인가?
	bool IsSkipStartMotion( EGCGameMode eGameMode_ );
  
    int GetDungeonLevelString( EGCGameMode eGameMode_, int nLevel );
    int GetBasicLevelString( int nLevel );

public:

    //  던전 난이도 갯수를 가져온다. ( 챔피언 모드 미포함 ) 
    int GetLimiteOpenLevel( EGCGameMode eGameMode_ );
	int GetLimiteOpenLevel( );
	std::vector< EGCGameMode > GetCurWorldMapInfo();

    // 던전 로딩 이미지 이름을 가져온다. 
    std::string GetDungeonLoadingImgName(const EGCGameMode eGamemode_);
    std::string GetDungeonLoadingBreakImgName(const EGCGameMode eGamemode_);

    bool CheckQuestInContinent(EGCWorldMap eWorldMap);
private:
    MAP_DUNGEON_INFO    m_mapDungeonInfo;

 
    //던전 오픈 정보
    std::map< EGCGameMode, std::pair<EGCGameMode,int> > m_mapWorldMapDungeonInfo;    
    
    
    //월드맵 파티클 정보
    std::map< int, std::vector<SWorldMapParticle> > m_mapWorldMapParticleInfo;

	//대륙별 정보를 가짐. 전체 대륙 모드 정보는 없음.
	std::map< int, std::vector< EGCGameMode > > m_mapGameMode;
	std::map< int, std::map<int, EGCGameMode > > m_mapICONGameMode;


    EGCWorldMap m_eCurrentWorldMap; // 현재 선택된 월드맵
    int m_iCurrentSelectDungeon;    // 현재 선택된 던전

    static int m_aiDungeonGameModeSet[];

	std::set<EGCUpperWorldMapIcon> m_setOpenedContinent;

    int m_iNewQuest;
    int m_iMissionListedIndex;

	std::map<int,std::vector<KGameModeState> > m_mapRoomCount;

	DWORD m_dwLastRoomUpdateTime;
	int m_iLastRoomUpdateMap;
    int m_iCharBaseLife;

    std::map<int,std::vector<int>> m_mapDungeonTipList; // 던전 로딩 시 팁 스트링 ID 들 

    std::vector<int> m_vecSkillListGuideDungeonList; // 스킬리스트가 나오는 던전리스트

public:
    bool GetWorldMapOfGameMode(EGCGameMode eGameMode_, OUT int& iGetWorldMap_);

    std::map<int,int> GetEnableRegionNumberAndStringNumber();
    std::map<int,int> GetEnableDungeonNumberAndStringNumber(int iRegion_);
    int GetRegionStringNumber(int iRegionNum);
    int GetDungeonStringNumber(EGCGameMode iDungeonNum);
	std::pair<int, int> GetCurrentDungeonPlayLevelAndLastLevel( EGCGameMode eGameMode_ = GC_GM_INVALID );
	bool IsPlayEnableDungeon( EGCGameMode eGameMode_ , int iCharLevel );
	int GetWorldMapIconNum( EGCWorldMap eWorldmap , int iMode);
	int GetWorldMapPathNum( EGCWorldMap eWorldmap , int iMode);
	void EnableWorldObject();
	void EnableDungeonObject();

	std::map< int, std::vector< EGCGameMode > > GetAllGameMode() {return m_mapGameMode;}
	void SetCurrentSelectGameMode( int iGameMode);
	int GetDungeonIconIndex( EGCGameMode eGameMode_ );
	bool IsEnableWorldObject( EGCGameMode eCurGameMode );
	int GetHeroDungeonPlayType( EGCGameMode eGameMode_ = GC_GM_INVALID , int iChar = -1 );	
    bool IsHeroDungeonGradeCheck( EGCGameMode eGameMode = GC_GM_INVALID , int iChar = -1 );
    int GetChangeUserGradeInfo(int iCharType);
    int GetWorldMapBgm( EGCWorldMap eWorldMap );
    int GetDungeonLoadingTip( EGCGameMode iGameMode );
    bool IsSkillListGuideDungeon( EGCGameMode eGameMode_ );
    int GetCurrentDungeonRank( bool bRoom , bool bBoosInfo = false);
    int GetContinentNameIndex( EGCGameMode eGameMode_ );
};

DefSingletonInline( KGCWorldMapManager );

#endif // _KGCWORLDMAPMANAGER_H_
