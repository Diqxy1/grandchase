#include "stdafx.h"
#include ".\GCFootHold.h"
//




KGCFootHold::KGCFootHold(void) : KFootHold<KGCFootHold>()
, m_pParent(NULL)
{
}

KGCFootHold::~KGCFootHold(void)
{
    m_pParent = NULL;
}

void KGCFootHold::SetPos( const float& fX, const float& fY )
{
    m_fX = fX + m_fLocalX;
    m_fY = fY + m_fLocalY;

    m_fPrevX = m_fX;
    m_fPrevY = m_fY;

    m_iIndexX = TO_INDEX(m_fX);
    m_iIndexY = TO_INDEX(m_fY);

    m_iPrevIndexX = m_iIndexX;
    m_iPrevIndexY = m_iIndexY;
}
