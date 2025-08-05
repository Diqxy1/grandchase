#pragma once

#include "KUITexture.h"
//#include "KActorManager.h"

class KUIModel : public KUITexture
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    virtual void Load(TiXmlNode* pkNode);

    virtual void CreateTexture();

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
	/*
    KActorManagerPtr m_spActorMgr;
    NiNodePtr m_spModel;
    NiCameraPtr m_spCamera;
    NiRendererPtr m_spRenderer;
    NiRenderedTexturePtr m_spRenTexture;
    NiRenderTargetGroupPtr m_spRenderTargetGroup;
	*/

};