// Precompile Header
#include "stdafx.h"

#include "KUIComboBox.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIComboBox, KUIFrame);

//---------------------------------------------------------------------------
KUIComboBox::KUIComboBox()
: m_iCurElement(0), m_bExtends(false), m_bExtendDirUp(false)
{
}
//---------------------------------------------------------------------------
void KUIComboBox::RegisterLuabind()
{
    lua_tinker::class_<KUIComboBox>("KUIComboBox")
        .inh<KUIFrame>()
        .def("ExtendElements", &KUIComboBox::ExtendElements)
        .def("ShrinkElements", &KUIComboBox::ShrinkElements)
        .def("IsExtends", &KUIComboBox::IsExtends)
        .def("SetCurElement", &KUIComboBox::SetCurElement)        
        ;
}
//---------------------------------------------------------------------------
void KUIComboBox::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    TiXmlNode* pkEltNode = pkNode->FirstChild("ComboElement");
    if (pkEltNode)
    {
        KUIComboElement* pkElementSrc = new KUIComboElement;
        pkElementSrc->Load(pkEltNode);
        m_spElementSrc = pkElementSrc;
    }

    CreateCurElement();
    
    KUIButton* pkExtendBtn = NULL;
    pkExtendBtn = KUIWidget::CreateOne<KUIButton>(
        this, pkNode, "ExtendButton");
    if (pkExtendBtn)
    {
        DetachWidget(m_spExtendBtn);
        m_spExtendBtn = pkExtendBtn;
        m_spExtendBtn->SetExtName("ExtendButton");
    }

    TiXmlElement* pkItemElt = pkNode->FirstChildElement("Item");
    while (pkItemElt)
    {
        const char* pcText = pkItemElt->GetText();
        if (pcText)
        {
            InsertElement(pcText);
        }

        pkItemElt = pkItemElt->NextSiblingElement("Item");
    }

    UpdateExtends();
}
//---------------------------------------------------------------------------
void KUIComboBox::CreateCurElement()
{
    KUIComboElement* pkElement = 
        (KUIComboElement*) m_spElementSrc->Clone(this);
    DetachWidget(m_spCurElement);
    m_spCurElement = pkElement;
    m_spCurElement->LockInput();
    m_spCurElement->SetExtName("ComboElement");
    //AttachWidget(m_spCurElement);
}
//---------------------------------------------------------------------------
void KUIComboBox::ClearComboElements()
{
    FindWidgetByExtName("ComboElement");
    
    unsigned int iW;
    for (iW = 0; iW < GetResWidgetCount(); ++iW)
    {
        KUIWidget* pkWidget = GetResWidgetAt(iW);
        DetachWidget(pkWidget);
    }    
}
//---------------------------------------------------------------------------
void KUIComboBox::Render()    
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
void KUIComboBox::Update()
{
    KUIFrame::Update();

    UpdateCurElement();
}
//---------------------------------------------------------------------------
void KUIComboBox::SetCurElement(std::string strText)
{
    unsigned int iE;
    for (iE = 0; iE < m_vecElements.size(); ++iE)
    {
        if (m_vecElements[iE]->GetText() == strText)
        {
            m_iCurElement = iE;
            break;
        }
    }
}
//---------------------------------------------------------------------------
void KUIComboBox::UpdateCurElement()
{
    if (m_iCurElement >= GetElementCnt())
        return;
    
    m_spCurElement->SetText(
        m_vecElements[m_iCurElement]->GetText());
}
//---------------------------------------------------------------------------
bool KUIComboBox::UpdateInput()
{
    KUIFrame::UpdateInput();

    return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUIComboBox::CreateObject()
{
    return new KUIComboBox;
}
//---------------------------------------------------------------------------
void KUIComboBox::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUIComboBox* pkComboBox = (KUIComboBox*) pkWidget;

    if (pkComboBox->m_spElementSrc)
    {
        m_spElementSrc = (KUIComboElement*) 
            pkComboBox->m_spElementSrc->Clone(this);
    }

    if (pkComboBox->m_spExtendBtn)
    {
        m_spExtendBtn = (KUIButton*) FindWidgetByExtName("ExtendButton");
    }

    ClearComboElements();
    m_spCurElement = NULL;
    RemoveAllElements();

    UpdateExtends();
}
//---------------------------------------------------------------------------
void KUIComboBox::InsertElement(std::string strText)
{
    float fDir = 1.f;
    if (m_bExtendDirUp)
        fDir = -1.f;

    KUIComboElement* pkElement = 
        (KUIComboElement*) m_spElementSrc->Clone(this);
    pkElement->SetText(strText);
    pkElement->SetLocalPos(fDir * D3DXVECTOR2(0.f, 
        pkElement->GetSize().y * (GetElementCnt() + 1)));
    pkElement->SetExtName("ComboElement");

    m_vecElements.push_back(pkElement);
}
//---------------------------------------------------------------------------
void KUIComboBox::RemoveElementAt(int iIdx)
{
    std::vector<KUIComboElement*>::iterator iE;
    iE = m_vecElements.begin();
    std::advance(iE, iIdx);

    m_vecElements.erase(iE);
}
//---------------------------------------------------------------------------
void KUIComboBox::RemoveAllElements()
{
    m_vecElements.clear();
}
//---------------------------------------------------------------------------
int KUIComboBox::GetElementCnt()
{
    return m_vecElements.size();
}
//---------------------------------------------------------------------------
void KUIComboBox::ExtendElements()
{
    m_bExtends = true;

    UpdateExtends();
}
//---------------------------------------------------------------------------
void KUIComboBox::ShrinkElements()
{
    m_bExtends = false;

    UpdateExtends();
}
//---------------------------------------------------------------------------
void KUIComboBox::UpdateExtends()
{
    unsigned int iE;

    if (m_bExtends)
    {
        for (iE = 0; iE < m_vecElements.size(); ++iE)
        {
            m_vecElements[iE]->Show();
        }
    }
    else
    {   
        for (iE = 0; iE < m_vecElements.size(); ++iE)
        {
            m_vecElements[iE]->Hide();
        }
    }
}
//---------------------------------------------------------------------------
bool KUIComboBox::IsExtends()
{
    return m_bExtends;
}
//---------------------------------------------------------------------------
