// Precompile Header
#include "stdafx.h"

#include "KUIComboElement.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIComboElement, KUIFrame);

//---------------------------------------------------------------------------
void KUIComboElement::RegisterLuabind()
{
    lua_tinker::class_<KUIComboElement>("KUIComboElement")
        .inh<KUIFrame>()
        .def("GetText", &KUIComboElement::GetText)   
        ;
}
//---------------------------------------------------------------------------
void KUIComboElement::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    KUITexture* pkBack = NULL;
    pkBack = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "BackTexture");
    if (pkBack)
    {
        DetachWidget(m_spBack);
        m_spBack = pkBack;
        m_spBack->SetExtName("BackTexture");
    }

    KUIFontString* pkText = NULL;
    pkText = KUIWidget::CreateOne<KUIFontString>(
        this, pkNode, "ElementFont");
    if (pkBack)
    {
        DetachWidget(m_spText);
        m_spText = pkText;
        m_spText->SetExtName("ElementFont");
    }
}
//---------------------------------------------------------------------------
void KUIComboElement::Render()    
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
void KUIComboElement::Update()
{
    KUIFrame::Update();
}
//---------------------------------------------------------------------------
bool KUIComboElement::UpdateInput()
{
    if (!m_bInputEnable)
        return false;
/*
    KUIFrame::UpdateInput();

    NiInputMouse* pkMouse = _KAppHolder()->GetInputSystem()->GetMouse();

    if (IsCursorInRect())
    {
        if (pkMouse->ButtonWasReleased(NiInputMouse::NIM_LEFT))
        {
            m_kScript.Execute(KUIScript::eOnBtnDown); 
            return true;
        }
    }
*/
    return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUIComboElement::CreateObject()
{
    return new KUIComboElement;
}
//---------------------------------------------------------------------------
void KUIComboElement::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUIComboElement* pkComboElt = (KUIComboElement*) pkWidget;

    if (pkComboElt->m_spBack)
    {
        m_spBack = (KUITexture*) FindWidgetByExtName("BackTexture");
    }

    if (pkComboElt->m_spText)
    {
        m_spText = (KUIFontString*) FindWidgetByExtName("ElementFont");
    }
}
//---------------------------------------------------------------------------
void KUIComboElement::SetText(std::string strText)
{
    m_spText->SetText(strText);
}
//---------------------------------------------------------------------------
std::string KUIComboElement::GetText()
{
    return m_spText->GetText();
}
//---------------------------------------------------------------------------
const char* KUIComboElement::GetText_()
{
    return m_spText->GetText().c_str();
}
//---------------------------------------------------------------------------
