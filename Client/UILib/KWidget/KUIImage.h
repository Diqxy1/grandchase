#pragma once

#include "KUIWidget.h"
#include "KUITexture.h"
//#include "KLuabinder.h"

//NiSmartPointer(KUIImage);

class KUIFrame;
class TiXmlNode;

class KUIImage : public KUIWidget
{

    //NiDeclareRTTI;

public:
    static void RegisterLuabind();
    static KUIImage* CreateDef(char* strFilename);

    virtual void Load(KUIFrame* pkUIFrame, TiXmlNode* pkNode);
    virtual void Save(TiXmlNode* pkNode);

    virtual bool UpdateInput();
    virtual void Update();
    virtual void Render();
    //virtual void Move(NiPoint2 kPos);

protected:
    KUITexture* m_spBackground;

};