#include "stdafx.h"
#include ".\gcinterpolate.h"
//

//************************************************************************
// 선형보간 클래스 구현
//************************************************************************
KGCLinearInterpolate::KGCLinearInterpolate(void)
{

}

KGCLinearInterpolate::~KGCLinearInterpolate(void)
{

}

void KGCLinearInterpolate::Step(void)
{
    if( dwRemainStepTime > 0 )
    {
        --dwRemainStepTime;

        fCurValue += fStep;
        if( fStep >= 0 )
        {
            if( fCurValue > fEndValue )
            {
                fCurValue = fEndValue;
                dwRemainStepTime = 0;
            }
        }
        else
        {
            if( fCurValue < fEndValue )
            {
                fCurValue = fEndValue;
                dwRemainStepTime = 0;
            }
        }
    }
}

float KGCLinearInterpolate::GetCurValue(void)
{
    return fCurValue;
}

void KGCLinearInterpolate::SetCurValue(float fValue)
{
    fCurValue = fValue;
}

bool KGCLinearInterpolate::IsComplete(void)
{
    if( dwRemainStepTime <= 0 )
    {
        fCurValue = fEndValue;
        return true;
    }

    return false;
}

void KGCLinearInterpolate::SetInterpolate(DWORD dwStepTime, float fStart, float fEnd)
{
    float fValue = 1.0f;
    if( fEnd - fStart < 0 )
        fValue = -fValue;

    fValue *= fabs(fEnd - fStart);

    fStep               = (fValue)/(float)dwStepTime;

    fStartValue         = fStart;
    fCurValue           = fStart;
    fEndValue           = fEnd;
    dwRemainStepTime    = dwStepTime;
    dwTotalStepTime     = dwStepTime;
}

//************************************************************************
// 가속 보간 클래스 구현
//************************************************************************
KGCAccelInterpolate::KGCAccelInterpolate(void)
{
    fAcceleration       = 0.0f;
    fSpeed              = 0.0f;
}

KGCAccelInterpolate::~KGCAccelInterpolate(void)
{

}

void KGCAccelInterpolate::Step(void)
{
    if( dwRemainStepTime > 0 )
    {
        --dwRemainStepTime;

        DWORD dwCurStep = dwTotalStepTime - dwRemainStepTime;

        fSpeed = dwCurStep*fAcceleration;
        fCurValue = fStartValue + (fSpeed)*dwCurStep;

        if( fEndValue >= fStartValue )
        {
            if( fCurValue > fEndValue )
            {
                fCurValue = fEndValue;
                dwRemainStepTime = 0;
            }
        }
        else
        {
            if( fCurValue < fEndValue )
            {
                fCurValue = fEndValue;
                dwRemainStepTime = 0;
            }
        }
    }
}

float KGCAccelInterpolate::GetCurValue(void)
{
    return fCurValue;
}

void KGCAccelInterpolate::SetCurValue(float fValue)
{
    fCurValue = fValue;
}

bool KGCAccelInterpolate::IsComplete(void)
{
    if( dwRemainStepTime <= 0 )
    {
        fCurValue = fEndValue;
        return true;
    }

    return false;
}

void KGCAccelInterpolate::SetInterpolate(DWORD dwStepTime, float fStart, float fEnd)
{
    fStartValue     = fStart;
    fCurValue		= fStart;
    fEndValue		= fEnd;
    fSpeed          = 0.0f;

    // x = x0 + v0*t + a*t*t.2 에서 유도한 공식
    fAcceleration       = (fEnd-fStart)/(dwStepTime*dwStepTime/4);
    dwTotalStepTime     = dwStepTime;
    dwRemainStepTime    = dwStepTime;
}
