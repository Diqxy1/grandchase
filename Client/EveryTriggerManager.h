#pragma once
#include "Trigger.h"
//#include "TriggerManager.h"
#include "StructOfQuest.h"
//#include <map>
//#include <string>
using namespace std;

class CTriggerManager;
class CTriggerCondition;

class CEveryTriggerManager
{
public:
    CEveryTriggerManager(void);
    ~CEveryTriggerManager(void);

protected:
    // 스테이지 넘어가는 처리용
    bool                                    m_bLoadingScreen;         // 다음스테이지로 넘어갈 때 기본 로딩 스크린을 띄워줄 여부
    int                                     m_iStartDelay;            // 다음 스테이지로 넘어가기 까지 딜레이 시간 시작
    int                                     m_iNextStageDelay;        // 다음 스테이지로 넘어가기 까지 딜레이 시간 끝
    
    // 스테이지 관리용
    TileMap                                 m_ActiveTileMap;          // 진행중인 스테이지 타일맵
    TileMap                                 m_NextTileMap;            // 다음 스테이지 타일맵
	bool									m_bNeedLoading;

    // 트리거 관리용
    vector<CTrigger*>                       m_vtActiveTrigger;        // 사용하고 있는 트리거
    vector<CTriggerManager*>                m_vtTriggerManager;       // 전체 트리거의 정보를 가지고있다.
    vector<CTriggerCondition*>              m_vtConditionToStart;     // Host에서 받은 실행 대기중인 함수 정보를 가진다.

public:
    void Init(void);
	bool MovetoNextStage(void);
    void FrameMove(void);
    void ClearEveryTrigger(void);                                       // 트리거의 정보들을 지운다.
    bool IsExistenceTriggerMgr(int iIndex) const;                       // 존재하는 트리거 매니저 인덱스인가?

    void AddTriggerMgr(CTriggerManager *pTriggerMgr);                   // 트리거매니저 클래스 추가 (Stage 추가)
    void AddActiveTrigger(CTrigger *pTrigger);                          // 액티브트리거 클래스 추가
    void AddCheckWIthConditionForStart(CTriggerCondition *pConditon);   // Host에서 받은 실행 대기중인 함수 정보를 저장한다.

    CTrigger *FindTrigger(std::string &strTGName) const;                      // strTGName 이름을 가진 트리거를 찾는다.
    CTrigger *FindActiveTrigger(std::string &strTGName) const;                // strTGName 이름을 가진 액티브트리거를 찾는다.
    CTriggerCondition *FindTriggerConditonByID(const int iTG_ID) const;       // 트리거 아이디로 트리거 컨디션 리턴
    CTriggerCondition *FindActiveTriggerConditonByID(const int iTG_ID) const; // 트리거 아이디로 트리거 컨디션 리턴

    int  GetCntOfTriggerType(int iStage, CTrigger::enTriggerCondition enTG_Type) const;
    void GetTriggerConditionByType(int iStage, CTrigger::enTriggerCondition enTG_Type, std::vector<CTriggerCondition*> &vtTGCondition);

    int GetTotalStageCnt(void) const { return (int)m_vtTriggerManager.size(); }
    int GetRemainNextDelay(void) const;

    int  GetActiveStage() const { return m_ActiveTileMap.iStage; }
    TileMap GetNextTileMap(void) const { return m_NextTileMap; }
    TileMap GetActiveTileMap(void) const { return m_ActiveTileMap; }
    CTriggerManager* GetCurrentStageTriggerManager() const;
    CTriggerManager* GetNextStageTriggerManager() const;
    void SetNextStage(TileMap tilemap, bool bLoadingScreen = true, int iNextStageTime = 0);     // 다음 스테이지를 설정한다.

    // 트리거 정보가 맞지 않을 경우 사용되는 함수들
    void SetNotMatchTrigger(bool bNotMatch, bool bNoSendReport = false);
    bool FrameMove_NotMatchTrigger(void);

protected:
    void SetActiveStage(TileMap tilemap);                       // 스테이지를 변경한다.
    bool                                m_bNotMatchTrigger;     // 트리거 정보가 맞지 않아 게임을 종료 시켜야할 경우 쓰임.
    DWORD                               m_dwNotMatchEndTime;    // 트리거 정보가 맞지 않아 게임을 종료 시켜야할 경우 시간을 재는 타이머

};

extern CEveryTriggerManager g_kEveryTriggerManager;
