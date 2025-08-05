#pragma once

class KGCRoomExtraOptionCharPanel;
class KGCRoomExtraOptionRankPanel;

class KGCRoomListFilterDlg : public KD3DDialog,		// extends
							 public KActionListener // implements
{
public:
	DECLARE_CLASSNAME( KGCRoomListFilterDlg );
	/// Default constructor
	KGCRoomListFilterDlg( void );
	/// Default destructor
	virtual ~KGCRoomListFilterDlg( void );
	/// Copy constructor
	KGCRoomListFilterDlg( const KGCRoomListFilterDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomListFilterDlg& operator=( const KGCRoomListFilterDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum
	{
		MAX_ROOM_OPTION = 5,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );

private:
	void OnClickCharPanelSet();
	void OnClickCloseBtn();
	void OnClickApplyBtn();
	void OnClickResetBtn();

private:
	KD3DWnd* m_pkCloseBtn;
	KD3DWnd* m_pkResetBtn;
	KD3DWnd* m_pkApplyBtn;

	KGCCheckBox* m_pkUsePetCheck;
	KGCCheckBox* m_pkUse4SPCheck;
	KGCCheckBox* m_pkUseBallanceCheck;
	KGCCheckBox* m_pkUseRankCheck;

	KD3DWnd* m_pkCharSetBtn;
	KD3DWnd* m_pkCharModifyBtn;

	KSafeArray<KD3DStatic*, MAX_ROOM_OPTION> m_saExtraOptions;

	KGCRoomExtraOptionCharPanel* m_pkCharPanel;
};

DEFINE_WND_FACTORY( KGCRoomListFilterDlg );
DECLARE_WND_FACTORY( KGCRoomListFilterDlg );
DECLARE_WND_FACTORY_NAME( KGCRoomListFilterDlg );