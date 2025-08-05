#pragma once
#include "StdAfx.h"
#include "BaseTrigger.h"
#include "MapSystem/KQSLayerPart.h"

class CObjectManager
{
public:
    CObjectManager(void);
    virtual ~CObjectManager(void);

    virtual void SetObjectID(CString strObjectID) { m_strCurObject = strObjectID; }
    virtual void SetFont(CD3DFont *pFont) { m_pFont = pFont; }
    virtual bool IsInLayerPart(D3DVECTOR pos, float fLayerPosX, float fLayerPosY, CString strMon);
    virtual void DrawPartDesc(LPDIRECT3DDEVICE9 pd3dDevice, float fLayerPosX, float fLayerPosY, CString strDesc, CString strObject);
    virtual void DrawPartObject(float fLayerPosX, float fLayerPosY, bool bInverse, CString strObject);

    virtual void FrameMoveMode(void) { }
    virtual void DrawMode(void) { }
    virtual void DrawSelectedObject(void) { }
    virtual void MouseMoveMode(D3DVECTOR pos) { }
    virtual void MouseLBDownMode(D3DVECTOR pos) { }
    virtual void MouseLBUpMode(D3DVECTOR pos) { }

protected:
    CString                     m_strCurObject;          // 선택한 Object Enum String
    CPoint                      m_ptDragItem;            // 드래그 중인 아이템의 위치 저장.
    CD3DFont                    *m_pFont;                // Font for drawing text
};
