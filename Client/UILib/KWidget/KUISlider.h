#pragma once

#include "KUIFrame.h"
#include "KUITexture.h"
#include "KUIButton.h"

//NiSmartPointer(KUISlider);

class KUISlider : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUISlider();

    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);

    void LoadButton(TiXmlNode* pkNode);
    void LoadTexture(TiXmlNode* pkNode);

    void SetButtonPosition();

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    virtual bool IsCursorInRect();
    bool IsCursorInRectY();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
    KUIButton* m_spBtnUp;
    KUIButton* m_spBtnDown;

    // Thumb
public:
    void UpdateThumb();

    void SetRange(int iMin, int iMax);
    void SetCurValue(int iValue);
    int GetCurValue();
    void MaxCurValue();
    void IncCurValue();
    void DecCurValue();

    float GetMinPos();
    float GetMaxPos();
    float GetCurPos();
    int PosToValue(float fPos);

protected:
    int m_iMinRange;
    int m_iMaxRange;
    int m_iCurValue;
    bool m_bThumbPressed;

    KUITexture* m_spThumbTex;

};