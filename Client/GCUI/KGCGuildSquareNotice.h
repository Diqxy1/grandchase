#pragma once

class KD3DImageIcon;

class KGCGuildSquareNotice	:	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildSquareNotice );
	/// Default constructor
	KGCGuildSquareNotice( void );
	/// Default destructor
	virtual ~KGCGuildSquareNotice( void );
	/// Copy constructor
	KGCGuildSquareNotice( const KGCGuildSquareNotice& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildSquareNotice& operator=( const KGCGuildSquareNotice& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual HRESULT FrameMove( void );
	virtual void PostChildDraw( void );
	virtual void ActionPerformed( const KActionEvent& event );

protected:
	KD3DImageIcon*	m_pkGuildMarkIcon;

public:
	void Close();
};

DEFINE_WND_FACTORY( KGCGuildSquareNotice );
DECLARE_WND_FACTORY( KGCGuildSquareNotice );
DECLARE_WND_FACTORY_NAME( KGCGuildSquareNotice );
