#pragma once
#include "FunctionManager.h"

class CAreaAssignFunctionManager : public CFunctionManager
{
public:
    CAreaAssignFunctionManager(void);
    ~CAreaAssignFunctionManager(void);

    virtual void SetListener(CWnd *pWnd) { m_pListener = pWnd; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    bool m_bDrag;
    CRect m_rtArea;
    CWnd                            *m_pListener;
    //CLayerPart* m_pAreaPart;
};
