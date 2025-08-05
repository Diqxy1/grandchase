#pragma once

#include "KUIFrame.h"

class KUILayer : public KUIFrame
{

    KDeclareLuaInvoke;

public:
    virtual void Load(TiXmlNode* pkNode);
    virtual void Save(TiXmlNode* pkNode);

    virtual void Render();    
    virtual void Update();

    virtual KUIWidget* CreateObject();

};