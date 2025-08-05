#pragma once

#include "KUIFrame.h"
#include "KUITexture.h"
#include "KUIFontString.h"

//NiSmartPointer(KUIComboElement);

class KUIComboElement : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

    void SetText(std::string strText);
    std::string GetText();
    const char* GetText_();

protected:
    KUITexture* m_spBack;
    KUIFontString* m_spText;

};