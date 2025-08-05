#pragma once
#include "TriggerCondition.h"
////#include <vector>
using namespace std;

class CTGNextStage : public CTriggerCondition
{
public:
    CTGNextStage(void);
    ~CTGNextStage(void);

    int             m_iLevel;
    int             m_iDecisionStage;
    vector<int>     m_vtStage;
    vector<int>     m_vtPercent;

    void NextStage(void);
    void FrameMove(void);
    void SetDecisionStage( int iDecisionStage_ ) { m_iDecisionStage = iDecisionStage_; }
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
    virtual bool CheckEqualCondition(CTriggerCondition *pCondition);
};
