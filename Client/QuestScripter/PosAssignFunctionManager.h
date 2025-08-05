#pragma once
#include "FunctionManager.h"
#include "stdafx.h"

class CPosAssignFunctionManager : public CFunctionManager
{
public:
    CPosAssignFunctionManager(void);
    ~CPosAssignFunctionManager(void);

    void SetListener(CWnd *pWnd) { m_pListener = pWnd; }
    void DrawMode(void);
    void MouseLBUpMode(D3DVECTOR pos);
    void MouseMoveMode(D3DVECTOR pos);
    void MouseLBDownMode(D3DVECTOR pos);

    void SetHeight(int iHeight) { m_iHeight = iHeight; }

protected:
    bool                            m_bDrag;
    int                             m_iHeight;
    CWnd                            *m_pListener;
};
