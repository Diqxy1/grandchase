#pragma once

class KGCRankIcon;

class KGCRoomExtraOptionRankPanel : public KD3DWnd,    // extends
									public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCRoomExtraOptionRankPanel );
	/// Default constructor
	KGCRoomExtraOptionRankPanel( void );
	/// Default destructor
	virtual ~KGCRoomExtraOptionRankPanel( void );
	/// Copy constructor
	KGCRoomExtraOptionRankPanel( const KGCRoomExtraOptionRankPanel& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomExtraOptionRankPanel& operator=( const KGCRoomExtraOptionRankPanel& )
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
	KSafeArray< KGCRankIcon*, GC_BG_GRADE_MAX> m_saRankIcon;

	bool m_bIsCreatingRoom;
};

DEFINE_WND_FACTORY( KGCRoomExtraOptionRankPanel );
DECLARE_WND_FACTORY( KGCRoomExtraOptionRankPanel );
DECLARE_WND_FACTORY_NAME( KGCRoomExtraOptionRankPanel );
