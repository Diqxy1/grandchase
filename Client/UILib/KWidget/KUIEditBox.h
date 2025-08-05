#pragma once

#include "KUIFontString.h"

#define UNI_HANGUL 949

//NiSmartPointer(KUIEditBox);

class KUIEditBox : public KUIFontString
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUIEditBox();

    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);
    
    void UpdateCursor();
    
    void ClearBuf();
    const char* GetBuf();
    int GetBufSize();

    void SetComp(const char* pcText);
    void SetBuf(const char* pcText);

protected:
    std::string m_strComp;
    std::string m_strBuf;

    int m_iCodePage;
    char m_cCursor;
    int m_iLetters;

};