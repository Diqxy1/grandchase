#pragma once
//#include <map>
//#include <vector>
//#include <set>
//#include "knc/UserPacket.h"

typedef std::pair<int, int> CompletionCount;


class KGCUserCollectionMission
{
public:
    KGCUserCollectionMission(void);
    ~KGCUserCollectionMission(void);
    enum{
        UM_OK,
        UM_NOT_ENOUGH,
        UM_MISSION_NOT_EXIST,
        UM_MISSION_ALREADY_EXIST,
    };

    void SetUserCollectionMissionList( const std::vector<KCollectionData>& kList );  //서버에서 받은 미션 
    void SetUserCollectionMission( const KCollectionData& kData );

    void RefreshUserCollectionMission();  //유저의 미션 수행 상황을 다시 알아봅니다.  

    // 미션 등록과 제거
    int CompleteCollectionMission( DWORD dwMissionID );    //수행 중인 미션을 완료 합니다. (완료가 가능할 시 )
    int DeleteCollectionMission( DWORD dwMissionID );    //유저가 미션을 삭제 합니다.
    int AddCollectionMission( DWORD dwMissionID );       //미션을 등록합니다.
    void ClearUserCollectionMission();    //미션들을 모두 지웁니다.
    void ReadyCollectionMissionDelete( DWORD dwMissionID )    { m_dwDeleteReadyMissionID = dwMissionID; }
    DWORD GetDeleteReadyCollectionMission()    { return m_dwDeleteReadyMissionID; }

    DWORD IsInvalidCollectionMission( DWORD dwMissionID );    //현재 유저가 가지고 있는 미션과 중복하는지 체크 해 줍니다.
  
    // 등록된 미션의 정보
    bool GetCharMission( int iCharKind , std::vector< DWORD >& vecList ); //해당 케릭터의 미션 리스트를 받아옵니다.
    bool IsUserCollectionMission( DWORD dwMissionID );
    int GetUserCollectionMissionNum() { return (int)m_mapUserCollectionMission.size(); }
    int GetCharMissionNum( int iCharKind );
    int GetCharCompleteMissionNum( int iCharKind ); 
    int GetCompleteMissionNum();
    int GetCompletionCount( DWORD dwMissionID, int iSubMissionID );
    bool IsCompleteCollectionMission( DWORD dwMissionID );
    bool IsCompleteSubMission( int iSubMissionID, int iCount );
    bool IsFailedCollectionMission( DWORD dwMissionID );
    std::vector< DWORD > GetCharacterCollectionMission( int iCharKind );  // 해당 케릭터가 수행할 수 있는 미션 리스트를 받습니다.

    void GetUnCompleteCollectionMission( std::vector<DWORD>& vecMission );
    std::vector<DWORD> GetCompleteCollectionMission( const std::vector<DWORD>& vecBefore, const std::vector<DWORD>& vecAfter );
    std::vector< DWORD > GetThisTimeCompleteCollectionMission() const { return m_vecCompleteThisTime; }
    void ClearThisTimeCompleteMission() { m_vecCompleteThisTime.clear(); }
    int MissionCharacter( DWORD dwMissionID );

	bool IsCompleteCTMission( DWORD dwMissionID );
	int GetNewCompleteTitleID( void );

protected:
    void RefreshMissionItemCount();
    void RefreshMissionPeriod();

    bool GetInvalidCollectionMissionList( DWORD dwMissionID , std::vector< DWORD >& vecList );

private:
    std::map< DWORD, std::vector< std::pair< int, int > > > m_mapUserCollectionMission;   //map < 미션 아이디, std::vector< std::pair < 서브미션 ID, 수행 횟수 > > >
    DWORD   m_dwDeleteReadyMissionID;
    std::vector< DWORD > m_vecCompleteThisTime;
    std::set< DWORD > m_setCompleteMission;
};
