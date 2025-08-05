#pragma once
#include "Trigger.h"
//#include <string>

class CTriggerManager
{
public:
    CTriggerManager(void);
    ~CTriggerManager(void);

    bool                            m_bActive;
    bool                            m_bComplete;
    std::vector<CTrigger*>          m_vtTriggerList;

public:
    void StartTrigger(void);
    void FrameMove(void);
    bool CheckWithConditionForStart(CTriggerCondition *pCondition);

    bool IsComplete(void) { return m_bComplete; }
    bool IsActive(void) { return m_bActive; }
    void SetActive(bool bActive){ m_bActive = bActive; }

    CTrigger *FindTrigger(std::string &strTGName);
    CTriggerCondition *FindTriggerConditonByID(const int iTG_ID);
    
    void AddTrigger(CTrigger *pTrigger);
    void ClearTrigger(void);

    void GetTriggerConditionByType(CTrigger::enTriggerCondition enTG_Type, std::vector<CTriggerCondition*> &vtTGCondition);
    int GetCntOfTriggerType(CTrigger::enTriggerCondition enTG_Type);
};
