#pragma once

class KGCHotEventBtn : public KD3DWnd    // extends
{
public:
	DECLARE_CLASSNAME( KGCHotEventBtn );
	/// Default constructor
	KGCHotEventBtn( void );
	/// Default destructor
	virtual ~KGCHotEventBtn( void );
	/// Copy constructor
	KGCHotEventBtn( const KGCHotEventBtn& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCHotEventBtn& operator=( const KGCHotEventBtn& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void OnDestroy( void );
	virtual void FrameMoveInEnabledState();
	virtual void BindToLua();

	static void	SetHotStateOn();
	static void	SetHotStateOff();

private:
	void OnClickBtn();
	void OnCursorEnter();

private:
	KD3DWnd*	m_pkAniOn;
	KD3DWnd*	m_pkAniOff;

private:
	static bool		m_bHotState;
};

DEFINE_WND_FACTORY( KGCHotEventBtn );
DECLARE_WND_FACTORY( KGCHotEventBtn );
DECLARE_WND_FACTORY_NAME( KGCHotEventBtn );
