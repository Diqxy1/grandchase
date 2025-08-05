// MinMax.cpp: implementation of the CMinMax class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KGCPerformanceCounter.h"
#include "MinMax.h"

int RandomNumber(int iMin, int iMax)
{
	if (iMin == iMax) return(iMin);
	int m_iValue = ((rand() % (abs(iMax-iMin)+1))+iMin);
	return m_iValue;
}

float RandomNumber(float fMin, float fMax)
{
	float m_fValue;
	if (fMin == fMax)
	{
		m_fValue = fMin;
	}
	else
	{
		float fRandom = (float)rand() / (float)RAND_MAX;
		m_fValue = (fRandom * ASMfabs(fMax-fMin))+fMin;
	}
	return m_fValue;
}
