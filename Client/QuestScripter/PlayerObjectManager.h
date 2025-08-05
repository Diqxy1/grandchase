#pragma once
#include "ObjectManager.h"
//#include <vector>

class CdlgTriggerContent;
class CPlayerPosition;

class CPlayerObjectManager : public CObjectManager
{
public:
    CPlayerObjectManager(void);
    virtual ~CPlayerObjectManager(void);

    void SetDlgTGContent(CdlgTriggerContent *pDlgTGContent) { m_pDlgTGContent = pDlgTGContent; }
    void SetPlayerList(std::vector<CPlayerPosition*> *pvtObject) { m_pvtObject = pvtObject; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void DrawSelectedObject(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    bool                            m_bDrag;
    CPlayerPosition                 *m_pSelectedItem;
    CdlgTriggerContent              *m_pDlgTGContent;
    std::vector<CPlayerPosition*>        *m_pvtObject;
};
