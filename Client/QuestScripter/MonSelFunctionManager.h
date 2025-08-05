#pragma once
//#include <vector>
#include "FunctionManager.h"

class CMonsterGenerater;

class CMonSelFunctionManager : public CFunctionManager
{
public:
    CMonSelFunctionManager(void);
    ~CMonSelFunctionManager(void);

    virtual void SetMonsterList(std::vector<CMonsterGenerater*> *pvtObject) { m_pvtObject = pvtObject; }
    virtual void SetListener(CWnd *pWnd) { m_pListener = pWnd; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    CWnd                            *m_pListener;
    std::vector<CMonsterGenerater*>      *m_pvtObject;
};
