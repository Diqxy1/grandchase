#pragma once

class KD3DStatic;
class KGCGradeMark;

class KGCGuildSquareMenu :	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildSquareMenu );
	/// Default constructor
	KGCGuildSquareMenu( void );
	/// Default destructor
	virtual ~KGCGuildSquareMenu( void );
	/// Copy constructor
	KGCGuildSquareMenu( const KGCGuildSquareMenu& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildSquareMenu& operator=( const KGCGuildSquareMenu& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		MAX_BTN_NUM = 4,
	};

	enum EGuildMenuBtn
	{
		BTN_GUILD_MATCH		= 0,
		BTN_GUILD_INFO		= 1,
		BTN_GUILD_MEMBER	= 2,
		BTN_GUILD_MANAGE	= 3,
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
	KSafeArray<KD3DWnd*,MAX_BTN_NUM>	m_pkTitleMenu;
	KSafeArray<KD3DWnd*,MAX_BTN_NUM>	m_pkBtnMenu;
};

DEFINE_WND_FACTORY( KGCGuildSquareMenu );
DECLARE_WND_FACTORY( KGCGuildSquareMenu );
DECLARE_WND_FACTORY_NAME( KGCGuildSquareMenu );