#include "stdafx.h"
#include ".\foothold.h"
#include "KGCMap.h"

CFootHold::CFootHold(void) : KFootHold<CFootHold>()
{
}

CFootHold::~CFootHold(void)
{
}

void CFootHold::SetPos(const float& fX, const float& fY)
{
	m_fX = fX + m_fLocalX;
	m_fY = fY + m_fLocalY;

	m_fPrevX = m_fX;
	m_fPrevY = m_fY;

	m_iIndexX = static_cast<int>(m_fX / FOOTHOLDSIZE);
	m_iIndexY = static_cast<int>(m_fY / FOOTHOLDSIZE);

	m_iPrevIndexX = m_iIndexX;
	m_iPrevIndexY = m_iIndexY;
}