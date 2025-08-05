#pragma once

class CFunctionManager
{
public:
    CFunctionManager(void);
    ~CFunctionManager(void);

    virtual bool IsInLayerPart(D3DVECTOR pos, float fLayerPosX, float fLayerPosY, CString strMon);

    virtual void FrameMoveMode(void) { }
    virtual void DrawMode(void) { }
    virtual void MouseMoveMode(D3DVECTOR pos) { }
    virtual void MouseLBDownMode(D3DVECTOR pos) { }
    virtual void MouseLBUpMode(D3DVECTOR pos) { }
};
