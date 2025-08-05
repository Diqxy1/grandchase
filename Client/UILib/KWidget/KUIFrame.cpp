// Precompile Header
#include "stdafx.h"

#include "KUIFrame.h"
#include "KUIButton.h"
#include "KUIManager.h"
#include "KUIFontString.h"
#include "KUIEditBox.h"
#include "KUITexture.h"
#include "KUIBackDrop.h"
#include "KUICheckButton.h"
#include "KUISlider.h"
#include "KUIScrollFrame.h"
#include "KUIScrollingMessageFrame.h"
#include "KUIComboBox.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIFrame, KUIWidget);

//---------------------------------------------------------------------------
void KUIFrame::RegisterLuabind()
{
    lua_tinker::class_<KUIFrame>("KUIFrame")
        .inh<KUIWidget>()
        ;
}
//---------------------------------------------------------------------------
void KUIFrame::Load(TiXmlNode* pkNode)
{
    KUIWidget::Load(pkNode);

    CreateWidgets(pkNode);

    // Frame은 따로 생성
    KUIWidget::Creates<KUIFrame>(this, pkNode, "Frame");
}
//---------------------------------------------------------------------------
void KUIFrame::CreateWidgets(TiXmlNode* pkNode)
{
    KUIWidget::Creates<KUIBackDrop>(this, pkNode, "BackDrop");
    KUIWidget::Creates<KUITexture>(this, pkNode, "Texture");
    KUIWidget::Creates<KUIButton>(this, pkNode, "Button");
    KUIWidget::Creates<KUIFontString>(this, pkNode, "FontString");
    KUIWidget::Creates<KUIEditBox>(this, pkNode, "EditBox");
    KUIWidget::Creates<KUICheckButton>(this, pkNode, "CheckButton");
    KUIWidget::Creates<KUISlider>(this, pkNode, "Slider");
    KUIWidget::Creates<KUIScrollFrame>(this, pkNode, "ScrollFrame");
    KUIWidget::Creates<KUIScrollingMessageFrame>(this, pkNode, "ScrollingMessageFrame");
    KUIWidget::Creates<KUIComboBox>(this, pkNode, "ComboBox");
}
//---------------------------------------------------------------------------
void KUIFrame::Render()
{
    if (!m_bVisible)
        return;

    KUIWidget::Render();

    unsigned int iW;
    for (iW = 0; iW < GetWidgetCount(); ++iW)
    {
        KUIWidget* pkWidget = m_vecWidget[iW];
        pkWidget->Render();
    }
}
//---------------------------------------------------------------------------
void KUIFrame::Update()
{
    if (!m_bVisible)
        return;

    KUIWidget::Update();

    unsigned int iW;
    for (iW = 0; iW < GetWidgetCount(); ++iW)
    {
        KUIWidget* pkWidget = m_vecWidget[iW];
        pkWidget->Update();
    }
}
//---------------------------------------------------------------------------
bool KUIFrame::UpdateInput()
{
    if (!m_bVisible)
        return false;

    unsigned int iW;
    for (iW = 0; iW < GetWidgetCount(); ++iW)
    {
        KUIWidget* pkWidget = m_vecWidget[iW];
        if (pkWidget->UpdateInput())
            break;
    }

    return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUIFrame::CreateObject()
{
    return new KUIFrame;
}
//---------------------------------------------------------------------------
void KUIFrame::CopyMember(KUIWidget* pkWidget)
{
    KUIWidget::CopyMember(pkWidget);

    KUIFrame* pkFrame = (KUIFrame*) pkWidget;

    // Widget
    std::vector<KUIWidgetPtr>& vecWidget = pkFrame->m_vecWidget;
    unsigned int iW;
    for (iW = 0; iW < vecWidget.size(); ++iW)
    {
        KUIWidget* pkChild = vecWidget[iW];
        KUIWidget* pkNewChild = pkChild->Clone(this);

        //m_vecWidget.push_back(pkNewChild);
        //pkNewChild->AttachParent(this);
        pkNewChild->SetExtName(pkChild->GetExtName());
    }
}
//---------------------------------------------------------------------------
void KUIFrame::AttachWidget(KUIWidget* pkWidget)
{
    m_vecWidget.push_back(pkWidget);

    pkWidget->AttachParent(this);
}
//---------------------------------------------------------------------------
void KUIFrame::DetachWidgetAt(unsigned int uiIdx)
{
    KUIWidgetVector::iterator iW = m_vecWidget.begin();
    std::advance(iW, uiIdx);

    (*iW)->DetachParent();
    m_vecWidget.erase(iW);
}
//---------------------------------------------------------------------------
void KUIFrame::DetachWidget(KUIWidget* pkWidget)
{
    KUIWidgetVector::iterator iW = 
        std::find(m_vecWidget.begin(), m_vecWidget.end(), pkWidget);

    if (iW != m_vecWidget.end())
        m_vecWidget.erase(iW);
}
//---------------------------------------------------------------------------
void KUIFrame::DetachAllWidgets()
{
    unsigned int iW;
    for (iW = 0; iW < GetWidgetCount(); ++iW)
    {
        m_vecWidget[iW]->DetachParent();
    }

    m_vecWidget.clear();
}
//---------------------------------------------------------------------------
unsigned int KUIFrame::GetWidgetCount()
{
    return (int)m_vecWidget.size();
}
//---------------------------------------------------------------------------
KUIWidget* KUIFrame::GetWidgetAt(unsigned int uiIdx)
{
    return m_vecWidget[uiIdx];
}
//---------------------------------------------------------------------------
KUIWidget* KUIFrame::GetWidgetByName(std::string strName)
{
    KUIWidget* pkWidget = NULL;
    KApplyDownWidget(this, KGetWidgetByName(pkWidget, strName));

    return pkWidget;
}
//---------------------------------------------------------------------------
KUIWidget* KUIFrame::FindWidgetByExtName(std::string strName)
{
    m_vecResWidget.clear();

    unsigned int iW;
    for (iW = 0; iW < GetWidgetCount(); ++iW)
    {
        KUIWidget* pkWidget = m_vecWidget[iW];
        if (strName == pkWidget->GetExtName())
        {
            m_vecResWidget.push_back(pkWidget);
        }
    }

    if (m_vecResWidget.empty())
        return NULL;

    return m_vecResWidget[0];
}
//---------------------------------------------------------------------------
int KUIFrame::GetResWidgetCount()
{
    return (int)m_vecResWidget.size();
}
//---------------------------------------------------------------------------
KUIWidget* KUIFrame::GetResWidgetAt(unsigned int uiIdx)
{
    return m_vecResWidget[uiIdx];
}
//---------------------------------------------------------------------------