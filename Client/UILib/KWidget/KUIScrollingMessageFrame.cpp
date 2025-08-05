// Precompile Header
#include "stdafx.h"

#include "KUIScrollingMessageFrame.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIScrollingMessageFrame, KUIFrame);

//---------------------------------------------------------------------------
void KUIScrollingMessageFrame::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    KUIScrollFrame* pkScroll = NULL;
    pkScroll = KUIWidget::CreateOne<KUIScrollFrame>(
        this, pkNode, "TargetFrame");
    if (pkScroll)
    {
        DetachWidget(m_spScrollFrame);
        m_spScrollFrame = pkScroll;
        m_spScrollFrame->SetExtName("TargetFrame");
    }

    KUIEditBox* pkEditBox = NULL;
    pkEditBox = KUIWidget::CreateOne<KUIEditBox>(
        this, pkNode, "MessageBox");
    if (pkEditBox)
    {
        DetachWidget(m_spChatBox);
        m_spChatBox = pkEditBox;
        m_spChatBox->SetExtName("MessageBox");
    }
}
//---------------------------------------------------------------------------
void KUIScrollingMessageFrame::Render()
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
void KUIScrollingMessageFrame::Update()
{
    KUIFrame::Update();
}
//---------------------------------------------------------------------------
bool KUIScrollingMessageFrame::UpdateInput()
{
    KUIFrame::UpdateInput();

    return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUIScrollingMessageFrame::CreateObject()
{
    return new KUIScrollingMessageFrame;
}
//---------------------------------------------------------------------------
void KUIScrollingMessageFrame::CopyMember(KUIWidget* pkWidget)
{
    KUIWidget::CopyMember(pkWidget);

    KUIScrollingMessageFrame* pkMsgFrame = (KUIScrollingMessageFrame*) pkWidget;

    if (pkMsgFrame->m_spScrollFrame)
    {
        m_spScrollFrame = (KUIScrollFrame*) FindWidgetByExtName("TargetFrame");
    }

    if (pkMsgFrame->m_spChatBox)
    {
        m_spChatBox = (KUIEditBox*) FindWidgetByExtName("MessageBox");
    }
}
//---------------------------------------------------------------------------
