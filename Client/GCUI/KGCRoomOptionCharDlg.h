#pragma once

class KGCCharIcon;

class KGCRoomOptionCharDlg : public KD3DDialog,
							 public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRoomOptionCharDlg );
	/// Default constructor
	KGCRoomOptionCharDlg( void );
	/// Default destructor
	virtual ~KGCRoomOptionCharDlg( void );
	/// Copy constructor
	KGCRoomOptionCharDlg( const KGCRoomOptionCharDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomOptionCharDlg& operator=( const KGCRoomOptionCharDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	virtual void OnCreate();
	virtual bool CheckCondition( IN const KDialogInfo& kInfo_ );
	virtual void ActionPerformed( const KActionEvent& kEvent );
	virtual void OnCreateComplete();

public:
	void OnClickBtn();

private:
	void UpdateCharSlot();

private:
	KD3DWnd* m_pkCloseBtn;
	KD3DWnd* m_pkOkBtn;

	DWORD m_dwAllowedCharacters;

	KSafeArray< KGCCharIcon*, GC_CHAR_NUM > m_saAllowedCharacters;
};

DEFINE_WND_FACTORY( KGCRoomOptionCharDlg );
DECLARE_WND_FACTORY( KGCRoomOptionCharDlg );
DECLARE_WND_FACTORY_NAME( KGCRoomOptionCharDlg );