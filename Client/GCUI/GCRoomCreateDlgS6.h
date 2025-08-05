#pragma once

class KGCRoomListBtnSetS6;
class KGCDungeonTitleColorS6;
class KGCRoomCreateDlgS6 : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomCreateDlgS6 );
    /// Default constructor
    KGCRoomCreateDlgS6( void );
    /// Default destructor
    virtual ~KGCRoomCreateDlgS6( void );
    /// Copy constructor
    KGCRoomCreateDlgS6( const KGCRoomCreateDlgS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomCreateDlgS6& operator=( const KGCRoomCreateDlgS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KDSI_BTN_SET_0 = 0,
		KDSI_BTN_SET_1,
		KDSI_BTN_SET_2,
		KDSI_BTN_SET_3,
		KDSI_BTN_SET_4,
		KDSI_BTN_SET_5,
		KDSI_BTN_SET_NUM,
	};
	
	enum
	{
#if defined( NATION_PHILIPPINE )
        MAX_EDIT_LEN = 26,
#else
		MAX_EDIT_LEN = 16,
#endif
		MAX_PASSWOD_EDIT_LEN = 14,
	};
public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void UpdateState( void );
	void RenderBtnSet( int index );
	void OnClickBtnClose( );
	void OnCreateRoom( void );
	void OnClickColorBtn( );

	void CheckPaint();
private:
	KSafeArray<KGCRoomListBtnSetS6*, KDSI_BTN_SET_NUM> m_saBtnSet;
	KGCDungeonBossInfoS6 *m_pkBossInfo;
	KGCDungeonTitleColorS6 *m_pkColorBtnSet;

	KD3DWnd *m_pkCreateRoom;
	KD3DEdit *m_pkRoomName;
	KD3DEdit *m_pkRoomPassword;
	KD3DWnd *m_pkClose;

	KD3DSizingBox *m_pkLevelBackGround;
	KD3DWnd *m_pkLevelTitle;

	KD3DWnd *m_pkPaintTitle;

};

DEFINE_WND_FACTORY( KGCRoomCreateDlgS6 );
DECLARE_WND_FACTORY( KGCRoomCreateDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomCreateDlgS6 );

class KGCRoomQuickJoinDlgS6 : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCRoomQuickJoinDlgS6 );
	/// Default constructor
	KGCRoomQuickJoinDlgS6( void );
	/// Default destructor
	virtual ~KGCRoomQuickJoinDlgS6( void );
	/// Copy constructor
	KGCRoomQuickJoinDlgS6( const KGCRoomQuickJoinDlgS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomQuickJoinDlgS6& operator=( const KGCRoomQuickJoinDlgS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum OPTION_TYPE {
		OT_USER_NUM = 0,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void OnClickQuickJoin( void );
	void Close( );
private:
	KGCDungeonBossInfoS6 *m_pkBossInfo;
	KD3DWnd *m_pkJoinRoomBtn;
	KD3DWnd *m_pkCloseBtn;
	std::map<DWORD,DWORD> m_mapOption;
	KD3DComboBox*		m_pkUserCombo;

};

DEFINE_WND_FACTORY( KGCRoomQuickJoinDlgS6 );
DECLARE_WND_FACTORY( KGCRoomQuickJoinDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomQuickJoinDlgS6 );
