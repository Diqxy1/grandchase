#include "stdafx.h"
#include ".\kgcmath.h"
#include <math.h>
#define _USE_MATH_DEFINES
//
//

ImplementSingleton(KGCMath);

KGCMath::KGCMath(void)
{
}

KGCMath::~KGCMath(void)
{
}

float KGCMath::f( int iStyle, float x )
{
	switch( iStyle )
	{
		case EP_LINEAR:		return	x;
		case EP_SLOWER:		return	QuadraticFunction( x, -1.0f, 1.0f, 1.0f );
		case EP_FASTER:		return	QuadraticFunction( x );
		case EP_SPRING:		return	1.0f - SpringFunction( 0.09f, 0.042f, 8.3f, 2.78f, x );
		case EP_SPRING2:	return	SpringFunction( 0.09f, 0.42f, 8.3f, 2.78f, 1.0f - x );
		case EP_VIBRATE:	return	VibrateFunction( 1.2f, 0.085f, 0.8f, 0.09f, 0.775f, 15.0f, x );
		case EP_CLOUD:		return	sin( x * 2 * (float)M_PI );
		case EP_OVER:		return	-2.5f * powf( x, 2 ) + 3.5f * x;
		case EP_OVER2:		return	2.5f * powf( x, 2 ) - 1.5f * x;
		case EP_SINCOSCOS:	return  sin( 2.0f * (float)M_PI * x ) * cos( 0.9f * x ) * cos( 2.1f * x );
		default:
			break;
	}
	return x;
}

float KGCMath::QuadraticFunction( float x, float a, float b, float c )
{
	return powf( a * ( x - b ) , 2.0f ) + c;
}

float KGCMath::SpringFunction( float t, float g, float w, float e, float x )
{
	t += x;

	return powf( e, -g*w*t ) * (cos( ASMsqrt( 1-g*g ) * w * t ) + sin( ASMsqrt( 1-g*g ) * w * t ) );
}

float KGCMath::VibrateFunction( float a, float b, float c, float d, float g, float w, float x )
{
	return c * powf( a, -g*w*(x - b) ) * (cos ( ASMsqrt( 1 - powf( g, 2 ) ) * w * ( x - b ) ) + sin( ASMsqrt( 1 - powf( g, 2 ) ) * w * ( x - b ) ) );
}

void KGCMath::BindLua()
{

	lua_tinker::class_add<KGCMath>( KGCLuabinder::getInstance().GetLuaState(), "KGCMathClass" );
    lua_tinker::class_def<KGCMath>( KGCLuabinder::getInstance().GetLuaState(), "func", 			&KGCMath::f  );
    lua_tinker::class_def<KGCMath>( KGCLuabinder::getInstance().GetLuaState(), "mod", 			&KGCMath::Mod  );

	lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "KGCMath", this );
}

int KGCMath::Mod( int v, int i )
{
    return v%i;
}