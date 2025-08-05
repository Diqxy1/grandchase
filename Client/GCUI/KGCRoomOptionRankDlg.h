#pragma once

class KGCRoomOptionRankDlg : public KD3DDialog,
							 public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRoomOptionRankDlg );
	/// Default constructor
	KGCRoomOptionRankDlg( void );
	/// Default destructor
	virtual ~KGCRoomOptionRankDlg( void );
	/// Copy constructor
	KGCRoomOptionRankDlg( const KGCRoomOptionRankDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomOptionRankDlg& operator=( const KGCRoomOptionRankDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	virtual void OnCreate();
	virtual bool CheckCondition( IN const KDialogInfo& kInfo_ );
	virtual void ActionPerformed( const KActionEvent& kEvent );
	virtual void OnCreateComplete();

public:
	void OnClickBtn();

private:
	void UpdateGradeSlot();

private:
	KD3DWnd* m_pkCloseBtn;
	KD3DWnd* m_pkOkBtn;

	DWORD m_dwAllowedRanks;

	KSafeArray< KD3DWnd*, GC_BG_GRADE_MAX > m_saGrades;
};

DEFINE_WND_FACTORY( KGCRoomOptionRankDlg );
DECLARE_WND_FACTORY( KGCRoomOptionRankDlg );
DECLARE_WND_FACTORY_NAME( KGCRoomOptionRankDlg );