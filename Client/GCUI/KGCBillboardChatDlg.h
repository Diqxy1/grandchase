#pragma once

class KGCDungeonTitleColorS6;

class KGCBillboardChatDlg :
	public KD3DDialog, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCBillboardChatDlg );
	/// Default constructor
	KGCBillboardChatDlg( void );
	/// Default destructor
	virtual ~KGCBillboardChatDlg( void );
	/// Copy constructor
	KGCBillboardChatDlg( const KGCBillboardChatDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCBillboardChatDlg& operator=( const KGCBillboardChatDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void ActionPerformed( const KActionEvent& event ); 

	virtual void FrameMoveInEnabledState();

	virtual void OnCreate( void );
	virtual void OnCreateComplete( void ); 

	void CheckPaint();
	void OnClickColorBtn();

private:
	int m_iMessageMaxLenght;

	KD3DWnd * m_pkBtnConfirm;
	KD3DWnd * m_pkBtnClose;
	KD3DStatic * m_pkStaticLenght;
	KD3DStatic * m_pkStaticAvailableCnt;
	KD3DEdit * m_pkEditMsg;
	KGCDungeonTitleColorS6 * m_pkPaintBtnSet;
	D3DCOLOR m_Color;
	bool m_bInit;

public:
	void InitCreate();
	void OnClose();
	void OnConfirm();
};

DEFINE_WND_FACTORY( KGCBillboardChatDlg );
DECLARE_WND_FACTORY( KGCBillboardChatDlg );
DECLARE_WND_FACTORY_NAME( KGCBillboardChatDlg );