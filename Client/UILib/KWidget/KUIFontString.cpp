// Precompile Header
#include "stdafx.h"
#include "KUIFontString.h"
#include "KUIFrame.h"

#include "tinyxml.h"

//#include "KFontMgr.h"

//NiImplementRTTI(KUIFontString, KUIFrame);

//---------------------------------------------------------------------------
KUIFontString::KUIFontString()
: m_iFontWidth(0), m_iFontHeight(0)
{
}
//---------------------------------------------------------------------------
void KUIFontString::RegisterLuabind()
{
    lua_tinker::class_<KUIFontString>("KUIFontString")
        .inh<KUIFrame>()
        ;
}
//---------------------------------------------------------------------------
void KUIFontString::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    TiXmlElement* pkFontElt = pkNode->FirstChildElement("Font");
    if (!pkFontElt)
        return;

    const char* pcFontname = pkFontElt->Attribute("name");
    if (pcFontname)
    {
        m_strFontName = pcFontname;
    }
    
    const char* pcText = pkFontElt->Attribute("text");
    if (pcText)
    {
        m_strText = pcText;
    }

    int iWidth, iHeight;
    pkFontElt->Attribute("width", &iWidth);
    pkFontElt->Attribute("height", &iHeight);
    if (iWidth && iHeight)
    {
        m_iFontWidth = iWidth;
        m_iFontHeight = iHeight;
    }    

    LoadColor(pkNode);
    CreateString2D();
}
//---------------------------------------------------------------------------
void KUIFontString::LoadColor(TiXmlNode* pkNode)
{
    m_kColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
    TiXmlElement* pkColElt = pkNode->FirstChildElement("Color");
    if (pkColElt)
    {
        KStrToType(pkColElt->Attribute("r"), m_kColor.r);
        KStrToType(pkColElt->Attribute("g"), m_kColor.g);
        KStrToType(pkColElt->Attribute("b"), m_kColor.b);
        KStrToType(pkColElt->Attribute("a"), m_kColor.a);		
    }
}
//---------------------------------------------------------------------------
void KUIFontString::CreateString2D()
{
    if (m_strFontName.empty())
        return;

	/*
    KFontData* pkFontData = _KFontMgr()->Create(
        m_strFontName.c_str(), m_iFontWidth, m_iFontHeight);
    m_spString2D = new KFont;
    m_spString2D->SetFontData(pkFontData);
    m_spString2D->OStream() << m_strText;
    m_spString2D->SetSize(GetSize());
	*/
}
//---------------------------------------------------------------------------
void KUIFontString::Update()
{
    KUIFrame::Update();

	/*
    if (m_spString2D)
    {
        m_spString2D->Clear();
        m_spString2D->OStream() << m_strText;
        m_spString2D->SetPos(GetWorldPos());
    }
	*/
}
//---------------------------------------------------------------------------
void KUIFontString::Render()
{
    KUIFrame::Render();

	/*
    if (m_spString2D)
        m_spString2D->Render();
	*/
}
//---------------------------------------------------------------------------
KUIWidget* KUIFontString::CreateObject()
{
    return new KUIFontString;
}
//---------------------------------------------------------------------------
void KUIFontString::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUIFontString* pkFontString = (KUIFontString*) pkWidget;

    m_strText = pkFontString->m_strText;
    m_strFontName = pkFontString->m_strFontName;
    m_iFontWidth = pkFontString->m_iFontWidth;
    m_iFontHeight = pkFontString->m_iFontHeight;
    m_kColor = pkFontString->m_kColor;

    CreateString2D();
}
//---------------------------------------------------------------------------
void KUIFontString::SetText(std::string strText)
{
    if (strText.empty())
        return;

    m_strText = strText;
}
//---------------------------------------------------------------------------
std::string KUIFontString::GetText()
{
    return m_strText;
}
//---------------------------------------------------------------------------
void KUIFontString::SetArrangeMode(int iArrangeMode)
{
	/*
    if (m_spString2D)
        m_spString2D->SetArrangeMode(iArrangeMode);
	*/
}
//---------------------------------------------------------------------------
