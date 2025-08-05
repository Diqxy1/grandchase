#ifndef _SPLINE_H_
#define _SPLINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>

class KSpline
{
public:
    virtual float operator()( float t ) const = 0;
};

class KSplineFlat : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return 1.0f;
    }
};

class KSplineLinear : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return t;
    }
};

class KSplineExponential : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return t * t;
    }
};

class KSplineLogarithm : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return ::sqrtf( t );
    }
};

class KSplineSineLinear : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return t + 0.5f * ::sinf( 2.0f * D3DX_PI * t );
    }
};

class KSplineSineExponential : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return t * t + 0.5f * ::sinf( 2.0f * D3DX_PI * t );
    }
};

class KSplineSineLogarithm : public KSpline
{
public:
    virtual float operator()( float t ) const
    {
        return ::sqrtf( t ) + 0.5f * ::sinf( 2.0f * D3DX_PI * t );
    }
};

extern KSplineFlat              g_kSplineFlat;
extern KSplineLinear            g_kSplineLinear;
extern KSplineExponential       g_kSplineExponential;
extern KSplineLogarithm         g_kSplineLogarithm;
extern KSplineSineLinear        g_kSplineSineLinear;
extern KSplineSineExponential   g_kSplineSineExponential;
extern KSplineSineLogarithm     g_kSplineSineLogarithm;

#define SPLINE_FLAT             (&g_kSplineFlat)
#define SPLINE_LINEAR           (&g_kSplineLinear)
#define SPLINE_EXPONENTIAL      (&g_kSplineExponential)
#define SPLINE_LOGARITHM        (&g_kSplineLogarithm)
#define SPLINE_SINE_LINEAR      (&g_kSplineSineLinear)
#define SPLINE_SINE_EXPONENTIAL (&g_kSplineSineExponential)
#define SPLINE_SINE_LOGARITHM   (&g_kSplineSineLogarithm)

#endif // _SPLINE_H_