// Precompile Header
#include "stdafx.h"
#include "KUIDimension2.h"

#include "tinyxml.h"

//---------------------------------------------------------------------------
KUIDimension2::KUIDimension2()
: m_fX(0), m_fY(0)
{
}
//---------------------------------------------------------------------------
void KUIDimension2::Load(TiXmlNode* pkNode)
{
    TiXmlElement* pkAbsElt = pkNode->FirstChildElement("Dimension");
    if (pkAbsElt)
    {
        KStrToType(pkAbsElt->Attribute("x"), m_fX);
        KStrToType(pkAbsElt->Attribute("y"), m_fY);
    }
}
//---------------------------------------------------------------------------
D3DXVECTOR2 KUIDimension2::GetDim()
{
    return D3DXVECTOR2(m_fX, m_fY);
}
//---------------------------------------------------------------------------
void KUIDimension2::SetDim(D3DXVECTOR2 kDim)
{
    m_fX = kDim.x;
    m_fY = kDim.y;
}
//---------------------------------------------------------------------------
bool KUIDimension2::IsZero()
{
    return (m_fX == 0 && m_fY == 0);
}
//---------------------------------------------------------------------------
