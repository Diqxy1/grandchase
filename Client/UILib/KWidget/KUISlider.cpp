// Precompile Header
#include "stdafx.h"

#include "KUISlider.h"
#include "tinyxml.h"

//NiImplementRTTI(KUISlider, KUIFrame);

//---------------------------------------------------------------------------
KUISlider::KUISlider()
: m_iMinRange(0), m_iMaxRange(1), m_iCurValue(0),
m_bThumbPressed(false)
{
}
//---------------------------------------------------------------------------
void KUISlider::RegisterLuabind()
{
    lua_tinker::class_<KUISlider>("KUISlider")
        .inh<KUIFrame>()
        .def("MaxCurValue", &KUISlider::MaxCurValue)
        .def("IncCurValue", &KUISlider::IncCurValue)
        .def("DecCurValue", &KUISlider::DecCurValue)        
        ;
}
//---------------------------------------------------------------------------
void KUISlider::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    LoadButton(pkNode);
    LoadTexture(pkNode);

    SetButtonPosition();
}
//---------------------------------------------------------------------------
void KUISlider::SetButtonPosition()
{
    float fHeight = m_spBtnDown->GetSize().y;
    m_spBtnDown->SetLocalPos(D3DXVECTOR2(0.f, GetSize().y - fHeight));
}
//---------------------------------------------------------------------------
void KUISlider::LoadButton(TiXmlNode* pkNode)
{
    KUIButton* pkButton = NULL;

    pkButton = KUIWidget::CreateOne<KUIButton>(
        this, pkNode, "UpButton");
    if (pkButton)
    {
        DetachWidget(m_spBtnUp);
        m_spBtnUp = pkButton;
        m_spBtnUp->SetExtName("UpButton");
    }

    pkButton = KUIWidget::CreateOne<KUIButton>(
        this, pkNode, "DownButton");
    if (pkButton)
    {
        DetachWidget(m_spBtnDown);
        m_spBtnDown = pkButton;
        m_spBtnDown->SetExtName("DownButton");
    }
}
//---------------------------------------------------------------------------
void KUISlider::LoadTexture(TiXmlNode* pkNode)
{
    KUITexture* pkTexture = NULL;
    pkTexture = KUIWidget::CreateOne<KUITexture>(
        this, pkNode, "ThumbTexture");

    if (pkTexture)
    {
        DetachWidget(m_spThumbTex);
        m_spThumbTex = pkTexture;
        m_spThumbTex->SetExtName("ThumbTexture");
    }
}
//---------------------------------------------------------------------------
void KUISlider::Render()    
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
void KUISlider::Update()
{
    KUIFrame::Update();

    UpdateThumb();
}
//---------------------------------------------------------------------------
void KUISlider::UpdateThumb()
{
    if (m_spThumbTex)
    {
        m_spThumbTex->SetLocalPos(D3DXVECTOR2(0.f, GetCurPos()));
    }
}
//---------------------------------------------------------------------------
bool KUISlider::UpdateInput()
{
    KUIFrame::UpdateInput();

    //m_spBtnUp->UpdateInput();
    //m_spBtnDown->UpdateInput();

	/*
    NiInputMouse* pkMouse = _KAppHolder()->GetInputSystem()->GetMouse();
    NiPoint2 kCurPos = _KAppHolder()->GetCursorPos();
	

    if (!m_bThumbPressed)
    {
        if (IsCursorInRect())
        {
            if (pkMouse->ButtonWasPressed(NiInputMouse::NIM_LEFT))
            {
                m_iCurValue = PosToValue(kCurPos.y);

                m_bThumbPressed = true;
            }
        }
    }
    else
    {
        if (IsCursorInRectY())
        {
            m_iCurValue = PosToValue(kCurPos.y);
        }
    }

    if (pkMouse->ButtonWasReleased(NiInputMouse::NIM_LEFT))
    {
        m_bThumbPressed = false;
    }
	*/

    return false;
}
//---------------------------------------------------------------------------
bool KUISlider::IsCursorInRect()
{
	/*
    NiPoint2 kCurPos = _KAppHolder()->GetCursorPos();
    return (GetWorldPos().x < kCurPos.x &&
        kCurPos.x < GetWorldPos().x + m_kSize.GetDim().x &&
        GetWorldPos().y + m_spBtnUp->GetSize().y < kCurPos.y &&
        kCurPos.y < GetWorldPos().y + m_kSize.GetDim().y - m_spBtnDown->GetSize().y);
	*/
	return false;
}
//---------------------------------------------------------------------------
bool KUISlider::IsCursorInRectY()
{
	/*
    NiPoint2 kCurPos = _KAppHolder()->GetCursorPos();
    return (GetWorldPos().y + m_spBtnUp->GetSize().y < kCurPos.y &&
        kCurPos.y < GetWorldPos().y + m_kSize.GetDim().y - m_spBtnDown->GetSize().y);
	*/
	return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUISlider::CreateObject()
{
    return new KUISlider;
}
//---------------------------------------------------------------------------
void KUISlider::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUISlider* pkSlider = (KUISlider*) pkWidget;

    if (pkSlider->m_spThumbTex)
    {
        m_spThumbTex = (KUITexture*) FindWidgetByExtName("ThumbTexture");
    }

    if (pkSlider->m_spBtnUp)
    {
        m_spBtnUp = (KUIButton*) FindWidgetByExtName("UpButton");
    }    

    if (pkSlider->m_spBtnDown)
    {
        m_spBtnDown = (KUIButton*) FindWidgetByExtName("DownButton");
    }    

    SetButtonPosition();
}
//---------------------------------------------------------------------------
void KUISlider::SetRange(int iMin, int iMax)
{
    m_iMinRange = iMin;
    m_iMaxRange = iMax;
}
//---------------------------------------------------------------------------
int KUISlider::GetCurValue()
{
    return m_iCurValue;
}
//---------------------------------------------------------------------------
void KUISlider::SetCurValue(int iValue)
{
    m_iCurValue = iValue;
}
//---------------------------------------------------------------------------
void KUISlider::MaxCurValue()
{
    m_iCurValue = m_iMaxRange;
}
//---------------------------------------------------------------------------
void KUISlider::DecCurValue()
{    
    --m_iCurValue;
    
    if (m_iCurValue < m_iMinRange)
        m_iCurValue = m_iMinRange;
}
//---------------------------------------------------------------------------
void KUISlider::IncCurValue()
{
    ++m_iCurValue;

    if (m_iCurValue > m_iMaxRange)
        m_iCurValue = m_iMaxRange;
}
//---------------------------------------------------------------------------
float KUISlider::GetMinPos()
{
    return m_spBtnUp->GetSize().y;
}
//---------------------------------------------------------------------------
float KUISlider::GetMaxPos()
{
    return GetSize().y - m_spBtnDown->GetSize().y * 2.f;
}
//---------------------------------------------------------------------------
float KUISlider::GetCurPos()
{
    float fMinPos = GetMinPos();
    float fMaxPos = GetMaxPos();

    float fRange = (float) (m_iMaxRange - m_iMinRange);
    if (fRange == 0.f)
        return fMinPos;

    float fRaito = m_iCurValue / fRange;
    if (fRaito > 1.f)
        fRaito = 1.f;

    float fPos = (fMaxPos - fMinPos) * fRaito;
    return fPos + fMinPos;
}
//---------------------------------------------------------------------------
int KUISlider::PosToValue(float fPos)
{
    float fMinPos = GetMinPos();
    float fMaxPos = GetMaxPos();

    float fRatio = (fPos - (GetWorldPos().y + fMinPos)) / fMaxPos;
    float fRange = (float) (m_iMaxRange - m_iMinRange);

    return (int) (fRange * fRatio + 0.5f);
}
//---------------------------------------------------------------------------
