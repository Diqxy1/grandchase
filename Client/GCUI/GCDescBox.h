#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DSizingBox.h"

class KGCDescBox: public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCDescBox );
	/// Default constructor
	KGCDescBox( void );
	/// Default destructor
	virtual ~KGCDescBox( void );
	/// Copy constructor
	KGCDescBox( const KGCDescBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDescBox& operator=( const KGCDescBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	virtual void OnCreate( void );
	//virtual void OnCreateComplete();
	//virtual void OnDestroy( void );
	//virtual void FrameMoveInEnabledState( void );

	void SetTitle( std::wstring str );
	void SetDesc( std::wstring str );
	void SetMiddleDesc( std::wstring str );
	void SetTargetPosition( D3DXVECTOR2 vPos, DWORD dwTargetWidth, DWORD dwTargetHeight );

private:
	KD3DStatic*		m_pkTitle;
	KD3DStatic*		m_pkDesc;
	KD3DWnd*		m_pkDescBack;
	KD3DStatic*		m_pkMiddleDesc;
	KD3DWnd*		m_pkMiddleDescBack;

	KD3DSizingBox*	m_pkBackground;

	KD3DWnd*		m_pkLeft;
	KD3DWnd*		m_pkRight;

	void RefreshSizeAndPosition();
};

DEFINE_WND_FACTORY( KGCDescBox );
DECLARE_WND_FACTORY( KGCDescBox );
DECLARE_WND_FACTORY_NAME( KGCDescBox );
