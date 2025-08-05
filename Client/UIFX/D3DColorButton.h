#ifndef _KD3DCOLOR_BUTTON_H_
#define _KD3DCOLOR_BUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KD3DColorButton : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KD3DColorButton );
	/// Default constructor
	KD3DColorButton( void );
	/// Default destructor
	virtual ~KD3DColorButton( void );
	/// Copy constructor
	KD3DColorButton( const KD3DColorButton& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DColorButton& operator=( const KD3DColorButton& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void PreDraw( void );
	void SetColor(D3DCOLOR color) { m_Color = color; }
	void EnableColor(bool bEnable);
	
protected:
	virtual void OnCreate( void );	

	bool m_bEnableColor;
	D3DCOLOR m_Color;
	RECT m_rtEdge;
	int  m_iFocusShiftX;
	int  m_iFocusShiftY;
};

DEFINE_WND_FACTORY( KD3DColorButton );
DECLARE_WND_FACTORY( KD3DColorButton );
DECLARE_WND_FACTORY_NAME( KD3DColorButton );

#endif // _KD3DCOLOR_BUTTON_H_
