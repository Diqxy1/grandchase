#pragma once


class KGCLogoutBox : public KD3DDialog,     
	public KActionListener 
{
public:
	DECLARE_CLASSNAME( KGCLogoutBox );
	/// Default constructor
	KGCLogoutBox( void );
	/// Default destructor
	virtual ~KGCLogoutBox( void );
	/// Copy constructor
	KGCLogoutBox( const KGCLogoutBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCLogoutBox& operator=( const KGCLogoutBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum E_USE
	{
		QUIT,
		CANCEL,
		LOGOUT,
		DETECT_HACK_QUIT,
	};

public:

	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );

	int GetReturn(){return m_iReturn;}

protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );

	void OnClickLogout();
	void OnClickCancel();
	void OnClickQuit();
protected:
	KD3DWnd*    m_pkLogout;
	KD3DWnd*    m_pkQuit;
	KD3DWnd*    m_pkCancel;	

	int		m_iReturn;	
};

DEFINE_WND_FACTORY( KGCLogoutBox );
DECLARE_WND_FACTORY( KGCLogoutBox );
DECLARE_WND_FACTORY_NAME( KGCLogoutBox );
