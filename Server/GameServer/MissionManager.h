#pragma once
#include <string>
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <boost/random.hpp>
#include "MissionInfo.h"

struct sCondition;
class KSubMission;
class KRoom;
struct sTriggerInfo;
struct KMissionPenaltyInfo;
class KLuaManager;
class KUser;
class KItemContainer;
class KMissionManager
{
    DeclareSingleton( KMissionManager );
    NiDeclareRootRTTI( KMissionManager );
    DeclToStringW;
public:
    
    KMissionManager(void);
    ~KMissionManager(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void ClearAll();

    bool IsInMissions( DWORD dwMissionID )
    {
        KLocker lock( m_csMissions );
        return m_mapMissionInfo.find( dwMissionID ) != m_mapMissionInfo.end();
    };

    bool GetMissionInfo( OUT KMissionInfo& kMissionInfo, IN const DWORD dwMissionID ); // 미션 정보 얻기, 미션데이터랑 무슨 차이더라
    bool GetMissionData( OUT KMissionData& kMissionData, IN const DWORD dwMissionID, IN bool bFillCount = true  ); // 미션 데이터 얻기, 미션데이터는 패킷용.
    bool GetCountSubMissionList( OUT std::vector<int>& vecSubmissions, IN const DWORD dwMissionID ); // 카운트용 서브미션 목록 , 카운트용 미션이란?
    bool CheckMissionComplete( IN KUser& kUser,
                               IN const KMissionData& kMissionData, //EVENT_COMPLETE_MISSION_REQ 에서 부름
                               IN KItemContainer& kInventory,       // 미션 완료 체크
                               OUT std::vector<KItem>& vecDelItem );
    // 드랍서브미션 , 결국 END_GAME_REQ 에서 호출 ( AdditionalDrop 안에서 호출) , 아이템을 주거나 서브미션 카운트 증가
    void DropSubMission( IN KUser& kUser,
                         IN KRoom& kRoom,
                         IN const KGameResultIn& gameResult,
                         IN const KEndGameReq& kReq,
                         OUT std::vector<KDropItemInfo>& vecdrops );

    bool ProcessMiniGame( IN KMiniGameRankInfo& kGamResult, IN OUT std::vector<KMissionData>& vecMissionSlot, IN std::wstring& strLogin );
    bool IsInInvalidMission( IN const std::vector<KMissionData>& vecMissionSlot, IN const DWORD dwMissionID ); // 유효한 미션인지 체크 , 중복미션인지도 체크함
    int GetCount( IN const std::vector<KMissionData>& vecMissions, IN const char cCharacter ); // EVENT_REGIST_MISSION_REQ 에서 호출, 현재 캐릭에 등록된 미션 개수
    void PrintSubMission( int iIndex );
    void PrintMission( int iIndex );
    void GetMissionDropList( IN const KGameInfo& kGameInfo, IN std::vector<sTriggerInfo> vecTriggers,
                             OUT std::vector< KGameDropElement >& vecMissionDrop );
    void DumpUnique();
    void GetMissionBonusExp( IN OUT int& Exp_ );
    void GetMissionBonusGp( IN OUT int& Gp_ );
    void SetMissionBonusExpGp( IN float Exp_ , IN float Gp_ ){ KLocker lock(m_csMissionBonusExpGp); m_fMissionBonusExp = Exp_; m_fMissionBonusGp = Gp_; };

    void GetMissionPenaltyRatio( IN const int nMissionLv, IN const int nCharLv, OUT float& fExpRatio, OUT float& fGPRatio );

    // 실시간 미션처리
    void MonsterKillSubMission( IN KUser& kUser, IN const char& cCharType, IN const int& nStartNum, IN const int& nModeID, IN const int& nDifficulty, IN const int& nMonID, IN const int& nMonLv );
    void PvpKillSubmission( IN KUser& kUser, IN const char& cCharType, IN const int& nStartNum, IN const int& nModeID, IN const int& nMapID );
    void ModeSubmission( IN KUser& kUser, IN const int& nModeID, IN const int& nDifficulty, IN const int& nMapID );

    bool IsEnableDelete( IN const DWORD dwMissionID );  // 삭제 가능 여부
    bool CheckControlMission( IN OUT KUser& kUser, IN const KGameResultIn& kGameResultIn, IN const int& nModeID, IN const int& nDifficulty );

    bool GetMissionInfoListByDungeonID( OUT std::vector< KMissionInfo >& vecMissionInfo, IN const int& nDungeonID );

private:
    bool LoadMissionInfo( OUT std::string& strScript );
    bool LoadCondition( IN KLuaManager& kLuaMng, OUT std::map< int, sCondition >& mapConditions );
    bool LoadSubMission( IN KLuaManager& kLuaMng, OUT std::map< int, KSubMission >& mapSubmission );
    bool LoadMission( IN KLuaManager& kLuaMng, OUT std::map< DWORD, KMissionInfo >& mapMissionInfo );

    bool LoadMissionPenalty( OUT std::string& strScript );
    bool LoadPenaltyInfo( IN KLuaManager& kLuaMng, IN const int nIndex,  OUT KMissionPenaltyInfo& kInfo );

    bool GetSubMission( OUT KSubMission& kSubMission, IN const int nSubMissionID );    // 서브미션 얻기
    void GetGoalCollection( IN const std::vector<KMissionData>& vecMissionSlot, OUT std::map<int,int>& mapGoalCollection ); // 서브미션 완료에 관련된 것 같음
    bool GetCondition( IN const int nConditionID, OUT sCondition& condition ); // 미션 컨디션(조건?) 얻기
    bool IsDuplicateMission( IN const std::vector<DWORD>& vecMissions, IN const DWORD dwMissionID ); // 이름 그대로 중복 미션 체크
    bool IsInInvalidCompleteMission( IN const std::vector<DWORD>& vecMissionSlot, IN const DWORD dwMissionID );
    float GetRatio();
    DWORD GetRandomNum() { return die(); };
    bool CheckCondition( IN sCondition& condition, IN bool bResultWin_, DWORD dwMissionID, DWORD dwSubmissionID, const std::wstring& strName );

    bool GetPenaltyInfo( IN const int nLvDiff, OUT KMissionPenaltyInfo& kInfo );
    bool CheckTypeCondition( IN const std::wstring& strName, IN const DWORD& dwMissionID, IN const DWORD dwSubMissionID, IN const sCondition& condition, IN const KUserControlInfo& kUserControlInfoData, IN std::map<int,int>& mapPositionUseCount );

private:
    boost::lagged_fibonacci607 rng;
    boost::uniform_int<DWORD> uint32;
    boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_int<DWORD> > die;

    mutable KncCriticalSection          m_csMissions;
    std::map< int, sCondition >         m_mapConditions;
    std::map< int, KSubMission >        m_mapSubmission;
    std::map< DWORD, KMissionInfo >     m_mapMissionInfo;

    mutable KncCriticalSection          m_csMissionBonusExpGp;
    float                               m_fMissionBonusExp;
    float                               m_fMissionBonusGp;

    mutable KncCriticalSection          m_csMissionPenalty;
    std::vector< KMissionPenaltyInfo >  m_vecMissionPenalty;

};
DefSingletonInline( KMissionManager );
DeclOstmOperatorA( KMissionManager );
