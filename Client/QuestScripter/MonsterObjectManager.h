#pragma once
#include "ObjectManager.h"

//#include <vector>

class CdlgTriggerContent;
class CMonsterGenerater;

class CMonsterObjectManager : public CObjectManager
{
public:
    CMonsterObjectManager(void);
    virtual ~CMonsterObjectManager(void);

    void SetDlgTGContent(CdlgTriggerContent *pDlgTGContent) { m_pDlgTGContent = pDlgTGContent; }
    void SetMonsterList(std::vector<CMonsterGenerater*> *pvtObject) { m_pvtObject = pvtObject; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void DrawSelectedObject(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    bool m_bDrag;
    CMonsterGenerater               *m_pSelectedItem;
    CdlgTriggerContent              *m_pDlgTGContent;
    std::vector<CMonsterGenerater*>      *m_pvtObject;
};
