#pragma once
#include "../uifx/D3DWnd.h"
#include "../uifx/D3DStatic.h"
#include "../uifx/D3DEdit.h"
//#include "../uifx/D3DStaticLink.h"

//#include <map>

class KGCGuildCreateDlg	:	public KD3DDialog,
							public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildCreateDlg );
	/// Default constructor
	KGCGuildCreateDlg( void );
	/// Default destructor
	virtual ~KGCGuildCreateDlg( void );
	/// Copy constructor
	KGCGuildCreateDlg( const KGCGuildCreateDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildCreateDlg& operator=( const KGCGuildCreateDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	//virtual void PostDraw( void );
	//virtual void OnDestroyComplete( void );
 public:
	void SetInfoText( std::wstring& rText, const int index, bool bClear = true );
	bool IsExistBlank(std::wstring& str_);
protected:
	enum{
		LISTBOSGUILDINFO	=	2,
		NUMOFSCROLLBAR		=	3,
	};
	KSafeArray<KD3DListBox*,NUMOFSCROLLBAR>		m_apkGuildCreateDlgInfo;
	KSafeArray<KD3DScrollbar*,NUMOFSCROLLBAR>	m_apkGuildCreateDlgScroll;
	
	KD3DListBox*	m_pkGuildCreateInfoDesc;
	KD3DStatic*		m_pkGuildCreateInfoName;

	KD3DEdit*		m_pkGuildCreateNameEdit;
	KD3DEdit*		m_pkGuildCreateURLEdit;
#ifdef NATION_KOREA
	KD3DStatic*		m_pkGuildCreateURLStatic;
#endif
	KD3DWnd*		m_pkConfirmBtn;
	KD3DWnd*		m_pkCancelBtn;
};

DEFINE_WND_FACTORY( KGCGuildCreateDlg );
DECLARE_WND_FACTORY( KGCGuildCreateDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildCreateDlg );
