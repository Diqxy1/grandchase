#pragma once
//#include <windows.h>

class KGCInterpolate
{
public:
    KGCInterpolate(void)
    {
        fStartValue         = 0.0f;
        fCurValue           = 0.0f;
        fStep               = 0.0f;
        fEndValue           = 0.0f;
        dwRemainStepTime    = 0;
        dwTotalStepTime     = 0;
    }
    ~KGCInterpolate(void) { }

    virtual void Step(void) = 0;
    virtual float GetCurValue(void) = 0;
    virtual void SetCurValue(float fValue) = 0;
    virtual bool IsComplete(void) = 0;
    virtual void SetInterpolate(DWORD dwStepTime, float fStart, float fEnd) = 0;
    void Init()
    {
        fCurValue = fStartValue;
        dwRemainStepTime = dwTotalStepTime;
    }
	void SetEnd()
	{
		dwRemainStepTime = 0;
		fCurValue = fEndValue;
	}

protected:
    float   fStartValue;            // 시작 값
    float   fCurValue;              // 현재 값
    float   fStep;                  // 변화 값
    float   fEndValue;              // 종료 값
    DWORD   dwRemainStepTime;       // 남은 스텝 횟수
    DWORD   dwTotalStepTime;        // 총 스텝 횟수
};

//************************************************************************
// 선형보간
//************************************************************************
class KGCLinearInterpolate : public KGCInterpolate
{
public:
    KGCLinearInterpolate(void);
    virtual ~KGCLinearInterpolate(void);

    virtual void Step(void);
    virtual float GetCurValue(void);
    virtual void SetCurValue(float fValue);
    virtual bool IsComplete(void);
    virtual void SetInterpolate(DWORD dwStepTime, float fStart, float fEnd);
};

//************************************************************************
// 가속보간
//************************************************************************
class KGCAccelInterpolate : public KGCInterpolate
{
public:
    KGCAccelInterpolate(void);
    virtual ~KGCAccelInterpolate(void);

    virtual void Step(void);
    virtual float GetCurValue(void);
    virtual void SetCurValue(float fValue);
    virtual bool IsComplete(void);
    virtual void SetInterpolate(DWORD dwStepTime, float fStart, float fEnd);

protected:
    float   fAcceleration;          // 가속보간 시 필요한 변수
    float   fSpeed;                 // 가속보간 시 필요한 변수
};