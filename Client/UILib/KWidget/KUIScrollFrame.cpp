// Precompile Header
#include "stdafx.h"

#include "KUIScrollFrame.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIScrollFrame, KUIFrame);

//---------------------------------------------------------------------------
KUIScrollFrame::KUIScrollFrame()
: m_iScrLine(0), m_iMaxLine(30)
{
}
//---------------------------------------------------------------------------
void KUIScrollFrame::RegisterLuabind()
{
    lua_tinker::class_<KUIScrollFrame>("KUIScrollFrame")
        .inh<KUIFrame>()
        .def("InsertText", &KUIScrollFrame::InsertText)
        ;
}
//---------------------------------------------------------------------------
void KUIScrollFrame::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    KUIFontString* pkFont = NULL;
    pkFont = KUIWidget::CreateOne<KUIFontString>(
        this, pkNode, "ScrollString");
    if (pkFont)
    {
        DetachWidget(m_spFont);
        m_spFont = pkFont;
        m_spFont->SetExtName("ScrollString");
    }

    KUISlider* pkSlider = NULL;
    pkSlider = KUIWidget::CreateOne<KUISlider>(
        this, pkNode, "ScrollSlider");
    if (pkSlider)
    {
        DetachWidget(m_spSlider);
        m_spSlider = pkSlider;
        m_spSlider->SetExtName("ScrollSlider");
    }

    TiXmlElement* pkTextElt = pkNode->FirstChildElement("Text");
    while (pkTextElt)
    {
        const char* pcText = pkTextElt->GetText();
        if (pcText)
        {
            InsertText(pcText);
        }

        pkTextElt = pkTextElt->NextSiblingElement("Text");
    }

    TiXmlElement* pkScrollElt = pkNode->ToElement();
    pkScrollElt->Attribute("line", &m_iScrLine);
    //KAssert(m_iScrLine > 0);
}
//---------------------------------------------------------------------------
void KUIScrollFrame::Render()
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
void KUIScrollFrame::Update()
{
    KUIFrame::Update();

    UpdateText();
}
//---------------------------------------------------------------------------
void KUIScrollFrame::UpdateText()
{
    std::ostringstream os;
    int iCurPos = m_spSlider->GetCurValue();
    unsigned int iT = iCurPos;

    while (true)
    {
        if (iT >= iCurPos + m_iScrLine)
            break;

        if (iT >= m_deqText.size())
            break;

        os << m_deqText[iT] << std::endl;

        ++iT;
    }

    m_spFont->SetText(os.str());

    int iRangeMax = m_deqText.size() - m_iScrLine;
    if (iRangeMax < 0)
        iRangeMax = 0;
    m_spSlider->SetRange(0.f, iRangeMax);
}
//---------------------------------------------------------------------------
bool KUIScrollFrame::UpdateInput()
{
    KUIFrame::UpdateInput();

    return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUIScrollFrame::CreateObject()
{
    return new KUIScrollFrame;
}
//---------------------------------------------------------------------------
void KUIScrollFrame::CopyMember(KUIWidget* pkWidget)
{
    KUIWidget::CopyMember(pkWidget);

    KUIScrollFrame* pkScroll = (KUIScrollFrame*) pkWidget;

    if (pkScroll->m_spFont)
    {
        m_spFont = (KUIFontString*) FindWidgetByExtName("ScrollString");
    }

    if (pkScroll->m_spSlider)
    {
        m_spSlider = (KUISlider*) FindWidgetByExtName("ScrollSlider");
    }
    
    CopyText(pkScroll);
}
//---------------------------------------------------------------------------
void KUIScrollFrame::CopyText(KUIScrollFrame* pkScroll)
{
    unsigned int iT;
    for (iT = 0; iT < pkScroll->m_deqText.size(); ++iT)
    {
        InsertText(pkScroll->m_deqText[iT].c_str());
    }
}
//---------------------------------------------------------------------------
void KUIScrollFrame::InsertText(const char* strText)
{
    if (m_deqText.size() >= m_iMaxLine)
        RemoveTextAt(0);

    m_deqText.push_back(strText);
}
//---------------------------------------------------------------------------
void KUIScrollFrame::RemoveTextAt(int iIdx)
{
    std::deque<std::string>::iterator iT = m_deqText.begin();
    std::advance(iT, iIdx);
    m_deqText.erase(iT);
}
//---------------------------------------------------------------------------
