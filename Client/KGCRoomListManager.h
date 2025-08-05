#pragma once
#include "KNCSingleton.h"

struct SBossImageInfo
{
	int iBigImageFileIndex;
	int iBigImageOrderIndex;
	int iSmallImageFileIndex;
	int iSmallImageOrderIndex;
    int iRoomBigImageFileIndex;
    int iRoomBigImageOrderIndex;

	int iBreakBigImageFileIndex;
	int iBreakBigImageOrderIndex;
	int iBreakRoomBigImageFileIndex;
	int iBreakRoomBigImageOrderIndex;

    SBossImageInfo()
    : iBigImageFileIndex( 0 )
    , iBigImageOrderIndex( 0 )
    , iSmallImageFileIndex( 0 )
    , iSmallImageOrderIndex( 0 )
    , iRoomBigImageFileIndex( 0 )
    , iRoomBigImageOrderIndex( 0 )
	, iBreakBigImageFileIndex(0)
	, iBreakBigImageOrderIndex(0)
	, iBreakRoomBigImageFileIndex(0)
	, iBreakRoomBigImageOrderIndex(0)
    {
    }
};

class KGCRoomListManager
{
    DeclareSingleton( KGCRoomListManager );

public:
    KGCRoomListManager( void );
    ~KGCRoomListManager( void );

	enum {
		KRLM_LEVEL_1 = 0,
		KRLM_LEVEL_2 = 1,
		KRLM_LEVEL_3 = 2,
		KRLM_LEVEL_HELL = 255,
		KRLM_LEVEL_BREAK = 256,
	};

	enum {
		KRLM_PRIMIUM_NOT = 0,
		KRLM_PRIMIUM_GCCLUB,
		KRLM_PRIMIUM_GCBLOG,
		KRLM_PRIMIUM_CARNIVAL,
		KRLM_PRIMIUM_GOLDEN_TICKET,
		KRLM_PRIMIUM_GCCLUB_PREMIUM = 16
	};

	enum EFILTER_CATEGORY
	{
		ECATEGORY_ALL = 0,          // 전체모드
		ECATEGORY_MATCH,            // 대전
		ECATEGORY_TAGMATCH,         // 태그매치
		ECATEGORY_MONSTERCRUSADER,  // 몬스터원정대 (보스러쉬)
		ECATEGORY_MONSTERHUNT,      // 몬스터대전
		ECATEGORY_DEATHMATCH,       // 데스매치
		ECATEGORY_GUILD,            // 유저가 속한 길드방
		ECATEGORY_ANGLE_EGG,        // 천사 알깨기      
		ECATEGORY_DOTA,             // 점령전
        ECATEGORY_FATAL_DEATHMATCH, // 페이탈 데스매치

		NUM_CATEGORY,
	};

	void SetCurrentLevel(int iLevel);
	int GetCurrentLevel( ) { return m_iCurrentLevel; }
	void LoadBossImageFileInfo();
	bool GetBossBigImageInfo( OUT int &iFileNum, OUT int &iPictureNum, OUT bool &bHellMod );
	bool GetBossSmallImageInfo( OUT int &iFileNum, OUT int &iPictureNum, OUT bool &bLock, int iGameMode );
	bool GetRoomBossBigImageInfo( OUT int &iFileNum, OUT int &iPictureNum );
    
    int GetNumMission();
	std::wstring GetLevelText(int iLevel);
	int ConvertPremium(DWORD dwPremium);
	bool IsHellMode();
	bool IsBreakMode();

	int GetCurrentPage(){ return m_iCurrentPage; }
	bool SetCurrentPage( int iPage_ , bool bSendPacket = true);
	int GetMaxPage(){ return m_iMaxPage; }
	void UpdateRoomList( std::vector<KSimpleRoomInfo>& vecRoomList, int iCurrentPage, int iMaxPage );
	std::vector<KSimpleRoomInfo>& GetRoomList(){ return m_vecRoomList; }
	void ClearRoomList();
	void SetWaitRoom( bool bWaitRoom );
	bool GetWaitRoom(){ return m_bWaitRoom; }
	void SendRoomListReq( bool bSendDelay_ = true );
	void SetRoomFilter( int iFilter, bool bDungeon = false );
	int GetRoomFilter(){ return m_iRoomFilter; }
	bool GetFilterSetGameMode() { return m_bRoomDungeon; }
	void SetFilterEnable( EFILTER_CATEGORY eCategory_ );
	void SetFilterDIsable( EFILTER_CATEGORY eCategory_ );
	bool IsFilterEnabled( EFILTER_CATEGORY eCategory_ );
	void ClearRoomFilter();
	EFILTER_CATEGORY GetFilterEnumFromCategory( EGCGameModeCategory eCategory_ );
	std::pair<int,int> GetRecomUser( int iGameMode, int iLevel);
	void SetCreateRoomTitleColor(int iIndex);
	int GetCreateRoomTitleColorIndex( ){ return m_iCreateRoomTitleColor;}
	DWORD GetCreateRoomTitleColor( );
	std::vector<GCITEMID> GetCurrentModeAndLevelRewardItem( );

	void LoadRoomOptionsFile();
	void SaveRoomOptionsFile();

	KRoomOptions GetCreateRoomOptions() { return m_kCreateRoomConfiguration; }
	KRoomOptions GetRoomListFilterOptions() { return m_kListRoomFilters; }

	void SetCreateRoomOptions(KRoomOptions kOptions) { m_kCreateRoomConfiguration = kOptions; }
	void SetRoomListFilterOptions(KRoomOptions kOptions) { m_kListRoomFilters = kOptions; }

	int m_iCreateRoomTitleColor;
private:
	int m_iCurrentLevel;

	//보스이미지 정보<던전 번호,이미지 정보>
	std::map<int, SBossImageInfo> m_mapBossImageInfo; 
	
	//플레이 가능한 던전인지 알아볼 수 있는 정보.
	std::map<std::pair<int,int>, bool> m_mapDungeonPlayInfo;

	//룸리스트 페이지
	int m_iCurrentPage;
	//름 마지막 페이지
	int m_iMaxPage;
	//wait 필터 
	bool m_bWaitRoom;
	int m_iRoomFilter;
	bool m_bRoomDungeon;

	std::vector<KSimpleRoomInfo> m_vecRoomList;
	std::vector<KSimpleRoomInfo> m_vecPreWaitRoomList;
	std::vector<KSimpleRoomInfo> m_vecPreAllRoomList;

	std::set<EFILTER_CATEGORY>  m_setRoomFilter;
	std::map<int, std::map<int,std::pair< int, int >>> m_mapRecomUserNUM;

	DWORD m_dwPreUpdateTime;
	KRoomOptions m_kCreateRoomConfiguration;
	KRoomOptions m_kListRoomFilters;

};

DefSingletonInline( KGCRoomListManager );
