#pragma once

#include "ObjectManager.h"
//#include <vector>

class CdlgTriggerContent;
class CGateObjectGenerater;

class CGateObjectManager : public CObjectManager
{
public:
    CGateObjectManager(void);
    virtual ~CGateObjectManager(void);

    void SetDlgTGContent(CdlgTriggerContent *pDlgTGContent) { m_pDlgTGContent = pDlgTGContent; }
    void SetGateList(std::vector<CGateObjectGenerater*> *pvtObject) { m_pvtGateObject = pvtObject; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void DrawSelectedObject(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    bool                            m_bDrag;
    CGateObjectGenerater            *m_pSelectedItem;
    CdlgTriggerContent              *m_pDlgTGContent;
    std::vector<CGateObjectGenerater*>   *m_pvtGateObject;
};
