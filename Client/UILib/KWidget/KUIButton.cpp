// Precompile Header
#include "stdafx.h"

#include "KUIManager.h"
#include "KUIButton.h"
//#include "KLuabinder.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIButton, KUIFrame);

//---------------------------------------------------------------------------
KUIButton::KUIButton()
: m_spCurTexture(NULL), m_pkCurText(NULL)
{
	m_spNormalTexture = NULL;
	m_spHighlightTexture = NULL;	
	m_spPushedTexture = NULL;	
	m_spDisabledTexture = NULL;
}
//---------------------------------------------------------------------------
void KUIButton::RegisterLuabind()
{
    lua_tinker::class_<KUIButton>("KUIButton")
        .inh<KUIFrame>()
        ;
}
//---------------------------------------------------------------------------
void KUIButton::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    LoadTexture(pkNode);

    LoadText(pkNode);

    //SetTextPosition();
}
//---------------------------------------------------------------------------
void KUIButton::LoadText(TiXmlNode* pkNode)
{
    TiXmlElement* pkBtnElt = pkNode->ToElement();
    const char* pcText = pkBtnElt->Attribute("text");
    if (pcText)
        m_strText = pkBtnElt->Attribute("text");

    KUIFontString* pkFont = NULL;
    pkFont = KUIWidget::CreateOne<KUIFontString>(this, pkNode, "NormalText");
    if (pkFont)
    {
        DetachWidget(m_spNormalText);
        m_spNormalText = pkFont;
        m_spNormalText->SetExtName("NormalText");
    }

    m_pkCurText = m_spNormalText;
}
//---------------------------------------------------------------------------
void KUIButton::SetTextPosition()
{
    if (m_strText.empty())
        return;

    //float fWidth, fHeight;
    m_spNormalText->SetText(m_strText);
	
    D3DXVECTOR2 kSize = m_spNormalText->GetSize();
    D3DXVECTOR2 kPos = D3DXVECTOR2(	(GetSize().x - kSize.x)/2.f,
									(GetSize().y - kSize.y)/2.f);
	

    m_spNormalText->SetLocalPos(kPos);
}
//---------------------------------------------------------------------------
void KUIButton::LoadTexture(TiXmlNode* pkNode)
{
    KUITexture* pkTexture = NULL;

    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "HighlightTexture");
    if (pkTexture)
    {
        DetachWidget(m_spHighlightTexture);
        m_spHighlightTexture = pkTexture;
        m_spHighlightTexture->SetExtName("HighlightTexture");
    }

    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "NormalTexture");
    if (pkTexture)
    {
        DetachWidget(m_spNormalTexture);
        m_spNormalTexture = pkTexture;
        m_spNormalTexture->SetExtName("NormalTexture");
    }

    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "PushedTexture");
    if (pkTexture)
    {
        DetachWidget(m_spPushedTexture);
        m_spPushedTexture = pkTexture;
        m_spPushedTexture->SetExtName("PushedTexture");
    }

    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "DisabledTexture");
    if (pkTexture)
    {
        DetachWidget(m_spDisabledTexture);
        m_spDisabledTexture = pkTexture;
        m_spDisabledTexture->SetExtName("DisabledTexture");
    }

    m_spCurTexture = m_spNormalTexture;
}
//---------------------------------------------------------------------------
bool KUIButton::UpdateInput()
{
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
        else if (pkMouse->ButtonIsDown(NiInputMouse::NIM_LEFT))
        {
            m_spCurTexture = m_spPushedTexture;
            return true;
        }
        else
        {
            m_spCurTexture = m_spHighlightTexture;
            return true;
        }
    }
    else
    {
        m_spCurTexture = m_spNormalTexture;
    }
	*/

    return false;
}
//---------------------------------------------------------------------------
void KUIButton::Update()
{
    KUIFrame::Update();

    // 기본 텍스처 숨김
    if (m_spNormalTexture)
        m_spNormalTexture->Hide();
    if (m_spHighlightTexture)
        m_spHighlightTexture->Hide();
    if (m_spPushedTexture)
        m_spPushedTexture->Hide();
    if (m_spDisabledTexture)
        m_spDisabledTexture->Hide();

    // 현재 텍스처 활성화
    if (m_spCurTexture)
        m_spCurTexture->Show();

    if (m_pkCurText)
    {
        m_pkCurText->SetText(m_strText);
        //m_pkCurText->SetArrangeMode(KFont::eArrCenter);
    }    
}
//---------------------------------------------------------------------------
void KUIButton::Render()
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
KUIWidget* KUIButton::CreateObject()
{
    return new KUIButton;
}
//---------------------------------------------------------------------------
void KUIButton::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUIButton* pkButton = (KUIButton*) pkWidget;

    // Text
    if (!pkButton->m_strText.empty())
        m_strText = pkButton->m_strText;

    // Texture
    if (pkButton->m_spNormalTexture)
    {        
        m_spNormalTexture = (KUITexture*) FindWidgetByExtName("NormalTexture");
    }    

    if (pkButton->m_spHighlightTexture)
    {
        m_spHighlightTexture = (KUITexture*) FindWidgetByExtName("HighlightTexture");
    }    

    if (pkButton->m_spPushedTexture)
    {
        m_spPushedTexture = (KUITexture*) FindWidgetByExtName("PushedTexture");
    }    

    if (pkButton->m_spDisabledTexture)
    {
        m_spDisabledTexture = (KUITexture*) FindWidgetByExtName("DisabledTexture");
    }

    m_spCurTexture = m_spNormalTexture;

    // FontString
    if (pkButton->m_spNormalText)
    {
        m_spNormalText = (KUIFontString*) FindWidgetByExtName("NormalText");
    }
    
    m_pkCurText = m_spNormalText;

    //SetTextPosition();
}
//---------------------------------------------------------------------------
