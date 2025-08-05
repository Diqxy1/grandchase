#pragma once
#include <WTypes.h>
#include <KNCSingleton.h>

class KGCMath
{
	DeclareSingleton(KGCMath);
public:
	KGCMath(void);
	~KGCMath(void);

	enum {
		EP_LINEAR = 0,
		EP_SLOWER,
		EP_FASTER,
		EP_SPRING,
		EP_SPRING2,
		EP_VIBRATE,
		EP_CLOUD,
		EP_OVER,
		EP_OVER2,
		EP_SINCOSCOS,
	};

	void BindLua();

    int Mod( int v, int i );
	float SpringFunction( float t, float g, float w, float e, float x );
	float VibrateFunction( float a, float b, float c, float d, float g, float w, float x );
	float QuadraticFunction( float x, float a = 1.0f, float b = 0.0f, float c = 0.0f );	// y = a(x-b)^2 + c
	float f( int iStyle, float x );
};

DefSingletonInline(KGCMath);