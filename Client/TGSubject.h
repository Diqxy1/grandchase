#pragma once
#include "TriggerCondition.h"

class CTGSubject : public CTriggerCondition
{
public:
    CTGSubject(void);
    ~CTGSubject(void);

public:
    void InitData(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

private:
    void LoadSubject();
    int SelectSubject( int iNoSelectID = -1 );

private:
    bool m_bBeginTG;
    std::vector<int> m_vecConCharType;
    std::vector<int> m_vecConLevel;
    std::vector<int> m_vecConJobLevel;
    std::vector<std::pair<int, int>> m_vecSubject;   //pair( 과제ID, 확률 )
};