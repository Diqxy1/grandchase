#pragma once

#include "KUIFrame.h"
#include "KUIComboElement.h"
#include "KUIButton.h"

class KUIComboBox : public KUIFrame
{

    KDeclareLuaInvoke;
    //NiDeclareRTTI;

public:
    KUIComboBox();

    static void RegisterLuabind();

    virtual void Load(TiXmlNode* pkNode);

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);

    void ClearComboElements();

    // CurElement
public:
    void CreateCurElement();
    void UpdateCurElement();
    void SetCurElement(std::string strText);
    void RemoveCurElement();

protected:
    KUIButton* m_spExtendBtn;
    KUIComboElement* m_spCurElement;
    int m_iCurElement;
    
    // Combo Element
public:
    void InsertElement(std::string strText);
    void RemoveElementAt(int iIdx);
    void RemoveAllElements();
    int GetElementCnt();

    void UpdateExtends();
    void ExtendElements();
    void ShrinkElements();
    bool IsExtends();

protected:
    bool m_bExtendDirUp;
    bool m_bExtends;
    std::vector<KUIComboElement*> m_vecElements;
    KUIComboElement* m_spElementSrc;

};