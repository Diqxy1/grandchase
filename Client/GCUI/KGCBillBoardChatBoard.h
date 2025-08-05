#pragma once

class KGCBillBoardChatBoard :
	public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCBillBoardChatBoard );
	/// Default constructor
	KGCBillBoardChatBoard( void );
	/// Default destructor
	virtual ~KGCBillBoardChatBoard( void );
	/// Copy constructor
	KGCBillBoardChatBoard( const KGCBillBoardChatBoard& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCBillBoardChatBoard& operator=( const KGCBillBoardChatBoard& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
 	virtual void OnCreate( void ); 
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState();
	virtual void ActionPerformed( const KActionEvent& event );

public:
	void SetBlink( bool bBlink ) { m_bBlink = bBlink; }
	void ToggleButton( bool bToggle ) { m_bToggle = bToggle; }
	void SetIngamePosition();

private:
	void OnBillboardClick();

private:
	KD3DWnd* m_pkBackNameLeft;
	KD3DWnd* m_pkBackNameCenter;
	KD3DWnd* m_pkBackNameRight;
	KD3DWnd* m_pkWndBillboardNum;
	KD3DSizingBox* m_pkBackBody;
	KD3DStatic* m_pkStaticMsg;
	KD3DStatic* m_pkStaticNickname;
	KD3DStatic* m_pkStaticBillboardNum;
	std::wstring m_strNickName;
	std::wstring m_strMessage;
	bool m_bBlink;
	int m_iItemCount; 
	KD3DWnd* m_pkWndBillboard; //0x350
	bool m_bToggle;

};

DEFINE_WND_FACTORY( KGCBillBoardChatBoard );
DECLARE_WND_FACTORY( KGCBillBoardChatBoard );
DECLARE_WND_FACTORY_NAME( KGCBillBoardChatBoard );