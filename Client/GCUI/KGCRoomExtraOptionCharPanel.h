#pragma once

class KGCCharIcon;

class KGCRoomExtraOptionCharPanel : public KD3DWnd,    // extends
									public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCRoomExtraOptionCharPanel );
	/// Default constructor
	KGCRoomExtraOptionCharPanel( void );
	/// Default destructor
	virtual ~KGCRoomExtraOptionCharPanel( void );
	/// Copy constructor
	KGCRoomExtraOptionCharPanel( const KGCRoomExtraOptionCharPanel& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomExtraOptionCharPanel& operator=( const KGCRoomExtraOptionCharPanel& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );

	void IsCreatingRoom( bool bIsCreating ) { m_bIsCreatingRoom = bIsCreating; }

private:
	void OnClickIcon( int iIndex );
	void OnClose();
	void OnReset();
	void OnOK();

private:
	KD3DWnd* m_pkCloseBtn;
	KD3DWnd* m_pkResetBtn;
	KD3DWnd* m_pkOkBtn;
	KSafeArray< KGCCharIcon*, GC_CHAR_NUM> m_saCharactersIcon;

	bool m_bIsCreatingRoom;
};

DEFINE_WND_FACTORY( KGCRoomExtraOptionCharPanel );
DECLARE_WND_FACTORY( KGCRoomExtraOptionCharPanel );
DECLARE_WND_FACTORY_NAME( KGCRoomExtraOptionCharPanel );
