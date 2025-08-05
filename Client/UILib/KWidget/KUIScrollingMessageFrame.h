#pragma once

#include "KUIFrame.h"
#include "KUIScrollFrame.h"
#include "KUIEditBox.h"

class KUIScrollingMessageFrame : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    virtual void Load(TiXmlNode* pkNode);

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

protected:
    KUIScrollFrame* m_spScrollFrame;
    KUIEditBox* m_spChatBox;

};