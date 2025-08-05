#pragma once
#include "StdAfx.h"
#include "ObjectGenerater.h"
#include "DlgTGObjectGenerater.h"
#include "MapSystem/QSFantasticMap.h"
#include "MapSystem/KQSLayerManager.h"
#include "ObjectManager.h"

//#include <vector>

class CdlgTriggerContent;
class CObjectGenerater;

class CDecorObjectManager : public CObjectManager
{
public:
    CDecorObjectManager(void);
    virtual ~CDecorObjectManager(void);

    void SetDlgTGContent(CdlgTriggerContent *pDlgTGContent) { m_pDlgTGContent = pDlgTGContent; }
    void SetDecorList(std::vector<CObjectGenerater*> *pvtObject) { m_pvtObject = pvtObject; }

    virtual void FrameMoveMode(void);
    virtual void DrawMode(void);
    virtual void DrawSelectedObject(void);
    virtual void MouseMoveMode(D3DVECTOR pos);
    virtual void MouseLBDownMode(D3DVECTOR pos);
    virtual void MouseLBUpMode(D3DVECTOR pos);

protected:
    bool                            m_bDrag;
    CObjectGenerater                *m_pSelectedItem;
    CdlgTriggerContent              *m_pDlgTGContent;
    std::vector<CObjectGenerater*>       *m_pvtObject;
};
