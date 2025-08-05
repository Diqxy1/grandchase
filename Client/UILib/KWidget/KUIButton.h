#pragma once

#include "KUIFrame.h"
#include "KUITexture.h"
#include "KUIFontString.h"

class KUIFrame;
class TiXmlNode;

//NiSmartPointer(KUIButton);

class KUIButton : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUIButton();

    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);

    void LoadText(TiXmlNode* pkNode);
    virtual void LoadTexture(TiXmlNode* pkNode);

    void SetTextPosition();

    virtual bool UpdateInput();
    virtual void Update();
    virtual void Render();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
    std::string m_strText;
    
    KUITexture* m_spNormalTexture;
    KUITexture* m_spHighlightTexture;
    KUITexture* m_spPushedTexture;
    KUITexture* m_spDisabledTexture;
    KUITexture* m_spCurTexture;

    KUIFontString* m_spNormalText;
    KUIFontString* m_pkCurText;

};