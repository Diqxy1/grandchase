#pragma once
#include "TriggerCondition.h"
////#include <vector>
//#include <string>

class CTrigger : public CTriggerCondition
{
public:
    CTrigger();
    ~CTrigger();

    bool                             m_bUse;             // 트리거 사용 여부
    bool                             m_bActive;          // 활성화 여부
    bool                             m_bStartWithActive; // 초기실행 여부
    std::string                      m_strTGName;        // 트리거의 이름
    std::vector<CTriggerCondition*>  m_vtEvent;          // 트리거 발동과 동시에 시작하는 이벤트함수
    std::vector<CTriggerCondition*>  m_vtCondition;      // Action을 실행할 조건함수
    std::vector<CTriggerCondition*>  m_vtAction;         // 조건 완료시 실행함수

public:
    void StartTrigger(void);                        // 트리거 시작전 초기화
    void EndTrigger(void);                          // 트리거 종료 후 사용
    void EventTrigger(void);                        // 이벤트 파트 실행
    void FrameMove(void);                           // 조건들을 실행
    void ActionTrigger(void);                       // 액션 파트 실행
    void ForceStartWithOutCheck(void);              // 내가 Host가 아닐경우 방장과의 통신을 하지 않고 트리거 실행할때 사용.
    bool CheckWithConditionToStart(CTriggerCondition* pConditon);       // 방장과의 통신으로 받은 함수와 일치하는 함수가 있나?

    void SetActive(bool bActive) { m_bActive = bActive; }
    bool IsActive(void) { return m_bActive; }
    
    void AddEventTrigger(CTriggerCondition *cdt);   // 이벤트 트리거 추가.
    void AddCondition(CTriggerCondition *cdt);      // 조건 추가
    void AddActionTrigger(CTriggerCondition *cdt);  // 이벤트 트리거 추가.

    CTriggerCondition *FindTriggerConditonByID(const int iTG_ID);
    void GetTriggerConditionByType(CTrigger::enTriggerCondition enTG_Type, std::vector<CTriggerCondition*> &vtTGCondition);
    int GetCntOfTriggerType(enTriggerCondition enTG_ID);
};