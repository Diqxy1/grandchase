#pragma once
#include "StdAfx.h"
#include "ObjectManager.h"
//#include <vector>

class CdlgTriggerContent;
class CAddDamage;

class CDamageObjectManager : public CObjectManager
{
public:
    CDamageObjectManager(void);
    virtual ~CDamageObjectManager(void);

    void SetDlgTGContent(CdlgTriggerContent *pDlgTGContent) { m_pDlgTGContent = pDlgTGContent; }
    void SetDamageList(std::vector<CAddDamage*> *pvtObject) { m_pvtObject = pvtObject; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void DrawSelectedObject(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    bool                            m_bDrag;
    CAddDamage                      *m_pSelectedItem;
    CdlgTriggerContent              *m_pDlgTGContent;
    std::vector<CAddDamage*>        *m_pvtObject;
};
