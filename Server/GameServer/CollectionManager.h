#pragma once
#include "UserPacket.h"
#include "MissionInfo.h"
#include <string>
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <boost/random.hpp>
#include "GameDrop.h"


class KLuaManager;
class KUser;
class KCollectionManager
{
    DeclareSingleton( KCollectionManager );
    NiDeclareRootRTTI( KCollectionManager );
    DeclToStringW;

public:
    KCollectionManager(void); //생성자
    ~KCollectionManager(void);//소멸자
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    //void ClearAll();

    //bool IsInMission( DWORD dwMissionID )
    //{

    //}
    bool GetMissionInfo( OUT KMissionInfo& kMissionInfo, IN const DWORD dwMissionID ); // 미션 정보 얻기, 미션데이터랑 무슨 차이더라
    bool GetSubMission( OUT KSubMission& kSubMission, IN const int nSubMissionID );    // 서브미션 얻기
    bool GetCondition( IN const int nConditionID, OUT sCondition& condition ); // 미션 컨디션(조건?) 얻기

    bool LoadCondition( IN KLuaManager& kLuaMng, OUT std::map< int, sCondition >& mapConditions );
    bool LoadSubMission( IN KLuaManager& kLuaMng, OUT std::map< int, KSubMission >& mapSubmission );
    bool LoadMission( IN KLuaManager& kLuaMng, OUT std::map< DWORD, KMissionInfo >& mapMissionInfo );

    bool CheckMissionComplete( IN const KCollectionData& kMissionData,                             //EVENT_COMPLETE_MISSION_REQ 에서 부름
        IN KItemContainer& kInventory ); // 미션 완료 체크

    void DropSubMission( IN KUser& kUser,
        IN KRoom& kRoom,
        IN const KGameResultIn& gameResult,
        IN const KEndGameReq& kReq,
        OUT  std::vector<KDropItemInfo>& vecdrops );

    bool GetMissionData( OUT KCollectionData& kMissionData, IN const DWORD dwMissionID, IN bool bFillCount = true  ); // 미션 데이터 얻기, 미션데이터는 패킷용.

    void GetMissionIDList( OUT std::vector<DWORD>& vecMissionIDList );

    bool LoadTitleList( IN KLuaManager& kLuaMng, OUT std::vector<DWORD>& vecTitleList );
    bool LoadTitleOrder( IN KLuaManager& kLuaMng, OUT std::map< GCITEMID, GCITEMID >& mapTitleOrder );

    void GetTitleList( OUT std::vector<DWORD>& vecTitleList );
    bool IsTitleItem( const GCITEMID ItemID_ );
    std::wstring HowGetTitle( IN std::wstring strKey ); // 칭호 드랍, 미션 완료시에 호출됨, 어떻게 얻었는지 표시해주는 메세지 구성에 필요

    void CheckGetPrevTitle( IN const GCITEMID dwItemID, OUT std::vector<GCITEMID>& vecPrevCTList );
    bool GetBroadcastCheck() { KLocker lock( m_csMissions ); return m_bBroadcastCheck; }

    float GetRatio();
    DWORD GetRandomNum() { return die(); };

protected:
    bool                        m_bBroadcastCheck;      // 칭호획득시 공지여부
    std::vector<DWORD>          m_vecTitleCollection;   // 칭호Item 리스트
    std::map<GCITEMID,GCITEMID> m_mapTitleOrder;        // 칭호획득 우선조건(획득할 타이틀,선행조건 타이틀)

    boost::lagged_fibonacci607 rng;
    boost::uniform_int<DWORD> uint32;
    boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_int<DWORD> > die;

    mutable KncCriticalSection          m_csMissions;
    std::map< int, sCondition >         m_mapConditions;
    std::map< int, KSubMission >        m_mapSubmission;
    std::map< DWORD, KMissionInfo >     m_mapMissionInfo;

};
DefSingletonInline( KCollectionManager );
DeclOstmOperatorA( KCollectionManager );