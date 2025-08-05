// Precompile Header
#include "stdafx.h"
#include "KUICheckButton.h"
#include "tinyxml.h"

//NiImplementRTTI(KUICheckButton, KUIFrame);

//---------------------------------------------------------------------------
KUICheckButton::KUICheckButton()
: m_iCheckState(0)
{
}
//---------------------------------------------------------------------------
void KUICheckButton::Load(TiXmlNode* pkNode)
{
    KUIButton::Load(pkNode);
}
//---------------------------------------------------------------------------
void KUICheckButton::LoadTexture(TiXmlNode* pkNode)
{
    KUIButton::LoadTexture(pkNode);

    KUITexture* pkTexture = NULL;

    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "CheckedTexture");
    if (pkTexture)
    {
        DetachWidget(m_spCheckTex);
        m_spCheckTex = pkTexture;
        m_spCheckTex->SetExtName("CheckedTexture");
    }

    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "DisabledCheckedTexture");
    if (pkTexture)
    {
        DetachWidget(m_spDisCheckTex);
        m_spDisCheckTex = pkTexture;
        m_spDisCheckTex->SetExtName("DisabledCheckedTexture");
    }

    m_pkCurCheckTex = m_spCheckTex;
}
//---------------------------------------------------------------------------
bool KUICheckButton::UpdateInput()
{
    //KUIButton::UpdateInput();
	/*
    NiInputMouse* pkMouse = _KAppHolder()->GetInputSystem()->GetMouse();

    if (IsCursorInRect())
    {
        if (pkMouse->ButtonWasPressed(NiInputMouse::NIM_LEFT))
        {
            switch (GetCheckState())
            {
            case eChecked:
                SetCheckState(eUnChecked);
                break;
            case eUnChecked:
                SetCheckState(eChecked);
                break;
            }

            m_spCurTexture = m_spPushedTexture;
            return true;
        }
        else
        {
            m_spCurTexture = m_spHighlightTexture;
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
void KUICheckButton::Update()
{
    KUIButton::Update();

    // 기본 텍스처 숨김
    if (m_spCheckTex)
        m_spCheckTex->Hide();
    if (m_spDisCheckTex)
        m_spDisCheckTex->Hide();

    // 현재 텍스처 활성화
    if (m_pkCurCheckTex)
        m_pkCurCheckTex->Show();
}
//---------------------------------------------------------------------------
void KUICheckButton::Render()
{
    KUIButton::Render();
}
//---------------------------------------------------------------------------
KUIWidget* KUICheckButton::CreateObject()
{
    return new KUICheckButton;
}
//---------------------------------------------------------------------------
void KUICheckButton::CopyMember(KUIWidget* pkWidget)
{
    KUIButton::CopyMember(pkWidget);

    KUICheckButton* pkButton = (KUICheckButton*) pkWidget;

    // Texture
    if (pkButton->m_spCheckTex)
    {
        m_spCheckTex = (KUITexture*) FindWidgetByExtName("CheckedTexture");
    }

    if (pkButton->m_spDisCheckTex)
    {
        m_spDisCheckTex = (KUITexture*) FindWidgetByExtName("DisabledCheckedTexture");
    }

    m_pkCurCheckTex = m_spCheckTex;
}
//---------------------------------------------------------------------------
void KUICheckButton::SetCheckState(int iCheckState)
{
    m_iCheckState = iCheckState;

    switch (iCheckState)
    {
    case eChecked:
        m_pkCurCheckTex = m_spCheckTex;
        break;
    case eUnChecked:
        m_pkCurCheckTex = NULL;
        break;
    case eDisabled:
        m_pkCurCheckTex = m_spDisCheckTex;
        break;
    }
}
//---------------------------------------------------------------------------
int KUICheckButton::GetCheckState()
{
    return m_iCheckState;
}
//---------------------------------------------------------------------------
