#pragma once

class KD3DStaticBtn :	public KD3DWnd // extends
{
public:
	DECLARE_CLASSNAME( KD3DStaticBtn );
	/// Default constructor
	KD3DStaticBtn( void );
	/// Default destructor
	virtual ~KD3DStaticBtn( void );
	/// Copy constructor
	KD3DStaticBtn( const KD3DStaticBtn& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DStaticBtn& operator=( const KD3DStaticBtn& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void OnCreate( void );
	void SetText( std::wstring strText );
	void SetText( int iText );

private:
	KD3DStatic*	m_pkStatic;
};

DEFINE_WND_FACTORY( KD3DStaticBtn );
