#pragma once
//#include <map>
//#include <vector>
//#include "knc/UserPacket.h"

typedef std::pair<int, int> CompletionCount;


class KGCUserMission
{
	friend class KGCMissionProgressBox;
public:
    KGCUserMission(void);
    ~KGCUserMission(void);
    enum{
        UM_OK,
        UM_NOT_ENOUGH,
        UM_MISSION_NOT_EXIST,
        UM_MISSION_ALREADY_EXIST,
    };

    void SetUserMissionList( const std::vector<KMissionData>& kList, bool bNewMision = true );  //서버에서 받은 미션 
    void SetUserMission( const KMissionData& kData, bool bNewMision = true );

    void RefreshUserMission();  //유저의 미션 수행 상황을 다시 알아봅니다.  

	void SetAutoMission( const std::vector<KMissionData>& kMission );
	bool IsAutoMissionExist()	{ return !m_vecAutoMission.empty(); }
	void ShowAutoMission();

    // 미션 등록과 제거
    int CompleteMission( DWORD dwMissionID );    //수행 중인 미션을 완료 합니다. (완료가 가능할 시 )
    int DeleteMission( DWORD dwMissionID );    //유저가 미션을 삭제 합니다.
    int AddMission( DWORD dwMissionID );       //미션을 등록합니다.
    void ClearUserMission();    //미션들을 모두 지웁니다.
	void LoadGuideExceptionMssion(std::string strFileName_ );
    void ReadyMissionDelete( DWORD dwMissionID )    { m_dwDeleteReadyMissionID = dwMissionID; }
    DWORD GetDeleteReadyMission()    { return m_dwDeleteReadyMissionID; }

    DWORD IsInvalidMission( DWORD dwMissionID );    //현재 유저가 가지고 있는 미션과 중복하는지 체크 해 줍니다.
  
    // 등록된 미션의 정보
    bool GetCharMission( int iCharKind , std::vector< DWORD >& vecList , bool bCommon = false /*공용미션 포함?*/); //해당 케릭터의 미션 리스트를 받아옵니다.
    bool IsUserMission( DWORD dwMissionID );
    int GetUserMissionNum() { return (int)m_mapUserMission.size(); }
    int GetCharMissionNum( int iCharKind );
    int GetCharCompleteMissionNum( int iCharKind ); 
    int GetCompleteMissionNum();
    int GetCompleteMissionNum(int iCharType);
    int GetCompletionCount( DWORD dwMissionID, int iSubMissionID );
    bool IsCompleteMission( DWORD dwMissionID );
    bool IsCompleteSubMission( int iSubMissionID, int iCount );
    bool IsFailedMission( DWORD dwMissionID );
    std::vector< DWORD > GetCharacterMission( int iCharKind );  // 해당 케릭터가 수행할 수 있는 미션 리스트를 받습니다.
    std::vector< DWORD > GetCharacterAndCommonMission( int iCharKind );
    std::vector< DWORD > GetTypeFromCharacterMission( int iCharKind, int iTabKind );  // 해당 케릭터에 특정탭에서 수행할 수 있는 미션 리스트를 받습니다.
    int GetTypeFromCharacterMissionNum( int iCharKind, int iTabKind );

    void GetUnCompleteMission( std::vector<DWORD>& vecMission );
    std::vector<DWORD> GetCompleteMission( const std::vector<DWORD>& vecBefore, const std::vector<DWORD>& vecAfter );
    std::vector< DWORD > GetThisTimeCompleteMission() const { return m_vecCompleteThisTime; }
    void ClearThisTimeCompleteMission() { m_vecCompleteThisTime.clear(); }
    int MissionCharacter( DWORD dwMissionID );
    int MissionType( DWORD dwMissionID );

    bool IsNewMission(int iCharType);
	bool IsNewMission( DWORD dwMissionID );
	bool IsNewMission( void );
	void SetNewMission( DWORD dwMissionID, bool bNew );

	void SetInsertMission( DWORD dwMissionID, std::vector< std::pair< int, int > > &vList, bool bNewMision = true );

public:
	//미션 진행 상황표시를 위해 임시적인 Count
	void InitVirtualMission();//게임 시작할 때 초기화를 위한 함수
	void VirtualKillMonster( int iMonsterID, int iMonLevel = -1, int eNPCType = 0 );
	void VirtualGetMissionItem( DWORD dwItemID );
    void AddVirtualMissionToLevelUp();

private:
	std::vector< std::pair< int, int > > m_vecVirtualCount;
    std::vector< DWORD >  m_vecVirtualCountMainMissionID; //m_vecVirtualCount에 들어가있는 서브미션의 메인미션ID정보

public:
    // 게임중 표시해줄 진행 상황을 위한 체크리스트
    void SetCheck( DWORD dwMissionID, bool bOnOff = true );
    bool GetCheck( DWORD dwMissionID );
	void RefreshMissionItemCount();

protected:
    void RefreshMissionPeriod();

    bool GetInvalidMissionList( DWORD dwMissionID , std::vector< DWORD >& vecList, bool bNewMission = true );

private:
    std::map< DWORD, std::vector< std::pair< int, int > > > m_mapUserMission;   //map < 미션 아이디, std::vector< std::pair < 서브미션 ID, 수행 횟수 > > >
	std::map< DWORD, bool > m_mapUserNewMission;   //map < 미션 아이디, 신규미션 여부 >
	std::vector<KMissionData> m_vecAutoMission;	//자동 드랍 미션이 떨어지는 순간 저장된다. 미션 다이얼로그를 띄워도 되는 사황이 되면 비우면서 띄워준다.
	std::vector<DWORD> m_vecGuildExceptionMissionID;	// 월드맵에 가이드 안해주는 미션 목록
    std::set< DWORD > m_setCheckedMission;  //set< 미션 아이디 >    
    DWORD   m_dwDeleteReadyMissionID;
    std::vector< DWORD > m_vecCompleteThisTime;

	//미션 패키지 작업
	std::set< int > m_setPackageMission;
	std::set< int > m_setMissionPackageItem;

    bool m_bShowMissionUI;

public:
	void SetPackageMission( std::map< int, std::set< int> >& _mapPackageMission);
	bool IsPackageMission( DWORD _dwMissionID ); //패키지를 통해서 등록된 미션인지..
	bool IsMissionPackageItem( int _iItemID ); //미션 패키지 아이템인지

    bool IsMissionDungeon( EGCGameMode eGameMode );
    bool IsMissionMonster( EGCMonster eMonster, int iMonLevel = -1 );
    std::vector<DWORD> GetDungeonMissionList( EGCGameMode eGameMode, int iDunLevel = -1, bool bComplate = true );

public:
    std::vector< std::pair< int, int > > GetVirtualCount() { return m_vecVirtualCount; }
    DWORD GetVirtualCountMainMissionID( int index );

    void SetShowMissionUI( bool bShow ) { m_bShowMissionUI = bShow; }
    bool GetShowMissionUI() { return m_bShowMissionUI; }
};
