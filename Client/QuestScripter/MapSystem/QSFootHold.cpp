#include "stdafx.h"
//#include "../stdafx.h"
#include ".\QSFootHold.h"
//#include <cmath.h>
#include "QSFantasticMap.h"
#include "QSFHVector.h"
//#include "../../GCGlobal.h"

KQSFootHold::KQSFootHold(void) : KFootHold<KQSFootHold>(), m_pParent(NULL)
{
}

KQSFootHold::~KQSFootHold(void)
{
    m_pParent = NULL;
}

void KQSFootHold::SetPos( const float& fX, const float& fY )
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
