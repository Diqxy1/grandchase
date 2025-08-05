#pragma once

class KGCRoomListS6;
class KGCAutoMatchingS6;

class KGCMatchWindowS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMatchWindowS6 );
    /// Default constructor
    KGCMatchWindowS6( void );
    /// Default destructor
    virtual ~KGCMatchWindowS6( void );
    /// Copy constructor
    KGCMatchWindowS6( const KGCMatchWindowS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMatchWindowS6& operator=( const KGCMatchWindowS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void OnClickAutoMatchTap( void );
	void OnClickCustomMatchTap( void );
	void OnCreateRoom();
	void OnQuickJoin();
	void EnterScene();
private:
	KGCRoomListS6* m_pkRoomList;
	KGCAutoMatchingS6 *m_pkAutoMatch;

	KD3DWnd* m_pkAutoMatchTapBtn;
	KD3DWnd* m_pkCustomMatchTapBtn;

	
	KD3DWnd* m_pkCreateRoomBtn;
	KD3DWnd* m_pkQuickJoinBtn;

	

};

DEFINE_WND_FACTORY( KGCMatchWindowS6 );
DECLARE_WND_FACTORY( KGCMatchWindowS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchWindowS6 );
