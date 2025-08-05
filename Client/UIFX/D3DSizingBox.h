#ifndef _KD3DSizingBox_H_
#define _KD3DSizingBox_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

enum
{	
	SBC_LEFT_TOP,
	SBC_TOP,
	SBC_RIGHT_TOP,
	SBC_LEFT,
	SBC_CENTER,	
	SBC_RIGHT,
	SBC_LEFT_BOTTOM,	
	SBC_BOTTOM,
	SBC_RIGHT_BOTTOM,
	
	SIZINGBOX_COMPONENT_NUM,
};

class KD3DSizingBox : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KD3DSizingBox );
	/// Default constructor
	KD3DSizingBox( void );
	/// Default destructor
	virtual ~KD3DSizingBox( void );
	/// Copy constructor
	KD3DSizingBox( const KD3DSizingBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DSizingBox& operator=( const KD3DSizingBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	virtual void SetSize(DWORD iWidth, DWORD iHeight);
	void SetSizeWithoutEdge( int iWidth, int iHeight );
	virtual void OnSetWidth( DWORD dwWidth );
    virtual void OnSetHeight( DWORD dwHeight );
    void SetWidthWithoutEdge(int iWidth );
    void SetHeightWithoutEdge( int iHeight );
	void ForceSetWindowColor( D3DCOLOR dwWndColor );

	void SetPosWithoutEdge(D3DXVECTOR2 vPos_);
protected:
	virtual void OnCreate( void );

	KSafeArray<KD3DWnd*,SIZINGBOX_COMPONENT_NUM> m_pBoxComponent;
};

DEFINE_WND_FACTORY( KD3DSizingBox );
DECLARE_WND_FACTORY( KD3DSizingBox );
DECLARE_WND_FACTORY_NAME( KD3DSizingBox );

#endif // _KD3DSizingBox_H_
