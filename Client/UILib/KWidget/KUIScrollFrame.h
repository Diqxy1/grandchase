#pragma once

#include "KUIFrame.h"
#include "KUIFontString.h"
#include "KUISlider.h"

//NiSmartPointer(KUIScrollFrame);

class KUIScrollFrame : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUIScrollFrame();

    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);
    void CopyText(KUIScrollFrame* pkScroll);

protected:
    KUIFontString* m_spFont;
    KUISlider* m_spSlider;

    int m_iMaxLine;
    int m_iScrLine;

    // Text List
public:
    void InsertText(const char* strText);
    void RemoveTextAt(int iIdx);
    void UpdateText();

protected:
    std::deque<std::string> m_deqText;

};