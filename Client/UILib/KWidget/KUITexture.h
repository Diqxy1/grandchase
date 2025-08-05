#pragma once

#include "KUIFrame.h"

//NiSmartPointer(KUITexture);

class TiXmlNode;

class KUITexture : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUITexture();

    virtual void Load(TiXmlNode* pkNode);

    virtual void CreateTexture();

    virtual void Render();
    virtual void Update();

    void UpdateCoords();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
    std::string m_strFilename;
    float m_fTexLeft, m_fTexTop;
    GCUITexture* m_spScrTexture;

};