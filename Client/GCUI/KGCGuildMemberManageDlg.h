#pragma once

class KD3DStatic;
class KGCGradeMark;
class KGCGuildMemberListDlg;

class KGCGuildMemberManageDlg :	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildMemberManageDlg );
	/// Default constructor
	KGCGuildMemberManageDlg( void );
	/// Default destructor
	virtual ~KGCGuildMemberManageDlg( void );
	/// Copy constructor
	KGCGuildMemberManageDlg( const KGCGuildMemberManageDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildMemberManageDlg& operator=( const KGCGuildMemberManageDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum
	{
		MAX_MEMBER_LIST = 8,
	};
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
	KD3DWnd*					m_pkBtnOk;
	KD3DWnd*					m_pkBtnCancel;
	KGCGuildMemberListDlg*		m_pkGuildMemberList;    

public:
	void SetGuildGrade( unsigned char ucUserLevel_ );
	void SetMemberInfo();
};

DEFINE_WND_FACTORY( KGCGuildMemberManageDlg );
DECLARE_WND_FACTORY( KGCGuildMemberManageDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildMemberManageDlg );