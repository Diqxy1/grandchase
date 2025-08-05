#include "stdafx.h"
#include "GCCollisionRect.h"



FRect::FRect()
{
	left = 0.0f;
	right = 0.0f;
	top = 0.0f;
	bottom = 0.0f;
}

FRect::~FRect()
{

}

void FRect::SetRect( float _left, float _top, float _right, float _bottom )
{
	left = _left;
	right = _right;
	top = _top;
	bottom = _bottom;
}

void FRect::Zoom( float Zoom )
{
	float xdelta = 1.0f - ( 1.0f / Zoom );
	float ydelta = 0.75f * xdelta; // 수식을 정리 
	left += xdelta;
	right -= xdelta;
	top -= ydelta;
	bottom += ydelta;
}

bool FRect::PtInRect( float fX, float fY )
{
	return fX >= left && fX <= right && fY >= top && fY <= bottom;
}

bool FRect::RectInRect( float fMinX, float fMinY, float fMaxX, float fMaxY )
{
	if( left > fMaxX  )
		return false;
	if( right < fMinX )
		return false;
	if( bottom > fMaxY )
		return false;
	if( top < fMinY )
		return false;

	return true;
}

void FRect::AdjustPtInRect( float& fX, float& fY, float fDelta )
{
	float fXMergin, fYMergin;
	fXMergin = ( right - left ) * fDelta;
	fYMergin = ( top - bottom ) * fDelta;
	if( fX > right - fXMergin )
		fX = right - fXMergin;
	else if( fX < left + fXMergin )
		fX = left + fXMergin;

	if( fY < bottom + fYMergin)
		fY = bottom + fYMergin;	
	else if( fY > top - fYMergin)
		fY = top - fYMergin;	
}

