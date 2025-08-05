// Precompile Header
#include "stdafx.h"

#include "KUIOffset.h"
#include "KUIWidget.h"
#include "tinyxml.h"

//---------------------------------------------------------------------------
KUIOffset::KUIOffset(KUIWidget* pkOwner)
: m_iPointDir(eTopLeft)
{
    SetOwner(pkOwner);
}
//---------------------------------------------------------------------------
void KUIOffset::SetOwner(KUIWidget* pkOwner)
{
    m_pkOwner = pkOwner;
}
//---------------------------------------------------------------------------
void KUIOffset::Load(TiXmlNode* pkNode)
{
    TiXmlElement* pkOffsetElt = pkNode->FirstChildElement("Offset");
    if (pkOffsetElt)
    {
        KUIDimension2::Load(pkOffsetElt);

        LoadPointDir(pkOffsetElt);
    }
}
//---------------------------------------------------------------------------
void KUIOffset::LoadPointDir(TiXmlElement* pkElt)
{
    const char* pcPoint = pkElt->Attribute("point");
    if (pcPoint)
    {
        m_iPointDir = eTopLeft;

        std::string strPoint = pcPoint;
        if (strPoint == "TOPLEFT")
        {
            m_iPointDir = eTopLeft;
        }
        else if (strPoint == "TOP")
        {
            m_iPointDir = eTop;
        }
        else if (strPoint == "TOPRIGHT")
        {
            m_iPointDir = eTopRight;
        }
        else if (strPoint == "RIGHT")
        {
            m_iPointDir = eRight;
        }
        else if (strPoint == "BOTTOMRIGHT")
        {
            m_iPointDir = eBottomRight;
        }
        else if (strPoint == "BOTTOM")
        {
            m_iPointDir = eBottom;
        }
        else if (strPoint == "BOTTOMLEFT")
        {
            m_iPointDir = eBottomLeft;
        }
        else if (strPoint == "LEFT")
        {
            m_iPointDir = eLeft;
        }
        else if (strPoint == "CENTER")
        {
            m_iPointDir = eCenter;
        }
    }
}
//---------------------------------------------------------------------------
D3DXVECTOR2 KUIOffset::GetDim()
{
    D3DXVECTOR2 kExtraPos(0.0f,0.0f);
    KUIWidget* pkParent = m_pkOwner->GetParent();    

    if (pkParent)
    {
        D3DXVECTOR2 kOwnerSize = m_pkOwner->GetSize();
        D3DXVECTOR2 kParentSize = pkParent->GetSize();

        switch (m_iPointDir)
        {        
        case eTopRight:
            kExtraPos = D3DXVECTOR2(kParentSize.x - kOwnerSize.x, 0);
            break;        
        case eBottomRight:
            kExtraPos = D3DXVECTOR2(kParentSize.x - kOwnerSize.x,
                kParentSize.y - kOwnerSize.y);
            break;        
        case eBottomLeft:
            kExtraPos = D3DXVECTOR2(0, kParentSize.y - kOwnerSize.y);
            break;
        case eTop:
            kExtraPos = D3DXVECTOR2((kParentSize.x - kOwnerSize.x)/2.f, 0);
            break;
        case eRight:
            kExtraPos = D3DXVECTOR2(kParentSize.x - kOwnerSize.x,
                (kParentSize.y - kOwnerSize.y)/2.f);
            break;
        case eBottom:
            kExtraPos = D3DXVECTOR2((kParentSize.x - kOwnerSize.x)/2.f,
                kParentSize.y - kOwnerSize.y);
            break;
        case eLeft:
            kExtraPos = D3DXVECTOR2(0, (kParentSize.y - kOwnerSize.y)/2.f);
            break;
        case eCenter:
            kExtraPos = D3DXVECTOR2((kParentSize.x - kOwnerSize.x)/2.f, 
                (kParentSize.y - kOwnerSize.y)/2.f);
            break;
        }
    }

    return KUIDimension2::GetDim() + kExtraPos;
}
//---------------------------------------------------------------------------
void KUIOffset::SetDim(D3DXVECTOR2 kDim)
{
    KUIDimension2::SetDim(kDim);
}
//---------------------------------------------------------------------------
