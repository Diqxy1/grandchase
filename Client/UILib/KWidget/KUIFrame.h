#pragma once

#include "KUIWidget.h"

//NiSmartPointer(KUIFrame);

class KUIFrame : public KUIWidget
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    static void RegisterLuabind();

    void CreateWidgets(TiXmlNode* pkNode);

    virtual void Load(TiXmlNode* pkNode);

    virtual void Render();
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

    // Widget Container
    void AttachWidget(KUIWidget* pkWidget);
    void DetachWidgetAt(unsigned int uiIdx);
    void DetachWidget(KUIWidget* pkWidget);
    void DetachAllWidgets();
    unsigned int GetWidgetCount();
    KUIWidget* GetWidgetAt(unsigned int uiIdx);
    KUIWidget* GetWidgetByName(std::string strName);
    KUIWidget* FindWidgetByExtName(std::string strName);
    int GetResWidgetCount();
    KUIWidget* GetResWidgetAt(unsigned int uiIdx);

    // Functors
    class KGetWidgetByName;
    class KIsIncludedFrame;

protected:
    typedef std::vector<KUIWidgetPtr> KUIWidgetVector;
    KUIWidgetVector m_vecWidget;    
    KUIWidgetVector m_vecResWidget;

};

#include "KUIFrame.inl"