#pragma once

#include "KUIFrame.h"
//#include "KFont.h"

class KUIFrame;
class TiXmlNode;

//NiSmartPointer(KUIFontString);

class KUIFontString : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUIFontString();

    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);

    void LoadColor(TiXmlNode* pkNode);
    void CreateString2D();

    virtual void Update();
    virtual void Render();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

    void SetText(std::string strText);
    std::string GetText();

    void SetArrangeMode(int iArrangeMode);

protected:
    std::string m_strText;
    std::string m_strFontName;

    int m_iFontWidth;
    int m_iFontHeight;
    D3DXCOLOR m_kColor;
    //KFont* m_spString2D;

};