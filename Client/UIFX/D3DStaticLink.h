#pragma once
//#include "D3DWnd.h"
//#include "D3DStatic.h"
//#include <list>
//#include <string>

class KD3DStaticLink :	public KD3DWnd,           // extends
						public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KD3DWnd );
	/// Default constructor
	KD3DStaticLink( void );
	/// Default destructor
	virtual ~KD3DStaticLink( void );
	/// Copy constructor
	KD3DStaticLink( const KD3DStaticLink& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DStaticLink& operator=( const KD3DStaticLink& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		SL_ELEMENT_CLICK = 1,
	};

public:
	void ActionPerformed( const KActionEvent& event );
	void OnCreate( void );
	void FrameMoveInEnabledState( void );

public:
	int AddLinkString( std::wstring strLink, bool bFixedSize = false, int iFixSize = 20 );
	int RemoveLink( int iIndex );
	void Clear();
	void RefreshStringPos();

	void SetDefaultColor( D3DCOLOR color ) { m_dwDefColor = color; };
	void SetHoverColor( D3DCOLOR color ) { m_dwHovColor = color; };

    void SelectLink( std::wstring strLink );

private:
	std::list< KD3DStatic* > m_listLinks;

	D3DCOLOR m_dwDefColor;
	D3DCOLOR m_dwHovColor;

    KD3DStatic* m_pCurSelect;
    D3DCOLOR m_dwSelColor;
};

DEFINE_WND_FACTORY( KD3DStaticLink );
