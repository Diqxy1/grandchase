#pragma once
#include "../uifx/D3DWnd.h"
#include "../uifx/D3DStatic.h"
#include "../uifx/D3DEdit.h"
//#include "../uifx/D3DStaticLink.h"

//#include <map>

class KGCGuildPublicMbox	:	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildPublicMbox );
	/// Default constructor
	KGCGuildPublicMbox( void );
	/// Default destructor
	virtual ~KGCGuildPublicMbox( void );
	/// Copy constructor
	KGCGuildPublicMbox( const KGCGuildPublicMbox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildPublicMbox& operator=( const KGCGuildPublicMbox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum EGuildMboxType
	{
		MBR_CONFIRM_CANCEL,
		MBR_CONFIRM_ONLY,
	};

	enum EGuildPublicBoxResult
	{
		MBR_OK,
		MBR_YES,
		MBR_NO,
	};

    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	//virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	//virtual void PostDraw( void );
	//virtual void OnDestroyComplete( void );

public:

	void SetType( EGuildMboxType eType);
	void SetText( const std::wstring& str1, const std::wstring& str2 );
	void SetUse( DWORD dwUse_ ) { m_dwUse = dwUse_; }
	DWORD GetUse() const { return m_dwUse; }

private:
	KD3DWnd*		m_pkConfirmBtn;
	KD3DWnd*		m_pkCancelBtn;
	KD3DWnd*		m_pkConfirmOnlyBtn;

	KD3DStatic*		m_pkGuildMboxTitle;
	KD3DStatic*		m_pkGuildMboxDesc;

	EGuildMboxType	m_mbox_type;

	DWORD			m_dwUse;
};

DEFINE_WND_FACTORY( KGCGuildPublicMbox );
DECLARE_WND_FACTORY( KGCGuildPublicMbox );
DECLARE_WND_FACTORY_NAME( KGCGuildPublicMbox );
