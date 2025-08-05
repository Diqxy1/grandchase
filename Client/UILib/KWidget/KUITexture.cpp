// Precompile Header
#include "stdafx.h"

#include "KUITexture.h"
#include "KUIFrame.h"
#include "tinyxml.h"
//#include <assert.h>

//NiImplementRTTI(KUITexture, KUIFrame);

//---------------------------------------------------------------------------
KUITexture::KUITexture()
{
	m_spScrTexture = NULL;
}
//---------------------------------------------------------------------------
void KUITexture::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    TiXmlElement* pkTexElt = pkNode->ToElement();
    const char* pcTexFile = pkTexElt->Attribute("file");
    assert(pcTexFile);
    m_strFilename = pcTexFile;

    TiXmlElement* pkTexCoord = pkNode->FirstChildElement("TexCoords");
    assert(pkTexCoord);
    KStrToType(pkTexCoord->Attribute("left"), m_fTexLeft);
    KStrToType(pkTexCoord->Attribute("top"), m_fTexTop);

    CreateTexture();
    UpdateCoords();
}
//---------------------------------------------------------------------------
void KUITexture::CreateTexture()
{
    if (m_spScrTexture)
	{
        return;
	}

	m_spScrTexture = new GCUITexture( m_strFilename );

    float fWidth = GetSize().x;
    float fHeight = GetSize().y;

    if (fWidth == 0 && fHeight == 0)
    {
        fWidth = m_spScrTexture->GetWidth();
        fHeight = m_spScrTexture->GetHeight();

        m_kSize.SetDim(D3DXVECTOR2(fWidth, fHeight));
    }

    m_spScrTexture->AddNewScreenRect(GetWorldPos().y, GetWorldPos().x, 
        fWidth, fHeight, m_fTexTop, m_fTexLeft);
}
//---------------------------------------------------------------------------
void KUITexture::Update()
{
    if (!m_bVisible)
        return;

    KUIFrame::Update();

    UpdateCoords();
}
//---------------------------------------------------------------------------
void KUITexture::UpdateCoords()
{
    if (m_spScrTexture->GetScreenRect(0).m_vtScreenPos.x != GetWorldPos().x ||
        m_spScrTexture->GetScreenRect(0).m_vtScreenPos.y != GetWorldPos().y )
    {
        m_spScrTexture->GetScreenRect(0).m_vtScreenPos.x = GetWorldPos().x;
        m_spScrTexture->GetScreenRect(0).m_vtScreenPos.y = GetWorldPos().y;

        // m_spScrTexture->MarkAsChanged(NiScreenTexture::EVERYTHING_MASK);
    }	
}
//---------------------------------------------------------------------------
void KUITexture::Render()
{
    if (!m_bVisible)
        return;

    KUIFrame::Render();

	m_spScrTexture->Render();
    //m_spScrTexture->Draw(_KAppHolder()->GetRenderer());
}
//---------------------------------------------------------------------------
KUIWidget* KUITexture::CreateObject()
{
    return new KUITexture;
}
//---------------------------------------------------------------------------
void KUITexture::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUITexture* pkTexture = (KUITexture*) pkWidget;

    m_strFilename = pkTexture->m_strFilename;
    m_fTexLeft = pkTexture->m_fTexLeft;
    m_fTexTop = pkTexture->m_fTexTop;

 //   m_spScrTexture = (NiScreenTexture*) pkTexture->m_spScrTexture->Clone();
    CreateTexture();
    UpdateCoords();
}
//---------------------------------------------------------------------------
