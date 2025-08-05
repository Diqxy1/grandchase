#pragma once


class KGCUserMigrationDlg : public KD3DDialog,     
	public KActionListener 
{
public:
	DECLARE_CLASSNAME( KGCUserMigrationDlg );
	/// Default constructor
	KGCUserMigrationDlg( void );
	/// Default destructor
	virtual ~KGCUserMigrationDlg( void );
	/// Copy constructor
	KGCUserMigrationDlg( const KGCUserMigrationDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCUserMigrationDlg& operator=( const KGCUserMigrationDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );

    void SendUserInfo();
    void OnCancel();

protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );

protected:
	KD3DWnd*    m_pkBtnOk;
	KD3DWnd*    m_pkBtnCancel;

    KD3DStatic* m_pkStaticTitle;
    KD3DStatic* m_pkStaticLoginID;
    KD3DStatic* m_pkStaticEmail;
    KD3DStatic* m_pkStaticNickName;
    KD3DStatic* m_pkStaticBirthDay;

    KD3DEdit*   m_pkEditLoginID;
    KD3DEdit*   m_pkEditEmail;
    KD3DEdit*   m_pkEditNickname;
    KD3DEdit*   m_pkEditDay;
    KD3DEdit*   m_pkEditMonth;
    KD3DEdit*   m_pkEditYear;

};

DEFINE_WND_FACTORY( KGCUserMigrationDlg );
DECLARE_WND_FACTORY( KGCUserMigrationDlg );
DECLARE_WND_FACTORY_NAME( KGCUserMigrationDlg );
