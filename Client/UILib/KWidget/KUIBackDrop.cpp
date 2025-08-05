// Precompile Header
#include "stdafx.h"

#include "KUIBackDrop.h"

#include "tinyxml.h"

//NiImplementRTTI(KUIBackDrop, KUIFrame);

//---------------------------------------------------------------------------
void KUIBackDrop::Load(TiXmlNode* pkNode)
{
    KUIFrame::Load(pkNode);

    TiXmlElement* pkBDElt = pkNode->ToElement();
    m_strBackFilename = pkBDElt->Attribute("bgFile");
    m_strBorderFilename = pkBDElt->Attribute("edgeFile");

    TiXmlElement* pkTileSizeElt = pkNode->FirstChildElement("TileSize");
    if (pkTileSizeElt)
    {
        KStrToType(pkTileSizeElt->Attribute("AbsValue"), m_fTileSize);
    }

    TiXmlElement* pkEdgeSizeElt = pkNode->FirstChildElement("EdgeSize");
    if (pkEdgeSizeElt)
    {
        KStrToType(pkEdgeSizeElt->Attribute("AbsValue"), m_fEdgeSize);
    }

    CreateBackground();
}
//---------------------------------------------------------------------------
void KUIBackDrop::CreateBackground()
{   
    m_spBackTex = new GCUITexture( m_strBackFilename );
	m_spEdgeTex = new GCUITexture( m_strBorderFilename );

    CreateBack();
    CreateBorder();
}
//---------------------------------------------------------------------------
void KUIBackDrop::CreateBack()
{    
    float fCurX;
    float fCurY;

    bool bDoneX;
    bool bDoneY;

    fCurY = 0;
    bDoneY = false;
    while (true)
    {        
        if (bDoneY)
            break;

        fCurX = 0;
        bDoneX = false;
        while (true)
        {
            if (bDoneX)
                break;

            m_spBackTex->AddNewScreenRect(
                GetWorldPos().y + fCurY, GetWorldPos().x + fCurX,
                m_fTileSize, m_fTileSize, 0, 0);

            fCurX += m_fTileSize;
            if (fCurX >= GetSize().x)
            {
                fCurX = GetSize().x;
                bDoneX = true;
            }
        }

        fCurY += m_fTileSize;
        if (fCurY >= GetSize().y)
        {
            fCurY = GetSize().y;
            bDoneY = true;
        }
    }
}
//---------------------------------------------------------------------------
void KUIBackDrop::CreateBorder()
{
    // 모서리 영역
    CreateBorderRectConer();

    // 변 영역
    CreateBorderRectEdge();
}
//---------------------------------------------------------------------------
void KUIBackDrop::CreateBorderRectConer()
{
    // Top Left
    m_spEdgeTex->AddNewScreenRect(
        GetWorldPos().y, GetWorldPos().x,
        m_fEdgeSize, m_fEdgeSize, 0, 6 * m_fEdgeSize);

    // Top Right
    m_spEdgeTex->AddNewScreenRect(
        GetWorldPos().y,
        GetWorldPos().x + GetSize().x - m_fEdgeSize,
        m_fEdgeSize, m_fEdgeSize, 0, 7 * m_fEdgeSize);

    // Bottom Left
    m_spEdgeTex->AddNewScreenRect(
        GetWorldPos().y + GetSize().y - m_fEdgeSize,
        GetWorldPos().x,
        m_fEdgeSize, m_fEdgeSize, 0, 4 * m_fEdgeSize);

    // Bottom Right
    m_spEdgeTex->AddNewScreenRect(
        GetWorldPos().y + GetSize().y - m_fEdgeSize,
        GetWorldPos().x + GetSize().x - m_fEdgeSize,
        m_fEdgeSize, m_fEdgeSize, 0, 5 * m_fEdgeSize);
}
//---------------------------------------------------------------------------
void KUIBackDrop::CreateBorderRectEdge()
{
    float fCurPos;
    bool bDone;

    // Top
    fCurPos = m_fEdgeSize;
    bDone = false;
    while (true)
    {
        if (bDone)
            break;

        m_spEdgeTex->AddNewScreenRect(
            GetWorldPos().y, GetWorldPos().x + fCurPos,
            m_fEdgeSize, m_fEdgeSize, 0, 3 * m_fEdgeSize);

        fCurPos += m_fEdgeSize;
        if (fCurPos >= GetSize().x - m_fEdgeSize)
        {
            fCurPos = GetSize().x - m_fEdgeSize;
            bDone = true;
        }
    }

    // Bottom
    fCurPos = m_fEdgeSize;
    bDone = false;
    while (true)
    {
        if (bDone)
            break;

        m_spEdgeTex->AddNewScreenRect(
            GetWorldPos().y + GetSize().y - m_fEdgeSize,
            GetWorldPos().x + fCurPos,
            m_fEdgeSize, m_fEdgeSize, 0, 2 * m_fEdgeSize);

        fCurPos += m_fEdgeSize;
        if (fCurPos >= GetSize().x - m_fEdgeSize)
        {
            fCurPos = GetSize().x - m_fEdgeSize;
            bDone = true;
        }
    }

    // Left
    fCurPos = m_fEdgeSize;
    bDone = false;
    while (true)
    {
        if (bDone)
            break;

        m_spEdgeTex->AddNewScreenRect(
            GetWorldPos().y + fCurPos,
            GetWorldPos().x,
            m_fEdgeSize, m_fEdgeSize, 0, 0 * m_fEdgeSize);

        fCurPos += m_fEdgeSize;
        if (fCurPos >= GetSize().y - m_fEdgeSize)
        {
            fCurPos = GetSize().y - m_fEdgeSize;
            bDone = true;
        }
    }

    // Right
    fCurPos = m_fEdgeSize;
    bDone = false;
    while (true)
    {
        if (bDone)
            break;

        m_spEdgeTex->AddNewScreenRect(
            GetWorldPos().y + fCurPos,
            GetWorldPos().x + GetSize().x - m_fEdgeSize,
            m_fEdgeSize, m_fEdgeSize, 0, 1 * m_fEdgeSize);

        fCurPos += m_fEdgeSize;
        if (fCurPos >= GetSize().y - m_fEdgeSize)
        {
            fCurPos = GetSize().y - m_fEdgeSize;
            bDone = true;
        }
    }
}
//---------------------------------------------------------------------------
void KUIBackDrop::Render()
{
    KUIFrame::Render();
	
    m_spBackTex->Render();
    m_spEdgeTex->Render();
	
}
//---------------------------------------------------------------------------
void KUIBackDrop::Update()
{
    KUIFrame::Update();
}
//---------------------------------------------------------------------------
void KUIBackDrop::UpdateBackground()
{
    CreateBack();
    CreateBorder();
}
//---------------------------------------------------------------------------
KUIWidget* KUIBackDrop::CreateObject()
{
    return new KUIBackDrop;
}
//---------------------------------------------------------------------------
void KUIBackDrop::CopyMember(KUIWidget* pkWidget)
{
    KUIFrame::CopyMember(pkWidget);

    KUIBackDrop* pkBackDrop = (KUIBackDrop*) pkWidget;

    m_fTileSize = pkBackDrop->m_fTileSize;
    m_fEdgeSize = pkBackDrop->m_fEdgeSize;

    m_strBackFilename = pkBackDrop->m_strBackFilename;
    m_strBorderFilename = pkBackDrop->m_strBorderFilename;

    CreateBackground();
    UpdateBackground();
}
//---------------------------------------------------------------------------
