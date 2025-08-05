#pragma once

#include "KUIButton.h"

class TiXmlNode;

class KUICheckButton : public KUIButton
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    enum ECheckState
    {
        eChecked,
        eUnChecked,
        eDisabled,
        eCheckStateMax
    };

public:
    KUICheckButton();

    virtual void Load(TiXmlNode* pkNode);
    virtual void LoadTexture(TiXmlNode* pkNode);

    virtual bool UpdateInput();
    virtual void Update();
    virtual void Render();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
    KUITexture* m_spCheckTex;
    KUITexture* m_spDisCheckTex;
    KUITexture* m_pkCurCheckTex;

public:
    void SetCheckState(int iCheckState);
    int GetCheckState();

    // Checked
protected:
    int m_iCheckState;

};