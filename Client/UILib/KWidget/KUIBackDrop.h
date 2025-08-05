#pragma once

#include "KUIFrame.h"

class TiXmlNode;

class KUIBackDrop : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    virtual void Load(TiXmlNode* pkNode);

    void CreateBackground();
    void CreateBack();
    void CreateBorder();
    void CreateBorderRectConer();
    void CreateBorderRectEdge();

    void UpdateBackground();

    virtual void Render();
    virtual void Update();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
    std::string m_strBackFilename;
    std::string m_strBorderFilename;

    float m_fTileSize;
    float m_fEdgeSize;

	GCUITexture*	m_spBackTex;
	GCUITexture*	m_spEdgeTex;

    //NiScreenTexture* m_spBackTex;
    //NiScreenTexture* m_spEdgeTex;

};