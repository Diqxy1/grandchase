#pragma once

class KGCRoomListS6;
class KGCDungeonInfoS6;
class KGCChampionPopup;

class KGCRoomListDlgS6 : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomListDlgS6 );
    /// Default constructor
    KGCRoomListDlgS6( void );
    /// Default destructor
    virtual ~KGCRoomListDlgS6( void );
    /// Copy constructor
    KGCRoomListDlgS6( const KGCRoomListDlgS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomListDlgS6& operator=( const KGCRoomListDlgS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
	void OnClickBtnClose( void );
	void OnClickBtnCreateRoom( void );
	void OnClickBtnQuickJoinRoom( void );
	void OnClickBtnDungeonSelect();
private:
	KGCRoomListS6    *m_pkRoomList;
	KGCDungeonInfoS6 *m_pkDungeonInfo;
	KD3DWnd          *m_pkCloseBtn;
	KD3DWnd          *m_pkCreateRoomBtn;
	KD3DWnd          *m_pkQuickJoinBtn;
	KD3DWnd          *m_pkDungeonSelBtn;
    KD3DWnd          *m_pkHelpBtn;
    KD3DWnd          *m_pkHeroHelpText;
    KGCChampionPopup *m_pkChampionPopup;
};

DEFINE_WND_FACTORY( KGCRoomListDlgS6 );
DECLARE_WND_FACTORY( KGCRoomListDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomListDlgS6 );
