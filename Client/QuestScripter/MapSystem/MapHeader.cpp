#include "stdafx.h"
#include "MapHeader.h"

FRect_Quest::FRect_Quest()
{
    left = 0.0f;
    right = 0.0f;
    top = 0.0f;
    bottom = 0.0f;
}

FRect_Quest::~FRect_Quest()
{

}

void FRect_Quest::SetRect( float _left, float _top, float _right, float _bottom )
{
    left = _left;
    right = _right;
    top = _top;
    bottom = _bottom;
}

void FRect_Quest::Zoom( float Zoom )
{
    float xdelta = 1.0f - ( 1.0f / Zoom );
    float ydelta = 0.75f - ( 1.5f / ( Zoom * 2.0f) );
    left += xdelta;
    right -= xdelta;
    top -= ydelta;
    bottom += ydelta;
}

bool FRect_Quest::PtInRect( float fX, float fY )
{
    return fX >= left && fX <= right && fY >= top && fY <= bottom;
}

bool FRect_Quest::RectInRect( float fMinX, float fMinY, float fMaxX, float fMaxY )
{
    if( left > fMaxX )
        return false;
    if( right < fMinX )
        return false;
    if( bottom > fMaxY )
        return false;
    if( top < fMinY )
        return false;

    return true;
}